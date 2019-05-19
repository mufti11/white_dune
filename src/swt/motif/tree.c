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
    
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>

#include "swt.h"
#include "mysnprintf.h"

#ifndef NULL
#ifdef __cplusplus
#define NULL 0
#else
#define NULL (void*)0
#endif
#endif

typedef struct STreeItem STreeItem;

typedef struct STree {
    SWND                wnd;
    SWND                scroller;
    STreeItem          *root;
    STreeItem          *current;
    STreeItem          *dropTarget;
    SBITMAP             imageList;
    SBITMAP             mask;
    int                 iconWidth;
    int                 iconHeight;
    int                 itemHeight;
    int                 overlayImage;
    TreeCallback        callback;
    void               *data;
    STreeItem         **items;
    int                 nItems;
} STree;

struct STreeItem {
    char               *name;
    int                 flags;
    STreeItem          *parent;
    STreeItem          *prev;
    STreeItem          *next;
    STreeItem          *first;
    STreeItem          *last;
    int                 image1, image2;
    int                 index; /* position in tree's vis list */
    void               *data;
};

#define LINE_WIDTH 4
#define LEFT_MARGIN 3 /* space to the left of root item */
#define TEXT_MARGIN 3 /* space between icon & name */

enum {
    HIT_BOX, HIT_ICON, HIT_NAME
};

#define DRAG_MARGIN 5
static enum {
    WAITING, 
    PICKED,
    DRAGGING
} state = WAITING;
static int btnDownX, btnDownY;

static int drawTreeRec(STree *tree, SDC dc, STreeItem *item, int x, int y);
static STreeItem *treeHitTest(STree *tree, int x, int y, int *type);
static void expose(void *data, int x, int y, int width, int height);
static void keyCallback(void *data, int keycode, int value,
                        int x, int y, int modifiers);
static void mouseCallback(void *data, int x, int y, int modifiers);
static STreeItem *newSTreeItem(void);
static void setSelection(STree *tree, STreeItem *item);
static void setCurrent(STree *tree, STreeItem *item);
static void invalidateItem(STree *tree, STreeItem *item);

static int maxWidth;  /* temporary used while drawing */

extern STREE
swCreateTree(int x, int y, int width, int height, SWND parent)
{
    STree *tree;

    if (!parent) return NULL;

    tree = (STree *) malloc(sizeof(STree));
    tree->scroller = swCreateScrolledWindow(x, y, width, height, parent);
    tree->wnd = swCreateCanvas("", x, y, width, height, tree->scroller);
    tree->root = NULL;
    tree->current = NULL;
    tree->dropTarget = NULL;
    tree->iconWidth = 0;
    tree->iconHeight = 0;
    tree->itemHeight = swGetFontHeight(swGetDefaultFont()) + 4;
    tree->imageList = NULL;
    tree->mask = NULL;
    tree->overlayImage = -1;
    tree->callback = NULL;
    tree->data = NULL;
    tree->items = NULL;
    tree->nItems = 0;

    swSetExposeCallback(tree->wnd, expose);
    swSetKeyCallback(tree->wnd, keyCallback);
    swSetMouseCallback(tree->wnd, mouseCallback);
    swSetClientData(tree->wnd, tree);

    return tree;
}

static int
countItemsRec(STree *tree, STreeItem *item)
{
    STreeItem *c;
    int n = 1;

    if (!item) return 0;
    if (!(item->flags & SW_TREE_ITEM_COLLAPSED)) {
        for (c = item->first; c != NULL; c = c->next) {
            n += countItemsRec(tree, c);
        }
    }
    return n;
}

static void
buildItemListRec(STree *tree, STreeItem *item, STreeItem **list, int *index)
{
    STreeItem  *c;

    if (!list) return;
    if (!item) return;
    item->index = *index;
    list[(*index)++] = item;
    if (!(item->flags & SW_TREE_ITEM_COLLAPSED)) {
        for (c = item->first; c != NULL; c = c->next) {
            buildItemListRec(tree, c, list, index);
        }
    }
}

static void
buildItemList(STree *tree)
{
    int count = countItemsRec(tree, tree->root);
    int index = 0;
    
    if (count==0) {
        if (tree->items) free(tree->items);
        tree->items = NULL;
    } else if (count != tree->nItems) {
        if (tree->items) free(tree->items);
        tree->items = (STreeItem **) malloc(count * sizeof(STreeItem *));
    }
    buildItemListRec(tree, tree->root, tree->items, &index);
    tree->nItems = count;
}

