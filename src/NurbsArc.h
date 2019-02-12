/*
 * NurbsArc.h
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

#include "Vec2f.h"
#include "Vec3f.h"
#include "Array.h"

class NurbsArc {
public:
                    NurbsArc(int narcs, float degree, Vec3f sPoint, 
                             Vec3f point, Vec3f vector, int pDegree = 2); 
             Vec3f  getControlPoints(int index) {return points[index];}
             float  getWeights(int index) {return weights[index];}
             float  getKnots(int index) {return knots[index];}
             int    getKnotSize() {return knots.size();}
             int    getPointSize () {return points.size();}
             int    getWeightSize() {return weights.size();}
             bool   isValid() { return m_valid; }

protected:
                    MyArray<Vec3f> points;
                    MyArray<float> weights;
                    MyArray<float> knots;
             void   makeKnotvector(int narcs);
             int    Intersect3DLines(Vec3f &P1, Vec3f &T1, Vec3f &P2, Vec3f &T2);
             Vec3f  PointToLine(Vec3f lPoint, Vec3f lVector, Vec3f Point);
             Vec3f  m_intersection;
             bool   m_valid;
};
