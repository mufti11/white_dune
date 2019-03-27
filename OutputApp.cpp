/*
 * OutputApp.cpp
 *
 * Copyright (C) 2003 J. "MUFTI" Scheurich
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
#include <ctype.h>
#include <math.h>
#ifndef WIN32
# include <unistd.h>
# include <stdlib.h>
#endif

#include "stdafx.h"
#include "swt.h"

#include "Scene.h"
#include "DuneApp.h"
#include "WonderlandModuleExport.h"
#include "PreferencesApp.h"

#ifdef HAVE_CHECK_IN_COMMAND
# define CHECK_IN_COMMAND_ENABLED true
#else
# define CHECK_IN_COMMAND_ENABLED false
# define HAVE_CHECK_IN_COMMAND " "
#endif

#define DEFAULT_INDENT 2
#define DEFAULT_FLOAT_DIGITS 6

#define DEFAULT_AC3D_MATERIAL_NAME "window"
#define DEFAULT_CATT_MATERIAL_NAME "TOTREF"
#define DEFAULT_MATERIAL_NAME_GENERATION MATERIAL_NAME_FULL

#ifdef HAVE_XITE_PATH
# define DEFAULT_XITE_PATH HAVE_XITE_PATH
#else
# define DEFAULT_XITE_PATH "https://cdn.rawgit.com/create3000/x_ite/4.1.7/dist/"
#endif

#ifdef HAVE_X3DOM_PATH
# define DEFAULT_X3DOM_PATH HAVE_X3DOM_PATH
#else
# define DEFAULT_X3DOM_PATH "http://x3dom.org/download/dev/"
#endif

#define DEFAULT_X3DOM_PARAMETER "xmlns=\"http://www.web3d.org/specifications/x3d-namespace\"  showStat=\"false\" showLog=\"false\" x=\"0px\" y=\"0px\" width=\"512px\" height=\"512px\""

#define WONDERLAND_PREFIX "X3d"

#ifdef HAVE_ANT
# define DEFAULT_ANT_PATH HAVE_ANT 
#else
# define DEFAULT_ANT_PATH "ant"
#endif

#define DEFAULT_ANT_OPTIONS "-Xms256m -Xmx1024m"

#define DEFAULT_UNIX_ANT_COMMAND " -e sh -c \"if ! %s deploy ; then read PAUSE; fi\""
#define DEFAULT_WIN32_ANT_COMMAND "%s deploy || pause"

#define DEFAULT_DOWNLOAD_DIRECTORY "~/Downloads"

OutputApp::OutputApp()
{
    m_revisionControlCheckinCommand = NULL;
    m_downloadDirectory = NULL;
    m_defaultAc3dMaterialName = NULL;
    m_defaultCattMaterialName = NULL;

    m_cattExportPath = NULL;

    m_wonderlandModuleExport = NULL;
    m_wonderlandModuleExportPath = NULL;
    m_wonderlandModuleExportPrefix = NULL;
    m_writtenElementsPerJavaArray = -1;

    m_xitePath = NULL;

    m_x3domPath = NULL;
    m_x3domParameter = NULL;

    m_runAnt = false;
    m_antPath = NULL;
    m_antCommand = NULL;
    m_antOptions = NULL;

    m_ribTexturer = NULL;
    m_imageConverter = NULL;

    OutputSetDefaults();
}

void
OutputApp::OutputSetDefaults()
{
    m_keepURLs = false;
    m_krFormating = true;
    m_indent = DEFAULT_INDENT;
    SetFloatDigits(DEFAULT_FLOAT_DIGITS);
    m_oldFloatDigits = DEFAULT_FLOAT_DIGITS;
    m_useEFloatWriteFormat = true;
    m_normalsOnMeshCreation = true;
    m_skipMaterialNameBeforeFirstUnderscore = false;
    m_skipMaterialNameAfterLastUnderscore = false;

    if (m_defaultAc3dMaterialName != NULL)
        free(m_defaultAc3dMaterialName);
    m_defaultAc3dMaterialName = strdup(DEFAULT_AC3D_MATERIAL_NAME);
    m_ac3dExport4Raven = false;
    m_ac3dExportConvert2Gif = false;
    if (m_defaultCattMaterialName != NULL)
        free(m_defaultCattMaterialName);
    m_defaultCattMaterialName = strdup(DEFAULT_CATT_MATERIAL_NAME);

    m_revisionControlCheckinFlag = CHECK_IN_COMMAND_ENABLED;
    if (m_revisionControlCheckinCommand != NULL)
        free(m_revisionControlCheckinCommand);
    m_revisionControlCheckinCommand = strdup(HAVE_CHECK_IN_COMMAND);

    if (m_xitePath != NULL)
        free(m_xitePath);
    m_xitePath = strdup(DEFAULT_XITE_PATH);

    if (m_x3domPath != NULL)
        free(m_x3domPath);
    m_x3domPath = strdup(DEFAULT_X3DOM_PATH);
    if (m_x3domParameter != NULL)
        free(m_x3domParameter);
    m_x3domParameter = strdup(DEFAULT_X3DOM_PARAMETER);

    m_wonderlandModuleExportManyClasses = false;
    m_wonderlandModuleExportAllX3dClasses = false;
    m_runAnt = false;
    if (m_antPath != NULL)
        free(m_antPath);
    m_antPath = strdup(DEFAULT_ANT_PATH);
    if (m_antCommand != NULL)
        free(m_antCommand);
    MyString buf = "";
#ifdef WIN32
    buf += DEFAULT_WIN32_ANT_COMMAND;
#else
#ifdef HAVE_TERM
    buf += HAVE_TERM;
#else
    buf += "xterm";
#endif
    buf += DEFAULT_UNIX_ANT_COMMAND;
#endif
    m_antCommand = strdup(buf);
    m_defaultAntCommand = strdup(buf);
    if (m_antOptions != NULL)
        free(m_antOptions);
    m_antOptions = strdup(DEFAULT_ANT_OPTIONS);
    if (m_downloadDirectory != NULL)
        free(m_downloadDirectory);
    m_downloadDirectory = strdup(DEFAULT_DOWNLOAD_DIRECTORY);
    
#ifdef HAVE_RIB_TEXTURER
    m_ribTexturer = strdup(HAVE_RIB_TEXTURER);
#else
    m_ribTexturer = (char *)"";
#endif

#ifdef HAVE_IMAGE_CONVERTER
    m_imageConverter = strdup(HAVE_IMAGE_CONVERTER);
#else
    m_imageConverter = "";
#endif


}

void
OutputApp::OutputLoadPreferences()
{
    assert(TheApp != NULL);

    OutputSetDefaults();

    m_keepURLs = TheApp->GetBoolPreference("KeepURLs", false);    
    m_indent = TheApp->GetIntPreference("Indent", DEFAULT_INDENT); 
    SetFloatDigits(TheApp->GetIntPreference("FloatDigits", 
                                            DEFAULT_FLOAT_DIGITS));

    m_krFormating = TheApp->GetBoolPreference("KrFormating", true);    
    m_includeProtos = TheApp->GetBoolPreference("IncludeProtos", false);    
    m_compress = TheApp->GetBoolPreference("Compress", false);    
    m_normalsOnMeshCreation = TheApp->GetBoolPreference(
                                     "NormalsOnMeshCreation", true);    
    m_skipMaterialNameBeforeFirstUnderscore = TheApp->GetBoolPreference(
          "SkipMaterialNameBeforeFirstUnderscore", false);
    m_skipMaterialNameAfterLastUnderscore = TheApp->GetBoolPreference(
          "SkipMaterialNameAfterLastUnderscore", false);

    if (m_downloadDirectory != NULL)
        free(m_downloadDirectory);
     m_downloadDirectory = strdup(TheApp->GetPreference(
                                  "DownloadDirectory",
                                  DEFAULT_DOWNLOAD_DIRECTORY));
    TheApp->setDownloadDirectory(m_downloadDirectory);

    if (m_defaultAc3dMaterialName != NULL)
        free(m_defaultAc3dMaterialName);
    m_defaultAc3dMaterialName = strdup(TheApp->GetPreference(
                                       "DefaultAc3dMaterialName",
                                       DEFAULT_AC3D_MATERIAL_NAME));
    m_ac3dExport4Raven = TheApp->GetBoolPreference("Ac3dExport4Raven", false);
#ifdef HAVE_IMAGE_CONVERTER
    m_ac3dExportConvert2Gif = TheApp->GetBoolPreference("Ac3dExportConvert2Gif",
                                                        true);
#else
    m_ac3dExportConvert2Gif = false;
#endif
    if (m_defaultCattMaterialName != NULL)
        free(m_defaultCattMaterialName);
    m_defaultCattMaterialName = strdup(TheApp->GetPreference(
                                       "DefaultCattMaterialName",
                                       DEFAULT_CATT_MATERIAL_NAME));
    m_cattExportSrcRec = TheApp->GetBoolPreference("CattExportSrcRec", true);    

    m_revisionControlCheckinFlag = TheApp->GetBoolPreference(
                                   "UseRevisionControl",
                                   CHECK_IN_COMMAND_ENABLED); 
    if (m_revisionControlCheckinCommand != NULL)
        free(m_revisionControlCheckinCommand);
    m_revisionControlCheckinCommand = strdup(TheApp->GetPreference(
                                             "RevisionControlCheckInCommand",
                                             HAVE_CHECK_IN_COMMAND));

    if (m_xitePath != NULL)
        free(m_xitePath);
    m_xitePath = strdup(TheApp->GetPreference("XitePath",DEFAULT_XITE_PATH));

    if (m_x3domPath != NULL)
        free(m_x3domPath);
    m_x3domPath = strdup(TheApp->GetPreference("X3domPath",DEFAULT_X3DOM_PATH));
    if (m_x3domParameter != NULL)
        free(m_x3domParameter);
    m_x3domParameter = strdup(TheApp->GetPreference("X3domParameter",
                                                    DEFAULT_X3DOM_PARAMETER));

    if (m_wonderlandModuleExportPath != NULL)
        free(m_wonderlandModuleExportPath);
    m_wonderlandModuleExportPath = strdup(TheApp->GetPreference(
                                          "WonderlandModuleExportPath", ""));
    if  (m_wonderlandModuleExportPrefix != NULL)
        free(m_wonderlandModuleExportPrefix);
    m_wonderlandModuleExportPrefix = strdup(TheApp->GetPreference(
                                            "WonderlandModuleExportPrefix",
                                            WONDERLAND_PREFIX));
    m_wonderlandModuleExportManyClasses = TheApp->GetBoolPreference(
                                          "WonderlandModuleExportManyClasses",
                                          false);
    m_wonderlandModuleExportAllX3dClasses = TheApp->GetBoolPreference(
          "WonderlandModuleExportAllX3dClasses", false);
    m_runAnt = TheApp->GetBoolPreference("RunAnt", false);
    m_antPath = strdup(TheApp->GetPreference("AntPath", DEFAULT_ANT_PATH));
    m_antCommand = strdup(TheApp->GetPreference("AntCommand", 
                                                m_defaultAntCommand));
    m_antOptions = strdup(TheApp->GetPreference("AntOptions", 
                                                DEFAULT_ANT_OPTIONS));

    TheApp->updatePrefix();
    if (m_cattExportPath != NULL)
        free(m_cattExportPath);
    m_cattExportPath = strdup(TheApp->GetPreference("CattExportPath", ""));

#ifndef _WIN32
    if (m_ribTexturer != NULL)
        if (strlen(m_ribTexturer) != 0)
            free(m_ribTexturer);
#endif

#ifdef HAVE_RIB_TEXTURER
    m_ribTexturer = strdup(TheApp->GetPreference("RibTexturer", 
                                                 HAVE_RIB_TEXTURER));
#else
    m_ribTexturer = strdup(TheApp->GetPreference("RibTexturer", ""));
#endif

#ifndef _WIN32
    if (m_imageConverter != NULL)
        if (strlen(m_imageConverter) != 0)
            free(m_imageConverter);
#endif

#ifdef HAVE_IMAGE_CONVERTER
    m_imageConverter = strdup(TheApp->GetPreference("ImageConverter", 
                                                    HAVE_IMAGE_CONVERTER));
#else
    m_imageConverter = strdup(TheApp->GetPreference("ImageConverter", ""));
#endif
}

void
OutputApp::OutputSavePreferences()
{
    assert(TheApp != NULL);
 
    TheApp->SetBoolPreference("KeepURLs", m_keepURLs);
    TheApp->SetIntPreference("Indent", m_indent);
    TheApp->SetIntPreference("FloatDigits", m_floatDigits); 
    TheApp->SetBoolPreference("KrFormating", m_krFormating);
    TheApp->SetBoolPreference("IncludeProtos", m_includeProtos);
    TheApp->SetBoolPreference("Compress", m_compress);
    TheApp->SetBoolPreference("NormalsOnMeshCreation", 
                              m_normalsOnMeshCreation);
    TheApp->SetPreference("DownloadDirectory", 
                          TheApp->getDownloadDirectory());

    TheApp->SetPreference("DefaultAc3dMaterialName",
                          m_defaultAc3dMaterialName);
    TheApp->SetBoolPreference("Ac3dExport4Raven",
                              m_ac3dExport4Raven);
    TheApp->SetBoolPreference("Ac3dExportConvert2Gif",
                              m_ac3dExportConvert2Gif);
    TheApp->SetBoolPreference("SkipMaterialNameBeforeFirstUnderscore",
                              m_skipMaterialNameBeforeFirstUnderscore);
    TheApp->SetBoolPreference("SkipMaterialNameAfterLastUnderscore",
                              m_skipMaterialNameAfterLastUnderscore);
    TheApp->SetPreference("DefaultCattMaterialName",
                          m_defaultCattMaterialName);
    TheApp->SetBoolPreference("CattExportSrcRec", m_cattExportSrcRec);
    TheApp->SetPreference("CattExportPath", m_cattExportPath);

    TheApp->SetBoolPreference("UseRevisionControl", m_revisionControlCheckinFlag);
    TheApp->SetPreference("RevisionControlCheckInCommand", 
                          m_revisionControlCheckinCommand);

    TheApp->SetPreference("XitePath", m_xitePath);

    TheApp->SetPreference("X3domPath", m_x3domPath);
    TheApp->SetPreference("X3domParameter", m_x3domParameter);

    TheApp->SetPreference("WonderlandModuleExportPath", 
                          m_wonderlandModuleExportPath);
    TheApp->SetPreference("WonderlandModuleExportPrefix", 
                          m_wonderlandModuleExportPrefix);
    TheApp->SetBoolPreference("WonderlandModuleExportManyClasses", 
                              m_wonderlandModuleExportManyClasses);
    TheApp->SetBoolPreference("WonderlandModuleExportAllX3dClasses", 
                              m_wonderlandModuleExportAllX3dClasses);
    TheApp->SetBoolPreference("RunAnt", m_runAnt);
    TheApp->SetPreference("AntPath", m_antPath);
    TheApp->SetPreference("AntCommand", m_antCommand);
    TheApp->SetPreference("RibTexturer", m_ribTexturer);
    TheApp->SetPreference("ImageConverter", m_imageConverter);
}
 
bool parseCommandlineArgumentOutput(int & i,int argc, char** argv)
{
    bool found = true;
    if (strcmp(argv[i],"-krFormating")==0) {
        TheApp->SetkrFormating(true);
    } else if (strcmp(argv[i],"-indent")==0) {
        int indent;
        if (i++>=argc) return found;
        if (sscanf(argv[i],"%d",&indent)==1)
            TheApp->SetIndent(indent);
    } else if (strcmp(argv[i],"-floatDigits")==0) {
        int floatDigits;
        if (i++>=argc) return found;
        if (sscanf(argv[i],"%d",&floatDigits)==1)
            TheApp->SetFloatDigits(floatDigits);
    } else if (strcmp(argv[i],"-noEFloatWriteFormat")==0) {
        TheApp->forbidEFloatWriteFormat();
    } else
        return false;
    return found;
}


void
OutputApp::SetFloatDigits(int value)
{
    set_number_of_digits(value);
    m_floatDigits = value;
}

void
OutputApp::forbidEFloatWriteFormat()
{
    m_useEFloatWriteFormat = false;
}

void
OutputApp::enableEFloatWriteFormat(int writeFlags)
{
    if (m_useEFloatWriteFormat)
        useEFloatWriteFormat(1);
    if (writeFlags & X3D_4_WONDERLAND)
        useEFloatWriteFormat(1);        
}

void
OutputApp::disableEFloatWriteFormat(int writeFlags)
{
    if (m_useEFloatWriteFormat)
        useEFloatWriteFormat(0);
    if (writeFlags & X3D_4_WONDERLAND)
        useEFloatWriteFormat(0);        
}

void
OutputApp::useEFloatWriteFormat(int flag)
{ 
    if (!m_useEFloatWriteFormat)
        return;
    if (flag) {
        SetFloatDigits(m_oldFloatDigits);
        set_Efloat_format(0);
    } else {
        m_oldFloatDigits = m_floatDigits;
        set_Efloat_format(1);
    }
}

void
OutputApp::SetCattExportPath(const char *path)
{
    MyString exportPath = "";
    exportPath += path;
    if (exportPath[exportPath.length() - 1] != swGetPathSeperator())
        if (exportPath[exportPath.length() - 1] != '/') { // for posix API
            exportPath += swGetPathSeperator();
        }
    if (m_cattExportPath != NULL)
        free(m_cattExportPath);
    m_cattExportPath = strdup(exportPath);
}

void
OutputApp::SetXitePath(const char *path)
{
    MyString xitePath = "";
    xitePath += path;
    if (xitePath[xitePath.length() - 1] != swGetPathSeperator())
        if (xitePath[xitePath.length() - 1] != '/') { // for posix API
            xitePath += swGetPathSeperator();
        }
    if (m_xitePath != NULL)
        free(m_xitePath);
    m_xitePath = strdup(xitePath);
}

void
OutputApp::SetX3domPath(const char *path)
{
    MyString x3domPath = "";
    x3domPath += path;
    if (x3domPath[x3domPath.length() - 1] != swGetPathSeperator())
        if (x3domPath[x3domPath.length() - 1] != '/') { // for posix API
            x3domPath += swGetPathSeperator();
        }
    if (m_x3domPath != NULL)
        free(m_x3domPath);
    m_x3domPath = strdup(x3domPath);
}

void
OutputApp::SetX3domParameter(const char *parameter)
{
    MyString x3domParameter = " ";
    x3domParameter += parameter;
    // should we check something here ?
    if (m_x3domParameter != NULL)
        free(m_x3domParameter);
    m_x3domParameter = strdup(x3domParameter);
}

int 
OutputApp::writeWonderlandModule(const char* directory, const char* name, 
                                 Scene* scene, bool manyClasses)
{
    m_wonderlandModuleExport = new WonderlandModuleExport();
    int ret = m_wonderlandModuleExport->write(directory, name, scene, 
                                              manyClasses);
    delete m_wonderlandModuleExport;
    m_wonderlandModuleExport = NULL;
    return ret;
}

int 
OutputApp::writeWonderlandModuleArtPath(int filedes, const char *fileName) 
{
    if (m_wonderlandModuleExport != NULL)
        return m_wonderlandModuleExport->writeArtPath(filedes, fileName);
    swDebugf("Internal error: tried to export to a WonderlandModule"); 
    swDebugf(" but function is not uninitialised\n");
    return -1;         
}

void                
OutputApp::SetWonderlandModuleExportPrefix(char *prefix)
{
    if (m_wonderlandModuleExportPrefix != NULL)
        free(m_wonderlandModuleExportPrefix);
    m_wonderlandModuleExportPrefix = strdup(prefix);
    TheApp->updatePrefix();
}

void                
OutputApp::SetWonderlandModuleExportPath(char *path)
{
    if (m_wonderlandModuleExportPath != NULL)
        free(m_wonderlandModuleExportPath);
    m_wonderlandModuleExportPath = strdup(path);
    if (m_wonderlandModuleExportPath != NULL) {
        bool chompSlash;
        do {
            chompSlash = false;
            int len = strlen(m_wonderlandModuleExportPath);
            if (len > 0)
                if (m_wonderlandModuleExportPath[len - 1] == '/') {
                    chompSlash = true;
                    m_wonderlandModuleExportPath[len - 1] = 0;
                }
         } while (chompSlash);
   }
}


void
OutputApp::runAnt(Scene *scene)
{
    swIconifyWindow(TheApp->mainWnd());
    swDisableTimers();
    char cwd[1024];
    if (getcwd(cwd, 1023) != NULL) {
        if (chdir(m_wonderlandModuleExportPath) == 0) 
            if (chdir(WONDERLAND_EXPORT_DIRECTORY) == 0) {
                MyString antEnv = "";
                antEnv += "ANT_OPTS=";
                antEnv += m_antOptions;
                const char *env = strdup(antEnv);
                putenv((char *)env);
                URL *fileUrl = new URL(scene->getURL());
                char *dirName = strdup(fileUrl->GetFileNameWithoutExtension());
                dirName[0] = tolower(dirName[0]);
                if (chdir(dirName) == 0) {
                   char buf[1024];
                   mysnprintf(buf, 1023, m_antCommand, m_antPath);
                   if (system(buf) != 0)
                      fprintf(stderr, "failed: %s\n", buf);
                }
            }
        while (chdir(cwd) == -2);    
    }
    swDeIconifyWindow(TheApp->mainWnd());
    swInvalidateWindow(TheApp->mainWnd());
    swShowWindow(TheApp->mainWnd());
    swEnableTimers();
}
