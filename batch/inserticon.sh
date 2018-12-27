#!/bin/bash

# This file is free software; the Free Software Foundation    
# gives unlimited permission to copy, distribute and modify it.

HERE=`dirname $0`
export TARGET=
sh $HERE/inserticonWithoutTarget.sh $*
#export TARGET=_bw
#sh $HERE/inserticonWithoutTarget.sh $*
