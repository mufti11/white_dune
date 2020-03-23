/* .//src/config.h.  Generated from config.h.in by configure.  */
/* Copyright (c) 1999 Stephen F. White (GPL)*/

#ifndef CONFIG_H
#define CONFIG_H

/* Define to test "this" against NULL. Almost! impossible, but needed... */
#define HAVE_NULL_COMPARE 1

/* Define to empty if the keyword does not work.  */
/* #undef const */

/* Define to 0 if X3DOM support PROTOS (not yet) */
#define HAVE_NO_PROTOS_X3DOM 1

/* Define to 0 if X3DOM support StaticGroup (not really yet) */
#define HAVE_NO_STATIC_GROUP_X3DOM 1

// define if you want optimization and do not have animation in a PROTO 
// use with care !
/* #undef HAVE_PROTO_INITIALIZATION_OPTIMIZATION */

/* Define to test "this" against NULL. Almost! impossible */
/* #undef HAVE_NULL_COMPARE */

/* Define if you have the ANSI C header files.  */
/* #undef STDC_HEADERS */

/* Define if you have the expat library */
#define HAVE_LIBEXPAT 1

/* Define if you have the vcg library (include files only)*/
#define HAVE_LIBVCG 1

/* Define if you have the CGAL library */
#define HAVE_LIBCGAL 1

/* Define if you have the curl library */
#define HAVE_LIBCURL 1

/* Define if you have the IL library */
/* #undef HAVE_LIBIL */

/* Define if you have the ILU library  */
/* #undef HAVE_LIBILU */

/* IL + ILU = DevIL */

#ifdef HAVE_LIBIL
# ifdef HAVE_LIBILU
#  define HAVE_LIBDEVIL 1
# endif
#endif

/* Define if you have the jpeg library */
#define HAVE_LIBJPEG 1

/* Define if you have the png library  */
#define HAVE_LIBPNG 1

/* Define if the link of the png Library fails cause of png_handle_unknown */
/* #undef HAVE_NO_PNG_HANDLE_UNKNOWN */

/* Define if you want to use the gif loading routines */
#define HAVE_GIF 1

/* Define if you have the z library  */
#define HAVE_LIBZ 1

/* Define if you have the ffmpeg libraries (see configure/configure.in */
#define HAVE_FFMPEG 1

/* Define if you have OpenMP */
#define HAVE_OMP 1

/* define if you use the spaceball library */
/* #undef HAVE_LIBSBALL */

/* define if you have the libusb library */
#ifndef __CYGWIN__
# define HAVE_LIBUSB 1
#endif

/* define if you want to convert to The Nebula Device 3D engine ("sanddune") */
/* #undef HAVE_SAND */

/* Define if you have snprintf.  */
#define HAVE_SNPRINTF 1

/* Define if you have XML_StopParser.  */
#define HAVE_XML_STOPPARSER 1

#ifndef HAVE_SNPRINTF
#   define snprintf mysnprintf
#endif

/* Define if you have glFogCoordf.  */
#define HAVE_GLFOGCOORDF 1

/* Define if you have glDeleteTextures.  */
#define HAVE_GLDELETETEXTURES 1

/* Define if you have glGenTextures.  */
#define HAVE_GLGENTEXTURES 1

/* Define if you have glBindTexture.  */
#define HAVE_GLBINDTEXTURE 1

/* Define if you have glActiveTexture */
#define HAVE_GLACTIVETEXTURE 1

#ifdef HAVE_GLACTIVETEXTURE
# define HAVE_MULTI_TEXTURE 1
#endif

/* Define if you have OSMesaCreateContext */
/* #undef HAVE_OSMESACREATECONTEXT */

#ifdef HAVE_OSMESACREATECONTEXT
# define HAVE_MESA_OFF_SCREEN_RENDERING 1 
#endif

/* Define if you have OSMesaCreateContextExt */
/* #undef HAVE_OSMESACREATECONTEXTEXT */

#ifdef HAVE_OSMESACREATECONTEXTEXT
# define HAVE_MESA_OFF_SCREEN_RENDERING 1 
#endif

/* Define if you have GLU_NURBS_TESSELLATOR.  */
#define HAVE_GLU_NURBS_TESSELLATOR 1

/* Define if you have gluNewTess.  */
#define HAVE_GLUNEWTESS 1

/* Define if you have _GLUfuncptr.  */
#define HAVE_GLU_FUNC_PTR 1

#ifdef HAVE_GLU_FUNC_PTR
# define MY_GLU_CALLBACK_PTR _GLUfuncptr
#else
# define MY_GLU_CALLBACK_PTR GLvoid (*)()
#endif

/* Define if you have gettimeofday.  */
#define HAVE_GETTIMEOFDAY 1

