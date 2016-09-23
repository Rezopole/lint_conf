
// {
//
//  lint_conf, checks for missing, unusued, etc, declarations in switches and routers configurations, and more ...
//  Copyright (C) 2016 Jean-Daniel Pauget <jdpauget@rezopole.net>
//  
//  This program is free software; you can redistribute it and/or
//  modify it under the terms of the GNU General Public License
//  as published by the Free Software Foundation; either version 2
//  of the License, or (at your option) any later version.
//  
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//  
//  You should have received a copy of the GNU General Public License
//  along with this program; if not, write to the Free Software
//  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
//  
//
// }



// TODO :
//
//  set comm-list Local-Prefixes delete    <Local-Prefixes>


#include <iostream>
#include <iomanip>
#include <fstream>

#include <string>
#include <map>
#include <vector>

using namespace std;


// ----------------- readline -----------------------------------------------------------

void readline (istream &cin, string &s) {
    while (cin) {
	char c;
	size_t n=0;
	while (cin && cin.get(c) && (c!=10) && (c!=13)) {
	    if (c==9) {
		int nn = 8 - (n%8);
		for (int i=0 ; i<nn ; i++) s+=' ', n++;
	    } else {
		s+=c, n++;
	    }
	}
	if (!cin) return;
	if (c==10) {
	    if (cin.get(c) && (c!=13)) {
		cin.unget();
	    }
	    return;
	}
	if (c==13) {
	    if (cin.get(c) && (c!=10)) {
		cin.unget();
	    }
	    return;
	}
    }
}


// ----------------- maigre -------------------------------------------------------------

string & maigre (const string &in, string &out) {
    size_t p = 0, l=in.size();
    while ((p<l) && (isspace(in[p]))) p++;
    size_t q = l-1;
    while ((q>p) && (isspace(in[q]))) q--;
    if (q>p)
	out = in.substr (p, q-p+1);
    else
	out.clear();
    return out;
}


// ----------------- cut collate_cut ----------------------------------------------------

void cut (const string &s, char delim, size_t field_no, string &result) {
    size_t p, q, n;

    for (p=0,n=0 ; ; ) {
	q = s.find (delim, p);
	n++;
	if (q == string::npos) break;
	if (n == field_no) break;
	p = q+1;
    }
    if (n == field_no) {
	if (q == string::npos) {
	    result = s.substr (p);
	} else {
	    result = s.substr (p, q-p);
	}
    } else {
	result.clear();
    }
}


void collate_cut (const string &s, char delim, size_t field_no, string &result) {
    size_t p=0, q, n;
    while ((p<s.size()) && (s[p]==delim)) p++;
    for (n=0 ; ; ) {
	q = s.find (delim, p);
	n++;
	if (q == string::npos) break;
	if (n == field_no) break;
	while ((q<s.size()) && (s[q] == delim)) q++;
	if (q>=s.size()) { q=string::npos; break; }
	p = q;
    }
    if (n == field_no) {
	if (q == string::npos) {
	    result = s.substr (p);
	} else {
	    result = s.substr (p, q-p);
	}
    } else {
	result.clear();
    }
}

// ----------------- charcute -----------------------------------------------------------

size_t charcute (const string& s, vector<string> &v) {
    size_t p=0, l=s.size(), q; l=s.size();
    v.clear();
    while (p<l) {
	q = p;
	while ((p<l) && (!isspace(s[p]))) p++;
	if (p>q) {
	    v.push_back (s.substr(q,p-q));
	}
	while ((p<l) && (isspace(s[p]))) p++;
    }
    return v.size();
}

// ----------------- Tcutstring ---------------------------------------------------------

class Tcutstring : public vector<string> {
  public:
    vector<string> &v;
    Tcutstring (const string & s) : v(*this) {
	charcute (s, *this);
    }
    bool match (const char *m) {
	if ((size() >= 1) && (m == v[0])) return true;
	return false;
    }
    bool match (const char *m0, const char *m1) {
	if ((size() >= 2) && (m0 == v[0]) && (m1 == v[1])) return true;
	return false;
    }
    bool match (const char *m0, const char *m1, const char *m2) {
	if ((size() >= 3) && (m0 == v[0]) && (m1 == v[1]) && (m2 == v[2])) return true;
	return false;
    }
};

// ----------------- OutputMode ---------------------------------------------------------

typedef enum {
    ALL_EXIST_AND_ERRORS,   // displays every reported ID and errors
    ERRORS_ONLY		    // displays only ID involved in errors
} TOutputMode;

