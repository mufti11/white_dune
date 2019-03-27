/*
 * NodeMultiTextureTransform.cpp
 *
 * Copyright (C) 1999 Stephen F. White, 2008 J. "MUFTI" Scheurich
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

#include "NodeMultiTextureTransform.h"
#include "Proto.h"
#include "FieldValue.h"
#include "MFNode.h"

ProtoMultiTextureTransform::ProtoMultiTextureTransform(Scene *scene)
  : Proto(scene, "MultiTextureTransform")
{
    textureTransform.set(addExposedField(MFNODE, "textureTransform", 
                                         new MFNode(), 
                                         TEXTURE_TRANSFORM_NODE | 
                                         NOT_SELF_NODE));
}

Node *
ProtoMultiTextureTransform::create(Scene *scene)
{ 
    return new NodeMultiTextureTransform(scene, this); 
}

NodeMultiTextureTransform::NodeMultiTextureTransform(Scene *scene, Proto *def)
  : Node(scene, def)
{
}

void
NodeMultiTextureTransform::setField(int index, FieldValue *value, int cf)
{
    Node::setField(index, value, cf);
    if (hasParent())
        getParent()->update();
}

void
NodeMultiTextureTransform::bind()
{
#ifdef HAVE_MULTI_TEXTURE
    for (int i = 0; i < textureTransform()->getSize(); i++) {
        Node *texTrans = textureTransform()->getValue(i);
        if (texTrans->getType() == VRML_TEXTURE_TRANSFORM) {
            glActiveTexture(GL_TEXTURE0 + i);
            texTrans->bind();
        }
    }
#endif
}

void
NodeMultiTextureTransform::unbind()
{
#ifdef HAVE_MULTI_TEXTURE
    for (int i = 0; i < textureTransform()->getSize(); i++) {
        Node *texTrans = textureTransform()->getValue(i);
        if (texTrans->getType() == VRML_TEXTURE_TRANSFORM) {
            glActiveTexture(GL_TEXTURE0 + i);
            texTrans->unbind();
        }
    }
    glActiveTexture(GL_TEXTURE0);
#endif
}

