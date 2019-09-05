/*
 * browser.c - unix interface to web browser
 *
 * Copyright (C) 2000 Stephen F. White, 2003 J. "MUFTI" Scheurich
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

#ifdef __sun
# define BSD_COMP
#endif
#include <fcntl.h>

#include <X11/Intrinsic.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>

#include "config.h"

#ifdef HAVE_TERM
# define TERM HAVE_TERM
#else
# define TERM "xterm" 
#endif

#ifdef HAS_MLOCK
# include <sys/mman.h>
#endif

#include "swt.h"
#include "mysnprintf.h"
#include "WriteFlags.h"

typedef struct SBrowser {
    STABLE prefs;
    char  *command;
    int    vrmlLevel;
    int    useFork;
    int    useRemote;
    char  *remoteCommand;
    pid_t  pid;
} SBrowser;

extern XtAppContext TheAppContext;

static int console = 2;

static int browserLaunch(SBROWSER browser, const char *path, SWND wnd);
static int browserRemote(SBROWSER browser, const char *path);
static void childInput(XtPointer closure , int *source, XtInputId *id);
static int waitForProcess(pid_t pid);
static pid_t createProcess(const char *cmdline, SWND wnd);
static pid_t createSimpleProcess(const char *cmdline);

static int errorToConsole = 1;

/*******************/
/* browser preview */
/*******************/

