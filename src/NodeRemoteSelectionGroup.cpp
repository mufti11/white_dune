/*
 * NodeRemoteSelectionGroup.cpp
 *
 * Copyright (C) 2018 J. "MUFTI" Scheurich
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

#include "NodeRemoteSelectionGroup.h"
#include "Proto.h"
#include "FieldValue.h"
#include "MFNode.h"
#include "MFNode.h"
#include "MFNode.h"
#include "MFFloat.h"
#include "SFVec3f.h"
#include "SFVec3f.h"
#include "DuneApp.h"

ProtoRemoteSelectionGroup::ProtoRemoteSelectionGroup(Scene *scene)
  : ProtoGroup(scene, "RemoteSelectionGroup")
{
    enableCulling.set(
        addExposedField(SFBOOL, "enableCulling", new SFBool(true)));
    setFieldFlags(enableCulling, FF_X3DOM_ONLY);

    invisibleNodes.set(
        addExposedField(MFSTRING, "invisibleNodes", new MFString()));
    setFieldFlags(invisibleNodes, FF_X3DOM_ONLY);

    label.set(
        addExposedField(MFSTRING, "label", new MFString()));
    setFieldFlags(label, FF_X3DOM_ONLY);

    maxRenderedIds.set(
        addExposedField(SFINT32, "maxRenderedIds", new SFInt32(-1)));
    setFieldFlags(maxRenderedIds, FF_X3DOM_ONLY);

    reconnect.set(
        addExposedField(SFBOOL, "reconnect", new SFBool(true)));
    setFieldFlags(reconnect, FF_X3DOM_ONLY);

    render.set(
        addExposedField(SFBOOL, "render", new SFBool(true)));
    setFieldFlags(render, FF_X3DOM_ONLY);

    scaleRenderedIdsOnMove.set(
        addExposedField(SFFLOAT, "scaleRenderedIdsOnMove", new SFFloat(0),
                        new SFFloat(0), new SFFloat(1)));
    setFieldFlags(scaleRenderedIdsOnMove, FF_X3DOM_ONLY);

    url.set(
        addExposedField(MFSTRING, "url", 
                        new MFString("ws://localhost:35668/cstreams/0")));
    setFieldFlags(url, FF_X3DOM_ONLY);
}

Node *
ProtoRemoteSelectionGroup::create(Scene *scene)
{ 
    return new NodeRemoteSelectionGroup(scene, this); 
}

NodeRemoteSelectionGroup::NodeRemoteSelectionGroup(Scene *scene, Proto *def)
  : NodeGroup(scene, def)
{
}
