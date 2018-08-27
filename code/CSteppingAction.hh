/***************************************************************************
                           CSteppingAction.hh  -  description
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
// * MODULE:            CSteppingAction.hh                        *
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
//
// 16.10.2001 R.Nartallo
// - Clean up code to avoid 'pedantic' and 'ANSI' compiler warnings
//
// 06.11.2000 R.Nartallo
// - First implementation of X-ray Telescope advanced example.
// - Based on Chandra and XMM models
//
//
// **********************************************************************

#ifndef CSteppingAction_h
#define CSteppingAction_h 1

#include "G4UserSteppingAction.hh"
#include "G4ios.hh"
#include "globals.hh"
#include "G4ThreeVector.hh"
#include "G4TouchableHistory.hh"

class G4Step;



class CSteppingAction : public G4UserSteppingAction {
public:

    CSteppingAction();

    virtual ~CSteppingAction();

    virtual void UserSteppingAction(const G4Step*);

protected:

    virtual void writeTXTFile();

    virtual void writeFITSFile();


public:

    G4String volNameInput;
    G4int volReplica;
    G4int motherID;
    G4String procName;
    G4String inout;
    G4int inoutCode; //0 IN, 1 OUT
    G4double ekin; //energia cinetica dell'evento
    G4int particleIDstep;

    // 		G4double edep; //energia cinetica depositata nel volume
    G4double gltime;
    G4long trackID;
    G4ThreeVector pos;
    G4ThreeVector mom;
    G4ThreeVector momDir;
    G4String particleName;
    //unsigned char particleID;
    unsigned char processID;

    int lastProcessID;
    //int lastParticleID;
    int lastTrackID;


};

#endif
