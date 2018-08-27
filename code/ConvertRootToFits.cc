/***************************************************************************
	ConvertRootToFits.cc  -  description
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

#ifndef WITHOUTROOT


#include "ConvertRootToFits.hh"
#include <TFile.h>
#include <TTree.h>
#include "FITSOutput_EnergyVolEnd.hh"
#include "THELGlobalMemory.hh"

ConvertRootToFits::ConvertRootToFits()
{
}


ConvertRootToFits::~ConvertRootToFits()
{
}


void ConvertRootToFits::Convert(char* fileroot) {
    TFile f1(fileroot);
    TTree* tt1 = (TTree*) f1.Get("ENERGYDEP");
    if(tt1 == 0) {
        cout << "### ENERGYVOL tree error. EXIT."<<endl;
        return;
    }

    Int_t neve = tt1->GetEntries();

    int evtid;
    double energyDep;
    int volume;
    int ttrkid;
    byte tpart;
    byte tproc;
    ushort tmultI;
    ushort tmultO;

    tt1->SetBranchAddress("EVTID", &evtid);
    tt1->SetBranchAddress("EDEP", &energyDep);
    tt1->SetBranchAddress("VOLID", &volume);
    tt1->SetBranchAddress("TRKID", &ttrkid);
    tt1->SetBranchAddress("PART", &tpart);
    tt1->SetBranchAddress("PROC", &tproc);
    tt1->SetBranchAddress("MULTI", &tmultI);
    tt1->SetBranchAddress("MULTO", &tmultO);

    FITSOutput_EnergyVolEnd* fits_energyvolend = new FITSOutput_EnergyVolEnd();
    fits_energyvolend->init();

// 	fits_energyvolend->neventsinfile = fits_energyvolend->neventsinfile * 2;

    for(long i=0; i<neve; i++) {
        tt1->GetEntry(i);
        fits_energyvolend->setBuffer(evtid, energyDep, volume, ttrkid, tpart, tproc, tmultI, tmultO);
// 		G4cout << evtid << G4endl;
    }

    fits_energyvolend->writeData();
    fits_energyvolend->Aindex--;
    fits_energyvolend->close();
    f1.Close();
}

#endif