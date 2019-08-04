/*
 * NodeSuperEllipsoid.cpp
 *
 * Copyright (C) 1999 Stephen F. White, 2004 J. "MUFTI" Scheurich
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
#include <math.h>
#include "stdafx.h"

#ifdef _WIN32
# define M_PI_2 (M_PI/2.0)
#endif

#include "DuneApp.h"
#include "NodeSuperEllipsoid.h"
#include "Proto.h"
#include "FieldValue.h"
#include "Scene.h"
#include "MyMesh.h"
#include "SFFloat.h"
#include "SFInt32.h"
#include "SFBool.h"
#include "NodeIndexedFaceSet.h"
#include "NodeCoordinate.h"
#include "NodeNormal.h"
#include "NodeTextureCoordinate.h"
#include "NodeNurbsSurface.h"
#include "RenderState.h"
#include "Util.h"
 
ProtoSuperEllipsoid::ProtoSuperEllipsoid(Scene *scene)
  : Proto(scene, "SuperEllipsoid")
{
    n1.set( 
          addExposedField(SFFLOAT, "n1", new SFFloat(1.0f), new SFFloat(0.0f)));
    n2.set( 
          addExposedField(SFFLOAT, "n2", new SFFloat(1.0f), new SFFloat(0.0f)));

    border.set( 
          addExposedField(SFFLOAT, "border", new SFFloat(M_PI / 2.0),
                          new SFFloat(-M_PI / 2.0), new SFFloat(M_PI / 2.0)));
    bottom.set( 
          addField(SFBOOL, "bottom", new SFBool(false)));
    bottomBorder.set( 
          addExposedField(SFFLOAT, "bottomBorder", new SFFloat(-M_PI / 2.0),
                          new SFFloat(-M_PI / 2.0), new SFFloat(M_PI / 2.0)));
    ccw.set( 
          addField(SFBOOL, "ccw", new SFBool(false)));
    creaseAngle.set( 
          addField(SFFLOAT, "creaseAngle", new SFFloat(0.7854f), 
                   new SFFloat(0.0f)));

    size.set(
          addExposedField(SFVEC3F, "size", new SFVec3f(1.0f, 1.0f, 1.0f), 
                          new SFFloat(0.0f)));
    solid.set( 
          addField(SFBOOL, "solid", new SFBool(false)));
    texCoord.set(
          // workaround against PROTO implementation bug in FreeWRL
          addField/*addExposedField*/(SFNODE, "texCoord", new SFNode(NULL), 
                          TEXTURE_COORDINATE_NODE));
    top.set( 
          addField(SFBOOL, "top", new SFBool(false)));
    uTessellation.set(
          addExposedField(SFINT32, "uTessellation", new SFInt32(0)));
    vTessellation.set(
          addExposedField(SFINT32, "vTessellation", new SFInt32(0)));

    addURLs(URL_SCRIPTED_NODES);
}

Node *
ProtoSuperEllipsoid::create(Scene *scene)
{
    return new NodeSuperEllipsoid(scene, this); 
}

NodeSuperEllipsoid::NodeSuperEllipsoid(Scene *scene, Proto *def)
  : MeshMorphingNode(scene, def)
{
}

NodeSuperEllipsoid::~NodeSuperEllipsoid()
{
}

static float cossign(float f)
   {
   // range -PI to PI
   if ((f < M_PI_2 + FEPSILON) && (f > M_PI_2 - FEPSILON))
       return 0;
   if (f > M_PI_2)
       return -1;
   if ((f < -M_PI_2 + FEPSILON) && (f > -M_PI_2 - FEPSILON))
       return 0;
   if (f > -M_PI_2)
       return 1;
   // -PI < x < -PI/2
   return -1;
   }

static float sinsign(float f)
   {
   // range -PI to PI
   if ((f < M_PI + FEPSILON) && (f > M_PI - FEPSILON))
       return 0;
   if (f > M_PI)
       return -1;
   if ((f < 0 + FEPSILON) && (f > 0 - FEPSILON))
       return 0;
   if (f > 0)
       return 1;
   if ((f < -M_PI + FEPSILON) && (f > -M_PI - FEPSILON))
       return 0;
   if (f < -M_PI)
       return 1;
   // -PI < x < 0
   return -1;
   }

    
