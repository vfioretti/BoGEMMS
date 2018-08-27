/***************************************************************************
                          FITSOutput_INOUT.cc  -  description
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

#include "FITSOutput_INOUT.hh"
#include "CSteppingAction.hh"
#include "THELGlobalMemory.hh"
#include "ConfigFile.hh"

#if defined(GEANT4_10_1) || defined(GEANT4_10_2) || defined(GEANT4_10_3) || defined(GEANT4_10_4)
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"
#endif


//ricordati di indicare il numero delle colonne
// #define FITS_TYPE_D "1D"
// #define FITS_TYPE_D2 TDOUBLE
#define FITS_TYPE_D "1E"
#define FITS_TYPE_D2 TFLOAT
#define G4_TYPE G4float

#define NCOLS_FITS 20
//#define ADDCOLSTXT

//possibile parametri di formattazione del file INOUT
//1) write position
//2) write momentum
//3) write momentum direction
//4) write element ID
//5) write element name
//6) don't write input event
//7) don't write output event
FITSOutput_INOUT::FITSOutput_INOUT() : FITSOutputFileBinaryTable(NCOLS_FITS) {
    writePosition = false;
    writeMomentum = false;
    writeMomentumDirection = false;
    writeElementID = true;
    writeElementName = false;
    writeInputEvents = true;
    writeOutputEvents = true;
    colconfig = new bool[NCOLS_FITS];
    ttype1_template = new char* [NCOLS_FITS];
    tform1_template = new char* [NCOLS_FITS];
    tunit1_template = new char* [NCOLS_FITS];
    pfind1_template = new int [NCOLS_FITS]; 	//  for field index in .packet
    tycom1_template = (char**) new  char* [NCOLS_FITS];
    current_event_id = -1;
}


FITSOutput_INOUT::~FITSOutput_INOUT() {

}

// bool FITSOutput_INOUT::close() {
// 	MIOOutputFileFITSBinaryTable::close();
// 	G4String ss; ss = "gzip -9 "; ss += filename;
// 	G4cout << ss << " on close " << G4endl;
//  	system(ss);
// }

bool FITSOutput_INOUT::init() {
    setFileName((char*)gm.GetOutputStepBoundaryFileNameFITS().data());
    extname1 = (char*)"BOUNDARY";

    //definizione delle colonne
    tform1_template[0]  = (char*)"1V";
    ttype1_template[0] = (char*)"EVT_ID";
    tunit1_template[0]  = (char*)"";
    pfind1_template[0]  = 0;
    tycom1_template[0]  = (char*)"Event ID";
    tform1_template[1]  = (char*)"1V";
    ttype1_template[1] = (char*)"TRK_ID";
    tunit1_template[1]  = (char*)"";
    pfind1_template[1]  = 0;
    tycom1_template[1]  = (char*)"Track ID";
    tform1_template[2]  = (char*)"1B";
    ttype1_template[2] = (char*)"DIRECTION";
    tunit1_template[2]  = (char*)"";
    pfind1_template[2]  = 0;
    tycom1_template[2]  = (char*)"In/out identification";
    tform1_template[3]  = (char*)FITS_TYPE_D;
    ttype1_template[3] = (char*)"GTIME";
    tunit1_template[3]  = (char*)"ns";
    pfind1_template[3]  = 0;
    tycom1_template[3]  = (char*)"Global time";
    tform1_template[4]  = (char*)"1D";
    ttype1_template[4] = (char*)"EVT_KE";
    tunit1_template[4]  =(char*) "keV";
    pfind1_template[4]  = 0;
    tycom1_template[4]  = (char*)"Kinetic energy";
    tform1_template[5]  = (char*)"1J";
    ttype1_template[5] = (char*)"PARTICLE_ID";
    tunit1_template[5]  = (char*)"";
    pfind1_template[5]  = 0;
    tycom1_template[5]  = (char*)"Particle ID";
    tform1_template[6]  = (char*)"1B";
    ttype1_template[6] = (char*)"PROCESS_ID";
    tunit1_template[6]  = (char*)"";
    pfind1_template[6]  = 0;
    tycom1_template[6]  = (char*)"Process ID";
    tform1_template[7]  = (char*)"1V";
    ttype1_template[7] = (char*)"VOLUME_ID";
    tunit1_template[7]  = (char*)"";
    pfind1_template[7]  = 0;
    tycom1_template[7]  = (char*)"Volume ID";
    tform1_template[8]  = (char*)FITS_TYPE_D;
    ttype1_template[8] = (char*)"X";
    tunit1_template[8]  = (char*)"mm";
    pfind1_template[8]  = 0;
    tycom1_template[8]  = (char*)"position X";
    tform1_template[9]  = (char*)FITS_TYPE_D;
    ttype1_template[9] = (char*)"Y";
    tunit1_template[9]  = (char*)"mm";
    pfind1_template[9]  = 0;
    tycom1_template[9]  = (char*)"position Y";
    tform1_template[10]  = (char*)FITS_TYPE_D;
    ttype1_template[10] = (char*)"Z";
    tunit1_template[10]  = (char*)"mm";
    pfind1_template[10]  = 0;
    tycom1_template[10]  = (char*)"position Z";
    tform1_template[11]  = (char*)FITS_TYPE_D;
    ttype1_template[11] = (char*)"MX";
    tunit1_template[11]  = (char*)"";
    pfind1_template[11]  = 0;
    tycom1_template[11]  = (char*)"momentum X";
    tform1_template[12]  = (char*)FITS_TYPE_D;
    ttype1_template[12] = (char*)"MY";
    tunit1_template[12]  = (char*)"";
    pfind1_template[12]  = 0;
    tycom1_template[12]  = (char*)"momentum Y";
    tform1_template[13]  = (char*)FITS_TYPE_D;
    ttype1_template[13] = (char*)"MZ";
    tunit1_template[13]  = (char*)"";
    pfind1_template[13]  = 0;
    tycom1_template[13]  = (char*)"momentum Z";
    tform1_template[14] = (char*)"32A";
    ttype1_template[14] = (char*)"PARTICLE";
    tunit1_template[14] = (char*)"";
    pfind1_template[14] = 0;
    tycom1_template[14] = (char*)"Particle name";
    tform1_template[15] = (char*)"32A";
    ttype1_template[15] = (char*)"PROCESS";
    tunit1_template[15] = (char*)"";
    pfind1_template[15] = 0;
    tycom1_template[15] = (char*)"Process name";
    tform1_template[16] = (char*)"32A";
    ttype1_template[16] = (char*)"VOLUME";
    tunit1_template[16] = (char*)"";
    pfind1_template[16] = 0;
    tycom1_template[16] = (char*)"Volume name";
    tform1_template[17] = (char*)FITS_TYPE_D;
    ttype1_template[17] = (char*)"MDX";
    tunit1_template[17] = (char*)"";
    pfind1_template[17] = 0;
    tycom1_template[17] = (char*)"momentum direction X";
    tform1_template[18] = (char*)FITS_TYPE_D;
    ttype1_template[18] = (char*)"MDY";
    tunit1_template[18] = (char*)"";
    pfind1_template[18] = 0;
    tycom1_template[18] = (char*)"momentum direction Y";
    tform1_template[19] = (char*)FITS_TYPE_D;
    ttype1_template[19] = (char*)"MDZ";
    tunit1_template[19] = (char*)"";
    pfind1_template[19] = 0;
    tycom1_template[19] = (char*)"momentum direction Z";

    //lettura del file di configurazione

    gm.config->readInto(colconfig[0], "IOINOUT.EVT_ID");
    gm.config->readInto(colconfig[1], "IOINOUT.TRK_ID");
    gm.config->readInto(colconfig[2], "IOINOUT.DIRECTION");
    gm.config->readInto(colconfig[3], "IOINOUT.GTIME");
    gm.config->readInto(colconfig[4], "IOINOUT.EVT_KE");
    gm.config->readInto(colconfig[5], "IOINOUT.PARTICLE_ID");
    gm.config->readInto(colconfig[6], "IOINOUT.PROCESS_ID");
    gm.config->readInto(colconfig[7], "IOINOUT.VOLUME_ID");
    gm.config->readInto(colconfig[8], "IOINOUT.X");
    gm.config->readInto(colconfig[9], "IOINOUT.Y");
    gm.config->readInto(colconfig[10], "IOINOUT.Z");
    gm.config->readInto(colconfig[11], "IOINOUT.MX");
    gm.config->readInto(colconfig[12], "IOINOUT.MY");
    gm.config->readInto(colconfig[13], "IOINOUT.MZ");
    gm.config->readInto(colconfig[14], "IOINOUT.PARTICLE");
    gm.config->readInto(colconfig[15], "IOINOUT.PROCESS");
    gm.config->readInto(colconfig[16], "IOINOUT.VOLUME");
    gm.config->readInto(colconfig[17], "IOINOUT.MDX");
    gm.config->readInto(colconfig[18], "IOINOUT.MDY");
    gm.config->readInto(colconfig[19], "IOINOUT.MDZ");

    gm.config->readInto(writeIN, "IOINOUT.IN");
    gm.config->readInto(writeOUT, "IOINOUT.OUT");
    gm.config->readInto(neventsinfile, "IOINOUT.NRows");
    gm.config->readInto(writeInitialCondition, "IOINOUT.InitialCondition");
    ncol_header1 = 0;
    int ncolw = 0;
    for(int i=0; i<NCOLS_FITS; i++) {
        if(colconfig[i] == 1) {
            tform1[ncolw] = tform1_template[i];
            ttype1[ncolw] = ttype1_template[i];
            tunit1[ncolw] = tunit1_template[i];
            pfind1[ncolw] = pfind1_template[i];
            tycom1[ncolw] = tycom1_template[i];
            G4cout << "Added column number " << ncolw << " of name " << ttype1[ncolw] << G4endl;
            ncolw++;
        }
    }
    ncol_header1 = ncolw;
    G4cout << "Write IN rows " << writeIN << G4endl;
    G4cout << "Write OUT rows " << writeOUT << G4endl;
    G4cout << "Write nrows for each file " << neventsinfile << G4endl;

    //volume selection
    volselectionN = 0;
    gm.config->readInto(volselectionN, "IOINOUT.volumeselection.number");
    G4cout << "Volume selection: " << volselectionN << G4endl;
    char c[512];
    for(int i=0; i<volselectionN; i++) {
        sprintf(c, "IOINOUT.volumeselection.%i.start", i);
        gm.config->readInto(volselectionstart[i], c);
        G4cout << "Volume selection " << i << " start " << volselectionstart[i] << G4endl;
        sprintf(c, "IOINOUT.volumeselection.%i.end", i);
        gm.config->readInto(volselectionend[i], c);
        G4cout << "Volume selection " << i << " end " << volselectionend[i] << G4endl;
    }
    FITSOutputFileBinaryTable::init();
    lastevent = -1;
    open();
    close();
    newfile = true;
    return true;
}

void FITSOutput_INOUT::writeData(CSteppingAction* stepping, bool writeInAnyCase) {
    //era 1000000
// 	if(gm.event_id%neventsinfile == 0 && gm.event_id != 0) {
    bool newevent = false;
    if(lastevent != gm.event_id) {
        newevent = true;
        lastevent = gm.event_id;
    }
    else
        newevent = false;




    if(nrows_event_hdu >= neventsinfile  && newevent == true) {
        if(newfile == true) {
            close();
            char* basename = (char*)gm.outputStepBoundaryFileNameFITS_base.data();
            gm.CopyFileFits(filename, basename);
            G4String ss;
            ss = "gzip -9 ";
            //ss += filename;
            ss += basename;
            G4cout << ss << G4endl;
            system(ss);

            // 		setFileName((char*)gm.GetOutputStepBoundaryFileNameFITS().data());
            init();
            open();
            newfile = false;
        }
    } else
        newfile = true;


    unsigned char inout = 0;
    inout = stepping->inoutCode;
    if(current_event_id != gm.event_id) {
// 		initial_step = stepping;
        current_event_id = gm.event_id;
        initial_condition_written = false;
    }
    bool write = true;

    /*	if(!(writeInitialCondition == true && writeOUT == false && writeIN == true && inout == 0 && initial_condition_written == false)) {
    		write = false;
    	} else
    		initial_condition_written = true;*/
    //IN -> 0 OUT -> 1
    //si genera un evento che è OUT, poi questo entra in un volume (IN)
    if(write)
        if(writeInAnyCase == false) {
            if(writeIN == false && inout == 0) //se non devo scrivere un evento IN e questo è un evento IN
                write = false;
            if(writeOUT == false && inout == 1)
                write = false;
        }
    //se devo scrivere un evento IN e questo è un evento IN ma non devo scrivere un evento OUT ma non ho ancora scritto la condizione
    //iniziale e sono nel ciclo di non scrivere in ogni caso
    /*	if(writeInitialCondition == true && initial_condition_written == false)
    		write = true;*/
    if(writeInitialCondition && inout == 1 && initial_condition_written == false) {
        initial_condition_written = true;
        write = true;
    }

    G4int volReplica = stepping->volReplica;

    if(volselectionN != 0) {
        bool volumein = false;
        for(int i=0; i<volselectionN; i++) {
// 			G4cout << volReplica << " " << volselectionstart[i] << " " << volselectionend[i] << " - ";
            if(volReplica >= volselectionstart[i] && volReplica <= volselectionend[i])
                volumein = true;
        }
        if(volumein == false)
            write = false;
    }
    if(!write)
        return;



    long firstelem;
    int status;
