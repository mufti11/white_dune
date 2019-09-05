/*
 * browser.c - win32 interface to web browser
 *
 * Copyright (C) 2000 Stephen F. White
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

#include <swt.h>
#include <windows.h>
#include <stdio.h>
#include <fcntl.h>
#include "WriteFlags.h"
#include "../../config.h"
#include "../../mysnprintf.h"

#define DEFAULT_BROWSER "\"C:\\Program Files\\Internet Explorer\\IExplore.exe\""
#define DEFAULT_FIREFOX "\"C:\\Program Files\\Mozilla Firefox\\firefox.exe\""
#define DEFAULT_EDGE    "\"C:\\Program Files\\C:\Windows\System32\cmd.exe start edge:\""

#define STARTUP_TIMEOUT 10000

typedef struct SBrowser {
    STABLE  prefs;
    char   *command;
    char   *application;
    int     vrmlLevel;
    int     useRemote;
    int     xtypExecute;
    char   *remoteCommand;
    char   *topic;

    DWORD   DDEInst;
    HSZ     ourService;
    HSZ     theirService;
    HSZ     topicSZ;
    HSZ     exitSZ;
    HANDLE  hProcess;
} SBrowser;

static int browserLaunch(char* command , HANDLE* hProcess, 
                         const char *path, SWND wnd);
static int browserConnect(SBROWSER browser);
static int browserGoTo(SBROWSER browser, const char *path);
static int bringToFront(SBROWSER browser);
static char *fmtMsg(LPCTSTR lpszFormat, ...);
static int getShellOpenCommand(SBROWSER browser, const char *extension);
static void saveBrowserPreferences(SBROWSER browser);

/*******************/
/* browser preview */
/*******************/

SBROWSER
swBrowserInit(STABLE prefs)
{
    SBrowser *browser = malloc(sizeof(SBrowser));
    browser->prefs = prefs;
    browser->command = mystrdup(swGetPreference(prefs, "PreviewCommand", ""));
    browser->useRemote = swGetIntPreference(prefs, "PreviewUseRemote", FALSE);
    browser->vrmlLevel = swGetIntPreference(prefs, "PreviewVrmlLevel", X3DOM);
    browser->xtypExecute = swGetIntPreference(prefs, "XtypExecuteDDE", FALSE);
    browser->remoteCommand = mystrdup(swGetPreference(prefs, 
                                      "PreviewRemoteCommand", ""));
    browser->application = mystrdup(swGetPreference(prefs, 
                                                    "PreviewApplication", ""));
    browser->topic = mystrdup(swGetPreference(prefs, "PreviewTopic", ""));

    if (!browser->command[0])
         getShellOpenCommand(browser, ".wrl");
    if (!browser->command[0])
         browser->command = strdup(DEFAULT_BROWSER);
    if (!browser->remoteCommand[0])
        browser->useRemote = FALSE;

    browser->hProcess = NULL;
    browser->DDEInst = 0L;

    return browser;
}

int
swBrowserGetVrmlLevel(SBROWSER browser)
{
    return browser->vrmlLevel;
}

int
swBrowserGetRemote(SBROWSER browser)
{
    return browser->useRemote;
} 

void
swBrowserSetRemote(SBROWSER browser, int remote)
{
    browser->useRemote = remote; 
} 

void
swBrowserGetSettings(SBROWSER browser, const char **command, 
                     int* vrmlLevel, int *useRemote, int *xtypExecute,
                     const char **remoteCommand, const char **application,
                     const char **topic)
{
    *command = browser->command;
    *vrmlLevel = browser->vrmlLevel;
    *xtypExecute = browser->xtypExecute;
    *useRemote = browser->useRemote;
    *remoteCommand = browser->remoteCommand;
    *application = browser->application;
    *topic = browser->topic;
}

void
swBrowserSetSettings(SBROWSER browser, const char *command, 
                     int vrmlLevel, int useRemote, int xtypExecute,
                     const char *remoteCommand, const char *application,
                     const char *topic)
{
    free(browser->command);
    free(browser->remoteCommand);
    free(browser->application);
    free(browser->topic);

    browser->command = mystrdup(command);
    browser->vrmlLevel = vrmlLevel;
    browser->useRemote = useRemote;
    browser->xtypExecute = xtypExecute;
    browser->remoteCommand = mystrdup(remoteCommand);
    browser->application = mystrdup(application);
    browser->topic = mystrdup(topic);

    saveBrowserPreferences(browser);
}

static void
saveBrowserPreferences(SBROWSER browser)
{
    swSetPreference(browser->prefs, "PreviewCommand", browser->command);
    swSetIntPreference(browser->prefs, "PreviewVrmlLevel", browser->vrmlLevel);
    swSetIntPreference(browser->prefs, "PreviewUseRemote", browser->useRemote);
    swSetIntPreference(browser->prefs, "XtypExecuteDDE", browser->xtypExecute);
    swSetPreference(browser->prefs, "PreviewRemoteCommand", browser->remoteCommand);
    swSetPreference(browser->prefs, "PreviewApplication", browser->application);
    swSetPreference(browser->prefs, "PreviewTopic", browser->topic);
}

