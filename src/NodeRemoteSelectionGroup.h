/*
 * NodeRemoteSelectionGroup.h
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

#ifndef _NODE_REMOTE_SELECTION_GROUP_H
#define _NODE_REMOTE_SELECTION_GROUP_H

#ifndef _NODE_H
#include "ExternTheApp.h"
#include "Node.h"
#endif
#ifndef _PROTO_MACROS_H
#include "ProtoMacros.h"
#endif
#ifndef _PROTO_H
#include "Proto.h"
#endif

#include "NodeGroup.h"

#include "SFMFTypes.h"

class ProtoRemoteSelectionGroup : public ProtoGroup {
public:
                    ProtoRemoteSelectionGroup(Scene *scene);
    virtual Node   *create(Scene *scene);

    virtual int     getType() const { return X3DOM_REMOTE_SELECTION_GROUP; }
    virtual int     getNodeClass() const { return GROUPING_NODE; }

    virtual bool    isX3domProto(void) { return true; }

    FieldIndex enableCulling;
    FieldIndex invisibleNodes;
    FieldIndex label;
    FieldIndex maxRenderedIds;
    FieldIndex reconnect;
    FieldIndex render;
    FieldIndex scaleRenderedIdsOnMove;
    FieldIndex url;
};

class NodeRemoteSelectionGroup : public NodeGroup {
public:
                    NodeRemoteSelectionGroup(Scene *scene, Proto *proto);

    virtual Node *copy() { return new NodeRemoteSelectionGroup(
                                  m_scene, m_proto); }

    fieldMacros(SFBool, enableCulling, ProtoRemoteSelectionGroup)
    fieldMacros(MFString, invisibleNodes, ProtoRemoteSelectionGroup)
    fieldMacros(MFString, label, ProtoRemoteSelectionGroup)
    fieldMacros(SFInt32, maxRenderedIds, ProtoRemoteSelectionGroup)
    fieldMacros(SFBool, reconnect, ProtoRemoteSelectionGroup)
    fieldMacros(SFBool, render, ProtoRemoteSelectionGroup)
    fieldMacros(SFFloat, scaleRenderedIdsOnMove, ProtoRemoteSelectionGroup)
    fieldMacros(MFString, url, ProtoRemoteSelectionGroup)
};

#endif
