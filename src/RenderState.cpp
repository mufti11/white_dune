/*
 * RenderState.cpp
 *
 * Copyright (C) 2000 Stephen F. White, 2015 Doug Sanden
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

#include "stdafx.h"

#include "DuneApp.h"
#include "RenderState.h"
#include "Scene.h"
#include "Util.h"

#define ROTATION_HANDLE_SEGMENTS    40

RenderState::RenderState()
{
    m_axesStart = 0.0f;
    m_vectorStart = 0.0f;
    m_modelviewMatrix = Matrixd::identity();
}

Vec3f
RenderState::project(const Vec3f &point)
{
    Vec3d w(point.x, point.y, point.z);
    Vec3d d = project(w);
    Vec3f r(d.x, d.y, d.z);   
    return r;
}

Vec3d
RenderState::project(const Vec3d &point)
{
    Vec3d w = m_projectionMatrix * (m_modelviewMatrix * point);
    Vec3d r;

    r.x = m_viewport[0] + (w.x + 1.0f) * m_viewport[2] * 0.5;
    r.y = m_viewport[1] + (w.y + 1.0f) * m_viewport[3] * 0.5;
    r.z = (w.z + 1.0) * 0.5;
    
    return r;
}

void
RenderState::startDrawHandles()
{
    glGetDoublev(GL_MODELVIEW_MATRIX, (GLdouble *) m_modelviewMatrix);
    glGetDoublev(GL_PROJECTION_MATRIX, (GLdouble *) m_projectionMatrix);
    glGetDoublev(GL_VIEWPORT, (GLdouble *) m_viewport);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(m_viewport[0], m_viewport[0] + m_viewport[2],
            m_viewport[1], m_viewport[1] + m_viewport[3],
            -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glEnable(GL_CULL_FACE);
    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
}

void
RenderState::drawHandle(const Vec3f &pos)
{
    Vec3f w = project(pos);

    float handleSize = TheApp->GetHandleSize();

    glBegin(GL_POLYGON);
    glVertex3f(w.x + handleSize, w.y - handleSize, w.z);
    glVertex3f(w.x + handleSize, w.y + handleSize, w.z);
    glVertex3f(w.x - handleSize, w.y + handleSize, w.z);
    glVertex3f(w.x - handleSize, w.y - handleSize, w.z);
    glEnd();
}

void
RenderState::drawHandle(const Vec3d &pos)
{
    Vec3d w = project(pos);

    float handleSize = TheApp->GetHandleSize();

    glBegin(GL_POLYGON);
    glVertex3d(w.x + handleSize, w.y - handleSize, w.z);
    glVertex3d(w.x + handleSize, w.y + handleSize, w.z);
    glVertex3d(w.x - handleSize, w.y + handleSize, w.z);
    glVertex3d(w.x - handleSize, w.y - handleSize, w.z);
    glEnd();
}

void
RenderState::setHandleColor(Scene *scene, int handle)
{
    if (scene->getInfoHandles())
        Util::myGlColor3f(1.0f, 0.0f, 0.0f);
    else if (scene->isInSelectedHandles(handle))
        Util::myGlColor3f(0.0f, 1.0f, 0.0f);
    else
        Util::myGlColor3f(1.0f, 1.0f, 1.0f);
}

void
RenderState::endDrawHandles()
{
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glLoadMatrixd((GLdouble *) m_projectionMatrix);
    glMatrixMode(GL_MODELVIEW);
}

/*
static void gl_get_camera_parameter_from_perspective_matrix(GLdouble *mat,
                  double &fovy_rad, double &aspect_ratio,
                  double &clip_min, double & clip_max)
    {
    GLdouble const aa = mat[0];
    GLdouble const bb = mat[5];
    GLdouble const cc = mat[10];
    GLdouble const dd = mat[14];
    
    aspect_ratio = bb / aa;
    fovy_rad = 2.0f * atan(1.0f / bb);
    
    GLdouble const kk = (cc - 1.0f) / (cc + 1.0f);
    clip_min = (dd * (1.0f - kk)) / (2.0f * kk);
    clip_max = kk * clip_min;
}
*/

