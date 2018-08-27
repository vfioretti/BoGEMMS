/* ==========================================================================
 #
 # Copyright (C) 2010 INAF - OAS Bologna
 # Authors:  Andrea Bulgarelli <andrea.bulgarelli@inaf.it>
 #           Valentina Fioretti <valentina.fioretti@inaf.it>
 #
 # This program is free software: you can redistribute it and/or modify
 # it under the terms of the GNU General Public License as published by
 # the Free Software Foundation, either version 3 of the License, or
 # (at your option) any later version.
 #
 # This program is distributed in the hope that it will be useful,
 # but WITHOUT ANY WARRANTY; without even the implied warranty of
 # MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 # GNU General Public License for more details.
 #
 # You should have received a copy of the GNU General Public License
 # along with this program.  If not, see <http://www.gnu.org/licenses/>.
 #
 # ==========================================================================*/

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


#include "G4RunManager.hh"
#include "G4MTRunManager.hh"
#include "G4UImanager.hh"
#include "G4UIGAG.hh"
#include "G4UIterminal.hh"
#include "G4UItcsh.hh"
//#include "G4UIQt.hh"
#include "G4VisExecutive.hh"
#include "CEventAction.hh"
#include "CRunAction.hh"
#include "CSteppingAction.hh"
#include "CPrimaryGeneratorAction.hh"
#include "version.h"
#include <vector>
#include "Randomize.hh"
#include "GeometryBaseConstruction.hh"
#include "THELGlobalMemory.hh"
#include "THELSatellite.hh"
#include "CActionInitialization.hh"


// GDML
#include "G4GDMLParser.hh"
#include "G4VPhysicalVolume.hh"

#include <iostream>
using namespace std;

#include "CPhysicsListBase1.hh"
#include "CPhysicsListPhot.hh"

#ifndef WITHOUTROOT
#include "ConvertRootToFits.hh"
#endif

