/*
 * KEYBOARD.CPP
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
#include "Keyboard.h"
#include "resource.h"

//#include <windows.h>
//#include <commctrl.h>
#include <GL/gl.h>

#include <stdio.h>
#include <sys/types.h>
#include <sys/timeb.h>
#include <Dialog2.h>

#include "swt.h"
//#include "swt_private.h"



// char string[128];

Keyboard::Keyboard(SWND parent)
  : Dialog(parent, IDD_KEYBOARD)
{   
    shift_value = false;
    altgr_value = false;
    LoadData();
    swnd = swGetDialogItem(_dlg, IDC_KEYBOARD_TEXT);
    swSetTextCaretPos(swnd, 1);
}

Keyboard::~Keyboard()
{
}

void
Keyboard::SaveData()
{
    swGetText(swnd, string, 128);
}

bool
Keyboard::Validate()
{
    return true;
}

void
Keyboard::LoadData()
{
    snprintf(string, 128, "%s" , "off");
    swSetText(swGetDialogItem(_dlg, IDC_EDIT_SHIFTTEXT), string);
    snprintf(string, 128, "%s" , "off");
    swSetText(swGetDialogItem(_dlg, IDC_EDIT_ALTGRTEXT), string);
    string[0] = 0;  // need to 
}

int 
Keyboard::GetCharacterPositionOfCaret(char* string)
{
    int caretPosition = swGetTextCaretPos(swnd);

// get character position
    int str_width=0; 
    int nu_char =0;
    for (nu_char = 0; str_width < caretPosition - 1; nu_char++)
    {
        char buf[128]; 
        mystrncpy_secure(buf, string + nu_char, 2);
        int char_width = swGetStringWidth(swGetDefaultFont(), (const char*) buf);
        str_width =str_width + char_width;
    }
// number of characters
    return nu_char;
}


void
Keyboard::SetCaretPositionRight(char* string)
{
    int i=0; 
    int str_width=0;  
// Caret position at client window
    int caretPosition = swGetTextCaretPos(swnd);
    do 
    {
        char buf[128];
        mystrncpy_secure(buf, string + i, 2);
        int char_width = swGetStringWidth(swGetDefaultFont(), (const char*) buf);
        str_width = str_width + char_width;
        i++;
    } while (str_width < caretPosition);
    
    swSetTextCaretPos(swnd, str_width + 1);
}




void
Keyboard::Keyboardbutton(const char* asciiChar)
{   
    char  stringLeftCaret[128];
    swGetText(swnd, string, 128);
    int nu_char = GetCharacterPositionOfCaret(string);
    mystrncpy_secure(stringLeftCaret, string + nu_char , 128 - nu_char - 1);
    
    mystrncpy_secure(string + nu_char , asciiChar, 2);
    
    mystrncpy_secure(string + nu_char + 1, stringLeftCaret, 128 - nu_char - 1);
    swSetText(swnd,string);
    SetCaretPositionRight(string);
}

void
Keyboard::OnCommand(int id)
{
     const char* asciiChar;
//////////////////////////////////////////////////////////////////////////////////////////
//  ` ~ 
        if (id == IDC_BUTTON_SQUOTATION) 
        {
/**~**/     if (shift_value == true) Keyboardbutton("\x7E");
/**`**/     else Keyboardbutton("\x60");
        //    swInvalidateWindow(_dlg);
        }
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//  1 ! 
        if (id == IDC_BUTTON_1) 
        {
/**!**/     if (shift_value == true) Keyboardbutton("\x21");
/**1**/     else Keyboardbutton("\x31");
        //  swvalidateWindow(_dlg);
        }
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//  2 @        
        if (id == IDC_BUTTON_2) 
        {
/**@**/     if (shift_value == true) Keyboardbutton("\x40");
/**2**/     if (shift_value == false) Keyboardbutton("\x32");
        //  swInvalidateWindow(_dlg);
        }
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//  3 #
        if (id == IDC_BUTTON_3) 
        {
/**#**/     if (shift_value == true) Keyboardbutton("\x23");
/**3**/     if (shift_value == false) Keyboardbutton("\x33");
        //  swInvalidateWindow(_dlg);
        }
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//  4 $ 
        if (id == IDC_BUTTON_4) 
        {
/**$**/     if (shift_value == true) Keyboardbutton("\x24");
/**4**/     else Keyboardbutton("\x34");
        //  swInvalidateWindow(_dlg);
        }
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//  5 % 
        if (id == IDC_BUTTON_5) 
        {
/**%**/     if (shift_value == true) Keyboardbutton("\x25");
/**5**/     else Keyboardbutton("\x35");
        //    swInvalidateWindow(_dlg);
        }
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//  6 ^ 
        if (id == IDC_BUTTON_6) 
        {
/**^**/     if (shift_value == true) Keyboardbutton("\x5E");
/**6**/     else Keyboardbutton("\x36");
        //  swInvalidateWindow(_dlg);
        }
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//  7  & 
        if (id == IDC_BUTTON_7) 
        {
/** & **/   if (shift_value == true) Keyboardbutton("\x26");
/** 7 **/   if (shift_value == false) Keyboardbutton("\x37");
        //  swInvalidateWindow(_dlg);
        }
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//  8 *
        if (id == IDC_BUTTON_8) 
        {
/** * **/   if (shift_value == true) Keyboardbutton("\x2A");
/**8**/     if (shift_value == false) Keyboardbutton("\x38");
        //swInvalidateWindow(_dlg);
        }
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//  9 (
        if (id == IDC_BUTTON_9) 
        {
/**(**/     if (shift_value == true) Keyboardbutton("\x28");
/**9**/     if (shift_value == false) Keyboardbutton("\x39");
        //swInvalidateWindow(_dlg);
        }
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//  0 )
        if (id == IDC_BUTTON_0) 
        {
/**)**/     if (shift_value == true) Keyboardbutton("\x29");
/**0**/     if (shift_value == false) Keyboardbutton("\x30");
        //swInvalidateWindow(_dlg);
        }
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//  - _ 
        if (id == IDC_BUTTON_MINUS_UNDERLINE) 
        {
/**-**/     if (shift_value == true) Keyboardbutton("\x5F");
/**_**/     else Keyboardbutton("\x2D");
        //swInvalidateWindow(_dlg);
        }
