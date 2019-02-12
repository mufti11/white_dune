/*
 * NodeExtrusion.cpp
 *
 * Copyright (C) 1999 Stephen F. White, 2004 Wu qingwei, 2009 J. "MUFTI" Scheurich
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

/* 
  - code of computeExtrusion() 
    based on Viewer.cpp of OpenVRML
  - code of Vset(), Vscale(), Vdot(), Vlength(),Vdiff(),VCross(),Vnorm()
    from MathUtil.h/cpp of OpenVRML

  additional patches in computeExtrusion() from the Viewer.cpp of the 
  Orbisnap free vrml viewer

//
// Copyright (C) 1998  Chris Morley
// Copyright 1998-2004 HUMUSOFT s.r.o.
// 
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
// 
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

  - cap creation of createMesh() 
    based on ExtrusionNode.cpp of "CyberX3D for C++"
 
  Copyright (C) 2002-2003 Satoshi Konno
  All rights reserved.

  Redistribution and use in source and binary forms, with or without 
  modification, are permitted provided that the following conditions are met:

  1. Redistributions of source code must retain the above copyright notice, 
     this list of conditions and the following disclaimer. 

  2. Redistributions in binary form must reproduce the above copyright notice, 
     this list of conditions and the following disclaimer in the documentation 
     and/or other materials provided with the distribution. 

  3. The name of the author may not be used to endorse or promote products 
     derived from this software without specific prior written permission. 

  THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
  WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
  MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO 
  EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; 
  OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, 
  WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR 
  OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF 
  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include <stdio.h>
#include "stdafx.h"
#include <math.h>
#ifndef FLT_MAX
# include <float.h>
#endif

#include "NodeExtrusion.h"
#include "Proto.h"
#include "FieldValue.h"
#include "SFBool.h"
#include "SFFloat.h"
#include "MFVec2f.h"
#include "MFVec3f.h"
#include "MFRotation.h"
#include "MFFloat.h"
#include "SFString.h"
#include "MyMesh.h"
#include "Field.h"
#include "RenderState.h"
#include "Scene.h"
#include "resource.h"

#define FPZERO(n) fpzero(n)

ProtoExtrusion::ProtoExtrusion(Scene *scene)
  : GeometryProto(scene, "Extrusion")
{
    float      *v;

    beginCap.set(
          addField(SFBOOL, "beginCap", new SFBool(true)));

    ccw.set(
          addField(SFBOOL, "ccw", new SFBool(true)));

    convex.set(
          addField(SFBOOL, "convex", new SFBool(true)));

    creaseAngle.set(
          addField(SFFLOAT, "creaseAngle", new SFFloat(0.0f), 
                   new SFFloat(0.0f)));

    float values[] = { 
            1.0,  1.0, 
            1.0, -1.0, 
           -1.0, -1.0, 
           -1.0,  1.0, 
            1.0,  1.0 
    };
    v = new float[10];  
    memcpy(v, values, 10 * sizeof(float));
    crossSection.set(
          addField(MFVEC2F, "crossSection", new MFVec2f(v, 10)));

    endCap.set(
          addField(SFBOOL, "endCap", new SFBool(true)));

    float values2[] = { 0.0, 0.0, 1.0, 0.0 };
    v = new float[4];  
    memcpy(v, values2, 4 * sizeof(float));
    orientation.set(
          addField(MFROTATION, "orientation", new MFRotation(v, 4)));

    float values3[] = { 1.0, 1.0 };
    v = new float[2];  
    memcpy(v, values3, 2 * sizeof(float));
    scale.set(
          addField(MFVEC2F, "scale", new MFVec2f(v, 2), new SFFloat(0.0f)));
    
    solid.set(
          addField(SFBOOL, "solid", new SFBool(true)));

    float values4[] = { 0.0, 0.0, 0.0, 0.0, 1.0, 0.0 };
    v = new float[6];  memcpy(v, values4, 6 * sizeof(float));
    spine.set(
          addField(MFVEC3F, "spine", new MFVec3f(v, 6)));

    texCoord.set(
          addExposedField(SFNODE, "texCoord", new SFNode(NULL), 
                          GENERATED_TEXTURE_COORDINATE_NODE));
    setFieldFlags(texCoord, FF_KAMBI_ONLY);

    x3domGeometryCommonFields()

    addEventIn(MFVEC2F, "set_crossSection", 0, crossSection);
    addEventIn(MFROTATION, "set_orientation", 0, orientation);
    addEventIn(MFVEC2F, "set_scale", 0, scale);
    addEventIn(MFVEC3F, "set_spine", EIF_RECOMMENDED, spine);
}

Node *
ProtoExtrusion::create(Scene *scene)
{ 
    return new NodeExtrusion(scene, this); 
}

NodeExtrusion::NodeExtrusion(Scene *scene, Proto *def)
  : MeshBasedNode(scene, def)
{
}

const float FPTOLERANCE(1.0e-6);

bool fpzero(const float f) {
    return (fabs(f) <= FPTOLERANCE);
}


// Vector ops

#define Vset( V, A ) \
 ((V[0]=A[0]),(V[1]=A[1]),(V[2]=A[2]))             // V <= A
#define Vscale( V, s) \
 ((V[0]*=(s)),(V[1]*=(s)),(V[2]*=s))               // V *= s
#define Vdot( A, B ) \
 (A[0]*B[0]+A[1]*B[1]+A[2]*B[2])                   // A.B

double Vlength( float V[3] )
{
  double vlen = sqrt(V[0]*V[0]+V[1]*V[1]+V[2]*V[2]);
  return (fpzero(vlen) ? 0.0 : vlen);
}

double Vlength2( float V[3] )
{
  double vlen2 = V[0]*V[0]+V[1]*V[1]+V[2]*V[2];
  return (fpzero(vlen2) ? 0.0 : vlen2);
}

void Vdiff( float V[3], const float A[3], const float B[3] )
{
  V[0] = A[0] - B[0];
  V[1] = A[1] - B[1];
  V[2] = A[2] - B[2];
}

void Vcross( float V[3], const float A[3], const float B[3] )
{
  float x,y,z; // Use temps so V can be A or B
  x = A[1]*B[2] - A[2]*B[1];
  y = A[2]*B[0] - A[0]*B[2];
  z = A[0]*B[1] - A[1]*B[0];
  V[0] = x;
  V[1] = y;
  V[2] = z;
}

void Vnorm( float V[3] )
{
  float vlen = (float) sqrt(V[0]*V[0]+V[1]*V[1]+V[2]*V[2]);
  if (! fpzero(vlen))
    {
      V[0] /= vlen;
      V[1] /= vlen;
      V[2] /= vlen;
    }
}

float myAcos(float c) 
{
    float x = c;
    while (x < -1)
        x = fmod(x - 1.0, 2.0) + 1;
    while (x > 1)
        x = fmod(x + 1.0, 2.0) - 1;
    return acos(x);
}

// Note that these matrices are stored in natural (C) order (the transpose
// of the OpenGL matrix). Could change this someday...

void Midentity( double M[4][4] )
{
  for (int i=0; i<4; ++i)
    for (int j=0; j<4; ++j)
      M[i][j] = (i == j) ? 1.0 : 0.0;
}

// Convert from axis/angle to transformation matrix GG p466

void Mrotation( double M[4][4], const float axisAngle[4] )
{
  float aa_norm[4];
  aa_norm[0] = axisAngle[0];
  aa_norm[1] = axisAngle[1];
  aa_norm[2] = axisAngle[2];
  aa_norm[3] = axisAngle[3];
  Vnorm( aa_norm );

  double s = sin(aa_norm[3]);
  double c = cos(aa_norm[3]);
  double t = 1.0 - c;
  double x = aa_norm[0];
  double y = aa_norm[1];
  double z = aa_norm[2];

  M[0][0] = t*x*x + c;
  M[0][1] = t*x*y - s*z;
  M[0][2] = t*x*z + s*y;
  M[0][3] = 0.0;
  M[1][0] = t*x*y + s*z;
  M[1][1] = t*y*y + c;
  M[1][2] = t*y*z - s*x;
  M[1][3] = 0.0;
  M[2][0] = t*x*z - s*y;
  M[2][1] = t*y*z + s*x;
  M[2][2] = t*z*z + c;
  M[2][3] = 0.0;
  M[3][0] = M[3][1] = M[3][2] = 0.0;
  M[3][3] = 1.0;
}

//  Build an extrusion.

// use ORBISNAP patches
#define __ORBISNAP__ 1
        
void computeExtrusion(int nOrientation,
                      const float *orientation,
                      int nScale,
                      const float *scale,
                      int nCrossSection,
                      const float *crossSection,
                      int nSpine,
                      const float *spine,
                      float *c,   // OUT: coordinates
                      float *tc  // OUT: texture coords 
                      /*, int *faces*/)     // OUT: face list
{
  // Xscp, Yscp, Zscp- columns of xform matrix to align cross section
  // with spine segments.
  float Xscp[3] = { 1.0, 0.0, 0.0};
  float Yscp[3] = { 0.0, 1.0, 0.0};
  float Zscp[3] = { 0.0, 0.0, 1.0};
  float lastZ[3];

  // Is the spine a closed curve (last pt == first pt)?
  bool spineClosed = (FPZERO(spine[ 3*(nSpine-1)+0 ] - spine[0]) &&
                      FPZERO(spine[ 3*(nSpine-1)+1 ] - spine[1]) &&
                      FPZERO(spine[ 3*(nSpine-1)+2 ] - spine[2]));

  // Is the spine a straight line?
  bool spineStraight = true;
  for (int i = 1; i < nSpine-1; ++i)
    {
      float v1[3], v2[3];
      v1[0] = spine[3*(i-1)+0] - spine[3*(i)+0];
      v1[1] = spine[3*(i-1)+1] - spine[3*(i)+1];
      v1[2] = spine[3*(i-1)+2] - spine[3*(i)+2];
      v2[0] = spine[3*(i+1)+0] - spine[3*(i)+0];
      v2[1] = spine[3*(i+1)+1] - spine[3*(i)+1];
      v2[2] = spine[3*(i+1)+2] - spine[3*(i)+2];
      Vcross(v1, v2, v1);
      if (Vlength(v1) != 0.0)
        {
          spineStraight = false;
          Vnorm( v1 );
          Vset( lastZ, v1 );
          break;
        }
    }

  // If the spine is a straight line, compute a constant SCP xform
  if (spineStraight)
    {
      float V1[3] = { 0.0, 1.0, 0.0}, V2[3], V3[3];
      V2[0] = spine[3*(nSpine-1)+0] - spine[0];
      V2[1] = spine[3*(nSpine-1)+1] - spine[1];
      V2[2] = spine[3*(nSpine-1)+2] - spine[2];
      Vcross( V3, V2, V1 );
      double len = Vlength(V3);
      if (len != 0.0)           // Not aligned with Y axis
        {
          Vscale(V3, 1.0/len);

          float orient[4];              // Axis/angle
          Vset(orient, V3);
          orient[3] = myAcos(Vdot(V1,V2));
          double scp[4][4];             // xform matrix
          Mrotation( scp, orient );
          for (int k=0; k<3; ++k) {
            Xscp[k] = scp[0][k];
            Yscp[k] = scp[1][k];
            Zscp[k] = scp[2][k];
          }
        }
    }

  // Orientation matrix
  double om[4][4];

#if defined(__ORBISNAP__)

  if (!FPZERO(orientation[3])) Mrotation(om, orientation);
  else Midentity(om);

#else

  if (nOrientation == 1 && ! FPZERO(orientation[3]) )
    Mrotation( om, orientation );

#endif

  // Compute coordinates, texture coordinates:
  for (int i = 0, ci = 0; i < nSpine; ++i, ci+=nCrossSection) {

    // Scale cross section
    for (int j = 0; j < nCrossSection; ++j) {
      c[3*(ci+j)+0] = scale[0] * crossSection[ 2*j ];
      c[3*(ci+j)+1] = 0.0;
      c[3*(ci+j)+2] = scale[1] * crossSection[ 2*j+1 ];
    }

    // Compute Spine-aligned Cross-section Plane (SCP)
    if (! spineStraight)
      {
        float S1[3], S2[3];     // Spine vectors [i,i-1] and [i,i+1]
        int yi1, yi2, si1, s1i2, s2i2;

        if (spineClosed && (i == 0 || i == nSpine-1))
          {
            yi1 = 3*(nSpine-2);
            yi2 = 3;
            si1 = 0;
            s1i2 = 3*(nSpine-2);
            s2i2 = 3;
          }
        else if (i == 0)
          {
            yi1 = 0;
            yi2 = 3;
            si1 = 3;
            s1i2 = 0;
            s2i2 = 6;
          }
        else if (i == nSpine-1)
          {
            yi1 = 3*(nSpine-2);
            yi2 = 3*(nSpine-1);
            si1 = 3*(nSpine-2);
            s1i2 = 3*(nSpine-3);
            s2i2 = 3*(nSpine-1);
          }
        else
          {
            yi1 = 3*(i-1);
            yi2 = 3*(i+1);
            si1 = 3*i;
            s1i2 = 3*(i-1);
            s2i2 = 3*(i+1);
          }

        Vdiff( Yscp, &spine[yi2], &spine[yi1] );
        Vdiff( S1, &spine[s1i2], &spine[si1] );
        Vdiff( S2, &spine[s2i2], &spine[si1] );

        Vnorm( Yscp );
        Vset(lastZ, Zscp);      // Save last Zscp
        Vcross( Zscp, S2, S1 );

        float VlenZ = Vlength(Zscp);
        if ( VlenZ == 0.0 )
          Vset(Zscp, lastZ);
        else
          Vscale( Zscp, 1.0/VlenZ );

        if ((i > 0) && (Vdot( Zscp, lastZ ) < 0.0))
          Vscale( Zscp, -1.0 );

        Vcross( Xscp, Yscp, Zscp );
      }

#if !defined(__ORBISNAP__)

    // Rotate cross section into SCP
    for (int j = 0; j < nCrossSection; ++j) {
      float cx, cy, cz;
      cx = c[3*(ci+j)+0]*Xscp[0]+c[3*(ci+j)+1]*Yscp[0]+c[3*(ci+j)+2]*Zscp[0];
      cy = c[3*(ci+j)+0]*Xscp[1]+c[3*(ci+j)+1]*Yscp[1]+c[3*(ci+j)+2]*Zscp[1];
      cz = c[3*(ci+j)+0]*Xscp[2]+c[3*(ci+j)+1]*Yscp[2]+c[3*(ci+j)+2]*Zscp[2];

      c[3*(ci+j)+0] = cx;
      c[3*(ci+j)+1] = cy;
      c[3*(ci+j)+2] = cz;
    }

#endif

    // Apply orientation
    if (! FPZERO(orientation[3]) )
      {
        if (nOrientation > 1)

#if defined(__ORBISNAP__)

        // rotate in the reverse angle
        {
          float ori[4];
          ori[0] = orientation[0];
          ori[1] = orientation[1];
          ori[2] = orientation[2];
          ori[3] = -orientation[3];
          Mrotation(om, ori);
        }

#else
          Mrotation( om, orientation );
#endif

        for (int j = 0; j < nCrossSection; ++j) {
          float cx, cy, cz;
          cx = c[3*(ci+j)+0]*om[0][0]+c[3*(ci+j)+1]*om[1][0]+c[3*(ci+j)+2]*om[2][0];
          cy = c[3*(ci+j)+0]*om[0][1]+c[3*(ci+j)+1]*om[1][1]+c[3*(ci+j)+2]*om[2][1];
          cz = c[3*(ci+j)+0]*om[0][2]+c[3*(ci+j)+1]*om[1][2]+c[3*(ci+j)+2]*om[2][2];

          c[3*(ci+j)+0] = cx;
          c[3*(ci+j)+1] = cy;
          c[3*(ci+j)+2] = cz;
        }
      }

#if defined(__ORBISNAP__)

    // According to VRML specs, we rotate the cross section into SCP
    // and then orient it. However, it seems the appropriate matrix
    // multiplications must be done in reverse order. Is it a rule? Maybe :o)

    // Rotate cross section into SCP
    for (int j = 0; j < nCrossSection; ++j) {
      float cx, cy, cz;
      cx = c[3*(ci+j)+0]*Xscp[0]+c[3*(ci+j)+1]*Yscp[0]+c[3*(ci+j)+2]*Zscp[0];
      cy = c[3*(ci+j)+0]*Xscp[1]+c[3*(ci+j)+1]*Yscp[1]+c[3*(ci+j)+2]*Zscp[1];
      cz = c[3*(ci+j)+0]*Xscp[2]+c[3*(ci+j)+1]*Yscp[2]+c[3*(ci+j)+2]*Zscp[2];

      c[3*(ci+j)+0] = cx;
      c[3*(ci+j)+1] = cy;
      c[3*(ci+j)+2] = cz;
    }

#endif

    // Translate cross section
    for (int j = 0; j < nCrossSection; ++j) {
      c[3*(ci+j)+0] += spine[3*i+0];
      c[3*(ci+j)+1] += spine[3*i+1];
      c[3*(ci+j)+2] += spine[3*i+2];

      if (tc != NULL) {
         // Texture coords
         tc[3*(ci+j)+0] = ((float) j) / (nCrossSection-1);
         tc[3*(ci+j)+1] = ((float) i) / (nSpine-1);
         tc[3*(ci+j)+2] = 0.0;
         }
    }

#if defined(__ORBISNAP__)

    // VRML specs say that nScale and nOrientation should be equal to nSpine,
    // or 1. However, we should not crash when we get something else.
    // So, if nScale or nOrientation is larger than 1 but lower than nSpine,
    // only the first value is used, ignoring the rest.

    if (nScale >= nSpine) scale += 2;
    if (nOrientation >= nSpine) orientation += 4;

#else

    if (nScale > 1) scale += 2;
    if (nOrientation > 1) orientation += 4;

#endif
  }

/*
  // And compute face indices:
  if (faces)
    {
      int polyIndex = 0;
      for (int i = 0, ci = 0; i < nSpine-1; ++i, ci+=nCrossSection) {
        for (int j = 0; j < nCrossSection-1; ++j) {
          faces[polyIndex + 0] = ci+j;
          faces[polyIndex + 1] = ci+j+1;
          faces[polyIndex + 2] = ci+j+1 + nCrossSection;
          faces[polyIndex + 3] = ci+j + nCrossSection;
          faces[polyIndex + 4] = -1;
          polyIndex += 5;
        }
      }
    }
*/
}

