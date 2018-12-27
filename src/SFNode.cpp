/*
 * SFNode.cpp
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

#include <stdio.h>
#include "stdafx.h"
#include "Proto.h"
#include "DuneApp.h"

#include "SFNode.h"
#include "Node.h"

SFNode::SFNode(Node *value) : FieldValue()
{
    m_value = value;
    if (m_value) 
        m_value->ref();
}

SFNode::SFNode(Node *value, int containerField) : FieldValue()
{
    m_value = value;
    if (m_value) 
        m_value->ref();
    m_containerField = containerField;
}

SFNode::SFNode(const SFNode &other) : FieldValue()
{
    if (other.m_value) {
        m_value = other.m_value->copy();
        m_value->ref();
        if (!m_value->isPROTO())
            m_value->reInit();
    } else {
        m_value = NULL;
    }
}

SFNode::~SFNode()
{
    if (m_value) m_value->unref();
}

FieldValue *
SFNode::copy() 
{ 
    SFNode *ret = new SFNode(*this);
    ret->ref();
    ref();
    return ret; 
}
int SFNode::writeData(int f, int i) const
{
    // hidden by SFNode::write and SFNode::writeXml
    assert(false);
    return 0;
}

int
SFNode::write(int filedes, int indent) const
{ 
    if (m_value)
        RET_ONERROR( m_value->write(filedes, indent) )  
    else {
        RET_ONERROR( mywritestr(filedes, "NULL\n") )
        TheApp->incSelectionLinenumber();
    }
    return 0;
}

int
SFNode::writeXml(int filedes, int indent) const
{ 
    if (m_value)
        return m_value->writeXml(filedes, indent + TheApp->GetIndent(), 
                                 m_containerField);
    else       
        RET_ONERROR( mywritestr(filedes, "value='NULL'") )
    return 0;
}

const char *
SFNode::getTypeC(int languageFlag) const 
{ 
    if (languageFlag & JAVA_SOURCE)
        return TheApp->getCNodeName();
    return TheApp->getCNodeNamePtr(); 
}

int
SFNode::writeC(int filedes, const char* variableName, int languageFlag) const
{
    RET_ONERROR( mywritestr(filedes, variableName) )
    RET_ONERROR( mywritestr(filedes, " = ") )
    bool writeNull = m_value == NULL;
    if (!writeNull) {
        if (m_value->getProto()->isMismatchingProto())
            writeNull = true;
        if (m_value->getType() == VRML_COMMENT)
            writeNull = true;
    }
    if (writeNull) {
        if (languageFlag & JAVA_SOURCE)
            RET_ONERROR( mywritestr(filedes, "null") )
        else
            RET_ONERROR( mywritestr(filedes, "NULL") )
    } else {
        if (languageFlag & (C_SOURCE | CC_SOURCE))
            RET_ONERROR( mywritestr(filedes, "&") )
        RET_ONERROR( m_value->writeCVariable(filedes, languageFlag) )
    }
    RET_ONERROR( mywritestr(filedes, ";\n") )
    return 0;
}

int
SFNode::writeCDeclaration(int filedes, int languageFlag) const
{
    if (m_value)
        RET_ONERROR( m_value->getProto()->writeCDeclaration(filedes,
                                                           languageFlag) )
    return(0);
}

bool        
SFNode::readLine(int index, char *line)
{
    assert(0);
    return false;
}

bool
SFNode::equals(const FieldValue *value) const
{
    return value->getType() == SFNODE && ((SFNode *) value)->getValue() == m_value;
}

FieldValue *
SFNode::addNode(Node *node, int index) const
{
    assert(m_value == NULL);
    return new SFNode(node);
}

FieldValue *
SFNode::removeNode(Node *node) const
{
    if (m_value != node)
        return NULL;
    return new SFNode(NULL);
}

MyString
SFNode::getEcmaScriptComment(MyString name, int flags) const
{
    const char *indent = ((FieldValue *)this)->getEcmaScriptIndent(flags);
    MyString ret;
    ret = "";
    if (TheApp->GetEcmaScriptAddAllowedValues()) {
        ret += indent;
        ret += "// allowed values:\n";

        ret += indent;
        ret += "   // a VRML node object\n";
    }
    if (TheApp->GetEcmaScriptAddAllowedComponents()) {
        ret += indent;
        ret += "// allowed components:\n";

        ret += indent;
        ret += "   // depends from type of node: ";
        ret += name;
        ret += ".eventin_of_node or ";
        ret += name;
        ret += ".eventout_of_node\n";
    }
    if (TheApp->GetEcmaScriptAddAvailableFunctions()) {
        ret += indent;
        ret += "// available functions:\n";
        if (flags != EL_EVENT_IN) {
            ret += indent;
            ret += "   // ";
            ret += name;
            ret += " = new SFNode(string_vrmlstring);\n";
        }
        if (flags != EL_EVENT_OUT) {
            ret += indent;
            ret += "   // string_str = ";
            ret += name;
            ret += ".toString();\n";
        }
    }
    if (TheApp->GetEcmaScriptAddBrowserObject()) {
        if (flags != EL_EVENT_OUT) {
            ret += indent;
            ret += "// related Browser Object functions:\n";

            ret += indent;
            ret += "   // Browser.addRoute(";
            ret += name;
            ret += ", string_fromEventOut, sfnode_toNode, string_toEventIn);\n";

            ret += indent;
            ret += "   // Browser.deleteRoute(";
            ret += name;
            ret += ", string_fromEventOut, sfnode_toNode, string_toEventIn);\n";
        }
    }
    if (TheApp->GetEcmaScriptAddExampleUsage()) {
        ret += indent;
        ret += "// example usage:\n";
        if (flags != EL_EVENT_IN) {
             ret += indent;
             ret += "   // ";
             ret += name;
             ret += "= new SFNode('Shape {geometry Text {string \\\"A\\\"}}');\n";
             ret += indent;
             ret += "   // ";
             ret += name;
             ret += ".geometry.string = 'hello';\n";
        } 
        if (flags != EL_EVENT_OUT) {
             ret += indent;
             ret += "   // string_str =";
             ret += name;
             ret += ".toString();\n";
        }
    }
    return ret;
}

#include "Scene.h"

Node *
SFNode::convert2X3d(void)
{
    Node *node = getValue();
    if (node == NULL)
        return NULL;
    Node *newNode = node->convert2X3d();
    if (newNode != NULL) {
        m_value = newNode->copy();
        m_value->ref();
        if (!m_value->isPROTO())
            m_value->reInit();
    }
    return NULL;
}

Node *
SFNode::convert2Vrml(void)
{
    Node *node = getValue();
    if (node == NULL)
        return NULL;
    Node *newNode = node->convert2Vrml();
    if (newNode != NULL) {
        m_value = newNode->copy();
        m_value->ref();
        if (!m_value->isPROTO())
            m_value->reInit();
    }
    return NULL;
}

bool
SFNode::isNullNode(void) const
{
    return m_value == NULL;
}

bool
SFNode::isUseNode(void) const
{
    if (m_value != NULL)
        return m_value->getFlag(NODE_FLAG_DEFED);
    return false;
}

const char *
SFNode::getDefName(void) const
{
    if (m_value != NULL)
        return m_value->getName();
    return "";
}

int SFNode::writeAc3d(int f, int indent) const
{
    if (m_value)
        return m_value->writeAc3d(f, indent);
    return 0;
}

int SFNode::writeRib(int f, int indent) const
{
    if (m_value)
        return m_value->writeRib(f, indent);
    return 0;
}

void
SFNode::handleAc3dMaterial(ac3dMaterialCallback callback, Scene* scene)
{
    Node *node = getValue();
    if (node != NULL)
        node->handleAc3dMaterial(callback, scene);        
}

int SFNode::writeCattGeo(int f, int indent) const
{
    if (m_value)
        return m_value->writeCattGeo(f, indent);
    return 0;
}

int SFNode::writeLdrawDat(int f, int indent) const
{
    if (m_value)
        return m_value->writeLdrawDat(f, indent);
    return 0;
}

#include "Scene.h"

FieldValue *
SFNode::getRandom(Scene *scene, int nodeType)
{
    int type = nodeType;
    if (nodeType == -1)
        type = (int)(RAND() * LAST_NODE);
    else if (type > ANY_NODE)
        for (int i = 0; i < LAST_NODE; i++) {
            for (int j = LAST_NODE - i; j >= 0; j--) {
               type = i + (int)(RAND() * j);
               if (matchNodeClass(type, nodeType))
                   break;
            }
            if (matchNodeClass(type, nodeType))
               break;
        }
    return new SFNode(scene->createNode(type));
}


