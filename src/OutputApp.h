/*
 * OutputApp.h
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

#pragma once

bool parseCommandlineArgumentOutput(int & i,int argc, char** argv);

enum {
    MATERIAL_NAME_BEFORE_LAST_UNDERSCORE = 1,
    MATERIAL_NAME_AFTER_FIRST_UNDERSCORE = 2
};

class WonderlandModuleExport;

class OutputApp {
public:
                        OutputApp();

    void                SetKeepURLs(bool flag) { m_keepURLs = flag; }
    bool                GetKeepURLs() const { return m_keepURLs; }        

    void                SetIndent(int value) {m_indent = value;}
    int                 GetIndent(void) {return m_indent;}

    void                SetFloatDigits(int value);
    int                 GetFloatDigits(void) {return m_floatDigits;}

    void                forbidEFloatWriteFormat();
    void                enableEFloatWriteFormat(int writeFlags);
    void                disableEFloatWriteFormat(int writeFlags);
    void                useEFloatWriteFormat(int flag);

    bool                GetIncludeProtos() const { return m_includeProtos; }
    void                SetIncludeProtos(bool flag) { m_includeProtos = flag; }

    bool                GetkrFormating() const { return m_krFormating; }
    void                SetkrFormating(bool flag) { m_krFormating = flag; }

    bool                GetCompress() const { return m_compress; }
    void                SetCompress(bool flag) { m_compress = flag; }

    bool                GetNormalsOnMeshCreation() const 
                           { return m_normalsOnMeshCreation; }        
    void                SetNormalsOnMeshCreation(bool flag) 
                           { m_normalsOnMeshCreation = flag; }

    int                 writeWonderlandModule(const char* directory, 
                                              const char* name, Scene* scene,
                                              bool manyClasses);
    bool                isWonderlandModuleExport(void) 
                           { return m_wonderlandModuleExport != NULL; }
    int                 writeWonderlandModuleArtPath(int filedes,  
                                                     const char* fileName);

    const char         *GetWonderlandModuleExportPath(void)
                           { return m_wonderlandModuleExportPath;}
    void                SetWonderlandModuleExportPath(char *path);

    const char         *GetWonderlandModuleExportPrefix(void)
                           { return m_wonderlandModuleExportPrefix;}
    void                SetWonderlandModuleExportPrefix(char *prefix);

    bool                GetWonderlandModuleExportManyClasses(void)
                           { return m_wonderlandModuleExportManyClasses; }
    void                SetWonderlandModuleExportManyClasses(bool flag)
                           { m_wonderlandModuleExportManyClasses = flag; }

    bool                GetWonderlandModuleExportAllX3dClasses(void)
                           { return m_wonderlandModuleExportAllX3dClasses; }
    void                SetWonderlandModuleExportAllX3dClasses(bool flag)
                           { m_wonderlandModuleExportAllX3dClasses = flag; }

    void                SetWrittenElementsPerJavaArray(int numberElements) 
                           { m_writtenElementsPerJavaArray = numberElements; }
    int                 GetWrittenElementsPerJavaArray(void)
                           { return m_writtenElementsPerJavaArray; }

    bool                GetCattExportSrcRec(void) { return m_cattExportSrcRec; }

    const char         *GetCattExportPath(void)
                           { return m_cattExportPath;}
    void                SetCattExportPath(const char *path);

    const char         *GetX3domPath(void)
                           { return m_x3domPath;}
    void                SetX3domPath(const char *path);

    const char         *GetX3domParameter(void)
                           { return m_x3domParameter; }
    void                SetX3domParameter(const char *parameter);    

    const char         *GetXitePath(void)
                           { return m_xitePath;}
    void                SetXitePath(const char *path);

    void                OutputSetDefaults();

    void                OutputLoadPreferences();
    void                OutputSavePreferences();

    bool                SkipMaterialNameBeforeFirstUnderscore(void)
                           { return m_skipMaterialNameBeforeFirstUnderscore; }
    bool                SetSkipMaterialNameBeforeFirstUnderscore(bool f)
                           { return m_skipMaterialNameBeforeFirstUnderscore = f;}
    bool                SkipMaterialNameAfterLastUnderscore(void)
                           { return m_skipMaterialNameAfterLastUnderscore; }
    bool                SetSkipMaterialNameAfterLastUnderscore(bool f)
                           { return m_skipMaterialNameAfterLastUnderscore = f;}

    bool                IsMaterialNameBeforeLastUnderscore(void)
                           { return m_materialNameGeneration & 
                                    MATERIAL_NAME_BEFORE_LAST_UNDERSCORE; }
    bool                IsMaterialNameAfterFirstUnderscore(void)
                           { return m_materialNameGeneration & 
                                    MATERIAL_NAME_AFTER_FIRST_UNDERSCORE; }

    bool                GetRevisionControlCheckinFlag(void) 
                           { return m_revisionControlCheckinFlag; }
    void                SetRevisionControlCheckinFlag(bool flag) 
                           { m_revisionControlCheckinFlag = flag; }

    const char         *GetRevisionControlCheckinCommand(void) 
                           { return m_revisionControlCheckinCommand; }
    void                SetRevisionControlCheckinCommand(const char* command) 
                           {
                           if (m_revisionControlCheckinCommand != NULL)
                              free(m_revisionControlCheckinCommand); 
                           m_revisionControlCheckinCommand = strdup(command); 
                           }

    const char          *GetDefaultAc3dMaterialName(void) 
                           { return m_defaultAc3dMaterialName; }
    void                 SetDefaultAc3dMaterialName(const char *name) 
                           { 
                           if (m_defaultAc3dMaterialName != NULL)
                              free(m_defaultAc3dMaterialName); 
                           m_defaultAc3dMaterialName = strdup(name); 
                           }

    bool                GetAc3dExport4Raven(void) { return m_ac3dExport4Raven; }
    void                SetAc3dExport4Raven(bool b) { m_ac3dExport4Raven = b; }

    bool                GetAc3dExportConvert2Gif(void) 
#ifdef HAVE_IMAGE_CONVERTER
                           { return m_ac3dExportConvert2Gif; }
#else
                           { return false; }
#endif
    void                SetAc3dExportConvert2Gif(bool b) 
                           { m_ac3dExportConvert2Gif = b; }

    char                *GetDefaultCattMaterialName(void) 
                           { return m_defaultCattMaterialName; }
    void                 SetDefaultCattMaterialName(const char *name) 
                           { 
                           if (m_defaultCattMaterialName != NULL)
                              free(m_defaultCattMaterialName); 
                           m_defaultCattMaterialName = strdup(name); 
                           }

    char                *GetAntPath(void) { return m_antPath; }
    void                 SetAntPath(const char *path)
                           {
                           if (m_antPath != NULL)
                               free(m_antPath);
                           m_antPath = strdup(path);
                           }   
    const char          *GetAntCommand(void) { return m_antCommand; }
    void                 SetAntCommand(const char *command)
                           {
                           if (m_antCommand != NULL)
                               free(m_antCommand);
                           m_antCommand = strdup(command);
                           }   
    const char          *GetAntOptions(void) { return m_antOptions; }
    void                 SetAntOptions(const char *opts)
                           {
                           if (m_antOptions != NULL)
                               free(m_antOptions);
                           m_antOptions = strdup(opts);
                           }   
    bool                 GetRunAnt(void) { return m_runAnt; }
    void                 SetRunAnt(bool flag) { m_runAnt = flag; }

    void                 runAnt(Scene *scene);
                         
    const char          *GetRibTexturer(void) { return m_ribTexturer; }
    void                 SetRibTexturer(const char *texturer)
                           {
                           if (m_ribTexturer != NULL)
                               free(m_ribTexturer);
                           m_ribTexturer = strdup(texturer);
                           }   

    const char          *GetImageConverter(void) { return m_imageConverter; }
    void                 SetImageConverter(const char *converter)
                           {
                           if (m_imageConverter != NULL)
                               free(m_imageConverter);
                           m_imageConverter = strdup(converter);
                           }   

private:
    bool                    m_keepURLs;
    int                     m_indent;
    int                     m_floatDigits;
    int                     m_oldFloatDigits;
    bool                    m_useEFloatWriteFormat;
    bool                    m_krFormating;
    bool                    m_includeProtos;
    bool                    m_compress;    
    bool                    m_normalsOnMeshCreation;
    bool                    m_skipMaterialNameBeforeFirstUnderscore;
    bool                    m_skipMaterialNameAfterLastUnderscore;
    int                     m_materialNameGeneration;
    char                   *m_downloadDirectory;
    char                   *m_defaultAc3dMaterialName;
    bool                    m_ac3dExport4Raven;
    bool                    m_ac3dExportConvert2Gif;
    char                   *m_defaultCattMaterialName;
    bool                    m_revisionControlCheckinFlag;
    char                   *m_revisionControlCheckinCommand;
    WonderlandModuleExport *m_wonderlandModuleExport;
    char                   *m_wonderlandModuleExportPath;
    char                   *m_wonderlandModuleExportPrefix;
    bool                    m_wonderlandModuleExportManyClasses;
    bool                    m_wonderlandModuleExportAllX3dClasses;
    int                     m_writtenElementsPerJavaArray;
    bool                    m_cattExportSrcRec;
    char                   *m_cattExportPath;
    char                   *m_xitePath;
    char                   *m_x3domPath;
    char                   *m_x3domParameter;
    char                   *m_antPath;
    char                   *m_antCommand;
    char                   *m_antOptions;
    char                   *m_defaultAntCommand;
    bool                    m_runAnt;
    char                   *m_ribTexturer;
    char                   *m_imageConverter;
};


