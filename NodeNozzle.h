/*
 * NodeNozzle.h
 *
 * Copyright (C) 1999 Stephen F. White
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

#ifndef _NODE_Nozzle_H
#define _NODE_Nozzle_H

#ifndef _PROTO_MACROS_H
#include "ProtoMacros.h"
#endif
#ifndef _PROTO_H
#include "Proto.h"
#endif

#include "GeometryNode.h"

#include "SFMFTypes.h"

class ProtoNozzle : public Proto {
public:
                    ProtoNozzle(Scene *scene);
    virtual Node   *create(Scene *scene);

    virtual int     getType() const { return X3DOM_NOZZLE; }
    virtual int     getNodeClass() const { return GEOMETRY_NODE; }

    FieldIndex ccw;
    FieldIndex height;
    FieldIndex innerRadius;
    FieldIndex nozzleHeight;
    FieldIndex nozzleRadius;
    FieldIndex outerRadius;
    FieldIndex solid;
    FieldIndex subdivision;
    x3domGeometryCommonFieldIndex()
};

class NodeNozzle : public Node {
public:
                    NodeNozzle(Scene *scene, Proto *proto);

    virtual int     getX3dVersion(void) const { return -1; }
    virtual Node   *copy() const { return new NodeNozzle(*this); }

    fieldMacros(SFBool,  ccw,          ProtoNozzle)
    fieldMacros(SFFloat, height,       ProtoNozzle)
    fieldMacros(SFFloat, innerRadius,  ProtoNozzle)
    fieldMacros(SFFloat, nozzleHeight, ProtoNozzle)
    fieldMacros(SFFloat, nozzleRadius, ProtoNozzle)
    fieldMacros(SFFloat, outerRadius,  ProtoNozzle)
    fieldMacros(SFBool,  solid,        ProtoNozzle)
    fieldMacros(SFFloat, subdivision,  ProtoNozzle)
    x3domGeometryCommonFieldMacros(ProtoNozzle) 
};


#endif
