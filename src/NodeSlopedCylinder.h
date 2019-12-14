/*
 * NodeSlopedCylinder.h
 *
 * Copyright (C) 1999 Stephen F. White
 *               2003 Th. Rothermel
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

#ifndef _NODE_SlopedCylinder_H
#define _NODE_SlopedCylinder_H

#ifndef _MESH_BASED_NODE_H
#include "MeshBasedNode.h"
#endif
#ifndef _PROTO_MACROS_H
#include "ProtoMacros.h"
#endif
#ifndef _PROTO_H
#include "Proto.h"
#endif
#ifndef _VEC3F_H
#include "Vec3f.h"
#endif

#include "SFMFTypes.h"

class ProtoSlopedCylinder : public Proto {
public:
                    ProtoSlopedCylinder(Scene *scene);
    virtual Node   *create(Scene *scene);

    virtual int     getType() const { return X3DOM_SLOPED_CYLINDER; }
    virtual int     getNodeClass() const { return GEOMETRY_NODE; }

    FieldIndex bottom;
    FieldIndex height;
    FieldIndex radius;
    FieldIndex side;
    FieldIndex top;
    FieldIndex solid;
    x3domGeometryCommonFieldIndex()
    FieldIndex ccw;
    FieldIndex subdivision;
    FieldIndex xbshear;
    FieldIndex xtshear;
    FieldIndex ybshear;
    FieldIndex ytshear;
};

class NodeSlopedCylinder : public Node {
public:
                    NodeSlopedCylinder(Scene *scene, Proto *proto);

    virtual Node   *copy() { return new NodeSlopedCylinder(m_scene, m_proto); }
    virtual int     getX3dVersion(void) const { return -1; }

    x3domGeometryCommonFieldMacros(ProtoSlopedCylinder) 
    fieldMacros(SFBool,  bottom,      ProtoSlopedCylinder)
    fieldMacros(SFFloat, height,      ProtoSlopedCylinder)
    fieldMacros(SFFloat, radius,      ProtoSlopedCylinder)
    fieldMacros(SFBool,  side,        ProtoSlopedCylinder)
    fieldMacros(SFBool,  top,         ProtoSlopedCylinder)
    fieldMacros(SFBool,  solid,       ProtoSlopedCylinder)
    fieldMacros(SFBool,  ccw,         ProtoSlopedCylinder)
    fieldMacros(SFFloat, subdivision, ProtoSlopedCylinder)
    fieldMacros(SFFloat, xbshear,     ProtoSlopedCylinder)
    fieldMacros(SFFloat, xtshear,     ProtoSlopedCylinder)
    fieldMacros(SFFloat, ybshear,     ProtoSlopedCylinder)
    fieldMacros(SFFloat, ytshear,     ProtoSlopedCylinder)
};

#endif
