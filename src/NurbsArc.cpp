/*
 * NurbsArc.cpp
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

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#ifndef _STDAFX_H
#include "stdafx.h"
#endif

#ifndef _WIN32
# include "stdlib.h"
#endif

#ifndef _NURBSARC_H
#include "NurbsArc.h"
#endif
#ifndef _NURBS_CURVE_DEGREE_ELEVATE_H
#include "NurbsCurveDegreeElevate.h"
#endif

//definition of "numerical 0", change it as you like (and the program keeps working...)
#define NUMZERO 1e-6

NurbsArc::NurbsArc(int narcs, float degree, Vec3f sPoint, Vec3f point, Vec3f vector, int pDegree)
{
  m_valid = true;
  /*Build an arc from NURBS somewhere in space, but orthogonal to a straight line
    defined by point and vector. sPoint is the starting point of the arc. 
    The algorithm has been adapted from Piegl/Tiller "The NURBS Book", 
    2nd ed., p. 346,
  */

  /*
  if(pDegree<2){
    assert((pDegree<2) != 0);
    pDegree = 2;
  }
  
  if(degree>=(180*narcs)){
    assert((degree>=(180*narcs)) != 0);
    narcs = narcs + 1; //Otherwise it may happen, that a arc is build with one point at 
                       //infinity (-->weight=(cos(M_PI))). 
  }
  */

  Vec3f rPoint, X, Y, P0, T0, P1, P2, T2;
  int index;
  float r;
  float angle = 0;
  float rDegree = degree / 180 * M_PI;
  float delta = (float) (rDegree/narcs);
  float w1 = cos(delta / 2);
  Array<float> cosines, sines;

  for(int i=1; i<=narcs; i++){
    angle = angle + delta;
    cosines[i] = cos(angle);
    sines[i] = sin(angle);
  }
    
  rPoint = PointToLine(point, vector, sPoint);
  X = sPoint - rPoint;
  r = X.length();
  
  if(r<NUMZERO){              
    /* Points that are have a distance < NUMZERO from axis of rotation
       will be considered to be on the axis
    */
    points[0] = rPoint;
    weights[0] = 1;
    index = 0;
    for(int i=1; i<=narcs; i++){
      points[index+1] = rPoint;
      points[index+2] = rPoint;
      weights[index+1] = w1;
      weights[index+2] = 1;
      index = index + 2;
    }
  }
  else{
    X.normalize();
    Y = vector.cross(X);
    P0 = sPoint;
    T0 = Y;
    points[0] = P0;
    weights[0] = 1;
    index = 0;
    angle = 0;
    for(int i=1; i<=narcs; i++){
      P2 = rPoint + X * r * cosines[i]  + Y * r * sines[i];
      points[index+2] = P2;
      weights[index+2] = 1;
      T2 = X * (-sines[i]) + Y * cosines[i];
      //Lines intersect?
      if (Intersect3DLines(P0, T0, P2, T2) != 1) {
           m_valid = false;
           return;
      }
      points[index+1] = m_intersection;
      weights[index+1] = w1;
      P0 = P2;
      T0 = T2;
      index = index + 2;
    }
  }
  
  //setup knotvector
  makeKnotvector(narcs);

  //if pDegree>2 degree elevate arc
  
  if(pDegree>2)
    {  
    int dimension = getPointSize();
    int upDegree = pDegree - 2;
    int deg = 2;
    int k;

    k = getPointSize();
    
    Vec3f *tPoints = new Vec3f[k];
    float *tWeights = new float[k];
      
    for(int i=0; i<k; i++){
      tPoints[i] = getControlPoints(i);
      tWeights[i] = getWeights(i);
    }

    k = knots.size();
    
    Array<float> tKnots(k);

    for(int i=0; i<k; i++){
      tKnots[i] = getKnots(i);
    }   

    NurbsCurveDegreeElevate elevatedArc(tPoints, tWeights, tKnots, dimension, 
                                        deg, upDegree);

    for(int i=0; i<elevatedArc.getPointSize(); i++){
      points[i] = elevatedArc.getControlPoints(i);
      weights[i] = elevatedArc.getWeights(i);
    }
    
    for(int i=0; i<elevatedArc.getKnotSize(); i++){
      knots[i] = elevatedArc.getKnots(i);
    }
  }
}

int
NurbsArc::Intersect3DLines(Vec3f &P1, Vec3f &T1, Vec3f &P2, Vec3f &T2)
{
  Vec3f N;
  
  float t, det;
  
  if(T1==T2){
    return 0;
  }

  det = T1.x * T2.y * (P2.z - P1.z) + T1.y * T2.z * (P2.x - P1.x) + T1.z * T2.x * (P2.y - P1.y) -
    ((P2.x - P1.x) * T2.y * T1.z + (P2.y - P1.y) * T2.z * T1.x + (P2.z - P1.z) * T2.x *T1.y);

  if(fabs(det)>(NUMZERO)){
    return 0;
  }
  

  N = (T1.cross(T2)).cross(T2);
  t = N.dot(P2-P1) / N.dot(T1);
  m_intersection = P1 + T1 * t;

  return 1 ;
}

Vec3f 
NurbsArc::PointToLine(Vec3f lPoint, Vec3f lVector, Vec3f point)
{
  Vec3f rPoint;
  float t;

  assert(lVector.length()>NUMZERO);
  t = (lVector.dot(point - lPoint)) / (lVector.length()*lVector.length());
  
  rPoint = lPoint + lVector * t;
  
  return rPoint;
}


void 
NurbsArc::makeKnotvector(int narcs)
{
  
  int n = 2 * narcs;  
  int j = n + 1; 
  for(int i=0; i<3; i++){ 
    knots[i] = 0;
    knots[i+j] = 1;
  }
  int index = 3;
  float k = 1;
  for(int i=1; i<narcs; i++){
    knots[index] = k / narcs;
    knots[index+1] = k / narcs;
    index = index + 2;
    k = k + 1;
  }
  
}

