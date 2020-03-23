/*
 * buttons.c
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

static void
cb(void *data, int cmd)
{
    char buf[1024];

    swGetTitle((SWND) data, buf, 1023);
    swDebugf("clicked %s\n", buf);
}

SWND b1, b2, b3, b4, b5;

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

    window = swCreateMainWindow("Buttons Test", 30, 30, 200, 150);
    
    b1 = swCreateButton("Button", 0, 0, 200, 30, window);
    b2 = swCreateCheckBox("CheckBox", 0, 30, 200, 30, window);
    b3 = swCreateRadioButton("RadioButton 1", 0, 60, 200, 30, window);
    b4 = swCreateRadioButton("RadioButton 2", 0, 90, 200, 30, window);
    b5 = swCreateRadioButton("RadioButton 3", 0, 120, 200, 30, window);

    swSetCommandCallback(b1, cb);
    swSetClientData(b1, b1);
    swSetCommandCallback(b2, cb);
    swSetClientData(b2, b2);
    swSetCommandCallback(b3, cb);
    swSetClientData(b3, b3);
    swSetCommandCallback(b4, cb);
    swSetClientData(b4, b4);
    swSetCommandCallback(b5, cb);
    swSetClientData(b5, b5);
    swShowWindow(window);
    swMainLoop();
    swDestroyWindow(window);
    return 0;
}
