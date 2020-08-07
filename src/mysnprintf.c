/*
 * Copyright Patrick Powell 1995
 * This code is based on code written by Patrick Powell (papowell@astart.com)
 * It may be used for any purpose as long as this notice remains intact
 * on all source code distributions
 */
/**************************************************************
 * Original:
 * Patrick Powell Tue Apr 11 09:48:21 PDT 1995
 * A bombproof version of doprnt (dopr) included.
 * Sigh.  This sort of thing is always nasty do deal with.  Note that
 * the version here does not include floating point...
 *
 * snprintf() is used instead of sprintf() as it does limit checks
 * for string length.  This covers a nasty loophole.
 *
 * The other functions are there to prevent NULL pointers from
 * causing nast effects.
 *
 * More Recently:
 *  Brandon Long <blong@fiction.net> 9/15/96 for mutt 0.43
 *  This was ugly.  It is still ugly.  I opted out of floating point
 *  numbers, but the formatter understands just about everything
 *  from the normal C string format, at least as far as I can tell from
 *  the Solaris 2.5 printf(3S) man page.
 *
 *  Brandon Long <blong@fiction.net> 10/22/97 for mutt 0.87.1
 *    Ok, added some minimal floating point support, which means this
 *    probably requires libm on most operating systems.  Don't yet
 *    support the exponent (e,E) and sigfig (g,G).  Also, fmtint()
 *    was pretty badly broken, it just wasn't being exercised in ways
 *    which showed it, so that's been fixed.  Also, formated the code
 *    to mutt conventions, and removed dead code left over from the
 *    original.  Also, there is now a builtin-test, just compile with:
 *           gcc -DTEST_SNPRINTF -o snprintf snprintf.c -lm
 *    and run snprintf for results.
 * 
 *  Thomas Roessler <roessler@guug.de> 01/27/98 for mutt 0.89i
 *    The PGP code was using unsigned hexadecimal formats. 
 *    Unfortunately, unsigned formats simply didn't work.
 *
 *  Michael Elkins <me@cs.hmc.edu> 03/05/98 for mutt 0.90.8
 *    The original code assumed that both snprintf() and vsnprintf() were
 *    missing.  Some systems only have snprintf() but not vsnprintf(), so
 *    the code is now broken down under HAVE_SNPRINTF and HAVE_VSNPRINTF.
 *
 **************************************************************/

#include "config.h"

#include <string.h>
#include <ctype.h>
#include <sys/types.h>
#ifndef _WIN32
#include <sys/stat.h>
#include <fcntl.h>
#endif

#ifndef FLT_MAX 
# include <float.h>
#endif
#include <math.h>
#include <stdlib.h>

#ifdef _WIN32
#include <io.h>
#include <stdio.h>
#include <windows.h>
#else
#include <unistd.h>
#endif

/* varargs declarations: */

#if defined(HAVE_STDARG_H)
# include <stdarg.h>
# define HAVE_STDARGS    /* let's hope that works everywhere (mj) */
# define VA_LOCAL_DECL   va_list ap;
# define VA_START(f)     va_start(ap, f)
# define VA_SHIFT(v,t)  ;   /* no-op for ANSI */
# define VA_END          va_end(ap)
#else
# if defined(HAVE_VARARGS_H)
#  include <varargs.h>
#  undef HAVE_STDARGS
#  define VA_LOCAL_DECL   va_list ap;
#  define VA_START(f)     va_start(ap)      /* f is ignored! */
#  define VA_SHIFT(v,t) v = va_arg(ap,t)
#  define VA_END        va_end(ap)
# else
/*XX ** NO VARARGS ** XX*/
# error no either stdarg or varargs needed
# endif
#endif

static void dopr (char *buffer, size_t maxlen, const char *format, 
                  va_list args);
static void fmtstr (char *buffer, size_t *currlen, size_t maxlen,
                    const char *value, int flags, int min, int max);
static void fmtint (char *buffer, size_t *currlen, size_t maxlen,
                    long value, int base, int min, int max, int flags);
static void fmtfp (char *buffer, size_t *currlen, size_t maxlen,
                   long double fvalue, int min, int max, int flags);
static void fmtefp (char *buffer, size_t *currlen, size_t maxlen,
                    long double fvalue, int min, int max, int flags);
