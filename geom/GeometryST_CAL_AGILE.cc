/***************************************************************************
                          GeometryST_CAL_AGILE.cc  -  description
                             -------------------
	                  Building the AGILE Calorimeter
    begin                : 2013
    copyright            : (C) 2013 by  Valentina Fioretti
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

#include "GeometryST_CAL_AGILE.hh"
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

GeometryST_CAL_AGILE::GeometryST_CAL_AGILE() {

    World_phys = 0;
    World_log = 0;
    //VacuumMaterial = 0;
    materials = new MaterialsDefinition;
}

GeometryST_CAL_AGILE::~GeometryST_CAL_AGILE() {}

G4VPhysicalVolume* GeometryST_CAL_AGILE::Construct() {

    //DefineMaterials();

    //World_phys = gm.ConstructWorld();
    //World_log = gm.World_log;
    G4cout << "##################################" << G4endl;
    G4cout << "AGILE Calorimeter construct" << G4endl;
    G4cout << "##################################" << G4endl;

    World_phys = ConstructGeometry(World_phys);

    return World_phys;


}

void  GeometryST_CAL_AGILE::SetPhysicalWorld(G4VPhysicalVolume* World_phys) {
    this->World_phys = World_phys;
    this->World_log = World_phys->GetLogicalVolume();
}

G4VPhysicalVolume* GeometryST_CAL_AGILE::ConstructGeometry(G4VPhysicalVolume* World_phys) {

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
    gm.config->readInto(CalBarSide, "GEOM.ST.CAL.BAR.SIDE");
    G4cout << "GEOM.ST.CAL.BAR.SIDE: " << CalBarSide << G4endl;
    gm.config->readInto(CalBarHeight, "GEOM.ST.CAL.BAR.HEIGHT");
    G4cout << "GEOM.ST.CAL.BAR.HEIGHT: " << CalBarHeight << G4endl;
    gm.config->readInto(CalBarsDist, "GEOM.ST.CAL.BARS.DISTANCE");
    G4cout << "GEOM.ST.CAL.BARS.DISTANCE: " << CalBarsDist << G4endl;

    // Carbin Fiber thickness given by the bar X/Y distance
    CF_t = CalBarsDist/2.0;

    CalNbarX = 15; // N pixel in x
    CalNbarY = 15; // N pixel in y

    // Calorimeter total side
    CalSide = CalBarSide*CalNbarX + 2.*CF_t*CalNbarX;
    G4cout << "Calorimeter total side: " << CalSide << G4endl;

    Cal_mat = materials->GetMaterial(12);
    CalBar_mat = materials->GetMaterial(14);

    CalX_sol = new G4Box("sCalX", CalSide/2.0, CalSide/2.0, CalBarHeight/2.0);
    CalY_sol = new G4Box("sCalY", CalSide/2.0, CalSide/2.0, CalBarHeight/2.0);

    CalBarX_sol = new G4Box("sCalBarX", CalBarSide/2., (CalSide - 2.*CF_t)/2., CalBarHeight/2.0);
    CalBarY_sol = new G4Box("sCalBarY", (CalSide - 2.*CF_t)/2., CalBarSide/2., CalBarHeight/2.0);

    CalX_log = new G4LogicalVolume(CalX_sol, Cal_mat, "lCalX",0,0,0);
    CalY_log = new G4LogicalVolume(CalY_sol, Cal_mat, "lCalY",0,0,0);

    CalBarX_log = new G4LogicalVolume(CalBarX_sol, CalBar_mat, "lCalBarX",0,0,0);
    CalBarY_log = new G4LogicalVolume(CalBarY_sol, CalBar_mat, "lCalBarY",0,0,0);

    G4ThreeVector CalXPos(-7.0, -7.0, -(CalDist + CF_t + (CalBarHeight/2.0)));
    G4ThreeVector CalYPos(-7.0, -7.0, -(CalDist + CF_t + CalBarHeight + CalBarsDist + (CalBarHeight/2.0)));

    CalX_phys = new G4PVPlacement(0, CalXPos, CalX_log, "pCalX", World_log, false,0);
    CalY_phys = new G4PVPlacement(0, CalYPos, CalY_log, "pCalY", World_log, false,0);

    G4int copyNoBarX = 50000;
    G4int copyNoBarY = 60000;

    for (G4int ib=0; ib<CalNbarX; ib++)
    {
        // XBAR ID starts from -Xv (-Yk)
        G4ThreeVector CalBarXPos((-CalSide/2. + CF_t) + (ib*(CalBarSide+(2.*CF_t))) + (CalBarSide/2.0), 0.0, 0.0);
        CalBarX_phys = new G4PVPlacement(0, CalBarXPos, CalBarX_log, "pCalBarX", CalX_log, false,copyNoBarX);
        copyNoBarX++;
        // YBAR ID starts from +Yv (-Xk)
        G4ThreeVector CalBarYPos(0.0, (+CalSide/2. - CF_t) - (ib*(CalBarSide+(2.*CF_t))) - (CalBarSide/2.0), 0.0);
        CalBarY_phys = new G4PVPlacement(0, CalBarYPos, CalBarY_log, "pCalBarY", CalY_log, false,copyNoBarY);
        copyNoBarY++;
    }


    // Visualization
    CalX_log -> SetVisAttributes (G4VisAttributes::GetInvisible());
    CalY_log -> SetVisAttributes (G4VisAttributes::GetInvisible());

    black_visAtt = new G4VisAttributes(G4Colour::Black());
    G4VisAttributes*red_visAtt = new G4VisAttributes(G4Colour::Red());
    G4VisAttributes*blue_visAtt = new G4VisAttributes(G4Colour::Blue());
    if (visAllBlack) {
        CalBarX_log->SetVisAttributes( red_visAtt );
        CalBarY_log->SetVisAttributes( red_visAtt );
    } else {
        if (visFlag) {
            CalBar_visAtt = new G4VisAttributes(G4Colour::Yellow());
            CalBarX_log->SetVisAttributes( CalBar_visAtt );
            CalBarY_log->SetVisAttributes( CalBar_visAtt );
        }
        else {
            CalBarX_log -> SetVisAttributes (G4VisAttributes::GetInvisible());
            CalBarY_log -> SetVisAttributes (G4VisAttributes::GetInvisible());
        }
    }
    gm.AddXYZDetector( CalBarX_log);
    gm.AddXYZDetector( CalBarY_log);



    return World_phys;
}
