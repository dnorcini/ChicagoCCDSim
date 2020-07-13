// ChicagoCCDDetectorConstruction.cc
// implementation of the ChicagoCCDDetectorConstruction class

#include "ChicagoCCDDetectorConstruction.hh"
#include "ChicagoCCDDetectorMessenger.hh"

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
  skipperDetectorMessenger = new ChicagoCCDDetectorMessenger(this);
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

  //std::vector<G4String> StEl {"Fe","Cr","Ni"};
  //std::vector<G4double> StElWt {.66,.17,.12};
  G4double StDens = 8*g/cm3;
  Steel = new G4Material("Stainless-Steel", StDens, 9);

  G4Element* elC = nist->FindOrBuildElement("C", true);
  G4Isotope* Si28 = new G4Isotope("Si28", 14, 28);
  G4Element* elSi = new G4Element("Si", "Si", 1);
  elSi->AddIsotope(Si28, 100. * perCent);
  G4Isotope* P31 = new G4Isotope("P31", 15, 31);
  G4Element* elP = new G4Element("P", "P", 1);
  elP->AddIsotope(P31, 100. * perCent);
  G4Isotope* S32 = new G4Isotope("S32", 16, 32);
  G4Element* elS = new G4Element("S", "S", 1);
  elS->AddIsotope(S32, 100. * perCent);
  G4Isotope* Cr52 = new G4Isotope("Cr52", 24, 52);
  G4Isotope* Cr50 = new G4Isotope("Cr50", 24, 50);
  G4Element* elCr = new G4Element("Cr", "Cr", 2);
  elCr->AddIsotope(Cr52, 85. * perCent);
  elCr->AddIsotope(Cr50, 15. * perCent);
  G4Isotope* Mn55 = new G4Isotope("Mn55", 25, 55);
  G4Element* elMn = new G4Element("Mn", "Mn", 1);
  elMn->AddIsotope(Mn55, 100. * perCent);
  G4Isotope* Fe56 = new G4Isotope("Fe56", 26, 56);
  G4Element* elFe = new G4Element("Fe", "Fe", 1);
  elFe->AddIsotope(Fe56, 100. * perCent);
  G4Isotope* Ni58 = new G4Isotope("Ni58", 24, 58);
  G4Isotope* Ni62 = new G4Isotope("Ni62", 24, 62);
  G4Element* elNi = new G4Element("Ni", "Ni", 2);
  elNi->AddIsotope(Ni58, 96. * perCent);
  elNi->AddIsotope(Ni62, 4. * perCent);
  G4Isotope* Mo97 = new G4Isotope("Mo97", 42, 97);
  G4Element* elMo = new G4Element("Mo", "Mo", 1);
  elMo->AddIsotope(Mo97, 100. * perCent);

  Steel->AddElement(elC, 0.0019);
  Steel->AddElement(elSi, 0.010048);
  Steel->AddElement(elP, 0.000413);
  Steel->AddElement(elS, 0.00026);
  Steel->AddElement(elCr, 0.181986);
  Steel->AddElement(elMn, 0.010274);
  Steel->AddElement(elFe, 0.666811);
  Steel->AddElement(elNi, 0.113803);
  Steel->AddElement(elMo, 0.014504);

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

  // **KEEP IN MIND, ALL DIMENSIONS ARE HALF-DIMENSIONS**

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
  std::vector<G4double> rXNegZNeg{0,1,0,0,0,1,1,0,0};
  G4tgbRotationMatrix rot;
  G4RotationMatrix* rotX = rot.BuildG4RotMatrixFrom9(rX);
  G4RotationMatrix* rotXNeg = rot.BuildG4RotMatrixFrom9(rXNeg);
  G4RotationMatrix* rotY = rot.BuildG4RotMatrixFrom9(rY);
  G4RotationMatrix* rotZXNeg = rot.BuildG4RotMatrixFrom9(rZXNeg);
  G4RotationMatrix* rotXNegZNeg = rot.BuildG4RotMatrixFrom9(rXNegZNeg);

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

  G4VSolid* solidFullFlange = new G4Tubs("FullFlange", 0, 76.2*mm, 10.5*mm, 0, 2*M_PI);
  G4VSolid* solidSmallTubeFlange = new G4Tubs("SmallTubeFlange", 19.1*mm, 76.2*mm, 10.5*mm, 0, 2*M_PI);
  G4VSolid* solidNippleFlange = new G4Tubs("NippleFlange", 51.16*mm, 76.2*mm, 10.5*mm, 0, 2*M_PI);
  G4VSolid* solidNipple = new G4Tubs("Nipple", 48*mm, 51.16*mm, 85.55*mm, 0, 2*M_PI);
  G4VSolid* solidSmallTube = new G4Tubs("SmallTube", 18*mm, 19.05*mm, 27.73*mm, 0, 2*M_PI);
  G4VSolid* solidSmallCap = new G4Cons("SmallCap", 18*mm, 19.05*mm, 12.37*mm, 13.42*mm, 20.31*mm, 0, 2*M_PI);
  G4VSolid* solidFrontFlangeHole = new G4Tubs("FrontFlangeHole", 0, 50.8*mm, 8.6*mm, 0, 2*M_PI);
  G4SubtractionSolid* solidFrontFlange = new G4SubtractionSolid("FrontFlange", solidFullFlange, solidFrontFlangeHole, 0, G4ThreeVector(0, 0, -2.1*mm));
  G4LogicalVolume* logicFullFlange = new G4LogicalVolume(solidFullFlange, Steel, "FullFlange");
  G4LogicalVolume* logicSmallTubeFlange = new G4LogicalVolume(solidSmallTubeFlange, Steel, "SmallTubeFlange");
  G4LogicalVolume* logicNippleFlange = new G4LogicalVolume(solidNippleFlange, Steel, "NippleFlange");
  G4LogicalVolume* logicNipple = new G4LogicalVolume(solidNipple, Steel, "Nipple");
  G4LogicalVolume* logicSmallTube = new G4LogicalVolume(solidSmallTube, Steel, "SmallTube");
  G4LogicalVolume* logicSmallCap = new G4LogicalVolume(solidSmallCap, Steel, "SmallCap");
  G4LogicalVolume* logicFrontFlange = new G4LogicalVolume(solidFrontFlange, Steel, "FrontFlange");
  flangePhys.push_back(new G4PVPlacement(rotX,G4ThreeVector(0,86.7*mm,0), logicFullFlange, "BackFlange", logicWorld, false, 0, checkOverlaps));
  flangePhys.push_back(new G4PVPlacement(0,G4ThreeVector(0, 0,-86.7*mm), logicFullFlange, "BottomRestFlange", logicWorld, false, 1, checkOverlaps));
  flangePhys.push_back(new G4PVPlacement(rotXNeg,G4ThreeVector(0,-86.7*mm,0), logicFrontFlange, "FrontFlange", logicWorld, false, 0, checkOverlaps));
  flangePhys.push_back(new G4PVPlacement(0,G4ThreeVector(0, 0,86.7*mm), logicFullFlange, "TopRestFlange", logicWorld, false, 1, checkOverlaps));
  flangePhys.push_back(new G4PVPlacement(rotY,G4ThreeVector(86.7*mm,0,0), logicNippleFlange, "RightNippleFlange", logicWorld, false, 0, checkOverlaps));
  flangePhys.push_back(new G4PVPlacement(rotY,G4ThreeVector(-86.7*mm,0,0), logicNippleFlange, "LeftNippleFlange", logicWorld, false, 1, checkOverlaps));
  flangePhys.push_back(new G4PVPlacement(rotY,G4ThreeVector(215.8*mm,0,0), logicNippleFlange, "RightNippleFlange", logicWorld, false, 2, checkOverlaps));
  flangePhys.push_back(new G4PVPlacement(rotY,G4ThreeVector(-215.8*mm,0,0), logicNippleFlange, "LeftNippleFlange", logicWorld, false, 3, checkOverlaps));
  flangePhys.push_back(new G4PVPlacement(rotY,G4ThreeVector(236.8*mm,0,0), logicNippleFlange, "RightNippleFlange", logicWorld, false, 4, checkOverlaps)); 
  flangePhys.push_back(new G4PVPlacement(rotY,G4ThreeVector(-236.8*mm,0,0), logicNippleFlange, "LeftNippleFlange", logicWorld, false, 5, checkOverlaps));
  flangePhys.push_back(new G4PVPlacement(rotY,G4ThreeVector(161.75*mm,0,0), logicNipple, "RightNipple", logicWorld, false, 0, checkOverlaps));
  flangePhys.push_back(new G4PVPlacement(rotY,G4ThreeVector(-161.75*mm,0,0), logicNipple, "LeftNipple", logicWorld, false, 1, checkOverlaps));

  G4Region* steelRegion2 = new G4Region("SteelRegion2");
  logicFullFlange->SetRegion(steelRegion);
  steelRegion->AddRootLogicalVolume(logicFullFlange);               
  logicSmallTubeFlange->SetRegion(steelRegion);
  steelRegion->AddRootLogicalVolume(logicSmallTubeFlange); 
  logicNippleFlange->SetRegion(steelRegion2);
  steelRegion2->AddRootLogicalVolume(logicNippleFlange); 
  logicNipple->SetRegion(steelRegion2);
  steelRegion2->AddRootLogicalVolume(logicNipple); 
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
//  Copper Box of CCD
//

  G4Box* solidCopperBoxSketch = new G4Box("CopperBoxSketch", 73.025*mm, 34.925*mm, 1.905*mm);
  G4Box* solidCopperBoxGroove1a = new G4Box("CopperBoxGroove1a", 65.405*mm, 27.305*mm, 0.636*mm);
  G4Box* solidCopperBoxGroove1b = new G4Box("CopperBoxGroove1b", 3.82*mm, 15.24*mm, 0.636*mm);
  G4Box* solidCopperBoxGroove2 = new G4Box("CopperBoxGroove2", 55.753*mm, 21.1582*mm, 0.4555*mm);
  G4Box* solidCopperBoxGroove3 = new G4Box("CopperBoxGroove3", 50.038*mm, 14.986*mm, 0.8365*mm);
  G4SubtractionSolid* solidCopperBox = new G4SubtractionSolid("CopperBox", solidCopperBoxSketch, solidCopperBoxGroove1a, 0, G4ThreeVector(0,0,1.271*mm));
  solidCopperBox = new G4SubtractionSolid("CopperBox", solidCopperBox, solidCopperBoxGroove1b, 0, G4ThreeVector(69.215*mm, 0, 1.271*mm));
  solidCopperBox = new G4SubtractionSolid("CopperBox", solidCopperBox, solidCopperBoxGroove2, 0, G4ThreeVector(0, 0, 0.2015*mm));
  solidCopperBox = new G4SubtractionSolid("CopperBox", solidCopperBox, solidCopperBoxGroove3, 0, G4ThreeVector(0, 0, -1.0805*mm));
  G4LogicalVolume* logicCopperBox = new G4LogicalVolume(solidCopperBox, Cu, "CopperBox");
  physCopperBox = new G4PVPlacement(rotZXNeg, G4ThreeVector(-12.079*mm,-11.43*mm,0), logicCopperBox, "CopperBox", logicWorld, false, 0, checkOverlaps);

