#!/bin/sh
# This script is free software; the Free Software Foundation
# gives unlimited permission to copy, distribute and modify it.

CXX=$1
shift

if test "$CXX" = "c++" ; then 
   mkdep="g++ -MM";
else 
   if test "$CXX" = "g++" ; then 
      mkdep="g++ -MM";
   else 
      if test "`uname`" = "SunOS" ; then
         mkdep="$CXX -xM1";
      else 
         if test "`uname`" = "HP-UX" ; then
            mkdep="$CXX +make -E";
         else
            mkdep="$CXX -M";
         fi
      fi
   fi
fi
$mkdep $*

