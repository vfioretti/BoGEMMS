/***************************************************************************
                          CXYZHit.cc  -  description
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


#include "CXYZHit.hh"
#include "G4UnitsTable.hh"
#include "G4VVisManager.hh"
#include "G4Circle.hh"
#include "G4Colour.hh"
#include "G4VisAttributes.hh"
#include "THELGlobalMemory.hh"

G4Allocator<CXYZHit> CHitXYZAllocator;

CXYZHit::CXYZHit() :
    eventID(-1), trackID(-1), parentTrackID(-1), volumeID(-1), volumeName('a'), motherID(-1), edep(0.), pos_entrance(0., 0., 0.), pos_exit(0., 0., 0.),
    ekin_entrance(0.), ekin_exit(0.), momdir_entrance(0., 0., 0.), momdir_exit(0., 0., 0.),
    globalTime_entrance(0.), globalTime_exit(0.), particleID(-1), particleType('a'), physVol(NULL), processCreatorID(-1), processCreator('a'){
}

CXYZHit::~CXYZHit() {

}


CXYZHit::CXYZHit(const CXYZHit& right)
    : G4VHit() {
    eventID   = right.eventID;
    trackID   = right.trackID;
    parentTrackID   = right.parentTrackID;
    volumeID = right.volumeID;
    volumeName = right.volumeName;
    motherID = right.motherID;
    edep      = right.edep;
    pos_entrance       = right.pos_entrance;
    pos_exit       = right.pos_exit;
    ekin_entrance       = right.ekin_entrance;
    ekin_exit       = right.ekin_exit;
    momdir_entrance = right.momdir_entrance;
    momdir_exit = right.momdir_exit;
    globalTime_entrance = right.globalTime_entrance;
    globalTime_exit = right.globalTime_exit;
    particleID = right.particleID;
    particleType = right.particleType;
    physVol = right.physVol;
    processCreatorID = right.processCreatorID;
    processCreator = right.processCreator;
}

const CXYZHit& CXYZHit::operator=(const CXYZHit& right) {
    eventID   = right.eventID;
    trackID   = right.trackID;
    parentTrackID   = right.parentTrackID;
    volumeID = right.volumeID;
    volumeName = right.volumeName;
    motherID = right.motherID;
    edep      = right.edep;
    pos_entrance       = right.pos_entrance;
    pos_exit       = right.pos_exit;
    ekin_entrance       = right.ekin_entrance;
    ekin_exit       = right.ekin_exit;
    momdir_entrance = right.momdir_entrance;
    momdir_exit = right.momdir_exit;
    globalTime_entrance = right.globalTime_entrance;
    globalTime_exit = right.globalTime_exit;
    particleID = right.particleID;
    particleType = right.particleType;
    physVol = right.physVol;
    processCreatorID = right.processCreatorID;
    processCreator = right.processCreator;
    return *this;
}

G4int CXYZHit::operator==(const CXYZHit& right) const {
    return ((eventID == right.eventID) && //
            (trackID == right.trackID) && //
            (parentTrackID == right.parentTrackID) && //
            (volumeID == right.volumeID) && //
            (volumeName == right.volumeName) && //
            (motherID == right.motherID) && //
            (edep == right.edep) && //
            (pos_entrance == right.pos_entrance) && //
            (pos_exit == right.pos_exit) && //
            (ekin_entrance == right.ekin_entrance) && //
            (ekin_exit == right.ekin_exit) && //
            (momdir_entrance == right.momdir_entrance) && //
            (momdir_exit == right.momdir_exit) && //
            (globalTime_entrance == right.globalTime_entrance) && //
            (globalTime_exit == right.globalTime_exit) && //
            (particleID == right.particleID) && //
            (particleType == right.particleType) && //
            (physVol == right.physVol) && //
            (processCreatorID == right.processCreatorID)) &&//
            (processCreator == right.processCreator);
}


void CXYZHit::Draw() {
}


void CXYZHit::Print() {
}

/*
unsigned char CXYZHit::GetParticleID() {
	return gm.LUTParticleType(particleType);
}
*/

