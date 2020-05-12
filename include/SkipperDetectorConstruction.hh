//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
// $Id: SkipperDetectorConstruction.hh 69565 2013-05-08 12:35:31Z gcosmo $
//
/// \file SkipperDetectorConstruction.hh
/// \brief Definition of the SkipperDetectorConstruction class

#ifndef SkipperDetectorConstruction_h
#define SkipperDetectorConstruction_h 1

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

class SkipperDetectorConstruction : public G4VUserDetectorConstruction
{
  public:
    SkipperDetectorConstruction();
    virtual ~SkipperDetectorConstruction();

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

