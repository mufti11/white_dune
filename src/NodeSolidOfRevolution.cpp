/*
 * NodeSolidOfRevolution.cpp
 *
 * Copyright (C) 1999 Stephen F. White, 2004 Wu qingwei, 2018 J. "MUFTI" Scheurich
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

#include <math.h>

#include "NodeSolidOfRevolution.h"
#include "Proto.h"

ProtoSolidOfRevolution::ProtoSolidOfRevolution(Scene *scene)
  : GeometryProto(scene, "SolidOfRevolution")
{
    angle.set(
          addExposedField(SFFLOAT, "angle", new SFFloat(M_PI)));
    setFieldFlags(angle, FF_X3DOM_ONLY);

    ccw.set(
          addExposedField(SFBOOL, "ccw", new SFBool(true)));
    setFieldFlags(ccw, FF_X3DOM_ONLY);

    caps.set(
          addExposedField(SFBOOL, "caps", new SFBool(true)));
    setFieldFlags(caps, FF_X3DOM_ONLY);

    creaseAngle.set(
          addExposedField(SFFLOAT, "creaseAngle", new SFFloat(0.0f), 
                   new SFFloat(0.0f)));
    setFieldFlags(creaseAngle, FF_X3DOM_ONLY);

    crossSection.set(
          addExposedField(MFVEC2F, "crossSection", new MFVec2f()));
    setFieldFlags(crossSection, FF_X3DOM_ONLY);
    
    solid.set(
          addExposedField(SFBOOL, "solid", new SFBool(true)));
    setFieldFlags(solid, FF_X3DOM_ONLY);

    subdivision.set(
          addExposedField(SFFLOAT, "subdivision", new SFFloat(32.0f)));
    setFieldFlags(subdivision, FF_X3DOM_ONLY);

    x3domGeometryCommonFields()
}

Node *
ProtoSolidOfRevolution::create(Scene *scene)
{ 
    return new NodeSolidOfRevolution(scene, this); 
}

NodeSolidOfRevolution::NodeSolidOfRevolution(Scene *scene, Proto *def)
  : Node(scene, def)
{
}
