/***************************************************************************
                           CRunAction.cc  -  description
                             -------------------
    begin                : 2001-2008
    copyright            : (C) 2008 R Nartallo, modified by Andrea Bulgarelli
    email                : bulgarelli@iasfbo.inaf.it
 ***************************************************************************/
//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
//
// **********************************************************************
// *                                                                    *
// *                    GEANT 4 xray_telescope advanced example         *
// *                                                                    *
// * MODULE:            CRunAction.cc                             *
// * -------                                                            *
// *                                                                    *
// * Version:           0.5                                             *
// * Date:              16/10/01                                        *
// * Author:            R Nartallo                                      *
// * Organisation:      ESA/ESTEC, Noordwijk, THe Netherlands           *
// *                                                                    *
// **********************************************************************
//
// CHANGE HISTORY
// --------------
//
// 07.11.2001 M.G. Pia
// - Modified the analysis management
// - Small design iteration
//
// 16.10.2001 R. Nartallo
// - Updated "/vis" commands to new versions
// - Clean up code to avoid 'pedantic' and 'ANSI' compiler warnings
//
// 30.11.2000 R. Nartallo
// - Add pre-processor directives to compile without analysis option
//
// 16.11.2000 A. Pfeiffer
// - Implementation of analysis manager call
//
// 06.11.2000 R.Nartallo
// - First implementation of xray_telescope Physics list
// - Based on Chandra and XMM models
//
//
// **********************************************************************

#include "G4Run.hh"
#include "G4UImanager.hh"
#include "G4VVisManager.hh"
#include "CRunAction.hh"
#include "CAnalysis.hh"
#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4UnitsTable.hh"
#include "THELGlobalMemory.hh"
#include "FITSOutput_INOUT.hh"
#include "FITSOutputFile.hh"
#include "FITSOutput_Step.hh"
#include "FITSOutput_EnergyVol.hh"
#include "RootOutput_EnergyVol.hh"
#include "RootOutput_EnergyVolEnd.hh"
#include "RootOutput_XYZ.hh"
#include "SQLliteOutput_XYZ.hh"
// VF
#include "FITSOutput_XYZ.hh"

#if defined(GEANT4_10_1) || defined(GEANT4_10_2)|| defined(GEANT4_10_3) || defined(GEANT4_10_4)
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"
#endif


CRunAction::CRunAction()
    :nEnteringTracks(0), totEnteringEnergy(0.) { }


CRunAction::~CRunAction() { }


void CRunAction::BeginOfRunAction(const G4Run* aRun) {
// 	G4RunManager::GetRunManager()->SetRandomNumberStore(true);
    G4int runN = aRun->GetRunID();
    if ( runN % 1000 == 0 )
        G4cout << "### Run : " << runN << G4endl;

    if (G4VVisManager::GetConcreteInstance()) {
        G4UImanager* UI = G4UImanager::GetUIpointer();
        UI->ApplyCommand("/vis/scene/notifyHandlers");
    }

    nEnteringTracks = 0;
    totEnteringEnergy = 0.;

    // Book histograms and ntuples
    CAnalysis* analysis = CAnalysis::getInstance();
    analysis->book();
}


void CRunAction::EndOfRunAction(const G4Run* ) {
    CAnalysis* analysis = CAnalysis::getInstance();
    analysis->finish();

    if(gm.enableWriteFITS && gm.enableWriteINOUT) {
        gm.fits_inout->close();
        char* filename = (char*)gm.outputStepBoundaryFileNameFITS.data();
        char* basename = (char*)gm.outputStepBoundaryFileNameFITS_base.data();
        gm.CopyFileFits(filename, basename);
        G4String ss;
        ss = "gzip -9 ";
        //ss += gm.outputStepBoundaryFileNameFITS;
        ss += basename;
        G4cout << ss << G4endl;
        system(ss);
    }
    //VF
    if(gm.enableWriteFITS && gm.enableWriteXYZ) {
        gm.fits_xyz->close();
        char* filename = (char*)gm.outputXYZFileNameFITS.data();
        char* basename = (char*)gm.outputXYZFileNameFITS_base.data();
        gm.CopyFileFits(filename, basename);
        G4String ss;
        ss = "gzip -9 ";
        //ss += gm.outputStepBoundaryFileNameFITS;
        ss += basename;
        G4cout << ss << G4endl;
        system(ss);
    }
    if(gm.enableWriteFITS && gm.enableWriteEnergyVol) {
        gm.fits_energyvol->close();
        char* filename = (char*)gm.outputEnergyVolFileNameFITS.data();
        char* basename = (char*)gm.outputEnergyVolFileNameFITS_base.data();
        gm.CopyFileFits(filename, basename);
        G4String ss;
        ss = "gzip -9 ";
        //ss += gm.outputEnergyVolFileNameFITS;
        ss += basename;
        G4cout << ss << G4endl;
        system(ss);
    }
    if(gm.enableWriteFITS && gm.enableWriteSTEP) {
        gm.fits_step->close();
        char* filename = (char*)gm.outputHitFileNameFITS.data();
        char* basename = (char*)gm.outputHitFileNameFITS_base.data();
        gm.CopyFileFits(filename, basename);
        G4String ss;
        ss = "gzip -9 ";
        //ss += gm.outputHitFileNameFITS;
        ss += basename;
        G4cout << ss << G4endl;
        system(ss);
    }
#ifndef WITHOUTROOT
    if(gm.enableWriteRoot && gm.enableWriteEnergyVol) {
        gm.root_energyvol->close();

    }
    if(gm.enableWriteRootEnd  && gm.enableWriteEnergyVol) {
        gm.root_energyvolend->close();
    }
	if(gm.enableWriteRoot && gm.enableWriteXYZ) {
		gm.root_xyz->close();
	}
#endif

#ifdef SQLITE3
	if(gm.enableWriteSQlite3 && gm.enableWriteXYZ) {
		gm.sqlite3_xyz->close();
	}
#endif

    if(gm.enableWriteFITSEnd && gm.enableWriteEnergyVol) {
        gm.fits_energyvolend->close();
    }


    if (G4VVisManager::GetConcreteInstance())
        G4UImanager::GetUIpointer()->ApplyCommand("/vis/viewer/update");

    G4cout << "End of Run summary" << G4endl << G4endl;

    G4cout << "Total Entering Detector : " << nEnteringTracks  << G4endl;
    G4cout << "Total Entering Detector Energy : "
           << totEnteringEnergy/MeV
           << " MeV"
           << G4endl;
    if(gm.fits_inout) gm.fits_inout->close();
}


void CRunAction::Update(G4double energy) {
    nEnteringTracks++;
    totEnteringEnergy += energy;
}
