float NodeSuperEllipsoid::superellipse1xy(float angle1, float n2)
   {
   return cossign(angle1) * pow(fabs(cos(angle1)), n2);
   }
    
float NodeSuperEllipsoid::superellipse1z(float angle1, float n2)
   {
   return sinsign(angle1)* pow(fabs(sin(angle1)), n2);
   }
    
float NodeSuperEllipsoid::superellipse2x(float angle2, float n1)
   {
   float n = n1;
   if (fabs(n) < 1e-5)
       n = 1e-5;
   return cossign(angle2) * pow(fabs(cos(angle2)), n);
   }
    
float NodeSuperEllipsoid::superellipse2y(float angle2, float n1)
   {
   float n = n1;
   if (fabs(n) < 1e-5)
       n = 1e-5;
   return sinsign(angle2) * pow(fabs(sin(angle2)), n);
   }
    
void
NodeSuperEllipsoid::createMesh(SuperEllipsoidData *data, bool cleanVertices)
{
    int uTess = uTessellation()->getValue();
    int vTess = vTessellation()->getValue();
    
    if (uTess <= 0) uTess = TheApp->getTessellation();
    if (vTess <= 0) vTess = TheApp->getTessellation();

    uTess++;
    vTess++;

    if (uTess < 3) return;
    if (vTess < 3) return;

    bool needTop = top()->getValue();
    if (data->border == (M_PI / 2.0))
        needTop = false;

    bool needBottom = bottom()->getValue();
    if (data->bottomBorder == - (M_PI / 2.0))
        needBottom = false;

    int size = 0;
    int csize = 0;
    size += uTess * vTess;
    csize += 8 * size;
    if (needTop) {
        size++;
        csize += uTess * 4;
    }
    if (needBottom) {
        size++;
        csize += uTess * 4;
    }
    int* ci = new int[csize];

    float low = data->bottomBorder;
    float high = data->border;

    if (high < low) {
        float temp = high;
        high = low;
        low = temp;
    }

    float inc1 = (high - low) / (vTess - 1);
    float inc2 = M_PI / (uTess - 1) * 2.0;
    int a1;
    int a2;
    MyArray<float> s1xy(vTess);
    MyArray<float> s1z(vTess);
    for (a1 = 0; a1 < vTess; a1++) {
        float angle1 = low + a1 * inc1;
        s1xy[a1] = superellipse1xy(angle1, data->n2);
        s1z[a1] = superellipse1z(angle1, data->n2);
    }
    MyArray<float> s2x(uTess);
    MyArray<float> s2y(uTess);
    for (a2 = 0; a2 < uTess; a2++) {
        float angle2 = -M_PI + a2 * inc2;
        s2x[a2] = superellipse2x(angle2, data->n1);
        s2y[a2] = superellipse2y(angle2, data->n1);
    }
    const float *fsize = data->size;
    MFVec3f *vert = new MFVec3f(size);
    int index=0;
    int cindex=0;
    for (a2 = 0; a2 < vTess; a2++) {
        for (a1 = 0; a1 < uTess; a1++) {
            vert->setSFValue(index, fsize[0] * s1xy[a2] * s2y[a1], 
                                    fsize[1] * s1z[a2], 
                                    fsize[2] * s1xy[a2] * s2x[a1]);
            index++;
            int c2 = a2 + 1;
            if (c2 == vTess)
                c2 = 0;    
            int c1 = a1 + 1;
            if (data->triangulate) {
                if (c1 < uTess) {
                    if (a1 < (uTess / 2)) {
                        ci[cindex++] = a1 + c2 * uTess;
                        ci[cindex++] = c1 + c2 * uTess;
                        ci[cindex++] = a1 + a2 * uTess;
                        ci[cindex++] = -1;
                        ci[cindex++] = a1 + a2 * uTess;
                        ci[cindex++] = c1 + c2 * uTess;
                        ci[cindex++] = c1 + a2 * uTess;
                        ci[cindex++] = -1;
                    } else {
                        ci[cindex++] = c1 + c2 * uTess;
                        ci[cindex++] = c1 + a2 * uTess;
                        ci[cindex++] = a1 + c2 * uTess;
                        ci[cindex++] = -1;
                        ci[cindex++] = a1 + a2 * uTess;
                        ci[cindex++] = a1 + c2 * uTess;
                        ci[cindex++] = c1 + a2 * uTess;
                        ci[cindex++] = -1;
                    }
                }
            } else {
                if (c1 < uTess) {
                    if (a1 < (uTess / 2)) {
                        ci[cindex++] = a1 + c2 * uTess;
                        ci[cindex++] = c1 + c2 * uTess;
                        ci[cindex++] = c1 + a2 * uTess;
                        ci[cindex++] = a1 + a2 * uTess;
                        ci[cindex++] = -1;
                    } else {
                        ci[cindex++] = c1 + c2 * uTess;
                        ci[cindex++] = c1 + a2 * uTess;
                        ci[cindex++] = a1 + a2 * uTess;
                        ci[cindex++] = a1 + c2 * uTess;
                        ci[cindex++] = -1;
                    }
                }
            }
        }
    }
    if (needTop) {
        vert->setSFValue(index, 0, fsize[1] * s1z[uTess - 1], 0);
        for (a2 = 0; a2 < vTess - 1; a2++) {
            ci[cindex++] = index;           
            ci[cindex++] = index - 1 - a2;
            ci[cindex++] = index - 2 - a2;
            ci[cindex++] = -1;
        }
        index++;
    }
    if (needBottom) {
        vert->setSFValue(index, 0, fsize[1] * s1z[0], 0);
        for (a2 = 0; a2 < vTess - 1; a2++) {
            ci[cindex++] = index;           
            ci[cindex++] = a2;
            ci[cindex++] = a2 + 1;
            ci[cindex++] = -1;
        }
        index++;
    }
    cindex = optimizeNormals(ci, vert, NULL, cindex, false, cleanVertices);  
    MFInt32 *coordIndex = new MFInt32(ci, cindex);

    MyArray<MFVec2f *> texCoords;
    Util::getTexCoords(texCoords, texCoord()->getValue());    
    if (texCoords.size() == 0) {
        MFVec2f *texCoord = generateTextureCoordinates(vert, coordIndex, 0.0f);
        texCoords.append(texCoord);
    }

    bool bsolid = solid()->getValue();
    int meshFlags = ccw()->getValue() ? MESH_CCW : 0;
    if (bsolid)
        meshFlags |= MESH_SOLID;
    meshFlags |= MESH_COLOR_PER_VERTEX;
    meshFlags |= MESH_NORMAL_PER_VERTEX;

    m_mesh = new MyMesh(this, vert, coordIndex, NULL, NULL, NULL, NULL, 
                        texCoords, NULL, creaseAngle()->getValue(), meshFlags, 
                        0);
}

