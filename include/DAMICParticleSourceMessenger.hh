#ifndef DAMICParticleSourceMessenger_h
#define DAMICParticleSourceMessenger_h 1

#include "G4UImessenger.hh"
#include "globals.hh"

class DAMICParticleSource;

class G4ParticleTable;
class G4UIcommand;
class G4UIdirectory;
class G4UIcmdWithoutParameter;
class G4UIcmdWithAString;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWith3Vector;
class G4UIcmdWith3VectorAndUnit;
class G4UIcmdWithAnInteger;
class G4UIcmdWithADouble;
class G4UIcmdWithABool;
class G4UIcmdWithoutParameter;


class DAMICParticleSourceMessenger: public G4UImessenger{

  public:
    DAMICParticleSourceMessenger(DAMICParticleSource *fPartSource);
    ~DAMICParticleSourceMessenger();

  void SetNewValue(G4UIcommand *command, G4String newValues);

  private:

    DAMICParticleSource *fParticleGun;
    G4ParticleTable *particleTable;

  private:

    G4UIdirectory* gunDirectory;
    G4UIdirectory* EnergyDirectory;
    G4UIdirectory* DirectionDirectory;
    G4UIdirectory* PositionDirectory;
    G4UIcmdWithoutParameter* ListCmd;

  private:
    // Energy

    G4UIcmdWithADoubleAndUnit* DoMonoNRJCmd;
    G4UIcmdWithAString* DoDistriNRJCmd;
    G4UIcmdWithADoubleAndUnit* UniformBotCmd;
    G4UIcmdWithADoubleAndUnit* UniformTopCmd;

  private:

    // Direction Momentum

    G4UIcmdWithoutParameter* DoOneDCmd;
    G4UIcmdWithADouble* DoOneDXCmd;
    G4UIcmdWithADouble* DoOneDYCmd;
    G4UIcmdWithADouble* DoOneDZCmd;

    G4UIcmdWithAString* DoDistriDCmd;
    G4UIcmdWithADouble* IsoThetaMinCmd;
    G4UIcmdWithADouble* IsoThetaMaxCmd;
    G4UIcmdWithADouble* IsoPhiMinCmd;
    G4UIcmdWithADouble* IsoPhiMaxCmd;

  private:
    // Position
    G4UIcmdWithAString* DoMaterialCmd;
    G4UIcmdWithAString* MotherVolumeCmd;

    G4UIcmdWithoutParameter* DoVolumeCmd;
    G4UIcommand* AddVolumeCmd;

    G4UIcmdWithoutParameter* DoSourceCmd;
	
	G4UIcmdWithoutParameter* DoSurfaceCmd;
    G4UIcmdWithAString* DoShapeCmd;

    G4UIcmdWithADoubleAndUnit* CenterXCmd;
    G4UIcmdWithADoubleAndUnit* CenterYCmd;
    G4UIcmdWithADoubleAndUnit* CenterZCmd;

    G4UIcmdWithADoubleAndUnit* ParaXCmd;
    G4UIcmdWithADoubleAndUnit* ParaYCmd;
    G4UIcmdWithADoubleAndUnit* ParaZCmd;

    G4UIcmdWithADoubleAndUnit* SphereRCmd;

    G4UIcmdWithADoubleAndUnit* CylRCmd;
    G4UIcmdWithADoubleAndUnit* CylHCmd;

  private:
    // Particle Spec
    G4UIcmdWithAString* ParticleCmd;
    G4UIcmdWithADouble* ChargeCmd;
    G4UIcommand* IonCmd;

  private:
    G4bool   fShootIon;
    G4int    fAtomicNumber;
    G4int    fAtomicMass;
    G4int    fIonCharge;
    G4double fIonExciteEnergy;
};
#endif
