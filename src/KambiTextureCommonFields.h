/*
 * KambiTextureCommonFields.h
 *
 * Copyright (C) 2011 J. "MUFTI" Scheurich
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

#define kambiTextureCommonFieldIndex() \
    FieldIndex effects;


#define kambiTextureCommonFieldMacros(proto) \
    fieldMacros(MFNode,  effects,          proto)

#define kambiTextureCommonFields() \
    effects.set( \
          addField(MFNODE, "effects", new MFNode(), KAMBI_EFFECT)); \
    setFieldFlags(effects, FF_KAMBI_ONLY);


#define kambiVrmlTextureCommonFieldIndex() \
    FieldIndex alphaChannel;

#define kambiVrmlTextureCommonFieldMacros(proto) \
    fieldMacros(SFString,  alphaChannel,          proto)

#define kambiVrmlTextureCommonFields() \
    alphaChannel.set( \
          addField(SFSTRING, "alphaChannel", new SFString(), \
                   new SFString("AUTO"))); \
    setFieldFlags(alphaChannel, FF_KAMBI_ONLY);
