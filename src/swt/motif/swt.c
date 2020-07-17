/*
 * swt.c - stephen's window toolkit
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

#define MOTIF_FILE_REGEX 1

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
#ifdef HAVE_INCLUDE_X11_XMU
# include <X11/Xmu/StdCmap.h>
#endif
#include <X11/keysym.h>
#include <X11/IntrinsicP.h>
#include <X11/cursorfont.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <errno.h>
#include <ctype.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#ifdef MACOSX
# include <sys/stat.h>
#else
# include <sys/param.h>
# ifndef __FreeBSD__ 
#  include <wait.h>
# endif
#endif

#include <GL/gl.h>
#include <GL/glx.h>

#include "config.h"
#include "maxpath.h"

#ifdef HAVE_XKBKEYCODETOKEYSYM
# include <X11/XKBlib.h>
#endif

#ifdef HAS_MLOCK
# include <sys/mman.h>
#endif

#include "swt.h"
#include "swtxinput.h"
#include "mysnprintf.h"
#include "copy.xbm"
#include "copy_mask.xbm"
#include "link.xbm"
#include "link_mask.xbm"
#include "error.xbm"
#include "error_mask.xbm"
#include "cursor_none.xbm"
#include "dune.xbm"
#include "dune_mask.xbm"
#include "stereoview.h"

#include "swt_structs.h"

#undef MOTIF_DRAGNDROP

#define MAX_PREFLINE 1024

XtAppContext TheAppContext = NULL;

static Display     *TheDisplay = NULL;
static XVisualInfo *TheVisual = NULL;
static Colormap     TheColormap;
static SFont       *TheDefaultFont = NULL;
static int          WindowCapacity;
static SWND        *WindowsToUpdate;
static int          NumWindowsToUpdate = 0;

void closeCallback(void)
{
    NumWindowsToUpdate = 0;
}

static int          InDragNDrop = FALSE;
#ifdef MOTIF_DRAGNDROP
static XEvent       TheLastButtonPress;
static Widget       TheDragContext;
static Widget       TheCopyIcon;
static Widget       TheLinkIcon;
static Widget       TheMoveIcon;
#else /* !MOTIF_DRAGNDROP */
static Widget       TheDragWindow = 0;
static Widget       TheDragPopup = 0;
static Pixmap       TheDragPixmap;
static Pixmap       TheDragMask;
static int          TheDragPixmapOffsetX;
static int          TheDragPixmapOffsetY;
static Cursor       TheCopyCursor;
static Cursor       TheLinkCursor;
static Cursor       TheMoveCursor;
static Cursor       TheErrorCursor;
#endif /* !MOTIF_DRAGNDROP */
static int          _running = FALSE;
static int          _dialogDepth = 0;
static int          _dialogRC = 0;
static int          _keycodeMap[65536];
static int          _invKeycodeMap[65536];
static Pixmap       TheCopyPixmap;
static Pixmap       TheLinkPixmap;
static Pixmap       TheMovePixmap;
static Atom         WM_DELETE_WINDOW;
static int          acceleratorsEnabled = TRUE;
static char        *defaultFontSpec = NULL;
 
static Widget createContainer(const char *title, int x, int y, int width,
                              int height, Widget parent);
static void initWidget(Widget widget, SWND wnd);
static void expose(Widget widget, XtPointer data, XtPointer event_data);
static void update();
static void input(Widget widget, XtPointer data, XtPointer event_data);
static void enterEvent(Widget widget, XtPointer data, XEvent *event, 
                       Boolean *continue_to_dispatch);
static void acceleratorHandler(Widget w, XtPointer data, XEvent *event,
                               Boolean *cont);
static void resize(Widget widget, XtPointer data, XtPointer event_data);
static void scrollCallback(Widget widget, XtPointer data, XtPointer event_data);
static void buttonCallback(Widget widget, XtPointer data, XtPointer event_data);
static void comboBoxActivateCallback(Widget widget, XtPointer data,
                                     XtPointer event_data);
static void menuArmCallback(Widget widget, XtPointer data, 
                            XtPointer event_data);
static void menuDisarmCallback(Widget widget, XtPointer data,
                               XtPointer event_data);
static void menuActivateCallback(Widget widget, XtPointer data,
                                 XtPointer event_data);
static void textActivateCallback(Widget widget, XtPointer data,
                                 XtPointer event_data);
static void textFocusCallback(Widget widget, XtPointer data,
                              XtPointer event_data);
static void textLosingFocusCallback(Widget widget, XtPointer data,
                                    XtPointer event_data);
static void destroyCallback(Widget widget, XtPointer data, 
                            XtPointer event_data);
static void destroyMenuCallback(Widget widget, XtPointer data,
                                XtPointer event_data);
static int doFileDialog(const char *title, SWND wnd, const char *filters,
                        char *buf, int len, int flags, const char* ext);
static void onDeleteWindow(Widget widget, XtPointer data, XtPointer event_data);
static void onDeleteDialog(Widget widget, XtPointer data, XtPointer event_data);
static void timerProc(XtPointer closure, XtIntervalId *id);
static void dlgBtnCallback(Widget widget, XtPointer data, XtPointer event_data);
#ifdef MOTIF_DRAGNDROP
static void dragProc(Widget widget, XtPointer data, XtPointer event_data);
static void dropProc(Widget widget, XtPointer data, XtPointer event_data);
static Boolean convertProc(Widget widget, Atom *selection, Atom *target,
                           Atom *typeRtn, XtPointer *valueRtn,
                           unsigned long *lengthRtn, int *formatRtn,
                           unsigned long *max_lengthRtn,
                           XtPointer data, XtRequestId *request_id);
static void transferProc(Widget widget, XtPointer data,
                         Atom *selection, Atom *type, XtPointer value,
                         unsigned long *length, int *format);
static void dragFinish(Widget widget, XtPointer data,
                       XtPointer event_data);
static Widget createDragIcon(Widget widget, Pixmap pixmap, Pixmap mask,
                             int width, int height);
static void createSourceCursor(Pixmap pixmap, int xPos, int yPos,
                              int width, int height, Widget widget,
                              Pixmap *pixmapOut, Pixmap *maskOut);
static int opToMop(int op);
#else
static void doDragOver(SWindow *wnd, int x, int y, int modifiers);
static void doDrop(SWindow *wnd, int x, int y, int modifiers);
static void exposeDragDropWindow(Widget widget, XtPointer data,
                                 XtPointer event_data);
static void updateCursor(Window window, int operation);
#endif
static const char *parseMenu(const char *s1, char *s2, char *mnemonic);
static int getModifiers(int state);
static int queryModifiers();
static int messageBox(Widget parent, const char *text, const char *title,
                      int type, int icon);
static int fileExists(const char *filename);
static int dirExists(const char *filename);
static SMENU findMenu(Widget widget, int id);
extern void insertMenuItem(Widget menu, SWND wnd, short pos, const char *text, 
                           int id);
static void resetRadioButtons(Widget widget);
static void callCommandCallback(Widget widget, void *id);
static void callHighlightCallback(Widget widget, void *id);
static SWND findMainWindow(Widget widget);
static void appendPair(STABLE table, const char *key, const char *value);

static SFONT getFont(const char *fontspec);

static void setFontPath(char *path, int special, int before, int after);

static char *fontPath = NULL;

void swSetFontPath(char *path)
{
    fontPath = path;
}

static SWindow *newSWindow();
static SMenu *newSMenu();
static SBitmap *newSBitmap();
static SCursor *newSCursor();
static STimer *newSTimer(int timeout, TimerCallback cb, void *data);
static SDContext *newSDContext();

static const char *translations = "             \
         <BtnDown>:     DrawingAreaInput()      \n\
         <BtnUp>:       DrawingAreaInput()      \n\
         <KeyDown>:     DrawingAreaInput()      \n\
         <KeyUp>:       DrawingAreaInput()      \n\
         <Motion>:      DrawingAreaInput()      \n";

static Pixmap cursorNonePixmap, cursorNoneMask;
static XColor cursorNoneColor;

static int truecolor_mode=0;

static int start_aqua_X11=0;

static Bool direct_rendering = GL_TRUE;

void swSetInDirectRendering(void) {
    direct_rendering = GL_FALSE;
}

#define USE_SW_COLOR_SHIFT \
    static struct swColorShift c; \
    static int initalized=0; \
    if (initalized==0) { \
        swSetupTrueColor(&c); \
        initalized=1; \
    }

void set_start_aqua_X11(void)
{
    start_aqua_X11=1;
}

unsigned long swX11ColorBits(int color,int bits)
{
    unsigned long ret = color;
    if (bits < 8)
       ret = color >> (8 - bits);
    return ret;
}

unsigned long swX11Color8Bits(int color,int bits)
{
    unsigned long ret = color;
    if (bits < 8)
       ret = color << (8 - bits);
    return ret;
}

int swGetColorMaskShift(unsigned long mask) 
{
    int shift = 0; 
    while ((mask & 1) == 0) { 
       shift++; 
       mask >>= 1;
    }
    return shift;
}

int swGetColorMaskBits(unsigned long mask) 
{
    int bits = 0; 
    while ((mask & 1) == 0) { 
       mask >>= 1;
    }
    while ((mask & 1) == 1) { 
       bits++; 
       mask >>= 1;
    }
    return bits;
}

static void swSetupTrueColor(struct swColorShift* colorshift)
{
    colorshift->rshift = swGetColorMaskShift(TheVisual->red_mask);
    colorshift->rbits  = swGetColorMaskBits(TheVisual->red_mask);

    colorshift->gshift = swGetColorMaskShift(TheVisual->green_mask);
    colorshift->gbits  = swGetColorMaskBits(TheVisual->green_mask);

    colorshift->bshift = swGetColorMaskShift(TheVisual->blue_mask);
    colorshift->bbits  = swGetColorMaskBits(TheVisual->blue_mask);
}

unsigned long swRGBToX11Color(int red, int green, int blue)
{
    USE_SW_COLOR_SHIFT
    return
      ((swX11ColorBits(red,c.rbits)   << c.rshift) & TheVisual->red_mask) | 
      ((swX11ColorBits(green,c.gbits) << c.gshift) & TheVisual->green_mask) | 
      ((swX11ColorBits(blue,c.bbits)  << c.bshift) & TheVisual->blue_mask);
}

int swX11ColorToRed8Bit(unsigned long color_pixel)
{
    USE_SW_COLOR_SHIFT
    return swX11Color8Bits((color_pixel & TheVisual->red_mask) >> c.rshift, 
                           c.rbits);
}

int swX11ColorToGreen8Bit(unsigned long color_pixel)
{
    USE_SW_COLOR_SHIFT
    return swX11Color8Bits((color_pixel & TheVisual->green_mask) >> c.gshift, 
                           c.gbits);
}

int swX11ColorToBlue8Bit(unsigned long color_pixel)
{
    USE_SW_COLOR_SHIFT
    return swX11Color8Bits((color_pixel & TheVisual->blue_mask) >> c.bshift, 
                           c.bbits);
}

int swX11ColorToSwColor(unsigned long color_pixel)
{
    return SW_RGB(swX11ColorToRed8Bit(color_pixel), 
                  swX11ColorToGreen8Bit(color_pixel), 
                  swX11ColorToBlue8Bit(color_pixel));
}

unsigned long swColorToX11Color(int color)
{
    int red, green, blue;
    red = (color & 0x000000ff);
    green = (color & 0x0000ff00) >> 8;
    blue = (color & 0x00ff0000) >> 16;

    return swRGBToX11Color(red,green,blue);
}

/* swInitialize gives back a flag if stereo is available */

extern int
swInitialize(int *argc, char **argv,int stereotype)
{
    XrmDatabase db;
    char bogus_bits[1] = { 0 };
    XColor fg, bg;
    Pixmap copyMask, linkMask, errorPixmap, errorMask;
    int i;
    int attrib,firstattrib;
    int stereoret=NO_STEREO;

    int legal_depth_list[] = { 32, 24, 16, 15, 8, 4, 1 };

    int attribs[] = {
        GLX_DEPTH_SIZE,  32,
        GLX_RED_SIZE,     8,
        GLX_ALPHA_SIZE,   8,
        GLX_DOUBLEBUFFER, GL_TRUE,
#ifndef HAVE_NO_STEREO
        GLX_STEREO,       GL_TRUE,
#endif
        GLX_RGBA,         GL_TRUE,
        GLX_STENCIL_SIZE,  8,                       
        0
    };

    int attribs1[] = {
        GLX_DEPTH_SIZE,   32,
        GLX_RED_SIZE,      8,
        GLX_ALPHA_SIZE,    8,
        GLX_DOUBLEBUFFER,  GL_TRUE,
        GLX_RGBA,          GL_TRUE,
        /* placeholders for GLX_ACCUM_RED/BLUE/GREEN_SIZE */
        0,                 0,                       
        0,                 0,                       
        0,                 0,
        GLX_STENCIL_SIZE,  8,                       
        0
    };

    int attribs2[] = {
        GLX_DEPTH_SIZE,   16,
        GLX_RED_SIZE,      8,
        GLX_RGBA,          GL_TRUE,
        /* placeholders for GLX_ACCUM_RED/BLUE/GREEN_SIZE */
        0,                 0,                       
        0,                 0,                       
        0,                 0,                       
        GLX_STENCIL_SIZE,  8,                       
        0
    };

    int attribs3[] = {
        GLX_DEPTH_SIZE,   16,
        GLX_RGBA,         GL_TRUE,
        GLX_STENCIL_SIZE, 8,                       
        0
    };

    int attribs4[] = {
        GLX_DEPTH_SIZE,   16,
        GLX_RGBA,         GL_FALSE,
        0
    };
    int errorflag = 0;
    const char *fontname = NULL;
    int fontsize = -1;
#ifdef HAVE_X11STARTCOMMAND
    struct stat fstat;

    if (lstat("/opt/X11/bin/Xquartz", &fstat) != 0)
        system("osascript -e 'display dialog \"you need to install (free) XQuartz (https://www.xquartz.org/)\"'");
#endif

    XInitThreads();

    XtToolkitInitialize ();
    TheAppContext = XtCreateApplicationContext();
#ifdef HAVE_X11STARTCOMMAND
    if (getenv("DISPLAY") == NULL) {
        if (start_aqua_X11) {
            fprintf(stderr,"$DISPLAY unset, using \":0\"\n");
            putenv("DISPLAY=:0");
        } else {
            system("osascript -e 'display dialog \"you need to install (free) XQuartz (https://www.xquartz.org/)\"'");
            fprintf(stderr,"$DISPLAY unset, unable to start X11 application\n");
            fprintf(stderr,"either use the \"-startX11aqua\" option\n");
            fprintf(stderr,"or ");
            fprintf(stderr,"start X11 and use e.g. ");
            fprintf(stderr,"for csh: \"setenv DISPLAY :0\" or for sh: \"export DISPLAY=:0\" \n");
             swCleanup();
            exit(11);               
        }
    }   
#endif

    TheDisplay = XtOpenDisplay(TheAppContext, NULL, NULL, "ProgramClass",
                               NULL, 0, argc, argv);

    if (!TheDisplay) {
#ifdef HAVE_X11STARTCOMMAND
        if (start_aqua_X11) {
            int pid;
            pid=fork();
            if (pid==-1)
                perror("fork: ");
            else if (pid==0) {
                int i;
                char** startcommand;
                int numCommands=7;
                setsid();
                startcommand=(char**)malloc(sizeof(char*)*numCommands);
                startcommand[0]=HAVE_X11STARTCOMMAND;
                /* "panther" */
                startcommand[1]="/Applications/Utilities/X11.app/Contents/MacOS/X11";
                /* "jaguar" */
                startcommand[2]="/Applications/X11.app/Contents/MacOS/X11";
                startcommand[3]="open /Applications/Utilities/X11.app";
                startcommand[4]="open /Applications/X11.app";
                startcommand[5]="/usr/X11R6/bin/X";
                startcommand[6]="X";
                for (i=0;i<numCommands;i++) 
                    if ((i==0) || 
                        (strcmp(startcommand[i],HAVE_X11STARTCOMMAND) != 0)) { 
                        fprintf(stderr,"Security warning:");
                        fprintf(stderr," try to start on console:\n%s\n",
                                startcommand[i]);
                        if (system(startcommand[i]) == 0)
                            exit(0);
                        else {
                            system("osascript -e 'display dialog \"you need to install (free) XQuartz (https://www.xquartz.org/)\"'");
                            perror(startcommand[i]);
                        } 
                   }
                exit(11);  
            } else {
                int i;
                /* wait for X11 startup */
                int seconds = 30;
                for (i = 0; i < seconds; i++) {
                    errorflag = 0;
                    sleep(1); 
                    TheDisplay = XtOpenDisplay(TheAppContext, NULL, NULL,
                                               "ProgramClass", NULL, 0, 
                                               argc, argv);
                    if (!TheDisplay) {
                        system("osascript -e 'display dialog \"you need to install (free) XQuartz (https://www.xquartz.org/)\"'");
                        errorflag = 1;
                    } else
                        break;
                }
                if (errorflag == 1) {
                    fprintf(stderr, "can't open display: %s\n", getenv("DISPLAY"));
                    fprintf(stderr, "after trying to start X11.app and wait %d seconds\n",
                            seconds),
                    swCleanup();
                    system("osascript -e 'display dialog \"you need to install (free) XQuartz (https://www.xquartz.org/)\"'");
                    exit(11);
                }
            }
        } else
            errorflag = 1;
#else 
        errorflag = 1;
#endif 
    }
    if (errorflag == 1) {
        if (getenv("DISPLAY") == NULL)
            fprintf(stderr, "can't open display\n");
        else
            fprintf(stderr, "can't open display: %s\n", getenv("DISPLAY"));
        swCleanup();
        exit(11);
    }

    /* 
     *      Try different color depths from legal_depth_list 
     *      Try a few different attributes 
     *      e.g. try GLX_DOUBLEBUFFER attribute, which is not supported by
     *      all machines
     *      use best attributes first
     */

    truecolor_mode = 1;
    TheVisual = NULL;
    /* check for stereo visual */
    if (stereotype == QUAD_BUFFER_STEREO)
        firstattrib = 0;
    else
        firstattrib = 1;
    for (attrib = firstattrib; attrib < 5; attrib++) {
        int idepth;
        for (idepth = 0; idepth < sizeof(legal_depth_list) / sizeof(int);
             idepth++) {
            int alpha;
            for (alpha = 1; alpha >= 0; alpha--) {
                int redsize;
                for (redsize = 8; redsize >= 4; redsize--) {
                    int* attribs_pointer = attribs;
                    if (attrib == 1)
                        attribs_pointer = attribs1;
                    if (attrib == 2)
                        attribs_pointer = attribs2;
                    if (attrib == 3)
                        attribs_pointer = attribs3;
                    if (attrib == 4) {
                        attribs_pointer = attribs4;
                        truecolor_mode = 0;
                    }
                    attribs_pointer[1] = legal_depth_list[idepth];
                    if ((attrib == 0) || (attrib == 1)) {
                        attribs_pointer[3] = redsize;
                        if (alpha)
                            attribs_pointer[5] = redsize;
                        else
                            attribs_pointer[5] = 0;
                    }
                    if (((attrib == 1) || (attrib == 2)) && 
                        ((stereotype == RED_GREEN_ANAGLYPH_STEREO) || 
                         (stereotype == GREEN_RED_ANAGLYPH_STEREO))) {
                        attribs1[10] = GLX_ACCUM_RED_SIZE;
                        attribs1[11] = redsize;
                        attribs1[12] = GLX_ACCUM_GREEN_SIZE;
                        attribs1[13] = redsize;
                    }
                    if (((attrib == 1) || (attrib == 2)) && 
                        ((stereotype == RED_BLUE_ANAGLYPH_STEREO) || 
                         (stereotype == BLUE_RED_ANAGLYPH_STEREO))) {
                        attribs2[6] = GLX_ACCUM_RED_SIZE;
                        attribs2[7] = redsize;
                        attribs2[8] = GLX_ACCUM_BLUE_SIZE;
                        attribs2[9] = redsize;
                    }
                    if (((attrib == 1) || (attrib == 2)) && 
                        ((stereotype == RED_CYAN_ANAGLYPH_STEREO) || 
                         (stereotype == CYAN_RED_ANAGLYPH_STEREO))) {
                        attribs2[6] = GLX_ACCUM_RED_SIZE;
                        attribs2[7] = GLX_ACCUM_GREEN_SIZE;
                        attribs2[8] = GLX_ACCUM_BLUE_SIZE;
                        attribs2[9] = redsize;
                    }
                    TheVisual = glXChooseVisual(TheDisplay, 
                                                DefaultScreen(TheDisplay),
                                                attribs_pointer);
                    if (TheVisual) {
                        if (attrib == 0) {
#ifdef HAVE_STEREOCOMMAND
                            system(HAVE_STEREOCOMMAND);             
#endif
#ifdef HAVE_NO_STEREO
                            stereoret = NO_STEREO;
#else
                            stereoret = QUAD_BUFFER_STEREO;
#endif
                        }
                        if (((attrib == 1) || (attrib == 2)) && 
                            ((stereotype == RED_GREEN_ANAGLYPH_STEREO) || 
                            (stereotype == GREEN_RED_ANAGLYPH_STEREO) || 
                            (stereotype == RED_BLUE_ANAGLYPH_STEREO) || 
                            (stereotype == BLUE_RED_ANAGLYPH_STEREO) ||
                            (stereotype == RED_CYAN_ANAGLYPH_STEREO) || 
                            (stereotype == CYAN_RED_ANAGLYPH_STEREO))) 
                            stereoret = stereotype;
                        break;
                    }
                }
                if (TheVisual) break;
            }
            if (TheVisual) break;
        }
        if (TheVisual) break;
    }
    if (!TheVisual) {
        fprintf(stderr, "couldn't find an appropriate visual for OpenGL, bailing\n");
        swCleanup();
        exit(1);
    }

    if (truecolor_mode) { 
       XStandardColormap *stdColorMap = NULL;
       int i, numCmaps = 0;
       truecolor_mode=0;

#ifdef HAVE_INCLUDE_X11_XMU       
       if (XmuLookupStandardColormap(TheDisplay, TheVisual->screen, 
                                     TheVisual->visualid, TheVisual->depth, 
                                     XA_RGB_DEFAULT_MAP, False, True) !=0) 
#endif
          if (XGetRGBColormaps(TheDisplay, DefaultRootWindow(TheDisplay),
                                    &stdColorMap, &numCmaps, 
                                    XA_RGB_DEFAULT_MAP) != 0) 
             if (stdColorMap != NULL) {
                for (i = 0; i < numCmaps; i++)
                   if (stdColorMap[i].visualid == TheVisual->visualid) {
                      TheColormap = stdColorMap[i].colormap;
                      stdColorMap = NULL;
                      if ( (TheVisual->visual->class==TrueColor ) ||
                           (TheVisual->visual->class==DirectColor ) )
                          truecolor_mode=1;
                      break;
                   }
                XFree(stdColorMap);
             }
             
       if (truecolor_mode==0)
          fprintf(stderr,"no true/directcolor visual, %s\n",
                         "fall back to slow color modus 8-(");
    }
    if (truecolor_mode==0)
        TheColormap = XCreateColormap(TheDisplay,
                                      DefaultRootWindow(TheDisplay),
                                      TheVisual->visual, AllocNone);

    if (fontPath != NULL) {
        setFontPath(fontPath, 0, 0, 1);
        setFontPath("rehash", 1, 0, 0);
    }

    if (defaultFontSpec != NULL) {
        fontname = defaultFontSpec;
        TheDefaultFont = getFont(defaultFontSpec);
    } else {
#ifdef HAVE_OLPC
        TheDefaultFont = getFont("-*-*-*-r-*-*-*-160-*-*-*-*-*-*");
        if (TheDefaultFont == NULL) {
            fontname = "*";
            fontsize = 17;
            TheDefaultFont = swFindFont(fontname, SW_PLAIN, fontsize);
        }
#else
        fontname = "Fixed";
        fontsize = 15;
        TheDefaultFont = swFindFont(fontname, SW_PLAIN, fontsize);
#endif
    }
    if (TheDefaultFont == NULL) {
        fprintf(stderr, "font %s ", fontname);
        if (fontsize != -1) 
            fprintf(stderr, "size %d ", fontsize);
        fprintf(stderr, "not found, trying \"fixed\"\n");
        fprintf(stderr, "check if you can use the \"-fn\" option %s",
                        " (e.g. \"-fn 10x20\")\n");
        fontsize = -1; 
        TheDefaultFont = getFont("fixed");
        if (TheDefaultFont == NULL) {
            fprintf(stderr, "font \"fixed\" not found, trying any font\n");
            TheDefaultFont = getFont("-*-*-*-*-*-*-*-*-*-*-*-*-*-*");
        }
    }

    WM_DELETE_WINDOW = XmInternAtom(TheDisplay, (String) "WM_DELETE_WINDOW",
                                    False);

    db = XtDatabase(TheDisplay);

    XrmPutStringResource(&db, "*XmDrawingArea.translations", translations);
    XrmPutStringResource(&db, "Dune*background", "#C0C0C0");
    XrmPutStringResource(&db, "Dune*XmTextField.foreground", "Black");
    XrmPutStringResource(&db, "Dune*XmTextField.background", "White");
    XrmPutStringResource(&db, "Dune*XmTextField.borderWidth", "0");

    memset(_keycodeMap, 0, 65536 * sizeof(int));
    _keycodeMap[Button1] = SW_MOUSE1;
    _keycodeMap[Button2] = SW_MOUSE2;
    _keycodeMap[Button3] = SW_MOUSE3;
    _keycodeMap[Button4] = SW_MOUSE4;
    _keycodeMap[Button5] = SW_MOUSE5;
    _keycodeMap[XK_Up] = SW_UP;
    _keycodeMap[XK_Down] = SW_DOWN;
    _keycodeMap[XK_Left] = SW_KEY_LEFT;
    _keycodeMap[XK_Right] = SW_KEY_RIGHT;
    _keycodeMap[XK_Delete] = SW_DELETE;
    for (i = 0x40; i < 0x80; i++) {
         _keycodeMap[i] = i;
    }
    for (i = 0; i < 65536; i++) {
         _invKeycodeMap[_keycodeMap[i]] = i;
    }

    WindowCapacity = 16;
    WindowsToUpdate = malloc(WindowCapacity * sizeof(SWND));

    TheCopyPixmap = XCreateBitmapFromData(TheDisplay,
                                          DefaultRootWindow(TheDisplay),
                                          copy_bits, copy_width, copy_height);

    copyMask = XCreateBitmapFromData(TheDisplay,
                             DefaultRootWindow(TheDisplay),
                             copy_mask_bits, copy_mask_width, copy_mask_height);

    TheLinkPixmap = XCreateBitmapFromData(TheDisplay,
                                          DefaultRootWindow(TheDisplay),
                                          link_bits, link_width, link_height);

    linkMask = XCreateBitmapFromData(TheDisplay,
                             DefaultRootWindow(TheDisplay),
                             link_mask_bits, link_mask_width, link_mask_height);

    TheMovePixmap = XCreateBitmapFromData(TheDisplay,
                                          DefaultRootWindow(TheDisplay),
                                          bogus_bits, 1, 1);

    errorPixmap = XCreateBitmapFromData(TheDisplay,
                                        DefaultRootWindow(TheDisplay),
                                        error_bits, error_width, error_height);

    errorMask = XCreateBitmapFromData(TheDisplay,
                                      DefaultRootWindow(TheDisplay),
                                      error_mask_bits, 
                                      error_mask_width, error_mask_height);

    cursorNonePixmap = XCreateBitmapFromData(TheDisplay, 
                                             DefaultRootWindow(TheDisplay),
                                             cursor_none_bits, 
                                             cursor_none_width, 
                                             cursor_none_height);
    cursorNoneMask = XCreateBitmapFromData(TheDisplay, 
                                           DefaultRootWindow(TheDisplay),
                                           cursor_none_bits, 
                                           cursor_none_width, 
                                           cursor_none_height);

    fg.red = fg.green = fg.blue = 0;
    bg.red = bg.green = bg.blue = 65535;
    cursorNoneColor.red = cursorNoneColor.green = cursorNoneColor.blue = 0;
    if (truecolor_mode) {
        fg.pixel=swRGBToX11Color(fg.red>>8,fg.green>>8,fg.blue>>8);
        bg.pixel=swRGBToX11Color(bg.red>>8,bg.green>>8,bg.blue>>8);
        cursorNoneColor.pixel=swRGBToX11Color(cursorNoneColor.red>>8,
                                              cursorNoneColor.green>>8,
                                              cursorNoneColor.blue>>8);
    } else { 
        XAllocColor(TheDisplay, TheColormap, &fg);
        XAllocColor(TheDisplay, TheColormap, &bg);
        XAllocColor(TheDisplay, TheColormap, &cursorNoneColor);
    }

#ifndef MOTIF_DRAGNDROP
    TheCopyCursor = XCreatePixmapCursor(TheDisplay, TheCopyPixmap, copyMask,
                                        &fg, &bg, copy_hot_x, copy_hot_y);
    TheLinkCursor = XCreatePixmapCursor(TheDisplay, TheLinkPixmap, linkMask,
                                        &fg, &bg, link_hot_x, link_hot_y);
    TheMoveCursor = None;
    TheErrorCursor = XCreatePixmapCursor(TheDisplay, errorPixmap, errorMask,
                                         &fg, &bg, error_hot_x, error_hot_y);
#endif

    return stereoret;
}

