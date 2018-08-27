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

#include "G4VPhysicsConstructor.hh"
#include "globals.hh"

#include "G4HadronElasticProcess.hh"

#include "G4ProtonInelasticProcess.hh"
#include "G4NeutronInelasticProcess.hh"
#include "G4DeuteronInelasticProcess.hh"
#include "G4TritonInelasticProcess.hh"
#include "G4AlphaInelasticProcess.hh"
#include "G4IonInelasticProcess.hh"
#include "G4HadronFissionProcess.hh"
#include "G4HadronCaptureProcess.hh"


// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// taken from GammaRayTelescope !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

// processes
#ifdef GEANT4_9_1
#include "G4MultipleScattering.hh"
#endif
#ifdef GEANT4_9_4
#include "G4eMultipleScattering.hh"
#include "G4hMultipleScattering.hh"
#endif

#include "G4hIonisation.hh"

#include "G4AntiProtonInelasticProcess.hh"
#include "G4AntiNeutronInelasticProcess.hh"
#include "G4PionPlusInelasticProcess.hh"
#include "G4PionMinusInelasticProcess.hh"
#include "G4KaonPlusInelasticProcess.hh"
#include "G4KaonZeroSInelasticProcess.hh"
#include "G4KaonZeroLInelasticProcess.hh"
#include "G4KaonMinusInelasticProcess.hh"
#include "G4LambdaInelasticProcess.hh"
#include "G4AntiLambdaInelasticProcess.hh"
#include "G4SigmaPlusInelasticProcess.hh"
#include "G4SigmaMinusInelasticProcess.hh"
#include "G4AntiSigmaPlusInelasticProcess.hh"
#include "G4AntiSigmaMinusInelasticProcess.hh"
#include "G4XiZeroInelasticProcess.hh"
#include "G4XiMinusInelasticProcess.hh"
#include "G4AntiXiZeroInelasticProcess.hh"
#include "G4AntiXiMinusInelasticProcess.hh"
#include "G4DeuteronInelasticProcess.hh"
#include "G4TritonInelasticProcess.hh"
#include "G4AlphaInelasticProcess.hh"
#include "G4OmegaMinusInelasticProcess.hh"
#include "G4AntiOmegaMinusInelasticProcess.hh"

// models
#include "G4LEAntiProtonInelastic.hh"
#include "G4LEAntiNeutronInelastic.hh"
#include "G4LEPionPlusInelastic.hh"
#include "G4LEPionMinusInelastic.hh"
#include "G4LEKaonPlusInelastic.hh"
#include "G4LEKaonZeroSInelastic.hh"
#include "G4LEKaonZeroLInelastic.hh"
#include "G4LEKaonMinusInelastic.hh"
#include "G4LELambdaInelastic.hh"
#include "G4LEAntiLambdaInelastic.hh"
#include "G4LESigmaPlusInelastic.hh"
#include "G4LESigmaMinusInelastic.hh"
#include "G4LEAntiSigmaPlusInelastic.hh"
#include "G4LEAntiSigmaMinusInelastic.hh"
#include "G4LEXiZeroInelastic.hh"
#include "G4LEXiMinusInelastic.hh"
#include "G4LEAntiXiZeroInelastic.hh"
#include "G4LEAntiXiMinusInelastic.hh"
#include "G4LEDeuteronInelastic.hh"
#include "G4LETritonInelastic.hh"
#include "G4LEAlphaInelastic.hh"
#include "G4LEOmegaMinusInelastic.hh"
#include "G4LEAntiOmegaMinusInelastic.hh"

