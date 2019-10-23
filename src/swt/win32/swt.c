/*
 * swt.c - stephen's window toolkit, win32 implementation
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
   define DEFECT_LOAD_STRING to use own parser and resource.c instead of 
   possible defect LoadString()
*/
#define DEFECT_LOAD_STRING 1

#include "../../config.h"

#ifdef HAVE_WINDOWS_SPACEBALL
  /* 
     Windows spacenavigator type spaceballs with this driver
     work only with Micro$oft Windows XP
  */
# define _WIN32_WINNT 0x0501
#endif
#include <math.h>
#include <assert.h>
#include <stdio.h>
#include <stdarg.h>

#include <Windows.h>

#include <commctrl.h>
#include <GL/gl.h>

#include <sys/types.h>
#include <fcntl.h>

#include <stdio.h>
#include <sys/types.h>
#include <sys/timeb.h>
#include <process.h>
#include <io.h>

#ifdef DEFECT_LOAD_STRING
# include "res.h"
#endif

#include "../../mysnprintf.h"
#include "swt.h"
#include "swt_private.h"
#include "stereoview.h"

#ifndef PI
#define PI 3.141592653589
#endif

#ifndef DEG2RAD
#define DEG2RAD(D) ((D) * (PI / 180.0))
#endif

#ifndef RAD2DEG
#define RAD2DEG(R) ((R) * (180.0 / PI))
#endif

#define BUFSIZE 65536

typedef struct SDContext {
    HDC      hDC;
    HWND     hWnd; // for window DC
    HBITMAP  hBitmap;  // for bitmap DC
    COLORREF fgColor;
    int      lineStyle;
    int      capStyle;
    int      joinStyle;
    int      lineWidth;
    HPEN     hOldPen;
    HBITMAP  hOldBitmap;
    int      penDirty;
} SDContext;

typedef struct SBitmap {
    HBITMAP hBitmap;
} SBitmap;

typedef struct SCursor {
    HCURSOR hCursor;
} SCursor;

typedef struct SFont {
    HFONT hFont;
} SFont;

typedef struct STimer {
    HWND          hWnd;
    UINT          id;
    TimerCallback callback;
    void         *data;
} STimer;

typedef struct STree {
    SWND          wnd;
    HWND          hWnd;
    HIMAGELIST    hImageList;
    void         *data;
    TreeCallback  treeCallback;
} STree;

typedef struct SHeader {
    HWND            hWnd;
    void           *data;
    HeaderCallback  headerCallback;
} SHeader;

static void registerMainWindowClass(void);
static void adjustSize(int *width, int *height, DWORD style, BOOL menu);
static void updatePen(SDContext *gc, int capStyle);
static SWindow *newSWindow(void);
static STree *newSTree(void);
static SHeader *newSHeader(void);
static SDContext *newSDContext(void);
static SFont *newSFont(void);
static STimer *newSTimer(HWND hWnd, TimerCallback cb, void *data);
static LRESULT CALLBACK mainWndProc(HWND hWnd, UINT message, 
                                    WPARAM wParam, LPARAM lParam);
static LRESULT CALLBACK dialogProc(HWND hWnd, UINT message, 
                                   WPARAM wParam, LPARAM lParam);
static void updateBars(SWND wnd);
static int CALLBACK timerProc(HWND hWnd, UINT uMsg, void *idEvent, DWORD dwTime);
static HWND findFrame(HWND wnd);
static BOOL doAccelerators(MSG *msg);
static void trackMouseLeave(HWND wnd);
static void centerWindow(HWND hWnd, HWND hParent);
static int findExposeCallback(HWND wnd);
static void getTextMetrics(HFONT font, TEXTMETRIC *metrics);

static HGLRC        TheVisual = NULL;
static HINSTANCE    TheInstance;
static SFont       *TheDefaultFont;
static int          keyMap[65536];
static int          acceleratorsEnabled = TRUE;
static int          registered = FALSE;
static int          defaultIcon = -1;
static int          stereoType = NO_STEREO;
static int          want8Bit = FALSE;

#ifdef _M_AMD64
# undef SetWindowLong
# define SetWindowLong SetWindowLongPtr
# undef GWL_USERDATA
# define GWL_USERDATA GWLP_USERDATA
#endif

void 
swInitialize(HINSTANCE hInstance, int stereoTypeValue, int want8BitFlag)
{
    LOGFONT lf;
    int i;

    TheInstance = hInstance;
    stereoType = stereoTypeValue;
    want8Bit = want8BitFlag;

    InitCommonControls();
    OleInitialize(NULL);

    TheDefaultFont = newSFont();

    SystemParametersInfo(SPI_GETICONTITLELOGFONT, sizeof(LOGFONT), &lf, 0);
    TheDefaultFont->hFont = CreateFontIndirect(&lf);

    keyMap[VK_UP] = SW_UP;
    keyMap[VK_DOWN] = SW_DOWN;
    keyMap[VK_LEFT] = SW_KEY_LEFT;
    keyMap[VK_RIGHT] = SW_KEY_RIGHT;
    keyMap[VK_DELETE] = SW_DELETE;
    for (i = 0x41; i <= 0x5A; i++) {
        keyMap[i] = i + 0x20;
    }
}

extern int
swMainLoop(void)
{
    MSG msg;
    while (GetMessage(&msg, (HWND) NULL, 0, 0)) { 
        if (!doAccelerators(&msg)) {
            TranslateMessage(&msg);  
            DispatchMessage(&msg);
        }
    }
    return msg.wParam;
}

extern void
swExit(void)
{
    swDeleteFont(TheDefaultFont);
    TheDefaultFont = NULL;
    PostQuitMessage(0);
    swCleanup();
}

CleanupCallback cleanup=NULL;

extern void swSetCleanup(CleanupCallback cb)
{
    cleanup=cb;
}

extern void swCleanup(void)
{
    if (cleanup!=NULL)
        cleanup();
}

extern void
swSetDefaultIcon(int resource)
{
    defaultIcon = resource;
}

extern void
swGetErrorStr(char *buf, int size)
{
    FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                  NULL, GetLastError(), 
                  MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR) buf,
                  size, NULL);
}

