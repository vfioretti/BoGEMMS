/***************************************************************************
                          GeometryST.cc  -  description
                             -------------------
	                  Building the Gamma-Light tracker
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

#include "GeometryST.hh"
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
#include "G4PVReplica.hh"
#include "CXYZSD.hh"
#include "G4SDManager.hh"
#include "GeometryST_AC.hh"

#if defined(GEANT4_10_1) || defined(GEANT4_10_2) || defined(GEANT4_10_3) || defined(GEANT4_10_4)
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"
#endif

GeometryST::GeometryST() {

    World_phys = 0;
    World_log = 0;
    //VacuumMaterial = 0;
    materials = new MaterialsDefinition;

}

GeometryST::~GeometryST() {}

G4VPhysicalVolume* GeometryST::Construct() {

    //DefineMaterials();
    World_phys = gm.ConstructWorld();
    World_log = gm.World_log;
    World_phys = ConstructGeometry(World_phys);
    G4cout << "Small Telescope Simulation" << G4endl;

    return World_phys;


}

G4VPhysicalVolume* GeometryST::ConstructGeometry(G4VPhysicalVolume* World_phys) {

    G4double zero = 0.;
    G4double pis2 = asin(1.0);
    G4double pi = 2.0 * pis2;
    G4double twopi = 2.0 * pi;

    G4RotationMatrix* zRot90 = new G4RotationMatrix;
    zRot90->rotateZ(pis2);
    G4RotationMatrix* zRot180 = new G4RotationMatrix;
    zRot180->rotateZ(pi);
    G4RotationMatrix* zRot270 = new G4RotationMatrix;
    zRot270->rotateZ(3.0*pis2);

    gm.config->readInto(visAllBlack, "GEOM.ST.VIS.BLACK");
    G4cout << "GEOM.ST.VIS.BLACK: " << visAllBlack << G4endl;


    // ##############################################################
    // ##############            TRACKER                #############
    // ##############################################################

    gm.config->readInto(visType, "GEOM.ST.TRACKER.VIS");
    G4cout << "GEOM.ST.TRACKER.VIS: " << visType << G4endl;

    // AGILE tracker
    isTrayAgile = false;
    gm.config->readInto(isTrayAgile, "GEOM.ST.TRAY.AGILE");
    G4cout << "GEOM.ST.TRAY.AGILE: " << isTrayAgile << G4endl;
    FirstTrayConv = 0;
    gm.config->readInto(FirstTrayConv, "GEOM.ST.TRAY.AGILE.CONV");
    G4cout << "GEOM.ST.TRAY.AGILE.CONV: " << FirstTrayConv << G4endl;

    // ASTROMEV tracker
    isTrayASTROMEV = false; // Double-Sided Silicon
    gm.config->readInto(isTrayASTROMEV, "GEOM.ST.TRAY.ASTROMEV");
    G4cout << "GEOM.ST.TRAY.ASTROMEV: " << isTrayASTROMEV << G4endl;
    versionASTROMEV = 3; // version 3 is Double-Sided with pixels
    gm.config->readInto(versionASTROMEV, "GEOM.ST.TRAY.ASTROMEV.VERSION");
    G4cout << "GEOM.ST.TRAY.ASTROMEV.VERSION: " << versionASTROMEV << G4endl;

    isStrip = false;
    gm.config->readInto(isStrip, "GEOM.ST.TRAY.STRIP");
    G4cout << "GEOM.ST.TRAY.STRIP: " << isStrip << G4endl;
    isStripReplicated = false;
    gm.config->readInto(isStripReplicated, "GEOM.ST.TRAY.STRIP.REPLICATED");
    G4cout << "GEOM.ST.TRAY.STRIP.REPLICATED: " << isStripReplicated << G4endl;
    Strip_N = 0;
    gm.config->readInto(Strip_N, "GEOM.ST.TRAY.STRIP.N");
    G4cout << "GEOM.ST.TRAY.STRIP.N: " << Strip_N << G4endl;
    Strip_pitch = 0.;
    gm.config->readInto(Strip_pitch, "GEOM.ST.TRAY.STRIP.PITCH");
    G4cout << "GEOM.ST.TRAY.STRIP.PITCH: " << Strip_pitch << G4endl;

    isPixel = false;
    gm.config->readInto(isPixel, "GEOM.ST.TRAY.PIXEL");
    G4cout << "GEOM.ST.TRAY.PIXEL: " << isPixel << G4endl;
    isPixelReplicated = false;
    gm.config->readInto(isPixelReplicated, "GEOM.ST.TRAY.PIXEL.REPLICATED");
    G4cout << "GEOM.ST.TRAY.PIXEL.REPLICATED: " << isPixelReplicated << G4endl;
    gm.config->readInto(Pixel_side, "GEOM.ST.TRAY.PIXEL.SIDE");
    G4cout << "GEOM.ST.TRAY.PIXEL.SIDE: " << Pixel_side << G4endl;

    gm.config->readInto(Tray_N, "GEOM.ST.TRAY.NUMBER");
    G4cout << "GEOM.ST.TRAY.NUMBER: " << Tray_N << G4endl;
    gm.config->readInto(Tray_side, "GEOM.ST.TRAY.SIDE");
    G4cout << "GEOM.ST.TRAY.SIDE: " << Tray_side << G4endl;
    gm.config->readInto(Tray_dist, "GEOM.ST.TRAY.DIST");
    G4cout << "GEOM.ST.TRAY.DIST: " << Tray_dist << G4endl;

    if (isPixel){
    	Pixel_NX = Tray_side/Pixel_side;
    	Pixel_NY = Tray_side/Pixel_side;
    	Pixel_N = Pixel_NX*Pixel_NY;
    }

    gm.config->readInto(isConvertInTrk, "GEOM.ST.TRAY.CONVERTER");
    G4cout << "GEOM.ST.TRAY.CONVERTER: " << isConvertInTrk << G4endl;
    gm.config->readInto(isGlueInTrk, "GEOM.ST.TRAY.GLUE");
    G4cout << "GEOM.ST.TRAY.GLUE: " << isGlueInTrk << G4endl;
    gm.config->readInto(isKaptonInTrk, "GEOM.ST.TRAY.KAPTON");
    G4cout << "GEOM.ST.TRAY.KAPTON: " << isKaptonInTrk << G4endl;
    gm.config->readInto(isGlueToCu, "GEOM.ST.TRAY.GLUETOCU");
    G4cout << "GEOM.ST.TRAY.GLUETOCU: " << isGlueToCu << G4endl;
    AddPassive = true;
    gm.config->readInto(AddPassive, "GEOM.ST.TRACKER.PASSIVE");
    G4cout << "GEOM.ST.TRACKER.PASSIVE: " << AddPassive << G4endl;
    gm.config->readInto(HoneyType, "GEOM.ST.TRAY.ALHONEY.TYPE");
    G4cout << "GEOM.ST.TRAY.ALHONEY.TYPE: " << HoneyType << G4endl;
    AddSensitivePassive = false;
    gm.config->readInto(AddSensitivePassive, "GEOM.ST.TRACKER.PASSIVE.SENSITIVE");
    G4cout << "GEOM.ST.TRACKER.PASSIVE.SENSITIVE: " << AddSensitivePassive << G4endl;

    gm.config->readInto(SiDetector_t, "GEOM.ST.TRAY.SILICON.THICKNESS");
    G4cout << "GEOM.ST.TRAY.SILICON.THICKNESS: " << SiDetector_t << G4endl;
    gm.config->readInto(GlueTray_t, "GEOM.ST.TRAY.GLUE.THICKNESS");
    G4cout << "GEOM.ST.TRAY.GLUE.THICKNESS: " << GlueTray_t << G4endl;
    gm.config->readInto(KaptonTray_t, "GEOM.ST.TRAY.KAPTON.THICKNESS");
    G4cout << "GEOM.ST.TRAY.KAPTON.THICKNESS: " << KaptonTray_t << G4endl;
    gm.config->readInto(CFiberTray_t, "GEOM.ST.TRAY.CFIBER.THICKNESS");
    G4cout << "GEOM.ST.TRAY.CFIBER.THICKNESS: " << CFiberTray_t << G4endl;
    gm.config->readInto(AlHoneyTray_t, "GEOM.ST.TRAY.ALHONEY.THICKNESS");
    G4cout << "GEOM.ST.TRAY.ALHONEY.THICKNESS: " << AlHoneyTray_t << G4endl;
    gm.config->readInto(ConverterTray_t, "GEOM.ST.TRAY.CONVERTER.THICKNESS");
    G4cout << "GEOM.ST.TRAY.CONVERTER.THICKNESS: " << ConverterTray_t << G4endl;


    // MATERIALS
    if ((isTrayAgile) || ((isTrayASTROMEV) && (versionASTROMEV == 1))){
	    if (isStrip) {
    		SiDetector_mat = materials->GetMaterial(12);
		SiStrip_mat = materials->GetMaterial(9);
    	    }
    	    else {
    		SiDetector_mat = materials->GetMaterial(9);
 	    }
    }
    if ((isTrayASTROMEV) && (versionASTROMEV == 2)){
    	if (isPixel) {
    		SiDetector_mat = materials->GetMaterial(12);
 		SiPixel_mat = materials->GetMaterial(9);           
    	}
	else {
    		SiDetector_mat = materials->GetMaterial(9);
        }
    }    
    if ((isTrayASTROMEV) && (versionASTROMEV == 3)){
    	if (isPixel) {
    		SiDetector_mat = materials->GetMaterial(12);
 		SiPixel_mat = materials->GetMaterial(9);           
    	}
	else {
    		SiDetector_mat = materials->GetMaterial(9);
        }
    }    

    if (AddPassive) {
        if (isGlueInTrk) {
            if (isGlueToCu==0) GlueTray_mat = materials->GetMaterial(25);
            if (isGlueToCu==1) GlueTray_mat = materials->GetMaterial(5);
            G4cout << "Glue density [g/cm3]: " << GlueTray_mat->GetDensity()/(g/cm3) << G4endl;
        }
        if (isKaptonInTrk) {
		KaptonTray_mat = materials->GetMaterial(23);
	}
        CFiberTray_mat = materials->GetMaterial(22);
        if (HoneyType==0) AlHoneyTray_mat = materials->GetMaterial(24);
        if (HoneyType==1) AlHoneyTray_mat = materials->GetMaterial(28);
        G4double AlHoneyTray_rho = AlHoneyTray_mat->GetDensity()/(g/cm3);
        G4cout << "Aluminum honeycomb density [g/cm3]: " << AlHoneyTray_rho << G4endl;
        if (isConvertInTrk) ConverterTray_mat = materials->GetMaterial(7);
    } else {
        if (isGlueInTrk) GlueTray_mat = materials->GetMaterial(12);
        if (isKaptonInTrk) KaptonTray_mat = materials->GetMaterial(12);
        CFiberTray_mat = materials->GetMaterial(12);
        AlHoneyTray_mat = materials->GetMaterial(12);
        if (isConvertInTrk) ConverterTray_mat = materials->GetMaterial(12);
    }

    // THICKNESS

    if (!isConvertInTrk) ConverterTray_t = 0.0;
    if (!isGlueInTrk) GlueTray_t = 0.0;
    if (!isKaptonInTrk) KaptonTray_t = 0.0;
    if (versionASTROMEV==3) CFiberTray_t = 0.0;

    // Modules thickness
    if (isTrayAgile) {
	    CentralModule_t = (2.0*CFiberTray_t) + AlHoneyTray_t;
	    UpperModule_t = SiDetector_t + GlueTray_t + KaptonTray_t;
	    LowerModule_t = ConverterTray_t + GlueTray_t + KaptonTray_t + SiDetector_t;
	    TotalModule_t = LowerModule_t + CentralModule_t + UpperModule_t;
    }
    if (isTrayASTROMEV) {     
    	if (versionASTROMEV==1) {
	    CentralModule_t = (2.0*CFiberTray_t) + AlHoneyTray_t;
	    UpperModule_t = (2.*SiDetector_t) + GlueTray_t + KaptonTray_t;
	    LowerModule_t = ConverterTray_t + GlueTray_t + KaptonTray_t + (2.*SiDetector_t);
	    TotalModule_t = LowerModule_t + CentralModule_t + UpperModule_t;
    	}
    	if (versionASTROMEV==2) {
	    CentralModule_t = (2.0*CFiberTray_t) + AlHoneyTray_t;
	    UpperModule_t = SiDetector_t + GlueTray_t + KaptonTray_t;
	    LowerModule_t = ConverterTray_t + GlueTray_t + KaptonTray_t + SiDetector_t;
	    TotalModule_t = LowerModule_t + CentralModule_t + UpperModule_t;
    	}
    	if (versionASTROMEV==3) {
	    CentralModule_t = (2.0*CFiberTray_t) + AlHoneyTray_t;
	    UpperModule_t = SiDetector_t + GlueTray_t + KaptonTray_t;
	    LowerModule_t = ConverterTray_t + GlueTray_t + KaptonTray_t;
	    TotalModule_t = LowerModule_t + CentralModule_t + UpperModule_t;
    	}
    }



    // VOLUMES
    //Creation of Silicon Tracker geometry
    if (isTrayAgile){
	SiXDetector_sol = new G4Box("sSiXDetector", Tray_side/2., Tray_side/2., SiDetector_t/2.);
    	SiXDetector_log = new G4LogicalVolume(SiXDetector_sol, SiDetector_mat,"lSiXDetector",0,0,0);
    	SiYDetector_sol = new G4Box("sSiYDetector", Tray_side/2., Tray_side/2., SiDetector_t/2.);
    	SiYDetector_log = new G4LogicalVolume(SiYDetector_sol, SiDetector_mat,"lSiYDetector",0,0,0);
    }
    if (isTrayASTROMEV){
    	if (versionASTROMEV==1) {
		SiXBottomDetector_sol = new G4Box("sSiXBottomDetector", Tray_side/2., Tray_side/2., SiDetector_t/2.);
    		SiXBottomDetector_log = new G4LogicalVolume(SiXBottomDetector_sol, SiDetector_mat,"lSiXBottomDetector",0,0,0);
    		SiYBottomDetector_sol = new G4Box("sSiYBottomDetector", Tray_side/2., Tray_side/2., SiDetector_t/2.);
    		SiYBottomDetector_log = new G4LogicalVolume(SiYBottomDetector_sol, SiDetector_mat,"lSiYBottomDetector",0,0,0);
		SiXTopDetector_sol = new G4Box("sSiXTopDetector", Tray_side/2., Tray_side/2., SiDetector_t/2.);
    		SiXTopDetector_log = new G4LogicalVolume(SiXTopDetector_sol, SiDetector_mat,"lSiXTopDetector",0,0,0);
    		SiYTopDetector_sol = new G4Box("sSiYTopDetector", Tray_side/2., Tray_side/2., SiDetector_t/2.);
    		SiYTopDetector_log = new G4LogicalVolume(SiYTopDetector_sol, SiDetector_mat,"lSiYTopDetector",0,0,0);
	}
    	if (versionASTROMEV==2) {
		SiBottomDetector_sol = new G4Box("sSiBottomDetector", Tray_side/2., Tray_side/2., SiDetector_t/2.);
   		SiBottomDetector_log = new G4LogicalVolume(SiBottomDetector_sol, SiDetector_mat,"lSiBottomDetector",0,0,0);
		SiTopDetector_sol = new G4Box("sSiTopDetector", Tray_side/2., Tray_side/2., SiDetector_t/2.);
   		SiTopDetector_log = new G4LogicalVolume(SiTopDetector_sol, SiDetector_mat,"lSiTopDetector",0,0,0);
	}
    	if (versionASTROMEV==3) {
		SiTopDetector_sol = new G4Box("sSiTopDetector", Tray_side/2., Tray_side/2., SiDetector_t/2.);
   		SiTopDetector_log = new G4LogicalVolume(SiTopDetector_sol, SiDetector_mat,"lSiTopDetector",0,0,0);
	}
    }

    
    if (isTrayAgile) {
    	//Creation of Silicon strip X
    	StripX_sol = new G4Box("sStripX", Strip_pitch/2., Tray_side/2., SiDetector_t/2.);
    	StripX_log = new G4LogicalVolume(StripX_sol, SiStrip_mat,"lStripX",0,0,0);
    
    	//Creation of Silicon strip Y
    	StripY_sol = new G4Box("sStripY", Tray_side/2., Strip_pitch/2., SiDetector_t/2.);
    	StripY_log = new G4LogicalVolume(StripY_sol, SiStrip_mat,"lStripY",0,0,0);
    }

    if (isTrayASTROMEV) {
    	if (versionASTROMEV==1) {
    		//Creation of Silicon strip X
    		StripXTop_sol = new G4Box("sStripXTop", Strip_pitch/2., Tray_side/2., SiDetector_t/2.);
    		StripXTop_log = new G4LogicalVolume(StripXTop_sol, SiStrip_mat,"lStripXTop",0,0,0);

    		//Creation of Silicon strip Y
    		StripYTop_sol = new G4Box("sStripYTop", Tray_side/2., Strip_pitch/2., SiDetector_t/2.);
    		StripYTop_log = new G4LogicalVolume(StripYTop_sol, SiStrip_mat,"lStripYTop",0,0,0);

    		//Creation of Silicon strip X
    		StripXBottom_sol = new G4Box("sStripXBottom", Strip_pitch/2., Tray_side/2., SiDetector_t/2.);
    		StripXBottom_log = new G4LogicalVolume(StripXBottom_sol, SiStrip_mat,"lStripXBottom",0,0,0);

    		//Creation of Silicon strip Y
    		StripYBottom_sol = new G4Box("sStripYBottom", Tray_side/2., Strip_pitch/2., SiDetector_t/2.);
    		StripYBottom_log = new G4LogicalVolume(StripYBottom_sol, SiStrip_mat,"lStripYBottom",0,0,0);
	}
    	if (versionASTROMEV==2) {
    		//Creation of Silicon Top pixels
    		PixelTop_sol = new G4Box("sPixelTop", Pixel_side/2., Pixel_side/2., SiDetector_t/2.);
    		PixelTop_log = new G4LogicalVolume(PixelTop_sol, SiPixel_mat,"lPixelTop",0,0,0);

    		//Creation of Silicon Bottom pixels
    		PixelBottom_sol = new G4Box("sPixelBottom", Pixel_side/2., Pixel_side/2., SiDetector_t/2.);
    		PixelBottom_log = new G4LogicalVolume(PixelBottom_sol, SiPixel_mat,"lPixelBottom",0,0,0);

    		//Creation of Silicon Top pixels
    		PixelTopLine_sol = new G4Box("sPixelTopLine", Pixel_side/2., Tray_side/2., SiDetector_t/2.);
    		PixelTopLine_log = new G4LogicalVolume(PixelTopLine_sol, SiDetector_mat,"lPixelTopLine",0,0,0);

    		//Creation of Silicon Bottom pixels
    		PixelBottomLine_sol = new G4Box("sPixelBottomLine", Pixel_side/2., Tray_side/2., SiDetector_t/2.);
    		PixelBottomLine_log = new G4LogicalVolume(PixelBottomLine_sol, SiDetector_mat,"lPixelBottomLine",0,0,0);

	}
    	if (versionASTROMEV==3) {
    		//Creation of Silicon Top pixels
    		PixelTop_sol = new G4Box("sPixelTop", Pixel_side/2., Pixel_side/2., SiDetector_t/2.);
    		PixelTop_log = new G4LogicalVolume(PixelTop_sol, SiPixel_mat,"lPixelTop",0,0,0);

    		//Creation of Silicon Top pixels
    		PixelTopLine_sol = new G4Box("sPixelTopLine", Pixel_side/2., Tray_side/2., SiDetector_t/2.);
    		PixelTopLine_log = new G4LogicalVolume(PixelTopLine_sol, SiDetector_mat,"lPixelTopLine",0,0,0);

	}
    }
    
    //Creation of Glue geometry
    if (isGlueInTrk) GlueTray_sol = new G4Box("sGlueTray", Tray_side/2., Tray_side/2., GlueTray_t/2.);
    if (isGlueInTrk) GlueTray_log = new G4LogicalVolume(GlueTray_sol, GlueTray_mat,"lGlueTray",0,0,0);

    //Creation of Kapton geometry
    if (isKaptonInTrk) KaptonTray_sol = new G4Box("sKaptonTray", Tray_side/2., Tray_side/2., KaptonTray_t/2.);
    if (isKaptonInTrk) KaptonTray_log = new G4LogicalVolume(KaptonTray_sol, KaptonTray_mat,"lKaptonTray",0,0,0);

    //Creation of CFiber geometry
    if (versionASTROMEV!=3) CFiberTray_sol = new G4Box("sCFiberTray", Tray_side/2., Tray_side/2., CFiberTray_t/2.);
    if (versionASTROMEV!=3) CFiberTray_log = new G4LogicalVolume(CFiberTray_sol, CFiberTray_mat,"lCFiberTray",0,0,0);

    //Creation of AlHoney geometry
    AlHoneyTray_sol = new G4Box("sAlHoneyTray", Tray_side/2., Tray_side/2., AlHoneyTray_t/2.);
    AlHoneyTray_log = new G4LogicalVolume(AlHoneyTray_sol, AlHoneyTray_mat,"lAlHoneyTray",0,0,0);
    G4double AlHoneyMass;
    AlHoneyMass = AlHoneyTray_log->GetMass()/g;
    G4cout << "Aluminum honeycomb mass [g]: " << AlHoneyMass << G4endl;

    //Creation of ConverterTray geometry
    if (isConvertInTrk) ConverterTray_sol = new G4Box("sConverterTray", Tray_side/2., Tray_side/2., ConverterTray_t/2.);
    if (isConvertInTrk) ConverterTray_log = new G4LogicalVolume(ConverterTray_sol, ConverterTray_mat,"lConverterTray",0,0,0);


    // Visualization

    black_visAtt = new G4VisAttributes(G4Colour::Black());
    G4VisAttributes*red_visAtt = new G4VisAttributes(G4Colour::Red());
    G4VisAttributes*blue_visAtt = new G4VisAttributes(G4Colour::Blue());
    if (visAllBlack) {
	
	if (isTrayAgile){
        	SiXDetector_log->SetVisAttributes( blue_visAtt );
        	SiYDetector_log->SetVisAttributes( blue_visAtt );
        	StripX_log->SetVisAttributes(G4VisAttributes::GetInvisible());
        	StripY_log->SetVisAttributes(G4VisAttributes::GetInvisible());
	}
	if (isTrayASTROMEV){
    		if (versionASTROMEV==1) {
        		SiXBottomDetector_log->SetVisAttributes( blue_visAtt );
        		SiYBottomDetector_log->SetVisAttributes( blue_visAtt );
        		SiXTopDetector_log->SetVisAttributes( blue_visAtt );
        		SiYTopDetector_log->SetVisAttributes( blue_visAtt );
        		StripXBottom_log->SetVisAttributes(G4VisAttributes::GetInvisible());
        		StripYBottom_log->SetVisAttributes(G4VisAttributes::GetInvisible());
        		StripXTop_log->SetVisAttributes(G4VisAttributes::GetInvisible());
        		StripYTop_log->SetVisAttributes(G4VisAttributes::GetInvisible());
		}
    		if (versionASTROMEV==2) {
        		SiBottomDetector_log->SetVisAttributes( blue_visAtt );
        		SiTopDetector_log->SetVisAttributes( blue_visAtt );
        		PixelBottom_log->SetVisAttributes(G4VisAttributes::GetInvisible());
        		PixelTop_log->SetVisAttributes(G4VisAttributes::GetInvisible());
		}
    		if (versionASTROMEV==3) {
        		SiTopDetector_log->SetVisAttributes( blue_visAtt );
        		PixelTop_log->SetVisAttributes(G4VisAttributes::GetInvisible());
		}
	}
        if (isGlueInTrk) GlueTray_log->SetVisAttributes(blue_visAtt );
        if (isKaptonInTrk) KaptonTray_log->SetVisAttributes( blue_visAtt);
        if (versionASTROMEV!=3) CFiberTray_log->SetVisAttributes( blue_visAtt);
        AlHoneyTray_log->SetVisAttributes( blue_visAtt);
        if (isConvertInTrk) ConverterTray_log->SetVisAttributes(blue_visAtt );
    } else {
        if (visType==0) {
            SiDetector_visAtt = new G4VisAttributes(G4Colour::Grey());
	if (isTrayAgile){	    
            	SiXDetector_log->SetVisAttributes( SiDetector_visAtt );
            	SiYDetector_log->SetVisAttributes( SiDetector_visAtt );
            	StripX_log->SetVisAttributes(G4VisAttributes::GetInvisible());
            	StripY_log->SetVisAttributes(G4VisAttributes::GetInvisible());
	}
	if (isTrayASTROMEV){	    
    		if (versionASTROMEV==1) {
            	       SiXBottomDetector_log->SetVisAttributes( SiDetector_visAtt );
            	       SiYBottomDetector_log->SetVisAttributes( SiDetector_visAtt );
            	       StripXBottom_log->SetVisAttributes(G4VisAttributes::GetInvisible());
            	       StripYBottom_log->SetVisAttributes(G4VisAttributes::GetInvisible());
            	       SiXTopDetector_log->SetVisAttributes( SiDetector_visAtt );
            	       SiYTopDetector_log->SetVisAttributes( SiDetector_visAtt );
            	       StripXTop_log->SetVisAttributes(G4VisAttributes::GetInvisible());
            	       StripYTop_log->SetVisAttributes(G4VisAttributes::GetInvisible());
		}
    		if (versionASTROMEV==2) {
            	       SiBottomDetector_log->SetVisAttributes( SiDetector_visAtt );
            	       PixelBottom_log->SetVisAttributes(G4VisAttributes::GetInvisible());
            	       SiTopDetector_log->SetVisAttributes( SiDetector_visAtt );
            	       PixelTop_log->SetVisAttributes(G4VisAttributes::GetInvisible());
		}
    		if (versionASTROMEV==3) {
            	       SiTopDetector_log->SetVisAttributes( SiDetector_visAtt );
            	       PixelTop_log->SetVisAttributes(G4VisAttributes::GetInvisible());
		}
	}
            if (isGlueInTrk) GlueTray_visAtt = new G4VisAttributes(G4Colour::White());
            if (isGlueInTrk) GlueTray_log->SetVisAttributes(GlueTray_visAtt );
            if (isKaptonInTrk) KaptonTray_visAtt = new G4VisAttributes(G4Colour::Black());
            if (isKaptonInTrk) KaptonTray_log->SetVisAttributes(KaptonTray_visAtt );
            if (versionASTROMEV!=3) CFiberTray_visAtt = new G4VisAttributes(G4Colour::Red());
            if (versionASTROMEV!=3) CFiberTray_log->SetVisAttributes(CFiberTray_visAtt );
            AlHoneyTray_visAtt = new G4VisAttributes(G4Colour::Yellow());
            AlHoneyTray_log->SetVisAttributes(AlHoneyTray_visAtt );
            if (isConvertInTrk) ConverterTray_visAtt = new G4VisAttributes(G4Colour::Green());
            if (isConvertInTrk) ConverterTray_log->SetVisAttributes(ConverterTray_visAtt );
        }
        if (visType==1) {
            SiDetector_visAtt = new G4VisAttributes(G4Colour::Grey());
	    if (isTrayAgile){	    
            	SiXDetector_log->SetVisAttributes( SiDetector_visAtt );
            	SiYDetector_log->SetVisAttributes( SiDetector_visAtt );
            	StripX_log->SetVisAttributes(G4VisAttributes::GetInvisible());
            	StripY_log->SetVisAttributes(G4VisAttributes::GetInvisible());
            }
	    if (isTrayASTROMEV){	    
    		if (versionASTROMEV==1) {
            	       SiXBottomDetector_log->SetVisAttributes( SiDetector_visAtt );
            	       SiYBottomDetector_log->SetVisAttributes( SiDetector_visAtt );
            	       StripXBottom_log->SetVisAttributes(G4VisAttributes::GetInvisible());
            	       StripYBottom_log->SetVisAttributes(G4VisAttributes::GetInvisible());
            	       SiXTopDetector_log->SetVisAttributes( SiDetector_visAtt );
            	       SiYTopDetector_log->SetVisAttributes( SiDetector_visAtt );
            	       StripXTop_log->SetVisAttributes(G4VisAttributes::GetInvisible());
            	       StripYTop_log->SetVisAttributes(G4VisAttributes::GetInvisible());
		}
    		if (versionASTROMEV==2) {
            	       SiBottomDetector_log->SetVisAttributes( SiDetector_visAtt );
            	       PixelBottom_log->SetVisAttributes(G4VisAttributes::GetInvisible());
            	       SiTopDetector_log->SetVisAttributes( SiDetector_visAtt );
            	       PixelTop_log->SetVisAttributes(G4VisAttributes::GetInvisible());
		}
    		if (versionASTROMEV==3) {
            	       SiTopDetector_log->SetVisAttributes( SiDetector_visAtt );
            	       PixelTop_log->SetVisAttributes(G4VisAttributes::GetInvisible());
		}
            }
	    if (isGlueInTrk) GlueTray_visAtt = new G4VisAttributes(G4Colour::White());
            if (isGlueInTrk) GlueTray_log->SetVisAttributes(GlueTray_visAtt );
            if (isKaptonInTrk) KaptonTray_visAtt = new G4VisAttributes(G4Colour::Black());
            if (isKaptonInTrk) KaptonTray_log->SetVisAttributes(KaptonTray_visAtt );
            if (versionASTROMEV!=3) CFiberTray_visAtt = new G4VisAttributes(G4Colour::White());
            if (versionASTROMEV!=3) CFiberTray_log->SetVisAttributes(CFiberTray_visAtt );
            AlHoneyTray_visAtt = new G4VisAttributes(G4Colour::Black());
            AlHoneyTray_log->SetVisAttributes(AlHoneyTray_visAtt );
            if (isConvertInTrk) ConverterTray_visAtt = new G4VisAttributes(G4Colour::White());
            if (isConvertInTrk) ConverterTray_log->SetVisAttributes(ConverterTray_visAtt );
        }
        if (visType==2) {
            SiDetector_visAtt = new G4VisAttributes(G4Colour::Grey());
	    if (isTrayAgile){	    
            	SiXDetector_log->SetVisAttributes( SiDetector_visAtt );
            	SiYDetector_log->SetVisAttributes( SiDetector_visAtt );
            }
	    if (isTrayASTROMEV){	    
    		if (versionASTROMEV==1) {
            		SiXBottomDetector_log->SetVisAttributes( SiDetector_visAtt );
            		SiYBottomDetector_log->SetVisAttributes( SiDetector_visAtt );
            		SiXTopDetector_log->SetVisAttributes( SiDetector_visAtt );
            		SiYTopDetector_log->SetVisAttributes( SiDetector_visAtt );
		}
    		if (versionASTROMEV==2) {
            		SiBottomDetector_log->SetVisAttributes( SiDetector_visAtt );
            		SiTopDetector_log->SetVisAttributes( SiDetector_visAtt );
		}
    		if (versionASTROMEV==3) {
            		SiTopDetector_log->SetVisAttributes( SiDetector_visAtt );
		}
            }
	    if (isGlueInTrk) GlueTray_log->SetVisAttributes(G4VisAttributes::GetInvisible());
            if (isKaptonInTrk) KaptonTray_log->SetVisAttributes(G4VisAttributes::GetInvisible());
            if (versionASTROMEV!=3) CFiberTray_log->SetVisAttributes(G4VisAttributes::GetInvisible());
            AlHoneyTray_log->SetVisAttributes(G4VisAttributes::GetInvisible());
            if (isConvertInTrk) ConverterTray_log->SetVisAttributes(G4VisAttributes::GetInvisible());
        }
    }
   
    if ((isStrip && isStripReplicated) || (isPixel && isPixelReplicated)) {
    	if (isTrayAgile){
	        //Strip pysical volumes
	        for (G4long st = 0; st < Strip_N; st++)
	        {
	            G4ThreeVector StripPosX(-(Tray_side/2.0) + (Strip_pitch/2.) + (Strip_pitch*st), 0.0, 0.0);
	            G4ThreeVector StripPosY(0.0, -(Tray_side/2.0) + (Strip_pitch/2.) + (Strip_pitch*st), 0.0);

	            //StripX_phys = new G4PVPlacement(0, StripPosX, StripX_log,"pStripX", SiXDetector_log, false, st);
	            //StripY_phys = new G4PVPlacement(0, StripPosY, StripY_log,"pStripY", SiYDetector_log, false, st);
		    StripX_phys = new G4PVReplica( "pStripX",StripX_log,SiXDetector_log, kXAxis, Strip_N, Strip_pitch, 0);
		    StripY_phys = new G4PVReplica( "pStripY",StripY_log,SiYDetector_log, kYAxis, Strip_N, Strip_pitch, 0);
	        }
	}
    	if (isTrayASTROMEV){
    		if (versionASTROMEV==1) {
	               //Strip pysical volumes
	               for (G4long st = 0; st < Strip_N; st++)
	               {
	        	   G4ThreeVector StripPosX(-(Tray_side/2.0) + (Strip_pitch/2.) + (Strip_pitch*st), 0.0, 0.0);
	        	   G4ThreeVector StripPosY(0.0, -(Tray_side/2.0) + (Strip_pitch/2.) + (Strip_pitch*st), 0.0);

	        	   //StripXBottom_phys = new G4PVPlacement(0, StripPosX, StripX_log,"pStripX", SiXBottomDetector_log, false, st);
	        	   //StripYBottom_phys = new G4PVPlacement(0, StripPosY, StripY_log,"pStripY", SiYBottomDetector_log, false, st);
	        	   //StripXTop_phys = new G4PVPlacement(0, StripPosX, StripX_log,"pStripX", SiXTopDetector_log, false, st);
	        	   //StripYTop_phys = new G4PVPlacement(0, StripPosY, StripY_log,"pStripY", SiYTopDetector_log, false, st);
			   StripXBottom_phys = new G4PVReplica( "pStripXBottom",StripXBottom_log,SiXBottomDetector_log, kXAxis, Strip_N, Strip_pitch, 0);
			   StripYBottom_phys = new G4PVReplica( "pStripYBottom",StripYBottom_log,SiYBottomDetector_log, kYAxis, Strip_N, Strip_pitch, 0);
			   StripXTop_phys = new G4PVReplica( "pStripXTop",StripXTop_log,SiXTopDetector_log, kXAxis, Strip_N, Strip_pitch, 0);
			   StripYTop_phys = new G4PVReplica( "pStripYTop",StripYTop_log,SiYTopDetector_log, kYAxis, Strip_N, Strip_pitch, 0);
	               }
		}
   		if (versionASTROMEV==2) {
	               //Pixel pysical volumes
    			G4int copyNoPixel = 0;
			PixelTop_phys = new G4PVReplica( "pPixelPos",PixelTop_log,PixelTopLine_log, kYAxis, Pixel_NY, Pixel_side, 0);
			PixelTopLine_phys = new G4PVReplica( "pPixelPosTopLine",PixelTopLine_log,SiTopDetector_log, kXAxis, Pixel_NX, Pixel_side, 0);

			PixelBottom_phys = new G4PVReplica( "pPixelPos",PixelBottom_log,PixelBottomLine_log, kYAxis, Pixel_NY, Pixel_side, 0);
			PixelBottomLine_phys = new G4PVReplica( "pPixelPosBottomLine",PixelBottomLine_log,SiBottomDetector_log, kXAxis, Pixel_NX, Pixel_side, 0);

		}
   		if (versionASTROMEV==3) {
	               //Pixel pysical volumes
    			G4int copyNoPixel = 0;
			PixelTop_phys = new G4PVReplica( "pPixelPos",PixelTop_log,PixelTopLine_log, kYAxis, Pixel_NY, Pixel_side, 0);
			PixelTopLine_phys = new G4PVReplica( "pPixelPosTopLine",PixelTopLine_log,SiTopDetector_log, kXAxis, Pixel_NX, Pixel_side, 0);
		}
	}
    }
	   

    G4int copyM;
    G4double ConverterTray_t_inTray;
    G4double LowerModule_t_inTray;
    G4double LastTotalModule_t_inTray;
    G4double SummedTotalModule_t_inTray = 0.0;

    for (G4int nt = 0; nt < Tray_N; nt++)
    {
        copyM = 1000000 + 1000000*nt;
        ConverterTray_t_inTray = ConverterTray_t;

        if (isTrayAgile && nt < FirstTrayConv) {
            ConverterTray_t_inTray = 0.0;
        }

        if (isTrayAgile) LowerModule_t_inTray = ConverterTray_t_inTray + GlueTray_t + KaptonTray_t + SiDetector_t;
        if (isTrayASTROMEV) {
    		if (versionASTROMEV==1) {
			LowerModule_t_inTray = ConverterTray_t_inTray + GlueTray_t + KaptonTray_t + (2.*SiDetector_t);
		}
    		if (versionASTROMEV==2) {
			LowerModule_t_inTray = ConverterTray_t_inTray + GlueTray_t + KaptonTray_t + SiDetector_t;
		}
    		if (versionASTROMEV==3) {
			LowerModule_t_inTray = ConverterTray_t_inTray + GlueTray_t + KaptonTray_t;
		}
	}
	
        // -------------> LOWER MODULE
        G4ThreeVector LowerModulePos(0.0, 0.0, (SummedTotalModule_t_inTray) + (nt*Tray_dist) + (LowerModule_t_inTray/2.));

        if (isTrayAgile) {
	 if (nt > 0) {
	    	G4ThreeVector SiLowerModulePos(0.0, 0.0, LowerModulePos[2] -(LowerModule_t_inTray/2.) + SiDetector_t/2.);
	    	G4ThreeVector GlueLowerModulePos(0.0, 0.0, LowerModulePos[2]  -(LowerModule_t_inTray/2.) + (SiDetector_t) + GlueTray_t/2.);
	    	G4ThreeVector KLowerModulePos(0.0, 0.0, LowerModulePos[2] -(LowerModule_t_inTray/2.) + (SiDetector_t) + GlueTray_t + KaptonTray_t/2.);
	    	G4ThreeVector ConvLowerModulePos(0.0, 0.0, LowerModulePos[2] -(LowerModule_t_inTray/2.) + (SiDetector_t) + GlueTray_t + KaptonTray_t + ConverterTray_t_inTray/2 ); 
		G4cout <<" Si X Z: " << LowerModulePos[2] -(LowerModule_t_inTray/2.) + SiDetector_t/2. << G4endl;           

            	if (!isStrip) SiXDetector_phys = new G4PVPlacement(0,SiLowerModulePos,SiXDetector_log,"pSiXDetector",World_log,false,copyM);
            	if (isStrip && isStripReplicated) SiXDetector_phys = new G4PVPlacement(0,SiLowerModulePos,SiXDetector_log,"pSiXDetector",World_log,false,copyM);

            	if (isStrip && !isStripReplicated) {
                	// %%%%%%%%%%%%%%%%%%%%%%%%%%%% STRIP NUMBER FROM 0 !!!!!!!! %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
                	for (G4long st = 0; st < Strip_N; st++) {
                	    G4ThreeVector StripXLowerModulePos(-(Tray_side/2.0) + (Strip_pitch/2.) + (Strip_pitch*st), 0.0,LowerModulePos[2]  -(LowerModule_t_inTray/2.) + SiDetector_t/2.);
                	    StripX_phys = new G4PVPlacement(0,StripXLowerModulePos,StripX_log,"pStripX",World_log,false, copyM + st);
                	}
            	}
                if (isGlueInTrk) GlueTray_phys = new G4PVPlacement(0,GlueLowerModulePos,GlueTray_log,"pGlueTray",World_log,false,copyM + 1*10000);
            	if (isKaptonInTrk) KaptonTray_phys = new G4PVPlacement(0,KLowerModulePos,KaptonTray_log,"pKaptonTray",World_log,false,copyM + 2*10000);
            	if (isConvertInTrk) {
                	if (!isTrayAgile) {
                    		ConverterTray_phys = new G4PVPlacement(0,ConvLowerModulePos,ConverterTray_log,"pConverterTray",World_log,false,copyM + 3*10000);
                	} else {
                    		if (nt >= FirstTrayConv) ConverterTray_phys = new G4PVPlacement(0,ConvLowerModulePos,ConverterTray_log,"pConverterTray",World_log,false,copyM + 3*10000);
                	}
            	}		
	 }
	}
	    
	if (isTrayASTROMEV) {
    		if (versionASTROMEV==1) {
	    		 G4ThreeVector SiYLowerModulePos(0.0, 0.0, LowerModulePos[2] -(LowerModule_t_inTray/2.) + SiDetector_t/2.); 
			 G4ThreeVector SiXLowerModulePos(0.0, 0.0, LowerModulePos[2] -(LowerModule_t_inTray/2.) + SiDetector_t + SiDetector_t/2.); // the X layer is placed above
	    		 G4ThreeVector GlueLowerModulePos(0.0, 0.0, LowerModulePos[2]  -(LowerModule_t_inTray/2.) + (2.*SiDetector_t) + GlueTray_t/2.);
	    		 G4ThreeVector KLowerModulePos(0.0, 0.0, LowerModulePos[2] -(LowerModule_t_inTray/2.) + (2.*SiDetector_t) + GlueTray_t + KaptonTray_t/2.);
	    		 G4ThreeVector ConvLowerModulePos(0.0, 0.0, LowerModulePos[2] -(LowerModule_t_inTray/2.) + (2.*SiDetector_t) + GlueTray_t + KaptonTray_t + ConverterTray_t_inTray/2 ); 
			 G4cout <<" Si X Bottom: " << LowerModulePos[2] -(LowerModule_t_inTray/2.) + SiDetector_t + SiDetector_t/2. << G4endl;    

            		 if (!isStrip) {
				 SiXBottomDetector_phys = new G4PVPlacement(0,SiXLowerModulePos,SiXBottomDetector_log,"pSiXBottomDetector",World_log,false,copyM); // X bottom = 0
				 SiYBottomDetector_phys = new G4PVPlacement(0,SiYLowerModulePos,SiYBottomDetector_log,"pSiYBottomDetector",World_log,false,copyM + 100000); // Y bottom = 1 
            		 }
			 if (isStrip && isStripReplicated) {
				 SiXBottomDetector_phys = new G4PVPlacement(0,SiXLowerModulePos,SiXBottomDetector_log,"pSiXBottomDetector",World_log,false,copyM);
				 SiYBottomDetector_phys = new G4PVPlacement(0,SiYLowerModulePos,SiYBottomDetector_log,"pSiYBottomDetector",World_log,false,copyM + 100000);
			 }
            		 if (isStrip && !isStripReplicated) {
                		 // %%%%%%%%%%%%%%%%%%%%%%%%%%%% STRIP NUMBER FROM 0 !!!!!!!! %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
                		 for (G4long st = 0; st < Strip_N; st++) {
                		     G4ThreeVector StripXBottomLowerModulePos(-(Tray_side/2.0) + (Strip_pitch/2.) + (Strip_pitch*st), 0.0,LowerModulePos[2]  -(LowerModule_t_inTray/2.) + SiDetector_t + SiDetector_t/2.);
                		     StripXBottom_phys = new G4PVPlacement(0,StripXBottomLowerModulePos,StripXBottom_log,"pStripXBottom",World_log,false, copyM + st);
                		     G4ThreeVector StripYBottomLowerModulePos(0.0, -(Tray_side/2.0) + (Strip_pitch/2.) + (Strip_pitch*st),LowerModulePos[2]  -(LowerModule_t_inTray/2.) + SiDetector_t/2.);
                		     StripYBottom_phys = new G4PVPlacement(0,StripYBottomLowerModulePos,StripYBottom_log,"pStripYBottom",World_log,false, (copyM+100000) + st);
                		 }
            		 }		                   
            		 if (isGlueInTrk) GlueTray_phys = new G4PVPlacement(0,GlueLowerModulePos,GlueTray_log,"pGlueTray",World_log,false,copyM + 1*10000);
            		 if (isKaptonInTrk) KaptonTray_phys = new G4PVPlacement(0,KLowerModulePos,KaptonTray_log,"pKaptonTray",World_log,false,copyM + 2*10000);
            		 if (isConvertInTrk) ConverterTray_phys = new G4PVPlacement(0,ConvLowerModulePos,ConverterTray_log,"pConverterTray",World_log,false,copyM + 3*10000);
		}
    		if (versionASTROMEV==2) {
	    		 G4ThreeVector SiLowerModulePos(0.0, 0.0, LowerModulePos[2] -(LowerModule_t_inTray/2.) + SiDetector_t/2.); 
	    		 G4ThreeVector GlueLowerModulePos(0.0, 0.0, LowerModulePos[2]  -(LowerModule_t_inTray/2.) + SiDetector_t + GlueTray_t/2.);
	    		 G4ThreeVector KLowerModulePos(0.0, 0.0, LowerModulePos[2] -(LowerModule_t_inTray/2.) + SiDetector_t + GlueTray_t + KaptonTray_t/2.);
	    		 G4ThreeVector ConvLowerModulePos(0.0, 0.0, LowerModulePos[2] -(LowerModule_t_inTray/2.) + SiDetector_t + GlueTray_t + KaptonTray_t + ConverterTray_t_inTray/2 ); 
			 G4cout <<" Si Bottom: " << LowerModulePos[2] -(LowerModule_t_inTray/2.) + SiDetector_t/2. << G4endl;    

            		 if (!isPixel) {
				 SiBottomDetector_phys = new G4PVPlacement(0,SiLowerModulePos,SiBottomDetector_log,"pSiBottomDetector",World_log,false,copyM); // bottom = 0
            		 }
			 if (isPixel && isPixelReplicated) {
				 SiBottomDetector_phys = new G4PVPlacement(0,SiLowerModulePos,SiBottomDetector_log,"pSiBottomDetector",World_log,false,copyM);
			 }
            		 if (isPixel && !isPixelReplicated) {
                		 // %%%%%%%%%%%%%%%%%%%%%%%%%%%% PIXEL NUMBER FROM 0 !!!!!!!! %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    				 G4int copyNoPixel = 0;

				 for (G4int ip=0; ip<Pixel_NY; ip++)
    				 {
        				 for (G4int jp=0; jp<Pixel_NX; jp++)
        				 {
                		     		 G4ThreeVector PixelLowerModulePos((-Tray_side/2.) + (jp*Pixel_side) + (Pixel_side/2.0), (-Tray_side/2.) + (ip*Pixel_side) + (Pixel_side/2.0),LowerModulePos[2]  -(LowerModule_t_inTray/2.) + SiDetector_t/2.);
            					 PixelBottom_phys = new G4PVPlacement(0, PixelLowerModulePos, PixelBottom_log, "pPixelBottom", World_log, false,copyM + copyNoPixel);
            					 copyNoPixel++;
        				 }
    				 }
            		 }		                   
            		 if (isGlueInTrk) GlueTray_phys = new G4PVPlacement(0,GlueLowerModulePos,GlueTray_log,"pGlueTray",World_log,false,copyM + 1*10000);
            		 if (isKaptonInTrk) KaptonTray_phys = new G4PVPlacement(0,KLowerModulePos,KaptonTray_log,"pKaptonTray",World_log,false,copyM + 2*10000);
            		 if (isConvertInTrk) ConverterTray_phys = new G4PVPlacement(0,ConvLowerModulePos,ConverterTray_log,"pConverterTray",World_log,false,copyM + 3*10000);
		}
    		if (versionASTROMEV==3) {
	    		 G4ThreeVector GlueLowerModulePos(0.0, 0.0, LowerModulePos[2]  -(LowerModule_t_inTray/2.) + GlueTray_t/2.);
	    		 G4ThreeVector KLowerModulePos(0.0, 0.0, LowerModulePos[2] -(LowerModule_t_inTray/2.) + GlueTray_t + KaptonTray_t/2.);
	    		 G4ThreeVector ConvLowerModulePos(0.0, 0.0, LowerModulePos[2] -(LowerModule_t_inTray/2.) + GlueTray_t + KaptonTray_t + ConverterTray_t_inTray/2 ); 

          		 if (isGlueInTrk) GlueTray_phys = new G4PVPlacement(0,GlueLowerModulePos,GlueTray_log,"pGlueTray",World_log,false,copyM + 1*10000);
            		 if (isKaptonInTrk) KaptonTray_phys = new G4PVPlacement(0,KLowerModulePos,KaptonTray_log,"pKaptonTray",World_log,false,copyM + 2*10000);
            		 if (isConvertInTrk) ConverterTray_phys = new G4PVPlacement(0,ConvLowerModulePos,ConverterTray_log,"pConverterTray",World_log,false,copyM + 3*10000);
		}
	}

        

        // -------------> CENTRAL MODULE
        G4ThreeVector CentralModulePos(0.0, 0.0, (SummedTotalModule_t_inTray) + (nt*Tray_dist) + (LowerModule_t_inTray + (CentralModule_t/2.)));

        G4ThreeVector CLowCentralModulePos(0.0, 0.0, CentralModulePos[2] -(AlHoneyTray_t/2. + CFiberTray_t/2.));
        G4ThreeVector AlCentralModulePos(0.0, 0.0, CentralModulePos[2]);
        G4ThreeVector CUpCentralModulePos(0.0, 0.0, CentralModulePos[2] +(AlHoneyTray_t/2. + CFiberTray_t/2.));

        if (versionASTROMEV!=3) CFiberTray_phys = new G4PVPlacement(0,CLowCentralModulePos,CFiberTray_log,"pCFiberTray",World_log,false,copyM + 4*10000);
        AlHoneyTray_phys = new G4PVPlacement(0,AlCentralModulePos,AlHoneyTray_log,"pAlHoneyTray",World_log,false, copyM + 5*10000);
        if (versionASTROMEV!=3) CFiberTray_phys = new G4PVPlacement(0,CUpCentralModulePos,CFiberTray_log,"pCFiberTray",World_log,false,copyM + 6*10000);


        // -------------> UPPER MODULE
        G4ThreeVector UpperModulePos(0.0, 0.0, (SummedTotalModule_t_inTray) + (nt*Tray_dist) + (LowerModule_t_inTray + CentralModule_t + (UpperModule_t/2.)));

	if (isTrayAgile){
	   if (nt < (Tray_N-1)) {

            	G4ThreeVector KUpperModulePos(0.0, 0.0, UpperModulePos[2] -(UpperModule_t/2.) + KaptonTray_t/2.);
            	G4ThreeVector GlueUpperModulePos(0.0, 0.0, UpperModulePos[2] -(UpperModule_t/2.) + KaptonTray_t + GlueTray_t/2.);
            	G4ThreeVector SiUpperModulePos(0.0, 0.0, UpperModulePos[2] -(UpperModule_t/2.) + KaptonTray_t + GlueTray_t + SiDetector_t/2.);

            	G4cout <<" Si Y Z: " << UpperModulePos[2] -(UpperModule_t/2.) + KaptonTray_t + GlueTray_t + SiDetector_t/2. << G4endl;

            	if (!isStrip) SiYDetector_phys = new G4PVPlacement(0,SiUpperModulePos,SiYDetector_log,"pSiYDetector",World_log,false,copyM + 9*10000);
            	if (isStrip && isStripReplicated) SiYDetector_phys = new G4PVPlacement(0,SiUpperModulePos,SiYDetector_log,"pSiYDetector",World_log,false,copyM + 9*10000);
            	if (isStrip && !isStripReplicated) {
                	for (G4long st = 0; st < Strip_N; st++) {
                    		G4ThreeVector StripYUpperModulePos(0.0,  -(Tray_side/2.0) + (Strip_pitch/2.) + (Strip_pitch*st), UpperModulePos[2] -(UpperModule_t/2.) + KaptonTray_t + GlueTray_t + SiDetector_t/2.);
                    		StripY_phys = new G4PVPlacement(0,StripYUpperModulePos,StripY_log,"pStripY",World_log,false, (copyM + 9*10000) + st);
                	}
            	}
			   if (isKaptonInTrk) KaptonTray_phys = new G4PVPlacement(0,KUpperModulePos,KaptonTray_log,"pKaptonTray",World_log,false,copyM + 7*10000);
           if (isGlueInTrk) GlueTray_phys = new G4PVPlacement(0,GlueUpperModulePos,GlueTray_log,"pGlueTray",World_log,false,copyM + 8*10000);
	  }
	}
	
	if (isTrayASTROMEV){
    		if (versionASTROMEV==1) {
            		  G4ThreeVector KUpperModulePos(0.0, 0.0, UpperModulePos[2] -(UpperModule_t/2.) + KaptonTray_t/2.);
            		  G4ThreeVector GlueUpperModulePos(0.0, 0.0, UpperModulePos[2] -(UpperModule_t/2.) + KaptonTray_t + GlueTray_t/2.);
            		  G4ThreeVector SiXUpperModulePos(0.0, 0.0, UpperModulePos[2] -(UpperModule_t/2.) + KaptonTray_t + GlueTray_t + SiDetector_t + SiDetector_t/2.);
            		  G4ThreeVector SiYUpperModulePos(0.0, 0.0, UpperModulePos[2] -(UpperModule_t/2.) + KaptonTray_t + GlueTray_t + SiDetector_t/2.);

            		  G4cout <<" Si X Top: " << UpperModulePos[2] -(UpperModule_t/2.) + KaptonTray_t + GlueTray_t + SiDetector_t + SiDetector_t/2. << G4endl;

            		  if (!isStrip) {
				  SiXTopDetector_phys = new G4PVPlacement(0,SiXUpperModulePos,SiXTopDetector_log,"pSiXTopDetector",World_log,false,copyM + 9*10000);
				  SiYTopDetector_phys = new G4PVPlacement(0,SiYUpperModulePos,SiYTopDetector_log,"pSiYTopDetector",World_log,false,(copyM+100000) + 9*10000);
            		  }
			  if (isStrip && isStripReplicated) {
				  SiXTopDetector_phys = new G4PVPlacement(0,SiXUpperModulePos,SiXTopDetector_log,"pSiXTopDetector",World_log,false,copyM + 9*10000);
				  SiYTopDetector_phys = new G4PVPlacement(0,SiYUpperModulePos,SiYTopDetector_log,"pSiYTopDetector",World_log,false,(copyM+100000) + 9*10000);
            		  }
			  if (isStrip && !isStripReplicated) {
                		  for (G4long st = 0; st < Strip_N; st++) {
                    			  G4ThreeVector StripXUpperModulePos(-(Tray_side/2.0) + (Strip_pitch/2.) + (Strip_pitch*st), 0.0, UpperModulePos[2] -(UpperModule_t/2.) + KaptonTray_t + GlueTray_t + SiDetector_t + SiDetector_t/2.);
                    			  StripXTop_phys = new G4PVPlacement(0, StripXUpperModulePos,StripXTop_log,"pStripXTop",World_log,false, (copyM + 9*10000) + st);
                    			  G4ThreeVector StripYUpperModulePos(0.0,  -(Tray_side/2.0) + (Strip_pitch/2.) + (Strip_pitch*st), UpperModulePos[2] -(UpperModule_t/2.) + KaptonTray_t + GlueTray_t + SiDetector_t/2.);
                    			  StripYTop_phys = new G4PVPlacement(0,StripYUpperModulePos,StripYTop_log,"pStripYTop",World_log,false, ((copyM+100000) + 9*10000) + st);
                		  }
            		  }
	   		  if (isKaptonInTrk) KaptonTray_phys = new G4PVPlacement(0,KUpperModulePos,KaptonTray_log,"pKaptonTray",World_log,false,copyM + 7*10000);
           		  if (isGlueInTrk) GlueTray_phys = new G4PVPlacement(0,GlueUpperModulePos,GlueTray_log,"pGlueTray",World_log,false,copyM + 8*10000);
		}
    		if (versionASTROMEV==2) {
            		  G4ThreeVector KUpperModulePos(0.0, 0.0, UpperModulePos[2] -(UpperModule_t/2.) + KaptonTray_t/2.);
            		  G4ThreeVector GlueUpperModulePos(0.0, 0.0, UpperModulePos[2] -(UpperModule_t/2.) + KaptonTray_t + GlueTray_t/2.);
            		  G4ThreeVector SiUpperModulePos(0.0, 0.0, UpperModulePos[2] -(UpperModule_t/2.) + KaptonTray_t + GlueTray_t + SiDetector_t/2.);

            		  G4cout <<" Si Top: " << UpperModulePos[2] -(UpperModule_t/2.) + KaptonTray_t + GlueTray_t + SiDetector_t/2. << G4endl;

            		  if (!isPixel) {
				  SiTopDetector_phys = new G4PVPlacement(0,SiUpperModulePos,SiTopDetector_log,"pSiTopDetector",World_log,false,copyM + 9*10000);
            		  }
			  if (isPixel && isPixelReplicated) {
				  SiTopDetector_phys = new G4PVPlacement(0,SiUpperModulePos,SiTopDetector_log,"pSiTopDetector",World_log,false,copyM + 9*10000);
            		  }
			  if (isPixel && !isPixelReplicated) {
                		 // %%%%%%%%%%%%%%%%%%%%%%%%%%%% PIXEL NUMBER FROM 0 !!!!!!!! %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    				 G4int copyNoPixel = 0;

				 for (G4int ip=0; ip<Pixel_NY; ip++)
    				 {
        				 for (G4int jp=0; jp<Pixel_NX; jp++)
        				 {
                		     		 G4ThreeVector PixelUpperModulePos((-Tray_side/2.) + (jp*Pixel_side) + (Pixel_side/2.0), (-Tray_side/2.) + (ip*Pixel_side) + (Pixel_side/2.0),UpperModulePos[2] -(UpperModule_t/2.) + KaptonTray_t + GlueTray_t + SiDetector_t/2.);
            					 PixelTop_phys = new G4PVPlacement(0, PixelUpperModulePos, PixelTop_log, "pPixelTop", World_log, false,(copyM + 9*10000) + copyNoPixel);
            					 copyNoPixel++;
        				 }
    				 }


            		  }
	   		  if (isKaptonInTrk) KaptonTray_phys = new G4PVPlacement(0,KUpperModulePos,KaptonTray_log,"pKaptonTray",World_log,false,copyM + 7*10000);
           		  if (isGlueInTrk) GlueTray_phys = new G4PVPlacement(0,GlueUpperModulePos,GlueTray_log,"pGlueTray",World_log,false,copyM + 8*10000);
		}
    		if (versionASTROMEV==3) {
            		  G4ThreeVector KUpperModulePos(0.0, 0.0, UpperModulePos[2] -(UpperModule_t/2.) + KaptonTray_t/2.);
            		  G4ThreeVector GlueUpperModulePos(0.0, 0.0, UpperModulePos[2] -(UpperModule_t/2.) + KaptonTray_t + GlueTray_t/2.);
            		  G4ThreeVector SiUpperModulePos(0.0, 0.0, UpperModulePos[2] -(UpperModule_t/2.) + KaptonTray_t + GlueTray_t + SiDetector_t/2.);

            		  G4cout <<" Si Top: " << UpperModulePos[2] -(UpperModule_t/2.) + KaptonTray_t + GlueTray_t + SiDetector_t/2. << G4endl;

            		  if (!isPixel) {
				  SiTopDetector_phys = new G4PVPlacement(0,SiUpperModulePos,SiTopDetector_log,"pSiTopDetector",World_log,false,copyM + 9*10000);
            		  }
			  if (isPixel && isPixelReplicated) {
				  SiTopDetector_phys = new G4PVPlacement(0,SiUpperModulePos,SiTopDetector_log,"pSiTopDetector",World_log,false,copyM + 9*10000);
            		  }
			  if (isPixel && !isPixelReplicated) {
                		 // %%%%%%%%%%%%%%%%%%%%%%%%%%%% PIXEL NUMBER FROM 0 !!!!!!!! %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    				 G4int copyNoPixel = 0;

				 for (G4int ip=0; ip<Pixel_NY; ip++)
    				 {
        				 for (G4int jp=0; jp<Pixel_NX; jp++)
        				 {
                		     		 G4ThreeVector PixelUpperModulePos((-Tray_side/2.) + (jp*Pixel_side) + (Pixel_side/2.0), (-Tray_side/2.) + (ip*Pixel_side) + (Pixel_side/2.0),UpperModulePos[2] -(UpperModule_t/2.) + KaptonTray_t + GlueTray_t + SiDetector_t/2.);
            					 PixelTop_phys = new G4PVPlacement(0, PixelUpperModulePos, PixelTop_log, "pPixelTop", World_log, false,(copyM + 9*10000) + copyNoPixel);
            					 copyNoPixel++;
        				 }
    				 }


            		  }
	   		  if (isKaptonInTrk) KaptonTray_phys = new G4PVPlacement(0,KUpperModulePos,KaptonTray_log,"pKaptonTray",World_log,false,copyM + 7*10000);
           		  if (isGlueInTrk) GlueTray_phys = new G4PVPlacement(0,GlueUpperModulePos,GlueTray_log,"pGlueTray",World_log,false,copyM + 8*10000);
		}
	}
	   
        LastTotalModule_t_inTray = LowerModule_t_inTray + CentralModule_t + UpperModule_t;
        SummedTotalModule_t_inTray = SummedTotalModule_t_inTray + LastTotalModule_t_inTray;


    }

    if (isTrayAgile){
    	    if (AddSensitivePassive){
		if (isGlueInTrk) gm.AddXYZDetector(GlueTray_log);	    
		if (isKaptonInTrk) gm.AddXYZDetector(KaptonTray_log);	    
	        if (isConvertInTrk) gm.AddXYZDetector(ConverterTray_log);	    
	        gm.AddXYZDetector(CFiberTray_log);	    
	        gm.AddXYZDetector(AlHoneyTray_log);	    	    
	    }
	    if (isStrip) {
	        gm.AddXYZDetector( StripX_log);
	        gm.AddXYZDetector( StripY_log);	
	    } else {
	        gm.AddXYZDetector( SiXDetector_log);
	        gm.AddXYZDetector( SiYDetector_log);
	    }
    }
    if (isTrayASTROMEV){
    	    if (versionASTROMEV==1) {
    		    if (AddSensitivePassive){
			if (isGlueInTrk) gm.AddXYZDetector(GlueTray_log);	    
			if (isKaptonInTrk) gm.AddXYZDetector(KaptonTray_log);	    
	        	gm.AddXYZDetector(CFiberTray_log);	    
	        	gm.AddXYZDetector(AlHoneyTray_log);	    	    
		    }
		    if (isStrip) {
	        	gm.AddXYZDetector( StripXTop_log);
	        	gm.AddXYZDetector( StripYTop_log);	
	        	gm.AddXYZDetector( StripXBottom_log);
	        	gm.AddXYZDetector( StripYBottom_log);	
		    } else {
	        	gm.AddXYZDetector( SiXTopDetector_log);
	        	gm.AddXYZDetector( SiYTopDetector_log);
	        	gm.AddXYZDetector( SiXBottomDetector_log);
	        	gm.AddXYZDetector( SiYBottomDetector_log);
		    }
	    }
    	    if (versionASTROMEV==2) {
    		    if (AddSensitivePassive){
			if (isGlueInTrk) gm.AddXYZDetector(GlueTray_log);	    
			if (isKaptonInTrk) gm.AddXYZDetector(KaptonTray_log);	    
	        	gm.AddXYZDetector(CFiberTray_log);	    
	        	gm.AddXYZDetector(AlHoneyTray_log);	    	    
		    }
		    if (isPixel) {
	        	gm.AddXYZDetector( PixelTop_log);	
	        	gm.AddXYZDetector( PixelBottom_log);	
		    } else {
	        	gm.AddXYZDetector( SiTopDetector_log);
	        	gm.AddXYZDetector( SiBottomDetector_log);
		    }
	    }
    	    if (versionASTROMEV==3) {
    		    if (AddSensitivePassive){
			if (isGlueInTrk) gm.AddXYZDetector(GlueTray_log);	    
			if (isKaptonInTrk) gm.AddXYZDetector(KaptonTray_log);	    
	        	gm.AddXYZDetector(AlHoneyTray_log);	    	    
		    }
		    if (isPixel) {
	        	gm.AddXYZDetector( PixelTop_log);	
		    } else {
	        	gm.AddXYZDetector( SiTopDetector_log);
		    }
	    }
    }
    // ##############################################################
    // ##############              HAT                  #############
    // ##############################################################

    isHat = false;
    gm.config->readInto(isHat, "GEOM.ST.HAT");
    G4cout << "GEOM.ST.HAT: " << isHat << G4endl;
    Hat_dist = 0.;
    gm.config->readInto(Hat_dist, "GEOM.ST.HAT.DISTANCE");
    G4cout << "GEOM.ST.HAT.DISTANCE: " << Hat_dist << G4endl;
    Hat_sens = false;
    gm.config->readInto(Hat_sens, "GEOM.ST.HAT.SENSITIVE");
    G4cout << "GEOM.ST.HAT.SENSITIVE: " << Hat_sens << G4endl;

    gm.config->readInto(CFiberHat_t, "GEOM.ST.HAT.CFIBER.THICKNESS");
    G4cout << "GEOM.ST.HAT.CFIBER.THICKNESS: " << CFiberHat_t << G4endl;
    gm.config->readInto(AlHoneyHat_t, "GEOM.ST.HAT.ALHONEY.THICKNESS");
    G4cout << "GEOM.ST.HAT.ALHONEY.THICKNESS: " << AlHoneyHat_t << G4endl;

    if (isTrayAgile) ElectroBox_h = ElectroBox_h - UpperModule_t; // Electronics height
    if (isTrayASTROMEV) ElectroBox_h = ElectroBox_h + LowerModule_t_inTray; // Electronics height
    
    if ((isTrayASTROMEV) && (versionASTROMEV == 3)){
    	if (isHat){

    		G4double Tracker_h = LowerModule_t + CentralModule_t + UpperModule_t;
    		for (G4int nt = 1; nt < Tray_N; nt++)
    		{
			Tracker_h = Tracker_h + (LowerModule_t + CentralModule_t + UpperModule_t) + Tray_dist; // Electronics height
    		}

		G4double Hat_h = Tracker_h + Hat_dist;

		CFiberHat_mat = materials->GetMaterial(22);
		AlHoneyHat_mat = materials->GetMaterial(24);
		
		CFiberHat_sol = new G4Box("sCFiberHat", Tray_side/2., Tray_side/2., CFiberHat_t/2.);
		CFiberHat_log = new G4LogicalVolume(CFiberHat_sol, CFiberHat_mat,"lCFiberHat",0,0,0);

		AlHoneyHat_sol = new G4Box("sAlHoneyHat", Tray_side/2., Tray_side/2., AlHoneyHat_t/2.);
    		AlHoneyHat_log = new G4LogicalVolume(AlHoneyHat_sol, AlHoneyHat_mat,"lAlHoneyHat",0,0,0);

        	G4ThreeVector CFiberLowHatPos(0.0, 0.0, Hat_h + (CFiberHat_t/2.));
        	G4ThreeVector AlHoneyHatPos(0.0, 0.0, Hat_h + CFiberHat_t + (AlHoneyHat_t/2.));
        	G4ThreeVector CFiberHighHatPos(0.0, 0.0, Hat_h + CFiberHat_t + AlHoneyHat_t + (CFiberHat_t/2.));

		CFiberHat_phys = new G4PVPlacement(0,CFiberLowHatPos,CFiberHat_log,"pCFiberHat",World_log,false,(Tray_N+1)*1000000 + 10000);
        	AlHoneyHat_phys = new G4PVPlacement(0,AlHoneyHatPos,AlHoneyHat_log,"pAlHoneyHat",World_log,false, (Tray_N+1)*1000000 + 2*10000);
		CFiberHat_phys = new G4PVPlacement(0,CFiberHighHatPos,CFiberHat_log,"pCFiberHat",World_log,false, (Tray_N+1)*1000000 + 3*10000);

		if (Hat_sens){
			gm.AddXYZDetector(AlHoneyHat_log);
			gm.AddXYZDetector(CFiberHat_log);
		}

		if (visAllBlack) {

			CFiberHat_log->SetVisAttributes( blue_visAtt);
        		AlHoneyHat_log->SetVisAttributes( blue_visAtt);
    		} else {
        		if (visType==0) {
            			CFiberHat_visAtt = new G4VisAttributes(G4Colour::Red());
            			CFiberHat_log->SetVisAttributes(CFiberHat_visAtt );
            			AlHoneyHat_visAtt = new G4VisAttributes(G4Colour::Yellow());
            			AlHoneyHat_log->SetVisAttributes(AlHoneyHat_visAtt );
	        	}
        		if (visType==1) {
				CFiberHat_visAtt = new G4VisAttributes(G4Colour::White());
				CFiberHat_log->SetVisAttributes(CFiberHat_visAtt );
            			AlHoneyHat_visAtt = new G4VisAttributes(G4Colour::Black());
            			AlHoneyHat_log->SetVisAttributes(AlHoneyHat_visAtt );

        		}	
	        	if (visType==2) {
				CFiberHat_log->SetVisAttributes(G4VisAttributes::GetInvisible());
            			AlHoneyHat_log->SetVisAttributes(G4VisAttributes::GetInvisible());
        		}
    		}


	}
    }    


    
    // ##############################################################
    // ##############              Electronics          #############
    // ##############################################################

    gm.config->readInto(GEOMELEC, "GEOM.ST.ELEC");
    G4cout << "GEOM.ST.ELEC: " << GEOMELEC << G4endl;
    gm.config->readInto(ElecType, "GEOM.ST.ELEC.TYPE");
    G4cout << "GEOM.ST.ELEC.TYPE: " << ElecType << G4endl;

    gm.config->readInto(visElecFlag, "GEOM.ST.ELEC.VIS");
    G4cout << "GEOM.ST.ELEC.VIS: " << visElecFlag << G4endl;
    gm.config->readInto(ElectroBox_dist, "GEOM.ST.ELEC.DISTANCE");
    G4cout << "GEOM.ST.ELEC.DISTANCE: " << ElectroBox_dist << G4endl;
    gm.config->readInto(ElectroBox_t, "GEOM.ST.ELEC.THICKNESS");
    G4cout << "GEOM.ST.ELEC.THICKNESS: " << ElectroBox_t << G4endl;

    // Measures
    ElectroBox_side = Tray_side + (2.0*ElectroBox_dist);  // Electronics side

    //G4double ConverterTray_t_inTray;
    //G4double LowerModule_t_inTray;

    ElectroBox_h = CentralModule_t + UpperModule_t;
    for (G4int nt = 1; nt < Tray_N; nt++)
    {
        ConverterTray_t_inTray = ConverterTray_t;

        if (isTrayAgile && nt < FirstTrayConv) {
            ConverterTray_t_inTray = 0.0;
        }

        if (isTrayAgile) LowerModule_t_inTray = ConverterTray_t_inTray + GlueTray_t + KaptonTray_t + SiDetector_t;
        if (isTrayASTROMEV) {
		if (versionASTROMEV==1) {
			LowerModule_t_inTray = ConverterTray_t_inTray + GlueTray_t + KaptonTray_t + (2.*SiDetector_t);
		}
		if (versionASTROMEV==2) {
			LowerModule_t_inTray = ConverterTray_t_inTray + GlueTray_t + KaptonTray_t + SiDetector_t;
		}
		if (versionASTROMEV==3) {
			LowerModule_t_inTray = ConverterTray_t_inTray + GlueTray_t + KaptonTray_t;
		}
        }
	ElectroBox_h = ElectroBox_h + (LowerModule_t_inTray + CentralModule_t + UpperModule_t) + Tray_dist; // Electronics height
    }

    if (isTrayAgile) ElectroBox_h = ElectroBox_h - UpperModule_t; // Electronics height
    if (isTrayASTROMEV) ElectroBox_h = ElectroBox_h + LowerModule_t_inTray; // Electronics height
    G4cout << "ELECTRONIC SYSTEM HEIGHT: " << ElectroBox_h << G4endl;

    G4double firstLowerModule_t;
    if (isTrayASTROMEV) {
    	if (versionASTROMEV==1) {
	        firstLowerModule_t = ConverterTray_t + GlueTray_t + KaptonTray_t + (2.*SiDetector_t);
	}
    	if (versionASTROMEV==2) {
	        firstLowerModule_t = ConverterTray_t + GlueTray_t + KaptonTray_t + SiDetector_t;
	}
    	if (versionASTROMEV==3) {
	        firstLowerModule_t = ConverterTray_t + GlueTray_t + KaptonTray_t;
	}
    } else if (isTrayAgile) {
	    	firstLowerModule_t = GlueTray_t + KaptonTray_t + SiDetector_t;
    }

    G4cout << "TRACKER HEIGHT [mm]: " << ElectroBox_h << G4endl;


    ElectroAl_t = 1*mm;
    ElectroSi_t = 1*mm;

    // Material
    ElectroBox_mat = materials->GetMaterial(12);
    ElectroAl_mat = materials->GetMaterial(1);
    ElectroSi_mat = materials->GetMaterial(9);

    // Box
    ElectroBox_sol = new G4Box("sElectroBox", ElectroBox_side/2., ElectroBox_t/2., ElectroBox_h/2.);
    ElectroBox_log = new G4LogicalVolume(ElectroBox_sol,ElectroBox_mat,"lElectroBox",0,0,0);
    G4ThreeVector ElectroBox_pos1(0.0, -(Tray_side/2. + ElectroBox_dist + ElectroBox_t/2.), (ElectroBox_h/2.0) + firstLowerModule_t); // -y
    G4ThreeVector ElectroBox_pos2(-(Tray_side/2. + ElectroBox_dist + ElectroBox_t/2.), 0.0, (ElectroBox_h/2.0) + firstLowerModule_t); // -x
    G4ThreeVector ElectroBox_pos3(0.0, +(Tray_side/2. + ElectroBox_dist + ElectroBox_t/2.), (ElectroBox_h/2.0) + firstLowerModule_t); // +y
    G4ThreeVector ElectroBox_pos4(+(Tray_side/2. + ElectroBox_dist + ElectroBox_t/2.), 0.0, (ElectroBox_h/2.0) + firstLowerModule_t); // +x

    // Aluminum box
    ElectroAlFull_sol = new G4Box("sElectroAlFull", ElectroBox_side/2., ElectroBox_t/2., ElectroBox_h/2.);
    ElectroAlEmpty_sol = new G4Box("sElectroAlEmpty", ElectroBox_side/2. - ElectroAl_t, ElectroBox_t/2. - ElectroAl_t, ElectroBox_h/2. - ElectroAl_t);
    ElectroAl_sol = new G4SubtractionSolid("sElectroAl", ElectroAlFull_sol, ElectroAlEmpty_sol);

    ElectroAl_log = new G4LogicalVolume(ElectroAl_sol,ElectroAl_mat,"lElectroAl",0,0,0);

    ElectroSi_sol = new G4Box("sElectroSi", ElectroBox_side/2. - ElectroAl_t, ElectroSi_t/2., ElectroBox_h/2. - ElectroAl_t);
    ElectroSi_log = new G4LogicalVolume(ElectroSi_sol,ElectroSi_mat,"lElectroSi",0,0,0);

    if (GEOMELEC) {
        if (ElecType == 0) {
            ElectroBox_phys = new G4PVPlacement(0,ElectroBox_pos1,ElectroBox_log,"pElectroBox",World_log,false,201); // -y
            ElectroBox_phys = new G4PVPlacement(zRot90,ElectroBox_pos2,ElectroBox_log,"pElectroBox",World_log,false,202);   // -x
        }
        ElectroBox_phys = new G4PVPlacement(0,ElectroBox_pos3,ElectroBox_log,"pElectroBox",World_log,false,203);     // +y
        ElectroBox_phys = new G4PVPlacement(zRot90,ElectroBox_pos4,ElectroBox_log,"pElectroBox",World_log,false,204);   // +x
        ElectroAl_phys = new G4PVPlacement(0,G4ThreeVector(0.0, 0.0, 0.0),ElectroAl_log,"pElectroAl", ElectroBox_log,false,205);
        ElectroSi_phys = new G4PVPlacement(0,G4ThreeVector(0.0, 0.0, 0.0),ElectroSi_log,"pElectroSi", ElectroBox_log,false,206);
    }

    // ##############################################################
    // ##############           Mainframe columns       #############
    // ##############################################################

    // Measures
    MainframeCol_t = 2.0*mm;
    MainframeCol_h = ElectroBox_h + 2.0*MainframeCol_t;
    MainframeCol_side = 30.*mm;

    // Material
    MainframeCol_mat = materials->GetMaterial(1);

    // Geometry
    MainframeColFull_sol = new G4Tubs("sMainframeColFull", 0.0, MainframeCol_side, MainframeCol_h/2.0, 0.0, pis2);
    MainframeColEmpty_sol = new G4Tubs("sMainframeColEmpty", 0.0, MainframeCol_side, MainframeCol_h/2.0 - MainframeCol_t, 0.0, pis2);
    MainframeCol_sol = new G4SubtractionSolid("sMainframeCol", MainframeColFull_sol, MainframeColEmpty_sol, 0, G4ThreeVector(MainframeCol_t, MainframeCol_t, 0.0));
    MainframeCol_log = new G4LogicalVolume(MainframeCol_sol,MainframeCol_mat,"lMainframeCol",0,0,0);

    //G4ThreeVector MainframeCol_pos1(- (ElectroBox_side + MainframeCol_t)/2.0 + ElectroBox_t, -(ElectroBox_t + MainframeCol_t)/2.0, 0.0);
    G4ThreeVector MainframeCol_pos1(- ((ElectroBox_side/2.0) + ElectroBox_t + MainframeCol_t), - ((ElectroBox_side/2.0) + ElectroBox_t + MainframeCol_t),(ElectroBox_h/2.0) + LowerModule_t );
    G4ThreeVector MainframeCol_pos2(- ((ElectroBox_side/2.0) + ElectroBox_t + MainframeCol_t), + ((ElectroBox_side/2.0) + ElectroBox_t + MainframeCol_t),(ElectroBox_h/2.0) + LowerModule_t );
    G4ThreeVector MainframeCol_pos3(+ ((ElectroBox_side/2.0) + ElectroBox_t + MainframeCol_t), + ((ElectroBox_side/2.0) + ElectroBox_t + MainframeCol_t),(ElectroBox_h/2.0) + LowerModule_t );
    G4ThreeVector MainframeCol_pos4(+ ((ElectroBox_side/2.0) + ElectroBox_t + MainframeCol_t), - ((ElectroBox_side/2.0) + ElectroBox_t + MainframeCol_t),(ElectroBox_h/2.0) + LowerModule_t );

    if (GEOMELEC) {
        if (ElecType == 0) MainframeCol_phys = new G4PVPlacement(0,MainframeCol_pos1,MainframeCol_log,"pMainframeCol", World_log,false,211);
        if (ElecType == 0) MainframeCol_phys = new G4PVPlacement(zRot90,MainframeCol_pos2,MainframeCol_log,"pMainframeCol", World_log,false,212);
        MainframeCol_phys = new G4PVPlacement(zRot180,MainframeCol_pos3,MainframeCol_log,"pMainframeCol", World_log,false,213);
        if (ElecType == 0) MainframeCol_phys = new G4PVPlacement(zRot270,MainframeCol_pos4,MainframeCol_log,"pMainframeCol", World_log,false,214);
    }

    // Visualization
    if (visAllBlack) {
        ElectroBox_log->SetVisAttributes( blue_visAtt );
        ElectroAl_log->SetVisAttributes( blue_visAtt);
        ElectroSi_log->SetVisAttributes( blue_visAtt);
        MainframeCol_log->SetVisAttributes( blue_visAtt);
    } else {
        if (visElecFlag) {
            // Electronics Visualization
            ElectroBox_log -> SetVisAttributes (G4VisAttributes::GetInvisible());
            ElectroAl_visAtt = new G4VisAttributes(G4Colour::Blue());
            ElectroAl_log->SetVisAttributes( ElectroAl_visAtt );
            ElectroSi_visAtt = new G4VisAttributes(G4Colour::Grey());
            ElectroSi_log->SetVisAttributes( ElectroSi_visAtt );

            // Mainframe Columns Visualization
            MainframeCol_visAtt = new G4VisAttributes(G4Colour::White());
            MainframeCol_log->SetVisAttributes( MainframeCol_visAtt );
        } else {
            ElectroBox_log -> SetVisAttributes (G4VisAttributes::GetInvisible());
            ElectroAl_log -> SetVisAttributes (G4VisAttributes::GetInvisible());
            ElectroSi_log-> SetVisAttributes (G4VisAttributes::GetInvisible());
            MainframeCol_log-> SetVisAttributes (G4VisAttributes::GetInvisible());
        }
    }
    return World_phys;
}

G4double GeometryST::ComputeTrackerHeight() {

    // ##############################################################
    // ##############            TRACKER                #############
    // ##############################################################

    gm.config->readInto(Tray_N, "GEOM.ST.TRAY.NUMBER");
    gm.config->readInto(Tray_side, "GEOM.ST.TRAY.SIDE");
    gm.config->readInto(Tray_dist, "GEOM.ST.TRAY.DIST");

    isTrayAgile = false;
    gm.config->readInto(isTrayAgile, "GEOM.ST.TRAY.AGILE");
    FirstTrayConv = 0;
    gm.config->readInto(FirstTrayConv, "GEOM.ST.TRAY.AGILE.CONV");

    isTrayASTROMEV = false;
    gm.config->readInto(isTrayASTROMEV, "GEOM.ST.TRAY.ASTROMEV");
    versionASTROMEV = 1;
    gm.config->readInto(versionASTROMEV, "GEOM.ST.TRAY.ASTROMEV.VERSION");


    gm.config->readInto(isConvertInTrk, "GEOM.ST.TRAY.CONVERTER");
    gm.config->readInto(isGlueInTrk, "GEOM.ST.TRAY.GLUE");
    gm.config->readInto(isKaptonInTrk, "GEOM.ST.TRAY.KAPTON");
    gm.config->readInto(SiDetector_t, "GEOM.ST.TRAY.SILICON.THICKNESS");
    gm.config->readInto(GlueTray_t, "GEOM.ST.TRAY.GLUE.THICKNESS");
    gm.config->readInto(KaptonTray_t, "GEOM.ST.TRAY.KAPTON.THICKNESS");
    gm.config->readInto(CFiberTray_t, "GEOM.ST.TRAY.CFIBER.THICKNESS");
    gm.config->readInto(AlHoneyTray_t, "GEOM.ST.TRAY.ALHONEY.THICKNESS");
    gm.config->readInto(ConverterTray_t, "GEOM.ST.TRAY.CONVERTER.THICKNESS");

    if (!isConvertInTrk) ConverterTray_t = 0.0;
    if (!isGlueInTrk) GlueTray_t = 0.0;
    if (!isKaptonInTrk) KaptonTray_t = 0.0;

    // Modules thickness
    if (isTrayAgile) {
	    CentralModule_t = (2.0*CFiberTray_t) + AlHoneyTray_t;
	    UpperModule_t = SiDetector_t + GlueTray_t + KaptonTray_t;
	    LowerModule_t = ConverterTray_t + GlueTray_t + KaptonTray_t + SiDetector_t;
	    TotalModule_t = LowerModule_t + CentralModule_t + UpperModule_t;
    }
    if (isTrayASTROMEV) {
	    if (versionASTROMEV==1) {
		  CentralModule_t = (2.0*CFiberTray_t) + AlHoneyTray_t;
		  UpperModule_t = (2.*SiDetector_t) + GlueTray_t + KaptonTray_t;
		  LowerModule_t = ConverterTray_t + GlueTray_t + KaptonTray_t + (2.*SiDetector_t);
		  TotalModule_t = LowerModule_t + CentralModule_t + UpperModule_t;
	    }
	    if (versionASTROMEV==2) {
		  CentralModule_t = (2.0*CFiberTray_t) + AlHoneyTray_t;
		  UpperModule_t = SiDetector_t + GlueTray_t + KaptonTray_t;
		  LowerModule_t = ConverterTray_t + GlueTray_t + KaptonTray_t + SiDetector_t;
		  TotalModule_t = LowerModule_t + CentralModule_t + UpperModule_t;
	    }
	    if (versionASTROMEV==3) {
		  CentralModule_t = (2.0*CFiberTray_t) + AlHoneyTray_t;
		  UpperModule_t = SiDetector_t + GlueTray_t + KaptonTray_t;
		  LowerModule_t = ConverterTray_t + GlueTray_t + KaptonTray_t;
		  TotalModule_t = LowerModule_t + CentralModule_t + UpperModule_t;
	    }
    }

    // ##############################################################
    // ##############              Electronics          #############
    // ##############################################################


    gm.config->readInto(visElecFlag, "GEOM.ST.ELEC.VIS");
    gm.config->readInto(ElectroBox_dist, "GEOM.ST.ELEC.DISTANCE");
    gm.config->readInto(ElectroBox_t, "GEOM.ST.ELEC.THICKNESS");

    // Measures
    ElectroBox_side = Tray_side + (2.0*ElectroBox_dist);  // Electronics side

    G4double ConverterTray_t_inTray;
    G4double LowerModule_t_inTray;

    if (isTrayAgile) ElectroBox_h = CentralModule_t + UpperModule_t;
    if (isTrayASTROMEV) ElectroBox_h = LowerModule_t + CentralModule_t + UpperModule_t;
    
    for (G4int nt = 1; nt < Tray_N; nt++)
    {
        ConverterTray_t_inTray = ConverterTray_t;

        if (isTrayAgile && nt < FirstTrayConv) {
            ConverterTray_t_inTray = 0.0;
        }

        if (isTrayAgile) LowerModule_t_inTray = ConverterTray_t_inTray + GlueTray_t + KaptonTray_t + SiDetector_t;
        if ((isTrayASTROMEV) && (versionASTROMEV==1)) LowerModule_t_inTray = ConverterTray_t_inTray + GlueTray_t + KaptonTray_t + (2.*SiDetector_t);
        if ((isTrayASTROMEV) && (versionASTROMEV==2)) LowerModule_t_inTray = ConverterTray_t_inTray + GlueTray_t + KaptonTray_t + SiDetector_t;
        if ((isTrayASTROMEV) && (versionASTROMEV==3)) LowerModule_t_inTray = ConverterTray_t_inTray + GlueTray_t + KaptonTray_t;
        ElectroBox_h = ElectroBox_h + (LowerModule_t_inTray + CentralModule_t + UpperModule_t) + Tray_dist; // Electronics height
    }

    if (isTrayAgile) ElectroBox_h = ElectroBox_h - UpperModule_t; // Electronics height

    ElectroAl_t = 1*mm;
    ElectroSi_t = 1*mm;


    // ##############################################################
    // ##############           Mainframe columns       #############
    // ##############################################################

    // Measures
    MainframeCol_t = 2.0*mm;
    MainframeCol_h = ElectroBox_h + 2.0*MainframeCol_t;
    MainframeCol_side = 30.*mm;

    return ElectroBox_h;
}

G4double GeometryST::ComputeTrkCalDistance() {


    // ##############################################################
    // ##############            TRACKER                #############
    // ##############################################################

    gm.config->readInto(Tray_N, "GEOM.ST.TRAY.NUMBER");
    gm.config->readInto(Tray_side, "GEOM.ST.TRAY.SIDE");
    gm.config->readInto(Tray_dist, "GEOM.ST.TRAY.DIST");

    isTrayAgile = false;
    gm.config->readInto(isTrayAgile, "GEOM.ST.TRAY.AGILE");
    FirstTrayConv = 0;
    gm.config->readInto(FirstTrayConv, "GEOM.ST.TRAY.AGILE.CONV");

    isTrayASTROMEV = false;
    gm.config->readInto(isTrayASTROMEV, "GEOM.ST.TRAY.ASTROMEV");
    versionASTROMEV = 1;
    gm.config->readInto(versionASTROMEV, "GEOM.ST.TRAY.ASTROMEV.VERSION");

    gm.config->readInto(isConvertInTrk, "GEOM.ST.TRAY.CONVERTER");
    gm.config->readInto(isGlueInTrk, "GEOM.ST.TRAY.GLUE");
    gm.config->readInto(SiDetector_t, "GEOM.ST.TRAY.SILICON.THICKNESS");
    gm.config->readInto(GlueTray_t, "GEOM.ST.TRAY.GLUE.THICKNESS");
    gm.config->readInto(KaptonTray_t, "GEOM.ST.TRAY.KAPTON.THICKNESS");
    gm.config->readInto(CFiberTray_t, "GEOM.ST.TRAY.CFIBER.THICKNESS");
    gm.config->readInto(AlHoneyTray_t, "GEOM.ST.TRAY.ALHONEY.THICKNESS");
    gm.config->readInto(ConverterTray_t, "GEOM.ST.TRAY.CONVERTER.THICKNESS");

    G4double DistCal;
    gm.config->readInto(DistCal, "GEOM.ST.CAL.DISTANCE");


    if (!isConvertInTrk) ConverterTray_t = 0.0;
    if (!isGlueInTrk) GlueTray_t = 0.0;
    if (!isKaptonInTrk) KaptonTray_t = 0.0;

    // Modules thickness
    G4double Trk_Cal_dist;
    if (isTrayAgile) {
	    CentralModule_t = (2.0*CFiberTray_t) + AlHoneyTray_t;
	    UpperModule_t = SiDetector_t + GlueTray_t + KaptonTray_t;
	    LowerModule_t = ConverterTray_t + GlueTray_t + KaptonTray_t + SiDetector_t;
	    TotalModule_t = LowerModule_t + CentralModule_t + UpperModule_t;

	    G4double firstLowerModule_t;
    	    if (FirstTrayConv > 0) firstLowerModule_t = GlueTray_t + KaptonTray_t + SiDetector_t;
            else firstLowerModule_t = ConverterTray_t + GlueTray_t + KaptonTray_t + SiDetector_t;
	    
	    Trk_Cal_dist = DistCal + firstLowerModule_t;

    }
    if (isTrayASTROMEV) {
	    Trk_Cal_dist = DistCal;
    }    
    return Trk_Cal_dist;
}

G4double GeometryST::ComputeTrkZstart() {


    // ##############################################################
    // ##############            TRACKER                #############
    // ##############################################################

    gm.config->readInto(Tray_N, "GEOM.ST.TRAY.NUMBER");
    gm.config->readInto(Tray_side, "GEOM.ST.TRAY.SIDE");
    gm.config->readInto(Tray_dist, "GEOM.ST.TRAY.DIST");

    isTrayAgile = false;
    gm.config->readInto(isTrayAgile, "GEOM.ST.TRAY.AGILE");
    FirstTrayConv = 0;
    gm.config->readInto(FirstTrayConv, "GEOM.ST.TRAY.AGILE.CONV");

    isTrayASTROMEV = false;
    gm.config->readInto(isTrayASTROMEV, "GEOM.ST.TRAY.ASTROMEV");
    versionASTROMEV = 1;
    gm.config->readInto(versionASTROMEV, "GEOM.ST.TRAY.ASTROMEV.VERSION");

    gm.config->readInto(isConvertInTrk, "GEOM.ST.TRAY.CONVERTER");
    gm.config->readInto(isGlueInTrk, "GEOM.ST.TRAY.GLUE");
    gm.config->readInto(SiDetector_t, "GEOM.ST.TRAY.SILICON.THICKNESS");
    gm.config->readInto(GlueTray_t, "GEOM.ST.TRAY.GLUE.THICKNESS");
    gm.config->readInto(KaptonTray_t, "GEOM.ST.TRAY.KAPTON.THICKNESS");
    gm.config->readInto(CFiberTray_t, "GEOM.ST.TRAY.CFIBER.THICKNESS");
    gm.config->readInto(AlHoneyTray_t, "GEOM.ST.TRAY.ALHONEY.THICKNESS");
    gm.config->readInto(ConverterTray_t, "GEOM.ST.TRAY.CONVERTER.THICKNESS");
    G4double DistCal;
    gm.config->readInto(DistCal, "GEOM.ST.CAL.DISTANCE");

    if (!isConvertInTrk) ConverterTray_t = 0.0;
    if (!isGlueInTrk) GlueTray_t = 0.0;

    // Modules thickness
    //CentralModule_t = (2.0*CFiberTray_t) + AlHoneyTray_t;
    //UpperModule_t = SiDetector_t + GlueTray_t + KaptonTray_t;
    //LowerModule_t = ConverterTray_t + GlueTray_t + KaptonTray_t + SiDetector_t;
    //TotalModule_t = LowerModule_t + CentralModule_t + UpperModule_t;

    G4double firstLowerModule_t;
    G4int temp_zstart;
    if (isTrayAgile){
    	if (FirstTrayConv>0) {
		firstLowerModule_t = GlueTray_t + KaptonTray_t + SiDetector_t;
	}
        else {
		firstLowerModule_t = ConverterTray_t + GlueTray_t + KaptonTray_t + SiDetector_t;
	}
	temp_zstart = firstLowerModule_t;
    }
    if (isTrayASTROMEV){
    	if (versionASTROMEV==1) {
		firstLowerModule_t = ConverterTray_t + GlueTray_t + KaptonTray_t + (2.*SiDetector_t);
	}
        if (versionASTROMEV==2) {
		firstLowerModule_t = ConverterTray_t + GlueTray_t + KaptonTray_t + SiDetector_t;
	}
        if (versionASTROMEV==3) {
		firstLowerModule_t = ConverterTray_t + GlueTray_t + KaptonTray_t;
	}
	temp_zstart = firstLowerModule_t;

    }
    

    return temp_zstart;
}


