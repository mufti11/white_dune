/* Copyright (c) 1999 Stephen F. White (GPL)*/

#ifndef CONFIG_H
#define CONFIG_H

/* Define to test "this" against NULL. Almost! impossible, but needed... */
#define HAVE_NULL_COMPARE 1

/* Define to empty if the keyword does not work.  */
#undef const

/* Define if you have the ANSI C header files.  */
#undef STDC_HEADERS

/* Define if you have the expat Library */
#define HAVE_LIBEXPAT 1

/* Define if you have the vcg Library (include files only)*/
#define HAVE_LIBVCG 1

/* Define if you have the CGAL Library */
#define HAVE_LIBCGAL 1

/* Define if you have the Curl Library */
#define HAVE_LIBCURL 1

/* Define if you have the IL Library */
#undef HAVE_LIBIL

/* Define if you have the ILU Library  */
#undef HAVE_LIBILU

/* IL + ILU = DevIL */

#ifdef HAVE_LIBIL
# ifdef HAVE_LIBILU
#  define HAVE_LIBDEVIL 1
# endif
#endif

/* Define if you have the jpeg Library */
#define HAVE_LIBJPEG 1

/* Define if you have the png Library  */
#define HAVE_LIBPNG 1

/* Define if you have the z Library  */
#define HAVE_LIBZ 1

/* Define if you want to use the gif loading routines */
#define HAVE_GIF 1

/* Define if you have OpenMP */
#define HAVE_OMP 1

#define HAVE_LIBOSDCPU 1

/* Define if you have the Xdriver Library */
#define HAVE_XDRIVER

/* define if you use the spaceball library */
#undef HAVE_LIBSBALL

/* Define if you have GLU_NURBS_TESSELLATOR. We link against a newer libGLU */
#define HAVE_GLU_NURBS_TESSELLATOR 1

/* Define if you have gluNewTess.  */
#define HAVE_GLUNEWTESS 1

/* Define if you have _GLUfuncptr.  */
#undef HAVE_GLU_FUNC_PTR

#ifdef HAVE_GLU_FUNC_PTR
# define MY_GLU_CALLBACK_PTR _GLUfuncptr
#else
# ifndef WIN32
#  define MY_GLU_CALLBACK_PTR GLvoid (*)()
# else
#  define MY_GLU_CALLBACK_PTR void (__stdcall *)(void)
#endif
#endif

/* define if you want to convert to The Nebula Device 3D engine ("sanddune") */
#undef HAVE_SAND

/* Define if you have snprintf.  */
#define HAVE_SNPRINTF 1

#ifndef HAVE_SNPRINTF
#   define snprintf mysnprintf
#endif

/* Define if you have glDeleteTextures.  */
#undef HAVE_GLDELETETEXTURES

/* Define if you have glGenTextures.  */
#undef HAVE_GLGENTEXTURES

/* Define if you have glBindTexture.  */
#undef HAVE_GLBINDTEXTURE

/* Define if you have gettimeofday.  */
#undef HAVE_GETTIMEOFDAY

/* Define if you have powf.  */
#define HAVE_POWF 1

/* Define if you have powl. */
#define HAVE_POWL 1

/* Define if you have rint.  */
#define HAVE_RINT 1

#ifndef HAVE_RINT
# define rint(x) (x - (((x + 0.5) - floor(x))))
#endif

/* Define if you have stdarg.h */
#define HAVE_STDARG_H 1

/* Define if you have varargs.h */
#undef HAVE_VARARGS_H

/* Define if you have signal_handler(int) */
#undef HAVE_SIGNAL_INT

/* Define if your C++ compiler support "typename" */
#define HAVE_TYPENAME 1

#ifdef HAVE_TYPENAME
# define TYPENAME typename
#else
# define TYPENAME
#endif

/* Define if you suspect a problem in lesstif: click to a icon, nothing works */
#undef HAVE_BUG_IN_LESSTIF

/*
   Define to disable the function, if file -> textedit do not return.
   This has been occured in glXDestroyContext on a radeon graphics card
   on FreeBSD 5.0-RELEASE.
   This is possibly a bug in the file -> textedit implementation itself 8-(
*/
#undef HAVE_NO_TEXTEDIT

/* Define if you want euler angles instead of VRML like rotations */
#undef HAVE_EULER_ROT

/* define if ROUTE statements should be at the end of the VRML file */
#undef HAVE_ROUTE_AT_END

/*
  command to switch into quadbuffer stereomode
  e.g. "/usr/gfx/setmon -n 1024x768_96s"
  on some SGI IRIX systems
*/
#undef HAVE_STEREOCOMMAND

/* define if you do not have shutterglases */
#undef HAVE_NO_STEREO

/* define if you have a Joystick under Linux */
#undef LINUX_JOYSTICK

/* define if you have a Joystick under M$Windows */
#define WINDOWS_JOYSTICK

