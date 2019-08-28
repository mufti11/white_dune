/*
 * Util.cpp
 *
 * Copyright (C) 1999 Stephen F. White
 *               2004 Wu Qingwei       
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

 /*
    Util::ConvertLDrawColor2LeoCADColor() based on ConvertColor() in
    leocad/common/pieceinf.cpp of the LeoCAD 0.75 sources by Leonardo Zide

    LeoCAD uses GNU GENERAL PUBLIC LICENSE Version 2, June 1991,
    see leocad/docs/COPYING.txt of the LeoCAD source package
 */

#include "stdafx.h"
#include "Util.h"
#include "Matrix.h"
#include "DuneApp.h"
#include "MyMesh.h"
#include "SFVec3f.h"
#include "SFBool.h"
#include "MFVec3f.h"
#include "MFInt32.h"
#include "MFColor.h"
#include "LdrawDefines.h"
#include "NodeTextureCoordinate.h"
#include "NodeMultiTextureCoordinate.h"

float boxCorners[8][3] = {
        { -1.0f, -1.0f, -1.0f },
        {  1.0f, -1.0f, -1.0f },
        {  1.0f,  1.0f, -1.0f },
        { -1.0f,  1.0f, -1.0f },
        { -1.0f, -1.0f,  1.0f },
        { -1.0f,  1.0f,  1.0f },
        {  1.0f,  1.0f,  1.0f },
        {  1.0f, -1.0f,  1.0f },
};

float boxPolygonCorners[24][3] = {
        { -1.0f, -1.0f, -1.0f },
        {  1.0f, -1.0f, -1.0f },
        {  1.0f,  1.0f, -1.0f },
        { -1.0f,  1.0f, -1.0f },

        { -1.0f, -1.0f,  1.0f },
        { -1.0f,  1.0f,  1.0f },
        {  1.0f,  1.0f,  1.0f },
        {  1.0f, -1.0f,  1.0f },

        { -1.0f, -1.0f,  1.0f },
        { -1.0f, -1.0f, -1.0f },
        {  1.0f, -1.0f, -1.0f },
        {  1.0f, -1.0f,  1.0f },

        { -1.0f,  1.0f,  1.0f },
        {  1.0f,  1.0f, -1.0f },
        { -1.0f,  1.0f, -1.0f },
        {  1.0f,  1.0f,  1.0f },


        { -1.0f,  1.0f, -1.0f },
        { -1.0f,  1.0f,  1.0f },
        { -1.0f, -1.0f,  1.0f },
        { -1.0f, -1.0f,  1.0f },

        {  1.0f,  1.0f, -1.0f },
        {  1.0f,  1.0f,  1.0f },
        {  1.0f, -1.0f,  1.0f },
        {  1.0f, -1.0f,  1.0f },
};


int boxIndices[24] = {
        TLF, BLF, BRF, TRF,  // front
        TRF, BRF, BRB, TRB,  // right side
        TRB, BRB, BLB, TLB,  // back
        TLB, BLB, BLF, TLF,  // left side
        TLB, TLF, TRF, TRB,  // top
        BLF, BLB, BRB, BRF,  // bottom
};

float boxNormals[6][3] = {
        {  0.0f,  0.0f,  1.0f },
        {  1.0f,  0.0f,  0.0f },
        {  0.0f,  0.0f, -1.0f },
        { -1.0f,  0.0f,  0.0f },
        {  0.0f,  1.0f,  0.0f },
        {  0.0f, -1.0f,  0.0f },
};

float    boxTexCoords[4][2] = {
        { 1.0f, 1.0f },
        { 1.0f, 0.0f },
        { 0.0f, 0.0f },
        { 0.0f, 1.0f },
};

float spereCorners[6][3] = {
        {  0.0f,  1.0f,  0.0f },
        {  0.0f, -1.0f,  0.0f },
        { -1.0f,  0.0f,  0.0f },
        {  0.0f, -0.0f, -1.0f },
        {  1.0f,  0.0f,  0.0f },
        {  0.0f,  0.0f,  1.0f },
};

