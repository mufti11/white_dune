/*
 * NodeRigidBody.cpp
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

#include "NodeRigidBody.h"
#include "Proto.h"
#include "FieldValue.h"
#include "MFString.h"
#include "SFString.h"
#include "DuneApp.h"
#include "RenderState.h"
#include "Scene.h"
#include "NodeNavigationInfo.h"
#include "Util.h"

ProtoRigidBody::ProtoRigidBody(Scene *scene)
  : Proto(scene, "RigidBody")
{
    angularDampingFactor.set(addExposedField(SFFLOAT, "angularDampingFactor", 
                                             new SFFloat(0.001), 
                                             new SFFloat(0), new SFFloat(1)));
    angularVelocity.set(addExposedField(SFVEC3F, "angularVelocity", 
                                        new SFVec3f()));
    autoDamp.set(addExposedField(SFBOOL, "autoDamp", new SFBool(false)));
    autoDisable.set(addExposedField(SFBOOL, "autoDisable", new SFBool(false)));
    centerOfMass.set(addExposedField(SFVEC3F, "centerOfMass", 
                                     new SFVec3f()));
    disableAngularSpeed.set(addExposedField(SFFLOAT, "disableAngularSpeed", 
                                            new SFFloat(0), new SFFloat(0))); 
    disableLinearSpeed.set(addExposedField(SFFLOAT, "disableLinearSpeed", 
                                           new SFFloat(0), new SFFloat(0))); 
    disableTime.set(addExposedField(SFFLOAT, "disableTime", 
                                    new SFFloat(0), new SFFloat(0))); 
    enabled.set(addExposedField(SFBOOL, "enabled", new SFBool(true)));
    finiteRotationAxis.set(addExposedField(SFVEC3F, "finiteRotationAxis", 
                                        new SFVec3f()));
    fixed.set(addExposedField(SFBOOL, "fixed", new SFBool(false)));
    forces.set(addExposedField(MFVEC3F, "forces", new MFVec3f()));
    setFieldFlags(forces, EIF_RECOMMENDED);
    geometry.set(addExposedField(MFNODE, "geometry", new MFNode(),
                                 BODY_COLLIDABLE_NODE));
    inertia.set(addExposedField(SFMATRIX3F, "inertia", new SFMatrix3f()));
    
    linearDampingFactor.set(addExposedField(SFFLOAT, "linearDampingFactor", 
                                            new SFFloat(0.001), 
                                            new SFFloat(0), new SFFloat(1)));
    linearVelocity.set(addExposedField(SFVEC3F, "linearVelocity", 
                                       new SFVec3f()));
    mass.set(addExposedField(SFFLOAT, "mass", new SFFloat(0), new SFFloat(0))); 
    massDensityModel.set(addExposedField(SFNODE, "massDensityModel",
         new SFNode(NULL), PRIMITIVE_GEOMETRY_OR_MASS_DENSITY_MODEL_NODE));
    orientation.set(addExposedField(SFROTATION, "orientation", 
                                    new SFRotation(0, 0, 1, 0)));
    position.set(addExposedField(SFVEC3F, "position", new SFVec3f()));
    torques.set(addExposedField(MFVEC3F, "torques", new MFVec3f()));
    setFieldFlags(torques, EIF_RECOMMENDED);
    useFiniteRotation.set(addExposedField(SFBOOL, "useFiniteRotation", 
                          new SFBool(false)));
    useGlobalGravity.set(addExposedField(SFBOOL, "useGlobalGravity", 
                         new SFBool(true)));
}

Node *
ProtoRigidBody::create(Scene *scene)
{ 
    return new NodeRigidBody(scene, this); 
}

NodeRigidBody::NodeRigidBody(Scene *scene, Proto *def)
  : RigidBodyPhysicsNode(scene, def)
{
    m_matrixDirty = true;
    int lastLoadName = CENTER;
    lastLoadName++;
    m_handleNameLinearVelocity = lastLoadName++;
    m_handleNameAngularVelocityX1 = lastLoadName++;
    m_handleNameAngularVelocityY1 = lastLoadName++;
    m_handleNameAngularVelocityZ1 = lastLoadName++;
    m_handleNameAngularVelocityX2 = lastLoadName++;
    m_handleNameAngularVelocityY2 = lastLoadName++;
    m_handleNameAngularVelocityZ2 = lastLoadName++;
}

void
NodeRigidBody::transform()
{
    const float *frotation = orientation()->getValue();
    const float *ftranslation  = position()->getValue();

    if (m_matrixDirty) {
        glPushMatrix();
        glLoadIdentity();
        glTranslatef(ftranslation[0], ftranslation[1], ftranslation[2]);
        glRotatef(RAD2DEG(frotation[3]), 
                  frotation[0], frotation[1], frotation[2]);
        glGetFloatv(GL_MODELVIEW_MATRIX, m_matrix);
        glPopMatrix();
        m_matrixDirty = false;
    }
    glMultMatrixf((GLfloat *) m_matrix);
}

void
NodeRigidBody::transformForHandle(int handle)
{
    const float *frotation = orientation()->getValue();
    const float *ftranslation = position()->getValue();

    glTranslatef(ftranslation[0], ftranslation[1], ftranslation[2]);
    glRotatef(RAD2DEG(frotation[3]), frotation[0], frotation[1], frotation[2]);
    return;
}

void
NodeRigidBody::preDraw()
{
    NodeList *childList = geometry()->getValues();

    glPushMatrix();
    transform();

    for (int i = 0; i < childList->size(); i++)
        childList->get(i)->preDraw();

    glPopMatrix();
}

void
NodeRigidBody::draw(int pass)
{
    NodeList *childList = geometry()->getValues();
    int n = childList->size();

    glPushMatrix();
    transform();

    for (int i = 0; i < n; i++)
        childList->get(i)->bind();

    glPushName(geometry_Field());  // field
    glPushName(0);                 // index
    for (int i = 0; i < n; i++) {
        glLoadName(i);
        childList->get(i)->draw(pass);
    }
    glPopName();
    glPopName();

    for (int i = 0; i < n; i++)
        childList->get(i)->unbind();

    glPopMatrix();
}

void
NodeRigidBody::drawVelocityHandles(float handleScale)
{
    RenderState state;

    const float *ftranslation  = position()->getValue();

    glPushMatrix();
    glPushMatrix();
    glLoadIdentity();
    glTranslatef(ftranslation[0], ftranslation[1], ftranslation[2]);
    glGetFloatv(GL_MODELVIEW_MATRIX, m_matrix);
    m_matrixDirty = true;
    glPopMatrix();
    glMultMatrixf((GLfloat *) m_matrix);

    // draw initial velocity
    const float *flinearVelocity = linearVelocity()->getValue();
 
    glPushName(NO_HANDLE);
    glPushName(0);                 
    glDisable(GL_LIGHTING);
    Util::myGlColor3f(1.0f, 0.5f, 0.0f);
    glBegin(GL_LINE_STRIP);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(flinearVelocity[0], flinearVelocity[1], flinearVelocity[2]);
    glEnd();

    state.startDrawHandles();
    glLoadName(m_handleNameLinearVelocity);
    state.drawHandle(Vec3f(flinearVelocity));
    state.endDrawHandles();

    // draw angular velocity

    const float *fangularVelocity = angularVelocity()->getValue();

    float axisPoint = handleScale * 0.4;

    // draw lines first

    glLoadName(NO_HANDLE);
 
    // x
    if (fangularVelocity[0] != 0) {
        Util::myGlColor3f(1.0f, 0.0f, 0.0f);

        glBegin(GL_LINE_STRIP);
        glVertex3f(0.0f, axisPoint, 0.0f);
        glVertex3f(0.0f, axisPoint, fangularVelocity[0]);
        glEnd();

        glBegin(GL_LINE_STRIP);
        glVertex3f(0.0f,                 0.0f, axisPoint);
        glVertex3f(0.0f, -fangularVelocity[0], axisPoint);
        glEnd();
    }

    // y
    if (fangularVelocity[1] != 0) {
        Util::myGlColor3f(0.0f, 1.0f, 0.0f);

        glBegin(GL_LINE_STRIP);
        glVertex3f(               0.0f, 0.0f, axisPoint);
        glVertex3f(fangularVelocity[1], 0.0f, axisPoint);
        glEnd();

        glBegin(GL_LINE_STRIP);
        glVertex3f(axisPoint, 0.0f, 0.0f);
        glVertex3f(axisPoint, 0.0f, -fangularVelocity[1]);
        glEnd();
    }

    // z
    if (fangularVelocity[2] != 0) {
        Util::myGlColor3f(0.0f, 0.0f, 1.0f);

        glBegin(GL_LINE_STRIP);
        glVertex3f(axisPoint,                0.0f, 0.0f);
        glVertex3f(axisPoint, fangularVelocity[2], 0.0f);
        glEnd();

        glBegin(GL_LINE_STRIP);
        glVertex3f(                0.0f, axisPoint, 0.0f);
        glVertex3f(-fangularVelocity[2], axisPoint, 0.0f);
        glEnd();
    }
    

    // draw handles later

    state.startDrawHandles();

    // x
    Util::myGlColor3f(1.0f, 0.0f, 0.0f);
    glLoadName(m_handleNameAngularVelocityX1);
    state.drawHandle(Vec3f(0.0f, axisPoint, fangularVelocity[0]));
    if (fangularVelocity[0] != 0) {
        glLoadName(m_handleNameAngularVelocityX2);
        state.drawHandle(Vec3f(0.0f, -fangularVelocity[0], axisPoint));
    }

    // y
    Util::myGlColor3f(0.0f, 1.0f, 0.0f);
    glLoadName(m_handleNameAngularVelocityY1);
    state.drawHandle(Vec3f(fangularVelocity[1], 0.0f, axisPoint));
    if (fangularVelocity[1] != 0) {
        glLoadName(m_handleNameAngularVelocityY2);
        state.drawHandle(Vec3f(axisPoint, 0.0f, -fangularVelocity[1]));
    }

    // z
    Util::myGlColor3f(0.0f, 0.0f, 1.0f);
    glLoadName(m_handleNameAngularVelocityZ1);
    state.drawHandle(Vec3f(axisPoint, fangularVelocity[2], 0.0f));

    if (fangularVelocity[2] != 0) {
        glLoadName(m_handleNameAngularVelocityZ2);
        state.drawHandle(Vec3f(-fangularVelocity[2], axisPoint, 0.0f));
    }

    state.endDrawHandles();

    glPopName();
    glPopName();
    glEnable(GL_LIGHTING);
    glPopMatrix();    
}

void
NodeRigidBody::drawHandles()
{
    RenderState state;
    state.setAxesStart(0.5);

    m_handleScale = TheApp->GetHandleScale() *
                    m_scene->getNavigationInfoNode()->speed()->getValue();
                   
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
    drawVelocityHandles(m_handleScale);
}

Vec3f
NodeRigidBody::getHandle(int handle, int *constraint, int *field)
{
    SFRotation *sfrotation = orientation();

    switch (handle) {
      case NO_HANDLE:
        *constraint = CONSTRAIN_NONE;
        *field = getProto()->metadata_Field();        
        return Vec3f(0.0f, 0.0f, 0.0f);        
      case TRANSLATION_X:
        *constraint = CONSTRAIN_X;
        *field = position_Field();
        return Vec3f(HANDLE_SIZE, 0.0f, 0.0f);
      case TRANSLATION_Y:
        *constraint = CONSTRAIN_Y;
        *field = position_Field();
        return Vec3f(0.0f, HANDLE_SIZE, 0.0f);
      case TRANSLATION_Z:
        *constraint = CONSTRAIN_Z;
        *field = position_Field();
        return Vec3f(0.0f, 0.0f, HANDLE_SIZE);
      case TRANSLATION:
        *field = position_Field();
        return Vec3f(0.0f, 0.0f, 0.0f);
      case ROTATION:
        *field = orientation_Field();
        *constraint = CONSTRAIN_SPHERE;
        return sfrotation->getEulerAngles(0);
      case ROTATION_X:
        *field = orientation_Field();
        *constraint = CONSTRAIN_YZ;
        return Vec3f(sfrotation->getEulerAngles(0).x, 0.0f, 0.0f);
      case ROTATION_Y:
        *field = orientation_Field();
        *constraint = CONSTRAIN_ZX;
        return Vec3f(0.0f, sfrotation->getEulerAngles(0).y, 0.0f);
      case ROTATION_Z:
        *field = orientation_Field();
        *constraint = CONSTRAIN_XY;
        return Vec3f(0.0f, 0.0f, sfrotation->getEulerAngles(0).z);
      default:
        if (handle == m_handleNameLinearVelocity) {
            *constraint = m_scene->getConstrain();
            *field = linearVelocity_Field();
            return Vec3f(linearVelocity()->getValue());
        } else if (handle == m_handleNameAngularVelocityX1) {
            const float *fa = angularVelocity()->getValue();
            *constraint = CONSTRAIN_Z;
            *field = angularVelocity_Field();
            return Vec3f(fa[2], fa[1], fa[0]);
        } else if (handle == m_handleNameAngularVelocityX2) {
            const float *fa = angularVelocity()->getValue();
            *constraint = CONSTRAIN_Y;
            *field = angularVelocity_Field();
            return Vec3f(fa[1], -fa[0], fa[2]);
        } else if (handle == m_handleNameAngularVelocityY1) {
            const float *fa = angularVelocity()->getValue();
            *constraint = CONSTRAIN_X;
            *field = angularVelocity_Field();
            return Vec3f(fa[1], fa[0], fa[2]);
        } else if (handle == m_handleNameAngularVelocityY2) {
            const float *fa = angularVelocity()->getValue();
            *constraint = CONSTRAIN_Z;
            *field = angularVelocity_Field();
            return Vec3f(fa[0], fa[2], -fa[1]);
        } else if (handle == m_handleNameAngularVelocityZ1) {
            const float *fa = angularVelocity()->getValue();
            *constraint = CONSTRAIN_Y;
            *field = angularVelocity_Field();
            return Vec3f(fa[0], fa[2], fa[1]);
        } else if (handle == m_handleNameAngularVelocityZ2) {
            const float *fa = angularVelocity()->getValue();
            *constraint = CONSTRAIN_X;
            *field = angularVelocity_Field();
            return Vec3f(-fa[2], fa[1], fa[0]);
        }
    }
    *field = getProto()->metadata_Field();
    return Vec3f(0.0f, 0.0f, 0.0f);
}

void
NodeRigidBody::setField(int field, FieldValue *value, int cf)
{
    m_matrixDirty = true;
    Node::setField(field, value, cf);
}

void
NodeRigidBody::setHandle(int handle, const Vec3f &v)
{
    SFRotation *sfrotation = orientation();
    const float *rot = sfrotation->getValue();
    const float *ftranslation = position()->getValue();
    Matrix mat;

    glPushMatrix();
    glLoadIdentity();
    glTranslatef(ftranslation[0], ftranslation[1], ftranslation[2]);
    glRotatef(RAD2DEG(rot[3]), rot[0], rot[1], rot[2]);
    glGetFloatv(GL_MODELVIEW_MATRIX, mat);
    glPopMatrix();

    ProtoRigidBody *proto = (ProtoRigidBody *)getProto();

    switch (handle) {
      case TRANSLATION:
        {
        Vec3f vec(m_scene->constrainVec(v));
        m_scene->setField(this, proto->position, new SFVec3f(mat * vec));
        }
        break;
      case TRANSLATION_X:
        m_scene->setField(this, proto->position, 
              new SFVec3f(mat * (v - Vec3f(HANDLE_SIZE, 0.0f, 0.0f))));
        break;
      case TRANSLATION_Y:
        m_scene->setField(this, proto->position, 
              new SFVec3f(mat * (v - Vec3f(0.0f, HANDLE_SIZE, 0.0f))));
        break;
      case TRANSLATION_Z:
        m_scene->setField(this, proto->position, 
              new SFVec3f(mat * (v - Vec3f(0.0f, 0.0f, HANDLE_SIZE))));
        break;
      case ROTATION:
        m_scene->setField(this, proto->orientation, new SFRotation(v, 0));
        break;
      case ROTATION_X:
        m_scene->setField(this, proto->orientation, 
              new SFRotation(Quaternion(Vec3f(1.0f, 0.0f, 0.0f), v.x) * 
                             sfrotation->getQuat()));
        break;
      case ROTATION_Y:
        m_scene->setField(this, proto->orientation, 
              new SFRotation(Quaternion(Vec3f(0.0f, 1.0f, 0.0f), v.y) * 
                             sfrotation->getQuat()));
        break;
      case ROTATION_Z:
        m_scene->setField(this, proto->orientation, 
              new SFRotation(Quaternion(Vec3f(0.0f, 0.0f, 1.0f), v.z) * 
                             sfrotation->getQuat()));
        break;
      default:
        if (handle == m_handleNameLinearVelocity)
            m_scene->setField(this, proto->linearVelocity, new SFVec3f(v));
        else if (handle == m_handleNameAngularVelocityX1)
            m_scene->setField(this, proto->angularVelocity, 
                             new SFVec3f(v.z, v.y, v.x));
        else if (handle == m_handleNameAngularVelocityX2)
            m_scene->setField(this, proto->angularVelocity, 
                             new SFVec3f(-v.y, v.x, v.z));
        else if (handle == m_handleNameAngularVelocityY1)
            m_scene->setField(this, proto->angularVelocity, 
                             new SFVec3f(v.y, v.x, v.z));
        else if (handle == m_handleNameAngularVelocityY2)
            m_scene->setField(this, proto->angularVelocity, 
                             new SFVec3f(v.x, -v.z, v.y));
        else if (handle == m_handleNameAngularVelocityZ1)
            m_scene->setField(this, proto->angularVelocity, 
                             new SFVec3f(v.x, v.z, v.y));
        else if (handle == m_handleNameAngularVelocityZ2)
            m_scene->setField(this, proto->angularVelocity, 
                             new SFVec3f(v.z, v.y, -v.x));
    }
}


