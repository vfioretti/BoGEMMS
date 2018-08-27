/***************************************************************************
                          CEventActionMessenger.cc  -  description
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
// * MODULE:            CEventActionMessenger.cc                  *
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
// 06.11.2000 R.Nartallo
// - First implementation of xray_telescope event action messenger
// - Based on Chandra and XMM models
//
//
// **********************************************************************

#include "G4UIcmdWithAString.hh"

#include "globals.hh"

#include "CEventAction.hh"
#include "CEventActionMessenger.hh"


CEventActionMessenger::CEventActionMessenger(CEventAction* EvAct)
    :eventAction(EvAct) {
    DrawCmd = new G4UIcmdWithAString("/event/drawTracks",this);
    DrawCmd->SetGuidance("Draw the tracks in the event");
    DrawCmd->SetGuidance("  Choice : none, charged, all (default)");
    DrawCmd->SetParameterName("choice",true);
    DrawCmd->SetDefaultValue("charged");
    DrawCmd->SetCandidates("none charged all");
    DrawCmd->AvailableForStates(G4State_Idle);
}


CEventActionMessenger::~CEventActionMessenger() {
    delete DrawCmd;
}


void CEventActionMessenger::SetNewValue(G4UIcommand * command,G4String newValue) {
    if(command == DrawCmd) {
        eventAction->SetDrawFlag(newValue);
    }
}





