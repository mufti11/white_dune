/*
 * AnchoredJointNode.cpp
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

#include "AnchoredJointNode.h"
#include "Scene.h"
#include "RenderState.h"
#include "NodeNavigationInfo.h"

AnchoredJointProto::AnchoredJointProto(Scene *scene, const char *name)
  : X3DRigidJointProto(scene, name)
{
    anchorPoint.set(addExposedField(SFVEC3F, "anchorPoint", new SFVec3f()));
    addEventOut(SFVEC3F, "body1AnchorPoint");
    addEventOut(SFVEC3F, "body2AnchorPoint");
}

AnchoredJointNode::AnchoredJointNode(Scene *scene, Proto *def)
  : X3DRigidJointNode(scene, def)
{
}

void
AnchoredJointNode::transformForHandle(int handle)
{
    const float *ftranslation = anchorPoint()->getValue();

    glTranslatef(ftranslation[0], ftranslation[1], ftranslation[2]);
}



Vec3f
AnchoredJointNode::getHandle(int handle, int *constraint, int *field)
{
    *field = anchorPoint_Field();
    switch (handle) {
      case TRANSLATION_X:
        *constraint = CONSTRAIN_X;
        return Vec3f(HANDLE_SIZE, 0.0f, 0.0f);
      case TRANSLATION_Y:
        *constraint = CONSTRAIN_Y;
        return Vec3f(0.0f, HANDLE_SIZE, 0.0f);
      case TRANSLATION_Z:
        *constraint = CONSTRAIN_Z;
        return Vec3f(0.0f, 0.0f, HANDLE_SIZE);
      case TRANSLATION:
        return Vec3f(0.0f, 0.0f, 0.0f);
      default:
        *constraint = CONSTRAIN_NONE;
        *field = getProto()->metadata_Field();
        return Vec3f(0.0f, 0.0f, 0.0f);        
    }
}

void
AnchoredJointNode::setHandle(int handle, const Vec3f &v)
{
    Vec3f anchor(anchorPoint()->getValue());

    switch (handle) {
      case TRANSLATION_X:
        m_scene->setField(this, anchorPoint_Field(), 
              new SFVec3f(anchor + v - Vec3f(HANDLE_SIZE, 0.0f, 0.0f)));
        break;
      case TRANSLATION_Y:
        m_scene->setField(this, anchorPoint_Field(), 
              new SFVec3f(anchor + v - Vec3f(0.0f, HANDLE_SIZE, 0.0f)));
        break;
      case TRANSLATION_Z:
        m_scene->setField(this, anchorPoint_Field(), 
              new SFVec3f(anchor + v - Vec3f(0.0f, 0.0f, HANDLE_SIZE)));
        break;
      default:
        Vec3f vec(m_scene->constrainVec(v));
        m_scene->setField(this, anchorPoint_Field(), 
                         new SFVec3f(anchor + vec));
    }
}

void
AnchoredJointNode::drawHandles(bool hasAxes)
{
    m_handleScale = TheApp->GetHandleScale() *
                   m_scene->getNavigationInfoNode()->speed()->getValue();
                   
    RenderState state;
    if (hasAxes)
        state.setAxesStart(0.5);
    state.drawTranslationHandles(this, m_handleScale);
}
