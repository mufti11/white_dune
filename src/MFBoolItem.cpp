/*
 * MFBoolItem.cpp
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

#include "MFBoolItem.h"
#include "MFBool.h"
#include "SFBoolItem.h"
#include "SFBool.h"
#include "FieldView.h"
#include "swt.h"

MFBoolItem::MFBoolItem(FieldView *view) : MFieldViewItem(view)
{
}

FieldValue *
MFBoolItem::OnMouseMove(FieldValue *value, int index, int delta)
{
    bool val;
    
    if (delta > 0)
        val = true;
    else
        val = false;
    return new MFBool(val);
}

FieldViewItem *
MFBoolItem::CreateSFItem()
{
    return new SFBoolItem(m_view);
}

//
// when an item is collapsed, draw the 1st element
//

void
MFBoolItem::Draw(SDC dc, int x, int y)
{
    if (IsCollapsed()) {
        if (((MFBool *) m_value)->getSize() == 1) {
            m_children[0]->Draw(dc, x, y);
        }
    }
}

//
// allow the 1st element to be edited, when the item is collapsed
//

bool
MFBoolItem::IsEditable() const
{
    return !IsCollapsed() || (((MFBool *)m_value)->getSize() < 2);
}

bool
MFBoolItem::IsTrackable() const
{
    return !IsCollapsed() || (((MFBool *)m_value)->getSize() == 1);
}


void
MFBoolItem::StartEditing(MyString &str, int offset)
{
    MFBool *v = (MFBool *) m_value;
    char buf[128];

    if (!IsCollapsed() || (v->getSize() == 0)) {
        InsertSFValue(0);
        m_children.insert(new FieldViewItem(m_view), 0); 
    } else
        str = "";
    InitIndexValue(0, m_value);
    if (v->getValue(0))
        strcpy(buf, "TRUE");
    else
       strcpy(buf, "FALSE");
    str = buf;
}

FieldValue *
MFBoolItem::StopEditing(const char *str, int offset)
{
    int i = 0;
    ((MFBool *)m_value)->setSFValue(offset, i);
    InitIndexValue(offset, m_value);
    return m_value;
}

void
MFBoolItem::InsertSFValue(int index)
{
    ((MFBool *)m_value)->insertSFValue(index, false);
}

void                
MFBoolItem::RemoveSFValue(int index) 
{
    ((MFBool *)m_value)->removeSFValue(index);
}

FieldValue *
MFBoolItem::OnMouseDown(int x, int y, int modifiers)
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

