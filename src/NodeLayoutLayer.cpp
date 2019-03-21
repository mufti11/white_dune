/*
 * NodeLayoutLayer.cpp
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

#include "NodeLayoutLayer.h"
#include "Proto.h"
#include "FieldValue.h"
#include "MFNode.h"
#include "MFNode.h"
#include "MFNode.h"
#include "SFBool.h"
#include "SFNode.h"
#include "SFNode.h"
#include "DuneApp.h"

ProtoLayoutLayer::ProtoLayoutLayer(Scene *scene)
  : Proto(scene, "LayoutLayer")
{
    addEventIn(MFNODE, "addChildren");
    addEventIn(MFNODE, "removeChildren");
    children.set(
        addExposedField(MFNODE, "children", new MFNode(), CHILD_NODE));
    isPickable.set(
        addExposedField(SFBOOL, "isPickable", new SFBool(true)));
    layout.set(
        addExposedField(SFNODE, "layout", new SFNode(), X3D_LAYOUT));
    viewport.set(
        addExposedField(SFNODE, "viewport", new SFNode(), X3D_VIEWPORT));
}

Node *
ProtoLayoutLayer::create(Scene *scene)
{ 
    return new NodeLayoutLayer(scene, this); 
}

NodeLayoutLayer::NodeLayoutLayer(Scene *scene, Proto *def)
  : Node(scene, def)
{
}

void    
NodeLayoutLayer::preDraw() 
{ 
    if (!m_scene->getDrawViewports())
        children()->preDraw();
}

void    
NodeLayoutLayer::draw(int pass) 
{ 
    if (m_scene->getDrawViewports()) {
        glPushMatrix();

        glMatrixMode(GL_MODELVIEW);
        Vec3f pos = m_scene->getCamera()->getPosition();
        glTranslatef(pos.x, pos.y, pos.z);
        Quaternion quat = m_scene->getCamera()->getOrientation();
        SFRotation rot(quat);
        const float *frotation = rot.getValue();
        glRotatef(RAD2DEG(frotation[3]),
                  frotation[0], frotation[1], frotation[2]);

        glTranslatef(0, 0, -1.36);

        children()->draw(pass, children_Field()); 

        glPopMatrix();
    }
}


