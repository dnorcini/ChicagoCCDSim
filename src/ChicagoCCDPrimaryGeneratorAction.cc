// ChicagoCCDPrimaryGeneratorAction.cc
// implementation of the ChicagoCCDPrimaryGeneratorAction class

#include "ChicagoCCDPrimaryGeneratorAction.hh"

#include "G4RunManager.hh"
#include "G4Event.hh"
#include "G4GeneralParticleSource.hh"
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
  fParticleSource->GeneratePrimaryVertex(anEvent);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

