/*
 * SFBoolItem.cpp
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

#include "SFBoolItem.h"

#include "FieldView.h"
#include "SFBool.h"
#include "Path.h"
#include "FieldCommand.h"

SFBoolItem::SFBoolItem(FieldView *view) : FieldViewItem(view)
{
    m_button = NULL;
}

SFBoolItem::~SFBoolItem()
{
    if (m_button) swDestroyWindow(m_button);
}

static void
boolButtonCallback(void *data, void *id)
{
    ((SFBoolItem *) data)->OnClicked();
}

void
SFBoolItem::Draw(SDC dc, int x, int y)
{
    char buf[128];

    if (((SFBool *) m_value)->getValue())
        strcpy(buf, "     TRUE");
    else
        strcpy(buf, "     FALSE");
    swDrawText(dc, x, y + m_view->GetItemHeight() - 3, buf);
}

void
SFBoolItem::CreateControl(const Rect &rect, SWND wnd)
{
    m_button = swCreateCheckBox("", rect.left, rect.top,
                               rect.Height()-1, rect.Height()-1, wnd);
    swSetCommandCallback(m_button, boolButtonCallback);
    swSetClientData(m_button, this);
}

void
SFBoolItem::CreateControl(const Rect &rect)
{
    CreateControl(rect, m_view->GetWindow());
}


void
SFBoolItem::MoveControl(int x, int y)
{
    if (m_button) swSetPosition(m_button, x, y);
}

void
SFBoolItem::UpdateControl()
{
    if (m_button) swSetCheck(m_button, ((SFBool *) m_value)->getValue());
}

void
SFBoolItem::OnClicked()
{
    int check = swGetCheck(m_button);
    Node *node = m_view->GetScene()->getSelection()->getNode();
    FieldValue *value = new SFBool(check != 0);
    m_view->GetScene()->execute(new FieldCommand(node, m_index, value));
}
