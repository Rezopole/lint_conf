## {
##  lint_conf, checks for missing, unusued, etc, declarations in switches and routers configurations, and more ...
##  Copyright (C) 2016-2020 Jean-Daniel Pauget <jdpauget@rezopole.net>
##  
##  This program is free software; you can redistribute it and/or
##  modify it under the terms of the GNU General Public License
##  as published by the Free Software Foundation; either version 2
##  of the License, or (at your option) any later version.
##  
##  This program is distributed in the hope that it will be useful,
##  but WITHOUT ANY WARRANTY; without even the implied warranty of
##  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
##  GNU General Public License for more details.
##  
##  You should have received a copy of the GNU General Public License
##  along with this program; if not, write to the Free Software
##  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
## }


ACLOCAL_AMFLAGS = -I m4

vimtest: lint_conf
	./lint_conf < example.ios
	./lint_conf example.ios

bin_PROGRAMS = lint_conf
man1_MANS = lint_conf.1

lint_conf_SOURCES = lint_conf.cpp
lint_conf_CXXFLAGS = @CXXFLAGS@


clean-local:
	:

distclean-local:
	:
	rm -f m4/*.m4
	rm -f config.h.in~
	rm -rf debian
	rm -f Makefile
	rm -f lint-conf*.deb



