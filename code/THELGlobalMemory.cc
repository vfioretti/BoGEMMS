/***************************************************************************
                          THELGlobalMemory.cc  -  description
                             -------------------
    begin                : 2007
    copyright            : (C) 2007 by  Andrea Bulgarelli
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

#include "THELGlobalMemory.hh"
#include <fstream>
#include <iomanip>
#include <stdio.h>
#include "G4LogicalVolume.hh"
#include <G4VPhysicalVolume.hh>
#include "G4ThreeVector.hh"
#include <G4VSolid.hh>
#include "G4PVPlacement.hh"
#include "G4VUserDetectorConstruction.hh"
#include "G4PVReplica.hh"
#include "G4SDManager.hh"
#include "CSensitiveDetector.hh"
#include "G4Element.hh"
#include "G4Material.hh"
#include "G4PVPlacement.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4ThreeVector.hh"
#include "G4VisAttributes.hh"
#include "G4SubtractionSolid.hh"
#include "G4Region.hh"
#include "G4RegionStore.hh"
#include "globals.hh"
#include "FITSOutput_INOUT.hh"
#include "FITSOutput_Step.hh"
#include "FITSOutput_EnergyVol.hh"
#include "FITSOutput_EnergyVolEnd.hh"
#include "RootOutput_EnergyVol.hh"
#include "RootOutput_EnergyVolEnd.hh"
#include "RootOutput_XYZ.hh"
#include "SQLliteOutput_XYZ.hh"
#include "G4VUserDetectorConstruction.hh"

// VF
#include "FITSOutput_XYZ.hh"
#include "CXYZSD.hh"

#if defined(GEANT4_10_1) || defined(GEANT4_10_2) || defined(GEANT4_10_3) || defined(GEANT4_10_4)
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"
#endif


#include <fitsio.h>

using namespace std;

THELGlobalMemory gm;

THELGlobalMemory::THELGlobalMemory() {
    //scrivi il file IN/OUT
    enableWriteINOUT = true;

    //scrivi il file XYZ - VF
    enableWriteXYZ = true;

    //se true, registra anche i sensitive detector e crea le hit
    enableWriteSTEP = false;

    //scrivi i file di output in formato FITS
    enableWriteFITS = false;

    enableWriteRoot = true;

    enableWriteRootEnd = false;

    enableWriteFITSEnd = false;

    enableWriteXrootd = false;

    enableWriteEnergyVol = false;

    //scrivi i file di output in formato TXT
    enableWriteTXT = false;

    enableLowEnergyPhysic = true;

    enableHighEnergyPhysic = true;

    nfileFITSINOUT = 0;

    // VF
    nfileFITSXYZ = 0;

    nfileFITSHit = 0;

    compressionlevel = 9;

    nfileFITSENERGYVOL = 0;

    DOWNLOADGEOMETRY = false;

    detector_volmin = -1;
    detector_volmax = -1;

// 	G4cout << "THELGlobalMemory::THELGlobalMemory()" << G4endl;
}


THELGlobalMemory::~THELGlobalMemory() {
    //cout << "CLOSE"<< endl;
    /*if(enableWriteFITS && enableWriteINOUT) fits_inout->close();
    if(enableWriteFITS && enableWriteSTEP) fits_step->close();
    if(enableWriteFITS && enableWriteEnergyVol) fits_energyvol->close();
    if(enableWriteRoot && enableWriteEnergyVol) {
    		root_energyvol->close();

    }
    if(enableWriteRootEnd  && enableWriteEnergyVol) {

    	root_energyvolend->close();
    }
    if(enableWriteFITSEnd && enableWriteEnergyVol) {

    	fits_energyvolend->close();
    }*/
}

