/*
 * NodeGeometryExample.cpp
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
 * not, write to the Free Software Foundation, Inc., 
 * 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

// the following is a typical template for a geometric node,
// that is animatable (not rigid during time)

#include <stdio.h>
#include "stdafx.h"

#include "NodeGeometryExample.h"
#include "Proto.h"
#include "DuneApp.h"
#include "FieldValue.h"
#include "Scene.h"
#include "Mesh.h"
#include "SFSomething.h"
#include "MFSomething.h"
#include "SFBool.h"
#include "RenderState.h"
#include "Util.h"

ProtoGeometryExample::ProtoGeometryExample(Scene *scene)
  : Proto(scene, "GeometryExample")
{
    field1.set( 
          addExposedField(SFSOMETHING, "field1", new SFSomething(args)));
    field2.set( 
          addExposedField(MFSOMETHING, "field2", new MFSomething(args)));

    // creaseAngle and solid are typical fields needed for geometry,
    // see IndexedFaceSet in the VRML97 specification for further description
    creaseAngle.set(
          addField(SFFLOAT, "creaseAngle", new SFFloat(0.0), 
                   new SFFloat(0.0f)));
    solid.set( 
          addField(SFBOOL, "solid", new SFBool(false)));
}

Node *
ProtoGeometryExample::create(Scene *scene)
{
    return new NodeGeometryExample(scene, this); 
}

NodeGeometryExample::NodeGeometryExample(Scene *scene, Proto *def)
  : MeshMorphingNode(scene, def)
{
}

NodeGeometryExample::~NodeGeometryExample()
{
}

void
NodeGeometryExample::createMesh(bool cleanDoubleVertices)
{
    GeometryExampleData data;
    data.field1 = field1()->getValue();
    data.field2 = field2()->getValue();
    data.creaseAngle = creaseAngle()->getValue(); 

    data.cleanDoubleVertices = cleanDoubleVertices;

    createMesh(data);    
}

void
NodeGeometryExample::createMesh(GeometryExampleData &data)
{
    if (_mesh)
        delete _mesh;
    _mesh = NULL;

    sfsomething sffield1 = data.field1;
    mfsomething mffield2 = data.field2;
    bool bsolid = solid()->getValue();

    // edge points of your geometry
    MFVec3f *vert = new MFVec3f(...);
    // the coordIndex values 0 1 2 -1 represent a triangle made of first, second and third edge in "vert"
    MFInt32 *coordIndex = new MFInt32(...);
    int meshFlags = 0;
    if (bsolid)
        meshFlags |= MESH_SOLID;

    _mesh = new Mesh(vert, coordIndex, NULL, NULL, NULL, NULL, NULL, NULL, 
                     data.creaseAngle, meshFlags);
}

/*
// use the following function, if you build a scriptedNode
int
NodeGeometryExample::writeProto(int f)
{
    return ((Node *)this)->writeProto(f, "", "scripted_Nodes"
#ifdef HAVE_SCRIPTED_NODES_PROTO_URL
                                            , HAVE_SCRIPTED_NODES_PROTO_URL
#endif
                                            );
}
*/

/*
// use the following function, if you want to create IndexedFaceSet data for pure VRML97 export
int             
NodeGeometryExample::write(int filedes, int indent) 
{
    if (_scene->isPureVRML()) {
        _meshDirty = true;
        Node *node = toIndexedFaceSet(TheApp->GetNormalsOnPureVRML97(), true);
        if (!writeCoordinateInterpolator(node, filedes, indent, true)) {
            node = toIndexedFaceSet(TheApp->GetNormalsOnPureVRML97(), false);
            writeCoordinateInterpolator(node, filedes, indent, false);
        } else
            writeCoordinateInterpolator(node, filedes, indent, true);
        RET_ONERROR( node->write(filedes, indent) )
        node->unref();
        _meshDirty = true;
    } else
        RET_ONERROR( NodeData::write(filedes, indent) )
    return 0;
}
*/

void
NodeGeometryExample::setField(int index, FieldValue *value)
{
    _meshDirty = true;
    Node::setField(index, value);
}

void * 
NodeGeometryExample::initializeData(void)
{
    GeometryExampleData *data = new GeometryExampleData();

    data->field1 = field1()->getValue();
    data->field2 = field2()->getValue();
    data->creaseAngle = creaseAngle()->getValue();

    return data;
}


void
NodeGeometryExample::loadDataFromInterpolators(void* GeometryExampleData, Interpolator *inter, int field, float key)
{
    GeometryExampleData *data = (GeometryExampleData *)GeometryExampleData;
    // all interpolators are scalarInterpolators, 
    // one float is sufficent as target of "interpolate()"
    if (field == field1_Field())
        inter->interpolate(key, &(data->field1));
    else if (field == field2_Field())
        inter->interpolate(key, &(data->field2));
    else if (field == creaseAngle_Field())
        inter->interpolate(key, &(data->creaseAngle));
    else
        assert(0);
}

void
NodeGeometryExample::createMeshFromData(void* GeometryExampleData, bool optimize)
{
    _meshDirty = true;
    GeometryExampleData *data = (GeometryExampleData *)GeometryExampleData;
    data->cleanDoubleVertices = optimize;
    createMesh(*data);
}

void
NodeGeometryExample::finalizeData(void* data)
{
    delete (GeometryExampleData *) data;
}


void
NodeGeometryExample::drawHandles()
{

    RenderState state;

    glPushMatrix();
    glPushAttrib(GL_LIGHTING);
    glDisable(GL_LIGHTING);
    glPushName(0);
        
    state.startDrawHandles();
    for (int i = 0; i < some_size; i++) {
        state.setHandleColor(_scene, i);
        glLoadName(i);
        state.drawHandle(some_vertex(i));
    }
    state.endDrawHandles();
    glPopName();
    glPopAttrib();
    glPopMatrix(); 
}

Vec3f
NodeGeometryExample::getHandle(int handle, int *constraint , int *field)
{
    *field = 0;
    switch (handle) {
      case something:
        return Vec3f(something);
      default:
        assert(0);
        return Vec3f(1.0f, 1.0f, 1.0f);
    }
}

void 
NodeGeometryExample::setHandle(int handle, const Vec3f &v)
{
    switch (handle) {
      case something:
        _scene->setField(this, size_Field(), new SFVec3f(Vec3f(something(v)))); 
        break;
    }
}

// some shapes can be flipped on one direction more intelligent than
// convert to mesh and then flip this direction
// e.g. a cone only need to flip the y direction, but not the x and z direction

void
NodeGeometryExample::flip(int index)
{
    // 0 == x / 1 == y / 2 == z
    if (index == 1) {
        MeshBasedNode::flip(index);
}

// some rare shapes can swap directions more intelligent than
// convert to mesh and then swap directions
// e.g. a cone do not need to swap the x/z directions

void
NodeGeometryExample::swap(int fromTo)
{
    if (fromTo == SWAP_XY) {
        Node *mesh = _scene->convertToIndexedFaceSet(this);
        if (mesh != NULL)
            mesh->swap(fromTo);
    }
}


