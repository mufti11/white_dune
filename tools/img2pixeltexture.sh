#!bin/sh

# image file with RGB values (e.g. png, jpg etc) 
# to 
# VRML97 pixeltexture converter

# input: imagefile
# output: standardout 

# Copyright (C) 2003 J. "MUFTI" Scheurich
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

if test $# != 1; then
    echo usage: $0 file 1>&2
    exit 1
fi

cat << EOT
#VRML V2.0 utf8

    Shape
      {
      appearance       Appearance
        {  
        material        Material
          {
          }
        texture         PixelTexture
          {
          image    
EOT

anytopnm $1 | pnmtoplainpnm | awk '
     {
     line++;
     if (line==2) 
        print $1,$2,3; 
     if (line>3) 
        for (i=1;i<=NF;i+=3) 
           print 256*256*$i+256*$(i+1)+$(i+2);
     }'   


cat << EOT
          }
        }
      }     
EOT
