/*
 * NodeElevationGrid.cpp
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

#include "NodeElevationGrid.h"
#include "MFColor.h"
#include "MFFloat.h"
#include "MFInt32.h"
#include "MFVec2f.h"
#include "MFVec3f.h"
#include "SFBool.h"
#include "SFFloat.h"
#include "SFInt32.h"
#include "SFNode.h"
#include "Scene.h"
#include "MyMesh.h"
#include "RenderState.h"
#include "NodeNormal.h"
#include "NodeColor.h"
#include "NodeColorRGBA.h"
#include "NodeTextureCoordinate.h"
#include "Util.h"
#include "Field.h"
#include "resource.h"


ProtoElevationGrid::ProtoElevationGrid(Scene *scene)
  : GeometryProto(scene, "ElevationGrid")
{
    attrib.set(
          addExposedField(MFNODE, "attrib", new MFNode(),
                          VERTEX_ATTRIBUTE_NODE));
    setFieldFlags(attrib, FF_X3D_ONLY); 

    color.set(
          addExposedField(SFNODE, "color", new SFNode(NULL), COLOR_NODE));

    fogCoord.set(
          addExposedField(SFNODE, "fogCoord", new SFNode(NULL), 
                          X3D_FOG_COORDINATE));
    setFieldFlags(fogCoord, FF_X3D_ONLY);

    normal.set(
          addExposedField(SFNODE, "normal", new SFNode(NULL), VRML_NORMAL));

    texCoord.set(
          addExposedField(SFNODE, "texCoord", 
                          new SFNode(NULL), TEXTURE_COORDINATE_NODE));

    ccw.set(
          addField(SFBOOL, "ccw", new SFBool(true)));

    colorPerVertex.set(
          addField(SFBOOL, "colorPerVertex", new SFBool(true)));

    creaseAngle.set(
          addField(SFFLOAT, "creaseAngle", new SFFloat(0.0f), 
                   new SFFloat(0.0f)));

    height.set(
          addField(MFFLOAT, "height", new MFFloat()));

    normalPerVertex.set(
          addField(SFBOOL, "normalPerVertex", new SFBool(true)));

    solid.set(
          addField(SFBOOL, "solid", new SFBool(true)));

    xDimension.set(
          addField(SFINT32, "xDimension", new SFInt32(0), new SFInt32(0)));

    xSpacing.set(
          addField(SFFLOAT, "xSpacing", new SFFloat(1.0f), new SFFloat(0)));

    zDimension.set(
          addField(SFINT32, "zDimension", new SFInt32(0), new SFInt32(0)));

    zSpacing.set(
          addField(SFFLOAT, "zSpacing", new SFFloat(1.0f), new SFFloat(0)));

    x3domGeometryCommonFields()

    addEventIn(MFFLOAT, "set_height", EIF_RECOMMENDED, height);
}

Node *
ProtoElevationGrid::create(Scene *scene)
{ 
    return new NodeElevationGrid(scene, this); 
}

NodeElevationGrid::NodeElevationGrid(Scene *scene, Proto *def)
  : MeshBasedNode(scene, def)
{
}

NodeElevationGrid::~NodeElevationGrid()
{
}

#define HEIGHT(i, j) ((i) + (j) * ixDimension < height()->getSize() ? \
                      fheight[(i) + (j) * ixDimension] : 0)

void
NodeElevationGrid::createMesh(bool cleanDoubleVertices, bool triangulate)
{
    Node *ncolor = color()->getValue();
    Node *nnormal = normal()->getValue();
    const float *fheight = height()->getValues();
    int ixDimension = xDimension()->getValue();
    int izDimension = zDimension()->getValue();

    if (ixDimension == 0 || izDimension == 0) return;

    MFVec3f *normals = nnormal ? ((NodeNormal *)nnormal)->vector() : NULL;
    int meshFlags = 0;   
    int numColorComponents = 3;
    MFFloat *colors = NULL;
    int colorSize = 0;
    MFFloat *localColors = NULL;
    MFInt32 *colorIndex = NULL;
    if (ncolor) {
        if (ncolor->getType() == VRML_COLOR) {
            colors = ((NodeColor *)ncolor)->color();
            colorSize = ((NodeColor *)ncolor)->color()->getSize();
        } else if (ncolor->getType() == X3D_COLOR_RGBA) {
            colors = ((NodeColorRGBA *)ncolor)->color();
            colorSize = ((NodeColorRGBA *)ncolor)->color()->getSize();
            meshFlags |= MESH_COLOR_RGBA;
            numColorComponents = 4;
        }
        if (colors && colorPerVertex()->getValue()) 
            localColors = new MFFloat(colors->getValues(), colors->getSize());
        else {
            localColors = new MFFloat();
            int numColors = 0;
            int nColorC = numColorComponents;
            for (int iz = 0; iz < izDimension - 1; iz++) {
                for (int ix = 0; ix < ixDimension - 1; ix++) {
                    for (int j = 0; j < nColorC; j++) {
                        int index = (iz * (ixDimension - 1) + ix) * nColorC + j;
                        int localIndex = numColors * nColorC * 2 + j;
                        if (index < colorSize) {
                            localColors->setValue(localIndex, 
                                                  colors->getValue(index));
                            localColors->setValue(localIndex + nColorC, 
                                                  colors->getValue(index));
                        } else
                            swDebugf("not enough colors in ElevationGrid\n"); 
                    }
                    numColors++;
                }
            }
        }
    }

    int size = ixDimension * izDimension;
    float *vertices = new float[size * 3];

    int index = 0;
    int cindex = 0;

    int *ci = new int[size * 8];

    bool warning = false;
    for (int j = 0; j < izDimension; j++) {
        for (int i = 0; i < ixDimension; i++) {
            if (((i + j * ixDimension) > height()->getSize()) && !warning) {
                MyString valError = "";
                valError += "validation error: ElevationGrid.height size ";
                valError.catInt(height()->getSize());
                valError += " do not fit to ixDimension=";
                valError.catInt(ixDimension);
                valError += " * izDimension=";
                valError.catInt(izDimension);
                TheApp->PrintMessageWindows(valError);
                warning = true;
                continue;
            }
            vertices[index * 3    ] = i * xSpacing()->getValue();
            vertices[index * 3 + 1] = HEIGHT(i, j);
            vertices[index * 3 + 2] = j * zSpacing()->getValue();
            index++;
            if ((j < izDimension-1) && (i < ixDimension-1)) {
                ci[cindex++] = j * ixDimension + i;
                ci[cindex++] = (j+1) * ixDimension + (i+1);
                ci[cindex++] = j * ixDimension + (i+1);
                ci[cindex++] = -1;
                ci[cindex++] = j * ixDimension + i;
                ci[cindex++] = (j+1) * ixDimension + i;
                ci[cindex++] = (j+1) * ixDimension + (i+1);
                ci[cindex++] = -1;
            }
        }
    }
    MFVec3f *v = new MFVec3f(vertices, size * 3);
    MFInt32 *coordIndex = new MFInt32(ci, cindex);
    if (colorPerVertex()->getValue()) { 
        colorIndex = coordIndex;
        colorIndex->ref();
    }

    MyArray<MFVec2f *> texCoords;
    Util::getTexCoords(texCoords, texCoord()->getValue());    

    if (ccw()->getValue())
        meshFlags |= MESH_CCW;
    if (solid()->getValue())
        meshFlags |= MESH_SOLID;
    if (colorPerVertex()->getValue())
        meshFlags |= MESH_COLOR_PER_VERTEX;
    if (normalPerVertex()->getValue())
        meshFlags |= MESH_NORMAL_PER_VERTEX;

    m_mesh = new MyMesh(this, v, coordIndex, normals, NULL,
                       colorPerVertex()->getValue() ? colors : localColors, 
                       /*colorPerVertex()->getValue() ? colorIndex : */NULL, 
                       texCoords, NULL, 
                       creaseAngle()->getFixedAngle(m_scene->getUnitAngle()), 
                       meshFlags, 0, NULL, texCoord()->getValue());
}

