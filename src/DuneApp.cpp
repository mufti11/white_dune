/*
 * DuneApp.cpp
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

#include <errno.h>
#ifdef _WIN32
# include <direct.h>
#endif

#include <sys/types.h>
#include <sys/stat.h>
#ifndef _WIN32
# include <unistd.h>
# include <signal.h>
#endif

#include <ctype.h>
#include <time.h>

#include "stdafx.h"
#include "swt.h"

#include "parser.h"
#include "DuneApp.h"
#include "Scene.h"
#include "Path.h"
#include "MainWindow.h"
#include "URL.h"
#include "EulerAngles.h"
#include "resource.h"
#include "TexteditSettingsDialog.h"
#include "xerrorhandler.h"
#include "WriteFlags.h"

#ifdef HAVE_OLPC
#define DEFAULT_WIDTH 1200
#define DEFAULT_HEIGHT 900
#else
#define DEFAULT_WIDTH 1024
#define DEFAULT_HEIGHT 768
#endif

DuneApp *TheApp = NULL;
extern void setLanguage(char *lang);

void returntracker(void)
   {
   TheApp->returnTracker();
   }

#define MAX_RECENT_FILES 4

DuneApp::DuneApp() : PreferencesApp(), EcmaScriptApp(), StereoViewApp(), 
                     InputDeviceApp()                     
{
    TheApp = this;
    m_downloadDirectory = "";
    swSetDefaultIcon(IDI_DUNE_ICON);
    m_mainWnd = NULL;

    initPreferences();

    int n = GetIntPreference("RecentFiles", 0);
    for (int i = 0; i < n; i++) {
        char buf[1024];
        const char *filename;
        mysnprintf(buf, 1024, "RecentFile%d", i);
        filename = TheApp->GetPreference(buf, "");
        m_recentFiles.append(filename);
    }
    m_clipboardNode = NULL;
    swSetCleanup(returntracker);
    init_handlers();
    m_currentWindow = NULL;
    m_keyProtoFile = "ProtoFile";
    m_keyProtoCategory = "ProtoCategory";
    m_is4Kids = false;
    m_is4Catt = false;
    m_x3dv = false;
    m_numberLoadedInlines = 0;
    m_coverMode = false;
    m_kambiMode = false;
    m_x3domMode = false;
    m_importURL = "";
    m_oldImportURL = "";
    m_importFile = "";
    m_fileDialogDir = NULL;
    m_createAtZero[0] = false;
    m_createAtZero[1] = false;
    m_createAtZero[2] = false;
    m_blackAndWhiteIcons = false;
    m_glErrorCount = 0;
    m_vrml1Error = false;
    m_timeOut = -1;
    m_timeOutSet = true;
    m_lastInteraction = -1;
    m_tessellation = DEFAULT_TESSELLATION;
    if (m_tessellation <= 8)
        m_tessellation = 8;
    setLanguage(TheApp->GetStartLanguage()); 
    char *variant = TheApp->GetStartVariant();
    if (variant) {
        if (strcmp(variant, "4kids") == 0)
            TheApp->set4Kids();
        if (strcmp(variant, "4catt") == 0)
            TheApp->set4Catt();
        if (strcmp(variant, "cover") == 0)
            TheApp->setCoverMode();
        if (strcmp(variant, "kambi") == 0)
            TheApp->setKambiMode();
        if (strcmp(variant, "x3dom") == 0)
            TheApp->setX3domMode();
    }
    m_selectionLinenumberFlag = false;
    m_renderFaster = true;
    m_trackAnimation = false;
    m_checkSimpleCyclicSceneGraph = false;
    setScriptHeaderC("c:");
    setScriptHeaderCpp("c++:");
    setScriptHeaderJava("java:");
    m_hideStandardToolbar = false;
    m_importingDataFile = false;
    m_importingProtoLibrary = false;
    m_verbose = false;
    m_disableGlList = false;
    m_numExportFiles = 1;
    m_exitPid = 0;
}

void DuneApp::initPreferences(void)
{
    m_browser = swBrowserInit(getPrefs());
    m_upload = swUploadInit(getPrefs());
    m_helpBrowser = swHelpBrowserInit(getPrefs());
    m_textedit = swTextEditInit(getPrefs());
    StereoViewLoadPreferences();
    EcmaScriptLoadPreferences();
    OutputLoadPreferences();
    RouteViewLoadPreferences();
    StartWithLoadPreferences();
    ModellingLoadPreferences();
    RenderLoadPreferences();
    // load InputDeviceLoadPreferences() later, may need GUI
    m_loadInputDevicePreferences = true;
}

void 
DuneApp::setDefaults(void)
{
    swPreferencesDefault();
    initPreferences();
    PreferencesDefaults();
    OutputSetDefaults();
    RouteViewSetDefaults();
    StartWithSetDefaults();
    StereoViewSetDefaults();
}



void
DuneApp::SaveRecentFileList()
{
    int i, n = m_recentFiles.size();

    SetIntPreference("RecentFiles", n);
    for (i = 0; i < n; i++) {
        char buf[1024];
        mysnprintf(buf, 1024, "RecentFile%d", i);
        SetPreference(buf, m_recentFiles[i]);
    }
}

// there may be only one message at a time

void 
DuneApp::PrintMessageWindows(const char *text)
{
    bool written = false;
    List<MainWindow *>::Iterator *i;
    for (i = m_windows.first(); i; i = i->next()) {
         written = true;
         i->item()->setStatusText(text);
    }
    if (written == false)
        swDebugf("%s\n", text);
}


void 
DuneApp::PrintMessageWindowsId(int id)
{
    static char text[256];
    swLoadString(id, text, 255);
    PrintMessageWindows(text);
}

void 
DuneApp::PrintMessageWindowsInt(int id, int integer)
{
    char idText[256];
    static char text[512];
    swLoadString(id, idText, 255);
    mysnprintf(text, 511, idText, integer);
    PrintMessageWindows(text);

}

void 
DuneApp::PrintMessageWindowsFloat(int id, float f)
{
    char idText[256];
    static char text[512];
    swLoadString(id, idText, 255);
    mysnprintf(text, 511, idText, f);
    PrintMessageWindows(text);
}

void
DuneApp::PrintMessageWindowsString(int id, const char *string)
{
    char idText[256];
    static char text[512];
    swLoadString(id, idText, 255);
    mysnprintf(text, 511, idText, string);
    PrintMessageWindows(text);
}


void 
DuneApp::PrintMessageWindowsVertex(int id, const char *fieldName, int handle,
                                   Vec3f vec)
{
    if (is4Kids())
        return;
    static char text[256];
    static char idText[256];
    swLoadString(id, idText, 255);
    mysnprintf(text, 255, idText, fieldName, handle, vec.x, vec.y, vec.z);
    PrintMessageWindows(text);
}

// there may be only one MessageBox at a time

char *
DuneApp::loadPrompt(int prompt)
{
     static char string[256];
     string[0] = 0;
     if (prompt == SW_MB_ERROR)
         swLoadString(IDS_DUNE_ERROR, string, 255);
     else if (prompt == SW_MB_WARNING)
         swLoadString(IDS_DUNE_WARNING, string, 255);
     else 
         swLoadString(IDS_DUNE, string, 255);
     return string;
}

void                
DuneApp::MessageBox(const char *text, int prompt)
{
     if (m_windows.size() == 0)
         swDebugf("%s\n", text);
     else
         swMessageBox(m_mainWnd, text, loadPrompt(prompt), SW_MB_OK, 
                      prompt == -1 ? SW_MB_ERROR : prompt);
}

void
DuneApp::MessageBoxId(int id, int prompt)
{
     char string[256];
     swLoadString(id, string, 255);
     MessageBox(string, prompt);
}

void
DuneApp::MessageBox(int id, const char *str, int prompt)
{
    static char text[512];

    char idText[256];
    swLoadString(id, idText, 255);
    mysnprintf(text, 511, idText, str);
    MessageBox(text, prompt);
}

void
DuneApp::MessageBox(int id, const char *str1, const char *str2, int prompt)
{
    static char text[1024];

    char idText[256];
    swLoadString(id, idText, 255);
    mysnprintf(text, 1023, idText, str1, str2);
    MessageBox(text, prompt);
}

void
DuneApp::MessageBox(int id, int integer, int prompt)
{
    static char text[256];

    char idText[256];
    swLoadString(id, idText, 255);
    mysnprintf(text, 255, idText, integer);
    MessageBox(text, prompt);
}
 
void
DuneApp::MessageBox(int id, float f, int prompt)
{
    static char text[256];

    char idText[256];
    swLoadString(id, idText, 255);
    mysnprintf(text, 255, idText, f);
    MessageBox(text, prompt);
}

void
DuneApp::MessageBoxPerror(const char *object)
{
   char error[1024];
   mysnprintf(error, 1023,"%s: %s", object, strerror(swGetLastError()));
   MessageBox(error); 
}

void
DuneApp::MessageBoxPerror(int id, const char *object)
{
   MessageBox(id, object, strerror(swGetLastError())); 
}


void 
DuneApp::newMainWnd(SWND &mainWnd)
{
    int width = GetIntPreference("WindowWidth", DEFAULT_WIDTH);
    int height = GetIntPreference("WindowHeight", DEFAULT_HEIGHT);
    // sanity check
    if ((width <= 0) || (height <= 0)) {
        width = DEFAULT_WIDTH;
        height = DEFAULT_HEIGHT;
    }
    mainWnd = swCreateMainWindow("Dune", 0, 0, width, height);
    if (m_loadInputDevicePreferences) {
        InputDeviceLoadPreferences();
        m_loadInputDevicePreferences = false;
    }
}

void 
DuneApp::forgetClipBoardNode(void)
{
    if (m_clipboardNode)
        m_clipboardNode->unref();       
    m_clipboardNode = NULL;
}

void
DuneApp::OnFileNew(int id)
{
#ifndef HAVE_OPEN_IN_NEW_WINDOW
    if (!TheApp->checkSaveOldWindow())
        return;
#endif

    forgetNameTranslation();
    forgetClipBoardNode();

    Scene *scene = new Scene();
    if (id == ID_DUNE_FILE_NEW_X3DV)
        scene->setX3dv();
    else if (id == ID_DUNE_FILE_NEW_X3D_XML)
        scene->setX3dXml();
    newMainWnd(m_mainWnd);
    MainWindow *window = new MainWindow(scene, m_mainWnd);
    m_windows.append(window);

#ifndef HAVE_OPEN_IN_NEW_WINDOW
    TheApp->deleteOldWindow();
#endif
}

void
DuneApp::OnFileNewWindow()
{
    Scene *scene = new Scene();
    if (m_x3dv)
        scene->setX3dv();
    newMainWnd(m_mainWnd);
    MainWindow *window = new MainWindow(scene, m_mainWnd);
    m_windows.append(window);
}

const char* 
DuneApp::getExtension(int writeFlags)
{
    const char *ret = ".wrl";
    switch (writeFlags) {
      case X3D_XML:
        ret = ".xml";
        break;
      case X3DV:
        ret = ".x3dv";
        break;
    }
    if (writeFlags & X3DOM)
        ret = ".html";
    if (writeFlags & XITE)
        ret = ".html";
    return ret;
}

/* 
 * temporary files are written into the current directory 
 * temporary files are written into a list and deleted at program end
 */

