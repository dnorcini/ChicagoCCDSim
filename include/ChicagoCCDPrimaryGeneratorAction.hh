// ChicagoCCDPrimaryGeneratorAction.hh
// definition of the ChicagoCCDPrimaryGeneratorAction class

#ifndef ChicagoCCDPrimaryGeneratorAction_h
#define ChicagoCCDPrimaryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4GeneralParticleSource.hh"
#include "globals.hh"

class G4GeneralParticleSource;
class ChicagoCCDDetectorConstruction;
class G4Event;

/// The primary generator action class with particle gun.
///
/// The default kinematic is a 6 MeV gamma, randomly distribued 
/// in front of the phantom across 80% of the (X,Y) phantom size.

class ChicagoCCDPrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
  public:
    ChicagoCCDPrimaryGeneratorAction();    
    virtual ~ChicagoCCDPrimaryGeneratorAction();

    // method from the base class
    virtual void GeneratePrimaries(G4Event*);

    G4GeneralParticleSource* fParticleSource;
};

#endif
