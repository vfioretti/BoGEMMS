/***************************************************************************
                          FITSOutputFile.cc  -  description
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

#include "FITSOutputFile.hh"
#include "THELGlobalMemory.hh"
#include <G4String.hh>
#include "G4ios.hh"
#include "globals.hh"
#include <unistd.h>
#include <iostream>
#include <string>
using namespace std;

FITSOutputFile::FITSOutputFile() {
    fptr=(fitsfile **)malloc(sizeof(fitsfile));
    verbose = false;
    flush_rows = 1;
    currentrow = 0;
    filename = (char*)"";
    opened = false;
    rownum = 0;
    colnum = 0;
    findfirstrow = 8;
}


FITSOutputFile::~FITSOutputFile() {
    free(fptr);
}

void FITSOutputFile::PrintErrorStatus(int status, int headnum, int colnum) {
    cerr << "FITS file error: header " << headnum << ", column " << colnum << endl;
    printerror(status);
}


long FITSOutputFile::getRowNum() {
    if(!changeHeader(masterheadnum))
        return -1;
    status = 0;
    fits_get_num_rows(*fptr, &rownum, &status);
    return rownum;
}

int FITSOutputFile::getColNum(char* nomecol) {
    if(!changeHeader(masterheadnum))
        return -1;
    status = 0;
    fits_get_colnum(*fptr,CASEINSEN,nomecol,&colnum,&status);
    return colnum;
}


bool FITSOutputFile::open() {
    status = 0;
    if(opened)
        return true;

    if ( !fits_open_file(fptr, filename, READWRITE, &status) ) {
        cout << filename << " opened. " << endl;
        opened = true;
    } else
        opened = false;
    return opened;
}

bool FITSOutputFile::close() {
    status = 0;
    if(!opened) return true;
    if ( fits_close_file(*fptr, &status) )
        printerror( status );

    opened = false;

// 	cout << "FITSOutputFile::close() " << endl;
    cout << "Evt id: " << gm.event_id << endl;
    return true;

}

void FITSOutputFile::setMasterHeader(int masterheadnum) {
    this->masterheadnum = masterheadnum;
}

long FITSOutputFile::findFirstRow(char* expr) {
    if(!changeHeader(masterheadnum))
        return -1;
    status = 0;
    // 	long n_good[10];
    // 	char row_status[10];
    if(fits_find_first_row(*fptr, expr, &findfirstrow, &status))
        printerror( status );
    // 	fits_find_rows(*fptr, expr, 1, 10, n_good, row_status, &status);
    // 	for(int i=0; i<10; i++) {
    // 		cout << n_good[i] << " " << (int)row_status[i] << endl;
    // 	}
    return findfirstrow;
}

void FITSOutputFile::printerror(int status) {
    if (status) {
        fits_report_error(stderr, status);
        exit( status );
    }
    return;
}


void FITSOutputFile::writeInitHeaderKey(int header) {
    int status = 0;

    string str1;

    if(!changeHeader(header))
        return;

    if ( fits_write_key(*fptr,  TSTRING, (char*)"TELESCOP" ,
                        (void*)"ANY", (char*)"Telescope or mission (OGIP/93-013)" , &status) )
        printerror( status );
    if ( fits_write_key(*fptr,  TSTRING, (char*)"INSTRUME" ,
                        (void*)"OVERALL", (char*)"Instrument of the telescope (OGIP/93-013)" , &status) )
        printerror( status );
    if ( fits_write_key(*fptr,  TSTRING, (char*)"DETNAM" ,
                        (void*)"OVERALL", (char*)"Detector of the instrument (OGIP/93-013)" , &status) )
        printerror( status );
    if ( fits_write_key(*fptr,  TSTRING, (char*)"CREATOR" ,
                        (void*)"BoGEMMS", (char*)"Program that created this FITS file (HFWG R7)" , &status) )
        printerror( status );
    if ( fits_write_key(*fptr,  TSTRING, (char*)"CREAT_ID" ,
                        (void*)"Andrea Bulgarelli", (char*)"Unique ID of the creator" , &status) )
        printerror( status );
    if ( fits_write_key(*fptr,  TSTRING, (char*)"FILENAME" ,
                        (void*)filename, (char*)"Original file name of this FITS" , &status) )
        printerror( status );
    if ( fits_write_key(*fptr,  TSTRING, (char*)"FILEVER" ,
                        (void*)"1", (char*)"File version" , &status) )
        printerror( status );
    char* datestr = new char[30];
    int timeref = 0;
    status = 0;
    if(fits_get_system_time(datestr, &timeref, &status))
        printerror(status);
    if ( fits_write_key(*fptr, TSTRING, (char*)"DATE",
                        (void*) datestr, (char*)"FITS File creation date (GTB)", &status) )
        printerror( status );
    delete[] datestr;

}

void FITSOutputFile::writeHistoryFile() {
    return;
}

void FITSOutputFile::writeEndHeaderKey(int header) {
    int status = 0;

    //move to the first hdu
    if(!changeHeader(header))
        return;

    for (int ii = 0; ii< 10; ii++ ) {
        if ( fits_write_comment(*fptr,
                                "                                    ", &status) )
            printerror( status );
    }

    writeHistoryFile();

}

void FITSOutputFile::writeChksum(int header) {
    int status = 0;
    if(!changeHeader(header))
        return;
    if(fits_write_chksum(*fptr, &status))
        printerror(status);
}

bool FITSOutputFile::changeHeader(int headnum) {
    status = 0;
    if ( fits_movabs_hdu(*fptr, headnum, 0, &status) ) {
        printerror( status );
        return false;
    }
    return true;
}


char* FITSOutputFile::setFileName(char* filenamebase) {
    filename = filenamebase;
    lastrow_flushed = 0;
    return filename;
}

bool FITSOutputFile::flush_file() {
    int status=0;
    if(verbose) cout << "FITSOutputFile: Current row: " << currentrow << ". Last row flushed: " << lastrow_flushed << ". Flush every " << flush_rows << "rows" << endl;
    if(currentrow >= lastrow_flushed + flush_rows) {
        fits_flush_file (*fptr, &status);
        lastrow_flushed = currentrow;
        return true;
    }
    return false;
}


int FITSOutputFile::openOutFits(char * fitsname, char * templatefile ) {
    int status_fits=0 ;

    if (fits_create_template(fptr,fitsname ,templatefile, &status_fits))
        printerror( 0, (char*)" called fits_create_template: ", status_fits );

    return status_fits;

}


int FITSOutputFile::printerror(int codice, const char* messaggio,int status) {
    char errtext[40] ; // max per fitsio  30 char + '/0'

    cerr << " printerror called from: " << messaggio << " codice: " << codice << endl ;

    if (status) {
        fits_get_errstatus( status, errtext);
        cerr << " ERROR IN FITSOutputFileIO called from L1fits! : Number: " << status << " "
             << errtext << endl ;

        fits_report_error(stderr,status) ;  //detailded report

        cerr << endl <<endl ;
    }
    flushCloseandExit() ;
    return 1 ;
}


int FITSOutputFile::flushCloseandExit() {
    int status ;

    if ( fits_flush_file(*fptr, &status) )
        printerror( status );
    if ( fits_close_file(*fptr, &status) )
        printerror( status );
    cerr << " Error fits recovery: flush fits file, close fits file, exit program ..." << endl ;
    exit( status ) ;
}

//====================================================================================================


FITSOutputFileBinaryTable::FITSOutputFileBinaryTable(int ncol_header1, int ncol_header2, int ncol_header3, int ncol_header4, int ncol_header5, int ncol_header6) : FITSOutputFile() {
    this->ncol_header1 = ncol_header1;
    this->ncol_header2 = ncol_header2;
    this->ncol_header3 = ncol_header3;
    this->ncol_header4 = ncol_header4;
    this->ncol_header5 = ncol_header5;
    this->ncol_header6 = ncol_header6;

    ttype1 = new char* [ncol_header1];
    tform1 = new char* [ncol_header1];
    tunit1 = new char* [ncol_header1];
    pfind1 = new int [ncol_header1]; 	//  for field index in .packet
    tycom1 = (char**) new  char* [ncol_header1]; //  for keyword comment field in the extensions
    if(ncol_header2 > 0) {
        tform2 = new char* [ncol_header2];
        ttype2 = new char* [ncol_header2];
        tunit2 = new char* [ncol_header2];
        pfind2 = new int [ncol_header2];
        tycom2 =  (char**)  new  char* [ncol_header2];
    } else {
        tform2 = 0;
        ttype2 = 0;
        tunit2 = 0;
        pfind2 = 0;
        tycom2 = 0;
    }
    if(ncol_header3 > 0) {
        tform3 = new char* [ncol_header3];
        ttype3 = new char* [ncol_header3];
        tunit3 = new char* [ncol_header3];
        pfind3 = new int [ncol_header3];
        tycom3 = (char**)  new char* [ncol_header3];

    } else {
        tform3 = 0;
        ttype3 = 0;
        tunit3 = 0;
        pfind3 = 0;
        tycom3 = 0;
    }
    if(ncol_header4 > 0) {
        tform4 = new char* [ncol_header4];
        ttype4 = new char* [ncol_header4];
        tunit4 = new char* [ncol_header4];
        pfind4 = new int [ncol_header4];
        tycom4 = (char**) new  char* [ncol_header4];

    } else {
        tform4 = 0;
        ttype4 = 0;
        tunit4 = 0;
        pfind4 = 0;
        tycom4 = 0;
    }
    if(ncol_header5 > 0) {
        tform5 = new char* [ncol_header5];
        ttype5 = new char* [ncol_header5];
        tunit5 = new char* [ncol_header5];
        pfind5 = new int [ncol_header5];
        tycom5 = (char**) new  char* [ncol_header5];

    } else {
        tform5 = 0;
        ttype5 = 0;
        tunit5 = 0;
        pfind5 = 0;
        tycom5 = 0;
    }
    if(ncol_header6 > 0) {
        tform6 = new char* [ncol_header6];
        ttype6 = new char* [ncol_header6];
        tunit6 = new char* [ncol_header6];
        pfind6 = new int [ncol_header6];
        tycom6 = (char**) new  char* [ncol_header6];

    } else {
        tform6 = 0;
        ttype6 = 0;
        tunit6 = 0;
        pfind6 = 0;
        tycom6 = 0;
    }
}


FITSOutputFileBinaryTable::~FITSOutputFileBinaryTable() {
    delete[] tform1;
    delete[] ttype1;
    delete[] tunit1;
    delete[] pfind1;
    delete[] tycom1;
    delete[] tform2;
    delete[] ttype2;
    delete[] tunit2;
    delete[] pfind2;
    delete[] tycom2;
    delete[] tform3;
    delete[] ttype3;
    delete[] tunit3;
    delete[] pfind3;
    delete[] tycom3;
    delete[] tform4;
    delete[] ttype4;
    delete[] tunit4;
    delete[] pfind4;
    delete[] tycom4;
    delete[] tform5;
    delete[] ttype5;
    delete[] tunit5;
    delete[] pfind5;
    delete[] tycom5;
    delete[] tform6;
    delete[] ttype6;
    delete[] tunit6;
    delete[] pfind6;
    delete[] tycom6;
}


bool FITSOutputFileBinaryTable::init() {

    long nrows = 1;
    char TTYPEXX[10];


    nrows_event_hdu = 1;
    nrows_event_hdu2 = 1;
    nrows_event_hdu3 = 1;
    nrows_event_hdu4 = 1;
    nrows_event_hdu5 = 1;
    nrows_event_hdu6 = 1;

    //important
    currentrow = 1;
    status=0;

    /* create new FITSOutputFile file */
    if (fits_create_file(fptr, filename, &status))
        printerror( status );

    if ( fits_create_tbl(*fptr, BINARY_TBL, nrows, ncol_header1, ttype1, tform1,
                         tunit1, extname1, &status) )
        printerror( status );
    cout << "NCOL created " << ncol_header1 << endl;
    writeInitHeaderKey(2);

    if ( fits_movabs_hdu(*fptr, 2, 0, &status) )
        printerror( status );
    for (int i= 0; i < ncol_header1; i++ ) {
        sprintf(TTYPEXX, "TTYPE%d",i+1);
        if (fits_modify_comment (*fptr, TTYPEXX, tycom1[i], &status))
            printerror(status);
    }

    if(ncol_header2 > 0) {
        if ( fits_create_tbl(*fptr, BINARY_TBL, nrows, ncol_header2, ttype2, tform2,
                             tunit2, extname2, &status) )
            printerror( status );

        writeInitHeaderKey(3);
        if ( fits_movabs_hdu(*fptr, 3, 0, &status) )
            printerror( status );
        for (int i= 0; i < ncol_header2; i++ ) {
            sprintf(TTYPEXX, "TTYPE%d",i+1);
            if (fits_modify_comment (*fptr, TTYPEXX, tycom2[i], &status))
                printerror(status);
        }
    }

    if(ncol_header3 > 0) {
        if ( fits_create_tbl(*fptr, BINARY_TBL, nrows, ncol_header3, ttype3, tform3,
                             tunit3, extname3, &status) )
            printerror( status );

        writeInitHeaderKey(4);
        if ( fits_movabs_hdu(*fptr, 4, 0, &status) )
            printerror( status );
        for (int i= 0; i < ncol_header3; i++ ) {
            sprintf(TTYPEXX, "TTYPE%d",i+1);
            if (fits_modify_comment (*fptr, TTYPEXX, tycom3[i], &status))
                printerror(status);
        }
    }

    if(ncol_header4 > 0) {
        if ( fits_create_tbl(*fptr, BINARY_TBL, nrows, ncol_header4, ttype4, tform4,
                             tunit4, extname4, &status) )
            printerror( status );

        writeInitHeaderKey(5);
        if ( fits_movabs_hdu(*fptr, 5, 0, &status) )
            printerror( status );
        for (int i= 0; i < ncol_header4; i++ ) {
            sprintf(TTYPEXX, "TTYPE%d",i+1);
            if (fits_modify_comment (*fptr, TTYPEXX, tycom4[i], &status))
                printerror(status);
        }
    }

    if(ncol_header5 > 0) {
        if ( fits_create_tbl(*fptr, BINARY_TBL, nrows, ncol_header5, ttype5, tform5,
                             tunit5, extname5, &status) )
            printerror( status );

        writeInitHeaderKey(6);
        if ( fits_movabs_hdu(*fptr, 6, 0, &status) )
            printerror( status );
        for (int i= 0; i < ncol_header5; i++ ) {
            sprintf(TTYPEXX, "TTYPE%d",i+1);
            if (fits_modify_comment (*fptr, TTYPEXX, tycom5[i], &status))
                printerror(status);
        }
    }

    if(ncol_header6 > 0) {
        if ( fits_create_tbl(*fptr, BINARY_TBL, nrows, ncol_header6, ttype6, tform6,
                             tunit6, extname6, &status) )
            printerror( status );

        writeInitHeaderKey(7);
        if ( fits_movabs_hdu(*fptr, 7, 0, &status) )
            printerror( status );
        for (int i= 0; i < ncol_header6; i++ ) {
            sprintf(TTYPEXX, "TTYPE%d",i+1);
            if (fits_modify_comment (*fptr, TTYPEXX, tycom6[i], &status))
                printerror(status);
        }
    }
    return status;
}




