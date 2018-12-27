#!/bin/sh

# This file is free software; the Free Software Foundation    
# gives unlimited permission to copy, distribute and modify it.

cd `dirname $0`/..
# compile time optimization for machines with less RAM and enough free diskspace
make realclean
export CFLAGS=
export CXXFLAGS=
./configure --without-devil --with-archives && make

