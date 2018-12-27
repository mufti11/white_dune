#!/bin/sh

# This file is free software; the Free Software Foundation    
# gives unlimited permission to copy, distribute and modify it.

if test "X_$EDITOR" = "X_" ; then
    EDITOR=me
fi

DIR=`dirname $0`/../src
awk -v e=$EDITOR '/\t/ {system(e " +" FNR " " FILENAME);filenam=FILENAME}
                  {if (filenam!="" && filenam!=FILENAME) exit(1)}' `find $DIR -name '*.[ch]*' -print | grep -v lexer.cpp | grep -v parser.cpp | grep -v resource.c | grep -v y.tab.h | grep -v rcparse.c` 
