/*
 * DynamicFieldsNode.h
 *
 * Copyright (C) 2006 J. "MUFTI" Scheurich
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

#ifndef _DYNAMIC_FIELDS_NODE_H
#define _DYNAMIC_FIELDS_NODE_H

#ifndef _NODE_H
#include "Node.h"
#endif

#ifndef _PROTO_MACROS_H
#include "ProtoMacros.h"
#endif
#include "SFMFTypes.h"
#include "CommandList.h"
#include "InterfaceArray.h"

class DynamicFieldsNode : public Node {
public:
                    DynamicFieldsNode(Scene *scene, Proto *proto) :
                               Node(scene, proto) 
                       { initialise(); }
                    DynamicFieldsNode(Node *node, Proto* proto) : 
                               Node(*node, proto)
                       { initialise(); }
    virtual bool    isDynamicFieldsNode(void) { return true; }


    void            initialise(void);
    virtual int     write(int filedes, int indent);
    virtual int     writeXml(int filedes, int indent, int containerField = -1);

    int             writeField(int f, int indent, int fieldIndex);
    virtual int     writeFields(int filedes, int indent);
    int             writeEvents(int filedes, int indent);

    virtual int     writeProto(int f)  { return Node::writeProto(f); }

    virtual int     writeCDynamicNodeCallback(int f, int languageFlag) 
                       { return 0; }


    void            addField(int fieldType, const MyString &name);
    void            addExposedField(int fieldType, const MyString &name);
    void            addEventIn(int fieldType, const MyString &name);
    void            addEventOut(int fieldType, const MyString &name);
    void            deleteEventOut(int index);

    // for multiline string values the number for lines must be counted
    // usage for the "url" field of NodeScript
    virtual int     getMultilineMFStringField(void) { return -1; }

    void            updateDynamicFields();

    InterfaceArray *getInterfaceData(void);

    void            updateEventIn(int newIndex, int oldIndex);
    void            updateEventOut(int newIndex, int oldIndex);

protected:

private:
    CommandList    *m_routeList;
};

#endif // _DYNAMIC_FIELDS_NODE_H
