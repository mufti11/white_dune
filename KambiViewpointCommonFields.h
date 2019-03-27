/*
 * KambiViewpointCommonFields.h
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

#ifndef _KAMBI_Viewpoint_COMMON_FIELDS_H
#define _KAMBI_Viewpoint_COMMON_FIELDS_H

#define kambiViewpointCommonFieldIndex() \
    FieldIndex cameraMatrixSendAlsoOnOffscreenRendering;


#define kambiViewpointCommonFieldMacros(proto) \
    fieldMacros(SFBool, cameraMatrixSendAlsoOnOffscreenRendering, proto)

#define kambiViewpointCommonFields() \
    cameraMatrixSendAlsoOnOffscreenRendering.set( \
        addExposedField(SFBOOL, "cameraMatrixSendAlsoOnOffscreenRendering", \
                        new SFBool(false))); \
    setFieldFlags(cameraMatrixSendAlsoOnOffscreenRendering, FF_KAMBI_ONLY); \
    addEventOut(SFMATRIX4F, "cameraMatrix", FF_KAMBI_ONLY); \
    addEventOut(SFMATRIX4F, "cameraInverseMatrix", FF_KAMBI_ONLY); \
    addEventOut(SFMATRIX3F, "cameraRotationMatrix", FF_KAMBI_ONLY); \
    addEventOut(SFMATRIX3F, "cameraRotationInverseMatrix", FF_KAMBI_ONLY);
#endif

