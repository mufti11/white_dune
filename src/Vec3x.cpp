/*
 * Vec3x.cpp
 *
 * Copyright (C) 1999 Stephen F. White, 2006,2019 J. "MUFTI" Scheurich
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

/****************************************************************************
 code of Vec3x::orthogonalVec() based on vec.cpp of libQGLViewer-2.2.4-1

 Copyright (C) 2002-2006 Gilles Debunne (Gilles.Debunne@imag.fr)

 This function is part of the QGLViewer library.
 Version 2.2.4-1, released on December 12, 2006.

 http://artis.imag.fr/Members/Gilles.Debunne/QGLViewer

 libQGLViewer is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.

 libQGLViewer is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with libQGLViewer; if not, write to the Free Software
 Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

*****************************************************************************/


#include "Vec3d.h"
#include "Vec3f.h"

#include <math.h>
#include "stdafx.h"

Vec3d 
operator+(const Vec3d &v, Vec3f vec)
{
    return Vec3d(v.x + vec.x, v.y + vec.y, v.z + vec.z);
}

Vec3d 
operator-(const Vec3d &v, Vec3f vec)
{
    return Vec3d(v.x - vec.x, v.y - vec.y, v.z - vec.z);
}


