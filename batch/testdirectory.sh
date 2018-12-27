#!/bin/sh

# This file is free software; the Free Software Foundation
# gives unlimited permission to copy, distribute and modify it.

if test "$#" != "1" ; then
   echo Usage $0 directory_with_x3dv_files 1>&2
   exit 1
fi

for i in `find $1 -name '*.x3dv' -print` ; do  
   echo $i
   ../batch/gdbstart -x "run $i" -x "where" -x "quit" 
done

