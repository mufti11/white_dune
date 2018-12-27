/*
 * ColorPaletteCircle.c
 *
 * Copyright (C) 2002 Christian Hanisch
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

#ifdef WIN32
#include <windows.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#ifndef FLT_MAX
# include <float.h>
#endif
#include "swt.h"
extern "C" {
#include "../../mysnprintf.h"
}

#define REDWINKEL 0.0
#define YELLOWWINKEL 60.0
#define GREENWINKEL 120.0
#define CYANWINKEL 180.0
#define BLUEWINKEL 240.0
#define MAGENTAWINKEL 300.0
#define MAXWINKEL 360.0

#define MAXFARBANTEIL 255
#define AUFLOESUNG 100

#define X_ScrollBar 30
#define Y_ScrollBar_TextEdit 160
#define Y_ABSTAND 20
#define ScrollBarVisit 10

#define VALUEBARWIDTH 20

#define X_TEXTEDIT 240

#define X_WINDOWSIZE 300
#define Y_WINDOWSIZE 240

#define X0_POINT -20
#define Y0_POINT 200

SWND canvas, RedScrollBar, GreenScrollBar, BlueScrollBar, CloseButton, 
     OkeyButton, RedTextEdit, GreenTextEdit, BlueTextEdit;
int drawing = 0;

/**
 * SetGetColorPoint -- Setzen oder Rueckgabe der gemerkten Farbe
 *
 * Color  zu setzende Farbe
 *
 * Set    wenn gesetzt soll die Farbe sie gesetzt werden. Wenn nicht gesetzt soll nur
 *        die Farbe uebergeben werden.
 *
 * RETURN Rueckgabe der aktuellen Farbe
 */
int SetGetColorPoint(int Color, bool Set)
{
        static int ColorAll=0;

        if(Set)
                ColorAll = Color;
             return ColorAll;
}

void OldColor(float *Red, float *Green, float *Blue, bool oldcolor)
{
        static float RedValue, GreenValue, BlueValue;
        
        if(oldcolor)
        {
          RedValue = *Red;
          GreenValue = *Green;
          BlueValue = *Blue;
        }else
        {
          *Red = RedValue;
          *Green = GreenValue;
          *Blue = BlueValue;
        }
}

/**
 * ScrollBar -- Stuktur fuer die ScrollBars zum setzten der Position und der Groesse fuer
 *                der Funktion risizeCallBack gedacht
 *
 * x                X - Position des jeweiligen ScrollBar
 *
 * y                Y - Position des jeweiligen ScrollBar
 *
 * width        Breite des jeweiligen ScrollBar
 *
 * height        Hoehe des jeweiligen ScrollBar
 */
struct ScrollBar
{
        int x, y;
        int width, height;
};


/**
 * TextEdit -- Stuktur fuer die TextEdits zum setzten der Position und der Groesse fuer
 *                der Funktion risizeCallBack gedacht
 *
 * x                X - Position des jeweiligen TextEdit
 *
 * y                Y - Position des jeweiligen TextEdit
 *
 * width        Breite des jeweiligen TextEdit
 *
 * height        Hoehe des jeweiligen TextEdit
 */struct TextEdit
{
        int x, y;
        int width, height;
};



/**
 * Button -- Stuktur fuer die Buttons zum setzten der Position und der Groesse fuer
 *             der Funktion risizeCallBack gedacht
 *
 * x                X - Position des jeweiligen Button
 *
 * y                Y - Position des jeweiligen Button
 *
 * width        Breite des jeweiligen Button
 *
 * height        Hoehe des jeweiligen Button
 */
struct Button
{
        int x, y;
        int width, height;
};

/**
 * ScrollBars -- Struktur fuer die ScrollBars der jeweiligen Farbanteile Rot, Gruen und
 *                 Blau
 *
 * red                ScrollBar von dem Rotanteil der Positionen und Groessen
 *
 * green        ScrollBar von dem Gruenanteil der Positionen und Groessen
 *
 * blue                ScrollBar von dem Blauanteil der Positionen und Groessen
 */
struct ScrollBars
{
        struct ScrollBar red;
        struct ScrollBar green;
        struct ScrollBar blue;
};

/**
 * TextEdits -- Struktur fuer die TextEdits der jeweiligen Farbanteile Rot, Gruen und Blau
 *
 * red                ScrollBar von dem Rotanteil der Positionen und Groessen
 *
 * green        ScrollBar von dem Gruenanteil der Positionen und Groessen
 *
 * blue                ScrollBar von dem Blauanteil der Positionen und Groessen
 */
struct TextEdits
{
        struct TextEdit red;
        struct TextEdit green;
        struct TextEdit blue;
};


/**
 * Buttons -- Struktur fuer die Buttons der jeweiligem Button
 *
 * okey                 Button fuer das Okay zum Bestaetigen der ausgewaehlten Farbe der Positionen und Groessen
 *
 * close         Button fuer das Close zum Abbrechen der Farbauswahl der Positionen und Groessen
 */
struct Buttons
{
        struct Button okay;
        struct Button close;
};  

/**
 * SetScrollBar -- Setzen der Position und Groesse aller ScrollBars, TextEdit und Buttons 
 *                   bei einem resizeCallBack
 *
 * scroll        alle Werte fuer die Postion und Groesse der ScrollBars
 * 
 * TextEdit        alle Werte fuer die Postion und Groesse der TextEdits
 *
 * buton        alle Werte fuer die Postion und Groesse der buttons
 */
void SetScrollBar(struct ScrollBars scroll, struct TextEdits textedit, struct Buttons button)
{

//         Hier kann die ScrollBar Groesse und die Button Groesse mit dem Befehl 
//        extern void        swSetSize(SWND wnd, int width, int height); gesetzt werden.

        swSetPosition(RedScrollBar, scroll.red.x, scroll.red.y);
        swSetPosition(GreenScrollBar, scroll.green.x, scroll.green.y);
        swSetPosition(BlueScrollBar, scroll.blue.x, scroll.blue.y);

        swSetPosition(RedTextEdit, textedit.red.x, textedit.red.y);
        swSetPosition(GreenTextEdit, textedit.green.x, textedit.green.y);
        swSetPosition(BlueTextEdit, textedit.blue.x, textedit.blue.y);

        swSetPosition(OkeyButton, button.okay.x, button.okay.y);
        swSetPosition(CloseButton, button.close.x, button.close.y);        

}   

