/*
 * NodeHAnimHumanoid.cpp
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

#include <stdio.h>
#include "stdafx.h"
#include <sys/types.h>
#include <sys/stat.h>
#ifndef _WIN32
# include <fcntl.h>
#endif

#include "swt.h"
#include "NodeHAnimHumanoid.h"
#include "Scene.h"
#include "Proto.h"
#include "FieldValue.h"
#include "SFVec3f.h"
#include "SFRotation.h"
#include "SFFloat.h"
#include "MFNode.h"
#include "FieldCommand.h"
#include "Matrix.h"
#include "Util.h"
#include "Field.h"
#include "Path.h"
#include "NodeViewpoint.h"
#include "NodeNavigationInfo.h"
#include "ExposedField.h"
#include "NodeShape.h"
#include "NodeIndexedFaceSet.h"
#include "NodeMaterial.h"
#include "NodeHAnimJoint.h"

#include "RenderState.h"

void ProtoHAnimHumanoid::addElements(void) 
{
    info.set (
           addExposedField(MFSTRING, "info", new MFString()));
    joints.set (
           addExposedField(MFNODE, "joints", new MFNode(), X3D_HANIM_JOINT));
    name.set(
           addExposedField(SFSTRING, "name", new SFString()));
    segments.set (
           addExposedField(MFNODE, "segments", new MFNode(), X3D_HANIM_SEGMENT));
    sites.set (
           addExposedField(MFNODE, "sites", new MFNode(), X3D_HANIM_SITE));
    skeleton.set(
           addExposedField(MFNODE, "skeleton", new MFNode(), X3D_HANIM_JOINT));
    skin.set(
           addExposedField(MFNODE, "skin", new MFNode(), CHILD_NODE));
    skinCoord.set(
           addExposedField(SFNODE, "skinCoord", new SFNode(), COORDINATE_NODE));
    skinNormal.set(
           addExposedField(SFNODE, "skinNormal", new SFNode(), VRML_NORMAL));
    version.set(
           addExposedField(SFSTRING, "version", new SFString()));
    viewpoints.set(
           addExposedField(MFNODE, "viewpoints", new MFNode(), X3D_HANIM_SITE));
}

ProtoHAnimHumanoid::ProtoHAnimHumanoid(Scene *scene) 
  : TransformProto(scene, "HAnimHumanoid")
{                    
     addElements();     
}

Node *
ProtoHAnimHumanoid::create(Scene *scene)
{ 
    return new NodeHAnimHumanoid(scene, this); 
}

NodeHAnimHumanoid::NodeHAnimHumanoid(Scene *scene, Proto *def)
  : TransformNode(scene, def)
{
    m_meshes = NULL;
    m_numMeshes = 0;
    m_matrixDirty = true;
    m_hasNoWeightArrayDirty = true;
}

NodeHAnimHumanoid::~NodeHAnimHumanoid()
{
    if (m_numMeshes != 0)
        delete [] m_meshes;
}

int
NodeHAnimHumanoid::getComponentLevel(void) const
{
    return 1;
}

const char* 
NodeHAnimHumanoid::getComponentName(void) const
{
    static const char* name = "H-Anim";
    return name;
}

void
NodeHAnimHumanoid::drawHandles(void)
{
    m_scene->setLastSelectedHAnimJointOrHumanoid(this);
    float fscale = TheApp->getHumanoidScale();

    m_handleScale = TheApp->GetHandleScale() *
                    m_scene->getNavigationInfoNode()->speed()->getValue();

    RenderState state;
                   
    transform();

    TransformMode* tm=m_scene->getTransformMode();
    if (tm->tmode==TM_TRANSLATE)
       state.drawTranslationHandles(this, m_handleScale);
    else if (tm->tmode==TM_6D)
       state.draw6DHandles(this, m_handleScale);
    else if (tm->tmode==TM_6DLOCAL)
       state.draw6DlocalHandles(this, m_handleScale);
    else if (tm->tmode==TM_ROCKET)
       state.drawRocketHandles(this, m_handleScale);
    else if (tm->tmode==TM_HOVER)
       state.drawHoverHandles(this, m_handleScale);
    else if (tm->tmode==TM_ROTATE)
       state.drawRotationHandles(this, m_handleScale);
    else if (tm->tmode==TM_SCALE)
       state.drawScaleHandles(this, m_handleScale, scale()->getValue());
    else if (tm->tmode==TM_UNIFORM_SCALE)
       state.drawUniformScaleHandles(this, m_handleScale, scale()->getValue());
    else if (tm->tmode==TM_CENTER)
       state.drawCenterHandles(this, m_handleScale);

    glPushName(getBasicChildrenField());  // field
    glPushName(0);                 // index
    NodeList *childList = getBasicChildren()->getValues();
    for (long i = 0; i < childList->size(); i++) {
        if (childList->get(i) &&
            (childList->get(i)->getType() == X3D_HANIM_JOINT)) {
            glLoadName(i);
            childList->get(i)->drawJointHandles(fscale, this, NULL);
        }
    } 
    glPopName();
    glPopName();

}

void
NodeHAnimHumanoid::updateHandles(void)
{
    float fscale = TheApp->getHumanoidScale();

    NodeList *childList = getBasicChildren()->getValues();
    for (long i = 0; i < childList->size(); i++) {
        if (childList->get(i) &&
            childList->get(i)->getType() == X3D_HANIM_JOINT) {
            childList->get(i)->drawJointHandles(fscale, this, NULL);
        }
    }
}

void              
NodeHAnimHumanoid::drawJointHandle(float scale, Node *that)
{
    NodeList *childList = getBasicChildren()->getValues();
    for (long i = 0; i < childList->size(); i++) {
        if (childList->get(i) &&
            childList->get(i)->getType() == X3D_HANIM_JOINT) {
            childList->get(i)->drawJointHandles(scale, this, that);
        }
    }
}

Vec3f
NodeHAnimHumanoid::getHandle(int handle, int *constraint, int *field)
{
    const float *fscale = scale()->getValue();
    SFRotation *sfrotation = rotation();

    switch (handle) {
      case TRANSLATION_X:
        *constraint = CONSTRAIN_X;
        *field = translation_Field();
        return Vec3f(HANDLE_SIZE, 0.0f, 0.0f);
      case TRANSLATION_Y:
        *constraint = CONSTRAIN_Y;
        *field = translation_Field();
        return Vec3f(0.0f, HANDLE_SIZE, 0.0f);
      case TRANSLATION_Z:
        *constraint = CONSTRAIN_Z;
        *field = translation_Field();
        return Vec3f(0.0f, 0.0f, HANDLE_SIZE);
      case TRANSLATION:
        *field = translation_Field();
        return Vec3f(0.0f, 0.0f, 0.0f);
      case ROTATION:
        *field = rotation_Field();
        *constraint = CONSTRAIN_SPHERE;
        return sfrotation->getEulerAngles(0);
      case ROTATION_X:
        *field = rotation_Field();
        *constraint = CONSTRAIN_YZ;
        return Vec3f(sfrotation->getEulerAngles(0).x, 0.0f, 0.0f);
      case ROTATION_Y:
        *field = rotation_Field();
        *constraint = CONSTRAIN_ZX;
        return Vec3f(0.0f, sfrotation->getEulerAngles(0).y, 0.0f);
      case ROTATION_Z:
        *field = rotation_Field();
        *constraint = CONSTRAIN_XY;
        return Vec3f(0.0f, 0.0f, sfrotation->getEulerAngles(0).z);
      case SCALE_X:
      case UNIFORM_SCALE_X:
        *constraint = CONSTRAIN_X;
        *field = scale_Field();
        return Vec3f(fscale[0] * HANDLE_SIZE, 0.0f, 0.0f);
      case SCALE_Y:
      case UNIFORM_SCALE_Y:
        *constraint = CONSTRAIN_Y;
        *field = scale_Field();
        return Vec3f(0.0f, fscale[1] * HANDLE_SIZE, 0.0f);
      case SCALE_Z:
      case UNIFORM_SCALE_Z:
        *constraint = CONSTRAIN_Z;
        *field = scale_Field();
        return Vec3f(0.0f, 0.0f, fscale[2] * HANDLE_SIZE);
      case CENTER_X:
        *constraint = CONSTRAIN_X;
        *field = center_Field();
        return Vec3f(HANDLE_SIZE, 0.0f, 0.0f);
      case CENTER_Y:
        *constraint = CONSTRAIN_Y;
        *field = center_Field();
        return Vec3f(0.0f, HANDLE_SIZE, 0.0f);
      case CENTER_Z:
        *constraint = CONSTRAIN_Z;
        *field = center_Field();
        return Vec3f(0.0f, 0.0f, HANDLE_SIZE);
      case CENTER:
        *constraint = CONSTRAIN_NONE;
        *field = center_Field();
        return Vec3f(0.0f, 0.0f, 0.0f);
      default:
        *field = scale_Field();
        return Vec3f(fscale);
    }
}

void
NodeHAnimHumanoid::setHandle(int handle, const Vec3f &v)
{
    const float *fcenter = center()->getValue();
    SFRotation *sfrotation = rotation();
    const float *rot = sfrotation->getValue();
    const float *fscale = scale()->getValue();
    const float *ftranslation = translation()->getValue();
    Matrix mat;

    glPushMatrix();
    glLoadIdentity();
    if ((handle==CENTER_X) || (handle==CENTER_Y) || (handle==CENTER_Z) ||
        (handle==CENTER))
       glTranslatef(fcenter[0], fcenter[1], fcenter[2]);
    else
       glTranslatef(ftranslation[0], ftranslation[1], ftranslation[2]);
    glRotatef(RAD2DEG(rot[3] * m_scene->getUnitAngle()), 
              rot[0], rot[1], rot[2]);
    glGetFloatv(GL_MODELVIEW_MATRIX, mat);
    glPopMatrix();

    ProtoHAnimHumanoid *proto = (ProtoHAnimHumanoid *)getProto();

    switch (handle) {
      case TRANSLATION:
        {
        Vec3f vec(m_scene->constrainVec(v));
        m_scene->setField(this, proto->translation, new SFVec3f(mat * vec));
        }
        break;
      case TRANSLATION_X:
        m_scene->setField(this, proto->translation, 
              new SFVec3f(mat * (v - Vec3f(HANDLE_SIZE, 0.0f, 0.0f))));
        break;
      case TRANSLATION_Y:
        m_scene->setField(this, proto->translation, 
              new SFVec3f(mat * (v - Vec3f(0.0f, HANDLE_SIZE, 0.0f))));
        break;
      case TRANSLATION_Z:
        m_scene->setField(this, proto->translation, 
              new SFVec3f(mat * (v - Vec3f(0.0f, 0.0f, HANDLE_SIZE))));
        break;
      case ROTATION:
        m_scene->setField(this, proto->rotation, new SFRotation(v, 0));
        break;
      case ROTATION_X:
        m_scene->setField(this, proto->rotation, 
              new SFRotation(Quaternion(Vec3f(1.0f, 0.0f, 0.0f), v.x) * 
                             sfrotation->getQuat()));
        break;
      case ROTATION_Y:
        m_scene->setField(this, proto->rotation, 
              new SFRotation(Quaternion(Vec3f(0.0f, 1.0f, 0.0f), v.y) * 
                             sfrotation->getQuat()));
        break;
      case ROTATION_Z:
        m_scene->setField(this, proto->rotation, 
              new SFRotation(Quaternion(Vec3f(0.0f, 0.0f, 1.0f), v.z) * 
                             sfrotation->getQuat()));
        /*
        m_scene->setField(this, proto->rotation, 
              new SFRotation(sfrotation->getQuat() 
                             * Quaternion(Vec3f(0.0f, 0.0f, 1.0f), v.x) *
                             sfrotation->getQuat().conj()));
        */
        break;
      case SCALE_X:
        if (v.x > 0.0f) {
            m_scene->setField(this, proto->scale, 
                             new SFVec3f(Vec3f(v.x, fscale[1], fscale[2])));
        }
        break;
      case SCALE_Y:
        if (v.y > 0.0f) {
            m_scene->setField(this, proto->scale, 
                             new SFVec3f(Vec3f(fscale[0], v.y, fscale[2])));
        }
        break;
      case SCALE_Z:
        if (v.z > 0.0f) {
            m_scene->setField(this, proto->scale, 
                             new SFVec3f(Vec3f(fscale[0], fscale[1], v.z)));
        }
        break;
      case UNIFORM_SCALE_X:
        if (v.x > 0.0f) {
            float mult = v.x / fscale[0];
            Vec3f vMult(v.x, fscale[1] * mult, fscale[2] * mult);
            m_scene->setField(this, proto->scale, 
                             new SFVec3f(vMult.x, vMult.y, vMult.z));
        }
        break;
      case UNIFORM_SCALE_Y:
        if (v.y > 0.0f) {
            float mult = v.y / fscale[1];
            Vec3f vMult(fscale[0] * mult, v.y, fscale[2] * mult);
            m_scene->setField(this, proto->scale, 
                             new SFVec3f(vMult.x, vMult.y, vMult.z));
        }
        break;
      case UNIFORM_SCALE_Z:
        if (v.z > 0.0f) {
            float mult = v.z / fscale[2];
            Vec3f vMult(fscale[0] * mult, fscale[1] * mult, v.z);
            m_scene->setField(this, proto->scale, 
                             new SFVec3f(vMult.x, vMult.y, vMult.z));
        }
        break;
      case SCALE:
        if ((v.x > 0.0f) && (v.y > 0.0f) && (v.z > 0.0f)) {
            m_scene->setField(this, proto->scale, 
                             new SFVec3f(Vec3f(v.x,v.y,v.z)));
        }
        break;
      case CENTER:
        m_scene->setField(this, proto->center, new SFVec3f(mat * v));
        break;
      case CENTER_X:
        m_scene->setField(this, proto->center, 
              new SFVec3f(mat * (v - Vec3f(HANDLE_SIZE, 0.0f, 0.0f))));
        break;
      case CENTER_Y:
        m_scene->setField(this, proto->center, 
              new SFVec3f(mat * (v - Vec3f(0.0f, HANDLE_SIZE, 0.0f))));
        break;
      case CENTER_Z:
        m_scene->setField(this, proto->center, 
              new SFVec3f(mat * (v - Vec3f(0.0f, 0.0f, HANDLE_SIZE))));
        break;
    }
}

