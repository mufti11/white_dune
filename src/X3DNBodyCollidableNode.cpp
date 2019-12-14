/*
 * X3DNBodyCollidableNode.cpp
 *
 * Copyright (C) 1999 Stephen F. White, 2007 J. "MUFTI" Scheurich
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

#include "X3DNBodyCollidableNode.h"
#include "Util.h"
#include "Proto.h"
#include "ExposedField.h"
#include "Field.h"
#include "DuneApp.h"
#include "Scene.h"
#include "RenderState.h"
#include "NodeNavigationInfo.h"
#include "NodeRigidBody.h"

X3DNBodyCollidableProto::X3DNBodyCollidableProto(Scene *scene, const char *name)
  : Proto(scene, name)
{
    enabled.set(addExposedField(SFBOOL, "enabled", new SFBool(true)));
    rotation.set(addExposedField(SFROTATION, "rotation", 
                                 new SFRotation(0.0f, 0.0f, 1.0f, 0.0f)));
    translation.set(addExposedField(SFVEC3F, "translation", 
                                    new SFVec3f(0.0f, 0.0f, 0.0f)));
    bboxCenter.set(addField(SFVEC3F, "bboxCenter", new SFVec3f(0, 0, 0)));
    bboxSize.set(addField(SFVEC3F, "bboxSize", new SFVec3f(-1, -1, -1), 
                          new SFFloat(-1.0f)));

}

X3DNBodyCollidableNode::X3DNBodyCollidableNode(Scene *scene, Proto *def)
  : RigidBodyPhysicsNode(scene, def)
{
    m_matrixDirty = true;
    m_handleScale = 1.0f;
}

void
X3DNBodyCollidableNode::transform()
{
    const float *frotation = rotation()->getValue();
    const float *ftranslation = translation()->getValue();

    if (m_matrixDirty) {
        glPushMatrix();
        glLoadIdentity();
        glTranslatef(ftranslation[0], ftranslation[1], ftranslation[2]);
        glRotatef(RAD2DEG(frotation[3] * m_scene->getUnitAngle()), 
              frotation[0], frotation[1], frotation[2]);
        glGetDoublev(GL_MODELVIEW_MATRIX, m_matrix);
        glPopMatrix();
        m_matrixDirty = false;
    }
    glMultMatrixd((GLdouble *) m_matrix);
}

void
X3DNBodyCollidableNode::transformForHandle(int handle)
{
    const float *frotation = rotation()->getValue();
    const float *ftranslation = translation()->getValue();

    glTranslatef(ftranslation[0], ftranslation[1], ftranslation[2]);
    glRotatef(RAD2DEG(frotation[3] * m_scene->getUnitAngle()), 
              frotation[0], frotation[1], frotation[2]);
    return;
}

void
X3DNBodyCollidableNode::drawHandles(void)
{
    RenderState renderstate;

    m_handleScale = TheApp->GetHandleScale() *
                   m_scene->getNavigationInfoNode()->speed()->getValue();
                   
    TransformMode* tm=m_scene->getTransformMode();
    if (tm->tmode==TM_TRANSLATE)
       renderstate.drawTranslationHandles(this, m_handleScale);
    else if (tm->tmode==TM_6D)
       renderstate.draw6DHandles(this, m_handleScale);
    else if (tm->tmode==TM_6DLOCAL)
       renderstate.draw6DlocalHandles(this, m_handleScale);
    else if (tm->tmode==TM_ROCKET)
       renderstate.drawRocketHandles(this, m_handleScale);
    else if (tm->tmode==TM_HOVER)
       renderstate.drawHoverHandles(this, m_handleScale);
    else if (tm->tmode==TM_ROTATE)
       renderstate.drawRotationHandles(this, m_handleScale);
}

Vec3f
X3DNBodyCollidableNode::getHandle(int handle, int *constraint, int *field)
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
    }
    *field = getProto()->metadata_Field();
    return Vec3f(0.0f, 0.0f, 0.0f);
}

void
X3DNBodyCollidableNode::setHandle(int handle, const Vec3f &v)
{
    SFRotation *sfrotation = rotation();
    const float *rot = sfrotation->getValue();
    const float *ftranslation = translation()->getValue();
    Matrix mat;

    glPushMatrix();
    glLoadIdentity();
    glTranslatef(ftranslation[0], ftranslation[1], ftranslation[2]);
    glRotatef(RAD2DEG(rot[3]), rot[0], rot[1], rot[2]);
    glGetFloatv(GL_MODELVIEW_MATRIX, mat);
    glPopMatrix();

    switch (handle) {
      case TRANSLATION:
        {
        Vec3f vec(m_scene->constrainVec(v));
        m_scene->setField(this, translation_Field(), new SFVec3f(mat * vec));
        }
        break;
      case TRANSLATION_X:
        m_scene->setField(this, translation_Field(), 
              new SFVec3f(mat * (v - Vec3f(HANDLE_SIZE, 0.0f, 0.0f))));
        break;
      case TRANSLATION_Y:
        m_scene->setField(this, translation_Field(), 
              new SFVec3f(mat * (v - Vec3f(0.0f, HANDLE_SIZE, 0.0f))));
        break;
      case TRANSLATION_Z:
        m_scene->setField(this, translation_Field(), 
              new SFVec3f(mat * (v - Vec3f(0.0f, 0.0f, HANDLE_SIZE))));
        break;
      case ROTATION:
        m_scene->setField(this, rotation_Field(), new SFRotation(v, 0));
        break;
      case ROTATION_X:
        m_scene->setField(this, rotation_Field(), 
              new SFRotation(Quaternion(Vec3f(1.0f, 0.0f, 0.0f), v.x) * 
                             sfrotation->getQuat()));
        break;
      case ROTATION_Y:
        m_scene->setField(this, rotation_Field(), 
              new SFRotation(Quaternion(Vec3f(0.0f, 1.0f, 0.0f), v.y) * 
                             sfrotation->getQuat()));
        break;
      case ROTATION_Z:
        m_scene->setField(this, rotation_Field(), 
              new SFRotation(Quaternion(Vec3f(0.0f, 0.0f, 1.0f), v.z) * 
                             sfrotation->getQuat()));
        /*
        m_scene->setField(this, rotation_Field(), 
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
X3DNBodyCollidableNode::receiveEvent(int eventIn, double timestamp, FieldValue *value)
{
    switch (eventIn) {
    case 0:
        // FIXME:  do addChildren here
        break;
    case 1:
        // FIXME:  do removeChildren here
        break;
    default:
        Node::receiveEvent(eventIn, timestamp, value);
        break;
    }
}

void
X3DNBodyCollidableNode::setField(int field, FieldValue *value, int cf)
{
    m_matrixDirty = true;
    Node::setField(field, value, cf);
}

bool
X3DNBodyCollidableNode::modelViewIsIdentity(void)
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


