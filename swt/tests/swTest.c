/*
 * swTest.c
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

SWND popup = 0, window1, toggleButton;

void
MyScrollCallback(void *data, int type, int value)
{
    swDebugf("scrolled to %d, type %d\n", value, type);
    swSetPosition(toggleButton, 50, value);
}

void
MyButtonCallback(void *data, int id)
{
    char buf[1024];
    static int showing = 0;

    swGetTitle((SWND) data, buf, 1023);
    swDebugf("clicked %s\n", buf);

    if (data == toggleButton) {
         if (!showing) {
             swShowWindow(popup);
         } else {
             swHideWindow(popup);
         }
         showing = !showing;
    }
}

#ifdef WIN32
int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE prevInstance,
            LPSTR lpCmdLine, int nCmdShow)
#else
int main(int argc, char **argv)
#endif
{
    SWND scrollBar, button, button2, window2;
    SMENU  menu, fileMenu, editMenu, buzzMenu;
    int width, height;

#ifdef WIN32
    swInitialize(hInstance);
#else
    swInitialize(&argc, argv, 0);
#endif
    window1 = swCreateMainWindow("SampleApp", 0, 0, 200, 200);
    popup = swCreatePopup("", 500, 100, 50, 30, window1);

    menu = swCreateMenuBar(window1);
    fileMenu = swAppendMenu(menu, "&File");
    editMenu = swAppendMenu(menu, "&Edit");
    swAppendMenuItem(fileMenu, "&New...", 0);
    swAppendMenuItem(fileMenu, "&Open", 0);
    swAppendMenuItem(fileMenu, "&Close", 0);
    swAppendMenuItem(fileMenu, "E&xit", 0);
    swAppendMenuItem(editMenu, "Cu&t", 0);
    swAppendMenuItem(editMenu, "&Copy", 0);
    swAppendMenuItem(editMenu, "&Paste", 0);
    buzzMenu = swAppendMenu(editMenu, "&Buzz");
    swAppendMenuItem(buzzMenu, "&Kilroy", 0);
    swAppendMenuItem(buzzMenu, "&Wuz", 0);
    swAppendMenuItem(buzzMenu, "&Here", 0);
    swInvalidateWindow(window1);

    swGetSize(window1, &width, &height);

    scrollBar = swCreateScrollBar(SW_VERTICAL, 0, 0, 15, height,
                                  0, 100, 0, 30, window1);
    swCreateLabel("Welcome to SWT!", 20, height / 2 - 30, 180, 30, window1);
    swCreateLabel("Boo!", 0, 0, 50, 30, popup);
    swSetScrollBarCallback(scrollBar, MyScrollCallback);
    button = swCreateButton("OK", 120, height -30, 80, 30, window1);
    button2 = swCreateButton("Cancel", 20, height - 30, 80, 30, window1);
    swSetCommandCallback(button, MyButtonCallback);
    swSetClientData(button, button);
    swSetCommandCallback(button2, MyButtonCallback);
    swSetClientData(button2, button2);

    window2 = swCreateMainWindow("second window", 20, 20, 200, 200);
    toggleButton = swCreateButton("Toggle Popup", 50, 70, 100, 30, window2);
    swSetCommandCallback(toggleButton, MyButtonCallback);
    swSetClientData(toggleButton, toggleButton);

    swCreateTextEdit(SW_MULTI_LINE, 0, 100, 200, 100, window2);
    swDebugf("initialized!\n");
    swShowWindow(window1);
    swShowWindow(window2);
    swMainLoop();

    return 0;
}
