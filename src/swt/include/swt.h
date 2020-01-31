/*
 * swt.h - stephen's window toolkit
 *
 * Copyright (C) 1999 Stephen F. White, 2005 J. "MUFTI" Scheurich
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

#ifndef _SWT_H
#define _SWT_H

#ifdef __cplusplus
extern "C" {
#endif

#ifdef WIN32
struct HINSTANCE__;
typedef struct HINSTANCE__ *HINSTANCE;
#endif

#include "keycodes.h"
#include "swlanguages.h"
#include "swDebugf.h"

#define SW_RGB(r, g, b) ((r) | ((g) << 8) | ((b) << 16))
#define SW_RED(c)       ( (c)        & 0x0000ff)
#define SW_GREEN(c)     (((c) >> 8)  & 0x0000ff)
#define SW_BLUE(c)      (((c) >> 16) & 0x0000ff)

#include "swttypedef.h"

typedef unsigned char cbyte;

typedef void (*InitCallback)();
typedef void (*ExposeCallback)(void *data, int x, int y, int width, int height);
typedef void (*MouseCallback)(void *data, int x, int y, int modifiers);
typedef void (*KeyCallback)(void *data, int key, int value, int x, int y, 
                            int modifiers);
typedef void (*FocusCallback)(void *data, int value);
typedef void (*ResizeCallback)(void *data, int width, int height);
typedef void (*EnterCallback)(void *data, int value);
typedef void (*ActivateCallback)(void *data);
typedef void (*ScrollBarCallback)(void *data, int type, int value);
typedef void (*CommandCallback)(void *data, void *cmd);
typedef void (*HighlightCallback)(void *data, void *cmd);
typedef void (*CloseCallback)(void *data);
typedef int  (*DropCallback)(void *data,int type, int x, int y, int modifiers);
typedef void (*TreeCallback)(void *data, int type, STREEITEM item);
typedef void (*HeaderCallback)(void *data, int pos, int width);
typedef const char *(*TooltipCallback)(void *data, int id);
typedef int  (*TimerCallback)(void *data);

typedef void (*CleanupCallback)(void);

extern void swSetInitCallback(InitCallback cb );
extern void swSetExposeCallback(SWND wnd, ExposeCallback cb);
extern void swSetMouseCallback(SWND wnd, MouseCallback cb);
extern void swSetKeyCallback(SWND wnd, KeyCallback cb);
extern void swSetFocusCallback(SWND wnd, FocusCallback cb);
extern void swSetResizeCallback(SWND wnd, ResizeCallback cb);
extern void swSetEnterCallback(SWND wnd, EnterCallback cb);
extern void swSetActivateCallback(SWND wnd, ActivateCallback cb);
extern void swSetScrollBarCallback(SWND wnd, ScrollBarCallback cb);
extern void swSetCommandCallback(SWND wnd, HighlightCallback cb);
extern void swSetHighlightCallback(SWND wnd, CommandCallback cb);
extern void swSetCloseCallback(SWND wnd, CloseCallback cb);
extern void swSetClientData(SWND wnd, void *data);
extern void swCallCommandCallback(SWND wnd, void* id);
extern void swCallHighlightCallback(SWND wnd, void* id);

#ifdef WIN32
extern void swInitialize(HINSTANCE hInstance, int stereotype, int want8BitFlag);
extern void swOpenGLStartup(SWND wnd);
#else
extern int swInitialize( int *argc, char **argv, int stereotype);
#endif

int swHasVisual(void);

extern int swMainLoop(void);
extern void swExit(void);
extern void swSetCleanup(CleanupCallback cb);
extern void swCleanup(void);

extern void swGetScreenSize(int *x, int *y);
extern void swSetDefaultIcon(int resource);
extern void swDestroyWindow(SWND wnd);
extern int  swIsHidden(SWND wnd);
extern void swDestroyUpdate(SWND wnd);
extern void swSetPosition(SWND wnd, int x, int y);
extern void swGetPosition(SWND wnd, int *x, int *y);
extern void swGetAbsPosition(SWND wnd, int *x, int *y);
extern void swSetSize(SWND wnd, int width, int height);
extern void swGetSize(SWND wnd, int *width, int *height);
extern void swGetTotalSize(SWND wnd, int *width, int *height);
extern void swMoveWindow(SWND wnd, int x, int y, int width, int height);
extern void swGetTitle(SWND wnd, char *buf, int len);
extern void swSetTitle(SWND wnd, const char *title);
extern void swSetIcon(SWND wnd, SBITMAP bitmap, SBITMAP mask, int big);
extern SWND swGetParent(SWND wnd);
extern int swGetWindowColor(SWND wnd, int type);
extern void swShowWindow(SWND wnd);
extern void swHideWindow(SWND wnd);
extern void swMaximizeWindow(SWND wnd);
extern void swIconifyWindow(SWND wnd);
extern void swDeIconifyWindow(SWND wnd);
extern void swUpdate(void);
extern void swSetWindowBG(SWND wnd, int color);
extern int swIsVisible(SWND wnd);
extern int swIsMaximized(SWND wnd);
extern void swEnableWindow(SWND wnd, int enable);
extern void swSetFocus(SWND wnd);
extern void swEnableAccelerators(int enable);
extern void swInvalidateRect(SWND wnd, int x, int y,
                            int width, int height);
extern void swInvalidateWindow(SWND wnd);

extern STIMER swSetTimer(SWND wnd, int timeout, TimerCallback cb, void *data);
extern void swKillTimer(STIMER timer);
extern void swEnableTimers(void);
extern void swDisableTimers(void);

/* browser preview */
extern SBROWSER swBrowserInit(STABLE prefs);
extern void swBrowserPreview(SBROWSER browser, const char *path, SWND wnd);
extern void swBrowserGetSettings(SBROWSER browser,
                                 const char **command,
                                 int *vrmlLevel,
                                 int *useRemote,
                                 int *xtypExecuteOrFork,
                                 const char **remoteCommand,
                                 const char **application,
                                 const char **topic);
