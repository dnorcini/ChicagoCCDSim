// ChicagoCCDSteppingAction.hh
// definition of the ChicagoCCDSteppingAction class

#ifndef ChicagoCCDSteppingAction_h
#define ChicagoCCDSteppingAction_h 1

#include "G4UserSteppingAction.hh"
#include "globals.hh"

#include <vector>
#include <utility>

class ChicagoCCDEventAction;
class ChicagoCCDRunAction;
class ChicagoCCDDetectorConstruction;

class G4LogicalVolume;

/// Stepping action class
/// 

class ChicagoCCDSteppingAction : public G4UserSteppingAction
{
  public:
    ChicagoCCDSteppingAction(ChicagoCCDEventAction* eventAction, ChicagoCCDRunAction* runAction, ChicagoCCDDetectorConstruction* detectorConstruction);
    virtual ~ChicagoCCDSteppingAction();

    // method from the base class
    virtual void UserSteppingAction(const G4Step*);

  private:
    ChicagoCCDEventAction*  fEventAction;
    G4LogicalVolume* fScoringVolume;
    ChicagoCCDRunAction* fRunAction;
    ChicagoCCDDetectorConstruction* fDetectorConstruction;
};

#endif
