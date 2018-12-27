/*
 * FieldView.cpp
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

#include "stdafx.h"
#include "FieldView.h"
#include "swt.h"
#include "resource.h"
#include "FieldViewItem.h"
#include "Node.h"
#include "Proto.h"
#include "Field.h"
#include "FieldValue.h"
#include "FieldCommand.h"
#include "MFieldCommand.h"
#include "Path.h"
#include "Scene.h"
#include "SFNode.h"

#define CB_WIDTH  9
#define CB_HEIGHT  9
#define CB_SPACING 3
#define CB_ARRAY_INSERT (CB_WIDTH + CB_SPACING * 3)

static void
headerCallback(void *data, int pos, int width)
{
    ((FieldView *) data)->OnHeaderChange(pos, width);
}

static void
textCommandCallback(void *data, void *command)
{
    if (command) {
        ((FieldView *) data)->StopEditing();
    } else {
       ((FieldView *) data)->AbortEditing();
    }
}

static void
textFocusCallback(void *data, int value)
{
    if (!value) ((FieldView *) data)->StopEditing();
}

static void
expose(void *data, int x, int y, int width, int height)
{
    ((FieldView *) data)->OnDraw(x, y, width, height);
}

static void
keyCB(void *data, int key, int value, int x, int y, int modifiers)
{
    if (key == SW_MOUSE1) {
        if (value) {
            ((SceneView *) data)->OnLButtonDown(x, y, modifiers);
        } else {
            ((SceneView *) data)->OnLButtonUp(x, y, modifiers);
        }
    } else if (value) {
        ((SceneView *) data)->OnKeyDown(key, x, y, modifiers);
    } else {
        ((SceneView *) data)->OnKeyUp(key, x, y, modifiers);
    }
}

static void
mouseCB(void *data, int x, int y, int modifiers)
{
    ((SceneView *) data)->OnMouseMove(x, y, modifiers);
}

static void
mouseEnterCB(void *data, int value)
{
    if (value) {
        ((SceneView *) data)->OnMouseEnter();
    } else {
        ((SceneView *) data)->OnMouseLeave();
    }
}

FieldView::FieldView(Scene *scene, SWND parent)
  : SceneView(scene, parent)
{
    int width, height;
    int fg, bg;

    swGetSize(parent, &width, &height);

    m_scroller = swCreateScrolledWindow(0, 0, width, height, parent);
    m_window = swCreateCanvas("", 0, 0, width, height, m_scroller);
    swScrolledWindowSetChild(m_scroller, m_window);
    swSetExposeCallback(m_window, expose);
    swSetKeyCallback(m_window, keyCB);
    swSetMouseCallback(m_window, mouseCB);
    swSetEnterCallback(m_window, mouseEnterCB);
    swSetClientData(m_window, this);

    m_state = NORMAL;
    m_trackPoint.x = m_trackPoint.y = 0;
    m_trackValue = NULL;
    m_selectedField = -1;
    m_selectedNode = NULL;
    m_selectedProto = NULL;
    m_selectedItem = -1;
    m_pageHeight = 0;
    m_scrollY = 0;
    m_scrollRatio = 1.0f;
    fg = swGetWindowColor(parent, SW_COLOR_WINDOW_FG);
    bg = swGetWindowColor(parent, SW_COLOR_WINDOW_BG);
    int mapFrom[2] = { 0x00ffffff, 0x00ff0000 };
    int mapTo[2] = { fg, bg };
    m_halftoneBitmap = swLoadMappedBitmap(parent, IDB_HALFTONE,
                                         mapFrom, mapTo, 2);
    m_itemHeight = swGetFontHeight(swGetDefaultFont()) + 2;
    m_floatWidth = swGetFontHeight(swGetDefaultFont()) * 4;
    m_height = m_itemHeight;
    m_header = NULL; // this is so we can handle OnSize() correctly
    m_header = swCreateHeader(0, 0, width, m_itemHeight, m_window);
    swHeaderSetCallback(m_header, headerCallback);
    swHeaderSetClientData(m_header, this);
    int cx = MIN(width, 160);
    swHeaderInsertItem(m_header, 0, "Field", cx);
    swHeaderInsertItem(m_header, 1, "Value", width - cx);

    m_width = 550;

    m_uFont = swFindFont("Helvetica", SW_UNDERLINE, 10);
    m_fieldViewActive=true;

    m_cursorArrow = swLoadCursor(SW_CURSOR_ARROW);
    m_cursorHMove = swLoadCursor(SW_CURSOR_DBL_ARROW_HORZ);
    m_cursorIsArrow = true;
}

FieldView::~FieldView()
{
    DeleteView();
    DeleteFields();
    swDestroyCursor(m_cursorArrow);
    swDestroyCursor(m_cursorHMove);
//    swDestroyWindow(m_window);
//    m_window = NULL;
//    swDestroyWindow(m_scroller);
//    m_scroller = NULL;
//    swDestroyWindow(m_wnd);
//    m_wnd = NULL;
}

void FieldView::DeleteFields()
{
    if (!getEnabled())
        return;
    for (int i = 0; i < m_fields.size(); i++) {
       delete m_fields[i];
    }
    m_fields.resize(0);
}

void FieldView::DeleteView()
{
    m_fieldViewActive=false;
    if (m_halftoneBitmap!=NULL) {
       swDestroyBitmap(m_halftoneBitmap);
       m_halftoneBitmap=NULL;
    }
    if (m_header!=NULL) {
       swHeaderSetCallback(m_header,NULL);
       swDestroyHeader(m_header);
       m_header=NULL;
    }
    if (m_uFont!=NULL) {
       swDeleteFont(m_uFont);
       m_uFont=NULL;
    }
    if (m_window != NULL)
       swDeleteCallbacks(m_window);
    if (m_scroller != NULL)
       swDeleteCallbacks(m_scroller);
    if (m_wnd != NULL)
       swDeleteCallbacks(m_wnd);
    swDestroyWindow(m_window);
    m_window = NULL;
    swDestroyWindow(m_scroller);
    m_scroller = NULL;
}

void FieldView::OnDraw(int x, int y, int width, int height)
{
    int n = m_items.size();
    
    SDC dc = swCreateDC(m_window);
    SDC bdc = swCreateBitmapDC(dc, x + width, y + height);
    if (bdc == NULL) {
        swDestroyDC(dc);
        return;
    }
    swSetFGColor(bdc, swGetWindowColor(m_window, SW_COLOR_WINDOW_BG));
    swFillRect(bdc, x, MAX(y, m_itemHeight), width, height);
    int scrollX, scrollY;
    swGetScrollPosition(m_scroller, &scrollX, &scrollY);
    int start = ((float)scrollY / (float)m_height) * n;
    int end = start + n * ((float)height + (float)y) / (float)m_height + 1;
    for (int i = start - 2 > -1 ? start - 2 : 0; (i < end) && (i < n); i++) {
        DrawItem(i, bdc);
    }
    swCopyRect(bdc, dc, x, y, x, y, width, height);
    swDestroyDC(bdc);
    swDestroyDC(dc);
}

int FieldView::FindField(int field)
{
    int currentField = -1;
    for (int i = 0; i < m_fields.size(); i++)
        if (m_fields[i]->GetIndex() == field) {
           currentField = i;
           break;
        }
    return currentField;
}

void FieldView::OnUpdate(SceneView *sender, int type, Hint *hint) 
{
    const Path *sel = m_scene->getSelection();
    Node *node = sel ? sel->getNode() : NULL;
    int field = sel ? sel->getField() : -1;
    Proto *proto = NULL;
    if (sel->isProto(m_scene))
        proto = sel->getProto(m_scene);
    Rect rect;
    FieldUpdate *fieldUpdate;
    NodeUpdate *nodeUpdate;
    int currentField = -1;
    Node *currentNode = NULL;

    if ((m_state == EDITING) && (type != UPDATE_FIELD))
        StopEditing();

    switch (type) {
      CASE_UPDATE(UPDATE_ALL)
        UpdateAll();
        break;
      CASE_UPDATE(UPDATE_SELECTION)
        m_selectedNode = node;
        m_selectedField = field;
        m_selectedProto = proto;
        swSetScrollPosition(m_scroller, 0, 0);
        UpdateAll();
        break;
      CASE_UPDATE(UPDATE_FIELD)
        fieldUpdate = (FieldUpdate *) hint;
        currentField = FindField(fieldUpdate->field);
        currentNode = fieldUpdate->node;
        if ((fieldUpdate->node == node) && (currentField == field))
            if (m_state == EDITING)
                StopEditing();
        if ((fieldUpdate->node == node) && (currentField != -1)) {
            if (fieldUpdate->index == -1) {
                FieldViewItem *item = m_fields[currentField];
                FieldValue *value = node->getField(fieldUpdate->field);
                item->SetValue(value);
                if (item->IsGroup() && !item->IsCollapsed()) {
                    if (m_fieldViewActive)
                        swInvalidateWindow(m_window);
                }
            } else if ((fieldUpdate->node == currentNode) &&
                       m_fields[currentField]->IsGroup()) {
                MFieldValue *value = (MFieldValue *) node->getField(fieldUpdate->field);
                MFieldViewItem *parent = (MFieldViewItem *) m_fields[currentField];
                parent->GetChild(fieldUpdate->index)->SetValue(value->getSFValue(fieldUpdate->index));
            }
            if (m_fieldViewActive) {
                RefreshItemList();
                GetFieldRect(currentField, fieldUpdate->index, &rect);
                swInvalidateWindow(m_window);
            }
        }
        break;
      CASE_UPDATE(UPDATE_CHANGE_INTERFACE_NODE)
        UpdateAll();
        break;
      CASE_UPDATE(UPDATE_ADD_NODE)
      CASE_UPDATE(UPDATE_REMOVE_NODE)
        nodeUpdate = (NodeUpdate *) hint;
        if ((nodeUpdate->node == NULL) || (nodeUpdate->field == -1))
            break;
        currentField = FindField(nodeUpdate->field);
        if (nodeUpdate->parent == node) {
            if (nodeUpdate->field)
                GetFieldRect(currentField, -1, &rect);
            RefreshItemList();
            swInvalidateRect(m_window, rect.left, rect.top,
                             rect.Width(), rect.Height());
        }
        break;
      CASE_UPDATE(UPDATE_REDRAW)
        UpdateAll();
        swInvalidateWindow(m_window);
        break;
    }
}

void FieldView::UpdateAll(bool resetSelection)
{
    int width0 = GetColumnWidth(0);
    int width1 = GetColumnWidth(1);

    bool x3d = m_scene->isX3d();

/*
    Array<MyString> collapsedNames;
    if (m_selectedNode) {
        Proto *def = m_selectedNode->getProto();
        if (def) {
            for (int i = 0; i < m_fields.size(); i++)
                if (m_fields[i]->IsCollapsed() && 
                    (def->getField(i)->getName(x3d).length() != 0))
                    collapsedNames.append(mystrdup(def->getField(i)->getName(x3d)));
        }
    }                
*/

    DeleteFields();

    if (m_selectedNode) {
        Proto *def = m_selectedNode->getProto();
        if (m_selectedProto != NULL)
            def = m_selectedProto;
        if (def) {            
            m_fields.resize(0);
            int numFields = 0;
            for (int i = 0; i < def->getNumFields(); i++) {
                Field *field = def->getField(i);
                if (m_scene->isInvalidElement(field))
                    continue;
                if (TheApp->is4Kids() && (!m_selectedNode->hasNumbers4kids()))
                    if (!(field->getFlags() & FF_4KIDS))
                        continue;
                FieldValue *value = NULL;
                if (m_selectedProto != NULL) {
                    value = m_selectedProto->getField(i)->getDefault(true);
                    value->ref();
                } else if (m_selectedNode != NULL)
                    value = m_selectedNode->getField(i);
                Rect r1;
                GetItemRect(numFields, &r1);
                Rect rect(width0, r1.top, width0 + width1 - 1, r1.bottom);
                m_fields.append(FieldViewItem::CreateItem(field, this));
                m_fields[numFields]->SetIndex(i);
                m_fields[numFields]->CreateControl(rect, m_window);
                m_fields[numFields]->SetValue(value);
/*
                for (int j = 0; j < collapsedNames.size(); j++)
                    if (strcmp(collapsedNames[j], field->getName(x3d)) == 0)
                        m_fields[numFields]->SetFlag(FVIS_COLLAPSED);
*/
                numFields++;
            }
        }
    }

    m_isRoot = false;
    if ((m_selectedNode == m_scene->getRoot()) && (m_selectedProto == NULL))
        m_isRoot = true;    

    if (resetSelection)
        m_selectedItem = m_selectedField = -1;

    RefreshItemList();
    swInvalidateWindow(m_window);
}

