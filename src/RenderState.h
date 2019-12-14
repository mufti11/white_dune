/*
 * RenderState.h
 *
 * Copyright (C) 2000 Stephen F. White
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

#include "Matrix.h"
#include "swttypedef.h"


#ifndef INT_MAX
# include <limits.h>
#endif

#define HANDLE_SIZE 1.25f

enum {
    NO_HANDLE = 0x8ffe0,
    TRANSLATION,
    TRANSLATION_X,
    TRANSLATION_Y,
    TRANSLATION_Z,
    ROTATION,
    ROTATION_X,
    ROTATION_Y,
    ROTATION_Z,
    SCALE_X,
    SCALE_Y,
    SCALE_Z,
    SCALE,
    UNIFORM_SCALE_X,
    UNIFORM_SCALE_Y,
    UNIFORM_SCALE_Z,
    CENTER_X,
    CENTER_Y,
    CENTER_Z,
    CENTER,
    HUMAN_JOINT,
    DIRECTION,
    AXIS1,
    AXIS2,
    AXIS3,
    PROXIMITY_CENTER,
    PROXIMITY_CENTER_X,
    PROXIMITY_CENTER_Y,
    PROXIMITY_CENTER_Z
};

class RenderState {
public:
                RenderState();

    void        startDrawHandles();
    void        drawHandle(const Vec3f &point);
    void        drawHandle(const Vec3d &point);
    void        setHandleColor(Scene *scene, int handle);
    void        endDrawHandles();

    double      autoScale(void);

    void        drawCone(GLUquadricObj *obj, float x, float y, float z);
    void        drawCone(GLUquadricObj *obj, float x, float y, float z,
                                             float r, float b, float g);
    void        drawSphere(GLUquadricObj *obj, float x, float y, float z);
    void        drawSphere(GLUquadricObj *obj, float x, float y, float z,
                                               float r, float b, float g);
    void        drawAntiCone(GLUquadricObj *obj, float x, float y, float z);
    void        drawAntiCone(GLUquadricObj *obj, float x, float y, float z,
                                                 float r, float b, float g);
    void        drawCylinder(GLUquadricObj *obj, float x, float y, float z);
    void        drawCylinder(GLUquadricObj *obj, 
                                     float x, float y, float z,
                                     float r, float b, float g);
    void        drawScaleHandle(float x, float y, float z, 
                                bool uniform = false);
    void        drawAxisLine();
    void        drawAxisLines();
    void        drawTranslationHandles(Node *node, float scale);
    void        drawRotationHandles(Node* node, float scale = 1.0f);
    void        drawDirectionHandles(Node* node, float scale = 1.0f);
    void        drawScaleHandles(Node *node, float scale, const float *fscale);
    void        drawUniformScaleHandles(Node *node, float scale,
                                        const float *fscale);
    void        drawCenterHandles2(Node *node, float scale);
    void        drawCenterHandles(Node *node, float scale);
    void        drawProximityCenterHandles(Node *node, float scale);
    void        draw6DHandles(Node *node, float scale);
    void        draw6DlocalHandles(Node *node, float scale);
    void        drawRocketHandles(Node *node, float scale);
    void        drawHoverHandles(Node *node, float scale);

    void        drawJointOrRotationHandles(Node* node, int handle, Matrixd m, 
                                           float scale);

    void        drawJointHandles(Node* node, Matrixd m, float scale, 
                                 int handle = HUMAN_JOINT);

    void        drawVectorHandle(int handleName, float x, float y, float z,
                                                 float r, float b, float g);

    void        setAxesStart(float start) { m_axesStart = start; }
    void        setVectorStart(float start) { m_vectorStart = start; }

    Vec3f       project(const Vec3f &point);
    Vec3d       project(const Vec3d &point);

private:
    Matrixd     m_modelviewMatrix;
    Matrixd     m_projectionMatrix;
    double      m_viewport[4];
    float       m_axesStart;
    float       m_vectorStart;
};

