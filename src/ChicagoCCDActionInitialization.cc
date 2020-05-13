// ChicagoCCDActionInitialization.cc
// implementation of the ChicagoCCDActionInitialization class

#include "ChicagoCCDActionInitialization.hh"
#include "ChicagoCCDPrimaryGeneratorAction.hh"
#include "ChicagoCCDRunAction.hh"
#include "ChicagoCCDEventAction.hh"
#include "ChicagoCCDSteppingAction.hh"
#include "ChicagoCCDDetectorConstruction.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ChicagoCCDActionInitialization::ChicagoCCDActionInitialization(ChicagoCCDDetectorConstruction* detectConstruct)
 : G4VUserActionInitialization(),
        detectorConstruction(detectConstruct)

{
 // detectorConstruction = detectConstruct;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ChicagoCCDActionInitialization::~ChicagoCCDActionInitialization()
{
delete detectorConstruction;
//delete primGenAction;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ChicagoCCDActionInitialization::BuildForMaster() const
{
//  ChicagoCCDRunAction* runAction = new ChicagoCCDRunAction(detectorConstruction, primGenAction);
//  SetUserAction(runAction);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ChicagoCCDActionInitialization::Build() const
{
  ChicagoCCDPrimaryGeneratorAction* primGenAction = new ChicagoCCDPrimaryGeneratorAction(detectorConstruction);
  SetUserAction(primGenAction);

  ChicagoCCDRunAction* runAction = new ChicagoCCDRunAction(detectorConstruction, primGenAction);
  SetUserAction(runAction);
  
  ChicagoCCDEventAction* eventAction = new ChicagoCCDEventAction(runAction);
  SetUserAction(eventAction);
  
  SetUserAction(new ChicagoCCDSteppingAction(eventAction,runAction,detectorConstruction));
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
