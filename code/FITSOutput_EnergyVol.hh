/***************************************************************************
                          FITSOutput_EnergyVol.hh  -  description
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

#ifndef FITSOUTPUT_ENVOL_H
#define FITSOUTPUT_ENVOL_H

#include "FITSOutputFile.hh"
#include "IOEnergyBase.hh"

class FITSOutput_EnergyVol : public FITSOutputFileBinaryTable, public IOEnergyBase {
public:
    FITSOutput_EnergyVol();

    virtual ~FITSOutput_EnergyVol();

    virtual bool init();

    virtual void writeData();


public:

    long neventsinfile;

protected:

    long lastevent;

    bool newfile;

    char **ttype1_template;
    char **tform1_template;
    char **tunit1_template;
    int* pfind1_template;
    char ** tycom1_template;
    bool* colconfig;






    G4long current_event_id;




};

#endif
