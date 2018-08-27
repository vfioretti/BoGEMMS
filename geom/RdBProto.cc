/***************************************************************************
                          RdBProto.cc  -  description
                             -------------------
    begin                : 2015
    copyright            : (C) 2015 by  Valentina Fioretti
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

#include "RdBProto.hh"
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

#if defined(GEANT4_10_1) || defined(GEANT4_10_2) || defined(GEANT4_10_3) || defined(GEANT4_10_4)
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"
#endif

//GDML
//#include "G4Writer/G4GDMLWriter.h"



RdBProto::RdBProto() {

    World_phys = 0;
    World_log = 0;
    materials = new MaterialsDefinition;
}

RdBProto::~RdBProto() {}

G4VPhysicalVolume* RdBProto::Construct() {

    //DefineMaterials();
    World_phys = gm.ConstructWorld();
    World_log = gm.World_log;
    World_phys = ConstructGeometry(World_phys);
    G4cout << "********* INAF/IASFBO RdBProto **********" << G4endl;
    G4cout << "Author: V. Fioretti" << G4endl;

    return World_phys;


}

G4VPhysicalVolume* RdBProto::ConstructGeometry(G4VPhysicalVolume* World_phys) {

    //Rotations
    G4RotationMatrix* no_rotation = new G4RotationMatrix(0,0,0);

	// *****************************************
	// 				SLAB
	// *****************************************

	//Slab side
    G4double Slab_side = 0.*mm;
    gm.config->readInto(Slab_side, "GEOM.RDB.SIDE");
    G4cout << "GEOM.RDB.SIDE: " << Slab_side << G4endl;

	// Slab material    
    G4int Slab_mat_index = 0;
    gm.config->readInto(Slab_mat_index, "GEOM.RDB.MATERIAL");
    G4cout << "GEOM.RDB.MATERIAL: " << Slab_mat_index << G4endl;

    G4double Slab_t = 0;
    gm.config->readInto(Slab_t, "GEOM.RDB.THICKNESS");
    G4cout << "GEOM.RDB.THICKNESS: " << Slab_t << G4endl;

    //Creation of detector geometry
    G4Box* Slab = new G4Box("slab",
                            Slab_side/2.,
                            Slab_side/2.,
                            Slab_t/2.);


	//Material assignment
	Slab_mat = materials->GetMaterial(Slab_mat_index);

	//Slab copy number
	G4int Slab_copy = 10;
	
    //Logical and Physical Volume
    logSlab  = new G4LogicalVolume(Slab,Slab_mat,"logSlab",0,0,0);
    physSlab       = new G4PVPlacement(0,G4ThreeVector(0.0, 0.0, 0.0),logSlab,"pSlab",World_log,false,Slab_copy);

    VisSlab = new G4VisAttributes(G4Colour::Yellow());
    logSlab->SetVisAttributes( VisSlab );
    
    gm.AddSensitiveDetector( physSlab );
    gm.AddXYZDetector( logSlab);


	// *****************************************
	// 				EMPTY SPHERE
	// *****************************************
    
    G4double FakeSphere_radius = 1000.0*mm;
    G4double FakeSphere_t = 0.00001*um;
    
    //Creation of geometry
    G4Sphere* FakeSphere = new G4Sphere("FakeSphere",
                                        FakeSphere_radius,
                                        FakeSphere_radius + FakeSphere_t,
                                        0.,
                                        twopi,
                                        0.,
                                        pi);
 
 	//Material assignment for fake sphere (vacuum)
	Sphere_mat = materials->GetMaterial(12);
    
    //Sphere copy number
	G4int Sphere_copy = 1000;
    //Logical and Physical Volume
    logFakeSphere  = new G4LogicalVolume(FakeSphere,Sphere_mat,"logFakeSphere",0,0,0);

    physFakeSphere       = new G4PVPlacement(0,G4ThreeVector(0.0, 0.0, 0.0),logFakeSphere,"pFakeSphere",World_log,false,Sphere_copy);

    VisFakeSphere = new G4VisAttributes(G4Colour::Gray());
    logFakeSphere->SetVisAttributes( VisFakeSphere );
    
    gm.AddSensitiveDetector( physFakeSphere );
    gm.AddXYZDetector( logFakeSphere);



    return World_phys;
}
