/*
 * NodeImageGeometry.h
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

class ProtoImageGeometry : public Proto {
public:
                    ProtoImageGeometry(Scene *scene);
    virtual Node   *create(Scene *scene);

    virtual int     getType() const { return X3DOM_IMAGE_GEOMETRY; }

    virtual bool    isX3domProto(void) { return true; }

    x3domGeometryCommonFieldIndex()
    FieldIndex ccw;
    FieldIndex color;
    FieldIndex coord;
    FieldIndex implicitMeshSize;
    FieldIndex index;
    FieldIndex normal;
    FieldIndex normalAsSphericalCoordinates;
    FieldIndex normalPerVertex;
    FieldIndex numColorComponents;
    FieldIndex numTexCoordComponents;
    FieldIndex position;
    FieldIndex primType;
    FieldIndex solid;
    FieldIndex size;
    FieldIndex texCoord;
    FieldIndex vertexCount;
};

class NodeImageGeometry : public GeometryNode {
public:
                    NodeImageGeometry(Scene *scene, Proto *proto);

    virtual Node   *copy() const { return new NodeImageGeometry(*this); }

    virtual int     getX3dVersion(void) const { return -1; }

    virtual bool    showFields() { return true; }

    x3domGeometryCommonFieldMacros(ProtoImageGeometry) 
    fieldMacros(SFBool,   ccw,                          ProtoImageGeometry)
    fieldMacros(SFNode,   color,                        ProtoImageGeometry)
    fieldMacros(MFNode,   coord,                        ProtoImageGeometry)
    fieldMacros(SFVec2f,  implicitMeshSize,             ProtoImageGeometry)
    fieldMacros(SFNode,   index,                        ProtoImageGeometry)
    fieldMacros(SFString, normal,                       ProtoImageGeometry)
    fieldMacros(SFInt32,  numColorComponents,           ProtoImageGeometry)
    fieldMacros(SFInt32,  numTexCoordComponents,        ProtoImageGeometry)
    fieldMacros(SFVec3f,  position,                     ProtoImageGeometry)
    fieldMacros(MFString, primType,                     ProtoImageGeometry)
    fieldMacros(SFVec3f,  size,                         ProtoImageGeometry)
    fieldMacros(SFBool,   solid,                        ProtoImageGeometry)
    fieldMacros(SFString, texCoord,                     ProtoImageGeometry)
    fieldMacros(MFInt32,  vertexCount,                  ProtoImageGeometry)
};

