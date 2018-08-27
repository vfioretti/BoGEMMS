/***************************************************************************
                          CXYZSD.cc  -  description
                             -------------------
    begin                : 2012
    copyright            : (C) 2012 Valentina Fioretti
    email                : fioretti@iasfbo.inaf.it
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


#include "CXYZSD.hh"
#include "CXYZHit.hh"
#include "G4RunManager.hh"

#include "G4HCofThisEvent.hh"
#include "G4Step.hh"
#include "G4ThreeVector.hh"
#include "G4VProcess.hh"
#include "G4VTouchable.hh"
#include "G4TouchableHistory.hh"
#include "G4SDManager.hh"
#include <G4StepPoint.hh>
#include <G4VProcess.hh>
#include "G4EventManager.hh"
#include "G4ios.hh"
#include <fstream>
#include <iomanip>
#include "THELGlobalMemory.hh"
#include "FITSOutput_XYZ.hh"
#include "RootOutput_XYZ.hh"
#include "SQLliteOutput_XYZ.hh"

CXYZSD::CXYZSD(G4String name)
    :G4VSensitiveDetector(name) {

    G4String HCXYZname = name + "_CollectionXYZ";
    collectionName.insert(HCXYZname);
    xyzhitCollection = 0;
    HCIDXYZ = -1;
    //collectionName.insert("XYZHitCollection");
}


CXYZSD::~CXYZSD() { }


void CXYZSD::Initialize(G4HCofThisEvent* HCE) {

    xyzhitCollection = new CHitsXYZCollection
    (SensitiveDetectorName,collectionName[0]);
    //static G4int HCIDXYZ = -1;
    if(HCIDXYZ<0) {
        HCIDXYZ = G4SDManager::GetSDMpointer()->GetCollectionID(xyzhitCollection);
    }
    HCE->AddHitsCollection( HCIDXYZ, xyzhitCollection );

    CXYZVector.clear();
    gm.config->readInto(xyzOutType, "IOXYZ.TYPE");
    
    volID_Type = 0;
    gm.config->readInto(volID_Type, "GEOM.VOLID.TYPE");

}

G4bool CXYZSD::ProcessHits(G4Step* aStep, G4TouchableHistory*) {

    G4cout.precision(16);
    
    G4TouchableHistory* theTouchable = (G4TouchableHistory*) (aStep->GetPreStepPoint()->GetTouchable());
    G4VPhysicalVolume *currentVolume = theTouchable->GetVolume();
    G4TrackStatus trackStatus = aStep->GetTrack()->GetTrackStatus();
    G4VProcess* creatorProcess = (G4VProcess*) (aStep->GetTrack()->GetCreatorProcess());
    
    G4StepPoint * preStepPoint = aStep->GetPreStepPoint();
    G4StepPoint * postStepPoint = aStep->GetPostStepPoint();
    G4TouchableHandle preTouchable  =  preStepPoint->GetTouchableHandle();
    G4TouchableHandle postTouchable = postStepPoint->GetTouchableHandle();

    G4StepStatus  preStepStatus =  preStepPoint->GetStepStatus();
    G4StepStatus postStepStatus = postStepPoint->GetStepStatus();
    
    info_eventID = G4EventManager::GetEventManager()->GetConstCurrentEvent()->GetEventID();

    G4int depth=1;

    info_trackID     = aStep->GetTrack()->GetTrackID();
    if (info_trackID != 1){
        info_Parent_trackID     = aStep->GetTrack()->GetParentID();
    } else{
        info_Parent_trackID     = 0; // If the particle is a primary the Parent Track ID is set to 0.
    }
    
    G4int ndepth = preTouchable->GetHistoryDepth() ;
    

    G4int currentCopyNo;
    G4int currentMotherCopyNo;
    if (ndepth <= 2) {
       info_volumeID    = preTouchable->GetCopyNumber();
       info_volumeName  = preTouchable->GetVolume()->GetName();
       info_motherID    = theTouchable->GetCopyNumber(depth);
       currentCopyNo = info_volumeID;
       currentMotherCopyNo = info_motherID;
    
       //G4cout << " current volume ID " << currentCopyNo << G4endl;
       //G4cout << " current Mother ID " << currentMotherCopyNo << G4endl;
    }
    if (ndepth == 3) {
        if (volID_Type == 1) {
            G4int mother1 = preTouchable->GetCopyNumber(depth);
            G4int mother2 = preTouchable->GetCopyNumber(depth+1);
       
            info_volumeID    = preTouchable->GetCopyNumber();
            info_volumeName  = preTouchable->GetVolume()->GetName();
            info_motherID    = mother1 + mother2;
            currentCopyNo = info_volumeID;
            currentMotherCopyNo = info_motherID;
        }
        if (volID_Type == 0) {
            info_volumeID    = preTouchable->GetCopyNumber();
            info_volumeName  = preTouchable->GetVolume()->GetName();
            info_motherID    = theTouchable->GetCopyNumber(depth);
            currentCopyNo = info_volumeID;
            currentMotherCopyNo = info_motherID;
        }
       //G4cout << " current volume ID " << currentCopyNo << G4endl;
       //G4cout << " current Mother ID " << currentMotherCopyNo << G4endl;
    }
    
    //G4VProcess* aProcess = (G4VProcess*) postStepPoint->GetProcessDefinedStep();
    //G4cout << " Process " << aProcess->GetProcessName() << G4endl;
    info_edep        = aStep->GetTotalEnergyDeposit();

    info_pos_entrance         =  preStepPoint->GetPosition();
    //if (preStepPoint->GetStepStatus() == fGeomBoundary){
    //    G4cout << " Z position at boundary: " << preStepPoint->GetPosition() << G4endl;
    //}
    info_ekin_entrance        =  preStepPoint->GetKineticEnergy();
    info_momdir_entrance      =  preStepPoint->GetMomentumDirection();
    info_globalTime_entrance  =  preStepPoint->GetGlobalTime();

    info_pos_exit             =  postStepPoint->GetPosition();
    info_ekin_exit            =  postStepPoint->GetKineticEnergy();
    info_momdir_exit          =  postStepPoint->GetMomentumDirection();
    info_globalTime_exit      =  postStepPoint->GetGlobalTime();

    info_particleID = aStep->GetTrack()->GetDefinition()->GetPDGEncoding();
    info_particleType = aStep->GetTrack()->GetDynamicParticle()->GetDefinition()->GetParticleName();
    
    if (info_Parent_trackID != 0) {
        info_processCreator = creatorProcess->GetProcessName();
    } else {
        info_processCreator = "world";
    }
    
    //G4cout << " Process Name " << info_processCreator << G4endl;

    info_processCreatorID = 0;
    // assign ID to the Process name
    if (info_processCreator == "world")     info_processCreatorID = 1;
    if (info_processCreator == "phot")      info_processCreatorID = 2;
    if (info_processCreator == "compt")     info_processCreatorID = 3;
    if (info_processCreator == "eBrem")     info_processCreatorID = 4;
    if (info_processCreator == "eIoni")     info_processCreatorID = 5;
    if (info_processCreator == "annihil")   info_processCreatorID = 6;
    if (info_processCreator == "conv")      info_processCreatorID = 7;
    if (info_processCreator == "hIoni")     info_processCreatorID = 8;
    if (info_processCreator == "Rayl")      info_processCreatorID = 9;


    vector<CXYZHit*>::const_iterator i;

    G4bool newHit = true;

    for (i = CXYZVector.begin(); i != CXYZVector.end(); i++) {
        //G4cout << " Volume ID " << (*i)->GetVolumeID() << G4endl;
        //G4cout << " Mother ID " << (*i)->GetMotherID() << G4endl;

        if (currentCopyNo == (*i)->GetVolumeID() && currentMotherCopyNo == (*i)->GetMotherID() && info_trackID == (*i)->GetTrackID() && info_pos_entrance == (*i)->GetPosExit()) // this is an old hit
        {
            //G4cout << "Summing energy" << G4endl;
	    newHit = false;
            (*i)->AddEnergy(info_edep);
            (*i)->SetPosExit(info_pos_exit);
            (*i)->SetEkinExit(info_ekin_exit);
            (*i)->SetMomDirExit(info_momdir_exit);
            (*i)->SetGlobalTimeExit(info_globalTime_exit);

        }
    }

    if (newHit) // this is a new hit
    {
        CXYZHit* newXYZHit = new CXYZHit;

        newXYZHit->SetEventID(info_eventID);
        newXYZHit->SetTrackID(info_trackID);
        newXYZHit->SetParentTrackID(info_Parent_trackID);
        newXYZHit->SetVolumeID(info_volumeID);
        newXYZHit->SetVolumeName(info_volumeName);
        newXYZHit->SetMotherID(info_motherID);
        newXYZHit->AddEnergy(info_edep);
        newXYZHit->SetPosEntrance         (info_pos_entrance);
        //G4cout << " Z position at entrance: " << info_pos_entrance << G4endl;
        newXYZHit->SetPosExit             (info_pos_exit);
        newXYZHit->SetEkinEntrance        (info_ekin_entrance);
        newXYZHit->SetEkinExit            (info_ekin_exit);
        newXYZHit->SetMomDirEntrance      (info_momdir_entrance);
        newXYZHit->SetMomDirExit          (info_momdir_exit);
        newXYZHit->SetGlobalTimeEntrance  (info_globalTime_entrance);
        newXYZHit->SetGlobalTimeExit      (info_globalTime_exit);
        newXYZHit->SetParticleID          (info_particleID);
        newXYZHit->SetParticleType        (info_particleType);
        newXYZHit->SetProcessCreatorID    (info_processCreatorID);
        newXYZHit->SetProcessCreator      (info_processCreator);
        

        xyzhitCollection->insert(newXYZHit) - 1;

        //HCIDXYZ = xyzhitCollection->insert(newXYZHit) - 1;   // IT CAUSES A MEMORY LEAK!!!!
        CXYZVector.push_back(newXYZHit);
    }

    return true;

}

void CXYZSD::EndOfEvent(G4HCofThisEvent* HCE) {


    CXYZVector.clear();


    G4int NbHits = xyzhitCollection->entries();

    if(gm.enableWriteFITS && gm.enableWriteXYZ) {
        // 		G4cout << "{" << NbHits << "}";
        if(NbHits > 0) {

            for (G4int i=0; i<NbHits; i++) {
                CXYZHit* hit = (*xyzhitCollection)[i];
                gm.fits_xyz->open();
                gm.fits_xyz->writeData(hit, i);
            }
        }
    }
	
#ifndef WITHOUTROOT
	if(gm.enableWriteRoot && gm.enableWriteXYZ) {
		// 		G4cout << "{" << NbHits << "}";
		if(NbHits > 0) {
			
			for (G4int i=0; i<NbHits; i++) {
				CXYZHit* hit = (*xyzhitCollection)[i];
				//gm.root_xyz->open();
				gm.root_xyz->writeData(hit, i);
			}
		}
	}
#endif

#ifdef SQLITE3
	if(gm.enableWriteSQlite3 && gm.enableWriteXYZ) {
		if(NbHits > 0) {
			for (G4int i=0; i<NbHits; i++) {
				CXYZHit* hit = (*xyzhitCollection)[i];
				//gm.root_xyz->open();
				gm.sqlite3_xyz->writeData(hit, i);
			}
		}
	}
#endif

}


void CXYZSD::clear() {
}


void CXYZSD::DrawAll() {
}


void CXYZSD::PrintAll() {
}

