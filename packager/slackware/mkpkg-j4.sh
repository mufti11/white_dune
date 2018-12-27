#!/bin/sh

# This file is free software; the Free Software Foundation    
# gives unlimited permission to copy, distribute and modify it.


export CFLAGS=
export CXXFLAGS=
DUNEMAKEFLAGS=-j4
export DUNEMAKEFLAGS
sh `dirname $0`/mkpkg.sh


