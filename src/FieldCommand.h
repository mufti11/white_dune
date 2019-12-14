/*
 * FieldCommand.h
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

class FieldCommand : public Command {
public:
                        FieldCommand(Node *node, int field,
                                     FieldValue *newValue = NULL);
    virtual           ~ FieldCommand();
    virtual void        execute(SceneView *sender = NULL);
    virtual void        undo();
    virtual int         getType() { return FIELD_COMMAND; }
    Node               *getNode() { return m_node; }
    int                 getField() { return m_field; }
private:
    Node               *m_node;
    int                 m_field;
    FieldValue         *m_oldValue;
    FieldValue         *m_newValue;
};

