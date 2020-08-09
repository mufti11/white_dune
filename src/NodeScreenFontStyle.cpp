/*
 * NodeScreenFontStyle.cpp
 *
 * Copyright (C) 1999 Stephen F. White, 2019 J. "MUFTI" Scheurich
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

#include "NodeScreenFontStyle.h"
#include "Proto.h"
#include "FieldValue.h"
#include "MFString.h"
#include "SFBool.h"
#include "SFString.h"
#include "SFFloat.h"
#include "Field.h"
#include "Scene.h"

ProtoScreenFontStyle::ProtoScreenFontStyle(Scene *scene)
  : Proto(scene, "ScreenFontStyle")
{
    static const char *families[] = { "SERIF", "SANS", "TYPEWRITER", NULL};
    family.set(
          addField(MFSTRING, "family", new MFString("SERIF"), 0, families));

    horizontal.set(
          addField(SFBOOL, "horizontal", new SFBool(true)));

    static const char *justifies[] = { "FIRST", "BEGIN", "MIDDLE", "END", NULL};
    justify.set(
          addField(MFSTRING, "justify", new MFString("BEGIN"), 
                   FF_FIXEDSTRINGS, justifies));

    language.set(
          addField(SFSTRING, "language", new SFString("")));

    leftToRight.set(
          addField(SFBOOL, "leftToRight", new SFBool(true)));

    pointSize.set(
          addField(SFFLOAT, "pointSize", new SFFloat(12.0f), 
                                         new SFFloat(0.0f)));

    spacing.set(
          addField(SFFLOAT, "spacing", new SFFloat(1.0f), new SFFloat(0.0f)));

    static const char *styles[] = { "PLAIN", "BOLD", "ITALIC", "BOLDITALIC", 
                                    NULL};
    style.set(
          addField(SFSTRING, "style", new SFString("PLAIN"), FF_FIXEDSTRINGS, 
                   styles));

    topToBottom.set(
          addField(SFBOOL, "topToBottom", new SFBool(true)));

    quality.set(
          addExposedField(SFFLOAT, "quality", new SFFloat(0)));
    setFieldFlags(quality, FF_X3DOM_ONLY);
}

Node *
ProtoScreenFontStyle::create(Scene *scene)
{ 
    return new NodeScreenFontStyle(scene, this);
}

NodeScreenFontStyle::NodeScreenFontStyle(Scene *scene, Proto *def)
    : Node(scene, def)
{
}

void
NodeScreenFontStyle::setField(int index, FieldValue *value, int cf)
{
    Node::setField(index, value, cf);
    if (hasParent())
        getParents().update();
}

float   
NodeScreenFontStyle::getSizeX(void) const 
{ 
     float sceneWidth = m_scene->getWidth();
     if (sceneWidth != 0)
         return pointSize()->getValue() / sceneWidth * 10.0f; // ??? 
     return 1;
}

float   
NodeScreenFontStyle::getSizeY(void) const 
{ 
     float sceneHeight = m_scene->getHeight();
     if (sceneHeight != 0)
         return pointSize()->getValue() / sceneHeight * 10.0f; // ??? 
     return 1;
}


