/***************************************************************************
                          GeometryBaseConstruction.cc  -  description
                             -------------------
    begin                : 2010
    copyright            : (C) 2010 by Andrea Bulgarelli
    email                : bulgarelli@iasfbo.inaf.it
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software for non commercial purpose              *
 *   and for public research institutes; you can redistribute it and/or    *
 *   modify it under the terms of the GNU General Public License.          *
 *   For commercial purpose see appropriate license terms                  *
 *                                                                         *
 ***************************************************************************/



//Open questions:
// - Materialien / Isotopenverteilungen / Verunreinigungen allgemein
// - Material of HEDelectronics (right now Au)
// - existancte / width of collimator baffle (right now 200 mm)
// - Material of Separation Foil between detectors (right now vacuum)
// - Material of ceramics plane for LEDetector (right now vacuum)
// - Geometry around AC ? (right now Al-Box)

#include "GeometryBaseConstruction.hh"
#include "G4Element.hh"
#include "G4Material.hh"
#include "G4Box.hh"
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
#include "G4Cons.hh"
#include "CXYZSD.hh"
#include "G4UserLimits.hh"

#if defined(GEANT4_10_1) || defined(GEANT4_10_2) || defined(GEANT4_10_3)  || defined(GEANT4_10_4)
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"
#endif

//GDML
//#include "G4Writer/G4GDMLWriter.h"

#include <iostream>
using namespace std;

GeometryBaseConstruction::GeometryBaseConstruction() {

    World_phys = 0;
    World_log = 0;
    materials = new MaterialsDefinition;

}

GeometryBaseConstruction::~GeometryBaseConstruction() {
    delete fStepLimit;
}

G4VPhysicalVolume* GeometryBaseConstruction::Construct() {

    //G4GDMLWriter g4writer("G:/Daten/Doktorarbeit/GEANT4/TheCode/GDML/GDMLSchema/gdml.xsd", "Simbol_X.gdml");

    World_phys = gm.ConstructWorld();
    World_log = gm.World_log;
    World_phys = ConstructGeometry(World_phys);

    G4cout << "GEOM BASE OK " << World_phys << " " << World_log << G4endl;
    return World_phys;

}




G4VPhysicalVolume* GeometryBaseConstruction::ConstructGeometry(G4VPhysicalVolume* World_phys) {

    int N = 0;
    gm.config->readInto(N, "GEOM.BOXGEOMETRY.N");
    G4cout << "GEOM.BOXGEOMETRY.N " << N << G4endl;
    for(G4int i = 0; i<N; i++)
        MakeBoxVolume(i);


    return World_phys;
}

