/*
 * NodeNavigationInfo.h
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

#ifndef _NODE_NAVIGATIONINFO_H
#define _NODE_NAVIGATIONINFO_H

#ifndef _NODE_H
#include "Node.h"
#endif
#ifndef _PROTO_MACROS_H
#include "ProtoMacros.h"
#endif
#ifndef _PROTO_H
#include "Proto.h"
#endif

#include "SFMFTypes.h"

class ProtoNavigationInfo : public Proto {
public:
                    ProtoNavigationInfo(Scene *scene);
                    ProtoNavigationInfo(Scene *scene, const char *name);
    void            addElements(void);

    virtual Node   *create(Scene *scene);

    virtual int     getType() const { return VRML_NAVIGATION_INFO; }

    FieldIndex avatarSize;
    FieldIndex headlight;
    FieldIndex speed;
    FieldIndex transitionTime;
    FieldIndex transitionType; 
    FieldIndex type;
    FieldIndex typeX3D;
    FieldIndex visibilityLimit;
    FieldIndex scale;
    FieldIndex blendingSort;
    FieldIndex bind;
    FieldIndex explorationMode;
    FieldIndex isActive;
    FieldIndex typeParams;
};

class NodeNavigationInfo : public Node {
public:
                    NodeNavigationInfo(Scene *scene, Proto *proto);

    virtual int     getProfile(void) const;
    virtual int     getX3dVersion(void) const { return 0; }
    virtual Node   *copy() const { return new NodeNavigationInfo(*this); }

    virtual bool    hasNumbers4kids(void) { return true; } 

    virtual void    preDraw();
    void            apply() {}

    virtual bool    hasCoverFields(void) { return true; }   

    fieldMacros(MFFloat,  avatarSize,      ProtoNavigationInfo) 
    fieldMacros(SFBool,   headlight,       ProtoNavigationInfo)
    fieldMacros(SFFloat,  speed,           ProtoNavigationInfo) 
    fieldMacros(SFTime,   transitionTime,  ProtoNavigationInfo);
    fieldMacros(MFString, transitionType,  ProtoNavigationInfo) 
    fieldMacros(MFString, type,            ProtoNavigationInfo) 
    fieldMacros(MFString, typeX3D,         ProtoNavigationInfo) 
    fieldMacros(SFFloat,  visibilityLimit, ProtoNavigationInfo)
    fieldMacros(SFFloat,  scale,           ProtoNavigationInfo)
    fieldMacros(SFString, blendingSort,    ProtoNavigationInfo)
    fieldMacros(SFBool,   bind,            ProtoNavigationInfo);
    fieldMacros(SFString, explorationMode, ProtoNavigationInfo);
    fieldMacros(SFBool,   isActive,        ProtoNavigationInfo);
    fieldMacros(MFFloat,  typeParams,      ProtoNavigationInfo);
};

#endif // _NODE_NAVIGATIONINFO_H
