/***************************************************************************
                          FITSOutput_EnergyVol.cc  -  description
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

#include "FITSOutput_EnergyVol.hh"
#include "THELGlobalMemory.hh"
#include "ConfigFile.hh"
//ricordati di indicare il numero delle colonne
// #define FITS_TYPE_D "1D"
// #define FITS_TYPE_D2 TDOUBLE
#define FITS_TYPE_D "1E"
#define FITS_TYPE_D2 TFLOAT
#define G4_TYPE G4float

#define NCOLS_FITS 8
//#define ADDCOLSTXT

//possibile parametri di formattazione del file EnergyVol
//1) write position
//2) write momentum
//3) write momentum direction
//4) write element ID
//5) write element name
//6) don't write input event
//7) don't write output event
FITSOutput_EnergyVol::FITSOutput_EnergyVol() : FITSOutputFileBinaryTable(NCOLS_FITS), IOEnergyBase() {
    colconfig = new bool[NCOLS_FITS];
    ttype1_template = new char* [NCOLS_FITS];
    tform1_template = new char* [NCOLS_FITS];
    tunit1_template = new char* [NCOLS_FITS];
    pfind1_template = new int [NCOLS_FITS]; 	//  for field index in .packet
    tycom1_template = (char**) new  char* [NCOLS_FITS];
    current_event_id = -1;
    //definizione delle colonne
    tform1_template[0]  = (char*)"1V";
    ttype1_template[0] = (char*)"EVT_ID";
    tunit1_template[0]  = (char*)"";
    pfind1_template[0]  = 0;
    tycom1_template[0]  = (char*)"Event ID";
    tform1_template[1]  = (char*)"1V";
    ttype1_template[1] = (char*)"VOLUME_ID";
    tunit1_template[1]  = (char*)"";
    pfind1_template[1]  = 0;
    tycom1_template[1]  = (char*)"Volume ID";
    tform1_template[2]  = (char*)"1D";
    ttype1_template[2] = (char*)"ENERGY";
    tunit1_template[2]  =(char*) "keV";
    pfind1_template[2]  = 0;
    tycom1_template[2]  = (char*)"Kinetic energy";

    tform1_template[3]  = (char*)"1V";
    ttype1_template[3] = (char*)"TRK_ID";
    tunit1_template[3]  = (char*)"";
    pfind1_template[3]  = 0;
    tycom1_template[3]  = (char*)"Track ID";
    tform1_template[4]  = (char*)"1J";
    ttype1_template[4] = (char*)"PARTICLE_ID";
    tunit1_template[4]  = (char*)"";
    pfind1_template[4]  = 0;
    tycom1_template[4]  = (char*)"Particle ID";
    tform1_template[5]  = (char*)"1B";
    ttype1_template[5] = (char*)"PROCESS_ID";
    tunit1_template[5]  = (char*)"";
    pfind1_template[5]  = 0;
    tycom1_template[5]  = (char*)"Process ID";
    tform1_template[6]  = (char*)"1U";
    ttype1_template[6] = (char*)"MULTI";
    tunit1_template[6]  = (char*)"";
    pfind1_template[6]  = 0;
    tycom1_template[6]  = (char*)"Multiplicity Input";
    tform1_template[7]  = (char*)"1U";
    ttype1_template[7] = (char*)"MULTO";
    tunit1_template[7]  = (char*)"";
    pfind1_template[7]  = 0;
    tycom1_template[7]  = (char*)"Multiplicity Output";

    ncol_header1 = NCOLS_FITS;

    extname1 = (char*)"BOUNDARY";
    for(int i=0; i<NCOLS_FITS; i++) {

        tform1[i] = tform1_template[i];
        ttype1[i] = ttype1_template[i];
        tunit1[i] = tunit1_template[i];
        pfind1[i] = pfind1_template[i];
        tycom1[i] = tycom1_template[i];
        G4cout << "Added column number " << i << " of name " << ttype1[i] << G4endl;

    }

}


FITSOutput_EnergyVol::~FITSOutput_EnergyVol() {
    cout << "FITSOutput_EnergyVol::~FITSOutput_EnergyVol()" << endl;
}



bool FITSOutput_EnergyVol::init() {
    G4cout << "bool FITSOutput_EnergyVol::init()  " << G4endl;
    ResetData();
    setFileName((char*)gm.GetOutputEnergyVolFileNameFITS().data());

    //lettura del file di configurazione
    gm.config->readInto(neventsinfile, "IOENERGY.NRows");

//	ncol_header1 = 0;

    G4cout << "Write nrows for each file " << neventsinfile << G4endl;

    IOEnergyBase::initConf();
    FITSOutputFileBinaryTable::init();
    lastevent = -1;
    open();
    newfile = true;
    return true;
}


void FITSOutput_EnergyVol::writeData() {

    bool newevent = false;
    if(lastevent != gm.event_id) {
        newevent = true;
        lastevent = gm.event_id;
    } else
        newevent = false;

    if(newevent == false)
        return;

    if(nrows_event_hdu >= neventsinfile  && newevent == true) {
        if(newfile == true) {
            close();
            char* basename = (char*)gm.outputEnergyVolFileNameFITS_base.data();
            gm.CopyFileFits(filename, basename);
            if(gm.compressionlevel != 0) {
                G4String ss;
                ss = "gzip -1 ";
                ss += basename;
                G4cout << ss << G4endl;
                system(ss);
            }
            double s =  energystart;
            init();
            energystart = energy[0] = s;
            open();
            newfile = false;
        }
    } else
        newfile = true;


    if(current_event_id != gm.event_id) {
        current_event_id = gm.event_id;
    }
    long firstelem;
    int status;
    status=0;
    firstelem = 1;
    int headnum;
    headnum = 2;


    if ( fits_movabs_hdu(*fptr, headnum, 0, &status) )
        printerror( status );

	double energyCheck = 0;
    for(int i=0; i<NMAXVOLFOREVENT; i++) {
        if(volumeID[i] != -1 ) {

			evtid = gm.event_id;
			volume = volumeID[i];
			if(writeonlydetectorevents > 0) {
				if (!(volumeID[i] >= gm.detector_volmin && volumeID[i] <= gm.detector_volmax))
					continue;
			}
			if(energy[i] <= 10e-8)
				energy[i] = 0;
			energyDep = energy[i];
//			if(particle[i]>0)
//				energyCheck += energy[i];
			if(writeonlyifenergydeposit && energyDep == 0)
				continue;
			ttrkid = trkid[i];
			tpart = particle[i];
			if(removeWorldEvent && particle[i] == 0)
				continue;
			tproc = process[i];
			tmultI = multI[i];
			tmultO = multO[i];
//			if(energyCheck > 3001)
//				cout << "@ " << (double)energyCheck << endl;

            if(fits_write_col(*fptr, TINT32BIT, 1, nrows_event_hdu, firstelem, (long)1, &evtid, &status))
                PrintErrorStatus(status, headnum, 1);

            //VOLUME_ID
			
            if(fits_write_col(*fptr, TINT32BIT, 2, nrows_event_hdu, firstelem, (long)1, &(volume), &status))
                PrintErrorStatus(status, headnum, 1);

            //EVT_KE

            if(fits_write_col(*fptr, TDOUBLE, 3, nrows_event_hdu, firstelem, (long)1, &energyDep, &status))
                PrintErrorStatus(status, headnum, 1);

			
            if(fits_write_col(*fptr, TINT32BIT, 4, nrows_event_hdu, firstelem, (long)1, &ttrkid, &status))
                PrintErrorStatus(status, headnum, 1);

            //PARTICLE
			
            if(fits_write_col(*fptr, TLONG, 5, nrows_event_hdu, firstelem, (long)1, &tpart, &status))
                PrintErrorStatus(status, headnum, 1);
            //PROCESS
			
            if(fits_write_col(*fptr, TBYTE, 6, nrows_event_hdu, firstelem, (long)1, &tproc, &status))
                PrintErrorStatus(status, headnum, 1);

			
            if(fits_write_col(*fptr, TUSHORT, 7, nrows_event_hdu, firstelem, (long)1, &tmultI, &status))
                PrintErrorStatus(status, headnum, 1);

			
            if(fits_write_col(*fptr, TUSHORT, 8, nrows_event_hdu, firstelem, (long)1, &tmultO, &status))
                PrintErrorStatus(status, headnum, 1);

            nrows_event_hdu++;
        } else
            break;
    }
    ResetData();
}
