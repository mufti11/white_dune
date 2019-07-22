/*
 * NodePointSet.h
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

#ifndef _NODE_POINTSET_H
#define _NODE_POINTSET_H

#ifndef _GEOMETRY_NODE_H
#include "GeometryNode.h"
#endif
#ifndef _PROTO_MACROS_H
#include "ProtoMacros.h"
#endif
#ifndef _PROTO_H
#include "Proto.h"
#endif
#ifndef _COLORED_H
#include "Colored.h"
#endif

#include "SFMFTypes.h"

class ProtoPointSet : public GeometryProto {
public:
                    ProtoPointSet(Scene *scene);
    virtual Node   *create(Scene *scene);

    virtual bool    isDeclaredInRwd_h() { return true; }      

    virtual int     getType() const { return VRML_POINT_SET; }

    FieldIndex attrib;
    FieldIndex color;
    FieldIndex coord;
    FieldIndex fogCoord;
};

class NodePointSet : public GeometryNode, Colored {
public:
                    NodePointSet(Scene *scene, Proto *proto);

    virtual int     getProfile(void) const { return PROFILE_INTERCHANGE; }
    virtual int     getX3dVersion(void) const { return 0; } 
    virtual Node   *copy() const { return new NodePointSet(*this); }

    virtual void    draw();

    virtual bool    isInvalidChildNode(void) { return true; }
    virtual bool    hasBoundingBox(void)     { return true; }
    virtual Vec3f   getMinBoundingBox(void);
    virtual Vec3f   getMaxBoundingBox(void);
    virtual void    flip(int index);
    virtual void    swap(int fromTo);

    virtual bool    maySetDefault(void) { return false; }

    virtual NodeColor *getColorNode() 
                    { return (NodeColor *)color()->getValue(); }
    virtual NodeColorRGBA *getColorRGBANode() 
                    { return (NodeColorRGBA *)color()->getValue(); }
    virtual NodeCoordinate *getCoordinateNode(void) 
                    { return (NodeCoordinate *)coord()->getValue(); }
    virtual Colored *getColored() { return this; }

    virtual int     colorPerVertexField() const 
                       { return -1; }
    virtual int     colorIndexField() const
                       { return -1; }

    fieldMacros(MFNode, attrib,   ProtoPointSet)
    fieldMacros(SFNode, color,    ProtoPointSet)
    fieldMacros(SFNode, coord,    ProtoPointSet)
    fieldMacros(SFNode, fogCoord, ProtoPointSet)

    void pointDraw();
};

#endif // _NODE_POINTSET_H