//
//  Copper Face Plate
//

  G4Box* solidCopperFacePlateFull = new G4Box("CopperFacePlateFull", 64.77*mm, 26.67*mm, 0.795*mm);
  G4Box* solidCopperFaceHole = new G4Box("CopperFaceHole", 56.515*mm, 18.8*mm, 0.8*mm);
  G4SubtractionSolid* solidCopperFacePlate = new G4SubtractionSolid("CopperFacePlate", solidCopperFacePlateFull, solidCopperFaceHole, 0, G4ThreeVector());
  G4LogicalVolume* logicCopperFacePlate = new G4LogicalVolume(solidCopperFacePlate, Cu, "CopperFacePlate");
  physCopperFacePlate = new G4PVPlacement(rotZXNeg, G4ThreeVector(-12.079*mm,-13.0105*mm,0), logicCopperFacePlate, "CopperFacePlate", logicWorld, false, 0, checkOverlaps);

//
//  Puck Model
//

  G4Box* solidPuck = new G4Box("Puck", 5.08*mm, 9.525*mm, 9.525*mm);
  G4LogicalVolume* logicPuck = new G4LogicalVolume(solidPuck, Cu, "Puck");
  physPuck = new G4PVPlacement(0, G4ThreeVector(55.866*mm,0,0), logicPuck, "Puck", logicWorld, false, 0, checkOverlaps);

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
//  Detailed CCD Model - One of two possibilities:
//  1. (Default) Include dead layer in front as part of detecting region, so that diffusion of this area can be considered so long as we artificially drop efficiency
//  2. Dead layer in front is a separate volume, detection will only occur within the active layer, and assume full efficiency for this region
//

  //     
  // Active Layer
  //  
  ActiveVecs.push_back(std::make_pair(G4ThreeVector(-6.614*mm, -11.9055*mm, 0), rotXNegZNeg));
  ActiveDims.push_back(G4ThreeVector(7680*um, 46320*um, 340*um));

  G4Box* solidActive = new G4Box("CCDSensor", ActiveDims[0].getX(), ActiveDims[0].getY(), ActiveDims[0].getZ());
  G4LogicalVolume* logicActive = new G4LogicalVolume(solidActive, Si, "CCDSensor");

  G4Region* actRegion = new G4Region("ActiveRegion");
  logicActive->SetRegion(actRegion);
  actRegion->AddRootLogicalVolume(logicActive);               

  //
  // Dead Shell
  //
  G4Box* solidDeadFull = new G4Box("DeadShell", ActiveDims[0].getX() + 1291*um, ActiveDims[0].getY() + 1002*um, ActiveDims[0].getZ());
  G4Box* solidActiveHole = new G4Box("ActiveHole", ActiveDims[0].getX(), ActiveDims[0].getY(), ActiveDims[0].getZ() + 10*um);
  G4SubtractionSolid* solidDeadShell = new G4SubtractionSolid("DeadShell", solidDeadFull, solidActiveHole, 0, G4ThreeVector());
  G4LogicalVolume* logicDeadShell = new G4LogicalVolume(solidDeadShell, Si, "Dead");

  for (unsigned int i=0; i < ActiveVecs.size(); i++) {  
    ActivePVs.push_back(new G4PVPlacement(ActiveVecs[i].second, ActiveVecs[i].first, logicActive, "CCDSensor", logicWorld, false, i, checkOverlaps));
    DeadTopPVs.push_back(new G4PVPlacement(ActiveVecs[i].second, ActiveVecs[i].first, logicDeadShell, "Dead", logicWorld, false, i, checkOverlaps));
  }

