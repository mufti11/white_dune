/*
 * Matrix.h
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

#include "Vec3f.h"
#include "Vec3d.h"

class Quaternion;

class Matrix {
public:
                    Matrix();
                    Matrix(float mat[4][4]);
                    Matrix(Quaternion quat);
                    Matrix(float *f);

    Matrix          operator*(const Matrix &m) const;
    Vec3f           operator*(const Vec3f& v) const;
    Vec3d           operator*(const Vec3d& v) const;

    operator        float *() { return (float *) m_mat; }
    operator        const float *() const { return (const float *) m_mat; }
    float          &operator[](int index) { return *((float*)m_mat + index); }
    void            getValue(float mat[4][4]) const;
    void            copyTo(Matrix &m);

    Matrix          invert() const;

    static Matrix   identity();
    bool            isZero(void);
    void            swapRows(int i, int j);

private:
    float           m_mat[4][4];
};

class Matrixd {
public:
                    Matrixd();
                    Matrixd(double mat[4][4]);
                    Matrixd(double *d);

    Matrixd         operator*(const Matrixd &m) const;
    Vec3d           operator*(const Vec3d& v) const;
    Vec3f           operator*(const Vec3f& v) const;

    operator        double *() { return (double *) m_mat; }
    operator        const double *() const { return (const double *) m_mat; }
    double         &operator[](int index) { return *((double*)m_mat + index); }
    void            getValue(double mat[4][4]) const;
    void            copyTo(Matrixd &m);

    Matrixd         invert() const;

    static Matrixd  identity();
    bool            isZero(void);
    void            swapRows(int i, int j);

private:
    double          m_mat[4][4];
};

void printf(Matrix mat);
void printf(Matrixd mat);


