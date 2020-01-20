/*
 * Scene3DView.h
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
#include "Quaternion.h"
#include "Matrix.h"
#include "SceneView.h"
#include "InputDevice.h"
#include "swttypedef.h"

class Scene;
class SFVec3f;

class Scene3DView : public SceneView
{
public:
                        Scene3DView(Scene *scene, SWND parent);
    virtual            ~Scene3DView();
    virtual void        OnDraw(int x, int y, int width, int height, 
                               bool update = false);
    virtual void        OnKeyDown(int key, int x, int y, int modifiers);
    virtual void        OnKeyUp(int key, int x, int y, int modifiers);
    virtual void        OnMouseEnter();
    virtual void        OnMouseLeave();
    virtual void        OnMouseMove(int x, int y, int modifiers);
    virtual void        OnLButtonDown(int x, int y, int modifiers);
    virtual void        OnMButtonDown(int x, int y, int modifiers);
    virtual void        OnRButtonDown(int x, int y, int modifiers);
    virtual void        OnLButtonUp(int x, int y, int modifiers);
    virtual void        OnMButtonUp(int x, int y, int modifiers);
    virtual void        OnRButtonUp(int x, int y, int modifiers);
    virtual void        OnUpdate(SceneView *sender, int type, Hint *hint);
    virtual void        OnSize(int width, int height);

    unsigned int        getHit(int x, int y);

    const char         *getName(void) const { return "Scene3DView"; }

    void                startWalking(bool walkForward);
    void                startWalking(void);
    void                stopWalking(void);
    int                 walk();

    int                 readInputDevice(void);
    void                Transform3D(const Path* path, InputDevice* inputDevice);
    void                Navigate3D(InputDevice * inputDevice);
    void                Handle3D(const Path* path,InputDevice* inputDevice, 
                                 int handle);
    void                drawViewPort(Node* viewport, int count, 
                                     bool update = false);

protected:
    Quaternion         &getQuat(const Path* path);
    float               constrainLine(float x1, float y1, float z1,
                                      float x2, float y2, float z2) const;
    void                constrainPlane(float x1, float y1, float z1,
                                       float x2, float y2, float z2,
                                       float *x, float *y) const;
    Vec3f               constrainSphere(const Vec3f &euler, float dx, float dy,
                                        const Matrix &mat);

protected:
    SDC         m_dc;
    SGLC        m_glc;
    bool        m_trackMouse;
    int         m_trackX;
    int         m_trackY;
    int         m_rubberBandX;
    int         m_rubberBandY;

    Vec3f       m_offset;
    bool        m_backedUp;
    SFVec3f    *m_oldCameraPos;
    Vec3f       m_trackBase;
    SCURSOR     m_cursorArrow;
    SCURSOR     m_cursorZoom;
    SCURSOR     m_cursorRotate;
    SCURSOR     m_cursorRoll;
    SCURSOR     m_cursorMove;
    SCURSOR     m_cursorNone;
    Matrix      m_handleMatrix;

    bool        m_button1down;
    bool        m_button2down;
    bool        m_button3down;

    STIMER      m_inputDeviceTimer;

    STIMER      m_walkTimer;
    Vec3f       m_walk;
    bool        m_walking;
    bool        m_walkForward;

    int         m_width;
    int         m_height;

    bool        m_fieldEdit;

    int         m_mouseX;
    int         m_mouseY;

    bool        m_hitHandle;
};

