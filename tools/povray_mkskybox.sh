#!/bin/sh

# make a vrml97 skybox from a povray file

# input file:    povrayfile as first argument of the commandline
# output files:  sky.wrl
#                sky_front.jpg sky_right.jpg sky_back.jpg 
#                sky_left.jpg  sky_top.jpg   sky_bottom.jpg
#
# Copyright (C) 2003 J. "MUFTI" Scheurich, based on a idea of Marc Schimmler
# 
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program (see the file "COPYING" for details); if 
# not, write to the Free Software Foundation, Inc., 675 Mass Ave, 
# Cambridge, MA 02139, USA.


SIZE=512
POVRAY_INC=/usr/local/src/povray31/include

TMPM4=/tmp/sky$$.m4

cat > $TMPM4 << EOT
camera{ location <0,0,0>
        up y
        right x
        angle 90
ifdef(\`sky_top',   \`look_at <            0, 32000,            0>')
ifdef(\`sky_bottom',\`look_at <            0,-32000,            0>')
ifdef(\`sky_left', \`look_at < 32000,            0,            0>')
ifdef(\`sky_right',  \`look_at <-32000,            0,            0>')
ifdef(\`sky_front', \`look_at <            0,            0, 32000>')
ifdef(\`sky_back',  \`look_at <            0,            0,-32000>')
}   
EOT

for i in _left _front _right _back _top _bottom; do
   TMPFILE=/tmp/sky_$i_$$.pov
   cp $1 $TMPFILE || exit 1
   # last camera wins 8-)
   m4 -Dsky''$i= $TMPM4 >> $TMPFILE || exit 1
   TMPTGA=/tmp/sky$i_$$.tga
   x-povray +D -I$TMPFILE -L$POVRAY_INC +FT -W$SIZE -H$SIZE +a0.0001 \
            -o $TMPTGA || exit 1
   rm -f $TMPFILE
   convert $TMPTGA sky$i.png || exit 1
   rm -f $TMPTGA
done

rm -f $TMPM4

cat > sky.wrl << EOT
#VRML V2.0 utf8

Background 
  {
  frontUrl 
    [
    "sky_front.png"
    ]
  rightUrl 
    [
    "sky_right.png"
    ]
  backUrl 
    [
    "sky_back.png"
    ]
  leftUrl 
    [
    "sky_left.png"
    ]
  topUrl 
    [
    "sky_top.png"
    ]
  bottomUrl 
    [
    "sky_bottom.png"
    ]
  }
EOT