float CylinderCorners[6][3] = {
        {  0.0f,  1.0f,  0.0f },
        {  0.0f, -1.0f,  0.0f },
        { -1.0f,  0.0f,  0.0f },
        {  0.0f, -0.0f, -1.0f },
        {  1.0f,  0.0f,  0.0f },
        {  0.0f,  0.0f,  1.0f },
};

float ConeCorners[5][3] = {
        { 0.0f,  1.0f,  0.0f },
        { 1.0f, -1.0f,  0.0f },
        {-1.0f, -1.0f,  0.0f },
        { 0.0f, -1.0f,  1.0f },
        { 0.0f, -1.0f, -1.0f }
};

void     
Util::printVec3f(Vec3f vec)
{
    printf("%f %f %f\n", vec.x, vec.y, vec.z);
}    

void 
Util::printVec3fln(Vec3f vec)
{
    printVec3f(vec);
    printf("\n");
}

void
Util::DrawBox(float sizeX, float sizeY, float sizeZ)
{
    glPushMatrix();
    glScalef(sizeX, sizeY, sizeZ);

    glEnable(GL_CULL_FACE);
    for (int i = 0; i < 24; i++) {
        if (i % 4 == 0) 
            glBegin(GL_POLYGON);
        glNormal3fv( boxNormals[i / 4] );
        glTexCoord2fv( boxTexCoords[i % 4] );
        glVertex3fv( boxPolygonCorners[boxIndices[i]] );
        if (i % 4 == 3) 
            glEnd();
    }
    glPopMatrix();
}

//
// IntersectSphere
//
// intersect a ray in 3-space (x1, y1, z1)->(x2, y2, z2) with a unit sphere 
// at the origin return the nearest point

Vec3f
Util::IntersectSphere(float x1, float y1, float z1, float x2, float y2, float z2)
{
    float i = x2 - x1;
    float j = y2 - y1;
    float k = z2 - z1;

    float a = i * i + j * j + k * k;
    float b = 2 * i * x1 + 2 * j * y1 + 2 * k * z1;
    float c = x1 * x1 + y1 * y1 + z1 * z1 - 1;

    if (a == 0.0f) return Vec3f(0.0f, 0.0f, 0.0f);

    float t = -b + (float) sqrt(b * b - 4.0f * a * c) / 2.0f * a;

    return Vec3f(x1 + t * i, y1 + t * j, z1 + t * k);
}

//
// IntersectLines()
//
// return "true" if the given 2D lines (x1, y1) - (x2, y2) and 
// (x3, y3) - (x4, y4) intersect
//

bool
Util::IntersectLines(int x1, int y1, int x2, int y2,
                     int x3, int y3, int x4, int y4)
{
    int denom = (x4 - x3) * (y2 - y1) - (y4 - y3) * (x2 - x1);
    int num = (y3 - y1) * (x2 - x1) - (x3 - x1) * (y2 - y1);

    float p2 = (float) num / (float) denom;
    float p1;
    if (x1 != x2) {
        p1 = (x3 + p2 * (x4 - x3) - x1) / (x2 - x1);
    } else if (y1 != y2) {
        p1 = (y3 + p2 * (y4 - y3) - y1) / (y2 - y1);
    } else {
        return false;
    }

    return p1 >= 0.0f && p1 <= 1.0f && p2 >= 0.0f && p2 <= 1.0f;
}   

float    
Util::grayFromColor(float r, float g, float b)
{
    return (r + g + b) / 3.0;
}

void
Util::myGlColor3f(float r, float g, float b)
{
    if (TheApp->isAnaglyphStereo()) {
        float gray = Util::grayFromColor(r, g, b);
        glColor3f(gray, gray, gray);
    } else
        glColor3f(r, g, b);
    
}

void     
Util::myGlColor4f(float r, float g, float b, float a)
{
    if (TheApp->isAnaglyphStereo()) {
        float gray = Util::grayFromColor(r, g, b);
        glColor4f(gray, gray, gray, a);
    } else
        glColor4f(r, g, b, a);
    
}

void     
Util::myGlColor3fv(const float *c)
{
    if (TheApp->isAnaglyphStereo()) {
        float gray = Util::grayFromColor(c[0], c[1], c[2]);
        glColor3f(gray, gray, gray);
    } else
        glColor3fv(c);
    
}

