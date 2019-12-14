/*
 * MyMesh.cpp
 *
 * Copyright (C) 1999 Stephen F. White, 2019 J. "MUFTI" Scheurich
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

#ifndef FLT_MAX
# include <float.h>
#endif

#ifdef TEST
#include "Scene.h"
#include "NodeBox.h"
#endif

#include "MyMesh.h"

template <class X,class MFX,class VEC3X>
Node * 
MyMeshX<X, MFX, VEC3X>::toIndexedFaceSet(int meshFlags, Scene *scene)
{
    bool wantNormal = (meshFlags & MESH_WANT_NORMAL);

    NodeCoordinate *ncoord = (NodeCoordinate *)scene->createNode("Coordinate");
    MFVec3f *points = new MFVec3f();
    MFX *vertices = getVertices();
    for (int i = 0; i < vertices->getSFSize(); i++) {
        VEC3X vec = vertices->getValue(i);
        Vec3f point(vec.x, vec.y, vec.z); 
        points->setVec(i, point);
    } 
    ncoord->point(points);
    NodeIndexedFaceSet *node = (NodeIndexedFaceSet *)
                               scene->createNode("IndexedFaceSet");
    node->coord(new SFNode(ncoord));
    if (wantNormal) {
        NodeNormal *nnormal = NULL;
        if (getSmoothNormals()) {
            nnormal = (NodeNormal *)scene->createNode("Normal");
            nnormal->vector(getSmoothNormals());
        }
        if (nnormal) {
            node->normal(new SFNode(nnormal));
            MFInt32 *ni = getSmoothNormalIndex();
            if (ni != NULL) {
                node->normalIndex(ni);
                node->normalIndex()->ref();
            }
        } 
    } 
    node->colorPerVertex(new SFBool(colorPerVertex()));
    if (getColorIndex())
        node->colorIndex(new MFInt32(getColorIndex()));
    node->normalPerVertex(new SFBool(normalPerVertex()));
    if (getNormalIndex())
        node->normalIndex(new MFInt32(getNormalIndex()));
    node->creaseAngle(new SFFloat(creaseAngle())); 
    node->coordIndex(new MFInt32(getCoordIndex()));
    node->solid(new SFBool(solid()));
    node->ccw(new SFBool(ccw()));
    node->convex(new SFBool(convex()));
    NodeColor *nColor = NULL;
    if (getColors()) {
        nColor = (NodeColor *)
                    scene->createNode("Color");
        nColor->color(new MFColor((float *)getColors()->getValues(), 
                                  getColors()->getSize()));
    }
    if (nColor) {
        node->color(new SFNode(nColor));
        node->color()->ref();
    }
    NodeTextureCoordinate *ntexCoord = NULL;
    if (getTexCoords()) {
        ntexCoord = (NodeTextureCoordinate *)
                    scene->createNode("TextureCoordinate");
        ntexCoord->point(getTexCoords());
    }
    if (ntexCoord) {
        node->texCoord(new SFNode(ntexCoord));
        node->texCoordIndex(getTexCoordIndex());
        node->texCoordIndex()->ref();
    }

    node->ref();

    return node;
}

void MyMesh::drawVertex(float *v)
{
     glVertex3fv(v);
}

void MyMeshDouble::drawVertex(double *v)
{
     glVertex3dv(v);
}

void beginTriangleCallback(GLenum type, void *data)
{
    MyMesh *self = (MyMesh*)data;
    self->startTessellation(type);
}

void newVertexCallback(void *vertex_data, void *data)
{
    MyMesh *self = (MyMesh*)data;
    VertexInfo *vertexInfo = (VertexInfo *) vertex_data;

    self->addNewVertex(vertexInfo);    
}

void endTriangleCallback(void *data) {
    MyMesh *self = (MyMesh*)data;    
    self->endNewPolygon();
}

void 
combineVertices(GLdouble coords[3], GLdouble *vertex_data[4],
                GLfloat weight[4], GLdouble **dataOut, void *data) 
{
    MyMesh *self = (MyMesh*)data;
    VertexInfo *vertexInfo = new VertexInfo();
    self->collectMemoryToFree(vertexInfo);

    vertexInfo->vertex[0] = coords[0];
    vertexInfo->vertex[1] = coords[1];
    vertexInfo->vertex[2] = coords[2];

    VertexInfo *vertexData[4];
    for (int i = 0; i < 4; i++)
        vertexData[i] = (VertexInfo *)vertex_data[i]; 

    for (int i = 0; i < 3; i++) {
        vertexInfo->normal[i] = 0;
        for (int j = 0; j < 4; j++)
            if (vertexData[j] != NULL)
                vertexInfo->normal[i] += weight[j] * vertexData[j]->normal[i];
    }

    for (int i = 0; i < 4; i++) {
        vertexInfo->color[i] = 0;
        for (int j = 0; j < 4; j++)
            if (vertexData[j] != NULL)
                vertexInfo->color[i] += weight[j] * vertexData[j]->color[i];
    }

    for (int i = 0; i < 2; i++) {
        vertexInfo->texCoord[i] = 0;
        for (int j = 0; j < 4; j++)
            if (vertexData[j] != NULL)
                vertexInfo->texCoord[i] += weight[j] * 
                                           vertexData[j]->texCoord[i];
    }

    *dataOut = &vertexInfo->vertex[0];
}


#ifdef _WIN32
void tesselationError(GLenum error) 
{
    swDebugf("%s\n", gluErrorString(error));
}
#endif


#ifdef TEST
int main(int argc, char **argv)
{
    TheApp = new DuneApp();
    Scene* scene = new Scene();
    NodeBox* nodeBox = (NodeBox*)scene->createNode("Box"); 
    nodeBox->toTriangleSet();
    return 0;
}
#endif
