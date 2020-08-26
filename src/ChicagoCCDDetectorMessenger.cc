#include "ChicagoCCDDetectorMessenger.hh"
#include "ChicagoCCDDetectorConstruction.hh"

#include "G4UIdirectory.hh"
#include "G4UIcmdWithoutParameter.hh"
#include "G4UIcmdWithABool.hh"
#include "G4SystemOfUnits.hh"

ChicagoCCDDetectorMessenger::ChicagoCCDDetectorMessenger(ChicagoCCDDetectorConstruction* Det)
:ChicagoCCDDetector(Det)
{
  CommandDir = new G4UIdirectory("/ChicagoGeom/");
  CommandDir->SetGuidance("Chicago Sims geometry commands.");

  DelGeomCmd = new G4UIcmdWithoutParameter("/ChicagoGeom/deleteGeometry",this);
  DelGeomCmd->SetGuidance("Delete geometry for comparison.");

  CCDDeadCmd = new G4UIcmdWithABool("/ChicagoGeom/setDeadLayer",this);
  CCDDeadCmd->SetGuidance("Sets whether CCD has inactive dead layers or not.");
  CCDDeadCmd->SetDefaultValue(true);
}

ChicagoCCDDetectorMessenger::~ChicagoCCDDetectorMessenger()
{
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
}
