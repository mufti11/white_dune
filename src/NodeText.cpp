/*
 * NodeText.cpp
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
#include "stdafx.h"

#include "NodeText.h"
#include "Proto.h"
#include "FieldValue.h"
#include "MFString.h"
#include "SFNode.h"
#include "MFFloat.h"
#include "SFFloat.h"
#include "SFString.h"
#include "SFBool.h"
#include "Scene.h"
#include "NodeFontStyle.h"
#include "FontInfo.h"
#include "Util.h"
#include "MyMesh.h"

ProtoText::ProtoText(Scene *scene, const char* name)
  : GeometryProto(scene, name)
{
    addElements();
}

ProtoText::ProtoText(Scene *scene)
  : GeometryProto(scene, "Text")
{
    addElements();
}

void 
ProtoText::addElements(void)
{
    fontStyle.set(
          addExposedField(SFNODE, "fontStyle", new SFNode(NULL), 
                          FONT_STYLE_NODE));
    length.set(
          addExposedField(MFFLOAT, "length", new MFFloat(), new MFFloat(0.0f)));
    maxExtent.set(
          addExposedField(SFFLOAT, "maxExtent", new SFFloat(0.0f), 
                          new SFFloat(0.0f)));
    string.set(
          addExposedField(MFSTRING, "string", new MFString()));

    solid.set(
         addField(SFBOOL, "solid", new SFBool(false)));
    setFieldFlags(solid, FF_X3D_ONLY | FF_KAMBI_ONLY);

    texCoord.set(
          addExposedField(SFNODE, "texCoord", new SFNode(NULL), 
                          GENERATED_TEXTURE_COORDINATE_NODE));
    setFieldFlags(texCoord, FF_KAMBI_ONLY);

    x3domGeometryCommonFields()

    ccw.set(
          addExposedField(SFBOOL, "ccw", new SFBool(true)));
    setFieldFlags(ccw, FF_X3DOM_ONLY);

    addEventOut(MFVEC2F, "lineBounds", FF_X3D_ONLY);
    addEventOut(SFVEC3F, "origin", FF_X3D_ONLY);
    addEventOut(MFVEC2F, "textBounds", FF_X3D_ONLY);
}

Node *
ProtoText::create(Scene *scene)
{ 
    return new NodeText(scene, this); 
}

#ifdef HAVE_LIBFREETYPE
NodeText::NodeText(Scene *scene, Proto *def)
  : MeshBasedNode(scene, def)
#else
NodeText::NodeText(Scene *scene, Proto *def)
  : GeometryNode(scene, def)
#endif
{
    m_textDirty = true;
    m_deBackslashedStrings = NULL;
}

void            
NodeText::reInit(void) 
{
    m_textDirty = true; 
    Node::reInit();
}

#ifdef HAVE_LIBFREETYPE

#include <ft2build.h>
#include <freetype/freetype.h>
#include <freetype/ftglyph.h>
#include <freetype/ftoutln.h>
#include <freetype/fttrigon.h>
//#include <vector>
#include <iostream>
#include <fstream>

/* 
 * Text rendering based on https://github.com/codetiger/Font23D/ Font2OBJ.cpp
 * with the following license:
 *
 *  A license with no conditions whatsoever which dedicates works to the public
 *  domain. Unlicensed works, modifications, and larger works may be 
 *  distributed under different terms and without source code.
 */

#include "PointFtgl.h"
#include "Vectoriser.h"
#include "poly2tri.h"

#include "Vec3f.h"
#include "Array.h"

using namespace std;
using namespace p2t;

int USE_OPENGL = 0;

struct Tri
{
    Vec3f a, b, c;
};

MyArray<Tri> tris;

vector<p2t::Point*> triangulateContour(Vectoriser *vectoriser, int c, float offset) {
    vector<p2t::Point*> polyline;
    const Contour* contour = vectoriser->GetContour(c);
    for (size_t p = 0; p < contour->PointCount(); ++p) {
        const double* d = contour->GetPoint(p);
        polyline.push_back(new p2t::Point((d[0]/64.0f) + offset, d[1]/64.0f));
    }
    return polyline;
}

