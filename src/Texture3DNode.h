/*
 * Texture3DNode.h
 *
 * Copyright (C) 2019 J. "MUFTI" Scheurich
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

class Texture3DNode {
public:
    Texture3DNode();

    void          loadTexture();
    void          drawQuads(Node *node);
    virtual void  preDraw();
    void          drawTexture3D(Node *node, Scene *scene);
protected:
    int          *m_imageData;
    int           m_imageSizeX;
    int           m_imageSizeY;
    int           m_imageSizeZ;
    bool          m_loadedTexture;
#ifdef HAVE_TEXTURE_3D
    GLuint        m_gTex3D;
#else
    MyArray<GLuint> m_gTex2Ds;
#endif
};

