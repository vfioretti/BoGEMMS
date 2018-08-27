/***************************************************************************
                          RootOutput_EnergyVolEnd.hh  -  description
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

#ifndef ROOTOUTPUT_ENVOLEND_H
#define ROOTOUTPUT_ENVOLEND_H


#include <TTree.h>
#include <TFile.h>
#include <TXNetFile.h>
#include "IOEnergyBase.hh"

class RootOutput_EnergyVolEnd : public IOEnergyBase {
public:
    RootOutput_EnergyVolEnd();

    virtual ~RootOutput_EnergyVolEnd();

    virtual bool init();

    virtual bool close();

    virtual void writeData();


public:



protected:

    long lastevent;

    bool newfile;


    G4long current_event_id;


    TXNetFile* fileoutx;
    TFile* fileout;
    TTree* treeout;
    TTree* treeeve;
    int neve;

    char* filename;

};

#endif

#endif
