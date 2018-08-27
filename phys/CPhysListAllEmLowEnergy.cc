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
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


#ifdef CPhysicsListAll_h

#include "CPhysListAllEmLowEnergy.hh"
#include "G4ParticleDefinition.hh"
#include "G4ProcessManager.hh"

// gamma
#include "G4LowEnergyRayleigh.hh"
#include "G4LowEnergyPhotoElectric.hh"
#include "G4LowEnergyCompton.hh"
#include "G4LowEnergyGammaConversion.hh"


// e-
#include "G4LowEnergyIonisation.hh"
#include "G4LowEnergyBremsstrahlung.hh"

// e+
#include "G4eIonisation.hh"
#include "G4eBremsstrahlung.hh"
#include "G4eplusAnnihilation.hh"

#ifdef GEANT4_9_1
#include "G4MultipleScattering.hh"
#endif

#include "G4hIonisation.hh"


#ifdef GEANT4_9_4

#include "G4hMultipleScattering.hh"
#include "G4eMultipleScattering.hh"

//#include "G4hBremsstrahlung.hh"
//#include "G4hPairProduction.hh"

// gamma
#include "G4PhotoElectricEffect.hh"
#include "G4LivermorePhotoElectricModel.hh"

#include "G4ComptonScattering.hh"
#include "G4LivermoreComptonModel.hh"
#include "G4LivermorePolarizedComptonModel.hh" // alternative for polarized photons

#include "G4GammaConversion.hh"
#include "G4LivermoreGammaConversionModel.hh"

#include "G4RayleighScattering.hh"
#include "G4LivermoreRayleighModel.hh"
#include "G4LivermorePolarizedRayleighModel.hh" // alternative for polarized photons

// e-

#include "G4eMultipleScattering.hh"
#include "G4UniversalFluctuation.hh"

#include "G4eIonisation.hh"
#include "G4LivermoreIonisationModel.hh"

#include "G4eBremsstrahlung.hh"
#include "G4LivermoreBremsstrahlungModel.hh"

//Penelope
#include "G4PenelopeRayleighModel.hh"
#include "G4PenelopeComptonModel.hh";
#include "G4PenelopePhotoElectricModel.hh";
#include "G4PenelopeGammaConversionModel.hh";
#include "G4PenelopeIonisationModel.hh";
#include "G4PenelopeBremsstrahlungModel.hh";
#include "G4PenelopeAnnihilationModel.hh";

#endif

#include "G4MuIonisation.hh"
#include "G4MuBremsstrahlung.hh"
#include "G4MuPairProduction.hh"
#include "G4MuonMinusCaptureAtRest.hh"

#include "G4hLowEnergyIonisation.hh"
#include "G4ionIonisation.hh"



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

