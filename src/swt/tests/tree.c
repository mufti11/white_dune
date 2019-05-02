/*
 * tree.c
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

SWND window, canvas;
SDC dc;
SFONT font;
STREE tree;
STREEITEM root, item1, item2, item3;
STREEITEM subitem1, subitem2, subitem3;
STREEITEM subsubitem1, subsubitem2;

static void
callback(void *data, int type, STREEITEM current)
{
    char buf[1024];

    swTreeGetItemTitle(tree, current, buf, 1024);
    swDebugf("current set to \"%s\"\n", buf);
}

static void
resizeCallback(void *data, int width, int height)
{
    swSetSize(swTreeGetWindow(tree), width, height);
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
    window = swCreateMainWindow("Tree Test", 50, 50, 200, 200);
    tree = swCreateTree(0, 0, 200, 200, window);
    root = swTreeInsertItem(tree, SW_INSERT_AFTER, 0, "root");
    item1 = swTreeInsertItem(tree, SW_INSERT_FIRST_CHILD, root, "item1");
    item2 = swTreeInsertItem(tree, SW_INSERT_AFTER, item1, "item2");
    item3 = swTreeInsertItem(tree, SW_INSERT_AFTER, item2, "item3");
    subitem1 = swTreeInsertItem(tree, SW_INSERT_FIRST_CHILD, item2, "subitem1");
    subitem2 = swTreeInsertItem(tree, SW_INSERT_AFTER, subitem1, "subitem2");
    subitem3 = swTreeInsertItem(tree, SW_INSERT_AFTER, subitem2, "subitem3");
    subsubitem1 = swTreeInsertItem(tree, SW_INSERT_FIRST_CHILD, subitem3, "subsubitem1");
    subsubitem2 = swTreeInsertItem(tree, SW_INSERT_AFTER, subsubitem1, "subsubitem2");
    swTreeSetCallback(tree, callback);
    swSetResizeCallback(window, resizeCallback);
    swShowWindow(window);
    swMainLoop();
    swDestroyWindow(window);
    swDestroyDC(dc);
    return 0;
}
