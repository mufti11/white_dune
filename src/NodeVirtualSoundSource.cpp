/*
 * NodeVirtualSoundSource.cpp
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

#include <stdio.h>
#include "stdafx.h"

#include "NodeVirtualSoundSource.h"
#include "Proto.h"
#include "FieldValue.h"
#include "SFBool.h"
#include "SFInt32.h"
#include "SFString.h"
#include "SFRotation.h"
#include "SFVec3f.h"
#include "RenderState.h"
#include "Util.h"
#include "Scene.h"

ProtoVirtualSoundSource::ProtoVirtualSoundSource(Scene *scene)
  : Proto(scene, "VirtualSoundSource")
{
    mute.set(
          addExposedField(SFBOOL, "mute", new SFBool(false)));
    gain.set(
          addExposedField(SFFLOAT, "gain", new SFFloat(1.0f),
                          new SFFloat(0.0f), new SFFloat(1.0f)));
    position.set(
          addExposedField(SFVEC3F, "position", new SFVec3f(0, 0, 0)));
    orientation.set(
          addExposedField(SFROTATION, "orientation", 
                          new SFRotation(0, 0, 1, 0)));
    soundMode.set(
          addField(SFSTRING, "soundMode", new SFString("Tracked")));
    soundType.set(
          addField(SFSTRING, "soundType", new SFString("File")));
    source.set(
          addField(SFSTRING, "source", new SFString("")));

    addURLs(URL_COVER);
}

Node *
ProtoVirtualSoundSource::create(Scene *scene)
{ 
    return new NodeVirtualSoundSource(scene, this); 
}

NodeVirtualSoundSource::NodeVirtualSoundSource(Scene *scene, Proto *def)
  : CoverNode(scene, def)
{
}

void
NodeVirtualSoundSource::setField(int index, FieldValue *value, int cf)
{
    Node::setField(index, value, cf);
    update();
}

void
NodeVirtualSoundSource::drawHandles()
{
    float white[4] = { 1.0f, 1.0f, 1.0f, 1.0f };

    glPushMatrix();
    glPushAttrib(GL_LIGHTING);
    glDisable(GL_LIGHTING);

    const float *srcLocation = position()->getValue();
    glTranslatef(srcLocation[0], srcLocation[1], srcLocation[2]);

    glPushName(position_Field());
    glLoadName(position_Field());

    GLUquadricObj *obj = gluNewQuadric();

    Util::myGlMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, white);
    gluSphere(obj, 0.05f, 8, 4);
    glPopName();
    gluDeleteQuadric(obj);

    glPopAttrib();
    glPopMatrix();
}

Vec3f
NodeVirtualSoundSource::getHandle(int handle, int *constraint, int *field)
{
    *field = position_Field();
    return position()->getValue();
}

void
NodeVirtualSoundSource::setHandle(int handle, const Vec3f &v)
{
    m_scene->setField(this, position_Field(), new SFVec3f(v));
}

