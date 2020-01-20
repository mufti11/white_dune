/*
 * NodeTriangleSet2D.h
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

#pragma once

#include "MeshBasedNode.h"
#include "ProtoMacros.h"
#include "Proto.h"
#include "DuneApp.h"
#include "MyMesh.h"
#include "Vec3f.h"
#include "NodeCoordinate.h"
#include "SFMFTypes.h"

class ProtoTriangleSet2D : public GeometryProto {
public:
                    ProtoTriangleSet2D(Scene *scene);
    virtual Node   *create(Scene *scene);

    virtual int     getType() const { return X3D_TRIANGLE_SET_2D; }

    virtual bool    isX3dInternalProto(void) { return true; }

    virtual bool    isMesh(void) { return true; }

    FieldIndex vertices;
    FieldIndex solid;
    x3domGeometryCommonFieldIndex()
};

class NodeTriangleSet2D : public MeshBasedNode {
public:
                    NodeTriangleSet2D(Scene *scene, Proto *proto);

protected:
    virtual        ~NodeTriangleSet2D();

public:
    virtual int     getProfile(void) const;
    virtual const char* getComponentName(void) const 
                           { return "Geometry2D"; }
    virtual int         getComponentLevel(void) const { return 1; }
    virtual int     getX3dVersion(void) const { return 0; }
    virtual Node   *copy() const { return new NodeTriangleSet2D(*this); }

    virtual void    setField(int index, FieldValue *value, int cf = -1);

    virtual bool    hasTwoSides(void) { return true; }
    virtual bool    isDoubleSided(void) { return !solid()->getValue(); }
    virtual void    toggleDoubleSided(void)
                       { solid(new SFBool(!solid()->getValue())); }
    virtual int     getSolidField() { return solid_Field(); }

    virtual bool    maySetDefault(void) { return false; }

    virtual void    draw() { meshDraw(); }

    virtual int     writeProto(int f) { return writeX3dProto(f); }

    fieldMacros(MFVec2f, vertices, ProtoTriangleSet2D)
    fieldMacros(SFBool,  solid,    ProtoTriangleSet2D)
    x3domGeometryCommonFieldMacros(ProtoTriangleSet2D)

protected:
    virtual void    createMesh(bool cleanDoubleVertices = true,
                               bool triangulate = true);

protected:
    MFInt32        *m_coordIndex;
    bool            m_colorPerVertexWarning;
};

