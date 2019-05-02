/*
 * NodeBlock.cpp
 *
 * Copyright (C) 2018 J. "MUFTI" Scheurich
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

#include "NodeBlock.h"
#include "Proto.h"
#include "FieldValue.h"
#include "MFNode.h"
#include "MFNode.h"
#include "MFNode.h"
#include "MFFloat.h"
#include "SFVec3f.h"
#include "SFVec3f.h"
#include "DuneApp.h"

ProtoBlock::ProtoBlock(Scene *scene)
  : ProtoGroup(scene, "Block")
{
    nameSpaceName.set(
        addExposedField(MFSTRING, "nameSpaceName", new MFString()));
    setFieldFlags(nameSpaceName, FF_X3DOM_ONLY);
}

Node *
ProtoBlock::create(Scene *scene)
{ 
    return new NodeBlock(scene, this); 
}

NodeBlock::NodeBlock(Scene *scene, Proto *def)
  : NodeGroup(scene, def)
{
}
