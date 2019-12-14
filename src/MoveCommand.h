/*
 * MoveCommand.h
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

#ifndef _COMMAND_H
#include "Command.h"
#endif

class Node;
class FieldValue;

class MoveCommand : public Command {
public:
                        MoveCommand(Node *node, Node *src, int srcField, 
                                    Node *dest, int destField, 
                                    int destIndex = -1, bool handleUSE = false);
    virtual            ~MoveCommand();

    virtual void        execute(SceneView *sender = NULL);
    virtual void        undo();
    virtual int         getType() { return MOVE_COMMAND; }

protected:
    Node               *m_node;
    Node               *m_src;
    int                 m_srcField;
    Node               *m_dest;
    int                 m_destField;
    FieldValue         *m_oldValueSrc;
    FieldValue         *m_oldValueDest;
    FieldValue         *m_newValueSrc;
    FieldValue         *m_newValueDest;
    bool                m_failed;
    bool                m_isUSE;
};
