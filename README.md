

# lint_conf #

## purpose : ##

**lint_conf** is a straitghforward simple router or switch configuration-checker.

Its first purpose is to dig out undeclared object that are still in use in a given configuration

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

### TODO : ###
- [ ] user-definable "*begining of configuration*" mark
- [ ] flag to turn on/off unused objects reports
- [ ] user-tunable list of identifier tokens
- [ ] a-la-make/vim error output, for jumping straight to error-lines
