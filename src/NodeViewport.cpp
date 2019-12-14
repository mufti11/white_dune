/*
 * NodeViewport.cpp
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

#include "NodeViewport.h"
#include "Proto.h"
#include "FieldValue.h"
#include "MFNode.h"
#include "MFNode.h"
#include "MFNode.h"
#include "MFFloat.h"
#include "SFVec3f.h"
#include "SFVec3f.h"
#include "DuneApp.h"
#include "Scene.h"

ProtoViewport::ProtoViewport(Scene *scene)
  : ProtoGroup(scene, "Viewport")
{
    float boundary[] = { 0, 1, 0, 1 }; 
    float *b = new float[4];  
    memcpy(b, boundary, 4 * sizeof(float));
    clipBoundary.set(
        addExposedField(MFFLOAT, "clipBoundary", new MFFloat(b, 4)));
}

Node *
ProtoViewport::create(Scene *scene)
{ 
    return new NodeViewport(scene, this); 
}

NodeViewport::NodeViewport(Scene *scene, Proto *def)
  : NodeGroup(scene, def)
{
}

void        
NodeViewport::setField(int index, FieldValue *value, int containerField)
{
    m_scene->setViewPorts();
    NodeGroup::setField(index, value, containerField);
}

void
NodeViewport::preDraw() 
{ 
    if (m_scene->getDrawViewports())
        children()->preDraw(); 
}
void
NodeViewport::draw(int pass) 
{ 
    if (m_scene->getDrawViewports())
        children()->draw(pass, children_Field()); 
}



