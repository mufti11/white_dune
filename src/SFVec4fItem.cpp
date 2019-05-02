/*
 * SFVec4fItem.cpp
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

#include "SFVec4fItem.h"
#include "SFVec4f.h"
#include "FieldView.h"
#include "swt.h"

SFVec4fItem::SFVec4fItem(FieldView *view)
  : FieldViewItem(view)
{
}

void
SFVec4fItem::Draw(SDC dc, int x, int y)
{
    DrawFloats(((SFVec4f *) m_value)->getValue(), 4, "XYZ?", dc, x, y+m_view->GetItemHeight()-3, m_view->GetFloatWidth());
}

FieldValue *
SFVec4fItem::OnMouseMove(FieldValue *value, int index, int delta)
{
    SFVec4f *v = (SFVec4f *) value;
    SFVec4f *newValue = new SFVec4f(v->getValue());
    newValue->setValue(index, v->getValue(index) + delta * 0.02f);
    return newValue;
}

int
SFVec4fItem::GetFieldOffset(int xpos) const
{
    int offset = xpos / m_view->GetFloatWidth();
    if (offset > 3) offset = 3;
    return offset;
}

void
SFVec4fItem::StartEditing(MyString &str, int offset)
{
    char buf[128];

    mysnprintf(buf, 128, "%g", ((SFVec4f *) m_value)->getValue(offset));
    str = buf;
}

FieldValue *
SFVec4fItem::StopEditing(const char *str, int offset)
{
    SFVec4f     *newValue = new SFVec4f(((SFVec4f *) m_value)->getValue());
    newValue->setValue(offset, (float) atof(str));
    return newValue;
}
