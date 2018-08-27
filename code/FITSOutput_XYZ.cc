/***************************************************************************
                          FITSOutput_XYZ.cc  -  description
                             -------------------
    begin                : 2012
    copyright            : (C) 2012 by  Valentina Fioretti
    email                : fioretti@iasfbo.inaf.it
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software for non commercial purpose              *
 *   and for public research institutes; you can redistribute it and/or    *
 *   modify it under the terms of the GNU General Public License.          *
 *   For commercial purpose see appropriate license terms                  *
 *                                                                         *
 ***************************************************************************/

#include "FITSOutput_XYZ.hh"
#include "CSteppingAction.hh"
#include "THELGlobalMemory.hh"
#include "CXYZHit.hh"

#if defined(GEANT4_10_1) || defined(GEANT4_10_2) || defined(GEANT4_10_3) || defined(GEANT4_10_4)
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"
#endif

#include "ConfigFile.hh"
//ricordati di indicare il numero delle colonne
// #define FITS_TYPE_D "1D"
// #define FITS_TYPE_D2 TDOUBLE
#define FITS_TYPE_D "1D"
#define FITS_TYPE_D2 TFLOAT
#define FITS_TYPE_D3 TDOUBLE
#define G4_TYPE G4float

#define NCOLS_FITS 27
//#define ADDCOLSTXT

//possibile parametri di formattazione del file INOUT
//1) write position
//2) write momentum
//3) write momentum direction
//4) write element ID
//5) write element name
//6) don't write input event
//7) don't write output event

