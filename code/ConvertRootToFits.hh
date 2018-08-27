/***************************************************************************
	ConvertRootToFits.HH  -  description
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

#ifndef WITHOUTROOT


#ifndef CONVERTROOTTOFITS_HH
#define CONVERTROOTTOFITS_HH


class ConvertRootToFits {
public:
    ConvertRootToFits();

    ~ConvertRootToFits();

    void Convert(char* fileroot) ;

};

#endif

#endif
