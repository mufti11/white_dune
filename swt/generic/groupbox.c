/*
 * groupbox.c
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

#include <stdio.h>
#include <stdlib.h>

#include "swt.h"

static void expose(void *data, int x, int y, int width, int height);

extern SWND
swCreateGroupBox(const char *title, int x, int y, int width, int height,
                 SWND parent)
{
    SWND w;

    if (!parent) return NULL;

    w = swCreateCanvas(title, x, y, width, height, parent);
    swSetExposeCallback(w, expose);
    swSetClientData(w, w);
    return w;
}

static void
expose(void *data, int x, int y, int width, int height)
{
    SWND wnd = (SWND) data;
    SDC dc = swCreateDC(wnd);
    SFONT font = swGetDefaultFont();
    int w, h;
    char title[1024];
    int theight = swGetFontAscent(font);
    int twidth = swGetStringWidth(font, "m");

    /* FIXME:  for now, group box has to erase its background (for motif) */
    swSetFGColor(dc, swGetWindowColor(wnd, SW_COLOR_WINDOW_BG));
    swFillRect(dc, x, y, width, height);
    swGetTitle(wnd, title, 1024);
    swGetSize(wnd, &w, &h);
    swSetFGColor(dc, swGetWindowColor(wnd, SW_COLOR_TSHADOW));
    swDrawRect(dc, twidth/2+1, theight/2+1, w - twidth, h - theight);
    swSetFGColor(dc, swGetWindowColor(wnd, SW_COLOR_BSHADOW));
    swDrawRect(dc, twidth/2, theight/2, w - twidth, h - theight);
    swSetFGColor(dc, swGetWindowColor(wnd, SW_COLOR_WINDOW_BG));
    swFillRect(dc, twidth, 0,
               swGetStringWidth(font, title) + twidth/2+1, theight);
    swSetFGColor(dc, swGetWindowColor(wnd, SW_COLOR_TEXT));
    swDrawText(dc, twidth * 3 / 2, theight, title);
    swDestroyDC(dc);
}
