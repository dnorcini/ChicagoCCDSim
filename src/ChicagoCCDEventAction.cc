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
#include <functional>
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
  usedPix.clear();
  // Since the vectors used for the outputs were created by RunAction, we can't make them anew but we'll clear them at the beginning of each event
  fRunAction->pdgPrim.clear();
  fRunAction->primaryid.clear();
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
  fRunAction->primaryidCCD.clear();
  fRunAction->CCDid.clear();
  fRunAction->posxCCD.clear();
  fRunAction->posyCCD.clear();
  fRunAction->poszCCD.clear();
  fRunAction->gposxCCD.clear();
  fRunAction->gposyCCD.clear();
  fRunAction->gposzCCD.clear();
  fRunAction->Edep.clear();
  fRunAction->time.clear();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ChicagoCCDEventAction::EndOfEventAction(const G4Event* event)
{ 
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  G4int EventID = event->GetEventID();
  // If there was energy deposition, seal the deal and record it
  if (!fRunAction->Edep.empty()) {
    std::transform(fRunAction->posxCCD.begin(), fRunAction->posxCCD.end(), fRunAction->Edep.begin(), fRunAction->posxCCD.begin(), std::divides<G4double>());
    std::transform(fRunAction->posyCCD.begin(), fRunAction->posyCCD.end(), fRunAction->Edep.begin(), fRunAction->posyCCD.begin(), std::divides<G4double>());
    std::transform(fRunAction->poszCCD.begin(), fRunAction->poszCCD.end(), fRunAction->Edep.begin(), fRunAction->poszCCD.begin(), std::divides<G4double>());
    std::transform(fRunAction->gposxCCD.begin(), fRunAction->gposxCCD.end(), fRunAction->Edep.begin(), fRunAction->gposxCCD.begin(), std::divides<G4double>());
    std::transform(fRunAction->gposyCCD.begin(), fRunAction->gposyCCD.end(), fRunAction->Edep.begin(), fRunAction->gposyCCD.begin(), std::divides<G4double>());
    std::transform(fRunAction->gposzCCD.begin(), fRunAction->gposzCCD.end(), fRunAction->Edep.begin(), fRunAction->gposzCCD.begin(), std::divides<G4double>());
    std::transform(fRunAction->time.begin(), fRunAction->time.end(), fRunAction->Edep.begin(), fRunAction->time.begin(), std::divides<G4double>());
    analysisManager->FillNtupleIColumn(2, 0, EventID);
    analysisManager->AddNtupleRow(2);
  }
  else {
    primVert = event->GetPrimaryVertex();
    primPart = primVert->GetPrimary();

    G4Navigator* navigator = G4TransportationManager::GetTransportationManager()->GetNavigatorForTracking();
    G4VPhysicalVolume* container = navigator->LocateGlobalPointAndSetup(primVert->GetPosition());
    G4String containerName = ";" + container->GetName() + ";";
    G4String nameList = fRunAction->runInfoVals.concatedVolumeNames;
    G4int namePos = nameList.find(containerName);
    G4int volid = std::count(nameList.begin(), nameList.begin() + namePos, ';');
  
    fRunAction->pdgPrim.push_back(primPart->GetPDGcode());
    fRunAction->primaryid.push_back(1);
    fRunAction->chargePrim.push_back(G4int(primPart->GetCharge()));
    fRunAction->volidPrim.push_back(volid);
    fRunAction->energyPrim.push_back(primPart->GetKineticEnergy() / eV);
    fRunAction->posxPrim.push_back(primVert->GetX0() / mm);
    fRunAction->posyPrim.push_back(primVert->GetY0() / mm);
    fRunAction->poszPrim.push_back(primVert->GetZ0() / mm);
    fRunAction->momxPrim.push_back(primPart->GetPx() / eV);
    fRunAction->momyPrim.push_back(primPart->GetPy() / eV);
    fRunAction->momzPrim.push_back(primPart->GetPz() / eV); 
    fRunAction->triggerTime.push_back(primVert->GetT0() / s);
  }
  analysisManager->FillNtupleIColumn(1, 0, EventID);
  analysisManager->AddNtupleRow(1);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
