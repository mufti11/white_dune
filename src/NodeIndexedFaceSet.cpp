/*
 * NodeIndexedFaceSet.cpp
 *
 * Copyright (C) 1999 Stephen F. White, 2018 J. "MUFTI" Scheurich
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

// if this file is compiled with CGAL: CGAL is licenced under different
// licences including GNU General Public License 3

// if this file is compiled with VCG: VCG is licenced under
// GNU General Public License 3

#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

#include <algorithm>

#include "stdafx.h"

#include "NodeIndexedFaceSet.h"
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
#include "NodeShape.h"
#include "NodeFogCoordinate.h"
#include "Util.h"
#include "MoveCommand.h"
#include "Field.h"
#include "resource.h"
#include "Path.h"
#include "MyString.h"

class FaceVec {
    int m_face;
    Vec3f m_vec;
public:
    FaceVec(int face, Vec3f vec) {
        m_face = face;
        m_vec = vec;
    }
    int getFace(void) { return m_face; }
    Vec3f getVec(void) { return m_vec; }
};

MyArray<FaceVec *> m_faces;

ProtoIndexedFaceSet::ProtoIndexedFaceSet(Scene *scene)
  : GeometryProto(scene, "IndexedFaceSet")
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
    if (TheApp->getCoverMode()) {
        // non standard Covise/COVER extensions
        texCoord2.set(
          addExposedField(SFNODE, "texCoord2", new SFNode(NULL), 
                          TEXTURE_COORDINATE_NODE));
        setFieldFlags(texCoord2, FF_COVER_ONLY);
        texCoord3.set(
          addExposedField(SFNODE, "texCoord3", new SFNode(NULL), 
                          TEXTURE_COORDINATE_NODE));
        setFieldFlags(texCoord3, FF_COVER_ONLY);
        texCoord4.set(
          addExposedField(SFNODE, "texCoord4", new SFNode(NULL), 
                          TEXTURE_COORDINATE_NODE));
        setFieldFlags(texCoord4, FF_COVER_ONLY);
    }
    ccw.set(
          addField(SFBOOL, "ccw", new SFBool(true)));
    setFieldFlags(ccw, FF_4KIDS);
    colorIndex.set(
          addField(MFINT32, "colorIndex", new MFInt32(), new SFInt32(-1)));
    colorPerVertex.set(
          addField(SFBOOL, "colorPerVertex", new SFBool(true)));
    convex.set(
          addField(SFBOOL, "convex", new SFBool(true)));
    coordIndex.set(
          addField(MFINT32, "coordIndex", new MFInt32(), new SFInt32(-1)));
    creaseAngle.set(
          addField(SFFLOAT, "creaseAngle", new SFFloat(0.0), 
                   new SFFloat(0.0f)));
    setFieldFlags(creaseAngle, FF_4KIDS);
    normalIndex.set(
          addField(MFINT32, "normalIndex", new MFInt32(), new SFInt32(-1)));
    normalPerVertex.set(
          addField(SFBOOL, "normalPerVertex", new SFBool(true)));
    solid.set(
          addField(SFBOOL, "solid", new SFBool(true)));
    setFieldFlags(solid, FF_4KIDS);
    texCoordIndex.set(
          addField(MFINT32, "texCoordIndex", new MFInt32(), new SFInt32(-1)));
    if (TheApp->getCoverMode()) {
        // non standard Covise/COVER extensions
        texCoordIndex2.set(
          addField(MFINT32, "texCoordIndex2", new MFInt32(), new SFInt32(-1)));
        setFieldFlags(texCoordIndex2, FF_COVER_ONLY);
        texCoordIndex3.set(
          addField(MFINT32, "texCoordIndex3", new MFInt32(), new SFInt32(-1)));
        setFieldFlags(texCoordIndex3, FF_COVER_ONLY);
        texCoordIndex4.set(
          addField(MFINT32, "texCoordIndex4", new MFInt32(), new SFInt32(-1)));
        setFieldFlags(texCoordIndex4, FF_COVER_ONLY);
    }
    ComposedGeometryElements()
    x3domGeometryCommonFields()
    normalUpdateMode.set(
      addExposedField(SFSTRING, "normalUpdateMode", new SFString("fast")));
    setFieldFlags(normalUpdateMode, FF_X3DOM_ONLY);
    addEventIn(MFINT32, "set_colorIndex", 0, colorIndex);
    addEventIn(MFINT32, "set_coordIndex", 0, coordIndex);
    addEventIn(MFINT32, "set_normalIndex", 0, normalIndex);
    addEventIn(MFINT32, "set_texCoordIndex", 0, texCoordIndex);
}

Node *
ProtoIndexedFaceSet::create(Scene *scene)
{ 
      Node *node = new NodeIndexedFaceSet(scene, this);
      return node;
}

NodeIndexedFaceSet::NodeIndexedFaceSet(Scene *scene, Proto *def)
  : MeshBasedNode(scene, def)
{
     m_alreadyInChangeColorPerVertex = false;
}

NodeIndexedFaceSet::~NodeIndexedFaceSet()
{
}

void 
NodeIndexedFaceSet::draw()
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
NodeIndexedFaceSet::setField(int index, FieldValue *value, int cf)
{
    m_meshDirty = true;
    if (index == colorPerVertexField()) {
        bool bColorPerVertex = ((SFBool *)value)->getValue();
        if (bColorPerVertex && !colorPerVertex()->getValue())
            changeToColorPerVertex();
        if (!bColorPerVertex && colorPerVertex()->getValue())
            changeToColorPerFace();
    }
    Node::setField(index, value, cf);
}

void
NodeIndexedFaceSet::addToConvertedNodes(int flags)
{
    if (m_already_converted)
        return;
    m_already_converted = true;
    if (flags & TRIANGULATE) {
        if (!canSimpleTriangulate() || !hasBranchInputs()) {
            MeshBasedNode::addToConvertedNodes(flags);
            setAlreadyConverted();
            m_meshDirty = true;
            return;
        }
    } 
    if (getHumanoid() != NULL)
        return;
    NodeIndexedFaceSet *node = (NodeIndexedFaceSet *)this->copy();
    node->setVariableName(strdup(this->getVariableName()));
    m_scene->copyRoutes(node, this);
    if (coord()->getValue() != NULL) {
        Node *target = node->coord()->getValue();
        m_scene->copyRoutes(target, this->coord()->getValue());
        target->addParent(node, node->coord_Field());
        
    }
    if (normal()->getValue() != NULL) {
        Node *target = node->normal()->getValue();
        target->setVariableName(strdup(target->getVariableName()));
        m_scene->copyRoutes(target, this->normal()->getValue());
        target->addParent(node,  node->normal_Field());
    }
    if (color()->getValue() != NULL) {
        Node *target = node->color()->getValue();
        m_scene->copyRoutes(target, this->color()->getValue());
        target->addParent(node,  node->color_Field());
    }
    if (texCoord()->getValue() != NULL) {
        Node *target = node->texCoord()->getValue();
        m_scene->copyRoutes(target, this->texCoord()->getValue());
        target->addParent(node,  node->texCoord_Field());
    }
    if (fogCoord()->getValue() != NULL) {
        Node *target = node->fogCoord()->getValue();
        m_scene->copyRoutes(target, this->fogCoord()->getValue());
        target->addParent(node,  node->fogCoord_Field());
    }
    
    if (m_convertedNodes.size() != 0)
        return;
    m_meshDirty = true;
    m_mesh = NULL;

    node->createMesh(false);
    if (normal()->getValue() == NULL) {
        MFVec3f *smoothNormals = node->getSmoothNormals();
        if (smoothNormals && (smoothNormals->getValues() != NULL)) {
            NodeNormal *normals = (NodeNormal *)m_scene->createNode("Normal");
            normals->vector(new MFVec3f((MFVec3f *)smoothNormals->copy()));
            m_scene->setField(node, normal_Field(), new SFNode(normals));
            MFInt32 *smoothNormalIndex = node->getSmoothNormalIndex();
            m_scene->setField(node, normalIndex_Field(), 
                             new MFInt32((MFInt32 *)smoothNormalIndex->copy()));
        }
    }
    MyMesh *mesh = node->simpleQuadTriangulateMesh();
    MFInt32 *mfcoordIndex = mesh->getCoordIndex();
    if (mfcoordIndex && (mfcoordIndex->getValues() != NULL)) {
        mfcoordIndex = (MFInt32 *)mfcoordIndex->copy();
        m_scene->setField(node, coordIndex_Field(), new MFInt32(mfcoordIndex));
    }
    MFInt32 *mfnormalIndex = mesh->getNormalIndex();
    if (mfnormalIndex && (mfnormalIndex->getValues() != NULL)) {
        mfnormalIndex = (MFInt32 *)mfnormalIndex->copy();
        m_scene->setField(node, normalIndex_Field(), new MFInt32(mfnormalIndex));
    }
    MFInt32 *mfcolorIndex = mesh->getColorIndex();
    if (mfcolorIndex && (mfcolorIndex->getValues() != NULL) &&
        (mfcolorIndex != mesh->getCoordIndex())) {
        mfcolorIndex = (MFInt32 *)mfcolorIndex->copy();
        m_scene->setField(node, colorIndex_Field(), new MFInt32(mfcolorIndex));
    }
    MFInt32 *mftexCoordIndex = mesh->getTexCoordIndex();
    if (mftexCoordIndex && (mftexCoordIndex->getValues() != NULL) &&
        (mftexCoordIndex != mesh->getCoordIndex())) {
        mftexCoordIndex = (MFInt32 *)mftexCoordIndex->copy();
        m_scene->setField(node, texCoordIndex_Field(), 
                         new MFInt32(mftexCoordIndex));
    }
    m_convertedNodes.append(node);
    node->setAlreadyConverted();
    node->addParent(getParent(), getParentField());
    return;
}


MFVec3f *
NodeIndexedFaceSet::getCoordinates() 
{
    Node *ncoord = coord()->getValue();
    if (ncoord == NULL)
        return NULL;
    else
        return ((NodeCoordinate *)ncoord)->point();
}

MFInt32 *
NodeIndexedFaceSet::getCoordIndex()
{
    return coordIndex();
}

MFInt32 *
NodeIndexedFaceSet::getColorIndex()
{
    return colorIndex();
}

MFInt32 *
NodeIndexedFaceSet::getNormalIndex()
{
    return normalIndex();
}

MFVec2f *
NodeIndexedFaceSet::getTextureCoordinates()
{
    Node *ntexCoord = texCoord()->getValue();
    if (ntexCoord == NULL)
        return NULL;
    else
        return ((NodeTextureCoordinate *)ntexCoord)->point();
}

MFInt32 *
NodeIndexedFaceSet::getTexCoordIndex()
{
    return texCoordIndex();
}

void
NodeIndexedFaceSet::createMesh(bool cleanDoubleVertices, bool triangulate)
{
    Node *coord = ((SFNode *) getField(coord_Field()))->getValue();
    bool bcolorPerVertex = colorPerVertex() ? 
                           colorPerVertex()->getValue() : true;
    bool bnormalPerVertex = normalPerVertex() ?
                            normalPerVertex()->getValue() : true;
    MFInt32 *colorIndex = getColorIndex();
    MFInt32 *coordIndex = getCoordIndex();
    MFInt32 *normalIndex = getNormalIndex();
    MFInt32 *texCoordIndex = getTexCoordIndex();
   
    if (!coord)
        return;

    MFVec3f *normals = NULL;
    MFFloat *colors = NULL;

    if (normal() && normal()->getValue())
        if (normal()->getValue()->getType() == VRML_NORMAL)
            normals = ((NodeNormal *)(normal()->getValue()))->vector();
    
    int meshFlags = 0;
    if (color() && color()->getValue()) {
        if (color()->getValue()->getType() == VRML_COLOR) 
            colors = ((NodeColor *)(color()->getValue()))->color();
        else if (color()->getValue()->getType() == X3D_COLOR_RGBA) {
            colors = ((NodeColorRGBA *)(color()->getValue()))->color();
            meshFlags |= MESH_COLOR_RGBA;
        }
    }    

    MFFloat *fogCoords = NULL;
    if (fogCoord() && fogCoord()->getValue())
        if (fogCoord()->getValue()->getType() == X3D_FOG_COORDINATE)
            fogCoords = ((NodeFogCoordinate *) 
                         (fogCoord()->getValue()))->depth();

    MyArray<MFVec2f *> texCoords;
    if (texCoord())
        Util::getTexCoords(texCoords, texCoord()->getValue());    

    if (bcolorPerVertex)
        if (colorIndex && colorIndex->getSize() != coordIndex->getSize())
            colorIndex = NULL;
    if (texCoordIndex && texCoordIndex->getSize() != coordIndex->getSize())
        texCoordIndex = NULL;
    if (bnormalPerVertex)
        if (normalIndex && normalIndex->getSize() != coordIndex->getSize())
            normalIndex = NULL;
    float transparency = 0;
    if (hasParent())
        transparency = getParent()->getTransparency();
    if (ccw() && ccw()->getValue())
        meshFlags |= MESH_CCW;
    if (solid() && solid()->getValue())
        meshFlags |= MESH_SOLID;
    if (convex() && convex()->getValue())
        meshFlags |= MESH_CONVEX;
    if (bcolorPerVertex)
        meshFlags |= MESH_COLOR_PER_VERTEX;
    if (bnormalPerVertex)
        meshFlags |= MESH_NORMAL_PER_VERTEX;
    if (coord->getType() == VRML_COORDINATE) {
        MFVec3f *coords = ((NodeCoordinate *)coord)->point();
        m_mesh = new MyMesh(this, coords, coordIndex, normals, normalIndex, 
                            colors, colorIndex, texCoords, texCoordIndex,
                            creaseAngle()->getFixedAngle(
                                m_scene->getUnitAngle()), 
                            meshFlags, transparency, fogCoords);
        m_isDoubleMesh = false;
    } else if (coord->getType() == VRML_GEO_COORDINATE) {
        MFVec3d *coords = ((NodeGeoCoordinate *)coord)->pointX3D();
        m_meshDouble = new MyMeshDouble(this, coords, coordIndex, normals, 
                                        normalIndex, colors, colorIndex, 
                                        texCoords, texCoordIndex, 
                                        creaseAngle()->getFixedAngle(
                                            m_scene->getUnitAngle()), 
                                        meshFlags, transparency, fogCoords);
        m_isDoubleMesh = true;
    }
}

Node * 
NodeIndexedFaceSet::toIndexedLineSet(void)
{
    if (m_mesh == NULL)
        return NULL;
    NodeCoordinate *ncoord = (NodeCoordinate *)m_scene->createNode("Coordinate");
    ncoord->point(new MFVec3f((MFVec3f *)m_mesh->getVertices()->copy()));
    NodeIndexedLineSet *node = (NodeIndexedLineSet *)
                               m_scene->createNode("IndexedLineSet");
    node->coord(new SFNode(ncoord));
    node->coordIndex(new MFInt32((MFInt32 *)m_mesh->getCoordIndex()->copy()));
    return node;
}

void
NodeIndexedFaceSet::flip(int index)
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
NodeIndexedFaceSet::swap(int fromTo)
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

void
NodeIndexedFaceSet::setNormalFromMesh(Node *nnormal)
{
    if (nnormal->getType() != VRML_NORMAL)
        return;

    if (meshDirty() || (m_mesh == NULL)) {
        createMesh();
        m_meshDirty = false;
    }

    if (m_mesh == NULL)
        return;

    MFVec3f *v = m_mesh->getNormals();
    if (v != NULL) {
        ((NodeNormal *)nnormal)->vector(v);
        MFInt32 *ni = m_mesh->getNormalIndex();
        if (ni != NULL)
            normalIndex(ni);
        normalPerVertex(new SFBool(true));
    }
}

void
NodeIndexedFaceSet::setTexCoordFromMesh(Node *ntexCoord)
{
    if (ntexCoord->getType() != VRML_TEXTURE_COORDINATE)
        return;
    if (m_mesh == NULL)
        return;
    MFVec2f *v = m_mesh->generateTextureCoordinates();
    if (v != NULL) {
        ((NodeTextureCoordinate *)ntexCoord)->point(v);
        MFInt32 *ni = (MFInt32 *)getTexCoordIndex()->copy();
        if (ni != NULL)
            texCoordIndex(ni);
    }
}

void            
NodeIndexedFaceSet::optimize(void)
{
    MFVec3f *mfVertices = NULL;
    MFInt32 *mfCoordIndex = NULL;
    optimizeMesh(&mfVertices, &mfCoordIndex);
    if (mfVertices && mfCoordIndex) {
        NodeCoordinate *ncoord = (NodeCoordinate *)coord()->getValue();
        m_scene->backupFieldsStart();
        m_scene->backupFieldsAppend(ncoord, ncoord->point_Field());
        m_scene->backupFieldsAppend(this, coordIndex_Field());
        m_scene->backupFieldsDone();

        ncoord->point(new MFVec3f(*mfVertices));
        coordIndex(new MFInt32(*mfCoordIndex));
    }
}

void
NodeIndexedFaceSet::optimizeCoordIndex(void)
{
    MyMesh *mesh = getMesh();
    mesh->optimizeCoordIndex();
    m_scene->backupField(this, coordIndex_Field());
    MFInt32 *ci = new MFInt32((MFInt32 *)mesh->getCoordIndex()->copy());    
    m_scene->setField(this, coordIndex_Field(), ci);    
    m_meshDirty = true;
}

NodeIndexedFaceSet *
NodeIndexedFaceSet::splitSelectedFaces(void)
{
    bool bccw = ccw()->getValue();
    bool bsolid = solid()->getValue();
    bool bconvex = convex()->getValue();
    float fcreaseAngle = creaseAngle()->getValue();
    if (m_scene->getSelectionMode() != SELECTION_MODE_FACES)
        return NULL;
    MFVec3f *newVertices = new MFVec3f();
    MFInt32 *newCoordIndex = new MFInt32();
    MFVec3f *splitVertices = new MFVec3f();
    MFInt32 *splitCoordIndex = new MFInt32();
    NodeCoordinate *ncoord = (NodeCoordinate *)coord()->getValue();
    if (ncoord == NULL)
        return NULL;
    int numNewCoordIndex = 0;
    int numSplitCoordIndex = 0;
    for (int i = 0; i < getMesh()->getNumFaces(); i++) {
        FaceData *face = getMesh()->getFace(i);
        int offset = face->getOffset();
        int numVertices = face->getNumVertices();
        bool addedNew = false;
        bool addedSplit = false;
        for (int j = 0; j < numVertices; j++) {
            int ci = coordIndex()->getValue(offset + j);
            if (m_scene->isInSelectedHandles(i)) {
                newVertices->appendSFValue(ncoord->point()->getValue(ci));
                newCoordIndex->appendSFValue(numNewCoordIndex++);
                addedNew = true;
            } else {
                splitVertices->appendSFValue(ncoord->point()->getValue(ci));
                splitCoordIndex->appendSFValue(numSplitCoordIndex++);
                addedSplit = true;
            }
        }
        if (addedNew)
            newCoordIndex->appendSFValue(-1);
        if (addedSplit)
            splitCoordIndex->appendSFValue(-1);
    }
    if (newVertices && newCoordIndex && hasParent()) {
        Node *parent = getParent();
        Node *grandParent = parent->getParent();
        if (grandParent == NULL)
            return NULL;
        int grandParentField = parent->getParentField();
        if (grandParent->getProto()->getField(grandParentField)->getType() != 
            MFNODE)
            return NULL;
        NodeIndexedFaceSet *newFaceSet = (NodeIndexedFaceSet *) 
                                         m_scene->createNode("IndexedFaceSet");
        NodeCoordinate *coord = (NodeCoordinate *)
                                m_scene->createNode("Coordinate");
        coord->point(new MFVec3f(newVertices));

        newFaceSet->coordIndex(new MFInt32(newCoordIndex));
        newFaceSet->ccw(new SFBool(bccw));
        newFaceSet->solid(new SFBool(bsolid));
        newFaceSet->convex(new SFBool(bconvex));
        newFaceSet->creaseAngle(new SFFloat(fcreaseAngle));

        NodeIndexedFaceSet *splitFaceSet = (NodeIndexedFaceSet *) 
                                           m_scene->createNode("IndexedFaceSet");
        NodeCoordinate *ncoord = (NodeCoordinate *)
                                 m_scene->createNode("Coordinate");
        splitFaceSet->coord(new SFNode(ncoord));
        ncoord->point(new MFVec3f(splitVertices));
        splitFaceSet->coordIndex(new MFInt32(splitCoordIndex));

        MFVec3f *mfVertices = NULL;
        MFInt32 *mfCoordIndex = NULL;
        splitFaceSet->optimizeMesh(&mfVertices, &mfCoordIndex);
        if (mfVertices && mfCoordIndex) {
            ncoord->point(new MFVec3f(*mfVertices));
            splitFaceSet->coordIndex(new MFInt32(*mfCoordIndex));
            splitFaceSet->ccw(new SFBool(bccw));
            splitFaceSet->solid(new SFBool(bsolid));
            splitFaceSet->convex(new SFBool(bconvex));
            splitFaceSet->creaseAngle(new SFFloat(fcreaseAngle));
            NodeShape *newShape = (NodeShape *) m_scene->createNode("Shape");
            NodeMaterial *newMaterial = (NodeMaterial *) 
                                        m_scene->createNode("Material");
            NodeAppearance *newAppearance = (NodeAppearance *) 
                                            m_scene->createNode("Appearance");
            newFaceSet->coord(new SFNode(coord));

            m_scene->execute(new MoveCommand(newShape, NULL, -1, 
                                             grandParent, grandParentField));
            m_scene->execute(new MoveCommand(newAppearance, NULL, -1, 
                                             newShape, 
                                             newShape->appearance_Field())); 
            m_scene->execute(new MoveCommand(newMaterial, NULL, -1, 
                                             newAppearance, 
                                             newAppearance->material_Field())); 
            m_scene->execute(new MoveCommand(newFaceSet, NULL, -1, 
                                             newShape, 
                                             newShape->geometry_Field())); 
            return splitFaceSet;
        }
    }
    return NULL;
}

bool
NodeIndexedFaceSet::checkBorderMidPoint(int icoordIndex, MyArray<int> symFaces)
{
    int verticesCount = 0;
    NodeCoordinate *ncoord = (NodeCoordinate *)coord()->getValue();
    if (ncoord == NULL)
        return true;
    if (icoordIndex < 0)
        return true;
    Vec3f vec1(ncoord->point()->getVec(icoordIndex));
    for (int i = 0; i < getMesh()->getNumFaces(); i++) {
        if (!(m_scene->isInSelectedHandles(i) || symFaces.contains(i)))
            continue;
        FaceData *face = getMesh()->getFace(i);
        int offset = face->getOffset();
        int numVertices = face->getNumVertices();
        for (int j = offset; j < offset + numVertices; j++) {
            int ci = coordIndex()->getValue(j);
            Vec3f vec2(ncoord->point()->getVec(ci));
            if ((vec1 - vec2).length() == 0.0f)
                verticesCount++;
        }
    }
    return verticesCount > 3;
}

bool
NodeIndexedFaceSet::checkBorderFace(MyArray<int> innerBorder, 
                                    MyArray<int> outerBorder, MFInt32 *coordIndex,
                                    int borderIndex1, int borderIndex2,
                                    MyArray<int> symFaces, bool sym)
{
    NodeCoordinate *ncoord = (NodeCoordinate *)coord()->getValue();
    if (ncoord == NULL)
        return false;
    if (checkBorderMidPoint(outerBorder[borderIndex1], symFaces) ||
        checkBorderMidPoint(outerBorder[borderIndex2], symFaces))
        return false;
    if (outerBorder[borderIndex1] < 0)
        return true;
    if (outerBorder[borderIndex2] < 0)
        return true;
    Vec3f vec11(ncoord->point()->getVec(outerBorder[borderIndex1]));
    Vec3f vec12(ncoord->point()->getVec(outerBorder[borderIndex2]));
    for (long j = 0; j < (long)innerBorder.size() - 1; j++) {
        int n = j + 1;
        if (innerBorder[j] == -1)
            continue;
        if (innerBorder[n] == -1)
            continue;
        if ((j != borderIndex2) && (n != borderIndex1)) {
            Vec3f vec21(ncoord->point()->getVec(outerBorder[j]));
            Vec3f vec22(ncoord->point()->getVec(outerBorder[n]));
            if (((vec22 - vec11).length() == 0) &&
                ((vec21 - vec12).length() == 0))
                return false;     
        }
    }
    return true;
}

static bool compareVectors(Vec3f vec, Vec3f vec2)
{
     bool ret = true;
     float epsilon = TheApp->GetHandleEpsilon();
     if ((fabs(vec.x + vec2.x) > epsilon) ||
         (fabs(vec.y - vec2.y) > epsilon) ||
         (fabs(vec.z - vec2.z) > epsilon)) {
         return false;
     }
     return ret;
}


static bool comparePermutation(MyArray<int> intArrayCi,
                               MyArray<int> intArrayPerm, MFVec3f *mfvec)
{
    bool ret = true;
    int *arr = new int[intArrayPerm.size()];
    for (int i = 0; i < intArrayPerm.size(); i++)
        arr[i] = intArrayPerm[i]; 
    std::sort(arr, arr + intArrayPerm.size());   
    do {
        bool vectorsEqual = true;
        for (int i = 0; i < intArrayPerm.size(); i++)
        for (int i = 0; i < intArrayPerm.size(); i++) {
             if (!compareVectors(mfvec->getValue(intArrayCi[i]), 
                                 mfvec->getValue(arr[i]))) {
                 vectorsEqual = false;
                 break;
             }
        }
        if (vectorsEqual) {
            delete [] arr;
            return true;
        }
    } while (std::next_permutation(arr, arr + intArrayPerm.size()));
    bool vectorsEqual = true;
        for (int i = 0; i < intArrayPerm.size(); i++)
    for (int i = 0; i < intArrayPerm.size(); i++) {
         if (!compareVectors(mfvec->getValue(intArrayCi[i]), 
                             mfvec->getValue(arr[i]))) {
             vectorsEqual = false;
             break;
        }
    }
    if (vectorsEqual) {
        delete [] arr;
        return true;
    }
    delete [] arr;
    return false;
}

int
NodeIndexedFaceSet::symetricFace(int iface)
{
    NodeCoordinate *ncoord = (NodeCoordinate *)coord()->getValue();
    if (ncoord == NULL)
        return -1;
    FaceData *face = getMesh()->getFace(iface);
    int offset = face->getOffset();
    int numVertices = face->getNumVertices();
    int numFaces = getMesh()->getNumFaces();
    for (int i = 0; i < numFaces; i++) {
        if (i == iface)
            continue;
        FaceData *face2 = getMesh()->getFace(i);
        int offset2 = face2->getOffset();
        int numVertices2 = face2->getNumVertices();
        if (numVertices != numVertices2)
            continue;
        MyArray<int> intArrayCi;
        for (int j = 0; j < numVertices; j++) {
             int ci = coordIndex()->getValue(offset + j);
             intArrayCi.append(ci);
        }
        MyArray<int> intArrayPerm;
        for (int n = 0; n < numVertices; n++) {
            int ci2 = coordIndex()->getValue(offset2 + n);
            intArrayPerm.append(ci2);
        }                 
        if (comparePermutation(intArrayCi, intArrayPerm, ncoord->point()))
            return i;
     }    
     return -1;
}

void
NodeIndexedFaceSet::deleteFaces(MFInt32 *coordIndex, 
                                MyArray<int> *facesToDelete)
{
    MyArray<int> sortedFaces;
    int facesToDeleteSize = (*facesToDelete).size();
    for (int i = 0; i < facesToDeleteSize; i++) {
        long k = 0;
        while (k < sortedFaces.size()) {
            if ((*facesToDelete)[i] > sortedFaces[k]) {
                sortedFaces.insert((*facesToDelete)[i], k);
                break;
            }
            k++;
        }
        if (k >= sortedFaces.size()) {
            sortedFaces.append((*facesToDelete)[i]);
        }
    }

    for (long i = 0; i < sortedFaces.size(); i++) {
        FaceData *face = getMesh()->getFace(sortedFaces[i]);
        int offset = face->getOffset();
        int numVertices = face->getNumVertices();
        for (int j = offset; j < offset + numVertices; j++)
            coordIndex->removeSFValue(offset);
        coordIndex->removeSFValue(offset);
    }
}

bool
NodeIndexedFaceSet::isSymetricFace(int iface)
{
    FaceData *face = getMesh()->getFace(iface);
    int offset = face->getOffset();
    int numVertices = face->getNumVertices();
    bool sym = false;
    float fepsilon = TheApp->GetHandleEpsilon();
    NodeCoordinate *ncoord = (NodeCoordinate *)coord()->getValue();
    if (ncoord == NULL)
        return false;
    for (int i = offset; i < offset + numVertices; i++) {
        int ci = coordIndex()->getValue(i);
        Vec3f vec = ncoord->point()->getValue(ci);
        sym = false;
        for (int j = offset; j < offset + numVertices; j++) {
            if (i == j)
                continue;
            int ci2 = coordIndex()->getValue(j);
            Vec3f vec2 = ncoord->point()->getValue(ci2);
            if (((vec2.x + vec.x) < fepsilon) &&
                ((vec2.y - vec.y) < fepsilon) &&
                ((vec2.z - vec.z) < fepsilon))
                sym = true;
        }
        if (!sym)
            return false;
    }
    return sym;   
}

void
NodeIndexedFaceSet::extrudeFaces(float dist)
{
    if (m_scene->getSelectionMode() != SELECTION_MODE_FACES)
        return;
    NodeCoordinate *ncoord = (NodeCoordinate *)coord()->getValue();
    if (ncoord == NULL)
        return;
    MFVec3f *newVertices = new MFVec3f((MFVec3f *)ncoord->point()->copy());
    MFInt32 *newCoordIndex = new MFInt32((MFInt32 *)coordIndex()->copy());
    int numNewCoordIndex = newVertices->getSFSize();
    int borderCount = 0;
    MyArray<int> innerBorder;
    MyArray<int> outerBorder;
    MyArray<bool> symBorder;
    MyArray<bool> validBorder;
    int numFaces = 0;
    MyArray<int> newFaces;
    MyArray<int> symFaces;
    MyArray<int> bothSidesSymFaces;
    for (int i = 0; i < m_scene->getSelectedHandlesSize(); i++) {
        int iface = m_scene->getSelectedHandle(i);
        if (isSymetricFace(iface))
            symFaces.append(iface);
        int symFace = symetricFace(iface);
        if (symFace > 0)
            bothSidesSymFaces.append(symFace);
    }
    bool sym = m_scene->getXSymetricMode();
    bool symFace = sym;
    for (int i = 0; i < getMesh()->getNumFaces(); i++) {
        FaceData *face = getMesh()->getFace(i);
       int offset = face->getOffset();
        int numVertices = face->getNumVertices();
        if (m_scene->isInSelectedHandles(i))
            if (sym)
                if (!symFaces.contains(i)) {
                    symFace = false;
                }
    }
    for (int i = 0; i < getMesh()->getNumFaces(); i++) {
        FaceData *face = getMesh()->getFace(i);
        int offset = face->getOffset();
        int numVertices = face->getNumVertices();
        int ci2 = -1;
        int oldBorderCount = borderCount;        
        for (int j = offset; j < offset + numVertices; j++) {
            int ci = coordIndex()->getValue(j);
            if (m_scene->isInSelectedHandles(i)) {
                Vec3f vec = ncoord->point()->getValue(ci);
                vec.x = vec.x;
                if (!symFace)
                    vec.x += dist;
                vec.z = vec.z + dist;
                newVertices->appendSFValue(vec.x, vec.y, vec.z);
                if (j == offset)
                    ci2 = numNewCoordIndex;
                innerBorder[borderCount] = numNewCoordIndex++;
                outerBorder[borderCount] = ci;
                symBorder[borderCount] = false;
                validBorder[borderCount] = true;
                borderCount++;
            } 
        }
        if (m_scene->isInSelectedHandles(i)) {
            int ci3 = coordIndex()->getValue(offset + numVertices - 1);
            int ci4 = coordIndex()->getValue(offset);
            if ((ncoord->point()->getVec(ci3) -
                 ncoord->point()->getVec(ci4)).length() != 0) {
                innerBorder[borderCount] = ci2;
                outerBorder[borderCount] = ci4;
                symBorder[borderCount] = false;
                validBorder[borderCount] = false;
                borderCount++;
            }
        }
        if (borderCount > oldBorderCount) {
            newFaces[numFaces] = getMesh()->getNumFaces() + numFaces;
            numFaces++;
            innerBorder[borderCount] = -1;
            outerBorder[borderCount] = -1;
            symBorder[borderCount] = false;
            validBorder[borderCount] = true;
            borderCount++;
        }

        if (sym && (bothSidesSymFaces.contains(i))) {
            oldBorderCount = borderCount;        
            for (int j = offset; j < offset + numVertices; j++) {
                int ci = coordIndex()->getValue(j);
                Vec3f vec = ncoord->point()->getValue(ci);
                if (!symFace)
                    vec.x = vec.x - dist;
                vec.z = vec.z + dist;
                newVertices->appendSFValue(vec.x, vec.y, vec.z);
                if (j == offset)
                    ci2 = numNewCoordIndex;
                innerBorder[borderCount] = numNewCoordIndex++;
                outerBorder[borderCount] = ci;
                symBorder[borderCount] = true;
                validBorder[borderCount] = true;
                borderCount++;
            }
            int ci5 = coordIndex()->getValue(offset + numVertices - 1);
            int ci6 = coordIndex()->getValue(offset);
            if ((ncoord->point()->getVec(ci5) -
                 ncoord->point()->getVec(ci6)).length() != 0) {
                innerBorder[borderCount] = ci2;
                outerBorder[borderCount] = ci6;
                symBorder[borderCount] = true;
                validBorder[borderCount] = false;
                borderCount++;
            }
            if (borderCount > oldBorderCount) {        
                innerBorder[borderCount] = -1;
                outerBorder[borderCount] = -1;
                symBorder[borderCount] = true;
                validBorder[borderCount] = true;
                borderCount++;
            }
        }
    }

    for (int i = 0; i < borderCount; i++)
        if (validBorder[i] && !symBorder[i])
            newCoordIndex->appendSFValue(innerBorder[i]);
    for (int i = 0; i < borderCount; i++)
        if (validBorder[i] && symBorder[i])
            newCoordIndex->appendSFValue(innerBorder[i]);

    if (newCoordIndex->getValue(newCoordIndex->getSFSize() - 1) > -1)
        newCoordIndex->appendSFValue(-1);

    MyArray<int> facesToDelete;
    for (int i = 0; i < getMesh()->getNumFaces(); i++)
        if (m_scene->isInSelectedHandles(i)) {
            int facesToDeleteSize = facesToDelete.size();
            bool inserted = false;
            for (int j = 0; j < facesToDeleteSize; j++)
                if (i > facesToDelete[j]) {
                    facesToDelete.insert(i, j);
                    inserted = true;
                    break;
                }
            if (!inserted)
                facesToDelete.append(i);    
        }       

    for (long i = 0; i < symFaces.size(); i++) {
        int facesToDeleteSize = facesToDelete.size();
        bool inserted = false;
        for (int j = 0; j < facesToDeleteSize; j++)
            if (symFaces[i] > facesToDelete[j]) {
                facesToDelete.insert(symFaces[i], j);
                inserted = true;
                break;
            }
        if (!inserted)
            facesToDelete.append(symFaces[i]);    
    }
    int face1[4];
    int face1Count = 0;
    int face2[4];
    int face2Count = 0;
    for (int i = 0; i < borderCount - 1; i++) {
        int n = i + 1;
    
        if (!checkBorderFace(innerBorder, outerBorder, newCoordIndex, i, n,
                             symFaces, symBorder[i]))
            continue;
        if (symBorder[i]) {
            if ((innerBorder[i] < 0) || (!symBorder[n])) {
                continue;
            } else {
                face1[face1Count++] = innerBorder[i];
            }
            if (outerBorder[i] < 0) {
                continue;
            } else {
                face1[face1Count++] = outerBorder[i];
            }
        
            if (outerBorder[n] < 0) {
                continue;
            } else {
                face1[face1Count++] = outerBorder[n];
            }
            if (innerBorder[n] < 0) {
                continue;
            } else {
                face1[face1Count++] = innerBorder[n];
            }
        } else {
            if ((innerBorder[i] < 0) || (symBorder[n])) {
                continue;
            } else {
                face2[face2Count++] = innerBorder[i];
            }
            if (outerBorder[i] < 0) {
                continue;
            } else {
                face2[face2Count++] = outerBorder[i];
            }
        
            if (outerBorder[n] < 0) {
                continue;
            } else {
                face2[face2Count++] = outerBorder[n];
            }
            if (innerBorder[n] < 0) {
                continue;
            } else {
                face2[face2Count++] = innerBorder[n];
            }
        }
        if (face1Count >= 4) {
            for (int j = 0; j < 4; j++)
                newCoordIndex->appendSFValue(face1[j]);
            newCoordIndex->appendSFValue(-1);
            face1Count = 0;
        }
        if (face2Count >= 4) {
            for (int j = 0; j < 4; j++)
                newCoordIndex->appendSFValue(face2[j]);
            newCoordIndex->appendSFValue(-1);
            face2Count = 0;
        }    
    }
    m_scene->backupFieldsStart();
    m_scene->backupFieldsAppend(ncoord, ncoord->point_Field());
    ncoord->point(new MFVec3f(newVertices));
    m_scene->backupFieldsAppend(this, coordIndex_Field());
    coordIndex(new MFInt32(newCoordIndex));
    m_scene->backupFieldsDone();
    m_scene->execute(new MoveCommand(normal()->getValue(), this, normal_Field(),
                                     NULL, -1));
    this->createMesh(false);
    ncoord->drawHandles();
    m_scene->setSelection(ncoord);
    m_scene->removeSelectedHandles();
    for (int i = 0; i < numFaces; i++)
        m_scene->addSelectedHandle(newFaces[i]);
    m_scene->UpdateViews(NULL, UPDATE_SELECTION);
}

bool            
NodeIndexedFaceSet::buildQuad(void)
{
    NodeCoordinate *ncoord = (NodeCoordinate *)coord()->getValue();
    if (ncoord == NULL)
        return false;
    MFVec3f *point = ncoord->point();
    MFInt32 *ci = coordIndex();
    int iface1 = m_scene->getSelectedHandle(0);
    int iface2 = m_scene->getSelectedHandle(1);
    if (iface1 == iface2)
        return false;
    FaceData *face1 = getMesh()->getFace(iface1);
    int offset1 = face1->getOffset();
    int numVertices1 = face1->getNumVertices();
    FaceData *face2 = getMesh()->getFace(iface2);
    int offset2 = face2->getOffset();
    int numVertices2 = face2->getNumVertices();
    if ((numVertices1 != 3) || (numVertices2 != 3))
        return false;
    for (int i = offset1; i < offset1 + numVertices1; i++) {
        int first = i;
        Vec3f firstVertex = point->getVec(ci->getValue(first));
        int first2 = i + 1 < offset1 + numVertices1 ? i + 1 : offset1;
        Vec3f firstVertex2 = point->getVec(ci->getValue(first2));
        for (int j = offset2; j < offset2 + numVertices2; j++) {
             if (firstVertex == point->getVec(ci->getValue(j))) {
                 int second = -1;
                 bool validSecond = false;
                 second = j + 1 < offset2 + numVertices2 ? j + 1 : offset2;
                 Vec3f secondVertex = point->getVec(ci->getValue(second));
                 if (secondVertex == firstVertex2)
                     validSecond = true;
                 else {
                     second = j - 1 >= offset2 ? j - 1 : 
                                                 offset2 + numVertices2 - 1;
                     Vec3f secondVertex = point->getVec(ci->getValue(second));
                     if (secondVertex == firstVertex2)
                         validSecond = true;
                 }
                 if (validSecond) {
                     for (int n = offset1; n < offset1 + numVertices1; n++)
                         if (((point->getVec(ci->getValue(first))) !=
                              (point->getVec(ci->getValue(n)))) &&
                             ((point->getVec(ci->getValue(second))) !=
                              (point->getVec(ci->getValue(n))))) {
                             ci->appendSFValue(ci->getValue(n));
                             break;
                         }
                     ci->appendSFValue(ci->getValue(first));
                     for (int n = offset2; n < offset2 + numVertices2; n++)
                         if (((point->getVec(ci->getValue(first))) !=
                              (point->getVec(ci->getValue(n)))) &&
                             ((point->getVec(ci->getValue(second))) !=
                              (point->getVec(ci->getValue(n))))) { 
                             ci->appendSFValue(ci->getValue(n));
                             break;
                         }
                     ci->appendSFValue(ci->getValue(second));
                     ci->appendSFValue(-1);
                     MyArray<int> facesToDelete;
                     if (iface1 < iface2) {
                         facesToDelete.append(iface2);
                         facesToDelete.append(iface1);
                     } else {
                         facesToDelete.append(iface1);                       
                         facesToDelete.append(iface2);
                     }
                     deleteFaces(ci, &facesToDelete);
                     m_scene->removeSelectedHandles();
                     m_scene->setSelectedHandle(getMesh()->getNumFaces() - 2);
                     m_scene->backupFieldsStart();
                     m_scene->backupFieldsAppend(this, coordIndex_Field());
                     coordIndex(new MFInt32(ci));
                     m_scene->backupFieldsDone();
                     m_meshDirty = true;
                     return true;    
                 }
             }
        }            
    }
    return false;
}

void
NodeIndexedFaceSet::snapTogether(void)
{
    NodeCoordinate *ncoord = (NodeCoordinate *)coord()->getValue();
    if (ncoord == NULL)
        return;
    MFVec3f *point = ncoord->point();
    int numVertices = m_scene->getSelectedHandlesSize();
    Vec3f sum(0, 0, 0);
    for (int i = 0; i < numVertices; i++) {
        int vertex = m_scene->getSelectedHandle(i);
        sum = sum + point->getVec(vertex); 
    }
    sum = sum / numVertices;
    for (int i = 0; i < numVertices; i++) {
        int vertex = m_scene->getSelectedHandle(i);
        point->setVec(vertex, sum); 
    }        
    m_scene->backupFieldsStart();
    m_scene->backupFieldsAppend(ncoord, ncoord->point_Field());
    ncoord->point(new MFVec3f(point));
    m_scene->backupFieldsDone();
    m_meshDirty = true;
}

bool
NodeIndexedFaceSet::canSplitFace(int iface)
{
    NodeCoordinate *ncoord = (NodeCoordinate *)coord()->getValue();
    if (ncoord == NULL)
        return false;
    MFVec3f *point = ncoord->point();
    MFInt32 *ci = coordIndex();
    bool has4Edges = false;
    FaceData *face = getMesh()->getFace(iface);
    int offset = face->getOffset();
    if (face->getNumVertices() == 4)
        if (point->getValue(ci->getValue(offset)) !=
            point->getValue(ci->getValue(offset + face->getNumVertices() - 1)))
            has4Edges = true;
    if (face->getNumVertices() == 5)
        if (point->getValue(ci->getValue(offset)) ==
            point->getValue(ci->getValue(offset + face->getNumVertices() - 1)))
        has4Edges = true;
    return has4Edges;
}

class CoordIndexMapper {
public:
    int count;
    int coordIndex;
    int edgeCoordIndex;
    int offset;
    CoordIndexMapper() {
        count = -1;
        coordIndex = -1;
        edgeCoordIndex = -1;
        offset = -1;
    }
    CoordIndexMapper(int i, int c, int e, int o) {
        count = i;
        coordIndex = c;
        edgeCoordIndex = e;
        offset = o;
    }
};

bool
NodeIndexedFaceSet::checkMidpoint(Vec3f midPoint, int jLoop, int nLoop,
                                  int point1, int point2, 
                                  int uPieces, int vPieces)
{
    MFInt32 *ci = coordIndex();
    NodeCoordinate *ncoord = (NodeCoordinate *)coord()->getValue();
    if (ncoord == NULL)
        return false;
    MFVec3f *vertices = new MFVec3f(ncoord->point());
    Vec3f v1 = vertices->getValue(ci->getValue(point1));
    Vec3f v2 = vertices->getValue(ci->getValue(point2));
    float loop = -1;
    float maxLoop = -1;
    if ((jLoop == 0) || (jLoop == uPieces -1)) {
        loop = nLoop;
        if (uPieces == 1)
            maxLoop = vPieces;
        else
            maxLoop = uPieces;
    } else if ((nLoop == 0) || (nLoop == vPieces -1)) {
        loop = jLoop;
        if (vPieces == 1)
            maxLoop = uPieces;
        else
            maxLoop = vPieces;
    }
    if (loop == -1)
        return false;
    if (loop == 0) 
        loop = 1;
    if (loop == maxLoop)
        loop = 1;
    Vec3f v = v1 + (v2 - v1) * loop / maxLoop;
    if ((fabs(v.x - midPoint.x) < floatEpsilon()) &&   
        (fabs(v.y - midPoint.y) < floatEpsilon()) &&   
        (fabs(v.z - midPoint.z) < floatEpsilon()))
        return true;
    return false;
}

bool
NodeIndexedFaceSet::canSplitFaces(void)
{
    for (int i = 0; i < m_scene->getSelectedHandlesSize(); i++) {
        int iface = m_scene->getSelectedHandle(i);
        if (canSplitFace(iface))
            return true;
    }
    return false;
}

static int isInVertices(MFVec3f *newVertices, Vec3f v)
{
    MFVec3f *vertices = newVertices;
    for (int i = 0; i < vertices->getSFSize(); i++)
        if (v == vertices->getValue(i))
            return i;
    return -1;
}

int
NodeIndexedFaceSet::getEgdeCoordIndex(int iface, Vec3f midPoint,
                                      int uLoop, int vLoop,
                                      int uPieces, int vPieces)
{
    for (int i = 0; i < getMesh()->getNumFaces(); i++) {
        if (iface == i)
            continue;
        FaceData *face = getMesh()->getFace(i);
        int offset = face->getOffset();
        int numVertices = face->getNumVertices();
        for (int j = offset + 1; j < offset + numVertices; j++) {
            if (checkMidpoint(midPoint, uLoop, vLoop, j - 1, j, 
                              uPieces, vPieces))
                return j;
        }        
        if (checkMidpoint(midPoint, uLoop, vLoop, 
                          offset, offset + numVertices -1, uPieces, vPieces))
            return offset;
    }    
    return -1;
}

void            
NodeIndexedFaceSet::splitIntoPieces(int u, int v)
{
    if (!canSplitFaces())
        return;
    NodeCoordinate *ncoord = (NodeCoordinate *)coord()->getValue();
    if (ncoord == NULL)
        return;
    MFVec3f *newVertices = new MFVec3f((MFVec3f *)ncoord->point()->copy());
    MFInt32 *newCoordIndex = new MFInt32((MFInt32 *)coordIndex()->copy());
    MyArray<int> symFaces;
    if (m_scene->getXSymetricMode())
        for (int i = 0; i < m_scene->getSelectedHandlesSize(); i++) {
            int iface = symetricFace(m_scene->getSelectedHandle(i));
            if (iface > -1)
                if (!m_scene->isInSelectedHandles(iface))
                    symFaces.append(iface);
    }
    MyArray<CoordIndexMapper> coordIndexToAdded;
    int numAddedVertices = 0;
    for (int i = 0; i < getMesh()->getNumFaces(); i++) {
        FaceData *face = getMesh()->getFace(i);
        if ((!m_scene->isInSelectedHandles(i)) && (!symFaces.contains(i)))
            continue;
        int offset = face->getOffset();
        if (canSplitFace(i)) {
            int piecesU = u; 
            int piecesV = v;
            int off = newCoordIndex->getValue(offset);
            Vec3f vec = newVertices->getValue(off);
            int off1 = newCoordIndex->getValue(offset + 1);
            Vec3f s = newVertices->getValue(off1);
            int off2 = newCoordIndex->getValue(offset + 2);
            Vec3f t = newVertices->getValue(off2);
            int off3 = newCoordIndex->getValue(offset + 3);
            Vec3f r = newVertices->getValue(off3);
            for (int j = 0; j < piecesU; j++) {
                for (int n = 0; n < piecesV; n++) {
                    if ((n > 0) && (n == piecesV - 1))
                        continue;

                    Vec3f p1 = vec + (r - vec) * j / piecesU;
                    Vec3f p2 = s + (t - s) * j / piecesU;
                    Vec3f q1 = vec + (r - vec) * (j + 1) / piecesU; 
                    Vec3f q2 = s + (t - s) * (j + 1) / piecesU;

                    Vec3f v1 = p1 + (p2 - p1) * n / piecesV;
                    int n1 = isInVertices(newVertices, v1);
                    if (n1 < 0) {
                        CoordIndexMapper coordIndexMap(numAddedVertices++,
                            newVertices->getSFSize(), 
                            getEgdeCoordIndex(i, v1, j, n, piecesU, piecesV), 
                            off);
                        newVertices->appendSFValue(v1.x, v1.y, v1.z);
                        coordIndexToAdded.append(coordIndexMap);
                    }
                    Vec3f v2 = p1 + (p2 - p1) * (n + 1) / piecesV;
                    int n2 = isInVertices(newVertices, v2);
                    if (n2 < 0) {
                        CoordIndexMapper coordIndexMap(numAddedVertices++,
                            newVertices->getSFSize(),
                            getEgdeCoordIndex(i, v2, j, n + 1,
                                              piecesU, piecesV), 
                            off1);
                        newVertices->appendSFValue(v2.x, v2.y, v2.z);
                        coordIndexToAdded.append(coordIndexMap);
                    }
                    Vec3f v3 = q1 + (q2 - q1) * (n + 1) / piecesV;
                    int n3 = isInVertices(newVertices, v3);
                    if (n3 < 0) {
                        CoordIndexMapper coordIndexMap(numAddedVertices++,
                            newVertices->getSFSize(),
                            getEgdeCoordIndex(i, v3, j + 1, n + 1,
                                              piecesU, piecesV), 
                            off2);
                        newVertices->appendSFValue(v3.x, v3.y, v3.z);
                        coordIndexToAdded.append(coordIndexMap);
                    }
                    Vec3f v4 = q1 + (q2 - q1) * n / piecesV;
                    int n4 = isInVertices(newVertices, v4);
                    if (n4 < 0) {
                        CoordIndexMapper coordIndexMap(numAddedVertices++,
                            newVertices->getSFSize(),
                            getEgdeCoordIndex(i, v4, j + 1, n,
                                              piecesU, piecesV), 
                            off3);
                        newVertices->appendSFValue(v4.x, v4.y, v4.z);
                        coordIndexToAdded.append(coordIndexMap);
                    }
               }
            }
            for (int j = 0; j < piecesU; j++) {
                Vec3f p1 = vec + (r - vec) * j / piecesU;
                Vec3f p2 = s + (t - s) * j / piecesU;
                Vec3f q1 = vec + (r - vec) * (j + 1) / piecesU; 
                Vec3f q2 = s + (t - s) * (j + 1) / piecesU;
                for (int n = 0; n < piecesV; n++) {
                    Vec3f v1 = p1 + (p2 - p1) * n / piecesV;
                    int n1 = isInVertices(newVertices, v1);
                    if (n1 < 0) {
                        n1 = newVertices->getSFSize();
                        newVertices->appendSFValue(v1.x, v1.y, v1.z);
                    }
                    Vec3f v2 = p1 + (p2 - p1) * (n + 1) / piecesV;
                    int n2 = isInVertices(newVertices, v2);
                    if (n2 < 0) {
                        n2 = newVertices->getSFSize();
                        newVertices->appendSFValue(v2.x, v2.y, v2.z);
                    }
                    Vec3f v3 = q1 + (q2 - q1) * (n + 1) / piecesV;
                    int n3 = isInVertices(newVertices, v3);
                    if (n3 < 0) {
                        n3 = newVertices->getSFSize();
                        newVertices->appendSFValue(v3.x, v3.y, v3.z);
                    }
                    Vec3f v4 = q1 + (q2 - q1) * n / piecesV;
                    int n4 = isInVertices(newVertices, v4);
                    if (n4 < 0) {
                        n4 = newVertices->getSFSize();
                        newVertices->appendSFValue(v4.x, v4.y, v4.z);
                    }
                    newCoordIndex->appendSFValue(n1);
                    newCoordIndex->appendSFValue(n2);
                    newCoordIndex->appendSFValue(n3);
                    newCoordIndex->appendSFValue(n4);
                    newCoordIndex->appendSFValue(-1);
                }
            }
        }
    }
    MyArray<int> sortedM;
    MyArray<int> sortedL;
    MyArray<int> sortedO;
    for (long n = 0; n < coordIndexToAdded.size(); n++) {
        int m = coordIndexToAdded[n].edgeCoordIndex;
        int l = coordIndexToAdded[n].coordIndex;
        int o = coordIndexToAdded[n].offset;
        long k = 0;
        while (k < sortedM.size()) {
            if ((m > sortedM[k]) && (m > -1)) {
                sortedM.insert(m, k);
                sortedL.insert(l, k);
                sortedO.insert(o, k);
                break;
            }
            k++;
        }
        if ((k >= sortedM.size()) && (m > -1)) {
            sortedM.append(m);
            sortedL.append(l);
            sortedO.append(o);
        }
    }
    MyArray<int> sortedFaces;
    for (int i = 0; i < getMesh()->getNumFaces(); i++) {
        if (m_scene->isInSelectedHandles(i) || symFaces.contains(i)) {
            long k = 0;
            while (k < sortedFaces.size()) {
                if ((i > sortedFaces[k])) {
                    sortedFaces.insert(i, k);
                    break;
                }
                k++;
            }
            if (k >= sortedFaces.size()) {
                sortedFaces.append(i);
            }
        }
    }
    for (long i = 0; i < sortedFaces.size(); i++) {
        FaceData *face = getMesh()->getFace(sortedFaces[i]);
        int offset = face->getOffset();
        int numVertices = face->getNumVertices();
        for (int j = offset; j < offset + numVertices; j++)
            newCoordIndex->removeSFValue(offset);
        newCoordIndex->removeSFValue(offset);
        for (long j = 0; j < sortedM.size(); j++)
            if (sortedM[j] >= offset)
                sortedM[j] -= (numVertices + 1); 
    }    
    for (long j = 0; j < sortedM.size(); j++) {
        newCoordIndex->insertSFValue(sortedM[j], sortedL[j]);
    }
    m_scene->backupFieldsStart();
    m_scene->setField(this, normal_Field(), new SFNode(NULL));
    m_scene->backupFieldsAppend(ncoord, ncoord->point_Field());
    ncoord->point(new MFVec3f(newVertices));
    m_scene->backupFieldsAppend(this, coordIndex_Field());
    coordIndex(new MFInt32(newCoordIndex));
    m_scene->backupFieldsDone();
    m_meshDirty = true;
}

float*  
NodeIndexedFaceSet::intersectVector0Plane(Vec3f endPoint, Vec3f startPoint, 
                                          int direction)
{
    static float ret[3];
    Vec3f normal;
    switch (direction) {
      case 0:
        normal.x = 1;
        normal.y = 0;
        normal.z = 0;
        break;
      case 1:
        normal.x = 0;
        normal.y = 1;
        normal.z = 0;
        break;
      case 2:
        normal.x = 0;
        normal.y = 0;
        normal.z = 1;
        break;
    }
    Vec3f pointOfPlane(0.0f, 0.0f, 0.0f);
    Vec3f direct = (endPoint - startPoint);
    Vec3f diff = (endPoint - pointOfPlane);
    float len = diff.dot(normal);
    float a = direct.dot(normal);
    if (a == 0) 
        return NULL;
    Vec3f result = endPoint - direct * len / a;
    if ((result.x == 0) && (result.y == 0) && (result.z == 0))
        if ((endPoint.x != 0) || (endPoint.y != 0) || (endPoint.z != 0))
            return NULL;
    ret[0] = result.x;
    ret[1] = result.y;
    ret[2] = result.z;
    return ret;
}

void            
NodeIndexedFaceSet::makeSymetric(int direction, bool plus)
{
    NodeCoordinate *ncoord = (NodeCoordinate *)coord()->getValue();
    if (ncoord == NULL)
        return;
    MFVec3f *vertices = new MFVec3f((MFVec3f *)ncoord->point());
    MyArray<float> halfVertices;
    MyArray<float> startVertices;
    MyArray<int> halfCoordIndex;
    int newCi = 0;
    for (int i = 0; i < getMesh()->getNumFaces(); i++) {
        FaceData *face = getMesh()->getFace(i);
        int offset = face->getOffset();
        int numVertices = face->getNumVertices();
        int wrongVertices = 0;
        for (int j = offset; j < offset + numVertices; j++) {
            const float *vert = vertices->getValue(coordIndex()->getValue(j));
            if (plus) {
                if (vert[direction] < 0)
                    wrongVertices++; 
            } else {
                if (vert[direction] > 0)
                    wrongVertices++; 
            }
        }
        if ((wrongVertices >= 0) && (wrongVertices < numVertices)) {
            for (int j = offset; j < offset + numVertices; j++) {
                int ci = coordIndex()->getValue(j);
                const float *vert = vertices->getValue(ci);
                halfVertices.append(vert[0]);
                halfVertices.append(vert[1]);
                halfVertices.append(vert[2]);
                halfCoordIndex.append(newCi++); 
                int j2 = j;
                if (j == offset) {
                    j2 = offset + numVertices - 1;
                    int ci2 = coordIndex()->getValue(j2);
                    if (vertices->getValue(ci2) == 
                        vertices->getValue(coordIndex()->getValue(offset)))
                        j2--;
                } else
                    j2--;
                int ci2 = coordIndex()->getValue(j);
                const float *startVert = vertices->getValue(ci2);
                startVertices.append(startVert[0]);
                startVertices.append(startVert[1]);
                startVertices.append(startVert[2]);
            }
            halfCoordIndex.append(-1); 
        }
    }
    int halfVerticesSize = halfVertices.size() / 3;
    int halfCoordIndexSize = halfCoordIndex.size();
    for (int i = 0; i < halfVerticesSize; i++)
        if (plus) {
            if (halfVertices[3 * i + direction] < 0) {
                Vec3f endPoint(halfVertices[3 * i],
                               halfVertices[3 * i + 1],
                               halfVertices[3 * i + 2]);
                Vec3f startPoint(startVertices[3 * i],
                                 startVertices[3 * i + 1],
                                 startVertices[3 * i + 2]);
                float *result = intersectVector0Plane(endPoint, startPoint, 
                                                      direction);
                if (result != NULL) {
                    for (int j = 0; j < 3; j++)
                         halfVertices[3 * i + j] = result[j];
                }
            }
        } else {
            if (halfVertices[3 * i + direction] > 0) {
                Vec3f startPoint(startVertices[3 * i],
                                 startVertices[3 * i + 1],
                                 startVertices[3 * i + 2]);
                Vec3f endPoint(halfVertices[3 * i],
                               halfVertices[3 * i + 1],
                               halfVertices[3 * i + 2]);
                float *result = intersectVector0Plane(endPoint, startPoint, 
                                                      direction);
                if (result != NULL) {
                    for (int j = 0; j < 3; j++)
                         halfVertices[3 * i + j] = result[j];
                }
            }
        }
    MFVec3f *newVertices = new MFVec3f();
    MFInt32 *newCoordIndex = new MFInt32();
    for (int i = 0; i < halfVerticesSize; i++) {
        newVertices->appendSFValue(halfVertices[3 * i + 0],
                                   halfVertices[3 * i + 1],
                                   halfVertices[3 * i + 2]);
    }
    for (int i = 0; i < halfVerticesSize; i++) {
        halfVertices[3 * i + direction] *= -1;
        newVertices->appendSFValue(halfVertices[3 * i + 0],
                                   halfVertices[3 * i + 1],
                                   halfVertices[3 * i + 2]);
    }
    for (int i = 0; i < halfCoordIndexSize; i++)
        newCoordIndex->appendSFValue(halfCoordIndex[i]);
    for (int i = halfCoordIndexSize -1; i > -1; i--) {
        if (halfCoordIndex[i] > -1)
            halfCoordIndex[i] += newCi;
        if ((i != 0) || (halfCoordIndex[i] > -1))
            newCoordIndex->appendSFValue(halfCoordIndex[i]);
    }
    newCoordIndex->appendSFValue(-1);
    m_scene->backupFieldsStart();
    m_scene->backupFieldsAppend(ncoord, ncoord->point_Field());
    ncoord->point(new MFVec3f(newVertices));
    m_scene->backupFieldsAppend(this, coordIndex_Field());
    coordIndex(new MFInt32(newCoordIndex));
    m_scene->backupFieldsAppend(this, color_Field());
    color(new SFNode());
    m_scene->backupFieldsAppend(this, normal_Field());
    normal(new SFNode());
    m_scene->backupFieldsAppend(this, texCoord_Field());
    texCoord(new SFNode());
    m_scene->backupFieldsAppend(this, colorIndex_Field());
    colorIndex(new MFInt32);
    m_scene->backupFieldsAppend(this, colorIndex_Field());
    normalIndex(new MFInt32);
    m_scene->backupFieldsAppend(this, texCoordIndex_Field());
    texCoordIndex(new MFInt32);
    m_scene->backupFieldsDone();
    m_meshDirty = true;
}

static float midOffset(int count, int numCount)
{
    if (count == 0)
        return 0;
    return count - 1;
}

static float border1(int count, int numCount)
{
    if (count == 0)
        return -1.0f;
    return (float)count - 1.0f;
}

static float border2(int count, int numCount)
{
    return border1(count,  numCount) + 0.25f;
}

static float border3(int count, int numCount)
{
    return border1(count,  numCount) + 0.75f;
}

static float border4(int count, int numCount)
{
    return border1(count,  numCount) + 1.0f;
}

static void storeVertexN(MFVec3f *vertices, MFInt32 *ci, Vec3f vertex, 
                         int face, int countX, int numX, int numY)
{
    Vec3f vec(vertex.x, vertex.y, vertex.z);

    if (numY == 2 && numX == 3) {
        if (face == FACE_FRONT || face == FACE_BACK) {
            vec.x *= 1.0f / (float)numX;
            vec.x += 1.0f;
            vec.x -= 1.0f / (float)numX;
        }
        if (face == FACE_TOP || face == FACE_BOTTOM) {
            vec.z /= (float)numX;
        }
    }
    if (numY == 3 && numX == 2) {
        if (face == FACE_BACK) {
            vec.x *= -1.0f;

            vec.x += 1.0f;
            vec.y += 1.0f;
        }
        if (face == FACE_FRONT) {
            vec.x += 1.0f;
            vec.y += 1.0f;
        }
        if (face == FACE_TOP || face == FACE_BOTTOM) {
            vec.z /= (float)numX;
        }
    }
    if (numX == 3 && numY == 3) {
        if (face == FACE_TOP || face == FACE_BOTTOM) {
           vec.z /= (float)numX;
        }
        if (face == FACE_FRONT || face == FACE_BACK) {
            vec.x /= (float)numX;
            vec.x += 1.0f - 1.0f / (float)numX;
        }
        if (face == FACE_RIGHT || face == FACE_LEFT) {
            vec.y /= (float)numX;
        }
    }
    ci->appendSFValue(vertices->getSFSize());
    vertices->setVec(vertices->getSFSize(), vec);
}

static void storeVertex(MFVec3f *vertices, MFInt32 *ci, Vec3f newVertex, 
                        int face, int countX, int countY, int numX, int numY)
{
    Vec3f vec(newVertex.x, newVertex.y, newVertex.z);
    if (numY == 1 && numX == 2) {
        if (face == FACE_FRONT || face == FACE_BACK) {
            vec.y *= 2.0;
            vec.x += 1.0f;
            vec.y -= 1.0f;
            if (face == FACE_BACK) {
                vec.x *= -1.0f;
                vec.x += 2.0f;
            }
        }
        if (face == FACE_TOP || face == FACE_BOTTOM) {
            float temp = vec.y;
            vec.y = vec.z;
            vec.z = temp;

            if (face == FACE_TOP) {
                vec.y = 1.0f;
                vec.x *= -1.0f; 
                vec.x -= 1.0f; 
                vec.z -= 1.0f;                 
            } else {
                vec.y = -1.0f; 
                vec.x += 1.0f; 
                vec.z += 1.0f; 
            }                
            vec.z *= 2.0f;
            vec.z += 1.0f;
        }
        if (face == FACE_RIGHT || face == FACE_LEFT) {
            float temp = vec.x;
            vec.x = vec.z;
            vec.z = temp;

            if (face == FACE_RIGHT) {
                vec.x = 1.0f;
                vec.y *= -1.0f; 
                vec.z -= 1.0f;                 
            } else {
                vec.x = -1.0f; 
                vec.y -= 1.0f; 
                vec.z += 1.0f; 
            }    
            vec.y *= 2.0f;            
            vec.y += 1.0f;
        }
    }
    if (numY == 1 && numX == 3) {
        if (face == FACE_FRONT || face == FACE_BACK) {
            vec.y *= 2.0f; 
            vec.y += 1.0f; 
            vec.x *= 2.0f / (float)numX; 
            vec.x += 1.0f / (float)numX;
            if (face == FACE_BACK) {
                vec.x *= -1.0f;
                vec.x -= -4.0f / (float)numX;
            }
        }       
        if (face == FACE_TOP || face == FACE_BOTTOM) {
            float temp = vec.y;
            vec.y = vec.z;
            vec.z = temp;
    
            vec.x *= 2.0f / (float)numX; 
            vec.z *= 2.0f; 
            if (face == FACE_TOP)
                vec.z *= -1.0f;
            vec.x += 1.0f;
            if (face == FACE_TOP)
                vec.y += 2.0f;
            else
                vec.y -= 2.0f;
            vec.z += 1.0f;
            if (face == FACE_BOTTOM)
                vec.z += 2.0f;
          
        }
        if (face == FACE_LEFT || face == FACE_RIGHT) {
            float temp = vec.x;
            vec.x = vec.z;
            vec.z = temp;

            vec.z *= 2.0f * (float)numY / (float)numX;

            if (face == FACE_RIGHT) {
                vec.x = 1.0f;
                vec.z *= -1.0f;
                vec.z += 1.0f / (float)numX;
            } else {
                vec.x = -1.0f;
                vec.z += 1.0f;
            }
            vec.y *= 2.0f;
            vec.y -= 1.0f;
        }
    }
    if (numY == 2 && numX == 1) {
        if (face == FACE_FRONT || face == FACE_BACK) {
            vec.x *= 2.0f;
            if (face == FACE_FRONT)
                vec.x += 1.0f;
            else {
                vec.x *= -1.0f;
                vec.x += 3.0f;
            }
            vec.y -= 1.0f;
        }
        if (face == FACE_RIGHT || face == FACE_LEFT) {
            float temp = vec.x;
            vec.x = vec.z;
            vec.z = temp;

            if (face == FACE_RIGHT) {
                vec.z *= -2.0f; 
                vec.x = 1;
                vec.y -= 1.0f;
                vec.z += 3.0f;
            } else {
                vec.x = -1;
                vec.z *= 2.0f; 
                vec.y -= 1.0f; 
                vec.z += 1.0f;
            }
        }
    }
    if (numY == 2 && numX == 2) {
        if (face == FACE_FRONT || face == FACE_BACK) {
            vec.x += 1.0f;
            vec.y -= 1.0f;
        }
        if (face == FACE_TOP || face == FACE_BOTTOM) {
            float temp = vec.y;
            vec.y = vec.z;
            vec.z = temp;

            if (face == FACE_TOP)
                vec.z *= -1.0f;
            else            
                vec.z *= +1.0f;
            if (face == FACE_TOP)
                vec.y = 1.0f;
            else
                vec.y = -1.0f;
            vec.x += 1.0;
            vec.z += 1.0;
        }
        if (face == FACE_LEFT || face == FACE_RIGHT) {
            float temp = vec.x;
            vec.x = vec.z;
            vec.z = temp;
            if (face == FACE_LEFT) {
                vec.x = -1.0f;
                vec.y -= 1.0f;
                vec.z += 1.0f;
            } else {
                vec.x = 1.0f;
                vec.y *= -1.0f;
                vec.y += 1.0f;
                vec.z -= 1.0f;
            }
        }
    }
    if (numY == 2 && numX == 3) {
        if (face == FACE_FRONT) {
            vec.x *= 2.0f;
            vec.x -= 1.0f - 0.025;
            vec.y -= 1.0f;
        }
        if (face == FACE_BACK) {
            vec.x *= 2.05f;
            vec.x -= 1.0f - 0.025;
            vec.x *= -1.0f;
            vec.y -= 1.0f;
        }
        if (face == FACE_TOP || face == FACE_BOTTOM) {
            float temp = vec.y;
            vec.y = vec.x;
            vec.x = temp;
    
            temp = vec.y;
            vec.y = vec.z;
            vec.z = temp;
    
            if (face == FACE_BOTTOM) {
                vec.y = -1.0f;            
                vec.z *= -2.0f;
            } else
                vec.y = 1.0f;            

            vec.x += 1.0f / (float)numY;            
            vec.z -= 1.0f / (float)numY;            
    
            vec.x *= 2.0f - 2.0f / (float)numY; 
    
            vec.x -= 1.5;
    
            if (face == FACE_TOP) {
                vec.z *= 2.0f;
                vec.z += 2.0f;
            } else {
                vec.x += 2.0f;            
                vec.z -= 0.333;
            }
        }
        if (face == FACE_RIGHT || face == FACE_LEFT) {
            float temp = vec.x;
            vec.x = vec.z;
            vec.z = temp;

            vec.z *= -(float)numY / (float)numX;
            if (face == FACE_RIGHT) {
                vec.x = 1.0f;
            } else {
                vec.x = -1.0f;
                vec.z *= -1.0f;
                vec.z += 0.05f - 2 / (float)numX;
            }
            vec.y += - 1.27;
            vec.y += 0.75f / (float)numX;
            vec.z += 1.0f;
        }    
    }
    if (numY == 3 && numX == 1) {
        if (face == FACE_FRONT || face == FACE_BACK) {
            vec.x *= 2.0f;
            vec.x += 1.0f;
            vec.y *= 0.6666f;
            vec.y -= 1.0f;
        }
        if (face == FACE_TOP) {
            float temp = vec.y;
            vec.y = vec.x;
            vec.x = temp;
    
            temp = vec.y;
            vec.y = vec.z;
            vec.z = temp;
    
            vec.y += 2.0f;            
            vec.x += 1.0f / (float)numY;            
            vec.z -= 1.0f / (float)numY;            
    
            vec.x *= 2.0f; // - 2.0f / (float)numY; 
    
            vec.x += -2.0f + 0.45;
    
            vec.z *= 2.0f;
            vec.z += 2.0f;
        }
        if (face == FACE_BOTTOM) {
            float temp = vec.y;
            vec.y = vec.x;
            vec.x = temp;
    
            temp = vec.y;
            vec.y = vec.z;
            vec.z = temp;
    
            vec.y -= 2.0f;            
            vec.x += 1.0f / (float)numY;            
            vec.z -= 1.0f / (float)numY;            
    
            vec.x *= 2.0f - 2.0f / (float)numY; 
    
            vec.x += 0.5;
    
            vec.z *= -2.0f;
            vec.z -= 2.0f;
        }
    }
    if (numY == 3 && numX == 2) {
        if (face == FACE_FRONT || face == FACE_BACK) {
            vec.x *= 2.0f;
            if (face == FACE_FRONT) {
                vec.y -= 1.0f;
            } else {
                vec.y -= 1.0f;
            }
            vec.x -= 1.0f;
            vec.x *= 0.5f;
            vec.x += 1.5f / (float)numY;
            vec.y *= (float)numX / (float)numY;
            vec.y += 0.5f / (float)numY;
            vec.y -= 1.5f;
        }
        if (face == FACE_TOP) {
            float temp = vec.y;
            vec.y = vec.z;
            vec.z = temp;

            vec.y = 1.0f;
            vec.z *= (float)numX / (float)numY;

            vec.x += 3.0f / (float)numY;            
    
            vec.x *= -1.0f;
    
            vec.z *= 2.0f;
            vec.z += 2.0f - 4.0f;
        }
        if (face == FACE_BOTTOM) {
            float temp = vec.y;
            vec.y = vec.z;
            vec.z = temp;

            vec.y = -1.0f;
            vec.z *= (float)numX / (float)numY;

            vec.x += 3.0f / (float)numY;            
            vec.z += 2.0f / (float)numY;            
    
            vec.x *= -1.0f;
    
            vec.z *= 2.0f;
            vec.z += 2.0f - 4.0f;
        }
        if (face == FACE_RIGHT) {
            float temp = vec.y;
            vec.y = vec.z;
            vec.z = temp;
    
            temp = vec.y;
            vec.y = vec.x;
            vec.x = temp;

            vec.z *= -((float)numY - 1) / (float)numY;

            vec.x += 2.0f;
            vec.y -= 1.0f;
            vec.y *= 2.0f;
            vec.z -= 1.0f;
            vec.y += 1.0f;
        }
        if (face == FACE_LEFT) {
            float temp = vec.y;
            vec.y = vec.z;
            vec.z = temp;
    
            temp = vec.y;
            vec.y = vec.x;
            vec.x = temp;
    
            vec.x *= -1.0f;
            vec.z *= ((float)numY - 1) / (float)numY;

            vec.x += 2.0f;
            vec.y += 1.0f;
            vec.y *= 2.0f;
            vec.y += 1.0f;
            vec.z *= -1.0f;
        }
    }
    if (numX == numY && numX == 3) {
        if (face == FACE_TOP) {
            float temp = vec.y;
            vec.y = vec.x;
            vec.x = temp;
    
            temp = vec.y;
            vec.y = vec.z;
            vec.z = temp;
    
            vec.y += 2.0f;            
            vec.x += 1.0f / (float)numY;            
            vec.z -= 1.0f / (float)numY / 4.0f;            
    
            vec.x *= 1.0f - 1.0f / (float)numY;     
            vec.x -= 1.5 + 1.0f / 24.0f - 0.035f;
    
            vec.z *= 2.0f;
            vec.z += 3.25f - 1.0f / 24.0f - 1.0f / 48.0f;
        }
        if (face == FACE_BOTTOM) {
            float temp = vec.y;
            vec.y = vec.x;
            vec.x = temp;
    
            temp = vec.y;
            vec.y = vec.z;
            vec.z = temp;
    
            vec.y -= 2.0f;            
            vec.x += 1.0f / (float)numY;            
            vec.z -= 1.0f / (float)numY / 4.0f;            
    
            vec.x *= 1.0f - 1.0f / (float)numY;     
            vec.x -= 1.5f + 1.0f / 24.0f;
            vec.x += 1.0f;            
    
            vec.z *= -1.0f;
            vec.z += 3.25f - 1.0f / 24.0f - 1.0f / 48.0f;
        }
        if (face == FACE_FRONT) {
            vec.x -= 0.5f;
            vec.x *= 2.0f;
            if (numY > 1) 
                vec.y *= ((float)numY - 1.0f) / (float)numY;
            vec.y -= 1.0f;
        }
        if (face == FACE_BACK) {
            vec.x -= 0.5f;
            vec.x *= -2.0f;
            if (numY > 1) 
                vec.y *= ((float)numY - 1.0f) / (float)numY;
            vec.y -= 1.0f;
        }
        if (face == FACE_RIGHT) {
            float temp = vec.y;
            vec.y = vec.z;
            vec.z = temp;
    
            temp = vec.y;
            vec.y = vec.x;
            vec.x = temp;
    
            vec.x += 2.0f;
            vec.y -= 1.0f;
            vec.y *= 2.0f;
            vec.z -= 1.0f;
            vec.y += 1.0f;
            vec.z *= ((float)numY - 1) / (float)numY;
        }
        if (face == FACE_LEFT) {
            float temp = vec.y;
            vec.y = vec.z;
            vec.z = temp;
    
            temp = vec.y;
            vec.y = vec.x;
            vec.x = temp;
    
            vec.y += 1.0f;
            vec.y *= 2.0f;
            vec.z *= ((float)numY - 1) / (float)numY;
            vec.y += 1.0f;
            vec.z *= -1.0f;
        }
    }
    storeVertexN(vertices, ci, vec, face, countX, numX, numY);
}

static void storeVertexMid(MFVec3f *vertices, MFInt32 *ci, Vec3f newVertex, 
                           int face, int countX, int countY, 
                           int numX, int numY, bool first)
{
    Vec3f vec(newVertex.x, newVertex.y, newVertex.z);
    if (numY == 1 && numX == 2) {
        if (face == FACE_FRONT || face == FACE_BACK) {
            vec.x *= -1.0;
            vec.x += -1.0f / (float)numX;
        }
        if (face == FACE_TOP || face == FACE_BOTTOM) {
            if (face == FACE_TOP)
                vec.y = 1.0;
            else {
                vec.y = -1.0;
            }
            vec.x *= -1.0;
            vec.x += 1.0;
            vec.x += -1.0f / (float)numX;
        }
        if (face == FACE_RIGHT || face == FACE_LEFT) {
            vec.z *= -1.0f / (float)numX;             
            if (face == FACE_LEFT) {
                vec.x = -1.0f;
            } else {
                vec.x = 1.0f;
            }
            vec.y -= 0.5f;
            vec.z -= 0.8f;        
        }
    } 
    if (numY == 1 && numX == 3) {
        if (face == FACE_FRONT || face == FACE_BACK) {
            vec.x *= -1.0;
            vec.x += -2.0f / (float)numX;
        }       
        if (face == FACE_TOP || face == FACE_BOTTOM) {
            vec.x *= -1.0f;             
            vec.x -= 1.0f / (float)numX - 1.0f;
            if (face == FACE_TOP)
                vec.y = 1.0f;
            else
                vec.y = -1.0f;
        }
        if (face == FACE_RIGHT || face == FACE_LEFT) {
            vec.z *= -1.0f / (float)numX;             
            vec.z += -0.25f / (float)numX;             
            vec.z += -0.01;             
            vec.y -= 0.75f - 0.25f / (float)numX;
            if (face == FACE_LEFT) {
                vec.x = -1.0f;
            } else {
                vec.x = 1.0f;
            }
        }
    }
    if (numY == 2 && numX == 1) {
        if (face == FACE_FRONT || face == FACE_BACK) {
            vec.x *= -1.0f; 
            vec.x += 0.75f; 
        }
        if (face == FACE_TOP || face == FACE_BOTTOM) {
            vec.x *= -1.0f; 
            vec.z *= 0.5f;
            if (face == FACE_TOP)
                vec.y = 1.0f;
            else
                vec.y = -1.0f;
            vec.x -= 0.75f;
            vec.z -= 0.25f;
        }
        if (face == FACE_RIGHT || face == FACE_LEFT) {
            if (face == FACE_RIGHT) {
                vec.x = -1;
            } else {
                vec.x = 1;
            }
            vec.y -= 0.5f; 
            vec.z += 3.625f;
        }
    }
    if (numY == 2 && numX == 2) {
        if (face == FACE_FRONT || face == FACE_BACK) {
            vec.x *= -1.0f; 
            vec.x += 0.25f; 
        }
        if (face == FACE_TOP || face == FACE_BOTTOM) {
            vec.x *= -1.0f;            
            vec.z *=  0.5f;            

            if (face == FACE_TOP)
                vec.y = 1.0f;
            else
                vec.y = -1.0f;
            vec.x -= 0.25f;
            vec.z -= 0.25f;
        }
        if (face == FACE_LEFT || face == FACE_RIGHT) {
            vec.z *= 0.51f;
            if (face == FACE_RIGHT) {
                vec.x = -1.0f;
                vec.y -= 0.5f;
                vec.z += 2.15;
            } else {
                vec.x = 1.0f;
                vec.y -= 0.5f;
                vec.z += 2.15f;
            }
        }
    } 
    if (numY == 2 && numX == 3) {
        if (face == FACE_FRONT || face == FACE_BACK) {
            vec.x *= -1.0f; 
            vec.x += 0.08f;
        }       
        if (face == FACE_TOP) {
            float temp = vec.x;
            vec.x = vec.z;
            vec.z = temp;

            vec.x *= 1.0f / (float)numY;
            vec.x -= 0.235f;
            vec.y = 1.0f;
            vec.z += 0.0815f;
        }
        if (face == FACE_BOTTOM) {
            float temp = vec.x;
            vec.x = vec.z;
            vec.z = temp;

            vec.x *= 1.0f / (float)numY;
            vec.x -= 0.235f;
            vec.y = -1.0f;
            vec.z += 0.1365f;
        }
        if (face == FACE_RIGHT || face == FACE_LEFT) {
            vec.z *= -1.0f / (float)numY;
            if (face == FACE_RIGHT) {
                vec.x = 1.0f;
            }
            if (face == FACE_LEFT)
                vec.x = -1.0f;
            vec.y -= 1.189f;
            vec.z -= 0.5f;
        }
    }
    if (numY == 3 && numX== 1) {
        if (face == FACE_FRONT || face == FACE_BACK) {
            vec.x *= -1.0f; 
            vec.x += 1.716666f; 
            vec.y -= 1.1f; 
        }
    }
    if (numY == 3 && numX == 2) {
        if (face == FACE_FRONT || face == FACE_BACK) {
            vec.x *= -1.0f; 
            vec.x += 1.0f; 
            vec.x += 0.5f / (float)numY;
            vec.y -= 1.0f; 
        }
        if (face == FACE_TOP) {
            vec.z *= -1.0f /numY; 
            vec.z += -1.0f / numY;
            vec.x += 2.002f - 1.0f / numY;
            vec.y = 1.0f;
            vec.z += 1.10285f;
        }
        if (face == FACE_BOTTOM) {
            vec.z *= -1.0f / numY;
            vec.z += -1.0f / numY;
            vec.x += 1.972f - 1.0f / numY;;
            vec.y = -1.0f;
            vec.z += 1.12585f;
        }
        if (face == FACE_RIGHT) {
        }
    }
    if (numX == 3 && numY == 3) {
        if (face == FACE_TOP) {
            vec.z *= -1.0f / numY;
            vec.z += -1.0f / numY;
            vec.x += 1.05f;
            vec.y = 1.0f;
            vec.z += 1.11f;
        }
        if (face == FACE_BOTTOM) {
            vec.z *= -1.0f / numY;
            vec.z += -1.0f / numY;
            vec.x += 1.05f;
            vec.y = -1.0f;
            vec.z += 1.11f;
        }
        if (face == FACE_FRONT) {
            vec.x *= -1.0f;
            vec.x += 1.0f;
            vec.y -= 1.0f;
            vec.y *= 1.0f + 0.01f;
        }
        if (face == FACE_BACK) {
            vec.x *= -1.0f;
            vec.x += 1.05f;
            vec.y -= 1.0f;
            vec.y *= 1.0f + 0.01f;
        }
        if (face == FACE_RIGHT) {
            vec.x = 1.0f;
            vec.z *= -1.0f / numY;
            vec.z -= 0.5f / numY;
            vec.y -= 1.0f;
            vec.z += 1.0f;
            vec.z -= 0.05f;
        }
        if (face == FACE_LEFT) {
            vec.x = -1.0f;
            vec.z *= -1.0f / numY;
            vec.z -= 0.5f / numY;
            vec.y -= 1.0f;
            vec.z += 0.95f;
            vec.z -= 0.05f;
        }
    }
    ci->appendSFValue(vertices->getSFSize());
    vertices->setVec(vertices->getSFSize(), vec);
}

void            
NodeIndexedFaceSet::insetFaces(float factor, int numX, int numY)
{
    m_faces.resize(0);
    if (m_mesh == NULL)
        return;
    NodeCoordinate *ncoord = (NodeCoordinate *)coord()->getValue();
    if (ncoord == NULL)
        return;
    MFVec3f *vertices = new MFVec3f((MFVec3f *)ncoord->point()->copy());
    MFVec3f *vertices2 = new MFVec3f((MFVec3f *)ncoord->point()->copy());
    MFInt32 *ci = (MFInt32 *)coordIndex()->copy();
    MyArray<int> facesToDelete;
    for (int i = 0; i < m_scene->getSelectedHandlesSize(); i++) {
         facesToDelete.append(m_scene->getSelectedHandle(i));
    }
    if (m_scene->getXSymetricMode())
        for (int i = 0; i < m_scene->getSelectedHandlesSize(); i++) {
            int iface = symetricFace(m_scene->getSelectedHandle(i));
            if (iface > -1)
                if (!m_scene->isInSelectedHandles(iface))
                    facesToDelete.append(iface);
    }
    if (numX == 1  && numY == 1) {
        int inc = m_scene->getSelectedHandlesSize();
        int numFace = m_mesh->getNumFaces() - 1;
        for (long i = 0; i < facesToDelete.size(); i++) {
             FaceData *face = getMesh()->getFace(facesToDelete[i]);
             int offset = face->getOffset();
             int numVertices = face->getNumVertices();
             Vec3f mid(0, 0, 0);
             for (int j = offset; j < offset + numVertices; j++) {
                 mid += vertices->getVec(ci->getValue(j));
             }
             mid /= numVertices;
             int start = vertices->getSFSize();
             for (int j = offset; j < offset + numVertices; j++) {
                  Vec3f newVertex = mid + (vertices->getVec(ci->getValue(j)) - 
                                          mid) *factor;
                  vertices->setVec(vertices->getSFSize(), newVertex);
            }
            for (int j = offset; j < offset + numVertices; j++) {
                 int k = j - offset;
                 ci->appendSFValue(ci->getValue(j));
                 if (j + 1 >= offset + numVertices)
                     ci->appendSFValue(ci->getValue(offset));
                 else
                     ci->appendSFValue(ci->getValue(j + 1));
                 if (k + 1 >= numVertices)
                     ci->appendSFValue(start);
                 else
                     ci->appendSFValue(start + k + 1);
                 ci->appendSFValue(start + k);
                 ci->appendSFValue(-1);
                 numFace++;
             }
             for (int j = offset; j < offset + numVertices; j++)
                 ci->appendSFValue(start + j - offset);
             ci->appendSFValue(-1);
             numFace++;
             if (inc == 1)
                 m_scene->addSelectedHandle(numFace - 1);
             else
                 m_scene->addSelectedHandle(numFace - 2 * inc);
        }
    } else {
        int inc = m_scene->getSelectedHandlesSize();
        Vec3f vecMax(-FLT_MAX, -FLT_MAX, -FLT_MAX);
        Vec3f vecMin(FLT_MAX, FLT_MAX, FLT_MAX);
        for (long i = 0; i < facesToDelete.size(); i++) {
             FaceData *face = getMesh()->getFace(facesToDelete[i]);
             int offset = face->getOffset();
             int numVertices = face->getNumVertices();
             int numYGreater0 = 0;
             int numYLess0 = 0;
             int numXGreater0 = 0;
             int numXLess0 = 0;
             int numZGreater0 = 0;
             int numZLess0 = 0;
             for (int j = offset; j < offset + numVertices; j++) {
                 Vec3f val = vertices->getVec(ci->getValue(j));
                 if (val.y > 0) 
                     numYGreater0++;
                 if (val.y < 0) 
                     numYLess0++;
                 if (val.x > 0) 
                     numXGreater0++;
                 if (val.x < 0) 
                     numXLess0++;
                 if (val.z > 0) 
                     numZGreater0++;
                 if (val.z < 0) 
                     numZLess0++;
                 
                 if (val.x > vecMax.x)
                     vecMax.x = val.x;
                 if (val.y > vecMax.y)
                     vecMax.y = val.y;
                 if (val.z > vecMax.z)
                     vecMax.z = val.z;
                 if (val.x < vecMin.x)
                     vecMin.x = val.x;
                 if (val.y < vecMin.y)
                     vecMin.y = val.y;
                 if (val.z < vecMin.z)
                     vecMin.z = val.z;
             }
             if (numYGreater0 > numYLess0 && 
                 numYGreater0 > numXLess0 && 
                 numYGreater0 > numYLess0 && 
                 numYGreater0 > numXGreater0 &&
                 numYGreater0 > numZGreater0)
                 face->setType(FACE_TOP); 
             if (numYLess0 > numYGreater0 && 
                 numYLess0 > numXGreater0 && 
                 numYLess0 > numZGreater0 &&
                 numYLess0 > numXLess0 &&
                 numYLess0 > numZLess0)
                 face->setType(FACE_BOTTOM); 
             if (numXGreater0 > numYLess0 && 
                 numXGreater0 > numXLess0 && 
                 numXGreater0 > numYLess0 && 
                 numXGreater0 > numYGreater0 &&
                 numXGreater0 > numZGreater0)
                 face->setType(FACE_RIGHT); 
             if (numXLess0 > numYGreater0 && 
                 numXLess0 > numXGreater0 && 
                 numXLess0 > numZGreater0 &&
                 numXLess0 > numYLess0 &&
                 numXLess0 > numZLess0)
                 face->setType(FACE_LEFT); 
             if (numZGreater0 > numYLess0 && 
                 numZGreater0 > numXLess0 && 
                 numZGreater0 > numYLess0 && 
                 numZGreater0 > numXGreater0 &&
                 numZGreater0 > numYGreater0)
                 face->setType(FACE_FRONT); 
             if (numZLess0 > numYGreater0 && 
                 numZLess0 > numXGreater0 && 
                 numZLess0 > numZGreater0 &&
                 numZLess0 > numXLess0 &&
                 numZLess0 > numYLess0)
                 face->setType(FACE_BACK);  
        }
        float f = (1.0f - factor);
        Vec3f lenX((vecMax.x - vecMin.x) * f / numX, 0.0f, 
                   (vecMax.z - vecMin.z) * f / numX);
        bool zDirection = false;
        if (lenX.z > lenX.x)
            zDirection = true;
        Vec3f lenY(0.0f, (vecMax.y - vecMin.y) * f / numY, 0.0f);
        for (int n = 0; n < numX; n++) 
            for (int m = 0; m < numY; m++) 
                for (long i = 0; i < facesToDelete.size(); i++) {
                    FaceData *face = getMesh()->getFace(facesToDelete[i]);
                    int offset = face->getOffset();
                    int numVertices = face->getNumVertices();
                    Vec3f mid(0, 0, 0);
                    for (int j = offset; j < offset + numVertices; j++) {
                        mid += vertices2->getVec(ci->getValue(j));
                    }
                    mid /= numVertices;
                    int midstart = vertices->getSFSize();
                    for (int j = offset + numVertices - 1; j >= offset; j--) {
                        Vec3f newVertex = mid + 
                                          (vertices2->getVec(ci->getValue(j))
                                          - mid) * factor;

                        newVertex.x *= lenX.x;
                        newVertex.y *= lenY.y;
                        float xoffset = 0.0f;
                        float yoffset = 0.0f;
                        if (numY == 2) {
                            xoffset -= 1.0f / (float)numY + 0.5f / (float)numY;
                            yoffset -= 1.0f / (float)numY;
                        }
                        if (numY == 3) {
                            xoffset = 1.0f / (float)numY - 2.0f;
                            yoffset = 1.0f / (float)numY;
                        }
                        if (face->getType() == FACE_FRONT ||
                            face->getType() == FACE_BACK) {
                            newVertex.x -= xoffset +
                                           2.0f * (float)n / (float)numX;
                            newVertex.y += yoffset + 
                                            2.0f * (float)m / (float)numY;
                        }
                        if (face->getType() == FACE_TOP ||
                            face->getType() == FACE_BOTTOM) {
                             newVertex.x += xoffset +
                                            (2.0f * (float)n) / (float)numX;
                             if (numY == 2) {
                                 if (numX == 1) {
                                     newVertex.z += yoffset + 
                                                    4.0f * (float)m / 
                                                        (float)numY;
                                 } else if (numX == 2) {
                                     newVertex.z += yoffset + 
                                                    4.0f * (float)m / 
                                                        (float)numY;
                                 } else 
                                     newVertex.z += yoffset + 
                                                    4.0f * (float)m / 
                                                        (float)numY;
                             }
                             if (numY == 3)
                                 newVertex.z += yoffset + 
                                                6.0f * (float)m / (float)numY;
                        }
                        if (face->getType() == FACE_RIGHT ||
                            face->getType() == FACE_LEFT) {
                             if (numY == 1) {
                                 yoffset = 1.0f;
                                 newVertex.z += -0.12f + 1.5f - 3.0f -
                                                 2.0f / (float)numX +
                                                 2.0f * (float)numY * (float)n;
                                 newVertex.y += yoffset - 1.0f / (float)numX +
                                                1.0f * (float)m;
                             }
                             if (numY == 2) {
                                 yoffset = 1.0f;
                                 newVertex.z += -0.12f + 1.5f - 3.0f -
                                                 2.0f / (float)numX +
                                                 0.66f * (float)numY * (float)n;
                                 if (numX == 2) {
                                      newVertex.z += -0.12f + 1.5f - 3.0f -
                                                     2.0f / (float)numX +
                                                     0.36f * (float)numY * 
                                                         (float)n;
                                      newVertex.y += yoffset 
                                                     - 2.0f / (float)numX +
                                                     1.0f * (float)m;
                                 } else
                                      newVertex.y += yoffset 
                                                     - 1.0f / (float)numX +
                                                     1.0f * (float)m;
                             }
                             if (numY == 3) {
                                 newVertex.y += 1.0f / (float)numY +
                                                2.0f * (float)n / (float) numX;
                                 newVertex.z += yoffset + 
                                                6.0f * (float)m / (float)numY;
                            }
                        }
                        storeVertexMid(vertices, ci, newVertex, face->getType(),
                                       n, m, numX, numY, false);
                    }
                    ci->appendSFValue(-1);
                }
        for (int n = 0; n < numX; n++) 
            for (int m = 0; m < numY; m++) 
                for (long i = 0; i < facesToDelete.size(); i++) {
                    FaceData *face = getMesh()->getFace(facesToDelete[i]);
                    int offset = face->getOffset();
                    int numVertices = face->getNumVertices();
                    for (int j = offset; j < offset + numVertices; j++) {
                        if (j == offset) {
                            // right limiter
                            Vec3f startVertex = 
                                 vertices->getVec(ci->getValue(j));

                            if (numY == 1 && numX == 2) {
                                if (face->getType() == FACE_FRONT ||
                                    face->getType() == FACE_BACK) {
                                    startVertex.y -= 1.0;
                                }
                                if (face->getType() == FACE_TOP ||
                                    face->getType() == FACE_BOTTOM) {
                                    startVertex.y -= 1.0;
                                }
                                if (face->getType() == FACE_RIGHT || 
                                    face->getType() == FACE_LEFT) {
                                    startVertex.y -= 1.0;
                                }
                            } 
                            if (numY == 1 && numX == 3) {
                                if (face->getType() == FACE_FRONT ||
                                    face->getType() == FACE_BACK) {
                                    startVertex.x += 3.0f / (float)numX;
                                    startVertex.y -= 2.0f;
                                }
                                if (face->getType() == FACE_TOP || 
                                    face->getType() == FACE_BOTTOM) {
                                    startVertex.y -= 1.0f;
                                }
                                if (face->getType() == FACE_RIGHT) {
                                    startVertex.y -= 1.0f;
                                }
                                if (face->getType() == FACE_LEFT) {
                                    startVertex.y -= 1.0f;
                                }
                            }
                            if (numY == 2 && numX == 3) {
                                if (face->getType() == FACE_FRONT ||
                                    face->getType() == FACE_BACK) {
                                    startVertex.x += 1.09f;// - 1.0f / (float)numX;
                                    startVertex.y += 1.0f + 2.25f / (float)numX;
                                }
                                if (face->getType() == FACE_TOP || 
                                    face->getType() == FACE_BOTTOM) {       
                                    startVertex.x += 1.0f;
                                    startVertex.y += 1.0f + 
                                                     2.25f / (float)numX;
                                }
                                if (face->getType() == FACE_RIGHT ||
                                    face->getType() == FACE_LEFT) {
                                    startVertex.x += 1.0f;
                                    startVertex.y += 2.0f - 0.5f / numX - 0.1;
                                }
                                startVertex.y += - 2.0f + 0.25f; 
                            }
                            if (numY == 3 && numX == 1) {
                                if (face->getType() == FACE_FRONT || 
                                    face->getType() == FACE_BACK) {
                                    startVertex.y += 1.0f;
                                }
                                if (face->getType() == FACE_TOP || 
                                    face->getType() == FACE_BOTTOM) {
                                    startVertex.y += 3.0f;
                                }
                                if (face->getType() == FACE_RIGHT || 
                                    face->getType() == FACE_LEFT) {
                                    startVertex.y += 1.0f;
                                }
                            }
                            if (numY == 3 && numX == 2) {
                                if (face->getType() == FACE_FRONT || 
                                    face->getType() == FACE_BACK) {
                                    startVertex.y += 1.0f; 
                                }
                                if (face->getType() == FACE_TOP || 
                                    face->getType() == FACE_BOTTOM) {
                                    startVertex.y += 1.0f; 
                                }
                                if (face->getType() == FACE_RIGHT || 
                                    face->getType() == FACE_LEFT) {
                                    startVertex.y -= 3.0f; 
                                }
                            } 
                            if (numY == 3 && numX == 3) {
                                if (face->getType() == FACE_TOP)    
                                    startVertex.y += 1.0f + 
                                                     1.0f / (float)numY +
                                                     0.5f / (float)numY;
    
                                if (face->getType() == FACE_BOTTOM)    
                                    startVertex.y += 1.0f + 
                                                     1.0f / (float)numY +
                                                     0.5f / (float)numY;
    
                                if (face->getType() == FACE_FRONT)    
                                    startVertex.x += 1.0f;
                                if (face->getType() == FACE_FRONT)
                                    startVertex.y += 2.0f / (float)numY;
                                if (face->getType() == FACE_FRONT)
                                    startVertex.y += 1.0f / (float)numY;
                                if (face->getType() == FACE_FRONT)
                                    startVertex.y += 0.5f / 3.0f / (float)numY -
                                                     0.125f / 4.0f;
    
                                if (face->getType() == FACE_BACK)    
                                    startVertex.x += 1.0f;
                                if (face->getType() == FACE_BACK)
                                    startVertex.y += 2.0f / (float)numY;
                                if (face->getType() == FACE_BACK)
                                    startVertex.y += 1.0f / (float)numY;
                                if (face->getType() == FACE_BACK)
                                    startVertex.y += 0.5f / 3.0f / (float)numY -
                                                     0.125f / 4.0f;
    
                                if (face->getType() == FACE_RIGHT)
                                    startVertex.y += 1.0f / (float)numY -
                                                     0.25f / (float)numY + 0.25;
                                if (face->getType() == FACE_LEFT)
                                    startVertex.y -= 1.5 / (float)numY;
                            }

                            Vec3f newVertex = startVertex;
    
                            newVertex.x -= border1(n, numX);
                            newVertex.y -= border1(m, numY);
    
                            storeVertex(vertices, ci, newVertex, 
                                        face->getType(), n,m, numX, numY);
    
                            newVertex = startVertex;
    
                            newVertex.x -= border2(n, numX);
                            newVertex.y -= border2(m, numY);
    
                            storeVertex(vertices, ci, newVertex,
                                        face->getType(), n, m, numX, numY);
    
                            newVertex = startVertex;
    
                            newVertex.x -= border2(n, numX);
                            newVertex.y -= border3(m, numY);
    
                            storeVertex(vertices, ci, newVertex,
                                        face->getType(), n, m, numX, numY);
    
                            newVertex = startVertex;
    
                            newVertex.x -= border1(n, numX);
                            newVertex.y -= border4(m, numY);
    
                            storeVertex(vertices, ci, newVertex, 
                                        face->getType(), n ,m,numX, numY);
                            ci->appendSFValue(-1);
                        }
                        if (j == offset + 1) {
                            // top limiter
                            Vec3f startVertex = vertices->getVec(
                                 ci->getValue(j - 1));

                            if (numY == 1 && numX == 2) {
                                if (face->getType() == FACE_FRONT ||
                                    face->getType() == FACE_BACK) {
                                    startVertex.y += 1.0;
                                }
                                if (face->getType() == FACE_TOP ||
                                    face->getType() == FACE_BOTTOM) {
                                    startVertex.y -= 1.0;
                                }
                                if (face->getType() == FACE_RIGHT || 
                                    face->getType() == FACE_LEFT) {
                                    startVertex.y -= 1.0;
                                }
                            } 
                            if (numY == 1 && numX == 3) {
                                if (face->getType() == FACE_FRONT ||
                                    face->getType() == FACE_BACK) {
                                    startVertex.y -= 2.0f;
                                    startVertex.x += 3.0f / numX;
                                }
                                if (face->getType() == FACE_TOP ||
                                    face->getType() == FACE_BOTTOM) {
                                    startVertex.y -= 1.0f;
                                }
                                if (face->getType() == FACE_RIGHT ||
                                    face->getType() == FACE_LEFT) {
                                    startVertex.y -= 1.0f;
                                }
                            }
                            if (numY == 2 && numX == 3) {
                                startVertex.x += 1.0f;
                            }
                            if (numY == 3 && numX == 1) {
                                if (face->getType() == FACE_FRONT || 
                                    face->getType() == FACE_BACK) {
                                    startVertex.y += 1.0f;
                                }
                            }
                            if (numY == 3 && numX == 2) {
                                startVertex.y += 1.0f;
                            }
                            if (numY == 3 && numX == 3) { 
                                if (face->getType() == FACE_TOP) {
                                    startVertex.y += 4.5f /(float)numY;
                                }
                                if (face->getType() == FACE_BOTTOM) {
                                    startVertex.y += 4.5f /(float)numY;
                                }
    
                                if (face->getType() == FACE_FRONT)    
                                    startVertex.x += 1.0f;
                                if (face->getType() == FACE_FRONT)
                                    startVertex.y += 1.5f; 
                                if (face->getType() == FACE_FRONT)
                                    startVertex.y -= 1.0f / (float)numY +
                                                     0.5f / (float)numY;
                                if (face->getType() == FACE_FRONT)
                                    startVertex.y += 0.5f / 3.0f / (float)numY;
    
                                if (face->getType() == FACE_BACK)    
                                    startVertex.x += 1.0f;
                                if (face->getType() == FACE_BACK)
                                    startVertex.y += 1.5f; 
                                if (face->getType() == FACE_BACK)
                                    startVertex.y -= 1.0f / (float)numY +
                                                     0.5f / (float)numY;
                                if (face->getType() == FACE_BACK)
                                    startVertex.y += 0.5f / 3.0f / (float)numY;
    
                                if (face->getType() == FACE_RIGHT)
                                    startVertex.y += 1.5f / (float)numY;
                                if (face->getType() == FACE_LEFT)
                                    startVertex.y -= 1.5f / (float)numY;
                            }

                            Vec3f newVertex = startVertex;
    
                            newVertex.x -= border4(n, numX);
                            newVertex.y -= border1(m, numY);
    
                            storeVertex(vertices, ci, newVertex, 
                                        face->getType(), n,m, numX, numY);
    
                            newVertex = startVertex;
    
                            newVertex.x -= border3(n, numX);
                            newVertex.y -= border2(m, numY);
    
                            storeVertex(vertices, ci, newVertex, 
                                        face->getType(), n, m, numX, numY);
    
                            newVertex = startVertex;
    
                            newVertex.x -= border2(n, numX);
                            newVertex.y -= border2(m, numY);
    
                            storeVertex(vertices, ci, newVertex, 
                                        face->getType(), n, m, numX, numY);
    
                            newVertex = startVertex;
        
                            newVertex.x -= border1(n, numX);
                            newVertex.y -= border1(m, numY);

                            storeVertex(vertices, ci, newVertex, 
                                        face->getType(), n, m, numX, numY);
                             ci->appendSFValue(-1);
                        }
                        if (j == offset + 2) {
                            // bottom limiter
                            Vec3f startVertex = 
                                 vertices->getVec(ci->getValue(j - 2));

                            if (numY == 1 && numX == 3) {
                                if (face->getType() == FACE_FRONT ||
                                    face->getType() == FACE_BACK) {
                                    startVertex.y -= 1.0f;
                                    startVertex.x += 1.00f;
                                }
                            }
                            if (numY == 2 && numX == 3) {
                                startVertex.x += 1.0f; 
                            }
                            if (numY == 3 && numX == 3) {
                                if (face->getType() == FACE_TOP) {
                                    startVertex.y += 4.0f / (float)numY -
                                                     2.5f / (float)numY;
                                }
                                if (face->getType() == FACE_BOTTOM) {
                                    startVertex.y += 4.0f / (float)numY -
                                                     2.5f / (float)numY;
                                }
    
                                if (face->getType() == FACE_FRONT)
                                    startVertex.x += 1.0f;
                                if (face->getType() == FACE_FRONT)
                                    startVertex.y -= - 0.5f / (float)numY + 0.25f; 
                                if (face->getType() == FACE_FRONT)
                                    startVertex.y += 0.125f / 4.0f;
                                if (face->getType() == FACE_FRONT)
                                    startVertex.y += 0.5f / 3.0f / (float)numY;
    
                                if (face->getType() == FACE_BACK)
                                    startVertex.x += 1.0f;
                                if (face->getType() == FACE_BACK)
                                    startVertex.y -= - 0.5f / (float)numY + 0.25f; 
                                if (face->getType() == FACE_BACK)
                                    startVertex.y += 0.125f / 4.0f;
                                if (face->getType() == FACE_BACK)
                                    startVertex.y += 0.5f / 3.0f / (float)numY;
    
                                if (face->getType() == FACE_RIGHT)
                                    startVertex.y -= 1.0f / (float)numY +
                                                 0.5f / (float)numY;
                                if (face->getType() == FACE_LEFT)
                                    startVertex.y -= 4.5f / (float)numY;

                            }
 
                            Vec3f newVertex = startVertex;
    
                            newVertex.x -= border1(n, numX);
                            newVertex.y += border1(m, numY);
    
                            storeVertex(vertices, ci, newVertex, 
                                        face->getType(), n, m, numX, numY);
    
                            newVertex = startVertex;
    
                            newVertex.x -= border2(n, numX);
                            newVertex.y += border2(m, numY);
    
                            storeVertex(vertices, ci, newVertex, 
                                        face->getType(), n, m, numX, numY);
    
                            newVertex = startVertex;
    
                            newVertex.x -= border3(n, numX);
                            newVertex.y += border2(m, numY);
    
                            storeVertex(vertices, ci, newVertex, 
                                        face->getType(), n, m, numX, numY);
    
                            newVertex = startVertex;
 
                            newVertex.x -= border4(n, numX);
                            newVertex.y += border1(m, numY);
    
                            storeVertex(vertices, ci, newVertex, 
                                        face->getType(), n, m, numX, numY);
                            ci->appendSFValue(-1);
                        }
                        if (j == offset + 3) {
                            // left limiter
                            Vec3f startVertex = vertices->getVec(
                                 ci->getValue(j - 3));
                          
                            if (numY == 1 && numX == 3) {
                                if (face->getType() == FACE_FRONT ||
                                    face->getType() == FACE_BACK) {
                                    startVertex.x += 3.0f / (float)numX;
                                    startVertex.y *= 2.0f;  
                                    startVertex.y -= 2.0f;  
                                }
                            }
                            if (numX == 2 && numY == 2) { 
                                if (face->getType() == FACE_RIGHT ||
                                    face->getType() == FACE_LEFT) {
                                    startVertex.x += 2.0f / (float)numY;
                                }
                                if (face->getType() == FACE_RIGHT ||
                                    face->getType() == FACE_LEFT) {
                                    startVertex.x -= 1.0;
                                }
                            }
                            if (numY == 2 && numX == 3) {
                                startVertex.x += 1.0f; 
                            }
                            if (numX == 3 && numY == 3) { 
                                if (face->getType() == FACE_FRONT)    
                                    startVertex.x += 1.0f;
                                if (face->getType() == FACE_FRONT)
                                    startVertex.y += 0.5f / 3.0f / (float)numY;
    
                                if (face->getType() == FACE_BACK)    
                                    startVertex.x += 1.0f;
                                if (face->getType() == FACE_BACK)
                                    startVertex.y += 0.5f / 3.0f / (float)numY;
    
                                if (face->getType() == FACE_TOP) {
                                    startVertex.y += 1.5f / numY;
                                }
                                if (face->getType() == FACE_BOTTOM) {
                                    startVertex.y += 1.5f / numY;
                                }
                                if (face->getType() == FACE_RIGHT)
                                    startVertex.y -= 1.0f / (float)numY +
                                                     0.5f / (float)numY;
                                if (face->getType() == FACE_LEFT)
                                    startVertex.y -= 4.5f / (float)numY;
                            }
                            Vec3f newVertex = startVertex;

                            newVertex.x -= border4(n, numX);
                            newVertex.y += border1(m, numY);
    
                            storeVertex(vertices, ci, newVertex, 
                                        face->getType(), n, m, numX, numY);
    
                            newVertex = startVertex;
    
                            newVertex.x -= border3(n, numX);
                            newVertex.y += border2(m, numY);
    
                            storeVertex(vertices, ci, newVertex, 
                                        face->getType(), n, m, numX, numY);
    
                            newVertex = startVertex;
    
                            newVertex.x -= border3(n, numX);
                            newVertex.y += border3(m, numY);
    
                            storeVertex(vertices, ci, newVertex, 
                                        face->getType(), n, m, numX, numY);
    
                            newVertex = startVertex;
 
                            newVertex.x -= border4(n, numX);
                            newVertex.y += border4(m, numY);
    
                            storeVertex(vertices, ci, newVertex, 
                                        face->getType(), n, m, numX, numY);

                            ci->appendSFValue(-1);
                        }
                    }
                }
    }
    deleteFaces(ci, &facesToDelete);

    m_scene->backupFieldsStart();
    m_scene->backupFieldsAppend(ncoord, ncoord->point_Field());
    m_scene->backupFieldsAppend(this, color_Field());    
    m_scene->backupFieldsAppend(this, normal_Field());    
    m_scene->backupFieldsAppend(this, texCoord_Field());    
    m_scene->backupFieldsAppend(this, coordIndex_Field());
    m_scene->backupFieldsAppend(this, colorIndex_Field());
    m_scene->backupFieldsAppend(this, normalIndex_Field());
    m_scene->backupFieldsAppend(this, texCoordIndex_Field());
    m_scene->backupFieldsDone();

    ncoord->point(new MFVec3f(*vertices));
    coordIndex(new MFInt32(*ci));
    color(new SFNode(NULL));
    normal(new SFNode(NULL));
    texCoord(new SFNode(NULL));

    m_meshDirty = true;
}

Node *
NodeIndexedFaceSet::simpleJoin(MyArray<FacesetAndMatrix> data)
{
    NodeIndexedFaceSet *ret = NULL;
    MFVec3f *vertices = new MFVec3f();
    MFInt32 *ci = new MFInt32();
    MFColorRGBA *colorRGBA = new MFColorRGBA();
    MFColor *color = new MFColor();
    Scene *scene = NULL;
    if (data.size() > 0) {
        scene = data[0].node->getScene();
        bool x3d = scene->isX3d();
        ret = (NodeIndexedFaceSet *)scene->createNode("IndexedFaceSet");
        ret->coord(new SFNode(scene->createNode("Coordinate")));
        if (x3d)
            ret->color(new SFNode(scene->createNode("ColorRGBA")));
        else
            ret->color(new SFNode(scene->createNode("Color")));
        int numCi = 0;
        for (long i = 0; i < data.size(); i++) {
            NodeIndexedFaceSet *node = data[i].node;
            MyMesh *mesh = node->getMesh();
            MFVec3f *vert = mesh->getVertices();
            MFFloat *colors = mesh->getColors();
            int offset = 3;
            if (mesh->hasColorRGBA())
                offset = 4;
            MFInt32 *coordIndex = mesh->getCoordIndex();            
            for (int j = 0; j < vert->getSFSize(); j++) {
                vertices->appendVec(data[i].matrix * vert->getVec(j));
                if (data[i].material != NULL) {
                    float *c = (float *)
                        data[i].material->diffuseColor()->getValue();
                    if (colors && colors->getSize() > 0) {
                        c[0] = colors->getValue(j * offset);
                        c[1] = colors->getValue(j * offset + 1);
                        c[2] = colors->getValue(j * offset + 2);
                    }    
                    float t = data[i].material->transparency()->getValue();
                    if (colors && (colors->getSize() > 0) && 
                        mesh->hasColorRGBA())
                        t = 1 - colors->getValue(j * offset + 3);
                    if (x3d)
                        colorRGBA->appendSFValue(c[0], c[1], c[2], 1 - t);
                    else
                        color->appendSFValue(c[0], c[1], c[2]);
                }    
            }
            for (int j = 0; j < coordIndex->getSFSize(); j++)
                if (coordIndex->getValue(j) < 0)
                    ci->appendSFValue(coordIndex->getValue(j));
                else
                    ci->appendSFValue(coordIndex->getValue(j) + numCi);
            if (coordIndex->getValue(coordIndex->getSFSize() - 1) > -1) {
                ci->appendSFValue(-1);
                numCi++;
            }
            numCi += vert->getSFSize();
        }
        NodeCoordinate *ncoord = ((NodeCoordinate *)ret->coord()->getValue());
        ncoord->point(new MFVec3f(*vertices));
        if (x3d) {
            NodeColorRGBA *ncolor = ((NodeColorRGBA *)ret->color()->getValue());
            ncolor->color(new MFColorRGBA(*colorRGBA));
        } else {
            NodeColor *ncolor = ((NodeColor *)ret->color()->getValue());
            ncolor->color(new MFColor(*color));
        }
        ret->coordIndex(new MFInt32(*ci));

        return ret;
    }

    return NULL;
}

void
NodeIndexedFaceSet::changeToColorPerVertex(void)
{
    if (m_alreadyInChangeColorPerVertex)
        return;
    m_alreadyInChangeColorPerVertex = true;
    if (hasColorRGBA()) {
        NodeColorRGBA *ncolor = (NodeColorRGBA *)color()->getValue();
        if (ncolor == NULL)
            return;
        MFColorRGBA *colors = ncolor->color(); 
        MFColorRGBA *newColors = new MFColorRGBA();
        if (getMesh() == NULL)
            return;
        for (int i = 0; i < getMesh()->getNumFaces(); i++) {
            FaceData *face = getMesh()->getFace(i);
            int offset = face->getOffset();
            int numVertices = face->getNumVertices();
            float r = 0;
            float g = 0;
            float b = 0;
            float a = 0;
            for (int j = offset; j < offset + numVertices; j++) {
                 int ci = coordIndex()->getValue(j);            
                 r += colors->getValue(ci)[0];
                 g += colors->getValue(ci)[1];
                 b += colors->getValue(ci)[2];
                 a += colors->getValue(ci)[3];
            }
            newColors->appendSFValue(r / numVertices, 
                                     g / numVertices, 
                                     b / numVertices, 
                                     a / numVertices);
        }                
                
        m_scene->backupFieldsStart();
        m_scene->backupFieldsAppend(ncolor, ncolor->color_Field());
        m_scene->backupFieldsAppend(this, color_Field());
        m_scene->backupFieldsAppend(this, colorPerVertex_Field());
        m_scene->backupFieldsDone();

        ncolor->color(newColors);
        color(new SFNode(ncolor, -1));
        colorPerVertex(new SFBool(true));
    } else {
        NodeColor *ncolor = (NodeColor *)color()->getValue();
        if (ncolor == NULL)
            return;
        MFColor *colors = ncolor->color(); 
        MFColor *newColors = new MFColor();
        if (getMesh() == NULL)
            return;
        for (int i = 0; i < getMesh()->getNumFaces(); i++) {
            FaceData *face = getMesh()->getFace(i);
            int offset = face->getOffset();
            int numVertices = face->getNumVertices();
            float r = 0;
            float g = 0;
            float b = 0;
            for (int j = offset; j < offset + numVertices; j++) {
                int ci = coordIndex()->getValue(j);            
                r += colors->getValue(ci)[0];
                g += colors->getValue(ci)[1];
                b += colors->getValue(ci)[2];
            }
            newColors->appendSFValue(r / numVertices, 
                                     g / numVertices, 
                                     b / numVertices);
        }                
                
        m_scene->backupFieldsStart();
        m_scene->backupFieldsAppend(ncolor, ncolor->color_Field());
        m_scene->backupFieldsAppend(this, color_Field());
        m_scene->backupFieldsAppend(this, colorPerVertex_Field());
        m_scene->backupFieldsDone();

        ncolor->color(newColors);
        color(new SFNode(ncolor, -1));
        colorPerVertex(new SFBool(true));
    }
    m_alreadyInChangeColorPerVertex = false;
}

void
NodeIndexedFaceSet::changeToColorPerFace(void)
{
    if (m_alreadyInChangeColorPerVertex)
        return;
    m_alreadyInChangeColorPerVertex = true;
    if (hasColorRGBA()) {
        NodeColorRGBA *ncolor = (NodeColorRGBA *)color()->getValue();
        if (ncolor == NULL)
            return;
        MFColorRGBA *colors = ncolor->color(); 
        MFColorRGBA *newColors = new MFColorRGBA();
        if (getMesh() == NULL)
            return;
        for (int i = 0; i < getMesh()->getNumFaces(); i++) {
            FaceData *face = getMesh()->getFace(i);
            int offset = face->getOffset();
            int numVertices = face->getNumVertices();
            float r = 0.8; 
            float g = 0.8;
            float b = 0.8;
            float a = 1;
            for (int j = offset; j < offset + numVertices; j++) {
                if (i <= colors->getSFSize()) {
                    r = colors->getValue(i)[0];
                    g = colors->getValue(i)[1];
                    b = colors->getValue(i)[2];
                    a = colors->getValue(i)[3];
                }
            }
            newColors->appendSFValue(r, g, b, a);
        }
                
        if (getMesh()->getNumFaces() > 0) {
            m_scene->backupFieldsStart();
            m_scene->backupFieldsAppend(ncolor, ncolor->color_Field());
            m_scene->backupFieldsAppend(this, color_Field());
            m_scene->backupFieldsAppend(this, colorPerVertex_Field());
            m_scene->backupFieldsDone();

            ncolor->color(newColors);
            color(new SFNode(ncolor, -1));
            colorPerVertex(new SFBool(false));
        }
    } else {
        NodeColor *ncolor = (NodeColor *)color()->getValue();
        if (ncolor == NULL)
            return;
        MFColor *colors = ncolor->color(); 
        MFColor *newColors = new MFColor();
        if (getMesh() == NULL)
            return;
        for (int i = 0; i < getMesh()->getNumFaces(); i++) {
            FaceData *face = getMesh()->getFace(i);
            int offset = face->getOffset();
            int numVertices = face->getNumVertices();
            float r = 0.8; 
            float g = 0.8;
            float b = 0.8;
            for (int j = offset; j < offset + numVertices; j++) {
                if (i <= colors->getSFSize()) {
                    r = colors->getValue(i)[0];
                    g = colors->getValue(i)[1];
                    b = colors->getValue(i)[2];
                }
            }
            newColors->appendSFValue(r, g, b);
        }                

        if (getMesh()->getNumFaces() > 0) {
            m_scene->backupFieldsStart();
            m_scene->backupFieldsAppend(ncolor, ncolor->color_Field());
            m_scene->backupFieldsAppend(this, color_Field());
            m_scene->backupFieldsAppend(this, colorPerVertex_Field());
            m_scene->backupFieldsDone();

            ncolor->color(newColors);
            color(new SFNode(ncolor, -1));
            colorPerVertex(new SFBool(false));
      }
    }                
    m_alreadyInChangeColorPerVertex = false;
}
int 
NodeIndexedFaceSet::getProfile(void) const
{ 
    // TODO:
    // X3D each face is terminated with coordIndex = -1, even the last
    // X3D check repeating coordIndex
    
    /* the following criteria are required from each IndexedFaceSet 
       "otherwise the results are undefined" in ISO/IEt 19775:2005 13.3.6
    
       - X3D check planar polygons
       - X3D check 3 non-coincident vertices
       - X3D check self-intersection polygon
      
       In ISO/IEC 19775:2005 table B3 (incerchange profile) this criteria are
       repeated as requirement for the interchange profile, but they are not 
       repeated in ISO/IEc 19775:2005 table E.3 (immersive profile) and
       ISO/IEC 19775:2005 table F.3 (full profile).
       
    conclusion: IndexedFaceSets in X3D files using the full or immersive
    profile may produce undefined results, while in the interchange
    profile it may not produce undefined results 8-(
    
       TODO: find out what is going on 8-(
    */
    MFInt32* coords = coordIndex();          
    if (coords) 
        if (coords->getSize() > 0) {
            if (coords->getValue(coords->getSize() - 1) != -1)
                return PROFILE_IMMERSIVE;
            int vertexCounter = 0;
            for (int i = 0; i < coords->getSize(); i++)
                if (coords->getValue(i) == -1) {
                    if (vertexCounter < 3)
                        return PROFILE_IMMERSIVE;
                    else
                        vertexCounter = 0;
                    vertexCounter++;
                }
        }
    if (m_mesh != NULL)
        if (!(m_mesh->onlyPlanarFaces()))
            return PROFILE_IMMERSIVE;
    if (hasInput("set_colorIndex"))
        return PROFILE_IMMERSIVE;
    if (hasInput("set_normalIndex"))
        return PROFILE_IMMERSIVE;
    if (!isDefault(ccw_Field()))
        return PROFILE_IMMERSIVE;
    if (!isDefault(normal_Field()))
        return PROFILE_IMMERSIVE;
    if (!isDefault(convex_Field()))
        return PROFILE_IMMERSIVE;
    if (!isDefault(normalIndex_Field()))
        return PROFILE_IMMERSIVE;
    float fcreaseAngle = creaseAngle()->getFixedAngle(m_scene->getUnitAngle());
    if ((fcreaseAngle != 0) && (fcreaseAngle != M_PI))
        return PROFILE_IMMERSIVE;         
    return PROFILE_INTERCHANGE; 
}

    
#ifdef HAVE_LIBCGAL
#undef max
#include <CGAL/Simple_cartesian.h>
typedef CGAL::Simple_cartesian<double> Kernel;
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Surface_mesh.h>
#include <CGAL/Polygon_mesh_processing/corefinement.h>
#include <CGAL/Polygon_mesh_processing/self_intersections.h>
#include <CGAL/Polygon_mesh_processing/triangulate_faces.h>
#include <CGAL/boost/graph/dijkstra_shortest_paths.h>
#include <CGAL/exceptions.h>
#include <CGAL/IO/Color.h>
#include <boost/graph/prim_minimum_spanning_tree.hpp>
#include <boost/foreach.hpp>
typedef Kernel::Point_3 Point;
typedef CGAL::Surface_mesh<Point> Surface;
typedef CGAL::Surface_mesh<Point> SurfaceMesh;
typedef CGAL::Color Color;
typedef Surface::Vertex_index vertex_index;
typedef boost::graph_traits<Surface>::vertex_descriptor vertex_descriptor;
typedef boost::graph_traits<Surface>::face_descriptor face_descriptor;
typedef boost::graph_traits<Surface>::halfedge_descriptor halfedge_descriptor;