void        
NodeHAnimHumanoid::setField(int index, FieldValue *value, int cf)
{
    if (index < info_Field())
        TransformNode::setField(index, value, cf);
    else
        Node::setField(index, value, cf);
    m_hasNoWeightArrayDirty = true;
}

static bool searchDirty(Node *node, void *data)
{
    bool *dirty = (bool *)data;
    if (node->getMatrixDirty()) {
        *dirty = true;
        return false;
    }
    return true;     
}

void
NodeHAnimHumanoid::preDraw()
{
    bool matrixDirty = false;
    NodeList *childList = getBasicChildren()->getValues();

    for (long i = 0; i < childList->size(); i++) {
        if (childList->get(i)) {
            childList->get(i)->doWithBranch(searchDirty, &matrixDirty,
                                            false, false);
            if (matrixDirty)
               break;
        }
    }

    glPushMatrix();
    transform();

    for (long i = 0; i < childList->size(); i++)
        if (childList->get(i))
            childList->get(i)->preDraw();

    if (m_scene->getNumDraw() == 0)
        matrixDirty = true;

    bool meshesDirty = false;
    for (long i = 0; i < m_meshNodes.size(); i++)
        if (m_meshNodes[i]->meshDirty())
            meshesDirty = true;

    if (matrixDirty || meshesDirty) {
        createMeshes();
    }
    glPopMatrix();
}