FITSOutput_XYZ::FITSOutput_XYZ() : FITSOutputFileBinaryTable(NCOLS_FITS) {
    colconfig = new bool[NCOLS_FITS];
    ttype1_template = new char* [NCOLS_FITS];
    tform1_template = new char* [NCOLS_FITS];
    tunit1_template = new char* [NCOLS_FITS];
    pfind1_template = new int [NCOLS_FITS]; 	//  for field index in .packet
    tycom1_template = (char**) new  char* [NCOLS_FITS];
    current_event_id = -1;

	extname1 = (char*)"XYZ";
	
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
    tform1_template[2]  = (char*)"1V";
    ttype1_template[2] = (char*)"PARENT_TRK_ID";
    tunit1_template[2]  = (char*)"";
    pfind1_template[2]  = 0;
    tycom1_template[2]  = (char*)"Parent Track ID";
    tform1_template[3]  = (char*)"1V";
	ttype1_template[3] = (char*)"VOLUME_ID";
	tunit1_template[3]  = (char*)"";
	pfind1_template[3]  = 0;
	tycom1_template[3]  = (char*)"Volume ID";
	tform1_template[4] = (char*)"32A";
	ttype1_template[4] = (char*)"VOLUME_NAME";
	tunit1_template[4] = (char*)"";
	pfind1_template[4] = 0;
	tycom1_template[4] = (char*)"Volume name";
	tform1_template[5]  = (char*)"1V";
	ttype1_template[5] = (char*)"MOTHER_ID";
	tunit1_template[5]  = (char*)"";
	pfind1_template[5]  = 0;
	tycom1_template[5]  = (char*)"Mother Volume ID";
	tform1_template[6]  = (char*)"1D";
	ttype1_template[6] = (char*)"E_DEP";
	tunit1_template[6]  =(char*) "keV";
	pfind1_template[6]  = 0;
	tycom1_template[6]  = (char*)"Energy deposit";
	tform1_template[7]  = (char*)FITS_TYPE_D;
	ttype1_template[7] = (char*)"X_ENT";
	tunit1_template[7]  = (char*)"mm";
	pfind1_template[7]  = 0;
	tycom1_template[7]  = (char*)"Entrance X";
	tform1_template[8]  = (char*)FITS_TYPE_D;
	ttype1_template[8] = (char*)"Y_ENT";
	tunit1_template[8]  = (char*)"mm";
	pfind1_template[8]  = 0;
	tycom1_template[8]  = (char*)"Entrance Y";
	tform1_template[9]  = (char*)FITS_TYPE_D;
	ttype1_template[9] = (char*)"Z_ENT";
	tunit1_template[9]  = (char*)"mm";
	pfind1_template[9]  = 0;
	tycom1_template[9]  = (char*)"Entrance Z";
	tform1_template[10]  = (char*)FITS_TYPE_D;
	ttype1_template[10] = (char*)"X_EXIT";
	tunit1_template[10]  = (char*)"mm";
	pfind1_template[10]  = 0;
	tycom1_template[10]  = (char*)"Exit X";
	tform1_template[11]  = (char*)FITS_TYPE_D;
	ttype1_template[11] = (char*)"Y_EXIT";
	tunit1_template[11]  = (char*)"mm";
	pfind1_template[11]  = 0;
	tycom1_template[11]  = (char*)"Exit Y";
	tform1_template[12]  = (char*)FITS_TYPE_D;
	ttype1_template[12] = (char*)"Z_EXIT";
	tunit1_template[12]  = (char*)"mm";
	pfind1_template[12]  = 0;
	tycom1_template[12]  = (char*)"Exit Z";
	tform1_template[13]  = (char*)"1D";
	ttype1_template[13] = (char*)"E_KIN_ENT";
	tunit1_template[13]  =(char*) "keV";
	pfind1_template[13]  = 0;
	tycom1_template[13]  = (char*)"Entrance kinetic energy";
	tform1_template[14]  = (char*)"1D";
	ttype1_template[14] = (char*)"E_KIN_EXIT";
	tunit1_template[14]  =(char*) "keV";
	pfind1_template[14]  = 0;
	tycom1_template[14]  = (char*)"Exit kinetic energy";
	tform1_template[15] = (char*)FITS_TYPE_D;
	ttype1_template[15] = (char*)"MDX_ENT";
	tunit1_template[15] = (char*)"";
	pfind1_template[15] = 0;
	tycom1_template[15] = (char*)"Entrance momentum direction X";
	tform1_template[16] = (char*)FITS_TYPE_D;
	ttype1_template[16] = (char*)"MDY_ENT";
	tunit1_template[16] = (char*)"";
	pfind1_template[16] = 0;
	tycom1_template[16] = (char*)"Entrance momentum direction Y";
	tform1_template[17] = (char*)FITS_TYPE_D;
	ttype1_template[17] = (char*)"MDZ_ENT";
	tunit1_template[17] = (char*)"";
	pfind1_template[17] = 0;
	tycom1_template[17] = (char*)"Entrance momentum direction Z";
	tform1_template[18] = (char*)FITS_TYPE_D;
	ttype1_template[18] = (char*)"MDX_EXIT";
	tunit1_template[18] = (char*)"";
	pfind1_template[18] = 0;
	tycom1_template[18] = (char*)"Exit momentum direction X";
	tform1_template[19] = (char*)FITS_TYPE_D;
	ttype1_template[19] = (char*)"MDY_EXIT";
	tunit1_template[19] = (char*)"";
	pfind1_template[19] = 0;
	tycom1_template[19] = (char*)"Exit momentum direction Y";
	tform1_template[20] = (char*)FITS_TYPE_D;
	ttype1_template[20] = (char*)"MDZ_EXIT";
	tunit1_template[20] = (char*)"";
	pfind1_template[20] = 0;
	tycom1_template[20] = (char*)"Exit momentum direction Z";
	tform1_template[21]  = (char*)FITS_TYPE_D;
	ttype1_template[21] = (char*)"GTIME_ENT";
	tunit1_template[21]  = (char*)"ns";
	pfind1_template[21]  = 0;
	tycom1_template[21]  = (char*)"Entrance global time";
	tform1_template[22]  = (char*)FITS_TYPE_D;
	ttype1_template[22] = (char*)"GTIME_EXIT";
	tunit1_template[22]  = (char*)"ns";
	pfind1_template[22]  = 0;
	tycom1_template[22]  = (char*)"Exit global time";
	tform1_template[23]  = (char*)"1I";
	ttype1_template[23] = (char*)"PARTICLE_ID";
	tunit1_template[23]  = (char*)"";
	pfind1_template[23]  = 0;
	tycom1_template[23]  = (char*)"Particle ID";
    tform1_template[24] = (char*)"32A";
    ttype1_template[24] = (char*)"PARTICLE_NAME";
    tunit1_template[24] = (char*)"";
    pfind1_template[24] = 0;
    tycom1_template[24] = (char*)"Particle name";
    tform1_template[25]  = (char*)"1V";
	ttype1_template[25] = (char*)"PROCESS_ID";
	tunit1_template[25]  = (char*)"";
	pfind1_template[25]  = 0;
	tycom1_template[25]  = (char*)"Process creator id";
    tform1_template[26] = (char*)"32A";
    ttype1_template[26] = (char*)"PROCESS_NAME";
    tunit1_template[26] = (char*)"";
    pfind1_template[26] = 0;
    tycom1_template[26] = (char*)"Process creator name";
    
	//lettura del file di configurazione
	
	gm.config->readInto(colconfig[0], "IOXYZ.EVT_ID");
	gm.config->readInto(colconfig[1], "IOXYZ.TRK_ID");
    gm.config->readInto(colconfig[2], "IOXYZ.PARENT_TRK_ID");
    gm.config->readInto(colconfig[3], "IOXYZ.VOLUME_ID");
	gm.config->readInto(colconfig[4], "IOXYZ.VOLUME_NAME");
	gm.config->readInto(colconfig[5], "IOXYZ.MOTHER_ID");
	gm.config->readInto(colconfig[6], "IOXYZ.E_DEP");
	gm.config->readInto(colconfig[7], "IOXYZ.X_ENT");
	gm.config->readInto(colconfig[8], "IOXYZ.Y_ENT");
	gm.config->readInto(colconfig[9], "IOXYZ.Z_ENT");
	gm.config->readInto(colconfig[10], "IOXYZ.X_EXIT");
	gm.config->readInto(colconfig[11], "IOXYZ.Y_EXIT");
	gm.config->readInto(colconfig[12], "IOXYZ.Z_EXIT");
	gm.config->readInto(colconfig[13], "IOXYZ.E_KIN_ENT");
	gm.config->readInto(colconfig[14], "IOXYZ.E_KIN_EXIT");
	gm.config->readInto(colconfig[15], "IOXYZ.MDX_ENT");
	gm.config->readInto(colconfig[16], "IOXYZ.MDY_ENT");
	gm.config->readInto(colconfig[17], "IOXYZ.MDZ_ENT");
	gm.config->readInto(colconfig[18], "IOXYZ.MDX_EXIT");
	gm.config->readInto(colconfig[19], "IOXYZ.MDY_EXIT");
	gm.config->readInto(colconfig[20], "IOXYZ.MDZ_EXIT");
	gm.config->readInto(colconfig[21], "IOXYZ.GTIME_ENT");
	gm.config->readInto(colconfig[22], "IOXYZ.GTIME_EXIT");
	gm.config->readInto(colconfig[23], "IOXYZ.PARTICLE_ID");
    gm.config->readInto(colconfig[24], "IOXYZ.PARTICLE_NAME");
    gm.config->readInto(colconfig[25], "IOXYZ.PROCESS_ID");
    gm.config->readInto(colconfig[26], "IOXYZ.PROCESS_NAME");
	
	//gm.config->readInto(writeIN, "IOXYZ.IN");
	//gm.config->readInto(writeOUT, "IOXYZ.OUT");
	gm.config->readInto(neventsinfile, "IOXYZ.NRows");
	//gm.config->readInto(writeInitialCondition, "IOXYZ.InitialCondition");
	ncol_header1 = 0;
	int ncolw = 0;
	for(int i=0; i<NCOLS_FITS; i++) {
		if(colconfig[i] == 1) {
			tform1[ncolw] = tform1_template[i];
			ttype1[ncolw] = ttype1_template[i];
			tunit1[ncolw] = tunit1_template[i];
			pfind1[ncolw] = pfind1_template[i];
			tycom1[ncolw] = tycom1_template[i];
			G4cout << "Added column number " << ncolw << " of name " << ttype1[ncolw] << " " << tform1[ncolw] << G4endl;
			ncolw++;
		}
	}
	ncol_header1 = ncolw;
	//G4cout << "Write IN rows " << writeIN << G4endl;
	//G4cout << "Write OUT rows " << writeOUT << G4endl;
	G4cout << "Write nrows for each file " << neventsinfile << G4endl;
	
	//volume selection
	volselectionN = 0;
	gm.config->readInto(volselectionN, "IOXYZ.volumeselection.number");
	G4cout << "Volume selection XYZ: " << volselectionN << G4endl;
	char c[512];
	for(int i=0; i<volselectionN; i++) {
		sprintf(c, "IOXYZ.volumeselection.%i.start", i);
		gm.config->readInto(volselectionstart[i], c);
		G4cout << "Volume selection XYZ" << i << " start " << volselectionstart[i] << G4endl;
		sprintf(c, "IOXYZ.volumeselection.%i.end", i);
		gm.config->readInto(volselectionend[i], c);
		G4cout << "Volume selection XYZ" << i << " end " << volselectionend[i] << G4endl;
	}
	/*
	 gm.config->readInto(howmanylayers, "IOXYZ.writeTrackerLayers");
	 gm.config->readInto(trayStart, "IOXYZ.trayselection.start");
	 gm.config->readInto(trayEnd, "IOXYZ.trayselection.end");
	 
	 for(int i=0; i<howmanylayers; i++) {
	 sprintf(c, "IOXYZ.writeTrackerLayer.%i.number", i);
	 gm.config->readInto(layernumber[i], c);
	 G4cout << "Tracker layer selection XYZ:" << layernumber[i] << G4endl;
	 }
	 */
	writeonlyifenergydeposit = 0;
	gm.config->readInto(writeonlyifenergydeposit, "IOXYZ.writeonlyifenergydeposit");
	cout << "writeonlyifenergydeposit: " << writeonlyifenergydeposit << endl;
	
	removeWorldEvent = 0;
	gm.config->readInto(removeWorldEvent, "IOXYZ.removeWorldEvent");
	cout << "IOXYZ.removeWorldEvent: " << removeWorldEvent << endl;

	

}


