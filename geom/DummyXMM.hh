
/***************************************************************************
                          DummyXMM.hh  -  description
                             -------------------
    begin                : 2014
    copyright            : (C) 2014 by Simone Lotti
    copyright            : (C) 2014 by Valentina Fioretti
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

#ifndef DummyXMM_H
#define DummyXMM_H 1
#include "globals.hh"

class G4LogicalVolume;
class G4VPhysicalVolume;
class G4Material;
class G4Region;

#include "G4VisAttributes.hh"
#include "MaterialsDefinition.hh"
#include "G4VUserDetectorConstruction.hh"

class DummyXMM: public G4VUserDetectorConstruction {
public:

    DummyXMM();
    ~DummyXMM();

    G4VPhysicalVolume* Construct();
    G4VPhysicalVolume* ConstructGeometry(G4VPhysicalVolume* World_phys);


protected:

    void DefineSensitiveDetector();
    MaterialsDefinition* materials;


    // World
    //Physical Volumes
    G4VPhysicalVolume* World_phys;
    G4LogicalVolume* World_log;


    // Proton shield
    G4LogicalVolume* logShield;
    G4VPhysicalVolume* physShield;
    G4VisAttributes* VisShield;
    G4Material* Shield_mat;
    
    // Detector
    G4LogicalVolume* logDetector;
    G4VPhysicalVolume* physDetector;
    G4VisAttributes* VisDetector;
    G4Material* Detector_mat;
    
    // Filter
    G4LogicalVolume* logFilter;
    G4VPhysicalVolume* physFilter;
    G4VisAttributes* VisFilter;
    G4Material* Filter_mat;
    
    // CraftBox
    G4LogicalVolume* logCraftBox;
    G4VPhysicalVolume* physCraftBox;
    G4VisAttributes* VisCraftBox;
    G4Material* CraftBox_mat;
    
    // Hat
    G4LogicalVolume* logHat;
    G4VPhysicalVolume* physHat;
    G4VisAttributes* VisHat;
    G4Material* Hat_mat;
    
    // Dummy Sphere
    G4LogicalVolume* logDummySphere;
    G4VPhysicalVolume* physDummySphere;
    G4VisAttributes* VisDummySphere;
    G4Material* dummySphere_mat;
    
};

#endif

