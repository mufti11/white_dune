/*
 * Commandline.cpp 
 *
 * Copyright (C) 1999 Stephen F. White, 2010-2015 J. "MUFTI" Scheurich
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

#include <stdio.h>
#include <string.h>
#include "config.h" 
#include "DuneApp.h" 
#include "InputDevice.h" 
#include "xerrorhandler.h" 
#include "MainWindow.h"
#include "Scene.h"
#include "IconSize.h"
#include "errno.h"

#include "CommandlineLanguages.h"

#define errorprintf fprintf

extern int illegal2vrml(int argc, char *argv[]);
//extern int vrml2dune(int argc, char *argv[]);

bool parseCommandlineArgumentInputDevice(int &i,int argc, char** argv)
{
    static int currentInputDevice = -1;
    static int currentAflockDevice = -1;
    bool found=true;
    if (0 == 1) {
#ifdef LINUX_JOYSTICK
# define HAS_INPUT_DEVICE 1
    } else if (strcmp(argv[i],"-joystick")==0) {
       if (++i >= argc) return found;
       currentInputDevice=TheApp->setInputDevice(new linux_joystick(argv[i]));
#endif
#ifdef WINDOWS_JOYSTICK
# define HAS_INPUT_DEVICE 1
    } else if (strcmp(argv[i],"-joystick")==0) {
       if (++i >= argc) return found;
       currentInputDevice=TheApp->setInputDevice(new windows_joystick(argv[i]));
#endif
#ifdef HAVE_SDL_JOYSTICK
# define HAS_INPUT_DEVICE 1
    } else if (strcmp(argv[i],"-SDLjoystick")==0) {
       if (++i >= argc) return found;
       currentInputDevice=TheApp->setInputDevice(new SDL_joystick(argv[i]));
#endif
#ifdef HAVE_LIBSBALL
# define HAS_INPUT_DEVICE 1
    } else if (strcmp(argv[i],"-spaceball")==0) {
       if (++i >= argc) return found;
       currentInputDevice=TheApp->setInputDevice(new spaceball(argv[i]));
#endif
#ifdef HAVE_WINDOWS_SPACEBALL
# define HAS_INPUT_DEVICE 1
    } else if (strcmp(argv[i],"-spaceball")==0) {
       if (++i >= argc) return found;
       currentInputDevice=TheApp->setInputDevice(new windowsspaceball(argv[i]));
#endif
#ifdef HAVE_WINDOWS_P5
# define HAS_INPUT_DEVICE 1
    } else if (strcmp(argv[i],"-p5")==0) {
       if (++i >= argc) return found;
       currentInputDevice=TheApp->setInputDevice(new windowsp5(argv[i]));
#endif
#ifdef HAVE_NXT_DIALS
# define HAS_INPUT_DEVICE 1
    } else if (strcmp(argv[i],"-nxtdials")==0) {
       if (++i >= argc) return found;
       currentInputDevice=TheApp->setInputDevice(new nxtDials(argv[i]));
#endif
#ifdef HAVE_XINPUT
# define HAS_INPUT_DEVICE 1
    } else if (strcmp(argv[i],"-xinput")==0) {
       if (++i >= argc) return found;
       currentInputDevice=TheApp->setInputDevice(new xinput(argv[i]));
     } else if (strcmp(argv[i],"-xinputlist")==0) {
       swListXinputDevice(0);
       normalExit(0);
    } else if (strcmp(argv[i],"-xinputlistlong")==0) {
       swListXinputDevice(1);
       normalExit(0);
#endif
#ifdef HAVE_AFLOCK
# define HAS_INPUT_DEVICE 1
    } else if (strcmp(argv[i],"-aflock")==0) {
       if (++i >= argc) return found;
       currentAflockDevice=TheApp->setAflockDevice(new AflockDevice(argv[i]));
    // zillions of flock of birds parameter
    } else if (strcmp(argv[i],"-baud")==0) {

       if (++i >= argc) return found;
       if (currentAflockDevice>-1)
          TheApp->getAflockDevice(currentAflockDevice)->setBaud(argv[i]);

    } else if (strcmp(argv[i],"-sync")==0) {

       if (++i >= argc) return found;
       if (currentAflockDevice>-1)
          TheApp->getAflockDevice(currentAflockDevice)->setSync(argv[i]);

    } else if (strcmp(argv[i],"-block")==0) {

       if (++i >= argc) return found;
       if (currentAflockDevice>-1)
          TheApp->getAflockDevice(currentAflockDevice)->setBlock(argv[i]);

    } else if (strcmp(argv[i],"-numbirds")==0) {

       if (++i >= argc) return found;
       if (currentAflockDevice>-1)
          TheApp->getAflockDevice(currentAflockDevice)->setNumBrds(argv[i]);


    } else if (strcmp(argv[i],"-master")==0) {

       if (++i >= argc) return found;
       if (currentAflockDevice>-1)
          TheApp->getAflockDevice(currentAflockDevice)->setTransmit(argv[i]);

    } else if (strcmp(argv[i],"-hemisphere")==0) {

       if (++i >= argc) return found;
       if (currentAflockDevice>-1)
          TheApp->getAflockDevice(currentAflockDevice)->setHemi(argv[i]);

    } else if (strcmp(argv[i],"-filter")==0) {

       if (++i >= argc) return found;
       if (currentAflockDevice>-1)
          TheApp->getAflockDevice(currentAflockDevice)->setFilt(argv[i]);

    } else if (strcmp(argv[i],"-suddenchangelock")==0) {

       if (++i >= argc) return found;
       if (currentAflockDevice>-1)
          TheApp->getAflockDevice(currentAflockDevice)->
                setSuddenChangeLock(argv[i]);

    } else if (strcmp(argv[i],"-report")==0) {

       if (++i >= argc) return found;
       if (currentAflockDevice>-1)
          TheApp->getAflockDevice(currentAflockDevice)->
                  setReport(argv[i]);
    } else if (strcmp(argv[i],"-calfile")==0) {

       if (++i >= argc) return found;
       if (currentAflockDevice>-1)
          TheApp->getAflockDevice(currentAflockDevice)->
                  setCalfile(argv[i]);

    } else if (strcmp(argv[i],"-wand")==0) {

       if (++i >= argc) return found;
       if (TheApp->getNumberAflockDevices()==0)
          errorprintf(stderr,"no -aflock option found\n"); 
       else
          currentInputDevice = TheApp->setInputDevice(new aflock(
                TheApp->getAflockDevice(currentAflockDevice),argv[i],false));

    } else if (strcmp(argv[i],"-tracker")==0) {

       if (++i >= argc) return found;
       if (TheApp->getNumberAflockDevices()==0)
          errorprintf(stderr,"no -aflock option found\n"); 
       else
          currentInputDevice = TheApp->setInputDevice(new aflock(
                TheApp->getAflockDevice(currentAflockDevice),argv[i],true));

    } else if (stringncmp(argv[i],"-headnavigation")==0) {
       if (currentInputDevice>-1)
          TheApp->getInputDevice(currentInputDevice)->setHeadNavigation();
    } else if (stringncmp(argv[i],"-ignoresize")==0) {
       if (++i >= argc) return found;
       if (currentAflockDevice==-1)
          errorprintf(stderr,"no -aflock option found\n"); 
       else
          TheApp->getAflockDevice(currentAflockDevice)->setIgnoreSize(argv[i]);
#endif
#ifdef HAS_INPUT_DEVICE 
    } else if ((stringncmp(argv[i],"-x")==0) ||
               (stringncmp(argv[i],"-y")==0) ||
               (stringncmp(argv[i],"-z")==0) ||
               (stringncmp(argv[i],"-all")==0) ||
               (stringncmp(argv[i],"-none")==0) ||
               (stringncmp(argv[i],"-axes")==0) ||
               (stringncmp(argv[i],"-sendalways")==0)) {
        if (currentInputDevice>-1)
            TheApp->setInputDeviceSetting(TheApp->getInputDevice(
                                                currentInputDevice), argv[i]);
    } else if (stringncmp(argv[i],"-dontcarefocus")==0) {
        TheApp->setDontCareFocus();
#endif
    } else
       found=false;
    if (currentInputDevice>-1)
        if (!TheApp->getInputDevice(currentInputDevice)->isValid())
            swDebugf("%s\n", TheApp->getInputDevice(currentInputDevice)->
                             getErrorMessage());

    return found;    
}

bool parseCommandlineArgument(int & i,int argc, char** argv)
{
    bool found = parseCommandlineArgumentStereoView(i, argc, argv);
    if (found)
       return true;
    found = parseCommandlineArgumentOutput(i, argc, argv);
    if (found)
       return true;
    else if (strcmp(argv[i],"-4kids")==0) {
       TheApp->set4Kids();
    } else if (strcmp(argv[i],"-4catt")==0) {
       TheApp->set4Catt();
    } else if (strcmp(argv[i],"-defaults")==0) {
       TheApp->setDefaults();
    } else if (strcmp(argv[i],"-full")==0) {
       TheApp->setFull();
    } else if (strcmp(argv[i],"-x3dv")==0) {
       TheApp->setX3dv();
    } else if (strcmp(argv[i],"-uninstall")==0) {
       TheApp->unInstall();
       exit(0);
    } else if (strcmp(argv[i],"-prefix")==0) {
       if (++i >= argc) return found;
       found=true;
       TheApp->setPrefix(argv[i]);
    } else if (strcmp(argv[i],"-proto")==0) {
       if (++i >= argc) return found;
       char *category = argv[i];
       if (++i >= argc) return found;          
       TheApp->addToProtoLibrary(category, argv[i]);
       TheApp->SavePreferences();
       normalExit(0);
    } else if (strcmp(argv[i],"-renderslower")==0) {
       TheApp->SetRenderFaster(false);
    } else if (strcmp(argv[i],"-fieldofview")==0) {
       if (++i >= argc) return found;
       float fov;
       if (sscanf(argv[i],"%f",&fov)==1)
          TheApp->setFixFieldOfView(fov);
    } else if (strcmp(argv[i],"-tessellation")==0) {
       if (++i >= argc) return found;
       int tessellation = 8;
       if (sscanf(argv[i],"%d",&tessellation)==1)
          if (tessellation > 2)
              TheApp->setTessellation(tessellation);
    } else if (stringncmp(argv[i],"-demo")==0) {
       if (++i >= argc) return found;
       int timeOut = 0;
       if (sscanf(argv[i],"%d",&timeOut)==1)
           TheApp->setDemoMode(timeOut);
    } else if (stringncmp(argv[i],"-stereo")==0) {
       // parse special commandline parameters
    } else if (stringncmp(argv[i],"-cover")==0) {
       TheApp->setCoverMode();
    } else if (stringncmp(argv[i],"-kambi")==0) {
       TheApp->setKambiMode();
    } else if (stringncmp(argv[i],"-x3dom")==0) {
       TheApp->setX3dv();
       TheApp->setX3domMode();
    } else if (stringncmp(argv[i],"-hidestandardtoolbar")==0) {
       TheApp->hideStandardToolbar();
    } else if (strcmp(argv[i],"-filedialogdir")==0) {
       if (++i >= argc) return found;
       char *filedialogdir = argv[i];
       struct stat buf;
       if ((stat(filedialogdir, &buf) == 0) && (buf.st_mode & S_IFDIR))
          TheApp->setFileDialogDir(filedialogdir);
#ifndef _WIN32
    } else if (stringncmp(argv[i],"-fn")==0) {
       // parse special UNIX fn commandline parameters
       if (++i >= argc) return found; // skip -fn argument
    } else if (stringncmp(argv[i],"-fp")==0) {
       // parse special UNIX fontpath commandline parameters
       if (++i >= argc) return found; // skip -fp argument
    } else if (stringncmp(argv[i],"-indirect")==0) {
       swSetInDirectRendering();
    } else if (stringncmp(argv[i],"-nogllist")==0) {
       TheApp->setDisableGlList();
#endif
#ifdef _WIN32
    } else if (stringncmp(argv[i],"-8bit")==0) {
       // parse special WIN32 commandline parameters
#endif
#ifdef MACOSX
    } else if (strcmp(argv[i],"-startX11aqua")==0) {
       // parse special MacOSX commandline parameters
    } else if (stringncmp(argv[i],"-psn_")==0) {
       // parse special MacOSX commandline parameters
    } else if (strcmp(argv[i],"-exitPid")==0) {
       int pid;
       if (++i >= argc) return found;
       pid = atoi(argv[i]);
       if (pid > 0)
           TheApp->setExitPid(pid);
#endif
    } else if (strcmp(argv[i],"-checkSimpleCyclicSceneGraph")==0) {
       TheApp->setCheckSimpleCyclicSceneGraph();
    } else if (strcmp(argv[i],"-scriptHeaderC")==0) {
       if (++i >= argc) return found;
       TheApp->setScriptHeaderC(argv[i]);
    } else if (strcmp(argv[i],"-scriptHeaderC++")==0) {
       if (++i >= argc) return found;
       TheApp->setScriptHeaderCpp(argv[i]);
    } else if (strcmp(argv[i],"-scriptHeaderJava")==0) {
       if (++i >= argc) return found;
       TheApp->setScriptHeaderJava(argv[i]);
    } else {
       return parseCommandlineArgumentInputDevice(i, argc, argv) || 
              parseCommandlineArgumentLanguages(i, argc, argv);
    }
    return true;    
}

static void checkNotEnoughArgumentsError(int argc, int currentArgc) 
{
    if (currentArgc >= argc) {
        swDebugf("not enough arguments for conversion\n");
        exit(1);
    }
}

void parseCommandlineUsage(
#ifdef _WIN32
                           HINSTANCE hInstance,
#endif
                           int argc, char** argv) 
{
    bool download = false;
    checkLanguageEnvironment();
    if (argc > 2) {
        if (strcmp(argv[1],"-illegal2vrml")==0) {
            int rc = illegal2vrml(argc, argv);
            normalExit(rc);
        }
        const char* url = "./export.wrl";
        int convert = 0;
        int convertionArgument = 1;
        if (convertionArgument >= argc)
            return;
        if (strcmp(argv[convertionArgument],"-download")==0) {
            download = true;
            TheApp->setVerbose(true);
        } else if (strcmp(argv[convertionArgument],"-prefix")==0) {
            checkNotEnoughArgumentsError(argc, convertionArgument++);
            TheApp->setPrefix(mystrdup(argv[convertionArgument]));
            checkNotEnoughArgumentsError(argc, convertionArgument++);
        }  
        if (convertionArgument >= argc)
            return;
        if (strcmp(argv[convertionArgument],
                          "-checkSimpleCyclicSceneGraph")==0) {
            checkNotEnoughArgumentsError(argc, convertionArgument++);
            TheApp->setCheckSimpleCyclicSceneGraph();
        }  
        if (convertionArgument >= argc)
            return;
        if (strcmp(argv[convertionArgument],"-scriptHeaderC")==0) {
            checkNotEnoughArgumentsError(argc, convertionArgument++);
            TheApp->setScriptHeaderC(argv[convertionArgument]);
            checkNotEnoughArgumentsError(argc, convertionArgument++);
        }  
        if (convertionArgument >= argc)
            return;
        if (strcmp(argv[convertionArgument],"-scriptHeaderC++")==0) {
            checkNotEnoughArgumentsError(argc, convertionArgument++);
            TheApp->setScriptHeaderCpp(argv[convertionArgument++]);
            checkNotEnoughArgumentsError(argc, convertionArgument++);
        }  
        if (convertionArgument >= argc)
            return;
        if (strcmp(argv[convertionArgument],"-scriptHeaderJava")==0) {
            checkNotEnoughArgumentsError(argc, convertionArgument++);
            TheApp->setScriptHeaderJava(argv[convertionArgument]);
            checkNotEnoughArgumentsError(argc, convertionArgument++);
        }  
        if (convertionArgument >= argc)
            return;
        if (strcmp(argv[convertionArgument],"-tessellation")==0) {
            if (convertionArgument++ >= argc) {
                swDebugf("integer argment missing for -tessellation\n");
                exit(1);
            }
            int tessellation = 8;
            if (sscanf(argv[convertionArgument], "%d", &tessellation)==1)
                if (tessellation > 2)
                    TheApp->setTessellation(tessellation);
            convertionArgument++;
        } else if (strcmp(argv[convertionArgument],"-floatdigits")==0) {
            if (++convertionArgument >= argc) return;
            int digits = 6;
            if (sscanf(argv[convertionArgument],"%d",&digits)==1)
                TheApp->SetFloatDigits(digits);
            convertionArgument++;
        }
        if (convertionArgument >= argc)
            return;
        char firstChar = argv[convertionArgument][0];
        if ((firstChar == '-') || (firstChar == '+')) {
            if (strcmp(argv[convertionArgument],"-vrml97")==0)
                convert = PURE_VRML97;
            else if (strcmp(argv[convertionArgument],"-vrml97levelx3dv")==0)
                convert = PURE_X3DV;
            else if (strcmp(argv[convertionArgument],"-x3dv")==0)
                convert = X3DV;
            else if (strcmp(argv[convertionArgument],"-x3d")==0)
                convert = X3D_XML;
            else if (strcmp(argv[convertionArgument],"-X3DOM")==0)
                convert = X3DOM | X3D_XML;
            else if (strcmp(argv[convertionArgument],"-x3d4wonderland")==0) {
                TheApp->forbidEFloatWriteFormat();
                convert = X3D_XML | X3D_4_WONDERLAND;
            } else if (strcmp(argv[convertionArgument],"-kanim")==0) {
                convert = KANIM;
                checkNotEnoughArgumentsError(argc, ++convertionArgument);
                url = argv[convertionArgument];

            } else if (strcmp(argv[convertionArgument]+1,"c")==0)
                convert = C_SOURCE;
            else if (strcmp(argv[convertionArgument]+1,"3c")==0)
                convert = C_SOURCE | TRIANGULATE;

            else if (strcmp(argv[convertionArgument]+1,"c++")==0)
                convert = CC_SOURCE;
            else if (strcmp(argv[convertionArgument]+1,"3c++")==0)
                convert = CC_SOURCE | TRIANGULATE;

            else if (strcmp(argv[convertionArgument]+1,"java")==0) {
                convert |= JAVA_SOURCE;
                convert |= MANY_JAVA_CLASSES; // default
            } else if (strcmp(argv[convertionArgument]+1,"3java")==0) {
                convert |= JAVA_SOURCE | TRIANGULATE;
                convert |= MANY_JAVA_CLASSES; // default
            }
        } 
        bool tempSave = true;
        if (strcmp(argv[convertionArgument], "-o") == 0) {
             checkNotEnoughArgumentsError(argc, convertionArgument++);
             url = mystrdup(argv[convertionArgument++]);
             tempSave = false;
        }
        if (strcmp(argv[convertionArgument],"-files") == 0) {
            checkNotEnoughArgumentsError(argc, convertionArgument++);
            int files = atoi(argv[convertionArgument++]);
            if (files < 1) {
                fprintf(stderr, "-files argument must be a integer > 0\n");
                exit(1);
            } else
                TheApp->setNumExportFiles(files);
        }
        if (strcmp(argv[convertionArgument],"-wonderland")==0) {
            convert |= WONDERLAND | TRIANGULATE;
            convert |= MANY_JAVA_CLASSES; // default
            checkNotEnoughArgumentsError(argc, ++convertionArgument);
            url = argv[convertionArgument];
            if (strlen(TheApp->getCPrefix()) == 0)
                TheApp->setPrefix("X3d");
        } else if ((strcmp(argv[convertionArgument],"-ac3d")==0) || 
                   (strcmp(argv[convertionArgument],"-rib")==0) || 
                   (strcmp(argv[convertionArgument],"-ldraw")==0) || 
                   (convert & TRIANGULATE)) {
# ifdef _WIN32
            swInitialize(hInstance, 0, 0);
            TheApp->OnFileNewWindow();
            swOpenGLStartup(TheApp->getCurrentMainWindow()->getScene3DWindow());
# else
            swInitialize(&argc, argv, 0);
            TheApp->OnFileNewWindow();
# endif
#ifndef _WIN32
            SDC sdc = swCreateDC(TheApp->getCurrentMainWindow()->getScene3DWindow());
            swOSMakeCurrent(swCreateGLContext(sdc));            
#endif
            swHideWindow(TheApp->mainWnd());
            swIconifyWindow(TheApp->mainWnd());
        } 
        if (strcmp(argv[convertionArgument],"-ac3d")==0)
            convert = AC3D;
        else if (strcmp(argv[convertionArgument],"-rib")==0)
            convert = RIB;
        else if (strcmp(argv[convertionArgument],"-ldraw")==0)
            convert = LDRAW_DAT;
        if (convert != 0) {
            if (convertionArgument < (argc - 1)) {
                if (strcmp(argv[convertionArgument + 1],"-manyclasses")==0) {
                    convert |= MANY_JAVA_CLASSES;
                    convertionArgument++;
                }
                if ((convertionArgument < (argc - 2)) &&
                    (strcmp(argv[convertionArgument + 1],"-elementsInJavaArray")
                     == 0)) {
                    int elements = atoi(argv[convertionArgument + 2]);
                    if (elements > 0)
                        TheApp->SetWrittenElementsPerJavaArray(elements);
                    convertionArgument += 2;
                }
            }
            int inputFileArg = convertionArgument + 1;
            checkNotEnoughArgumentsError(argc, inputFileArg);
            if (convert == KANIM)
                checkNotEnoughArgumentsError(argc, inputFileArg++);
            if (strcmp(argv[inputFileArg], "-cover") == 0) {
                TheApp->setCoverMode();
                checkNotEnoughArgumentsError(argc, inputFileArg++);
            }
            Scene *scene = new Scene();
            if ((strcmp(argv[convertionArgument],"-ac3d")==0) || 
                (strcmp(argv[1],"-rib")==0) || 
                (strcmp(argv[1],"-ldraw")==0) || 
                (convert & TRIANGULATE)) {
                scene->drawScene();
            }
            scene->setExternProtoWarning(false);
            int rc = 1;
            checkNotEnoughArgumentsError(argc, inputFileArg);
            if (TheApp->AddFile(argv[inputFileArg], scene)) {
                rc = 0;
                if (convert == KANIM)
                    scene->getRoot()->preDraw();
                int fileDescriptor = 1;
                if (!tempSave && 
                    (!((convert == RIB) && (TheApp->getNumExportFiles() > 1))))
                    fileDescriptor = open(url, O_WRONLY | O_CREAT, 00666);
                if (fileDescriptor == -1) {
                    myperror(url);
                    exit(1);
                }
                rc = scene->write(fileDescriptor, url, 
                                  convert | SKIP_SAVED_TEST | 
                                  (tempSave ? TEMP_SAVE : 0)); 
                if (fileDescriptor != 1) {
                    if (rc != 0) {
                        myperror(url);
                        if (convert != RIB) // RIB files alread closed
                            if (swTruncateClose(fileDescriptor))
                                myperror(url);
                    }
                }
            } else if (errno != 0)
                myperror(argv[inputFileArg]);
            if (convert & TRIANGULATE)
                TheApp->printRenderErrors(true);
            delete TheApp;
            if (rc != 0) {
                if (TheApp->getVrml1Error()) {
                    rc=97;
                    swDebugf("Unable to parser input file\n");   
                } else if (errno != 0)
                    myperror("write file");
            }
            normalExit(rc);
        }
        if (download) {
            int inputFileArg = convertionArgument + 1;
            checkNotEnoughArgumentsError(argc, inputFileArg);
            Scene *scene = new Scene();
            int rc = 1;
            checkNotEnoughArgumentsError(argc, inputFileArg);
            if (TheApp->AddFile(argv[inputFileArg], scene))
                rc = 0;
            delete TheApp;
            if (rc != 0) {
                if (TheApp->getVrml1Error()) {
                    rc=97;
                    swDebugf("Unable to parser input file\n");   
                }
            }
            normalExit(rc);
        }
        if (strcmp(argv[convertionArgument],"-catt8geo")==0) {
#ifdef _WIN32
            swInitialize(hInstance, 0, 0);
            TheApp->OnFileNewWindow();
            swOpenGLStartup(TheApp->getCurrentMainWindow()->getScene3DWindow());
#else
            swInitialize(&argc, argv, 0);
            TheApp->OnFileNewWindow();
#endif
            swIconifyWindow(TheApp->mainWnd());
            swUpdate();
            int outputDirArg = convertionArgument + 1;
            checkNotEnoughArgumentsError(argc, outputDirArg);
            if (strcmp(argv[outputDirArg],"-cover")==0) {
                TheApp->setCoverMode();
                outputDirArg++;
            }
            Scene *scene = new Scene();
            scene->setExternProtoWarning(false);
            int rc = 1;
            if (argc <= (outputDirArg + 1)) {
                swDebugf("Usage: %s", argv[0]);
                swDebugf(" -catt8geo ");
                swDebugf("outputdir_with_material.geo");
                swDebugf(" file.wrl\n");
                swUpdate();
#ifdef _WIN32
                system("pause");
#endif
                normalExit(1);
            }
            if (TheApp->AddFile(argv[outputDirArg + 1], scene)) {
                rc = 0;
                MyString outputDir = ""; 
                outputDir += argv[outputDirArg];
                outputDir += swGetPathSeperator();
                MyString materialGeoFile = strdup(outputDir);
                materialGeoFile += "material.geo";
                struct stat fileStat;
                if (stat(materialGeoFile, &fileStat) == -1) {
                    swDebugf("Warning: file %s missing\n",
                             (const char *) materialGeoFile);
                    swUpdate();
#ifdef _WIN32
                    system("pause");
#endif
                }
                TheApp->SetCattExportPath(outputDir);
                if (scene->writeCattGeo() != 0) {
                    swDebugf("conversion to Catt8 failed");
                    swUpdate();
#ifdef _WIN32
                    system("pause");
#endif
                }
            } else {
                swDebugf("problem to open file: %s", argv[outputDirArg + 1]);
                myperror(argv[outputDirArg + 1]);
                swUpdate();
#ifdef _WIN32
                system("pause");
#endif
            }
            delete TheApp;
            if (rc != 0)
                if (TheApp->getVrml1Error())
                    rc=97;
            normalExit(rc);
        }
//        if (strcmp(argv[1],"-vrml2dune")==0)
//            vrml2dune(argc, argv);
    }
    for (int i = 0; i < argc; i++) {
#ifdef HAVE_DUNE_VERSION
        if (stringncmp(argv[i],"--version")==0) {
            printf("%s\n", HAVE_DUNE_VERSION);
            normalExit(0);
        }
#endif
        if (strcmp(argv[i],"--copyrightdetails")==0) {
            printf("License for most parts of this program:\n");
            printf("\n");
            printf("Copyright (C) 1999-2015 Stephen F. White and others\n");
            printf("\n");
            printf(" This program is free software; you can redistribute it and/or modify\n");
            printf(" it under the terms of the GNU General Public License as published by\n");
            printf(" the Free Software Foundation; either version 2 of the License, or\n");
            printf(" (at your option) any later version.\n");
            printf("\n");
            printf(" This program is distributed in the hope that it will be useful,\n");
            printf(" but WITHOUT ANY WARRANTY; without even the implied warranty of\n");
            printf(" MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\n");
            printf(" GNU General Public License for more details.\n");
            printf("\n");
            printf(" You should have received a copy of the GNU General Public License\n");
            printf(" along with this program (see the file \"COPYING\" for details); if \n");
            printf(" not, write to the Free Software Foundation, Inc., 51 Franklin St, Fifth Floor,\n");
            printf(" Boston, MA 02110-1301, USA.\n");
    
#ifdef HAVE_LIBJPEG
            printf("\n");
            printf("***********************************************************\n");
            printf("\n");

            printf("This software is based in part on the work of the Independent JPEG Group\n");
#endif
            printf("\n");
            printf("***********************************************************\n");
            printf("\n");
    
            printf("code of pngLoad.c based on a similar file in OpenVRML\n");
            printf("codebase of hull creation of NodeExtrusion::createMesh and supporting functions of OpenVRML\n");
            printf("CylinderSensor code in WonderlandModuleExport.cpp based on cylinder_sensor.cpp of OpenVRML\n");
            printf("SphereSensor code in WonderlandModuleExport.cpp based on sphere_sensor.cpp of OpenVRML\n");
            printf("\n");
            printf("Copyright (C) 1998  Chris Morley\n");
            printf("Copyright 2001, 2002, 2003, 2004, 2005, 2006, 2007  Braden McDaniel\n");
            printf("Copyright 2002  S. K. Bose\n");
            printf("\n");
            printf("patches to OpenVRML's NodeExtrusion::createMesh from the Orbisnap free vrml viewer");
            printf("Copyright 1998-2004 HUMUSOFT s.r.o.\n");
            printf("\n");
            printf(" This library is free software; you can redistribute it and/or\n");
            printf(" modify it under the terms of the GNU Lesser General Public\n");
            printf(" License as published by the Free Software Foundation; either\n");
            printf(" version 2.1 of the License, or (at your option) any later version.\n");
            printf("\n");
            printf(" This library is distributed in the hope that it will be useful,\n");
            printf(" but WITHOUT ANY WARRANTY; without even the implied warranty of\n");
            printf(" MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU\n");
            printf(" Lesser General Public License for more details.\n");
            printf("\n");
            printf(" You should have received a copy of the GNU Lesser General Public\n");
            printf(" License along with this library; if not, write to the Free Software\n");
            printf(" Foundation, Inc., 51 Franklin St, Fifth Floor, Boston MA 02110-1301, USA.\n");
     
            printf("\n");
            printf("***********************************************************\n");
            printf("\n");
     
            printf("code of generateTextureCoordinates() in Mesh.cpp \n");
            printf("based on Polyrep.c of FreeWRL\n");
            printf("setColor(), drawQuad(), drawSky() and drawGround() in NodeBackground.cpp\n");
            printf("based on render_Background() in Bindable.c of FreeWRL\n");
            printf("The ImageTexture3D reading code is also based on FreeWRL\n");
            printf("\n");
            printf(" Copyright (C) 1998 Tuomas J. Lukka\n");
            printf(" Copyright (C) 2002 John Stewart, CRC Canada.\n");
            printf(" DISTRIBUTED WITH NO WARRANTY, EXPRESS OR IMPLIED.\n");
            printf(" See the GNU Library General Public License (file COPYING in the distribution)\n");
            printf(" for conditions of use and redistribution.\n");
    
            printf("\n");
            printf("***********************************************************\n");
            printf("\n");
    
            printf("code of Aflock.h/cpp based on similar files in VR Juggler\n");
            printf("\n");
            printf("VR Juggler is (C) Copyright 1998, 1999, 2000 by Iowa State University\n");
            printf("\n");
            printf("Original Authors:\n");
            printf("  Allen Bierbaum, Christopher Just,\n");
            printf("  Patrick Hartling, Kevin Meinert,\n");
            printf("  Carolina Cruz-Neira, Albert Baker\n");
            printf("\n");
            printf(" This library is free software; you can redistribute it and/or\n");
            printf(" modify it under the terms of the GNU Library General Public\n");
            printf(" License as published by the Free Software Foundation; either\n");
            printf(" version 2 of the License, or (at your option) any later version.\n");
            printf("\n");
            printf(" This library is distributed in the hope that it will be useful,\n");
            printf(" but WITHOUT ANY WARRANTY; without even the implied warranty of\n");
            printf(" MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU\n");
            printf(" Library General Public License for more details.\n");
            printf("\n");
            printf(" You should have received a copy of the GNU Library General Public\n");
            printf(" License along with this library; if not, write to the\n");
            printf(" Free Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston,\n");
            printf(" MA 02110-1301, USA.\n");
     
            printf("\n");
            printf("***********************************************************\n");
            printf("\n");
     
            printf("code of createMesh() in NodeExtrusion.cpp\n");
            printf("    based on ExtrusionNode.cpp of \"CyberX3D for C++\"\n");
            printf("code of VectorNormalize(), VectorGetDirection(), VectorGetCross() and\n");
            printf("    GetNormalFromVertices in NodeExtrusion.cpp based on \n");
            printf("    MathUtill.cpp of \"CyberX3D for C++\"\n");
            printf("\n");
            printf("  Copyright (C) 2002-2003 Satoshi Konno\n");
            printf("  All rights reserved.\n");
            printf("\n");
            printf("  Redistribution and use in source and binary forms, with or without \n");
            printf("  modification, are permitted provided that the following conditions are met:\n");
            printf("\n");
            printf("  1. Redistributions of source code must retain the above copyright notice, \n");
            printf("     this list of conditions and the following disclaimer. \n");
            printf("\n");
            printf("  2. Redistributions in binary form must reproduce the above copyright notice, \n");
            printf("     this list of conditions and the following disclaimer in the documentation \n");
            printf("     and/or other materials provided with the distribution. \n");
            printf("\n");
            printf("  3. The name of the author may not be used to endorse or promote products \n");
            printf("     derived from this software without specific prior written permission. \n");
            printf("\n");
            printf("  THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED\n");
            printf("  WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF \n");
            printf("  MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO \n");
            printf("  EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, \n");
            printf("  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,\n");
            printf("  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; \n");
            printf("  OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, \n");
            printf("  WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR \n");
            printf("  OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF \n");
            printf("  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.\n");
     
            printf("\n");
            printf("***********************************************************\n");
            printf("\n");
     
            printf("code of EulerAngle.cpp/EulerAngle.h/unmatrix.cpp/unmatrix.h/GraphicsGems.h\n");
            printf("http://www1.acm.org/pubs/tog/GraphicsGems/gemsiv/euler_angle/\n");
            printf("\n");
            printf(" from http://www1.acm.org/pubs/tog/GraphicsGems/index.html\n");
            printf("\n");
            printf(" This is the official on-line repository for the code from the \n");
            printf(" Graphics Gems series of books (from Academic Press). This series\n");
            printf(" focusses on short to medium length pieces of code which perform a \n");
            printf(" wide variety of computer graphics related tasks. All code\n");
            printf(" here can be used without restrictions. The code distributions here \n");
            printf(" contain all known bug fixes and enhancements. We also\n");
            printf(" provide errata listings for the text of each book. Please report any \n");
            printf(" new errata or bugs to Eric Haines (erich@acm.org).\n");
     
            printf("\n");
            printf("***********************************************************\n");
            printf("\n");
     
            printf("code of mysnprintf.h/mysnprintf.c\n");   
            printf("\n");
     
            printf("/*\n");
            printf(" * Copyright Patrick Powell 1995\n");
            printf(" * This code is based on code written by Patrick Powell (papowell@astart.com)\n");
            printf(" * It may be used for any purpose as long as this notice remains intact\n");
            printf(" * on all source code distributions\n");
            printf(" */\n");
     
            printf("\n");
            printf("***********************************************************\n");
            printf("\n");
     
            printf("code of gif.c/gif.h/kljcpyrght.h\n");   
            printf("\n");
            printf("/****\n");
            printf(" * adapted from code by kirk johnson (tuna@athena.mit.edu).  most of this\n");
            printf(" * code is unchanged. -- jim frost 12.31.89\n");
            printf(" * Copyright 1989 Kirk L. Johnson (see the included file\n");
            printf(" * \"kljcpyrght.h\" for complete copyright information)\n");
            printf(" *\n");
            printf("  Copyright 1989, 1990 Kirk L. Johnson\n");
            printf("\n");
            printf("  Permission to use, copy, modify, distribute, and sell this\n");
            printf("  software and its documentation for any purpose is hereby granted\n");
            printf("  without fee, provided that the above copyright notice appear in\n");
            printf("  all copies and that both that copyright notice and this\n");
            printf("  permission notice appear in supporting documentation. The\n");
            printf("  author makes no representations about the suitability of this\n");
            printf("  software for any purpose. It is provided \"as is\" without express\n");
            printf("  or implied warranty.\n");
            printf("\n");
            printf("  THE AUTHOR DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE,\n");
            printf("  INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS,\n");
            printf("  IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, INDIRECT\n");
            printf("  OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM\n");
            printf("  LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT,\n");
            printf("  NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN\n");
            printf("  CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.\n");
            printf("****/\n");

            printf("\n");
            printf("***********************************************************\n");
            printf("\n");
     
            printf("code of GpsMathLib.cpp/gpsdatum.h/gpsmath.h/gpsport.h\n");   
            printf("\n");
            printf("/* @source JEEPS functions\n");
            printf("**\n");
            printf("** @author Copyright (C) 1999 Alan Bleasby\n");
            printf("** @version 1.0 \n");
            printf("** @modified Feb 04 2000 Alan Bleasby. First version\n");
            printf("** @@\n");
            printf("** \n");
            printf("** This library is free software; you can redistribute it and/or\n");
            printf("** modify it under the terms of the GNU Library General Public\n");
            printf("** License as published by the Free Software Foundation; either\n");
            printf("** version 2 of the License, or (at your option) any later version.\n");
            printf("** \n");
            printf("** This library is distributed in the hope that it will be useful,\n");
            printf("** but WITHOUT ANY WARRANTY; without even the implied warranty of\n");
            printf("** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU\n");
            printf("** Library General Public License for more details.\n");
            printf("** \n");
            printf("** You should have received a copy of the GNU Library General Public\n");
            printf("** License along with this library; if not, write to the\n");
            printf("** Free Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston,\n");
            printf("** MA 02110-1301, USA.\n");
            printf("*/\n");
            printf("//BOBMAX INSERT--------------------------------------------------------------------\n");
     
            printf("\n");
            printf("***********************************************************\n"); 
            printf("\n");
     
            printf("code of openglutfont.c/openglutfont.h\n");   
            printf("\n");
            printf("/*\n");
            printf(" * Portions copyright (C) 2004, the OpenGLUT project contributors.\n");
            printf(" * OpenGLUT branched from freeglut in February, 2004.\n");
            printf(" *\n");
            printf(" * Copyright (c) 1999-2000 Pawel W. Olszta. All Rights Reserved.\n");
            printf(" * Written by Pawel W. Olszta, <olszta@sourceforge.net>\n");
            printf(" * Creation date: Thu Dec 2 1999\n");
            printf(" *\n");
            printf(" * Permission is hereby granted, free of charge, to any person obtaining a\n");
            printf(" * copy of this software and associated documentation files (the \"Software\"),\n");
            printf(" * to deal in the Software without restriction, including without limitation\n");
            printf(" * the rights to use, copy, modify, merge, publish, distribute, sublicense,\n");
            printf(" * and/or sell copies of the Software, and to permit persons to whom the\n");
            printf(" * Software is furnished to do so, subject to the following conditions:\n");
            printf(" *\n");
            printf(" * The above copyright notice and this permission notice shall be included\n");
            printf(" * in all copies or substantial portions of the Software.\n");
            printf(" *\n");
            printf(" * THE SOFTWARE IS PROVIDED \"AS IS\", WITHOUT WARRANTY OF ANY KIND, EXPRESS\n");
            printf(" * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,\n");
            printf(" * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL\n");
            printf(" * PAWEL W. OLSZTA BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER\n");
            printf(" * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN\n");
            printf(" * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.\n");
            printf(" */\n");
            printf("\n");

            printf("***********************************************************\n");
            printf("\n");

            printf(" code of Quaternion::Quaternion(Vec3f from, Vec3f to) based on quaternion.cpp\n");
            printf(" of libQGLViewer-2.2.4-1\n");
            printf(" code of Vec3x::orthogonalVec() based on vec.cpp of libQGLViewer-2.2.4-1\n");
            printf("\n");
            printf(" Copyright (C) 2002-2006 Gilles Debunne (Gilles.Debunne@imag.fr)\n");
            printf("\n");
            printf("This function is part of the QGLViewer library.\n");
            printf(" Version 2.2.4-1, released on December 12, 2006.\n");
            printf("\n");
            printf(" http://artis.imag.fr/Members/Gilles.Debunne/QGLViewer\n");
            printf("\n");
            printf(" libQGLViewer is free software; you can redistribute it and/or modify\n");
            printf(" it under the terms of the GNU General Public License as published by\n");
            printf(" the Free Software Foundation; either version 2 of the License, or\n");
            printf(" (at your option) any later version.\n");
            printf("\n");
            printf(" libQGLViewer is distributed in the hope that it will be useful,\n");
            printf(" but WITHOUT ANY WARRANTY; without even the implied warranty of\n");
            printf(" MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\n");
            printf(" GNU General Public License for more details.\n");
            printf("\n");
            printf(" You should have received a copy of the GNU General Public License\n");
            printf(" along with libQGLViewer; if not, write to the Free Software\n");
            printf(" Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA\n");

            printf("\n");
            printf("***********************************************************\n");
            printf("\n");

            printf("/* \n");
            printf(" * swSetFontPath() in swt/motif/swt.c based on\n");
            printf(" * $Xorg: xset.c,v 1.6 2001/02/09 02:05:58 xorgcvs Exp $\n");
            printf(" */\n");
            printf("\n");
            printf("/*\n");
            printf("\n");
            printf("Copyright 1985, 1998  The Open Group\n");
            printf("\n");
            printf("Permission to use, copy, modify, distribute, and sell this software and its\n");
            printf("documentation for any purpose is hereby granted without fee, provided that\n");
            printf("the above copyright notice appear in all copies and that both that\n");
            printf("copyright notice and this permission notice appear in supporting\n");
            printf("documentation.\n");
            printf("\n");
            printf("The above copyright notice and this permission notice shall be included in\n");
            printf("all copies or substantial portions of the Software.\n");
            printf("\n");
            printf("THE SOFTWARE IS PROVIDED \"AS IS\", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR\n");
            printf("IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,\n");
            printf("FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE\n");
            printf("OPEN GROUP BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN\n");
            printf("AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN\n");
            printf("CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.\n");
            printf("\n");
            printf("Except as contained in this notice, the name of The Open Group shall not be\n");
            printf("used in advertising or otherwise to promote the sale, use or other dealings\n");
            printf("in this Software without prior written authorization from The Open Group.\n");
            printf("\n");
            printf("*/\n");
            printf("/* $XFree86: xc/programs/xset/xset.c,v 3.31 2003/05/27 22:27:09 tsi Exp $ */\n");
            printf("/* Modified by Stephen so keyboard rate is set using XKB extensions */\n");

