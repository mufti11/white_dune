/*
 * NurbsCurveDegreeElevate.h
 *
 * Copyright (C) 2004 Th. Rothermel
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

#ifndef _NURBS_SURFACE_DEGREE_ELEVATE_H
#define _NURBS_SURFACE_DEGREE_ELEVATE_H

#ifndef _VEC3F_H
#include "Vec3f.h"
#endif
#ifndef _ARRAY_H
#include "Array.h"
#endif

class NurbsSurfaceDegreeElevate {
public:
                 NurbsSurfaceDegreeElevate(Vec3f Points[], float Weights[], 
                                           Array<float> uKnots, 
                                           Array<float> vKnots, 
                                           int uDimension, int vDimension, 
                                           int uDegree, int vDegree,
                                           int uUpDegree, int vUpDegree);
    int          getUKnotSize() {return newUKnots.size();}
    int          getVKnotSize() {return newVKnots.size();}
    int          getPointSize() {return newPoints.size();}
    int          getWeightSize() {return newWeights.size();}
    Vec3f        getControlPoints(int index) {return newPoints[index];}
    float        getWeights(int index) {return newWeights[index];}
    float        getUKnots(int index) {return newUKnots[index];}
    float        getVKnots(int index) {return newVKnots[index];}
    int          getUDimension() {return newUDimension;}
    int          getVDimension() {return newVDimension;}

protected:
    Array<Vec3f> newPoints;
    Array<float> newWeights;
    Array<float> newUKnots;
    Array<float> newVKnots;
    int          newUDimension;
    int          newVDimension;
    
    Array<Vec3f> tempPoints;
    Array<float> tempWeights;
    //Array<float> tempUKnots;
};  
#endif

