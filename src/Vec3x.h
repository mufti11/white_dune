/*
 * Vec3x.h
 *
 * Copyright (C) 1999 Stephen F. White, 2006 J. "MUFTI" Scheurich
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


#ifndef _VEC3X_H
#define _VEC3X_H

#include <math.h>
#include "stdafx.h"

template<class T> class Vec3x {
public:
                Vec3x()
                   { x = 0.0f; y = 0.0f; z = 0.0f; }
                Vec3x(const Vec3x &v)
                   { x = v.x; y = v.y; z = v.z; }
                Vec3x(T nx, T ny, T nz)
                   { x = nx; y = ny; z = nz; }
                Vec3x(const T *v)
                   { if (v) { x = v[0]; y = v[1]; z = v[2];} }

     int        operator==(const Vec3x &v) const
                   { return x == v.x && y == v.y && z == v.z; }
     int        operator!=(const Vec3x &v) const
                   { return x != v.x || y != v.y || z != v.z; }
     Vec3x      operator+(const Vec3x &v) const
                   { return Vec3x(x + v.x, y + v.y, z + v.z); }
     Vec3x      operator-(const Vec3x &v) const
                   { return Vec3x(x - v.x, y - v.y, z - v.z); }
     Vec3x      operator-() const
                   { return Vec3x(-x, -y, -z); }
     Vec3x      operator*(T f) const
                   { return Vec3x(x * f, y * f, z * f); }
     Vec3x      operator/(T f) const
                   { return Vec3x(x / f, y / f, z / f); }
     Vec3x      operator*(const Vec3x &v) const
                   { return Vec3x(x * v.x, y * v.y, z * v.z); }
     T         &operator[](int i) 
                   { 
                   switch(i) {
                      case 0: return x;
                      case 1: return y;
                      case 2: return z;
                      default: assert(0);
                   }
                   // never reached ...
                   return z;
                   }
     void       zero()
                   { x = y = z = 0.0f; }
     bool       isZero()
                   { return (x == 0.0f) && (y == 0.0f) && (z == 0.0f); }
     void       scale(T s)
                   { x *= s; y *= s; z *= s; }
     T          length() const
                   { return (T) sqrt(x * x + y * y + z * z); }
     T          squaredNorm() const
                   { return x * x + y * y + z * z; }
     Vec3x      cross(const Vec3x &v) const
                   { return Vec3x(y * v.z - z * v.y,
                                  z * v.x - x * v.z,
                                  x * v.y - y * v.x); }
     T          dot(const Vec3x &v) const
                   { return x * v.x + y * v.y + z * v.z; }
     void       normalize()
                   { 
                   T len = length();
                   if (len != 0.0f) { x /= len; y /= len; z /= len; } 
                   }
     Vec3x     &operator+=(const Vec3x &v)
                   { x += v.x;  y += v.y; z += v.z; return *this; }
     Vec3x     &operator/=(const float f)
                   { x /= f;  y /= f; z /= f; return *this; }
     bool       validTriangle(Vec3x &v1, Vec3x &v2)
                   {
                   Vec3x v(v1 - v2);
                   if (sqrt(v.x * v.x + v.y * v.y + v.z * v.z) < FEPSILON) 
                       return false; 
                   if (EQUALF(x, v1.x) && EQUALF(y, v1.y) &&  EQUALF(z, v1.z))
                       return false; 
                   if (EQUALF(x, v2.x) &&  EQUALF(y, v2.y) &&  EQUALF(z, v2.z))
                       return false; 
                   return true;
                   }
     /* 
        Returns a Vec orthogonal to the Vec. 
        Its norm() depends on the Vec, but is zero only for a null Vec. 
        Note that the function that associates an orthogonalVec() to a Vec is
        not continous. 
      */
     Vec3x      orthogonalVec() const
                   {
                   // Find smallest component. Keep equal case for null values.
                   if ((fabs(y) >= 0.9 * fabs(x)) && (fabs(z) >= 0.9 * fabs(x)))
                       return Vec3x(0.0, -z, y);
                   else
                       if ((fabs(x) >= 0.9 * fabs(y)) && 
                           (fabs(z) >= 0.9 * fabs(y)))
                          return Vec3x(-z, 0.0, x);
                       else
                          return Vec3x(-y, x, 0.0);
                   }

public:
     T x, y, z;
};

typedef Vec3x<double> Vec3d;
typedef Vec3x<float> Vec3f;

Vec3d operator+(const Vec3d &v, Vec3f vec);
Vec3d operator-(const Vec3d &v, Vec3f vec);


#endif // _VEC3X_H
