/*
 * NodeBinaryGeometry.h
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

#ifndef _NODE_BINARY_GEOMETRY_H
#define _NODE_BINARY_GEOMETRY_H

#ifndef _NODE_H
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
#ifndef GEOMETRY_NODE_H
#include "GeometryNode.h"
#endif

#include "SFMFTypes.h"

class ProtoBinaryGeometry : public Proto {
public:
                    ProtoBinaryGeometry(Scene *scene);
    virtual Node   *create(Scene *scene);

    virtual int     getType() const { return X3DOM_BINARY_GEOMETRY; }

    virtual bool    isX3domProto(void) { return true; }

    x3domGeometryCommonFieldIndex()
    FieldIndex binormal;
    FieldIndex binormalType;
    FieldIndex ccw;
    FieldIndex color;
    FieldIndex colorType;
    FieldIndex compressed;
    FieldIndex coord;
    FieldIndex coordType;
    FieldIndex idsPerVertex;
    FieldIndex index;
    FieldIndex indexType;
    FieldIndex normal;
    FieldIndex normalAsSphericalCoordinates;
    FieldIndex normalPerVertex;
    FieldIndex normalType;
    FieldIndex numTexCoordComponents;
    FieldIndex position;
    FieldIndex primType;
    FieldIndex rgbaColors;
    FieldIndex size;
    FieldIndex solid;
    FieldIndex tangent;
    FieldIndex tangentType;
    FieldIndex texCoord;
    FieldIndex texCoordType;
    FieldIndex vertexCount;
};

class NodeBinaryGeometry : public GeometryNode {
public:
                    NodeBinaryGeometry(Scene *scene, Proto *proto);

    virtual Node   *copy() const { return new NodeBinaryGeometry(*this); }

    virtual int     getX3dVersion(void) const { return -1; }

    virtual bool    showFields() { return true; }

    virtual void    load();

    x3domGeometryCommonFieldMacros(ProtoBinaryGeometry) 
    fieldMacros(SFString, binormal,                     ProtoBinaryGeometry)
    fieldMacros(SFString, binormalType,                 ProtoBinaryGeometry)
    fieldMacros(SFBool,   ccw,                          ProtoBinaryGeometry)
    fieldMacros(SFString, color,                        ProtoBinaryGeometry)
    fieldMacros(SFString, colorType,                    ProtoBinaryGeometry)
    fieldMacros(SFBool,   compressed,                   ProtoBinaryGeometry)
    fieldMacros(SFString, coord,                        ProtoBinaryGeometry)
    fieldMacros(SFString, coordType,                    ProtoBinaryGeometry)
    fieldMacros(SFBool,   idsPerVertex,                 ProtoBinaryGeometry)
    fieldMacros(SFString, index,                        ProtoBinaryGeometry)
    fieldMacros(SFString, indexType,                    ProtoBinaryGeometry)
    fieldMacros(SFString, normal,                       ProtoBinaryGeometry)
    fieldMacros(SFBool,   normalAsSphericalCoordinates, ProtoBinaryGeometry)
    fieldMacros(SFBool,   normalPerVertex,              ProtoBinaryGeometry)
    fieldMacros(SFString, normalType,                   ProtoBinaryGeometry)
    fieldMacros(SFInt32,  numTexCoordComponents,        ProtoBinaryGeometry)
    fieldMacros(SFVec3f,  position,                     ProtoBinaryGeometry)
    fieldMacros(MFString, primType,                     ProtoBinaryGeometry)
    fieldMacros(SFBool,   rgbaColors,                   ProtoBinaryGeometry)
    fieldMacros(SFVec3f,  size,                         ProtoBinaryGeometry)
    fieldMacros(SFBool,   solid,                        ProtoBinaryGeometry)
    fieldMacros(SFString, tangent,                      ProtoBinaryGeometry)
    fieldMacros(SFString, tangentType,                  ProtoBinaryGeometry)
    fieldMacros(SFString, texCoord,                     ProtoBinaryGeometry)
    fieldMacros(SFString, texCoordType,                 ProtoBinaryGeometry)
    fieldMacros(MFInt32,  vertexCount,                  ProtoBinaryGeometry)
};

#endif