/*
  //     
  // Active Layer
  //  
  ActiveVecs.push_back(std::make_pair(G4ThreeVector(-6.614*mm, -12.4055*mm, 0), rotXNegZNeg));
  ActiveDims.push_back(G4ThreeVector(7680*um, 46320*um, 337.5*um));

  G4Box* solidActive = new G4Box("CCDSensor", ActiveDims[0].getX(), ActiveDims[0].getY(), ActiveDims[0].getZ());
  G4LogicalVolume* logicActive = new G4LogicalVolume(solidActive, Si, "CCDSensor");

  G4Region* actRegion = new G4Region("ActiveRegion");
  logicActive->SetRegion(actRegion);
  actRegion->AddRootLogicalVolume(logicActive);               

  //
  // Gettering Layer
  //
  G4Box* solidGet = new G4Box("Gettering", 47322*um, 8971*um, 0.5*um);
  G4LogicalVolume* logicGet = new G4LogicalVolume(solidGet, Si, "Gettering");

  //
  // Dead Layers
  //
  G4Box* solidDead = new G4Box("Dead", 47322*um, 8971*um, 1*um);
  G4LogicalVolume* logicDead = new G4LogicalVolume(solidDead, Si, "Dead");

  for (unsigned int i=0; i < ActiveVecs.size(); i++) {  
    ActivePVs.push_back(new G4PVPlacement(ActiveVecs[i].second, ActiveVecs[i].first, logicActive, "CCDSensor", logicWorld, false, i, checkOverlaps));
 
    G4double ActX = ActiveVecs[i].first.getX();
    G4double ActY = ActiveVecs[i].first.getY();
    G4double ActZ = ActiveVecs[i].first.getZ();

    G4ThreeVector posGet = G4ThreeVector(ActX, ActY + 338*um, ActZ);
    GetteringPVs.push_back(new G4PVPlacement(rotXNeg, posGet, logicGet, "Gettering", logicWorld, false, i, checkOverlaps));

    G4ThreeVector posDeadTop = G4ThreeVector(ActX, ActY - 338.5*um, ActZ);
    DeadTopPVs.push_back(new G4PVPlacement(rotXNeg, posDeadTop, logicDead, "Dead_Top", logicWorld, false, i, checkOverlaps));

    G4ThreeVector posDeadBottom = G4ThreeVector(ActX, ActY + 339.5*um, ActZ);
    DeadBottomPVs.push_back(new G4PVPlacement(rotXNeg, posDeadBottom, logicDead, "Dead_Bottom", logicWorld, false, i, checkOverlaps));

  }
*/
  fScoringVolume = logicActive;

  //
  //always return the physical World
  //
  return physWorld;
}

