/*
 * NodeNurbsSweptSurface.cpp
 *
 * Copyright (C) 1999 Stephen F. White, 2008 J. "MUFTI" Scheurich
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

#include "stdafx.h"
#include "NodeNurbsSweptSurface.h"
#include "Scene.h"
#include "FieldValue.h"
#include "SFInt32.h"
#include "MyMesh.h"
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
#include "NodeIndexedFaceSet.h"
#include "NodeNurbsCurve2D.h"
#include "NurbsSurfaceDegreeElevate.h"
#include "Util.h"
#include "Field.h"
#include "ExposedField.h"
#include "MoveCommand.h"

ProtoNurbsSweptSurface::ProtoNurbsSweptSurface(Scene *scene)
  : GeometryProto(scene, "NurbsSweptSurface")
{
    crossSectionCurve.set(
          addExposedField(SFNODE, "crossSectionCurve", new SFNode(NULL), 
                          NURBS_CONTROL_CURVE_NODE));
    trajectoryCurve.set(
          addExposedField(SFNODE, "trajectoryCurve", new SFNode(NULL), 
                          VRML_NURBS_CURVE));
    ccw.set(
          addField(SFBOOL, "ccw", new SFBool(true)));
    solid.set(
          addField(SFBOOL, "solid", new SFBool(true)));
}

Node *
ProtoNurbsSweptSurface::create(Scene *scene)
{
    return new NodeNurbsSweptSurface(scene, this); 
}

NodeNurbsSweptSurface::NodeNurbsSweptSurface(Scene *scene, Proto *proto)
  : MeshMorphingNode(scene, proto)
{
    m_extrusionDirty = true;
    m_extrusion = (NodeExtrusion *) scene->createNode("Extrusion");
    m_crossSectionCurve = NULL;
}

NodeNurbsSweptSurface::~NodeNurbsSweptSurface()
{
    delete m_extrusion;
    delete m_crossSectionCurve;
}

void
NodeNurbsSweptSurface::createExtrusion()
{
    NodeNurbsCurve *spineCurve = (NodeNurbsCurve *) 
                                 trajectoryCurve()->getValue();
    if (spineCurve == NULL)
        return;

    NodeNurbsCurve2D *crossSectionCurve2D = (NodeNurbsCurve2D *)
                                            crossSectionCurve()->getValue();
    if (crossSectionCurve2D == NULL)
        return;
    
    MFVec2d *crossSectionPoints = crossSectionCurve2D->controlPoint();
    if (crossSectionPoints == NULL)
        return;
    if (crossSectionPoints->getSize() == 0)
        return;

    if (m_extrusion == NULL)
        m_extrusion = (NodeExtrusion *) m_scene->createNode("Extrusion");

    if (crossSectionCurve2D)
        if (crossSectionCurve2D->getType() == VRML_NURBS_CURVE_2D) {
            crossSectionCurve2D->setHandleRendering(HANDLE_RENDERING_XZ);
            if (m_crossSectionCurve == NULL) {
                m_crossSectionCurve = (NodeNurbsCurve *) 
                                      m_scene->createNode("NurbsCurve");
                m_crossSectionCurve->setInternal(true);
            }
        }

    int len = crossSectionPoints->getSize() / 2;
    float *points = new float[len * 3];
    for (int i = 0; i < len; i++) {
         for (int j = 0; j < 2; j++)
             points[i * 3 + j] = crossSectionPoints->getValue(i)[j];
         points[i * 3 + 2] = 0.0f;
    }     
    if (m_crossSectionCurve != NULL) {
        m_crossSectionCurve->setControlPoints(new MFVec3f(points, len * 3));
        float *weights = new float[crossSectionCurve2D->weight()->getSize()];
        for (int i = 0; i < crossSectionCurve2D->weight()->getSize(); i++)
             weights[i] = crossSectionCurve2D->weight()->getValues()[i];
        m_crossSectionCurve->weight(new MFFloat(weights, crossSectionCurve2D->
                                                weight()->getSize()));
        float *knots = new float[crossSectionCurve2D->knot()->getSize()];
        for (int i = 0; i < crossSectionCurve2D->knot()->getSize(); i++)
            knots[i] = crossSectionCurve2D->knot()->getValues()[i];
        m_crossSectionCurve->knot(new MFFloat(knots, crossSectionCurve2D->
                                                  knot()->getSize()));
        m_crossSectionCurve->order(new SFInt32(crossSectionCurve2D->order()->
                                               getValue()));
        m_crossSectionCurve->tessellation(new SFInt32(crossSectionCurve2D->
                                                      tessellation()
                                                      ->getValue()));
        const Vec3f *chain = m_crossSectionCurve->getChain();
        int chainLength = m_crossSectionCurve->getChainLength();
        float *fchain = new float[chainLength * 2];
        for (int i = 0; i < chainLength; i++) {
            fchain[i * 2    ] = chain[i].x;
            fchain[i * 2 + 1] = chain[i].y;
        }   
        m_extrusion->crossSection(new MFVec2f(fchain, chainLength * 2));
    } else {
        m_extrusion->crossSection(new MFVec2f((MFVec2f*)
            crossSectionPoints->copy()));
    }
    
    m_extrusion->solid(new SFBool(solid()->getValue()));
    m_extrusion->ccw(new SFBool(ccw()->getValue()));
    m_extrusion->creaseAngle(new SFFloat(M_PI));
    m_extrusion->beginCap(new SFBool(false));
    m_extrusion->endCap(new SFBool(false));

    const Vec3f *chain = spineCurve->getChain();
    int chainLength = spineCurve->getChainLength();
    float *fchain = new float[chainLength * 3];
    for (int i = 0; i < chainLength; i++) {
         fchain[i * 3    ] = chain[i].x;
         fchain[i * 3 + 1] = chain[i].y;
         fchain[i * 3 + 2] = chain[i].z;
    }   
    m_extrusion->spine(new MFVec3f(fchain, chainLength * 3));
    m_extrusionDirty = false;
}

void
NodeNurbsSweptSurface::createMesh(bool cleanDoubleVertices, bool triangulate)
{
    if (m_extrusionDirty || (m_mesh == NULL)) {
        createExtrusion();
        m_extrusionDirty = false;
        if (!m_extrusion) return;
        m_extrusion->createMesh(cleanDoubleVertices, triangulate);
    }

    if (!m_extrusion) return;

    m_mesh = m_extrusion->getMesh();        
    m_meshDirty = false;
}

void
NodeNurbsSweptSurface::update()
{
    m_extrusionDirty = true;
    m_meshDirty = true;
}

void
NodeNurbsSweptSurface::setField(int index, FieldValue *value, int cf)
{
    m_extrusionDirty = true;
    Node::setField(index, value, cf);
    if (index == trajectoryCurve_Field())
        if (trajectoryCurve()->getValue() != NULL)
            trajectoryCurve()->getValue()->setInternal(true);
    if (index == crossSectionCurve_Field())
        if (crossSectionCurve()->getValue() != NULL)
            crossSectionCurve()->getValue()->setInternal(true);
    update();
}

void
NodeNurbsSweptSurface::draw()
{
    if (crossSectionCurve()->getValue() == NULL)
        return;
    if (trajectoryCurve()->getValue() == NULL)
        return;
    
    if (!crossSectionCurve()->getValue()->hasControlPoints())
        return;

    if (m_extrusionDirty) {
        createExtrusion();
        m_extrusionDirty = false;
    }

    if (!m_extrusion) return;

    m_extrusion->draw();
}

void 
NodeNurbsSweptSurface::findInterpolators(InterpolatorInfo& info)
{
    // there is a matching interpolator (to trajectoryCurve->controlPoint)

    NodeNurbsCurve *ntrajectoryCurve = (NodeNurbsCurve *) 
                                       trajectoryCurve()->getValue();
    if (ntrajectoryCurve) {
        Node *node = NULL;
        int field = -1;
        if (m_scene->isX3d()) {
            node = ntrajectoryCurve->controlPointX3D()->getValue();
            field = ((NodeCoordinate *)node)->point_Field();
        } else {
            node = ntrajectoryCurve;
            field = ntrajectoryCurve->controlPoint_Field();
        }

        Interpolator *inter = m_scene->findUpstreamInterpolator(node, field); 
        if (inter != NULL) {
            info.interpolator.append(inter);
            info.field.append(trajectoryCurve_Field());
        }
    }

    if (!crossSectionCurve())
        return;
    if (!crossSectionCurve()->getValue())
        return;
    if (crossSectionCurve()->getValue()->getType() != VRML_NURBS_CURVE_2D)
        return;
    NodeNurbsCurve2D *ncrossSectionCurve = (NodeNurbsCurve2D *) 
                                           crossSectionCurve()->getValue();

    if (ncrossSectionCurve) {
        Node *node = ncrossSectionCurve;
        int field = ncrossSectionCurve->controlPoint_Field();

        Interpolator *inter = m_scene->findUpstreamInterpolator(node, field); 
        if (inter != NULL) {
            info.interpolator.append(inter);
            info.field.append(crossSectionCurve_Field());
        }
    }
}

void
NodeNurbsSweptSurface::copyData(NurbsSweptSurfaceData *data)
{
    NodeNurbsCurve *ntrajectoryCurve = (NodeNurbsCurve *) 
                                       trajectoryCurve()->getValue();
    if (ntrajectoryCurve) {
        if (m_scene->isX3d()) {
            Node *node = ntrajectoryCurve->controlPointX3D()->getValue();
            MFVec3f *controlPoints = ((NodeCoordinate *)node)->point();
            data->controlPoint = (MFVec3f *)controlPoints->copy();
        } else {
            MFVec3f *controlPoints = ntrajectoryCurve->controlPoint();
            data->controlPoint = (MFVec3f *)controlPoints->copy();
        }
    }

    if (!crossSectionCurve())
        return;
    if (!crossSectionCurve()->getValue())
        return;
    if (crossSectionCurve()->getValue()->getType() != VRML_NURBS_CURVE_2D)
        return;
    NodeNurbsCurve2D *ncrossSectionCurve = (NodeNurbsCurve2D *) 
                                           crossSectionCurve()->getValue();
    if (ncrossSectionCurve) {
        MFVec2d *controlPoints = ncrossSectionCurve->controlPoint();
        data->controlPoint2D = (MFVec2d *)controlPoints->copy();
    }
}

void *
NodeNurbsSweptSurface::initializeData(void)
{
    NurbsSweptSurfaceData *data = new NurbsSweptSurfaceData();

    copyData(data);
    copyData(&m_tempStoreData);

    return data;
}

void
NodeNurbsSweptSurface::loadDataFromInterpolators(void *nurbsSweptSurfaceData, 
                                                 Interpolator *inter,
                                                 int field, float key)
{
    NurbsSweptSurfaceData *data = (NurbsSweptSurfaceData *)
                                   nurbsSweptSurfaceData;
    if (field == trajectoryCurve_Field())
        inter->interpolate(key, (float *)data->controlPoint->getValues());
    if (field == crossSectionCurve_Field())
        inter->interpolate(key, (float *)data->controlPoint2D->getValues());
}

void
NodeNurbsSweptSurface::createMeshFromData(void* nurbsSweptSurfaceData, 
                                          bool optimize)
{
    NurbsSweptSurfaceData *data = (NurbsSweptSurfaceData *)
                                  nurbsSweptSurfaceData;
    NodeNurbsCurve *ntrajectoryCurve = (NodeNurbsCurve *) 
                                       trajectoryCurve()->getValue();
    if (ntrajectoryCurve && 
        (ntrajectoryCurve->getType() == VRML_NURBS_CURVE)) {
        MFVec3f *newData = (MFVec3f *)data->controlPoint->copy();
        if (m_scene->isX3d()) {
            Node *node = ntrajectoryCurve->controlPointX3D()->getValue();
            ((NodeCoordinate *)node)->point(new MFVec3f(newData));
        } else {
            ntrajectoryCurve->controlPoint(new MFVec3f(newData));
        }
        m_extrusionDirty = true;
    }    

    NodeNurbsCurve2D *ncrossSection = (NodeNurbsCurve2D *) 
                                      crossSectionCurve()->getValue();
    if (ncrossSection && 
        (ncrossSection->getType() == VRML_NURBS_CURVE_2D)) {
        MFVec2d *newData = (MFVec2d *)data->controlPoint2D->copy();
        ncrossSection->controlPoint(new MFVec2d(newData));
        m_extrusionDirty = true;
    }    
    if (m_extrusionDirty)
        createMesh(optimize);
}

void
NodeNurbsSweptSurface::finalizeData(void* data)
{
    m_extrusionDirty = true;
    createMeshFromData(&m_tempStoreData, false);
    delete (NurbsSweptSurfaceData *)data;
}

Node *
NodeNurbsSweptSurface::convert2X3d(void)
{
    NodeNurbsCurve *ntrajectoryCurve = (NodeNurbsCurve *) 
                                       trajectoryCurve()->getValue();
    if (ntrajectoryCurve)
        ntrajectoryCurve->convert2X3d();
    return NULL;
}


Node *
NodeNurbsSweptSurface::convert2Vrml(void) 
{
    NodeNurbsCurve *ntrajectoryCurve = (NodeNurbsCurve *) 
                                       trajectoryCurve()->getValue();
    if (ntrajectoryCurve)
        ntrajectoryCurve->convert2Vrml();
    return NULL;
}

Vec3f   
NodeNurbsSweptSurface::getMinBoundingBox(void)
{
    if (m_extrusionDirty) {
        createExtrusion();
        m_extrusionDirty = false;
    }
 
    return m_extrusion->getMinBoundingBox();
}

Vec3f   
NodeNurbsSweptSurface::getMaxBoundingBox(void)
{
    if (m_extrusionDirty) {
        createExtrusion();
        m_extrusionDirty = false;
    }
 
    return m_extrusion->getMaxBoundingBox();
}

void
NodeNurbsSweptSurface::flip(int index)
{
    Node *ncrossSectionCurve = crossSectionCurve()->getValue();    
    if (ncrossSectionCurve)
        ncrossSectionCurve->flip(index);
    Node *ntrajectoryCurve = trajectoryCurve()->getValue();    
    if (ntrajectoryCurve)
        ntrajectoryCurve->flip(index);
    SFBool *bccw = new SFBool(!(ccw()->getValue()));
    ccw(bccw);
    m_meshDirty = true;    
}

void
NodeNurbsSweptSurface::swap(int fromTo)
{
    Node *ncrossSectionCurve = crossSectionCurve()->getValue();    
    if (ncrossSectionCurve)
        ncrossSectionCurve->swap(fromTo);
    Node *ntrajectoryCurve = trajectoryCurve()->getValue();    
    if (ntrajectoryCurve)
        ntrajectoryCurve->swap(fromTo);
    SFBool *bccw = new SFBool(!(ccw()->getValue()));
    ccw(bccw);
    m_meshDirty = true;    
}

Node *
NodeNurbsSweptSurface::toExtrusion(void)
{
    if (m_extrusionDirty) {
        createExtrusion();
        m_extrusionDirty = false;
    }

    if (!m_extrusion) 
        return NULL;

    return m_extrusion->copy();
}

int
NodeNurbsSweptSurface::writeRib(int filedes, int indent)
{ 
    if (m_extrusionDirty) {
        createExtrusion();
        m_extrusionDirty = false;
    }

    if (!m_extrusion) return 0;
  
    m_extrusion->writeRib(filedes, indent);

    return 0;
}