void beginCallback(GLenum which)
{
    printf("Unknown tesselasion enum: %d\n", which);
    glBegin(GL_TRIANGLES);
}

void endCallback(void)
{
    glEnd();
}

void flagCallback( GLboolean ) 
{

}

void errorCallback(GLenum errorCode)
{
    const GLubyte *estring;

    estring = gluErrorString(errorCode);
    fprintf(stderr, "Tessellation Error: %s\n", estring);
}

static int tcount = 0;
static Tri t;

void vertexCallback(GLvoid *vertex)
{
    const GLdouble *d;
    d = (GLdouble *) vertex;

    if (tcount == 0) {
        t.a.x = d[0];
        t.a.y = d[1];
        t.a.z = d[2];
        tcount++;
    } else if (tcount == 1) {
         t.c.x = d[0];
         t.c.y = d[1];
         t.c.z = d[2];
         tcount++;
    } else if (tcount == 2) {
         t.b.x = d[0];
         t.b.y = d[1];
         t.b.z = d[2];
         tris.append(t);
         tcount = 0;
         printf("Creating triangles\n");
    }
}

void combineCallback(GLdouble coords[3], GLdouble *vertex_data[4], GLfloat weight[4], GLdouble **dataOut)
{
    printf("combineCallback\n");
    GLdouble *vertex;
    int i;

    vertex = (GLdouble *) malloc(6 * sizeof(GLdouble));
    vertex[0] = coords[0];
    vertex[1] = coords[1];
    vertex[2] = coords[2];
    for (i = 3; i < 7; i++)
        vertex[i] = weight[0] * vertex_data[0][i] + weight[1] * vertex_data[1][i] + weight[2] * vertex_data[2][i] + weight[3] * vertex_data[3][i];
    *dataOut = vertex;
}


