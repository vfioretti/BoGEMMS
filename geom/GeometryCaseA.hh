
/***************************************************************************
                          GeometryCaseA.hh  -  description
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

#ifndef GeometryCaseA_H
#define GeometryCaseA_H 1
#include "globals.hh"

class G4LogicalVolume;
class G4VPhysicalVolume;
class G4Material;
class G4Region;
#include "G4VUserDetectorConstruction.hh"

class GeometryCaseA: public G4VUserDetectorConstruction {
public:

    GeometryCaseA();
    ~GeometryCaseA();

    G4VPhysicalVolume* Construct();
    void DefineMaterials();
    G4VPhysicalVolume* ConstructGeometry(G4VPhysicalVolume* World_phys);


protected:

    void DefineSensitiveDetector();

    //Physical Volumes
    G4VPhysicalVolume* World_phys;
    G4VPhysicalVolume* physSlab;

    //Logical Volumes
    G4LogicalVolume* World_log;
    G4LogicalVolume* logSlab;

    //Materials
    G4Material* CdZnTeMaterial;
    G4Material* SiMaterial;
    G4Material* VacuumMaterial;



};

#endif

