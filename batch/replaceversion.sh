#!/bin/sh

# This file is free software; the Free Software Foundation    
# gives unlimited permission to copy, distribute and modify it.

if test $# != 1 ;then
   echo usage: $0 'stringtoreplace < infile > outfile'
   exit 1
fi

EXECDIR=`dirname $0`
gawk -v version=`(cd $EXECDIR && cd .. && basename $PWD)` \
     -v replace=$1 -f $EXECDIR/replaceversion.awk
