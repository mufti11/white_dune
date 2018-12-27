/*
 * NodeSliderJoint.cpp
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
#ifndef FLT_MAX
# include <float.h>
#endif
#include "stdafx.h"

#include "NodeSliderJoint.h"
#include "Proto.h"
#include "Field.h"
#include "FieldValue.h"
#include "MFNode.h"
#include "SFBool.h"
#include "DuneApp.h"
#include "Scene.h"
#include "RenderState.h"

ProtoSliderJoint::ProtoSliderJoint(Scene *scene)
  : X3DRigidJointProto(scene, "SliderJoint")
{
    addElements();
}

ProtoSliderJoint::ProtoSliderJoint(Scene *scene, const char* name)
  : X3DRigidJointProto(scene, name)
{
    addElements();
}

void 
ProtoSliderJoint::addElements(void)
{
    axis.set(addExposedField(SFVEC3F, "axis", new SFVec3f(0, 1, 0)));
    maxSeparation.set( 
          addExposedField(SFFLOAT, "maxSeparation", new SFFloat(1),
                          new SFFloat(0)));
    minSeparation.set( 
          addExposedField(SFFLOAT, "minSeparation", new SFFloat(0),
                          new SFFloat(0)));
    stopBounce.set(addExposedField(SFFLOAT, "stopBounce", new SFFloat(0), 
                                   new SFFloat(0), new SFFloat(1)));
    stopErrorCorrection.set(addExposedField(SFFLOAT, 
                                            "stopErrorCorrection", 
                                            new SFFloat(1), 
                                            new SFFloat(0), new SFFloat(1)));
    addEventOut(SFFLOAT, "separation");
    addEventOut(SFFLOAT, "separationRate");
}

Node *
ProtoSliderJoint::create(Scene *scene)
{ 
    return new NodeSliderJoint(scene, this); 
}

NodeSliderJoint::NodeSliderJoint(Scene *scene, Proto *def)
  : X3DRigidJointNode(scene, def)
{
}

Vec3f   
NodeSliderJoint::getHandle(int handle, int *constraint, int *field)
{
    const float *v = NULL;
    switch (handle) {
      case AXIS1:
        *constraint = CONSTRAIN_NONE;
        *field = axis_Field();
        v = axis()->getValue();
        return Vec3f(v[0], v[1], v[2]);
      default:
        *constraint = CONSTRAIN_NONE;
        *field = getProto()->metadata_Field();       
        return Vec3f(0.0f, 0.0f, 0.0f);        
    }
}

void    
NodeSliderJoint::setHandle(int handle, const Vec3f &v)
{
    Vec3f normedV(0, 0, 0);
    switch (handle) {
      case AXIS1:
        if (m_scene->getXonly() || (m_scene->getConstrain() == CONSTRAIN_NONE))
            normedV.x = v.x;
        if (m_scene->getYonly() || (m_scene->getConstrain() == CONSTRAIN_NONE))
            normedV.y = v.y;
        if (m_scene->getZonly() || (m_scene->getConstrain() == CONSTRAIN_NONE))
            normedV.z = v.z;
        normedV.normalize();
        m_scene->setField(this, axis_Field(),
                         new SFVec3f(normedV.x, normedV.y, normedV.z));
        break;
    }
}

void
NodeSliderJoint::drawHandles() {

    const float *faxis = axis()->getValue();
    
    glPushMatrix();
    transformForHandle(AXIS1);
  
    RenderState state;
    state.drawVectorHandle(AXIS1, faxis[0], faxis[1], faxis[2], 
                           1.0f, 0.5f, 0.0f);
    glPopMatrix();
}


