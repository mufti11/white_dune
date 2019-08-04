/*
 * NodeSphere.cpp
 *
 * Copyright (C) 1999 Stephen F. White
 *               2003 Th. Rothermel
 *               2004 Wu Qingwei
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

#include <stdio.h>
#include "stdafx.h"

#include "NodeSphere.h"
#include "Proto.h"
#include "FieldValue.h"
#include "Scene.h"
#include "MyMesh.h"
#include "SFFloat.h"
#include "MFNode.h"
#include "SFNode.h"
#include "SFInt32.h"
#include "MFFloat.h"
#include "MFVec3f.h"
#include "NurbsArc.h"
#include "NurbsMakeRevolvedSurface.h"
#include "NodeNurbsSurface.h"
#include "RenderState.h"
#include "Util.h"
#include "SFVec3f.h"

ProtoSphere::ProtoSphere(Scene *scene)
  : Proto(scene, "Sphere")
{
    radius.set(
          addField(SFFLOAT, "radius", new SFFloat(1.0F), new SFFloat(0.0f)));

    solid.set(
          addField(SFBOOL, "solid", new SFBool(true)));
    setFieldFlags(solid, FF_X3D_ONLY);

    texCoord.set(
          addExposedField(SFNODE, "texCoord", new SFNode(NULL), 
                          GENERATED_TEXTURE_COORDINATE_NODE));
    setFieldFlags(texCoord, FF_KAMBI_ONLY);

    x3domGeometryCommonFields()

    ccw.set(
          addExposedField(SFBOOL, "ccw", new SFBool(true)));
    setFieldFlags(ccw, FF_X3DOM_ONLY);

    subdivision.set(
          addExposedField(SFVEC2F, "subdivision", new SFVec2f(24, 24)));
    setFieldFlags(subdivision, FF_X3DOM_ONLY);
}

Node *
ProtoSphere::create(Scene *scene)
{ 
    return new NodeSphere(scene, this); 
}

NodeSphere::NodeSphere(Scene *scene, Proto *def)
  : MeshBasedNode(scene, def)
{
}

void
NodeSphere::drawHandles()
{
    float fradius = radius()->getValue();
    RenderState state;

    glPushMatrix();
    glScalef(fradius*1.0f,fradius*1.0f,fradius*1.0f);
    glPushAttrib(GL_LIGHTING);
    glDisable(GL_LIGHTING);
    glPushName(radius_Field());
        
    state.startDrawHandles();
    for (int i = 0; i < 6; i++) {
        state.setHandleColor(m_scene, i);
        glLoadName(i);
        Vec3f vec(spereCorners[i][0], spereCorners[i][1], spereCorners[i][2]);
        state.drawHandle(vec);
    }
    state.endDrawHandles();
    glPopName();
    glPopAttrib();
    glPopMatrix(); 
}

Vec3f
NodeSphere::getHandle(int handle, int *constraint , int *field)
{
    *field = radius_Field();
    float fradius = radius()->getValue();
    Vec3f ps;
    ps.x = fradius;
    ps.y = fradius;
    ps.z = fradius;
    switch (handle) {
      case STLF: case STLB:
        return Vec3f(ps) * Vec3f(spereCorners[handle]) * 1.0f; //z-direction
      case SBRB:case SBLB:
        return Vec3f(ps) * Vec3f(spereCorners[handle]) * 1.0f; //y-direction
      case STRB:case SBLF:
        return Vec3f(ps) * Vec3f(spereCorners[handle]) * 1.0f; //x-direction
      default:
        return Vec3f(0.0f, 0.0f, 0.0f);
    }
}

void 
NodeSphere::setHandle(int handle, const Vec3f &v)
{
    switch (handle) {
      case STLF: case STLB:
        m_scene->setField(this, radius_Field(),new SFFloat(v.z*1.0f*Vec3f(spereCorners[handle]).z)); 
        break;
      case SBRB:case SBLB:
        m_scene->setField(this, radius_Field(),new SFFloat(v.y*1.0f*Vec3f(spereCorners[handle]).y));
        break;
      case STRB:case SBLF:
        m_scene->setField(this, radius_Field(),new SFFloat(v.x*1.0f*Vec3f(spereCorners[handle]).x)); 
        break;
    }
}

Node*
NodeSphere::toNurbs(int narcslong,  int narcslat, int uDegree, int vDegree)
{   
  NodeNurbsSurface *node = (NodeNurbsSurface *) m_scene->createNode(
                                 "NurbsSurface");
  float fradius = radius()->getValue();
  
  /*meaning of most important variables

    int narcslong ->number of circular arc segments from pole to pole 
                    (longitudinal)
    int narcslat  -> number of circular arc segments in lateral direction
    int vDegree -> degree in direction v, currently limited to 2
    int uDegree -> degree in direction u, currently limited to 2
  */

  int vOrder = vDegree +1;
  int uOrder = uDegree +1;

  float arc = 180;
  Vec3f PS, P1, P2;
  
  PS.x = 0;
  PS.y = -fradius;
  PS.z = 0;

  P1.x = P2.x = 0;
  P1.y = P2.y = 0;
  P1.z = 0;
  P2.z = 1;

  NurbsArc tmpArc(narcslong, arc, PS, P1, P2, vDegree);
  
  Vec3f *longitudinalArc = new Vec3f[tmpArc.getPointSize()];
  float *tmpWeights = new float[tmpArc.getPointSize()];
  
  //load generatrix
  for (int i=0; i<tmpArc.getPointSize(); i++){
    longitudinalArc[i] = tmpArc.getControlPoints(i);
    tmpWeights[i] = tmpArc.getWeights(i);    
  }

  float *vKnots = new float[tmpArc.getKnotSize()];
  
  //load v-knotvector
  for (int i=0; i<(tmpArc.getKnotSize()); i++){
    vKnots[i] = tmpArc.getKnots(i);
  }

  //rotate generatrix 
  arc = 360;
  P1.x = P2.x = 0;
  P1.z = P2.z = 0;
  P1.y = 0;
  P2.y = 1;

  NurbsMakeRevolvedSurface nurbsSphere(longitudinalArc, tmpWeights, tmpArc.getPointSize(), narcslat, arc, uDegree, P1, P2);
  if (!nurbsSphere.isValid())
      return NULL;

  //get result
  
  int vDimension = tmpArc.getPointSize();
  float *controlPoints = new float[nurbsSphere.getPointSize()];
  float *weights = new float[nurbsSphere.getWeightSize()];
  int uDimension = (int) (nurbsSphere.getWeightSize() / vDimension);
  float *uKnots = new float[uDimension + uOrder]; 

  int max_i = nurbsSphere.getPointSize();
  for(int i=0; i<max_i; i++){
    controlPoints[i] = nurbsSphere.getControlPoints(i);
  }
  max_i = nurbsSphere.getWeightSize();
  for(int i=0; i<max_i; i++){
    weights[i] = nurbsSphere.getWeights(i);
  }
  if (uDegree == 1) {
    //set u-knotvektor
    for(int i=0; i<uOrder; i++){
      uKnots[i] = 0.0f;
      uKnots[i+uDimension] = (float) (uDimension - uOrder +1);
    }
    for(int i=0; i<(uDimension-uOrder); i++){
      uKnots[i+uOrder] = (float) (i + 1);  
    } 
  }
  else {
    max_i = nurbsSphere.getKnotSize();
    for(int i=0; i<max_i; i++){
      uKnots[i] = nurbsSphere.getKnots(i);
    }
  }
  if (vDegree == 1) {
    //set v-knotvektor
    for(int i=0; i<vOrder; i++){
      vKnots[i] = 0.0f;
      vKnots[i+vDimension] = (float) (vDimension - vOrder +1);
    }
    for(int i=0; i<(vDimension-vOrder); i++){
      vKnots[i+vOrder] = (float) (i + 1);  
    } 
  }  
  node->setField(node->uDimension_Field(), new SFInt32(uDimension));
  node->setField(node->vDimension_Field(), new SFInt32(vDimension));
  node->uKnot(new MFFloat(uKnots, uDimension + uOrder));
  node->vKnot(new MFFloat(vKnots, vDimension + vOrder));
  node->setField(node->uOrder_Field(), new SFInt32(uOrder));
  node->setField(node->vOrder_Field(), new SFInt32(vOrder));
  node->setField(node->uClosed_Field(), new SFBool(true));
  node->setField(node->vClosed_Field(), new SFBool(true));
  node->createControlPoints(new MFVec3f(controlPoints, 
                                        uDimension * vDimension * 3));
  node->weight(new MFFloat(weights, uDimension * vDimension));

  return node;   
}

