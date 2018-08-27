/***************************************************************************
                           CSteppingAction.cc  -  description
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
// * MODULE:            CSteppingAction.cc                        *
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
// 05.12.2001 R. Nartallo
// - Return condition for entering detector (cf LowEnTest)
// - Remove line to kill track if too many steps
//
// 07.11.2001 M.G. Pia
// - Modified the analysis management
// - Small design iteration
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
// **********************************************************************

#include "CSteppingAction.hh"

#include "G4Track.hh"
#include "G4Step.hh"
#include "G4RunManager.hh"
#include "CEventAction.hh"
#include "CAnalysis.hh"
#include <iostream>
#include "Randomize.hh"
#include <fstream>
#include <iomanip>
#include "THELGlobalMemory.hh"
#include "FITSOutput_INOUT.hh"
#include "FITSOutput_EnergyVol.hh"
#include "RootOutput_EnergyVol.hh"
#include "RootOutput_EnergyVolEnd.hh"
#include <fitsio.h>
#include "G4TouchableHistory.hh"

//VF
#include "FITSOutput_XYZ.hh"
#include "CXYZHit.hh"
#include "G4VTouchable.hh"
#include "G4SteppingManager.hh"

#if defined(GEANT4_10_1) || defined(GEANT4_10_2) || defined(GEANT4_10_3) || defined(GEANT4_10_4)
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"
#endif


using namespace std;

CSteppingAction::CSteppingAction() {
}


CSteppingAction::~CSteppingAction() {
    // 	gm.fits_inout->close();
}


void CSteppingAction::UserSteppingAction(const G4Step* step) {


    G4bool entering = false;
    /*
    G4Track* track = step->GetTrack();
    // 	cout << "[" << gm.event_id << "]";
    G4String volName;
    if (track->GetVolume()) volName =  track->GetVolume()->GetName();
    G4String nextVolName;
    if (track->GetNextVolume()) nextVolName =  track->GetNextVolume()->GetName();
    */
    // 	cout << volName << " " << nextVolName << endl;
    // Entering Detector
    // 	if (volName != "SA_P" && nextVolName == "SA_P")
    //   	if(true)

    /*if(volName != nextVolName && nextVolName == "Baffle_P_3")
    {
    cout << "#####Entering: " << volName << " " << nextVolName << endl;
    entering = true;

    // Notify the corresponding UserAction that the event must be visualised
    G4RunManager* runManager = G4RunManager::GetRunManager();
    // Casting is safe here: one knows the RTTI of the UserActions
    // in the current application (otherwise one could dynamic_cast)
    XrayTelEventAction* eventAction = (XrayTelEventAction*) runManager->GetUserEventAction();
    eventAction->Update();

    // Notify the corresponding UserAction to update the run counters
    CRunAction* runAction = (CRunAction*) runManager->GetUserRunAction();
    runAction->Update(track->GetKineticEnergy());
    }*/

    // Do the analysis related to this step
    //CAnalysis* analysis = CAnalysis::getInstance();
    //analysis->analyseStepping(*track, entering);

    //scrittura del file sx.step.boundary.out

    bool written = false;
    if((gm.enableWriteEnergyVol || gm.enableWriteINOUT) && step->GetPreStepPoint()->GetStepStatus() == fGeomBoundary) {
        //la particella è entrata nel volume, la scrivo
        //G4TouchableHistory* theTouchable = (G4TouchableHistory*)(step->GetPreStepPoint()->GetTouchable());
        //volReplica = theTouchable->GetVolume()->GetCopyNo();

        G4StepPoint * preStepPoint = step->GetPreStepPoint();
        G4TouchableHandle theTouchable  =  preStepPoint->GetTouchableHandle();
        volReplica = theTouchable->GetCopyNumber();

        //G4TouchableHandle preTouchable  =  preStepPoint->GetTouchableHandle();
        //G4int depth=1;
        //motherID    = preTouchable->GetVolume(depth)->GetCopyNo();
        //cout << "motherID" << motherID << endl;


        particleIDstep = step->GetTrack()->GetDefinition()->GetPDGEncoding();

        // 		volNameInput = step->GetTrack()->GetVolume()->GetName();
        volNameInput = theTouchable->GetVolume()->GetName();
        ekin = step->GetPreStepPoint()->GetKineticEnergy();
        particleName = step->GetTrack()->GetDynamicParticle()->GetDefinition()->GetParticleName();
        //lastParticleID = particleID = gm.LUTParticleType(particleName);

        pos = step->GetPreStepPoint()->GetPosition();
        mom = step->GetPreStepPoint()->GetMomentum();
        momDir = step->GetPreStepPoint()->GetMomentumDirection();
        gltime = step->GetPreStepPoint()->GetGlobalTime();
        if(step->GetPostStepPoint()->GetProcessDefinedStep() != NULL) {
            procName =  step->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName();
        } else {
            procName = "UserLimit";
        }
        processID = gm.LUTProcessType(procName);
        if(processID != 2)
            lastProcessID = processID;
        lastTrackID = trackID = step->GetTrack()->GetTrackID();
        inout = "IN";
        inoutCode = 0;
        if(gm.enableWriteFITS)
            writeFITSFile();
        if(gm.enableWriteTXT)
            writeTXTFile();
        written = true;
        if(gm.enableWriteFITS && gm.enableWriteEnergyVol) {
            // 			gm.fits_energyvol->open();
            // 			gm.fits_energyvol->writeData();
            gm.fits_energyvol->addData(this, 1);
        }
#ifndef WITHOUTROOT
        if(gm.enableWriteRoot && gm.enableWriteEnergyVol) {
            gm.root_energyvol->addData(this, 1);
        }
        if(gm.enableWriteRootEnd  && gm.enableWriteEnergyVol) {
            gm.root_energyvolend->addData(this, 1);
        }
#endif
        if(gm.enableWriteFITSEnd && gm.enableWriteEnergyVol) {
            gm.fits_energyvolend->addData(this, 1);
        }
    }
    if((gm.enableWriteEnergyVol || gm.enableWriteINOUT) && step->GetPostStepPoint()->GetStepStatus() == fGeomBoundary) {
        //la particella è uscita nel volume, la scrivo. C'er un bug in mom e momDir: prendere quelle in ingresso

        //G4TouchableHistory* theTouchable = (G4TouchableHistory*)(step->GetPreStepPoint()->GetTouchable());
        //volReplica = theTouchable->GetVolume()->GetCopyNo();
        G4StepPoint * preStepPoint = step->GetPreStepPoint();
        G4TouchableHandle theTouchable  =  preStepPoint->GetTouchableHandle();
        volReplica = theTouchable->GetCopyNumber();


        particleIDstep = step->GetTrack()->GetDefinition()->GetPDGEncoding();

        volNameInput = theTouchable->GetVolume()->GetName();

        //G4TouchableHandle preTouchable  =  preStepPoint->GetTouchableHandle();
        //G4int depth=1;
        //motherID    = preTouchable->GetVolume(depth)->GetCopyNo();
        //cout << "motherID" << motherID << endl;

        // 		volNameInput = step->GetTrack()->GetVolume()->GetName();
        ekin = step->GetPostStepPoint()->GetKineticEnergy();
        particleName = step->GetTrack()->GetDynamicParticle()->GetDefinition()->GetParticleName();
        //lastParticleID = particleID = gm.LUTParticleType(particleName);
        pos = step->GetPostStepPoint()->GetPosition();
        mom = step->GetPostStepPoint()->GetMomentum();
        momDir = step->GetPostStepPoint()->GetMomentumDirection();
        gltime = step->GetPostStepPoint()->GetGlobalTime();
        if(step->GetPostStepPoint()->GetProcessDefinedStep() != NULL) {
            procName =  step->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName();
        } else {
            procName = "UserLimit";
        }
        processID = gm.LUTProcessType(procName);
        if(processID != 2)
            lastProcessID = processID;
        lastTrackID = trackID = step->GetTrack()->GetTrackID();
        inout = "OU";
        inoutCode = 1;
        if(gm.enableWriteFITS)
            writeFITSFile();
        if(gm.enableWriteTXT)
            writeTXTFile();
        written = true;
        if(gm.enableWriteFITS && gm.enableWriteEnergyVol) {
            /*			gm.fits_energyvol->open();
            			gm.fits_energyvol->writeData();*/
            gm.fits_energyvol->addData(this, 0);
        }
#ifndef WITHOUTROOT
        if(gm.enableWriteRoot && gm.enableWriteEnergyVol) {
            gm.root_energyvol->addData(this, 0);
        }
        if(gm.enableWriteRootEnd  && gm.enableWriteEnergyVol) {
            gm.root_energyvolend->addData(this, 0);
        }
#endif
        if(gm.enableWriteFITSEnd && gm.enableWriteEnergyVol) {
            gm.fits_energyvolend->addData(this, 0);
        }
    }

}


