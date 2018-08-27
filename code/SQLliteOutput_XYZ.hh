/***************************************************************************
                          SQLliteOutput_XYZ.hh  -  description
                             -------------------
    begin                : 2017
    copyright            : (C) 2017 by  Andrea Bulgarelli
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

#ifdef SQLITE3

#ifndef SQLliteOUTPUT_XYZ_H
#define SQLliteOUTPUT_XYZ_H


#include "CXYZHit.hh"
#include <sqlite3.h>

class SQLliteOutput_XYZ  {

public:

    SQLliteOutput_XYZ();

    virtual ~SQLliteOutput_XYZ();

    virtual bool init();

    virtual bool close();

    virtual void writeData(CXYZHit* hitting, G4int i);

public:



protected:

	
	

    long lastevent;

    bool newfile;

    G4long current_event_id;

    sqlite3 *db;

    char* filename;

    long nrows_file; //shared
	
	//
	double energystart;
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
	

};

#endif

#endif