CPhysListAllEmLowEnergy::CPhysListAllEmLowEnergy(int version, const G4String& name)
    :  G4VPhysicsConstructor(name)
{
    this->version = version;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

CPhysListAllEmLowEnergy::~CPhysListAllEmLowEnergy()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void CPhysListAllEmLowEnergy::ConstructProcess()
{
    // Add standard EM Processes

    theParticleIterator->reset();
    while( (*theParticleIterator)() ) {
        G4ParticleDefinition* particle = theParticleIterator->value();
        G4ProcessManager* pmanager = particle->GetProcessManager();
        G4String particleName = particle->GetParticleName();

        if (particleName == "gamma") {

            if(version == 1000) {
                pmanager->AddDiscreteProcess(new G4LowEnergyRayleigh());
                pmanager->AddDiscreteProcess(new G4LowEnergyPhotoElectric);
                pmanager->AddDiscreteProcess(new G4LowEnergyCompton);
                pmanager->AddDiscreteProcess(new G4LowEnergyGammaConversion);
            }
            if(version == 1001) {
#ifdef GEANT4_9_1
                G4cout << "low energy gamma Livermore not implemented" << G4endl;
                exit(0);
#endif
#ifdef GEANT4_9_4
                //gamma
                G4PhotoElectricEffect* thePhotoElectricEffect = new G4PhotoElectricEffect();
                G4LivermorePhotoElectricModel* theLivermorePhotoElectricModel = new G4LivermorePhotoElectricModel();
                thePhotoElectricEffect->SetModel(theLivermorePhotoElectricModel);
                pmanager->AddDiscreteProcess(thePhotoElectricEffect);

                theLivermorePhotoElectricModel->ActivateAuger(true);

                G4ComptonScattering* theComptonScattering = new G4ComptonScattering();
                G4LivermoreComptonModel* theLivermoreComptonModel = new G4LivermoreComptonModel();
                theComptonScattering->SetModel(theLivermoreComptonModel);
                pmanager->AddDiscreteProcess(theComptonScattering);

                G4GammaConversion* theGammaConversion = new G4GammaConversion();
                G4LivermoreGammaConversionModel* theLivermoreGammaConversionModel = new G4LivermoreGammaConversionModel();
                theGammaConversion->SetModel(theLivermoreGammaConversionModel);
                pmanager->AddDiscreteProcess(theGammaConversion);

                G4RayleighScattering* theRayleigh = new G4RayleighScattering();
                G4LivermoreRayleighModel* theRayleighModel = new G4LivermoreRayleighModel();
                theRayleigh->SetModel(theRayleighModel);
                pmanager->AddDiscreteProcess(theRayleigh);

                //????
                //pmanager->AddProcess(new G4StepLimiter(), -1, -1, 5);
#endif

            }
            if(version == 1002) {
#ifdef GEANT4_9_1
                G4cout << "low energy gamma Penelope not implemented" << G4endl;
                exit(0);
#endif
#ifdef GEANT4_9_4
                G4PhotoElectricEffect* thePhotoElectricEffect = new G4PhotoElectricEffect();
                G4PenelopePhotoElectricModel* thePenelopePhotoElectricModel = new G4PenelopePhotoElectricModel();
                thePhotoElectricEffect->SetModel(thePenelopePhotoElectricModel);
                pmanager->AddDiscreteProcess(thePhotoElectricEffect);

                thePenelopePhotoElectricModel->ActivateAuger(true);

                G4ComptonScattering* theComptonScattering = new G4ComptonScattering();
                G4PenelopeComptonModel* thePenelopeComptonModel = new G4PenelopeComptonModel();
                theComptonScattering->SetModel(thePenelopeComptonModel);
                pmanager->AddDiscreteProcess(theComptonScattering);

                G4GammaConversion* theGammaConversion = new G4GammaConversion();
                G4PenelopeGammaConversionModel* thePenelopeGammaConversionModel = new G4PenelopeGammaConversionModel();
                theGammaConversion->SetModel(thePenelopeGammaConversionModel);
                pmanager->AddDiscreteProcess(theGammaConversion);

                G4RayleighScattering* theRayleigh = new G4RayleighScattering();
                G4PenelopeRayleighModel* theRayleighModel = new G4PenelopeRayleighModel();
                theRayleigh->SetModel(theRayleighModel);
                pmanager->AddDiscreteProcess(theRayleigh);
#endif
            }

        } else if (particleName == "e-") {
            if(version == 1000) {
#ifdef GEANT4_9_1
                pmanager->AddProcess(new G4MultipleScattering,      -1, 1,1);
#endif
#ifdef GEANT4_9_4
                pmanager->AddProcess(new G4eMultipleScattering,      -1, 1,1);
#endif
                pmanager->AddProcess(new G4LowEnergyIonisation,     -1, 2,2);
                pmanager->AddProcess(new G4LowEnergyBremsstrahlung, -1,-1,3);
            }
            if(version == 1001) {
#ifdef GEANT4_9_1
                G4cout << "low energy e- Livermore not implemented" << G4endl;
                exit(0);
#endif
#ifdef GEANT4_9_4
                //electron
                G4eMultipleScattering* msc = new G4eMultipleScattering();
                //msc->SetStepLimitType(fUseDistanceToBoundary); //????
                pmanager->AddProcess(msc,                   -1, 1, 1);

                // Ionisation
                G4eIonisation* eIoni = new G4eIonisation();
                G4LivermoreIonisationModel* theLivermoreIonisationModel = new G4LivermoreIonisationModel();
                eIoni->SetEmModel(theLivermoreIonisationModel);
                //eIoni->SetStepFunction(0.2, 100*um); //????

                theLivermoreIonisationModel->ActivateAuger(true);

                pmanager->AddProcess(eIoni,                 -1, 2, 2);

                // Bremsstrahlung
                G4eBremsstrahlung* eBrem = new G4eBremsstrahlung();
                eBrem->SetEmModel(new G4LivermoreBremsstrahlungModel());
                pmanager->AddProcess(eBrem,         -1,-3, 3);

                //pmanager->AddProcess(new G4StepLimiter(), -1, -1, 4); //????
#endif
            }
            if(version == 1002) {
#ifdef GEANT4_9_1
                G4cout << "low energy e- Penelope not implemented" << G4endl;
                exit(0);
#endif
#ifdef GEANT4_9_4
                //electron
                G4eMultipleScattering* msc = new G4eMultipleScattering();
                //msc->SetStepLimitType(fUseDistanceToBoundary); //????
                pmanager->AddProcess(msc,                   -1, 1, 1);

                // Ionisation
                G4eIonisation* eIoni = new G4eIonisation();
                G4PenelopeIonisationModel* thePenelopeIonisationModel = new G4PenelopeIonisationModel();
                eIoni->SetEmModel(thePenelopeIonisationModel);
                //eIoni->SetStepFunction(0.2, 100*um); //????

                thePenelopeIonisationModel->ActivateAuger(true);

                pmanager->AddProcess(eIoni,                 -1, 2, 2);

                // Bremsstrahlung
                G4eBremsstrahlung* eBrem = new G4eBremsstrahlung();
                eBrem->SetEmModel(new G4PenelopeBremsstrahlungModel());
                pmanager->AddProcess(eBrem,         -1,-3, 3);



#endif
            }

        } else if (particleName == "e+") {
            if(version == 1000) {
                //Non presente in questa estensione alle basse energie
#ifdef GEANT4_9_1
                pmanager->AddProcess(new G4MultipleScattering,      -1, 1,1);
#endif
#ifdef GEANT4_9_4
                pmanager->AddProcess(new G4eMultipleScattering,      -1, 1,1);
#endif
                pmanager->AddProcess(new G4eIonisation,        -1, 2,2);
                pmanager->AddProcess(new G4eBremsstrahlung,    -1,-1,3);
                pmanager->AddProcess(new G4eplusAnnihilation,   0,-1,4);
            }
            if(version == 1001) {

                //Non presente nel Livermore
#ifdef GEANT4_9_1
                pmanager->AddProcess(new G4MultipleScattering,      -1, 1,1);
#endif
#ifdef GEANT4_9_4
                pmanager->AddProcess(new G4eMultipleScattering,      -1, 1,1);
#endif
                pmanager->AddProcess(new G4eIonisation,        -1, 2,2);
                pmanager->AddProcess(new G4eBremsstrahlung,    -1,-1,3);
                pmanager->AddProcess(new G4eplusAnnihilation,   0,-1,4);

            }
            if(version == 1002) {
#ifdef GEANT4_9_1
                G4cout << "low energy e+ Penelope not implemented" << G4endl;
                exit(0);
#endif
#ifdef GEANT4_9_4
                G4eMultipleScattering* msc = new G4eMultipleScattering();
                //msc->SetStepLimitType(fUseDistanceToBoundary); //????
                pmanager->AddProcess(msc,                   -1, 1, 1);

                // Ionisation
                G4eIonisation* eIoni = new G4eIonisation();
                G4PenelopeIonisationModel* thePenelopeIonisationModel = new G4PenelopeIonisationModel();
                eIoni->SetEmModel(thePenelopeIonisationModel);
                //eIoni->SetStepFunction(0.2, 100*um); //????

                thePenelopeIonisationModel->ActivateAuger(true);

                pmanager->AddProcess(eIoni,                 -1, 2, 2);

                // Bremsstrahlung
                G4eBremsstrahlung* eBrem = new G4eBremsstrahlung();
                eBrem->SetEmModel(new G4PenelopeBremsstrahlungModel());
                pmanager->AddProcess(eBrem,         -1,-3, 3);

                //annihilation
                G4eplusAnnihilation* epan = new G4eplusAnnihilation();
                G4PenelopeAnnihilationModel* thePenelopeAnnModel = new G4PenelopeAnnihilationModel();
                epan->SetModel(thePenelopeAnnModel);
                pmanager->AddProcess(epan,                 0, -1, 4);

#endif
            }

        } else if( particleName == "mu+" ||
                   particleName == "mu-"    ) {
#ifdef GEANT4_9_1
            pmanager->AddProcess(new G4MultipleScattering,      -1, 1,1);
#endif
#ifdef GEANT4_9_4
            pmanager->AddProcess(new G4hMultipleScattering,      -1, 1,1);
#endif

            pmanager->AddProcess(new G4MuIonisation,      -1, 2,2);
            pmanager->AddProcess(new G4MuBremsstrahlung,  -1,-1,3);
            pmanager->AddProcess(new G4MuPairProduction,  -1,-1,4);
            if( particleName == "mu-" ) pmanager->AddProcess(new G4MuonMinusCaptureAtRest(),0,-1,-1);
        } else if( particleName == "tau+" ||
                   particleName == "tau-"    ) {

#ifdef GEANT4_9_1
            pmanager->AddProcess(new G4MultipleScattering(),ordInActive,1,1);
#endif
#ifdef GEANT4_9_4
            pmanager->AddProcess(new G4hMultipleScattering,      ordInActive, 1,1);
#endif


            pmanager->AddProcess(new G4hIonisation(),       ordInActive,2,2);
            //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

        }
        /*else if( particleName == "proton" ) {

        	if(version == 1001 || version == 1002) {
        		//proton
        		#ifdef GEANT4_9_1
        		pmanager->AddProcess(new G4MultipleScattering,      -1, 1,1);
        		#endif
        		#ifdef GEANT4_9_4
        		pmanager->AddProcess(new G4hMultipleScattering, -1, 1, 1);
        		#endif
        		pmanager-> AddProcess(new G4hIonisation,         -1, 2, 2);
        		pmanager->AddProcess(new G4hBremsstrahlung,     -1, 3, 3);
        		pmanager->AddProcess(new G4hPairProduction,     -1, 4, 4);
        	}

        }*/
        else if (particleName == "GenericIon") {

#ifdef GEANT4_9_1
            pmanager->AddProcess(new G4MultipleScattering,      -1, 1,1);
#endif
#ifdef GEANT4_9_4
            pmanager->AddProcess(new G4hMultipleScattering,      -1, 1,1);
#endif
            if(version == 1000) {
                pmanager->AddProcess(new G4hLowEnergyIonisation, -1, 2,2);
            }
            if(version == 1001 || version == 1002) {
                pmanager->AddProcess(new G4hIonisation, -1, 2,2);
            }
        } else if ((!particle->IsShortLived()) &&
                   (particle->GetPDGCharge() != 0.0) &&
                   (particle->GetParticleName() != "chargedgeantino")) {

#ifdef GEANT4_9_1
            pmanager->AddProcess(new G4MultipleScattering,      -1, 1,1);
#endif
#ifdef GEANT4_9_4
            pmanager->AddProcess(new G4hMultipleScattering,      -1, 1,1);
#endif
            if(version == 1000) {
                pmanager->AddProcess(new G4hLowEnergyIonisation, -1, 2,2);
            }
            if(version == 1001 || version == 1002) {
                pmanager->AddProcess(new G4hIonisation, -1, 2,2);
            }
        }
    }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

