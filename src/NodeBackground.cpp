/*
 * NodeBackground.cpp
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

// setColor(), drawQuad(), drawSky() and drawGround() based on 
// render_Background() in Bindable.c of FreeWRL (ported by wu qingwei)
/*******************************************************************
 Copyright (C) 1998 Tuomas J. Lukka
 Copyright (C) 2002 John Stewart, CRC Canada.
 DISTRIBUTED WITH NO WARRANTY, EXPRESS OR IMPLIED.
 See the GNU Library General Public License (file COPYING in the distribution)
 for conditions of use and redistribution.
*********************************************************************/

#include <stdio.h>
#include "stdafx.h"

#include "NodeBackground.h"
#include "Proto.h"
#include "FieldValue.h"
#include "MFColor.h"
#include "MFFloat.h"
#include "MFString.h"
#include "SFColor.h"
#include "SFRotation.h"
#include "NodeImageTexture.h"
#include "NodeViewpoint.h"
#include "Scene.h"
#include "Util.h"
#include "Field.h" // for FF_URL
#include "Node.h"
#include "Matrix.h"

ProtoBackground::ProtoBackground(Scene *scene)
  : Proto(scene, "Background")
{
    double unitAngle = scene->getUnitAngle();

    addEventIn(SFBOOL, "set_bind");

    groundAngle.set(
          addExposedField(MFFLOAT, "groundAngle", new MFFloat(), 
                          new SFFloat(0.0f), 
                          new SFFloat(M_PI / 2.0f / unitAngle)));

    groundColor.set(
         addExposedField(MFCOLOR, "groundColor", new MFColor()));

    backUrl.set(
         addExposedField(MFSTRING, "backUrl", new MFString(), FF_URL));

    rightUrl.set(
         addExposedField(MFSTRING, "rightUrl", new MFString(), FF_URL));

    frontUrl.set(
         addExposedField(MFSTRING, "frontUrl", new MFString(), FF_URL));

    leftUrl.set(
         addExposedField(MFSTRING, "leftUrl", new MFString(), FF_URL));

    topUrl.set(
         addExposedField(MFSTRING, "topUrl", new MFString(), FF_URL));

    bottomUrl.set(
         addExposedField(MFSTRING, "bottomUrl", new MFString(), FF_URL));

    skyAngle.set(
         addExposedField(MFFLOAT, "skyAngle", new MFFloat(), new SFFloat(0.0f),
                         new SFFloat(M_PI / unitAngle)));

    transparency.set(
         addExposedField(SFFLOAT, "transparency", new SFFloat(0), 
                         new SFFloat(0.0f), new SFFloat(1.0f)));

    float *colors = new float[3];
    colors[0] = colors[1] = colors[2] = 0.0f;
    skyColor.set(
          addExposedField(MFCOLOR, "skyColor", new MFColor(colors, 3)));

    bind.set(
          addExposedField(SFBOOL, "bind", new SFBool(false)));
    setFieldFlags(bind, FF_X3DOM_ONLY);

    crossOrigin.set(
          addExposedField(SFSTRING, "crossOrigin", new SFString("")));
    setFieldFlags(crossOrigin, FF_X3DOM_ONLY);

    description.set(
          addExposedField(SFSTRING, "description", new SFString("")));
    setFieldFlags(description, FF_X3DOM_ONLY);

    isActive.set(
          addExposedField(SFBOOL, "isActive", new SFBool(false)));
    setFieldFlags(isActive, FF_X3DOM_ONLY);

    addEventOut(SFBOOL, "isBound");
}

Node *
ProtoBackground::create(Scene *scene)
{ 
    return new NodeBackground(scene, this); 
}

NodeBackground::NodeBackground(Scene *scene, Proto *def)
  : Node(scene, def)
{
    for (int i = 0; i < 6; i++) {
        imageTextures[i] = (NodeImageTexture *) 
                           scene->createNode("ImageTexture");
        imageTextures[i]->ref();
    }
}

NodeBackground::~NodeBackground()
{
    for (int i = 0; i < 6; i++) {
        imageTextures[i]->unref();
    }
}

void
NodeBackground::setField(int field, FieldValue *value, int cf)
{
    if (field >= backUrl_Field() && field <= bottomUrl_Field()) {
        imageTextures[field - backUrl_Field()]->url((MFString *)value);
    }

    Node::setField(field, value, cf);
}

