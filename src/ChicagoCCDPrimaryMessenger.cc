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

  SourceCmd = new G4UIcmdWithAString("/ChicagoPrimaryGen/setSource", this);
  SourceCmd->SetGuidance("Sets the isotope of the source.");
  SourceCmd->SetCandidates("Co57 Fe55");
  SourceCmd->SetDefaultValue("Fe55");
}

ChicagoCCDPrimaryMessenger::~ChicagoCCDPrimaryMessenger()
{
  delete SourceCmd;
  delete GammaSourceCmd;
  delete CommandDir;
}

void ChicagoCCDPrimaryMessenger::SetNewValue(G4UIcommand* command,G4String newValue)
{
  if( command == GammaSourceCmd )
    {
      ChicagoCCDPrimaryGenerator->SetGammaSource(GammaSourceCmd->GetNewBoolValue(newValue));
    }
  else if( command == SourceCmd )
    {
      ChicagoCCDPrimaryGenerator->SetSource(newValue);
    }
}