char *
DuneApp::SaveTempFile(Scene *scene, const char *name, int writeFlags,
                      char *wrlpath)
{
    static char path[1024];

    mystrncpy_secure(path,scene->getPath(),1024);
    int i = 0;
    // produce a absolute path (remote netscape commands need one)
#ifdef _WIN32
    const char *dirsign="\\";
    if (path[1] != ':') 
        if (getcwd(path, 1023)!=NULL) {
#else
    const char *dirsign="/";
    if (path[0] != '/')
        if (getcwd(path, 1023)!=NULL) {
#endif
          mystrncpy_secure(path+strlen(path),dirsign,1024-strlen(path));
          mystrncpy_secure(path+strlen(path),scene->getPath(),
                           1024-strlen(path));
       }
    // strip filename
    for (i = strlen(path);( i >= 0) && (path[i] != dirsign[0]); i--);
    i++;
    bool writeError = false;
    if (writeFlags & X3DOM)
        writeFlags |= PURE_X3D;
    if (writeFlags & XITE) {
        writeFlags |= PURE_X3DV | X3DV;
        swGetTempFile(path+i, name, getExtension(X3DV), 1024-i);
        int f = open(path, O_WRONLY | O_CREAT,00666);
        if (f == -1) {
            if (scene->write(f, path, (writeFlags | SKIP_SAVED_TEST | 
                             (TEMP_SAVE & (~XITE)))))
               writeError = true;
            else if (swTruncateClose(f))
                writeError = true;
            AddToFilesToDelete(path);
        }
    }
    swGetTempFile(path+i, name, getExtension(writeFlags), 1024-i);
    int f = open(path, O_WRONLY | O_CREAT,00666);

    if (f == -1) {
        swGetTempPath(path, name, getExtension(writeFlags), 1024);
        f = open(path, O_WRONLY | O_CREAT,00666);
        if (f == -1) {
            char errorstring1[256];
            swLoadString(IDS_SAVE_PREVIEW_ERROR1, errorstring1, 255);
            char errorstring2[256];
            swLoadString(IDS_SAVE_PREVIEW_ERROR2, errorstring2, 255);
            char msg[1024];
            mysnprintf(msg, 1023, errorstring1, path, strerror(errno), 
                       errorstring2);
            TheApp->MessageBox(msg);
            return (char *)"";
        }
    } 
    URL fileURL;
    fileURL.FromPath(path);
    bool oldKeepUrls = TheApp->GetKeepURLs();
    TheApp->SetKeepURLs(true);
    if (scene->write(f, fileURL, writeFlags | TEMP_SAVE | SKIP_SAVED_TEST, 
                     wrlpath))
        writeError = true;
    else if (swTruncateClose(f))
        writeError = true;
    TheApp->SetKeepURLs(oldKeepUrls);
    if (writeError)
        TheApp->MessageBoxPerror(path);
    AddToFilesToDelete(path);
    if (writeError)
        return (char *)"";    
    return path;
}

void DuneApp::OnFilePreview(Scene* scene)
{
    // stop animation
    List<MainWindow *>::Iterator *wIter;
    for (wIter = m_windows.first(); wIter != NULL; wIter = wIter->next()) {
#ifndef _WIN32
        if (swBrowserGetUseFork(TheApp->GetBrowser()))
#endif        
            wIter->item()->Stop();
    }
#ifdef HAVE_AFLOCK
    stopTrackers();
#endif
    char wrlpath[1024] = { 0 };
    if (swBrowserGetVrmlLevel(GetBrowser()) & XITE) {
        mystrncpy_secure(wrlpath, SaveTempFile(scene, ".dune_preview", PURE_X3DV), 1024);
#ifdef _WIN32
    char posixPath[1024] = { 0 };
    // X_ITE nneds posix path
    toPosixPath(posixPath, wrlpath, 1024);
    mystrncpy_secure(wrlpath, posixPath, 1024);
#endif
    }
    char *temppath = SaveTempFile(scene,".dune_preview",
                                  swBrowserGetVrmlLevel(TheApp->GetBrowser()),
                                  wrlpath[0] ? wrlpath : NULL);

#ifndef _WIN32
    if (!swBrowserGetUseFork(TheApp->GetBrowser())) {
        
        swHideWindow(m_mainWnd);
//        swIconifyWindow(m_mainWnd);
        swDisableTimers();
        
    }
#endif
    const char *textEditCommand;
    const char *textEditLinenumberOption;
    int textEditUseExtensionTxt;
    swTextEditGetSettings(m_textedit,
                          &textEditCommand, &textEditLinenumberOption,
                          &textEditUseExtensionTxt);
    saveTempFiles(m_currentWindow, textEditUseExtensionTxt);
    if (strlen(temppath) != 0) {
        int remote = swBrowserGetRemote(GetBrowser());
        if (swBrowserGetVrmlLevel(GetBrowser()) & (X3DOM | XITE))
            swBrowserSetRemote(GetBrowser(), 1);
#ifdef _WIN32
        MyString path = "";
        path += "file://";
        path += temppath;
        swBrowserPreview(GetBrowser(), path, m_mainWnd);
#else
        swBrowserPreview(GetBrowser(), temppath, m_mainWnd);
#endif
        swBrowserSetRemote(GetBrowser(), remote);
    }
#ifdef HAVE_AFLOCK
    restartTrackers();
#endif
#ifndef _WIN32
    if (!swBrowserGetUseFork(TheApp->GetBrowser())) {
//        swDeIconifyWindow(m_mainWnd);
        restoreTempFiles();
        swEnableTimers();

    }
#else
    restoreTempFiles();
#endif
}

void
DuneApp::OnFileUpload(Scene* scene)
{
    char *temppath = SaveTempFile(scene,".dune_upload",     
                                  swBrowserGetVrmlLevel(TheApp->GetBrowser()));
    char *htmlpath = (char *)"";
    if (strlen(temppath) != 0) {
        htmlpath = swUpload(m_upload, temppath, m_helpBrowser, m_mainWnd);
    }
    if (strlen(htmlpath) != 0)
        AddToFilesToDelete(htmlpath);    
}

class PathInt
{
public:
    int *path;
    int len;
    PathInt(int *p, int l) {
        path = p;
        len = l;
    }
};


void DuneApp::saveTempPath(Scene* scene)
{
    if (scene->getSelection()) {
        PathInt *pathInt = new PathInt(scene->getSelection()->copyPathInt(),
                                       scene->getSelection()->getPathLen());
        m_tempPath.append(pathInt);
    }
}

void DuneApp::restoreTempPath(Scene* scene, int i)
{
    if (m_tempPath[i]) {
        scene->setSelection(new Path(m_tempPath[i]->path,
                                     m_tempPath[i]->len, scene));
        delete [] m_tempPath[i]->path;
        delete m_tempPath[i];
        m_tempPath[i] = NULL;
    }
}


#ifndef HAVE_OPEN_IN_NEW_WINDOW

// save VRML files (with .wrl/.x3dv or .txt extension) into a list of 
// temporary files and hide the windows (to call a editor next)
// the file of the currentWindow is first stored in the list

bool
DuneApp::saveTempFiles(MainWindow *currentWindow, int useExtensionTxt)
{
    m_tempPath.resize(0);
    m_tempFiles.Init();
    int count = 0;
    List<MainWindow *>::Iterator *wIter;
    for (wIter = m_windows.first(); wIter != NULL; wIter = wIter->next()) {
        Scene *scene = wIter->item()->GetScene();
        URL oldURL = scene->getURL();
        char* oldPath = mystrdup(scene->downloadPath(oldURL));

        char filename[1024] = { 0 };
        mysnprintf(filename,1024,"%s_%d",".dune_textedit",count++);
        char *savefile = (char *)malloc(1024);
        if (!useExtensionTxt)
            if (scene->isX3d())
                swGetTempFile(savefile, filename, ".x3dv", 1024);
            else
                swGetTempFile(savefile, filename, ".wrl", 1024);
        else
            swGetTempFile(savefile, filename, ".txt", 1024);

        URL url(oldPath, savefile);

        // is file writable ?
        int f = open(url.GetPath(), O_WRONLY | O_CREAT,00666);
        if (f == -1) {
            if (!useExtensionTxt)    
                if (scene->isX3d())
                    swGetTempPath(savefile, filename, ".x3dv", 1024);
                else
                    swGetTempPath(savefile,filename,".wrl",1024);
            else
                swGetTempPath(savefile,filename,".txt",1024);
            f = open(savefile, O_WRONLY | O_CREAT,00666);
            if (f == -1) {
                char errorstring1[256];
                swLoadString(IDS_SAVE_EDIT_ERROR1, errorstring1, 255);
                char errorstring2[256];
                swLoadString(IDS_SAVE_EDIT_ERROR2, errorstring2, 255);
                char msg[1024];
                mysnprintf(msg, 1023, errorstring1, savefile, 
                           strerror(errno), errorstring2);
                TheApp->MessageBox(msg);
                return false;
            }
        }
        AddToFilesToDelete(savefile);
        close(f);

        if (wIter->item() == currentWindow)
            initSelectionLinenumber();
        bool oldKeepUrls = TheApp->GetKeepURLs();
        TheApp->SetKeepURLs(true);
        int writeFlags = TEMP_SAVE | SKIP_SAVED_TEST;
        if (scene->isX3d())
            writeFlags |= X3DV;
        const char *file = url.GetPath();
        if (wIter->item()->SaveFile(file, savefile, writeFlags)) {
            saveTempPath(wIter->item()->GetScene());
            TheApp->removeClipboardNode(getClipboardNode());
            if ((currentWindow != NULL) && (wIter->item() == currentWindow))
                m_tempFiles.insert(new FileBackup(file, oldURL, oldPath));
            else
                m_tempFiles.append(new FileBackup(file, oldURL, oldPath));
            // hide and remove window
            swHideWindow(wIter->item()->getParentWindow());
        } else {
             // save failed, need to restore...
             return false;
        }  
        TheApp->SetKeepURLs(oldKeepUrls);
#ifndef _WIN32
        free(oldPath);
#endif
     }
     // deleting from a shrinking List is not trivial...
     wIter = m_windows.first(); 
     while (wIter != NULL) {
         MainWindow *windowToDelete = wIter->item();
         wIter = wIter->next();
         if (windowToDelete != NULL) {
             windowToDelete->destroyMainWindow();
             m_windows.remove(m_windows.find(windowToDelete));
// fix me         delete windowToDelete;
         }
     }
     swUpdate();
     return true;
}

static bool checkLinenumberOption(const char *linenumberOption)
{
    bool hasLinenumberOption = (linenumberOption[0] != 0);

    // check if linenumber option only contain blanks
    if (hasLinenumberOption) {
        hasLinenumberOption = false;
        for (unsigned int i = 0; i < strlen(linenumberOption); i++)
            if (linenumberOption[i] != ' ')
                hasLinenumberOption = true;
    }
    return hasLinenumberOption;
}


void
DuneApp::restoreTempFiles(void)
{
    List<FileBackup *>::Iterator *fIter = m_tempFiles.first();       
    for (int i = 0; i < m_tempFiles.size(); i++) {
        Scene *scene = new Scene();
        forgetClipBoardNode();
        forgetNameTranslation();
        bool error = false;
        do {
            newMainWnd(m_mainWnd);
            URL url(fIter->item()->m_url.GetPath(),
                    fIter->item()->m_backupFile);
            if (ImportFile(url.GetPath(), scene)) {
                scene->setExtraModifiedFlag();
                scene->setURL(fIter->item()->m_url);
                MyString path = fIter->item()->m_url.ToPath();
                scene->setPath(path);
                MainWindow *newwindow = new MainWindow(scene, m_mainWnd);
                m_windows.append(newwindow);
                error = false;
            } else {
                error = true;
                checkAndRepairTextEditCommand();

                const char *textEditCommand;
                const char *textEditLinenumberOption;
                int textEditUseExtensionTxt;

                swTextEditGetSettings(m_textedit, 
                                      &textEditCommand, 
                                      &textEditLinenumberOption,
                                      &textEditUseExtensionTxt);

                MyString command = mystrdup(textEditCommand);
                if (checkLinenumberOption(textEditLinenumberOption)) {
                    command += " ";
                    command += mystrdup(textEditLinenumberOption);
                    char number[1024];
                    mysnprintf(number, 1023, "%d", scene->getErrorLineNumber());
                    command += number;
                }
                command += " "; 
                command += fIter->item()->m_backupFile; 
                swHideWindow(m_mainWnd);
                swUpdate();
                delete scene;
                scene = new Scene();
                if (system((const char*)command) != 0) {
                    fprintf(stderr, "failed: %s\n", (const char*)command);
                }
            }
        } while (error);
        fIter = fIter->next();
        scene->findBindableNodes();
        UpdateAllWindows();
        restoreTempPath(scene, i);
        scene->UpdateViews(NULL, UPDATE_SELECTION);
    }
}

void
DuneApp::OnFileEdit(MainWindow *window, Scene* oldScene, char* filename)
{
    checkAndRepairTextEditCommand();

    const char *TextEditCommand;
    const char *TextEditLinenumberOption;
    int TextEditUseExtensionTxt;

    swTextEditGetSettings(m_textedit, 
                          &TextEditCommand, &TextEditLinenumberOption,
                          &TextEditUseExtensionTxt);

    swDisableTimers();

    bool saveSuccess = saveTempFiles(window, TextEditUseExtensionTxt);

    MyString command = mystrdup(TextEditCommand);
 
    if (checkLinenumberOption(TextEditLinenumberOption)) {
        command += " ";
        command += mystrdup(TextEditLinenumberOption);
        char number[1024];
        mysnprintf(number, 1023, "%d", getSelectionLinenumber());
        command += number;
    } 
         
    SavePreferences();
  
    if (filename == NULL) {
         List<FileBackup *>::Iterator *fIter = m_tempFiles.first();       
        for (int i = 0; i < m_tempFiles.size(); i++) {
            command += " ";
            command += fIter->item()->m_backupFile;
            fIter = fIter->next();
        }
    } else {
        command += " ";
        command += filename;
    }

    bool commandFailed = false;
    swHideWindow(m_mainWnd);
    swUpdate();
    if (saveSuccess)
        if (system((const char*)command) != 0)
           commandFailed = true; 
    swDestroyWindow(m_mainWnd);
    swEnableTimers();
    newMainWnd(m_mainWnd);
    if (!saveSuccess) { 
        char errorstring[256];
        swLoadString(IDS_SAVE_FAILED, errorstring, 255);
        TheApp->MessageBoxPerror(errorstring);
    }
    if (commandFailed)
        TheApp->MessageBoxId(IDS_EDIT_COMMAND_ERROR);
    restoreTempFiles();
}
#endif

bool 
DuneApp::AddFile(char* openpath, Scene* scene)
{
    if (!TheApp->ImportFile(openpath, scene)) {
        delete scene;
        return false;
    }
    return true;
}

bool
DuneApp::ImportFile(const char *openpath, Scene* scene, Node *node, int field)
{
    FILE *file;
    char path1[1024];
    char* filepath;

    URL importURL(openpath);
    setImportURL(openpath);
    setImportFile(openpath);

    MyString myPath;
    if (!scene->Download(importURL, &myPath))
        return false;

    mystrncpy_secure(path1, myPath, 1024); 
    filepath = swKillFileQuotes(path1);
    if (strcmp(filepath, "-") == 0)
        file = stdin;
    else if (!(file = fopen(filepath, "rb")))
       return false;

    scene->saveProtoStatus();

    int undoStackTopBeforeParse = scene->getUndoStackTop();    

    const char *errors = scene->parse(file, node, field, SCAN_FOR_BOTH);
    fclose(file);

    importURL = scene->getURL();

    scene->setSelection(scene->getRoot());
    scene->UpdateViews(NULL, UPDATE_SELECTION);
    scene->UpdateViews(NULL, UPDATE_ALL, NULL);

    if (errors[0]) {
#ifdef _WIN32
        swDebugf("%s", errors);
#endif
        TheApp->MessageBox(errors);
        // delete so far successful imported nodes on errors
        while ((scene->getUndoStackTop() > undoStackTopBeforeParse) &&
               scene->canUndo()) 
            scene->undo(); 
        scene->restoreProtoStatus();
           

        return false;
    }
    return true;
}

bool
DuneApp::OpenFile(const char *openpath)
{
    URL url;
    url.FromPath(openpath);

    forgetClipBoardNode();
    forgetNameTranslation();

    Scene *scene = new Scene();
    scene->setURL(url);
    scene->setPath(url.ToPath());

    if (!ImportFile(openpath, scene)) {
        delete scene;
        return false;
    }

    scene->findBindableNodes();

    scene->setSelection(scene->getRoot());
    scene->UpdateViews(NULL, UPDATE_SELECTION);
    scene->UpdateViews(NULL, UPDATE_ALL);
    AddToRecentFiles(openpath);

    // create a new window for our new scene
    newMainWnd(m_mainWnd);
    MainWindow *window = new MainWindow(scene, m_mainWnd);
    m_windows.append(window);

    UpdateAllWindows();

    return true;
}

void DuneApp::OnFileClose(MainWindow *window)
{

    if (!window->SaveModified()) {
        return;
    }
    List<MainWindow *>::Iterator *currentWindow = m_windows.find(window);
    SWND currentWnd = currentWindow->item()->getParentWindow();
    m_windows.remove(currentWindow);
// MacOSX X11 can freeze here. Bug in X11 shutdown code ?
#ifndef MACOSX
    delete window;
#endif
    if (m_windows.size() == 0) {
        for (size_t j = 0; j < m_filesToDelete.size(); j++)
            swRemoveFile(*m_filesToDelete[j]);
        swUploadCleanupPasswd(m_upload);
        Exit();
    } else if (currentWnd == m_mainWnd) {
        m_mainWnd = m_windows.first()->item()->getParentWindow();
    }
}

void DuneApp::OnFileExit()
{
    List<MainWindow *>::Iterator *i;
    for (i = m_windows.first(); i; i = i->next()) {
        if (!i->item()->SaveModified()) return;
    }
    for (i = m_windows.first(); i; i = i->next()) {
        delete i->item();
    }
    for (size_t j = 0; j < m_filesToDelete.size(); j++)
        swRemoveFile(*m_filesToDelete[j]);
    swUploadCleanupPasswd(m_upload);
    Exit();
}

void DuneApp::Exit()
{
    SaveRecentFileList();

    SavePreferences();
    swBrowserShutdown(m_browser);
// do not delete m_prefs, cause there may be a crash in swExit 8-(
//    swDeletePreferences(m_prefs);
    setNormalExit();
#ifdef MACOSX
    // needed to kill whitedune starter on MacOSX exit
    if (m_exitPid > 0) {
        kill(m_exitPid, 15);
    }
#endif
    swExit();
}

void DuneApp::UpdateAllWindows()
{
    for (List<MainWindow *>::Iterator *i = m_windows.first(); i; i = i->next())
        i->item()->GetScene()->UpdateViews(NULL, UPDATE_ALL);
}

int
DuneApp::GetNumRecentFiles() const
{
    return m_recentFiles.size();
}

const MyString &
DuneApp::GetRecentFile(int index) const
{
    return m_recentFiles[index];
}

void
DuneApp::AddToRecentFiles(const MyString &filename)
{
    int i = m_recentFiles.find(filename);

    if (i >= 0) {
        m_recentFiles.remove(i);
    }
    int n = MIN(m_recentFiles.size(), MAX_RECENT_FILES-1);
    for (i = n; i > 0; i--) {
        m_recentFiles[i] = "";
        m_recentFiles[i] = m_recentFiles[i-1];
    }
    m_recentFiles[0] = filename;
}

void
DuneApp::AddToFilesToDelete(char* string)
{
    // test if string is already in list
    for (size_t i = 0; i < m_filesToDelete.size(); i++)
       if (strcmp(string,(char*) m_filesToDelete[i])==0)
          return;
    m_filesToDelete.append(new MyString(string));
}

void
DuneApp::initSelectionLinenumber(void) 
{
    m_selectionLinenumberFlag = false;
    m_selectionLinenumber = 1;
}

void
DuneApp::checkSelectionLinenumberCounting(Scene* scene, Node* node)
{
    if (scene->getSelection()->getNode() == node)
        m_selectionLinenumberFlag = true;
}

void
DuneApp::incSelectionLinenumber(int increment)
{
    if (!m_selectionLinenumberFlag)
        m_selectionLinenumber += increment;
}

int
DuneApp::getSelectionLinenumber(void) 
{
    if (m_selectionLinenumberFlag)
        return m_selectionLinenumber;
    else 
        return 1;
}

void                
DuneApp::reOpenWindow(Scene* scene)
{
#ifndef HAVE_OPEN_IN_NEW_WINDOW
    newMainWnd(m_mainWnd);
    MainWindow *window = new MainWindow(scene, m_mainWnd);
    m_windows.append(window);
    TheApp->deleteOldWindow();
#endif    
}

bool 
DuneApp::checkSaveOldWindow(void)
{
    //  give user a chance to save first
    List<MainWindow *>::Iterator *curWinItr = m_windows.last();
    m_currentWindow = curWinItr->item();
    if (curWinItr != NULL) {
        if (curWinItr->item() != NULL) 
            if (!curWinItr->item()->SaveModified()) 
                return false;
        SavePreferences();
    }
    return true;
}

void 
DuneApp::deleteOldWindow(void)
{
    // delete the old window
    List<MainWindow *>::Iterator *curWinItr = NULL;
    for (List<MainWindow *>::Iterator *WinItr = m_windows.first(); WinItr; 
        WinItr = WinItr->next())
        if (WinItr->item() == m_currentWindow) {
            curWinItr = WinItr;
            m_currentWindow = NULL;
            break;
        }
    if (curWinItr != NULL) {
        if (curWinItr->item() != NULL)
            delete curWinItr->item();
        m_windows.remove(curWinItr);
    }
}

bool
DuneApp::hasUpload(void) 
{ 
    return swHasUpload(m_upload);
}

void
DuneApp::addToProtoLibrary(char* category, char* protoFile)
{
    // search for a free key
    for (int i=1; i<1000; i++) {
        char key[1024];
        mysnprintf(key, 1023, "%s%d", m_keyProtoFile, i);
        const char *value = GetPreference(key, "");
        if (strlen(value) == 0) {
            SetPreference(key, protoFile);
            mysnprintf(key, 1023, "%s%d", m_keyProtoCategory, i);
            SetPreference(key,category);
            break;
        }
    }
}

bool
DuneApp::readProtoLibrary(Scene* scene)
{
    m_importingProtoLibrary = true;
    for (int i=1; i<1000; i++) {
        char key[1024];
        mysnprintf(key, 1023, "%s%d", m_keyProtoFile, i);
        const char *value = GetPreference(key, "");
        if (strlen(value) != 0) {
            if (!ImportFile(value, scene)) {
                m_importingProtoLibrary = false;
                return false;
            }
        }
    }
    m_importingProtoLibrary = false;
    return true;
}

bool 
DuneApp::loadNewInline()
{
    if (m_numberLoadedInlines > GetMaxInlinesToLoad())
        return false; 
    m_numberLoadedInlines++;
    return true;
}

bool DuneApp::browseCommand(char *buf, int len, int ids_text) 
{
    buf[0] = 0;
    SWND wnd = mainWnd();
    char prompt[256];

    swLoadString(ids_text + swGetLang(), prompt, 255);
#ifdef _WIN32
    const char *choice = "executable (*.exe, *.com, *.bat)\0*.exe;*.com;*.bat\0All Files (*.*)\0*.*\0\0";
#else
    const char *choice = "executable (*)\0*;\0\0";
#endif
    return swOpenExecutableDialog(wnd, prompt, choice, buf, len);
}

bool DuneApp::checkCommand(const char *oldCommand, bool checkForFile)
{
    bool ret = true;
    bool checkFile = checkForFile;
#ifdef _WIN32
    checkFile = true;
#endif

    if (strlen(oldCommand) == 0)
        return false;

    // isolate first command and check for \ or /
    char *command = new char[strlen(oldCommand) + 1];
    strcpy(command, oldCommand);
    char *file = command;
    bool hasPathSeperator = false;
    bool hasQuote = false;
    bool isInString = false;
    for (unsigned int i = 0; i < strlen(command); i++)
        if ((command[i] == '/') || (command[i] == '\\'))
            hasPathSeperator = true;
        else if (command[i] == '"') {
            isInString = !isInString;
            hasQuote = true;
        } else if ((command[i] == ' ') && !isInString) {
            command[i] = 0;
            break;
        }

    // return if there is no path seperator
    if (checkFile)
        if (!hasPathSeperator) {
            delete [] command;
            return false;
        }

    // strip "s from file
    if (hasQuote) {
        file = command + 1;
        if (file[strlen(file) - 1] == '"')
            file[strlen(file) - 1] = 0;
    }

    if (checkFile) {
        // return if first command is not a file
        struct stat fileStat;
        if ((stat(file, &fileStat) == 0) && S_ISREG(fileStat.st_mode))
            ret = true;
        else
            ret = false;
    }
    delete [] command;
    return ret;
}

void DuneApp::checkAndRepairTextEditCommand(void)
{
    const char *textEditCommand;
    const char *textEditLinenumberOption;
    int textEditUseExtensionTxt;

    while (1) {
        swTextEditGetSettings(TheApp->GetTextedit(), 
                              &textEditCommand, &textEditLinenumberOption,
                              &textEditUseExtensionTxt);
        if (checkCommand(textEditCommand))
            break;
        TheApp->MessageBoxId(IDS_TEXTEDIT_PROMPT);
        TexteditSettingsDialog dlg(m_mainWnd);
        dlg.DoModal();
    }
}

void DuneApp::checkAndRepairImageEditCommand(void)
{
    const char *imageEditCommand;

    while (1) {
        imageEditCommand = swImageEditGetSettings(TheApp->GetTextedit());
        if (checkCommand(imageEditCommand))
            break;
        TheApp->MessageBoxId(IDS_IMAGEEDIT_PROMPT);
        TexteditSettingsDialog dlg(m_mainWnd);
        dlg.DoModal();
    }
}

void DuneApp::checkAndRepairImageEdit4KidsCommand(void)
{
    const char *imageEditCommand;

    while (1) {
        imageEditCommand = swImageEdit4KidsGetSettings(TheApp->GetTextedit());
        if (checkCommand(imageEditCommand))
            break;
        TheApp->MessageBoxId(IDS_IMAGEEDIT4KIDS_PROMPT);
        TexteditSettingsDialog dlg(m_mainWnd);
        dlg.DoModal();
    }
}

void DuneApp::checkAndRepairSoundEditCommand(void)
{
    const char *soundEditCommand;

    while (1) {
        soundEditCommand = swSoundEditGetSettings(TheApp->GetTextedit());
        if (checkCommand(soundEditCommand))
            break;
        TheApp->MessageBoxId(IDS_SOUNDEDIT_PROMPT);
        TexteditSettingsDialog dlg(m_mainWnd);
        dlg.DoModal();
    }
}

void DuneApp::checkAndRepairMovieEditCommand(void)
{
    const char *movieEditCommand;

    while (1) {
        movieEditCommand = swMovieEditGetSettings(TheApp->GetTextedit());
        if (checkCommand(movieEditCommand))
            break;
        TheApp->MessageBoxId(IDS_MOVIEEDIT_PROMPT);
        TexteditSettingsDialog dlg(m_mainWnd);
        dlg.DoModal();
    }
}

int
DuneApp::printRenderErrors(bool printOnOutOfMemory)
{
    GLenum error = glGetError();
    bool printError = (error != GL_NO_ERROR);
    if (printError)
        if ((!printOnOutOfMemory) && (error == GL_OUT_OF_MEMORY))
            printError = false;
    if (printError)
        printRenderErrors(error);
    return error;
}

void
DuneApp::printRenderErrors(GLenum error)
{
    if (error != GL_NO_ERROR) {
        if (m_glErrorCount < TheApp->GetX11ErrorsLimit())
            printf("%s\n",(const char *)gluErrorString(error));
        m_glErrorCount++;
    }
}

int                 
DuneApp::searchIllegalChar(char *id)
{
    int invalidCharPosition = -1;
    // see Annex A.3 of ISO-19776
    static const int illegalFirstChars[13] = 
       { 
       0x22, 0x23, 0x27, 0x2b, 0x2c, 0x2d, 0x2e, 
       0x5b, 0x5c, 0x5d, 
       0x7b, 0x7d, 0x7f
       };
    static const int illegalChars[11] = 
       { 
       0x22, 0x23, 0x27, 0x2c, 0x2e, 
       0x5b, 0x5c, 0x5d, 
       0x7b, 0x7d, 0x7f 
       }; 
    if (!isalpha(id[0])) {
        if (isdigit(id[0]))
            invalidCharPosition = 0;
        else
            if ((id[0] > 0) && (id[0] <= 0x20))
                invalidCharPosition = 0;
            else
                for (unsigned int j = 0; 
                     j < sizeof(illegalFirstChars)/sizeof(int); j++)
                    if (id[0] == illegalFirstChars[j]) {
                       invalidCharPosition = 0;
                       break;
                    }
    }
    if (invalidCharPosition == -1) 
        for (unsigned int i = 1; i < strlen(id); i++)
            if ((!isalnum(id[i])) && (!isdigit(id[i]))) {
                if ((id[i] > 0) && (id[i] <= 0x20))
                    invalidCharPosition = i;
                else
                    for (unsigned int j = 0; 
                         j < sizeof(illegalChars)/sizeof(int); j++)
                        if (id[i] == illegalChars[j]) {
                            invalidCharPosition = i;
                            break;
                        }
            }
    return invalidCharPosition;
}


void                
DuneApp::interact(void)
{
    if (m_timeOut > 0) {
        m_lastInteraction = time(NULL);
        m_timeOutSet = false;
    }
}

bool                
DuneApp::timeOut(void)
{
//    if (m_timeOutSet)
//        return true;
    if ((time(NULL) - m_lastInteraction) >= m_timeOut) {
        m_timeOutSet = true;
        // set selection to root of screengraph on timeout
        List<MainWindow *>::Iterator *wIter;
        for (wIter = m_windows.first(); wIter != NULL; wIter  = wIter ->next()) {
            Scene *scene = wIter->item()->GetScene();
            scene->setSelection(scene->getRoot());
            if (!scene->isRunning())
                wIter->item()->Play();
            scene->UpdateViews(NULL, UPDATE_SELECTION);    
        }
        return true;
    }
    return false;
}

void
DuneApp::endTrackAnimation(void)
{
   m_trackAnimation = false;
   List<MainWindow *>::Iterator *wIter;
   for (wIter = m_windows.first(); wIter != NULL; wIter  = wIter ->next()) {
       Scene *scene = wIter->item()->GetScene();
       if (scene->isRunning() && scene->isRecording())
           m_trackAnimation = true;
   }
}

FileBackup::FileBackup(const char* backupFile, URL url, const char* path)
{
     m_backupFile = "";
     m_backupFile += backupFile;

     m_url = url;

     m_path = "";
     m_path += path;
}