static void dopr_outch (char *buffer, size_t *currlen, size_t maxlen, char c );

/* 
 * myisdigit(): signalhandlerparanoia isdigit version independend of C-Library
 */

static int myisdigit(int c)
   {
   switch(c)
      {
      case '0':
         return 1;
      case '1':
         return 1;
      case '2':
         return 1;
      case '3':
         return 1;
      case '4':
         return 1;
      case '5':
         return 1;
      case '6':
         return 1;
      case '7':
         return 1;
      case '8':
         return 1;
      case '9':
         return 1;
      default:
         return 0; 
      }
   }

 /* 
 * mystrlen(): signalhandlerparanoia strlen version independend of C-Library
 */

size_t mystrlen (const char *s)
   {
   int strln;
   for (strln = 0; s[strln]; ++strln);
   return strln;
   }

/* 
 * mystrcpy(): signalhandlerparanoia strcpy version independend of C-Library
 */

char *mystrcpy(char *s1, const char *s2)
   {
   int strln;
   for (strln = 0; s2[strln]; ++strln)
      s1[strln]=s2[strln];
   s1[strln]=0;
   return s1;
   }

/* 
 * mystrcat(): signalhandlerparanoia strcpy version independend of C-Library
 */

char *mystrcat(char *s1, const char *s2)
   {
   int strln;
   int length=mystrlen(s1);
   for (strln = 0; s2[strln]; ++strln)
      s1[length+strln]=s2[strln];
   s1[length+strln]=0;
   return s1;
   }


#ifdef HAVE_XOPEN_SOURCE_4_FPCLASSIFY
# define _XOPEN_SOURCE 600
# define HAVE_FPCLASSIFY 1
#endif

#ifdef HAVE_FPCLASSIFY

#define MAX_VALUE FLT_MAX

static long double handle_nan(int fpclass, long double fvalue)
{
  const char *error = NULL;
  long double ret = fvalue;
  switch (fpclass) {
    case FP_INFINITE:
//      error = "dune internal error: tried to write +/- INFINITE floating point value !\n"; 
      ret = 0;
      break;
    case FP_NAN:
//      error = "dune internal error: tried to write NAN floating point value !\n"; 
    case FP_SUBNORMAL:
//      error = "dune internal error: tried to write SUBNORMAL floating point value !\n"; 
      ret = 0;
      break;
    case FP_ZERO:
    case FP_NORMAL:
      break;
//    default:
//      error = "dune internal error: tried to write unclassified floatingpoint !\n"; 
  }
  if (error != NULL)
      while (write(2, error, strlen(error)) == -1);
  return ret;  
}

static long double check_nan(long double fvalue)
{
  return handle_nan(fpclassify(fvalue),fvalue); 
}

/*
static long double check_dnan(double fvalue)
{
  return handle_nan(fpclassify(fvalue),fvalue); 
}

static long double check_fnan(float fvalue)
{
  return handle_nan(fpclassify(fvalue),fvalue); 
}
*/

#endif 

/*
 * dopr(): poor man's version of doprintf
 */

/* format read states */
#define DP_S_DEFAULT 0
#define DP_S_FLAGS   1
#define DP_S_MIN     2
#define DP_S_DOT     3
#define DP_S_MAX     4
#define DP_S_MOD     5
#define DP_S_CONV    6
#define DP_S_DONE    7

/* format flags - Bits */
#define DP_F_MINUS      (1 << 0)
#define DP_F_PLUS       (1 << 1)
#define DP_F_SPACE      (1 << 2)
#define DP_F_NUM        (1 << 3)
#define DP_F_ZERO       (1 << 4)
#define DP_F_UP         (1 << 5)
#define DP_F_UNSIGNED   (1 << 6)

/* Conversion Flags */
#define DP_C_SHORT   1
#define DP_C_LONG    2
#define DP_C_LDOUBLE 3

#define char_to_int(p) (p - '0')
#ifndef MAX
# define MAX(p,q) ((p >= q) ? p : q)
#endif

