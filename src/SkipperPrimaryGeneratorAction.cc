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
// $Id: SkipperPrimaryGeneratorAction.cc 94307 2015-11-11 13:42:46Z gcosmo $
//
/// \file SkipperPrimaryGeneratorAction.cc
/// \brief Implementation of the SkipperPrimaryGeneratorAction class

#include "SkipperPrimaryGeneratorAction.hh"
#include "DAMICParticleSource.hh"
#include "SkipperDetectorConstruction.hh"

#include "G4RunManager.hh"
#include "G4Event.hh"
#include "G4GeneralParticleSource.hh"
#include "G4GeneralParticleSourceMessenger.hh"
#include "G4SPSEneDistribution.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"
#include "G4String.hh"
#include "Randomize.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SkipperPrimaryGeneratorAction::SkipperPrimaryGeneratorAction(SkipperDetectorConstruction* detectorConstruction)
: G4VUserPrimaryGeneratorAction(),
  fDetectorConstruction(detectorConstruction)
{
  fParticleSource = new G4GeneralParticleSource();
  skipperPrimaryMessenger = new SkipperPrimaryMessenger(this, fDetectorConstruction);
  SetGammaSource(false);
//  fDAMICSource = new DAMICParticleSource();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SkipperPrimaryGeneratorAction::~SkipperPrimaryGeneratorAction()
{
  delete fParticleSource;
  delete skipperPrimaryMessenger;
//  delete fDAMICSource;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SkipperPrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
  if (GetGammaSource() == true) {
    G4double gammaEn;
    G4double randGen = G4UniformRand() * 1.0538;
    if (randGen <= 0.8551) {gammaEn = 122.06065*keV;}
    else if (randGen > 0.8551 && randGen <= 0.9622) {gammaEn = 136.47356*keV;}
    else if (randGen > 0.9622) {gammaEn = 14.41295*keV;};
    G4SPSEneDistribution* eneDist = fParticleSource->GetCurrentSource()->GetEneDist();
    eneDist->SetEnergyDisType("Mono");
    eneDist->SetMonoEnergy(gammaEn);
  }
  fParticleSource->GeneratePrimaryVertex(anEvent);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