void FieldView::DrawItem(int index, SDC dc)
{
    bool x3d = m_selectedNode ? m_selectedNode->getScene()->isX3d() : true;
    Proto *def = NULL;
    if (m_selectedProto != NULL)
        def = m_selectedProto;
    else if (m_selectedNode != NULL)
       def = m_selectedNode->getProto();
    if (def == NULL)
        return;
    Field *field;
    if (index >= m_items.size())
        return;
    FieldViewItem *item = m_items[index];
    if (item == NULL)
        return;
    FieldViewItem *parent = item->GetParent();
    int cx = GetColumnWidth(0);
    Rect r;

    GetItemRect(index, &r);

    swSetClipRect(dc, 0, r.top, r.Width(), m_itemHeight);

    if (m_isRoot && !TheApp->getX3domMode())
        return;

    if (parent) { // item is a MField entry
        field = def->getField(parent->GetIndex());
    } else {
        field = def->getField(item->GetIndex());
        parent = item;
    }

    int fg = 0;
    if (item->GetState() & FVIS_SELECTED) {
        int highlight = swGetWindowColor(m_window, SW_COLOR_HIGHLIGHT);
        swSetFGColor(dc, highlight);
        swFillRect(dc, r.left, r.top, r.Width(), r.Height());
        swSetBGColor(dc, highlight);
        fg = swGetWindowColor(m_window, SW_COLOR_HIGHLIGHT_TEXT);
        swSetFGColor(dc, fg);
    } else {
        int bg = swGetWindowColor(m_window, SW_COLOR_WINDOW_BG);
        swSetFGColor(dc, bg);
        swFillRect(dc, r.left, r.top, r.Width(), r.Height());
        swSetBGColor(dc, bg);
        if (parent->GetValue()) {
            if (parent->GetValue()->equals(field->getDefault(x3d))) {
                fg = swGetWindowColor(m_window, SW_COLOR_HIGHLIGHT);
            } else {
                fg = swGetWindowColor(m_window, SW_COLOR_TEXT);
            }
        }
        swSetFGColor(dc, fg);
    }

    swSetClipRect(dc, 0, r.top, cx, m_itemHeight);

    if (item->IsGroup() && ((MFieldViewItem *) item)->GetNumChildren() > 0) {
        // draw a collapse box
        if (item->IsCollapsed()) {
            swDrawPlusBox(dc, CB_SPACING, r.top + (m_itemHeight - CB_HEIGHT) / 2);
        } else {
            swDrawMinusBox(dc, CB_SPACING, r.top + (m_itemHeight - CB_HEIGHT) / 2);
        }
        swSetFGColor(dc, fg);
    }

    MyString str;
    int x = CB_WIDTH + CB_SPACING * 2;
    int y = r.bottom - 2;

    if (item->GetParent()) {
        // it's an element of an MField; draw a plus sign to insert a new index
        int extraSpace = 0;
        if (item->IsCollapsed()) {
            extraSpace = CB_ARRAY_INSERT;
            swDrawPlusBox(dc, extraSpace, 
                          r.top + (m_itemHeight - CB_HEIGHT) / 2);
            swSetFGColor(dc, fg);
        }

        // it's an element of an MField; draw the array index, right-justified
        char buf[128];
        mysnprintf(buf, 128, "[%d] ", item->GetIndex());
        swDrawText(dc, x + extraSpace, y, buf);
    } else if (field->getFlags() & FF_KAMBI_DEPRECATED) {
        char deprecated[256];
        swLoadString(IDS_DEPRECATED, deprecated, 255);
        MyString fieldName = "";
        fieldName += "(";
        fieldName += deprecated;
        fieldName += ") ";
        fieldName += field->getName(x3d);
        swDrawTextTruncated(dc, x, r.top, cx - x, r.Height() - 1, 
                            fieldName);
    } else {
        // is a parent node, draw the name
        swDrawTextTruncated(dc, x, r.top, cx - x, r.Height() - 1, 
                            field->getName(x3d));
    }

    swSetClipRect(dc, r.left, r.top, r.Width(), m_itemHeight);

    if (item->GetState() & FVIS_SELECTED) {
        fg = swGetWindowColor(m_window, SW_COLOR_WINDOW_BG);
        int bg = swGetWindowColor(m_window, SW_COLOR_HIGHLIGHT);
        swSetBGColor(dc, bg);
    } else if (parent->GetValue()) {
        if (parent->GetValue()->equals(field->getDefault(x3d))) {
            fg = swGetWindowColor(m_window, SW_COLOR_HIGHLIGHT);
            int bg = swGetWindowColor(m_window, SW_COLOR_WINDOW_BG);
            swSetBGColor(dc, bg);
        } else {
            fg = swGetWindowColor(m_window, SW_COLOR_TEXT);
            int bg = swGetWindowColor(m_window, SW_COLOR_WINDOW_BG);
            swSetBGColor(dc, bg);
        }
    }

    swSetFGColor(dc, fg);
    swFillPatternRect(dc, 0, r.bottom, r.Width(), 1, m_halftoneBitmap);
    swFillPatternRect(dc, cx-1, r.top, 1, r.Height(), m_halftoneBitmap);

    if (!item->isZero())
        item->Draw(dc, cx + 2, r.top);
}

