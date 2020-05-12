# Chicago CCD GEANT4 simulation

Nick Corso (ncorso@uchicago.edu), Danielle Norcini (dnorcini@kicp.uchicago.edu), May 2020

## Overview
This program models energy depositions from various particle types in a silicon CCD. Using GEANT-4, a particle physics simulation based on C++ and the ROOT framework, the Chicago CCD Chamber geometry represented includes:

- add all chamber, CCD geo details here...
- ...

The goal of the program is generate models for various particle interactions in silicon, which can be compared to theoretical predictions. This is especially essential for low-energy events (sub-keV) that Skipper CCDs have access to, which are currently not well-modeled or measured. 

This package was created based from Example B1 in the GEANT-4 examples guide (http://geant4.web.cern.ch/geant4/UserDocumentation/Doxygen/examples_doc/html/ExampleB1.html). Currently operating in single-threaded mode, the program makes use of G4GeomtryMessengers class to change the values of parameters upon runtime within looped macros. It is important to note that currently the simulation only exports information about the energy deposition in the so called 'sensitive detectors', e.g. the scintillator material and HPGe crystal. Individual photons are not tracked. Furthermore, PMTs and the scintillator properities itself are not modeled, as this adds unncessary complication for our purposes. Only gamma-ray physics within the FTFP_BERT physics list is utilized.   

## Build 
The program is tested and supported on `zev.uchicago.edu`. `Geant4.10`, configured with Qt viewer, is used and the source code can be found on zev via `/find_source_directory!!!/geant4.10.02.p01`.
 
To install on `zev`, complete the following steps (this assumes a working GitHub account):
```
$ cd ~
$ git clone https://github.com/dnorcini/ChicagoCCDSim  //(or fork, or download zip)
$ cd ~
$ mkdir ChicagoCCDSim-build
$ cd ChicagoCCDSim build
$ cmake -DGeant4_DIR=/find_build_directory!!!/geant4.10.02.p01-build/lib64/Geant4-10.2.1 ~/ChicagoCCDSim 
$ make 
```
## Run
The Qt GUI can be entered with the command `./ChicagoCCDSim` in the build directory. Individual user commands can be entered interactively on the GUI command line. To run in batch mode for long-statistics runs, the basic command `./ComptonSim -m batch.mac` can be used. 

.... add info about loops and geometry messengers

## To Do: Improvements
- ...

## Notes
- any runtime macros should be added to source code in macros directory, and listed in CMakeLists.txt
- ...
