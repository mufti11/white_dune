/*
 * ToolbarWindow.cpp
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

#include "stdafx.h"
#include "ToolbarWindow.h"
#include "IconSize.h"

#include <swt.h>
#include <string.h>

#define BUFSIZE 1024

static const char *tooltipCB(void *data, int id)
{
    return ((ToolbarWindow *) data)->OnTooltipCB(id);
}

ToolbarWindow::ToolbarWindow(Scene *scene, SWND wnd, MainWindow *mainWindow)
  : SceneView(scene, wnd)
{
    m_mainWindow = mainWindow;
}

ToolbarWindow::~ToolbarWindow()
{
    int n = m_toolbars.size();

    for (int i = 0; i < n; i++) {
        swDestroyToolbar(m_toolbars[i]);
        swDestroyBitmap(m_bitmaps[i]);
    }
    m_toolbars.resize(0);
    m_bitmaps.resize(0);
}

STOOLBAR
ToolbarWindow::LoadToolbar(int id, int count, const int *buttonInfo)
{
    int face = swGetWindowColor(m_wnd, SW_COLOR_FACE);
    int mapFrom = 0x808000;
    int mapTo = face;
    SBITMAP bitmap;
    STOOLBAR tb;

    bitmap = swLoadMappedBitmap(m_wnd, id, &mapFrom, &mapTo, 1);
    m_bitmaps.append(bitmap);
    tb = swCreateToolbar(m_wnd, bitmap, ICON_SIZE_X, ICON_SIZE_Y, 
                         count, buttonInfo);
    swToolbarSetTooltipCallback(tb, tooltipCB);
    swToolbarSetClientData(tb, this);
    m_toolbars.append(tb);
    Layout();
    return tb;
}

void
ToolbarWindow::ShowToolbar(STOOLBAR tb, bool show)
{
    SWND wnd = swToolbarGetWindow(tb);

    if (show) {
        swShowWindow(wnd);
    } else {
        swHideWindow(wnd);
    }
    Layout();
}

void
ToolbarWindow::Layout()
{
    int n = m_toolbars.size();
    int width, height, y = 0;

    for (int i = 0; i < n; i++) {
        SWND wnd = swToolbarGetWindow(m_toolbars[i]);
        if (swIsVisible(wnd)) {
            swSetPosition(wnd, 0, y);
            swGetSize(wnd, &width, &height);
            y += height;
        }
    }
    swGetSize(m_wnd, &width, &height);
    if (y > 0) {
        swSetSize(m_wnd, width, y);
        swShowWindow(m_wnd);
    } else {
        swHideWindow(m_wnd);
    }
}

void
ToolbarWindow::OnSize(int width, int height)
{
    int n = m_toolbars.size();
    int oldWidth, oldHeight;

    for (int i = 0; i < n; i++) {
        SWND window = swToolbarGetWindow(m_toolbars[i]);
        swGetSize(window, &oldWidth, &oldHeight);
        swSetSize(window, width, oldHeight);
        swInvalidateRect(window, oldWidth-1, 0, 1, oldHeight);
    }
}

void
ToolbarWindow::OnHighlight(int id)
{
    m_mainWindow->OnHighlight(swToPtr(id));
}

const char *
ToolbarWindow::OnTooltipCB(int id)
{
    static char buf[BUFSIZE];

    buf[0] = '\0';
    swLoadString(id, buf, BUFSIZE);
    char *b = strchr(buf, '\n');
    if (b) {
        return b + 1;
    } else {
        return "";
    }
}

void
ToolbarWindow::OnCommand(void *id)
{
    // send it to the enclosing parent
    m_mainWindow->OnCommand(id);
}
