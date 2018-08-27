//
////////////////////////////////////////////////////////////////
//                                                            //
//  Title:  Space Sciences Physics List                       //
//  Date:   20 March 2005                                     //
//  Author: D.H. Wright (SLAC)                                //
//                                                            //
////////////////////////////////////////////////////////////////
//

#ifdef GEANT4_9_1

#include "G4VModularPhysicsList.hh"


class SSPhysicsList: public G4VModularPhysicsList
{
public:
    SSPhysicsList();
    virtual ~SSPhysicsList();

    virtual void SetCuts();
};

#endif



