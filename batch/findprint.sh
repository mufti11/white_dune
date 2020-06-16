#!/bin/sh

# This file is free software; the Free Software Foundation    
# gives unlimited permission to copy, distribute and modify it.

DIR=`dirname $0`/../src
SWT=$DIR/swt
awk '/^print/ {print FILENAME}' $DIR/*.h $DIR/*.c* $SWT/*/*.h $SWT/*/*.c* | uniq | xargs echo
awk '/^fprintf\(stderr,/ {print FILENAME}' $DIR/*.h $DIR/*.c* $SWT/*/*.h $SWT/*/*.c* | uniq | xargs echo

