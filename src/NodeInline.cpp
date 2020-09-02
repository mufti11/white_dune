/*
 * NodeInline.cpp
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

#include "NodeInline.h"
#include "Proto.h"
#include "FieldValue.h"
#include "MFString.h"
#include "SFVec3f.h"
#include "SFFloat.h"
#include "Field.h"  // for FF_URL
#include "Scene.h"
#include "NodeInlineLoadControl.h"
#include "NodeGroup.h"
#include "MoveCommand.h"

ProtoInline::ProtoInline(Scene *scene)
  : WonderlandExportProto(scene, "Inline")
{
    addElements();
}

ProtoInline::ProtoInline(Scene *scene, const char *name)
  : WonderlandExportProto(scene, name)
{
    addElements();
}

void ProtoInline::addElements(void) 
{
// in X3D Inline will be replaced with InlineLoadControl
    load.set(
        addExposedField(SFBOOL, "load", new SFBool(true), new SFBool(true)));
    setFieldFlags(load, FF_X3D_ONLY);
    url.set(
          addExposedField(MFSTRING, "url", new MFString(), FF_URL, NULL));
    bboxCenter.set(
          addField(SFVEC3F, "bboxCenter", new SFVec3f(0.0f, 0.0f, 0.0f)));
    bboxSize.set(
          addField(SFVEC3F, "bboxSize", new SFVec3f(-1.0f, -1.0f, -1.0f), 
                   new SFFloat(-1.0f)));

    children.set(
        addExposedField(MFNODE, "children", new MFNode(), X3DOM_MULTI_PART));
    setFieldFlags(children, FF_X3DOM_ONLY);

    mapDEFToID.set(
        addExposedField(SFBOOL, "mapDEFToID", new SFBool(false)));
    setFieldFlags(mapDEFToID, FF_X3DOM_ONLY);

    nameSpaceName.set(
        addExposedField(MFSTRING, "nameSpaceName", new MFString()));
    setFieldFlags(nameSpaceName, FF_X3DOM_ONLY);

    render.set(
        addExposedField(SFBOOL, "render", new SFBool(true)));
    setFieldFlags(render, FF_X3DOM_ONLY);
}

Node *
ProtoInline::create(Scene *scene)
{
    return new NodeInline(scene, this); 
}

NodeInline::NodeInline(Scene *scene, Proto *def)
  : Node(scene, def)
{
    m_unitLength = 1;
    m_loadedNodes = NULL;
}

NodeInline::NodeInline(NodeInlineLoadControl *inlineLoadControl)
  : Node(inlineLoadControl->getScene(), 
         inlineLoadControl->getScene()->getProto("Inline"))
{
    url(inlineLoadControl->url());
    bboxCenter(inlineLoadControl->bboxCenter());
    bboxSize(inlineLoadControl->bboxSize());
}

void    
NodeInline::addFieldNodeList(int index, NodeList *childList, int cf)
{
    m_loadedNodes = childList;
    if (m_loadedNodes != NULL)
        m_scene->scanForMultimedia(m_loadedNodes);
}

void
NodeInline::draw(int pass)
{
    if (m_loadedNodes == NULL)
        return;

    glPushName(url_Field());  // field offset

    for (long i = 0; i < m_loadedNodes->size(); i++)
        if (m_loadedNodes->get(i))
            m_loadedNodes->get(i)->bind();

    glPushName(0);
    if (m_unitLength > 0)
        glScaled(1 / m_unitLength, 1 / m_unitLength, 1 / m_unitLength);
    for (long i = 0; i < m_loadedNodes->size(); i++) {
        if (m_loadedNodes->get(i)) {
            glLoadName(i);
            m_loadedNodes->get(i)->draw(pass);
        }
    }
    glPopName();

    for (long i = 0; i < m_loadedNodes->size(); i++)
        if (m_loadedNodes->get(i))
            m_loadedNodes->get(i)->unbind();

    glPopName();
}

void
NodeInline::preDraw()
{
    if (m_loadedNodes == NULL)
        return;

    if (m_unitLength > 0)
        glScaled(1 / m_unitLength, 1 / m_unitLength, 1 / m_unitLength);
    for (long i = 0; i < m_loadedNodes->size(); i++)
        if (m_loadedNodes->get(i))
            m_loadedNodes->get(i)->preDraw();
}

void
NodeInline::setField(int index, FieldValue *value, int cf)
{
    Node::setField(index, value, cf);
    if (!m_scene->isParsing())
        if (index == url_Field()) 
            if (TheApp->loadNewInline()) {
                m_scene->readInline(this);
            }
}


int         
NodeInline::countPolygons(void)
{
    if (m_loadedNodes == NULL)
        return 0;

    int ret = 0;

    for (long i = 0; i < m_loadedNodes->size(); i++)
        ret += m_loadedNodes->get(i)->countPolygons();

    return ret;
}

int         
NodeInline::countPrimitives(void)
{
    if (m_loadedNodes == NULL)
        return 0;

    int ret = 0;

    for (long i = 0; i < m_loadedNodes->size(); i++)
        ret += m_loadedNodes->get(i)->countPrimitives();

    return ret;
}

int         
NodeInline::countPolygons1Sided(void)
{
    if (m_loadedNodes == NULL)
        return 0;

    int ret = 0;

    for (long i = 0; i < m_loadedNodes->size(); i++)
        ret += m_loadedNodes->get(i)->countPolygons1Sided();

    return ret;
}

int         
NodeInline::countPolygons2Sided(void)
{
    if (m_loadedNodes == NULL)
        return 0;

    int ret = 0;

    for (long i = 0; i < m_loadedNodes->size(); i++)
        ret += m_loadedNodes->get(i)->countPolygons2Sided();

    return ret;
}

int
NodeInline::getProfile(void) const
{ 
    int profile = PROFILE_INTERACTIVE;
    if (m_loadedNodes == NULL)
        return profile;
    for (long i = 0; i < m_loadedNodes->size(); i++) {
        int nodeProfile = m_loadedNodes->get(i)->getProfile();
        if (nodeProfile > profile)
            profile = nodeProfile;
    }
    return profile; 
}


int
NodeInline::getComponentLevel(void) const
{
    if (m_scene->isX3d())
        if (!isDefault(load_Field()))
            return 3;
    return -1;
}

const char* 
NodeInline::getComponentName(void) const
{
    static const char* name = "Networking";
    return name;
}

bool 
NodeInline::canWriteAc3d()
{ 
    if (m_loadedNodes == NULL)
        return false;
    for (long i = 0; i < m_loadedNodes->size(); i++)
        if (m_loadedNodes->get(i) != NULL)
            if (m_loadedNodes->get(i)->canWriteAc3d())
                return true; 
    return false; 
}

int
NodeInline::writeAc3d(int filedes, int indent)
{
    if (m_loadedNodes == NULL)
        return 0;
    RET_ONERROR( m_loadedNodes->writeAc3d(filedes, indent) )
    return 0;
}

void    
NodeInline::handleAc3dMaterial(ac3dMaterialCallback callback, Scene* scene)
{
    if (m_loadedNodes == NULL)
        return;
    for (long i = 0; i < m_loadedNodes->size(); i++)    
        m_loadedNodes->get(i)->handleAc3dMaterial(callback, scene);
}

bool 
NodeInline::canWriteCattGeo()
{ 
    if (m_loadedNodes == NULL)
        return false;
    for (long i = 0; i < m_loadedNodes->size(); i++)
        if (m_loadedNodes->get(i) != NULL)
            if (m_loadedNodes->get(i)->canWriteCattGeo())
                return true; 
    return false; 
}

int
NodeInline::writeCattGeo(int filedes, int indent)
{
    RET_ONERROR( m_loadedNodes->writeCattGeo(this, filedes, indent) )
    return 0;
}

static bool generateConvertedNodes(Node *node, void *data)
{
    long *writeFlags = (long *)data;               
    if (node != NULL)
        node->addToConvertedNodes(*writeFlags);
    return true;
}

void
NodeInline::addToConvertedNodes(long flags)
{ 
    if (m_convertedNodes.size() > 0)
        return;
    NodeGroup *node = (NodeGroup *)m_scene->createNode("Group");
    node->addParent(getParent(), getParentField());
    if (m_loadedNodes != NULL)
        for (long i = 0; i < m_loadedNodes->size(); i++) {
            Node *newNode = m_loadedNodes->get(i);            
            if (newNode != NULL) {
                if (newNode != this) {
                    Node *copyNode = newNode->copy();
                    copyNode->doWithBranch(generateConvertedNodes, &flags);
                    copyNode->addParent(getParent(), getParentField());
                    MoveCommand *command = new MoveCommand(copyNode, 
                                                 NULL, -1,
                                                 node, node->children_Field());
                    command->execute();
                }
            }
        }
    node->setVariableName(strdup(getVariableName()));
    node->ref();
    m_convertedNodes.append(node);
}

int         
NodeInline::writeXml(int filedes, int indent, int containerField, bool avoidUse)
{ 
#if HAVE_NO_PROTOS_X3DOM
    if ((m_scene->getWriteFlags() & X3DOM) && m_loadedNodes) {
        for (int i = 0; i < m_loadedNodes->size(); i++) 
            RET_ONERROR( (*m_loadedNodes)[i]->writeXml(filedes, indent, 
                                                       containerField, true) )
        return 0;
    }    
#endif
    return Node::writeXml(filedes, indent); 
}

