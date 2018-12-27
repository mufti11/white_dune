/*
 * CommandList.h
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

#ifndef _COMMAND_LIST_H
#define _COMMAND_LIST_H

#ifndef _COMMAND_H
#include "Command.h"
#endif
#ifndef _LIST_H
#include "List.h"
#endif

class CommandList : public Command {
public:
    virtual            ~CommandList();

    virtual void        execute(SceneView *sender = NULL);
    virtual void        undo();
    virtual int         getType() { return COMMAND_LIST; }

    void                append(Command *cmd);

private:
    List<Command *>     m_children;
};

#endif // _COMMAND_LIST_H
