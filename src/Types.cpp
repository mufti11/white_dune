/*
 * Types.cpp
 *
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

#include "Types.h"
#include "parser.h"

#include "SFMFTypes.h"

FieldValue* defaultTypeArray[] = 
{
   new MFBool(),
   new MFColor(),
   new MFColorRGBA(),
   new MFDouble(),
   new MFFloat(),
   new MFInt32(),
   new MFMatrix3f(),
   new MFMatrix4f(),
   new MFNode(),
   new MFRotation(),
   new MFString(),
   new MFTime(),
   new MFVec2d(),
   new MFVec2f(),
   new MFVec3d(),
   new MFVec3f(),
   new MFVec4f(),
   new SFBool(),
   new SFColor(),
   new SFColorRGBA(),
   new SFDouble(),
   new SFFloat(),
   new SFImage(),
   new SFInt32(),
   new SFMatrix3f(),
   new SFMatrix4f(),
   new SFNode(),
   new SFRotation(),
   new SFString(),
   new SFTime(),
   new SFVec2d(),
   new SFVec2f(),
   new SFVec3d(),
   new SFVec3f(),
   new SFVec4f(),
};

int
typeStringToEnum(const char* str)
{
   int len = sizeof(defaultTypeArray)/sizeof(FieldValue*);
   for (int i = 0; i < len; i++)
       if (strcmp(defaultTypeArray[i]->getTypeName(),str)==0)
           return defaultTypeArray[i]->getType();
   return -1;
}

const char* 
typeEnumToString(int type)
{
   return defaultTypeArray[type]->getTypeName();
}

FieldValue *typeDefaultValue(int type)
{
   FieldValue *ret = defaultTypeArray[type]->copy();
   ret->setIsDefaultValue();
   return ret;
}

const char *getTypeC(int type, int languageFlag)
{
   return defaultTypeArray[type]->getTypeC(languageFlag);
}

bool isArrayInC(int type)
{
   return defaultTypeArray[type]->isArrayInC();
}

int getSFType(int type) 
{
   switch(type) {
     case MFBOOL:
       return SFBOOL;
     case MFCOLOR:
       return SFCOLOR;
     case MFCOLORRGBA:
       return SFCOLORRGBA;
     case MFDOUBLE:
       return SFDOUBLE;
     case MFFLOAT:
       return SFFLOAT;
     case MFINT32:
       return SFINT32;
     case MFMATRIX3F:
       return SFMATRIX3F;
     case MFMATRIX4F:
       return SFMATRIX4F;
     case MFNODE:
       return SFNODE;
     case MFROTATION:
       return SFROTATION;
     case MFSTRING:
       return SFSTRING;
     case MFTIME:
       return SFTIME;
     case MFVEC2D:
       return SFVEC2D;
     case MFVEC2F:
       return SFVEC2F;
     case MFVEC3D:
       return SFVEC3D;
     case MFVEC3F:
       return SFVEC3F;
     case MFVEC4F:
       return SFVEC4F;
   }   
   return type;
}

bool isMFType(int type) 
{
   switch(type) {
     case MFBOOL:
       return true;
     case MFCOLOR:
       return true;
     case MFCOLORRGBA:
       return true;
     case MFDOUBLE:
       return true;
     case MFFLOAT:
       return true;
     case MFINT32:
       return true;
     case MFMATRIX3F:
       return true;
     case MFMATRIX4F:
       return true;
     case MFNODE:
       return true;
     case MFROTATION:
       return true;
     case MFSTRING:
       return true;
     case MFTIME:
       return true;
     case MFVEC2D:
       return true;
     case MFVEC2F:
       return true;
     case MFVEC3D:
       return true;
     case MFVEC3F:
       return true;
     case MFVEC4F:
       return true;
   }   
   return false;
}
