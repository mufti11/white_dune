/*
 * MFMatrix4fItem.h
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

#include "FieldViewItem.h"
#include "MFFloatItem.h"

class MFMatrix4fItem : public MFFloatItem {
public:
                             MFMatrix4fItem(FieldView *view);

    virtual FieldValue      *OnMouseMove(FieldValue *value, int index, 
                                         int delta);
    virtual FieldValue      *OnMouseDown(int x, int y, int modifiers);
    virtual void             StartEditing(MyString &str, int offset);
    virtual FieldValue      *StopEditing(const char *str, int offset);
    virtual FieldViewItem   *CreateSFItem();
    virtual void             InsertSFValue(int index);
    virtual void             RemoveSFValue(int index);
    virtual int              GetFieldOffset(int xpos) const;
};
