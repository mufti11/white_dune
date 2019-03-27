/*
 * StatusBar.cpp
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
#include "StatusBar.h"
#include "swt.h"

StatusBar::StatusBar(Scene *scene, SWND wnd)
  : SceneView(scene, wnd)
{
    int width, height;

    m_bar = swCreateStatusBar(wnd);
    swGetSize(swStatusBarGetWindow(m_bar), &width, &height);
    swSetSize(wnd, width, height);
}

StatusBar::~StatusBar()
{
    swDestroyStatusBar(m_bar);
}

void
StatusBar::SetText(const char *text)
{
    if (text == NULL)
        return;
    swStatusBarSetText(m_bar, text);
}

void
StatusBar::OnSize(int width, int height)
{
    swSetSize(swStatusBarGetWindow(m_bar), width, height);
}