/**
 * window_size -- Struktur fuer den Mittelpunkt des Farbkreises und der Groesse des
 *                   Arbeitsfensters und des Radius des Farbkreises
 * 
 * x0                Position vom Mittelpunkt des Farbkreises in der X-Achse
 * 
 * y0                Position vom Mittelpunkt des Farbkreises in der Y-Achse
 * 
 * x_window        Fenstergoesse in der X-Achse
 *
 * y_window        Fenstergoesse in der Y-Achse
 *
 * Radius        Radius des Farbkreises
 */
struct window_size
{
        int x0, y0;
        int x_window, y_window;
        float Radius;
};

/**
 * GetPositions -- Errechnen der Stellen oder auch Postionen fuer alle Objekte
 * 
 * windowback        Inhalt aller wichtigen Information fuer die Stellungen der Objekte
 *
 * resize        Pruefen ob es sich um ein resizeCallBack handelt oder nicht, um sich die
 *                neue Fenstergroesse zumerken und sie dann auch setzen, ruft dabei die Funktion 
 *                SetScrollBar auf um die Objekte zusetzen
 */
void GetPositions(struct window_size* windowback, bool resize)
{
        static struct window_size window = {X0_POINT, Y0_POINT, X_WINDOWSIZE, Y_WINDOWSIZE, 0};
        struct ScrollBars scrollbar;
        struct TextEdits textedit;
        struct Buttons button;
        static int x_SetPalette, y_SetPalette;
        
        if(resize)
        {
          window.x_window = windowback->x_window;
          window.y_window = windowback->y_window;
          button.okay.x = (window.x_window / 4) - 60; 
          button.okay.y = window.y_window - 40; 
          button.close.x = (window.x_window / 2) + 30; 
          button.close.y = window.y_window - 40;
          scrollbar.red.x = (window.x_window / 4) - 40;
          scrollbar.red.y = window.y_window  - 60 - 2*Y_ABSTAND;
          scrollbar.green.x = (window.x_window / 4) - 40;
          scrollbar.green.y = window.y_window  - 60 - Y_ABSTAND;
          scrollbar.blue.x = (window.x_window / 4) - 40;
          scrollbar.blue.y = window.y_window - 60;
          textedit.red.x = (window.x_window / 2) + 90;
          textedit.red.y = window.y_window - 100;
          textedit.green.x = (window.x_window / 2) + 90;
          textedit.green.y = window.y_window  - 100 + Y_ABSTAND;
          textedit.blue.x = (window.x_window / 2) + 90;
          textedit.blue.y = window.y_window - 100 + 2*Y_ABSTAND;
          SetScrollBar(scrollbar, textedit, button);
          
        }

        window.x0 = (window.x_window / 2) - 30;

        if(window.x_window>window.y_window*0.4)
          {
            windowback->Radius = ((window.x_window - window.x0-60)/ 2);
          }else
          {
            windowback->Radius = ((window.y_window * 0.4) / 2);
          }
         
          
        windowback->y0 = (int) windowback->Radius + 10;
        windowback->x0 = (window.x_window / 2) + (int) windowback->Radius - 60;        
        
        windowback->x_window = window.x_window;
        windowback->y_window = window.y_window;
          
}


/**
 * Round_float_to_int -- Runden einer Floatzahl
 *
 * number        zu rundende Zahl
 *
 * RETURN        gerundeter Integer
 */
int Round_float_to_int(float number)
{
        float rest;

        rest = number - floor(number);
        number -= rest;
        if(rest>=0.5)
          number++;
        
        return (int) number;
         
}

/**
 * GetColorFloatToInt -- Errechnet die zusammengesetze Farbe von Scrollbar(0-100)  float werten in
 *                         Farbwerte(0-255) und setzt diese dann zusammen in rin Integer Wert
 *                         um diesen gibt er dann als Rueckgabewert zurueck
 * 
 * red                        Rotanteil der ganzen Farbe
 * 
 * green                Gruenanteil der ganzen Farbe
 *
 * blue                        Blauanteil der ganzen Farbe
 *
 * RETURN                Rueckgabe der vollstaendigen Farbe als ein Integerwert
 */
int GetColorFloatToInt(float red, float green, float blue)
{  
        int Color;
        int red_int, green_int, blue_int;
        
        
        red_int = Round_float_to_int(red * MAXFARBANTEIL);
        green_int = Round_float_to_int(green * MAXFARBANTEIL);
        blue_int = Round_float_to_int(blue * MAXFARBANTEIL);
                
        Color = 0;
        Color += (red_int & 0xff);
            Color += (green_int & 0xff) << 8;
            Color += (blue_int & 0xff) << 16;
        return Color;
} 


/**
 * GetBarValue -- Holen einer Farbe vom Farbbalken
 * 
 * x                X-Achse des Farbbalken von der die Farbe ermittlet werden soll
 * 
 * y                Y-Achse des Farbbalken von der die Farbe ermittlet werden soll
 *
 * RETURN        Rueckgabe der jeweiligen Farbe als Integer, wenn die y Koordinate nicht im Farbbalken ist
 *                dann gibt die Funktion die Fehlermeldung -1 zurueck
 *
 * ACHTUNG        Die Funtkion berechnet selbststaendig alle Positionen aus
 */
int GetBarValue(int y)
{
        struct window_size window;
        int Color, Red, Green, Blue;
        float RedColor=0, GreenColor=0, BlueColor=0;
        float DifRedValue, DifGreenValue, DifBlueValue;
        int y0, y_again, y1;
        
        GetPositions(&window, false);

              Color = SetGetColorPoint(0, false);
        Red = (Color & 0xff);
        Green = (Color & 0xff00) >> 8;
        Blue = (Color & 0xff0000) >> 16;
        
        y0 = window.y0 - (int) window.Radius;        
        y1 = y - y0;
        y_again = 2 * (int) window.Radius;
        
        if(y1!=0)
        { 
          RedColor =  (1 / ((window.Radius * 2) / y1) * (float) Red);
          GreenColor =  (1 / ((window.Radius * 2) / y1) * (float) Green);
          BlueColor =  (1 / ((window.Radius * 2) / y1) * (float) Blue);
        }
        if(Color==0)
        { 
          RedColor =  (1 / ((window.Radius * 2) / y1) * MAXFARBANTEIL);
          GreenColor =  (1 / ((window.Radius * 2) / y1) * MAXFARBANTEIL);
          BlueColor =  (1 / ((window.Radius * 2) / y1) * MAXFARBANTEIL);
        }

        if((y<=y_again+y0)&&(y>=y0))
        {
          Red = (int) RedColor;
          Green = (int) GreenColor;
          Blue = (int) BlueColor;
          if(Red>=MAXFARBANTEIL+1)
            Red = MAXFARBANTEIL;
          if(Green>=MAXFARBANTEIL+1)
            Green = MAXFARBANTEIL;
          if(Blue>=MAXFARBANTEIL+1)
            Blue = MAXFARBANTEIL;
          Color = (Red & 0xff);
          Color += (Green & 0xff) << 8;
          Color += (Blue & 0xff) << 16;

          return Color;  
        }else
        {
          return -1;
        }
}

