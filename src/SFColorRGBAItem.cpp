/*
 * SFColorRGBAItem.cpp
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

#include "SFColorRGBAItem.h"
#include "SFColorRGBA.h"
#include "FieldView.h"
#include "swt.h"

SFColorRGBAItem::SFColorRGBAItem(FieldView *view) : FieldViewItem(view)
{
}

FieldValue *
SFColorRGBAItem::OnMouseDown(int x, int y, int modifiers)
{
    if (x < m_view->GetItemHeight() - 1) {
//    CColorDialog dlg;
//      const float *c = ((SFColorRGBA *) m_value)->getValue();
//      COLORREF rgb = RGB(
//          (BYTE) (c[0] * 255.0f), 
//          (BYTE) (c[1] * 255.0f), 
//          (BYTE) (c[2] * 255.0f));

//      dlg.m_cc.rgbResult = rgb;
//      dlg.m_cc.Flags |= CC_RGBINIT | CC_FULLOPEN;
//      if (dlg.DoModal() == IDOK) {
//          COLORREF color = dlg.GetColor();
//          return new SFColorRGBA(GetRValue(color) / 255.0f, GetGValue(color) / 255.0f, GetBValue(color) / 255.0f);
//       } else {
            return NULL;
//       }
    } else {
        return NULL;
    }
}

SFColorRGBAItem::~SFColorRGBAItem()
{
}

void
SFColorRGBAItem::Draw(SDC dc, int x, int y)
{
    const float *c = ((SFColorRGBA *) m_value)->getValue();
    int size = m_view->GetItemHeight() - 1;

    DrawFloats(((SFColorRGBA *) m_value)->getValue(), 4, "RGBA", dc,
               x+size+2, y+size-2, m_view->GetFloatWidth());
    swDraw3DRect(dc, m_view->GetWindow(), x, y, size, size);
    swDraw3DRect(dc, m_view->GetWindow(), x+1, y+1, size-2, size-2);
    swSetFGColor(dc, SW_RGB((int) (c[0] * 255.0f),
                            (int) (c[1] * 255.0f),
                            (int) (c[2] * 255.0f)));
    swFillRect(dc, x+2, y+2, size-4, size-4);
}

void
SFColorRGBAItem::UpdateControl()
{
}

FieldValue *
SFColorRGBAItem::OnMouseMove(FieldValue *value, int index, int delta) 
{
    SFColorRGBA *newValue = new SFColorRGBA(((SFColorRGBA *) value)->getValue());
    float f = ((SFColorRGBA *) value)->getValue()[index] + delta * 0.005f;
    f = CLAMP(f, 0.0f, 1.0f);
    newValue->setValue(index, f);
    return newValue;
}

int
SFColorRGBAItem::GetFieldOffset(int xpos) const
{
    int offset = (xpos - m_view->GetItemHeight() - 1) / m_view->GetFloatWidth();
    if (offset > 3) offset = 3;
    return offset;
}

void
SFColorRGBAItem::StartEditing(MyString &buf, int offset)
{
    assert( offset >= 0 && offset < 4);

    char tmp[20];
    mysnprintf(tmp, 128, "%g", ((SFColorRGBA *) m_value)->getValue()[offset]);
    buf = tmp;
}

FieldValue *
SFColorRGBAItem::StopEditing(const char *str, int offset)
{
    SFColorRGBA *newValue = new SFColorRGBA(((SFColorRGBA *) m_value)->getValue());
    newValue->setValue(offset, (float) atof(str));
    return newValue;
}
