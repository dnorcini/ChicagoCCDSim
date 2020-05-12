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
// $Id: SkipperActionInitialization.cc 68058 2013-03-13 14:47:43Z gcosmo $
//
/// \file SkipperActionInitialization.cc
/// \brief Implementation of the SkipperActionInitialization class

#include "SkipperActionInitialization.hh"
#include "SkipperPrimaryGeneratorAction.hh"
#include "SkipperRunAction.hh"
#include "SkipperEventAction.hh"
#include "SkipperSteppingAction.hh"
#include "SkipperDetectorConstruction.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SkipperActionInitialization::SkipperActionInitialization(SkipperDetectorConstruction* detectConstruct)
 : G4VUserActionInitialization()
{
  detectorConstruction = detectConstruct;
  primGenAction = new SkipperPrimaryGeneratorAction(detectorConstruction); 
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SkipperActionInitialization::~SkipperActionInitialization()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SkipperActionInitialization::BuildForMaster() const
{
  SkipperRunAction* runAction = new SkipperRunAction(detectorConstruction, primGenAction);
  SetUserAction(runAction);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SkipperActionInitialization::Build() const
{
  SetUserAction(primGenAction);

  SkipperRunAction* runAction = new SkipperRunAction(detectorConstruction, primGenAction);
  SetUserAction(runAction);
  
  SkipperEventAction* eventAction = new SkipperEventAction(runAction);
  SetUserAction(eventAction);
  
  SetUserAction(new SkipperSteppingAction(eventAction,runAction,detectorConstruction));
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
