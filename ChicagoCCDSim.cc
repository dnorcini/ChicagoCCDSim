// ChicagoCCDSim.cc
// main() program of ChicagoCCDSim

// includes
//detector construction and physics processes
#include "SkipperDetectorConstruction.hh"
#include "SkipperActionInitialization.hh"

//multithreaded run manager toolkit
#ifdef G4MULTITHREADED
#include "G4MTRunManager.hh"
#else
#include "G4RunManager.hh"
#endif

//user interfaces
#include "G4UImanager.hh"
#include "G4UIcommand.hh"

//visualization
#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"

//utils
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"

//physics lists
#include "G4SystemOfUnits.hh"
#include "QBBC.hh"
#include "LBE.hh"
#include "PSPhysListFactory.hh"
#include "PSPhysicsList.hh"
#include "DAMICPhysicsList.hh"
#include "defines.hh"

// define namespace
namespace {
  void PrintUsage() {
    G4cerr << "Usage: " << G4endl;
    G4cerr << "ChicagoCCDSim [-m macro] [-u UIsession] [-t nThreads]" << G4endl;
    G4cerr << "\tnote: -t option is only for multi-threaded mode." << G4endl;
  }
}

int main(int argc,char** argv)
{
  if (argc > 7) {
    PrintUsage();
    return 1;
  }

  // define UI session if no macro given
  G4UIExecutive* ui = 0;
  if ( argc == 1 ) {
    ui = new G4UIExecutive(argc, argv);
  }

  // choose the Random engine
  G4Random::setTheEngine(new CLHEP::RanecuEngine);
  
  // construct the default run manager
#ifdef G4MULTITHREADED
  G4MTRunManager* runManager = new G4MTRunManager;
#else
  G4RunManager* runManager = new G4RunManager;
#endif

  // set mandatory initialization classes
  //initialize detector construction
  SkipperDetectorConstruction* detectorConstruction = new SkipperDetectorConstruction();
  runManager->SetUserInitialization(detectorConstruction);

  //initialize physics processes
  //G4VModularPhysicsList* physicsList = new QBBC;
  //G4VModularPhysicsList* physicsList = new LBE;
  DAMICPhysicsList* physicsList = new DAMICPhysicsList;
  //physicsList->SetVerboseLevel(1);
  runManager->SetUserInitialization(physicsList);
    
  //initialize primary generator
  runManager->SetUserInitialization(new SkipperActionInitialization(detectorConstruction));
  
  //initialize visualization
  G4VisManager* visManager = new G4VisExecutive;
  visManager->Initialize();

  //get pointer to the User Interface manager
  G4UImanager* UImanager = G4UImanager::GetUIpointer();

  //process macro or start UI session
  if ( ! ui ) { 
    // batch mode
    G4String command = "/control/execute ";
    G4String fileName = argv[1];
    UImanager->ApplyCommand(command+fileName);
  }
  else { 
    // interactive mode
    UImanager->ApplyCommand("/control/execute init_vis.mac");
    ui->SessionStart();
    delete ui;
  }

  // Job termination
  // Free the store: user actions, physics_list and detector_description are
  // owned and deleted by the run manager, so they should not be deleted 
  // in the main() program !
  delete visManager;
  delete runManager;
}


