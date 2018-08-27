/***************************************************************************
                          RootOutput_EnergyVol.cc  -  description
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

#ifndef WITHOUTROOT

#include "RootOutput_EnergyVol.hh"

#include "CSteppingAction.hh"
#include "THELGlobalMemory.hh"
#include "ConfigFile.hh"
//ricordati di indicare il numero delle colonne
// #define Root_TYPE_D "1D"
// #define Root_TYPE_D2 TDOUBLE



#define NMAXVOLFOREVENT 100000

using namespace std;

//#define ADDCOLSTXT

//possibile parametri di formattazione del file EnergyVol
//1) write position
//2) write momentum
//3) write momentum direction
//4) write element ID
//5) write element name
//6) don't write input event
//7) don't write output event
RootOutput_EnergyVol::RootOutput_EnergyVol() : IOEnergyBase() {
    current_event_id = -1;
}


RootOutput_EnergyVol::~RootOutput_EnergyVol() {
    cout << "RootOutput_EnergyVol::~RootOutput_EnergyVol()" << endl;
}



bool RootOutput_EnergyVol::init() {
    cout << "RootOutput_EnergyVol::init()" << endl;
    ResetData();
    filename = ((char*)gm.GetOutputEnergyVolFileNameRoot().data());

    cout << "Root filename " <<  filename << endl;

    IOEnergyBase::initConf();

    lastevent = -1;

    if(gm.enableWriteXrootd == false)
        fileout = new TFile(filename, "recreate");
    if(gm.enableWriteXrootd == true)
        fileoutx = new TXNetFile(filename, "recreate");
// 	fileout->SetCompressionLevel(0);
    treeout = new TTree("ENERGYDEP", "");
    treeout->Branch("EVTID", &evtid, "EVTID/I");
    treeout->Branch("EDEP", &energyDep, "EDEP/D");
    treeout->Branch("VOLID", &volume, "VOLID/I");
    treeout->Branch("TRKID", &ttrkid, "TRKID/I");
    treeout->Branch("PART", &tpart, "PART/b");
    treeout->Branch("PROC", &tproc, "PROC/b");
    treeout->Branch("MULTI", &tmultI, "MULTI/s");
    treeout->Branch("MULTO", &tmultO, "MULTO/s");
    newfile = true;
    nrows_file = 0;
    cout << "Root init end" <<  endl;
    return true;
}



void RootOutput_EnergyVol::writeData() {
// 	cout << "void RootOutput_EnergyVol::writeData() " << endl;

	double energyCheck = 0;
    bool newevent = false;
    if(lastevent != gm.event_id) {
        newevent = true;
        lastevent = gm.event_id;
    } else
        newevent = false;

    if(newevent == false)
        return;

    if(nrows_file >= nmaxevents  && newevent == true) {
        if(newfile == true) {
            close();
            double s =  energystart;
            init();
            energystart = energy[0] = s;
// 			open();
            newfile = false;
        }
    } else
        newfile = true;


    if(current_event_id != gm.event_id) {
        current_event_id = gm.event_id;
    }
    bool write = true;

	energyCheck = 0;
    for(int i=0; i<NMAXVOLFOREVENT; i++) {
        if(volumeID[i] != -1 ) {
            volume = volumeID[i];
			if(writeonlydetectorevents > 0) {
				if (!(volumeID[i] >= gm.detector_volmin && volumeID[i] <= gm.detector_volmax))
					continue;
			}
            energyDep = energy[i];

            evtid = gm.event_id;
            ttrkid = trkid[i];
            tpart = particle[i];
			//if(particle[i]>0)
			//	energyCheck += energy[i];
			if(writeonlyifenergydeposit && energyDep == 0)
				continue;
			if(removeWorldEvent && particle[i] == 0)
				continue;
            tproc = process[i];
            tmultI = multI[i];
            tmultO = multO[i];
            treeout->Fill();
            nrows_file++;
			//if(energyCheck > 3001)
			//	cout << "@ " << (double)energyCheck << " tmultI " << tmultI << " tmultO " << tmultO << endl;
        }
        else
            break;
    }
    ResetData();
}

bool RootOutput_EnergyVol::close() {
// 	cout << "bool RootOutput_EnergyVol::close() " << endl;
    treeout->Write();
    if(gm.enableWriteXrootd == false)
        fileout->Close();
    if(gm.enableWriteXrootd == true)
        fileoutx->Close();
    cout << "Evt id: " << gm.event_id << endl;
	return true;
}

#endif