void FieldView::OnHeaderChange(int index, int width)
{
    if (index == 0) {
        int w, h;
        swHeaderGetSize(m_header, &w, &h);
        swHeaderSetItemWidth(m_header, 0, width);
        swHeaderSetItemWidth(m_header, 1, w - width);
        MoveControls(width);
        swInvalidateRect(m_window, 0, h, w, m_height);
    }
}

int FieldView::GetColumnWidth(int column) const
{
    return swHeaderGetItemWidth(m_header, column);
}

void FieldView::SetColumnWidth(int column, int width)
{
    swHeaderSetItemWidth(m_header, column, width);
}

void FieldView::MoveControls(int left)
{
    Rect r;

    for (int i = 0; i < m_items.size(); i++) {
        GetItemRect(i, &r);
        m_items[i]->MoveControl(left, r.top);
    }
}

int FieldView::HitTest(int x, int y)
{
    int width, height;

    swHeaderGetSize(m_header, &width, &height);
    int pos = (y - height) / m_itemHeight;

    if (pos >= 0 && pos < m_items.size()) {
        return pos;
    } else {
        return -1;
    }
}

void FieldView::GetItemRect(int i, Rect *r)
{
    int width, height;

    swGetSize(m_window, &width, &height);
    r->top = (i+1 - m_scrollY)*m_itemHeight;
    r->bottom = r->top + m_itemHeight - 1;
    r->left = 0;
    r->right = width - 1;


}

