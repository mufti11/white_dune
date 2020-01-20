/*
 * Vec2f.h
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

#include <math.h>

class Vec2d {
public:
                Vec2d()
                {}
                Vec2d(const Vec2d &v)
                { x = v.x; y = v.y; }
                Vec2d(float x, float y)
                { this->x = x; this->y = y; }
                Vec2d(const float *v)
                { x = v[0]; y = v[1]; }

     int        operator==(const Vec2d &v) const
                { return x == v.x && y == v.y; }
     int        operator!=(const Vec2d &v) const
                { return x != v.x || y != v.y; }
     Vec2d      operator+(const Vec2d &v) const
                { return Vec2d(x + v.x, y + v.y); }
     Vec2d      operator-(const Vec2d &v) const
                { return Vec2d(x - v.x, y - v.y); }
     Vec2d      operator-() const
                { return Vec2d(-x, -y); }
     Vec2d      operator*(float f) const
                { return Vec2d(x * f, y * f); }
     Vec2d      operator/(float f) const
                { return Vec2d(x / f, y / f); }
     Vec2d      operator*(const Vec2d &v) const
                { return Vec2d(x * v.x, y * v.y); }
     void       zero()
                { x = y = 0.0f; }
     void       scale(float s)
                { x *= s; y *= s; }
     float      length() const
                { return (float) sqrt(x * x + y * y); }
     float      dot(const Vec2d &v) const
                { return x * v.x + y * v.y; }
     void       normalize()
                { float len = length();
                if (len != 0.0) { x /= len; y /= len; } }
     Vec2d     &operator+=(const Vec2d &v)
                { x += v.x;  y += v.y; return *this; }

public:
     double      x, y;
};