void THELGlobalMemory::OpenConfigFile() {
    config = new ConfigFile(gm.configFileName);
    config->readInto(enableWriteFITS, "IO.FILETYPE.FITS");
    config->readInto(enableWriteFITSEnd, "IO.FILETYPE.FITSEND");
    config->readInto(enableWriteRoot, "IO.FILETYPE.ROOT");
    config->readInto(enableWriteRootEnd, "IO.FILETYPE.ROOTEND");
    config->readInto(enableWriteXrootd, "IO.FILETYPE.XROOTD");
    config->readInto(enableWriteSQlite3, "IO.FILETYPE.SQLITE3");
    cout << "IO.FILETYPE.FITS " << enableWriteFITS << endl;
    cout << "IO.FILETYPE.FITSEND " << enableWriteFITSEnd << endl;
    cout << "IO.FILETYPE.ROOT " << enableWriteRoot << endl;
    cout << "IO.FILETYPE.ROOTEND " << enableWriteRootEnd << endl;
    cout << "IO.FILETYPE.XROOTD " << enableWriteXrootd << endl;
    cout << "IO.FILETYPE.SQLITE3 " << enableWriteSQlite3 << endl;
    config->readInto(enableWriteINOUT, "IO.FILE.INOUT");
    // VF
    config->readInto(enableWriteXYZ, "IO.FILE.XYZ");
    config->readInto(enableWriteSTEP, "IO.FILE.STEP");
    config->readInto(enableWriteEnergyVol, "IO.FILE.ENERGYVOL");
    cout << "IO.FILE.INOUT " << enableWriteINOUT << endl;
    // VF
    cout << "IO.FILE.XYZ " << enableWriteXYZ << endl;
    cout << "IO.FILE.STEP " << enableWriteSTEP << endl;
    cout << "IO.FILE.ENERGYVOL " << enableWriteEnergyVol << endl;
    config->readInto(geomVersion, "GEOM.VERSION");
    config->readInto(physVersion, "ENERGYPROCESS.VERSION");
    cout << "GEOM.VERSION " << geomVersion << endl;
    cout << "ENERGYPROCESS.VERSION " << physVersion << endl;

    if(enableWriteXrootd) {
        config->readInto(xrootdcon, "XROOTD.con");
        cout << "XROOTD.con " << xrootdcon << endl;
    }
}

void THELGlobalMemory::InitFiles() {
    if(enableWriteFITS && enableWriteINOUT) {
        fits_inout = new FITSOutput_INOUT();
        fits_inout->init();
    }
    // VF
    if(enableWriteFITS && enableWriteXYZ) {
        fits_xyz = new FITSOutput_XYZ();
        fits_xyz->init();
    }
    if(enableWriteFITS && enableWriteSTEP) {
        fits_step = new FITSOutput_Step();
        fits_step->init();
    }
    if(enableWriteFITS && enableWriteEnergyVol) {
        fits_energyvol = new FITSOutput_EnergyVol();
        fits_energyvol->init();
    }
	
#ifndef WITHOUTROOT

    if(enableWriteRoot && enableWriteEnergyVol) {
        root_energyvol = new RootOutput_EnergyVol();
        root_energyvol->init();
    }
    if(enableWriteRootEnd   && enableWriteEnergyVol) {

        root_energyvolend = new RootOutput_EnergyVolEnd();
        root_energyvolend->init();

    }
	
	//AB
	if(enableWriteRoot && enableWriteXYZ) {
		root_xyz = new RootOutput_XYZ();
		root_xyz->init();
	}

	
#endif

#ifdef SQLITE3
	if(enableWriteSQlite3 && enableWriteXYZ) {
		sqlite3_xyz = new SQLliteOutput_XYZ();
		sqlite3_xyz->init();
	}
#endif
	
    if(enableWriteFITSEnd && enableWriteEnergyVol) {

        fits_energyvolend = new FITSOutput_EnergyVolEnd();
        fits_energyvolend->init();

    }
}

G4String THELGlobalMemory::GetOutputStepBoundaryFileNameTXT() {
    char fn[255];
    sprintf(fn, "inout.%ld.out", nfileFITSINOUT);
    std::ofstream asciiFile(fn, std::ios::app);
    if(asciiFile.is_open()) {
        long pos;
        pos=asciiFile.tellp();
        if(pos > 1700000000)
            nfileFITSINOUT++;
    }
    sprintf(fn, "inout.%ld.out", nfileFITSINOUT);
    outputStepBoundaryFileNameTXT = fn;
    return outputStepBoundaryFileNameTXT;
}

// VF
G4String THELGlobalMemory::GetOutputXYZFileNameTXT() {
    char fn[255];
    sprintf(fn, "xyz.%ld.out", nfileFITSXYZ);
    std::ofstream asciiFile(fn, std::ios::app);
    if(asciiFile.is_open()) {
        long pos;
        pos=asciiFile.tellp();
        if(pos > 1700000000)
            nfileFITSXYZ++;
    }
    sprintf(fn, "xyz.%ld.out", nfileFITSXYZ);
    outputXYZFileNameTXT = fn;
    return outputXYZFileNameTXT;
}

