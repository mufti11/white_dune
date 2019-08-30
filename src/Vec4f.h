/*
 * Vec4f.h
 *
 * Copyright (C) 2003 Th. Rothermel
 *
 * derived from Vec3f.h Copyright (C) 1999 Stephen F. White
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

#ifndef _VEC4F_H
#define _VEC4F_H

#include <math.h>
#include "stdafx.h"

class Vec4f {
public:
                Vec4f()
                { x = 0.0f; y = 0.0f; z = 0.0f; w = 0.0f; }
                Vec4f(const Vec4f &v)
                { x = v.x; y = v.y; z = v.z; w = v.w; }
                Vec4f(float nx, float ny, float nz, float nw)
                { x = nx; y = ny; z = nz; w = nw; }
                Vec4f(const float *v)
                { x = v[0]; y = v[1]; z = v[2]; w = v[3]; }

     int        operator==(const Vec4f &v) const
                { return x == v.x && y == v.y && z == v.z && w == v.w; }
     int        operator!=(const Vec4f &v) const
                { return x != v.x || y != v.y || z != v.z || w != v.w; }
     Vec4f      operator+(const Vec4f &v) const
                { return Vec4f(x + v.x, y + v.y, z + v.z, w + v.w); }
     Vec4f      operator-(const Vec4f &v) const
                { return Vec4f(x - v.x, y - v.y, z - v.z, w - v.w); }
     Vec4f      operator-() const
                { return Vec4f(-x, -y, -z, -w); }
     Vec4f      operator*(float f) const
                { return Vec4f(x * f, y * f, z * f, w * f); }
     Vec4f      operator/(float f) const
                { return Vec4f(x / f, y / f, z / f, w / f); }
     Vec4f      operator*(const Vec4f &v) const
                { return Vec4f(x * v.x, y * v.y, z * v.z, w * v.w); }
     float     &operator[](int i) { 
                switch(i) {
                   case 0: return x;
                   case 1: return y;
                   case 2: return z;
                   case 3: return w;
                   default: assert(0);
                   }
                   assert(0);
                   static float f = 0.0f;
                   return f;
                }
     void       zero()
                { x = y = z = w = 0.0f; }
     void       scale(float s)
                { x *= s; y *= s; z *= s; w *= s; }
     float      length() const
                { return (float) sqrt(x * x + y * y + z * z + w * w); }
     float      dot(const Vec4f &v) const
                { return x * v.x + y * v.y + z * v.z + w * v.w; }
     void       normalize()
                { float len = length();
                  if (len != 0.0f) { x /= len; y /= len; z /= len; w /= len; } }
     Vec4f     &operator+=(const Vec4f &v)
                { x += v.x;  y += v.y; z += v.z; w += v.w; return *this; }

public:
     float x, y, z, w;
};

#endif // _VEC4F_H