void CSteppingAction::writeTXTFile() {

    std::ofstream asciiFile(gm.GetOutputStepBoundaryFileNameTXT(), std::ios::app);
    if(asciiFile.is_open()) {
        //EVENTID
        asciiFile << gm.event_id << "\t";
        //TRACKID
        asciiFile << trackID << "\t";
        //INOUT
        asciiFile <<  " " << inout << "\t";
        //GLTIME
        asciiFile << std::setiosflags(std::ios::fixed)
                  << std::setprecision(6)
                  << std::setiosflags(std::ios::right)
                  << std::setw(6);
        asciiFile << gltime / ns;
        asciiFile << "\t";

        //POSX
        asciiFile << std::setiosflags(std::ios::fixed)
                  << std::setprecision(6)
                  << std::setiosflags(std::ios::right)
                  << std::setw(6);
        asciiFile << pos[0] / mm;
        asciiFile << "\t";
        //POSY
        asciiFile << std::setiosflags(std::ios::fixed)
                  << std::setprecision(6)
                  << std::setiosflags(std::ios::right)
                  << std::setw(6);
        asciiFile << pos[1] / mm;
        asciiFile << "\t";
        //POSZ
        asciiFile << std::setiosflags(std::ios::fixed)
                  << std::setprecision(6)
                  << std::setiosflags(std::ios::right)
                  << std::setw(6);
        asciiFile << pos[2] / mm;
        asciiFile << "\t";
        //MOMX
        asciiFile << std::setiosflags(std::ios::fixed)
                  << std::setprecision(6)
                  << std::setiosflags(std::ios::right)
                  << std::setw(6);
        asciiFile << mom[0];
        asciiFile << "\t";
        //MOMY
        asciiFile << std::setiosflags(std::ios::fixed)
                  << std::setprecision(6)
                  << std::setiosflags(std::ios::right)
                  << std::setw(6);
        asciiFile << mom[1];
        asciiFile << "\t";
        //MOMZ
        asciiFile << std::setiosflags(std::ios::fixed)
                  << std::setprecision(6)
                  << std::setiosflags(std::ios::right)
                  << std::setw(6);
        asciiFile << mom[2];
        asciiFile << "\t";
        //MOMDIRX
        asciiFile << std::setiosflags(std::ios::fixed)
                  << std::setprecision(6)
                  << std::setiosflags(std::ios::right)
                  << std::setw(6);
        asciiFile << momDir[0];
        asciiFile << "\t";
        //MOMDIRY
        asciiFile << std::setiosflags(std::ios::fixed)
                  << std::setprecision(6)
                  << std::setiosflags(std::ios::right)
                  << std::setw(6);
        asciiFile << momDir[1];
        asciiFile << "\t";
        //MOMDIRZ
        asciiFile << std::setiosflags(std::ios::fixed)
                  << std::setprecision(6)
                  << std::setiosflags(std::ios::right)
                  << std::setw(6);
        asciiFile << momDir[2];
        asciiFile << "\t";
        //EKIN
        asciiFile << std::setiosflags(std::ios::fixed)
                  << std::setprecision(6)
                  << std::setiosflags(std::ios::right)
                  << std::setw(6);
        asciiFile << ekin / keV;
        asciiFile << "\t";
        //PARTICLENAME
        asciiFile <<  " " << particleName << "\t";
        //PROCNAME
        asciiFile <<  " " << procName << "\t";
        //VOLNAME
        asciiFile <<  " " << volNameInput << " "
                  << G4endl;
    }
    asciiFile.close();
}

void CSteppingAction::writeFITSFile() {
    if(gm.enableWriteFITS && gm.enableWriteINOUT) {
        gm.fits_inout->open();
        gm.fits_inout->writeData(this);
    }
    if(gm.enableWriteFITS && gm.enableWriteSTEP) {
        // 		gm.fits_step->open();
        // 		gm.fits_step->writeData(this);
    }


}