extern int swBrowserGetVrmlLevel(SBROWSER browser);
extern int swBrowserGetRemote(SBROWSER browser);
extern void swBrowserSetRemote(SBROWSER browser, int remote);
#ifndef WIN32
extern int swBrowserGetUseFork(SBROWSER browser);
#endif
extern void swBrowserSetSettings(SBROWSER browser,
                                 const char *command,
                                 int vrmlLevel,
                                 int useRemote,
                                 int xtypExecuteOrFork,
                                 const char *remoteCommand,
                                 const char *application,
                                 const char *topic);
extern void swBrowserSetDefault(SBROWSER browser);
extern void swBrowserShutdown(SBROWSER browser);
extern void swBrowserSavePreferences(SBROWSER browser, STABLE prefs);
#ifndef WIN32
 extern int swBrowserGetSettingsErrorToConsole(SBROWSER browser);
 extern void swBrowserSetSettingsErrorToConsole(SBROWSER browser, int flag);
#endif
/* upload */

extern SUPLOAD   swUploadInit(STABLE prefs);
extern void      swUploadGetSettings(SUPLOAD upload, 
                                     const char **commandline, 
                                     const char **htmlTag, 
                                     const char **password);
extern void      swUploadSetSettings(SUPLOAD upload, 
                                     const char *commandline, 
                                     const char *htmlTag, 
                                     const char *password);
extern void      swUploadCleanupPasswd(SUPLOAD upload); 
extern int       swHasUpload(SUPLOAD upload);
extern char     *swUpload(SUPLOAD upload, char *fileToUpload, 
                          SHBROWSER browser, SWND wnd);
extern void     *swToPtr(int id);
extern int       swFromPtr(void *id);

/* help browser */

#ifdef HAVE_WWW_BROWSER
# define WWW_BROWSER HAVE_WWW_BROWSER
#else                                   
# ifdef _WIN32
#  define WWW_BROWSER "\"C:\\Program Files\\Internet Explorer\\IExplore.exe\" %s"
# else
#  define WWW_BROWSER "firefox %s"
# endif
#endif

#ifdef HAVE_HELP_URL
# define HELP_URL HAVE_HELP_URL
#else                                   
# define HELP_URL "http://wdune.ourproject.org/docs/"
#endif

