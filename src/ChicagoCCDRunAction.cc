// ChicagoCCDRunAction.cc
// implementation of the ChicagoCCDRunAction class

#include "ChicagoCCDRunAction.hh"
#include "ChicagoCCDPrimaryGeneratorAction.hh"
#include "ChicagoCCDDetectorConstruction.hh"
#include "ChicagoCCDAnalysis.hh"
//#include "ChicagoCCDRun.hh"

#include "G4RunManager.hh"
#include "G4Run.hh"
#include "G4GeneralParticleSource.hh"
#include "G4GeometryManager.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"

#include <vector>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ChicagoCCDRunAction::ChicagoCCDRunAction(ChicagoCCDDetectorConstruction *skipperDetector, ChicagoCCDPrimaryGeneratorAction* primGenAction)
: G4UserRunAction(),
  fDetectorConstruction(skipperDetector),
  fPrimGen(primGenAction)
{
  G4RunManager::GetRunManager()->SetPrintProgress(1);
  G4AnalysisManager* analysisManager =G4AnalysisManager::Instance();
  G4cout << "Using " << analysisManager->GetType() << G4endl;
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
  analysisManager->CreateNtupleIColumn(runinfo, "volumeMass", runInfoVals.volumeMass); // mass for each volume in geom
  analysisManager->CreateNtupleFColumn(runinfo, "volumeDensity", runInfoVals.volumeDensity); // density ''
  analysisManager->CreateNtupleIColumn(runinfo, "volumeVolume", runInfoVals.volumeVolume); // volume ''
  analysisManager->CreateNtupleIColumn(runinfo, "volumeSurface", runInfoVals.volumeSurface); // surface area ''
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
  analysisManager->CreateNtupleIColumn(ccdout, "parentid", parentid); // track id of mother particle
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
/*
  // 
  // OldInfo Tree
  //

  oldinfo = analysisManager->CreateNtuple("Output","Output Info");
  analysisManager->CreateNtupleDColumn(oldinfo, "XCoord");
  analysisManager->CreateNtupleDColumn(oldinfo, "YCoord");
  analysisManager->CreateNtupleDColumn(oldinfo, "ZCoord");
  analysisManager->CreateNtupleDColumn(oldinfo, "TopBot");
  analysisManager->CreateNtupleDColumn(oldinfo, "EnergyDeposit");
  analysisManager->CreateNtupleIColumn(oldinfo, "PartIDElec");
  analysisManager->CreateNtupleIColumn(oldinfo, "IDPrim");
  analysisManager->CreateNtupleIColumn(oldinfo, "ProcessNum1");
  analysisManager->CreateNtupleIColumn(oldinfo, "ProcessNum2");
  analysisManager->CreateNtupleIColumn(oldinfo, "CCDNum");
  analysisManager->CreateNtupleIColumn(oldinfo, "ParentIDElec");
  analysisManager->CreateNtupleIColumn(oldinfo, "PrimaryName");
  analysisManager->CreateNtupleIColumn(oldinfo, "GammaSource");
  analysisManager->FinishNtuple(oldinfo);
*/
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ChicagoCCDRunAction::~ChicagoCCDRunAction()
{
  delete G4AnalysisManager::Instance();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ChicagoCCDRunAction::BeginOfRunAction(const G4Run*)
{ 
  // inform the runManager to save random number seed
  G4RunManager::GetRunManager()->SetRandomNumberStore(false);

  // reset parameters to their initial values
  G4AnalysisManager* analysisManager =G4AnalysisManager::Instance();
  analysisManager->OpenFile();

  runInfoVals.concatedVolumeNames = ";";
  BuildVolList(runInfoVals, fDetectorConstruction->GetPhysWorld()); 
  G4cout << "convatedVolumeNames = " << runInfoVals.concatedVolumeNames << G4endl;
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

//  G4cout << "concatedVolumeNames" << runInfoVals.concatedVolumeNames << G4endl;
//  G4cout << "volumeNameID" << runInfoVals.volumeNameID << G4endl;
//  G4cout << "volumeMass" << runInfoVals.volumeMass << G4endl;
//  G4cout << "volumeDensity" << runInfoVals.volumeDensity << G4endl;
//  G4cout << "volumeVolume" << runInfoVals.volumeVolume << G4endl;
//  G4cout << "volumeSurface" << runInfoVals.volumeSurface << G4endl;

  G4ParticleDefinition* partDef = fPrimGen->fParticleSource->GetCurrentSource()->GetParticleDefinition(); 
  G4String primaryParticle = partDef->GetParticleName();

  std::ostringstream ms;
  ms << partDef->GetAtomicMass();
  G4String atMass = ms.str();
  std::ostringstream ns;
  ns << partDef->GetAtomicNumber();
  G4String atNum = ns.str();

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
  G4LogicalVolume* logVol = physVol->GetLogicalVolume();
  G4VSolid* solVol = logVol->GetSolid();
  volVals.concatedVolumeNames += physVol->GetName() + ";"; 
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

