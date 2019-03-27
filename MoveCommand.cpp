/*
 * MoveCommand.cpp
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

#include "MoveCommand.h"
#include "Scene.h"
#include "Node.h"
#include "FieldValue.h"
#include "SFNode.h"
#include "MFNode.h"
#include "Path.h"
#include "DuneApp.h"

MoveCommand::MoveCommand(Node *node, Node *src, int srcField, 
                         Node *dest, int destField, int destIndex)
{
    m_node = node;

    m_src = src;
    m_srcField = srcField;
    m_dest = dest;
    m_destField = destField;
    m_failed = false;

    if (m_src) {
        m_oldValueSrc = m_src->getField(m_srcField);
        m_oldValueSrc->ref();
        if ((m_oldValueSrc->getType() == SFNODE) ||
            (m_oldValueSrc->getType() == MFNODE))
            m_newValueSrc = m_oldValueSrc->removeNode(node);
        else
            m_newValueSrc = NULL;
        if (m_newValueSrc == NULL) {
            m_failed = true;
            return;
        }
        m_newValueSrc->ref();
    } else {
        m_oldValueSrc = m_newValueSrc = NULL;
    }

    if (m_dest) {
        m_oldValueDest = m_dest->getField(m_destField);
        m_oldValueDest->ref();
        m_newValueDest = m_oldValueDest->addNode(node, destIndex);
        m_newValueDest->ref();
    } else {
        m_oldValueDest = m_newValueDest = NULL;
    }
}

MoveCommand::~MoveCommand()
{
    if (m_src) {
        m_oldValueSrc->unref();
        m_newValueSrc->unref();
    }
    if (m_dest) {
        m_oldValueDest->unref();
        m_newValueDest->unref();
    }
}

void
MoveCommand::execute(SceneView* sender)
{
    if (m_failed)
        return;
    if (m_node == NULL)
        return;
    const Path *selection = m_node->getScene()->getSelection();
    if (selection && (m_node == selection->getNode())) {
        if ((m_dest == NULL) && m_node->hasParent()) {
            m_node->getScene()->setSelection(m_node->getParent());
            TheApp->removeClipboardNode(m_node);
        } else
            m_node->getScene()->setSelection(m_node);
        m_node->getScene()->UpdateViews(sender, UPDATE_SELECTION);
    }
    if (m_src) {
        m_src->setField(m_srcField, m_newValueSrc);
        MyString nodeName = "";
        if (m_node->hasName())
            nodeName += m_node->getName();
        m_node->getScene()->OnRemoveNode(m_node, m_src, m_srcField);
        m_node->getScene()->def(nodeName, m_node);
    }
    if (m_dest) {
        MyString nodeName = "";
        if (m_node->hasName())
            nodeName += m_node->getName();
        m_dest->setField(m_destField, m_newValueDest);
        m_node->getScene()->def(nodeName, m_node);
        m_node->setOutsideProto(m_dest->getOutsideProto());
        m_node->getScene()->getNodes()->clearFlag(NODE_FLAG_TOUCHED);
        m_node->getScene()->OnAddNode(m_node, m_dest, m_destField);
    } 
}

void
MoveCommand::undo()
{
    if (m_node == NULL)
        return;
    m_node->getScene()->setSelection(m_node);
    m_node->getScene()->UpdateViews(NULL, UPDATE_SELECTION);
    MyString nodeName = "";
    if (m_node->hasName())
        nodeName += m_node->getName();
    if (m_dest) {
        m_dest->setField(m_destField, m_oldValueDest);
        m_node->getScene()->def(nodeName, m_node);
        m_node->getScene()->OnRemoveNode(m_node, m_dest, m_destField);
    }
    if (m_src) {
        m_src->setField(m_srcField, m_oldValueSrc);
        m_node->getScene()->def(nodeName, m_node);
        m_node->getScene()->OnAddNode(m_node, m_src, m_srcField);
    } 
}
