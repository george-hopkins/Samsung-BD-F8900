Synergy Multi Technology Builds
-------------------------------
This readme explains how to configure and perform a build
of multiple installed Synergy components. I.e. to select 
among multiple installed versions of the Synergy Framework and
Synergy Technologies (BT, FM, WIFI,...) and do a combined build
using just one build command.

The steps are as follows. All steps are performed from the top
installation directory (c:/csr/synergy). 

1) Select Synergy components to build:
--------------------------------------
First do:

> make config

The build system then searches for installed Synergy components and 
prompts the user for a configuration name for the build.

> Please enter Synergy Configuration name: my_multi_build

> Installed Synergy fw versions
> ----------------------------------------------
> 1  :  1.0.0
> 2  :  1.1.0
> ----------------------------------------------
> Please select version to use (s to skip) :

> Installed Synergy bt versions
> ----------------------------------------------
> 1  :  17.1.0
> ----------------------------------------------
> Please select version to use (s to skip) :

After selecting components to build, users may make more configurations.
Configurations made can be listed as follows:

> make list
> Available configurations
> -----------------------
> my_multi_build

2) Building:
------------
To build a configured multi build do:

> make all CFG=my_multi_build
