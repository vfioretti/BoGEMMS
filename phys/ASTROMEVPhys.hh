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
//                          ASTROMEVPhys.hh  -  description
//                             -------------------
//    Author		: Valentina Fioretti
//    creation date	: 12/11/2014
//    email		: fioretti@iasfbo.inaf.it
// ***************************************************************************/



#if defined(GEANT4_9_6) || defined(GEANT4_10_1) || defined(GEANT4_10_2) || defined(GEANT4_10_3) || defined(GEANT4_10_4)

#ifndef ASTROMEVPhys_h
#define ASTROMEVPhys_h 1

#include "G4VModularPhysicsList.hh"
#include "globals.hh"

class G4VPhysicsConstructor;


class ASTROMEVPhys: public G4VModularPhysicsList
{
public:

  ASTROMEVPhys();
  virtual ~ASTROMEVPhys();

  void ConstructParticle();
    
  void SetCuts();
  void SetCutForGamma(G4double);
  void SetCutForElectron(G4double);
  void SetCutForPositron(G4double);
  void SetCutForProton(G4double);
        
  void AddPhysicsList(const G4String& name);
  void ConstructProcess();
  
private:

  void SetBuilderList0(G4bool flagHP = false);
  void SetBuilderList1(G4bool flagHP = false);
  void SetBuilderList2(G4bool addStopping = false);
  void SetBuilderList3();
  void SetBuilderList4();

  G4double cutForGamma;
  G4double cutForElectron;
  G4double cutForPositron;
  G4double cutForProton;

  G4VPhysicsConstructor*  emASTROMEVPhys;
  G4VPhysicsConstructor*  decASTROMEVPhys;
  G4VPhysicsConstructor*  raddecASTROMEVPhys;
  std::vector<G4VPhysicsConstructor*>  hadronASTROMEVPhys;
    
};


#endif

#endif

