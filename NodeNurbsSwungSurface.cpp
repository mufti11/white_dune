/*
 * NodeNurbsSwungSurface.cpp
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
#include "NodeNurbsSwungSurface.h"
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
#include "NodeContourPolyline2D.h"
#include "NodeNurbsCurve2D.h"
#include "NodeNurbsCurve.h"
#include "NodeCoordinate.h"
#include "NodeNormal.h"
#include "NodeExtrusion.h"
#include "NodeTextureCoordinate.h"
#include "NodeIndexedFaceSet.h"
#include "NodeNurbsGroup.h"
#include "NurbsSurfaceDegreeElevate.h"
#include "Util.h"
#include "Field.h"
#include "ExposedField.h"
#include "MoveCommand.h"

ProtoNurbsSwungSurface::ProtoNurbsSwungSurface(Scene *scene)
  : GeometryProto(scene, "NurbsSwungSurface")
{
    profileCurve.set(
          addExposedField(SFNODE, "profileCurve", new SFNode(NULL), 
                          NURBS_CONTROL_CURVE_NODE));
    trajectoryCurve.set(
          addExposedField(SFNODE, "trajectoryCurve", new SFNode(NULL), 
                          NURBS_CONTROL_CURVE_NODE));
    ccw.set(
          addField(SFBOOL, "ccw", new SFBool(true)));
    solid.set(
          addField(SFBOOL, "solid", new SFBool(true)));
}

Node *
ProtoNurbsSwungSurface::create(Scene *scene)
{
    return new NodeNurbsSwungSurface(scene, this); 
}

NodeNurbsSwungSurface::NodeNurbsSwungSurface(Scene *scene, Proto *proto)
  : MeshMorphingNode(scene, proto)
{
    m_extrusionDirty = true;
    m_extrusion = (NodeExtrusion *) scene->createNode("Extrusion");
    m_profileCurve = NULL;
    m_spineCurve = NULL;
}

NodeNurbsSwungSurface::~NodeNurbsSwungSurface()
{
    delete m_extrusion;
    delete m_profileCurve;
    delete m_spineCurve;
}

void
NodeNurbsSwungSurface::flip(int index)
{
    Node *nprofileCurve = profileCurve()->getValue();    
    if (nprofileCurve)
        nprofileCurve->flip(index);
    Node *ntrajectoryCurve = trajectoryCurve()->getValue();    
    if (ntrajectoryCurve)
        ntrajectoryCurve->flip(index);
    SFBool *bccw = new SFBool(!(ccw()->getValue()));
    ccw(bccw);
    m_meshDirty = true;    
}

void
NodeNurbsSwungSurface::swap(int fromTo)
{
    Node *nprofileCurve  = profileCurve ()->getValue();    
    if (nprofileCurve)
        nprofileCurve->swap(fromTo);
    Node *ntrajectoryCurve = trajectoryCurve()->getValue();    
    if (ntrajectoryCurve)
        ntrajectoryCurve->swap(fromTo);
    SFBool *bccw = new SFBool(!(ccw()->getValue()));
    ccw(bccw);
    m_meshDirty = true;    
}


void
NodeNurbsSwungSurface::createExtrusion()
{
    NodeNurbsCurve2D *spineCurve2D = (NodeNurbsCurve2D *) 
                                     trajectoryCurve()->getValue();
    if (spineCurve2D == NULL)
        return;

    MFVec2d *spinePoints = spineCurve2D->controlPoint();
    if (spinePoints == NULL)
        return;
    if (spinePoints->getSize() == 0)
        return;

    if (spineCurve2D)
        if (spineCurve2D->getType() == VRML_NURBS_CURVE_2D) {
            if (m_spineCurve == NULL) {
                m_spineCurve = (NodeNurbsCurve *) 
                               m_scene->createNode("NurbsCurve");
                m_spineCurve->setInternal(true);
            }
            spineCurve2D->setInternal(true);
            spineCurve2D->setHandleRendering(HANDLE_RENDERING_XZ);
        }

    NodeNurbsCurve2D *profileCurve2D = (NodeNurbsCurve2D *)
                                       profileCurve()->getValue();
    if (profileCurve2D == NULL)
        return;
    
    MFVec2d *profilePoints = profileCurve2D->controlPoint();
    if (profilePoints == NULL)
        return;
    if (profilePoints->getSize() == 0)
        return;

    if (m_extrusion == NULL)
        m_extrusion = (NodeExtrusion *) m_scene->createNode("Extrusion");

    if (profileCurve2D)
        if (profileCurve2D->getType() == VRML_NURBS_CURVE_2D) {
            if (m_profileCurve == NULL) {
                m_profileCurve = (NodeNurbsCurve *) 
                                      m_scene->createNode("NurbsCurve");
                m_profileCurve->setInternal(true);
            }
            profileCurve2D->setInternal(true);
            profileCurve2D->setHandleRendering(HANDLE_RENDERING_ZY);
        }

    int len = spinePoints->getSize() / 2;
    float *points = new float[len * 3];
    for (int i = 0; i < len; i++) {
         for (int j = 0; j < 2; j++)
             points[i * 3 + j] = spinePoints->getValue(i)[j];
         points[i * 3 + 2] = 0.0f;
    }     
    if (m_spineCurve != NULL) {
        m_spineCurve->setControlPoints(new MFVec3f(points, len * 3));
        float *weights = new float[spineCurve2D->weight()->getSize()];
        for (int i = 0; i < spineCurve2D->weight()->getSize(); i++)
             weights[i] = spineCurve2D->weight()->getValues()[i];
        m_spineCurve->weight(new MFFloat(weights, spineCurve2D->
                                                weight()->getSize()));
        float *knots = new float[spineCurve2D->knot()->getSize()];
        for (int i = 0; i < spineCurve2D->knot()->getSize(); i++)
            knots[i] = spineCurve2D->knot()->getValues()[i];
        m_spineCurve->knot(new MFFloat(knots, spineCurve2D->
                                                  knot()->getSize()));
        m_spineCurve->order(new SFInt32(spineCurve2D->order()->
                                               getValue()));
        m_spineCurve->tessellation(new SFInt32(spineCurve2D->
                                                      tessellation()
                                                      ->getValue()));
        const Vec3f *chain = m_spineCurve->getChain();
        int chainLength = m_spineCurve->getChainLength();
        float *fchain = new float[chainLength * 3];
        for (int i = 0; i < chainLength; i++) {
            fchain[i * 3    ] = chain[i].x;
            fchain[i * 3 + 1] = 0;
            fchain[i * 3 + 2] = chain[i].y;
        }   
        m_extrusion->spine(new MFVec3f(fchain, chainLength * 3));
    } else {
        const MFVec2d *chain = spinePoints;
        int chainLength = spinePoints->getSFSize();
        float *fchain = new float[chainLength * 3];
        for (int i = 0; i < chainLength; i++) {
            fchain[i * 3    ] = chain->getValue(i)[0];
            fchain[i * 3 + 1] = 0;
            fchain[i * 3 + 2] = chain->getValue(i)[1];
        }   
        m_extrusion->spine(new MFVec3f(fchain, chainLength * 3));
    }
    
    m_extrusion->solid(new SFBool(solid()->getValue()));
    m_extrusion->ccw(new SFBool(ccw()->getValue()));
    m_extrusion->creaseAngle(new SFFloat(M_PI));
    m_extrusion->beginCap(new SFBool(false));
    m_extrusion->endCap(new SFBool(false));

    int len2 = profilePoints->getSize() / 2;
    float *points2 = new float[len2 * 3];
    for (int i = 0; i < len2; i++) {
         for (int j = 0; j < 2; j++)
             points2[i * 3 + j] = profilePoints->getValue(i)[j];
         points2[i * 3 + 2] = 0.0f;
    }     
    if ((m_profileCurve != NULL) &&
        profileCurve()->getValue() &&
        (profileCurve()->getValue()->getType() == VRML_NURBS_CURVE_2D)) {
        m_profileCurve->setControlPoints(new MFVec3f(points2, len2 * 3));
        float *weights = new float[profileCurve2D->weight()->getSize()];
        for (int i = 0; i < profileCurve2D->weight()->getSize(); i++)
             weights[i] = profileCurve2D->weight()->getValues()[i];
        m_profileCurve->weight(new MFFloat(weights, profileCurve2D->
                                                weight()->getSize()));
        float *knots = new float[profileCurve2D->knot()->getSize()];
        for (int i = 0; i < profileCurve2D->knot()->getSize(); i++)
            knots[i] = profileCurve2D->knot()->getValues()[i];
        m_profileCurve->knot(new MFFloat(knots, profileCurve2D->
                                                  knot()->getSize()));
        m_profileCurve->order(new SFInt32(profileCurve2D->order()->
                                               getValue()));
        m_profileCurve->tessellation(new SFInt32(profileCurve2D->
                                                      tessellation()
                                                      ->getValue()));
        const Vec3f *chain = m_profileCurve->getChain();
        int chainLength = m_profileCurve->getChainLength();
        float *fchain = new float[chainLength * 2];
        for (int i = 0; i < chainLength; i++) {
            fchain[i * 2    ] = chain[i].x;
            fchain[i * 2 + 1] = chain[i].y;
        }   
        m_extrusion->crossSection(new MFVec2f(fchain, chainLength * 2));
    } else {
        m_extrusion->crossSection(new MFVec2f((MFVec2f*)profilePoints->copy()));
    }
    m_extrusionDirty = false;
}


void
NodeNurbsSwungSurface::createMesh(bool cleanDoubleVertices, bool triangulate)
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
NodeNurbsSwungSurface::finalizeData(void* data)
{
    m_extrusionDirty = true;
    createMeshFromData(&m_tempStoreData, false);
    delete (NurbsSwungSurfaceData *)data;
}

void 
NodeNurbsSwungSurface::findInterpolators(InterpolatorInfo& info)
{
    if (profileCurve() && profileCurve()->getValue()) {
        int field = -1;
        if (profileCurve()->getValue()->getType() == VRML_NURBS_CURVE_2D) {
            NodeNurbsCurve2D *nprofileCurve = (NodeNurbsCurve2D *) 
                                              profileCurve()->getValue();
            field = nprofileCurve->controlPoint_Field();
        } else if (profileCurve()->getValue()->getType() == 
                   X3D_CONTOUR_POLYLINE_2D) {
            NodeContourPolyline2D *nprofileCurve = (NodeContourPolyline2D *)
                                                   profileCurve()->getValue();
            field = nprofileCurve->lineSegments_Field();
        }
        if (field > -1) {
            Node *node = profileCurve()->getValue();
            
            Interpolator *inter = m_scene->findUpstreamInterpolator(node, 
                                                                    field); 
            if (inter != NULL) {
                info.interpolator.append(inter);
                info.field.append(profileCurve_Field());
            }
        }
    }
    if (trajectoryCurve() && trajectoryCurve()->getValue()) {
        int field = -1;
        if (trajectoryCurve()->getValue()->getType() == VRML_NURBS_CURVE_2D) {
            NodeNurbsCurve2D *ntrajectoryCurve = (NodeNurbsCurve2D *) 
                                              trajectoryCurve()->getValue();
            field = ntrajectoryCurve->controlPoint_Field();
        } else if (trajectoryCurve()->getValue()->getType() == 
                   X3D_CONTOUR_POLYLINE_2D) {
            NodeContourPolyline2D *ntrajectoryCurve = (NodeContourPolyline2D *)
                                                   trajectoryCurve()->getValue();
            field = ntrajectoryCurve->lineSegments_Field();
        }
        if (field > -1) {
            Node *node = trajectoryCurve()->getValue();
            
            Interpolator *inter = m_scene->findUpstreamInterpolator(node, 
                                                                    field); 
            if (inter != NULL) {
                info.interpolator.append(inter);
                info.field.append(trajectoryCurve_Field());
            }
        }
    }
}

void
NodeNurbsSwungSurface::copyData(NurbsSwungSurfaceData *data)
{
    if (profileCurve() && profileCurve()->getValue()) {
        if (profileCurve()->getValue()->getType() == VRML_NURBS_CURVE_2D) {
            NodeNurbsCurve2D *nprofileCurve = (NodeNurbsCurve2D *) 
                                              profileCurve()->getValue();

            if (nprofileCurve) {
                MFVec2d *controlPoints = nprofileCurve->controlPoint();
                data->controlPoint1 = (MFVec2d *)controlPoints->copy();
            }
        } else if (profileCurve()->getValue()->getType() == X3D_CONTOUR_POLYLINE_2D) {
            NodeContourPolyline2D *nprofileCurve = (NodeContourPolyline2D *) 
                                                   profileCurve()->getValue();

            if (nprofileCurve) {
                MFVec2d *controlPoints = nprofileCurve->lineSegments();
                data->controlPoint1 = (MFVec2d *)controlPoints->copy();
            }
        }
    }
    if (trajectoryCurve() && trajectoryCurve()->getValue()) {
        if (trajectoryCurve()->getValue()->getType() == VRML_NURBS_CURVE_2D) {
            NodeNurbsCurve2D *ntrajectoryCurve = (NodeNurbsCurve2D *) 
                                                 trajectoryCurve()->getValue();

            if (ntrajectoryCurve) {
                MFVec2d *controlPoints = ntrajectoryCurve->controlPoint();
                data->controlPoint2 = (MFVec2d *)controlPoints->copy();
            }
        } else if (trajectoryCurve()->getValue()->getType() == X3D_CONTOUR_POLYLINE_2D) {
            NodeContourPolyline2D *ntrajectoryCurve = (NodeContourPolyline2D *) 
                trajectoryCurve()->getValue();

            if (ntrajectoryCurve) {
                MFVec2d *controlPoints = ntrajectoryCurve->lineSegments();
                data->controlPoint2 = (MFVec2d *)controlPoints->copy();
            }
        }
    }
}

void *
NodeNurbsSwungSurface::initializeData(void)
{
    NurbsSwungSurfaceData *data = new NurbsSwungSurfaceData();

    copyData(data);
    copyData(&m_tempStoreData);

    return data;
}

void
NodeNurbsSwungSurface::createMeshFromData(void* nurbsSwungSurfaceData, 
                                          bool optimize)
{
    NurbsSwungSurfaceData *data = (NurbsSwungSurfaceData *)
                                  nurbsSwungSurfaceData;
    if (trajectoryCurve() && trajectoryCurve()->getValue()) {
        if (trajectoryCurve()->getValue()->getType() == VRML_NURBS_CURVE_2D) {
            NodeNurbsCurve2D *ntrajectoryCurve = (NodeNurbsCurve2D *) 
                                                 trajectoryCurve()->getValue();
            MFVec2d *newData = (MFVec2d *)data->controlPoint2->copy();
            ntrajectoryCurve->controlPoint(new MFVec2d(newData));
            m_extrusionDirty = true;
        } else if (trajectoryCurve()->getValue()->getType() == 
                   X3D_CONTOUR_POLYLINE_2D) {
            NodeContourPolyline2D *ntrajectoryCurve = (NodeContourPolyline2D *) 
                trajectoryCurve()->getValue();
            MFVec2d *newData = (MFVec2d *)data->controlPoint2->copy();
            ntrajectoryCurve->lineSegments(new MFVec2d(newData));
            m_extrusionDirty = true;
        }
    }          

    if (profileCurve() && profileCurve()->getValue()) {
        if (profileCurve()->getValue()->getType() == VRML_NURBS_CURVE_2D) {
            NodeNurbsCurve2D *nprofileCurve = (NodeNurbsCurve2D *) 
                                              profileCurve()->getValue();
            MFVec2d *newData = (MFVec2d *)data->controlPoint1->copy();
            nprofileCurve->controlPoint(new MFVec2d(newData));
            m_extrusionDirty = true;
        } else if (profileCurve()->getValue()->getType() == 
                   X3D_CONTOUR_POLYLINE_2D) {
            NodeContourPolyline2D *nprofileCurve = (NodeContourPolyline2D *) 
                                                   profileCurve()->getValue();
            MFVec2d *newData = (MFVec2d *)data->controlPoint1->copy();
            nprofileCurve->lineSegments(new MFVec2d(newData));
            m_extrusionDirty = true;
        }
    }          

    if (m_extrusionDirty)
        createMesh(optimize);
}

void
NodeNurbsSwungSurface::loadDataFromInterpolators(void *nurbsSwungSurfaceData, 
                                                 Interpolator *inter,
                                                 int field, float key)
{
    NurbsSwungSurfaceData *data = (NurbsSwungSurfaceData *)
                                   nurbsSwungSurfaceData;
    if (field == trajectoryCurve_Field() && data->controlPoint2)
        inter->interpolate(key, (float *)data->controlPoint2->getValues());
    if (field == profileCurve_Field() && data->controlPoint1)
        inter->interpolate(key, (float *)data->controlPoint1->getValues());
}

void
NodeNurbsSwungSurface::draw()
{
    if (trajectoryCurve()->getValue() == NULL)
        return;
    if (profileCurve()->getValue() == NULL)
        return;
    if (m_extrusionDirty) {
        createExtrusion();
        m_extrusionDirty = false;
    }

    if (!trajectoryCurve()->getValue()->hasControlPoints())
        return;
    if (!profileCurve()->getValue()->hasControlPoints())
        return;

    if (!m_extrusion) return;

    m_extrusion->draw();
}


Node *
NodeNurbsSwungSurface::toExtrusion(void)
{
    if (m_extrusionDirty) {
        createExtrusion();
        m_extrusionDirty = false;
    }

    if (!m_extrusion) 
        return NULL;

    return m_extrusion->copy();
}

void
NodeNurbsSwungSurface::update()
{
    m_extrusionDirty = true;
    m_meshDirty = true;
}

int
NodeNurbsSwungSurface::writeRib(int filedes, int indent)
{ 
    if (m_extrusionDirty) {
        createExtrusion();
        m_extrusionDirty = false;
    }

    if (!m_extrusion) return 0;
  
    m_extrusion->writeRib(filedes, indent);

    return 0;
}

