#!/bin/sh

# This file is free software; the Free Software Foundation
# gives unlimited permission to copy, distribute and modify it.


OUTPUT=/tmp/file4doxygen_$$
mkdir $OUTPUT

PIPE=/tmp/pipe4doxygen_$$
mkdir $PIPE

DOXY=/tmp/Doxyfile_$$

for i in ../../src/*.cpp # ../../src/swt/include/*.h
do 
   cp $i $OUTPUT
done

for i in ../../src/ProtoMacros.h ../../src/*.h ../../src/*.cpp
do 
   gawk -v name=`basename $i .h` '
       {
       if ($0 ~ ".*ProtoMacros.h.*") 
          print $0;
       else
          {
          sub(/#[ \t]*include/,"// hidefromdoxygen include");              
          print $0;
          } 
       } 
   '  $i > $PIPE/`basename $i`
   gcc -I$PIPE -E -C -P $PIPE/`basename $i` |
   gawk '{sub("// hidefromdoxygen include","#include");print $0}' > $OUTPUT/`basename $i`
done

../../batch/replaceversion.sh REPLACE_WITH_VERSION < Doxyfile.in |
gawk -v dir=$OUTPUT '{sub("../../src/",dir);print $0}' > $DOXY
doxygen $DOXY

rm -rf $OUTPUT $PIPE $DOXY