extern int
swMainLoop(void)
{
    initCallback();

    _running = TRUE;
    while (_running)
    {
         XtAppProcessEvent(TheAppContext, XtIMAll);
         if (XtAppPending(TheAppContext) == 0) {
             update();
         }
    }
    return 0;
}

extern void
swExit(void)
{
    _running = FALSE;
    XFreePixmap(TheDisplay, TheCopyPixmap);
    XFreePixmap(TheDisplay, TheLinkPixmap);
    XFreePixmap(TheDisplay, TheMovePixmap);

#ifndef MOTIF_DRAGNDROP
    XFreeCursor(TheDisplay, TheCopyCursor);
    XFreeCursor(TheDisplay, TheLinkCursor);
    XFreeCursor(TheDisplay, TheErrorCursor);
#endif
    
    swCleanup();
/*    XtDestroyApplicationContext(TheAppContext); */
}

CleanupCallback cleanup=NULL;

extern void     swSetCleanup(CleanupCallback cb)
{
    cleanup=cb;
}

extern void     swCleanup(void)
{
    if (cleanup!=NULL)
       cleanup();
}

extern void
swGetScreenSize(int *width, int *height)
{
    int x, y;
    unsigned int w, h, border_width, depth;
    Window root;

    XGetGeometry(TheDisplay, DefaultRootWindow(TheDisplay), &root, &x, &y,
                 &w, &h, &border_width, &depth);
    *width = (int) w;
    *height = (int) h;
}

extern void
swSetDefaultIcon(int resource)
{
}

extern int
swModalLoop(void)
{
    int minDepth = _dialogDepth;
    _dialogDepth++;
    while (_dialogDepth > minDepth)
    {
         XtAppProcessEvent(TheAppContext, XtIMAll);
         if (XtAppPending(TheAppContext) == 0) {
             update();
         }
    }
    return _dialogRC;
}

extern void
swExitModalLoop(int rc)
{
    _dialogDepth--;
    _dialogRC = rc;
}

static InitCallback initialCallback = NULL;

void initCallback(void)
{
    if (initialCallback)
        initialCallback();
}

extern void
swSetInitCallback(InitCallback callback)
{
    initialCallback = callback;
}

extern void
swSetExposeCallback(SWND wnd, ExposeCallback exposeCallback)
{
    if (!wnd) return;

    wnd->exposeCallback = exposeCallback;
    if (exposeCallback) {
         XtVaSetValues(wnd->widget, XmNbackgroundPixmap, None, NULL);
    }
}

extern void
swSetMouseCallback(SWND wnd, MouseCallback mouseCallback)
{
    if (!wnd) return;

    wnd->mouseCallback = mouseCallback;
}

extern void
swSetKeyCallback(SWND wnd, KeyCallback keyCallback)
{
    if (!wnd) return;

    wnd->keyCallback = keyCallback;
}

extern void
swSetFocusCallback(SWND wnd, FocusCallback focusCallback)
{
    if (!wnd) return;

    wnd->focusCallback = focusCallback;
}

extern void
swSetResizeCallback(SWND wnd, ResizeCallback resizeCallback)
{
    if (!wnd) return;

    wnd->resizeCallback = resizeCallback;
}

extern void
swSetEnterCallback(SWND wnd, EnterCallback enterCallback)
{
    if (!wnd) return;

    wnd->enterCallback = enterCallback;
}

extern void
swSetScrollBarCallback(SWND wnd, ScrollBarCallback scrollBarCallback)
{
    if (!wnd) return;

    wnd->scrollBarCallback = scrollBarCallback;
}

extern void
swSetCommandCallback(SWND wnd, CommandCallback cb)
{
    if (!wnd) return;

    wnd->commandCallback = cb;
}

extern void
swSetHighlightCallback(SWND wnd, CommandCallback cb)
{
    if (!wnd) return;

    wnd->highlightCallback = cb;
}

extern void
swCallCommandCallback(SWND wnd, void *id)
{
    callCommandCallback(wnd->widget, id);
}

extern void
swCallHighlightCallback(SWND wnd, void *id)
{
    callHighlightCallback(wnd->widget, id);
}

extern void
swSetCloseCallback(SWND wnd, CloseCallback cb)
{
    wnd->closeCallback = cb;
}

extern void
swSetClientData(SWND wnd, void *data)
{
    if (!wnd) return;
    wnd->data = data;
}

static void deleteFromWindowsToUpdate(SWND wnd)
{
    int i;
    for (i = 0; i < NumWindowsToUpdate; i++)
         if (WindowsToUpdate[i] == wnd) {
             WindowsToUpdate[i] = NULL;
         }
}

extern void
swDestroyWindow(SWND wnd)
{
    if (!wnd) return;

    swDeleteCallbacks(wnd);

    if (wnd->menu) XtDestroyWidget(wnd->menu->widget);

    if (wnd->shell) {
         XtDestroyWidget(wnd->shell);
    } else if (wnd->mainWnd) {
         XtDestroyWidget(wnd->mainWnd);
    } else if (wnd->widget) {
         XtDestroyWidget(wnd->widget);
    }

    deleteFromWindowsToUpdate(wnd);
}

/*********************/
/* window attributes */
/*********************/

extern void
swGetPosition(SWND wnd, int *x, int *y)
{
    Position px, py;

    if (!wnd) return;

    XtVaGetValues(wnd->shell ? wnd->shell : wnd->widget,
                  XmNx, &px, XmNy, &py, NULL);
    *x = px;  
    *y = py;
}

extern void
swGetAbsPosition(SWND wnd, int *x, int *y)
{
    Position wx, wy;

    if (!wnd) return;

    XtTranslateCoords(wnd->widget, 0, 0, &wx, &wy);
    *x = wx;
    *y = wy;
}

extern void
swSetPosition(SWND wnd, int x, int y)
{
    if (!wnd) return;

    XtMoveWidget(wnd->shell ? wnd->shell : wnd->widget, x, y);
}

extern void
swGetSize(SWND wnd, int *width, int *height)
{
    Dimension w, h;

    if (!wnd) return;

    XtVaGetValues(wnd->widget, XmNwidth, &w, XmNheight, &h, NULL);
    *width = w;
    *height = h;
}

extern void
swGetTotalSize(SWND wnd, int *width, int *height)
{
    Dimension w, h;

    if (!wnd) return;

    XtVaGetValues(wnd->shell, XmNwidth, &w, XmNheight, &h, NULL);
    *width = w;
    *height = h;
}

extern void
swSetSize(SWND wnd, int width, int height)
{
    Widget widget;
    Dimension borderWidth;

    if (!wnd) return;

    widget = wnd->shell ? wnd->shell : wnd->widget;
    XtVaGetValues(widget, XmNborderWidth, &borderWidth, NULL);
    XtResizeWidget(widget, (Dimension) width, (Dimension) height, borderWidth);
}

extern void
swMoveWindow(SWND wnd, int x, int y, int width, int height)
{
    Widget widget;
    Dimension borderWidth;

    if (!wnd) return;

    widget = wnd->shell ? wnd->shell : wnd->widget;
    XtVaGetValues(widget, XmNborderWidth, &borderWidth, NULL);
    XtConfigureWidget(widget, x, y,
                      (Dimension) width, (Dimension) height, borderWidth);
}

extern void
swGetTitle(SWND wnd, char *buf, int len)
{
    String str;
    if (!wnd || !wnd->widget) return;

    str = XtName(wnd->widget);
    strncpy(buf, str, len);
}

extern void
swSetTitle(SWND wnd, const char *title)
{
    if (!wnd || !wnd->shell) return;

    XtVaSetValues(wnd->shell, XmNtitle, title, NULL);
}

extern void
swSetIcon(SWND wnd, SBITMAP bitmap, SBITMAP mask, int big)
{
    if (!bitmap || !mask || !wnd || !wnd->shell) return;

    if (big) {
         XtVaSetValues(wnd->shell, XmNiconPixmap, bitmap->pixmap,
                       XmNiconMask, mask->pixmap, NULL);
    }
}

extern SWND
swGetParent(SWND wnd)
{
    Widget p;
    SWND pwnd;

    if (!wnd) return NULL;
    
    if (wnd->mainWnd) return NULL;

    for (p = XtParent(wnd->widget); p != NULL; p = XtParent(p)) {
         XtVaGetValues(p, XmNuserData, &pwnd, NULL);
         if (pwnd != NULL) return pwnd;
    }
    return NULL;
}

extern int
swGetWindowColor(SWND wnd, int type)
{
    XColor c;
    
    if (!wnd) return 0;

    switch (type) {
      case SW_COLOR_WINDOW_FG:
      case SW_COLOR_TEXT:
         XtVaGetValues(wnd->widget, XmNforeground, &c.pixel, NULL);
         break;
      case SW_COLOR_WINDOW_BG:
      case SW_COLOR_FACE:
      case SW_COLOR_HIGHLIGHT_TEXT:
         XtVaGetValues(wnd->widget, XmNbackground, &c.pixel, NULL);
         break;
      case SW_COLOR_TSHADOW:
         XtVaGetValues(wnd->widget, XmNtopShadowColor, &c.pixel, NULL);
         break;
      case SW_COLOR_BSHADOW:
         XtVaGetValues(wnd->widget, XmNbottomShadowColor, &c.pixel, NULL);
         break;
      case SW_COLOR_HIGHLIGHT:
/* XmNhighlightColor is actually the focus rect color (isn't motif wonderful) */
         XtVaGetValues(wnd->widget, XmNhighlightColor, &c.pixel, NULL);
         return 0x00FF3030;
      default:
         return 0;
    }

    if (truecolor_mode)
        return swX11ColorToSwColor(c.pixel);
    else {
        XQueryColor(TheDisplay, TheColormap, &c);
        return SW_RGB((c.red >> 8), (c.green >> 8), (c.blue >> 8));
    }
}

extern void
swShowWindow(SWND wnd)
{
    if (!wnd) return;

    if (wnd->mainWnd) {
         XtRealizeWidget(wnd->shell);
    } else if (wnd->shell) {
         XtPopup(wnd->shell, XtGrabNone);
    } else {
         XtManageChild(wnd->widget);
    }

    wnd->isHidden = FALSE;
}

extern void
swHideWindow(SWND wnd)
{
    if (!wnd) return;
    
    if (wnd->mainWnd) {
         XtUnrealizeWidget(wnd->shell);
    } else if (wnd->shell) {
         XtPopdown(wnd->shell);
    } else {
         XtUnmanageChild(wnd->widget);
    }

    wnd->isHidden = TRUE;
}

extern void
swIconifyWindow(SWND wnd)
{
    if (!wnd) return;

    XtVaSetValues(wnd->mainWnd, XmNiconic, True, NULL);
    XtVaSetValues(wnd->widget, XmNiconic, True, NULL);
    XtVaSetValues(wnd->shell, XmNiconic, True, NULL);

    XSync(TheDisplay, False);
}

extern void
swDeIconifyWindow(SWND wnd)
{
    if (!wnd) return;

    XtVaSetValues(wnd->shell, XmNinitialState, NormalState, NULL);
    XtVaSetValues(wnd->shell, XmNiconic, False, NULL);
    XtVaSetValues(wnd->widget, XmNinitialState, NormalState, NULL);
    XtVaSetValues(wnd->widget, XmNiconic, False, NULL);
    XtVaSetValues(wnd->mainWnd, XmNinitialState, NormalState, NULL);
    XtVaSetValues(wnd->mainWnd, XmNiconic, False, NULL);

    XSync(TheDisplay, False);
}

extern int
swIsHidden(SWND wnd)
{
    return wnd->isHidden;
}

extern void
swMaximizeWindow(SWND wnd)
{
}

extern void
swSetWindowBG(SWND wnd, int color)
{
    if (!wnd) return;

    if (truecolor_mode)
       XtVaSetValues(wnd->widget, XmNbackground, swColorToX11Color(color), NULL);
    else  {   
       XColor c;

       c.red = (color & 0x000000ff) << 8;
       c.green = (color & 0x0000ff00);
       c.blue = (color & 0x00ff0000) >> 8;
       XAllocColor(TheDisplay, TheColormap, &c);
       XtVaSetValues(wnd->widget, XmNbackground, c.pixel, NULL);
    }
}



extern int
swIsVisible(SWND wnd)
{
    if (!wnd) return FALSE;

    return XtIsManaged(wnd->widget);
}

extern int
swIsMaximized(SWND wnd)
{
    return FALSE;
}

extern void
swEnableWindow(SWND wnd, int flag)
{
    if (!wnd) return;

    XtSetSensitive(wnd->widget, flag);
}

extern void
swInvalidateRect(SWND wnd, int x, int y, int width, int height)
{

    int x2 = x + width - 1;
    int y2 = y + height - 1;

    if (!wnd) return;

    if (x < 0) x = 0;
    if (y < 0) y = 0;

    if (wnd->invx1 < 0) {
         if (NumWindowsToUpdate >= WindowCapacity) {
             WindowCapacity *= 2;
             WindowsToUpdate = (SWND *) realloc(WindowsToUpdate,
                                                WindowCapacity * sizeof(SWND));
         }
         WindowsToUpdate[NumWindowsToUpdate++] = wnd;
    }

    /* extend the invalidation region to include the new window */
    if (wnd->invx1 < 0 || x < wnd->invx1) wnd->invx1 = x;
    if (wnd->invy1 < 0 || y < wnd->invy1) wnd->invy1 = y;
    if (x2 > wnd->invx2) wnd->invx2 = x2;
    if (y2 > wnd->invy2) wnd->invy2 = y2;
}