static void dopr (char *buffer, size_t maxlen, const char *format, va_list args)
{
  char ch = 0;
  long value = 0;
  long double fvalue = 0;
  char *strvalue = NULL;
  int min = 0;
  int max = -1;
  int state = 0;
  int flags = 0;
  int cflags = 0;
  size_t currlen = 0;
  
  state = DP_S_DEFAULT;
  currlen = flags = cflags = min = 0;
  ch = *format++;

  while (state != DP_S_DONE)
  {
    if ((ch == '\0') || (currlen >= maxlen)) 
      state = DP_S_DONE;

    switch(state) 
    {
    case DP_S_DEFAULT:
      if (ch == '%') 
        state = DP_S_FLAGS;
      else 
        dopr_outch (buffer, &currlen, maxlen, ch);
      ch = *format++;
      break;
    case DP_S_FLAGS:
      switch (ch) 
      {
      case '-':
        flags |= DP_F_MINUS;
        ch = *format++;
        break;
      case '+':
        flags |= DP_F_PLUS;
        ch = *format++;
        break;
      case ' ':
        flags |= DP_F_SPACE;
        ch = *format++;
        break;
      case '#':
        flags |= DP_F_NUM;
        ch = *format++;
        break;
      case '0':
        flags |= DP_F_ZERO;
        ch = *format++;
        break;
      default:
        state = DP_S_MIN;
        break;
      }
      break;
    case DP_S_MIN:
      if (myisdigit((unsigned char)ch)) 
      {
        min = 10 * min + char_to_int (ch);
        ch = *format++;
      } 
      else if (ch == '*') 
      {
        min = va_arg (args, int);
        ch = *format++;
        state = DP_S_DOT;
      } 
      else 
        state = DP_S_DOT;
      break;
    case DP_S_DOT:
      if (ch == '.') 
      {
        state = DP_S_MAX;
        ch = *format++;
      } 
      else 
        state = DP_S_MOD;
      break;
    case DP_S_MAX:
      if (myisdigit((unsigned char)ch)) 
      {
        if (max < 0)
          max = 0;
        max = 10 * max + char_to_int (ch);
        ch = *format++;
      } 
      else if (ch == '*') 
      {
        max = va_arg (args, int);
        ch = *format++;
        state = DP_S_MOD;
      } 
      else 
        state = DP_S_MOD;
      break;
    case DP_S_MOD:
      /* Currently, we don't support Long Long, bummer */
      switch (ch) 
      {
      case 'h':
        cflags = DP_C_SHORT;
        ch = *format++;
        break;
      case 'l':
        cflags = DP_C_LONG;
        ch = *format++;
        break;
      case 'L':
        cflags = DP_C_LDOUBLE;
        ch = *format++;
        break;
      default:
        break;
      }
      state = DP_S_CONV;
      break;
    case DP_S_CONV:
      switch (ch) 
      {
      case 'd':
      case 'i':
        if (cflags == DP_C_SHORT) 
          value =(short int)  va_arg (args, int);
        else if (cflags == DP_C_LONG)
          value = va_arg (args, long int);
        else
          value = va_arg (args, int);
        fmtint (buffer, &currlen, maxlen, value, 10, min, max, flags);
        break;
      case 'o':
        flags |= DP_F_UNSIGNED;
        if (cflags == DP_C_SHORT)
          value = (unsigned short int) va_arg (args, int);
        else if (cflags == DP_C_LONG)
          value = va_arg (args, unsigned long int);
        else
          value = va_arg (args, unsigned int);
        fmtint (buffer, &currlen, maxlen, value, 8, min, max, flags);
        break;
      case 'u':
        flags |= DP_F_UNSIGNED;
        if (cflags == DP_C_SHORT)
          value = (unsigned short int) va_arg (args, int);
        else if (cflags == DP_C_LONG)
          value = va_arg (args, unsigned long int);
        else
          value = va_arg (args, unsigned int);
        fmtint (buffer, &currlen, maxlen, value, 10, min, max, flags);
        break;
      case 'X':
        flags |= DP_F_UP;
      case 'x':
        flags |= DP_F_UNSIGNED;
        if (cflags == DP_C_SHORT)
          value = (unsigned short int) va_arg (args, int);
        else if (cflags == DP_C_LONG)
          value = va_arg (args, unsigned long int);
        else
          value = va_arg (args, unsigned int);
        fmtint (buffer, &currlen, maxlen, value, 16, min, max, flags);
        break;
      case 'f':
        if (cflags == DP_C_LDOUBLE)
          fvalue = va_arg (args, long double);
        else
          fvalue = va_arg (args, double);
        /* um, floating point? */
        fmtfp (buffer, &currlen, maxlen, fvalue, min, max, flags);
        break;
      case 'E':
        flags |= DP_F_UP;
      case 'e':
        if (cflags == DP_C_LDOUBLE)
          fvalue = va_arg (args, long double);
        else
          fvalue = va_arg (args, double);
        /* um, floating point? */
        fmtefp (buffer, &currlen, maxlen, fvalue, min, max, flags);
        break;
      case 'G':
        flags |= DP_F_UP;
      case 'g':
        if (cflags == DP_C_LDOUBLE)
          fvalue = va_arg (args, long double);
        else
          fvalue = va_arg (args, double);
        /* um, floating point? */
        fmtefp (buffer, &currlen, maxlen, fvalue, min, max, flags);
        break;
      case 'c':
        dopr_outch (buffer, &currlen, maxlen, va_arg (args, int));
        break;
      case 's':
        strvalue = va_arg (args, char *);
        if (max < 0) 
          max = maxlen; /* ie, no max */
        fmtstr (buffer, &currlen, maxlen, strvalue, flags, min, max);
        break;
      case 'p':
        strvalue = va_arg (args, void *);
        fmtint (buffer, &currlen, maxlen, (long) strvalue, 16, min, max, flags);
        break;
      case 'n':
        if (cflags == DP_C_SHORT) 
        {
          short int *num;
          num = va_arg (args, short int *);
          *num = currlen;
        } 
        else if (cflags == DP_C_LONG) 
        {
          long int *num;
          num = va_arg (args, long int *);
          *num = currlen;
        } 
        else 
        {
          int *num;
          num = va_arg (args, int *);
          *num = currlen;
        }
        break;
      case '%':
        dopr_outch (buffer, &currlen, maxlen, ch);
        break;
      case 'w':
        /* not supported yet, treat as next char */
        ch = *format++;
        break;
      default:
        /* Unknown, skip */
        break;
      }
      ch = *format++;
      state = DP_S_DEFAULT;
      flags = cflags = min = 0;
      max = -1;
      break;
    case DP_S_DONE:
      break;
    default:
      /* hmm? */
      break; /* some picky compilers need this */
    }
  }
  if (currlen < maxlen - 1) 
    buffer[currlen] = '\0';
  else 
    buffer[maxlen - 1] = '\0';
}

