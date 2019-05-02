/*
 * NurbsCurve.cpp
 *
 * Copyright (C) 2003 Th. Rothermel, 2019 J. "MUFTI" Scheurich
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

#include "stdafx.h"
#include "NurbsCurve.h"
#include "Vec3f.h"

int
NurbsCurve::findSpan(int dimension, int order, float u, const float knots[])
{
    int low, mid, high;
    int n = dimension + order - 1;

    if (u >= knots[n]) {
        return n - order;
    }
    low = order - 1;
    high = n - order + 1;

    mid = (low + high) / 2;

    int oldLow = low;
    int oldHigh = high;
    int oldMid = mid;
    while (u < knots[mid] || u >= knots[mid+1]) {
        if (u < knots[mid]) {
            high = mid;
        } else {
            low = mid;
        }
        mid = (low+high)/2;
        // emergency abort of loop, otherwise a endless loop can occure
        if ((low == oldLow) && (high == oldHigh) && (mid == oldMid))
            break;

        oldLow = low;
        oldHigh = high;
        oldMid = mid;
    }
    return mid;
}

void
NurbsCurve::basisFuns(int span, float u, int order, const float knots[], 
                      float basis[], float deriv[])
{
    float *left = (float *) malloc(order * sizeof(float));
    float *right = (float *) malloc(order * sizeof(float));

    if ((left==NULL) || (right==NULL))
       return;
    basis[0] = 1.0f;
    for (int j = 1; j < order; j++) {
        left[j] = u - knots[span+1-j];
        right[j] = knots[span+j]-u;
        float saved = 0.0f, dsaved = 0.0f;
        for (int r = 0; r < j; r++) {
            float divisor = (right[r+1] + left[j-r]);
            float temp = 0;
            if (divisor != 0)                             
                temp = basis[r] / divisor;
            basis[r] = saved + right[r+1] * temp;
            deriv[r] = dsaved - j * temp;
            saved = left[j-r] * temp;
            dsaved = j * temp;
        }
        basis[j] = saved;
        deriv[j] = dsaved;
    }
    free(left);
    free(right);
}

Vec3f
NurbsCurve::curvePoint(int dimension, int order, const float knots[],
                       const Vec3f controlPoints[], const float weight[], 
                       float u)
{
    float *basis = (float *) malloc(order * sizeof(float));
    float *deriv = (float *) malloc(order * sizeof(float));

    if((basis==NULL) || (deriv==NULL)){
      return NULL;
    }
    
    int span = findSpan(dimension, order, u, knots);

    basisFuns(span, u, order, knots, basis, deriv);

    Vec3f C(0.0f, 0.0f, 0.0f);
    float w = 0.0f;
    for(int i=0; i<order; i++){
      C += controlPoints[span-order+1+i] * basis[i];
      w += weight[span-order+1+i] * basis[i];
    }   
    
    if (w != 0)
        C = C / w;

    free(basis);
    free(deriv);
    
    return C;
}