void     
Util::myGlColor4fv(const float *c)
{
    if (TheApp->isAnaglyphStereo()) {
        float gray = Util::grayFromColor(c[0], c[1], c[2]);
        glColor4f(gray, gray, gray, c[3]);
    } else
        glColor4fv(c);
}

bool 
Util::need4Colors(GLenum pname)
{
    float need4Colors = false;
    switch (pname) {
      case GL_AMBIENT:
      case GL_DIFFUSE:
      case GL_SPECULAR:
      case GL_EMISSION:
      case GL_AMBIENT_AND_DIFFUSE:
        need4Colors = true;
        break;
      case GL_COLOR_INDEXES:
        // GL_COLOR_INDEXES not supported yet
        assert(0);
    }
    return need4Colors;
}

void
Util::myGlMaterial3fv(GLenum face, GLenum pname,  const float *c)
{
    bool has4Colors = need4Colors(pname); 
    if (TheApp->isAnaglyphStereo()) {
        float gray = Util::grayFromColor(c[0], c[1], c[2]);
        if (has4Colors) {
            float color[4] = { gray, gray, gray, 1.0f };
            glMaterialfv(face, pname, color);
        } else {
            float color[3] = { gray, gray, gray };
            glMaterialfv(face, pname, color);
        }
    } else {
        if (has4Colors) {
            float color[4] = { c[0], c[1], c[2], 1.0f };
            glMaterialfv(face, pname, color);
        } else 
            glMaterialfv(face, pname, c);
    }
}

void
Util::myGlMaterialfv(GLenum face, GLenum pname,  const float *c)
{
    bool has4Colors = need4Colors(pname); 
    if (TheApp->isAnaglyphStereo()) {
        float gray = Util::grayFromColor(c[0], c[1], c[2]);
        if (has4Colors) {
            float color[4] = { gray, gray, gray, c[3] };
            glMaterialfv(face, pname, color);
        } else {
            float color[3] = { gray, gray, gray };
            glMaterialfv(face, pname, color);
        }
    } else 
        glMaterialfv(face, pname, c);
}

MyMesh *coneMesh = NULL;
MyMesh *coneMeshOnlySide = NULL;
MyMesh *coneMeshOnlyBottom = NULL;

void
Util::createConeMesh(bool cleanDoubleVertices, bool bside, bool bbottom)
{
    float fradius = 1;
    float fheight = 1;

    int tess = TheApp->getTessellation();
    int numPoints = (1 + tess + 1) * 3;
    
    if (!bside && !bbottom)
        return;

    float *fpoint = new float[numPoints];
    float *fnormal = new float[numPoints];

    int offset = 0;
    fpoint[offset + 0] = 0;
    fpoint[offset + 1] = -fheight / 2.0f;
    fpoint[offset + 2] = 0;
    fnormal[offset + 0] = 0;
    fnormal[offset + 1] = -1;
    fnormal[offset + 2] = 0;

    for (int i = 0; i < tess; i++) {
        float theta = 2.0f * M_PI * i / tess;
        int offset = (i + 1) * 3; 
        Vec3f point(sin(theta), -fheight / 2.0f, cos(theta));
        fpoint[offset + 0] = fradius * point.x;
        fpoint[offset + 1] = point.y;
        fpoint[offset + 2] = fradius * point.z;            
        point.normalize();
        fnormal[offset + 0] = point.x;
        fnormal[offset + 1] = point.y;
        fnormal[offset + 2] = point.z;            
    }

    offset = (tess + 1) * 3;
    fpoint[offset + 0] = 0;
    fpoint[offset + 1] = fheight / 2.0f;
    fpoint[offset + 2] = 0;
    fnormal[offset + 0] = 0;
    fnormal[offset + 1] = 1;
    fnormal[offset + 2] = 0;

    MyArray<int> icoordIndex;
    if (bbottom)
        for (int i = 0; i < tess; i++) {    
            if (i == (tess - 1))
                icoordIndex.append(1);
            else
                icoordIndex.append(1 + i + 1);
            icoordIndex.append(0);
            icoordIndex.append(1 + i);
            icoordIndex.append(-1);
        } 
    if (bside)
        for (int i = 0; i < tess; i++) { 
            icoordIndex.append(tess + 1);
            if (i == (tess - 1))
                icoordIndex.append(1);
            else
                icoordIndex.append(1 + i + 1);
            icoordIndex.append(1 + i);
            icoordIndex.append(-1);
        } 

    MFVec3f *coords = new MFVec3f(fpoint, numPoints);

    MFInt32 *coordIndex = new MFInt32((int *)icoordIndex.getData(), 
                                        icoordIndex.size());

    MFVec3f *normals = new MFVec3f(fnormal, numPoints);

    MFInt32 *normalIndex = NULL;
    
    MFColor *colors = NULL;
    MFInt32 *colorIndex = NULL;
    MyArray<MFVec2f *>texCoords;
    MFInt32 *texCoordIndex = NULL;

    int meshFlags = MESH_NORMAL_PER_VERTEX;

    MyMesh *mesh = new MyMesh(NULL,
                              coords, coordIndex, normals, normalIndex, colors, 
                              colorIndex, texCoords, texCoordIndex,
                              M_PI / 2.0 - 0.0001f, meshFlags, 0);
    mesh->smoothNormals();
    if (bside && bbottom) {
        if (coneMesh)
            delete coneMesh;
        coneMesh = mesh;
    } else if (bside) {
        if (coneMeshOnlySide) 
            delete coneMeshOnlySide;
        coneMeshOnlySide = mesh;
    } else if (bbottom) {
        if (coneMeshOnlyBottom)
            delete coneMeshOnlyBottom;
        coneMeshOnlyBottom = mesh;
    }
}