void
NodeSuperEllipsoid::createMesh(bool cleanVertices, bool triangulate)
{
    SuperEllipsoidData data;
    data.triangulate = triangulate;
    data.n1 = n1()->getValue();
    data.n2 = n2()->getValue();
    data.border = border()->getValue();
    data.bottomBorder = bottomBorder()->getValue();
    const float *fsize = size()->getValue();
    data.size[0] = fsize[0];
    data.size[1] = fsize[1];
    data.size[2] = fsize[2];

    createMesh(&data, cleanVertices);    
}

int
NodeSuperEllipsoid::writeProto(int f)
{
    return ((Node *)this)->writeProto(f, "", "scripted_Nodes"
#ifdef HAVE_SCRIPTED_NODES_PROTO_URL
                                            , HAVE_SCRIPTED_NODES_PROTO_URL
#endif
                                            );
}

void
NodeSuperEllipsoid::setField(int index, FieldValue *value, int cf)
{
    m_meshDirty = true;
    Node::setField(index, value, cf);
}

Node*
NodeSuperEllipsoid::toNurbs(int uTess,  int vTess, int uDegree, int vDegree)
{   
    if (meshDirty()) {
        createMesh();
        m_meshDirty = false;
    }
    NodeNurbsSurface *node = (NodeNurbsSurface *) m_scene->createNode(
                             "NurbsSurface");

    int vOrder = vDegree + 1;
    int uOrder = uDegree + 1;

    int oldUTess = uTessellation()->getValue();
    int oldVTess = vTessellation()->getValue();

    bool storeOldTessellation = false;
    if ((oldUTess != uTess) || (oldVTess != vTess)) {
        storeOldTessellation = true;
        uTessellation(new SFInt32(uTess));
        vTessellation(new SFInt32(vTess));
        createMesh();
        m_meshDirty = true;
    }

    uTess++;
    vTess++;

    int uDimension = uTess;
    int vDimension = vTess;

    bool needTop = top()->getValue();
    if (border()->getValue() == (M_PI / 2.0))
        needTop = false;
    if (needTop)
        vDimension++;

    bool needBottom = bottom()->getValue();
    if (bottomBorder()->getValue() == - (M_PI / 2.0))
        needBottom = false;  
    if (needBottom)
        vDimension++;

    int size = uDimension * vDimension;

    float *controlPoints = new float[size * 3];
    float *weights = new float[size];
    float *uKnots = new float[uDimension + uOrder]; 
    float *vKnots = new float[vDimension + vOrder]; 

    const float *vertices = m_mesh->getVertices()->getValues();
    int offset = 0;
    if (needBottom) {
        offset = uTess * 3;
        int bottomOffset = m_mesh->getVertices()->getSize() - 3;
        for (int i = 0; i < uTess; i++)
            for (int j = 0; j < 3; j++)
                controlPoints[i * 3 + j] = vertices[bottomOffset + j];
    }
    
    for (int i = 0; i < uTess * vTess * 3; i++) {
        controlPoints[i + offset] = vertices[i];
    }

    if (needTop) {
        int topOffset = m_mesh->getVertices()->getSize() - 3;
        if (needBottom)
            topOffset -= 3;
        offset += uTess * vTess * 3;
        const float *vertices = m_mesh->getVertices()->getValues();
        for (int i = 0; i < uTess; i++)
            for (int j = 0; j < 3; j++)
                controlPoints[offset + i * 3 + j] = vertices[topOffset + j];
    }
    
    for(int i = 0; i < size; i++){
        weights[i] = 1;
    }
    //set u-knotvektor
    for(int i = 0; i < uOrder; i++){
        uKnots[i] = 0.0f;
        uKnots[i + uDimension] = (float) (uDimension - uOrder + 1);
    }
    for(int i = 0; i < (uDimension - uOrder); i++){
        uKnots[i + uOrder] = (float) (i + 1);  
    } 
    //set v-knotvektor
    for(int i = 0; i < vOrder; i++){
        vKnots[i] = 0.0f;
        vKnots[i + vDimension] = (float) (vDimension - vOrder + 1);
    }
    for(int i = 0; i < (vDimension - vOrder); i++){
        vKnots[i + vOrder] = (float) (i + 1);  
    } 
    node->setField(node->uDimension_Field(), new SFInt32(uDimension));
    node->setField(node->vDimension_Field(), new SFInt32(vDimension));
    node->uKnot(new MFFloat(uKnots, uDimension + uOrder));
    node->vKnot(new MFFloat(vKnots, vDimension + vOrder));
    node->setField(node->uOrder_Field(), new SFInt32(uOrder));
    node->setField(node->vOrder_Field(), new SFInt32(vOrder));
    node->createControlPoints(new MFVec3f(controlPoints, size * 3));
    node->weight(new MFFloat(weights, size));
    node->ccw(new SFBool(m_mesh->ccw()));
    node->solid(new SFBool(m_mesh->solid()));

    node->setField(node->uTessellation_Field(), new SFInt32(uTess - 1));
    node->setField(node->vTessellation_Field(), new SFInt32(vTess - 1));

    for (int iteration = 0; iteration < 32; iteration++) {
        node->reInit();
        node->createMesh(false);    
        if (node->getVertices() != NULL) {
            float *vert = new float[size * 3];
            MFVec3f *nurbsControlPoints = node->getControlPoints();    
            for (int i = 0; i < size * 3; i++) {
                vert[i] = node->getVertices()->getValues()[i];
                float meshValue =m_mesh->getVertices()->getValues()[i];
                float nurbsValue = nurbsControlPoints->getValues()[i];
                if (fabs(vert[i]) > FEPSILON) {
                    vert[i] = nurbsValue * meshValue / vert[i];
                }
            }
            node->setControlPoints(new MFVec3f(vert, size * 3));
        }
    }
    node->setField(node->uTessellation_Field(), new SFInt32(0));
    node->setField(node->vTessellation_Field(), new SFInt32(0));

    if (storeOldTessellation) {
        uTessellation(new SFInt32(oldUTess));
        vTessellation(new SFInt32(oldVTess));
    }

    return node;   
}

