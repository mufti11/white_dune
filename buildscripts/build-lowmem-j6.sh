#!/bin/sh

# This file is free software; the Free Software Foundation    
# gives unlimited permission to copy, distribute and modify it.

cd `dirname $0`/..
export CFLAGS=
export CXXFLAGS=
make realclean
(./configure --without-devil && cd src && make -j6 duneLowMem)

