/*
 * Quaternion.cpp
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

/****************************************************************************
 code of Quaternion::Quaternion(Vec3f from, Vec3f to) based on quaternion.cpp
 of libQGLViewer-2.2.4-1

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

#include <math.h>
#include "stdafx.h"

#include "Quaternion.h"

Quaternion::Quaternion(const Vec3f &axis, float angle)
{
    float s = (float) sin(angle * 0.5f);

    x = axis.x * s;
    y = axis.y * s;
    z = axis.z * s;
    w = (float) cos(angle * 0.5f);
}

/* 

from The Matrix and Quaternions FAQ

http://skal.planet-d.net/demo/matrixfaq.htm#Q48

Feel free to distribute or copy this FAQ as you please.

Q48. How do I convert a rotation matrix to a quaternion?
--------------------------------------------------------


        |  0  1  2  3 |
        |             |
        |  4  5  6  7 |
    m = |             |
        |  8  9 10 11 |
        |             |
        | 12 13 14 15 |

  A rotation may be converted back to a quaternion through the use of
  the following algorithm:

  The process is performed in the following stages, which are as follows:

    Calculate the trace of the matrix T from the equation:

                2     2     2
      T = 4 - 4x  - 4y  - 4z

                 2    2    2
        = 4( 1 -x  - y  - z )

        = mat[0] + mat[5] + mat[10] + 1


    If the trace of the matrix is greater than zero, then
    perform an "instant" calculation.

      S = 0.5 / sqrt(T)

      W = 0.25 / S

      X = ( mat[9] - mat[6] ) * S

      Y = ( mat[2] - mat[8] ) * S

      Z = ( mat[4] - mat[1] ) * S


    If the trace of the matrix is less than or equal to zero
    then identify which major diagonal element has the greatest
    value.

    Depending on this value, calculate the following:

      Column 0:
        S  = sqrt( 1.0 + mr[0] - mr[5] - mr[10] ) * 2;

        Qx = 0.5 / S;
        Qy = (mr[1] + mr[4] ) / S;
        Qz = (mr[2] + mr[8] ) / S;
        Qw = (mr[6] + mr[9] ) / S;

      Column 1:
        S  = sqrt( 1.0 + mr[5] - mr[0] - mr[10] ) * 2;

        Qx = (mr[1] + mr[4] ) / S;
        Qy = 0.5 / S;
        Qz = (mr[6] + mr[9] ) / S;
        Qw = (mr[2] + mr[8] ) / S;

      Column 2:
        S  = sqrt( 1.0 + mr[10] - mr[0] - mr[5] ) * 2;

        Qx = (mr[2] + mr[8] ) / S;
        Qy = (mr[6] + mr[9] ) / S;
        Qz = 0.5 / S;
        Qw = (mr[1] + mr[4] ) / S;

     The quaternion is then defined as:

       Q = | Qx Qy Qz Qw |

*/

Quaternion::Quaternion(const Matrix &matrix4)
{
    const float* mat=&(*matrix4);
    float trace=mat[0]+mat[5]+mat[10]+1;    
    if (trace>0)
       {
       float s = 0.5 / sqrt(trace);
       x = ( mat[9] - mat[6] ) * s;
       y = ( mat[2] - mat[8] ) * s;
       z = ( mat[4] - mat[1] ) * s;
       w = 0.25 / s;
       }      
    else
       {
       int column_mindiag=0;
       float mindiag=mat[0];
       if (mat[5]<mindiag)
          {
          mindiag=mat[5];
          column_mindiag=1;
          }
       if (mat[10]<mindiag)
          {
          mindiag=mat[10];
          column_mindiag=2;
          }
       switch (column_mindiag)
          {
          float s;
          case 0:
             s  = sqrt( 1.0 + mat[0] - mat[5] - mat[10] ) * 2;
    
             x = 0.5 / s;
             y = (mat[1] + mat[4] ) / s;
             z = (mat[2] + mat[8] ) / s;
             w = (mat[6] + mat[9] ) / s;
             break;
          case 1:
             s  = sqrt( 1.0 + mat[5] - mat[0] - mat[10] ) * 2;

             x = (mat[1] + mat[4] ) / s;
             y = 0.5 / s;
             z = (mat[6] + mat[9] ) / s;
             w = (mat[2] + mat[8] ) / s;
             break;
          case 2:
             s  = sqrt( 1.0 + mat[10] - mat[0] - mat[5] ) * 2;

             x = (mat[2] + mat[8] ) / s;
             y = (mat[6] + mat[9] ) / s;
             z = 0.5 / s;
             w = (mat[1] + mat[4] ) / s;
             break;
          }
       }
}

Quaternion
Quaternion::operator *(const Quaternion &q2) const
{
    const Quaternion   &q1 = *this;
    Quaternion r;

    r.x = q2.w * q1.x + q2.x * q1.w + q2.y * q1.z - q2.z * q1.y;
    r.y = q2.w * q1.y + q2.y * q1.w + q2.z * q1.x - q2.x * q1.z;
    r.z = q2.w * q1.z + q2.z * q1.w + q2.x * q1.y - q2.y * q1.x;
    r.w = q2.w * q1.w - q2.x * q1.x - q2.y * q1.y - q2.z * q1.z;

    return r;
}

Vec3f
Quaternion::operator *(const Vec3f &v) const
{
    Quaternion r(conj() * Quaternion(v.x, v.y, v.z, 0.0f) * *this);

    return Vec3f(r.x, r.y, r.z);
}

float
Quaternion::norm() const
{
    return (float) sqrt(x * x + y * y + z * z + w * w);
}

Quaternion
Quaternion::conj() const
{
    return Quaternion(-x, -y, -z, w);
}

Quaternion
Quaternion::operator *(float f) const
{
    return Quaternion(f * x, f * y, f * z, f * w);
}

void
Quaternion::normalize()
{
    float rlen = norm(); 
    if (rlen > 0.000000005) 
        rlen = 1 / rlen; 
    else 
        rlen = 1;

    x *= rlen;
    y *= rlen;
    z *= rlen;
    w *= rlen;
}

Vec3f operator *(const Vec3f &v,const Quaternion &q)
{ 
    return q * v; 
}

/* Constructs a Quaternion that will rotate from the "from" direction to the 
   "to" direction.

   Note that this rotation is not uniquely defined. The selected axis is 
   usually orthogonal to "from" and "to". 
   However, this method is robust and can handle small or almost identical 
   vectors. 
*/

Quaternion::Quaternion(Vec3f from, Vec3f to)
{
    const float epsilon = 1E-10f;

    const float fromSqNorm = from.squaredNorm();
    const float toSqNorm   = to.squaredNorm();
    // Identity Quaternion when one vector is null
    if ((fromSqNorm < epsilon) || (toSqNorm < epsilon)) {
        x = y = z = 0.0;
        w = 1.0;
    } else {
        Vec3f axis(from.cross(to));
        const float axisSqNorm = axis.squaredNorm();

        // Aligned vectors, pick any axis, not aligned with from or to
        if (axisSqNorm < epsilon)
            axis = from.orthogonalVec();

        double angle = asin(sqrt(axisSqNorm / (fromSqNorm * toSqNorm)));

        if ((from.dot(to)) < 0.0)
            angle = M_PI - angle;
 
       float s = (float) sin(angle * 0.5f);
 
       x = axis.x * s;
       y = axis.y * s;
       z = axis.z * s;
       w = (float) cos(angle * 0.5f);
    }
}
