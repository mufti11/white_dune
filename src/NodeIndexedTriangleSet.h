/*
 * NodeIndexedTriangleSet.h
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
#include "IndexedTriangleSetNode.h"

class ProtoIndexedTriangleSet : public IndexedTriangleSetProto {
public:
                    ProtoIndexedTriangleSet(Scene *scene);
    virtual Node   *create(Scene *scene);

    virtual int     getType() const { return X3D_INDEXED_TRIANGLE_SET; }

    virtual bool    isX3dInternalProto(void) { return true; }

    virtual bool    isMesh(void) { return true; }
    virtual bool    isExportTargetMesh(void) { return true; }

    x3domGeometryCommonFieldIndex()
    FieldIndex normalUpdateMode;
};

class NodeIndexedTriangleSet : public IndexedTriangleSetNode, Colored {
public:
                    NodeIndexedTriangleSet(Scene *scene, Proto *proto);

protected:
    virtual        ~NodeIndexedTriangleSet();

public:
    virtual int     getX3dVersion(void) const { return 0; }
    virtual Node   *copy() { return new NodeIndexedTriangleSet(
                              m_scene, m_proto); }

    virtual int     getNormalField() { return normal_Field(); }
    virtual int     getTexCoordField() { return texCoord_Field(); }

//    virtual void    setNormalFromMesh(Node *nnormal);
//    virtual void    setTexCoordFromMesh(Node *ntexCoord);

    virtual bool    shouldConvertToTriangleSet(void) { return false; }
    virtual bool    shouldConvertToIndexedTriangleSet(void) { return false; }

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

    virtual bool    showFields() { return true; } 

    x3domGeometryCommonFieldMacros(ProtoIndexedTriangleSet)
    fieldMacros(SFString, normalUpdateMode, ProtoIndexedTriangleSet)

protected:
    void            createMesh(bool cleanDoubleVertices = true,
                               bool triangulate = true);

protected:
    bool            m_colorPerVertexWarning;
};