TOutputMode outputmode = ERRORS_ONLY;


// ----------------- IDPoolEntry --------------------------------------------------------

class IDPoolEntry {
  public:
    string id;			// identifier
    bool allowmdecl;
    
    vector<size_t> ldecl;	// line(s) of (re)declaration
    vector<size_t> luse;	// lines of use;

    IDPoolEntry (const string &id, bool allowmdecl = false) : id(id), allowmdecl(allowmdecl) {}
    void adduse  (size_t l) {  luse.push_back (l); }
    void adddecl (size_t l) { ldecl.push_back (l); }
};

ostream &operator<< (ostream &out, const IDPoolEntry &e) {
// cerr << "ici " << endl;
// cerr << "ici " << e.id << endl;
    bool undeclared = e.ldecl.empty() && (!e.luse.empty());
    bool anormal_mdecl = (e.ldecl.size() > 1) && (!e.allowmdecl);
    bool allempty = e.ldecl.empty() && e.luse.empty();
    // bool unused = e.luse.empty();
    bool errorinvolved = undeclared || anormal_mdecl || allempty ; // || unused;

    if ((outputmode == ERRORS_ONLY) && (!errorinvolved))
	return out;

    out << "   " << e.id;
    if (e.ldecl.empty()) {
	out << " undeclared";
	undeclared = true;
    } else if ((e.ldecl.size() > 1) && (!e.allowmdecl)) {
	out << " multiple declarations ( ";
	for (size_t i=0 ; i<e.ldecl.size() ; i++)
	    out << e.ldecl[i] << " ";
	out << ")";
    }

    if (e.luse.empty()) {
	out << " unused";
    } else if (undeclared) {
	out << " use ( ";
	for (size_t i=0 ; i<e.luse.size() ; i++)
	    out << e.luse[i] << " ";
	out << ")";
    }
    return out << endl;
}

// ----------------- IDPool -------------------------------------------------------------

class IDPool : public map<string, IDPoolEntry*> {
  public:
    string name;    // object name
    bool allowmdecl;
    IDPool (const string & s) : name(s), allowmdecl(false) {}
    IDPool (const char *s) : name(s) {}
    bool addentry (const string &id) {
	IDPool::iterator mi = find (id);
	IDPool & me = *this;
	if (mi == end()) {
	    IDPoolEntry *pid = new IDPoolEntry (id, allowmdecl);
	    if (pid == NULL) {
		cerr << "ERROR: could not allocate a new IDPoolEntry !" << endl;
		return false;
	    }
	    me[id] = pid;
	}
	return true;
    }
    bool adduse (const string id, size_t l) {
	if (!addentry (id)) return false;
	IDPool & me = *this;
	me[id]->adduse (l);
	return true;
    }
    bool adddecl (const string id, size_t l) {
	if (!addentry (id)) return false;
	IDPool & me = *this;
	me[id]->adddecl (l);
	return true;
    }
};
ostream &operator<< (ostream &out, const IDPool &idp) {
//cerr << "there" <<  endl;
    if (idp.empty()) return out;
    out << idp.name << " [" << idp.size() << "] :" << endl;
    IDPool::const_iterator mi;
    for (mi=idp.begin() ; mi!=idp.end() ; mi++)
	out << *(mi->second);
    return out;
}

// ----------------- IDPoolMap ----------------------------------------------------------

class IDPoolMap : public map<string, IDPool*> {
  public:
    string fname;

    IDPoolMap (const string &fname) : fname(fname) {}

    bool createpool (const string &s) {
	iterator mi = find(s);
	if (mi == end()) {
	    IDPoolMap & me = *this;
	    IDPool *pidp = new IDPool(s);
	    if (pidp == NULL) {
		cerr << "ERROR: could not allocate a new IDPool !" << endl;
		return false;
	    }
	    me[s] = pidp;
	}
	return true;
    }
    bool addentry_decl (const string &poolname, const string &id, size_t l) {
	if (!createpool (poolname)) return false;
	IDPoolMap & me = *this;
	IDPool & thepool = *me[poolname];
	return thepool.adddecl (id, l);
    }
    bool addentry_use (const string &poolname, const string &id, size_t l) {
	if (!createpool (poolname)) return false;
	IDPoolMap & me = *this;
	IDPool & thepool = *me[poolname];
	return thepool.adduse (id, l);
    }
    bool allow_multiple_decl (const string &poolname, bool allowmdecl=true) {
	if (!createpool (poolname)) return false;
	IDPoolMap & me = *this;
	IDPool & thepool = *me[poolname];
	thepool.allowmdecl = allowmdecl;
	return true;
    }
};
ostream &operator<< (ostream &out, const IDPoolMap &idpm) {
//cerr << "here" <<  endl;
    if (idpm.empty()) {
	if (idpm.fname.size() != 0) {
	    return out << idpm.fname << ": no identifiers" << endl;
	} else {
	    return out << "no identifiers" << endl;
	}
    }
    IDPoolMap::const_iterator mi;
    for (mi=idpm.begin() ; mi!=idpm.end() ; mi++) {
	if (idpm.fname.size() != 0) {
	    out << idpm.fname << ": " << *(mi->second);
	} else {
	    out << *(mi->second);
	}
    }
    return out;
}


