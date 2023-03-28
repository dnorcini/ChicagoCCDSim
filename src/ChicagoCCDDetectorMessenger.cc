#include "ChicagoCCDDetectorMessenger.hh"
#include "ChicagoCCDDetectorConstruction.hh"

#include "G4UIdirectory.hh"
#include "G4UIcmdWithoutParameter.hh"
#include "G4UIcmdWithABool.hh"
#include "G4UIcmdWithAString.hh"
#include "G4SystemOfUnits.hh"

ChicagoCCDDetectorMessenger::ChicagoCCDDetectorMessenger(ChicagoCCDDetectorConstruction* Det)
:ChicagoCCDDetector(Det)
{
  CommandDir = new G4UIdirectory("/ChicagoGeom/");
  CommandDir->SetGuidance("Chicago Sims geometry commands.");

  DelGeomCmd = new G4UIcmdWithoutParameter("/ChicagoGeom/deleteGeometry", this);
  DelGeomCmd->SetGuidance("Delete geometry for comparison.");

  CCDDeadCmd = new G4UIcmdWithABool("/ChicagoGeom/setDeadLayer", this);
  CCDDeadCmd->SetGuidance("Sets whether CCD has inactive dead layers or not.");
  CCDDeadCmd->SetDefaultValue(true);

  SetShieldCmd = new G4UIcmdWithAString("/ChicagoGeom/setShield", this);
  SetShieldCmd->SetGuidance("Sets the material for a shield in front of the CCD.");
  SetShieldCmd->SetCandidates("Pb B D2O Con Poly");
  SetShieldCmd->SetDefaultValue("Pb");

/*
  LidCmd = new G4UIcmdWithADoubleAndUnit("/ChicagoGeom/setLidThickness", this);
  LidCmd->SetGuidance("Set the thickness of the lid attached to the copper.");
  LidCmd->SetUnitCategory("Length");
  LidCmd->SetDefaultValue(1.);
  LidCmd->SetDefaultUnit("mm");
  LidMatCmd = new G4UIcmdWithAString("/ChicagoGeom/setLidMaterial", this);
  LidMatCmd->SetGuidance("Set the material of the lid attached to the copper.");
  LidMatCmd->SetDefaultValue("Aluminum");
*/
}

ChicagoCCDDetectorMessenger::~ChicagoCCDDetectorMessenger()
{
  //delete LidMatCmd;
  //delete LidCmd;
  delete SetShieldCmd;
  delete CCDDeadCmd;
  delete DelGeomCmd;
  delete CommandDir;
}

void ChicagoCCDDetectorMessenger::SetNewValue(G4UIcommand* command, G4String newvalue)
{
  if( command == DelGeomCmd )
    {
      ChicagoCCDDetector->ToggleGeometry();
    }
  if( command == CCDDeadCmd )
    {
      ChicagoCCDDetector->SetCCDDead(CCDDeadCmd->GetNewBoolValue(newvalue));
    }
  if( command == SetShieldCmd )
    {
      ChicagoCCDDetector->SetShielding(newvalue);
    }
 /* if( command == LidCmd )
    {
      ChicagoCCDDetector->AssembleAlLids(LidCmd->GetNewDoubleValue(newvalue));
    }
  if( command == LidMatCmd )
    {
      ChicagoCCDDetector->SetLidMat(newvalue);
    }*/
}