void GeometryBaseConstruction::MakeBoxVolume(G4int i) {

    G4String name, symbol;

    char c[100];
    sprintf(c, "%d", i);
    G4String sname;
    sname = "GEOMBASE_S_";
    sname += c;

    G4Box* STSolid;
    G4Cons* STSolid2;
    G4Sphere* STSolid3;
    G4LogicalVolume* STLogicalVolume;
    G4VPhysicalVolume* STPhysicalVolume;
    // Rotation Matrix
    G4RotationMatrix *rotateMatrix = new G4RotationMatrix();
    
    G4int type = 0;
    sprintf(c, "GEOMBASE.%i.type", i); //0 box, 1 cone
    gm.config->readInto(type, c);
    G4cout << c << " " << type << G4endl;
    G4double basex, basey, height, diameter1, diameter2, rad1, rad2;
    G4double xpos, ypos, zpos, xrot, yrot, zrot;
    G4int material = 0;
    if(type == 0) {
        sprintf(c, "GEOMBASE.%i.dim.basex", i);
        gm.config->readInto(basex, c);
        cout << c << " " << basex << endl;
        sprintf(c, "GEOMBASE.%i.dim.basey", i);
        gm.config->readInto(basey, c);
        cout << c << " " << basey << endl;
        sprintf(c, "GEOMBASE.%i.dim.height", i);
        gm.config->readInto(height, c);
        cout << c << " " << height << endl;
    }
    if(type == 1) {
        sprintf(c, "GEOMBASE.%i.dim.diameter1", i);
        gm.config->readInto(diameter1, c);
        cout << c << " " << diameter1 << endl;
        sprintf(c, "GEOMBASE.%i.dim.diameter2", i);
        gm.config->readInto(diameter2, c);
        cout << c << " " << diameter2 << endl;
        sprintf(c, "GEOMBASE.%i.dim.height", i);
        gm.config->readInto(height, c);
        cout << c << " " << height << endl;
    }
    if(type == 2) {
        sprintf(c, "GEOMBASE.%i.dim.radmin", i);
        gm.config->readInto(rad1, c);
        cout << c << " " << rad1 << endl;
        sprintf(c, "GEOMBASE.%i.dim.radmax", i);
        gm.config->readInto(rad2, c);
        cout << c << " " << rad2 << endl;
    }
    sprintf(c, "GEOMBASE.%i.xpos", i);
    gm.config->readInto(xpos, c);
    cout << c << " " << xpos << endl;
    sprintf(c, "GEOMBASE.%i.ypos", i);
    gm.config->readInto(ypos, c);
    cout << c << " " << ypos << endl;
    sprintf(c, "GEOMBASE.%i.zpos", i);
    gm.config->readInto(zpos, c);
    cout << c << " " << zpos << endl;
    sprintf(c, "GEOMBASE.%i.material", i);
    gm.config->readInto(material, c);
    cout << c << " " << material << endl;

    sprintf(c, "GEOMBASE.%i.xrot", i);
    gm.config->readInto(xrot, c);
    cout << c << " " << xrot << endl;
    sprintf(c, "GEOMBASE.%i.yrot", i);
    gm.config->readInto(yrot, c);
    cout << c << " " << yrot << endl;
    sprintf(c, "GEOMBASE.%i.zrot", i);
    gm.config->readInto(zrot, c);
    cout << c << " " << zrot << endl;
    
    rotateMatrix->rotateX(xrot*deg);
    rotateMatrix->rotateY(yrot*deg);
    rotateMatrix->rotateZ(zrot*deg);
    
    
    
    G4int rcolor = 255, gcolor = 255, bcolor = 255;

    sprintf(c, "GEOMBASE.%i.Rcolor", i);
    gm.config->readInto(rcolor, c);
    cout << c << " " << rcolor << endl;
    sprintf(c, "GEOMBASE.%i.Gcolor", i);
    gm.config->readInto(gcolor, c);
    cout << c << " " << gcolor << endl;
    sprintf(c, "GEOMBASE.%i.Bcolor", i);
    gm.config->readInto(bcolor, c);
    cout << c << " " << bcolor << endl;

    G4bool visible = false;
    sprintf(c, "GEOMBASE.%i.visible", i);
    gm.config->readInto(visible, c);
    cout << c << " " << visible << endl;

    //material selection
    G4Material* box1Material = materials->GetMaterial(material);


    if(type == 0) {
        STSolid = new G4Box( sname, basex/2.0, basey/2.0, height/2.0);
        STLogicalVolume = new G4LogicalVolume(STSolid, box1Material, sname, 0, 0, 0 );
        
        // setting the maximum step size
        G4double maxStep = 0.1*height;
        sprintf(c, "GEOMBASE.%i.MaxStep", i);
        gm.config->readInto(maxStep, c);
        cout << c << " " << maxStep << endl;
        
        fStepLimit = new G4UserLimits(maxStep);
        STLogicalVolume->SetUserLimits(fStepLimit);
    }
    if(type == 1) {
        STSolid2 = new G4Cons( sname, 0.0, diameter1/2.0, 0.0, diameter2/2.0, height/2.0, 0*deg, 360.*deg);
        STLogicalVolume = new G4LogicalVolume(STSolid2, box1Material, sname, 0, 0, 0 );
    }
    if(type == 2) {
        STSolid3 = new G4Sphere( sname, rad1, rad2, 0., twopi, 0., pi);
        STLogicalVolume = new G4LogicalVolume(STSolid3, box1Material, sname, 0, 0, 0 );
    }
    sname = "GEOMBASE_L_";
    sname += c;

    G4VisAttributes* VisST = new G4VisAttributes(G4Colour(rcolor/255., gcolor/255., bcolor/255.) );
    STLogicalVolume->SetVisAttributes( VisST );

    sname = "GEOMBASE_P_";
    sname += c;
    STPhysicalVolume = new G4PVPlacement(rotateMatrix,
                                         G4ThreeVector( xpos, ypos, zpos ),
                                         "G4_P",
                                         STLogicalVolume,
                                         World_phys, false, 0 );
    //--- Make Invisible
    if(!visible) STLogicalVolume -> SetVisAttributes( G4VisAttributes::Invisible );
    STPhysicalVolume->SetCopyNo(i+1000);
    gm.AddSensitiveDetector( STPhysicalVolume );
    gm.AddXYZDetector( STLogicalVolume );
    
    
}
