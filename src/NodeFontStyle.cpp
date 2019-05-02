/*
 * NodeFontStyle.cpp
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

#include "NodeFontStyle.h"
#include "Proto.h"
#include "FieldValue.h"
#include "MFString.h"
#include "SFBool.h"
#include "SFString.h"
#include "SFFloat.h"
#include "Field.h"

ProtoFontStyle::ProtoFontStyle(Scene *scene)
  : FontStyleProto(scene, "FontStyle")
{
    size.set(
          addField(SFFLOAT, "size", new SFFloat(1.0f), new SFFloat(0.0f)));
}

Node *
ProtoFontStyle::create(Scene *scene)
{ 
    return new NodeFontStyle(scene, this);
}

NodeFontStyle::NodeFontStyle(Scene *scene, Proto *def)
  : FontStyleNode(scene, def)
{
}

void
NodeFontStyle::setField(int index, FieldValue *value, int cf)
{
    Node::setField(index, value, cf);
    if (hasParent())
        getParent()->update();
}


