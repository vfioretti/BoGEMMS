//
/////////////////////////////////////////////////////////////////////////////
//                                                                         //
//  Title:  Space Sciences Physics List                                    //
//  Date:   20 March 2005                                                  //
//  Author: D.H. Wright (SLAC)                                             //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////
//

#ifdef GEANT4_9_1

#include "SSPhysicsList.hh"

#include "SSDecayPhysics.hh"
#include "SSBosonPhysics.hh"
#include "SSLeptonPhysics.hh"
#include "SSNeutronPhysics.hh"
#include "SSHadronPhysics.hh"
#include "SSIonPhysics.hh"

SSPhysicsList::SSPhysicsList():  G4VModularPhysicsList()
{
    // default cut value  (1.0mm)
    defaultCutValue = 1.0*mm;
    // SetVerboseLevel(1);

    // Particle decays
    RegisterPhysics( new SSDecayPhysics("decay"));

    // Bosons (gamma + geantinos)
    RegisterPhysics( new SSBosonPhysics("boson"));

    // Leptons
    RegisterPhysics( new SSLeptonPhysics("lepton"));

    // Neutron Physics
    RegisterPhysics( new SSNeutronPhysics("neutron"));

    // Hadron Physics
    RegisterPhysics( new SSHadronPhysics("hadron"));

    // Ion Physics
    RegisterPhysics( new SSIonPhysics("ion"));
}

SSPhysicsList::~SSPhysicsList()
{
    ;
}

void SSPhysicsList::SetCuts()
{
    // Use default cut values gamma and e processes
    SetCutsWithDefault();
}

#endif
