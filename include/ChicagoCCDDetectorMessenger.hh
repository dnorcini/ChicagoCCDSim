#ifndef ChicagoCCDDetectorMessenger_h
#define ChicagoCCDDetectorMessenger_h 1

#include "globals.hh"
#include "G4UIcmdWithoutParameter.hh"
#include "G4UIcmdWithABool.hh"
#include "G4UImessenger.hh"

class ChicagoCCDDetectorConstruction;
class G4UIdirectory;
class G4UIcommand;
class G4UIcmdWithoutParameter;
class G4UIcmdWithABool;

class ChicagoCCDDetectorMessenger: public G4UImessenger
{
  public:
    ChicagoCCDDetectorMessenger(ChicagoCCDDetectorConstruction*);
   ~ChicagoCCDDetectorMessenger();

    void SetNewValue(G4UIcommand*, G4String);

  private:
    ChicagoCCDDetectorConstruction* ChicagoCCDDetector;

    G4UIdirectory* CommandDir;
    G4UIcmdWithoutParameter* DelGeomCmd;
    G4UIcmdWithABool* CCDDeadCmd;
};

#endif
