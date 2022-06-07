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
- xerces-c (https://xerces.apache.org/xerces-c/): new dependency for G4, XML parser

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
Note: You will need to open a new command line session for these commands to run. If you add them or make changes and you want them right away, run the commands outside of `.bashrc` as well.

The output directory should be located in a place with adequate disk space (e.g. `/data/` on `zev`), while both the source and build paths can live in the `/home/` space.

Complete the following steps to compile ChicagoCCDSim (this assumes a working GitHub account):
```
$ cd <path/to/source/directory>
$ git clone https://github.com/dnorcini/ChicagoCCDSim  //(or fork, or download zip)
$ mkdir <path/to/build/directory>/ChicagoCCDSim_build
$ cd $CG4_BUILD
$ cmake -DCMAKE_BUILD_TYPE=Release -DGEANT4_BUILD_MULTITHREADED=ON -DGeant4_DIR=/usr/local/geant41031/lib64/Geant4-10.3.1 $CG4 
$ make -j<number of processors on your computer> 
```

Note: if you do not compile with the multithreaded flag, you can still run the simulation in singlethreaded mode, as long as your run macro does not indicate multiple threads.

## Run
The QT GUI can be entered with the command `./ChicagoCCDSim` in the build directory. By default, this will pull up the visualization window, which could just as well be called with `./ChicagoCCDSim mac/init_vis.mac`. To run a select macro, use `./ChicagoCCDSim <macro_file.mac>`. See the `mac/` directory for example macros. Specifically, those with 'guide' in their names (i.e. `mac/guide.mac`, `mac/loop_guide_master.mac`, and `mac/loop_guide_worker.mac`) will aim to teach the basics of macro construction, including basic commands and looping.

For general simulation macros, the user should specify at a minimum the output file location (`/Analysis/setFileName`) and the number of events to be simulated (`/run/beamOn`). The commands are described in detail in the Geant4 User Manual's section on General Particle Source, Section 2.7.

Custom commands are designed by messenger files in the source code. If you need to modify the source code and alter the value from the command level, refer to the current messenger files. An instance of each one is created in the constructor of its corresponding class and deleted in the destructor of said class.

## Output
The simulation will require an output file name to be specified in the macro. In Multithreaded mode, it will create a copy of this file for each thread. The file will contain three ROOT TTrees. See below for information about each TTree.

### RunInfo
(One row per simulation)

* __NEvts__ (int): number of events simulated
* __NCCD__ (int): number of CCDs in the detector geometry
* __Seed__ (int): initial seed (-1 if based on the process time)
* __concatedVolumeNames__ (str): semicolon-separated list of the volume’s names
* __volumeNameID__ (vec&lt;int&gt;): ID for each volume in the detector geometry
* __volumeMass__ (vec&lt;float&gt;) (g): vector with the mass for each volume in the detector
* __volumeDensity__ (vec&lt;float&gt;) (g/cm^3): vector with the density for each volume in geometry
* __volumeVolume__ (vec&lt;float&gt;) (cm^3): vector with the volume for each volume in geometry
* __volumeSurface__ (vec&lt;float&gt;) (cm^2): vector with the surface for each volume in geometry
* __primaryParticle__ (str): primary simulated particle type (electron, muon, ion, gamma, ...)
* __primaryIon__ (str): simulated ion: XXXaXXz (XXX, XX: mass and atomic)
* __simulatedVolume__ (str): name of the simulated volume

### EventOut
(One row per event. Individual instances for if multiple primaries lead to deposition)

* __EventID__ (int): event ID number
* __pdg__ (vec&lt;int&gt;): pdg code of the primary particles
* __primaryid__ (vec&lt;int&gt;): track ID of the primary particles
* __charge__ (vec&lt;int&gt;): charge of the primary particles
* __volid__ (vec&lt;int&gt;): volume where the particles are generated
* __energy__ (vec&lt;double&gt;) (eV): kinetic energy of the primary particle
* __posx__ (vec&lt;double&gt;) (mm):
* __posy__ (vec&lt;double&gt;) (mm): global position of the primary particle
* __posz__ (vec&lt;double&gt;) (mm):
* __momx__ (vec&lt;double&gt;) (eV):
* __momy__ (vec&lt;double&gt;) (eV): primary particle’s momentum
* __momz__ (vec&lt;double&gt;) (eV):
* __triggerTime__ (vec&lt;float&gt;) (s): trigger time

### CCDOut
(One row per event where energy was deposited in the CCD. Individual instances for each step of energy deposition)

* __EventID__ (int): event ID number
* __pdg__ (vec&lt;int&gt;): pdg codes of the particles that deposit energy in the CCD
* __trackid__ (vec&lt;int&gt;): track ID of the depositing particles
* __parentid__ (vec&lt;int&gt;): track ID of the depositing particles' parents
* __primaryid__ (vec&lt;int&gt;): track ID of the depositing particles' primaries
* __CCDid__ (vec&lt;int&gt;): ID number of the CCD receiving energy
* __posx__ (vec&lt;double&gt;) (mm):
* __posy__ (vec&lt;double&gt;) (mm): local positions of the depositing particles
* __posz__ (vec&lt;double&gt;) (mm):
* __gposx__ (vec&lt;double&gt;) (mm):
* __gposy__ (vec&lt;double&gt;) (mm): global positions of the depositing particles
* __gposz__ (vec&lt;double&gt;) (mm):
* __Edep__ (vec&lt;double&gt;) (eV): total energy deposited in the pixel per pdg
* __time__ (vec&lt;double&gt;) (s): time wrt to event trigger time

## Geometry
Currently the Chicago CCD chamber (basement) geometry represented in the package includes:

- vacuum chamber (at coordintes (0,0,0))
- chamber flanges
- Kapton flex cable
- cold head mount
- cold head
- CCD backing
- CCD base
- detailed 1kx6k skipper CCD model (active, gettering, and dead layers)

Note: the coordinate system implemented uses the following representations: x (top/bottom), y (left/right), and z (back/front). As such, the observer would be observing the system from + -> - z.

## Physics lists
- DAMICPhysListLivermore: This physics list is constructed by the DAMIC collaborators in France and designed to accurately suit the low energy analysis that we are performing with DAMIC.
- ... add details

## Particle generators
This simulation code uses Geant4's default General Particle Source code. The Geant4 User Manual offers useful information about its usage in Section 2.7.
- ...

## Troubleshooting
- Visualization depends on X11 forwarding over ssh. If running a macro with visualization and you receive the error
```
Unrecognized OpenGL version
Unrecognized OpenGL version
Segmentation fault (core dumped)
```

this means there is an issue with the forwarding On a Mac, this is handled by XQuartz and there can be an issue with the install. Try entering
```
$ defaults write org.macosforge.xquartz.X11 enable_iglx -bool true
```
then exit/restart XQuartz.

## To Do: Improvements
- create millicharged particle generator
- implement Bischel low energy model
- new upstairs chamber geometry
- run config files
- ...

## Notes
- any runtime macros should be added to source code in macros directory, and listed in CMakeLists.txt
- ...