double 
RenderState::autoScale(void)
{
    //auto-scaling translation handles
    glGetDoublev(GL_MODELVIEW_MATRIX, (GLdouble *) m_modelviewMatrix);
    Vec3d zerod = Vec3d(0.0,0.0,0.0);
    Vec3d oned = Vec3d(1.0,1.0,1.0);
    Vec3d unprojected = m_modelviewMatrix * zerod;
    Vec3d unprojectedb = m_modelviewMatrix * oned;
    Vec3d delta = unprojected - unprojectedb;
    double dist10 = delta.length() / oned.length();
    double distd = -unprojected.z;
    double dscale = 1.0;
    dscale = (distd/10.0)*(1.0/dist10);
    return dscale;
}

void
RenderState::drawTranslationHandles(Node* node, float scale)
{
    float black[4] = { 0.0f, 0.0f, 0.0f, 1.0f };

    Scene *scene = node->getScene();

    glPushMatrix();
    node->transformForHandle(TRANSLATION);

    scale *= autoScale();

    glScalef(HANDLE_SIZE, HANDLE_SIZE, HANDLE_SIZE);

    glScalef(scale, scale, scale);

    glPushName(TRANSLATION);
    drawAxisLines();

    glEnable(GL_CULL_FACE);

    Util::myGlMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, black);
    Util::myGlMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, black);

    GLUquadricObj *obj = gluNewQuadric();

    if (scene->getXonly() || (scene->getConstrain() == CONSTRAIN_NONE)) {
        glLoadName(TRANSLATION_X);
        drawCone(obj, 1.0f, 0.0f, 0.0f);
    }

    if (scene->getYonly() || (scene->getConstrain() == CONSTRAIN_NONE)) {
        glLoadName(TRANSLATION_Y);
        drawCone(obj, 0.0f, 1.0f, 0.0f);
    }

    if (scene->getZonly() || (scene->getConstrain() == CONSTRAIN_NONE)) {
        glLoadName(TRANSLATION_Z);
        drawCone(obj, 0.0f, 0.0f, 1.0f);
    }

    gluDeleteQuadric(obj);
    glPopName();

    glDisable(GL_CULL_FACE);

    glPopMatrix();
}

void
RenderState::draw6DHandles(Node* node, float scale)
{
    float black[4] = { 0.0f, 0.0f, 0.0f, 1.0f };

    Scene *scene = node->getScene();

    glPushMatrix();
    node->transformForHandle(TRANSLATION);

    scale *= autoScale();

    glScalef(HANDLE_SIZE, HANDLE_SIZE, HANDLE_SIZE);

    glScalef(scale, scale, scale);

    glPushName(TRANSLATION);
    drawAxisLines();

    glEnable(GL_CULL_FACE);

    Util::myGlMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, black);
    Util::myGlMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, black);

    GLUquadricObj *obj = gluNewQuadric();

    if (scene->getXonly() || (scene->getConstrain() == CONSTRAIN_NONE)) {
        glLoadName(TRANSLATION_X);
        drawSphere(obj, 1.0f, 0.0f, 0.0f);
    }

    if (scene->getYonly() || (scene->getConstrain() == CONSTRAIN_NONE)) {
        glLoadName(TRANSLATION_Y);
        drawSphere(obj, 0.0f, 1.0f, 0.0f);
    }

    if (scene->getZonly() || (scene->getConstrain() == CONSTRAIN_NONE)) {
        glLoadName(TRANSLATION_Z);
        drawSphere(obj, 0.0f, 0.0f, 1.0f);
    }

    gluDeleteQuadric(obj);
    glPopName();

    glDisable(GL_CULL_FACE);

    glPopMatrix();
}

