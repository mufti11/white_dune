/*
 * NodeIndexedQuadSet.h
 *
 * Copyright (C) 1999 Stephen F. White, 2009 J. "MUFTI" Scheurich
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
#include "Colored.h"
#include "SFMFTypes.h"
#include "ComposedGeometryMacros.h"

class ProtoIndexedQuadSet : public GeometryProto {
public:
                    ProtoIndexedQuadSet(Scene *scene);
    virtual Node   *create(Scene *scene);

    virtual int     getType() const { return X3D_INDEXED_QUAD_SET; }

    virtual bool    isMesh(void) { return true; }
    
    virtual bool    isX3dInternalProto(void) { return true; }

    FieldIndex color;
    FieldIndex coord;
    FieldIndex normal;
    FieldIndex texCoord;
    FieldIndex ccw;
    FieldIndex colorPerVertex;
    FieldIndex normalPerVertex;
    FieldIndex solid;
    FieldIndex index;
    ComposedGeometryProtoMacro()
    x3domGeometryCommonFieldIndex()
    FieldIndex normalUpdateMode;
};

class NodeIndexedQuadSet : public MeshBasedNode, Colored {
public:
                    NodeIndexedQuadSet(Scene *scene, Proto *proto);

protected:
    virtual        ~NodeIndexedQuadSet();

public:
    virtual int     getProfile(void) const { return PROFILE_CAD_INTERCHANGE; }
    virtual const char* getComponentName(void) const { return "CADGeometry"; }
    virtual int         getComponentLevel(void) const { return 1; }
    virtual int     getX3dVersion(void) const { return 1; }
    virtual Node   *copy() { return new NodeIndexedQuadSet(m_scene, m_proto); }

    virtual void    setField(int index, FieldValue *value, int cf = -1);

    virtual void    draw();

    virtual void    flip(int index);
    virtual void    swap(int fromTo);

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

    virtual bool    maySetDefault(void) { return false; }

    virtual NodeColor *getColorNode() 
                    { return (NodeColor *)color()->getValue(); }
    virtual NodeColorRGBA *getColorRGBANode() 
                    { return (NodeColorRGBA *)color()->getValue(); }
    virtual NodeCoordinate *getCoordinateNode(void) 
                    { return (NodeCoordinate *)coord()->getValue(); }
    virtual Colored *getColored() { return this; }

    virtual int     colorPerVertexField() 
                       { return colorPerVertex_Field(); }
    virtual int     colorIndexField() { return index_Field(); }

    MFVec3f        *getCoordinates();
    MFVec2f        *getTextureCoordinates();

    Node           *toIndexedLineSet(void);

    fieldMacros(SFNode, color, ProtoIndexedQuadSet);
    fieldMacros(SFNode, coord, ProtoIndexedQuadSet);
    fieldMacros(SFNode, normal, ProtoIndexedQuadSet);
    fieldMacros(SFNode, texCoord, ProtoIndexedQuadSet);
    fieldMacros(SFBool, ccw, ProtoIndexedQuadSet);
    fieldMacros(SFBool, colorPerVertex, ProtoIndexedQuadSet);
    fieldMacros(SFBool, normalPerVertex, ProtoIndexedQuadSet);
    fieldMacros(SFBool, solid, ProtoIndexedQuadSet);
    fieldMacros(MFInt32, index, ProtoIndexedQuadSet);
    ComposedGeometryFieldMacros(ProtoIndexedQuadSet)
    x3domGeometryCommonFieldMacros(ProtoIndexedQuadSet)
    fieldMacros(SFString, normalUpdateMode, ProtoIndexedQuadSet)

protected:
    void            createMesh(bool cleanDoubleVertices = true,
                               bool triangulate = true);

protected:
    MFInt32        *m_coordIndex;
    bool            m_colorPerVertexWarning;
};

