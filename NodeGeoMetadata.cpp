/*
 * NodeGeoMetadata.cpp
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

#include "NodeGeoMetadata.h"
#include "Proto.h"
#include "MFVec3f.h"
#include "ExposedField.h"
#include "Field.h"
#include "RenderState.h"
#include "DuneApp.h"
#include "Util.h"
#include "Vec3f.h"
#include "Scene.h"
#include "NodeIndexedFaceSet.h"
#include "NodeIndexedLineSet.h"
#include "NodePointSet.h"

ProtoGeoMetadata::ProtoGeoMetadata(Scene *scene)
  : Proto(scene, "GeoMetadata")
{
    data.set(
          addExposedField(MFNODE, "data", new MFNode(), METADATA_NODE));
    summary.set(
          addExposedField(MFSTRING, "summary", new MFString()));
    url.set(
          addExposedField(MFSTRING, "url", new MFString()));
}

Node *
ProtoGeoMetadata::create(Scene *scene)
{ 
    return new NodeGeoMetadata(scene, this); 
}

NodeGeoMetadata::NodeGeoMetadata(Scene *scene, Proto *def)
  : Node(scene, def)
{
}
