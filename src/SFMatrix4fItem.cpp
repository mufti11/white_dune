/*
 * SFMatrix4fItem.cpp
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

#include "SFMatrix4fItem.h"
#include "SFMatrix4f.h"
#include "FieldView.h"
#include "swt.h"

SFMatrix4fItem::SFMatrix4fItem(FieldView *view)
  : FieldViewItem(view)
{
}

void
SFMatrix4fItem::Draw(SDC dc, int x, int y)
{
    DrawFloats(((SFMatrix4f *) m_value)->getValue(), 16, "123456789abcef0", dc, x, y+m_view->GetItemHeight()-9, m_view->GetFloatWidth());
}

FieldValue *
SFMatrix4fItem::OnMouseMove(FieldValue *value, int index, int delta)
{
    SFMatrix4f *v = (SFMatrix4f *) value;
    SFMatrix4f *newValue = new SFMatrix4f(v->getValue());
    newValue->setValue(index, v->getValue(index) + delta * 0.02f);
    return newValue;
}

int
SFMatrix4fItem::GetFieldOffset(int xpos) const
{
    int offset = xpos / m_view->GetFloatWidth();
    if (offset > 2) offset = 2;
    return offset;
}

void
SFMatrix4fItem::StartEditing(MyString &str, int offset)
{
    char buf[128];

    mysnprintf(buf, 128, "%g", ((SFMatrix4f *) m_value)->getValue(offset));
    str = buf;
}

FieldValue *
SFMatrix4fItem::StopEditing(const char *str, int offset)
{
    SFMatrix4f *newValue = new SFMatrix4f(((SFMatrix4f *) m_value)->getValue());
    newValue->setValue(offset, (float) atof(str));
    return newValue;
}
