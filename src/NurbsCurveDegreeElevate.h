/*
 * NurbsCurveDegreeElevate.h
 *
 * Copyright (C) 2003 Th. Rothermel
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

#ifndef _NURBS_CURVE_DEGREE_ELEVATE_H
#define _NURBS_CURVE_DEGREE_ELEVATE_H

#ifndef _VEC3F_H
#include "Vec3f.h"
#endif
#ifndef _ARRAY_H
#include "Array.h"
#endif

class NurbsCurveDegreeElevate {
public:
                  NurbsCurveDegreeElevate(Vec3f controlPoints[], 
                                          float weights[], MyArray<float> knots,
                                          int dimension, 
                                          int pDegree, int upDegree);
     int          getKnotSize() {return newKnots.size();}
     int          getPointSize() {return newPoints.size();}
     int          getWeightSize() {return newWeights.size();}
     Vec3f        getControlPoints(int index) {return newPoints[index];}
     float        getWeights(int index) {return newWeights[index];}
     float        getKnots(int index) {return newKnots[index];}

protected:
     MyArray<Vec3f> newPoints;
     MyArray<float> newWeights;
     MyArray<float> newKnots;
     MyArray<int>   binomi;

     void         makeBinomi(int n, int k);
     int          getBinomi(int n, int k){return(binomi[(n*max)+n+k]);}
     int          max;
     int          minimum(int a, int b);
     int          maximum(int a, int b);
};

#endif // _NURBS_CURVE_DEGREE_ELEVATE_H 
