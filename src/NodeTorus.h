/*
 * NodeTorus.h
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

#pragma once

#include "ProtoMacros.h"
#include "Proto.h"
#include "GeometryNode.h"
#include "SFMFTypes.h"

class ProtoTorus : public Proto {
public:
                    ProtoTorus(Scene *scene);
    virtual Node   *create(Scene *scene);

    virtual int     getType() const { return X3DOM_TORUS; }
    virtual int     getNodeClass() const { return GEOMETRY_NODE; }

    x3domGeometryCommonFieldIndex()
    FieldIndex angle;
    FieldIndex caps;
    FieldIndex ccw;
    FieldIndex height;
    FieldIndex innerRadius;
    FieldIndex insideOutsideRadius;
    FieldIndex outerRadius;
    FieldIndex solid;
    FieldIndex subdivision;
};

class NodeTorus : public Node {
public:
                    NodeTorus(Scene *scene, Proto *proto);

    virtual int     getX3dVersion(void) const { return -1; }
    virtual Node   *copy() const { return new NodeTorus(*this); }

    x3domGeometryCommonFieldMacros(ProtoTorus) 
    fieldMacros(SFFloat, angle, ProtoTorus)
    fieldMacros(SFBool, caps, ProtoTorus)
    fieldMacros(SFBool, ccw, ProtoTorus)
    fieldMacros(SFFloat, height, ProtoTorus)
    fieldMacros(SFFloat, innerRadius, ProtoTorus)
    fieldMacros(SFBool, insideOutsideRadius, ProtoTorus)
    fieldMacros(SFFloat, outerRadius, ProtoTorus)
    fieldMacros(SFVec2f, subdivision, ProtoTorus)
    fieldMacros(SFBool, solid, ProtoTorus)
};

