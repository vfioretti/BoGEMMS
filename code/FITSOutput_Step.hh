/***************************************************************************
                          FITSOutput_Step.hh  -  description
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

#ifndef FITSOUTPUT_STEP_H
#define FITSOUTPUT_STEP_H

#include "FITSOutputFile.hh"
#include "G4ios.hh"
#include "G4String.hh"

class CHit;

class FITSOutput_Step : public FITSOutputFileBinaryTable {
public:
    FITSOutput_Step();

    virtual ~FITSOutput_Step();

    virtual bool init();

    virtual void writeData(CHit* hit, G4int i);

protected:

    long lastevent;

    bool newfile;

    long neventsinfile;

    G4long current_event_id;

};

#endif
