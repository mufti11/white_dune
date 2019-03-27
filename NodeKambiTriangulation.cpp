/*
 * NodeKambiTriangulation.cpp
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

#include "NodeKambiTriangulation.h"
#include "Proto.h"
#include "FieldValue.h"
#include "MFString.h"
#include "SFString.h"
#include "DuneApp.h"

ProtoKambiTriangulation::ProtoKambiTriangulation(Scene *scene)
  : Proto(scene, "KambiTriangulation")
{
    quadricSlices.set(
          addExposedField(SFINT32, "quadricSlices", new SFInt32(-1), 
                          new SFInt32(-1)));
    quadricStacks.set(
          addExposedField(SFINT32, "quadricStacks", new SFInt32(-1), 
                          new SFInt32(-1)));
    rectDivisions.set(
          addExposedField(SFINT32, "rectDivisions", new SFInt32(-1), 
                          new SFInt32(-1)));

    addURLs(URL_KAMBI);
}

Node *
ProtoKambiTriangulation::create(Scene *scene)
{ 
    return new NodeKambiTriangulation(scene, this); 
}

NodeKambiTriangulation::NodeKambiTriangulation(Scene *scene, Proto *def)
  : Node(scene, def)
{
}

