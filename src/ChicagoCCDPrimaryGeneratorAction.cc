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
  skipperPrimaryMessenger = new ChicagoCCDPrimaryMessenger(this);
  SetGammaSource(false);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ChicagoCCDPrimaryGeneratorAction::~ChicagoCCDPrimaryGeneratorAction()
{
  delete fParticleSource;
  delete skipperPrimaryMessenger;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ChicagoCCDPrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
  // GammaSource defines whether gammas of specific energies are used as the source rather than an ion
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