void FieldView::GetFieldRect(int field, int index, Rect *r)
{
    int width, height;
    swHeaderGetSize(m_header, &width, &height);
    int top = height - m_scrollY * m_itemHeight;

    for (int i = 0; i < field; i++) {
        if (m_fields[i]->IsGroup() && !m_fields[i]->IsCollapsed()) {
            top += (((MFieldViewItem *) m_fields[i])->GetNumChildren() + 1) * m_itemHeight;
        } else {
            top += m_itemHeight;
        }
    }

    if (index == -1) {
        if ((field != -1) &&
            (m_fields[field]->IsGroup() && !m_fields[field]->IsCollapsed())) {
            r->top = top;
            r->bottom = (((MFieldViewItem *) m_fields[field])->GetNumChildren() + 1) * m_itemHeight - 1;
        } else {
            r->top = top;
            r->bottom = top + m_itemHeight - 1;
        }
    } else {
        r->top = top + (index+1) * m_itemHeight;
        r->bottom = r->top + m_itemHeight - 1;
    }
    r->left = 0;
    r->right = width - 1;
}

void FieldView::OnSize(int width, int height) 
{
    swSetSize(m_scroller, width, height);
    swSetScrollSizes(m_scroller, m_width, height);
    int widthReduce = 0;
    if (m_header) {
        swHeaderSetSize(m_header, width, m_itemHeight);
        widthReduce = swHeaderGetItemWidth(m_header, 0);
        swHeaderSetItemWidth(m_header, 1, width - widthReduce);
    }
    m_pageHeight = height / m_itemHeight;
    UpdateBars();
}

