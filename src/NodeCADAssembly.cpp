/*
 * NodeCADAssembly.cpp
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

#include "NodeCADAssembly.h"
#include "Proto.h"
#include "FieldValue.h"
#include "MFNode.h"
#include "SFString.h"
#include "SFVec3f.h"
#include "SFVec3f.h"
#include "DuneApp.h"

ProtoCADAssembly::ProtoCADAssembly(Scene *scene)
  : StaticGroupProto(scene, "CADAssembly", PRODUCT_STRUCTURE_CHILD_NODE)
{
    name.set(
          addExposedField(SFSTRING, "name", new SFString("")));

    render.set(
        addExposedField(SFBOOL, "render", new SFBool(true)));
    setFieldFlags(render, FF_X3DOM_ONLY);
}

Node *
ProtoCADAssembly::create(Scene *scene)
{ 
    return new NodeCADAssembly(scene, this); 
}

NodeCADAssembly::NodeCADAssembly(Scene *scene, Proto *def)
  : StaticGroupNode(scene, def)
{
}
