/*
 * MFDoubleItem.cpp
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

#include "MFDoubleItem.h"
#include "MFDouble.h"
#include "SFDoubleItem.h"
#include "FieldView.h"
#include "swt.h"

MFDoubleItem::MFDoubleItem(FieldView *view) : MFieldViewItem(view)
{
}

FieldValue *
MFDoubleItem::OnMouseMove(FieldValue *value, int /* index */, int delta)
{
    return new MFDouble(((MFDouble *)value)->getValue(0) + delta * 0.02f);
}

FieldViewItem *
MFDoubleItem::CreateSFItem()
{
    return new SFDoubleItem(m_view);
}

//
// when an item is collapsed, draw the 1st element
//

void
MFDoubleItem::Draw(SDC dc, int x, int y)
{
    if (IsCollapsed()) {
        if (((MFieldValue *) m_value)->getSFSize() == 1) {
            m_children[0]->Draw(dc, x, y);
        }
    }
}

//
// allow the 1st element to be edited, when the item is collapsed
//

bool
MFDoubleItem::IsEditable() const
{
    return !IsCollapsed() || (((MFieldValue *)m_value)->getSFSize() < 2);
}

bool
MFDoubleItem::IsTrackable() const
{
    return !IsCollapsed() || (((MFieldValue *)m_value)->getSFSize() == 1);
}

void
MFDoubleItem::StartEditing(MyString &str, int offset)
{
    MFDouble *v = (MFDouble *) m_value;
    char buf[128];

    if (!IsCollapsed() || (v->getSFSize() == 0)) {
        InsertSFValue(0);
        m_children.insert(new FieldViewItem(m_view), 0); 
    } else
        str = "";
    InitIndexValue(0, m_value);
    mysnprintf(buf, 128, "%g", (v->getValue(0)));
    str = buf;
}

FieldValue *
MFDoubleItem::StopEditing(const char *str, int offset)
{
    double f = atof(str);
    ((MFDouble *)m_value)->setSFValue(0, f);
    InitIndexValue(0, m_value);
    return m_value;
}

void
MFDoubleItem::InsertSFValue(int index)
{
    ((MFDouble *)m_value)->insertSFValue(index, 0.0);
}

void                
MFDoubleItem::RemoveSFValue(int index) 
{
    ((MFDouble *)m_value)->removeSFValue(index);
}

FieldValue *
MFDoubleItem::OnMouseDown(int x, int y, int modifiers)
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

