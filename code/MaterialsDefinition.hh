/***************************************************************************
                          SX5DetectorConstruction.hh  -  description
                             -------------------
    begin                : 2006-2008
    copyright            : (C) 2008 Remi Chipaux, modified by Andrea Bulgarelli
    email                : remi.chipaux@cea.fr, bulgarelli@iasfbo.inaf.it
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software for non commercial purpose              *
 *   and for public research institutes; you can redistribute it and/or    *
 *   modify it under the terms of the GNU General Public License.          *
 *   For commercial purpose see appropriate license terms                  *
 *                                                                         *
 ***************************************************************************/

#ifndef MaterialsDefinition_H
#define MaterialsDefinition_H 1

#include "G4Material.hh"
#include "G4MaterialTable.hh"

class G4Material;
class MaterialsDefinition;

class MaterialsDefinition {
public:

    MaterialsDefinition();
    ~MaterialsDefinition();

    G4Material* GetMaterial(int index);
    //G4double GetDensity(int index);

public:
    void DefineMaterials();
    void DestroyMaterials();

    // Define Elements

    G4Element* elH;  // Z = 1
    G4Element* elHe; // Z = 2
    G4Element* elLi; // Z = 3
    G4Element* elBe; // Z = 4
    G4Element* elB;  // Z = 5
    G4Element* elC;  // Z = 6
    G4Element* elN;  // Z = 7
    G4Element* elO;  // Z = 8
    G4Element* elF;  // Z = 9
    G4Element* elNe; // Z = 10
    G4Element* elNa; // Z = 11
    G4Element* elMg; // Z = 12
    G4Element* elAl; // Z = 13
    G4Element* elSi; // Z = 14
    G4Element* elP;  // Z = 15
    G4Element* elS;  // Z = 16
    G4Element* elCl; // Z = 17
    G4Element* elAr; // Z = 18
    G4Element* elK;  // Z = 19
    G4Element* elCa; // Z = 20
    G4Element* elSc; // Z = 21
    G4Element* elTi; // Z = 22
    G4Element* elV;  // Z = 23
    G4Element* elCr; // Z = 24
    G4Element* elMn; // Z = 25
    G4Element* elFe; // Z = 26
    G4Element* elCo; // Z = 27
    G4Element* elNi; // Z = 28
    G4Element* elCu; // Z = 29
    G4Element* elZn; // Z = 30
    G4Element* elGa; // Z = 31
    G4Element* elGe; // Z = 32
    G4Element* elAs; // Z = 33
    G4Element* elSe; // Z = 34
    G4Element* elBr; // Z = 35
    G4Element* elKr; // Z = 36
    G4Element* elRb; // Z = 37
    G4Element* elSr; // Z = 38
    G4Element* elY;  // Z = 39
    G4Element* elZr; // Z = 40
    G4Element* elNb; // Z = 41
    G4Element* elMo; // Z = 42
    G4Element* elTc; // Z = 43
    G4Element* elRu; // Z = 44
    G4Element* elRh; // Z = 45
    G4Element* elPd; // Z = 46
    G4Element* elAg; // Z = 47
    G4Element* elCd; // Z = 48
    G4Element* elIn; // Z = 49
    G4Element* elSn; // Z = 50
    G4Element* elSb; // Z = 51
    G4Element* elTe; // Z = 52
    G4Element* elI;  // Z = 53
    G4Element* elXe; // Z = 54
    G4Element* elCs; // Z = 55
    G4Element* elBa; // Z = 56
    G4Element* elLa; // Z = 57
    G4Element* elCe; // Z = 58
    G4Element* elPr; // Z = 59
    G4Element* elNd; // Z = 60
    G4Element* elPm; // Z = 61
    G4Element* elSm; // Z = 62
    G4Element* elEu; // Z = 63
    G4Element* elGd; // Z = 64
    G4Element* elTb; // Z = 65
    G4Element* elDy; // Z = 66
    G4Element* elHo; // Z = 67
    G4Element* elEr; // Z = 68
    G4Element* elTm; // Z = 69
    G4Element* elYb; // Z = 70
    G4Element* elLu; // Z = 71
    G4Element* elHf; // Z = 72
    G4Element* elTa; // Z = 73
    G4Element* elW;  // Z = 74
    G4Element* elRe; // Z = 75
    G4Element* elOs; // Z = 76
    G4Element* elIr; // Z = 77
    G4Element* elPt; // Z = 78
    G4Element* elAu; // Z = 79
    G4Element* elHg; // Z = 80
    G4Element* elTl; // Z = 81
    G4Element* elPb; // Z = 82
    G4Element* elBi; // Z = 83
    G4Element* elPo; // Z = 84
    G4Element* elAt; // Z = 85
    G4Element* elRn; // Z = 86
    G4Element* elFr; // Z = 87
    G4Element* elRa; // Z = 88
    G4Element* elAc; // Z = 89
    G4Element* elTh; // Z = 90
    G4Element* elPa; // Z = 91
    G4Element* elU;  // Z = 92
    G4Element* elNp; // Z = 93
    G4Element* elPu; // Z = 94
    G4Element* elAm; // Z = 95
    G4Element* elCm; // Z = 96
    G4Element* elBk; // Z = 97
    G4Element* elCf; // Z = 98
    G4Element* elEs; // Z = 99
    G4Element* elFm; // Z = 100