int main( int argc, char** argv ) {
    // Choose the Random engine
    //
    G4cout << argc << G4endl;
    G4cout << "/n#######################################################" << G4endl;
    G4cout << "BoGEMMS" << G4endl;
    G4cout << "Version: " << VERSION2 << G4endl;
    G4cout << "#######################################################" << G4endl;

#ifndef WITHOUTROOT
    if( argc == 5) {
        gm.configFileName = argv[1];
        gm.OpenConfigFile();
        cout << "Config file name: " << gm.configFileName << endl;
        G4cout << "********************* argc = 5 *************************** " << G4endl;
        //.conf
        //nome del file root da convertire
        //numero del file fits di output
        //dummy ma obblgatorio
        gm.nfileFITSHit = atoi(argv[3]);
        gm.nfileFITSINOUT = atoi(argv[3]);
        gm.nfileFITSXYZ = atoi(argv[3]);
        gm.nfileFITSENERGYVOL = atoi(argv[3]);
        gm.nfileRootENERGYVOL = atoi(argv[3]);
		gm.nfileRootXYZ = atoi(argv[3]);
        int numberFile = atoi(argv[3]);
        cout << "Start number of output file: " << numberFile << endl;
        ConvertRootToFits convert1;
        gm.compressionlevel = atoi(argv[4]);
        convert1.Convert(argv[2]);
        return 0;
    }
#endif
    if(argc > 1) {
        // Create a pointer to the User Interface manager
        gm.configFileName = argv[1];
        gm.OpenConfigFile();
        cout << "Config file name: " << gm.configFileName << endl;

        //CLHEP::HepRandom::setTheEngine(new CLHEP::RanecuEngine);
        G4Random::setTheEngine(new CLHEP::RanecuEngine);
        
        // Construct the default run manager
        
        G4bool MTFlag = 0;
        gm.config->readInto(MTFlag, "RUN.MT.ACTIVATE");
        cout << "RUN.MT.ACTIVATE: " << MTFlag << endl;
        if (!MTFlag){
            G4RunManager * runManager = new G4RunManager;
            // set mandatory initialization classes
            THELSatellite* nhxm = new THELSatellite;
            runManager->SetUserInitialization(nhxm) ;
            
            gm.config->readInto(gm.DOWNLOADGEOMETRY, "GEOM.PRINT_GEOMETRY");
            cout << "GEOM.PRINT_GEOMETRY: " << gm.DOWNLOADGEOMETRY << endl;
            
            if(gm.DOWNLOADGEOMETRY)
                exit(1);
            
            nhxm->DefinePhysicList(runManager);
            
            
            // set mandatory user action class
            runManager->SetUserAction(new CPrimaryGeneratorAction);
            runManager->SetUserAction(new CRunAction);
            runManager->SetUserAction(new CEventAction);
            runManager->SetUserAction(new CSteppingAction);
            
            // visualization manager
            G4VisManager* visManager = new G4VisExecutive;
            visManager->Initialize();
            
            //Initialize G4 kernel
            runManager->Initialize();
            cout << argc << endl;
            
        }
        if (MTFlag){
            
            G4MTRunManager* runManager = new G4MTRunManager;
            // set mandatory initialization classes
            THELSatellite* nhxm = new THELSatellite;
            runManager->SetUserInitialization(nhxm) ;
            
            gm.config->readInto(gm.DOWNLOADGEOMETRY, "GEOM.PRINT_GEOMETRY");
            cout << "GEOM.PRINT_GEOMETRY: " << gm.DOWNLOADGEOMETRY << endl;
            
            if(gm.DOWNLOADGEOMETRY)
                exit(1);
            
            nhxm->DefinePhysicList(runManager);
            
            runManager->SetUserInitialization(new CActionInitialization());
            
            
            // set mandatory user action class
            //runManager->SetUserAction(new CPrimaryGeneratorAction);
            //runManager->SetUserAction(new CRunAction);
            //runManager->SetUserAction(new CEventAction);
            //runManager->SetUserAction(new CSteppingAction);
            
            // visualization manager
            G4VisManager* visManager = new G4VisExecutive;
            visManager->Initialize();
            
            //Initialize G4 kernel
            //runManager->Initialize();
            //cout << argc << endl;
            
        }

       
        // GDML
        G4bool GDMLWrite = 0;
        gm.config->readInto(GDMLWrite, "GEOM.WRITE.GDML");
        cout << "GEOM.WRITE.GDML: " << GDMLWrite << endl;
        G4String  GDMLGeometryName = "";
        gm.config->readInto(GDMLGeometryName, "GEOM.WRITE.GDML.NAME");
        cout << "GEOM.WRITE.GDML.NAME: " << GDMLGeometryName << endl;
        if (GDMLWrite){
           G4VPhysicalVolume* pWorld = G4TransportationManager::GetTransportationManager()->GetNavigatorForTracking()->GetWorldVolume();
           G4GDMLParser parser;
           parser.Write(GDMLGeometryName, pWorld);
        }
		

        // get the pointer to the User Interface manager
        G4UImanager *UI = G4UImanager::GetUIpointer();
        G4UIsession * session;
        if ( argc==2 ) {
        	
			#ifdef G4UI_USE_TCSH
            	session = new G4UIterminal(new G4UItcsh);
			#else
            	session = new G4UIQt( argc, argv );
            	//session = new G4UIGAG;
			#endif
            
            //G4UIsession * session = new G4UIterminal;
            session->SessionStart();
    		
            delete session;
        }

        if ( argc==4 ) {
            gm.nfileFITSHit = atoi(argv[2]);
            gm.nfileFITSINOUT = atoi(argv[2]);
            gm.nfileFITSENERGYVOL = atoi(argv[2]);
            gm.nfileRootENERGYVOL = atoi(argv[2]);
			gm.nfileRootXYZ = atoi(argv[2]);
            int numberFile = atoi(argv[2]);
            cout << "Start number of output file: " << numberFile << endl;
            gm.InitFiles();
            G4String command = "/control/execute ";
            for (int i=3; i<argc; i++) {
                G4String macroFileName = argv[i];
                cout << "Macro file name: " << macroFileName << endl;
                UI->ApplyCommand(command+macroFileName);
            }

        }

    }
    if ( argc == 1 ) {
        cerr << "Sintax: " << endl;
        cerr << "./" << argv[0] << " <config file> <start outfile number> <mac file> " << endl;
        cerr << "Example: ./" << argv[0] << " base.conf 0 run_50kev.mac" << endl;
        return -1;
    }

    cout << "End of processing" << endl;

    // job termination

    return 0;
}
