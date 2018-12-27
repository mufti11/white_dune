#!/bin/sh

# This file is free software; the Free Software Foundation    
# gives unlimited permission to copy, distribute and modify it.

AWK=awk

if test "X_`uname`" = "X_SunOS"; then
   if test -x /usr/xpg4/bin/awk ; then
      AWK=/usr/xpg4/bin/awk
   fi
fi      

EXECDIR=`dirname $0`
$AWK -v string=`(cd $EXECDIR && cd .. && basename $PWD)` \
     'BEGIN {\
             sub("wdune","",string);\
             sub("white_dune","",string);\
             sub("-", "",string);\
             print string}'
