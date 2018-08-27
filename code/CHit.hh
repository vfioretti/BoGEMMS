/***************************************************************************
                          CHit.hh  -  description
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

#ifndef CHit_h
#define CHit_h 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"


class CHit : public G4VHit {
public:

    CHit();
    ~CHit();
    CHit(const CHit&);
    const CHit& operator=(const CHit&);
    G4int operator==(const CHit&) const;

    inline void* operator new(size_t);
    inline void  operator delete(void*);

    void Draw();
    void Print();

public:

    void SetTrackID  (G4int track)      {
        trackID = track;
    };
    void SetGeomNumber(G4int gm)      {
        geomNumber = gm;
    };
    void SetVolumeName(G4String vn)      {
        volumeName = vn;
    };
    void SetEdep     (G4double de)      {
        edep = de;
    };
    void SetMothID     (G4int mvid)      {
        mothid = mvid;
    };
    void SetPos      (G4ThreeVector xyz) {
        pos = xyz;
    };
    void SetMom      (G4ThreeVector m) {
        mom = m;
    };
    void SetGlobalTime     (G4double t)      {
        globalTime = t;
    };
    void SetLocalTime     (G4double t)      {
        localTime = t;
    };
    void SetProperTime     (G4double t)      {
        properTime = t;
    };
    void SetKineticEnergy(G4double ted) {
        ke = ted;
    };
    void SetStepLength(G4double ssl) {
        stepLength=ssl;
    };
    void SetTrackLength(G4double stl) {
        trackLength = stl;
    };
    void SetProcessName(G4String procN) {
        processName = procN;
    };
    void SetParticleID(G4int pid) {
        particleID = pid;
    };
    void SetParticleType(G4String pt) {
        particleType = pt;
    };
    G4int GetTrackID()    {
        return trackID;
    };
    G4int GetGeomNumber()  {
        return geomNumber;
    };
    G4String GetVolumeName()  {
        return volumeName;
    };
    G4double GetEdep()    {
        return edep;
    };
    G4double GetMothID()    {
        return mothid;
    };
    G4ThreeVector GetPos() {
        return pos;
    };
    G4ThreeVector GetMom() {
        return mom;
    };
    G4double GetGlobalTime()    {
        return globalTime;
    };
    G4double GetLocalTime()    {
        return localTime;
    };
    G4double GetProperTime()    {
        return properTime;
    };
    G4double GetKineticEnergy() {
        return ke;
    };
    G4double GetStepLength() {
        return stepLength;
    };
    G4double GetTrackLength() {
        return trackLength;
    };
    G4String GetProcessName() {
        return processName;
    };
    G4int    GetParticleID() {
        return particleID;
    };
    G4String GetParticleType() {
        return particleType;
    };
    unsigned char GetProcessID();
    //unsigned char GetParticleID();
private:

    G4int         trackID;
    G4int         geomNumber;
    G4String      volumeName;
    G4double      edep;
    G4int         mothid;
    G4ThreeVector pos;
    G4ThreeVector mom;
    G4double globalTime;
    G4double localTime;
    G4double properTime;
    G4double ke; //total energy deposit
    G4double stepLength;
    G4double trackLength;
    G4String    processName;
    G4int    particleID;
    G4String    particleType;
};


typedef G4THitsCollection<CHit> CHitsCollection;

extern G4Allocator<CHit> CHitAllocator;


inline void* CHit::operator new(size_t) {
    void *aHit;
    aHit = (void *) CHitAllocator.MallocSingle();
    return aHit;
}


inline void CHit::operator delete(void *aHit) {
    CHitAllocator.FreeSingle((CHit*) aHit);
}


#endif
