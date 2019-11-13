/*
 * MeshMorphingNode.cpp
 *
 * Copyright (C) 2005 J. "MUFTI" Scheurich
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

#ifndef FLT_MIN
# include <float.h>
#endif

#include "MeshMorphingNode.h"
#include "MyMesh.h"
#include "Field.h"
#include "Scene.h"
#include "NodeIndexedFaceSet.h"
#include "NodeCoordinateInterpolator.h"
#include "NodeGroup.h"
#include "NodeNormal.h"
#include "NodeNormalInterpolator.h"
#include "EventOut.h"

int             
MeshMorphingNode::write(int f, int indent, bool avoidUse) 
{
    int flags = m_scene->getWriteFlags();
    if ((flags & (PURE_VRML97 | PURE_X3DV | PURE_X3D)) && 
        shouldConvertToIndexedFaceSet()) {
        m_meshDirty = true;
        Node *node = NULL;
        if (m_scene->getWriteKanimNow())
            node = toIndexedFaceSet(TheApp->GetNormalsOnMeshCreation(), false);
        else {
            node = toIndexedFaceSet(TheApp->GetNormalsOnMeshCreation(), true);
            if (!addCoordinateInterpolator(node, true, true)) {
                node = toIndexedFaceSet(TheApp->GetNormalsOnMeshCreation(), 
                                        false);
                addCoordinateInterpolator(node, false, true);
            }
        }
        if (node == NULL)
            return Node::write(f, indent, avoidUse);
        else {
            // use strange DEF name according to a rule for covise/cover
            m_scene->def(m_scene->getUniqueNodeName("Animated_"), node); 
        }
        if (isX3dXml(flags))
            RET_ONERROR( node->writeXml(f, indent, avoidUse) )
        else
            RET_ONERROR( node->write(f, indent, avoidUse) )
        node->unref();
        m_meshDirty = true;
    } else {
        if (isX3dXml(flags))
            RET_ONERROR( MeshBasedNode::writeXml(f, indent, avoidUse) )
        else
            RET_ONERROR( MeshBasedNode::write(f, indent, avoidUse) )
    }
    return 0;
}

void
MeshMorphingNode::addToConvertedNodes(int flags)
{
    if (flags & TRIANGULATE) {
        if (!canSimpleTriangulate() || !hasBranchInputs())
            if (shouldConvertToIndexedTriangleSet() || 
                shouldConvertToTriangleSet()) {
               MeshBasedNode::addToConvertedNodes(flags);
               m_meshDirty = true;
               return;
            }
    }
    if (m_convertedNodes.size() != 0)
        return;
    m_meshDirty = true;

    int meshFlag = (TheApp->GetNormalsOnMeshCreation() ? MESH_WANT_NORMAL : 0) |
                   (flags & TRIANGULATE ? MESH_SIMPLE_TRIANGULATE : 0);
                   
    Node *node = toIndexedFaceSet(meshFlag, true);
    if (!addCoordinateInterpolator(node, true, false)) {
        node = toIndexedFaceSet(meshFlag, false);
        addCoordinateInterpolator(node, false, false);
    }

    node->setVariableName(strdup(getVariableName()));
    m_convertedNodes.append(node);
    node->addParent(getParent(), getParentField());
    m_meshDirty = true;
    return;
}

void 
MeshMorphingNode::findInterpolators(InterpolatorInfo& info)
{
    MyArray<const Node *> nodeList;
    MyArray<int> fieldList;
    for (int i = 0; i < m_proto->getNumFields(); i++) {
        int type = m_proto->getField(i)->getType();
        int animatedField = getAnimatedNodeField(i);
        if ((type == SFNODE) && (animatedField != -1)) {
            nodeList.append(((SFNode *)getField(i))->getValue());
            fieldList.append(animatedField);
        } else {
            if (typeDefaultValue(type)->supportAnimation(m_scene->isX3d())) {
                nodeList.append(this);
                fieldList.append(i);
            }
        }
    }
    
    for (long i = 0; i < fieldList.size(); i++) {
        Interpolator *inter = m_scene->findUpstreamInterpolator(nodeList[i], 
                                                               fieldList[i]);
        if (inter != NULL) {
            info.interpolator.append(inter);
            info.node.append(nodeList[i]);
            info.field.append(fieldList[i]);
        }
    }
}

bool
MeshMorphingNode::addCoordinateInterpolator(Node *node,
                                            bool optimize, 
                                            bool appendToScene)
{
    // find interpolators which have routes to the morphing node
    InterpolatorInfo info;

    findInterpolators(info);

    if (info.interpolator.size() == 0) {
        // nothing to do
        return true;
    }

    // get name of output of used input of interpolators
    // only the first found input is used
    MyString interOutputName;
    MyString interOutputEventName;
    Node *interOutput = NULL;
    int interOutputEvent = -1;
    bool foundInterOutput = false;
    for (long k = 0; k < info.interpolator.size(); k++) {
        Node *node = info.interpolator[k];
        int eventIn = node->lookupEventIn("set_fraction", false);
        if (eventIn == INVALID_INDEX)
            continue;

        // use first eventIn found to drive Interpolator
        SocketList::Iterator *iter= node->getInput(eventIn).first();
        if (iter && iter->item().getNode()) {
            interOutput = iter->item().getNode();
            interOutputName = interOutput->getNameOrNewName();
            interOutputEvent = iter->item().getField();
            interOutputEventName = interOutput->getProto()->
                                       getEventOut(interOutputEvent)->
                                       getName(interOutput->getScene()->isX3d());
            foundInterOutput = true;
            break;
        }
    }
        
    // sort list of keys
    MyArray<float> keys;
    MyArray<float> coordKeyValues;
    MyArray<float> normalKeyValues;
    bool foundMinKey = false;
    float currentKey = -1;
    do {
       foundMinKey = false;
       float minKey = 0;
       for (long i = 0; i < info.interpolator.size(); i++)
           for (int j = 0; j < info.interpolator[i]->getNumKeys(); j++)
               if (info.interpolator[i]->getKey(j) > currentKey) {
                   if (foundMinKey == false) {
                       minKey = info.interpolator[i]->getKey(j);
                       foundMinKey = true;
                   } else if (info.interpolator[i]->getKey(j) < minKey)
                       minKey = info.interpolator[i]->getKey(j);
               }
       if (foundMinKey) {
           keys.append(minKey);
           currentKey = minKey;
       }
       if (!foundMinKey)
           break;
    } while (foundMinKey);


    // get data from Interpolators and generate mesh

    void *data = initializeData();
    if (data == NULL)
        return true;
    for (long i = 0; i < keys.size(); i++) {
        for (long j = 0; j < info.interpolator.size(); j++) {
            Interpolator *inter = info.interpolator[j];
            int field = info.field[j];
            if (inter != NULL)
                loadDataFromInterpolators(data, inter, field, keys[i]);
        }

        createMeshFromData(data, optimize);

        // check if coordIndex of optimized mesh is identical to optimized node
        if (optimize) {
            MFInt32 *micoordIndex = ((NodeIndexedFaceSet *)node)->coordIndex();
            if (!micoordIndex->equals(m_mesh->getCoordIndex()))
                return false;
        }

        MFVec3f *vertices = m_mesh->getVertices();
        for (int k = 0; k < vertices->getSize(); k++)
            coordKeyValues.append(vertices->getValues()[k]);
        if (TheApp->GetNormalsOnMeshCreation()) {
            MFVec3f *normals = getSmoothNormals();
            for (int k = 0; k < normals->getSize(); k++)
                normalKeyValues.append(normals->getValues()[k]);
        }
    }
    finalizeData(data);
    data = NULL;    

    NodeCoordinateInterpolator *coordInter = (NodeCoordinateInterpolator *) 
          m_scene->createNode("CoordinateInterpolator");

    // write CoordinateInterpolator
    MFFloat *mfkeys = new MFFloat((float *)keys.getData(), keys.size());      
    coordInter->key((MFFloat *)mfkeys->copy());

    MFVec3f *mfCoordKeyValues = new MFVec3f((float *)coordKeyValues.getData(),
                                             coordKeyValues.size());
    coordInter->keyValue((MFVec3f *)mfCoordKeyValues->copy());
    if (appendToScene)
        m_scene->addDelayedWriteNode(coordInter); 
    else {
        m_convertedNodes.append(coordInter);
        coordInter->setVariableName(strdup(coordInter->getName()));
        coordInter->addParent(getParent(), getParentField()); 
    }

    // write route
    Node *ncoord = ((NodeIndexedFaceSet *)node)->coord()->getValue();
    ncoord->setVariableName(strdup(ncoord->getVariableName()));
    if (appendToScene)
        m_scene->addRouteString(m_scene->createRouteString(
               coordInter->getNameOrNewName(), "value_changed",
               ncoord->getNameOrNewName(), "set_point"));
    else
        m_scene->addRoute(coordInter, coordInter->value_changed_Field(), ncoord,
                          ((NodeCoordinate *)ncoord)->point_Field());
    if (foundInterOutput) {
        if (appendToScene)
            m_scene->addEndRouteString(m_scene->createRouteString(
                   interOutputName, interOutputEventName,
                   coordInter->getNameOrNewName(), "set_fraction"));
        else
            m_scene->addRoute(interOutput, interOutputEvent,
                             coordInter, coordInter->set_fraction_Field());
    }
    /* X3DOM do not support NormalInterpolator (yet) */
    if ((!(m_scene->getWriteFlags() & X3DOM)) && 
        TheApp->GetNormalsOnMeshCreation()) {
        // write NormalInterpolator

        NodeNormalInterpolator *normalInter = (NodeNormalInterpolator *) 
               m_scene->createNode("NormalInterpolator");
    
        normalInter->key((MFFloat *)mfkeys->copy());

        MFVec3f *mfNormalKeyValues = new MFVec3f((float *)
                                                 normalKeyValues.getData(),
                                                 normalKeyValues.size());
        normalInter->keyValue((MFVec3f *)mfNormalKeyValues->copy());
        if (appendToScene)
            m_scene->addDelayedWriteNode(normalInter); 
        else {
            m_convertedNodes.append(normalInter);
            normalInter->setVariableName(strdup(normalInter->getName()));
            normalInter->addParent(getParent(), getParentField()); 
        } 

        // write Route
        Node *nnormal = ((NodeIndexedFaceSet *)node)->normal()->getValue();
        if (appendToScene)
            m_scene->addRouteString(m_scene->createRouteString(
                                   normalInter->getNameOrNewName(), 
                                   "value_changed", 
                                   nnormal->getNameOrNewName(), 
                                   "set_vector"));
        else 
            m_scene->addRoute(normalInter, normalInter->value_changed_Field(), 
                              nnormal, 
                              ((NodeNormal *)nnormal)->vector_Field());
        if (foundInterOutput) {
            if (appendToScene)
                m_scene->addEndRouteString(m_scene->createRouteString(
                                           interOutputName, interOutputEventName, 
                                           normalInter->getNameOrNewName(), 
                                           "set_fraction"));        
            else 
                m_scene->addRoute(interOutput, interOutputEvent,
                                  normalInter, 
                                  normalInter->set_fraction_Field());
        }   
    }   
    return true;
} 