/**
 * SetBarValue -- Setzen des Farbbalkens in die canvas(Leinwand) auf dem Bildschirm
 *
 * ACHTUNG        Die Funtkion berechnet selbststaendig alle Positionen aus
 */
void SetBarValue()
{
        SDC                dc = swCreateDC(canvas);
        struct window_size window;
        int x0, y0, x_again, y_again, i, k, Color;

        GetPositions(&window, false);
         x0 = window.x0 + (int) window.Radius + 30;                                
        y0 = window.y0 - (int) window.Radius;        
        x_again = VALUEBARWIDTH;
        y_again = 2 * (int) window.Radius;

        for(i=y0;i<=y_again+y0;i++)
        {
          Color = GetBarValue(i);
          for(k=x0;k<=x_again+x0;k++)
          {
            if(Color!=-1)
            {
              swSetFGColor(dc, Color);
              swDrawPoint(dc, k , i);
            }
          }
        }
            swDestroyDC(dc);
}



/**
 * Colors -- Struktur fuer die Farbanteile um nur die Farbanteile von der GetColorCircle
 *                 zu uebergeben. Benoetigen einige Funktionen
 * 
 * red                Rotanteil als float
 * 
 * green        Gruenanteil als float
 * 
 * blue                Blauanteil als float
 */
struct Colors
{
   float red;
   float green;
   float blue;
};

/**
 * GetColorCircle -- Holen der Farbe fuer den Farbkreis
 * 
 * x                X-Position des holende Farbe fuer dem Farbkeis
 * 
 * y                Y-Position des holende Farbe fuer dem Farbkeis
 *
 * colors        Farbenuebergabe in float(0-1.00) brauchen einige Funktionen
 *
 * ACHTUNG        Die Funtkion berechnet selbststaendig alle Positionen aus
 */
int GetColorCircle(int x, int y, struct Colors* colors)
{
         float red, green, blue;
        int Color;
        float alpha;
        float r, r2, R;
        struct window_size window;

        GetPositions(&window, false);

        x = x - window.x0;                                
        y = y - window.y0;
        R = window.Radius;
        
         alpha = (atan2((float)y, (float)x) * 180.0 / M_PI) + 180;  // Winkel berechnen
   
   
           r2 = (x * x) + (y * y);                
           r = sqrt(r2);                // r-achse berechnen (Groesse des Kreises)


        if(r<=R)                                // Pruefen ob die Koordinate im Kreis ist
        {
          float temp = 1 - (r / R);
            if(alpha>BLUEWINKEL)
          { 
            if (alpha>MAGENTAWINKEL)
            {
               blue = 1 - alpha / (MAXWINKEL - MAGENTAWINKEL) 
                       + MAGENTAWINKEL / (MAXWINKEL - MAGENTAWINKEL);
               red = 1;
            }
            else 
            {
               blue = 1;
               red =  alpha / (MAGENTAWINKEL - BLUEWINKEL) 
                        - BLUEWINKEL / (MAGENTAWINKEL - BLUEWINKEL);
            }
            green = temp;
            blue=blue - temp * blue + temp;
            red=red - temp * red + temp;          
            }else        if(alpha>GREENWINKEL)                // Winkelbereich
          { 
            if (alpha>CYANWINKEL)
            {
               green = 1 - alpha / (BLUEWINKEL - CYANWINKEL) 
                       + CYANWINKEL / (BLUEWINKEL - CYANWINKEL);
               blue = 1;
            }
            else 
            {
               green = 1;
               blue = alpha / (CYANWINKEL - GREENWINKEL) 
                        - GREENWINKEL / (CYANWINKEL - GREENWINKEL);
            }
            red = temp;
            blue=blue - temp * blue + temp;
            green=green - temp * green + temp;
          }else if(alpha>REDWINKEL)                // Winkelbereich
          {
            if (alpha>YELLOWWINKEL)
            {
               red = 1- alpha / (GREENWINKEL - YELLOWWINKEL) 
                       + YELLOWWINKEL / ( GREENWINKEL - YELLOWWINKEL);
               green = 1;
            }
            else 
            {
               red = 1;
               green =  alpha / (YELLOWWINKEL - REDWINKEL) 
                        - REDWINKEL / (YELLOWWINKEL - REDWINKEL);
            }
            red=red - temp * red + temp;
            green=green - temp * green + temp;
            blue = temp;
          } 
          Color=0;
          colors->red=red;
          colors->green=green;
          colors->blue=blue;
          Color = GetColorFloatToInt(red, green, blue);
        }else 
        {                           // 1 - (R / r) *255
          Color = -1;
        }
        return Color;                                // Rueckgabe der Farbe fuer die Zeichnung
}

/**
 * SetColorPalette -- Setzen der FarbauswahlRechtecks
 * 
 * Color        Diese Farbe wird als Rechteck in das Fenster geschrieben
 * 
 * 
 * ACHTUNG das Rechteck wird immer an die gleiche Position geschrieben, es ist noch nicht
 *                 im resizeCallBack mit drin
 */
void SetColorPalette(int Color)
{
        SDC                dc = swCreateDC(canvas);
        int i, k;
        int start_x=10, start_y=10, end_x=50, end_y=50;

              swSetFGColor(dc, Color);

        for(i=start_x;i<end_x;i++)
        {
          for(k=start_y;k<end_y;k++)
          {
            swDrawPoint(dc, i , k);
          }
        }        
        swDestroyDC(dc);
}

/**
 * ClearCircleCursor -- Loeschen des FarbKreis Cursor
 * 
 * x                X-Position des Farbkreis Cursor
 *  
 * y                Y-Position des Farbkreis Cursor
 */
