/*
 * NodeClipPlane.cpp
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

#include "NodeClipPlane.h"
#include "RenderState.h"
#include "Proto.h"
#include "FieldValue.h"
#include "SFBool.h"
#include "SFVec4f.h"
#include "DuneApp.h"
#include "Scene.h"

ProtoClipPlane::ProtoClipPlane(Scene *scene)
  : Proto(scene, "ClipPlane")
{
    enabled.set(
        addExposedField(SFBOOL, "enabled", new SFBool(true)));
    plane.set(
        addExposedField(SFVEC4F, "plane", new SFVec4f(0, 1, 0, 0)));
    
    cappingColor.set(
        addExposedField(SFCOLOR, "cappingColor", new SFColor(0, 0, 0)));
    setFieldFlags(cappingColor, FF_X3DOM_ONLY);

    cappingStrength.set(
        addExposedField(SFFLOAT, "cappingStrength", new SFFloat(0)));
    setFieldFlags(cappingStrength, FF_X3DOM_ONLY);

    on.set(
        addExposedField(SFBOOL, "on", new SFBool(true)));
    setFieldFlags(on, FF_X3DOM_ONLY);
}

Node *
ProtoClipPlane::create(Scene *scene)
{ 
    return new NodeClipPlane(scene, this); 
}

NodeClipPlane::NodeClipPlane(Scene *scene, Proto *def)
  : Node(scene, def)
{
}

NodeClipPlane::~NodeClipPlane()
{
    unbind();
}

void NodeClipPlane::bind()
{
    bool benabled = enabled()->getValue();
    const float *fplane = plane()->getValue();
    double dplane[4] = { fplane[0], fplane[1], fplane[2], fplane[3] };

    if (benabled) {
        m_clipPlane = m_scene->allocateClipPlane();
        glClipPlane(m_clipPlane, dplane);
        glEnable(m_clipPlane);
    }
}

void NodeClipPlane::unbind()
{
    bool benabled = enabled()->getValue();

    if (benabled) {
        glDisable(m_clipPlane);
        m_scene->freeClipPlane();
    }
}

void    
NodeClipPlane::flip(int index)
{
    const float *fplane = plane()->getValue();
    Vec3f n(fplane);
    n.normalize();
    float norm[4] = { index == 0 ? -n.x : n.x, 
                      index == 1 ? -n.y : n.y, 
                      index == 2 ? -n.z : n.z, fplane[3] };
    Node::setField(plane_Field(), new SFVec4f(norm));
}

void    
NodeClipPlane::swap(int fromTo)
{
    const float *fplane = plane()->getValue();
    Vec3f n(fplane);
    n.normalize();
    switch(fromTo) {
      case SWAP_XY:
         ::myswap(n.x, n.y);
         break;
      case SWAP_XZ:
         ::myswap(n.x, n.z);
         break;
      case SWAP_YZ:
         ::myswap(n.y, n.z);
         break;
    }
    float norm[4] = { n.x, n.y, n.z, fplane[3] };
    Node::setField(plane_Field(), new SFVec4f(norm));
}

void    
NodeClipPlane::setField(int index, FieldValue *value, int cf)
{
    const float *fplane = ((SFVec4f *)value)->getValue();
    Vec3f n(fplane);
    n.normalize();
    float norm[4] = { n.x, n.y, n.z, fplane[3] };
    Node::setField(plane_Field(), new SFVec4f(norm), cf);
}

Vec3f   
NodeClipPlane::getHandle(int handle, int *constraint, int *field)
{
    const float *v = NULL;
    switch (handle) {
      case 0:    
        *constraint = CONSTRAIN_NONE;
        *field = plane_Field();
        v = plane()->getValue();
        return Vec3f(v[0], v[1], v[2]);
        break;
      case 1:
        *constraint = CONSTRAIN_NONE;
        *field = plane_Field();
        v = plane()->getValue();
        return Vec3f(0, -v[3], 0);
        break;
      default:
        *constraint = CONSTRAIN_NONE;
        *field = getProto()->metadata_Field();
        return Vec3f(0.0f, 0.0f, 0.0f);        
    }
}

void    
NodeClipPlane::setHandle(int handle, const Vec3f &v)
{
    const float *fplane = plane()->getValue();
    Vec3f n(v);
    n.normalize();
    float norm[4] = { n.x, n.y, n.z, fplane[3] };
    float len[4] = { fplane[0], fplane[1], fplane[2], -v.y};
    switch (handle) {
      case 0:    
        m_scene->setField(this, plane_Field(), new SFVec4f(norm));
        break;
      case 1:
        m_scene->setField(this, plane_Field(), new SFVec4f(len));        
        break;
    }
}

void
NodeClipPlane::drawHandles(void) 
{
    glPushMatrix();
  
    RenderState state;
    state.setVectorStart(0.1);
    const float *fplane = plane()->getValue();
    Vec3f dist(fplane);
    dist = dist * fplane[3];
    state.drawVectorHandle(0, fplane[0], fplane[1], fplane[2], 
                           1.0f, 0.5f, 0.0f);
    state.drawVectorHandle(1, -dist[0], -dist[1], -dist[2], 
                           1.0f, 0.0f, 0.5f);
    glPopMatrix();
}



