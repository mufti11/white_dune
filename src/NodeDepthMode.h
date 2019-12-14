/*
 * NodeDepthMode.h
 *
 * Copyright (C) 1999 Stephen F. White, 2018 J. "MUFTI" Scheurich
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

#ifndef _NODE_DEPTH_MODE_H
#define _NODE_DEPTH_MODE_H

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
#ifndef _DUNEAPP_H
#include "DuneApp.h"
#endif

#include "SFMFTypes.h"

class ProtoDepthMode : public Proto {
public:
                    ProtoDepthMode(Scene *scene);
    virtual Node   *create(Scene *scene);

    virtual int     getType() const { return X3DOM_COLOR_MASK_MODE; }

    virtual bool    isX3domProto(void) { return true; }

    FieldIndex depthFunc;
    FieldIndex enableDepthTest;
    FieldIndex readOnly;
    FieldIndex zFarRange;
    FieldIndex zNearRange;
};

class NodeDepthMode : public Node {
public:
                    NodeDepthMode(Scene *scene, Proto *proto);

    virtual Node   *copy() { return new NodeDepthMode(m_scene, m_proto); }

    virtual int     getX3dVersion(void) const { return -1; }

    virtual bool    showFields() { return true; }

    fieldMacros(SFString, depthFunc,       ProtoDepthMode)
    fieldMacros(SFBool,   enableDepthTest, ProtoDepthMode)
    fieldMacros(SFBool,   readOnly,        ProtoDepthMode)
    fieldMacros(SFFloat,  zFarRange,       ProtoDepthMode)
    fieldMacros(SFFloat,  zNearRange,      ProtoDepthMode)
};

#endif