extern void
swInvalidateWindow(SWND wnd)
{
    Dimension w, h;
    if (!wnd) return;

    XtVaGetValues(wnd->widget, XmNwidth, &w, XmNheight, &h, NULL);
    swInvalidateRect(wnd, 0, 0, w, h);
}

extern void
swSetFocus(SWND wnd)
{
    if (!wnd) return;

    XmProcessTraversal(wnd->widget, XmTRAVERSE_CURRENT);
}

extern void
swEnableAccelerators(int enable)
{
    acceleratorsEnabled = enable;
}

static int timerDisabled = 0;

extern void swEnableTimers(void)
{
    timerDisabled = 0;
}

extern void swDisableTimers(void)
{
    timerDisabled = 1;
}

extern STIMER
swSetTimer(SWND wnd, int timeout, TimerCallback cb, void *data)
{
    STimer *t = 0;

    if (timerDisabled)
        return 0;

    t = newSTimer(timeout, cb, data);

    t->id = XtAppAddTimeOut(TheAppContext, timeout, timerProc, t);
    return t;
}

extern void
swKillTimer(STIMER timer)
{
    if (!timer) return;

    XtRemoveTimeOut(timer->id);
    timer->id=0;
    free(timer);
}

/***************/
/* Main Window */
/***************/

extern SWND
swCreateMainWindow(const char *title, int x, int y, int width, int height)
{
    SWindow    *wnd = newSWindow();
    
    /* create a shell for the top-level window */

    wnd->shell = XtVaAppCreateShell(title, "ProgramClass",
                                    applicationShellWidgetClass, TheDisplay,
                                    XmNx, x,
                                    XmNy, y,
                                    XmNwidth, width,
                                    XmNheight, height,
                                    XmNdefaultFontList, 
                                    TheDefaultFont->fontList,
                                    XmNbuttonFontList, TheDefaultFont->fontList,
                                    XmNlabelFontList, TheDefaultFont->fontList,
                                    XmNtextFontList, TheDefaultFont->fontList,
                                    XmNvisual, TheVisual->visual,
                                    XmNdepth, TheVisual->depth,
                                    XmNcolormap, TheColormap,
                                    XmNdeleteResponse, XmDO_NOTHING,
/*                                  XmNdeleteResponse, XmUNMAP, */
                                    NULL);

    /* create icon of iconify */
    if (truecolor_mode) { 
        Pixmap dunePixmap;
        Pixmap duneMask;
        dunePixmap = XCreateBitmapFromData(TheDisplay,
                                           DefaultRootWindow(TheDisplay),
                                           dune_bits, dune_width, dune_height);
        duneMask = XCreateBitmapFromData(TheDisplay,
                                         DefaultRootWindow(TheDisplay),
                                         dune_mask_bits, 
                                         dune_mask_width, dune_mask_height);
        XtVaSetValues (wnd->shell, XmNiconPixmap, dunePixmap, XmNiconMask, 
                       duneMask, NULL);
    }


    /* set up a delete response */
    XmAddWMProtocolCallback(wnd->shell, WM_DELETE_WINDOW, onDeleteWindow, wnd);

    /* create a main window, for menus & such */
    wnd->mainWnd = XmCreateMainWindow(wnd->shell, (String) title, NULL, 0);
    XtManageChild(wnd->mainWnd);

    /* create a composite to hold all the children */
    wnd->widget = createContainer(title, x, y, width, height, wnd->mainWnd);
    initWidget(wnd->mainWnd, wnd);

    XmMainWindowSetAreas(wnd->mainWnd, NULL, NULL, NULL, NULL, wnd->widget);

    XtAddCallback(wnd->widget, XmNexposeCallback, expose, wnd);
    XtAddCallback(wnd->widget, XmNinputCallback, input, wnd);
    XtAddCallback(wnd->widget, XmNresizeCallback, resize, wnd);

    return wnd;
}

/**********/
/* Canvas */
/**********/

extern SWND
swCreateCanvas(const char *title, int x, int y, int width, int height,
                SWND parent)
{
    SWindow *w;

    if (!parent || !parent->widget) return NULL;

    w = newSWindow();
    w->widget = createContainer(title, x, y, width, height, parent->widget);
    XtAddEventHandler(w->widget, EnterWindowMask, False,
                      enterEvent, (XtPointer) w);
    XtAddEventHandler(w->widget, LeaveWindowMask, False,
                      enterEvent, (XtPointer) w);
    XtAddCallback(w->widget, XmNexposeCallback, expose, w);
    XtAddCallback(w->widget, XmNinputCallback, input, w);
    XtAddCallback(w->widget, XmNresizeCallback, resize, w);
    initWidget(w->widget, w);

    return (SWND) w;
}

/*********/
/* Popup */
/*********/

extern SWND
swCreatePopup(const char *title, int x, int y, int width, int height,
               SWND parent)
{
    SWindow     *s;

    if (!parent || !parent->widget) return NULL;

    s = newSWindow();
    s->shell = XtVaCreatePopupShell(title, overrideShellWidgetClass,
                                    parent->widget,
                                    XmNx, x,
                                    XmNy, y,
                                    XmNwidth, width,
                                    XmNheight, height,
                                    XmNvisual, TheVisual->visual,
                                    NULL);
    
    s->widget = createContainer(title, x, y, width, height, s->shell);
    initWidget(s->widget, s);

    return s;
}

/*************/
/* ScrollBar */
/*************/

extern SWND
swCreateScrollBar(int flags, int x, int y, int width, int height,
          int min, int max, int value, int visible,
          SWND parent)
{
    SWindow *sb;
    int orientation, showArrows;
    Widget w;
    if (!parent || !parent->widget) return NULL;

    sb = newSWindow();
    orientation = (flags & SW_HORIZONTAL) ? XmHORIZONTAL : XmVERTICAL;
    showArrows = !(flags & SW_NO_ARROWS);

    w = XtVaCreateManagedWidget("scrollbar",
                                xmScrollBarWidgetClass,
                                parent->widget,
                                XmNx, x,
                                XmNy, y,
                                XmNwidth, width,
                                XmNheight, height,
                                XmNminimum, min,
                                XmNmaximum, max,
                                XmNsliderSize, visible,
                                XmNvalue, value,
                                XmNorientation, orientation,
                                XmNshowArrows, showArrows,
                                NULL);

    XtAddCallback(w, XmNvalueChangedCallback, scrollCallback, sb);
    XtAddCallback(w, XmNdragCallback, scrollCallback, sb);
    XtAddCallback(w, XmNincrementCallback, scrollCallback, sb);
    XtAddCallback(w, XmNdecrementCallback, scrollCallback, sb);
    XtAddCallback(w, XmNpageIncrementCallback, scrollCallback, sb);
    XtAddCallback(w, XmNpageDecrementCallback, scrollCallback, sb);
    initWidget(w, sb);

    sb->widget = w;

    return sb;
}

extern void
swScrollBarSetValue(SWND sb, int value)
{
    if (!sb || !sb->widget) return;

    XtVaSetValues(sb->widget, XmNvalue, value, NULL);
}

extern void
swScrollBarSetRange(SWND sb, int min, int max)
{
    if (!sb || sb->widget) return;

    XtVaSetValues(sb->widget, XmNminimum, min, XmNmaximum, max, NULL);
}

extern void
swScrollBarSetVisible(SWND sb, int visible)
{
    if (!sb || !sb->widget) return;

    XtVaSetValues(sb->widget, XmNsliderSize, visible, NULL);
}

/**********/
/* Button */
/**********/

extern SWND
swCreateButton(const char *title, int x, int y, int width, int height,
                SWND parent)
{
    SWindow *b;
    char *newTitle;
    char mnemonic[2];
    
    if (!title || !parent || !parent->widget) return NULL;

    newTitle = malloc(strlen(title) + 1);
    parseMenu(title, newTitle, mnemonic);

    b = newSWindow();
    b->widget = XtVaCreateManagedWidget(newTitle, xmPushButtonWidgetClass,
                                        parent->widget,
                                        XmNx, x,
                                        XmNy, y,
                                        XmNwidth, width,
                                        XmNheight, height,
                                        NULL);

    XtAddCallback(b->widget, XmNactivateCallback, buttonCallback, b);
    initWidget(b->widget, b);

    free(newTitle);
    return b;
}

/************/
/* CheckBox */
/************/

extern SWND
swCreateCheckBox(const char *title, int x, int y, int width, int height,
         SWND parent)
{
    SWindow *b;
    char *newTitle;
    char mnemonic[2];

    if (!parent || !parent->widget) return NULL;

    newTitle = malloc(strlen(title) + 1);
    parseMenu(title, newTitle, mnemonic);

    b = newSWindow();
    b->widget = XtVaCreateManagedWidget(newTitle, xmToggleButtonWidgetClass,
                                        parent->widget,
                                        XmNx, x,
                                        XmNy, y,
                                        XmNwidth, width,
                                        XmNheight, height,
                                        XmNmarginWidth, 0,
                                        XmNmarginHeight, 0,
                                        XmNalignment, XmALIGNMENT_BEGINNING,
                                        NULL);

    XtAddCallback(b->widget, XmNvalueChangedCallback, buttonCallback, b);
    initWidget(b->widget, b);
    free(newTitle);

    return b;
}

extern void
swSetCheck(SWND wnd, int check)
{
    if (!wnd) return;

    XtVaSetValues(wnd->widget, XmNset, check, NULL);
}

extern int
swGetCheck(SWND wnd)
{
    Boolean check;
    if (!wnd) return FALSE;

    XtVaGetValues(wnd->widget, XmNset, &check, NULL);
    return check;
}

/***************/
/* RadioButton */
/***************/

extern SWND
swCreateRadioButton(const char *title, int x, int y, int width, int height,
            SWND parent)
{
    SWindow *b;
    char *newTitle;
    char mnemonic[2];

    if (!parent || !parent->widget) return NULL;
    newTitle = malloc(strlen(title) + 1);
    parseMenu(title, newTitle, mnemonic);

    b = newSWindow();
    b->widget = XtVaCreateManagedWidget(newTitle, xmToggleButtonWidgetClass,
                                        parent->widget,
                                        XmNx, x,
                                        XmNy, y,
                                        XmNwidth, width,
                                        XmNheight, height,
                                        XmNindicatorType, XmONE_OF_MANY,
                                        XmNalignment, XmALIGNMENT_BEGINNING,
                                        NULL);

    XtAddCallback(b->widget, XmNvalueChangedCallback, buttonCallback, b);
    initWidget(b->widget, b);
    free(newTitle);

    return b;
}

/************/
/* TextEdit */
/************/

static void passwdInput(Widget widget, XtPointer client_data, XtPointer data)
{
    int  len = 0;
    char *passwd = (char *) client_data;
    XmTextVerifyCallbackStruct *cbs = (XmTextVerifyCallbackStruct *) data;

    if (cbs->reason == XmCR_ACTIVATE) {
              return;
    }

    if (cbs->startPos < cbs->currInsert) {/* backspace */
        cbs->endPos = strlen (passwd); /* delete from here to end */
        passwd[cbs->startPos] = 0;
        /* backspace--terminate */
        return;
    }

    if ((cbs->endPos + cbs->text->length + 1) > PASSWORD_LENGTH)
        return;

    strncat (passwd, cbs->text->ptr, cbs->text->length);
    passwd[cbs->endPos + cbs->text->length] = 0;

    for (len = 0; len < cbs->text->length; len++)
        cbs->text->ptr[len] = '*';
 }

extern SWND
swCreateTextEdit(int flags, int x, int y, int width, int height, SWND parent)
{
    SWindow *t;
    int editable;
    Arg av[15];
    int ac = 0;

    if (!parent || !parent->widget) return NULL;

    t = newSWindow();
    t->password_data[0]=0;
    editable = !(flags & SW_NON_EDITABLE);

    XtSetArg(av[ac], XmNx, x);  ac++;
    XtSetArg(av[ac], XmNy, y);  ac++;
    XtSetArg(av[ac], XmNwidth, width);  ac++;
    XtSetArg(av[ac], XmNheight, height);  ac++;
    XtSetArg(av[ac], XmNborderWidth, 0);  ac++;
    XtSetArg(av[ac], XmNhighlightThickness, 0);  ac++;
    XtSetArg(av[ac], XmNshadowThickness, 0);  ac++;
    XtSetArg(av[ac], XmNeditable, editable);  ac++;
    XtSetArg(av[ac], XmNmarginHeight, 0);  ac++;
    XtSetArg(av[ac], XmNmarginWidth, 0);  ac++;

    if (flags & SW_SINGLE_LINE) {
         t->widget = XtCreateManagedWidget("", xmTextFieldWidgetClass,
                                           parent->widget, av, ac);
    } else {
         XtSetArg(av[ac], XmNeditMode, XmMULTI_LINE_EDIT);  ac++;
         t->widget = XtCreateManagedWidget("", xmTextWidgetClass,
                                           parent->widget, av, ac);
    }


    if (flags & SW_PASSWORD) {
#ifdef HAS_MLOCK
         mlock(t->password_data, PASSWORD_LENGTH);
#endif
         XtAddCallback(t->widget, XmNmodifyVerifyCallback, passwdInput,
                       t->password_data);
         XtAddCallback(t->widget, XmNactivateCallback, passwdInput, 
                       t->password_data);
    } else 
         XtAddCallback(t->widget, XmNactivateCallback, textActivateCallback, t);
    XtAddCallback(t->widget, XmNfocusCallback, textFocusCallback, t);
    XtAddCallback(t->widget, XmNlosingFocusCallback, textLosingFocusCallback, t);
    initWidget(t->widget, t);

    return t;
}

extern void
swSetText(SWND wnd, const char *text)
{
    if (!wnd || !text) return;

    XtVaSetValues(wnd->widget, XmNvalue, text, NULL);
}

extern void
swGetText(SWND wnd, char *text, int len)
{
    char *str;

    if (!wnd || !text) return;

    if (strlen(wnd->password_data) > 0) {
        strncpy(text, wnd->password_data, len);
    } else {
        str = XmTextFieldGetString(wnd->widget);
        strncpy(text, str, len);
        XtFree(str);
    }
}

extern void     
swSetTextCleanupPasswd(SWND wnd) 
{
    int i;
    /* remove password data from memory */
    for (i = 0; i < strlen(wnd->password_data); i++) 
        wnd->password_data[i] = (char) 0;
#ifdef HAS_MLOCK
    munlock(wnd->password_data, PASSWORD_LENGTH);
#endif
}

extern void
swTextEditSetSelection(SWND wnd, int start, int end)
{
    if (!wnd) return;

    XmTextFieldSetSelection(wnd->widget, start, end, CurrentTime);
}

extern int
swGetTextCaretPos(SWND wnd)
{
/*
    XmTextPosition textPosition;
*/
    if (!wnd) return -1;

/* 
    XtVaGetValues(wnd->widget, XmNcursorPosition, &textPosition, NULL);
    return (int)textPosition;
*/
    return XmTextFieldGetCursorPosition(wnd->widget);
}

extern void
swSetTextCaretPos(SWND wnd, int textPosition)
{
    if (!wnd) return;

/*    XtVaSetValues(wnd->widget, XmNcursorPosition, textPosition, NULL); */
    XmTextFieldSetCursorPosition(wnd->widget, textPosition);
    swInvalidateWindow(wnd);
}


/*********/
/* Label */
/*********/

extern SWND
swCreateLabel(const char *title, int x, int y, int width, int height,
               SWND parent)
{
    SWindow *l;
    char *newTitle;
    char mnemonic[2];

    if (!parent || !parent->widget) return NULL;

    l = newSWindow();
    newTitle = malloc(strlen(title) + 1);
    parseMenu(title, newTitle, mnemonic);
    l->widget = XtVaCreateManagedWidget(newTitle, xmLabelWidgetClass,
                                        parent->widget,
                                        XmNx, x,
                                        XmNy, y,
                                        XmNwidth, width,
                                        XmNheight, height,
                                        XmNalignment, XmALIGNMENT_BEGINNING,
                                        XmNmarginWidth, 0,
                                        XmNmarginHeight, 0,
                                        NULL);

    initWidget(l->widget, l);
    free(newTitle);
    return l;
}

extern void
swLabelSetText(SWND wnd, const char *text)
{
    XmString s;

    if (!wnd) return;

    s = XmStringCreateLocalized((String) text);
    XtVaSetValues(wnd->widget, XmNlabelString, s, NULL);
    XmStringFree(s);
}

/*******************/
/* Scrolled Window */
/*******************/

extern SWND
swCreateScrolledWindow(int x, int y, int width, int height, SWND parent)
{
    SWindow *w;

    if (!parent || !parent->widget) return NULL;

    w = newSWindow();
    w->widget = XtVaCreateManagedWidget("swScrolledWindow",
                                        xmScrolledWindowWidgetClass,
                                        parent->widget,
                                        XmNx, x, XmNy, y,
                                        XmNwidth, width,
                                        XmNheight, height,
                                        XmNscrollBarDisplayPolicy, XmAS_NEEDED,
                                        XmNscrollingPolicy, XmAUTOMATIC,
                                        NULL);

    initWidget(w->widget, w);
    return (SWND) w;
}

extern void
swScrolledWindowSetChild(SWND wnd, SWND child)
{
    Widget clipWindow;

    if (!wnd || !child) return;

    XtVaSetValues(wnd->widget, XmNworkWindow, child->widget, NULL);
    XtVaGetValues(wnd->widget, XmNclipWindow, &clipWindow, NULL);
    XtVaSetValues(clipWindow, XmNbackground, 0, NULL);
}

extern void
swGetScrollPosition(SWND wnd, int *x, int *y)
{
    Widget hsb, vsb;

    if (!wnd) return;

    XtVaGetValues(wnd->widget, XmNhorizontalScrollBar, &hsb, 
                               XmNverticalScrollBar, &vsb,
                               NULL);
    
    if (x) XtVaGetValues(hsb, XmNvalue, x, NULL);
    if (y) XtVaGetValues(vsb, XmNvalue, y, NULL);
}

extern void
swSetScrollPosition(SWND wnd, int x, int y)
{
    Widget hsb, vsb, workWindow;

    if (!wnd) return;

    XtVaGetValues(wnd->widget, XmNhorizontalScrollBar, &hsb, 
                               XmNverticalScrollBar, &vsb,
                               XmNworkWindow, &workWindow,
                               NULL);
    
    XtVaSetValues(hsb, XmNvalue, x, NULL);
    XtVaSetValues(vsb, XmNvalue, y, NULL);
    XtMoveWidget(workWindow, -x, -y);
}

extern void
swGetScrollViewportSize(SWND wnd, int *w, int *h)
{
    Widget clipWindow;
    Dimension width, height;

    if (!wnd || !w || !h) return;

    XtVaGetValues(wnd->widget, XmNclipWindow, &clipWindow, NULL);
    XtVaGetValues(clipWindow, XmNwidth, &width, XmNheight, &height, NULL);
    *w = width;
    *h = height;
}

extern void
swSetScrollSizes(SWND wnd, int width, int height)
{
    Widget workWindow;

    if (!wnd) return;

    if (width <= 0) width = 1;
    if (height <= 0) height = 1;

    XtVaGetValues(wnd->widget, XmNworkWindow, &workWindow, NULL);
    XtVaSetValues(workWindow, XmNwidth, width, XmNheight, height, NULL);
}

/********************/
/* Drawing Contexts */
/********************/

extern SDC
swCreateDC(SWND wnd)
{
    SDContext *dc;
    char dash_list[2] = { 5, 5};
    
    if (!wnd || !wnd->widget) return NULL;

    dc = newSDContext();

    dc->gc = XCreateGC(TheDisplay, XtWindow(wnd->widget), 0, NULL);
    dc->drawable = XtWindow(wnd->widget);
    XSetDashes(TheDisplay, dc->gc, 0, dash_list, 2);
    XSetFont(TheDisplay, dc->gc, TheDefaultFont->fs->fid);

    return dc;
}

extern SDC
swCreateBitmapDC(SDC dc, int width, int height)
{
    SDContext *cdc;
    char dash_list[2] = { 5, 5};
    
    if (!dc || !dc->drawable) return NULL;

    cdc = newSDContext();

    cdc->drawable = XCreatePixmap(TheDisplay,
                                  dc->drawable, width, height,
                                  TheVisual->depth);
    cdc->gc = XCreateGC(TheDisplay, cdc->drawable, 0, NULL);
    cdc->isBitmapDC = 1;
    XSetDashes(TheDisplay, cdc->gc, 0, dash_list, 2);
    XSetFont(TheDisplay, cdc->gc, TheDefaultFont->fs->fid);

    return cdc;
}

extern void
swDestroyDC(SDC dc)
{
    if (!dc) return;

    if (dc->isBitmapDC) XFreePixmap(TheDisplay, dc->drawable);
    XFreeGC(TheDisplay, dc->gc);
    free(dc);
}

