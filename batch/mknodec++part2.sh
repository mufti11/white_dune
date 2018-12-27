#!/bin/sh 

# This file is free software; the Free Software Foundation    
# gives unlimited permission to copy, distribute and modify it.

if test $# != 1 ;then
   echo usage: $0 nodename 1>&2
   exit 1
fi
NODENAME=$1

cat << EOT
#include "DuneApp.h"

Proto$NODENAME::Proto$NODENAME(Scene *scene)
  : Proto(scene, "$NODENAME")
{
EOT
