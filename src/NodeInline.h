/*
 * NodeInline.h
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

#include "Node.h"
#include "ProtoMacros.h"
#include "Proto.h"
#include "SFMFTypes.h"

class NodeList;

class NodeInlineLoadControl;

class ProtoInline : public WonderlandExportProto {
public:
                    ProtoInline(Scene *scene);
                    ProtoInline(Scene *scene, const char *name);
    virtual Node   *create(Scene *scene);

    virtual int     getType() const { return VRML_INLINE; }
    virtual int     getNodeClass() const 
                       { return CHILD_NODE | URL_NODE | INLINE_NODE; }

    virtual bool    hasX3domOnclick(void) { return true; }

    void            addElements(void);

    FieldIndex load;
    FieldIndex url;
    FieldIndex bboxCenter;
    FieldIndex bboxSize;
    FieldIndex children;
    FieldIndex mapDEFToID;
    FieldIndex nameSpaceName;
    FieldIndex render;
};

class NodeInline : public Node {
public:
                    NodeInline(Scene *scene, Proto *proto);
                    NodeInline(NodeInlineLoadControl *inlineLoadControl);

    virtual int     getProfile(void) const;
    virtual int     getComponentLevel(void) const;
    virtual const char *getComponentName(void) const;
    virtual int     getX3dVersion(void) const { return 0; }
    virtual Node   *copy() const { return new NodeInline(*this); }
    virtual void    addFieldNodeList(int index, NodeList *childList, 
                                     int containerField = -1);

    void            setUnitLength(double f) { m_unitLength = f; }

    virtual int     countPolygons(void);
    virtual int     countPrimitives(void);

    virtual int     countPolygons1Sided(void);
    virtual int     countPolygons2Sided(void);

    virtual void    preDraw(void);
    virtual void    draw(int pass);
    virtual void    setField(int index, FieldValue *value, int cf = -1);

    virtual int     writeXml(int filedes, int indent, int containerField = -1, 
                             bool avoidUse = false);

    virtual bool    canWriteAc3d();
    virtual int     writeAc3d(int filedes, int indent);
    virtual void    handleAc3dMaterial(ac3dMaterialCallback callback, 
                                       Scene* scene);
    virtual bool    canWriteCattGeo();
    virtual int     writeCattGeo(int filedes, int indent);

    bool            alreadyLoaded(void) { return m_loadedNodes != NULL; }
    virtual NodeList *getLoadedNodes(void) { return m_loadedNodes; }

    virtual void    addToConvertedNodes(long writeFlags);

    fieldMacros(SFBool,   load,          ProtoInline)
    fieldMacros(MFString, url,           ProtoInline)
    fieldMacros(SFVec3f,  bboxCenter,    ProtoInline)
    fieldMacros(SFVec3f,  bboxSize,      ProtoInline)
    fieldMacros(MFNode,   children,      ProtoInline)
    fieldMacros(SFBool,   mapDEFToID,    ProtoInline)
    fieldMacros(MFString, nameSpaceName, ProtoInline)
    fieldMacros(SFBool,   render,        ProtoInline)

protected:
    NodeList       *m_loadedNodes;
    double          m_unitLength;
};
