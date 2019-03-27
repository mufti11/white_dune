/*
 * FieldCommand.cpp
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

#include "FieldCommand.h"
#include "Scene.h"

#include "Node.h"
#include "FieldValue.h"

FieldCommand::FieldCommand(Node *node, int field, FieldValue *newValue)
{
    m_node = node;
    m_field = field;
    if (newValue) {
        m_newValue = newValue;
        m_newValue->ref();
        node->getScene()->setField(node, field, newValue);
    } else {
        m_newValue = node->getField(field);
        m_newValue->ref();
    }
    m_oldValue = node->getField(field);
    m_oldValue->ref();
}

FieldCommand::~FieldCommand()
{
    m_newValue->unref();
    m_oldValue->unref();
}

void
FieldCommand::execute(SceneView *sender)
{
    m_oldValue->unref();
    m_oldValue = m_node->getField(m_field);
    m_oldValue->ref();
    m_node->setField(m_field, m_newValue);
    m_node->getScene()->OnFieldChange(m_node, m_field);
}

#include "NodeNurbsSurface.h"
#include "MFVec3f.h"

void
FieldCommand::undo()
{
    m_newValue->unref();
    m_newValue = m_node->getField(m_field);
    m_newValue->ref();
    m_node->setField(m_field, m_oldValue);
    m_node->getScene()->OnFieldChange(m_node, m_field);
}