void
RenderState::draw6DlocalHandles(Node* node, float scale)
{
    float black[4] = { 0.0f, 0.0f, 0.0f, 1.0f };

    Scene *scene = node->getScene();

    glPushMatrix();
    node->transformForHandle(TRANSLATION);

    scale *= autoScale();

    glScalef(HANDLE_SIZE, HANDLE_SIZE, HANDLE_SIZE);

    glScalef(scale, scale, scale);

    glPushName(TRANSLATION);
    drawAxisLines();

    glEnable(GL_CULL_FACE);

    Util::myGlMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, black);
    Util::myGlMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, black);

    GLUquadricObj *obj = gluNewQuadric();

    if (scene->getXonly() || (scene->getConstrain() == CONSTRAIN_NONE)) {
        glLoadName(TRANSLATION_X);
        drawSphere(obj, 1.0f, 0.0f, 0.0f);
    }

    if (scene->getYonly() || (scene->getConstrain() == CONSTRAIN_NONE)) {
        glLoadName(TRANSLATION_Y);
        drawSphere(obj, 0.0f, 1.0f, 0.0f);
    }

    if (scene->getZonly() || (scene->getConstrain() == CONSTRAIN_NONE)) {
        glLoadName(TRANSLATION_Z);
        drawCone(obj, 0.0f, 0.0f, 1.0f);
    }

    gluDeleteQuadric(obj);
    glPopName();

    glDisable(GL_CULL_FACE);

    glPopMatrix();
}

void
RenderState::drawRocketHandles(Node* node, float scale)
{
    float black[4] = { 0.0f, 0.0f, 0.0f, 1.0f };

    Scene *scene = node->getScene();

    glPushMatrix();
    node->transformForHandle(TRANSLATION);

    scale *= autoScale();

    glScalef(HANDLE_SIZE, HANDLE_SIZE, HANDLE_SIZE);

    glScalef(scale, scale, scale);

    glPushName(TRANSLATION);
    drawAxisLines();

    glEnable(GL_CULL_FACE);

    Util::myGlMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, black);
    Util::myGlMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, black);

    GLUquadricObj *obj = gluNewQuadric();

    if (scene->getXonly() || (scene->getConstrain() == CONSTRAIN_NONE)) {
        glLoadName(TRANSLATION_X);
        drawCylinder(obj, 1.0f, 0.0f, 0.0f);
    }

    if (scene->getYonly() || (scene->getConstrain() == CONSTRAIN_NONE)) {
        glLoadName(TRANSLATION_Y);
        drawCylinder(obj, 0.0f, 1.0f, 0.0f);
    }

    if (scene->getZonly() || (scene->getConstrain() == CONSTRAIN_NONE)) {
        glLoadName(TRANSLATION_Z);
        drawCone(obj, 0.0f, 0.0f, 1.0f);
    }

    gluDeleteQuadric(obj);
    glPopName();

    glDisable(GL_CULL_FACE);

    glPopMatrix();
}

void
RenderState::drawHoverHandles(Node* node, float scale)
{
    float black[4] = { 0.0f, 0.0f, 0.0f, 1.0f };

    Scene *scene = node->getScene();

    glPushMatrix();
    node->transformForHandle(TRANSLATION);

    scale *= autoScale();

    glScalef(HANDLE_SIZE, HANDLE_SIZE, HANDLE_SIZE);

    glScalef(scale, scale, scale);

    glPushName(TRANSLATION);
    drawAxisLines();

    glEnable(GL_CULL_FACE);

    Util::myGlMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, black);
    Util::myGlMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, black);

    GLUquadricObj *obj = gluNewQuadric();

    if (scene->getXonly() || (scene->getConstrain() == CONSTRAIN_NONE)) {
        glLoadName(TRANSLATION_X);
        drawAntiCone(obj, 1.0f, 0.0f, 0.0f);
    }

    if (scene->getYonly() || (scene->getConstrain() == CONSTRAIN_NONE)) {
        glLoadName(TRANSLATION_Y);
        drawAntiCone(obj, 0.0f, 1.0f, 0.0f);
    }

    if (scene->getZonly() || (scene->getConstrain() == CONSTRAIN_NONE)) {
        glLoadName(TRANSLATION_Z);
        drawCone(obj, 0.0f, 0.0f, 1.0f);
    }

    gluDeleteQuadric(obj);
    glPopName();

    glDisable(GL_CULL_FACE);

    glPopMatrix();
}

