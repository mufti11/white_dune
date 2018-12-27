/*
 * NodeFlags.h
 *
 * Copyright (C) 2016 Stephen F. White, J. Scheurich
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

#ifndef _NODE_FLAGS_H
#define _NODE_FLAGS_H

enum flags {
    NODE_FLAG_TOUCHED,
    NODE_FLAG_COLLAPSED,
    NODE_FLAG_DEFED,
    NODE_FLAG_CONVERTED,
    NODE_FLAG_FLIPPED,
    NODE_FLAG_SWAPPED,
    NODE_FLAG_X3D_ONLY,
    NODE_FLAG_VRML_ONLY
};

#endif

