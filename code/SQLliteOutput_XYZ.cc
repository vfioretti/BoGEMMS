/***************************************************************************
                          SQLliteOutput_XYZ.cc  -  description
                             -------------------
    begin                : 2017
    copyright            : (C) 2017 by  Andrea Bulgarelli
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

#ifdef SQLITE3

#include "SQLliteOutput_XYZ.hh"

#include "THELGlobalMemory.hh"
#include "ConfigFile.hh"
#include "CXYZHit.hh"

#if defined(GEANT4_10_1) || defined(GEANT4_10_2) || defined(GEANT4_10_3) || defined(GEANT4_10_4)
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"
#endif

using namespace std;

#define G4_TYPE G4float


SQLliteOutput_XYZ::SQLliteOutput_XYZ()  {

    current_event_id = -1;
	gm.config->readInto(nmaxevents, "IOXYZ.NRows");
	G4cout << "Write nrows for each file " << nmaxevents << G4endl;

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

	writeonlyifenergydeposit = 0;
	gm.config->readInto(writeonlyifenergydeposit, "IOXYZ.writeonlyifenergydeposit");
	cout << "writeonlyifenergydeposit: " << writeonlyifenergydeposit << endl;
	
	removeWorldEvent = 0;
	gm.config->readInto(removeWorldEvent, "IOXYZ.removeWorldEvent");
	cout << "IOXYZ.removeWorldEvent: " << removeWorldEvent << endl;
	
	//INIT
	
	//cout << "SQLliteOutput_XYZ::init()" << endl;

    char *err_msg = 0;
    
    int rc = sqlite3_open("test.db", &db);

	if (rc != SQLITE_OK) {
        
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        close();
        

    }
    

    //cout << "Root filename " <<  filename << endl;

    lastevent = -1;

	char* sql="CREATE TABLE XYZ(EVTID INTEGER, TRKID INTEGER, PARENTTRKID INTEGER, VOLID INTEGER, MOTHERID INTEGER, EDEP REAL, X_ENT REAL, Y_ENT REAL, Z_ENT REAL, X_EXIT REAL, Y_EXIT REAL, Z_EXIT REAL, EKIN_ENT REAL, EKIN_EXIT REAL, GTIME_ENT REAL, GTIME_EXIT REAL, PARTID INTEGER, PROCID INTEGER )";
	
	rc = sqlite3_exec(db, sql, 0, 0, &err_msg);
    
    if (rc != SQLITE_OK ) {
        
        fprintf(stderr, "SQL error: %s\n", err_msg);
        
        sqlite3_free(err_msg);        
        close();
        

    } 
    rc = sqlite3_exec(db, "BEGIN TRANSACTION", NULL, NULL, &err_msg);
    
	if (rc != SQLITE_OK ) {

		fprintf(stderr, "SQL error: %s\n", err_msg);

		sqlite3_free(err_msg);        
		close();


	} 
	cout << "SQLite: BEGIN TRANSACTION START " << nmaxevents << endl;
		
    /*
    rc = sqlite3_exec(db, "PRAGMA synchronous = OFF", NULL, NULL, &err_msg);
    
    if (rc != SQLITE_OK ) {
        
        fprintf(stderr, "SQL error: %s\n", err_msg);
        
        sqlite3_free(err_msg);        
        close();
        
        return 1;
    } 
    */
    
    newfile = true;
    nrows_file = 0;

}


SQLliteOutput_XYZ::~SQLliteOutput_XYZ() {
    cout << "SQLliteOutput_XYZ::~SQLliteOutput_XYZ()" << endl;
}



bool SQLliteOutput_XYZ::init() {
	
    cout << "SQLlite init end" <<  endl;
    return true;
}



