#!/bin/sh

# change creaseAngle in all IndexedFaceSet nodes of a VRML file

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

if test "$#" = "0" ; then
   echo Change the creaseAngle in the whole file
   echo Usage $0 newcreaseangle [ inputfile.wrl ] 1>&2
   exit 1
fi

NEW_CREASEANGLE=$1

shift

awk -v newangle=$NEW_CREASEANGLE  -f `dirname $0`/creaseangleChange.awk $*

