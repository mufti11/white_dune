#!/bin/sh
# This file is free software; the Free Software Foundation
# gives unlimited permission to copy, distribute and modify it.

for i in `find .. -name '*.wrl'` 
do 
   FILE=`basename $i`
   if test "$FILE" != "Dampers.wrl" ; then
      if test "$FILE" != "manta2.wrl" ; then
         FILE1=/tmp/`basename $i .wrl`_1.x3d
         FILE2=/tmp/`basename $i .wrl`_2.x3d 
         (../bin/dune -x3d -o $FILE1 $i || exit 1)
         (../bin/dune -x3d -o $FILE2 $FILE1 || exit 1)
         NIX1=/tmp/`basename $i .wrl`_3.x3d
         NIX2=/tmp/`basename $i .wrl`_4.x3d
         diff -bBw $FILE1 $FILE2 || (
            gawk '{ for (i=1;i<=NF;i++) if ($i == "-0") print "0"; else print $i;}' $FILE1 > $NIX1 && 
            gawk '{ for (i=1;i<=NF;i++) if ($i == "-0") print "0"; else print $i;}' $FILE2 > $NIX2 && 
            mgdiff $NIX1 $NIX2)
         rm -f $FILE1 $FILE2 $NIX1 $NIX2 || exit 1
      fi
   fi
done