/*************/
/* GLContext */
/*************/

extern SGLC
swCreateGLContext(SDC dc)
{
    SGLContext *c;

    if (!dc || !dc->drawable) return NULL;

    c = (SGLContext *) malloc(sizeof(SGLContext));
    c->glxContext = glXCreateContext(TheDisplay, TheVisual, NULL, 
                                     direct_rendering);
    return c;
}

extern void
swDestroyGLContext(SGLC context)
{
    if (!context) return;

    glXDestroyContext(TheDisplay, context->glxContext);
    free(context);
}

extern void
swMakeCurrent(SDC dc, SGLC context)
{
    if (!dc || !dc->drawable || !context || !context->glxContext) return;

    glXMakeCurrent(TheDisplay, dc->drawable, context->glxContext);
}

#define OS_WIDTH  1280
#define OS_HEIGHT 1024

#ifdef HAVE_MESA_OFF_SCREEN_RENDERING
/*
 Mesa off screen rendering functions of this #ifdef based on:
 *Demo of off-screen Mesa rendering
 *
 * See Mesa/include/GL/osmesa.h for documentation of the OSMesa functions.
 *
 * This program is in the public domain.
 *
 * Brian Paul
 */

extern SGLC
swOSCreateGLContext(int *argc, char **argv)
{
    SGLContext *c;

    c = (SGLContext *) malloc(sizeof(SGLContext));
#if OSMESA_MAJOR_VERSION * 100 + OSMESA_MINOR_VERSION >= 305
    /* specify Z, stencil, accum sizes */
    c->osMesaContext = OSMesaCreateContextExt(OSMESA_RGBA, 16, 0, 0, NULL);
#else
    c->osMesaContext = OSMesaCreateContext(OSMESA_RGBA, NULL);
#endif
    return c;
}

extern void
swOSDestroyGLContext(SGLC context)
{
    if (!context) return;

    OSMesaDestroyContext(context->osMesaContext);
    free(context);
}

extern void
swOSMakeCurrent(SGLC context)
{
    void *buffer;
    if (!context || !context->osMesaContext) return;

    /* Allocate the image buffer */
    buffer = malloc(OS_WIDTH * OS_HEIGHT * 4 * sizeof(GLubyte));

    /* Bind the buffer to the context and make it current */
    if (!OSMesaMakeCurrent(context->osMesaContext, buffer, GL_UNSIGNED_BYTE, 
                           OS_WIDTH, OS_HEIGHT )) {
        fprintf(stderr, "OSMesaMakeCurrent failed!\n");
        return;
    }
}

#else

extern SGLC
swOSCreateGLContext(int *argc, char**argv)
{
    SGLContext *c;
    int attribs[] = {
        GLX_DEPTH_SIZE,   16,
        GLX_RGBA,         GL_TRUE,
        GLX_RED_SIZE,     1,
        GLX_GREEN_SIZE,   1,                       
        GLX_BLUE_SIZE,    1,
        0
    };

    c = (SGLContext *) malloc(sizeof(SGLContext));
    if (getenv("DISPLAY") == NULL) {
        fprintf(stderr,"$DISPLAY unset, using \":0\"");
        fprintf(stderr," (X11 should run, at least Xvfb)\n");
        putenv("DISPLAY=:0");
    }
    XtToolkitInitialize ();
    TheAppContext = XtCreateApplicationContext();
    TheDisplay = XtOpenDisplay(TheAppContext, NULL, NULL, "ProgramClass", 
                               NULL, 0, argc, argv);
    if (!TheDisplay) {
        fprintf(stderr, "can't open display: %s\n", getenv("DISPLAY"));
    }

    TheVisual = NULL;
    TheVisual = glXChooseVisual(TheDisplay, DefaultScreen(TheDisplay),
                                attribs);

    if (!TheVisual) {
        fprintf(stderr, "couldn't find an appropriate visual for OpenGL, bailing\n");
        swCleanup();
        exit(1);
    }

    c->glxContext = glXCreateContext(TheDisplay, TheVisual,  NULL, False);
    return c;
}

extern void
swOSDestroyGLContext(SGLC context)
{
    if (!context) return;

    glXDestroyContext(TheDisplay, context->glxContext);
    free(context);
}

extern void
swOSMakeCurrent(SGLC context)
{
    Pixmap pixmap;
    GLXPixmap glxPixmap;

    if (!context || !context->glxContext) return;

    pixmap = XCreatePixmap(TheDisplay, RootWindow(TheDisplay, 
                           TheVisual->screen), OS_WIDTH, OS_HEIGHT, 
                           TheVisual->depth);
    glxPixmap = glXCreateGLXPixmap(TheDisplay, TheVisual, pixmap);
    if (!glXMakeCurrent(TheDisplay, glxPixmap, context->glxContext)) {
        printf("glXMesaMakeCurrent failed!\n");
        return;
    }
}
#endif

extern void
swSwapBuffers(SDC dc, SGLC context)
{
    if (!dc || !dc->drawable) return;

    glXSwapBuffers(TheDisplay, dc->drawable);
    if (!glXIsDirect(TheDisplay, context->glxContext)) glFinish();
}

extern int
swLoadGLFont(const char *name, int style, float *kernX, float *kernY)
{
    XFontStruct *fontInfo;
    Font id;
    unsigned int first, last;
    int base;

/* you may want to insert something like the following instead of FONT24
 * 
 *  "-Misc-Fixed-Medium-R-Normal--20-200-75-75-C-100-ISO8859-1"
 *  "-Sony-Fixed-Medium-R-Normal--24-170-100-100-C-120-ISO8859-1"
 *  "-adobe-helvetica-medium-r-normal--17-120-100-100-p-88-iso8859-1"
 *  if in doubt, use something like 
 *    xlsfonts -ll '*x2?'
 *  in the commandline, to find current fonts
 */
    fontInfo = NULL;
#ifdef FONT24
    fontInfo = XLoadQueryFont(TheDisplay,FONT24);
#endif
    if (fontInfo == NULL) {
        fprintf(stderr,"no font found\n");
        return 0;
    }
    id = fontInfo->fid;
    first = fontInfo->min_char_or_byte2;
    last = fontInfo->max_char_or_byte2; 
    base = glGenLists(last+1);
    if (base == 0) {
        fprintf(stderr,"out of display lists\n");
        return 0;
    }
    glXUseXFont(id, first, last-first+1,base+first);
    return base;
}

/**********************/
/* Drawing Attributes */
/**********************/

extern void
swSetFGColor(SDC dc, int color)
{
    if (truecolor_mode) {
        XSetForeground(TheDisplay, dc->gc, swColorToX11Color(color));
    } else {
        XColor c;

        c.red = (color & 0x000000ff) << 8;
        c.green = (color & 0x0000ff00);
        c.blue = (color & 0x00ff0000) >> 8;
        XAllocColor(TheDisplay, TheColormap, &c);
        XSetForeground(TheDisplay, dc->gc, c.pixel);
    }
}

extern void
swSetBGColor(SDC dc, int color)
{
    if (truecolor_mode) 
        XSetBackground(TheDisplay, dc->gc, swColorToX11Color(color));
    else {
        XColor c;

        c.red = (color & 0x000000ff) << 8;
        c.green = (color & 0x0000ff00);
        c.blue = (color & 0x00ff0000) >> 8;
        XAllocColor(TheDisplay, TheColormap, &c);
        XSetBackground(TheDisplay, dc->gc, c.pixel);
    }
}

extern void
swSetFont(SDC dc, SFONT font)
{
    if (!dc || !font) return;

    XSetFont(TheDisplay, dc->gc, font->fs->fid);
    dc->font = font;
}

extern int
swGetFontAscent(SFONT font)
{
    if (!font) return 0;
    return font->fs->ascent;
}

extern int
swGetFontDescent(SFONT font)
{
    if (!font) return 0;
    return font->fs->descent;
}

extern int
swGetFontHeight(SFONT font)
{
    if (!font) return 0;
    return font->fs->ascent + font->fs->descent;
}

extern void
swSetLineStyle(SDC dc, int style)
{
    XGCValues values;
    if (!dc) return;

    XGetGCValues(TheDisplay, dc->gc, GCLineStyle, &values);

    if (style == SW_SOLID) {
         values.line_style = LineSolid;
    } else if (style == SW_DASH) {
         char dash_list[2] = { 5, 5};
         values.line_style = LineOnOffDash;
         XSetDashes(TheDisplay, dc->gc, 0, dash_list, 2);
    } else if (style == SW_DOT) {
         char dash_list[2] = { 1, 1};
         values.line_style = LineOnOffDash;
         XSetDashes(TheDisplay, dc->gc, 0, dash_list, 2);
    }
    XChangeGC(TheDisplay, dc->gc, GCLineStyle, &values);
}

extern void
swSetLineWidth(SDC dc, int width)
{
    XGCValues values;
    if (!dc) return;

    XGetGCValues(TheDisplay, dc->gc, GCLineWidth, &values);
    values.line_width = width;
    XChangeGC(TheDisplay, dc->gc, GCLineWidth, &values);
}

extern void
swSetCapStyle(SDC dc, int style)
{
    XGCValues values;
    if (!dc) return;

    XGetGCValues(TheDisplay, dc->gc, GCCapStyle, &values);

    if (style == SW_CAP_ROUND) {
         values.cap_style = CapRound;
    } else if (style == SW_CAP_SQUARE) {
         values.cap_style = CapProjecting;
    } else if (style == SW_CAP_FLAT) {
         values.cap_style = CapButt;
    }
    XChangeGC(TheDisplay, dc->gc, GCCapStyle, &values);
}

extern void
swSetJoinStyle(SDC dc, int style)
{
}

extern void
swSetClipRect(SDC dc, int x, int y, int width, int height)
{
    XRectangle rect;

    if (!dc) return;

    rect.x = x;  rect.y = y;  rect.width = width;  rect.height = height;
    XSetClipRectangles(TheDisplay, dc->gc, 0, 0, &rect, 1, Unsorted);
}

/***********/
/* Drawing */
/***********/

extern void
swDrawLine(SDC dc, int x1, int y1, int x2, int y2)
{
    if (!dc || !dc->gc) return;

    XDrawLine(TheDisplay, dc->drawable, dc->gc, x1, y1, x2, y2);
}

extern void
swDrawPoint(SDC dc, int x, int y)
{
    if (!dc) return;

    XDrawPoint(TheDisplay, dc->drawable, dc->gc, x, y);
}

extern void
swDrawRect(SDC dc, int x, int y, int width, int height)
{
    if (!dc) return;

    XDrawRectangle(TheDisplay, dc->drawable, dc->gc, x, y, width, height);
}

extern void
swDrawArc(SDC dc, int x, int y, int width, int height,
           float startAngle, float endAngle)
{
    if (!dc) return;

    XDrawArc(TheDisplay, dc->drawable, dc->gc, x, y, width, height,
           (int) (startAngle * 64.0),
           (int) ((endAngle - startAngle) * 64.0));
}

extern void
swFillRect(SDC dc, int x, int y, int width, int height)
{
    if (!dc) return;

    XFillRectangle(TheDisplay, dc->drawable, dc->gc, x, y, width, height);
}

extern void
swFillPatternRect(SDC dc, int x, int y, int width, int height, SBITMAP bitmap)
{
    XGCValues values;
    if (!dc) return;

    values.tile = bitmap->pixmap;
    values.fill_style = FillTiled;
    XChangeGC(TheDisplay, dc->gc, GCFillStyle | GCTile, &values);
    XFillRectangle(TheDisplay, dc->drawable, dc->gc, x, y, width, height);
    values.fill_style = FillSolid;
    XChangeGC(TheDisplay, dc->gc, GCFillStyle, &values);
}

extern void
swDrawText(SDC dc, int x, int y, const char *str)
{
    if (!dc || !str) return;

    XDrawString(TheDisplay, dc->drawable, dc->gc, x, y, str, strlen(str));
}

extern void
swDrawTextTruncated(SDC dc, int x, int y, int width, int height,
            const char *str)
{
    int len;
    XRectangle rect;
    XFontStruct *fs;

    if (!dc || !str || width <= 0 || height <= 0) return;

    fs = dc->font->fs;

    len = strlen(str);
    rect.x = x;  rect.y = y;  rect.width = width;  rect.height = height;
    XSetClipRectangles(TheDisplay, dc->gc, 0, 0, &rect, 1, Unsorted);
    if (XTextWidth(fs, str, len) > width) {
         char *buf = malloc(len + 4);
         strcpy(buf, str);
         strcpy(buf + len, "...");
         while(len > 1 && XTextWidth(fs, buf, len + 3) > width) {
             len--;
             strcpy(buf + len, "...");
         }
         XDrawString(TheDisplay, dc->drawable, dc->gc, x, y + fs->ascent,
                     buf, len + 3);
         free(buf);
    } else {
         XDrawString(TheDisplay, dc->drawable, dc->gc, x, y + fs->ascent,
                     str, len);
    }
    XSetClipMask(TheDisplay, dc->gc, None);
}

extern void
swDrawBitmap(SDC dc, SBITMAP bitmap, int sx, int sy, int dx, int dy,
              int width, int height)
{
    if (!dc || !bitmap) return;

    XCopyArea(TheDisplay, bitmap->pixmap, dc->drawable, dc->gc,
              sx, sy, width, height, dx, dy);
}

extern void
swDrawMaskedBitmap(SDC dc, SBITMAP bitmap, SBITMAP mask, int sx, int sy,
           int dx, int dy, int width, int height)
{
    if (!dc || !bitmap) return;

    if (mask != NULL)
        XSetClipMask(TheDisplay, dc->gc, mask->pixmap);
    XSetClipOrigin(TheDisplay, dc->gc, dx - sx, dy - sy);
    XCopyArea(TheDisplay, bitmap->pixmap, dc->drawable, dc->gc,
              sx, sy, width, height, dx, dy);
    XSetClipMask(TheDisplay, dc->gc, None);
}

extern void
swCopyRect(SDC src, SDC dest, int sx, int sy, int dx, int dy,
              int width, int height)
{
    if (!src || !dest) return;

    XCopyArea(TheDisplay, src->drawable, dest->drawable, dest->gc,
              sx, sy, width, height, dx, dy);
}

/********/
/* Font */
/********/

static SFONT getFont(const char *fontspec)
{
    SFont *font;
    XFontStruct *fs;
    XmFontListEntry entry;
    static int xftInitialised = 0;

    fs = XLoadQueryFont(TheDisplay, fontspec);
    if (fs) {
         font = (SFont *) malloc(sizeof(SFont));
         entry = XmFontListEntryCreate("XmFONTLIST_DEFAULT_TAG", XmFONT_IS_FONT,
                                       fs);
         font->fontList = XmFontListAppendEntry (NULL, entry);
         XmFontListEntryFree(&entry);
         font->fs = fs;
         return font;
    } else {
         return NULL;
    }
}

extern SFONT
swFindFont(const char *family, int style, int pointSize)
{
    char fontspec[1024];
    char fam[768];
    const char *weight, *slant;
    SFont *font;
    XFontStruct *fs;
    XmFontListEntry entry;

    strncpy(fam, family, 767);

    weight = (style & SW_BOLD) ? "bold" : "medium";
    slant = (style & SW_ITALIC) ? "i" : "r";

    mysnprintf(fontspec, 1024, "-*-%s-%s-%s-*-*-%d-*-*-*-*-*-*-*",
               fam, weight, slant, pointSize);

    return getFont(fontspec);
}

extern void
swDeleteFont(SFONT font)
{
    XmFontListFree(font->fontList);
    XFreeFont(TheDisplay, font->fs);
    free(font);
}

extern SFONT
swGetDefaultFont()
{
    return TheDefaultFont;
}

extern int
swGetStringWidth(SFONT font, const char *str)
{
    if (!font || !str) return 0;
    return XTextWidth(font->fs, str, strlen(str));
}

extern void
swSetDefaultFontSpec(char *fontSpec)
{
    defaultFontSpec = strdup(fontSpec);
}

/********/
/* Menu */
/********/

extern SMENU
swCreateMenuBar(SWND wnd)
{
    SMenu *menu;
    Arg av[5];
    int ac = 0;

    if (!wnd || !wnd->mainWnd) return NULL;

    menu = newSMenu();
    XtSetArg(av[ac], XmNmarginWidth, 0);  ac++;
    XtSetArg(av[ac], XmNmarginHeight, 0);  ac++;
    menu->widget = XmCreateMenuBar(wnd->mainWnd, (String) "", av, ac);
    XmMainWindowSetAreas(wnd->mainWnd, menu->widget, NULL, NULL, NULL,
                         wnd->widget);
    XtAddCallback(menu->widget, XmNdestroyCallback, destroyMenuCallback, menu);
    XtManageChild(menu->widget);
    wnd->menu = menu;
    menu->wnd = wnd;
    return menu;
}

extern void
swMenuSetFlags(SMENU menu, int id, int mask, int flags)
{
    if (!menu) return;
    menu = findMenu(menu->widget, id);
    if (!menu) return;
    menu->flags = (menu->flags & ~mask) | flags;
    XtVaSetValues(menu->widget,
                  XmNsensitive, menu->flags & SW_MENU_DISABLED ? False : True,
                  XmNset, menu->flags & SW_MENU_CHECKED ? True : False,
                  XmNindicatorType, menu->flags & SW_MENU_RADIO_ITEM ? 
                                    XmONE_OF_MANY : XmN_OF_MANY, NULL);
}

extern SMENU
swAppendMenu(SMENU parent, const char *text)
{
    SMenu *menu;
    Widget cascade;
    Arg av[16];
    int ac = 0;
    char *newText = malloc(strlen(text) + 3);
    char mnemonic[2];

    mnemonic[0] = '\0';
    parseMenu(text, newText, mnemonic);
    mnemonic[1] = '\0';

    if (!parent) return NULL;

    menu = newSMenu();
    XtSetArg(av[ac], XmNvisual, TheVisual->visual);  ac++;

    menu->widget = XmCreatePulldownMenu(parent->widget, newText, av, ac);
    menu->wnd = parent->wnd;
    XtAddCallback(menu->widget, XmNdestroyCallback, destroyMenuCallback, menu);

    ac = 0;
    XtSetArg(av[ac], XmNsubMenuId, menu->widget);  ac++;

    if (mnemonic[0]) {
        KeySym sym = XStringToKeysym(mnemonic);
        /* causes a invalid read 8 in valgrind */
        if (sym != NoSymbol) {
            XtSetArg(av[ac], XmNmnemonic, sym);  ac++;
        }
    }
    cascade = XmCreateCascadeButton(parent->widget, (String) newText, av, ac);

    XtManageChild(cascade);
    free(newText);

    return menu;
}

extern void
swAppendMenuItem(SMENU parent, const char *text, int id)
{
    if (!parent || !text) return;

    insertMenuItem(parent->widget, parent->wnd, XmLAST_POSITION, text, id);
}

extern void
swInsertMenuItem(SMENU menu, unsigned int beforeId, const char *text, int id)
{
    short pos;
    
    if (!menu || !text) return;
    menu = findMenu(menu->widget, beforeId);
    if (!menu) return;
    XtVaGetValues(menu->widget, XmNpositionIndex, &pos, NULL);

    insertMenuItem(XtParent(menu->widget), menu->wnd, pos, text, id);
}

extern void
swDeleteMenuItem(SMENU menu, int id)
{
    if (!menu) return;
    menu = findMenu(menu->widget, id);
    if (!menu) return;
    XtDestroyWidget(menu->widget);
}

extern void
swAppendSeparator(SMENU parent)
{
    SMenu *m;
    
    if (!parent) return;
    
    m = newSMenu();
    m->widget = XtVaCreateManagedWidget("---", xmSeparatorWidgetClass,
                                        parent->widget,
                                        NULL);
    m->wnd = parent->wnd;
    XtAddCallback(m->widget, XmNdestroyCallback, destroyMenuCallback, m);
}

/**********/
/* Bitmap */
/**********/

extern SBITMAP
swCreateBitmap(SWND wnd, int width, int height)
{
    SBitmap     *bitmap;
    if (!wnd) return NULL;

    bitmap = newSBitmap();
    bitmap->pixmap = XCreatePixmap(TheDisplay,
                                   DefaultRootWindow(TheDisplay), width, height,
                                   TheVisual->depth);
    return bitmap;
}

extern SBITMAP
swCreateMask(SBITMAP bitmap, int width, int height, int bgcolor)
{
    XImage *src, *dst;
    GC gc;
    Pixel pixel;
    SBITMAP mask;
    int x, y;
    XColor bg;

    if (!bitmap) return NULL;

    bg.red = (bgcolor & 0x000000ff) << 8;
    bg.green = (bgcolor & 0x0000ff00);
    bg.blue = (bgcolor & 0x00ff0000) >> 8;
    if (truecolor_mode)
       bg.pixel = swColorToX11Color(bgcolor);
    else
       XAllocColor(TheDisplay, TheColormap, &bg);

    mask = newSBitmap();
    mask->pixmap = XCreatePixmap(TheDisplay, bitmap->pixmap, width, height, 1);
    gc = XCreateGC(TheDisplay, mask->pixmap, 0, NULL);

    src = XGetImage(TheDisplay, bitmap->pixmap, 0, 0, width, height,
                    AllPlanes, ZPixmap);

    if (src == NULL) /* cause of a nasty olpc X server limit */
        return NULL;
    dst = XCreateImage(TheDisplay, TheVisual->visual, 1,
                       XYBitmap, 0, malloc(width * height), width, height, 32, 
                       0);

    for (y = 0; y < height; y++) { 
         for (x = 0; x < width; x++) {
             pixel = XGetPixel(src, x, y);
             XPutPixel(dst, x, y, pixel == bg.pixel);
         }
    }
    XPutImage(TheDisplay, mask->pixmap, gc, dst, 0, 0, 0, 0, width, height);
#ifndef DO_NOT_X_DESTROY_IMAGE
    /*
       XDestroyImage can crash during a advanced efence test
       cause XDestroyImage calls internally the free(3) function
       in a advanced efence test but explicit efence malloc calls do not
       match Xlib intern free calls 
     */
    XDestroyImage(src);
    XDestroyImage(dst);
#endif
    XFreeGC(TheDisplay, gc);
    return mask;
}

