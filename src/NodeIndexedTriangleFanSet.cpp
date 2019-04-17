/*
 * NodeIndexedTriangleFanSet.cpp
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
#include "NodeFogCoordinate.h"
#include "Util.h"

ProtoIndexedTriangleFanSet::ProtoIndexedTriangleFanSet(Scene *scene)
  : IndexedTriangleSetProto(scene, "IndexedTriangleFanSet")
{
}

Node *
ProtoIndexedTriangleFanSet::create(Scene *scene)
{ 
      Node *node = new NodeIndexedTriangleFanSet(scene, this);
      return node;
}

NodeIndexedTriangleFanSet::NodeIndexedTriangleFanSet(Scene *scene, Proto *def)
  : IndexedTriangleSetNode(scene, def)
{
    m_colorPerVertexWarning = false;
}

NodeIndexedTriangleFanSet::~NodeIndexedTriangleFanSet()
{
}

void
NodeIndexedTriangleFanSet::createMesh(bool cleanDouble, bool triangulate)
{
    Node *coord = ((SFNode *) getField(coord_Field()))->getValue();
    bool bcolorPerVertex = colorPerVertex()->getValue();
    bool bnormalPerVertex = normalPerVertex()->getValue();
   
    if (!coord || ((NodeCoordinate *) coord)->point()->getType() != MFVEC3F)
        return;

    MFVec3f *coords = NULL;
    MFVec3d *coordsDouble = NULL;
    if (coord->getType() == VRML_COORDINATE)
        coords = ((NodeCoordinate *)coord)->point();
    else
        coordsDouble = ((NodeGeoCoordinate *)coord)->pointX3D();

    MFVec3f *normals = NULL;
    MFFloat *colors = NULL;

    if (normal()->getValue())
        if (normal()->getValue()->getType() == VRML_NORMAL)
            normals = ((NodeNormal *)(normal()->getValue()))->vector();
    
    int meshFlags = 0;
    if (color()->getValue()) {
        if (color()->getValue()->getType() == VRML_COLOR) 
            colors = ((NodeColor *)(color()->getValue()))->color();
        else if (color()->getValue()->getType() == X3D_COLOR_RGBA) {
            colors = ((NodeColorRGBA *)(color()->getValue()))->color();
            meshFlags |= MESH_COLOR_RGBA;
        }
    }    

    if (m_coordIndex != NULL)
        m_coordIndex->unref();
    m_coordIndex = new MFInt32();    
    m_coordIndex->ref();

    MFInt32 *indices = index();

    if (coords->getSFSize() > 0) {
        int fanSetFirst = -1;
        int fanSetSecond = -1;
        for (int i = 0; i < indices->getSize(); i++) {
            int currentIndex = indices->getValue(i);
            if (currentIndex == -1) {
                fanSetFirst = -1;
                fanSetSecond = -1;
            } else {
                if (fanSetFirst == -1) {
                    // first vertex of fan
                    fanSetFirst = i;
                } else if (fanSetSecond == -1) {
                    // second vertex of fan
                    fanSetSecond = i;
                } else {
                    // other vertices of fan
                    m_coordIndex->appendSFValue(indices->getValue(fanSetFirst));
                    m_coordIndex->appendSFValue(indices->getValue(fanSetSecond));
                    m_coordIndex->appendSFValue(currentIndex);
                    m_coordIndex->appendSFValue(-1);
                    fanSetSecond = i;
                }
            }
        }
    }

    MFFloat *fogCoords = NULL;
    if (fogCoord()->getValue())
        if (fogCoord()->getValue()->getType() == X3D_FOG_COORDINATE)
            fogCoords = ((NodeFogCoordinate *) 
                         (fogCoord()->getValue()))->depth();

    MyArray<MFVec2f *> texCoords;
    Util::getTexCoords(texCoords, texCoord()->getValue());    
    
//    if (!texCoord()->getValue())
//        texCoords = generateTextureCoordinates(coords, texCoordIndex);
    float transparency = 0;
    if (hasParent())
        transparency = getParent()->getTransparency();
    if (ccw()->getValue())
        meshFlags |= MESH_CCW;
    if (solid()->getValue())
        meshFlags |= MESH_SOLID;
    // according to 
    // http://www.web3d.org/x3d/specifications/ISO-IEC-19775-X3DAbstractSpecification/Part01/components/rendering.html#TriangleSet
    // The value of the colorPerVertex field is ignored and always treated as 
    // TRUE. 
    if (!bcolorPerVertex && !m_colorPerVertexWarning) {
        m_scene->warning("Warning: According to chapter 11.4.13 TriangleSet of ISO/IEC 19775-1:2008, the value of the colorPerVertex field is ignored and always treated as TRUE");
        m_colorPerVertexWarning = true;
    }
    meshFlags |= MESH_COLOR_PER_VERTEX;
    if (bnormalPerVertex)
        meshFlags |= MESH_NORMAL_PER_VERTEX;
    float creaseAngle = M_PI / 2.0f;

    if (coords)
        m_mesh = new MyMesh(this, coords, m_coordIndex, normals, NULL, colors, 
                            NULL, texCoords, NULL, creaseAngle, meshFlags, 
                            transparency, fogCoords);
   else
        m_meshDouble = new MyMeshDouble(this, coordsDouble, m_coordIndex, 
                                        normals, NULL, colors, NULL, texCoords, 
                                        NULL, creaseAngle, meshFlags, 
                                        transparency, fogCoords);
}
