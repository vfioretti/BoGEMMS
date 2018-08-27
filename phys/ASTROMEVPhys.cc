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
//
// ***************************************************************************
//                          ASTROMEVPhys.cc  -  description
//                             -------------------
//    Author		: Valentina Fioretti, Andrea Bulgarelli
//    creation date	: 12/11/2014
//    email		: fioretti@iasfbo.inaf.it, bulgarelli@iasfbo.inaf.it
// ***************************************************************************/
//
//
// Modified by V. Fioretti (INAF/IASF Bologna) on 21/02/2016 to flag the Pair/Compton interaction
// Modified by V. Fioretti (INAF/IASF Bologna) on 27/08/2018 to remove Compton physics list


#include "THELGlobalMemory.hh"

#if defined(GEANT4_9_6) || defined(GEANT4_10_1) || defined(GEANT4_10_2) || defined(GEANT4_10_3) || defined(GEANT4_10_4)

#include "ASTROMEVPhys.hh"

#include "G4SystemOfUnits.hh"
#include "G4UnitsTable.hh"
#include "G4LossTableManager.hh"


// EM physics
#ifdef GEANT4_9_6
#include "G4EmLivermorePolarizedPhysics.hh"
#endif
#if defined(GEANT4_10_1) || defined(GEANT4_10_2) || defined(GEANT4_10_3) || defined(GEANT4_10_4)
#include "G4EmLivermorePolarizedPhysics.hh"
#endif

// Hadron physics
#ifdef GEANT4_9_6
#include "HadronPhysicsQGSP_BIC_HP.hh"
#endif
#if defined(GEANT4_10_1) || defined(GEANT4_10_2) || defined(GEANT4_10_3) || defined(GEANT4_10_4)
#include "G4HadronPhysicsQGSP_BIC_HP.hh"
#endif
#include "G4EmExtraPhysics.hh"
#include "G4HadronElasticPhysicsHP.hh"
#include "G4HadronElasticPhysics.hh"
#ifdef GEANT4_9_6
#include "G4QStoppingPhysics.hh"
#endif
#if defined(GEANT4_10_1) || defined(GEANT4_10_2) || defined(GEANT4_10_3) || defined(GEANT4_10_4)
#include "G4StoppingPhysics.hh"
#endif
#include "G4IonBinaryCascadePhysics.hh"
#include "G4NeutronTrackingCut.hh"
#include "G4IonPhysics.hh"
#ifdef GEANT4_9_6
#include "G4HadronElasticPhysicsLHEP.hh"
#endif
#include "G4IonPhysics.hh"
#include "G4HadronElasticPhysicsXS.hh"
#ifdef GEANT4_9_6
#include "G4HadronQElasticPhysics.hh"
#endif

// Decay and radioactive physics plus particles
#include "G4DecayPhysics.hh"
#include "G4RadioactiveDecayPhysics.hh"


ASTROMEVPhys::ASTROMEVPhys() : G4VModularPhysicsList()
{
  G4LossTableManager::Instance();
  defaultCutValue = 0.005*mm;
  cutForGamma     = defaultCutValue;
  cutForElectron  = defaultCutValue;
  cutForPositron  = defaultCutValue;
  cutForProton    = defaultCutValue;
  verboseLevel    = 1;

  // Decay physics and all particles
  decASTROMEVPhys = new G4DecayPhysics();

  //radioactive physics
  raddecASTROMEVPhys = new G4RadioactiveDecayPhysics();

  // Low energy EM physics Livermore Polarized
  #ifdef GEANT4_9_6
    emASTROMEVPhys = new G4EmLivermorePolarizedPhysics();
  #endif  
  #if defined(GEANT4_10_1) || defined(GEANT4_10_2) || defined(GEANT4_10_3) || defined(GEANT4_10_4)
    emASTROMEVPhys = new G4EmLivermorePolarizedPhysics();
  #endif

}


