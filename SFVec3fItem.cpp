/*
 * SFVec3fItem.cpp
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

#include "SFVec3fItem.h"
#include "SFVec3f.h"
#include "FieldView.h"
#include "swt.h"

SFVec3fItem::SFVec3fItem(FieldView *view)
  : FieldViewItem(view)
{
}

void
SFVec3fItem::Draw(SDC dc, int x, int y)
{
    DrawFloats(((SFVec3f *) m_value)->getValue(), 3, "XYZ", dc, x, y+m_view->GetItemHeight()-3, m_view->GetFloatWidth());
}

FieldValue *
SFVec3fItem::OnMouseMove(FieldValue *value, int index, int delta)
{
    SFVec3f *v = (SFVec3f *) value;
    SFVec3f *newValue = new SFVec3f(v->getValue());
    newValue->setValue(index, v->getValue(index) + delta * 0.02f);
    return newValue;
}

int
SFVec3fItem::GetFieldOffset(int xpos) const
{
    int offset = xpos / m_view->GetFloatWidth();
    if (offset > 2) offset = 2;
    return offset;
}

void
SFVec3fItem::StartEditing(MyString &str, int offset)
{
    char buf[128];

    mysnprintf(buf, 128, "%g", ((SFVec3f *) m_value)->getValue(offset));
    str = buf;
}

FieldValue *
SFVec3fItem::StopEditing(const char *str, int offset)
{
    SFVec3f     *newValue = new SFVec3f(((SFVec3f *) m_value)->getValue());
    newValue->setValue(offset, (float) atof(str));
    return newValue;
}
