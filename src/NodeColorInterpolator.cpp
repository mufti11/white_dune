/*
 * NodeColorInterpolator.cpp
 *
 * Copyright (C) 1999 Stephen F. White
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

#include "NodeColorInterpolator.h"
#include "Proto.h"
#include "FieldValue.h"
#include "MFFloat.h"
#include "MFColor.h"
#include "Scene.h"
#include "MFieldCommand.h"
#include "SFColor.h"

ProtoColorInterpolator::ProtoColorInterpolator(Scene *scene)
  : ProtoInterpolator(scene, "ColorInterpolator", SFCOLOR, MFCOLOR, 
                      new MFColor())
{
}

Node *
ProtoColorInterpolator::create(Scene *scene)
{ 
    return new NodeColorInterpolator(scene, this); 
}

NodeColorInterpolator::NodeColorInterpolator(Scene *scene, Proto *def)
  : Interpolator(scene, def)
{
}

FieldValue *
NodeColorInterpolator::createKey(float *value) const
{
    FieldValue *r = new SFColor(value);
    return r;
}

FieldValue *
NodeColorInterpolator::createKeys(float *value, int numKeys) const
{
    return new MFColor(value, numKeys * 3);
}

void
NodeColorInterpolator::setKeyValue(int channel, int index, float value)
{
    value = CLAMP(value, 0.0f, 1.0f);
    Interpolator::setKeyValue(channel, index, value);
}