void NodeExtrusion::createMesh(bool cleanDoubleVertices, bool triangulate)
{
    double unitAngle = m_scene->getUnitAngle();

    int nOrientation = orientation()->getSFSize();
    float *porientation = (float *)orientation()->getValues();
    float *forientation = new float[4 * nOrientation];
    for (int i = 0; i < 4 * nOrientation; i++)
        forientation[i] = porientation[i];
    for (int i = 0; i < nOrientation; i++)
        forientation[i * 4 + 3] *= unitAngle;
    int nScale = scale()->getSFSize();
    const float *fscale = scale()->getValues();
    int nCrossSection = crossSection()->getSFSize();
    const float *fcrossSection = crossSection()->getValues();
    int nSpine = spine()->getSFSize();
    const float *fspine = spine()->getValues();

    float *c;  // OUT: coordinates
    c = new float[3 * nSpine * nCrossSection];

    float *tc = NULL;    

    computeExtrusion(nOrientation, forientation, nScale, fscale,
                     nCrossSection, fcrossSection, nSpine, fspine,
                     c, tc);

    int numCoord = 0;
    MFInt32 *coordIndex = new MFInt32();
    int numVertex = 0;
    MFVec3f *vertices = new MFVec3f();
 
    // compute beginCap
    if (beginCap()->getValue()) {
        for (int k = 0; k < nCrossSection; k++) {
            vertices->setValue(numVertex*3  , c[k*3]);
            vertices->setValue(numVertex*3+1, c[k*3+1]);
            vertices->setValue(numVertex*3+2, c[k*3+2]);
            coordIndex->setSFValue(numCoord++, nCrossSection - 1 - numVertex);
            numVertex++;
        }
        coordIndex->setSFValue(numCoord++, -1);
    }

    // copy mesh vertices
    for (int i = 0, ci = 0; i < nSpine; ++i, ci+=nCrossSection) {
        for (int j = 0; j < nCrossSection; ++j) {
            int index = 3 * (ci + j);
            vertices->setValue(index,   c[index]);
            vertices->setValue(index + 1, c[index + 1]);
            vertices->setValue(index + 2, c[index + 2]);
        }
    }

    // And compute face indices:
    for (int i = 0, ci = 0; i < nSpine - 1; i++, ci += nCrossSection) {
        int offset = 0;
        float minLen2 = FLT_MAX;
        float *v = c + ci * 3;
        float point[3] = {v[0], v[1], v[2]}; 
        // find smallest len^2 to build smoothest connections between vertices
        for (int j = 0; j < nCrossSection - 2; j++) {
            v = c + (ci + j + nCrossSection) * 3;
            float vec[3] = {v[0] - point[0], v[1] - point[1], v[2] - point[2]};
            float len2 = Vlength2(vec);
            if (len2 < minLen2) {
                minLen2 = len2;
                offset = j;
            }
        }
        for (int o = 0; o < nCrossSection - 1; o++) {
            int j = o + offset;
            if (j >= (nCrossSection - 1))
                j -= (nCrossSection - 1);
            coordIndex->setSFValue(numCoord++, ci + o);
            coordIndex->setSFValue(numCoord++, ci + o + 1);
            coordIndex->setSFValue(numCoord++, ci + j + 1 + nCrossSection);
            coordIndex->setSFValue(numCoord++, ci + j + nCrossSection);
            coordIndex->setSFValue(numCoord++, -1);
        }
    }

    // compute endCap
    if (endCap()->getValue()) {
        numVertex += nSpine * nCrossSection;
        for (int k = 0; k < nCrossSection; k++) {
            vertices->setValue(numVertex*3  , 
                               c[((nSpine-1)*nCrossSection+k)*3]);
            vertices->setValue(numVertex*3+1, 
                               c[((nSpine-1)*nCrossSection+k)*3+1]);
            vertices->setValue(numVertex*3+2, 
                               c[((nSpine-1)*nCrossSection+k)*3+2]);
            coordIndex->setSFValue(numCoord++, numVertex++);
        }
        coordIndex->setSFValue(numCoord++, -1);
    }

    delete [] c;

    int meshFlags = 0;
    if (ccw()->getValue())
        meshFlags |= MESH_CCW;
    if (solid()->getValue())
        meshFlags |= MESH_SOLID;
    if (convex()->getValue())
        meshFlags |= MESH_CONVEX;
    meshFlags |= MESH_COLOR_PER_VERTEX;
    meshFlags |= MESH_NORMAL_PER_VERTEX;
    MyArray<MFVec2f *>texCoords;
    m_mesh = new MyMesh(this, vertices, coordIndex, NULL, NULL,
                        NULL, NULL, texCoords, NULL, creaseAngle()->getValue(), 
                        meshFlags);
//#ifdef HAVE_GLUNEWTESS
//    m_mesh->triangulate();
//#endif
    delete [] forientation;
}

