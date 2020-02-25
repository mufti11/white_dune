/*
 * ColorCircle.cpp
 *
 * Copyright (C) 2002 Christian Hanisch, 2020 J. "MUFTI" Scheurich
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

#include "math.h"

#include "ColorCircle.h"
#include "NodeColor.h"
#include "NodeColorRGBA.h"
#include "NodeIndexedFaceSet.h"
#include "NodeElevationGrid.h"
#include "NodeGeoElevationGrid.h"
#include "MFColor.h"
#include "MFColorRGBA.h"
#include "MyMesh.h"

int ColorCircle::SetGetColorPoint(int Color, bool Set)
{
    static int ColorAll=0;

    if (Set)
        ColorAll = Color;
    return ColorAll;
}

void ColorCircle::OldColor(float *Red, float *Green, float *Blue, bool oldcolor)
{
    static float RedValue, GreenValue, BlueValue;

    if (oldcolor) {
        RedValue = *Red;
        GreenValue = *Green;
        BlueValue = *Blue;
    } else {
        *Red = RedValue;
        *Green = GreenValue;
        *Blue = BlueValue;
    }
}

void ColorCircle::SetScrollBars(struct ScrollBars scroll, 
                                       struct TextEdits textedit, 
                                       struct Buttons button)
{
// Hier kann die ScrollBar Groesse und die Button Groesse mit dem Befehl 
// extern void swSetSize(SWND wnd, int width, int height); 
// gesetzt werden.

    swSetPosition(RedScrollBar, scroll.red.x, scroll.red.y);
    swSetPosition(GreenScrollBar, scroll.green.x, scroll.green.y);
    swSetPosition(BlueScrollBar, scroll.blue.x, scroll.blue.y);
    swSetPosition(AlphaScrollBar, scroll.alpha.x, scroll.alpha.y);

    swSetPosition(RedTextEdit, textedit.red.x, textedit.red.y);
    swSetPosition(GreenTextEdit, textedit.green.x, textedit.green.y);
    swSetPosition(BlueTextEdit, textedit.blue.x, textedit.blue.y);
    swSetPosition(AlphaTextEdit, textedit.alpha.x, textedit.alpha.y);

    swSetPosition(OkeyButton, button.okay.x, button.okay.y);
    swSetPosition(CloseButton, button.close.x, button.close.y);
}   

#define X_WINDOWSIZE 300
#define Y_WINDOWSIZE 240

#define X0_POINT -20
#define Y0_POINT 200

#define Y_ABSTAND 20
#define LINKS_FENSTER 60
#define OBEN_FENSTER 90
#define RECHTS_FENSTER 90
#define UNTEN_FENSTER 100 

void ColorCircle::GetPositions(struct window_size* windowback, bool resize)
{
    static struct window_size window = {X0_POINT, Y0_POINT, 
                                         X_WINDOWSIZE, Y_WINDOWSIZE, 0};
    struct ScrollBars scrollbar;
    struct TextEdits textedit;
    struct Buttons button;

    if (resize) {
        window.x_window = windowback->x_window;
        window.y_window = windowback->y_window;
        button.okay.x = (window.x_window / 4) - 60; 
        button.okay.y = window.y_window - LINKS_FENSTER; 
        button.close.x = (window.x_window / 2) + 30; 
        button.close.y = window.y_window - LINKS_FENSTER;
        scrollbar.red.x = (window.x_window / 4) - LINKS_FENSTER;
        scrollbar.red.y = window.y_window  - OBEN_FENSTER - 3*Y_ABSTAND;
        scrollbar.green.x = (window.x_window / 4) - LINKS_FENSTER;
        scrollbar.green.y = window.y_window  - OBEN_FENSTER - 2*Y_ABSTAND;
        scrollbar.blue.x = (window.x_window / 4) - LINKS_FENSTER;
        scrollbar.blue.y = window.y_window - OBEN_FENSTER - Y_ABSTAND;
        scrollbar.alpha.x = (window.x_window / 4) - LINKS_FENSTER;
        scrollbar.alpha.y = window.y_window - OBEN_FENSTER;
        textedit.red.x = (window.x_window / 2) + RECHTS_FENSTER;
        textedit.red.y = window.y_window - UNTEN_FENSTER;
        textedit.green.x = (window.x_window / 2) + RECHTS_FENSTER;
        textedit.green.y = window.y_window  - UNTEN_FENSTER + Y_ABSTAND;
        textedit.blue.x = (window.x_window / 2) + RECHTS_FENSTER;
        textedit.blue.y = window.y_window - UNTEN_FENSTER + 2*Y_ABSTAND;
        textedit.alpha.x = (window.x_window / 2) + RECHTS_FENSTER;
        textedit.alpha.y = window.y_window - UNTEN_FENSTER + 3*Y_ABSTAND;
        SetScrollBars(scrollbar, textedit, button);
    }

    window.x0 = (window.x_window / 2) - 30;

    if (window.x_window>window.y_window*0.4) {
        windowback->Radius = ((window.x_window - window.x0 - OBEN_FENSTER)/ 2);
    } else {
        windowback->Radius = ((window.y_window * 0.4) / 2);
    }
 
    windowback->y0 = (int) windowback->Radius + UPPER_LIMIT;
    windowback->x0 = (window.x_window / 2) + (int) windowback->Radius - 60;

    windowback->x_window = window.x_window;
    windowback->y_window = window.y_window;
}

#ifndef HAVE_RINT
int ColorCircle::Round_float_to_int(float number)
{
    if (number - floor(number) >= 0.5)
        return (int) ceil(number);
    else           
       return (int) floor(number);
}
#endif

int ColorCircle::GetColorFloatToInt(float red, float green, float blue)
{  
    int Color;
    int red_int, green_int, blue_int;

    red_int = Round_float_to_int(red * MAXFARB);
    green_int = Round_float_to_int(green * MAXFARB);
    blue_int = Round_float_to_int(blue * MAXFARB);

    Color = 0;
    Color += (red_int & 0xff);
    Color += (green_int & 0xff) << 8;
    Color += (blue_int & 0xff) << 16;
    return Color;
} 

int ColorCircle::GetBarValue(int y)
{
    struct window_size window;
    int Color, Red, Green, Blue;
    float RedColor=0, GreenColor=0, BlueColor=0;
    int y0, y_again, y1;

    GetPositions(&window, false);

    Color = SetGetColorPoint(0, false);
    Red = (Color & 0xff);
    Green = (Color & 0xff00) >> 8;
    Blue = (Color & 0xff0000) >> 16;

    y0 = window.y0 - (int) window.Radius;
    y1 = y - y0;
    y_again = 2 * (int) window.Radius;
        
    if (y1 != 0) { 
        RedColor =  (1 / ((window.Radius * 2) / y1) * (float) Red);
        GreenColor =  (1 / ((window.Radius * 2) / y1) * (float) Green);
        BlueColor =  (1 / ((window.Radius * 2) / y1) * (float) Blue);
        if (Color==0) { 
            RedColor =  (1 / ((window.Radius * 2) / y1) * MAXFARB);
            GreenColor =  (1 / ((window.Radius * 2) / y1) * MAXFARB);
            BlueColor =  (1 / ((window.Radius * 2) / y1) * MAXFARB);
        }
    }

    if ((y <= y_again + y0) && (y >= y0)) {
        Red = (int) RedColor;
        Green = (int) GreenColor;
        Blue = (int) BlueColor;
        if (Red >= MAXFARB + 1)
            Red = MAXFARB;
        if (Green >= MAXFARB + 1)
            Green = MAXFARB;
        if (Blue >= MAXFARB + 1)
            Blue = MAXFARB;
        Color = (Red & 0xff);
        Color += (Green & 0xff) << 8;
        Color += (Blue & 0xff) << 16;

        return Color;  
    } else 
       return -1;
}

void ColorCircle::SetBarValue()
{
    struct window_size window;
    int x0, y0, y_again, i, Color;

    GetPositions(&window, false);
    x0 = window.x0 + (int) window.Radius + 30;
    y0 = window.y0 - (int) window.Radius;
    y_again = 2 * (int) window.Radius;

    for (i = y0; i <= y_again + y0; i++) {
        Color = GetBarValue(i);
        swSetFGColor(m_dc, Color);
        swDrawLine(m_dc, x0, i, x0+VALUEBARWIDTH, i);
    }
}

int ColorCircle::GetColorCircle(int x, int y, struct Colors* colors)
{
    float red = 0, green = 0, blue = 0;
    int Color = 0;
    float alpha;
    float r, r2, R;
    struct window_size window;

    GetPositions(&window, false);

    x = x - window.x0;
    y = y - window.y0;
    R = window.Radius;

    // Winkel berechnen
    alpha = (atan2((float)y, (float)x) * 180.0 / M_PI) + 180;  
   
    // r-achse berechnen (Groesse des Kreises)
    r2 = (x * x) + (y * y);
    r = sqrt(r2);

    // Pruefen ob die Koordinate im Kreis ist
    if (r <= R) {
        float temp = 1 - (r / R);
        if (alpha > BLUEWINKEL) { 
            if (alpha > MAGENTAWINKEL) {
                blue = 1 - alpha / (MAXWINKEL - MAGENTAWINKEL) 
                       + MAGENTAWINKEL / (MAXWINKEL - MAGENTAWINKEL);
                red = 1;
            } else {
                blue = 1;
                red =  alpha / (MAGENTAWINKEL - BLUEWINKEL) 
                        - BLUEWINKEL / (MAGENTAWINKEL - BLUEWINKEL);
            }
            green = temp;
            blue = blue - temp * blue + temp;
            red = red - temp * red + temp;
        } else if (alpha > GREENWINKEL) { // Winkelbereich
            if (alpha > CYANWINKEL) {
                green = 1 - alpha / (BLUEWINKEL - CYANWINKEL) 
                        + CYANWINKEL / (BLUEWINKEL - CYANWINKEL);
                blue = 1;
            } else {
                green = 1;
                blue = alpha / (CYANWINKEL - GREENWINKEL)
                       - GREENWINKEL / (CYANWINKEL - GREENWINKEL);
            }
            red = temp;
            blue = blue - temp * blue + temp;
            green = green - temp * green + temp;
        } else if (alpha > REDWINKEL) { // Winkelbereich
            if (alpha > YELLOWWINKEL) {
                red = 1 - alpha / (GREENWINKEL - YELLOWWINKEL) 
                      + YELLOWWINKEL / ( GREENWINKEL - YELLOWWINKEL);
                green = 1;
            }
            else 
            {
                red = 1;
                green =  alpha / (YELLOWWINKEL - REDWINKEL) 
                         - REDWINKEL / (YELLOWWINKEL - REDWINKEL);
            }
            red= red - temp * red + temp;
            green = green - temp * green + temp;
            blue = temp;
        } 
        Color = 0;
        colors->red = red;
        colors->green = green;
        colors->blue = blue;
        Color = GetColorFloatToInt(red, green, blue);
    } else {
        // 1 - (R / r) *255
        Color = -1;
    }
    // Rueckgabe der Farbe
    return Color;
}

void ColorCircle::SetColorPalette(int Color)
{
    int start_x=20, start_y=UPPER_LIMIT, width_x=50, hight_y=50;

    swSetFGColor(m_dc, Color);
    swFillRect(m_dc, start_x, start_y, width_x, hight_y);
    swSetFGColor(m_dc, 0xffffff);
    swFillRect(m_dc, start_x, start_y+hight_y, width_x, hight_y);

    float Red = ((float) (Color & 0xff)) / (float)MAXFARB;
    float Green = ((float) ((Color & 0xff00) >> 8)) / (float)MAXFARB;
    float Blue = ((float) ((Color & 0xff0000) >> 16)) / (float)MAXFARB;
    setColorsViaFieldUpdate(Red,Green,Blue, m_alpha);
}

void ColorCircle::ClearCircleCursor(int x, int y)
{
    struct Colors colors = {0,0,0};
    int i, k, Color;

    for (i = 0; i <= 8; i++) {
        for (k = 0; k <= 8 - 1; k++)
        {
            Color = GetColorCircle((x + k) - 4, (y + i) - 4, &colors);
            if (Color != -1) {
                Color = GetColorFloatToInt(colors.red, colors.green, 
                                           colors.blue);
                swSetFGColor(m_dc, Color);
                swDrawPoint(m_dc, (x + k) - 4 , (y + i) - 4);
            } else {   
                swSetFGColor(m_dc, swGetWindowColor(canvas, SW_COLOR_WINDOW_BG));
                swDrawPoint(m_dc, (x + k) - 4 , (y + i) - 4);
            }
            Color = GetColorCircle((x + i) - 4, (y + k) - 4, &colors);
            if (Color != -1) {
                swSetFGColor(m_dc, Color);
                swDrawPoint(m_dc, (x + i) - 4 , (y + k) - 4);
            } else {
                swSetFGColor(m_dc, swGetWindowColor(canvas, SW_COLOR_WINDOW_BG));
                swDrawPoint(m_dc, (x + i) - 4 , (y + k) - 4);
            }
        }
    }
    for (i = 8; i >= 0; i--) {
        for (k = 8; k >= 0 + 1; k--)
        {
            Color = GetColorCircle((x + k) - 4, (y + i) - 4, &colors);
            if (Color != -1) {
                swSetFGColor(m_dc, Color);
                swDrawPoint(m_dc, (x + k) - 4 , (y + i) - 4);
            } else {
                swSetFGColor(m_dc, swGetWindowColor(canvas, SW_COLOR_WINDOW_BG));
                swDrawPoint(m_dc, (x + k) - 4 , (y + i) - 4);
            }    
            Color = GetColorCircle((x + i) - 4, (y + k) - 4, &colors);
            if (Color != -1) {
                swSetFGColor(m_dc, Color);
                swDrawPoint(m_dc, (x + i) - 4 , (y + k) - 4);
            } else {
                swSetFGColor(m_dc, swGetWindowColor(canvas, SW_COLOR_WINDOW_BG));
                swDrawPoint(m_dc, (x + i) - 4 , (y + k) - 4);
            }
        }
    }
}

void ColorCircle::DrawCircleCursor(int x, int y)
{
    int i, k;

    swSetFGColor(m_dc, 0);
    for (i = 0; i <= 8; i++) {
        for (k = 0; k <= 8 - 1; k++)
        {
            swDrawPoint(m_dc, (x + k) - 4 , (y + i) - 4);
            swDrawPoint(m_dc, (x + i) - 4 , (y + k) - 4);
        }
    }
    for (i = 8; i >= 0; i--) {
        for (k = 8; k >= 0 + 1; k--)
        {
            swDrawPoint(m_dc, (x + k) - 4 , (y + i) - 4);
            swDrawPoint(m_dc, (x + i) - 4 , (y + k) - 4);
        }
    }
}

void ColorCircle::SetCircleCursor(int x, int y)
{
    static int xo, yo;

    if ((x != xo) || (y != yo)) {
        ClearCircleCursor(xo, yo);
        DrawCircleCursor(x, y);
        xo = x;
        yo = y;
    }
}

void ColorCircle::GetCircleCursor(int ColorNew)
{
    int Color, x, y, i, k, again;
    int red, green, blue, k_merk, i_merk;
    int RedValue, GreenValue, BlueValue;
    struct Colors colors = {0,0,0};
    struct window_size window;
    float minwert=FLT_MAX, absolutwert;

    Color = SetGetColorPoint(5, false);

    RedValue = (Color & 0xff);
    GreenValue = ((Color & 0xff00) >> 8);
    BlueValue = ((Color & 0xff0000) >> 16);

    GetPositions(&window, false);
           
    x = window.x0 - (int) window.Radius;
    y = window.y0 - (int) window.Radius;
    again = 2 * (int) window.Radius;

    i_merk=window.x0;
    k_merk=window.y0;

    for (i = y; i <= again + y; i++) {
        for (k = x; k <= again + x; k++) {
            Color = GetColorCircle(k, i, &colors);
            if (Color != -1) {
                red = Round_float_to_int(colors.red * MAXFARB);
                green = Round_float_to_int(colors.green * MAXFARB);
                blue = Round_float_to_int(colors.blue * MAXFARB);

                if ((RedValue == red) && (GreenValue == green) &&
                    (BlueValue == blue)) {
                    SetCircleCursor(k, i);
                    return;
                } else {
                    absolutwert = (float)fabs((float)(red - RedValue)) + 
                                  (float)fabs((float)(green - GreenValue)) + 
                                  (float)fabs((float)(blue - BlueValue));
                    if (minwert > absolutwert) {
                        minwert = absolutwert;
                        k_merk = k;
                        i_merk = i;
                    }
                }
            }
        }
    }
    SetCircleCursor(k_merk, i_merk);
}

void ColorCircle::SetTextColorsValues(int value)
{
    static float Red = 0, Green = 0, Blue = 0;
    char ColorValue[128];

    SetColorPalette(value);
    Red = (value & 0xff) / ((float)MAXFARB/(float)AUFLOESUNG);
    Green = ((value & 0xff00) >> 8) / ((float)MAXFARB/(float)AUFLOESUNG);
    Blue = ((value & 0xff0000) >> 16) / ((float)MAXFARB/(float)AUFLOESUNG);
    mysnprintf(ColorValue,128," %1.2f", Red / AUFLOESUNG);
    swSetText(RedTextEdit, ColorValue);
    mysnprintf(ColorValue,128," %1.2f", Green / AUFLOESUNG);
    swSetText(GreenTextEdit, ColorValue);
    mysnprintf(ColorValue,128," %1.2f", Blue/ AUFLOESUNG);
    swSetText(BlueTextEdit, ColorValue);
    if (m_alphaEnabled) {
        mysnprintf(ColorValue,128," %1.2f", m_alpha);
        swSetText(AlphaTextEdit, ColorValue);
    }
}

void ColorCircle::SetTextValue(float value, SWND textedit)
{
    char ColorValue[128];
     
    mysnprintf(ColorValue,128," %1.2f",value / AUFLOESUNG);
    swSetText(textedit, ColorValue);
}

void ColorCircle::ClearValueBarCursor(int y)
{
    int ColorBar=0, x;
    struct window_size window;

    GetPositions(&window, false);

    x = window.x0 + (int) window.Radius + 30;

    ColorBar = GetBarValue(y);

    swSetFGColor(m_dc, ColorBar);

    swDrawLine(m_dc, x, y, x+VALUEBARWIDTH, y);
}

void ColorCircle::DrawValueBarCursor(int y, int Color)
{
    int x_again = VALUEBARWIDTH, i, ColorBar=0, x;
    struct window_size window;
    float win;

    GetPositions(&window, false);

    ColorBar = Color ^ 0xffffff; 

    x = window.x0 + (int) window.Radius + 30;
    x_again = VALUEBARWIDTH;

    swSetFGColor(m_dc, ColorBar);

    for (i = x; i <= x + x_again; i++)
        swDrawPoint(m_dc, i , y);

    win = (float) y;
          
    win -= ((float)window.y0 - window.Radius);
          
    win /= (2 * window.Radius);
    win *= AUFLOESUNG;
          
    y = Round_float_to_int(win);
}

void ColorCircle::SetBarValueCursor(int y, int Color)
{
    static int yo=10;
    struct window_size window;

    GetPositions(&window, false);
          
    if (y != yo) {
        ClearValueBarCursor(yo);
        DrawValueBarCursor(y, Color);
        yo = y;
    }
}

static void RedTextEditInputCB(void *data, void *command)
{
    ((ColorCircle *) data)->RedTextEditInput(command);
}

void
ColorCircle::RedTextEditInput(void *command)
{
    char TextInput[10];
    float RedValue, RedColorTemp, Red;
    double Value; 
    int Color;
          
    swGetText(RedTextEdit, TextInput, 10);
    Value = atof(TextInput);

    RedValue = (float) Value * AUFLOESUNG;
    Color = SetGetColorPoint(5, false);
    Red = ((float) (Color & 0xff)) / ((float)MAXFARB / (float)AUFLOESUNG);
    Color = (Color & 0xff) ^ Color;
    RedColorTemp = (float) Color;
    RedColorTemp += ((RedValue) * ((float)MAXFARB/(float)AUFLOESUNG));
    Color += (int) RedColorTemp;

    if ((RedValue <= AUFLOESUNG) && (RedValue >= 0.0)) {
        SetTextValue(RedValue, RedTextEdit);
        swScrollBarSetValue(RedScrollBar, (int) RedValue);
        SetGetColorPoint(Color, true);
        SetColorPalette(Color);
        SetBarValue();
        GetCircleCursor(Color);              
    } else {
        SetTextValue(Red, RedTextEdit);
    }          
}


static void GreenTextEditInputCB(void *data, void *command)
{
    ((ColorCircle *) data)->GreenTextEditInput(command);
}

void
ColorCircle::GreenTextEditInput(void *command)
{
    char TextInput[10];
    float GreenValue, GreenColorTemp, Green;
    double Value; 
    int Color, GreenColor;
    swGetText(GreenTextEdit, TextInput, 10);
    Value = atof(TextInput);
    GreenValue = (float) Value * AUFLOESUNG;
                   
    Color = SetGetColorPoint(0, false);
    Green = ((float) ((Color & 0xff00) >> 8)) / 
                      ((float)MAXFARB / (float)AUFLOESUNG);
    Color = (Color & 0xff00) ^ Color;
    GreenColorTemp = GreenValue * ((float)MAXFARB/(float)AUFLOESUNG);
    GreenColor = (int) GreenColorTemp;
    Color += (GreenColor << 8);     
          
    if ((GreenValue <= AUFLOESUNG) && (GreenValue >= 0.0)) {
        SetTextValue(GreenValue, GreenTextEdit);
        swScrollBarSetValue(GreenScrollBar, (int) GreenValue);
        SetGetColorPoint(Color, true);
        SetColorPalette(Color);
        SetBarValue();
        GetCircleCursor(Color);              
    } else {
        SetTextValue(Green, GreenTextEdit);
    }
}


static void BlueTextEditInputCB(void *data, void *command)
{
    ((ColorCircle *) data)->BlueTextEditInput(command);
}

void
ColorCircle::BlueTextEditInput(void *command)
{
    char TextInput[10];
    float BlueValue, BlueColorTemp, Blue;
    double Value; 
    int Color, BlueColor;
          
    swGetText(BlueTextEdit, TextInput, 10);
    Value = atof(TextInput);
    BlueValue = Value * AUFLOESUNG;
                    
    Color = SetGetColorPoint(0, false);
    Blue = ((float) ((Color & 0xff0000) >> 16)) / 
                     ((float)MAXFARB / (float)AUFLOESUNG);
    Color = (Color & 0xff0000) ^ Color;
    BlueColorTemp = BlueValue * ((float)MAXFARB / (float)AUFLOESUNG);
    BlueColor = (int) BlueColorTemp;

    Color += (BlueColor << 16);     

    if ((BlueValue <= AUFLOESUNG) && (BlueValue >= 0.0)) {
        SetTextValue(BlueValue, BlueTextEdit);
        swScrollBarSetValue(BlueScrollBar, (int) BlueValue);
        SetGetColorPoint(Color, true);
        SetColorPalette(Color);
        SetBarValue();
        GetCircleCursor(Color);              
    } else {
        SetTextValue(Blue, BlueTextEdit);
    }       
}


static void AlphaTextEditInputCB(void *data, void *command)
{
    ((ColorCircle *) data)->AlphaTextEditInput(command);
}

void
ColorCircle::AlphaTextEditInput(void *command)
{
    char TextInput[10];
    float AlphaValue, AlphaColorTemp;
    double Value; 
    int AlphaColor;
          
    swGetText(AlphaTextEdit, TextInput, 10);
    Value = atof(TextInput);
    AlphaValue = Value * AUFLOESUNG;
                    
    AlphaColorTemp = AlphaValue * ((float)MAXFARB / (float)AUFLOESUNG);
    AlphaColor = (int) AlphaColorTemp;

    if ((AlphaValue <= AUFLOESUNG) && (AlphaValue >= 0.0)) {
        SetTextValue(AlphaValue, AlphaTextEdit);
        swScrollBarSetValue(AlphaScrollBar, (int) AlphaValue);

        int Color = AlphaColor;
        SetGetColorPoint(Color, true);
        SetColorPalette(Color);
    }
}

static void CloseButtonCB(void *data, void *id)
{
    ((ColorCircle *) data)->CloseButtonCallback(id);
}

void
ColorCircle::CloseButtonCallback(void *id)
{
    float RedValue, GreenValue, BlueValue;

    OldColor(&RedValue, &GreenValue, &BlueValue, false);
          
    setColorsViaFieldUpdate(RedValue, GreenValue, BlueValue, m_alpha);

    m_scene->UpdateViews(this, UPDATE_CLOSE_COLOR_CIRCLE);
}

static void OkeyButtonCB(void *data, void *id)
{
    ((ColorCircle *) data)->OkeyButtonCallback(id);
}

void
ColorCircle::OkeyButtonCallback(void *id)
{
    m_scene->UpdateViews(this, UPDATE_CLOSE_COLOR_CIRCLE);
}

void ColorCircle::SetValue_ScrollBar(int Color)
{
    int Red, Green, Blue, Alpha;
    float ColorTemp;
                    
    Red = (Color & 0xff);
    Green = (Color >> 8) & 0xff;
    Blue = (Color >> 16) & 0xff;          
          
    ColorTemp = (float) Red;
    ColorTemp /= ((float)MAXFARB/(float)AUFLOESUNG);
    Red = (int) ColorTemp;
    ColorTemp = (float) Green;
    ColorTemp /= ((float)MAXFARB/(float)AUFLOESUNG);
    Green = (int) ColorTemp;
    ColorTemp = (float) Blue;
    ColorTemp /= ((float)MAXFARB/(float)AUFLOESUNG);
    Blue = (int) ColorTemp;
    ColorTemp = (float) m_alpha;
    Alpha = ColorTemp * AUFLOESUNG;

    swScrollBarSetValue(RedScrollBar, Red);  
    swScrollBarSetValue(GreenScrollBar, Green);  
    swScrollBarSetValue(BlueScrollBar, Blue);
    if (m_alphaEnabled)
        swScrollBarSetValue(AlphaScrollBar, Alpha);
}

static void RedScrollCB(void *data, int type, int value)
{
    ((ColorCircle *) data)->RedScrollCallback(type,value);
}

void
ColorCircle::RedScrollCallback(int type, int value)
{
    int Color;
    float value_float, ColorTemp;

    Color = SetGetColorPoint(5, false);
    ColorTemp = (Color & 0xff) ^ Color;
    ColorTemp += (float) (value * ((float)MAXFARB/(float)AUFLOESUNG));
    Color = (int) ColorTemp;
    SetGetColorPoint(Color, true);
    SetColorPalette(Color);
          
    value_float = (float) value;

    SetTextValue(value_float, RedTextEdit);
    SetBarValue();
    GetCircleCursor(Color);              
}


static void GreenScrollCB(void *data, int type, int value)
{
    ((ColorCircle *) data)->GreenScrollCallback(type,value);
}

void
ColorCircle::GreenScrollCallback(int type, int value)
{  
    int Color, GreenColor;
    float value_float, GreenColorTemp;

    Color = SetGetColorPoint(0, false);
    Color = (Color & 0xff00) ^ Color;
    GreenColorTemp = (float) value;
    GreenColorTemp *= ((float)MAXFARB / (float)AUFLOESUNG);
    GreenColor = (int) GreenColorTemp;
    Color += (GreenColor << 8);     
    SetGetColorPoint(Color, true);
    SetColorPalette(Color);

    value_float = (float) value;
          
    SetTextValue(value_float, GreenTextEdit);
    SetBarValue();
    GetCircleCursor(Color);              
}


static void BlueScrollCB(void *data, int type, int value)
{
    ((ColorCircle *) data)->BlueScrollCallback(type,value);
}

void
ColorCircle::BlueScrollCallback(int type, int value)
{
    int Color, BlueColor;
    float value_float, BlueColorTemp;

    Color = SetGetColorPoint(5, false);
    Color = (Color & 0xff0000) ^ Color;
    BlueColorTemp = (float) value;
    BlueColorTemp *= ((float)MAXFARB/(float)AUFLOESUNG);
    BlueColor = (int) BlueColorTemp;
    Color += (BlueColor << 16);
    SetGetColorPoint(Color, true);
    SetColorPalette(Color);
    
    value_float = (float) value;

    SetTextValue(value_float, BlueTextEdit);
    SetBarValue();
    GetCircleCursor(Color);              
}

static void AlphaScrollCB(void *data, int type, int value)
{
    ((ColorCircle *) data)->AlphaScrollCallback(type,value);
}

void
ColorCircle::AlphaScrollCallback(int type, int value)
{
    float value_float;

    value_float = (float) value;

    SetTextValue(value_float, AlphaTextEdit);
    SetBarValue();

    m_alpha = value_float / AUFLOESUNG;

    int Color = SetGetColorPoint(5, false);
    SetGetColorPoint(Color, true);
    SetColorPalette(Color);
}

void ColorCircle::SetCircle() {
    struct window_size window;
    struct Colors colors = {0,0,0};
    int Color, i, k, x0, y0, again, Radius;

    GetPositions(&window, false);
    
    Radius = (int) window.Radius;
    x0 = window.x0 - Radius;                                        
    y0 = window.y0 - Radius;          
    again = 2 * Radius;

    for (i = y0; i <= again + y0; i++) {
        for (k = x0; k <= again + x0; k++) {
            Color = GetColorCircle(k, i, &colors);
            if (Color != -1) {
                swSetFGColor(m_dc, Color);
                swDrawPoint(m_dc, k , i);
            }  
        }
    }
}

static void mouseCB(void *data, int x, int y, int modifiers)
{
    ((ColorCircle *) data)->mouseCallback(x, y, modifiers);
}

void
ColorCircle::mouseCallback(int x, int y, int modifiers)
{
    int Color = 0, ColorTemp, x0, y0, x3 , y3;
    struct window_size window;
    struct Colors colors = {0,0,0};
    float r, r2, x1, y1, x2, y2;
    static bool CircleMerk=false, ValueBarMerk=false;
    int x_again, y_again;

    if (drawing) {
        GetPositions(&window, false);
        x0 = x;
        y0 = y;

        x3 = x - window.x0;                                        
        y3 = y - window.y0;          
                                          
        // r-achse berechnen (Groesse des Kreises)
        r2 = (x3 * x3) + (y3 * y3);
        r = sqrt(r2);                    

        // Pruefen ob die Koordinate im Kreis ist
        if (!ValueBarMerk)
            if ((r <= window.Radius) || (CircleMerk)) {
                CircleMerk = true;
                if (r <= window.Radius) {
                    ColorTemp = GetColorCircle(x0, y0, &colors);
                    if (ColorTemp != -1)
                        Color = ColorTemp;
                    SetGetColorPoint(Color, true);
                    SetValue_ScrollBar(Color);
                    SetTextColorsValues(Color);
                    SetCircleCursor(x3 + window.x0, y3 + window.y0);
                    SetBarValue();
                } else {
                    x1 = (float) x0 - (float) window.x0;
                    y1 = (float) y0 - (float) window.y0;
                    x2 = window.x0 + ((x1 * window.Radius) / 
                                      (sqrt((x1 * x1) + (y1 * y1))));
                    y2 = window.y0 + ((y1 * window.Radius) /
                                      (sqrt((x1 * x1) + (y1 * y1))));
                    SetCircleCursor((int) x2, (int) y2);
                    ColorTemp = GetColorCircle((int) x2, (int) y2, &colors);
                    if (ColorTemp != -1)
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

        if (!CircleMerk)
            if ((((y <= y_again + y0) && (y > y0)) && 
                 ((x <= x_again + x0) && (x >= x0))) ||
                ((ValueBarMerk) && ((y <= y_again + y0) && (y > y0)))) {
                ColorTemp = GetBarValue(y);
                if (ColorTemp != -1)
                    Color = ColorTemp;
                ValueBarMerk = true;
                SetValue_ScrollBar(Color);
                SetTextColorsValues(Color);
                SetBarValueCursor(y, Color);
                SetColorPalette(Color);
            } 
    } else {
        if (CircleMerk)
            SetBarValue();
        CircleMerk = false;
        ValueBarMerk = false;
    }          
}

static void keyCB(void *data, int key, int state, int x, int y, int modifiers)
{
    ((ColorCircle *) data)->keyCallback(key, state, x, y, modifiers);
}

void
ColorCircle::keyCallback(int key, int state, int x, int y, int modifiers)
{
    if (key == SW_MOUSE1) {
        drawing = state;
        mouseCallback(x, y, modifiers);
    }
}

void
ColorCircle::resizeCallback(int x, int y)
{
    struct window_size window;
          
    swSetFGColor(m_dc, swGetWindowColor(canvas, SW_COLOR_WINDOW_BG));
    swFillRect(m_dc, 0, 0, x, y);

    window.x_window = x;
    window.y_window = y;

    GetPositions(&window, true);
}

static void exposeCB(void *data, int x, int y, int width, int height)
{
    ((ColorCircle *) data)->exposeCallback(x, y, width, height);
}

void
ColorCircle::exposeCallback(int x, int y, int width, int height)
{
    int Color;

    swSetFGColor(m_dc, swGetWindowColor(canvas, SW_COLOR_WINDOW_BG));
    swFillRect(m_dc, x, y, width, height);
          
    Color = SetGetColorPoint(0, false);
    SetGetColorPoint(Color, true);
    SetValue_ScrollBar(Color);
    SetTextColorsValues(Color);
    SetColorPalette(Color);
    SetCircle();
    SetBarValue();
}

ColorCircle::ColorCircle(Scene *scene, SWND parent, FieldUpdate* fieldUpdate)
                        : SceneView(scene, parent)
{
    int width, height;

    swGetSize(parent, &width, &height);
    drawing = 0;
    canvas = parent;
    m_fieldUpdate = *fieldUpdate;
    m_dc = NULL;
    m_alpha = 1;
    m_alphaEnabled = false;
    if (m_fieldUpdate.node && m_fieldUpdate.node->getType() == X3D_COLOR_RGBA)
        m_alphaEnabled = true;

    int ScrollBarWidth = 200;
    int ScrollBarHeight = 15;
    int TextEditWidth = 30;
    int TextEditHeight = 15;
    int ButtonWidth = 80;
    
    int Color, ColorTemp;

    swSetClientData(canvas,this);
    swSetExposeCallback(canvas, exposeCB);
    swSetKeyCallback(canvas, keyCB);
    swSetMouseCallback(canvas, mouseCB);

    CloseButton = swCreateButton("Close", 180,  
                                 Y_ScrollBar_TextEdit + 4 * Y_ABSTAND, 
                                 ButtonWidth, 
                                 20, canvas);
    swSetClientData(CloseButton,this);
    swSetCommandCallback(CloseButton, CloseButtonCB);

    OkeyButton = swCreateButton("OK", 20,  
                                 Y_ScrollBar_TextEdit + 
                                 4 * Y_ABSTAND, 
                                 ButtonWidth, 20, canvas);
    swSetClientData(OkeyButton,this);
    swSetCommandCallback(OkeyButton, OkeyButtonCB);

    RedScrollBar = swCreateScrollBar(SW_HORIZONTAL, 
                                     X_ScrollBar, 
                                     Y_ScrollBar_TextEdit, 
                                     ScrollBarWidth, ScrollBarHeight, 
                                     0, AUFLOESUNG + ScrollBarVisit, 
                                     0, ScrollBarVisit,
                                     canvas);
    swSetClientData(RedScrollBar,this);
    swSetScrollBarCallback(RedScrollBar, RedScrollCB);

    GreenScrollBar = swCreateScrollBar(SW_HORIZONTAL, 
                                       X_ScrollBar, 
                                       Y_ScrollBar_TextEdit + Y_ABSTAND, 
                                       ScrollBarWidth, ScrollBarHeight, 
                                       0, AUFLOESUNG + ScrollBarVisit,
                                       0, ScrollBarVisit, canvas);
    swSetClientData(GreenScrollBar,this);
    swSetScrollBarCallback(GreenScrollBar, GreenScrollCB);

    BlueScrollBar = swCreateScrollBar(SW_HORIZONTAL, 
                                      X_ScrollBar, 
                                      Y_ScrollBar_TextEdit + 2 * Y_ABSTAND, 
                                      ScrollBarWidth, ScrollBarHeight, 
                                      0, AUFLOESUNG + ScrollBarVisit,
                                      0, ScrollBarVisit, canvas);
    swSetClientData(BlueScrollBar,this);
    swSetScrollBarCallback(BlueScrollBar, BlueScrollCB);

    if (m_alphaEnabled) {
        AlphaScrollBar = swCreateScrollBar(SW_HORIZONTAL, 
                                           X_ScrollBar, 
                                           Y_ScrollBar_TextEdit + 3 * Y_ABSTAND, 
                                           ScrollBarWidth, ScrollBarHeight, 
                                           0, AUFLOESUNG + ScrollBarVisit,
                                           0, ScrollBarVisit, canvas);
        swSetClientData(AlphaScrollBar,this);
        swSetScrollBarCallback(AlphaScrollBar, AlphaScrollCB);
    }

    RedTextEdit = swCreateTextEdit(SW_SINGLE_LINE, X_TEXTEDIT, 
                                   Y_ScrollBar_TextEdit, 
                                   TextEditWidth, TextEditHeight, canvas); 
    swTextEditSetSelection(RedTextEdit, 1, 4);
    swSetClientData(RedTextEdit,this);
    swSetCommandCallback(RedTextEdit, RedTextEditInputCB);          

    GreenTextEdit = swCreateTextEdit(SW_SINGLE_LINE, X_TEXTEDIT, 
                                     Y_ScrollBar_TextEdit + Y_ABSTAND, 
                                     TextEditWidth, TextEditHeight, canvas);   
    swTextEditSetSelection(GreenTextEdit, 1, 4);
    swSetClientData(GreenTextEdit,this);
    swSetCommandCallback(GreenTextEdit, GreenTextEditInputCB);

    BlueTextEdit = swCreateTextEdit(SW_SINGLE_LINE, X_TEXTEDIT, 
                                    Y_ScrollBar_TextEdit + 2 * Y_ABSTAND, 
                                    TextEditWidth, TextEditHeight, canvas);
    swTextEditSetSelection(BlueTextEdit, 1, 4);
    swSetClientData(BlueTextEdit,this);
    swSetCommandCallback(BlueTextEdit, BlueTextEditInputCB);          

    if (m_alphaEnabled) {
        AlphaTextEdit = swCreateTextEdit(SW_SINGLE_LINE, X_TEXTEDIT, 
                                         Y_ScrollBar_TextEdit + 3 * Y_ABSTAND, 
                                         TextEditWidth, TextEditHeight, canvas);
        swTextEditSetSelection(AlphaTextEdit, 1, 4);
        swSetClientData(AlphaTextEdit,this);
        swSetCommandCallback(AlphaTextEdit, AlphaTextEditInputCB);          
    }
    float RedValue, GreenValue, BlueValue;

    getColorsFromFieldUpdate(RedValue,GreenValue,BlueValue);

    OldColor(&RedValue, &GreenValue, &BlueValue, true);
    
    Color = Round_float_to_int((RedValue) * ((float)MAXFARB));
    ColorTemp = Round_float_to_int((GreenValue) * ((float)MAXFARB));
    Color += ColorTemp << 8;
    ColorTemp = Round_float_to_int((BlueValue) * ((float)MAXFARB));
    Color += ColorTemp << 16;
    
    SetGetColorPoint(Color, true);    

    if (m_dc==NULL) 
       m_dc = swCreateDC(m_wnd);
    swSetFGColor(m_dc, swGetWindowColor(canvas, SW_COLOR_WINDOW_BG));
    swFillRect(m_dc, 0, 0, width, height);
}

void ColorCircle::getColorsFromFieldUpdate(float& r, float& g, float &b)
{
    // get colors from FieldUpdate hint

    FieldValue *value = NULL;
    Node *node = m_fieldUpdate.node;
    if ((node->getType() == VRML_COLOR) || 
        (node->getType() == X3D_COLOR_RGBA)) {
        if (node->hasParent()) {
            GeometryNode *geometry = (GeometryNode *)node->getParent();
            NodeMaterial *material = geometry->getMaterialNode();
            if (material)
                value = material->diffuseColor();
        }
    } else {
        if (m_fieldUpdate.index == -1) {
            value = node->getField(m_fieldUpdate.field);
        } else {
            MFieldValue *val = (MFieldValue *) 
                               node->getField(m_fieldUpdate.field);
            value = val->getSFValue(m_fieldUpdate.index);           
        }
    }
    r=((SFColor*) value)->getValue(0);
    g=((SFColor*) value)->getValue(1);
    b=((SFColor*) value)->getValue(2);
}

void ColorCircle::setColorsViaFieldUpdate(float& r, float& g, float &b,
                                          float &a)
{
    // set colors via FieldUpdate hint

    SFColor newVal(r,g,b);
    FieldValue* newValue=&newVal;
    Node *node = m_fieldUpdate.node;
    m_scene->addNextCommand();
    m_scene->backupField(node, m_fieldUpdate.field);            
    if (node->getType()== VRML_COLOR) {
        NodeColor *ncolor = (NodeColor *)node;
        MFColor *colors = ncolor->color();
        if (node->hasParent()) {
            if (node->getParent()->getColored()) {
                Node *geometry = node->getParent();
                Colored *colored = geometry->getColored();
                GeometryNode *geo = (GeometryNode *)geometry;
                SFColor *diffuseColor = new SFColor(0.8,0.8,0.8);
                if (colored->getMaterialNode(geo)) {
                    delete diffuseColor;
                    diffuseColor = colored->getMaterialNode(
                                   (GeometryNode *)geometry)->diffuseColor();
                }
                NodeIndexedFaceSet *face = NULL;
                if (m_scene->getSelectionMode() == SELECTION_MODE_FACES) {
                    if (geometry->getType() == VRML_INDEXED_FACE_SET) {
                        face = (NodeIndexedFaceSet *)geometry;
                        if (face->colorPerVertex()->getValue())
                            face->changeToColorPerFace();
                    }    
                    NodeCoordinate *coord = geometry->getCoordinateNode();
                    if (colors && (colors->getSize() == 0)) {
                        colors = new MFColor();
                        if (coord->point())
                            for (int i = 0; 
                                 i < coord->point()->getSFSize();i++)
                                colors->insertSFValue(i, diffuseColor); 
                    } else if (coord->point() && 
                               (coord->point()->getSFSize() >
                               colors->getSFSize())) {
                              colors = new MFColor((MFColor *)
                                                   (ncolor->color()->copy()));
                              for (int i = colors->getSFSize(); 
                                   i < coord->point()->getSFSize();i++)
                                  colors->insertSFValue(i, diffuseColor); 
                    } else if (coord->point() && 
                           (coord->point()->getSFSize() < 
                            colors->getSFSize())) {
                        colors = new MFColor((MFColor *)
                                             (ncolor->color()->copy()));
                        for (int i = coord->point()->getSFSize();
                             i < colors->getSFSize(); i++)
                            colors->removeSFValue(coord->point()->getSFSize()); 
                    } else
                        colors = new MFColor((MFColor *)(ncolor->color()->
                                                         copy()));
                    float rgb[] = {r, g, b};
                    if (coord) {
                        for (int i = 0; i < m_scene->getSelectedHandlesSize();
                             i++) {
                            if (face) {
                                colors->setSFValue(
                                    m_scene->getSelectedHandle(i), rgb);
                                int j = face->symetricFace(
                                            m_scene->getSelectedHandle(i));
                                if (j != -1)
                                     colors->setSFValue(j, rgb);
                            }
                        }
                        m_scene->setField(node, m_fieldUpdate.field, colors);
                        if (colored->colorIndexField() != -1)
                            m_scene->setField(geometry, 
                                              colored->colorIndexField(),
                                              new MFInt32());
                    }                    
                } else {
                    NodeCoordinate *coord = geometry->getCoordinateNode();
                    if (geometry->getType() == VRML_INDEXED_FACE_SET) {
                        NodeIndexedFaceSet *face = (NodeIndexedFaceSet *)
                                                   geometry;
                        if (face->colorPerVertex()->getValue())
                            face->changeToColorPerVertex();
                    }    
                    int numFaces = geo->getMesh()->getNumFaces();
                    if (colors && (colors->getSize() == 0)) {
                        colors = new MFColor();
                        if (colors->getSFSize() == 0)
                            for (int i = 0; i < numFaces;i++)
                                colors->insertSFValue(i, diffuseColor); 
                    } else if (numFaces > colors->getSFSize()) {
                              colors = new MFColor((MFColor *)
                                                   (ncolor->color()->copy()));
                              for (int i = colors->getSFSize(); i < numFaces;
                                   i++)
                                  colors->insertSFValue(i, diffuseColor); 
                    } else if (numFaces < colors->getSFSize()) {
                        colors = new MFColor((MFColor *)
                                             (ncolor->color()->copy()));
                        for (int i = numFaces; i < colors->getSFSize(); i++)
                            colors->removeSFValue(coord->point()->getSFSize()); 
                    } else
                        colors = new MFColor((MFColor *)(ncolor->color()->
                                                         copy()));
                    float rgb[] = {r, g, b};

                    if (coord) {
                        if (node->getScene()->getXSymetricMode()) {
                            for (int i = 0; i < coord->getMaxHandle(); i++)
                                if (coord->checkHandle(i))
                                    colors->setSFValue(i, rgb);
                        } 
                        for (int i = 0; i < m_scene->getSelectedHandlesSize();
                             i++)
                             if (coord->validHandle(
                                 m_scene->getSelectedHandle(i)))
                                 colors->setSFValue(
                                    m_scene->getSelectedHandle(i), rgb);
                        m_scene->setField(node, m_fieldUpdate.field, colors);
                    }                    
                }
            } else if (node->getParent()->getType() == VRML_ELEVATION_GRID) {
                NodeElevationGrid *elevationGrid = (NodeElevationGrid *)
                                                   node->getParent();
                if (colors && (colors->getSize() == 0)) {
                    colors = new MFColor();
                    SFColor *diffuseColor = new SFColor(0.8,0.8,0.8);
                    if (elevationGrid->getMaterialNode()) {
                        delete diffuseColor;
                        diffuseColor = elevationGrid->getMaterialNode()->
                                       diffuseColor();
                    }
                    for (int i = 0; i <= elevationGrid->getMaxHandle(); i++)
                        colors->insertSFValue(i, diffuseColor); 
                } else
                    colors = new MFColor((MFColor *)(ncolor->color()->copy()));
                float rgb[] = {r, g, b};
                if (node->getScene()->getXSymetricMode()) {
                    for (int i = 0; i < elevationGrid->getMaxHandle(); i++)
                        if (elevationGrid->checkHandle(i))
                            colors->setSFValue(i, rgb);
                } else
                    for (int i = 0; i < m_scene->getSelectedHandlesSize(); 
                         i++)
                        colors->setSFValue(m_scene->getSelectedHandle(i), 
                                           rgb);
                m_scene->setField(node, m_fieldUpdate.field, colors);
                m_scene->setField(elevationGrid, 
                                 elevationGrid->colorPerVertex_Field(),
                                 new SFBool(true));
            } else if (node->getParent()->getType() == 
                       VRML_GEO_ELEVATION_GRID) {
                NodeGeoElevationGrid *elevationGrid = (NodeGeoElevationGrid *)
                                                      node->getParent();
                if (colors && (colors->getSize() == 0)) {
                    colors = new MFColor();
                    SFColor *diffuseColor = new SFColor(0.8,0.8,0.8);
                    if (elevationGrid->getMaterialNode()) {
                        delete diffuseColor;
                        diffuseColor = elevationGrid->getMaterialNode()->
                                       diffuseColor();
                    }
                    for (int i = 0; i <= elevationGrid->getMaxHandle(); i++)
                        colors->insertSFValue(i, diffuseColor); 
                } else
                    colors = new MFColor((MFColor *)(ncolor->color()->copy()));
                float rgb[] = {r, g, b};
                if (node->getScene()->getXSymetricMode()) {
                    for (int i = 0; i < elevationGrid->getMaxHandle(); i++)
                        if (elevationGrid->checkHandle(i))
                            colors->setSFValue(i, rgb);
                } else
                    for (int i = 0; i < m_scene->getSelectedHandlesSize(); 
                         i++)
                        colors->setSFValue(m_scene->getSelectedHandle(i), 
                                           rgb);
                m_scene->setField(node, m_fieldUpdate.field, colors);
                m_scene->setField(elevationGrid, 
                                 elevationGrid->colorPerVertex_Field(),
                                 new SFBool(true));
            }
        }
    } else if (node->getType()== X3D_COLOR_RGBA) {
        NodeColorRGBA *ncolor = (NodeColorRGBA *)node;
        MFColorRGBA *colors = ncolor->color();
        if (node->hasParent()) {
            if (node->getParent()->getColored()) {
                Node *geometry = node->getParent();
                Colored *colored = geometry->getColored();
                SFColor *diffuseColor = new SFColor(0.8,0.8,0.8);
                GeometryNode *geo = (GeometryNode *)geometry;
                if (colored->getMaterialNode(geo)) {
                    delete diffuseColor;
                    diffuseColor = colored->getMaterialNode(
                                   (GeometryNode *)geometry)->diffuseColor();
                }
                NodeCoordinate *coord = geometry->getCoordinateNode();
                if (colors && (colors->getSize() == 0)) {
                    colors = new MFColorRGBA();
                    if (coord->point())
                        for (int i = 0; 
                             i < coord->point()->getSFSize();i++)
                            colors->insertSFValue(i, diffuseColor); 
                } else if (colored->getColorPerVertex(geo) && coord->point() && 
                           (coord->point()->getSFSize() >
                            colors->getSFSize())) {
                        colors = new MFColorRGBA((MFColorRGBA *)
                                                 ncolor->color()->copy());
                        for (int i = colors->getSFSize(); 
                             i < coord->point()->getSFSize();i++)
                            colors->insertSFValue(i, diffuseColor); 
                } else if (colored->getColorPerVertex(geo) && coord->point() && 
                           (coord->point()->getSFSize() < 
                            colors->getSFSize())) {
                        colors = new MFColorRGBA((MFColorRGBA *)
                                                 (ncolor->color()->copy()));
                        for (int i = coord->point()->getSFSize();
                             i < colors->getSFSize(); i++)
                            colors->removeSFValue(coord->point()->getSFSize()); 
                } else
                    colors = new MFColorRGBA((MFColorRGBA *)
                                             ncolor->color()->copy());
                float rgba[] = {r, g, b, a};
                bool faces = m_scene->getSelectionMode() == 
                             SELECTION_MODE_FACES;
                if (faces)
                    m_scene->setSelectionMode(SELECTION_MODE_VERTICES);  

                if (coord) {
                    if (node->getScene()->getXSymetricMode()) {
                        for (int i = 0; i <= coord->getMaxHandle(); i++)
                            if (coord->checkHandle(i))
                                colors->setSFValue(i, rgba);
                    } else
                        for (int i = 0; i < m_scene->getSelectedHandlesSize(); 
                             i++)
                            if (coord->validHandle(
                                m_scene->getSelectedHandle(i)))
                                colors->setSFValue(m_scene->
                                                   getSelectedHandle(i), rgba);
                    m_scene->setField(node, m_fieldUpdate.field, colors);
                    if (colored->colorPerVertexField() != -1)
                        m_scene->setField(geometry, 
                                         colored->colorPerVertexField(),
                                         new SFBool(true));
                    if (colored->colorIndexField() != -1)
                        m_scene->setField(geometry, colored->colorIndexField(),
                                         new MFInt32());
                }                    
                if (faces)
                    m_scene->setSelectionMode(SELECTION_MODE_FACES);  
            }
            else if (node->getParent()->getType() == VRML_ELEVATION_GRID) {
                NodeElevationGrid *elevationGrid = (NodeElevationGrid *)
                                                   node->getParent();
                if (colors && (colors->getSize() == 0)) {
                    colors = new MFColorRGBA();
                    SFColor *diffuseColor = new SFColor(0.8,0.8,0.8);
                    if (elevationGrid->getMaterialNode()) {
                        delete diffuseColor;
                        diffuseColor = elevationGrid->getMaterialNode()->
                                       diffuseColor();
                    }
                    for (int i = 0; i <= elevationGrid->getMaxHandle(); i++)
                        colors->insertSFValue(i, diffuseColor); 
                } else
                    colors = new MFColorRGBA((MFColorRGBA *)
                                             ncolor->color()->copy());
                float rgba[] = {r, g, b, a};
                if (node->getScene()->getXSymetricMode()) {
                    for (int i = 0; i < elevationGrid->getMaxHandle(); i++)
                        if (elevationGrid->checkHandle(i))
                            colors->setSFValue(i, rgba);
                } else
                    for (int i = 0; i < m_scene->getSelectedHandlesSize(); 
                         i++)
                        colors->setSFValue(m_scene->getSelectedHandle(i), 
                                           rgba);
                m_scene->setField(node, m_fieldUpdate.field, colors);
                m_scene->setField(elevationGrid, 
                                 elevationGrid->colorPerVertex_Field(),
                                 new SFBool(true));
            } else if (node->getParent()->getType() == 
                       VRML_GEO_ELEVATION_GRID) {
                NodeGeoElevationGrid *elevationGrid = (NodeGeoElevationGrid *)
                                                      node->getParent();
                if (colors && (colors->getSize() == 0)) {
                    colors = new MFColorRGBA();
                    SFColor *diffuseColor = new SFColor(0.8,0.8,0.8);
                    if (elevationGrid->getMaterialNode()) {
                        delete diffuseColor;
                        diffuseColor = elevationGrid->getMaterialNode()->
                                       diffuseColor();
                    }
                    for (int i = 0; i <= elevationGrid->getMaxHandle(); i++)
                        colors->insertSFValue(i, diffuseColor); 
                } else
                    colors = new MFColorRGBA((MFColorRGBA *)
                                             ncolor->color()->copy());
                float rgba[] = {r, g, b, a};
                if (node->getScene()->getXSymetricMode()) {
                    for (int i = 0; i < elevationGrid->getMaxHandle(); i++)
                        if (elevationGrid->checkHandle(i))
                            colors->setSFValue(i, rgba);
                } else
                    for (int i = 0; i < m_scene->getSelectedHandlesSize(); 
                         i++)
                        colors->setSFValue(m_scene->getSelectedHandle(i), 
                                           rgba);
                m_scene->setField(node, m_fieldUpdate.field, colors);
                m_scene->setField(elevationGrid, 
                                 elevationGrid->colorPerVertex_Field(),
                                 new SFBool(true));
            }
        }
    } else if (m_fieldUpdate.index == -1) {
       m_scene->backupField(node, m_fieldUpdate.field);
       m_scene->setField(node, m_fieldUpdate.field, new SFColor(r,g,b));
    } else {
       MFieldValue *value = (MFieldValue *) node->getField(m_fieldUpdate.field);
       value->setSFValue(m_fieldUpdate.index, newValue);
       m_scene->UpdateViews(NULL, UPDATE_FIELD, (Hint *) &m_fieldUpdate);
    }
}

void ColorCircle::DeleteView()
{
    int width, height;

    swGetSize(canvas, &width, &height);
    swSetFGColor(m_dc, swGetWindowColor(canvas, SW_COLOR_WINDOW_BG));
    swFillRect(m_dc, 0, 0, width, height);

    swDeleteCallbacks(RedScrollBar);
    swDestroyWindow(RedScrollBar);
    RedScrollBar=NULL;

    swDeleteCallbacks(GreenScrollBar);
    swDestroyWindow(GreenScrollBar);
    GreenScrollBar=NULL;

    swDeleteCallbacks(BlueScrollBar);
    swDestroyWindow(BlueScrollBar);
    BlueScrollBar=NULL;

    swDeleteCallbacks(CloseButton);
    swDestroyWindow(CloseButton);
    CloseButton=NULL;

    swDeleteCallbacks(OkeyButton);
    swDestroyWindow(OkeyButton);
    OkeyButton=NULL;

    swDeleteCallbacks(RedTextEdit);
    swDestroyWindow(RedTextEdit); 
    RedTextEdit=NULL;
    swDeleteCallbacks(GreenTextEdit);
    swDestroyWindow(GreenTextEdit);
    GreenTextEdit=NULL; 
    swDeleteCallbacks(BlueTextEdit);
    swDestroyWindow(BlueTextEdit);
    BlueTextEdit=NULL;
    if (m_alphaEnabled) {
        swDeleteCallbacks(AlphaTextEdit);
        swDestroyWindow(AlphaTextEdit);
        AlphaTextEdit=NULL;
    }

    swDestroyDC(m_dc);
    m_dc=NULL;

    swDeleteCallbacks(canvas);
}
