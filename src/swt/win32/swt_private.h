/*
 * swt_private.h
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

#define MODIFIERS(wParam)    ((wParam & MK_CONTROL ? SW_CONTROL : 0) \
                              | (wParam & MK_SHIFT ? SW_SHIFT : 0))

typedef struct SWindow {
    HWND              hWnd;
    void             *data;
    HACCEL            accelerators;
    HCURSOR           cursor;
    ExposeCallback    exposeCallback;
    MouseCallback     mouseCallback;
    KeyCallback       keyCallback;
    FocusCallback     focusCallback;
    ResizeCallback    resizeCallback;
    EnterCallback     enterCallback;
    ActivateCallback  activateCallback;
    ScrollBarCallback scrollBarCallback;
    TimerCallback     timerCallback;
    CommandCallback   commandCallback;
    HighlightCallback highlightCallback;
    CloseCallback     closeCallback;
    TooltipCallback   tooltipCallback;
    IDropTarget      *dropTarget;
    SWND              child;  // for scrolled windows
    BOOL              mouseInWindow;
    int               bgColor;
    HBRUSH            bgBrush;
    int               isHidden;
} SWindow;

typedef struct Pair {
    char *key;
    char *value;
} Pair;

struct FileSTable {
    int   size;
    int   capacity;
    Pair *data;
    char *filename;
} FileSTable;
 
typedef struct FileSTable *FILE_STABLE;

#ifdef HAVE_WINDOWS_SPACEBALL
void processWM_INPUTEvent(LPARAM lParam);
#endif
