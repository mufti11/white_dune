/*
 * toolbar.c 
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
    
#include <stdio.h>
#include <stdlib.h>

#include "swt.h"

#define MARGIN 4 /* space around the whole shebang */
#define BORDER 1 /* thickness of button border */
#define INNER_MARGIN 2 /* space around each bitmap */

#define SPACING (BORDER + INNER_MARGIN)

#define ITEM_WIDTH (toolbar->bitmapWidth + SPACING * 2)
#define ITEM_HEIGHT (toolbar->bitmapHeight + SPACING * 2)

#define SEPARATOR_WIDTH 6
#define TT_MARGIN 3

typedef struct SToolbarItem SToolbarItem;

#define isEnabled(item) ((item.flags & SW_TB_DISABLED) == 0)
#define isChecked(item) ((item.flags & SW_TB_CHECKED) != 0)

typedef struct SToolbar {
    SWND                wnd;
    SWND                parent;
    SBITMAP             bitmap;
    SToolbarItem       *items;
    int                 nItems;
    int                 bitmapWidth;
    int                 bitmapHeight;
    int                 current;
    int                 selected;
    TooltipCallback     tooltipCallback;
    SWND                popup;
    SWND                popupLabel;
    STIMER              timer;
    void               *data;
} SToolbar;

struct SToolbarItem {
    int                 id;
    int                 button;
    int                 x;
    int                 flags;
};

static void             expose(void *data, int x, int y, int width, int height);
static void             keyCallback(void *data, int keycode, int value,
                                    int x, int y, int modifiers);
static void             mouseCallback(void *data, int x, int y, int modifiers);
static void             enterCallback(void *data, int value);
static void             invalidateItem(SToolbar *toolbar, int item);
static int              timerCB(void *data);

extern STOOLBAR
swCreateToolbar(SWND parent, SBITMAP bitmap, int width, int height, int count,
        const int *buttonInfo)
{
    SToolbar *toolbar;
    int i, x;

    if (!parent) return NULL;

    toolbar = (SToolbar *) malloc(sizeof(SToolbar));
    toolbar->bitmapWidth = width;
    toolbar->bitmapHeight = height;
    toolbar->wnd = swCreateCanvas("", 0, 0, ITEM_WIDTH * count + MARGIN * 2,
                                  ITEM_HEIGHT + MARGIN * 2, parent);
    toolbar->parent = parent;
    toolbar->bitmap = bitmap;
    toolbar->items = (SToolbarItem *) malloc(count * sizeof(SToolbarItem));
    toolbar->nItems = count;
    toolbar->current = toolbar->selected = -1;
    toolbar->tooltipCallback = NULL;
    toolbar->timer = NULL;
    toolbar->data = NULL;

    swSetExposeCallback(toolbar->wnd, expose);
    swSetKeyCallback(toolbar->wnd, keyCallback);
    swSetMouseCallback(toolbar->wnd, mouseCallback);
    swSetEnterCallback(toolbar->wnd, enterCallback);
    swSetClientData(toolbar->wnd, toolbar);

    for (i = 0, x = MARGIN; i < count; i++) {
         SToolbarItem *item = toolbar->items + i;

         item->button = buttonInfo[i*2];
         item->id = buttonInfo[i*2+1];
         item->x = x;
         item->flags = 0;
         x += (item->button >= 0) ? ITEM_WIDTH : SEPARATOR_WIDTH;
    }

    toolbar->popup = swCreatePopup("", 0, 0, 1, 1, toolbar->wnd);
    swHideWindow(toolbar->popup);
    toolbar->popupLabel = swCreateLabel("", TT_MARGIN, TT_MARGIN,
                                        1, 1, toolbar->popup);
    swSetWindowBG(toolbar->popup, 0xC0FFFF);
    swSetWindowBG(toolbar->popupLabel, 0xC0FFFF);

    return toolbar;
}

extern void
swDestroyToolbar(STOOLBAR tb)
{
    if (tb->timer) swKillTimer(tb->timer);
    swDestroyWindow(tb->popupLabel);
    swDestroyWindow(tb->popup);
    swDestroyWindow(tb->wnd);
    free(tb->items);
    free(tb);
}

extern SWND
swToolbarGetWindow(STOOLBAR tb)
{
    if (!tb) return NULL;

    return tb->wnd;
}

extern void
swToolbarSetButtonFlags(STOOLBAR tb, int index, int mask, int flags)
{
    if (!tb || index < 0 || index >= tb->nItems) return;

    if ((tb->items[index].flags & mask) != flags) {
         tb->items[index].flags = (tb->items[index].flags & ~mask) | flags;
         invalidateItem(tb, index);
    }
}

extern void
swToolbarSetTooltipCallback(STOOLBAR tb, TooltipCallback cb)
{
    if (!tb) return;

    tb->tooltipCallback = cb;
}

extern void
swToolbarSetClientData(STOOLBAR tb, void *data)
{
    if (!tb) return;

    tb->data = data;
}