extern void
swDestroyTree(STREE tree)
{
    if (!tree) return;

    swTreeDeleteItem(tree, tree->root);
    swDestroyWindow(tree->wnd);
    swDestroyWindow(tree->scroller);
    free(tree->items);
    free(tree);
}

extern STREEITEM
swTreeHitTest(STree *tree, int x, int y)
{
    int type;

    return treeHitTest(tree, x, y, &type);
}

extern int
swTreeGetItemTitle(STREE tree, STREEITEM item, char *buf, int len)
{
    strncpy(buf, item->name, len);
    return strlen(buf);
}

static void
expose(void *data, int x, int y, int width, int height)
{
    STree *tree = (STree *) data;
    SDC dc;
    int oldWidth, oldHeight;
    int newHeight;
    assert(tree != NULL);

    if (tree->root != NULL) {
        dc = swCreateDC(tree->wnd);
        swSetFGColor(dc, swGetWindowColor(tree->wnd, SW_COLOR_WINDOW_BG));
        swFillRect(dc, x, y, width, height);
        swGetSize(tree->wnd, &oldWidth, &oldHeight);
        maxWidth = 0;
        newHeight = drawTreeRec(tree, dc, tree->root, LEFT_MARGIN, 0);
        if (newHeight != oldHeight || maxWidth != oldWidth) {
            swSetScrollSizes(tree->scroller, maxWidth, newHeight);
        }
        swDestroyDC(dc);
    }
}

static void
invalidateSelected(STree *tree)
{
    int i;
    for (i = 0; i < tree->nItems; i++) {
        STreeItem *item = tree->items[i];

        if (item->flags & SW_TREE_ITEM_SELECTED) {
            invalidateItem(tree, item);
        }
    }
}

static void
invalidateItem(STree *tree, STreeItem *item)
{
    if (item && item->index >= 0) {
        swInvalidateRect(tree->wnd, 0, item->index * tree->itemHeight,
                         500, tree->itemHeight);
    }
}

static void
invalidateItemToEnd(STree *tree, STreeItem *item)
{
    if (item->index >= 0) {
        swInvalidateRect(tree->wnd, 0, item->index * tree->itemHeight,
                         500, (tree->nItems - item->index) * tree->itemHeight);
    }
}

static void
clearSelection(STree *tree, STreeItem *item)
{
    STreeItem *child;

    if (item->flags & SW_TREE_ITEM_SELECTED) {
        item->flags &= ~SW_TREE_ITEM_SELECTED;
        invalidateItem(tree, item);
    }

    for (child = item->first; child != NULL; child = child->next) {
        clearSelection(tree, child);
    }
}

static void
toggleSelection(STree *tree, STreeItem *item)
{
    item->flags ^= SW_TREE_ITEM_SELECTED;
    invalidateItem(tree, item);
}

static void
extendSelection(STree *tree, STreeItem *current, STreeItem *item)
{
    int i, start, end;

    if (current->index > item->index) {
        start = item->index;
        end = current->index;
    } else {
        start = current->index;
        end = item->index;
    }
    for (i = start; i <= end; i++) {
        if (!(tree->items[i]->flags & SW_TREE_ITEM_SELECTED)) {
            tree->items[i]->flags |= SW_TREE_ITEM_SELECTED;
            invalidateItem(tree, tree->items[i]);
        }
    }
}

static STreeItem *
nextUncollapsed(STreeItem *item)
{
    if (item->first && !(item->flags & SW_TREE_ITEM_COLLAPSED)) {
        return item->first;
    } else if (item->next) {
        return item->next;
    } else {
        STreeItem *p;
        for (p = item->parent; p != NULL; p = p->parent) {
            if (p->next) {
                return p->next;
            }
        }
    }
    return item;
}

static STreeItem *
prevUncollapsed(STreeItem *item)
{
    if (item->prev) {
        STreeItem *p;
        for (p = item->prev; p->last != NULL; p = p->last) {
            if (p->flags && SW_TREE_ITEM_COLLAPSED) break;
        }
        return p;
    } else if (item->parent) {
        return item->parent;
    } else {
        return item;
    }
}

