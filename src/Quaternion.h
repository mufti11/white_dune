/*
 * Quaternion.h
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

#ifndef _QUATERNION_H
#define _QUATERNION_H

#include "Vec3f.h"
#include "Matrix.h"

class Quaternion {
public:
                Quaternion()
                { x = 0.0f; y = 0.0f; z = 0.0f; w = 1.0f; }
                Quaternion(float ix, float iy, float iz, float iw)
                { x = ix; y = iy; z = iz; w = iw; }
                Quaternion(const Vec3f &axis, float angle);
                Quaternion(const Matrix &matrix4D);
                Quaternion(Vec3f from, Vec3f to);

    float       norm() const;
    void        normalize();
    Quaternion  conj() const;
    Quaternion  operator *(float f) const;
    Quaternion  operator *(const Quaternion &q2) const;
    Vec3f       operator *(const Vec3f &v) const;

public:
    float       x, y, z, w;
};

Vec3f      operator *(const Vec3f &v,const Quaternion &q);

inline void printf(Quaternion quat) { printf("%f %f %f %f\n", 
                                             quat.x, quat.y, quat.z, quat.w); }

#endif // _QUATERNION_H
