/*
 * SFMFTypesItem.h 
 *
 * Copyright (C) 1999 Stephen F. White
 * Copyright (C) 2003 J. "MUFTI" Scheurich
 *
 * automatic (via configure) generated file, direct change is useless.
 * change batch/mkSFMFTypes.sh instead
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

#ifndef _SFMFTYPES_ITEM_H
#define _SFMFTYPES_ITEM_H

#include "MFBoolItem.h"
#include "MFColorItem.h"
#include "MFColorRGBAItem.h"
#include "MFDoubleItem.h"
#include "MFFloatItem.h"
#include "MFInt32Item.h"
#include "MFMatrix3fItem.h"
#include "MFMatrix4fItem.h"
#include "MFNodeItem.h"
#include "MFRotationItem.h"
#include "MFStringItem.h"
#include "MFTimeItem.h"
#include "MFVec2dItem.h"
#include "MFVec2fItem.h"
#include "MFVec3dItem.h"
#include "MFVec3fItem.h"
#include "MFVec4fItem.h"
#include "SFBoolItem.h"
#include "SFColorItem.h"
#include "SFColorRGBAItem.h"
#include "SFDoubleItem.h"
#include "SFFloatItem.h"
#include "SFImageItem.h"
#include "SFInt32Item.h"
#include "SFMatrix3fItem.h"
#include "SFMatrix4fItem.h"
#include "SFNodeItem.h"
#include "SFRotationItem.h"
#include "SFStringItem.h"
#include "SFTimeItem.h"
#include "SFVec2dItem.h"
#include "SFVec2fItem.h"
#include "SFVec3dItem.h"
#include "SFVec3fItem.h"
#include "SFVec4fItem.h"

#define CreateFieldViewItem(ret, type, view) \
    switch (type) { \
     case MFBOOL: \
       ret = new MFBoolItem(view);\
       break;\
     case MFCOLOR: \
       ret = new MFColorItem(view);\
       break;\
     case MFCOLORRGBA: \
       ret = new MFColorRGBAItem(view);\
       break;\
     case MFDOUBLE: \
       ret = new MFDoubleItem(view);\
       break;\
     case MFFLOAT: \
       ret = new MFFloatItem(view);\
       break;\
     case MFINT32: \
       ret = new MFInt32Item(view);\
       break;\
     case MFMATRIX3F: \
       ret = new MFMatrix3fItem(view);\
       break;\
     case MFMATRIX4F: \
       ret = new MFMatrix4fItem(view);\
       break;\
     case MFNODE: \
       ret = new MFNodeItem(view);\
       break;\
     case MFROTATION: \
       ret = new MFRotationItem(view);\
       break;\
     case MFSTRING: \
       ret = new MFStringItem(view);\
       break;\
     case MFTIME: \
       ret = new MFTimeItem(view);\
       break;\
     case MFVEC2D: \
       ret = new MFVec2dItem(view);\
       break;\
     case MFVEC2F: \
       ret = new MFVec2fItem(view);\
       break;\
     case MFVEC3D: \
       ret = new MFVec3dItem(view);\
       break;\
     case MFVEC3F: \
       ret = new MFVec3fItem(view);\
       break;\
     case MFVEC4F: \
       ret = new MFVec4fItem(view);\
       break;\
     case SFBOOL: \
       ret = new SFBoolItem(view);\
       break;\
     case SFCOLOR: \
       ret = new SFColorItem(view);\
       break;\
     case SFCOLORRGBA: \
       ret = new SFColorRGBAItem(view);\
       break;\
     case SFDOUBLE: \
       ret = new SFDoubleItem(view);\
       break;\
     case SFFLOAT: \
       ret = new SFFloatItem(view);\
       break;\
     case SFIMAGE: \
       ret = new SFImageItem(view);\
       break;\
     case SFINT32: \
       ret = new SFInt32Item(view);\
       break;\
     case SFMATRIX3F: \
       ret = new SFMatrix3fItem(view);\
       break;\
     case SFMATRIX4F: \
       ret = new SFMatrix4fItem(view);\
       break;\
     case SFNODE: \
       ret = new SFNodeItem(view);\
       break;\
     case SFROTATION: \
       ret = new SFRotationItem(view);\
       break;\
     case SFSTRING: \
       ret = new SFStringItem(view);\
       break;\
     case SFTIME: \
       ret = new SFTimeItem(view);\
       break;\
     case SFVEC2D: \
       ret = new SFVec2dItem(view);\
       break;\
     case SFVEC2F: \
       ret = new SFVec2fItem(view);\
       break;\
     case SFVEC3D: \
       ret = new SFVec3dItem(view);\
       break;\
     case SFVEC3F: \
       ret = new SFVec3fItem(view);\
       break;\
     case SFVEC4F: \
       ret = new SFVec4fItem(view);\
       break;\
      default: \
        ret = new FieldViewItem(view); \
        break; \
    }

#endif // _SFMFTYPES_ITEM_H