ASTROMEVPhys::~ASTROMEVPhys()
{

  delete decASTROMEVPhys;
  delete emASTROMEVPhys;
  delete raddecASTROMEVPhys;
  for(size_t i=0; i<hadronASTROMEVPhys.size(); i++) {
    delete hadronASTROMEVPhys[i];
  }
}


void ASTROMEVPhys::ConstructParticle()
{
  decASTROMEVPhys->ConstructParticle();
}


void ASTROMEVPhys::ConstructProcess()
{
  AddTransportation();

  emASTROMEVPhys->ConstructProcess();
  decASTROMEVPhys->ConstructProcess();
  raddecASTROMEVPhys->ConstructProcess();
  for(size_t i=0; i<hadronASTROMEVPhys.size(); i++) {
    hadronASTROMEVPhys[i]->ConstructProcess();
  }
}


void ASTROMEVPhys::AddPhysicsList(const G4String& name)
{
  if (verboseLevel>0) {
    G4cout << "ASTROMEVPhys::AddPhysicsList: <" << name << ">" << G4endl;
  }
  if (name == "QGSP_BIC_HP") {

    SetBuilderList0(true);
    #ifdef GEANT4_9_6
    hadronASTROMEVPhys.push_back( new HadronPhysicsQGSP_BIC_HP());
    #endif
    #if defined(GEANT4_10_1) || defined(GEANT4_10_2) || defined(GEANT4_10_3) || defined(GEANT4_10_4)
    hadronASTROMEVPhys.push_back( new G4HadronPhysicsQGSP_BIC_HP());
    #endif
  } else {

    G4cout << "ASTROMEVPhys::AddPhysicsList: <" << name << ">"
           << " is not defined"
           << G4endl;
  }
}


void ASTROMEVPhys::SetBuilderList0(G4bool flagHP)
{
  hadronASTROMEVPhys.push_back( new G4EmExtraPhysics(verboseLevel));
  if(flagHP) {
    hadronASTROMEVPhys.push_back( new G4HadronElasticPhysicsHP(verboseLevel) );
  } else {
    hadronASTROMEVPhys.push_back( new G4HadronElasticPhysics(verboseLevel) );
  }
  #ifdef GEANT4_9_6
  hadronASTROMEVPhys.push_back( new G4QStoppingPhysics(verboseLevel));
  #endif
  #if defined(GEANT4_10_1) || defined(GEANT4_10_2) || defined(GEANT4_10_3) || defined(GEANT4_10_4)
  hadronASTROMEVPhys.push_back( new G4StoppingPhysics(verboseLevel));
  #endif
  hadronASTROMEVPhys.push_back( new G4IonBinaryCascadePhysics(verboseLevel));
  hadronASTROMEVPhys.push_back( new G4NeutronTrackingCut(verboseLevel));
}


void ASTROMEVPhys::SetBuilderList1(G4bool flagHP)
{
  hadronASTROMEVPhys.push_back( new G4EmExtraPhysics(verboseLevel));
  if(flagHP) {
    hadronASTROMEVPhys.push_back( new G4HadronElasticPhysicsHP(verboseLevel) );
  } else {
    hadronASTROMEVPhys.push_back( new G4HadronElasticPhysics(verboseLevel) );
  }
  #ifdef GEANT4_9_6
  hadronASTROMEVPhys.push_back( new G4QStoppingPhysics(verboseLevel));
  #endif
  #if defined(GEANT4_10_1) || defined(GEANT4_10_2) || defined(GEANT4_10_3) || defined(GEANT4_10_4)
  hadronASTROMEVPhys.push_back( new G4StoppingPhysics(verboseLevel));
  #endif
  hadronASTROMEVPhys.push_back( new G4IonPhysics(verboseLevel));
  hadronASTROMEVPhys.push_back( new G4NeutronTrackingCut(verboseLevel));
}


