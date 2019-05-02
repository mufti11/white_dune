/*
 * MFRotationItem.cpp
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

#include "MFRotationItem.h"
#include "MFRotation.h"
#include "SFRotationItem.h"
#include "SFRotation.h"
#include "FieldView.h"
#include "swt.h"

MFRotationItem::MFRotationItem(FieldView *view) : MFFloatItem(view)
{
}

FieldValue *
MFRotationItem::OnMouseMove(FieldValue *value, int index, int delta)
{
    const float *vec = ((MFRotation *)value)->getValues(); 
    float f[4] = { vec[0], vec[1], vec[2], vec[3] };
    f[index] += delta * 0.02f;
    ((MFRotation *)value)->setSFValue(0, f); 
    return value; 
}

FieldViewItem *
MFRotationItem::CreateSFItem()
{
    return new SFRotationItem(m_view);
}

void
MFRotationItem::StartEditing(MyString &str, int offset)
{
    MFRotation *v = (MFRotation *) m_value;
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
MFRotationItem::StopEditing(const char *str, int offset)
{
    float f = atof(str);
    if (IsCollapsed()) {
        const float *oldFloats = ((MFRotation *)m_value)->getValues();
        float newFloats[4] = { oldFloats[0], oldFloats[1], oldFloats[2], oldFloats[3] };
        newFloats[offset] = f;
        return (new MFRotation(newFloats, 4))->copy();
    } else {
        SFRotation *newValue = new SFRotation(((MFRotation *) m_value)->getValues());
        newValue->setValue(offset, f);
        ((MFRotation *)m_value)->setSFValue(0, newValue);
        InitIndexValue(offset, m_value);
        return m_value;
    }
}

void
MFRotationItem::InsertSFValue(int index)
{
    ((MFRotation *)m_value)->insertSFValue(index, (FieldValue *)new SFRotation());
}

void                
MFRotationItem::RemoveSFValue(int index) 
{
    ((MFRotation *)m_value)->removeSFValue(index);
}

FieldValue *
MFRotationItem::OnMouseDown(int x, int y, int modifiers)
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
MFRotationItem::GetFieldOffset(int xpos) const
{
    int offset = xpos / m_view->GetFloatWidth();
    if (offset > 3) offset = 3;
    return offset;
}



