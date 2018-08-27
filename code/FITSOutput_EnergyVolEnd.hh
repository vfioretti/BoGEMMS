/***************************************************************************
                          FITSOutput_EnergyVolEnd.hh  -  description
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

#ifndef FITSOUTPUT_ENVOLEND_H
#define FITSOUTPUT_ENVOLEND_H

#include "FITSOutput_EnergyVol.hh"
#include "CSteppingAction.hh"


class FITSOutput_EnergyVolEnd : public FITSOutput_EnergyVol {
public:
    FITSOutput_EnergyVolEnd();

    virtual ~FITSOutput_EnergyVolEnd();

    virtual bool init();

    virtual void writeData();

    virtual bool close();

public:

    int compress;

protected:






};

#endif