#ifdef HAVE_VRML_NODES_URL
# define VRML_NODES_URL HAVE_VRML_NODES_URL
#else                                   
# define VRML_NODES_URL "http://www.web3d.org/documents/specifications/14772/V2.0/part1/nodesRef.html"
#endif

#ifdef HAVE_X3D_URL
# define X3D_URL HAVE_X3D_URL
#else                                   
# define X3D_URL "http://www.web3d.org/documents/specifications/19775-1/V3.3/"
#endif

/* vrml browser */

#ifdef HAVE_VRML_BROWSER
# define VRML_BROWSER HAVE_VRML_BROWSER
#else                                   
# define VRML_BROWSER "firefox %s"
#endif

#ifdef HAVE_VRML_REMOTE_BROWSER
# define VRML_REMOTE 1
# define VRML_REMOTE_BROWSER HAVE_VRML_REMOTE_BROWSER
#else                          
# define VRML_REMOTE_BROWSER "firefox -remote OpenURL\\(%s,new-window\\)"
#endif

extern SHBROWSER swHelpBrowserInit(STABLE prefs);
extern void      swHelpBrowserSetDefaults(SHBROWSER browser);
extern void      swHelpBrowserGetSettings(SHBROWSER browser, 
                                          const char **Command, 
                                          const char **remoteCommand, 
                                          const char **helpUrl, 
                                          const char **vrmlUrl,
                                          const char **x3dUrl,
                                          const char **application, 
                                          const char **topic);
extern void      swHelpBrowserSetSettings(SHBROWSER browser, 
                                          const char *Command, 
                                          const char *remoteCommand, 
                                          const char *helpUrl, 
                                          const char *vrmlUrl,
                                          const char *x3dUrl,
                                          const char *application, 
                                          const char *topic);
extern void      swHelpBrowserHTML(SHBROWSER browser, SWND wnd);
extern char     *swGetHelpUrl(SHBROWSER browser);
extern void      swHelpBrowserPath(SHBROWSER browser, const char* path, 
                                   SWND wnd);
extern void      swHelpBrowserVRML(SHBROWSER browser, 
                                   const char* selection_string,
                                   int isCover, SWND wnd);
void             swHelpBrowserX3d(SHBROWSER browser, const char* string, 
                                  SWND wnd);
extern void      swHelpBrowserShutdown(SHBROWSER browser);

/* Texteditor */

extern STEXTEDIT swTextEditInit(STABLE prefs);
extern void      swTextEditGetSettings(STEXTEDIT textEdit, 
                                       const char **textEditCommand,
                                       const char **textEditLinenumberOption,
                                       int *textEditUseExtensionTxt);

extern void      swTextEditGetSettingsUseExtensionTxt(STEXTEDIT textEdit, 
                       int *textEditUseExtensionTxt);

extern void      swTextEditSetSettings(STEXTEDIT textEdit, 
                                       const char *textEditCommand,
                                       const char *textEditLinenumberOption,
                                       int textEditUseExtensionTxt);
extern char     *swTextEditGetCommand(STEXTEDIT textEdit);


/* Editors */

extern char     *swImageEditGetSettings(STEXTEDIT textEdit);
extern void      swImageEditSetSettings(STEXTEDIT textEdit, 
                                        const char *imageEditCommand);
extern char     *swImageEdit4KidsGetSettings(STEXTEDIT textEdit);
extern void      swImageEdit4KidsSetSettings(STEXTEDIT textEdit, 
                                             const char *imageEdit4KidsCommand);

extern char     *swSoundEditGetSettings(STEXTEDIT textEdit);
extern void      swSoundEditSetSettings(STEXTEDIT textEdit, 
                                        const char *soundEditCommand);

extern char     *swMovieEditGetSettings(STEXTEDIT textEdit);
extern void      swMovieEditSetSettings(STEXTEDIT textEdit, 
                                        const char *movieEditCommand);

/* Main Window */

extern SWND swCreateMainWindow(const char *title, int x, int y,
                               int width, int height);

/* ScrollBar */
extern SWND swCreateScrollBar(int flags,
                              int x, int y, int width, int height,
                              int min, int max, int value, int visible,
                              SWND parent);