//////////////////////////////////////////////////////////////////////////////////////////
//  +  =
        if (id == IDC_BUTTON_PLUS) 
        {
/**=**/     if (shift_value == true) Keyboardbutton("\x3D");
/**+**/     if (shift_value == false) Keyboardbutton("\x2B");
        //swInvalidateWindow(_dlg);
        }
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//  Q q@
        if (id == IDC_BUTTON_Q)
        {
/**Q**/     if (shift_value == true) Keyboardbutton("\x51");
/**q**/     if (shift_value == false) Keyboardbutton("\x71");
        //swInvalidateWindow(_dlg);
        }
///////////////////////////////////////////////////////////////////////////////////////////
//  W w 
        if (id == IDC_BUTTON_W) 
        {
/**W**/     if (shift_value == true) Keyboardbutton("\x57");
/**w**/     else Keyboardbutton("\x77");
        //swInvalidateWindow(_dlg);
        }
////////////////////////////////////////////////////////////////////////////////////////// 
//  E e 
        if (id == IDC_BUTTON_E) 
        {
/**E**/     if (shift_value == true) Keyboardbutton("\x45");
/**e**/     if (shift_value == false) Keyboardbutton("\x65");
        //swInvalidateWindow(_dlg);
        }
//////////////////////////////////////////////////////////////////////////////////////////
//  R r 
        if (id == IDC_BUTTON_R) 
        {
/**R**/     if (shift_value == true) Keyboardbutton("\x52");
/**r**/     else Keyboardbutton("\x72");
        //swInvalidateWindow(_dlg);
        }
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//  T t 
        if (id == IDC_BUTTON_T) 
        {
/**T**/     if (shift_value == true) Keyboardbutton("\x54");
/**t**/     else Keyboardbutton("\x74");
        //swInvalidateWindow(_dlg);
        }
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//  Y y 
        if (id == IDC_BUTTON_Y) 
        {
/**Y**/     if (shift_value == true) Keyboardbutton("\x59");
/**y**/     else Keyboardbutton("\x79");
        //swInvalidateWindow(_dlg);
        }
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//  U u 
        if (id == IDC_BUTTON_U) 
        {
/**U**/     if (shift_value == true) Keyboardbutton("\x55");
/**u**/     else Keyboardbutton("\x75");
        //swInvalidateWindow(_dlg);
        }
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//  I i
        if (id == IDC_BUTTON_I) 
        {
/**I**/     if (shift_value == true) Keyboardbutton("\x49");
/**i**/     else Keyboardbutton("\x69");
        //swInvalidateWindow(_dlg);
        }
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//  O o 
        if (id == IDC_BUTTON_O) 
        {
/**O**/     if (shift_value == true) Keyboardbutton("\x4F");
/**o**/     else Keyboardbutton("\x6F");
        //swInvalidateWindow(_dlg);
        }
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//  P p 
        if (id == IDC_BUTTON_P) 
        {
/**P**/     if (shift_value == true) Keyboardbutton("\x50");
/**p**/     else Keyboardbutton("\x70");
        //swInvalidateWindow(_dlg);
        }
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//  [  {
        if (id == IDC_BUTTON_LBRACKET) 
        {
/**{**/     if (shift_value == true) Keyboardbutton("\x7B");
/**[**/     else Keyboardbutton("\x5B");
        //swInvalidateWindow(_dlg);
        }
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//  ]   }
        if (id == IDC_BUTTON_RBRACKET) 
        {
/**}**/     if (shift_value == true) Keyboardbutton("\x7D");
/**]**/     if (shift_value == false) Keyboardbutton("\x5D");
        //swInvalidateWindow(_dlg);
        }
