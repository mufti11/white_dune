/*
 * CheckBoxWindow.cpp
 *
 * Copyright (C) 2005 J. "MUFTI" Scheurich
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

#include "CheckBoxWindow.h"
#include "stdafx.h"
#include <ctype.h>

// for debugging
// #include "swt/motif/swt_structs.h"

#define X_SPACING 2
#define Y_SPACING 2

static SWND canvas;
static SWND scroll;
static SWND parent_window;

#define XPOS 20
#define YPOS 100
#ifdef _WIN32
# define XMAX 300
# define YMAX 240
#else
# define XMAX 388
# define YMAX 300
#endif
#define SCROLL_SIZE 5

static int ymax;

static void
CheckBoxWindowRedraw(void* data)
{
    SDC dc = swCreateDC(canvas);
    swSetFGColor(dc, swGetWindowColor(parent_window, SW_COLOR_WINDOW_BG));
    CheckBoxWindow* window = (CheckBoxWindow *) data;
    window->accountYmax();
    swFillRect(dc, 1, 1, XMAX - 2, ymax - 2);
    swSetFGColor(dc, 0);
    swDrawLine(dc,        0,        0, XMAX - 1,        0);
    swDrawLine(dc,        0, ymax - 1, XMAX - 1, ymax - 1);
    swDrawLine(dc,        0,        0,        0, ymax - 1);
    swDrawLine(dc, XMAX - 1,        0, XMAX - 1, ymax - 1);
    window->drawInterface(dc);
    swDestroyDC(dc);
}

static void
CheckBoxWindowExposeCallback(void *data, int x, int y, int width, int height)
{
    CheckBoxWindowRedraw(data);
}

#ifdef _WIN32
static void
CheckBoxWindowEnterCallback(void *data, int command)
{
    CheckBoxWindowRedraw(data);
}
#endif

CheckBoxWindow::CheckBoxWindow(void)
{
    scroll = NULL;
    canvas = NULL;
}

void CheckBoxWindow::initCheckBoxWindow(SWND parent, SWND dlg)
{
    parent_window = parent;
    ymax = YMAX;
    m_fontHeight = swGetFontHeight(swGetDefaultFont());
    accountYmax();

    scroll = swCreateScrolledWindow(XPOS, YPOS, XMAX - XPOS, YMAX - YPOS, dlg);
    canvas = swCreateCanvas("Interaction dialog", 0, 0, XMAX, YMAX, scroll);
    swScrolledWindowSetChild(scroll, canvas);
    swSetScrollSizes(scroll, XMAX - XPOS - SCROLL_SIZE, 
                             YMAX - YPOS - SCROLL_SIZE);

    swSetClientData(canvas, this);
#ifdef _WIN32
    swSetEnterCallback(canvas, CheckBoxWindowEnterCallback);
#else
    swSetExposeCallback(canvas, CheckBoxWindowExposeCallback);
#endif
    swInvalidateWindow(canvas);

    swShowWindow(canvas);
}

CheckBoxWindow::~CheckBoxWindow()
{
#ifdef _WIN32
    swSetEnterCallback(canvas, NULL);
#else
    swSetExposeCallback(canvas, NULL);
#endif
    for (size_t i = 0 ; i < m_buttons.size(); i++)
        swDestroyWindow(m_buttons[i]);
    if (canvas)
        swDestroyWindow(canvas);
    if (scroll)
        swDestroyWindow(scroll); 
}

bool CheckBoxWindow::isValid(void)
{
    if ((scroll == NULL) && (canvas = NULL))
        return false;
    return true;
}

void 
CheckBoxWindow::drawInterface(SDC dc)
{
    int y = 0;
    for (size_t i = 0 ; i < m_strings.size(); i++) {
        y += Y_SPACING ;
  
        m_buttonsPressed[i] = false;
        if (i < m_buttons.size()) {
            if (swGetCheck(m_buttons[i]) == 0)
               m_buttonsPressed[i] = false;
            else
               m_buttonsPressed[i] =  true;
            swDestroyWindow(m_buttons[i]);
        }
        if (m_initButtonsPressed[i]) {
            m_buttonsPressed[i] = true;
            m_initButtonsPressed[i] = false;
        }
        m_buttons[i]=swCreateCheckBox("", X_SPACING, y + Y_SPACING, 
                                     m_fontHeight, m_fontHeight, canvas);
        swSetCheck(m_buttons[i], m_buttonsPressed[i] ? 1 : 0);
        y += m_fontHeight;
        swDrawText(dc, 2 * X_SPACING + m_fontHeight, y, (const char*) 
                   m_strings[i]);
    }
}

void 
CheckBoxWindow::accountYmax()
{
    ymax = m_strings.size() * (Y_SPACING + m_fontHeight) + 2 * Y_SPACING; 
    if (ymax > YMAX - YPOS - SCROLL_SIZE) {
        swSetSize(canvas, XMAX, ymax);
        swSetScrollSizes(scroll, XMAX - XPOS - SCROLL_SIZE - 1, ymax);
    } else {
        swSetSize(canvas, XMAX, ymax);
        swSetScrollSizes(scroll, XMAX, ymax);
    }
}   

void
CheckBoxWindow::resize0(void)
{
    m_strings.resize(0);
    for (size_t i = 0; i < m_buttons.size(); i++)
        swDestroyWindow(m_buttons[i]);
    m_buttons.resize(0);
    m_buttonsPressed.resize(0);
    m_initButtonsPressed.resize(0);
}

void
CheckBoxWindow::invalidateWindow(void)
{
    swInvalidateWindow(canvas); 
    swShowWindow(canvas);
}

