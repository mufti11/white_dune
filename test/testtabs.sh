#!/bin/sh

# This file is free software; the Free Software Foundation
# gives unlimited permission to copy, distribute and modify it.

DIR=`dirname $0`/../src
find $DIR -name '*.[ch]*' -print | grep -v lexer.cpp | grep -v parser.cpp |
grep -v resource.c | grep -v y.tab. | grep -v rcparse.c | grep -v ',v' | grep -v main.cpp |
xargs awk '/\t/ {print("tab character in line " FNR " of " FILENAME);exit(1)}' 1>&2 || exit $?

awk '/\t/ {print("tab character in line " FNR " of " FILENAME);exit(1)}' $DIR/dune.*.rc 1>&2 
exit $?

