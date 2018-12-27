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


