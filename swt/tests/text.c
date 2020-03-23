/*
 * text.c
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
    window = swCreateMainWindow("Text Test", 30, 30, 300, 400);
    
    swCreateLabel("single line, editable:",
                  0, 0, 300, 20, window);
    swCreateTextEdit(SW_SINGLE_LINE | SW_EDITABLE,
                     0, 20, 300, 30, window);
    swCreateLabel("single line, non-editable:",
                  0, 50, 300, 20, window);
    swCreateTextEdit(SW_SINGLE_LINE | SW_NON_EDITABLE,
                     0, 70, 300, 30, window);
    swCreateLabel("multi line, editable:",
                  0, 100, 300, 20, window);
    swCreateTextEdit(SW_MULTI_LINE | SW_EDITABLE,
                     0, 120, 300, 130, window);
    swCreateLabel("multi line, non-editable:",
                  0, 250, 300, 20, window);
    swCreateTextEdit(SW_MULTI_LINE | SW_NON_EDITABLE,
                     0, 270, 300, 130, window);
    swShowWindow(window);
    swMainLoop();

    return 0;
}
