/*
 * KambiCommonFunctions.h
 *
 * 2008 J. "MUFTI" Scheurich
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
 * 
 * Implements common functions for nonstandard extensions of the 
 * Kambi vrml game engine
 */

#ifndef _KAMBI_COMMON_FUNCTIONS_H
#define _KAMBI_COMMON_FUNCTIONS_H

#define kambiCommonFunctions() \
    virtual int writeProto(int filedes) \
    { \
        return ((Node *)this)->writeProto(filedes, \
                                          "urn:vrmlengine.sourceforge.net:node", \
                                          "KambiNodes", \
                                          "http://vrmlengine.sourceforge.net/fallback_prototypes.wrl", \
                                          false); \
    } \
    virtual bool isKambiNode(void) { return true; } \
    virtual int  getX3dVersion(void) const { return -1; }


#endif 