namespace params = CGAL::Polygon_mesh_processing::parameters;

static void
build_mesh(Surface *meshOut, MyMesh *mesh, Matrix matrix)
{
    MyArray<vertex_index> u;    
    for (int i = 0; i < mesh->getVertices()->getSFSize(); i++) {
        Vec3f v = mesh->getVertices()->getVec(i);
        v = matrix * v;
        u.append(meshOut->add_vertex(Kernel::Point_3(v.x, v.y, v.z)));
    }
    MFInt32 *meshCoords = mesh->getCoordIndex();
    int vert = 0;
    vertex_index v1;
    vertex_index v2;
    vertex_index v3;

    for (int i = 0; i < meshCoords->getSFSize(); i++) {
         int face = meshCoords->getValue(i);
         if (face > -1) {
             switch (vert) {
               case 0:
                 v1 = u[face];
                 break;  
               case 1:
                 v2 = u[face];
                 break;  
               case 2:
                 v3 = u[face];
                 break;
             }
             if (vert++ == 2) {
                 vert = 0;
                 meshOut->add_face(v1, v2, v3);
             }  
         }
    }
};

NodeIndexedFaceSet * 
NodeIndexedFaceSet::csg(NodeIndexedFaceSet *face, int operation, 
                        Matrix matrix1, Matrix matrix2)
{
    try {
        MyMesh *mesh = m_mesh;
        if (mesh->getVertices()->getSFSize() == 0)
            return NULL;
        
        Surface surface1;
        build_mesh(&surface1, mesh, matrix1);
        Surface surface2;
        build_mesh(&surface2, face->getMesh(), matrix2);
        Surface surfaceMesh;

        CGAL::Polygon_mesh_processing::triangulate_faces(surface1);
        CGAL::Polygon_mesh_processing::triangulate_faces(surface2);
    
        CGAL::Polygon_mesh_processing::parameters::all_default();

        switch (operation) {
          case UNION:
            if (!CGAL::Polygon_mesh_processing::corefine_and_compute_union(
                surface1, surface2, surfaceMesh, params::all_default(),
                                                 params::all_default(),
                                                 params::all_default())
               ) return NULL;
            break;
          case INTERSECTION:
            if (!CGAL::Polygon_mesh_processing::corefine_and_compute_intersection(
                surface1, surface2, surfaceMesh, params::all_default(),
                                                 params::all_default(),
                                                 params::all_default())
               ) return NULL;
            break;
          case SUBTRACT:
            if (!CGAL::Polygon_mesh_processing::corefine_and_compute_difference(
                surface1, surface2, surfaceMesh, params::all_default(),
                                                 params::all_default(),
                                                 params::all_default())
               ) return NULL;
            break;
        }
        NodeCoordinate *ncoord = (NodeCoordinate *)
                                 m_scene->createNode("Coordinate");
        NodeIndexedFaceSet *faceSet = (NodeIndexedFaceSet *)
                                       m_scene->createNode("IndexedFaceSet");
        faceSet->coord(new SFNode(ncoord));
    
        MFVec3f *newVertices = new MFVec3f();
        MFInt32 *newCoordIndex = new MFInt32();
    
        BOOST_FOREACH(vertex_descriptor vd, vertices(surfaceMesh)){
            newVertices->appendSFValue(surfaceMesh.point(vd).x(), 
                                       surfaceMesh.point(vd).y(), 
                                       surfaceMesh.point(vd).z()); 
        }
        BOOST_FOREACH(face_descriptor fd, faces(surfaceMesh)) {
            Surface::Halfedge_index hf = surfaceMesh.halfedge(fd);
            BOOST_FOREACH(halfedge_descriptor hi, 
                          halfedges_around_face(hf, surfaceMesh)) {
                Surface::Vertex_index vi = target(hi, surfaceMesh);
                newCoordIndex->appendSFValue((std::size_t)vi);
            }
            newCoordIndex->appendSFValue(-1);
        }
        ncoord->point(new MFVec3f(newVertices));
        faceSet->coordIndex(new MFInt32(newCoordIndex));
        return faceSet;
    } catch(CGAL::Assertion_exception e) {
        char str[256], title[256], message[256];
        swLoadString(IDS_DUNE, title, 255);
        swLoadString(IDS_CGAL_EXCEPTION, str, 255);
        mysnprintf(message, 255, str, e.expression().c_str());
        swMessageBox(TheApp->mainWnd(), message, title, SW_MB_OK, 
                     SW_MB_ERROR);
    }    
    return NULL;
}
#endif