static void fmtstr (char *buffer, size_t *currlen, size_t maxlen,
                    const char *value, int flags, int min, int max)
{
  int padlen, strln;     /* amount to pad */
  int cnt = 0;
  
  if (value == 0)
  {
    value = "<NULL>";
  }

  for (strln = 0; value[strln]; ++strln); /* strlen */
  padlen = min - strln;
  if (padlen < 0) 
    padlen = 0;
  if (flags & DP_F_MINUS) 
    padlen = -padlen; /* Left Justify */

  while ((padlen > 0) && (cnt < max)) 
  {
    dopr_outch (buffer, currlen, maxlen, ' ');
    --padlen;
    ++cnt;
  }
  while (*value && (cnt < max)) 
  {
    dopr_outch (buffer, currlen, maxlen, *value++);
    ++cnt;
  }
  while ((padlen < 0) && (cnt < max)) 
  {
    dopr_outch (buffer, currlen, maxlen, ' ');
    ++padlen;
    ++cnt;
  }
}

/* Have to handle DP_F_NUM (ie 0x and 0 alternates) */

static void fmtint (char *buffer, size_t *currlen, size_t maxlen,
                    long value, int base, int min, int max, int flags)
{
  int signvalue = 0;
  unsigned long uvalue;
  char convert[20];
  int place = 0;
  int spadlen = 0; /* amount to space pad */
  int zpadlen = 0; /* amount to zero pad */
  int caps = 0;
  
  if (max < 0)
    max = 0;

  uvalue = value;

  if(!(flags & DP_F_UNSIGNED))
  {
    if( value < 0 ) {
      signvalue = '-';
      uvalue = -value;
    }
    else
      if (flags & DP_F_PLUS)  /* Do a sign (+/i) */
        signvalue = '+';
    else
      if (flags & DP_F_SPACE)
        signvalue = ' ';
  }
  
  if (flags & DP_F_UP) caps = 1; /* Should characters be upper case? */

  do {
    convert[place++] =
      (caps ? "0123456789ABCDEF0" : "0123456789abcdef0")
      [uvalue % (unsigned)base];
    uvalue = (uvalue / (unsigned)base );
  } while(uvalue && (place < 20));
  if (place == 20) place--;
  convert[place] = 0;

  zpadlen = max - place;
  spadlen = min - MAX (max, place) - (signvalue ? 1 : 0);
  if (zpadlen < 0) zpadlen = 0;
  if (spadlen < 0) spadlen = 0;
  if (flags & DP_F_ZERO)
  {
    zpadlen = MAX(zpadlen, spadlen);
    spadlen = 0;
  }
  if (flags & DP_F_MINUS) 
    spadlen = -spadlen; /* Left Justifty */

#ifdef DEBUG_SNPRINTF
  dprint (1, (debugfile, "zpad: %d, spad: %d, min: %d, max: %d, place: %d\n",
      zpadlen, spadlen, min, max, place));
#endif

  /* Spaces */
  while (spadlen > 0) 
  {
    dopr_outch (buffer, currlen, maxlen, ' ');
    --spadlen;
  }

  /* Sign */
  if (signvalue) 
    dopr_outch (buffer, currlen, maxlen, signvalue);

  /* Zeros */
  if (zpadlen > 0) 
  {
    while (zpadlen > 0)
    {
      dopr_outch (buffer, currlen, maxlen, '0');
      --zpadlen;
    }
  }

  /* Digits */
  while (place > 0) 
    dopr_outch (buffer, currlen, maxlen, convert[--place]);
  
  /* Left Justified spaces */
  while (spadlen < 0) {
    dopr_outch (buffer, currlen, maxlen, ' ');
    ++spadlen;
  }
}

