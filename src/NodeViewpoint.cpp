/*
 * NodeViewpoint.cpp
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

#include "DuneApp.h"
#include "NodeViewpoint.h"
#include "Scene.h"
#include "Proto.h"
#include "FieldValue.h"
#include "SFBool.h"
#include "SFFloat.h"
#include "SFRotation.h"
#include "SFString.h"
#include "SFTime.h"
#include "SFVec3f.h"
#include "Field.h"
#include "ExposedField.h"
#include "resource.h"
#include "Util.h"

ProtoViewpoint::ProtoViewpoint(Scene *scene)
  : Proto(scene, "Viewpoint")
{
    double unitAngle = scene->getUnitAngle();

    set_bind.set(addEventIn(SFBOOL, "set_bind"));
    if (TheApp->getCoverMode()) {
        // non standard Covise/COVER extensions
        addEventIn(SFBOOL, "set_bindLast", FF_COVER_ONLY);
    }
    centerOfRotation.set(
          addExposedField(SFVEC3F, "centerOfRotation", new SFVec3f(0, 0, 0)));
    setFieldFlags(centerOfRotation, FF_X3D_ONLY);
    description.set(
          addField(SFSTRING, "description", new SFString("")));
    setFieldFlags(description, FF_VRML_ONLY);
    descriptionX3D.set(
          addExposedField(SFSTRING, "description", new SFString("")));
    setFieldFlags(descriptionX3D, FF_X3D_ONLY);
    fieldOfView.set(
          addExposedField(SFFLOAT, "fieldOfView", 
                          new SFFloat(M_PI / 4.0f / unitAngle), 
                          new SFFloat(0.0f), new SFFloat(M_PI / unitAngle)));
    jump.set(
         addExposedField(SFBOOL, "jump", new SFBool(true)));
    orientation.set(
         addExposedField(SFROTATION, "orientation", 
                         new SFRotation(0, 0, 1, 0)));
    setFieldFlags(orientation, EIF_RECOMMENDED);
    position.set(
         addExposedField(SFVEC3F, "position", new SFVec3f(0, 0, 10)));
    setFieldFlags(position, EIF_RECOMMENDED);
    retainUserOffsets.set(
        addExposedField(SFBOOL, "retainUserOffsets", new SFBool(false)));
    setFieldFlags(retainUserOffsets, FF_X3D_ONLY);

    if (TheApp->getCoverMode()) {
        // non standard Covise/COVER extensions
        type.set(
           addExposedField(SFSTRING, "type", new SFString("")));
        setFieldFlags(type, FF_COVER_ONLY);
    }
    if (TheApp->getKambiMode()) {
        // non standard Kambi game engine extensions
        direction.set(
           addExposedField(MFVEC3F, "direction", new MFVec3f()));
        setFieldFlags(direction, FF_KAMBI_ONLY);
        up.set(
           addExposedField(MFVEC3F, "up", new MFVec3f()));
        setFieldFlags(up, FF_KAMBI_ONLY);
        gravityUp.set(
           addExposedField(SFVEC3F, "gravityUp", new SFVec3f(0, 1, 0)));
        setFieldFlags(gravityUp, FF_KAMBI_ONLY);

        kambiViewpointCommonFields()
    }

    bind.set(
          addExposedField(SFBOOL, "bind", new SFBool(false)));
    setFieldFlags(bind, FF_X3DOM_ONLY);

    isActive.set(
       addExposedField(SFBOOL, "isActive", new SFBool(false)));
    setFieldFlags(isActive, FF_X3DOM_ONLY);

    zFar.set(
          addExposedField(SFFLOAT, "zFar", new SFFloat(10000)));
    setFieldFlags(zFar, FF_X3DOM_ONLY);

    zNear.set(
          addExposedField(SFFLOAT, "zNear", new SFFloat(1)));
    setFieldFlags(zNear, FF_X3DOM_ONLY);

    addEventOut(SFTIME, "bindTime");
    addEventOut(SFBOOL, "isBound");
}

Node *
ProtoViewpoint::create(Scene *scene)
{ 
    return new NodeViewpoint(scene, this); 
}

NodeViewpoint::NodeViewpoint(Scene *scene, Proto *def)
  : CoverNode(scene, def)
{
    Matrix matrix = Matrix::identity();
    for (int i = 0; i < 16; i++)
        m_matrix[i] = matrix[i];
    m_matrix[14] = -10.0f;
}

void 
NodeViewpoint::preDraw(bool useStereo)
{
    float matrix[16];
    float matrix2[16];

    glGetFloatv(GL_MODELVIEW_MATRIX, matrix);
    Util::invertMatrix(matrix2, matrix);
    glPushMatrix();
    glLoadIdentity();
    if (m_scene->isCurrentViewpoint(this)) 
        transformForViewpoint(useStereo);
    glMultMatrixf((GLfloat *) matrix2);
    glGetFloatv(GL_MODELVIEW_MATRIX, m_matrix);
    glPopMatrix();

    m_scene->addViewpoint(this);
}

void 
NodeViewpoint::apply(bool useStereo)
{
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    if (m_scene->isCurrentViewpoint(this)) {
        double unitAngle = m_scene->getUnitAngle();
        const float *rot = orientation()->getValue();
        const float *pos = position()->getValue();

        glRotatef(RAD2DEG(-rot[3] * unitAngle), rot[0], rot[1], rot[2]);
        glTranslatef(-pos[0], -pos[1], -pos[2]);
    }
    glGetFloatv(GL_MODELVIEW_MATRIX, m_matrix);
    glPopMatrix();
    transformForViewpoint(useStereo);
}

void 
NodeViewpoint::transformForViewpoint(bool useStereo)
{
    double unitAngle = m_scene->getUnitAngle();

    const float *rot = orientation()->getValue();
    const float *pos = position()->getValue();

    float eyeposition=0; 
    float eyeangle=0; 
    if (useStereo)
       {
       // inexact "toe in" stereo method 
       if (TheApp->getEyeMode()==EM_RIGHT)
          {
          eyeposition= - TheApp->getEyeHalfDist();
          eyeangle= - TheApp->getEyeAngle();
          }
       else if (TheApp->getEyeMode()==EM_LEFT)
          {
          eyeposition= + TheApp->getEyeHalfDist();
          eyeangle= + TheApp->getEyeAngle();
          }
       }
    glTranslatef(eyeposition, 0, 0);
    glRotatef(eyeangle, 0,1,0);
    glRotatef(-RAD2DEG(rot[3] * unitAngle), rot[0], rot[1], rot[2]);
    glTranslatef(-pos[0], -pos[1], -pos[2]);
}

Vec3f
NodeViewpoint::getPosition() const
{
    return Vec3f(((NodeViewpoint *)this)->position()->getValue());
}

void NodeViewpoint::setPosition(const Vec3f &pos)
{
    m_scene->setField(this, position_Field(), new SFVec3f(pos));
}

const Quaternion &NodeViewpoint::getOrientation() const
{
    return ((NodeViewpoint *)this)->orientation()->getQuat();
}


void NodeViewpoint::setOrientation(const Quaternion &quat)
{
    SFRotation *rot = new SFRotation(quat);
    rot->reverseFixAngle(m_scene->getUnitAngle());
    m_scene->setField(this, orientation_Field(), rot);
}

void NodeViewpoint::flip(int index)
{
    position()->flip(index);
    orientation()->flip(index);
}

void NodeViewpoint::swap(int fromTo)
{
    position()->swap(fromTo);
    orientation()->swap(fromTo);
    update();
}

int
NodeViewpoint::getAnimationCommentID(void) 
{ 
    return IDS_ANIMATION_HELP_VIEWPOINT + swGetLang(); 
}


void
NodeViewpoint::getMatrix(float* matrix)
{
    for (int i = 0; i < 16; i++)
       matrix[i] = m_matrix[i];
}

int NodeViewpoint::getProfile(void) const
{ 
    if (!isDefault(fieldOfView_Field()))
        return PROFILE_IMMERSIVE;
//    if (!isDefault(description_Field()))
//        return PROFILE_IMMERSIVE;
    return PROFILE_INTERCHANGE;
}