/* define if you have the DLL's for a DirectX8 joystick under M$Windows */
#undef DIRECTX_JOYSTICK

/* define if you have a P5 data glove under M$Windows */
#undef HAVE_WINDOWS_P5

/* defined if a Joystick supported by the SDLjoystick directory was detected */
#undef HAVE_SDL_JOYSTICK

/* define if you have a xinput device like magellan or spaceball */
#undef HAVE_XINPUT

/* undefine if you dont want support for a Ascention Flock of birds tracker */
#undef HAVE_AFLOCK

/* define if you have c_ospeed in the termios struct */
#undef HAVE_C_OSPEED

/* define size of short */
#undef SIZEOF_SHORT

/* define if you want to see debug messsages for Ascention Flock of birds
   tracker */
#undef HAVE_AFLOCK_DEBUG

/* define if you want the menu in german language */
#undef HAVE_MENU_GERMAN

/* where the java engine exist */
#undef JAVA_PATH

/*
    switch off emergencysave signalhandling
    Useful for testing, or if the signalhandling code do not match your system
 */
#undef HAVE_WANT_CORE

/* Define if you have _FPU_SETCW to enable fpu interrupts (i386 Linux) */
#undef HAVE_FPU_SETCW

#ifdef HAVE_FPU_SETCW
# define HAVE_FPU_ENABLE_INTERRUPTS 1
#endif

/* Define if you have HAVE_HANDLE_SIGFPES to enable fpu interrupts (IRIX) */
#undef HAVE_HANDLE_SIGFPES

#ifdef HAVE_HANDLE_SIGFPES
# define HAVE_FPU_ENABLE_INTERRUPTS 1
#endif

/* Define if you have a working C++ new handler */
#undef HAVE_NEW_HANDLER

/*
   insert a extra menupoint for testing new things, look for
   MainWindow::testInMenu() in MainWindow.cpp to see how to use this menupoint
 */
#undef HAVE_TEST_IN_MENU

/* www browser to view html/vrml files*/
#undef HAVE_WWW_BROWSER

/* vrml browser to view vrml files*/
#undef HAVE_VRML_BROWSER

/* vrml browser to view vrml files with a remote command*/
#undef HAVE_VRML_REMOTE_BROWSER

/* 
  default non interactive of graphical checkin command of a revision control 
  system. Under M$Windows you can use ports (e.g. cygwin ports) of UNIX 
  based systems like rcs, csv, svn or something like the batch orientated
  checkin command from Micro$oft Sourcesave....
*/
/* #undef HAVE_CHECK_IN_COMMAND */

/* Define if you want to set the URL of the helppages */
#undef HAVE_HELP_URL

/* Define if you want to set the URL of the vrml node list */
#undef HAVE_VRML_NODES_URL

/* Define if you want to try to get "edit -> cut" working */
#undef HAVE_CUT

/* Define if you want to use the nonstandard "mode" field in TextureImage */
#undef HAVE_TEXTUREIMAGE_MODE

/* Define if opening a new file require a new window */
#undef HAVE_OPEN_IN_NEW_WINDOW

/*
   Define if you do not want to automatically replace "vrmlscript:"
   (e.g. made by cosmoworlds 1.02)  with "javascript:" in URLs
   ("vrmlscript" not recommended for VRML97 compatibility)
*/
#undef HAVE_DONT_REPLACE_VRMLSCRIPT

/*
   Define if you want to set the location where to find the PROTOs of the
   VRML97 Amendment1 nodes
*/
#undef HAVE_VRML97_AMENDMENT1_PROTO_URL

/*
   Define if you want to set the location where to find the PROTOs of the
   X3D draft nodes
*/
#undef HAVE_X3D_PROTO_URL

/*
   Define if you want to set the location where to find the PROTOs of the
   nodes, which are implemented only via scripts
*/
#undef HAVE_SCRIPTED_NODES_PROTO_URL

/*
   Define command how to start X11 (only important on the MacOS X desktop)
*/
#undef HAVE_X11STARTCOMMAND

/* Define if you have fpclassify to test for floating point NAN and INFINITE */
#define HAVE_FPCLASSIFY 1

#if ((__GNUC__!=2) || (__GNUC_MINOR__!=8))
# define HAVE_GNUC_ECGS_2_8 1
#endif

#undef HAVE_DUNE_VERSION

#define HAVE_IMAGE_EDITOR "C:\\WinNT\\system32\\mspaint.exe"
#define HAVE_IMAGE_EDITOR4KIDS "C:\\WinNT\\system32\\mspaint.exe"
#define HAVE_SOUND_EDITOR ""
#define HAVE_MOVIE_EDITOR ""

/* define if you have the freetype2 headers/library */
#define HAVE_LIBFREETYPE 1

/* define if you have a tty file for UNIX/Linux fontrendering */
#define HAVE_DEFAULT_TTF_FILE "C:\\Windows\\Fonts\\arial.ttf"


#endif /* !CONFIG_H */
