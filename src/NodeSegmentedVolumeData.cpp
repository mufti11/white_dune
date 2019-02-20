/*
 * NodeSegmentedVolumeData.cpp
 *
 * Copyright (C) 2011 J. "MUFTI" Scheurich
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

#include "NodeSegmentedVolumeData.h"
#include "Proto.h"
#include "FieldValue.h"
#include "SFVec3f.h"
#include "MFNode.h"
#include "MFBool.h"
#include "SFNode.h"
#include "SFNode.h"
#include "SFVec3f.h"
#include "SFVec3f.h"
#include "DuneApp.h"

ProtoSegmentedVolumeData::ProtoSegmentedVolumeData(Scene *scene)
  : Proto(scene, "SegmentedVolumeData")
{
    dimensions.set(
        addExposedField(SFVEC3F, "dimensions", new SFVec3f(1, 1, 1)));
    renderStyle.set(
        addExposedField(MFNODE, "renderStyle", new MFNode(),
                        VOLUME_RENDER_STYLE_NODE));
    segmentEnabled.set(
        addExposedField(MFBOOL, "segmentEnabled", new MFBool()));
    segmentIdentifiers.set(
        addExposedField(SFNODE, "segmentIdentifiers", new SFNode(),
                        TEXTURE_3D_NODE));
    voxels.set(
        addExposedField(SFNODE, "voxels", new SFNode(), 
                        TEXTURE_3D_NODE));
    bboxCenter.set(
        addField(SFVEC3F, "bboxCenter", new SFVec3f(0, 0, 0)));
    bboxSize.set(
        addField(SFVEC3F, "bboxSize", new SFVec3f(-1, -1, -1)));

    allowViewpointInside.set(
          addExposedField(SFBOOL, "allowViewpointInside", new SFBool(true)));
    setFieldFlags(allowViewpointInside, FF_X3DOM_ONLY);

    appearance.set(
          addExposedField(SFNODE, "appearance", new SFNode(NULL), VRML_APPEARANCE));
    setFieldFlags(appearance, FF_X3DOM_ONLY);

    geometry.set(
          addExposedField(SFNODE, "geometry", new SFNode(NULL), GEOMETRY_NODE));
    setFieldFlags(geometry, FF_X3DOM_ONLY);

    isPickable.set(
          addExposedField(SFBOOL, "isPickable", new SFBool(true)));
    setFieldFlags(isPickable, FF_X3DOM_ONLY);

    numberOfMaxSegments.set(
          addExposedField(SFFLOAT, "numberOfMaxSegments", new SFFloat(0)));
    setFieldFlags(numberOfMaxSegments, FF_X3DOM_ONLY);

    render.set(
          addExposedField(SFBOOL, "render", new SFBool(true)));
    setFieldFlags(render, FF_X3DOM_ONLY);
}

Node *
ProtoSegmentedVolumeData::create(Scene *scene)
{ 
    return new NodeSegmentedVolumeData(scene, this); 
}

NodeSegmentedVolumeData::NodeSegmentedVolumeData(Scene *scene, Proto *def)
  : Node(scene, def)
{
    m_renderFlags = 0;
}
