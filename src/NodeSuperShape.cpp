/*
 * NodeSuperShape.cpp
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
#include "stdafx.h"

#include "NodeSuperShape.h"
#include "Proto.h"
#include "DuneApp.h"
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

ProtoSuperShape::ProtoSuperShape(Scene *scene)
  : Proto(scene, "SuperShape")
{
    ua.set( 
          addExposedField(SFFLOAT, "ua", new SFFloat(1.0f)));
    ub.set( 
          addExposedField(SFFLOAT, "ub", new SFFloat(1.0f)));
    um.set( 
          addExposedField(SFFLOAT, "um", new SFFloat(0.0f)));
    un1.set( 
          addExposedField(SFFLOAT, "un1", new SFFloat(1.0f)));
    un2.set( 
          addExposedField(SFFLOAT, "un2", new SFFloat(1.0f)));
    un3.set( 
          addExposedField(SFFLOAT, "un3", new SFFloat(1.0f)));

    va.set( 
          addExposedField(SFFLOAT, "va", new SFFloat(1.0f)));
    vb.set( 
          addExposedField(SFFLOAT, "vb", new SFFloat(1.0f)));
    vm.set( 
          addExposedField(SFFLOAT, "vm", new SFFloat(0.0f)));
    vn1.set( 
          addExposedField(SFFLOAT, "vn1", new SFFloat(1.0f)));
    vn2.set( 
          addExposedField(SFFLOAT, "vn2", new SFFloat(1.0f)));
    vn3.set( 
          addExposedField(SFFLOAT, "vn3", new SFFloat(1.0f)));

    bottom.set( 
          addField(SFBOOL, "bottom", new SFBool(false)));
    bottomBorder.set( 
          addExposedField(SFFLOAT, "bottomBorder", new SFFloat(-M_PI / 2.0),
                          new SFFloat(-M_PI / 2.0), new SFFloat(M_PI / 2.0)));
    border.set( 
          addExposedField(SFFLOAT, "border", new SFFloat(M_PI / 2.0),
                          new SFFloat(-M_PI / 2.0), new SFFloat(M_PI / 2.0)));
    ccw.set( 
          addField(SFBOOL, "ccw", new SFBool(false)));
    creaseAngle.set( 
          addField(SFFLOAT, "creaseAngle", new SFFloat(3), 
                   new SFFloat(0.0f)));
    size.set(
          addExposedField(SFVEC3F, "size", new SFVec3f(1.0f, 1.0f, 1.0f), 
                          new SFFloat(0.0f)));
    setFieldFlags(size, EIF_RECOMMENDED);
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
ProtoSuperShape::create(Scene *scene)
{
    return new NodeSuperShape(scene, this); 
}

NodeSuperShape::NodeSuperShape(Scene *scene, Proto *def)
  : MeshMorphingNode(scene, def)
{
}

NodeSuperShape::~NodeSuperShape()
{
}

static float superFormula(float angle, float a, float b, float m, 
                          float n1, float n2, float n3)
{
    float f = m * angle / 4.0;
    float c = cos(f);
    float s = sin(f);
    return pow(pow(fabs(c / a), n2) + pow(fabs(s / b), n3), -1.0f / n1);
}

void
NodeSuperShape::createMesh(bool cleanDoubleVertices, bool triangulate)
{
    SuperShapeData data;
    data.ua = ua()->getValue();
    data.ub = ub()->getValue();
    data.um = um()->getValue();
    data.un1 = un1()->getValue();
    data.un2 = un2()->getValue();
    data.un3 = un3()->getValue();
    data.va = va()->getValue();
    data.vb = vb()->getValue();
    data.vm = vm()->getValue();
    data.vn1 = vn1()->getValue();
    data.vn2 = vn2()->getValue();
    data.vn3 = vn3()->getValue();
    data.border = border()->getValue();
    data.bottomBorder = bottomBorder()->getValue();
    data.creaseAngle = creaseAngle()->getValue(); 
    const float *fsize = size()->getValue();
    data.size[0] = fsize[0];
    data.size[1] = fsize[1];
    data.size[2] = fsize[2];
    data.texCoord = texCoord()->getValue();
    data.uTessellation = uTessellation()->getValue();
    data.vTessellation = vTessellation()->getValue();

    data.cleanDoubleVertices = cleanDoubleVertices;

    createMesh(data);    
}

void
NodeSuperShape::createMesh(SuperShapeData &data)
{
    if (m_mesh)
        delete m_mesh;
    m_mesh = NULL;
    int uTess = data.uTessellation;
    int vTess = data.vTessellation;

    if (uTess <= 0) uTess = TheApp->getTessellation();
    if (vTess <= 0) vTess = TheApp->getTessellation();

    uTess++;
    vTess++;

    if (uTess < 3) return;
    if (vTess < 3) return;

    bool needTop = top()->getValue();
    if (data.border == (M_PI / 2.0))
        needTop = false;

    bool needBottom = bottom()->getValue();
    if (data.bottomBorder == - (M_PI / 2.0))
        needBottom = false;

    int size = uTess * vTess;
    int csize = size * 5;
    if (needTop) {
        size++;
        csize += uTess * 4;
    }
    if (needBottom) {
        size++;
        csize += uTess * 4;
    }

    int* ci = new int[csize];

    float low = data.bottomBorder;
    float high = data.border;
    
    if (high < low) {
        float temp = high;
        high = low;
        low = temp;
    }

    float inc1 = M_PI / (uTess - 1) * 2.0;
    float inc2 = (high - low) / (vTess - 1);
    if (inc2 < FEPSILON) return;
    int a1;
    int a2;
    float fua = data.ua;
    float fub = data.ub;
    float fum = data.um;
    float fun1 = data.un1;
    float fun2 = data.un2;
    float fun3 = data.un3;
    MyArray<float> r1(uTess);
    MyArray<float> c1(uTess);
    MyArray<float> s1(uTess);
    for (a1 = 0; a1 < uTess; a1++) {
        float angle1 = -M_PI + a1 * inc1;
        r1[a1] = superFormula(angle1, fua, fub, fum, fun1, fun2, fun3);
        c1[a1] = cos(angle1);
        s1[a1] = sin(angle1);
    }
    float fva = data.va;
    float fvb = data.vb;
    float fvm = data.vm;
    float fvn1 = data.vn1;
    float fvn2 = data.vn2;
    float fvn3 = data.vn3;
    MyArray<float> r2(vTess);
    MyArray<float> c2(vTess);
    MyArray<float> s2(vTess);
    for (a2 = 0; a2 < vTess; a2++) {
        float angle2 = low + a2 * inc2;
        r2[a2] = superFormula(angle2, fva, fvb, fvm, fvn1, fvn2, fvn3);
        c2[a2] = cos(angle2);
        s2[a2] = sin(angle2);
    }
    MFVec3f *vert = new MFVec3f(size);
    int index = 0;
    int cindex = 0;
    for (a2 = 0; a2 < vTess; a2++) {
        for (a1 = 0; a1 < uTess; a1++) {
            float x = 0;
            float y = 0;
            float z = 0;
            if ((r1[a1] == 0) || (r2[a2] == 0) || (c2[a2] == 0)) {
                z = 0;
                x = 0;
            } else {                
                if (c1[a1] == 0)
                    z = 0;
                else
                    z = data.size[2] * r1[a1] * c1[a1] * r2[a2] * c2[a2];
                if (s1[a1] == 0)
                    x = 0;
                else
                    x = data.size[0] * r1[a1] * s1[a1] * r2[a2] * c2[a2];
            }
            if ((r2[a2] == 0) || (s2[a2] == 0))
                y = 0;
            else
                y = data.size[1] * r2[a2] * s2[a2];
            vert->setSFValue(index, x, y, z);
            index++;
            int n1 = a1 + 1;
            if (n1 == uTess)
                n1 = 0;
            int n2 = a2 + 1;
            if (n2 < vTess) {
                if (a1 < uTess / 2) {
                    ci[cindex++] = a1 + a2 * uTess;
                    ci[cindex++] = a1 + n2 * uTess;
                    ci[cindex++] = n1 + n2 * uTess;
                    ci[cindex++] = n1 + a2 * uTess;
                    ci[cindex++] = -1;
                } else {
                    ci[cindex++] = a1 + n2 * uTess;
                    ci[cindex++] = n1 + n2 * uTess;
                    ci[cindex++] = n1 + a2 * uTess;
                    ci[cindex++] = a1 + a2 * uTess;
                    ci[cindex++] = -1;
                }
            }
        }
    }
    if (needTop) {
        if ((r2[vTess - 1] == 0) || (s2[vTess - 1] == 0))
            vert->setSFValue(index, 0, 0, 0);
        else
            vert->setSFValue(index, 0,
                             data.size[1] * r2[vTess - 1] * s2[vTess - 1], 0);
        for (a2 = 0; a2 < uTess; a2++) {
            ci[cindex++] = index;           
            ci[cindex++] = index - 1 - a2;
            if (a2 == uTess - 1)
                ci[cindex++] = index - 1;
            else
                ci[cindex++] = index - 2 - a2;
            ci[cindex++] = -1;
        }
        index++;
    }
    if (needBottom) {
        if ((r2[0] == 0) || (s2[0] == 0))
            vert->setSFValue(index, 0, 0, 0);
        else
            vert->setSFValue(index, 0, data.size[1] * r2[0] * s2[0], 0);
        for (a2 = 0; a2 < uTess; a2++) {
            ci[cindex++] = index;           
            ci[cindex++] = a2;
            if (a2 + 1 == uTess)
                ci[cindex++] = 0;
            else 
                ci[cindex++] = a2 + 1;
            ci[cindex++] = -1;
        }
        index++;
    }

    cindex = optimizeNormals(ci, vert, NULL, cindex, false, 
                             data.cleanDoubleVertices);    

    MFInt32 *coordIndex = new MFInt32(ci, cindex);
    MyArray<MFVec2f *> texCoords;
    Util::getTexCoords(texCoords, data.texCoord);    
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
                        texCoords, NULL, data.creaseAngle, meshFlags);
}

int
NodeSuperShape::writeProto(int f)
{
    return ((Node *)this)->writeProto(f, "", "scripted_Nodes"
#ifdef HAVE_SCRIPTED_NODES_PROTO_URL
                                            , HAVE_SCRIPTED_NODES_PROTO_URL
#endif
                                            );
}

void
NodeSuperShape::setField(int index, FieldValue *value, int cf)
{
    m_meshDirty = true;
    Node::setField(index, value, cf);
}

Node*
NodeSuperShape::toNurbs(int uTessel,  int vTessel, int uDegree, int vDegree)
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

    int uTess = uTessel;
    int vTess = vTessel; 

    uTessellation(new SFInt32(uTess));
    vTessellation(new SFInt32(vTess - 1));
    createMesh(false);
    m_meshDirty = true;
    if (m_mesh == NULL)
        return NULL;

    uTess += 1;

    int size = m_mesh->getVertices()->getSize();
    int uDimension = uTess;
    int vDimension = vTess;
    float *controlPoints = new float[size];
    float *weights = new float[size];
    float *uKnots = new float[uDimension + uOrder]; 
    float *vKnots = new float[vDimension + vOrder]; 

    for (int i = 0; i < size; i++) {
        controlPoints[i] = m_mesh->getVertices()->getValues()[i];
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
    node->setField(node->uClosed_Field(), new SFBool(true));
    node->setField(node->vClosed_Field(), new SFBool(true));
    node->createControlPoints(new MFVec3f(controlPoints, 
                                          uDimension * vDimension * 3));
    node->weight(new MFFloat(weights, uDimension * vDimension));
    node->ccw(new SFBool(m_mesh->ccw()));
    node->solid(new SFBool(m_mesh->solid()));

    node->setField(node->uTessellation_Field(), new SFInt32(uTess - 1));
    node->setField(node->vTessellation_Field(), new SFInt32(vTess - 1));

    for (int iteration = 0; iteration < 32; iteration++) {
        node->reInit();
        node->createMesh(false);    
        if (node->getVertices() != NULL) {
            assert(size == node->getVertices()->getSize() -
                           node->getNumExtraVertices() * 3);
            float *vert = new float[size];
            MFVec3f *nurbsControlPoints = node->controlPoint();    
            for (int i = 0; i < size; i++) {
                vert[i] = node->getVertices()->getValues()[i];
                float meshValue = m_mesh->getVertices()->getValues()[i];
                float nurbsValue = nurbsControlPoints->getValues()[i];
                if (fabs(vert[i]) > FEPSILON)
                    vert[i] = nurbsValue * meshValue / vert[i];
            }
            for (int i = 0; i < size; i++) {
                nurbsControlPoints->setValue(i, vert[i]);
            }
        }
    }
    node->setField(node->uTessellation_Field(), new SFInt32(0));
    node->setField(node->vTessellation_Field(), new SFInt32(0));

    uTessellation(new SFInt32(oldUTess));
    vTessellation(new SFInt32(oldVTess));

    return node;   
}

void * 
NodeSuperShape::initializeData(void)
{
    SuperShapeData *data = new SuperShapeData();

    data->ua = ua()->getValue();
    data->ub = ub()->getValue();
    data->um = um()->getValue();
    data->un1 = un1()->getValue();
    data->un2 = un2()->getValue();
    data->un3 = un3()->getValue();
    data->va = va()->getValue();
    data->vb = vb()->getValue();
    data->vm = vm()->getValue();
    data->vn1 = vn1()->getValue();
    data->vn2 = vn2()->getValue();
    data->vn3 = vn3()->getValue();
    data->border = border()->getValue();
    data->creaseAngle = creaseAngle()->getValue();
    const float *fsize = size()->getValue();
    data->size[0] = fsize[0];
    data->size[1] = fsize[1];
    data->size[2] = fsize[2];
    data->texCoord = texCoord()->getValue();
    data->uTessellation = uTessellation()->getValue();
    data->vTessellation = vTessellation()->getValue();

    return data;
}


void
NodeSuperShape::loadDataFromInterpolators(void* superShapeData,
                                          Interpolator *inter, 
                                          int field, float key)
{
    SuperShapeData *data = (SuperShapeData *)superShapeData;
    // all interpolators are scalarInterpolators, 
    // one float is sufficent as target of "interpolate()"
    if (field == ua_Field())
        inter->interpolate(key, &(data->ua));
    else if (field == ub_Field())
        inter->interpolate(key, &(data->ub));
    else if (field == um_Field())
        inter->interpolate(key, &(data->um));
    else if (field == un1_Field())
        inter->interpolate(key, &(data->un1));
    else if (field == un2_Field())
        inter->interpolate(key, &(data->un2));
    else if (field == un3_Field())
        inter->interpolate(key, &(data->un3));
    else if (field == va_Field())
        inter->interpolate(key, &(data->va));
    else if (field == vb_Field())
        inter->interpolate(key, &(data->vb));
    else if (field == vm_Field())
        inter->interpolate(key, &(data->vm));
    else if (field == vn1_Field())
        inter->interpolate(key, &(data->vn1));
    else if (field == vn2_Field())
        inter->interpolate(key, &(data->vn2));
    else if (field == vn3_Field())
        inter->interpolate(key, &(data->vn3));
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
NodeSuperShape::createMeshFromData(void* superShapeData, bool optimize)
{
    m_meshDirty = true;
    SuperShapeData *data = (SuperShapeData *)superShapeData;
    data->cleanDoubleVertices = optimize;
    createMesh(*data);
}

void
NodeSuperShape::finalizeData(void* data)
{
    delete (SuperShapeData *) data;
}


void
NodeSuperShape::drawHandles()
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
        state.drawHandle(spereCorners[i]);
    }
    state.endDrawHandles();
    glPopName();
    glPopAttrib();
    glPopMatrix(); 
}

Vec3f
NodeSuperShape::getHandle(int handle, int *constraint , int *field)
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
NodeSuperShape::setHandle(int handle, const Vec3f &v)
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
NodeSuperShape::flip(int index)
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
    } else if (index == 2) 
        MeshBasedNode::flip(index);
}



