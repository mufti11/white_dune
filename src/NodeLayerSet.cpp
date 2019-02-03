/*
 * NodeLayerSet.cpp
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

#include "NodeLayerSet.h"
#include "Proto.h"
#include "FieldValue.h"
#include "SFInt32.h"
#include "MFNode.h"
#include "MFInt32.h"
#include "DuneApp.h"

ProtoLayerSet::ProtoLayerSet(Scene *scene)
  : Proto(scene, "LayerSet")
{
    activeLayer.set(
        addExposedField(SFINT32, "activeLayer", new SFInt32(0)));
    layers.set(
        addExposedField(MFNODE, "layers", new MFNode(), LAYER_NODE));
    order.set(
        addExposedField(MFINT32, "order", new MFInt32(0)));
}

Node *
ProtoLayerSet::create(Scene *scene)
{ 
    return new NodeLayerSet(scene, this); 
}

NodeLayerSet::NodeLayerSet(Scene *scene, Proto *def)
  : Node(scene, def)
{
}

void    
NodeLayerSet::draw(int pass)
{
    NodeList *childList = layers()->getValues();
    int n = childList->size();

    glPushMatrix();

    glDepthMask(GL_FALSE);

    glPushName(layers_Field());  // field
    glPushName(0);                 // index
    for (int j = 0; j < order()->getSize(); j++) {
        int i = order()->getValue(j);
        if ((i < 0) || (i >= n))
            continue;
        glLoadName(i);
        if (childList->get(i) != this)
            childList->get(i)->draw(pass);
    }
    glPopName();
    glPopName();

    glDepthMask(GL_TRUE);

    glPopMatrix();    
}

