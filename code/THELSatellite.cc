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

#include "THELSatellite.hh"
#include "globals.hh"
#include "G4VUserDetectorConstruction.hh"
#include "G4Element.hh"
#include "G4Material.hh"
#include "G4Box.hh"
#include "G4Cons.hh"
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4ThreeVector.hh"
#include "G4PVPlacement.hh"
#include "G4VisAttributes.hh"
#include "G4SubtractionSolid.hh"
#include "G4Region.hh"
#include "G4RegionStore.hh"
#include "ConfigFile.hh"
#include "THELGlobalMemory.hh"
#include "G4RunManager.hh"

#ifdef GEANT4_9_1
#include "CPhysicsList.hh"
#include "CPhysicsListBase1.hh"
#include "CPhysicsListPhot.hh"
#include "SSPhysicsList.hh"
#endif

#ifdef GEANT4_9_4
#include "PhysicsList.hh"
#include "QGSP_BERT.hh"
#endif

#ifdef GEANT4_9_6
#include "G4PhysListFactory.hh"
#include "ASTROMEVPhys.hh"
#endif

#if defined(GEANT4_10_1) || defined(GEANT4_10_2) || defined(GEANT4_10_3) || defined(GEANT4_10_4)
#include "G4PhysListFactory.hh"
#include "ASTROMEVPhys.hh"
#include "RdBProtoPhys.hh"
#endif

#if defined(GEANT4_10_1) || defined(GEANT4_10_2) || defined(GEANT4_10_3) || defined(GEANT4_10_4)
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"
#endif


#include "XrayTelPhysicsList.hh"

#include <iostream>
using namespace std;

#include "GeometryCaseA.hh"
#include "GeometryST.hh"
#include "GeometryST_CAL.hh"
#include "GeometryST_CAL_AGILE.hh"
#include "GeometryST_AC_AGILE.hh"
#include "GeometryST_SC.hh"
#include "RdBProto.hh"
#include "DummyXMM.hh"


THELSatellite::THELSatellite() {
    World_phys = 0;
}


THELSatellite::~THELSatellite() {}