void
swBrowserSetDefault(SBROWSER browser)
{
    browser->command = mystrdup("");
    browser->useRemote = TRUE;
    browser->vrmlLevel = X3DOM;
    browser->xtypExecute = FALSE;
    browser->remoteCommand = mystrdup("");
    browser->application = mystrdup("");
    browser->topic = mystrdup("");

    if (!browser->command[0])
        getShellOpenCommand(browser, ".wrl");
    if (!browser->command[0])
        browser->command = strdup(DEFAULT_BROWSER);
    if (!browser->remoteCommand[0])
        browser->useRemote = FALSE;
    browser->hProcess = NULL;
    browser->DDEInst = 0L;
}

void
swBrowserPreview(SBROWSER browser, const char *path, SWND wnd)
{
    if (browser->useRemote) {
//        if (!browserGoTo(browser, path)) {
            if (browserLaunch(browser->command,browser->hProcess, path, wnd)) {
                browserGoTo(browser, path);
            }
//        }
    } else {
        browserLaunch(browser->command,browser->hProcess, path, wnd);
    }
}

void
swBrowserShutdown(SBROWSER browser)
{
    saveBrowserPreferences(browser);
    if (browser->hProcess) {
        TerminateProcess(browser->hProcess, 1);
        CloseHandle(browser->hProcess);
    }
    free(browser->command);
    free(browser->remoteCommand);
    free(browser->application);
    free(browser->topic);

    if (browser->DDEInst != 0) {
        DdeNameService( browser->DDEInst, NULL, NULL, DNS_UNREGISTER);
        DdeFreeStringHandle( browser->DDEInst, browser->theirService);
        DdeFreeStringHandle( browser->DDEInst, browser->ourService);
        DdeFreeStringHandle( browser->DDEInst, browser->topicSZ);
        DdeUninitialize( browser->DDEInst );
    }
    free(browser);
}

static char *
fmtMsg(LPCTSTR lpszFormat, ...)
{
    char    *buf;
    LPTSTR lpszTemp;

    // format message into temporary buffer lpszTemp
    va_list argList;
    va_start(argList, lpszFormat);

    FormatMessage(FORMAT_MESSAGE_FROM_STRING|FORMAT_MESSAGE_ALLOCATE_BUFFER,
                  lpszFormat, 0, 0, (LPTSTR)&lpszTemp, 0, &argList);

    // assign lpszTemp into the resulting string and free the temporary
    buf = mystrdup(lpszTemp);
    LocalFree(lpszTemp);
    va_end(argList);
    return buf;
}

static int browserLaunch(char* command, HANDLE* hProcess, const char *path, 
                         SWND wnd)
{
    STARTUPINFO startInfo;
    PROCESS_INFORMATION processInfo;
    int rc;
    char *s, *cmdline;

    if (path && path[0]) {
        if (strstr(command,"%s") == NULL)
            cmdline = fmtMsg(command, path);
        else {
            // expand %s to path
            char buf[1024];
            mysnprintf(buf, 1023, command, path);
            cmdline = mystrdup(buf);
        }
    } else {
        cmdline = mystrdup(command);
        if (cmdline[0] == '"') {
            s = strchr(cmdline+1, '"');
            if (s) (*(s+1)) = 0;
        } else {
            s = strchr(cmdline, ' ');
            if (s) *s = 0;
        }
    }

    memset(&startInfo, 0, sizeof(startInfo));

    startInfo.cb = sizeof(STARTUPINFO);
    startInfo.dwFlags = STARTF_USESHOWWINDOW;
    startInfo.wShowWindow = SW_SHOWNA;

    if (CreateProcess(NULL, cmdline, NULL, NULL, FALSE, 0, NULL, NULL,
                      &startInfo, &processInfo) != 0) {
        WaitForInputIdle( processInfo.hProcess, STARTUP_TIMEOUT );
        CloseHandle(processInfo.hThread);
        hProcess = processInfo.hProcess;
        rc = TRUE;
    } else {
        rc = FALSE;
    }
    free(cmdline);
    return rc;
}

static HDDEDATA DDECallback(
    UINT type,          /* transaction type */
    UINT fmt,           /* clipboard data format    */
    HCONV hconv,        /* handle of conversation   */
    HSZ hsz1,           /* handle of string */
    HSZ hsz2,           /* handle of string */
    HDDEDATA hData,     /* handle of global memory object   */
    DWORD dwData1,      /* transaction-specific data    */
    DWORD dwData2       /* transaction-specific data    */ )
{
    return (HDDEDATA) 1;
}