/* Define if you have powf. */
#define HAVE_POWF 1

/* Define if you have powl. */
#define HAVE_POWL 1

/* Define if you have rint. */
#define HAVE_RINT 1

#ifndef HAVE_RINT
# define rint(x) (x - (((x + 0.5) - floor(x))))
#endif

/* Define if you have atexit. */
#define HAVE_ATEXIT 1

/* Define if you have _exit. */
#define HAVE__EXIT 1

#ifdef HAVE_ATEXIT
# ifdef HAVE__EXIT
#  define EXIT_HANDLER
# endif
#endif

/* define if you have the freetype2 headers/library */
#define HAVE_LIBFREETYPE 1

/* define if you have the OpenSubdiv headers/library */
#define HAVE_LIBOSDCPU 1

/* Define if you have stdarg.h */ 
#define HAVE_STDARG_H 1

/* Define if you have varargs.h */ 
/* #undef HAVE_VARARGS_H */

/* Define if you have signal_handler(int) */
#define HAVE_SIGNAL_INT 1

#ifdef __CYGWIN__
# define HAVE_SIGNAL_INT 1
#endif

/* Define if your computer is a OLPC ("one laptop per child") */
/* #undef HAVE_OLPC */

/* Define if your computer has 64 bit (void *) pointers */
#define HAVE_VOID_PTR_SIZE_8 1

/* Define if you want double size icons (automatically set on OLPC) */
/* #undef HAVE_ICON_SCALE_2 */

/* Define if your C++ compiler support "typename" */
#define HAVE_TYPENAME 1

#ifdef HAVE_TYPENAME
# define TYPENAME typename
#else
# define TYPENAME
#endif

/* Define if you suspect a problem with a older motif/lesstif version */ 
/* #undef HAVE_OLD_MOTIF */

/* 
   Define to disable the function, if file -> textedit do not return. 
   This has been occured in glXDestroyContext on a radeon graphics card 
   on FreeBSD 5.0-RELEASE. 
   This is possibly a bug in the file -> textedit implementation itself 8-(
*/
/* #undef HAVE_NO_TEXTEDIT */

/* Define if you want euler angles instead of VRML like rotations */
/* #undef HAVE_EULER_ROT */

/* define if ROUTE statements should be at the end of the VRML file */ 
/* #undef HAVE_ROUTE_AT_END */

/* 
  command to switch into quadbuffer stereomode 
  e.g. "/usr/gfx/setmon -n 1024x768_96s"
  on some SGI IRIX systems
*/ 
/* #undef HAVE_STEREOCOMMAND */

/* define if you do not have shutterglases */
/* #undef HAVE_NO_STEREO */

/* define if you have a Joystick under Linux */
#define LINUX_JOYSTICK 1

/* define if you have a Joystick under M$Windows */
/* #undef WINDOWS_JOYSTICK */

/* define if you have a P5 data glove under M$Windows */
/* #undef HAVE_WINDOWS_P5 */

/* define if you have a space navigator device under M$Windows XP */
/* #undef WINDOWS_SPACEBALL */

/* defined if a Joystick supported by the SDLjoystick directory was detected */
/* #undef HAVE_SDL_JOYSTICK */

/* if libusb is available, nxtdials would work */
#ifdef HAVE_LIBUSB
# define HAVE_NXT_DIALS 1
#endif

/* define if you have a xinput device like magellan or spaceball */
#define HAVE_XINPUT 1

/* define if you have XkbKeycodeToKeysym in libX11 */
#define HAVE_XKBKEYCODETOKEYSYM 1

/* undefine if you dont want support for a Ascention Flock of birds tracker */
#ifndef __CYGWIN__
# define HAVE_AFLOCK 1 /* now aways true */
#endif

/* define if you have c_ospeed in the termios struct */
/* #undef HAVE_C_OSPEED */

/* define if you want to see debug messsages for update requests for the 
   different views/windows */
/* #undef HAVE_UPDATE_DEBUG */

/* define if you want to see debug messsages for Ascention Flock of birds 
   tracker */
/* #undef HAVE_AFLOCK_DEBUG */

/* define if you want the menu in german language */
/* #undef HAVE_MENU_GERMAN */

/* where the java engine exist */
/* #undef JAVA_PATH */

/*  
    switch off emergencysave signalhandling 
    Useful for testing, or if the signalhandling code do not match your system
 */
/* #undef HAVE_WANT_CORE */

/* Define if you have _FPU_SETCW to enable fpu interrupts (i386 Linux) */
/* #undef HAVE_FPU_SETCW */

#ifdef HAVE_FPU_SETCW
# define HAVE_FPU_ENABLE_INTERRUPTS 1
#endif

/* Define if you have handle_sigfpes to enable fpu interrupts (IRIX) */
/* #undef HAVE_HANDLE_SIGFPES */