G4String THELGlobalMemory::GetOutputHitFileNameTXT() {
    char fn[255];
    sprintf(fn, "step.%ld.out", nfileFITSHit);
    std::ofstream asciiFile(fn, std::ios::app);
    if(asciiFile.is_open()) {
        long pos;
        pos=asciiFile.tellp();
        if(pos > 1700000000)
            nfileFITSHit++;
    }
    sprintf(fn, "step.%ld.out", nfileFITSHit);
    outputHitFileNameTXT = fn;
    return outputHitFileNameTXT;
}

G4String THELGlobalMemory::GetOutputStepBoundaryFileNameFITS() {
    char fn[255];
    char fn2[255];
    G4String name;
    name = "";
    gm.config->readInto(name, "TEMPFITSOUTPUT.PREFIX");
    sprintf(fn, "%sinout.%ld.fits", name.data(), nfileFITSINOUT);
    sprintf(fn2, "inout.%ld.fits", nfileFITSINOUT);
    // 	std::ofstream asciiFile(fn, std::ios::app);
    // 	if(asciiFile.is_open()) {
    // 		long pos;
    // 		pos=asciiFile.tellp();
    // 		if(pos > 1700000000)
    // 			nfile++;
    // 	}
// 	sprintf(fn, "sx.step.boundary.%d.fits", nfileFITSINOUT);
    outputStepBoundaryFileNameFITS = fn;
    outputStepBoundaryFileNameFITS_base = fn2;
    nfileFITSINOUT++;
    return outputStepBoundaryFileNameFITS;
}

//VF
G4String THELGlobalMemory::GetOutputXYZFileNameFITS() {
    char fn[255];
    char fn2[255];
    G4String name;
    name = "";
    gm.config->readInto(name, "TEMPFITSOUTPUT.PREFIX");
    sprintf(fn, "%sxyz.%ld.fits", name.data(), nfileFITSXYZ);
    sprintf(fn2, "xyz.%ld.fits", nfileFITSXYZ);
    // 	std::ofstream asciiFile(fn, std::ios::app);
    // 	if(asciiFile.is_open()) {
    // 		long pos;
    // 		pos=asciiFile.tellp();
    // 		if(pos > 1700000000)
    // 			nfile++;
    // 	}
// 	sprintf(fn, "sx.step.boundary.%d.fits", nfileFITSINOUT);
    outputXYZFileNameFITS = fn;
    outputXYZFileNameFITS_base = fn2;
    nfileFITSXYZ++;
    return outputXYZFileNameFITS;
}

G4String THELGlobalMemory::GetOutputHitFileNameFITS() {
    char fn[255];
    char fn2[255];
    G4String name;
    name = "";
    gm.config->readInto(name, "TEMPFITSOUTPUT.PREFIX");
    sprintf(fn, "%sstep.%ld.fits", name.data(), nfileFITSHit);
    sprintf(fn2, "step.%ld.fits", nfileFITSHit);
    // 	std::ofstream asciiFile(fn, std::ios::app);
    // 	if(asciiFile.is_open()) {
    // 		long pos;
    // 		pos=asciiFile.tellp();
    // 		if(pos > 1700000000)
    // 			nfile++;
    // 	}
// 	sprintf(fn, "sx.hit.%d.fits", nfileFITSHit);
    outputHitFileNameFITS = fn;
    outputHitFileNameFITS_base = fn2;
    nfileFITSHit++;
    return outputHitFileNameFITS;
}

G4String THELGlobalMemory::GetOutputEnergyVolFileNameFITS() {
    char fn[255];
    char fn2[255];
    G4String name;
    name = "";
    gm.config->readInto(name, "TEMPFITSOUTPUT.PREFIX");
    sprintf(fn, "%senergyvol.%ld.fits", name.data(), nfileFITSENERGYVOL);
    sprintf(fn2, "energyvol.%ld.fits", nfileFITSENERGYVOL);
    // 	std::ofstream asciiFile(fn, std::ios::app);
    // 	if(asciiFile.is_open()) {
    // 		long pos;
    // 		pos=asciiFile.tellp();
    // 		if(pos > 1700000000)
    // 			nfile++;
    // 	}
// 	sprintf(fn, "sx.energy.volume.%d.fits", nfileFITSENERGYVOL);
    outputEnergyVolFileNameFITS = fn;
    outputEnergyVolFileNameFITS_base = fn2;
    nfileFITSENERGYVOL++;
    return outputEnergyVolFileNameFITS;
}

