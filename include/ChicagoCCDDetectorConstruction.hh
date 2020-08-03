// ChicagoCCDDetectorConstruction.hh
// definition of the ChicagoCCDDetectorConstruction class

#ifndef ChicagoCCDDetectorConstruction_h
#define ChicagoCCDDetectorConstruction_h 1

#include "ChicagoCCDDetectorMessenger.hh"

#include "G4VUserDetectorConstruction.hh"
#include "G4ThreeVector.hh"
#include "G4RotationMatrix.hh"
#include "G4tgbRotationMatrix.hh"
#include "globals.hh"
#include "G4NistManager.hh"

#include <vector>
#include <utility>

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

    G4VPhysicalVolume* GetPhysWorld() const {return physWorld;};

    void ConstructMaterials();

    int GetCCDNum(G4VPhysicalVolume *volume);
    int GetTotCCDs() {return ActivePVs.size();};
    std::vector<G4ThreeVector> GetActiveDims() const {return ActiveDims;};

  protected:
    ChicagoCCDDetectorMessenger* skipperDetectorMessenger;

    G4LogicalVolume*  fScoringVolume;

    G4LogicalVolume* logicWorld;
    G4VPhysicalVolume* physWorld;
    G4VPhysicalVolume* ConstructWorld();

    std::vector< std::pair<G4ThreeVector, G4RotationMatrix*> > ActiveVecs;
    std::vector<G4ThreeVector> ActiveDims;
    std::vector<G4VPhysicalVolume*> ActivePVs;
    std::vector<G4VPhysicalVolume*> GetteringPVs;
    std::vector<G4VPhysicalVolume*> DeadTopPVs;
    std::vector<G4VPhysicalVolume*> DeadBottomPVs;
    std::vector<G4VPhysicalVolume*> DeadSidePVs;

    std::vector<G4VPhysicalVolume*> flangePhys;
    std::vector<G4VPhysicalVolume*> leadPhys;
    std::vector<G4VPhysicalVolume*> BeOPhys;
    G4VPhysicalVolume* physChamber;
    G4VPhysicalVolume* physPuck;
    G4VPhysicalVolume* physSmallColdHead;
    G4VPhysicalVolume* physLargeColdHead;
    G4VPhysicalVolume* physFlex;
    G4VPhysicalVolume* physSiBacking;
    G4VPhysicalVolume* physCopperBox;
    G4VPhysicalVolume* physCopperFacePlate;

    G4Material *Si;
    G4Material *Cu;
    G4Material *Pb;
    G4Material *Ny;
    G4Material *BeO;
    G4Material *Sb;
    G4Material *Kap;
    G4Material *world_mat;
    G4Material *Epoxy;
    G4Material *Steel;
    G4Material *Co57;
    G4Material *Am241;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

