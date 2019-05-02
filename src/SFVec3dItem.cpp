/*
 * SFVec3dItem.cpp
 *
 * Copyright (C) 1999 Stephen F. White, 2006 J. "MUFTI" Scheurich
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

#include "SFVec3dItem.h"
#include "SFVec3d.h"
#include "FieldView.h"
#include "swt.h"

SFVec3dItem::SFVec3dItem(FieldView *view)
  : FieldViewItem(view)
{
}

void
SFVec3dItem::Draw(SDC dc, int x, int y)
{
    DrawFloats(((SFVec3d *) m_value)->getValue(), 3, "XYZ", dc, x, y+m_view->GetItemHeight()-3, m_view->GetFloatWidth());
}

FieldValue *
SFVec3dItem::OnMouseMove(FieldValue *value, int index, int delta)
{
    SFVec3d *v = (SFVec3d *) value;
    SFVec3d *newValue = new SFVec3d(v->getValue());
    newValue->setValue(index, v->getValue(index) + delta * 0.02f);
    return newValue;
}

int
SFVec3dItem::GetFieldOffset(int xpos) const
{
    int offset = xpos / m_view->GetFloatWidth();
    if (offset > 2) offset = 2;
    return offset;
}

void
SFVec3dItem::StartEditing(MyString &str, int offset)
{
    char buf[128];

    mysnprintf(buf, 128, "%g", ((SFVec3d *) m_value)->getValue(offset));
    str = buf;
}

FieldValue *
SFVec3dItem::StopEditing(const char *str, int offset)
{
    SFVec3d *newValue = new SFVec3d(((SFVec3d *) m_value)->getValue());
    newValue->setValue(offset, (double) atof(str));
    return newValue;
}