void
RenderState::drawCenterHandles2(Node *node, float scale)
{
    float black[4] = { 0.0f, 0.0f, 0.0f, 1.0f };

    Scene *scene = node->getScene();

    glScalef(HANDLE_SIZE, HANDLE_SIZE, HANDLE_SIZE);

    glScalef(scale, scale, scale);

    glPushName(CENTER);
    drawAxisLines();

    glEnable(GL_CULL_FACE);

    Util::myGlMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, black);
    Util::myGlMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, black);

    GLUquadricObj *obj = gluNewQuadric();

    if (scene->getXonly() || (scene->getConstrain() == CONSTRAIN_NONE)) {
        glLoadName(CENTER_X);
        drawCone(obj, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f);
    }

    if (scene->getYonly() || (scene->getConstrain() == CONSTRAIN_NONE)) {
        glLoadName(CENTER_Y);
        drawCone(obj, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f);
    }

    if (scene->getZonly() || (scene->getConstrain() == CONSTRAIN_NONE)) {
        glLoadName(CENTER_Z);
        drawCone(obj, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f);
    }

    gluDeleteQuadric(obj);
    glPopName();

    glDisable(GL_CULL_FACE);
}

void
RenderState::drawCenterHandles(Node* node, float scale)
{
    glPushMatrix();
    node->transformForHandle(CENTER);
    scale *= autoScale();
    drawCenterHandles2(node, scale);
    glPopMatrix();
}

void
RenderState::drawScaleHandles(Node* node, float scale, const float *fscale)
{
    float black[4] = { 0.0f, 0.0f, 0.0f, 1.0f };

    Scene *scene = node->getScene();

    glPushMatrix();
    node->transformForHandle(SCALE_X);

    scale *= autoScale();

    glScalef(fscale[0], fscale[1], fscale[2]);

    glScalef(scale, scale, scale);

    glScalef(HANDLE_SIZE, HANDLE_SIZE, HANDLE_SIZE);

    glPushName(SCALE);
    drawAxisLines();

    glEnable(GL_CULL_FACE);

    Util::myGlMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, black);
    Util::myGlMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, black);

    if (scene->getXonly() || (scene->getConstrain() == CONSTRAIN_NONE)) {
        glLoadName(SCALE_X);
        drawScaleHandle(1.0f, 0.0f, 0.0f);
    }

    if (scene->getYonly() || (scene->getConstrain() == CONSTRAIN_NONE)) {
        glLoadName(SCALE_Y);
        drawScaleHandle(0.0f, 1.0f, 0.0f);
    }

    if (scene->getZonly() || (scene->getConstrain() == CONSTRAIN_NONE)) {
        glLoadName(SCALE_Z);
        drawScaleHandle(0.0f, 0.0f, 1.0f);
    }

    glPopName();

    glDisable(GL_CULL_FACE);

    glPopMatrix();
}

