
/***************************************************************************
                          GeometryST_CAL.hh  -  description
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

#ifndef GeometryST_CAL_H
#define GeometryST_CAL_H 1
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

#include "G4VisAttributes.hh"
#include "G4Colour.hh"

class G4LogicalVolume;
class G4VPhysicalVolume;
class G4Material;
class G4Region;
#include "G4VUserDetectorConstruction.hh"
#include "MaterialsDefinition.hh"

#include "CXYZSD.hh"


class GeometryST_CAL: public G4VUserDetectorConstruction {
public:

    GeometryST_CAL();
    ~GeometryST_CAL();

    G4VPhysicalVolume* Construct();
    void SetPhysicalWorld(G4VPhysicalVolume* World_phys);
    void DefineMaterials();
    G4VPhysicalVolume* ConstructGeometry(G4VPhysicalVolume* World_phys);


protected:

    void DefineSensitiveDetector();
    MaterialsDefinition* materials;


    // World
    G4VPhysicalVolume* World_phys;
    G4LogicalVolume* World_log;

    // Global parameters
    G4VisAttributes* black_visAtt;
    G4double CalDist;  // distance z=0 - calorimeter
    G4double CalPixelSide; // pixel side
    G4double CalHeight; // Calorimeter height
    G4double CalSide; // Calorimeter side
    G4double SDD_t; // SDD Silicon thickness
    G4double CB_t; // Circuit board thickness

    
    G4double CalNpixX; // N pixel in x
    G4double CalNpixY; // N pixel in y
    G4bool visFlag;
    G4bool visAllBlack ;


    // Calorimeter
    G4Box* Cal_sol;
    G4LogicalVolume* Cal_log;
    G4VPhysicalVolume* Cal_phys;
    G4Box* CalPixel_sol;
    G4LogicalVolume* CalPixel_log;
    G4Material* Cal_mat;
    G4Material* CalPixel_mat;
    G4VisAttributes* CalPixel_visAtt;

    // SDD
    G4Box* SDD_sol;
    G4LogicalVolume* SDD_log;
    G4Material* SDD_mat;
    G4VisAttributes* SDD_visAtt;

    // Circuit Board
    G4Box* CB_sol;
    G4LogicalVolume* CB_log;
    G4Material* CB_mat;
    G4VisAttributes* CB_visAtt;


};

#endif

