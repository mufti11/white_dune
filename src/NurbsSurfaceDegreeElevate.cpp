/*
 * NurbsSurfaceDegreeElevate.cpp
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

#include "NurbsSurfaceDegreeElevate.h"
#include "NurbsCurveDegreeElevate.h"

#ifndef _VEC3F_H
#include "Vec3f.h"
#endif
#ifndef _ARRAY_H
#include "Array.h"
#endif

NurbsSurfaceDegreeElevate::NurbsSurfaceDegreeElevate(Vec3f Points[], 
      float Weights[], Array<float> uKnots, Array<float> vKnots,
      int uDimension, int vDimension, int uDegree, int vDegree,
      int uUpDegree, int vUpDegree)
{
  /*As a NURBS surface is a product surface, the already known function 
    "NurbsCurveDegreeElevate" is applied to the rows (u-direction) and
    columns (v-direction) of the surface control points separately.
    There is some extra memory needed for the storage of the temporary 
    surface after elevating in u-direction. For sure there is a more clever 
    way to implement this, but considering that a tesselated NURBS surface 
    uses far more memory than just the matrix of control points, this is
    not considered being grave.
  */
  
  newPoints.resize(0);
  newWeights.resize(0);
  newUKnots.resize(0);
  newVKnots.resize(0);
  tempPoints.resize(0);
  tempWeights.resize(0);

  //elevate u-direction
  int uKnotSize = uDimension + uDegree + 1;
  Vec3f *tRowPoint = new Vec3f[uDimension];
  float *tWeight = new float[uDimension];
  if(uUpDegree==0){ //case of no elevation in u-direction
    for(int i=0; i<(uDimension*vDimension); i++){
      tempPoints[i] = Points[i];
    }
    for(int i=0; i<(uDimension*vDimension); i++){
      tempWeights[i] = Weights[i];
    }
    for(int i=0; i<uKnotSize; i++){
      newUKnots[i] = uKnots[i];
    }
  } else{ 
    /*Loop through vector of points, extracting rows in u-direction,
      elevate these rows and write them into temporary matrix
    */
    for (int j=0; j<vDimension; j++){
      for (int i=0; i<uDimension; i++) {
        tRowPoint[i] = Points[(j*uDimension)+i] / Weights[(j*uDimension)+i];
        tWeight[i] = Weights[(j*uDimension)+i];
      }
      NurbsCurveDegreeElevate elevatedRow(tRowPoint, tWeight, uKnots, uDimension, uDegree, uUpDegree);
      int k = elevatedRow.getPointSize();
      for(int l=0; l<k; l++){
        tempPoints[(j*k)+l] = elevatedRow.getControlPoints(l) * elevatedRow.getWeights(l);
        tempWeights[(j*k)+l] = elevatedRow.getWeights(l);
      }
      k = elevatedRow.getKnotSize();
      for (int l=0; l<k; l++){
        newUKnots[l] = elevatedRow.getKnots(l);
      }
    }
  }
  
  //elevate v-direction
  newUDimension =  tempPoints.size() / vDimension;
  int vKnotSize = vDimension + vDegree + 1;
  Vec3f *tColumnPoint = new Vec3f[vDimension];
  delete[] tWeight;
  tWeight = NULL;
  tWeight = new float[vDimension];
  if(vUpDegree==0){ //case of no elevation in v-direction
    for(int i=0; i<(newUDimension*vDimension); i++){
      newPoints[i] = tempPoints[i];
    }
    for(int i=0; i<(newUDimension*vDimension); i++){
      newWeights[i] = tempWeights[i];
    }
    for(int i=0; i<vKnotSize; i++){
      newVKnots[i] = vKnots[i];
    }
    newVDimension = newPoints.size() / newUDimension;
  } else{
    /*Loop through vector of points, extracting columns in v-direction,
      elevate these columns and write them into output matrix
    */
    for(int j=0; j<newUDimension; j++){
      for(int i=0; i<vDimension; i++){
        tColumnPoint[i] = tempPoints[(i*newUDimension)+j] / 
                                     tempWeights[(i*newUDimension)+j];
        tWeight[i] = tempWeights[(i*newUDimension)+j];
      }
      NurbsCurveDegreeElevate elevatedColumn(tColumnPoint, tWeight, vKnots, vDimension, vDegree, vUpDegree);
      int k = elevatedColumn.getPointSize();
      for(int l=0; l<k; l++){
        newPoints[(l*newUDimension)+j] = elevatedColumn.getControlPoints(l) * elevatedColumn.getWeights(l);
        newWeights[(l*newUDimension)+j] = elevatedColumn.getWeights(l);
      }
      k = elevatedColumn.getKnotSize();
      for (int l=0; l<k; l++){
        newVKnots[l] = elevatedColumn.getKnots(l);
      }
    }
    newVDimension = newPoints.size() / newUDimension;
  }
}
