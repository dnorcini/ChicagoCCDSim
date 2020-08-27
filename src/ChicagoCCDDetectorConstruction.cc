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
  Al = nist->FindOrBuildMaterial("G4_Al");
  Kap = nist->FindOrBuildMaterial("G4_KAPTON");

  G4double StDens = 8*g/cm3;
  Steel = new G4Material("Stainless-Steel", StDens, 8);

  G4Element* elC = nist->FindOrBuildElement("C", true);
  G4Element* elSi = nist->FindOrBuildElement("Si", true);
  G4Isotope* P31 = new G4Isotope("P31", 15, 31);
  G4Element* elP = new G4Element("P", "P", 1);
  elP->AddIsotope(P31, 100. * perCent);
  G4Element* elS = nist->FindOrBuildElement("S", true);
  G4Element* elCr = nist->FindOrBuildElement("Cr", true);
  G4Isotope* Mn55 = new G4Isotope("Mn55", 25, 55);
  G4Element* elMn = new G4Element("Mn", "Mn", 1);
  elMn->AddIsotope(Mn55, 100. * perCent);
  G4Element* elFe = nist->FindOrBuildElement("Fe", true);
  G4Element* elNi = nist->FindOrBuildElement("Ni", true);

  Steel->AddElement(elC, 0.0004);
  Steel->AddElement(elSi, 0.005);
  Steel->AddElement(elP, 0.000225);
  Steel->AddElement(elS, 0.00015);
  Steel->AddElement(elCr, 0.19);
  Steel->AddElement(elMn, 0.01);
  Steel->AddElement(elFe, 0.701725);
  Steel->AddElement(elNi, 0.0925);

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
  checkOverlaps = false;

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

  std::vector<G4double> rID{ 0    ,  0    , 0};
  std::vector<G4double> rX{ 90*deg,  0    , 0};
  std::vector<G4double> rY{  0    , 90*deg, 0};
  G4tgbRotationMatrix rot;
  G4RotationMatrix* rotID = rot.BuildG4RotMatrixFrom3(rID);
  G4RotationMatrix* rotX  = rot.BuildG4RotMatrixFrom3(rX );
  G4RotationMatrix* rotY  = rot.BuildG4RotMatrixFrom3(rY );

//
//  Chamber
//

  G4Box* solidBox = new G4Box("Box", 76.2*mm, 76.2*mm, 76.2*mm);
  G4VSolid* hole  = new G4Tubs("Hole", 0, 50.8*mm, 77*mm, 0, 2*M_PI);
  G4SubtractionSolid* solidChamber = new G4SubtractionSolid("Chamber", solidBox,     hole);
  solidChamber                     = new G4SubtractionSolid("Chamber", solidChamber, hole, rotX, G4ThreeVector());
  solidChamber                     = new G4SubtractionSolid("Chamber", solidChamber, hole, rotY, G4ThreeVector());
  G4LogicalVolume* logicChamber = new G4LogicalVolume(solidChamber, Steel, "Chamber");
  physChamber = new G4PVPlacement(0,G4ThreeVector(), logicChamber, "Chamber", logicWorld, false, 0, checkOverlaps);

  G4Region* steelRegion = new G4Region("SteelRegion");
  logicChamber->SetRegion(steelRegion);
  steelRegion->AddRootLogicalVolume(logicChamber);

