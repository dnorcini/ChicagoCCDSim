// ChicagoCCDPrimaryMessenger.c
// definition of ChicagoCCDPrimaryMessenger class

#ifndef ChicagoCCDPrimaryMessenger_h
#define ChicagoCCDPrimaryMessenger_h 1

#include "globals.hh"
#include "G4UIcmdWithoutParameter.hh"
#include "G4UIcmdWithABool.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UImessenger.hh"

class ChicagoCCDPrimaryGeneratorAction;
class G4UIdirectory;
class G4UIcommand;
class G4UIcmdWithABool;
class G4UIcmdWithAString;

class ChicagoCCDPrimaryMessenger: public G4UImessenger
{
  public:
    ChicagoCCDPrimaryMessenger(ChicagoCCDPrimaryGeneratorAction*);
   ~ChicagoCCDPrimaryMessenger();

    void SetNewValue(G4UIcommand*, G4String);

  private:
    ChicagoCCDPrimaryGeneratorAction* ChicagoCCDPrimaryGenerator;

    G4UIdirectory* CommandDir;
    G4UIcmdWithABool* GammaSourceCmd;
    G4UIcmdWithAString* SourceCmd;
};

#endif
