/*
 * SFVec2dItem.cpp
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

#include "SFVec2dItem.h"
#include "SFVec2d.h"
#include "FieldView.h"
#include "swt.h"

SFVec2dItem::SFVec2dItem(FieldView *view)
  : FieldViewItem(view)
{
}

void
SFVec2dItem::Draw(SDC dc, int x, int y)
{
    DrawFloats(((SFVec2d *) m_value)->getValue(), 2, "XY", dc, x, 
               y+m_view->GetItemHeight()-3, m_view->GetFloatWidth());
}

FieldValue *
SFVec2dItem::OnMouseMove(FieldValue *value, int index, int delta)
{
    SFVec2d *v = (SFVec2d *) value;
    SFVec2d *newValue = new SFVec2d(v->getValue());
    newValue->setValue(index, v->getValue(index) + delta * 0.02f);
    return newValue;
}

int
SFVec2dItem::GetFieldOffset(int xpos) const
{
    int offset = xpos / m_view->GetFloatWidth();
    if (offset > 1) offset = 1;
    return offset;
}

void
SFVec2dItem::StartEditing(MyString &str, int offset)
{
    char buf[128];

    mysnprintf(buf, 128, "%g", ((SFVec2d *) m_value)->getValue(offset));
    str = buf;
}

FieldValue *
SFVec2dItem::StopEditing(const char *str, int offset)
{
    SFVec2d *newValue = new SFVec2d(((SFVec2d *) m_value)->getValue());
    newValue->setValue(offset, atof(str));
    return newValue;
}
