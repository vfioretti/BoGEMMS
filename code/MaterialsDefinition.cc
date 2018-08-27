/***************************************************************************
                          SX5DetectorConstruction.cc  -  description
                             -------------------
    begin                : 2006-2010
    copyright            : (C) 2006 Remi Chipaux, modified by Andrea Bulgarelli
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

#include "MaterialsDefinition.hh"
#include "THELGlobalMemory.hh"

#include "G4ios.hh"
#include "globals.hh"
#include "G4NistManager.hh"

#if defined(GEANT4_10_1) || defined(GEANT4_10_2) || defined(GEANT4_10_3) || defined(GEANT4_10_4)
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"
#endif


MaterialsDefinition::MaterialsDefinition() {
    DefineMaterials();
}

MaterialsDefinition::~MaterialsDefinition() {}

void MaterialsDefinition::DefineMaterials() {
    G4String name, symbol;
    G4double a, z, density;
    G4double fractionmass;
    G4double temperature, pressure;
    G4int ncomponents, natoms;

    G4NistManager* manNist = G4NistManager::Instance();

    // Elements

    elH  = new G4Element(name="Hydrogen",  symbol="H",  z=  1., a = 1.00794*g/mole);
    elHe = new G4Element(name="Helium",    symbol="He", z=  2., a = 4.002602*g/mole);
    elB  = new G4Element(name="Boron",     symbol="B",  z=  5., a = 10.811*g/mole);
    elC  = new G4Element(name="Carbon",    symbol="C",  z=  6., a = 12.0107*g/mole);
    elN  = new G4Element(name="Nitrogen",  symbol="N",  z=  7., a = 14.0067*g/mole);
    elO  = new G4Element(name="Oxygen",    symbol="O",  z=  8., a = 15.9994*g/mole);
    elF  = new G4Element(name="Fluor",     symbol="F",  z=  9., a = 18.998403*g/mole);
    elNa = new G4Element(name="Sodium",    symbol="Na", z= 11., a = 22.989770*g/mole);
    elMg = new G4Element(name="Magnesium", symbol="Mg", z= 12., a = 24.305*g/mole);
    elAl = new G4Element(name="Aluminium", symbol="Al", z= 13., a = 26.981538*g/mole);
    elSi = new G4Element(name="Silicon",   symbol="Si", z= 14., a = 28.0855*g/mole);
    elP  = new G4Element(name="Phosphorus",symbol="P",  z= 15., a = 30.973762*g/mole);
    elS  = new G4Element(name="Sulfur",    symbol="S",  z= 16., a = 32.065*g/mole);
    elK  = new G4Element(name="Potassium", symbol="K",  z= 19., a = 39.0983*g/mole);
    elCa = new G4Element(name="Calcium",   symbol="Ca", z= 20., a = 40.078*g/mole);
    elTi = new G4Element(name="Titanium",  symbol="Ti", z= 22., a = 47.867*g/mole);
    elV  = new G4Element(name="Vanadium",  symbol="V",  z= 23., a = 50.9415*g/mole);
    elCr = new G4Element(name="Chromium",  symbol="Cr", z= 24., a = 51.9961*g/mole);
    elMn = new G4Element(name="Manganese", symbol="Mn", z= 25., a = 54.938044*g/mole);
    elFe = new G4Element(name="Iron",      symbol="Fe", z= 26., a = 55.845*g/mole);
    elCo = new G4Element(name="Cobalt",    symbol="Co", z= 27., a = 58.933195*g/mole);
    elNi = new G4Element(name="Nickel",    symbol="Ni", z= 28., a = 58.69*g/mole);
    elCu = new G4Element(name="Copper",    symbol="Cu", z= 29., a = 63.546*g/mole);
    elZn = new G4Element(name="Zinc",      symbol="Zn", z= 30., a = 65.409*g/mole);
    elGe = new G4Element(name="Germanium", symbol="Ge", z= 32., a = 72.61*g/mole);
    elBr = new G4Element(name="Bromine",   symbol="Br", z= 35., a = 79.904*g/mole);
    elY  = new G4Element(name="Yttrium",   symbol="Y",  z= 39., a = 88.90585*g/mole);
    elCd = new G4Element(name="Cadmium",   symbol="Cd", z= 48., a = 112.411*g/mole);
    elSn = new G4Element(name="Tin",       symbol="Sn", z= 50., a = 118.710*g/mole);
    elTe = new G4Element(name="Tellurium", symbol="Te", z= 52., a = 127.60*g/mole);
    elI  = new G4Element(name="Iodine",    symbol="I",  z= 53., a = 126.90447*g/mole);
    elCs = new G4Element(name="Cesium",    symbol="Cs", z= 55., a = 132.90545*g/mole);
    elLa = new G4Element(name="Lanthanum", symbol="La", z= 57., a = 138.9055*g/mole);
    elCe = new G4Element(name="Cesium",    symbol="Ce", z= 58., a = 140.116*g/mole);
    elLu = new G4Element(name="Lutecium",  symbol="Lu", z= 71., a = 174.967*g/mole);
    elTa = new G4Element(name="Tantalum",  symbol="Ta", z= 73., a = 180.9479*g/mole);
    elW  = new G4Element(name="Tungsten",  symbol="W",  z= 74., a = 183.84*g/mole);
    elAu = new G4Element(name="Gold",      symbol="Au", z= 79., a = 196.96655*g/mole);
    elHg = new G4Element(name="Mercury",   symbol="Hg", z= 80., a = 200.59*g/mole);
    elTl = new G4Element(name="Thallium",  symbol="Tl", z= 81., a = 204.3833*g/mole);
    elPb = new G4Element(name="Lead",      symbol="Pb", z= 82., a = 207.2*g/mole);
    elBi = new G4Element(name="Bismuth",   symbol="Bi", z= 83., a = 208.98038*g/mole);


    density = 2.699*g/cm3;
    G4Material* Al = new G4Material(name="Aluminum", density, ncomponents=1);
    Al->AddElement(elAl, fractionmass=1.);
    AlMaterial = Al;

    //C
    density = 2.0*g/cm3;
    G4Material* C = new G4Material(name="Carbon", density, ncomponents=1);
    C->AddElement(elC, fractionmass=1.);
    CMaterial = C;

    //Pb
    density = 11.35*g/cm3;
    G4Material* Pb = new G4Material(name="Lead", density, ncomponents=1);
    Pb->AddElement(elPb, fractionmass=1.);
    PbMaterial = Pb;

    density = 7.31*g/cm3;
    G4Material* Sn = new G4Material(name="Tin", density, ncomponents=1);
    Sn->AddElement(elSn, fractionmass=1.);
    SnMaterial = Sn;

    density = 16.654*g/cm3;
    G4Material* Ta = new G4Material(name="Tantalum", density, ncomponents=1);
    Ta->AddElement(elTa, fractionmass=1.);
    TaMaterial = Ta;

    density = 19.3*g/cm3;
    G4Material* W = new G4Material(name="Tungsten", density, ncomponents=1);
    W->AddElement(elW, fractionmass=1.);
    WMaterial = W;

    density = 8.908*g/cm3;
    G4Material* Ni = new G4Material(name="Nickel", density, ncomponents=1);
    Ni->AddElement(elNi, fractionmass=1.);
    NiMaterial = Ni;
    
    density = 8.96*g/cm3;
    G4Material* Cu = new G4Material(name="Copper", density, ncomponents=1);
    Cu->AddElement(elCu, fractionmass=1.);
    CuMaterial = Cu;

    //Materials
    density = 1.032*g/cm3;
    plastic_scintillator = new G4Material(name="plastic scintillator", density, ncomponents=2);
    plastic_scintillator->AddElement(elC, natoms=9);
    plastic_scintillator->AddElement(elH, natoms=10);

    density = 1.06000*g/cm3;
    polystyrene = new G4Material(name="polystyrene", density, ncomponents=2);
    polystyrene->AddElement(elC,   0.922582);
    polystyrene->AddElement(elH,  0.077418);

    density = 1.032*g/cm3;
    BC404 = new G4Material(name="BC404", density, ncomponents=2);
    BC404->AddElement(elC,  natoms= 474);
    BC404->AddElement(elH,  natoms= 521);

    const G4int plastic_scintillator_NUMENTRIES = 6;
    G4double plastic_scintillator_ENERGY[plastic_scintillator_NUMENTRIES] = {2.6953095*eV, 2.8178236*eV, 2.9520057*eV, 3.0388294*eV, 3.099606*eV, 3.2627431*eV};
    G4double plastic_scintillator_SCINT[plastic_scintillator_NUMENTRIES] = { 0.1, 0.3, 0.5, 1.0, 0.9, 0.1};
    G4double plastic_scintillator_RIND[plastic_scintillator_NUMENTRIES]  = { 1.58 , 1.58, 1.58, 1.58 , 1.58, 1.58 };
    G4double plastic_scintillator_ABSL[plastic_scintillator_NUMENTRIES]  = { 160.*cm, 160.*cm, 160.*cm, 160.*cm, 160.*cm, 160.*cm};

    plastic_scintillator_MPT = new G4MaterialPropertiesTable();
    plastic_scintillator_MPT->AddProperty("FASTCOMPONENT", plastic_scintillator_ENERGY, plastic_scintillator_SCINT, plastic_scintillator_NUMENTRIES);
    plastic_scintillator_MPT->AddProperty("SLOWCOMPONENT", plastic_scintillator_ENERGY, plastic_scintillator_SCINT, plastic_scintillator_NUMENTRIES);
    plastic_scintillator_MPT->AddConstProperty("SCINTILLATIONYIELD", 14000./MeV);
    plastic_scintillator_MPT->AddConstProperty("RESOLUTIONSCALE", 1.0);
    plastic_scintillator_MPT->AddProperty("ABSLENGTH", plastic_scintillator_ENERGY, plastic_scintillator_ABSL, plastic_scintillator_NUMENTRIES);
    plastic_scintillator_MPT->AddProperty("RINDEX", plastic_scintillator_ENERGY, plastic_scintillator_RIND, plastic_scintillator_NUMENTRIES);
    plastic_scintillator_MPT->AddConstProperty("FASTTIMECONSTANT", 1.*ns);
    plastic_scintillator_MPT->AddConstProperty("SLOWTIMECONSTANT", 1.*ns);
    plastic_scintillator_MPT->AddConstProperty("YIELDRATIO", 1.0);
    // plasticScintillator->SetMaterialPropertiesTable(plastic_scintillator_MPT);

    density = 2.230000*g/cm3;
    pyrex_glass = new G4Material(name="pyrex glass", density, ncomponents=6);
    pyrex_glass->AddElement(elB,   4.0610*perCent);
    pyrex_glass->AddElement(elO,  53.9564*perCent);
    pyrex_glass->AddElement(elNa,  2.8191*perCent);
    pyrex_glass->AddElement(elAl,  1.1644*perCent);
    pyrex_glass->AddElement(elSi, 37.7220*perCent);
    pyrex_glass->AddElement(elK,   0.3321*perCent);
    
    density = 2.40000*g/cm3;
    plate_glass = new G4Material(name="plate glass", density, ncomponents=4);
    plate_glass->AddElement(elO,   fractionmass=0.459800);
    plate_glass->AddElement(elNa,  fractionmass=0.096442);
    plate_glass->AddElement(elSi,  fractionmass=0.336553);
    plate_glass->AddElement(elCa,  fractionmass=0.107205);

    density = 4.51*g/cm3;
    CsI = new G4Material(name="cesium iodide", density, ncomponents=3);
    CsI->AddElement(elCs, natoms= 99);
    CsI->AddElement(elI,  natoms=100);
    CsI->AddElement(elTl, natoms=  1);

    density= 5.81*g/cm3;
    CdZnTe = new G4Material(name="CdZnTe", density, ncomponents=3);
    CdZnTe->AddElement(elCd, fractionmass=0.45);
    CdZnTe->AddElement(elZn, fractionmass=0.05);
    CdZnTe->AddElement(elTe, fractionmass=0.50);


    density= 6.2*g/cm3;
    CdTe = new G4Material(name="CdTe", density, ncomponents=2);
    CdTe->AddElement(elCd, fractionmass=0.468355);
    CdTe->AddElement(elTe, fractionmass=0.531645);

    /*density= 7.13*g/cm3;
    BGO = new G4Material(name="BGO", density, ncomponents=3);
    BGO->AddElement(elO,  fractionmass=0.154126);
    BGO->AddElement(elGe, fractionmass=0.17482);
    BGO->AddElement(elBi, fractionmass=0.671054);
    */

    G4NistManager* man = G4NistManager::Instance();
    BGO = man->FindOrBuildMaterial("G4_BGO");
    //G4cout << BGO;

    density = 3.67*g/cm3;
    NaI = new G4Material(name="sodium iodide", density, ncomponents=3);
    NaI->AddElement(elNa, natoms= 99);
    NaI->AddElement(elI,  natoms=100);
    NaI->AddElement(elTl, natoms=  1);

    density = 5.07*g/cm3;
    LaBr3 = new G4Material(name="lanthanum bromide", density, ncomponents=3);
    // source : F. Quaratia et al, Nucl. Inst. and Methods A 574 (2007) 115
    LaBr3->AddElement(elLa, natoms= 95);
    LaBr3->AddElement(elBr, natoms=300);
    LaBr3->AddElement(elCe, natoms=  5);

    density = 7.1*g/cm3;
    LYSO = new G4Material(name="LYSO", density, ncomponents=5);
    LYSO->AddElement(elLu, natoms=178);
    LYSO->AddElement(elY,  natoms= 20);
    LYSO->AddElement(elSi, natoms=100);
    LYSO->AddElement(elO,  natoms=500);
    LYSO->AddElement(elCe, natoms=  2);

    density= 5.81*g/cm3;
    CZT = new G4Material(name="CZT", density, ncomponents=3);
    CZT->AddElement(elCd, fractionmass=0.45);
    CZT->AddElement(elZn, fractionmass=0.05);
    CZT->AddElement(elTe, fractionmass=0.50);

    density = 1.290*mg/cm3;
    air = new G4Material(name="air", density, ncomponents=2);
    air->AddElement(elN, fractionmass=0.7);
    air->AddElement(elO, fractionmass=0.3);

    sea_air = manNist->FindOrBuildMaterial("G4_AIR");
    
    density = 2.700*g/cm3;
    aluminium_metal = new G4Material(name="aluminium metal", density, ncomponents=1);
    aluminium_metal->AddElement(elAl, fractionmass=1.);

    density = 2.267*g/cm3;
    graphite = new G4Material(name="graphite", density, ncomponents=1);
    graphite->AddElement(elC, fractionmass=1.);

    density = 7.874*g/cm3;
    iron_metal = new G4Material(name="iron metal", density, ncomponents=1);
    iron_metal->AddElement(elFe, fractionmass=1.);

    density = 11.340*g/cm3;
    lead_metal = new G4Material(name="lead metal", density, ncomponents=1);
    lead_metal->AddElement(elPb, fractionmass=1.);

    density = 2.330*g/cm3;
    silicon_metal = new G4Material(name="silicon metal", density, ncomponents=1);
    silicon_metal->AddElement(elSi, fractionmass=1.);

    density = 7.310*g/cm3;
    tin_metal = new G4Material(name="tin metal", density, ncomponents=1);
    tin_metal->AddElement(elSn, fractionmass=1.);

    density = 8.920*g/cm3;
    copper_metal = new G4Material(name="copper metal", density, ncomponents=1);
    copper_metal->AddElement(elCu, fractionmass=1.);

    density = 16.6950*g/cm3;
    tantalum_metal = new G4Material(name="tantalum metal", density, ncomponents=1);
    tantalum_metal->AddElement(elTa, fractionmass=1.);

    density = 19.250*g/cm3;
    tungsten_metal = new G4Material(name="tungsten metal", density, ncomponents=1);
    tungsten_metal->AddElement( elW, fractionmass=1.);

    density = 19.3*g/cm3;
    gold_metal = new G4Material(name="gold metal", density, ncomponents=1);
    gold_metal->AddElement(elAu, fractionmass=1.);

    density     = universe_mean_density;
    pressure    = 3.e-18*pascal;
    temperature = 2.73*kelvin;
    vacuum = new G4Material(name="galactic vacuum", z=1., a=1.01*g/mole, density, kStateGas, temperature, pressure);

    density = 0.4711*g/cm3;
    light_aluminium_metal = new G4Material(name="light aluminium metal", density, ncomponents=1);
    light_aluminium_metal->AddElement(elAl, fractionmass=1.);

    density = 2.2985*g/cm3;
    Caliste_composite = new G4Material(name="Caliste composite", density, ncomponents=2);
    Caliste_composite->AddElement(elC, fractionmass=0.5);
    Caliste_composite->AddElement(elSi, fractionmass=0.5);

    density = 1.46*g/cm3; // Carbon fiber from Agile, via G400, but T300 density = 1.76 g/cm3
    CarbonFiber = new G4Material(name="Carbon fiber", density, ncomponents=3);
    CarbonFiber->AddElement(elC, fractionmass = 0.7195);
    CarbonFiber->AddElement(elH, fractionmass = 0.2439);
    CarbonFiber->AddElement(elO, fractionmass = 0.0366);

    /*
    density = 1.42*g/cm3; //Kapton from Agile, via G400
    Kapton = new G4Material(name = "Kapton material", density, ncomponents = 4);
    Kapton->AddElement(elC, fractionmass = 0.5641);
    Kapton->AddElement(elH, fractionmass = 0.2564);
    Kapton->AddElement(elO, fractionmass = 0.1282);
    Kapton->AddElement(elN, fractionmass = 0.0513);
    */

    Kapton  = manNist->FindOrBuildMaterial("G4_KAPTON");

    // Aluminium Honeycomb
    //density= 0.04*g/cm3;
    //density= 0.0912*g/cm3;
    density= 0.032*g/cm3;
    G4cout << density << G4endl;
    AlHoneycomb = new G4Material("Aluminum honeycomb", density, ncomponents=1);
    AlHoneycomb->AddElement(elAl, fractionmass=1.);

    // Aluminium Honeycomb density from .conf file

    gm.config->readInto(HoneyDensity, "GEOM.ST.TRAY.ALHONEY.DENSITY");
    G4cout << "GEOM.ST.TRAY.ALHONEY.DENSITY: " << HoneyDensity << G4endl;

    // Aluminium Honeycomb modified for Fermi
    //density= 0.04*g/cm3;
    //density= 0.0777619*g/cm3;
    density = HoneyDensity*g/cm3;
    AlHoneycomb_modified = new G4Material("Aluminum honeycomb modified", density, ncomponents=1);
    AlHoneycomb_modified->AddElement(elAl, fractionmass=1.);

    /*
    AlHoneycomb_Fermi_32 = new G4Material("Aluminum honeycomb Fermi 32 mm", density, ncomponents=1);
           AlHoneycomb_Fermi_32->AddElement(elAl, fractionmass=1.);

    density= 0.0711808*g/cm3;
           AlHoneycomb_Fermi_35 = new G4Material("Aluminum honeycomb Fermi 35 mm", density, ncomponents=1);
           AlHoneycomb_Fermi_35->AddElement(elAl, fractionmass=1.);

           // Aluminium Honeycomb modified for G400
    density= 0.0780505*g/cm3;
           AlHoneycomb_G400_32 = new G4Material("Aluminum honeycomb G400 32 mm", density, ncomponents=1);
           AlHoneycomb_G400_32->AddElement(elAl, fractionmass=1.);

    density= 0.0707550*g/cm3;
           AlHoneycomb_G400_35 = new G4Material("Aluminum honeycomb G400 35 mm", density, ncomponents=1);
           AlHoneycomb_G400_35->AddElement(elAl, fractionmass=1.);
           */


    // Glue (from ARGO)
    density= 1.2*g/cm3;
    Glue = new G4Material("Glue material", density, ncomponents=2);
    Glue->AddElement(elC,natoms=1);
    Glue->AddElement(elH,natoms=1);

    density = 0.272109*g/cm3;
    GL_Alpayload = new G4Material("GL Al equivalent", density, ncomponents=1);
    GL_Alpayload->AddElement(elAl, fractionmass=1.);
    
    density = 1.8*g/cm3;
    CircuitBoard = new G4Material(name="CircuitBoard", density, ncomponents=9);
    CircuitBoard->AddElement(elH,    fractionmass = 0.030);
    CircuitBoard->AddElement(elC,    fractionmass = 0.174);
    CircuitBoard->AddElement(elO,    fractionmass = 0.392);
    CircuitBoard->AddElement(elAl,   fractionmass = 0.100);
    CircuitBoard->AddElement(elSi,   fractionmass = 0.244);
    CircuitBoard->AddElement(elFe,   fractionmass = 0.010);
    CircuitBoard->AddElement(elCu,   fractionmass = 0.030);
    CircuitBoard->AddElement(elSn,   fractionmass = 0.010);
    CircuitBoard->AddElement(elPb,   fractionmass = 0.010);
    
    // material SiO2
    SiO2 = manNist->FindOrBuildMaterial("G4_SILICON_DIOXIDE");
    
    density = 3.44*g/cm3;
    Si3N4 = new G4Material(name="Si3N4", density, ncomponents=2);
    Si3N4->AddElement(elSi, fractionmass=60*perCent);
    Si3N4->AddElement(elN, fractionmass=40*perCent);
    
    // Titanium Alloy (Ti-6Al-4V)
    density = 4.43*g/cm3;
    TiAlloy   = new G4Material(name="TiAlloy", density, ncomponents=3);
    TiAlloy->AddElement(elTi,  fractionmass = 0.90);
    TiAlloy->AddElement(elAl,  fractionmass = 0.06);
    TiAlloy->AddElement(elV,   fractionmass = 0.04);
    
    //Stainless steel AISI 304
    density = 8.03*g/cm3;
    Steel304   = new G4Material(name="Steel304", density, ncomponents=9);
    Steel304->AddElement(elC,  fractionmass = 0.0008);
    Steel304->AddElement(elMn,  fractionmass = 0.0200);
    Steel304->AddElement(elP,   fractionmass = 0.00045);
    Steel304->AddElement(elS,   fractionmass = 0.00030);
    Steel304->AddElement(elSi,   fractionmass = 0.0075);
    Steel304->AddElement(elCr,   fractionmass = 0.190);
    Steel304->AddElement(elNi,   fractionmass = 0.100);
    Steel304->AddElement(elN,   fractionmass = 0.0010);
    Steel304->AddElement(elFe,   fractionmass = 0.67995);

    //G4cout << *(G4Material::GetMaterialTable()) << G4endl;
    
    // ----------> HITOMI materials

    // Mass relative rate
    G4double HeTankRate = 0.36;
    G4double HeTankHeRate = 0.94;
    G4double JTShieldRate = 1.17;
    G4double MainShellRate = 1.12;
    G4double StrutRate = 2.95;
    G4double OVCS_MLIRate = 0.09;
    
    //Al Alloy A6061
    density = 2.69*g/cm3;
    AlAlloy6061 = new G4Material(name="AlAlloy6061", density, ncomponents=5);
    AlAlloy6061->AddElement(elAl, fractionmass = 0.986);
    AlAlloy6061->AddElement(elMg, fractionmass = 0.008);
    AlAlloy6061->AddElement(elSi, fractionmass = 0.004);
    AlAlloy6061->AddElement(elCu, fractionmass = 0.0015);
    AlAlloy6061->AddElement(elCr, fractionmass = 0.0005);

    //Al Alloy A6061 for HITOMI/SXS He Tank
    density = (2.69*HeTankRate)*g/cm3;
    AlAlloy6061HeTank = new G4Material(name="AlAlloy6061HeTank", density, ncomponents=5);
    AlAlloy6061HeTank->AddElement(elAl, fractionmass = 0.986);
    AlAlloy6061HeTank->AddElement(elMg, fractionmass = 0.008);
    AlAlloy6061HeTank->AddElement(elSi, fractionmass = 0.004);
    AlAlloy6061HeTank->AddElement(elCu, fractionmass = 0.0015);
    AlAlloy6061HeTank->AddElement(elCr, fractionmass = 0.0005);

    //Al Alloy A6061 for HITOMI/SXS JTShield
    density = (2.69*JTShieldRate)*g/cm3;
    AlAlloy6061JTShield = new G4Material(name="AlAlloy6061JTShield", density, ncomponents=5);
    AlAlloy6061JTShield->AddElement(elAl, fractionmass = 0.986);
    AlAlloy6061JTShield->AddElement(elMg, fractionmass = 0.008);
    AlAlloy6061JTShield->AddElement(elSi, fractionmass = 0.004);
    AlAlloy6061JTShield->AddElement(elCu, fractionmass = 0.0015);
    AlAlloy6061JTShield->AddElement(elCr, fractionmass = 0.0005);

    //Al Alloy A6061 for HITOMI/SXS Mainshell
    density = (2.69*MainShellRate)*g/cm3;
    AlAlloy6061Mainshell = new G4Material(name="AlAlloy6061Mainshell", density, ncomponents=5);
    AlAlloy6061Mainshell->AddElement(elAl, fractionmass = 0.986);
    AlAlloy6061Mainshell->AddElement(elMg, fractionmass = 0.008);
    AlAlloy6061Mainshell->AddElement(elSi, fractionmass = 0.004);
    AlAlloy6061Mainshell->AddElement(elCu, fractionmass = 0.0015);
    AlAlloy6061Mainshell->AddElement(elCr, fractionmass = 0.0005);
    
    //LiquidHelium
    density = (0.144*HeTankHeRate)*g/cm3;
    LiquidHelium = new G4Material(name="LiquidHelium", density, ncomponents=1);
    LiquidHelium->AddElement(elHe, fractionmass = 1.);
 
 
    //Charcoal
    density = (1.0*OVCS_MLIRate) * g/cm3; // undefined
    Charcoal = new G4Material(name="Charcoal", density, ncomponents=1);
    Charcoal->AddElement(elC, fractionmass = 1.);


    //HgTe
    density = 8.17 * g/cm3;
    HgTe = new G4Material(name="HgTe", density, ncomponents=2);
    HgTe->AddElement(elHg,natoms=1);
    HgTe->AddElement(elTe,natoms=1);
    
    //Alumina
    density = 3.97 * g/cm3;
    Alumina = new G4Material(name="Alumina", density, ncomponents=2);
    Alumina->AddElement(elAl,natoms=2);
    Alumina->AddElement(elO,natoms=3);
    
    //CFRP
    density = (2.0*StrutRate) * g/cm3;
    CFRP = new G4Material(name="CFRP", density, ncomponents=3);
    CFRP->AddElement(elC, fractionmass = 89.5 * perCent);
    CFRP->AddElement(elH, fractionmass = 3.1 * perCent);
    CFRP->AddElement(elO, fractionmass = 7.4 * perCent);
    
    //SUS304
    density = 7.9 * g/cm3;
    SUS304 = new G4Material(name="SUS304", density, ncomponents=5);
    SUS304->AddElement(elFe, fractionmass = 70.5 * perCent);
    SUS304->AddElement(elCr, fractionmass = 19.0 * perCent);
    SUS304->AddElement(elNi, fractionmass = 9.0 * perCent);
    SUS304->AddElement(elMn, fractionmass = 1.0 * perCent);
    SUS304->AddElement(elSi, fractionmass = 0.5 * perCent);
    
    //VanadiumPermender
    density = 7.9 * g/cm3;  // uncertain
    VanadiumPermender = new G4Material(name="VanadiumPermender", density, ncomponents=3);
    VanadiumPermender->AddElement(elFe, fractionmass = 49. * perCent);
    VanadiumPermender->AddElement(elCo, fractionmass = 49. * perCent);
    VanadiumPermender->AddElement(elV, fractionmass = 2. * perCent);

    //Aluminium oxide
    density = 3.987 * g/cm3;
    al_oxide = new G4Material(name="al_oxide", density, ncomponents=2);
    al_oxide->AddElement(elAl, natoms=2);
    al_oxide->AddElement(elO, natoms=3);
    
    //Carbon-filled epoxy Stycast 2850
    density = 2.4 * g/cm3;
    Stycast2850 = new G4Material(name="Stycast2850", density, ncomponents=3);
    Stycast2850->AddMaterial(al_oxide, fractionmass = 76. * perCent);
    Stycast2850->AddMaterial(Glue, fractionmass = 16. * perCent);
    Stycast2850->AddElement(elC, fractionmass = 8. * perCent);
    
    //Titanium foil 15-3-3-3
    density = 4.76 * g/cm3;
    TiAlloy15333 = new G4Material(name="TiAlloy15333", density, ncomponents=5);
    TiAlloy15333->AddElement(elTi, fractionmass = 75. * perCent);
    TiAlloy15333->AddElement(elV, fractionmass = 15. * perCent);
    TiAlloy15333->AddElement(elCr, fractionmass = 3.5 * perCent);
    TiAlloy15333->AddElement(elAl, fractionmass = 3.5 * perCent);
    TiAlloy15333->AddElement(elSn, fractionmass = 3. * perCent);
    
    //ABS
    density = 1.07 * g/cm3;
    ABS = new G4Material(name="ABS", density, ncomponents=3);
    ABS->AddElement(elC,natoms=15);
    ABS->AddElement(elH,natoms=17);
    ABS->AddElement(elN,natoms=1);
    
    // -------------------------------------------------------------------------
   
    // POLYETHYLENE
    polyeth = manNist->FindOrBuildMaterial("G4_POLYETHYLENE");
    
    
}

