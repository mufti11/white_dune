/*
 * NodeBlendMode.cpp
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

#include "NodeBlendMode.h"
#include "Proto.h"
#include "FieldValue.h"
#include "SFString.h"
#include "SFString.h"
#include "SFColor.h"
#include "SFFloat.h"
#include "DuneApp.h"

ProtoBlendMode::ProtoBlendMode(Scene *scene)
  : Proto(scene, "BlendMode")
{
    srcFactor.set(
        addExposedField(SFSTRING, "srcFactor", new SFString("src_alpha")));
    setFieldFlags(srcFactor, FF_KAMBI_ONLY | FF_X3DOM_ONLY);
    destFactor.set(
        addExposedField(SFSTRING, "destFactor", new SFString("one_minus_src_alpha")));
    setFieldFlags(destFactor, FF_KAMBI_ONLY | FF_X3DOM_ONLY);
    color.set(
        addExposedField(SFCOLOR, "color", new SFColor(1, 1, 1)));
    setFieldFlags(color, FF_KAMBI_ONLY | FF_X3DOM_ONLY);
    colorTransparency.set(
        addExposedField(SFFLOAT, "colorTransparency", new SFFloat(0)));
    setFieldFlags(colorTransparency, FF_KAMBI_ONLY | FF_X3DOM_ONLY);

    alphaFunc.set(
        addExposedField(SFSTRING, "alphaFunc", new SFString("none")));
    setFieldFlags(alphaFunc, FF_X3DOM_ONLY);
    alphaFuncValue.set(
        addExposedField(SFFLOAT, "alphaFuncValue", new SFFloat(0)));
    setFieldFlags(alphaFuncValue, FF_X3DOM_ONLY);
    equation.set(
        addExposedField(SFSTRING, "equation", new SFString("none")));
    setFieldFlags(equation, FF_X3DOM_ONLY);
}

Node *
ProtoBlendMode::create(Scene *scene)
{ 
    return new NodeBlendMode(scene, this); 
}

NodeBlendMode::NodeBlendMode(Scene *scene, Proto *def)
  : Node(scene, def)
{
}
