/*
 * IndexedTrianglesNode
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

#include <stdio.h>
#include <string.h>
#include "stdafx.h"

#include "NodeIndexedTriangleFanSet.h"
#include "Proto.h"
#include "Scene.h"
#include "FieldValue.h"
#include "Node.h"
#include "MyMesh.h"
#include "FaceData.h"
#include "Vec3f.h"
#include "NodeColor.h"
#include "NodeColorRGBA.h"
#include "NodeCoordinate.h"
#include "NodeGeoCoordinate.h"
#include "NodeNormal.h"
#include "NodeTextureCoordinate.h"
#include "NodeIndexedLineSet.h"

IndexedTriangleSetProto::IndexedTriangleSetProto(Scene *scene, const char *name)
  : GeometryProto(scene, name)
{
    color.set(
          addExposedField(SFNODE, "color", new SFNode(NULL), COLOR_NODE));
    coord.set(
          addExposedField(SFNODE, "coord", new SFNode(NULL), COORDINATE_NODE));
    normal.set(
          addExposedField(SFNODE, "normal", new SFNode(NULL), VRML_NORMAL));
    texCoord.set(
          addExposedField(SFNODE, "texCoord", new SFNode(NULL), 
                          TEXTURE_COORDINATE_NODE));
    ccw.set(
          addField(SFBOOL, "ccw", new SFBool(true)));
    colorPerVertex.set(
        addField(SFBOOL, "colorPerVertex", new SFBool(true)));
    normalPerVertex.set(
          addField(SFBOOL, "normalPerVertex", new SFBool(true)));
    solid.set(
          addField(SFBOOL, "solid", new SFBool(true)));
    index.set(
          addField(MFINT32, "index", new MFInt32(), new SFInt32(-1)));
    addEventIn(MFINT32, "set_index", 0, index);
}

IndexedTriangleSetNode::IndexedTriangleSetNode(Scene *scene, Proto *def)
  : MeshBasedNode(scene, def)
{
    m_coordIndex = NULL;
}

IndexedTriangleSetNode::~IndexedTriangleSetNode()
{
    if (m_coordIndex != NULL)
        m_coordIndex->unref();
}

void 
IndexedTriangleSetNode::draw()
{
    Node *ncoord = coord()->getValue();
    if (ncoord != NULL) {
        glPushName(coord_Field());       // field coord
        glPushName(0);                   // index 0
        if (ncoord->getType() == VRML_COORDINATE)
            ((NodeCoordinate *)ncoord)->draw(this);
        else if (ncoord->getType() == VRML_GEO_COORDINATE) {
            setDoubleMesh(true);
            ((NodeGeoCoordinate *)ncoord)->draw(this);
        }
        glPopName();
        glPopName();
    }
}

void
IndexedTriangleSetNode::setField(int index, FieldValue *value, int cf)
{
    m_meshDirty = true;
    Node::setField(index, value, cf);
    update();
}

void
IndexedTriangleSetNode::flip(int index)
{
    NodeCoordinate *ncoord = (NodeCoordinate *)coord()->getValue();
    if (ncoord)
        if (ncoord->getType() == VRML_COORDINATE)
            ncoord->flip(index);
    NodeNormal *nnormal = (NodeNormal *)normal()->getValue();
    if (nnormal)
        if (nnormal->getType() == VRML_NORMAL)
            nnormal->flip(index);
    SFBool *bccw = new SFBool(!(ccw()->getValue()));
    ccw(bccw);
    m_meshDirty = true;
}

void
IndexedTriangleSetNode::swap(int fromTo)
{
    NodeCoordinate *ncoord = (NodeCoordinate *)coord()->getValue();
    if (ncoord)
        if (ncoord->getType() == VRML_COORDINATE) 
            ncoord->swap(fromTo);
    NodeNormal *nnormal = (NodeNormal *)normal()->getValue();
    if (nnormal)
        if (nnormal->getType() == VRML_NORMAL)
            nnormal->swap(fromTo);
    SFBool *bccw = new SFBool(!(ccw()->getValue()));
    ccw(bccw);
    m_meshDirty = true;
}

