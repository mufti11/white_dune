#!/bin/sh

# This file is free software; the Free Software Foundation    
# gives unlimited permission to copy, distribute and modify it.

if test $# != 2 ;then
   echo usage: $0 newurl oldurl 1>&2
   exit 1
fi

EXECDIR=`dirname $0`

$EXECDIR/replace_url.sh $1 $2 ../configure.in
$EXECDIR/replace_url.sh $1 $2 ../INSTALL
$EXECDIR/replace_url.sh $1 $2 ../src/swt/include/swt.h

find $EXECDIR/.. -name '*.html' -print | xargs -l $EXECDIR/replace_url.sh $1 $2
find $EXECDIR/.. -name '*.1' -print | xargs -l $EXECDIR/replace_url.sh $1 $2

