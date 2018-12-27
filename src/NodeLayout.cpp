/*
 * NodeLayout.cpp
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

#include "NodeLayout.h"
#include "Proto.h"
#include "FieldValue.h"
#include "MFString.h"
#include "MFFloat.h"
#include "MFString.h"
#include "StringArray.h"
#include "DuneApp.h"

ProtoLayout::ProtoLayout(Scene *scene)
  : Proto(scene, "Layout")
{
    StringArray *defaultalign = new StringArray();
    defaultalign->append("CENTER");
    defaultalign->append("CENTER");
    align.set(
        addExposedField(MFSTRING, "align", new MFString(defaultalign)));

    float offsetvalues[] = { 1.0f, 1.0f };
    float *o = new float[2];  
    memcpy(o, offsetvalues, 2 * sizeof(float));
    offset.set(
        addExposedField(MFFLOAT, "offset", new MFFloat(o, 2)));

    StringArray *defaultoffsetUnits = new StringArray();
    defaultoffsetUnits->append("WORLD");
    defaultoffsetUnits->append("WORLD");
    offsetUnits.set(
        addExposedField(MFSTRING, "offsetUnits", 
                        new MFString(defaultoffsetUnits)));

    StringArray *defaultscaleMode = new StringArray();
    defaultscaleMode->append("NONE");
    defaultscaleMode->append("NONE");
    scaleMode.set(
        addExposedField(MFSTRING, "scaleMode", new MFString(defaultscaleMode)));

    float sizevalues[] = { 1.0f, 1.0f };
    float *s = new float[2];  
    memcpy(s, sizevalues, 2 * sizeof(float));
    size.set(
        addExposedField(MFFLOAT, "size", new MFFloat(s, 2)));

    StringArray *defaultsizeUnits = new StringArray();
    defaultsizeUnits->append("WORLD");
    defaultsizeUnits->append("WORLD");
    sizeUnits.set(
        addExposedField(MFSTRING, "sizeUnits", new MFString(defaultsizeUnits)));
}

Node *
ProtoLayout::create(Scene *scene)
{ 
    return new NodeLayout(scene, this); 
}

NodeLayout::NodeLayout(Scene *scene, Proto *def)
  : Node(scene, def)
{
}
