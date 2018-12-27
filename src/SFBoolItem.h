/*
 * SFBoolItem.h
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

#ifndef _SFBOOL_ITEM_H
#define _SFBOOL_ITEM_H

#include "FieldViewItem.h"
#include "swt.h"

class SFBoolItem : public FieldViewItem {
public:
                        SFBoolItem(FieldView *view);
    virtual            ~SFBoolItem();

    virtual void        MoveControl(int x, int y);

    virtual void        CreateControl(const Rect &rect);
    void                CreateControl(const Rect &rect, SWND wnd);

    virtual void        UpdateControl();
    void                OnClicked();

    // only for MFBool handling
    virtual void        Draw(SDC dc, int x, int y);


private:
    SWND                m_button;
};

#endif // _SFBOOL_ITEM_H
