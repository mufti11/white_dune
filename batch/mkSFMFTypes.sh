#!/bin/sh

# This file is free software; the Free Software Foundation    
# gives unlimited permission to copy, distribute and modify it.

# script to generate src/SFMFTypes.h, src/Types.h and src/Types.cpp 
# by scanning for files named "src/[SM]F[A-Z][a-z]*.h" to find VRML types

AWK=awk

if test "X_`uname`" = "X_SunOS"; then
   if test -x /usr/xpg4/bin/awk ; then
      AWK=/usr/xpg4/bin/awk
   fi
fi      

if test $# != 0 ;then
   echo usage: $0 
   exit 1
fi

EXECDIR=`dirname $0`

OUTPUT1=$EXECDIR/../src/SFMFTypes.h

cat > $OUTPUT1 << EOT
/*
 * SFMFTypes.h 
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

#pragma once

EOT

for i in `ls $EXECDIR/../src/[SM]F[A-Z][a-z]*.h | grep -v Item | grep -v SFMF | grep -v Command` ; do
   echo "#include \"`basename $i .h`.h\"" >> $OUTPUT1
done

OUTPUT2=$EXECDIR/../src/Types.h

cat > $OUTPUT2 << EOT
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

#pragma once

#include "MyString.h"
class FieldValue;

enum {
EOT

for i in `ls $EXECDIR/../src/[SM]F[A-Z][a-z]*.h | grep -v Item | grep -v SFMF | grep -v Command` ; do
  TYPE=`basename $i .h`
  ENUM=`echo '    ' $TYPE | tr [a-z] [A-Z]`
  LAST=$ENUM
done


NUM=0
for i in `ls $EXECDIR/../src/[SM]F[A-Z][a-z]*.h | grep -v Item | grep -v SFMF | grep -v Command` ; do
  TYPE=`basename $i .h`
  ENUM=`echo '    ' $TYPE | tr [a-z] [A-Z]`
  if test "$LAST" = "$ENUM" ; then
    echo "   " $ENUM = $NUM>> $OUTPUT2
  else
    echo "   " $ENUM = $NUM,  >> $OUTPUT2
  fi
  NUM=`expr $NUM + 1`
done

cat >> $OUTPUT2 << EOT
};

EOT

echo "#define LAST_TYPE " $ENUM >> $OUTPUT2 

cat >> $OUTPUT2 << EOT

int typeStringToEnum(const char* str);

const char* typeEnumToString(int type);

FieldValue *typeDefaultValue(int type);

const char *getTypeC(int type, int languageFlag);

bool isArrayInC(int type);

int getSFType(int type);

bool isMFType(int type);

EOT

OUTPUT3=$EXECDIR/../src/Types.cpp

cat > $OUTPUT3 << EOT
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
EOT

for i in `ls $EXECDIR/../src/[SM]F[A-Z][a-z]*.h | grep -v Item | grep -v SFMF | grep -v Command` ; do
  TYPE=`basename $i .h`
  echo "   new $TYPE(),"  >> $OUTPUT3
done
     
cat >> $OUTPUT3 << EOT
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
EOT

for i in `ls $EXECDIR/../src/MF[A-Z][a-z]*.h | grep -v Item | grep -v SFMF | grep -v Command` ; do
  TYPE=`basename $i .h`
  ENUM=`echo $TYPE | tr [a-z] [A-Z]`
  echo "     case $ENUM:"  >> $OUTPUT3
  SFTYPE=`$AWK -v type=$ENUM 'BEGIN {sub("MF","SF",type);print type}'`
  echo "       return $SFTYPE;"  >> $OUTPUT3
done

cat >> $OUTPUT3 << EOT
   }   
   return type;
}

bool isMFType(int type) 
{
   switch(type) {
EOT

for i in `ls $EXECDIR/../src/MF[A-Z][a-z]*.h | grep -v Item | grep -v SFMF | grep -v Command` ; do
  TYPE=`basename $i .h`
  ENUM=`echo $TYPE | tr [a-z] [A-Z]`
  echo "     case $ENUM:"  >> $OUTPUT3
  echo "       return true;"  >> $OUTPUT3
done

cat >> $OUTPUT3 << EOT
   }   
   return false;
}
EOT

OUTPUT4=$EXECDIR/../src/SFMFTypesItem.h

cat > $OUTPUT4 << EOT
/*
 * SFMFTypesItem.h 
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

#pragma once

EOT

for i in `ls $EXECDIR/../src/[SM]F[A-Z][a-z]*Item.h | grep -v SFMF | grep -v Command` ; do
   echo "#include \"`basename $i .h`.h\"" >> $OUTPUT4
done

cat >> $OUTPUT4 << EOT

#define CreateFieldViewItem(ret, type, view) \\
    switch (type) { \\
EOT

for i in `ls $EXECDIR/../src/[SM]F[A-Z][a-z]*.h | grep -v Item | grep -v SFMF | grep -v Command` ; do
  TYPE=`basename $i .h`
  ENUM=`echo $TYPE | tr [a-z] [A-Z]`
  echo "     case $ENUM: \\"  >> $OUTPUT4
  echo "       ret = new $TYPE""Item(view);\\"  >> $OUTPUT4
  echo "       break;\\"   >> $OUTPUT4
done

cat >> $OUTPUT4 << EOT
      default: \\
        ret = new FieldViewItem(view); \\
        break; \\
    }

EOT