extern void swScrollBarSetRange(SWND scrollBar, int min, int max);
extern void swScrollBarSetVisible(SWND scrollBar, int visible);
extern void swScrollBarSetValue(SWND scrollBar, int value);

/* Button */
extern SWND swCreateButton(const char *title, int x, int y,
                           int width, int height, SWND parent);

extern SWND swCreateCheckBox(const char *title, int x, int y,
                             int width, int height, SWND parent);

extern SWND swCreateRadioButton(const char *title, int x, int y,
                                int width, int height, SWND parent);
extern void swSetCheck(SWND wnd, int check);
extern int swGetCheck(SWND wnd);

/* Label */
extern SWND swCreateLabel(const char *title, int x, int y,
                          int width, int height, SWND parent);
extern void swLabelSetText(SWND wnd, const char *text);

/* Popup */

extern SWND swCreatePopup(const char *title, int x, int y,
                          int width, int height, SWND parent);

/* Text */

extern SWND swCreateTextEdit(int flags, int x, int y, int width, int height,
                             SWND parent);
extern void swSetText(SWND wnd, const char *text);
extern void swGetText(SWND wnd, char *text, int len);
extern void swTextEditSetSelection(SWND wnd, int start, int end);
extern void swSetTextCleanupPasswd(SWND wnd);
extern int swGetTextCaretPos(SWND wnd);
extern void swSetTextCaretPos(SWND wnd, int pos);

/* Canvas */
extern SWND swCreateCanvas(const char *title, int x, int y,
                           int width, int height, SWND parent);

/* Scrolled Window */
extern SWND swCreateScrolledWindow(int x, int y, int width, int height,
                                   SWND parent);
extern void swScrolledWindowSetChild(SWND wnd, SWND child);
extern void swGetScrollPosition(SWND wnd, int *x, int *y);
extern void swSetScrollPosition(SWND wnd, int x, int y);
extern void swGetScrollViewportSize(SWND wnd, int *w, int *h);
extern void swSetScrollSizes(SWND wnd, int width, int height);

/* Group Box */
extern SWND swCreateGroupBox(const char *title, int x, int y, int width,
                             int height, SWND parent);

/* Drawing Context */

extern SDC swCreateDC(SWND wnd);
extern SDC swCreateBitmapDC(SDC dc, int width, int height);
extern void swDestroyDC(SDC dc);

/* GL Context */
extern SGLC swCreateGLContext(SDC dc);
extern void swDestroyGLContext(SGLC context);
extern void swMakeCurrent(SDC dc, SGLC context);
extern void swSwapBuffers(SDC dc, SGLC context);
extern int swLoadGLFont(const char *name, int style,
                        float *kernX, float *kernY);
extern SGLC swCreateGLContext(SDC dc);
extern void swDestroyGLContext(SGLC context);
extern void swMakeCurrent(SDC dc, SGLC context);
#ifndef WIN32
extern SGLC swOSCreateGLContext(int *argc, char **argv);
extern void swOSDestroyGLContext(SGLC context);
extern void swOSMakeCurrent(SGLC context);
#endif
#ifndef WIN32
extern void swSetInDirectRendering(void);
#endif

/* Drawing */

extern void swSetFGColor(SDC dc, int color);
extern void swSetBGColor(SDC dc, int color);
extern void swSetFont(SDC dc, SFONT font);
extern void swSetLineStyle(SDC dc, int style);
extern void swSetLineWidth(SDC dc, int width);
extern void swSetCapStyle(SDC dc, int style);
extern void swSetJoinStyle(SDC dc, int style);
extern void swSetClipRect(SDC dc, int x, int y, int width, int height);
extern void swDrawLine(SDC dc, int x1, int y1, int x2, int y2);
extern void swDrawPoint(SDC dc, int x, int y);
extern void swDrawRect(SDC dc, int x, int y, int width, int height);
extern void swDraw3DRect(SDC dc, SWND wnd, int x, int y,
                         int width, int height);
extern void swDraw3DIndent(SDC dc, SWND wnd, int x, int y,
                           int width, int height);
