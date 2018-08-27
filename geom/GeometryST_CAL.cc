/***************************************************************************
                          GeometryST_CAL.cc  -  description
                             -------------------
	                  Building the Gamma-Light Calorimeter
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

#include "GeometryST_CAL.hh"
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

GeometryST_CAL::GeometryST_CAL() {

    World_phys = 0;
    World_log = 0;
    //VacuumMaterial = 0;
    materials = new MaterialsDefinition;
}

GeometryST_CAL::~GeometryST_CAL() {}

G4VPhysicalVolume* GeometryST_CAL::Construct() {

    //DefineMaterials();

    //World_phys = gm.ConstructWorld();
    //World_log = gm.World_log;
    G4cout << "##################################" << G4endl;
    G4cout << "ASTROMEV Calorimeter construct" << G4endl;
    G4cout << "##################################" << G4endl;

    World_phys = ConstructGeometry(World_phys);

    return World_phys;


}

void  GeometryST_CAL::SetPhysicalWorld(G4VPhysicalVolume* World_phys) {
    this->World_phys = World_phys;
    this->World_log = World_phys->GetLogicalVolume();
}

G4VPhysicalVolume* GeometryST_CAL::ConstructGeometry(G4VPhysicalVolume* World_phys) {

    G4double zero = 0.;
    G4double pis2 = asin(1.0);
    G4double pi = 2.0 * pis2;
    G4double twopi = 2.0 * pi;
    G4RotationMatrix rot45Z,rot90Z, rot180Z, rot270Z;

    //Rotations
    G4RotationMatrix* no_rotation = new G4RotationMatrix(0,0,0);
    rot45Z.rotateZ(pis2/2.);
    rot90Z.rotateZ(pis2);
    rot180Z.rotateZ(pi);
    rot270Z.rotateZ(3.*pis2);

    
    gm.config->readInto(visAllBlack, "GEOM.ST.VIS.BLACK");
    G4cout << "GEOM.ST.VIS.BLACK: " << visAllBlack << G4endl;

    gm.config->readInto(visFlag, "GEOM.ST.CAL.VIS");
    G4cout << "GEOM.ST.CAL.VIS: " << visFlag << G4endl;

    gm.config->readInto(CalDist, "GEOM.ST.CAL.DISTANCE");
    G4cout << "GEOM.ST.CAL.DISTANCE: " << CalDist << G4endl;
    gm.config->readInto(CalPixelSide, "GEOM.ST.CAL.PIXEL.SIDE");
    G4cout << "GEOM.ST.CAL.PIXEL.SIDE: " << CalPixelSide << G4endl;
    gm.config->readInto(CalHeight, "GEOM.ST.CAL.HEIGHT");
    G4cout << "GEOM.ST.CAL.HEIGHT: " << CalHeight << G4endl;
    gm.config->readInto(CalSide, "GEOM.ST.CAL.SIDE");
    G4cout << "GEOM.ST.CAL.SIDE: " << CalSide << G4endl;
    // SDD thickness
    gm.config->readInto(SDD_t, "GEOM.ST.CAL.SDD.THICKNESS");
    G4cout << "GEOM.ST.CAL.SDD.THICKNESS: " << SDD_t << G4endl;
    // Circuit board (CB) thickness
    gm.config->readInto(CB_t, "GEOM.ST.CAL.CB.THICKNESS");
    G4cout << "GEOM.ST.CAL.CB.THICKNESS: " << CB_t << G4endl;
    
    // Calorimeter pixels
    CalNpixX = CalSide/CalPixelSide;
    CalNpixY = CalSide/CalPixelSide;
    G4int CalNpix = CalNpixX*CalNpixY;

    Cal_mat = materials->GetMaterial(12);
    CalPixel_mat = materials->GetMaterial(14);

    // SDD and CB material
    SDD_mat = materials->GetMaterial(9);
    CB_mat = materials->GetMaterial(29);

    
    // Solid and logical volumes
    Cal_sol = new G4Box("sCal", CalSide/2.0, CalSide/2.0, CalHeight/2.0);
    CalPixel_sol = new G4Box("sCalPixel", CalPixelSide/2., CalPixelSide/2., CalHeight/2.0);
    SDD_sol = new G4Box("sSDD", CalSide/2.0, CalSide/2.0, SDD_t/2.0);
    CB_sol = new G4Box("sCB", CalSide/2.0, CalSide/2.0, CB_t/2.0);

    Cal_log = new G4LogicalVolume(Cal_sol, Cal_mat, "lCal",0,0,0);
    CalPixel_log = new G4LogicalVolume(CalPixel_sol, CalPixel_mat, "lCalPixel",0,0,0);
    SDD_log = new G4LogicalVolume(SDD_sol, SDD_mat, "lSDD",0,0,0);
    CB_log = new G4LogicalVolume(CB_sol, CB_mat, "lCB",0,0,0);

    
    // SDD and CB Top
    G4VPhysicalVolume* SDDTop_phys;
    G4VPhysicalVolume* CBTop_phys;
    
    G4ThreeVector SDDTopPos(0.0, 0.0, -(CalDist + (SDD_t/2.0)));
    G4ThreeVector CBTopPos(0.0, 0.0, -(CalDist + SDD_t + (CB_t/2.0)));
    
    SDDTop_phys = new G4PVPlacement(0, SDDTopPos, SDD_log, "pSDD", World_log, false,49600);
    CBTop_phys = new G4PVPlacement(0, CBTopPos, CB_log, "pCB", World_log, false,49700);
    
    G4ThreeVector CalPos(0.0, 0.0, -(CalDist + SDD_t + CB_t + (CalHeight/2.0)));
    Cal_phys = new G4PVPlacement(0, CalPos, Cal_log, "pCal", World_log, false,0);

    
    G4VPhysicalVolume* CalPixel_phys;

    G4int copyNoPixel = 50000;

    for (G4int ip=0; ip<CalNpixY; ip++)
    {
        for (G4int jp=0; jp<CalNpixX; jp++)
        {
            G4ThreeVector CalPixelPos((-CalSide/2.) + (jp*CalPixelSide) + (CalPixelSide/2.0), (CalSide/2.) + (-ip*CalPixelSide) + (-CalPixelSide/2.0), 0.0);
            CalPixel_phys = new G4PVPlacement(0, CalPixelPos, CalPixel_log, "pCalPixel", Cal_log, false,copyNoPixel);
            copyNoPixel++;
        }
    }

    // SDD and CB bottom
    G4VPhysicalVolume* CBBottom_phys;
    G4VPhysicalVolume* SDDBottom_phys;
    
    G4ThreeVector CBBottomPos(0.0, 0.0, -(CalDist + SDD_t + CB_t + CalHeight + (CB_t/2.0)));
    G4ThreeVector SDDBottomPos(0.0, 0.0, -(CalDist + SDD_t + CB_t + CalHeight + CB_t + (SDD_t/2.0)));
    
    CBBottom_phys = new G4PVPlacement(0, CBBottomPos, CB_log, "pCB", World_log, false,49800);
    SDDBottom_phys = new G4PVPlacement(0, SDDBottomPos, SDD_log, "pSDD", World_log, false,49900);

    black_visAtt = new G4VisAttributes(G4Colour::Black());
    G4VisAttributes*green_visAtt = new G4VisAttributes(G4Colour::Green());
    G4VisAttributes*red_visAtt = new G4VisAttributes(G4Colour::Red());
    G4VisAttributes*blue_visAtt = new G4VisAttributes(G4Colour::Blue());
    if (visAllBlack) {
        CalPixel_log->SetVisAttributes( red_visAtt );
        SDD_log->SetVisAttributes( black_visAtt );
        CB_log->SetVisAttributes( green_visAtt );
    } else {
        if (visFlag) {
            CalPixel_visAtt = new G4VisAttributes(G4Colour::Yellow());
            CalPixel_log->SetVisAttributes( CalPixel_visAtt );
            SDD_log->SetVisAttributes( black_visAtt );
            CB_log->SetVisAttributes( green_visAtt );
        }
        else {
            CalPixel_log -> SetVisAttributes (G4VisAttributes::GetInvisible());
            SDD_log->SetVisAttributes( G4VisAttributes::GetInvisible());
            CB_log->SetVisAttributes( G4VisAttributes::GetInvisible() );
        }
    }
    gm.AddXYZDetector( CalPixel_log);



    return World_phys;
}