#ifdef HAVE_HANDLE_SIGFPES
# define HAVE_FPU_ENABLE_INTERRUPTS 1
#endif

/* Define if you need to define _XOPEN_SOURCE 600 to use fpclassify */
/* #undef HAVE_XOPEN_SOURCE_4_FPCLASSIFY */

/* Define if you have fpclassify to test for floating point NAN and INFINITE */
#define HAVE_FPCLASSIFY 1

/* Define if you have isless to test for floating point INFINITE or -INFINITE */
/* #undef HAVE_ISLESS */

/* Define if you have a working C++ new handler */
/* #undef HAVE_NEW_HANDLER */

/* Define if you have a working new style C++ new handler */
#define HAVE_NEW_NEW_HANDLER 1

/* 
   add a extra submenu useful for teaching things
 */
/* #undef HAVE_TEACHER_MENU */

/* 
   insert a extra menupoint for testing new things, look for 
   MainWindow::testInMenu() in MainWindow.cpp to see how to use this menupoint 
 */
/* #undef HAVE_TEST_IN_MENU */

/* www browser to view html/vrml files*/
#define HAVE_WWW_BROWSER "/usr/bin/xdg-open %s &"

/* vrml browser to view vrml files*/
#define HAVE_VRML_BROWSER "/usr/bin/view3dscene %s"

/* vrml browser to view vrml files with a remote command*/
#define HAVE_VRML_REMOTE_BROWSER "/usr/bin/xdg-open %s"

/* text editor for txt files */
#define HAVE_X11_EDITOR "/usr/bin/gedit"

/* object editor for bitmap (.png/.jpg/.gif) files */
#define HAVE_IMAGE_EDITOR "/usr/bin/gimp"

/* (path to) ant (apache ant: "Another Neat Tool") */
/* #undef HAVE_ANT */

/* (path to) graphical commandline terminal (e.g. xterm) */
/* #undef HAVE_TERM */

/* easy object editor for bitmap (.png/.jpg/.gif) files for beginners */
#define HAVE_IMAGE_EDITOR4KIDS "/usr/bin/kolourpaint"

/* object editor for sound (.wav/.midi) files */
#define HAVE_SOUND_EDITOR "/usr/bin/audacity"

/* object editor for movie (.mpeg) files */
#define HAVE_MOVIE_EDITOR ""

/* renderman image texturer (eg. teqser) */
#define HAVE_RIB_TEXTURER "/usr/bin/teqser"

/* 
  default non interactive of graphical checkin command of a revision control 
  system such as rcs, csv, svn etc.
*/
#define HAVE_CHECK_IN_COMMAND "ci -l -q -f \"%s\" < /dev/null"

/* converter to convert from bitmap (.png/.jpg/.gif) files to bmp files for ac3d export*/
#define HAVE_IMAGE_CONVERTER "/usr/bin/convert"

/* Define if you want to set the URL of the helppages */
#define HAVE_HELP_URL "/home/mufti/wdune-1.876/docs/index.html"

/* Define if you want to set the URL of the vrml node list */
/* #undef HAVE_VRML_NODES_URL */

/* Define if you want to set the URL of the x3d standard (base url) */
/* #undef HAVE_X3D_URL */

/* Define if you want to try to get "edit -> cut" working */
/* #undef HAVE_CUT */

/* Define if you want to use the nonstandard "mode" field in TextureImage */
/* #undef HAVE_TEXTUREIMAGE_MODE */

/* Define if you want to use the deprecated "Wave" node of the cover browser */
/* #undef HAVE_COVER_WAVE */

/* Define if you want to use the incomplete "TUIMap" node of the cover browser */
/* #undef HAVE_COVER_TUI_MAP */

/* Define if opening a new file require a new window */
/* #undef HAVE_OPEN_IN_NEW_WINDOW */

/* 
   Define if you do not want to automatically replace "vrmlscript:" 
   (e.g. made by cosmoworlds 1.02)  with "javascript:" in URLs 
   ("vrmlscript" not recommended for VRML97 compatibility) 
*/
/* #undef HAVE_DONT_REPLACE_VRMLSCRIPT */

/*  
   Define if you want to set the location where to find the PROTOs of the 
   VRML97 Amendment1 nodes 
*/
#define HAVE_VRML97_AMENDMENT1_PROTO_URL ".//docs/vrml97Amendment1"

/*  
   Define if you want to set the location where to find the PROTOs of the 
   X3D nodes 
*/
#define HAVE_X3D_PROTO_URL ".//docs/x3d"

/*  
   Define if you want to set the location where to find the PROTOs of the 
   nodes, which are implemented only via scripts
*/
#define HAVE_SCRIPTED_NODES_PROTO_URL ".//docs/scripted_Nodes"

