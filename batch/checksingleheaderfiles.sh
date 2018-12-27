#!/bin/sh

# This file is free software; the Free Software Foundation
# gives unlimited permission to copy, distribute and modify it.

for i in *.h ; do if test ! -f `basename $i .h`.cpp ; then if test ! -f `basename $i .h`.c ; then echo $i ; fi ; fi ; done 
