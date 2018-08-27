
/***************************************************************************
                          GeometryST_CAL_AGILE.hh  -  description
                             -------------------
    begin                : 2013
    copyright            : (C) 2013 by Valentina Fioretti
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

#ifndef GeometryST_CAL_AGILE_H
#define GeometryST_CAL_AGILE_H 1
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


class GeometryST_CAL_AGILE: public G4VUserDetectorConstruction {
public:

    GeometryST_CAL_AGILE();
    ~GeometryST_CAL_AGILE();

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
    G4double CalBarSide; // bar side
    G4double CalBarHeight; // bar height
    G4double CalBarsDist; // bar distance (X-Y)
    G4double CalSide; // Calorimeter side
    G4double CF_t; // Carbon Fiber thickness

    G4double CalNbarX; // N bar in x
    G4double CalNbarY; // N bar in y
    G4bool visFlag;
    G4bool visAllBlack ;


    // Calorimeter
    G4Box* CalX_sol;
    G4Box* CalY_sol;
    G4LogicalVolume* CalX_log;
    G4LogicalVolume* CalY_log;
    G4VPhysicalVolume* CalX_phys;
    G4VPhysicalVolume* CalY_phys;
    G4Box* CalBarX_sol;
    G4LogicalVolume* CalBarX_log;
    G4Box* CalBarY_sol;
    G4LogicalVolume* CalBarY_log;
    G4VPhysicalVolume* CalBarX_phys;
    G4VPhysicalVolume* CalBarY_phys;
    G4Material* Cal_mat;
    G4Material* CalBar_mat;
    G4VisAttributes* CalBar_visAtt;




};

#endif