void ClearCircleCursor(int x, int y)
{
        SDC                dc = swCreateDC(canvas);
            struct Colors colors = {0,0,0};
        int i, k, Color;

        for(i=0;i<=8;i++)
        {
          for(k=0;k<=8 - 1;k++);
          {
            Color = GetColorCircle((x + k) - 4, (y + i) - 4, &colors);
            if(Color!=-1)
            {
              Color = GetColorFloatToInt(colors.red, colors.green, colors.blue);
              swSetFGColor(dc, Color);
              swDrawPoint(dc, (x + k) - 4 , (y + i) - 4);
            }else
            {           
               swSetFGColor(dc, swGetWindowColor(canvas, SW_COLOR_WINDOW_BG));
              swDrawPoint(dc, (x + k) - 4 , (y + i) - 4);
            }
            Color = GetColorCircle((x + i) - 4, (y + k) - 4, &colors);
            if(Color!=-1)
            {
              swSetFGColor(dc, Color);
              swDrawPoint(dc, (x + i) - 4 , (y + k) - 4);
            }else
            {
               swSetFGColor(dc, swGetWindowColor(canvas, SW_COLOR_WINDOW_BG));
              swDrawPoint(dc, (x + i) - 4 , (y + k) - 4);
            }
          }
        }
        for(i=8;i>=0;i--)
        {
          for(k=8;k>=0 + 1;k--);
          {
            Color = GetColorCircle((x + k) - 4, (y + i) - 4, &colors);        
            if(Color!=-1)
            {
              swSetFGColor(dc, Color);
              swDrawPoint(dc, (x + k) - 4 , (y + i) - 4);
            }else
            {
               swSetFGColor(dc, swGetWindowColor(canvas, SW_COLOR_WINDOW_BG));
              swDrawPoint(dc, (x + k) - 4 , (y + i) - 4);
            }    
            Color = GetColorCircle((x + i) - 4, (y + k) - 4, &colors);
            if(Color!=-1)
            {
              swSetFGColor(dc, Color);
              swDrawPoint(dc, (x + i) - 4 , (y + k) - 4);
            }else
            {
               swSetFGColor(dc, swGetWindowColor(canvas, SW_COLOR_WINDOW_BG));
              swDrawPoint(dc, (x + i) - 4 , (y + k) - 4);
            }
          }
        }
        swDestroyDC(dc);
}


/**
 * DrawCircleCursor -- Setzen des FarbKreis Cursor
 * 
 * x                X-Position des Farbkreis Cursor
 *  
 * y                Y-Position des Farbkreis Cursor
 */
void DrawCircleCursor(int x, int y)
{
        SDC                dc = swCreateDC(canvas);
        int i, k;

        swSetFGColor(dc, 0);
        for(i=0;i<=8;i++)
        {
          for(k=0;k<=8 - 1;k++);
          {
            swDrawPoint(dc, (x + k) - 4 , (y + i) - 4);
            swDrawPoint(dc, (x + i) - 4 , (y + k) - 4);
          }
        }
        for(i=8;i>=0;i--)
        {
          for(k=8;k>=0 + 1;k--);
          {
            swDrawPoint(dc, (x + k) - 4 , (y + i) - 4);
            swDrawPoint(dc, (x + i) - 4 , (y + k) - 4);
          }
        }
        swDestroyDC(dc);
}

/**
 * SetCircleCursor -- Ermoeglicht eine Animation des Farbkreis Cursor
 * 
 * x                neue X-Position
 * 
 * y                neue Y-Position
 * 
 * Color        neue Farbe in der Farbbalken Cursor erstellt werden soll
 */
void SetCircleCursor(int x, int y)
{
        static int xo, yo;
        
        if((x!=xo)||(y!=yo))
        {
          ClearCircleCursor(xo, yo);
          DrawCircleCursor(x, y);
          xo = x;
          yo = y;
        }
}


/**
 * GetCircleCursor -- Ermittelt die Cursor Position des Farbkreises per ScrollBar
 *                                         einstellung
 * 
 * RedValue                Der Rotanteil des RedScrollBar
 * 
 * GreenValue                Der Gruenanteil des RedScrollBar
 * 
 * BlueValue                Der Blauanteil des RedScrollBar
 */
void GetCircleCursor(int ColorNew)
{
        int Color, ColorIndex, x, y, i, k, again;
        int red, green, blue, Radius, k_merk, i_merk;
        int RedValue, GreenValue, BlueValue;
            struct Colors colors = {0,0,0};
        struct window_size window;
        float minwert=FLT_MAX, absolutwert;
        bool merk=false;

        SDC dc = swCreateDC(canvas);
        
              Color = SetGetColorPoint(5, false);

        RedValue = (Color & 0xff);;
        GreenValue = ((Color & 0xff00) >> 8);
        BlueValue = ((Color & 0xff0000) >> 16);

        GetPositions(&window, false);
                  
          x = window.x0 - (int) window.Radius;
        y = window.y0 - (int) window.Radius;
        again = 2 * (int) window.Radius;

        i_merk=window.x0;
        k_merk=window.y0;


        for(i=y;i<=again+y;i++)
        {
          for(k=x;k<=again+x;k++)
          {
            Color = GetColorCircle(k, i, &colors);
            if(Color!=-1)
            {
               red = Round_float_to_int(colors.red * MAXFARBANTEIL);
               green = Round_float_to_int(colors.green * MAXFARBANTEIL);
               blue = Round_float_to_int(colors.blue * MAXFARBANTEIL);

              if((RedValue==red)&&(GreenValue==green)&&(BlueValue==blue))
              {
                SetCircleCursor(k, i);
                return;
              } else 
              {
                absolutwert = (float)fabs((float)(red - RedValue)) + 
                              (float)fabs((float)(green - GreenValue)) + 
                              (float)fabs((float)(blue - BlueValue));

                if(minwert>absolutwert)
                {
                  minwert = absolutwert;
                  k_merk = k;
                  i_merk = i;
                }
              }
             }
           }
        }
        SetCircleCursor(k_merk, i_merk);        
        swDestroyDC(dc);                
}

/**
 * SetTextColorsValues -- Setzt alle Farbwerte in den jeweiligen Textfeldern der ScrollBars
 *                                   ein fuer die installisierung gedacht
 * 
 * value                Farbe fuer die jeweiligen Farbsetztung
 */
void SetTextColorsValues(int value)
{
        static float Red = 0, Green = 0, Blue = 0;
        char ColorValue[20];

         SetColorPalette(value);
        Red = (value & 0xff) / ((float)MAXFARBANTEIL/(float)AUFLOESUNG);
        Green = ((value & 0xff00) >> 8) / ((float)MAXFARBANTEIL/(float)AUFLOESUNG);
        Blue = ((value & 0xff0000) >> 16) / ((float)MAXFARBANTEIL/(float)AUFLOESUNG);
        mysnprintf(ColorValue,20," %1.2f", Red / AUFLOESUNG);
        swSetText(RedTextEdit, ColorValue);
               mysnprintf(ColorValue,20," %1.2f", Green / AUFLOESUNG);
        swSetText(GreenTextEdit, ColorValue);
               mysnprintf(ColorValue,20," %1.2f", Blue/ AUFLOESUNG);
        swSetText(BlueTextEdit, ColorValue);
}