// 	unsigned char b;             //1 unsigned char unsigned
// 	unsigned short w, wh, wl;     //2 unsigned char unsigned
// 	long int li, lil, lih;
// 	unsigned long d, dh, dl;    //4 unsigned char unsigned
// 	double d64;
// 	float e, e1, e2;
    status=0;
    firstelem = 1;
    int headnum;
    headnum = 2;

    G4String volNameInput;
    G4String procName;
// 	G4String inout;
    G4double ekin = stepping->ekin / keV;
    G4_TYPE gltime = stepping->gltime / ns;

// 	G4int trackID;
// 	G4ThreeVector pos;
    G4_TYPE pos[3];
    pos[0] = stepping->pos[0] / mm;
    pos[1] = stepping->pos[1] / mm;
    pos[2] = stepping->pos[2] / mm;
// 	G4ThreeVector mom;
    G4_TYPE mom[3];
    mom[0] = stepping->mom[0];
    mom[1] = stepping->mom[1];
    mom[2] = stepping->mom[2];
// 	G4ThreeVector momDir;
    G4_TYPE momDir[3];
    momDir[0] = stepping->momDir[0];
    momDir[1] = stepping->momDir[1];
    momDir[2] = stepping->momDir[2];

    if ( fits_movabs_hdu(*fptr, headnum, 0, &status) )
        printerror( status );
    //EVT_ID
    int colnumw = 1;
    if(colconfig[0])
        if(fits_write_col(*fptr, TINT32BIT, colnumw++, nrows_event_hdu, firstelem, (long)1, &gm.event_id, &status))
            PrintErrorStatus(status, headnum, 1);
    //TRK_ID
    if(colconfig[1])
        if(fits_write_col(*fptr, TINT32BIT, colnumw++, nrows_event_hdu, firstelem, (long)1, &stepping->trackID, &status))
            PrintErrorStatus(status, headnum, 1);

    //DIRECTION
    char* aaa;
