/*
 * drawing.c
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

#ifdef WIN32
#include <windows.h>
#endif

#include <stdlib.h>

#include "swt.h"

SWND window;

static void
exposeCallback(void *data, int x, int y, int width, int height)
{
    int i;
    SDC dc = swCreateDC(window);
    SFONT font = swFindFont("helvetica", SW_PLAIN, 14);

    swSetFont(dc, font);
    swSetFGColor(dc, swGetWindowColor(window, SW_COLOR_WINDOW_BG));
    swFillRect(dc, x, y, width, height);
    for (i = 0; i < 10; i++) {
         int x1, y1;
         int width, height;
         x1 = rand() % 200;
         y1 = rand() % 200;
         width = rand() % (200 - x1);
         height = rand() % (200 - y1);
         swSetFGColor(dc, rand());
         swSetLineWidth(dc, 10);
         swSetCapStyle(dc, SW_CAP_ROUND);
         swDrawArc(dc, x1, y1, width, height, 135.0, 405.0);
         swSetFGColor(dc, rand());
         swDrawText(dc, rand() % 200, rand() % 200, "bamboozle");
    }
    swDestroyDC(dc);
}

#ifdef WIN32
int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE prevInstance,
            LPSTR lpCmdLine, int nCmdShow)
#else
int main(int argc, char **argv)
#endif
{
#ifdef WIN32
    swInitialize(hInstance);
#else
    swInitialize(&argc, argv, 0);
#endif
    window = swCreateMainWindow("Drawing Test", 50, 50, 200, 200);
    swShowWindow(window);

    swSetExposeCallback(window, exposeCallback);
    swMainLoop();
    swDestroyWindow(window);
    return 0;
}