/**
 * SetTextValue -- Setzt ein Textfeld eines ScrollBar in das jeweilige Textfeld ein
 * 
 * value        Wert fuer die jeweiligen Farbsetztung
 * 
 * textedit        um den Wert in das richtige Textfeld einzusetzen 
 */
void SetTextValue(float value, SWND textedit)
{
        char ColorValue[20];
        
              mysnprintf(ColorValue,20," %1.2f",value / AUFLOESUNG);
        swSetText(textedit, ColorValue);
}

/**
 * ClearValueBarCursor -- Loeschen des Farbbalken Cursor
 *  
 * y                Y-Position des Farbbalken Cursor
 */
void ClearValueBarCursor(int y)
{
        SDC                dc = swCreateDC(canvas);
        int x_again = VALUEBARWIDTH, i, ColorBar=0, x;
        struct window_size window;
        
        GetPositions(&window, false);

         x = window.x0 + (int) window.Radius + 30;                                
        x_again = VALUEBARWIDTH;
        
        ColorBar = GetBarValue(y);
        
        swSetFGColor(dc, ColorBar);
        
        for(i=x;i<=x+x_again;i++)
          swDrawPoint(dc, i , y);

        swDestroyDC(dc);
}

/**
 * DrawValueBarCursor -- Setzen des Farbbalken Cursor
 * 
 * y                Y-Position des Farbbalken Cursor
 *  
 * Color        Farbe des Farbbalken Cursor der in dem er wieder gesetzt werden soll
 */
void DrawValueBarCursor(int y, int Color)
{
        SDC                dc = swCreateDC(canvas);
        int x_again = VALUEBARWIDTH, i, ColorBar=0, x, ColorTemp, Red, Green, Blue;
        struct window_size window;
        float win;
        
        GetPositions(&window, false);

              ColorTemp = SetGetColorPoint(0, false);
        Red = (ColorTemp & 0xff);
        Green = (ColorTemp & 0xff00) >> 8;
        Blue = (ColorTemp & 0xff0000) >> 16;
        

        ColorBar = Color ^ 0xffffff; 

         x = window.x0 + (int) window.Radius + 30;                                
        x_again = VALUEBARWIDTH;
        
        swSetFGColor(dc, ColorBar);
        
        for(i=x;i<=x+x_again;i++)
          swDrawPoint(dc, i , y);
        
        win = (float) y;
        
        win -= ((float)window.y0 - window.Radius);
        
        win /= (2 * window.Radius);
        win *= AUFLOESUNG;
        
        y = Round_float_to_int(win);
        

        swDestroyDC(dc);
}

/**
 * SetBarValueCursor -- Ermoeglicht eine Animation des Farbbalken Cursor
 * 
 * y                neue Y-Position
 * 
 * Color        neue Farbe in der Farbbalken Cursor erstellt werden soll
 */
void SetBarValueCursor(int y, int Color)
{
        static int yo=10;
        struct window_size window;

        GetPositions(&window, false);
        
        if(y!=yo)
        {
          ClearValueBarCursor(yo);
          DrawValueBarCursor(y, Color);
          yo = y;
        }

}

/**
 * RedTextEditInput -- Callback fuer den Textfeld des Rotanteils. Liest erst den eingegebenen Text ein 
 *                       und wandelt den Text in ein double um. Danach liest diese Funktion die aktuelle
 *                       Farbe ein, holt sich daraus den Rotanteil. Prueft nun erstmal ob der eingegebene
 *                       Text in dem gueltigen Eingabe Bereich ist. Wenn es passt dann gibt er den neuen
 *                       Rotanteil als Text aus in dem RotTextFeld(RedTextEdit), nun wird der RedScrollBar
 *                       auf dem Rotanteil gesetzt und die geaenderte Farbe wird gesetzt in der 
 *                       SetGetColorPoint Funktion, die dafuer geschrieben wurde. Die ColorPalette wird
 *                       gesetzt und der Farbbalken wird auch gesetzt und noch die Farbewird im Farbkreis
 *                       ueberprueft. Wenn der eingegebene Farbanteil nicht in dem Eingabe Bereich liegt,
 *                       wird der alte Farbanteil in dem entsprechenden TextFenster geschrieben.
 */
static void
RedTextEditInput(void *data, void *command)
{
        char TextInput[10];
        float RedValue, RedColorTemp, Red;
        double Value; 
        int Color;
        bool Valuemerk;
        
        swGetText(RedTextEdit, TextInput, 10);
        Value = atof(TextInput);

        RedValue = (float) Value * AUFLOESUNG;
               Color = SetGetColorPoint(5, false);
        Red = ((float) (Color & 0xff)) / ((float)MAXFARBANTEIL/(float)AUFLOESUNG);
              Color = (Color & 0xff) ^ Color;
        RedColorTemp = (float) Color;
              RedColorTemp += ((RedValue) * ((float)MAXFARBANTEIL/(float)AUFLOESUNG));
        Color += (int) RedColorTemp;

        if((RedValue<=AUFLOESUNG)&&(RedValue>=0.0))        
        {
             SetTextValue(RedValue, RedTextEdit);
            swScrollBarSetValue(RedScrollBar, (int) RedValue);
          SetGetColorPoint(Color, true);
                 SetColorPalette(Color);
          SetBarValue();
          GetCircleCursor(Color);            
        }else
        {
             SetTextValue(Red, RedTextEdit);
        }
        

}

/**
 * GreenTextEditInput -- Callback fuer den Textfeld des Gruenanteils. Liest erst den eingegebenen Text ein 
 *                         und wandelt den Text in ein double um. Danach liest diese Funktion die aktuelle
 *                         Farbe ein, holt sich daraus den Gruenanteil. Prueft nun erstmal ob der eingegebene
 *                         Text in dem gueltigen Eingabe Bereich ist. Wenn es passt dann gibt er den neuen
 *                         Gruenanteil als Text aus in dem GruenTextFeld(GreenTextEdit), nun wird der GreenScrollBar
 *                         auf dem Gruenanteil gesetzt und die geaenderte Farbe wird gesetzt in der 
 *                         SetGetColorPoint Funktion, die dafuer geschrieben wurde. Die ColorPalette wird
 *                         gesetzt und der Farbbalken wird auch gesetzt und noch die Farbe wird im Farbkreis
 *                         ueberprueft. Wenn der eingegebene Farbanteil nicht in dem Eingabe Bereich liegt,
 *                         wird der alte Farbanteil in dem entsprechenden TextFenster geschrieben.
 */
