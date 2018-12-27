#!/bin/sh

# This file is free software; the Free Software Foundation    
# gives unlimited permission to copy, distribute and modify it.

if test $# != 3 ;then
   echo usage: $0 newurl oldurl file 1>&2
   exit 1
fi

TMPFILE=/tmp/replace_url$$

gawk -vnewurl="$1" -voldurl="$2" '{gsub(oldurl, newurl);print $0}' $3 > $TMPFILE &&
cp $TMPFILE $3 && rm -f $TMPFILE