extern void
swDestroyBitmap(SBITMAP bitmap)
{
    if (!bitmap) return;

    XFreePixmap(TheDisplay, bitmap->pixmap);
    free(bitmap);
}

extern void
swDrawMappedPixels(SBITMAP bitmap, int srcx, int srcy, int dstx, int dsty, 
           int width, int height, const cbyte *data, int numColors,
           const int *palette)
{
    XImage *image;
    XColor colors[256];
    int i;
    char *d;
    GC gc;

    if (!bitmap || !data || !palette) return;

    gc = XCreateGC(TheDisplay, bitmap->pixmap, 0, NULL);

    for (i = 0; i < numColors; i++) {
         cbyte r = SW_RED(palette[i]);
         cbyte g = SW_GREEN(palette[i]);
         cbyte b = SW_BLUE(palette[i]);

         colors[i].pixel = i;
         colors[i].red = r << 8;
         colors[i].green = g << 8;
         colors[i].blue = b << 8;
         colors[i].flags = DoRed | DoGreen | DoBlue;
        
       if (truecolor_mode) {
            colors[i].pixel=swColorToX11Color(palette[i]);
        } else 
             if (!XAllocColor(TheDisplay, TheColormap, &colors[i])) {
                fprintf(stderr, "couldn't get color %d\n", i);
         }
    }

    switch (TheVisual->depth) {
      case 8:
      {
         char *newData = malloc(width * height);
         for (i = 0; i < width * height; i++) {
             newData[i] = (char) colors[(int) data[i]].pixel;
         }
         d = newData;
         break;
      }
      case 12:
      case 15:
      case 16:
      {
         short *newData = (short *) malloc(width * height * 2);
         for (i = 0; i < width * height; i++) {
             newData[i] = (short) colors[(int) data[i]].pixel;
         }
         d = (char *) newData;
         break;
      }
      case 18:
      case 21:
      case 24:
      case 32:
      {
         int *newData = (int *) malloc(width * height * 4);
         for (i = 0; i < width * height; i++) {
             newData[i] = (int) colors[(int) data[i]].pixel;
         }
         d = (char *) newData;
         break;
      }
      default:
        fprintf(stderr,"unsupported Visual: depth %d\n",TheVisual->depth);
        XFreeGC(TheDisplay, gc);
        return;
    }
      
    image = XCreateImage(TheDisplay, TheVisual->visual, TheVisual->depth,
                         ZPixmap, 0, d, width, height, 8, 0);

    XPutImage(TheDisplay, bitmap->pixmap, gc, image, srcx, srcy,
              dstx, dsty, width, height);
#ifndef DO_NOT_X_DESTROY_IMAGE
    /*
       XDestroyImage can crash during a advanced efence test
       cause XDestroyImage calls internally the free(3) function
       in a advanced efence test but explicit efence calls to malloc do not
       match Xlib intern free calls 
     */
    XDestroyImage(image);
#endif
    XFreeGC(TheDisplay, gc);
}

/**********/
/* cursor */
/**********/

extern SCURSOR
swLoadCursor(int id)
{
    SCursor     *cursor = newSCursor();

    switch (id) {
      case SW_CURSOR_ARROW:
        cursor->cursor = XCreateFontCursor(TheDisplay, XC_top_left_arrow);
        break;
      case SW_CURSOR_DBL_ARROW_VERT:
        cursor->cursor = XCreateFontCursor(TheDisplay, XC_sb_v_double_arrow);
        break;
      case SW_CURSOR_DBL_ARROW_HORZ:
        cursor->cursor = XCreateFontCursor(TheDisplay, XC_sb_h_double_arrow);
        break;
      case SW_CURSOR_CROSS:
        cursor->cursor = XCreateFontCursor(TheDisplay, XC_cross);
        break;
      case SW_CURSOR_CIRCLE:
        cursor->cursor = XCreateFontCursor(TheDisplay, XC_circle);
        break;
      case SW_CURSOR_ROLL:
        cursor->cursor = XCreateFontCursor(TheDisplay, XC_exchange);
        break;
      case SW_CURSOR_NONE:
        cursor->cursor = XCreatePixmapCursor(TheDisplay, 
              cursorNonePixmap, cursorNonePixmap, 
              &cursorNoneColor, &cursorNoneColor, 0, 0);
         break;
      default:
        cursor->cursor = None; /* FIXME: load cursor from resource here */
        break;
    }
    return cursor;
}

extern void
swSetCursor(SWND wnd, SCURSOR cursor)
{
    if (!wnd || !wnd->widget || !XtWindow(wnd->widget) || !cursor) return;

    XDefineCursor(TheDisplay, XtWindow(wnd->widget), cursor->cursor);
}

extern void
swDestroyCursor(SCURSOR cursor)
{
    XFreeCursor(TheDisplay, cursor->cursor);
    free(cursor);
}

/************/
/* combobox */
/************/

extern SWND
swCreateComboBox(const char *title, int x, int y, int width, int height,
         SWND parent, int id)
{
    SWindow *window;
    Arg av[15];
    int ac = 0;
    Widget pulldown;

    if (!parent) return NULL;

    window = newSWindow();
    window->menu = newSMenu();
    window->menu->id = id;
    XtSetArg(av[ac], XmNvisual, TheVisual->visual);  ac++;
    pulldown = XmCreatePulldownMenu(parent->widget, (String) "", av, ac);
    window->menu->wnd = parent;

    ac = 0;
    XtSetArg(av[ac], XmNx, x);  ac++;
    XtSetArg(av[ac], XmNy, y);  ac++;
    XtSetArg(av[ac], XmNwidth, width);  ac++;
    XtSetArg(av[ac], XmNheight, height);  ac++;
    XtSetArg(av[ac], XmNmarginWidth, 0);  ac++;
    XtSetArg(av[ac], XmNmarginHeight, 0);  ac++;
    XtSetArg(av[ac], XmNsubMenuId, pulldown); ac++;
    XtSetArg(av[ac], XmNuserData, window); ac++;
    XtSetArg(av[ac], XmNorientation, XmVERTICAL); ac++;

    window->widget = XmCreateOptionMenu(parent->widget, (String) title, av, ac);
    initWidget(window->widget, window);
    XtManageChild(window->widget);
    return window;
}

extern void
swComboBoxDeleteAll(SWND list)
{
    Widget pulldown;
    WidgetList children;
    int i, numChildren;

    if (!list) return;

    XtVaGetValues(list->widget, XmNsubMenuId, &pulldown, NULL);
    XtVaGetValues(pulldown, XmNchildren, &children,
                  XmNnumChildren, &numChildren, NULL);
    for (i = 0; i < numChildren; i++) {
        XtUnmanageChild(children[i]);
        XtDestroyWidget(children[i]);
    }
}

extern void
swComboBoxAppendItem(SWND list, const char *str)
{
    SWindow *window;    
    Widget pulldown;
    XmString s;
    Widget w;    

    if (!list) return;

    XtVaGetValues(list->widget, XmNuserData, &window, NULL);
    XtVaGetValues(list->widget, XmNsubMenuId, &pulldown, NULL);
    s = XmStringCreateLocalized((String) str);
    w = XtVaCreateManagedWidget(str, xmToggleButtonWidgetClass,
                                pulldown,
                                XmNlabelString, s,
                                XmNindicatorOn, False,
                                NULL);
    XtAddCallback(w, XmNvalueChangedCallback, comboBoxActivateCallback, 
                  window);
    XmStringFree(s);
}

extern void
swComboBoxSetSelection(SWND list, int selection)
{
    Widget pulldown;
    WidgetList children;
    int numChildren = 0;

    if (!list) return;

    XtVaGetValues(list->widget, XmNsubMenuId, &pulldown, NULL);
    XtVaGetValues(pulldown, XmNnumChildren, &numChildren, NULL);
    if (numChildren > 0) {
        XtVaGetValues(pulldown, XmNchildren, &children, NULL); 
        if (selection >= 0 && selection < numChildren)
            XtVaSetValues(list->widget, XmNmenuHistory, children[selection], 
                          NULL);
    }
}

extern int
swComboBoxGetSelection(SWND list)
{
    Widget pulldown;
    WidgetList children;
    int i, numChildren = 0;
    Widget selected;

    if (!list) return -1;

    XtVaGetValues(list->widget, XmNsubMenuId, &pulldown, NULL);
    XtVaGetValues(pulldown, XmNnumChildren, &numChildren, NULL);
    if (numChildren > 0) {
        XtVaGetValues(pulldown, XmNchildren, &children, NULL);
        XtVaGetValues(list->widget, XmNmenuHistory, &selected, NULL);
        for (i = 0; i < numChildren; i++)
            if (children[i] == selected)
                return i;
    }
    return -1;
}

/***********/
/* dialogs */
/***********/

extern SWND
swCreateDialog(const char *title, int x, int y, int width, int height,
                SWND parent)
{
    SWindow *w;
    Arg av[10];
    int ac = 0;
    XmString str;
    Widget p;

    if (!parent) return NULL;

    p = parent->shell ? parent->shell : parent->widget;

    w = newSWindow();
    XtSetArg(av[ac], XmNx, x);  ac++;
    XtSetArg(av[ac], XmNy, y);  ac++;
    XtSetArg(av[ac], XmNwidth, width);  ac++;
    XtSetArg(av[ac], XmNheight, height);  ac++;
    XtSetArg(av[ac], XmNautoUnmanage, False);  ac++;
    XtSetArg(av[ac], XmNvisual, TheVisual->visual); ac++;
    str = XmStringCreateLocalized((String) title);
    XtSetArg(av[ac], XmNdialogTitle, str);  ac++;
    XtSetArg(av[ac], XmNdeleteResponse, XmDO_NOTHING);  ac++;

    w->widget = XmCreateBulletinBoardDialog(p, (String) title, av, ac);
    XmStringFree(str);
    /* set up a delete response */
    XmAddWMProtocolCallback(XtParent(w->widget), WM_DELETE_WINDOW,
                            onDeleteDialog, w);
    initWidget(w->widget, w);
    return w;
}

extern int
swDialog(SWND dialog)
{
    int rc;

    if (!dialog) return 0;

    XtVaSetValues(dialog->widget, XmNdialogStyle,
                  XmDIALOG_PRIMARY_APPLICATION_MODAL, NULL);
    XtManageChild(dialog->widget);
    rc = swModalLoop();
    XtUnmanageChild(dialog->widget);
    return rc;
}

extern void
swEndDialog(int rc)
{
    swExitModalLoop(rc);
}

extern SWND
swGetDialogItem(SWND dialog, int item)
{
    int i, numChildren;
    WidgetList children;

    if (!dialog) return NULL;

    XtVaGetValues(dialog->widget, XmNnumChildren, &numChildren,
                  XmNchildren, &children, NULL);
    for (i = 0; i < numChildren; i++) {
        SWND wnd;

        XtVaGetValues(children[i], XmNuserData, &wnd, NULL);
        if (wnd && wnd->data == swToPtr(item)) {
            return wnd;
        }
    }

    return NULL;
}

extern void
swSetDefaultButton(SWND dialog, SWND button)
{
    if (!dialog || !button) return;

    XtVaSetValues(dialog->widget, XmNdefaultButton, button->widget, NULL);
}

extern void
swSetCancelButton(SWND dialog, SWND button)
{
    if (!dialog || !button) return;

    XtVaSetValues(dialog->widget, XmNcancelButton, button->widget, NULL);
}

void swChAppleDir(void)
{
#ifdef MACOSX
/*
 *  MacOSX programs started from Desktop use "/" as default path,
 *  so change directory to $HOME/Desktop first
 */
    char *home;
    char *path;
    char *desktop = "/Desktop";
    home = getenv("HOME");
    if (home != NULL) {
        path = (char *) malloc(strlen(home)+strlen(desktop)+1);
        strcpy(path, home);
        strcat(path, desktop);        
        chdir(path);
        free(path);
    }
#endif
}

extern int
swOpenDirDialog(SWND wnd, const char *title, const char *filters, 
                char *buf, int len)
{
    return doFileDialog(title, wnd, filters, buf, len, FILE_MUST_EXIST, NULL);
}


extern int
swOpenFileDialog(SWND wnd, const char *title, const char *filters,
                 char *buf, int len)
{
    return doFileDialog(title, wnd, filters, buf, len, NOT_A_DIRECTORY, NULL);
}

extern int
swOpenMaybeFileDialog(SWND wnd, const char *title, const char *filters,
                      char *buf, int len)
{
    return doFileDialog(title, wnd, filters, buf, len, MAY_BE_FILE, NULL);
}

extern int
swOpenExecutableDialog(SWND wnd, const char *title, const char *filters,
                       char *buf, int len)
{
    return doFileDialog(title, wnd, filters, buf, len, 
                        NOT_A_DIRECTORY | FILE_EXECUTABLE, NULL);
}

extern int
swSaveFileDialog(SWND wnd, const char *title, const char *filters,
                 char *buf, int len, const char* ext)
{
    return doFileDialog(title, wnd, filters, buf, len, OVERWRITE_PROMPT, ext);
}

/***************/
/* message box */
/***************/

extern int
swMessageBox(SWND parent, const char *text, const char *title, int type,
              int icon)
{
    Widget top;

    if (!parent) return 0;

    top = parent->shell ? parent->shell : parent->widget;
    return messageBox(top, text, title, type, icon);
}

/********************/
/* debugging output */
/********************/

extern void
swDebugf(const char *fmt, ...)
{
    va_list ap;
    char message[1024];

    va_start(ap, fmt);
    myvsnprintf(message, 1024, fmt, ap);
    fprintf(stderr, "%s", message);
}

/*****************/
/* miscellaneous */
/*****************/

extern double
swGetCurrentTime(void)
{
    struct timeval      now;

    gettimeofday(&now, NULL);
    return (double) now.tv_sec + (double) now.tv_usec / 1000000.0;
}

extern void
swGetTempFile(char *buf, const char* name, const char* ext, int len)
{
    static int tempCounter = 0;
 
    mysnprintf(buf,len,"%s_%d_%d%s",name,getpid(),++tempCounter,ext);
}

extern void
swGetTempDir(char *buf, int len)
{
    strncpy(buf, "/tmp/", len);
}

extern void
swGetTempPath(char *buf, const char* name, const char* ext, int len)
{
    swGetTempDir(buf,len);
    swGetTempFile(buf+strlen(buf),name,ext,len-strlen(buf));
}

extern char             
swGetPathSeperator(void)
{
    return '/';
}

/* return 0 on successfull write of remaining buffers */
extern int 
swTruncateClose(int f)
{
    off_t length=0;
    off_t seek0=0; 
    if (myflushall() < 0)
        return -1;
    length=lseek(f,seek0,SEEK_CUR);
    if (ftruncate(f,length) < 0)
        return -1;
    return myclose(f);
}

extern const char*
swGetLinefeedString(void)
{
    return "\r\n";
}

extern int 
swIsXML(const char *filename)
{
    int i;

    if (strlen(filename)==0)
        return(0);
    for (i=strlen(filename)-1;(i>0)&&(filename[i]!='.');i--);
    return !strcmp(&filename[i],".x3d");
}

extern char*
swKillFileQuotes(char* filename)
{
    return filename;
}
         
extern char*
swGetEnv(const char* variablename)
{
    return getenv(variablename);
}

extern const char*
swGetToNullDevice(void)
{
    return " > /dev/null";
}


extern int
swMkdirCommand(SWND wnd, const char* dir)
{
    char* string;
    const char* mkdir = "mkdir -p ";
    int ret;

    string = (char*) malloc(strlen(mkdir) + strlen(dir) + 1);
    if (string == NULL) return 1;
    strcpy(string, mkdir);
    strcat(string, dir);
    ret = system(string);
    free(string);
    return ret;
}    

extern int
swCopyFile(SWND wnd, const char* path, const char* dir)
{
    FILE *inputFile;
    int ret = 0;

    inputFile = fopen(path, "rb");
    if (inputFile == NULL) {
        perror(path);
        return -1;
    }
    if (fseek(inputFile, 0, SEEK_END) == -1) {
        perror(path);
        ret = -1;
    } else {
        long fileLen = 0;
        char *content;

        fileLen = ftell(inputFile);
        rewind(inputFile);
        content = (char *) malloc(fileLen);
        if (content == NULL)
            ret = -1;
        else {
            if (fread(content, 1, fileLen, inputFile) != fileLen) {
                perror(path);
                ret = -1;
            } else {
               const char *filename;
               char *outputFilename;
   
               filename = strrchr(path, swGetPathSeperator());
               if (filename == NULL)
                   filename = path;
               else
                   filename += 1;
               outputFilename = (char*) malloc(strlen(dir) + 1 + 
                                               strlen(filename) + 1);
               if (outputFilename == NULL) 
                   ret = -1;
               else {
                   FILE *outputFile;     
 
                   strcpy(outputFilename, dir);
                   if (strlen(outputFilename) > 0)
                       if (outputFilename[strlen(outputFilename) - 1] != 
                           swGetPathSeperator()) {
                           int len = strlen(outputFilename);
                           outputFilename[len] = swGetPathSeperator();
                           outputFilename[len + 1] = 0;
                       }
                   strcat(outputFilename, filename);

                   outputFile = fopen(outputFilename, "wb");
                   if (outputFile == NULL) {
                       perror(outputFilename);
                       ret = -1;
                   } else {
                       if (fwrite(content, 1, fileLen, outputFile) != fileLen) {
                           perror(outputFilename);
                           ret = -1;
                       }
                       fclose(outputFile);
                   }
               }
               free(outputFilename);
            }
        }
        free(content);
    }
    fclose(inputFile);

    return ret;
}    

extern int             
swShellCommand(SWND wnd, const char* file, const char* command, 
               const char* directory)
{
    char* string;
    const char* cd="cd ";
    int ret;

    string=(char*)malloc(strlen(cd)+strlen(directory)+1+strlen(command)+1);
    if (string==NULL) return 1;
    strcpy(string,cd);
    strcat(string,directory);
    strcat(string,";");
    strcat(string,command);
    ret=system(string);
    free(string);
    return ret;
}

extern void 
swDeleteCallbacks(SWND wnd)
{
    if (wnd==NULL) return;
    wnd->exposeCallback = NULL;
    wnd->mouseCallback = NULL;
    wnd->keyCallback = NULL;
    wnd->focusCallback = NULL;
    wnd->resizeCallback = NULL;
    wnd->enterCallback = NULL;
    wnd->scrollBarCallback = NULL;
    wnd->commandCallback = NULL;
    wnd->highlightCallback = NULL;
    wnd->closeCallback = NULL;
    wnd->dropCallback = NULL;
}

static int swLanguage = 0;
static const char *swYes = "yes";
static const char *swNo = "no";

extern void 
swSetLang(int lang)
{
    swLanguage = lang;
    swYes = swLangYes(lang);
    swNo = swLangNo(lang);
}

extern int
swGetLang(void)
{
    return swLanguage;
}

/**************************/
/* persistent preferences */
/**************************/

static int defaultPreferences = 0;

extern void swPreferencesDefault(void)
{
    defaultPreferences = 1;
}

extern STABLE
swLoadPreferences(const char *company, const char *appName)
{
    char line[MAX_PREFLINE], *c, *key, *value;
    const char *home = getenv("HOME");
    FILE *f;
    STable *table;
    int len;
    char *rcFileEnvironmentName;
    int i;

    /* company is (intentionally) ignored */

    if (!appName) return NULL;
    
    table = (STable *) malloc(sizeof(STable));
    table->size = 0;
    table->capacity = 8;
    rcFileEnvironmentName = (char *) malloc(strlen(appName) + strlen("RC")
                                           + 1);
    strcpy(rcFileEnvironmentName, appName);
    for (i = 0; i < strlen(rcFileEnvironmentName); i++)
        rcFileEnvironmentName[i] = toupper(rcFileEnvironmentName[i]);
    strcat(rcFileEnvironmentName, "RC");
    if (getenv(rcFileEnvironmentName) == NULL) {    
        len = (home == NULL) ? 0 : strlen(home) + strlen(appName) + 5;
        table->filename = (char *) malloc(len);
        mysnprintf(table->filename, len, "%s/.%src", home, appName);
    } else
        table->filename = getenv(rcFileEnvironmentName);
        
    table->data = (Pair *) malloc(table->capacity * sizeof(Pair));

    f = fopen(table->filename, "r");

    if (!f) return table;

    while (fgets(line, MAX_PREFLINE, f))
    {
        if (line[0] == '#') continue;
        c = line;
        while(isspace(*c)) c++;
        if (!*c) continue;
        key = c;
        while(*c && !isspace(*c)) c++;
        *c = '\0';
        c++;
        while(isspace(*c)) c++;
        value = c;
        while(*c && *c != '\n') c++;
        if (*c == '\n') *c = '\0';
        appendPair(table, key, value);
    }
    fclose(f);

    return table;
}

