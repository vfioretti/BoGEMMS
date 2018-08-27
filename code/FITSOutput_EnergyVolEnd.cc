/***************************************************************************
                          FITSOutput_EnergyVolEnd.cc  -  description
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

#include "FITSOutput_EnergyVolEnd.hh"

#include "CSteppingAction.hh"
#include "THELGlobalMemory.hh"
#include "ConfigFile.hh"



//#define ADDCOLSTXT

//possibile parametri di formattazione del file EnergyVol
//1) write position
//2) write momentum
//3) write momentum direction
//4) write element ID
//5) write element name
//6) don't write input event
//7) don't write output event
FITSOutput_EnergyVolEnd::FITSOutput_EnergyVolEnd() : FITSOutput_EnergyVol() {
    CreateDataBuffer();
    compress = -9;
}


FITSOutput_EnergyVolEnd::~FITSOutput_EnergyVolEnd() {
    cout << "FITSOutput_EnergyVolEnd::~FITSOutput_EnergyVolEnd()" << endl;
}



bool FITSOutput_EnergyVolEnd::init() {
    FITSOutput_EnergyVol::init();
//	open();
    ResetDataBuffer();
    return true;
}


void FITSOutput_EnergyVolEnd::writeData() {

    CopyBuffer();
    ResetData();
    if(Aindex > nmaxevents) {
        close();
        init();
// 		open();
// 		ResetDataBuffer();
    }

}



bool FITSOutput_EnergyVolEnd::close() {

    bool write = true;
    long firstelem;
    int status;
    status=0;
    firstelem = 1;
    int headnum;
    headnum = 2;

    if ( fits_movabs_hdu(*fptr, headnum, 0, &status) )
        printerror( status );
    for(long i=0; i<Aindex; i++) {
        volume = AvolumeID[i];
        energyDep = (float)Aenergy[i];
        energyStart = (float)Aenergystart[i];
        evtid = Aevent_id[i];
// 			G4cout << evtid << G4endl;
// 			if(energyDep  > 10e-8) {
        if(fits_write_col(*fptr, TINT32BIT, 1, nrows_event_hdu, firstelem, (long)1, &evtid, &status))
            PrintErrorStatus(status, headnum, 1);

        //VOLUME_ID
        if(fits_write_col(*fptr, TINT32BIT, 2, nrows_event_hdu, firstelem, (long)1, &(volume), &status))
            PrintErrorStatus(status, headnum, 1);

        //EVT_KE
        if(energyDep <= 10e-8)
            energyDep = 0;
        if(fits_write_col(*fptr, TDOUBLE, 3, nrows_event_hdu, firstelem, (long)1, &energyDep, &status))
            PrintErrorStatus(status, headnum, 1);

        ttrkid = Atrkid[i];
        if(fits_write_col(*fptr, TINT32BIT, 4, nrows_event_hdu, firstelem, (long)1, &ttrkid, &status))
            PrintErrorStatus(status, headnum, 1);

        //PARTICLE
        tpart = Aparticle[i];
        if(fits_write_col(*fptr, TLONG, 5, nrows_event_hdu, firstelem, (long)1, &tpart, &status))
            PrintErrorStatus(status, headnum, 1);
        //PROCESS
        tproc = Aprocess[i];
        if(fits_write_col(*fptr, TBYTE, 6, nrows_event_hdu, firstelem, (long)1, &tproc, &status))
            PrintErrorStatus(status, headnum, 1);

        tmultI = AmultI[i];
        if(fits_write_col(*fptr, TUSHORT, 7, nrows_event_hdu, firstelem, (long)1, &tmultI, &status))
            PrintErrorStatus(status, headnum, 1);

        tmultO = AmultO[i];
        if(fits_write_col(*fptr, TUSHORT, 8, nrows_event_hdu, firstelem, (long)1, &tmultO, &status))
            PrintErrorStatus(status, headnum, 1);

        nrows_event_hdu++;
// 			}

    }

    FITSOutputFile::close();

    if(Aindex > 0 && gm.compressionlevel != 0) {
        char* basename = (char*)gm.outputEnergyVolFileNameFITS_base.data();
        gm.CopyFileFits(filename, basename);
        G4String ss;
        ss = "gzip  -1 ";
        //ss += filename;

        ss += " ";
        ss += basename;
        G4cout << ss << G4endl;
        system(ss);
    }
}
