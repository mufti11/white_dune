
 /*
 * TransformMode.cpp
 *
 * Copyright (C) 2002 J. "MUFTI" Scheurich
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

#include "TransformMode.h"

bool TransformMode::hasRotation(void)
   {
   if (
       (tmode==TM_ROTATE)  ||
       (tmode==TM_6D)      ||    
       (tmode==TM_6DLOCAL) ||  
       (tmode==TM_ROCKET)  || 
       (tmode==TM_HOVER)
      )
      return true;
   return false;
   }

bool TransformMode::hasTranslation(void)
   {
   if (
       (tmode==TM_TRANSLATE) || 
       (tmode==TM_6D)        ||         
       (tmode==TM_6DLOCAL)   ||    
       (tmode==TM_ROCKET)    ||    
       (tmode==TM_HOVER)
      )     
      return true;
   return false;
   }

bool TransformMode::isLocal(void)
   {
   if (
       (tmode==TM_6DLOCAL) || 
       (tmode==TM_ROCKET)  ||   
       (tmode==TM_HOVER)
      )   
      return true;
   return false;
   }

