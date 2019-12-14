/*
 * Command.h
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

enum {
   FIELD_COMMAND,
   MFIELD_COMMAND,
   MOVE_COMMAND,
   ROUTE_COMMAND,
   UNROUTE_COMMAND,
   SELECTION_COMMAND,
   NEXT_COMMAND,
   COMMAND_LIST
};

class SceneView;

class Command {
public:
    virtual            ~Command() {}

    virtual void        execute(SceneView *sender = NULL) = 0;
    virtual void        undo() = 0;
    virtual int         getType() = 0;
    int                 strange_dummy_for_aix_gcc2_95;
};
