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
// **********************************************************************
// *                                                                    *
// *                    GEANT 4 xray_telescope advanced example         *
// *                                                                    *
// * MODULE:            CPhysicsListPhot.cc                           *
// * -------                                                            *
// *                                                                    *
// * Version:           0.4                                             *
// * Date:              06/11/00                                        *
// * Author:            R Nartallo                                      *
// * Organisation:      ESA/ESTEC, Noordwijk, THe Netherlands           *
// *                                                                    *
// **********************************************************************
//
// CHANGE HISTORY
// --------------
//
// 06.11.2000 R.Nartallo
// - First implementation of xray_telescope Physics list
// - Based on Chandra and XMM models
//
//
// **********************************************************************


#ifdef GEANT4_9_1

#include "THELGlobalMemory.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleWithCuts.hh"
#include "G4ProcessManager.hh"
#include "G4ProcessVector.hh"
#include "G4RegionStore.hh"
#include "G4ParticleTypes.hh"
#include "G4ParticleTable.hh"
#include "G4ShortLivedConstructor.hh"
#include "G4Material.hh"
#include "G4MaterialTable.hh"
#include "G4ios.hh"

#include "globals.hh"

#include "CPhysicsListPhot.hh"

CPhysicsListPhot::CPhysicsListPhot():  G4VUserPhysicsList() {
    // Default cut values

    gm.config->readInto(gm.enableHighEnergyPhysic, "HIGHENERGY.ENABLE");
    gm.config->readInto(gm.enableLowEnergyPhysic, "LOWENERGY.ENABLE");
    G4cout << "ENABLE HIGH ENERGY PROCESSES " << gm.enableHighEnergyPhysic << G4endl;
    G4cout << "ENABLE LOW ENERGY PROCESSES " << gm.enableLowEnergyPhysic << G4endl;

    cutForGamma     = 1.0*micrometer; //1e-6 m //VERSIONE 1
    cutForElectron  = 1.0*micrometer;
    cutForPositron  = 1.0*micrometer;

    defaultCutValue  = 0.01*mm; //1e-5 m //VERSIONE CHIPAUX 5
//	cutForDetector   = defaultCutValue;
//	cutForCollimator = defaultCutValue;
//	cutForPlattform  = 1.*mm;

//	Detector_cuts = 0;
//	Collimator_cuts = 0;
//	Plattform_cuts = 0;

    SetVerboseLevel(2);
}

CPhysicsListPhot::~CPhysicsListPhot() {}

void CPhysicsListPhot::ConstructParticle() {
    // Here are constructed all particles
    ConstructBosons();
    ConstructLeptons();
    ConstructMesons();
    ConstructBaryons();
    ConstructAllShortLiveds();
}

// In this method, static member functions should be called for ALL particles to be used.

void CPhysicsListPhot::ConstructBosons() {
    // pseudo-particles
    G4Geantino::GeantinoDefinition();
    G4ChargedGeantino::ChargedGeantinoDefinition();

    // gamma
    G4Gamma::GammaDefinition();

    // optical photon
    G4OpticalPhoton::OpticalPhotonDefinition();
}
void CPhysicsListPhot::ConstructLeptons() {
    // leptons
    G4Electron::ElectronDefinition();
    G4Positron::PositronDefinition();

    G4NeutrinoE::NeutrinoEDefinition();
    G4AntiNeutrinoE::AntiNeutrinoEDefinition();
    G4NeutrinoMu::NeutrinoMuDefinition();
    G4AntiNeutrinoMu::AntiNeutrinoMuDefinition();
}
void CPhysicsListPhot::ConstructMesons() {}
void CPhysicsListPhot::ConstructBaryons() {
    //  barions
    G4Proton::ProtonDefinition();
    G4AntiProton::AntiProtonDefinition();
    G4Neutron::NeutronDefinition();
    G4AntiNeutron::AntiNeutronDefinition();
}

void CPhysicsListPhot::ConstructAllShortLiveds() {}

void CPhysicsListPhot::ConstructProcess() {
    // Transportation, electromagnetic and general processes
    AddTransportation();
    ConstructGeneral();
    ConstructEM();

}

// Here are respective header files for chosen processes

#include "G4ComptonScattering.hh"
#include "G4GammaConversion.hh"
#include "G4PhotoElectricEffect.hh"
#include "G4eIonisation.hh"
#include "G4eBremsstrahlung.hh"
#include "G4eplusAnnihilation.hh"
#include "G4MultipleScattering.hh"
#include "G4hLowEnergyIonisation.hh"
#include <G4GammaConversionToMuons.hh>
#include <G4LowEnergyCompton.hh>
#include <G4LowEnergyPolarizedCompton.hh>
#include <G4LowEnergyRayleigh.hh>
#include <G4LowEnergyGammaConversion.hh>
#include <G4LowEnergyPhotoElectric.hh>
#include <G4LowEnergyIonisation.hh>
#include <G4LowEnergyBremsstrahlung.hh>
#include "G4eIonisation.hh"
#include "G4hIonisation.hh"
#include "G4eplusAnnihilation.hh"
#include "G4LowEnergyIonisation.hh"
#include "G4hLowEnergyIonisation.hh"

