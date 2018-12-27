# creaseAngleChange.awk

# a simple script to change the creaseAngle field of all IndexedFaceSets
# in a VRML file

# Important: You need to load and store to a different file with the program 
#            white_dune before use, this script depends on white_dune layout 

# example usage:

# awk -v newangle=1.57 -f creaseAngleChange.awk inputfile.wrl

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

/IndexedFaceSet/ { 
                 faceSetFlag = 1;
                 braceCount = 0;
                 }

/{/              { 
                 if (faceSetFlag) 
                    {
                    faceSetStarted = 1; 
                    braceCount++;
                    }
                 }

/creaseAngle.*/  { 
                 if (faceSetStarted)
                    { 
                    creaseAngleLine = 1;
                    faceSetHasCreaseAngle = 1;
                    }
                 print "creaseAngle " newangle 
                 } 

/}/              { 
                 braceCount--;
                 if (faceSetFlag == 1)
                    if (braceCount == 0)
                       {
                       faceSetFlag = 0;                             
                       if (!faceSetHasCreaseAngle) 
                          print "creaseAngle " newangle; 
                       faceSetHasCreaseAngle = 0;
                       }
                 }

/^/              {
                 if (!creaseAngleLine)
                     print $0;
                 creaseAngleLine = 0;
                 }                    