static long double abs_val (long double value)
{
  long double result = value;

  if (value < 0)
    result = -value;

  return result;
}

static long double mypow10 (int exp)
{
  long double result = 1;

  while (exp)
  {
    result *= 10;
    exp--;
  }
  
  return result;
}

static int default_max = 6;

void set_number_of_digits(int digits)
{
  default_max = digits;
}

static int use_Efloat_format = 1;

void set_Efloat_format(int flag)
{
  use_Efloat_format = flag;
}

static void fmtfp (char *buffer, size_t *currlen, size_t maxlen,
                   long double value, int min, int max, int flags)
{
  int signvalue = 0;
  long double ufvalue = 0;
  char iconvert[20];
  char fconvert[20];
  int iplace = 0;
  int fplace = 0;
  int padlen = 0; /* amount to pad */
  int zpadlen = 0; 
  int caps = 0;
  long intpart = 0;
  long fracpart = 0;
  long old_fracpart = 0;
  int zeros = 0;
  int i = 0;
  long double fvalue = 0;

  fvalue = value;

  /* 
   * AIX manpage says the default is 0, but Solaris says the default
   * is 6, and sprintf on AIX defaults to 6
   */
  if (max < 0) {
//     fprintf(stderr, "setting max to %d\n", default_max);
     max = default_max;
  }

#ifdef HAVE_FPCLASSIFY
   fvalue = check_nan(fvalue);
#endif

  ufvalue = abs_val(fvalue);

  if (fvalue < 0)
    signvalue = '-';
  else
    if (flags & DP_F_PLUS)  /* Do a sign (+/i) */
      signvalue = '+';
    else
      if (flags & DP_F_SPACE)
        signvalue = ' ';

#if 0
  if (flags & DP_F_UP) caps = 1; /* Should characters be upper case? */
#endif

  intpart = 
#ifdef _WIN32
// the free Micro$oft express compiler do some strange float -> int conversions
            floor(
#endif
                  ufvalue
#ifdef _WIN32
                  )
#endif
;

  /* 
   * Sorry, we only support 9 digits past the decimal because of our 
   * conversion method
   */
  if (max > 9)
     max = 9;

  /* We "cheat" by converting the fractional part to integer by
   * multiplying by a factor of 10
   */

#ifdef _WIN32
  fracpart = rint((mypow10(max)) * (ufvalue - intpart));
#else
  fracpart = round((mypow10(max)) * (ufvalue - intpart));  
#endif  

  if (fracpart>0)
     {
     zeros=fabs(floor(log(fracpart / mypow10(max))/log(10))+1);
     if (zeros>=20)
       zeros=20;
     }
  else
     {
     fracpart=0;
     zeros=0;
     }
  old_fracpart=fracpart;

  if (fracpart >= mypow10 (max))
  {
    intpart++;
    fracpart -= mypow10 (max);
  }

#ifdef DEBUG_SNPRINTF
  dprint (1, (debugfile, "fmtfp: %f =? %d.%d\n", fvalue, intpart, fracpart));
#endif

  /* Convert integer part */
  do {
    iconvert[iplace++] = intpart < 0 ? '0' :
      (caps ? "0123456789ABCDEF0" : "0123456789abcdef0")[intpart % 10];
    intpart = (intpart / 10);
  } while(intpart && (iplace < 20));
  if (iplace == 20) iplace--;
  iconvert[iplace] = 0;

  /* Convert fractional part */
  do {
    fconvert[fplace++] =
      (caps ? "0123456789ABCDEF0" : "0123456789abcdef0")[fracpart % 10];
    fracpart = (fracpart / 10);
  } while(fracpart && (fplace-zeros < 20));
  for (i=0;(i<zeros);i++)
     fconvert[fplace++]='0';
  if (fplace == 20) fplace--;
  fconvert[fplace] = 0;

  /* -1 for decimal point, another -1 if we are printing a sign */
  padlen = min - iplace - max - 1 - ((signvalue) ? 1 : 0); 
  zpadlen = max - fplace;
  if (zpadlen < 0)
    zpadlen = 0;
  if (padlen < 0) 
    padlen = 0;
  if (flags & DP_F_MINUS) 
    padlen = -padlen; /* Left Justifty */

  if ((flags & DP_F_ZERO) && (padlen > 0)) 
  {
    if (signvalue) 
    {
      dopr_outch (buffer, currlen, maxlen, signvalue);
      --padlen;
      signvalue = 0;
    }
    while (padlen > 0)
    {
      dopr_outch (buffer, currlen, maxlen, '0');
      --padlen;
    }
  }
  while (padlen > 0)
  {
    dopr_outch (buffer, currlen, maxlen, ' ');
    --padlen;
  }
  if (signvalue) 
    dopr_outch (buffer, currlen, maxlen, signvalue);

  while (iplace > 0) 
    dopr_outch (buffer, currlen, maxlen, iconvert[--iplace]);

  if (old_fracpart!=0) {
     /*
      * Decimal point.  This should probably use locale to find the correct
      * char to print out.
      */
     dopr_outch (buffer, currlen, maxlen, '.');
   
     while (fplace > 0) 
       dopr_outch (buffer, currlen, maxlen, fconvert[--fplace]);
   
     while (zpadlen > 0)
     {
       dopr_outch (buffer, currlen, maxlen, '0');
       --zpadlen;
     }
   
     while (padlen < 0) 
     {
       dopr_outch (buffer, currlen, maxlen, ' ');
       ++padlen;
     }
  }
}


