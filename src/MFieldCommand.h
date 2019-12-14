/*
 * MFieldCommand.h
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

#pragma once

#include "Command.h"

class Node;
class FieldValue;
#ifndef NULL
#define NULL 0
#endif

class MFieldCommand : public Command {
public:
                        MFieldCommand(Node *node, int field, int index, 
                                      FieldValue *newValue = 0);
    virtual            ~MFieldCommand();
    virtual void        execute(SceneView *sender = NULL);
    virtual void        undo();
    virtual int         getType() { return MFIELD_COMMAND; }
private:
    Node               *m_node;          // which node to change
    int                 m_field;         // which field within that node
    int                 m_index;         // which element within that field
    FieldValue         *m_oldValue;      // the old and new values of
    FieldValue         *m_newValue;      // the element (in SFValue form)
};

