/*
 * ColorCircle.h
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

#ifdef _WIN32
#include <windows.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#ifndef FLT_MAX
# include <float.h>
#endif
#include "swt.h"

#include "stdafx.h"
#include "Scene.h"
#include "SceneView.h"
#include "Node.h"
#include "FieldValue.h"
#include "SFColor.h"
#include "MFieldCommand.h"

#define REDWINKEL 0.0
#define YELLOWWINKEL 60.0
#define GREENWINKEL 120.0
#define CYANWINKEL 180.0
#define BLUEWINKEL 240.0
#define MAGENTAWINKEL 300.0
#define MAXWINKEL 360.0

#define MAXFARB 255
#define AUFLOESUNG 100

#define X_ScrollBar 30
#define Y_ScrollBar_TextEdit 160
#define ScrollBarVisit 10

#define VALUEBARWIDTH 20

#define X_TEXTEDIT 240

#define UPPER_LIMIT 20

class ColorCircle : public SceneView
   {
public:   
   ColorCircle(Scene *scene, SWND parent, FieldUpdate* fieldUpdate);

   const char *getName(void) const { return "ColorCircle"; }

   void DeleteView();

   /**
    * RedTextEditInput -- Callback fuer den Textfeld des Rotanteils. 
    *                     Liest erst den eingegebenen Text ein und wandelt 
    *                     den Text in ein double um. Danach liest diese 
    *                     Funktion die aktuelle Farbe ein, holt sich daraus 
    *                     den Rotanteil. Prueft nun erstmal ob der eingegebene
    *                     Text in dem gueltigen Eingabe Bereich ist. Wenn es 
    *                     passt dann gibt er den neuen Rotanteil als Text aus 
    *                     in dem RotTextFeld(RedTextEdit), nun wird der 
    *                     RedScrollBar auf dem Rotanteil gesetzt und die 
    *                     geaenderte Farbe wird gesetzt in der 
    *                     SetGetColorPoint Funktion, die dafuer geschrieben 
    *                     wurde. Die ColorPalette wird gesetzt und der 
    *                     Farbbalken wird auch gesetzt und noch die Farbe wird 
    *                     im Farbkreis ueberprueft. Wenn der eingegebene 
    *                     Farbanteil nicht in dem Eingabe Bereich liegt,
    *                     wird der alte Farbanteil in dem entsprechenden 
    *                     TextFenster geschrieben.
    */
    
   void RedTextEditInput(void *command);
   
   /**
    * GreenTextEditInput -- siehe RedTextEditInput
    */

   void GreenTextEditInput(void *command);
   
   /**
    * BlueTextEditInput -- siehe RedTextEditInput
    */

   void BlueTextEditInput(void *command);

   /**
    * AlphaTextEditInput -- siehe RedTextEditInput
    */

   void AlphaTextEditInput(void *command);
   
   /**
    * CloseButtonCallback -- ein Callback fuer den Close Button, wenn er 
    *                        gedrueckt wurde, wird die ganze Farbfunktion 
    *                        abgebrochen und die alte Farbe an dune zurueck 
    *                        gegeben.
    */

   void
   CloseButtonCallback(void *id);
   
   /**
    * OkayButtonCallback -- ein Callback fuer den Okay Button, wenn er 
    *                       gedrueckt wurde, wird die ganze Farbfunktion 
    *                       unterbrochen und der neue also die ausgewaehlte 
    *                       Farbe an dune zurueck gegeben.
    */
    
   void
   OkeyButtonCallback(void *id);
   
   void
   RedScrollCallback(int type, int value);
   
   void
   GreenScrollCallback(int type, int value);
   
   void
   BlueScrollCallback(int type, int value);
   
   void
   AlphaScrollCallback(int type, int value);
   
   void
   mouseCallback(int x, int y, int modifiers);

   void
   keyCallback(int key, int state, int x, int y, int modifiers);
   
   void
   resizeCallback(int x, int y);
   
   void
   exposeCallback(int x, int y, int width, int height);

   /**
    * ScrollBar -- Stuktur fuer die ScrollBars zum setzten der Position 
    *              und der Groesse fuer der Funktion resizeCallBack gedacht
    *
    * x         X - Position des jeweiligen ScrollBar
    *
    * y         Y - Position des jeweiligen ScrollBar
    *
    * width     Breite des jeweiligen ScrollBar
    *
    * height    Hoehe des jeweiligen ScrollBar
    */
    
   struct ScrollBar
   {
       int x, y;
       int width, height;
   };
   
   
   /**
    * TextEdit -- Stuktur fuer die TextEdits zum setzten der Position und der 
    *             Groesse fuer der Funktion resizeCallBack gedacht
    *
    * x         X - Position des jeweiligen TextEdit
    *
    * y         Y - Position des jeweiligen TextEdit
    *
    * width     Breite des jeweiligen TextEdit
    *
    * height    Hoehe des jeweiligen TextEdit
    */
    
   struct TextEdit
   {
        int x, y;
        int width, height;
   };
   
   /**
    * Button -- Stuktur fuer die Buttons zum setzten der Position und der 
    *           Groesse fuer der Funktion resizeCallBack gedacht
    *
    * x         X - Position des jeweiligen Button
    *
    * y         Y - Position des jeweiligen Button
    *
    * width     Breite des jeweiligen Button
    *
    * height    Hoehe des jeweiligen Button
    */
    
   struct Button
   {
        int x, y;
        int width, height;
   };
   