static void dopr_outch (char *buffer, size_t *currlen, size_t maxlen, char c)
{
  if (*currlen < maxlen)
    buffer[(*currlen)++] = c;
}

int myvsnprintf (char *str, size_t count, const char *fmt, va_list args)
{
  str[0] = 0;
  dopr(str, count, fmt, args);
  return(mystrlen(str));
}

/* VARARGS3 */
#ifdef HAVE_STDARGS
int mysnprintf (char *str,size_t count,const char *fmt,...)
#else
int mysnprintf (va_alist) va_dcl
#endif
{
#ifndef HAVE_STDARGS
  char *str = NULL;
  size_t count = 0;
  char *fmt = NULL;
#endif
  VA_LOCAL_DECL;
    
  VA_START (fmt);
  VA_SHIFT (str, char *);
  VA_SHIFT (count, size_t );
  VA_SHIFT (fmt, char *);
  (void) myvsnprintf(str, count, fmt, ap);
  VA_END;
  return(mystrlen(str));
}

#ifndef HAVE_POWL
long double mypowl(long double ten,int exp)
{
  // poor mans powl, only useful if second argument is a integer
  int i;
  long double result = 1;
  for (i = 0; i < exp; i++)
      result *= ten;
  for (i = 0; i > exp; i--)
      result /= ten;
   return result;
}
#endif

