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
// $Id: ChicagoCCDActionInitialization.cc 68058 2013-03-13 14:47:43Z gcosmo $
//
/// \file ChicagoCCDActionInitialization.cc
/// \brief Implementation of the ChicagoCCDActionInitialization class

#include "ChicagoCCDActionInitialization.hh"
#include "ChicagoCCDPrimaryGeneratorAction.hh"
#include "ChicagoCCDRunAction.hh"
#include "ChicagoCCDEventAction.hh"
#include "ChicagoCCDStackingAction.hh"
#include "ChicagoCCDTrackingAction.hh"
#include "ChicagoCCDSteppingAction.hh"
#include "ChicagoCCDDetectorConstruction.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ChicagoCCDActionInitialization::ChicagoCCDActionInitialization(ChicagoCCDDetectorConstruction* detectConstruct)
 : G4VUserActionInitialization(),
        detectorConstruction(detectConstruct)

{;}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ChicagoCCDActionInitialization::~ChicagoCCDActionInitialization()
{;}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ChicagoCCDActionInitialization::BuildForMaster() const
{;}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ChicagoCCDActionInitialization::Build() const
{
  ChicagoCCDPrimaryGeneratorAction* primGenAction = new ChicagoCCDPrimaryGeneratorAction();
  SetUserAction(primGenAction);

  ChicagoCCDRunAction* runAction = new ChicagoCCDRunAction(detectorConstruction, primGenAction);
  SetUserAction(runAction);
  
  ChicagoCCDEventAction* eventAction = new ChicagoCCDEventAction(runAction);
  SetUserAction(eventAction);

  SetUserAction(new ChicagoCCDStackingAction());

  SetUserAction(new ChicagoCCDTrackingAction());

  SetUserAction(new ChicagoCCDSteppingAction(eventAction,runAction,detectorConstruction));
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