SBROWSER
swBrowserInit(STABLE prefs)
{
    SBrowser *browser = malloc(sizeof(SBrowser));
    browser->prefs = prefs;
    browser->command = mystrdup(swGetPreference(prefs, "PreviewCommand",
                                                VRML_BROWSER));
    browser->useFork = swGetIntPreference(prefs, "PreviewUseFork", FALSE);
    browser->vrmlLevel = swGetIntPreference(prefs, "PreviewVrmlLevel", X3DOM);
    browser->useRemote = swGetIntPreference(prefs, "PreviewUseRemote", FALSE);
    browser->remoteCommand = mystrdup(swGetPreference(prefs,
                                                      "PreviewRemoteCommand",
                                                      VRML_REMOTE_BROWSER));
    browser->pid = 0;

    errorToConsole = swGetIntPreference(prefs, "PreviewErrorToConsole", FALSE);

    console = open("/dev/console", O_WRONLY | O_NONBLOCK);
    if (console == -1)
        console = 2;

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

int
swBrowserGetUseFork(SBROWSER browser)
{
    return browser->useFork;
}

void
swBrowserGetSettings(SBROWSER browser, const char **command, 
                     int* vrmlLevel, int *useRemote, int *useFork,
                     const char **remoteCommand, const char **application,
                     const char **topic)
{
    *command = browser->command;
    *vrmlLevel = browser->vrmlLevel;
    *useRemote = browser->useRemote;
    *useFork = browser->useFork;
    *remoteCommand = browser->remoteCommand;
    *application = NULL;
    *topic = NULL;
}

void
swBrowserSetSettings(SBROWSER browser, const char *command, 
                     int vrmlLevel, int useRemote, int useFork,
                     const char *remoteCommand, const char *application,
                     const char *topic)
{
    free(browser->command);
    free(browser->remoteCommand);

    browser->command = mystrdup(command);
    browser->vrmlLevel = vrmlLevel;
    browser->useRemote = useRemote;
    browser->useFork = useFork;
    browser->remoteCommand = mystrdup(remoteCommand);

    swSetPreference(browser->prefs, "PreviewCommand", browser->command);
    swSetIntPreference(browser->prefs, "PreviewUseRemote", browser->useRemote);
    swSetIntPreference(browser->prefs, "PreviewVrmlLevel", browser->vrmlLevel);
    swSetIntPreference(browser->prefs, "PreviewUseFork", browser->useFork);
    swSetPreference(browser->prefs, "PreviewRemoteCommand", browser->remoteCommand);
}

int swBrowserGetSettingsErrorToConsole(SBROWSER browser)
{
    return errorToConsole;
}

void swBrowserSetSettingsErrorToConsole(SBROWSER browser, int toConsole)
{
    errorToConsole = toConsole;    
    swSetIntPreference(browser->prefs, "PreviewErrorToConsole", errorToConsole);
}

void
swBrowserSetDefault(SBROWSER browser)
{
    browser->command = mystrdup(VRML_BROWSER);
#ifdef VRML_REMOTE
    browser->useRemote = TRUE;
#else
    browser->useRemote = FALSE;
#endif
    browser->vrmlLevel = X3DOM;
    browser->useFork = TRUE;
    browser->remoteCommand = mystrdup(VRML_REMOTE_BROWSER);
    browser->pid = 0;
}

void
swBrowserPreview(SBROWSER browser, const char *path, SWND wnd)
{
    if (browser->useRemote) {
        if (!browserRemote(browser, path)) {
            browserLaunch(browser, path, wnd);
        }
    } else {
        browserLaunch(browser, path, wnd);
    }
}

void
swBrowserShutdown(SBROWSER browser)
{
    if (!browser) return;

    if (browser->pid != 0) {
        kill(browser->pid, SIGTERM);
        waitForProcess(browser->pid);
    }
    free(browser->command);
    free(browser->remoteCommand);
    free(browser);
}

static int
browserLaunch(SBROWSER browser, const char *path, SWND wnd)
{
    char arg[1024];

    mysnprintf(arg, 1023, browser->command, path);
    if (browser->useFork) {
        browser->pid = createProcess(arg, wnd);
        if (browser->pid != 0)
            return TRUE;
        else
            return FALSE;
    } else
        return system(arg);
}

static int
browserRemote(SBROWSER browser, const char *path)
{
    char url[1024];
    char arg[1024];
    pid_t pid;
    int ret;
    int i;

    mysnprintf(url, 1023, "file://%s", path);
    mysnprintf(arg, 1023, browser->remoteCommand, url);
    pid = createSimpleProcess(arg);
    ret = waitForProcess(pid);
    if (!ret) {
        for (i = 0; (browser->remoteCommand[i] != ' ') && 
                    (browser->remoteCommand[i] != 0) && (i < 1023); i++)
            arg[i] = browser->remoteCommand[i];
        arg[i] = ' ';
        arg[i + 1] = 0;
        mysnprintf(arg + i + 1, 1023 - i, url);
        if (i != 0) {
            pid = createSimpleProcess(arg);
            ret = waitForProcess(pid);
        }
    } 
    return ret;
}

static pid_t
createProcess(const char *cmdline, SWND wnd)
{
    pid_t pid;
    int fd[2];
    const char *argv[4];

    if (!cmdline || !wnd) return 0;

    argv[0] = "sh";
    argv[1] = "-c";
    argv[2] = cmdline;
    argv[3] = NULL;

    if (pipe(fd) != 0) return 0;

    pid = fork();
    switch(pid) {
      case -1:
        /* error */
        close(fd[0]);
        close(fd[1]);
        return 0;
      case 0:
        /* child process */
        if (fd[1] != STDERR_FILENO) {
            dup2(fd[1], STDERR_FILENO); /* pipe output is now stderr */
            close(fd[1]);  /* close old pipe fds */
        }
        close(fd[0]);
        execv("/bin/sh", (char * const *) argv);
        swCleanup();
        exit(1);  /* if we get this far, it's an error */
      default:
        /* parent process */
        /* add fd[0] to list of inputs, so we can watch the shell's output */
//        XtAppAddInput(TheAppContext, fd[0], (XtPointer) XtInputReadMask,
//                      childInput, (XtPointer) wnd);
/*      close(fd[1]); */
        break;
    }
    return pid;
}

static pid_t
createSimpleProcess(const char *cmdline)
{
    pid_t       pid;
    const char *argv[4];

    if (!cmdline) return 0;

    argv[0] = "sh";
    argv[1] = "-c";
    argv[2] = cmdline;
    argv[3] = NULL;
    pid = fork();
    switch(pid) {
      case -1:
        /* error */
        return 0;
      case 0:
        /* child process */
        execv("/bin/sh", (char * const *) argv);
        swCleanup();
        exit(1);  /* if we get this far, it's an error */
      default:
        /* parent process */
        break;
    }
    return pid;
}

static void
childInput(XtPointer closure , int *source, XtInputId *id)
{
    char buf[1024];
    int bytes;

    bytes = read(*source, buf, 1023);
    buf[bytes] = '\0';
    if (errorToConsole)
        mywritestr(console, buf);
    else
        swMessageBox((SWND) closure, buf, "Preview", SW_MB_OK, SW_MB_WARNING);
}

static int
waitForProcess(pid_t pid)
{
    int status;

    for (;;) {
        if (waitpid(pid, &status, 0) == -1) {
            if (errno != EINTR)
                return 0;
        } else {
            return WIFEXITED(status) && WEXITSTATUS(status) == 0;
        }
    }
}

void swRemoveFile(const char* filename)
{
    unlink(filename);
}

/*****************/
/* help browser  */
/*****************/

typedef struct SHelpBrowser {
    STABLE prefs;
    char *helpCommand;
    char *helpUrl;
    char *vrmlUrl;
    char *x3dUrl;
    pid_t pid;
} SHelpBrowser;

int helpBrowserRemote(SHBROWSER browser, const char *path, SWND wnd);

SHBROWSER
swHelpBrowserInit(STABLE prefs)
{
    char *dunedocs = NULL;
    SHelpBrowser *browser = malloc(sizeof(SHelpBrowser));
    browser->prefs = prefs;
    browser->helpCommand = mystrdup(swGetPreference(prefs,"HelpCommand",
                                                    WWW_BROWSER));
    dunedocs = getenv("DUNEDOCS");
    if (dunedocs != NULL) {
        char *url;
        url = (char *) malloc(strlen(dunedocs) + /* strlen(html) */ + 1);
        strcpy(url, dunedocs);
        browser->helpUrl = mystrdup(swGetPreference(prefs, "HelpURL", url));
        free(url);
    } else {
        char *url;
        const char *index = "index.html";
        const char *helpDir = HELP_URL;
        int helpDirLen;
        url = (char *) malloc(strlen(HELP_URL) + strlen(index) + 2);
        strcpy(url, HELP_URL);
        helpDirLen = strlen(helpDir);
        if (helpDirLen > 0)
            if (helpDir[helpDirLen - 1] != '/')
                strcat(url, "/");
        strcat(url, index);
        browser->helpUrl = mystrdup(swGetPreference(prefs, "HelpURL", url));
        free(url);
    }
    browser->vrmlUrl = mystrdup(swGetPreference(prefs, "HelpVrml",
                                                VRML_NODES_URL));
    browser->x3dUrl = mystrdup(swGetPreference(prefs, "HelpX3D",
                                               X3D_URL));

    browser->pid = 0;

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

    browser->pid = 0;
}

void
swHelpBrowserGetSettings(SHBROWSER browser, 
                         const char **helpCommand, 
                         const char **helpRemoteCommand, 
                         const char **helpUrl, 
                         const char **vrmlUrl, const char **x3dUrl,
                         const char **application, const char **topic)
{
    *helpCommand = browser->helpCommand;
    *helpUrl = browser->helpUrl;
    *vrmlUrl = browser->vrmlUrl;
    *x3dUrl = browser->x3dUrl;
    *application = NULL;
    *topic = NULL;
}

void
swHelpBrowserSetSettings(SHBROWSER browser, 
                         const char *helpCommand, 
                         const char *helpRemoteCommand, 
                         const char *helpUrl, 
                         const char *vrmlUrl, const char *x3dUrl,
                         const char *application, const char *topic)
{
    free(browser->helpCommand);
    browser->helpCommand = mystrdup(helpCommand);

    free(browser->helpUrl);
    browser->helpUrl = mystrdup(helpUrl);
    
    free(browser->vrmlUrl);
    browser->vrmlUrl = mystrdup(vrmlUrl);

    free(browser->x3dUrl);
    browser->x3dUrl = mystrdup(vrmlUrl);
    
    swSetPreference(browser->prefs, "HelpCommand", browser->helpCommand);
    swSetPreference(browser->prefs, "HelpURL", browser->helpUrl);
    swSetPreference(browser->prefs, "HelpVrml", browser->vrmlUrl);
    swSetPreference(browser->prefs, "HelpX3d", browser->x3dUrl);
}

void
swHelpBrowserHTML(SHBROWSER browser, SWND wnd)
{
    char* path=browser->helpUrl;
    
    helpBrowserRemote(browser, path, wnd);
}

char *swGetHelpUrl(SHBROWSER browser)
{
    return browser->helpUrl;
}

void
swHelpBrowserPath(SHBROWSER browser, const char *path, SWND wnd)
{
    helpBrowserRemote(browser, path, wnd);
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
    helpBrowserRemote(browser, path , wnd);
    free(path);
}

void
swHelpBrowserX3d(SHBROWSER browser, const char* string, SWND wnd)
{
    int pathLen=strlen(browser->x3dUrl);
    char *path=malloc(pathLen+strlen(string)+2);
    strcpy(path,browser->x3dUrl);
    strcat(path,string);
    helpBrowserRemote(browser, path , wnd);
    free(path);
}

void
swHelpBrowserShutdown(SHBROWSER browser)
{
    if (!browser) return;

    if (browser->pid != 0) {
        kill(browser->pid, SIGTERM);
        waitForProcess(browser->pid);
    }
    free(browser->helpCommand);
    free(browser->helpUrl);
    free(browser->vrmlUrl);
    free(browser->x3dUrl);
    free(browser);
}

int
helpBrowserRemote(SHBROWSER browser, const char *path, SWND wnd)
{
    char url[1024];
    char arg[1024];
    char command[1024];
    pid_t pid;
    int i;

    if (path[0] == '/')
        mysnprintf(url, 1023, "file://%s", path);
    else
        mysnprintf(url, 1023, path);
    mysnprintf(arg, 1023, browser->helpCommand, url);
    pid = createSimpleProcess(arg);
    /* if failed, search first string (till blank) as command */
    if (waitForProcess(pid)==0) {
        for (i=0;(i<strlen(browser->helpCommand)) && (i<1024-3);i++) {
            command[i]=browser->helpCommand[i];
            if ((browser->helpCommand[i]  !=' ') &&   
                (browser->helpCommand[i+1]==' ')) {
                i++;
                break;
            }
        }
        command[i]=0;
        strcat(command," %s");
        mysnprintf(arg, 1024, command, path);
        browser->pid = createProcess(arg, wnd);
    }
    if (browser->pid != 0) {
        return TRUE;
    } else {
        return FALSE;
    }   
}


/****************/
/* text editor */
/**************/

static int fdpipe_message[2];
static int fdpipe_close[2];

int 
swCheckRunningProcess(void)
{
    char message[2] = { 'l' , '\0' };
    /* check if there is something in the pipe (process ended) */
    int byteFromPipe = 0;
    if (read(fdpipe_message[0],message,1) == 1)
        byteFromPipe = 1;       
    if (byteFromPipe) {
        while (write(fdpipe_close[1], message, 1) == -1)
        /* 
           send notification to other process
           (otherwise data possibly would die with close of pipe)
        */
        close(fdpipe_close[1]);
        close(fdpipe_message[0]);

    } 
    return !byteFromPipe;
}

int
swCreateCheckableProcess(const char *cmdline)
{
    char message[2] = { 'l' , '\0' };
    pid_t       pid;

    if (pipe(fdpipe_message) != 0) return -1;
    
    if (pipe(fdpipe_close) != 0) {
        close(fdpipe_message[0]);
        close(fdpipe_message[1]);        
        return -1;
    }

    /* switch to non blocking io for pipe of first message */
    fcntl(fdpipe_message[0],F_SETFL,O_NONBLOCK);
    fcntl(fdpipe_message[1],F_SETFL,O_NONBLOCK);

    pid = fork();
    switch(pid) {
      case -1:
        /* error */
        close(fdpipe_message[0]);
        close(fdpipe_message[1]);
        close(fdpipe_close[0]);
        close(fdpipe_close[1]);
        return -1;
      case 0:
        /* child process */
        close(fdpipe_message[0]);
        close(fdpipe_close[1]);
        if (system(cmdline)!=0) {
            if (fprintf(stderr,"failed: %s\n",cmdline)==0) {}
        }    
        /* send message to pipe */
        while (write(fdpipe_message[1],message,1)==-1);
        /* wait for notification */
        while (read(fdpipe_close[0],message,1)==-1);
        close(fdpipe_message[1]);
        close(fdpipe_close[0]);
#ifdef EXIT_HANDLER
        _exit(0);   
#else
        exit(0);   
#endif             
      default:
        /* parent process */
        close(fdpipe_message[1]);
        close(fdpipe_close[0]);
        break;
    }
    return 0;
}

typedef struct STextedit {
    STABLE prefs;
    char  *textEditCommand;
    char  *textEditLinenumberOption;
    int   textEditUseExtensionTxt;
    char  *imageEditCommand;
    char  *imageEdit4KidsCommand;
    char  *soundEditCommand;
    char  *movieEditCommand;
} STextedit;


STEXTEDIT
swTextEditInit(STABLE prefs)
{
    char *editor = NULL;
    STextedit *textEdit = malloc(sizeof(STextedit));
    textEdit->prefs = prefs;

    if (getenv("WINEDITOR") != NULL) {
        textEdit->textEditCommand = mystrdup(swGetPreference(
              prefs, "TextEditCommand", getenv("WINEDITOR")));
        textEdit->textEditLinenumberOption = mystrdup(swGetPreference(
              prefs, "TextEditLinenumberOption", "+"));
    } else if (getenv("EDITOR") != NULL) {
        editor = getenv("EDITOR");
        textEdit->textEditLinenumberOption = mystrdup(swGetPreference(
              prefs, "TextEditLinenumberOption", "+"));
    } else {
#ifndef MACOSX
        editor = "vi";
        textEdit->textEditLinenumberOption = mystrdup(swGetPreference(
              prefs, "TextEditLinenumberOption", "+"));
#else
        /* 
           typical MacOS X users would die if they should use vi, 
           but the normal MacOS X textEditor: 
           "/usr/bin/open /Applications/TextEdit.app"
           is useless here, cause it can not wait for the completion of editing
         */ 
        textEdit->textEditCommand = mystrdup(swGetPreference(
              prefs, "TextEditCommand", "/usr/X11R6/bin/xedit"));
        textEdit->textEditLinenumberOption = mystrdup(swGetPreference(
              prefs, "TextEditLinenumberOption", ""));
#endif
    }
    if (editor != NULL) {
        const char *term = TERM;
        const char *executeoption = " -e ";
        char *command;
        command = (char *) malloc(strlen(term) + strlen(executeoption) +
                                  strlen(editor) + 1);
        strcpy(command, term);
        strcat(command, executeoption);
        strcat(command, editor);

        textEdit->textEditCommand = mystrdup(swGetPreference(prefs, 
              "TextEditCommand", command));
    }

    textEdit->textEditUseExtensionTxt = swGetIntPreference(
          prefs, "TextEditUseExtensionTxt", 1);
    textEdit->imageEditCommand = mystrdup(swGetPreference(
          prefs, "ImageEditCommand", HAVE_IMAGE_EDITOR));
    textEdit->imageEdit4KidsCommand = mystrdup(swGetPreference(
          prefs, "ImageEdit4KidsCommand", HAVE_IMAGE_EDITOR4KIDS));
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
#ifdef HAS_MLOCK
    mlock(upload->password, 1024);
#endif
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
    int f = -1;
    int i;
    int needPassword = 0;
    static char htmlTags[1024];
    char *htmlpath = (char *) malloc(1024);
    htmlpath[0] = 0;

    if ((upload->password != NULL) && (strlen(upload->password) != 0))
        if (strstr(upload->commandline, "%s")  !=  NULL)
            if (strstr(strstr(upload->commandline, "%s") + 2, "%s") !=  NULL)
                needPassword = 1; /* if commandline has two %s */
#ifdef HAS_MLOCK
    mlock(uploadCommand, 1024);
#endif
    if (needPassword)
        mysnprintf(uploadCommand, 1023, upload->commandline, upload->password,
                   fileToUpload);
    else
        mysnprintf(uploadCommand, 1023, upload->commandline, fileToUpload, "");

    if ((cmd = popen(uploadCommand, "w")) != NULL) {
        if ((upload->password != NULL) && (strlen(upload->password) != 0))
            fputs(upload->password, cmd);
        if (pclose(cmd) != 0) {
            swMessageBox(wnd, "upload failed", "Upload failed",
                         SW_MB_OK, SW_MB_WARNING);
            htmlpath[0] = 0;
            return htmlpath;
        }
    }
 
    swGetTempPath(htmlpath, ".dune_upload_reload", ".html", 1024);
    f = open(htmlpath, O_WRONLY | O_CREAT, 00666);
    if (f == -1) {
        swMessageBox(wnd, "open of html file failed", "Upload reload error", 
                     SW_MB_OK, SW_MB_WARNING);
        htmlpath[0] = 0;
        return htmlpath;
    }
    if ((upload->password != NULL) && (strlen(upload->password) != 0))
        mysnprintf(htmlTags, 1023, upload->htmlTag, upload->password);
    else
        mysnprintf(htmlTags, 1023, upload->htmlTag, "");
    if (write(f, htmlTags, sizeof(htmlTags)) != sizeof(htmlTags)) {
        swMessageBox(wnd, "open of html file failed", "Upload reload error", 
                     SW_MB_OK, SW_MB_WARNING);
        return htmlpath;
    }
    close(f);
    helpBrowserRemote(browser, htmlpath, wnd);
    for (i = 0; i < 1024; i++)
        uploadCommand[i] = (char) 0;
#ifdef HAS_MLOCK
    munlock(uploadCommand, 1024);    
#endif
    return htmlpath;
}

void
swUploadCleanupPasswd(SUPLOAD upload)
{
    int i;
    for (i = 0; i < strlen(upload->password); i++)
        upload->password[i] = (char) 0;
#ifdef HAS_MLOCK
    munlock(upload->password, 1024);
#endif
}