FITSOutput_XYZ::~FITSOutput_XYZ() {

}

// bool FITSOutput_XYZ::close() {
// 	MIOOutputFileFITSBinaryTable::close();
// 	G4String ss; ss = "gzip -9 "; ss += filename;
// 	G4cout << ss << " on close " << G4endl;
//  	system(ss);
// }

bool FITSOutput_XYZ::init() {
    setFileName((char*)gm.GetOutputXYZFileNameFITS().data());
    FITSOutputFileBinaryTable::init();
    lastevent = -1;
    open();
    newfile = true;
	
    //current_event_id = -1;
    return true;
}

void FITSOutput_XYZ::writeData(CXYZHit* hitting, G4int i) {
	/*
    current_event_id = gm.event_id;

    if(nrows_event_hdu >= neventsinfile) {
        if( current_event_id != lastevent) {
            close();
            char* basename = (char*)gm.outputXYZFileNameFITS_base.data();
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
	 */
	
    // VF
    int eventHit = hitting->GetEventID();
    
	//--------- AB
	bool newevent = false;
	if(lastevent != eventHit) {
		newevent = true;
		lastevent = eventHit;
	} else
		newevent = false;

	if(nrows_event_hdu >= neventsinfile  && newevent == true) {
		if(newfile == true) {
			close();
			char* basename = (char*)gm.outputXYZFileNameFITS_base.data();
			gm.CopyFileFits(filename, basename);
			if(gm.compressionlevel != 0) {
				G4String ss;
				ss = "gzip -1 ";
				ss += basename;
				G4cout << ss << G4endl;
				system(ss);
			}
			//double s =  energystart;
			init();
			//energystart = energy[0] = s;
			open();
			newfile = false;
		}
	} else
		newfile = true;
	
	
	if(current_event_id != eventHit) {
		current_event_id = eventHit;
	}

	//------------
	LONGLONG firstelem;
    int status;
    status=0;
    firstelem = 1;
    int headnum;
    headnum = 2;
	
	if ( fits_movabs_hdu(*fptr, headnum, 0, &status) ) {
		printerror( status );
	}

    int v1 = hitting->GetVolumeID();
    bool volumein = false;
    bool layerin = false;
    bool trackerin = false;

    if(volselectionN != 0) {
        for(int i=0; i<volselectionN; i++) {
            if(v1 >= volselectionstart[i] && v1 <= volselectionend[i])
                volumein = true;
        }
    }

    long LayerRealNumber;
    G4int layer_div;
    /*
    	if(volumein) {
      	  if(howmanylayers != 0) {
                if(v1 <= trayEnd && v1 >= trayStart){
    	        trackerin = true;
    		for(int i=0; i<howmanylayers; i++) {
    		        layer_div = v1%1000;
    			if (layer_div == layernumber[i]){
    			   layerin = true;
    			}
    		}
    	     }
    	 }
    	}
    */
    if (volumein) {

        /*   if (layerin == true || trackerin == false){*/
		/*
		 if(writeonlydetectorevents > 0) {
			if (!(volumeid >= gm.detector_volmin && volumeid <= gm.detector_volmax))
		 return;
		 }
		 */
		
		double energyDep = hitting->GetEdep() / keV;
		long tpart = hitting->GetParticleID();
		
		
		if(writeonlyifenergydeposit && energyDep == 0)
			return;
		if(removeWorldEvent && tpart == 0)
			return;

        //EVT_ID
        int colnumw = 1;
        if(colconfig[0])
            if(fits_write_col(*fptr, TUINT, colnumw++, nrows_event_hdu, firstelem, (LONGLONG)1, &eventHit, &status))
                PrintErrorStatus(status, headnum, colnumw-1);
		
        //TRK_ID
		
        int tri = hitting->GetTrackID();
        if(colconfig[1])
            if(fits_write_col(*fptr, TUINT, colnumw++, nrows_event_hdu, firstelem, (LONGLONG)1, &tri, &status))
                PrintErrorStatus(status, headnum, colnumw-1);

        //PARENT_TRK_ID
        
        int par_tri = hitting->GetParentTrackID();
        if(colconfig[2])
            if(fits_write_col(*fptr, TUINT, colnumw++, nrows_event_hdu, firstelem, (LONGLONG)1, &par_tri, &status))
                PrintErrorStatus(status, headnum, colnumw-1);

        //VOLUME_ID
        if(colconfig[3])
            if(fits_write_col(*fptr, TUINT, colnumw++, nrows_event_hdu, firstelem, (LONGLONG)1, &v1, &status))
                PrintErrorStatus(status, headnum, colnumw-1);
        //VOLUME NAME
        char* aaa = (char*)hitting->GetVolumeName().data();
        if(colconfig[4]) {
            if(fits_write_col(*fptr, TSTRING, colnumw++, nrows_event_hdu, firstelem, (LONGLONG)1, &(aaa), &status))
                PrintErrorStatus(status, headnum, colnumw-1);
        }
        //MOTHERVOL_ID
        int m1 = hitting->GetMotherID();
        if(colconfig[5])
            if(fits_write_col(*fptr, TUINT, colnumw++, nrows_event_hdu, firstelem, (LONGLONG)1, &m1, &status))
                PrintErrorStatus(status, headnum, colnumw-1);

        //E_DEP
        G4double energy = hitting->GetEdep() / keV;
        if(colconfig[6])
            if(fits_write_col(*fptr, TDOUBLE, colnumw++, nrows_event_hdu, firstelem, (LONGLONG)1, &energy, &status))
                PrintErrorStatus(status, headnum, colnumw-1);

        //X_ENT
        G4_TYPE pos = hitting->GetPosEntrance()[0] / mm;
        if(colconfig[7])
            if(fits_write_col(*fptr, FITS_TYPE_D2, colnumw++, nrows_event_hdu, firstelem, (LONGLONG)1, &pos, &status))
                PrintErrorStatus(status, headnum, colnumw-1);

        //Y_ENT
        pos = hitting->GetPosEntrance()[1] / mm;
        if(colconfig[8])
            if(fits_write_col(*fptr, FITS_TYPE_D2, colnumw++, nrows_event_hdu, firstelem, (LONGLONG)1, &pos, &status))
                PrintErrorStatus(status, headnum, colnumw-1);

        //Z_ENT
        pos = hitting->GetPosEntrance()[2] / mm;
        if(colconfig[9])
            if(fits_write_col(*fptr, FITS_TYPE_D2, colnumw++, nrows_event_hdu, firstelem, (LONGLONG)1, &pos, &status))
                PrintErrorStatus(status, headnum, colnumw-1);

        //X_EXIT
        pos = hitting->GetPosExit()[0] / mm;
        if(colconfig[10])
            if(fits_write_col(*fptr, FITS_TYPE_D2, colnumw++, nrows_event_hdu, firstelem, (LONGLONG)1, &pos, &status))
                PrintErrorStatus(status, headnum, colnumw-1);

        //Y_EXIT
        pos = hitting->GetPosExit()[1] / mm;
        if(colconfig[11])
            if(fits_write_col(*fptr, FITS_TYPE_D2, colnumw++, nrows_event_hdu, firstelem, (LONGLONG)1, &pos, &status))
                PrintErrorStatus(status, headnum, colnumw-1);

        //Z_EXIT
        pos = hitting->GetPosExit()[2] / mm;
        if(colconfig[12])
            if(fits_write_col(*fptr, FITS_TYPE_D2, colnumw++, nrows_event_hdu, firstelem, (LONGLONG)1, &pos, &status))
                PrintErrorStatus(status, headnum, colnumw-1);

        //EKIN_ENT
        energy = hitting->GetEkinEntrance() / keV;
        if(colconfig[13])
            if(fits_write_col(*fptr, TDOUBLE, colnumw++, nrows_event_hdu, firstelem, (LONGLONG)1, &energy, &status))
                PrintErrorStatus(status, headnum, colnumw-1);

        //EKIN_EXIT
        energy = hitting->GetEkinExit() / keV;
        if(colconfig[14])
            if(fits_write_col(*fptr, TDOUBLE, colnumw++, nrows_event_hdu, firstelem, (LONGLONG)1, &energy, &status))
                PrintErrorStatus(status, headnum, colnumw-1);

        //MDX_ENT
        G4_TYPE momdir = hitting->GetMomDirEntrance()[0];
        if(colconfig[15])
            if(fits_write_col(*fptr, FITS_TYPE_D2, colnumw++, nrows_event_hdu, firstelem, (LONGLONG)1, &momdir, &status))
                PrintErrorStatus(status, headnum, colnumw-1);

        //MDY_ENT
        momdir = hitting->GetMomDirEntrance()[1];
        if(colconfig[16])
            if(fits_write_col(*fptr, FITS_TYPE_D2, colnumw++, nrows_event_hdu, firstelem, (LONGLONG)1, &momdir, &status))
                PrintErrorStatus(status, headnum, colnumw-1);

        //MDZ_ENT
        momdir = hitting->GetMomDirEntrance()[2];
        if(colconfig[17])
            if(fits_write_col(*fptr, FITS_TYPE_D2, colnumw++, nrows_event_hdu, firstelem, (LONGLONG)1, &momdir, &status))
                PrintErrorStatus(status, headnum, colnumw-1);

        //MDX_EXIT
        momdir = hitting->GetMomDirExit()[0];
        if(colconfig[18])
            if(fits_write_col(*fptr, FITS_TYPE_D2, colnumw++, nrows_event_hdu, firstelem, (LONGLONG)1, &momdir, &status))
                PrintErrorStatus(status, headnum, colnumw-1);

        //MDY_EXIT
        momdir = hitting->GetMomDirExit()[1];
        if(colconfig[19])
            if(fits_write_col(*fptr, FITS_TYPE_D2, colnumw++, nrows_event_hdu, firstelem, (LONGLONG)1, &momdir, &status))
                PrintErrorStatus(status, headnum, colnumw-1);

        //MDZ_EXIT
        momdir = hitting->GetMomDirExit()[2];
        if(colconfig[20])
            if(fits_write_col(*fptr, FITS_TYPE_D2, colnumw++, nrows_event_hdu, firstelem, (LONGLONG)1, &momdir, &status))
                PrintErrorStatus(status, headnum, colnumw-1);

        //GTIME_ENT
        G4_TYPE time = hitting->GetGlobalTimeEntrance()/ns;
        if(colconfig[21])
            if(fits_write_col(*fptr, FITS_TYPE_D2, colnumw++, nrows_event_hdu, firstelem, (LONGLONG)1, &time, &status))
                PrintErrorStatus(status, headnum, colnumw-1);

        //GTIME_EXIT
        time = hitting->GetGlobalTimeExit()/ns;
        if(colconfig[22])
            if(fits_write_col(*fptr, FITS_TYPE_D2, colnumw++, nrows_event_hdu, firstelem, (LONGLONG)1, &time, &status))
                PrintErrorStatus(status, headnum, colnumw-1);

        //PARTICLE_ID
        G4int pidn = hitting->GetParticleID();
        if(colconfig[23])
            if(fits_write_col(*fptr, TSHORT, colnumw++, nrows_event_hdu, firstelem, (LONGLONG)1, &pidn, &status))
                PrintErrorStatus(status, headnum, colnumw-1);

        //PARTICLE_NAME
        aaa = (char*)hitting->GetParticleType().data();
        if(colconfig[24]) {
            if(fits_write_col(*fptr, TSTRING, colnumw++, nrows_event_hdu, firstelem, (LONGLONG)1, &(aaa), &status))
                PrintErrorStatus(status, headnum, colnumw-1);
        }
 
        //PROCESS_ID
        G4int procid = hitting->GetProcessCreatorID();
        if(colconfig[25]){
            if(fits_write_col(*fptr, TUINT, colnumw++, nrows_event_hdu, firstelem, (LONGLONG)1, &procid, &status))
                PrintErrorStatus(status, headnum, colnumw-1);
        }

        //PROCESS_NAME
        aaa = (char*)hitting->GetProcessCreator().data();
        if(colconfig[26]) {
            if(fits_write_col(*fptr, TSTRING, colnumw++, nrows_event_hdu, firstelem, (LONGLONG)1, &(aaa), &status))
                PrintErrorStatus(status, headnum, colnumw-1);
        }



        nrows_event_hdu++;
        lastevent = eventHit;
		
		
		
        //}
    }
}