extern void
swSavePreferences(STABLE table)
{
    int f;
    int i;

    f = open(table->filename, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    if (f==-1) return;

    for (i = 0; i < table->size; i++) {
        mywritestr(f,table->data[i].key);
        mywritestr(f," ");
        mywritestr(f,table->data[i].value);
        mywritestr(f,"\n");
    }
    swTruncateClose(f);
}

extern void
swDeletePreferences(STABLE table)
{
    int i;

    for (i = 0; i < table->size; i++) {
        free(table->data[i].key);
        free(table->data[i].value);
    }
    free(table->filename);
    free(table->data);
    free(table);
}

extern const char *
swGetPreference(STABLE table, const char *key, const char *defaultValue)
{
    int i;

    if (defaultPreferences)
        return defaultValue;  
    for (i = 0; i < table->size; i++) {
        if (!strcmp(table->data[i].key, key)) {
            return table->data[i].value;
        }
    }
    return defaultValue;
}

extern void
swSetPreference(STABLE table, const char *key, const char *value)
{
    int i;

    for (i = 0; i < table->size; i++) {
        if (!strcmp(table->data[i].key, key)) {
            free(table->data[i].value);
            table->data[i].value = mystrdup(value);
            return;
        }
    }
    appendPair(table, key, value);
}

extern int
swGetIntPreference(STABLE table, const char *key, int defaultValue)
{
    int i;

    if (defaultPreferences)
        return defaultValue;  
    for (i = 0; i < table->size; i++) {
        if (!strcmp(table->data[i].key, key)) {
            return atoi(table->data[i].value);
        }
    }
    return defaultValue;
}

extern void
swSetIntPreference(STABLE table, const char *key, int value)
{
    char str[128];
    mysnprintf(str, 128, "%d", value);
    swSetPreference(table, key, str);
}

extern void 
swUninstall(STABLE table, const char *companyName, const char *appName)
{
    if (table->filename)
        printf("name of the local configuration file is %s\n",table->filename);
}

/*****************/
/* mouse capture */
/*****************/
extern void
swSetCapture(SWND wnd)
{
    if (!wnd) return;

    XtGrabPointer(wnd->widget, False,
                  PointerMotionMask | ButtonPressMask | ButtonReleaseMask,
                  GrabModeAsync, GrabModeAsync, None, None, CurrentTime);
}

extern void
swReleaseCapture(SWND wnd)
{
    if (!wnd) return;

    XtUngrabPointer(wnd->widget, CurrentTime);
}

/****************/
/* accelerators */
/****************/

extern void
swAddAccelerator(SWND wnd, int key, int modifiers, int id)
{
    int state = 0;
    AccelTable *table;
    Accelerator *accel;

    if (!wnd) return;

    if (modifiers & SW_SHIFT) state |= ShiftMask;
    if (modifiers & SW_CONTROL) state |= ControlMask;
    if (modifiers & SW_ALT) state |= Mod1Mask;

    table = wnd->accelTable;
    if (!table) {
        table = (AccelTable *) malloc(sizeof(AccelTable));
        table->size = 0;
        table->capacity = 8;
        table->data = (Accelerator *) malloc(8 * sizeof(Accelerator));
        wnd->accelTable = table;
    }
    if (table->size == table->capacity) {
        table->capacity *= 2;
        table->data = (Accelerator *)
        realloc(table->data, table->capacity * sizeof(Accelerator));
    }
    accel = table->data + table->size;

/*  
 * quick and dirty bugfix from 
 *  http://sourceforge.net/tracker/index.php?func=detail&aid=205443&group_id=3356&atid=103356
 *  MUFTI
 */
    if (key<=256) 
        accel->key = _invKeycodeMap[tolower(key)]; 
    else 
        accel->key = _invKeycodeMap[key]; 

    accel->state = state;
    accel->id = id;
    table->size++;
}

/*****************/
/* drag und drop */
/*****************/

#ifdef MOTIF_DRAGNDROP

extern void
swRegisterDropTarget(SWND wnd, DropCallback dropCB)
{
    Arg av[10];
    int ac = 0;
    Atom importTargets[1] = { XA_STRING };

    if (!wnd || !dropCB) return;

    wnd->dropCallback = dropCB;
    XtVaSetValues(XmGetXmDisplay(TheDisplay),
                  XmNdragReceiverProtocolStyle,  XmDRAG_DYNAMIC,
                  XmNdragInitiatorProtocolStyle, XmDRAG_DYNAMIC,
                  NULL);
    XtSetArg(av[ac], XmNdropSiteOperations,
             XmDROP_MOVE | XmDROP_COPY | XmDROP_LINK);  ac++;
    XtSetArg(av[ac], XmNimportTargets, importTargets);  ac++;
    XtSetArg(av[ac], XmNnumImportTargets, 1);  ac++;
    XtSetArg(av[ac], XmNdropProc, dropProc);  ac++;
    XtSetArg(av[ac], XmNdragProc, dragProc);  ac++;
    XmDropSiteRegister(wnd->widget, av, ac);
    XtVaSetValues(wnd->widget, XmNuserData, wnd, NULL);
}

extern void
swUnregisterDropTarget(SWND wnd)
{
    XmDropSiteUnregister(wnd->widget);
    wnd->dropCallback = NULL;
}

extern int
swDragDrop(SWND wnd, int operations, SBITMAP bitmap, SBITMAP mask,
            int x, int y, int width, int height)
{
    XEvent event;
    Arg av[15];
    int ac = 0;
    Atom exportTargets[1] = { XA_STRING };
    Widget icon = NULL;

    if (!wnd || !bitmap || !mask) return 0;

    if (bitmap) {
        Pixmap pixmap, mask;
        createSourceCursor(bitmap->pixmap, x, y, width, height,
                           wnd->widget, &pixmap, &mask);
        icon = createDragIcon(wnd->widget, pixmap, mask, width, height);
        XtSetArg(av[ac], XmNsourceCursorIcon, icon);  ac++;
    }

    TheCopyIcon = createDragIcon(wnd->widget, TheCopyPixmap, 0,
                                 copy_width, copy_height);
    TheLinkIcon = createDragIcon(wnd->widget, TheLinkPixmap, 0,
                                 link_width, link_height);
    TheMoveIcon = createDragIcon(wnd->widget, TheMovePixmap, 0, 1, 1);

    XtSetArg(av[ac], XmNcursorBackground, 0); ac++;
    XtSetArg(av[ac], XmNconvertProc, convertProc); ac++;
    XtSetArg(av[ac], XmNexportTargets, exportTargets); ac++;
    XtSetArg(av[ac], XmNnumExportTargets, 1);  ac++;
    XtSetArg(av[ac], XmNdragOperations, opToMop(operations));  ac++;
    XtSetArg(av[ac], XmNclientData, wnd);  ac++;
    TheDragContext = XmDragStart(wnd->widget, &TheLastButtonPress, av, ac);
    XtAddCallback(TheDragContext, XmNdragDropFinishCallback, dragFinish, wnd);

    InDragNDrop = TRUE;

    while (InDragNDrop)
    {
        XtAppProcessEvent(TheAppContext, XtIMAll);
        if (XtAppPending(TheAppContext) == 0) {
            update();
        }
    }
    XtDestroyWidget(TheCopyIcon);
    XtDestroyWidget(TheLinkIcon);
    XtDestroyWidget(TheMoveIcon);
    if (icon) XtDestroyWidget(icon);
    return 0;
}

#else /* !MOTIF_DRAGNDROP */

extern void
swRegisterDropTarget(SWND wnd, DropCallback dropCB)
{
    if (!wnd || !dropCB) return;

    wnd->dropCallback = dropCB;
}

extern void
swUnregisterDropTarget(SWND wnd)
{
    wnd->dropCallback = NULL;
}

extern int
swDragDrop(SWND wnd, int operations, SBITMAP bitmap, SBITMAP mask,
            int x, int y, int width, int height)
{
    int rx, ry, wx, wy;
    unsigned state;
    Window root, child;

    if (!wnd || !bitmap || !mask) return 0;

    XQueryPointer(TheDisplay, XtWindow(wnd->widget), &root, &child, &rx, &ry,
                  &wx, &wy, &state);
                         
    TheDragPopup = XtVaCreatePopupShell("swDragWindow",
                                         overrideShellWidgetClass,
                                         XtParent(wnd->widget),
                                         XmNwidth, width,
                                         XmNheight, height,
                                         XmNborderWidth, 0,
                                         XmNx, rx, XmNy, ry,
                                         XmNvisual, TheVisual->visual,
                                         NULL);
    
    TheDragWindow = createContainer("", 0, 0, width, height, TheDragPopup);

    XtAddCallback(TheDragWindow, XmNexposeCallback, exposeDragDropWindow, NULL);

    TheDragPixmapOffsetX = x;
    TheDragPixmapOffsetY = y;
    TheDragPixmap = bitmap->pixmap;
    TheDragMask = mask->pixmap;

    XtPopup(TheDragPopup, XtGrabNone);

    InDragNDrop = TRUE;

    while (InDragNDrop)
    {
        XtAppProcessEvent(TheAppContext, XtIMAll);
        if (XtAppPending(TheAppContext) == 0) {
            update();
        }
    }
    return 0;
}

#endif /* !MOTIF_DRAGNDROP */

/********************/
/* static functions */
/********************/

static Widget
createContainer(const char *title, int x, int y, int width, int height,
        Widget parent)
{
    return XtVaCreateManagedWidget(title, xmDrawingAreaWidgetClass,
                                   parent,
                                   XmNx, x, XmNy, y,
                                   XmNwidth, width,
                                   XmNheight, height,
                                   XmNmarginWidth, 0,
                                   XmNmarginHeight, 0,
  /* ignore resize requests */     XmNresizePolicy, XmRESIZE_NONE, 
  /* from children */
                                   XtNvisual, TheVisual->visual,
                                   NULL);

}

static SWindow *
newSWindow()
{
    SWindow *wnd = malloc(sizeof(SWindow));
    wnd->widget = NULL;
    wnd->shell = NULL;
    wnd->mainWnd = NULL;
    wnd->menu = NULL;
    wnd->accelTable = NULL;
    wnd->data = NULL;
    wnd->exposeCallback = NULL;
    wnd->mouseCallback = NULL;
    wnd->keyCallback = NULL;
    wnd->focusCallback = NULL;
    wnd->resizeCallback = NULL;
    wnd->enterCallback = NULL;
    wnd->scrollBarCallback = NULL;
    wnd->commandCallback = NULL;
    wnd->highlightCallback = NULL;
    wnd->closeCallback = NULL;
    wnd->dropCallback = NULL;
    wnd->invx1 = wnd->invx2 = wnd->invy1 = wnd->invy2 = -1;
    wnd->isHidden = FALSE;

    return wnd;
}

static SMenu *
newSMenu()
{
    SMenu *menu = malloc(sizeof(SMenu));
    menu->widget = NULL;
    menu->wnd = NULL;
    menu->id = -1;
    menu->flags = 0;

    return menu;
}

static SDContext *
newSDContext()
{
    SDContext *dc = malloc(sizeof(SDContext));
    dc->gc = NULL;
    dc->drawable = 0;
    dc->isBitmapDC = 0;
    dc->font = TheDefaultFont;

    return dc;
}

static SBitmap *
newSBitmap()
{
    SBitmap *bitmap = (SBitmap *) malloc(sizeof(SBitmap));
    bitmap->pixmap = 0;

    return bitmap;
}

static SCursor *
newSCursor()
{
    SCursor *cursor = (SCursor *) malloc(sizeof(SCursor));
    cursor->cursor = 0;

    return cursor;
}

static STimer *
newSTimer(int timeout, TimerCallback cb, void *data)
{
    STimer *timer = (STimer *) malloc(sizeof(STimer));

    timer->timeout = timeout;
    timer->timerCallback = cb;
    timer->data = data;

    return timer;
}

static void
initWidget(Widget widget, SWND wnd)
{
    SWND mainWnd = findMainWindow(widget);

    XtVaSetValues(widget, XmNuserData, wnd, NULL);
    XtAddCallback(widget, XmNdestroyCallback, destroyCallback, wnd);
    if (mainWnd && mainWnd->accelTable) {
        XtInsertEventHandler(widget, KeyPressMask,
                             False, acceleratorHandler, (XtPointer) mainWnd,
                             XtListHead);
    }
}

static void
resize(Widget widget, XtPointer data, XtPointer event_data)
{
    SWindow *w = (SWindow *) data;
    Dimension width, height;

    if (!w) return;

    XtVaGetValues(widget, XmNwidth, &width, XmNheight, &height, NULL);
    if (w->resizeCallback) {
        w->resizeCallback(w->data, width, height);
    }
}

static void
scrollCallback(Widget widget, XtPointer data, XtPointer event_data)
{
    SWindow *sb = (SWindow *) data;
    XmScrollBarCallbackStruct *cbs = (XmScrollBarCallbackStruct *) event_data;
    int type;

    if (!sb || !sb->scrollBarCallback || !cbs) return;

    switch (cbs->reason) {
      case XmCR_VALUE_CHANGED:
      case XmCR_DRAG:
        type = SW_SB_SCROLL;
        break;
      case XmCR_INCREMENT:
        type = SW_SB_LINE_DOWN;
        break;
      case XmCR_DECREMENT:
        type = SW_SB_LINE_UP;
        break;
      case XmCR_PAGE_INCREMENT:
        type = SW_SB_PAGE_DOWN;
        break;
      case XmCR_PAGE_DECREMENT:
        type = SW_SB_PAGE_UP;
        break;
      default:
        type = SW_SB_SCROLL;
        break;
    }

    (sb->scrollBarCallback)(sb->data, type, cbs->value);
}

static void
buttonCallback(Widget widget, XtPointer data, XtPointer event_data)
{
    SWindow *b = (SWindow *) data;
    unsigned char type;

    if (!b) return;

    if (XtClass(widget) == xmToggleButtonWidgetClass) {
        XtVaGetValues(widget, XmNindicatorType, &type, NULL);
        if (type == XmONE_OF_MANY) {
            XtVaSetValues(widget, XmNset, True, NULL);
            resetRadioButtons(widget);
        }
    }
    callCommandCallback(b->widget, b->data);
}

static void
textActivateCallback(Widget widget, XtPointer data, XtPointer event_data)
{
    SWindow *wnd = (SWindow *) data;
    if (!wnd) return;

    callCommandCallback(wnd->widget, wnd->data);
}

static void
textFocusCallback(Widget widget, XtPointer data, XtPointer event_data)
{
    SWindow    *w = (SWindow *) data;
    if (!w || !w->focusCallback) return;

    w->focusCallback(w->data, TRUE);
}

static void
textLosingFocusCallback(Widget widget, XtPointer data, XtPointer event_data)
{
    SWindow    *w = (SWindow *) data;
    if (!w || !w->focusCallback) return;

    w->focusCallback(w->data, FALSE);
}

static void
destroyCallback(Widget widget, XtPointer data, XtPointer event_data)
{
    SWindow    *wnd = (SWindow *) data;

    if (wnd->accelTable) {
        wnd->accelTable->size = 0;
        free(wnd->accelTable->data);
        wnd->accelTable->data = NULL;
        free(wnd->accelTable);
    }
    free(wnd);
}

static void
comboBoxActivateCallback(Widget widget, XtPointer data, XtPointer event_data)
{
    SWindow *window = (SWindow *) data;
    if (window) {
        callCommandCallback(window->menu->wnd->widget, 
                            swToPtr(window->menu->id));
    }
}

static void
destroyMenuCallback(Widget widget, XtPointer data, XtPointer event_data)
{
    SMenu *m = (SMenu *)data;

    if (XtHasCallbacks(m->widget, XmNarmCallback) == XtCallbackHasSome)
        XtRemoveCallback(m->widget, XmNarmCallback, menuArmCallback, NULL);
    if (XtHasCallbacks(m->widget, XmNdisarmCallback) == XtCallbackHasSome)
        XtRemoveCallback(m->widget, XmNdisarmCallback, menuDisarmCallback, m);
    if (XtHasCallbacks(m->widget, XmNvalueChangedCallback) == XtCallbackHasSome)
        XtRemoveCallback(m->widget, XmNvalueChangedCallback, 
                         menuActivateCallback, m);
    XtRemoveCallback(m->widget, XmNdestroyCallback, destroyMenuCallback, m);

    free((SMenu *) data);
}

static void
menuActivateCallback(Widget widget, XtPointer data, XtPointer event_data)
{
    SMenu *m = (SMenu *) data;
    if (m) {
        XtVaSetValues(m->widget,
                      XmNset, m->flags & SW_MENU_CHECKED ? True : False, NULL);
         
        callCommandCallback(m->wnd->widget, swToPtr(m->id));
    }
}

static void
menuArmCallback(Widget widget, XtPointer data, XtPointer event_data)
{
    SMenu *m = (SMenu *) data;
    if (m) {
        callHighlightCallback(m->wnd->widget, swToPtr(m->id));
    }
}

static void
menuDisarmCallback(Widget widget, XtPointer data, XtPointer event_data)
{
    SMenu *m = (SMenu *) data;
    if (m) {
        callHighlightCallback(m->wnd->widget, NULL);
    }
}

/* 
   Make a shell like regular expressions like "*.[wx][rm][ll]"
   from a Windows like "*.wrl;*.xml" filetypelist
*/

char *mkfilter(char* filters) {
    int i,j;
    char* filter;
    int numpatterns;
    char** patterns;
    int maxlen;
    char* ret;
    int nextcharto;
    int abortflag=FALSE;

    filter=mystrdup(filters);
    /* how much patterns ? */
    numpatterns=1;
    strtok(filter,";");
    while (strtok(NULL,";")!=NULL)
        numpatterns++;
    patterns=(char**) malloc((numpatterns+1)*sizeof(char*));
    free(filter);
    filter=mystrdup(filters);
    /* split string into pattern */
    patterns[i=0]=strtok(filter,";");
    while ((patterns[++i]=strtok(NULL,";"))!=NULL);
    /* how long can be returnvalue string ? */
    maxlen=0;
    for (i=0;i<numpatterns;i++)
        if (strlen(patterns[i])>maxlen)
            maxlen=strlen(patterns[i]);
    /* build return value */
    nextcharto=0;
    ret=(char*) malloc((numpatterns+2)*maxlen+1);
    for (i=0;i<maxlen;i++) {
        /* collect characters to a [123] string */
        int samecharactersflag=TRUE;
        int nextcharforward=0;
        ret[nextcharto+nextcharforward++]='[';
        for (j=0;j<numpatterns;j++) {
            /* test if pattern ended */
            if (i>=strlen(patterns[j])) {
                abortflag=TRUE;
                break;
            }
            ret[nextcharto+nextcharforward++]=patterns[j][i];         
            if (j!=0)
                if (patterns[j][i]!=patterns[j-1][i])
                    samecharactersflag=FALSE;
        }
        if (abortflag==TRUE) {
            ret[nextcharto++]='*';
            break;
        }
        ret[nextcharto+nextcharforward++]=']';
        /* if all characters are the same, use this character instead */
        if (samecharactersflag==TRUE) 
            ret[nextcharto++]=patterns[0][i];
        else
            nextcharto+=nextcharforward;
    }
    ret[nextcharto]=0;
    free(filter);
    free(patterns);
    return ret;
}

static int
doFileDialog(const char *title, SWND wnd, const char *filters, char *buf,
             int len, int flags, const char* ext)
{
    Widget dlg;
    Arg av[10];
    int ac = 0;
    XmString str, tstr;
    char *b;
    int rc;
    int cont;
    char *newFilters;

    if (!wnd || !buf || !filters) return 0;

    swChAppleDir();

    newFilters=mystrdup(mkfilter((char *)filters));

    str = XmStringCreateLocalized((String) newFilters);
    XtSetArg(av[ac], XmNpattern, str);  ac++;
    tstr = XmStringCreateLocalized((String) title);
    XtSetArg(av[ac], XmNdialogTitle, tstr);  ac++;
    XtSetArg(av[ac], XmNdialogStyle, XmDIALOG_PRIMARY_APPLICATION_MODAL); ac++;
    XtSetArg(av[ac], XmNdeleteResponse, XmDO_NOTHING);  ac++;
    XtSetArg(av[ac], XmNvisual, TheVisual->visual);  ac++;
/*
    XtSetArg(av[ac], XmNfileFilterStyle, XmFILTER_NONE);  ac++;
*/

    dlg = XmCreateFileSelectionDialog(wnd->shell ? wnd->shell : wnd->widget,
                                      (String) title, av, ac);
    XtAddCallback(dlg, XmNokCallback, dlgBtnCallback, (XtPointer) 1);
    XtAddCallback(dlg, XmNcancelCallback, dlgBtnCallback, (XtPointer) 2);
    XmAddWMProtocolCallback(XtParent(dlg), WM_DELETE_WINDOW, dlgBtnCallback, 
                            (XtPointer) 2);
    if (buf[0] != 0)
        XmTextFieldSetString(XmFileSelectionBoxGetChild(dlg, XmDIALOG_TEXT), 
                             buf);
                         
    XtManageChild(dlg);
    XmStringFree(str);
    XmStringFree(tstr);
    do {
        cont = FALSE;
        rc = swModalLoop();
        if (rc == 1) {
            XtVaGetValues(dlg, XmNtextString, &str, NULL);
            XmStringGetLtoR(str, XmFONTLIST_DEFAULT_TAG, &b);
            strncpy(buf, b, len);
            XtFree(b);
            XmStringFree(str);

            /* attach default file extension, if no extension given */
            if (ext != NULL) {
                char* path = mystrdup(buf);
                char* freepath = path;
                while (strstr(path, "/") != NULL)
                    path = strstr(path, "/") + 1; 
                if (strstr(path, ".") == NULL) {
                    int bufLen = strlen(buf);
                    int extLen = strlen(ext);
                    if (bufLen < extLen)
                        strcat(buf, ext);
                    else {
                        if (strcasecmp(buf + bufLen - extLen, ext) != 0)
                            if ((bufLen + extLen) < len)
                                strcat(buf, ext);
                    }
                }
                free(freepath);
            }

            if (!(flags & MAY_BE_FILE))
                if ( ((flags & (FILE_MUST_EXIST)) || 
                      (flags & (NOT_A_DIRECTORY)))
                     && !fileExists(buf)) {
                char msg[1024];
                mysnprintf(msg, 1024, "%s:  %s\n", buf, strerror(errno));
                rc = messageBox(XtParent(dlg), msg, title, SW_MB_OK,
                                SW_MB_WARNING);
                cont = TRUE;
            }
            if (flags & OVERWRITE_PROMPT && fileExists(buf)) {
                char msg[1024];
                mysnprintf(msg, 1024, "Replace existing file %s?", buf);
                rc = messageBox(XtParent(dlg), msg, title, SW_MB_YESNO,
                                SW_MB_WARNING);
                if (rc == IDYES) {
                    rc = IDOK;
                } else if (rc == IDNO || rc == IDCANCEL) {
                    rc = IDCANCEL;
                    cont = TRUE;
                }
            }
            if (flags & NOT_A_DIRECTORY && dirExists(buf)) {
                char msg[1024];
                mysnprintf(msg, 1024, 
                           "choose a %s file, not a directory: %s\n", 
                           newFilters, buf);
                rc = messageBox(XtParent(dlg), msg, title, SW_MB_OK,
                                SW_MB_WARNING);
                cont = TRUE;
            }
            if (flags & FILE_EXECUTABLE) {
                struct stat fileStat;
                if ((stat(buf, &fileStat) == 0) && 
                    !((fileStat.st_mode & S_IXUSR) | 
                      (fileStat.st_mode & S_IWGRP) | 
                      (fileStat.st_mode & S_IXOTH)) 
                    ) {
                    char msg[1024];
                    mysnprintf(msg, 1024, 
                              "please choose a executable file: %s is not executable\n", 
                               buf);
                    rc = messageBox(XtParent(dlg), msg, title, SW_MB_OK,
                                    SW_MB_WARNING);
                    cont = TRUE;
                }
            }
        }
    } while (cont);
    XtDestroyWidget(dlg);
    free(newFilters);
    return rc == 1;
}

#ifndef MOTIF_DRAGNDROP
static SWND
findDropTarget(SWND wnd)
{
    Widget widget;

    if (wnd->dropCallback) return wnd;

    for (widget = wnd->widget; widget != NULL; widget = XtParent(widget)) {
        XtVaGetValues(widget, XmNuserData, &wnd, NULL);
        if (wnd && wnd->dropCallback) return wnd;
    }
    return NULL;
}

static void
doDragOver(SWindow *wnd, int x, int y, int modifiers)
{
    SWindow *target = findDropTarget(wnd);
    if (target != NULL) {
        int operation = target->dropCallback(target->data,
                                             SW_DRAG_OVER, x, y, modifiers);
        updateCursor(XtWindow(wnd->widget), operation);
    }
}

static void
doDrop(SWindow *wnd, int x, int y, int modifiers)
{
    SWindow *target = findDropTarget(wnd);
    if (target != NULL) {
        target->dropCallback(target->data, SW_DROP, x, y, modifiers);
        InDragNDrop = FALSE;
        XUndefineCursor(TheDisplay, XtWindow(wnd->widget));
        XtDestroyWidget(TheDragPopup);
    }
}
#endif

static void
input(Widget widget, XtPointer data, XtPointer event_data)
{
    SWindow *w = (SWindow *) data;
    XmDrawingAreaCallbackStruct *cbs =
         (XmDrawingAreaCallbackStruct *) event_data;
    XEvent *event = (XEvent *) cbs->event;

    if (!w) return;

    if (event->type == MotionNotify) {
        XMotionEvent *e = (XMotionEvent *) event;
#ifndef MOTIF_DRAGNDROP
        if (InDragNDrop) {
            XtMoveWidget(TheDragPopup, e->x_root - 24, e->y_root - 8);
            doDragOver(w, e->x, e->y, getModifiers(e->state));
        } else
#endif
        if (w->mouseCallback)
            (w->mouseCallback)(w->data, e->x, e->y, getModifiers(e->state));
    } else if (event->type == KeyPress) {
        XKeyEvent *e = (XKeyEvent *) event;
#ifdef HAVE_XKBKEYCODETOKEYSYM
        KeySym k = XkbKeycodeToKeysym(e->display, e->keycode, 0, 0);
#else
        KeySym k = XKeycodeToKeysym(e->display, e->keycode, 0);
#endif
#ifndef MOTIF_DRAGNDROP
        if (InDragNDrop) {
            doDragOver(w, e->x, e->y, queryModifiers());
        } else
#endif
        if ((w->keyCallback) && (k < 65536))
            (w->keyCallback)(w->data, _keycodeMap[k], TRUE, e->x, e->y, getModifiers(e->state));
    } else if (event->type == KeyRelease) {
        XKeyEvent *e = (XKeyEvent *) event;
#ifdef HAVE_XKBKEYCODETOKEYSYM
        KeySym k = XkbKeycodeToKeysym(e->display, e->keycode, 0, 0);
#else
        KeySym k = XKeycodeToKeysym(e->display, e->keycode, 0);
#endif
#ifndef MOTIF_DRAGNDROP
        if (InDragNDrop) {
            doDragOver(w, e->x, e->y, queryModifiers());
        } else
#endif
        if (w->keyCallback && (k < 65536))
            (w->keyCallback)(w->data, _keycodeMap[k], FALSE, e->x, e->y, 
                             getModifiers(e->state));
    } else if (event->type == ButtonPress) {
        XButtonEvent *e = (XButtonEvent *) event;
#ifdef MOTIF_DRAGNDROP
        TheLastButtonPress = *event;
#endif
        if (w->keyCallback && (e->button < 65536))
            (w->keyCallback)(w->data, _keycodeMap[e->button], TRUE,
                             e->x, e->y, getModifiers(e->state));
    } else if (event->type == ButtonRelease) {
        XButtonEvent *e = (XButtonEvent *) event;
#ifndef MOTIF_DRAGNDROP
        if (InDragNDrop) {
            doDrop(w, e->x, e->y, getModifiers(e->state));
        } else
#endif
        if (w->keyCallback && (e->button < 65536))
            (w->keyCallback)(w->data, _keycodeMap[e->button], FALSE,
                             e->x, e->y, getModifiers(e->state));
    }
}

static void
enterEvent(Widget w, XtPointer data, XEvent *event,
            Boolean *continue_to_dispatch)
{
    SWindow *wnd = (SWindow *) data;

    if (wnd->enterCallback)
        wnd->enterCallback(wnd->data, event->type == EnterNotify);
    *continue_to_dispatch = False;
}

static void
acceleratorHandler(Widget widget, XtPointer data, XEvent *event,
           Boolean *continue_to_dispatch)
{
    SWindow *w, *mainWnd = (SWindow *) data;
    XKeyEvent *e = (XKeyEvent *) event;
    int mask = ShiftMask | ControlMask | Mod1Mask;
#ifdef HAVE_XKBKEYCODETOKEYSYM
    KeySym key = XkbKeycodeToKeysym(e->display, e->keycode, 0, 0);
#else
    KeySym key = XKeycodeToKeysym(e->display, e->keycode, 0);
#endif

    if (XtClass(widget) == xmTextFieldWidgetClass && key == XK_Escape) {
        XtVaGetValues(widget, XmNuserData, &w, NULL);
        if (w && w->commandCallback) {
            w->commandCallback(w->data, FALSE);
            return;
        }
    }
    if (mainWnd->accelTable && acceleratorsEnabled) {
        int i, n = mainWnd->accelTable->size;

        for (i = 0; i < n; i++) {
            Accelerator *accel = mainWnd->accelTable->data + i;
            if (accel->key == key && (e->state & mask) == accel->state) {
                callCommandCallback(mainWnd->widget, swToPtr(accel->id));
                break;
             }
        }
    }
    *continue_to_dispatch = True;
}

static void
expose(Widget widget, XtPointer data, XtPointer event_data)
{
    SWindow *wnd = (SWindow *) data;
    XmDrawingAreaCallbackStruct *cbs =
          (XmDrawingAreaCallbackStruct *) event_data;
    XExposeEvent *event = (XExposeEvent *) cbs->event;

    if (!wnd || !wnd->exposeCallback) return;

    swInvalidateRect(wnd, event->x, event->y, event->width, event->height);
}

static void
update()
{
    int i;
    for (i = 0; i < NumWindowsToUpdate; i++) {
        SWND w = WindowsToUpdate[i];

        if (w && w->invx1 >= 0 && w->exposeCallback) {
            w->exposeCallback(w->data, w->invx1, w->invy1,
                              w->invx2 - w->invx1 + 1, 
                              w->invy2 - w->invy1 + 1);
            w->invx1 = w->invx2 = w->invy1 = w->invy2 = -1;
        }
    }
    NumWindowsToUpdate = 0;
}

void swUpdate(void)
{
    int running = 1;
    do {
        running = 0;
        if (XtAppPending(TheAppContext) != 0) {
            XtAppProcessEvent(TheAppContext, XtIMAll);
            running = 1;
            update();
        }
    } while (running);
}


static void
onDeleteWindow(Widget widget, XtPointer data, XtPointer event_data)
{
    SWindow *wnd = (SWindow *) data;

    XtRemoveCallback(wnd->widget, XmNexposeCallback, expose, wnd);
    XtRemoveCallback(wnd->widget, XmNinputCallback, input, wnd);
    XtRemoveCallback(wnd->widget, XmNresizeCallback, resize, wnd);

    deleteFromWindowsToUpdate(wnd);
    if (wnd && wnd->closeCallback)
        wnd->closeCallback(wnd->data);
    else 
        swExit();
}

static void
onDeleteDialog(Widget widget, XtPointer data, XtPointer event_data)
{
    SWindow *wnd = (SWindow *) data;

    deleteFromWindowsToUpdate(wnd);
    swExitModalLoop(IDCANCEL);
}

static void
timerProc(XtPointer closure, XtIntervalId *id)
{
    STimer *t = (STimer *) closure;

    if (t->timerCallback(t->data))
        t->id = XtAppAddTimeOut(TheAppContext, t->timeout, timerProc, t);
}

static void
dlgBtnCallback(Widget widget, XtPointer data, XtPointer event_data)
{
    swExitModalLoop(swFromPtr(data));
}

#ifdef MOTIF_DRAGNDROP
static void
dragProc(Widget widget, XtPointer data, XtPointer event_data)
{
    XmDragProcCallbackStruct *cbs = (XmDragProcCallbackStruct *) event_data;
    SWindow *wnd;
    int op = XmDROP_NOOP;
    int modifiers;
    Widget icon;

    modifiers = queryModifiers();
                         
    XtVaGetValues(widget, XmNuserData, &wnd, NULL);
    if (wnd && wnd->dropCallback) switch (cbs->reason) {
      case XmCR_DROP_SITE_ENTER_MESSAGE:
        op = wnd->dropCallback(wnd->data, SW_DRAG_ENTER,
                               cbs->x, cbs->y, modifiers);
        break;
      case XmCR_OPERATION_CHANGED:
        /* FALL THRU ? */
      case XmCR_DROP_SITE_MOTION_MESSAGE:
        op = wnd->dropCallback(wnd->data, SW_DRAG_OVER,
                               cbs->x, cbs->y, modifiers);
        break;
      case XmCR_DROP_SITE_LEAVE_MESSAGE:
        op = wnd->dropCallback(wnd->data, SW_DRAG_LEAVE,
                               cbs->x, cbs->y, modifiers);
        break;
    }
    cbs->dropSiteStatus = op != 0 ? XmDROP_SITE_VALID : XmDROP_SITE_INVALID;
    cbs->operation = opToMop(op);
    if (cbs->operation == XmDROP_COPY) {
        icon = TheCopyIcon;
    } else if (cbs->operation == XmDROP_LINK) {
        icon = TheLinkIcon;
    } else if (cbs->operation == XmDROP_MOVE) {
        icon = TheMoveIcon;
    } else {
        icon = NULL;
    }
    XtVaSetValues(TheDragContext, XmNoperationCursorIcon, icon, NULL);
    cbs->animate = False;
}

static void
dropProc(Widget widget, XtPointer data, XtPointer event_data)
{
    XmDropProcCallbackStruct *cbs = (XmDropProcCallbackStruct *) event_data;
    XmDropTransferEntryRec  transferEntries[1];
    Arg av[10];
    int ac = 0;
    SWindow *wnd;
    int modifiers = queryModifiers();

    XtVaGetValues(widget, XmNuserData, &wnd, NULL);

    if (cbs->dropAction == XmDROP) {
        transferEntries[0].target = XA_STRING;
        transferEntries[0].client_data = (XtPointer) widget;

        XtSetArg(av[ac], XmNdropTransfers, transferEntries); ac++;
        XtSetArg(av[ac], XmNnumDropTransfers, 1); ac++;
        XtSetArg(av[ac], XmNtransferProc, transferProc); ac++;
        wnd->dropCallback(wnd->data, SW_DROP, cbs->x, cbs->y, modifiers);
    } else {
        XtSetArg(av[ac], XmNtransferStatus, XmTRANSFER_FAILURE); ac++;
        XtSetArg(av[ac], XmNnumDropTransfers, 0); ac++;
        cbs->operation = XmDROP_NOOP;
        cbs->dropSiteStatus = XmINVALID_DROP_SITE;
    }
    XmDropTransferStart(cbs->dragContext, av, ac);
}

static void
dragFinish(Widget widget, XtPointer data, XtPointer event_data)
{
    InDragNDrop = FALSE;
}

static Boolean
convertProc(Widget widget, Atom *selection, Atom *target, Atom *typeRtn,
             XtPointer *valueRtn, unsigned long *lengthRtn, int *formatRtn,
             unsigned long *max_lengthRtn, XtPointer data,
             XtRequestId *request_id)
{
    const char *tmpString = "blah";
    char *passtext;
         
    if (*target != XA_STRING)
        return(False);
       
    passtext = XtMalloc(strlen(tmpString)+1);
    strcpy(passtext, tmpString);
 
    *typeRtn = XA_STRING;
    *valueRtn = (XtPointer) passtext;
    *lengthRtn = strlen(passtext);
    *formatRtn = 8;
    return(True);
}

static void
transferProc(Widget widget, XtPointer data, Atom *selection, Atom *type,
              XtPointer value, unsigned long *length, int *format)
{
    XtFree(value);
}

static Widget
createDragIcon(Widget widget, Pixmap pixmap, Pixmap mask,
                int width, int height)
{
    Arg av[10];
    int ac = 0;

    XtSetArg(av[ac], XmNwidth, width);  ac++;
    XtSetArg(av[ac], XmNheight, height);  ac++;
    XtSetArg(av[ac], XmNdepth, 1);  ac++;
    XtSetArg(av[ac], XmNpixmap, pixmap);  ac++;
    if (mask) XtSetArg(av[ac], XmNmask, mask);  ac++;
    return XmCreateDragIcon(widget, (String) "swDragIcon", av, ac);
}

/*
 * createSourceCursor()
 * 
 * given a multiplane (colour) pixmap, create a monochrome bitmap and mask.
 * uses a braindead algo for conversion to mono (luminance threshold).
 */

static void
createSourceCursor(Pixmap pixmap, int xPos, int yPos, int width, int height,
           Widget widget, Pixmap *pixmapOut, Pixmap *maskOut)
{
    Pixel bg;
    XImage *imageIn, *maskImage, *image;
    GC gc;
    Window window = XtWindow(widget);
    int x, y;
    XColor c;

    imageIn = XGetImage(TheDisplay, pixmap, xPos, yPos, width, height,
                        AllPlanes, ZPixmap);
    maskImage = XCreateImage(TheDisplay, TheVisual->visual, 1, XYPixmap,
                             0, malloc(width * height), width, height, 8, 0);
    image = XCreateImage(TheDisplay, TheVisual->visual, 1, XYPixmap,
                         0, malloc(width * height), width, height, 8, 0);
    *pixmapOut = XCreatePixmap(TheDisplay, window, width, height, 1);
    *maskOut = XCreatePixmap(TheDisplay, window, width, height, 1);
    XtVaGetValues(widget, XmNbackground, &bg, NULL);

    for (y = 0; y < height; y++) {
        for (x = 0; x < width; x++) {
            int red;
            int green;
            int blue;
            c.pixel = XGetPixel(imageIn, x, y);
            if (truecolor_mode)
                red = swX11ColorToRed8Bit(c.pixel) << 8;
                green = swX11ColorToGreen8Bit(c.pixel) << 8;
                blue = swX11ColorToBlue8Bit(c.pixel) << 8;
            } else {
                XQueryColor(TheDisplay, TheColormap, &c);
                red = c.red;
                green = c.green;
                blue = c.blue;
            }
            XPutPixel(image, x, y, red + green + blue < 32768 * 3);
            XPutPixel(maskImage, x, y, c.pixel != bg);
        }
    }
    gc = XCreateGC(TheDisplay, *pixmapOut, 0, NULL);
    XPutImage(TheDisplay, *pixmapOut, gc, image, 0, 0, 0, 0, width, height);
    XPutImage(TheDisplay, *maskOut, gc, maskImage, 0, 0, 0, 0, width, height);
    XFreeGC(TheDisplay, gc);
#ifndef DO_NOT_X_DESTROY_IMAGE
    /*
       XDestroyImage can crash during a advanced efence test
       cause XDestroyImage calls internally the free(3) function
       in a advanced efence test but explicit efence calls to malloc do not
       match Xlib intern free calls 
     */
    XDestroyImage(imageIn);
    XDestroyImage(image);
    XDestroyImage(maskImage);
#endif
}

