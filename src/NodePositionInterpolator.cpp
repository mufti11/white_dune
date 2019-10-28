/*
 * NodePositionInterpolator.cpp
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

#include "NodePositionInterpolator.h"
#include "Proto.h"
#include "FieldValue.h"
#include "MFFloat.h"
#include "MFVec3f.h"
#include "Scene.h"
#include "MFieldCommand.h"
#include "FieldCommand.h"
#include "CommandList.h"
#include "SFVec3f.h"

ProtoPositionInterpolator::ProtoPositionInterpolator(Scene *scene)
  : ProtoInterpolator(scene, "PositionInterpolator", SFVEC3F, MFVEC3F,
                      new MFVec3f())
{
}

Node *
ProtoPositionInterpolator::create(Scene *scene)
{ 
    return new NodePositionInterpolator(scene, this);
}

NodePositionInterpolator::NodePositionInterpolator(Scene *scene, Proto *def)
  : Interpolator(scene, def)
{
    m_isInternal = false;   
}

FieldValue *
NodePositionInterpolator::createKey(void *value) const
{
    FieldValue *r = new SFVec3f((float *)value);
    return r;
}

FieldValue *
NodePositionInterpolator::createKeys(void *value, int numKeys) const
{
    return new MFVec3f((float *)value, numKeys * 3);
}

void
NodePositionInterpolator::setField(int index, FieldValue *value, int cf)
{
    Node::setField(index, value, cf);
    if (m_isInternal && hasParent()) {
        for (int i = 0; i < getNumParents(); i++)
            getParent(i)->update();
    }
}
