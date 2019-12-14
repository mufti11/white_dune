/*
 * NodeLdrawDatExport.h
 *
 * Copyright (C) 2008 J. "MUFTI" Scheurich
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

#ifndef _NODE_LDRAW_DAT_EXPORT_H
#define _NODE_LDRAW_DAT_EXPORT_H

#ifndef _NODE_H
#include "ExternTheApp.h"
#include "Node.h"
#endif
#ifndef _PROTO_MACROS_H
#include "ProtoMacros.h"
#endif
#ifndef _PROTO_H
#include "Proto.h"
#endif

#include "SFMFTypes.h"



class ProtoLdrawDatExport : public Proto {
public:
                    ProtoLdrawDatExport(Scene *scene);
    virtual Node   *create(Scene *scene);

    virtual int     getType() const { return DUNE_LDRAW_DAT_EXPORT; }
    virtual int     getNodeClass() const { return CHILD_NODE; }

    virtual bool    isScriptedExternProto(void) { return true; }

    FieldIndex partDescription;
    FieldIndex author;
    FieldIndex fileType;
    FieldIndex license;
    FieldIndex help;
    FieldIndex bfc;
    FieldIndex category;
    FieldIndex keywords;
    FieldIndex cmdline;
    FieldIndex history;
    FieldIndex commands;
};

class NodeLdrawDatExport : public Node {
public:
                    NodeLdrawDatExport(Scene *scene, Proto *proto);

    virtual int     getProfile(void) const { return PROFILE_INTERCHANGE; }
    virtual int     getX3dVersion(void) const { return -1; }
    virtual Node *  copy() { return new NodeLdrawDatExport(m_scene, m_proto); }
    
    virtual int     writeProto(int filedes);

    virtual bool    validate(void);

    virtual bool    canWriteLdrawDat() { return true; }
    virtual int     writeLdrawDatHeader(int filedes, const char *filename);

    bool            checkMissingData(int field, unsigned int limit = 0);

    fieldMacros(SFString, partDescription, ProtoLdrawDatExport)
    fieldMacros(SFString, author,          ProtoLdrawDatExport)
    fieldMacros(SFString, fileType,        ProtoLdrawDatExport)
    fieldMacros(SFString, license,         ProtoLdrawDatExport)
    fieldMacros(SFString, bfc,             ProtoLdrawDatExport)
    fieldMacros(MFString, help,            ProtoLdrawDatExport)
    fieldMacros(SFString, category,        ProtoLdrawDatExport)
    fieldMacros(MFString, keywords,        ProtoLdrawDatExport)
    fieldMacros(SFString, cmdline,         ProtoLdrawDatExport)
    fieldMacros(MFString, history,         ProtoLdrawDatExport)
    fieldMacros(MFString, commands,        ProtoLdrawDatExport)
};

#endif
