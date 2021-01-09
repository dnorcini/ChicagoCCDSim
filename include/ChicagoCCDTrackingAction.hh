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
/// \file runAndEvent/ChicagoCCD/include/ChicagoCCDTrackingAction.hh
/// \brief Definition of the ChicagoCCDTrackingAction class
//
// $Id: ChicagoCCDTrackingAction.hh 66379 2012-12-18 09:46:33Z gcosmo $
//

#ifndef ChicagoCCDTrackingAction_h
#define ChicagoCCDTrackingAction_h 1

#include "G4Types.hh"
#include "G4UserTrackingAction.hh"
#include "ChicagoCCDStackingAction.hh"

class ChicagoCCDStackingAction;

class ChicagoCCDTrackingAction : public G4UserTrackingAction 
{
public:
  ChicagoCCDTrackingAction();
  virtual ~ChicagoCCDTrackingAction(){};
   
  virtual void PreUserTrackingAction(const G4Track*);
  virtual void PostUserTrackingAction(const G4Track*);
};

#endif

