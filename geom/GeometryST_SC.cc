/***************************************************************************
                          GeometryST_SC.cc  -  description
                             -------------------
	                  Building the Gamma-Light Spacecraft
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

#include "GeometryST_SC.hh"
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

GeometryST_SC::GeometryST_SC() {

    World_phys = 0;
    World_log = 0;
    //VacuumMaterial = 0;
    materials = new MaterialsDefinition;
    stget = new GeometryST;
}

GeometryST_SC::~GeometryST_SC() {}

G4VPhysicalVolume* GeometryST_SC::Construct() {

    //DefineMaterials();

    //World_phys = gm.ConstructWorld();
    //World_log = gm.World_log;
    G4cout << "##################################" << G4endl;
    G4cout << "GAMMA-LIGHT Spacecraft construct" << G4endl;
    G4cout << "##################################" << G4endl;
    World_phys = ConstructGeometry(World_phys);

    return World_phys;


}

void  GeometryST_SC::SetPhysicalWorld(G4VPhysicalVolume* World_phys) {
    this->World_phys = World_phys;
    this->World_log = World_phys->GetLogicalVolume();
}

G4VPhysicalVolume* GeometryST_SC::ConstructGeometry(G4VPhysicalVolume* World_phys) {

    G4double zero = 0.;
    G4double pis2 = asin(1.0);
    G4double pi = 2.0 * pis2;
    G4double twopi = 2.0 * pi;

    gm.config->readInto(visAllBlack, "GEOM.ST.VIS.BLACK");
    G4cout << "GEOM.ST.VIS.BLACK: " << visAllBlack << G4endl;

    gm.config->readInto(visSCFlag, "GEOM.ST.SC.VIS");
    G4cout << "GEOM.ST.SC.VIS: " << visSCFlag << G4endl;

    gm.config->readInto(CalDist, "GEOM.ST.CAL.DISTANCE");
    G4cout << "GEOM.ST.CAL.DISTANCE: " << CalDist << G4endl;
    gm.config->readInto(CalHeight, "GEOM.ST.CAL.HEIGHT");
    G4cout << "GEOM.ST.CAL.HEIGHT: " << CalHeight << G4endl;
    gm.config->readInto(SCDist, "GEOM.ST.SC.DISTANCE");
    G4cout << "GEOM.ST.SC.DISTANCE: " << SCDist << G4endl;
    gm.config->readInto(SCSide, "GEOM.ST.SC.SIDE");
    G4cout << "GEOM.ST.SC.SIDE: " << SCSide << G4endl;
    gm.config->readInto(SCHeight, "GEOM.ST.SC.HEIGHT");
    G4cout << "GEOM.ST.SC.HEIGHT: " << SCHeight << G4endl;

    SC_mat = materials->GetMaterial(27);

    // SC volume
    SC_sol = new G4Box("sSC", SCSide/2.0, SCSide/2.0, SCHeight/2.0);
    SC_log = new G4LogicalVolume(SC_sol, SC_mat, "lSC",0,0,0);

    // SC physical volume
    G4ThreeVector SC_pos(0.0, 0.0, - (CalDist + CalHeight + SCDist + (SCHeight/2.0)));
    SC_phys = new G4PVPlacement(0, SC_pos, SC_log, "pSC", World_log, false, 400);

    // Visualization
    SC_visAtt = new G4VisAttributes(G4Colour::Gray());
    black_visAtt = new G4VisAttributes(G4Colour::Black());
    if (visAllBlack) {
        SC_log->SetVisAttributes( black_visAtt );
    } else {
        if (visSCFlag) {
            SC_log->SetVisAttributes( SC_visAtt );
        } else {
            SC_log -> SetVisAttributes (G4VisAttributes::GetInvisible());
        }
    }

    return World_phys;
}
