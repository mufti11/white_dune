/*
 * NodeGeoCoordinate.cpp
 *
 * Copyright (C) 1999 Stephen F. White, 2006/2007 BOBMAX, 
 *               2019 J. "MUFTI" Scheurich
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

#include "NodeGeoCoordinate.h"
#include "NodeCoordinate.h"
#include "Proto.h"
#include "MFVec3f.h"
#include "ExposedField.h"
#include "Field.h"
#include "RenderState.h"
#include "DuneApp.h"
#include "Util.h"
#include "Vec3f.h"
#include "Scene.h"
#include "resource.h"
#include "FaceData.h"
#include "NodeIndexedFaceSet.h"
#include "NodeIndexedLineSet.h"
#include "NodePointSet.h"
#include "NodeLineSet.h"
#include "NodeTriangleSet.h"
#include "NodeTriangleFanSet.h"
#include "NodeTriangleStripSet.h"
#include "NodeIndexedTriangleSet.h"
#include "NodeIndexedTriangleFanSet.h"
#include "NodeIndexedTriangleStripSet.h"
#include "NodeQuadSet.h"
#include "NodeIndexedQuadSet.h"
#include "NodeNurbsGroup.h"
#include "NodeNurbsSurface.h"
#include "NodeNurbsCurve.h"
#include "NodeNurbsTrimmedSurface.h"
#include "NodeHAnimHumanoid.h"
#include "NodeHAnimJoint.h"
#include "NodeGeoOrigin.h"

ProtoGeoCoordinate::ProtoGeoCoordinate(Scene *scene)
  : GeoProto(scene, "GeoCoordinate")
{
    point.set(
          addExposedField(MFSTRING, "point", new MFString()));
    setFieldFlags(point, FF_VRML_ONLY | EIF_RECOMMENDED);
    pointX3D.set(
          addExposedField(MFVEC3D, "point", new MFVec3d()));
    setFieldFlags(pointX3D, FF_X3D_ONLY | EIF_RECOMMENDED);
}

Node *
ProtoGeoCoordinate::create(Scene *scene)
{ 
    return new NodeGeoCoordinate(scene, this); 
}

int 
ProtoGeoCoordinate::translateField(int field) const
{
    bool x3d = m_scene->isX3d();
    if (x3d && (field == point))
        return pointX3D;
    else if (!x3d && (field == pointX3D))
        return point;
    return field;
}

NodeGeoCoordinate::NodeGeoCoordinate(Scene *scene, Proto *def)
  : GeoNode(scene, def)
{
}

Node *
NodeGeoCoordinate::convert2Vrml(void) 
{
    const double *values = pointX3D()->getValues();
    MFString *strings = new MFString();
    for (int i = 0; i < pointX3D()->getSFSize(); i++) {
        char string[4096];
        mysnprintf(string, 4095, "%g %g %g",
                   values[i * 3], values[i * 3 + 1], values[i * 3 + 2]);
        strings->insertSFValue(i, strdup(string));
    }
    point(strings);    
    return NULL;
}

void
NodeGeoCoordinate::setField(int index, FieldValue *value, int cf)
{
    bool setNurbsField = false;
    if (hasParent()) {
        for (int i = 0; i < getNumParents(); i++) {
            Node *parent = getParent(i);
            if (parent->getType() == VRML_NURBS_SURFACE) {
                MFVec3f *newValue = new MFVec3f((MFVec3f *)value);
                newValue->ref();
                ((NodeNurbsSurface *)parent)->setControlPoints(newValue);
                setNurbsField = true;
            } else if (parent->getType() == X3D_NURBS_TRIMMED_SURFACE) {
                MFVec3f *newValue = new MFVec3f((MFVec3f *)value);
                newValue->ref();
                ((NodeNurbsTrimmedSurface *)parent)->setControlPoints(newValue);
                setNurbsField = true;
            } else if (parent->getType() == VRML_NURBS_CURVE) {
                MFVec3f *newValue = new MFVec3f((MFVec3f *)value);
                newValue->ref();
                ((NodeNurbsCurve *)parent)->setControlPoints(newValue);
                setNurbsField = true;
            } else
                parent->update();
        }
    }
    if (!setNurbsField)        
        Node::setField(index, value, cf);
}

void
NodeGeoCoordinate::drawHandles(void)
{
    RenderState state;

    glPushMatrix();

    NodeGeoOrigin *origin = (NodeGeoOrigin *)geoOrigin()->getValue();

    if (origin) {
        Vec3d vec = origin->getVec();
        glTranslated(vec.x, vec.y, vec.z);
    }

    glPushAttrib(GL_LIGHTING);
    glDisable(GL_LIGHTING);
    glPushName(0);

    int mode = m_scene->getSelectionMode();
    MyMeshDouble *mesh = getDoubleMesh();
    if (mode == SELECTION_MODE_VERTICES) {
        state.startDrawHandles();
        for (int ci = 0; ci < point()->getSFSize(); ci++) {
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
            state.drawHandle(Vec3d(pointX3D()->getValue(ci)));
        }
        state.endDrawHandles();
    } else if (mode == SELECTION_MODE_FACES) {
        if (mesh == NULL) {
            glPopName();
            glPopAttrib();
            return;
        }
        if (mesh) {
            if (mesh->ccw())
               glFrontFace(GL_CCW);
            else 
               glFrontFace(GL_CW);
        }
        state.startDrawHandles();
        MFInt32 *ci = mesh->getCoordIndex();
        for (int i = 0; i < mesh->getNumFaces(); i++) {
            FaceData *face = mesh->getFace(i);
            int offset = face->getOffset();

            glEnable(GL_COLOR_MATERIAL);
            glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE); 

            if (m_scene->isInSelectedHandles(i))
                Util::myGlColor3f(1.0f, 1.0f, 1.0f);
            else
                Util::myGlColor4f(0.8f, 0.8f, 0.8f, 0.0f);
            glLineWidth(2.0f);
            glPolygonMode(GL_FRONT, GL_LINE);
            glLoadName(NO_HANDLE);
            glBegin(GL_POLYGON);
            for (int j = offset; j < offset + face->getNumVertices(); j++) {
                if (ci->getValue(j) >= point()->getSFSize())
                    continue;
                Vec3d vec = pointX3D()->getValue(ci->getValue(j));
                Vec3d w = state.project(vec);
                glVertex3d(w.x, w.y, w.z);
            }
            glEnd();

            Util::myGlColor4f(0.8f, 0.8f, 0.8f, 0.0f);

            glEnable(GL_POLYGON_OFFSET_FILL);
            glPolygonOffset(0.0f, 2.0f);

            glPolygonMode(GL_FRONT, GL_FILL);

            glLineWidth(1.0f);

            glLoadName(i);
            glBegin(GL_POLYGON);
            for (int j = offset; j < offset + face->getNumVertices(); j++) {
                if (ci->getValue(j) >= point()->getSFSize())
                    continue;
                Vec3d vec = pointX3D()->getValue(ci->getValue(j));
                Vec3d w = state.project(vec);
                glVertex3d(w.x, w.y, w.z);
            }
            glEnd();

            glPolygonOffset(0.0f, 0.0f);
            glDisable(GL_POLYGON_OFFSET_FILL);
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }
        state.endDrawHandles();
    } else if (mode == SELECTION_MODE_LINES) {
        if (mesh == NULL) {
            glPopName();
            glPopAttrib();
            return;
        }
        if (mesh) {
            if (mesh->ccw())
               glFrontFace(GL_CCW);
            else 
               glFrontFace(GL_CW);
        }
        state.startDrawHandles();
        MFInt32 *ci = mesh->getCoordIndex();
        for (int i = 0; i < mesh->getNumLines(); i++) {
            glEnable(GL_COLOR_MATERIAL);
            glColorMaterial(GL_BACK, GL_AMBIENT_AND_DIFFUSE); 

            int numLine = mesh->getLine(i).begin;
            if (m_scene->isInSelectedHandles(numLine))
                Util::myGlColor3f(0.0f, 1.0f, 0.0f);
            else
                Util::myGlColor3f(1.0f, 1.0f, 1.0f);

            glLoadName(numLine);
            glBegin(GL_LINES);
            Vec3d vec = pointX3D()->getValue(ci->getValue(numLine));
            Vec3d w = state.project(vec);
            glVertex3d(w.x, w.y, w.z);
            vec = pointX3D()->getValue(ci->getValue(mesh->getLine(i).end));
            w = state.project(vec);
            glVertex3d(w.x, w.y, w.z);
            glEnd();
        }
        state.endDrawHandles();
    } if (mode == SELECTION_HANIM_JOINT_WEIGHT) {
        NodeHAnimHumanoid *human = this->getHumanoid();
        if (human != NULL) {
            state.startDrawHandles();
            NodeHAnimJoint *joint = (NodeHAnimJoint *)
                                    m_scene->getLastSelectedHAnimJoint();
            if (joint) {
                for (int ci = 0; ci < point()->getSFSize(); ci++)
                    if (joint->skinCoordIndex()->find(ci) > -1) { 
                        bool hidden = false;
                        for (int i = 0; i < m_scene->getNumHiddenVertices(); 
                             i++)
                            if (m_scene->getHiddenVertex(i) == ci) {
                                hidden = true;
                                break;
                            }
                        if (hidden)
                           continue;
                        state.setHandleColor(m_scene, ci);
                        glLoadName(ci);
                        state.drawHandle(Vec3d(pointX3D()->getValue(ci)));
                    }
            } else {
                human->buildJointHasWeightArray();
                MyArray<int> *arrayPtr = human->getJointHasNoWeightArray();
                for (size_t i = 0; i < (*arrayPtr).size(); i++) {
                    int ci = (*arrayPtr)[i];
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
                    state.drawHandle(Vec3d(pointX3D()->getValue(ci)));
                }
            }
            state.endDrawHandles();
        }
    }
    glPopName();
    glPopAttrib();

    glPopMatrix();
}

Vec3f
NodeGeoCoordinate::getHandle(int handle, int *constraint, int *field)
{
    *field = point_Field() ;

    int mode = m_scene->getSelectionMode();
    if (mode == SELECTION_MODE_VERTICES) {
        if (handle >= 0 && handle < point()->getSFSize()) {
            Vec3f ret((Vec3f)pointX3D()->getMFVec3f()->getValue(handle));
            TheApp->PrintMessageWindowsVertex(IDS_VERTEX_SELECTED, "point", 
                handle, pointX3D()->getMFVec3f()->getValue(handle));
            return ret;
        }
    } else if (mode == SELECTION_MODE_FACES) {
        MyMeshDouble *mesh = getDoubleMesh();
        if (mesh == NULL)
            return Vec3f(0.0f, 0.0f, 0.0f);
        MFInt32 *ci = mesh->getCoordIndex();
        if (handle >= 0 && handle < mesh->getNumFaces()) {
            FaceData *face = mesh->getFace(handle);
            int offset = face->getOffset();
            Vec3f first;
            if (face->getNumVertices() > 0)
                first = pointX3D()->getMFVec3f()->
                        getValue(ci->getValue(offset));
            return first;
        }
    } else if (mode == SELECTION_MODE_LINES) {
        MyMesh *mesh = getMesh();
        if (mesh == NULL)
            return Vec3f(0.0f, 0.0f, 0.0f);
        MFInt32 *ci = mesh->getCoordIndex();
        if (handle >= 0 && handle < ci->getSFSize()) {
            Vec3f first = pointX3D()->getMFVec3f()->
                          getValue(ci->getValue(handle));
            return first;
        }
    }
    if (mode == SELECTION_HANIM_JOINT_WEIGHT) {
        if (handle >= 0 && handle < point()->getSFSize()) {
            Vec3f ret(pointX3D()->getMFVec3f()->getValue(handle));
            TheApp->PrintMessageWindowsVertex(IDS_VERTEX_SELECTED, "point", 
                handle, (Vec3f)pointX3D()->getMFVec3f()->getValue(handle));
            return ret;
        }
    }
    return Vec3f(0.0f, 0.0f, 0.0f);
}

void
NodeGeoCoordinate::setHandle(int handle, const Vec3f &v)
{
    int mode = m_scene->getSelectionMode();
    if (mode == SELECTION_MODE_VERTICES)
        setHandleVertices(handle, v);
    else if (mode == SELECTION_MODE_FACES)
        setHandleFaces(handle, v);
    else if (mode == SELECTION_MODE_LINES)
        setHandleLines(handle, v);
}

void
NodeGeoCoordinate::setBeginHandles(void)
{
    m_selectedVerticesHandles.resize(0);
    m_selectedVertices.resize(0);
    m_selectedVertices2.resize(0);
    m_selectedVerticesWithoutX.resize(0);
    m_validSymVerticesHandles.resize(0);
}

void
NodeGeoCoordinate::setEndHandles(void)
{
    MyMesh *mesh = getMesh();
    if (mesh == NULL)
        return;
    if ((m_scene->getSelectionMode() == SELECTION_MODE_FACES) ||
        (m_scene->getSelectionMode() == SELECTION_MODE_LINES)) {
        for (size_t i = 0; i < m_selectedVerticesHandles.size(); i++)
            if (m_validSymVerticesHandles[i]) {
                setHandleVertices(m_selectedVerticesHandles[i], 
                                  m_selectedVerticesWithoutX[i]);
            } else {
                setHandleVertices(m_selectedVerticesHandles[i], 
                                  m_selectedVertices2[i]);
            }
    }
}

void
NodeGeoCoordinate::setHandleFaces(int handle, const Vec3f &v)
{
    MyMesh *mesh = getMesh();
    if (mesh == NULL)
        return;
    float epsilon = TheApp->GetHandleEpsilon();
    MFInt32 *ci = mesh->getCoordIndex();
    if (handle >= mesh->getNumFaces())
        return;
    FaceData *face = mesh->getFace(handle);
    int offset = face->getOffset();
    Vec3f first;
    if (face->getNumVertices() > 0)
        first = pointX3D()->getMFVec3f()->getValue(ci->getValue(offset));
    for (int j = offset; j < offset + face->getNumVertices(); j++) {
        Vec3f vec = pointX3D()->getMFVec3f()->getValue(ci->getValue(j));
        Vec3f vec2 = vec - first + v;
        Vec3f vecWithoutX = vec2;
        vecWithoutX.x = vec.x;
        bool validSymVerticesHandles = false;
        bool isInSelectedVertices = false;
        for (size_t n = 0; n < m_selectedVertices.size(); n++) {
            if (m_selectedVerticesHandles[n] == ci->getValue(j)) {
                isInSelectedVertices = true;
                break;
            }    
            if ((!isInSelectedVertices) && m_scene->getXSymetricMode() &&
                (fabsf(m_selectedVertices[n].x + vec.x) < epsilon) &&
                (fabsf(m_selectedVertices[n].y - vec.y) < epsilon) &&
                (fabsf(m_selectedVertices[n].z - vec.z) < epsilon)) {
                validSymVerticesHandles = true;
                m_validSymVerticesHandles[n] = true;
                break;
            }
        }          
        if (!isInSelectedVertices) {
            m_selectedVerticesHandles.append(ci->getValue(j));
            m_selectedVertices.append(vec);
            m_selectedVertices2.append(vec2);
            m_selectedVerticesWithoutX.append(vecWithoutX);
            m_validSymVerticesHandles.append(validSymVerticesHandles);
        }
    }
}

void
NodeGeoCoordinate::setHandleLines(int handle, const Vec3f &v)
{
    MyMesh *mesh = getMesh();
    if (mesh == NULL)
        return;
    float epsilon = TheApp->GetHandleEpsilon();
    MFInt32 *ci = mesh->getCoordIndex();
    if (handle >= ci->getSFSize())
        return;
    int index = -1;
    for (int i = 0; i < mesh->getNumLines(); i++)
        if (mesh->getLine(i).begin == handle) {
            index = i;
            break;
        }
    if (index < 0)
        return;
    int index1 = mesh->getLine(index).begin;
    int index2 = mesh->getLine(index).end;
    Vec3f vec = pointX3D()->getMFVec3f()->getValue(ci->getValue(index1));
    Vec3f first = vec;
    Vec3f vec2 = vec + v - first;
    Vec3f vecWithoutX = vec2;
    vecWithoutX.x = vec.x;
    bool validSymVerticesHandles = false;
    bool isInSelectedVertices = false;
    for (size_t n = 0; n < m_selectedVertices.size(); n++) {
        if (m_selectedVerticesHandles[n] == ci->getValue(index1)) {
            isInSelectedVertices = true;
            break;
        }   
        if ((fabsf(m_selectedVertices[n].x + vec.x) < epsilon) &&
            (fabsf(m_selectedVertices[n].y - vec.y) < epsilon) &&
            (fabsf(m_selectedVertices[n].z - vec.z) < epsilon)) {
            m_validSymVerticesHandles[n] = true;            
            validSymVerticesHandles = true;
            break;
        }          
    } 
    if (!isInSelectedVertices) {
        m_selectedVerticesHandles.append(ci->getValue(index1));
        m_selectedVertices.append(vec);
        m_selectedVertices2.append(vec2);
        m_selectedVerticesWithoutX.append(vecWithoutX);
        m_validSymVerticesHandles.append(validSymVerticesHandles);
    }
    vec = pointX3D()->getMFVec3f()->getValue(ci->getValue(index2));
    vec2 = vec + v - first;
    isInSelectedVertices = false;
    for (size_t n = 0; n < m_selectedVertices.size(); n++) {
        if (m_selectedVerticesHandles[n] == ci->getValue(index2))
        {
            isInSelectedVertices = true;
            break;
        }    
        if (m_scene->getXSymetricMode() &&
            (fabsf(m_selectedVertices[n].x + vec.x) < epsilon) &&
            (fabsf(m_selectedVertices[n].y - vec.y) < epsilon) &&
            (fabsf(m_selectedVertices[n].z - vec.z) < epsilon)) {
            validSymVerticesHandles = true;
            m_validSymVerticesHandles[n] = true;
            break;
        }          
    }
    if (!isInSelectedVertices) {
        m_selectedVerticesHandles.append(ci->getValue(index2));
        m_selectedVertices.append(vec);
        m_selectedVertices2.append(vec2);
        m_selectedVerticesWithoutX.append(vecWithoutX);
        m_validSymVerticesHandles.append(validSymVerticesHandles);
    }
}

void
NodeGeoCoordinate::setHandleVertices(int handle, const Vec3f &v)
{
    MFVec3f *newValue = new MFVec3f(*pointX3D()->getMFVec3f());

    float epsilon = TheApp->GetHandleEpsilon();
    int numPoints = point()->getSFSize();
    if (handle >= 0 && handle < numPoints) {
        Vec3f oldV = pointX3D()->getMFVec3f()->getValue(handle);
        Vec3f newV = v;
        if ((v.x == - oldV.x) && (v.x != 0.0f))
            return;
        if (m_scene->getXSymetricMode() && (fabsf(oldV.x) < epsilon))
            newValue->setValue(handle * 3, 0);
        else
            newValue->setValue(handle * 3, newV.x);
        newValue->setValue(handle * 3 + 1, newV.y);
        newValue->setValue(handle * 3 + 2, newV.z);
        // set other handles for symetric modelling
        // which also snap handles at the same place
        setHandle(newValue, handle, newV, oldV, true);
    }
}

bool
NodeGeoCoordinate::setHandle(MFVec3f *value, int handle,
                          const Vec3f &newV, const Vec3f &oldV,
                          bool already_changed, bool bypassChecks)
{
    bool ret = false;
    bool changed = false;
    MFVec3f *newValue = (MFVec3f *) value->copy();

    if (m_scene->getXSymetricMode()) {
        float epsilon = TheApp->GetHandleEpsilon();
        int numPoints = newValue->getSize() / 3;
        for (int i = 0; i < numPoints; i++) {
            if (i != handle) {
                Vec3f vPoint = pointX3D()->getMFVec3f()->getValue(i);
                if (   (fabsf(vPoint.z - oldV.z) < epsilon) 
                    && (fabsf(vPoint.y - oldV.y) < epsilon)) {
                    if (fabsf(vPoint.x - oldV.x) < epsilon) {
                        ret = true;
                        changed = true;
                        if (fabsf(oldV.x) < epsilon)
                            newValue->setValue(i * 3, 0);
                        else
                            newValue->setValue(i * 3, newV.x);
                        newValue->setValue(i * 3 + 1, newV.y);
                        newValue->setValue(i * 3 + 2, newV.z);
                    } else if (fabsf(vPoint.x + oldV.x) < epsilon) {
                        ret = true;
                        changed = true;
                        if (fabsf(oldV.x) < epsilon)
                            newValue->setValue(i * 3, 0);
                        else
                            newValue->setValue(i * 3, - newV.x);
                        newValue->setValue(i * 3 + 1,   newV.y);
                        newValue->setValue(i * 3 + 2,   newV.z);
                    }  
                }                 
            }
        }
    }
    if (already_changed | bypassChecks)
        changed = true;

    if (changed) {
        m_scene->setField(this, point_Field(), newValue);
    }
    return ret;
}

bool
NodeGeoCoordinate::checkHandle(int handle)
{
    MFVec3f *points = pointX3D()->getMFVec3f();

    if (points && (points->getSize() > 0) && 
        m_scene->getXSymetricMode()) {
        float epsilon = TheApp->GetHandleEpsilon();
        Vec3f hPoint = pointX3D()->getMFVec3f()->getValue(handle);
        for (int i = 0; i < m_scene->getSelectedHandlesSize(); i++) {
            if (!validHandle(m_scene->getSelectedHandle(i)))
                continue;
            if (m_scene->getSelectedHandle(i) == handle) 
                return true;
            else {
                if (m_scene->getSelectedHandle(i) >= point()->getSFSize()) 
                    continue;
                Vec3f vPoint = pointX3D()->getMFVec3f()->
                                   getValue(m_scene->getSelectedHandle(i));
                if (   (fabsf(vPoint.z - hPoint.z) < epsilon) 
                    && (fabsf(vPoint.y - hPoint.y) < epsilon)) {
                    if (fabsf(vPoint.x - hPoint.x) < epsilon)
                        return true;
                    else if (fabsf(vPoint.x + hPoint.x) < epsilon)
                        return true;  
                }                 
            }
        }
    }
    return false;
}

int
NodeGeoCoordinate::getMaxHandle(void)
{
    if (point() == NULL)
        return 0;
    int mode = m_scene->getSelectionMode();
    if (mode == SELECTION_MODE_VERTICES)
        return point()->getSFSize() - 1;
    else if (mode == SELECTION_MODE_FACES) {
        MyMesh *mesh = getMesh();
        if (mesh == NULL)
            return 0;
        return mesh->getNumFaces();
    }     

    return point()->getSFSize() - 1;
}

void 
NodeGeoCoordinate::draw(Node *node)
{
    glPushMatrix();

    NodeGeoOrigin *origin = (NodeGeoOrigin *)geoOrigin()->getValue();

    if (origin) {
        Vec3d vec = origin->getVec();
        glTranslated(vec.x, vec.y, vec.z);
    }
    switch (node->getType()) {
      case VRML_INDEXED_FACE_SET:
        ((NodeIndexedFaceSet *)node)->meshDraw();
        break;
      case VRML_INDEXED_LINE_SET:
        ((NodeIndexedLineSet *)node)->lineDraw();
        break;
      case VRML_POINT_SET:
        ((NodePointSet *)node)->pointDraw();
        break;
      case X3D_LINE_SET:
        ((NodeLineSet *)node)->lineDraw();
        break;
      case X3D_TRIANGLE_SET:
        ((NodeTriangleSet *)node)->meshDraw();
        break;
      case X3D_TRIANGLE_FAN_SET:
        ((NodeTriangleFanSet *)node)->meshDraw();
        break;
      case X3D_TRIANGLE_STRIP_SET:
        ((NodeTriangleStripSet *)node)->meshDraw();
        break;
      case X3D_INDEXED_TRIANGLE_SET:
        ((NodeIndexedTriangleSet *)node)->meshDraw();
        break;
      case X3D_INDEXED_TRIANGLE_FAN_SET:
        ((NodeIndexedTriangleFanSet *)node)->meshDraw();
        break;
      case X3D_INDEXED_TRIANGLE_STRIP_SET:
        ((NodeIndexedTriangleStripSet *)node)->meshDraw();
        break;
      case X3D_QUAD_SET:
        ((NodeQuadSet *)node)->meshDraw();
        break;
      case X3D_INDEXED_QUAD_SET:
        ((NodeIndexedQuadSet *)node)->meshDraw();
        break;
      default:
        fprintf(stderr, "Internal error: Coordinate not drawn\n");
    }
    glPopMatrix();
}

void
NodeGeoCoordinate::update()
{
    if (hasParent())
        getParents().update();
}

bool    
NodeGeoCoordinate::validHandle(int handle) 
{ 
    if (handle < 0)
        return false;
    if (point() == NULL)
        return false;
    int mode = m_scene->getSelectionMode();
    if (mode == SELECTION_MODE_VERTICES) {
        if (handle >= point()->getSFSize())
            return false;
        for (int i = 0; i < m_scene->getNumHiddenVertices(); i++)
            if (m_scene->getHiddenVertex(i) == handle)
                return false;
        if (!m_scene->getXSymetricMode())
            return true;
        return !m_scene->checkXSymetricOrSameHandle(handle, 
                                                    pointX3D()->getMFVec3f());
    } else if (mode == SELECTION_MODE_FACES) {
        MyMesh *mesh = getMesh();
        if (mesh == NULL)
            return false;
        if (handle > mesh->getNumFaces())
            return false;
    } else if (mode == SELECTION_MODE_LINES) {
        MyMesh *mesh = getMesh();
        if (mesh == NULL)
            return false;
        if (handle > mesh->getNumLines())
            return false;
    }
    return true;
}

MyMeshDouble *
NodeGeoCoordinate::getDoubleMesh(void)
{
    NodeIndexedFaceSet *faceSet = NULL;
    MyMeshDouble *mesh = NULL;
    if (hasParent())
        if (getParent()->getType() == VRML_INDEXED_FACE_SET) {
            faceSet = (NodeIndexedFaceSet *)getParent();
            mesh = faceSet->getMeshDouble();
        }
    return mesh;
}


