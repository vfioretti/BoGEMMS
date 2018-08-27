/***************************************************************************
                          GeometryBaseConstruction.hh  -  description
                             -------------------
    begin                : 2008
    copyright            : (C) 2008 by Andrea Bulgarelli
    email                : bulgarelli@iasfbo.inaf.it
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software for non commercial purpose              *
 *   and for public research institutes; you can redistribute it and/or    *
 *   modify it under the terms of the GNU General Public License.          *
 *   For commercial purpose see appropriate license terms                  *
 *                                                                         *
 ***************************************************************************/

#ifndef GeometryBaseConstruction_H
#define GeometryBaseConstruction_H 1
#include "globals.hh"
class G4LogicalVolume;
class G4VPhysicalVolume;
class G4Material;
class G4Region;
#include "G4VUserDetectorConstruction.hh"
#include "MaterialsDefinition.hh"
#include "G4UserLimits.hh"

//##Documentation
//## Box construction class
//## $Date: 2010/12/23 14:26:35 $
//## $Header: /home/repository/cvs/nhxm/bogems/include/GeometryBaseConstruction.hh,v 1.1.1.1 2010/12/23 14:26:35 bulgarelli Exp $
//## $Id: GeometryBaseConstruction.hh,v 1.1.1.1 2010/12/23 14:26:35 bulgarelli Exp $
//## $Revision: 1.1.1.1 $
//## \brief ox construction class
class GeometryBaseConstruction : public G4VUserDetectorConstruction {
public:

    GeometryBaseConstruction();
    ~GeometryBaseConstruction();

    G4VPhysicalVolume* Construct();
    G4VPhysicalVolume* ConstructGeometry(G4VPhysicalVolume* World_phys);
    void MakeBoxVolume(G4int i);


protected:

    void DefineSensitiveDetector();

    //Physical Volumes
    G4VPhysicalVolume* World_phys;
    G4VPhysicalVolume** physBox;

    // Logical Volumes
    G4LogicalVolume* World_log;
    G4LogicalVolume** logBox;

    MaterialsDefinition* materials;
    
    G4UserLimits* fStepLimit;

};

#endif

