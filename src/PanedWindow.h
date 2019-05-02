/*
 * PanedWindow.h
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

#ifndef _PANED_WINDOW_H
#define _PANED_WINDOW_H

#ifndef _SCENE_VIEW_H
#include "SceneView.h"
#endif

#ifndef _LIST_H
#include "List.h"
#endif

#include "swttypedef.h"

class PanedWindow : public SceneView {
public:
                        PanedWindow(Scene *scene, SWND parent, bool sashes);
    virtual            ~PanedWindow();

    virtual void        OnSize(int width, int height);
    virtual void        OnMouseMove(int x, int y, int modifiers);
    virtual void        OnLButtonDown(int x, int y, int modifiers);
    virtual void        OnLButtonUp(int x, int y, int modifiers);
    virtual void        OnDraw(int x, int y, int width, int height,
                               bool update = false);
    virtual void        OnMouseLeave();

    const char         *getName(void) const { return "PanedWindow"; }

    void                SetPane(SceneView *pane, int position);
    void                Layout(bool maximize=false);

protected:
    void                UpdateCursor(int state);
    void                DrawVSash(SDC dc, int x, int y, int width, int height);
    void                DrawHSash(SDC dc, int x, int y, int width, int height);
    int                 HitTest(int x, int y);

private:
    SceneView          *m_top;
    SceneView          *m_bottom;
    SceneView          *m_left;
    SceneView          *m_right;
    SceneView          *m_center;
    bool                m_sashes;

    int                 m_state;

    SCURSOR             m_cursorH, m_cursorV, m_cursorNormal;
};

enum {
    PW_TOP,
    PW_BOTTOM,
    PW_LEFT,
    PW_RIGHT,
    PW_CENTER
};

#endif /* _PANED_WINDOW_H */
