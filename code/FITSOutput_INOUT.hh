/***************************************************************************
                          FITSOutput_INOUT.hh  -  description
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

#ifndef FITSOUTPUT_INOUT_H
#define FITSOUTPUT_INOUT_H

#include "FITSOutputFile.hh"
#include "CSteppingAction.hh"
class CSteppingAction;

class FITSOutput_INOUT : public FITSOutputFileBinaryTable {
public:
    FITSOutput_INOUT();

    virtual ~FITSOutput_INOUT();

    virtual bool init();

    // 		virtual bool close();

    virtual void writeData(CSteppingAction* stepping, bool writeInAnyCase = false);

public:
    //1) write position
    bool writePosition;
    //2) write momentum
    bool writeMomentum;
    //3) write momentum direction
    bool writeMomentumDirection;
    //4) write element ID
    bool writeElementID;
    //5) write element name
    bool writeElementName;
    //6) don't write input event
    bool writeInputEvents;
    //7) don't write output event
    bool writeOutputEvents;


protected:

    long lastevent;

    bool newfile;

    char **ttype1_template;
    char **tform1_template;
    char **tunit1_template;
    int* pfind1_template;
    char ** tycom1_template;
    bool* colconfig;

    bool writeIN;
    bool writeOUT;

    long neventsinfile;
    bool initial_condition_written;
    bool writeInitialCondition;
    CSteppingAction* initial_step;

    G4long current_event_id;

    G4int volselectionstart[1024];
    G4int volselectionend[1024];
    G4int volselectionN;

};

#endif
