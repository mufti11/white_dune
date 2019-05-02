/*
 * MFStringItem.cpp
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

#include "MFStringItem.h"
#include "MFString.h"
#include "SFStringItem.h"
#include "SFString.h"
#include "FieldView.h"
#include "swt.h"

MFStringItem::MFStringItem(FieldView *view) : MFieldViewItem(view)
{
}

FieldViewItem *
MFStringItem::CreateSFItem()
{
    return new SFStringItem(m_view);
}

//
// when an item is collapsed, draw the 1st element
//

void
MFStringItem::Draw(SDC dc, int x, int y)
{
    int size = m_view->GetItemHeight() - 1;
    if (IsCollapsed()) {
        if (((MFString *) m_value)->getSize()) {
            m_children[0]->Draw(dc, x, y);
        } else if (m_field->getFlags() & FF_URL) {
            int bx = x + m_view->GetItemWidth() - size;
            swDraw3DRect(dc, m_view->GetWindow(), bx, y, size, size);
            swSetFGColor(dc, swGetWindowColor(m_view->GetWindow(), SW_COLOR_TEXT));
            swDrawText(dc, bx + 2, y + m_view->GetItemHeight() - 3, "...");
        }
    }
}

//
// allow the 1st element to be edited, when the item is collapsed
//

bool
MFStringItem::IsEditable() const
{
    return !IsCollapsed() || (((MFString *)m_value)->getSize() < 2);
}

void
MFStringItem::StartEditing(MyString &str, int offset)
{
    MFString *v = (MFString *) m_value;

    if (!IsCollapsed() || (v->getSize() == 0)) {
        InsertSFValue(0);
        m_children.insert(new FieldViewItem(m_view), 0); 
    } else
        str = "";
    InitIndexValue(0, m_value);
    str = v->getValue(0);
}

FieldValue *
MFStringItem::StopEditing(const char *str, int offset)
{
    ((MFString *)m_value)->setSFValue(offset, str);
    InitIndexValue(offset, m_value);
    return m_value;
}

void
MFStringItem::InsertSFValue(int index)
{
    ((MFString *)m_value)->insertSFValue(index, "");
}

void                
MFStringItem::RemoveSFValue(int index) 
{
    ((MFString *)m_value)->removeSFValue(index);
}

FieldValue *
MFStringItem::OnMouseDown(int x, int y, int modifiers)
{
    int width = m_view->GetItemWidth();
    int buttonSize = m_view->GetItemHeight() - 1;

    if (x >= width - buttonSize && x < width) {
        if (m_field->getFlags() & FF_URL) {
/*
            CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY, 
                            "Image Files (*.gif, *.jpg)|*.gif;*.jpg|All Files (*.*)|*.*||",
                            m_view);

            if (dlg.DoModal() == IDOK) {
                URL parent, child;
                const char *url = m_view->GetDocument()->getScene()->getURL();

                if (url[0]) {
                    parent = url;
                } else {
                    char cdir[_MAX_DIR];
                    GetCurrentDirectory(_MAX_DIR, cdir);
                    parent.FromDOSPath(cdir);
                }
                child.FromDOSPath(dlg.GetPathName());
                return new MFString(child.RelativeTo(parent));
            }
*/
        } else if (m_field->getStrings()) {
            // create popup
        }
    }
    return NULL;
}
