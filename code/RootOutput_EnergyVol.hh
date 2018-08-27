/***************************************************************************
                          RootOutput_EnergyVol.hh  -  description
                             -------------------
    begin                : 2010
    copyright            : (C) 2010 by  Andrea Bulgarelli
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

#ifndef RootOUTPUT_ENVOL_H
#define RootOUTPUT_ENVOL_H


#include "CSteppingAction.hh"
class CSteppingAction;
#include <TTree.h>
#include <TXNetFile.h>
#include <TFile.h>
#include "IOEnergyBase.hh"

class RootOutput_EnergyVol : public IOEnergyBase {

public:

    RootOutput_EnergyVol();

    virtual ~RootOutput_EnergyVol();

    virtual bool init();

    virtual bool close();

    virtual void writeData();

public:



protected:

    long lastevent;

    bool newfile;

    long neventsinfile;

    G4long current_event_id;

    TXNetFile* fileoutx;
    TFile* fileout;
    TTree* treeout;

    char* filename;

    long nrows_file;


};

#endif

#endif
