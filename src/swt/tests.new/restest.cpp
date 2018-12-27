/*
 * restest.c
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
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#ifdef WIN32
#include <windows.h>
#include <process.h>
#endif

#include "swttypedef.h"
#include "swt.h"
#include "res.h"
#include "resource.h"
#include "ElevationGridDialog2.h"
#include "Dialog2.h"
#include "Keyboard.h"


FILE *f;
SWND wnd, wnd_big, wnd_red, wnd_blue, wnd_green,wnd_light_green,wnd_rosa;
SWND wnd_black,wnd_yellow, wnd_lilla, wnd_grau, wnd_aqua, wnd_braun, wnd_orange;
SBITMAP bitmap, bitmap_button;
int drawing=0; int k=1;
int pr_x,pr_y=0; 
double l,m,x_Pos,y_Pos=0;


static void
mouseCallback(void *data, int x, int y, int modifiers);

static void
commandCallback(void *data, int id)
{
        if (id == ID_EDIT_DELETE) 
        {
                ElevationGridDialog   dlg((SWND) data, 0, 10);
                if (dlg.DoModal() == IDOK) {}
        }
        
        if ( id == IDD_KEYBOARD) 
        {

                Keyboard   dlg((SWND) data);
        if (dlg.DoModal() == IDOK) {}
    }
        
        
}

static void
keyCallback(void *data, int key, int value, int x, int y, int modifiers)
{
        
        if (key == SW_MOUSE1) 
        {
                fprintf(f,"                 Mausklick Button1 x Pos: %d y Pos: %d \n", x, y);
                pr_x = x;
                pr_y = y;
                k++;
        }

//                drawing = value;
//                mouseCallback(data, x, y, modifiers);

        
}


static void
mouseCallback(void *data, int x, int y, int modifiers)
{
        fprintf(f,"Mausmove x Pos: %d y Pos: %d ", x, y);
        fprintf(f,"length is: %.0f\n", l);        
        
        if (k>1)
        {
                x_Pos = pr_x - x;
                y_Pos = pr_y - y;
                m = sqrt(((x_Pos * x_Pos) + (y_Pos * y_Pos)));
                l = m+l;
                pr_x = x;
                pr_y = y;
        }        
        
/**        if (drawing)
        {
                SDC dc = swCreateDC(wnd);
                swDrawBitmap(dc, bitmap, 0, 0, x - 24, y - 24, 48, 48);
                swDestroyDC(dc);
        }
**/
                
}

#ifdef WIN32
int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE prevInstance,
                    LPSTR lpCmdLine, int nCmdShow)