static int browserGoTo(SBROWSER browser, const char *path)
{
    char *buf;

    HSZ data;
    HCONV conv;
    HDDEDATA res;
    int timeout = 120;
    int rc = FALSE;

    if (browser->DDEInst == 0) {
        if (!browserConnect(browser)) return FALSE;
    }
    conv = DdeConnect( browser->DDEInst, browser->theirService,
               browser->topicSZ, NULL );
    if (conv != NULL) {
        buf = fmtMsg( browser->remoteCommand, path );
        data = DdeCreateStringHandle( browser->DDEInst, buf, CP_WINANSI );

        if (browser->xtypExecute)
            res = DdeClientTransaction(buf, strlen(buf)+1, conv, 0L, 0, 
                                        XTYP_EXECUTE, TIMEOUT_ASYNC, NULL);
        else
            res = DdeClientTransaction(NULL, 0, conv, data, CF_TEXT, 
                                       XTYP_REQUEST, TIMEOUT_ASYNC, NULL);

         DdeDisconnect( conv);
         DdeFreeStringHandle( browser->DDEInst, data);
         free(buf);
         if (!browser->xtypExecute)
             bringToFront(browser);
         rc = TRUE;
    }
    return rc;
}

LONG queryValue(HKEY key, const char *subKey, char **result)
{
    LONG size, rc;

    rc = RegQueryValue(key, subKey, NULL, &size);
    if (rc == ERROR_SUCCESS) {
        *result = (char *) malloc(size + 1);
        RegQueryValue(key, subKey, *result, &size);
    } else {
        *result = mystrdup("");
    }
    return rc;
}

static int
getShellOpenCommand(SBROWSER browser, const char *extension)
{
    HKEY key;
    char *info;

    if (queryValue(HKEY_CLASSES_ROOT, extension, &info) != ERROR_SUCCESS) {
        return FALSE;
    }

    if (RegOpenKey(HKEY_CLASSES_ROOT, info, &key) != ERROR_SUCCESS) {
        free(info);
        return FALSE;
    }

    free(browser->command);
    free(browser->remoteCommand);
    free(browser->application);
    free(browser->topic);
    queryValue(key, "Shell\\Open\\command", &browser->command);
    queryValue(key, "Shell\\Open\\ddeexec", &browser->remoteCommand);
    browser->useRemote = browser->remoteCommand[0];
    queryValue(key, "Shell\\Open\\ddeexec\\Application", &browser->application);
    queryValue(key, "Shell\\Open\\ddeexec\\Topic", &browser->topic);

    RegCloseKey(key);
    free(info);
    return TRUE;
}

static int
browserConnect(SBROWSER browser)
{
    int rc = FALSE;

    UINT initRes = DdeInitialize(&browser->DDEInst,
                                 (PFNCALLBACK) DDECallback,
                                 APPCLASS_STANDARD | 
                                 CBF_FAIL_CONNECTIONS | 
                                 CBF_SKIP_DISCONNECTS,
                                 0);

    if (initRes == DMLERR_NO_ERROR) {
        browser->theirService = DdeCreateStringHandle(browser->DDEInst, browser->application, CP_WINANSI );
        browser->ourService = DdeCreateStringHandle(browser->DDEInst, "Dune", CP_WINANSI );
        browser->topicSZ = DdeCreateStringHandle(browser->DDEInst, browser->topic, CP_WINANSI );

        DdeNameService(browser->DDEInst, browser->ourService, NULL, DNS_REGISTER);
        rc = TRUE;
    }
    return rc;
}

static int bringToFront(SBROWSER browser)
{
    HSZ data, activate;
    HCONV conv;
    int timeout = 120;
    int rc = FALSE;

    activate = DdeCreateStringHandle(browser->DDEInst, "WWW_Activate", CP_WINANSI );
    conv = DdeConnect(browser->DDEInst, browser->theirService, activate, NULL );
    if (conv != NULL) {
        data = DdeCreateStringHandle(browser->DDEInst, "0xFFFFFFFF", CP_WINANSI );

        DdeClientTransaction(NULL, 0, conv, data, CF_TEXT, XTYP_REQUEST,
                             TIMEOUT_ASYNC, NULL);

        DdeFreeStringHandle( browser->DDEInst, data);
        DdeDisconnect( conv);
        rc = TRUE;
    }
    DdeFreeStringHandle( browser->DDEInst, activate);
    return rc;
}

void swRemoveFile(const char* filename)
{
    unlink(filename);
}


/**********/
/* upload */
/**********/

typedef struct SUpload {
    STABLE prefs;
    char  *commandline;
    char  *htmlTag;
    char  *password;
} SUpload;

SUPLOAD   
swUploadInit(STABLE prefs)
{
    SUpload *upload = malloc(sizeof(SUpload));
    upload->prefs = prefs;
    upload->commandline = mystrdup(swGetPreference(prefs, "UploadCommandLine",
                                                   ""));
    upload->htmlTag = mystrdup(swGetPreference(prefs, "UploadHtmlTags",
                                               ""));
    /* do not read/store password for security reasons */
    upload->password = malloc(1024);
    upload->password[0] = 0;
    VirtualLock(upload->password, 1024);

    return upload;
}

