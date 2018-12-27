/*
 * NodeScreenFontStyle.cpp
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

#include "NodeScreenFontStyle.h"
#include "Proto.h"
#include "FieldValue.h"
#include "MFString.h"
#include "SFBool.h"
#include "MFString.h"
#include "SFString.h"
#include "SFBool.h"
#include "SFFloat.h"
#include "SFFloat.h"
#include "SFString.h"
#include "SFBool.h"
#include "DuneApp.h"

ProtoScreenFontStyle::ProtoScreenFontStyle(Scene *scene)
  : Proto(scene, "ScreenFontStyle")
{
    family.set(
        addField(MFSTRING, "family", new MFString("SERIF")));
    horizontal.set(
        addField(SFBOOL, "horizontal", new SFBool(true)));
    justify.set(
        addField(MFSTRING, "justify", new MFString("BEGIN")));
    language.set(
        addField(SFSTRING, "language", new SFString("")));
    leftToRight.set(
        addField(SFBOOL, "leftToRight", new SFBool(true)));
    pointSize.set(
        addField(SFFLOAT, "pointSize", new SFFloat(12.0)));
    spacing.set(
        addField(SFFLOAT, "spacing", new SFFloat(1.0)));
    style.set(
        addField(SFSTRING, "style", new SFString("PLAIN")));
    topToBottom.set(
        addField(SFBOOL, "topToBottom", new SFBool(true)));
}

Node *
ProtoScreenFontStyle::create(Scene *scene)
{ 
    return new NodeScreenFontStyle(scene, this); 
}

NodeScreenFontStyle::NodeScreenFontStyle(Scene *scene, Proto *def)
  : Node(scene, def)
{
}