float               
NodeIndexedFaceSet::getOffFactor(int tabCount, MyArray<char *> strings, 
                                 int beginData, int endData)
{
    // check if color data is integer or float, return factor 1.0/255.0 or 1.0  
    for (int i = beginData; i < endData; i++) {
        MyArray<MyString> numbers;
        MyString splitString(strings[i]);
        splitString.split(&numbers, " ");
        for (int j = tabCount; j < numbers.size(); j++)
            if (!numbers[j].isInt())
                return 1.0f; 
    }
    return 1.0f/255.0f;
}


NodeIndexedFaceSet *
NodeIndexedFaceSet::readOff(const char *filename)
{
    int f = open(filename, O_RDONLY);
    if (f == -1)
        return NULL;

    NodeCoordinate *ncoord = (NodeCoordinate *)
                             m_scene->createNode("Coordinate");
    NodeIndexedFaceSet *faceSet = (NodeIndexedFaceSet *)
                                   m_scene->createNode("IndexedFaceSet");
    faceSet->coord(new SFNode(ncoord));

    MFVec3f *newVertices = new MFVec3f();
    MFInt32 *newCoordIndex = new MFInt32();

    bool isColor = false;
    bool isColorRGBA = false;
    MFColor *newColor = new MFColor();
    MFColorRGBA *newColorRGBA = new MFColorRGBA();

    int fileLen = lseek(f, SEEK_SET, SEEK_END);
    lseek(f, SEEK_SET, 0);
   
    char *fileData = new char[fileLen + 2];
    if (read(f, fileData, fileLen) == -1) {
        swDebugf("%s", strerror(errno));
        return NULL;
    }
    fileData[fileLen] = 0;
    fileData[fileLen + 1] = 0;

    MyArray<char *> lines;
    char *newLine = strchr(fileData, '\n');
    if (newLine != NULL) {
        if (*(newLine - 1) == '\r')
            *(newLine - 1) = 0;
        newLine[0] = 0;
        newLine++;
        if (fileData[0] != '#')
            lines.append(fileData);
    }

    while (newLine != NULL) {
        char *oldLine = newLine;
        newLine = strchr(newLine, '\n');
        if (newLine != NULL) {
            if (*(newLine - 1) == '\r')
                *(newLine - 1) = 0;
            newLine[0] = 0;
            newLine++;
            if (oldLine[0] != '#')
                lines.append(oldLine);
        }
    }

    if (lines.size() < 2) {
        char message[256];
        swLoadString(IDS_NOT_ENOUGH_LINES + swGetLang(), message, 255);
        swDebugf(message);
        return NULL;
    }
    
    MyString header = lines[0];
    if ((strcmp(header.getData(), "OFF") != 0) &&
        (strcmp(header.getData(), "COFF") != 0) &&
        (strcmp(header.getData(), "CNOFF") != 0)) {
        char message[256];
        swLoadString(IDS_WRONG_OFF_HEADER + swGetLang(), message, 255);
        swDebugf(message);
    }

    MyString headerData = lines[1];
    MyArray<MyString> intStrings;
    headerData.split(&intStrings, " ");
    if (intStrings.size() != 3) { 
        char message[256];
        swLoadString(IDS_WRONG_OFF_HEADER + swGetLang(), message, 255);
        swDebugf(message);
        return NULL;
    }

    int numVertices        = atoi(intStrings[0]);
    int numFaces           = atoi(intStrings[1]); 
    int numVerticesPerFace = atoi(intStrings[2]);

    float verticesFactor = getOffFactor(3, lines, 2, numVertices + 2);

    for (int i = 0; i < numVertices; i++) {
        MyString dataString = lines[i + 2];
        MyArray<MyString> floatStrings;
        dataString.split(&floatStrings, " ");
        if (floatStrings.size() < 3) { 
            char message[256];
            swLoadString(IDS_WRONG_OFF_DATA + swGetLang(), message, 255);
            swDebugf(message);
            return NULL;
        }
        newVertices->appendSFValue(atof((const char *)floatStrings[0]),
                                   atof((const char *)floatStrings[1]), 
                                   atof((const char *)floatStrings[2]));
        int numCi = 3;
        if ((floatStrings.size() < 3) ||
            ((floatStrings.size() != numCi) && 
            (floatStrings.size() != numCi + 3) &&  // 3 color values
            (floatStrings.size() != numCi + 4)) || // 4 color values
            (isColor && (floatStrings.size() == numCi + 4)) || 
            (isColorRGBA && (floatStrings.size() == numCi + 3))) { 
            char message[256];
            swLoadString(IDS_WRONG_OFF_DATA + swGetLang(), message, 255);
            swDebugf(message);
            return NULL;
        }
        if (floatStrings.size() == numCi + 3) {
            float c[3];
            for (int j = numCi; j < floatStrings.size(); j++) {
                c[j - (numCi)] = atof(floatStrings[j].getData()) * 
                                      verticesFactor;
            }
            newColor->appendSFValue(c[0], c[1], c[2]);
            faceSet->colorPerVertex(new SFBool(true));
            isColor = true;
        } else if (floatStrings.size() == numCi + 4) {
            float c[4];
            for (int j = numCi; j < floatStrings.size(); j++) {
                c[j - (numCi)] = atof(floatStrings[j].getData()) *
                                      verticesFactor;
            }
            isColorRGBA = true;
            faceSet->colorPerVertex(new SFBool(true));
            newColorRGBA->appendSFValue(c[0], c[1], c[2], c[3]);
        }
    }

    float facesFactor = getOffFactor(3, lines, numVertices + 2, 
                                               numVertices + numFaces + 2);

    for (int i = numVertices; i < numVertices + numFaces; i++) {
        MyString dataString = lines[i + 2];
        MyArray<MyString> floatStrings;
        dataString.split(&floatStrings, " ");
        int numCi = atoi((const char *)floatStrings[0]);
        if ((floatStrings.size() < 3) ||
             ((floatStrings.size() != numCi + 1) && 
              (floatStrings.size() != numCi + 1 + 3) &&  // 3 color values
              (floatStrings.size() != numCi + 1 + 4)) || // 4 color values
             (isColor && (floatStrings.size() == numCi + 1 + 4)) || 
             (isColorRGBA && (floatStrings.size() == numCi + 1 + 3))) { 
            char message[256];
            swLoadString(IDS_WRONG_OFF_DATA + swGetLang(), message, 255);
            swDebugf(message);
            return NULL;
        }
        for (int j = 1; j < numCi + 1; j++)
            newCoordIndex->appendSFValue(atoi(floatStrings[j]));
        newCoordIndex->appendSFValue(-1);
        if (floatStrings.size() == numCi + 1 + 3) {
            float c[3];
            for (int j = numCi + 1; j < floatStrings.size(); j++) {
                c[j - (numCi + 1)] = atof(floatStrings[j].getData()) *
                                          facesFactor;
            }
            newColor->appendSFValue(c[0], c[1], c[2]);
            faceSet->colorPerVertex(new SFBool(false));
            isColor = true;
        } else if (floatStrings.size() == numCi + 1 + 4) {
            float c[4];
            for (int j = numCi + 1; j < floatStrings.size(); j++) {
                c[j - (numCi + 1)] = atof(floatStrings[j].getData()) *
                                          facesFactor;
            }
            isColorRGBA = true;
            faceSet->colorPerVertex(new SFBool(false));
            newColorRGBA->appendSFValue(c[0], c[1], c[2], c[3]);
        }
    }

    delete [] fileData;

    ncoord->point(new MFVec3f(newVertices));
    faceSet->coordIndex(new MFInt32(newCoordIndex));
    if (isColor) {
        NodeColor *ncolor = (NodeColor *)m_scene->createNode("Color");
        faceSet->color(new SFNode(ncolor)); 
        ((NodeColor *)faceSet->color()->getValue())->color(
            new MFColor(newColor));
    } else
        delete newColor;
    if (isColorRGBA) {
        NodeColorRGBA *ncolor = (NodeColorRGBA *)
                                m_scene->createNode("ColorRGBA");
        faceSet->color(new SFNode(ncolor)); 
        ((NodeColorRGBA *)faceSet->color()->getValue())->color(
            new MFColorRGBA(newColorRGBA));
    } else
        delete newColorRGBA;
    return faceSet;
}
    