//
//  Flanges
//

  G4VSolid* solidFullFlange            = new G4Tubs("FullFlange"       ,  0      , 76.2 *mm,  9.525*mm, 0, 2*M_PI);
  G4VSolid* solidFullAlumFlange        = new G4Tubs("FullAlumFlange"   ,  0      , 76.2 *mm, 10.75 *mm, 0, 2*M_PI);
  G4VSolid* solidSmallTubeFlange       = new G4Tubs("SmallTubeFlange"  , 19.1 *mm, 76.2 *mm,  9.525*mm, 0, 2*M_PI);
  G4VSolid* solidNippleFlange          = new G4Tubs("NippleFlange"     , 51.16*mm, 76.2 *mm,  9.525*mm, 0, 2*M_PI);
  G4VSolid* solidNippleElecFlange      = new G4Tubs("NippleElecFlange" , 51.16*mm, 76.2 *mm,  7.4  *mm, 0, 2*M_PI);
  G4VSolid* solidNipple                = new G4Tubs("Nipple"           , 48   *mm, 51.16*mm, 85.55 *mm, 0, 2*M_PI);
  G4VSolid* solidSmallTube             = new G4Tubs("SmallTube"        , 18   *mm, 19.1 *mm, 27.73 *mm, 0, 2*M_PI);
  G4VSolid* solidSmallLid              = new G4Tubs("SmallLid"         ,  0      , 21.  *mm,  7.4  *mm, 0, 2*M_PI);
  G4VSolid* solidSmallCap              = new G4Cons("SmallCap", 18*mm, 19.05*mm, 12.37*mm, 13.42 *mm, 20.31*mm, 0, 2*M_PI);
  G4VSolid* solidFrontFlangeHole       = new G4Tubs("FrontFlangeHole"  ,  0      , 50.8 *mm,  8.735*mm, 0, 2*M_PI);
  G4VSolid* solidBackFlangeHole        = new G4Tubs("BackFlangeHole"   ,  0      , 48.93*mm,  8.95 *mm, 0, 2*M_PI);
  G4SubtractionSolid* solidFrontFlange = new G4SubtractionSolid("FrontFlange", solidFullFlange    , solidFrontFlangeHole, 0, G4ThreeVector(0, 0,-0.8 *mm));
  G4SubtractionSolid* solidBackFlange  = new G4SubtractionSolid("BackFlange" , solidFullAlumFlange, solidBackFlangeHole , 0, G4ThreeVector(0, 0, 1.85*mm));
  G4LogicalVolume* logicFullFlange       = new G4LogicalVolume(solidFullFlange     , Steel, "FullFlange"      );
  G4LogicalVolume* logicSmallTubeFlange  = new G4LogicalVolume(solidSmallTubeFlange, Steel, "SmallTubeFlange" );
  G4LogicalVolume* logicNippleElecFlange = new G4LogicalVolume(solidNippleFlange   , Steel, "NippleElecFlange");
  G4LogicalVolume* logicNippleFlange     = new G4LogicalVolume(solidNippleFlange   , Steel, "NippleFlange"    );
  G4LogicalVolume* logicNipple           = new G4LogicalVolume(solidNipple         , Steel, "Nipple"          );
  G4LogicalVolume* logicSmallTube        = new G4LogicalVolume(solidSmallTube      , Steel, "SmallTube"       );
  G4LogicalVolume* logicSmallLid         = new G4LogicalVolume(solidSmallLid       , Steel, "SmallLid"       );
  G4LogicalVolume* logicSmallCap         = new G4LogicalVolume(solidSmallCap       , Steel, "SmallCap"        );
  G4LogicalVolume* logicFrontFlange      = new G4LogicalVolume(solidFrontFlange    , Steel, "FrontFlange"     );
  G4LogicalVolume* logicBackFlange       = new G4LogicalVolume(solidBackFlange     , Steel, "BackFlange"      );
  flangePhys.push_back(new G4PVPlacement(0   , G4ThreeVector(  0       ,   0       , -86.95 *mm), logicBackFlange      , "BackFlange"       , logicWorld, false, 0, checkOverlaps));
  flangePhys.push_back(new G4PVPlacement(rotY, G4ThreeVector( 85.725*mm,   0       ,   0       ), logicFullFlange      , "BottomRestFlange" , logicWorld, false, 1, checkOverlaps));
  flangePhys.push_back(new G4PVPlacement(0   , G4ThreeVector(  0       ,   0       ,  85.725*mm), logicSmallTubeFlange , "FrontFlange"      , logicWorld, false, 0, checkOverlaps));
  flangePhys.push_back(new G4PVPlacement(0   , G4ThreeVector(  0       ,   0       , 103.93 *mm), logicSmallTube       , "SmallTube"        , logicWorld, false, 0, checkOverlaps));
  flangePhys.push_back(new G4PVPlacement(0   , G4ThreeVector(  0       ,   0       , 139.06 *mm), logicSmallLid        , "SmallLid"         , logicWorld, false, 0, checkOverlaps));
  flangePhys.push_back(new G4PVPlacement(rotY, G4ThreeVector(-85.725*mm,   0       ,   0       ), logicSmallTubeFlange , "TopRestFlange"    , logicWorld, false, 1, checkOverlaps));
  flangePhys.push_back(new G4PVPlacement(rotX, G4ThreeVector(  0       ,  85.725*mm,   0       ), logicNippleFlange    , "RightNippleFlange", logicWorld, false, 0, checkOverlaps));
  flangePhys.push_back(new G4PVPlacement(rotX, G4ThreeVector(  0       , -85.725*mm,   0       ), logicNippleFlange    , "LeftNippleFlange" , logicWorld, false, 1, checkOverlaps));
  flangePhys.push_back(new G4PVPlacement(rotX, G4ThreeVector(  0       , 217.3  *mm,   0       ), logicNippleFlange    , "RightNippleFlange", logicWorld, false, 2, checkOverlaps));
  flangePhys.push_back(new G4PVPlacement(rotX, G4ThreeVector(  0       ,-217.3  *mm,   0       ), logicNippleFlange    , "LeftNippleFlange" , logicWorld, false, 3, checkOverlaps));
  flangePhys.push_back(new G4PVPlacement(rotX, G4ThreeVector(  0       , 237.3  *mm,   0       ), logicNippleFlange    , "RightNippleFlange", logicWorld, false, 4, checkOverlaps)); 
  flangePhys.push_back(new G4PVPlacement(rotX, G4ThreeVector(  0       ,-237.3  *mm,   0       ), logicNippleElecFlange, "LeftNippleFlange" , logicWorld, false, 5, checkOverlaps));
  flangePhys.push_back(new G4PVPlacement(rotX, G4ThreeVector(  0       , 161.75 *mm,   0       ), logicNipple          , "RightNipple"      , logicWorld, false, 0, checkOverlaps));
  flangePhys.push_back(new G4PVPlacement(rotX, G4ThreeVector(  0       ,-161.75 *mm,   0       ), logicNipple          , "LeftNipple"       , logicWorld, false, 1, checkOverlaps));

  G4Region* steelRegion2 = new G4Region("SteelRegion2");
  logicFullFlange->SetRegion(steelRegion);
  steelRegion->AddRootLogicalVolume(logicFullFlange);               
  logicSmallTubeFlange->SetRegion(steelRegion);
  steelRegion->AddRootLogicalVolume(logicSmallTubeFlange); 
  logicNippleFlange->SetRegion(steelRegion2);
  steelRegion2->AddRootLogicalVolume(logicNippleFlange); 
  logicNippleElecFlange->SetRegion(steelRegion2);
  steelRegion2->AddRootLogicalVolume(logicNippleElecFlange); 
  logicNipple->SetRegion(steelRegion2);
  steelRegion2->AddRootLogicalVolume(logicNipple); 
  logicSmallTube->SetRegion(steelRegion2);
  steelRegion2->AddRootLogicalVolume(logicSmallTube); 
  logicSmallLid->SetRegion(steelRegion2);
  steelRegion2->AddRootLogicalVolume(logicSmallLid); 
  logicSmallCap->SetRegion(steelRegion2);
  steelRegion2->AddRootLogicalVolume(logicSmallCap); 
  logicFrontFlange->SetRegion(steelRegion);
  steelRegion->AddRootLogicalVolume(logicFrontFlange);               
  logicBackFlange->SetRegion(steelRegion);
  steelRegion->AddRootLogicalVolume(logicBackFlange);               