void
NodeHAnimHumanoid::draw(int pass)
{
    int type = m_scene->getSelection()->getNode()->getType();
    if ((type == VRML_COORDINATE) || (type == VRML_COLOR) || 
                                     (type == X3D_COLOR_RGBA)) {
        glEnable(GL_ALPHA_TEST);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glAlphaFunc(GL_NOTEQUAL, 0);
    }

    NodeList *childList = getBasicChildren()->getValues();
    int n = childList->size();

    glPushMatrix();
    transform();

    for (int i = 0; i < n; i++)
        childList->get(i)->bind();

    glPushName(getBasicChildrenField());  // field
    glPushName(0);                 // index
    for (int i = 0; i < n; i++) {
        if (childList->get(i)) {
            glLoadName(i);
            childList->get(i)->draw(pass);
        }
    }
    glPopName();
    glPopName();

    for (int i = 0; i < n; i++)
        childList->get(i)->unbind();

    glPushName(info_Field());  // field
    if (m_meshes != NULL) {
        for (int i = 0; i < m_numMeshes; i++)
            if (m_meshes[i] != NULL) {
                glLoadName(info_Field());
                if (m_materials[i])
                    m_materials[i]->bind();
                if (m_textures[i])
                    m_textures[i]->bind();
                m_meshes[i]->draw(pass);
                if (m_materials[i])
                    m_materials[i]->unbind();
                if (m_textures[i])
                    m_textures[i]->unbind();
            }
    }

    glPopName();

    glPopMatrix();
}