#ifdef HAVE_WINDOWS_SPACEBALL
            printf("\n");
            printf("***********************************************************\n");
            printf("\n");

            printf("/*\n");
            printf("Spacenavigator related code in swt.c based on SpaceNav.cpp");
            printf("from Virtual Terrain Project\n");
            printf("\n");
            printf("/ Purpose: Implements Win32-specific support for the SpaceNavigator 6DOF device.\n");
            printf("/\n");
            printf("/ Based on: WM_INPUTExample.cpp, 3Dconnexion Inc. May 2007\n");
            printf("/\n");
            printf("/ Uses WM_INPUT/RawInput to get data from 3Dx devices.\n");
            printf("/ You can disconnect and reconnect devices at will while the program is running.\n");
            printf("/ You can connect more than one device at a time, and distinguish the arriving\n");
            printf("/ data between the different devices.\n");
            printf("/\n");
            printf("/ Copyright (c) 2008 Virtual Terrain Project\n");
            printf("/ Free for all uses, see license.txt for details.\n");
            printf("\n");
            printf("/ Content of license.txt from the Virtual Terrain Project follows :\n");
            printf("\n");
            printf("The source code and data in this distribution\n");
            printf("are Copyright (c) 2001-2007 Virtual Terrain Project.\n");
            printf("\n");
            printf("Permission is hereby granted, free of charge, to any person obtaining a\n");
            printf("copy of this software and associated documentation files (the \"Software\"),\n");
            printf("to deal in the Software without restriction, including without limitation\n");
            printf("the rights to use, copy, modify, merge, publish, distribute, sublicense,\n");
            printf("and/or sell copies of the Software, and to permit persons to whom the\n");
            printf("Software is furnished to do so, subject to the following conditions:\n");
            printf("\n");
            printf("The above copyright notice and this permission notice shall be included\n");
            printf("in all copies or substantial portions of the Software.\n");
            printf("\n");
            printf("THE SOFTWARE IS PROVIDED \"AS IS\", WITHOUT WARRANTY OF ANY KIND, EXPRESS\n");
            printf("OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,\n");
            printf("FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL\n");
            printf("THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR\n");
            printf("OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,\n");
            printf("ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR\n");
            printf("OTHER DEALINGS IN THE SOFTWARE.\n");
            printf("*/\n");
 #endif
            printf("\n");
            printf("***********************************************************\n");
            printf("\n");
            printf(" Mesa off screen rendering functions in swt/motif/swt.c based on\n");
            printf(" Demo of off-screen Mesa rendering\n");
            printf(" \n");
            printf(" See Mesa/include/GL/osmesa.h for documentation of the OSMesa functions.\n");
            printf(" \n");
            printf(" This demo program is in the public domain.\n");
            printf(" \n");
            printf(" Brian Paul\n");
            printf("\n");
            printf("***********************************************************\n");
            printf("\n");
            printf("License of\n");
            printf("BooleanSequencerPROTO.wrl\n");
            printf("BooleanSequencerExample.wrl\n");
            printf("IntegerSequencerPROTO.wrl\n");
            printf("Arc2DPROTO.wrl\n");
            printf("ArcClose2DPROTO.wrl\n");
            printf("Circle2DPROTO.wrl\n");
            printf("CADGeometryPROTO.wrl\n");
            printf("Disk2DPROTO.wrl\n");
            printf("Polyline2DPROTO.wrl\n");
            printf("Polypoint2DPROTO.wrl\n");
            printf("Rectangle2DPROTO.wrl\n");
            printf("TriangleSet2DPROTO.wrl\n");
            printf("BooleanFilterPROTO.wrl\n");
            printf("BooleanTogglePROTO.wrl\n");
            printf("BooleanTriggerPROTO.wrl\n");
            printf("IntegerTriggerPROTO.wrl\n");
            printf("TimeTriggerPROTO.wrl\n");
            printf("\n");

            printf("NPS MOVES Software License\n");
            printf("\n");
            printf("Copyright (c) 1995-2008 held by the author(s).  All rights reserved.</p>\n");
            printf("\n");
            printf("Redistribution and use in source and binary forms, with or without\n");
            printf("modification, are permitted provided that the following conditions\n");
            printf("are met:\n");
            printf("\n");
            printf("       Redistributions of source code must retain the above copyright\n");
            printf("       notice, this list of conditions and the following disclaimer.\n");
            printf("\n");
            printf("       Redistributions in binary form must reproduce the above copyright\n");
            printf("       notice, this list of conditions and the following disclaimer\n");
            printf("       in the documentation and/or other materials provided with the\n");
            printf("       distribution.\n");
            printf("\n");
            printf("       Neither the names of the \n");
            printf("       Naval Postgraduate School (NPS)\n");
            printf("       Modeling Virtual Environments and Simulation (MOVES) Institute\n");
            printf("       nor the names of its contributors may be used to endorse or\n");
            printf("       promote products derived from this software without specific\n");
            printf("       prior written permission.\n");
            printf("\n");
            printf("THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS\n");
            printf("\"AS IS\" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT\n");
            printf("LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS\n");
            printf("FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE\n");
            printf("COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,\n");
            printf("INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,\n");
            printf("BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;\n");
            printf("LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER\n");
            printf("CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT\n");
            printf("LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN\n");
            printf("ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE\n");
            printf("POSSIBILITY OF SUCH DAMAGE.\n");
            printf("\n");
            printf("description: NPS MOVES Software License\n");
            printf("author: Donald P. Brutzman\n");
            printf("created: 4 March 2005\n");
            printf("revised: 17 December 2006\n");
            printf("reference: http://www.oreilly.com/catalog/osfreesoft\n");
            printf("reference: http://opensource.org/licenses/bsd-license.php\n");
            printf("reference: http://xchat.movesinstitute.org/bugzilla/show_bug.cgi?id=32\n");

            printf("***********************************************************\n");
            printf("\n");
            printf("License of\n");
            printf("writeWonderlandModule.cpp\n");
            printf("\n");

            printf("Copyright (c) 2004-2009, Sun Microsystems, Inc., All Rights Reserved\n");
            printf("\n");
            printf("Redistributions in source code form must reproduce the above\n");
            printf("copyright and this condition.\n");
            printf("\n");
            printf("The contents of this file are subject to the GNU General Public\n");
            printf("License, Version 2 (the \"License\"); you may not use this file\n");
            printf("except in compliance with the License. A copy of the License is\n");
            printf("available at http://www.opensource.org/licenses/gpl-license.php.\n");
            printf("\n");
            printf("Sun designates this particular file as subject to the \"Classpath\"\n");
            printf("exception as provided by Sun in the License file that accompanied\n");
            printf("this code.\n");
            printf("\n");

            printf("The License file is a standard Licence File of the\n");
            printf("GNU General Public License (GPL) Version 2, June 1991\n");
            printf("with the following attachment:\n");
            printf("\n");

            printf("\"CLASSPATH\" EXCEPTION TO THE GPL VERSION 2\n");
            printf("\n");
            printf("Certain source files distributed by Sun Microsystems, Inc.  are subject to the\n");
            printf("following clarification and special exception to the GPL Version 2, but only\n");
            printf("where Sun has expressly included in the particular source file's header the\n");
            printf("words \"Sun designates this particular file as subject to the \"Classpath\"\n");
            printf("exception as provided by Sun in the License file that accompanied this code.\" \n");
            printf("\n");
            printf("Linking this library statically or dynamically with other modules is making a\n");
            printf("combined work based on this library.  Thus, the terms and conditions of the GNU\n");
            printf("General Public License Version 2 cover the whole combination. \n");
            printf("\n");
            printf("As a special exception, the copyright holders of this library give you \n");
            printf("permission to link this library with independent modules to produce an \n");
            printf("executable, regardless of the license terms of these independent modules, \n");
            printf("and to copy and distribute the resulting executable under terms of your \n");
            printf("choice, provided that you also meet, for each linked independent module, \n");
            printf("the terms and conditions of the license of that module.  An independent \n");
            printf("module is a module which is not derived from or based on this library.  If \n");
            printf("you modify this library, you may extend this exception to your version of \n");
            printf("the library, but you are not obligated to do so.  If you do not wish to do \n");
            printf("so, delete this exception statement from your version.\n");

            printf("***********************************************************\n");
            printf("\n");
            printf("License of\n");
            printf("winres.h\n");
            printf("\n");

            printf(" * Copyright (C) 2000 Francois Gouget\n");
            printf(" * \n");
            printf(" * This library is free software; you can redistribute it and/or\n");
            printf(" * modify it under the terms of the GNU Lesser General Public\n");
            printf(" * License as published by the Free Software Foundation; either\n");
            printf(" * version 2.1 of the License, or (at your option) any later version.\n");
            printf(" *\n");
            printf(" * This library is distributed in the hope that it will be useful,\n");
            printf(" * but WITHOUT ANY WARRANTY; without even the implied warranty of\n");
            printf(" * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU\n");
            printf(" * Lesser General Public License for more details.\n");
            printf(" *\n");
            printf(" * You should have received a copy of the GNU Lesser General Public\n");
            printf(" * License along with this library; if not, write to the Free Software\n");
            printf(" * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA\n");
            printf("\n");

            printf("***********************************************************\n");
            printf("\n");
            printf("/*\n");
            printf(" * write_png_file() in WonderlandModuleExport.cpp based on example application\n");
            printf(" *\n");
            printf(" * Copyright 2002-2010 Guillaume Cottenceau.\n");
            printf(" *\n");
            printf(" * This software may be freely redistributed under the terms\n");
            printf(" * of the X11 license.\n");
            printf(" *\n");
            printf(" */\n");

            printf("***********************************************************\n");

            printf("\n");
            printf("/*\n");
            printf(" * XML parsing code from doug sanden (inside parser.y)\n");
            printf(" *\n");
            printf(" * Copyright (C) 2010 Doug Sanden\n");
            printf(" *\n");
            printf(" * This source is licensed under the GNU LGPL v2.1\n");
            printf(" * Please read http://www.gnu.org/copyleft/lgpl.html for more information\n");
            printf(" *\n");
            printf(" * This software comes with the standard NO WARRANTY disclaimer for any\n");
            printf(" * purpose. Use it at your own risk. If there's a problem you get to fix it.\n");
            printf(" *\n");
            printf(" *\n");
            printf(" * I, Doug Sanden, hereby grant the permission to change the License on the \n");
            printf(" * white_dune XML parsing code from GPL to LGPL. \n");
            printf(" *\n");
            printf(" */\n");

            printf("***********************************************************\n");

            printf(" /*\n");
            printf("  * copyright for mesh.h/mesh.c subd.h/subd.c\n"); 
            printf("                  mathx.h/mathx.c and buf.h/buf.c\n");
            printf(" \n");
            printf(" The MIT License\n");
            printf(" \n");
            printf(" Copyright (c) 2009-2017 Slavomir Kaslev, https://github.com/skaslev/catmull-clark\n");
            printf(" \n");
            printf(" Permission is hereby granted, free of charge, to any person obtaining a copy\n");
            printf(" of this software and associated documentation files (the \"Software\"), to deal\n");
            printf(" in the Software without restriction, including without limitation the rights\n");
            printf(" to use, copy, modify, merge, publish, distribute, sublicense, and/or sell\n");
            printf(" copies of the Software, and to permit persons to whom the Software is\n");
            printf(" furnished to do so, subject to the following conditions:\n");
            printf(" \n");
            printf(" The above copyright notice and this permission notice shall be included in\n");
            printf(" all copies or substantial portions of the Software.\n");
            printf(" \n");
            printf(" THE SOFTWARE IS PROVIDED \"AS IS\", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR\n");
            printf(" IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,\n");
            printf(" FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE\n");
            printf(" AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER\n");
            printf(" LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,\n");
            printf(" OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN\n");
            printf(" THE SOFTWARE.\n");
            printf("\n");

            printf("/***************************************************************************\n");
            printf(" * Scene::Download() based on ftpget example of Curl library\n");
            printf(" * \n");
            printf(" * Copyright (C) 1998 - 2017, Daniel Stenberg, <daniel@haxx.se>, et al.\n");
            printf(" * \n");
            printf(" * This software is licensed as described in the file COPYING, which   \n");
            printf(" * you should have received as part of this distribution. The terms \n");
            printf(" * are also available at https://curl.haxx.se/docs/copyright.html.\n");
            printf(" *\n");
            printf(" * You may opt to use, copy, modify, merge, publish, distribute and/or sell\n");
            printf(" * copies of the Software, and permit persons to whom the Software is\n");
            printf(" * furnished to do so, under the terms of the COPYING file.\n");
            printf(" * \n");
            printf(" * This software is distributed on an \"AS IS\" basis, WITHOUT WARRANTY OF ANY\n");
            printf(" * KIND, either express or implied.\n");
            printf(" * \n");
            printf(" ***************************************************************************/\n");
            printf("\n");

            printf("/***************************************************************************\n");
            printf(" * Text rendering in NodeText.cpp based on https://github.com/codetiger/Font23D/ Font2OBJ.cpp\n");
            printf(" * with the following license:\n");
            printf(" *\n");
            printf(" * A license with no conditions whatsoever which dedicates works to the public\n");
            printf(" * domain. Unlicensed works, modifications, and larger works may be\n");
            printf(" * distributed under different terms and without source code.\n");
            printf(" ***************************************************************************/\n");
            printf("\n");

            printf("/***************************************************************************\n");
            printf(" * Contour.cpp/.h, PointFtgl.cpp/.h and Vectoriser.cpp/.h based on FTGL lib\n");
            printf(" * \n");
            printf(" * FTGL - OpenGL font library\n");
            printf(" *\n");
            printf(" * Copyright (c) 2001-2004 Henry Maddocks <ftgl@opengl.geek.nz>\n");
            printf(" * Copyright (c) 2008 Éric Beets <ericbeets@freefr>\n");
            printf(" * Copyright (c) 2008 Sam Hocevar <sam@zoy.org>\n");
            printf(" *\n");
            printf(" * Permission is hereby granted, free of charge, to any person obtaining\n");
            printf(" * a copy of this software and associated documentation files (the\n");
            printf(" * \"Software\"), to deal in the Software without restriction, including\n");
            printf(" * without limitation the rights to use, copy, modify, merge, publish,\n");
            printf(" * distribute, sublicense, and/or sell copies of the Software, and to\n");
            printf(" * permit persons to whom the Software is furnished to do so, subject to\n");
            printf(" * the following conditions:\n");
            printf(" *\n");
            printf(" * The above copyright notice and this permission notice shall be\n");
            printf(" * included in all copies or substantial portions of the Software.\n");
            printf(" *\n");
            printf(" * THE SOFTWARE IS PROVIDED \"AS IS\", WITHOUT WARRANTY OF ANY KIND,\n");
            printf(" * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF\n");
            printf(" * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.\n");
            printf(" * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY\n");
            printf(" * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,\n");
            printf(" * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE\n");
            printf(" * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.\n");
            printf(" ***************************************************************************/\n");
            printf("\n");

            printf("/***************************************************************************\n");
            printf(" * poly2tri.h, advancing_front.cpp/.h, sweep.cpp/.h, cdt.cpp/.h, utils.h\n"); 
            printf(" * and shapes.cpp/.h based on Poly2Tri library\n");
            printf(" * \n");
            printf(" * Poly2Tri Copyright (c) 2009-2010, Poly2Tri Contributors\n");
            printf(" * http://code.google.com/p/poly2tri/\n");
            printf(" *\n");
            printf(" * All rights reserved.\n");
            printf(" *\n");
            printf(" * Redistribution and use in source and binary forms, with or without modification,\n");
            printf(" * are permitted provided that the following conditions are met:\n");
            printf(" *\n");
            printf(" * * Redistributions of source code must retain the above copyright notice,\n");
            printf(" *   this list of conditions and the following disclaimer.\n");
            printf(" * * Redistributions in binary form must reproduce the above copyright notice,\n");
            printf(" *   this list of conditions and the following disclaimer in the documentation\n");
            printf(" *   and/or other materials provided with the distribution.\n");
            printf(" * * Neither the name of Poly2Tri nor the names of its contributors may be\n");
            printf(" *   used to endorse or promote products derived from this software without specific\n");
            printf(" *   prior written permission.\n");
            printf(" *\n");
            printf(" * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS\n");
            printf(" * \"AS IS\" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT\n");
            printf(" * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR\n");
            printf(" * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR\n");
            printf(" * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,\n");
            printf(" * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,\n");
            printf(" * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR\n");
            printf(" * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF\n");
            printf(" * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING\n");
            printf(" * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS\n");
            printf(" * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.\n");
            printf(" ***************************************************************************/\n");
            printf("\n");