//
// Flex Cable
//

  G4Box* solidFlexSketch = new G4Box("FlexSketch", 20.82 *mm,  75.89*mm,  0.1525*mm);
  G4Box* solidFlexHole1  = new G4Box("FlexHole1",  11.3  *mm,  52.71*mm,  0.16  *mm);
  G4Box* solidFlexHole2  = new G4Box("FlexHole2",   3.475*mm,  15.28*mm,  0.16  *mm);
  G4SubtractionSolid* solidFlex = new G4SubtractionSolid("Flex", solidFlexSketch, solidFlexHole1, 0, G4ThreeVector(  0      , 23.18*mm, 0)); 
  solidFlex                     = new G4SubtractionSolid("Flex", solidFlex      , solidFlexHole2, 0, G4ThreeVector( 17.35*mm,-60.61*mm, 0));
  solidFlex                     = new G4SubtractionSolid("Flex", solidFlex      , solidFlexHole2, 0, G4ThreeVector(-17.35*mm,-60.61*mm, 0));
  G4LogicalVolume* logicFlex = new G4LogicalVolume(solidFlex, Kap, "Flex");
  physFlex = new G4PVPlacement(0,G4ThreeVector(0, -26.361*mm, -20.1675*mm), logicFlex, "Flex", logicWorld, false, 0, checkOverlaps);

