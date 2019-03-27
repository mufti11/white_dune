/********************************************************************
** @source JEEPS functions
**
** @author Copyright (C) 1999 Alan Bleasby
** @version 1.0 
** @modified Feb 04 2000 Alan Bleasby. First version
** @@
** 
** This library is free software; you can redistribute it and/or
** modify it under the terms of the GNU Library General Public
** License as published by the Free Software Foundation; either
** version 2 of the License, or (at your option) any later version.
** 
** This library is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
** Library General Public License for more details.
** 
** You should have received a copy of the GNU Library General Public
** License along with this library; if not, write to the
** Free Software Foundation, Inc., 59 Temple Place - Suite 330,
** Boston, MA  02111-1307, USA.
********************************************************************/

/*
 *  For portability any '32' type must be 32 bits
 *                  and '16' type must be 16 bits
 */
typedef unsigned char UC;
typedef short int16;
typedef unsigned short uint16;
typedef uint16 US;

#if defined(__alpha)
typedef int int32;
typedef unsigned int uint32;
#else
typedef long int32;
typedef unsigned long uint32;
#endif
