#!/bin/sh

# This file is free software; the Free Software Foundation    
# gives unlimited permission to copy, distribute and modify it.

cd `dirname $0`/..
make realclean
./configure --without-devil --with-aflockdebug && make selftest

