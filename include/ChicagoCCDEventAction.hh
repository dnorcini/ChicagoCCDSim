// ChicagoCCDEventAction.hh
// definition of the ChicagoCCDEventAction class

#ifndef ChicagoCCDEventAction_h
#define ChicagoCCDEventAction_h 1

#include "G4UserEventAction.hh"
#include "G4SystemOfUnits.hh"
#include "globals.hh"

class ChicagoCCDRunAction;

/// Event action class
///

class ChicagoCCDEventAction : public G4UserEventAction
{
  public:
    ChicagoCCDEventAction(ChicagoCCDRunAction* runAction);
    virtual ~ChicagoCCDEventAction();

    virtual void BeginOfEventAction(const G4Event* event);
    virtual void EndOfEventAction(const G4Event* event);

    void SetGammaSource(G4bool source) {gammaSource = source;}
    G4bool GetGammaSource() {return gammaSource;}

    G4EventManager* evManager;

  private:
    ChicagoCCDRunAction* fRunAction;
    G4bool gammaSource;
};

#endif

    