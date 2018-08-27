//
////////////////////////////////////////////////////////////////
//                                                            //
//  Title:  Decay physics for Space Sciences Physics List     //
//  Date:   20 March 2005                                     //
//  Author: D.H. Wright (SLAC)                                //
//                                                            //
////////////////////////////////////////////////////////////////
//

#ifdef GEANT4_9_1

#include "SSDecayPhysics.hh"
#include "G4ProcessManager.hh"


SSDecayPhysics::SSDecayPhysics(const G4String& name)
    :  G4VPhysicsConstructor(name)
{
    ;
}

SSDecayPhysics::~SSDecayPhysics()
{
    ;
}

void SSDecayPhysics::ConstructParticle()
{
    ;
}

void SSDecayPhysics::ConstructProcess()
{
    // Add Decay Process
    theParticleIterator->reset();
    while( (*theParticleIterator)() ) {
        G4ParticleDefinition* particle = theParticleIterator->value();
        G4ProcessManager* pmanager = particle->GetProcessManager();
        if (fDecayProcess.IsApplicable(*particle)) {
            pmanager ->AddProcess(&fDecayProcess);
            // set ordering for PostStepDoIt and AtRestDoIt
            pmanager ->SetProcessOrdering(&fDecayProcess, idxPostStep);
            pmanager ->SetProcessOrdering(&fDecayProcess, idxAtRest);
        }
    }
}

#endif

