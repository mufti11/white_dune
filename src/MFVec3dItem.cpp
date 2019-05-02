/*
 * MFVec3dItem.cpp
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

#include "MFVec3dItem.h"
#include "MFVec3d.h"
#include "SFVec3dItem.h"
#include "SFVec3d.h"
#include "FieldView.h"
#include "swt.h"

FieldValue *
MFVec3dItem::OnMouseMove(FieldValue *value, int index, int delta)
{
    const double *vec = ((MFVec3d *)value)->getValues(); 
    double f[3] = { vec[0], vec[1], vec[2]};
    f[index] += delta * 0.02f;
    ((MFVec3d *)value)->setSFValue(0, f); 
    return value; 
}

FieldViewItem *
MFVec3dItem::CreateSFItem()
{
    return new SFVec3dItem(m_view);
}

void
MFVec3dItem::StartEditing(MyString &str, int offset)
{
    MFVec3d *v = (MFVec3d *) m_value;
    char buf[128];

    if (!IsCollapsed() || (v->getSFSize() == 0)) {
        InsertSFValue(0);
        m_children.insert(new FieldViewItem(m_view), 0); 
    } else
        str = "";
    InitIndexValue(0, m_value);
    mysnprintf(buf, 128, "%g", (double) (v->getValue(0))[offset]);
    str = buf;
}

FieldValue *
MFVec3dItem::StopEditing(const char *str, int offset)
{
    double f = atof(str);
    if (IsCollapsed()) {
        const double *oldFloats = ((MFVec3d *)m_value)->getValues();
        double newFloats[3] = { oldFloats[0], oldFloats[1], oldFloats[2] };
        newFloats[offset] = f;
        return (new MFVec3d(newFloats, 3))->copy();
    } else {
        SFVec3d *newValue = new SFVec3d(((MFVec3d *) m_value)->getValues());
        newValue->setValue(offset, f);
        ((MFVec3d *)m_value)->setSFValue(0, newValue);
        InitIndexValue(offset, m_value);
        return m_value;
    }
}

void
MFVec3dItem::InsertSFValue(int index)
{
    ((MFVec3d *)m_value)->insertSFValue(index, (FieldValue *)new SFVec3d());
}

void                
MFVec3dItem::RemoveSFValue(int index) 
{
    ((MFVec3d *)m_value)->removeSFValue(index);
}

FieldValue *
MFVec3dItem::OnMouseDown(int x, int y, int modifiers)
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
MFVec3dItem::GetFieldOffset(int xpos) const
{
    int offset = xpos / m_view->GetFloatWidth();
    if (offset > 2) offset = 2;
    return offset;
}



