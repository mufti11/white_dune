/*
 * IndexedTriangleSetNode.h
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
#include "SFMFTypes.h"
#include "ComposedGeometryMacros.h"

class IndexedTriangleSetProto : public GeometryProto {
public:
                    IndexedTriangleSetProto(Scene *scene);
                    IndexedTriangleSetProto(Scene *scene, const char *name);

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
};

class IndexedTriangleSetNode : public MeshBasedNode {
public:
                    IndexedTriangleSetNode(Scene *scene, Proto *proto);

protected:
    virtual        ~IndexedTriangleSetNode();

public:
    virtual const char* getComponentName(void) const 
                           { return "Rendering"; }
    virtual int         getComponentLevel(void) const { return 3; }

    virtual void    setField(int index, FieldValue *value, int cf = -1);

    virtual void    draw();

    virtual void    flip(int index);
    virtual void    swap(int fromTo);

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
 
    fieldMacros(SFNode,  color,           IndexedTriangleSetProto)
    fieldMacros(SFNode,  coord,           IndexedTriangleSetProto)
    fieldMacros(SFNode,  normal,          IndexedTriangleSetProto)
    fieldMacros(SFNode,  texCoord,        IndexedTriangleSetProto)
    fieldMacros(SFBool,  ccw,             IndexedTriangleSetProto)
    fieldMacros(SFBool,  normalPerVertex, IndexedTriangleSetProto)
    fieldMacros(SFBool,  colorPerVertex,  IndexedTriangleSetProto)
    fieldMacros(SFBool,  solid,           IndexedTriangleSetProto)
    fieldMacros(MFInt32, index,           IndexedTriangleSetProto)
    ComposedGeometryFieldMacros(IndexedTriangleSetProto)

protected:
    MFInt32        *m_coordIndex;
};