    //	Basic material
    G4Material* AlMaterial;
    G4Material* CMaterial;
    G4Material* PbMaterial;
    G4Material* SnMaterial;
    G4Material* CuMaterial;
    G4Material* TaMaterial;
    G4Material* WMaterial;
    G4Material* NiMaterial;

    //	Compound Materials
    G4Material* aluminium_metal;
    G4Material* gold_metal;
    G4Material* graphite;
    G4Material* copper_metal;
    G4Material* iron_metal;
    G4Material* lead_metal;
    G4Material* silicon_metal;
    G4Material* tin_metal;
    G4Material* tantalum_metal;
    G4Material* tungsten_metal;

    G4Material* air;
    G4Material* sea_air;
    G4Material* vacuum;

    G4Material* light_aluminium_metal;
    G4Material* Caliste_composite;
    G4Material* CsI;
    G4Material* CZT;
    G4Material* BGO;
    G4Material* CdZnTe;
    G4Material* CdTe;
    G4Material* LaBr3_old;
    G4Material* LaBr3;
    G4Material* LYSO;
    G4Material* NaI;
    G4Material* plastic_scintillator;
    G4Material* polystyrene;
    G4Material* pyrex_glass;
    G4Material* BC404;

    G4Material* CarbonFiber;
    G4Material* Kapton;
    G4Material* AlHoneycomb;
    G4Material* AlHoneycomb_modified;
    G4Material* Glue;

    G4Material* GL_Alpayload;

    G4Material* CircuitBoard;
    G4Material* Si3N4;
    G4Material* SiO2;
    G4Material* TiAlloy;
    G4Material* Steel304;
    
    G4MaterialPropertiesTable* plastic_scintillator_MPT;
    
    G4Material* AlAlloy6061;
    G4Material* AlAlloy6061HeTank;
    G4Material* AlAlloy6061JTShield;
    G4Material* AlAlloy6061Mainshell;
    G4Material* LiquidHelium;
    G4Material* Charcoal;
    G4Material* HgTe;
    G4Material* Alumina;
    G4Material* CFRP;
    G4Material* SUS304;
    G4Material* VanadiumPermender;
    G4Material* al_oxide;
    G4Material* Stycast2850;
    G4Material* TiAlloy15333;
    G4Material* ABS;
    G4Material* plate_glass;
    
    G4Material* polyeth;
    



    G4double HoneyDensity;
};

#endif