//
//  Silicon Backing of CCD
//
  G4Box* solidSiBacking = new G4Box("SiBacking", 18.9992*mm, 53.5051*mm, 0.3375*mm);
  G4LogicalVolume* logicSiBacking = new G4LogicalVolume(solidSiBacking, Si, "SiBacking");
  physSiBacking = new G4PVPlacement(0, G4ThreeVector(0, -12.3271*mm, -20.6575*mm), logicSiBacking, "SiBacking", logicWorld, false, 0, checkOverlaps);

//
//  Copper Box of CCD
//

  G4Box* solidCopperBoxSketch   = new G4Box("CopperBoxSketch",   34.925 *mm, 73.025*mm, 1.905 *mm);
  G4Box* solidCopperBoxGroove1a = new G4Box("CopperBoxGroove1a", 27.305 *mm, 65.405*mm, 0.636 *mm);
  G4Box* solidCopperBoxGroove1b = new G4Box("CopperBoxGroove1b", 15.24  *mm,  3.82 *mm, 0.636 *mm);
  G4Box* solidCopperBoxGroove2  = new G4Box("CopperBoxGroove2",  21.1582*mm, 55.753*mm, 0.4555*mm);
  G4Box* solidCopperBoxGroove3  = new G4Box("CopperBoxGroove3",  14.986 *mm, 50.038*mm, 0.8365*mm);
  G4SubtractionSolid* solidCopperBox = new G4SubtractionSolid("CopperBox", solidCopperBoxSketch, solidCopperBoxGroove1a, 0, G4ThreeVector(0,  0       , 1.271 *mm));
  solidCopperBox                     = new G4SubtractionSolid("CopperBox", solidCopperBox      , solidCopperBoxGroove1b, 0, G4ThreeVector(0,-69.215*mm, 1.271 *mm));
  solidCopperBox                     = new G4SubtractionSolid("CopperBox", solidCopperBox      , solidCopperBoxGroove2 , 0, G4ThreeVector(0,  0       , 0.2015*mm));
  solidCopperBox                     = new G4SubtractionSolid("CopperBox", solidCopperBox      , solidCopperBoxGroove3 , 0, G4ThreeVector(0,  0       ,-1.0805*mm));
  G4LogicalVolume* logicCopperBox = new G4LogicalVolume(solidCopperBox, Cu, "CopperBox");
  physCopperBox = new G4PVPlacement(0, G4ThreeVector(0, -12.079*mm, -20.955*mm), logicCopperBox, "CopperBox", logicWorld, false, 0, checkOverlaps);