void FieldView::UpdateBars()
{
    static bool inUpdate = false;
    
    if (!inUpdate) {
        inUpdate = true;

        int x = 0;
        int y = 0; 
        int width = 0;
        int height = 0;

        swGetPosition(m_window, &x, &y);
        swGetSize(m_window, &width, &height);

        if (m_height > y + height) {
//            SCROLLINFO info;
//            info.cbSize = sizeof(SCROLLINFO);
//            info.fMask = SIF_PAGE | SIF_RANGE;
//            info.nMin = 0;
//            if (m_items.size() > 32767) {
//                info.nMax = 32767;
             m_scrollRatio = m_items.size() / 32767.0f;
//            } else {
//              info.nMax = m_items.size();
                m_scrollRatio = 1.0f;
//            }
//            info.nPage = m_pageHeight;
//            SetScrollInfo(SB_VERT, &info);
            if (m_items.size() > m_pageHeight) {
//                EnableScrollBarCtrl(SB_VERT, TRUE);
            } else {
//                EnableScrollBarCtrl(SB_VERT, FALSE);
                m_scrollY = 0;
            }
        } else {
//            EnableScrollBarCtrl(SB_VERT, FALSE);
            m_scrollY = 0;
        }

        inUpdate = false;
    }
}

void FieldView::StartEditing()
{
    Rect r;
    int cx = GetColumnWidth(0);
    MyString buf;
    
    m_scene->UpdateViews(this, UPDATE_START_FIELD_EDIT);
    FieldViewItem *item = m_items[m_selectedItem];
    m_selectedOffset = item->GetFieldOffset(m_trackPoint.x - cx);

    GetItemRect(m_selectedItem, &r);   

    item->StartEditing(buf, m_selectedOffset);

#ifdef _WIN32
    m_edit = swCreateTextEdit(SW_SINGLE_LINE,
                              cx + m_selectedOffset * m_floatWidth, r.top - 4,
                              r.Width(), r.Height() + 8, m_window);
#else
    m_edit = swCreateTextEdit(SW_SINGLE_LINE,
                              cx + m_selectedOffset * m_floatWidth, r.top,
                              r.Width(), r.Height(), m_window);
#endif

    swSetText(m_edit, buf);
    swTextEditSetSelection(m_edit, 0, strlen(buf));
    swSetCommandCallback(m_edit, textCommandCallback);
    swSetFocusCallback(m_edit, textFocusCallback);
    swSetClientData(m_edit, this);
    swSetFocus(m_edit);
    swEnableAccelerators(FALSE);
    m_state = EDITING;
}