void               
NodeIndexedFaceSet::accountOffData(int f)
{
    MFVec3f *vertices = getCoordinates();
    if (vertices)
        m_sumVertices += vertices->getSFSize();
    m_sumVerticesPerFaces += getMesh()->getNumFaces();
}

void               
NodeIndexedFaceSet::writeOffVerticesAndColors(int f, Node *node)
{
    if (node == NULL)
        return;
    static Matrix transformMatrix = Matrix::identity();
    Path *trans = node->getScene()->searchTransform(node->getPath());
    Node *transform = NULL;
    if (trans)
        transform = trans->getNode();
    if (transform) {
        transform->update();
        transform->transform();
        transform->getMatrix(transformMatrix);
    }

    MFVec3f *vertices = getCoordinates();
    if (vertices)
        for (int i = 0; i < vertices->getSFSize(); i++) {
            Vec3f vec = vertices->getVec(i);
            vec = transformMatrix * vec;
            mywritef(f, "  %f %f %f", vec.x, vec.y, vec.z);
            if (colorPerVertex()->getValue() &&
                (NodeColorRGBA *)color()->getValue() &&
                ((MeshBasedNode *)this)->hasColorRGBA()) {
                NodeColorRGBA *colorRGBANode = (NodeColorRGBA *)
                                               color()->getValue();
                if (color() && colorRGBANode->color()) {
                    const float *c = 
                         ((NodeColorRGBA *)color()->getValue())->color()->
                         getValues() + i * 4;
                     mywritef(f, " %f %f %f %f", c[0], c[1], c[2], c[3]);
                }
            } else if (color() && colorPerVertex()->getValue() &&
                       ((NodeColor *)color()->getValue()) &&
                       ((NodeColor *)color()->getValue())->color()) {
                NodeColor *colorNode = (NodeColor *)color()->getValue();
                const float *c = ((NodeColorRGBA *)
                                 color()->getValue())->color()->
                    getValues() + i * 3;
                 mywritef(f, " %f %f %f", c[0], c[1], c[2]);
            } 
            mywritestr(f, "\n");
        }
}

