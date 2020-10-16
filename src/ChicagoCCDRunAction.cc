// ChicagoCCDRunAction.cc
// implementation of the ChicagoCCDRunAction class

#include "ChicagoCCDRunAction.hh"
#include "ChicagoCCDPrimaryGeneratorAction.hh"
#include "ChicagoCCDDetectorConstruction.hh"
#include "ChicagoCCDAnalysis.hh"

#include "G4RunManager.hh"
#include "G4Run.hh"
#include "G4GeneralParticleSource.hh"
#include "G4GeometryManager.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"

#include "Randomize.hh"
#include "G4MTHepRandom.hh"

#include <ctime>
#include <vector>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ChicagoCCDRunAction::ChicagoCCDRunAction(ChicagoCCDDetectorConstruction *skipperDetector, ChicagoCCDPrimaryGeneratorAction* primGenAction)
: G4UserRunAction(),
  fDetectorConstruction(skipperDetector),
  fPrimGen(primGenAction)
{
  // Create output trees
  // N.B. many columns are vectors. These must be passed in by reference from the start. So when we update the rows, we fill the vector values and the row update will automatically put in the values
  G4RunManager::GetRunManager()->SetPrintProgress(1);
  G4AnalysisManager* analysisManager =G4AnalysisManager::Instance();
  analysisManager->SetVerboseLevel(3);

  // 
  // RunInfo Tree
  //

  runinfo = analysisManager->CreateNtuple("RunInfo","RunInfo");
  analysisManager->CreateNtupleIColumn(runinfo, "NEvts"); // # of Simulated events
  analysisManager->CreateNtupleIColumn(runinfo, "NCCD"); // # of CCDs
  analysisManager->CreateNtupleIColumn(runinfo, "Seed"); // Initial Seed (-1 if from process time)
  analysisManager->CreateNtupleSColumn(runinfo, "concatedVolumeNames"); // Semicolon-separated volume name list
  analysisManager->CreateNtupleIColumn(runinfo, "volumeNameID", runInfoVals.volumeNameID); // ID for each volume in geom
  analysisManager->CreateNtupleDColumn(runinfo, "volumeMass", runInfoVals.volumeMass); // mass for each volume in geom
  analysisManager->CreateNtupleDColumn(runinfo, "volumeDensity", runInfoVals.volumeDensity); // density ''
  analysisManager->CreateNtupleDColumn(runinfo, "volumeVolume", runInfoVals.volumeVolume); // volume ''
  analysisManager->CreateNtupleDColumn(runinfo, "volumeSurface", runInfoVals.volumeSurface); // surface area ''
  analysisManager->CreateNtupleSColumn(runinfo, "primaryParticle"); // primary simulated particle type
  analysisManager->CreateNtupleSColumn(runinfo, "primaryIon"); // simulated ion
  analysisManager->CreateNtupleSColumn(runinfo, "simulatedVolume"); // name of simulated volume
  analysisManager->FinishNtuple(runinfo); 

  //
  // EventOut Tree
  //

  eventout = analysisManager->CreateNtuple("EventOut","EventOut");
  analysisManager->CreateNtupleIColumn(eventout, "EventID"); // event ID #
  analysisManager->CreateNtupleIColumn(eventout, "pdg", pdgPrim); // pdg code of primary particle
  analysisManager->CreateNtupleIColumn(eventout, "primaryid", primaryid); // track ID of primary
  analysisManager->CreateNtupleIColumn(eventout, "charge", chargePrim); // charge of ''
  analysisManager->CreateNtupleIColumn(eventout, "volid", volidPrim); // volume where particle is generated
  analysisManager->CreateNtupleDColumn(eventout, "energy", energyPrim); // kinetic energy of primary particle
  analysisManager->CreateNtupleDColumn(eventout, "posx", posxPrim); //
  analysisManager->CreateNtupleDColumn(eventout, "posy", posyPrim); // absolute position of ''
  analysisManager->CreateNtupleDColumn(eventout, "posz", poszPrim); //
  analysisManager->CreateNtupleDColumn(eventout, "momx", momxPrim); //
  analysisManager->CreateNtupleDColumn(eventout, "momy", momyPrim); // momentum of ''
  analysisManager->CreateNtupleDColumn(eventout, "momz", momzPrim); //
  analysisManager->CreateNtupleDColumn(eventout, "triggerTime", triggerTime); // trigger time
  analysisManager->FinishNtuple(eventout); 

  // 
  // CCDOut Tree
  //

  ccdout = analysisManager->CreateNtuple("CCDOut","CCDOut");
  analysisManager->CreateNtupleIColumn(ccdout, "EventID"); // event ID #
  analysisManager->CreateNtupleIColumn(ccdout, "pdg", pdgCCD); // pdg code for particle hitting CCD
  analysisManager->CreateNtupleIColumn(ccdout, "trackid", trackid); // track ID number
  analysisManager->CreateNtupleIColumn(ccdout, "parentid", parentid); // track ID of mother particle
  analysisManager->CreateNtupleIColumn(ccdout, "primaryid", primaryidCCD); // track ID of primary
  analysisManager->CreateNtupleIColumn(ccdout, "CCDid", CCDid); // ID # of CCD
  analysisManager->CreateNtupleDColumn(ccdout, "posx", posxCCD); //
  analysisManager->CreateNtupleDColumn(ccdout, "posy", posyCCD); // Edep-weighted avg position per pixel
  analysisManager->CreateNtupleDColumn(ccdout, "posz", poszCCD); //
  analysisManager->CreateNtupleDColumn(ccdout, "gposx", gposxCCD); //
  analysisManager->CreateNtupleDColumn(ccdout, "gposy", gposyCCD); // abs pos of generated primary particle
  analysisManager->CreateNtupleDColumn(ccdout, "gposz", gposzCCD); //
  analysisManager->CreateNtupleDColumn(ccdout, "Edep", Edep); // total energy deposited
  analysisManager->CreateNtupleDColumn(ccdout, "time", time); // time wrt to event trigger time
  analysisManager->FinishNtuple(ccdout); 
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ChicagoCCDRunAction::~ChicagoCCDRunAction()
{
  delete G4AnalysisManager::Instance();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ChicagoCCDRunAction::BeginOfRunAction(const G4Run*)
{ 
  runInfoVals.volumeNameID.clear();
  runInfoVals.volumeMass.clear(); 
  runInfoVals.volumeDensity.clear();
  runInfoVals.volumeVolume.clear();
  runInfoVals.volumeSurface.clear();
  // inform the runManager to save random number seed
  G4RunManager::GetRunManager()->SetRandomNumberStore(false);

  // reset parameters to their initial values
  G4AnalysisManager* analysisManager =G4AnalysisManager::Instance();
  analysisManager->OpenFile();

  runInfoVals.concatedVolumeNames = ";";
  BuildVolList(runInfoVals, fDetectorConstruction->GetPhysWorld()); 
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ChicagoCCDRunAction::EndOfRunAction(const G4Run* run)
{
  G4int nEvts = run->GetNumberOfEvent();
  if (nEvts == 0) return;

  // Merge parameters 
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();

  //
  // Collect RunInfo
  //

  G4int NCCD = fDetectorConstruction->GetTotCCDs();

  G4int Seed = G4RunManager::GetRunManager()->GetRandomNumberStore();

  G4ParticleDefinition* partDef = fPrimGen->fParticleSource->GetCurrentSource()->GetParticleDefinition(); 
  G4String primaryParticle = partDef->GetParticleName();

  std::ostringstream massstr;
  massstr << partDef->GetAtomicMass();
  G4String atMass = massstr.str();
  std::ostringstream numstr;
  numstr << partDef->GetAtomicNumber();
  G4String atNum = numstr.str();

  G4String primaryIon = atMass + "a" + atNum + "z";
  G4String simulatedVolume = "World";

  //
  // Fill RunInfo
  //

  analysisManager->FillNtupleIColumn(0, 0,  nEvts); // NEvts
  analysisManager->FillNtupleIColumn(0, 1,  NCCD);  // NCCD
  analysisManager->FillNtupleIColumn(0, 2,  Seed); // Seed
  analysisManager->FillNtupleSColumn(0, 3,  runInfoVals.concatedVolumeNames); // concatedVolumeNames
  analysisManager->FillNtupleSColumn(0, 9,  primaryParticle); // primaryParticle
  analysisManager->FillNtupleSColumn(0, 10, primaryIon); // primaryIon
  analysisManager->FillNtupleSColumn(0, 11, simulatedVolume); // simulatedVolume

  analysisManager->AddNtupleRow(0);
  analysisManager->Write();
  analysisManager->CloseFile();
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ChicagoCCDRunAction::BuildVolList (volumeVals& volVals, G4VPhysicalVolume* physVol) {
  // The RunInfo output requires a bunch of information about the geometry of the simulation. This function starts with one physical volume and iterates through it and its descendents to construct a list of their names and corresponding vectors of their properties. Input the world volume to get every piece of geometry
  G4LogicalVolume* logVol = physVol->GetLogicalVolume();
  G4VSolid* solVol = logVol->GetSolid();
  volVals.concatedVolumeNames += physVol->GetName() + ";";
  G4cout << physVol->GetName() << G4endl;
  if (volVals.volumeNameID.size() == 0) {
    volVals.volumeNameID.push_back(0);
  }
  else {
    volVals.volumeNameID.push_back(volVals.volumeNameID.back() + 1);
  };
  volVals.volumeMass.push_back(logVol->GetMass() / g);
  volVals.volumeDensity.push_back(logVol->GetMaterial()->GetDensity() / (g/cm3));
  volVals.volumeVolume.push_back(solVol->GetCubicVolume() / cm3);
  volVals.volumeSurface.push_back(solVol->GetSurfaceArea() / cm2);

  G4int nDaughters = logVol->GetNoDaughters();
  if (nDaughters > 0) {
    for (int i=0; i<nDaughters; i++) {
      BuildVolList(volVals, logVol->GetDaughter(i));
    }; 
  };
}



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

