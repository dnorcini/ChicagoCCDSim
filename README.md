# Chicago CCD GEANT4 simulation

<!-- Nick Corso (ncorso@uchicago.edu)-->
<!-- Danielle Norcini (dnorcini@kicp.uchicago.edu)-->
May 2020

## Overview
This program models energy depositions from various particle types in a silicon CCD. Using GEANT-4, a particle physics simulation based on C++ and the ROOT framework, the Chicago CCD Chamber geometry represented includes:

- add all chamber, CCD geo details here...
- ...

The goal of the program is generate models for various particle interactions in silicon, which can be compared to theoretical predictions. This is especially essential for low-energy events (sub-keV) that Skipper CCDs have access to, which are currently not well-modeled or measured. 

This package was created based from Example B1 in the GEANT-4 examples guide (http://geant4.web.cern.ch/geant4/UserDocumentation/Doxygen/examples_doc/html/ExampleB1.html). 
ADD: details about single vs multi threaded, if using special classes like geometry messengers, sensitive detector volues, photon tracking, ....

## Build 
The program is tested and supported on `zev.uchicago.edu`. `Geant4.10`, configured with Qt viewer, is used and the source code can be found on zev via `/find_source_directory!!!/geant4.10.02.p01`.
 
To install on `zev`, to make life easier, first add some environmental variables to your `.bashrc` file:
```
# env variables
export CG4=<path/to/source/directory>/ChicagoCCDSim
export CG4_build=<path/to/source/directory>/ChicagoCCDSim_build
```

Then complete the following steps (this assumes a working GitHub account):
```
$ mkdir <path/to/source/directory>/ChicagoCCDSim
$ mkdir <path/to/build/directory>/ChicagoCCDSim_build
$ cd $CG4
$ git clone https://github.com/dnorcini/ChicagoCCDSim  //(or fork, or download zip)
$ cd $CG4_build
$ cmake -DGeant4_DIR=/usr/local/geant41031/lib64/Geant4-10.3.1/ $CG4
$ make 
```
## Run
The Qt GUI can be entered with the command `./ChicagoCCDSim` in the build directory. Individual user commands can be entered interactively on the GUI command line. To run in batch mode for long-statistics runs, the basic command `./ChicagoCCDSim -m batch.mac` can be used. 

.... add info about loops and geometry messengers

## Physics lists
- ...

## Particle generators
- ...

## To Do: Improvements
- create millicharged particle generator
- ...

## Notes
- any runtime macros should be added to source code in macros directory, and listed in CMakeLists.txt
- ...
