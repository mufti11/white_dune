/*
 * statusbar.c
 *
 * Copyright (C) 2000 Stephen F. White
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
#include <string.h>

#include "swt.h"
#include "mysnprintf.h"

typedef struct SStatusBar {
    SWND  wnd;
    char *text;
} SStatusBar;

#define MARGIN 3  /* space around the whole shebang */
#define BORDER 1  /* thickness of inset border */
#define INSET  2  /* space between border and text */
#define SPACING (MARGIN + BORDER + INSET)

void expose(void *data, int x, int y, int width, int height);

extern SSTATUSBAR
swCreateStatusBar(SWND wnd)
{
    int pWidth, pHeight;
    int height;
    SStatusBar *bar;

    if (!wnd) return NULL;

    bar = (SStatusBar *) malloc(sizeof(SStatusBar));

    swGetSize(wnd, &pWidth, &pHeight);
    height = swGetFontHeight(swGetDefaultFont()) + SPACING * 2;
    bar->wnd = swCreateCanvas("", 0, 0, pWidth, height, wnd);
    bar->text = mystrdup("");
    swSetExposeCallback(bar->wnd, expose);
    swSetClientData(bar->wnd, bar);
    return bar;
}

void
swDestroyStatusBar(SSTATUSBAR bar)
{
    if (!bar) return;

    swDestroyWindow(bar->wnd);
    free(bar->text);
    free(bar);
}

void
swStatusBarSetText(SSTATUSBAR bar, const char *text)
{
    int width, height;
    if (!bar || !text) return;

    if (text == NULL)
        return;
    free(bar->text);
    bar->text = mystrdup(text);
    swGetSize(bar->wnd, &width, &height);
    swInvalidateRect(bar->wnd, SPACING, SPACING,
                     width - SPACING * 2, height - SPACING * 2);
}

SWND
swStatusBarGetWindow(SSTATUSBAR bar)
{
    if (!bar) return NULL;

    return bar->wnd;
}

void
expose(void *data, int x, int y, int width, int height)
{
    SStatusBar *bar = (SStatusBar *) data;
    SWND wnd = bar->wnd;
    SDC dc = swCreateDC(wnd);
    int m = MARGIN, w, h;
    int descent = swGetFontDescent(swGetDefaultFont());

    swSetFGColor(dc, swGetWindowColor(wnd, SW_COLOR_FACE));
    swFillRect(dc, x, y, width, height);
    swGetSize(wnd, &w, &h);
    swDraw3DRect(dc, wnd, 0, 0, w, h);
    swDraw3DIndent(dc, wnd, m, m, w - m * 2, h - m * 2);
    swSetFGColor(dc, swGetWindowColor(wnd, SW_COLOR_TEXT));
    swDrawText(dc, SPACING, h - (SPACING + descent), bar->text);
    swDestroyDC(dc);
}
