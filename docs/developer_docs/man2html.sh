#!/bin/sh

# This file is free software; the Free Software Foundation
# gives unlimited permission to copy, distribute and modify it.

# unfortunatly, it is not easy, to find a simple working man2html command
# for the commandline

# so lets try polyglot man (rman/RosettaMan) first

#TYPE_RMAN=`type rman 2> /dev/null`
#if test "X_$TYPE_RMAN" != "X_" ; then 
#  rman -f HTML ../../man/$1
#else
  man2html ../../man/$1
#fi

# # you need a version of man2html that allow to process files outside the 
# # manpath 
# # this may need a change in the configuration of the man2html program
#
# # The version "man2html" 3.01 (written in perl) is not able to interpret
# # a troff formated man page like dune.1
#
#PATH1=`dirname $PWD`
#FILEPATH=`dirname $PATH1`
#RELPATH=`awk -v filepath=$FILEPATH 'BEGIN {print substr(filepath,2)}'`
#man2html -M / $RELPATH/man/$1

