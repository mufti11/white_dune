#!/bin/sh

# This file is free software; the Free Software Foundation    
# gives unlimited permission to copy, distribute and modify it.

NPROC=4
if test -x /proc/cpuinfo; then 
     NPROC=`grep processor /proc/cpuinfo | wc -l`
fi
cd `dirname $0`
make realclean
# ubunutu has currently a non working CGAL, mageia 7 fail with OMP
./configure --without-devil --without-cgal --without-omp && make -j$NPROC
cd -