MFVec2f *
NodeElevationGrid::accountTexCoord(void)
{
    int ixDimension = xDimension()->getValue();
    int izDimension = zDimension()->getValue();
    int size = ixDimension * izDimension;

    float *tc = new float[size * 2];
    
    int index = 0;
    for (int j = 0; j < izDimension; j++)
        for (int i = 0; i < ixDimension; i++) {
            tc[index * 2    ] = i / (ixDimension - 1.0f);
            tc[index * 2 + 1] = j / (izDimension - 1.0f);
            index++;
    }

    return new MFVec2f(tc, size * 2);
}

void
NodeElevationGrid::drawHandles(void)
{
    const float  *fheight = height()->getValues();
    int ixDimension = xDimension()->getValue();
    int izDimension = zDimension()->getValue();
    RenderState state;

    if (ixDimension < 2 || izDimension < 2) return;

    if (height()->getSize() != ixDimension * izDimension)
        return;

    glDisable(GL_LIGHTING);
    glPushName(0);
    state.startDrawHandles();
    for (int j = 0; j < izDimension; j++) {
        for (int i = 0; i < ixDimension; i++) {
            state.setHandleColor(m_scene, j * ixDimension + i);
            glLoadName(j * ixDimension + i);
            state.drawHandle(Vec3f(i * xSpacing()->getValue(), 
                                   HEIGHT(i, j), j * zSpacing()->getValue()));
        }
    }
    state.endDrawHandles();
    glPopName();
    glEnable(GL_LIGHTING);
}