static int numMeshes = 0;

int skinNumber = -1;
static MyArray<MeshBasedNode *> meshNodes; 
NodeHAnimHumanoid *human = NULL;

static bool searchMeshes(Node *node, void *data)
{
    if (node->getType() == VRML_MATERIAL) {
        human->m_materials[skinNumber] = (NodeMaterial *)node;
    } else if (node->getType() == VRML_IMAGE_TEXTURE) {
        human->m_textures[skinNumber] = (NodeMaterial *)node;
    } else if (node->getType() == VRML_PIXEL_TEXTURE) {
        human->m_textures[skinNumber] = (NodeMaterial *)node;
    } else if (node->isMeshBasedNode()) {
        MyMesh **mesh = (MyMesh **)data;
        mesh[numMeshes] = (MyMesh *)node->getMesh()->copy();
        mesh[numMeshes]->setColorRGBA();
        numMeshes++;
        meshNodes.append((MeshBasedNode *)node);
    }
    return true;     
}

static bool newSkinMesh = true;
static float currentColor[4] = { 0.8, 0.8, 0.8, 1 };

static bool addToMesh(Node *node, void *data)
{
    if (node->getType() == VRML_MATERIAL) {
        NodeMaterial *material = (NodeMaterial *)node;
        for (int i = 0; i < 3; i++)
            currentColor[i] = material->diffuseColor()->getValue(i);
        currentColor[3] = 1 - material->transparency()->getValue();
    } else if (node->isMeshBasedNode() && node->getMesh()) {
        MyMesh **mesh = (MyMesh **)data;
        if (newSkinMesh) {
            ((MeshBasedNode *)node)->createMesh((MyMesh *)node->getMesh()->copy());
            mesh[numMeshes] = (MyMesh *)node->getMesh()->copy();
            int numVertices = mesh[numMeshes]->getVertices()->getSFSize();
            MFColorRGBA *color = new MFColorRGBA();
            for (int i = 0; i < numVertices; i++)
                color->setSFValue(color->getSFSize(), currentColor);
            mesh[numMeshes]->setRGBAColor(color);
            numMeshes++;
        } else {
            mesh[numMeshes - 1]->addCoords(node->getMesh()->getCoordIndex(),
                                           currentColor, 
                                           node->getMesh()->getColors(),
                                           node->getMesh()->hasColorRGBA(),
                                           node->getMesh()->getNormals(),
                                           node->getMesh()->getTexCoordsSize()
                                           > 0 ? 
                                           node->getMesh()->getTexCoords(0) :
                                           NULL);
        }
        meshNodes.append((MeshBasedNode *)node);
        newSkinMesh = false;
    }
    return true;     
}

