#!/bin/bash

# This file is free software; the Free Software Foundation    
# gives unlimited permission to copy, distribute and modify it.

HERE=`dirname $0`

GIMP12=$HOME/.gimp-1.2/scripts
GIMP22=$HOME/.gimp-2.2/scripts
GIMP24=$HOME/.gimp-2.4/scripts
GIMP26=$HOME/.gimp-2.6/scripts
GIMP28=$HOME/.gimp-2.8/scripts

if `test -d $GIMP12`; then
  VERSION=-gimp1_2
  GIMP=$GIMP12
else 
  if `test -d $GIMP22`; then
    VERSION=-gimp2_2
    GIMP=$GIMP22
  else 
    if `test -d $GIMP24`; then
      VERSION=-gimp2_4
      GIMP=$GIMP24
    else 
      if `test -d $GIMP26`; then
        VERSION=-gimp2_6
        GIMP=$GIMP26
      else
        if `test -d $GIMP28`; then
          VERSION=-gimp2_8
          GIMP=$GIMP28
        else
          echo $HOME/.gimp/scripts not found 1>&2
          exit 1 
        fi
      fi
    fi
  fi
fi

if `test ! -d $GIMP`; then
  echo did not found gimp script directory 1>&2
  exit 1 
fi

cp $HERE/inserticon$VERSION.scm $GIMP/inserticon.scm

POSITION=`awk -v what=$1 '$1 ~ /VRML_/ {if (startcount) count++} /X3D_/ {if (startcount) count++} /DUNE_/ {if (startcount) count++} /COVER_/ {if (startcount) count++} /KAMBI_/ {if (startcount) count++} /X3DOM_/ {if (startcount) count++} /enum NodeEnum/ {startcount = 1} { if ($1 == what ",") {print count;exit(0)}}' $HERE/../src/Node.h`

if test "$POSITION" = "" ; then
   echo $1 not found in Node.h
   exit 1
fi
export ICON=\"/tmp/node_icons.bmp\"
cp $HERE/../src/res/node_icons$TARGET.bmp /tmp/node_icons.bmp &&
gimp --no-data --verbose -b "(script-fu-new-duneInsertIcon-batch $ICON $POSITION)" 
if test $? = 0 ;then
   mv /tmp/node_icons.bmp $HERE/../src/res/node_icons$TARGET.bmp
else
   echo gimp failed 1>&2
   exit 1
fi