#ifdef HAVE_LIBCGAL
            printf("\n");
            printf(" This file is compiled with CGAL: CGAL is licenced under different\n");
            printf(" licences including GNU General Public License 3 \n");
#endif

#ifdef HAVE_LIBFREETYPE
            printf("\n");
            printf(" This file is compiled with freetype2: freetype is licenced under different\n");
            printf(" licences including GNU General Public License 3 \n");
#endif

#ifdef HAVE_LIBVCG
            printf("\n");
            printf(" This file is compiled with libvcg: libvcg is licenced under\n");
            printf(" GNU General Public License 3\n");
#endif
            printf("\n");
            printf("This file uses code from vx\n");
            printf("https://github.com/neurolabusc/vx/blob/master/src/tex3d.inc\n");
            printf("\n");
            printf("****************************************************************************\n");
            printf("This is a BSD 3-Clause License :: https://opensource.org/licenses/BSD-3-Clause\n");
            printf("\n");
            printf("The Software has been developed for research purposes only and is not a clinical tool\n");
            printf("Copyright (c) 2014-2018 Chris Rorden. All rights reserved.\n");
            printf("\n");
            printf("Redistribution and use in source and binary forms, with or without\n");
            printf("modification, are permitted provided that the following conditions\n");
            printf("are met:\n");
            printf("1. Redistributions of source code must retain the above copyright\n");
            printf("   notice, this list of conditions and the following disclaimer.\n");
            printf("2. Redistributions in binary form must reproduce the above copyright\n");
            printf("   notice, this list of conditions and the following disclaimer in the\n");
            printf("   documentation and/or other materials provided with the distribution.\n");
            printf("3. The name of the copyright owner may be used to endorse or promote\n");
            printf("   products derived from this software without specific prior written\n");
            printf("   permission.\n");
            printf("\n");
            printf("THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT OWNER ``AS IS'' AND ANY EXPRESS OR\n");
            printf("IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF\n");
            printf("MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO\n");
            printf("EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,\n");
            printf("INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,\n");
            printf("BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,\n");
            printf("DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF\n");
            printf("LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE\n");
            printf("OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF\n");
            printf("ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.\n");
            printf("****************************************************************************/\n");

            normalExit(0);
        }
    }
}

