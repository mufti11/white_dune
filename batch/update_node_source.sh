#!/bin/sh

EXECDIR=`dirname $0`

# generate templates of sourcefiles for not already existing nodes in white_dune
# from X3D specification component html files

if test "X_$CURRENT_X3D_URL" = "X_" ; then
   CURRENT_X3D_URL=http://www.web3d.org/files/specifications/19775-1/V3.3/
# reading from local stored standard
#   CURRENT_X3D_URL=../../ISO-IEC-19775-1.2-X3D-AbstractSpecification
fi

COMPONENTS=" \
CADGeometry \
core \
dis \
env_texture \
enveffects \
envsensor \
followers \
geodata \
geometry2D \
geometry3D \
group \
hanim \
interp \
keyboard \
layering \
layout \
lighting \
navigation \
networking \
nurbs \
particle_systems \
picking \
pointingsensor \
rendering \
rigid_physics \
scripting \
shaders \
shape \
sound \
text \
texture3D \
texturing \
time \
utils \
volume" 

for i in $COMPONENTS ; do
   lynx -dump $CURRENT_X3D_URL/Part01/components/$i.html | 
   awk -v path=$EXECDIR/ -f $EXECDIR/mknodesource.awk 
done