static void
GreenTextEditInput(void *data, void *command)
{
        char TextInput[10];
        float GreenValue, GreenColorTemp, Green;
        double Value; 
        int Color, GreenColor;
        bool Valuemerk;
        
        swGetText(GreenTextEdit, TextInput, 10);
        Value = atof(TextInput);
        GreenValue = (float) Value * AUFLOESUNG;
                
              Color = SetGetColorPoint(0, false);
          Green = ((float) ((Color & 0xff00) >> 8)) / ((float)MAXFARBANTEIL/(float)AUFLOESUNG);
            Color = (Color & 0xff00) ^ Color;
              GreenColorTemp = GreenValue * ((float)MAXFARBANTEIL/(float)AUFLOESUNG);
        GreenColor = (int) GreenColorTemp;
        Color += (GreenColor << 8);     
        
        if((GreenValue<=AUFLOESUNG)&&(GreenValue>=0.0))        
        {
             SetTextValue(GreenValue, GreenTextEdit);
            swScrollBarSetValue(GreenScrollBar, (int) GreenValue);
          SetGetColorPoint(Color, true);
                 SetColorPalette(Color);
          SetBarValue();
          GetCircleCursor(Color);            
        }else
        {
            SetTextValue(Green, GreenTextEdit);
        }
}

/**
 * BlueTextEditInput -- Callback fuer den Textfeld des Blauanteils. Liest erst den eingegebenen Text ein 
 *                       und wandelt den Text in ein double um. Danach liest diese Funktion die aktuelle
 *                       Farbe ein, holt sich daraus den Blauanteil. Prueft nun erstmal ob der eingegebene
 *                       Text in dem gueltigen Eingabe Bereich ist. Wenn es passt dann gibt er den neuen
 *                       Blauanteil als Text aus in dem BlauTextFeld(BlueTextEdit), nun wird der BlueScrollBar
 *                       auf dem Blauanteil gesetzt und die geaenderte Farbe wird gesetzt in der 
 *                       SetGetColorPoint Funktion, die dafuer geschrieben wurde. Die ColorPalette wird
 *                       gesetzt und der Farbbalken wird auch gesetzt und noch die Farbewird im Farbkreis
 *                       ueberprueft. Wenn der eingegebene Farbanteil nicht in dem Eingabe Bereich liegt,
 *                       wird der alte Farbanteil in dem entsprechenden TextFenster geschrieben.
 */
static void
BlueTextEditInput(void *data, void *command)
{
        char TextInput[10];
        float BlueValue, BlueColorTemp, Blue;
        double Value; 
        int Color, BlueColor;
        bool Valuemerk;
        
        swGetText(BlueTextEdit, TextInput, 10);
        Value = atof(TextInput);
        BlueValue = Value * AUFLOESUNG;
                
              Color = SetGetColorPoint(0, false);
          Blue = ((float) ((Color & 0xff0000) >> 16)) / ((float)MAXFARBANTEIL/(float)AUFLOESUNG);
              Color = (Color & 0xff0000) ^ Color;
              BlueColorTemp = BlueValue * ((float)MAXFARBANTEIL/(float)AUFLOESUNG);
        BlueColor = (int) BlueColorTemp;

        Color += (BlueColor << 16);     

        
        if((BlueValue<=AUFLOESUNG)&&(BlueValue>=0.0))        
        {
             SetTextValue(BlueValue, BlueTextEdit);
            swScrollBarSetValue(BlueScrollBar, (int) BlueValue);
          SetGetColorPoint(Color, true);
                 SetColorPalette(Color);
          SetBarValue();
          GetCircleCursor(Color);            
        }else
        {
            SetTextValue(Blue, BlueTextEdit);
        }
        
}

/**
 * CloseButtonCallback -- ein Callback fuer den Close Button, wenn er gedrueckt wurde, wird die ganze
 *                          Farbfunktion abgebrochen und die alte Farbe an dune zurueck gegeben.
 *
 * ACHTUNG: diese Funktion ist nicht fertig gestellt und bedarf fuer die einbindung in dune noch einige 
 *                            Schritte
 */
void
CloseButtonCallback(void *data, void *id)
{
        float RedValue, GreenValue, BlueValue;

        OldColor(&RedValue, &GreenValue, &BlueValue, false);
        
        // Alte Farbwwerte ausgeben, weil man hier mit das Programm unterbricht
           printf("Red: %1.2f  ,Green:  %1.2f   ,Blue: %1.2f\n", RedValue, GreenValue, BlueValue);

        swExit();        
}

/**
 * OkayButtonCallback -- ein Callback fuer den Okay Button, wenn er gedrueckt wurde, wird die ganze
 *                          Farbfunktion unterbrochen und der neue also die ausgewaehlte Farbe an dune 
 *                                                        zurueck gegeben.
 *
 * ACHTUNG: diese Funktion ist nicht fertig gestellt und bedarf fuer die einbindung in dune noch einige 
 *                            Schritte
 */
void
OkeyButtonCallback(void *data, void *id)
{
        float RedValue, GreenValue, BlueValue;
        int Color;

               Color = SetGetColorPoint(5, false);
        RedValue = ((float) (Color & 0xff)) / (float)MAXFARBANTEIL;
          GreenValue = ((float) ((Color & 0xff00) >> 8)) / (float)MAXFARBANTEIL;
          BlueValue = ((float) ((Color & 0xff0000) >> 16)) / (float)MAXFARBANTEIL;
        

        // Neue Farbwwerte ausgeben, weil man hier mit diesem Programm eine neue Farbe ausgewaehlt hat
           printf("Red: %1.2f  ,Green:  %1.2f   ,Blue: %1.2f\n", RedValue, GreenValue, BlueValue);

        swExit();        
}

/**
 * SetValue_ScrollBar -- Setzt alle Farbwerte in den Scrollbars ein, errechnet vorher die aktuelle Farbe
 *                                                dafuer aus.
 */