float AddCharacter(FT_Face face, char ch, unsigned short bezierSteps, float offset, float extrude) 
{
    static FT_UInt prevCharIndex = 0, curCharIndex = 0;
        static FT_Pos  prev_rsb_delta = 0;

    curCharIndex = FT_Get_Char_Index( face, ch );
    if (FT_Load_Glyph( face, curCharIndex, FT_LOAD_DEFAULT ))
        printf("FT_Load_Glyph failed\n");

    FT_Glyph glyph;
    if (FT_Get_Glyph( face->glyph, &glyph ))
        printf("FT_Get_Glyph failed\n");
 
    if (glyph->format != FT_GLYPH_FORMAT_OUTLINE) {
        printf("Invalid Glyph Format\n");
        exit(0);
    }

    if (FT_HAS_KERNING(face) && prevCharIndex) {
        FT_Vector  kerning;
        FT_Get_Kerning(face, prevCharIndex, curCharIndex, FT_KERNING_DEFAULT,
                       &kerning );
        offset += kerning.x >> 6;
    }

    if (prev_rsb_delta - face->glyph->lsb_delta >= 32)
        offset -= 1.0f;
    else if ( prev_rsb_delta - face->glyph->lsb_delta < -32 )
        offset += 1.0f;

    prev_rsb_delta = face->glyph->rsb_delta;                       

    Vectoriser *vectoriser = new Vectoriser(face->glyph, bezierSteps);
    for (size_t c = 0; c < vectoriser->ContourCount(); ++c) {
        const Contour* contour = vectoriser->GetContour(c);

        for (size_t p = 0; p < contour->PointCount() - 1; ++p) {
            const double* d1 = contour->GetPoint(p);
            const double* d2 = contour->GetPoint(p + 1);
            Tri t1;
            t1.a.x = (d1[0]/64.0f) + offset;
            t1.a.y = d1[1]/64.0f;
            t1.a.z = 0.0f;
            t1.b.x = (d2[0]/64.0f) + offset;
            t1.b.y = d2[1]/64.0f;
            t1.b.z = 0.0f;
            t1.c.x = (d1[0]/64.0f) + offset;
            t1.c.y = d1[1]/64.0f;
            t1.c.z = extrude;
            tris.append(t1);

            Tri t2;
            t2.a.x = (d1[0]/64.0f) + offset;
            t2.a.y = d1[1]/64.0f;
            t2.a.z = extrude;
            t2.c.x = (d2[0]/64.0f) + offset;
            t2.c.y = d2[1]/64.0f;
            t2.c.z = extrude;
            t2.b.x = (d2[0]/64.0f) + offset;
            t2.b.y = d2[1]/64.0f;
            t2.b.z = 0.0f;
            tris.append(t2);
        }

        if (contour->GetDirection()) {
            if (USE_OPENGL) {
                GLUtesselator* tobj = gluNewTess();

                gluTessCallback(tobj, GLU_TESS_VERTEX, (GLvoid (*) ()) &vertexCallback);
                gluTessCallback(tobj, GLU_TESS_BEGIN, (GLvoid (*) ()) &beginCallback);
                gluTessCallback(tobj, GLU_TESS_END, (GLvoid (*) ()) &endCallback);
                gluTessCallback(tobj, GLU_TESS_ERROR, (GLvoid (*) ()) &errorCallback);
                gluTessCallback(tobj, GLU_TESS_COMBINE, (GLvoid (*) ()) &combineCallback);
                gluTessCallback(tobj, GLU_TESS_EDGE_FLAG, (GLvoid (*) ()) &flagCallback);

                gluTessProperty(tobj, GLU_TESS_WINDING_RULE, GLU_TESS_WINDING_ODD);

                gluTessProperty(tobj, GLU_TESS_TOLERANCE, 0);
                gluTessNormal(tobj, 0.0f, 0.0f, 0.0f);

                gluTessBeginPolygon(tobj, NULL);
                for (size_t c = 0; c < vectoriser->ContourCount(); ++c) {
                    const Contour* contour = vectoriser->GetContour(c);
                    gluTessBeginContour(tobj);
                    for (size_t p = 0; p < contour->PointCount(); ++p) {
                        const double* d1 = contour->GetPoint(p);
                        double *d = new double[3];
                        d[0] = d1[0] / 64.0f + offset;
                        d[1] = d1[1] / 64.0f;
                        d[2] = d1[2] / 64.0f;
                        gluTessVertex(tobj, (GLdouble *)d, (GLvoid *)d);
                    }

                    gluTessEndContour(tobj);
                }
                gluTessEndPolygon(tobj);

            } else {
                vector<p2t::Point*> polyline = triangulateContour(vectoriser, c, offset);
                CDT* cdt = new CDT(polyline);

                for (size_t cm = 0; cm < vectoriser->ContourCount(); ++cm) {
                    const Contour* sm = vectoriser->GetContour(cm);
                    if (c != cm && !sm->GetDirection() && 
                        sm->IsInside(contour)) {
                        vector<p2t::Point*> pl = triangulateContour(vectoriser, 
                                                                    cm, offset);
                        cdt->AddHole(pl);
                    }
                }

                cdt->Triangulate();
                vector<Triangle*> ts = cdt->GetTriangles();
                for (unsigned int i = 0; i < ts.size(); i++) {
                    Triangle* ot = ts[i];

                    Tri t1;
                    t1.a.x = ot->GetPoint(0)->x;
                    t1.a.y = ot->GetPoint(0)->y;
                    t1.a.z = 0.0f;
                    t1.b.x = ot->GetPoint(1)->x;
                    t1.b.y = ot->GetPoint(1)->y;
                    t1.b.z = 0.0f;
                    t1.c.x = ot->GetPoint(2)->x;
                    t1.c.y = ot->GetPoint(2)->y;
                    t1.c.z = 0.0f;
                    tris.append(t1);

                    Tri t2;
                    t2.a.x = ot->GetPoint(1)->x;
                    t2.a.y = ot->GetPoint(1)->y;
                    t2.a.z = extrude;
                    t2.b.x = ot->GetPoint(0)->x;
                    t2.b.y = ot->GetPoint(0)->y;
                    t2.b.z = extrude;
                    t2.c.x = ot->GetPoint(2)->x;
                    t2.c.y = ot->GetPoint(2)->y;
                    t2.c.z = extrude;
                    tris.append(t2);
                }
                delete cdt;
            }
        }
    }

    delete vectoriser;
    vectoriser = NULL;

    prevCharIndex = curCharIndex;
    float chSize = face->glyph->advance.x >> 6;
    return offset + chSize;
}

