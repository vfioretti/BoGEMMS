/***************************************************************************
                          GeometryST_AC.cc  -  description
                             -------------------
	                  Building the Gamma-Light AC
    begin                : 2012
    copyright            : (C) 2012 by  Valentina Fioretti
    email                : fioretti@iasfbo.inaf.it
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software for non commercial purpose              *
 *   and for public research institutes; you can redistribute it and/or    *
 *   modify it under the terms of the GNU General Public License.          *
 *   For commercial purpose see appropriate license terms                  *
 *                                                                         *
 ***************************************************************************/

#include "GeometryST_AC.hh"
#include "G4Element.hh"
#include "G4Material.hh"
#include "G4Box.hh"
#include "G4Cons.hh"
#include "G4Polyhedra.hh"
#include "G4Tubs.hh"
#include "G4Sphere.hh"
#include "G4LogicalVolume.hh"
#include "G4ThreeVector.hh"
#include "G4PVPlacement.hh"
#include "G4VisAttributes.hh"
#include "G4SubtractionSolid.hh"
#include "G4Region.hh"
#include "G4RegionStore.hh"
#include "THELGlobalMemory.hh"
#include "globals.hh"

#include "CXYZSD.hh"
#include "G4SDManager.hh"
#include "GeometryST.hh"

GeometryST_AC::GeometryST_AC() {

    World_phys = 0;
    World_log = 0;
    //VacuumMaterial = 0;
    materials = new MaterialsDefinition;
    stget = new GeometryST;
}

GeometryST_AC::~GeometryST_AC() {}

G4VPhysicalVolume* GeometryST_AC::Construct() {

    //DefineMaterials();

    //World_phys = gm.ConstructWorld();
    //World_log = gm.World_log;
    G4cout << "##################################" << G4endl;
    G4cout << "GAMMA-LIGHT AC construct" << G4endl;
    G4cout << "##################################" << G4endl;
    World_phys = ConstructGeometry(World_phys);

    return World_phys;


}

void  GeometryST_AC::SetPhysicalWorld(G4VPhysicalVolume* World_phys) {
    this->World_phys = World_phys;
    this->World_log = World_phys->GetLogicalVolume();
}

