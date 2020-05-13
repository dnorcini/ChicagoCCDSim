// ChicagoCCDPrimaryMessenger.cc
// implementation of ChicagoCCDPrimaryMessenger class

#include "ChicagoCCDPrimaryMessenger.hh"
#include "ChicagoCCDPrimaryGeneratorAction.hh"
#include "ChicagoCCDDetectorConstruction.hh"

#include "G4UIdirectory.hh"
/*
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithoutParameter.hh"
*/
#include "G4UIcmdWithABool.hh"
#include "G4UIcmdWithoutParameter.hh"
#include "G4SystemOfUnits.hh"

ChicagoCCDPrimaryMessenger::ChicagoCCDPrimaryMessenger(ChicagoCCDPrimaryGeneratorAction* Prim, ChicagoCCDDetectorConstruction* Det)
:ChicagoCCDPrimaryGenerator(Prim),
ChicagoCCDDetector(Det)
{
  ComptonDir = new G4UIdirectory("/compton/");
  ComptonDir->SetGuidance("Commands for Compton test.");

  GammaSourceCmd = new G4UIcmdWithABool("/compton/setGammaSource",this);
  GammaSourceCmd->SetGuidance("Set whether the the source is pure gamma or ion.");

  DelGeomCmd = new G4UIcmdWithoutParameter("/compton/deleteGeometry",this);
  DelGeomCmd->SetGuidance("Delete geometry for comparison.");
}

ChicagoCCDPrimaryMessenger::~ChicagoCCDPrimaryMessenger()
{
  delete DelGeomCmd;
  delete GammaSourceCmd;
  delete ComptonDir;
}

void ChicagoCCDPrimaryMessenger::SetNewValue(G4UIcommand* command,G4String newValue)
{
  if( command == GammaSourceCmd )
    {
      ChicagoCCDPrimaryGenerator->SetGammaSource(GammaSourceCmd->GetNewBoolValue(newValue));
    }
  if( command == DelGeomCmd )
    {
      ChicagoCCDDetector->ToggleGeometry();
    }
}