FT_Face face;

static void initFreeType(const char *ttfFile) 
{
    FT_Library library;
    if (FT_Init_FreeType(&library))
        printf("FT_Init_FreeType failed\n");

    if (FT_New_Face( library, ttfFile, 0, &face ))
        printf("FT_New_Face failed (there is probably a problem with your font file\n");
 } 

#define JUSTIFY_BEGIN 1
#define JUSTIFY_END 2
#define JUSTIFY_MIDDLE 3

#define SPACING 0.8

void
NodeText::createMesh(bool cleanDoubleVertices, bool triangulateMesh)
{
    static bool initialized = false;
    if (!initialized)
        initFreeType(TheApp->getTtfFile());
    initialized = true;

    int height = 48;
    unsigned short bezierSteps = 4;
    float extrude = getDepth();

    FT_Set_Char_Size( face, height << 6, height << 6, 96, 96);

    NodeFontStyle *fontStyle = (NodeFontStyle *) 
                    ((SFNode *) getField(fontStyle_Field()))->getValue();

    float fsize = SPACING;
    float fspacing = 1;

    int ijustify = JUSTIFY_BEGIN;
    if (fontStyle) {
        fsize = fontStyle->size()->getValue() * SPACING;
        fspacing = fontStyle->spacing()->getValue();
    }

    MFVec3f *coords = new MFVec3f();
    MFInt32 *coordIndex = new MFInt32();
    int triangles = 0;
    for (int j = 0; j < string()->getSize(); j++) {
        tris.resize(0);
        const char* str = string()->getValue(j);

        if (fontStyle) {
            if (strcmp(fontStyle->justify()->getValue(j), "MIDDLE") == 0)
                ijustify = JUSTIFY_MIDDLE;
            if (strcmp(fontStyle->justify()->getValue(j), "END") == 0)
                ijustify = JUSTIFY_END;
        }
        float offset = 0; 
        for (unsigned int i = 0; i < strlen(str); i++) {
            offset = AddCharacter(face, str[i], bezierSteps, offset, extrude);
        }

        float maxX = FLT_MIN;
        for (int i = 0; i < tris.size(); i++) {
            Tri t = tris[i];
            float x1 = fsize * (t.a.x / height); 
            if (x1 > maxX)
                maxX = x1;
            float x2 = fsize * (t.b.x / height); 
            if (x2 > maxX)
                maxX = x2;
            float x3 = fsize * (t.c.x / height); 
            if (x3 > maxX)
                maxX = x3;
        }
        float addX = 0;
        if (ijustify == JUSTIFY_MIDDLE)
            addX = -maxX / 2;
        else if (ijustify == JUSTIFY_END)
            addX = -maxX;
        for (int i = 0; i < tris.size(); i++) {
            Tri t = tris[i];
            coords->appendSFValue(fsize * (t.a.x / height) + addX, 
                                  fsize * (t.a.y / height) - j * fspacing, 
                                  t.a.z - extrude / 2);
            coords->appendSFValue(fsize * (t.b.x / height) + addX,
                                  fsize * (t.b.y / height) - j * fspacing, 
                                  t.b.z - extrude / 2);
            coords->appendSFValue(fsize * (t.c.x / height) + addX, 
                                  fsize * (t.c.y / height) - j * fspacing, 
                                  t.c.z - extrude / 2);
            coordIndex->appendSFValue(triangles++);
            coordIndex->appendSFValue(triangles++);
            coordIndex->appendSFValue(triangles++);
            coordIndex->appendSFValue(-1);
        }
    }
    float transparency = 0;
    if (hasParent())
        transparency = getParent()->getTransparency();
    MyArray<MFVec2f *> texCoords;
    m_mesh = new MyMesh(this, coords, coordIndex, NULL, NULL, NULL, NULL,
                        texCoords, NULL, 0, 0, transparency);

}

