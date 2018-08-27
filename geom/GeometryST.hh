
/***************************************************************************
                          GeometryST.hh  -  description
                             -------------------
    begin                : 2012
    copyright            : (C) 2012 by Valentina Fioretti
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

#ifndef GeometryST_H
#define GeometryST_H 1
#include "globals.hh"
#include "G4VUserDetectorConstruction.hh"
#include "G4TransportationManager.hh"

#include "G4ThreeVector.hh"

#include "G4Material.hh"
#include "G4VSolid.hh"
#include "G4Box.hh"
#include "G4Cons.hh"
#include "G4Trd.hh"
#include "G4Tubs.hh"
#include "G4Trap.hh"
#include "G4Sphere.hh"
#include "G4Polyhedra.hh"
#include "G4TessellatedSolid.hh"
#include "G4VFacet.hh"
#include "G4QuadrangularFacet.hh"
#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4PVParameterised.hh"

#include "G4SubtractionSolid.hh"
#include "G4IntersectionSolid.hh"
#include "G4UnionSolid.hh"
#include "G4Region.hh"
#include "G4RegionStore.hh"

#include "G4SDManager.hh"
#include "G4RunManager.hh"

#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4SolidStore.hh"

#include "G4PVReplica.hh"

#include "G4VisAttributes.hh"
#include "G4Colour.hh"

class G4LogicalVolume;
class G4VPhysicalVolume;
class G4Material;
class G4Region;
#include "G4VUserDetectorConstruction.hh"
#include "MaterialsDefinition.hh"

#include "CXYZSD.hh"


class GeometryST: public G4VUserDetectorConstruction {
public:

    GeometryST();
    ~GeometryST();

    G4VPhysicalVolume* Construct();
    void DefineMaterials();
    G4double ComputeTrackerHeight();
    G4double ComputeTrkCalDistance();
    G4double ComputeTrkZstart();
    G4VPhysicalVolume* ConstructGeometry(G4VPhysicalVolume* World_phys);

protected:

    void DefineSensitiveDetector();
    MaterialsDefinition* materials;


    // World
    G4VPhysicalVolume* World_phys;
    G4LogicalVolume* World_log;

    // Global parameters
    G4int Tray_N;  // Number of trays
    G4double Tray_side;
    G4double Tray_dist; // tray distance = distance between Silicon detectors

    G4bool isTrayAgile;  //tray > 10 has no converter
    G4bool isTrayASTROMEV;  //Double sided Si
    G4int versionASTROMEV;  //Double sided Si with pixels
    G4int FirstTrayConv; //tray number from 0, from 11 there is no converter

    G4bool isStrip;  //Strips are created
    G4bool isStripReplicated;  //Strips are created once or not
    G4double Strip_N; //total number of strips for each layer
    G4double Strip_pitch; //strip pitch

    G4bool isPixel;  //Pixel are created
    G4bool isPixelReplicated;  //Pixel are created once or not
    G4int Pixel_NX; //total number of pixel for each side X
    G4int Pixel_NY; //total number of pixel for each side Y
    G4int Pixel_N; //total number of pixels
    G4double Pixel_side; //pixel side

    G4bool isConvertInTrk;
    G4bool isGlueInTrk;
    G4bool isKaptonInTrk;
    G4bool isGlueToCu;
    G4bool AddPassive;
    G4bool AddSensitivePassive;
    G4int HoneyType;
    G4int visType;
    G4bool visAllBlack ;
    G4VisAttributes* black_visAtt;

    // Modules
    G4double CentralModule_t;
    G4double UpperModule_t;
    G4double LowerModule_t;
    G4double TotalModule_t;

    // Silicon Tracker
    G4Box* SiXDetector_sol;
    G4LogicalVolume* SiXDetector_log;
    G4VPhysicalVolume* SiXDetector_phys;
    G4Box* SiYDetector_sol;
    G4LogicalVolume* SiYDetector_log;
    G4VPhysicalVolume* SiYDetector_phys;

    G4Box* SiXBottomDetector_sol;
    G4LogicalVolume* SiXBottomDetector_log;
    G4VPhysicalVolume* SiXBottomDetector_phys;
    G4Box* SiYBottomDetector_sol;
    G4LogicalVolume* SiYBottomDetector_log;
    G4VPhysicalVolume* SiYBottomDetector_phys;
    G4Box* SiXTopDetector_sol;
    G4LogicalVolume* SiXTopDetector_log;
    G4VPhysicalVolume* SiXTopDetector_phys;
    G4Box* SiYTopDetector_sol;
    G4LogicalVolume* SiYTopDetector_log;
    G4VPhysicalVolume* SiYTopDetector_phys;

    G4Box* SiBottomDetector_sol;
    G4LogicalVolume* SiBottomDetector_log;
    G4VPhysicalVolume* SiBottomDetector_phys;
    G4Box* SiTopDetector_sol;
    G4LogicalVolume* SiTopDetector_log;
    G4VPhysicalVolume* SiTopDetector_phys;
    
    G4Material* SiDetector_mat;
    G4VisAttributes* SiDetector_visAtt;
    G4double SiDetector_t;

    // Strips X
    G4Box* StripX_sol;
    G4LogicalVolume* StripX_log;
    G4VPhysicalVolume* StripX_phys;

    G4Box* StripXBottom_sol;
    G4LogicalVolume* StripXBottom_log;
    G4VPhysicalVolume* StripXBottom_phys;

    G4Box* StripXTop_sol;
    G4LogicalVolume* StripXTop_log;
    G4VPhysicalVolume* StripXTop_phys;

    // Strips Y
    G4Box* StripY_sol;
    G4LogicalVolume* StripY_log;
    G4VPhysicalVolume* StripY_phys;

    G4Box* StripYBottom_sol;
    G4LogicalVolume* StripYBottom_log;
    G4VPhysicalVolume* StripYBottom_phys;

    G4Box* StripYTop_sol;
    G4LogicalVolume* StripYTop_log;
    G4VPhysicalVolume* StripYTop_phys;

    // Pixel
    G4Box* PixelBottom_sol;
    G4LogicalVolume* PixelBottom_log;
    G4VPhysicalVolume* PixelBottom_phys;
    G4Box* PixelTop_sol;
    G4LogicalVolume* PixelTop_log;
    G4VPhysicalVolume* PixelTop_phys;

    G4Box* PixelBottomLine_sol;
    G4LogicalVolume* PixelBottomLine_log;
    G4VPhysicalVolume* PixelBottomLine_phys;
    G4Box* PixelTopLine_sol;
    G4LogicalVolume* PixelTopLine_log;
    G4VPhysicalVolume* PixelTopLine_phys;

    // Strip material
    G4Material* SiStrip_mat;

    // Pixel material
    G4Material* SiPixel_mat;

    // Tray Glue
    G4Box* GlueTray_sol;
    G4LogicalVolume* GlueTray_log;
    G4VPhysicalVolume* GlueTray_phys;
    G4Material* GlueTray_mat;
    G4VisAttributes* GlueTray_visAtt;
    G4double GlueTray_t;


    // Tray Kapton
    G4Box* KaptonTray_sol;
    G4LogicalVolume* KaptonTray_log;
    G4VPhysicalVolume* KaptonTray_phys;
    G4Material* KaptonTray_mat;
    G4VisAttributes* KaptonTray_visAtt;
    G4double KaptonTray_t;

    // Tray Carbon fiber
    G4Box* CFiberTray_sol;
    G4LogicalVolume* CFiberTray_log;
    G4VPhysicalVolume* CFiberTray_phys;
    G4Material* CFiberTray_mat;
    G4VisAttributes* CFiberTray_visAtt;
    G4double CFiberTray_t;

    // Tray Al honeycomb
    G4Box* AlHoneyTray_sol;
    G4LogicalVolume* AlHoneyTray_log;
    G4VPhysicalVolume* AlHoneyTray_phys;
    G4Material* AlHoneyTray_mat;
    G4VisAttributes* AlHoneyTray_visAtt;
    G4double AlHoneyTray_t;

    // Tray Converter (W)
    G4Box* ConverterTray_sol;
    G4LogicalVolume* ConverterTray_log;
    G4VPhysicalVolume* ConverterTray_phys;
    G4Material* ConverterTray_mat;
    G4VisAttributes* ConverterTray_visAtt;
    G4double ConverterTray_t;

    // -----> Hat for the ASTROGAM V3.0 version

    G4bool isHat;
    G4double Hat_dist;
    G4bool Hat_sens;

    // CFiber
    G4Box* CFiberHat_sol;
    G4LogicalVolume* CFiberHat_log;
    G4VPhysicalVolume* CFiberHat_phys;
    G4Material* CFiberHat_mat;
    G4VisAttributes* CFiberHat_visAtt;
    G4double CFiberHat_t;
    // Al honeycomb
    G4Box* AlHoneyHat_sol;
    G4LogicalVolume* AlHoneyHat_log;
    G4VPhysicalVolume* AlHoneyHat_phys;
    G4Material* AlHoneyHat_mat;
    G4VisAttributes* AlHoneyHat_visAtt;
    G4double AlHoneyHat_t;

    // -----> Electronics

    G4bool GEOMELEC;
    G4int ElecType;   // 0 = GLIGHT, 1 = G400

    // Box
    G4bool visElecFlag;
    G4double ElectroBox_dist;
    G4double ElectroBox_side;
    G4double ElectroBox_t;
    G4double ElectroBox_h;
    G4Box* ElectroBox_sol;
    G4LogicalVolume* ElectroBox_log;
    G4VPhysicalVolume* ElectroBox_phys;
    G4Material* ElectroBox_mat;

    // Silicon layer
    G4double ElectroSi_t;
    G4Box* ElectroSi_sol;
    G4LogicalVolume* ElectroSi_log;
    G4VPhysicalVolume* ElectroSi_phys;
    G4Material* ElectroSi_mat;
    G4VisAttributes* ElectroSi_visAtt;

    // Al box
    G4double ElectroAl_t;
    G4Box* ElectroAlFull_sol;
    G4Box* ElectroAlEmpty_sol;
    G4SubtractionSolid* ElectroAl_sol;
    G4LogicalVolume* ElectroAl_log;
    G4VPhysicalVolume* ElectroAl_phys;
    G4Material* ElectroAl_mat;
    G4VisAttributes* ElectroAl_visAtt;

    // -----> Mainframe columns
    G4double MainframeCol_t;
    G4double MainframeCol_h;
    G4double MainframeCol_side;
    G4Tubs* MainframeColFull_sol;
    G4Tubs* MainframeColEmpty_sol;
    G4SubtractionSolid* MainframeCol_sol;
    G4LogicalVolume* MainframeCol_log;
    G4VPhysicalVolume* MainframeCol_phys;
    G4Material* MainframeCol_mat;
    G4VisAttributes* MainframeCol_visAtt;


};

#endif