/**
 *
 */
void CPhysicsListPhot::ConstructEM() {
    theParticleIterator->reset();

    while( (*theParticleIterator)() ) {
        G4ParticleDefinition* particle = theParticleIterator->value();
        G4ProcessManager* pmanager = particle->GetProcessManager();
        G4String particleName = particle->GetParticleName();

        if (particleName == "gamma") {
            G4cout << "@@@@@ gamma" << G4endl;
            //gamma
            if(gm.enableHighEnergyPhysic == true) {
                pmanager->AddDiscreteProcess(new G4PhotoElectricEffect());
                pmanager->AddDiscreteProcess(new G4ComptonScattering());
                pmanager->AddDiscreteProcess(new G4GammaConversion());
            }
            //low energy
            if(gm.enableLowEnergyPhysic == true) {

                pmanager->AddDiscreteProcess(new G4LowEnergyRayleigh());
                pmanager->AddDiscreteProcess(new G4LowEnergyPhotoElectric());
                pmanager->AddDiscreteProcess(new G4LowEnergyCompton());
                pmanager->AddDiscreteProcess(new G4LowEnergyGammaConversion());

                //G4LowEnergyPhotoElectric* LePeprocess = new G4LowEnergyPhotoElectric();
                //LePeprocess->ActivateAuger(true);
                //LePeprocess->SetCutForLowEnSecPhotons(0.250 * keV);
                //LePeprocess->SetCutForLowEnSecElectrons(0.250 * keV);

                //pmanager->AddDiscreteProcess(LePeprocess);

            }
        } else if (particleName == "e-") {
            G4cout << "@@@@@ e-" << G4endl;
            //electron
            if(gm.enableHighEnergyPhysic == true) {
                pmanager->AddProcess(new G4MultipleScattering(),-1,1,1);
                pmanager->AddProcess(new G4eIonisation(),       -1,2,2);
                pmanager->AddProcess(new G4eBremsstrahlung(),   -1,3,3);
            }
            //low energy
            if(gm.enableLowEnergyPhysic == true) {
                pmanager->AddProcess(new G4MultipleScattering,-1,1,1);
                pmanager->AddProcess(new G4LowEnergyIonisation, -1, 2,2);
                pmanager->AddProcess(new G4LowEnergyBremsstrahlung, -1,-1,3);
                //LeIoprocess = new G4LowEnergyIonisation("IONI");
                //LeIoprocess->ActivateAuger(true);
                //eIoProcess = new G4eIonisation("stdIONI");
                //LeIoprocess->SetCutForLowEnSecPhotons(0.1*keV);
                //LeIoprocess->SetCutForLowEnSecElectrons(0.1*keV);
                //pmanager->AddProcess(LeIoprocess, -1,  2, 2);

                //LeBrprocess = new G4LowEnergyBremsstrahlung();
                //pmanager->AddProcess(LeBrprocess, -1, -1, 3);


            }
        } else if (particleName == "e+") {
            G4cout << "@@@@@ e+" << G4endl;
            //positron
            if(gm.enableHighEnergyPhysic == true) {
                pmanager->AddProcess(new G4MultipleScattering(),-1, 1,1);
                pmanager->AddProcess(new G4eIonisation(),       -1, 2,2);
                pmanager->AddProcess(new G4eBremsstrahlung(),   -1,-1,3);
                pmanager->AddProcess(new G4eplusAnnihilation(),  0,-1,4);
            }
            //low energy --
            if(gm.enableLowEnergyPhysic == true) {
                pmanager->AddProcess(new G4MultipleScattering, -1, 1,1);
                pmanager->AddProcess(new G4eIonisation,        -1, 2,2);
                pmanager->AddProcess(new G4eBremsstrahlung,    -1,-1,3);
                pmanager->AddProcess(new G4eplusAnnihilation,   0,-1,4);
            }
        } else if (particleName == "proton") {
            G4cout << "@@@@@ proton" << G4endl;

            if(gm.enableHighEnergyPhysic == true) {
                pmanager->AddProcess(new G4MultipleScattering,-1,1,1);
                pmanager->AddProcess(new G4hIonisation,       -1,2,2);
            }
            if(gm.enableLowEnergyPhysic == true) {
                G4double demax = 0.05;  // try to lose at most 5% of the energy in
                //    a single step (in limit of large energies)
                G4double stmin = 1.e-9 * m;  // length of the final step: 10 angstrom
                // reproduced angular distribution of TRIM
                pmanager->AddProcess(new G4MultipleScattering(),-1,1,1);

                G4hLowEnergyIonisation* lowEIonisation = new G4hLowEnergyIonisation();
                pmanager->AddProcess( lowEIonisation, -1,2,2);
                lowEIonisation->SetStepFunction( demax, stmin );
                lowEIonisation->SetFluorescence(true);
            }
        } else if ((!particle->IsShortLived()) &&
                   (particle->GetPDGCharge() != 0.0) &&
                   ((particle->GetParticleName() != "chargedgeantino") || (particle->GetParticleName() != "proton"))) {
            G4cout << "@@@@@ all other particles " << G4endl;
            //all others charged particles except geantino
            if(gm.enableHighEnergyPhysic == true) {
                pmanager->AddProcess(new G4MultipleScattering(),-1,1,1);
                //		pmanager->AddProcess(new G4hIonisation,       -1,2,2);
            }

            if(gm.enableLowEnergyPhysic == true) {
                pmanager->AddProcess(new G4MultipleScattering(),-1,1,1);

                G4double demax = 0.05;  // try to lose at most 5% of the energy in
                //    a single step (in limit of large energies)
                G4double stmin = 1.e-9 * m;  // length of the final step: 10 angstrom
                // reproduced angular distribution of TRIM

                G4hLowEnergyIonisation* lowEIonisation = new G4hLowEnergyIonisation();
                pmanager->AddProcess( lowEIonisation, -1,2,2);
                lowEIonisation->SetStepFunction( demax, stmin );
            }
        }
    }
}

