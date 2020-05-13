// ChicagoCCDPrimaryGeneratorAction.hh
// definition of the ChicagoCCDPrimaryGeneratorAction class

#ifndef ChicagoCCDPrimaryGeneratorAction_h
#define ChicagoCCDPrimaryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4GeneralParticleSource.hh"
#include "DAMICParticleSource.hh"
#include "globals.hh"
#include "ChicagoCCDPrimaryMessenger.hh"

class G4GeneralParticleSource;
class DAMICParticleSource;
class ChicagoCCDDetectorConstruction;
class G4Event;

/// The primary generator action class with particle gun.
///
/// The default kinematic is a 6 MeV gamma, randomly distribued 
/// in front of the phantom across 80% of the (X,Y) phantom size.

class ChicagoCCDPrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
  public:
    ChicagoCCDPrimaryGeneratorAction(ChicagoCCDDetectorConstruction* detectorConstruction);    
    virtual ~ChicagoCCDPrimaryGeneratorAction();

    // method from the base class
    virtual void GeneratePrimaries(G4Event*);

    void SetGammaSource(G4bool state) {gammaSource = state;};
    G4bool GetGammaSource() const {return gammaSource;};

    const DAMICParticleSource* GetParticleGun() const {return fDAMICSource;};

    G4GeneralParticleSource* fParticleSource;
  private:
    G4bool gammaSource;
    DAMICParticleSource* fDAMICSource;
    ChicagoCCDPrimaryMessenger* skipperPrimaryMessenger;
    ChicagoCCDDetectorConstruction* fDetectorConstruction;
};

#endif
