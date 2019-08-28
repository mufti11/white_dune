/*
 * NodeBox.cpp
 *
 * Copyright (C) 1999 Stephen F. White
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

#include "NodeBox.h"
#include "Scene.h"
#include "Proto.h"
#include "FieldValue.h"
#include "FieldCommand.h"
#include "SFVec3f.h"
#include "SFFloat.h"
#include "RenderState.h"
#include "Node.h"
#include "Util.h"
#include "MFNode.h"
#include "SFNode.h"
#include "SFInt32.h"
#include "MFVec3f.h"
#include "NodeShape.h"
#include "NodeAppearance.h"
#include "NodeMaterial.h"
#include "NodeIndexedFaceSet.h"
#include "NodeCoordinate.h"
#include "NodeNormal.h"
#include "NodeNurbsGroup.h"
#include "NodeNurbsSurface.h"
#include "Util.h"
#include "MyMesh.h"

ProtoBox::ProtoBox(Scene *scene)
  : Proto(scene, "Box")
{
    size.set(
          addField(SFVEC3F, "size", new SFVec3f(2.0f, 2.0f, 2.0f), 
                   new SFFloat(0.0f)));

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

    hasHelperColors.set(
          addExposedField(SFBOOL, "hasHelperColors", new SFBool(false)));
    setFieldFlags(hasHelperColors, FF_X3DOM_ONLY);
}

Node *
ProtoBox::create(Scene *scene)
{ 
    return new NodeBox(scene, this); 
}

NodeBox::NodeBox(Scene *scene, Proto *def)
  : MeshBasedNode(scene, def)
{
}

void
NodeBox::drawHandles(void)
{
    const float *fsize = size()->getValue();
    RenderState state;

    glPushMatrix();
    glScalef(fsize[0] * 0.5f, fsize[1] * 0.5f, fsize[2] * 0.5f);
    glPushAttrib(GL_LIGHTING);
    glDisable(GL_LIGHTING);
    glPushName(size_Field());
    state.startDrawHandles();
    for (int i = 0; i < 8; i++) {
        state.setHandleColor(m_scene, i);
        glLoadName(i);
        Vec3f vec(boxCorners[i][0], boxCorners[i][1], boxCorners[i][2]);
        state.drawHandle(vec);
    }
    state.endDrawHandles();
    glPopName();
    glPopAttrib();
    glPopMatrix();
}

Vec3f
NodeBox::getHandle(int handle, int * /* constraint */, int *field)
{
    const float *fsize = size()->getValue();
    switch (handle) {
      case TRF: case TLF: case TRB: case TLB:
      case BRF: case BLF: case BRB: case BLB:
        *field = size_Field();
        return Vec3f(fsize) * Vec3f(boxCorners[handle]) * 0.5f;
      default:
        *field = getProto()->metadata_Field();
        return Vec3f(0.0f, 0.0f, 0.0f);
    }
}

void
NodeBox::setHandle(int handle, const Vec3f &v)
{
    switch (handle) {
      case TRF: case TLF: case TRB: case TLB:
      case BRF: case BLF: case BRB: case BLB:
        m_scene->setField(this, size_Field(), 
                         new SFVec3f(v * 2.0f * Vec3f(boxCorners[handle])));
        break;
    }
}

