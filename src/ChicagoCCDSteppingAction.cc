// ChicagoCCDSteppingAction.cc
// implementation of the ChicagoCCDSteppingAction class

#include "ChicagoCCDSteppingAction.hh"
#include "ChicagoCCDEventAction.hh"
#include "ChicagoCCDRunAction.hh"
#include "ChicagoCCDDetectorConstruction.hh"
#include "ChicagoCCDAnalysis.hh"
#include "ChicagoCCDTrackInformation.hh"

#include "G4Step.hh"
#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4LogicalVolume.hh"
#include "G4SteppingManager.hh"
#include "G4TouchableHistory.hh"
#include "G4Gamma.hh"
#include "G4TransportationManager.hh"

#include "G4ThreeVector.hh"

#include <vector>
#include <utility>
#include <algorithm>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ChicagoCCDSteppingAction::ChicagoCCDSteppingAction(ChicagoCCDEventAction* eventAction, ChicagoCCDRunAction* runAction, ChicagoCCDDetectorConstruction* detectorConstruction)
: G4UserSteppingAction(),
  fEventAction(eventAction),
  fScoringVolume(0),
  fRunAction(runAction),
  fDetectorConstruction(detectorConstruction)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ChicagoCCDSteppingAction::~ChicagoCCDSteppingAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ChicagoCCDSteppingAction::UserSteppingAction(const G4Step* step)
{
  const ChicagoCCDDetectorConstruction* ldetectorConstruction
      = static_cast<const ChicagoCCDDetectorConstruction*>
        (G4RunManager::GetRunManager()->GetUserDetectorConstruction());
  if (!fScoringVolume) { 
    fScoringVolume = ldetectorConstruction->GetScoringVolume();   
  }

  // get volume of the current step
  G4LogicalVolume* logVolume 
    = step->GetPreStepPoint()->GetTouchableHandle()
      ->GetVolume()->GetLogicalVolume();
  // check if we are in scoring volume
  if (logVolume != fScoringVolume) return;

  G4double edepStep = step->GetTotalEnergyDeposit();  
  if (edepStep > 0.) {
    // check if we are looking at the decay particle
    G4Track *track=step->GetTrack();
    // This step runs under the assumption that no particle that hits the CCD is not or does not descend from a primary
    // If it's breaking here, check that the primaries defined in StackingAction are not too restrictive
    ChicagoCCDTrackInformation* trackInfo = (ChicagoCCDTrackInformation*)(track->GetUserInformation());
    G4int primaryID = trackInfo->GetPrimaryTrackID();
    // If this primary has yet to be recorded in this event, add it to the list of primaries for this event and collect EventOut Info
    if (std::find(fRunAction->primaryid.begin(), fRunAction->primaryid.end(), primaryID) == fRunAction->primaryid.end()) {
      G4ParticleDefinition *primParticle = trackInfo->GetPrimaryParticleDefinition();
      G4double primEnergy = trackInfo->GetPrimaryEnergy();
      G4ThreeVector primPosition = trackInfo->GetPrimaryPosition();
      G4ThreeVector primMomentum = trackInfo->GetPrimaryMomentum();
      G4double trigTime = trackInfo->GetPrimaryTime();

      G4Navigator* navigator = new G4Navigator();
      navigator->SetWorldVolume(fDetectorConstruction->GetPhysWorld());
      G4VPhysicalVolume* container = navigator->LocateGlobalPointAndSetup(primPosition);
      G4String containerName = ";" + container->GetName() + ";";
      G4String nameList = fRunAction->runInfoVals.concatedVolumeNames;
      G4int namePos = nameList.find(containerName);
      G4int volid = std::count(nameList.begin(), nameList.begin() + namePos, ';');
      delete navigator;

      fRunAction->pdgPrim.push_back(primParticle->GetPDGEncoding());
      fRunAction->primaryid.push_back(primaryID);
      fRunAction->chargePrim.push_back(G4int(primParticle->GetPDGCharge()));
      fRunAction->volidPrim.push_back(volid);
      fRunAction->energyPrim.push_back(primEnergy / eV);
      fRunAction->posxPrim.push_back(primPosition.getX() / mm);
      fRunAction->posyPrim.push_back(primPosition.getY() / mm);
      fRunAction->poszPrim.push_back(primPosition.getZ() / mm);
      fRunAction->momxPrim.push_back(primMomentum.getX() / eV);
      fRunAction->momyPrim.push_back(primMomentum.getY() / eV);
      fRunAction->momzPrim.push_back(primMomentum.getZ() / eV);
      fRunAction->triggerTime.push_back(trigTime / s);
    }
   
    //
    // Collect CCDOut Info
    //

    G4VPhysicalVolume* physVolume = step->GetPreStepPoint()->GetTouchableHandle()->GetVolume();
    G4int CCDNum = fDetectorConstruction->GetCCDNum(physVolume);
    std::vector<G4ThreeVector> ActiveDims = fDetectorConstruction->GetActiveDims();
    G4ThreeVector CCDDim = ActiveDims[CCDNum - 1];
    G4double pixWidth = fDetectorConstruction->GetPixWidth();

    G4TouchableHandle theTouchable = step->GetPreStepPoint()->GetTouchableHandle();
    G4ThreeVector worldPosition = step->GetPreStepPoint()->GetPosition();
    G4ThreeVector localPosition = theTouchable->GetHistory()->GetTopTransform().TransformPoint(worldPosition);

    G4double posx = (localPosition.getX() + CCDDim.getX())/mm;
    G4double posy = (localPosition.getY() + CCDDim.getY())/mm;
    G4double posz = (CCDDim.getZ() - localPosition.getZ())/mm;

    G4int pixx = (G4int) floor(posx / pixWidth);
    G4int pixy = (G4int) floor(posy / pixWidth);
    G4int trackid = track->GetTrackID();
    std::vector<G4int> thisPix = {CCDNum, pixx, pixy, trackid};
    std::vector< std::vector<G4int> >::iterator it = std::find(fEventAction->usedPix.begin(), fEventAction->usedPix.end(), thisPix);
    if (it == fEventAction->usedPix.end()) {
      fEventAction->usedPix.push_back(thisPix);
      fRunAction->pdgCCD.push_back(track->GetParticleDefinition()->GetPDGEncoding());
      fRunAction->trackid.push_back(trackid);
      fRunAction->parentid.push_back(track->GetParentID());
      fRunAction->primaryidCCD.push_back(primaryID);
      fRunAction->CCDid.push_back(CCDNum);
      fRunAction->posxCCD.push_back(posx * edepStep / eV);
      fRunAction->posyCCD.push_back(posy * edepStep / eV);
      fRunAction->poszCCD.push_back(posz * edepStep / eV);
      fRunAction->gposxCCD.push_back(worldPosition.getX() * edepStep / (mm * eV));
      fRunAction->gposyCCD.push_back(worldPosition.getY() * edepStep / (mm * eV));
      fRunAction->gposzCCD.push_back(worldPosition.getZ() * edepStep / (mm * eV));
      fRunAction->Edep.push_back(edepStep / eV);
      fRunAction->time.push_back(track->GetGlobalTime() * edepStep / (s * eV));
    }
    else {
      G4int ind = std::distance(fEventAction->usedPix.begin(), it);
      fRunAction->posxCCD.at(ind) += posx * edepStep / eV;
      fRunAction->posyCCD.at(ind) += posy * edepStep / eV;
      fRunAction->poszCCD.at(ind) += posz * edepStep / eV;
      fRunAction->gposxCCD.at(ind) += worldPosition.getX() * edepStep/(mm * eV);
      fRunAction->gposyCCD.at(ind) += worldPosition.getY() * edepStep/(mm * eV);
      fRunAction->gposzCCD.at(ind) += worldPosition.getZ() * edepStep/(mm * eV);
      fRunAction->Edep.at(ind) += edepStep / eV;
      fRunAction->time.at(ind) += track->GetGlobalTime() * edepStep / (s * eV);
    }
  }  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