// 	char* aaa = (char*)stepping->inout.data();

    inout = stepping->inoutCode;
    if(colconfig[2])
        if(fits_write_col(*fptr, TBYTE, colnumw++, nrows_event_hdu, firstelem, (long)1, &(inout), &status))
            PrintErrorStatus(status, headnum, 1);

    //GTIME
    if(colconfig[3])
        if(fits_write_col(*fptr, FITS_TYPE_D2, colnumw++, nrows_event_hdu, firstelem, (long)1, &gltime, &status))
            PrintErrorStatus(status, headnum, 1);

    //EVT_KE
    if(colconfig[4])
        if(fits_write_col(*fptr, TDOUBLE, colnumw++, nrows_event_hdu, firstelem, (long)1, &ekin, &status))
            PrintErrorStatus(status, headnum, 1);

    //PARTICLE
    if(colconfig[5])
        if(fits_write_col(*fptr, TLONG, colnumw++, nrows_event_hdu, firstelem, (long)1, &stepping->particleIDstep, &status))
            PrintErrorStatus(status, headnum, 1);

    //PROCESS
    if(colconfig[6])
        if(fits_write_col(*fptr, TBYTE, colnumw++, nrows_event_hdu, firstelem, (long)1, &stepping->processID, &status))
            PrintErrorStatus(status, headnum, 1);

    //VOLUME_ID
    if(colconfig[7])
        if(fits_write_col(*fptr, TINT32BIT, colnumw++, nrows_event_hdu, firstelem, (long)1, &stepping->volReplica, &status))
            PrintErrorStatus(status, headnum, 1);

    //X
    if(colconfig[8])
        if(fits_write_col(*fptr, FITS_TYPE_D2, colnumw++, nrows_event_hdu, firstelem, (long)1, &pos[0], &status))
            PrintErrorStatus(status, headnum, 1);

    //Y
    if(colconfig[9])
        if(fits_write_col(*fptr, FITS_TYPE_D2, colnumw++, nrows_event_hdu, firstelem, (long)1, &pos[1], &status))
            PrintErrorStatus(status, headnum, 1);

    //Z
    if(colconfig[10])
        if(fits_write_col(*fptr, FITS_TYPE_D2, colnumw++, nrows_event_hdu, firstelem, (long)1, &pos[2], &status))
            PrintErrorStatus(status, headnum, 1);

    //MX
    if(colconfig[11])
        if(fits_write_col(*fptr, FITS_TYPE_D2, colnumw++, nrows_event_hdu, firstelem, (long)1, &mom[0], &status))
            PrintErrorStatus(status, headnum, 1);

    //MY
    if(colconfig[12])
        if(fits_write_col(*fptr, FITS_TYPE_D2, colnumw++, nrows_event_hdu, firstelem, (long)1, &mom[1], &status))
            PrintErrorStatus(status, headnum, 1);

    //MZ
    if(colconfig[13])
        if(fits_write_col(*fptr, FITS_TYPE_D2, colnumw++, nrows_event_hdu, firstelem, (long)1, &mom[2], &status))
            PrintErrorStatus(status, headnum, 1);




    //PARTICLE


    if(colconfig[14]) {
        aaa = (char*)stepping->particleName.data();
        if(fits_write_col(*fptr, TSTRING, colnumw++, nrows_event_hdu, firstelem, (long)1, &(aaa), &status))
            PrintErrorStatus(status, headnum, 1);
    }

    //PROCESS

    if(colconfig[15]) {
        aaa = (char*)stepping->procName.data();
        if(fits_write_col(*fptr, TSTRING, colnumw++, nrows_event_hdu, firstelem, (long)1, &(aaa), &status))
            PrintErrorStatus(status, headnum, 1);
    }

    //VOLUME
    if(colconfig[16]) {
        aaa = (char*)stepping->volNameInput.data();
        if(fits_write_col(*fptr, TSTRING, colnumw++, nrows_event_hdu, firstelem, (long)1, &(aaa), &status))
            PrintErrorStatus(status, headnum, 1);
    }

    //MDX
    if(colconfig[17])
        if(fits_write_col(*fptr, FITS_TYPE_D2, colnumw++, nrows_event_hdu, firstelem, (long)1, &momDir[0], &status))
            PrintErrorStatus(status, headnum, 1);

    //MDY
    if(colconfig[18])
        if(fits_write_col(*fptr, FITS_TYPE_D2, colnumw++, nrows_event_hdu, firstelem, (long)1, &momDir[1], &status))
            PrintErrorStatus(status, headnum, 1);

    //MDZ
    if(colconfig[19])
        if(fits_write_col(*fptr, FITS_TYPE_D2, colnumw++, nrows_event_hdu, firstelem, (long)1, &momDir[2], &status))
            PrintErrorStatus(status, headnum, 1);

    nrows_event_hdu++;
    //G4cout << nrows_event_hdu << G4endl;

// 	close();
}
