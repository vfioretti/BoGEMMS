/***************************************************************************
                          CXYZHit.hh  -  description
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

#ifndef CXYZHit_h
#define CXYZHit_h 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"
#include "G4VPhysicalVolume.hh"

class CXYZHit : public G4VHit {
public:

    CXYZHit();
    ~CXYZHit();
    CXYZHit(const CXYZHit&);
    const CXYZHit& operator=(const CXYZHit&);
    G4int operator==(const CXYZHit&) const;

    inline void* operator new(size_t);
    inline void  operator delete(void*);

    void Draw();
    void Print();

public:

    inline void SetEventID              (G4int event)            {
        eventID = event;
    };
    
    inline void SetTrackID              (G4int track)            {
        trackID = track;
    };
    
    inline void SetParentTrackID        (G4int parentTrack)      {
        parentTrackID = parentTrack;
    };
    
    inline void SetVolumeID             (G4int vid)              {
        volumeID = vid;
    };
    inline void SetVolumeName           (G4String vn)            {
        volumeName = vn;
    };
    inline void SetMotherID             (G4int mid)              {
        motherID = mid;
    };
    inline void SetEdep                 (G4double de)            {
        edep = de;
    };
    inline void AddEnergy               (G4double adde)          {
        edep += adde;
    };
    inline void SetPosEntrance          (G4ThreeVector xyz_ent)  {
        pos_entrance = xyz_ent;
    };
    inline void SetPosExit              (G4ThreeVector xyz_exit) {
        pos_exit = xyz_exit;
    };
    inline void SetEkinEntrance         (G4double e_ent)         {
        ekin_entrance = e_ent;
    };
    inline void SetEkinExit             (G4double e_exit)        {
        ekin_exit = e_exit;
    };
    inline void SetMomDirEntrance       (G4ThreeVector modi_ent) {
        momdir_entrance = modi_ent;
    };
    inline void SetMomDirExit           (G4ThreeVector modi_exit) {
        momdir_exit = modi_exit;
    };
    inline void SetGlobalTimeEntrance   (G4double t_ent)         {
        globalTime_entrance = t_ent;
    };
    inline void SetGlobalTimeExit       (G4double t_exit)        {
        globalTime_exit = t_exit;
    };
    inline void SetParticleID           (G4int pid)              {
        particleID = pid;
    };
    inline void SetParticleType         (G4String pt)            {
        particleType = pt;
    };
    inline void SetProcessCreatorID     (G4int pc_id)            {
        processCreatorID = pc_id;
    };
    inline void SetProcessCreator       (G4String proc)          {
        processCreator = proc;
    };
    inline G4int GetEventID()                  const {
        return eventID;
    };
    inline G4int GetTrackID()                  const {
        return trackID;
    };
    inline G4int GetParentTrackID()            const {
        return parentTrackID;
    };
    inline G4int GetVolumeID()                 const {
        return volumeID;
    };
    inline G4String GetVolumeName()            const {
        return volumeName;
    };
    inline G4int GetMotherID()                 const {
        return motherID;
    };
    inline G4double GetEdep()                  const {
        return edep;
    };
    inline G4ThreeVector GetPosEntrance()      const {
        return pos_entrance;
    };
    inline G4ThreeVector GetPosExit()          const {
        return pos_exit;
    };
    inline G4double GetEkinEntrance()          const {
        return ekin_entrance;
    };
    inline G4double GetEkinExit()              const {
        return ekin_exit;
    };
    inline G4ThreeVector GetMomDirEntrance()   const {
        return momdir_entrance;
    };
    inline G4ThreeVector GetMomDirExit()       const {
        return momdir_exit;
    };
    inline G4double GetGlobalTimeEntrance()    const {
        return globalTime_entrance;
    };
    inline G4double GetGlobalTimeExit()        const {
        return globalTime_exit;
    };
    inline G4int GetParticleID()               const {
        return particleID;
    };
    inline G4String GetParticleType()          const {
        return particleType;
    };
    inline G4VPhysicalVolume *GetPhysicalVolume() const {
        return physVol;
    };
     inline G4int GetProcessCreatorID()           const {
        return processCreatorID;
    };
    inline G4String GetProcessCreator()              const {
        return processCreator;
    };
    //unsigned char GetParticleID();


private:

    G4int         eventID;
    G4int         trackID;
    G4int         parentTrackID;
    G4int         volumeID;
    G4String      volumeName;
    G4int         motherID;
    G4double      edep;
    G4ThreeVector pos_entrance;
    G4ThreeVector pos_exit;
    G4double      ekin_entrance;
    G4double      ekin_exit;
    G4ThreeVector momdir_entrance;
    G4ThreeVector momdir_exit;
    G4double      globalTime_entrance;
    G4double      globalTime_exit;
    G4int         particleID;
    G4String      particleType;
    G4VPhysicalVolume *physVol;
    G4int         processCreatorID;
    G4String      processCreator;
};

typedef G4THitsCollection<CXYZHit> CHitsXYZCollection;

extern G4Allocator<CXYZHit> CHitXYZAllocator;

inline void* CXYZHit::operator new(size_t) {
    void *aHit;
    aHit = (void *) CHitXYZAllocator.MallocSingle();
    return aHit;
}

inline void CXYZHit::operator delete(void *aHit) {
    CHitXYZAllocator.FreeSingle((CXYZHit*) aHit);
}

#endif