G4String THELGlobalMemory::GetOutputEnergyVolFileNameRoot() {
    char fn[255];
    char fn2[255];
    G4String name;
    name = "";

    if(enableWriteXrootd) {
        name = xrootdcon;
        sprintf(fn2, "%senergyvol.%ld.root", name.data(), nfileRootENERGYVOL);
    } else {
        sprintf(fn2, "energyvol.%ld.root", nfileRootENERGYVOL);
    }
    outputEnergyVolFileNameRoot = fn2;

    nfileRootENERGYVOL++;
    return outputEnergyVolFileNameRoot;
}

G4String THELGlobalMemory::GetOutputXYZFileNameRoot() {
	char fn[255];
	char fn2[255];
	G4String name;
	name = "";
	
	if(enableWriteXrootd) {
		name = xrootdcon;
		sprintf(fn2, "%sxyz.%ld.root", name.data(), nfileRootXYZ);
	} else {
		sprintf(fn2, "xyz.%ld.root", nfileRootXYZ);
	}
	outputXYZFileNameRoot = fn2;
	
	nfileRootXYZ++;
	return outputXYZFileNameRoot;
}


int THELGlobalMemory::CopyFileFits(char* in, char* out) {
    G4String ins;
    ins = in;
    G4String outs;
    outs = out;
    if(ins == outs)
        return 0;
    G4cout << "Copy files from " << ins << " to " << outs << G4endl;
    fitsfile *infptr, *outfptr;   /* FITS file pointers defined in fitsio.h */
    int status = 0;       /* status must always be initialized = 0  */
    /* Open the input file */
    if ( !fits_open_file(&infptr, in, READONLY, &status) )
    {
        /* Create the output file */
        if ( !fits_create_file(&outfptr, out, &status) )
        {

            /* copy the previous, current, and following HDUs */
            fits_copy_file(infptr, outfptr, 1, 1, 1, &status);

            fits_close_file(outfptr,  &status);
        }
        fits_delete_file(infptr, &status);
    }
    //delete temp file
// 	fits_delete_file(infptr, &status);
    /* if error occured, print out error message */
    if (status) fits_report_error(stderr, status);
    return(status);

}


void THELGlobalMemory::AddSensitiveDetector(G4VPhysicalVolume* pv) {
    //se geometria non attiva e se step non attivo, esci
    //altrimenti setta il physical volume come sensitive detector

    if(!gm.DOWNLOADGEOMETRY)
        if((!enableWriteSTEP))
            return;

    G4int verboseLevel = 3;
    // 	G4cout << "CAVLO " << verboseLevel << G4endl;
    if (verboseLevel >= 2 ) {
        G4cout << "##############################################################################" << G4endl;
        G4cout << "Physical volume: " << pv->GetName() <<  " - copy number " << pv->GetCopyNo() <<  G4endl;
        G4cout << " Center: " << pv->GetTranslation() / mm << " mm" << G4endl;
        G4cout << " Mass: " << pv->GetLogicalVolume()->GetMass() / kg << " Kg " << G4endl;
        G4cout <<  pv->GetLogicalVolume()->GetMaterial()  << G4endl;
        pv->GetLogicalVolume()->GetSolid()->DumpInfo();
        G4cout << G4endl;
    }
    G4SDManager* SDman = G4SDManager::GetSDMpointer();
    G4LogicalVolume* logpv = pv->GetLogicalVolume();
    G4String name = logpv->GetName();
    name += "SD";
    CSensitiveDetector* logpvSD = new CSensitiveDetector( name );
    SDman->AddNewDetector( logpvSD );
    logpv->SetSensitiveDetector( logpvSD );
    if(verboseLevel >= 2) {
        G4cout << " Sensitive detector name: " << name << G4endl << G4endl << G4endl;
    }
}
void THELGlobalMemory::AddXYZDetector(G4LogicalVolume* logs) {
    G4SDManager* SDman_XYZ = G4SDManager::GetSDMpointer();
    G4String name_xyz = logs->GetName();
    name_xyz += "SD";
    CXYZSD *vsd = new CXYZSD(name_xyz);
    SDman_XYZ->AddNewDetector(vsd);
    logs->SetSensitiveDetector(vsd);

}

