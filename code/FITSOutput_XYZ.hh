/***************************************************************************
                          FITSOutput_XYZ.hh  -  description
                             -------------------
    begin                : 2012
    copyright            : (C) 2012 by  Valentina Fioretti
    email                : fioretti@iasfbo.inaf.it
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software for non commercial purpose              *
 *   and for public research institutes; you can redistribute it and/or    *
 *   modify it under the terms of the GNU General Public License.          *
 *   For commercial purpose see appropriate license terms                  *
 *                                                                         *
 ***************************************************************************/

#ifndef FITSOUTPUT_XYZ_H
#define FITSOUTPUT_XYZ_H

#include "FITSOutputFile.hh"
#include "CXYZHit.hh"

class CXYZSD;

class FITSOutput_XYZ : public FITSOutputFileBinaryTable {
public:
    FITSOutput_XYZ();

    virtual ~FITSOutput_XYZ();

    virtual bool init();

    // 		virtual bool close();

    virtual void writeData(CXYZHit* hitting, G4int i);

public:


protected:
	
	bool writeonlyifenergydeposit;
	bool removeWorldEvent;
	
    long lastevent;

    bool newfile;

    char **ttype1_template;
    char **tform1_template;
    char **tunit1_template;
    int* pfind1_template;
    char ** tycom1_template;
    bool* colconfig;


    long neventsinfile;
    //CXYZHit* initial_step;

    G4long current_event_id;
    G4int volselectionstart[1024];
    G4int volselectionend[1024];
    G4int volselectionN;

    G4int howmanylayers;
    G4int trayStart;
    G4int trayEnd;
    G4int layernumber[1024];

};

#endif