#include "G4HEAntiProtonInelastic.hh"
#include "G4HEAntiNeutronInelastic.hh"
#include "G4HEPionPlusInelastic.hh"
#include "G4HEPionMinusInelastic.hh"
#include "G4HEKaonPlusInelastic.hh"
#include "G4HEKaonZeroInelastic.hh"
#include "G4HEKaonZeroInelastic.hh"
#include "G4HEKaonMinusInelastic.hh"
#include "G4HELambdaInelastic.hh"
#include "G4HEAntiLambdaInelastic.hh"
#include "G4HESigmaPlusInelastic.hh"
#include "G4HESigmaMinusInelastic.hh"
#include "G4HEAntiSigmaPlusInelastic.hh"
#include "G4HEAntiSigmaMinusInelastic.hh"
#include "G4HEXiZeroInelastic.hh"
#include "G4HEXiMinusInelastic.hh"
#include "G4HEAntiXiZeroInelastic.hh"
#include "G4HEAntiXiMinusInelastic.hh"
#include "G4HEOmegaMinusInelastic.hh"
#include "G4HEAntiOmegaMinusInelastic.hh"

// Stopping processes
#include "G4AntiProtonAnnihilationAtRest.hh"
#include "G4AntiNeutronAnnihilationAtRest.hh"

#ifdef TRIUMF_STOP_PIMINUS
#include "G4PionMinusAbsorptionAtRest.hh"
#else
#include "G4PiMinusAbsorptionAtRest.hh"
#endif
#ifdef TRIUMF_STOP_KMINUS
#include "G4KaonMinusAbsorption.hh"
#else
#include "G4KaonMinusAbsorptionAtRest.hh"
#endif


class CPhysListAllHadron : public G4VPhysicsConstructor
{
public:
    CPhysListAllHadron(int version, const G4String& name = "hadron");
    virtual ~CPhysListAllHadron();

public:
    // Construct particle and physics
    void ConstructParticle() {};
    //
    void ConstructProcess();

private:

    int version;

    G4HadronElasticProcess  theElasticProcess;
    G4ProtonInelasticProcess theProtonInelastic;
    G4NeutronInelasticProcess  theNeutronInelastic;
    G4HadronElasticProcess* theNeutronElasticProcess;
    G4HadronFissionProcess* theFissionProcess;
    G4HadronCaptureProcess* theCaptureProcess;
    G4DeuteronInelasticProcess* theDeuteronInelasticProcess;
    G4TritonInelasticProcess* theTritonInelasticProcess;
    G4AlphaInelasticProcess* theAlphaInelasticProcess;
    G4IonInelasticProcess* theIonInelasticProcess;


    // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    // taken from GammaRayTelescope !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

    // proton
#ifdef GEANT4_9_1
    G4MultipleScattering theProtonMult;
#endif
#ifdef GEANT4_9_4
    G4hMultipleScattering theProtonMult;
#endif

    G4hIonisation theProtonIonisation;

    // anti-proton
    G4AntiProtonInelasticProcess theAntiProtonInelastic;
    G4LEAntiProtonInelastic* theLEAntiProtonModel;
    G4HEAntiProtonInelastic* theHEAntiProtonModel;


#ifdef GEANT4_9_1
    G4MultipleScattering theAntiProtonMult;
#endif
#ifdef GEANT4_9_4
    G4hMultipleScattering theAntiProtonMult;
#endif

    G4hIonisation theAntiProtonIonisation;
    G4AntiProtonAnnihilationAtRest  theAntiProtonAnnihilation;

    // anti-neutron
    G4AntiNeutronInelasticProcess  theAntiNeutronInelastic;
    G4LEAntiNeutronInelastic* theLEAntiNeutronModel;
    G4HEAntiNeutronInelastic* theHEAntiNeutronModel;
    G4AntiNeutronAnnihilationAtRest  theAntiNeutronAnnihilation;

    // Pi +
    G4PionPlusInelasticProcess thePionPlusInelastic;
    G4LEPionPlusInelastic* theLEPionPlusModel;

#ifdef GEANT4_9_1
    G4MultipleScattering thePionPlusMult;
    G4MultipleScattering thePionMinusMult;
#endif
#ifdef GEANT4_9_4
    G4hMultipleScattering thePionPlusMult;
    G4hMultipleScattering thePionMinusMult;
#endif
    G4hIonisation thePionPlusIonisation;

