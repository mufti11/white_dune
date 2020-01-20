/*
 * NodeIndexedLineSet.h
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

#pragma once

#include "GeometryNode.h"
#include "ProtoMacros.h"
#include "Proto.h"
#include "Colored.h"
#include "SFMFTypes.h"

class ProtoIndexedLineSet : public GeometryProto {
public:
                    ProtoIndexedLineSet(Scene *scene);
    virtual Node   *create(Scene *scene);

    virtual bool    isDeclaredInRwd_h() { return true; }      

    virtual int     getType() const { return VRML_INDEXED_LINE_SET; }
    virtual int     getNodeClass() const 
                       { return LINE_SET_NODE | GEOMETRY_NODE; }

    FieldIndex attrib;
    FieldIndex color;
    FieldIndex coord;
    FieldIndex colorIndex;
    FieldIndex colorPerVertex;
    FieldIndex coordIndex;
    FieldIndex fogCoord;
    x3domGeometryCommonFieldIndex()
    FieldIndex ccw;
    FieldIndex solid;
};

class NodeIndexedLineSet : public GeometryNode, Colored {
public:
                    NodeIndexedLineSet(Scene *scene, Proto *proto);

    virtual int     getProfile(void) const;
    virtual int     getX3dVersion(void) const { return 0; }
    virtual Node   *copy() const { return new NodeIndexedLineSet(*this); }

    virtual void    draw();

    virtual bool    isInvalidChildNode(void) { return true; }
    virtual bool    hasBoundingBox(void)     { return true; }
    virtual Vec3f   getMinBoundingBox(void);
    virtual Vec3f   getMaxBoundingBox(void);
    virtual void    flip(int index);
    virtual void    swap(int fromTo);

    virtual bool    maySetDefault(void) { return false; }

    Node           *toPointSet(void);

    virtual bool    canWriteAc3d() { return true; }
    virtual int     writeAc3d(int filedes, int indent);

    virtual bool    canWriteLdrawDat() { return true; }
    virtual int     writeLdrawDat(int filedes, int indent);

    virtual NodeColor *getColorNode() 
                    { return (NodeColor *)color()->getValue(); }
    virtual NodeColorRGBA *getColorRGBANode() 
                    { return (NodeColorRGBA *)color()->getValue(); }
    virtual NodeCoordinate *getCoordinateNode(void) 
                    { return (NodeCoordinate *)coord()->getValue(); }
    virtual Colored *getColored() { return this; }

    virtual int     colorPerVertexField() const 
                       { return colorPerVertex_Field(); }
    virtual int     colorIndexField() const
                       { return colorIndex_Field(); }

    fieldMacros(MFNode,  attrib,         ProtoIndexedLineSet)
    fieldMacros(SFNode,  color,          ProtoIndexedLineSet)
    fieldMacros(SFNode,  coord,          ProtoIndexedLineSet)
    fieldMacros(MFInt32, colorIndex,     ProtoIndexedLineSet)
    fieldMacros(SFBool,  colorPerVertex, ProtoIndexedLineSet)
    fieldMacros(MFInt32, coordIndex,     ProtoIndexedLineSet)
    fieldMacros(SFNode,  fogCoord,       ProtoIndexedLineSet)
    x3domGeometryCommonFieldMacros(ProtoIndexedLineSet) 
    fieldMacros(SFBool,  ccw,            ProtoIndexedLineSet)
    fieldMacros(SFBool,  solid,          ProtoIndexedLineSet)

    void            lineDraw();
};

