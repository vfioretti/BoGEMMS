
/***************************************************************************
                          GeometryST_SC.hh  -  description
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

#ifndef GeometryST_SC_H
#define GeometryST_SC_H 1
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

class GeometryST_SC: public G4VUserDetectorConstruction {
public:

    GeometryST_SC();
    ~GeometryST_SC();

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
    G4double CalDist;  // distance z=0 - calorimeter
    G4double CalHeight; // Calorimeter height
    G4double SCDist;  // distance spacecraft - calorimeter
    G4double SCSide;  // SC side
    G4double SCHeight; // SC height


    G4bool visSCFlag;
    G4bool visAllBlack ;
    G4VisAttributes* black_visAtt;


    // Lateral AC
    G4Box* SC_sol;
    G4LogicalVolume* SC_log;
    G4VPhysicalVolume* SC_phys;

    G4Material* SC_mat;
    G4VisAttributes* SC_visAtt;




};

#endif