void
NodeHAnimHumanoid::update()
{
    for (long i = 0; i < m_meshNodes.size(); i++)
        m_meshNodes[i]->setMeshDirty();
     m_hasNoWeightArrayDirty = true;
}

void
NodeHAnimHumanoid::createMeshes(bool cleanDoubleVertices, bool triangulateMesh)
{
     if (m_numMeshes != 0) {
         for (int i = 0; i < m_numMeshes; i++) {
             delete m_meshes[i];
             delete m_origVertices[i];
         }
         delete [] m_meshes;
         delete [] m_origVertices;
         m_meshNodes.resize(0);
     }

     MFNode *mfSkin = skin();
     if (mfSkin == NULL)
         return;
     if (mfSkin->getSize() == 0)
         return;
     m_numMeshes = mfSkin->getSize();

     m_meshes = new MyMesh *[m_numMeshes];
     m_origVertices = new MFVec3f*[m_numMeshes];
     for (int i = 0; i < m_numMeshes; i++) {
         m_meshes[i] = NULL;
         m_origVertices[i] = NULL;
     }

     human = this;
     m_materials.resize(0);
     for (int i = 0; i < m_numMeshes; i++)
         m_materials[i] = NULL;
     m_textures.resize(0);
     for (int i = 0; i < m_numMeshes; i++)
         m_textures[i] = NULL;
     numMeshes = 0;
     meshNodes.resize(0); 

     for (int i = 0; i < m_numMeshes; i++)
         if (mfSkin->getValue(i)) {
            if (mfSkin->getValue(i)->getType() == VRML_SHAPE) {
                skinNumber = i;
                mfSkin->getValue(i)->doWithBranch(searchMeshes, m_meshes,
                                                  false, false);
            } else {
                newSkinMesh = true;
                mfSkin->getValue(i)->doWithBranch(addToMesh, m_meshes,
                                                  false, false);
            }
         }

     m_numMeshes = numMeshes;
     for (long i = 0; i < meshNodes.size(); i++)
         m_meshNodes.append(meshNodes[i]);

     for (int i = 0; i < m_numMeshes; i++) {
         if (m_meshes[i] != NULL)
             m_origVertices[i] = (MFVec3f *)m_meshes[i]->getVertices()->copy();
     }

     NodeList *childList = getBasicChildren()->getValues();
     int n = childList->size();

     for (int i = 0; i < m_numMeshes; i++)
         if (m_meshes[i] != NULL) {
             m_meshes[i]->setVerticesZero();
         }

     for (int j = 0; j < m_numMeshes; j++)
         for (int i = 0; i < n; i++)
              if (childList->get(i)->getType() == X3D_HANIM_JOINT) {
                  NodeHAnimJoint *joint = (NodeHAnimJoint *)childList->get(i);
                  if (m_meshes[j] != NULL)
                      joint->applyJoint(j, m_meshes[j], m_origVertices[j], 
                                        this);
              }

     for (int j = 0; j < m_numMeshes; j++)
         if (m_meshes[j] != NULL) {
             m_meshes[j]->generateFaceNormals();
             m_meshes[j]->smoothNormals();
         }
}

