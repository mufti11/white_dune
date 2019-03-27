/*
 * NurbsMakeRevolvedSurface.cpp
 *
 * Copyright (C) 2003 Th.Rothermel
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program (see the file "COPYING" for details); if 
 * not, write to the Free Software Foundation, Inc., 675 Mass Ave, 
 * Cambridge, MA 02139, USA.
 */
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#ifndef _WIN32
# include "stdlib.h"
#endif

#include "stdafx.h"
#include "NurbsArc.h"
#include "NurbsMakeRevolvedSurface.h"


NurbsMakeRevolvedSurface::NurbsMakeRevolvedSurface(Vec3f gControlPoints[], float gWeights[], int gDimension, int narcs, float arc, int rDegree, Vec3f point, Vec3f vector)
{
  valid = true;
  /***ATTENTION: input variable "vector" has to be normalized!***/
  
  Vec3f sPoint, tmpControlPoint;
  sPoint.x = 0; sPoint.y = 0;  sPoint.z = 0;
  
  NurbsArc tmpArc(narcs, arc, sPoint, point, vector, rDegree);
  if (!tmpArc.isValid()){
    valid = false;
    return;
  }

  int rDimension;
  rDimension  = tmpArc.getPointSize();

  //rotate generatrix
  for (int j=0; j<gDimension; j++){
    NurbsArc tmpArc(narcs, arc, gControlPoints[j], point, vector, rDegree);
    if (!tmpArc.isValid()){
      valid = false;
      return;
    }
    for(int i=0; i<rDimension; i++){
      tmpControlPoint = tmpArc.getControlPoints(i);
      points[(j*rDimension+i)*3] = tmpControlPoint.x * tmpArc.getWeights(i) * 
                                   gWeights[j]; 
      points[(j*rDimension+i)*3+1] = tmpControlPoint.y * tmpArc.getWeights(i) * 
                                     gWeights[j];
      points[(j*rDimension+i)*3+2] = tmpControlPoint.z * tmpArc.getWeights(i) *
                                     gWeights[j];
      weights[j*rDimension + i] = tmpArc.getWeights(i) * gWeights[j];
    }  
  }

  sPoint.x = 0; sPoint.y = 0;  sPoint.z = 0;
  NurbsArc tmpArc2(narcs, arc, sPoint, point, vector, rDegree);
  if (!tmpArc2.isValid()){
    valid = false;
    return;
  }
  //load knotvector in direction of rotation
  for (int i=0; i<tmpArc2.getKnotSize(); i++){
    knots[i] = tmpArc2.getKnots(i);
  }  
}  
  

