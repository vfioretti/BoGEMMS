/***************************************************************************
                          FITSOutput_Step.cc  -  description
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

#include "FITSOutput_Step.hh"
#include "CSteppingAction.hh"
#include "THELGlobalMemory.hh"
#include "CHit.hh"

#if defined(GEANT4_10_1) || defined(GEANT4_10_2) || defined(GEANT4_10_3) || defined(GEANT4_10_4)
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"
#endif


//ricordati di indicare il numero delle colonne
FITSOutput_Step::FITSOutput_Step() : FITSOutputFileBinaryTable(20) {

}


FITSOutput_Step::~FITSOutput_Step() {

}


bool FITSOutput_Step::init() {
    G4String str = gm.GetOutputHitFileNameFITS();
    G4cout << str << G4endl;
    setFileName((char*)str.data());
    extname1 = (char*)"HITS";
    //definizione delle colonne
    tform1[0] = (char*)"1V";
    ttype1[0] = (char*)"EVT_ID";
    tunit1[0] = (char*)"";
    pfind1[0] = 0;
    tycom1[0] = (char*)"Event ID";
    tform1[1] = (char*)"1V";
    ttype1[1] = (char*)"TRK_ID";
    tunit1[1] = (char*)"";
    pfind1[1] = 0;
    tycom1[1] = (char*)"Track ID";
    tform1[2] = (char*)"1V";
    ttype1[2] = (char*)"STEP";
    tunit1[2] = (char*)"";
    pfind1[2] = 0;
    tycom1[2] = (char*)"Step ID";
    tform1[3] = (char*)"1D";
    ttype1[3] = (char*)"GTIME";
    tunit1[3] = (char*)"ns";
    pfind1[3] = 0;
    tycom1[3] = (char*)"Global time";
    tform1[4] = (char*)"1D";
    ttype1[4] = (char*)"LTIME";
    tunit1[4] = (char*)"ns";
    pfind1[4] = 0;
    tycom1[4] = (char*)"Local time";
    tform1[5] = (char*)"1D";
    ttype1[5] = (char*)"PTIME";
    tunit1[5] = (char*)"ns";
    pfind1[5] = 0;
    tycom1[5] = (char*)"Proper time";
    tform1[6] = (char*)"1D";
    ttype1[6] = (char*)"EVT_KE";
    tunit1[6] = (char*)"keV";
    pfind1[6] = 0;
    tycom1[6] = (char*)"Kinetic energy";
    tform1[7] = (char*)"1D";
    ttype1[7] = (char*)"STEP_E";
    tunit1[7] = (char*)"keV";
    pfind1[7] = 0;
    tycom1[7] = (char*)"Energy deposit step";
    tform1[8] = (char*)"1D";
    ttype1[8] = (char*)"X";
    tunit1[8] = (char*)"mm";
    pfind1[8] = 0;
    tycom1[8] = (char*)"position X";
    tform1[9] = (char*)"1D";
    ttype1[9] = (char*)"Y";
    tunit1[9] = (char*)"mm";
    pfind1[9] = 0;
    tycom1[9] = (char*)"position Y";
    tform1[10] = (char*)"1D";
    ttype1[10] = (char*)"Z";
    tunit1[10] = (char*)"mm";
    pfind1[10] = 0;
    tycom1[10] = (char*)"position Z";
    tform1[11] = (char*)"1D";
    ttype1[11] = (char*)"MX";
    tunit1[11] = (char*)"";
    pfind1[11] = 0;
    tycom1[11] = (char*)"momentum X";
    tform1[12] = (char*)"1D";
    ttype1[12] = (char*)"MY";
    tunit1[12] = (char*)"";
    pfind1[12] = 0;
    tycom1[12] = (char*)"momentum Y";
    tform1[13] = (char*)"1D";
    ttype1[13] = (char*)"MZ";
    tunit1[13] = (char*)"";
    pfind1[13] = 0;
    tycom1[13] = (char*)"momentum Z";
    tform1[14] = (char*)"1D";
    ttype1[14] = (char*)"L_STEP";
    tunit1[14] = (char*)"mm";
    pfind1[14] = 0;
    tycom1[14] = (char*)"Step length";
    tform1[15] = (char*)"1D";
    ttype1[15] = (char*)"L_TRK";
    tunit1[15] = (char*)"mm";
    pfind1[15] = 0;
    tycom1[15] = (char*)"Track length";
    /*	tform1[16] = (char*)"10A"; ttype1[16] = (char*)"PARTICLE"; tunit1[16] = (char*)""; pfind1[16] = 0; tycom1[16] = (char*)"Particle name";
    	tform1[17] = (char*)"32A"; ttype1[17] = (char*)"PROCESS"; tunit1[17] = (char*)""; pfind1[17] = 0; tycom1[17] = (char*)"Process name";
    	tform1[18] = (char*)"32A"; ttype1[18] = (char*)"VOLUME"; tunit1[18] = (char*)""; pfind1[18] = 0; tycom1[18] = (char*)"Volume name";*/
    tform1[16]  = (char*)"1J";
    ttype1[16] = (char*)"PARTICLE_ID";
    tunit1[16]  = (char*)"";
    pfind1[16]  = 0;
    tycom1[16]  = (char*)"Particle ID";
    tform1[17]  = (char*)"1B";
    ttype1[17] = (char*)"PROCESS_ID";
    tunit1[17]  = (char*)"";
    pfind1[17]  = 0;
    tycom1[17]  = (char*)"Process ID";
    tform1[18]  = (char*)"1V";
    ttype1[18] = (char*)"VOLUME_ID";
    tunit1[18]  = (char*)"";
    pfind1[18]  = 0;
    tycom1[18]  = (char*)"Volume ID";
    tform1[19]  = (char*)"1V";
    ttype1[19] = (char*)"MOTHERVOL_ID";
    tunit1[19]  = (char*)"";
    pfind1[19]  = 0;
    tycom1[19]  = (char*)"Mother Volume ID";
    tform1[20] = (char*)"32A";
    ttype1[20] = (char*)"PROCESS";
    tunit1[20] = (char*)"";
    pfind1[20] = 0;
    tycom1[20] = (char*)"Process name";

    gm.config->readInto(neventsinfile, "IOSTEP.NRows");

    FITSOutputFileBinaryTable::init();
    lastevent = -1;
    open();
    close();
    newfile = true;
    current_event_id = -1;
    return true;


}

