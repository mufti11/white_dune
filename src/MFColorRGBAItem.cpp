/*
 * MFColorRGBAItem.cpp
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

#include "MFColorRGBAItem.h"
#include "MFColorRGBA.h"
#include "SFColorRGBAItem.h"
#include "SFColorRGBA.h"
#include "FieldView.h"
#include "swt.h"

MFColorRGBAItem::MFColorRGBAItem(FieldView *view) : MFFloatItem(view)
{
}

FieldValue *
MFColorRGBAItem::OnMouseMove(FieldValue *value, int index, int delta)
{
    const float *vec = ((MFColorRGBA *)value)->getValues(); 
    float f[4] = { vec[0], vec[1], vec[2], vec[3] };
    f[index] += delta * 0.02f;
    ((MFColorRGBA *)value)->setSFValue(0, f); 
    return value; 
}

FieldViewItem *
MFColorRGBAItem::CreateSFItem()
{
    return new SFColorRGBAItem(m_view);
}

void
MFColorRGBAItem::StartEditing(MyString &str, int offset)
{
    MFColorRGBA *v = (MFColorRGBA *) m_value;
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
MFColorRGBAItem::StopEditing(const char *str, int offset)
{
    float f = atof(str);
    if (IsCollapsed()) {
        const float *oldFloats = ((MFColorRGBA *)m_value)->getValues();
        float newFloats[4] = { oldFloats[0], 
                               oldFloats[1], 
                               oldFloats[2], 
                               oldFloats[3] 
                             };
        newFloats[offset] = f;
        return (new MFColorRGBA(newFloats, 4))->copy();
    } else {
        SFColorRGBA *newValue = new SFColorRGBA(((MFColorRGBA *) m_value)->getValues());
        newValue->setValue(offset, f);
        ((MFColorRGBA *)m_value)->setSFValue(0, newValue);
        InitIndexValue(offset, m_value);
        return m_value;
    }
}

void
MFColorRGBAItem::InsertSFValue(int index)
{
    ((MFColorRGBA *)m_value)->insertSFValue(index, (FieldValue *)new SFColorRGBA());
}

void                
MFColorRGBAItem::RemoveSFValue(int index) 
{
    ((MFColorRGBA *)m_value)->removeSFValue(index);
}

FieldValue *
MFColorRGBAItem::OnMouseDown(int x, int y, int modifiers)
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
MFColorRGBAItem::GetFieldOffset(int xpos) const
{
    int offset = xpos / m_view->GetFloatWidth();
    if (offset > 3) offset = 3;
    return offset;
}