    // Pi -
    G4PionMinusInelasticProcess thePionMinusInelastic;
    G4LEPionMinusInelastic* theLEPionMinusModel;

    G4hIonisation thePionMinusIonisation;
#ifdef TRIUMF_STOP_PIMINUS
    G4PionMinusAbsorptionAtRest thePionMinusAbsorption;
#else
    G4PiMinusAbsorptionAtRest thePionMinusAbsorption;
#endif

    // K +
    G4KaonPlusInelasticProcess theKaonPlusInelastic;
    G4LEKaonPlusInelastic* theLEKaonPlusModel;
    G4HEKaonPlusInelastic* theHEKaonPlusModel;

#ifdef GEANT4_9_1
    G4MultipleScattering theKaonPlusMult;
#endif
#ifdef GEANT4_9_4
    G4hMultipleScattering theKaonPlusMult;
#endif
    G4hIonisation theKaonPlusIonisation;

    // K -
    G4KaonMinusInelasticProcess theKaonMinusInelastic;
    G4LEKaonMinusInelastic* theLEKaonMinusModel;
    G4HEKaonMinusInelastic* theHEKaonMinusModel;

#ifdef GEANT4_9_1
    G4MultipleScattering theKaonMinusMult;
#endif
#ifdef GEANT4_9_4
    G4hMultipleScattering theKaonMinusMult;
#endif

    G4hIonisation theKaonMinusIonisation;
#ifdef TRIUMF_STOP_KMINUS
    G4KaonMinusAbsorption theKaonMinusAbsorption;
#else
    G4PiMinusAbsorptionAtRest theKaonMinusAbsorption;
#endif

    // K0L
    G4KaonZeroLInelasticProcess theKaonZeroLInelastic;
    G4LEKaonZeroLInelastic* theLEKaonZeroLModel;
    G4HEKaonZeroInelastic* theHEKaonZeroLModel;

    // K0S
    G4KaonZeroSInelasticProcess theKaonZeroSInelastic;
    G4LEKaonZeroSInelastic* theLEKaonZeroSModel;
    G4HEKaonZeroInelastic* theHEKaonZeroSModel;


    // Lambda
    G4LambdaInelasticProcess  theLambdaInelastic;
    G4LELambdaInelastic*  theLELambdaModel;
    G4HELambdaInelastic*  theHELambdaModel;

    // AntiLambda
    G4AntiLambdaInelasticProcess  theAntiLambdaInelastic;
    G4LEAntiLambdaInelastic*  theLEAntiLambdaModel;
    G4HEAntiLambdaInelastic*  theHEAntiLambdaModel;

    // SigmaMinus
    G4SigmaMinusInelasticProcess  theSigmaMinusInelastic;
    G4LESigmaMinusInelastic*  theLESigmaMinusModel;
    G4HESigmaMinusInelastic*  theHESigmaMinusModel;

#ifdef GEANT4_9_1
    G4MultipleScattering theSigmaMinusMult;
#endif
#ifdef GEANT4_9_4
    G4hMultipleScattering theSigmaMinusMult;
#endif
    G4hIonisation theSigmaMinusIonisation;

    // AntiSigmaMinus
    G4AntiSigmaMinusInelasticProcess  theAntiSigmaMinusInelastic;
    G4LEAntiSigmaMinusInelastic*  theLEAntiSigmaMinusModel;
    G4HEAntiSigmaMinusInelastic*  theHEAntiSigmaMinusModel;

#ifdef GEANT4_9_1
    G4MultipleScattering theAntiSigmaMinusMult;
#endif
#ifdef GEANT4_9_4
    G4hMultipleScattering theAntiSigmaMinusMult;
#endif

