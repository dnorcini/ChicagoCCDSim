// ChicagoCCDPrimaryGeneratorAction.cc
// implementation of the ChicagoCCDPrimaryGeneratorAction class

#include "ChicagoCCDPrimaryGeneratorAction.hh"
#include "ChicagoCCDDetectorConstruction.hh"

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

ChicagoCCDPrimaryGeneratorAction::ChicagoCCDPrimaryGeneratorAction()
: G4VUserPrimaryGeneratorAction()
{
  fParticleSource = new G4GeneralParticleSource();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ChicagoCCDPrimaryGeneratorAction::~ChicagoCCDPrimaryGeneratorAction()
{
  delete fParticleSource;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ChicagoCCDPrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
  G4SPSAngDistribution* angDist = fParticleSource->GetCurrentSource()->GetAngDist();
  
  G4double theta, phi, sintheta, sinphi, costheta, cosphi;

  phi = G4UniformRand()*2*3.1415927;
  theta = std::acos(pow(1-(G4UniformRand()),0.25));
  costheta = std::cos(theta);
  sintheta = std::sin(theta);
  cosphi = std::cos(phi);
  sinphi = std::sin(phi);

  G4ParticleMomentum p;
  p.setX(sintheta*cosphi);
  p.setY(sintheta*sinphi);
  p.setZ(-costheta);

  angDist->SetParticleMomentumDirection(p);  
	
  fParticleSource->GeneratePrimaryVertex(anEvent);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

