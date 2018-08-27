/***************************************************************************
                          THELSatellite.hh  -  description
                             -------------------
    begin                : 2010
    copyright            : (C) 2010, 2014  Andrea Bulgarelli
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
#ifndef THELSATELLITE_H
#define THELSATELLITE_H
#include "globals.hh"
#include "G4RunManager.hh"
class G4LogicalVolume;
class G4VPhysicalVolume;
class G4Material;
class G4Region;
#include "G4VUserDetectorConstruction.hh"
#include "GeometryBaseConstruction.hh"


class THELSatellite : public G4VUserDetectorConstruction {
public:
    THELSatellite();

    ~THELSatellite();

    G4VPhysicalVolume* Construct();

    void DefinePhysicList(G4RunManager * runManager);

    void MakeSteppingVolumes();

    void MakeSteppingVolume(G4int i);

protected:


    G4VPhysicalVolume* World_phys;

};

#endif