void
NodeText::setField(int index, FieldValue *value, int cf)
{
    m_meshDirty = true;
    Node::setField(index, value, cf);
}

#else

// since version white_dune-0.29beta548 incorperate the needed font parts 
// from openglut
// #ifdef HAVE_LIBGLUT
// #include "GL/glut.h"

# ifndef _WIN32
extern "C" {
#include "openglutfont.h"
}

static char *
deBackslashString(const char *string)
{
    char *ret;

    if (string == NULL)
        return NULL;

    ret = (char *) malloc(strlen(string) + 2);
    ret[0] = 0;
  
    int retCounter = 0;
    for (int i = 0; i < strlen(string); i++) {
        if (string[i] == '\\')
            i++;
        ret[retCounter] = string[i];
        retCounter++;
    }
    ret[retCounter] = 0;
    return ret;
}

void
NodeText::cleanText(void)
{
    MFString *mfstring = string();
    if (mfstring == NULL)
        return;
    delete m_deBackslashedStrings;
    m_deBackslashedStrings = new MFString(); 
    for (int i = 0; i < mfstring->getSize(); i++) {
        const char *oldString = mfstring->getValue(i);
        char *newString = deBackslashString(oldString);
        MyString sfValue = "";
        sfValue += newString;
        m_deBackslashedStrings->insertSFValue(i, sfValue);
        free(newString);
    }
    m_textDirty = false;
}

// yet another ugly textnode implementation 8-(

void
NodeText::draw()
{
    if (m_textDirty)
        cleanText();

    MFString *mfstring = m_deBackslashedStrings;
    if (mfstring == NULL)
        return;
    NodeFontStyle *fontStyle = (NodeFontStyle *) 
                    ((SFNode *) getField(fontStyle_Field()))->getValue();

    float fsize = 1;
    if (fontStyle)
        fsize = fontStyle->size()->getValue();
    
    GLfloat diff_color[4];
    GLfloat em_color[4];

    glGetMaterialfv(GL_FRONT, GL_DIFFUSE, diff_color);
    glGetMaterialfv(GL_FRONT, GL_EMISSION, em_color);
    GLfloat color[4];
    for (int i = 0; i < 3; i++) {
        color[i] = em_color[i];
        if (m_scene->getNumLights() != 0)
           color[i] += diff_color[i];
        if (color[i] > 1)
           color[i] = 1 ;
    }
    color[3] = diff_color[3];

    glPushAttrib(GL_ENABLE_BIT);

    glDisable(GL_LIGHTING);
    glDisable(GL_BLEND);
    
    Util::myGlColor4f(color[0], color[1], color[2], color[3]);
    glEnable(GL_LINE_SMOOTH);

    for (int j = 0; j < mfstring->getSize(); j++) {
        const char  *str = mfstring->getValue(j);
        int n = strlen(str);
        glPushMatrix();
        glTranslatef(0, -j * fsize, 0);
        const float GLUT_STROKE_ROMAN_SIZE = 119.05;
        float scale = 1/GLUT_STROKE_ROMAN_SIZE;
        glScalef(scale * fsize, scale * fsize, 1.0);
        for (int i = 0; i < n; i++)
            glutStrokeCharacter(GLUT_STROKE_ROMAN, str[i]);
        glPopMatrix();
    }

    glLineWidth(1.0);

    glPopAttrib();
}
# else

