/*
 * NodeMPRVolumeStyle.h
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

#ifndef _NODE_MPR_VOLUME_STYLE_H
#define _NODE_MPR_VOLUME_STYLE_H

#ifndef _NODE_H
#include "Node.h"
#endif
#ifndef _PROTO_MACROS_H
#include "ProtoMacros.h"
#endif
#ifndef _PROTO_H
#include "Proto.h"
#endif

#include "SFMFTypes.h"

class ProtoMPRVolumeStyle : public Proto {
public:
                    ProtoMPRVolumeStyle(Scene *scene);
    virtual Node   *create(Scene *scene);

    virtual int     getType() const { return X3DOM_MPR_VOLUME_STYLE; }

    FieldIndex enabled;
    FieldIndex finalLine;
    FieldIndex originLine;
    FieldIndex positionLine;
    FieldIndex transferFunction;
};

class NodeMPRVolumeStyle : public Node {
public:
                    NodeMPRVolumeStyle(Scene *scene, Proto *proto);

    virtual Node   *copy() const { return new NodeMPRVolumeStyle(*this); }

    virtual int     getX3dVersion(void) const { return -1; }

    fieldMacros(SFBool, enabled, ProtoMPRVolumeStyle)
    fieldMacros(SFVec3f, finalLine, ProtoMPRVolumeStyle)
    fieldMacros(SFVec3f, originLine, ProtoMPRVolumeStyle)
    fieldMacros(SFFloat, positionLine, ProtoMPRVolumeStyle)
    fieldMacros(SFNode, transferFunction, ProtoMPRVolumeStyle)
};

#endif