extern void
swSetExposeCallback(SWND wnd, ExposeCallback exposeCallback)
{
    if (!wnd) return;

    wnd->exposeCallback = exposeCallback;
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
swSetEnterCallback(SWND wnd, EnterCallback cb)
{
    if (!wnd) return;

    wnd->enterCallback = cb;
}

extern void
swSetActivateCallback(SWND wnd, ActivateCallback activateCallback)
{
    if (!wnd) return;

    wnd->activateCallback = activateCallback;
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
swSetHighlightCallback(SWND wnd, HighlightCallback cb)
{
    if (!wnd) return;

    wnd->highlightCallback = cb;
}

extern void
swSetCloseCallback(SWND wnd, CloseCallback cb)
{
    if (!wnd) return;

    wnd->closeCallback = cb;
}

extern void
swSetClientData(SWND wnd, void *data)
{
    if (!wnd) return;

    wnd->data = data;
}

extern void
swCallCommandCallback(SWND wnd, void* id)
{
    if (wnd && wnd->commandCallback) {
        wnd->commandCallback(wnd->data, id);
    }
}

extern void
swDestroyWindow(SWND wnd)
{
    if (!wnd || !wnd->hWnd) return;

    if (wnd->bgBrush) DeleteObject(wnd->bgBrush);
    DestroyWindow(wnd->hWnd);
//    free(wnd);
}

/*********************/
/* Window Attributes */
/*********************/

extern void
swGetPosition(SWND wnd, int *x, int *y)
{
    RECT r;

    if (!wnd || !wnd->hWnd) return;

    GetClientRect(wnd->hWnd, &r);
    *x = r.left;
    *y = r.top;
}

extern void
swGetAbsPosition(SWND wnd, int *x, int *y)
{
    RECT r;
    if (!wnd || !wnd->hWnd) return;

    GetWindowRect(wnd->hWnd, &r);
    *x = r.left;
    *y = r.top;
}

extern void
swSetPosition(SWND wnd, int x, int y)
{
    RECT r;

    if (!wnd || !wnd->hWnd) return;

    GetClientRect(wnd->hWnd, &r);
    MoveWindow(wnd->hWnd, x, y, r.right - r.left, r.bottom - r.top, TRUE);
}

extern void
swMoveWindow(SWND wnd, int x, int y, int width, int height)
{
    if (!wnd) return;

    MoveWindow(wnd->hWnd, x, y, width, height, TRUE);
}

extern void
swGetScreenSize(int *width, int *height)
{ 
    *width=GetSystemMetrics(SM_CXSCREEN);
    *height=GetSystemMetrics(SM_CYSCREEN); 
}

extern void
swGetSize(SWND wnd, int *width, int *height)
{
    RECT r;

    if (!wnd || !wnd->hWnd) return;

    GetClientRect(wnd->hWnd, &r);
    *width = r.right - r.left;
    *height = r.bottom - r.top;
}

extern void
swGetTotalSize(SWND wnd, int *width, int *height)
{
    if (!wnd) return;

    swGetSize(wnd, width, height);
}

extern void
swSetSize(SWND wnd, int width, int height)
{
    RECT r;
    POINT p;

    if (!wnd || !wnd->hWnd) return;

    GetWindowRect(wnd->hWnd, &r);
    p.x = r.left;  p.y = r.top;
    ScreenToClient(GetParent(wnd->hWnd), &p);
    MoveWindow(wnd->hWnd, p.x, p.y, width, height, FALSE);
    if (wnd->child) {
        updateBars(wnd);
    }
}

extern void
swGetTitle(SWND wnd, char *buf, int len)
{
    if (!wnd || !wnd->hWnd) return;

    GetWindowText(wnd->hWnd, buf, len);
}

extern void
swSetTitle(SWND wnd, const char *title)
{
    if (!wnd || !wnd->hWnd) return;

    SetWindowText(wnd->hWnd, title);
}

extern void
swSetIcon(SWND wnd, SBITMAP bitmap, SBITMAP mask, int big)
{
    int size = big ? ICON_BIG : ICON_SMALL;
    ICONINFO info;
    HICON icon;

    if (!wnd || !bitmap || !mask) return;

    info.fIcon = TRUE;
    info.xHotspot = info.yHotspot = 0;
    info.hbmColor = bitmap->hBitmap;
    info.hbmMask = mask->hBitmap;
    icon = CreateIconIndirect(&info);
    SendMessage(wnd->hWnd, WM_SETICON, size, (LPARAM) icon);
    swDestroyBitmap(bitmap);
    swDestroyBitmap(mask);
}

extern int
swGetWindowColor(SWND wnd, int type)
{
    COLORREF c;

    if (!wnd) return 0;

    switch (type) {
      case SW_COLOR_WINDOW_FG:
        c = GetSysColor(COLOR_3DFACE);
        break;
      case SW_COLOR_WINDOW_BG:
        c = wnd->bgColor;
        break;
      case SW_COLOR_TSHADOW:
        c = GetSysColor(COLOR_3DHIGHLIGHT);
        break;
      case SW_COLOR_BSHADOW:
        c = GetSysColor(COLOR_3DSHADOW);
        break;
      case SW_COLOR_FACE:
        c = GetSysColor(COLOR_3DFACE);
        break;
      case SW_COLOR_HIGHLIGHT:
        c = GetSysColor(COLOR_HIGHLIGHT);
        break;
      case SW_COLOR_TEXT:
        c = GetSysColor(COLOR_WINDOWTEXT);
        break;
      case SW_COLOR_HIGHLIGHT_TEXT:
        c = GetSysColor(COLOR_HIGHLIGHTTEXT);
        break;
    }
    return (int) c;
}

extern void 
swDeIconifyWindow(SWND wnd)
{
    if (!wnd || !wnd->hWnd) return;

    OpenIcon(wnd->hWnd);
}

extern void 
swIconifyWindow(SWND wnd)
{
    if (!wnd || !wnd->hWnd) return;

    CloseWindow(wnd->hWnd);
}

extern int
swIsHidden(SWND wnd)
{
    return wnd->isHidden;
}

extern void
swShowWindow(SWND wnd)
{
    if (!wnd || !wnd->hWnd) return;

    ShowWindow(wnd->hWnd, SW_SHOW);

    wnd->isHidden = FALSE;
}

extern void
swHideWindow(SWND wnd)
{
    if (!wnd || !wnd->hWnd) return;

    ShowWindow(wnd->hWnd, SW_HIDE);

    wnd->isHidden = TRUE;
}

extern void
swMaximizeWindow(SWND wnd)
{
    if (!wnd || !wnd->hWnd) return;

    ShowWindow(wnd->hWnd, SW_SHOWMAXIMIZED);
}

extern void
swSetWindowBG(SWND wnd, int color)
{
    if (!wnd) return;

    wnd->bgColor = color;
    if (wnd->bgBrush) {
        DeleteObject(wnd->bgBrush);
    }
    wnd->bgBrush = CreateSolidBrush(color);
}

extern int
swIsVisible(SWND wnd)
{
    if (!wnd) return FALSE;

    return (GetWindowLong(wnd->hWnd, GWL_STYLE) & WS_VISIBLE) != 0;
}

extern int
swIsMaximized(SWND wnd)
{
    WINDOWPLACEMENT wp;

    if (!wnd) return FALSE;

    wp.length = sizeof(WINDOWPLACEMENT);
    GetWindowPlacement(wnd->hWnd, &wp);
    return wp.showCmd == SW_SHOWMAXIMIZED;
}

extern void
swEnableWindow(SWND wnd, int enable)
{
    if (!wnd) return;

    EnableWindow(wnd->hWnd, (BOOL) enable);
}

extern void
swInvalidateRect(SWND wnd, int x, int y, int width, int height)
{
    RECT r;

    if (!wnd || !wnd->hWnd) return;

    r.left = x;  r.right = x + width;
    r.top = y;   r.bottom = y + height;

    InvalidateRect(wnd->hWnd, &r, TRUE);
}

extern void
swInvalidateWindow(SWND wnd)
{
    if (!wnd) return;

    InvalidateRect(wnd->hWnd, NULL, TRUE);
}

extern void
swSetFocus(SWND wnd)
{
    if (!wnd) return;

    SetFocus(wnd->hWnd);
}

extern void
swEnableAccelerators(int enable)
{
    acceleratorsEnabled = enable;
}

static timerDisabled = 0;

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

    if (!wnd || !cb) return 0;

    t = newSTimer(wnd->hWnd, cb, data);
    t->id = SetTimer(wnd->hWnd, (UINT_PTR)t, timeout, (TIMERPROC) timerProc);
    return t;
}

extern void
swKillTimer(STIMER timer)
{
    if (!timer) return;

    KillTimer(timer->hWnd, timer->id);
    free(timer);
}


/***************/
/* Main Window */
/***************/

extern SWND
swCreateMainWindow(const char *text, int x, int y, int width, int height)
{
    SWindow *wnd = newSWindow();
    
    adjustSize(&width, &height, WS_OVERLAPPEDWINDOW, TRUE);
    wnd->hWnd = CreateWindow("swMainWndClass", text, 
                             WS_OVERLAPPEDWINDOW,
                             x, y, width, height,
                             NULL, NULL, TheInstance, NULL);

    SetWindowLong(wnd->hWnd, GWL_USERDATA, (LONG_PTR)wnd);
    SendMessage(wnd->hWnd, WM_SETFONT, (WPARAM) TheDefaultFont->hFont, MAKELPARAM(FALSE, 0));

    return wnd;
}

/**********/
/* Canvas */
/**********/

extern SWND
swCreateCanvas(const char *title, int x, int y, int width, int height, 
                SWND parent)
{
    SWindow *wnd = newSWindow();

    wnd->hWnd = CreateWindow("swMainWndClass", title, WS_CHILD | WS_VISIBLE,
                             x, y, width, height,
                             parent->hWnd, NULL, NULL, NULL);
    SendMessage(wnd->hWnd, WM_SETFONT, (WPARAM) TheDefaultFont->hFont, 
                MAKELPARAM(FALSE, 0));

    trackMouseLeave(wnd->hWnd);

    SetWindowLong(wnd->hWnd, GWL_USERDATA, (LONG_PTR)wnd);

    return wnd;
}

/*********/
/* Popup */
/*********/

extern SWND
swCreatePopup(const char *text, int x, int y, int width, int height,
              SWND parent)
{
    SWindow *s = newSWindow();

    adjustSize(&width, &height, WS_POPUP | WS_BORDER, FALSE);
    s->hWnd = CreateWindow("swMainWndClass", text, WS_POPUP | WS_BORDER, 
                           x, y, width, height,
                           NULL, NULL, NULL, NULL);

    SetWindowLong(s->hWnd, GWL_USERDATA, (LONG_PTR)s);
    SendMessage(s->hWnd, WM_SETFONT, (WPARAM) TheDefaultFont->hFont, MAKELPARAM(FALSE, 0));

    return s;
}

/*************/
/* ScrollBar */
/*************/

extern SWND
swCreateScrollBar(int flags, int x, int y, int width, int height, 
                  int min, int max, int value, int visible, SWND parent)
{
    SCROLLINFO info;

    SWindow *sb = newSWindow();

    int orient = (flags & SW_VERTICAL) ? SBS_VERT : SBS_HORZ;

    HWND hWnd = CreateWindow("SCROLLBAR", NULL,
                             WS_CHILD | WS_VISIBLE | orient,
                             x, y, width, height, parent->hWnd,
                             0, NULL, NULL);

    SetWindowLong(hWnd, GWL_USERDATA, (LONG_PTR)sb);

    info.cbSize = sizeof(SCROLLINFO);
    info.fMask = SIF_PAGE | SIF_POS | SIF_RANGE;
    info.nMin = min;
    info.nMax = max - 1;
    info.nPage = visible;
    info.nPos = value;
    SetScrollInfo(hWnd, SB_CTL, &info, TRUE);

    sb->hWnd = hWnd;
    return sb;
}

extern void
swScrollBarSetValue(SWND sb, int value)
{
    if (!sb || !sb->hWnd) return;

    SetScrollPos(sb->hWnd, SB_CTL, value, TRUE);
}

extern void
swScrollBarSetRange(SWND sb, int min, int max)
{
    if (!sb || !sb->hWnd) return;

    SetScrollRange(sb->hWnd, SB_CTL, min, max, TRUE);
}

extern void
swScrollBarSetVisible(SWND sb, int visible)
{
    SCROLLINFO info;

    if (!sb || !sb->hWnd) return;

    info.fMask = SIF_PAGE;
    info.nPage = visible;
    SetScrollInfo(sb->hWnd, SB_CTL, &info, TRUE);
}

/**********/
/* Button */
/**********/

extern SWND
swCreateButton(const char *text, int x, int y, int width, int height,
                SWND parent)
{
    SWindow *w = newSWindow();
    
    HWND hWnd = CreateWindow("BUTTON", text, WS_CHILD | WS_VISIBLE,
                             x, y, width, height, 
                             parent->hWnd, 0, NULL, NULL);

    SetWindowLong(hWnd, GWL_USERDATA, (LONG_PTR)w);
    SendMessage(hWnd, WM_SETFONT, (WPARAM) TheDefaultFont->hFont,
                MAKELPARAM(FALSE, 0));
    w->hWnd = hWnd;

    return w;
}

/************/
/* CheckBox */
/************/

extern SWND
swCreateCheckBox(const char *text, int x, int y, int width, int height,
                  SWND parent)
{
    SWindow *w;

    if (!parent || !parent->hWnd) return NULL;

    w = newSWindow();
    
    w->hWnd = CreateWindow("BUTTON", text, 
                           WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX,
                           x, y, width, height, parent->hWnd, 0, NULL, NULL);

    SetWindowLong(w->hWnd, GWL_USERDATA, (LONG_PTR)w);
    SendMessage(w->hWnd, WM_SETFONT, (WPARAM) TheDefaultFont->hFont,
                MAKELPARAM(FALSE, 0));

    return w;
}

extern void
swSetCheck(SWND wnd, int check)
{
    if (!wnd) return;

    SendMessage(wnd->hWnd, BM_SETCHECK, check ? BST_CHECKED : BST_UNCHECKED, 0);
}

extern int
swGetCheck(SWND wnd)
{
    if (!wnd) return 0;

    return (SendMessage(wnd->hWnd, BM_GETCHECK, 0, 0) == BST_CHECKED);
}

/***************/
/* RadioButton */
/***************/

extern SWND
swCreateRadioButton(const char *text, int x, int y, int width, int height,
                    SWND parent)
{
    SWindow *w;

    if (!parent || !parent->hWnd) return NULL;
    
    w = newSWindow();
    w->hWnd = CreateWindow("BUTTON", text, WS_CHILD | WS_VISIBLE | 
                           BS_AUTORADIOBUTTON, x, y, width, height, 
                           parent->hWnd, 0, NULL, NULL);

    SetWindowLong(w->hWnd, GWL_USERDATA, (LONG_PTR)w);
    SendMessage(w->hWnd, WM_SETFONT, (WPARAM) TheDefaultFont->hFont, MAKELPARAM(FALSE, 0));

    return w;
}

/************/
/* TextEdit */
/************/

extern SWND
swCreateTextEdit(int flags, int x, int y, int width, int height, SWND parent)
{
    SWindow *wnd = newSWindow();

    int style = WS_CHILD | WS_BORDER | WS_VISIBLE |
                ES_AUTOHSCROLL | ES_AUTOVSCROLL;
    style |= flags & SW_SINGLE_LINE ? 0 : ES_MULTILINE;
    style |= flags & SW_NON_EDITABLE ? ES_READONLY : 0;
    style |= flags & SW_PASSWORD ? ES_PASSWORD : 0;

    wnd->hWnd = CreateWindow("EDIT", "", style,
                             x, y, width, height,
                             parent->hWnd, 0, NULL, NULL);

    SetWindowLong(wnd->hWnd, GWL_USERDATA, (LONG_PTR)wnd);
    SendMessage(wnd->hWnd, WM_SETFONT, (WPARAM) TheDefaultFont->hFont,
        MAKELPARAM(FALSE, 0));

    return wnd;
}

extern void
swSetText(SWND wnd, const char *text)
{
    if (!wnd || !text) return;

    SetWindowText(wnd->hWnd, text);
}

extern void
swGetText(SWND wnd, char *text, int len)
{
    if (!wnd || !text) return;

    GetWindowText(wnd->hWnd, text, len);
}

extern void     
swSetTextCleanupPasswd(SWND wnd) 
{
    char buffer[1024];
    int len;
    int i;
    // Micro$oft MSDN GetWindowTextLength: 
    //"To obtain the exact length of the text, use the 
    // ... GetWindowText function" 8-(
    len = GetWindowText(wnd->hWnd, buffer, 1023);
    // hopefully the following is more than Micro$oft superstition
    // and really erases the internal memory of the GUI element 8-(
    for (i = len - 1; i >= 0; i--) 
        buffer[i] = (char) ' ' + rand() % (' ' - '~');
    SetWindowText(wnd->hWnd, buffer);
    for (i = len - 1; i >= 0; i--) { 
        buffer[i] = (char) 0;
        SetWindowText(wnd->hWnd, buffer);
    }
}

extern void
swTextEditSetSelection(SWND wnd, int start, int end)
{
    if (!wnd) return;

    SendMessage(wnd->hWnd, EM_SETSEL, (WPARAM) start, (LPARAM) end);
}

extern int
swGetTextCaretPos(SWND wnd)
{
    POINT pt;

    if (!wnd) return -1;

    GetCaretPos(&pt);
    return pt.x;
}

extern void
swSetTextCaretPos(SWND wnd, int textPosition)
{
    POINT pt;

    if (!wnd) return;

    GetCaretPos(&pt);
    SetCaretPos(textPosition,pt.y);    
}

/*********/
/* Label */
/*********/

extern SWND
swCreateLabel(const char *text, int x, int y, int width, int height,
               SWND parent)
{
    SWindow *l = newSWindow();
    
    l->hWnd = CreateWindow("STATIC", text, WS_CHILD | WS_VISIBLE,
                           x, y, width, height, parent->hWnd, 0, NULL, NULL);

    SetWindowLong(l->hWnd, GWL_USERDATA, (LONG_PTR)l);
    SendMessage(l->hWnd, WM_SETFONT, (WPARAM) TheDefaultFont->hFont,
                MAKELPARAM(FALSE, 0));

    return l;
}

extern void
swLabelSetText(SWND wnd, const char *text)
{
    if (!wnd || !text) return;

    SetWindowText(wnd->hWnd, text);
}

/*******************/
/* Scrolled Window */
/*******************/

extern SWND
swCreateScrolledWindow(int x, int y, int width, int height, SWND parent)
{
    SWindow *wnd;

    if (!parent || !parent->hWnd) return NULL;

    wnd = newSWindow();
    wnd->hWnd = CreateWindow("swMainWndClass", "scrolledWindow", WS_CHILD | WS_HSCROLL | WS_VSCROLL,
                             x, y, width, height,
                             parent->hWnd, NULL, NULL, NULL);
    SendMessage(wnd->hWnd, WM_SETFONT, (WPARAM) TheDefaultFont->hFont, MAKELPARAM(FALSE, 0));
    SetWindowLong(wnd->hWnd, GWL_USERDATA,(LONG_PTR) wnd);
    ShowWindow(wnd->hWnd, SW_SHOW);

    return (SWND) wnd;
}

extern void
swScrolledWindowSetChild(SWND wnd, SWND child)
{
    if (!wnd || !wnd->hWnd || !child || !child->hWnd);

    wnd->child = child;
    updateBars(wnd);
}

extern void
swGetScrollPosition(SWND wnd, int *x, int *y)
{
    if (!wnd || !wnd->hWnd) return;

    *x = GetScrollPos(wnd->hWnd, SB_HORZ);
    *y = GetScrollPos(wnd->hWnd, SB_VERT);
}

extern void
swSetScrollPosition(SWND wnd, int x, int y)
{
    SCROLLINFO infoX;
    SCROLLINFO infoY;

    if (!wnd || !wnd->hWnd) return;

    infoX.cbSize = infoY.cbSize = sizeof(SCROLLINFO);
    infoX.fMask = infoY.fMask = SIF_RANGE | SIF_POS | SIF_PAGE;
    GetScrollInfo(wnd->hWnd, SB_HORZ, &infoX);
    GetScrollInfo(wnd->hWnd, SB_VERT, &infoY);
    if (x < 0) x = 0;
    if (x + (int) infoX.nPage > infoX.nMax + 1) x = infoX.nMax - infoX.nPage + 1;
    if (y < 0) y = 0;
    if (y + (int) infoY.nPage > infoY.nMax + 1) y = infoY.nMax - infoY.nPage + 1;
    ScrollWindow(wnd->hWnd, infoX.nPos - x, infoY.nPos - y, NULL, NULL);
    SetScrollPos(wnd->hWnd, SB_HORZ, x, TRUE);
    SetScrollPos(wnd->hWnd, SB_VERT, y, TRUE);
}

extern void
swGetScrollViewportSize(SWND wnd, int *w, int *h)
{
    RECT r;

    if (!wnd || !wnd->hWnd) return;

    GetClientRect(wnd->hWnd, &r);
    *w = r.right - r.left;
    *h = r.bottom - r.top;
}

extern void
swSetScrollSizes(SWND wnd, int width, int height)
{
    RECT r;
    int w, h;

    if (!wnd || !wnd->hWnd) return;

    GetClientRect(wnd->hWnd, &r);
    w = r.right - r.left;
    h = r.bottom - r.top;

    if (width < w) width = w;
    if (height < h) height = h;

    swSetSize(wnd->child, width, height);
    updateBars(wnd);

    GetClientRect(wnd->hWnd, &r);
    w = r.right - r.left;
    h = r.bottom - r.top;

    if (width < w) width = w;
    if (height < h) height = h;

    swSetSize(wnd->child, width, height);
}

/********************/
/* Drawing Contexts */
/********************/

extern SDC
swCreateDC(SWND wnd)
{
    SDContext  *dc;
    
    if (!wnd || !wnd->hWnd) return NULL;

    dc = newSDContext();

    dc->hDC = GetDC(wnd->hWnd);
    dc->hWnd = wnd->hWnd;
    SetBkMode(dc->hDC, TRANSPARENT);  /* we want text on a transparent bg */
    SetTextAlign(dc->hDC, TA_BASELINE); /* we want text aligned on the baseline */
    SelectObject(dc->hDC, TheDefaultFont->hFont);
    return dc;
}

extern SDC
swCreateBitmapDC(SDC dc, int width, int height)
{
    SDContext  *cdc;
    
    if (!dc || !dc->hWnd) return NULL;

    cdc = newSDContext();

    cdc->hDC = CreateCompatibleDC(dc->hDC);
/*  
    M$Windows NT (+) only
    if (cdc->hDC == NULL) {
        int error = GetLastError();
    }
*/
// FIX M$Windows:
// will fail if width*height > 32M when colordepth == 32 on a M$Windows 2000  1GB RAM system
// will fail if width*height > 64M when colordepth == 16 on a M$Windows 2000  1GB RAM system
// according to MSDN documentation, will fail if width*height > 16M on M$Windows 95, 98 and ME

    cdc->hBitmap = CreateCompatibleBitmap(dc->hDC, width, height);

/*  
    M$Windows NT (+) only
    if (cdc->hBitmap == NULL) {
        int error = GetLastError();
    }
*/
    if (!(cdc->hDC && cdc->hBitmap))
        return NULL;
    cdc->hOldBitmap = SelectObject(cdc->hDC, cdc->hBitmap);

    SetBkMode(cdc->hDC, TRANSPARENT); /* we want text on a transparent bg */
    SetTextAlign(cdc->hDC, TA_BASELINE); /* we want text aligned on the baseline */
    SelectObject(cdc->hDC, TheDefaultFont->hFont);
    return cdc;
}

extern void
swDestroyDC(SDC dc)
{
    if (!dc) return;

    if (dc->hOldBitmap) {
        HBITMAP lastBitmap = SelectObject(dc->hDC, dc->hOldBitmap);
        DeleteObject(lastBitmap);
    }
    if (dc->hOldPen) {
        HPEN lastPen = SelectObject(dc->hDC, dc->hOldPen);
        DeleteObject(lastPen);
    }
    if (dc->hWnd) {  // it's a window DC
        ReleaseDC(dc->hWnd, dc->hDC);
    } else if (dc->hBitmap) {  // it's a bitmap DC
        DeleteDC(dc->hDC);
    }
    free(dc);
}

/*************/
/* GLContext */
/*************/

// initialize OpenGL only to enable to call some OpenGL functions
extern void
swOpenGLStartup(SWND wnd) 
{
    wnd->exposeCallback(wnd->data, 0, 1, 0, 1);
}

extern SGLC
swCreateGLContext(SDC dc)
{
    HGLRC hGLRC;
    LOGPALETTE* logicalPalette;
    HPALETTE paletteHandle;
    PIXELFORMATDESCRIPTOR pfd = {
        sizeof(PIXELFORMATDESCRIPTOR),  // size of this pfd
        1,                              // version number
        PFD_DRAW_TO_WINDOW |            // support window
        PFD_SUPPORT_OPENGL |            // support OpenGL
        PFD_DOUBLEBUFFER,               // double buffered
        PFD_TYPE_RGBA,                  // RGBA type
        24,                             // 24-bit color depth
        0, 0, 0, 0, 0, 0,               // color bits ignored
        0,                              // no alpha buffer
        0,                              // shift bit ignored
        0,                              // no accumulation buffer
        0, 0, 0, 0,                     // accum bits ignored
        32,                             // 32-bit z-buffer
        0,                              // no stencil buffer
        0,                              // no auxiliary buffer
        PFD_MAIN_PLANE,                 // main layer
        0,                              // reserved
        0, 0, 0                         // layer masks ignored
    };
    int currentShift = 0;
    int paletteSize = 0;
    int pixelformat;

    if (stereoType == QUAD_BUFFER_STEREO)
        pfd.dwFlags = pfd.dwFlags | PFD_STEREO;       

    if ((stereoType == RED_GREEN_ANAGLYPH_STEREO) || 
        (stereoType == GREEN_RED_ANAGLYPH_STEREO)) {
        pfd.cAccumBits = 8;
        pfd.cAccumGreenBits = 4;
        pfd.cAccumRedBits = 4;
    }

    if ((stereoType == RED_BLUE_ANAGLYPH_STEREO) || 
        (stereoType == BLUE_RED_ANAGLYPH_STEREO)) {
        pfd.cAccumBits = 8;
        pfd.cAccumBlueBits = 4;
        pfd.cAccumRedBits = 4;
    }
    if ((stereoType == RED_CYAN_ANAGLYPH_STEREO) || 
        (stereoType == CYAN_RED_ANAGLYPH_STEREO)) {
        pfd.cAccumBits = 8;
        pfd.cAccumBlueBits = 3;
        pfd.cAccumGreenBits = 2;
        pfd.cAccumRedBits = 3;
    }

    if ((want8Bit) || (GetDeviceCaps(dc->hDC, BITSPIXEL) == 8)) {
        pfd.dwFlags = pfd.dwFlags | PFD_NEED_PALETTE;       
        // set reduced depths
        pfd.cColorBits = 8;
        pfd.cRedBits = 3;
        pfd.cRedShift = currentShift;
        currentShift += pfd.cRedBits;
        pfd.cGreenBits = 3;
        pfd.cGreenShift = currentShift;
        currentShift += pfd.cGreenBits;
        pfd.cBlueBits = 2;
        pfd.cBlueShift = currentShift;
        currentShift += pfd.cBlueBits;

        // setup Palette
        paletteSize = 1 << pfd.cColorBits;

        logicalPalette = (LOGPALETTE*)
             malloc(sizeof(LOGPALETTE) + paletteSize * sizeof(PALETTEENTRY));
        logicalPalette->palVersion = 0x300;
        logicalPalette->palNumEntries = paletteSize;

        /* build RGB palette */
        {
            int i;
            int redMask = (1 << pfd.cRedBits) - 1;
            int greenMask =  (1 << pfd.cGreenBits) - 1;
            int blueMask = (1 << pfd.cBlueBits) - 1;

            for (i=0; i<paletteSize; ++i) {
                logicalPalette->palPalEntry[i].peRed =
                     (((i >> pfd.cRedShift) & redMask) * 255) / redMask;
                logicalPalette->palPalEntry[i].peGreen =
                    (((i >> pfd.cGreenShift) & greenMask) * 255) / greenMask;
                logicalPalette->palPalEntry[i].peBlue =
                    (((i >> pfd.cBlueShift) & blueMask) * 255) / blueMask;
               logicalPalette->palPalEntry[i].peFlags = 0;
            }
        }

        paletteHandle = CreatePalette(logicalPalette);
        free(logicalPalette);

        if (paletteHandle) {
            SelectPalette(dc->hDC, paletteHandle, FALSE);
            RealizePalette(dc->hDC);
        }
    }

    if ( (pixelformat = ChoosePixelFormat(dc->hDC, &pfd)) == 0 )
    {
        char message[256];
        swGetErrorStr(message, 255);
        MessageBox(dc->hWnd, message, "ChoosePixelFormat failed", SW_MB_OK);
        swDebugf("%s: %s\n", "ChoosePixelFormat failed", message);
        return NULL;
    }

    if (SetPixelFormat(dc->hDC, pixelformat, &pfd) == FALSE)
    {
        char message[256];
        swGetErrorStr(message, 255);
        MessageBox(dc->hWnd, message, "SetPixelFormat failed", SW_MB_OK);
        swDebugf("%s: %s\n", "SetPixelFormat failed", message);
        return NULL;
    }

    hGLRC = wglCreateContext(dc->hDC);

    if (hGLRC == NULL) {
        char message[256];
        swGetErrorStr(message, 255);
        MessageBox(dc->hWnd, message, "wglCreateContext failed", SW_MB_OK);
        swDebugf("%s: %s\n", "wglCreateContext failed", message);
        return NULL;
    }

    TheVisual = hGLRC;
    return (SGLC) hGLRC;
}

extern void
swDestroyGLContext(SGLC context)
{
    wglDeleteContext((HGLRC) context);
}

extern void
swMakeCurrent(SDC dc, SGLC context)
{
    HGLRC hGLRC = (HGLRC) context;

    if (dc) {
        if (wglGetCurrentContext() != hGLRC) wglMakeCurrent(dc->hDC, hGLRC);
    } else {
        wglMakeCurrent(NULL, NULL);
    }
    if ((stereoType == RED_GREEN_ANAGLYPH_STEREO) ||
        (stereoType == GREEN_RED_ANAGLYPH_STEREO)) {
        int redbits = 0;
        int greenbits = 0;
        char buf[1024];

        glGetIntegerv(GL_ACCUM_RED_BITS, &redbits);
        glGetIntegerv(GL_ACCUM_GREEN_BITS, &greenbits);

        if ((redbits == 0) || (greenbits == 0)) {
            MessageBox(NULL, 
                       "visual do not have enough Accumulation bits for anaglyph stereo", 
                       "Warning", SW_MB_ERROR);
            exit(1);
        }
    }

    if ((stereoType == RED_BLUE_ANAGLYPH_STEREO) ||
        (stereoType == BLUE_RED_ANAGLYPH_STEREO)) {
        int redbits = 0;
        int bluebits = 0;
        char buf[1024];

        glGetIntegerv(GL_ACCUM_RED_BITS, &redbits);
        glGetIntegerv(GL_ACCUM_BLUE_BITS, &bluebits);

        if ((redbits == 0) || (bluebits == 0)) {
            MessageBox(NULL, 
                       "visual do not have enough Accumulation bits for anaglyph stereo", 
                       "Warning", SW_MB_ERROR);
            exit(1);
        }
    }

    if ((stereoType == RED_CYAN_ANAGLYPH_STEREO) ||
        (stereoType == CYAN_RED_ANAGLYPH_STEREO)) {
        int redbits = 0;
        int greenbits = 0;
        int bluebits = 0;
        char buf[1024];

        glGetIntegerv(GL_ACCUM_RED_BITS, &redbits);
        glGetIntegerv(GL_ACCUM_GREEN_BITS, &greenbits);
        glGetIntegerv(GL_ACCUM_BLUE_BITS, &bluebits);

        if ((redbits == 0) || (bluebits == 0) || (greenbits == 0)) {
            MessageBox(NULL, 
                       "visual do not have enough Accumulation bits for anaglyph stereo", 
                       "Warning", SW_MB_ERROR);
            exit(1);
        }
    }

/*
    if (stereoType == QUAD_BUFFER_STEREO) {
        int quadbuffer;
        glGetIntegerv(GL_STEREO, &quadbuffer);
        if (quadbuffer == 0) {
            MessageBox(NULL, "Unable to get quadbuffer stereo visual", 
                       "Error", SW_MB_ERROR);
            exit(1);
        }
    }
*/
}

extern void
swSwapBuffers(SDC dc, SGLC context)
{
    SwapBuffers(dc->hDC);
}

extern int
swLoadGLFont(const char *name, int style, float *kernX, float *kernY)
{
    int i;
    int base;
    HFONT font;
    GLYPHMETRICSFLOAT gmf[256];
    HGDIOBJ oldFont;
    int rc;
    HDC dc = wglGetCurrentDC();
    LOGFONT lf;

    base = glGenLists(256);

    if (base == 0) return 0;

    lf.lfHeight = -10;
    lf.lfWidth = 0;
    lf.lfEscapement = 0;
    lf.lfOrientation = 0;
    lf.lfWeight = style & SW_BOLD ? 700 : 400;
    lf.lfItalic = (style & SW_ITALIC) != 0;
    lf.lfUnderline = (style & SW_UNDERLINE) != 0;
    lf.lfStrikeOut = FALSE;
    lf.lfCharSet = ANSI_CHARSET;
    lf.lfOutPrecision = OUT_DEFAULT_PRECIS;
    lf.lfClipPrecision = CLIP_DEFAULT_PRECIS;
    lf.lfQuality = DEFAULT_QUALITY;
    lf.lfPitchAndFamily = DEFAULT_PITCH | FF_DONTCARE;
    strncpy(lf.lfFaceName, name, 32);

    font = CreateFontIndirect(&lf);

    oldFont = SelectObject(dc, font);

    rc = wglUseFontOutlines(dc, 0, 256, base, 0.0f, 0.0f, WGL_FONT_POLYGONS,
                            gmf);

    SelectObject(dc, oldFont);
    DeleteObject(font);
    if (!rc) return 0;

    for (i = 0; i < 256; i++) {
        kernX[i] = gmf[i].gmfCellIncX;
        kernY[i] = gmf[i].gmfCellIncY;
    }

    return base;
}

/**********************/
/* Drawing Attributes */
/**********************/

extern void
swSetFGColor(SDC dc, int color)
{
    COLORREF c = (COLORREF) color;
    if (!dc) return;

    if (dc->fgColor != c) {
        dc->fgColor = c;
        dc->penDirty = TRUE;
        SetTextColor(dc->hDC, c);
    }
}

extern void
swSetBGColor(SDC dc, int color)
{
    COLORREF c = (COLORREF) color;
    if (!dc) return;

    SetBkColor(dc->hDC, c);
}

extern void
swSetFont(SDC dc, SFONT font)
{
    if (!dc || !font) return;

    SelectObject(dc->hDC, font->hFont);
}

extern void
swSetLineStyle(SDC dc, int style)
{
    int lineStyle;
    if (!dc) return;

    lineStyle = dc->lineStyle;

    if (style == SW_SOLID) {
        lineStyle = PS_SOLID;
    } else if (style == SW_DASH) {
        lineStyle = PS_DASH;
    } else if (style == SW_DOT) {
        lineStyle = PS_DOT;
    }

    if (lineStyle != dc->lineStyle) {
        dc->lineStyle = lineStyle;
        dc->penDirty = TRUE;
    }
}

extern void
swSetLineWidth(SDC dc, int width)
{
    if (!dc) return;

    dc->lineWidth = width;
    dc->penDirty = TRUE;
}

extern void
swSetCapStyle(SDC dc, int style)
{
    if (!dc) return;

    if (style == SW_CAP_ROUND) {
        dc->capStyle = PS_ENDCAP_ROUND;
    } else if (style == SW_CAP_SQUARE) {
        dc->capStyle = PS_ENDCAP_SQUARE;
    } else if (style == SW_CAP_FLAT) {
        dc->capStyle = PS_ENDCAP_FLAT;
    }
    dc->penDirty = TRUE;
}

extern void
swSetJoinStyle(SDC dc, int style)
{
    int joinStyle;
    if (!dc) return;

    joinStyle = dc->joinStyle;

    if (style == SW_JOIN_ROUND) {
        joinStyle = PS_JOIN_ROUND;
    } else if (style == SW_JOIN_MITER) {
        joinStyle = PS_JOIN_MITER;
    } else if (style == SW_JOIN_BEVEL) {
        joinStyle = PS_JOIN_BEVEL;
    }
    if (joinStyle != dc->joinStyle) {
        dc->joinStyle = joinStyle;
        dc->penDirty = TRUE;
    }
}

extern void
swSetClipRect(SDC dc, int x, int y, int width, int height)
{
    if (!dc) return;

    SelectClipRgn(dc->hDC, NULL);
    IntersectClipRect(dc->hDC, x, y, x + width, y + height);
}

/***********/
/* Drawing */
/***********/

extern void
swDrawLine(SDC dc, int x1, int y1, int x2, int y2)
{
    int usePath;
    if (!dc || !dc->hDC) return;

    if (dc->penDirty) updatePen(dc, dc->capStyle);
    usePath = dc->capStyle != PS_ENDCAP_ROUND;

    if (usePath) BeginPath(dc->hDC);
    MoveToEx(dc->hDC, x1, y1, NULL);
    LineTo(dc->hDC, x2, y2);
    /* swDrawLine includes last pixel */
    SetPixel(dc->hDC, x2, y2, dc->fgColor);
    if (usePath) {
        EndPath(dc->hDC);
        StrokePath(dc->hDC);
    }
}

extern void
swDrawPoint(SDC dc, int x, int y)
{
    if (!dc) return;

    SetPixel(dc->hDC, x, y, dc->fgColor);
}

extern void
swDrawRect(SDC dc, int x, int y, int width, int height)
{
    if (!dc || !dc->hDC) return;

    if (dc->penDirty) updatePen(dc, PS_ENDCAP_ROUND);

    MoveToEx(dc->hDC, x, y, NULL);
    LineTo(dc->hDC, x + width, y);
    LineTo(dc->hDC, x + width, y + height);
    LineTo(dc->hDC, x, y + height);
    LineTo(dc->hDC, x, y);
}

extern void
swDrawArc(SDC dc, int x, int y, int width, int height,
           float startAngle, float endAngle)
{
    double hw = width * 0.5, hh = height * 0.5;
    int x1, y1, x2, y2;
    if (!dc || !dc->hDC) return;

    if (dc->penDirty) updatePen(dc, PS_ENDCAP_ROUND);

    x1 = x + (int) ( (1.0 + cos(DEG2RAD(startAngle))) * hw);
    y1 = y + (int) ( (1.0 - sin(DEG2RAD(startAngle))) * hh);
    x2 = x + (int) ( (1.0 + cos(DEG2RAD(endAngle))) * hw);
    y2 = y + (int) ( (1.0 - sin(DEG2RAD(endAngle))) * hh);
    Arc(dc->hDC, x, y, x + width, y + height, x1, y1, x2, y2);
}

extern void
swFillRect(SDC dc, int x, int y, int width, int height)
{
    RECT r;
    HBRUSH brush;

    if (!dc || !dc->hDC) return;

    r.left = x;  r.right = x + width;
    r.top = y;   r.bottom = y + height;

    brush = CreateSolidBrush(dc->fgColor);
    FillRect(dc->hDC, &r, brush);
    DeleteObject(brush);
}

extern void
swFillPatternRect(SDC dc, int x, int y, int width, int height, SBITMAP bitmap)
{
    RECT r;
    HBRUSH brush;

    if (!dc || !dc->hDC) return;

    r.left = x;  r.right = x + width;
    r.top = y;   r.bottom = y + height;

    brush = CreatePatternBrush(bitmap->hBitmap);
    FillRect(dc->hDC, &r, brush);
    DeleteObject(brush);
}

extern void
swFillArc(SDC dc, int x, int y, int width, int height,
           float startAngle, float endAngle)
{
    double hw = width * 0.5, hh = height * 0.5;
    int x1, y1, x2, y2;
    if (!dc || !dc->hDC) return;

    if (dc->penDirty) updatePen(dc, PS_ENDCAP_ROUND);

    x1 = x + (int) ( (1.0 + cos(DEG2RAD(startAngle))) * hw);
    y1 = y + (int) ( (1.0 - sin(DEG2RAD(startAngle))) * hh);
    x2 = x + (int) ( (1.0 + cos(DEG2RAD(endAngle))) * hw);
    y2 = y + (int) ( (1.0 - sin(DEG2RAD(endAngle))) * hh);
    Chord(dc->hDC, x, y, x + width, y + height, x1, y1, x2, y2);
}

extern void
swFillPie(SDC dc, int x, int y, int width, int height,
           float startAngle, float endAngle)
{
    double hw = width * 0.5, hh = height * 0.5;
    int x1, y1, x2, y2;

    if (!dc || !dc->hDC) return;

    if (dc->penDirty) updatePen(dc, PS_ENDCAP_ROUND);

    x1 = x + (int) ( (1.0 + cos(DEG2RAD(startAngle))) * hw);
    y1 = y + (int) ( (1.0 - sin(DEG2RAD(startAngle))) * hh);
    x2 = x + (int) ( (1.0 + cos(DEG2RAD(endAngle))) * hw);
    y2 = y + (int) ( (1.0 - sin(DEG2RAD(endAngle))) * hh);
    Pie(dc->hDC, x, y, x + width, y + height, x1, y1, x2, y2);
}

extern void
swDrawText(SDC dc, int x, int y, const char *str)
{
    if (!dc || !str) return;

    TextOut(dc->hDC, x, y, str, strlen(str));
}

extern void
swDrawTextTruncated(SDC dc, int x, int y, int width, int height,
            const char *str)
{
    RECT r;

    if (!dc || !str) return;

    r.left = x;  r.top = y;
    r.right = x + width - 1;  r.bottom = y + height - 1;

    SetTextAlign(dc->hDC, TA_TOP);
    DrawText(dc->hDC, str, strlen(str), &r, DT_SINGLELINE | DT_TOP | DT_END_ELLIPSIS);
    SetTextAlign(dc->hDC, TA_BASELINE);
}

extern void
swDrawBitmap(SDC dc, SBITMAP bitmap, int sx, int sy, int dx, int dy,
              int width, int height)
{
    HDC bdc;
    HBITMAP oldBitmap;

    if (!dc || !bitmap) return;

    bdc = CreateCompatibleDC(dc->hDC);

    oldBitmap = SelectObject(bdc, bitmap->hBitmap);
    BitBlt(dc->hDC, dx, dy, width, height, bdc, sx, sy, SRCCOPY);

    SelectObject(bdc, oldBitmap);
    DeleteDC(bdc);
}

extern void
swCopyRect(SDC src, SDC dest, int sx, int sy, int dx, int dy,
            int width, int height)
{
    BitBlt(dest->hDC, dx, dy, width, height, src->hDC, sx, sy, SRCCOPY);
}

/********/
/* Font */
/********/

extern SFONT
swFindFont(const char *family, int style, int pointSize)
{
    LOGFONT lf;
    HFONT hFont;
    SFont *font;

    lf.lfHeight = -pointSize;
    lf.lfWidth = 0;
    lf.lfEscapement = 0;
    lf.lfOrientation = 0;
    lf.lfWeight = style & SW_BOLD ? 700 : 400;
    lf.lfItalic = (style & SW_ITALIC) != 0;
    lf.lfUnderline = (style & SW_UNDERLINE) != 0;
    lf.lfStrikeOut = FALSE;
    lf.lfCharSet = ANSI_CHARSET;
    lf.lfOutPrecision = OUT_DEFAULT_PRECIS;
    lf.lfClipPrecision = CLIP_DEFAULT_PRECIS;
    lf.lfQuality = DEFAULT_QUALITY;
    lf.lfPitchAndFamily = DEFAULT_PITCH | FF_DONTCARE;
    strncpy(lf.lfFaceName, family, 32);

    hFont = CreateFontIndirect(&lf);

    if (hFont) {
        font = newSFont();
        font->hFont = hFont;
    } else {
        font = NULL;
    } 
    return font;
}

extern void
swDeleteFont(SFONT font)
{
    if (!font) return;

    DeleteObject(font->hFont);
    free(font);
}

extern SFONT
swGetDefaultFont()
{
    return TheDefaultFont;
}

extern int
swGetFontAscent(SFONT font)
{
    TEXTMETRIC metric;
    if (!font) return 0;
    getTextMetrics(font->hFont, &metric);
    return metric.tmAscent;
}

extern int
swGetFontDescent(SFONT font)
{
    TEXTMETRIC metric;
    if (!font) return 0;
    getTextMetrics(font->hFont, &metric);
    return metric.tmDescent;
}

extern int
swGetFontHeight(SFONT font)
{
    TEXTMETRIC metric;
    if (!font) return 0;
    getTextMetrics(font->hFont, &metric);
    return metric.tmHeight;
}

extern int
swGetStringWidth(SFONT font, const char *str)
{
    HDC dc;
    SIZE size;
    HGDIOBJ oldFont;

    if (!font || !str) return 0;

    dc = GetDC(NULL);
    oldFont = SelectObject(dc, font->hFont);
    GetTextExtentPoint32(dc, str, strlen(str), &size);
    SelectObject(dc, oldFont);
    ReleaseDC(NULL, dc);
    return size.cx;
}

/********/
/* Menu */
/********/

extern SMENU
swCreateMenuBar(SWND wnd)
{
    HMENU   menu;

    if (!wnd) return NULL;

    menu = CreateMenu();
    SetMenu(wnd->hWnd, menu);
    return (SMENU) menu;
}

extern SMENU
swAppendMenu(SMENU parent, const char *text)
{
    HMENU menu;
    BOOL rc;

    if (!parent || !text) return NULL;

    menu = CreateMenu();
    rc = AppendMenu((HMENU) parent, MF_POPUP, (UINT_PTR)menu, text);
    return (SMENU) menu;
}

extern void
swAppendMenuItem(SMENU menu, const char *text, int id)
{
    if (!menu || !text) return;

    AppendMenu((HMENU) menu, MF_STRING, id, text);
}

extern void
swInsertMenuItem(SMENU menu, UINT position, const char *text, int id)
{
    if (!menu || !text) return;

    InsertMenu((HMENU) menu, position, MF_BYCOMMAND, id, text);   


}

extern void
swDeleteMenuItem(SMENU menu, int id)
{
    int error=0;
    if (DeleteMenu((HMENU) menu, id, MF_BYCOMMAND)==0)
        error=GetLastError();
}

extern void
swAppendSeparator(SMENU parent)
{
    if (!parent) return;

    AppendMenu((HMENU) parent, MF_SEPARATOR, 0, "");
}

extern SMENU
swLoadMenuBar(SWND wnd, int id)
{
    HMENU menu;

    if (!wnd) return NULL;

    menu = LoadMenu(TheInstance, MAKEINTRESOURCE(id));
    SetMenu(wnd->hWnd, menu);
    wnd->accelerators = LoadAccelerators(TheInstance, MAKEINTRESOURCE(id));
    return (SMENU) menu;
}

extern void
swMenuSetFlags(SMENU menu, int id, int mask, int flags)
{
    if (!menu) return;

    if (mask & SW_MENU_CHECKED) {
         CheckMenuItem((HMENU) menu, id, MF_BYCOMMAND | 
                       ((flags & SW_MENU_CHECKED) ? MF_CHECKED : MF_UNCHECKED));
    }
    if (mask & SW_MENU_RADIO_ITEM) {
         CheckMenuRadioItem((HMENU) menu, id, id, id, MF_BYCOMMAND | 
                            ((flags & SW_MENU_CHECKED) ? 
                            MF_CHECKED : MF_UNCHECKED));
    }
    if (mask & SW_MENU_DISABLED) {
         EnableMenuItem((HMENU) menu, id, MF_BYCOMMAND | 
                        ((flags & SW_MENU_DISABLED) ? MF_GRAYED : MF_ENABLED));
    }
}

/**************/
/* status bar */
/**************/

extern SSTATUSBAR
swCreateStatusBar(SWND wnd)
{
    SWindow *bar;

    if (!wnd) return NULL;

    bar = newSWindow();
    bar->hWnd = CreateStatusWindow(WS_VISIBLE | WS_CHILD | SBARS_SIZEGRIP,
                                   "", wnd->hWnd, 0);
    SendMessage(bar->hWnd, WM_SETFONT, (WPARAM) TheDefaultFont->hFont, 
                MAKELPARAM(FALSE, 0));
    return (SSTATUSBAR) bar;
}

extern void
swStatusBarSetText(SSTATUSBAR bar, const char *text)
{
    SendMessage(((SWindow *) bar)->hWnd, SB_SETTEXT, 0, (LPARAM) text);
}

extern SWND
swStatusBarGetWindow(SSTATUSBAR bar)
{
    return (SWND) bar;
}

extern void
swDestroyStatusBar(SSTATUSBAR bar)
{
    swDestroyWindow((SWND) bar);
}

/**********/
/* Bitmap */
/**********/

extern SBITMAP
swCreateBitmap(SWND wnd, int width, int height)
{
    SBitmap *s;

    if (!wnd) return NULL;

    s = (SBitmap *) malloc(sizeof(SBitmap));
    s->hBitmap = CreateBitmap(width, height, 8, 8, NULL);

    return s;
}

extern SBITMAP
swCreateMask(SBITMAP bitmap, int width, int height, int bgcolor)
{
    SBitmap *mask;
    HDC srcDC, destDC;
    HBITMAP oldSrcBitmap, oldDestBitmap;

    if (!bitmap) return NULL;

    mask = (SBitmap *) malloc(sizeof(SBitmap));
    mask->hBitmap = CreateBitmap(width, height, 1, 1, NULL);

    srcDC = CreateCompatibleDC(NULL);
    destDC = CreateCompatibleDC(NULL);
    SetBkColor(srcDC, (COLORREF) bgcolor);
    oldSrcBitmap = SelectObject(srcDC, bitmap->hBitmap);
    oldDestBitmap = SelectObject(destDC, mask->hBitmap);
    BitBlt(destDC, 0, 0, width, height, srcDC, 0, 0, SRCCOPY);
    SelectObject(srcDC, oldSrcBitmap);
    SelectObject(destDC, oldDestBitmap);
    DeleteDC(srcDC);
    DeleteDC(destDC);
    return mask;
}

extern SBITMAP
swLoadBitmap(SWND wnd, int id)
{
    SBitmap      *s;

    if (!wnd) return NULL;

    s = (SBitmap *) malloc(sizeof(SBitmap));
    s->hBitmap = LoadBitmap(TheInstance, MAKEINTRESOURCE(id));
    return s;
}

extern SBITMAP
swLoadMappedBitmap(SWND wnd, int id, const int *mapFrom, 
           const int *mapTo, int nMaps)
{
    SBitmap *s;
    COLORMAP *map;
    int i;

    if (!wnd) return NULL;

    map = (COLORMAP *) malloc(nMaps * sizeof(COLORMAP));

    for (i = 0; i < nMaps; i++) {
        map[i].from = mapFrom[i];
        map[i].to = mapTo[i];
    }

    s = (SBitmap *) malloc(sizeof(SBitmap));
    s->hBitmap = CreateMappedBitmap(TheInstance, id, 0, map, nMaps);
    free(map);
    return s;
}

extern void
swDestroyBitmap(SBITMAP bitmap)
{
    if (!bitmap) return;

    DeleteObject(bitmap->hBitmap);
    free(bitmap);
}

/**********/
/* cursor */
/**********/

extern SCURSOR
swLoadCursor(int id)
{
    SCursor *c = (SCursor *) malloc(sizeof(SCursor));
    switch (id) {
      case SW_CURSOR_ARROW:
        c->hCursor = LoadCursor(NULL, IDC_ARROW);
        break;
      case SW_CURSOR_DBL_ARROW_HORZ:
        c->hCursor = LoadCursor(NULL, IDC_SIZEWE);
        break;
      case SW_CURSOR_DBL_ARROW_VERT:
        c->hCursor = LoadCursor(NULL, IDC_SIZENS);
        break;
      case SW_CURSOR_CROSS:
        c->hCursor = LoadCursor(NULL, IDC_CROSS);
        break;
      case SW_CURSOR_CIRCLE:
        c->hCursor = LoadCursor(NULL, IDC_SIZEALL);
        break;
      case SW_CURSOR_ROLL:
        c->hCursor = LoadCursor(NULL, IDC_SIZEALL);
        break;
      case SW_CURSOR_NONE:
        c->hCursor = LoadCursor(NULL, IDC_ICON);
        break;
      default:
        c->hCursor = LoadCursor(TheInstance, MAKEINTRESOURCE(id));
        break;
    }
    return c;
}

extern void
swSetCursor(SWND wnd, SCURSOR cursor)
{
    if (!wnd || !cursor) return;

    wnd->cursor = cursor->hCursor;
    SetCursor(cursor->hCursor);
}

extern void
swDestroyCursor(SCURSOR cursor)
{
    if (!cursor) return;

    DestroyCursor(cursor->hCursor);
    free(cursor);
}

/************/
/* combobox */
/************/

extern SWND
swCreateComboBox(const char *name, int x, int y, int width, int height, 
         SWND parent, int id)
{
    SWindow *w;

    if (!parent) return NULL;

    w = newSWindow();
    w->hWnd = CreateWindow("COMBOBOX", name,
                           WS_CHILD | WS_VISIBLE,
                           x, y, width, height, parent->hWnd,
                           0, NULL, NULL);
    return w;
}

extern void
swComboBoxDeleteAll(SWND list)
{
    if (!list) return;

    SendMessage(list->hWnd, CB_RESETCONTENT, 0, 0);
}

extern void
swComboBoxAppendItem(SWND list, const char *str)
{
    if (!list) return;

    SendMessage(list->hWnd, CB_ADDSTRING, 0, (LPARAM) str);
}

extern void
swComboBoxSetSelection(SWND list, int selection)
{
    if (!list) return;

    SendMessage(list->hWnd, CB_SETCURSEL, (WPARAM) selection, 0);
}

extern int
swComboBoxGetSelection(SWND list)
{
    if (!list) return 0;

    return SendMessage(list->hWnd, CB_GETCURSEL, 0, 0);
}

/***********/
/* dialogs */
/***********/

extern SWND
swLoadDialog(SWND parent, int id)
{
    HRSRC res;
    HGLOBAL global;
    LPCDLGTEMPLATE dlgTemplate;
    SWindow *w;

    if (!parent) return NULL;

    w = newSWindow();

    res = FindResource(TheInstance, MAKEINTRESOURCE(id), RT_DIALOG);
    global = LoadResource(TheInstance, res);
    dlgTemplate = (LPCDLGTEMPLATE) LockResource(global);
    w->hWnd = CreateDialogIndirect(TheInstance, dlgTemplate, parent->hWnd, dialogProc);
    SetWindowLong(w->hWnd, GWL_USERDATA, (LONG_PTR)w);
    centerWindow(w->hWnd, parent->hWnd);
    UnlockResource(dlgTemplate);
    return w;
}

static int inDialog = FALSE;
static int dialogRC = 0;

extern int
swDialog(SWND dialog)
{
    MSG msg;

    if (!dialog) return 0;

    EnableWindow(GetParent(dialog->hWnd), FALSE);
    ShowWindow(dialog->hWnd, SW_SHOW);
    SetFocus(dialog->hWnd);
    inDialog = TRUE;
    dialogRC = 0;
    while (inDialog && GetMessage(&msg, (HWND) NULL, 0, 0))
    { 
        if (!IsDialogMessage(dialog->hWnd, &msg)) {
            TranslateMessage(&msg);  
            DispatchMessage(&msg);
        }
    }
    EnableWindow(GetParent(dialog->hWnd), TRUE);
    ShowWindow(dialog->hWnd, SW_HIDE);
    return dialogRC;
}

extern void
swEndDialog(int rc)
{
    inDialog = FALSE;
    dialogRC = rc;
}
extern SWND
swGetDialogItem(SWND dialog, int item)
{
    HWND hWnd;
    SWindow *w;

    if (!dialog) return NULL;

    hWnd = GetDlgItem(dialog->hWnd, item);
    if (!hWnd) return NULL;
        w = (SWindow *)GetWindowLong(hWnd, GWL_USERDATA);
        if (!w) {
            w = newSWindow();
            w->hWnd = hWnd;
            SetWindowLong(hWnd, GWL_USERDATA, (LONG_PTR)w);
    }
    return w;
}

extern int
swOpenDirDialog(SWND wnd, const char *title, const char *filters, 
                char *buf, int len)
{
    OPENFILENAME ofn;
    int i;
    BOOL rc;

    if (!wnd || !filters || !buf) return FALSE;

    memset(&ofn, 0, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = wnd->hWnd;
    ofn.lpstrFilter = filters;
    ofn.lpstrFile = buf;
    ofn.nMaxFile = len;
    ofn.lpstrFileTitle = NULL;
    ofn.nMaxFileTitle = 0;
    ofn.lpstrInitialDir = NULL;
    ofn.lpstrTitle = title;
    ofn.Flags = OFN_EXPLORER | OFN_PATHMUSTEXIST;

    rc = GetOpenFileName(&ofn);
    
    if (rc)       
        for (i=strlen(buf)-1;i>=0;i--)
            if (buf[i]=='\\') {
                buf[i]=(char) 0;
                break;
            }

    return rc;
}

extern int
swOpenMaybeFileDialog(SWND wnd, const char *title, const char *filters, 
                 char *buf, int len)
{
    OPENFILENAME ofn;
    BOOL rc;

    if (!wnd || !filters || !buf) return FALSE;

    memset(&ofn, 0, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = wnd->hWnd;
    ofn.lpstrFilter = filters;
    ofn.lpstrFile = buf;
    ofn.nMaxFile = len;
    ofn.lpstrFileTitle = NULL;
    ofn.nMaxFileTitle = 0;
    ofn.lpstrInitialDir = NULL;
    ofn.lpstrTitle = title;
    ofn.Flags = OFN_EXPLORER;

    rc = GetOpenFileName(&ofn);

    return rc;
}

extern int
swOpenFileDialog(SWND wnd, const char *title, const char *filters, 
                 char *buf, int len)
{
    OPENFILENAME ofn;
    BOOL rc;

    if (!wnd || !filters || !buf) return FALSE;

    memset(&ofn, 0, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = wnd->hWnd;
    ofn.lpstrFilter = filters;
    ofn.lpstrFile = buf;
    ofn.nMaxFile = len;
    ofn.lpstrFileTitle = NULL;
    ofn.nMaxFileTitle = 0;
    ofn.lpstrInitialDir = NULL;
    ofn.lpstrTitle = title;
    ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;

    rc = GetOpenFileName(&ofn);

    return rc;
}

extern int
swOpenExecutableDialog(SWND wnd, const char *title, const char *filters, 
                       char *buf, int len)
{
    return swOpenFileDialog(wnd, title, filters, buf, len);
}

extern int
swSaveFileDialog(SWND wnd, const char *title, const char *filters,
                 char *buf, int len, const char* default_extension)
{
    OPENFILENAME ofn;
    BOOL rc;

    if (!wnd || !filters || !buf) return FALSE;

    memset(&ofn, 0, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = wnd->hWnd;
    ofn.lpstrFilter = filters;
    ofn.lpstrFile = buf;
    ofn.nMaxFile = len;
    ofn.lpstrFileTitle = NULL;
    ofn.nMaxFileTitle = 0;
    ofn.lpstrInitialDir = NULL;
    ofn.lpstrTitle = title;
    ofn.Flags = OFN_OVERWRITEPROMPT;
    ofn.lpstrDefExt = default_extension;

    rc = GetSaveFileName(&ofn);


    if (!rc) {
        DWORD err = CommDlgExtendedError();
    }
    return rc;
}

/**************/
/* messagebox */
/**************/

extern int
swMessageBox(SWND wnd, const char *text, const char *title, int type,
              int iconType)
{
    int newType;

    if (!wnd || !text || !title) return IDCANCEL;

    switch (type) {
      case SW_MB_OKCANCEL:
        newType = MB_OKCANCEL;
        break;
      case SW_MB_OK:
        newType = MB_OK;
        break;
      case SW_MB_YESNO:
        newType = MB_YESNO;
        break;
      case SW_MB_YESNOCANCEL:
        newType = MB_YESNOCANCEL;
        break;
    }
    switch (iconType) {
      case SW_MB_NOICON:
        break;
      case SW_MB_ERROR:
        newType |= MB_ICONERROR;
        break;
      case SW_MB_INFORMATION:
        newType |= MB_ICONINFORMATION;
        break;
      case SW_MB_QUESTION:
        newType |= MB_ICONQUESTION;
        break;
      case SW_MB_WARNING:
        newType |= MB_ICONWARNING;
        break;
    }
    return MessageBox(wnd->hWnd, text, title, newType);
}

/********************/
/* debugging output */
/********************/

#ifdef HAVE_WINDOWS_XP
static HANDLE hStdErr = NULL;
#endif

static void
swInitConsole(void)
{
#ifdef HAVE_WINDOWS_XP
    BOOL ac = AllocConsole();
    if (ac)
        hStdErr = GetStdHandle(STD_ERROR_HANDLE);
#endif
}

static int isPopupConsole = 0;
static int isConsoleInitialized = 0;
int popupConsole(){
    return isPopupConsole;
}

void OpenConsole()
{
    if (!isConsoleInitialized){
        int outHandle, errHandle, inHandle;
        FILE *outFile, *errFile, *inFile;
        if (!AttachConsole(ATTACH_PARENT_PROCESS)){
            AllocConsole();
            isPopupConsole = 1;
        }

        outHandle = _open_osfhandle((intptr_t)GetStdHandle(STD_OUTPUT_HANDLE), 
                                    _O_TEXT);
        errHandle = _open_osfhandle((intptr_t)GetStdHandle(STD_ERROR_HANDLE),
                                     _O_TEXT);
        inHandle = _open_osfhandle((intptr_t)GetStdHandle(STD_INPUT_HANDLE),
                                    _O_TEXT);

        outFile = _fdopen(outHandle, "w" );
        errFile = _fdopen(errHandle, "w");
        inFile = _fdopen(inHandle, "r");

        *stdout = *outFile;
        *stderr = *errFile;
        *stdin = *inFile;

        setvbuf( stdout, NULL, _IONBF, 0 );
        setvbuf( stderr, NULL, _IONBF, 0 );
        setvbuf( stdin, NULL, _IONBF, 0 );
        setvbuf( stdin, NULL, _IONBF, 0 );
        isConsoleInitialized = 1;
    }
}

extern void
swDebugf(const char *fmt, ...)
{
    va_list ap;
    char buf[BUFSIZE];
    DWORD cWritten;

    OpenConsole();

    va_start(ap, fmt);
    _vsnprintf(buf, BUFSIZE, fmt, ap);
    OutputDebugString(buf);

    fprintf(stderr,"%s",buf);
}

/*****************/
/* miscellaneous */
/*****************/

extern double
swGetCurrentTime(void)
{
    struct _timeb t;

    _ftime(&t);

    return (double) t.time + t.millitm / 1000.0;
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
    GetTempPath(len, buf);
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
    return '\\';
}

/* return 0 on successfull write of remaining buffers */
extern int 
swTruncateClose(int f)
{
    if (f > -1)
        if (_chsize(f, tell(f)))
            return -1;    
    return myclose(f);
}

extern const char*
swGetLinefeedString(void)
{
    return "\n";
}

extern int 
swIsXML(const char *filename)
{
    int i;

    if (strlen(filename)==0)
        return(0);
    for (i=strlen(filename)-1;(i>0)&&(filename[i]!='.');i--);
    if ((strcmp(&filename[i],".x3d")==0) || (strcmp(&filename[i],".x3d\"")==0))
        return 1;
    else
        return 0;       
}

extern char*            
swKillFileQuotes(char* filename)
{
    if ((filename[0]=='"') && (filename[strlen(filename)-1]=='"')) {
        filename[strlen(filename)-1]=0;
        return filename+1;
    } else
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
    return " > NULL:";
}

extern int
swMkdirCommand(SWND wnd,const char* dir)
{
    char* string;
//    char* mkdir="COMMAND.COM /C MKDIR ";
    char* mkdir="MKDIR ";
    const char* tonull=swGetToNullDevice();
    int ret;

    string=(char*)malloc(strlen(mkdir)+strlen(dir)+1+
                         strlen(tonull)+1);
    if (string==NULL) return 1;
    strcpy(string,mkdir);
    strcat(string,dir);
    strcat(string," ");
    strcat(string,tonull);
/*
    EnableWindow(wnd->hWnd,FALSE);
    EnableWindow(GETSAFEHWND(wnd->hwnd),FALSE);
    ret=WinExec(string,SW_HIDE);
    EnableWindow(GETSAFEHWND(wnd->hwnd),TRUE);
    EnableWindow(wnd->hWnd,TRUE);
*/
    system(string);
    ret=0;
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
        return -1;
    }
    if (fseek(inputFile, 0, SEEK_END) == -1) 
        ret = -1;
    else {
        long fileLen = 0;
        char *content;

        fileLen = ftell(inputFile);
        rewind(inputFile);
        content = (char *) malloc(fileLen);
        if (content == NULL)
            ret = -1;
        else {
            if (fread(content, 1, fileLen, inputFile) != fileLen)
                ret = -1;
            else {
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
                   if (outputFile == NULL)
                       ret = -1;
                   else {
                       if (fwrite(content, 1, fileLen, outputFile) != fileLen)
                           ret = -1;    
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
swShellCommand(SWND wnd,const char* file, const char* command, 
               const char* directory)
{
/*
    PROCESS_INFORMATION procinfo;
    STARTUPINFO startinfo;
    DWORD lpExitCode;

    EnableWindow(wnd->hWnd,FALSE);
    EnableWindow(GETSAFEHWND(wnd->hwnd),FALSE);
    memset(&startinfo, 0, sizeof(startinfo));
    startinfo.cb = sizeof(STARTUPINFO);
    startinfo.dwFlags = STARTF_USESHOWWINDOW;
    startinfo.lpTitle=NULL;
    startinfo.wShowWindow=SW_HIDE;
    CreateProcess(file,command,NULL,NULL,FALSE,CREATE_NO_WINDOW,NULL,
                  directory,&startinfo,&procinfo);
    do {
        Sleep(100);
        GetExitCodeProcess(procinfo.hProcess,&lpExitCode);
       } while (lpExitCode==STILL_ACTIVE);
    CloseHandle(procinfo.hProcess);
    CloseHandle(procinfo.hThread);
    EnableWindow(GETSAFEHWND(wnd->hwnd),TRUE);
    EnableWindow(wnd->hWnd,TRUE);
    return(lpExitCode);
*/
    return(system(command));
}

static swLanguage = 0;

extern void 
swSetLang(int lang)
{
    swLanguage = lang;
}

extern int
swGetLang(void)
{
    return swLanguage;
}

static void
swLoadStringFromResourceDotC(int id, char *buf, int len)
{
    int i;

    if (len < 1)
        return;
    buf[0] = 0;
    for (i = 0; i < swNumStrings; i++) {
        if (swStrings[i].id == (id + swLanguage)) {
            strncpy(buf, swStrings[i].str, len);
            break;
        }
    }
}

extern void
swLoadString(int id, char *buf, int len)
{
//    if (!buf) return;

    LoadString(TheInstance, id + swLanguage, buf, len);
 
    if (buf[0] == 0)
        swLoadStringFromResourceDotC(id, buf, len);                
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
//    wnd->dropCallback = NULL;
}


/**************************/
/* persistent preferences */
/**************************/

static int defaultPreferences = 0;

extern void swPreferencesDefault(void)
{
    defaultPreferences = 1;
}

static FILE_STABLE fileSTable;

#define MAX_PREFLINE 1024

static void
appendPair(FILE_STABLE table, const char *key, const char *value)
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

static FILE_STABLE
swLoadFilePreferences(const char *rcFileEnvironmentName, const char *appName)
{
    char line[MAX_PREFLINE], *c, *key, *value;
    const char *home = getenv("HOME");
    FILE *f;
    FILE_STABLE table;
    int len;

    /* company is (intentionally) ignored */

    if (!appName) return NULL;
    

    table = (FILE_STABLE) malloc(sizeof(FileSTable));
    table->size = 0;
    table->capacity = 8;
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

static void
swSaveFilePreferences(FILE_STABLE table)
{
    int f;
    int i;
    char filenameNew[1024];
    int flag = 0;

    f = open(table->filename, O_WRONLY | O_CREAT | O_TRUNC, 00666);
    if (f == -1) return; // silently ignore intentionally

    for (i = 0; i < table->size; i++) {
        mywritestr(f,table->data[i].key);
        mywritestr(f," ");
        mywritestr(f,table->data[i].value);
        mywritestr(f,"\n");
    }
    swTruncateClose(f);
}

static void
swDeleteFilePreferences(FILE_STABLE table)
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

static const char *
swGetFilePreference(FILE_STABLE table, const char *key, const char *defaultValue)
{
    int i;

    for (i = 0; i < table->size; i++) {
        if (!strcmp(table->data[i].key, key)) {
            return table->data[i].value;
        }
    }
    return defaultValue;
}

static void
swSetFilePreference(FILE_STABLE table, const char *key, const char *value)
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

static int
swGetIntFilePreference(FILE_STABLE table, const char *key, int defaultValue)
{
    int i;

    for (i = 0; i < table->size; i++) {
        if (!strcmp(table->data[i].key, key)) {
            return atoi(table->data[i].value);
        }
    }
    return defaultValue;
}

static void
swSetIntFilePreference(STABLE table, const char *key, int value)
{
    char str[128];
    mysnprintf(str, 128, "%d", value);
    swSetPreference(table, key, str);
}

extern STABLE
swLoadPreferences(const char *companyName, const char *appName)
{
    HKEY software, company, app;
    STABLE rc = NULL;
    DWORD dummy;

    char *rcFileEnvironmentName;
    int i;

    fileSTable = NULL;
    /* company is (intentionally) ignored */

    if (!appName) return NULL;
    
    rcFileEnvironmentName = (char *) malloc(strlen(appName) + strlen("RC") + 1);
    strcpy(rcFileEnvironmentName, appName);
    for (i = 0; i < strlen(rcFileEnvironmentName); i++)
        rcFileEnvironmentName[i] = toupper(rcFileEnvironmentName[i]);
    strcat(rcFileEnvironmentName, "RC");
    if (getenv(rcFileEnvironmentName) != NULL) {    
        fileSTable = swLoadFilePreferences(rcFileEnvironmentName, appName);
        return (STABLE)fileSTable;
    }

    if (RegOpenKeyEx(HKEY_CURRENT_USER, "software", 0, 
                     KEY_READ | KEY_WRITE, &software) == ERROR_SUCCESS) {
        if (RegCreateKeyEx(software, companyName, 0, REG_NONE,
                           REG_OPTION_NON_VOLATILE, KEY_READ | KEY_WRITE,
                           NULL, &company, &dummy) == ERROR_SUCCESS) {
            if (RegCreateKeyEx(company, appName, 0, REG_NONE,
                               REG_OPTION_NON_VOLATILE, KEY_READ | KEY_WRITE,
                               NULL, &app, &dummy) == ERROR_SUCCESS) {
                rc = (STABLE) app;
            }
            RegCloseKey(company);
        }
        RegCloseKey(software);
    }
    return rc;
}

extern void
swSavePreferences(STABLE table)
{
    if (fileSTable != NULL)
        swSaveFilePreferences(fileSTable);
    // if no file is used, this is a no-op (the keys have already been saved)
}

extern void
swDeletePreferences(STABLE pref)
{
    if (fileSTable != NULL) {
        swDeleteFilePreferences(fileSTable);
        return;
    }

    if (!pref) return;

    RegCloseKey((HKEY) pref);
}

extern const char *
swGetPreference(STABLE table, const char *title, const char *defaultValue)
{
    static char buffer[BUFSIZE];
    DWORD type, size = BUFSIZE;

    if (defaultPreferences)
        return defaultValue;
  
    if (fileSTable != NULL) {
        return swGetFilePreference(fileSTable, title, defaultValue);
    }

    if (!table || !title || !defaultValue) return "";

    if (RegQueryValueEx((HKEY) table, title, NULL, &type, (LPBYTE) buffer, 
                        &size) == ERROR_SUCCESS && type == REG_SZ) {
        return buffer;
    } else {
        return defaultValue;
    }
}

extern void
swSetPreference(STABLE table, const char *title, const char *value)
{
    if (fileSTable != NULL) {
        swSetFilePreference(fileSTable, title, value);
        return;
    }


    if (!table || !title || !value) return;

    RegSetValueEx((HKEY) table, title, 0, REG_SZ, (LPBYTE) value, 
                  strlen(value) + 1);
}

extern int
swGetIntPreference(STABLE table, const char *title, int defaultValue)
{
    int value;
    DWORD type, size = sizeof(int), rc;

    if (defaultPreferences)
        return defaultValue;  

    if (fileSTable != NULL) {
        return swGetIntFilePreference(fileSTable, title, defaultValue);
    }

    if (!table || !title) return defaultValue;

    if ((rc = RegQueryValueEx((HKEY) table, title, NULL, &type, 
                              (LPBYTE) &value, &size)) == ERROR_SUCCESS 
        && type == REG_DWORD) {
        return value;
    } else {
        return defaultValue;
    }
}

extern void
swSetIntPreference(STABLE table, const char *title, int value)
{
    if (fileSTable != NULL) {
        swSetIntFilePreference((STABLE)fileSTable, title, value);
        return;
    }

    if (!table || !title) return;

    RegSetValueEx((HKEY) table, title, 0, REG_DWORD, (LPBYTE) &value, 
                  sizeof(int));
}

extern void 
swUninstall(STABLE table, const char *companyName, const char *appName)
{
    HKEY software, company, app;
    TCHAR subkey[256];

    if (RegOpenKeyEx(HKEY_CURRENT_USER, "software", 0, 
                     KEY_READ | KEY_SET_VALUE, &software) == ERROR_SUCCESS) {
        if (RegOpenKeyEx(software, companyName, 0, KEY_READ | KEY_SET_VALUE,
                         &company) == ERROR_SUCCESS) {
            DWORD next = RegOpenKeyEx(company, appName, 0, 
                                      KEY_ENUMERATE_SUB_KEYS | KEY_SET_VALUE,
                                      &app);
            while (next == ERROR_SUCCESS) {
                DWORD subkeyLen = 256;
                next = RegEnumKeyEx(app, 0, subkey, &subkeyLen, NULL, NULL,
                                    NULL, NULL);
                if (next == ERROR_SUCCESS)
                    RegDeleteKey(app, subkey);
            }
            RegCloseKey(app);
            RegDeleteKey(company, appName);
            RegCloseKey(company);
        }
        RegDeleteKey(software, companyName);
        RegCloseKey(software);
    }
}

/*****************/
/* mouse capture */
/*****************/

extern void
swSetCapture(SWND wnd)
{
    if (!wnd) return;

    SetCapture(wnd->hWnd);
}

extern void
swReleaseCapture(SWND wnd)
{
    if (!wnd) return;

    ReleaseCapture();
}

/********/
/* Tree */
/********/

extern STREE
swCreateTree(int x, int y, int width, int height, SWND parent)
{
    STree   *tree;
    
    if (!parent) return NULL;

    tree = newSTree();
    
    tree->hWnd = CreateWindow(WC_TREEVIEW, NULL,
                              WS_CHILD | WS_VISIBLE | TVS_HASLINES | 
                              TVS_LINESATROOT | TVS_HASBUTTONS | 
                              TVS_SHOWSELALWAYS,
                              x, y, width, height, parent->hWnd, 
                              0, TheInstance, NULL);

    tree->wnd = newSWindow();
    tree->wnd->hWnd = tree->hWnd;

    SetWindowLong(tree->hWnd, GWL_USERDATA, (LONG_PTR)tree);
    SendMessage(tree->hWnd, WM_SETFONT, (WPARAM) TheDefaultFont->hFont, MAKELPARAM(FALSE, 0));

    return tree;
}

extern void
swDestroyTree(STREE tree)
{
    if (!tree) return;

    DestroyWindow(tree->hWnd);
    ImageList_Destroy(tree->hImageList);
    free(tree->wnd);
    free(tree);
}

extern STREEITEM
swTreeHitTest(STREE tree, int x, int y)
{
    TV_HITTESTINFO info;
    info.pt.x = x;
    info.pt.y = y;

    TreeView_HitTest(tree->hWnd, &info);
    return (STREEITEM) info.hItem;
}

extern STREEITEM
swTreeGetRootItem(STREE tree)
{
    if (!tree) return NULL;

    return (STREEITEM) SendMessage(tree->hWnd, TVM_GETNEXTITEM, TVGN_ROOT, 0);
}

extern SWND
swTreeGetWindow(STREE tree)
{
    if (!tree) return NULL;

    return tree->wnd;
}

extern STREEITEM
swTreeInsertItem(STREE tree, int position, STREEITEM relative, const char *name)
{
    TV_INSERTSTRUCT tvis;

    if (!tree) return NULL;

    if (position == SW_INSERT_ROOT) {
        tvis.hParent = TVI_ROOT;
        tvis.hInsertAfter = NULL;
    } else if (position == SW_INSERT_FIRST_CHILD) {
        tvis.hInsertAfter = TVI_FIRST;
        tvis.hParent = (HTREEITEM) relative;
    } else if (position == SW_INSERT_LAST_CHILD) {
        tvis.hInsertAfter = TVI_LAST;
        tvis.hParent = (HTREEITEM) relative;
    } else if (position == SW_INSERT_AFTER) {
        tvis.hInsertAfter = (HTREEITEM) relative;
        tvis.hParent = (HTREEITEM) SendMessage(tree->hWnd, TVM_GETNEXTITEM, TVGN_PARENT, (LPARAM) relative);
    } else if (position == SW_INSERT_BEFORE) {
        tvis.hParent = (HTREEITEM) SendMessage(tree->hWnd, TVM_GETNEXTITEM, TVGN_PARENT, (LPARAM) relative);
        relative = (STREEITEM) SendMessage(tree->hWnd, TVM_GETNEXTITEM, TVGN_PREVIOUS, (LPARAM) relative);
        if (relative == NULL) relative = (STREEITEM) TVI_LAST;
        tvis.hInsertAfter = (HTREEITEM) relative;
    }
    tvis.item.mask = TVIF_TEXT;
    tvis.item.pszText = (LPTSTR) name;
    tvis.item.iImage = 0;
    tvis.item.iSelectedImage = 0;
    tvis.item.state = 0;
    tvis.item.stateMask = 0;
    tvis.item.lParam = 0;
    return (STREEITEM) SendMessage(tree->hWnd, TVM_INSERTITEM, 0, (LPARAM)&tvis);
}

extern void *
swTreeGetItemData(STREE tree, STREEITEM item)
{
    TV_ITEM info;
    info.hItem = (HTREEITEM) item;
    info.mask = TVIF_PARAM;

    TreeView_GetItem(tree->hWnd, &info);
    return (void *) info.lParam;
}

extern void
swTreeSetItemData(STREE tree, STREEITEM item, void *data)
{
    TV_ITEM info;
    info.hItem = (HTREEITEM) item;
    info.mask = TVIF_HANDLE | TVIF_PARAM;
    info.lParam = (LPARAM) data;

    TreeView_SetItem(tree->hWnd, &info);
}

extern int
swTreeGetItemTitle(STREE tree, STREEITEM item, char *buf, int len)
{
    TV_ITEM info;
    info.hItem = (HTREEITEM) item;
    info.pszText = buf;
    info.cchTextMax = len;
    info.mask = TVIF_TEXT;

    TreeView_GetItem(tree->hWnd, &info);
    return info.cchTextMax;
}

extern void
swTreeSetItemImage(STREE tree, STREEITEM item, int image1, int image2)
{
    TV_ITEM info;
    info.hItem = (HTREEITEM) item;
    info.mask = TVIF_HANDLE | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
    info.iImage = image1;
    info.iSelectedImage = image2;

    TreeView_SetItem(tree->hWnd, &info);
}

extern STREEITEM
swTreeGetFirstChild(STREE tree, STREEITEM item)
{
    if (!tree) return NULL;

    return (STREEITEM) TreeView_GetChild(tree->hWnd, item);
}

extern STREEITEM
swTreeGetNextItem(STREE tree, STREEITEM item)
{
    if (!tree) return NULL;

    return (STREEITEM) TreeView_GetNextSibling(tree->hWnd, item);
}

extern STREEITEM
swTreeGetPrevItem(STREE tree, STREEITEM item)
{
    if (!tree) return NULL;

    return (STREEITEM) TreeView_GetPrevSibling(tree->hWnd, item);
}

extern STREEITEM
swTreeGetParentItem(STREE tree, STREEITEM item)
{
    if (!tree) return NULL;

    return (STREEITEM) TreeView_GetParent(tree->hWnd, item);
}

extern void
swTreeDeleteItem(STREE tree, STREEITEM item)
{
    if (!tree) return;

    TreeView_DeleteItem(tree->hWnd, item);
}

extern void
swTreeDeleteAllItems(STREE tree)
{
    if (!tree) return;

    TreeView_DeleteAllItems(tree->hWnd);
}

extern void
swTreeSelectItem(STREE tree, STREEITEM item)
{
}

extern void
swTreeDeselectItem(STREE tree, STREEITEM item)
{
}

extern void
swTreeDeselectAll(STREE tree)
{
}

extern STREEITEM
swTreeGetCurrentItem(STREE tree)
{
    if (!tree) return NULL;

    return (STREEITEM) TreeView_GetSelection(tree->hWnd);
}

extern void
swTreeSetCurrentItem(STREE tree, STREEITEM item)
{
    if (!tree) return;

    TreeView_SelectItem(tree->hWnd, item);
}

extern void
swTreeSelectDropTarget(STREE tree, STREEITEM item)
{
    if (!tree) return;

    TreeView_SelectDropTarget(tree->hWnd, item);
}

extern void
swTreeSetItemName(STREE tree, STREEITEM item, const char* name)
{
    TV_ITEM info;
    info.hItem = (HTREEITEM) item;
    info.pszText = (LPTSTR) name;
    info.cchTextMax = strlen(name);
    info.mask = TVIF_TEXT;

    TreeView_SetItem(tree->hWnd, &info);
}

extern char *
swTreeGetItemName(STREE tree, STREEITEM item)
{
    static const size_t maxLen = 128;
    LPSTR buffer[/*maxLen*/ 128 + 1];

    if (item == NULL)
        return NULL;

    TV_ITEM info;
    info.hItem = (HTREEITEM)item;
    info.mask = TVIF_TEXT;
    info.cchTextMax = maxLen;
    info.pszText = &buffer[0];

    TreeView_GetItem(tree->hWnd, &info);

    return info.pszText;
}



extern void
swTreeSetImageList(STREE tree, SBITMAP bitmap, SBITMAP mask, int width, int height, int count)
{
    if (tree->hImageList) ImageList_Destroy(tree->hImageList);
    tree->hImageList = ImageList_Create(width, height, ILC_MASK, count, 8);
    ImageList_Add(tree->hImageList, bitmap->hBitmap, mask->hBitmap);
    TreeView_SetImageList(tree->hWnd, tree->hImageList, TVSIL_NORMAL);
}

extern void
swTreeSetOverlayImage(STREE tree, int index)
{
    ImageList_SetOverlayImage(tree->hImageList, index, 1);
}

extern void
swTreeSetFlags(STREE tree, STREEITEM item, int mask, int flags)
{
    if (mask & SW_TREE_ITEM_COLLAPSED) {
        int flag = flags & SW_TREE_ITEM_COLLAPSED ? TVE_COLLAPSE : TVE_EXPAND;
        TreeView_Expand(tree->hWnd, item, flag);
    }
    if (mask & SW_TREE_ITEM_OVERLAY) {
         TV_ITEM tvitem;
         BOOL rc;
         tvitem.mask = TVIF_STATE;
         tvitem.hItem = (HTREEITEM) item;
         tvitem.state = flags & SW_TREE_ITEM_OVERLAY ? INDEXTOOVERLAYMASK(1) : 0;
         tvitem.stateMask = TVIS_OVERLAYMASK;
         rc = TreeView_SetItem(tree->hWnd, &tvitem);
    }
}

extern void
swTreeSetItemCollapsed(STREE tree, STREEITEM item, int collapsed)
{
    TreeView_Expand(tree->hWnd, item, collapsed ? TVE_COLLAPSE : TVE_EXPAND);
}

extern void
swTreeSetCallback(STREE tree, TreeCallback cb)
{
    tree->treeCallback = cb;
}

extern void
swTreeSetClientData(STREE tree, void *data)
{
    tree->data = data;
    tree->wnd->data = data;
}

/******************/
/* header control */
/******************/

extern SHEADER
swCreateHeader(int x, int y, int width, int height, SWND parent)
{
    SHeader   *header;
    
    if (!parent) return NULL;

    header = newSHeader();
    
    header->hWnd = CreateWindow(WC_HEADER, NULL,
                                WS_CHILD | WS_VISIBLE | HDS_HORZ,
                                x, y, width, height, parent->hWnd, 
                                0, TheInstance, NULL);

    SetWindowLong(header->hWnd, GWL_USERDATA, (LONG_PTR)header);
    SendMessage(header->hWnd, WM_SETFONT, (WPARAM) TheDefaultFont->hFont, MAKELPARAM(FALSE, 0));

    return header;
}

extern void
swDestroyHeader(SHEADER header)
{
    if (!header) return;

    DestroyWindow(header->hWnd);
    free(header);
}

extern void
swHeaderInsertItem(SHEADER header, int pos, const char *name, int width)
{
    HD_ITEM item;

    if (!header || !name) return;

    item.mask = HDI_TEXT | HDI_WIDTH | HDI_FORMAT;
    item.pszText = (char *) name;
    item.cchTextMax = strlen(name);
    item.cxy = width;
    item.fmt = HDF_LEFT;
    Header_InsertItem(header->hWnd, pos, &item);
}

extern void
swHeaderGetSize(SHEADER header, int *width, int *height)
{
    RECT r;
    if (!header || !header->hWnd) return;

    GetClientRect(header->hWnd, &r);
    *width = r.right - r.left + 1;
    *height = r.bottom - r.top + 1;
}

extern void
swHeaderSetSize(SHEADER header, int width, int height)
{
    RECT r;
    if (!header || !header->hWnd) return;

    GetClientRect(header->hWnd, &r);
    MoveWindow(header->hWnd, r.left, r.top, width, height, FALSE);
}

extern int
swHeaderGetItemWidth(SHEADER header, int pos)
{
    HD_ITEM item;

    if (!header) return 0;

    item.mask = HDI_WIDTH;
    Header_GetItem(header->hWnd, pos, &item);
    return item.cxy;
}

extern void
swHeaderSetItemWidth(SHEADER header, int pos, int width)
{
    HD_ITEM item;

    if (!header) return;

    item.mask = HDI_WIDTH;
    item.cxy = width;
    Header_SetItem(header->hWnd, pos, &item);
}

extern void
swHeaderSetCallback(SHEADER header, HeaderCallback cb)
{
    if (!header) return;

    header->headerCallback = cb;
}

extern void
swHeaderSetClientData(SHEADER header, void *data)
{
    if (!header) return;

    header->data = data;
}

/***********/
/* toolbar */
/***********/

extern STOOLBAR
swCreateToolbar(SWND parent, SBITMAP bitmap, int width, int height, int count,
        const int *buttonInfo)
{
    SWindow *wnd;
    HWND tt;
    int i;
    TBBUTTON *buttons;

    if (!parent) return NULL;

    buttons = (TBBUTTON *) malloc(count * sizeof(TBBUTTON));

    for (i = 0; i < count; i++) {
         buttons[i].iBitmap = buttonInfo[i*2];
         if (buttonInfo[i*2+1] == 0)
             buttons[i].idCommand = -1;
         else
             buttons[i].idCommand = buttonInfo[i*2+1];
         buttons[i].fsState = TBSTATE_ENABLED;
         buttons[i].fsStyle = buttonInfo[i*2] == -1 ? TBSTYLE_SEP : 0;
         buttons[i].dwData = 0;
         buttons[i].iString = 0;
    }

    wnd = newSWindow();
    wnd->hWnd = CreateToolbarEx(parent->hWnd, WS_CHILD | WS_VISIBLE
                                | CCS_NOMOVEY | TBSTYLE_TOOLTIPS
                                | TBSTYLE_FLAT, 0, 
                                count, NULL, (UINT_PTR)bitmap->hBitmap, 
                                buttons, count,
                                width, height, width, height, 
                                sizeof(TBBUTTON));

    SetWindowLong(wnd->hWnd, GWL_USERDATA, (LONG_PTR)parent);
    tt = (HWND) SendMessage(wnd->hWnd, TB_GETTOOLTIPS, 0, 0);
    SetWindowLong(tt, GWL_USERDATA, (LONG_PTR)wnd);
    free(buttons);

    return (STOOLBAR) wnd;
}

extern void
swDestroyToolbar(STOOLBAR tb)
{
    swDestroyWindow((SWND) tb);
}

extern SWND
swToolbarGetWindow(STOOLBAR tb)
{
    return (SWND) tb;
}

extern void
swToolbarSetButtonFlags(STOOLBAR tb, int index, int mask, int flags)
{
    SWND wnd = (SWND) tb;
    TBBUTTON info;

    if (!tb || index < 0) return;

    SendMessage(wnd->hWnd, TB_GETBUTTON, (WPARAM) index, (LPARAM) &info);

    if (mask & SW_TB_DISABLED) {
         info.fsState &= ~TBSTATE_ENABLED;
         info.fsState |= (flags & SW_TB_DISABLED) ? 0 : TBSTATE_ENABLED;
    }
    if (mask & SW_TB_CHECKED) {
         info.fsState &= ~TBSTATE_CHECKED;
         info.fsState |= (flags & SW_TB_CHECKED) ? TBSTATE_CHECKED : 0;
    }

    SendMessage(wnd->hWnd, TB_SETSTATE, (WPARAM) info.idCommand, 
        (LPARAM) MAKELONG(info.fsState, 0));
}

extern void
swToolbarSetTooltipCallback(STOOLBAR tb, TooltipCallback cb)
{
    if (!tb) return;

    ((SWindow *) tb)->tooltipCallback = cb;
}

extern void
swToolbarSetClientData(STOOLBAR tb, void *data)
{
    if (!tb) return;

    ((SWindow *) tb)->data = data;
}

/*******************/
/* static funtions */
/*******************/

void swUpdate(void)
{
}

static SWindow *
newSWindow(void)
{
    SWindow *wnd = malloc(sizeof(SWindow));
    wnd->hWnd = NULL;
    wnd->data = NULL;
    wnd->accelerators = NULL;
    wnd->cursor = NULL;
    wnd->exposeCallback = NULL;
    wnd->mouseCallback = NULL;
    wnd->keyCallback = NULL;
    wnd->focusCallback = NULL;
    wnd->resizeCallback = NULL;
    wnd->enterCallback = NULL;
    wnd->activateCallback = NULL;
    wnd->scrollBarCallback = NULL;
    wnd->timerCallback = NULL;
    wnd->commandCallback = NULL;
    wnd->highlightCallback = NULL;
    wnd->closeCallback = NULL;
    wnd->tooltipCallback = NULL;
    wnd->dropTarget = NULL;
    wnd->child = NULL;
    wnd->mouseInWindow = FALSE;
    wnd->bgColor = GetSysColor(COLOR_WINDOW);
    wnd->bgBrush = GetSysColorBrush(COLOR_WINDOW);
    wnd->isHidden = FALSE;

    if (!registered) {
         registerMainWindowClass();
         registered = TRUE;
    }
    return wnd;
}

static STree *
newSTree(void)
{
    STree *wnd = malloc(sizeof(STree));
    wnd->hWnd = NULL;
    wnd->hImageList = NULL;
    wnd->data = NULL;
    wnd->treeCallback = NULL;

    return wnd;
}

static SHeader *
newSHeader(void)
{
    SHeader *header = malloc(sizeof(SHeader));
    header->hWnd = NULL;
    return header;
}

static SDContext *
newSDContext(void)
{
    SDContext *dc = malloc(sizeof(SDContext));
    dc->hDC = NULL;
    dc->hWnd = NULL;
    dc->hBitmap = NULL;
    dc->fgColor = RGB(0, 0, 0);
    dc->lineStyle = PS_SOLID;
    dc->capStyle = PS_ENDCAP_ROUND;
    dc->joinStyle = PS_JOIN_ROUND;
    dc->lineWidth = 1;
    dc->hOldPen = NULL;
    dc->hOldBitmap = NULL;
    dc->penDirty = TRUE;

    return dc;
}

static SFont *
newSFont(void)
{
    SFont *font = malloc(sizeof(SFont));
    font->hFont = NULL;

    return font;
}

static STimer *
newSTimer(HWND hWnd, TimerCallback callback, void *data)
{
    STimer *timer = malloc(sizeof(STimer));
    timer->hWnd = hWnd;
    timer->callback = callback;
    timer->data = data;

    return timer;
}

static void
registerMainWindowClass(void)
{
    WNDCLASS wc;
    wc.lpszClassName = "swMainWndClass";
    wc.lpfnWndProc = mainWndProc; 
    wc.style = CS_OWNDC | CS_VREDRAW | CS_HREDRAW;
    wc.hInstance = TheInstance;
    if (defaultIcon != -1) {
         wc.hIcon = LoadIcon(TheInstance, MAKEINTRESOURCE(defaultIcon));
    } else {
         wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    }
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = GetSysColorBrush(COLOR_WINDOW);
    wc.lpszMenuName =  "swMainMenu";
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0; 

    RegisterClass(&wc);
}

static void
adjustSize(int *width, int *height, DWORD style, BOOL menu)
{
    RECT r;

    r.left = r.top = 0;
    r.right = *width - 1;
    r.bottom = *height - 1;
    AdjustWindowRect(&r, style, menu);
    *width = r.right - r.left + 1;
    *height = r.bottom - r.top + 1;
}

static 
doMButtonCallback(int key, int value, HWND hWnd, UINT message, WPARAM wParam, 
          LPARAM lParam)
{
    SWindow *w = (SWindow *) GetWindowLong(hWnd, GWL_USERDATA);
    int x = LOWORD(lParam);
    int y = HIWORD(lParam);
    LONG rc;

    if (w && w->keyCallback) {
         w->keyCallback(w->data, key, value, x, y, MODIFIERS(wParam));
         rc = 0L;
    } else {
         rc = DefWindowProc( hWnd, message, wParam, lParam);
    }
    return rc;
}

static LONG
doKeyCallback(int value, HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    SWindow *w = (SWindow *) GetWindowLong(hWnd, GWL_USERDATA);
    LONG     rc;

    if (w && w->keyCallback) {
         w->keyCallback(w->data, keyMap[wParam], value, 0, 0, SW_CONTROL);
         rc = 0L;
    } else {
         rc = DefWindowProc( hWnd, message, wParam, lParam);
    }
    return rc;
}

static void
trackMouseLeave(HWND wnd)
{
    TRACKMOUSEEVENT tme;

    tme.cbSize = sizeof(TRACKMOUSEEVENT);
    tme.dwFlags = TME_LEAVE;
    tme.hwndTrack = wnd;
    tme.dwHoverTime = HOVER_DEFAULT;

    _TrackMouseEvent(&tme);
}

static LRESULT CALLBACK        
mainWndProc(HWND hWnd, UINT message, WPARAM  wParam, LPARAM lParam)
{
    LONG rc = 0L;

    switch(message) {
      case WM_MOUSEMOVE:
         {
             SWindow *w = (SWindow *) GetWindowLong(hWnd, GWL_USERDATA);
             short xPos = LOWORD(lParam);  // horizontal position of cursor 
             short yPos = HIWORD(lParam);  // vertical position of cursor 

             if (!w->mouseInWindow) {
                 trackMouseLeave(w->hWnd);
                 w->mouseInWindow = TRUE;
                 if (w->enterCallback) {
                     w->enterCallback(w->data, TRUE);
                 }
             }
             if (w->cursor) SetCursor(w->cursor);
             if (w && w->mouseCallback) {
                 w->mouseCallback(w->data, xPos, yPos, MODIFIERS(wParam));
             }
         }
         break;
      case WM_MOUSELEAVE:
         {
             SWindow *w = (SWindow *) GetWindowLong(hWnd, GWL_USERDATA);
 
             if (w && w->enterCallback) {
                 w->enterCallback(w->data, FALSE);
             }
             w->mouseInWindow = FALSE;
         }
         break;
      case WM_SETFOCUS:
      case WM_KILLFOCUS:
         {
             SWindow *w = (SWindow *) GetWindowLong(hWnd, GWL_USERDATA);
 
             if (w && w->focusCallback) {
                 w->focusCallback(w->data, message == WM_SETFOCUS);
             }
         }
         break;
      case WM_LBUTTONDOWN:
         rc = doMButtonCallback(SW_MOUSE1, TRUE, hWnd, message, wParam, lParam);
         break;
      case WM_LBUTTONUP:
         rc = doMButtonCallback(SW_MOUSE1, FALSE, hWnd, message, wParam, lParam);
         break;
      case WM_MBUTTONDOWN:
         rc = doMButtonCallback(SW_MOUSE2, TRUE, hWnd, message, wParam, lParam);
         break;
      case WM_MBUTTONUP:
         rc = doMButtonCallback(SW_MOUSE2, FALSE, hWnd, message, wParam, lParam);
         break;
      case WM_RBUTTONDOWN:
         rc = doMButtonCallback(SW_MOUSE3, TRUE, hWnd, message, wParam, lParam);
         break;
      case WM_RBUTTONUP:
         rc = doMButtonCallback(SW_MOUSE3, FALSE, hWnd, message, wParam, lParam);
      case WM_MOUSEWHEEL:
         if ((short)HIWORD(wParam) > 0)
             rc = doMButtonCallback(SW_MOUSE4, TRUE, hWnd, message, wParam, lParam);
         else
             rc = doMButtonCallback(SW_MOUSE5, TRUE, hWnd, message, wParam, lParam);
         break;
      case WM_KEYDOWN:
         rc = doKeyCallback(TRUE, hWnd, message, wParam, lParam);
         break;
      case WM_KEYUP:
         rc = doKeyCallback(FALSE, hWnd, message, wParam, lParam);
         break;
      case WM_NOTIFY:
         {
           int idCtrl = (int) wParam; 
           LPNMHDR pnmh = (LPNMHDR) lParam;

           if (pnmh->code == TVN_SELCHANGED) {
               NM_TREEVIEW *pnmtv = (NM_TREEVIEW *) lParam;
               STree *t = (STree *) GetWindowLong(pnmh->hwndFrom, GWL_USERDATA);
               if (t && t->treeCallback) {
                   t->treeCallback(t->data, SW_TREE_SELECT, 
                                   (STREEITEM) pnmtv->itemNew.hItem);
               }
           } else if (pnmh->code == TVN_BEGINDRAG) {
               NM_TREEVIEW *pnmtv = (NM_TREEVIEW *) lParam;
               STree *t = (STree *) GetWindowLong(pnmh->hwndFrom, GWL_USERDATA);
               if (t && t->treeCallback) {
                   t->treeCallback(t->data, SW_TREE_BEGIN_DRAG, 
                                   (STREEITEM) pnmtv->itemNew.hItem);
               }
           } else if (pnmh->code == HDN_TRACK) {
               HD_NOTIFY *phdr = (HD_NOTIFY *) lParam;
               SHeader *h = (SHeader *) GetWindowLong(pnmh->hwndFrom, 
                                                      GWL_USERDATA);
               if (h && h->headerCallback) {
                   h->headerCallback(h->data, phdr->iItem, phdr->pitem->cxy);
               }
           } else if (pnmh->code == TTN_NEEDTEXT) {
               TOOLTIPTEXT *pTTT = (TOOLTIPTEXT *) lParam;
               UINT nID = pTTT->hdr.idFrom;
               SWindow *w = (SWindow *) GetWindowLong(pnmh->hwndFrom, 
                                                      GWL_USERDATA);
               if (w && w->tooltipCallback) {
                   pTTT->lpszText = (char *) w->tooltipCallback(w->data, nID);
                   rc = TRUE;
               }
           } else if (pnmh->code == TBN_HOTITEMCHANGE) {
               NMTBHOTITEM *info = (NMTBHOTITEM *) lParam;
               SWindow *w = (SWindow *) GetWindowLong(pnmh->hwndFrom, 
                                                      GWL_USERDATA);
               if (w && w->highlightCallback) {
                   w->highlightCallback(w->data, swToPtr(info->idNew));
               }
           }
           break;
         }
      case WM_ERASEBKGND:
         {
             SWindow *wnd = (SWindow *) GetWindowLong(hWnd, GWL_USERDATA);
             if (wnd && !findExposeCallback(hWnd)) {
                 HDC hdc = (HDC) wParam;
                 RECT r;
                 GetClientRect(hWnd, &r);
                 FillRect((HDC) wParam, &r, wnd->bgBrush);
             }
         }
         break;
      case WM_CTLCOLORSTATIC:
         {
             SWindow *wnd = (SWindow *) GetWindowLong(hWnd, GWL_USERDATA);
             SetBkMode((HDC) wParam, TRANSPARENT);
             rc = (LRESULT) wnd->bgBrush;
             break;
         }
         break;
      case WM_PAINT:
         {
             SWindow *w = (SWindow *) GetWindowLong(hWnd, GWL_USERDATA);

             if (w && w->exposeCallback) {
                 PAINTSTRUCT p;

                 BeginPaint(w->hWnd, &p);
                 (*w->exposeCallback)(w->data, 
                                      p.rcPaint.left, p.rcPaint.top,
                                      p.rcPaint.right - p.rcPaint.left,
                                      p.rcPaint.bottom - p.rcPaint.top);
                 EndPaint(w->hWnd, &p);
             } else {
                 rc = DefWindowProc( hWnd, message, wParam, lParam);
             }
         }
         break;
      case WM_COMMAND:
         {
             int notifyCode = HIWORD(wParam); /* notification code */
             void *id = LOWORD(wParam); /* item, control, or accelerator identifier */
             HWND hWndCtl = (HWND) lParam; /* handle of control */

             SWindow *w = NULL;
             if (hWndCtl) {
                 w = (SWindow *) GetWindowLong(hWndCtl, GWL_USERDATA);
             }
             if (w && w->commandCallback && notifyCode == BN_CLICKED) {
                 w->commandCallback(w->data, id);
             } else if (w && w->focusCallback && notifyCode == EN_KILLFOCUS) {
                 w->focusCallback(w->data, FALSE);
             } else if (hWnd) {
                 w = (SWindow *) GetWindowLong(hWnd, GWL_USERDATA);
                 if (w && w->commandCallback) {
                     w->commandCallback(w->data, id);
                 }
             }
         }
         break;
      case WM_MENUSELECT:
         {
             SWindow *w = (SWindow *) GetWindowLong(hWnd, GWL_USERDATA);

             if (w && w->highlightCallback) {
                 w->highlightCallback(w->data, (void *)wParam);
             }
         }
         break;
      case WM_HSCROLL:
      case WM_VSCROLL:
         {
             int nScrollCode = (int) LOWORD(wParam);  /* scroll bar value */
             short nPos = (short int) HIWORD(wParam);  /* scroll box position */
             HWND hWndScrollBar = (HWND) lParam;  /* handle of scroll bar */
             int type, pos;
             SCROLLINFO info;
             SWindow *sb;

             info.cbSize = sizeof(SCROLLINFO);
             info.fMask = SIF_PAGE | SIF_POS;
             if (hWndScrollBar) {
                 GetScrollInfo(hWndScrollBar, SB_CTL, &info);
             } else if (message == WM_HSCROLL) {
                 GetScrollInfo(hWnd, SB_HORZ, &info);
             } else if (message == WM_VSCROLL) {
                 GetScrollInfo(hWnd, SB_VERT, &info);
             }
             pos = info.nPos;
             switch (nScrollCode) {
               case SB_THUMBPOSITION:
               case SB_THUMBTRACK:
                 type = SW_SB_SCROLL;
                 pos = nPos;
                 break;
               case SB_LINEUP:
                 type = SW_SB_LINE_UP;
                 pos -= 1;
                 break;
               case SB_LINEDOWN:
                 type = SW_SB_LINE_DOWN;
                 pos += 1;
                 break;
               case SB_PAGEUP:
                 type = SW_SB_PAGE_UP;
                 pos -= info.nPage;
                 break;
               case SB_PAGEDOWN:
                 type = SW_SB_PAGE_DOWN;
                 pos += info.nPage;
                 break;
               default:
                 type = SW_SB_SCROLL; 
                 break;
             }
             info.fMask = SIF_POS;
             if (hWndScrollBar) {
                 SetScrollPos(hWndScrollBar, SB_CTL, pos, TRUE);
                 sb = (SWindow *) GetWindowLong(hWndScrollBar, GWL_USERDATA);
                 if (sb && sb->scrollBarCallback) {
                     sb->scrollBarCallback(sb->data, type, info.nPos);
                 }
             } else if (message == WM_HSCROLL) {
                 ScrollWindow(hWnd, info.nPos - pos, 0, NULL, NULL);
                 UpdateWindow(hWnd);
                 SetScrollPos(hWnd, SB_HORZ, pos, TRUE);
             } else if (message == WM_VSCROLL) {
                 ScrollWindow(hWnd, 0, info.nPos - pos, NULL, NULL);
                 UpdateWindow(hWnd);
                 SetScrollPos(hWnd, SB_VERT, pos, TRUE);
             }
         }
         break;
      case WM_SIZE:
         {
             SWindow *w = (SWindow *) GetWindowLong(hWnd, GWL_USERDATA);
             int width = LOWORD(lParam);
             int height = HIWORD(lParam);
             if (w && w->resizeCallback) {
                 w->resizeCallback(w->data, width, height);
             }
             break;
        }
         break;
      case WM_CLOSE:
         {
             SWindow *w = (SWindow *) GetWindowLong(hWnd, GWL_USERDATA);
             if (w && w->closeCallback) {
                 w->closeCallback(w->data);
             } else {
                 swExit();
             }
         }
         break;
#ifdef HAVE_WINDOWS_SPACEBALL
      case WM_INPUT:
         processWM_INPUTEvent(lParam);
         break;
#endif
      default:
         rc = DefWindowProc( hWnd, message, wParam, lParam);
         break;
    }
    return rc;
}

static LRESULT CALLBACK dialogProc(HWND hWnd, UINT message, 
                                   WPARAM wParam, LPARAM lParam)
{
    switch(message) {
      case WM_COMMAND:
         {
             int notifyCode = HIWORD(wParam); /* notification code */
             int id = LOWORD(wParam); /* item, control, or accelerator identifier */
             HWND hWndCtl = (HWND) lParam; /* handle of control */

             SWindow *w = (SWindow *) GetWindowLong(hWndCtl, GWL_USERDATA);

             if (w && w->commandCallback && notifyCode == BN_CLICKED) {
                 w->commandCallback(w->data, swToPtr(id));
             } else {
                 w = (SWindow *) GetWindowLong(hWnd, GWL_USERDATA);
                 if (w && w->commandCallback) {
                     w->commandCallback(w->data, id);
                 }
             }
         }
         break;
      case WM_DESTROY:
         {
             /* free any windows allocated by swGetDialogItem */
             HWND child;
             for (child = GetTopWindow(hWnd); child != NULL;
                  child = GetNextWindow(child, GW_HWNDNEXT)) {
                 SWindow *w = (SWindow *) GetWindowLong(child, GWL_USERDATA);
                 if (w) free(w);
             }
         }
         break;
    }
    return FALSE;
}

static int CALLBACK
timerProc(HWND hWnd, UINT uMsg, STimer *idEvent, DWORD dwTime)
{
    STimer *t = (STimer *) idEvent;

    // not sure why this happens, but DDE sometimes clobbers the timer event
    if (idEvent < 0x10000) return 0;

    if (!t->callback(t->data)) swKillTimer(t);

    return 0;
}

static void
updatePen(SDContext *gc, int capStyle)
{
    HPEN oldPen, newPen;

    assert(gc != NULL);

    if (capStyle == PS_ENDCAP_ROUND) {
         newPen = CreatePen(PS_COSMETIC | gc->lineStyle, gc->lineWidth, gc->fgColor);
    } else {
         LOGBRUSH brush;

         brush.lbStyle = BS_SOLID;
         brush.lbColor = gc->fgColor;
         brush.lbHatch = 0;
         newPen = ExtCreatePen(PS_GEOMETRIC | gc->lineStyle | capStyle,
                               gc->lineWidth, &brush, 0, NULL);
    }
    oldPen = SelectObject(gc->hDC, newPen);
    if (gc->hOldPen == NULL) {
         gc->hOldPen = oldPen;
    } else {
         DeleteObject(oldPen);
    }
    gc->penDirty = FALSE;
}

static void
updateBars(SWND wnd)
{
    int cw, ch;  // child width & height
    int pw, ph;  // parent width & height
    SCROLLINFO info;

    swGetSize(wnd, &pw, &ph);
    swGetSize(wnd->child, &cw, &ch);

    info.cbSize = sizeof(SCROLLINFO);
    info.fMask = SIF_PAGE | SIF_RANGE;
    info.nPage = pw;
    info.nMin = 0;
    info.nMax = cw - 1;

    SetScrollInfo(wnd->hWnd, SB_HORZ, &info, FALSE);

    info.nPage = ph;
    info.nMin = 0;
    info.nMax = ch - 1;

    SetScrollInfo(wnd->hWnd, SB_VERT, &info, FALSE);
}

static HWND
findFrame(HWND wnd)
{
    HWND parent;

    if (!wnd) return NULL;
    for(;;) {
         parent = GetParent(wnd);
         if (!parent) return wnd;
         wnd = parent;
    }
    return NULL;
}

static BOOL
doAccelerators(MSG *msg)
{
    HWND frame;
    SWindow *w;
    char buf[BUFSIZE];

    if (!msg) return FALSE;

    if (msg->message == WM_CHAR && (msg->wParam == VK_RETURN || msg->wParam == VK_ESCAPE)
         && GetClassName(msg->hwnd, buf, BUFSIZE) != 0 && !strcmp(buf, "Edit")) {
         w = (SWindow *) GetWindowLong(msg->hwnd, GWL_USERDATA);
         if (msg->wParam == VK_RETURN && w->commandCallback) {
             w->commandCallback(w->data, TRUE);
             return TRUE;
         } else if (msg->wParam == VK_ESCAPE && w->commandCallback) {
             w->commandCallback(w->data, FALSE);
             return TRUE;
         }
    }
    if (!acceleratorsEnabled) return FALSE;

    frame = findFrame(msg->hwnd);
    w = (SWindow *) GetWindowLong(frame, GWL_USERDATA);
    if (w && w->accelerators) {
         return TranslateAccelerator(frame, w->accelerators, msg);
    } else {
         return FALSE;
    }
}

static void
centerWindow(HWND hWnd, HWND hParent)
{
    RECT r, pr;
    int x, y, width, height, pwidth, pheight, swidth, sheight;

    GetWindowRect(hWnd, &r);
    GetWindowRect(hParent, &pr);
    swidth = GetSystemMetrics(SM_CXSCREEN);
    sheight = GetSystemMetrics(SM_CYSCREEN);
    width = r.right - r.left + 1;
    height = r.bottom - r.top + 1;
    pwidth = pr.right - pr.left + 1;
    pheight = pr.bottom - pr.top + 1;
    x = pr.left + (pwidth - width) / 2;
    y = pr.top + (pheight - height) / 2;
    if (x < 0) x = 0;
    if (y < 0) y = 0;
    if (x > swidth - width) x = swidth - width;
    if (y > sheight - height) y = sheight - height;
    MoveWindow(hWnd, x, y, width, height, FALSE);
}

static int
findExposeCallback(HWND hWnd)
{
    for (; hWnd != NULL; hWnd = GetParent(hWnd)) {
        SWindow *w = (SWindow *) GetWindowLong(hWnd, GWL_USERDATA);
         if (w && w->exposeCallback) return TRUE;
    }
    return FALSE;
}

static void
getTextMetrics(HFONT font, TEXTMETRIC *metrics)
{
    HDC  hDC;
    HGDIOBJ oldFont;

    hDC = GetDC(NULL);
    oldFont = SelectObject(hDC, font);
    GetTextMetrics(hDC, metrics);
    SelectObject(hDC, oldFont);
    ReleaseDC(NULL, hDC);
}

int swGetLastError(void)
{
    return GetLastError();
}

#ifdef HAVE_WINDOWS_SPACEBALL
/*
 Based on SpaceNav.cpp from Virtual Terrain Project

/ Purpose: Implements Win32-specific support for the SpaceNavigator 6DOF device.
/
/ Based on: WM_INPUTExample.cpp, 3Dconnexion Inc. May 2007
/
/ Uses WM_INPUT/RawInput to get data from 3Dx devices.
/ You can disconnect and reconnect devices at will while the program is running.
/ You can connect more than one device at a time, and distinguish the arriving
/ data between the different devices.
/
/ Copyright (c) 2008 Virtual Terrain Project
/ Free for all uses, see license.txt for details.

/ Content of license.txt:

The source code and data in this distribution
are Copyright (c) 2001-2007 Virtual Terrain Project.

Permission is hereby granted, free of charge, to any person obtaining a
copy of this software and associated documentation files (the "Software"),
to deal in the Software without restriction, including without limitation
the rights to use, copy, modify, merge, publish, distribute, sublicense,
and/or sell copies of the Software, and to permit persons to whom the
Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included
in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR
OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
OTHER DEALINGS IN THE SOFTWARE.
*/



PRAWINPUTDEVICELIST g_pRawInputDeviceList = NULL;
PRAWINPUTDEVICE g_pRawInputDevices = NULL;
int g_nUsagePage1Usage8Devices;

#define UINT unsigned int

void swSpaceNavFree()
{
    if (g_pRawInputDeviceList)
        free(g_pRawInputDeviceList);
    if (g_pRawInputDevices)
        free(g_pRawInputDevices);
}

int swSpaceNavInit()
{
    // Find the Raw Devices
    UINT nDevices;
    UINT i;
    // Get Number of devices attached
    if (GetRawInputDeviceList(NULL, &nDevices, sizeof(RAWINPUTDEVICELIST)) != 0)
    { 
        return FALSE;
    }
    // Create list large enough to hold all RAWINPUTDEVICE structs
    if ((g_pRawInputDeviceList = (PRAWINPUTDEVICELIST)malloc(sizeof(RAWINPUTDEVICELIST) * nDevices)) == NULL)
    {
        return FALSE;
    }
    // Now get the data on the attached devices
    if (GetRawInputDeviceList(g_pRawInputDeviceList, &nDevices, sizeof(RAWINPUTDEVICELIST)) == -1) 
    {
        return FALSE;
    }

    g_pRawInputDevices = (PRAWINPUTDEVICE)malloc( nDevices * sizeof(RAWINPUTDEVICE) );
    g_nUsagePage1Usage8Devices = 0;

    /* Look through device list for RIM_TYPEHID devices with UsagePage == 1, Usage == 8 */
    
    for(i=0; i<nDevices; i++)
    {
        if (g_pRawInputDeviceList[i].dwType == RIM_TYPEHID)
        {
            RID_DEVICE_INFO dinfo;
            UINT nchars = 300;
            TCHAR deviceName[300];
            UINT sizeofdinfo = sizeof(dinfo);
            GetRawInputDeviceInfo(g_pRawInputDeviceList[i].hDevice,
                                  RIDI_DEVICENAME, deviceName, &nchars);
            
            dinfo.cbSize = sizeofdinfo;
            if (GetRawInputDeviceInfo(g_pRawInputDeviceList[i].hDevice,
                                      RIDI_DEVICEINFO, &dinfo, &sizeofdinfo ) >= 0)
            {
                if (dinfo.dwType == RIM_TYPEHID)
                {
                    RID_DEVICE_INFO_HID *phidInfo = &dinfo.hid;

                    // Add this one to the list of interesting devices?
                    // Actually only have to do this once to get input from all usage 1, usagePage 8 devices
                    // This just keeps out the other usages.
                    // You might want to put up a list for users to select amongst the different devices.
                    // In particular, to assign separate functionality to the different devices.
                    if (phidInfo->usUsagePage == 1 && phidInfo->usUsage == 8)
                    {
                        g_pRawInputDevices[g_nUsagePage1Usage8Devices].usUsagePage = phidInfo->usUsagePage;
                        g_pRawInputDevices[g_nUsagePage1Usage8Devices].usUsage    = phidInfo->usUsage;
                        g_pRawInputDevices[g_nUsagePage1Usage8Devices].dwFlags    = 0;
                        g_pRawInputDevices[g_nUsagePage1Usage8Devices].hwndTarget  = NULL;
                        g_nUsagePage1Usage8Devices++;
                    }
                }
            }
        }
    }
    // Register for input from the devices in the list
    if (RegisterRawInputDevices( g_pRawInputDevices, g_nUsagePage1Usage8Devices, sizeof(RAWINPUTDEVICE) ) == FALSE )
    {
        return FALSE;
    }
    return TRUE;
}

int all6DOFs[6];
int gotData;

void processWM_INPUTEvent(LPARAM lParam)
{  
    RAWINPUTHEADER header;
    UINT size = sizeof(header);
    LPRAWINPUT evt;
    gotData = FALSE;
    if ( GetRawInputData( (HRAWINPUT)lParam, RID_HEADER, &header,  &size, sizeof(RAWINPUTHEADER) ) == -1)
    {
        return;
    }

    // Set aside enough memory for the full event
    size = header.dwSize;
    evt = (LPRAWINPUT)malloc(size);
    if (GetRawInputData( (HRAWINPUT)lParam, RID_INPUT, evt, &size, sizeof(RAWINPUTHEADER) ) == -1)
    {
        free(evt);
        return;
    }
    else
    {
        if (evt->header.dwType == RIM_TYPEHID)
        {
            static BOOL bGotTranslation = FALSE, 
                        bGotRotation    = FALSE;
            LPRAWHID pRawHid = &evt->data.hid;

            // Translation or Rotation packet?  They come in two different packets.
            if (pRawHid->bRawData[0] == 1) // Translation vector
            {
                all6DOFs[0] = (pRawHid->bRawData[1] & 0x000000ff) | ((signed short)(pRawHid->bRawData[2]<<8) & 0xffffff00); 
                all6DOFs[1] = (pRawHid->bRawData[3] & 0x000000ff) | ((signed short)(pRawHid->bRawData[4]<<8) & 0xffffff00); 
                all6DOFs[2] = (pRawHid->bRawData[5] & 0x000000ff) | ((signed short)(pRawHid->bRawData[6]<<8) & 0xffffff00);
                bGotTranslation = TRUE;
            }
            else if (pRawHid->bRawData[0] == 2) // Rotation vector
            {
                all6DOFs[3] = (pRawHid->bRawData[1] & 0x000000ff) | ((signed short)(pRawHid->bRawData[2]<<8) & 0xffffff00); 
                all6DOFs[4] = (pRawHid->bRawData[3] & 0x000000ff) | ((signed short)(pRawHid->bRawData[4]<<8) & 0xffffff00); 
                all6DOFs[5] = (pRawHid->bRawData[5] & 0x000000ff) | ((signed short)(pRawHid->bRawData[6]<<8) & 0xffffff00);
                bGotRotation = TRUE;
            }
/*
            else if (pRawHid->bRawData[0] == 3) // Buttons (display most significant byte to least)
            {
                 OnButtons((unsigned char)pRawHid->bRawData[3],
                          (unsigned char)pRawHid->bRawData[2],
                          (unsigned char)pRawHid->bRawData[1]);
            }
*/

            if (bGotTranslation || bGotRotation)
                gotData = TRUE;               
        }
    }
    free(evt);
}

int swSpaceNavGetData(int index) 
{
    if (gotData)
        return all6DOFs[index];
    else
        return 0;
}
#else
int swSpaceNavInit() { return FALSE; }
int swSpaceNavGetData(int index) { return FALSE; }
void swSpaceNavFree() {}
#endif

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
    long lid = vid;
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
    return 1;
}

