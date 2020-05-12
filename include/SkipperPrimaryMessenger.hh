#ifndef SkipperPrimaryMessenger_h
#define SkipperPrimaryMessenger_h 1

#include "globals.hh"
#include "G4UImessenger.hh"

class SkipperPrimaryGeneratorAction;
class SkipperDetectorConstruction;
class G4UIdirectory;
class G4UIcommand;
class G4UIcmdWithABool;
class G4UIcmdWithoutParameter;

class SkipperPrimaryMessenger: public G4UImessenger
{
  public:
    SkipperPrimaryMessenger(SkipperPrimaryGeneratorAction*, SkipperDetectorConstruction*);
   ~SkipperPrimaryMessenger();

    void SetNewValue(G4UIcommand*, G4String);

  private:
    SkipperPrimaryGeneratorAction* SkipperPrimaryGenerator;
    SkipperDetectorConstruction* SkipperDetector;

    G4UIdirectory* ComptonDir;
    G4UIcmdWithABool* GammaSourceCmd;
    G4UIcmdWithoutParameter* DelGeomCmd;
};

#endif