void
NodeExtrusion::setField(int index, FieldValue *value, int cf)
{
    m_meshDirty = true;
    Node::setField(index, value, cf);
}


void
NodeExtrusion::drawHandles(void)
{
    if (meshDirty()) {
        createMesh();
        m_meshDirty = false;
    }

    int spineLen = spine()->getSFSize();
    int scaleLen = scale()->getSFSize();
    int crossLen = crossSection()->getSFSize();
        
    int nCrossSections = crossSection()->getSFSize();
    int sp = 0;

    glColor3f(1.0f, 1.0f, 1.0f);
    glDisable(GL_LIGHTING);

    for (sp = 0; sp < spineLen && sp + 1 < spineLen; sp++) {
        glLoadName(NO_HANDLE);
        glBegin(GL_LINE_STRIP);
        glVertex3f(Vec3f(spine()->getValue(sp)).x,
                   Vec3f(spine()->getValue(sp)).y,
                   Vec3f(spine()->getValue(sp)).z);
        glVertex3f(Vec3f(spine()->getValue(sp + 1)).x,
                   Vec3f(spine()->getValue(sp + 1)).y,
                   Vec3f(spine()->getValue(sp + 1)).z);
        glEnd();
    }

    if (m_mesh != NULL) {
        MFVec3f *vertices = m_mesh->getVertices();
        if (vertices != NULL) {
            for (int k = nCrossSections - 1; k > 0; k--) {
                glLoadName(NO_HANDLE);
                glBegin(GL_LINE_STRIP);
                glColor3f(1.0f, 1.0f, 1.0f);
                glVertex3f(vertices->getValue(k)[0], 
                           vertices->getValue(k)[1],
                           vertices->getValue(k)[2]);
                glVertex3f(vertices->getValue(k - 1)[0], 
                           vertices->getValue(k - 1)[1],
                           vertices->getValue(k - 1)[2]);
                glEnd();
            }
        }
    }

    RenderState state;
    glPushMatrix();
    glPushAttrib(GL_LIGHTING);
    glDisable(GL_LIGHTING);
    glPushName(0);
    state.startDrawHandles();
    for (sp = 0; sp < spineLen; sp++) {
        state.setHandleColor(m_scene, sp);
        glLoadName(sp);
        state.drawHandle(Vec3f(spine()->getValue(sp)));
    }

    if (m_mesh != NULL) {
        MFVec3f *vertices = m_mesh->getVertices();
        if (vertices != NULL) {
            for (int k = nCrossSections - 1; k >= 0; k--) {
                state.setHandleColor(m_scene, spineLen + k);
                glLoadName(spineLen + k);
                state.drawHandle(Vec3f(vertices->getValue(k)));
            }
        }
    }

    for (int s = 0; s < scaleLen; s++) {
        if (s >= spineLen) 
            break;
        Vec2f fscale = scale()->getValue(s);

        Vec3f offset1(fscale.x, 0, 0);
        state.setHandleColor(m_scene, 2 * s + crossLen + spineLen);
        glLoadName(2 * s + crossLen + spineLen);
        state.drawHandle(offset1 + Vec3f(spine()->getValue(s)));

        Vec3f offset2(0, 0,  fscale.y);
        state.setHandleColor(m_scene, 2 * s + crossLen + spineLen + 1);
        glLoadName(2 * s + crossLen + spineLen + 1);
        state.drawHandle(offset2 + Vec3f(spine()->getValue(s)));
    }        
    state.endDrawHandles();
    glEnable(GL_LIGHTING);
    glPopName();
    glPopAttrib();
    glPopMatrix();
}

