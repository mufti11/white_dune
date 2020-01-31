/*
 * Util.h
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
#include "Array.h"

class MFVec3f;
class MFVec2f;
class Node;
class MeshBasedNode;
class NodeIndexedFaceSet;
class Scene;

extern float boxCorners[8][3];
extern float boxTexCoords[4][2];
extern int boxIndices[24];
extern float spereCorners[6][3];
extern float CylinderCorners[6][3];
extern float ConeCorners[5][3];

enum BoxHandles {
    TRF = 6, TLF = 5, TRB = 2, TLB = 3, BRF = 7, BLF = 4, BRB = 1, BLB = 0
};

enum spereHandles {
    /*STRF = 6,*/ STLF = 5, STRB = 2, STLB = 3,/* SBRF = 7*/ SBLF = 4, SBRB = 1, SBLB = 0
};

enum CylinderHandles {
    /*STRF = 6,*/ CTLF = 5, CTRB = 2, CTLB = 3,/* SBRF = 7*/ CBLF = 4, CBRB = 1, CBLB = 0
};

enum ConeHandles {
     COTRB = 2, COTLB = 3,/* SBRF = 7*/ COBLF = 4, COBRB = 1, COBLB = 0
};

struct TableLdrawColorRGBValues;

class Util {
public:
    static void     printVec3f(Vec3f vec);
    static void     printVec3fln(Vec3f vec);

    static void     DrawBox(float sizeX, float sizeY, float sizeZ);
    static void     DrawCone(float radius, float height, 
                             bool side, bool bottom, int meshFlags);
    static Vec3f    IntersectSphere(float x1, float y1, float z1,
                                    float x2, float y2, float z2);
    static bool     IntersectLines(int x1, int y1, int x2, int y2,
                                   int x3, int y3, int x4, int y4);

    static void     createConeMesh(bool cleanDoubleVertices, 
                                   bool side, bool bottom);

    static int      getLdrawColorFromRGBA(float r, float g, float b, float a,
                                          bool leocad);

    // for viewing with anaglyph glasses, 
    // color settings have to be replaced with color settings of the gray value 
    static bool     need4Colors(GLenum pname);
    static float    grayFromColor(float r, float g, float b);
    static void     myGlColor3f(float r, float g, float b);
    static void     myGlColor4f(float r, float g, float b, float a);
    static void     myGlColor3fv(const float *c);
    static void     myGlColor4fv(const float *c);
    static void     myGlMaterial3fv(GLenum face, GLenum pname, 
                                    const float *params);
    static void     myGlMaterialfv(GLenum face, GLenum pname, 
                                   const float *params);
    static bool     invertMatrix(float out[16], const float in[16]);
    static void     getTexCoords(MyArray<MFVec2f *>&texCoords, Node *texCoord);

    Node *          getNurbsConvexHull(void);


#ifdef HAVE_LIBCGAL
    static NodeIndexedFaceSet *convexHull(Scene *scene, MyArray<Vec3f> vec);
#endif
};