void
RenderState::drawProximityCenterHandles(Node* node, float scale)
{
    float black[4] = { 0.0f, 0.0f, 0.0f, 1.0f };

    Scene *scene = node->getScene();

    glPushMatrix();
    node->transformForHandle(PROXIMITY_CENTER);

    scale *= autoScale();

    glScalef(HANDLE_SIZE, HANDLE_SIZE, HANDLE_SIZE);

    glScalef(scale, scale, scale);

    glPushName(PROXIMITY_CENTER);
    drawAxisLines();

    glEnable(GL_CULL_FACE);

    Util::myGlMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, black);
    Util::myGlMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, black);

    GLUquadricObj *obj = gluNewQuadric();

    if (scene->getXonly() || (scene->getConstrain() == CONSTRAIN_NONE)) {
        glLoadName(PROXIMITY_CENTER_X);
        drawCone(obj, 1.0f, 0.0f, 0.0f);
    }

    if (scene->getYonly() || (scene->getConstrain() == CONSTRAIN_NONE)) {
        glLoadName(PROXIMITY_CENTER_Y);
        drawCone(obj, 0.0f, 1.0f, 0.0f);
    }

    if (scene->getZonly() || (scene->getConstrain() == CONSTRAIN_NONE)) {
        glLoadName(PROXIMITY_CENTER_Z);
        drawCone(obj, 0.0f, 0.0f, 1.0f);
    }

    gluDeleteQuadric(obj);
    glPopName();

    glDisable(GL_CULL_FACE);

    glPopMatrix();
}

void
RenderState::drawUniformScaleHandles(Node* node, float scale, 
                                     const float *fscale)
{
    float black[4] = { 0.0f, 0.0f, 0.0f, 1.0f };

    Scene *scene = node->getScene();

    glPushMatrix();
    node->transformForHandle(SCALE);

    scale *= autoScale();

    glScalef(fscale[0], fscale[1], fscale[2]);

    glScalef(scale, scale, scale);

    glScalef(HANDLE_SIZE, HANDLE_SIZE, HANDLE_SIZE);

    glPushName(SCALE);
    drawAxisLines();

    glEnable(GL_CULL_FACE);

    Util::myGlMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, black);
    Util::myGlMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, black);

    if (scene->getXonly() || (scene->getConstrain() == CONSTRAIN_NONE)) {
        glLoadName(UNIFORM_SCALE_X);
        drawScaleHandle(1.0f, 0.0f, 0.0f, true);
    }

    if (scene->getYonly() || (scene->getConstrain() == CONSTRAIN_NONE)) {
        glLoadName(UNIFORM_SCALE_Y);
        drawScaleHandle(0.0f, 1.0f, 0.0f, true);
    }

    if (scene->getZonly() || (scene->getConstrain() == CONSTRAIN_NONE)) {
        glLoadName(UNIFORM_SCALE_Z);
        drawScaleHandle(0.0f, 0.0f, 1.0f, true);
    }

    glPopName();

    glDisable(GL_CULL_FACE);

    glPopMatrix();
}

void
RenderState::drawScaleHandle(float x, float y, float z, bool uniform)
{
    float color[4] = {x, y, z, 1.0f};

    if (uniform)
        color[0] = color[1] = color[2] = 1.0f;

    Util::myGlMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, color);

    glPushMatrix();
    glRotatef(90.0f * x - 90.0f * y, y, x, z);
    glTranslatef(0.0f, 0.0f, 1.1f);
    Util::DrawBox(0.1f, 0.1f, 0.1f);
    glPopMatrix();
}

void
RenderState::drawCone(GLUquadricObj *obj, float x, float y, float z)
{
    drawCone(obj,x,y,z, x,y,z);
}

void
RenderState::drawCone(GLUquadricObj *obj, float x, float y, float z,
                      float r, float b, float g)
{
    float color[4] = {r, b, g, 1.0f};

    Util::myGlMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, color);
    Util::myGlColor3fv(color);

    glPushMatrix();

    glRotatef(90.0f * x - 90.0f * y, y, x, z);
    glTranslatef(0.0f, 0.0f, 1.0f);
    gluCylinder(obj, 0.07, 0.0, 0.2, 10, 10);
    gluQuadricOrientation(obj, (GLenum) GLU_INSIDE);
    gluDisk(obj, 0.0, 0.07, 10, 1);
    gluQuadricOrientation(obj, (GLenum) GLU_OUTSIDE);
    glPopMatrix();
}