Vec3f NodeExtrusion::getHandle(int handle, int *constraint, int *field)
{
    MFVec3f *oldSpine = (MFVec3f *)getField(spine_Field());
    MFVec2f *oldCrossSection = (MFVec2f *)getField(crossSection_Field());
        
    if ((handle >= 0) && (handle < oldSpine->getSFSize())) {
        *field = spine_Field();
        Vec3f vec3f(spine()->getValue(handle)[0], spine()->getValue(handle)[1],
                     spine()->getValue(handle)[2]);
        TheApp->PrintMessageWindowsVertex(IDS_VERTEX_SELECTED, "spine", 
                                          handle, vec3f);
        return vec3f;
    } else if ((handle >= oldSpine->getSFSize()) && 
               (handle < oldSpine->getSFSize() + crossSection()->getSFSize())) {
        *field = crossSection_Field();
        int yHandle = handle - spine()->getSFSize();
        Vec3f vec3f(crossSection()->getValue(yHandle)[0], 0,
                     crossSection()->getValue(yHandle)[1]);
        TheApp->PrintMessageWindowsVertex(IDS_VERTEX_SELECTED, "crossSection",
                                          yHandle, vec3f);
        return vec3f;
    } else if (handle < (oldSpine->getSFSize() + 
                         crossSection()->getSFSize() + 
                         2 * scale()->getSFSize())) {
        *field=scale_Field();
        int yHandle;
        yHandle = handle - oldSpine->getSFSize() - oldCrossSection->getSFSize();
        yHandle = (yHandle - (yHandle % 2)) / 2;
        Vec3f vec3f(scale()->getValue(yHandle)[0], 1.0f,
                     scale()->getValue(yHandle)[1]);
        TheApp->PrintMessageWindowsVertex(IDS_VERTEX_SELECTED, "scale",
                                          yHandle, vec3f);
        return vec3f;
    }
    Vec3f vec3f;
    *field = getProto()->metadata_Field();
    return vec3f;
}
  
