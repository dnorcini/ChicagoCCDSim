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
/// \file runAndEvent/ChicagoCCD/src/ChicagoCCDTrackInformation.cc
/// \brief Implementation of the ChicagoCCDTrackInformation class
//
//
// $Id: ChicagoCCDTrackInformation.cc 97671 2016-06-07 08:25:00Z gcosmo $
//

#include "ChicagoCCDTrackInformation.hh"
#include "G4ios.hh"
#include "G4SystemOfUnits.hh"    

G4ThreadLocal G4Allocator<ChicagoCCDTrackInformation> *
                                   aTrackInformationAllocator = 0;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
ChicagoCCDTrackInformation::ChicagoCCDTrackInformation()
  : G4VUserTrackInformation()
{
    fOriginalTrackID = 0;
    fParticleDefinition = 0;
    fOriginalPosition = G4ThreeVector(0.,0.,0.);
    fOriginalMomentum = G4ThreeVector(0.,0.,0.);
    fOriginalEnergy = 0.;
    fOriginalTime = 0.;
    fParentStatus = false;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
ChicagoCCDTrackInformation::ChicagoCCDTrackInformation(const G4Track* aTrack)
  : G4VUserTrackInformation()
{
    fOriginalTrackID = aTrack->GetTrackID();
    fParticleDefinition = aTrack->GetDefinition();
    fOriginalPosition = aTrack->GetPosition();
    fOriginalMomentum = aTrack->GetMomentum();
    fOriginalEnergy = aTrack->GetKineticEnergy();
    fOriginalTime = aTrack->GetGlobalTime();
    fParentStatus = false;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
ChicagoCCDTrackInformation
::ChicagoCCDTrackInformation(const ChicagoCCDTrackInformation* aTrackInfo)
  : G4VUserTrackInformation()
{
    fOriginalTrackID = aTrackInfo->fOriginalTrackID;
    fParticleDefinition = aTrackInfo->fParticleDefinition;
    fOriginalPosition = aTrackInfo->fOriginalPosition;
    fOriginalMomentum = aTrackInfo->fOriginalMomentum;
    fOriginalEnergy = aTrackInfo->fOriginalEnergy;
    fOriginalTime = aTrackInfo->fOriginalTime;
    fParentStatus = aTrackInfo->fParentStatus;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
ChicagoCCDTrackInformation::~ChicagoCCDTrackInformation()
{;}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void ChicagoCCDTrackInformation::ResetPrimary(const G4Track* aTrack)
{
    fOriginalTrackID = aTrack->GetTrackID();
    fParticleDefinition = aTrack->GetDefinition();
    fOriginalPosition = aTrack->GetPosition();
    fOriginalMomentum = aTrack->GetMomentum();
    fOriginalEnergy = aTrack->GetKineticEnergy();
    fOriginalTime = aTrack->GetGlobalTime();
    fParentStatus = false;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
ChicagoCCDTrackInformation& ChicagoCCDTrackInformation
::operator =(const ChicagoCCDTrackInformation& aTrackInfo)
{
    fOriginalTrackID = aTrackInfo.fOriginalTrackID;
    fParticleDefinition = aTrackInfo.fParticleDefinition;
    fOriginalPosition = aTrackInfo.fOriginalPosition;
    fOriginalMomentum = aTrackInfo.fOriginalMomentum;
    fOriginalEnergy = aTrackInfo.fOriginalEnergy;
    fOriginalTime = aTrackInfo.fOriginalTime;
    fParentStatus = aTrackInfo.fParentStatus;

    return *this;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void ChicagoCCDTrackInformation::Print() const
{
    G4cout
      << "Original primary track ID " << fOriginalTrackID << " (" 
      << fParticleDefinition->GetParticleName() << ","
     << fOriginalEnergy/keV << "[keV])" << G4endl;
}

