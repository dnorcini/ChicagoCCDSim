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
/// \file runAndEvent/ChicagoCCD/include/ChicagoCCDTrackInformation.hh
/// \brief Definition of the ChicagoCCDTrackInformation class
//
//
// $Id: ChicagoCCDTrackInformation.hh 97671 2016-06-07 08:25:00Z gcosmo $
//

#ifndef ChicagoCCDTrackInformation_h
#define ChicagoCCDTrackInformation_h 1

#include "globals.hh"
#include "G4ThreeVector.hh"
#include "G4ParticleDefinition.hh"
#include "G4Track.hh"
#include "G4Allocator.hh"
#include "G4VUserTrackInformation.hh"

class ChicagoCCDTrackInformation : public G4VUserTrackInformation 
{
public:
  ChicagoCCDTrackInformation();
  ChicagoCCDTrackInformation(const G4Track* aTrack);
  ChicagoCCDTrackInformation(const ChicagoCCDTrackInformation* aTrackInfo);
  virtual ~ChicagoCCDTrackInformation();
   
  inline void *operator new(size_t);
  inline void operator delete(void *aTrackInfo);

  ChicagoCCDTrackInformation& operator =(const ChicagoCCDTrackInformation& right);
  
  virtual void Print() const;

public:
  G4int                 GetPrimaryTrackID() {return fOriginalTrackID;};
  G4ParticleDefinition* GetPrimaryParticleDefinition() {return fParticleDefinition;};
  G4ThreeVector         GetPrimaryPosition() {return fOriginalPosition;};
  G4ThreeVector         GetPrimaryMomentum() {return fOriginalMomentum;};
  G4double              GetPrimaryEnergy() {return fOriginalEnergy;};
  G4double              GetPrimaryTime() {return fOriginalTime;};

  G4bool isNew = true;
private:
  // Information of the primary track at the primary vertex
  G4int                 fOriginalTrackID;
  G4ParticleDefinition* fParticleDefinition;
  G4ThreeVector         fOriginalPosition;
  G4ThreeVector         fOriginalMomentum;
  G4double              fOriginalEnergy;
  G4double              fOriginalTime;
};

extern G4ThreadLocal
 G4Allocator<ChicagoCCDTrackInformation> * aTrackInformationAllocator;

inline void* ChicagoCCDTrackInformation::operator new(size_t)
{
  if(!aTrackInformationAllocator)
    aTrackInformationAllocator = new G4Allocator<ChicagoCCDTrackInformation>;
  return (void*)aTrackInformationAllocator->MallocSingle();
}

inline void ChicagoCCDTrackInformation::operator delete(void *aTrackInfo)
{ aTrackInformationAllocator->FreeSingle((ChicagoCCDTrackInformation*)aTrackInfo);}

#endif

