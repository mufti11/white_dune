/*
 * NodeNurbsSurface.cpp
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

#include "stdafx.h"
#include "NodeNurbsSurface.h"
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
#include "NodeNurbsGroup.h"
#include "NodeNurbsSet.h"
#include "NurbsSurfaceDegreeElevate.h"
#include "Util.h"
#include "Field.h"
#include "ExposedField.h"
#include "MoveCommand.h"
#include "NodeNurbsTrimmedSurface.h"
#include "NodeCoordinateDouble.h"
#include "resource.h"

ProtoNurbsSurface::ProtoNurbsSurface(Scene *scene)
  : Proto(scene, "NurbsSurface")
{
    addElements(); 
}

ProtoNurbsSurface::ProtoNurbsSurface(Scene *scene, const char *name)
  : Proto(scene, name)
{
    addElements(); 
}

void ProtoNurbsSurface::addElements(void) 
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
    solid.set(
          addField(SFBOOL, "solid", new SFBool(true)));
    setFieldFlags(solid, FF_4KIDS);
    ccw.set(
          addField(SFBOOL, "ccw", new SFBool(true)));
    setFieldFlags(ccw, FF_VRML_ONLY | FF_4KIDS);
    controlPoint.set(
          addExposedField(MFVEC3F, "controlPoint", new MFVec3f()));
    setFieldFlags(controlPoint, FF_VRML_ONLY | EIF_RECOMMENDED);
    controlPointX3D.set(
          addExposedField(SFNODE, "controlPoint", new SFNode(NULL), 
                          COORDINATE_NODE));
    setFieldFlags(controlPointX3D, FF_X3D_ONLY);
    weight.set(
          addExposedField(MFFLOAT, "weight", new MFFloat(), new SFFloat(0.0f)));
    uTessellation.set(
          addExposedField(SFINT32, "uTessellation", new SFInt32(0)));
    setFieldFlags(uTessellation, FF_4KIDS);
    vTessellation.set(
          addExposedField(SFINT32, "vTessellation", new SFInt32(0)));
    setFieldFlags(vTessellation, FF_4KIDS);
    texCoord.set(
          addExposedField(SFNODE, "texCoord", new SFNode(NULL), 
                          NURBS_TEXTURE_COORDINATE_OR_TEXTURE_COORDINATE_NODE));
    uClosed.set(
          addField(SFBOOL, "uClosed", new SFBool(false)));
    setFieldFlags(uClosed, FF_X3D_ONLY | FF_4KIDS);
    vClosed.set(
          addField(SFBOOL, "vClosed", new SFBool(false)));
    setFieldFlags(vClosed, FF_X3D_ONLY | FF_4KIDS);

    m_x3dName = "";
    m_x3dName += "NurbsPatchSurface";

    addURLs(URL_VRML97_AMENDMENT1);
}

Node *
ProtoNurbsSurface::create(Scene *scene)
{
    return new NodeNurbsSurface(scene, this); 
}

NodeNurbsSurface::NodeNurbsSurface(Scene *scene, Proto *proto)
  : MeshMorphingNode(scene, proto)
{
    m_numExtraVertices = 0;
    m_createExtraTess = true;
    setField(controlPointX3D_Field(), controlPointX3D());
}

NodeNurbsSurface::~NodeNurbsSurface()
{
}

int 
NodeNurbsSurface::repairField(int field) const
{
    bool x3d = m_scene->isX3d();
    if (x3d && (field == controlPoint_Field()))
        return controlPointX3D_Field();
    else if (!x3d && (field == controlPointX3D_Field()))
        return controlPoint_Field();
    return field;
}

// store controlPoint information (MFVec3f *) both in 
// controlPoint (VRML97 Amendment 1) and controlPoint.point (X3D)
void
NodeNurbsSurface::setField(int index, FieldValue *value, int cf)
{
    if (index == controlPoint_Field()) {
        if (controlPointX3D()->getType() == X3D_COORDINATE_DOUBLE) {
            NodeCoordinateDouble *coord = (NodeCoordinateDouble *)
                                          controlPointX3D()->getValue();
            if (coord == NULL) {
                createControlPoints(((MFVec3d *)value)->getMFVec3f());
                return;
            }
            m_scene->setField(coord, coord->point_Field(), value);
        } else if (controlPointX3D()->getType() == VRML_COORDINATE) {
            NodeCoordinate *coord = (NodeCoordinate *)
                                    controlPointX3D()->getValue();
            if (coord == NULL) {
                createControlPoints((MFVec3f *)value);
                return;
            }
            m_scene->setField(coord, coord->point_Field(), value);
        }
    } 
    if (index == controlPointX3D_Field()) {
        if ((value != NULL) && (value->getType() == SFNODE)) {
            Node *node = ((SFNode *)value)->getValue();
            if ((node != NULL) && (node->getType() == VRML_COORDINATE)) {
                NodeCoordinate *coord = (NodeCoordinate *) node;
                Node::setField(controlPoint_Field(), coord->point());
            }
            if ((node != NULL) && (node->getType() == X3D_COORDINATE_DOUBLE)) {
                NodeCoordinateDouble *coord = (NodeCoordinateDouble *) node;
                Node::setField(controlPoint_Field(),
                               coord->point()->getMFVec3f());
            }
        }
    } 
    Node::setField(index, value, cf);
    m_meshDirty = true;
}

MFVec3f *
NodeNurbsSurface::getControlPoints(void)
{
    return controlPoint();
}

void
NodeNurbsSurface::setControlPoints(MFVec3f *points)
{
    Node *control = controlPointX3D()->getValue();
    if (control && control->getType() == X3D_COORDINATE_DOUBLE) {
        NodeCoordinateDouble *coord = (NodeCoordinateDouble *)
                                      controlPointX3D()->getValue();
        if (coord == NULL) {
            createControlPoints(points);
            return;
        }
        MFVec3d *pointsDouble = points->getMFVec3d();
        Node::setField(controlPoint_Field(), points);
        ((Node *)coord)->Node::setField(coord->point_Field(), pointsDouble);
    } else if (control && control->getType() == VRML_COORDINATE) {
        NodeCoordinate *coord = (NodeCoordinate *)controlPointX3D()->getValue();
        if (coord == NULL) {
            createControlPoints(points);
            return;
        }
        Node::setField(controlPoint_Field(), points);
        ((Node *)coord)->Node::setField(coord->point_Field(), points);
    } else {
        NodeCoordinate *coord = (NodeCoordinate *)controlPointX3D()->getValue();
        if (coord == NULL) {
            createControlPoints(points);
            return;
        }
        Node::setField(controlPoint_Field(), points);
    }
    m_meshDirty = true;
}

void
NodeNurbsSurface::createControlPoints(MFVec3f *points)
{
    controlPointX3D(new SFNode(m_scene->createNode("Coordinate", 
                                                   NODE_FLAG_X3D_ONLY)));
    setControlPoints(points);
}

void    
NodeNurbsSurface::update() 
{ 
    m_meshDirty = true;
}

void
NodeNurbsSurface::flipSide(void)
{
    MFVec3f *newControlPoints = new MFVec3f();
    MFFloat *newWeights = new MFFloat();
    int iuDimension = uDimension()->getValue();
    int ivDimension = vDimension()->getValue();
        
    for (int j = 0; j < ivDimension; j ++)
        for (int i = iuDimension - 1; i > -1; i--) {
            const float *v = getControlPoints()->getValue(i + j * iuDimension);
            newControlPoints->appendSFValue(v[0], v[1], v[2]);
            newWeights->appendSFValue(weight()->getValue(i + j * iuDimension));
        }
    setControlPoints(newControlPoints);
    weight(newWeights);
    m_meshDirty = true;
}

void
NodeNurbsSurface::repairNormal(MFVec3f *normals, MFVec3f *tess, 
                               int c1, int c2, int c3)
{
    Vec3f tessC1 = tess->getVec(c1); 
    Vec3f tessC2 = tess->getVec(c2); 
    Vec3f tessC3 = tess->getVec(c3); 
    Vec3f v1 = tessC3 - tessC1;
    Vec3f v2 = tessC2 - tessC1;
    Vec3f normal = ccw()->getValue() ? v2.cross(v1) : v1.cross(v2);
    normal.normalize();
    normals->setVec(c1, normal);   
}

Node *
NodeNurbsSurface::convert2X3d(void)
{
    if (!ccw()->getValue()) {
        flipSide();
        ccw(new SFBool(true));
    }    
    NodeCoordinate *coord = (NodeCoordinate *)controlPointX3D()->getValue();
    if (coord != NULL) {
        coord->getVariableName();
        m_scene->changeRoutes(coord, coord->point_Field(), 
                              this, controlPoint_Field());
    } else {
        MFVec3f *points = (MFVec3f *)getUntranslatedField(
           ((ProtoNurbsSurface *)getPrimaryProto())->controlPoint);
        if (points->getSize() > 0) {
            points->ref();
            createControlPoints(points);
            coord = (NodeCoordinate *)controlPointX3D()->getValue();
            m_scene->changeRoutes(coord, coord->point_Field(), 
                                  this, controlPoint_Field());
        }
    }
    return NULL;
}

Node *
NodeNurbsSurface::convert2Vrml(void) 
{
    if (controlPointX3D()->getType() == X3D_COORDINATE_DOUBLE) {
        NodeCoordinateDouble *coord = (NodeCoordinateDouble *)
                                      controlPointX3D()->getValue();
        if (coord != NULL) {
            m_scene->changeRoutes(this, controlPoint_Field(),
                                  coord, coord->point_Field());
        }
    } else {
        NodeCoordinate *coord = (NodeCoordinate *)controlPointX3D()->getValue();
        if (coord != NULL) {
            m_scene->changeRoutes(this, controlPoint_Field(),
                                  coord, coord->point_Field());
        }
    }
    return NULL;
}


void
NodeNurbsSurface::createMesh(const Vec3f *controlPoints, bool cleanVertices,
                             bool triangulate)
{
    int iuDimension = uDimension()->getValue();
    int ivDimension = vDimension()->getValue();
    int iuTess = uTessellation()->getValue();
    int ivTess = vTessellation()->getValue();
    float *weights = NULL;

    if (iuDimension == 0 || ivDimension == 0) return;

    if (uKnot()->getSize() != uOrder()->getValue() + iuDimension
     || vKnot()->getSize() != vOrder()->getValue() + ivDimension)
        return;

    if (weight()->getSize() == 0) {
        weights = new float[iuDimension * ivDimension];
        for (int i = 0; i < iuDimension * ivDimension; i++) {
            weights[i] = 1.0f;
        }
    } else if (weight()->getSize() != iuDimension * ivDimension) {
        return;
    }

    bool hasTexCoords = false;
    int size = iuTess * ivTess;
    MFVec2f *mfTexCoord = new MFVec2f(size);
    if (texCoord()->getValue()) 
        if (texCoord()->getValue()->getType() == VRML_TEXTURE_COORDINATE) {
            NodeTextureCoordinate *ntexCoord = (NodeTextureCoordinate *)
                                               texCoord()->getValue();
            if (ntexCoord->point()) {
                for (int i = 0; i < ntexCoord->point()->getSFSize(); i++)
                    mfTexCoord->setSFValue(i, ntexCoord->point()->getSFValue(i));
                hasTexCoords = true;
            }
    }

    if (iuTess <= 0) iuTess = TheApp->getTessellation();
    if (ivTess <= 0) ivTess = TheApp->getTessellation();
    bool midQuadUTess = false;
    if ((iuTess % 2) == 1) 
        midQuadUTess = (iuTess > 1);
    MFVec3f *vertices = new MFVec3f(size);
    MFVec3f *normal = new MFVec3f(size);

    const float *uKnots = uKnot()->getValues();
    const float *vKnots = vKnot()->getValues();
    float uInc = (uKnots[uKnot()->getSize()-1] - uKnots[0]) / iuTess;
    float vInc = (vKnots[vKnot()->getSize()-1] - vKnots[0]) / ivTess;

    int index = 0;
    const float *w = weights ? weights : weight()->getValues();
    int i, j;
    float u, v;
    for (j = 0, v = vKnots[0]; j <= ivTess; j++, v += vInc) {
        for (i = 0, u = uKnots[0]; i <= iuTess; i++, u += uInc) {
            Vec3f norm;
            Vec3f tess = surfacePoint(iuDimension, uOrder()->getValue(), uKnots,
                                      ivDimension, vOrder()->getValue(), vKnots,
                                      controlPoints, w, u, v, norm);
            vertices->setVec(index, tess);
            if (!ccw()->getValue()) norm = -norm;
            normal->setVec(index, norm);
            index++;
        }
    }

    if (!hasTexCoords) {
        index = 0;
        Vec2f tc; 
        float uinv = 1.0f / iuTess;
        float vinv = 1.0f / ivTess;
        for (int j = 0; j <= ivTess; j++) {
            for (i = 0; i <= iuTess; i++) {
                tc.x = i * uinv;
                tc.y = j * vinv;
                mfTexCoord->setVec(index++, tc);
            }
        }
    }
    MFInt32 *coordIndex = new MFInt32();
    index = 0;
    int newIndex = vertices->getSFSize();
    m_numExtraVertices = 0;
    for (int j = 0; j < ivTess; j++) {
        for (int i = 0; i < iuTess; i++) {
            int c1 = j * (iuTess+1) + i;
            int c2 = j * (iuTess+1) + (i+1);
            int c3 = (j+1) * (iuTess+1) + (i+1);
            int c4 = (j+1) * (iuTess+1) + i;

            // repair normals at begin of v
            if (j == 0) {
                repairNormal(normal, vertices, c1, c3, c4);
                repairNormal(normal, vertices, c2, c3, c4);
            }
            // repair normals at end of v
            if (j == (ivTess - 1)) {
                repairNormal(normal, vertices, c3, c1, c2);
                repairNormal(normal, vertices, c4, c1, c2);
            }

            Vec3f tessC1 = vertices->getVec(c1);
            Vec3f tessC2 = vertices->getVec(c2);
            Vec3f tessC3 = vertices->getVec(c3);
            Vec3f tessC4 = vertices->getVec(c4);
            if (midQuadUTess && m_createExtraTess) {
                if (!triangulate) {
                    coordIndex->appendSFValue(c1);
                    coordIndex->appendSFValue(c2);
                    coordIndex->appendSFValue(c3);
                    coordIndex->appendSFValue(c4);
                    coordIndex->appendSFValue(-1);                
                } else if (cleanVertices && 
                         !tessC1.validTriangle(tessC2, tessC3)) {
                    coordIndex->appendSFValue(c2);
                    coordIndex->appendSFValue(c3);
                    coordIndex->appendSFValue(c4);
                    coordIndex->appendSFValue(-1);                
                } else if (cleanVertices && 
                           !tessC3.validTriangle(tessC4, tessC1)) {
                    coordIndex->appendSFValue(c1);
                    coordIndex->appendSFValue(c2);
                    coordIndex->appendSFValue(c4);
                    coordIndex->appendSFValue(-1);
                } else if (m_createExtraTess) {
                    Vec3f midVertex = (tessC1 + tessC2 + tessC3 + tessC4) / 4; 
                    vertices->setVec(vertices->getSFSize(), midVertex);
                    m_numExtraVertices++;
                    Vec3f normalC1 = normal->getVec(c1);  
                    Vec3f normalC2 = normal->getVec(c2);
                    Vec3f normalC3 = normal->getVec(c3);
                    Vec3f normalC4 = normal->getVec(c4);
                    Vec3f midNormal = (normalC1 + normalC2 + 
                                       normalC3 + normalC4) / 4; 
                    midNormal.normalize();
                    normal->setVec(normal->getSFSize(), midNormal);
                    Vec2f texCoordC1 = mfTexCoord->getVec(c1);  
                    Vec2f texCoordC2 = mfTexCoord->getVec(c2);
                    Vec2f texCoordC3 = mfTexCoord->getVec(c3);
                    Vec2f texCoordC4 = mfTexCoord->getVec(c4);
                    Vec2f midTexCoord = (texCoordC1 + texCoordC2 + 
                                         texCoordC3 + texCoordC4) / 4; 
                    mfTexCoord->setVec(mfTexCoord->getSFSize(), midTexCoord);
                    coordIndex->appendSFValue(c2);
                    coordIndex->appendSFValue(newIndex);
                    coordIndex->appendSFValue(c1);
                    coordIndex->appendSFValue(-1);
                    coordIndex->appendSFValue(c3);
                    coordIndex->appendSFValue(newIndex);
                    coordIndex->appendSFValue(c2);
                    coordIndex->appendSFValue(-1);
                    coordIndex->appendSFValue(c4);
                    coordIndex->appendSFValue(newIndex);
                    coordIndex->appendSFValue(c3);
                    coordIndex->appendSFValue(-1);
                    coordIndex->appendSFValue(c1);
                    coordIndex->appendSFValue(newIndex);
                    coordIndex->appendSFValue(c4);
                    coordIndex->appendSFValue(-1);
                    newIndex++;
                }
            } else {
                if (triangulate) {
                    if (i > iuTess / 2) {
                        if (!cleanVertices || 
                            tessC1.validTriangle(tessC2, tessC3)) {
                            coordIndex->appendSFValue(c1);
                            coordIndex->appendSFValue(c2);
                            coordIndex->appendSFValue(c3);
                            coordIndex->appendSFValue(-1);
                        }
                    } else {
                        if (!cleanVertices || 
                            tessC4.validTriangle(tessC1, tessC2)) {
                            coordIndex->appendSFValue(c4);
                            coordIndex->appendSFValue(c1);
                            coordIndex->appendSFValue(c2);
                            coordIndex->appendSFValue(-1);
                        }
                    }
                    if (i > iuTess / 2) {
                        if (!cleanVertices || 
                            tessC3.validTriangle(tessC4, tessC1)) {
                            coordIndex->appendSFValue(c3);
                            coordIndex->appendSFValue(c4);
                            coordIndex->appendSFValue(c1);
                            coordIndex->appendSFValue(-1);
                        }
                    } else {
                        if (!cleanVertices || 
                            tessC2.validTriangle(tessC3, tessC4)) {
                            coordIndex->appendSFValue(c2);
                            coordIndex->appendSFValue(c3);
                            coordIndex->appendSFValue(c4);
                            coordIndex->appendSFValue(-1);
                        }
                    }
                } else {
                    if (i < iuTess / 2) {
                        coordIndex->appendSFValue(c1);
                        coordIndex->appendSFValue(c2);
                        coordIndex->appendSFValue(c3);
                        coordIndex->appendSFValue(c4);
                        coordIndex->appendSFValue(-1);
                    } else {
                        coordIndex->appendSFValue(c2);
                        coordIndex->appendSFValue(c3);
                        coordIndex->appendSFValue(c4);
                        coordIndex->appendSFValue(c1);
                        coordIndex->appendSFValue(-1);
                    }
                }
            }
        }
    }

    int meshFlags = MESH_TRIANGULATED;
    if (ccw()->getValue())
        meshFlags |= MESH_CCW;
    if (solid()->getValue())
        meshFlags |= MESH_SOLID;
    meshFlags |= MESH_CONVEX;
    meshFlags |= MESH_COLOR_PER_VERTEX;
    meshFlags |= MESH_NORMAL_PER_VERTEX;
    MyArray<MFVec2f *>texCoords;
    texCoords.append(mfTexCoord);
    m_mesh = new MyMesh(this, vertices, coordIndex, normal, NULL,
                        NULL, NULL, texCoords, NULL, 1.57, meshFlags, 0);
    delete [] weights;
}

void
NodeNurbsSurface::createMesh(bool cleanVertices, bool triangulate)
{
    MFVec3f *controlPoints = NULL;
    if (m_scene->isX3d()) {
        Node *node = controlPointX3D()->getValue();
        if (node && node->getType() == X3D_COORDINATE_DOUBLE) {
            NodeCoordinateDouble *coord = (NodeCoordinateDouble *)
                                          controlPointX3D()->getValue();
            if (coord != NULL) {
                controlPoints = coord->point()->getMFVec3f();
            }
        } else if (node && node->getType() == VRML_COORDINATE) {
            NodeCoordinate *coord = (NodeCoordinate *)
                                    controlPointX3D()->getValue();
            if (coord != NULL) {
                controlPoints = coord->point();
            }
        } else
            controlPoints = controlPoint();
    } else
        controlPoints = controlPoint();
        
    if (controlPoints == NULL)
        return;
    if (controlPoints->getSize() != uDimension()->getValue() * 
                                    vDimension()->getValue() * 3)
        return;
    
    createMesh((const Vec3f *) controlPoints->getValues(), cleanVertices,
               triangulate);
}

void
NodeNurbsSurface::drawHandles()
{
    int iuDimension = uDimension()->getValue();
    int ivDimension = vDimension()->getValue();
    RenderState state;

    if (getControlPoints()->getSize() != iuDimension * ivDimension * 3)
        return;

    if (weight()->getSize() != iuDimension * ivDimension) {
        return;
    }

    glPushName(iuDimension * ivDimension + 1);
    glDisable(GL_LIGHTING);
    Util::myGlColor3f(1.0f, 1.0f, 1.0f);
    if (TheApp->GetHandleMeshAlways() || 
        ((m_scene->getSelectedHandlesSize() == 0) ||
         (m_scene->getfirstSelectionRangeHandle() > -1))) {
        for (int i = 0; i < iuDimension; i++) {
            glBegin(GL_LINES);
            for (int j = 0; j < ivDimension - 1; j++) {
                bool hidden = false;
                for (int n = 0; n < m_scene->getNumHiddenVertices(); n++) {
                     if (m_scene->getHiddenVertex(n) == i + j * iuDimension) {
                         hidden = true;
                         break;
                     }
                     if (m_scene->getHiddenVertex(n) == i + (j + 1) * 
                                                        iuDimension) {
                         hidden = true;
                         break;
                     }
                }
                if (hidden)
                    continue;
                const float *v1 = getControlPoints()->getValue(i + 
                                                               j * iuDimension);
                float w1 = weight()->getValue(i + j * iuDimension);
                glVertex3f(v1[0] / w1, v1[1] / w1, v1[2] / w1);
                const float *v2 = getControlPoints()->getValue(i + (j + 1) * 
                                                               iuDimension);
                float w2 = weight()->getValue(i + (j + 1) * iuDimension);
                glVertex3f(v2[0] / w2, v2[1] / w2, v2[2] / w2);
            }
            glEnd();
        }
        for (int j = 0; j < ivDimension; j++) {
            glBegin(GL_LINES);
            for (int i = 0; i < iuDimension - 1; i++) {
                bool hidden = false;
                for (int n = 0; n < m_scene->getNumHiddenVertices(); n++) {
                     if (m_scene->getHiddenVertex(n) == i + j * iuDimension) {
                         hidden = true;
                         break;
                     }
                     if (m_scene->getHiddenVertex(n) == i + 1 + j * 
                                                        iuDimension) {
                         hidden = true;
                         break;
                     }
                }
                if (hidden)
                    continue;
                const float *v1 = getControlPoints()->getValue(i + 
                                                               j * iuDimension);
                float w1 = weight()->getValue(i + j * iuDimension);
                glVertex3f(v1[0] / w1, v1[1] / w1, v1[2] / w1);
                const float *v2 = getControlPoints()->getValue(i + 1 +
                                                               j * iuDimension);
                float w2 = weight()->getValue(i + 1 + j * iuDimension);
                glVertex3f(v2[0] / w2, v2[1] / w2, v2[2] / w2);
            }
            glEnd();
        }
    }
    state.startDrawHandles();
    for (int ci = 0; ci < iuDimension * ivDimension; ci++) {
        bool hidden = false;
        for (int i = 0; i < m_scene->getNumHiddenVertices(); i++)
             if (m_scene->getHiddenVertex(i) == ci) {
                 hidden = true;
                 break;
             }
        if (hidden)
            continue;
        state.setHandleColor(m_scene, ci);
        glLoadName(ci);
        state.drawHandle(Vec3f(getControlPoints()->getValue(ci)) / 
                               weight()->getValue(ci));
    }
    state.endDrawHandles();
    glPopName();
    glEnable(GL_LIGHTING);
}

Vec3f
NodeNurbsSurface::getHandle(int handle, int *constraint,
                            int *field)
{
    *field = controlPoint_Field();

    if (handle >= 0 && handle < getControlPoints()->getSize() / 3) {
        Vec3f ret((Vec3f)getControlPoints()->getValue(handle)
                         / weight()->getValue(handle));
        TheApp->PrintMessageWindowsVertex(IDS_VERTEX_SELECTED, "controlPoint", 
                                          handle, ret);
        return ret;
    } else {
        return Vec3f(0.0f, 0.0f, 0.0f);
    }
}

void
NodeNurbsSurface::setHandle(MFVec3f *value, int handle, float newWeight,
                            const Vec3f &newV, const Vec3f &oldV,
                            bool already_changed, bool bypassChecks)
{
    bool changed = false;
    MFVec3f *newValue = (MFVec3f *) value->copy();

    if (m_scene->getXSymetricMode()) {
        float epsilon = TheApp->GetHandleEpsilon();
        int numPoints = newValue->getSize() / 3; 
        for (int i = 0; i < numPoints; i++) {
            if (i != handle) {
                Vec3f vPoint = getControlPoints()->getValue(i);
                float wPoint = weight()->getValue(i);
                float w = wPoint;
                if (wPoint != 0)
                   vPoint = vPoint / wPoint;
                if (newWeight != 0)
                   w = w / newWeight;
                if (   (fabs(vPoint.z - oldV.z) < epsilon) 
                    && (fabs(vPoint.y - oldV.y) < epsilon)) {
                    if (fabs(vPoint.x - oldV.x) < epsilon) {
                        changed = true;
                        if (fabs(oldV.x) < epsilon)
                            newValue->setValue(i * 3, 0);
                         else
                            newValue->setValue(i * 3, newV.x * w);
                         newValue->setValue(i * 3 + 1, newV.y * w);
                         newValue->setValue(i * 3 + 2, newV.z * w);
                    } else if (fabs(vPoint.x + oldV.x) < epsilon) {
                         changed = true;
                         if (fabs(oldV.x) < epsilon)
                             newValue->setValue(i * 3, 0);
                         else
                             newValue->setValue(i * 3, -newV.x * w);
                         newValue->setValue(i * 3 + 1, newV.y * w);
                         newValue->setValue(i * 3 + 2, newV.z * w);
                    }
                }                 
            }
        }
    }
    if (already_changed | bypassChecks)
        changed = true;
    
    if (changed) {
        m_meshDirty = true;

        if (controlPointX3D() && controlPointX3D()->getValue() &&
            controlPointX3D()->getValue()->getType() == X3D_COORDINATE_DOUBLE) {
            NodeCoordinateDouble *pointNode = (NodeCoordinateDouble *)
                                              controlPointX3D()->getValue();
            if (pointNode == NULL) {
                createControlPoints(newValue);
                return;
            }
            MFVec3d *doubleValue = newValue->getMFVec3d();
            m_scene->setField(pointNode, pointNode->point_Field(), 
                              doubleValue->copy());
            m_scene->setField(this, controlPoint_Field(), newValue);
        } else if (controlPointX3D() && controlPointX3D()->getValue()) {
            NodeCoordinate *pointNode = (NodeCoordinate *)
                                         controlPointX3D()->getValue();
            if (pointNode == NULL) {
                createControlPoints(newValue);
                return;
            }
            m_scene->setField(pointNode, pointNode->point_Field(),
                              newValue->copy());
            m_scene->setField(this, controlPoint_Field(), newValue);
        }
    }
}

void
NodeNurbsSurface::setHandle(float newWeight, 
                            const Vec3f &newV, const Vec3f &oldV,
                            bool bypassChecks)
{
    setHandle(getControlPoints(), -1, newWeight, newV, oldV, false, 
              bypassChecks);
}

NodeNurbsGroup *
NodeNurbsSurface::findNurbsGroup()
{
    if (hasParent()) {
        Node* parent = getParent();
        if (parent->getType() == VRML_NURBS_GROUP) {
            return (NodeNurbsGroup *) parent;
        } else if (parent->getType() == VRML_SHAPE)
            if (parent->hasParent())
                 if (parent->getParent()->getType() == VRML_NURBS_GROUP) {
                    return (NodeNurbsGroup *) parent->getParent();    
               } 
    }
    return NULL;
}

NodeNurbsSet *
NodeNurbsSurface::findNurbsSet()
{
    if (hasParent()) {
        Node* parent = getParent();
        if (parent->getType() == X3D_NURBS_SET) {
            return (NodeNurbsSet *) parent;
        } else if (parent->getType() == VRML_SHAPE)
            if (parent->hasParent())
                if (parent->getParent()->getType() == X3D_NURBS_SET) {
                    return (NodeNurbsSet *) parent->getParent();    
               } 
    }
    return NULL;
}

void
NodeNurbsSurface::setHandle(int handle, const Vec3f &v)
{
    MFVec3f *newValue = new MFVec3f(*getControlPoints());

    float epsilon = TheApp->GetHandleEpsilon();
    int numPoints = getControlPoints()->getSize() / 3; 
    if (handle >= 0 && handle < numPoints) {
        float w = weight()->getValue(handle);
        Vec3f oldV = getControlPoints()->getValue(handle);
        if (w != 0)
            oldV = oldV / w;
        Vec3f newV = v * w;
        if (m_scene->getXSymetricMode() && (fabs(oldV.x) < epsilon))
            newValue->setValue(handle * 3, 0);
        else
            newValue->setValue(handle * 3, newV.x);
        newValue->setValue(handle * 3 + 1, newV.y);
        newValue->setValue(handle * 3 + 2, newV.z);
        // set other handles for symetric modelling
        // which also snap handles at the same place
        setHandle(newValue, handle, w, newV, oldV, true, false);
        // search for NurbsGroup nodes and set handles
        if (m_scene->getXSymetricMode()) {
            NodeNurbsGroup *nurbsGroup = findNurbsGroup();
            if (nurbsGroup)
               nurbsGroup->setHandle(this, w, newV, oldV);
            else {
               NodeNurbsSet *nurbsSet = findNurbsSet();
               if (nurbsSet)
                   nurbsSet->setHandle(this, w, newV, oldV);
            }
        }
    }
}

int
NodeNurbsSurface::findSpan(int dimension, int order, float u,
                           const float knots[])
{
    int low, mid, high;
    int n = dimension + order - 1;

    if (u >= knots[n]) {
        return n - order;
    }
    low = order - 1;
    high = n - order + 1;

    mid = (low + high) / 2;

    int oldLow = low;
    int oldHigh = high;
    int oldMid = mid;
    while (u < knots[mid] || u >= knots[mid+1]) {
        if (u < knots[mid]) {
            high = mid;
        } else {
            low = mid;
        }
        mid = (low+high)/2;

        // emergency abort of loop, otherwise a endless loop can occure
        if ((low == oldLow) && (high == oldHigh) && (mid == oldMid))
            break;

        oldLow = low;
        oldHigh = high;
        oldMid = mid;
    }
    return mid;
}

void
NodeNurbsSurface::basisFuns(int span, float u, int order,
                            const float knots[], float basis[],
                            float deriv[])
{
    float *left = (float *) malloc(order * sizeof(float));
    float *right = (float *) malloc(order * sizeof(float));

    if ((left==NULL) || (right==NULL))
       return;
    basis[0] = 1.0f;
    for (int j = 1; j < order; j++) {
        left[j] = u - knots[span+1-j];
        right[j] = knots[span+j]-u;
        float saved = 0.0f, dsaved = 0.0f;
        for (int r = 0; r < j; r++) {
            if ((right[r+1] + left[j-r]) == 0) {
                free(left);
                free(right);
                return;
            }
            float temp = basis[r] / (right[r+1] + left[j-r]);
            basis[r] = saved + right[r+1] * temp;
            deriv[r] = dsaved - j * temp;
            saved = left[j-r] * temp;
            dsaved = j * temp;
        }
        basis[j] = saved;
        deriv[j] = dsaved;
    }
    free(left);
    free(right);
}

Vec3f
NodeNurbsSurface::surfacePoint(int uDimension, int uOrder, const float uKnots[],
                               int vDimension, int vOrder, const float vKnots[],
                               const Vec3f controlPoints[],
                               const float weight[], float u, float v,
                               Vec3f &normal)
{
    float *uBasis = (float *) malloc(uOrder * sizeof(float));
    float *vBasis = (float *) malloc(vOrder * sizeof(float));
    float *uDeriv = (float *) malloc(uOrder * sizeof(float));
    float *vDeriv = (float *) malloc(vOrder * sizeof(float));

    int uSpan = findSpan(uDimension, uOrder, u, uKnots);
    int vSpan = findSpan(vDimension, vOrder, v, vKnots);

    basisFuns(uSpan, u, uOrder, uKnots, uBasis, uDeriv);
    basisFuns(vSpan, v, vOrder, vKnots, vBasis, vDeriv);

    int uBase = uSpan-uOrder+1;
    int vBase = vSpan-vOrder+1;

    int index = vBase*uDimension + uBase;
    Vec3f S(0.0f, 0.0f, 0.0f), du(0.0f, 0.0f, 0.0f), dv(0.0f, 0.0f, 0.0f);
    float w = 0.0f, duw = 0.0f, dvw = 0.0f;
    for (int j = 0; j < vOrder; j++) {
        for (int i = 0; i < uOrder; i++) {
            float gain = uBasis[i] * vBasis[j];
            float dugain = uDeriv[i] * vBasis[j];
            float dvgain = uBasis[i] * vDeriv[j];
            S += controlPoints[index] * gain;
            w += weight[index] * gain;
            du += controlPoints[index] * dugain;
            dv += controlPoints[index] * dvgain;
            duw += weight[index] * dugain;
            dvw += weight[index] * dvgain;
            index++;
        }
        index += uDimension - uOrder;
    }
    S = S / w;
    Vec3f un = (du - S * duw) / w;
    Vec3f vn = (dv - S * dvw) / w;
    normal = un.cross(vn);
    normal.normalize();
    free(uBasis);
    free(vBasis);
    free(uDeriv);
    free(vDeriv);
    return S;
}

int
NodeNurbsSurface::writeProto(int f)
{
    if (m_scene->isX3dXml())
        return writeX3dProto(f);

    RET_ONERROR( mywritestr(f ,"EXTERNPROTO NurbsSurface[\n") )    
    TheApp->incSelectionLinenumber();
    RET_ONERROR( writeProtoArguments(f) )
    RET_ONERROR( mywritestr(f ," ]\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f ,"[\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f ," \"urn:web3d:vrml97:node:NurbsSurface\",\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f ," \"urn:inet:blaxxun.com:node:NurbsSurface\",\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f ," \"urn:ParaGraph:NurbsSurface\",\n") )
    TheApp->incSelectionLinenumber();
#ifdef HAVE_VRML97_AMENDMENT1_PROTO_URL
    RET_ONERROR( mywritestr(f ," \"") )
    RET_ONERROR( mywritestr(f ,HAVE_VRML97_AMENDMENT1_PROTO_URL) )
    RET_ONERROR( mywritestr(f ,"/NurbsSurfacePROTO.wrl") )
    RET_ONERROR( mywritestr(f ,"\"\n") )
    TheApp->incSelectionLinenumber();
#else
    RET_ONERROR( mywritestr(f ," \"NurbsSurfacePROTO.wrl\",\n") )
    TheApp->incSelectionLinenumber();
#endif
    RET_ONERROR( mywritestr(f ," \"http://wdune.ourproject.org/docs/vrml97Amendment1/NurbsSurfacePROTO.wrl\"\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f ,"]\n") )
    TheApp->incSelectionLinenumber();
    return 0;
}

void
NodeNurbsSurface::rotate(SFRotation rot)
{
    int numPoints = getControlPoints()->getSize() / 3;

    for (int i = 0; i < numPoints; i++) {
        Vec3f vPoint = getControlPoints()->getValue(i);
        vPoint = rot.getQuat() * vPoint;
        getControlPoints()->setSFValue(i, new SFVec3f(vPoint));
    }        
}

void
NodeNurbsSurface::flip(int index)
{
    if (getControlPoints())
        getControlPoints()->flip(index);
    SFBool *bccw = new SFBool(!(ccw()->getValue()));
    ccw(bccw);
    m_meshDirty = true;    
}

void
NodeNurbsSurface::swap(int fromTo)
{
    if (getControlPoints())
        getControlPoints()->swap(fromTo);
    SFBool *bccw = new SFBool(!(ccw()->getValue()));
    ccw(bccw);
    m_meshDirty = true;    
}

void 
NodeNurbsSurface::linearUknot(void)
{
    int iuOrder = uOrder()->getValue();
    int iuDimension = uDimension()->getValue();
    float *uknots = new float[iuOrder + iuDimension]; 
    for(int i = 0; i < iuOrder; i++) {
        uknots[i] = 0.0f;
        uknots[i + iuDimension] = (float) (iuDimension - iuOrder + 1);
    }
    for(int i = 0; i < (iuDimension - iuOrder); i++) {
        uknots[i + iuOrder] = (float)(i + 1);  
    } 
    for (int i = 0; i < (iuOrder + iuDimension); i++)
        uknots[i] /= (iuDimension - iuOrder + 1);
    m_scene->setField(this, uKnot_Field(), 
                     new MFFloat(uknots, iuOrder + iuDimension));
    m_meshDirty = true;    
}

void 
NodeNurbsSurface::linearVknot(void)
{
    int ivOrder = vOrder()->getValue();
    int ivDimension = vDimension()->getValue();
    float *vknots = new float[ivOrder + ivDimension]; 
    for(int i = 0; i < ivOrder; i++) {
        vknots[i] = 0.0f;
        vknots[i + ivDimension] = (float) (ivDimension - ivOrder + 1);
    }
    for(int i = 0; i < (ivDimension - ivOrder); i++) {
        vknots[i + ivOrder] = (float)(i + 1);  
    } 
    for (int i = 0; i < (ivOrder + ivDimension); i++)
        vknots[i] /= (ivDimension - ivOrder + 1);
    m_scene->setField(this, vKnot_Field(), 
                     new MFFloat(vknots, ivOrder + ivDimension));
    m_meshDirty = true;    
}

Node*
NodeNurbsSurface::degreeElevate(int newUDegree, int newVDegree)
{
  
  //degree elevate a nurbs surface,  
  if(newUDegree < ((uOrder()->getValue())-1)){
    return NULL;
  }
  if(newVDegree < ((vOrder()->getValue())-1)){
    return NULL;
  }
  
  NodeNurbsSurface *node = (NodeNurbsSurface *)
                         m_scene->createNode("NurbsSurface");

  if((newUDegree >= ((uOrder()->getValue())-1)) && 
     (newVDegree >= ((vOrder()->getValue())-1))){
    
    //load old values
    int tuDimension = uDimension()->getValue();
    int tvDimension = vDimension()->getValue();
    Vec3f *tPoints = new Vec3f[tuDimension * tvDimension];
    float *tWeights = new float[tuDimension * tvDimension];
    int tuOrder = uOrder()->getValue();
    int tvOrder = vOrder()->getValue();
    int uKnotSize = uKnot()->getSize();
    int vKnotSize = vKnot()->getSize();
    MyArray<float> tuKnots(uKnotSize);
    MyArray<float> tvKnots(vKnotSize);
    int tuDegree = tuOrder - 1;
    int tvDegree = tvOrder - 1;
    int tuUpDegree = newUDegree - tuDegree;
    int tvUpDegree = newVDegree - tvDegree;
    
    for (int i=0; i<(tuDimension*tvDimension); i++){
      tPoints[i] = getControlPoints()->getValue(i);
      tWeights[i] =weight()->getValue(i);
    }

    for (int i=0; i<uKnotSize; i++){
      tuKnots[i] = uKnot()->getValue(i);
    }
    for (int i=0; i<vKnotSize; i++){
      tvKnots[i] = vKnot()->getValue(i);
    }

    //elevate surface
    NurbsSurfaceDegreeElevate elevatedSurface(tPoints, tWeights, 
          tuKnots, tvKnots, tuDimension, tvDimension, tuDegree, tvDegree, 
          tuUpDegree, tvUpDegree);

    //load new node
    
    int newUDimension = elevatedSurface.getUDimension();
    int newVDimension = elevatedSurface.getVDimension();
    float *newControlPoints = new float[newUDimension * newVDimension *3];
    float *newWeights = new float[newUDimension * newVDimension];
    float *newUKnots = new float[elevatedSurface.getUKnotSize()];
    float *newVKnots = new float[elevatedSurface.getVKnotSize()];
    int newUOrder = newUDegree + 1;
    int newVOrder = newVDegree + 1;
    
    for(int i=0; i<(newUDimension * newVDimension); i++){
      newControlPoints[(i*3)] = elevatedSurface.getControlPoints(i).x;
      newControlPoints[(i*3)+1] = elevatedSurface.getControlPoints(i).y;
      newControlPoints[(i*3)+2] = elevatedSurface.getControlPoints(i).z;
      newWeights[i] = elevatedSurface.getWeights(i);
    }
    for(int i=0; i<(elevatedSurface.getUKnotSize()); i++){
      newUKnots[i] = elevatedSurface.getUKnots(i);
    }
    for(int i=0; i<(elevatedSurface.getVKnotSize()); i++){
      newVKnots[i] = elevatedSurface.getVKnots(i);
    }
   
    node->setField(node->uDimension_Field(), new SFInt32(newUDimension));
    node->setField(node->vDimension_Field(), new SFInt32(newVDimension));
    node->uKnot(new MFFloat(newUKnots, newUDimension + newUOrder));
    node->vKnot(new MFFloat(newVKnots, newVDimension + newVOrder));
    node->setField(node->uOrder_Field(), new SFInt32(newUOrder));
    node->setField(node->vOrder_Field(), new SFInt32(newVOrder));
    node->createControlPoints(new MFVec3f(newControlPoints, 
                                          newUDimension * newVDimension * 3));
    node->weight(new MFFloat(newWeights, newUDimension * newVDimension));
    node->uTessellation(new SFInt32(uTessellation()->getValue()));
    node->vTessellation(new SFInt32(vTessellation()->getValue()));
    node->texCoord(new SFNode(texCoord()->getValue()));
    node->solid(new SFBool(solid()->getValue()));
    node->ccw(new SFBool(ccw()->getValue()));
    
    return node;
  }
  return NULL;
}

int
NodeNurbsSurface::getAnimatedNodeField(int field)
{
    if (field == controlPointX3D_Field()) {
        Node *node = controlPointX3D()->getValue();
        if ((node != NULL) && (node->getType() == VRML_COORDINATE)) {
            NodeCoordinate *coord = (NodeCoordinate *)node;
            return coord->point_Field();
        }
        if ((node != NULL) && (node->getType() == X3D_COORDINATE_DOUBLE)) {
            NodeCoordinateDouble *coord = (NodeCoordinateDouble *)node;
            return coord->point_Field();
        }
    }
    return -1;
}

void *
NodeNurbsSurface::initializeData(void)
{
    int size = getControlPoints()->getSFSize();

    if (size != uDimension()->getValue() * vDimension()->getValue())
        return NULL;

    return new Vec3f[size];
}

void    
NodeNurbsSurface::loadDataFromInterpolators(void *data, Interpolator *inter,
                                            int field, float key)
{
    inter->interpolate(key, (float *)data);
}

void    
NodeNurbsSurface::createMeshFromData(void* data, bool optimize)
{
    m_meshDirty = true;
    createMesh((Vec3f *)data, optimize, true);
}

void    
NodeNurbsSurface::finalizeData(void* data)
{
    delete [] (Vec3f *)data;
}

void
NodeNurbsSurface::backupFieldsAppend(int field)
{
    m_scene->addNextCommand();
    m_scene->backupFieldsAppend(this, field);
    if (controlPointX3D() &&
        controlPointX3D()->getType() == X3D_COORDINATE_DOUBLE) {
        NodeCoordinateDouble *coord = (NodeCoordinateDouble *)
                                      controlPointX3D()->getValue();
        if (coord != NULL)
            m_scene->backupFieldsAppend(coord, coord->point_Field());
    } else if (controlPointX3D()) {
        NodeCoordinate *coord = (NodeCoordinate *)controlPointX3D()->getValue();
        if (coord != NULL)
            m_scene->backupFieldsAppend(coord, coord->point_Field());
    }
}

bool
NodeNurbsSurface::checkXSymetricOrSameHandle(int handle, MFVec3f *points)
{
    if ((handle >= points->getSFSize()) || (handle < 0))
        return true;
    float wIndex = weight()->getValue(handle);
    Vec3f vIndex = points->getValue(handle);
    float epsilon = TheApp->GetHandleEpsilon();
    for (int i = 0; i < m_scene->getSelectedHandlesSize(); i++) {
        int checkedHandle = m_scene->getSelectedHandle(i);
        if ((checkedHandle >= points->getSFSize()) || (checkedHandle < 0))
            continue;
        if (checkedHandle != handle) {
            float wPoint = weight()->getValue(checkedHandle);
            if (wPoint == 0.0f) {
                if (wIndex == 0.0f)
                    return true;
                else
                    return false;
            }
            if (wIndex == 0.0f)
                return false; 
            Vec3f vPoint = points->getValue(checkedHandle);
            if (   (fabs(vPoint.z / wPoint - vIndex.z / wIndex) < epsilon) 
                && (fabs(vPoint.y / wPoint - vIndex.y / wIndex) < epsilon)) {
                if (fabs(vPoint.x / wPoint - vIndex.x / wIndex) < epsilon) {
                    return true;
                } else                 
                    if (fabs(vPoint.x / wPoint + vIndex.x / wIndex) < epsilon)
                        return true;         
            }                 
        }
    }
    return false;
}

bool    
NodeNurbsSurface::validHandle(int handle) 
{ 
    if ((handle < 0) || (handle >= controlPoint()->getSFSize()))
        return false;
    for (int i = 0; i < m_scene->getNumHiddenVertices(); i++)
        if (m_scene->getHiddenVertex(i) == handle)
            return false;
    if (!m_scene->getXSymetricMode())
        return true;
    
    return !checkXSymetricOrSameHandle(handle, controlPoint());
}

Node *
NodeNurbsSurface::toNurbsTrimmedSurface(void)
{
    NodeNurbsTrimmedSurface *ret = (NodeNurbsTrimmedSurface *)
                                   m_scene->createNode("NurbsTrimmedSurface");
    ret->setField(ret->uDimension_Field(), 
                  new SFInt32(uDimension()->getValue()));
    ret->setField(ret->vDimension_Field(), 
                  new SFInt32(vDimension()->getValue()));
    ret->setField(ret->uOrder_Field(), new SFInt32(uOrder()->getValue()));
    ret->setField(ret->vOrder_Field(), new SFInt32(vOrder()->getValue()));
    ret->uTessellation(new SFInt32(uTessellation()->getValue()));
    ret->vTessellation(new SFInt32(vTessellation()->getValue()));
    ret->texCoord(new SFNode(*(SFNode *)texCoord()));
    ret->solid(new SFBool(solid()->getValue()));
    ret->ccw(new SFBool(ccw()->getValue()));

    MFFloat *weights = weight();
    ret->weight(new MFFloat(((MFFloat *)weights->copy())));

    MFFloat *uknots = uKnot();
    ret->uKnot(new MFFloat(((MFFloat *)uknots->copy())));

    MFFloat *vknots = vKnot();
    ret->vKnot(new MFFloat(((MFFloat *)vknots->copy())));

    MFVec3f *points = NULL;
    points = getControlPoints();
    MFVec3f *newPoints = new MFVec3f();
    for (int i = 0; i < points->getSFSize(); i++)
        newPoints->setVec(i, points->getVec(i));
    NodeCoordinate *node = (NodeCoordinate *)
                           m_scene->createNode("Coordinate");
    node->point(newPoints);

    SFNode *old = controlPointX3D();
    if (old != NULL)
        if (old->getValue() != NULL)
            m_scene->execute(new MoveCommand(old->getValue(), 
                                            this, controlPointX3D_Field(),
                                            NULL, -1));
    m_scene->execute(new MoveCommand(node, NULL, -1, 
                                     ret, ret->controlPointX3D_Field()));
    m_scene->changeRoutes(node, node->point_Field(),
                          ret, ret->controlPoint_Field());        
    return ret;
}

Node *            
NodeNurbsSurface::setWeightsTo1(void)
{
    NodeNurbsSurface *ret = (NodeNurbsSurface *)copy();
    
    MFFloat *weights = weight();
    MFVec3f *points = getControlPoints();
    if (weights->getSize() < points->getSFSize()) {
        swDebugf("not enough weights\n");
        return NULL;
    }
    MFVec3f *newPoints = new MFVec3f();
    MFFloat *newWeights = new MFFloat();
    for (int i = 0; i < points->getSFSize(); i++) {
        Vec3f p = points->getVec(i); 
        p = p / weights->getValue(i);
        newPoints->setVec(i, p);
        newWeights->setValue(i, 1.0f);
    }
    
    m_scene->backupFieldsStart();
    if (controlPointX3D()->getType() == X3D_COORDINATE_DOUBLE) {
        NodeCoordinateDouble *coord = (NodeCoordinateDouble *)
                                      controlPointX3D()->getValue();
        if (coord != NULL)
            m_scene->backupFieldsAppend(coord, coord->point_Field());
    } else {
        NodeCoordinate *coord = (NodeCoordinate *)controlPointX3D()->getValue();
        if (coord != NULL)
            m_scene->backupFieldsAppend(coord, coord->point_Field());
    }
    m_scene->backupFieldsAppend(ret, controlPoint_Field());
    m_scene->backupFieldsAppend(ret, weight_Field());
    m_scene->backupFieldsDone();

    ret->weight(newWeights);
    ret->setControlPoints(newPoints);
   
    return ret;
}


Node*
NodeNurbsSurface::toNurbs(int uTessel, int vTessel, int uDegree, int vDegree)
{   
    NodeNurbsSurface *node = (NodeNurbsSurface *) m_scene->createNode(
                             "NurbsSurface");

    int vOrder = vDegree + 1;
    int uOrder = uDegree + 1;

    int uTess = uTessel;
    int vTess = vTessel; 

    uTessellation(new SFInt32(uTess - 1));
    vTessellation(new SFInt32(vTess - 1));
    setCreateExtraTess(false);
    createMesh(false);
    m_meshDirty = true;
    if (m_mesh == NULL)
        return NULL;

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

    for (int iteration = 0; iteration < 64; iteration++) {
        node->reInit();
        node->setCreateExtraTess(false);
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

    setField(node->uTessellation_Field(), new SFInt32(0));
    setField(node->vTessellation_Field(), new SFInt32(0));

    setCreateExtraTess(true);
    return node;   
}

