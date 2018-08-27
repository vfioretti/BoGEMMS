/***************************************************************************
                          THELGlobalMemory.hh  -  description
                             -------------------
    begin                : 2010
    copyright            : (C) 2010 by  Andrea Bulgarelli
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
#ifndef SXGLOBALMEMORY_H
#define SXGLOBALMEMORY_H

#define USE_FITS 1


#include "G4ios.hh"
#include "G4String.hh"
#include "CXYZSD.hh"
#include "ConfigFile.hh"

class G4VPhysicalVolume;
class FITSOutput_INOUT;
// VF
class FITSOutput_XYZ;
class FITSOutput_Step;
class FITSOutput_EnergyVol;
class FITSOutput_EnergyVolEnd;
class G4LogicalVolume;
class G4VPhysicalVolume;
class G4Material;

#ifndef WITHOUTROOT
class RootOutput_XYZ;
class RootOutput_EnergyVol;
class RootOutput_EnergyVolEnd;
#endif

#ifdef SQLITE3
class SQLliteOutput_XYZ;
#endif

class THELGlobalMemory {
public:
    THELGlobalMemory();

    ~THELGlobalMemory();

    G4String configFileName; //the name of the config file

    G4String GetOutputHitFileNameTXT();

    G4String GetOutputStepBoundaryFileNameTXT();

    //VF
    G4String GetOutputXYZFileNameTXT();

    G4String GetOutputHitFileNameFITS();

    G4String GetOutputStepBoundaryFileNameFITS();

    // VF
    G4String GetOutputXYZFileNameFITS();

    G4String GetOutputEnergyVolFileNameFITS();

    G4String GetOutputEnergyVolFileNameRoot();
	
	G4String GetOutputXYZFileNameRoot();

    void OpenConfigFile();

    G4int event_id;

    G4bool DOWNLOADGEOMETRY;

    int compressionlevel;

    void AddSensitiveDetector(G4VPhysicalVolume* pv);
    void AddXYZDetector(G4LogicalVolume* logs);

    FITSOutput_INOUT* fits_inout;

    // VF
    FITSOutput_XYZ* fits_xyz;

    FITSOutput_Step* fits_step;

    FITSOutput_EnergyVol* fits_energyvol;

    FITSOutput_EnergyVol* fits_energyvolend;
    
#ifdef SQLITE3
	
	SQLliteOutput_XYZ* sqlite3_xyz;
	
#endif
	
#ifndef WITHOUTROOT

	// AB
	RootOutput_XYZ* root_xyz;

    RootOutput_EnergyVol* root_energyvol;

    RootOutput_EnergyVolEnd* root_energyvolend;
	
#endif

    void InitFiles();

    long nfileFITSINOUT;

    // VF
    long nfileFITSXYZ;

    long nfileRootENERGYVOL;
	
	long nfileRootXYZ;

    long nfileFITSENERGYVOL;

    long nfileFITSHit;

    ConfigFile* config;

    G4VPhysicalVolume* ConstructWorld();

    G4LogicalVolume* World_log;

    G4VPhysicalVolume* World_phys;

    G4Material* World_mat;

    unsigned char LUTParticleType(G4String p);

    unsigned char LUTProcessType(G4String p);

    int CopyFileFits(char* in, char* out);

public:

    int detector_volmin;

    int detector_volmax;

    //configuration (see constructor)

    bool enableLowEnergyPhysic;

    bool enableHighEnergyPhysic;

    G4bool enableWriteINOUT;

    // VF
    G4bool enableWriteXYZ;

    G4bool enableWriteSTEP;

    G4bool enableWriteEnergyVol;

    G4bool enableWriteFITS;

    G4bool enableWriteFITSEnd;

    G4bool enableWriteRoot;

    G4bool enableWriteRootEnd;

    G4bool enableWriteXrootd;
    
    G4bool enableWriteSQlite3;

    G4bool enableWriteTXT;

    int geomVersion;

    int physVersion;

    G4String xrootdcon;

public:

    G4String outputHitFileNameTXT;

    G4String outputStepBoundaryFileNameTXT;

    G4String outputStepBoundaryFileNameFITS;

    // VF
    G4String outputXYZFileNameTXT;

    G4String outputXYZFileNameFITS;

    G4String outputStepBoundaryFileNameRoot;

    G4String outputStepBoundaryFileNameFITS_base;

    // VF
    G4String outputXYZFileNameRoot;

    G4String outputXYZFileNameFITS_base;

    G4String outputHitFileNameFITS;

    G4String outputHitFileNameFITS_base;

    G4String outputEnergyVolFileNameFITS;

    G4String outputEnergyVolFileNameRoot;

    G4String outputEnergyVolFileNameFITS_base;

};

extern THELGlobalMemory gm;

#endif