void
Util::DrawCone(float radius, float height, bool side, bool bottom, 
               int meshFlags)
{
    glPushMatrix();
    glScalef(radius, height, radius);
    if (side && bottom) {
        if (coneMesh == NULL) 
            createConeMesh(false, side, bottom);
        coneMesh->draw(meshFlags);
    } else if (side) {
        if (coneMeshOnlySide == NULL) 
            createConeMesh(false, side, bottom);
        coneMeshOnlySide->draw(meshFlags);
    } else if (bottom) {
        if (coneMeshOnlyBottom == NULL) 
            createConeMesh(false, side, bottom);
        coneMeshOnlyBottom->draw(meshFlags);
    }
    glPopMatrix();
}

struct TableLdrawColorRGBValues {
    int ldrawColor;
    float r;
    float g;
    float b;
    float a;
    bool matchingColorName;
};

// data based on "LDraw.org Color Chart" http://www.ldraw.org/Article93.html

struct TableLdrawColorRGBValues ldrawColorTable[] = {
    // please blame the creators of the Ldraw standard for the 
    // magic number problem 8-)
    {   0,  0.13, 0.13, 0.13,    1, true },
    {   1,  0.00, 0.20, 0.70,    1, true },
    {  10,  0.42, 0.93, 0.56,    1, true },
    {   6,  0.36, 0.13, 0.00,    1, true },
    { 334,  0.88, 0.43, 0.07,    1, true },
    { 383,  0.88, 0.88, 0.88,    1, true },
    {  47,  1.00, 1.00, 1.00, 0.90, true },
    { 272,  0.00, 0.11, 0.41,    1, true },
    {   8,  0.39, 0.37, 0.32,    1, true },
    { 288,  0.15, 0.27, 0.17,    1, true },
    { 484,  0.70, 0.24, 0.00,    1, true },
    {   5,  0.87, 0.40, 0.58,    1, true },
    { 320,  0.47, 0.00, 0.11,    1, true },
    {  72,  0.39, 0.37, 0.38,    1, true },
    {  28,  0.77, 0.59, 0.31,    1, false },
    { 366,  0.82, 0.51, 0.02,    1, true },
    { 494,  0.82, 0.82, 0.82,    1, false },
    {   7,  0.76, 0.76, 0.76,    1, true },
    {   2,  0.00, 0.55, 0.08,    1, true },
    {   9,  0.42, 0.67, 0.86,    1, true },
    { 503,  0.90, 0.89, 0.85,    1, true },
    {  17,  0.73, 1.00, 0.81,    1, true },
    { 462,  1.00, 0.62, 0.02,    1, true },
    {  12,  1.00, 0.50, 0.20,    1, true }, // leocad orange, ldraw defines {  12,  1.00, 0.52, 0.48,    1, true }, a rather pink orange
    {  20,  0.84, 0.77, 0.90,    1, true },
    {  18,  0.99, 0.91, 0.59,    1, true },
    {  27,  0.84, 0.94, 0.00,    1, true },
    {  26,  0.85, 0.11, 0.43,    1, true },
    {  25,  0.98, 0.38, 0.00,    1, true },
    { 134,  0.58, 0.53, 0.40,    1, true },
    { 142,  0.84, 0.66, 0.29,    1, true },
    { 135,  0.67, 0.68, 0.67,    1, true },
    { 137,  0.42, 0.48, 0.59,    1, true },
    {  21,  0.88, 1.00, 0.69,    1, true },
    {  13,  0.98, 0.64, 0.78,    1, true },
    {   4,  0.77, 0.00, 0.15,    1, true },
    {  70,  0.41, 0.25, 0.15,    1, true },
    { 256,  0.13, 0.13, 0.13,    1, true },
    { 273,  0.00, 0.20, 0.70,    1, true },
    { 375,  0.76, 0.76, 0.76,    1, true },
    { 324,  0.77, 0.00, 0.15,    1, true },
    { 511,  1.00, 1.00, 1.00,    1, true },
    { 379,  0.42, 0.48, 0.59,    1, true },
    { 378,  0.63, 0.74, 0.67,    1, true },
    { 335,  0.75, 0.53, 0.51,    1, true },
    { 373,  0.52, 0.37, 0.52,    1, true },
    {  71,  0.64, 0.64, 0.64,    1, true },
    {  19,  0.91, 0.81, 0.63,    1, true },
    {   3,  0.00, 0.60, 0.62,    1, true },
    {  33,  0.00, 0.13, 0.63, 0.90, true },
    {  42,  0.75, 1.00, 0.00, 0.90, true },
    {  57,  0.98, 0.38, 0.00, 0.80, true },
    {  40,  0.39, 0.37, 0.32, 0.90, true },
    {  34,  0.02, 0.39, 0.20, 0.90, true },
    {  41,  0.68, 0.94, 0.93, 0.95, true },
    {  45,  0.87, 0.40, 0.58,    1, false },
    {  36,  0.77, 0.00, 0.15, 0.90 , true }, 
    {  37,  0.39, 0.00, 0.38,    1, false },
    {  46,  0.79, 0.69, 0.00, 0.90, true },
    {  11,  0.20, 0.65, 0.65,    1, true },
    {  22,  0.51, 0.00, 0.48,    1, true },
    {  23,  0.28, 0.20, 0.69,    1, true },
    {  15,  1.00, 1.00, 1.00,    1, true },
    {  14,  1.00, 0.86, 0.00,    1, true },
    {  44,  1.00, 0.50, 0.20, 0.90, true } // added: leocad transparent orange
};

