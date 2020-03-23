/*
 * swt_structs.h - structures of stephen's window toolkit
 *
 * This file excluses the structures of the file swt.c
 * It is only usefull for debugging problems with the structures in swt,
 * e.g. include this file into the sourcefile and recompile if the debugger 
 *      tells you, it do not know the content of structure "SWindow"
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

/* 
   Define if your XmCreateFileSelectionDialog can do 
   shell like regular expressions like *.[wx][rm]l
   Rather impossible to test via configure 8-(
*/

#include <Xm/Xm.h>
#include <Xm/BulletinB.h>
#include <Xm/CascadeB.h>
#include <Xm/DragDrop.h>
#include <Xm/DrawingA.h>
#include <Xm/FileSB.h>
#include <Xm/Label.h>
#include <Xm/MainW.h>
#include <Xm/MessageB.h>
#include <Xm/Protocols.h>
#include <Xm/PushB.h>
#include <Xm/RowColumn.h>
#include <Xm/ScrollBar.h>
#include <Xm/ScrolledW.h>
#include <Xm/Separator.h>
#include <Xm/Text.h>
#include <Xm/TextF.h>
#include <Xm/ToggleB.h>
#include <X11/keysym.h>
#include <X11/IntrinsicP.h>
#include <X11/cursorfont.h>

#include "swt.h"

#include <GL/gl.h>
#include <GL/glx.h>
#include <GL/glu.h>

#ifdef HAVE_MESA_OFF_SCREEN_RENDERING
# include <GL/osmesa.h>
#endif

#include <stdlib.h>

struct AccelTable;

typedef struct SWindow {
    Widget             widget;
    Widget             shell;
    Widget             mainWnd;
    SMENU              menu;
    struct AccelTable *accelTable;
    int                invx1, invy1, invx2, invy2;
    void              *data;
    ExposeCallback     exposeCallback;
    MouseCallback      mouseCallback;
    KeyCallback        keyCallback;
    FocusCallback      focusCallback;
    ResizeCallback     resizeCallback;
    EnterCallback      enterCallback;
    ScrollBarCallback  scrollBarCallback;
    CommandCallback    commandCallback;
    HighlightCallback  highlightCallback;
    CloseCallback      closeCallback;
    DropCallback       dropCallback;
    int                isVisible;
    int                isHidden;
#define PASSWORD_LENGTH 1024
    char               password_data[PASSWORD_LENGTH];
} SWindow;

typedef struct SDContext {
    GC        gc;
    Drawable  drawable;
    int       isBitmapDC;
    SFONT     font;
} SDContext;

typedef struct SBitmap {
    Pixmap pixmap;
} SBitmap;

typedef struct SCursor {
    Cursor cursor;
} SCursor;

typedef struct SMenu {
    Widget widget;
    SWND   wnd;
    int    id;
    int    flags;
} SMenu;
    
typedef struct SFont {
    XFontStruct *fs;
    XmFontList   fontList;
} SFont;

typedef struct SGLContext {
    GLXContext glxContext;
#ifdef HAVE_MESA_OFF_SCREEN_RENDERING
    OSMesaContext osMesaContext;
#endif
} SGLContext;

typedef struct STimer {
    int            timeout;
    TimerCallback  timerCallback;
    void          *data;
    XtIntervalId   id;
} STimer;

typedef struct Pair {
    char *key;
    char *value;
} Pair;

typedef struct STable {
    int   size;
    int   capacity;
    Pair *data;
    char *filename;
} STable;

typedef struct Accelerator {
    int key;
    int state;
    int id;
} Accelerator;

typedef struct AccelTable {
    int          size;
    int          capacity;
    Accelerator *data;
} AccelTable;

enum {
    FILE_MUST_EXIST =  0x0001,
    OVERWRITE_PROMPT = 0x0002,
    NOT_A_DIRECTORY =  0x0004,
    FILE_EXECUTABLE =  0x0008,
    MAY_BE_FILE     =  0x0010
};

struct swColorShift { 
   int rshift,rbits;
   int gshift,gbits;
   int bshift,bbits;
};
