/*
 * NodeMatrixTransform.h
 *
 * Copyright (C) 2009 J. "MUFTI" Scheurich
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

#include "Node.h"
#include "ProtoMacros.h"
#include "Proto.h"
#include "SFMFTypes.h"
#include "KambiCommonFunctions.h"

class ProtoMatrixTransform : public Proto {
public:
                    ProtoMatrixTransform(Scene *scene);
    virtual Node   *create(Scene *scene);

    virtual int     getType() const { return KAMBI_MATRIX_TRANSFORM; }

    FieldIndex matrix;
    FieldIndex invert;
    FieldIndex showProxyGeometry;
    FieldIndex children;
    FieldIndex bboxCenter;
    FieldIndex bboxSize;
    FieldIndex render;
    FieldIndex showBBox;
    FieldIndex logFeature;

    virtual bool    isKambiProto(void) { return true; }
};

class NodeMatrixTransform : public Node {
public:
                    NodeMatrixTransform(Scene *scene, Proto *proto);

    virtual Node   *copy() const { return new NodeMatrixTransform(*this); }

    kambiCommonFunctions()

    fieldMacros(SFMatrix4f, matrix, ProtoMatrixTransform);
    fieldMacros(SFBool, invert, ProtoMatrixTransform);
    fieldMacros(SFBool, showProxyGeometry, ProtoMatrixTransform);
    fieldMacros(MFNode, children, ProtoMatrixTransform);
    fieldMacros(SFVec3f, bboxCenter, ProtoMatrixTransform);
    fieldMacros(SFVec3f, bboxSize, ProtoMatrixTransform);
    fieldMacros(SFBool, render, ProtoMatrixTransform);
    fieldMacros(SFBool, showBBox, ProtoMatrixTransform);
    fieldMacros(MFString, logFeature, ProtoMatrixTransform);
};