G4VPhysicalVolume* GeometryST_AC::ConstructGeometry(G4VPhysicalVolume* World_phys) {

    G4double zero = 0.;
    G4double pis2 = asin(1.0);
    G4double pi = 2.0 * pis2;
    G4double twopi = 2.0 * pi;

    gm.config->readInto(visAllBlack, "GEOM.ST.VIS.BLACK");
    G4cout << "GEOM.ST.VIS.BLACK: " << visAllBlack << G4endl;

    gm.config->readInto(visACFlag, "GEOM.ST.AC.VIS");
    G4cout << "GEOM.ST.AC.VIS: " << visACFlag << G4endl;
    gm.config->readInto(viewInside, "GEOM.ST.VIEW.INSIDE");
    G4cout << "GEOM.ST.VIEW.INSIDE: " << viewInside << G4endl;

    gm.config->readInto(visACTopFlag, "GEOM.ST.AC.TOP.VIS");
    G4cout << "GEOM.ST.AC.TOP.VIS: " << visACTopFlag << G4endl;

    gm.config->readInto(CaseAC, "GEOM.ST.AC.TYPE");
    G4cout << "GEOM.ST.AC.VIS: " << CaseAC << G4endl;

    gm.config->readInto(SensAC, "GEOM.ST.AC.SENSITIVE");
    G4cout << "GEOM.ST.AC.SENSITIVE: " << SensAC << G4endl;


    gm.config->readInto(ACDist, "GEOM.ST.AC.DISTANCE");
    G4cout << "GEOM.ST.AC.DISTANCE: " << ACDist << G4endl;
    gm.config->readInto(ACBottomDist, "GEOM.ST.AC.BOTTOM.DISTANCE");
    G4cout << "GEOM.ST.AC.BOTTOM.DISTANCE: " << ACBottomDist << G4endl;

    gm.config->readInto(ACLat_t, "GEOM.ST.AC.LAT.THICKNESS");
    G4cout << "GEOM.ST.AC.LAT.THICKNESS: " << ACLat_t << G4endl;
    gm.config->readInto(ACLat_extra, "GEOM.ST.AC.LAT.EXTRA");
    G4cout << "GEOM.ST.AC.LAT.EXTRA: " << ACLat_extra << G4endl;
    gm.config->readInto(ACTop_t, "GEOM.ST.AC.TOP.THICKNESS");
    G4cout << "GEOM.ST.AC.TOP.THICKNESS: " << ACTop_t << G4endl;
    gm.config->readInto(ACBottom_t, "GEOM.ST.AC.BOTTOM.THICKNESS");
    G4cout << "GEOM.ST.AC.BOTTOM.THICKNESS: " << ACBottom_t << G4endl;

    G4double Elec_dist;
    G4double Elec_t;
    G4double Trk_side;
    G4double Trk_h;
    G4double Elec_tot_side;
    G4double Trk_Cal_dist;
    G4double Cal_h;
    G4double Trk_Zstart;

    gm.config->readInto(Elec_dist, "GEOM.ST.ELEC.DISTANCE");
    gm.config->readInto(Trk_side, "GEOM.ST.TRAY.SIDE");
    gm.config->readInto(Elec_t, "GEOM.ST.ELEC.THICKNESS");
    gm.config->readInto(Cal_h, "GEOM.ST.CAL.HEIGHT");

    Elec_tot_side = Trk_side + (2.0*Elec_dist) + (2.0*Elec_t);
    ACLat_total_side = Elec_tot_side + 2.0*ACDist + ACLat_extra;  // Adding extra length to divide lateral panels
    ACLat_side = ACLat_total_side/3.0;
    Trk_h = stget->ComputeTrackerHeight();
    Trk_Cal_dist = stget->ComputeTrkCalDistance();
    Trk_Zstart = stget->ComputeTrkZstart();

    G4cout << "TRACKER - CALORIMETER DISTANCE: " << Trk_Cal_dist << G4endl;
    G4cout << "TRACKER Z START: " << Trk_Zstart << G4endl;
    G4cout << "AC lateral panel side: " << ACLat_side << G4endl;
    G4cout << "AC lateral total side:" << ACLat_total_side << G4endl;

    switch(CaseAC) {
    case 1:
        ACLat_h = Trk_h + (1.0*ACDist) + Trk_Cal_dist + Cal_h;
        break;
    case 2:
        ACLat_h = Trk_h + (1.0*ACDist) + Trk_Cal_dist + Cal_h;
        break;
    case 3:
        ACLat_h = Trk_h + (1.0*ACDist) + ACBottomDist + Trk_Zstart + ACBottom_t;
        break;
    }



    AC_mat = materials->GetMaterial(26);

    // LATERAL AC
    ACLatX_sol = new G4Box("sACLatX", ACLat_t/2.0, ACLat_side/2.0, ACLat_h/2.0);
    ACLatX_log = new G4LogicalVolume(ACLatX_sol, AC_mat, "lACLatX",0,0,0);

    // Lateral AC  -X
    G4ThreeVector ACLat_pos1minX(-((Elec_tot_side + ACLat_t)/2.0 + ACDist), +((Elec_tot_side)/2.0 + ACDist - ACLat_side/2.0), +(Trk_Zstart + Trk_h + ACDist - (ACLat_h/2.0)));
    ACLatX_phys = new G4PVPlacement(0, ACLat_pos1minX, ACLatX_log, "pACLatX", World_log, false,301);
    G4ThreeVector ACLat_pos2minX(-((Elec_tot_side + ACLat_t)/2.0 + ACDist), +((Elec_tot_side)/2.0 + ACDist - ACLat_side - ACLat_side/2.0), +(Trk_Zstart + Trk_h + ACDist - (ACLat_h/2.0)));
    ACLatX_phys = new G4PVPlacement(0, ACLat_pos2minX, ACLatX_log, "pACLatX", World_log, false,302);
    G4ThreeVector ACLat_pos3minX(-((Elec_tot_side + ACLat_t)/2.0 + ACDist), +((Elec_tot_side)/2.0 + ACDist - (2.0*ACLat_side) - ACLat_side/2.0), +(Trk_Zstart + Trk_h + ACDist - (ACLat_h/2.0)));
    ACLatX_phys = new G4PVPlacement(0, ACLat_pos3minX, ACLatX_log, "pACLatX", World_log, false,303);

    // Lateral AC  +X
    G4ThreeVector ACLat_pos1plusX(+((Elec_tot_side + ACLat_t)/2.0 + ACDist), -((Elec_tot_side)/2.0 + ACDist - ACLat_side/2.0), +(Trk_Zstart + Trk_h + ACDist - (ACLat_h/2.0)));
    ACLatX_phys = new G4PVPlacement(0, ACLat_pos1plusX, ACLatX_log, "pACLatX", World_log, false,304);
    G4ThreeVector ACLat_pos2plusX(+((Elec_tot_side + ACLat_t)/2.0 + ACDist), -((Elec_tot_side)/2.0 + ACDist - ACLat_side - ACLat_side/2.0), +(Trk_Zstart + Trk_h + ACDist - (ACLat_h/2.0)));
    ACLatX_phys = new G4PVPlacement(0, ACLat_pos2plusX, ACLatX_log, "pACLatX", World_log, false,305);
    G4ThreeVector ACLat_pos3plusX(+((Elec_tot_side + ACLat_t)/2.0 + ACDist), -((Elec_tot_side)/2.0 + ACDist - (2.0*ACLat_side) - ACLat_side/2.0), +(Trk_Zstart + Trk_h + ACDist - (ACLat_h/2.0)));
    ACLatX_phys = new G4PVPlacement(0, ACLat_pos3plusX, ACLatX_log, "pACLatX", World_log, false,306);

    ACLatY_sol = new G4Box("sACLatY", ACLat_side/2.0, ACLat_t/2.0, ACLat_h/2.0);
    ACLatY_log = new G4LogicalVolume(ACLatY_sol, AC_mat, "lACLatY",0,0,0);

    // Lateral AC  -Y
    G4ThreeVector ACLat_pos1minY(-((Elec_tot_side)/2.0 + ACDist - ACLat_side/2.0), -((Elec_tot_side + ACLat_t)/2.0 + ACDist), +(Trk_Zstart + Trk_h + ACDist - (ACLat_h/2.0)));
    ACLatY_phys = new G4PVPlacement(0, ACLat_pos1minY, ACLatY_log, "pACLatY", World_log, false,307);
    G4ThreeVector ACLat_pos2minY(-((Elec_tot_side)/2.0 + ACDist - ACLat_side - ACLat_side/2.0), -((Elec_tot_side + ACLat_t)/2.0 + ACDist), +(Trk_Zstart + Trk_h + ACDist - (ACLat_h/2.0)));
    ACLatY_phys = new G4PVPlacement(0, ACLat_pos2minY, ACLatY_log, "pACLatY", World_log, false,308);
    G4ThreeVector ACLat_pos3minY(-((Elec_tot_side)/2.0 + ACDist - (2.0*ACLat_side) - ACLat_side/2.0), -((Elec_tot_side + ACLat_t)/2.0 + ACDist), +(Trk_Zstart + Trk_h + ACDist - (ACLat_h/2.0)));
    ACLatY_phys = new G4PVPlacement(0, ACLat_pos3minY, ACLatY_log, "pACLatY", World_log, false,309);

    // Lateral AC  +Y
    G4ThreeVector ACLat_pos1plusY(+((Elec_tot_side)/2.0 + ACDist - ACLat_side/2.0), +((Elec_tot_side + ACLat_t)/2.0 + ACDist), +(Trk_Zstart + Trk_h + ACDist - (ACLat_h/2.0)));
    ACLatY_phys = new G4PVPlacement(0, ACLat_pos1plusY, ACLatY_log, "pACLatY", World_log, false,310);
    G4ThreeVector ACLat_pos2plusY(+((Elec_tot_side)/2.0 + ACDist - ACLat_side - ACLat_side/2.0), +((Elec_tot_side + ACLat_t)/2.0 + ACDist), +(Trk_Zstart + Trk_h + ACDist - (ACLat_h/2.0)));
    ACLatY_phys = new G4PVPlacement(0, ACLat_pos2plusY, ACLatY_log, "pACLatY", World_log, false,311);
    G4ThreeVector ACLat_pos3plusY(+((Elec_tot_side)/2.0 + ACDist - (2.0*ACLat_side) - ACLat_side/2.0), +((Elec_tot_side + ACLat_t)/2.0 + ACDist), +(Trk_Zstart + Trk_h + ACDist - (ACLat_h/2.0)));
    ACLatY_phys = new G4PVPlacement(0, ACLat_pos3plusY, ACLatY_log, "pACLatY", World_log, false,312);

    // TOP AC
    ACTop_sol = new G4Box("sACTop", (ACLat_total_side - ACLat_extra + (2.0*ACLat_t))/2.0, (ACLat_total_side - ACLat_extra + (2.0*ACLat_t))/2.0, ACTop_t/2.0);
    ACTop_log = new G4LogicalVolume(ACTop_sol, AC_mat, "lACTop",0,0,0);

    G4cout << "AC top panel side: " << ACLat_total_side - ACLat_extra + (2.0*ACLat_t) << G4endl;


    G4ThreeVector ACTop_pos(0.0, 0.0, Trk_Zstart + Trk_h + ACDist + (ACTop_t/2.0));
    ACTop_phys = new G4PVPlacement(0, ACTop_pos, ACTop_log, "pACTop", World_log, false,320);

    if (SensAC) {
        gm.AddXYZDetector( ACLatX_log);
        gm.AddXYZDetector( ACLatY_log);
        gm.AddXYZDetector( ACTop_log);
    }

    // BOTTOM AC
    ACBottom_sol = new G4Box("sACBottom", (ACLat_total_side - ACLat_extra)/2.0, (ACLat_total_side - ACLat_extra)/2.0, ACBottom_t/2.0);
    ACBottom_log = new G4LogicalVolume(ACBottom_sol, AC_mat, "lACBottom",0,0,0);

    G4cout << "AC bottom panel side: " << ACLat_total_side - ACLat_extra << G4endl;



    G4ThreeVector ACBottom_pos(0.0, 0.0, - ACBottomDist - (ACBottom_t/2.0));
    if (CaseAC != 1) {
        ACBottom_phys = new G4PVPlacement(0, ACBottom_pos, ACBottom_log, "pACBottom", World_log, false,330);
        if (SensAC) {
            gm.AddXYZDetector( ACBottom_log);
        }
    }

    // Visualization
    AC_visAtt = new G4VisAttributes(G4Colour::White());
    black_visAtt = new G4VisAttributes(G4Colour::Black());
    if (visAllBlack) {
        ACLatX_log->SetVisAttributes( black_visAtt );
        ACLatY_log->SetVisAttributes( black_visAtt);
        ACBottom_log ->SetVisAttributes( black_visAtt);
        ACTop_log->SetVisAttributes( black_visAtt);
    } else {
        if (visACFlag) {
            ACLatX_log->SetVisAttributes( AC_visAtt );
            ACLatY_log->SetVisAttributes( AC_visAtt );
            ACBottom_log->SetVisAttributes( AC_visAtt );
        } else {
            ACLatX_log -> SetVisAttributes (G4VisAttributes::GetInvisible());
            ACLatY_log -> SetVisAttributes (G4VisAttributes::GetInvisible());
            ACBottom_log-> SetVisAttributes (G4VisAttributes::GetInvisible());
        }
        if (visACTopFlag) {
            ACTop_log->SetVisAttributes( AC_visAtt );
        } else {
            ACTop_log-> SetVisAttributes (G4VisAttributes::GetInvisible());
        }
    }

    return World_phys;
}