// ----------------- Tstatemode ---------------------------------------------------------

typedef enum {
    SEEK_BEGINCONF,		// where looking for the "Building configuration..." mark
				// at the begining of dumped conf ...
    AT_ROOT,			// anywhere at root, ready for any configuration command
    
    UNKOWN
} Tstatemode;

// ----------------- filter_in ----------------------------------------------------------

int filter_in (istream & cin, const string &fname) {
    Tstatemode state = SEEK_BEGINCONF;
    IDPoolMap idpm (fname);

    idpm.allow_multiple_decl("acl");
    idpm.allow_multiple_decl("port-channel");
    idpm.allow_multiple_decl("route-map");
    idpm.allow_multiple_decl("ipv4 prefix-list");
    idpm.allow_multiple_decl("ipv6 prefix-list");
    idpm.allow_multiple_decl("community-list");

    vector <string> rawconf;
    size_t curline = 0;
    string address_family = "";


    while (cin) {
	string rs, s;
	readline (cin, rs);
	rawconf.push_back(s);
	curline ++;	// we call the first line "line no 1"
	maigre(rs, s);

	// we skip comments and empty lines
	if (s.empty() || (s[0] == '!'))
	    continue;

	Tcutstring cs (s);

	// vector<string> vid;
	// charcute (s, vid);

	if (cs.empty()) {
	    cerr << "ERROR : BUG : line=" << curline << "  [" << rs << "]    empty vector<string> ???" << endl;
	    continue; 
	}

// debug de charcute
// cerr << s << endl;
// for (size_t i=0 ; i<cs.size() ; i++) cerr << "      [" << cs[i] << "]" << endl;


// cerr << s << endl;

	switch (state) {
	    case SEEK_BEGINCONF:
		if ("Building configuration..." == rs) {
		    // cerr << "entering conf" << endl;
		    state = AT_ROOT;
		    address_family = "";
		}
		break;
	    case AT_ROOT:

if (cs.match ("vrf")) {
    if (cs.match ("vrf", "definition")) {
	if (cs.size() >= 3) idpm.addentry_decl ("vrf", cs[2], curline);
    } else if (cs.match("vrf", "forwarding")) {
	if (cs.size() >= 3) idpm.addentry_use ("vrf", cs[2], curline);
    }
} else if (cs.match ("interface")) {
    if ((cs.size() >= 2) && (cs[1].substr (0, 12) == "Port-channel")) {
	string pc = cs[1].substr(12);
	size_t p = pc.find('.');
	if (p == string::npos)
	    idpm.addentry_decl ("port-channel", pc, curline);
	else
	    idpm.addentry_decl ("port-channel", pc.substr(0, p), curline);
    } else if (cs.size() >= 2) {
	idpm.addentry_decl ("interface", cs[1], curline);
    }
} else if (cs.match ("channel-group")) {
    if (cs.size() >=2) {
	idpm.addentry_use ("port-channel", cs[1], curline);
    }
} else if (cs.match ("router")) {
    if (cs.match ("router", "ospf")) {
	if (cs.size() >= 3) idpm.addentry_decl ("router ospf", cs[2], curline);
    } else if (cs.match ("router", "ospfv3")) {
	if (cs.size() >= 3) idpm.addentry_decl ("router ospfv3", cs[2], curline);
    } else if (cs.match ("router", "bgp")) {
	if (cs.size() >= 3) idpm.addentry_decl ("router bgp", cs[2], curline);
    }
} else if (cs.match ("neighbor")) {
    if ((cs.size() == 3) && (cs[2] == "peer-group")) {
	idpm.addentry_decl ("peer group", cs[1], curline);
    } else if ((cs.size() == 4) && (cs[2] == "peer-group")) {
	idpm.addentry_use ("peer group", cs[3], curline);
    } else if ((cs.size() == 5) && (cs[2] == "route-map")) {
	idpm.addentry_use ("route-map", cs[3], curline);
    } else if ((cs.size() == 5) && (cs[2] == "prefix-list")) {
	if (address_family == "ipv4") {
	    idpm.addentry_use ("ipv4 prefix-list", cs[3], curline);
	} else if (address_family == "ipv6") {
	    idpm.addentry_use ("ipv6 prefix-list", cs[3], curline);
	}
    }
} else if (cs.match ("route-map")) {
    idpm.addentry_decl ("route-map", cs[1], curline);
} else if (cs.match ("ip", "policy", "route-map")) {
    if (cs.size() >= 4) {
	idpm.addentry_use ("route-map", cs[3], curline);
    }
} else if (cs.match ("ipv6", "policy", "route-map")) {
    if (cs.size() >= 4) {
	idpm.addentry_use ("route-map", cs[3], curline);
    }
} else if (cs.match ("ip", "prefix-list")) {
    idpm.addentry_decl ("ipv4 prefix-list", cs[2], curline);
} else if (cs.match ("ipv6", "prefix-list")) {
    idpm.addentry_decl ("ipv6 prefix-list", cs[2], curline);
} else if (cs.match ("address-family")) {
    if (cs.match ("address-family", "ipv4")) {
	address_family = "ipv4";
    } else if (cs.match ("address-family", "ipv6")) {
	address_family = "ipv6";
    } else if (cs.match ("exit-address-family")) {
	address_family = "";
    }
} else if (cs.match ("access-list")) {
    if ((cs.size() >= 3) && ((cs[2] == "permit") || (cs[2] == "deny"))) {
	idpm.addentry_decl ("acl", cs[1], curline);
    }
} else if (cs.match ("access-class")) {
    idpm.addentry_use ("acl", cs[1], curline);
} else if (cs.match ("snmp-server", "community")) {
    if ((cs.size() >= 5) && ((cs[3] == "RO"))) {
	idpm.addentry_use ("acl", cs[4], curline);
    }
} else if (cs.match ("ip", "community-list", "standard")) {
    if (cs.size() >= 3) {
	idpm.addentry_decl ("community-list", cs[3], curline);
    }
} else if (cs.match ("ip", "community-list", "expanded")) {
    if (cs.size() >= 3) {
	idpm.addentry_decl ("community-list", cs[3], curline);
    }
} else if (cs.match ("match", "community")) {
    if (cs.size() >= 3) {
	idpm.addentry_use ("community-list", cs[2], curline);
    }
} else if (cs.match ("set", "comm-list")) {
    if (cs.size() >= 4) {
	idpm.addentry_use ("community-list", cs[2], curline);
    }
} else if (cs.match ("ipv6")) {
    if (cs.match ("ipv6", "access-list")) {
	if (cs.size() >= 3) {
	    idpm.addentry_decl ("ipv6 access-list", cs[2], curline);
	}
    } else if (cs.match ("ipv6", "traffic-filter")) {
	if (cs.size() >= 3) {
	    idpm.addentry_use ("ipv6 access-list", cs[2], curline);
	}
    } else if (cs.match ("ipv6", "access-class")) {
	if (cs.size() >= 3) {
	    idpm.addentry_use ("ipv6 access-list", cs[2], curline);
	}
    }
} else if (cs.match ("match", "ipv6", "address")) {
    if (cs.size() >= 4) {
	    idpm.addentry_use ("ipv6 access-list", cs[3], curline);
    }
} else if (cs.match ("ip", "access-group")) {
    if (cs.size() >= 4) {
	idpm.addentry_use ("acl", cs[2], curline);
    }
} else {
}




		break;
	    case UNKOWN:
		break;
	}


    }


if (fname.size() != 0) cerr << fname << ": ";
cerr << rawconf.size() << " lines read." << endl;
//cerr << "curline = " << curline << endl;
cerr << endl << " identifiers : " << endl;
cerr << idpm << endl;


    return 0;
}

// ----------------- main ---------------------------------------------------------------

int main (int nb, char ** cmde) {

    if (nb == 1)
	return filter_in (cin, "stdin");

    int i;
    for (i=1 ; i<nb ; i++) {
	ifstream fin (cmde[i]);
	if (!fin) {
	    int e = errno;
	    cerr << cmde[0] << " error: could not read " << cmde[i] << " " << strerror (e) << endl;
	    continue;
	}
	filter_in (fin, cmde[i]);
    }

    return 0;
}