void
NodeElevationGrid::setField(int index, FieldValue *value, int cf)
{
    m_meshDirty = true;
    Node::setField(index, value, cf);
}

Vec3f
NodeElevationGrid::getHandle(int handle, int *constraint, int *field)
{
    int ixDimension = xDimension()->getValue();
    const float *fheight = height()->getValues();

    float x = (handle % ixDimension) * xSpacing()->getValue();
    float y = fheight[handle];
    float z = handle / ixDimension * zSpacing()->getValue();

    *field = height_Field();
    *constraint = CONSTRAIN_Y;

    Vec3f vec3f(x, y, z);
    TheApp->PrintMessageWindowsVertex(IDS_VERTEX_SELECTED, "height", handle,
                                      vec3f);
    return vec3f;
}

void
NodeElevationGrid::setHandle(int handle, const Vec3f &v)
{
    MFFloat *oldValue = (MFFloat *) getField(height_Field());
    MFFloat *newValue = (MFFloat *) oldValue->copy();

    newValue->setValue(handle, v.y);

    m_scene->setField(this, height_Field(), newValue);
}

void
NodeElevationGrid::setNormalFromMesh(Node *nnormal)
{
    if (nnormal->getType() != VRML_NORMAL)
        return;

    if (meshDirty() || (m_mesh == NULL)) {
        createMesh();
        m_meshDirty = false;
    }

    MFVec3f *v = m_mesh->getNormals();
    if (v != NULL) {
        ((NodeNormal *)nnormal)->vector(v);
        normalPerVertex(new SFBool(true));
    }
}

void
NodeElevationGrid::setTexCoordFromMesh(Node *ntexCoord)
{
    if (ntexCoord->getType() != VRML_TEXTURE_COORDINATE)
        return;
    MFVec2f *v = accountTexCoord();
    if (v != NULL) {
        ((NodeTextureCoordinate *)ntexCoord)->point(v);
    }
}

int NodeElevationGrid::getProfile(void) const
{ 
    if (!isDefault(ccw_Field()))
        return PROFILE_IMMERSIVE;
    return PROFILE_INTERACTIVE;
}

bool    
NodeElevationGrid::validHandle(int handle)
{
    if ((handle < 0) || (handle > getMaxHandle()))
        return false;
    return true;
}

bool    
NodeElevationGrid::checkHandle(int handle)
{
    for (int i = 0; i < m_scene->getSelectedHandlesSize(); i++) {
        int selectedHandle = m_scene->getSelectedHandle(i);
        if (!validHandle(selectedHandle))
            continue;
        int temph = handle % xDimension()->getValue();
        int xh = xDimension()->getValue() - temph - 1;
        int zh = (handle - temph);
        int tempi = selectedHandle % xDimension()->getValue();
        int xi = tempi;
        int zi = (selectedHandle - tempi); 
        if ((xh == xi) && (zh == zi) &&
            ((height()->getValue(xh + zh * xDimension()->getValue())) ==
             (height()->getValue(xi + zh * xDimension()->getValue()))))
            return true;
        if (selectedHandle == handle)
            return true;           
    }
    return false;
}

int     
NodeElevationGrid::getMaxHandle(void)
{
    return xDimension()->getValue() * zDimension()->getValue() - 1;
}
