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
ChicagoCCDTrackingAction::ChicagoCCDTrackingAction()
:G4UserTrackingAction()
{;}

void ChicagoCCDTrackingAction::PreUserTrackingAction(const G4Track* aTrack)
{;}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo...... 
void ChicagoCCDTrackingAction::PostUserTrackingAction(const G4Track* aTrack)
{
  G4TrackVector* secondaries = fpTrackingManager->GimmeSecondaries();
  size_t nSeco = secondaries->size();
  // If the particle is such that it would have information stored to pass down, then pass it down to secondaries
  ChicagoCCDTrackInformation* info = (ChicagoCCDTrackInformation*)(aTrack->GetUserInformation());
  if(!info->GetParentStatus() && nSeco>0)
  {
    for(size_t i=0;i<nSeco;i++)
    { 
      ChicagoCCDTrackInformation* infoNew = new ChicagoCCDTrackInformation(info);
      (*secondaries)[i]->SetUserInformation(infoNew);
    }
  }

  if(info->GetParentStatus() && nSeco>0)
  {
    for(size_t i=0;i<nSeco;i++) {
      ChicagoCCDTrackInformation* infoNew = new ChicagoCCDTrackInformation(info);
      (*secondaries)[i]->SetUserInformation(infoNew);
      G4int pdg = (*secondaries)[i]->GetParticleDefinition()->GetPDGEncoding();
      //if (pdg == 1000020040 || pdg == 1000932370) {
      //  (*secondaries)[i]->SetTrackStatus(fStopAndKill);
      //}
    }
  }
}

