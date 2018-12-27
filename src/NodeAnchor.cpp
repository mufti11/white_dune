/*
 * NodeAnchor.cpp
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
#include "NodeAnchor.h"
#include "FieldValue.h"
#include "MFNode.h"
#include "MFString.h"
#include "SFString.h"
#include "SFVec3f.h"
#include "SFFloat.h"
#include "Field.h" // for FF_URL

ProtoAnchor::ProtoAnchor(Scene *scene)
  : ProtoGroup(scene, "Anchor")
{
    description.set(
         addExposedField(SFSTRING, "description", new SFString("")));

    parameter.set(
         addExposedField(MFSTRING, "parameter", new MFString()));

    url.set(
        addExposedField(MFSTRING, "url", new MFString(), FF_URL, NULL));

    render.set(
        addExposedField(SFBOOL, "render", new SFBool(true)));
    setFieldFlags(render, FF_X3DOM_ONLY);
}

Node *
ProtoAnchor::create(Scene *scene)
{ 
    return new NodeAnchor(scene, this); 
}

NodeAnchor::NodeAnchor(Scene *scene, Proto *proto)
  : NodeGroup(scene, proto)
{
}
