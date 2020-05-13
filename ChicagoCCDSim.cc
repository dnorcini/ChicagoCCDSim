// ChicagoCCDSim.cc
// main program of the ChicagoCCD program

#include "ChicagoCCDDetectorConstruction.hh"
#include "ChicagoCCDActionInitialization.hh"

#ifdef G4MULTITHREADED
#include "G4MTRunManager.hh"
#else
#include "G4RunManager.hh"
#endif

#include "G4UImanager.hh"
#include "G4SystemOfUnits.hh"
#include "QBBC.hh"
#include "LBE.hh"

#include "PSPhysListFactory.hh"
#include "PSPhysicsList.hh"
//#include "DAMICPhysicsList.hh"
#include "DAMICPhysicsListLivermore.hh"
#include "defines.hh"

#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"

#include "Randomize.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

int main(int argc,char** argv)
{
  // Detect interactive mode (if no arguments) and define UI session
  //
  G4UIExecutive* ui = 0;
  if ( argc == 1 ) {
    ui = new G4UIExecutive(argc, argv);
  }

  // Choose the Random engine
  G4Random::setTheEngine(new CLHEP::RanecuEngine);
  
  // Construct the default run manager
  //
#ifdef G4MULTITHREADED
  G4MTRunManager* runManager = new G4MTRunManager;
#else
  G4RunManager* runManager = new G4RunManager;
#endif

  // Set mandatory initialization classes
  //
  // Detector construction
  ChicagoCCDDetectorConstruction* detectorConstruction = new ChicagoCCDDetectorConstruction();
  runManager->SetUserInitialization(detectorConstruction);

  // Physics list
//  G4VModularPhysicsList* physicsList = new QBBC;
//  G4VModularPhysicsList* physicsList = new LBE;
//  DAMICPhysicsList* physicsList = new DAMICPhysicsList;
  DAMICPhysicsListLivermore* physicsList = new DAMICPhysicsListLivermore;
//  physicsList->SetVerboseLevel(1);
  runManager->SetUserInitialization(physicsList);

  // User action initialization
  ChicagoCCDActionInitialization* actionInit = new ChicagoCCDActionInitialization(detectorConstruction);
  runManager->SetUserInitialization(actionInit);
  

  // Get the pointer to the User Interface manager
  G4UImanager* UImanager = G4UImanager::GetUIpointer();

  // Process macro or start UI session
  //
  if ( ! ui ) { 
    // batch mode
    G4String command = "/control/execute ";
    G4String fileName = argv[1];
    UImanager->ApplyCommand(command+fileName);
  }
  else { 
  // Initialize visualization
  //
  G4VisManager* visManager = new G4VisExecutive;
  // G4VisExecutive can take a verbosity argument - see /vis/verbose guidance.
  // G4VisManager* visManager = new G4VisExecutive("Quiet");
  visManager->Initialize();
    // interactive mode
    UImanager->ApplyCommand("/control/execute init_vis.mac");
    ui->SessionStart();
    delete ui;
 delete visManager;
  }

  // Job termination
  // Free the store: user actions, physics_list and detector_description are
  // owned and deleted by the run manager, so they should not be deleted 
  // in the main() program !
  
  delete runManager;
  delete detectorConstruction;
  delete actionInit;
  return 0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.....
