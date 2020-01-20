/*
 * SceneView.cpp
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

#include <stdlib.h>

#include "stdafx.h"

#include "swt.h"
#include "SceneView.h"
#include "Scene.h"
#include "resource.h"

#ifdef HAVE_UPDATE_DEBUG
# include "SceneViewDebugOnUpdate.cpp"
#endif

static void resizeCB(void *data, int width, int height);
static void exposeCB(void *data, int x, int y, int width, int height);
static void keyCB(void *data, int key, int value, int x, int y,
                  int modifiers);
static void mouseCB(void *data, int x, int y, int modifiers);
static void enterCB(void *data, int value);
static void commandCB(void *data, void *id);
static void highlightCB(void *data, void *id);
static void closeCB(void *data);
static int  dropCB(void *data, int type, int x, int y, int modifiers);

SceneView::SceneView(Scene *scene, SWND wnd)
{
    m_scene = scene;
    m_scene->AddView(this);
    m_wnd = wnd;
    m_enabled = true;

    swSetCommandCallback(wnd, commandCB);
    swSetHighlightCallback(wnd, highlightCB);
    swSetResizeCallback(wnd, resizeCB);
    swSetEnterCallback(wnd, enterCB);
    swSetExposeCallback(wnd, exposeCB);
    swSetKeyCallback(wnd, keyCB);
    swSetMouseCallback(wnd, mouseCB);
    swSetCloseCallback(wnd, closeCB);
    swSetClientData(wnd, this);
}

SceneView::~SceneView()
{
    if (destroySelf())
       swDestroyWindow(m_wnd);
}

void
SceneView::RegisterDropTarget()
{
    swRegisterDropTarget(m_wnd, dropCB);
}

void
SceneView::UnregisterDropTarget()
{
    swUnregisterDropTarget(m_wnd);
}

static void
resizeCB(void *data, int width, int height)
{
    if (data == NULL)
        return;
    ((SceneView *) data)->OnSize(width, height);
}

static void
exposeCB(void *data, int x, int y, int width, int height)
{
    if (data == NULL)
        return;
    SceneView *view = (SceneView *) data;
    if (view->getEnabled())
        view->OnDraw(x, y, width, height);
}

static void
keyCB(void *data, int key, int value,
            int x, int y, int modifiers)
{
    if (data == NULL)
        return;
    if (key == SW_MOUSE1) {
        if (value) {
            ((SceneView *) data)->OnLButtonDown(x, y, modifiers);
        } else {
            ((SceneView *) data)->OnLButtonUp(x, y, modifiers);
        }
    } else if (key == SW_MOUSE2) {
        if (value) {
            ((SceneView *) data)->OnMButtonDown(x, y, modifiers);
        } else {
            ((SceneView *) data)->OnMButtonUp(x, y, modifiers);
        }
    } else if (key == SW_MOUSE3) {
        if (value) {
            ((SceneView *) data)->OnRButtonDown(x, y, modifiers);
        } else {
            ((SceneView *) data)->OnRButtonUp(x, y, modifiers);
        }
    } else if (value) {
        ((SceneView *) data)->OnKeyDown(key, x, y, modifiers);
    } else {
        ((SceneView *) data)->OnKeyUp(key, x, y, modifiers);
    }
}

static void
mouseCB(void *data, int x, int y, int modifiers)
{
    if (data == NULL)
        return;
    ((SceneView *) data)->OnMouseMove(x, y, modifiers);
}

static void
commandCB(void *data, void *id)
{
    if (data == NULL)
        return;
    ((SceneView *) data)->OnCommand(id);
}

static void
highlightCB(void *data, void *id)
{
    if (data == NULL)
        return;
    ((SceneView *) data)->OnHighlight(id);
}

static void
closeCB(void *data)
{
    if (data == NULL)
        return;
    ((SceneView *) data)->OnCommand(swToPtr(ID_DUNE_FILE_CLOSE));
}

#include <stdio.h>

static void
enterCB(void *data, int value)
{
    if (data == NULL)
        return;
    if (value) {
        ((SceneView *) data)->OnMouseEnter();
    } else {
        ((SceneView *) data)->OnMouseLeave();
    }
}

static int
dropCB(void *data, int type, int x, int y, int modifiers)
{
    if (data == NULL)
        return 0;

    int rc;

    switch (type) {
      case SW_DRAG_ENTER:
        rc = ((SceneView *) data)->OnDragEnter(x, y, modifiers);
        break;
      case SW_DRAG_OVER:
        rc = ((SceneView *) data)->OnDragOver(x, y, modifiers);
        break;
      case SW_DRAG_LEAVE:
        ((SceneView *) data)->OnDragLeave();
        rc = 0;
        break;
      case SW_DROP:
        rc = ((SceneView *) data)->OnDrop(x, y, modifiers);
        break;
      default:
        rc = 0;
        break;
    }
    return rc;
}
