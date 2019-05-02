/*
 * SFStringItem.h
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

#ifndef _SFSTRING_ITEM_H
#define _SFSTRING_ITEM_H

#include "FieldViewItem.h"

class SFStringItem : public FieldViewItem {
public:
                        SFStringItem(FieldView *view);
                       ~SFStringItem();

    virtual void        CreateControl(const Rect &rect);
    virtual void        CreateControl(const Rect &rect, SWND wnd);
    virtual void        MoveControl(int x, int y);
    virtual void        Draw(SDC dc, int x, int y);
    virtual bool        IsTrackable() const { return false; }
    virtual bool        IsEditable() const { return m_comboBox == NULL; }
    virtual void        StartEditing(MyString &str, int offset);
    virtual FieldValue *StopEditing(const char *str, int offset);
    virtual FieldValue *OnCommand(int id);
    virtual FieldValue *OnMouseDown(int x, int y, int modifiers);
    virtual void        UpdateControl();

protected:
    SWND                m_comboBox;
    const char        **_strings;
};

#endif // _SFSTRING_ITEM_H
