/*
 * NodeNurbsTextureCoordinate.cpp
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

#include <stdlib.h>
#include <stdio.h>

#ifndef _WIN32
# include "stdlib.h"
#endif

#include "stdafx.h"
#include "NodeNurbsTextureCoordinate.h"
#include "Scene.h"
#include "FieldValue.h"
#include "SFInt32.h"
#include "MFFloat.h"
#include "MFInt32.h"
#include "MFVec2f.h"
#include "MFVec3f.h"
#include "SFNode.h"
#include "SFBool.h"
#include "SFVec3f.h"
#include "Vec2f.h"
#include "Vec3f.h"
#include "RenderState.h"
#include "DuneApp.h"
#include "NodeCoordinate.h"
#include "NodeNormal.h"
#include "NodeTextureCoordinate.h"
#include "Util.h"
#include "Field.h"
#include "ExposedField.h"
#include "NodeNurbsSurface.h"
#include "NodeNurbsTrimmedSurface.h"
#include "MyMesh.h"

ProtoNurbsTextureCoordinate::ProtoNurbsTextureCoordinate(Scene *scene,
                                                         const char *name)
  : Proto(scene, name)
{
    addElements();
}

ProtoNurbsTextureCoordinate::ProtoNurbsTextureCoordinate(Scene *scene)
  : Proto(scene, "NurbsTextureCoordinate")
{
    addElements();
}


void
ProtoNurbsTextureCoordinate::addElements(void)
{
    uDimension.set(
          addField(SFINT32, "uDimension", new SFInt32(0), new SFInt32(0)));
    vDimension.set(
          addField(SFINT32, "vDimension", new SFInt32(0), new SFInt32(0)));
    uKnot.set(
          addField(MFFLOAT, "uKnot", new MFFloat()));
    vKnot.set(
          addField(MFFLOAT, "vKnot", new MFFloat()));
    uOrder.set(
          addField(SFINT32, "uOrder", new SFInt32(3), new SFInt32(2)));
    vOrder.set(
          addField(SFINT32, "vOrder", new SFInt32(3), new SFInt32(2)));

    ExposedField* cpoint = new ExposedField(MFVEC2F, "controlPoint", 
                                            new MFVec2f());
    controlPoint.set(addExposedField(cpoint));

    weight.set(
          addExposedField(MFFLOAT, "weight", new MFFloat(), new SFFloat(0.0f)));
}

Node *
ProtoNurbsTextureCoordinate::create(Scene *scene)
{
    return new NodeNurbsTextureCoordinate(scene, this); 
}

NodeNurbsTextureCoordinate::NodeNurbsTextureCoordinate(Scene *scene, Proto *proto)
  : Node(scene, proto)
{
    m_nurbs = (NodeNurbsSurface *)m_scene->createNode("NurbsSurface");;
    m_dirty = true;
    m_points = new MFVec2f();
}

NodeNurbsTextureCoordinate::~NodeNurbsTextureCoordinate()
{
}

MFVec2f *
NodeNurbsTextureCoordinate::getPoint()
{
    if (m_dirty)
        createNurbsData();
    return m_points;
}

void
NodeNurbsTextureCoordinate::createNurbsData()
{
    m_dirty = false;

    float *newControlPoints = new float[controlPoint()->getSFSize() * 3];
    float *newWeights = (float *)weight()->getValues();
    for (int i = 0; i < controlPoint()->getSFSize(); i++) {
        newControlPoints[i * 3] = controlPoint()->getValue(i)[0];
        newControlPoints[i * 3 + 1] = controlPoint()->getValue(i)[1];
        newControlPoints[i * 3 + 2] = 0;
    }
    int newUDimension = uDimension()->getValue();
    float *newUKnots = (float *)uKnot()->getValues();
    int newUOrder = uOrder()->getValue();
    int newVDimension = vDimension()->getValue();
    float *newVKnots = (float *)vKnot()->getValues();
    int newVOrder = vOrder()->getValue();
    
    m_nurbs->uKnot(new MFFloat(newUKnots, newUDimension + newUOrder));
    m_nurbs->vKnot(new MFFloat(newVKnots, newVDimension + newVOrder));
    m_nurbs->setField(m_nurbs->uOrder_Field(), new SFInt32(newUOrder));
    m_nurbs->setField(m_nurbs->vOrder_Field(), new SFInt32(newVOrder));
    m_nurbs->setField(m_nurbs->uDimension_Field(), new SFInt32(newUDimension));
    m_nurbs->setField(m_nurbs->vDimension_Field(), new SFInt32(newVDimension));
    m_nurbs->weight(new MFFloat(newWeights, controlPoint()->getSFSize()));
    m_nurbs->createControlPoints(new MFVec3f(newControlPoints, 
                                 controlPoint()->getSize() / 2 * 3));
    int uTess = 0;
    int vTess = 0;
    if (hasParent()) {
        if (getParent()->getType() == VRML_NURBS_SURFACE) {
            NodeNurbsSurface *surface = (NodeNurbsSurface *)getParent();
            uTess = surface->uTessellation()->getValue();
            vTess = surface->vTessellation()->getValue();
        } else if (getParent()->getType() == X3D_NURBS_TRIMMED_SURFACE) {
            NodeNurbsTrimmedSurface *surface = (NodeNurbsTrimmedSurface *)
                                               getParent();
            uTess = surface->uTessellation()->getValue();
            vTess = surface->vTessellation()->getValue();
        }
    }
    m_nurbs->uTessellation(new SFInt32(uTess));
    m_nurbs->vTessellation(new SFInt32(vTess));

    m_nurbs->createMesh();

    MyMesh *mesh = m_nurbs->getMesh();
    for (int i = 0; i < m_points->getSFSize(); i++)
        m_points->removeSFValue(0);
    if (mesh != NULL) {
        float minX = 0;
        float maxX = 0;
        float minY = 0;
        float maxY = 0;
        if (mesh->getVertices()->getSFSize() > 0) {
            minX = mesh->getVertices()->getVec(0).x;
            maxX = mesh->getVertices()->getVec(0).x;
            minY = mesh->getVertices()->getVec(0).y;
            maxY = mesh->getVertices()->getVec(0).y;
        }
        for (int i = 0; i < mesh->getVertices()->getSFSize(); i++) {
            Vec3f vec = mesh->getVertices()->getVec(i);
            if (vec.x < minX)
                minX = vec.x;
            if (vec.x > maxX)
                maxX = vec.x;
            if (vec.y < minY)
                minY = vec.y;
            if (vec.y > maxY)
                maxY = vec.y;
        }
        float divX = maxX - minX;
        if (divX == 0)
            divX = 1;
        float divY = maxY - minY;
        if (divY == 0)
            divY = 1;        
        for (int i = 0; i < mesh->getVertices()->getSFSize(); i++) {
            Vec3f vec = mesh->getVertices()->getVec(i);
            m_points->appendSFValue((vec.x - minX) / divX,
                                    (vec.y - minY) / divY);
        }
    }
}

void    
NodeNurbsTextureCoordinate::setField(int index, FieldValue *value, int cf)
{
    Node::setField(index, value, cf);
    m_dirty = true;
}
