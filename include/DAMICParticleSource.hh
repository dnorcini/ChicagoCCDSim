#ifndef DAMICParticleSource_h
#define DAMICParticleSource_h

#include "G4VPrimaryGenerator.hh"
#include "G4Navigator.hh"
#include "G4ParticleMomentum.hh"
#include "G4ParticleDefinition.hh"

#include "DAMICParticleSourceMessenger.hh"

class DAMICVolumeCoordinateStore;

class DAMICParticleSource: public G4VPrimaryGenerator{
  public:
    DAMICParticleSource();
    ~DAMICParticleSource();
    void GeneratePrimaryVertex(G4Event* evt);
  public:
    //Particle Sets
    void SetParticleEnergy(G4double);
    void SetParticleCharge(G4double);
    void SetParticleMomentumDirection(G4ParticleMomentum);
    void SetParticleTime(G4double);
    void SetParticleDefinition(G4ParticleDefinition*);
    void SetParticlePosition(G4ThreeVector);

    //Position Sets
    void SetMaterial(G4String);
    void SetVolumeSource(G4String);
    void SetMaterialSource(G4String);
    void SetMotherVolume(G4String);
    void SetSourceShape(G4String);
    void SetSurface(G4bool);
    void SetVolume(G4bool);
    void SetMaterial(G4bool);
    void SetSource(G4bool);
    void SetShape(G4bool);

    void SetPositionNumSize(G4int);
    void SetPositionNumValue(G4double, G4int);

    // Position Reset

    void ResetPositionNum();


    //Energy Sets

    void SetDistriNRJ(G4bool, G4String);
    void SetMonoNRJ(G4bool);
    void SetEnergyNumSize(G4int);
    void SetEnergyNumValue(G4double, G4int);

    //Energy Reset

    void ResetEnergyNum();

    // Direction Sets

    void SetDistriD(G4bool, G4String);
    void SetOneD(G4bool);
    void SetDirectionNumSize(G4int);
    void SetDirectionNumValue(G4double, G4int);

    // Direction Reset

    void ResetDirectionNum();


    // VOLUME
    void AddVolume(G4String, G4double);
    void CalculProba();
    void ChooseVolume();

    //Energy

    void EnergyValue();

    // Direction

    void DirectionValue();

    //bools Position
    G4bool IsSurface();
    G4bool IsVolume();
    G4bool IsMaterial();
    G4bool IsShape();
    G4bool IsSource();

    // bools Energy

    G4bool IsDistriNRJ();
    G4bool IsMonoNRJ();

    // bools Direction

    G4bool IsDistriD();
    G4bool IsOneD();

    //changes bools Position

    void DoVolume();
    void DoMaterial();
    void DoSource();
    void DoShape(G4String);
	void DoSurface();

    //Changes bools Energy

    void DoMonoNRJ();
    void DoDistriNRJ(G4String);

    // Changes bools Momentum

    void DoOneD();
    void DoDistriD(G4String);


    //coordinatesCreation

    void MaterialCoordinates();
    void VolumeCoordinates();
    void SourceCoordinates();
    void ShapeCoordinates();
    void CalculPosition(G4String, G4String, G4String);
	void SurfaceCoordinates();
    //Mother Volume Hit and Miss

    void FindInMother(); // hit and miss


    G4double SpectrumBeta(G4double);

  private:
    // Particle Carac

    G4double ParticleEnergy;
    G4double ParticleCharge;
    G4ParticleMomentum ParticleMomentumDirection;
    G4double ParticleTime;
    G4ParticleDefinition* ParticleDefinition;
    G4ThreeVector ParticlePosition;

    //Position
    G4bool bSurface; //True on surface
    G4bool bVolume; //True if Volume
    G4bool bMaterial;
    G4bool bShape;
    G4bool bSource;
    G4String VolumeSource; // IF VOLUME MODE
    G4String MaterialSource; // IF MATERIAL MODE
    G4String MotherVolume;
    G4String Material;
    G4String Shape;


    // Energy

    G4bool bDistriNRJ;
    G4bool bMonoNRJ;
    G4String DistriNRJ;

    //Tritium

    G4bool CalculatedMaxTri;
    G4double MaxTritium;

    //Direction

    G4bool bOneD;
    G4bool bDistriD;
    G4String DistriD;

    std::vector <G4double> PositionNum;
    std::vector <G4double> EnergyNum;
    std::vector <G4double> DirectionNum;

    std::vector<G4String> VolumesUse;
    std::vector<G4double> VolumesUseMass;
    std::vector<G4double> VolumesConcentration;
    std::vector<G4double> Proba;
    // for volumes
    G4bool changes;
    //Navigator for Geometry
    G4Navigator* gNavigator;
    // Messenger

    DAMICParticleSourceMessenger* Messenger;
	DAMICVolumeCoordinateStore *CoordinateStore;
};

#endif
