/*
 * NodeTransform.cpp
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
#include <sys/types.h>
#include <sys/stat.h>
#ifndef _WIN32
# include <fcntl.h>
#endif

#include "swt.h"
#include "NodeTransform.h"
#include "Scene.h"
#include "Proto.h"
#include "FieldValue.h"
#include "SFVec3f.h"
#include "SFRotation.h"
#include "SFFloat.h"
#include "MFNode.h"
#include "FieldCommand.h"
#include "Matrix.h"
#include "Util.h"
#include "NodeViewpoint.h"
#include "NodeNavigationInfo.h"
#include "Field.h"
#include "ExposedField.h"

#include "RenderState.h"
#include "TransformFields.h"
#include "resource.h"

void ProtoTransform::addElements(int childrenNodeClass) 
{
    children.set (
           addExposedField(MFNODE, "children", new MFNode(), 
                           childrenNodeClass));

    render.set(
        addExposedField(SFBOOL, "render", new SFBool(true)));
    setFieldFlags(render, FF_X3DOM_ONLY);

    addEventIn(MFNODE, "addChildren");
    addEventIn(MFNODE, "removeChildren");
}

ProtoTransform::ProtoTransform(Scene *scene) 
  : TransformProto(scene, "Transform")
{                    
    addElements(CHILD_NODE);
}

ProtoTransform::ProtoTransform(Scene *scene, const char *name, 
                               int childrenNodeClass) 
  : TransformProto(scene, name)
{                    
    addElements(childrenNodeClass);
}

Node *
ProtoTransform::create(Scene *scene)
{ 
    return new NodeTransform(scene, this); 
}

NodeTransform::NodeTransform(Scene *scene, Proto *def)
  : TransformNode(scene, def)
{
    m_matrixDirty = true;
    m_matrix = Matrixd::identity();
}

NodeTransform::~NodeTransform()
{
}

void      
NodeTransform::setBoundingBox(void)
{
    TransformNode::setField(bboxSize_Field(), 
                            new SFVec3f(children()->getBboxSize()));
    TransformNode::setField(bboxCenter_Field(), 
                            new SFVec3f(children()->getBboxCenter()));
}

void
NodeTransform::setField(int field, FieldValue *value, int cf)
{
    bool setBbox = false;
    if (field == bboxSize_Field()) {
        Vec3f size = children()->getBboxSize();
        if ((size.x != -1) || (size.y != -1) || (size.z != -1)) {
            setBbox = true;
            setBoundingBox();
        }
    }
    if (!setBbox) { 
        if (field != getChildrenField()) 
            m_matrixDirty = true;
        if (field < children_Field())
            TransformNode::setField(field, value, cf);
        else
            Node::setField(field, value, cf);
    }
}

int
NodeTransform::getAnimationCommentID(void) 
{ 
    return IDS_ANIMATION_HELP_TRANSFORM + swGetLang(); 
}

int NodeTransform::getProfile(void) const
{ 
    if (((NodeTransform *)this)->hasInput("addChildren"))
        return PROFILE_INTERACTIVE;
    if (((NodeTransform *)this)->hasInput("removeChildren"))
        return PROFILE_INTERACTIVE;
    return PROFILE_INTERCHANGE; 
}
