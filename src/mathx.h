/* https://github.com/skaslev/catmull-clark/mathx.h
The MIT License

Copyright (c) 2009-2017 Slavomir Kaslev, https://github.com/skaslev/catmull-clark

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.

*/
#ifndef MATHX_H
#define MATHX_H

#ifdef _WIN32
#define _USE_MATH_DEFINES 1
#endif

#include <math.h>
#include <string.h>

#define PI        ((float) M_PI)
#define TAU        (2.0f * PI)

extern float degrees(float rad);
extern float radians(float deg);
extern float sqrf(float a);
extern float clampf(float a, float min, float max);
extern float minf(float a, float b);
extern float maxf(float a, float b);
extern float mixf(float a, float b, float t);

/* Vector operations */
typedef float vector[3];

extern void vec_copy(vector r, const vector a);
extern void vec_zero(vector r);
extern void vec_set(vector r, float x, float y, float z);
extern void vec_neg(vector r, const vector a);
extern void vec_add(vector r, const vector a, const vector b);
extern void vec_sub(vector r, const vector a, const vector b);
extern void vec_mul(vector r, float f, const vector a);
extern void vec_mad(vector r, float f, const vector a);
extern float vec_dot(const vector a, const vector b);
extern float vec_len(const vector a);
extern void vec_normalize(vector r, const vector a);
extern void vec_cross(vector r, const vector a, const vector b);
extern float vec_dist(const vector a, const vector b);
extern void vec_min(vector r, const vector a, const vector b);
extern void vec_max(vector r, const vector a, const vector b);
extern void vec_mix(vector r, const vector a, const vector b, float t);
extern void vec_spherical(vector r, float phi, float theta);
extern void vec_to_spherical(const vector r, float *phi, float *theta);

/* Matrix operations */
typedef float matrix[16];

extern void mat_copy(matrix r, const matrix a);
extern void mat_zero(matrix r);
extern void mat_identity(matrix r);

float mat_determinant(const matrix m);
void mat_invert(matrix r, const matrix a);
void mat_adjoint(matrix r, const matrix a, float *det);

void mat_mul(matrix r, const matrix a, const matrix b);
void mat_mul_point(vector r, const matrix a, const vector v);
void mat_mul_vector(vector r, const matrix a, const vector v);

void mat_scale(matrix r, float s);
void mat_translate(matrix r, const vector v);
void mat_rotate(matrix r, const vector axis, float angle);
void mat_lookat(matrix r, const vector eye, const vector at, const vector up);
void mat_frame(matrix r, const vector o, const vector x, const vector y, const vector z);
void mat_frustum(matrix r, float left, float right, float bot, float top, float near, float far);
void mat_ortho(matrix r, float left, float right, float bot, float top, float near, float far);
void mat_persp(matrix r, float fovy, float aspect, float near, float far);

#endif
