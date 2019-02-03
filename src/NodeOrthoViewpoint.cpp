/*
 * NodeOrthoViewpoint.cpp
 *
 * Copyright (C) 2009 J. "MUFTI" Scheurich
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

#include "NodeOrthoViewpoint.h"
#include "Proto.h"
#include "FieldValue.h"
#include "SFVec3f.h"
#include "SFString.h"
#include "MFFloat.h"
#include "SFBool.h"
#include "SFRotation.h"
#include "SFVec3f.h"
#include "SFBool.h"
#include "Scene.h"

ProtoOrthoViewpoint::ProtoOrthoViewpoint(Scene *scene)
  : Proto(scene, "OrthoViewpoint")
{
    centerOfRotation.set(
        addExposedField(SFVEC3F, "centerOfRotation", new SFVec3f(0, 0, 0)));
    description.set(
        addExposedField(SFSTRING, "description", new SFString("")));
    float values[] = { -1.0f, -1.0f, 1.0f, 1.0f };
    float *v = new float[4];  
    memcpy(v, values, 4 * sizeof(float));
    fieldOfView.set(
        addExposedField(MFFLOAT, "fieldOfView", new MFFloat(v, 4)));
    jump.set(
        addExposedField(SFBOOL, "jump", new SFBool(true)));
    orientation.set(
        addExposedField(SFROTATION, "orientation", new SFRotation(0, 0, 1, 0)));
    position.set(
        addExposedField(SFVEC3F, "position", new SFVec3f(0, 0, 10)));
    retainUserOffsets.set(
        addExposedField(SFBOOL, "retainUserOffsets", new SFBool(false)));

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
}

Node *
ProtoOrthoViewpoint::create(Scene *scene)
{ 
    return new NodeOrthoViewpoint(scene, this); 
}

NodeOrthoViewpoint::NodeOrthoViewpoint(Scene *scene, Proto *def)
  : Node(scene, def)
{
}

void 
NodeOrthoViewpoint::apply(bool useStereo)
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
NodeOrthoViewpoint::transformForViewpoint(bool useStereo)
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
NodeOrthoViewpoint::getPosition() const
{
    return Vec3f(((NodeOrthoViewpoint *)this)->position()->getValue());
}

void NodeOrthoViewpoint::setPosition(const Vec3f &pos)
{
    m_scene->setField(this, position_Field(), new SFVec3f(pos));
}

Quaternion 
NodeOrthoViewpoint::getOrientation() const
{
    return ((NodeOrthoViewpoint *)this)->orientation()->getQuat();
}

void NodeOrthoViewpoint::setOrientation(const Quaternion &quat)
{
    SFRotation *rot = new SFRotation(quat);
    rot->reverseFixAngle(m_scene->getUnitAngle());
    m_scene->setField(this, orientation_Field(), rot);
}

void
NodeOrthoViewpoint::getMatrix(float* matrix)
{
    for (int i = 0; i < 16; i++)
       matrix[i] = m_matrix[i];
}


