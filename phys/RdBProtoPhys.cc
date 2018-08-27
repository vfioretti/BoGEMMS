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
//                          RdBProtoPhys.cc  -  description
//                             -------------------
//    Author		: Valentina Fioretti, Andrea Bulgarelli
//    creation date	: 10/02/2015
//    email		: fioretti@iasfbo.inaf.it, bulgarelli@iasfbo.inaf.it
// ***************************************************************************/
//
//

#if defined(GEANT4_10_1) || defined(GEANT4_10_2) || defined(GEANT4_10_3) || defined(GEANT4_10_4)

#include "RdBProtoPhys.hh"

#include "G4SystemOfUnits.hh"
#include "G4UnitsTable.hh"
#include "G4LossTableManager.hh"


// EM physics
#include "G4EmStandardPhysics.hh"
#include "G4EmStandardPhysics_option1.hh"
#include "G4EmStandardPhysics_option2.hh"
#include "G4EmStandardPhysics_option3.hh"
#include "G4EmStandardPhysics_option4.hh"
#include "G4EmLivermorePhysics.hh"
#include "G4EmPenelopePhysics.hh"
#include "G4EmLowEPPhysics.hh"


// Hadron physics
#if defined(GEANT4_10_1) || defined(GEANT4_10_2) || defined(GEANT4_10_3) || defined(GEANT4_10_4)
#include "G4HadronPhysicsQGSP_BIC_HP.hh"
#include "G4HadronPhysicsQGSP_BERT.hh"
#endif
#include "G4EmExtraPhysics.hh"
#include "G4HadronElasticPhysicsHP.hh"
#include "G4HadronElasticPhysics.hh"
#if defined(GEANT4_10_1) || defined(GEANT4_10_2) || defined(GEANT4_10_3) || defined(GEANT4_10_4)
#include "G4StoppingPhysics.hh"
#endif
#include "G4IonBinaryCascadePhysics.hh"
#include "G4NeutronTrackingCut.hh"
#include "G4IonPhysics.hh"
#include "G4IonPhysics.hh"
#include "G4HadronElasticPhysicsXS.hh"


// Decay and radioactive physics plus particles
#include "G4DecayPhysics.hh"
#include "G4RadioactiveDecayPhysics.hh"

// Em options
#include "G4EmProcessOptions.hh"

// Regions
#include "G4Region.hh"
#include "G4RegionStore.hh"
#include "G4ProductionCuts.hh"


RdBProtoPhys::RdBProtoPhys() : G4VModularPhysicsList()
{
    G4LossTableManager::Instance();
    defaultCutValue = 100.*um;
    cutForGamma     = defaultCutValue;
    cutForElectron  = defaultCutValue;
    cutForPositron  = defaultCutValue;
    cutForProton    = defaultCutValue;
    verboseLevel    = 1;
    
    G4ProductionCutsTable::GetProductionCutsTable()->SetEnergyRange(200*eV, 100*GeV);

    // Decay physics and all particles
    decRdBProtoPhys = new G4DecayPhysics();
    
    //radioactive physics
    raddecRdBProtoPhys = new G4RadioactiveDecayPhysics();
    
    // EM physics
    emRdBProtoPhys = new G4EmLivermorePhysics();
    
}


RdBProtoPhys::~RdBProtoPhys()
{
    
    delete decRdBProtoPhys;
    delete emRdBProtoPhys;
    delete raddecRdBProtoPhys;
    for(size_t i=0; i<hadronRdBProtoPhys.size(); i++) {
        delete hadronRdBProtoPhys[i];
    }
}


void RdBProtoPhys::ConstructParticle()
{
    decRdBProtoPhys->ConstructParticle();
}