Vec3f   
MeshMorphingNode::getMinBoundingBox(void)
{
    bool optimize = true;
    // find interpolators which have routes to the morphing node
    InterpolatorInfo info;

    findInterpolators(info);

    // get name of output of used input of interpolators
    // only the first found input is used
    MyString interOutputName;
    MyString interOutputEventName;
    Node *interOutput = NULL;
    int interOutputEvent = -1;
    bool foundInterOutput = false;
    for (long k = 0; k < info.interpolator.size(); k++) {
        Node *node = info.interpolator[k];
        int eventIn = node->lookupEventIn("set_fraction", false);
        if (eventIn == INVALID_INDEX)
            continue;

        // use first eventIn found to drive Interpolator
        SocketList::Iterator *iter= node->getInput(eventIn).first();
        if (iter && iter->item().getNode()) {
            interOutput = iter->item().getNode();
            interOutputName = interOutput->getNameOrNewName();
            interOutputEvent = iter->item().getField();
            interOutputEventName = interOutput->getProto()->
                                       getEventOut(interOutputEvent)->
                                       getName(interOutput->getScene()->isX3d());
            foundInterOutput = true;
            break;
        }
    }
        
    // sort list of keys
    MyArray<float> keys;
    MyArray<float> coordKeyValues;
    MyArray<float> normalKeyValues;
    bool foundMinKey = false;
    float currentKey = -1;
    do {
       foundMinKey = false;
       float minKey = 0;
       for (long i = 0; i < info.interpolator.size(); i++)
           for (int j = 0; j < info.interpolator[i]->getNumKeys(); j++)
               if (info.interpolator[i]->getKey(j) > currentKey) {
                   if (foundMinKey == false) {
                       minKey = info.interpolator[i]->getKey(j);
                       foundMinKey = true;
                   } else if (info.interpolator[i]->getKey(j) < minKey)
                       minKey = info.interpolator[i]->getKey(j);
               }
       if (foundMinKey) {
           keys.append(minKey);
           currentKey = minKey;
       }
       if (!foundMinKey)
           break;
    } while (foundMinKey);


    // get data from Interpolators and generate mesh

    void *data = initializeData();
    Vec3f minResult = m_mesh->getVertices()->getMinBoundingBox();
    if (data == NULL)
        return minResult;
    for (long i = 0; i < keys.size(); i++) {
        for (long j = 0; j < info.interpolator.size(); j++) {
            Interpolator *inter = info.interpolator[j];
            int field = info.field[j];
            if (inter != NULL)
                loadDataFromInterpolators(data, inter, field, keys[i]);
        }

        createMeshFromData(data, optimize);

        Vec3f min = m_mesh->getVertices()->getMinBoundingBox();
        if ((min.x == FLT_MAX) && (min.y == FLT_MAX) && (min.y == FLT_MAX))
            continue;
        if (min.x < minResult.x)
            minResult.x = min.x;
        if (min.y < minResult.y)
            minResult.y = min.y;
        if (min.z < minResult.z)
            minResult.z = min.z;
    }
    finalizeData(data);
    data = NULL;
    return Vec3f(minResult.x, minResult.y, minResult.z);
}


