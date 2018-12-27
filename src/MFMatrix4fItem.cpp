/*
 * MFMatrix4fItem.cpp
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

#include "MFMatrix4fItem.h"
#include "MFMatrix4f.h"
#include "SFMatrix4fItem.h"
#include "SFMatrix4f.h"
#include "FieldView.h"
#include "swt.h"

MFMatrix4fItem::MFMatrix4fItem(FieldView *view) : MFFloatItem(view)
{
}

FieldValue *
MFMatrix4fItem::OnMouseMove(FieldValue *value, int index, int delta)
{
    const float *matrix = ((MFMatrix4f *)value)->getValues(); 
    float f[16];
    for (int i = 0; i < 16; i++)
        f[i] = matrix[i];
    f[index] += delta * 0.02f;
    ((MFMatrix4f *)value)->setSFValue(0, f); 
    return value; 
}

FieldViewItem *
MFMatrix4fItem::CreateSFItem()
{
    return new SFMatrix4fItem(m_view);
}

void
MFMatrix4fItem::StartEditing(MyString &str, int offset)
{
    MFMatrix4f *v = (MFMatrix4f *) m_value;
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
MFMatrix4fItem::StopEditing(const char *str, int offset)
{
    float f = atof(str);
    if (IsCollapsed()) {
        const float *oldFloats = ((MFMatrix4f *)m_value)->getValues();
        float newFloats[16];
        for (int i = 0; i < 16; i++)
            newFloats[i] = oldFloats[i];
        newFloats[offset] = f;
        return (new MFMatrix4f(newFloats, 16))->copy();
    } else {
        SFMatrix4f *newValue = new SFMatrix4f(((MFMatrix4f *) m_value)->getValues());
        newValue->setValue(offset, f);
        ((MFMatrix4f *)m_value)->setSFValue(0, newValue);
        InitIndexValue(offset, m_value);
        return m_value;
    }
}

void
MFMatrix4fItem::InsertSFValue(int index)
{
    ((MFMatrix4f *)m_value)->insertSFValue(index, (FieldValue *)new SFMatrix4f());
}

void                
MFMatrix4fItem::RemoveSFValue(int index) 
{
    ((MFMatrix4f *)m_value)->removeSFValue(index);
}

FieldValue *
MFMatrix4fItem::OnMouseDown(int x, int y, int modifiers)
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
MFMatrix4fItem::GetFieldOffset(int xpos) const
{
    int offset = xpos / m_view->GetFloatWidth();
    if (offset > 2) offset = 2;
    return offset;
}



