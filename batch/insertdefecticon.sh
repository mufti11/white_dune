#!/bin/bash

# This file is free software; the Free Software Foundation    
# gives unlimited permission to copy, distribute and modify it.

if test "$1" = "" ; then
   echo usage: $0 DUNE_NODE_TYPE 1>&2
   exit 1 
fi

SELF=`dirname $0`
export SELF

DIR=$SELF/../src/res

POS=`awk -v what=$1 '
(($1 ~ /VRML_/) || 
 ($1 ~ /X3D_/) || 
 ($1 ~ /COVER_/) || 
 ($1 ~ /KAMBI_/) || 
 ($1 ~ /DUNE_/)) {
  if (startcount) 
     count++
 } 
/enum NodeEnum/ {startcount = 1} 
{ 
if ($1 == what ",") {
   print (count - 1) * 16; 
   exit(0)
}
}' $SELF/../src/Node.h` &&
export POS

if test "$POS" = "" ; then
   echo $1 not found in Node.h  1>&2 
   exit 1
fi &&

DIST=`identify $SELF/../src/res/node_icons.bmp | 
     awk -v pos=$POS '{split($3,a,"x");print a[1]-pos}'` &&

TMP1_PNG=/tmp/node_icons_1_$$.png
TMP2_PNG=/tmp/node_icons_2_$$.png

for TARGET in "" _bw ; do 
    rm -f $TMP1_PNG $TMP2_PNG &&
    convert -crop $POS''x30+0+0 $DIR/node_icons$TARGET.bmp $TMP1_PNG &&
    convert -crop $DIST''x30+$POS+0 $DIR/node_icons$TARGET.bmp $TMP2_PNG &&
    convert -verbose +append $TMP1_PNG $DIR/empty_node_icon.bmp $TMP2_PNG  -format BMP -colorspace RGB -depth 4 -type palette $DIR/node_icons$TARGET.bmp
done
rm -f $TMP_PNG $TMP1_PNG $TMP2_PNG
convert $DIR/node_icons.bmp -scale 200% -set filter:blur 1 -format BMP -colorspace RGB -depth 4 -type palette $DIR/node_icons2.bmp
