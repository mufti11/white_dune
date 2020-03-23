/*
 * ElevationGridDialog.cpp
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

#include "stdafx.h"
#include "ElevationGridDialog2.h"
#include "resource.h"

#ifdef WIN32
# include <windows.h>
//#include <commctrl.h>
#else
# include <stdlib.h>
#endif
//#include <GL/gl.h>

#include <stdio.h>
#include <sys/types.h>
#include <sys/timeb.h>
#include <Dialog2.h>
#include "swt.h"
//#include "swt_private.h"


ElevationGridDialog::ElevationGridDialog(SWND parent, int number, int depth)
  : Dialog(parent, IDD_ELEVATION)
{
    _width = number;
    _widthSwnd = swGetDialogItem(_dlg, IDC_WIDTH);
    _depth = depth;
    _depthSwnd = swGetDialogItem(_dlg, IDC_DEPTH);
    LoadData();
}

ElevationGridDialog::~ElevationGridDialog()
{
}

void
ElevationGridDialog::SaveData()
{
    char number[128];
    swGetText(_widthSwnd, number, 128);
    _width = atoi(number);
    swGetText(_depthSwnd, number, 128);
    _depth = atoi(number);
}

bool
ElevationGridDialog::Validate()
{
    return _width > 0 && _depth > 0;
}

void
ElevationGridDialog::LoadData()
{
    char number[128];
    snprintf(number, 128, "%d", _width);
    swSetText(_widthSwnd, number);
    snprintf(number, 128, "%d", _depth);
    swSetText(_depthSwnd, buf);

}

int
ElevationGridDialog::GetCharacterPosition(SWND swnd, char* number)
{
    char compare_string[128];
    char zero_string[128];
    for(int k=0;k<127;k++)
    {
        zero_string[k] = '0';
        zero_string[k + 1] = 0;
        compare_string[k] = 0;
    }
    
    //Caret position at client window 
    int caretPosition = swGetTextCaretPos(swnd);

    //get character position
    int str_width=0;
    int j=0;
    int nu_char;
    for (nu_char=0; str_width < caretPosition - 1; nu_char++)
    {
        if(number[j] == compare_string[j])
        {
            mystrncpy_secure(number + j, zero_string + j, 2);
        }
    
        char buf[128]; 
        mystrncpy_secure(buf, number + nu_char, 2);
        int char_width = swGetStringWidth(swGetDefaultFont(), 
                                          (const char*) buf);
        str_width =str_width + char_width;
        j++;
    }
    return nu_char;
}

void
ElevationGridDialog::OnCommand(int id)
{ 
    if (id == ID_INCREASE)
    {
        swGetText(_widthSwnd, number, 128);

        // nu_char: number of character, which included in "number"  
        int nu_char = GetCharacterPosition(_widthSwnd, number);

        //Increase integer
        char buf[128];
        char temporary[128];
        mystrncpy_secure(temporary, number, 128);
        mystrncpy_secure(buf, number + nu_char , 2);

        int i = atoi(buf);
        i = i + 1;

        if (i == 10)
            i = 0;

        mysnprintf(buf,128,"%d",i);
        mystrncpy_secure(number + nu_char , buf, 2);
        mystrncpy_secure(number + nu_char + 1, temporary + nu_char + 1, 128 - nu_char - 1);

        swSetText(_widthSwnd, number);
        swInvalidateWindow(_dlg);
    }

    if (id == ID_DECREASE)
    { 
        swGetText(_widthSwnd, number, 128);

    // nu_char: number of character, which included in "number"  
        int nu_char = GetCharacterPosition(_widthSwnd, number);

    //Decrease integer
        char buf[128];
        char temporary[128];
        mystrncpy_secure(temporary, number, 128);
        mystrncpy_secure(buf, number + nu_char , 2);
    
        int i = atoi(buf);
        i = i - 1;

        if (i < 0) 
            i = 9;

        mysnprintf(buf,128,"%d",i);
        mystrncpy_secure(number + nu_char , buf, 2);
        mystrncpy_secure(number + nu_char + 1, temporary + nu_char + 1, 128 - nu_char - 1);

        swSetText(_widthSwnd, number);
        swInvalidateWindow(_dlg);
    }

    if (id == ID_MOVE_RIGHT)
    {
    //Caret position at client window 
        int caretPosition = swGetTextCaretPos(_widthSwnd);
        swGetText(_widthSwnd, buf, 128);

    //Move caret position 
        int str_width=0; 
        int char_width=0;
        int i=0;

        do 
        {
            char buf[128];
            mystrncpy_secure(buf, number + i, 2);
            char_width = swGetStringWidth(swGetDefaultFont(), (const char*) buf);
            str_width =str_width + char_width;
            i++;
        } while (str_width < caretPosition);
        caretPosition = str_width + 1;
        swSetTextCaretPos(_widthSwnd, caretPosition);
        swInvalidateWindow(_dlg);
    }

    if (id == ID_MOVE_LEFT)
    {
    //Caret position at client window 
        int caretPosition = swGetTextCaretPos(_widthSwnd);
        swGetText(_widthSwnd, number, 128);
    
    //Move caret position 
        int str_width=0; 
        int char_width=0;

        for (int i=0; str_width < caretPosition - 1; i++)
        {
            char buf[128];
            mystrncpy_secure(buf, number + i, 2);
            char_width = swGetStringWidth(swGetDefaultFont(), (const char*) buf);
            str_width = str_width + char_width;
        }
        caretPosition = str_width + 1 - char_width;
        swSetTextCaretPos(_widthSwnd, caretPosition);
        swInvalidateWindow(_dlg);
    }

////  CreateCaret(_widthSwnd->hWnd, (HBITMAP) NULL, 1, swGetFontHeight(swGetDefaultFont()));
////  ShowCaret(_widthSwnd->hWnd);

    Dialog::OnCommand(id);

}