static void
keyCallback(void *data, int keycode, int value,
             int x, int y, int modifiers)
{
    STree *tree = (STree *) data;
    int width, height;

    if (keycode == SW_MOUSE1 && value) {
        int type;
        STreeItem *item;
         
        swSetFocus(tree->wnd);

        btnDownX = x;
        btnDownY = y;
        item = treeHitTest(tree, x, y, &type);

        if (item != NULL) {
            swGetSize(tree->wnd, &width, &height);
            if (type == HIT_BOX) {
                item->flags ^= SW_TREE_ITEM_COLLAPSED;
                invalidateItemToEnd(tree, item);
                buildItemList(tree);
                invalidateItemToEnd(tree, item);
             } else if (modifiers & SW_SHIFT) {
                extendSelection(tree, tree->current, item);
                setCurrent(tree, item);
             } else if (modifiers & SW_CONTROL) {
                toggleSelection(tree, item);
                setCurrent(tree, item);
             } else {
                setSelection(tree, item);
                state = PICKED;
             }
        }
    } else if (keycode == SW_MOUSE1 && !value) {
        state = WAITING;
    } else if (keycode == SW_UP && value) {
        setSelection(tree, prevUncollapsed(tree->current));
    } else if (keycode == SW_DOWN && value) {
        setSelection(tree, nextUncollapsed(tree->current));
    } else if (keycode == SW_KEY_RIGHT && value) {
        if (tree->current->first) {
            if (tree->current->flags & SW_TREE_ITEM_COLLAPSED) {
                tree->current->flags &= ~SW_TREE_ITEM_COLLAPSED;
                invalidateItemToEnd(tree, tree->current);
                buildItemList(tree);
            } else {
                setSelection(tree, tree->current->first);
            }
        }
    } else if (keycode == SW_KEY_LEFT && value) {
        if (tree->current->first &&
            !(tree->current->flags & SW_TREE_ITEM_COLLAPSED)) {
            tree->current->flags |= SW_TREE_ITEM_COLLAPSED;
            invalidateItemToEnd(tree, tree->current);
            buildItemList(tree);
        } else if (!tree->current->prev && tree->current->parent) {
            setSelection(tree, tree->current->parent);
        }
    }
}

static void
mouseCallback(void *data, int x, int y, int modifiers)
{
    STree *tree = (STree *) data;
    if (state == PICKED
        && (abs(x - btnDownX) > DRAG_MARGIN || abs(y - btnDownY) > DRAG_MARGIN)
       ) {
        state = DRAGGING;
        if (tree->callback)
            tree->callback(tree->data, SW_TREE_BEGIN_DRAG, tree->current);
    }
}

extern SWND
swTreeGetWindow(STREE tree)
{
    if (!tree) return NULL;

    return tree->scroller;
}

extern STREEITEM
swTreeGetRootItem(STREE tree)
{
    if (!tree) return NULL;

    return tree->root;
}

extern STREEITEM
swTreeGetCurrentItem(STREE tree)
{
    if (!tree) return NULL;

    return tree->current;
}

extern void
swTreeSetCurrentItem(STREE tree, STREEITEM item)
{
    if (!tree || !item) return;

    setCurrent(tree, item);
}

extern STREEITEM
swTreeGetParentItem(STREE tree, STREEITEM item)
{
    if (!item) return NULL;

    return item->parent;
}

extern STREEITEM
swTreeGetFirstChild(STREE tree, STREEITEM item)
{
    if (!item) return NULL;

    return item->first;
}

extern STREEITEM
swTreeGetNextItem(STREE tree, STREEITEM item)
{
    if (!item) return NULL;

    return item->next;
}

extern void *
swTreeGetItemData(STREE tree, STREEITEM item)
{
    if (!item) return NULL;

    return item->data;
}

extern void
swTreeSetItemData(STREE tree, STREEITEM item, void *data)
{
    if (!item) return;

    item->data = data;
}

extern void
swTreeSetItemCollapsed(STREE tree, STREEITEM item, int collapsed)
{
    if (!tree || !item) return;

    if (collapsed) {
        if (!(item->flags & SW_TREE_ITEM_COLLAPSED)) {
            item->flags |= SW_TREE_ITEM_COLLAPSED;
            invalidateItemToEnd(tree, item);
            buildItemList(tree);
        }
    } else {
        if (item->flags & SW_TREE_ITEM_COLLAPSED) {
            item->flags &= ~SW_TREE_ITEM_COLLAPSED;
            invalidateItemToEnd(tree, item);
            buildItemList(tree);
        }
    }
}

