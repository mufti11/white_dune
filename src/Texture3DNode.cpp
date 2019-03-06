/*
 * Texture3DNode.cpp
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

#include <stdio.h>
#include "stdafx.h"
#include "GL/gl.h"

#include "NodeImageTexture3D.h"
#include "NodeViewpoint.h"
#include "NodeTransform.h"
#include "Proto.h"
#include "FieldValue.h"
#include "MFString.h"
#include "SFBool.h"
#include "SFNode.h"
#include "DuneApp.h"
#include "Scene.h"
#include "MyMesh.h"
#include "Util.h"
#include "NodeVolumeData.h"

Texture3DNode::Texture3DNode()
{
    m_imageData = NULL;                      
    m_imageSizeX = 0;
    m_imageSizeY = 0;
    m_imageSizeZ = 0;
    m_loadedTexture = false;
    m_gTex3D = 0;
}

/* the following use code from vx 
   https://github.com/neurolabusc/vx/blob/master/src/tex3d.inc

****************************************************************************
This is a BSD 3-Clause License :: https://opensource.org/licenses/BSD-3-Clause

The Software has been developed for research purposes only and is not a clinical tool
Copyright (c) 2014-2018 Chris Rorden. All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions
are met:
1. Redistributions of source code must retain the above copyright
   notice, this list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright
   notice, this list of conditions and the following disclaimer in the
   documentation and/or other materials provided with the distribution.
3. The name of the copyright owner may be used to endorse or promote
   products derived from this software without specific prior written
   permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT OWNER ``AS IS'' AND ANY EXPRESS OR
IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO
EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
****************************************************************************/

void
Texture3DNode::loadTexture()
{
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

        glGenTextures(1, &m_gTex3D);
        glBindTexture(GL_TEXTURE_3D, m_gTex3D);
        glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_BASE_LEVEL, 0);
        glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAX_LEVEL, 0);
        glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_BORDER);
        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
        glTexImage3D(GL_TEXTURE_3D, 0, GL_RGBA8, 
                     m_imageSizeX, m_imageSizeY, m_imageSizeZ ,0, GL_RGBA, 
                     GL_UNSIGNED_BYTE, m_imageData);

        GLenum error=glGetError();
        if (error != 0)
            fprintf(stderr,"GL Error: %s\n",gluErrorString(error));
}

void
Texture3DNode::drawQuads(Node *node)
{
    NodeVolumeData *vol = (NodeVolumeData *)node->getParent();
    if (vol->getType() != X3D_VOLUME_DATA)
        return;

    const float *dim = vol->dimensions()->getValue();

    if (dim[2] == 0)
        return;

    float size = 4;
    for (int i = 0; i < m_imageSizeZ; i++) {
        float tz = -dim[2] / 2 + i * dim[2] / (m_imageSizeZ - 1);
        glBegin(GL_QUADS);
        glTexCoord3f(-1.0f, -1.0f, tz);
        glVertex3f(-dim[0] * size, -dim[1] * size, tz);
        glTexCoord3f( 1.0f, -1.0f, tz);
        glVertex3f( dim[0] * size, -dim[1] * size, tz);
        glTexCoord3f( 1.0f,  1.0f, tz);
        glVertex3f( dim[0] * size,  dim[1] * size, tz);
        glTexCoord3f(-1.0f,  1.0f, tz);
        glVertex3f(-dim[0] * size,  dim[1] * size, tz);
        glEnd();
    }
}

void
Texture3DNode::preDraw()
{
    if (!m_loadedTexture) {
        loadTexture();
        m_loadedTexture = true;
    }
}

void
Texture3DNode::drawTexture3D(Node *node, Scene *scene)
{
    if (m_imageData == NULL)
        return;

    glEnable(GL_ALPHA_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glAlphaFunc(GL_NOTEQUAL, 0);

    glEnable(GL_TEXTURE_3D);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glBindTexture(GL_TEXTURE_3D, m_gTex3D);

     glPushMatrix();
  
     glMatrixMode(GL_MODELVIEW);
     Quaternion quat = scene->getCamera()->getOrientation();
     SFRotation rot(quat);
     const float *frotation = rot.getValue();
     glRotatef(RAD2DEG(frotation[3]),
               frotation[0], frotation[1], frotation[2]);
     Vec3f pos = scene->getCamera()->getPosition();
//     glTranslatef(-pos.x, -pos.y, -pos.z);
//     glScalef(2, 2, 2);

     glMatrixMode(GL_TEXTURE);
     glLoadIdentity();
     glScalef(2, 2, 2);
     glRotatef(RAD2DEG(frotation[3]),
               frotation[0], frotation[1], frotation[2]);

      drawQuads(node);

      glMatrixMode(GL_MODELVIEW);
      glPopMatrix();

      glDisable(GL_BLEND);
      glBindTexture(GL_TEXTURE_3D, 0);
}
