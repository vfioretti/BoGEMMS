
/***************************************************************************
                          CAnalysis.cc  -  description
                             -------------------
    begin                : 2001-2008
    copyright            : (C) 2008 A. Pfeiffer, modified by Andrea Bulgarelli
    email                : bulgarelli@iasfbo.inaf.it
 ***************************************************************************/
//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
// Author:  A. Pfeiffer (Andreas.Pfeiffer@cern.ch)
//         (copied from his UserAnalyser class)
//
// History:
// -----------
//  8 Nov 2002   GS         Migration to AIDA 3
//  7 Nov 2001   MGP        Implementation
//
// -------------------------------------------------------------------

#include "CAnalysis.hh"
#include "globals.hh"
#include "G4Track.hh"
#include "G4ios.hh"
#include <fstream>
#include <iomanip>
#include "G4SteppingManager.hh"
#include "G4ThreeVector.hh"

#if defined(GEANT4_10_1) || defined(GEANT4_10_2)|| defined(GEANT4_10_3) || defined(GEANT4_10_4)
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"
#endif

CAnalysis* CAnalysis::instance = 0;

CAnalysis::CAnalysis()
#ifdef G4ANALYSIS_USE
    : analysisFactory(0)
    , tree(0)
    , histoFactory(0)
    , tupleFactory(0), h1(0), h2(0), h3(0), h4(0), ntuple(0)
    // #ifdef G4ANALYSIS_USE_PLOTTER
    //   , plotterFactory(0)
    //   , plotter(0)
    // #endif
#endif
{
#ifdef G4ANALYSIS_USE
    histFileName = "template_geant4";
    histFileType = "hbook";
#endif

    asciiFileName="template_geant4.out";
    std::ofstream asciiFile(asciiFileName, std::ios::app);
    if(asciiFile.is_open()) {
        asciiFile << "V1 V2 Energy (keV)  x (mm)    y (mm)    z (mm)" << G4endl << G4endl;
    }
}

CAnalysis::~CAnalysis() {
#ifdef G4ANALYSIS_USE

    if (tupleFactory) delete tupleFactory;
    tupleFactory = 0;

    if (histoFactory) delete histoFactory;
    histoFactory = 0;

    if (tree) delete tree;
    tree = 0;

    if (analysisFactory) delete analysisFactory;
    analysisFactory = 0;
#endif
}

CAnalysis* CAnalysis::getInstance() {
    if (instance == 0) instance = new CAnalysis;
    return instance;
}


void CAnalysis::book() {
#ifdef G4ANALYSIS_USE
    //build up  the  factories
    analysisFactory = AIDA_createAnalysisFactory();
    if(analysisFactory) {
        //parameters for the TreeFactory
        G4bool fileExists = true;
        G4bool readOnly   = false;
        AIDA::ITreeFactory* treeFactory = analysisFactory->createTreeFactory();
        if(treeFactory) {
            G4String histFileNameComplete;
            histFileNameComplete = histFileName+".hbook";
            tree = treeFactory->create(histFileNameComplete, "hbook", readOnly, fileExists);
            G4cout << " Histogramfile: " << histFileNameComplete << G4endl;

            if (tree) {
                G4cout << "Tree store : " << tree->storeName() << G4endl;
                G4cout << "Booked Hbook File " << G4endl;

                //HistoFactory and TupleFactory depend on theTree
                histoFactory = analysisFactory->createHistogramFactory ( *tree );
                tupleFactory = analysisFactory->createTupleFactory     ( *tree );

                // Book histograms
                h1 = histoFactory->createHistogram1D("1","Energy, all /keV",  100,0.,100.);
                h2 = histoFactory->createHistogram2D("2","y-z, all /mm", 100,-500.,500.,100,-500.,500.);
                h3 = histoFactory->createHistogram1D("3","Energy, entering detector /keV", 500,0.,500.);
                h4 = histoFactory->createHistogram2D("4","y-z, entering detector /mm", 200,-50.,50.,200,-50.,50.);

                // Book ntuples
                ntuple = tupleFactory->create( "10", "Track ntuple",
                                               "double energy,x,y,z,dirx,diry,dirz" );
            }
            delete treeFactory;
        }
    }
#endif

}

