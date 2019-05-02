/*
 * MFColorItem.cpp
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

#include "MFColorItem.h"
#include "MFColor.h"
#include "SFColorItem.h"
#include "SFColor.h"
#include "FieldView.h"
#include "swt.h"

MFColorItem::MFColorItem(FieldView *view) : MFFloatItem(view)
{
}

FieldValue *
MFColorItem::OnMouseMove(FieldValue *value, int index, int delta)
{
    const float *vec = ((MFColor *)value)->getValues(); 
    float f[3] = { vec[0], vec[1], vec[2]};
    f[index] += delta * 0.02f;
    ((MFColor *)value)->setSFValue(0, f); 
    return value; 
}

FieldViewItem *
MFColorItem::CreateSFItem()
{
    return new SFColorItem(m_view);
}

void
MFColorItem::StartEditing(MyString &str, int offset)
{
    MFColor *v = (MFColor *) m_value;
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
MFColorItem::StopEditing(const char *str, int offset)
{
    float f = atof(str);
    if (IsCollapsed()) {
        const float *oldFloats = ((MFColor *)m_value)->getValues();
        float newFloats[3] = { oldFloats[0], oldFloats[1], oldFloats[2] };
        newFloats[offset] = f;
        return (new MFColor(newFloats, 3))->copy();
    } else {
        SFColor *newValue = new SFColor(((MFColor *) m_value)->getValues());
        newValue->setValue(offset, f);
        ((MFColor *)m_value)->setSFValue(0, newValue);
        InitIndexValue(offset, m_value);
        return m_value;
    }
}

void
MFColorItem::InsertSFValue(int index)
{
    ((MFColor *)m_value)->insertSFValue(index, (FieldValue *)new SFColor());
}

void                
MFColorItem::RemoveSFValue(int index) 
{
    ((MFColor *)m_value)->removeSFValue(index);
}

FieldValue *
MFColorItem::OnMouseDown(int x, int y, int modifiers)
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
MFColorItem::GetFieldOffset(int xpos) const
{
    int offset = xpos / m_view->GetFloatWidth();
    if (offset > 2) offset = 2;
    return offset;
}




