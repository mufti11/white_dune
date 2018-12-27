/*
 * Copyright Patrick Powell 1995
 * This code is based on code written by Patrick Powell (papowell@astart.com)
 * It may be used for any purpose as long as this notice remains intact
 * on all source code distributions
 */

#ifndef _BSD_SNPRINTF_H
#define _BSD_SNPRINTF_H

#include <sys/types.h> /* For size_t */
#include "config.h"

#ifdef MY_CPLUSPLUS 
 extern "C" 
#endif
int mysnprintf(char *str, size_t count, const char *fmt, ...);

#ifdef _WIN32
#include <Windows.h>
#include <stdio.h>
#ifdef MY_CPLUSPLUS 
extern "C"
#endif
int msgboxprintf(FILE* f, const char *fmt, ...);
#endif

/* VARARGS3 */
#if defined(HAVE_STDARG_H)
# include <stdarg.h>
# ifdef MY_CPLUSPLUS 
   extern "C" 
# endif
  int mywritef (int filedes,const char *fmt,...);
#else
# if defined(HAVE_VARARGS_H)
#  include <varargs.h>
#   ifdef MY_CPLUSPLUS 
     extern "C" 
#   endif
    int mywritef (va_alist) va_dcl;
# endif
#endif

#ifdef MY_CPLUSPLUS 
 extern "C" 
#endif
int myvsnprintf(char *str, size_t count, const char *fmt, va_list args);

#ifdef MY_CPLUSPLUS 
 extern "C" 
#endif
int myflushall(void);

#ifdef MY_CPLUSPLUS 
 extern "C" 
#endif
int myclose(int filedes);

#ifdef MY_CPLUSPLUS 
 extern "C" 
#endif
int mywrite(int filedes,const void *str,int size);

#ifdef MY_CPLUSPLUS 
 extern "C" 
#endif
int mywritestr(int filedes,const char *str);

#ifdef MY_CPLUSPLUS
 extern "C" 
#endif
size_t mystrlen(const char *str);

#ifdef MY_CPLUSPLUS 
 extern "C"
#endif
char *mystrcpy(char *s1, const char *s2);

#ifdef MY_CPLUSPLUS 
 extern "C" 
#endif
char *mystrcat(char *s1, const char *s2);

/*
 * strncpy that copies n bytes and can write zero to n+1-th byte
 * use this when you want to copy exact n bytes (or less)
 */
#ifdef MY_CPLUSPLUS 
 extern "C" 
#endif
char* mystrncpy_danger(char* str1,const char *str2,int n);

/*
 * strncpy that copies n bytes and then write zero to n-th byte
 * use this when you want to copy less than n bytes
 */
#ifdef MY_CPLUSPLUS
 extern "C"
#endif
char* mystrncpy_secure(char* str1,const char *str2,int n);

/*
 *  Macro to return at once on error
 */

#define RET_AND_RESET_ONERROR(x, y) { \
    int ret;           \
    if ((ret=(x))<0) { \
        y;             \
        return ret;    \
    }                  \
}

#define RET_ONERROR(x) RET_AND_RESET_ONERROR(x,)

#ifdef MY_CPLUSPLUS
 extern "C"
#endif
void set_number_of_digits(int digits);

#ifdef MY_CPLUSPLUS
 extern "C"
#endif
void set_Efloat_format(int flag);

#ifdef MY_CPLUSPLUS 
 extern "C" 
#endif
char *mystrdup(const char *s);

#endif /* _BSD_SNPRINTF_H */
