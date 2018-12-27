/*
 * util.c 
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
    
#include <swt.h>

static void drawBox(SDC dc, int x, int y);
static void drawShadedRect(SDC dc, int x, int y, int width, int height,
                           int topColor, int bottomColor);

extern void
swDraw3DRect(SDC dc, SWND wnd, int x, int y, int width, int height)
{
    int tColor = swGetWindowColor(wnd, SW_COLOR_BSHADOW);
    int bColor = swGetWindowColor(wnd, SW_COLOR_TSHADOW);

    drawShadedRect(dc, x, y, width, height, tColor, bColor);
}

extern void
swDraw3DIndent(SDC dc, SWND wnd, int x, int y, int width, int height)
{
    int tColor = swGetWindowColor(wnd, SW_COLOR_TSHADOW);
    int bColor = swGetWindowColor(wnd, SW_COLOR_BSHADOW);

    drawShadedRect(dc, x, y, width, height, tColor, bColor);
}

extern void
swDrawPlusBox(SDC dc, int x, int y)
{
    drawBox(dc, x, y);
    swDrawLine(dc, x + 2, y + 4, x + 6, y + 4);
    swDrawLine(dc, x + 4, y + 2, x + 4, y + 6);
}

extern void
swDrawMinusBox(SDC dc, int x, int y)
{
    drawBox(dc, x, y);
    swDrawLine(dc, x + 2, y + 4, x + 6, y + 4);
}

static void
drawBox(SDC dc, int x, int y)
{
    swSetLineStyle(dc, SW_SOLID);
    swSetFGColor(dc, 0x00ffffff);
    swFillRect(dc, x, y, 8, 8);
    swSetFGColor(dc, 0x00000000);
    swDrawRect(dc, x, y, 8, 8);
}

static void
drawShadedRect(SDC dc, int x, int y, int width, int height,
               int topColor, int bottomColor)
{
    int x2 = x + width - 1;
    int y2 = y + height - 1;

    swSetFGColor(dc, bottomColor);
    swDrawLine(dc, x,  y2, x,  y);
    swDrawLine(dc, x,  y,  x2, y);
    swSetFGColor(dc, topColor);
    swDrawLine(dc, x2, y,  x2, y2);
    swDrawLine(dc, x2, y2, x,  y2);
}
