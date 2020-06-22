# Chicago CCD GEANT4 simulation

Nick Corso (ncorso@uchicago.edu)  
Danielle Norcini (dnorcini@kicp.uchicago.edu)  
May 2020

## Overview
This program models energy depositions from various particle types in a silicon CCD using GEANT-4, a particle physics simulation based on C++ and the ROOT framework. Using detailed models of the CCD and the vacuum chamber it resides in, the program is used to generate models for various particle interactions in silicon, which can be compared to theoretical predictions. This is especially essential for low-energy events (sub-keV) that Skipper CCDs have access to, which are currently not well-modeled or measured. 

This package was created based from Example B1 in the GEANT-4 examples guide (http://geant4.web.cern.ch/geant4/UserDocumentation/Doxygen/examples_doc/html/ExampleB1.html). 

ADD: details about single vs multi threaded, if using special classes like geometry messengers, sensitive detector volues, photon tracking, ....

## Dependencies
- cmake (http://www.cmake.org/): sufficiently new version for other dependencies (>= 3 recommended)
- GEANT-4 (http://geant4.cern.ch): known to compile with versions 4.10.03 (cmake flags: -DGEANT4_INSTALL_DATA=ON)
- ROOT (http://root.cern.ch): known to work with versions 5.34/18
- Qt (https://www.qt.io/download-qt-installer): for visualization (cmake flags: -DGEANT4_USE_QT=ON)
- OpenGL (https://www.khronos.org/opengl/wiki/Getting_Started#Downloading_OpenGL): for visualization (cmake flags: -DGEANT4_USE_OPENGL_X11=ON)
- GDML (http://lcgapp.cern.ch/project/simu/framework/GDML/): geometry description format based on XML (cmake flags: -DGEANT4_USE_GDML=ON)

To build Geant4 with the above configurations, use the following cmake flags in the build directory:
```
$ cmake -DCMAKE_INSTALL_PREFIX=<absolute/path/to/geant_build_dir> <path/to/geant_source_dir> -DGEANT4_INSTALL_DATA=ON -DGEANT4_USE_GDML=ON  -DGEANT4_USE_QT=ON -DGEANT4_USE_OPENGL_X11=ON 
$ make -j<number of processors on your computer> 
```

## Build 
The program is tested and supported on `zev.uchicago.edu` using Geant4.10.03, configured with Qt viewer. The G4 source code can be found on `zev` via `/usr/local/geant41031/`.
 
To install ChicagoCCDSim on your `zev` account, and making life easier, first source the geant build and add some environmental variables to your `.bashrc` file:
```
# .bashrc

# geant4
source /usr/local/geant41031/bin/geant4.sh
export CG4=<path/to/source/directory>/ChicagoCCDSim
export CG4_BUILD=<path/to/build/directory>/ChicagoCCDSim_build
export CG4_OUTPUT=<path/to/output/directory>/ChicagoCCDSim_output
```
The output directory should be located in a place with adequate disk space (e.g. `/data/` on `zev`), while both the source and build paths can live in the `/home/` space.

Complete the following steps to compile ChicagoCCDSim (this assumes a working GitHub account):
```
$ cd <path/to/source/directory>/
$ git clone https://github.com/dnorcini/ChicagoCCDSim  //(or fork, or download zip)
$ mkdir <path/to/build/directory>/ChicagoCCDSim_build
$ cd $CG4_BUILD
$ cmake -DCMAKE_BUILD_TYPE=Release -DGEANT4_BUILD_MULTITHREADED=ON -DGeant4_DIR=//usr/local/geant41031/lib64/Geant4-10.3.1 $CG4 
$ make -j<number of processors on your computer> 
```

Note: if you do not compile with the multithreaded flag, you can still run the simulation in singlethreaded mode, as long as your run macro does not indicate multiple threads.

## Run
The QT GUI can be entered with the command `./ChicagoCCDSim` in the build directory (what default macro does it run??). To run a select macro, use `./ChicagoCCDSim <macro_file.mac>`. See the `mac/` directory for example macros. 

.... add info about loops and geometry messengers

## Output
The simulation output is a ROOT TTree. The branches of the TTree (named "???") ...

Output branches include:
- Prim (class ParticleEvent): event primary particles
- ... update

## Geometry
Currently the Chicago CCD chamber (basement) geometry represented in the package includes:

- vacuum chamber (at coordintes (0,0,0))
- chamber flanges
- Kapton flex cable
- cold head mount
- cold head
- CCD backing
- CCD base
- detailed 1kx6k skipper CCD model (active, getting, and dead layers)

Note: the coordinate system implemented uses the following representations: x (left/right), y (front/back), and z (top/bottom). This is in contrast to the typical GEANT4 coordinates where z represents the (left/right) or the direction of a particle beam in an accelerator. 

## Physics lists
- DAMICPhysListLivermore 
- ... add details

## Particle generators
- ...

## To Do: Improvements
- create millicharged particle generator
- implement Bischel low energy model
- new upstairs chamber geometry
- run config files
- ...

## Notes
- any runtime macros should be added to source code in macros directory, and listed in CMakeLists.txt
- ...