// Extracted and modified from common/pieceinf.cpp of LeoCAD 0.75
// needed modifications: returntype and all places with "return -1"

// Convert a color from LDraw to LeoCAD
static int convertLDrawColor2LeoCADColor(int c)
{
    if (c > 255) c -= 256;
    switch (c)
    {
      // please blame the creator of LeoCAD for the magic number problem 8-)
      case 0: return 9;    // black        (black)
      case 1: return 4;    // blue         (blue)
      case 2: return 2;    // green        (green)
      case 3: return 5;    // dark cyan
      case 4: return 0;    // red          (red)
      case 5: return 11;   // magenta
      case 6: return 10;   // brown        (brown)
      case 7: return 22;   // gray         (gray)
      case 8: return 8;    // dark gray    (dark gray)
      case 9: return 5;    // light blue   ()
      case 10: return 3;   // light green  (light green)
      case 11: return 5;   // cyan         (light blue)
      case 12: return 1;   // light red
      case 13: return 11;  // pink         (pink)
      case 14: return 6;   // yellow       (yellow)
      case 15: return 7;   // white        (white)
      case 16: return -1;  // invalid: LC_COL_DEFAULT; // special case
      case 24: return -1;  // invalid: LC_COL_EDGES; // edge
      case 32: return 9;   // black
      case 33: return 18;  // clear blue
      case 34: return 16;  // clear green
      case 35: return 5;   // dark cyan
      case 36: return 14;  // clear red
      case 37: return 11;  // magenta
      case 38: return 10;  // brown
      case 39: return 21;  // clear white  (clear gray)
      case 40: return 8;   // dark gray
      case 41: return 19;  // clear light  blue
      case 42: return 17;  // clear light  green
      case 43: return 19;  // clear cyan   (clear light blue)
      case 44: return 15;  // clear light  red ??
      case 45: return 11;  // pink
      case 46: return 20;  // clear yellow
      case 47: return 21;  // clear white
      case 70: return 10;  // maroon       (326)
      case 78: return 13;  // gold         (334)
      case 110: return 1;  // orange       (366 from fire logo pattern)
      case 126: return 23; // tan          (382)
      case 127: return 27; // silver/chrome (383)
      case 175: return 3;  // mint green   (431)
      case 206: return 1;  // orange       (462)
      case 238: return 6;  // light yellow (494 eletric contacts)
      case 239: return 6;  // light yellow (495)
      case 247: return 27; // 503 chrome
      case 250: return 3;  // 506 mint     (Belville)
      case 253: return 11; // 509 rose     (e.g. in Paradisa)
  
      // taken from l2p.doc but not verified
      case 178: return 11; // 434 dark     cyan (e.g. in New Technic Models)
      case 254: return 6;  // 510 light    yellow (e.g. in Belville)
    }
    return -1; // invalid
}

