/*
 * NodeArc2D.h
 *
 * Copyright (C) 1999 Stephen F. White, 2007 J. "MUFTI" Scheurich
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

#ifndef _NODE_ARC_2D_H
#define _NODE_ARC_2D_H

#ifndef _CHAIN_BASED_NODE_H
#include "ChainBasedNode.h"
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

class ProtoArc2D : public Proto {
public:
                    ProtoArc2D(Scene *scene);
    virtual Node   *create(Scene *scene);

    virtual int     getType() const { return X3D_ARC_2D; }
    virtual int     getNodeClass() const 
                       { return PARAMETRIC_GEOMETRY_NODE | GEOMETRY_NODE; }

    virtual bool    isX3dInternalProto(void) { return true; }

    virtual bool    needUpdate(void) { return true; }

    FieldIndex endAngle;
    FieldIndex radius;
    FieldIndex startAngle;
    x3domGeometryCommonFieldIndex()
    FieldIndex ccw;
    FieldIndex subdivision;
};

class NodeArc2D : public ChainBasedGeometryNode {
public:
                    NodeArc2D(Scene *scene, Proto *proto);

    virtual Node   *copy() const { return new NodeArc2D(*this); }

    virtual const char* getComponentName(void) const 
                           { return "Geometry2D"; }
    virtual int         getComponentLevel(void) const { return 2; }
    virtual int     getX3dVersion(void) const { return 0; }

    virtual int     countPrimitives(void) {return 1;}
    virtual int     countPolygons(void) { return 0; }

    virtual int     writeProto(int f) { return writeX3dProto(f); }

    fieldMacros(SFFloat, endAngle,   ProtoArc2D)
    fieldMacros(SFFloat, radius,     ProtoArc2D)
    fieldMacros(SFFloat, startAngle, ProtoArc2D)
    x3domGeometryCommonFieldMacros(ProtoArc2D) 
    fieldMacros(SFBool,  ccw,         ProtoArc2D)
    fieldMacros(SFFloat, subdivision, ProtoArc2D)

protected:
    void            createChain(void);

};

#endif