//////////////////////////////////////////////////////////////////////////////////////////

    //

//  A a 
        if (id == IDC_BUTTON_A) 
        {
/**A**/     if (shift_value == true) Keyboardbutton("\x41");
/**a**/     else Keyboardbutton("\x61");
        //swInvalidateWindow(_dlg);
        }
//////////////////////////////////////////////////////////////////////////////////////////
//  S s 
        if (id == IDC_BUTTON_S) 
        {
/**S**/     if (shift_value == true) Keyboardbutton("\x53");
/**s**/    else Keyboardbutton("\x73");
        //swInvalidateWindow(_dlg);
        }
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//  D d 
        if (id == IDC_BUTTON_D) 
        {
/**D**/     if (shift_value == true) Keyboardbutton("\x44");
/**d**/     else Keyboardbutton("\x64");
        //swInvalidateWindow(_dlg);
        }
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//  F f 
        if (id == IDC_BUTTON_F) 
        {
/**F**/     if (shift_value == true) Keyboardbutton("\x46");
/**f**/     else Keyboardbutton("\x66");
        //swInvalidateWindow(_dlg);
        }
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//  G g 
        if (id == IDC_BUTTON_G) 
        {
/**G**/     if (shift_value == true) Keyboardbutton("\x47");
/**g**/     else Keyboardbutton("\x67");
        //swInvalidateWindow(_dlg);
        }
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//  H h
        if (id == IDC_BUTTON_H) 
        {
/**H**/     if (shift_value == true) Keyboardbutton("\x48");
/**h**/     else Keyboardbutton("\x68");
        //swInvalidateWindow(_dlg);
        }
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//  J j
        if (id == IDC_BUTTON_J) 
        {
/**J**/     if (shift_value == true) Keyboardbutton("\x4A");
/**j**/     else Keyboardbutton("\x6A");
        //swInvalidateWindow(_dlg);
        }
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//  K k 
        if (id == IDC_BUTTON_K) 
        {
/**K**/     if (shift_value == true) Keyboardbutton("\x4B");
/**k**/     else Keyboardbutton("\x6B");
        //swInvalidateWindow(_dlg);
        }
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//  L l 
        if (id == IDC_BUTTON_L) 
        {
/**L**/     if (shift_value == true) Keyboardbutton("\x4C");
/**l**/     else Keyboardbutton("\x6C");
        //swInvalidateWindow(_dlg);
        }
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//  ;  : 
        if (id == IDC_BUTTON_SEMICOLON) 
        {
/**:**/     if (shift_value == true) Keyboardbutton("\x3A");
/**;**/     else Keyboardbutton("\x3B");
        //swInvalidateWindow(_dlg);
        }
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//  '  "
        if (id == IDC_BUTTON_QUOTATIONMARKS) 
        {
        
/**"**/     if (shift_value == true) Keyboardbutton("\x22");
/**'**/     else Keyboardbutton("\x27");
        //swInvalidateWindow(_dlg);
        }
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//  \  | 
        if (id == IDC_BUTTON_BACKSLASH) 
        {
/**|**/     if (shift_value == true) Keyboardbutton("\x7C");
/**\**/     else Keyboardbutton("\x5C");
        //swInvalidateWindow(_dlg);
        }
//////////////////////////////////////////////////////////////////////////////////////////

    //