int 
Util::getLdrawColorFromRGBA(float r, float g, float b, float a, bool leocad)
{
    float minError = 4.0f;
    int ret = LDRAW_CURRENT_COLOR;
    int tableLength = sizeof(ldrawColorTable) / 
                      sizeof(struct TableLdrawColorRGBValues);
    struct TableLdrawColorRGBValues *table = ldrawColorTable;
    for (int i = 0; i < tableLength; i++) {
        if ((a < 1) && (table[i].a == 1))
            continue;
        if ((a == 1) && (table[i].a < 1))
            continue;
        if (leocad && (!table[i].matchingColorName))
            continue;
        if (convertLDrawColor2LeoCADColor(table[i].ldrawColor) == -1)
            continue;
        float currentError = (table[i].r - r) * (table[i].r - r) + 
                             (table[i].g - g) * (table[i].g - g) + 
                             (table[i].b - b) * (table[i].b - b);
        if (currentError < minError) {
            ret = table[i].ldrawColor;
            minError = currentError;
        }
    } 
    return ret;
}

bool 
Util::invertMatrix(float out[16], const float in[16])
{
    float inv[16];
    float det = 0;

    inv[0] =   in[5]  *  in[10] *  in[15] - 
               in[5]  *  in[11] *  in[14] - 
               in[9]  *  in[6]  *  in[15] + 
               in[9]  *  in[7]  *  in[14] +
               in[13] *  in[6]  *  in[11] - 
               in[13] *  in[7]  *  in[10];

    inv[4] =  -in[4]  *  in[10] *  in[15] + 
               in[4]  *  in[11] *  in[14] + 
               in[8]  *  in[6]  *  in[15] - 
               in[8]  *  in[7]  *  in[14] - 
               in[12] *  in[6]  *  in[11] + 
               in[12] *  in[7]  *  in[10];

    inv[8] =   in[4]  *  in[9]  *  in[15] - 
               in[4]  *  in[11] *  in[13] - 
               in[8]  *  in[5]  *  in[15] + 
               in[8]  *  in[7]  *  in[13] + 
               in[12] *  in[5]  *  in[11] - 
               in[12] *  in[7]  *  in[9];

    inv[12] = -in[4]  *  in[9]  *  in[14] + 
               in[4]  *  in[10] *  in[13] +
               in[8]  *  in[5]  *  in[14] - 
               in[8]  *  in[6]  *  in[13] - 
               in[12] *  in[5]  *  in[10] + 
               in[12] *  in[6]  *  in[9];

    inv[1] =  -in[1]  *  in[10] * in[15] + 
               in[1]  *  in[11] * in[14] + 
               in[9]  *  in[2]  * in[15] - 
               in[9]  *  in[3]  * in[14] - 
               in[13] *  in[2]  * in[11] + 
               in[13] *  in[3]  * in[10];

    inv[5] =   in[0]  * in[10] * in[15] - 
               in[0]  * in[11] * in[14] - 
               in[8]  * in[2]  * in[15] + 
               in[8]  * in[3]  * in[14] + 
               in[12] * in[2]  * in[11] - 
               in[12] * in[3]  * in[10];

    inv[9] =  -in[0]  * in[9]  * in[15] + 
               in[0]  * in[11] * in[13] + 
               in[8]  * in[1]  * in[15] - 
               in[8]  * in[3]  * in[13] - 
               in[12] * in[1]  * in[11] + 
               in[12] * in[3]  * in[9];

    inv[13] =  in[0]  * in[9]  * in[14] - 
               in[0]  * in[10] * in[13] - 
               in[8]  * in[1]  * in[14] + 
               in[8]  * in[2]  * in[13] + 
               in[12] * in[1]  * in[10] - 
               in[12] * in[2]  * in[9];

    inv[2] =   in[1]  * in[6]  * in[15] - 
               in[1]  * in[7]  * in[14] - 
               in[5]  * in[2]  * in[15] + 
               in[5]  * in[3]  * in[14] + 
               in[13] * in[2]  * in[7] - 
               in[13] * in[3]  * in[6];

    inv[6] =  -in[0]  * in[6]  * in[15] + 
               in[0]  * in[7]  * in[14] + 
               in[4]  * in[2]  * in[15] - 
               in[4]  * in[3]  * in[14] - 
               in[12] * in[2]  * in[7] + 
               in[12] * in[3]  * in[6];

    inv[10] =  in[0]  * in[5]  * in[15] - 
               in[0]  * in[7]  * in[13] - 
               in[4]  * in[1]  * in[15] + 
               in[4]  * in[3]  * in[13] + 
               in[12] * in[1]  * in[7] - 
               in[12] * in[3]  * in[5];

    inv[14] = -in[0]  * in[5]  * in[14] + 
               in[0]  * in[6]  * in[13] + 
               in[4]  * in[1]  * in[14] - 
               in[4]  * in[2]  * in[13] - 
               in[12] * in[1]  * in[6] + 
               in[12] * in[2]  * in[5];

    inv[3] =  -in[1]  * in[6]  * in[11] + 
               in[1]  * in[7]  * in[10] + 
               in[5]  * in[2]  * in[11] - 
               in[5]  * in[3]  * in[10] - 
               in[9]  * in[2]  * in[7] + 
               in[9]  * in[3]  * in[6];

    inv[7] =   in[0]  * in[6]  * in[11] - 
               in[0]  * in[7]  * in[10] - 
               in[4]  * in[2]  * in[11] + 
               in[4]  * in[3]  * in[10] + 
               in[8]  * in[2]  * in[7] - 
               in[8]  * in[3]  * in[6];

    inv[11] = -in[0]  * in[5]  * in[11] + 
               in[0]  * in[7]  * in[9] + 
               in[4]  * in[1]  * in[11] - 
               in[4]  * in[3]  * in[9] - 
               in[8]  * in[1]  * in[7] + 
               in[8]  * in[3]  * in[5];

    inv[15] =  in[0]  * in[5]  * in[10] - 
               in[0]  * in[6]  * in[9] - 
               in[4]  * in[1]  * in[10] + 
               in[4]  * in[2]  * in[9] + 
               in[8]  * in[1]  * in[6] - 
               in[8]  * in[2]  * in[5];

    det = in[0] * inv[0] + in[1] * inv[4] + in[2] * inv[8] + in[3] * inv[12];

    if (det == 0)
        return false;

    det = 1.0 / det;

    for (int i = 0; i < 16; i++)
        out[i] = inv[i] * det;

    return true;
}