void FieldView::StopEditing()
{
    if (m_state != EDITING) return;

    Node *node = m_selectedNode;
    if (m_selectedProto == NULL) {
        static FieldUpdate hint(node, m_selectedField, m_selectedIndex);
        m_scene->UpdateViews(this, UPDATE_STOP_FIELD_EDIT, (Hint *) &hint);
    }
    FieldValue *newValue;
    char str[128];

    m_state = NORMAL;
    swGetText(m_edit, str, 128);
    newValue = m_items[m_selectedItem]->StopEditing(str, m_selectedOffset);

    if (newValue != NULL) {
        if (m_items[m_selectedItem]->GetParent()) {
            MFieldValue *mvalue = NULL;
            if (m_selectedProto != NULL)
                mvalue = (MFieldValue *)
                         m_selectedProto->getField(m_selectedField)->
                                          getDefault(true);
            else
                mvalue = (MFieldValue *) 
                         m_selectedNode->getField(m_selectedField);
            mvalue->setSFValue(m_selectedIndex, newValue);
            m_items[m_selectedItem]->SetValue(newValue);
            if (m_selectedProto != NULL)
                m_selectedProto->getField(m_selectedField)->
                                 setDefault(mvalue->copy());
            else
                m_scene->setField(m_selectedNode, m_selectedField, 
                                  mvalue->copy());
            swInvalidateWindow(m_window);
        } else {
            if (m_selectedProto != NULL)
                m_selectedProto->getField(m_selectedField)->
                                 setDefault(newValue);
            else {
                m_scene->backupField(m_selectedNode, m_selectedField);
                m_scene->setField(m_selectedNode, m_selectedField, newValue);
            }
        }
    }
    swDestroyWindow(m_edit);
    swEnableAccelerators(TRUE);
    if (m_selectedItem < m_items.size())
        if (m_items[m_selectedItem] && !m_items[m_selectedItem]->IsCollapsed())
            RefreshItemList();
    if (m_selectedProto != NULL)
         UpdateAll(false);
    swInvalidateWindow(m_window);
}

void FieldView::AbortEditing()
{
    m_state = NORMAL;
    swDestroyWindow(m_edit);
}

void FieldView::OnLButtonDown(int x, int y, int modifiers) 
{
    y += m_scrollY * m_itemHeight;
    int hit = HitTest(x, y);

    Rect r;

    if (m_state == EDITING) {
        StopEditing();
    } else if ((hit >= 0) && (hit < m_items.size())) {
        FieldViewItem *item = m_items[hit];
        if (m_selectedItem != hit) {
            // unhighlight old selection, if any
            if ((m_selectedItem > 0) && (m_selectedItem < m_items.size())) { 
                if (m_items[m_selectedItem] != NULL)
                    m_items[m_selectedItem]->ClearFlag(FVIS_SELECTED);
                swInvalidateWindow(m_window);
            }

            // highlight new selection
            item->SetFlag(FVIS_SELECTED);
            m_selectedItem = hit;
            swInvalidateWindow(m_window);
        }

        bool colorcircle_flag=false;
        if (item->GetParent()) {
            // selected item is a child of an MField; 
            // use the parent's field index
            m_selectedField = item->GetParent()->GetIndex();
            m_selectedIndex = item->GetIndex();
            m_scene->setViewOfLastSelection(this);
            int type = -1;
            if (m_selectedProto != NULL)
                type = m_selectedProto->getField(m_selectedField)->getType();
            else
                type = m_selectedNode->getField(m_selectedField)->getType();
            if (item->IsCollapsed() && 
                (x > CB_ARRAY_INSERT) && (x < CB_ARRAY_INSERT + CB_WIDTH) &&
                (type != MFNODE)
               ) {
                // add to array box was picked
                if (item->GetParent() != NULL) {
                    ((MFieldViewItem *)item->GetParent())->InsertItem(this, 
                    m_selectedIndex + 1);
                    if (m_selectedProto == NULL)
                        m_selectedNode->update();
                }
                RefreshItemList();
                // move all controls below the item toggled
                MoveControls(GetColumnWidth(0));
                swInvalidateWindow(m_window);
            } 
            FieldUpdate hint(m_scene->getSelection()->getNode(),
                             m_selectedField,m_selectedIndex);
            m_scene->UpdateViews(this,UPDATE_SELECTED_FIELD, (Hint *) &hint);
            if ((m_selectedProto == NULL) &&
                (m_selectedNode->getField(m_selectedField)->getType() == 
                 MFCOLOR)) {
               colorcircle_flag = true;
               m_scene->UpdateViews(this, UPDATE_ENABLE_COLOR_CIRCLE,
                                   (Hint *) &hint);
            }
        } else {
            // otherwise it's a SField, use its own index
            m_selectedField = item->GetIndex();
            m_selectedIndex = -1;
            m_scene->setViewOfLastSelection(this);
            FieldUpdate hint(m_scene->getSelection()->getNode(), m_selectedField);
            m_scene->UpdateViews(this, UPDATE_SELECTED_FIELD, (Hint *) &hint);
            if ((m_selectedProto == NULL) &&
                (m_selectedNode->getField(m_selectedField)->getType() == 
                 SFCOLOR)) {
                colorcircle_flag = true;
                m_scene->UpdateViews(this, UPDATE_ENABLE_COLOR_CIRCLE,
                                     (Hint *) &hint);
            }
        }
        if (!colorcircle_flag)
           m_scene->UpdateViews(this,UPDATE_DISABLE_COLOR_CIRCLE);

        GetItemRect(hit, &r);
        int width0 = GetColumnWidth(0);
        if (x > width0) {
            if (item->IsClickable()) {
                item->OnClicked(m_selectedIndex);
            }
            FieldValue *value = item->OnMouseDown(x - width0, y - r.top, 0);
            if (value && !value->equals(item->GetValue())) {
                if (m_selectedProto == NULL) {
                    if (item->GetParent()) {
                        m_scene->execute(new MFieldCommand(m_selectedNode, 
                                                           m_selectedField, 
                                                           m_selectedIndex, 
                                                           value));
                    } else {
                        m_scene->execute(new FieldCommand(m_selectedNode, 
                                                          m_selectedField, 
                                                          value));
                    }
                } else
                    m_selectedProto->getField(m_selectedField)->
                        setDefault(value);
            } else if (item->IsEditable()) {
                m_state = WAIT_EDIT;
            } else if (item->IsTrackable()) {
                m_state = WAIT_TRACK;
            } else if (item->GetValue()->getType() == SFNODE) {
                Node *child = ((SFNode *) item->GetValue())->getValue();

                if (child) {
                    m_scene->setSelection(child);
                    m_scene->UpdateViews(this, UPDATE_SELECTION);
                    return;
                }
            }
        }

        if (item->IsGroup() && x > CB_SPACING && x < CB_SPACING + CB_WIDTH) {
            // collapse box was picked
            item->SetCollapsed(!item->IsCollapsed());
            RefreshItemList();
            // move all controls below the item toggled
            MoveControls(GetColumnWidth(0));
            swInvalidateWindow(m_window);
        } else {
            m_trackPoint = Point(x, y);
            m_selectedOffset = item->GetFieldOffset(x - width0);
            if (m_selectedOffset > 2)
                m_selectedOffset = 2;
        }
    }
}

