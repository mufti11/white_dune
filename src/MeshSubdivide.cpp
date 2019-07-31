/*
 * MeshSubdivide.cpp
 *
 * Copyright (C) 2017 J. "MUFTI" Scheurich
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

#include "stdafx.h"

#include "NodeIndexedFaceSet.h"
#include "NodeCoordinate.h"
#include "Scene.h"
#include "Vec3f.h"
#include "FaceData.h"
#include "MyMesh.h"

#ifdef HAVE_LIBOSDCPU

// based on far_tutorial_1_1.cpp of OpenSubdiv

//
//   Copyright 2013 Pixar
//
//   Licensed under the Apache License, Version 2.0 (the "Apache License")
//   with the following modification; you may not use this file except in
//   compliance with the Apache License and the following modification to it:
//   Section 6. Trademarks. is deleted and replaced with:
//
//   6. Trademarks. This License does not grant permission to use the trade
//      names, trademarks, service marks, or product names of the Licensor
//      and its affiliates, except as required to comply with Section 4(c) of
//      the License and to reproduce the content of the NOTICE file.
//
//   You may obtain a copy of the Apache License at
//
//       http://www.apache.org/licenses/LICENSE-2.0
//
//   Unless required by applicable law or agreed to in writing, software
//   distributed under the Apache License with the above modification is
//   distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
//   KIND, either express or implied. See the Apache License for the specific
//   language governing permissions and limitations under the Apache License.
//

#include <opensubdiv/far/topologyDescriptor.h>
#include <opensubdiv/far/primvarRefiner.h>

#include <cstdio>

//------------------------------------------------------------------------------
// Vertex container implementation.
//
struct Vertex {

    // Minimal required interface ----------------------
    Vertex() { }

    Vertex(Vertex const & src) {
        _position[0] = src._position[0];
        _position[1] = src._position[1];
        _position[2] = src._position[2];
    }

    void Clear( void * =0 ) {
        _position[0]=_position[1]=_position[2]=0.0f;
    }

    void AddWithWeight(Vertex const & src, float weight) {
        _position[0]+=weight*src._position[0];
        _position[1]+=weight*src._position[1];
        _position[2]+=weight*src._position[2];
    }

    // Public interface ------------------------------------
    void SetPosition(float x, float y, float z) {
        _position[0]=x;
        _position[1]=y;
        _position[2]=z;
    }

    const float * GetPosition() const {
        return _position;
    }

private:
    float _position[3];
};

//------------------------------------------------------------------------------

using namespace OpenSubdiv;

bool
meshSubdivide(NodeIndexedFaceSet* node, MyMesh *mesh)
{
    Scene *scene = node->getScene(); 
    mesh->optimize(TheApp->GetHandleEpsilon());
    MFVec3f *vertices = mesh->getVertices();
    MFInt32 *ci = mesh->getCoordIndex();

    // Populate a topology descriptor with our raw data

    typedef Far::TopologyDescriptor Descriptor;

    Sdc::SchemeType type = OpenSubdiv::Sdc::SCHEME_CATMARK;

    Sdc::Options options;
    options.SetVtxBoundaryInterpolation(Sdc::Options::VTX_BOUNDARY_EDGE_ONLY);

    MyArray<int> vertsperface;
    MyArray<int> vertIndices;
    for (int i = 0; i < mesh->getNumFaces(); i++) {
        FaceData *face = mesh->getFace(i);
        vertsperface.append(face->getNumVertices());
        int offset = face->getOffset();
        int numVertices = face->getNumVertices();
        for (int j = 0; j < numVertices; j++)
            vertIndices.append(ci->getValue(offset + j));
    }

    Descriptor desc;
    desc.numVertices  = vertices->getSFSize();
    desc.numFaces     = mesh->getNumFaces();
    desc.numVertsPerFace = vertsperface.getData();
    desc.vertIndicesPerFace = vertIndices.getData();

    // Instantiate a Far::TopologyRefiner from the descriptor
    Far::TopologyRefiner *refiner = 
        Far::TopologyRefinerFactory<Descriptor>::Create(desc,
        Far::TopologyRefinerFactory<Descriptor>::Options(type, options));

    int maxlevel = 1;

    // Uniformly refine the topology up to 'maxlevel'
    refiner->RefineUniform(Far::TopologyRefiner::UniformOptions(maxlevel));

    // Allocate a buffer for vertex primvar data. The buffer length is set to
    // be the sum of all children vertices up to the highest level of refinement.
    std::vector<Vertex> vbuffer(refiner->GetNumVerticesTotal());
    Vertex* verts = &vbuffer[0];

    // Initialize coarse mesh positions
    int nCoarseVerts = vertices->getSFSize();
    for (int i=0; i<nCoarseVerts; ++i) {
        verts[i].SetPosition(vertices->getValue(i)[0], 
                             vertices->getValue(i)[1], 
                             vertices->getValue(i)[2]);
    }

    // Interpolate vertex primvar data
    Far::PrimvarRefiner primvarRefiner(*refiner);

    Vertex *src = verts;
    for (int level = 1; level <= maxlevel; ++level) {
        Vertex *dst = src + refiner->GetLevel(level-1).GetNumVertices();
        primvarRefiner.Interpolate(level, src, dst);
        src = dst;
    }


    MFVec3f *newPoint = new MFVec3f();
    MFInt32 *newCi = new MFInt32();

    { // Output data of the highest level refined -----------

        Far::TopologyLevel const &refLastLevel = refiner->GetLevel(maxlevel);

        int nverts = refLastLevel.GetNumVertices();
        int nfaces = refLastLevel.GetNumFaces();

        // Output vertex positions
        int firstOfLastVerts = refiner->GetNumVerticesTotal() - nverts;

        for (int vert = 0; vert < nverts; ++vert) {
            float const* pos = verts[firstOfLastVerts + vert].GetPosition();
            newPoint->setSFValue(vert, pos[0], pos[1], pos[2]);
        }

        // Output faces
        for (int face = 0; face < nfaces; ++face) {

            Far::ConstIndexArray fverts = refLastLevel.GetFaceVertices(face);

            // all refined Catmark faces should be quads
            // assert(fverts.size()==4);

            for (int vert=0; vert<fverts.size(); vert += 4) {
                newCi->appendSFValue(fverts[vert]);
                newCi->appendSFValue(fverts[vert + 1]);
                newCi->appendSFValue(fverts[vert + 2]);
                newCi->appendSFValue(fverts[vert + 3]);
                newCi->appendSFValue(-1);
            }
        }
    }

    NodeCoordinate *coordinate = (NodeCoordinate *)node->coord()->getValue();

    if (node->getMesh() == mesh) {
        scene->backupFieldsStart();
        scene->backupFieldsAppend(node, node->coordIndex_Field());
        scene->backupFieldsAppend(node, node->normal_Field());
        scene->backupFieldsAppend(node, node->ccw_Field());
        scene->backupFieldsAppend(node, node->solid_Field());
        scene->backupFieldsAppend(coordinate, coordinate->point_Field());
        scene->backupFieldsDone();
    }

    scene->setField(node, node->coordIndex_Field(), newCi);
    scene->setField(node, node->normal_Field(), new SFNode());
    scene->setField(node, node->ccw_Field(), new SFBool(mesh->ccw()));
    scene->setField(node, node->solid_Field(), new SFBool(mesh->solid()));    
    scene->setField(coordinate, coordinate->point_Field(), newPoint);

    return true;
}

#else

extern "C" {
#include "subd_mesh.h"
#include "subd.h"
}

bool
meshSubdivide(NodeIndexedFaceSet* node, MyMesh *mesh)
{
    Scene *scene = node->getScene(); 
    mesh->optimize(TheApp->GetHandleEpsilon());
    MFVec3f *vertices = mesh->getVertices();
    MFInt32 *ci = mesh->getCoordIndex();

    struct mesh *subdMesh = mesh_create();
    for (int i = 0; i < vertices->getSFSize(); i++)
        mesh_add_vertex(subdMesh, vertices->getValue(i));

    for (int i = 0; i < mesh->getNumFaces(); i++) {
        FaceData *face = mesh->getFace(i);
        int offset = face->getOffset();
        int numVertices = face->getNumVertices();
        if (numVertices > 0)
            mesh_begin_face(subdMesh);
        if (numVertices > 0)
            mesh_end_face(subdMesh);
        for (int j = offset; j < offset + numVertices; j++)
            if (!((j == offset + numVertices -1) && 
                  (ci->getValue(j) == ci->getValue(offset))))
                mesh_add_index(subdMesh, ci->getValue(j), ci->getValue(j));
    }

    struct mesh *newMesh = subdivide(subdMesh, 1);
    mesh_free(subdMesh);

    if (newMesh == NULL)
        return false;

    MFVec3f *newPoint = new MFVec3f();
    MFInt32 *newCi = new MFInt32();

    int count = 0;
    for (int i = 0; i < mesh_face_count(newMesh); i++) {
        for (int j = 0; j < mesh_face_vertex_count(newMesh, i); j++) {
            newPoint->setSFValue(count, mesh_get_vertex(newMesh, i, j));
            newCi->appendSFValue(count);
            count++;

        }
        newCi->appendSFValue(-1);
    }

    NodeCoordinate *coordinate = (NodeCoordinate *)node->coord()->getValue();

    mesh_free(newMesh);

    if (node->getMesh() == mesh) {
        scene->backupFieldsStart();
        scene->backupFieldsAppend(node, node->coordIndex_Field());
        scene->backupFieldsAppend(node, node->normal_Field());
        scene->backupFieldsAppend(node, node->ccw_Field());
        scene->backupFieldsAppend(node, node->solid_Field());
        scene->backupFieldsAppend(coordinate, coordinate->point_Field());
        scene->backupFieldsDone();
    }

    scene->setField(node, node->coordIndex_Field(), newCi);
    scene->setField(node, node->normal_Field(), new SFNode());
    scene->setField(node, node->ccw_Field(), new SFBool(mesh->ccw()));
    scene->setField(node, node->solid_Field(), new SFBool(mesh->solid()));    
    scene->setField(coordinate, coordinate->point_Field(), newPoint);

    return true;
}

#endif

