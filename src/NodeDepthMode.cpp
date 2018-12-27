/*
 * NodeDepthMode.cpp
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

#include "NodeDepthMode.h"
#include "SFBool.h"

ProtoDepthMode::ProtoDepthMode(Scene *scene)
  : Proto(scene, "DepthMode")
{
    depthFunc.set(
          addExposedField(SFSTRING, "depthFunc", new SFString("never")));
    setFieldFlags(depthFunc, FF_X3DOM_ONLY);

    enableDepthTest.set(
          addExposedField(SFBOOL, "enableDepthTest", new SFBool(true)));
    setFieldFlags(enableDepthTest, FF_X3DOM_ONLY);

    readOnly.set(
          addExposedField(SFBOOL, "readOnly", new SFBool(false)));
    setFieldFlags(readOnly, FF_X3DOM_ONLY);

    zFarRange.set(
          addExposedField(SFFLOAT, "zFarRange", new SFFloat(-1)));
    setFieldFlags(zFarRange, FF_X3DOM_ONLY);

    zNearRange.set(
          addExposedField(SFFLOAT, "zNearRange", new SFFloat(-1)));
    setFieldFlags(zNearRange, FF_X3DOM_ONLY);
}

Node *
ProtoDepthMode::create(Scene *scene)
{ 
    return new NodeDepthMode(scene, this); 
}

NodeDepthMode::NodeDepthMode(Scene *scene, Proto *def)
  : Node(scene, def)
{
}
