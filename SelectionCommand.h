/*
 * SelectionCommand.h
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

#ifndef _SELECTION_COMMAND_H
#define _SELECTION_COMMAND_H

#ifndef _COMMAND_H
#include "Command.h"
#endif

class Scene;
class Path;

class SelectionCommand : public Command
{
public:
                        SelectionCommand(Scene *scene, Path *newPath);
                       ~SelectionCommand();

    virtual void        execute();
    virtual void        undo();
    virtual int         getType() { return SELECTION_COMMAND; }

private:
    Scene              *m_scene;
    Path               *m_oldPath;
    Path               *m_newPath;
};

#endif // _SELECTION_COMMAND_H
