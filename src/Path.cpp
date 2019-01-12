/*
 * Path.cpp
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

#include "Path.h"
#include "Scene.h"
#include "SFNode.h"
#include "MFNode.h"
#include "Proto.h"
#include "Field.h"
#include "swDebugf.h"

Path::Path(const int *path, int len, Scene *scene, bool protoInTree,
           bool ignoreStrangePath)
{
    m_needExtraUpdateSelection = false;
    Node *node, *parent = NULL;
    int field = -1, parentField = -1;

    m_field = -1;
    m_path = NULL;
    m_len = len;
    m_protoInTree = protoInTree;
    m_ignoreStrangePath = ignoreStrangePath;

    node = scene->getRoot();
    parent = node;

    m_node = node;

#ifdef DEBUG
    swDebugf("Path List: ");
    for (int i = 0; i < len; i++)
        swDebugf("%d ", path[i]);
    swDebugf("\n");
#endif

    if (len > 0) {
        m_path = new int[m_len];
        memcpy(m_path, path, m_len * sizeof(int));

        int start = 0;
        if ((m_len >= 3) && (path[0] < 0)) {
            start = 4;
            Proto *proto = scene->getProto(-path[0] - 1); 
            if (proto != NULL) {
                if (path[1] != -1)
                    node = proto->getNode(path[1]);
                else
                    node = NULL;
            }
            if (node == NULL)
                start = m_len;
        }
        for (int i = start; i < m_len;) {
            field = path[i++];
            // shorten path for inlined nodes
            if (node->getType() == VRML_INLINE) {
                 m_len = i - 1;
                 m_field = field;
                 break;
            }
            if (i < m_len) {
                if (path[i] == -1) {
                    m_len = i;
                    break;
                }
            } else {
                m_field = field;
                break;
            }
            Node *newNode = getNextNode(node, field, i++);
            if (newNode != NULL) {
                parent = node;
                parentField = field;
                node = newNode;
                m_nodes.append(node);
            } else
                break;
        }
    }
    m_node = node;
    m_parent = parent;
    m_parentField = parentField;

//#ifdef DEBUG
//    dump();
//#endif
}

Path::~Path()
{
    delete [] m_path;
    m_path = NULL;
}

Node *
Path::getNextNode(Node *n, int f, int i) const
{
    Node* node = n;
    int field = f;

    Node* ret = NULL;

    if (m_protoInTree && node->isPROTO()) {
        node = node->getProto()->getNode(0);
    }

    FieldValue *value = NULL;
    if (field < 0)
        return NULL;
    if (node->getScene() && (node != node->getScene()->getRoot()))
        if (node->getNumParents() == 0)
            return NULL;
    if (node->getProto()->getField(field) == NULL)
        return NULL;
    value = node->getField(field);
    if (value == NULL)
        return NULL;
    if (value->getType() == SFNODE)
        ret = ((SFNode *) value)->getValue();
    else if ((value->getType() == MFNODE) && 
             (m_path[i] < ((MFNode *)value)->getSize()))
        ret = ((MFNode *) value)->getValue(m_path[i]);
    else {
        if (i != (m_len - 1)) {
            if (!m_ignoreStrangePath)
                printStrangePath(node, field, i); 
            return NULL;
        }
   }
    return ret;
}

Proto *
Path::getProto(Scene *scene) const
{
    if ((m_len > 1) && m_path && (m_path[0] < 0))
        return scene->getProto(-m_path[0] - 1);
    return NULL;
}

void
Path::printStrangePath(Node *node, int field, int depth) const
{
    Field *protoField = node->getProto()->getField(field);
    FieldValue *value = node->getField(field);

    swDebugf("\nstrange type in path: %s %s.%s field %d depth %d\n",
             typeEnumToString(value->getType()), 
             (const char *)node->getProto()->getName(true), 
             (const char *)protoField->getName(true), field, depth);
}

#include "swt.h"

void
Path::dump() const
{
    if (m_node == NULL)
        return;
    Scene *scene = m_node->getScene();

    int len = m_len;

    swDebugf("scene");
    Node *node = scene->getRoot();
    int start = 0;
    if ((m_len > 1) && (m_path[0] < 0))
        start = 2;
    if ((m_len > 3) && (m_path[0] < 0)) {
        start = 4;
        Proto *proto = scene->getProto(-m_path[0] - 1); 
        swDebugf(".%s", (const char *) proto->getName(false));
        if (proto != NULL)
            node = proto->getNode(m_path[1]);
        if (node->getName().length() > 0)
            swDebugf(".%s", (const char *) node->getName());
        else
            swDebugf(".%s", (const char *) node->getProto()->getName(false));
    }
    for (int i = start; i < len;) {
        if (m_protoInTree && node->isPROTO()) {
           node = node->getProto()->getNode(0);
        }
        int field = m_path[i++];
        swDebugf(".%s", (const char *) 
                 node->getProto()->getField(field)->getName(false));
        FieldValue *value = node->getField(field);
        if (i >= len)
            break;
        if (value == NULL)
            break;
        if (value->getType() == SFNODE) {
            node = ((SFNode *) value)->getValue(); 
            i++;
        } else if (value->getType() == MFNODE) {
            node = ((MFNode *) value)->getValue(m_path[i++]);
        } else {
            if (!m_ignoreStrangePath)
                printStrangePath(node, field, i); 
            break;
        }

        if (i == (len - 1))
            break;
        if (node) { 
            if (node->getName().length() > 0)
                swDebugf(".%s", (const char *) node->getName());
            else
                swDebugf(".%s", 
                         (const char *) node->getProto()->getName(false));
        } else
           break;
    }
    swDebugf("\n");
}

int *
Path::copyPathInt(void)
{
    int *newPath = new int[m_len];
    for (int i = 0; i < m_len; i++)
        newPath[i] = m_path[i];
    return newPath;
}
