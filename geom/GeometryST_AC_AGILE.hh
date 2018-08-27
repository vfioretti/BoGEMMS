
/***************************************************************************
                          GeometryST_AC_AGILE.hh  -  description
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

#ifndef GeometryST_AC_AGILE_H
#define GeometryST_AC_AGILE_H 1
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
#include "GeometryST.hh"

class GeometryST_AC_AGILE: public G4VUserDetectorConstruction {
public:

    GeometryST_AC_AGILE();
    ~GeometryST_AC_AGILE();

    G4VPhysicalVolume* Construct();
    void SetPhysicalWorld(G4VPhysicalVolume* World_phys);
    void DefineMaterials();
    //void ConstructGeometry();
    G4VPhysicalVolume* ConstructGeometry(G4VPhysicalVolume* World_phys);


protected:

    void DefineSensitiveDetector();
    MaterialsDefinition* materials;
    GeometryST* stget;


    // World
    G4VPhysicalVolume* World_phys;
    G4LogicalVolume* World_log;

    // Global parameters
    G4double ACDist;  // distance electronics - AC
    G4double ACBottomDist;  // distance tracker - bottom AC
    G4double ACLat_t; // Lateral AC thickness
    G4double ACLat_extra; // Lateral AC extra length (total side)
    G4double ACLat_side; // Lateral AC side
    G4double ACLat_total_side; // Lateral AC totale side
    G4double ACLat_h; // Lateral AC height
    G4double ACTop_t; // Top AC thickness
    G4double ACTop_side; // Top AC side
    G4double ACBottom_t; // Top AC thickness
    G4double ACBottom_side; // Top AC side
    G4bool visAllBlack ;
    G4VisAttributes* black_visAtt;


    G4bool visACFlag;
    G4bool visACTopFlag;
    G4bool viewInside;
    G4int CaseAC;
    G4bool SensAC;


    // Lateral AC
    G4Box* ACLatX_sol;
    G4LogicalVolume* ACLatX_log;
    G4VPhysicalVolume* ACLatX_phys;
    G4Box* ACLatY_sol;
    G4LogicalVolume* ACLatY_log;
    G4VPhysicalVolume* ACLatY_phys;

    // Top AC
    G4Box* ACTop_sol;
    G4LogicalVolume* ACTop_log;
    G4VPhysicalVolume* ACTop_phys;

    // Bottom AC
    G4Box* ACBottom_sol;
    G4LogicalVolume* ACBottom_log;
    G4VPhysicalVolume* ACBottom_phys;

    G4Material* AC_mat;
    G4VisAttributes* AC_visAtt;




};

#endif