void
RenderState::drawSphere(GLUquadricObj *obj, float x, float y, float z)
{
    drawSphere(obj,x,y,z, x,y,z);
}

void
RenderState::drawSphere(GLUquadricObj *obj, float x, float y, float z,
                                              float r, float b, float g)
{
    float color[4] = {r, b, g, 1.0f};

    Util::myGlMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, color);

    glPushMatrix();
    glRotatef(90.0f * x - 90.0f * y, y, x, z);
    glTranslatef(0.0f, 0.0f, 1.0f);
    gluSphere(obj, 0.07, 10, 10);
    glPopMatrix();
}

void
RenderState::drawAntiCone(GLUquadricObj *obj, float x, float y, float z)
{
    drawAntiCone(obj,x,y,z, x,y,z);
}

void
RenderState::drawAntiCone(GLUquadricObj *obj, float x, float y, float z,
                                                float r, float b, float g)
{
    float color[4] = {r, b, g, 1.0f};

    Util::myGlMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, color);

    glPushMatrix();
    glRotatef(90.0f * x - 90.0f * y, y, x, z);
    glTranslatef(0.0f, 0.0f, 1.0f);
    gluCylinder(obj, 0.0, 0.07, 0.2, 10, 10);
    glTranslatef(0.0f, 0.0f, 0.2f);
    gluDisk(obj, 0.0, 0.07, 10, 1);
    glPopMatrix();
}

void
RenderState::drawCylinder(GLUquadricObj *obj, float x, float y, float z)
{
    drawCylinder(obj,x,y,z, x,y,z);
}

void
RenderState::drawCylinder(GLUquadricObj *obj, float x, float y, float z,
                                                float r, float b, float g)
{
    float color[4] = {r, b, g, 1.0f};

    Util::myGlMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, color);

    glPushMatrix();
    glRotatef(90.0f * x - 90.0f * y, y, x, z);
    glTranslatef(0.0f, 0.0f, 1.0f);
    gluCylinder(obj, 0.07, 0.07, 0.2, 10, 10);
    gluQuadricOrientation(obj, (GLenum) GLU_INSIDE);
    gluDisk(obj, 0.0, 0.07, 10, 1);
    gluQuadricOrientation(obj, (GLenum) GLU_OUTSIDE);
    glTranslatef(0.0f, 0.0f, 0.2f);
    gluDisk(obj, 0.0, 0.07, 10, 1);
    glPopMatrix();
}

void
RenderState::drawRotationHandles(Node* node, float scale)
{
    Matrixd m = Matrixd::identity();
    drawJointOrRotationHandles(node, ROTATION, m, scale);
}

