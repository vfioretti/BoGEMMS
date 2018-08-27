/***************************************************************************
                          CXYSSD.hh  -  description
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

#ifndef CXYZSD_h
#define CXYZSD_h 1

#include "G4VSensitiveDetector.hh"
#include "CXYZHit.hh"
using namespace std;

class G4Step;
class G4HCofThisEvent;

class CXYZSD : public G4VSensitiveDetector {
public:
    CXYZSD(G4String);
    ~CXYZSD();

    void Initialize(G4HCofThisEvent*);
    G4bool ProcessHits(G4Step*, G4TouchableHistory*);
    void EndOfEvent(G4HCofThisEvent*);
    void clear();
    void DrawAll();
    void PrintAll();
    G4int volID_Type;
    

private:
    CHitsXYZCollection* xyzhitCollection;
    vector<CXYZHit*> CXYZVector;

    G4int HCIDXYZ;

    //Variable inizialization - info from step

    G4int         info_eventID;
    G4int         info_trackID;
    G4int         info_Parent_trackID;
    G4int         info_volumeID;
    G4String      info_volumeName;
    G4int         info_motherID;
    G4double      info_edep;
    G4ThreeVector info_pos_entrance;
    G4ThreeVector info_pos_exit;
    G4double      info_ekin_entrance;
    G4double      info_ekin_exit;
    G4ThreeVector info_momdir_entrance;
    G4ThreeVector info_momdir_exit;
    G4double      info_globalTime_entrance;
    G4double      info_globalTime_exit;
    G4int         info_particleID;
    G4String      info_particleType;

    G4String      info_processCreator;
    G4int         info_processCreatorID;

    G4int temp_ID;
    long temp_evt;
    G4int xyzOutType;

};


#endif