void
NodeSphere::createMesh(bool cleanDoubleVertices, bool triangulate)
{
    int tess = TheApp->getTessellation();
    float fradius = radius()->getValue();
    int numPoints = (1 + (tess - 2) * tess + 1) * 3;
    float *fpoint = new float[numPoints];

    fpoint[0] = 0;
    fpoint[1] = fradius;
    fpoint[2] = 0;

    float *fnormal = new float[numPoints];

    fnormal[0] = 0;
    fnormal[1] = 1;
    fnormal[2] = 0;

    int i = 0;
    for (i = 0; i < (tess - 2); i++) {
        float phi = M_PI * (i + 1.0f) / (tess - 1);
        for (int j = 0; j < tess; j++) {
            float theta = 2.0f * M_PI * (j + (i % 2) * 0.5f) / tess;
            int offset = (1 + i * tess + j) * 3; 
            Vec3f point((fradius + 0.001f) * sin(phi) * sin(theta), 
                        (fradius - 0.001f) * cos(phi),
//                        (fradius + 0.001f) * sin(phi) * cos(theta));
                        fradius * sin(phi) * cos(theta));
            fpoint[offset + 0] = point.x;
            fpoint[offset + 1] = point.y;
            fpoint[offset + 2] = point.z;            
            point.normalize();
            fnormal[offset + 0] = point.x;
            fnormal[offset + 1] = point.y;
            fnormal[offset + 2] = point.z;            
        }
    }

    int offset = numPoints - 3;
    fpoint[offset + 0] = 0;
    fpoint[offset + 1] = -fradius;
    fpoint[offset + 2] = 0;
    fnormal[offset + 0] = 0;
    fnormal[offset + 1] = -1;
    fnormal[offset + 2] = 0;

    MyArray<int> icoordIndex;
    for (i = 0; i < tess; i++) {    
        icoordIndex.append(1 + i);
        if (i == (tess - 1))
            icoordIndex.append(1);
        else
            icoordIndex.append(1 + i + 1);
        icoordIndex.append(0);
        icoordIndex.append(-1);
    }; 
    for (i = 0; i < (tess - 3); i++) {
        for (int j = 0; j < tess; j++) {
            int offset = 1 + i * tess + j; 

            if ((j == tess - 1) && ((i % 2) == 1))
                icoordIndex.append(offset + i % 2);            
            else
                icoordIndex.append(offset + tess + i % 2);            
            if (j == tess - 1)
                icoordIndex.append(offset - tess + 1);            
            else
                icoordIndex.append(offset + 1);            
            icoordIndex.append(offset);            
            icoordIndex.append(-1);

            if ((j == tess - 1) && ((i % 2) == 1))
                icoordIndex.append(offset + i % 2);            
            else
                icoordIndex.append(offset + tess + i % 2);
            if ((j == tess - 1) || ((j + 1 == tess - 1) && (i % 2) == 1))
                icoordIndex.append(offset + 1 + i % 2);            
            else
                icoordIndex.append(offset + tess + 1 + i % 2);            
            if (j == tess - 1)
                icoordIndex.append(offset - tess + 1);            
            else
                icoordIndex.append(offset + 1);            
            icoordIndex.append(-1);
        }
    }
    for (i = 0; i < tess; i++) {    
        int offset = numPoints / 3 - 1 - tess;
        if (i == (tess - 1))
            icoordIndex.append(offset);
        else
            icoordIndex.append(offset + i + 1);
        icoordIndex.append(offset + i);
        icoordIndex.append(numPoints / 3 - 1);
        icoordIndex.append(-1);
    }; 

    MFVec3f *coords = new MFVec3f(fpoint, numPoints);

    MFInt32 *coordIndex = (MFInt32 *)(new MFInt32((int *)icoordIndex.getData(), 
                                                  icoordIndex.size()))->copy();

    MFVec3f *normals = new MFVec3f(fnormal, numPoints);

    MFInt32 *normalIndex = NULL;
    
    MFColor *colors = NULL;
    MFInt32 *colorIndex = NULL;
    MFInt32 *texCoordIndex = NULL;

    int meshFlags = MESH_NORMAL_PER_VERTEX | MESH_CCW;
    if (m_scene->isX3d()) {
        if (solid()->getValue())
            meshFlags |= MESH_SOLID;
    } else
        meshFlags |= MESH_SOLID;

    float transparency = 0;
    if (hasParent())
        transparency = getParent()->getTransparency();

    MyArray<MFVec2f *>texCoords;
    m_mesh = new MyMesh(this, coords, coordIndex, normals, normalIndex, colors, 
                        colorIndex, texCoords, texCoordIndex,
                        M_PI / 2.0f - 0.0001f, meshFlags, transparency);
}

bool    
NodeSphere::validHandle(int handle) 
{ 
    switch (handle) {
      case STLF: case STLB:
      case SBRB:case SBLB:
      case STRB:case SBLF:
        return m_scene->getSelectedHandlesSize() <= 1; 
    }
    return false;
}

