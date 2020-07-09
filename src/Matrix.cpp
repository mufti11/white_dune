/*
 * Matrix.cpp
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

#include <stdio.h>
#include <string.h>
#include "stdafx.h"
#include "swt.h"

#include "Matrix.h"
#include "EulerAngles.h"

Matrix::Matrix()
{
    for (int i = 0; i < 4; i++)
       for (int k = 0; k < 4; k++)
          m_mat[i][k] = 0.0f;
}

Matrix::Matrix(float mat[4][4])
{
    for (int i = 0; i < 4; i++)
       for (int k = 0; k < 4; k++)
          m_mat[i][k] = mat[i][k];
}

Matrix::Matrix(Quaternion quat)
{
    HMatrix matrix;
    EulerAngles euler = Eul_FromQuat(quat, EulOrdXYZs);
    Eul_ToHMatrix(euler, matrix);
    this->getValue(matrix);
}

Matrix::Matrix(float *f)
{
    for (int i = 0; i < 4; i++)
       for (int k = 0; k < 4; k++)
          m_mat[i][k] = f[i * 4 + k];
}

Matrix
Matrix::operator *(const Matrix &m) const
{
    Matrix r;

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            r.m_mat[i][j] = 0.0f;
            for (int k = 0; k < 4; k++) {
                r.m_mat[i][j] += m_mat[k][j] * m.m_mat[i][k];
            }
        }
    }
    return r;
}

Vec3f
Matrix::operator *(const Vec3f &v) const
{
    Vec3f r;
    float iw;

    r.x = m_mat[0][0]*v.x + m_mat[1][0]*v.y + m_mat[2][0]*v.z + m_mat[3][0];
    r.y = m_mat[0][1]*v.x + m_mat[1][1]*v.y + m_mat[2][1]*v.z + m_mat[3][1];
    r.z = m_mat[0][2]*v.x + m_mat[1][2]*v.y + m_mat[2][2]*v.z + m_mat[3][2];
    iw = 1.0f / (m_mat[0][3]*v.x + m_mat[1][3]*v.y + m_mat[2][3]*v.z + m_mat[3][3]);
    
    return r * iw;
}

Vec3d
Matrix::operator *(const Vec3d &v) const
{
    Vec3d r;
    float iw;

    r.x = m_mat[0][0]*v.x + m_mat[1][0]*v.y + m_mat[2][0]*v.z + m_mat[3][0];
    r.y = m_mat[0][1]*v.x + m_mat[1][1]*v.y + m_mat[2][1]*v.z + m_mat[3][1];
    r.z = m_mat[0][2]*v.x + m_mat[1][2]*v.y + m_mat[2][2]*v.z + m_mat[3][2];
    iw = 1.0f / (m_mat[0][3]*v.x + m_mat[1][3]*v.y + m_mat[2][3]*v.z + m_mat[3][3]);
    
    return r * iw;
}

void
Matrix::getValue(float mat[4][4]) const
{
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            mat[i][j] = m_mat[i][j];
        }
    }
}

void 
Matrix::copyTo(Matrix &m)
{
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            m.m_mat[i][j] = m_mat[i][j];
        }
    }
}

void
Matrix::swapRows(int i, int j)
{
    float tmp;

    for (int k = 0; k < 4; k++) {
        tmp = m_mat[k][i];
        m_mat[k][i] = m_mat[k][j];
        m_mat[k][j] = tmp;
    }
}

Matrix
Matrix::invert() const        // Gauss-Jordan elimination with partial pivoting
{
    Matrix a(*this),          // As a evolves from original mat into identity
           b(identity());     // b evolves from identity into inverse(a)
    
    int  i, j, i1, k;

    // Loop over cols of a from left to right, eliminating above and below diag
    for (j = 0; j < 4; j++) {   
        // Find largest pivot in column j among rows j..2
        i1 = j;                 // Row with largest pivot candidate
        for ( i = j + 1; i < 4; i++ )
            if ( fabs( a.m_mat[j][i] ) > fabs( a.m_mat[j][i1] ) )
                i1 = i;

        // Swap rows i1 and j in a and b to put pivot on diagonal
        if (i1 != j) {
            a.swapRows(i1, j);
            b.swapRows(i1, j);
        }

        // Scale row j to have a unit diagonal
        if (a.m_mat[j][j] == 0.0f) {
            swDebugf("Matrix::invert():  singular matrix, can't invert\n");
            return b;
        }

        float s = 1.0f / a.m_mat[j][j];

        for (i = 0; i < 4; i++) {
            b.m_mat[i][j] *= s;
            a.m_mat[i][j] *= s;
        }

        // Eliminate off-diagonal elems in col j of a, doing identical ops to b
        for (i = 0; i < 4; i++) {
            if (i != j) {
                float t = a.m_mat[j][i];
                for (k = 0; k < 4; k++) {
                    b.m_mat[k][i] -= t * b.m_mat[k][j];
                    a.m_mat[k][i] -= t * a.m_mat[k][j];
                }
            }
        }
    }
    return b;
}

Matrix
Matrix::identity()
{
    Matrix r;

    r.m_mat[0][1] = r.m_mat[0][2] = r.m_mat[0][3] = 0.0f;
    r.m_mat[1][0] = r.m_mat[1][2] = r.m_mat[1][3] = 0.0f;
    r.m_mat[2][0] = r.m_mat[2][1] = r.m_mat[2][3] = 0.0f;
    r.m_mat[3][0] = r.m_mat[3][1] = r.m_mat[3][2] = 0.0f;
    r.m_mat[0][0] = r.m_mat[1][1] = r.m_mat[2][2] = r.m_mat[3][3] = 1.0f;

    return r;
}

bool            
Matrix::isValid(void)
{
#ifdef HAVE_FPCLASSIFY 
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            if (fpclassify(m_mat[i][j]) != FP_NORMAL && 
                fpclassify(m_mat[i][j]) != FP_ZERO)
                return false;
#endif
    return true;
}

void printf(Matrix mat)
{
    printf("%f %f %f %f\n", mat[0], mat[1], mat[2], mat[3]);
    printf("%f %f %f %f\n", mat[4], mat[5], mat[6], mat[7]);
    printf("%f %f %f %f\n", mat[8], mat[9], mat[10], mat[11]);
    printf("%f %f %f %f\n", mat[12], mat[13], mat[4], mat[15]);
}

Matrixd::Matrixd()
{
    for (int i=0; i < 4; i++)
       for (int k = 0; k < 4; k++)
          m_mat[i][k] = 0.0f;
}

Matrixd::Matrixd(double mat[4][4])
{
    for (int i = 0; i < 4; i++)
       for (int k = 0; k < 4; k++)
          m_mat[i][k] = mat[i][k];
}

Matrixd::Matrixd(double *d)
{
    for (int i = 0; i < 4; i++)
       for (int k = 0; k < 4; k++)
          m_mat[i][k] = d[i * 4 + k];
}

Matrixd
Matrixd::operator *(const Matrixd &m) const
{
    Matrixd r;

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            r.m_mat[i][j] = 0.0f;
            for (int k = 0; k < 4; k++) {
                r.m_mat[i][j] += m_mat[k][j] * m.m_mat[i][k];
            }
        }
    }
    return r;
}

Vec3d
Matrixd::operator *(const Vec3d &v) const
{
    Vec3d r;
    double iw;

    r.x = m_mat[0][0]*v.x + m_mat[1][0]*v.y + m_mat[2][0]*v.z + m_mat[3][0];
    r.y = m_mat[0][1]*v.x + m_mat[1][1]*v.y + m_mat[2][1]*v.z + m_mat[3][1];
    r.z = m_mat[0][2]*v.x + m_mat[1][2]*v.y + m_mat[2][2]*v.z + m_mat[3][2];
    iw = 1.0f / (m_mat[0][3]*v.x + m_mat[1][3]*v.y + m_mat[2][3]*v.z + m_mat[3][3]);
    
    return r * iw;
}

void
Matrixd::getValue(double mat[4][4]) const
{
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            mat[i][j] = m_mat[i][j];
        }
    }
}

void 
Matrixd::copyTo(Matrixd &m)
{
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            m.m_mat[i][j] = m_mat[i][j];
        }
    }
}

void
Matrixd::swapRows(int i, int j)
{
    double tmp;

    for (int k = 0; k < 4; k++) {
        tmp = m_mat[k][i];
        m_mat[k][i] = m_mat[k][j];
        m_mat[k][j] = tmp;
    }
}

Matrixd
Matrixd::invert() const        // Gauss-Jordan elimination with partial pivoting
{
    Matrixd a(*this),          // As a evolves from original mat into identity
           b(identity());     // b evolves from identity into inverse(a)
    
    int  i, j, i1, k;

    // Loop over cols of a from left to right, eliminating above and below diag
    for (j = 0; j < 4; j++) {   
        // Find largest pivot in column j among rows j..2
        i1 = j;                 // Row with largest pivot candidate
        for ( i = j + 1; i < 4; i++ )
            if ( fabs( a.m_mat[j][i] ) > fabs( a.m_mat[j][i1] ) )
                i1 = i;

        // Swap rows i1 and j in a and b to put pivot on diagonal
        if (i1 != j) {
            a.swapRows(i1, j);
            b.swapRows(i1, j);
        }

        // Scale row j to have a unit diagonal
        if (a.m_mat[j][j] == 0.0f) {
            swDebugf("Matrixd::invert():  singular Matrixd, can't invert\n");
            return b;
        }

        double s = 1.0f / a.m_mat[j][j];

        for (i = 0; i < 4; i++) {
            b.m_mat[i][j] *= s;
            a.m_mat[i][j] *= s;
        }

        // Eliminate off-diagonal elems in col j of a, doing identical ops to b
        for (i = 0; i < 4; i++) {
            if (i != j) {
                double t = a.m_mat[j][i];
                for (k = 0; k < 4; k++) {
                    b.m_mat[k][i] -= t * b.m_mat[k][j];
                    a.m_mat[k][i] -= t * a.m_mat[k][j];
                }
            }
        }
    }
    return b;
}

bool            
Matrixd::isValid(void)
{
#ifdef HAVE_FPCLASSIFY 
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            if (fpclassify(m_mat[i][j]) != FP_NORMAL && 
                fpclassify(m_mat[i][j]) != FP_ZERO)
                return false;
#endif
    return true;
}

Matrixd
Matrixd::identity()
{
    Matrixd r;

    r.m_mat[0][1] = r.m_mat[0][2] = r.m_mat[0][3] = 0.0f;
    r.m_mat[1][0] = r.m_mat[1][2] = r.m_mat[1][3] = 0.0f;
    r.m_mat[2][0] = r.m_mat[2][1] = r.m_mat[2][3] = 0.0f;
    r.m_mat[3][0] = r.m_mat[3][1] = r.m_mat[3][2] = 0.0f;
    r.m_mat[0][0] = r.m_mat[1][1] = r.m_mat[2][2] = r.m_mat[3][3] = 1.0f;

    return r;
}