void
Util::getTexCoords(MyArray<MFVec2f *>&texCoords, Node *texCoord)
{
    MFVec2f *mftexCoord = NULL;
    if (texCoord) {
        if (texCoord->getType() == VRML_TEXTURE_COORDINATE) {
            mftexCoord = ((NodeTextureCoordinate *)texCoord)->point();
            texCoords.append(mftexCoord);
        } else if (texCoord->getType() == 
                   X3D_MULTI_TEXTURE_COORDINATE) {
            NodeMultiTextureCoordinate *multiTexCoord = 
                (NodeMultiTextureCoordinate *)texCoord;
            for (int i = 0; i < multiTexCoord->texCoord()->getSize(); i++) {
                if (multiTexCoord->texCoord()->getValue(i)->getType() 
                    == VRML_TEXTURE_COORDINATE) {
                    mftexCoord = ((NodeTextureCoordinate *)
                                  multiTexCoord->texCoord()->getValue(i))->
                                  point();
                    texCoords.append(mftexCoord);
                }
            }
        }
    }         
}

#ifdef HAVE_LIBCGAL
#undef max
#include "Scene.h"
#include "NodeIndexedFaceSet.h"

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Polyhedron_3.h>
#include <CGAL/Surface_mesh.h>
#include <CGAL/convex_hull_3.h>
#include <vector>