class NoWeightData {
public:
    bool noWeight;
    int vertex;
};

static bool searchNoWeight(Node *node, void *data)
{
    NoWeightData *noWeight = (NoWeightData *)data;
    if (noWeight->noWeight == false)
        return false;
    if (node->getType() == X3D_HANIM_JOINT) {
        NodeHAnimJoint *joint = (NodeHAnimJoint *)node;
        if (joint->skinCoordIndex()->find(noWeight->vertex) > -1) {
            noWeight->noWeight = false;
            return false;
        }
    }
    return true;     
}

void
NodeHAnimHumanoid::buildJointHasWeightArray(void)
{
    if (!m_hasNoWeightArrayDirty)
        return; 
    m_hasNoWeightArray.resize(0);
    for (int i = 0; i < m_numMeshes; i++) {
        MFInt32 *ci = m_meshes[i]->getCoordIndex();
        for (int j = 0; j < ci->getSize(); j++) {
            int index = ci->getValue(j);
            if (index < 0)
                continue;
            if (jointHasNoWeight(index))
                m_hasNoWeightArray.append(index);
        }  
    }
    m_hasNoWeightArrayDirty = false;
}

bool              
NodeHAnimHumanoid::jointHasNoWeight(int vertex)
{
    NoWeightData noWeight;     
    noWeight.noWeight = true;
    noWeight.vertex = vertex;

    NodeList *childList = skeleton()->getValues();

    for (long i = 0; i < childList->size(); i++)
        childList->get(i)->doWithBranch(searchNoWeight, &noWeight);

    return noWeight.noWeight;
}

int     
NodeHAnimHumanoid::countPolygons(void)
{
    createMeshes();
    int ret = 0;
    for (int i = 0; i < m_numMeshes; i++)
        ret += m_meshes[i]->countPolygons();
    return ret;
}

int     
NodeHAnimHumanoid::countPolygons1Sided(void)
{
    createMeshes();
    int ret = 0;
    for (int i = 0; i < m_numMeshes; i++)
        ret += m_meshes[i]->countPolygons1Sided();
    return ret;
}

int     
NodeHAnimHumanoid::countPolygons2Sided(void)
{
    createMeshes();
    int ret = 0;
    for (int i = 0; i < m_numMeshes; i++)
        ret += m_meshes[i]->countPolygons2Sided();
    return ret;
}

Node *
NodeHAnimHumanoid::getJointParent(Node *node)
{
    Node *ret = NULL;

    NodeList *childList = skeleton()->getValues();

    for (long i = 0; i < childList->size(); i++)
        if (childList->get(i)->getType() == X3D_HANIM_JOINT) {
            NodeHAnimJoint *joint = (NodeHAnimJoint *)childList->get(i);
            ret = joint->getJointParent(node, this);
            if (ret != NULL)
                return ret;    
        }
    return ret;    
}

int     
NodeHAnimHumanoid::writeRib(int filedes, int indent)
{
    createMeshes();
    int ret = 0;
    for (int i = 0; i < m_numMeshes; i++) {
        NodeIndexedFaceSet *faceset = (NodeIndexedFaceSet *)
                                      m_meshes[i]->toIndexedFaceSet(0, m_scene);
        RET_ONERROR( faceset->writeRib(filedes, indent) )
    }
    return ret;    
}