//
//  Copper Face Plate
//

  G4Box* solidCopperFacePlateFull = new G4Box("CopperFacePlateFull", 26.67*mm, 64.77 *mm, 0.795*mm);
  G4Box* solidCopperFaceHole      = new G4Box("CopperFaceHole"     , 18.8 *mm, 56.515*mm, 0.8  *mm);
  G4SubtractionSolid* solidCopperFacePlate = new G4SubtractionSolid("CopperFacePlate", solidCopperFacePlateFull, solidCopperFaceHole, 0, G4ThreeVector());
  G4LogicalVolume* logicCopperFacePlate = new G4LogicalVolume(solidCopperFacePlate, Cu, "CopperFacePlate");
  physCopperFacePlate = new G4PVPlacement(0, G4ThreeVector(0, -12.079*mm, -19.3745*mm), logicCopperFacePlate, "CopperFacePlate", logicWorld, false, 0, checkOverlaps);

//
//  Puck Model
//

  G4Box* solidPuck = new G4Box("Puck", 19.05*mm, 5.08*mm, 19.05*mm);
  G4LogicalVolume* logicPuck = new G4LogicalVolume(solidPuck, Cu, "Puck");
  physPuck = new G4PVPlacement(0, G4ThreeVector(0, 55.866*mm, 0), logicPuck, "Puck", logicWorld, false, 0, checkOverlaps);

//
//  Cold Head
//

  G4VSolid* solidSmallColdHead = new G4Tubs("SmallColdHead", 0, 22.225 *mm, 22.9489*mm, 0, 2*M_PI);
  G4VSolid* solidLargeColdHead = new G4Tubs("LargeColdHead", 0, 40.4622*mm, 58.9661*mm, 0, 2*M_PI);
  G4LogicalVolume* logicSmallColdHead = new G4LogicalVolume(solidSmallColdHead, Cu, "SmallColdHead");
  G4LogicalVolume* logicLargeColdHead = new G4LogicalVolume(solidLargeColdHead, Cu, "LargeColdHead");
  physSmallColdHead = new G4PVPlacement(rotX, G4ThreeVector(0, 83.8949*mm,0), logicSmallColdHead, "SmallColdHead", logicWorld, false, 0, checkOverlaps);
  physLargeColdHead = new G4PVPlacement(rotX, G4ThreeVector(0,165.8099*mm,0), logicLargeColdHead, "LargeColdHead", logicWorld, false, 0, checkOverlaps);

  logicSmallColdHead->SetRegion(steelRegion2);
  steelRegion2->AddRootLogicalVolume(logicSmallColdHead);               
  logicLargeColdHead->SetRegion(steelRegion2);
  steelRegion2->AddRootLogicalVolume(logicLargeColdHead); 

//
//  Detailed CCD Model - One of two possibilities:
//  1. (Default) Include dead layer in front as part of detecting region, so that diffusion of this area can be considered so long as we artificially drop efficiency
//  2. Dead layer in front is a separate volume, detection will only occur within the active layer, and assume full efficiency for this region
//

// Option 1

  //     
  // Active Layer
  //  
  ActiveVecs.push_back(std::make_pair(G4ThreeVector(0, -6.614*mm, -19.98*mm), rotID));
  ActiveDims1.push_back(G4ThreeVector(7680*um, 46320*um, 340*um));

  G4Box* solidActiveFull = new G4Box("CCDSensor", ActiveDims1[0].getX(), ActiveDims1[0].getY(), ActiveDims1[0].getZ());
  ActiveFullLVs.push_back(new G4LogicalVolume(solidActiveFull, Si, "CCDSensor"));

  G4Region* actRegion = new G4Region("ActiveRegion");
  ActiveFullLVs[0]->SetRegion(actRegion);
  actRegion->AddRootLogicalVolume(ActiveFullLVs[0]);               

  //
  // Dead Shell
  //
  G4Box* solidDeadFull       = new G4Box("DeadShell" , ActiveDims1[0].getX() + 1291*um, ActiveDims1[0].getY() + 1002*um, ActiveDims1[0].getZ()        );
  G4Box* solidActiveFullHole = new G4Box("ActiveHole", ActiveDims1[0].getX()          , ActiveDims1[0].getY()          , ActiveDims1[0].getZ() + 10*um);
  G4SubtractionSolid* solidDeadShell = new G4SubtractionSolid("DeadShell", solidDeadFull, solidActiveFullHole, 0, G4ThreeVector());
  DeadShellLVs.push_back(new G4LogicalVolume(solidDeadShell, Si, "Dead"));

