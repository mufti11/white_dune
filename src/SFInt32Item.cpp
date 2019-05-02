/*
 * SFInt32Item.cpp
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

#include "SFInt32Item.h"
#include "SFInt32.h"
#include "FieldView.h"
#include "swt.h"

SFInt32Item::SFInt32Item(FieldView *view)
  : FieldViewItem(view)
{
    m_hexadezimal = false;
}

void
SFInt32Item::Draw(SDC dc, int x, int y)
{
    char buf[128];

    mysnprintf(buf, 128, getFormat(), ((SFInt32 *) m_value)->getValue());
    swDrawText(dc, x, y+m_view->GetItemHeight() - 3, buf);
}

FieldValue *
SFInt32Item::OnMouseMove(FieldValue *value, int /* index */, int delta)
{
    return new SFInt32(((SFInt32 *) value)->getValue() + delta / 10);
}

void
SFInt32Item::StartEditing(MyString &str, int /* offset */)
{
    char buf[128];

    mysnprintf(buf, 128, getFormat(), ((SFInt32 *) m_value)->getValue());
    str = buf;
}

FieldValue *
SFInt32Item::StopEditing(const char *str, int /* offset */)
{
    m_hexadezimal = false;
    if (strlen(str) > 2)
        if ((str[0] == '0') && ((str[1] == 'X') || (str[1] == 'x')))
            m_hexadezimal = true;
    int i = 0;
    sscanf(str, getFormat(), &i);
    return new SFInt32(i);
}

const char *SFInt32Item::getFormat(void) 
{
    return m_hexadezimal ? "%x" : "%d";        
}