void      
swUploadGetSettings(SUPLOAD upload, 
                    const char **commandline, 
                    const char **htmlTag, 
                    const char **password)
{
    *commandline = upload->commandline;
    *htmlTag = upload->htmlTag;
    *password = upload->password;
}

void      
swUploadSetSettings(SUPLOAD upload, 
                    const char *commandline, 
                    const char *htmlTag, 
                    const char *password)
{
    free(upload->commandline);
    free(upload->htmlTag);

    upload->commandline = mystrdup(commandline);
    upload->htmlTag = mystrdup(htmlTag);
    if (password != NULL)
        mystrncpy_secure(upload->password, password, 1023);
    else
        upload->password[0] = 0;

    swSetPreference(upload->prefs, "UploadCommandLine", upload->commandline);
    swSetPreference(upload->prefs, "UploadHtmlTags", upload->htmlTag);
    /* do not read/store password for security reasons */
}

int      
swHasUpload(SUPLOAD upload) 
{
    if (strlen(upload->commandline) > 0)
        return 1;
    else
        return 0;
}

char*
swUpload(SUPLOAD upload, char *fileToUpload, SHBROWSER browser, SWND wnd)
{
    static char uploadCommand[1024];
    FILE *cmd;
    HANDLE f; 
    struct _SECURITY_ATTRIBUTES security_attrib = { 
        sizeof(SECURITY_ATTRIBUTES),
        NULL,  // default security descriptor of the calling process
        FALSE        
    };
    DWORD bytesWritten;
    static char htmlTags[1024];
    int needPassword = 0;
    char *htmlpath = (char *) malloc(1024);
    htmlpath[0] = 0;
   
    if ((upload->password != NULL) && (strlen(upload->password) != 0))
        if (strstr(upload->commandline, "%s")  !=  NULL)
            if (strstr(strstr(upload->commandline, "%s") + 2, "%s") !=  NULL) 
                needPassword = 1; // if commandline has two %s 
    VirtualLock(uploadCommand, 1024);
    if (needPassword)
        mysnprintf(uploadCommand, 1023, upload->commandline, upload->password, 
                   fileToUpload);
    else
        mysnprintf(uploadCommand, 1023, upload->commandline, fileToUpload, "");

    if (system(uploadCommand) != 0) {
        swMessageBox(wnd, "upload failed", "Upload failed", 
                     SW_MB_OK, SW_MB_WARNING);
        htmlpath[0] = 0;
        VirtualUnlock(uploadCommand, 1024);
        return htmlpath;
    } else
        VirtualUnlock(uploadCommand, 1024);

    swGetTempPath(htmlpath, ".dune_upload_reload", ".html", 1024);
    
    f = CreateFile(htmlpath, 
                   GENERIC_WRITE,                // open for writing 
                   0,                            // do not share 
                   &security_attrib,             // security attributes
                   CREATE_NEW,                   // do not overwrite existing 
                   FILE_ATTRIBUTE_NORMAL,        // normal file 
                   NULL);                        // no attr. template 

    if (f == NULL) {
        char msg[1024];
        mysnprintf(msg, 1023, "creation of new html file %s failed", htmlpath);
        swMessageBox(wnd, msg, "Upload reload error", SW_MB_OK, SW_MB_WARNING);
        htmlpath[0] = 0;
        return htmlpath;
    }
    if ((upload->password != NULL) && (strlen(upload->password) != 0))
        mysnprintf(htmlTags, 1023, upload->htmlTag, upload->password);
    else
        mysnprintf(htmlTags, 1023, upload->htmlTag, "");
    if (WriteFile(f, htmlTags, sizeof(htmlTags), &bytesWritten,NULL) ==0) {
        char msg[1024];
        mysnprintf(msg, 1023, "write to html file %s failed", htmlpath);
        swMessageBox(wnd, msg, "Upload reload error", SW_MB_OK, SW_MB_WARNING);
        return htmlpath;
    }
    CloseHandle(f);
    helpBrowserGoTo(browser, htmlpath);
    return htmlpath;
}

void      
swUploadCleanupPasswd(SUPLOAD upload)
{
    int i;
    for (i = 0; i < strlen(upload->password); i++)
        upload->password[i] = (char) 0;
    VirtualUnlock(upload->password, 1024);
}

/* help browser */

static void saveHelpBrowserPreferences(SHBROWSER browser);
static int getShellOpenHelpCommand(SHBROWSER browser, const char *extension);
static int helpBrowserGoTo(SHBROWSER browser, const char *path);
static int helpBringToFront(SHBROWSER browser);
static int helpBrowserConnect(SHBROWSER browser);

typedef struct SHelpBrowser {
    STABLE prefs;
    char  *helpCommand;
    char  *helpRemoteCommand;
    char  *helpApplication;
    char  *helpTopic;
    char  *helpUrl;
    char  *vrmlUrl;
    char  *x3dUrl;
    DWORD  DDEInst;
    HSZ    ourService;
    HSZ    theirService;
    HSZ    topicSZ;
    HSZ    exitSZ;
    HANDLE hProcess;
} SHBrowser;


