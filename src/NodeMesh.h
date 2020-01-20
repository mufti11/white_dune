/*
 * NodeMesh.h
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

#pragma once

#include "Node.h"
#include "ProtoMacros.h"
#include "Proto.h"
#include "DuneApp.h"
#include "GeometryNode.h"
#include "SFMFTypes.h"

class ProtoMesh : public Proto {
public:
                    ProtoMesh(Scene *scene);
    virtual Node   *create(Scene *scene);

    virtual int     getType() const { return X3DOM_MESH; }

    virtual bool    isX3domProto(void) { return true; }

    x3domGeometryCommonFieldIndex()
    FieldIndex ccw;
    FieldIndex index;
    FieldIndex primType;
    FieldIndex solid;
    FieldIndex vertexAttributes;
};

class NodeMesh : public GeometryNode {
public:
                    NodeMesh(Scene *scene, Proto *proto);

    virtual Node   *copy() const { return new NodeMesh(*this); }

    virtual int     getX3dVersion(void) const { return -1; }

    virtual bool    showFields() { return true; }

    x3domGeometryCommonFieldMacros(ProtoMesh) 
    fieldMacros(SFBool,   ccw,                          ProtoMesh)
    fieldMacros(MFInt32,  index,                        ProtoMesh)
    fieldMacros(SFString, primType,                     ProtoMesh)
    fieldMacros(SFBool,   solid,                        ProtoMesh)
    fieldMacros(MFNode,   vertexAttributes,             ProtoMesh)
};

