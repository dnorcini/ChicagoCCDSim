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

    void SetCCDDead(G4bool newval, G4bool isFirst = false);
    G4bool GetCCDDead() const {return CCDDead;};

    void ConstructMaterials();

    int GetCCDNum(G4VPhysicalVolume *volume);
    int GetTotCCDs() {return ActivePVs.size();};
    std::vector<G4ThreeVector> GetActiveDims() const {return ActiveDims;};
    G4double GetPixWidth() {return pixWidth;};

    void AssembleAlLids(G4double thickness, G4bool isFirst = false);
    void SetLidMat(G4String mat) {lidMat = mat;};

  protected:
    ChicagoCCDDetectorMessenger* skipperDetectorMessenger;

    G4bool checkOverlaps;
    G4LogicalVolume*  fScoringVolume;

    G4LogicalVolume* logicWorld;
    G4VPhysicalVolume* physWorld;
    G4VPhysicalVolume* ConstructWorld();

    G4double getThick;
    G4double deadThick;

    G4bool CCDDead;

    std::vector< std::pair<G4ThreeVector, G4RotationMatrix*> > ActiveVecs;
    G4double pixWidth;
    std::vector<G4ThreeVector> ActiveDims;
    std::vector<G4ThreeVector> ActiveDims1;
    std::vector<G4ThreeVector> ActiveDims2;
    std::vector<G4VPhysicalVolume*> ActivePVs;
    std::vector<G4VPhysicalVolume*> GetteringPVs;
    std::vector<G4VPhysicalVolume*> DeadTopPVs;
    std::vector<G4VPhysicalVolume*> DeadBottomPVs;
    std::vector<G4VPhysicalVolume*> DeadSidePVs;

    std::vector<G4LogicalVolume*> ActiveFullLVs;
    std::vector<G4LogicalVolume*> DeadShellLVs;
    std::vector<G4LogicalVolume*> ActiveLVs;
    std::vector<G4LogicalVolume*> GetteringLVs;
    std::vector<G4LogicalVolume*> DeadLayLVs;
    std::vector<G4LogicalVolume*> DeadSideLVs;

    std::vector<G4VPhysicalVolume*> flangePhys;
    G4VPhysicalVolume* physChamber;
    G4VPhysicalVolume* physPuck;
    G4VPhysicalVolume* physSmallColdHead;
    G4VPhysicalVolume* physLargeColdHead;
    G4VPhysicalVolume* physFlex;
    G4VPhysicalVolume* physSiBacking;
    G4VPhysicalVolume* physCopperBox;
    G4VPhysicalVolume* physCopperFacePlate;

    G4bool mylarLid;
    G4String lidMat;
    G4VPhysicalVolume* physFrontLidMylar;
    G4VPhysicalVolume* physBackLidMylar;
    G4VPhysicalVolume* physFrontLidAlum;
    G4VPhysicalVolume* physBackLidAlum;

    G4Material *Si;
    G4Material *Cu;
    G4Material *Pb;
    G4Material *Ny;
    G4Material *Al;
    G4Material *Kap;
    G4Material *world_mat;
    G4Material *Steel;
    G4Material *Co57;
    G4Material *Am241;
    G4Material *Myl;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

