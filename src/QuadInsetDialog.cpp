/*
 * QuadInsetDialog.cpp
 *
 * Copyright (C) 1999 Stephen F. White, 2020 J. "MUFTI" Scheurich
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

#include "stdafx.h"
#include "QuadInsetDialog.h"
#include <stdio.h>
#include <stdlib.h>
#include "swt.h"
#include "resource.h"

QuadInsetDialog::QuadInsetDialog(SWND parent, int idd, float value,
                                 float lowerlimit, float upperlimit)
  : Dialog(parent, idd)
{
    m_value = value;
    m_lowerlimit = lowerlimit;
    m_upperlimit = upperlimit;
    if (m_upperlimit < m_lowerlimit) {
        m_lowerlimit = FLT_MIN;
        m_upperlimit = FLT_MAX;
    }
    m_numX = 1;
    m_numY = 1;
    LoadData();
}

QuadInsetDialog::~QuadInsetDialog()
{
}

void
QuadInsetDialog::SaveData()
{
    char buf[128];

    swGetText(swGetDialogItem(m_dlg, IDC_VALUE), buf, 128);
    m_value = atof(buf);

    swGetText(swGetDialogItem(m_dlg, IDC_NUM_X), buf, 128);
    m_numX = atof(buf);

    swGetText(swGetDialogItem(m_dlg, IDC_NUM_Y), buf, 128);
    m_numY = atof(buf);
}

bool
QuadInsetDialog::Validate()
{
    return (m_value >= m_lowerlimit) && (m_value <= m_upperlimit) &&
           (m_numX > 0) && (m_numY > 0) && (m_numX < 4) && (m_numY < 4);
}


static void scrollCB(void *data, int type, int value)
{
    ((QuadInsetDialog *) data)->scrollCallback(type,value);
}

void
QuadInsetDialog::scrollCallback(int type, int value)
{
    char buf[128];

    m_value = value / 100.f;
    mysnprintf(buf, 128, "%f", m_value);
    swSetText(swGetDialogItem(m_dlg, IDC_VALUE), buf);
}

#define X_ScrollBar 30
#define Y_ScrollBar 80
#define ScrollBarVisit 10

void
QuadInsetDialog::LoadData()
{
    char buf[128];

    mysnprintf(buf, 128, "%f", m_value);
    swSetText(swGetDialogItem(m_dlg, IDC_VALUE), buf);

#ifndef _WIN32
    int ScrollBarWidth = 200;
    int ScrollBarHeight = 15;

    m_scrollBar = swCreateScrollBar(SW_HORIZONTAL, 
                                    X_ScrollBar, 
                                    Y_ScrollBar, 
                                    ScrollBarWidth, ScrollBarHeight, 
                                    0, 100 + ScrollBarVisit, 
                                    0, ScrollBarVisit,
                                    m_dlg);
    swSetClientData(m_scrollBar, this);
    swSetScrollBarCallback(m_scrollBar, scrollCB);
    swScrollBarSetValue(m_scrollBar, (int) (m_value * 100));
#endif

    mysnprintf(buf, 128, "%d", m_numX);
    swSetText(swGetDialogItem(m_dlg, IDC_NUM_X), buf);

    mysnprintf(buf, 128, "%d", m_numY);
    swSetText(swGetDialogItem(m_dlg, IDC_NUM_Y), buf);

}