void               
NodeIndexedFaceSet::writeOffIndicesAndColors(int f, int startIndex, Node *node)
{
    if (node == NULL)
        return;
    MFVec3f *vertices = getCoordinates();
    if (vertices == NULL)
        return;
    for (int i = 0; i < getMesh()->getNumFaces(); i++) {
        FaceData *face = getMesh()->getFace(i);
        int offset = face->getOffset();
        int numVertices = face->getNumVertices();

        mywritef(f, "  %d ", numVertices);
        for (int j = 0; j < numVertices; j++) {
            int ci = coordIndex()->getValue(offset + j);
            mywritef(f, "%d ", ci + startIndex);
        }
        if ((!colorPerVertex()->getValue()) &&
            (NodeColorRGBA *)color()->getValue() &&
            getMesh()->hasColorRGBA()) {
            NodeColorRGBA *colorRGBANode = (NodeColorRGBA *)
                color()->getValue();
            if (color() && colorRGBANode->color()) {
                const float *c = 
                    ((NodeColorRGBA *)color()->getValue())->color()->
                    getValues() + i * 4;
                mywritef(f, " %f %f %f %f", c[0], c[1], c[2], c[3]);
            }
        } else if (color() && (!colorPerVertex()->getValue()) &&
            ((NodeColor *)color()->getValue()) &&
            ((NodeColor *)color()->getValue())->color()) {
            NodeColor *colorNode = (NodeColor *)color()->getValue();
            const float *c = 
                ((NodeColorRGBA *)color()->getValue())->color()->
                getValues() + i * 3;
                mywritef(f, " %f %f %f", c[0], c[1], c[2]);
        } 
        mywritestr(f, "\n");                 
    }
}

