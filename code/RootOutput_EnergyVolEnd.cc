/***************************************************************************
                          RootOutput_EnergyVolEnd.cc  -  description
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

#include "RootOutput_EnergyVolEnd.hh"

#include "CSteppingAction.hh"
#include "THELGlobalMemory.hh"
#include "ConfigFile.hh"

using namespace std;

//possibile parametri di formattazione del file EnergyVol
//1) write position
//2) write momentum
//3) write momentum direction
//4) write element ID
//5) write element name
//6) don't write input event
//7) don't write output event
RootOutput_EnergyVolEnd::RootOutput_EnergyVolEnd() : IOEnergyBase() {
    CreateDataBuffer();
    current_event_id = -1;
}


RootOutput_EnergyVolEnd::~RootOutput_EnergyVolEnd() {
    cout << "RootOutput_EnergyVolEnd::~RootOutput_EnergyVolEnd()" << endl;
}



bool RootOutput_EnergyVolEnd::init() {
    cout << "RootOutput_EnergyVolEnd::init()" << endl;
    ResetDataBuffer();
    filename = ((char*)gm.GetOutputEnergyVolFileNameRoot().data());

    cout << " filename " <<  filename << " " << nmaxevents << endl;

//	gm.config->readInto(neventsinfile, "IOENERGY.NRows");
    IOEnergyBase::initConf();
    lastevent = -1;
    cout << " init end" <<  endl;
    return true;
}


//si chiama alla fine di ogni evento. Se raggiungo il numero massimo di eventi salvo tutto
void RootOutput_EnergyVolEnd::writeData() {

    CopyBuffer(); //copia ogni evento da scrivere
    ResetData();
    if((writeonlydetectorevents == 0 && Aindex > nmaxevents) || ( writeonlydetectorevents != 0 && neventsstored_forthisfile >= writeonlydetectorevents ) ) {
        close();
        filename = ((char*)gm.GetOutputEnergyVolFileNameRoot().data());
        cout << " filename " <<  filename << " " << nmaxevents <<  " (" << Aindex << ")" << endl;
        ResetDataBuffer();
    }
}

bool RootOutput_EnergyVolEnd::close() {
// 	return true;
// 	cout << "bool RootOutput_EnergyVolEnd::write() " << endl;

    if(gm.enableWriteXrootd == false) {
        G4cout << "Open TFile " << filename << G4endl;
        fileout = new TFile(filename, "recreate");
    }
    if(gm.enableWriteXrootd == true) {
        G4cout << "Open TXNetFile " << filename << G4endl;
        fileoutx = new TXNetFile(filename, "recreate");
    }
    treeout = new TTree("ENERGYDEP", "");
    treeout->Branch("EVTID", &evtid, "EVTID/I");
    treeout->Branch("TRKID", &ttrkid, "TRKID/I");
    treeout->Branch("VOLID", &volume, "VOLID/I");
    treeout->Branch("EDEP", &energyDep, "EDEP/D");
    treeout->Branch("PART", &tpart, "PART/b");
    treeout->Branch("PROC", &tproc, "PROC/b");
    treeout->Branch("MULTI", &tmultI, "MULTI/s");
    treeout->Branch("MULTO", &tmultO, "MULTO/s");


    for(long i=0; i<Aindex; i++) {
        volume = AvolumeID[i];
// 		if(volume == 0)
// 			continue;
        energyDep = (float)Aenergy[i];
        energyStart = (float)Aenergystart[i];
        evtid = Aevent_id[i];
        ttrkid = Atrkid[i];
        tpart = Aparticle[i];
        tproc = Aprocess[i];
        tmultI = AmultI[i];
        tmultO = AmultO[i];

//		if(energyDep > energyStart)
//			G4cout << "Problem 1 in " << evtid << " " << volume << G4endl;
//		if(i>0 && Aenergy[i-1] == Aenergy[i] && AvolumeID[i-1] != 0)
//				 G4cout << "Problem 2 in " << evtid << " " << volume << G4endl;

        treeout->Fill();
    }

    treeout->Write();

    treeeve = new TTree("NEVENTS", "");
    treeeve->Branch("NEVENTS", &neve, "NEVENTS/I");
    neve = neventsprocessed_forthisfile;
    treeeve->Fill();
    treeeve->Write();

    if(gm.enableWriteXrootd == false)
        fileout->Close();
    if(gm.enableWriteXrootd == true) {
        G4cout << "Close TXNetFile " << filename << G4endl;
        fileoutx->Write();
        fileoutx->Close();
        delete fileoutx;
    }

    cout << "Evt id: " << gm.event_id << endl;
	return true;
}

#endif