// test if a mousedrag would change a value
// if yes, show a cursor different from usual Arrow

void
FieldView::UpdateTrackingCursor(int x, int y)
{
    bool cursorIsArrow = true;
    int hit=HitTest(x, y);
    if (hit != -1)
        if (m_items[hit] && (m_items[hit]->IsTrackable())) {
            Rect r;
            GetItemRect(hit, &r);
            int width0 = GetColumnWidth(0);
            if ((x > width0) && (x - width0 < GetItemWidth()))
                cursorIsArrow = false;
        }
    if ((cursorIsArrow) && (!m_cursorIsArrow))
        swSetCursor(m_window, m_cursorArrow);
    if ((!cursorIsArrow) && (m_cursorIsArrow))
        swSetCursor(m_window, m_cursorHMove);     
     m_cursorIsArrow = cursorIsArrow;
}

void FieldView::OnMouseMove(int x, int y, int modifiers) 
{
    FieldValue *newValue;

    if (m_selectedItem == -1)
        return;
    if (m_state == WAIT_EDIT || m_state == WAIT_TRACK) {
        if (m_items[m_selectedItem]->IsTrackable()) {
            StartTracking();
        } else {
            m_state = NORMAL;
        }
    }
    UpdateTrackingCursor(x, y);
    if ((m_state == TRACKING) && 
        (m_selectedItem > -1) && (m_selectedItem < m_items.size())) {
        FieldViewItem  *item = m_items[m_selectedItem];
        if (item == NULL)
            return;
        int delta = x - m_trackPoint.x;
        newValue = item->OnMouseMove(m_trackValue, m_selectedOffset, delta);
        ChangeValue(item, newValue);
    }
}

