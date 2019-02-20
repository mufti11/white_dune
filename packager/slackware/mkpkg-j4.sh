#!/bin/sh

# This file is free software; the Free Software Foundation    
# gives unlimited permission to copy, distribute and modify it.

DUNEMAKEFLAGS=-j4
export DUNEMAKEFLAGS
sh `dirname $0`/mkpkg.sh