private:

   SWND canvas, RedScrollBar, GreenScrollBar, BlueScrollBar, AlphaScrollBar,
        CloseButton, OkeyButton, RedTextEdit, GreenTextEdit, BlueTextEdit,
        AlphaTextEdit;
   int drawing;
   
   
   /**
    * SetGetColorPoint -- Setzen oder Rueckgabe der gemerkten Farbe
    *
    * Color     zu setzende Farbe
    *
    * Set       wenn gesetzt soll die Farbe sie gesetzt werden. 
    *           Wenn nicht gesetzt soll nur die Farbe uebergeben werden.
    *
    * RETURN    Rueckgabe der aktuellen Farbe
    */
    
   int SetGetColorPoint(int Color, bool Set);

   void OldColor(float *Red, float *Green, float *Blue, bool oldcolor);

   /**
    * ScrollBars -- Struktur fuer die ScrollBars der jeweiligen Farbanteile 
    *               Rot, Gruen und Blau
    *
    * red       ScrollBar von dem Rotanteil der Positionen und Groessen
    *
    * green     ScrollBar von dem Gruenanteil der Positionen und Groessen
    *
    * blue      ScrollBar von dem Blauanteil der Positionen und Groessen
    *
    * alpha     ScrollBar von dem Alphaanteil der Positionen und Groessen
    */
    
   struct ScrollBars
   {
       struct ScrollBar red;
       struct ScrollBar green;
       struct ScrollBar blue;
       struct ScrollBar alpha;
   };
   
   /**
    * TextEdits -- Struktur fuer die TextEdits der jeweiligen Farbanteile 
    *              Rot, Gruen und Blau
    *
    * red       ScrollBar von dem Rotanteil der Positionen und Groessen
    *
    * green     ScrollBar von dem Gruenanteil der Positionen und Groessen
    *
    * blue      ScrollBar von dem Blauanteil der Positionen und Groessen
    *
    * alpha     ScrollBar von dem Alphaanteil der Positionen und Groessen
    */

   struct TextEdits
   {
       struct TextEdit red;
       struct TextEdit green;
       struct TextEdit blue;
       struct TextEdit alpha;
   };
   
   /**
    * Buttons -- Struktur fuer die Buttons der jeweiligem Button
    *
    * okey      Button fuer das Okay zum Bestaetigen der ausgewaehlten 
    *           Farbe der Positionen und Groessen
    *
    * close     Button fuer das Close zum Abbrechen der Farbauswahl der 
    *           Positionen und Groessen
    */
    
   struct Buttons
   {
       struct Button okay;
       struct Button close;
   };  
   
   /**
    * SetScrollBars -- Setzen der Position und Groesse aller ScrollBars, 
    *                 TextEdit und Buttons bei einem resizeCallBack
    *
    * scroll    alle Werte fuer die Postion und Groesse der ScrollBars
    * 
    * TextEdit  alle Werte fuer die Postion und Groesse der TextEdits
    *
    * button    alle Werte fuer die Postion und Groesse der buttons
    */
    
   void SetScrollBars(struct ScrollBars scroll, struct TextEdits textedit, 
                     struct Buttons button);

   /**
    * window_size -- Struktur fuer den Mittelpunkt des Farbkreises und der 
    *                Groesse des Arbeitsfensters und des Radius des Farbkreises
    * 
    * x0        Position vom Mittelpunkt des Farbkreises in der X-Achse
    * 
    * y0        Position vom Mittelpunkt des Farbkreises in der Y-Achse
    * 
    * x_window  Fenstergoesse in der X-Achse
    *
    * y_window  Fenstergoesse in der Y-Achse
    *
    * Radius    Radius des Farbkreises
    */
    
   struct window_size
   {
       int x0, y0;
       int x_window, y_window;
       float Radius;
   };
   
   /**
    * GetPositions -- Errechnen der Stellen oder auch Postionen fuer alle 
    *                 Objekte
    * 
    * windowback   Inhalt aller wichtigen Information fuer die Stellungen 
    *              der Objekte
    *
    * resize       Pruefen ob es sich um ein resizeCallBack handelt oder 
    *              nicht, um sich die neue Fenstergroesse zumerken und 
    *              sie dann auch setzen, ruft dabei die Funktion 
    *              SetScrollBar auf um die Objekte zusetzen
    */
    
   void GetPositions(struct window_size* windowback, bool resize);
   
   /**
    * Round_float_to_int -- Runden einer Floatzahl
    *
    * number    zu rundende Zahl
    *
    * RETURN    gerundeter Integer
    */

