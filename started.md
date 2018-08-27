---
title: BoGEMMS: Getting started
---

### [Home](README.md)

Getting started
=====

### Platforms

BoGEMMS currently runs on Mac OSX and Linux distributions.

### Dependencies

* the Geant4 toolkit (9.6, 10.1, 10.2, 10.3, 10.4 releases)
* the CFITSIO library
* (optional) the ROOT framework

### How to use it

The Geant4 software provides all the libraries needed to perform the simulation, but the user has the
responsibility to choose the physics needed for its virtual experiment and to select which kind of information 
must be provided (e.g. the energy deposit, the particle energy or direction, the type of interaction), all 
features that can be modified only in the hard-coded Geant4 working files. As a direct consequence, for each new 
experiment a new simulator has to be developed.

The BoGEMMS solves this problem by allowing the user to set the simulator by means of a configuration file read 
in input by the simulator. The general features of the configuration file are:
* the setting of the physics list (e.g. electromagnetic, hadronic, both);
* the selection of the output file among three types, providing information on the particle energy and track, or 
the volumes energy deposit, or the physics process in play for each step, and the possibility to write only a set 
of volumes to speed up the analysis;
* in case of simple geometries, the possibility to build directly the instruments and shielding design, as well 
as the creation of empty non interacting volumes (“stepping volumes”) to track or remove the particles (e.g the 
protons angular and spatial distribution at the exit of the optics).
* in the case of complex geometries, two branches are provided for the simulation of X-ray and gamma-ray
missions, where the configuration files provides the user with a set of flags to modify the geometry

##### Compiling BoGEMMS
The Makefile provided in the github code is for Mac installation. If you prefer Linux instead, use the Makefile_linux file (after renaming the file to Makefile).
```shell
make COMPILEVARS="...."
```
COMPILEVARS:
* Geant4 release: -D GEANT4_X_X (-D GEANT4_9_6 or -D GEANT4_10_1 or -D GEANT4_10_2 or -D GEANT4_10_3 or -D GEANT4_10_4)
* if ROOT is not linked to the Geant4 installation: -D WITHOUTROOT
* for using the database option (in beta version): -D SQLITE3

##### Running BoGEMMS
To run BoGEMMS, just type:
```shell
bogemms <configuration>.conf 0 <input>.mac
```