void SetValue_ScrollBar(int Color)
{
        int Red, Green, Blue;
        float ColorTemp;
                
        Red = (Color & 0xff);
            Green = (Color >> 8)  & 0xff;
            Blue = (Color >> 16)  & 0xff;        
        
        ColorTemp = (float) Red;
        ColorTemp /= ((float)MAXFARBANTEIL/(float)AUFLOESUNG);
        Red = (int) ColorTemp;
        ColorTemp = (float) Green;
        ColorTemp /= ((float)MAXFARBANTEIL/(float)AUFLOESUNG);
        Green = (int) ColorTemp;
        ColorTemp = (float) Blue;
        ColorTemp /= ((float)MAXFARBANTEIL/(float)AUFLOESUNG);
        Blue = (int) ColorTemp;

        swScrollBarSetValue(RedScrollBar, Red);  
        swScrollBarSetValue(GreenScrollBar, Green);  
        swScrollBarSetValue(BlueScrollBar, Blue);  
    
}

/**
 * RedScrollCallback -- 
 */
void
RedScrollCallback(void *data, int type, int value)
{
              SDC                dc = swCreateDC(canvas);
              int i, k;
              int Color;
              float value_float, ColorTemp;
        bool Valuemerk;

              SFONT          font = swFindFont("helvetica", SW_PLAIN, 14);

              Color = SetGetColorPoint(5, false);
              ColorTemp = (Color & 0xff) ^ Color;
              ColorTemp += (float) (value * ((float)MAXFARBANTEIL/(float)AUFLOESUNG));
        Color = (int) ColorTemp;
              SetGetColorPoint(Color, true);
              SetColorPalette(Color);
        
        value_float = (float) value;

              SetTextValue(value_float, RedTextEdit);
        SetBarValue();
        GetCircleCursor(Color);            

              swDestroyDC(dc);

}


void
GreenScrollCallback(void *data, int type, int value)
{  
              SDC                dc = swCreateDC(canvas);
              int Color, GreenColor;
              float value_float, GreenColorTemp;
        bool Valuemerk;

              SFONT          font = swFindFont("helvetica", SW_PLAIN, 14);

              Color = SetGetColorPoint(0, false);
              Color = (Color & 0xff00) ^ Color;
              GreenColorTemp = (float) value;
        GreenColorTemp *= ((float)MAXFARBANTEIL/(float)AUFLOESUNG);
              GreenColor = (int) GreenColorTemp;
        Color += (GreenColor << 8);     
              SetGetColorPoint(Color, true);
        SetColorPalette(Color);

        value_float = (float) value;
        
        SetTextValue(value_float, GreenTextEdit);
        SetBarValue();
        GetCircleCursor(Color);            
       
        swDestroyDC(dc);        
}

void
BlueScrollCallback(void *data, int type, int value)
{

              SDC                dc = swCreateDC(canvas);
              int Color, BlueColor;
              float value_float, BlueColorTemp;
        bool Valuemerk;

              SFONT          font = swFindFont("helvetica", SW_PLAIN, 14);

              Color = SetGetColorPoint(5, false);
              Color = (Color & 0xff0000) ^ Color;
        BlueColorTemp = (float) value;
              BlueColorTemp *= ((float)MAXFARBANTEIL/(float)AUFLOESUNG);
        BlueColor = (int) BlueColorTemp;
        Color += (BlueColor << 16);
              SetGetColorPoint(Color, true);
        SetColorPalette(Color);
        
        value_float = (float) value;

        SetTextValue(value_float, BlueTextEdit);
        SetBarValue();
        GetCircleCursor(Color);            
       
        swDestroyDC(dc);        


}

void SetCircle()
{
        SDC        dc = swCreateDC(canvas);
        struct window_size window;
            struct Colors colors = {0,0,0};
        int Color, i, k, x0, y0, again, Radius;

        GetPositions(&window, false);
        
        Radius = (int) window.Radius;
          x0 = window.x0 - Radius;                                
        y0 = window.y0 - Radius;        
        again = 2 * Radius;

        for(i=y0;i<=again+y0;i++)
        {
          for(k=x0;k<=again+x0;k++)
          {
             Color = GetColorCircle(k, i, &colors);
             if (Color!=-1)
             {
               swSetFGColor(dc, Color);
               swDrawPoint(dc, k , i);
             }  
          }
        }

        swDestroyDC(dc);


}

static void
mouseCallback(void *data, int x, int y, int modifiers)
{
        int Color, ColorTemp, x0, y0, x3 , y3;
        struct window_size window;
            struct Colors colors = {0,0,0};
        float r, r2, x1, y1, x2, y2;
        static bool CircleMerk=false, ValueBarMerk=false;
        int x_again, y_again;



        if(drawing) 
        {
          SDC dc = swCreateDC(canvas);
          GetPositions(&window, false);
          x0 = x;
          y0 = y;

            x3 = x - window.x0;                                
          y3 = y - window.y0;        
                                  
        
             r2 = (x3 * x3) + (y3 * y3);
             r = sqrt(r2);                                        // r-achse berechnen (Groesse des Kreises)

          if(!ValueBarMerk)
            if((r<=window.Radius)||(CircleMerk))                // Pruefen ob die Koordinate im Kreis ist
          {
            CircleMerk = true;
            if(r<=window.Radius)
            {
              ColorTemp = GetColorCircle(x0, y0, &colors);
              if(ColorTemp!=-1)
                Color = ColorTemp;
              SetGetColorPoint(Color, true);
              SetValue_ScrollBar(Color);
              SetTextColorsValues(Color);
              SetCircleCursor(x3 + window.x0, y3 + window.y0);
            } else
            {
              x1 = (float) x0 - (float) window.x0;
              y1 = (float) y0 - (float) window.y0;
              x2 = window.x0 + ((x1 * window.Radius) / (sqrt((x1 * x1) + (y1 * y1))));
              y2 = window.y0 + ((y1 * window.Radius) / (sqrt((x1 * x1) + (y1 * y1))));
              SetCircleCursor((int) x2, (int) y2);
              ColorTemp = GetColorCircle((int) x2, (int) y2, &colors);
              if(ColorTemp!=-1)
                Color = ColorTemp;
              SetGetColorPoint(Color, true);
              SetValue_ScrollBar(Color);
              SetTextColorsValues(Color);
            } 
          }
          x0 = window.x0 + (int) window.Radius + 30;                                
          y0 = window.y0 - (int) window.Radius;        
          x_again = VALUEBARWIDTH;
          y_again = 2 * (int) window.Radius;

          if(!CircleMerk)
          if(((y<=y_again+y0)&&(y>y0))&&((x<=x_again+x0)&&(x>=x0)) ||
             (ValueBarMerk)&&((y<=y_again+y0)&&(y>y0)))
            {        
              ColorTemp = GetBarValue(y);
              if(ColorTemp!=-1)
                Color = ColorTemp;
              ValueBarMerk = true;
              SetValue_ScrollBar(Color);
              SetTextColorsValues(Color);
               SetBarValueCursor(y, Color);
                     SetColorPalette(Color);
            }else
            {
            }
          swDestroyDC(dc);
        }else
        {
          if(CircleMerk)
            SetBarValue();
          CircleMerk = false;
          ValueBarMerk = false;
        }        
}