/* beware: swDrawPlusBox changes foreground colour ! */
extern void swDrawPlusBox(SDC dc, int x, int y);
/* beware: swDrawMinusBox changes foreground colour ! */
extern void swDrawMinusBox(SDC dc, int x, int y);
extern void swDrawArc(SDC dc, int x, int y, int width, int height,
                      float startAngle, float endAngle);
extern void swDrawPolygon(SDC dc, int *points, int npoints);
extern void swDrawText(SDC dc, int x, int y, const char *str);
extern void swDrawTextTruncated(SDC dc, int x, int y, int width, int height, const char *str);
extern void swFillRect(SDC dc, int x, int y, int width, int height);
extern void swFillPatternRect(SDC dc, int x, int y, int width, int height, SBITMAP bitmap);
extern void swFillPolygon(SDC dc, int *points, int npoints);
extern void swFillArc(SDC dc, int x, int y, int width, int height,
                      float startAngle, float endAngle);
extern void swFillPie(SDC dc, int x, int y, int width, int height,
                      float startAngle, float endAngle);
extern void swDrawBitmap(SDC dc, SBITMAP bitmap, int sx, int sy,
                         int dx, int dy, int width, int height);
extern void swDrawMaskedBitmap(SDC dc, SBITMAP bitmap, SBITMAP mask,
                               int sx, int sy, int dx, int dy,
                               int width, int height);
extern void swCopyRect(SDC src, SDC dest, int sx, int sy,
                       int dx, int dy, int width, int height);
extern void swDrawMappedPixels(SBITMAP bitmap,
                               int srcx, int srcy, int dstx, int dsty,
                               int width, int height, const cbyte *data,
                               int numColors, const int *palette);

/* Font */

extern SFONT swGetDefaultFont();
extern SFONT swFindFont(const char *family, int style, int pointSize);
extern void swDeleteFont(SFONT font);
extern int swGetStringWidth(SFONT font, const char *str);
extern int swGetFontAscent(SFONT font);
extern int swGetFontDescent(SFONT font);
extern int swGetFontHeight(SFONT font);
#ifndef WIN32
extern void swSetDefaultFontSpec(char *fontSpec);
extern void swSetFontPath(char *path);
#endif

/* Menu */
extern SMENU swCreateMenuBar(SWND wnd);
extern SMENU swLoadMenuBar(SWND wnd, int id);
extern SMENU swAppendMenu(SMENU parent, const char *title);
extern void swAppendMenuItem(SMENU menu, const char *title, int id);
extern void swInsertMenuItem(SMENU menu, unsigned int position, const char *title,
                             int id);
extern void swDeleteMenuItem(SMENU menu, int id);
extern void swAppendSeparator(SMENU menu);
extern void swMenuSetFlags(SMENU menu, int id, int mask, int flags);

extern SSTATUSBAR swCreateStatusBar(SWND wnd);
extern void swDestroyStatusBar(SSTATUSBAR bar);
extern void swStatusBarSetText(SSTATUSBAR bar, const char *text);
extern SWND swStatusBarGetWindow(SSTATUSBAR bar);

/* Bitmap */
extern SBITMAP swLoadBitmap(SWND wnd, int id);
extern SBITMAP swLoadMappedBitmap(SWND wnd, int id, const int *mapFrom,
                                  const int *mapTo, int nMaps);
extern SBITMAP swCreateBitmap(SWND wnd, int width, int height);
extern SBITMAP swCreateMask(SBITMAP bitmap, int width, int height,
                            int bgcolor);
extern void swDestroyBitmap(SBITMAP bitmap);

/* Cursor */
extern SCURSOR swLoadCursor(int id);
extern void swSetCursor(SWND wnd, SCURSOR cursor);
extern void swDestroyCursor(SCURSOR cursor);

/* Dialogs */
extern SWND swCreateDialog(const char *title, int x, int y,
                           int width, int height, SWND parent);
extern SWND swLoadDialog(SWND parent, int id);
extern int swDialog(SWND dialog);
extern void swEndDialog(int rc);
extern SWND swGetDialogItem(SWND dialog, int item);
extern void swSetDefaultButton(SWND dialog, SWND button);
extern void swSetCancelButton(SWND dialog, SWND button);

