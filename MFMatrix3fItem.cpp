/*
 * MFMatrix3fItem.cpp
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

#include "MFMatrix3fItem.h"
#include "MFMatrix3f.h"
#include "SFMatrix3fItem.h"
#include "SFMatrix3f.h"
#include "FieldView.h"
#include "swt.h"

MFMatrix3fItem::MFMatrix3fItem(FieldView *view) : MFFloatItem(view)
{
}

FieldValue *
MFMatrix3fItem::OnMouseMove(FieldValue *value, int index, int delta)
{
    const float *matrix = ((MFMatrix3f *)value)->getValues(); 
    float f[9];
    for (int i = 0; i < 9; i++)
        f[i] = matrix[i];
    f[index] += delta * 0.02f;
    ((MFMatrix3f *)value)->setSFValue(0, f); 
    return value; 
}

FieldViewItem *
MFMatrix3fItem::CreateSFItem()
{
    return new SFMatrix3fItem(m_view);
}

void
MFMatrix3fItem::StartEditing(MyString &str, int offset)
{
    MFMatrix3f *v = (MFMatrix3f *) m_value;
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
MFMatrix3fItem::StopEditing(const char *str, int offset)
{
    float f = atof(str);
    if (IsCollapsed()) {
        const float *oldFloats = ((MFMatrix3f *)m_value)->getValues();
        float newFloats[9];
        for (int i = 0; i < 9; i++)
            newFloats[i] = oldFloats[i];
        newFloats[offset] = f;
        return (new MFMatrix3f(newFloats, 9))->copy();
    } else {
        SFMatrix3f *newValue = new SFMatrix3f(((MFMatrix3f *) m_value)->getValues());
        newValue->setValue(offset, f);
        ((MFMatrix3f *)m_value)->setSFValue(0, newValue);
        InitIndexValue(offset, m_value);
        return m_value;
    }
}

void
MFMatrix3fItem::InsertSFValue(int index)
{
    ((MFMatrix3f *)m_value)->insertSFValue(index, (FieldValue *)new SFMatrix3f());
}

void                
MFMatrix3fItem::RemoveSFValue(int index) 
{
    ((MFMatrix3f *)m_value)->removeSFValue(index);
}

FieldValue *
MFMatrix3fItem::OnMouseDown(int x, int y, int modifiers)
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
MFMatrix3fItem::GetFieldOffset(int xpos) const
{
    int offset = xpos / m_view->GetFloatWidth();
    if (offset > 2) offset = 2;
    return offset;
}



