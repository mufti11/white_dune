/*
 * SceneView.h
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

#ifndef _SCENE_VIEW_H
#define _SCENE_VIEW_H

#include "DuneApp.h"
#include "Scene.h"

class Hint;
class Scene;

#include "swttypedef.h"

#ifdef HAVE_UPDATE_DEBUG
# define CASE_UPDATE(x) case x: \
    debugOnUpdate(sender, type, hint);
#else
# define CASE_UPDATE(x) case x:
#endif

class SceneView {
public:
                        SceneView(Scene *scene, SWND parent);
    virtual            ~SceneView();
    virtual void        DeleteView() {};

    virtual const char *getName(void) const=0;

    virtual void        OnDraw(int x, int y, int width, int height) {}
    virtual void        OnSize(int width, int height) {}
    virtual void        OnMouseEnter() {m_scene->setHasFocus();}
    virtual void        OnMouseLeave() {m_scene->deleteHasFocus();}
    virtual void        OnKeyDown(int key, int x, int y, int modifiers) {}
    virtual void        OnKeyUp(int key, int x, int y, int modifiers) {}
    virtual void        OnLButtonDown(int x, int y, int modifiers) {}
    virtual void        OnLButtonUp(int x, int y, int modifiers) {}
    virtual void        OnMButtonDown(int x, int y, int modifiers) {}
    virtual void        OnMButtonUp(int x, int y, int modifiers) {}
    virtual void        OnRButtonDown(int x, int y, int modifiers) {}
    virtual void        OnRButtonUp(int x, int y, int modifiers) {}
    virtual void        OnMouseMove(int x, int y, int modifiers) {}
    virtual void        OnCommand(void *id) {}
    virtual void        OnHighlight(void *id) {}
    virtual int         OnDragEnter(int x, int y, int modifiers) { return 0; }
    virtual int         OnDragOver(int x, int y, int modifiers) { return 0; }
    virtual void        OnDragLeave() {}
    virtual int         OnDrop(int x, int y, int modifiers) { return 0; }
    virtual void        OnUpdate(SceneView *sender, int type, Hint *hint) {}
    virtual void        DeleteLastSelection(void) {}

    Scene              *GetScene() const { return m_scene; }
    void                SetScene(Scene *scene) { m_scene = scene; }
    SWND                GetWindow() const { return m_wnd; }

    void                RegisterDropTarget();
    void                UnregisterDropTarget();

    bool                getEnabled() { return m_enabled; }
    void                setEnabled(bool enabled) { m_enabled=enabled; }

    virtual void        zoomIn() {}
    virtual void        zoomOut() {}
    virtual void        unZoom() {}
    virtual bool        canZoomIn() { return false; }

    virtual void        Initialize() {}

    virtual void        moveToTop(Node *node, Node* ignoreNode = NULL) {}
    virtual void        moveRoutesToTop(Node *node) {}

    virtual bool        isFieldView(void) { return false; }

#ifdef HAVE_UPDATE_DEBUG
    void                debugOnUpdate(SceneView* sender, int type, Hint *hint);
#endif

protected:
    virtual bool        destroySelf() { return true; }

protected:
    Scene              *m_scene;
    SWND                m_wnd;
    bool                m_enabled;
};

#endif // _SCENE_VIEW_H