void CAnalysis::finish() {
#ifdef G4ANALYSIS_USE
    if (tree) {
        // Committing the transaction with the tree
        G4cout << "Committing..." << G4endl;
        // write all histograms to file
        tree->commit();

        G4cout << "Closing the tree..." << G4endl;

        // close (will again commit)
        tree->close();
    }

    // extra delete as objects are created in book() method rather than during
    // initialisation of class

    // #ifdef G4ANALYSIS_USE_PLOTTER
    //   if (plotterFactory)  delete plotterFactory;
    // #endif

    if (tupleFactory)    delete tupleFactory;
    if (histoFactory)    delete histoFactory;
    if (tree)            delete tree;
    if (analysisFactory) delete analysisFactory;
#endif
}

void CAnalysis::analyseStepping(const G4Track& track, G4bool entering) {
    return;
    eKin = track.GetKineticEnergy()/keV;
    G4ThreeVector pos = track.GetPosition()/mm;
    x = pos.x();
    y = pos.y();
    z = pos.z();
    G4ThreeVector dir = track.GetMomentumDirection();
    dirX = dir.x();
    dirY = dir.y();
    dirZ = dir.z();

#ifdef G4ANALYSIS_USE
    // Fill histograms, all tracks

    h1->fill(eKin);  // fill(x,y,weight)

    h2->fill(y,z);

    // Fill histograms and ntuple, tracks entering the detector
    if (entering) {
        // Fill and plot histograms

        h3->fill(eKin);

        h4->fill(y,z);
        // #ifdef G4ANALYSIS_USE_PLOTTER
        //     plotAll();
        // #endif
    }

    // Fill ntuple
    if (entering) {
        if (ntuple) {
            // Fill the secondaries ntuple
            ntuple->fill( ntuple->findColumn( "energy" ), (G4double) eKin );
            ntuple->fill( ntuple->findColumn( "x"      ), (G4double) x    );
            ntuple->fill( ntuple->findColumn( "y"      ), (G4double) y    );
            ntuple->fill( ntuple->findColumn( "z"      ), (G4double) z    );
            ntuple->fill( ntuple->findColumn( "dirx"   ), (G4double) dirX );
            ntuple->fill( ntuple->findColumn( "diry"   ), (G4double) dirY );
            ntuple->fill( ntuple->findColumn( "dirz"   ), (G4double) dirZ );

            ntuple->addRow(); // check for returning true ...
        } else {
            G4cout << "Ntuple not found" << G4endl;
        }
    }

#endif

    // Write to file
    if (entering) {
        std::ofstream asciiFile(asciiFileName, std::ios::app);
        if(asciiFile.is_open()) {
            asciiFile << track.GetTrackID() << " ";
            asciiFile << track.GetVolume()->GetName() << " ";
            asciiFile << track.GetNextVolume()->GetName() << " ";
            asciiFile << std::setiosflags(std::ios::fixed)
                      << std::setprecision(3)
                      << std::setiosflags(std::ios::right)
                      << std::setw(10);
            asciiFile << eKin;
            asciiFile << std::setiosflags(std::ios::fixed)
                      << std::setprecision(3)
                      << std::setiosflags(std::ios::right)
                      << std::setw(10);
            asciiFile << x;
            asciiFile << std::setiosflags(std::ios::fixed)
                      << std::setprecision(3)
                      << std::setiosflags(std::ios::right)
                      << std::setw(10);
            asciiFile << y;
            asciiFile << std::setiosflags(std::ios::fixed)
                      << std::setprecision(3)
                      << std::setiosflags(std::ios::right)
                      << std::setw(10);
            asciiFile << z
                      << G4endl;
            asciiFile.close();
        }
    }

}



