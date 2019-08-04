/*
 * SliderFloatDialog.cpp
 *
 * Copyright (C) 1999 Stephen F. White, 2019 J. "MUFTI" Scheurich
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
#include "SliderFloatDialog.h"
#include <stdio.h>
#include <stdlib.h>
#include "swt.h"
#include "resource.h"

SliderFloatDialog::SliderFloatDialog(SWND parent, int idd, float value,
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
    LoadData();
}

SliderFloatDialog::~SliderFloatDialog()
{
}

void
SliderFloatDialog::SaveData()
{
    char buf[128];

    swGetText(swGetDialogItem(m_dlg, IDC_VALUE), buf, 128);
    m_value = atof(buf);
}

bool
SliderFloatDialog::Validate()
{
    return (m_value >= m_lowerlimit) && (m_value <= m_upperlimit);
}


static void scrollCB(void *data, int type, int value)
{
    ((SliderFloatDialog *) data)->scrollCallback(type,value);
}

void
SliderFloatDialog::scrollCallback(int type, int value)
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
SliderFloatDialog::LoadData()
{
    char buf[128];

    mysnprintf(buf, 128, "%f", m_value);
    swSetText(swGetDialogItem(m_dlg, IDC_VALUE), buf);

#ifndef _WIN32
    int ScrollBarWidth = 200;
    int ScrollBarHeight = 15;
    int TextEditWidth = 30;
    int TextEditHeight = 15;
    int ButtonWidth = 80;

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
}