extern int
swTreeGetItemImage(STREE tree, STREEITEM item)
{
    if (!item) return 0;

    return item->image1;
}

extern void
swTreeSetItemImage(STREE tree, STREEITEM item, int image1, int image2)
{
    if (!item) return;

    item->image1 = image1;
    item->image2 = image2;
}

extern void
swTreeSelectItem(STREE tree, STREEITEM item)
{
    if (!item) return;

    item->flags |= SW_TREE_ITEM_SELECTED;
}

extern void
swTreeDeselectItem(STREE tree, STREEITEM item)
{
    if (!item) return;

    item->flags &= ~SW_TREE_ITEM_SELECTED;
}

extern void
swTreeDeselectAll(STREE tree)
{
    clearSelection(tree, tree->root);
}

STREEITEM
swTreeInsertItem(STREE tree, int position, STREEITEM relative, const char *name)
{
    STreeItem  *item;
    if (!tree || (!relative && tree->root)) return NULL;

    item = newSTreeItem();
    
    item->name = mystrdup(name);

    if (tree->root == NULL) {
        tree->root = item;
        tree->current = item;
        item->flags |= SW_TREE_ITEM_SELECTED;
    } else if (position == SW_INSERT_AFTER) {
        item->next = relative->next;
        item->prev = relative;
        relative->next = item;
        if (item->next) {
            item->next->prev = item;
        } else {
            relative->parent->last = item;
        }
        item->parent = relative->parent;
    } else if (position == SW_INSERT_BEFORE) {
        item->prev = relative->prev;
        item->next = relative;
        relative->prev = item;
        if (item->prev) {
            item->prev->next = item;
        } else {
            relative->parent->first = item;
        }
        item->parent = relative->parent;
    } else if (position == SW_INSERT_FIRST_CHILD) {
        if (relative->first) {
            relative->first->prev = item;
            item->next = relative->first;
        } else {
            relative->last = item;
        }
        relative->first = item;
        item->parent = relative;
    } else if (position == SW_INSERT_LAST_CHILD) {
        if (relative->last) {
            relative->last->next = item;
            item->prev = relative->last;
        } else {
            relative->first = item;
        }
        relative->last = item;
        item->parent = relative;
    }
    item->index = -1;
    buildItemList(tree);
    invalidateItemToEnd(tree, relative ? relative : item);
    return item;
}

extern void
swTreeDeleteItem(STREE tree, STREEITEM item)
{
    STreeItem *c, *next;

    if (!tree || !item) return;

    invalidateItemToEnd(tree, item);

    for (c = item->first; c != NULL; c = next) {
        next = c->next;
        swTreeDeleteItem(tree, c);
    }

    if (tree->current == item) {
        if (item->next) {
            setSelection(tree, item->next);
        } else if (item->prev) {
            setSelection(tree, item->prev);
        } else if (item->parent) {
            setSelection(tree, item->parent);
        } else {
            setSelection(tree, NULL);
        }
    }

    if (item->prev) {
        item->prev->next = item->next;
    } else if (item->parent) {
        item->parent->first = item->next;
    }
    if (item->next) {
        item->next->prev = item->prev;
    } else if (item->parent) {
        item->parent->last = item->prev;
    }

    free(item->name);
    free(item);
    if (item == tree->root) tree->root = NULL;
    buildItemList(tree);
}

extern void
swTreeDeleteAllItems(STREE tree)
{
    swTreeDeleteItem(tree, tree->root);
}

extern void
swTreeSetCallback(STREE tree, TreeCallback callback)
{
    if (!tree) return;

    tree->callback = callback;
}

extern void
swTreeSetClientData(STREE tree, void *data)
{
    if (!tree) return;

    tree->data = data;
}

extern void
swTreeSetImageList(STREE tree, SBITMAP imageList, SBITMAP mask,
                   int width, int height, int count)
{
    tree->imageList = imageList;
    tree->mask = mask;
    tree->iconWidth = width;
    tree->iconHeight = height;
    if (tree->iconHeight + 1 > tree->itemHeight) {
        tree->itemHeight = tree->iconHeight + 1;
    }
}