void 
NodeBackground::preDraw()
{
    glPushMatrix();
    glGetFloatv(GL_MODELVIEW_MATRIX, m_matrix);
    for (int i = 0; i < 3; i++)
        m_matrix[3 * 4 + i] = 0;
//  error: scaled transform not handled
    m_matrix[15] = 1;
/*
    for (int i = 0; i < 3; i++)
        m_matrix[i * 4 + 3] = 0;
    m_matrix[0] = 1;
    m_matrix[6] = 1;
    m_matrix[9] = 1;
*/
    glPopMatrix();    

    m_scene->addBackground(this);
}


void
NodeBackground::apply()
{
    glPushAttrib(GL_ENABLE_BIT|GL_TEXTURE_BIT);
    glPushMatrix();

    double unitAngle = m_scene->getUnitAngle();

    glDisable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glDisable(GL_FOG);
    glDisable(GL_CULL_FACE);
    glLoadIdentity();
    Quaternion r = m_scene->getCamera()->getOrientation();
    glRotatef(RAD2DEG(-r.w * unitAngle), r.x, r.y, r.z);
    glMultMatrixf(m_matrix);
    drawBackgroundSphere();
    drawBackgroundTextures();

    glPopMatrix();
    glPopAttrib();
}

void
NodeBackground::setColor(float *newColor)
{
    Util::myGlMaterial3fv(GL_FRONT, GL_EMISSION, newColor);
    Util::myGlColor3f(newColor[0], newColor[1], newColor[2]);
}

void
NodeBackground::drawQuad(float r, float va1, float va2, float h1, float h2, 
                         float *newColor)
{
    float ha1 = h1 * 2.0 * M_PI;
    float ha2 = h2 * 2.0 * M_PI;
    glVertex3d(r * sin(va2) * cos(ha1), r * cos(va2), r * sin(va2) * sin(ha1));
    glVertex3d(r * sin(va2) * cos(ha2), r * cos(va2), r * sin(va2) * sin(ha2));
    if (newColor)
        setColor(newColor);
    glVertex3d(r * sin(va1) * cos(ha2), r * cos(va1), r * sin(va1) * sin(ha2));
    glVertex3d(r * sin(va1) * cos(ha1), r * cos(va1), r * sin(va1) * sin(ha1));
}

#define NUMBER_HORIZONTAL_POLYGONS 20

void
NodeBackground::drawSky()
{
    double unitAngle = m_scene->getUnitAngle();

    float sc = TheApp->GetFarClippingPlaneDist() - 2;
    int hdiv = NUMBER_HORIZONTAL_POLYGONS;
    glBegin(GL_QUADS);
    if (skyColor()->getSFSize() == 1) {
        float va1 = 0;
        float va2 = M_PI / 2.0; 
        setColor(skyColor()->getValue(0));
        for (int v = 0; v < 2; v++) {
            for (float h = 0; h < hdiv; h++)
                drawQuad(sc, va1, va2, h / hdiv, (h + 1) / hdiv, NULL);
            va1 = va2;
            va2 = M_PI;
        }
    } else {
        float va1 = 0;
        float va2 = 0;
 
        if (skyAngle()->getSFSize() != 0) {
            va2 = skyAngle()->getValue(0) * unitAngle;
            float black[] = { 0, 0, 0 };
            float* colors2 = black;

            for(int v = 0; v < skyColor()->getSFSize() - 1; v++) {
                if (v >= skyAngle()->getSFSize())
                    break;
                va2 = skyAngle()->getValue(v) * unitAngle;
                float* colors1 = skyColor()->getValue(v);
                colors2 = skyColor()->getValue(v + 1);

                for (float h = 0; h < hdiv; h++) {
                    setColor(colors2);
                    drawQuad(sc, va1, va2, h / hdiv, (h + 1) / hdiv, colors1);
                }
                va1 = va2;
            }

            if (va2 < M_PI) {
                setColor(colors2);
                for (float h = 0; h < hdiv; h++)
                    drawQuad(sc, M_PI, va2, h / hdiv, (h + 1) / hdiv, NULL);
            }
        }
    }
    glEnd();
}