void ASTROMEVPhys::SetBuilderList2(G4bool addStopping)
{
  hadronASTROMEVPhys.push_back( new G4EmExtraPhysics(verboseLevel));
  #ifdef GEANT4_9_6
  hadronASTROMEVPhys.push_back( new G4HadronElasticPhysicsLHEP(verboseLevel));
  if(addStopping) { hadronASTROMEVPhys.push_back( new G4QStoppingPhysics(verboseLevel)); }
  #endif
  #if defined(GEANT4_10_1) || defined(GEANT4_10_2) || defined(GEANT4_10_3) || defined(GEANT4_10_4)
  hadronASTROMEVPhys.push_back( new G4HadronElasticPhysics(verboseLevel));
  if(addStopping) { hadronASTROMEVPhys.push_back( new G4StoppingPhysics(verboseLevel)); }
  #endif
  hadronASTROMEVPhys.push_back( new G4IonPhysics(verboseLevel));
}


void ASTROMEVPhys::SetBuilderList3()
{
  hadronASTROMEVPhys.push_back( new G4EmExtraPhysics(verboseLevel));
  RegisterPhysics( new G4HadronElasticPhysicsXS(verboseLevel) );
  #ifdef GEANT4_9_6
  hadronASTROMEVPhys.push_back( new G4QStoppingPhysics(verboseLevel));
  #endif
  #if defined(GEANT4_10_1) || defined(GEANT4_10_2) || defined(GEANT4_10_3) || defined(GEANT4_10_4)
  hadronASTROMEVPhys.push_back( new G4StoppingPhysics(verboseLevel));
  #endif
  hadronASTROMEVPhys.push_back( new G4IonBinaryCascadePhysics(verboseLevel));
  hadronASTROMEVPhys.push_back( new G4NeutronTrackingCut(verboseLevel));
}


void ASTROMEVPhys::SetBuilderList4()
{
  hadronASTROMEVPhys.push_back( new G4EmExtraPhysics(verboseLevel));
    
  #ifdef GEANT4_9_6
  hadronASTROMEVPhys.push_back( new G4HadronQElasticPhysics(verboseLevel));
  hadronASTROMEVPhys.push_back( new G4QStoppingPhysics(verboseLevel));
  #endif
  #if defined(GEANT4_10_1) || defined(GEANT4_10_2) || defined(GEANT4_10_3) || defined(GEANT4_10_4)
  hadronASTROMEVPhys.push_back( new G4HadronElasticPhysics(verboseLevel));
  hadronASTROMEVPhys.push_back( new G4StoppingPhysics(verboseLevel));
  #endif
  hadronASTROMEVPhys.push_back( new G4IonPhysics(verboseLevel));
  hadronASTROMEVPhys.push_back( new G4NeutronTrackingCut(verboseLevel));
}


void ASTROMEVPhys::SetCuts()
{

  if (verboseLevel >0){
    G4cout << "ASTROMEVPhys::SetCuts:";
    G4cout << "CutLength : " << G4BestUnit(defaultCutValue,"Length") << G4endl;
  }

  // set cut values for gamma at first and for e- second and next for e+,
  // because some processes for e+/e- need cut values for gamma
  SetCutValue(cutForGamma, "gamma");
  SetCutValue(cutForElectron, "e-");
  SetCutValue(cutForPositron, "e+");
  SetCutValue(cutForProton, "proton");

  if (verboseLevel>0) DumpCutValuesTable();
}


void ASTROMEVPhys::SetCutForGamma(G4double cut)
{
  cutForGamma = cut;
  SetParticleCuts(cutForGamma, G4Gamma::Gamma());
}


void ASTROMEVPhys::SetCutForElectron(G4double cut)
{
  cutForElectron = cut;
  SetParticleCuts(cutForElectron, G4Electron::Electron());
}


void ASTROMEVPhys::SetCutForPositron(G4double cut)
{
  cutForPositron = cut;
  SetParticleCuts(cutForPositron, G4Positron::Positron());
}


void ASTROMEVPhys::SetCutForProton(G4double cut)
{
  cutForProton = cut;
  SetParticleCuts(cutForProton, G4Proton::Proton());
}

#endif

