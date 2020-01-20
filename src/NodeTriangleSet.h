/*
 * NodeTriangleSet.h
 *
 * Copyright (C) 1999 Stephen F. White, 2008 J. "MUFTI" Scheurich
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
#include "DuneApp.h"
#include "MyMesh.h"
#include "Vec3f.h"
#include "NodeCoordinate.h"
#include "Colored.h"
#include "SFMFTypes.h"
#include "ComposedGeometryMacros.h"

class ProtoTriangleSet : public GeometryProto {
public:
                    ProtoTriangleSet(Scene *scene);
    virtual Node   *create(Scene *scene);

    virtual int     getType() const { return X3D_TRIANGLE_SET; }

    virtual bool    isX3dInternalProto(void) { return true; }

    virtual bool    isMesh(void) { return true; }
    virtual bool    isExportTargetMesh(void) { return true; }

    virtual bool    isDeclaredInRwd_h() { return true; }

    FieldIndex color;
    FieldIndex coord;
    FieldIndex normal;
    FieldIndex texCoord;
    FieldIndex ccw;
    FieldIndex colorPerVertex;
    FieldIndex normalPerVertex;
    FieldIndex solid;
    ComposedGeometryProtoMacro()
    x3domGeometryCommonFieldIndex()
    FieldIndex normalUpdateMode;
};

class NodeTriangleSet : public MeshBasedNode, Colored {
public:
                    NodeTriangleSet(Scene *scene, Proto *proto);

protected:
    virtual        ~NodeTriangleSet();

public:
    virtual const char* getComponentName(void) const 
                           { return "Rendering"; }
    virtual int         getComponentLevel(void) const { return 3; }
    virtual int         getX3dVersion(void) const { return 0; }
    virtual Node   *copy() const { return new NodeTriangleSet(*this); }

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

    virtual bool    canConvertToTriangleSet(void) { return hasParent(); }

    virtual bool    maySetDefault(void) { return false; }

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
                       { return -1; }

    MFVec3f        *getCoordinates();
    MFVec2f        *getTextureCoordinates();

    Node           *toIndexedLineSet(void);

    virtual bool    shouldConvertToTriangleSet(void) { return false; }
    virtual bool    shouldConvertToIndexedTriangleSet(void) { return false; }

    fieldMacros(SFNode, color, ProtoTriangleSet)
    fieldMacros(SFNode, coord, ProtoTriangleSet)
    fieldMacros(SFNode, normal, ProtoTriangleSet)
    fieldMacros(SFNode, texCoord, ProtoTriangleSet)
    fieldMacros(SFBool, ccw, ProtoTriangleSet)
    fieldMacros(SFBool, colorPerVertex, ProtoTriangleSet)
    fieldMacros(SFBool, normalPerVertex, ProtoTriangleSet)
    fieldMacros(SFBool, solid, ProtoTriangleSet)
    ComposedGeometryFieldMacros(ProtoTriangleSet)
    x3domGeometryCommonFieldMacros(ProtoTriangleSet)
    fieldMacros(SFString, normalUpdateMode, ProtoTriangleSet)

protected:
    void            createMesh(bool cleanDoubleVertices = true,
                               bool triangulate = true);

protected:
    MFInt32        *m_coordIndex;
    bool            m_colorPerVertexWarning;
};

