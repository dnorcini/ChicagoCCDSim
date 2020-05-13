// ChicagoCCDPrimaryMessenger.c
// definition of ChicagoCCDPrimaryMessenger class

#ifndef ChicagoCCDPrimaryMessenger_h
#define ChicagoCCDPrimaryMessenger_h 1

#include "globals.hh"
#include "G4UImessenger.hh"

class ChicagoCCDPrimaryGeneratorAction;
class ChicagoCCDDetectorConstruction;
class G4UIdirectory;
class G4UIcommand;
class G4UIcmdWithABool;
class G4UIcmdWithoutParameter;

class ChicagoCCDPrimaryMessenger: public G4UImessenger
{
  public:
    ChicagoCCDPrimaryMessenger(ChicagoCCDPrimaryGeneratorAction*, ChicagoCCDDetectorConstruction*);
   ~ChicagoCCDPrimaryMessenger();

    void SetNewValue(G4UIcommand*, G4String);

  private:
    ChicagoCCDPrimaryGeneratorAction* ChicagoCCDPrimaryGenerator;
    ChicagoCCDDetectorConstruction* ChicagoCCDDetector;

    G4UIdirectory* ComptonDir;
    G4UIcmdWithABool* GammaSourceCmd;
    G4UIcmdWithoutParameter* DelGeomCmd;
};

#endif
