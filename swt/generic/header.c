/*
 * header.c 
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
#include <string.h>
#include <assert.h>
#include <swt.h>

#ifndef NULL
#ifdef __cplusplus
#define NULL 0
#else
#define NULL (void*)0
#endif
#endif

typedef struct SHeaderItem SHeaderItem;

typedef struct SHeader {
    SWND                wnd;
    SHeaderItem       **items;
    int                 nItems;
    int                 height;
    int                 tracking;
    int                 trackOffset;
    HeaderCallback      headerCallback;
    void               *data;
} SHeader;

struct SHeaderItem {
    char               *name;
    int                 width;
};

static void             expose(void *data, int x, int y, int width, int height);
static void             keyCallback(void *data, int keycode, int value,
                                    int x, int y, int modifiers);
static void             mouseCallback(void *data, int x, int y, int modifiers);
static SHeaderItem     *newSHeaderItem(const char *name, int width);
static void             invalidateColumn(SHeader *header, int pos);

extern SHEADER
swCreateHeader(int x, int y, int width, int height, SWND parent)
{
    SHeader     *header;
    if (!parent) return NULL;

    header = (SHeader *) malloc(sizeof(SHeader));
    header->wnd = swCreateCanvas("", x, y, width, height, parent);
    header->items = NULL;
    header->nItems = 0;
    header->height = height;
    header->tracking = -1;
    header->trackOffset = 0;
    header->headerCallback = NULL;
    header->data = NULL;

    swSetExposeCallback(header->wnd, expose);
    swSetKeyCallback(header->wnd, keyCallback);
    swSetMouseCallback(header->wnd, mouseCallback);
    swSetClientData(header->wnd, header);

    return header;
}

extern void
swDestroyHeader(SHEADER header)
{
    int i;

    if (!header) return;

    swDestroyWindow(header->wnd);
    for (i = 0; i < header->nItems; i++) {
         free(header->items[i]->name);
         free(header->items[i]);
    }
    free(header->items);
    free(header);
}

extern void
swHeaderSetSize(SHeader *header, int width, int height)
{
    if (!header) return;

    swSetSize(header->wnd, width, height);
}

extern void
swHeaderGetSize(SHEADER header, int *width, int *height)
{
    if (!header) return;

    swGetSize(header->wnd, width, height);
}

extern void
swHeaderInsertItem(SHEADER header, int pos, const char *name, int width)
{
    SHeaderItem *item;
    int i;

    if (!header || !name || pos < 0 || pos > header->nItems + 1) return;

    item = newSHeaderItem(name, width);
    header->nItems++;
    header->items = realloc(header->items,
                            header->nItems * sizeof(SHeaderItem *));
    for (i = pos; i < header->nItems-1; i++) {
         header->items[i+1] = header->items[i];
    }
    header->items[pos] = item;
}

extern int
swHeaderGetItemWidth(SHEADER header, int pos)
{
    if (!header || pos < 0 || pos > header->nItems) return 0;

    return header->items[pos]->width;
}

extern void
swHeaderSetItemWidth(SHEADER header, int pos, int width)
{
    if (!header || pos < 0 || pos > header->nItems) return;

    if (width < 0) width = 0;

    invalidateColumn(header, pos);
    header->items[pos]->width = width;
    invalidateColumn(header, pos);
}

extern void
swHeaderSetCallback(SHEADER header, HeaderCallback cb)
{
    if (!header) return;

    header->headerCallback = cb;
}

extern void
swHeaderSetClientData(SHEADER header, void *data)
{
    if (!header) return;

    header->data = data;
}

static void
expose(void *data, int x, int y, int width, int height)
{
    SHeader *header = (SHeader *) data;
    SDC dc;
    int i, textX = 0;
    int face;

    assert(header);

    dc = swCreateDC(header->wnd);
    swSetClipRect(dc, x, y, width, height);
    face = swGetWindowColor(header->wnd, SW_COLOR_FACE);
    for (i = 0; i < header->nItems; i++) {
         swDraw3DRect(dc, header->wnd, textX, 0,
             header->items[i]->width, header->height);
         swSetFGColor(dc, face);
         swFillRect(dc, textX+1, 1, header->items[i]->width-2, header->height-2);
         swSetFGColor(dc, 0x00000000);
         swDrawTextTruncated(dc, textX + 5, 1,
                            header->items[i]->width-5, header->height,
                            header->items[i]->name);
         textX += header->items[i]->width;
    }
    swDestroyDC(dc);
}

static void
keyCallback(void *data, int keycode, int value,
             int x, int y, int modifiers)
{
    SHeader      *header = (SHeader *) data;

    if (keycode == SW_MOUSE1 && value) {
         int i, tot = 0;

         if (y >= header->height) return;

         for (i = 0; i < header->nItems; i++) {
             tot += header->items[i]->width;
             if (x >= tot - 5 && x <= tot + 5) {
        header->tracking = i;
        header->trackOffset = header->items[i]->width - x;
             }
         }
    } else if (keycode == SW_MOUSE1 && !value) {
         header->tracking = -1;
    }
}

static void
mouseCallback(void *data, int x, int y, int modifiers)
{
    SHeader      *header = (SHeader *) data;

    if (header->tracking != -1) {
         invalidateColumn(header, header->tracking);
         header->items[header->tracking]->width = x + header->trackOffset;
         invalidateColumn(header, header->tracking);
         if (header->headerCallback) {
             header->headerCallback(header->data, header->tracking,
        header->items[header->tracking]->width);
         }
    }
}

static SHeaderItem *
newSHeaderItem(const char *name, int width)
{
    SHeaderItem *item = malloc(sizeof(SHeaderItem));

    item->name = mystrdup(name);
    item->width = width;
    return item;
}

static void
invalidateColumn(SHeader *header, int pos)
{
    int i, x = 0;

    for (i = 0; i < pos; i++) {
         x += header->items[i]->width;
    }

    swInvalidateRect(header->wnd, x, 0,
                     header->items[pos]->width, header->height);
}