extern int swOpenDirDialog(SWND wnd, const char *title, const char *filters, 
                           char *buf, int len);
extern int swOpenMaybeFileDialog(SWND wnd, const char *title,
                            const char *filters, char *buf, int len);
extern int swOpenFileDialog(SWND wnd, const char *title,
                            const char *filters, char *buf, int len);
extern int swOpenExecutableDialog(SWND wnd, const char *title,
                                  const char *filters, char *buf, int len);
extern int swSaveFileDialog(SWND wnd, const char *title,
                            const char *filters, char *buf, int len, 
                            const char* default_extension);

/* MessageBox */
extern int swMessageBox(SWND wnd, const char *text, const char *title,
                        int type, int icon);

/* ComboBox */
extern SWND swCreateComboBox(const char *title, int x, int y,
                             int width, int height, SWND parent, int id);
extern void swComboBoxDeleteAll(SWND list);
extern void swComboBoxAppendItem(SWND list, const char *str);
extern int swComboBoxGetSelection(SWND list);
extern void swComboBoxSetSelection(SWND list, int index);

/* Tree */
extern STREE swCreateTree(int x, int y, int width, int height, SWND parent);
extern void swDestroyTree(STREE tree);
extern SWND swTreeGetWindow(STREE tree);
extern void *swTreeGetItemData(STREE tree, STREEITEM item);
extern void swTreeSetItemData(STREE tree, STREEITEM item, void *data);
extern void swTreeSetItemImage(STREE tree, STREEITEM item,
                              int image1, int image2);
extern STREEITEM swTreeGetRootItem(STREE tree);
extern STREEITEM swTreeGetFirstChild(STREE tree, STREEITEM item);
extern STREEITEM swTreeGetLastChild(STREE tree, STREEITEM item);
extern STREEITEM swTreeGetNextItem(STREE tree, STREEITEM item);
extern STREEITEM swTreeGetPrevItem(STREE tree, STREEITEM item);
extern STREEITEM swTreeGetParentItem(STREE tree, STREEITEM item);
extern void swTreeDeleteItem(STREE tree, STREEITEM item);
extern void swTreeDeleteAllItems(STREE tree);
extern void swTreeSelectItem(STREE tree, STREEITEM item);
extern void swTreeDeselectItem(STREE tree, STREEITEM item);
extern void swTreeDeselectAll(STREE tree);
extern STREEITEM swTreeGetCurrentItem(STREE tree);
extern void swTreeSetCurrentItem(STREE tree, STREEITEM item);
extern void swTreeSelectDropTarget(STREE tree, STREEITEM item);
extern STREEITEM swTreeInsertItem(STREE tree, int position,
                                  STREEITEM relative, const char *name);
extern void swTreeSetImageList(STREE tree, SBITMAP bitmap, SBITMAP mask, 
                               int width, int height, int count);
extern void swTreeSetItemCollapsed(STREE tree, STREEITEM item, int collapsed);
extern void swTreeSetCallback(STREE tree, TreeCallback cb);
extern void swTreeSetClientData(STREE tree, void *data);
extern int swTreeGetItemTitle(STREE tree, STREEITEM item, char *buf, int len);
extern STREEITEM swTreeHitTest(STREE tree, int x, int y);
extern void swTreeSetOverlayImage(STREE tree, int index);
extern void swTreeSetFlags(STREE tree, STREEITEM item, int mask, int flags);
extern void swTreeSetItemName(STREE tree, STREEITEM item, const char* name);
extern char *swTreeGetItemName(STREE tree, STREEITEM item);


/* header control */
extern SHEADER swCreateHeader(int x, int y, int width, int height,
                              SWND parent);
extern void swDestroyHeader(SHEADER header);
extern void swHeaderInsertItem(SHEADER header, int pos, const char *name, 
                               int width);
extern void swHeaderGetSize(SHEADER header, int *width, int *height);
extern void swHeaderSetSize(SHEADER header, int width, int height);
extern int swHeaderGetItemWidth(SHEADER header, int pos);
extern void swHeaderSetItemWidth(SHEADER header, int pos, int width);
extern void swHeaderSetCallback(SHEADER header, HeaderCallback cb);
extern void swHeaderSetClientData(SHEADER header, void *data);

