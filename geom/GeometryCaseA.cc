/***************************************************************************
                          GeometryCaseA.cc  -  description
                             -------------------
    begin                : 2009
    copyright            : (C) 2009 by  Valentina Fioretti
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

#include "GeometryCaseA.hh"
#include "G4Element.hh"
#include "G4Material.hh"
#include "G4Box.hh"
#include "G4Cons.hh"
#include "G4Polyhedra.hh"
#include "G4Tubs.hh"
#include "G4Sphere.hh"
#include "G4LogicalVolume.hh"
#include "G4ThreeVector.hh"
#include "G4PVPlacement.hh"
#include "G4VisAttributes.hh"
#include "G4SubtractionSolid.hh"
#include "G4Region.hh"
#include "G4RegionStore.hh"
#include "THELGlobalMemory.hh"
#include "globals.hh"

#if defined(GEANT4_10_1) || defined(GEANT4_10_2) || defined(GEANT4_10_3) || defined(GEANT4_10_4)
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"
#endif

//GDML
//#include "G4Writer/G4GDMLWriter.h"



GeometryCaseA::GeometryCaseA() {

    World_phys = 0;
    World_log = 0;
    VacuumMaterial = 0;
}

GeometryCaseA::~GeometryCaseA() {}

G4VPhysicalVolume* GeometryCaseA::Construct() {

    //G4GDMLWriter g4writer("G:/Daten/Doktorarbeit/GEANT4/TheCode/GDML/GDMLSchema/gdml.xsd", "Simbol_X.gdml");
    DefineMaterials();
    World_phys = gm.ConstructWorld();
    World_log = gm.World_log;
    World_phys = ConstructGeometry(World_phys);
// 	G4cout << "G OK " << World_phys << " " << World_log << G4endl;
    return World_phys;

}
void GeometryCaseA::DefineMaterials() {
    G4String name, symbol;
    G4double a, z, density;

    G4int ncomponents;
    G4double fractionmass;
    G4double temperature, pressure;

    //Si
    G4Element* elSi = new G4Element(name="Silicon",  symbol="Si", z= 14., a = 28.09*g/mole);
    density = 2.330*g/cm3;
    G4Material* Si = new G4Material(name="Silicon", density, ncomponents=1);
    Si->AddElement(elSi, fractionmass=1.);
    SiMaterial = Si;

    //CdZnTe
    G4Element* elCd = new G4Element(name="Cadmium",   symbol="Cd", z= 48., a = 112.41*g/mole);
    G4Element* elZn = new G4Element(name="Zinc",      symbol="Zn", z= 30., a = 65.409*g/mole);
    G4Element* elTe = new G4Element(name="Tellurium", symbol="Te", z= 52., a = 127.6*g/mole);
    density= 5.81*g/cm3;
    G4Material* CdZnTe = new G4Material(name="CdZnTe", density, ncomponents=3);
    CdZnTe->AddElement(elCd, fractionmass=0.45);
    CdZnTe->AddElement(elZn, fractionmass=0.05);
    CdZnTe->AddElement(elTe, fractionmass=0.50);
    CdZnTeMaterial = CdZnTe;


    density     = universe_mean_density;
    pressure    = 3.e-18*pascal;
    temperature = 2.73*kelvin;
    G4Material* vacuum = new G4Material(name="Galactic", z=1., a=1.01*g/mole, density,kStateGas,temperature,pressure);
    VacuumMaterial  = vacuum;


}


G4VPhysicalVolume* GeometryCaseA::ConstructGeometry(G4VPhysicalVolume* World_phys) {

    //Rotations
    G4RotationMatrix* no_rotation = new G4RotationMatrix(0,0,0);

    G4double side = 80.*mm; //Detector side

    G4int geomcasea_mat = 0;
    gm.config->readInto(geomcasea_mat, "GEOM.CASEA.MATERIAL");
    G4cout << "GEOM.CASEA.MATERIAL: " << geomcasea_mat << G4endl;

    G4double t_det;
    gm.config->readInto(t_det, "GEOM.CASEA.THICK");
    G4cout << "GEOM.CASEA.THICK: " << t_det << G4endl;

    G4Material* DetMaterial;
    if(geomcasea_mat < 1 || geomcasea_mat > 2) {
        G4cerr << "Error in the material. Only 1 (Si) or 2 (CdZnTe)" << G4endl;
        exit(-1);
    }
    switch(geomcasea_mat) {
    case 1:
        DetMaterial = SiMaterial;
        break;
    case 2:
        DetMaterial = CdZnTeMaterial;
        break;
    }


    //Creation of detector geometry
    G4Box* slab = new G4Box("slab",
                            side/2.,
                            side/2.,
                            t_det/2.);



    //Logical and Physical Volume
    G4LogicalVolume* logSlab  = new G4LogicalVolume(slab,DetMaterial,"logSlab",0,0,0);
    physSlab       = new G4PVPlacement(0,G4ThreeVector(0.0, 0.0, 0.0),logSlab,"pSlab",World_log,false,0);
    physSlab->SetCopyNo(10);
    G4VisAttributes* VisSlab = new G4VisAttributes(G4Colour(255./255., 255./255., 0./255.));
    logSlab->SetVisAttributes( VisSlab );
    gm.AddSensitiveDetector( physSlab );
    gm.AddXYZDetector( logSlab);


    return World_phys;
}
