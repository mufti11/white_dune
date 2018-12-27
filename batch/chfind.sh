#!/bin/sh

# This file is free software; the Free Software Foundation
# gives unlimited permission to copy, distribute and modify it.

if test "$#" = "0" ; then
   echo Usage $0 arguments to grep command and string to find 1>&2
   exit 1
fi

find . -name '*.[ch]*' -print | xargs grep $*
exit $?