#include "G4Decay.hh"

void CPhysicsListPhot::ConstructGeneral() {
    G4Decay* theDecayProcess = new G4Decay();
    theParticleIterator->reset();
    while( (*theParticleIterator)() ) {
        G4ParticleDefinition* particle = theParticleIterator->value();
        G4cout << "@@@@ particle: " << particle->GetParticleName() << G4endl;
        G4ProcessManager* pmanager = particle->GetProcessManager();
        if (theDecayProcess->IsApplicable(*particle)) {
            pmanager ->AddProcess(theDecayProcess);
            pmanager ->SetProcessOrdering(theDecayProcess, idxPostStep);
            pmanager ->SetProcessOrdering(theDecayProcess, idxAtRest);
        }
    }
}

void CPhysicsListPhot::SetCuts() {
    // defaultCutValue you have typed in is used

    if (verboseLevel >1) {
        G4cout << "CPhysicsListPhot::SetCuts:" << G4endl;
    }

    // set cut values for gamma at first and for e- second
    SetCutValue(cutForGamma, "gamma");
    SetCutValue(cutForElectron, "e-");
    SetCutValue(cutForPositron, "e+");


    // SetCutValueForOthers(defaultCutValue);
    /*
    	G4Region* region;

    	int geomdetectorversion = 1;
     	gm.config->readInto(geomdetectorversion, "GEOM.DETECTOR.VERSION");

     	if(geomdetectorversion == 5) {
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
    	}

    	if (verboseLevel >1) {
    		DumpCutValuesTable();
    	}*/
}

void CPhysicsListPhot::SetCutForGamma(G4double cut) {
    ResetCuts();
    cutForGamma = cut;
}

void CPhysicsListPhot::SetCutForElectron(G4double cut) {
    ResetCuts();
    cutForElectron = cut;
}

void CPhysicsListPhot::SetCutForPositron(G4double cut)
{
    cutForPositron = cut;
}


G4double CPhysicsListPhot::GetCutForGamma() const {
    return cutForGamma;
}

G4double CPhysicsListPhot::GetCutForElectron() const {
    return cutForElectron;
}

G4double CPhysicsListPhot::GetCutForPositron() const {
    return cutForPositron;
}


/*
void CPhysicsListPhot::SetCollimator_cuts(G4double cut)
{
	if( !Collimator_cuts ) Collimator_cuts = new G4ProductionCuts();

	Collimator_cuts->SetProductionCut(cut, idxG4GammaCut);
	Collimator_cuts->SetProductionCut(cut, idxG4ElectronCut);
	Collimator_cuts->SetProductionCut(cut, idxG4PositronCut);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void CPhysicsListPhot::SetDetector_cuts(G4double cut)
{
	if( !Detector_cuts ) Detector_cuts = new G4ProductionCuts();

	Detector_cuts->SetProductionCut(cut, idxG4GammaCut);
	Detector_cuts->SetProductionCut(cut, idxG4ElectronCut);
	Detector_cuts->SetProductionCut(cut, idxG4PositronCut);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void CPhysicsListPhot::SetPlattform_cuts(G4double cut)
{
	if( !Plattform_cuts ) Plattform_cuts = new G4ProductionCuts();

	Plattform_cuts->SetProductionCut(cut, idxG4GammaCut);
	Plattform_cuts->SetProductionCut(cut, idxG4ElectronCut);
	Plattform_cuts->SetProductionCut(cut, idxG4PositronCut);
}
*/

#endif
