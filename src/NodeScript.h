/*
 * NodeScript.h
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

#ifndef _NODE_SCRIPT_H
#define _NODE_SCRIPT_H

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
#include "CommandList.h"
#include "DynamicFieldsNode.h"

class ProtoScript : public WonderlandExportProto {
public:
                    ProtoScript(Scene *scene);
    virtual Node   *create(Scene *scene);

    virtual int     getType() const { return VRML_SCRIPT; }

    FieldIndex url;
    FieldIndex directOutput;
    FieldIndex mustEvaluate;
};

class MyString;

class NodeScript : public DynamicFieldsNode {
public:
                    NodeScript(Scene *scene);
                    NodeScript(Node *node, Proto* proto) : 
                               DynamicFieldsNode(node, proto) {}

    virtual bool    isNodeWithAdditionalEvents(void) { return true; }


    fieldMacros(MFString, url,          ProtoScript)
    fieldMacros(SFBool,   directOutput, ProtoScript)
    fieldMacros(SFBool,   mustEvaluate, ProtoScript)

    virtual int     getProfile(void) const { return PROFILE_IMMERSIVE; }
    virtual int     getX3dVersion(void) const { return 0; }
    virtual Node   *copy() const { return new NodeScript(*this); }

    virtual bool    showFields() { return true; }
    virtual void    update();

    virtual int     getMultilineMFStringField(void) 
                       { return url_Field(); }

    const char     *getEcmaScriptUrl(void);
    bool            getWantInitialize(void) 
                       { return m_wantInitialize; }
    bool            getWantEventsProcessed(void)
                       { return m_wantEventsProcessed; }
    bool            getWantShutdown(void)
                       { return m_wantShutdown; }
    void            wantInitialize(bool flag = true) 
                       { m_wantInitialize = flag; }
    void            wantEventsProcessed(bool flag = true)
                       { m_wantEventsProcessed = flag; }
    void            wantShutdown(bool flag = true)
                       { m_wantShutdown = flag; }
    bool            hasInitialize(char *string = NULL);
    bool            hasShutdown(char *string = NULL);
    bool            hasEventsProcessed(char* string = NULL);
    bool            strwhitespacestr(const char* string, const char* what);

    const char     *getUrlWithoutHeader(const char *header);
    virtual int     writeCDynamicNodeCallback(int f, int languageFlag);

    virtual int      writeXml(int filedes, int indent, 
                              int containerField = -1, bool avoidUse = false);
                                                

protected:
    virtual        ~NodeScript();
    virtual void    buildInterfaceData(void); 

private:
    bool            m_wantInitialize;
    bool            m_wantShutdown;
    bool            m_wantEventsProcessed;

};

#endif // _NODE_SCRIPT_H