void SQLliteOutput_XYZ::writeData(CXYZHit* hitting, G4int i) {
	cout << "H"; 
	char *err_msg = 0;
	
	long evtid;
	long ttrkid;
	long parenttrkid;
	long volumeid;
	long motherid;
	double energyDep;
	double x_ent;
	double y_ent;
	double z_ent;
	double x_exit;
	double y_exit;
	double z_exit;
	double ekin_ent;
	double ekin_exit;
	long tpart;
	long tproc;
	double gtime_ent;
	double gtime_exit;
	
	/*
    bool newevent = false;
    if(lastevent != gm.event_id) {
        newevent = true;
        
        lastevent = gm.event_id;
    } else
        newevent = false;
	*/


    if(nrows_file >= nmaxevents ) {
        
        	int rc = 0;
        	rc = sqlite3_exec(db, "END TRANSACTION", NULL, NULL, &err_msg);
			if (rc != SQLITE_OK ) {
		
				fprintf(stderr, "SQL error: %s\n", err_msg);
		
				sqlite3_free(err_msg);        
				close();
		
				return;
			} 
			cout << "SQLite: END TRANSACTION: " << nrows_file << " of " << nmaxevents << endl;
        	rc = sqlite3_exec(db, "BEGIN TRANSACTION", NULL, NULL, &err_msg);
			if (rc != SQLITE_OK ) {
		
				fprintf(stderr, "SQL error: %s\n", err_msg);
		
				sqlite3_free(err_msg);        
				close();
		
				return;
			} 
			cout << "SQLite: BEGIN TRANSACTION" << endl;
            //close();
            //double s =  energystart;
            //init();
           // energystart  = s;
// 			open();
 //           newfile = false;
            nrows_file = 0;
        
    } 

	/*
    if(current_event_id != gm.event_id) {
        current_event_id = gm.event_id;
    }
    */
    bool write = true;
    
    evtid = gm.event_id; //1

	long v1 = hitting->GetVolumeID();
	bool volumein = false;
	bool layerin = false;
	bool trackerin = false;
	
	if(volselectionN != 0) {
		for(int ii=0; ii<volselectionN; ii++) {
			if(v1 >= volselectionstart[ii] && v1 <= volselectionend[ii])
				volumein = true;
		}
	}
	
	long LayerRealNumber;
	G4int layer_div;
	
	if (volumein) {
		
		
		volumeid = v1; //2
		
		/*
		if(writeonlydetectorevents > 0) {
			if (!(volumeid >= gm.detector_volmin && volumeid <= gm.detector_volmax))
				return;
		}
		*/
		
		energyDep = hitting->GetEdep() / keV; //3
		tpart = hitting->GetParticleID(); //4
		
		//tpart=1 e' lo stepping volume per registrare l'energia generata dal GPS
		if(volumeid != 1) {
			if(writeonlyifenergydeposit && energyDep == 0)
				return;
			if(removeWorldEvent && tpart == 0)
				return;
		}
		
		

		
		ttrkid = hitting->GetTrackID(); //5
        parenttrkid = hitting->GetParentTrackID();
        motherid = hitting->GetMotherID();
		x_ent = hitting->GetPosEntrance()[0] / mm;
		y_ent = hitting->GetPosEntrance()[1] / mm;
		z_ent = hitting->GetPosEntrance()[2] / mm;
		x_exit =  hitting->GetPosExit()[0] / mm;
		y_exit =  hitting->GetPosExit()[1] / mm;
		z_exit =  hitting->GetPosExit()[2] / mm;
		ekin_ent = hitting->GetEkinEntrance() / keV;
		ekin_exit = hitting->GetEkinExit() / keV;
		tproc = hitting->GetProcessCreatorID();
		
		G4_TYPE time = hitting->GetGlobalTimeEntrance()/ns;
		gtime_ent = time;
		time = hitting->GetGlobalTimeExit()/ns;
		gtime_exit = time; //18
		
		char query[5000];
		int rc = 0;
		//CREATE TABLE XYZ(EVTID INTEGER, TRKID INTEGER, PARENTTRKID INTEGER, VOLID INTEGER, MOTHERID INTEGER, EDEP REAL, X_ENT REAL, Y_ENT REAL, Z_ENT REAL, X_EXIT REAL, Y_EXIT REAL, Z_EXIT REAL, EKIN_ENT REAL, EKIN_EXIT REAL, GTIME_ENT REAL, GTIME_EXIT REAL, PARTID INTEGER, PROCID INTEGER )
		
		sprintf(query, "INSERT INTO XYZ VALUES(%d, %d, %d, %d, %d, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %d, %d)", evtid, ttrkid,  parenttrkid, volumeid, motherid, energyDep, x_ent, y_ent, z_ent, x_exit, y_exit, z_exit, ekin_ent, ekin_exit, gtime_ent, gtime_exit, tpart, tproc);
		//cout << query << endl;
	
		rc = sqlite3_exec(db, query, 0, 0, &err_msg);
	
		if (rc != SQLITE_OK ) {
		
			fprintf(stderr, "SQL error: %s\n", err_msg);
		
			sqlite3_free(err_msg);        
			close();
		
			return ;
		} 		
		
		nrows_file++;
	}
}

bool SQLliteOutput_XYZ::close() {
	char *err_msg = 0;
	int rc = 0;
	rc = sqlite3_exec(db, "END TRANSACTION", NULL, NULL, &err_msg);
	if (rc != SQLITE_OK ) {

		fprintf(stderr, "SQL error: %s\n", err_msg);

		sqlite3_free(err_msg);        
		close();

		return 0;
	} 
	cout << "SQLite: END TRANSACTION" << endl;		
// 	cout << "bool SQLliteOutput_XYZ::close() " << endl;
    sqlite3_close(db);
    cout << "SQLite: close DB" << endl;	
    cout << "Evt id: " << gm.event_id << endl;
	return true;
}

#endif