//////////////////////////////////////////////////////////////////////////////////////////
//  Z z 
        if (id == IDC_BUTTON_Z) 
        {
/**Z**/     if (shift_value == true) Keyboardbutton("\x5A");
/**z**/     else Keyboardbutton("\x7A");
        //swInvalidateWindow(_dlg);
        }
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//  X x 
        if (id == IDC_BUTTON_X) 
        {
/**X**/     if (shift_value == true) Keyboardbutton("\x58");
/**x**/     else Keyboardbutton("\x78");
        //swInvalidateWindow(_dlg);
        }
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//  C c
        if (id == IDC_BUTTON_C) 
        {
/**C**/     if (shift_value == true) Keyboardbutton("\x43");
/**c**/     else Keyboardbutton("\x63");
        //swInvalidateWindow(_dlg);
        }
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//  V v 
        if (id == IDC_BUTTON_V) 
        {
/**V**/     if (shift_value == true) Keyboardbutton("\x56");
/**v**/     else Keyboardbutton("\x76");
        //swInvalidateWindow(_dlg);
        }
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//  B b 
        if (id == IDC_BUTTON_B) 
        {
/**B**/     if (shift_value == true) Keyboardbutton("\x42");
/**b**/     else Keyboardbutton("\x62");
        //swInvalidateWindow(_dlg);
        }
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//  N n 
        if (id == IDC_BUTTON_N) 
        {
/**N**/     if (shift_value == true) Keyboardbutton("\x4E");
/**n**/     else Keyboardbutton("\x6E");
        //swInvalidateWindow(_dlg);
        }
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//  M m µ
        if (id == IDC_BUTTON_M) 
        {
/**M**/     if (shift_value == true) Keyboardbutton("\x4D");
/**µ**/     if (shift_value == false && altgr_value == true) Keyboardbutton("\xB5");
/**m**/     if (shift_value == false && altgr_value == false) Keyboardbutton("\x6D");
        //swInvalidateWindow(_dlg);
        }
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//  , <
        if (id == IDC_BUTTON_COMMA) 
        {
/**<**/     if (shift_value == true) Keyboardbutton("\x3C");
/**,**/     else Keyboardbutton("\x2C");
        //swInvalidateWindow(_dlg);
        }
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//  . > 
        if (id == IDC_BUTTON_POINT) 
        {
/**>**/     if (shift_value == true) Keyboardbutton("\x3E");
/**:**/     else Keyboardbutton("\x2E");
        //swInvalidateWindow(_dlg);
        }
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//  /  ?
        if (id == IDC_BUTTON_SLASH) 
        {
/**?**/     if (shift_value == true) Keyboardbutton("\x3F");
/** / **/   else Keyboardbutton("\x2F");
        //swInvalidateWindow(_dlg);
        }

//////////////////////////////////////////////////////////////////////////////////////////

    //


//  SPACE 
        if (id == IDC_BUTTON_SPACE) 
        {
             Keyboardbutton("\x20");
             //swInvalidateWindow(_dlg);
        }
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//SHIFT
        if (id == IDC_BUTTON_SHIFT)
        {
            shift_value = !shift_value;

            if (shift_value == false) 
            {
                snprintf(string, 128, "%s" , "off");
                swSetText(swGetDialogItem(_dlg, IDC_EDIT_SHIFTTEXT), string);
            }
            else{
            snprintf(string, 128, "%s" , "on");
            swSetText(swGetDialogItem(_dlg, IDC_EDIT_SHIFTTEXT), string);
            }
        }
//////////////////////////////////////////////////////////////////////////////////////////
//AltGr
        if (id == IDC_BUTTON_ALTGR)
        {
            altgr_value = !altgr_value;

            if (altgr_value == false) 
            {
                snprintf(string, 128, "%s" , "off");
                swSetText(swGetDialogItem(_dlg, IDC_EDIT_ALTGRTEXT), string);
            }
            else{
                snprintf(string, 128, "%s" , "on");
                swSetText(swGetDialogItem(_dlg, IDC_EDIT_ALTGRTEXT), string);
            }
        }
//////////////////////////////////////////////////////////////////////////////////////////
/**<--**/if (id == IDC_BUTTON_BACK)
        {   
        swGetText(swnd, string, 128);
        //Caret position at client window
        int caretPosition = swGetTextCaretPos(swnd);
        
        char stringLeftCaret[128];
        int str_width=0;
        int char_width=0;
        int i;
        if (caretPosition <= 1) goto NEWPOSITION;
        //Move caret position 
        for (i=0; str_width < caretPosition - 1; i++)
        {
            char buf[128];
            mystrncpy_secure(buf, string + i, 2);
            char_width = swGetStringWidth(swGetDefaultFont(), (const char*) buf);
            str_width = str_width + char_width;
        }

        caretPosition = str_width + 1 - char_width;
        mystrncpy_secure(stringLeftCaret, string + i , 128 - i - 1);
        mystrncpy_secure(string + i - 1, stringLeftCaret, 128 - i - 1);
        swSetText(swnd,string);
NEWPOSITION:if (caretPosition <= 1) caretPosition=1;
        swSetTextCaretPos(swnd,caretPosition);
        //swInvalidateWindow(_dlg);
   }

