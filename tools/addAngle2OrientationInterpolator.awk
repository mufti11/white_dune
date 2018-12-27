# addAngle2OrientationInterpolator.awk

# A simple script to add angle (in RAD) to all OrientationInterpolators
# in a VRML file

# Important: You need to load and store to a different file with the program 
#            white_dune before use, this script depends on white_dune layout 

# example usage:

# awk -vangle=3.141 -f addAngle2OrientationInterpolator.awk inputfile.wrl > outputfile.wrl

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

BEGIN {
      }

/OrientationInterpolator/ { orientFlag = 1; }

/}/              { 
                 if (orientFlag == 1) 
                    orientFlag = 0;
                 }

/keyValue/       {
                 if (orientFlag) 
                    {
                    keyValueFlag = 1;
                    }                    
                 }

/\]/             { 
                 if (orientFlag == 1) 
                    keyValueFlag = 0;
                 }

                 { 
                 if (orientFlag && keyValueFlag && (NF == 4))
                    print $1,$2,$3,$4 + angle;
                 else
                    print $0; 
                 }

