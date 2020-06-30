// ChicagoCCDPrimaryMessenger.cc
// implementation of ChicagoCCDPrimaryMessenger class

#include "ChicagoCCDPrimaryMessenger.hh"
#include "ChicagoCCDPrimaryGeneratorAction.hh"
#include "ChicagoCCDDetectorConstruction.hh"

#include "G4UIdirectory.hh"
#include "G4UIcmdWithABool.hh"
#include "G4UIcmdWithoutParameter.hh"
#include "G4SystemOfUnits.hh"

ChicagoCCDPrimaryMessenger::ChicagoCCDPrimaryMessenger(ChicagoCCDPrimaryGeneratorAction* Prim)
:ChicagoCCDPrimaryGenerator(Prim)
{
  CommandDir = new G4UIdirectory("/ChicagoPrimaryGen/");
  CommandDir->SetGuidance("Commands for Chicago Sim Primary Generator.");

  GammaSourceCmd = new G4UIcmdWithABool("/ChicagoPrimaryGen/setGammaSource",this);
  GammaSourceCmd->SetGuidance("Set whether the the source is pure gamma or ion.");
}

ChicagoCCDPrimaryMessenger::~ChicagoCCDPrimaryMessenger()
{
  delete GammaSourceCmd;
  delete CommandDir;
}

void ChicagoCCDPrimaryMessenger::SetNewValue(G4UIcommand* command,G4String newValue)
{
  if( command == GammaSourceCmd )
    {
      ChicagoCCDPrimaryGenerator->SetGammaSource(GammaSourceCmd->GetNewBoolValue(newValue));
    }
}