void NodeExtrusion::setHandle(int handle, const Vec3f &v)
{
    MFVec2f *oldCrossSection = (MFVec2f *)getField(crossSection_Field());
    MFVec2f *newcrossSection = (MFVec2f *)oldCrossSection->copy();
    float crosspoint[2] = {v.x, v.z};
    MFVec2f *oldScale = (MFVec2f *)getField(scale_Field());
    MFVec2f *newscale = (MFVec2f *)oldScale->copy();

    float scalepoint[2] = {v.x, -v.z};
    MFVec3f *oldSpine = (MFVec3f *)getField(spine_Field());
    MFVec3f *newspine = (MFVec3f *)oldSpine->copy();

    float spinepoint[3] = {v.x, v.y, v.z};

    int yHandle;
    yHandle = handle - oldCrossSection->getSFSize() - oldSpine->getSFSize();

    if (yHandle < 2 * scale()->getSFSize()) {
        if ((yHandle % 2) == 0) {
            scalepoint[1] = oldScale->getValue(yHandle / 2)[1];
        } else if ((yHandle % 2) == 1) {
            scalepoint[0] = oldScale->getValue(yHandle / 2)[0];
            scalepoint[1] *= -1;
        }
    }

    if ((handle >= 0) && (handle < oldSpine->getSFSize())) {
        newspine->setSFValue(handle, spinepoint);
        m_scene->setField(this, spine_Field(), newspine);
    } else if ((handle >= oldSpine->getSFSize()) && 
               (handle < (oldSpine->getSFSize() + 
                          crossSection()->getSFSize()))) {
        newcrossSection->setSFValue(handle - oldSpine->getSFSize(), crosspoint);
        m_scene->setField(this, crossSection_Field(), newcrossSection);
    } else if ((handle >= oldSpine->getSFSize() +
                          oldCrossSection->getSFSize()) && 
               (handle < (oldSpine->getSFSize() +
                          oldCrossSection->getSFSize() + 
                          2 * oldSpine->getSFSize()))) {
        newscale->setSFValue((yHandle - (yHandle % 2)) / 2 , scalepoint);
        m_scene->setField(this, scale_Field(), newscale);
    }
}

void
NodeExtrusion::flip(int index)
{
    if (spine())
        spine()->flip(index);
    if (crossSection())
        crossSection()->flip(index);
    if (orientation())
        orientation()->flip(index);
    m_meshDirty = true;
}

void
NodeExtrusion::swap(int fromTo)
{
    if (spine())
        spine()->swap(fromTo);
    if (crossSection())
        crossSection()->swap(fromTo);
    if (orientation())
        orientation()->swap(fromTo);
    m_meshDirty = true;
}

bool    
NodeExtrusion::canMoveTo(int direction)
{
    return false;
}


bool    
NodeExtrusion::validHandle(int handle) 
{ 
    if ((handle >= 0) && (handle < (spine()->getSFSize() +
                                    crossSection()->getSFSize() + 
                                    2 * scale()->getSFSize()))){
        return m_scene->getSelectedHandlesSize() <= 1; 
    }
    return false;
}