void
NodeIndexedFaceSet::writeOffNormals(int f, Node *node)
{
    if (node == NULL)
        return;
    if (normal() == NULL)
        return;

    MFVec3f *vertices = getVertices();
    if (vertices == NULL)
        return;
    MFVec3f *normals = NULL;
    if (normal()->getValue() == NULL) {
        if (getMesh())
            normals = getMesh()->getNormals();
        else
            return;
    } else
        normals = ((NodeNormal *)normal()->getValue())->vector();
    for (int i = 0; i < vertices->getSFSize(); i++) {
        Vec3f vec = normals->getVec(i);
        mywritef(f, "  N %f %f %f\n", vec.x, vec.y, vec.z);
    }
}
    
#ifdef HAVE_LIBVCG
#undef max
#undef min
// stuff to define the mesh
#include <vcg/complex/complex.h>
#include <vcg/complex/algorithms/edge_collapse.h>

using namespace vcg;
using namespace tri;

// local optimization
#include "quadric_simp.h"

typedef typename CMeshO::VertexPointer VertexPointer;
typedef typename CMeshO::VertexIterator VertexIterator;
typedef typename CMeshO::FaceIterator FaceIterator;
typedef typename CMeshO::CoordType CoordType;

static bool QCallBack(const int pos, const char * str)
{
    return true;
}