static int
opToMop(int op)
{
    int mop = XmDROP_NOOP;

    if (op & SW_DRAG_MOVE)
        mop |= XmDROP_MOVE;
    if (op & SW_DRAG_COPY)
        mop |= XmDROP_COPY;
    if (op & SW_DRAG_LINK)
        mop |= XmDROP_LINK;

    return mop;
}

#else

static void
updateCursor(Window window, int operation)
{
    Cursor cursor;

    switch (operation) {
      case SW_DRAG_MOVE:
        cursor = TheMoveCursor;
        break;
      case SW_DRAG_COPY:
        cursor = TheCopyCursor;
        break;
      case SW_DRAG_LINK:
        cursor = TheLinkCursor;
        break;
      default:
        cursor = TheErrorCursor;
        break;
    }
    if (cursor)
        XDefineCursor(TheDisplay, window, cursor);
    else
        XUndefineCursor(TheDisplay, window);
}

static void
exposeDragDropWindow(Widget widget, XtPointer data, XtPointer event_data)
{
    GC gc = XCreateGC(TheDisplay, XtWindow(widget), 0, NULL);
    Dimension width, height;

    XtVaGetValues(widget, XmNwidth, &width, XmNheight, &height, NULL);
    XSetClipMask(TheDisplay, gc, TheDragMask);
    XSetClipOrigin(TheDisplay, gc, -TheDragPixmapOffsetX, -TheDragPixmapOffsetY);
    XCopyArea(TheDisplay, TheDragPixmap, XtWindow(widget),
              gc, TheDragPixmapOffsetX, TheDragPixmapOffsetY,
              width, height, 0, 0);
    XFreeGC(TheDisplay, gc);
}

#endif /* MOTIF_DRAGNDROP */

static const char *
parseMenu(const char *s1, char *s2, char *mnemonic)
{
    mnemonic[0] = '\0';
    while (*s1) {
        if (*s1 == '&') {
            mnemonic[0] = *(s1 + 1);
            mnemonic[1] = '\0';
            s1++;
        } else if (*s1 == '\t') {
            *s2 = '\0';
            return s1 + 1;
        } else {
            *s2++ = *s1++;
        }
    }
    *s2 = '\0';

    return NULL;
}

static int
getModifiers(int state)
{
    int modifiers = 0;

    if (state & ShiftMask) {
        modifiers |= SW_SHIFT;
    }
    if (state & ControlMask) {
        modifiers |= SW_CONTROL;
    }
    if (state & Mod1Mask) {
        modifiers |= SW_ALT;
    }
    return modifiers;
}