/* toolbar */
extern STOOLBAR swCreateToolbar(SWND parent, SBITMAP bitmap, 
                                int width, int height, int count,
                                const int *buttonInfo);
extern SWND swToolbarGetWindow(STOOLBAR toolbar);
extern void swToolbarSetButtonFlags(STOOLBAR toolbar, int button,
                                    int mask, int flags);
extern void swToolbarSetTooltipCallback(STOOLBAR toolbar, TooltipCallback cb);
extern void swToolbarSetClientData(STOOLBAR toolbar, void *data);
extern void swDestroyToolbar(STOOLBAR tb);

/* mouse capture */

extern void swSetCapture(SWND wnd);
extern void swReleaseCapture(SWND wnd);

/* miscellaneous */

extern double swGetCurrentTime(void);
extern void swGetTempFile(char *buf, const char* name, const char* ext, 
                          int len);
extern void swGetTempDir(char *path, int len);
extern void swGetTempPath(char *path, const char* name, const char* ext, 
                          int len);
extern char swGetPathSeperator(void);
extern int swTruncateClose(int filedes);
extern const char* swGetLinefeedString(void);
extern int swIsXML(const char *filename);
extern void swRemoveFile(const char* path);
extern char* swKillFileQuotes(char* filename);
extern char* swGetEnv(const char* variablename);
extern const char* swGetToNullDevice(void);
extern int swMkdirCommand(SWND wnd, const char* dir);
extern int swCopyFile(SWND wnd, const char* file, const char* dir);
extern int swShellCommand(SWND wnd, const char* file,
                          const char* command, const char* directory);
extern void swLoadString(int id, char *buf, int len);
extern void swDeleteCallbacks(SWND wnd);

extern int swCheckRunningProcess(void);
extern int swCreateCheckableProcess(const char *cmdline);
extern int swGetLastError(void);
extern void swSetLang(int lang);
extern int swGetLang(void);

/* persistent preferences */
extern void swPreferencesDefault(void);
extern STABLE swLoadPreferences(const char *companyName, const char *appName);
extern void swSavePreferences(STABLE pref);
extern void swDeletePreferences(STABLE pref);
extern const char *swGetPreference(STABLE table, const char *title,
                                   const char *defaultValue);
extern void swSetPreference(STABLE table, const char *title, const char *value);
extern int swGetIntPreference(STABLE table, const char *title, 
                              int defaultValue);
extern void swSetIntPreference(STABLE table, const char *title, int value);
extern void swUninstall(STABLE table, const char *companyName, 
                        const char *appName);

/* drag und drop */

extern void swRegisterDropTarget(SWND wnd, DropCallback dropCB);
extern void swUnregisterDropTarget(SWND wnd);
extern int swDragDrop(SWND wnd, int operations, SBITMAP bitmap, SBITMAP mask,
                      int x, int y, int width, int height);

/* accelerators */

extern void swAddAccelerator(SWND wnd, int key, int modifiers, int id);

/* MacOSX X11 startup flag */

#ifdef MACOSX
extern void set_start_aqua_X11(void);
#endif

/* init callback */
extern void initCallback(void); 

/* Windows SpaceNavigator driver */

#ifdef WIN32
extern int swSpaceNavInit();
extern int swSpaceNavGetData(int index);
extern void swSpaceNavFree();
#endif


/* tree relationships */
enum swInsert {
    SW_INSERT_ROOT,
    SW_INSERT_BEFORE,
    SW_INSERT_AFTER,
    SW_INSERT_FIRST_CHILD,
    SW_INSERT_LAST_CHILD
};

/* scrollbar flags */
enum swScrollbarFlags {
    SW_VERTICAL   = 0x0001,
    SW_HORIZONTAL = 0x0002,
    SW_ARROWS     = 0x0004,
    SW_NO_ARROWS  = 0x0008
};

/* types of scrollbar events */
enum swScrollbarEvents {
    SW_SB_SCROLL,
    SW_SB_LINE_UP,
    SW_SB_LINE_DOWN,
    SW_SB_PAGE_UP,
    SW_SB_PAGE_DOWN
};