SHBROWSER swHelpBrowserInit(STABLE prefs)
{
    SHBrowser *browser = malloc(sizeof(SHBrowser));
    char *url;
    const char *index = "index.html";
    const char *helpDir = HELP_URL;

    browser->prefs = prefs;
    browser->helpCommand = mystrdup(swGetPreference(prefs, "HelpCommand", ""));
    browser->helpRemoteCommand = mystrdup(swGetPreference(prefs, 
                                          "HelpRemoteCommand",""));
    browser->helpApplication = mystrdup(swGetPreference(prefs, 
                                        "HelpApplication", ""));
    browser->helpTopic = mystrdup(swGetPreference(prefs, "HelpTopic", ""));
    url = (char *) malloc(strlen(HELP_URL) + strlen(index) + 2);
    strcpy(url, HELP_URL);
    if (helpDir[strlen(helpDir - 1)] != '/')
        strcat(url, "/");
    strcat(url, index);
    browser->helpUrl = mystrdup(swGetPreference(prefs, "HelpURL", url));
    free(url);
    browser->vrmlUrl = mystrdup(swGetPreference(prefs, "HelpVrml", 
                                                VRML_NODES_URL));
    browser->x3dUrl = mystrdup(swGetPreference(prefs, "HelpX3D",
                                               X3D_URL));

    if (browser->helpCommand[0]==' ') {
        getShellOpenHelpCommand(browser, ".htm");
    } else if (!browser->helpCommand[0]) {
        getShellOpenHelpCommand(browser, ".htm");
    }

    browser->hProcess = NULL;
    browser->DDEInst = 0L;

    return browser;
}

void
swHelpBrowserSetDefaults(SHBROWSER browser)
{
    char *dunedocs = NULL;

    browser->helpCommand = mystrdup(WWW_BROWSER);
    dunedocs = getenv("DUNEDOCS");
    if (dunedocs != NULL) {
        char *url;
        url = (char *) malloc(strlen(dunedocs) + /* strlen(html) */ + 1);
        strcpy(url, dunedocs);
        browser->helpUrl = mystrdup(url);
        free(url);
    } else
        browser->helpUrl = mystrdup(HELP_URL);
    browser->vrmlUrl = mystrdup(VRML_NODES_URL);
    browser->x3dUrl = mystrdup(X3D_URL);
}


void swHelpBrowserGetSettings(SHBROWSER browser, 
                              const char **helpCommand, 
                              const char **helpRemoteCommand, 
                              const char **helpUrl, 
                              const char **vrmlUrl, const char **x3dUrl,
                              const char **helpApplication, 
                              const char **helpTopic)
{
    *helpCommand = browser->helpCommand;
    *helpRemoteCommand = browser->helpRemoteCommand;
    *helpUrl = browser->helpUrl;
    *vrmlUrl = browser->vrmlUrl;
    *x3dUrl = browser->x3dUrl;
    *helpApplication = browser->helpApplication;
    *helpTopic = browser->helpTopic;
}

void swHelpBrowserSetSettings(SHBROWSER browser, 
                              const char *helpCommand, 
                              const char *helpRemoteCommand, 
                              const char *helpUrl, 
                              const char *vrmlUrl, const char *x3dUrl,
                              const char *helpApplication, 
                              const char *helpTopic)
{
    free(browser->helpCommand);
    free(browser->helpRemoteCommand);
    free(browser->helpUrl);
    free(browser->vrmlUrl);
    free(browser->x3dUrl);
    free(browser->helpApplication);
    free(browser->helpTopic);

    browser->helpCommand = mystrdup(helpCommand);
    browser->helpRemoteCommand = mystrdup(helpRemoteCommand);
    browser->helpUrl = mystrdup(helpUrl);
    browser->vrmlUrl = mystrdup(vrmlUrl);
    browser->x3dUrl = mystrdup(vrmlUrl);
    browser->helpApplication = mystrdup(helpApplication);
    browser->helpTopic = mystrdup(helpTopic);

    saveHelpBrowserPreferences(browser);
}

void
swHelpBrowserX3d(SHBROWSER browser, const char* string, SWND wnd)
{
    int pathLen=strlen(browser->x3dUrl);
    char *path=malloc(pathLen+strlen(string)+2);
    strcpy(path,browser->x3dUrl);
    strcat(path,string);
    helpBrowserGoTo(browser, path);
    free(path);
}

static void
saveHelpBrowserPreferences(SHBROWSER browser)
{
    swSetPreference(browser->prefs, "HelpCommand", browser->helpCommand);
    swSetPreference(browser->prefs, "HelpRemoteCommand", browser->helpRemoteCommand);
    swSetPreference(browser->prefs, "HelpUrl", browser->helpUrl);
    swSetPreference(browser->prefs, "HelpVrml", browser->vrmlUrl);
    swSetPreference(browser->prefs, "HelpX3d", browser->x3dUrl);
    swSetPreference(browser->prefs, "HelpApplication", browser->helpApplication);
    swSetPreference(browser->prefs, "HelpTopic", browser->helpTopic);
}