NodeIndexedFaceSet *
NodeIndexedFaceSet::meshReduce(float percent)
{
    CMeshO m;
    MyMesh *trimesh = triangulateMesh();
    NodeIndexedFaceSet *indexedFaceSet = (NodeIndexedFaceSet *)
                                         m_scene->createNode("IndexedFaceSet");
    NodeCoordinate *ncoordinate = (NodeCoordinate *)
                                  m_scene->createNode("Coordinate");
    indexedFaceSet->coord(new SFNode(ncoordinate));
    ncoordinate->point(trimesh->getVertices());
    indexedFaceSet->coordIndex(trimesh->getCoordIndex());
    indexedFaceSet->optimize();
    MyMesh *mesh = indexedFaceSet->getMesh();
    
    if (mesh->getVertices()->getSFSize() == 0)
        return NULL;
  
    Allocator<CMeshO>::AddVertices(m, mesh->getVertices()->getSFSize());
    VertexPointer *ivp = new VertexPointer[mesh->getVertices()->getSFSize()];

    VertexIterator vi=m.vert.begin();
    for (int i = 0; i < mesh->getVertices()->getSFSize(); i++) {
        const float *v = mesh->getVertices()->getValue(i);
        ivp[i]=&*vi;
        (*vi).P()=CoordType(v[0], v[1], v[2]); 
        vi++;
    }

    MFInt32 *meshCoords = mesh->getCoordIndex();
    for (int i = 0; i < meshCoords->getSFSize(); i += 4) {
         int f1 = meshCoords->getValue(i);
         int f2 = meshCoords->getValue(i + 1);
         int f3 = meshCoords->getValue(i + 2);
         Allocator<CMeshO>::AddFace(m, ivp[f1], ivp[f2], ivp[f3]);
    }

    delete [] ivp; 

    int finalSize = percent * 0.01f * m.fn ;

    TriEdgeCollapseQuadricParameter qparams;
    qparams.QualityThr = 0.3;
    bool cleaningFlag = true;
    qparams.QualityCheck = true;
    qparams.NormalCheck = true;
    qparams.OptimalPlacement = true;
    qparams.ScaleIndependent = true;
    qparams.PreserveBoundary = true;
    qparams.PreserveTopology = true;
    qparams.QualityQuadric = true;
    qparams.QualityWeight = false;
    
    if (cleaningFlag) {
        tri::Clean<CMeshO>::RemoveDuplicateVertex(m);
        tri::Clean<CMeshO>::RemoveUnreferencedVertex(m);
    }

    vcg::tri::UpdateBounding<CMeshO>::Box(m);

//    m.updateDataMask( MeshModel::MM_VERTFACETOPO | MeshModel::MM_VERTMARK);
//    tri::UpdateFlags<CMeshO>::FaceBorderFromVF(m);

    QuadricSimplification(m, finalSize, false, qparams, QCallBack);

    Allocator<CMeshO>::CompactEveryVector(m);

    NodeCoordinate *ncoord = (NodeCoordinate *)
                             m_scene->createNode("Coordinate");
    NodeIndexedFaceSet *faceSet = (NodeIndexedFaceSet *)
                                   m_scene->createNode("IndexedFaceSet");
    faceSet->coord(new SFNode(ncoord));

    MFVec3f *newVertices = new MFVec3f();
    MFInt32 *newCoordIndex = new MFInt32();
    
    vi = m.vert.begin();
    for (int vertex = 0; vertex < m.vn; vertex++) {
        newVertices->appendSFValue((*vi).P().V(0), 
                                   (*vi).P().V(1), 
                                   (*vi).P().V(2));
        vi++;
    }

    FaceIterator fi = m.face.begin();
    for (int face = 0; face < m.fn; face++) {
         int d0 = vcg::tri::Index(m, fi->cV(0));
         newCoordIndex->appendSFValue(d0);
         int d1 = vcg::tri::Index(m, fi->cV(1));
         newCoordIndex->appendSFValue(d1);
         int d2 = vcg::tri::Index(m, fi->cV(2));
         newCoordIndex->appendSFValue(d2);
         newCoordIndex->appendSFValue(-1);
         fi++;
    }

    ncoord->point(new MFVec3f(newVertices));
    faceSet->coordIndex(new MFInt32(newCoordIndex));

    return faceSet;

}
#endif