void THELSatellite::DefinePhysicList(G4RunManager * runManager) {


    switch(gm.physVersion) {
    case 0: {
#ifdef GEANT4_9_1
        CPhysicsList* pPhysicsList = new CPhysicsList;
        runManager->SetUserInitialization(pPhysicsList);
        G4String name;
        gm.config->readInto(name, "ENERGYPROCESS.NAME");
        pPhysicsList->AddPhysicsList(name);
#endif
    }
    break;
    case 1: {
#ifdef GEANT4_9_1
        CPhysicsListBase1* physList = new CPhysicsListBase1;
        runManager->SetUserInitialization(physList);
#endif
    }
    break;
    case 2: {
#ifdef GEANT4_9_1
        CPhysicsListPhot* physList = new CPhysicsListPhot;
        runManager->SetUserInitialization(physList);
#endif
    }
    break;
    case 3: {
#ifdef GEANT4_9_4
        cout << "GEANT4_9_4" << endl;
        PhysicsList* pPhysicsList = new PhysicsList;
        runManager->SetUserInitialization(pPhysicsList);
        G4String name;
        gm.config->readInto(name, "ENERGYPROCESS.NAME");
        pPhysicsList->AddPhysicsList(name);
#endif
    }
    break;
    case 4: {
#ifdef GEANT4_9_4

        XrayTelPhysicsList* physList = new XrayTelPhysicsList;
        runManager->SetUserInitialization(physList);
#endif
    }
    break;
    case 200: {
#ifdef GEANT4_9_4
        runManager->SetUserInitialization(new QGSP_BERT);
#endif
    }
    break;
    case 300: {
#ifdef GEANT4_9_1
		//Space science physics list (Fermi)
        SSPhysicsList* physList = new SSPhysicsList;
        runManager->SetUserInitialization(physList);
#endif
    }
    break;

#ifdef GEANT4_9_6
	case 400: {
		
		///ASTRO-MEV
		//ENERGYPROCESS.VERSION = 400
		//HADRONPHYS.NAME = QGSP_BIC_HP
		ASTROMEVPhys *physList = new ASTROMEVPhys;
		runManager->SetUserInitialization(physList);
		G4String name;
		gm.config->readInto(name, "HADRONPHYS.NAME");
 		physList->AddPhysicsList(name);
	}
	break;
#endif
#if defined(GEANT4_10_1) || defined(GEANT4_10_2) || defined(GEANT4_10_3) || defined(GEANT4_10_4)
        case 400: {
            
            ///ASTRO-MEV
            //ENERGYPROCESS.VERSION = 400
            //HADRONPHYS.NAME = QGSP_BIC_HP
            ASTROMEVPhys *physList = new ASTROMEVPhys;
            runManager->SetUserInitialization(physList);
            G4String name;
            gm.config->readInto(name, "HADRONPHYS.NAME");
            physList->AddPhysicsList(name);
        }
            break;
        case 700: {
            
            ///RdB SLAB TEST
            //ENERGYPROCESS.VERSION = 700
            //HADRONPHYS.NAME = QGSP_BERT
            //EMPHYS.NAME = emlivermore
            RdBProtoPhys *physList = new RdBProtoPhys;
            runManager->SetUserInitialization(physList);
            G4String hadronname;
            gm.config->readInto(hadronname, "HADRONPHYS.NAME");
            G4String emname;
            gm.config->readInto(emname, "EMPHYS.NAME");
            physList->AddPhysicsList(emname);
            physList->AddPhysicsList(hadronname);
        }
            break;
#endif
			
#ifdef CPhysicsListAll_h
    //1000 old low energy em extension
    //1001 Livermore low energy em extension
    //1002 Penelope low energy em extension

    case 1000:
    case 1001:
    case 1002:
    {

        CPhysicsListAll* physList = new CPhysicsListAll(gm.physVersion);
        runManager->SetUserInitialization(physList);

    }
    break;
#endif
    case 10000: {

#ifdef GEANT4_9_6
        G4PhysListFactory *physListFactory = new G4PhysListFactory();
        G4VUserPhysicsList *physicsList = physListFactory->GetReferencePhysList("QGSP_BERT_LIV");
        runManager->SetUserInitialization(physicsList);
#endif
#if defined(GEANT4_10_1) || defined(GEANT4_10_2) || defined(GEANT4_10_3) || defined(GEANT4_10_4)
        G4PhysListFactory *physListFactory = new G4PhysListFactory();
        G4VUserPhysicsList *physicsList = physListFactory->GetReferencePhysList("QGSP_BERT_LIV");
        runManager->SetUserInitialization(physicsList);
#endif
    }
    break;
    case 10001: {

#ifdef GEANT4_9_6
	G4PhysListFactory *physListFactory = new G4PhysListFactory();
        G4VUserPhysicsList *physicsList = physListFactory->GetReferencePhysList("QGSP_BERT_EMV");                                             
	runManager->SetUserInitialization(physicsList);
#endif
#if defined(GEANT4_10_1) || defined(GEANT4_10_2) || defined(GEANT4_10_3) || defined(GEANT4_10_4)
        G4PhysListFactory *physListFactory = new G4PhysListFactory();
        G4VUserPhysicsList *physicsList = physListFactory->GetReferencePhysList("QGSP_BERT_EMV");
        runManager->SetUserInitialization(physicsList);
#endif
    }
    break;

	default: {
		cout << "Physic list version wrong: " << gm.physVersion << endl;
		exit(1);
	}
	break;
    }

    cout << "physics list ID " << gm.physVersion << endl;

}