extern void
swTreeSetOverlayImage(STREE tree, int index)
{
    tree->overlayImage = index;
}

extern void
swTreeSetFlags(STREE tree, STREEITEM item, int mask, int flags)
{
    item->flags &= ~mask;
    item->flags |= flags;
}

extern void
swTreeSelectDropTarget(STREE tree, STREEITEM item)
{
    if (!tree) return;

    if (!tree->dropTarget && item) {
        invalidateSelected(tree);
    }
    if (tree->dropTarget && !item) {
        invalidateSelected(tree);
    }
    if (tree->dropTarget != item) {
        invalidateItem(tree, tree->dropTarget);
        tree->dropTarget = item;
        invalidateItem(tree, item);
    }
}

extern void
swTreeSetItemName(STREE tree, STREEITEM item, const char* name)
{
    item->name = mystrdup(name);
    invalidateItem(tree,item);
}

extern char *
swTreeGetItemName(STREE tree, STREEITEM item)
{
    if (item == NULL)
        return NULL;
    return item->name;
}

static int
drawTreeRec(STree *tree, SDC dc, STreeItem *item, int x, int y)
{
    int cx = x;
    int cy = y + tree->itemHeight / 2;
    int tx, twidth;
    int starty;
    STreeItem *child;
    int ascent = swGetFontAscent(swGetDefaultFont());

    if (item->first) {
        /* draw expand/collapse box */
        if (item->flags & SW_TREE_ITEM_COLLAPSED) {
            swDrawPlusBox(dc, cx, cy - 4);
        } else {
            swDrawMinusBox(dc, cx, cy - 4);
        }
        /* draw line from collapse box to icon */
        swSetLineStyle(dc, SW_DOT);
        swDrawLine(dc, cx + 10, cy, cx + 10 + LINE_WIDTH, cy);
    } else {
        swSetLineStyle(dc, SW_DOT);
        swDrawLine(dc, cx + 5, cy, cx + 10 + LINE_WIDTH, cy);
    }
    cx += 10 + LINE_WIDTH;

    if (tree->imageList) {
        int iy = y + (tree->itemHeight - tree->iconHeight) / 2;
        int index;
        if (item->flags & SW_TREE_ITEM_COLLAPSED) {
            index = item->image1;
        } else {
            index = item->image2;
        }
        swDrawMaskedBitmap(dc, tree->imageList, tree->mask,
                           tree->iconWidth * index, 0, cx, iy,
                           tree->iconWidth, tree->iconHeight);
        if (item->flags & SW_TREE_ITEM_OVERLAY) {
            swDrawMaskedBitmap(dc, tree->imageList, tree->mask,
                               tree->iconWidth * tree->overlayImage, 0, cx, iy,
                               tree->iconWidth, tree->iconHeight);
        }
    }

    tx = cx + tree->iconWidth + TEXT_MARGIN;
    twidth = swGetStringWidth(swGetDefaultFont(), item->name);

    /* draw selection box */
    if (((item->flags & SW_TREE_ITEM_SELECTED) && !tree->dropTarget)
       || item == tree->dropTarget) {
        swSetFGColor(dc, swGetWindowColor(tree->wnd, SW_COLOR_HIGHLIGHT));
        swFillRect(dc, tx, y, twidth+3, tree->itemHeight);
        swSetFGColor(dc, swGetWindowColor(tree->wnd, SW_COLOR_HIGHLIGHT_TEXT));
    } else {
        swSetFGColor(dc, swGetWindowColor(tree->wnd, SW_COLOR_TEXT));
    }

    swDrawText(dc, cx + tree->iconWidth + TEXT_MARGIN + 1,
               cy + ascent / 2, item->name);
    swSetFGColor(dc, swGetWindowColor(tree->wnd, SW_COLOR_WINDOW_FG));

    /* draw focus box */
    if (item == tree->current && !tree->dropTarget) {
        swSetLineStyle(dc, SW_DOT);
        swDrawRect(dc, tx, y, twidth+2, tree->itemHeight-1);
    }

    y += tree->itemHeight;
    starty = cy + tree->iconHeight / 2;
    cx += tree->iconWidth / 2;

    /* draw all children */
    if (!(item->flags & SW_TREE_ITEM_COLLAPSED)) {
        for (child = item->first; child != NULL; child = child->next) {
            swSetLineStyle(dc, SW_DOT);
            if (child->first) {
                swDrawLine(dc, cx, starty, cx, y + tree->itemHeight / 2 - 4);
                starty = y + tree->itemHeight / 2 + 4;
            } else if (!child->next) {
                swDrawLine(dc, cx, starty, cx, y + tree->itemHeight / 2);
            }
            y = drawTreeRec(tree, dc, child, cx - 4, y);
        }
    }
    if (tx + twidth + 3 > maxWidth) maxWidth = tx + twidth + 3;
    return y;
}

