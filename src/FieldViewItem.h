/*
 * FieldViewItem.h
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

#ifndef _FIELD_VIEW_ITEM_H
#define _FIELD_VIEW_ITEM_H

#ifndef _ARRAY_H
#include "Array.h"
#endif

#ifndef _RECT_H
#include "Rect.h"
#endif

#ifndef _POINT_H
#include "Point.h"
#endif

#ifndef STRING_H
#include "MyString.h"
#endif

class FieldView;
class FieldValue;
//class Field;
#include "Field.h"
#include "Types.h"

#include "swttypedef.h"

#define FVIS_SELECTED  (1<<0)
#define FVIS_COLLAPSED (1<<1)

class FieldViewItem {
public:
                                FieldViewItem(FieldView *view);
    virtual                    ~FieldViewItem();

    virtual void                CreateControl(const Rect &rect) {}
    virtual void                CreateControl(const Rect &rect, SWND wnd) {}


    virtual void                SetValue(FieldValue *value);
    FieldValue                 *GetValue(void) { return m_value; }

    FieldViewItem              *GetParent(void) const { return m_parent; }
    int                         GetIndex(void) const { return m_index; }
    int                         GetState(void) const { return m_state; }

    void                        SetField(const Field *field) { m_field = field; }

    virtual void                MoveControl(int x, int y) {}

    void                        SetParent(FieldViewItem *parent)
                                   { m_parent = parent; }
    void                        SetIndex(int index) { m_index = index; }
    void                        SetFlag(int flag) { m_state |= flag; }
    void                        ClearFlag(int flag) { m_state &= ~flag; }

    virtual FieldValue         *OnCommand(int id) { return NULL; }
    virtual void                Draw(SDC, int, int) {}
    virtual FieldValue         *OnMouseDown(int, int, int) { return NULL; }

    virtual void                StartEditing(MyString &str, int offset) {}
    virtual FieldValue         *StopEditing(const char *str, int offset) 
                                   { return NULL; }

    virtual bool                IsGroup() const { return false; }
    bool                        IsCollapsed() const 
                                   { return (m_state & FVIS_COLLAPSED) != 0; }

    void                        SetCollapsed(bool collapsed) 
                                { if (collapsed) SetFlag(FVIS_COLLAPSED); 
                                  else ClearFlag(FVIS_COLLAPSED); }
                                
    virtual bool                IsTrackable() const { return false; }
    virtual bool                IsEditable() const { return false; }
    virtual bool                IsClickable() const { return false; }

    virtual void                OnClicked(int offset) {}

    virtual int                 GetFieldOffset(int pos) const { return 0; }

    virtual void                UpdateControl() {}
    virtual FieldValue         *OnMouseMove(FieldValue *value, int index, 
                                            int delta) { return NULL; }
    virtual bool                isZero(void) { return false; }
    static FieldViewItem       *CreateItem(const Field *field, FieldView *view);
    void                        DrawFloats(const float *floats, int len, 
                                           const char *labels, SDC dc, 
                                           int x, int y, int width);
    void                        DrawFloats(const double *floats, int len, 
                                           const char *labels, SDC dc, 
                                           int x, int y, int width);

protected:
    FieldView                  *m_view;
    FieldViewItem              *m_parent; // for items in an MField
    FieldValue                 *m_value;
    int                         m_state;
    const Field                *m_field;

    int                         m_index; // field # for fields, index for MF's
};

class MFieldViewItem : public FieldViewItem {
public:
                        MFieldViewItem(FieldView *view);
    virtual            ~MFieldViewItem();

    int                         GetNumChildren() const
                                   { return m_children.size(); }
    FieldViewItem              *GetChild(int index)
                                   { return m_children[index]; }
    virtual bool                IsGroup() const { return true; }
    virtual FieldViewItem      *CreateSFItem() = 0;
    virtual void                SetValue(FieldValue *value);
    void                        InitIndexValue(int index, FieldValue *value);
    virtual void                InsertSFValue(int index) = 0;
    virtual void                InsertItem(FieldView *view, int index)      
                                    { 
                                    InsertSFValue(index);
                                    m_children.insert(new FieldViewItem(view),index); 
                                    InitIndexValue(index, m_value);
                                    }
    virtual void                RemoveSFValue(int index) {}
    virtual void                RemoveItem(FieldView *view, int index)      
                                    { 
                                    RemoveSFValue(index);
                                    m_children.remove(index); 
                                    }

protected:
    Array<FieldViewItem *>      m_children;
};

#endif // _FIELD_VIEW_ITEM_H