Node*
NodeBox::toNurbs(int nuAreaPoints, int uDegree, int nvAreaPoints, 
                 int vDegree, int nzAreaPoints)
{
  //build a box with NURBS from 1 surface
  const float *fsize = size()->getValue();
  NodeNurbsSurface *node = (NodeNurbsSurface *)
                           m_scene->createNode("NurbsSurface");
  int index = 0;
  int tmpindex;
  float xSize = fsize[0];
  float ySize = fsize[1];
  float zSize = fsize[2];    
  float dz = zSize / (nvAreaPoints - 1);
  float tmpxSize;
  float tmpySize;
  float zPosition = 0;
  int uOrder = uDegree + 1;
  int vOrder = vDegree + 1;  
  int uDimension = 4 * nuAreaPoints + 4 * uDegree - 7;
  int vDimension = nvAreaPoints + 2 * (nzAreaPoints-1) + 2*vDegree - 2;
  float *uKnots = new float[uDimension + uOrder];
  float *vKnots = new float[vDimension + vOrder];
  float *controlPoints = new float[uDimension * vDimension * 3];
  float *weights = new float[uDimension * vDimension];   
  
  for (int i = 0; i < uDimension * vDimension * 3; i++) {
    controlPoints[i]=0.0f;
  }

  //surface orthogonal z-axis
  for (int j = 0; j < nzAreaPoints - 1; j++) {
    zPosition = zSize / 2;
    tmpxSize = j * xSize / (nzAreaPoints-1);
    tmpySize = j * ySize / (nzAreaPoints-1);
    makeNurbsRectangle(tmpxSize, tmpySize, zPosition, nuAreaPoints, uDegree);
    for (size_t i = 0; i < m_rectangle.size(); i++) {
      controlPoints[(j*uDimension + i)*3    ] = m_rectangle[i].x;  
      controlPoints[(j*uDimension + i)*3 + 1] = m_rectangle[i].y;
      controlPoints[(j*uDimension + i)*3 + 2] = m_rectangle[i].z;  
    }
    index++;
  }

  //make sharp edge
  tmpindex = 0;
  for (int j=index; j<index+vDegree-1; j++) {
    makeNurbsRectangle(xSize, ySize, zPosition, nuAreaPoints, uDegree);
    for (size_t i = 0; i < m_rectangle.size(); i++) {
      controlPoints[(j*uDimension + i)*3    ] = m_rectangle[i].x;  
      controlPoints[(j*uDimension + i)*3 + 1] = m_rectangle[i].y;
      controlPoints[(j*uDimension + i)*3 + 2] = m_rectangle[i].z;  
    }
    tmpindex++;
  }            
  index = index + tmpindex;

  //surfaces in direction of z-axis
  tmpindex = 0;
  for (int j=index, index2=0; j<index+nvAreaPoints; j++, index2++) {  
    zPosition = zSize / 2 - index2 * dz;
    makeNurbsRectangle(xSize, ySize, zPosition, nuAreaPoints, uDegree);
    for (size_t i = 0; i < m_rectangle.size(); i++) {
      controlPoints[(j*uDimension + i)*3    ] = m_rectangle[i].x;  
      controlPoints[(j*uDimension + i)*3 + 1] = m_rectangle[i].y;
      controlPoints[(j*uDimension + i)*3 + 2] = m_rectangle[i].z;  
    }
    tmpindex++;
  }
  index = index + tmpindex;

  //make sharp edge
  tmpindex = 0;
  for (int j = index; j < index+vDegree - 1; j++) {
    zPosition = -zSize / 2;
    makeNurbsRectangle(xSize, ySize, zPosition, nuAreaPoints, uDegree);
    for (size_t i = 0; i < m_rectangle.size(); i++) {
      controlPoints[(j*uDimension + i)*3    ] = m_rectangle[i].x;  
      controlPoints[(j*uDimension + i)*3 + 1] = m_rectangle[i].y;
      controlPoints[(j*uDimension + i)*3 + 2] = m_rectangle[i].z;  
    }
    tmpindex++;
  }              
  index = index + tmpindex;
  
  //surface orthogonal z-axis
  tmpindex = 0;
  for (int j=index, index2=1; j<index+nzAreaPoints-1; j++, index2++) {
    tmpxSize = xSize - index2 * xSize / (nzAreaPoints-1);
    tmpySize = ySize - index2 * ySize / (nzAreaPoints-1);
    makeNurbsRectangle(tmpxSize, tmpySize, zPosition, nuAreaPoints, uDegree);
    for (size_t i=0; i<m_rectangle.size(); i++) {
      controlPoints[(j*uDimension + i)*3    ] = m_rectangle[i].x;  
      controlPoints[(j*uDimension + i)*3 + 1] = m_rectangle[i].y;
      controlPoints[(j*uDimension + i)*3 + 2] = m_rectangle[i].z;  
    }
    tmpindex++;
  }              
  index = index + tmpindex;

  makeKnotvectors(uOrder, uDimension, vOrder, vDimension);

  for (size_t i=0; i<m_uKnots.size(); i++) {
    uKnots[i] = m_uKnots[i];
  }
  for (size_t i=0; i<m_vKnots.size(); i++) {
    vKnots[i] = m_vKnots[i];
  }  
  
  for (int i=0; i<uDimension*vDimension; i++) {
    weights[i] = 1.0f;
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


Node*
NodeBox::toNurbs(int nuAreaPoints, int uDegree, int nvAreaPoints, int vDegree)
{
  //build a box with NURBS from 6 seperate NURBS-surfaces
  
  Node *node = m_scene->createNode("NurbsGroup");
  
  FieldValue *box = new MFNode();

  m_nuAreaPoints = nuAreaPoints;
  m_nvAreaPoints = nvAreaPoints;
  
  m_uOrder = uDegree + 1;
  m_vOrder = vDegree + 1;
  m_uDimension = nuAreaPoints;
  m_vDimension = nvAreaPoints;
  
  for (int i=1; i<=6; i++) {
    Node *surface = makeNurbsSurfaces(i);
    box = box->addNode(surface);
  } 
  
  ((NodeNurbsGroup *)node)->children((MFNode *)box);
  
  return node;
}



Node*
NodeBox::makeNurbsSurfaces(int surface)
{
  NodeNurbsSurface *node = (NodeNurbsSurface *)
                            m_scene->createNode("NurbsSurface");
  const float *fsize = size()->getValue();
  
  float xSize = fsize[0];
  float ySize = fsize[1];
  float zSize = fsize[2];  

  float xPosition = xSize / 2;
  float yPosition = ySize / 2;
  float zPosition = zSize / 2;
  float dx = (float) xSize / (m_nuAreaPoints-1);
  float dy = (float) ySize / (m_nuAreaPoints-1);
  float dz = (float) zSize / (m_nvAreaPoints-1);
  int uOrder = m_uOrder;
  int vOrder = m_vOrder;
  int uDimension = m_uDimension;
  int vDimension = m_vDimension;
  float *uKnots = NULL;
  float *vKnots = NULL;
  float *controlPoints = NULL;
  float *weights = NULL;

  switch (surface) {
  case 1:
    uKnots = new float[uDimension + uOrder];
    vKnots = new float[vDimension + vOrder];
    controlPoints = new float[uDimension * vDimension * 3];
    weights = new float[uDimension * vDimension];      
    makeKnotvectors(uOrder, uDimension, vOrder, vDimension);
    for (int j = 0; j < vDimension; j++) {
      for (int i = 0; i < uDimension; i++) {
        controlPoints[(j*uDimension + i)*3    ] = xPosition;
        controlPoints[(j*uDimension + i)*3 + 1] = (float) +ySize / 2 - dy * i;
        controlPoints[(j*uDimension + i)*3 + 2] = (float) -zSize /2 + dz * (vDimension - 1 - j);
        weights[j*uDimension + i] = 1.0f;
      }
    }    
    break;
  case 2:
    uKnots = new float[uDimension + uOrder];
    vKnots = new float[vDimension + vOrder];
    controlPoints = new float[uDimension * vDimension * 3];
    weights = new float[uDimension * vDimension];      
    makeKnotvectors(uOrder, uDimension, vOrder, vDimension);
    for (int j = 0; j < vDimension; j++) {
      for (int i = 0; i < uDimension; i++) {
        controlPoints[(j*uDimension + i)*3    ] = -xPosition;
        controlPoints[(j*uDimension + i)*3 + 1] = (float) -ySize / 2 + dy * i;
        controlPoints[(j*uDimension + i)*3 + 2] = (float) -zSize /2 + dz * (vDimension - 1 - j);
        weights[j*uDimension + i] = 1.0f;
      }
    }
    
    break;
  case 3:
    uKnots = new float[uDimension + uOrder];
    vKnots = new float[vDimension + vOrder];
    controlPoints = new float[uDimension * vDimension * 3];
    weights = new float[uDimension * vDimension];      
    makeKnotvectors(uOrder, uDimension, vOrder, vDimension);
    for (int j = 0; j < vDimension; j++) {
      for (int i = 0; i < uDimension; i++) {
        controlPoints[(j*uDimension + i)*3    ] = (float) -xSize / 2 + dx * i;
        controlPoints[(j*uDimension + i)*3 + 1] = yPosition;
        controlPoints[(j*uDimension + i)*3 + 2] = (float) -zSize /2 + dz * (vDimension - 1 - j);
        weights[j*uDimension + i] = 1.0f;
      }
    }

    break;
  case 4:    
    uKnots = new float[uDimension + uOrder];
    vKnots = new float[vDimension + vOrder];
    controlPoints = new float[uDimension * vDimension * 3];
    weights = new float[uDimension * vDimension];      
    makeKnotvectors(uOrder, uDimension, vOrder, vDimension); 
    for (int j = 0; j < vDimension; j++) {
      for (int i = 0; i < uDimension; i++) {
        controlPoints[(j*uDimension + i)*3    ] = (float) xSize / 2 - dx * i;
        controlPoints[(j*uDimension + i)*3 + 1] = -yPosition;
        controlPoints[(j*uDimension + i)*3 + 2] = (float) -zSize /2 + dz * (vDimension - 1 - j);
        weights[j*uDimension + i] = 1.0f;
      }
    }

    break;
  case 5:
    vOrder = m_uOrder;
    vDimension = m_uDimension;
    uKnots = new float[uDimension + uOrder];
    vKnots = new float[vDimension + vOrder];
    controlPoints = new float[uDimension * vDimension * 3];
    weights = new float[uDimension * vDimension];      
    makeKnotvectors(uOrder, uDimension, vOrder, vDimension);
    for (int j = 0; j < vDimension; j++) {
      for (int i = 0; i < uDimension; i++) {
        controlPoints[(j*uDimension + i)*3    ] = (float) xSize / 2 - dx * i;
        controlPoints[(j*uDimension + i)*3 + 1] = (float) -ySize /2 + dy * (vDimension - 1 - j);
        controlPoints[(j*uDimension + i)*3 + 2] = zPosition;
        weights[j*uDimension + i] = 1.0f;
      }
    }

    break;
  case 6:
    vOrder= m_uOrder;
    vDimension = m_uDimension;
    uKnots = new float[uDimension + uOrder];
    vKnots = new float[vDimension + vOrder];
    controlPoints = new float[uDimension * vDimension * 3];
    weights = new float[uDimension * vDimension];      
    makeKnotvectors(uOrder, uDimension, vOrder, vDimension);
    for (int j = 0; j < vDimension; j++) {
      for (int i = 0; i < uDimension; i++) {
        controlPoints[(j*uDimension + i)*3    ] = (float) -xSize / 2 + dx * i;
        controlPoints[(j*uDimension + i)*3 + 1] = (float) -ySize /2 + dy * (vDimension - 1 - j);
        controlPoints[(j*uDimension + i)*3 + 2] = -zPosition;
        weights[j*uDimension + i] = 1.0f;
      }
    }

    break;
  }

  for (size_t i=0; i<m_uKnots.size(); i++) {
    uKnots[i] = m_uKnots[i];
  }
  for (size_t i=0; i<m_vKnots.size(); i++) {
    vKnots[i] = m_vKnots[i];
  }

  node->setField(node->uDimension_Field(), new SFInt32(uDimension));
  node->setField(node->vDimension_Field(), new SFInt32(vDimension));
  node->uKnot(new MFFloat(uKnots, uDimension + uOrder));
  node->vKnot(new MFFloat(vKnots, vDimension + vOrder));
  node->setField(node->uOrder_Field(), new SFInt32(uOrder));
  node->setField(node->vOrder_Field(), new SFInt32(vOrder));
  node->createControlPoints(new MFVec3f(controlPoints, 
                                        uDimension * vDimension * 3));
  node->weight(new MFFloat(weights, uDimension * vDimension));

  return node;
}


void
NodeBox::makeKnotvectors(int uOrder, int uDimension, int vOrder, int vDimension)
{
  if (m_uKnots.size() > 0) {
    m_uKnots.remove(0, m_uKnots.size()-1);
  }
  if (m_vKnots.size() > 0) {
    m_vKnots.remove(0, m_vKnots.size()-1);
  }

  for (int i = 0; i < uOrder; i++) {
    m_uKnots[i] = 0.0f;
    m_uKnots[uDimension + i] = (float) (uDimension - uOrder + 1);
  }
  for (int i = 0; i < uDimension - uOrder; i++) {
    m_uKnots[uOrder + i] = (float) (i + 1);
  }

  for (int i = 0; i < vOrder; i++) {
    m_vKnots[i] = 0.0f;
    m_vKnots[vDimension + i] = (float) (vDimension - vOrder + 1);
  }
  for (int i = 0; i < vDimension - vOrder; i++) {
    m_vKnots[vOrder + i] = (float) (i + 1);
  }
}


void
NodeBox::makeNurbsRectangle(float xSize, float ySize, float zPosition, 
                            int nuAreaPoints, int uDegree)
{
  int index = 0;
  float dx = xSize / (nuAreaPoints - 1);
  float dy = ySize / (nuAreaPoints - 1);

  for (int i=0; i<uDegree-1; i++) {
    m_rectangle[index].x = 0;           
    m_rectangle[index].y = ySize / 2;           
    m_rectangle[index].z = zPosition;           
    index++;
  }
  for (int i=0; i<uDegree-1; i++) {
    m_rectangle[index].x = xSize / 2;           
    m_rectangle[index].y = ySize / 2;           
    m_rectangle[index].z = zPosition;           
    index++;
  }
  for (int i=0; i<nuAreaPoints; i++) {
    m_rectangle[index].x = xSize / 2;  
    m_rectangle[index].y = ySize / 2 - i * dy;           
    m_rectangle[index].z = zPosition;           
    index++;
  }
  for (int i=0; i<uDegree-1; i++) {
    m_rectangle[index].x = xSize / 2;          
    m_rectangle[index].y = - ySize / 2;           
    m_rectangle[index].z = zPosition;           
    index++;
  }
  for (int i=1; i<nuAreaPoints; i++) {
    m_rectangle[index].x = xSize / 2 - i * dx;          
    m_rectangle[index].y = - ySize / 2;            
    m_rectangle[index].z = zPosition;
    index++;
  }    
  for (int i=0; i<uDegree-1; i++) {
    m_rectangle[index].x = - xSize / 2;          
    m_rectangle[index].y = - ySize / 2;          
    m_rectangle[index].z = zPosition;
    index++;
    }    
  for (int i=1; i<nuAreaPoints; i++) {
    m_rectangle[index].x = - xSize / 2;              
    m_rectangle[index].y = - ySize / 2 + i * dy;         
    m_rectangle[index].z = zPosition;
    index++;
  }    
  for (int i=0; i<uDegree-1; i++) {
    m_rectangle[index].x = - xSize / 2;          
    m_rectangle[index].y = ySize / 2;         
    m_rectangle[index].z = zPosition;
    index++;
  }    
  for (int i=1; i<nuAreaPoints/2; i++) {
    m_rectangle[index].x = - xSize / 2 + 2 * i * dx;          
    m_rectangle[index].y = ySize / 2;             
    m_rectangle[index].z = zPosition;
    index++;
  }
  for (int i=0; i<uDegree-1; i++) {
    m_rectangle[index].x = 0;           
    m_rectangle[index].y = ySize / 2;           
    m_rectangle[index].z = zPosition;           
    index++;
  }
        
}

void
NodeBox::swap(int fromTo)
{
    size()->swap(fromTo);
    update();
}

void
NodeBox::createMesh(bool cleanDoubleVertices, bool triangulate)
{
    const float *fsize = size()->getValue();
    float fpoint[] = {
        -fsize[0]/2.0f,  fsize[1]/2.0f,  fsize[2]/2.0f,
        -fsize[0]/2.0f, -fsize[1]/2.0f,  fsize[2]/2.0f,
         fsize[0]/2.0f,  fsize[1]/2.0f,  fsize[2]/2.0f,
         fsize[0]/2.0f, -fsize[1]/2.0f,  fsize[2]/2.0f,
         fsize[0]/2.0f,  fsize[1]/2.0f, -fsize[2]/2.0f,
         fsize[0]/2.0f, -fsize[1]/2.0f, -fsize[2]/2.0f,
        -fsize[0]/2.0f,  fsize[1]/2.0f, -fsize[2]/2.0f,
        -fsize[0]/2.0f, -fsize[1]/2.0f, -fsize[2]/2.0f 
    };
    int icoordIndex[] = {
         0, 1, 3, 2, -1, 
         4, 5, 7, 6, -1, 
         6, 0, 2, 4, -1, 
         1, 7, 5, 3, -1, 
         6, 7, 1, 0, -1,
         4, 2, 3, 5, -1,
    }; 
    float ftexCoord[] = {
        0, 1,
        0, 0,
        1, 1,
        1, 0
    };
    int itexCoordIndex[] = {
        0, 1, 3, 2, -1, 
        0, 1, 3, 2, -1, 
        0, 1, 3, 2, -1, 
        0, 1, 3, 2, -1, 
        0, 1, 3, 2, -1,
        0, 1, 3, 2, -1,
    };

    MFVec3f *coords = new MFVec3f();
    for (unsigned int i = 0; i < sizeof(fpoint)/sizeof(float); i += 3)
        coords->appendSFValue(fpoint[i], fpoint[i + 1], fpoint[i + 2]); 

    MFInt32 *coordIndex = new MFInt32();
    for (unsigned int i = 0; i < sizeof(icoordIndex)/sizeof(int); i++)
        coordIndex->appendSFValue(icoordIndex[i]);

    MFVec2f *texCoord = new MFVec2f();
    for (unsigned int i = 0; i < sizeof(ftexCoord)/sizeof(float); i += 2)
        texCoord->appendSFValue(ftexCoord[i], ftexCoord[i + 1]);
    MyArray<MFVec2f *> texCoords;
    texCoords.append(texCoord);

    MFInt32 *texCoordIndex = new MFInt32();
    for (unsigned int i = 0; i < sizeof(itexCoordIndex)/sizeof(int); i++)
        texCoordIndex->appendSFValue(itexCoordIndex[i]);

    MFVec3f *normals = NULL;
    MFInt32 *normalIndex = NULL;
    MFColor *colors = NULL;
    MFInt32 *colorIndex = NULL;

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
                        0, meshFlags, transparency);
}

bool    
NodeBox::validHandle(int handle) 
{ 
    switch(handle) {
      case TRF: case TLF: case TRB: case TLB:
      case BRF: case BLF: case BRB: case BLB:
        return m_scene->getSelectedHandlesSize() <= 1; 
    }
    return false;
}

