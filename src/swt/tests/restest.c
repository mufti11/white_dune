/*
 * restest.c
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

#include <stdlib.h>
#include <stdarg.h>
#ifdef WIN32
#include <windows.h>
#endif

#include "swt.h"
#include "res.h"
#include "resource.h"

SWND wnd;
SBITMAP bitmap;
int drawing = 0;

static void
mouseCallback(void *data, int x, int y, int modifiers);

static void
keyCallback(void *data, int key, int value, int x, int y, int modifiers)
{
    if (key == SW_MOUSE1) {
         drawing = value;
         mouseCallback(data, x, y, modifiers);
    }
}

static void
mouseCallback(void *data, int x, int y, int modifiers)
{
    if (drawing) {
         SDC dc = swCreateDC(wnd);
         swDrawBitmap(dc, bitmap, 0, 0, x - 24, y - 24, 48, 48);
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
#ifdef WIN32
    swInitialize(hInstance);
#else
    swInitialize(&argc, argv, 0);
#endif

    wnd = swCreateMainWindow("Resource Test", 50, 50, 200, 200);
    swLoadMenuBar(wnd, IDR_MENU1);
    bitmap = swLoadBitmap(wnd, IDB_BITMAP1);
    swSetKeyCallback(wnd, keyCallback);
    swSetMouseCallback(wnd, mouseCallback);
    swSetClientData(wnd, wnd);
    swShowWindow(wnd);
    swMainLoop();
    return 0;
}
