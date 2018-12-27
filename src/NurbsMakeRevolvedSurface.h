/*
 * NurbsMakeRevolvedSurface.h
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

//#include "Vec2f.h"
#include "Vec3f.h"
#include "Array.h"

class NurbsMakeRevolvedSurface {
public:
                 NurbsMakeRevolvedSurface(Vec3f gControlPoints[], 
                                          float gWeights[], int gDimension, 
                                          int narcs, float arc, int rDegree, 
                                          Vec3f point, Vec3f vector);
    int          getKnotSize() {return knots.size();}
    int          getPointSize() {return points.size();}
    int          getWeightSize() {return weights.size();}
    float        getControlPoints(int index) {return points[index];}
    float        getWeights(int index) {return weights[index];}
    float        getKnots(int index) {return knots[index];}
    bool         isValid() {return valid;}

protected:
    Array<float> points;
    Array<float> weights;
    Array<float> knots;
    Vec3f        IntersectLines(Vec3f &P1, Vec3f &T1, Vec3f &P2, Vec3f &T2);
    bool         valid;     
};
