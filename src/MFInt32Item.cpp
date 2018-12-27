/*
 * MFInt32Item.cpp
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

#include "MFInt32Item.h"
#include "MFInt32.h"
#include "SFInt32Item.h"
#include "SFInt32.h"
#include "FieldView.h"
#include "swt.h"

MFInt32Item::MFInt32Item(FieldView *view) : MFieldViewItem(view)
{
    m_hexadezimal = false;
}

FieldValue *
MFInt32Item::OnMouseMove(FieldValue *value, int /* index */, int delta)
{
    return new MFInt32(((MFInt32 *)value)->getValue(0) + delta / 10);
}

FieldViewItem *
MFInt32Item::CreateSFItem()
{
    return new SFInt32Item(m_view);
}

//
// when an item is collapsed, draw the 1st element
//

void
MFInt32Item::Draw(SDC dc, int x, int y)
{
    if (IsCollapsed()) {
        if (((MFInt32 *) m_value)->getSize() == 1) {
            m_children[0]->Draw(dc, x, y);
        }
    }
}

//
// allow the 1st element to be edited, when the item is collapsed
//

bool
MFInt32Item::IsEditable() const
{
    return !IsCollapsed() || (((MFInt32 *)m_value)->getSize() < 2);
}

bool
MFInt32Item::IsTrackable() const
{
    return !IsCollapsed() || (((MFInt32 *)m_value)->getSize() == 1);
}

void
MFInt32Item::StartEditing(MyString &str, int offset)
{
    MFInt32 *v = (MFInt32 *) m_value;
    char buf[128];

    if (!IsCollapsed() || (v->getSize() == 0)) {
        InsertSFValue(0);
        m_children.insert(new FieldViewItem(m_view), 0); 
    } else
        str = "";
    InitIndexValue(0, m_value);
    mysnprintf(buf, 128, getMFormat(), (int) (v->getValue(0)));
    str = buf;
}

FieldValue *
MFInt32Item::StopEditing(const char *str, int offset)
{
    m_hexadezimal = false;
    if (strlen(str) > 2)
        if ((str[0] == '0') && ((str[1] == 'X') || (str[1] == 'x')))
            m_hexadezimal = true;
    int i = 0;
    sscanf(str, getMFormat(), &i);
    ((MFInt32 *)m_value)->setSFValue(offset, i);
    InitIndexValue(offset, m_value);
    return m_value;
}

void
MFInt32Item::InsertSFValue(int index)
{
    ((MFInt32 *)m_value)->insertSFValue(index, 0);
}

void                
MFInt32Item::RemoveSFValue(int index) 
{
    ((MFInt32 *)m_value)->removeSFValue(index);
}

FieldValue *
MFInt32Item::OnMouseDown(int x, int y, int modifiers)
{
    int width = m_view->GetItemWidth();
    int buttonSize = m_view->GetItemHeight() - 1;

    if (x >= width - buttonSize && x < width) {
        if (m_field->getStrings()) {
            // create popup
        }
    }
    return NULL;
}

const char *MFInt32Item::getMFormat(void) 
{
    return m_hexadezimal ? "%x" : "%d";        
}