bool FITSOutputFileBinaryTable::close() {
    if(!opened) return true;

    status = 0;

    writeEndHeaderKey(1);

    writeChksum(1);

    writeEndHeaderKey(2);

    writeChksum(2);

    if(ncol_header2 > 0) {

        writeEndHeaderKey(3);

        writeChksum(3);

    }

    if(ncol_header3 > 0) {

        writeEndHeaderKey(4);

        writeChksum(4);

    }

    if(ncol_header4 > 0) {

        writeEndHeaderKey(5);

        writeChksum(5);

    }

    if(ncol_header5 > 0) {

        writeEndHeaderKey(6);

        writeChksum(6);

    }

    if(ncol_header6 > 0) {

        writeEndHeaderKey(7);

        writeChksum(6);

    }

    /* close the FITSOutputFile file */
    return FITSOutputFile::close();

}




char* FITSOutputFileBinaryTable::getValue(char* str) {
    int l = strlen(str);
    int ins = 0;
    for(ins=0; ins<l; ins++) {
        if(str[ins] == '=')
            break;
    }
    int size = l-ins;
    char* c = new char[size];
    for(int i=0; i<size-1; i++) {
        c[i] = str[ins+1+i];
    }
    c[size-1] = '\0';
    delete[] str;
    return c;
}


