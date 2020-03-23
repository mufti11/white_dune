/* src/config.h.  Generated automatically by configure.  */
/* Copyright (c) 1999 Stephen F. White (GPL)*/

#ifndef CONFIG_H
#define CONFIG_H

/* Define to empty if the keyword does not work.  */
/* #undef const */

/* Define if you have the ANSI C header files.  */
#define STDC_HEADERS 1

/* Define if you want "Save As Nebula Device" menu (exporter for Nebula)  */
#undef HAVE_SAND

/* Define if you have the IL Library */
#define HAVE_LIBIL 1

/* Define if you have the ILU Library  */
#define HAVE_LIBILU 1

/* IL + ILU = DevIL */

#ifdef HAVE_LIBIL
# ifdef HAVE_LIBILU
#  define HAVE_LIBDEVIL 1
# endif
#endif

/* Define if you have the z Library  (currently not working under m$windows) */
#undef HAVE_LIBZ

/* define if you use the spaceball library */
/* #undef HAVE_LIBSBALL */

/* Define if you have snprintf.  */
#define HAVE_SNPRINTF 1

#ifndef HAVE_SNPRINTF
#   define mysnprintf snprintf
#endif

/* Define if you have glDeleteTextures.  */
#define HAVE_GLDELETETEXTURES 1

/* Define if you have glGenTextures.  */
#define HAVE_GLGENTEXTURES 1

/* Define if you have glBindTexture.  */
#define HAVE_GLBINDTEXTURE 1

/* Define if you have gettimeofday.  */
#define HAVE_GETTIMEOFDAY 1

/* Define if you have powf.  */
#define HAVE_POWF 1

/* Define if you have rint.  */
/* #define HAVE_RINT 1  */

/* Define if you have stdarg.h */ 
#define HAVE_STDARG_H 1

/* Define if you have varargs.h */ 
#define HAVE_VARARGS_H 1

/* Define if you have signal_handler(int) */
/* #undef HAVE_SIGNAL_INT */

/* Define if you suspect a problem in lesstif: click to a icon, nothing works */ 
/* #undef HAVE_BUG_IN_LESSTIF */

/* Define if you want Kernigan/Richie like indent */
/* #undef HAVE_KR_LIKE_INDENT */

/* Define if you want euler angles instead of VRML like rotations */
/* #undef HAVE_EULER_ROT */

/* define if ROUTE statements should be at the end of the VRML file */ 
/* #undef HAVE_ROUTE_AT_END */

/* define if inputdevice do not care about window focus */ 
/* #undef HAVE_DONT_CARE_FOCUS */

/* 
  command to switch into quadbuffer stereomode 
  e.g. "/usr/gfx/setmon -n 1024x768_96s"
  on some SGI IRIX systems
*/ 
/* #undef HAVE_STEREOCOMMAND */

/* define if you do not have shutterglases */
/* #undef HAVE_NO_STEREO */

/* define if you have a Joystick under Linux */
/* #undef LINUX_JOYSTICK */

/* define if you have a Joystick under M$Windows */
#define WINDOWS_JOYSTICK 1

/* define if you have a xinput device like magellan or spaceball */
/* #define HAVE_XINPUT 1 */

/* define if you have a Ascention Flock of birds tracker */
/* #undef HAVE_AFLOCK */

/* define if you want to see debug messsages for Ascention Flock of birds 
   tracker */

/* #undef HAVE_AFLOCK_DEBUG */

/* define if you want the menu in german language */
#define HAVE_MENU_GERMAN 1

/* define if you want to have a very simple a NURBS modeller menu */
#define HAVE_SIMPLE_MENU_4_KIDS 1


#define READ_XML 1
#define WRITE_XML 1

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

/* Define if you have HAVE_HANDLE_SIGFPES to enable fpu interrupts (IRIX) */
/* #undef HAVE_HANDLE_SIGFPES */

#ifdef HAVE_HANDLE_SIGFPES
# define HAVE_FPU_ENABLE_INTERRUPTS 1
#endif

/* Define if you want to set the URL of the helppages */
#undef HAVE_HELP_URL

/* Define if you want to set the URL of the vrml node list */
#undef HAVE_VRML_NODES_URL


/* Define if you want to try to get "edit -> cut" working */
/* #undef HAVE_CUT */

/* Define if you want to use the nonstandard "mode" field in TextureImage */
/* #undef HAVE_TEXTUREIMAGE_MODE */

#if ((__GNUC__!=2) || (__GNUC_MINOR__!=8))
# define HAVE_GNUC_ECGS_2_8 1
#endif

#define TYPENAME

#endif /* !CONFIG_H */