void FITSOutput_Step::writeData(CHit* hit, G4int i) {

    //TODO
    /*bool newevent = false;
    if(lastevent != gm.event_id) {
    	newevent = true;
    	lastevent = gm.event_id;
    }
    else
    	newevent = false;

    if(newevent == false)
    	return;

    if(nrows_event_hdu >= neventsinfile  && newevent == true) {
    	if(newfile == true) {
    		close();
    		G4String ss; ss = "gzip -9 "; ss += filename;
    		G4cout << ss << G4endl;
    		system(ss);

    		init();
    		open();
    		newfile = false;
    	}
    } else
    	newfile = true;


    if(current_event_id != gm.event_id) {
    	current_event_id = gm.event_id;
    }*/

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

    G4double time;

    if ( fits_movabs_hdu(*fptr, headnum, 0, &status) )
        printerror( status );
    //EVT_ID
    if(fits_write_col(*fptr, TINT32BIT, 1, nrows_event_hdu, firstelem, (long)1, &gm.event_id, &status))
        PrintErrorStatus(status, headnum, 1);
    //TRK_ID
    G4int tri = hit->GetTrackID();
    if(fits_write_col(*fptr, TINT32BIT, 2, nrows_event_hdu, firstelem, (long)1, &tri, &status))
        PrintErrorStatus(status, headnum, 1);
    //STEP
    if(fits_write_col(*fptr, TINT32BIT, 3, nrows_event_hdu, firstelem, (long)1, &i, &status))
        PrintErrorStatus(status, headnum, 1);

    //GTIME
    time = hit->GetGlobalTime() / ns;
    if(fits_write_col(*fptr, TDOUBLE, 4, nrows_event_hdu, firstelem, (long)1, &time, &status))
        PrintErrorStatus(status, headnum, 1);
    //LTIME
    time = hit->GetLocalTime() / ns;
    if(fits_write_col(*fptr, TDOUBLE, 5, nrows_event_hdu, firstelem, (long)1, &time, &status))
        PrintErrorStatus(status, headnum, 1);
    //PTIME
    time = hit->GetProperTime() / ns;
    if(fits_write_col(*fptr, TDOUBLE, 6, nrows_event_hdu, firstelem, (long)1, &time, &status))
        PrintErrorStatus(status, headnum, 1);
    //EVT_KE
    G4double energy = hit->GetKineticEnergy() / keV;
    if(fits_write_col(*fptr, TDOUBLE, 7, nrows_event_hdu, firstelem, (long)1, &energy, &status))
        PrintErrorStatus(status, headnum, 1);
    //STEP_E
    energy = hit->GetEdep() / keV;
    if(fits_write_col(*fptr, TDOUBLE, 8, nrows_event_hdu, firstelem, (long)1, &energy, &status))
        PrintErrorStatus(status, headnum, 1);

    //X
    G4double pos = hit->GetPos()[0] / mm;
    if(fits_write_col(*fptr, TDOUBLE, 9, nrows_event_hdu, firstelem, (long)1, &pos, &status))
        PrintErrorStatus(status, headnum, 1);

    //Y
    pos = hit->GetPos()[1] / mm;
    if(fits_write_col(*fptr, TDOUBLE, 10, nrows_event_hdu, firstelem, (long)1, &pos, &status))
        PrintErrorStatus(status, headnum, 1);

    //Z
    pos = hit->GetPos()[2] / mm;
    if(fits_write_col(*fptr, TDOUBLE, 11, nrows_event_hdu, firstelem, (long)1, &pos, &status))
        PrintErrorStatus(status, headnum, 1);

    //MX
    G4double mom = hit->GetMom()[0];
    if(fits_write_col(*fptr, TDOUBLE, 12, nrows_event_hdu, firstelem, (long)1, &mom, &status))
        PrintErrorStatus(status, headnum, 1);

    //MY
    mom = hit->GetMom()[1];
    if(fits_write_col(*fptr, TDOUBLE, 13, nrows_event_hdu, firstelem, (long)1, &mom, &status))
        PrintErrorStatus(status, headnum, 1);

    //MZ
    mom = hit->GetMom()[2];
    if(fits_write_col(*fptr, TDOUBLE, 14, nrows_event_hdu, firstelem, (long)1, &mom, &status))
        PrintErrorStatus(status, headnum, 1);

    //L_STEP
    G4double lstep = hit->GetStepLength() / mm;
    if(fits_write_col(*fptr, TDOUBLE, 15, nrows_event_hdu, firstelem, (long)1, &lstep, &status))
        PrintErrorStatus(status, headnum, 1);
    //L_TRK
    lstep = hit->GetTrackLength() / mm;
    if(fits_write_col(*fptr, TDOUBLE, 16, nrows_event_hdu, firstelem, (long)1, &lstep, &status))
        PrintErrorStatus(status, headnum, 1);

    //PARTICLE
// 	char* aaa = (char*)hit->GetParticleType().data();
// 	if(fits_write_col(*fptr, TSTRING, 17, nrows_event_hdu, firstelem, (long)1, &(aaa), &status))
// 			PrintErrorStatus(status, headnum, 1);
//
// 	//PROCESS
// 	aaa = (char*)hit->GetProcessName().data();
// 	if(fits_write_col(*fptr, TSTRING, 18, nrows_event_hdu, firstelem, (long)1, &(aaa), &status))
// 			PrintErrorStatus(status, headnum, 1);
//
// 	//VOLUME
// 	aaa = (char*)hit->GetVolumeName().data();
// 	if(fits_write_col(*fptr, TSTRING, 19, nrows_event_hdu, firstelem, (long)1, &(aaa), &status))
// 			PrintErrorStatus(status, headnum, 1);

//	PARTICLEID
    G4int pidfla = hit->GetParticleID();
    if(fits_write_col(*fptr, TLONG, 17, nrows_event_hdu, firstelem, (long)1, &(pidfla), &status))
        PrintErrorStatus(status, headnum, 1);
    //PROCESSID
    unsigned char a2 = hit->GetProcessID();
    if(fits_write_col(*fptr, TBYTE, 18, nrows_event_hdu, firstelem, (long)1, &(a2), &status))
        PrintErrorStatus(status, headnum, 1);

// 	//VOLUMEID
    long v1 = hit->GetGeomNumber();
    if(fits_write_col(*fptr, TINT32BIT, 19, nrows_event_hdu, firstelem, (long)1, &(v1), &status))
        PrintErrorStatus(status, headnum, 1);
// 	//MOTHER VOLUMEID
    long mv1 = hit->GetMothID();
    if(fits_write_col(*fptr, TINT32BIT, 20, nrows_event_hdu, firstelem, (long)1, &(mv1), &status))
        PrintErrorStatus(status, headnum, 1);


// 	//PROCESS
    char* aaa = (char*)hit->GetProcessName().data();
    if(fits_write_col(*fptr, TSTRING, 21, nrows_event_hdu, firstelem, (long)1, &(aaa), &status))
        PrintErrorStatus(status, headnum, 1);

// 	G4cout << "write" << G4endl;
    nrows_event_hdu++;
// 	close();
}
