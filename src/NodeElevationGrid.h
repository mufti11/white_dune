/*
 * NodeElevationGrid.h
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

#include "MeshBasedNode.h"
#include "ProtoMacros.h"
#include "Proto.h"
#include "SFMFTypes.h"

class ProtoElevationGrid : public GeometryProto {
public:
                    ProtoElevationGrid(Scene *scene);
    virtual Node   *create(Scene *scene);

    virtual int     getType() const { return VRML_ELEVATION_GRID; }
    virtual int     getNodeClass() const { return GEOMETRY_NODE; }

    virtual bool    isMesh(void) { return true; }

    FieldIndex attrib;
    FieldIndex color;
    FieldIndex fogCoord;
    FieldIndex normal;
    FieldIndex texCoord;
    FieldIndex ccw;
    FieldIndex colorPerVertex;
    FieldIndex creaseAngle;
    FieldIndex height;
    FieldIndex normalPerVertex;
    FieldIndex solid;
    FieldIndex xDimension;
    FieldIndex xSpacing;
    FieldIndex zDimension;
    FieldIndex zSpacing;
    x3domGeometryCommonFieldIndex()
};

class MFVec2f;

class NodeElevationGrid : public MeshBasedNode {
public:
                    NodeElevationGrid(Scene *scene, Proto *proto);

protected:
    virtual        ~NodeElevationGrid();

public:
    virtual int     getProfile(void) const;
    virtual int     getX3dVersion(void) const { return 0; }
    virtual Node   *copy() const { return new NodeElevationGrid(*this); }

    virtual void    setField(int index, FieldValue *value, int cf = -1);

    virtual Vec3f   getHandle(int handle, int *constraint, int *field);
    virtual void    setHandle(int handle, const Vec3f &v);

    virtual void    draw() { meshDraw(); }
    virtual void    drawHandles(void);

    virtual bool    validHandle(int handle);
    virtual bool    checkHandle(int handle);
    virtual int     getMaxHandle(void);

    virtual int     getNormalField() { return normal_Field(); }
    virtual int     getTexCoordField() { return texCoord_Field(); }

    virtual void    setNormalFromMesh(Node *nnormal);
    virtual void    setTexCoordFromMesh(Node *ntexCoord);

    virtual bool    hasTwoSides(void) { return true; }
    virtual bool    isDoubleSided(void) { return !solid()->getValue(); }
    virtual void    toggleDoubleSided(void)
                       { solid(new SFBool(!solid()->getValue())); }
    virtual int     getSolidField() { return solid_Field(); }
    virtual void    flipSide(void) { ccw(new SFBool(!ccw()->getValue())); }

    virtual NodeColor *getColorNode() 
                    { return (NodeColor *)color()->getValue(); }
    virtual NodeColorRGBA *getColorRGBANode() 
                    { return (NodeColorRGBA *)color()->getValue(); }

    virtual bool    shouldConvertToIndexedFaceSet(void) { return false; }

    fieldMacros(MFNode,  attrib,          ProtoElevationGrid)
    fieldMacros(SFNode,  color,           ProtoElevationGrid)
    fieldMacros(SFNode,  fogCoord,        ProtoElevationGrid)
    fieldMacros(SFNode,  normal,          ProtoElevationGrid)
    fieldMacros(SFNode,  texCoord,        ProtoElevationGrid)
    fieldMacros(MFFloat, height,          ProtoElevationGrid)
    fieldMacros(SFBool,  ccw,             ProtoElevationGrid)
    fieldMacros(SFBool,  colorPerVertex,  ProtoElevationGrid)
    fieldMacros(SFFloat, creaseAngle,     ProtoElevationGrid)
    fieldMacros(SFBool,  normalPerVertex, ProtoElevationGrid)
    fieldMacros(SFBool,  solid,           ProtoElevationGrid)
    fieldMacros(SFInt32, xDimension,      ProtoElevationGrid)
    fieldMacros(SFFloat, xSpacing,        ProtoElevationGrid)
    fieldMacros(SFInt32, zDimension,      ProtoElevationGrid)
    fieldMacros(SFFloat, zSpacing,        ProtoElevationGrid)
    x3domGeometryCommonFieldMacros(ProtoElevationGrid) 

protected:
    void            createMesh(bool cleanDoubleVertices = true,
                               bool triangulate = true);
    MFVec2f        *accountTexCoord(void);

};

