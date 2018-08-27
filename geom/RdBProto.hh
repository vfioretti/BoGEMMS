
/***************************************************************************
                          RdBProto.hh  -  description
                             -------------------
    begin                : 2009
    copyright            : (C) 2009 by Valentina Fioretti
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

#ifndef RdBProto_H
#define RdBProto_H 1
#include "globals.hh"

class G4LogicalVolume;
class G4VPhysicalVolume;
class G4Material;
class G4Region;

#include "G4VUserDetectorConstruction.hh"
#include "MaterialsDefinition.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"

class RdBProto: public G4VUserDetectorConstruction {
public:

    RdBProto();
    ~RdBProto();

    G4VPhysicalVolume* Construct();
    void DefineMaterials();
    G4VPhysicalVolume* ConstructGeometry(G4VPhysicalVolume* World_phys);


protected:

    void DefineSensitiveDetector();
    MaterialsDefinition* materials;
    
    //Physical Volumes
    G4VPhysicalVolume* World_phys;
    G4VPhysicalVolume* physSlab;
	G4VPhysicalVolume* physFakeSphere;
	
    //Logical Volumes
    G4LogicalVolume* World_log;
    G4LogicalVolume* logSlab;
    G4LogicalVolume* logFakeSphere;

	//Materials
    G4Material* Slab_mat;
    G4Material* Sphere_mat;

	//Visualization
	G4VisAttributes* VisSlab;
    G4VisAttributes* VisFakeSphere;
    
};

#endif

