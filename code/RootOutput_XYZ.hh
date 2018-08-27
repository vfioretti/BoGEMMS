/***************************************************************************
                          RootOutput_XYZ.hh  -  description
                             -------------------
    begin                : 2014
    copyright            : (C) 2014 by  Andrea Bulgarelli
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

#ifndef WITHOUTROOT

#ifndef RootOUTPUT_XYZ_H
#define RootOUTPUT_XYZ_H

#include <TTree.h>
#include <TXNetFile.h>
#include <TFile.h>
#include "CXYZHit.hh"

class RootOutput_XYZ  {

public:

    RootOutput_XYZ();

    virtual ~RootOutput_XYZ();

    virtual bool init();

    virtual bool close();

    virtual void writeData(CXYZHit* hitting, G4int i);

public:



protected:
	

    long lastevent;

    bool newfile;



    G4long current_event_id;

    TXNetFile* fileoutx;
    TFile* fileout;
    TTree* treeout;

    char* filename;

    long nrows_file;
	
	//
	long evtid;
	double energystart;
	long ttrkid;
    long parenttrkid;
    long volumeid;
	long motherid;
	double energyDep;
	double x_ent;
	double y_ent;
	double z_ent;
	double x_exit;
	double y_exit;
	double z_exit;
	double ekin_ent;
	double ekin_exit;
	double gtime_ent;
	double gtime_exit;
	long tpart;
	long tproc;

	//
	//configuration
	long writeonlydetectorevents;
	bool writeonlyifenergydeposit;
	bool removeWorldEvent;
	
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

};

#endif

#endif
