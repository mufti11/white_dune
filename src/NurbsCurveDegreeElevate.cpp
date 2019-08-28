/*
 * NurbsCurveDegreeElevate.cpp
 *
 * -------------------------------------------------------------------
 * Portions of the code presented here have been adapted from the fine
 * work of the nurbs++ project, maintained by Philippe Lavoie as of 
 * today, June, 28th 2003
 * http://libnurbs.sourceforge.net/
 * -------------------------------------------------------------------
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
#include "NurbsCurveDegreeElevate.h"

#ifndef _ARRAY2D_H
#include "Array2D.h"
#endif
#ifndef _VEC4F_H
#include "Vec4f.h"
#endif

//definition of "numerical 0", change it as you like (and the program keeps working...)
#define NUMZERO 1e-6

NurbsCurveDegreeElevate::NurbsCurveDegreeElevate(Vec3f controlPoints[], 
      float weights[], MyArray<float> knots, int dimension, 
      int pDegree, int upDegree)
{
  newPoints.resize(0);
  newWeights.resize(0);
  newKnots.resize(0);
  binomi.resize(0);  

  if(upDegree<1) {return;}

  MyArray<Vec4f> hControlPoints; 
  MyArray<Vec4f> newHPoints;

  int p = pDegree;
  int t = upDegree;
  int m = dimension + p + 1;
  int ph = p + t;
  int ph2 = ph / 2;
  

  //build homogene coordinates
  for(int i=0; i<dimension; i++){
    hControlPoints[i].x = controlPoints[i].x * weights[i];
    hControlPoints[i].y = controlPoints[i].y * weights[i];
    hControlPoints[i].z = controlPoints[i].z * weights[i];
    hControlPoints[i].w = weights[i];
  }
  
  //workaround, needed later to break loop for sure
  float *tmp = new float[m+1];
  for(int i=0; i<m; i++){
    tmp[i] = knots[i];
  }
  tmp[m] = tmp[m-1];
  knots.resize(m+1);
  for(int i=0; i<m+1; i++){
    knots[i] = tmp[i];
  } 
  delete[] tmp;
  tmp = NULL;
  //end workaround
  

    //  knots[m] = knots[m-1]; //workaround, needed later to break loop for sure

  array2D<float> bezalfs(p+t+1, p+1);
  Vec4f *bpts = new Vec4f[p+1]; // pth-degree Bezier control points of the current segment
  Vec4f *ebpts = new Vec4f[p+t+1]; // (p+t)th-degree Bezier control points of the  current segment
  Vec4f *Nextbpts = new Vec4f[p-1]; // leftmost control points of the next Bezier segment
  float *alphas = new float[p-1]; // knot instertion alphas.

  // Compute the binomial coefficients
  makeBinomi(ph+1,ph2+1);
  
  // Compute Bezier degree elevation coefficients
  float inv;
  int mpi;
  bezalfs.set(0, 0, 1);
  bezalfs.set(ph, p, 1);
  for(int i=1; i<=ph2; i++){
    inv = 1.0/(float)getBinomi(ph,i);
    mpi = minimum(p,i);
    for(int j=maximum(0,i-t); j<=mpi; j++){
      bezalfs.set(i, j, (inv*((float)getBinomi(p,j))*((float)getBinomi(t,i-j))));
    }
  }

  for(int i=(ph2+1); i<=(ph-1); i++){
    mpi = minimum(p,i);
    for(int j=maximum(0,i-t); j<=mpi; j++)
      bezalfs.set(i, j, bezalfs.get((ph-i), (p-j)));
  }

  int mh = ph;
  int kind = ph + 1;
  float ua = knots[0];
  float ub;// = 0;
  int r = -1; 
  int oldr;
  int a = p;
  int b = p + 1; 
  int cind = 1;
  int rbz, lbz = 1; 
  int mul, save, s;
  float alf;
  int first, last, kj;
  float bet;
  float den, gam, numer;

  newHPoints[0] = hControlPoints[0];
  for(int i=0; i<=ph; i++){
    newKnots[i] = ua;
  }

  // Initialize the first Bezier segment

  for(int i=0; i<=p; i++){ 
    bpts[i] = hControlPoints[i];
  }
  int i;
  while(b<m){ // Big loop thru knot vector
    i = b;
    while((b<m) && (knots[b] == knots[b+1])){
      b++;
    }
    mul = b - i + 1; 
    mh  = mh + mul + t;
    ub = knots[b];
    oldr = r;
    r = p - mul;
    if(oldr>0){
      lbz = (oldr + 2) / 2;
    }
    else{
      lbz = 1;
    }
    if(r>0) {
      rbz = ph - (r + 1) / 2;
    }
    else{
      rbz = ph;
    }
    if(r>0){ // Insert knot to get Bezier segment
      numer = ub - ua;
      for(int k=p; k>mul; k--){
        alphas[k-mul-1] = numer/(knots[a+k]-ua);
      }
      for(int j=1; j<=r; j++){
        save = r - j; 
        s = mul + j;
        for(int k=p; k>=s; k--){
          bpts[k] = bpts[k] * alphas[k-s] + bpts[k-1] * (1.0-alphas[k-s]);
        }
        Nextbpts[save] = bpts[p];
      }
    }
    
    for(int i=lbz; i<=ph; i++){ // Degree elevate Bezier,  only the points lbz,...,ph are used
      ebpts[i].x = 0;
      ebpts[i].y = 0;
      ebpts[i].z = 0;
      ebpts[i].w = 0;
      mpi = minimum(p,i);
      for(int j=maximum(0,i-t); j<=mpi; j++){
        ebpts[i]= ebpts[i] + bpts[j] * bezalfs.get(i, j);
      }
    }

    if(oldr>1){ // Must remove knot u=c.U[a] oldr times
      // if(oldr>2) // Alphas on the right do not change
      // alfj = (ua-U[kind-1])/(ub-U[kind-1]) ;
      first = kind - 2; 
      last = kind;
      den = ub - ua;
      bet = (ub - newKnots[kind-1]) / den ;
      int i,j;
      for(int tr=1; tr<oldr; tr++){ // Knot removal loop
        i = first; 
        j = last;
        kj = j - kind + 1;
        while(j-i>tr){ // Loop and compute the new control points for one removal step
          if(i<cind){
            alf = (ub - newKnots[i]) / (ua - newKnots[i]);
            newHPoints[i] = newHPoints[i] * alf + newHPoints[i-1] * (1.0 - alf);
          }
          if(j>=lbz){
            if((j-tr)<=(kind-ph+oldr)){
              gam = (ub - newKnots[j-tr]) / den;
              ebpts[kj] = ebpts[kj] * gam + ebpts[kj+1] * (1.0 - gam);      
            }
            else{
              ebpts[kj] = ebpts[kj] * bet + ebpts[kj+1] * (1.0 - bet);
            }
          }
          i++; 
          j--; 
          kj--;
        }
        first--; 
        last++;
      }
    }

    if(a!=p){ // load the knot ua
      for(int i=0; i<(ph-oldr); i++){
        newKnots[kind] = ua;
        kind++;
      }
    }
    for(int j=lbz; j<=rbz; j++) { // load control points onto the curve
      newHPoints[cind] = ebpts[j];
      cind++;
    }
    if(b<m){ // Set up for next pass thru loop
      for(int j=0; j<r; j++){
        bpts[j] = Nextbpts[j];
      }
      for(int j=r; j<=p; j++){
        bpts[j] = hControlPoints[b-p+j];
      }
      a = b; 
      b++;
      ua = ub ;
    }
    else{
      for(int i=0; i<=ph; i++){
        newKnots[kind+i] = ub;
      }
    }
  }
   
  //Extract  weights and new controlpoints in cartesian system
  for(size_t i=0; i<(newHPoints.size()); i++){
    if(newHPoints[i].length()>NUMZERO){
      newPoints[i].x = newHPoints[i].x / newHPoints[i].w;
      newPoints[i].y = newHPoints[i].y / newHPoints[i].w;
      newPoints[i].z = newHPoints[i].z / newHPoints[i].w;
      newWeights[i]  = newHPoints[i].w;
    }
    else{
      newPoints[i].x = newHPoints[i].x;
      newPoints[i].y = newHPoints[i].y;
      newPoints[i].z = newHPoints[i].z;
      newWeights[i]  = newHPoints[i].w;
    } 
    //    newWeights[i]  = newHPoints[i].w;
  }
}

int
NurbsCurveDegreeElevate::minimum(int a, int b)
{
  if(a<b){return a;}
  else {return b;}
}

int
NurbsCurveDegreeElevate::maximum(int a, int b)
{  
  if(a>b){return a;}
  else {return b;}
}

void
NurbsCurveDegreeElevate::makeBinomi(int maxn, int maxk)
{
  /*
    Build Matrix of Binomial Coefficients /n\=/n-1\+/n-1\
                                          \k/ \k-1/ \ k /  
  */
  
  max = maximum(maxn, maxk);

  int n, k;

  //first line
  binomi[0] = 1;
  for(k=1; k<=max; k++){
    binomi[k] = 0;
  }

  //other lines
  for(n=1; n<=max; n++){
    binomi[(n*max)+n] = 1;
    for(k=1; k<=max; k++){      
      if(n<k){
        binomi[n*max+n+k] = 0;
      }
      else{
        binomi[n*max+n+k] = binomi[(n-1)*max+n+k-1] + binomi[(n-1)*max+n+k-2];
      }  
    }
  }
}

