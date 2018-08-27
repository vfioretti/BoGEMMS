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
/////////////////////////////////////////////////////////////////////////
//
// CPhysicsList
//
// Created: 31.04.2006 V.Ivanchenko
//
// Modified:
// 04.06.2006 Adoptation of hadr01 (V.Ivanchenko)
// 03.05.2010 Modified by A. Bulgarelli
//
////////////////////////////////////////////////////////////////////////
//

#ifdef GEANT4_9_1

#ifndef CPhysicsList_h
#define CPhysicsList_h 1

#include "G4VModularPhysicsList.hh"
#include "globals.hh"

class G4VPhysicsConstructor;
//class CPhysicsListMessenger;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class CPhysicsList: public G4VModularPhysicsList
{
public:

    CPhysicsList();
    virtual ~CPhysicsList();

    void ConstructParticle();

    void SetCuts();
    void SetCutForGamma(G4double);
    void SetCutForElectron(G4double);
    void SetCutForPositron(G4double);

    void AddPhysicsList(const G4String& name);
    void ConstructProcess();
    void List();

private:

    void SetBuilderList0(G4bool flagHP = false);
    void SetBuilderList1(G4bool flagHP = false);
    void SetBuilderList2(G4bool flagHP = false);
    void SetBuilderList3(G4bool flagHP = false);
    void SetBuilderList4(G4bool flagHP = false);
    void SetBuilderList5(G4bool flagHP = false);
    void SetBuilderList6(G4bool flagHP = false);

    G4double cutForGamma;
    G4double cutForElectron;
    G4double cutForPositron;

    G4VPhysicsConstructor*  emCPhysicsList;
    G4VPhysicsConstructor*  particleList;
    std::vector<G4VPhysicsConstructor*>  hadronPhys;

    //CPhysicsListMessenger* pMessenger;
    G4bool dump;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
#endif

