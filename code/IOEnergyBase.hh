/***************************************************************************
		IOEnergyBase.hh  -  description
 -------------------
 begin                : 2007
 copyright            : (C) 2007 by  Andrea Bulgarelli
 email                : bulgarelli@iasfbo.inaf.it
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software for non commercial purpose              *
 *   and for public research institutes; you can redistribute it and/or    *
 *   modify it under the terms of the GNU General Public License.          *
 *   For commercial purpose see appropriate license terms                  *
 *                                                                         *
 ***************************************************************************/

#ifndef IOENERGYBASE_HH
#define IOENERGYBASE_HH

#include "CSteppingAction.hh"
class CSteppingAction;

#define NMAXVOLFOREVENT 100000
#define NMAXVOLS 1100000

typedef unsigned char byte;
typedef unsigned short ushort;

class IOEnergyBase {
public:
    IOEnergyBase();

    ~IOEnergyBase();

    virtual void addData(CSteppingAction* stepping, bool in);

    virtual void ResetData();

    virtual void ResetDataBuffer();

    virtual void CreateDataBuffer();

    virtual void CopyBuffer();

    virtual void initConf();

    virtual void setBuffer(long evtid, double energyDep, long volume, long ttrkid, long tpart, byte tproc, ushort tmultI, ushort tmultO);

    long Aindex;

protected:

    //configuration
    long writeonlydetectorevents;
	bool writeonlyifenergydeposit;
	bool removeWorldEvent;

    double energystart;

    CSteppingAction* initial_step;

    G4int volselectionstart[1024];
    G4int volselectionend[1024];
    G4int volselectionN;

    G4int volgroupID[1024];
    G4int volgroupstart[1024];
    G4int volgroupend[1024];
    G4int volgroupN;


    long nmaxevents;
    long neventsprocessed_forthisfile;
    long neventsstored_forthisfile;

    G4long* volumeID;
    G4double* energy;
    G4long* trkid;
    G4long* particle;
    byte* process;
    ushort* multI;
    ushort* multO;
    //temp variable

    long evtid;
    double energyDep;
    long volume;
    double energyStart;
    long ttrkid;
    long tpart;
    byte tproc;
    ushort tmultI;
    ushort tmultO;


    //buffer

    int* AvolumeID;
    double* Aenergy;
    double* Aenergystart;
    long* Aevent_id;
    long* Atrkid;
    long* Aparticle;
    byte* Aprocess;
    ushort* AmultI;
    ushort* AmultO;


};

#endif
