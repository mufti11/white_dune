# collectViewpoints.awk

# A simple script to collect the parameters of all Viewpoints
# to a VRML file with a Transform animation of all Viewpoints.

# This script do not support the collections of Viewpoints which
# are moved/rotated/scaled etc. by Transform nodes.

# Important: You need to load and store to a different file with the program 
#            white_dune before use, this script depends on white_dune layout 

# example usage:

# awk -f collectViewpoints.awk inputfile.wrl > outputfile.wrl

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
      posIndex = 0;
      orientIndex = 0;
      viewPointIndex = 0;
      routeIndex = 0;
      lineIndex = 0;
      }

/Viewpoint/ { 
            viewPointFlag = 1;
            viewPointIndex++;
            }

/}/              { 
                 if (viewPointFlag == 1) 
                    {
                    viewPointFlag = 0;
                    if (orientIndex < viewPointIndex)
                       {
                       orientList[orientIndex,0]=0;
                       orientList[orientIndex,1]=0;
                       orientList[orientIndex,2]=1;
                       orientList[orientIndex,3]=0;
                       orientIndex++;
                       }                    
                    if (posIndex < viewPointIndex)
                       {
                       posList[posIndex,0]=0;
                       posList[posIndex,1]=0;
                       posList[posIndex,2]=-10;
                       posIndex++;
                       }
                    }                                              

                 }

/orientation/    {
                 if (viewPointFlag) 
                    {
                    orientList[orientIndex,0]=$2;
                    orientList[orientIndex,1]=$3;
                    orientList[orientIndex,2]=$4;
                    orientList[orientIndex,3]=-$5;
                    orientIndex++;
                    }                    
                 }

/position/       {
                 if (viewPointFlag) 
                    {
                    posList[posIndex,0]=-$2;
                    posList[posIndex,1]=-$3;
                    posList[posIndex,2]=-$4;
                    posIndex++;
                    }                    
                 }

/^/              { 
                 if ($1 == "ROUTE")
                    route[routeIndex++] = $0; 
                 else
                    line[lineIndex++] = $0; 
                 }

END              {
                 print("#VRML V2.0 utf8");

                 print("Viewpoint");
                 print("  {");
                 print("  position 0 0 0");
                 print("  }");

                 print("DEF POS PositionInterpolator");
                 print("  {");
                 print("  key");
                 print("    [");
                 for (i=0;i<posIndex;i++)
                     print("    " i/(posIndex-1));
                 print("    ]");
                 print("  keyValue");
                 print("    [");
                 for (i=0;i<posIndex;i++)
                     printf("    %f %f %f\n",
                            posList[i,0],posList[i,1],posList[i,2]);
                 print("    ]");
                 print("  }");

                 print("DEF ORIENT OrientationInterpolator");
                 print("  {");
                 print("  key");
                 print("    [");
                 for (i=0;i<orientIndex;i++)
                     print("    " i/(orientIndex-1));
                 print("    ]");
                 print("  keyValue");
                 print("    [");
                 for (i=0;i<orientIndex;i++)
                     printf("    %f %f %f %f\n",
                            orientList[i,0],orientList[i,1],orientList[i,2],
                            orientList[i,3]);
                 print("    ]");
                 print("  }");
                 print("DEF TIME TimeSensor");
                 print("  {");
                 print("  loop TRUE");
                 print("  cycleInterval 10");
                 print("  }");
                 print("DEF TRANSFORM1 Transform");
                 print("  {");
                 print("  children DEF TRANSFORM2 Transform");
                 print("    {");
                 print("    children");
                 print("      [");
                 for (i=0;i<lineIndex;i++)
                    print("      " line[i]);
                 print("      ]");
                 print("    }");
                 print("  }");
                 for (i=0;i<routeIndex;i++)
                    print(route[i]);
                 print("ROUTE TIME.fraction_changed TO ORIENT.set_fraction");
                 print("ROUTE ORIENT.value_changed TO TRANSFORM1.rotation");
                 print("ROUTE TIME.fraction_changed TO POS.set_fraction");
                 print("ROUTE POS.value_changed TO TRANSFORM2.translation");
                 }
