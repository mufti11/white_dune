/*
 * SFStringItem.cpp
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

#include "SFStringItem.h"
#include "SFString.h"
#include "FieldView.h"
#include "swt.h"

SFStringItem::SFStringItem(FieldView *view) : FieldViewItem(view)
{
    m_comboBox = NULL;
}

SFStringItem::~SFStringItem()
{
    if (m_comboBox != NULL) swDestroyWindow(m_comboBox);
}

void
SFStringItem::Draw(SDC dc, int x, int y)
{
    int size = m_view->GetItemHeight() - 1;
    const char *str = ((SFString *) m_value)->getValue();
    swDrawText(dc, x, y + m_view->GetItemHeight() - 3, str);
    if (m_field->getFlags() & FF_URL) {
        int bx = x + m_view->GetItemWidth() - size;
        swDraw3DRect(dc, m_view->GetWindow(), bx, y, size, size);
        swSetFGColor(dc, swGetWindowColor(m_view->GetWindow(), SW_COLOR_TEXT));
        swDrawText(dc, bx + 2, y + m_view->GetItemHeight() - 3, "...");
    } else if (m_field->getStrings()) {
/*
        int x = m_view->GetItemWidth() - size;
        dc->DrawFrameControl(CRect(x, 0, x + size, size), 
                             DFC_BUTTON, DFCS_BUTTONPUSH);
        CBitmap bmp;
        SDC bdc;
        bdc.CreateCompatibleDC(dc);
        bmp.LoadBitmap(OBM_COMBO);
        CBitmap *old = (CBitmap *) bdc.SelectObject(bmp);
        dc->BitBlt(x + 2, 2, 12, 12, &bdc, 0, 0, SRCCOPY);
        bdc.SelectObject(old);
*/
    }
}

FieldValue *
SFStringItem::OnMouseDown(int x, int y, int modifiers)
{
    int height = m_view->GetItemHeight();
    int width = m_view->GetItemWidth();
    const char **strings = m_field->getStrings();

    if (x >= width - height && x < width) {
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
                return new SFString(child.RelativeTo(parent));
            }
*/
        } else if (strings) {
/*
            // create popup
            const char **s;
            for (s = strings; *s != NULL; s++)
                ;
            int n = s - strings;
            CPoint p(m_view->GetColumnWidth(0), n * m_view->GetItemHeight());
            m_view->ClientToScreen(&p);
            CWnd popup;
            popup.CreateEx(0, "FunkyComboBoxClass", "", WS_POPUP | WS_BORDER | WS_VISIBLE,
                           CRect(p.x, p.y, m_view->GetColumnWidth(1),
                           n * m_view->GetItemHeight()), NULL, 0, NULL);
            return NULL;
*/
        }
    }
    return NULL;
}

void
SFStringItem::MoveControl(int x, int y)
{
    if (m_comboBox) swSetPosition(m_comboBox, x, y);
}

void
SFStringItem::CreateControl(const Rect &rect)
{
    const char **strings = m_field->getStrings();

    if (strings) {
/*
        // create a combobox
        int style;
        m_comboBox = swCreateComboBox();

        const char **s;
        HFONT font = (HFONT) GetStockObject(DEFAULT_GUI_FONT);

        int bsize = rect.Height();
        if (m_field->getFlags() & FF_FIXEDSTRINGS) {
            style = CBS_DROPDOWNLIST;
        } else {
            style = CBS_DROPDOWN;
        }
        m_comboBox->Create(style | WS_CHILD | WS_VISIBLE, rect, m_view, id);
        m_comboBox->SetFont(CFont::FromHandle(font));
        int h = m_comboBox->GetItemHeight(0);
        CRect crect(rect), w;

        m_comboBox->GetWindowRect(&w);
        crect.bottom += 6;

        for (s = strings; *s != NULL; s++) {
            m_comboBox->AddString(*s);
            crect.bottom += h;
        }
        m_comboBox->MoveControl(crect);
*/
    }
}

void
SFStringItem::CreateControl(const Rect &rect, SWND wnd) 
{
    CreateControl(rect);
}


void
SFStringItem::UpdateControl()
{
    if (m_comboBox) {
/*
        int index = swComboBoxFindString(m_comboBox, 0, ((SFString *) m_value)->getValue());
        if (index >= 0) swComboBoxSetCurSel(index);
*/
    }
}

void
SFStringItem::StartEditing(MyString &str, int /* offset */)
{
    str = ((SFString *) m_value)->getValue();
}

FieldValue *
SFStringItem::StopEditing(const char *str, int /* offset */)
{
    return new SFString((const char *) str);
}

FieldValue *
SFStringItem::OnCommand(int id)
{
//    if (m_comboBox && id == CBN_SELCHANGE) {
//       return new SFString(m_field->getStrings()[m_comboBox->GetCurSel()]);
//    } else {
      return NULL;
//    }
}
