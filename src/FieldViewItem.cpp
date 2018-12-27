/*
 * FieldViewItem.cpp
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

#include <stdlib.h>
#include "stdafx.h"

#include "FieldViewItem.h"
#include "swt.h"

#include "EulerAngles.h"
#include "Field.h"
#include "FieldView.h"
#include "FieldValue.h"
#include "FieldCommand.h"
#include "Node.h"
#include "Path.h"
#include "Proto.h"
#include "SFMFTypes.h"
#include "SFMFTypesItem.h"
#include "Scene.h"
#include "URL.h"
#include "DuneApp.h"

FieldViewItem::FieldViewItem(FieldView *view)
{
    m_view = view;
    m_value = NULL;
    m_parent = NULL;
    m_state = FVIS_COLLAPSED;
    m_field = NULL;
    m_index = -1;
}

FieldViewItem::~FieldViewItem()
{
    if (m_value) m_value->unref();
}

void
FieldViewItem::SetValue(FieldValue *value)
{
    if (value == NULL) 
        return;
    if (!m_view->isFieldView())
        return;
    m_value = value;
    m_value->ref();
    if (m_view->getEnabled())
        UpdateControl();
}

FieldViewItem *
FieldViewItem::CreateItem(const Field *field, FieldView *view)
{
    FieldViewItem *r;

    CreateFieldViewItem(r, field->getType(), view);
    r->SetField(field);
    return r;
}

void 
FieldViewItem::DrawFloats(const float *floats, int len, const char *labels, 
                          SDC dc, int x, int y, int width)
{
    for (int i = 0; i < len; i++) {
        char buf[128];
        mysnprintf(buf, 128, "%c: %5.2f", labels[i], floats[i]);
        swDrawText(dc, x + 1.25 * i * width, y, buf);
    }
}

void 
FieldViewItem::DrawFloats(const double *floats, int len, const char *labels, 
                          SDC dc, int x, int y, int width)
{
    for (int i = 0; i < len; i++) {
        char buf[128];
        mysnprintf(buf, 128, "%c: %5.2lf", labels[i], floats[i]);
        swDrawText(dc, x + i * width, y, buf);
    }
}

MFieldViewItem::MFieldViewItem(FieldView *view) : FieldViewItem(view)
{
}

void
MFieldViewItem::InitIndexValue(int index, FieldValue *value)
{
    MFieldValue *v = (MFieldValue *) value;
    m_children[index] = CreateSFItem();
    m_children[index]->SetParent(this);
    m_children[index]->SetField(m_field);
    m_children[index]->SetIndex(index);
    m_children[index]->SetValue(v->getSFValue(index));
}

void
MFieldViewItem::SetValue(FieldValue *value)
{
    MFieldValue *v = (MFieldValue *) value;

    m_children.resize(v->getSFSize());

    for (int i = 0; i < v->getSFSize(); i++)
        InitIndexValue(i, value);

    FieldViewItem::SetValue(value);
}

MFieldViewItem::~MFieldViewItem()
{
    int n = m_children.size();
    for (int i = 0; i < n; i++) {
        delete m_children[i];
    }
}