Vec3f   
MeshMorphingNode::getMaxBoundingBox(void)
{
    bool optimize = true;
    // find interpolators which have routes to the morphing node
    InterpolatorInfo info;

    findInterpolators(info);

    // get name of output of used input of interpolators
    // only the first found input is used
    MyString interOutputName;
    MyString interOutputEventName;
    Node *interOutput = NULL;
    int interOutputEvent = -1;
    bool foundInterOutput = false;
    for (long k = 0; k < info.interpolator.size(); k++) {
        Node *node = info.interpolator[k];
        int eventIn = node->lookupEventIn("set_fraction", false);
        if (eventIn == INVALID_INDEX)
            continue;

        // use first eventIn found to drive Interpolator
        SocketList::Iterator *iter= node->getInput(eventIn).first();
        if (iter && iter->item().getNode()) {
            interOutput = iter->item().getNode();
            interOutputName = interOutput->getNameOrNewName();
            interOutputEvent = iter->item().getField();
            interOutputEventName = interOutput->getProto()->
                                       getEventOut(interOutputEvent)->
                                       getName(interOutput->getScene()->isX3d());
            foundInterOutput = true;
            break;
        }
    }
        
    // sort list of keys
    MyArray<float> keys;
    MyArray<float> coordKeyValues;
    MyArray<float> normalKeyValues;
    bool foundMinKey = false;
    float currentKey = -1;
    do {
       foundMinKey = false;
       float minKey = 0;
       for (long i = 0; i < info.interpolator.size(); i++)
           for (int j = 0; j < info.interpolator[i]->getNumKeys(); j++)
               if (info.interpolator[i]->getKey(j) > currentKey) {
                   if (foundMinKey == false) {
                       minKey = info.interpolator[i]->getKey(j);
                       foundMinKey = true;
                   } else if (info.interpolator[i]->getKey(j) < minKey)
                       minKey = info.interpolator[i]->getKey(j);
               }
       if (foundMinKey) {
           keys.append(minKey);
           currentKey = minKey;
       }
       if (!foundMinKey)
           break;
    } while (foundMinKey);


    // get data from Interpolators and generate mesh

    void *data = initializeData();
    Vec3f maxResult = m_mesh->getVertices()->getMaxBoundingBox();
    if (data == NULL)
        return maxResult;
    for (long i = 0; i < keys.size(); i++) {
        for (long j = 0; j < info.interpolator.size(); j++) {
            Interpolator *inter = info.interpolator[j];
            int field = info.field[j];
            if (inter != NULL)
                loadDataFromInterpolators(data, inter, field, keys[i]);
        }

        createMeshFromData(data, optimize);

        Vec3f max = m_mesh->getVertices()->getMaxBoundingBox();
        if ((max.x == FLT_MIN) && (max.y == FLT_MIN) && (max.y == FLT_MIN))
            continue;
        if (max.x > maxResult.x)
            maxResult.x = max.x;
        if (max.y > maxResult.y)
            maxResult.y = max.y;
        if (max.z > maxResult.z)
            maxResult.z = max.z;
    }
    finalizeData(data);
    data = NULL;
    return Vec3f(maxResult.x, maxResult.y, maxResult.z);
}