G4VPhysicalVolume* THELSatellite::Construct() {

    //geometry definition
    switch(gm.geomVersion) {
    case 1: {
        cout << "Build geometry version 1" << endl;
        GeometryCaseA* detector = new GeometryCaseA();
        World_phys = detector->Construct();
        MakeSteppingVolumes();
    }
    break;
    case 9: {
        cout << "Build Small Telescope" << endl;
        GeometryST* detector = new GeometryST();
        World_phys = detector->Construct();
        MakeSteppingVolumes();

        bool GEOMCAL = false;
        gm.config->readInto(GEOMCAL, "GEOM.ST.CAL");
        cout << "GEOM.ST.CAL " << GEOMCAL << endl;

        if(GEOMCAL) {
            G4int GEOMCALTYPE;
            gm.config->readInto(GEOMCALTYPE, "GEOM.ST.CAL.TYPE");
            cout << "GEOM.ST.CAL.TYPE " << GEOMCALTYPE << endl;
            if(GEOMCALTYPE==1) {
                GeometryST_CAL_AGILE* stcal_agile = new GeometryST_CAL_AGILE();
                stcal_agile->SetPhysicalWorld(World_phys);
                stcal_agile->Construct();
            }
            if(GEOMCALTYPE==2) {
                GeometryST_CAL* stcal = new GeometryST_CAL();
                stcal->SetPhysicalWorld(World_phys);
                stcal->Construct();
            }
        }

        bool GEOMAC = false;
        gm.config->readInto(GEOMAC, "GEOM.ST.AC");
        cout << "GEOM.ST.AC " << GEOMAC << endl;

        if(GEOMAC) {
            GeometryST_AC_AGILE* stac = new GeometryST_AC_AGILE();
            stac->SetPhysicalWorld(World_phys);
            stac->Construct();
        }

        bool GEOMSC = false;
        gm.config->readInto(GEOMSC, "GEOM.ST.SC");
        cout << "GEOM.ST.SC " << GEOMSC << endl;

        if(GEOMSC) {
            GeometryST_SC* stsc = new GeometryST_SC();
            stsc->SetPhysicalWorld(World_phys);
            stsc->Construct();
        }

    }
    break;
    case 12: {
            cout << "Build RdBProto geometry" << endl;
            RdBProto* detector = new RdBProto();
            World_phys = detector->Construct();
            MakeSteppingVolumes();
    }
    break;
    case 13: {
            cout << "Build DummyXMM geometry" << endl;
            DummyXMM* detector = new DummyXMM();
            World_phys = detector->Construct();
            MakeSteppingVolumes();
    }
    break;
    
    case 1000: {
        cout << "Build geometry version 1000" << endl;
        GeometryBaseConstruction* detector = new GeometryBaseConstruction();
        World_phys = detector->Construct();
        MakeSteppingVolumes();
    }
    break;


    default:
        cout << "Geometry version wrong: " << gm.geomVersion << endl;
        exit(1);
    }
    return World_phys;
}

void THELSatellite::MakeSteppingVolumes() {
    G4int N;
    cout << "geometry ID " << gm.geomVersion << endl;
    //the stepping volumes
    gm.config->readInto(N, "STEPPINGVOL.N");
    cout << "STEPPINGVOL.N " << N << endl;
    for(G4int i = 0; i<N; i++)
        MakeSteppingVolume(i);
}

