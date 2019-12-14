/*
 * NodeGroup.cpp
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

#include "stdafx.h"

#include "swt.h"
#include "NodeGroup.h"
#include "Proto.h"
#include "FieldValue.h"
#include "MFNode.h"
#include "SFVec3f.h"
#include "SFFloat.h"
#include "NodeNurbsGroup.h"
#include "NodeNurbsSet.h"
#include "NodeShape.h"
#include "NodeAnchor.h"
#include "Scene.h"

ProtoGroup::ProtoGroup(Scene *scene, const char *name)
  : StaticGroupProto(scene, name)
{
     addElements(); 
}

ProtoGroup::ProtoGroup(Scene *scene)
  : StaticGroupProto(scene, "Group")
{                    
     addElements();     
}

void ProtoGroup::addElements(void) 
{
    doPickPass.set(
        addExposedField(SFBOOL, "doPickPass", new SFBool(true)));
    setFieldFlags(doPickPass, FF_ROOT_ONLY | FF_X3DOM_ONLY);

    pickMode.set(
        addExposedField(SFSTRING, "pickMode", new SFString("idBuf")));
    setFieldFlags(pickMode, FF_ROOT_ONLY | FF_X3DOM_ONLY);

    shadowObjectIdMapping.set(
        addExposedField(SFSTRING, "shadowObjectIdMapping", new SFString("")));
    setFieldFlags(shadowObjectIdMapping, FF_ROOT_ONLY | FF_X3DOM_ONLY);

    render.set(
        addExposedField(SFBOOL, "render", new SFBool(true)));
    setFieldFlags(render, FF_X3DOM_ONLY);

    addEventIn(MFNODE, "addChildren");
    addEventIn(MFNODE, "removeChildren");
}

Node *
ProtoGroup::create(Scene *scene)
{ 
    return new NodeGroup(scene, this); 
}

NodeGroup::NodeGroup(Scene *scene, Proto *def)
  : StaticGroupNode(scene, def)
{
}

NodeList *
NodeGroup::buildVrml97Children(MFNode *children) 
{
    // function can be used both for NurbsGroup and NurbsSet
    NodeList *newChildren = new NodeList();

    for (int i = 0; i < children->getSize(); i++) {
        Node *node = children->getValue(i);
        newChildren->append(node);
    }
    return newChildren;
}

NodeGroup::NodeGroup(NodeNurbsGroup *nurbsGroup)
  : StaticGroupNode(nurbsGroup->getScene(), 
                    nurbsGroup->getScene()->getProto("Group"))
{
    children(new MFNode(buildVrml97Children(nurbsGroup->children())));
    bboxCenter(new SFVec3f(nurbsGroup->bboxCenter()->getValue()));
    bboxSize(new SFVec3f(nurbsGroup->bboxSize()->getValue()));
}

NodeGroup::NodeGroup(NodeNurbsSet *nurbsSet)
  : StaticGroupNode(nurbsSet->getScene(), 
                    nurbsSet->getScene()->getProto("Group"))
{
    children(new MFNode(buildVrml97Children(nurbsSet->geometry())));
    bboxCenter(new SFVec3f(nurbsSet->bboxCenter()->getValue()));
    bboxSize(new SFVec3f(nurbsSet->bboxSize()->getValue()));
}

NodeGroup::NodeGroup(NodeAnchor *anchor)
  : StaticGroupNode(anchor->getScene(), anchor->getScene()->getProto("Group"))
{
    children(new MFNode(anchor->children()));
    bboxCenter(new SFVec3f(anchor->bboxCenter()->getValue()));
    bboxSize(new SFVec3f(anchor->bboxSize()->getValue()));
}

void      
NodeGroup::setBoundingBox(void)
{
    GroupNode::setField(bboxSize_Field(), 
                        new SFVec3f(children()->getBboxSize()));
    GroupNode::setField(bboxCenter_Field(), 
                        new SFVec3f(children()->getBboxCenter()));
}

void 
NodeGroup::setField(int index, FieldValue *value, int cf)
{
    bool setBbox = false;
    if (index == bboxSize_Field()) {
        Vec3f size = children()->getBboxSize();
        if ((size.x != -1) || (size.y != -1) || (size.z != -1)) {
            setBbox = true;
            setBoundingBox();
        } 
    }
    if (!setBbox)
        GroupNode::setField(index, value, cf);
}

int NodeGroup::getProfile(void) const
{ 
    if (((NodeGroup *)this)->hasInput("addChildren"))
        return PROFILE_INTERACTIVE;
    if (((NodeGroup *)this)->hasInput("removeChildren"))
        return PROFILE_INTERACTIVE;
    return PROFILE_INTERCHANGE; 
}




