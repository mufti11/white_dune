/*
 * NodeScalarInterpolator.cpp
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

#include "NodeScalarInterpolator.h"
#include "Proto.h"
#include "FieldValue.h"
#include "MFFloat.h"
#include "SFFloat.h"

 
ProtoScalarInterpolator::ProtoScalarInterpolator(Scene *scene)
  : ProtoInterpolator(scene, "ScalarInterpolator", SFFLOAT, MFFLOAT,
                      new MFFloat())
{
}

Node *
ProtoScalarInterpolator::create(Scene *scene)
{ 
    return new NodeScalarInterpolator(scene, this); 
}

NodeScalarInterpolator::NodeScalarInterpolator(Scene *scene, Proto *def)
  : Interpolator(scene, def)
{
}

FieldValue *
NodeScalarInterpolator::createKey(void *value) const
{
    FieldValue *r = new SFFloat(((float *)value)[0]);
    return r;
}

FieldValue *
NodeScalarInterpolator::createKeys(void *value, int numKeys) const
{
    return new MFFloat((float *)value, numKeys);
}
