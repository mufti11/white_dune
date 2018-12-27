#!/bin/sh

# This file is free software; the Free Software Foundation
# gives unlimited permission to copy, distribute and modify it.

# some systems do really strange things on zero length classes
# this routine tries to find problematic classes

grep class `dirname $0`/../src/*.h | grep '{' | grep -v template | 
    grep -v StringBuf | grep -v Iterator | grep -v Pair | grep -v Stack |
    grep -v gmp | grep -v Vectoriser | grep -v SweepContext |
    grep -v AdvancingFront | grep -v shapes | grep -v Contour |
awk '
BEGIN { 
   print "#include \"DuneApp.h\"\n#include \"stdio.h\"\n" 
   }
   {
   split($1,a,":");
   print "#include \"" a[1] "\""; 
   class = $2; 
   if (class == "class") 
      class = $3;
   if ((class != "{") && (class != "UsePhysicsEngine"))
      list[l++]="//" $0 "\nif (sizeof(" class ")==0) { printf(\"zero length class " class "\");}"
   }
END {
   print "main() {";
   for (i=0;i<l;i++) 
      print list[i];
   print "exit(0);";
   print "}"
   }'