/*  
   Define if you want to set the location where to find the PROTOs of the 
   nodes, which are placeholders for unportable Cover/Covise nodes
*/
#define HAVE_COVER_NODES_PROTO_URL ".//docs/coverNodes"

/*
   Define if you want to set the location where to find the PROTOs of the
   nodes, which are placeholders for nodes used as container for data of
   exportable fileformat
*/
/* #undef HAVE_EXPORT_CONTAINER_PROTO_URL */

/*
   Define if you want to set the location of the X_ITE support files
   (e.g. x_ite.css or x_ite.js)         
   If not set, a default value is used
*/
/* #undef HAVE_XITE_PATH */

/*
   Define if you want to set the location of the X3DOM support files
   (e.g. x3dom.css, x3dom.js or x3dom.swf)
   If not set, a default value is used
*/
/* #undef HAVE_X3DOM_PATH */

/* define if you have a tty file for UNIX/Linux fontrendering */
#define HAVE_DEFAULT_TTF_FILE "/usr/share/fonts/truetype/ttf-bitstream-vera/Vera.ttf"

/* 
   Define if you want to document the method to uninstall white_dune
*/
#define HAVE_UNINSTALL_COMMENT "no information how to uninstall white_dune: possibly it has been installed from source"

/* 
   Define command to start X11 (only important on the MacOS X desktop)
*/
/* #undef HAVE_X11STARTCOMMAND */

#if ((__GNUC__!=2) || (__GNUC_MINOR__!=8))
# define HAVE_GNUC_ECGS_2_8 1
#endif

/* define if OpenGL support Texture3D */
#define HAVE_TEXTURE_3D

/*
   Define if the executable should only run with Windows XP and newer
 */
/* #undef HAVE_WINDOWS_XP */

/*
   Define if the Compiler/M$Windows version supports
   Data Execution Prevention (DEP) / No eXecute (NX) 
*/
/* #undef HAVE_WINDOWS_NX */

/*
   Define the serial device (for devices like flock of birds)
*/
/* #undef SERIAL_DEVICE */

#ifndef SERIAL_DEVICE
# ifdef __linux__ 
#  define SERIAL_DEVICE "/dev/ttyS"
# else
#  ifdef IRIX
#   define SERIAL_DEVICE "/dev/ttyd"
#  else
#   ifdef AIX
#    define SERIAL_DEVICE "/dev/tty"
#   else
#    ifdef MACOSX
#     define SERIAL_DEVICE "/dev/tty.modem"
#    else
#     ifdef __FreeBSD__
#      define SERIAL_DEVICE "/dev/ttyd"
#     else
#      ifdef __sun
#       define SERIAL_DEVICE "/dev/term/a"
#      else
#       ifdef __sgi
#        define SERIAL_DEVICE "/dev/ttyd"
#       else
#        ifdef __OpenBSD__
#         define SERIAL_DEVICE "/dev/ttyd"
#        else
#         define SERIAL_DEVICE "/dev/tty"
#        endif
#       endif
#      endif
#     endif
#    endif
#   endif
#  endif
# endif
#endif

/*
   Define the offset of first available serial device
 */

/* #undef OFFSET_SERIAL_DEVICE */
/* #undef HAVE_NO_OFFSET_SERIAL_DEVICE */

#ifndef OFFSET_SERIAL_DEVICE
# ifdef __linux__ 
#  define OFFSET_SERIAL_DEVICE 0
# else
#  ifdef IRIX
#   define OFFSET_SERIAL_DEVICE 1
#  else
#   ifdef AIX
#    define OFFSET_SERIAL_DEVICE 0
#   else
#    ifdef MACOSX
#     define HAVE_NO_OFFSET_SERIAL_DEVICE 1
#    else
#     ifdef __FreeBSD__
#      define OFFSET_SERIAL_DEVICE 0
#     else
#      ifdef __sun
#       define HAVE_NO_OFFSET_SERIAL_DEVICE 1
#      else
#       ifdef __sgi
#        define OFFSET_SERIAL_DEVICE 1
#       else
#        ifdef __OpenBSD__
#         define OFFSET_SERIAL_DEVICE 0
#        else
#         define HAVE_NO_OFFSET_SERIAL_DEVICE 1
#        endif
#       endif
#      endif
#     endif
#    endif
#   endif
#  endif
# endif
#endif

#ifndef __CYGWIN__
# define HAS_MLOCK 1
#endif

#define HAVE_DUNE_VERSION "white_dune 1.876"

/*
   Define if you need compatibility to the deprecated NurbsCurveAnimation (now CurveAnimation) node (not implemented yet)
*/

#define NURBS_CURVE_ANIMATION_COMPATIBILTY 1

#endif /* !CONFIG_H */