/* types of text edit controls */
enum swTextEditControls {
    SW_EDITABLE     = 0x0000,
    SW_NON_EDITABLE = 0x0001,
    SW_MULTI_LINE   = 0x0002,
    SW_SINGLE_LINE  = 0x0004,
    SW_PASSWORD     = 0x0008
};

/* font styles */
enum swTextFontStyles {
    SW_PLAIN     = 0x0000,
    SW_BOLD      = 0x0001,
    SW_ITALIC    = 0x0002,
    SW_UNDERLINE = 0x0004
};

/* aligment */
enum swTextAlignment {
    SW_ALIGN_LEFT,
    SW_ALIGN_CENTER,
    SW_ALIGN_RIGHT
};

/* message box types */
enum swMessageBoxTypes {
    SW_MB_OK,
    SW_MB_OKCANCEL,
    SW_MB_YESNO,
    SW_MB_YESNOCANCEL
};

/* message box icon types */
enum swMessageBoxIconTypes {
    SW_MB_NOICON,
    SW_MB_ERROR,
    SW_MB_INFORMATION,
    SW_MB_QUESTION,
    SW_MB_WARNING
};

/* message box return values */
#define IDOK 1
#define IDCANCEL 2
#define IDYES 6
#define IDNO 7

/* line styles */
enum swLineStyles {
    SW_SOLID,
    SW_DASH,
    SW_DOT
};

/* line cap styles */
enum swLineCapStyles {
    SW_CAP_ROUND,
    SW_CAP_SQUARE,
    SW_CAP_FLAT
};

/* line join styles */

enum swLineJoinStyles {
    SW_JOIN_ROUND,
    SW_JOIN_MITER,
    SW_JOIN_BEVEL
};

/* fill styles */

enum swFillStyles {
    SW_FILL_SOLID,
    SW_FILL_STIPPLE
};

/* tree item flags */
enum swTreeItemFlags {
    SW_TREE_ITEM_COLLAPSED = 0x0001,
    SW_TREE_ITEM_SELECTED  = 0x0002,
    SW_TREE_ITEM_OVERLAY   = 0x0004
};

/* standard cursors */
enum swStandardCursors {
    SW_CURSOR_ARROW,
    SW_CURSOR_DBL_ARROW_HORZ,
    SW_CURSOR_DBL_ARROW_VERT,
    SW_CURSOR_CROSS,
    SW_CURSOR_CIRCLE,
    SW_CURSOR_ROLL,
    SW_CURSOR_NONE
};

enum swTreeFlags {
    SW_TREE_SELECT,
    SW_TREE_BEGIN_DRAG
};

/* modifier key masks */
enum swModifierKeyMasks {
    SW_SHIFT   = 0x0001,
    SW_ALT     = 0x0002,
    SW_CONTROL = 0x0004
};

enum swColorFlags {
    SW_COLOR_WINDOW_FG,
    SW_COLOR_WINDOW_BG,
    SW_COLOR_TSHADOW,
    SW_COLOR_BSHADOW,
    SW_COLOR_FACE,
    SW_COLOR_HIGHLIGHT,
    SW_COLOR_TEXT,
    SW_COLOR_HIGHLIGHT_TEXT
};

enum swDragDropFlags {
    SW_DRAG_ENTER,
    SW_DRAG_OVER,
    SW_DRAG_LEAVE,
    SW_DROP
};

enum swDragOperations {
    SW_DRAG_COPY = 0x0001,
    SW_DRAG_MOVE = 0x0002,
    SW_DRAG_LINK = 0x0004
};

enum swMenuFlags {
    SW_MENU_DISABLED   = 0x0001,
    SW_MENU_CHECKED    = 0x0002,
    SW_MENU_RADIO_ITEM = 0x0004
};

/* toolbar button states */

enum swToolbarFlags {
    SW_TB_DISABLED     = 0x0001,
    SW_TB_CHECKED      = 0x0002,
    SW_TB_DONT_UPDATE  = 0x0004
};

#ifdef __cplusplus
}
#endif

#endif /* _SWT_H */
