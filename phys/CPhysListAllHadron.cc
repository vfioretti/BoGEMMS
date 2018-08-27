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
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

#ifdef CPhysicsListAll_h


#include "CPhysListAllHadron.hh"

#include "G4ParticleDefinition.hh"
#include "G4ProcessManager.hh"

#include "G4TheoFSGenerator.hh"
#include "G4ExcitationHandler.hh"
#include "G4Evaporation.hh"
#include "G4FermiBreakUp.hh"
#include "G4GeneratorPrecompoundInterface.hh"
#include "G4PreCompoundModel.hh"
#include "G4QGSModel.hh"
#include "G4QGSParticipants.hh"
#include "G4QGSMFragmentation.hh"
#include "G4ExcitedStringDecay.hh"

#include "G4LElastic.hh"
#include "G4LFission.hh"
#include "G4LCapture.hh"

#include "G4CascadeInterface.hh"

#include "G4BinaryCascade.hh"
#include "G4BinaryLightIonReaction.hh"

//HPNeutron

#include "G4NeutronHPElastic.hh"
#include "G4NeutronHPElasticData.hh"
#include "G4NeutronHPCapture.hh"
#include "G4NeutronHPCaptureData.hh"
#include "G4NeutronHPInelastic.hh"
#include "G4NeutronHPInelasticData.hh"
//c-s
#include "G4TripathiCrossSection.hh"
#include "G4IonsShenCrossSection.hh"
#include "G4ProtonInelasticCrossSection.hh"
#include "G4NeutronInelasticCrossSection.hh"

CPhysListAllHadron::CPhysListAllHadron(int version, const G4String& name)
    :  G4VPhysicsConstructor(name)
{
    this->version = version;
}

