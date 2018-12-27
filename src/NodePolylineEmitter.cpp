/*
 * NodePolylineEmitter.cpp
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

#include "NodePolylineEmitter.h"
#include "Proto.h"
#include "FieldValue.h"
#include "SFNode.h"
#include "SFVec3f.h"
#include "SFFloat.h"
#include "MFInt32.h"
#include "DuneApp.h"

ProtoPolylineEmitter::ProtoPolylineEmitter(Scene *scene)
  : EmitterProto(scene, "PolylineEmitter")
{
    coord.set(
        addExposedField(SFNODE, "coord", new SFNode(), COORDINATE_NODE));
    direction.set(
        addExposedField(SFVEC3F, "direction", new SFVec3f(0, 1, 0)));
    coordIndex.set(
        addField(MFINT32, "coordIndex", new MFInt32(-1)));
}

Node *
ProtoPolylineEmitter::create(Scene *scene)
{ 
    return new NodePolylineEmitter(scene, this); 
}

NodePolylineEmitter::NodePolylineEmitter(Scene *scene, Proto *def)
  : EmitterNode(scene, def)
{
}
