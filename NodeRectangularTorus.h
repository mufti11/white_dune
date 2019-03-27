/*
 * NodeRectangularTorus.h
 *
 * Copyright (C) 1999 Stephen F. White
 *               2018 J. "MUFTI" Scheurich
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

#ifndef _NODE_Rectangular_Torus_H
#define _NODE_Rectangular_Torus_H

#ifndef _PROTO_MACROS_H
#include "ProtoMacros.h"
#endif
#ifndef _PROTO_H
#include "Proto.h"
#endif

#include "GeometryNode.h"
#include "SFMFTypes.h"

class ProtoRectangularTorus : public Proto {
public:
                    ProtoRectangularTorus(Scene *scene);
    virtual Node   *create(Scene *scene);

    virtual int     getType() const { return X3DOM_RECTANGULAR_TORUS; }
    virtual int     getNodeClass() const { return GEOMETRY_NODE; }

    x3domGeometryCommonFieldIndex()
    FieldIndex angle;
    FieldIndex caps;
    FieldIndex ccw;
    FieldIndex height;
    FieldIndex innerRadius;
    FieldIndex outerRadius;
    FieldIndex solid;
    FieldIndex subdivision;
};

class NodeRectangularTorus : public Node {
public:
                    NodeRectangularTorus(Scene *scene, Proto *proto);

    virtual int     getX3dVersion(void) const { return -1; }
    virtual Node   *copy() const { return new NodeRectangularTorus(*this); }

    x3domGeometryCommonFieldMacros(ProtoRectangularTorus) 
    fieldMacros(SFFloat, angle, ProtoRectangularTorus)
    fieldMacros(SFBool, caps, ProtoRectangularTorus)
    fieldMacros(SFBool, ccw, ProtoRectangularTorus)
    fieldMacros(SFFloat, height, ProtoRectangularTorus)
    fieldMacros(SFFloat, innerRadius, ProtoRectangularTorus)
    fieldMacros(SFFloat, outerRadius, ProtoRectangularTorus)
    fieldMacros(SFFloat, subdivision, ProtoRectangularTorus)
    fieldMacros(SFBool, solid, ProtoRectangularTorus)
};

#endif