CPhysListAllHadron::~CPhysListAllHadron()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void CPhysListAllHadron::ConstructProcess()
{

    G4ProcessManager * pManager = 0;

    // this will be the model class for high energies
    G4TheoFSGenerator * theTheoModel = new G4TheoFSGenerator;
    // all models for treatment of thermal nucleus
    G4Evaporation * theEvaporation = new G4Evaporation;
    G4FermiBreakUp * theFermiBreakUp = new G4FermiBreakUp;
    G4StatMF * theMF = new G4StatMF;
    // Evaporation logic
    G4ExcitationHandler * theHandler = new G4ExcitationHandler;
    theHandler->SetEvaporation(theEvaporation);
    theHandler->SetFermiModel(theFermiBreakUp);
    theHandler->SetMultiFragmentation(theMF);
    theHandler->SetMaxAandZForFermiBreakUp(12, 6);
    theHandler->SetMinEForMultiFrag(5*MeV);
    // Pre equilibrium stage
    G4PreCompoundModel * thePreEquilib = new G4PreCompoundModel(theHandler);

    // a no-cascade generator-precompound interaface
    G4GeneratorPrecompoundInterface * theCascade = new G4GeneratorPrecompoundInterface;
    theCascade->SetDeExcitation(thePreEquilib);

    // here come the high energy parts
    // the string model; still not quite according to design - Explicite use of the forseen interfaces
    G4VPartonStringModel * theStringModel;
    theStringModel = new G4QGSModel<G4QGSParticipants>;
    theTheoModel->SetTransport(theCascade);
    theTheoModel->SetHighEnergyGenerator(theStringModel);
    theTheoModel->SetMinEnergy(10*GeV);  // 15 GeV may be the right limit
    theTheoModel->SetMaxEnergy(100*TeV);

    G4VLongitudinalStringDecay * theFragmentation = new G4QGSMFragmentation;
    G4ExcitedStringDecay * theStringDecay = new G4ExcitedStringDecay(theFragmentation);
    theStringModel->SetFragmentationModel(theStringDecay);

    // Elastic Process

    theElasticProcess.RegisterMe(new G4LElastic());

    // ---------------------------------------------------------------------------
    // Hadron elastic process
    // for all particles except neutrons

    theParticleIterator->reset();
    while( (*theParticleIterator)() ) {
        G4ParticleDefinition* particle = theParticleIterator->value();
        G4String particleName = particle->GetParticleName();
        if (particleName != "neutron") {
            G4ProcessManager* pManager = particle->GetProcessManager();
            if (particle->GetPDGMass() > 110.*MeV && theElasticProcess.IsApplicable(*particle)
                    && !particle->IsShortLived()) {
                pManager->AddDiscreteProcess(&theElasticProcess);
                //
                //	G4cout << "### Elastic model are registered for "
                //       << particle->GetParticleName()
                //       << G4endl;
            }
        }
    }
    // Proton
    pManager = G4Proton::Proton()->GetProcessManager();
    // add inelastic process
    // Binary Cascade
    G4BinaryCascade * theBC = new G4BinaryCascade;
    theBC->SetMaxEnergy(10.5*GeV);
    theProtonInelastic.RegisterMe(theBC);
    // Higher energy
    theProtonInelastic.RegisterMe(theTheoModel);
    // now the cross-sections.
    G4ProtonInelasticCrossSection * theProtonData = new G4ProtonInelasticCrossSection;
    theProtonInelastic.AddDataSet(theProtonData);
    pManager->AddDiscreteProcess(&theProtonInelastic);

    // taken from GammaRayTel!!!!!!!!!
    pManager->AddDiscreteProcess(&theElasticProcess);
    if(version == 1000) {
        pManager->AddProcess(&theProtonIonisation, ordInActive,2, 2);
        pManager->AddProcess(&theProtonMult);
        pManager->SetProcessOrdering(&theProtonMult, idxAlongStep, 1);
        pManager->SetProcessOrdering(&theProtonMult, idxPostStep, 1);
    }
    //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    //
    //
    // Neutron
    pManager = G4Neutron::Neutron()->GetProcessManager();
    // add process
    // elastic scattering
    theNeutronElasticProcess =
        new G4HadronElasticProcess();
    G4LElastic* theElasticModel1 = new G4LElastic;
    G4NeutronHPElastic * theElasticNeutron = new G4NeutronHPElastic;
    theNeutronElasticProcess->RegisterMe(theElasticModel1);
    theElasticModel1->SetMinEnergy(19.*MeV);
    theNeutronElasticProcess->RegisterMe(theElasticNeutron);
    theElasticNeutron->SetMaxEnergy(20.*MeV);

    G4NeutronHPElasticData * theNeutronData = new G4NeutronHPElasticData;
    theNeutronElasticProcess->AddDataSet(theNeutronData);
    pManager->AddDiscreteProcess(theNeutronElasticProcess);
    // inelastic
    G4NeutronHPInelastic * theHPNeutronInelasticModel =
        new G4NeutronHPInelastic;
    theHPNeutronInelasticModel->SetMaxEnergy(20.*MeV);
    theNeutronInelastic.RegisterMe(theHPNeutronInelasticModel);
    G4NeutronHPInelasticData * theNeutronData1 = new G4NeutronHPInelasticData;
    theNeutronInelastic.AddDataSet(theNeutronData1);
    // binary
    G4BinaryCascade * neutronBC = new G4BinaryCascade;
    neutronBC->SetMinEnergy(19.*MeV);
    neutronBC->SetMaxEnergy(10.5*GeV);
    theNeutronInelastic.RegisterMe(neutronBC);
    // higher energy
    theNeutronInelastic.RegisterMe(theTheoModel);
    // now the cross-sections.
    G4NeutronInelasticCrossSection * theNeutronData2 = new G4NeutronInelasticCrossSection;
    theNeutronInelastic.AddDataSet(theNeutronData2);
    pManager->AddDiscreteProcess(&theNeutronInelastic);
    // fission
    theFissionProcess =
        new G4HadronFissionProcess;
    G4LFission* theFissionModel = new G4LFission;
    theFissionProcess->RegisterMe(theFissionModel);
    pManager->AddDiscreteProcess(theFissionProcess);
    //capture
    theCaptureProcess =
        new G4HadronCaptureProcess;
    G4LCapture* theCaptureModel = new G4LCapture;
    theCaptureProcess->RegisterMe(theCaptureModel);
    theCaptureModel->SetMinEnergy(19.*MeV);
    G4NeutronHPCapture * theHPNeutronCaptureModel = new G4NeutronHPCapture;
    theCaptureProcess->RegisterMe(theHPNeutronCaptureModel);
    G4NeutronHPCaptureData * theNeutronData3 = new G4NeutronHPCaptureData;
    theCaptureProcess->AddDataSet(theNeutronData3);
    pManager->AddDiscreteProcess(theCaptureProcess);

    // now light ions
    // light Ion BC
    G4BinaryLightIonReaction * theIonBC= new G4BinaryLightIonReaction;
    theIonBC->SetMinEnergy(1*MeV);
    theIonBC->SetMaxEnergy(20*GeV);
    G4TripathiCrossSection * TripathiCrossSection= new G4TripathiCrossSection;
    G4IonsShenCrossSection * aShen = new G4IonsShenCrossSection;

    // deuteron
    pManager = G4Deuteron::Deuteron()->GetProcessManager();
    theDeuteronInelasticProcess =
        new G4DeuteronInelasticProcess("inelastic");
    //  G4LEDeuteronInelastic* theDeuteronInelasticModel =
    //   new G4LEDeuteronInelastic;
    // theDeuteronInelasticModel->SetMaxEnergy(100*MeV);
    theDeuteronInelasticProcess->AddDataSet(TripathiCrossSection);
    theDeuteronInelasticProcess->AddDataSet(aShen);
    //  theDeuteronInelasticProcess->RegisterMe(theDeuteronInelasticModel);
    theDeuteronInelasticProcess->RegisterMe(theIonBC);
    theDeuteronInelasticProcess->RegisterMe(theTheoModel);
    pManager->AddDiscreteProcess(theDeuteronInelasticProcess);
    // triton
    pManager = G4Triton::Triton()->GetProcessManager();
    theTritonInelasticProcess =
        new G4TritonInelasticProcess("inelastic");
    //  G4LETritonInelastic* theTritonInelasticModel =
    //  new G4LETritonInelastic;
    //theTritonInelasticModel->SetMaxEnergy(100*MeV);
    theTritonInelasticProcess->AddDataSet(TripathiCrossSection);
    theTritonInelasticProcess->AddDataSet(aShen);
    //  theTritonInelasticProcess->RegisterMe(theTritonInelasticModel);
    theTritonInelasticProcess->RegisterMe(theIonBC);
    theTritonInelasticProcess->RegisterMe(theTheoModel);
    pManager->AddDiscreteProcess(theTritonInelasticProcess);
    // alpha
    pManager = G4Alpha::Alpha()->GetProcessManager();
    theAlphaInelasticProcess =
        new G4AlphaInelasticProcess("inelastic");
    // G4LEAlphaInelastic* theAlphaInelasticModel =
    //  new G4LEAlphaInelastic;
    //theAlphaInelasticModel->SetMaxEnergy(100*MeV);
    theAlphaInelasticProcess->AddDataSet(TripathiCrossSection);
    theAlphaInelasticProcess->AddDataSet(aShen);
    //  theAlphaInelasticProcess->RegisterMe(theAlphaInelasticModel);
    theAlphaInelasticProcess->RegisterMe(theIonBC);
    theAlphaInelasticProcess->RegisterMe(theTheoModel);
    pManager->AddDiscreteProcess(theAlphaInelasticProcess);
    // GenericIon
    pManager = G4GenericIon::GenericIon()->GetProcessManager();
    // need to add the elastic explicitly
    pManager->AddDiscreteProcess(&theElasticProcess);
    theIonInelasticProcess =
        new G4IonInelasticProcess();
    theIonInelasticProcess->AddDataSet(TripathiCrossSection);
    theIonInelasticProcess->AddDataSet(aShen);
    //  G4BinaryLightIonReaction * theGenIonBC= new G4BinaryLightIonReaction;
    // theGenIonBC->SetMinEnergy(0*MeV);
    //theGenIonBC->SetMaxEnergy(20*GeV);
    theIonInelasticProcess->RegisterMe(theIonBC);
    theIonInelasticProcess->RegisterMe(theTheoModel);
    pManager->AddDiscreteProcess(theIonInelasticProcess);



    // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    // taken from GammaRayTelescope !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    // replaced theModel through theTheoModel

    // anti-Proton
    pManager = G4AntiProton::AntiProton()->GetProcessManager();
    // add process
    pManager->AddDiscreteProcess(&theElasticProcess);

    theLEAntiProtonModel = new G4LEAntiProtonInelastic();
    theHEAntiProtonModel = new G4HEAntiProtonInelastic();
    theAntiProtonInelastic.RegisterMe(theLEAntiProtonModel);
    theAntiProtonInelastic.RegisterMe(theHEAntiProtonModel);
    pManager->AddDiscreteProcess(&theAntiProtonInelastic);

    pManager->AddProcess(&theAntiProtonIonisation, ordInActive,2, 2);

    pManager->AddProcess(&theAntiProtonMult);
    pManager->SetProcessOrdering(&theAntiProtonMult, idxAlongStep, 1);
    pManager->SetProcessOrdering(&theAntiProtonMult, idxPostStep, 1);

    pManager->AddRestProcess(&theAntiProtonAnnihilation);

    // AntiNeutron
    pManager = G4AntiNeutron::AntiNeutron()->GetProcessManager();
    // add process
    pManager->AddDiscreteProcess(&theElasticProcess);

    theLEAntiNeutronModel = new G4LEAntiNeutronInelastic();
    theHEAntiNeutronModel = new G4HEAntiNeutronInelastic();
    theAntiNeutronInelastic.RegisterMe(theLEAntiNeutronModel);
    theAntiNeutronInelastic.RegisterMe(theHEAntiNeutronModel);
    pManager->AddDiscreteProcess(&theAntiNeutronInelastic);

    pManager->AddRestProcess(&theAntiNeutronAnnihilation);

    // PionPlus
    G4ParticleDefinition* pion = G4PionPlus::PionPlusDefinition();
    pManager = pion ->GetProcessManager();
    // add process
    pManager->AddDiscreteProcess(&theElasticProcess);

    theLEPionPlusModel = new G4LEPionPlusInelastic();
    thePionPlusInelastic.RegisterMe(theLEPionPlusModel);
    thePionPlusInelastic.RegisterMe(theTheoModel);
    pManager->AddDiscreteProcess(&thePionPlusInelastic);

    pManager->AddProcess(&thePionPlusIonisation, ordInActive,2, 2);

    pManager->AddProcess(&thePionPlusMult);
    pManager->SetProcessOrdering(&thePionPlusMult, idxAlongStep, 1);
    pManager->SetProcessOrdering(&thePionPlusMult, idxPostStep, 1);

    // PionMinus
    G4ParticleDefinition* pionMinus = G4PionMinus::PionMinusDefinition();
    pManager = pionMinus -> GetProcessManager();
    // add process
    pManager->AddDiscreteProcess(&theElasticProcess);

    theLEPionMinusModel = new G4LEPionMinusInelastic();
    thePionMinusInelastic.RegisterMe(theLEPionMinusModel);
    thePionMinusInelastic.RegisterMe(theTheoModel);
    pManager->AddDiscreteProcess(&thePionMinusInelastic);

    pManager->AddProcess(&thePionMinusIonisation, ordInActive,2, 2);

    pManager->AddProcess(&thePionMinusMult);
    pManager->SetProcessOrdering(&thePionMinusMult, idxAlongStep, 1);
    pManager->SetProcessOrdering(&thePionMinusMult, idxPostStep, 1);

    pManager->AddRestProcess(&thePionMinusAbsorption, ordDefault);

    // KaonPlus
    G4ParticleDefinition* kaonPlus = G4KaonPlus::KaonPlusDefinition();
    pManager = kaonPlus->GetProcessManager();
    // add process
    pManager->AddDiscreteProcess(&theElasticProcess);

    theLEKaonPlusModel = new G4LEKaonPlusInelastic();
    theHEKaonPlusModel = new G4HEKaonPlusInelastic();
    theKaonPlusInelastic.RegisterMe(theLEKaonPlusModel);
    theKaonPlusInelastic.RegisterMe(theTheoModel);
    pManager->AddDiscreteProcess(&theKaonPlusInelastic);

    pManager->AddProcess(&theKaonPlusIonisation, ordInActive,2, 2);

    pManager->AddProcess(&theKaonPlusMult);
    pManager->SetProcessOrdering(&theKaonPlusMult, idxAlongStep, 1);
    pManager->SetProcessOrdering(&theKaonPlusMult, idxPostStep, 1);

    // KaonMinus
    G4ParticleDefinition* kaonMinus = G4KaonMinus::KaonMinusDefinition();
    pManager = kaonMinus->GetProcessManager();
    // add process
    pManager->AddDiscreteProcess(&theElasticProcess);

    theLEKaonMinusModel = new G4LEKaonMinusInelastic();
    theHEKaonMinusModel = new G4HEKaonMinusInelastic();
    theKaonMinusInelastic.RegisterMe(theLEKaonMinusModel);
    theKaonMinusInelastic.RegisterMe(theHEKaonMinusModel);
    pManager->AddDiscreteProcess(&theKaonMinusInelastic);

    pManager->AddProcess(&theKaonMinusIonisation, ordInActive,2, 2);

    pManager->AddProcess(&theKaonMinusMult);
    pManager->SetProcessOrdering(&theKaonMinusMult, idxAlongStep, 1);
    pManager->SetProcessOrdering(&theKaonMinusMult, idxPostStep, 1);

    pManager->AddRestProcess(&theKaonMinusAbsorption, ordDefault);

    // KaonZeroL
    pManager = G4KaonZeroLong::KaonZeroLong()->GetProcessManager();
    // add process
    pManager->AddDiscreteProcess(&theElasticProcess);

    theLEKaonZeroLModel = new G4LEKaonZeroLInelastic();
    theHEKaonZeroLModel = new G4HEKaonZeroInelastic();
    theKaonZeroLInelastic.RegisterMe(theLEKaonZeroLModel);
    theKaonZeroLInelastic.RegisterMe(theHEKaonZeroLModel);
    pManager->AddDiscreteProcess(&theKaonZeroLInelastic);

    // KaonZeroS
    pManager = G4KaonZeroShort::KaonZeroShort()->GetProcessManager();
    // add process
    pManager->AddDiscreteProcess(&theElasticProcess);

    theLEKaonZeroSModel = new G4LEKaonZeroSInelastic();
    theHEKaonZeroSModel = new G4HEKaonZeroInelastic();
    theKaonZeroSInelastic.RegisterMe(theLEKaonZeroSModel);
    theKaonZeroSInelastic.RegisterMe(theHEKaonZeroSModel);
    pManager->AddDiscreteProcess(&theKaonZeroSInelastic);

    // Lambda
    pManager = G4Lambda::Lambda()->GetProcessManager();
    // add process
    pManager->AddDiscreteProcess(&theElasticProcess);

    theLELambdaModel = new G4LELambdaInelastic();
    theHELambdaModel = new G4HELambdaInelastic();
    theLambdaInelastic.RegisterMe(theLELambdaModel);
    theLambdaInelastic.RegisterMe(theHELambdaModel);
    pManager->AddDiscreteProcess(&theLambdaInelastic);

    // AntiLambda
    pManager = G4AntiLambda::AntiLambda()->GetProcessManager();
    // add process
    pManager->AddDiscreteProcess(&theElasticProcess);

    theLEAntiLambdaModel = new G4LEAntiLambdaInelastic();
    theHEAntiLambdaModel = new G4HEAntiLambdaInelastic();
    theAntiLambdaInelastic.RegisterMe(theLEAntiLambdaModel);
    theAntiLambdaInelastic.RegisterMe(theHEAntiLambdaModel);
    pManager->AddDiscreteProcess(&theAntiLambdaInelastic);

    // SigmaMinus
    pManager = G4SigmaMinus::SigmaMinus()->GetProcessManager();
    // add process
    pManager->AddDiscreteProcess(&theElasticProcess);

    theLESigmaMinusModel = new G4LESigmaMinusInelastic();
    theHESigmaMinusModel = new G4HESigmaMinusInelastic();
    theSigmaMinusInelastic.RegisterMe(theLESigmaMinusModel);
    theSigmaMinusInelastic.RegisterMe(theHESigmaMinusModel);
    pManager->AddDiscreteProcess(&theSigmaMinusInelastic);

    pManager->AddProcess(&theSigmaMinusIonisation, ordInActive,2, 2);

    pManager->AddProcess(&theSigmaMinusMult);
    pManager->SetProcessOrdering(&theSigmaMinusMult, idxAlongStep, 1);
    pManager->SetProcessOrdering(&theSigmaMinusMult, idxPostStep, 1);

    // anti-SigmaMinus
    pManager = G4AntiSigmaMinus::AntiSigmaMinus()->GetProcessManager();
    // add process
    pManager->AddDiscreteProcess(&theElasticProcess);

    theLEAntiSigmaMinusModel = new G4LEAntiSigmaMinusInelastic();
    theHEAntiSigmaMinusModel = new G4HEAntiSigmaMinusInelastic();
    theAntiSigmaMinusInelastic.RegisterMe(theLEAntiSigmaMinusModel);
    theAntiSigmaMinusInelastic.RegisterMe(theHEAntiSigmaMinusModel);
    pManager->AddDiscreteProcess(&theAntiSigmaMinusInelastic);

    pManager->AddProcess(&theAntiSigmaMinusIonisation, ordInActive,2, 2);

    pManager->AddProcess(&theAntiSigmaMinusMult);
    pManager->SetProcessOrdering(&theAntiSigmaMinusMult, idxAlongStep, 1);
    pManager->SetProcessOrdering(&theAntiSigmaMinusMult, idxPostStep, 1);

    // SigmaPlus
    pManager = G4SigmaPlus::SigmaPlus()->GetProcessManager();
    // add process
    pManager->AddDiscreteProcess(&theElasticProcess);

    theLESigmaPlusModel = new G4LESigmaPlusInelastic();
    theHESigmaPlusModel = new G4HESigmaPlusInelastic();
    theSigmaPlusInelastic.RegisterMe(theLESigmaPlusModel);
    theSigmaPlusInelastic.RegisterMe(theHESigmaPlusModel);
    pManager->AddDiscreteProcess(&theSigmaPlusInelastic);

    pManager->AddProcess(&theSigmaPlusIonisation, ordInActive,2, 2);

    pManager->AddProcess(&theSigmaPlusMult);
    pManager->SetProcessOrdering(&theSigmaPlusMult, idxAlongStep, 1);
    pManager->SetProcessOrdering(&theSigmaPlusMult, idxPostStep, 1);

    // anti-SigmaPlus
    pManager = G4AntiSigmaPlus::AntiSigmaPlus()->GetProcessManager();
    // add process
    pManager->AddDiscreteProcess(&theElasticProcess);

    theLEAntiSigmaPlusModel = new G4LEAntiSigmaPlusInelastic();
    theHEAntiSigmaPlusModel = new G4HEAntiSigmaPlusInelastic();
    theAntiSigmaPlusInelastic.RegisterMe(theLEAntiSigmaPlusModel);
    theAntiSigmaPlusInelastic.RegisterMe(theHEAntiSigmaPlusModel);
    pManager->AddDiscreteProcess(&theAntiSigmaPlusInelastic);

    pManager->AddProcess(&theAntiSigmaPlusIonisation, ordInActive,2, 2);

    pManager->AddProcess(&theAntiSigmaPlusMult);
    pManager->SetProcessOrdering(&theAntiSigmaPlusMult, idxAlongStep, 1);
    pManager->SetProcessOrdering(&theAntiSigmaPlusMult, idxPostStep, 1);

    // XiMinus
    pManager = G4XiMinus::XiMinus()->GetProcessManager();
    // add process
    pManager->AddDiscreteProcess(&theElasticProcess);

    theLEXiMinusModel = new G4LEXiMinusInelastic();
    theHEXiMinusModel = new G4HEXiMinusInelastic();
    theXiMinusInelastic.RegisterMe(theLEXiMinusModel);
    theXiMinusInelastic.RegisterMe(theHEXiMinusModel);
    pManager->AddDiscreteProcess(&theXiMinusInelastic);

    pManager->AddProcess(&theXiMinusIonisation, ordInActive,2, 2);

    pManager->AddProcess(&theXiMinusMult);
    pManager->SetProcessOrdering(&theXiMinusMult, idxAlongStep, 1);
    pManager->SetProcessOrdering(&theXiMinusMult, idxPostStep, 1);

    // anti-XiMinus
    pManager = G4AntiXiMinus::AntiXiMinus()->GetProcessManager();
    // add process
    pManager->AddDiscreteProcess(&theElasticProcess);

    theLEAntiXiMinusModel = new G4LEAntiXiMinusInelastic();
    theHEAntiXiMinusModel = new G4HEAntiXiMinusInelastic();
    theAntiXiMinusInelastic.RegisterMe(theLEAntiXiMinusModel);
    theAntiXiMinusInelastic.RegisterMe(theHEAntiXiMinusModel);
    pManager->AddDiscreteProcess(&theAntiXiMinusInelastic);

    pManager->AddProcess(&theAntiXiMinusIonisation, ordInActive,2, 2);

    pManager->AddProcess(&theAntiXiMinusMult);
    pManager->SetProcessOrdering(&theAntiXiMinusMult, idxAlongStep, 1);
    pManager->SetProcessOrdering(&theAntiXiMinusMult, idxPostStep, 1);

    // XiZero
    pManager = G4XiZero::XiZero()->GetProcessManager();
    // add process
    pManager->AddDiscreteProcess(&theElasticProcess);

    theLEXiZeroModel = new G4LEXiZeroInelastic();
    theHEXiZeroModel = new G4HEXiZeroInelastic();
    theXiZeroInelastic.RegisterMe(theLEXiZeroModel);
    theXiZeroInelastic.RegisterMe(theHEXiZeroModel);
    pManager->AddDiscreteProcess(&theXiZeroInelastic);

    // anti-XiZero
    pManager = G4AntiXiZero::AntiXiZero()->GetProcessManager();
    // add process
    pManager->AddDiscreteProcess(&theElasticProcess);

    theLEAntiXiZeroModel = new G4LEAntiXiZeroInelastic();
    theHEAntiXiZeroModel = new G4HEAntiXiZeroInelastic();
    theAntiXiZeroInelastic.RegisterMe(theLEAntiXiZeroModel);
    theAntiXiZeroInelastic.RegisterMe(theHEAntiXiZeroModel);
    pManager->AddDiscreteProcess(&theAntiXiZeroInelastic);

    // OmegaMinus
    pManager = G4OmegaMinus::OmegaMinus()->GetProcessManager();
    // add process
    pManager->AddDiscreteProcess(&theElasticProcess);

    theLEOmegaMinusModel = new G4LEOmegaMinusInelastic();
    theHEOmegaMinusModel = new G4HEOmegaMinusInelastic();
    theOmegaMinusInelastic.RegisterMe(theLEOmegaMinusModel);
    theOmegaMinusInelastic.RegisterMe(theHEOmegaMinusModel);
    pManager->AddDiscreteProcess(&theOmegaMinusInelastic);

    pManager->AddProcess(&theOmegaMinusIonisation, ordInActive,2, 2);

    pManager->AddProcess(&theOmegaMinusMult);
    pManager->SetProcessOrdering(&theOmegaMinusMult, idxAlongStep, 1);
    pManager->SetProcessOrdering(&theOmegaMinusMult, idxPostStep, 1);

    // anti-OmegaMinus
    pManager = G4AntiOmegaMinus::AntiOmegaMinus()->GetProcessManager();
    // add process
    pManager->AddDiscreteProcess(&theElasticProcess);

    theLEAntiOmegaMinusModel = new G4LEAntiOmegaMinusInelastic();
    theHEAntiOmegaMinusModel = new G4HEAntiOmegaMinusInelastic();
    theAntiOmegaMinusInelastic.RegisterMe(theLEAntiOmegaMinusModel);
    theAntiOmegaMinusInelastic.RegisterMe(theHEAntiOmegaMinusModel);
    pManager->AddDiscreteProcess(&theAntiOmegaMinusInelastic);

    pManager->AddProcess(&theAntiOmegaMinusIonisation, ordInActive,2, 2);

    pManager->AddProcess(&theAntiOmegaMinusMult);
    pManager->SetProcessOrdering(&theAntiOmegaMinusMult, idxAlongStep, 1);
    pManager->SetProcessOrdering(&theAntiOmegaMinusMult, idxPostStep, 1);

}

#endif