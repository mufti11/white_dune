/*
 * FieldView.h
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

#pragma once

#include "Array.h"
#include "SceneView.h"
#include "Rect.h"
#include "Point.h"
#include "SFMFTypes.h"
#include "swttypedef.h"

class FieldViewItem;
class DuneDoc;
class Node;
class FieldValue;
class Hint;

class FieldView : public SceneView
{
public:
                        FieldView(Scene *scene, SWND wnd);
        virtual        ~FieldView();

public:

        virtual void    OnUpdate(SceneView *sender, int type, Hint *hint);
        virtual void    OnDraw(int x, int y, int width, int height,
                               bool update = false);
        virtual void    OnSize(int width, int height);
        virtual void    OnLButtonDown(int x, int y, int modifiers);
        virtual void    OnLButtonUp(int x, int y, int modifiers);
        virtual void    OnMouseMove(int x, int y, int modifiers);

        const char     *getName(void) const { return "FieldView"; }

        void            DeleteView();
        void            DeleteFields();

        void            OnHeaderChange(int pos, int width);

        int             GetColumnWidth(int column) const;
        SFONT           GetUFont() const { return m_uFont; }
        int             GetFloatWidth() const { return m_floatWidth; }

        int             GetItemWidth() const { return GetColumnWidth(1); }
        int             GetItemHeight() const { return m_itemHeight; }

// virtual BOOL PreTranslateMessage(MSG* pMsg);
// virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);

        void            DrawItem(int item, SDC dc);
        void            StartEditing();
        void            StopEditing();
        void            AbortEditing();
        void            StartTracking();
        void            StopTracking();
        void            SetColumnWidth(int column, int width);
        void            GetItemRect(int i, Rect *r);
        void            GetFieldRect(int field, int index, Rect *r);
        int             HitTest(int x, int y);
        void            MoveControls(int left);
        void            UpdateAll(bool resetSelection = false);
        void            SelectField(int field) {m_selectedField=field;}
        int             GetSelectedField(void) {return m_selectedField;}
        Node           *GetSelectedNode(void) {return m_selectedNode;}
        int             FindField(int field);

        void            RefreshItemList();
        void            ChangeValue(FieldViewItem *item, FieldValue *newValue);
        void            UpdateBars();

        void            DeleteLastSelection(void);
        void            CopyLastSelection(void);
        void            PasteLastSelection(void);
        void            PasteSymetricLastSelection(int direction);

        bool            canPaste() { return m_copiedFieldValue != NULL; }

        virtual bool    isFieldView(void) { return true; }

//  void OnItemclick(NMHDR* pNMHDR, LRESULT* pResult);
//  void OnLButtonDblClk(UINT nFlags, CPoint point);
//  void OnButtonClicked(UINT id);
//  void OnButtonKillfocus(UINT id);

        void            UpdateTrackingCursor(int x, int y);

protected:

        enum            FVState { 
                            NORMAL, 
                            WAIT_EDIT, 
                            WAIT_TRACK, 
                            TRACKING, 
                            EDITING 
                        };

        SWND            m_window;
        SWND            m_scroller;
        int             m_lastXPosition;
        int             m_lastYPosition;
        int             m_maxYPosition;

        int             m_autoScrolling;
        int             m_autoScrollPY;
        int             m_autoScrollDY;

        Node           *m_selectedNode;
        int             m_selectedField;
        int             m_selectedIndex; // for MField's
        Proto          *m_selectedProto;
        int             m_selectedOffset;
        int             m_selectedItem;
        SWND            m_edit;
        SHEADER         m_header;
        SBITMAP         m_halftoneBitmap;
        int             m_itemHeight;
        int             m_height;
        int             m_width;
        int             m_floatWidth;
        int             m_pageHeight;
        SFONT           m_uFont;
        int             m_scrollY; // in lines
        float           m_scrollRatio;

        FVState         m_state;

        Point           m_trackPoint;
        FieldValue     *m_trackValue;

        MyArray<FieldViewItem *> m_items;
        MyArray<FieldViewItem *> m_fields;

        bool            m_fieldViewActive;
        bool            m_isRoot;

        SCURSOR         m_cursorArrow;
        SCURSOR         m_cursorHMove;
 
        bool            m_cursorIsArrow;
        FieldValue     *m_copiedFieldValue;
};

