/***************************************************************************
                          CHit.cc  -  description
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
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************

#include "CHit.hh"
#include "G4UnitsTable.hh"
#include "G4VVisManager.hh"
#include "G4Circle.hh"
#include "G4Colour.hh"
#include "G4VisAttributes.hh"
#include "THELGlobalMemory.hh"

G4Allocator<CHit> CHitAllocator;


CHit::CHit() {}


CHit::~CHit() {
//  	G4cout << "DELETE HIT" << G4endl;
}


CHit::CHit(const CHit& right)
    : G4VHit() {
    trackID   = right.trackID;
    geomNumber = right.geomNumber;
    volumeName = right.volumeName;
    edep      = right.edep;
    mothid      = right.mothid;
    pos       = right.pos;
    mom = right.mom;
    ke = right.ke;
    stepLength = right.stepLength;
    trackLength = right.trackLength;
    particleID = right.particleID;
    processName = right.processName;
}


const CHit& CHit::operator=(const CHit& right) {
    trackID   = right.trackID;
    geomNumber = right.geomNumber;
    volumeName = right.volumeName;
    edep      = right.edep;
    mothid      = right.mothid;
    pos       = right.pos;
    mom = right.mom;
    ke = right.ke;
    stepLength = right.stepLength;
    trackLength = right.trackLength;
    particleID = right.particleID;
    processName = right.processName;
    return *this;
}


G4int CHit::operator==(const CHit& right) const {
    return (this==&right) ? 1 : 0;
}


void CHit::Draw() {
    return;
    G4VVisManager* pVVisManager = G4VVisManager::GetConcreteInstance();
    if(pVVisManager) {
        G4Circle circle(pos);
        circle.SetScreenSize(2.);
        circle.SetFillStyle(G4Circle::filled);
        G4Colour colour(1.,0.,0.);
        G4VisAttributes attribs(colour);
        circle.SetVisAttributes(attribs);
        pVVisManager->Draw(circle);
    }
}


void CHit::Print() {
    G4cout << "  trackID: " << trackID << "  volName " << volumeName << "  geomNumber: " << geomNumber
           << "  energy deposit: " << G4BestUnit(edep,"Energy")
           << "  position: " << G4BestUnit(pos,"Length") << G4endl;
}

unsigned char CHit::GetProcessID() {
    return gm.LUTProcessType(processName);
}