#ifdef HAVE_RINT
   inline int Round_float_to_int(float number) {return (int)rint(number);}
#else
   int Round_float_to_int(float number);
#endif
   
   /**
    * GetColorFloatToInt -- Errechnet die zusammengesetze Farbe von 
    *                       Scrollbar(0-100)  float werten in Farbwerte(0-255) 
    *                       und setzt diese dann zusammen in rin Integer Wert
    *                       um diesen gibt er dann als Rueckgabewert zurueck
    * 
    * red       Rotanteil der ganzen Farbe
    * 
    * green     Gruenanteil der ganzen Farbe
    *
    * blue      Blauanteil der ganzen Farbe
    *
    * RETURN    Rueckgabe der vollstaendigen Farbe als ein Integerwert
    */
    
   int GetColorFloatToInt(float red, float green, float blue);
   
   /**
    * GetBarValue -- Holen einer Farbe vom Farbbalken
    * 
    * x         X-Achse des Farbbalken von der die Farbe ermittlet werden soll
    * 
    * y         Y-Achse des Farbbalken von der die Farbe ermittlet werden soll
    *
    * RETURN    Rueckgabe der jeweiligen Farbe als Integer, wenn die y Koordinate nicht im Farbbalken ist
    *           dann gibt die Funktion die Fehlermeldung -1 zurueck
    *
    * ACHTUNG   Die Funktion berechnet selbststaendig alle Positionen aus
    */
    
   int GetBarValue(int y);
   
   /**
    * SetBarValue -- Setzen des Farbbalkens in die canvas(Leinwand) auf dem 
    *                Bildschirm
    *
    * ACHTUNG   Die Funktion berechnet selbststaendig alle Positionen aus
    */
    
   void SetBarValue();
   
   /**
    * Colors -- Struktur fuer die Farbanteile um nur die Farbanteile von der 
    *           GetColorCircle zu uebergeben. Benoetigen einige Funktionen
    * 
    * red       Rotanteil als float
    * 
    * green     Gruenanteil als float
    * 
    * blue      Blauanteil als float
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
    * x         X-Position des holende Farbe fuer dem Farbkeis
    * 
    * y         Y-Position des holende Farbe fuer dem Farbkeis
    *
    * colors    Farbenuebergabe in float(0-1.00) brauchen einige Funktionen
    *
    * ACHTUNG   Die Funktion berechnet selbststaendig alle Positionen aus
    */
    
   int GetColorCircle(int x, int y, struct Colors* colors);
   
   /**
    * SetColorPalette -- Setzen der FarbauswahlRechtecks
    * 
    * Color    Diese Farbe wird als Rechteck in das Fenster geschrieben
    * 
    * 
    * ACHTUNG das Rechteck wird immer an die gleiche Position geschrieben, es ist 
    *         noch nicht im resizeCallBack mit drin
    */
    
   void SetColorPalette(int Color);
   
   /**
    * ClearCircleCursor -- Loeschen des FarbKreis Cursor
    * 
    * x X-Position des Farbkreis Cursor
    *  
    * y Y-Position des Farbkreis Cursor
    */
    
   void ClearCircleCursor(int x, int y);
   
   /**
    * DrawCircleCursor -- Setzen des FarbKreis Cursor
    * 
    * x X-Position des Farbkreis Cursor
    *  
    * y Y-Position des Farbkreis Cursor
    */
    
   void DrawCircleCursor(int x, int y);
   
   /**
    * SetCircleCursor -- Ermoeglicht eine Animation des Farbkreis Cursor
    * 
    * x     neue X-Position
    * 
    * y     neue Y-Position
    * 
    * Color neue Farbe in der Farbbalken Cursor erstellt werden soll
    */
    
   void SetCircleCursor(int x, int y);
   
   /**
    * GetCircleCursor -- Ermittelt die Cursor Position des Farbkreises per 
    *                    ScrollBar-Einstellung
    * 
    * RedValue   Der Rotanteil des RedScrollBar
    *  
    * GreenValue Der Gruenanteil des RedScrollBar
    * 
    * BlueValue  Der Blauanteil des RedScrollBar
    */
    
   void GetCircleCursor(int ColorNew);
   
   /**
    * SetTextColorsValues -- Setzt alle Farbwerte in den jeweiligen Textfeldern 
    *                        der ScrollBars ein fuer die installisierung 
    *                        gedacht
    * 
    * value Farbe fuer die jeweiligen Farbsetztung
    */
    
   void SetTextColorsValues(int value);
   
   /**
    * SetTextValue -- Setzt ein Textfeld eines ScrollBar in das jeweilige 
    *                 Textfeld ein
    * 
    * value    Wert fuer die jeweiligen Farbsetztung
    * 
    * textedit um den Wert in das richtige Textfeld einzusetzen 
    */
    
   void SetTextValue(float value, SWND textedit);
   
   /**
    * ClearValueBarCursor -- Loeschen des Farbbalken Cursor
    *  
    * y Y-Position des Farbbalken Cursor
    */
    
   void ClearValueBarCursor(int y);
   
   /**
    * DrawValueBarCursor -- Setzen des Farbbalken Cursor
    * 
    * y         Y-Position des Farbbalken Cursor
    *  
    * Color     Farbe des Farbbalken Cursor der in dem er wieder gesetzt 
    *           werden soll
    */
    
   void DrawValueBarCursor(int y, int Color);
   
   /**
    * SetBarValueCursor -- Ermoeglicht eine Animation des Farbbalken Cursor
    * 
    * y         neue Y-Position
    * 
    * Color     neue Farbe in der Farbbalken Cursor erstellt werden soll
    */
    
   void SetBarValueCursor(int y, int Color);
   
   /**
    * SetValue_ScrollBar -- Setzt alle Farbwerte in den Scrollbars ein, 
    *                       errechnet vorher die aktuelle Farbe dafuer aus.
    */

   void SetValue_ScrollBar(int Color);

   /**
    * SetCircle -- Malt den Farbkreis auf den Bildschirm
    */
     
   void SetCircle();

   void getColorsFromFieldUpdate(float& red, float& green, float &blue);

   void setColorsViaFieldUpdate(float& r, float& g, float &b, float &a);

protected:
        SDC m_dc;
        float m_alpha;
        bool m_alphaEnabled;
        FieldUpdate m_fieldUpdate;
//        FieldUpdate* m_fieldUpdate;
   };