static void
keyCallback(void *data, int key, int state, int x, int y, int modifiers)
{

       if (key == SW_MOUSE1) 
        {
          drawing = state;
          mouseCallback(data, x, y, modifiers);
            }

}



static void
resizeCallback(void *data, int x, int y)
{
        SDC        dc = swCreateDC(canvas);

        int Color;
            struct Colors colors = {0,0,0};
        struct window_size window;
        
        swSetFGColor(dc, swGetWindowColor(canvas, SW_COLOR_WINDOW_BG));
        swFillRect(dc, 0, 0, x, y);

        window.x_window = x;
        window.y_window = y;

        GetPositions(&window, true);

        swDestroyDC(dc);
}


static void
exposeCallback(void *data, int x, int y, int width, int height)
{
        SDC        dc = swCreateDC(canvas);
        int Color, x0, y0;
            struct Colors colors = {0,0,0};


        swSetFGColor(dc, swGetWindowColor(canvas, SW_COLOR_WINDOW_BG));
        swFillRect(dc, x, y, width, height);
        
        Color = SetGetColorPoint(0, false);
        SetGetColorPoint(Color, true);
        SetValue_ScrollBar(Color);
        SetTextColorsValues(Color);
        SetColorPalette(Color);
        SetCircle();
        SetBarValue();
        
        swDestroyDC(dc);
}

void ColorPaletteCircle(float *Red, float *Green, float *Blue)
{

    SWND                window;

    int ScrollBarWidth = 200;
    int ScrollBarHeight = 15;
    int TextEditWidth = 30;
    int TextEditHeight = 15;
    int ButtonWidth = 80;
    
    int Color, ColorTemp;
    
    float RedValue, GreenValue, BlueValue;

    RedValue = *Red;
    GreenValue = *Green;
    BlueValue = *Blue;

    OldColor(&RedValue, &GreenValue, &BlueValue, true);
    
    Color = Round_float_to_int((RedValue) * ((float)MAXFARBANTEIL));
    ColorTemp = Round_float_to_int((GreenValue) * ((float)MAXFARBANTEIL));
    Color += ColorTemp << 8;
    ColorTemp = Round_float_to_int((BlueValue) * ((float)MAXFARBANTEIL));
    Color += ColorTemp << 16;
    
    SetGetColorPoint(Color, true);
    
    window = swCreateMainWindow("Palettes", 300, 200, X_WINDOWSIZE, Y_WINDOWSIZE);  
    // ("Name des Fensters", X-Postition, Y-Position, X-Size, Y-Size);

    canvas = swCreateCanvas("", 0, 0, 800, 800, window);
    // ("", X-Begin of draw, Y-Begin of draw, X-Size of draw, Y-Size of draw
    //       ,window);


    CloseButton = swCreateButton("Close", 180,  Y_ScrollBar_TextEdit + 3* Y_ABSTAND, ButtonWidth, 20, canvas);
    OkeyButton = swCreateButton("OK", 20,  Y_ScrollBar_TextEdit + 3* Y_ABSTAND, ButtonWidth, 20, canvas);

    RedScrollBar = swCreateScrollBar(SW_HORIZONTAL, X_ScrollBar, Y_ScrollBar_TextEdit, ScrollBarWidth, 
                                         ScrollBarHeight, 0, AUFLOESUNG + ScrollBarVisit, 0, ScrollBarVisit,
                                     canvas);
    GreenScrollBar = swCreateScrollBar(SW_HORIZONTAL, X_ScrollBar, Y_ScrollBar_TextEdit + Y_ABSTAND, 
                                           ScrollBarWidth, ScrollBarHeight, 0, AUFLOESUNG + ScrollBarVisit,
                                       0, ScrollBarVisit, canvas);

    BlueScrollBar = swCreateScrollBar(SW_HORIZONTAL, X_ScrollBar, Y_ScrollBar_TextEdit + 2 * Y_ABSTAND, 
                                          ScrollBarWidth, ScrollBarHeight, 0, AUFLOESUNG + ScrollBarVisit,
                                      0, ScrollBarVisit, canvas);


    RedTextEdit = swCreateTextEdit(SW_SINGLE_LINE,
                               X_TEXTEDIT, Y_ScrollBar_TextEdit, TextEditWidth, TextEditHeight, canvas);      
    GreenTextEdit = swCreateTextEdit(SW_SINGLE_LINE,
                               X_TEXTEDIT, Y_ScrollBar_TextEdit + Y_ABSTAND, TextEditWidth, TextEditHeight, canvas);      
    BlueTextEdit = swCreateTextEdit(SW_SINGLE_LINE,
                               X_TEXTEDIT, Y_ScrollBar_TextEdit + 2* Y_ABSTAND, TextEditWidth, TextEditHeight, canvas);      
                           

    swTextEditSetSelection(RedTextEdit, 1, 2);
    swTextEditSetSelection(GreenTextEdit, 1, 4);
    swTextEditSetSelection(BlueTextEdit, 1, 4);

    swSetCommandCallback(RedTextEdit, RedTextEditInput);        
    swSetCommandCallback(GreenTextEdit, GreenTextEditInput);        
    swSetCommandCallback(BlueTextEdit, BlueTextEditInput);        
    swSetExposeCallback(canvas, exposeCallback);
    swSetKeyCallback(canvas, keyCallback);
    swSetCommandCallback(CloseButton, CloseButtonCallback);
    swSetCommandCallback(OkeyButton, OkeyButtonCallback);
 
    swSetScrollBarCallback(RedScrollBar, RedScrollCallback);
    swSetScrollBarCallback(GreenScrollBar, GreenScrollCallback);
    swSetScrollBarCallback(BlueScrollBar, BlueScrollCallback);
    swSetMouseCallback(canvas, mouseCallback);
    swSetResizeCallback(window, resizeCallback);
    swShowWindow(window);
    swMainLoop();
    swDestroyWindow(window);

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

        float Red=0.40, Green=0.50, Blue=0.60;

        ColorPaletteCircle(&Red, &Green, &Blue);

        return 0;
}
