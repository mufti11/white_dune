/*
 * res.h
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

#ifndef _RES_H
#define _RES_H

#ifndef _SWT_H
#include <swt.h>
#endif

#define SW_AUTOCHECKBOX 0x0003
#define SW_AUTORADIOBUTTON 0x0009

typedef enum RCType {
    RCT_BUTTON,
    RCT_CHECKBOX,
    RCT_COMBOBOX,
    RCT_DIALOG,
    RCT_GROUPBOX,
    RCT_LABEL,
    RCT_MENU,
    RCT_MENUBAR,
    RCT_MENUITEM,
    RCT_RADIOBUTTON,
    RCT_SCROLLBAR,
    RCT_SEPARATOR,
    RCT_TEXTEDIT,
    RCT_TOOLBAR
} RCType;

typedef struct RCNode {
    int                 type;
    int                 id;
    const char         *name;
    int                 x, y;
    int                 width, height;
    int                 flags;
    int                 index;
    int                 next;
    int                 children;
} RCNode;

typedef struct RCBitmap {
    int                 id;
    int                 size;
    int                 width, height;
    int                 bits_per_pixel;
    int                *palette;
    unsigned char      *data;
} RCBitmap;

typedef struct RCString {
    int                 id;
    const char         *str;
} RCString;

extern int              swNumBitmaps;
extern int              swNumResources;
extern int              swNumStrings;
extern RCBitmap         swBitmaps[];
extern RCNode           swResources[];
extern RCString         swStrings[];
#endif
