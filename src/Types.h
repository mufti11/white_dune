/*
 * Types.h 
 *
 * Copyright (C) 1999 Stephen F. White
 * Copyright (C) 2003 J. "MUFTI" Scheurich
 *
 * automatic (via configure) generated file, direct change is useless.
 * change batch/mkSFMFTypes.sh instead
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program (see the file "COPYING" for details); if 
 * not, write to the Free Software Foundation, Inc., 675 Mass Ave, 
 * Cambridge, MA 02139, USA.
 */

#ifndef _TYPES_H
#define _TYPES_H

#include "MyString.h"
class FieldValue;

enum {
    MFBOOL = 0,
    MFCOLOR = 1,
    MFCOLORRGBA = 2,
    MFDOUBLE = 3,
    MFFLOAT = 4,
    MFINT32 = 5,
    MFMATRIX3F = 6,
    MFMATRIX4F = 7,
    MFNODE = 8,
    MFROTATION = 9,
    MFSTRING = 10,
    MFTIME = 11,
    MFVEC2D = 12,
    MFVEC2F = 13,
    MFVEC3D = 14,
    MFVEC3F = 15,
    MFVEC4F = 16,
    SFBOOL = 17,
    SFCOLOR = 18,
    SFCOLORRGBA = 19,
    SFDOUBLE = 20,
    SFFLOAT = 21,
    SFIMAGE = 22,
    SFINT32 = 23,
    SFMATRIX3F = 24,
    SFMATRIX4F = 25,
    SFNODE = 26,
    SFROTATION = 27,
    SFSTRING = 28,
    SFTIME = 29,
    SFVEC2D = 30,
    SFVEC2F = 31,
    SFVEC3D = 32,
    SFVEC3F = 33,
    SFVEC4F = 34
};

#define LAST_TYPE  SFVEC4F

int typeStringToEnum(const char* str);

const char* typeEnumToString(int type);

FieldValue *typeDefaultValue(int type);

const char *getTypeC(int type, int languageFlag);

bool isArrayInC(int type);

int getSFType(int type);

bool isMFType(int type);

#endif // _TYPES_H