#ifdef _WIN32
/* VARARGS3 */
#ifdef HAVE_STDARGS
int __cdecl msgboxprintf (FILE * f,const char *fmt,...)
#else
int __cdecl msgboxprintf (va_alist) va_dcl
#endif
{
#ifndef HAVE_STDARGS
  FILE* f;
  char *fmt;
#endif
  char text[1024];
  VA_LOCAL_DECL;
    
  VA_START (fmt);
  VA_SHIFT (text, char *);
  VA_SHIFT (1024, size_t );
  VA_SHIFT (fmt, char *);
  (void) myvsnprintf(text, 1024, fmt, ap);
  VA_END;
  MessageBox(NULL,text,"white_dune",MB_ICONINFORMATION);
  return(mystrlen(text));
}
#endif

static void fmtefp (char *buffer, size_t *currlen, size_t maxlen,
                    long double evalue, int min, int max, int flags)
{
  long double fvalue = 0;
  int exp = 0;

#ifdef HAVE_FPCLASSIFY
   evalue = check_nan(evalue);
#endif
  
  if (evalue==0)
     dopr_outch(buffer, currlen, maxlen, '0');
  else
     {
     exp=floor(log(fabs(evalue))/log(10.0));
     if (use_Efloat_format && ((exp>2) || (exp<-1)))
        {
#ifdef HAVE_POWL
        fvalue=evalue*powl(10.0,-(long double)exp);  
#else
        fvalue=evalue*mypowl(10.0,-exp);
#endif
        fmtfp (buffer, currlen, maxlen, fvalue, min, max, flags);  
        dopr_outch(buffer, currlen, maxlen, 'e');
        fmtint (buffer,currlen, maxlen,exp,10,min,max,flags);
        }
     else
        fmtfp (buffer, currlen, maxlen, evalue, min, max, flags);
     }
  
}

# define MY_MAX_BUFFER_SIZE 65536

static char buffer[MY_MAX_BUFFER_SIZE];
static int buffersize = 0;
static int current_filedes = -1;

int mywrite(int filedes,const char *str,int size)
  {
  int restlength=size;
  int position_written=0;

  while (restlength>0)
     {
     int writereturn;

     if ( (writereturn = write(filedes,str+position_written,restlength)) < 0 )
        return(-1);
     restlength=restlength-writereturn;
     position_written=position_written+writereturn;
     }
  return(0);
  }

static int myflushbuffer(void)
  {
  int ret = 0;
  if (buffersize > 0)
      ret=mywrite(current_filedes,buffer,buffersize);
  buffersize=0;
  current_filedes=-1;
  return ret;
  }

int myflushall(void)
  {
  return myflushbuffer();
  }

int myclose(int filedes)
  {
  int error=0;
  if (myflushbuffer() < 0) {
     error=-1;
  }
  buffersize=0;
  current_filedes=-1;
  close(filedes);
  return error;
  }

#ifdef DEBUG
/* write unbuffered */

int mywritestr(int filedes,const char *str)
  {
  if (mywrite(filedes,str,strlen(str)) < 0)
     return -1;
  return 0;
  }
#else
/* write via a buffer to improve performance */

int mywritestr(int filedes,const char *str)
  {
  int len=strlen(str);
  if (len==0)
     return 0;
  if (len > MY_MAX_BUFFER_SIZE) 
     {
     if (myflushbuffer() < 0)
        return -1;
     if (mywrite(filedes,str,len) < 0)
        return -1;
     return 0;
     }
  if (((current_filedes != -1) && (current_filedes != filedes)) ||
      ((buffersize+len) >= MY_MAX_BUFFER_SIZE)) 
     {
     if (myflushbuffer() < 0)
        return -1;
     } 
  current_filedes = filedes;
  memcpy(buffer+buffersize, str, len);
  buffersize+=len;
  return 0;
  }
#endif


/* VARARGS3 */
#ifdef HAVE_STDARGS
int mywritef (int filedes,const char *fmt,...)
#else
int mywritef (va_alist) va_dcl
#endif
{ 
  char str[8000];
#ifndef HAVE_STDARGS
  int filedes;
  char *fmt;
#endif
  VA_LOCAL_DECL;
    
  VA_START (fmt);
  VA_SHIFT (filedes, int);
  VA_SHIFT (fmt, char *);
  (void) myvsnprintf(str, 8000, fmt, ap);
  VA_END;
  /* error, myvsnprintf possibly truncated data */
  if (mystrlen(str)>=7999)
     return(-2);
  return mywritestr(filedes,str);
}