/**MOVE_LEFT**/
        if (id == IDC_BUTTON_MOVE_LEFT)
        {
            //Caret position at client window 
            int caretPosition = swGetTextCaretPos(swnd);
            swGetText(swGetDialogItem(_dlg, IDC_WIDTH), string, 128);
            //Move caret position 
            int str_width=0; 
            int char_width=0;

        for (int i=0; str_width < caretPosition - 1; i++)
        {
            char buf[128];
            mystrncpy_secure(buf, string + i, 2);
            char_width = swGetStringWidth(swGetDefaultFont(), (const char*) buf);
            str_width =str_width + char_width;
        }
    
        caretPosition = str_width + 1 - char_width;
        swSetTextCaretPos(swnd, caretPosition);
        swInvalidateWindow(_dlg);
    }
//////////////////////////////////////////////////////////////////////////////////////////
/**MOVE_RIGHT**/
    if (id == IDC_BUTTON_MOVE_RIGHT)
        {
        char compare_string[128];
        for(int i=0;i<127;i++)
            compare_string[i] = 0;
        swGetText(swGetDialogItem(_dlg, IDC_WIDTH), string, 128);
        //Caret position at client window 
        int caretPosition = swGetTextCaretPos(swnd);
        //Move caret position 
        int str_width=0;
        int char_width=0;
        int j=0; 
        do 
        {
            char buf[128];
            const char *oneChar;
            mystrncpy_secure(buf, string + j, 2);
            oneChar = &buf[0];
            if( *oneChar == compare_string[j]) goto POSITION;
            char_width = swGetStringWidth(swGetDefaultFont(), (const char*) oneChar);
            str_width = str_width + char_width;
            j++;
        } while (str_width < caretPosition);
    
        caretPosition = str_width + 1;
POSITION:swSetTextCaretPos(swnd,caretPosition);
    //swInvalidateWindow(_dlg);
        }
//////////////////////////////////////////////////////////////////////////////////////////
/**MOVE_UP**/
        if (id == IDC_BUTTON_MOVE_UP){}
//////////////////////////////////////////////////////////////////////////////////////////
/**MOVE_DOWN**/
        if (id == IDC_BUTTON_MOVE_DOWN){}
//////////////////////////////////////////////////////////////////////////////////////////
/**INSERT**/
        if (id == IDC_BUTTON_INSERT){}
//////////////////////////////////////////////////////////////////////////////////////////
//DEL
        if (id == IDC_BUTTON_DEL)
        {
        swGetText(swnd, string, 128);
        
        //Caret position at client window
        int caretPosition = swGetTextCaretPos(swnd);
        //Move caret position 
        int str_width=0;
        int char_width=0;
                                int k = 0;
        for (k=0; str_width < caretPosition - 1;k++)
        {
            char buf[128];
            mystrncpy_secure(buf, string + k, 2);
            char_width = swGetStringWidth(swGetDefaultFont(), (const char*) buf);
            str_width = str_width + char_width;
        }
        
        swSetTextCaretPos(swnd,caretPosition);

        char  stringLeftCaret[128];
        mystrncpy_secure(stringLeftCaret, string + k + 1 , 128 - k - 2);
        mystrncpy_secure(string + k , stringLeftCaret, 128 - k);
        swSetText(swnd,string);
    //swInvalidateWindow(_dlg);
        }
//////////////////////////////////////////////////////////////////////////////////////////
/**POS1**/
        if (id == IDC_BUTTON_POS1){}
//////////////////////////////////////////////////////////////////////////////////////////
/**END**/
        if (id == IDC_BUTTON_END){}
//////////////////////////////////////////////////////////////////////////////////////////
/**PAGEUP**/
        if (id == IDC_BUTTON_PAGEUP){}
//////////////////////////////////////////////////////////////////////////////////////////
/**PAGEDOWN**/
        if (id == IDC_BUTTON_PAGEDOWN){}
//////////////////////////////////////////////////////////////////////////////////////////
////CreateCaret(swnd->hWnd, NULL, 1, swGetFontHeight(swGetDefaultFont()));
////ShowCaret(swnd->hWnd);
//  swInvalidateWindow(_dlg);
    Dialog::OnCommand(id);
}

