#!/bin/sh

# This file is free software; the Free Software Foundation    
# gives unlimited permission to copy, distribute and modify it.

export OPTIM=-g
cd `dirname $0`/..
make realclean
./configure --without-devil --with-olpc --with-updatedebug && make

