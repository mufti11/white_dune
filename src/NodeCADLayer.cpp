/*
 * NodeCADLayer.cpp
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

#include "NodeCADLayer.h"
#include "Proto.h"
#include "FieldValue.h"
#include "MFNode.h"
#include "SFString.h"
#include "MFBool.h"
#include "SFVec3f.h"
#include "SFVec3f.h"
#include "DuneApp.h"

ProtoCADLayer::ProtoCADLayer(Scene *scene)
  : StaticGroupProto(scene, "CADLayer")
{
    name.set(
          addExposedField(SFSTRING, "name", new SFString("")));
    visible.set(
          addExposedField(MFBOOL, "visible", new MFBool()));

    render.set(
        addExposedField(SFBOOL, "render", new SFBool(true)));
    setFieldFlags(render, FF_X3DOM_ONLY);
}

Node *
ProtoCADLayer::create(Scene *scene)
{ 
    return new NodeCADLayer(scene, this); 
}

NodeCADLayer::NodeCADLayer(Scene *scene, Proto *def)
  : StaticGroupNode(scene, def)
{
}

void
NodeCADLayer::preDraw()
{
    for (int i = 0; i < children()->getSize(); i++)
        if (i >= visible()->getSize())
            children()->getValue(i)->preDraw();
        else if (visible()->getValue(i)) 
            children()->getValue(i)->preDraw();
}

void
NodeCADLayer::draw(int pass)
{ 
    glPushName(children_Field());  // field offset

    for (int i = 0; i < children()->getSize(); i++)
        if (i >= visible()->getSize())
            children()->getValue(i)->bind();
        else if (visible()->getValue(i)) 
            children()->getValue(i)->bind();

    glPushName(0);
    for (int i = 0; i < children()->getSize(); i++)
        if (i >= visible()->getSize()) {
             glLoadName(i);
            children()->getValue(i)->draw(pass);
        } else if (visible()->getValue(i)) {
            glLoadName(i);
            children()->getValue(i)->draw(pass);
        }
    glPopName();

    for (int i = 0; i < children()->getSize(); i++)
        if (i >= visible()->getSize())
            children()->getValue(i)->unbind();
        else if (visible()->getValue(i)) 
            children()->getValue(i)->unbind();
    glPopName();
}

