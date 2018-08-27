/***************************************************************************
                          DummyXMM.cc  -  description
                             -------------------
    begin                : 2014
    copyright            : (C) 2014 by Simone Lotti
    copyright            : (C) 2014 by  Valentina Fioretti
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

#include "DummyXMM.hh"
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
#include "G4NistManager.hh"
#include "G4PVReplica.hh"
#include "G4SubtractionSolid.hh"

#if defined(GEANT4_10_1) || defined(GEANT4_10_2) || defined(GEANT4_10_3) || defined(GEANT4_10_4)
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"
#endif

//GDML
//#include "G4Writer/G4GDMLWriter.h"



DummyXMM::DummyXMM() {

    World_phys = 0;
    World_log = 0;
    materials = new MaterialsDefinition;

}

DummyXMM::~DummyXMM() {}

G4VPhysicalVolume* DummyXMM::Construct() {

    //DefineMaterials();
    World_phys = gm.ConstructWorld();
    World_log = gm.World_log;
    World_phys = ConstructGeometry(World_phys);
    G4cout << "********* INAF/IASFBO XMMSim **********" << G4endl;
    G4cout << "Author: V. Fioretti" << G4endl;
    
    return World_phys;

}


G4VPhysicalVolume* DummyXMM::ConstructGeometry(G4VPhysicalVolume* World_phys) {

    
    //Rotations
    G4RotationMatrix* no_rotation = new G4RotationMatrix(0,0,0);
    
    // *****************************************
    // 				proton shield
    // *****************************************
    
    //inner diameter - detector side
    G4double inner_diam_low = 0.*mm;
    gm.config->readInto(inner_diam_low, "GEOM.SHIELD.INNER.LOWD");
    G4cout << "GEOM.SHIELD.INNER.LOWD: " << inner_diam_low << G4endl;
    //outer diameter - detector side
    G4double outer_diam_low = 0.*mm;
    gm.config->readInto(outer_diam_low, "GEOM.SHIELD.OUTER.LOWD");
    G4cout << "GEOM.SHIELD.OUTER.LOWD: " << outer_diam_low << G4endl;
    //inner diameter - sky side
    G4double inner_diam_up = 0.*mm;
    gm.config->readInto(inner_diam_up, "GEOM.SHIELD.INNER.UPD");
    G4cout << "GEOM.SHIELD.INNER.UPD: " << inner_diam_up << G4endl;
    //outer diameter - sky side
    G4double outer_diam_up = 0.*mm;
    gm.config->readInto(outer_diam_up, "GEOM.SHIELD.OUTER.UPD");
    G4cout << "GEOM.SHIELD.OUTER.UPD: " << outer_diam_up << G4endl;
    //height
    G4double shield_height = 0.*mm;
    gm.config->readInto(shield_height, "GEOM.SHIELD.HEIGHT");
    G4cout << "GEOM.SHIELD.HEIGHT: " << shield_height << G4endl;
    
    
    G4Cons* Shield = new G4Cons("shield",
                                 inner_diam_low/2.,
                                 outer_diam_low/2.,
                                 inner_diam_up/2.,
                                 outer_diam_up/2.,
                                 shield_height/2.,
                                 twopi,
                                 twopi);
    
    //Material assignment
    Shield_mat = materials->GetMaterial(1);
    
    //Shield copy number
    G4int Shield_copy = 10;
 
    // Shield position
    G4ThreeVector Shield_pos(0, 0.0, shield_height/2.);
    
    //Logical and Physical Volume
    logShield  = new G4LogicalVolume(Shield,Shield_mat,"logShield",0,0,0);
    physShield = new G4PVPlacement(0,Shield_pos,logShield,"pShield",World_log,false,Shield_copy);
    
    VisShield = new G4VisAttributes(G4Colour::Blue());
    logShield->SetVisAttributes( VisShield );
    
    gm.AddXYZDetector( logShield);
    
    // *****************************************
    // 				detector
    // *****************************************
    
    // size
    G4double detector_side = 0.*mm;
    gm.config->readInto(detector_side, "GEOM.DETECTOR.DIAMETER");
    G4cout << "GEOM.DETECTOR.DIAMETER: " << detector_side << G4endl;
    G4double detector_height = 0.*mm;
    gm.config->readInto(detector_height, "GEOM.DETECTOR.HEIGHT");
    G4cout << "GEOM.DETECTOR.HEIGHT: " << detector_height << G4endl;
    // distance from the shield base
    G4double detector_dist = 0.*mm;
    gm.config->readInto(detector_dist, "GEOM.DETECTOR.DIST");
    G4cout << "GEOM.DETECTOR.DIST: " << detector_dist << G4endl;
    
    
    //Creation of detector geometry
    G4Tubs* Detector = new G4Tubs("detector",
                                0.,
                                detector_side/2.,
                                detector_height/2.,
                                0.,
                                twopi);

    
    
    //Material assignment for fake CraftBox (vacuum)
    Detector_mat = materials->GetMaterial(9);
    
    //CraftBox copy number
    G4int Detector_copy = 20;
    
    // Detector position
    G4ThreeVector Detector_pos(0, 0.0, -detector_height/2. - detector_dist);
    
    //Logical and Physical Volume
    logDetector  = new G4LogicalVolume(Detector,Detector_mat,"logDetector",0,0,0);
    physDetector = new G4PVPlacement(0,Detector_pos,logDetector,"pDetector",World_log,false,Detector_copy);
    
    VisDetector = new G4VisAttributes(G4Colour::Yellow());
    logDetector->SetVisAttributes( VisDetector );
    
    gm.AddXYZDetector( logDetector);

    // *****************************************
    // 				Al filter
    // *****************************************
 
    // filter activation
    G4bool filter_activate = 0;
    gm.config->readInto(filter_activate, "GEOM.FILTER.ACTIVATE");
    G4cout << "GEOM.FILTER.ACTIVATE: " << filter_activate << G4endl;

    if (filter_activate){
        // size
        G4double filter_side = detector_side; // same diameter of the detector diameter
        G4double filter_height = 0.*mm;
        gm.config->readInto(filter_height, "GEOM.FILTER.HEIGHT");
        G4cout << "GEOM.FILTER.HEIGHT: " << filter_height << G4endl;
        
        //Creation of filter geometry
        G4Tubs* Filter = new G4Tubs("filter",
                                      0.,
                                      filter_side/2.,
                                      filter_height/2.,
                                      0.,
                                      twopi);
        
        
        
        //Material assignment for filter
        Filter_mat = materials->GetMaterial(1);
        
        //filter copy number
        G4int Filter_copy = 30;
        
        //  position
        G4ThreeVector Filter_pos(0, 0.0, -filter_height/2.);
        
        //Logical and Physical Volume
        logFilter  = new G4LogicalVolume(Filter,Filter_mat,"logFilter",0,0,0);
        physFilter = new G4PVPlacement(0,Filter_pos,logFilter,"pFilter",World_log,false,Filter_copy);
        
        VisFilter = new G4VisAttributes(G4Colour::Green());
        logFilter->SetVisAttributes( VisFilter );
        
        gm.AddXYZDetector(logFilter);
    }
    
    // *****************************************
    // 				spacecraft box
    // *****************************************
    
    // size
    G4double craftbox_side = detector_side*2.;
    G4double shield_within_box = 100.8*mm;
    G4double craftbox_height = shield_within_box*2.;
    G4double craftbox_t = 1.*mm;
    
    //Creation of detector geometry
    G4Box* CraftBox_ext = new G4Box("craftbox_ext",
                            craftbox_side/2.,
                            craftbox_side/2.,
                            craftbox_height/2.);

    //Creation of detector geometry
    G4Box* CraftBox_int = new G4Box("craftbox_int",
                                    (craftbox_side - 2.*craftbox_t)/2.,
                                    (craftbox_side - 2.*craftbox_t)/2.,
                                    (craftbox_height - 2.*craftbox_t)/2.);

    G4SubtractionSolid* CraftBox_noHole = new G4SubtractionSolid("CraftBox_noHole", CraftBox_ext, CraftBox_int);
    
    //G4ThreeVector zTrans(0, 0, 50);
    G4SubtractionSolid* CraftBox = new G4SubtractionSolid("CraftBox", CraftBox_noHole, Shield, no_rotation, Shield_pos);

    //Material assignment for fake CraftBox (vacuum)
    CraftBox_mat = materials->GetMaterial(12);
    
    //CraftBox copy number
    G4int CraftBox_copy = 1000;
    
    //  position
    G4ThreeVector CraftBox_pos(0, 0.0, 0.0);

    //Logical and Physical Volume
    logCraftBox  = new G4LogicalVolume(CraftBox,CraftBox_mat,"logCraftBox",0,0,0);
    physCraftBox = new G4PVPlacement(0,CraftBox_pos,logCraftBox,"pCraftBox",World_log,false,CraftBox_copy);
    
    VisCraftBox = new G4VisAttributes(G4Colour::Gray());
    logCraftBox->SetVisAttributes( VisCraftBox);
    
    gm.AddXYZDetector( logCraftBox);

    // *****************************************
    // 				shield hat
    // *****************************************
    
    // size
    G4double hat_diameter = outer_diam_up;
    G4double hat_height = 1.*mm;
    
    //Creation of Hat geometry
    G4Tubs* Hat = new G4Tubs("hat",
                            0.*mm,
                            hat_diameter/2.,
                            hat_height/2.,
                            0.,
                            twopi);
    
    //Material assignment for fake Hat (vacuum)
    Hat_mat = materials->GetMaterial(12);
    
    //Hat copy number
    G4int Hat_copy = 2000;

    // hat position
    G4ThreeVector Hat_pos(0.0, 0.0, shield_height + hat_height/2.);
   
    //Logical and Physical Volume
    logHat  = new G4LogicalVolume(Hat,Hat_mat,"logHat",0,0,0);
    physHat = new G4PVPlacement(0,Hat_pos,logHat,"pHat",World_log,false,Hat_copy);
    
    VisHat = new G4VisAttributes(G4Colour::Gray());
    //logHat->SetVisAttributes( VisHat);
    logHat->SetVisAttributes(G4VisAttributes::GetInvisible());
    
    gm.AddXYZDetector( logHat);
 
    // *****************************************
    // 	        Dummy sphere
    // *****************************************
    
    // dummy sphere activation
    G4bool dummySphere_activate = 0;
    gm.config->readInto(dummySphere_activate, "GEOM.SPHERE.ACTIVATE");
    G4cout << "GEOM.SPHERE.ACTIVATE: " << dummySphere_activate << G4endl;

    // dummy sphere dimension
    G4double dummySphere_Rin = 0.*mm;
    gm.config->readInto(dummySphere_Rin, "GEOM.SPHERE.R.INNER");
    G4cout << "GEOM.SPHERE.R.INNER: " << dummySphere_Rin << G4endl;
    G4double dummySphere_t = 0.*mm;
    gm.config->readInto(dummySphere_t, "GEOM.SPHERE.THICKNESS");
    G4cout << "GEOM.SPHERE.THICKNESS: " << dummySphere_t << G4endl;

    if (dummySphere_activate){
    
      G4Sphere* dummySphere = new G4Sphere("dummySphere",
                                         dummySphere_Rin,
                                         dummySphere_Rin + dummySphere_t,
                                         0.*rad,
                                         2.*pi*rad,
                                         0.*rad,
                                         pi*rad);
    
    
      //Material assignment for fake sphere (vacuum)
      dummySphere_mat = materials->GetMaterial(12);
    
      //fake sphere copy number
      G4int dummySphere_copy = 3000;
    
      // fake sphere position
      G4ThreeVector dummySphere_pos(0.0,0.0,0.0);
    
      //Logical and Physical Volume
      logDummySphere  = new G4LogicalVolume(dummySphere,dummySphere_mat,"logDummySphere",0,0,0);
      physDummySphere = new G4PVPlacement(0,dummySphere_pos,logDummySphere,"pDummySphere",World_log,false,dummySphere_copy);
    
      VisDummySphere = new G4VisAttributes(G4Colour::Green());
      //logDummySphere->SetVisAttributes(G4VisAttributes::GetInvisible());
      logDummySphere->SetVisAttributes(VisDummySphere);
    
      gm.AddXYZDetector( logDummySphere);

    } 
    return World_phys;
}