void *
NodeSuperEllipsoid::initializeData(void)
{
    SuperEllipsoidData *data = new SuperEllipsoidData();
    data->n1 = n1()->getValue();
    data->n2 = n2()->getValue();
    data->border = border()->getValue();
    data->bottomBorder = bottomBorder()->getValue();
    const float *fsize = size()->getValue();
    data->size[0] = fsize[0];
    data->size[1] = fsize[1];
    data->size[2] = fsize[2];
    return data; 
}

void
NodeSuperEllipsoid::loadDataFromInterpolators(void *superEllipsoidData,
                                              Interpolator *inter, 
                                              int field, float key)
{
    SuperEllipsoidData *data = (SuperEllipsoidData *)superEllipsoidData;
    if (field == n1_Field())
        inter->interpolate(key, &(data->n1));
    else if (field == n2_Field())
        inter->interpolate(key, &(data->n2));
    else if (field == border_Field())
        inter->interpolate(key, &(data->border));
    else if (field == bottomBorder_Field())
        inter->interpolate(key, &(data->bottomBorder));
    else if (field == size_Field())
        inter->interpolate(key, data->size);
    else
        assert(0);
}

void
NodeSuperEllipsoid::createMeshFromData(void* superEllipsoidData, bool optimize)
{
    SuperEllipsoidData *data = (SuperEllipsoidData *)superEllipsoidData;
    m_meshDirty = true;
    createMesh(data, optimize);
}

