// ChicagoCCDDetectorConstruction.hh
// definition of the ChicagoCCDDetectorConstruction class

#ifndef ChicagoCCDDetectorConstruction_h
#define ChicagoCCDDetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"
#include "G4ThreeVector.hh"
#include "globals.hh"
#include "G4NistManager.hh"

#include <vector>

class G4VPhysicalVolume;
class G4LogicalVolume;
class G4Box;
class G4Tubs;
class G4SubtractionSolid;
class G4Material;

/// Detector construction class to define materials and geometry.

class ChicagoCCDDetectorConstruction : public G4VUserDetectorConstruction
{
  public:
    ChicagoCCDDetectorConstruction();
    virtual ~ChicagoCCDDetectorConstruction();

    virtual G4VPhysicalVolume* Construct();
    void ToggleGeometry();
    
    G4LogicalVolume* GetScoringVolume() const { return fScoringVolume; };

    G4Material* GetRadioSource() const {return radioSource;};

    G4VPhysicalVolume* GetPhysWorld() const {return physWorld;};

    void SetRadioSource(G4Material *source) {radioSource = source;};
    void ConstructMaterials();

    int GetCCDNum(G4VPhysicalVolume *volume);
    int GetTotCCDs() {return ActivePVs.size();};

  protected:
    G4LogicalVolume*  fScoringVolume;

    G4Material *radioSource;
    G4LogicalVolume* logicWorld;
    G4VPhysicalVolume* physWorld;
    G4VPhysicalVolume* ConstructWorld();

    std::vector<G4ThreeVector> ActiveVecs;
    std::vector<G4VPhysicalVolume*> ActivePVs;
    std::vector<G4VPhysicalVolume*> GetteringPVs;
    std::vector<G4VPhysicalVolume*> DeadTopPVs;
    std::vector<G4VPhysicalVolume*> DeadBottomPVs;

    std::vector<G4VPhysicalVolume*> flangePhys;
    G4VPhysicalVolume* physChamber;
    G4VPhysicalVolume* physMount;
    G4VPhysicalVolume* physSmallColdHead;
    G4VPhysicalVolume* physLargeColdHead;
    G4VPhysicalVolume* physFlex;
    G4VPhysicalVolume* physSiBacking;
    G4VPhysicalVolume* physChicagoCCDBase;

    G4Material *Si;
    G4Material *Cu;
    G4Material *Pb;
    G4Material *Ny;
    G4Material *Kap;
    G4Material *world_mat;
    G4Material *Steel;
    G4Material *Co57;
    G4Material *Am241;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