void
NodeBackground::drawGround()
{
    double unitAngle = m_scene->getUnitAngle();

    int hdiv = NUMBER_HORIZONTAL_POLYGONS;
    // where to put the ground quads 
    float sc = TheApp->GetFarClippingPlaneDist() - 4; 

    glBegin(GL_QUADS);
    if (groundColor()->getSFSize() == 1) {
        float* gcolors1 = groundColor()->getValue(0);
        setColor(gcolors1);
        for (float h = 0; h < hdiv; h++)
            drawQuad(sc, M_PI / 2.0, M_PI , h / hdiv, (h + 1) / hdiv, NULL);
    } else {
        float va1 = M_PI;
        for (int v = 0; v < groundColor()->getSFSize() - 1; v++) {
            float* gcolors1 = groundColor()->getValue(v);
            float* gcolors2 = groundColor()->getValue(v+1);

            float va2;
            if (v >= groundAngle()->getSFSize())
                break;
            va2 = M_PI - groundAngle()->getValue(v) * unitAngle;

            for (float h = 0; h < hdiv; h++) {
                setColor(gcolors1);
                drawQuad(sc, va2, va1 , h / hdiv, (h + 1) / hdiv, gcolors2);
            }
            va1 = va2;
        }
    }
    glEnd();
}

void
NodeBackground::drawBackgroundSphere()
{
    glShadeModel(GL_SMOOTH);
    glDisable(GL_LIGHTING);

    if (skyColor())
        if (skyColor()->getSFSize() > 0)
            drawSky();

    if (groundColor())
        if (groundColor()->getSFSize()>0)
            drawGround();
}

void
NodeBackground::drawBackgroundTextures()
{
    glEnable(GL_LIGHTING);
    static float white[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
    Util::myGlMaterialfv(GL_FRONT, GL_EMISSION, white);

    glScalef(40.0f, 40.0f, 40.0f);

    for (int i = 0; i < 6; i++) {
        if (imageTextures[i]->url() && 
            imageTextures[i]->url()->getSize() != 0) {
            imageTextures[i]->bind();
            glBegin(GL_POLYGON);
            if (i < 4)
                for (int j = 0; j < 4; j++) {
                    glTexCoord2fv( boxTexCoords[j] );
                    glVertex3fv( boxCorners[boxIndices[i*4+j]] );
                }
            else
                for (int j = 0; j < 4; j++) {
                    glTexCoord2fv( boxTexCoords[(j+2)%4] );
                    glVertex3fv( boxCorners[boxIndices[i*4+j]] );
                }
            glEnd();
            imageTextures[i]->unbind();
        }
    }
}

int
NodeBackground::countPolygons(void)
{
    int ret = 0;
    if (skyColor())
        ret += NUMBER_HORIZONTAL_POLYGONS * skyColor()->getSFSize();
    if (groundColor())
        ret += NUMBER_HORIZONTAL_POLYGONS * groundColor()->getSFSize();
    for (int i = 0; i < 6; i++)
        if (imageTextures[i]->url()->getSize() > 0)
            ret++;
    return ret;
}

int NodeBackground::getProfile(void) const
{ 
    if (!isDefault(groundColor_Field()))
        if (groundColor())
            return PROFILE_IMMERSIVE;
    if (!isDefault(frontUrl_Field()))
        return PROFILE_IMMERSIVE;
    if (!isDefault(rightUrl_Field()))
        return PROFILE_IMMERSIVE;
    if (!isDefault(backUrl_Field()))
        return PROFILE_IMMERSIVE;
    if (!isDefault(leftUrl_Field()))
        return PROFILE_IMMERSIVE;
    if (!isDefault(topUrl_Field()))
        return PROFILE_IMMERSIVE;
    if (!isDefault(bottomUrl_Field()))
        return PROFILE_IMMERSIVE;
    return PROFILE_INTERCHANGE;
}

int
NodeBackground::getComponentLevel(void) const
{
    if (!isDefault(skyAngle_Field()))
        return 2;
    if (skyColor())
        if (skyColor()->getSFSize() > 1)
            return 2;
    if (!isDefaultAngle(groundAngle_Field()))
        return 2;
    if (!isDefault(groundColor_Field()))
        if (groundColor())
            if (groundColor()->getSFSize() > 1)
                return 2;
    return -1;
}

const char* 
NodeBackground::getComponentName(void) const
{
    static const char* name = "EnvironmentalEffects";
    return name;
}
