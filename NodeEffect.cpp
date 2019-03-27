/*
 * NodeEffect.cpp
 *
 * Copyright (C) 2011 J. "MUFTI" Scheurich
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

#include "NodeEffect.h"
#include "Proto.h"
#include "FieldValue.h"
#include "MFNode.h"
#include "SFString.h"
#include "DuneApp.h"

ProtoEffect::ProtoEffect(Scene *scene)
  : Proto(scene, "Effect")
{
    language.set(
        addField(SFSTRING, "language", new SFString("")));
    setFieldFlags(language, FF_STATIC);

    parts.set(
        addExposedField(MFNODE, "parts", new MFNode(), KAMBI_EFFECT_PART));
    setFieldFlags(parts, FF_STATIC);

    enabled.set(
        addExposedField(SFBOOL, "enabled", new SFBool(true)));
    setFieldFlags(enabled, FF_STATIC);

}

Node *
ProtoEffect::create(Scene *scene)
{ 
    return new NodeEffect(scene, this); 
}

NodeEffect::NodeEffect(Scene *scene, Proto *def)
  : DynamicFieldsNode(scene, new ProtoEffect(scene))
{
}

void
NodeEffect::update()  
{
    updateDynamicFields();
}