void
NodeText::draw()
{
    MFString *mfstring = string();
    NodeFontStyle *fontStyle = (NodeFontStyle *) 
                    ((SFNode *) getField(fontStyle_Field()))->getValue();
//    const float *length = ((MFFloat *) getField(length_Field()))->getValue();
//    float maxExtent = ((SFFloat *) getField(maxExtent_Field()))->getValue();
    FontInfo *font = NULL;
    bool fhorizontal = true;
    bool bleftToRight = true;
    float fsize = 1.0f;
    float fspacing = 1.0f;
    bool btopToBottom = true;

    if (fontStyle) {
        MFString *ffamily = fontStyle->family();
        fhorizontal = fontStyle->horizontal()->getValue();
//      MFString *justify = fontStyle->justify();
//      const char *language = fontStyle->language()->getValue();
        bleftToRight = fontStyle->leftToRight()->getValue();
        fsize = fontStyle->size()->getValue();
        fspacing = fontStyle->spacing()->getValue();
        const char *sstyle = fontStyle->style()->getValue();
        btopToBottom = fontStyle->topToBottom()->getValue();

        for (int i = 0; i < ffamily->getSize(); i++) {
            font = m_scene->LoadGLFont(ffamily->getValue(i), sstyle);
            if (font) break;
        }

    } else {
        font = m_scene->LoadGLFont("SERIF", "PLAIN");
    }

    if (!font) return; // couldn't find the font

    glPushMatrix();
    glPushAttrib(GL_TEXTURE_BIT);

    glScalef(fsize, fsize, 1.0f);

    glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
    glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);

    glEnable(GL_TEXTURE_GEN_S);
    glEnable(GL_TEXTURE_GEN_T);

    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
    float sPlane[4] = {1.0, 0.0, 0.0, 0.0};
    float tPlane[4] = {0.0, 1.0, 0.0, 0.0};

    float ySpacing = btopToBottom ? - fspacing : fspacing;

    for (int j = 0; j < mfstring->getSize(); j++) {
        const char  *str = mfstring->getValue(j);
        int n = strlen(str);
        if (fhorizontal) {
            sPlane[3] = 0.0f;
            glPushMatrix();
            for (int i = 0; i < n; i++) {
               if (bleftToRight) {
                    glTexGenfv(GL_S, GL_OBJECT_PLANE, sPlane);
                    glTexGenfv(GL_T, GL_OBJECT_PLANE, tPlane);
                    glCallList(font->displayListBase + str[i]);
                    sPlane[3] += font->kernX[str[i]];
                } else {
                    glTranslatef(-font->kernX[str[i]], 0.0f, 0.0f);
                    sPlane[3] -= font->kernX[str[i]];
                    glTexGenfv(GL_S, GL_OBJECT_PLANE, sPlane);
                    glTexGenfv(GL_T, GL_OBJECT_PLANE, tPlane);
                    glPushMatrix();
                    glCallList(font->displayListBase + str[i]);
                    glPopMatrix();
                }
            }
            glPopMatrix();
            glTranslatef(0.0f, ySpacing, 0.0f);
            tPlane[3] += ySpacing;
        } else {
            tPlane[3] = 0.0f;
            if (!bleftToRight) {
                glTranslatef(-fspacing * 0.5f, 0.0f, 0.0f);
                sPlane[3] += -fspacing * 0.5f;
            }
            glPushMatrix();
            for (int i = 0; i < n; i++) {
                if (btopToBottom) {
                    float y = -font->kernY[str[i]];
                    if (y == 0.0) y = -1.0f;  // FIXME:  default advance?
                    glTranslatef(0.0f, y, 0.0f);
                    tPlane[3] += y;
                    glPushMatrix();
                    glTexGenfv(GL_S, GL_OBJECT_PLANE, sPlane);
                    glTexGenfv(GL_T, GL_OBJECT_PLANE, tPlane);
                    glCallList(font->displayListBase + str[i]);
                    glPopMatrix();
                } else {
                    glPushMatrix();
                    glTexGenfv(GL_S, GL_OBJECT_PLANE, sPlane);
                    glTexGenfv(GL_T, GL_OBJECT_PLANE, tPlane);
                    glCallList(font->displayListBase + str[i]);
                    glPopMatrix();
                    float y = font->kernY[str[i]];
                    if (y == 0.0) y = 1.0f;  // FIXME:  default advance?
                    glTranslatef(0.0f, y, 0.0f);
                    tPlane[3] += y;
                }
            }
            glPopMatrix();
            if (bleftToRight) {
                glTranslatef(fspacing * 0.5f, 0.0f, 0.0f);
                sPlane[3] += fspacing * 0.5f;
            }
        }
    }

    glDisable(GL_TEXTURE_GEN_S);
    glDisable(GL_TEXTURE_GEN_T);

    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);

    glPopAttrib();
    glPopMatrix();
}

# endif

#endif
