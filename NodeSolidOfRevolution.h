/*
 * NodeSolidOfRevolution.h
 *
 * Copyright (C) 1999 Stephen F. White, 2004 Wu qingwei
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

#ifndef _NODE_SOLID_OF_REVOLUTION_H
#define _NODE_SOLID_OF_REVOLUTION_H

#ifndef _NODE_H
#include "Node.h"
#endif
#ifndef _PROTO_MACROS_H
#include "ProtoMacros.h"
#endif
#ifndef _PROTO_H
#include "Proto.h"
#endif

#include "GeometryNode.h"
#include "SFMFTypes.h"

class ProtoSolidOfRevolution : public GeometryProto {
public:
                    ProtoSolidOfRevolution(Scene *scene);
    virtual Node   *create(Scene *scene);

    virtual int     getType() const { return X3DOM_SOLID_OF_REVOLUTION; }

    FieldIndex angle;
    FieldIndex ccw;
    FieldIndex caps;
    FieldIndex creaseAngle;
    FieldIndex crossSection;
    FieldIndex solid;
    FieldIndex subdivision;
    x3domGeometryCommonFieldIndex()
};


class NodeSolidOfRevolution : public Node {
public:
                    NodeSolidOfRevolution(Scene *scene, Proto *proto);

    virtual int     getX3dVersion(void) const { return 0; }
    virtual Node   *copy() const { return new NodeSolidOfRevolution(*this); }

    fieldMacros(SFFloat,    angle,        ProtoSolidOfRevolution)
    fieldMacros(SFBool,     ccw,          ProtoSolidOfRevolution)
    fieldMacros(SFBool,     caps,         ProtoSolidOfRevolution)
    fieldMacros(SFFloat,    creaseAngle,  ProtoSolidOfRevolution)
    fieldMacros(MFVec2f,    crossSection, ProtoSolidOfRevolution)
    fieldMacros(SFBool,     solid,        ProtoSolidOfRevolution)
    fieldMacros(SFFloat,    subdivision,  ProtoSolidOfRevolution)
    x3domGeometryCommonFieldMacros(ProtoSolidOfRevolution) 
};

#endif
