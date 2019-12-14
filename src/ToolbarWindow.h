/*
 * ToolbarWindow.h
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

#include "SceneView.h"
#include "Array.h"
#include "swttypedef.h"

class ToolbarWindow;

class ToolbarWindow : public SceneView {
public:
                        ToolbarWindow(Scene *scene, SWND wnd,
                                      MainWindow *mainWindow);
    virtual            ~ToolbarWindow();

    virtual void        OnSize(int width, int height);
    virtual void        OnCommand(void *id);
    virtual void        OnHighlight(int id);

    const char         *getName(void) const { return "ToolbarWindow"; }

    STOOLBAR            LoadToolbar(int id, int count, const int *buttonInfo);
    void                ShowToolbar(STOOLBAR toolbar, bool show);

    const char         *OnTooltipCB(int id);

protected:
    void                Layout();

private:
    MainWindow         *m_mainWindow;
    MyArray<STOOLBAR>   m_toolbars;
    MyArray<SBITMAP>    m_bitmaps;
};
