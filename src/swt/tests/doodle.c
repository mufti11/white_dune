/*
 * doodle.c
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

#include "swt.h"

SWND canvas;
int drawing = 0;
int prevx, prevy;

static void
keyCallback(void *data, int key, int state, int x, int y, int modifiers)
{
    if (key == SW_MOUSE1) {
         drawing = state;
         prevx = x;  prevy = y;
    }
}

static void
mouseCallback(void *data, int x, int y, int modifiers)
{
    if (drawing) {
         SDC dc = swCreateDC(canvas);
         swDrawLine(dc, prevx, prevy, x, y);
         prevx = x;  prevy = y;
         swDestroyDC(dc);
    }
}

#ifdef WIN32
int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE prevInstance,
            LPSTR lpCmdLine, int nCmdShow)
#else
int main(int argc, char **argv)
#endif
{
    SWND window;

#ifdef WIN32
    swInitialize(hInstance);
#else
    swInitialize(&argc, argv, 0);
#endif
    window = swCreateMainWindow("Doodle", 50, 50, 200, 200);
    canvas = swCreateCanvas("", 0, 0, 200, 230, window);

    swSetMouseCallback(canvas, mouseCallback);
    swSetKeyCallback(canvas, keyCallback);
    swShowWindow(window);
    swMainLoop();
    swDestroyWindow(window);
    return 0;
}
