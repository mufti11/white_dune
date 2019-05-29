/*
 * NodeTextureCoordinate.cpp
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
#include "stdafx.h"

#include "NodeTextureCoordinate.h"
#include "Proto.h"
#include "FieldValue.h"
#include "Scene.h"
#include "NodeIndexedFaceSet.h"
#include "MFVec3f.h"
#include "MFVec2f.h"
#include "MFInt32.h"
#include "MyMesh.h"

ProtoTextureCoordinate::ProtoTextureCoordinate(Scene *scene)
  : WonderlandExportProto(scene, "TextureCoordinate")
{
    point.set(addExposedField(MFVEC2F, "point", new MFVec2f()));
}

Node *
ProtoTextureCoordinate::create(Scene *scene)
{ 
    return new NodeTextureCoordinate(scene, this); 
}

NodeTextureCoordinate::NodeTextureCoordinate(Scene *scene, Proto *def)
  : Node(scene, def)
{
}

void
NodeTextureCoordinate::setField(int index, FieldValue *value, int cf)
{
    Node::setField(index, value, cf);
    if (hasParent())
        getParents().update();
}

void
NodeTextureCoordinate::setTextureCoordinateFromIndexedFaceSet()
{
    Node* parent = getParent();
    if (parent && parent->getType() == VRML_INDEXED_FACE_SET) {
        NodeIndexedFaceSet *faceSet = (NodeIndexedFaceSet*) parent;
        MFVec3f *coords = faceSet->getCoordinates();   
        MFInt32 *coordIndex = faceSet->getCoordIndex();
        MFInt32 *texCoordIndex = faceSet->getTexCoordIndex();       
        if (texCoordIndex->getSize() != coordIndex->getSize()) {
            texCoordIndex = coordIndex;
        }
    MFVec2f *tc = generateTextureCoordinates(coords, texCoordIndex, 0.0f);
    if (tc != NULL)
        setField(point_Field(), tc);
    }
}

void
NodeTextureCoordinate::update()
{
    if (hasParent()) {
        for (int i = 0; i < getNumParents(); i++) {
            Node *parent = getParent(i);
            if (parent->getType() == X3D_MULTI_TEXTURE_COORDINATE) {
                parent->update();
            }
        }
    }        
}

