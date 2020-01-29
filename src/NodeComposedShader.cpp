/*
 * NodeComposedShader.cpp
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

#include "NodeComposedShader.h"
#include "Proto.h"
#include "FieldValue.h"
#include "MFNode.h"
#include "SFString.h"
#include "DuneApp.h"

ProtoComposedShader::ProtoComposedShader(Scene *scene)
  : DynamicFieldsProto(scene, "ComposedShader")
{
    addEventIn(SFBOOL, "activate", FF_STATIC);

    parts.set(
        addExposedField(MFNODE, "parts", new MFNode(), X3D_SHADER_PART));
    setFieldFlags(parts, FF_STATIC);

    language.set(
        addField(SFSTRING, "language", new SFString("")));
    setFieldFlags(language, FF_STATIC);

    effects.set(
        addField(MFNODE, "effects", new MFNode(), KAMBI_EFFECT));
    setFieldFlags(effects, FF_KAMBI_ONLY | FF_STATIC);

    addEventOut(SFBOOL, "isSelected", FF_STATIC);
    addEventOut(SFBOOL, "isValid", FF_STATIC);

}

Node *
ProtoComposedShader::create(Scene *scene)
{ 
    return new NodeComposedShader(scene, this); 
}

NodeComposedShader::NodeComposedShader(Scene *scene, Proto *def)
  : DynamicFieldsNode(scene, new ProtoComposedShader(scene))
{
}

void
NodeComposedShader::update()  
{
    updateDynamicFields();
}

