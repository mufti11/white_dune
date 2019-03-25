/*
 * EmitterNode.cpp
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

#include "EmitterNode.h"
#include "Proto.h"
#include "FieldValue.h"
#include "SFFloat.h"

EmitterProto::EmitterProto(Scene *scene,  const char *name)
  : Proto(scene, name)
{
    speed.set(
        addExposedField(SFFLOAT, "speed", new SFFloat(0), new SFFloat(0)));
    variation.set(
        addExposedField(SFFLOAT, "variation", new SFFloat(0.25),
                        new SFFloat(0), new SFFloat(1)));
    mass.set(
        addField(SFFLOAT, "mass", new SFFloat(0), new SFFloat(0)));
    surfaceArea.set(
        addField(SFFLOAT, "surfaceArea", new SFFloat(0)));
}

EmitterNode::EmitterNode(Scene *scene, Proto *def)
  : Node(scene, def)
{
}

