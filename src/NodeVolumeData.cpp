/*
 * NodeVolumeData.cpp
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

#include "NodeVolumeData.h"
#include "Proto.h"
#include "FieldValue.h"
#include "SFVec3f.h"
#include "SFNode.h"
#include "SFNode.h"
#include "SFVec3f.h"
#include "SFVec3f.h"
#include "DuneApp.h"
#include "Scene.h"
#include "NodeImageTexture3D.h"
#include "NodePixelTexture3D.h"

ProtoVolumeData::ProtoVolumeData(Scene *scene)
  : Proto(scene, "VolumeData")
{
    dimensions.set(
        addExposedField(SFVEC3F, "dimensions", new SFVec3f(1, 1, 1)));
    renderStyle.set(
        addExposedField(SFNODE, "renderStyle", new SFNode(),
                        VOLUME_RENDER_STYLE_NODE));
    voxels.set(
        addExposedField(SFNODE, "voxels", new SFNode(),
                        TEXTURE_3D_NODE));
    bboxCenter.set(
        addField(SFVEC3F, "bboxCenter", new SFVec3f(0, 0, 0)));
    bboxSize.set(
        addField(SFVEC3F, "bboxSize", new SFVec3f(-1, -1, -1)));

    allowViewpointInside.set(
        addField(SFBOOL, "allowViewpointInside", new SFBool(true)));
    setFieldFlags(allowViewpointInside, FF_X3D_ONLY);

    appearance.set(
        addExposedField(SFNODE, "appearance", new SFNode(),
                        VRML_APPEARANCE));
    setFieldFlags(appearance, FF_X3D_ONLY);

    isPickable.set(
        addField(SFBOOL, "isPickable", new SFBool(true)));
    setFieldFlags(isPickable, FF_X3D_ONLY);

    render.set(
        addField(SFBOOL, "render", new SFBool(true)));
    setFieldFlags(render, FF_X3D_ONLY);
}

Node *
ProtoVolumeData::create(Scene *scene)
{ 
    return new NodeVolumeData(scene, this); 
}

NodeVolumeData::NodeVolumeData(Scene *scene, Proto *def)
  : Node(scene, def)
{
    m_boxtris = NULL;
}

void
NodeVolumeData::preDraw()
{
    Node *node = voxels()->getValue();
    if (node && (node->getType() == X3D_IMAGE_TEXTURE_3D)) {
        ((NodeImageTexture3D *)node)->preDraw();
    }
    if (node && (node->getType() == X3D_PIXEL_TEXTURE_3D)) {
        ((NodePixelTexture3D *)node)->preDraw();
    }
}

void
NodeVolumeData::draw(int pass)
{
    Node *node = voxels()->getValue();
    if (node && (node->getType() == X3D_IMAGE_TEXTURE_3D)) {
        ((NodeImageTexture3D *)node)->draw(pass);
    }
    if (node && (node->getType() == X3D_PIXEL_TEXTURE_3D)) {
        ((NodePixelTexture3D *)node)->draw(pass);
    }
}


