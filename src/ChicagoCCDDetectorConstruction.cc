// ChicagoCCDDetectorConstruction.cc
// implementation of the ChicagoCCDDetectorConstruction class

#include "ChicagoCCDDetectorConstruction.hh"

#include "defines.hh"

#include "G4RunManager.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Cons.hh"
#include "G4Orb.hh"
#include "G4Sphere.hh"
#include "G4Trd.hh"
#include "G4SubtractionSolid.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4GeometryManager.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4SolidStore.hh"
#include "G4RegionStore.hh"
#include "G4SystemOfUnits.hh"
#include "G4RotationMatrix.hh"
#include "G4tgbRotationMatrix.hh"

#include <iostream>
#include <vector>
#include <iomanip>
#include <cmath>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ChicagoCCDDetectorConstruction::ChicagoCCDDetectorConstruction()
: G4VUserDetectorConstruction()
{
  ConstructMaterials();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ChicagoCCDDetectorConstruction::~ChicagoCCDDetectorConstruction()
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ChicagoCCDDetectorConstruction::ConstructMaterials() {
  G4NistManager* nist = G4NistManager::Instance();
  Si = nist->FindOrBuildMaterial("G4_Si");
  Cu = nist->FindOrBuildMaterial("G4_Cu");
  Pb = nist->FindOrBuildMaterial("G4_Pb");
  Kap = nist->FindOrBuildMaterial("G4_KAPTON");
  std::vector<G4String> StEl {"Fe","Cr","Ni"};
  std::vector<G4double> StElWt {.66,.17,.12};
  G4double StDens = 8*g/cm3;
  Steel = nist->ConstructNewMaterial("Stainless-Steel",StEl,StElWt,StDens,true);
  world_mat = nist->FindOrBuildMaterial("G4_Galactic");
}

G4VPhysicalVolume* ChicagoCCDDetectorConstruction::Construct()
{
  return ConstructWorld();
}

G4VPhysicalVolume* ChicagoCCDDetectorConstruction::ConstructWorld()
{  
  // Get nist material manager
  G4GeometryManager::GetInstance()->OpenGeometry();
  G4PhysicalVolumeStore::GetInstance()->Clean();
  G4LogicalVolumeStore::GetInstance()->Clean();
  G4SolidStore::GetInstance()->Clean();
  // Option to switch on/off checking of volumes overlaps
  //
  G4bool checkOverlaps = false;

  //     
  // World
  //
  G4double world_sizeXY = 60*cm;
  G4double world_sizeZ  = 60*cm;

  G4Box* solidWorld = new G4Box("World", 0.5*world_sizeXY, 0.5*world_sizeXY, 0.5*world_sizeZ);     //its size
  logicWorld = new G4LogicalVolume(solidWorld,world_mat,"World");
  physWorld = new G4PVPlacement(0, G4ThreeVector(), logicWorld, "World", 0, false, 0, checkOverlaps);

//
//  Rotation Matrices
//

  std::vector<G4double> rX{1,0,0,0,0,1,0,-1,0};
  std::vector<G4double> rXNeg{1,0,0,0,0,-1,0,1,0};
  std::vector<G4double> rZXNeg{-1,0,0,0,0,-1,0,-1,0};
  std::vector<G4double> rY{0,0,1,0,1,0,-1,0,0};
  G4tgbRotationMatrix rot;
  G4RotationMatrix* rotX = rot.BuildG4RotMatrixFrom9(rX);
  G4RotationMatrix* rotXNeg = rot.BuildG4RotMatrixFrom9(rXNeg);
  G4RotationMatrix* rotY = rot.BuildG4RotMatrixFrom9(rY);
  G4RotationMatrix* rotZXNeg = rot.BuildG4RotMatrixFrom9(rZXNeg);


//
//  Chamber
//

  G4Box* solidBox = new G4Box("Box", 76.2*mm, 76.2*mm, 76.2*mm);
  G4VSolid* hole = new G4Tubs("Hole", 0, 50.8*mm, 77*mm, 0, 2*M_PI);
  G4SubtractionSolid* solidChamber = new G4SubtractionSolid("Chamber", solidBox, hole);
  solidChamber = new G4SubtractionSolid("Chamber", solidChamber, hole, rotX, G4ThreeVector());
  solidChamber = new G4SubtractionSolid("Chamber", solidChamber, hole, rotY, G4ThreeVector());
  G4LogicalVolume* logicChamber = new G4LogicalVolume(solidChamber, Steel, "Chamber");
  physChamber = new G4PVPlacement(0,G4ThreeVector(), logicChamber, "Chamber", logicWorld, false, 0, checkOverlaps);

  G4Region* steelRegion = new G4Region("SteelRegion");
  logicChamber->SetRegion(steelRegion);
  steelRegion->AddRootLogicalVolume(logicChamber);

//
//  Flanges
//

  G4VSolid* solidFullFlange = new G4Tubs("FullFlange", 0, 76.2*mm, 9.73*mm, 0, 2*M_PI);
  G4VSolid* solidSmallTubeFlange = new G4Tubs("SmallTubeFlange", 19.1*mm, 76.2*mm, 9.73*mm, 0, 2*M_PI);
  G4VSolid* solidLargeTubeFlange = new G4Tubs("LargeTubeFlange", 51.16*mm, 76.2*mm, 9.73*mm, 0, 2*M_PI);
  G4VSolid* solidLargeTube = new G4Tubs("LargeTube", 48*mm, 51.16*mm, 85.55*mm, 0, 2*M_PI);
  G4VSolid* solidSmallTube = new G4Tubs("SmallTube", 18*mm, 19.05*mm, 27.73*mm, 0, 2*M_PI);
  G4VSolid* solidSmallCap = new G4Cons("SmallCap", 18*mm, 19.05*mm, 12.37*mm, 13.42*mm, 20.31*mm, 0, 2*M_PI);
  G4VSolid* solidFrontFlangeHole = new G4Tubs("FrontFlangeHole", 0, 50.8*mm, 8.79*mm, 0, 2*M_PI);
  G4SubtractionSolid* solidFrontFlange = new G4SubtractionSolid("FrontFlange", solidFullFlange, solidFrontFlangeHole, 0, G4ThreeVector(0, 0, -0.96*mm));
  G4LogicalVolume* logicFullFlange = new G4LogicalVolume(solidFullFlange, Steel, "FullFlange");
  G4LogicalVolume* logicSmallTubeFlange = new G4LogicalVolume(solidSmallTubeFlange, Steel, "SmallTubeFlange");
  G4LogicalVolume* logicLargeTubeFlange = new G4LogicalVolume(solidLargeTubeFlange, Steel, "LargeTubeFlange");
  G4LogicalVolume* logicLargeTube = new G4LogicalVolume(solidLargeTube, Steel, "LargeTube");
  G4LogicalVolume* logicSmallTube = new G4LogicalVolume(solidSmallTube, Steel, "SmallTube");
  G4LogicalVolume* logicSmallCap = new G4LogicalVolume(solidSmallCap, Steel, "SmallCap");
  G4LogicalVolume* logicFrontFlange = new G4LogicalVolume(solidFrontFlange, Steel, "FrontFlange");
  flangePhys.push_back(new G4PVPlacement(rotX,G4ThreeVector(0,85.93*mm,0), logicFullFlange, "FullFlange", logicWorld, false, 0, checkOverlaps));
  flangePhys.push_back(new G4PVPlacement(0,G4ThreeVector(0, 0,-85.93*mm), logicFullFlange, "FullFlange", logicWorld, false, 1, checkOverlaps));
  flangePhys.push_back(new G4PVPlacement(rotX,G4ThreeVector(0,-85.93*mm,0), logicFrontFlange, "FrontFlange", logicWorld, false, 0, checkOverlaps));
  flangePhys.push_back(new G4PVPlacement(0,G4ThreeVector(0, 0,85.93*mm), logicSmallTubeFlange, "SmallTubeFlange", logicWorld, false, 1, checkOverlaps));
  flangePhys.push_back(new G4PVPlacement(rotY,G4ThreeVector(85.93*mm,0,0), logicLargeTubeFlange, "LargeTubeFlange", logicWorld, false, 0, checkOverlaps));
  flangePhys.push_back(new G4PVPlacement(rotY,G4ThreeVector(-85.93*mm,0,0), logicLargeTubeFlange, "LargeTubeFlange", logicWorld, false, 1, checkOverlaps));
  flangePhys.push_back(new G4PVPlacement(rotY,G4ThreeVector(218.11*mm,0,0), logicLargeTubeFlange, "LargeTubeFlange", logicWorld, false, 2, checkOverlaps));
  flangePhys.push_back(new G4PVPlacement(rotY,G4ThreeVector(-218.11*mm,0,0), logicLargeTubeFlange, "LargeTubeFlange", logicWorld, false, 3, checkOverlaps));
  flangePhys.push_back(new G4PVPlacement(rotY,G4ThreeVector(237.57*mm,0,0), logicLargeTubeFlange, "LargeTubeFlange", logicWorld, false, 4, checkOverlaps)); 
  flangePhys.push_back(new G4PVPlacement(rotY,G4ThreeVector(-237.57*mm,0,0), logicLargeTubeFlange, "LargeTubeFlange", logicWorld, false, 5, checkOverlaps));
  flangePhys.push_back(new G4PVPlacement(rotY,G4ThreeVector(161.75*mm,0,0), logicLargeTube, "LargeTube", logicWorld, false, 0, checkOverlaps));
  flangePhys.push_back(new G4PVPlacement(rotY,G4ThreeVector(-161.75*mm,0,0), logicLargeTube, "LargeTube", logicWorld, false, 1, checkOverlaps));
//  flangePhys.push_back(new G4PVPlacement(rotX,G4ThreeVector(0,-103.93*mm,0), logicSmallTube, "SmallTube", logicWorld, false, 0, checkOverlaps));
  flangePhys.push_back(new G4PVPlacement(0,G4ThreeVector(0,0,103.93*mm), logicSmallTube, "SmallTube", logicWorld, false, 1, checkOverlaps));
  flangePhys.push_back(new G4PVPlacement(0,G4ThreeVector(0,0,151.97*mm), logicSmallCap, "SmallCap", logicWorld, false, 1, checkOverlaps));

  G4Region* steelRegion2 = new G4Region("SteelRegion2");
  logicFullFlange->SetRegion(steelRegion);
  steelRegion->AddRootLogicalVolume(logicFullFlange);               
  logicSmallTubeFlange->SetRegion(steelRegion);
  steelRegion->AddRootLogicalVolume(logicSmallTubeFlange); 
  logicLargeTubeFlange->SetRegion(steelRegion2);
  steelRegion2->AddRootLogicalVolume(logicLargeTubeFlange); 
  logicLargeTube->SetRegion(steelRegion2);
  steelRegion2->AddRootLogicalVolume(logicLargeTube); 
  logicSmallTube->SetRegion(steelRegion2);
  steelRegion2->AddRootLogicalVolume(logicSmallTube); 
  logicSmallCap->SetRegion(steelRegion2);
  steelRegion2->AddRootLogicalVolume(logicSmallCap); 
  logicFrontFlange->SetRegion(steelRegion);
  steelRegion->AddRootLogicalVolume(logicFrontFlange);               

//
// Flex Cable
//

  G4Box* solidFlexSketch = new G4Box("FlexSketch", 75.89*mm, 20.82*mm, 0.1525*mm);
  G4Box* solidFlexHole1 = new G4Box("FlexHole1", 52.71*mm, 11.3*mm, 0.16*mm);
  G4Box* solidFlexHole2 = new G4Box("FlexHole2", 15.28*mm, 3.475*mm, 0.16*mm);
  G4SubtractionSolid* solidFlex = new G4SubtractionSolid("Flex", solidFlexSketch, solidFlexHole1, 0, G4ThreeVector(23.18*mm,0,0)); 
  solidFlex = new G4SubtractionSolid("Flex", solidFlex, solidFlexHole2, 0, G4ThreeVector(-60.61*mm,17.35*mm,0));
  solidFlex = new G4SubtractionSolid("Flex", solidFlex, solidFlexHole2, 0, G4ThreeVector(-60.61*mm,-17.35*mm,0));
  G4LogicalVolume* logicFlex = new G4LogicalVolume(solidFlex, Kap, "Flex");
  physFlex = new G4PVPlacement(rotXNeg,G4ThreeVector(-26.361*mm, -12.2175*mm, 0), logicFlex, "Flex", logicWorld, false, 0, checkOverlaps);

//
//  Silicon Backing of CCD
//
  G4Box* solidSiBacking = new G4Box("SiBacking", 53.5051*mm, 18.9992*mm, 0.3375*mm);
  G4LogicalVolume* logicSiBacking = new G4LogicalVolume(solidSiBacking, Si, "SiBacking");
  physSiBacking = new G4PVPlacement(rotXNeg,G4ThreeVector(-12.3271*mm, -11.7275*mm, 0), logicSiBacking, "SiBacking", logicWorld, false, 0, checkOverlaps);

//
//  Base of CCD
//

  G4Box* solidChicagoCCDBaseSketch = new G4Box("ChicagoCCDBaseSketch", 73.025*mm, 34.925*mm, 1.905*mm);
  G4Box* solidChicagoCCDBaseGroove1a = new G4Box("ChicagoCCDBaseGroove1a", 65.405*mm, 27.305*mm, 0.636*mm);
  G4Box* solidChicagoCCDBaseGroove1b = new G4Box("ChicagoCCDBaseGroove1b", 3.82*mm, 15.24*mm, 0.636*mm);
  G4Box* solidChicagoCCDBaseGroove2 = new G4Box("ChicagoCCDBaseGroove2", 55.753*mm, 21.1582*mm, 0.4555*mm);
  G4Box* solidChicagoCCDBaseGroove3 = new G4Box("ChicagoCCDBaseGroove3", 50.038*mm, 14.986*mm, 0.8365*mm);
  G4SubtractionSolid* solidChicagoCCDBase = new G4SubtractionSolid("ChicagoCCDBase", solidChicagoCCDBaseSketch, solidChicagoCCDBaseGroove1a, 0, G4ThreeVector(0,0,1.271*mm));
  solidChicagoCCDBase = new G4SubtractionSolid("ChicagoCCDBase", solidChicagoCCDBase, solidChicagoCCDBaseGroove1b, 0, G4ThreeVector(69.215*mm, 0, 1.271*mm));
  solidChicagoCCDBase = new G4SubtractionSolid("ChicagoCCDBase", solidChicagoCCDBase, solidChicagoCCDBaseGroove2, 0, G4ThreeVector(0, 0, 0.2015*mm));
  solidChicagoCCDBase = new G4SubtractionSolid("ChicagoCCDBase", solidChicagoCCDBase, solidChicagoCCDBaseGroove3, 0, G4ThreeVector(0, 0, -1.0805*mm));
  G4LogicalVolume* logicChicagoCCDBase = new G4LogicalVolume(solidChicagoCCDBase, Cu, "ChicagoCCDBase");
  physChicagoCCDBase = new G4PVPlacement(rotZXNeg, G4ThreeVector(-12.079*mm,-11.43*mm,0), logicChicagoCCDBase, "ChicagoCCDBase", logicWorld, false, 0, checkOverlaps);


//
//  Mount Model
//

  G4Box* solidMount = new G4Box("Mount", 5.08*mm, 9.525*mm, 9.525*mm);
  G4LogicalVolume* logicMount = new G4LogicalVolume(solidMount, Cu, "Mount");
  physMount = new G4PVPlacement(0, G4ThreeVector(55.866*mm,0,0), logicMount, "Mount", logicWorld, false, 0, checkOverlaps);

//
//  Cold Head
//

  G4VSolid* solidSmallColdHead = new G4Tubs("SmallColdHead", 0, 22.225*mm, 22.9489*mm, 0, 2*M_PI);
  G4VSolid* solidLargeColdHead = new G4Tubs("LargeColdHead", 0, 40.4622*mm, 58.9661*mm, 0, 2*M_PI);
  G4LogicalVolume* logicSmallColdHead = new G4LogicalVolume(solidSmallColdHead, Cu, "SmallColdHead");
  G4LogicalVolume* logicLargeColdHead = new G4LogicalVolume(solidLargeColdHead, Cu, "LargeColdHead");
  physSmallColdHead = new G4PVPlacement(rotY, G4ThreeVector(83.8949*mm,0,0), logicSmallColdHead, "SmallColdHead", logicWorld, false, 0, checkOverlaps);
  physLargeColdHead = new G4PVPlacement(rotY, G4ThreeVector(165.8099*mm,0,0), logicLargeColdHead, "LargeColdHead", logicWorld, false, 0, checkOverlaps);

  logicSmallColdHead->SetRegion(steelRegion2);
  steelRegion2->AddRootLogicalVolume(logicSmallColdHead);               
  logicLargeColdHead->SetRegion(steelRegion2);
  steelRegion2->AddRootLogicalVolume(logicLargeColdHead); 

//
//  Detailed CCD Model
//

  //     
  // Active Layer
  //  
  ActiveVecs.push_back(G4ThreeVector(-6.614*mm, -12.4055*mm, 0));
        
  G4Box* solidActive = new G4Box("CCDSensor", 45000*um, 7500*um, 337.5*um);
  G4LogicalVolume* logicActive = new G4LogicalVolume(solidActive, Si, "CCDSensor");

  G4Region* actRegion = new G4Region("ActiveRegion");
  logicActive->SetRegion(actRegion);
  actRegion->AddRootLogicalVolume(logicActive);               

  //
  // Gettering Layer
  //
  G4Box* solidGet = new G4Box("Gettering", 46310*um, 8535*um, 0.5*um);
  G4LogicalVolume* logicGet = new G4LogicalVolume(solidGet, Si, "Gettering");

  //
  // Dead Layers
  //
  G4Box* solidDead = new G4Box("Dead", 46310*um, 8535*um, 1*um);
  G4LogicalVolume* logicDead = new G4LogicalVolume(solidDead, Si, "Dead");

  for (unsigned int i=0; i < ActiveVecs.size(); i++) {  
    ActivePVs.push_back(new G4PVPlacement(rotXNeg, ActiveVecs[i], logicActive, "CCDSensor", logicWorld, false, i, checkOverlaps));
 
    G4double ActX = ActiveVecs[i].getX();
    G4double ActY = ActiveVecs[i].getY();
    G4double ActZ = ActiveVecs[i].getZ();

    G4ThreeVector posGet = G4ThreeVector(ActX, ActY + 338*um, ActZ);
    GetteringPVs.push_back(new G4PVPlacement(rotXNeg, posGet, logicGet, "Gettering", logicWorld, false, i, checkOverlaps));

    G4ThreeVector posDeadTop = G4ThreeVector(ActX, ActY - 338.5*um, ActZ);
    DeadTopPVs.push_back(new G4PVPlacement(rotXNeg, posDeadTop, logicDead, "Dead_Top", logicWorld, false, i, checkOverlaps));

    G4ThreeVector posDeadBottom = G4ThreeVector(ActX, ActY + 339.5*um, ActZ);
    DeadBottomPVs.push_back(new G4PVPlacement(rotXNeg, posDeadBottom, logicDead, "Dead_Bottom", logicWorld, false, i, checkOverlaps));

  }
  fScoringVolume = logicActive;

  //
  //always return the physical World
  //
  return physWorld;
}

void ChicagoCCDDetectorConstruction::ToggleGeometry()
{
  logicWorld->RemoveDaughter(physChamber);
  logicWorld->RemoveDaughter(physMount);
  logicWorld->RemoveDaughter(physSmallColdHead);
  logicWorld->RemoveDaughter(physLargeColdHead);
  logicWorld->RemoveDaughter(physFlex);
  logicWorld->RemoveDaughter(physSiBacking);
  logicWorld->RemoveDaughter(physChicagoCCDBase);
  for (unsigned int i=0; i < flangePhys.size(); i++) {
    logicWorld->RemoveDaughter(flangePhys[i]);
  };
  for (unsigned int i=0; i < ActiveVecs.size(); i++) {  
    logicWorld->RemoveDaughter(GetteringPVs[i]);
    logicWorld->RemoveDaughter(DeadTopPVs[i]);
    logicWorld->RemoveDaughter(DeadBottomPVs[i]);
  };
  G4RunManager::GetRunManager()->GeometryHasBeenModified();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4int ChicagoCCDDetectorConstruction::GetCCDNum(G4VPhysicalVolume *volume) {
  for (int i=0; i < GetTotCCDs(); i++) {
    if (volume == ActivePVs.at(i)) return i+1;
  }
  return -1;
}
