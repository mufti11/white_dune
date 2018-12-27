/*
 * resload.c 
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
#include <res.h>
#include <stdlib.h>
#include <string.h>

#ifndef NULL
#ifdef __cplusplus
#define NULL 0
#else
#define NULL (void*)0
#endif
#endif

static int dialogScaleX = 0;
static int dialogScaleY = 0;

static SBITMAP createBitmapFromData(SWND wnd, RCBitmap *b, const int *mapFrom,
                                    const int *mapTo, int nmaps);

extern SBITMAP
swLoadBitmap(SWND wnd, int id)
{
    return swLoadMappedBitmap(wnd, id, NULL, NULL, 0);
}

extern SBITMAP
swLoadMappedBitmap(SWND wnd, int id, const int *mapFrom, const int *mapTo,
           int nmaps)
{
    RCBitmap *b = 0;
    int i;
    
    for (i = 0; i < swNumBitmaps; i++) {
        if (swBitmaps[i].id == id) {
            b = &swBitmaps[i];
            break;
        }
    }

    if (!b) return NULL;

    return createBitmapFromData(wnd, b, mapFrom, mapTo, nmaps);
}

void
swLoadString(int id, char *buf, int len)
{
    int i;

    if (len < 1)
        return;
    buf[0] = 0;
    for (i = 0; i < swNumStrings; i++) {
        if (swStrings[i].id == (id + swGetLang())) {
            strncpy(buf, swStrings[i].str, len);
            break;
        }
    }
}

static void
createMenuRec(SWND wnd, SMENU parent, RCNode *r)
{
    int c;
    
    if (r->type == RCT_MENU) {
        SMENU menu = swAppendMenu(parent, r->name);
        for (c = r->children; c != -1; c = swResources[c].next) {
            createMenuRec(wnd, menu, &swResources[c]);
        }
    } else if (r->type == RCT_MENUITEM) {
        swAppendMenuItem(parent, r->name, r->id);
        swMenuSetFlags(parent, r->id, r->width, r->width);
        if (r->x != 0) swAddAccelerator(wnd, r->x, r->y, r->id);
    } else if (r->type == RCT_SEPARATOR) {
        swAppendSeparator(parent);
    } else {
        swDebugf("unknown menu item type %d\n", r->type);
    }
}

static SWND
createDialogItem(SWND wnd, SWND dlg, RCNode *node)
{
    int x = node->x * dialogScaleX / 4;
    int y = node->y * dialogScaleY / 8;
    int width = node->width * dialogScaleX / 4;
    int height = node->height * dialogScaleY / 8;
    int id = node->id;
    void *vid = swToPtr(id);
    SWND r;

    switch(node->type) {
      case RCT_BUTTON:
        r = swCreateButton(node->name, x, y, width, height, dlg);
        swSetClientData(r, vid);
        if (node->id == 1) {
            swSetDefaultButton(dlg, r);
        } else if (node->id == 2) {
            swSetCancelButton(dlg, r);
        }
        break;
      case RCT_CHECKBOX:
        r = swCreateCheckBox(node->name, x, y, width, height, dlg);
        swSetClientData(r, vid);
        break;
      case RCT_RADIOBUTTON:
        r = swCreateRadioButton(node->name, x, y, width, height, dlg);
        swSetClientData(r, vid);
        break;
      case RCT_COMBOBOX:
        r = swCreateComboBox(node->name, x, y, width, height, dlg, id);
        swSetClientData(r, vid);
        break;
      case RCT_GROUPBOX:
        r = swCreateGroupBox(node->name, x, y, width, height, dlg);
        break;
      case RCT_LABEL:
        r = swCreateLabel(node->name, x, y, width, height, dlg);
        swSetClientData(r, vid);
        break;
      case RCT_TEXTEDIT:
        r = swCreateTextEdit(node->flags, x, y, width, height, dlg);
        swSetClientData(r, vid);
        break;
      default:
        swDebugf("unknown dialog item type %d\n", node->type);
        return NULL;
    }
    return r;
}

extern SMENU
swLoadMenuBar(SWND wnd, int id)
{
    RCNode *r = 0;
    int i, c;
    SMENU menu;
    
    for (i = 0; i < swNumResources; i++) {
        r = &swResources[i];
        if (r->id == id && r->type == RCT_MENUBAR) {
            break;
        }
    }

    if (!r) return NULL;

    menu = swCreateMenuBar(wnd);
    for (c = r->children; c != -1; c = swResources[c].next) {
        createMenuRec(wnd, menu, &swResources[c]);
    }
    return menu;
}

extern SWND
swLoadDialog(SWND wnd, int id)
{
    RCNode *r = 0;
    int i, c;
    SWND dlg;
    SFONT font = swGetDefaultFont();

    if (dialogScaleX == 0) {
        dialogScaleX = swGetStringWidth(font, "n");
        dialogScaleY = swGetFontHeight(font) + 1;
    }

    for (i = 0; i < swNumResources; i++) {
        r = &swResources[i];
        if (r->id == id && r->type == RCT_DIALOG) {
            break;
        }
    }

    if (!r) return NULL;

    dlg = swCreateDialog(r->name, 
                         r->x * dialogScaleX / 4, r->y * dialogScaleY / 8,
                         r->width * dialogScaleX / 4, 
                         r->height * dialogScaleY / 8,
                         wnd);
    for (c = r->children; c != -1; c = swResources[c].next) {
        createDialogItem(wnd, dlg, &swResources[c]);
    }
    return dlg;
}

/*
 * createBitmapFromData() - create a bitmap from paletted image data
 *
 * The data can be n bits per pixel, for n = 1, 2, 4, or 8.
 * The image data is big-endian.  The palette is 2^n entries,
 * where each entry is 4 bytes:  blue, green, red, dummy.
 */

static SBITMAP
createBitmapFromData(SWND wnd, RCBitmap *b,
                     const int *mapFrom, const int *mapTo, int nmaps)
{
    SBITMAP bitmap;
    int size = b->width * b->height;
    unsigned char *data = malloc(size);
/*    int ncolors = 1 << b->bits_per_pixel; */
    int ncolors = 16;
    int *palette = malloc(ncolors * 4);
    int mask = ncolors - 1;
    int offset = 8 - b->bits_per_pixel;
    int i, j, bit;
    
    bitmap = swCreateBitmap(wnd, b->width, b->height);
    memcpy(palette, b->palette, ncolors * 4);
    for (i = 0; i < nmaps; i++) {
        int from = mapFrom[i];
        int to = mapTo[i];
        for (j = 0; j < ncolors; j++) {
            if (palette[j] == from) {
                palette[j] = to;
            }
        }
    }
    for (i = 0, bit = 0; i < size; i++, bit += b->bits_per_pixel) { 
        int c = (b->data[bit / 8] >> (offset - (bit % 8))) & mask;
        data[i] = c;
    }
    swDrawMappedPixels(bitmap, 0, 0, 0, 0, b->width, b->height, data,
                       ncolors, palette);
    free(data);
    free(palette);
    return bitmap;
}