void THELSatellite::MakeSteppingVolume(G4int i) {
	
	cout << "MakeSteppingVolume number " << i + 1 << endl;

    G4double temperature, pressure, density;
    density     = universe_mean_density;
    pressure    = 3.e-18*pascal;
    temperature = 2.73*kelvin;
    G4String name, symbol;
    G4double a, z;
    G4Material* vacuum = new G4Material(name="Galactic", z=1., a=1.01*g/mole, density,kStateGas,temperature,pressure);

    char c[100];
    sprintf(c, "%d", i);
    G4String sname;
    sname = "G4STEPPINGVOLUME_S_";
    sname += c;

    G4Box* STSolid;
    G4Cons* STSolid2;
    G4SubtractionSolid* STSolid3;
    G4Box* STSolid3_full;
    G4Box* STSolid3_empty;
    G4LogicalVolume* STLogicalVolume;
    G4VPhysicalVolume* STPhysicalVolume;

    G4int type = 0;
    sprintf(c, "STEPPINGVOL.%i.type", i); //0 box, 1 cone, 2 empty box (1 mm thickness), 3 empty sphere, 4 sphere
    gm.config->readInto(type, c);
    cout << c << " " << type << endl;
    G4double base, height, diameter;
    G4double xpos, ypos, zpos;
    if(type == 0) {
        sprintf(c, "STEPPINGVOL.%i.dim.base", i);
        gm.config->readInto(base, c);
        cout << c << " " << base << endl;
    }
    if(type == 1 || type == 3 || type == 4) {
        sprintf(c, "STEPPINGVOL.%i.dim.diameter", i);
        gm.config->readInto(diameter, c);
        cout << c << " " << diameter << endl;
    }
    if(type == 2) {
        sprintf(c, "STEPPINGVOL.%i.dim.base", i);
        gm.config->readInto(base, c);
        cout << c << " " << base << endl;
    }

    sprintf(c, "STEPPINGVOL.%i.dim.height", i);
    gm.config->readInto(height, c);
    cout << c << " " << height << endl;
    sprintf(c, "STEPPINGVOL.%i.xpos", i);
    gm.config->readInto(xpos, c);
    cout << c << " " << xpos << endl;
    sprintf(c, "STEPPINGVOL.%i.ypos", i);
    gm.config->readInto(ypos, c);
    cout << c << " " << ypos << endl;
    sprintf(c, "STEPPINGVOL.%i.zpos", i);
    gm.config->readInto(zpos, c);
    cout << c << " " << zpos << endl;

    G4bool visible = false;
    sprintf(c, "STEPPINGVOL.%i.visible", i);
    gm.config->readInto(visible, c);
    cout << c << " " << visible << endl;

    G4bool sensitive = false;
    sprintf(c, "STEPPINGVOL.%i.sensitive", i);
    gm.config->readInto(sensitive, c);
    cout << c << " " << sensitive << endl;

    if(type == 0) {
        STSolid = new G4Box( sname, base/2.0, base/2.0, height/2.0);
        STLogicalVolume = new G4LogicalVolume(STSolid, vacuum, sname, 0, 0, 0 );
    }
    if(type == 1) {
        STSolid2 = new G4Cons( sname, 0.0, diameter/2.0, 0.0, diameter/2.0, height/2.0, 0*deg, 360.*deg);
        STLogicalVolume = new G4LogicalVolume(STSolid2, vacuum, sname, 0, 0, 0 );
    }
    if(type == 2) {
        STSolid3_full = new G4Box( sname += "_full", base/2.0, base/2.0, height/2.0);
        STSolid3_empty = new G4Box( sname += "_empty", (base-2.0)/2.0, (base-2.0)/2.0, (height-2.0)/2.0);
        STSolid3 = new G4SubtractionSolid(sname, STSolid3_full, STSolid3_empty);
        STLogicalVolume = new G4LogicalVolume(STSolid3, vacuum, sname, 0, 0, 0 );
    }
	if(type == 3) {
		G4Sphere* STSolid = new G4Sphere(sname, (diameter-height)/2.0, diameter/2.0, 0*deg,  360*deg, 0*deg, 180*deg);
		STLogicalVolume = new G4LogicalVolume(STSolid, vacuum, sname, 0, 0, 0 );
	}
    if(type == 4) {
        G4Sphere* STSolid = new G4Sphere(sname, 0., diameter/2.0, 0*deg,  360*deg, 0*deg, 180*deg);
        STLogicalVolume = new G4LogicalVolume(STSolid, vacuum, sname, 0, 0, 0 );
    }
    sname = "G4STEPPINGVOLUME_L_";
    sname += c;

    G4VisAttributes* VisST = new G4VisAttributes(G4Colour(255/255., 255/255., 255/255.) );
    STLogicalVolume->SetVisAttributes( VisST );

    // Rotation Matrix
    G4RotationMatrix *rotateMatrix = new G4RotationMatrix();
    // 	rotateMatrix -> rotateY(90.*deg);



    sname = "G4STEPPINGVOLUME_P_";
    sname += c;
    STPhysicalVolume = new G4PVPlacement(rotateMatrix,
                                         G4ThreeVector( xpos, ypos, zpos ),
                                         "G4_P",
                                         STLogicalVolume,
                                         World_phys, false, 0 );

    if(sensitive) gm.AddXYZDetector(STLogicalVolume);

    //--- Make Invisible
    if(!visible) STLogicalVolume -> SetVisAttributes( G4VisAttributes::Invisible );
    STPhysicalVolume->SetCopyNo(i+1);
    gm.AddSensitiveDetector( STPhysicalVolume );

}