void
NodeSuperEllipsoid::finalizeData(void* data)
{
    delete (SuperEllipsoidData *) data;
}

void
NodeSuperEllipsoid::drawHandles()
{
    const float* fsize = size()->getValue();
    RenderState state;

    glPushMatrix();
    glScalef(fsize[0]*1.0f,fsize[1]*1.0f,fsize[2]*1.0f);
    glPushAttrib(GL_LIGHTING);
    glDisable(GL_LIGHTING);
    glPushName(0);
        
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
NodeSuperEllipsoid::getHandle(int handle, int *constraint , int *field)
{
    *field = size_Field();
    const float *fsize = size()->getValue();
    Vec3f ps(fsize[0], fsize[1], fsize[2]);
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
NodeSuperEllipsoid::setHandle(int handle, const Vec3f &v)
{
    const float *fsize = size()->getValue();
    Vec3f newSize(fsize[0], fsize[1], fsize[2]);
    switch (handle) {
      case STLF: case STLB:
        newSize.z = v.z*Vec3f(spereCorners[handle]).z;
        m_scene->setField(this, size_Field(), new SFVec3f(Vec3f(newSize))); 
        break;
      case SBRB:case SBLB:
        newSize.y = v.y*Vec3f(spereCorners[handle]).y;
        m_scene->setField(this, size_Field(), new SFVec3f(Vec3f(newSize))); 
        break;
      case STRB:case SBLF:
        newSize.x = v.x * Vec3f(spereCorners[handle]).x;
        m_scene->setField(this, size_Field(), new SFVec3f(Vec3f(newSize))); 
        break;
    }
}

void
NodeSuperEllipsoid::flip(int index)
{
    if (index == 1) {
        // swap top/border with bottom/bottomBorder
        bool top = this->top()->getValue();
        float border = this->border()->getValue();
        bool bottom = this->bottom()->getValue();
        float bottomBorder = this->bottomBorder()->getValue();

        this->top(new SFBool(bottom));
        this->border(new SFFloat(-bottomBorder));
        this->bottom(new SFBool(top));
        this->bottomBorder(new SFFloat(-border));
    }
}

void
NodeSuperEllipsoid::swap(int fromTo)
{
    if (fromTo != SWAP_XZ) {
        // can not handle a xy- or yz-swap, convert to IndexedFaceSet
        Node *mesh = m_scene->convertToIndexedFaceSet(this);
        if (mesh != NULL)
            mesh->swap(fromTo);
    }
}

