/*
 * SFNodeItem.cpp
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

#include "SFNodeItem.h"
#include "SFNode.h"
#include "FieldView.h"
#include "swt.h"

SFNodeItem::SFNodeItem(FieldView *view)
  : FieldViewItem(view)
{
}

void
SFNodeItem::Draw(SDC dc, int x, int y)
{
    const char *str;

    if (m_value->getType() != SFNODE)
        return;

    Node *node = ((SFNode *) m_value)->getValue();
    if (node) {
        const char *name = node->getName();
        if (name && name[0]) {
            str = name;
        } else {
            bool x3d = node->getScene()->isX3d();
            str = (const char *) node->getProto()->getName(x3d);
        }
        swSetFont(dc, m_view->GetUFont());
        swSetFGColor(dc, 0xff0000);
        swDrawText(dc, x, y+m_view->GetItemHeight() - 3, str);
        swSetFont(dc, swGetDefaultFont());
    }
}

bool
SFNodeItem::isZero(void)
{
    if (m_value == NULL)
        return true;
    return false;
}

