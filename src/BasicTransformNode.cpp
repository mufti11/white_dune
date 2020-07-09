/*
 * BasicTransformNode.cpp
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
#include "BasicTransformNode.h"
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
#include "NodeViewpoint.h"
#include "NodeNavigationInfo.h"
#include "Field.h"
#include "ExposedField.h"

#include "RenderState.h"

void BasicTransformProto::addElements() 
{
    rotation.set(
          addExposedField(SFROTATION, "rotation", 
                          new SFRotation(0.0f, 0.0f, 1.0f, 0.0f)));
    setFieldFlags(rotation, EIF_RECOMMENDED);
    translation.set(
          addExposedField(SFVEC3F, "translation", 
                          new SFVec3f(0.0f, 0.0f, 0.0f)));
    setFieldFlags(translation, EIF_RECOMMENDED);
    bboxCenter.set(
          addField(SFVEC3F, "bboxCenter", new SFVec3f(0, 0, 0)));
    bboxSize.set(
          addField(SFVEC3F, "bboxSize", new SFVec3f(-1, -1, -1), 
                   new SFFloat(-1.0f)));
}

BasicTransformProto::BasicTransformProto(Scene *scene, const char *name)
  : WonderlandExportProto(scene, name)
{
     addElements(); 
}

BasicTransformNode::BasicTransformNode(Scene *scene, Proto *def)
  : Node(scene, def)
{
    m_matrixDirty = true;
    m_handleScale = 1.0f;
    m_matrix = Matrix::identity();
}

BasicTransformNode::~BasicTransformNode()
{
}

void
BasicTransformNode::getMatrix(float* matrix)
{
    for (int i = 0; i < 16; i++)
       matrix[i] = m_matrix[i];
}

void
BasicTransformNode::setMatrix(float* matrix)
{
    for (int i = 0; i < 16; i++)
       m_matrix[i] = matrix[i];
    Vec3f t(matrix[12], matrix[13], matrix[14]);
    translation(new SFVec3f(t.x, t.y, t.z));
}

const Quaternion &
BasicTransformNode::getQuat(void)
{
    return rotation()->getQuat();
}

void 
BasicTransformNode::setQuat(const Quaternion &quat)
{
    rotation(new SFRotation(quat));
}

void
BasicTransformNode::transform()
{
    const float *frotation = rotation()->getValue();
    const float *ftranslation = translation()->getValue();
    double angle = frotation[3];
    if (m_scene) {
        double angleUnit = m_scene->getUnitAngle();
        if (angleUnit != 0)
            angle *= angleUnit;
    }
    if (m_matrixDirty) {
        glPushMatrix();
        glLoadIdentity();
        glTranslatef(ftranslation[0], ftranslation[1], ftranslation[2]);
        glRotatef(RAD2DEG(angle), 
              frotation[0], frotation[1], frotation[2]);
        glGetFloatv(GL_MODELVIEW_MATRIX, m_matrix);
        glPopMatrix();
        m_matrixDirty = false;
    }
    glMultMatrixf((GLfloat *) m_matrix);
}

void
BasicTransformNode::transformForHandle(int handle)
{
    const float *frotation = rotation()->getValue();
    const float *ftranslation = translation()->getValue();

    double angle = frotation[3];
    if (m_scene) {
        double angleUnit = m_scene->getUnitAngle();
        if (angleUnit != 0)
            angle *= angleUnit;
    }

    glTranslatef(ftranslation[0], ftranslation[1], ftranslation[2]);
    glRotatef(RAD2DEG(angle), frotation[0], frotation[1], frotation[2]);
    return;
}

void
BasicTransformNode::preDraw()
{
    NodeList *childList = getBasicChildren()->getValues();

    glPushMatrix();
    transform();

    for (long i = 0; i < childList->size(); i++)
        childList->get(i)->preDraw();

    glPopMatrix();
}

void
BasicTransformNode::draw(int pass)
{
    int i;
    NodeList *childList = getBasicChildren()->getValues();
    int n = childList->size();

    glPushMatrix();
    transform();

    for (i = 0; i < n; i++)
        childList->get(i)->bind();

    glPushName(getBasicChildrenField());  // field
    glPushName(0);                 // index
    for (i = 0; i < n; i++) {
        glLoadName(i);
        childList->get(i)->draw(pass);
    }
    glPopName();
    glPopName();

    for (i = 0; i < n; i++)
        childList->get(i)->unbind();

    glPopMatrix();
}

void
BasicTransformNode::drawHandles(void)
{
    m_handleScale = TheApp->GetHandleScale() *
                    m_scene->getNavigationInfoNode()->speed()->getValue();

    RenderState state;
                   
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
}

void              
BasicTransformNode::drawRotationHandles(float scale) 
{ 
    RenderState state;
    state.drawRotationHandles(this, scale); 
}

Vec3f
BasicTransformNode::getHandle(int handle, int *constraint, int *field)
{
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
      default:
        *field = rotation_Field();
        *constraint = CONSTRAIN_SPHERE;
        return sfrotation->getEulerAngles(0);
    }
}

void
BasicTransformNode::setHandle(int handle, const Vec3f &v)
{
    SFRotation *sfrotation = rotation();
    const float *rot = sfrotation->getValue();
    const float *ftranslation = translation()->getValue();
    Matrix mat;

    double angle = rot[3];
    if (m_scene) {
        double angleUnit = m_scene->getUnitAngle();
        if (angleUnit != 0)
            angle *= angleUnit;
    }

    glPushMatrix();
    glLoadIdentity();
    glTranslatef(ftranslation[0], ftranslation[1], ftranslation[2]);
    glRotatef(RAD2DEG(angle), rot[0], rot[1], rot[2]);
    glGetFloatv(GL_MODELVIEW_MATRIX, mat);
    glPopMatrix();

    BasicTransformProto *proto = (BasicTransformProto *)getProto();

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
      default:
        assert(0);
        break;
    }
}

void
BasicTransformNode::receiveEvent(int eventIn, double timestamp, FieldValue *value)
{
    Node::receiveEvent(eventIn, timestamp, value);
}

void
BasicTransformNode::setField(int field, FieldValue *value, int cf)
{
    if (field != getBasicChildrenField()) 
        m_matrixDirty = true;
    Node::setField(field, value, cf);
}

void
BasicTransformNode::flip(int index)
{
    rotation()->flip(index);
    translation()->flip(index);
    bboxCenter()->flip(index);
    getBasicChildren()->flip(index);
    m_matrixDirty = true;
}

void
BasicTransformNode::swap(int fromTo)
{
    rotation()->swap(fromTo);
    translation()->swap(fromTo);
    bboxCenter()->swap(fromTo);
    bboxSize()->swap(fromTo);
    getBasicChildren()->swap(fromTo);
    m_matrixDirty = true;
}

bool
BasicTransformNode::modelViewIsIdentity(void)
{
    Matrix modelView;
    Matrix identity;

    glPushMatrix();
    glGetFloatv(GL_MODELVIEW_MATRIX, modelView);
    glLoadIdentity();
    glGetFloatv(GL_MODELVIEW_MATRIX, identity);
    glPopMatrix();
    return modelView == identity;
}

int
BasicTransformNode::writeAc3d(int f, int indent)
{ 
    NodeList *childList = getBasicChildren()->getValues();
    bool matrixIdentity = modelViewIsIdentity();
    if (!matrixIdentity) {
        glPushMatrix();
        transform();
    }
    int kids = 0;
    long i;
    for (i = 0; i < childList->size(); i++)
        if (childList->get(i)->canWriteAc3d())
            kids++;

    if (kids != 0) {

        RET_ONERROR( mywritestr(f, "OBJECT group\n") )

        if (hasName())
            RET_ONERROR( mywritef(f, "name \"%s\"\n", (const char *) 
                                                      getName()) )

        if (matrixIdentity && (rotation()->getValue(3) != 0.0f)) {
            RET_ONERROR( mywritestr(f, "rot ") )
            RET_ONERROR( rotation()->writeAc3d(f, indent) )
        }

        if (matrixIdentity) {
            RET_ONERROR( mywritestr(f, "loc ") )
            RET_ONERROR( translation()->writeAc3d(f, indent) )
        }

        RET_ONERROR( mywritef(f, "kids %d\n", kids) )        

        for (i = 0; i < childList->size(); i++)
            childList->get(i)->writeAc3d(f, indent);
    }

    if (!matrixIdentity)
        glPopMatrix();
    return 0; 
}

int
BasicTransformNode::writeRib(int f, int indent)
{ 
    NodeList *childList = getBasicChildren()->getValues();
    bool matrixIdentity = modelViewIsIdentity();
    if (!matrixIdentity) {
        glPushMatrix();
        transform();
    }

    for (long i = 0; i < childList->size(); i++)
        childList->get(i)->writeRib(f, indent);

    if (!matrixIdentity)
        glPopMatrix();
    return 0; 
}

int
BasicTransformNode::writePovray(int f, int indent)
{ 
    NodeList *childList = getBasicChildren()->getValues();
    bool matrixIdentity = modelViewIsIdentity();
    if (!matrixIdentity) {
        glPushMatrix();
        transform();
    }

    for (long i = 0; i < childList->size(); i++)
        childList->get(i)->writePovray(f, indent);

    if (!matrixIdentity)
        glPopMatrix();
    return 0; 
}

int
BasicTransformNode::writeCattGeo(int filedes, int indent)
{ 
    glPushMatrix();
    transform();
    int ret = getBasicChildren()->writeCattGeo(this, filedes, indent);
    glPopMatrix();
    return ret;
}

int
BasicTransformNode::writeLdrawDat(int filedes, int indent)
{ 
    glPushMatrix();
    transform();
    int ret = getBasicChildren()->writeLdrawDat(filedes, indent);
    glPopMatrix();
    return ret;
}

