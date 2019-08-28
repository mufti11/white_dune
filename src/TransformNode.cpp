/*
 * TransformNode.cpp
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
#include <sys/types.h>
#include <sys/stat.h>
#ifndef _WIN32
# include <fcntl.h>
#endif

#include "swt.h"
#include "TransformNode.h"
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

void TransformProto::addElements() 
{
    center.set( 
           addExposedField(SFVEC3F, "center", new SFVec3f(0.0f, 0.0f, 0.0f)));
    if (TheApp->getCoverMode()) {
        // non standard Covise/COVER extension: scale can be negative
       scale.set(
          addExposedField(SFVEC3F, "scale", new SFVec3f(1.0f, 1.0f, 1.0f))); 
    } else
       scale.set(
          addExposedField(SFVEC3F, "scale", new SFVec3f(1.0f, 1.0f, 1.0f), 
                          new SFFloat(0.0f)));
    scaleOrientation.set(
          addExposedField(SFROTATION, "scaleOrientation",
                          new SFRotation(0.0f, 0.0f, 1.0f, 0.0f)));

    render.set(
        addExposedField(SFBOOL, "render", new SFBool(true)));
    setFieldFlags(render, FF_X3DOM_ONLY);
}

TransformProto::TransformProto(Scene *scene, const char *name) 
  : BasicTransformProto(scene, name)
{
     addElements(); 
}

TransformNode::TransformNode(Scene *scene, Proto *def)
  : BasicTransformNode(scene, def)
{
}

TransformNode::~TransformNode()
{
}

const Vec3f &
TransformNode::getCenter(void)
{
    static Vec3f v;
    v = center()->getValue();
    return v;
}

const Vec3f &
TransformNode::getScale(void)
{
    static Vec3f v;
    v = scale()->getValue();
    return v;
}

void
TransformNode::transform()
{
    const float *fcenter = center()->getValue();
    const float *frotation = rotation()->getValue();
    const float *fscale = scale()->getValue();
    const float *fscaleOrientation = scaleOrientation()->getValue();
    const float *ftranslation = translation()->getValue();

    if (m_matrixDirty) {
        double rotAngle = frotation[3];
        double oriAngle = fscaleOrientation[3];
        if (m_scene) {
            double angleUnit = m_scene->getUnitAngle();
            if (angleUnit != 0) {
               rotAngle *= angleUnit;
               oriAngle *= angleUnit;
            }
        }
        glPushMatrix();
        glLoadIdentity();
        glTranslatef(ftranslation[0], ftranslation[1], ftranslation[2]);
        glTranslatef(fcenter[0], fcenter[1], fcenter[2]);
        glRotatef(RAD2DEG(rotAngle), 
              frotation[0], frotation[1], frotation[2]);
        glRotatef(RAD2DEG(oriAngle), 
              fscaleOrientation[0], fscaleOrientation[1], fscaleOrientation[2]);
        glScalef(fscale[0], fscale[1], fscale[2]);
        glRotatef(-RAD2DEG(oriAngle), 
              fscaleOrientation[0], fscaleOrientation[1], fscaleOrientation[2]);
        glTranslatef(-fcenter[0], -fcenter[1], -fcenter[2]);
        glGetFloatv(GL_MODELVIEW_MATRIX, m_matrix);
        glPopMatrix();
        m_matrixDirty = false;
    }
    glMultMatrixf((GLfloat *) m_matrix);
}

void
TransformNode::getInvertMatrix(float* matrix)
{
    const float *fcenter = center()->getValue();
    const float *frotation = rotation()->getValue();
    const float *fscale = scale()->getValue();
    const float *fscaleOrientation = scaleOrientation()->getValue();
    const float *ftranslation = translation()->getValue();

    double rotAngle = frotation[3];
    double oriAngle = fscaleOrientation[3];
    if (m_scene) {
        double angleUnit = m_scene->getUnitAngle();
        if (angleUnit != 0) {
            rotAngle *= angleUnit;
            oriAngle *= angleUnit;
        }
    }
    glPushMatrix();
    glLoadIdentity();
    glTranslatef(fcenter[0], fcenter[1], fcenter[2]);
    glRotatef(RAD2DEG(oriAngle), 
              fscaleOrientation[0], fscaleOrientation[1], fscaleOrientation[2]);
    float sx = fscale[0];
    if (sx != 0)
        sx = 1.0f / sx; 
    float sy = fscale[1];
    if (sy != 0)
        sy = 1.0f / sy; 
    float sz = fscale[2];
    if (sz != 0)
        sz = 1.0f / sz; 
    glScalef(sx, sy, sz);
    glRotatef(-RAD2DEG(oriAngle), 
              fscaleOrientation[0], fscaleOrientation[1], fscaleOrientation[2]);

    glRotatef(-RAD2DEG(rotAngle), frotation[0], frotation[1], frotation[2]);
    glTranslatef(-fcenter[0], -fcenter[1], -fcenter[2]);
    glTranslatef(-ftranslation[0], -ftranslation[1], -ftranslation[2]);
    glGetFloatv(GL_MODELVIEW_MATRIX, m_matrix);
    glPopMatrix();
    glMultMatrixf((GLfloat *) m_matrix);
    for (int i = 0; i < 16; i++)
       matrix[i] = m_matrix[i];    
    m_matrixDirty = true;
}

void
TransformNode::transformForHandle(int handle)
{
    const float *fcenter = center()->getValue();
    const float *frotation = rotation()->getValue();
    const float *fscaleOrientation = scaleOrientation()->getValue();
    const float *ftranslation = translation()->getValue();

    double rotAngle = frotation[3];
    double oriAngle = fscaleOrientation[3];
    if (m_scene) {
        double angleUnit = m_scene->getUnitAngle();
        if (angleUnit != 0) {
            rotAngle *= angleUnit;
            oriAngle *= angleUnit;
        }
    }

    if (handle != HUMAN_JOINT)
        glTranslatef(ftranslation[0], ftranslation[1], ftranslation[2]);
    glTranslatef(fcenter[0], fcenter[1], fcenter[2]);
    if (handle != HUMAN_JOINT)
        glRotatef(RAD2DEG(rotAngle), frotation[0], frotation[1], frotation[2]);
    if (handle == SCALE_X || handle == SCALE_Y || handle == SCALE_Z || 
        handle == SCALE) {
        glRotatef(RAD2DEG(oriAngle), 
              fscaleOrientation[0], fscaleOrientation[1], fscaleOrientation[2]);
    }

    return;
}

void
TransformNode::preDraw()
{
    NodeList *childList = getChildren()->getValues();

    glPushMatrix();
    transform();

    for (size_t i = 0; i < childList->size(); i++)
        if (childList->get(i) != this)
            childList->get(i)->preDraw();

    glPopMatrix();
}

void
TransformNode::draw(int pass)
{
    int i;
    NodeList *childList = getChildren()->getValues();
    int n = childList->size();

    glPushMatrix();
    transform();

    for (i = 0; i < n; i++)
        childList->get(i)->bind();

    glPushName(getChildrenField());  // field
    glPushName(0);                 // index
    for (i = 0; i < n; i++) {
        glLoadName(i);
        if (childList->get(i) != this)
            childList->get(i)->draw(pass);
    }
    glPopName();
    glPopName();

    for (i = 0; i < n; i++)
        childList->get(i)->unbind();

    glPopMatrix();
}

void
TransformNode::drawHandles(void)
{
    if (m_scene->getVertexModifier())
        return;

    m_handleScale = TheApp->GetHandleScale();

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
    else if (tm->tmode==TM_SCALE)
       state.drawScaleHandles(this, m_handleScale, scale()->getValue());
    else if (tm->tmode==TM_UNIFORM_SCALE)
       state.drawUniformScaleHandles(this, m_handleScale, scale()->getValue());
    else if (tm->tmode==TM_CENTER)
       state.drawCenterHandles(this, m_handleScale);
}

void
TransformNode::drawRotationHandles(float scale)
{
    RenderState state;

    state.drawRotationHandles(this, scale);
}

Vec3f
TransformNode::getHandle(int handle, int *constraint, int *field)
{
    const float *fscale = scale()->getValue();
    SFRotation *sfrotation = rotation();
    sfrotation->fixAngle(m_scene->getUnitAngle());

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
      case HUMAN_JOINT:
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
TransformNode::setHandle(int handle, const Vec3f &v)
{
    const float *fcenter = center()->getValue();
    SFRotation *sfrotation = rotation();
    const float *rot = sfrotation->getValue();
    const float *fscale = scale()->getValue();
    const float *ftranslation = translation()->getValue();
    Matrix mat;

    double rotAngle = rot[3];
    if (m_scene) {
        double angleUnit = m_scene->getUnitAngle();
        if (angleUnit != 0) {
            rotAngle *= angleUnit;
        }
    }

    glPushMatrix();
    glLoadIdentity();
    if ((handle==CENTER_X) || (handle==CENTER_Y) || (handle==CENTER_Z) ||
        (handle==CENTER))
       glTranslatef(fcenter[0], fcenter[1], fcenter[2]);
    else
       glTranslatef(ftranslation[0], ftranslation[1], ftranslation[2]);
    glRotatef(RAD2DEG(rotAngle), rot[0], rot[1], rot[2]);
    glGetFloatv(GL_MODELVIEW_MATRIX, mat);
    glPopMatrix();

    TransformProto *proto = (TransformProto *)getProto();

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
      case HUMAN_JOINT:
        {
        SFRotation rot2(v, 0);
        rot2.reverseFixAngle(m_scene->getUnitAngle());
        m_scene->setField(this, proto->rotation, new SFRotation(rot2));
        }
        break;
      case ROTATION_X:
        {
        SFRotation rot2(Quaternion(Vec3f(1.0f, 0.0f, 0.0f), v.x) * 
                                   sfrotation->getQuat());
        rot2.reverseFixAngle(m_scene->getUnitAngle());
        m_scene->setField(this, proto->rotation, new SFRotation(rot2));
        }
        break;
      case ROTATION_Y:
        {
        SFRotation rot2(Quaternion(Vec3f(0.0f, 1.0f, 0.0f), v.y) * 
                                   sfrotation->getQuat());
        rot2.reverseFixAngle(m_scene->getUnitAngle());
        m_scene->setField(this, proto->rotation, new SFRotation(rot2));
        }
        break;
      case ROTATION_Z:
        {
        SFRotation rot2(Quaternion(Vec3f(0.0f, 0.0f, 1.0f), v.z) * 
                                   sfrotation->getQuat());
        rot2.reverseFixAngle(m_scene->getUnitAngle());
        m_scene->setField(this, proto->rotation, new SFRotation(rot2));
        }
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
      default:
        assert(0);
        break;
    }
}

void
TransformNode::receiveEvent(int eventIn, double timestamp, FieldValue *value)
{
    Node::receiveEvent(eventIn, timestamp, value);
}

void
TransformNode::setField(int field, FieldValue *value, int cf)
{
    if (field != getChildrenField()) 
        m_matrixDirty = true;
    if (field < center_Field())
        BasicTransformNode::setField(field, value, cf);
    else
        Node::setField(field, value, cf);
}

void
TransformNode::flip(int index)
{
    center()->flip(index);
    rotation()->flip(index);
    scaleOrientation()->flip(index);
    translation()->flip(index);
    bboxCenter()->flip(index);
    getChildren()->flip(index);
    m_matrixDirty = true;
}

void
TransformNode::swap(int fromTo)
{
    center()->swap(fromTo);
    rotation()->swap(fromTo);
    scale()->swap(fromTo);
    scaleOrientation()->swap(fromTo);
    translation()->swap(fromTo);
    bboxCenter()->swap(fromTo);
    bboxSize()->swap(fromTo);
    getChildren()->swap(fromTo);
    m_matrixDirty = true;
}

bool
TransformNode::modelViewIsIdentity(void)
{
    Matrix modelView;
    Matrix identity;

    for (int i = 0; i < 3; i++)
        if (scale()->getValue(i) != 0.0f)
            return false;
    if (scaleOrientation()->getValue(3) != 0.0f)
        return false;

    return BasicTransformNode::modelViewIsIdentity();
}

int
TransformNode::writeAc3d(int f, int indent)
{ 
    NodeList *childList = getChildren()->getValues();
    bool matrixIdentity = modelViewIsIdentity();
    if (!matrixIdentity) {
        glPushMatrix();
        transform();
    }
    int kids = 0;
    size_t i;
    for (i = 0; i < childList->size(); i++)
        if (childList->get(i)->canWriteAc3d())
            kids++;

    if (kids != 0) {

        RET_ONERROR( mywritestr(f, "OBJECT group\n") )

        if (hasName())
            RET_ONERROR( mywritef(f, "name \"%s\"\n", (const char *)getName()) )

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
TransformNode::writeCattGeo(int filedes, int indent)
{ 
    glPushMatrix();
    transform();
    int ret = getChildren()->writeCattGeo(this, filedes, indent);
    glPopMatrix();
    return ret;
}

int
TransformNode::writeLdrawDat(int filedes, int indent)
{ 
    glPushMatrix();
    transform();
    int ret = getChildren()->writeLdrawDat(filedes, indent);
    glPopMatrix();
    return ret;
}