void swHelpBrowserHTML(SHBROWSER browser, SWND wnd)
{
    swHelpBrowserPath(browser,  browser->helpUrl, wnd);
}

void
swHelpBrowserPath(SHBROWSER browser, const char* path, SWND wnd)
{
    if (strstr(browser->helpCommand, "%s") != NULL) {
        char buf[4096];
        mysnprintf(buf,4096, browser->helpCommand, path);
        browserLaunch(buf, browser->hProcess, " ", wnd);
    } else if (!helpBrowserGoTo(browser, path))
        if (browserLaunch(browser->helpCommand, browser->hProcess, NULL, wnd)) {
            helpBrowserGoTo(browser, path);
    }
}

char *swGetHelpUrl(SHBROWSER browser)
{
    return browser->helpUrl;
}

void
swHelpBrowserVRML(SHBROWSER browser, const char* selection_string, 
                  int isCover, SWND wnd)
{
    int pathLen=strlen(browser->vrmlUrl);
    const char *coverFile="/coverNodes/nodesRef.html";
    char* path;
    if (isCover)
        pathLen=strlen(browser->helpUrl)+strlen(coverFile)+1;
    path=malloc(pathLen+strlen(selection_string)+2);
    if (isCover) {
        strcpy(path, browser->helpUrl);
        strcat(path, coverFile); 
    } else
        strcpy(path,browser->vrmlUrl);
    strcat(path,"#");
    strcat(path,selection_string);
    if (strstr(browser->helpCommand, "%s") != NULL) {
        char buf[4096];
        mysnprintf(buf,4096, browser->helpCommand, path);
        browserLaunch(buf, browser->hProcess, " ", wnd);
    } else if (!helpBrowserGoTo(browser, path))
        if (browserLaunch(browser->helpCommand, browser->hProcess, NULL, wnd)) {
            helpBrowserGoTo(browser, path);
    }   
    free(path);
}

void swHelpBrowserShutdown(SHBROWSER browser)
{
    saveHelpBrowserPreferences(browser);
    if (browser->hProcess) {
        TerminateProcess(browser->hProcess, 1);
        CloseHandle(browser->hProcess);
    }
    free(browser->helpCommand);
    free(browser->helpRemoteCommand);
    free(browser->helpUrl);
    free(browser->vrmlUrl);
    free(browser->helpApplication);
    free(browser->helpTopic);

    if (browser->DDEInst != 0) {
        DdeNameService(browser->DDEInst, NULL, NULL, DNS_UNREGISTER);
        DdeFreeStringHandle(browser->DDEInst, browser->theirService);
        DdeFreeStringHandle(browser->DDEInst, browser->ourService);
        DdeFreeStringHandle(browser->DDEInst, browser->topicSZ);
        DdeUninitialize(browser->DDEInst );
    }
    free(browser);
}

static int
getShellOpenHelpCommand(SHBROWSER browser, const char *extension)
{
    HKEY key;
    char *info;

    if (queryValue(HKEY_CLASSES_ROOT, extension, &info) != ERROR_SUCCESS) {
        return FALSE;
    }

    if (RegOpenKey(HKEY_CLASSES_ROOT, info, &key) != ERROR_SUCCESS) {
        free(info);
        return FALSE;
    }

    free(browser->helpCommand);
    free(browser->helpApplication);
    free(browser->helpTopic);

    queryValue(key, "Shell\\Open\\command", &browser->helpCommand);
//    queryValue(key, "Shell\\Open\\ddeexec", &browser->helpRemoteCommand);
    browser->helpRemoteCommand = mystrdup("%1");
    queryValue(key, "Shell\\Open\\ddeexec\\Application", &browser->helpApplication);
    queryValue(key, "Shell\\Open\\ddeexec\\Topic", &browser->helpTopic);

    RegCloseKey(key);
    free(info);
    return TRUE;
}

static int helpBrowserGoTo(SHBROWSER browser, const char *path)
{
    char       *buf;

    HSZ data;
    HCONV conv;
    HDDEDATA res;
    int timeout = 120;
    int rc = FALSE;

    if (browser->DDEInst == 0) {
        if (!helpBrowserConnect(browser)) return FALSE;
    }
    conv = DdeConnect(browser->DDEInst, browser->theirService,
                      browser->topicSZ, NULL );
    if (conv != NULL) {
        buf = fmtMsg( browser->helpRemoteCommand, path );
        data = DdeCreateStringHandle( browser->DDEInst, buf, CP_WINANSI );

        res = DdeClientTransaction(NULL, 0, conv, data, CF_TEXT, 
                                   XTYP_REQUEST, TIMEOUT_ASYNC, NULL);

        DdeDisconnect( conv);
        DdeFreeStringHandle( browser->DDEInst, data);
        free(buf);
        helpBringToFront(browser);
        rc = TRUE;
    }
    return rc;
}


