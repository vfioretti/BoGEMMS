/***************************************************************************
                          RootOutput_XYZ.cc  -  description
                             -------------------
    begin                : 2014
    copyright            : (C) 2014 by  Andrea Bulgarelli
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

#ifndef WITHOUTROOT

#include "RootOutput_XYZ.hh"

#include "THELGlobalMemory.hh"
#include "ConfigFile.hh"
#include "CXYZHit.hh"

#if defined(GEANT4_10_1) || defined(GEANT4_10_2) || defined(GEANT4_10_3) || defined(GEANT4_10_4)
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"
#endif

using namespace std;

#define G4_TYPE G4float


RootOutput_XYZ::RootOutput_XYZ()  {
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

}


RootOutput_XYZ::~RootOutput_XYZ() {
    cout << "RootOutput_XYZ::~RootOutput_XYZ()" << endl;
}



bool RootOutput_XYZ::init() {
    cout << "RootOutput_XYZ::init()" << endl;

    filename = ((char*)gm.GetOutputXYZFileNameRoot().data());

    cout << "Root filename " <<  filename << endl;

    lastevent = -1;

    if(gm.enableWriteXrootd == false)
        fileout = new TFile(filename, "recreate");
    if(gm.enableWriteXrootd == true)
        fileoutx = new TXNetFile(filename, "recreate");
// 	fileout->SetCompressionLevel(0);
	
	//QUI DA FARE IL TREE
    treeout = new TTree("XYZ", "");
    treeout->Branch("EVTID", &evtid, "EVTID/I");
	treeout->Branch("TRKID", &ttrkid, "TRKID/I");
    treeout->Branch("PARENTTRKID", &parenttrkid, "PARENTTRKID/I");
    treeout->Branch("VOLID", &volumeid, "VOLID/I");
    treeout->Branch("MOTHERID", &motherid, "MOTHERID/I");
	treeout->Branch("EDEP", &energyDep, "EDEP/D");
	treeout->Branch("X_ENT", &x_ent, "X_ENT/D");
	treeout->Branch("Y_ENT", &y_ent, "Y_ENT/D");
	treeout->Branch("Z_ENT", &z_ent, "Z_ENT/D");
	treeout->Branch("X_EXIT", &x_exit, "X_EXIT/D");
	treeout->Branch("Y_EXIT", &y_exit, "Y_EXIT/D");
	treeout->Branch("Z_EXIT", &z_exit, "Z_EXIT/D");
	treeout->Branch("EKIN_ENT", &ekin_ent, "EKIN_ENT/D");
	treeout->Branch("EKIN_EXIT", &ekin_exit, "EKIN_EXIT/D");
	treeout->Branch("GTIME_ENT", &gtime_ent, "GTIME_ENT/D");
	treeout->Branch("GTIME_EXIT", &gtime_ent, "GTIME_EXIT/D");
    treeout->Branch("PARTID", &tpart, "PARTID/I");
    treeout->Branch("PROCID", &tproc, "PROCID/I");
   
    newfile = true;
    nrows_file = 0;
	
    cout << "Root init end" <<  endl;
    return true;
}



void RootOutput_XYZ::writeData(CXYZHit* hitting, G4int i) {

    bool newevent = false;
    if(lastevent != gm.event_id) {
        newevent = true;
        lastevent = gm.event_id;
    } else
        newevent = false;

   // if(newevent == false)
    //    return;

    if(nrows_file >= nmaxevents  && newevent == true) {
        if(newfile == true) {
            close();
            double s =  energystart;
            init();
            energystart  = s;
// 			open();
            newfile = false;
        }
    } else
        newfile = true;


    if(current_event_id != gm.event_id) {
        current_event_id = gm.event_id;
    }
    bool write = true;

	long v1 = hitting->GetVolumeID();
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
	
	if (volumein) {
		
		evtid = gm.event_id;
		volumeid = v1;
		
		/*
		if(writeonlydetectorevents > 0) {
			if (!(volumeid >= gm.detector_volmin && volumeid <= gm.detector_volmax))
				return;
		}
		*/
		
		energyDep = hitting->GetEdep() / keV;
		tpart = hitting->GetParticleID();
		
		//tpart=1 e' lo stepping volume per registrare l'energia generata dal GPS
		if(volumeid != 1) {
			if(writeonlyifenergydeposit && energyDep == 0)
				return;
			if(removeWorldEvent && tpart == 0)
				return;
		}
		
		ttrkid = hitting->GetTrackID();
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
		gtime_exit = time;
		
		treeout->Fill();
		nrows_file++;
	}
}

bool RootOutput_XYZ::close() {
// 	cout << "bool RootOutput_XYZ::close() " << endl;
    treeout->Write();
    if(gm.enableWriteXrootd == false)
        fileout->Close();
    if(gm.enableWriteXrootd == true)
        fileoutx->Close();
    cout << "Evt id: " << gm.event_id << endl;
	return true;
}

#endif
