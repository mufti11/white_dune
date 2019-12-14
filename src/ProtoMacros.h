/*
 * ProtoMacros.h
 *
 * Copyright (C) 2003 J. "MUFTI" Scheurich
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

#pragma once

// the following macro will produce 4 functions: 
//
//  int   FIELDNAME_Field(void) to get the Index of the FieldValue
//  type& FIELDNAME(void)       to get the FieldValue
//  void  FIELDNAME(type *)     to set the FieldValue via a pointer
//  void  FIELDNAME(type &)     to set the FieldValue via a pointer
 //
// this functions are used inside a ProtoNODENAME class
// to define a easy and secure access fields of a node 
// (the orginal dune version used constant integer numbers e.g. 5 to access)

// example:
// to access the FIELDNAME "center" (type SFVec3f) of Node Transform,
// add 
// 
//    "FieldIndex  center;" 
//
// to ProtoTransform and initalise it with something like
//
//    center.set(addExposedField(SFVEC3F, "center" 
//
// in ProtoTransform::ProtoTransform. Then add 
// 
//    Field(SFVec3f, ProtoTransform, center)
// 
// to the NodeTransform class, to create the functions
//
//    SFVec3f &center(void)
//    void center(SFVec3f *) 
//    void center(SFVec3f &) 
//

#define fieldMacros(type, what, proto) \
 /** field "what" */ \
 type *what(void) const \
 { \
     return ((type *) getField(((proto *)getPrimaryProto())->what)); \
 } \
 void what(type *value) \
 { \
     setField(((proto *)getPrimaryProto())->what, value); \
 } \
 void what(type &value) \
 { \
     setField(((proto *)getPrimaryProto())->what, &value); \
 } \
 int what##_Field(void) const \
 { \
     return ((proto *)getPrimaryProto())->what; \
 }

