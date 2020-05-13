// ChicagoCCDRunAction.hh
// definition of the ChicagoCCDRunAction class

#ifndef ChicagoCCDRunAction_h
#define ChicagoCCDRunAction_h 1

#include "G4UserRunAction.hh"
//#include "G4Parameter.hh"
#include "globals.hh"
#include "G4GeneralParticleSource.hh"

#include <vector>
#include <string>

class G4Run;
class ChicagoCCDDetectorConstruction;
class ChicagoCCDPrimaryGeneratorAction;

/// Run action class
///
/// In EndOfRunAction(), it calculates the dose in the selected volume 
/// from the energy deposit accumulated via stepping and event actions.
/// The computed dose is then printed on the screen.
struct volumeVals {
  G4String concatedVolumeNames;
  std::vector<G4int> volumeNameID;
  std::vector<G4int> volumeMass;
  std::vector<G4float> volumeDensity;
  std::vector<G4int> volumeVolume;
  std::vector<G4int> volumeSurface;
};

class ChicagoCCDRunAction : public G4UserRunAction
{
  public:
    ChicagoCCDRunAction(ChicagoCCDDetectorConstruction *skipperDetector, ChicagoCCDPrimaryGeneratorAction *primGenAction);
    virtual ~ChicagoCCDRunAction();

    // virtual G4Run* GenerateRun();
    virtual void BeginOfRunAction(const G4Run*);
    virtual void   EndOfRunAction(const G4Run*);

    //RunInfo Vectors

    volumeVals runInfoVals;

    void BuildVolList (volumeVals& volVals, G4VPhysicalVolume* physVol);

    //EventOut Vectors
    std::vector<G4int> pdgPrim;
    std::vector<G4int> chargePrim;
    std::vector<G4int> volidPrim;
    std::vector<G4double> energyPrim;
    std::vector<G4double> posxPrim;
    std::vector<G4double> posyPrim;
    std::vector<G4double> poszPrim;
    std::vector<G4double> momxPrim;
    std::vector<G4double> momyPrim;
    std::vector<G4double> momzPrim;
    std::vector<G4double> triggerTime;

    //CCDOut Vectors
    std::vector<G4int> pdgCCD;
    std::vector<G4int> trackid;
    std::vector<G4int> parentid;
    std::vector<G4int> CCDid;
    std::vector<G4double> posxCCD;
    std::vector<G4double> posyCCD;
    std::vector<G4double> poszCCD;
    std::vector<G4double> gposxCCD;
    std::vector<G4double> gposyCCD;
    std::vector<G4double> gposzCCD;
    std::vector<G4double> Edep;
    std::vector<G4double> time;
  private:
    ChicagoCCDDetectorConstruction* fDetectorConstruction;
    ChicagoCCDPrimaryGeneratorAction* fPrimGen;
    G4int runinfo, eventout, ccdout, oldinfo;
};

#endif