void FieldView::ChangeValue(FieldViewItem *item, FieldValue *newValue)
{
    assert(item && newValue);

    if ((!newValue->equals(item->GetValue())) || 
        (isMFType(newValue->getType()))) {
        int field = m_selectedField;
        Proto *def = NULL;
        if (m_selectedProto != NULL)
            def = m_selectedProto;
        else
            def = m_selectedNode->getProto();
        newValue->clamp(def->getField(field)->getMin(),
                        def->getField(field)->getMax());
        int index = m_selectedIndex;        
        if (item->GetParent()) {
            MFieldValue *mvalue = NULL;
            if (m_selectedProto != NULL)
                mvalue = (MFieldValue *) m_selectedProto->getField(field)->
                                         getDefault(true);
            else
                mvalue = (MFieldValue *) m_selectedNode->getField(field);
            mvalue->setSFValue(m_selectedIndex, newValue);
            item->SetValue(newValue);
            if (m_selectedProto != NULL)
                m_selectedProto->getField(field)->setDefault(mvalue);
            else
                m_scene->setField(m_selectedNode, field, mvalue);
            swInvalidateWindow(m_window);
        } else if (isMFType(newValue->getType())) {             
            // index is 0, field is collapsed
            index = 0;
            MFieldValue *value;
            if (m_selectedProto != NULL)
                value = (MFieldValue *) m_selectedProto->getField(field)->
                                        getDefault(true);
            else
                value = (MFieldValue *) m_selectedNode->getField(field);
            FieldValue *sfValue = ((MFieldValue *)newValue)->getSFValue(0);
            value->setSFValue(index, sfValue);
            ((MFieldViewItem *)item)->InitIndexValue(0, newValue);
            if (m_selectedProto != NULL) {
                m_selectedProto->getField(field)->setDefault(value);
                UpdateAll(false);
            } else
                m_scene->setField(m_selectedNode, field, value);
            swInvalidateWindow(m_window);
        } else {
            if (m_selectedProto != NULL) {
                m_selectedProto->getField(field)->setDefault(newValue);
                UpdateAll(false);
            } else
                m_scene->setField(m_selectedNode, field, newValue);
            m_fields[FindField(field)]->SetValue(newValue);
        } 
        if (m_selectedProto == NULL) {
            FieldUpdate update(m_selectedNode, field, index);
            m_scene->UpdateViews(this, UPDATE_FIELD, (Hint *) &update);
        }
    } else {
        delete newValue;
    }
}

void FieldView::OnLButtonUp(int x, int y, int modifiers) 
{
    if (m_state == TRACKING) {
        StopTracking();
    } else if (m_state == WAIT_EDIT) {
        StartEditing();
    } else {
        m_state = NORMAL;
    }
}

void FieldView::StartTracking()
{
    Command *cmd;

    m_state = TRACKING;
    swSetCapture(m_window);
    m_trackValue = m_items[m_selectedItem]->GetValue();
    m_trackValue->ref();
    if (m_selectedProto == NULL) {
        if (m_selectedIndex != -1) {
            cmd = new MFieldCommand(m_selectedNode, m_selectedField, 
                                    m_selectedIndex);
        } else {
            cmd = new FieldCommand(m_selectedNode, m_selectedField);
        }
        m_scene->add(cmd);
    } else {
        Field *field = m_selectedProto->getField(m_selectedField);
        if (isMFType(field->getType())) {
            MFieldValue *mvalue = (MFieldValue *) field->getDefault(true);
            mvalue->setSFValue(m_selectedIndex, m_trackValue);
            field->setDefault(mvalue);
        } else 
            field->setDefault(m_trackValue);
    }
}

void FieldView::StopTracking()
{
    m_state = NORMAL;
    swReleaseCapture(m_window);
    m_trackValue->unref();
    m_trackValue = NULL;
}

void FieldView::RefreshItemList()
{
    int n = m_fields.size();
    int item = 0;

    m_items.resize(0);
    for (int i = 0; i < n; i++) {
        m_items[item++] = m_fields[i];
        if (m_fields[i]->IsGroup() && !m_fields[i]->IsCollapsed()) {
            MFieldViewItem *mf = (MFieldViewItem *) m_fields[i];
            int nc = mf->GetNumChildren();
            for (int j = 0; j < nc; j++) {
                m_items[item++] = mf->GetChild(j);
                mf->GetChild(j)->SetIndex(j);
            }
        }
    }
    m_height = (m_items.size()+1) * m_itemHeight;
    int width, height;
    swGetSize(m_window, &width, &height);
    swSetScrollSizes(m_scroller, m_width, m_height);
    m_pageHeight = height / m_itemHeight;
    UpdateBars();
}

void FieldView::DeleteLastSelection(void)
{
    if ((m_state == NORMAL) && (m_selectedItem > -1)) {
        FieldViewItem *item = m_items[m_selectedItem];
        MFieldViewItem *parent = (MFieldViewItem *)item->GetParent();
        if (parent) {
            // selected item is a child of an MField; 
            parent->RemoveItem(this, item->GetIndex());
            m_selectedField = -1;
            m_selectedItem = -1;
            RefreshItemList();
            Node *node = m_scene->getSelection()->getNode();                    
            if (node != NULL)
                node->update();
            // move all controls below the item toggled
            MoveControls(GetColumnWidth(0));
            swInvalidateWindow(m_window);
            m_scene->UpdateViews(this, UPDATE_SELECTION);
        }
    }
}