typedef CGAL::Exact_predicates_inexact_constructions_kernel  K;
typedef CGAL::Polyhedron_3<K>                     Polyhedron_3;
typedef K::Point_3                                Point_3;
typedef CGAL::Surface_mesh<Point_3>               Surface_mesh;
typedef Surface_mesh::Face_index                  Face_index;

// workaround a bug in Boost-1.54
#include <CGAL/boost/graph/dijkstra_shortest_paths.h>
#include <boost/graph/prim_minimum_spanning_tree.hpp>
#include <boost/foreach.hpp>
typedef boost::graph_traits<Surface_mesh>::vertex_descriptor vertex_descriptor;
typedef boost::graph_traits<Surface_mesh>::face_descriptor face_descriptor;
typedef boost::graph_traits<Surface_mesh>::halfedge_descriptor halfedge_descriptor;

NodeIndexedFaceSet *
Util::convexHull(Scene *scene, MyArray<Vec3f> vec)
{
    int diffentVecCount = 0;
    for (size_t i = 0; i < vec.size(); i++) {
        for (size_t j = i + 1; j < vec.size(); j++) {
            if ((vec[i] - vec[j]).length() != 0)
                diffentVecCount++;
            if (diffentVecCount > 3)
                break;
        }
     }
     if (diffentVecCount <= 3)
         return NULL;

    std::vector<Point_3> points;
    Point_3 p;
    for (size_t i = 0; i < vec.size(); i++) {
        Point_3 p(vec[i].x, vec[i].y, vec[i].z);
        points.push_back(p);
    }
    // compute convex hull of non-collinear points
    Surface_mesh poly;
    CGAL::convex_hull_3(points.begin(), points.end(), poly);

    NodeCoordinate *ncoord = (NodeCoordinate *)
                             scene->createNode("Coordinate");
    NodeIndexedFaceSet *faceSet = (NodeIndexedFaceSet *)
                                   scene->createNode("IndexedFaceSet");
    faceSet->coord(new SFNode(ncoord));

    MFVec3f *newVertices = new MFVec3f();
    MFInt32 *newCoordIndex = new MFInt32();

    BOOST_FOREACH(vertex_descriptor vd, vertices(poly)){
        newVertices->appendSFValue(poly.point(vd).x(), 
                                   poly.point(vd).y(), 
                                   poly.point(vd).z()); 
    }
    BOOST_FOREACH(face_descriptor fd, faces(poly)) {
        Surface_mesh::Halfedge_index hf = poly.halfedge(fd);
        BOOST_FOREACH(halfedge_descriptor hi, halfedges_around_face(hf,poly)) {
            Surface_mesh::Vertex_index vi = target(hi, poly);
            newCoordIndex->appendSFValue((std::size_t)vi);
        }
        newCoordIndex->appendSFValue(-1);
    }
    ncoord->point(new MFVec3f(newVertices));
    faceSet->coordIndex(new MFInt32(newCoordIndex));
    return faceSet;
}
#endif