static int helpBringToFront(SHBROWSER browser)
{
    HSZ data, activate;
    HCONV conv;
    int timeout = 120;
    int rc = FALSE;

    activate = DdeCreateStringHandle(browser->DDEInst, "WWW_Activate", CP_WINANSI );
    conv = DdeConnect( browser->DDEInst, browser->theirService, activate, NULL );
    if (conv != NULL) {
        data = DdeCreateStringHandle(browser->DDEInst, "0xFFFFFFFF", CP_WINANSI );

        DdeClientTransaction(NULL, 0, conv, data, CF_TEXT, XTYP_REQUEST,
                            TIMEOUT_ASYNC, NULL);

        DdeFreeStringHandle(browser->DDEInst, data);
        DdeDisconnect( conv);
        rc = TRUE;
    }
    DdeFreeStringHandle(browser->DDEInst, activate);
    return rc;
}

static int
helpBrowserConnect(SHBROWSER browser)
{
    int rc = FALSE;

    UINT initRes = DdeInitialize(&browser->DDEInst,
                                 (PFNCALLBACK) DDECallback,
                                 APPCLASS_STANDARD | 
                                 CBF_FAIL_CONNECTIONS | 
                                 CBF_SKIP_DISCONNECTS,
                                 0);

    if (initRes == DMLERR_NO_ERROR) {
        browser->theirService = DdeCreateStringHandle( browser->DDEInst, browser->helpApplication, CP_WINANSI );
        browser->ourService = DdeCreateStringHandle( browser->DDEInst, "Dune", CP_WINANSI );
        browser->topicSZ = DdeCreateStringHandle( browser->DDEInst, browser->helpTopic, CP_WINANSI );

        DdeNameService( browser->DDEInst, browser->ourService, NULL, DNS_REGISTER);
         rc = TRUE;
    }
    return rc;
}


/****************/
/* text editor */
/**************/

HANDLE hProcess;

int 
swCheckRunningProcess(void)
{
    DWORD ExitCode;
    if (GetExitCodeProcess(hProcess, &ExitCode))
        if (ExitCode == STILL_ACTIVE)
            return 1;
        else {
             CloseHandle(hProcess);
            return 0;
        }
    else
        return 0;

}

int
swCreateCheckableProcess(const char *cmdline)
{
    STARTUPINFO startInfo;
    PROCESS_INFORMATION processInfo;
    SECURITY_ATTRIBUTES securityAttributes;
    int rc;

    memset(&startInfo, 0, sizeof(startInfo));

    startInfo.cb = sizeof(STARTUPINFO);
    startInfo.dwFlags = STARTF_USESHOWWINDOW;
    startInfo.wShowWindow = SW_SHOWNA;

    memset(&securityAttributes, 0, sizeof(securityAttributes));
    
    securityAttributes.nLength = sizeof(securityAttributes);
    securityAttributes.lpSecurityDescriptor = (void*)PROCESS_QUERY_INFORMATION;
    securityAttributes.bInheritHandle = TRUE;
    
     if (CreateProcess(NULL, cmdline, &securityAttributes, NULL, TRUE, 0, 
                       NULL, NULL,
                       &startInfo, &processInfo) != 0) {
         WaitForInputIdle(processInfo.hProcess, STARTUP_TIMEOUT );
         CloseHandle(processInfo.hThread);
         hProcess = processInfo.hProcess;
         rc = 0;
    } else {
         rc = GetLastError();
    }
    return rc;
}

typedef struct STextedit {
    STABLE  prefs;
    char   *textEditCommand;
    char   *textEditLinenumberOption;
    int     textEditUseExtensionTxt;
    char   *imageEditCommand;
    char   *imageEdit4KidsCommand;
    char   *soundEditCommand;
    char   *movieEditCommand;
} STextedit;


STEXTEDIT
swTextEditInit(STABLE prefs)
{
    STextedit *textEdit = malloc(sizeof(STextedit));
    textEdit->prefs = prefs;
    if (getenv("WINEDITOR") != NULL) {
        textEdit->textEditCommand = mystrdup(swGetPreference(
              prefs, "TextEditCommand", getenv("WINEDITOR")));
        textEdit->textEditLinenumberOption = mystrdup(swGetPreference(
              prefs, "TextEditLinenumberOption", ""));
    } else {
        textEdit->textEditCommand = mystrdup(swGetPreference(
              prefs, "TextEditCommand", "C:\\Windows\\system32\\edit.com"));
        textEdit->textEditLinenumberOption = mystrdup(swGetPreference(
              prefs, "TextEditLinenumberOption", " "));
    }
    textEdit->textEditUseExtensionTxt = swGetIntPreference(
          prefs, "TextEditUseExtensionTxt", 1);
    textEdit->imageEditCommand = mystrdup(swGetPreference(
          prefs, "ImageEditCommand", "C:\\Windows\\system32\\mspaint.exe"));
    textEdit->imageEdit4KidsCommand = mystrdup(swGetPreference(
          prefs, "ImageEdit4KidsCommand", "C:\\Windows\\system32\\mspaint.exe"));
    textEdit->soundEditCommand = mystrdup(swGetPreference(
          prefs, "SoundEditCommand", HAVE_SOUND_EDITOR));
    textEdit->movieEditCommand = mystrdup(swGetPreference(
          prefs, "MovieEditCommand", HAVE_MOVIE_EDITOR));
    
    return textEdit;
}

