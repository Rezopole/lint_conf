

# lint_conf #

## checking _à-la-ios_ configurations : ##

**lint_conf** is a straitghforward simple router or switch configuration-checker.

Its first purpose is to dig out undeclared objects that are still in use in a given configuration

yet, it mainly reports about :

* **missing declaration** for objects such as
	* port-channel
	* peer-group
	* route-map
	* ipv4 or ipv6 prefix-list
	* access-list
	* community-list
* **unused** objects such as
	* vrf
	* interface
	* peer-group
	* route-map
	* ipv4 or ipv6 prefix-list
	* access-list
	* community-list

## build : ##

on a regular unix flavored machine a simple ```make``` should do the trick :
```
> make
g++ -Wall -o lint_conf lint_conf.cpp
```

## use : ##

the basic scheme is ```lint_conf configuration_file  [ ... more conf_files ... ]```

alternatively, if you're into pipes : ``` cat myconf | lint_conf```

## example : ##
```
> ./lint_conf example.ios
example.ios: 421 lines read.

 identifiers : 
example.ios: acl [3] :
   95 undeclared use ( 401 )
   96 undeclared use ( 400 )
   97 undeclared use ( 399 )
example.ios: community-list [6] :
   Not-For-Out undeclared use ( 371 )
   Not-For-Remote-Peerings undeclared use ( 368 )
example.ios: interface [54] :
example.ios: ipv4 prefix-list [2] :
   No-AS43100-in undeclared use ( 342 )
example.ios: ipv6 prefix-list [2] :
   No-AS43100-in undeclared use ( 353 )
example.ios: peer group [2] :
example.ios: port-channel [1] :
example.ios: route-map [2] :
   somedistantIX-in undeclared use ( 343 354 )
example.ios: router bgp [1] :
```

## TODO : ##
- [ ] user-definable "*begining of configuration*" mark
- [ ] flag to turn on/off unused objects reports
- [ ] user-tunable list of identifier tokens
- [ ] a-la-make/vim error output, for jumping straight to error-lines
- [ ] regular make / make instal framework !
- [ ] man-page
- [ ] limit results to one type of object (ex: acl)
- [ ] display the content of the line next to the line number

## Contacts : ##
observations, suggestions, additions : lint_conf@rezopole.net

