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
/// \file runAndEvent/ChicagoCCD/src/ChicagoCCDStackingAction.cc
/// \brief Implementation of the ChicagoCCDStackingAction class
//
// $Id: ChicagoCCDStackingAction.cc 90679 2015-06-08 07:58:19Z gcosmo $
//

#include "ChicagoCCDStackingAction.hh"
#include "ChicagoCCDTrackInformation.hh"

#include "G4SDManager.hh"
#include "G4RunManager.hh"
#include "G4Event.hh"
#include "G4HCofThisEvent.hh"
#include "G4Track.hh"
#include "G4TrackStatus.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleTypes.hh"
#include "G4SystemOfUnits.hh"    
#include "G4ios.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo...... 
ChicagoCCDStackingAction::ChicagoCCDStackingAction()
  :G4UserStackingAction(),
   fStage(0)
{;}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo...... 
ChicagoCCDStackingAction::~ChicagoCCDStackingAction()
{;}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo...... 
G4ClassificationOfNewTrack ChicagoCCDStackingAction
::ClassifyNewTrack(const G4Track * aTrack)
{
  G4ClassificationOfNewTrack classification = fUrgent;
  if(fStage==0)
  {
    ChicagoCCDTrackInformation* trackInfo;
    // Check if there is a particle we want to deem the "primary" for its daughters
    if(std::find(trackList.begin(), trackList.end(), aTrack->GetParentID()) == trackList.end() && aTrack->GetParticleDefinition()->GetParticleType() != "nucleus" && aTrack->GetParticleDefinition()->GetParticleType() != "GenericIon") {
      trackList.push_back(aTrack->GetTrackID());
      parentList.push_back(aTrack->GetTrackID());
      // If the particle is a primary, collect its information and store it in the track
      trackInfo = new ChicagoCCDTrackInformation(aTrack);
      G4Track* theTrack = (G4Track*)aTrack;
      theTrack->SetUserInformation(trackInfo);
    }
    else if ((aTrack->GetParticleDefinition()->GetParticleType() == "GenericIon" || aTrack->GetParticleDefinition()->GetParticleType() == "nucleus") && std::find(parentList.begin(), parentList.end(), aTrack->GetParentID()) == parentList.end()) {
      // Consider it a non-tracked parent of the primary
      parentList.push_back(aTrack->GetTrackID());
      // Give it track information so that it
      trackInfo = new ChicagoCCDTrackInformation(aTrack);
      G4Track* theTrack = (G4Track*)aTrack;
      theTrack->SetUserInformation(trackInfo);
    }
    else if ((aTrack->GetParticleDefinition()->GetParticleType() == "GenericIon" || aTrack->GetParticleDefinition()->GetParticleType() == "nucleus") && std::find(parentList.begin(), parentList.end(), aTrack->GetParentID()) != parentList.end()) {
      // Consider it a non-tracked parent of the primary
      parentList.push_back(aTrack->GetTrackID());
    }
    else if (std::find(trackList.begin(), trackList.end(), aTrack->GetParentID()) != trackList.end()) {
      // Record that this particle has been tracked and is a descendent of a primary
      trackList.push_back(aTrack->GetTrackID());
    }
  }
  return classification;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo...... 
void ChicagoCCDStackingAction::NewStage()
{
  if(stackManager->GetNUrgentTrack())
  {
    // Transfer all tracks in Urgent stack to Waiting stack, since all tracks
    // in Waiting stack have already been transfered to Urgent stack before
    // invokation of this method.
    stackManager->TransferStackedTracks(fUrgent,fWaiting);

    // Then, transfer only one track to Urgent stack.
    stackManager->TransferOneStackedTrack(fWaiting,fUrgent);

    fStage++;
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......     
void ChicagoCCDStackingAction::PrepareNewEvent()
{ 
  fStage = 0;
  trackList.clear();
  parentList.clear();
}

