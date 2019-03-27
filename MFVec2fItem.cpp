/*
 * MFVec2fItem.cpp
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

#include "MFVec2fItem.h"
#include "MFVec2f.h"
#include "SFVec2fItem.h"
#include "SFVec2f.h"
#include "FieldView.h"
#include "swt.h"


MFVec2fItem::MFVec2fItem(FieldView *view) : MFFloatItem(view)
{
}

FieldValue *
MFVec2fItem::OnMouseMove(FieldValue *value, int index, int delta)
{
    const float *vec = ((MFVec2f *)value)->getValues(); 
    float f[2] = { vec[0], vec[1] };
    f[index] += delta * 0.02f;
    ((MFVec2f *)value)->setSFValue(0, f); 
    return value; 
}

FieldViewItem *
MFVec2fItem::CreateSFItem()
{
    return new SFVec2fItem(m_view);
}

void
MFVec2fItem::StartEditing(MyString &str, int offset)
{
    MFVec2f *v = (MFVec2f *) m_value;
    char buf[128];

    if (!IsCollapsed() || (v->getSFSize() == 0)) {
        InsertSFValue(0);
        m_children.insert(new FieldViewItem(m_view), 0); 
    } else
        str = "";
    InitIndexValue(0, m_value);
    mysnprintf(buf, 128, "%g", (float) (v->getValue(0))[offset]);
    str = buf;
}

FieldValue *
MFVec2fItem::StopEditing(const char *str, int offset)
{
    float f = atof(str);
    if (IsCollapsed()) {
        const float *oldFloats = ((MFVec2f *)m_value)->getValues();
        float newFloats[2] = { oldFloats[0], oldFloats[1] };
        newFloats[offset] = f;
        return (new MFVec2f(newFloats, 2))->copy();
    } else {
        SFVec2f *newValue = new SFVec2f(((MFVec2f *) m_value)->getValues());
        newValue->setValue(offset, f);
        ((MFVec2f *)m_value)->setSFValue(0, newValue);
        InitIndexValue(offset, m_value);
        return m_value;
    }
}

void
MFVec2fItem::InsertSFValue(int index)
{
    ((MFVec2f *)m_value)->insertSFValue(index, (FieldValue *)new SFVec2f());
}

void                
MFVec2fItem::RemoveSFValue(int index) 
{
    ((MFVec2f *)m_value)->removeSFValue(index);
}

FieldValue *
MFVec2fItem::OnMouseDown(int x, int y, int modifiers)
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

int
MFVec2fItem::GetFieldOffset(int xpos) const
{
    int offset = xpos / m_view->GetFloatWidth();
    if (offset > 1) offset = 1;
    return offset;
}


