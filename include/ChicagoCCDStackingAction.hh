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
/// \file runAndEvent/ChicagoCCD/include/ChicagoCCDStackingAction.hh
/// \brief Definition of the ChicagoCCDStackingAction class
//
// $Id: ChicagoCCDStackingAction.hh 66379 2012-12-18 09:46:33Z gcosmo $
//

#ifndef ChicagoCCDStackingAction_H
#define ChicagoCCDStackingAction_H 1

#include "globals.hh"
#include "G4UserStackingAction.hh"

#include <vector>

class G4Track;

class ChicagoCCDStackingAction : public G4UserStackingAction
{
public:
  ChicagoCCDStackingAction();
  virtual ~ChicagoCCDStackingAction();

  std::vector< G4int > GetTrackList() const {return trackList;};

public:
  virtual G4ClassificationOfNewTrack ClassifyNewTrack(const G4Track* aTrack);
  virtual void NewStage();
  virtual void PrepareNewEvent();

  std::vector< G4int > GetParentList() {return parentList;};
private:
  std::vector< G4int > trackList;
  std::vector< G4int > parentList;
  G4int fStage;
};

#endif

