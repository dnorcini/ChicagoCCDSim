// ChicagoCCDEventAction.cc
// implementation of the ChicagoCCDEventAction class

#include "ChicagoCCDEventAction.hh"
#include "ChicagoCCDRunAction.hh"
#include "ChicagoCCDAnalysis.hh"

#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4EventManager.hh"
#include "G4UnitsTable.hh"
#include "G4TransportationManager.hh"

#include <string>
#include <cstring>
#include <vector>
#include <algorithm>
#include <stdio.h>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ChicagoCCDEventAction::ChicagoCCDEventAction(ChicagoCCDRunAction* runAction)
: G4UserEventAction(),
  fRunAction(runAction)
{} 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ChicagoCCDEventAction::~ChicagoCCDEventAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ChicagoCCDEventAction::BeginOfEventAction(const G4Event* event)
{ 
  SetGammaSource(false);

  G4int EventID = event->GetEventID();
  G4PrimaryVertex* primVert = event->GetPrimaryVertex();
  G4PrimaryParticle* primPart = primVert->GetPrimary();

  G4Navigator* navigator = G4TransportationManager::GetTransportationManager()->GetNavigatorForTracking();
  G4VPhysicalVolume* container = navigator->LocateGlobalPointAndSetup(primVert->GetPosition());
  G4String containerName = ";" + container->GetName() + ";";
  G4String nameList = fRunAction->runInfoVals.concatedVolumeNames;
  G4int namePos = nameList.find(containerName);
  G4int volid = std::count(nameList.begin(), nameList.begin() + namePos, ';');

  fRunAction->pdgPrim.clear();
  fRunAction->chargePrim.clear();
  fRunAction->volidPrim.clear();
  fRunAction->energyPrim.clear();
  fRunAction->posxPrim.clear();
  fRunAction->posyPrim.clear();
  fRunAction->poszPrim.clear();
  fRunAction->momxPrim.clear();
  fRunAction->momyPrim.clear();
  fRunAction->momzPrim.clear();
  fRunAction->triggerTime.clear();

  fRunAction->pdgCCD.clear();
  fRunAction->trackid.clear();
  fRunAction->parentid.clear();
  fRunAction->CCDid.clear();
  fRunAction->posxCCD.clear();
  fRunAction->posyCCD.clear();
  fRunAction->poszCCD.clear();
  fRunAction->gposxCCD.clear();
  fRunAction->gposyCCD.clear();
  fRunAction->gposzCCD.clear();
  fRunAction->Edep.clear();
  fRunAction->time.clear();

  fRunAction->pdgPrim.push_back(primPart->GetPDGcode());
  fRunAction->chargePrim.push_back(G4int(primPart->GetCharge()));
  fRunAction->volidPrim.push_back(volid);
  fRunAction->energyPrim.push_back(primPart->GetKineticEnergy() / eV);
  fRunAction->posxPrim.push_back(primVert->GetX0());
  fRunAction->posyPrim.push_back(primVert->GetY0());
  fRunAction->poszPrim.push_back(primVert->GetZ0());
  fRunAction->momxPrim.push_back(primPart->GetPx() / eV);
  fRunAction->momyPrim.push_back(primPart->GetPy() / eV);
  fRunAction->momzPrim.push_back(primPart->GetPz() / eV); 
  fRunAction->triggerTime.push_back(primVert->GetT0());

  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  analysisManager->FillNtupleIColumn(1, 0, EventID);
  analysisManager->AddNtupleRow(1);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ChicagoCCDEventAction::EndOfEventAction(const G4Event* event)
{ 
  if (!fRunAction->Edep.empty()) {
    G4int EventID = event->GetEventID(); 
    G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
    analysisManager->FillNtupleIColumn(2, 0, EventID);
    analysisManager->AddNtupleRow(2);
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
