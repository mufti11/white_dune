/*
 * GeometryNodeMacros.h
 *
 * Copyright (C) 1999 Stephen F. White, 2004 J. "MUFTI" Scheurich
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

class NodeCoordinate;

#define GeometryNodeDeclarationMacros()               \
public:                                         \
    NodeAppearance       *getAppearanceNode(void);    \
    NodeMaterial         *getMaterialNode(void);        \
    NodeImageTexture     *getImageTextureNode(void);      \
    NodeTextureTransform *getTextureTransformNode(void); 


#define GeometryNodeMacros(SelfNode)  \
NodeAppearance * \
SelfNode::getAppearanceNode(void) \
{ \
    Node *parent = NULL; \
    if (hasParent()) \
        parent = getParent(); \
 \
    NodeShape *nshape = NULL; \
    if (parent) \
        if (parent->getType() == VRML_SHAPE) \
            nshape = (NodeShape *) parent; \
 \
    NodeAppearance *nappearance = NULL; \
    if (nshape) \
        nappearance = (NodeAppearance *)(nshape->appearance()->getValue()); \
    return nappearance; \
} \
 \
NodeMaterial * \
SelfNode::getMaterialNode(void) \
{ \
    NodeAppearance *nappearance = getAppearanceNode(); \
 \
    NodeMaterial *nmaterial = NULL; \
    if (nappearance) \
        nmaterial = (NodeMaterial *)nappearance->material()->getValue(); \
    return nmaterial; \
} \
 \
 \
NodeImageTexture * \
SelfNode::getImageTextureNode(void) \
{ \
    NodeAppearance *nappearance = getAppearanceNode(); \
 \
    Node *ntexture = NULL; \
    NodeImageTexture *nimageTexture = NULL; \
    if (nappearance) { \
        ntexture = nappearance->texture()->getValue(); \
        if (ntexture) \
            if (ntexture->getType() == VRML_IMAGE_TEXTURE) \
                nimageTexture = (NodeImageTexture *)ntexture; \
    } \
    return nimageTexture; \
} \
 \
NodeTextureTransform * \
SelfNode::getTextureTransformNode(void) \
{ \
    NodeAppearance *nappearance = getAppearanceNode(); \
 \
    NodeTextureTransform *ntextureTransfrom = NULL; \
    if (nappearance) \
        ntextureTransfrom = (NodeTextureTransform  *) \
                            nappearance->textureTransform()->getValue(); \
    return ntextureTransfrom; \
}
