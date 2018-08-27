/***************************************************************************
                          CEventAction.cc  -  description
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
// * MODULE:            CEventAction.cc                           *
// * -------                                                            *
// *                                                                    *
// * Version:           0.4                                             *
// * Date:              06/11/00                                        *
// * Author:            R Nartallo                                      *
// * Organisation:      ESA/ESTEC, Noordwijk, THe Netherlands           *
// *                                                                    *
// **********************************************************************
//
// CHANGE HISTORY
// --------------
//
// 05.12.2001 R.Nartallo
// - Implement "Update" function as in LowEnTest
//
// 06.11.2000 R.Nartallo
// - First implementation of xray_telescope event action
// - Based on Chandra and XMM models
//
//
// **********************************************************************

#include "G4ios.hh"
#include "G4Event.hh"
#include "G4EventManager.hh"
#include "G4HCofThisEvent.hh"
#include "G4TrajectoryContainer.hh"
#include "G4Trajectory.hh"
#include "G4VVisManager.hh"
#include "G4UImanager.hh"
#include "G4UnitsTable.hh"
#include "CEventAction.hh"
#include "CEventActionMessenger.hh"
#include "THELGlobalMemory.hh"
#include "G4SDManager.hh"
#include "CHit.hh"
#include "CXYZHit.hh"
#include <iostream>
#include "FITSOutput_Step.hh"
#include "FITSOutput_INOUT.hh"
#include "FITSOutput_EnergyVol.hh"
#include "RootOutput_EnergyVol.hh"
#include "RootOutput_EnergyVolEnd.hh"

//VF
#include "FITSOutput_XYZ.hh"

using namespace std;

CEventAction::CEventAction()
    : drawFlag("all"),eventMessenger(0),drawEvent(false) {
    eventMessenger = new CEventActionMessenger(this);
}


CEventAction::~CEventAction() {
    delete eventMessenger;
}


void CEventAction::BeginOfEventAction(const G4Event* evt) {
    drawEvent = false;
    G4int event_id = evt->GetEventID();
    gm.event_id = event_id;

}


void CEventAction::EndOfEventAction(const G4Event* evt) {

    // 	if(gm.enableWriteFITS) {
    // 		if(gm.enableWriteINOUT)
    // 			gm.fits_inout->close();
    // 		if(gm.enableWriteSTEP)
    // 			gm.fits_step->close();
    // 	}

    //==============================================================
    //visualizzazione dell'evento
    if (drawEvent) {
        const G4Event* evt = fpEventManager->GetConstCurrentEvent();

        G4TrajectoryContainer * trajectoryContainer = evt->GetTrajectoryContainer();
        G4int n_trajectories = 0;
        if ( trajectoryContainer ) {
            n_trajectories = trajectoryContainer->entries();
        }

        #if defined(GEANT4_9_1) || defined(GEANT4_9_4) || defined(GEANT4_9_6)
            if ( G4VVisManager::GetConcreteInstance() ) {
                for ( G4int i=0; i<n_trajectories; i++ ) {
                    G4Trajectory* trj = (G4Trajectory*)(*(evt->GetTrajectoryContainer()))[i];
                    if ( drawFlag == "all" ) trj->DrawTrajectory(50);
                    else if ( (drawFlag == "charged")&&(trj->GetCharge() > 0.) )
                        trj->DrawTrajectory(50);
                    trj->ShowTrajectory();
                }
            }
        #endif
        #ifdef GEANT4_10_1
            if ( G4VVisManager::GetConcreteInstance() ) {
                for ( G4int i=0; i<n_trajectories; i++ ) {
                    G4Trajectory* trj = (G4Trajectory*)(*(evt->GetTrajectoryContainer()))[i];
                    if ( drawFlag == "all" ) trj->DrawTrajectory();
                    else if ( (drawFlag == "charged")&&(trj->GetCharge() > 0.) )
                        trj->DrawTrajectory();
                        trj->ShowTrajectory();
                }
            }
        #endif
        
    }
    if(gm.enableWriteFITS && gm.enableWriteEnergyVol) {
        gm.fits_energyvol->open();
        gm.fits_energyvol->writeData();
    }
	
#ifndef WITHOUTROOT
 
    if(gm.enableWriteRoot && gm.enableWriteEnergyVol) {

        gm.root_energyvol->writeData();

    }
    if(gm.enableWriteRootEnd  && gm.enableWriteEnergyVol) {

        gm.root_energyvolend->writeData();
    }
#endif
    if(gm.enableWriteFITSEnd && gm.enableWriteEnergyVol) {

        gm.fits_energyvolend->writeData();
    }

    //==============================================================
    //salvataggio dei dati dell'evento
    // 	cout << "Event " << evt->GetEventID() << " " << evt->GetPrimaryVertex()->GetPrimary()->GetMass() << endl;
    // 	G4int event_id = evt->GetEventID();
    //
    // 	// get number of stored trajectories
    // 	G4TrajectoryContainer* trajectoryContainer = evt->GetTrajectoryContainer();
    // 	G4int n_trajectories = 0;
    // 	if (trajectoryContainer) n_trajectories = trajectoryContainer->entries();
    // 	// periodic printing
    //  	if (event_id < 100 || event_id%100 == 0) {
    // 		G4cout << ">>> End event " << evt->GetEventID() << G4endl;
    // 		G4cout << "    " << n_trajectories
    // 				<< " trajectories stored in this event." << G4endl;
    // 	}

}


void CEventAction::Update() {
    // Update the flag identifying the event to be visualised
    drawEvent = true;
}












































