/*
 * FaceData.h
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

#pragma once

#include "Vec3f.h"

enum {
    FACE_TOP,
    FACE_FRONT,
    FACE_BACK,
    FACE_RIGHT,
    FACE_LEFT,
    FACE_BOTTOM
};

class FaceData {
public:
                FaceData(int numVertices, int offset);

    int         getNumVertices() const          { return m_numVertices; }
    int         getOffset() const               { return m_offset; }
    const Vec3f&getNormal() const               { return m_normal; }
    void        setNormal(const Vec3f &normal)  { m_normal = normal; }
    float       getMinZ() const                 { return m_minZ; }
    float       getMaxZ() const                 { return m_maxZ; }
    void        setMinZ(float minZ)             { m_minZ = minZ; }
    void        setMaxZ(float maxZ)             { m_maxZ = maxZ; }
    int         getType(void)                   { return m_type; }
    void        setType(int type)               { m_type = type; }

private:
    int         m_numVertices;
    int         m_offset;
    Vec3f       m_normal;
    float       m_minZ, m_maxZ;
    int         m_type;
};

