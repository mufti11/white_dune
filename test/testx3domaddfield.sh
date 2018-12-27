#!/bin/sh

# This file is free software; the Free Software Foundation
# gives unlimited permission to copy, distribute and modify it.

DIR=`dirname $0`/../src
for i in $DIR/Node*.cpp ; do 
    grep FF_X3DOM_ONLY $i | 
    awk -v v=$i '{ split($0, a, "("); split(a[2], b, ","); print "grep -H " b[1] " " v}' |
    sh | grep addField 
done

