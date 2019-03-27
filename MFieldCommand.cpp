/*
 * MFieldCommand.cpp
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

#include "stdafx.h"

#include "MFieldCommand.h"
#include "Scene.h"

#include "Node.h"
#include "FieldValue.h"

MFieldCommand::MFieldCommand(Node *node, int field, int index, 
                             FieldValue *newValue)
{
    m_node = node;
    m_field = field;
    m_index = index;
    if (newValue) {
        m_newValue = newValue;
        m_newValue->ref();
    } else {
        m_newValue = ((MFieldValue *) node->getField(field))->getSFValue(index);
        m_newValue->ref();
    }
    m_oldValue = ((MFieldValue *) node->getField(field))->getSFValue(index);
    m_oldValue->ref();
}

MFieldCommand::~MFieldCommand()
{
    m_newValue->unref();
    m_oldValue->unref();
}

void
MFieldCommand::execute(SceneView* sender)
{
    MFieldValue *value = (MFieldValue *) m_node->getField(m_field);

    m_oldValue->unref();
    m_oldValue = value->getSFValue(m_index);
    m_oldValue->ref();
    value->setSFValue(m_index, m_newValue);
    m_node->getScene()->OnFieldChange(m_node, m_field, m_index);
}

void
MFieldCommand::undo()
{
    MFieldValue *value = (MFieldValue *) m_node->getField(m_field);

    m_newValue->unref();
    m_newValue = value->getSFValue(m_index);
    m_newValue->ref();
    value->setSFValue(m_index, m_oldValue);
    m_node->getScene()->OnFieldChange(m_node, m_field, m_index);
}
