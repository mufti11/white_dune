#!/bin/sh

# This file is free software; the Free Software Foundation    
# gives unlimited permission to copy, distribute and modify it.

EXECDIR=`dirname $0`
SIZE1=`$1 $EXECDIR/../src/res/$2 2>&1 > /dev/null | awk '{ print $4;exit }'`
SIZE2=`$1 $EXECDIR/../src/res/$3 2>&1 > /dev/null | awk '{ print $4;exit }'`

if test "$SIZE1" != "$SIZE2" ; then
    echo size of $2: $SIZE1 not identical to size of $3: $SIZE2 
    exit 1
fi
exit 0
