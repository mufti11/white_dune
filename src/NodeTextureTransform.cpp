/*
 * NodeTextureTransform.cpp
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

#include "NodeTextureTransform.h"
#include "Proto.h"
#include "FieldValue.h"
#include "SFVec2f.h"
#include "SFFloat.h"
#include "Scene.h"

ProtoTextureTransform::ProtoTextureTransform(Scene *scene)
  : WonderlandExportProto(scene, "TextureTransform")
{
    center.set( 
          addExposedField(SFVEC2F, "center", new SFVec2f(0.0f, 0.0f)));
    rotation.set( 
          addExposedField(SFFLOAT, "rotation", new SFFloat(0.0f)));
    scale.set( 
          addExposedField(SFVEC2F, "scale", new SFVec2f(1.0f, 1.0f)));
    translation.set( 
          addExposedField(SFVEC2F, "translation", new SFVec2f(0.0f, 0.0f)));
}

Node *
ProtoTextureTransform::create(Scene *scene)
{ 
    return new NodeTextureTransform(scene, this); 
}

NodeTextureTransform::NodeTextureTransform(Scene *scene, Proto *def)
  : Node(scene, def)
{
}

void
NodeTextureTransform::bind()
{
    double unitAngle = m_scene->getUnitAngle();

    const float *fcenter = center()->getValue();

    glMatrixMode(GL_TEXTURE);
    glPushMatrix();

    glTranslatef(-fcenter[0], -fcenter[1], 0.0f);
    glScalef(scale()->getValue()[0], scale()->getValue()[1], 1.0f);
    glRotatef(RAD2DEG(rotation()->getValue() * unitAngle), 0.0f, 0.0f, 1.0f);
    glTranslatef(fcenter[0], fcenter[1], 0.0f);
    glTranslatef(translation()->getValue()[0], translation()->getValue()[1], 
                 0.0f);
    glMatrixMode(GL_MODELVIEW);
}

void
NodeTextureTransform::unbind()
{
    glMatrixMode(GL_TEXTURE);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
}