void ChicagoCCDDetectorConstruction::ToggleGeometry()
{
  // For testing purposes, this deletes all geometry except for the Active CCD layer
  logicWorld->RemoveDaughter(physChamber);
  logicWorld->RemoveDaughter(physPuck);
  logicWorld->RemoveDaughter(physSmallColdHead);
  logicWorld->RemoveDaughter(physLargeColdHead);
  logicWorld->RemoveDaughter(physFlex);
  logicWorld->RemoveDaughter(physCopperFacePlate);
  logicWorld->RemoveDaughter(physSiBacking);
  logicWorld->RemoveDaughter(physCopperBox);
  for (unsigned int i=0; i < flangePhys.size(); i++) {
    logicWorld->RemoveDaughter(flangePhys[i]);
  };
  for (unsigned int i=0; i < GetteringPVs.size(); i++) {  
    logicWorld->RemoveDaughter(GetteringPVs[i]);
  };
  for (unsigned int i=0; i < DeadTopPVs.size(); i++) {  
    logicWorld->RemoveDaughter(DeadTopPVs[i]);
  };
  for (unsigned int i=0; i < DeadBottomPVs.size(); i++) {  
    logicWorld->RemoveDaughter(DeadBottomPVs[i]);
  };
  G4RunManager::GetRunManager()->GeometryHasBeenModified();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4int ChicagoCCDDetectorConstruction::GetCCDNum(G4VPhysicalVolume *volume) {
  // If there are one or multiple CCDs, return the index of the CCD of the volume that was passed in, else return -1
  for (int i=0; i < GetTotCCDs(); i++) {
    if (volume == ActivePVs.at(i)) return i+1;
  }
  return -1;
}
