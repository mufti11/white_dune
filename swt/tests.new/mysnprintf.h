#ifndef _BSD_SNPRINTF_H
#define _BSD_SNPRINTF_H

#include <sys/types.h> /* For size_t */
#include "config.h"

#ifndef MY_NO_C 
 extern "C" 
#endif
int mysnprintf(char *str, const unsigned int count, const char *fmt, ...);
/* int myvsnprintf(char *str, size_t count, const char *fmt, va_list args); */

#ifdef WIN32
#include <Windows.h>
#include <stdio.h>
#ifndef MY_NO_C 
extern "C"
#endif
int msgboxprintf(FILE* f, const char *fmt, ...);
#endif

#ifndef MY_NO_C 
 extern "C" 
#endif
int mywritestr(int filedes,const char *str);

/* VARARGS3 */
#if defined(HAVE_STDARG_H)
# include <stdarg.h>
# ifndef MY_NO_C 
   extern "C" 
# endif
  int mywritef (int filedes,const char *fmt,...);
#else
# if defined(HAVE_VARARGS_H)
#  include <varargs.h>
#   ifndef MY_NO_C 
     extern "C" 
#   endif
    int mywritef (va_alist) va_dcl;
# endif
#endif

#ifndef MY_NO_C 
 extern "C" 
#endif
int mywritestr(int filedes,const char *str);

#ifndef MY_NO_C
 extern "C" 
#endif
size_t mystrlen(const char *str);

#ifndef MY_NO_C 
 extern "C"
#endif
char *mystrcpy(char *s1, const char *s2);

#ifndef MY_NO_C 
 extern "C" 
#endif
char *mystrcat(char *s1, const char *s2);

/*
 * strncpy that copies n bytes and can write zero to n+1-th byte
 * use this when you want to copy exact n bytes (or less)
 */
#ifndef MY_NO_C 
 extern "C" 
#endif
char* mystrncpy_danger(char* str1,const char *str2,int n);

/*
 * strncpy that copies n bytes and then write zero to n-th byte
 * use this when you want to copy less than n bytes
 */
#ifndef MY_NO_C
 extern "C"
#endif
char* mystrncpy_secure(char* str1,const char *str2,int n);

/*
 *  Macro to return at once on error
 */

#define RET_ONERROR(x) {int ret;if ((ret=(x))<0) return ret;}


#endif /* _BSD_SNPRINTF_H */
