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

#ifdef GEANT4_9_1
#define CPhysicsListAll_h 1
#endif
#ifdef GEANT4_9_4
#define CPhysicsListAll_h 1
#endif


#ifdef CPhysicsListAll_h


#include "G4VModularPhysicsList.hh"
#include "globals.hh"

class G4VPhysicsConstructor;
//class PhysicsListMessenger;
class G4ProductionCuts;


class CPhysicsListAll: public G4VModularPhysicsList
{
public:
    CPhysicsListAll(int version);
    ~CPhysicsListAll();

    void ConstructParticle();

    void SetCuts();
    void SetCutForGamma(G4double);
    void SetCutForElectron(G4double);
    void SetCutForPositron(G4double);

    void ConstructProcess();

//	void SetCollimator_cuts(G4double val);
//	void SetDetector_cuts(G4double val);
//	void SetPlattform_cuts(G4double val);

private:

// hide assignment operator
    CPhysicsListAll & operator=(const CPhysicsListAll &right);
    CPhysicsListAll(const CPhysicsListAll&);

    G4double cutForGamma;
    G4double cutForElectron;
    G4double cutForPositron;
//	G4double cutForPlattform;
//	G4double cutForDetector;
//	G4double cutForCollimator;

    G4VPhysicsConstructor*  emPhysicsList;
    G4VPhysicsConstructor*  muonPhysicsList;
    G4VPhysicsConstructor*  generalPhysicsList;
    G4VPhysicsConstructor*  particleList;
//  std::vector<G4VPhysicsConstructor*>  hadronPhys;
    G4VPhysicsConstructor*  hadPhysicsList;

    //PhysicsListMessenger* pMessenger;

    //G4ProductionCuts* Detector_cuts;
    //G4ProductionCuts* Collimator_cuts;
    //G4ProductionCuts* Plattform_cuts;

};





#endif