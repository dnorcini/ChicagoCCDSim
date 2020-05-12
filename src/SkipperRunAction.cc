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
// $Id: SkipperRunAction.cc 93886 2015-11-03 08:28:26Z gcosmo $
//
/// \file SkipperRunAction.cc
/// \brief Implementation of the SkipperRunAction class

#include "SkipperRunAction.hh"
#include "SkipperPrimaryGeneratorAction.hh"
#include "SkipperDetectorConstruction.hh"
#include "SkipperAnalysis.hh"
//#include "SkipperRun.hh"

#include "G4RunManager.hh"
#include "G4Run.hh"
#include "G4GeneralParticleSource.hh"
#include "G4ParameterManager.hh"
#include "G4GeometryManager.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"

#include <vector>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SkipperRunAction::SkipperRunAction(SkipperDetectorConstruction *skipperDetector, SkipperPrimaryGeneratorAction* primGenAction)
: G4UserRunAction(),
  fEdep("Edep",0.),
  fEdep2("Edep2",0.),
  fIntHit("IntHit",0.),
  fEvHit("EvHit",0.),
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

  G4ParameterManager* parameterManager = G4ParameterManager::Instance();
  parameterManager->RegisterParameter(fEdep);
  parameterManager->RegisterParameter(fEdep2);
  parameterManager->RegisterParameter(fIntHit);
  parameterManager->RegisterParameter(fEvHit);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SkipperRunAction::~SkipperRunAction()
{
  delete G4AnalysisManager::Instance();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SkipperRunAction::BeginOfRunAction(const G4Run*)
{ 
  // inform the runManager to save random number seed
  G4RunManager::GetRunManager()->SetRandomNumberStore(false);

  // reset parameters to their initial values
  G4ParameterManager* parameterManager = G4ParameterManager::Instance();
  parameterManager->Reset();

  G4AnalysisManager* analysisManager =G4AnalysisManager::Instance();
  analysisManager->OpenFile();

  runInfoVals.concatedVolumeNames = ";";
  BuildVolList(runInfoVals, fDetectorConstruction->GetPhysWorld()); 
  G4cout << "convatedVolumeNames = " << runInfoVals.concatedVolumeNames << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SkipperRunAction::EndOfRunAction(const G4Run* run)
{
  G4int nEvts = run->GetNumberOfEvent();
  if (nEvts == 0) return;

  // Merge parameters 
  G4ParameterManager* parameterManager = G4ParameterManager::Instance();
  parameterManager->Merge();
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
  G4String simulatedVolume = "Basement_Chamber";

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

void SkipperRunAction::BuildVolList (volumeVals& volVals, G4VPhysicalVolume* physVol) {
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

void SkipperRunAction::AddEdep(G4double edep)
{
  fEdep  += edep;
  fEdep2 += edep*edep;
}

void SkipperRunAction::AddIntHit(G4int hit)
{
  fIntHit += hit;
}

void SkipperRunAction::AddEvHit(G4int hit)
{
  fEvHit += hit;
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

