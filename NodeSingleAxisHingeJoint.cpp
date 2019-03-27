/*
 * NodeSingleAxisHingeJoint.cpp
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

#include "NodeSingleAxisHingeJoint.h"
#include "Proto.h"
#include "Field.h"
#include "FieldValue.h"
#include "MFNode.h"
#include "SFBool.h"
#include "DuneApp.h"
#include "Scene.h"
#include "RenderState.h"
#include "Util.h"

ProtoSingleAxisHingeJoint::ProtoSingleAxisHingeJoint(Scene *scene)
  : AnchoredJointProto(scene, "SingleAxisHingeJoint")
{
    addElements(scene);
}

ProtoSingleAxisHingeJoint::ProtoSingleAxisHingeJoint(Scene *scene,
                                                     const char * name)
  : AnchoredJointProto(scene, name)
{
    addElements(scene);
}

void 
ProtoSingleAxisHingeJoint::addElements(Scene *scene) 
{
    double unitAngle = scene->getUnitAngle();

    axis.set(addExposedField(SFVEC3F, "axis", new SFVec3f()));
    maxAngle.set( 
          addExposedField(SFFLOAT, "maxAngle", new SFFloat(M_PI / unitAngle),
                          new SFFloat(-M_PI / unitAngle), 
                          new SFFloat( M_PI / unitAngle)));
    minAngle.set( 
          addExposedField(SFFLOAT, "minAngle", new SFFloat(-M_PI / unitAngle),
                          new SFFloat(-M_PI / unitAngle), 
                          new SFFloat( M_PI / unitAngle)));
    stopBounce.set(addExposedField(SFFLOAT, "stopBounce", new SFFloat(0), 
                                   new SFFloat(0), new SFFloat(1)));
    stopErrorCorrection.set(addExposedField(SFFLOAT, 
                                            "stopErrorCorrection", 
                                            new SFFloat(0.8), 
                                            new SFFloat(0), new SFFloat(1)));
    addEventOut(SFFLOAT, "angle");
    addEventOut(SFFLOAT, "angleRate");
}

Node *
ProtoSingleAxisHingeJoint::create(Scene *scene)
{ 
    return new NodeSingleAxisHingeJoint(scene, this); 
}

NodeSingleAxisHingeJoint::NodeSingleAxisHingeJoint(Scene *scene, Proto *def)
  : AnchoredJointNode(scene, def)
{
}

Vec3f   
NodeSingleAxisHingeJoint::getHandle(int handle, int *constraint, int *field)
{
    const float *v = NULL;
    switch (handle) {
      case TRANSLATION:    
      case TRANSLATION_X:    
      case TRANSLATION_Y:    
      case TRANSLATION_Z:    
        return AnchoredJointNode::getHandle(handle, constraint, field);
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
NodeSingleAxisHingeJoint::setHandle(int handle, const Vec3f &v)
{
    Vec3f normedV(0, 0, 0);
    switch (handle) {
      case TRANSLATION:    
      case TRANSLATION_X:    
      case TRANSLATION_Y:    
      case TRANSLATION_Z:    
        AnchoredJointNode::setHandle(handle, v);
        break;
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
NodeSingleAxisHingeJoint::drawHandles() {

    AnchoredJointNode::drawHandles(true);
    const float *faxis = axis()->getValue();
    
    glPushMatrix();
    transformForHandle(AXIS1);
  
    RenderState state;
    state.drawVectorHandle(AXIS1, faxis[0], faxis[1], faxis[2], 
                           1.0f, 0.5f, 0.0f);
    glPopMatrix();
}

