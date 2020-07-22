/*
 * DuneApp.h
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

// data global to the whole application

#pragma once

// time between 2 frames in best case in milliseconds
#define FRAME_DELAY 70

#ifndef _LIST_H
#include "List.h"
#endif
#ifndef _ARRAY_H
#include "Array.h"
#endif
#ifndef _DUNE_STRING_H
#include "MyString.h"
#endif
#ifndef _NODE_H
#include "Node.h"
#endif
#ifndef _URL_H
#include "URL.h"
#endif
#ifndef _PREFERENCES_APP_H
#include "PreferencesApp.h"
#endif
#ifndef _ECMASCRIPT_APP_H
#include "EcmaScriptApp.h"
#endif
#ifndef _STEREO_VIEW_APP_H
#include "StereoViewApp.h"
#endif
#ifndef _INPUT_DEVICE_APP_H
#include "InputDeviceApp.h"
#endif
#ifndef _OUTPUT_APP_H
#include "OutputApp.h"
#endif
#ifndef _ROUTE_VIEW_APP_H
#include "RouteViewApp.h"
#endif
#ifndef _START_WITH_APP_H
#include "StartWithApp.h"
#endif
#ifndef _EXPORT_NAMES_APP_H
#include "ExportNamesApp.h"
#endif
#ifndef _MODELLING_APP_H
#include "ModellingApp.h"
#endif
#ifndef _RENDER_APP_H
#include "RenderApp.h"
#endif

#include <GL/gl.h>

class Scene;
class MainWindow;
class PathInt;

#define DEFAULT_TESSELLATION 31

class FileBackup { 
public:
                  FileBackup(const char* backupFile, URL url, const char* path);

    MyString      m_backupFile;
    URL           m_url;
    MyString      m_path;
};       

class DuneApp : public PreferencesApp, 
                public EcmaScriptApp, 
                public StereoViewApp, 
                public InputDeviceApp,
                public OutputApp,
                public RouteViewApp,
                public StartWithApp,
                public ExportNamesApp,
                public ModellingApp,
                public RenderApp {
public:
                        DuneApp();

    void                initPreferences(void);

    // file menu
    void                newMainWnd(SWND &mainWnd);
    void                OnFileNew(int id = -1);
    void                OnFileNewWindow();
    void                OnFilePreview(Scene* scene);
    void                OnFileUpload(Scene* scene);
    void                OnFileEdit(MainWindow *window, Scene* oldscene,
                                   char* filename=NULL);
    void                OnFileClose(MainWindow *window);
    void                OnFileExit();

    bool                ImportFile(const char *openpath, Scene* scene, 
                                   Node *node = NULL, int field = -1);
    bool                AddFile(char* openpath, Scene* scene);
    const char*         getExtension(int writeFlags);

    char *              SaveTempFile(Scene *scene, const char *name, 
                                     int writeFlags, char *wrlpath = NULL);
    bool                OpenFile(const char *path);

    void                reOpenWindow(Scene* scene);
    bool                checkSaveOldWindow(void);
    void                deleteOldWindow(void);

    // recent files in File menu 
    int                 GetNumRecentFiles() const;
    const MyString     &GetRecentFile(int index) const;
    void                AddToRecentFiles(const MyString &filename);
    void                SaveRecentFileList(void);

    int                 emergency_rescue(int sig = -1);

    SBROWSER            GetBrowser() const { return m_browser; }
    SHBROWSER           GetHelpBrowser() const { return m_helpBrowser; }
    STEXTEDIT           GetTextedit() const { return m_textedit; }
    SUPLOAD             GetUpload() const { return m_upload; }

    Node               *getClipboardNode(void) { return m_clipboardNode; }
    void                setClipboardNode(Node* node) { m_clipboardNode = node; }
    void                removeClipboardNode(Node* node) 
                           {
                           if (m_clipboardNode == node) 
                              m_clipboardNode = NULL; 
                           }

    bool                trackAnimation(void) { return m_trackAnimation; }
    void                setTrackAnimation(void) { m_trackAnimation = true; }
    void                endTrackAnimation(void);

    void                forgetClipBoardNode(void);

    SWND                mainWnd(void) {return m_mainWnd;}
    MainWindow         *getCurrentMainWindow(void) 
                           { 
                           List<MainWindow *>::Iterator *i = m_windows.first();
                           if (i != NULL)
                               return i->item(); 
                           else
                               return NULL;
                           }

    void                PrintMessageWindows(const char *text);
    void                PrintMessageWindowsId(int id);
    void                PrintMessageWindowsInt(int id, int integer);
    void                PrintMessageWindowsFloat(int id, float f);  
    void                PrintMessageWindowsString(int id, const char *string);
    void                PrintMessageWindowsVertex(int id, 
                                                  const char *fieldName, 
                                                  int handle, Vec3f vertex);

    char               *loadPrompt(int prompt);
    void                MessageBox(const char *text, int prompt = -1);
    void                MessageBoxId(int id, int prompt = -1);
    void                MessageBox(int id, const char *str, int prompt = -1);
    void                MessageBox(int id, const char *str1, const char *str2,
                                   int prompt = -1);
    void                MessageBox(int id, int integer, int prompt = -1);
    void                MessageBox(int id, float f, int prompt = -1);
    void                MessageBoxPerror(const char *object);
    void                MessageBoxPerror(int id, const char *object);

    void                AddToFilesToDelete(char* string); 

    MyString            getImportURL(void) { return m_importURL; }
    void                setImportURL(MyString newURL) 
                           {
                           m_oldImportURL = "";
                           m_oldImportURL += m_importURL;
                           m_importURL = "";
                           m_importURL += newURL; 
                           }
    MyString            getImportFile(void) { return m_importFile; }
    void                setImportFile(MyString newURL) 
                           {
                           m_importFile = "";
                           m_importFile += newURL; 
                           }
    MyString            getOldImportURL(void) { return m_oldImportURL; }

    void                initSelectionLinenumber(void); 
    void                checkSelectionLinenumberCounting(Scene* scene, 
                                                         Node* node);
    void                incSelectionLinenumber(int increment = 1);
    int                 getSelectionLinenumber(void);

    void                saveTempPath(Scene* scene);
    void                restoreTempPath(Scene* scene, int i);

    bool                saveTempFiles(MainWindow *currentWindow, 
                                      int useExtensionTxt);
    void                restoreTempFiles(void);

    void                UpdateAllWindows(void);

    List<MainWindow *>  getWindows(void) { return m_windows; }
    bool                hasUpload(void);

    void                addToProtoLibrary(char* category, char* protoFile);
    bool                readProtoLibrary(Scene* scene);
#ifdef HAVE_OLPC
    bool                isOLPC(void) { return true; } 
#else
    bool                isOLPC(void) { return false; } 
#endif
    bool                is4Kids(void) { return m_is4Kids; }
    void                set4Kids(void) { m_is4Kids = true; }
    bool                is4Catt(void) { return m_is4Catt; }
    void                set4Catt(void) { m_is4Catt = true; }
    bool                getX3dv(void) { return m_x3dv; }
    void                setFull(void) 
                            { 
                            m_is4Kids = false;
                            m_is4Catt = false;  
                            }
    void                setX3dv(void) { m_x3dv = true; }
    bool                loadNewInline(void);
    void                setCoverMode(void) { m_coverMode = true; }
    bool                getCoverMode(void) { return m_coverMode; }
    void                setKambiMode(void) { m_kambiMode = true; }
    bool                getKambiMode(void) { return m_kambiMode; }
    void                setX3domMode(void) { m_x3domMode = true; }
    bool                getX3domMode(void) { return m_x3domMode; }
    bool                GetNormalsOnMeshCreation() const
                           {
                           if (m_coverMode)
                               return true;
                           return OutputApp::GetNormalsOnMeshCreation(); 
                           }
    void                setFileDialogDir(char *dir) { m_fileDialogDir = dir; }
    char               *getFileDialogDir(void) { return m_fileDialogDir; }
    bool                getCreateAtZero(int i) { return m_createAtZero[i]; }
    void                setCreateAtZero(int i, bool b) { m_createAtZero[i] = b; }
    bool                browseCommand(char *buf, int len, int ids_text);
    bool                checkCommand(const char *command, 
                                     bool checkForFile = false);
    void                checkAndRepairTextEditCommand(void);
    void                checkAndRepairImageEditCommand(void);
    void                checkAndRepairImageEdit4KidsCommand(void);
    void                checkAndRepairSoundEditCommand(void);
    void                checkAndRepairMovieEditCommand(void);

    int                 getTessellation(void)
                            { return m_tessellation; }
    void                setTessellation(int tessellation)
                            { m_tessellation = tessellation; }

    void                printRenderErrors(GLenum error);
    int                 printRenderErrors(bool printOnOutOfMemory = true);

    bool                getVrml1Error(void) { return m_vrml1Error; }
    void                setVrml1Error(void) { m_vrml1Error = true; }

    int                 searchIllegalChar(char *id);

    void                setDefaults(void);

    bool                getDemoMode(void) { return m_timeOut != -1; }
    void                setDemoMode(int timeOut) { m_timeOut = timeOut; }

    bool                getRenderFaster(void)
                            { return m_renderFaster; }
    void                SetRenderFaster(bool s)
                            { m_renderFaster = s; }

    void                interact(void);
    bool                timeOut(void);
    bool                checkSimpleCyclicSceneGraph(void) 
                           { return m_checkSimpleCyclicSceneGraph; }
    void                setCheckSimpleCyclicSceneGraph(void) 
                           { m_checkSimpleCyclicSceneGraph = true; }

    const char *        getScriptHeaderC(void) { return m_scriptHeaderC; }
    void                setScriptHeaderC(const char *header) 
                           {
                           m_scriptHeaderC = "";   
                           m_scriptHeaderC += header;
                           }
 
    const char *        getScriptHeaderCpp(void) { return m_scriptHeaderCpp; }
    void                setScriptHeaderCpp(const char *header) 
                           {
                           m_scriptHeaderCpp = "";
                           m_scriptHeaderCpp += header;
                           }

    const char *        getScriptHeaderJava(void) { return m_scriptHeaderJava; }
    void                setScriptHeaderJava(const char *header) 
                           {
                           m_scriptHeaderJava = "";
                           m_scriptHeaderJava += header;
                           }
    void                hideStandardToolbar(void) 
                           { m_hideStandardToolbar = true; }
    bool                hasStandardToolbar(void) 
                           { return !m_hideStandardToolbar; }

    void                importingDataFile(bool flag) 
                           { m_importingDataFile = flag; }
    bool                isImportingDataFile(void) 
                           { return m_importingDataFile; }

    void                importingProtoLibrary(bool flag) 
                           { m_importingProtoLibrary = flag; }
    bool                isImportingProtoLibrary(void) 
                           { return m_importingProtoLibrary; }

    void                setDisableGlList(void) { m_disableGlList = true; }
    bool                disabledGlList(void) { return m_disableGlList; }

    MyString            getDownloadDirectory(void) 
                           { return m_downloadDirectory; }
    void                setDownloadDirectory(MyString dir) 
                           { 
                           m_downloadDirectory = ""; 
                           m_downloadDirectory += dir; 
                           }

    bool               getVerbose(void) { return m_verbose; }
    void               setVerbose(bool flag) { m_verbose = flag; }

    const char        *getWriteUrl(void) { return m_writeUrl; }
    void               setWriteUrl(const char *url)
                           {
                           m_writeUrl = "";
                           m_writeUrl += url;
                           }
    int                getNumExportFiles(void) { return m_numExportFiles; }
    void               setNumExportFiles(int i) { m_numExportFiles = i; }

    void               setExitPid(int pid) { m_exitPid = pid; }

    bool               getFullScreenAtBegin(void) 
                           { return m_fullScreenAtBegin; }
    void               setFullScreenAtBegin(void) { m_fullScreenAtBegin = 1; }

    bool               isCExporting(void) { return m_CExporting; }
    void               setCExporting(bool flag) { m_CExporting = flag; }

    MyString           getCrashedFile(void) { return m_recentFiles[0]; }

    void               setDynamicFieldsNode(void) 
                           { m_dynamicFieldsNode = true; }
    bool               getDynamicFieldsNode(void) 
                           { return m_dynamicFieldsNode; }
    void               unsetDynamicFieldsNode(void) 
                           { m_dynamicFieldsNode = false; }

    bool               getSkipChecks(void)
                           { return m_skipChecks; }
    void               setSkipChecks(bool flag)
                           { m_skipChecks = flag; }

    void                disableDraw(void) 
                           { m_avoidDraw = true; }
    void                enableDraw(void) 
                           { m_avoidDraw = false; }
    bool                getDrawAvoided(void) { return m_avoidDraw; }

    void                disableMatrixParallel()
                           { m_matrixParallel = false; }

    void                enableMatrixParallel()
                           { m_matrixParallel = true; }

    bool                getMatrixParallel()
                           { return m_matrixParallel; }

    void                setReadFromDownloads(bool flag)
                            { m_readFromDownloads = flag; }
    bool                getReadFromDownloads(void)
                            { return m_readFromDownloads; }
protected:
    void                Exit(void);

protected:
    List<MainWindow *>  m_windows;
    MyArray<MyString>   m_recentFiles;

    SBROWSER            m_browser;
    SHBROWSER           m_helpBrowser;
    STEXTEDIT           m_textedit;
    SUPLOAD             m_upload;

    SWND                m_mainWnd;
    Node               *m_clipboardNode;

    MyArray<MyString*>  m_filesToDelete;

    MyString            m_importURL;
    MyString            m_oldImportURL;
    MyString            m_importFile;

    int                 m_selectionLinenumber;
    bool                m_selectionLinenumberFlag;

    List<FileBackup *>  m_tempFiles;
    MyArray<PathInt *>  m_tempPath; 

    bool                m_trackAnimation;
    bool                m_coverMode;
    bool                m_kambiMode;
    bool                m_x3domMode;

    const char         *m_keyProtoFile;
    const char         *m_keyProtoCategory;
    bool                m_is4Kids;
    bool                m_is4Catt;
    bool                m_x3dv;
    int                 m_numberLoadedInlines;
    char               *m_fileDialogDir;
    bool                m_createAtZero[3];
    bool                m_blackAndWhiteIcons; 
    int                 m_tessellation;
    int                 m_glErrorCount;
    MainWindow         *m_currentWindow;
    bool                m_vrml1Error;
    int                 m_timeOut;
    bool                m_timeOutSet;
    int                 m_lastInteraction;
    bool                m_checkSimpleCyclicSceneGraph;
    bool                m_renderFaster;

    bool                m_loadInputDevicePreferences;

    MyString            m_scriptHeaderC;
    MyString            m_scriptHeaderCpp;    
    MyString            m_scriptHeaderJava;    

    bool                m_hideStandardToolbar;
    bool                m_importingDataFile;
    bool                m_importingProtoLibrary;

    bool                m_disableGlList;

    MyString            m_downloadDirectory;

    bool                m_verbose;

    MyString            m_writeUrl;

    int                 m_numExportFiles;

    int                 m_exitPid;

    int                 m_fullScreenAtBegin;

    bool                m_CExporting;

    bool                m_dynamicFieldsNode;

    bool                m_skipChecks;

    bool                m_avoidDraw;

    bool                m_matrixParallel;

    bool                m_readFromDownloads;
};

extern DuneApp *TheApp;


