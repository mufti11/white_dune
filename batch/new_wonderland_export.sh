#!/bin/sh
SELF=`dirname $0`
export SELF

if test $# != 2 ;then
   echo usage: $0 wonderlandModuleDirectory file.x3dv 1>&2
   exit 1
fi

rm -rf $1/exportX3d/`basename $2 .x3dv` &&
$SELF/../bin/dune -wonderland $1 $2 && 
cd $1/exportX3d/`basename $2 .x3dv` && 
ant deploy
exit $?

