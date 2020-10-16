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
/// \file runAndEvent/ChicagoCCD/src/ChicagoCCDTrackingAction.cc
/// \brief Implementation of the ChicagoCCDTrackingAction class
//
// $Id: ChicagoCCDTrackingAction.cc 66379 2012-12-18 09:46:33Z gcosmo $
//

#include "ChicagoCCDTrackingAction.hh"
#include "ChicagoCCDTrackInformation.hh"
#include "ChicagoCCDStackingAction.hh"

#include "G4TrackingManager.hh"
#include "G4Track.hh"

#include <vector>
#include <algorithm>
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo...... 
ChicagoCCDTrackingAction::ChicagoCCDTrackingAction(ChicagoCCDStackingAction* stackingAction)
:G4UserTrackingAction(),
  fStackingAction(stackingAction)
{;}

void ChicagoCCDTrackingAction::PreUserTrackingAction(const G4Track* aTrack)
{;}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo...... 
void ChicagoCCDTrackingAction::PostUserTrackingAction(const G4Track* aTrack)
{
  G4int parentid = aTrack->GetParentID();
  G4TrackVector* secondaries = fpTrackingManager->GimmeSecondaries();
  std::vector< G4int > trackList = fStackingAction->GetTrackList();
  // If the particle is such that it would have information stored to pass down, then pass it down to secondaries
  if ((aTrack->GetParticleDefinition()->GetParticleType() != "nucleus" && aTrack->GetParticleDefinition()->GetParticleType() != "GenericIon" && std::find(trackList.begin(), trackList.end(), aTrack->GetParentID()) == trackList.end()) || std::find(trackList.begin(), trackList.end(), aTrack->GetParentID()) != trackList.end())
  {
    ChicagoCCDTrackInformation* info = (ChicagoCCDTrackInformation*)(aTrack->GetUserInformation());
    size_t nSeco = secondaries->size();
    if(nSeco>0)
    {
      for(size_t i=0;i<nSeco;i++)
      { 
        ChicagoCCDTrackInformation* infoNew = new ChicagoCCDTrackInformation(info);
        (*secondaries)[i]->SetUserInformation(infoNew);
      }
    }
  }
  else {
    ChicagoCCDTrackInformation* info = (ChicagoCCDTrackInformation*)(aTrack->GetUserInformation());
    size_t nSeco = secondaries->size();
    if(nSeco>0)
    {
      for(size_t i=0;i<nSeco;i++)
      {
        if ((*secondaries)[i]->GetParticleDefinition()->GetParticleType() == "nucleus" || (*secondaries)[i]->GetParticleDefinition()->GetParticleType() == "GenericIon") {
          ChicagoCCDTrackInformation* infoNew = new ChicagoCCDTrackInformation(info);
          (*secondaries)[i]->SetUserInformation(infoNew);
        }
      }
    }
  }
}

