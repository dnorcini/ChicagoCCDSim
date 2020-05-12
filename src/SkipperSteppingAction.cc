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
// $Id: SkipperSteppingAction.cc 74483 2013-10-09 13:37:06Z gcosmo $
//
/// \file SkipperSteppingAction.cc
/// \brief Implementation of the SkipperSteppingAction class

#include "SkipperSteppingAction.hh"
#include "SkipperEventAction.hh"
#include "SkipperRunAction.hh"
#include "SkipperDetectorConstruction.hh"
#include "SkipperAnalysis.hh"

#include "G4Step.hh"
#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4LogicalVolume.hh"
#include "G4SteppingManager.hh"
#include "G4TouchableHistory.hh"
#include "G4Gamma.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SkipperSteppingAction::SkipperSteppingAction(SkipperEventAction* eventAction, SkipperRunAction* runAction, SkipperDetectorConstruction* detectorConstruction)
: G4UserSteppingAction(),
  fEventAction(eventAction),
  fScoringVolume(0),
  fRunAction(runAction),
  fDetectorConstruction(detectorConstruction)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SkipperSteppingAction::~SkipperSteppingAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SkipperSteppingAction::UserSteppingAction(const G4Step* step)
{
  const SkipperDetectorConstruction* ldetectorConstruction
      = static_cast<const SkipperDetectorConstruction*>
        (G4RunManager::GetRunManager()->GetUserDetectorConstruction());
  if (!fScoringVolume) { 
    fScoringVolume = ldetectorConstruction->GetScoringVolume();   
  }

  // get volume of the current step
  G4LogicalVolume* logVolume 
    = step->GetPreStepPoint()->GetTouchableHandle()
      ->GetVolume()->GetLogicalVolume();
  // check if we are in scoring volume
  if (logVolume != fScoringVolume) return;

  G4double edepStep = step->GetTotalEnergyDeposit();  
  if (edepStep > 0.) {
    // check if we are looking at the decay particle
    G4Track *track=step->GetTrack();

    G4VPhysicalVolume* physVolume = step->GetPreStepPoint()->GetTouchableHandle()->GetVolume();
    G4int CCDNum = fDetectorConstruction->GetCCDNum(physVolume);

    G4TouchableHandle theTouchable = step->GetPreStepPoint()->GetTouchableHandle();
    G4ThreeVector worldPosition = step->GetPreStepPoint()->GetPosition();
    G4ThreeVector localPosition = theTouchable->GetHistory()->GetTopTransform().TransformPoint(worldPosition);

/*
    G4int trackid = track->GetTrackID();
    G4int parentid = track->GetParentID();
    G4int ProcessCreatorType = 0;
    G4int ProcessCreatorSub = 0;
    if (trackid != 1) {
      ProcessCreatorType = track->GetCreatorProcess()->GetProcessType();
      ProcessCreatorSub = track->GetCreatorProcess()->GetProcessSubType();
    }
    G4int EventID = G4EventManager::GetEventManager()->GetConstCurrentEvent()->GetEventID();
    G4ParticleDefinition* ionPart = G4EventManager::GetEventManager()->GetConstCurrentEvent()->GetPrimaryVertex()->GetPrimary()->GetG4code();
    G4int primName = ionPart->GetAtomicNumber()*1000 + ionPart->GetAtomicMass();
    G4int gamma = int(fEventAction->GetGammaSource());
*/

    G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
/*
    analysisManager->FillNtupleDColumn(3, 0, (localPosition.getX()/um+45000)); //XCoord
    analysisManager->FillNtupleDColumn(3, 1, (localPosition.getY()/um+7500)); //YCoord
    analysisManager->FillNtupleDColumn(3, 2, (localPosition.getZ()/um+337.5)); //ZCoord
    analysisManager->FillNtupleDColumn(3, 3, 0); //TopBot
    analysisManager->FillNtupleDColumn(3, 4, edepStep/keV); //EnergyDeposit
    analysisManager->FillNtupleIColumn(3, 5, trackid); //PartIDElec
    analysisManager->FillNtupleIColumn(3, 6, EventID); //IDPrim
    analysisManager->FillNtupleIColumn(3, 7, ProcessCreatorType); //ProcessNum1
    analysisManager->FillNtupleIColumn(3, 8, ProcessCreatorSub); //ProcessNum2
    analysisManager->FillNtupleIColumn(3, 9, CCDNum); //CCDNum
    analysisManager->FillNtupleIColumn(3, 10, parentid); //ParentIDElec
    analysisManager->FillNtupleIColumn(3, 11, primName); //PrimaryName
    analysisManager->FillNtupleIColumn(3, 12, gamma); //GammaSource
    analysisManager->AddNtupleRow(3);
*/
    fRunAction->pdgCCD.push_back(track->GetParticleDefinition()->GetPDGEncoding());
    fRunAction->trackid.push_back(track->GetTrackID());
    fRunAction->parentid.push_back(track->GetParentID());
    fRunAction->CCDid.push_back(CCDNum);
    fRunAction->posxCCD.push_back(localPosition.getX() + 45);
    fRunAction->posyCCD.push_back(localPosition.getY() + 7.5); //not hard coded
    fRunAction->poszCCD.push_back(localPosition.getZ() + 0.3375);
    fRunAction->gposxCCD.push_back(worldPosition.getX());
    fRunAction->gposyCCD.push_back(worldPosition.getY());
    fRunAction->gposzCCD.push_back(worldPosition.getZ());
    fRunAction->Edep.push_back(edepStep / eV);
    fRunAction->time.push_back(track->GetGlobalTime() / s);
  }  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