G4VPhysicalVolume* THELGlobalMemory::ConstructWorld() {
    G4double WorldSizeX=40010.000*mm;
    G4double WorldSizeY=40010.000*mm;
    G4double WorldSizeZ=40010.000*mm;
    //G4double WorldSizeX=4000.000*mm;
    //G4double WorldSizeY=4000.000*mm;
    //G4double WorldSizeZ=4000.000*mm;

    G4cout << "######################################################" << G4endl;
    G4cout << "World dimension: " << WorldSizeX << " * " << WorldSizeY << " * " << WorldSizeZ << G4endl;
    G4cout << "######################################################" << G4endl;

    G4String name, symbol;
    G4double a, z, density;

    G4double temperature, pressure;
    density     = universe_mean_density;
    pressure    = 3.e-18*pascal;
    temperature = 2.73*kelvin;


    World_mat = new G4Material(name="Galactic", z=1., a=1.01*g/mole, density,kStateGas,temperature,pressure);

    G4Box* solidWorld = new G4Box("World",
                                  WorldSizeX/2,WorldSizeY/2,WorldSizeZ/2);

    World_log = new G4LogicalVolume(solidWorld,
                                    World_mat,
                                    "World");
    World_log->SetVisAttributes (G4VisAttributes::Invisible);

    World_phys = new G4PVPlacement(0,G4ThreeVector(),
                                   World_log,
                                   "World",
                                   0,
                                   false,
                                   0);

    return World_phys;
}

unsigned char THELGlobalMemory::LUTParticleType(G4String p) {
    unsigned char particleID;
    particleID = 0;
    if(p == "gamma")
        particleID = 1;
    if(p == "e-")
        particleID = 2;
    if(p == "e+")
        particleID = 3;
    if(p == "proton")
        particleID = 4;
    if(p == "mu+")
        particleID = 5;
    if(p == "mu-")
        particleID = 6;
    if(p == "tau+")
        particleID = 7;
    if(p == "tau-")
        particleID = 8;
    if(p == "GenericIon")
        particleID = 9;
    if(p == "neutron")
        particleID = 10;
    if(p == "pi+")
        particleID = 11;
    if(p == "pi-")
        particleID = 12;
    if(p == "triton")
        particleID = 13;
    if(p == "sigma-")
        particleID = 14;
    if(p == "sigma+")
        particleID = 15;
    if(p == "nu_mu")
        particleID = 16;
    if(p == "nu_e")
        particleID = 17;
    if(p == "lambda")
        particleID = 18;
    if(p == "kaon0S")
        particleID = 19;
    if(p == "kaon0L")
        particleID = 20;
    if(p == "kaon-")
        particleID = 21;
    if(p == "kaon+")
        particleID = 22;
    if(p == "deuteron")
        particleID = 23;
    if(p == "anti_xi0")
        particleID = 24;
    if(p == "anti_xi-")
        particleID = 25;
    if(p == "anti_sigma-")
        particleID = 26;
    if(p == "anti_proton")
        particleID = 27;
    if(p == "anti_nu_mu")
        particleID = 28;
    if(p == "anti_nu_e")
        particleID = 29;
    if(p == "anti_neutron")
        particleID = 30;
    if(p == "anti_lambda")
        particleID = 31;
    if(p == "alpha")
        particleID = 32;
    if(p == "Si30[0.0]")
        particleID = 33;
    if(p == "Si29[0.0]")
        particleID = 34;
    if(p == "Si28[0.0] ")
        particleID = 35;

    return particleID;
}

unsigned char THELGlobalMemory::LUTProcessType(G4String p) {
    unsigned char processID;
    processID = 0;
    if(p == "UserLimit")
        processID = 1;
    if(p == "Transportation")
        processID = 2;
    if(p == "hLowEIoni")
        processID = 3;
    if(p == "msc")
        processID = 4;
    if(p == "LowEnergyIoni")
        processID = 5;
    if(p == "LowEnBrem")
        processID = 6;
    if(p == "eIoni")
        processID = 7;
    if(p == "eBrem")
        processID = 8;
    if(p == "annihil")
        processID = 9;
    if(p == "phot")
        processID = 10;
    if(p == "compt")
        processID = 11;
    if(p == "conv")
        processID = 12;
    if(p == "LowEnPhotoElec")
        processID = 13;
    if(p == "LowEnCompton")
        processID=14;
    return processID;
}