void RdBProtoPhys::ConstructProcess()
{
    AddTransportation();
    
    emRdBProtoPhys->ConstructProcess();
    
    G4EmProcessOptions emOptions;
    //emOptions.SetFluo(true); // To activate the fluo emission if deexcitation is activated
    emOptions.SetAuger(true); // To activate Auger effect if deexcitation is activated
    emOptions.SetPIXE(true); // To activate Particle Induced X-Ray Emission (PIXE)
    
    
    decRdBProtoPhys->ConstructProcess();
    raddecRdBProtoPhys->ConstructProcess();
    for(size_t i=0; i<hadronRdBProtoPhys.size(); i++) {
        hadronRdBProtoPhys[i]->ConstructProcess();
    }
}


void RdBProtoPhys::AddPhysicsList(const G4String& name)
{
    if (verboseLevel>1) {
        G4cout << "RdBProtoPhys::AddPhysicsList: <" << name << ">" << G4endl;
    }
    
    if (name == "emstandard_opt0") {

    	delete emRdBProtoPhys;
    	emRdBProtoPhys = new G4EmStandardPhysics(1);

  	} else if (name == "emstandard_opt1") {

    	delete emRdBProtoPhys;
    	emRdBProtoPhys = new G4EmStandardPhysics_option1();

  	} else if (name == "emstandard_opt2") {

    	delete emRdBProtoPhys;
    	emRdBProtoPhys = new G4EmStandardPhysics_option2();
    
  	} else if (name == "emstandard_opt3") {

    	delete emRdBProtoPhys;
    	emRdBProtoPhys = new G4EmStandardPhysics_option3();    
    
  	} else if (name == "emstandard_opt4") {

    	delete emRdBProtoPhys;
    	emRdBProtoPhys = new G4EmStandardPhysics_option4();    

	} else if (name == "emlivermore") {
	
    	delete emRdBProtoPhys;
    	emRdBProtoPhys = new G4EmLivermorePhysics();

 	 } else if (name == "empenelope") {

    	delete emRdBProtoPhys;
    	emRdBProtoPhys = new G4EmPenelopePhysics();

  	} else if (name == "emlowenergy") {
  	
    	delete emRdBProtoPhys;
    	emRdBProtoPhys = new G4EmLowEPPhysics(); 
    }
    
    if (name == "QGSP_BIC_HP") {
        
        SetBuilderList0(true);
        #ifdef GEANT4_10_1
        hadronRdBProtoPhys.push_back( new G4HadronPhysicsQGSP_BIC_HP());
        #endif
    } else if (name == "QGSP_BERT") {
        
        SetBuilderList0(false);
        #ifdef GEANT4_10_1
        hadronRdBProtoPhys.push_back( new G4HadronPhysicsQGSP_BERT());
        #endif
    } else {
        
        G4cout << "RdBProtoPhys::AddPhysicsList: <" << name << ">"
        << " is not defined"
        << G4endl;
    }

}


void RdBProtoPhys::SetBuilderList0(G4bool flagHP)
{
    hadronRdBProtoPhys.push_back( new G4EmExtraPhysics(verboseLevel));
    if(flagHP) {
        hadronRdBProtoPhys.push_back( new G4HadronElasticPhysicsHP(verboseLevel) );
    } else {
        hadronRdBProtoPhys.push_back( new G4HadronElasticPhysics(verboseLevel) );
    }

    #ifdef GEANT4_10_1
    hadronRdBProtoPhys.push_back( new G4StoppingPhysics(verboseLevel));
    #endif
    hadronRdBProtoPhys.push_back( new G4IonBinaryCascadePhysics(verboseLevel));
    hadronRdBProtoPhys.push_back( new G4NeutronTrackingCut(verboseLevel));
}


void RdBProtoPhys::SetBuilderList1(G4bool flagHP)
{
    hadronRdBProtoPhys.push_back( new G4EmExtraPhysics(verboseLevel));
    if(flagHP) {
        hadronRdBProtoPhys.push_back( new G4HadronElasticPhysicsHP(verboseLevel) );
    } else {
        hadronRdBProtoPhys.push_back( new G4HadronElasticPhysics(verboseLevel) );
    }
    #ifdef GEANT4_10_1
    hadronRdBProtoPhys.push_back( new G4StoppingPhysics(verboseLevel));
    #endif
    hadronRdBProtoPhys.push_back( new G4IonPhysics(verboseLevel));
    hadronRdBProtoPhys.push_back( new G4NeutronTrackingCut(verboseLevel));
}