static int
queryModifiers()
{
    int x;
    Window window;
    unsigned state;

    XQueryPointer(TheDisplay, DefaultRootWindow(TheDisplay), &window, &window,
                  &x, &x, &x, &x, &state);
    return getModifiers(state);
}

static SMENU
findMenu(Widget widget, int id)
{
    int i, numChildren;
    WidgetList children;
    SMENU child;

    if (!widget) return NULL;

    XtVaGetValues(widget, XmNchildren, &children,
                          XmNnumChildren, &numChildren,
                          NULL);
    for (i = 0; i < numChildren; i++) {
        if (XtIsSubclass(children[i], xmToggleButtonWidgetClass)) {
            XtVaGetValues(children[i], XmNuserData, &child, NULL);
            if (child->id == id) return child;
        } else if (XtIsSubclass(children[i], xmCascadeButtonWidgetClass)) {
            Widget subMenu = NULL;
            XtVaGetValues(children[i], XmNsubMenuId, &subMenu, NULL);
            child = findMenu(subMenu, id);
            if (child) return child;
        }
    }
    return NULL;
}

extern void
insertMenuItem(Widget menu, SWND wnd, short pos, const char *text, int id)
{
    SMenu *m;
    char *newText = malloc(strlen(text) + 1);
    char mnemonic[2];
    const char *accelerator;
    
    accelerator = parseMenu(text, newText, mnemonic);
    mnemonic[1] = '\0';
    
    m = newSMenu();
    m->widget = XtVaCreateManagedWidget(newText, xmToggleButtonWidgetClass,
                                        menu,
                                        XmNpositionIndex, pos,
                                        XmNindicatorOn, True,
                                        XmNuserData, m,
                                        NULL);
    m->wnd = wnd;
    m->id = id;
    if (mnemonic[0]) {
        KeySym sym = XStringToKeysym(mnemonic);
        /* causes a invalid read 8 in valgrind */
        if (sym != NoSymbol)
            XtVaSetValues(m->widget, XmNmnemonic, XStringToKeysym(mnemonic), 
                          NULL);
    }
    if (accelerator) {
        XmString a = XmStringCreateLocalized((String) accelerator);
        XtVaSetValues(m->widget, XmNacceleratorText, a, NULL);
        XmStringFree(a);
    }


    XtAddCallback(m->widget, XmNarmCallback, menuArmCallback, m);
    XtAddCallback(m->widget, XmNdisarmCallback, menuDisarmCallback, m);
    XtAddCallback(m->widget, XmNvalueChangedCallback, menuActivateCallback, m);
    XtAddCallback(m->widget, XmNdestroyCallback, destroyMenuCallback, m);
    free(newText);
}

static void
resetRadioButtons(Widget widget)
{
    int i, numChildren;
    WidgetList children;
    unsigned char type;

    if (!widget) return;

    XtVaGetValues(XtParent(widget), XmNnumChildren, &numChildren,
                  XmNchildren, &children, NULL);

    for (i = 0; i < numChildren; i++) {
        if (children[i] != widget
            && XtClass(children[i]) == xmToggleButtonWidgetClass) {
            XtVaGetValues(children[i], XmNindicatorType, &type, NULL);
            if (type == XmONE_OF_MANY) {
                XtVaSetValues(children[i], XmNset, False, NULL);
            }
        }
    }
}

static void
callCommandCallback(Widget widget, void *id)
{
    SWND wnd;

    for (; widget != NULL; widget = XtParent(widget)) {
        XtVaGetValues(widget, XmNuserData, &wnd, NULL);
        if (wnd && wnd->commandCallback) {
            wnd->commandCallback(wnd->data, id);
            return;
        }
    }
}

static void
callHighlightCallback(Widget widget, void *id)
{
    SWND wnd;

    for (; widget != NULL; widget = XtParent(widget)) {
        XtVaGetValues(widget, XmNuserData, &wnd, NULL);
        if (wnd && wnd->highlightCallback) {
            wnd->highlightCallback(wnd->data, id);
            return;
        }
    }
}

static SWND
findMainWindow(Widget widget)
{
    SWND wnd;

    for (; widget != NULL; widget = XtParent(widget)) {
        XtVaGetValues(widget, XmNuserData, &wnd, NULL);
        if (wnd && wnd->mainWnd) return wnd;
    }
    return NULL;
}

static void
appendPair(STABLE table, const char *key, const char *value)
{
    if (table->size == table->capacity) {
        table->capacity *= 2;
        table->data = (Pair *) realloc(table->data,
                                        table->capacity * sizeof(Pair));
    }
    table->data[table->size].key = mystrdup(key);
    table->data[table->size].value = mystrdup(value);
    table->size++;
}

static int
messageBox(Widget parent, const char *text, const char *title, int type,
            int icon)
{
    int ac = 0;
    Arg av[10];
    XmString textStr, titleStr;
    Widget mbox, yes, no;
    int rc;
    unsigned char dialogType;

    if (!parent || !text || !title) return 0;

    XRaiseWindow(TheDisplay, XtWindow(parent));
    textStr = XmStringCreateLocalized((String) text);
    XtSetArg(av[ac], XmNmessageString, textStr);  ac++;
    titleStr = XmStringCreateLocalized((String) title);
    XtSetArg(av[ac], XmNdialogTitle, titleStr);  ac++;
    XtSetArg(av[ac], XmNdialogStyle, XmDIALOG_PRIMARY_APPLICATION_MODAL);  ac++;
    XtSetArg(av[ac], XmNautoUnmanage, False);  ac++;
    XtSetArg(av[ac], XmNvisual, TheVisual->visual);  ac++;
    switch(icon) {
      case SW_MB_NOICON:
        dialogType = XmDIALOG_MESSAGE;
        break;
      case SW_MB_ERROR:
        dialogType = XmDIALOG_ERROR;
        break;
      case SW_MB_INFORMATION:
        dialogType = XmDIALOG_INFORMATION;
        break;
      case SW_MB_QUESTION:
        dialogType = XmDIALOG_QUESTION;
        break;
      case SW_MB_WARNING:
        dialogType = XmDIALOG_WARNING;
        break;
    }
    XtSetArg(av[ac], XmNdialogType, dialogType);   ac++;
    mbox = XmCreateMessageDialog(parent, (String) title, av, ac);
    XmStringFree(textStr);
    XmStringFree(titleStr);
    XtUnmanageChild(XmMessageBoxGetChild(mbox, XmDIALOG_HELP_BUTTON));
    switch (type) {
      case SW_MB_OK:
        XtUnmanageChild(XmMessageBoxGetChild(mbox, XmDIALOG_CANCEL_BUTTON));
        break;
      case SW_MB_OKCANCEL:
        break;
      case SW_MB_YESNO:
        XtUnmanageChild(XmMessageBoxGetChild(mbox, XmDIALOG_CANCEL_BUTTON));
        /* FALL THRU */
      case SW_MB_YESNOCANCEL:
        XtUnmanageChild(XmMessageBoxGetChild(mbox, XmDIALOG_OK_BUTTON));
        yes = XtVaCreateManagedWidget(swYes, xmPushButtonWidgetClass,
                                      mbox, NULL);
        XtAddCallback(yes, XmNactivateCallback, dlgBtnCallback,
             (XtPointer) IDYES);
        no = XtVaCreateManagedWidget(swNo, xmPushButtonWidgetClass,
                                     mbox, NULL);
        XtAddCallback(no, XmNactivateCallback, dlgBtnCallback,
                     (XtPointer) IDNO);
        break;
         
    }
    XtAddCallback(mbox, XmNokCallback, dlgBtnCallback, (XtPointer) IDOK);
    XtAddCallback(mbox, XmNcancelCallback, dlgBtnCallback,
                  (XtPointer) IDCANCEL);
    XtManageChild(mbox);
    rc = swModalLoop();
    XtUnmanageChild(mbox);
    return rc;
}

static int
fileExists(const char *filename)
{
    struct stat buf;
    return stat(filename, &buf) == 0;
}

static int
dirExists(const char *filename)
{
    struct stat buf;
    if (stat(filename, &buf) == 0)
        if (buf.st_mode & S_IFDIR)
            return 1;
    return 0;
}

int swGetLastError(void)
{
    return errno;
}

#ifdef HAVE_XINPUT

void swInitXinputDevice(struct swXinput* xinput,const char *device)
    {
    XExtensionVersion* version;
    XDeviceInfo *info;
    int number_of_devices;
    int i;

    xinput->xinput=NULL;
    version=XGetExtensionVersion(TheDisplay,"XInputExtension");
    if ((version==NULL) || (version==(void*)NoSuchExtension)) {
        fprintf(stderr,"No XInputExtension (%s)\n",device);
        return;
    }
# ifdef DEBUG
    printf("X Input Extension Version %d.%d\n", 
           version->major_version,version->minor_version);
# endif
    XFree(version);
    info=XListInputDevices(TheDisplay,&number_of_devices);
    for (i=0;i<number_of_devices;++i) {
        int c;
        XAnyClassPtr xclass;
        xclass=info[i].inputclassinfo;
        if (strcmp(info[i].name,device)==0) {
            xinput->xinput=XOpenDevice(TheDisplay,info[i].id);
            xinput->number_buttons=0;
            xinput->number_axes=0;
            for (c=0;c<info[i].num_classes;++c) {
                if (xclass->class==ButtonClass) {
                    XButtonInfo *button_class=(XButtonInfo *)xclass;
                    xinput->number_buttons=button_class->num_buttons;
                }
                if (xclass->class==ValuatorClass) {
                    int i;
                    XValuatorInfo *axes_class=(XValuatorInfo *)xclass;
                    XAxisInfoPtr axes=axes_class->axes;
                    xinput->number_axes=axes_class->num_axes;
                    xinput->axes=malloc(axes_class->num_axes*sizeof(int));
                    xinput->minvalue=malloc(axes_class->num_axes*sizeof(int));
                    xinput->maxvalue=malloc(axes_class->num_axes*sizeof(int));
                    for (i=0;i<xinput->number_axes;i++) {
                        xinput->maxvalue[i]=axes->max_value;
                        xinput->minvalue[i]=axes->min_value;
                        axes++;
                    }
                }
                xclass=(XAnyClassPtr)((char*)xclass+xclass->length);
            }
        }
    }
    XFreeDeviceList(info); 
    if (xinput == NULL) {
        fprintf(stderr,"xinputdevice %s ",device);
        fprintf(stderr,"initialisation failed\n");
        return;
    }
}

void swListXinputDevice(int longlisting) {
    XExtensionVersion* version;
    XDeviceInfo *info;
    int number_of_devices;
    int i;

    version=XGetExtensionVersion(TheDisplay,"XInputExtension");
    if ((version==NULL) || (version==(void*)NoSuchExtension)) {
        fprintf(stderr,"No XInputExtension\n");
        return;
    }
    XFree(version);
    info=XListInputDevices(TheDisplay,&number_of_devices);
    for (i=0;i<number_of_devices;++i) {
        int c;
        XAnyClassPtr xclass;
        printf("\"%s\" ",info[i].name);
        if (longlisting) {
            if ((strcmp(info[i].name,"virtual_pointer")!=0) && 
                (strcmp(info[i].name,"keyboard")!=0))
                XOpenDevice(TheDisplay,info[i].id);
            xclass=info[i].inputclassinfo;
            for (c=0;c<info[i].num_classes;++c) {
                if (xclass->class==ValuatorClass) {
                    XValuatorInfo *axes_class=(XValuatorInfo *)xclass;
                    printf("axes: %d ",axes_class->num_axes);
                }
                if (xclass->class==ButtonClass) {
                    XButtonInfo *button_class=(XButtonInfo *)xclass;
                    printf("buttons: %d ",button_class->num_buttons);
                }
                xclass=(XAnyClassPtr)((char*)xclass+xclass->length);
            }       
        }
    printf("\n");
    }
}

void swQueryXinputDevice(struct swXinput* xinput) {
    int c;
    XDeviceState* state;
    XInputClass* xclass;
    state=XQueryDeviceState(TheDisplay,xinput->xinput);
    if (state==XI_BadDevice)
        xinput->xinput=NULL;
    xclass=state->data;
    for (c=0;c<state->num_classes;++c) {
        if (xclass->class==ButtonClass) {
            XButtonState  *button_class=(XButtonState *)xclass;
            xinput->number_buttons=button_class->num_buttons;
        }
        if (xclass->class==ValuatorClass) {
            XValuatorState *axes_class=(XValuatorState *)xclass;
            int i;
            for (i=0;i<axes_class->num_valuators;i++)
                xinput->axes[i]=axes_class->valuators[i];
        }
        xclass=(XInputClass*)((char*)xclass+xclass->length);
    }
}

void swCloseXinputDevice(struct swXinput* xinput) {
    if (xinput != NULL)
        XCloseDevice(TheDisplay,xinput->xinput);
}

int swGetNumXinputDevice(void)
{
    XExtensionVersion* version;
    int number_of_devices;

    version=XGetExtensionVersion(TheDisplay,"XInputExtension");
    if ((version==NULL) || (version==(void*)NoSuchExtension)) {
        return 0;
    }
    XFree(version);
    XListInputDevices(TheDisplay,&number_of_devices);
    return number_of_devices;
}

const char *swGetXinputDevice(int num_device)
{
    XExtensionVersion* version;
    XDeviceInfo *info;
    int number_of_devices;
    int i;

    version=XGetExtensionVersion(TheDisplay,"XInputExtension");
    if ((version==NULL) || (version==(void*)NoSuchExtension)) {
        fprintf(stderr,"No XInputExtension\n");
        return "";
    }
    XFree(version);
    info=XListInputDevices(TheDisplay,&number_of_devices);
    for (i=0;i<number_of_devices;++i) {
        if (i==num_device)
            return info[i].name;
    }
    return "";
}
#endif

/* 
 * swSetFontPath() based on
 * $Xorg: xset.c,v 1.6 2001/02/09 02:05:58 xorgcvs Exp $
 */

/*

Copyright 1985, 1998  The Open Group

Permission to use, copy, modify, distribute, and sell this software and its
documentation for any purpose is hereby granted without fee, provided that
the above copyright notice appear in all copies and that both that
copyright notice and this permission notice appear in supporting
documentation.

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
OPEN GROUP BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN
AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

Except as contained in this notice, the name of The Open Group shall not be
used in advertising or otherwise to promote the sale, use or other dealings
in this Software without prior written authorization from The Open Group.

*/
/* $XFree86: xc/programs/xset/xset.c,v 3.31 2003/05/27 22:27:09 tsi Exp $ */
/* Modified by Stephen so keyboard rate is set using XKB extensions */

#include <stdio.h>
#include <ctype.h>
#include <stdarg.h>
#include <stdlib.h>
#include <X11/Xos.h>
#include <X11/Xfuncs.h>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <X11/Xproto.h>
#include <X11/Xutil.h>
#ifdef HAVE_INCLUDE_X11_XMU
# include <X11/Xmu/Error.h>
#endif

/*
 * Set, add, or subtract the path according to before and after flags:
 *
 *      before  after   action
 *
 *         0      0     FontPath := path
 *        -1      0     FontPath := current - path
 *         0     -1     FontPath := current - path
 *         1      0     FontPath := path + current
 *         0      1     FontPath := current + path
 * original function name: set_font_path
 */
void
setFontPath(char *path, int special, int before, int after)
{
    char **directoryList = NULL; int ndirs = 0;
    char **currentList = NULL; int ncurrent = 0;
    Display *dpy = TheDisplay;

    if (special) {
        if (strcmp (path, "default") == 0) {
            XSetFontPath (dpy, NULL, 0);
            return;
        }
        if (strcmp (path, "rehash") == 0) {
            currentList = XGetFontPath (dpy, &ncurrent);
            if (!currentList) {
                fprintf (stderr, "unable to get current font path.\n");
                return;
            }
            XSetFontPath (dpy, currentList, ncurrent);
            XFreeFontPath (currentList);
            return;
        } 
        /*
         * for now, fall though and process keyword and directory list for
         * compatibility with previous versions.
         */
    }

    /*
     * parse the path list.  If before or after is non-zero, we'll need 
     * the current value.
     */

    if (before != 0 || after != 0) {
        currentList = XGetFontPath (dpy, &ncurrent);
        if (!currentList) {
            fprintf (stderr, "unable to get old font path.\n");
            before = after = 0;
        }
    }

    {
        /* count the number of directories in path */
        register char *cp = path;

        ndirs = 1;
        while ((cp = strchr(cp, ',')) != NULL) {
            ndirs++;
            cp++;
        }
    }

    directoryList = (char **) malloc (ndirs*sizeof (char *));
    if (!directoryList) fprintf (stderr, "out of memory for font path directory list");

    {
        /* mung the path and set directoryList pointers */
        int i = 0;
        char *cp = path;

        directoryList[i++] = cp;
        while ((cp = strchr(cp, ',')) != NULL) {
            directoryList[i++] = cp + 1;
            *cp++ = '\0';
        }
        if (i != ndirs) {
            fprintf (stderr, 
                     "internal error, only parsed %d of %d directories.\n",
                     i, ndirs);
            return;
        }
    }
            
    /*
     * now we have have parsed the input path, so we can set it
     */

    if (before == 0 && after == 0) {
        XSetFontPath (dpy, directoryList, ndirs);
    }

    /* if adding to list, build a superset */
    if (before > 0 || after > 0) {
        int nnew = ndirs + ncurrent;
        char **newList = (char **) malloc (nnew * sizeof (char *));
        
        if (!newList) fprintf (stderr, "out of memory");
        if (before > 0) { /* new + current */
            memmove( (char *) newList, (char *) directoryList, 
                   (unsigned) (ndirs*sizeof (char *)));
            memmove( (char *) (newList + ndirs), (char *) currentList, 
                        (unsigned) (ncurrent*sizeof (char *)));
            XSetFontPath (dpy, newList, nnew);
        } else if (after > 0) {
            memmove( (char *) newList, (char *) currentList, 
                   (unsigned) (ncurrent*sizeof (char *)));
            memmove( (char *) (newList + ncurrent), (char *) directoryList, 
                   (unsigned) (ndirs*sizeof (char *)));
            XSetFontPath (dpy, newList, nnew);
        } 
        free ((char *) newList);
    }

    /* if deleting from list, build one the same size */
    if (before < 0 || after < 0) {
        int i, j;
        int nnew = 0;
        char **newList = (char **) malloc (ncurrent * sizeof (char *));
        
        if (!newList) fprintf (stderr, "out of memory");
        for (i = 0; i < ncurrent; i++) {
            for (j = 0; j < ndirs; j++) {
                if (strcmp (currentList[i], directoryList[j]) == 0)
                  break;
            }
            /* if we ran out, then insert into new list */
            if (j == ndirs) newList[nnew++] = currentList[i];
        }
        if (nnew == ncurrent) {
            fprintf (stderr, "warning, no entries deleted from font path.\n");
        }
        XSetFontPath (dpy, newList, nnew);
        free ((char *) newList);
    }

    if (directoryList) free ((char *) directoryList);
    if (currentList) XFreeFontPath (currentList);

    return;
}

void *swToPtr(int id)
{
#ifdef HAVE_VOID_PTR_SIZE_8
    long lid = id;
    void *vid = (void *)lid;
#else
    void *vid = (void *)id;
#endif
    return vid;
}

int swFromPtr(void *vid)
{
#ifdef HAVE_VOID_PTR_SIZE_8
    long lid = (long)vid;
    int id = (int)lid;
#else
    int id = (int)vid;
#endif
    return id;
}

int swHasVisual(void)
{
    if (TheVisual == NULL)
        return 0;
    if (TheDisplay == NULL)
        return 0;
    if (!_running)
        return 0;
    return -1;
}

int swSystem(const char *command)
{
    return system(command);
/*
    int ret = 0;
    char *ptr = strchr((char *)command, ' ');
    int argc = 0;
    while (ptr != NULL) {
        argc++;
        ptr = strchr(ptr + 1, ' ');
    }
    char **argv = malloc(argc * sizeof(int));
    ptr = strchr((char *)command, ' ');
    argc = 0;
    while (ptr != NULL) {
        argv[argc] = ptr;
        argc++;
        ptr = strchr(ptr + 1, ' ');
    }
    int len = strlen(argv[0]) + 1;
    char *path = (char *)malloc(len);
    strcpy(path, argv[0]);
    int pid = fork();
    if (pid == 0) {
        ret = execv(path, argv + 1);
        free(path);
        free(argv);
    } else {
       int status;
       waitpid(-1, &status, 0);
    }
    return ret;
*/
}

int swSystemInDir(const char *command, const char *path)
{
    char cpath[MY_MAX_PATH];
    char *currentPath = getcwd(cpath, MY_MAX_PATH - 1);
    char dir[MY_MAX_PATH];
    char readDir[MY_MAX_PATH];
    strcpy(readDir, "dirname ");
    strcpy(readDir + strlen(readDir), path);
    FILE *filedes = popen(readDir, "r");
    if (fread(dir, MY_MAX_PATH - 1, 1, filedes) != 1) {
      // ??
      //    perror(path);
      //    return -1;
    }
    char *ptrN = strchr(dir, '\n');
    if (ptrN != NULL)
        *ptrN = 0;
    if (chdir(dir) != 0)
        return -1;
    int ret = swSystem(command);
    chdir(cpath);
    return ret;
}    