// Option 2

  //     
  // Active Layer
  //  
  ActiveDims2.push_back(G4ThreeVector(7680*um, 46320*um, 337.5*um));
  std::vector<G4ThreeVector> DeadDims;
  for (unsigned int i=0; i < ActiveDims2.size(); i++) {
    DeadDims.push_back(G4ThreeVector(ActiveDims2[0].getX() + 1291*um, ActiveDims2[0].getY() + 1002*um, ActiveDims2[0].getZ()));
  }

  G4Box* solidActive = new G4Box("CCDSensor", ActiveDims2[0].getX(), ActiveDims2[0].getY(), ActiveDims2[0].getZ());
  ActiveLVs.push_back(new G4LogicalVolume(solidActive, Si, "CCDSensor"));

  ActiveLVs[0]->SetRegion(actRegion);
  actRegion->AddRootLogicalVolume(ActiveLVs[0]);               

  //
  // Gettering Layer
  //
  getThick = 0.5*um;
  G4Box* solidGet = new G4Box("Gettering", DeadDims[0].getX(), DeadDims[0].getY(), getThick);
  GetteringLVs.push_back(new G4LogicalVolume(solidGet, Si, "Gettering"));

  //
  // Dead Layers
  //
  deadThick = 1*um;
  G4Box* solidDead = new G4Box("Dead", DeadDims[0].getX(), DeadDims[0].getY(), deadThick);
  DeadLayLVs.push_back(new G4LogicalVolume(solidDead, Si, "Dead"));

  G4Box* solidFullSideDead = new G4Box("FullSideDead", DeadDims[0].getX()  , DeadDims[0].getY()  , DeadDims[0].getZ()          );
  G4Box* solidActiveHole   = new G4Box("ActiveHole"  , ActiveDims2[0].getX(), ActiveDims2[0].getY(), ActiveDims2[0].getZ() + 10*um);
  G4SubtractionSolid* solidSideDead = new G4SubtractionSolid("SideDead", solidFullSideDead, solidActiveHole, 0, G4ThreeVector());
  DeadSideLVs.push_back(new G4LogicalVolume(solidSideDead, Si, "SideDead"));

  SetCCDDead(false, true);

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
  for (unsigned int i=0; i < DeadSidePVs.size(); i++) {  
    logicWorld->RemoveDaughter(DeadSidePVs[i]);
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

void ChicagoCCDDetectorConstruction::SetCCDDead(G4bool newval, G4bool isFirst) {
  if ((newval == false && newval != CCDDead) && isFirst == false) {
    for (unsigned int i=0; i < ActivePVs.size(); i++) {  
      logicWorld->RemoveDaughter(ActivePVs[i]);
      ActivePVs.erase(ActivePVs.begin() + i);
    };
    for (unsigned int i=0; i < GetteringPVs.size(); i++) {  
      logicWorld->RemoveDaughter(GetteringPVs[i]);
      GetteringPVs.erase(GetteringPVs.begin() + i);
    };
    for (unsigned int i=0; i < DeadTopPVs.size(); i++) {  
      logicWorld->RemoveDaughter(DeadTopPVs[i]);
      DeadTopPVs.erase(DeadTopPVs.begin() + i);
    };
    for (unsigned int i=0; i < DeadBottomPVs.size(); i++) {  
      logicWorld->RemoveDaughter(DeadBottomPVs[i]);
      DeadBottomPVs.erase(DeadBottomPVs.begin() + i);
    };
    for (unsigned int i=0; i < DeadSidePVs.size(); i++) {  
      logicWorld->RemoveDaughter(DeadSidePVs[i]);
      DeadSidePVs.erase(DeadSidePVs.begin() + i);
    };
  }
  else if ((newval == true && newval != CCDDead) && isFirst == false) {
    for (unsigned int i=0; i < ActivePVs.size(); i++) {  
      logicWorld->RemoveDaughter(ActivePVs[i]);
      ActivePVs.erase(ActivePVs.begin() + i);
    };
    for (unsigned int i=0; i < DeadSidePVs.size(); i++) {  
      logicWorld->RemoveDaughter(DeadSidePVs[i]);
      DeadSidePVs.erase(DeadSidePVs.begin() + i);
    };
  }

  if (newval == false && newval != CCDDead) {
    for (unsigned int i=0; i < ActiveVecs.size(); i++) {  
      ActivePVs.push_back(  new G4PVPlacement(ActiveVecs[i].second, ActiveVecs[i].first, ActiveFullLVs[0], "CCDSensor", logicWorld, false, i, checkOverlaps));
      DeadSidePVs.push_back(new G4PVPlacement(ActiveVecs[i].second, ActiveVecs[i].first, DeadShellLVs[0] , "Dead"     , logicWorld, false, i, checkOverlaps));
    }
    fScoringVolume = ActiveFullLVs[0];
    ActiveDims = ActiveDims1;
  }
  else if (newval == true && newval != CCDDead) {
    for (unsigned int i=0; i < ActiveVecs.size(); i++) {  
      ActivePVs.push_back(new G4PVPlacement(ActiveVecs[i].second    , ActiveVecs[i].first, ActiveLVs[0]   , "CCDSensor"  , logicWorld, false, i, checkOverlaps));
   
      G4double posX = ActiveVecs[i].first.getX();
      G4double posY = ActiveVecs[i].first.getY();
      G4double posZ = ActiveVecs[i].first.getZ();
      G4double actX = ActiveDims2[i].getX();
      G4double actY = ActiveDims2[i].getY();
      G4double actZ = ActiveDims2[i].getZ();

      G4ThreeVector posGet        = G4ThreeVector(posX, posY, posZ - actZ - 0.5*um);
      GetteringPVs.push_back( new G4PVPlacement(ActiveVecs[i].second, posGet             , GetteringLVs[0], "Gettering"  , logicWorld, false, i, checkOverlaps));

      G4ThreeVector posDeadTop    = G4ThreeVector(posX, posY, posZ + actZ + 1.*um );
      DeadTopPVs.push_back(   new G4PVPlacement(ActiveVecs[i].second, posDeadTop         , DeadLayLVs[0]  , "Dead_Top"   , logicWorld, false, i, checkOverlaps));

      G4ThreeVector posDeadBottom = G4ThreeVector(posX, posY, posZ - actZ - 2.*um );
      DeadBottomPVs.push_back(new G4PVPlacement(ActiveVecs[i].second, posDeadBottom      , DeadLayLVs[0]  , "Dead_Bottom", logicWorld, false, i, checkOverlaps));

      DeadSidePVs.push_back(  new G4PVPlacement(ActiveVecs[i].second, ActiveVecs[i].first, DeadSideLVs[0] , "Dead_Side"  , logicWorld, false, i, checkOverlaps));
    }
    fScoringVolume = ActiveLVs[0];
    ActiveDims = ActiveDims2;
  }
  else {return;}

  CCDDead = newval;

  if (isFirst == false) {G4RunManager::GetRunManager()->GeometryHasBeenModified();}
}
