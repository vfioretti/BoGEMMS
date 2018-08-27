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

#ifdef CPhysicsListAll_h

#include "CPhysicsListAll.hh"

#include "CPhysListAllParticles.hh"
#include "CPhysListAllGeneral.hh"
#include "CPhysListAllEmLowEnergy.hh"
#include "CPhysListAllHadron.hh"
#include "G4RegionStore.hh"
#include "G4Region.hh"
#include "G4ProductionCuts.hh"
#include "G4ProcessManager.hh"
#include "G4ParticleTypes.hh"
#include "G4ParticleTable.hh"

#include "G4Gamma.hh"
#include "G4Electron.hh"
#include "G4Positron.hh"

#include "G4UnitsTable.hh"
#include "G4LossTableManager.hh"

#include "HadronPhysicsQGSP_BERT.hh"
#include "HadronPhysicsQGSP_BIC.hh"
//#include "HadronPhysicsQGSP_HP.hh"
//#include "HadronPhysicsLHEP_BERT.hh"
//#include "HadronPhysicsLHEP_BERT_HP.hh"
//#include "HadronPhysicsLHEP_BIC.hh"//=>QGSP_BIC
//#include "HadronPhysicsLHEP_BIC_HP.hh"//=>QGSP_BIC_HP
#include "HadronPhysicsQGSP_BIC_HP.hh"//


CPhysicsListAll::CPhysicsListAll(int version) : G4VModularPhysicsList()
{
    G4LossTableManager::Instance();
    defaultCutValue  = 0.01*mm;
    cutForGamma      = defaultCutValue;
    cutForElectron   = defaultCutValue;
    cutForPositron   = defaultCutValue;
//	cutForDetector   = defaultCutValue;
//	cutForCollimator = defaultCutValue;
//	cutForPlattform  = 1.*mm;

//	Detector_cuts = 0;
//	Collimator_cuts = 0;
//	Plattform_cuts = 0;

    //pMessenger = new PhysicsListMessenger(this);

    SetVerboseLevel(0);
//default physics
    // Particles
    particleList = new CPhysListAllParticles("particles");

    // General Physics
    generalPhysicsList = new CPhysListAllGeneral("general");

// EM physics
    //emPhysicsList = new PhysListEmStandard("em-standard");
    emPhysicsList = new CPhysListAllEmLowEnergy(version, "lowe-em");

// Had physics
    //hadPhysicsList = 0;
    hadPhysicsList = new CPhysListAllHadron(version, "hadron");
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

CPhysicsListAll::~CPhysicsListAll()
{
    //delete pMessenger;
    delete generalPhysicsList;
    delete emPhysicsList;
    delete hadPhysicsList;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void CPhysicsListAll::ConstructParticle()
{
    particleList->ConstructParticle();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void CPhysicsListAll::ConstructProcess()
{
    AddTransportation();
    //general
    generalPhysicsList->ConstructProcess();
    // em
    emPhysicsList->ConstructProcess();
    // had
    hadPhysicsList->ConstructProcess();
}



void CPhysicsListAll::SetCuts()
{
    SetCutValue(cutForGamma, "gamma");
    SetCutValue(cutForElectron, "e-");
    SetCutValue(cutForPositron, "e+");
    G4cout << "** World cuts are set" << G4endl;
    /*
    	G4Region* region;

    	if( !Collimator_cuts ) SetCollimator_cuts (cutForCollimator);
    	region = (G4RegionStore::GetInstance())->GetRegion("Collimator");
    	if (region)
    	{
    		region->SetProductionCuts(Collimator_cuts );
    		G4cout << "** Collimator cuts are set" << G4endl;
    	}
    	else G4cout << "** Detector cuts are not set" << G4endl;

    	if( !Detector_cuts ) SetDetector_cuts(cutForDetector);
    	region = G4RegionStore::GetInstance()->GetRegion("Detector");
    	if (region)
    	{
    		region->SetProductionCuts(Detector_cuts);
    		G4cout << "** Detector cuts are set" << G4endl;
    	}
    	else G4cout << "** Detector cuts are not set" << G4endl;

    	if (!Plattform_cuts) SetPlattform_cuts(cutForPlattform);
    	region = G4RegionStore::GetInstance()->GetRegion("Plattform");
    	if (region)
    	{
    		region->SetProductionCuts(Plattform_cuts);
    		G4cout << "** Plattform cuts are set" << G4endl;
    	}
    	else G4cout << "** Plattform cuts are not set" << G4endl;

    	if (verboseLevel>0) DumpCutValuesTable();
    */
}



void CPhysicsListAll::SetCutForGamma(G4double cut)
{
    cutForGamma = cut;
}


void CPhysicsListAll::SetCutForElectron(G4double cut)
{
    cutForElectron = cut;
}


void CPhysicsListAll::SetCutForPositron(G4double cut)
{
    cutForPositron = cut;
}


#endif
