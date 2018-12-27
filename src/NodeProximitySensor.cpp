/*
 * NodeProximitySensor.cpp
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

#ifndef INT_MAX
# include <limits.h>
#endif

#include "NodeProximitySensor.h"
#include "Proto.h"
#include "FieldValue.h"
#include "SFVec3f.h"
#include "SFFloat.h"
#include "SFBool.h"
#include "Field.h"
#include "RenderState.h"
#include "Util.h"
#include "Scene.h"
#include "Path.h"
#include "NodeNavigationInfo.h"

ProtoProximitySensor::ProtoProximitySensor(Scene *scene)
  : WonderlandExportProto(scene, "ProximitySensor")
{
    center.set(
          addExposedField(SFVEC3F, "center", new SFVec3f(0.0f, 0.0f, 0.0f)));
    enabled.set(
          addExposedField(SFBOOL, "enabled", new SFBool(true)));
    setFieldFlags(enabled, EIF_WONDERLAND_SUPPORTED);

    size.set(
          addExposedField(SFVEC3F, "size", new SFVec3f(0.0f, 0.0f, 0.0f), 
                          new SFFloat(0.0f)));
    addEventOut(SFTIME, "enterTime", EOF_RECOMMENDED | 
                                     EOF_WONDERLAND_SUPPORTED);
    addEventOut(SFTIME, "exitTime", EOF_WONDERLAND_SUPPORTED);
    addEventOut(SFVEC3F, "centerOfRotation_changed", FF_X3D_ONLY);
    addEventOut(SFBOOL, "isActive", EOF_WONDERLAND_SUPPORTED);
    addEventOut(SFVEC3F, "position_changed", EOF_RECOMMENDED | 
                                             EOF_WONDERLAND_SUPPORTED);
    addEventOut(SFROTATION, "orientation_changed", EOF_RECOMMENDED | 
                                                   EOF_WONDERLAND_SUPPORTED);
}

Node *
ProtoProximitySensor::create(Scene *scene)
{ 
    return new NodeProximitySensor(scene, this); 
}

NodeProximitySensor::NodeProximitySensor(Scene *scene, Proto *def)
  : Node(scene, def)
{
    m_handleScale = 1.0f;
    m_handleOffset = PROXIMITY_CENTER_Z + 1;
}

void
NodeProximitySensor::transformForHandle(int handle)
{
    const float *fcenter = center()->getValue();

    glTranslatef(fcenter[0], fcenter[1], fcenter[2]);
    return;
}

void
NodeProximitySensor::drawHandles()
{
    m_handleScale = TheApp->GetHandleScale() *
                   m_scene->getNavigationInfoNode()->speed()->getValue();

    drawLines(true);    

    const float *fsize = size()->getValue();
    const float *fcenter = center()->getValue();
    RenderState state;

    glPushMatrix();
    state.drawProximityCenterHandles(this, m_handleScale);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(fcenter[0], fcenter[1], fcenter[2]);
    glScalef(fsize[0] * 0.5f, fsize[1] * 0.5f, fsize[2] * 0.5f);
    glPushAttrib(GL_LIGHTING);
    glDisable(GL_LIGHTING);
    glPushName(0);
    state.startDrawHandles();
    for (int i = 0; i < 8; i++) {
        state.setHandleColor(m_scene, i);
        glLoadName(i + m_handleOffset);
        state.drawHandle(boxCorners[i]);
    }
    state.endDrawHandles();
    glPopName();
    glPopAttrib();
    glPopMatrix();
}

Vec3f
NodeProximitySensor::getHandle(int handle, int *constraint, int *field)
{
    if (handle >= m_handleOffset) {
        const float *fsize = size()->getValue();
        int h = handle - m_handleOffset;
        switch (h) {
          case TRF: case TLF: case TRB: case TLB:
          case BRF: case BLF: case BRB: case BLB:
            *field = size_Field();
            return Vec3f(fsize) * Vec3f(boxCorners[h]) * 0.5f;
          case sizeof(boxCorners) / sizeof(float) / 3 + 1:
            break;
        }
    } else {
      switch (handle) {
        case PROXIMITY_CENTER_X:
          *constraint = CONSTRAIN_X;
          *field = center_Field();
          return Vec3f(HANDLE_SIZE, 0.0f, 0.0f);
        case PROXIMITY_CENTER_Y:
          *constraint = CONSTRAIN_Y;
          *field = center_Field();
          return Vec3f(0.0f, HANDLE_SIZE, 0.0f);
        case PROXIMITY_CENTER_Z:
          *constraint = CONSTRAIN_Z;
          *field = center_Field();
          return Vec3f(0.0f, 0.0f, HANDLE_SIZE);
        case PROXIMITY_CENTER:
          *field = center_Field();        
          return Vec3f(0.0f, 0.0f, 0.0f);
      }    
    }
    *field = getProto()->metadata_Field();
    return Vec3f(0.0f, 0.0f, 0.0f);
}

void
NodeProximitySensor::setHandle(int handle, const Vec3f &v)
{
    if (handle >= m_handleOffset) {
        int h = handle - m_handleOffset;
        switch (h) {
          case TRF: case TLF: case TRB: case TLB:
          case BRF: case BLF: case BRB: case BLB:
            m_scene->setField(this, size_Field(), 
                             new SFVec3f(v * 2.0f * Vec3f(boxCorners[h])));
            break;
        }
    } else {
        const float *fcenter = center()->getValue();
        Matrix mat;
        glPushMatrix();
        glLoadIdentity();
        glTranslatef(fcenter[0], fcenter[1], fcenter[2]);
        glGetFloatv(GL_MODELVIEW_MATRIX, mat);
        glPopMatrix();
        switch (handle) {
          case PROXIMITY_CENTER:
            {
            Vec3f vec(m_scene->constrainVec(v));
            m_scene->setField(this, center_Field(), new SFVec3f(mat * vec));
            }
            break;
          case PROXIMITY_CENTER_X:
            m_scene->setField(this, center_Field(),
                             new SFVec3f(mat * 
                                         (v - Vec3f(HANDLE_SIZE, 0.0f, 0.0f))));
            break;
          case PROXIMITY_CENTER_Y:
            m_scene->setField(this, center_Field(),
                             new SFVec3f(mat *
                                         (v - Vec3f(0.0f, HANDLE_SIZE, 0.0f))));
            break;
         case PROXIMITY_CENTER_Z:
            m_scene->setField(this, center_Field(),
                             new SFVec3f(mat * 
                                         (v - Vec3f(0.0f, 0.0f, HANDLE_SIZE))));
        }
    }
}

int NodeProximitySensor::getProfile(void) const
{ 
    if (hasOutput("position_changed"))
        return PROFILE_IMMERSIVE;
    if (hasOutput("orientation_changed"))
        return PROFILE_IMMERSIVE;
    return PROFILE_INTERACTIVE; 
}

void NodeProximitySensor::drawLines(bool handleDraw)
{
    glPushName(INT_MAX); 
    glDisable(GL_LIGHTING);
    if (handleDraw) {
        glLineWidth(1.0f);
        glEnable(GL_LINE_SMOOTH);
        Util::myGlColor3f(1.0f, 1.0f, 1.0f);
    } else {
        glLineWidth(2.0f);
        Util::myGlColor3f(0.0f, 1.0f, 0.0f);
    }
    const float* fsize = size()->getValue();
    const float* fcenter = center()->getValue();
    for (int i = 0; i < 6; i++) {
        glBegin(GL_LINE_STRIP);
        for (int j = 0; j < 4; j++) {
            glVertex3f(boxCorners[boxIndices[i * 4 + j]][0] * fsize[0] / 2.0f + 
                       fcenter[0],
                       boxCorners[boxIndices[i * 4 + j]][1] * fsize[1] / 2.0f + 
                       fcenter[1],
                       boxCorners[boxIndices[i * 4 + j]][2] * fsize[2] / 2.0f + 
                       fcenter[2]);
        }
        glVertex3f(boxCorners[boxIndices[i * 4 + 0]][0] * fsize[0] / 2.0f + 
                   fcenter[0],
                   boxCorners[boxIndices[i * 4 + 0]][1] * fsize[1] / 2.0f + 
                   fcenter[1],
                   boxCorners[boxIndices[i * 4 + 0]][2] * fsize[2] / 2.0f + 
                   fcenter[2]);
        glEnd();
    }

    glPopName();
    glEnable(GL_LIGHTING);
}

void NodeProximitySensor::draw(int pass)
{
    if (pass != RENDER_PASS_NON_TRANSPARENT)
        return;
    if (m_scene->getSelection()->getNode() != this)
        return;
    drawLines(false);
}

