/***************************************************************************
			IOEnergyBase.cc  -  description
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

#include "IOEnergyBase.hh"
#include "THELGlobalMemory.hh"

#if defined(GEANT4_10_1) || defined(GEANT4_10_2) || defined(GEANT4_10_3) || defined(GEANT4_10_4)
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"
#endif

using namespace std;

IOEnergyBase::IOEnergyBase()
{
    gm.config->readInto(nmaxevents, "IOENERGY.NRows");
    cout << "IOENERGY.NRows " << nmaxevents << endl;

    volumeID = new G4long[NMAXVOLFOREVENT];
    energy = new G4double[NMAXVOLFOREVENT];

    trkid = new G4long[NMAXVOLFOREVENT];;
    particle = new G4long[NMAXVOLFOREVENT];;
    process = new byte[NMAXVOLFOREVENT];;
    multI = new ushort[NMAXVOLFOREVENT];;
    multO = new ushort[NMAXVOLFOREVENT];;

    ResetData();

    //volume selection
    volselectionN = 0;
    gm.config->readInto(volselectionN, "IOENERGY.volumeselection.number");
    G4cout << "Volume selection: " << volselectionN << G4endl;
    char c[512];
    for(int i=0; i<volselectionN; i++) {
        sprintf(c, "IOENERGY.volumeselection.%i.start", i);
        gm.config->readInto(volselectionstart[i], c);
        G4cout << "Volume selection " << i << " start " << volselectionstart[i] << G4endl;
        sprintf(c, "IOENERGY.volumeselection.%i.end", i);
        gm.config->readInto(volselectionend[i], c);
        G4cout << "Volume selection " << i << " end " << volselectionend[i] << G4endl;
    }
    volgroupN = 0;

    G4cout << "Volume groups: " << volgroupN << G4endl;

    for(int i=0; i<volgroupN; i++) {
        sprintf(c, "IOENERGY.volumegroup.%i.start", i);
        gm.config->readInto(volgroupstart[i], c);
        G4cout << "En Volume group " << i << " start " << volselectionstart[i] << G4endl;
        sprintf(c, "IOENERGY.volumegroup.%i.end", i);
        gm.config->readInto(volgroupend[i], c);
        G4cout << "En Volume group " << i << " end " << volselectionend[i] << G4endl;
        sprintf(c, "IOENERGY.volumegroup.%i.ID", i);
        gm.config->readInto(volgroupID[i], c);

    }

    //0, scrivili tutti gli eventi processati, anche senza detectors
    //N, scrivi solo gli eventi con energia depostitata nei detectos, N eventi per ogni file
    writeonlydetectorevents = 0;
    gm.config->readInto(writeonlydetectorevents, "IOENERGY.writeonlydetectorevents");
	
	writeonlyifenergydeposit = 0;
	gm.config->readInto(writeonlyifenergydeposit, "IOENERGY.writeonlyifenergydeposit");
	cout << "writeonlyifenergydeposit: " << writeonlyifenergydeposit << endl;
	
	removeWorldEvent = 0;
	gm.config->readInto(removeWorldEvent, "IOENERGY.removeWorldEvent");
	

}


IOEnergyBase::~IOEnergyBase()
{
}

void IOEnergyBase::ResetData() {
    for(int i=0; i<NMAXVOLFOREVENT; i++)
        if(volumeID[i] != -1)
            volumeID[i] = -1, energy[i] = 0, trkid[i] = 0, particle[i] = 0, process[i] = 0 , multI[i] = 0, multO[i]=0;
        else
            break;
    volumeID[0] = 0; //AB1
    energy[0] = 0;
    energystart = -1;

}

void IOEnergyBase::initConf() {


}


void IOEnergyBase::ResetDataBuffer() {
    Aindex=0;
    neventsprocessed_forthisfile = 0;
    neventsstored_forthisfile = 0;
    for(long i=0; i<NMAXVOLS; i++)
        if(AvolumeID[i]!=-1)
            AvolumeID[i] = -1, Aenergy[i] = 0, Aenergystart[i] = 0, Aevent_id[i] = 0, Atrkid[i]=0, Aparticle[i]=0, Aprocess[i]=0, AmultI[i]=0, AmultO[i]=0;
        else
            break;
}

void IOEnergyBase::CreateDataBuffer() {
    AvolumeID = new int[NMAXVOLS];
    Aenergy = new double[NMAXVOLS];
    Aenergystart = new double[NMAXVOLS];
    Aevent_id = new long[NMAXVOLS];
    Atrkid = new long[NMAXVOLS];
    Aparticle = new long[NMAXVOLS];
    Aprocess = new byte[NMAXVOLS];
    AmultI = new ushort[NMAXVOLS];
    AmultO = new ushort[NMAXVOLS];
    ResetDataBuffer();
    if(nmaxevents > NMAXVOLS) {
        G4cout << "IOENERGY.NRows too rows: Exit." << G4endl;
        exit(0);
    }
}

void IOEnergyBase::CopyBuffer() {
    bool copy = false;
    if(writeonlydetectorevents > 0) {
        for(long i=0; i<NMAXVOLFOREVENT; i++) {
            if(volumeID[i] != -1) {
                //verifica se ci sono eventi nei detectors
                if (volumeID[i] >= gm.detector_volmin && volumeID[i] <= gm.detector_volmax) {
                    copy = true;
                }
            } else {
                break;
            }
        }
    } else {
        copy = true;
    }
    if (copy == true) {
        for(long i=0; i<NMAXVOLFOREVENT; i++) {
            if(volumeID[i] != -1) {
                if(energy[i] < 10e-8)
                    energy[i] = 0;
				if(writeonlyifenergydeposit && energy[i] == 0)
					continue;
                AvolumeID[Aindex] = volumeID[i];
                Aenergy[Aindex] = energy[i];
                Aevent_id[Aindex] = gm.event_id;
                Aenergystart[Aindex] = energystart;
                Atrkid[Aindex] = trkid[i];
                Aparticle[Aindex] = particle[i];
				if(removeWorldEvent && particle[i] == 0)
					continue;
                Aprocess[Aindex] = process[i];
                AmultI[Aindex] = multI[i];
                AmultO[Aindex] = multO[i];
                Aindex++;
            }
            else
                break;
        }
        neventsstored_forthisfile++;
    }
    neventsprocessed_forthisfile++;
}

void IOEnergyBase::setBuffer(long inevtid, double inenergyDep, long involume, long inttrkid, long intpart, byte intproc, ushort intmultI, ushort intmultO) {
    AvolumeID[Aindex] = involume;
    Aenergy[Aindex] = inenergyDep;
    Aevent_id[Aindex] = inevtid;
    Atrkid[Aindex] = inttrkid;
    Aparticle[Aindex] = intpart;
    Aprocess[Aindex] = intproc;
    AmultI[Aindex] = intmultI;
    AmultO[Aindex] = intmultO;
    Aindex++;
}


void IOEnergyBase::addData(CSteppingAction* stepping, bool in) {
    G4int volReplica = stepping->volReplica;

    if(energystart == -1) {
        energystart = stepping->ekin / keV;
        energy[0] = energystart;
    }

    //if(volReplica == 0) //AB1 - solo ottimizzazione, non e' necessario sommare e sottrarre la stessa energia. La somma da sempre 0
    //    return;

    bool write = true;
    if(volselectionN != 0) {
        bool volumein = false;
        for(int i=0; i<volselectionN; i++) {
            if(volReplica >= volselectionstart[i] && volReplica <= volselectionend[i])
                volumein = true;
        }
        if(volumein == false)
            write = false;
    }
    if(!write)
        return;

    //aggiungi o togli l'energia
    for(int i=0; i<NMAXVOLFOREVENT; i++) {
        if(volumeID[i] == -1) {
            volumeID[i] = volReplica;
            if(in == true) {
                energy[i] += (stepping->ekin / keV);
                multI[i]++;
			}
			if(in == false) {
                energy[i] -= (stepping->ekin / keV);
                multO[i]++;
            }
            particle[i] = stepping->particleIDstep;
            //particle[i] = stepping->lastParticleID;
            process[i] = stepping->lastProcessID;
            trkid[i] = stepping->lastTrackID;
            break;
        }
        if(volumeID[i] == volReplica) {
            if(in == true) {
                energy[i] += (stepping->ekin / keV);
                multI[i]++;
			}
			if(in == false) {
                energy[i] -= (stepping->ekin / keV);
                multO[i]++;
            }
            particle[i] = stepping->particleIDstep;
            //particle[i] = stepping->lastParticleID;
            process[i] = stepping->lastProcessID;
            trkid[i] = stepping->lastTrackID;
            break;
        }
    }
}
