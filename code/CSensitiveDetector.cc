/***************************************************************************
                          CSensitiveDetector.cc  -  description
                             -------------------
    begin                : 2006-2008
    copyright            : (C) 2008 modified by Andrea Bulgarelli
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
// * acllceptance of all terms of the Geant4 Software license.          *
// ********************************************************************

#include "CSensitiveDetector.hh"
#include "G4HCofThisEvent.hh"
#include "G4Step.hh"
#include "G4ThreeVector.hh"
#include "G4SDManager.hh"
#include <G4StepPoint.hh>
#include <G4VProcess.hh>
#include "G4ios.hh"
#include <fstream>
#include <iomanip>
#include "THELGlobalMemory.hh"
#include "FITSOutput_Step.hh"

#if defined(GEANT4_10_1) || defined(GEANT4_10_2) || defined(GEANT4_10_3) || defined(GEANT4_10_4)
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"
#endif


CSensitiveDetector::CSensitiveDetector(G4String name)
    :G4VSensitiveDetector(name) {
    G4String HCname = name + "_Collection";
    collectionName.insert(HCname);
    sxhitCollection = 0;
    HCID = -1;
}


CSensitiveDetector::~CSensitiveDetector() { }


void CSensitiveDetector::Initialize(G4HCofThisEvent* HCE) {
    if(sxhitCollection != 0) {
        G4int NbHits = sxhitCollection->entries();
        for (G4int i=0; i<NbHits; i++) {
            CHit* hit = (*sxhitCollection)[i];
            delete hit;
        }
        // 		delete sxhitCollection;
    }

    sxhitCollection = new CHitsCollection
    (SensitiveDetectorName,collectionName[0]);
    static G4int HCID = -1;
    if(HCID<0) {
        HCID = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);
    }
    HCE->AddHitsCollection( HCID, sxhitCollection );
    // 	G4cout << "CSensitiveDetector " << HCID << " " << sxhitCollection << G4endl;

}

G4bool CSensitiveDetector::ProcessHits(G4Step* aStep, G4TouchableHistory*) {
    // 	G4cout << "G4bool CSensitiveDetector::ProcessHits(G4Step* aStep, G4TouchableHistory*)" << G4endl;
    //return true;
    G4double edep = aStep->GetTotalEnergyDeposit();
    //G4Track->GetVertex---
    // 	G4cout << edep <<G4endl;
    // 	if(edep==0.) return false;

    CHit* newHit = new CHit();
    newHit->SetTrackID  (aStep->GetTrack()->GetTrackID());
    if( aStep->GetTrack()->GetNextVolume() != 0 ) {
        newHit->SetVolumeName  ( aStep->GetTrack()->GetVolume()->GetName() );
    } else {
        newHit->SetVolumeName  ( "OutOfWorld" );
    }
    newHit->SetGeomNumber(aStep->GetPreStepPoint()->GetTouchableHandle()->GetCopyNumber());
    newHit->SetEdep     (edep);
    newHit->SetMothID(aStep->GetPreStepPoint()->GetTouchableHandle()->GetCopyNumber());
    newHit->SetPos      (aStep->GetPostStepPoint()->GetPosition());
    newHit->SetMom      (aStep->GetPostStepPoint()->GetMomentum());
    newHit->SetGlobalTime    (aStep->GetPostStepPoint()->GetGlobalTime());
    newHit->SetLocalTime    (aStep->GetPostStepPoint()->GetLocalTime());
    newHit->SetProperTime    (aStep->GetPostStepPoint()->GetProperTime());

    newHit->SetKineticEnergy (aStep->GetTrack()->GetKineticEnergy() );
    newHit->SetStepLength(aStep->GetStepLength());
    newHit->SetTrackLength(aStep->GetTrack()->GetTrackLength());
    if(aStep->GetPostStepPoint()->GetProcessDefinedStep() != NULL) {
        newHit->SetProcessName( aStep->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName() );
    } else {
        newHit->SetProcessName( "UserLimit" );
    }
    newHit->SetParticleID(aStep->GetTrack()->GetDefinition()->GetPDGEncoding());
    G4String ptt;
    ptt = aStep->GetTrack()->GetDynamicParticle()->GetDefinition()->GetParticleName();
    newHit->SetParticleType(ptt);
    sxhitCollection->insert( newHit );

    return true;
}


void CSensitiveDetector::EndOfEvent(G4HCofThisEvent* HCE) {

    //static G4int HCID = -1; //ERA COSI'. CHE COSA CAMBIA?
    if(HCID<0) {
        HCID = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);
    }
    //   	HCE->AddHitsCollection(HCID,sxhitCollection);

    G4int NbHits = sxhitCollection->entries();
    // 	G4cout << "############### " << gm.event_id << " " << NbHits << G4endl;
    //if (verboseLevel>0) {
    if(NbHits > 0) {
        // 		G4cout << NbHits << G4endl;
        // 		G4int NbHits = sxhitCollection->entries();
        // 		G4cout << "\n--------> Hits Collection: in this event they are " << NbHits << G4endl;		for (G4int i=0;i<NbHits;i++) (*sxhitCollection)[i]->Print();
    }
    G4bool entering = true;
    if(gm.enableWriteTXT && gm.enableWriteSTEP)
        if (entering) {
            if(NbHits > 0) {
                std::ofstream asciiFile(gm.GetOutputHitFileNameTXT(), std::ios::app);
                if(asciiFile.is_open()) {
                    for (G4int i=0; i<NbHits; i++) {
                        CHit* hit = (*sxhitCollection)[i];
                        asciiFile << gm.event_id << "\t";
                        asciiFile << hit->GetTrackID() << "\t";
                        asciiFile << i << "\t";

                        asciiFile << std::setiosflags(std::ios::fixed)
                                  << std::setprecision(6)
                                  << std::setiosflags(std::ios::right)
                                  << std::setw(6);
                        asciiFile << hit->GetGlobalTime() / ns;
                        asciiFile << "\t";

                        asciiFile << std::setiosflags(std::ios::fixed)
                                  << std::setprecision(6)
                                  << std::setiosflags(std::ios::right)
                                  << std::setw(6);
                        asciiFile << hit->GetLocalTime() / ns;
                        asciiFile << "\t";

                        asciiFile << std::setiosflags(std::ios::fixed)
                                  << std::setprecision(6)
                                  << std::setiosflags(std::ios::right)
                                  << std::setw(6);
                        asciiFile << hit->GetProperTime() / ns;
                        asciiFile << "\t";

                        asciiFile << std::setiosflags(std::ios::fixed)
                                  << std::setprecision(6)
                                  << std::setiosflags(std::ios::right)
                                  << std::setw(6);
                        asciiFile << hit->GetKineticEnergy() / keV;
                        asciiFile << "\t";

                        asciiFile << std::setiosflags(std::ios::fixed)
                                  << std::setprecision(6)
                                  << std::setiosflags(std::ios::right)
                                  << std::setw(6);
                        asciiFile << hit->GetEdep() / keV;
                        asciiFile << "\t";

                        asciiFile << std::setiosflags(std::ios::fixed)
                                  << std::setprecision(6)
                                  << std::setiosflags(std::ios::right)
                                  << std::setw(6);
                        asciiFile << hit->GetPos()[0] / mm;
                        asciiFile << "\t";
                        asciiFile << std::setiosflags(std::ios::fixed)
                                  << std::setprecision(6)
                                  << std::setiosflags(std::ios::right)
                                  << std::setw(6);
                        asciiFile << hit->GetPos()[1] /mm;
                        asciiFile << "\t";
                        asciiFile << std::setiosflags(std::ios::fixed)
                                  << std::setprecision(6)
                                  << std::setiosflags(std::ios::right)
                                  << std::setw(6);
                        asciiFile << hit->GetPos()[2] /mm;
                        asciiFile << "\t";

                        asciiFile << std::setiosflags(std::ios::fixed)
                                  << std::setprecision(6)
                                  << std::setiosflags(std::ios::right)
                                  << std::setw(6);
                        asciiFile << hit->GetMom()[0];
                        asciiFile << "\t";
                        asciiFile << std::setiosflags(std::ios::fixed)
                                  << std::setprecision(10)
                                  << std::setiosflags(std::ios::right)
                                  << std::setw(6);
                        asciiFile << hit->GetMom()[1];
                        asciiFile << "\t";
                        asciiFile << std::setiosflags(std::ios::fixed)
                                  << std::setprecision(6)
                                  << std::setiosflags(std::ios::right)
                                  << std::setw(6);
                        asciiFile << hit->GetMom()[2];
                        asciiFile << "\t";

                        asciiFile << std::setiosflags(std::ios::fixed)
                                  << std::setprecision(6)
                                  << std::setiosflags(std::ios::right)
                                  << std::setw(6);
                        asciiFile << hit->GetStepLength() / mm;
                        asciiFile << "\t";

                        asciiFile << std::setiosflags(std::ios::fixed)
                                  << std::setprecision(6)
                                  << std::setiosflags(std::ios::right)
                                  << std::setw(6);
                        asciiFile << hit->GetTrackLength() / mm;
                        asciiFile << "\t";

                        asciiFile <<  " " << hit->GetParticleType() << "\t";

                        asciiFile <<  " " << hit->GetProcessName() << "\t";

                        asciiFile <<  " " << hit->GetVolumeName() << " "
                                  << G4endl;
                    }
                    asciiFile.close();
                }
            }
        }

    if(gm.enableWriteFITS && gm.enableWriteSTEP) {
        // 		G4cout << "{" << NbHits << "}";
        if(NbHits > 0) {

            for (G4int i=0; i<NbHits; i++) {
                CHit* hit = (*sxhitCollection)[i];
                gm.fits_step->open();
                gm.fits_step->writeData(hit, i);
            }
        }
    }

}


