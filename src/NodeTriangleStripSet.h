/*
 * NodeTriangleStripSet.h
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

#ifndef _NODE_TRIANGLE_STRIP_SET_H
#define _NODE_TRIANGLE_STRIP_SET_H

#ifndef _MESH_BASED_NODE_H
#include "MeshBasedNode.h"
#endif
#ifndef _PROTO_MACROS_H
#include "ProtoMacros.h"
#endif
#ifndef _PROTO_H
#include "Proto.h"
#endif
#ifndef _DUNEAPP_H
#include "DuneApp.h"
#endif
#ifndef _MY_MESH_H
#include "MyMesh.h"
#endif
#ifndef _VEC3F_H
#include "Vec3f.h"
#endif 
#ifndef _NODE_COORDINATE_H
#include "NodeCoordinate.h"
#endif
#ifndef _COLORED_H
#include "Colored.h"
#endif
#include "SFMFTypes.h"
#include "ComposedGeometryMacros.h"

class ProtoTriangleStripSet : public GeometryProto {
public:
                    ProtoTriangleStripSet(Scene *scene);
    virtual Node   *create(Scene *scene);

    virtual int     getType() const { return X3D_TRIANGLE_STRIP_SET; }

    virtual bool    isX3dInternalProto(void) { return true; }

    virtual bool    isMesh(void) { return true; }

    FieldIndex color;
    FieldIndex coord;
    FieldIndex normal;
    FieldIndex stripCount;
    FieldIndex texCoord;
    FieldIndex ccw;
    FieldIndex colorPerVertex;
    FieldIndex normalPerVertex;
    FieldIndex solid;
    ComposedGeometryProtoMacro()
};

class NodeTriangleStripSet : public MeshBasedNode, Colored {
public:
                    NodeTriangleStripSet(Scene *scene, Proto *proto);

protected:
    virtual        ~NodeTriangleStripSet();

public:
    virtual const char* getComponentName(void) const 
                           { return "Rendering"; }
    virtual int         getComponentLevel(void) const { return 3; }
    virtual int     getX3dVersion(void) const { return 0; }    
    virtual Node *copy() { return new NodeTriangleStripSet(m_scene, m_proto); }

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

    virtual int     colorPerVertexField()
                       { return colorPerVertex_Field(); }
    virtual int     colorIndexField()
                       { return -1; }

    MFVec3f        *getCoordinates();
    MFVec2f        *getTextureCoordinates();

    Node           *toIndexedLineSet(void);

    fieldMacros(SFNode,  color,           ProtoTriangleStripSet)
    fieldMacros(SFNode,  coord,           ProtoTriangleStripSet)
    fieldMacros(SFNode,  normal,          ProtoTriangleStripSet)
    fieldMacros(MFInt32, stripCount,      ProtoTriangleStripSet)
    fieldMacros(SFNode,  texCoord,        ProtoTriangleStripSet)
    fieldMacros(SFBool,  ccw,             ProtoTriangleStripSet)
    fieldMacros(SFBool,  colorPerVertex,  ProtoTriangleStripSet)
    fieldMacros(SFBool,  normalPerVertex, ProtoTriangleStripSet)
    fieldMacros(SFBool,  solid,           ProtoTriangleStripSet)
    ComposedGeometryFieldMacros(ProtoTriangleStripSet)

protected:
    void            createMesh(bool cleanDoubleVertices = true,
                               bool triangulate = true);

protected:
    MFInt32        *m_coordIndex;
    bool            m_colorPerVertexWarning;
};

#endif 