void RdBProtoPhys::SetBuilderList2(G4bool addStopping)
{
    hadronRdBProtoPhys.push_back( new G4EmExtraPhysics(verboseLevel));
    #ifdef GEANT4_10_1
    hadronRdBProtoPhys.push_back( new G4HadronElasticPhysics(verboseLevel));
    if(addStopping) { hadronRdBProtoPhys.push_back( new G4StoppingPhysics(verboseLevel)); }
    #endif
    hadronRdBProtoPhys.push_back( new G4IonPhysics(verboseLevel));
}


void RdBProtoPhys::SetBuilderList3()
{
    hadronRdBProtoPhys.push_back( new G4EmExtraPhysics(verboseLevel));
    RegisterPhysics( new G4HadronElasticPhysicsXS(verboseLevel) );
    #ifdef GEANT4_10_1
    hadronRdBProtoPhys.push_back( new G4StoppingPhysics(verboseLevel));
    #endif
    hadronRdBProtoPhys.push_back( new G4IonBinaryCascadePhysics(verboseLevel));
    hadronRdBProtoPhys.push_back( new G4NeutronTrackingCut(verboseLevel));

}


void RdBProtoPhys::SetBuilderList4()
{
    hadronRdBProtoPhys.push_back( new G4EmExtraPhysics(verboseLevel));
    
    #ifdef GEANT4_10_1
    hadronRdBProtoPhys.push_back( new G4HadronElasticPhysics(verboseLevel));
    hadronRdBProtoPhys.push_back( new G4StoppingPhysics(verboseLevel));
    #endif
    hadronRdBProtoPhys.push_back( new G4IonPhysics(verboseLevel));
    hadronRdBProtoPhys.push_back( new G4NeutronTrackingCut(verboseLevel));
}


void RdBProtoPhys::SetCuts()
{
    
    if (verboseLevel >0){
        G4cout << "RdBProtoPhys::SetCuts:";
        G4cout << "CutLength : " << G4BestUnit(defaultCutValue,"Length") << G4endl;
    }
    
    // set cut values for gamma at first and for e- second and next for e+,
    // because some processes for e+/e- need cut values for gamma
    SetCutValue(cutForGamma, "gamma");
    SetCutValue(cutForElectron, "e-");
    SetCutValue(cutForPositron, "e+");
    SetCutValue(cutForProton, "proton");
    
    // Production thresholds for detector regions
    /*
    G4Region* region;
    G4String regName;
    G4ProductionCuts* cuts;

    regName = "NbSlab";
    region = G4RegionStore::GetInstance()->GetRegion(regName);
    cuts = new G4ProductionCuts;
    cuts->SetProductionCut(100.*nm); // same cuts for gamma, e- and e+
    region->SetProductionCuts(cuts);
    */

    if (verboseLevel>0) DumpCutValuesTable();
}


void RdBProtoPhys::SetCutForGamma(G4double cut)
{
    cutForGamma = cut;
    SetParticleCuts(cutForGamma, G4Gamma::Gamma());
}


void RdBProtoPhys::SetCutForElectron(G4double cut)
{
    cutForElectron = cut;
    SetParticleCuts(cutForElectron, G4Electron::Electron());
}


void RdBProtoPhys::SetCutForPositron(G4double cut)
{
    cutForPositron = cut;
    SetParticleCuts(cutForPositron, G4Positron::Positron());
}


void RdBProtoPhys::SetCutForProton(G4double cut)
{
    cutForProton = cut;
    SetParticleCuts(cutForProton, G4Proton::Proton());
}

#endif

