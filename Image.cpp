/*
 * Image.cpp
 *
 * Copyright (C) 1999 Stephen F. White
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

#include "stdafx.h"
#include "Image.h"
#ifdef HAVE_GIF
# include "gif.h"
#endif
#include "jpegLoad.h"
#include "pngLoad.h"
#include <string.h>
#include <ctype.h>

Image::Image()
{
}

Image::~Image()
{
}

char* Image::file_extension(const char *filename)
   {
   /* identify file extension by '.' */
   int i;
   for (i=strlen(filename);(i>=0) && (filename[i]!='.');i--) {}
   return (char*)filename+i;       
   }

int Image::test_file_extension(char *extension)
   {
   /* copy extension and convert to lowercase */
   char case_extension[5];
   mystrncpy_secure(case_extension,extension+1,5);
   for (unsigned int i=0;i<=strlen(case_extension);i++)
      case_extension[i]=tolower(case_extension[i]);
   if (strcasecmp(case_extension,"jpeg")==0)
      return((int)JPEG);
   else if (strcasecmp(case_extension,"jfif")==0)
      return((int)JPEG);
   else if (strcasecmp(case_extension,"jpg")==0)
      return((int)JPEG);
#ifdef HAVE_GIF
   else if (strcasecmp(case_extension,"gif")==0)
      return((int)GIF);
#endif
   else if (strcasecmp(case_extension,"png")==0)
      return((int)PNG);
   else 
      return(-1);
   }

bool
Image::Open(const char *filename)
{
#ifdef HAVE_LIBDEVIL
    return true;
#else
    m_type=(Type)test_file_extension(file_extension(filename));
    switch (m_type)
       {
       default: /* no known fileextension, test all imagetypes */
#ifdef HAVE_GIF
       case GIF:
          if (gifOpen(filename, &m_width, &m_height)) 
             {
             m_type = GIF;
             m_components = 3;
             return true;
             }
#endif
        case JPEG:
           if (jpegOpen(filename, &m_width, &m_height, &m_components)) 
              {
              m_type = JPEG;
              return true;
              }
        case PNG:
           if (pngOpen(filename, &m_width, &m_height, &m_components)) 
              {
              m_type = PNG;
              return true;
              }
       }
    return false;
#endif
}

void
Image::Read(unsigned char *data)
{
    switch (m_type) {
#ifdef HAVE_GIF
      case GIF:
        gifRead(data);
        break;
#endif
      case JPEG:
        jpegRead(data);
        break;
      case PNG:
        pngRead(data);
        break;
//    default:
//      assert(0);
    }
}