    G4hIonisation theAntiSigmaMinusIonisation;

    // SigmaPlus
    G4SigmaPlusInelasticProcess  theSigmaPlusInelastic;
    G4LESigmaPlusInelastic*  theLESigmaPlusModel;
    G4HESigmaPlusInelastic*  theHESigmaPlusModel;

#ifdef GEANT4_9_1
    G4MultipleScattering theSigmaPlusMult;
#endif
#ifdef GEANT4_9_4
    G4hMultipleScattering theSigmaPlusMult;
#endif
    G4hIonisation theSigmaPlusIonisation;

    // AntiSigmaPlus
    G4AntiSigmaPlusInelasticProcess  theAntiSigmaPlusInelastic;
    G4LEAntiSigmaPlusInelastic*  theLEAntiSigmaPlusModel;
    G4HEAntiSigmaPlusInelastic*  theHEAntiSigmaPlusModel;

#ifdef GEANT4_9_1
    G4MultipleScattering theAntiSigmaPlusMult;
#endif
#ifdef GEANT4_9_4
    G4hMultipleScattering theAntiSigmaPlusMult;
#endif
    G4hIonisation theAntiSigmaPlusIonisation;

    // XiZero
    G4XiZeroInelasticProcess  theXiZeroInelastic;
    G4LEXiZeroInelastic*  theLEXiZeroModel;
    G4HEXiZeroInelastic*  theHEXiZeroModel;

    // AntiXiZero
    G4AntiXiZeroInelasticProcess  theAntiXiZeroInelastic;
    G4LEAntiXiZeroInelastic*  theLEAntiXiZeroModel;
    G4HEAntiXiZeroInelastic*  theHEAntiXiZeroModel;

    // XiMinus
    G4XiMinusInelasticProcess  theXiMinusInelastic;
    G4LEXiMinusInelastic*  theLEXiMinusModel;
    G4HEXiMinusInelastic*  theHEXiMinusModel;

#ifdef GEANT4_9_1
    G4MultipleScattering theXiMinusMult;
#endif
#ifdef GEANT4_9_4
    G4hMultipleScattering theXiMinusMult;
#endif
    G4hIonisation theXiMinusIonisation;

    // AntiXiMinus
    G4AntiXiMinusInelasticProcess  theAntiXiMinusInelastic;
    G4LEAntiXiMinusInelastic*  theLEAntiXiMinusModel;
    G4HEAntiXiMinusInelastic*  theHEAntiXiMinusModel;

#ifdef GEANT4_9_1
    G4MultipleScattering theAntiXiMinusMult;
#endif
#ifdef GEANT4_9_4
    G4hMultipleScattering theAntiXiMinusMult;
#endif
    G4hIonisation theAntiXiMinusIonisation;

    // OmegaMinus
    G4OmegaMinusInelasticProcess  theOmegaMinusInelastic;
    G4LEOmegaMinusInelastic*  theLEOmegaMinusModel;
    G4HEOmegaMinusInelastic*  theHEOmegaMinusModel;

#ifdef GEANT4_9_1
    G4MultipleScattering theOmegaMinusMult;
#endif
#ifdef GEANT4_9_4
    G4hMultipleScattering theOmegaMinusMult;
#endif
    G4hIonisation theOmegaMinusIonisation;

    // AntiOmegaMinus
    G4AntiOmegaMinusInelasticProcess  theAntiOmegaMinusInelastic;
    G4LEAntiOmegaMinusInelastic*  theLEAntiOmegaMinusModel;
    G4HEAntiOmegaMinusInelastic*  theHEAntiOmegaMinusModel;

#ifdef GEANT4_9_1
    G4MultipleScattering theAntiOmegaMinusMult;
#endif
#ifdef GEANT4_9_4
    G4hMultipleScattering theAntiOmegaMinusMult;
#endif
    G4hIonisation theAntiOmegaMinusIonisation;

};

#endif





