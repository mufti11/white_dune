/*
 * Colored.h
 *
 * Copyright (C) 2013 J. "MUFTI" Scheurich
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

#include "GeometryNode.h"
#include "NodeMaterial.h"
#include "NodeColor.h"
#include "NodeColorRGBA.h"

class Colored {
public:
    virtual NodeColor *getColorNode() = 0;
    virtual NodeColorRGBA *getColorRGBANode() = 0;
    virtual NodeCoordinate *getCoordinateNode(void) = 0;
    virtual int     colorPerVertexField() const = 0;
    virtual int     colorIndexField() const = 0;

    NodeMaterial   *getMaterialNode(GeometryNode *node)
                       { return node->getMaterialNode(); }
    bool            hasColor(void) { return true; }
};
