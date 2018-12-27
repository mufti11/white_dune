/*
 * NodeFog.cpp
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

#include "NodeFog.h"
#include "Proto.h"
#include "Field.h"
#include "FieldValue.h"
#include "SFColor.h"
#include "SFString.h"
#include "SFFloat.h"
#include "Scene.h"
#include "resource.h"

static const char *fogTypes[] = {"LINEAR", "EXPONENTIAL", NULL};

ProtoFog::ProtoFog(Scene *scene)
  : Proto(scene, "Fog")
{
    color.set(
          addExposedField(SFCOLOR, "color", new SFColor(1.0f, 1.0f, 1.0f)));

    fogType.set(
          addExposedField(SFSTRING, "fogType", new SFString("LINEAR"), 
                          FF_FIXEDSTRINGS, fogTypes));

    visibilityRange.set(
          addExposedField(SFFLOAT, "visibilityRange", new SFFloat(0.0f), 
                          new SFFloat(0.0f)));

    alternative.set(
         addExposedField(SFNODE, "alternative", new SFNode(NULL), VRML_FOG));
    setFieldFlags(alternative, FF_KAMBI_ONLY);

    FogElements()                          

    addEventIn(SFBOOL, "set_bind");
    addEventOut(SFTIME, "bindTime", FF_X3D_ONLY);
    addEventOut(SFBOOL, "isBound");
}

Node *
ProtoFog::create(Scene *scene)
{ 
    return new NodeFog(scene, this); 
}

NodeFog::NodeFog(Scene *scene, Proto *def)
  : Node(scene, def)
{
}

void
NodeFog::preDraw()
{
    glPushMatrix();
    glGetFloatv(GL_MODELVIEW_MATRIX, m_matrix);
    glPopMatrix();
    m_scene->addFog(this);
}

void
NodeFog::apply()
{
    if (visibilityRange()->getValue() > 0.0f) {
        if (!strcmp(fogType()->getValue(), "LINEAR")) {
            glFogi(GL_FOG_MODE, GL_LINEAR);
            glFogf(GL_FOG_END, visibilityRange()->getValue());
        } else if (!strcmp(fogType()->getValue(), "EXPONENTIAL")) {
            glFogi(GL_FOG_MODE, GL_EXP);
            glFogf(GL_FOG_DENSITY, 2.0f / visibilityRange()->getValue());
        }
        glFogfv(GL_FOG_COLOR, color()->getValue());
        glFogf(GL_FOG_DENSITY, 1);
        glEnable(GL_FOG);
    } else {
        glDisable(GL_FOG);
    }
}

int
NodeFog::getAnimationCommentID(void) 
{ 
    return IDS_ANIMATION_HELP_FOG + swGetLang();
}

