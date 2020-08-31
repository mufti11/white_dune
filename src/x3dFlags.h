/*
 * x3dFlags.h
 *
 * Copyright (C) 2009 J. "MUFTI" Scheurich
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

#include "WriteFlags.h"

inline bool isX3dv(const long flags)   
    { return flags & (X3DV | PURE_X3DV); }
inline bool isX3dXml(const long flags) 
    { return flags & (X3D_XML | X3DOM); }
inline bool isX3d(const long flags)   
    { return isX3dv(flags) || isX3dXml(flags); }

