/*
 * PanedWindow.cpp
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
#include "swt.h"
#include "PanedWindow.h"

#define SASH_WIDTH 5

enum {
    NORMAL,
    TRACK_TOP,
    TRACK_BOTTOM,
    TRACK_LEFT,
    TRACK_RIGHT
};

PanedWindow::PanedWindow(Scene *scene, SWND wnd, bool sashes)
  : SceneView(scene, wnd)
{
    m_sashes = sashes;
    m_top = m_left = m_bottom = m_right = m_center = NULL;
    m_state = NORMAL;
    m_cursorH = swLoadCursor(SW_CURSOR_DBL_ARROW_HORZ);
    m_cursorV = swLoadCursor(SW_CURSOR_DBL_ARROW_VERT);
    m_cursorNormal = swLoadCursor(SW_CURSOR_ARROW);
}

PanedWindow::~PanedWindow()
{
    swDestroyCursor(m_cursorH);
    swDestroyCursor(m_cursorV);
    swDestroyCursor(m_cursorNormal);
}

void
PanedWindow::OnSize(int width, int height)
{
    Layout();
}

void
PanedWindow::Layout(bool maximize)
{
    int width, height, rw, rh;
    int w, h, left = 0, top = 0;
#ifdef _WIN32
    if (maximize) {
        swGetScreenSize(&width, &height);
        swSetSize(m_wnd,width,height);
    } else {
        swGetSize(m_wnd, &width, &height);
        // otherwise problems with iconify/deiconify ... 
        if ((width==0) && (height==0))
            swGetScreenSize(&width, &height);
    }
//    return;
#else
    swGetSize(m_wnd, &width, &height);
#endif
    bool showTop = m_top && swIsVisible(m_top->GetWindow());
    bool showBottom = m_bottom && swIsVisible(m_bottom->GetWindow());
    bool showLeft = m_left && swIsVisible(m_left->GetWindow());
    bool showRight = m_right && swIsVisible(m_right->GetWindow());
    bool showCenter = m_center && swIsVisible(m_center->GetWindow());

    swInvalidateRect(m_wnd, 0, 0, width, height);
    rw = width;  
    rh = height;

    if (showLeft && m_sashes) rw -= SASH_WIDTH;
    if (showRight && m_sashes) rw -= SASH_WIDTH;
    if (rw < 0) rw = 0;

    if (showTop && m_sashes) rh -= SASH_WIDTH;
    if (showBottom && m_sashes) rh -= SASH_WIDTH;
    if (rh < 0) rh = 0;

    if (showTop) {
        swGetSize(m_top->GetWindow(), &w, &h);
        if (h > rh) h = rh;
        swSetSize(m_top->GetWindow(), width, h);
        top = h;
        if (m_sashes) top += SASH_WIDTH;
        rh -= h;
        if (rh < 1) rh = 1;
    }
    if (showBottom) {
        swGetSize(m_bottom->GetWindow(), &w, &h);
        if (h > rh) h = rh;
        swMoveWindow(m_bottom->GetWindow(), 0, height - h, width, h);
        rh -= h;
        if (rh < 1) rh = 1;
    }
    if (showLeft) {
        swGetSize(m_left->GetWindow(), &w, &h);
        if (w > rw) w = rw;
        swMoveWindow(m_left->GetWindow(), 0, top, w, rh);
        left = w;
        if (m_sashes) left += SASH_WIDTH;
        rw -= w;
        if (rw < 1) rw = 1;
    }
    if (showRight) {
        swGetSize(m_right->GetWindow(), &w, &h);
        if (w > rw) w = rw;
        swMoveWindow(m_right->GetWindow(), width - w, top, w, rh);
        rw -= w;
        if (rw < 1) rw = 1;
    }
    if (showCenter) {
        swMoveWindow(m_center->GetWindow(), left, top, rw, rh);
    }
}

void
PanedWindow::SetPane(SceneView *pane, int direction)
{
    switch (direction) {
      case PW_TOP:
        m_top = pane;
        break;
      case PW_BOTTOM:
        m_bottom = pane;
        break;
      case PW_LEFT:
        m_left = pane;
        break;
      case PW_RIGHT:
        m_right = pane;
        break;
      case PW_CENTER:
        m_center = pane;
        break;
    }
}

int
PanedWindow::HitTest(int x, int y)
{
    int width, height;
    int twidth = 0, theight = 0;
    int bwidth = 0, bheight = 0;
    int lwidth = 0, lheight = 0;
    int rwidth = 0, rheight = 0;

    swGetSize(m_wnd, &width, &height);
    if (m_top) swGetSize(m_top->GetWindow(), &twidth, &theight);
    if (m_bottom) swGetSize(m_bottom->GetWindow(), &bwidth, &bheight);
    if (m_left) swGetSize(m_left->GetWindow(), &lwidth, &lheight);
    if (m_right) swGetSize(m_right->GetWindow(), &rwidth, &rheight);
    if (m_top && y >= theight && y < theight + SASH_WIDTH) {
        return TRACK_TOP;
    } else if (m_bottom && y >= height - bheight - SASH_WIDTH && y < height - bheight) {
        return TRACK_BOTTOM;
    } else if (m_left && x >= lwidth && x < lwidth + SASH_WIDTH) {
        return TRACK_LEFT;
    } else if (m_right && x >= width - rwidth - SASH_WIDTH && x < width - rwidth) {
        return TRACK_RIGHT;
    } else {
        return NORMAL;
    }
}

void
PanedWindow::UpdateCursor(int state)
{
#ifndef _WIN32
    if (!TheApp->useStereo()) {
        if (state == TRACK_TOP || state == TRACK_BOTTOM) {
            swSetCursor(m_wnd, m_cursorV);
        } else if (state == TRACK_LEFT || state == TRACK_RIGHT) {
            swSetCursor(m_wnd, m_cursorH);
        } else {
            swSetCursor(m_wnd, m_cursorNormal);
        }
    }
#endif
}

void
PanedWindow::OnMouseMove(int x, int y, int modifiers)
{
    int width, height;
    int lwidth = 0, lheight = 0;

    if (!m_sashes) return;

    if (m_state == NORMAL) {
        UpdateCursor(HitTest(x, y));
    } else {
        swGetSize(m_wnd, &width, &height);
        x = CLAMP(x, 1, width - 1);
        y = CLAMP(y, 1, height - 1);
        if (m_left) swGetSize(m_left->GetWindow(), &lwidth, &lheight);
        if (m_state == TRACK_TOP) {
            swSetSize(m_top->GetWindow(), width, y);
        } else if (m_state == TRACK_BOTTOM) {
            swSetSize(m_bottom->GetWindow(), width, height - y);
        } else if (m_state == TRACK_LEFT) {
            swSetSize(m_left->GetWindow(), x, lheight);
        } else if (m_state == TRACK_RIGHT) {
            int rwidth, rheight;
            swGetSize(m_right->GetWindow(), &rwidth, &rheight);
            swSetSize(m_right->GetWindow(), width - x, rheight);
        }
        Layout();
    }
}

void
PanedWindow::OnMouseLeave()
{
    UpdateCursor(m_state);
}

void
PanedWindow::OnLButtonDown(int x, int y, int modifiers)
{
    if (!m_sashes) return;

    m_state = HitTest(x, y);
    UpdateCursor(m_state);
    if (m_state != NORMAL) swSetCapture(m_wnd);
}

void
PanedWindow::OnLButtonUp(int x, int y, int modifiers)
{
    if (!m_sashes) return;

    m_state = NORMAL;
    swReleaseCapture(m_wnd);
    swSetCursor(m_wnd, m_cursorNormal);
}

void
PanedWindow::OnDraw(int x, int y, int width, int height)
{
    if (!m_sashes) return;

    int top = 0;
    SDC dc = swCreateDC(m_wnd);

    swGetSize(m_wnd, &width, &height);
    if (m_top && swIsVisible(m_top->GetWindow())) {
        int w, h;
        swGetSize(m_top->GetWindow(), &w, &h);
        DrawHSash(dc, 0, h, width, SASH_WIDTH);
        top = h + SASH_WIDTH;
    }
    if (m_bottom && swIsVisible(m_bottom->GetWindow())) {
        int w, h;
        swGetSize(m_bottom->GetWindow(), &w, &h);
        DrawHSash(dc, 0, height - h - SASH_WIDTH, width, SASH_WIDTH);
    }
    if (m_left && swIsVisible(m_left->GetWindow())) {
        int w, h;
        swGetSize(m_left->GetWindow(), &w, &h);
        DrawVSash(dc, w, top, SASH_WIDTH, h);
    }
    if (m_right && swIsVisible(m_right->GetWindow())) {
        int w, h;
        swGetSize(m_right->GetWindow(), &w, &h);
        DrawVSash(dc, width - w - SASH_WIDTH, top, SASH_WIDTH, h);
    }
    swDestroyDC(dc);
}

void
PanedWindow::DrawVSash(SDC dc, int x, int y, int w, int h)
{
    swSetFGColor(dc, swGetWindowColor(m_wnd, SW_COLOR_TSHADOW));
    swDrawLine(dc, x, y, x, y + h - 1);
    swSetFGColor(dc, swGetWindowColor(m_wnd, SW_COLOR_BSHADOW));
    swDrawLine(dc, x + w - 1, y, x + w - 1, y + h - 1);
    swSetFGColor(dc, swGetWindowColor(m_wnd, SW_COLOR_FACE));
    swFillRect(dc, x + 1, y, w - 2, h - 1);
}

void
PanedWindow::DrawHSash(SDC dc, int x, int y, int w, int h)
{
    swSetFGColor(dc, swGetWindowColor(m_wnd, SW_COLOR_TSHADOW));
    swDrawLine(dc, x, y, x + w - 1, y);
    swSetFGColor(dc, swGetWindowColor(m_wnd, SW_COLOR_BSHADOW));
    swDrawLine(dc, x, y + h - 1, x + w - 1, y + h - 1);
    swSetFGColor(dc, swGetWindowColor(m_wnd, SW_COLOR_FACE));
    swFillRect(dc, x, y + 1, w - 1, h - 2);
}