static void
expose(void *data, int x, int y, int width, int height)
{
    SToolbar *toolbar = (SToolbar *) data;
    SDC dc = swCreateDC(toolbar->wnd);
    int i, b, button;
    int w, h;

    swSetFGColor(dc, swGetWindowColor(toolbar->wnd, SW_COLOR_FACE));
    swFillRect(dc, x, y, width, height);
    swGetSize(toolbar->wnd, &w, &h);
    swDraw3DRect(dc, toolbar->wnd, 0, 0, w, h);

    y = MARGIN;

    for (i = 0; i < toolbar->nItems; i++) {
         int offset = 0;
         x = toolbar->items[i].x;
         if (isEnabled(toolbar->items[i])) {
             if ((toolbar->selected == i && toolbar->current == i)
                 || isChecked(toolbar->items[i])) {
        for (b = 0; b < BORDER; b++) {
            swDraw3DIndent(dc, toolbar->wnd, x + b, y + b,
                           ITEM_WIDTH - b * 2, ITEM_HEIGHT - b * 2);
            offset = 1;
        }
             } else if (toolbar->selected == -1 && toolbar->current == i) {
        for (b = 0; b < BORDER; b++) {
            swDraw3DRect(dc, toolbar->wnd, x + b, y + b,
                         ITEM_WIDTH - b * 2, ITEM_HEIGHT - b * 2);
        }
             }
         }
         button = toolbar->items[i].button;
         if (button >= 0) {
             swDrawBitmap(dc, toolbar->bitmap,
                         button * toolbar->bitmapWidth,
                         isEnabled(toolbar->items[i]) ?
                         0 : toolbar->bitmapHeight,
                         x + BORDER + INNER_MARGIN + offset,
                         y + BORDER + INNER_MARGIN + offset,
                         toolbar->bitmapWidth, toolbar->bitmapHeight);
         } else {
             int x1 = x + SEPARATOR_WIDTH / 2;
             swSetFGColor(dc, swGetWindowColor(toolbar->wnd, SW_COLOR_BSHADOW));
             swDrawLine(dc, x1, y, x1, y + ITEM_HEIGHT - 1);
             swSetFGColor(dc, swGetWindowColor(toolbar->wnd, SW_COLOR_TSHADOW));
             swDrawLine(dc, x1 + 1, y, x1 + 1, y + ITEM_HEIGHT - 1);
         }
    }
    swDestroyDC(dc);
}

static int
hitTest(SToolbar *toolbar, int x, int y)
{
    int i, n = toolbar->nItems;

    for (i = 0; i < n; i++) {
         if (x >= toolbar->items[i].x 
             && x < toolbar->items[i].x + ITEM_WIDTH
             && y > MARGIN
             && y < MARGIN + ITEM_HEIGHT
             && toolbar->items[i].button >= 0)
             return i;
    }
    return -1;
}

static void
invalidateItem(SToolbar *toolbar, int item)
{
    if (item >= 0 && item < toolbar->nItems) {
         swInvalidateRect(toolbar->wnd,
                         toolbar->items[item].x, MARGIN,
                         ITEM_WIDTH, ITEM_HEIGHT);
    }
}

static void
keyCallback(void *data, int keycode, int value,
             int x, int y, int modifiers)
{
    SToolbar   *toolbar = (SToolbar *) data;

    if (keycode == SW_MOUSE1) {
         if (value) {
             if (toolbar->current != -1) {
        toolbar->selected = toolbar->current;
        swSetCapture(toolbar->wnd);
             }
         } else {
             if (toolbar->selected != -1) {
        swReleaseCapture(toolbar->wnd);
        toolbar->selected = -1;
        if (toolbar->current != -1) {
            swCallCommandCallback(toolbar->parent,
                                  toolbar->items[toolbar->current].id);
        }
        mouseCallback(data, x, y, modifiers);
             }
         }
         invalidateItem(toolbar, toolbar->current);
    }
}

static void
enterCallback(void *data, int value)
{
    SToolbar *toolbar = (SToolbar *) data;

    if (!value && toolbar->current != -1) {
         invalidateItem(toolbar, toolbar->current);
         toolbar->current = -1;
         swHideWindow(toolbar->popup);
         swCallHighlightCallback(toolbar->parent, -1);
         if (toolbar->timer) {
             swKillTimer(toolbar->timer);
             toolbar->timer = NULL;
         }
    }
}

static void
mouseCallback(void *data, int x, int y, int modifiers)
{
    SToolbar *toolbar = (SToolbar *) data;

    int item = hitTest(toolbar, x, y);
    int id = item >= 0 ? toolbar->items[item].id : -1;

    if (toolbar->selected != -1) {
         if (toolbar->selected != item) {
             item = -1;
         }
    }
    if (toolbar->current != item) {
         invalidateItem(toolbar, toolbar->current);
         toolbar->current = item;
         invalidateItem(toolbar, toolbar->current);
         swCallHighlightCallback(toolbar->parent, id);
         if (toolbar->tooltipCallback)
         {
             swHideWindow(toolbar->popup);
             if (toolbar->timer) {
        swKillTimer(toolbar->timer);
        toolbar->timer = NULL;
             }
             if (toolbar->current != -1) {
        toolbar->timer = swSetTimer(toolbar->wnd, 500, timerCB, toolbar);
             }
         }
    }
}

static int
timerCB(void *data)
{
    SToolbar *toolbar = (SToolbar *) data;
    const char *str;
    SToolbarItem *item = toolbar->items + toolbar->current;
    SFONT font = swGetDefaultFont();

    str = toolbar->tooltipCallback(toolbar->data, item->id);
    if (str && str[0]) {
         int x, y, w, h;
         swLabelSetText(toolbar->popupLabel, str);
         swGetAbsPosition(toolbar->wnd, &x, &y);
         swSetPosition(toolbar->popup,
              x + item->x + toolbar->bitmapWidth / 2,
                      y + ITEM_HEIGHT + 12);
         w = swGetStringWidth(font, str);
         h = swGetFontHeight(font);
         swSetSize(toolbar->popup, w + TT_MARGIN * 2, h + TT_MARGIN * 2);
         swSetSize(toolbar->popupLabel, w, h);
         swShowWindow(toolbar->popup);
    }
    toolbar->timer = NULL;
    return 0;  /* kill timer */
}
