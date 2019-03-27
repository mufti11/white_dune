/*
 * SFMatrix3fItem.cpp
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

#include "SFMatrix3fItem.h"
#include "SFMatrix3f.h"
#include "FieldView.h"
#include "swt.h"

SFMatrix3fItem::SFMatrix3fItem(FieldView *view)
  : FieldViewItem(view)
{
}

void
SFMatrix3fItem::Draw(SDC dc, int x, int y)
{
    DrawFloats(((SFMatrix3f *) m_value)->getValue(), 9, "XYZ", dc, x, y+m_view->GetItemHeight()-9, m_view->GetFloatWidth());
}

FieldValue *
SFMatrix3fItem::OnMouseMove(FieldValue *value, int index, int delta)
{
    SFMatrix3f *v = (SFMatrix3f *) value;
    SFMatrix3f *newValue = new SFMatrix3f(v->getValue());
    newValue->setValue(index, v->getValue(index) + delta * 0.02f);
    return newValue;
}

int
SFMatrix3fItem::GetFieldOffset(int xpos) const
{
    int offset = xpos / m_view->GetFloatWidth();
    if (offset > 2) offset = 2;
    return offset;
}

void
SFMatrix3fItem::StartEditing(MyString &str, int offset)
{
    char buf[128];

    mysnprintf(buf, 128, "%g", ((SFMatrix3f *) m_value)->getValue(offset));
    str = buf;
}

FieldValue *
SFMatrix3fItem::StopEditing(const char *str, int offset)
{
    SFMatrix3f *newValue = new SFMatrix3f(((SFMatrix3f *) m_value)->getValue());
    newValue->setValue(offset, (float) atof(str));
    return newValue;
}
