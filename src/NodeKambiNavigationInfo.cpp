/*
 * NodeKambiNavigationInfo.cpp
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

#include "NodeKambiNavigationInfo.h"
#include "Proto.h"
#include "FieldValue.h"
#include "MFFloat.h"
#include "SFBool.h"
#include "SFFloat.h"
#include "SFTime.h"
#include "MFString.h"
#include "MFString.h"
#include "SFFloat.h"
#include "SFBool.h"
#include "SFNode.h"
#include "SFNode.h"
#include "SFNode.h"
#include "SFNode.h"
#include "DuneApp.h"

ProtoKambiNavigationInfo::ProtoKambiNavigationInfo(Scene *scene)
  : ProtoNavigationInfo(scene, "KambiNavigationInfo")
{
    timeOriginAtLoad.set(
        addField(SFBOOL, "timeOriginAtLoad", new SFBool(false)));
    octreeRendering.set(
        addField(SFNODE, "octreeRendering", new SFNode()));
    octreeDynamicCollisions.set(
        addField(SFNODE, "octreeDynamicCollisions", new SFNode(), 
                 KAMBI_KAMBI_OCTREE_PROPERTIES));
    octreeVisibleTriangles.set(
        addField(SFNODE, "octreeVisibleTriangles", new SFNode(),
                 KAMBI_KAMBI_OCTREE_PROPERTIES));
    octreeCollidableTriangles.set(
        addField(SFNODE, "octreeCollidableTriangles", new SFNode(),
                 KAMBI_KAMBI_OCTREE_PROPERTIES));
    headBobbing.set(
        addExposedField(SFFLOAT, "headBobbing", new SFFloat(0.02f)));
    headBobbingDistance.set(
        addExposedField(SFFLOAT, "headBobbingDistance", new SFFloat(20.0f)));
    setFieldFlags(headBobbingDistance, FF_KAMBI_DEPRECATED);
    headBobbingTime.set(
        addExposedField(SFFLOAT, "headBobbingTime", new SFFloat(0.5f)));
    headlightNode.set(
        addExposedField(SFNODE, "headlightNode", new SFNode(), LIGHT_NODE));
}

Node *
ProtoKambiNavigationInfo::create(Scene *scene)
{ 
    return new NodeKambiNavigationInfo(scene, this); 
}

NodeKambiNavigationInfo::NodeKambiNavigationInfo(Scene *scene, Proto *def)
  : NodeNavigationInfo(scene, def)
{
}
