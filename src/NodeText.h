/*
 * NodeText.h
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

#ifndef _NODE_TEXT_H
#define _NODE_TEXT_H

#ifndef _GEOMETRY_NODE_H
#include "GeometryNode.h"
#endif
#ifndef _PROTO_MACROS_H
#include "ProtoMacros.h"
#endif
#ifndef _PROTO_H
#include "Proto.h"
#endif

#include "SFMFTypes.h"
#ifdef HAVE_LIBFREETYPE
# include "MeshBasedNode.h"
#endif

class ProtoText : public GeometryProto {
public:
                    ProtoText(Scene *scene);
                    ProtoText(Scene *scene, const char *name);
    void            addElements(void);
    virtual Node   *create(Scene *scene);

    virtual int     getType() const { return VRML_TEXT; }

//    virtual bool    isDeclaredInRwd_h() { return true; } 

#ifdef HAVE_LIBFREETYPE
    virtual bool    isMesh(void) { return true; }
    virtual bool    isExportTargetMesh(void) { return true; }
#endif

    FieldIndex fontStyle;
    FieldIndex length;
    FieldIndex maxExtent;
    FieldIndex string;
    FieldIndex solid;
    FieldIndex texCoord;
    x3domGeometryCommonFieldIndex()
    FieldIndex ccw;
};

#ifdef HAVE_LIBFREETYPE
class NodeText : public MeshBasedNode {
#else
class NodeText : public GeometryNode {
#endif
public:
                    NodeText(Scene *scene, Proto *proto);

    virtual int     getProfile(void) const { return PROFILE_IMMERSIVE; }
    virtual int     getX3dVersion(void) const { return 0; }
    virtual Node *copy() { return new NodeText( m_scene, m_proto); }

#ifdef HAVE_LIBFREETYPE
    virtual void    draw() { meshDraw(); }
#else
    virtual void    draw();
#endif

    void            update(void) 
                        { 
#ifdef HAVE_LIBFREETYPE
                        m_meshDirty = true;
#endif
                        m_textDirty = true;  
                        }
    void            reInit(void);

    virtual bool    isInvalidChildNode(void) { return true; }

#ifdef HAVE_LIBFREETYPE
    virtual void    createMesh(bool cleanDoubleVertices = true,
                               bool triangulateMesh = true);
    void            setField(int index, FieldValue *value, int cf = -1);
    virtual float   getDepth(void) { return 0; }
#endif

    fieldMacros(SFNode,   fontStyle, ProtoText)
    fieldMacros(MFFloat,  length,    ProtoText)
    fieldMacros(MFString, string,    ProtoText)
    fieldMacros(SFFloat,  maxExtent, ProtoText)
    fieldMacros(SFBool,   solid,     ProtoText)
    fieldMacros(SFNode,   texCoord,  ProtoText)
    x3domGeometryCommonFieldMacros(ProtoText) 
    fieldMacros(SFBool,  ccw,        ProtoText)
protected:
    void            cleanText(void);
protected:
    MFString       *m_deBackslashedStrings;
    bool            m_textDirty;
};

#endif // _NODE_TEXT_H
