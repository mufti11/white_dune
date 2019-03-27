/*
 * NodeCone.cpp
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

#include "NodeCone.h"
#include "Proto.h"
#include "FieldValue.h"
#include "Scene.h"
#include "MyMesh.h"
#include "SFFloat.h"
#include "SFBool.h"
#include "MFNode.h"
#include "SFNode.h"
#include "SFInt32.h"
#include "MFFloat.h"
#include "MFVec3f.h"
#include "NurbsMakeRevolvedSurface.h"
#include "NodeNurbsSurface.h"
#include "RenderState.h"
#include "SFVec3f.h"
#include "Util.h"
#include "MFNode.h"

ProtoCone::ProtoCone(Scene *scene)
  : Proto(scene, "Cone")
{
    bottom.set(
          addField(SFBOOL, "bottom", new SFBool(true)));

    bottomRadius.set(
          addField(SFFLOAT, "bottomRadius", new SFFloat(1.0f), 
                   new SFFloat(0.0f)));

    height.set(
          addField(SFFLOAT, "height", new SFFloat(2.0f), new SFFloat(0.0f)));

    side.set(
          addField(SFBOOL, "side", new SFBool(true)));

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
          addExposedField(SFINT32, "subdivision", new SFInt32(32),
                          new SFInt32(2)));
    setFieldFlags(subdivision, FF_X3DOM_ONLY);

    top.set(
          addExposedField(SFBOOL, "top", new SFBool(true)));
    setFieldFlags(top, FF_X3DOM_ONLY);

    topRadius.set(
          addExposedField(SFFLOAT, "topRadius", new SFFloat(0), 
                          new SFFloat(0)));
    setFieldFlags(topRadius, FF_X3DOM_ONLY);
}

Node *ProtoCone::create(Scene *scene)
{ 
    return new NodeCone(scene, this); 
}

NodeCone::NodeCone(Scene *scene, Proto *def)
  : MeshBasedNode(scene, def)
{
}

void NodeCone::drawHandles(void)
{
    RenderState state;
        
    float fheight = height()->getValue();
    float fbottomRadius = bottomRadius()->getValue();
   
    glPushMatrix();

    glScalef(fbottomRadius * 1.0f,fheight * 0.5f, fbottomRadius * 1.0f );
    glPushAttrib(GL_LIGHTING);
    glDisable(GL_LIGHTING);
    glPushName(0);
    state.startDrawHandles();    
    for (int i = 0; i < 5; i++) {
        state.setHandleColor(m_scene, i);
        glLoadName(i);
        state.drawHandle(ConeCorners[i]);
    }
    state.endDrawHandles();
    glPopName();
    glPopAttrib();
    glPopMatrix();


}

Vec3f
NodeCone::getHandle(int handle,int* constrait, int* field)
{
    float fheight = height()->getValue();
    float fbottomRadius = bottomRadius()->getValue();
  
    switch (handle) {
      case COBLB:
        *field = height_Field();
        return Vec3f(1.0f*fbottomRadius,1.0f*fheight,1.0f*fbottomRadius) * Vec3f(ConeCorners[handle]) * 0.5f;
      case COBRB: case COTRB: 
        *field = bottomRadius_Field();
        return Vec3f(1.0f*fbottomRadius,0.0f,1.0f*fbottomRadius) * Vec3f(ConeCorners[handle]) * 1.0f;
      case COTLB: case COBLF:
        *field = bottomRadius_Field();;
        return Vec3f(1.0f*fbottomRadius,0.0f,1.0f*fbottomRadius) * Vec3f(ConeCorners[handle]) * 1.0f;
      default:
        *field = getProto()->metadata_Field();
        return Vec3f(0.0f, 0.0f, 0.0f);
    }
        
}

void 
NodeCone::setHandle(int handle,const Vec3f &v)
{
    switch (handle) {
      case COBLB:
        m_scene->setField(this,height_Field(),new SFFloat(v.y*2.0f*Vec3f(ConeCorners[handle]).y));
        break; 
      case COBRB: case COTRB:
        m_scene->setField(this,bottomRadius_Field(),new SFFloat(v.x*1.0f*Vec3f(ConeCorners[handle]).x));
        break;
      case COTLB: case COBLF:
         m_scene->setField(this,bottomRadius_Field(),new SFFloat(v.z*1.0f*Vec3f(ConeCorners[handle]).z));
         break;
     }
}

Node*
NodeCone::toNurbs(int nshell, int narea, int narcs, int uDegree, int vDegree)
{   
  NodeNurbsSurface *node = (NodeNurbsSurface *)
                            m_scene->createNode("NurbsSurface");
  float fbottomRadius = bottomRadius()->getValue();
  float fheight = height()->getValue();
  int iside = side()->getValue();
  int ibottom = bottom()->getValue();

  /*At the edge (shell to bottom area) there are p circles at the same 
    position to achieve sharp edge  
  
    meaning of the main variables:

    int narcs ->number of circular arc segments, cone consists of
    int nshell -> number of controlpoints on shell in direction of axis
    int narea -> number of controlpoints on bottom area in direction of radius
    int vDegree -> degree in direction v
    int uDegree -> degree in direction u, currently limited to 2
  */

  int vOrder = vDegree +1;
  int uOrder = uDegree +1;

  int vDimension;
  if(iside==1){ 
    vDimension = (ibottom * (narea + vDegree - 2)) + nshell;
  }
  else{
    vDimension = (ibottom * narea);
  }

  float *vKnots = new float[vDimension + vOrder];

  Vec3f *generatrix = new Vec3f[vDimension];
  float *tmpWeights = new float[vDimension]; 

  int max_i; 
  float y;
  
  //load generatrix
  //shell
  float stepdown;
  float stepradius;
  if (iside==1){
    y = fheight / 2; //start at apex
    stepdown = fheight / (nshell - 1);
    stepradius = fbottomRadius / (nshell - 1); 
    for(int i=0; i<(iside*nshell); i++){
      generatrix[i].x = i * stepradius;
      generatrix[i].y = y;
      generatrix[i].z = 0;
      y = y - stepdown;
    }
  }
  //bottom area
  if (ibottom==1){
    y = - fheight / 2;
    stepradius = fbottomRadius / (narea - 1);
    if(iside==1){
      for(int i=(iside*nshell), j=1; i<((iside*nshell)+vDegree-2); i++, j++){
        generatrix[i].x = (narea - 1) * stepradius;
        generatrix[i].y = y; 
        generatrix[i].z = 0;
      }
      for(int i=(nshell+vDegree-2), j=1; i<(nshell+narea+vDegree-2); i++, j++){
        generatrix[i].x = (narea - j) * stepradius;
        generatrix[i].y = y;   
        generatrix[i].z = 0;
      }
    }
    else{
      for(int i=0, j=1; i<narea; i++, j++){
        generatrix[i].x = (narea - j) * stepradius;
        generatrix[i].y = y;
        generatrix[i].z = 0;
      }
    }
  }

  //weights
  for(int i=0; i<vDimension; i++){
    tmpWeights[i] = 1;
  }
  //v-knotvector
  for(int i=0; i<vOrder; i++){
    vKnots[i] = 0.0f;
    vKnots[i+vDimension] = (float) (vDimension - vOrder +1);
  }
  for(int i=0; i<(vDimension-vOrder); i++){
    vKnots[i+vOrder] = (float) (i + 1);  
  } 
  //rotate generatrix. 
  float arc = 360;
  Vec3f P1, P2; 
  P1.x = P2.x = 0;
  P1.z = P2.z = 0;
  P1.y = 0;
  P2.y = -1;

  NurbsMakeRevolvedSurface nurbsSphere(generatrix, tmpWeights, vDimension, narcs, arc, uDegree, P1, P2);   
  if (!nurbsSphere.isValid())
      return NULL;

  float *controlPoints = new float[nurbsSphere.getPointSize()];
  float *weights = new float[nurbsSphere.getWeightSize()];
  int uDimension = nurbsSphere.getWeightSize() / vDimension;
  float *uKnots = new float[uDimension + uOrder]; 

  //get control points
  max_i = nurbsSphere.getPointSize();
  for(int i=0; i<max_i; i++){
    controlPoints[i] = nurbsSphere.getControlPoints(i);
  }
  //get weights
  max_i = nurbsSphere.getWeightSize();
  for(int i=0; i<max_i; i++){
    weights[i] = nurbsSphere.getWeights(i);
  }

  if (uDegree == 1){
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
    //get u-knotvektor
    max_i = nurbsSphere.getKnotSize();
    for(int i=0; i<max_i; i++){
       uKnots[i] = nurbsSphere.getKnots(i);
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
NodeCone::flip(int index)
{
    if (index == 1) {
        // cone can not handle a y-flip, convert to IndexedFaceSet
        Node *mesh = m_scene->convertToIndexedFaceSet(this);
        if (mesh != NULL)
            mesh->flip(index);
    }
}

void
NodeCone::swap(int fromTo)
{
    if (fromTo != SWAP_XZ) {
        // cone can not handle a xy- or yz-swap, convert to IndexedFaceSet
        Node *mesh = m_scene->convertToIndexedFaceSet(this);
        if (mesh != NULL)
            mesh->swap(fromTo);
    }
}

void
NodeCone::createMesh(bool cleanDoubleVertices, bool triangulate)
{
    float fradius = bottomRadius()->getValue();
    float fheight = height()->getValue();

    bool bside = side()->getValue();
    bool bbottom = bottom()->getValue();

    int tess = TheApp->getTessellation();
    int sizePoints = (1 + tess + tess + tess) * 3;
    int sizeTexCoords = (1 + tess + tess + tess) * 2;
    
    if (!bside && !bbottom)
        return;

    float *fpoint = new float[sizePoints];
    float *fnormal = new float[sizePoints];
    float *ftexCoord = new float[sizeTexCoords];

    int offset = 0;
    int offsetTexCoord = 0;
    fpoint[offset + 0] = 0;
    fpoint[offset + 1] = -fheight / 2.0f;
    fpoint[offset + 2] = 0;
    fnormal[offset + 0] = 0;
    fnormal[offset + 1] = -1;
    fnormal[offset + 2] = 0;
    offset += 3;
    ftexCoord[offsetTexCoord + 0] = 0.5;
    ftexCoord[offsetTexCoord + 1] = 0.5;
    offsetTexCoord += 2;

    for (int i = 0; i < tess; i++) {
        float theta = M_PI + 2.0f * M_PI * i / (tess - 1);
        Vec3f point(sin(theta), -fheight / 2.0f, cos(theta));
        fpoint[offset + 0] = fradius * point.x;
        fpoint[offset + 1] = point.y;
        fpoint[offset + 2] = fradius * point.z;            
        fnormal[offset + 0] = 0;
        fnormal[offset + 1] = -1;
        fnormal[offset + 2] = 0;
        offset += 3;
        ftexCoord[offsetTexCoord + 0] = 0.5 + point.x / 2.0f;
        ftexCoord[offsetTexCoord + 1] = 0.5 + point.z / 2.0f;
        offsetTexCoord += 2;
    }

    for (int i = 0; i < tess; i++) {
        float theta = M_PI + 2.0f * M_PI * i / (tess -1);
        Vec3f point(sin(theta), -fheight / 2.0f, cos(theta));
        fpoint[offset + 0] = fradius * point.x;
        fpoint[offset + 1] = point.y;
        fpoint[offset + 2] = fradius * point.z;            
        Vec3f normal(point.x * fheight, fradius, point.z * fheight);
        normal.normalize();
        fnormal[offset + 0] = normal.x;
        fnormal[offset + 1] = normal.y;
        fnormal[offset + 2] = normal.z;            
        offset += 3;
        ftexCoord[offsetTexCoord + 0] = i / (tess - 1.0f);
        ftexCoord[offsetTexCoord + 1] = 0.0f;
        offsetTexCoord += 2;
    }

    for (int i = 0; i < tess; i++) {
        float theta = M_PI + 2.0f * M_PI *  i / (tess -1);
        Vec3f point(sin(theta), -fheight / 2.0f, cos(theta));
        fpoint[offset + 0] = 0;
        fpoint[offset + 1] = fheight / 2.0f;
        fpoint[offset + 2] = 0;
        Vec3f normal(point.x * fheight, fradius, point.z * fheight);
        normal.normalize();
        fnormal[offset + 0] = normal.x;
        fnormal[offset + 1] = normal.y;
        fnormal[offset + 2] = normal.z;            
        offset += 3;
        ftexCoord[offsetTexCoord + 0] = i / (tess - 1.0f);
        ftexCoord[offsetTexCoord + 1] = 1.0f;
        offsetTexCoord += 2;
    }

    MyArray<int> icoordIndex;
    if (bbottom)
        for (int i = 0; i < tess; i++) {    
            icoordIndex.append(1 + i);
            icoordIndex.append(0);
            if (i == (tess - 1))
                icoordIndex.append(1);
            else
                icoordIndex.append(1 + i + 1);
            icoordIndex.append(-1);
        } 
    if (bside)
        for (int i = 0; i < tess; i++) { 
            icoordIndex.append(tess + 1 + i);
            if (i == (tess - 1))
                icoordIndex.append(tess + 1);
            else
                icoordIndex.append(tess + 1 + i + 1);
            icoordIndex.append(tess + 1 + tess + i);
            icoordIndex.append(-1);
        } 

    MFVec3f* coords = new MFVec3f(fpoint, sizePoints);

    MFInt32 *mfcoordIndex = new MFInt32((int *)icoordIndex.getData(), 
                                        icoordIndex.size());
    MFInt32 *coordIndex = (MFInt32 *)mfcoordIndex->copy();

    MFVec3f *normals = new MFVec3f(fnormal, sizePoints);

    MFVec2f *texCoord = new MFVec2f(ftexCoord, sizeTexCoords);
    MyArray<MFVec2f *> texCoords;
    texCoords.append(texCoord);

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

    m_mesh = new MyMesh(this, coords, coordIndex, normals, normalIndex, colors, 
                        colorIndex, texCoords, texCoordIndex,
                        M_PI / 2.0 - 0.0001f, meshFlags, transparency);
    if (cleanDoubleVertices)
        m_mesh->optimize();
}

bool    
NodeCone::validHandle(int handle) 
{ 
    switch (handle) {
      case COBLB:
      case COBRB: case COTRB:
      case COTLB: case COBLF:
        return m_scene->getSelectedHandlesSize() <= 1; 
    }
    return false;
}