void swTextEditGetSettingsUseExtensionTxt(STEXTEDIT textEdit,
                                          int *textEditUseExtensionTxt)
{
    *textEditUseExtensionTxt = textEdit->textEditUseExtensionTxt;
}

void
swTextEditGetSettings(STEXTEDIT textEdit, 
                      const char **textEditCommand,
                      const char **textEditLinenumberOption,
                      int *textEditUseExtensionTxt)
{
    *textEditCommand = textEdit->textEditCommand;
    *textEditLinenumberOption = textEdit->textEditLinenumberOption;
    *textEditUseExtensionTxt = textEdit->textEditUseExtensionTxt;
}

char *swTextEditGetCommand(STEXTEDIT textEdit)
{
    return textEdit->textEditCommand;
}

char *swImageEditGetSettings(STEXTEDIT textEdit)
{
    return textEdit->imageEditCommand;
}

char *swImageEdit4KidsGetSettings(STEXTEDIT textEdit)
{
    return textEdit->imageEdit4KidsCommand;
}

char *swSoundEditGetSettings(STEXTEDIT textEdit)
{
    return textEdit->soundEditCommand;
}

char *swMovieEditGetSettings(STEXTEDIT textEdit)
{
    return textEdit->movieEditCommand;
}

void
swTextEditSetSettings(STEXTEDIT textEdit, 
                      const char *textEditCommand,
                      const char *textEditLinenumberOption,
                      int textEditUseExtensionTxt)
{
    free(textEdit->textEditCommand);
    textEdit->textEditCommand = mystrdup(textEditCommand);

    free(textEdit->textEditLinenumberOption);
    textEdit->textEditLinenumberOption = mystrdup(textEditLinenumberOption);

    textEdit->textEditUseExtensionTxt = textEditUseExtensionTxt;

    swSetPreference(textEdit->prefs, "TextEditCommand", 
                    textEdit->textEditCommand);
    swSetPreference(textEdit->prefs, "TextEditLinenumberOption", 
                    textEdit->textEditLinenumberOption);    
    swSetIntPreference(textEdit->prefs, "TextEditUseExtensionTxt", 
                       textEdit->textEditUseExtensionTxt);

    textEdit->imageEditCommand = mystrdup(swGetPreference(
          textEdit->prefs, "ImageEditCommand", HAVE_IMAGE_EDITOR));
    textEdit->imageEdit4KidsCommand = mystrdup(swGetPreference(
          textEdit->prefs, "ImageEdit4KidsCommand", HAVE_IMAGE_EDITOR4KIDS));
    textEdit->soundEditCommand = mystrdup(swGetPreference(
          textEdit->prefs, "C:\\usr\\bin\\SoundEditCommand.exe", ""));
    textEdit->movieEditCommand = mystrdup(swGetPreference(
          textEdit->prefs, "C:\\usr\\bin\\movieEditCommand.exe", ""));
}

void
swImageEditSetSettings(STEXTEDIT textEdit, 
                      const char *imageEditCommand)
{
    free(textEdit->imageEditCommand);
    textEdit->imageEditCommand = mystrdup(imageEditCommand);
    swSetPreference(textEdit->prefs, "ImageEditCommand", 
                    textEdit->imageEditCommand);
}

void
swImageEdit4KidsSetSettings(STEXTEDIT textEdit, 
                            const char *imageEditCommand)
{
    free(textEdit->imageEdit4KidsCommand);
    textEdit->imageEdit4KidsCommand = mystrdup(imageEditCommand);
    swSetPreference(textEdit->prefs, "ImageEdit4KidsCommand", 
                    textEdit->imageEdit4KidsCommand);
}

void
swSoundEditSetSettings(STEXTEDIT textEdit, 
                      const char *soundEditCommand)
{
    free(textEdit->soundEditCommand);
    textEdit->soundEditCommand = mystrdup(soundEditCommand);
    swSetPreference(textEdit->prefs, "SoundEditCommand", 
                    textEdit->soundEditCommand);
}

void
swMovieEditSetSettings(STEXTEDIT textEdit, 
                       const char *movieEditCommand)
{
    free(textEdit->movieEditCommand);
    textEdit->movieEditCommand = mystrdup(movieEditCommand);
    swSetPreference(textEdit->prefs, "MovieEditCommand", 
                    textEdit->movieEditCommand);
}


