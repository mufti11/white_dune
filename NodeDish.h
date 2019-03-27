/*
 * NodeDish.h
 *
 * Copyright (C) 1999 Stephen F. White
 *               2003 Th. Rothermel
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

#ifndef _NODE_DISH_H
#define _NODE_DISH_H

#ifndef _PROTO_MACROS_H
#include "ProtoMacros.h"
#endif
#ifndef _PROTO_H
#include "Proto.h"
#endif

#include "GeometryNode.h"


#include "SFMFTypes.h"

class ProtoDish : public Proto {
public:
                    ProtoDish(Scene *scene);
    virtual Node   *create(Scene *scene);

    virtual int     getType() const { return X3DOM_DISH; }
    virtual int     getNodeClass() const 
                       { return PRIMITIVE_GEOMETRY_NODE | GEOMETRY_NODE; }

    FieldIndex bottom;
    FieldIndex ccw;
    FieldIndex diameter;
    FieldIndex height;
    FieldIndex radius;
    FieldIndex side;
    FieldIndex solid;
    FieldIndex subdivision;
    FieldIndex texCoord;
    x3domGeometryCommonFieldIndex()
};

class NodeDish : public Node {
public:
                    NodeDish(Scene *scene, Proto *proto);

    virtual Node   *copy() const { return new NodeDish(*this); }
    virtual int     getX3dVersion(void) const { return -1; }

    fieldMacros(SFBool,  bottom,      ProtoDish)
    fieldMacros(SFBool,  ccw,         ProtoDish)
    fieldMacros(SFFloat, diameter,    ProtoDish)
    fieldMacros(SFFloat, height,      ProtoDish)
    fieldMacros(SFFloat, radius,      ProtoDish)
    fieldMacros(SFBool,  side,        ProtoDish)
    fieldMacros(SFBool,  solid,       ProtoDish)
    fieldMacros(SFVec2f, subdivision, ProtoDish)
    x3domGeometryCommonFieldMacros(ProtoDish) 
};

#endif
