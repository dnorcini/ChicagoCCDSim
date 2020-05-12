#include "SkipperPrimaryMessenger.hh"
#include "SkipperPrimaryGeneratorAction.hh"
#include "SkipperDetectorConstruction.hh"

#include "G4UIdirectory.hh"
/*
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithoutParameter.hh"
*/
#include "G4UIcmdWithABool.hh"
#include "G4SystemOfUnits.hh"

SkipperPrimaryMessenger::SkipperPrimaryMessenger(SkipperPrimaryGeneratorAction* Prim, SkipperDetectorConstruction* Det)
:SkipperPrimaryGenerator(Prim),
SkipperDetector(Det)
{
  ComptonDir = new G4UIdirectory("/compton/");
  ComptonDir->SetGuidance("Commands for Compton test.");

  GammaSourceCmd = new G4UIcmdWithABool("/compton/setGammaSource",this);
  GammaSourceCmd->SetGuidance("Set whether the the source is pure gamma or ion.");

}

SkipperPrimaryMessenger::~SkipperPrimaryMessenger()
{
  delete GammaSourceCmd;
  delete ComptonDir;
}

void SkipperPrimaryMessenger::SetNewValue(G4UIcommand* command,G4String newValue)
{
  if( command == GammaSourceCmd )
    {
      SkipperPrimaryGenerator->SetGammaSource(GammaSourceCmd->GetNewBoolValue(newValue));
      SkipperDetector->ToggleGeometry();
    }
}
