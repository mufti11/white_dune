/*
 * SFTimeItem.cpp
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

#include "SFTimeItem.h"
#include "SFTime.h"
#include "FieldView.h"
#include "swt.h"

SFTimeItem::SFTimeItem(FieldView *view) : FieldViewItem(view)
{
}

void
SFTimeItem::Draw(SDC dc, int x, int y)
{
    char buf[128];

    mysnprintf(buf, 128, "%8.2f", ((SFTime *) m_value)->getValue());
    swDrawText(dc, x, y + m_view->GetItemHeight() - 3, buf);
}

FieldValue *
SFTimeItem::OnMouseMove(FieldValue *value, int index, int delta)
{
    return new SFTime(((SFTime *) value)->getValue() + delta * 0.01);
}

void
SFTimeItem::StartEditing(MyString &str, int /* offset */)
{
    char buf[128];
    
    mysnprintf(buf, 128, "%g", ((SFTime *) m_value)->getValue());
    str = buf;
}

FieldValue *
SFTimeItem::StopEditing(const char *str, int /* offset */)
{
    return new SFTime(atof(str));
}