static STreeItem *newSTreeItem(void)
{
    STreeItem *item = malloc(sizeof(STreeItem));

    item->parent = NULL;
    item->first = item->last = NULL;
    item->prev = item->next = NULL;
    item->name = NULL;
    item->flags = 0;
    item->image1 = item->image2 = 0;
    item->index = 0;
    item->data = NULL;
    return item;
}

static STreeItem *treeHitTest(STree *tree, int x, int y, int *type)
{
    STreeItem *item, *parent;
    int index = y / tree->itemHeight;

    /* scroll if dragging mouse leave current scroller viewport */
    int sx,sy,w,h,tWidth,tHeight;
    swGetScrollPosition(tree->scroller, &sx, &sy);
    swGetScrollViewportSize(tree->scroller, &w, &h);
    swGetSize(tree->wnd, &tWidth, &tHeight);
    if ((y < sy) && (y > 0))
        swSetScrollPosition(tree->scroller, sx, y);
    else if ((y + tree->itemHeight > sy + h) && (y < tHeight))
        swSetScrollPosition(tree->scroller, sx, y + tree->itemHeight - h);

    if (index < 0 || index >= tree->nItems) return NULL;
    
    item = tree->items[index];

    for (parent = item->parent; parent; parent = parent->parent) {
         x -= 10 + LINE_WIDTH + tree->iconWidth / 2 - 4;
    }
         
    if (item->first && x >= 0 && x < 10 + LINE_WIDTH) {
        *type = HIT_BOX;
    } else if (x >= 10 + LINE_WIDTH
               && x < 10 + LINE_WIDTH + tree->iconWidth) {
        *type = HIT_ICON;
    } else {
        *type = HIT_NAME;
    }

    return item;
}

static void
setCurrent(STree *tree, STreeItem *item)
{
    STreeItem  *parent;
    int x, y, w, h;
    int itemPos, iPos;

    if (!item && tree->current) {
        invalidateItem(tree, tree->current);
        tree->current = NULL;
        return;
    }
    for (parent = item->parent; parent != NULL; parent = parent->parent) {
        if (parent->flags & SW_TREE_ITEM_COLLAPSED) {
            parent->flags &= ~SW_TREE_ITEM_COLLAPSED;
            invalidateItemToEnd(tree, parent);
            buildItemList(tree);
        }
    }
    if (tree->current != item) {
        invalidateItem(tree, tree->current);
        tree->current = item;
        invalidateItem(tree, item);
        swGetScrollPosition(tree->scroller, &x, &y);
        swGetScrollViewportSize(tree->scroller, &w, &h);
        itemPos = item->index * tree->itemHeight;
        if (itemPos + 3 * tree->itemHeight > y + h) {
            swSetScrollSizes(tree->scroller, w, y + h);
        } else 
            swSetScrollSizes(tree->scroller, w, itemPos + 3 * tree->itemHeight);
        iPos = itemPos;
        if (iPos < y) {
            swSetScrollPosition(tree->scroller, x, iPos);
        } else if (iPos + 3 * tree->itemHeight > y + h) {
            swSetScrollPosition(tree->scroller, x, 
                                iPos + 3 * tree->itemHeight - h);
        } else
            swSetScrollPosition(tree->scroller, x, y );

        if (tree->callback) {
            tree->callback(tree->data, SW_TREE_SELECT, item);
        }
    }
}

static void
setSelection(STree *tree, STreeItem *item)
{
    clearSelection(tree, tree->root);
    setCurrent(tree, item);
    if (item) {
        item->flags |= SW_TREE_ITEM_SELECTED;
    }
}
