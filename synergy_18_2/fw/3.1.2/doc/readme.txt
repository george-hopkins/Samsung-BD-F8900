General Information
------- -----------

The synergy framework is a set of generic and platform specific components.
It operates with the terms BSP and GSP, respectively short for "Board Support
Package" and "Generic Support Package".

The interface description for the BSP and GSP is placed respectively in this
directory under bsp/* and gsp/*. 

All the interfaces described under bsp/* are the interfaces which must be 
ported to a given platform.

The interfaces described under gsp/* are the generic components which the 
technologies running on top of the Synergy Framework can use and which does
not need porting to a platform.

In this document folder the gu-0001-users-guide.pdf describes all the general 
priciples a user needs to understand in order to build and use the Synergy 
Framework.
