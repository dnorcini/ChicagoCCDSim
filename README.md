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

## Dependencies
- cmake (http://www.cmake.org/): sufficiently new version for other dependencies (>= 3 recommended)
- Geant4 (http://geant4.cern.ch): known to compile with versions 4.10.03 (cmake flags: -DGEANT4_INSTALL_DATA=ON)
- ROOT (http://root.cern.ch): known to work with versions 5.34/18
- For visualization, it's also useful to have some of Geant4's optional graphics dependencies installed (see Geant4's install guides), e.g. QT and OpenGL libraries (cmake flags: -DGEANT4_USE_QT=ON -DGEANT4_USE_OPENGL_X11=ON)

## Build 
The program is tested and supported on `zev.uchicago.edu` using Geant4.10.03, configured with Qt viewer. The G4 source code can be found on `zev` via `/usr/local/geant41031/`.
 
To install ChicagoCCDSim on your `zev` account, and making life easier, first add some environmental variables to your `.bashrc` file:
```
# env variables
export CG4=<path/to/source/directory>/ChicagoCCDSim
export CG4_BUILD=<path/to/build/directory>/ChicagoCCDSim_build
export CG4_OUTPUT=<path/to/output/directory>/ChicagoCCDSim_output
```
The output directory should be located in a place with adequate disk space (e.g. `/data/` on `zev`), while both the source and build paths can live in the `/home/` space.

Complete the following steps to compile ChicagoCCDSim (this assumes a working GitHub account):
```
$ mkdir <path/to/source/directory>/ChicagoCCDSim
$ mkdir <path/to/build/directory>/ChicagoCCDSim_build
$ cd $CG4
$ git clone https://github.com/dnorcini/ChicagoCCDSim  //(or fork, or download zip)
$ cd $CG4_build
$ cmake -DCMAKE_INSTALL_PREFIX $CG4_BUILD -DGEANT4_DIR=$CG4 -DGEANT4_INSTALL_DATA=ON -DGEANT4_USE_QT=ON -DGEANT4_USE_OPENGL_X11=ON 
$ make -j<number of processors on your computer> 
```
## Run
The QT GUI can be entered with the command `./ChicagoCCDSim` in the build directory (what default macro does it run??). To run a select macro, use `./ChicagoCCDSim <macro_file.mac>`. See the `mac/` directory for example macros. 

.... add info about loops and geometry messengers

## Output
The simulation output is a ROOT TTree. The branches of the TTree (named "???") ...

Output branches include:
- Prim (class ParticleEvent): event primary particles
- ... update

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