void
RenderState::drawJointOrRotationHandles(Node* node, int handle, Matrixd m, 
                                        float scale)
{
    float inc = 2 * M_PI / ROTATION_HANDLE_SEGMENTS;
    float i;
    GLint mode;

    glGetIntegerv(GL_RENDER_MODE, &mode);
    bool picking = mode == GL_SELECT;

    glPushMatrix();
    glPushAttrib(GL_LIGHTING | GL_LINE_SMOOTH);
    glDisable(GL_LIGHTING);
    glEnable(GL_LINE_SMOOTH);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);

    glPushName(handle);
    glMultMatrixd(m);
    node->transformForHandle(handle);
    if (handle != HUMAN_JOINT)
        scale *= autoScale();
   
    if (picking) {
        float mat[4][4];
        glGetFloatv(GL_MODELVIEW_MATRIX, (GLfloat *) mat);
        glPushMatrix();
        glLoadIdentity();
        glTranslatef(mat[3][0], mat[3][1], mat[3][2]);
        glScalef(HANDLE_SIZE * scale, HANDLE_SIZE * scale, HANDLE_SIZE * scale);
        Util::myGlColor4f(1.0f, 1.0f, 1.0f, 0.5f);
        glBegin(GL_POLYGON);

        for (i = 0.0f; i < 2 * M_PI; i += inc) {
            float s = (float) sin(i);
            float c = (float) cos(i);
            glVertex3f(c, s, 0.0f);
        }
        glEnd();
        glPopMatrix();
    }
    if (handle == CENTER) {
        drawCenterHandles2(node, scale);
        glPopMatrix();
        glPopName();
        glPopAttrib();
        return;
    }
    glScalef(HANDLE_SIZE * scale, HANDLE_SIZE * scale, HANDLE_SIZE * scale);

    glLoadName(handle);
    Util::myGlColor3f(1.0f, 0.0f, 0.0f);
    glBegin(GL_LINE_LOOP);

    for (i = 0.0f; i < 2 * M_PI; i += inc) {
        float s = (float) sin(i);
        float c = (float) cos(i);
        glVertex3f(0.0f, c, s);
    }
    glEnd();

    glLoadName(handle);
    Util::myGlColor3f(0.0f, 1.0f, 0.0f);
    glBegin(GL_LINE_LOOP);

    for (i = 0.0f; i < 2 * M_PI; i += inc) {
        float s = (float) sin(i);
        float c = (float) cos(i);
        glVertex3f(s, 0.0f, c);
    }
    glEnd();

    glLoadName(handle);
    Util::myGlColor3f(0.0f, 0.0f, 1.0f);
    glBegin(GL_LINE_LOOP);

    for (i = 0.0f; i < 2 * M_PI; i += inc) {
        float s = (float) sin(i);
        float c = (float) cos(i);
        glVertex3f(c, s, 0.0f);
    }
    glEnd();

    glPopMatrix();
    glPopName();
    glPopAttrib();
}

void
RenderState::drawJointHandles(Node* node, Matrixd m, float scale, int handle)
{
    drawJointOrRotationHandles(node, handle, m, scale);
}

void
RenderState::drawAxisLines()
{
    float white[4] = { 1.0f, 1.0f, 1.0f, 1.0f };

    glPushAttrib(GL_LIGHTING | GL_LINE_SMOOTH);
    glDisable(GL_LIGHTING);
    glEnable(GL_LINE_SMOOTH);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);

    Util::myGlColor3fv(white);
    glBegin(GL_LINES);
    glVertex3f(m_axesStart,          0,          0);
    glVertex3f(         1,           0,          0);
    glVertex3f(         0, m_axesStart,          0);
    glVertex3f(         0,           1,          0);
    glVertex3f(         0,           0, m_axesStart);
    glVertex3f(         0,           0,          1);
    glEnd();

    glDisable(GL_CULL_FACE);
    glPopAttrib();
}

void
RenderState::drawAxisLine()
{
    glPushAttrib(GL_LIGHTING | GL_LINE_SMOOTH);
    glDisable(GL_LIGHTING);
    glEnable(GL_LINE_SMOOTH);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);

    glBegin(GL_LINES);
    glVertex3f(0, 0, 0);
    glVertex3f(0, 0, 1);
    glEnd();

    glDisable(GL_CULL_FACE);
    glPopAttrib();
}

void         
RenderState::drawVectorHandle(int handleName, float x, float y, float z, 
                              float r, float b, float g)
{
    glPushName(NO_HANDLE);
    glPushAttrib(GL_LIGHTING | GL_LINE_SMOOTH);
    glDisable(GL_LIGHTING);
    glEnable(GL_LINE_SMOOTH);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);

    Util::myGlColor3f(r, g, b);
    glBegin(GL_LINES);
    glVertex3f(x * m_vectorStart, y * m_vectorStart, z * m_vectorStart);
    glVertex3f(x, y, z);
    glEnd();

    RenderState state;
    state.startDrawHandles();
    glLoadName(handleName);
    state.drawHandle(Vec3f(x, y, z));
    state.endDrawHandles();

    glDisable(GL_CULL_FACE);
    glPopAttrib();
    glPopName();
}