/*
 * strncpy that copies n bytes and can write zero to n+1-th byte
 * use this when you want to copy exact n bytes (or less)
 */
char* mystrncpy_danger(char* str1,const char *str2,int n)
   {
   char* ret;
   ret = strncpy(str1,str2,n);
   ret[n] = 0;
   return ret;
   }

/*
 * strncpy that copies n bytes and then write zero to n-th byte
 * use this when you want to copy less than n bytes
 */
char* mystrncpy_secure(char* str1,const char *str2,int n)
   {
   char* ret;
   ret = strncpy(str1,str2,n);
   ret[n-1] = 0;
   return ret;
   }

/*
 * special version of strdup, that explicitly calls malloc 
 * this is needed to allow ld -r linking of the efence library 
 */
char *mystrdup(const char *s)
{
    char *ret = NULL;
    if (s == NULL) {
        ret = malloc(1);
        ret[0] = 0;
        return ret;    
    }        
    ret = malloc(strlen(s) + 1);
    strcpy(ret, s);
    return ret;
}

#ifdef TEST_SNPRINTF
#ifndef LONG_STRING
#define LONG_STRING 1024
#endif
#include <stdio.h>
int main (void)
{
  char buf1[LONG_STRING];
  char buf2[LONG_STRING];
  char *fp_fmt[] = {
    "%-1.5f",
    "%g",
    "%f",
    "%-1.5f",
    "%1.5f",
    "%123.9f",
    "%10.5f",
    "% 10.5f",
    "%+22.9f",
    "%+4.9f",
    "%01.3f",
    "%4f",
    "%3.1f",
    "%3.2f",
    NULL
  };
  float test_fp_nums[] = { -1, 1, 0.5, 0102.1, 1.0032, 203.9,-1.5, 134.21, 91340.2, 341.1234, 0203.9, 0.96, 0.996, 
    0.9996, 1.996, 1.003, 4.136, 3e-2, 0};
  char *int_fmt[] = {
    "%-1.5d",
    "%1.5d",
    "%123.9d",
    "%5.5d",
    "%10.5d",
    "% 10.5d",
    "%+22.33d",
    "%01.3d",
    "%4d",
    NULL
  };
  long int_nums[] = { -1, 134, 91340, 341, 0203, 0};
  int x, y;
  int fail = 0;
  int num = 0;

  printf ("Testing snprintf format codes against system sprintf...\n");

  for (x = 0; fp_fmt[x] != NULL ; x++)
    {
    for (y = 0; test_fp_nums[y] != 0 ; y++)
    {
      mysnprintf (buf1, sizeof (buf1), fp_fmt[x], test_fp_nums[y]);
      sprintf (buf2, fp_fmt[x], test_fp_nums[y]);
      if (strcmp (buf1, buf2))
      {
        printf("snprintf %lf doesn't match Format: %s\n\tsnprintf = %s\n\tsprintf  = %s\n", 
            test_fp_nums[y],fp_fmt[x], buf1, buf2);
        fail++;
      }
    }
   mysnprintf (buf1, sizeof (buf1), fp_fmt[x],0);
   sprintf (buf2, fp_fmt[x], 0);
   if (strcmp (buf1, buf2))
   {
      printf("snprintf %lf doesn't match Format: %s\n\tsnprintf = %s\n\tsprintf  = %s\n", 
      test_fp_nums[y],fp_fmt[x], buf1, buf2);
      fail++;
   }
   num++;
  }

  for (x = 0; int_fmt[x] != NULL ; x++)
    for (y = 0; int_nums[y] != 0 ; y++)
    {
      mysnprintf (buf1, sizeof (buf1), int_fmt[x], int_nums[y]);
      sprintf (buf2, int_fmt[x], int_nums[y]);
      if (strcmp (buf1, buf2))
      {
        printf("snprintf doesn't match Format: %s\n\tsnprintf = %s\n\tsprintf  = %s\n", 
            int_fmt[x], buf1, buf2);
        fail++;
      }
      num++;
    }
  printf ("%d tests failed out of %d.\n", fail, num);
}
#endif /* SNPRINTF_TEST */

