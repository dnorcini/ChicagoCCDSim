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
// $Id: SkipperRunAction.hh 93886 2015-11-03 08:28:26Z gcosmo $
//
/// \file SkipperRunAction.hh
/// \brief Definition of the SkipperRunAction class

#ifndef SkipperRunAction_h
#define SkipperRunAction_h 1

#include "G4UserRunAction.hh"
//#include "G4Parameter.hh"
#include "globals.hh"
#include "G4GeneralParticleSource.hh"

#include <vector>
#include <string>

class G4Run;
class SkipperDetectorConstruction;
class SkipperPrimaryGeneratorAction;

/// Run action class
///
/// In EndOfRunAction(), it calculates the dose in the selected volume 
/// from the energy deposit accumulated via stepping and event actions.
/// The computed dose is then printed on the screen.
struct volumeVals {
  G4String concatedVolumeNames;
  std::vector<G4int> volumeNameID;
  std::vector<G4int> volumeMass;
  std::vector<G4float> volumeDensity;
  std::vector<G4int> volumeVolume;
  std::vector<G4int> volumeSurface;
};

class SkipperRunAction : public G4UserRunAction
{
  public:
    SkipperRunAction(SkipperDetectorConstruction *skipperDetector, SkipperPrimaryGeneratorAction *primGenAction);
    virtual ~SkipperRunAction();

    // virtual G4Run* GenerateRun();
    virtual void BeginOfRunAction(const G4Run*);
    virtual void   EndOfRunAction(const G4Run*);

    //RunInfo Vectors

    volumeVals runInfoVals;

    void BuildVolList (volumeVals& volVals, G4VPhysicalVolume* physVol);

    //EventOut Vectors
    std::vector<G4int> pdgPrim;
    std::vector<G4int> chargePrim;
    std::vector<G4int> volidPrim;
    std::vector<G4double> energyPrim;
    std::vector<G4double> posxPrim;
    std::vector<G4double> posyPrim;
    std::vector<G4double> poszPrim;
    std::vector<G4double> momxPrim;
    std::vector<G4double> momyPrim;
    std::vector<G4double> momzPrim;
    std::vector<G4double> triggerTime;

    //CCDOut Vectors
    std::vector<G4int> pdgCCD;
    std::vector<G4int> trackid;
    std::vector<G4int> parentid;
    std::vector<G4int> CCDid;
    std::vector<G4double> posxCCD;
    std::vector<G4double> posyCCD;
    std::vector<G4double> poszCCD;
    std::vector<G4double> gposxCCD;
    std::vector<G4double> gposyCCD;
    std::vector<G4double> gposzCCD;
    std::vector<G4double> Edep;
    std::vector<G4double> time;
  private:
    SkipperDetectorConstruction* fDetectorConstruction;
    SkipperPrimaryGeneratorAction* fPrimGen;
    G4int runinfo, eventout, ccdout, oldinfo;
};

#endif

