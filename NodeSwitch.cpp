/*
 * NodeSwitch.cpp
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

#include "NodeSwitch.h"
#include "Proto.h"
#include "Scene.h"
#include "FieldValue.h"
#include "MFNode.h"
#include "SFInt32.h"

ProtoSwitch::ProtoSwitch(Scene *scene)
  : WonderlandExportProto(scene, "Switch")
{
    bboxCenter.set(
          addField(SFVEC3F, "bboxCenter", new SFVec3f(0.0f, 0.0f, 0.0f)));
    setFieldFlags(bboxCenter, FF_X3D_ONLY);
    bboxSize.set(
          addField(SFVEC3F, "bboxSize", new SFVec3f(-1.0f, -1.0f, -1.0f), 
                   new SFFloat(-1.0f)));
    setFieldFlags(bboxSize, FF_X3D_ONLY);
    choice.set(
          addExposedField(MFNODE, "choice", new MFNode(), CHILD_NODE, 
                          "children"));
    whichChoice.set(
          addExposedField(SFINT32, "whichChoice", new SFInt32(-1), 
                          new SFInt32(-1)));

    render.set(
        addExposedField(SFBOOL, "render", new SFBool(true)));
    setFieldFlags(render, FF_X3DOM_ONLY);

    addEventIn(MFNODE, "addChildren", FF_X3D_ONLY);
    addEventIn(MFNODE, "removeChildren", FF_X3D_ONLY);
}

Node *
ProtoSwitch::create(Scene *scene)
{ 
    return new NodeSwitch(scene, this); 
}

NodeSwitch::NodeSwitch(Scene *scene, Proto *def)
  : Node(scene, def)
{
}

int NodeSwitch::accountWhich()
{
    NodeList *choiceList = choice()->getValues();
    
    int which = whichChoice()->getValue();
    
    int whichCount = 0;
    for (int i = 0; i < choiceList->size(); i++)
        if (choiceList->get(i)->getType() != VRML_COMMENT) {
            if (whichCount == which)
                return i;
            else
                whichCount++;
        }
    return which;
}

void
NodeSwitch::preDraw()
{
    NodeList *choiceList = choice()->getValues();

    int which = accountWhich();

    if (which < 0 || which >= choiceList->size()) return;

    choiceList->get(which)->preDraw();
}

void
NodeSwitch::draw(int pass)
{
    NodeList *choiceList = choice()->getValues();

    int which = accountWhich();

    if (which < 0 || which >= choiceList->size()) return;

    glPushName(choice_Field());
    glPushName(which);
    choiceList->get(which)->draw(pass);
    glPopName();
    glPopName();
}

void
NodeSwitch::flip(int index)
{
    if (m_scene->isX3d())
        bboxCenter()->flip(index);
    choice()->flip(index);
}

void
NodeSwitch::swap(int fromTo)
{
    if (m_scene->isX3d()) {
        bboxCenter()->swap(fromTo);
        bboxSize()->swap(fromTo);
    }
    choice()->swap(fromTo);
}

// writing of file formats, which do not support switch-like constructs:
// if a valid choice is selected, write this choice, else write all in choice field

int
NodeSwitch::writeAc3d(int filedes, int indent)
{
    int which = accountWhich();
    if (which < 0 || which >= choice()->getValues()->size())
        return choice()->writeAc3d(filedes, indent);
    return choice()->getValues()->get(which)->writeAc3d(filedes, indent);
}

int
NodeSwitch::writeRib(int filedes, int indent)
{
    int which = accountWhich();
    if (which > -1 && which < choice()->getValues()->size())
        return choice()->getValues()->get(which)->writeRib(filedes, indent);
    return 0; 
}

int
NodeSwitch::writeCattGeo(int filedes, int indent)
{
    int which = accountWhich();
    if (which < 0 || which >= choice()->getValues()->size())
        return choice()->getValues()->writeCattGeo(this, filedes, indent);
    NodeList tempNode(choice()->getValue(which));
    // The following is a dangerous construct. 
    // Works only, cause writeCattGeo gets only things like name, scene etc. from node
    return tempNode.writeCattGeo(this, filedes, indent);
}