G4Material* MaterialsDefinition::GetMaterial(int index) {
    G4Material* dummat = 0;
    switch(index) {
    case 1:
        dummat = AlMaterial;
        break;
    case 2:
        dummat = CMaterial;
        break;
    case 3:
        dummat = PbMaterial;
        break;
    case 4:
        dummat = SnMaterial;
        break;
    case 5:
        dummat = CuMaterial;
        break;
    case 6:
        dummat = TaMaterial;
        break;
    case 7:
        dummat = WMaterial;
        break;
    case 8:
        dummat = iron_metal;
        break;
    case 9:
        dummat = silicon_metal;
        break;
    case 10:
        dummat = gold_metal;
        break;
    case 11:
        dummat = air;
        break;
    case 12:
        dummat = vacuum;
        break;
    case 13:
        dummat = plastic_scintillator;
        break;
    case 14:
        dummat = CsI;
        break;
    case 15:
        dummat = CdZnTe;
        break;
    case 16:
        dummat = NaI;
        break;
    case 17:
        dummat = LaBr3;
        break;
    case 18:
        dummat = CdTe;
        break;
    case 19:
        dummat = polystyrene;
        break;
    case 20:
        dummat = light_aluminium_metal;
        break;
    case 21:
        dummat = BGO;
        break;
    case 22:
        dummat = CarbonFiber;
        break;
    case 23:
        dummat = Kapton;
        break;
    case 24:
        dummat = AlHoneycomb;
        break;
    case 25:
        dummat = Glue;
        break;
    case 26:
        dummat = BC404;
        break;
    case 27:
        dummat = GL_Alpayload;
        break;
    case 28:
        dummat = AlHoneycomb_modified;
        break;
    case 29:
        dummat = CircuitBoard;
        break;
    case 30:
        dummat = NiMaterial;
        break;
    case 31:
        dummat = SiO2;
        break;
    case 32:
        dummat = Si3N4;
        break;
    case 33:
        dummat = TiAlloy;
        break;
    case 34:
        dummat = Steel304;
        break;
    case 35:
        dummat = AlAlloy6061;
        break;
    case 36:
        dummat = LiquidHelium;
        break;
    case 37:
        dummat = Charcoal;
        break;
    case 38:
        dummat = HgTe;
        break;
    case 39:
        dummat = Alumina;
        break;
    case 40:
        dummat = CFRP;
        break;
    case 41:
        dummat = SUS304;
        break;
    case 42:
        dummat = VanadiumPermender;
        break;
    case 43:
        dummat = CuMaterial;
        break;
    case 44:
        dummat = polyeth;
        break;
    case 45:
        dummat = sea_air;
        break;
    case 46:
        dummat = Stycast2850;
        break;
    case 47:
        dummat = TiAlloy15333;
        break;
    case 48:
        dummat = AlAlloy6061HeTank;
        break;
    case 49:
        dummat = AlAlloy6061JTShield;
        break;
    case 50:
        dummat = AlAlloy6061Mainshell;
        break;
    case 51:
        dummat = ABS;
        break;
    case 52:
        dummat = plate_glass;
        break;
    }
    return dummat;
}
/*
G4Material* MaterialsDefinition::GetDensity(int index) {
	G4double dumden = 0;
	switch(index) {
		case 24:
			dumden = 0.0912*g/cm3;
			break;
		case 28:
			dumden = 0.0777832*g/cm3;
			break;
	}
	return dumden;
}
*/

void MaterialsDefinition::DestroyMaterials() {
    // Destroy all allocated elements and materials
    size_t i;
    G4MaterialTable* matTable = (G4MaterialTable*)G4Material::GetMaterialTable();
    for(i=0; i<matTable->size(); i++ ) {
        delete   (*(matTable))[i];
    }
    matTable->clear();
    G4ElementTable* elemTable = (G4ElementTable*)G4Element::GetElementTable();
    for(i=0; i<elemTable->size(); i++ ) {
        delete   (*(elemTable))[i];
    }
    elemTable->clear();
}