#else
int main(int argc, char **argv)
#endif
{
#ifdef WIN32
    swInitialize(hInstance);
#else
    swInitialize(&argc, argv, 0);
#endif
        
        
        f = fopen("C:\\koordinaten.txt","w");
        
        wnd = swCreateMainWindow("Resource Test", 0, 0, 1024, 768);
        wnd_big = swCreateCanvas("", 0, 0, 1024, 768, wnd);
        
        wnd_red = swCreateCanvas("", 80, 80, 40, 40,wnd);
        swSetWindowBG(wnd_red,0x0000FF);

        wnd_blue = swCreateCanvas("", 400, 100, 40, 40,wnd);
        swSetWindowBG(wnd_blue,0xFF0000);

        wnd_green = swCreateCanvas("", 700, 45, 25, 25,wnd);
        swSetWindowBG(wnd_green,0x00A000);

        wnd_black = swCreateCanvas("", 700, 80, 25, 25,wnd);
        swSetWindowBG(wnd_black,0x000000);

        wnd_light_green = swCreateCanvas("", 100, 220, 25, 25,wnd);
        swSetWindowBG(wnd_light_green,0x00FF00);
        
        wnd_lilla = swCreateCanvas("", 130, 220, 25, 25,wnd);
        swSetWindowBG(wnd_lilla,0xFF00FF);

        wnd_yellow = swCreateCanvas("", 500, 300, 25, 25,wnd);
        swSetWindowBG(wnd_yellow,0x00FFFF);

        wnd_grau = swCreateCanvas("", 777, 280, 40, 40,wnd);
        swSetWindowBG(wnd_grau,0x808080);

        wnd_aqua = swCreateCanvas("", 50, 450, 25, 25,wnd);
        swSetWindowBG(wnd_aqua,0xFFFF00);

        wnd_braun = swCreateCanvas("", 450, 450, 40, 40,wnd);
        swSetWindowBG(wnd_braun,0x000080);

        wnd_orange = swCreateCanvas("", 500, 450, 40, 40,wnd);
        swSetWindowBG(wnd_orange,0x80FF);

        wnd_rosa = swCreateCanvas("", 777, 330, 40, 40,wnd);
        swSetWindowBG(wnd_rosa,0xFF80F0);



/**        bitmap_button = swLoadBitmap(button, IDB_BITMAP2);

        button = swCreateButton("", 25, 25, 14, 14, wnd_big);
        
        SDC dc1 = swCreateDC(button);
        swDrawBitmap(dc1, bitmap_button, 25, 25, 0, 0, 48, 48);
        swDestroyDC(dc1);
**/        
        
        
        swLoadMenuBar(wnd, IDR_MENU1);
//        bitmap = swLoadBitmap(wnd, IDB_BITMAP1);
        
        swSetKeyCallback(wnd_big, keyCallback);
        swSetMouseCallback(wnd_big, mouseCallback);
        swSetCommandCallback(wnd_big, commandCallback);
        swSetClientData(wnd_big, wnd_big);
        swShowWindow(wnd_big);

        swSetKeyCallback(wnd, keyCallback);
        swSetMouseCallback(wnd, mouseCallback);    
        swSetCommandCallback(wnd, commandCallback);
        swSetClientData(wnd, wnd);
        swShowWindow(wnd);
        
        swSetKeyCallback(wnd_red, keyCallback);
        swSetMouseCallback(wnd_red, mouseCallback); 
        swSetCommandCallback(wnd_red, commandCallback);
        swSetClientData(wnd_red, wnd_red);
        swShowWindow(wnd_red);
        
        swSetKeyCallback(wnd_blue, keyCallback);
        swSetMouseCallback(wnd_blue, mouseCallback); 
        swSetCommandCallback(wnd_blue, commandCallback);
        swSetClientData(wnd_blue, wnd_blue);
        swShowWindow(wnd_blue);
        
        swSetKeyCallback(wnd_green, keyCallback);
        swSetMouseCallback(wnd_green, mouseCallback); 
        swSetCommandCallback(wnd_green, commandCallback);
        swSetClientData(wnd_green, wnd_green);
        swShowWindow(wnd_green);

        swSetKeyCallback(wnd_light_green, keyCallback);
        swSetMouseCallback(wnd_light_green, mouseCallback); 
        swSetCommandCallback(wnd_light_green, commandCallback);
        swSetClientData(wnd_light_green, wnd_light_green);
        swShowWindow(wnd_light_green);

        swSetKeyCallback(wnd_black, keyCallback);
        swSetMouseCallback(wnd_black, mouseCallback); 
        swSetCommandCallback(wnd_black, commandCallback);
        swSetClientData(wnd_black, wnd_black);
        swShowWindow(wnd_black);

        swSetKeyCallback(wnd_lilla, keyCallback);
        swSetMouseCallback(wnd_lilla, mouseCallback); 
        swSetCommandCallback(wnd_lilla, commandCallback);
        swSetClientData(wnd_lilla, wnd_lilla);
        swShowWindow(wnd_lilla);

        swSetKeyCallback(wnd_yellow, keyCallback);
        swSetMouseCallback(wnd_yellow, mouseCallback); 
        swSetCommandCallback(wnd_yellow, commandCallback);
        swSetClientData(wnd_yellow, wnd_yellow);
        swShowWindow(wnd_yellow);
        
        swSetKeyCallback(wnd_grau, keyCallback);
        swSetMouseCallback(wnd_grau, mouseCallback); 
        swSetCommandCallback(wnd_grau, commandCallback);
        swSetClientData(wnd_grau, wnd_grau);
        swShowWindow(wnd_grau);

        swSetKeyCallback(wnd_aqua, keyCallback);
        swSetMouseCallback(wnd_aqua, mouseCallback); 
        swSetCommandCallback(wnd_aqua, commandCallback);
        swSetClientData(wnd_aqua, wnd_aqua);
        swShowWindow(wnd_aqua);

        swSetKeyCallback(wnd_braun, keyCallback);
        swSetMouseCallback(wnd_braun, mouseCallback); 
        swSetCommandCallback(wnd_braun, commandCallback);
        swSetClientData(wnd_braun, wnd_braun);
        swShowWindow(wnd_braun);
        
        swSetKeyCallback(wnd_orange, keyCallback);
        swSetMouseCallback(wnd_orange, mouseCallback); 
        swSetCommandCallback(wnd_orange, commandCallback);
        swSetClientData(wnd_orange, wnd_orange);
        swShowWindow(wnd_orange);
        
        swSetKeyCallback(wnd_rosa, keyCallback);
        swSetMouseCallback(wnd_rosa, mouseCallback); 
        swSetCommandCallback(wnd_rosa, commandCallback);
        swSetClientData(wnd_rosa, wnd_rosa);
        swShowWindow(wnd_rosa);
        swMainLoop();
        return 0;        
}
