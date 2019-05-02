/*
 * SFVec2fItem.cpp
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

#include "SFVec2fItem.h"
#include "SFVec2f.h"
#include "FieldView.h"
#include "swt.h"

SFVec2fItem::SFVec2fItem(FieldView *view)
  : FieldViewItem(view)
{
}

void
SFVec2fItem::Draw(SDC dc, int x, int y)
{
    DrawFloats(((SFVec2f *) m_value)->getValue(), 2, "XY", dc, x, 
               y+m_view->GetItemHeight()-3, m_view->GetFloatWidth());
}

FieldValue *
SFVec2fItem::OnMouseMove(FieldValue *value, int index, int delta)
{
    SFVec2f *v = (SFVec2f *) value;
    SFVec2f *newValue = new SFVec2f(v->getValue());
    newValue->setValue(index, v->getValue(index) + delta * 0.02f);
    return newValue;
}

int
SFVec2fItem::GetFieldOffset(int xpos) const
{
    int offset = xpos / m_view->GetFloatWidth();
    if (offset > 1) offset = 1;
    return offset;
}

void
SFVec2fItem::StartEditing(MyString &str, int offset)
{
    char buf[128];

    mysnprintf(buf, 128, "%g", ((SFVec2f *) m_value)->getValue(offset));
    str = buf;
}

FieldValue *
SFVec2fItem::StopEditing(const char *str, int offset)
{
    SFVec2f     *newValue = new SFVec2f(((SFVec2f *) m_value)->getValue());
    newValue->setValue(offset, (float) atof(str));
    return newValue;
}
