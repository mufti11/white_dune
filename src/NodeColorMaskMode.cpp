/*
 * NodeColorMaskMode.cpp
 *
 * Copyright (C) 1999 Stephen F. White, 2018 J. "MUFTI" Scheurich
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

#include "NodeColorMaskMode.h"
#include "SFBool.h"

ProtoColorMaskMode::ProtoColorMaskMode(Scene *scene)
  : Proto(scene, "ColorMaskMode")
{
    maskA.set(
          addExposedField(SFBOOL, "maskA", new SFBool(true)));
    setFieldFlags(maskA, FF_X3DOM_ONLY);

    maskB.set(
          addExposedField(SFBOOL, "maskB", new SFBool(true)));
    setFieldFlags(maskB, FF_X3DOM_ONLY);

    maskG.set(
          addExposedField(SFBOOL, "maskG", new SFBool(true)));
    setFieldFlags(maskG, FF_X3DOM_ONLY);

    maskR.set(
          addExposedField(SFBOOL, "maskR", new SFBool(true)));
    setFieldFlags(maskR, FF_X3DOM_ONLY);
}

Node *
ProtoColorMaskMode::create(Scene *scene)
{ 
    return new NodeColorMaskMode(scene, this); 
}

NodeColorMaskMode::NodeColorMaskMode(Scene *scene, Proto *def)
  : Node(scene, def)
{
}
