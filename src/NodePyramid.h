/*
 * NodePyramid.h
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

#ifndef _NODE_PYRAMID_H
#define _NODE_PYRAMID_H

#ifndef _PROTO_MACROS_H
#include "ProtoMacros.h"
#endif
#ifndef _PROTO_H
#include "Proto.h"
#endif

#include "GeometryNode.h"
#include "SFMFTypes.h"

class ProtoPyramid : public Proto {
public:
                    ProtoPyramid(Scene *scene);
    virtual Node   *create(Scene *scene);

    virtual int     getType() const { return X3DOM_PYRAMID; }
    virtual int     getNodeClass() const { return GEOMETRY_NODE; }

    x3domGeometryCommonFieldIndex()
    FieldIndex ccw;
    FieldIndex height;
    FieldIndex solid;
    FieldIndex xbottom;
    FieldIndex xoff;
    FieldIndex xtop;
    FieldIndex ybottom;
    FieldIndex yoff;
    FieldIndex ytop;
};

class NodePyramid : public Node {
public:
                    NodePyramid(Scene *scene, Proto *proto);

    virtual int     getX3dVersion(void) const { return -1; }
    virtual Node   *copy() const { return new NodePyramid(*this); }

    x3domGeometryCommonFieldMacros(ProtoPyramid) 
    fieldMacros(SFBool, ccw, ProtoPyramid)
    fieldMacros(SFFloat, height, ProtoPyramid)
    fieldMacros(SFBool, solid, ProtoPyramid)
    fieldMacros(SFFloat, xbottom, ProtoPyramid)
    fieldMacros(SFFloat, xoff, ProtoPyramid)
    fieldMacros(SFFloat, xtop, ProtoPyramid)
    fieldMacros(SFFloat, ybottom, ProtoPyramid)
    fieldMacros(SFFloat, yoff, ProtoPyramid)
    fieldMacros(SFFloat, ytop, ProtoPyramid)
};

#endif
