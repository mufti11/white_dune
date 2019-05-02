/*
 * SFRotationItem.cpp
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

#include "SFRotationItem.h"
#include "SFRotation.h"
#include "FieldView.h"
#include "DuneApp.h"

SFRotationItem::SFRotationItem(FieldView *view)
  : FieldViewItem(view)
{
}

void
SFRotationItem::Draw(SDC dc, int x, int y)
{
    const char *title = TheApp->GetRotationTitle();

#ifdef HAVE_EULER_ROT
    int rotationOrder = TheApp->GetRotationOrder();
    Vec3f v = ((SFRotation *) m_value)->getEulerAngles(rotationOrder);
    float f[3] = {v.x, v.y, v.z};
    DrawFloats(f, 3, title, dc, x, y + m_view->GetItemHeight() - 3,
               m_view->GetFloatWidth());
#else
   DrawFloats(((SFRotation *) m_value)->getValue(), 4, title, dc, 
               x, y + m_view->GetItemHeight() - 3, m_view->GetFloatWidth());
#endif
}

FieldValue *
SFRotationItem::OnMouseMove(FieldValue *value, int index, int delta)
{
    SFRotation *oldValue = (SFRotation *) value;
    SFRotation *newValue = new SFRotation(*oldValue);
    if (index<3) {
        int rotationOrder = TheApp->GetRotationOrder();
        Vec3f v;
        float v2[3];

        v = oldValue->getEulerAngles(rotationOrder);
        v2[0] = v.x; v2[1] = v.y; v2[2] = v.z;
        v2[index] += delta;
        newValue->setEulerAngles(v2, rotationOrder);
    }
    return newValue;
}

int
SFRotationItem::GetFieldOffset(int xpos) const
{
    int offset = xpos / m_view->GetFloatWidth();
#ifdef HAVE_EULER_ROT
    if (offset > 2) offset = 2;
#else 
    if (offset > 3) offset = 3;
#endif
    return offset;
}

void
SFRotationItem::StartEditing(MyString &str, int offset)
{
    char buf[128];
#ifdef HAVE_EULER_ROT
    int order = TheApp->GetRotationOrder();
    Vec3f v = ((SFRotation *) m_value)->getEulerAngles(order);
    mysnprintf(buf, 128, "%g", ((const float *) &v)[offset]);
#else
    float v[4];
    for (int i=0;i<4;i++)
       v[i]=((SFRotation *) m_value)->getValue()[i];
    mysnprintf(buf, 128, "%g", ((const float *) v)[offset]);
#endif
    str = buf;
}

FieldValue *
SFRotationItem::StopEditing(const char *str, int offset)
{
#ifdef HAVE_EULER_ROT
    int order = TheApp->GetRotationOrder();
    Vec3f v;
    float v2[3];
    v = ((SFRotation *) m_value)->getEulerAngles(order);
    v2[0] = v.x; v2[1] = v.y; v2[2] = v.z;
    v2[offset] = (float) atof(str);
    return new SFRotation(Vec3f(v2), order);
#else
    float v2[4];
    for (int i=0;i<4;i++)
       v2[i]=((SFRotation *) m_value)->getValue()[i];
    v2[offset] = (float) atof(str);
    return new SFRotation(v2);
#endif
}
