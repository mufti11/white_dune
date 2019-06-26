/*
 * Node.cpp
 *
 * Copyright (C) 1999 Stephen F. White, 2005 J. "MUFTI" Scheurich
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
#include <string.h>
#include <ctype.h>
#include "stdafx.h"

#include "Node.h"
#include "Scene.h"
#include "Field.h"
#include "FieldValue.h"
#include "FieldCommand.h"
#include "Proto.h"
#include "Path.h"
#include "SFNode.h"
#include "MFNode.h"
#include "SFString.h"
#include "MFString.h"
#include "EventIn.h"
#include "EventOut.h"
#include "ExposedField.h"
#include "URL.h"
#include "DuneApp.h"
#include "NodeScript.h"
#include "swDebugf.h"
#include "ScriptEventDialog.h"
#include "NodeCurveAnimation.h"
#include "NodePositionInterpolator.h"
#include "NodeOrientationInterpolator.h"
#include "NodeTimeSensor.h"
#include "NodeInline.h"
#include "NodeComment.h"

NodeData::NodeData(Scene *scene, Proto *proto)
{
    static int identifier_source = 0;
    identifier_source++;
    m_identifier = identifier_source;
    m_identifierCopy = -1;
    m_scene = scene;

    m_graphX = m_graphY = 0;
    m_graphWidth = m_graphHeight = 0;

    m_refs = 0;
    m_flags = 1<<NODE_FLAG_COLLAPSED;
    m_proto = proto;

    bool x3d = scene->isX3d();

    m_numFields = m_proto->getNumFields();
    m_fields = new FieldValue *[m_numFields];
    for (int i = 0; i < m_numFields; i++) {
        m_fields[i] = m_proto->getField(i)->getDefault(x3d);
        if (m_fields[i]) {
            m_fields[i] = m_fields[i]->copy();
            m_fields[i]->ref();
        }
    }
    m_numEventIns = m_proto->getNumEventIns();
    m_inputs = new SocketList[m_numEventIns];
    m_numEventOuts = m_proto->getNumEventOuts();
    m_outputs = new SocketList[m_numEventOuts];
    m_insideProto = NULL;
    m_nodePROTO = NULL;
    m_canDraw = true;
    handleIs();
    generateTreeLabel();
    m_name = "";
    m_variableName = "";
    m_isConvertedInCurveAnimaton = false;
    setCounter4SceneTreeViewToZero();
    m_written = false;
    m_scene->addNode((Node*)this);
}

NodeData::NodeData(const Node &node) {
    copyData(node);
    m_identifierCopy = node.m_identifier;
}    
    
void
NodeData::copyData(const NodeData &node)
{
    m_identifier = node.m_identifier;// all IDs are unique, except IDs of copies

    m_scene = node.m_scene;
    m_refs = 0;
    m_graphX = m_graphY = 0;
    m_graphWidth = m_graphHeight = 0;
    m_flags = node.m_flags;
    m_proto = node.m_proto;
    m_numFields = m_proto->getNumFields();
    m_fields = new FieldValue *[m_numFields];
    if (node.m_name.length() != 0) {
        if (!m_scene->hasAlreadyName(node.m_name)) 
            m_scene->def(node.m_name, (Node *)this);
        else
            m_scene->makeSimilarName((Node *)this, node.m_name);
    }
    for (int i = 0; i < m_numFields; i++) {
        m_fields[i] = node.m_fields[i]->copy();
        m_fields[i]->ref();
    }
    m_numEventIns = m_proto->getNumEventIns();
    m_inputs = new SocketList[m_numEventIns];
    m_numEventOuts = m_proto->getNumEventOuts();
    m_outputs = new SocketList[m_numEventOuts];
    m_insideProto = node.m_insideProto;
    m_nodePROTO = node.m_nodePROTO;
    m_canDraw = node.m_canDraw;
    handleIs();
    generateTreeLabel();        
    m_isConvertedInCurveAnimaton = node.m_isConvertedInCurveAnimaton;
    m_variableName = node.m_variableName;
    for (int i = 0; i < node.m_convertedNodes.size(); i++)
        m_convertedNodes[i] = node.m_convertedNodes[i];
    for (int i = 0; i < node.m_isEventIns.size(); i++)
        m_isEventIns[i] = node.m_isEventIns[i];
    for (int i = 0; i < node.m_isEventOuts.size(); i++)
        m_isEventOuts[i] = node.m_isEventOuts[i];
    for (int i = 0; i < node.m_isFields.size(); i++)
        m_isFields[i] = node.m_isFields[i];
    for (int i = 0; i < node.m_isExposedFields.size(); i++)
        m_isExposedFields[i] = node.m_isExposedFields[i];
    m_scene->addNode((Node*)this);
 }

NodeData::~NodeData()
{
    unref();
}

void 
NodeData::delete_this(void)
{
    for (int i = 0; i < m_numFields; i++) {
        m_fields[i]->unref();
    }
    m_numFields = 0;
    delete [] m_fields;
    delete [] m_inputs;
    delete [] m_outputs;
    m_scene->undef(m_name);
    m_scene->removeNode((Node *)this);
}

int                 
NodeData::translateField(int field) const
{ 
    return m_proto->translateField(field); 
}

int                 
NodeData::repairField(int field) const
{ 
    return translateField(field); 
}

bool      
NodeData::isMesh(void) 
{ 
    return m_proto->isMesh();
}


Node::Node(Scene *scene, Proto *proto) : NodeData(scene,proto) 
{
    m_commentsList = new NodeList;
    m_geometricParentIndex = -1;     
    m_numberCDataFunctions = 0;
    m_containerField = -1;
    m_protoParent = NULL;
    ref();
}

Node::Node(const Node &node) : NodeData(node)
{
    m_geometricParentIndex = node.getGeometricParentIndex();
    m_commentsList = new NodeList;
    ref();
    m_numberCDataFunctions = 0;
}

Node::Node(const Node &node, Proto *proto) : NodeData(node)
{ 
    m_geometricParentIndex = node.getGeometricParentIndex();
    m_proto = proto;
    for (int i = 0; i < proto->getNumEventOuts(); i++)
        m_outputs[i] = node.m_outputs[i];
    m_commentsList = new NodeList;
    ref();
    m_numberCDataFunctions = 0;
    m_containerField = -1;
}

Node::~Node()
{
    delete m_commentsList;
}

bool
NodeData::matchNodeClass(int childType) const
{ 
    return m_proto->matchNodeClass(childType); 
}

void
NodeData::copyParentList(const Node &node)
{ 
    for (int i = 0; i < node.m_parents.size(); i++)
        m_parents.append(node.m_parents[i]);
}

int         
NodeData::getType() const 
{ 
    if (m_proto && isPROTO() && m_scene)
        return m_scene->getProtoType(m_proto);
    if (m_proto)
        return m_proto->getType(); 
    return -1; 
}

int
NodeData::getNodeClass() const
{ 
    if (m_proto) 
        return m_proto->getNodeClass(); 
    return CHILD_NODE; 
}

int                
NodeData::getMaskedNodeClass(void)
{ 
    if (m_proto)
        return ::getMaskedNodeClass(getNodeClass()); 
    return 0;
}

bool
NodeData::hasName(void)
{
    if (m_name.length() == 0)
        if (needsDEF())
            m_scene->generateUniqueNodeName((Node *)this);       
    if (m_name.length() == 0) 
        return false;
    else
        return true;
}

const MyString& 
NodeData::getName(void)
{
    if (needsDEF())
       if (m_name.length() == 0) 
          m_scene->generateUniqueNodeName((Node *)this);       
    return m_name; 
}

const MyString& 
NodeData::getNameOrNewName(void)
{
    if (m_name.length() == 0) 
        m_scene->generateUniqueNodeName((Node *)this);       
    return m_name; 
}

void
NodeData::setName(const char *name)
{
    m_scene->undef(m_name);
    m_name = name; 
    generateTreeLabel();
}

bool
NodeData::needsDEF() const
{
    if (m_name.length() != 0) return true;

    if (((Node *)this)->getNumParents() > 1) return true;

    for (int i = 0; i < m_numEventIns; i++) {
        if (m_inputs[i].size() > 0) {
            return true;
        }
    }

    for (int j = 0; j < m_numEventOuts; j++) {
        if (m_outputs[j].size() > 0) {
            return true;
        }
    }

    return false;
}

// C/C++/java keywords not possible as variable names
static const char *keyWords[] = {
    "abstract",
    "asm",
    "auto",
    "bool",
    "boolean",
    "break",
    "byte",
    "case",
    "catch",
    "char",
    "class",
    "const",
    "continue",
    "default",
    "delete",
    "do",
    "double",
    "else",
    "enum",
    "extends",
    "extern",
    "final",
    "finally",
    "float",
    "for",
    "friend",
    "goto",
    "if",
    "implements",
    "import",
    "inline",
    "instanceof",
    "int",
    "interface",
    "long",
    "native",
    "new",
    "operator",
    "package",
    "private",
    "protected",
    "public",
    "register",
    "return",
    "short",
    "signed",
    "sizeof",
    "static",
    "strictfp",
    "String",
    "struct",
    "super",
    "switch",
    "synchronized",
    "template",
    "this",
    "throw",
    "throws",
    "transient",
    "try",
    "typedef",
    "union",
    "unsigned",
    "virtual",
    "void",
    "volatile",
    "while"
};

const char *
NodeData::getVariableName(void)
{ 
    bool isRoot = false;
    if (m_variableName.length() == 0) {
        MyString cName = "";
        if ((m_name != NULL) && (m_name.length() > 0)) {
            if (isalpha(m_name[0]))
                cName += m_name[0];
            else
                cName += "A";
            for (int i = 1; i < m_name.length(); i++)
                if (isalnum(m_name[i]))
                    cName += m_name[i];
                else
                    cName += "m_";
        }

        if (m_scene != NULL) {
            if (m_scene->getRoot() == this) {
                isRoot = true;
                m_variableName = "";
                m_variableName += "root";
            }
        }
        if ((!isRoot) && hasName()) {
            m_variableName = "";
            bool isKeyWord = false;
            for (unsigned int i = 0; 
                 i < sizeof(keyWords)/sizeof(const char *); i++)
                if (strcmp(m_name.getData(), keyWords[i]) == 0) {
                    isKeyWord = true;
                    m_variableName += m_scene->getUniqueNodeName(cName);
                    swDebugf("Warning: Variablename ");
                    swDebugf("\"%s\"", m_name.getData());
                    swDebugf(" is a C++/java keyword, ");
                    swDebugf("\"%s\"", m_variableName.getData());
                    swDebugf(" used instead\n");
                }
            if (!isKeyWord)
                m_variableName += cName;           
        }
        if (m_variableName.length() == 0)
            if (m_scene != NULL) {
                m_variableName = "";
                m_variableName += m_scene->generateVariableName((Node *)this);
            }
    }

    return m_variableName;
}    

void                
NodeData::setVariableName(const char *name)
{
    m_variableName = "";
    m_variableName += name;
}

const char *
NodeData::getClassName(void) 
{
    if (m_proto)
        return m_proto->getClassName();
    return NULL;
}


FieldValue *
NodeData::getField(int index) const
{
#ifdef DEBUG
    assert(index >= 0 && index < m_numFields);
#else
    if ((index < 0) && (index >= m_numFields)) {
        printf("Internal error in NodeData::getField\n");
        return NULL;
    }
#endif
    if (m_scene != NULL)
        if (((m_scene->getWriteFlags() & (CONVERT2VRML | CONVERT2X3D)) != 0) ||
             m_scene->isX3d())
            return m_fields[translateField(index)];
    return m_fields[index];
}

void
NodeData::setField(int fieldIndex, FieldValue *value, int containerField)
{
    int index = fieldIndex;
    if (m_scene != NULL)
        if (((m_scene->getWriteFlags() & (CONVERT2VRML | CONVERT2X3D)) == 0) ||
             m_scene->isX3d())
         index = translateField(index);
    assert(index >= 0 && index < m_numFields);
    Field *field = m_proto->getField(index);
    if ((field != NULL) && (value->getRefs() != 0))
        assert(field->getType() == value->getType());

    // if field is an SFNode or MFNode type, remove old values from 
    // children's parent list

    bool isNode = false;
    if (m_fields[index]->getType() == SFNODE) {
        isNode = true;
        Node *child = ((SFNode *) m_fields[index])->getValue();
        if (child) child->removeParent();
    } else if (m_fields[index]->getType() == MFNODE) {
        isNode = true;
        NodeList *childList = ((MFNode *) m_fields[index])->getValues();
        if (childList) {
            for (int i = 0; i < childList->size(); i++) {
                Node *child = childList->get(i);
                if (child) child->removeParent();
            }
        }
    }

    if (value->getRefs() > -1)
        value->clamp(field->getMin(), field->getMax());

    if (m_fields[index] != NULL)
        m_fields[index]->unref();
    m_fields[index] = value;
    m_fields[index]->ref();
    if (isNode) {
        if (containerField == -1)
            m_fields[index]->setContainerField(index);
        else
            m_fields[index]->setContainerField(containerField);
    }

    if (value->getType() == SFNODE) {
        Node *child = ((SFNode *) value)->getValue();
        if (child) child->addParent((Node*)this, index);
    } else if (value->getType() == MFNODE) {
        NodeList *childList = ((MFNode *) value)->getValues();
        if (childList) {
            for (int i = 0; i < childList->size(); i++) {
                Node *child = childList->get(i);
                if (child) child->addParent((Node*)this, index);
            }
        }
    }
    // handle IS
    if (field && (field->getFlags() & FF_IS))
        for (int i = 0; i < field->getNumIs(); i++) {
            Node *isNode = getIsNode(field->getIsNodeIndex(i));
            if (isNode)
                isNode->setField(field->getIsField(i), value);
        }
    ExposedField *expField = field->getExposedField();
    if (expField && (expField->getFlags() & FF_IS))
        for (int i = 0; i < expField->getNumIs(); i++) {
            Node *isNode = getIsNode(expField->getIsNodeIndex(i));
            if (isNode)
                isNode->setField(expField->getIsField(i), value);
        }
    if (m_scene)
        m_scene->setNotSaved();
}

FieldValue *
NodeData::getUntranslatedField(int index) const
{
#ifdef DEBUG
    assert(index >= 0 && index < m_numFields);
#else
    if ((index < 0) && (index >= m_numFields)) {
        printf("Internal error in NodeData::getField\n");
        return NULL;
    }
#endif
    return m_fields[index];
}

void
Node::addFieldNodeList(int index, NodeList *childList, int containerField)
{
    assert(index >= 0 && index < m_numFields);
    if (((MFNode*)m_fields[index])->getSize() == 0)
        setField(index, new MFNode(childList), containerField);
    else {
        for (int i = 0; i < childList->size(); i++) {
            Node *child = childList->get(i);
            FieldValue* newField= ((MFNode*)m_fields[index])->addNode(child);
            ((MFNode*)m_fields[index])->setContainerField(containerField);
            newField->clamp(m_proto->getField(index)->getMin(), 
                            m_proto->getField(index)->getMax());

            m_fields[index]->unref();
            m_fields[index] = newField;
            m_fields[index]->ref();
            if (child) {
                child->addParent(this, index);
            }
        }
    }
    ((MFNode*)m_fields[index])->getValues();
}

int 
Node::write(int f, int indent)
{
    bool x3d = m_scene->isX3dv();
    if (m_proto) {
        TheApp->checkSelectionLinenumberCounting(m_scene, (Node*) this);
        if (getFlag(NODE_FLAG_DEFED) && (m_name.length() > 0)) {
            RET_ONERROR( mywritestr(f, "USE ") )
            RET_ONERROR( mywritestr(f, (const char *) m_name) )
            RET_ONERROR( mywritestr(f, "\n") )
            TheApp->incSelectionLinenumber();
        } else {
            if (((Node *)this)->needsDEF()) {
                if (!m_name[0]) m_scene->generateUniqueNodeName((Node*)this);
                RET_ONERROR( mywritestr(f, "DEF ") )
                RET_ONERROR( mywritestr(f, (const char *) m_name) )
                RET_ONERROR( mywritestr(f, " ") )
            }
            setFlag(NODE_FLAG_DEFED);
            bool writeProtoName = true;
            if (m_scene->isPureVRML97())
                if ((TheApp->getCoverMode() && (!hasDefault(FF_COVER_ONLY))) ||
                    (TheApp->getKambiMode() && (!hasDefault(FF_KAMBI_ONLY)))) {
                    Proto *extension = m_scene->getExtensionProto(m_proto);
                    if (extension != NULL) {
                        writeProtoName = false;
                        RET_ONERROR( mywritestr(f, extension->getName(x3d)) )
                    }
                }
            if (writeProtoName) {
                RET_ONERROR( mywritestr(f, (const char *) m_proto->getName(x3d)))
            }

            if (!TheApp->GetkrFormating()) {
                RET_ONERROR( mywritestr(f, "\n") )
                TheApp->incSelectionLinenumber();
                RET_ONERROR( indentf(f, indent + TheApp->GetIndent()) )
            } else
                RET_ONERROR( mywritestr(f, " ") )

            RET_ONERROR( mywritestr(f, "{\n") )
            TheApp->incSelectionLinenumber();
            RET_ONERROR( writeFields(f, indent + TheApp->GetIndent()) )
            if (!TheApp->GetkrFormating())
                RET_ONERROR( indentf(f, indent + TheApp->GetIndent()) )
            else
                RET_ONERROR( indentf(f, indent) )
            RET_ONERROR( mywritestr(f, "}\n") )
            TheApp->incSelectionLinenumber();
            if (indent==0) {
                RET_ONERROR( mywritestr(f, "\n") )
                TheApp->incSelectionLinenumber();
            }
            RET_ONERROR( writeRoutes(f, indent) )
            setFlag(NODE_FLAG_TOUCHED);
        }
    }
    return(0);
}

int
Node::writeXml(int f, int indent, int containerField)
{
    bool x3d = true;
    if (getType() == VRML_COMMENT) {
        MFString *value = ((NodeComment *)this)->comment();
        RET_ONERROR( indentf(f, indent) )
        RET_ONERROR( mywritestr(f, "<!--") )
        if (value->getSize() > 1)
            RET_ONERROR( mywritestr(f, "\n") )
        TheApp->checkSelectionLinenumberCounting(m_scene, this);
        for (int i = 0; i < value->getSize(); i++) {
            if (value->getSize() > 1)
                RET_ONERROR( indentf(f, indent + TheApp->GetIndent()) )
            RET_ONERROR( mywritestr(f, (const char *) value->getValue(i)) )
            if (value->getSize() > 1)
                RET_ONERROR( mywritestr(f, "\n") )
            TheApp->incSelectionLinenumber();
        } 
        if (value->getSize() > 1)
            RET_ONERROR( indentf(f, indent) )
        RET_ONERROR( mywritestr(f, "-->\n") )
        return 0;
    }
    if (m_proto) {
        Proto *protoToWrite = NULL;
        TheApp->checkSelectionLinenumberCounting(m_scene, (Node*) this);
        RET_ONERROR( indentf(f, indent) )
        RET_ONERROR( mywritestr(f, "<") )
        if (m_scene->isPureVRML97() &&
            ((TheApp->getCoverMode() && (!hasDefault(FF_COVER_ONLY))) ||
             (TheApp->getKambiMode() && (!hasDefault(FF_KAMBI_ONLY))))) {
            protoToWrite = m_scene->getExtensionProto(m_proto);
        } else if (isPROTO() || 
                   m_proto->isScriptedProto() || 
                   m_proto->isScriptedExternProto()) {
            protoToWrite = m_proto;
        }    
        bool containerFieldWritten = false;
        if (protoToWrite == NULL) {
            RET_ONERROR( mywritestr(f, (const char *) m_proto->getName(x3d)))
            if (hasParent()) {
                if ((getParent()->showFields()) && 
                    (getParent()->getType() != VRML_SCRIPT)) {
                    Node *parent = getParent();
                    int cField = containerField;
                    if (cField == -1)
                        cField = getParentField();
                    if (cField != getParentField()) {
                        parent = searchParentField(cField);
                    }              
                    const char *name = parent->getProto()->getField(
                                           cField)->getName(x3d);
                    RET_ONERROR( mywritestr(f, " containerField='") )
                    RET_ONERROR( mywritestr(f, name) )
                    RET_ONERROR( mywritestr(f, "'") )
                    containerFieldWritten = true;
                }
            }
        } else {
            RET_ONERROR( mywritestr(f, "ProtoInstance name='") )
            RET_ONERROR( mywritestr(f, protoToWrite->getName(x3d)) )
            RET_ONERROR( mywritestr(f, "'") )
        }

        if (getFlag(NODE_FLAG_DEFED)) {
            RET_ONERROR( mywritestr(f, " USE='") )
            RET_ONERROR( mywritestr(f, (const char *) m_name) )
            RET_ONERROR( mywritestr(f, "'") )
            if (hasParent() && 
                (containerFieldWritten == false)) {
                if ((getParent()->showFields()) &&
                    (getParent()->getType() != VRML_SCRIPT)) {
                    Node *parent = getParent();
                    int cField = containerField;
                    if (cField == -1)
                        cField = getParentField();
                    if (cField != getParentField()) {
                        parent = searchParentField(cField);
                    }              
                    const char *name = parent->getProto()->getField(
                                           cField)->getName(x3d);
                    RET_ONERROR( mywritestr(f, " containerField='") )
                    RET_ONERROR( mywritestr(f, name) )
                    RET_ONERROR( mywritestr(f, "'") )
                }
            }
            RET_ONERROR( mywritestr(f, "/>\n") )
            TheApp->incSelectionLinenumber();
        } else {
            if (((Node *)this)->needsDEF()) {
                if (!m_name[0]) m_scene->generateUniqueNodeName((Node*)this);
                RET_ONERROR( mywritestr(f, " DEF='") )
                RET_ONERROR( mywritestr(f, (const char *) m_name) )
                RET_ONERROR( mywritestr(f, "' ") )
                setFlag(NODE_FLAG_DEFED);
                if (m_scene->getWriteFlags() & X3DOM) {
                    RET_ONERROR( mywritestr(f, " id='") )
                    RET_ONERROR( mywritestr(f, (const char *) m_name) )
                    RET_ONERROR( mywritestr(f, "' ") )
                }   
            }
            RET_ONERROR( mywritestr(f, " ") )
            RET_ONERROR( writeXmlFields(f, indent, XML_IN_TAG) )
            RET_ONERROR( mywritestr(f, ">\n") )
            TheApp->incSelectionLinenumber();
            RET_ONERROR( writeXmlFields(f, indent, XML_NODE) )
            RET_ONERROR( writeXmlFields(f, indent + TheApp->GetIndent(), 
                                        XML_PROTO_INSTANCE_FIELD) )
            RET_ONERROR( writeXmlFields(f, indent + TheApp->GetIndent(), 
                                        XML_IS) )
            RET_ONERROR( indentf(f, indent) )
            RET_ONERROR( mywritestr(f, "</") )
            if (protoToWrite == NULL)
                RET_ONERROR( mywritestr(f, (const char *) m_proto->getName(x3d)))
            else
                RET_ONERROR( mywritestr(f, "ProtoInstance") )
            RET_ONERROR( mywritestr(f, ">\n") )
            TheApp->incSelectionLinenumber();
            RET_ONERROR( writeRoutes(f, indent) )
            setFlag(NODE_FLAG_TOUCHED);
        }
    }
    return 0;
}

int            
NodeData::writeProto(int filedes)
{
    // used to write the EXTERNPROTO definition of extension nodes
    return 0;
}

int
NodeData::writeProto(int f, const char *urn, const char *directoryName,
                           const char *haveUrl, bool nameInUrl)
{
    if (m_scene->isX3dXml())
        return m_proto->write(f, 0, X3D_XML);
    const char* nodeName = m_proto->getName(m_scene->isX3d());
    RET_ONERROR( mywritef(f, "EXTERNPROTO %s[\n", nodeName) )    
    TheApp->incSelectionLinenumber();
    RET_ONERROR( writeProtoArguments(f) )
    RET_ONERROR( mywritestr(f, " ]\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f, "[\n") )
    TheApp->incSelectionLinenumber();
    if (strlen(urn) > 0) {
        RET_ONERROR( mywritef(f, "  \"%s:%s\"\n", urn, nodeName) )
        TheApp->incSelectionLinenumber();
    }
    RET_ONERROR( mywritef(f, "  \"%sPROTO.wrl\"\n", nodeName) )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f, "  \"") )
    char *dunedocs = getenv("DUNEDOCS");
    if (dunedocs != NULL) {
        RET_ONERROR( mywritestr(f, dunedocs) )
        RET_ONERROR( mywritestr(f, "/") )
        RET_ONERROR( mywritestr(f, directoryName) )
        RET_ONERROR( mywritestr(f, "/") )
    } else if (strlen(haveUrl) > 0) {
        RET_ONERROR( mywritestr(f, haveUrl) )
        if (nameInUrl)
            RET_ONERROR( mywritestr(f, "/") )
        else
            RET_ONERROR( mywritestr(f, "\"\n") )
    }
    if ((dunedocs != NULL) || (nameInUrl))
        RET_ONERROR( mywritef(f, "%sPROTO.wrl\"\n", nodeName) )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritef(f, 
                          "  \"%s/docs/%s/%sPROTO.wrl\"\n",
                          "http://wdune.ourproject.org/",
                          directoryName, nodeName) )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f, "]\n") )
    TheApp->incSelectionLinenumber();
    return 0;
}

int     
NodeData::writeX3dProto(int filedes) 
{
    return ((Node *)this)->writeProto(filedes, 
                                      "urn:web3d:x3d:node:",
                                      "x3d"
#ifdef HAVE_X3D_PROTO_URL
                                      , HAVE_X3D_PROTO_URL
#endif
                                      );
}

int 
NodeData::writeIs(int f, int indent, const char *name, const char* isName)
{
    RET_ONERROR( indentf(f, indent) )
    RET_ONERROR( mywritestr(f, name) )
    RET_ONERROR( mywritestr(f, " IS ") )
    RET_ONERROR( mywritestr(f, isName) )
    RET_ONERROR( mywritestr(f, "\n") )
    TheApp->incSelectionLinenumber();
    return 0;
}

int 
NodeData::writeXmlIs(int f, int indent, const char *name, const char* isName)
{
    RET_ONERROR( indentf(f, indent) )
    RET_ONERROR( mywritestr(f, "<connect nodeField='") )
    RET_ONERROR( mywritestr(f, name) )
    RET_ONERROR( mywritestr(f, "' protoField='") )
    RET_ONERROR( mywritestr(f, isName) )
    RET_ONERROR( mywritestr(f, "' />\n") )
    TheApp->incSelectionLinenumber();
    return 0;
}


int 
NodeData::writeXmlProtoInstanceField(int f, int indent, const char *name, 
                                     FieldValue* value)
{
    bool isNode = false;
    bool needValue = true;
    if (value->getType() == SFNODE) {
        if (((SFNode *)value)->getValue() == NULL)
            needValue = false;
        else
            isNode = true;
    }
    if (value->getType() == MFNODE) {
        if (((MFieldValue *)value)->getSFSize() == 0)
            needValue = false;
        else
            isNode = true; 
    }

    RET_ONERROR( indentf(f, indent) )
    RET_ONERROR( mywritestr(f, "<fieldValue name='") )
    RET_ONERROR( mywritestr(f, name) )
    RET_ONERROR( mywritestr(f, "' ") )
    if (isNode)
        RET_ONERROR( mywritestr(f, ">\n") )
    else if (needValue)
        RET_ONERROR( mywritestr(f, "value=") )
    RET_ONERROR( value->writeXml(f, indent) )        
    if (isNode) {
        RET_ONERROR( indentf(f, indent) )
        RET_ONERROR( mywritestr(f, "</fieldValue>\n") )
    } else { 
        RET_ONERROR( mywritestr(f, "/>\n") )
    }
    TheApp->incSelectionLinenumber();
    return 0;
}

const char *
NodeData::searchIsName(int i, int type)
{
    Element *element = NULL;
    switch (type) {
      case EL_FIELD:
        element = m_proto->getField(i);
        break;
      case EL_EVENT_IN:
        element = m_proto->getEventIn(i);
        break;
      case EL_EVENT_OUT:
        element = m_proto->getEventOut(i);
        break;
      default:
        return NULL;
    }
    if (m_scene->isInvalidElement(element))
        return NULL;
    if (!isInsideProto())
        return NULL;
    
    bool x3d = m_scene->isX3d();
    const char *isName = NULL;
    bool stopLoop = false;
    for (int j = 0; j < m_scene->getNumProtoNames(); j++) {
        const char *protoName = m_scene->getProtoName(j);
        Proto *proto = m_scene->getProto(protoName);
        int isElement = -1;
        ExposedField *exposedField = NULL;
        int field = i;
        int myType = type;
        switch (type) {
          case EL_EVENT_IN:
            exposedField = ((EventIn *)element)->getExposedField();
            if (exposedField) {
                field = exposedField->getFieldIndex();
                myType = EL_EXPOSED_FIELD;
            }
            isElement = proto->lookupIsEventIn((Node *)this, field, myType);
            if (isElement == -1)
                isElement = proto->lookupIsEventOut((Node *)this, field, 
                                                    EL_EVENT_IN);
            if (isElement != -1) {
                isName = proto->getEventIn(isElement)->getName(x3d);
                stopLoop = true;
            }
            break;
          case EL_EVENT_OUT:
            exposedField = ((EventOut *)element)->getExposedField();
            if (exposedField) {
                field = exposedField->getFieldIndex();
                myType = EL_EXPOSED_FIELD;
            }
            isElement = proto->lookupIsEventOut((Node *)this, field, myType);
            if (isElement == -1)
                isElement = proto->lookupIsEventOut((Node *)this, field, 
                                                    EL_EVENT_OUT);
            if (isElement != -1) {
                isName = proto->getEventOut(isElement)->getName(x3d);
                stopLoop = true;
            }
            break;
          case EL_FIELD:
            isElement = proto->lookupIsExposedField((Node *)this, i);
            if (isElement != -1) {
                isName = proto->getExposedField(isElement)->getName(x3d);
                stopLoop = true;
            } 
            if ((isName == NULL) || (isName && strlen(isName) == 0)) {
                isElement = proto->lookupIsField((Node *)this, i);
                if (isElement != -1) {
                    isName = proto->getField(isElement)->getName(x3d);
                    stopLoop = true;
                }
            }
        }
        if (stopLoop)
            break;
    }
    return isName;
}

int 
NodeData::writeFields(int f, int indent)
{
    if (!m_proto) return(0);

    // delay writing of Script.url
    int scriptUrlField = -1;
    if (getType() == VRML_SCRIPT)
        scriptUrlField = ((NodeScript *)this)->url_Field();

    for (int i = 0; i < m_numFields; i++) 
        if (i != scriptUrlField)
            RET_ONERROR( writeField(f, indent, i) )

    for (int i = 0; i < m_numEventIns; i++)
        RET_ONERROR( writeEventIn(f, indent, i) )

    for (int i = 0; i < m_numEventOuts; i++)
        RET_ONERROR( writeEventOut(f, indent, i) )

    if ((scriptUrlField != -1) && (!isPROTO()))
        RET_ONERROR( writeField(f, indent, scriptUrlField) )

    return(0);
}

int 
NodeData::writeXmlFields(int f, int indent, int when)
{
    if (!m_proto) return(0);

    if (isPROTO() || (getType() == DUNE_VRML_CUT) || 
                     (getType() == DUNE_VRML_SCENE)) {
        if (when != XML_PROTO_INSTANCE_FIELD)
            return 0;    
    } else 
        if (when == XML_PROTO_INSTANCE_FIELD)
            return 0;    

    bool writeIs = false;
    if (when == XML_IS) {
        for (int i = 0; i < m_numFields; i++)
            if (searchIsName(i, EL_FIELD) != NULL)
                writeIs = true;
        for (int i = 0; i < m_numEventIns; i++)
            if (searchIsName(i, EL_EVENT_IN) != NULL)
                writeIs = true;
        for (int i = 0; i < m_numEventOuts; i++)
            if (searchIsName(i, EL_EVENT_OUT) != NULL)
                writeIs = true;
    }
    if (writeIs) {
        RET_ONERROR( indentf(f, indent) )
        RET_ONERROR( mywritestr(f, "<IS>\n") )
        TheApp->incSelectionLinenumber();                
        indent += TheApp->GetIndent();
    }

    bool isScript = false;
    int scriptMetadataField = -1;
    int scriptUrlField = -1;
    if (getType() == VRML_SCRIPT) {
        scriptUrlField = ((NodeScript *)this)->url_Field();
        scriptMetadataField = getProto()->metadata_Field();
        isScript = true;
    }
    for (int i = 0; i < m_numFields; i++)
        if ((i != scriptUrlField) && (i != scriptMetadataField))
            RET_ONERROR( writeXmlField(f, indent, i, when, isScript) )

    for (int i = 0; i < m_numEventIns; i++)
        RET_ONERROR( writeXmlEventIn(f, indent, i, when) )

    for (int i = 0; i < m_numEventOuts; i++)
        RET_ONERROR( writeXmlEventOut(f, indent, i, when) )

    if (scriptUrlField != -1)
        RET_ONERROR( writeXmlField(f, indent, scriptUrlField, when) )

    if (when == XML_IN_TAG) {
            SocketList::Iterator *j;

            for (int i = 0; i < m_numEventOuts; i++)
                for (j = m_outputs[i].first(); j != NULL; j = j->next()) {
                    Node *dest = j->item().getNode();
                    int field = j->item().getField();
                    if ((dest->getType() == VRML_SCRIPT) &&
                        (hasX3domOnoutputchange() || hasX3domOnclick())) {
                        NodeScript *script = (NodeScript *)dest;
                        for (int n = 0; n < script->url()->getSize(); n++)
                            if (isX3domscript(script->url()->getValue(n))) {
                                RET_ONERROR( mywritestr(f, " ") )
                                if (hasX3domOnclick())
                                    RET_ONERROR( mywritestr(f, "onclick") )
                                else
                                    RET_ONERROR( mywritestr(f, 
                                                            "onoutputchange") )
                                RET_ONERROR( mywritestr(f, "=\'") )
                                const char *name = script->getProto()->
                                    getEventIn(field)->getName(true);
                                RET_ONERROR( mywritestr(f, name) )
                                RET_ONERROR( mywritestr(f, "(event)\' ") )
                            }
                    }    
                }     
    }

    if (writeIs) {
        indent -= TheApp->GetIndent();
        RET_ONERROR( indentf(f, indent) )
        RET_ONERROR( mywritestr(f, "</IS>\n") )
        TheApp->incSelectionLinenumber();                
    }

    return 0;
}

int 
NodeData::writeField(int f, int indent, int i, bool script)
{
    if (!m_proto) return(0);
    bool x3d = m_scene->isX3d();
    const char *oldBase = m_scene->getURL();
    const char *newBase = m_scene->getNewURL();
    bool tempSave = m_scene->isTempSave();

    Field *field = m_proto->getField(i);
    if (m_scene->isInvalidElement(field))
        return 0;
    FieldValue *value = m_fields[i];
    const char *name = field->getName(x3d);
    bool hasIsName = false;
    const char *isName = searchIsName(i, EL_FIELD);
    if (isName)
        hasIsName = true;
    else {
        isName = searchIsName(i, EL_EXPOSED_FIELD);
        if (isName)
            hasIsName = true;
    }
    if (hasIsName)
        RET_ONERROR( writeIs(f, indent, name, isName) )
    else if (value) {
        if (script || (value->getRefs() &&
                       !value->equals(field->getDefault(x3d)))) {
            RET_ONERROR( indentf(f, indent) )
            RET_ONERROR( mywritestr(f , name) )
            RET_ONERROR( mywritestr(f ," ") )
            if ((field->getFlags() & FF_URL) && (!TheApp->GetKeepURLs())) {
                value = rewriteField(value, oldBase, newBase,
                                     m_scene->getWriteFlags());
                RET_ONERROR( value->write(f, indent) )
                if (!tempSave) {
                    setField(i, value);
                    FieldUpdate* fieldUpdate=new FieldUpdate((Node*)this, i);
                    m_scene->UpdateViews(NULL, UPDATE_FIELD,
                                         (Hint *) fieldUpdate);    
                    delete fieldUpdate;
                } else {
                    delete value;
                }
            } else {
                RET_ONERROR( value->write(f, indent) )
            }
        }
    }
    return(0);
}
 
int 
NodeData::writeXmlField(int f, int indent, int i, int when, bool script)
{
    if (!m_proto) return(0);
    const char *oldBase = m_scene->getURL();
    const char *newBase = m_scene->getNewURL();
    bool tempSave = m_scene->isTempSave();
    bool x3d = m_scene->isX3d();

    Field *field = m_proto->getField(i);
    if (m_scene->isInvalidElement(field))
        return 0;
    bool isNode = (field->getType() == SFNODE) || (field->getType() == MFNODE);
    bool isScript = field->getFlags() & FF_IN_SCRIPT;
    bool isScriptUrl = field->getFlags() & FF_SCRIPT_URL;
    bool inTag = (when == XML_IN_TAG);
    FieldValue *value = m_fields[i];
    const char *name = field->getName(x3d);
    const char *isName = searchIsName(i, EL_FIELD); 
    if (when == XML_IS) {
        if (isName)
            RET_ONERROR( writeXmlIs(f, indent, name, isName) )
        else
            return 0;
    } else if (when == XML_PROTO_INSTANCE_FIELD) {
        if (isName)
            RET_ONERROR( writeXmlIs(f, indent, name, isName) )
        else
            RET_ONERROR( writeXmlProtoInstanceField(f, indent, name, value) )
    } else if (value) {
        int size = 1;
        MFString *urls = NULL;
        if (isScriptUrl) {
            urls = ((NodeScript *)this)->url();
            size = urls->getSize();
        }
        if (inTag && isScript)
            return 0;
        if (inTag && isScriptUrl) {
            int count = 0;
            for (int j = 0; j < size; j++)
                if (!isSortOfEcmascript(urls->getValue(j)))
                    count++;
            if (count == 0)
                return 0;
            else if (size > 0) {
                RET_ONERROR( mywritestr(f ,"url='") )
                for (int j = 0; j < size; j++) {
                    if (!isSortOfEcmascript(urls->getValue(j))) {
                        if (j > 0)
                            RET_ONERROR( mywritestr(f, " ") )
                        RET_ONERROR( mywritestr(f, "\"") )
                        RET_ONERROR( mywritestr(f, rewriteURL(urls->getValue(j),
                                                   oldBase, newBase)) )
                        RET_ONERROR( mywritestr(f, "\"") )
                    }
                }
                RET_ONERROR( mywritestr(f, "'") )
            }
        } else if (isScriptUrl) {
            for (int j = 0; j < size; j++) {
                if (isSortOfEcmascript(urls->getValue(j))) {
                    RET_ONERROR( indentf(f, indent + TheApp->GetIndent()) )
                    RET_ONERROR( mywritestr(f, "<![CDATA[") )
                    RET_ONERROR( mywritestr(f, urls->getValue(j)) )
                    RET_ONERROR( indentf(f, indent + TheApp->GetIndent()) )
                    RET_ONERROR( mywritestr(f, "\n") )
                    TheApp->incSelectionLinenumber();
                    RET_ONERROR( indentf(f, indent + TheApp->GetIndent()) )
                    RET_ONERROR( mywritestr(f, "]]>\n") )
                    TheApp->incSelectionLinenumber();
                }
            }
        } else if (isScript) {
            int flags = m_scene->getWriteFlags(); 
            const char *isName = searchIsName(i, EL_FIELD);
            if ((value->isNullNode()) && (!isName))
                flags |= NULL_VALUE;
            else if (isNode)
                flags |= WITHOUT_VALUE;
            if (isName)
                flags |= WITHOUT_VALUE;
            if (isName) {
                return 0;
                RET_ONERROR( indentf(f, indent + TheApp->GetIndent()) )
                RET_ONERROR( mywritef(f, 
                    "<IS> <connect nodeField=\"%s\" protoField=\"%s\"/> </IS>\n",
                    name, isName) )
                TheApp->incSelectionLinenumber();
            } else 
                RET_ONERROR( field->writeElementPart(f, indent, flags) )
            if (isName)
                return 0;
            if (flags & NULL_VALUE)
                if (!(flags & WITHOUT_VALUE)) {
                    RET_ONERROR( mywritestr(f, " value='NULL'") )
                }
            int type = field->getType();
            bool nodeField = (type == SFNODE) || (type == MFNODE);
            if ((flags & NULL_VALUE) && (!(flags & WITHOUT_VALUE))) {
                RET_ONERROR( mywritestr(f, "/>\n") )
                TheApp->incSelectionLinenumber();
            } else if (nodeField ) {
                RET_ONERROR( mywritestr(f, ">\n") )
                TheApp->incSelectionLinenumber();
                RET_ONERROR( value->writeXml(f, indent + TheApp->GetIndent()) )
                RET_ONERROR( indentf(f, indent + TheApp->GetIndent()) )
                RET_ONERROR( mywritestr(f, "</field>\n") )
                TheApp->incSelectionLinenumber();
            } else {
                RET_ONERROR( mywritestr(f, " value=") )
                RET_ONERROR( value->writeXml(f, 0) )
                RET_ONERROR( mywritestr(f, " />\n") )
                TheApp->incSelectionLinenumber();
            }
        } else if (!inTag && !isNode)
            return 0;
        else if (inTag && isNode)
            return 0;
        else if (!value->equals(field->getDefault(x3d))) {                
            if (inTag) {
                RET_ONERROR( mywritestr(f , name) )
                RET_ONERROR( mywritestr(f ,"=") )
            } 
            if ((field->getFlags() & FF_URL) && (!TheApp->GetKeepURLs())) {
                value = rewriteField(value, oldBase, newBase,
                                     m_scene->getWriteFlags());
                RET_ONERROR( value->writeXml(f, indent) )
                if (!tempSave) {
                     setField(i, value);
                     FieldUpdate* fieldUpdate=new FieldUpdate((Node*)this, i);
                     m_scene->UpdateViews(NULL, UPDATE_FIELD,
                                          (Hint *) fieldUpdate);    
                     delete fieldUpdate;
                } else {
                    delete value;
                }
            } else {
                RET_ONERROR( value->writeXml(f, indent) )
            }
            if (inTag) {
                RET_ONERROR( mywritestr(f ," ") )
            } 
        }
    }
    return(0);
}

int 
NodeData::writeEventIn(int f, int indent, int i, bool eventName)
{
    if (!m_proto) return(0);

    bool x3d = m_scene->isX3dv();

    EventIn *eventIn = m_proto->getEventIn(i);
    const char *name = eventIn->getName(x3d);
    const char *isName = searchIsName(i, EL_EVENT_IN); 
    if (isName)
        RET_ONERROR( writeIs(f, indent, name, isName) )
    else if (eventName) {
        RET_ONERROR( mywritestr(f, name) )
        RET_ONERROR( mywritestr(f, "\n") )
        TheApp->incSelectionLinenumber();
    }
    return(0);
}

int 
NodeData::writeXmlEventIn(int f, int indent, int i, int when, bool eventName)
{
    if (!m_proto) return(0);

    bool x3d = m_scene->isX3dv();

    EventIn *eventIn = m_proto->getEventIn(i);
    bool isScript = eventIn->getFlags() & FF_IN_SCRIPT;
    bool inTag = (when == XML_IN_TAG);
    const char *name = eventIn->getName(x3d);
    if (when == XML_IS) {
        const char *isName = searchIsName(i, EL_EVENT_IN); 
        if (isName)
            RET_ONERROR( writeXmlIs(f, indent, name, isName) )
    } else if (inTag && isScript) {
            return 0;
    } else if (isScript) {
        int flags = m_scene->getWriteFlags(); 
        RET_ONERROR( eventIn->write(f, indent, flags) )
        TheApp->incSelectionLinenumber();
    } else if (eventName) {
        RET_ONERROR( mywritestr(f, name) )
        RET_ONERROR( mywritestr(f, "\n") )
        TheApp->incSelectionLinenumber();
    }
    return(0);
}


int
NodeData::writeEventInStr(int f)
{
    if (m_scene->isX3d())
        RET_ONERROR( mywritestr(f, "inputOnly") )
    else
        RET_ONERROR( mywritestr(f, "eventIn") )
    return 0;
}

int
NodeData::writeEventOutStr(int f)
{
    if (m_scene->isX3d())
        RET_ONERROR( mywritestr(f, "outputOnly") )
    else
        RET_ONERROR( mywritestr(f, "eventOut") )
    return 0;
}

int
NodeData::writeFieldStr(int f)
{
    if (m_scene->isX3d())
        RET_ONERROR( mywritestr(f, "initializeOnly") )
    else
        RET_ONERROR( mywritestr(f, "field") )
    return 0;
}

int
NodeData::writeExposedFieldStr(int f)
{
    if (m_scene->isX3d())
        RET_ONERROR( mywritestr(f, "inputOutput") )
    else
        RET_ONERROR( mywritestr(f, "exposedField") )
    return 0;
}


int 
NodeData::writeEventOut(int f, int indent, int i, bool eventName)
{
    if (!m_proto) return(0);

    bool x3d = m_scene->isX3dv();

    EventOut *eventOut = m_proto->getEventOut(i);
    const char *name = eventOut->getName(x3d);
    const char *isName = searchIsName(i, EL_EVENT_OUT); 
    if (isName)
        RET_ONERROR( writeIs(f, indent, name, isName) )
    else if (eventName) {
        RET_ONERROR( mywritestr(f, name) )
        RET_ONERROR( mywritestr(f, "\n") )
        TheApp->incSelectionLinenumber();
    }
    return(0);
}

int 
NodeData::writeXmlEventOut(int f, int indent, int i, int when, bool eventName)
{
    if (!m_proto) return(0);

    bool x3d = m_scene->isX3dv();

    EventOut *eventOut = m_proto->getEventOut(i);
    bool isScript = eventOut->getFlags() & FF_IN_SCRIPT;
    bool inTag = (when == XML_IN_TAG);
    const char *name = eventOut->getName(x3d);
    if (when == XML_IS) {
        const char *isName = searchIsName(i, EL_EVENT_OUT); 
        if (isName)
            RET_ONERROR( writeXmlIs(f, indent, name, isName) )
    } else if (inTag && isScript) {
            return 0;
    } else if (isScript) {
        int flags = m_scene->getWriteFlags(); 
        RET_ONERROR( eventOut->write(f, indent, flags) )
        TheApp->incSelectionLinenumber();
    } else if (eventName) {
        RET_ONERROR( mywritestr(f, name) )
        RET_ONERROR( mywritestr(f, "\n") )
        TheApp->incSelectionLinenumber();
    }
    return(0);
}

int 
NodeData::writeRoutes(int f, int indent) const
{
    if (!m_proto) return(0);

    bool x3d = m_scene->isX3dv();

    SocketList::Iterator *j;

    for (int i = 0; i < m_numEventIns; i++) {
        for (j = m_inputs[i].first(); j != NULL; j = j->next()) {
            if (j->item().getNode()->getFlag(NODE_FLAG_TOUCHED)) {
                Node *src = j->item().getNode();
                int field = j->item().getField();
                if ((m_scene->getWriteFlags() & X3DOM) && 
                    (j->item().getNode()->getType() == VRML_SCRIPT)) {
                    NodeScript *script = (NodeScript *)j->item().getNode();
                    bool flag = false;
                    for (int i = 0; i < script->url()->getSize(); i++)
                        if (isX3domscript(script->url()->getValue(i)))
                            flag = true;
                    if (flag)
                        continue;
                            
                }
                if (m_scene->isPureVRML() &&  
                    (matchNodeClass(PARAMETRIC_GEOMETRY_NODE) || 
                     src->matchNodeClass(PARAMETRIC_GEOMETRY_NODE)))
                    continue;
                MyString routeString = m_scene->createRouteString(
                      src->getName(), 
                      src->getProto()->getEventOut(field)->getName(x3d),
                      m_name, getProto()->getEventIn(i)->getName(x3d));
                      m_scene->addRouteString(routeString);
            }
        }
    }

    for (int i = 0; i < m_numEventOuts; i++) {
        for (j = m_outputs[i].first(); j != NULL; j = j->next()) {
            if (j->item().getNode()->getFlag(NODE_FLAG_TOUCHED)) {
                Node *dst = j->item().getNode();
                int field = j->item().getField();
                if (m_scene->isPureVRML() &&  
                    (matchNodeClass(PARAMETRIC_GEOMETRY_NODE) || 
                     dst->matchNodeClass(PARAMETRIC_GEOMETRY_NODE)))
                    continue;
                MyString routestring = m_scene->createRouteString(m_name,
                         getProto()->getEventOut(i)->getName(x3d),
                         dst->getName(),
                         dst->getProto()->getEventIn(field)->getName(x3d));
                m_scene->addRouteString(routestring);
            }
        }
    }

#ifndef HAVE_ROUTE_AT_END
    if (indent==0)
       RET_ONERROR( m_scene->writeRouteStrings(f, indent) )
#endif
    return 0;
}

void
NodeData::removeRoutes(void)
{
    if (!m_proto) return;

    for (int i = 0; i < m_numEventIns; i++) {
        while (m_inputs[i].size() > 0) {
            SocketList::Iterator *j = m_inputs[i].first();
            Node *src = j->item().getNode();
            int field = j->item().getField();
            m_scene->deleteRoute(src, field, (Node *)this, i);
        }
    }

    for (int i = 0; i < m_numEventOuts; i++) {
        while (m_outputs[i].size() > 0) {
            SocketList::Iterator *j = m_outputs[i].first();
            Node *dst = j->item().getNode();
            int field = j->item().getField();
            m_scene->deleteRoute((Node *)this, i, dst, field);
        }
    }
}

int         
Node::writeCDataFunctionFields(int filedes, int languageFlag, 
                               bool forward, bool cont)
{
    for (int i = 0; i < m_numFields; i++) {
        Field *field = m_proto->getField(i);
        FieldValue *value = m_fields[i];

        if (field->getType() == SFNODE) {
            if (value) {
                Node *node = ((SFNode *)value)->getValue();
                if (node && (node != this)) { // avoid simple cyclic scenegraph
                    RET_ONERROR( node->writeCDataFunction(filedes, 
                                                          languageFlag,
                                                          forward, cont) )
                }
            }
        } else if (field->getType() == MFNODE) {
            if (value) {
                MFNode *nodes = (MFNode *)value;
                for (int i = 0; i < nodes->getSize(); i++) {
                    Node *node = nodes->getValue(i);
                    if (node && (node != this)) { //avoid simple cyclic scenegraph
                        RET_ONERROR( node->writeCDataFunction(filedes, 
                                                              languageFlag,
                                                              forward, cont) )
                    }
                }
            }
        }
    }
    if ((!cont) && (!isConvertedInCurveAnimaton()))
        RET_ONERROR( mywritestr(filedes , "    }\n") )
    return 0;
}

int 
Node::writeCGetParent(int filedes, int languageFlag)
{
    if (getType() == DUNE_CURVE_ANIMATION)
        return 0;

    RET_ONERROR( mywritestr(filedes, "         if (") )
    RET_ONERROR( writeCVariable(filedes, languageFlag) )
    if (languageFlag & JAVA_SOURCE)
        RET_ONERROR( mywritestr(filedes, " != null") )
    RET_ONERROR( mywritestr(filedes, ")\n") )

    RET_ONERROR( mywritestr(filedes, "              ") )
    RET_ONERROR( writeCVariable(filedes, languageFlag) )
    RET_ONERROR( mywritestr(filedes, ".m_parent = ") )

    if (hasParent()) {
        RET_ONERROR( getParent()->writeCVariable(filedes, languageFlag) )
    } else {
        if (languageFlag & JAVA_SOURCE)
            RET_ONERROR( mywritestr(filedes, "null") )            
        else 
            RET_ONERROR( mywritestr(filedes, "NULL") )            
    } 
    RET_ONERROR( mywritestr(filedes, ";\n") )  
    return 0;
}

int         
Node::writeCDataFunction(int filedes, int languageFlag, bool forward, bool cont)
{
    if (getWritten())
        return 0;

    if (m_convertedNodes.size() > 0) {
        for (int i = 0; i < m_convertedNodes.size(); i++)
            RET_ONERROR( m_convertedNodes[i]->writeCDataFunction(filedes, 
                                                                 languageFlag,
                                                                 forward,
                                                                 cont) )
        return 0;
    }

    if (forward)
        writeCDataFunctionFields(filedes, languageFlag, forward, cont);

    if (m_numberCDataFunctions > 0) {
        if (!cont){
            RET_ONERROR( mywritestr(filedes , "    ") )
            if ((languageFlag & MANY_JAVA_CLASSES) &&
                !isConvertedInCurveAnimaton()) {
                RET_ONERROR( mywritestr(filedes , "static ") )
                RET_ONERROR( mywritef(filedes , "void data%sFunction%d() {\n",
                             TheApp->getCPrefix(),
                             m_scene->getNumDataFunctions()) )
            }
        }
        m_scene->increaseNumDataFunctions();
        RET_ONERROR( mywritestr(filedes, "    ") )
        if (languageFlag & MANY_JAVA_CLASSES)
            RET_ONERROR( mywritestr(filedes, "    ") )
        RET_ONERROR( writeCVariable(filedes, languageFlag) )    
        RET_ONERROR( mywritestr(filedes, " = new ") )
            RET_ONERROR( mywritestr(filedes, getClassName()) )
        RET_ONERROR( mywritestr(filedes, "();\n") )    

        if (languageFlag & JAVA_SOURCE) {       
            if (languageFlag & MANY_JAVA_CLASSES)
                RET_ONERROR( mywritestr(filedes, "    ") )
        }
        RET_ONERROR( mywritestr(filedes, "    ") )
        if (languageFlag & C_SOURCE)
            RET_ONERROR( mywritestr(filedes, "self->") )    
        RET_ONERROR( writeCVariable(filedes, languageFlag) )    
        RET_ONERROR( mywritestr(filedes, ".m_parent = ") )

        if (hasParent()) {
            if (languageFlag & (C_SOURCE | CC_SOURCE))        
                RET_ONERROR( mywritestr(filedes, "&") )    
            if (languageFlag & C_SOURCE)
                RET_ONERROR( mywritestr(filedes, "self->") )    
            RET_ONERROR( getParent()->writeCVariable(filedes, languageFlag) )
        } else {
            if (languageFlag & JAVA_SOURCE)
                RET_ONERROR( mywritestr(filedes, "null") )            
            else 
                RET_ONERROR( mywritestr(filedes, "NULL") )            
        } 
        RET_ONERROR( mywritestr(filedes, ";\n") )  

        if ( m_numberCDataFunctions > 0) { 
            if (languageFlag & MANY_JAVA_CLASSES)
                RET_ONERROR( mywritef(filedes , 
                                      "        new %s%sDataClass0();\n", 
                                      TheApp->getPrefix(), getVariableName()) )
            else 
                RET_ONERROR( mywritef(filedes , 
                                      "        %s%sDataFunction0();\n", 
                                      TheApp->getPrefix(), getVariableName()) )
        }
        for (int i = 1; i < m_numberCDataFunctions; i++)
            if (languageFlag & MANY_JAVA_CLASSES)
                RET_ONERROR( mywritef(filedes , 
                                      "        new %s%sDataClass%d();\n", 
                                      TheApp->getPrefix(), getVariableName(), 
                                      i) )
            else 
                RET_ONERROR( mywritef(filedes , 
                                      "        %s%sDataFunction%d();\n", 
                                      TheApp->getPrefix(), getVariableName(), 
                                      i) )
    }

    if (!forward)
        writeCDataFunctionFields(filedes, languageFlag, forward, cont);

    if (hasProtoNodes()) {
        Node *protoRoot = ((NodePROTO *)this)->getProtoRoot();
        RET_ONERROR( protoRoot->writeCDataFunction(filedes, languageFlag, 
                                                   forward, cont))

        NodePROTO *nodeProto = (NodePROTO *)this;
        for (int i = 1; i < nodeProto->getNumProtoNodes(); i++) {
            RET_ONERROR( nodeProto->getProtoNode(i)->writeCDataFunction(
                             filedes, languageFlag, cont) )
       }
    }

    setWritten(true);
    return 0;
}

int
Node::writeC(int f, int languageFlag)
{
    if (m_convertedNodes.size() > 0) {
        for (int i = 0; i < m_convertedNodes.size(); i++)
            RET_ONERROR( m_convertedNodes[i]->writeC(f, languageFlag) )
        return 0;
    }

    if (getType() == VRML_COMMENT)
        return 0;
    if (m_proto->isMismatchingProto())
        return 0;   

    if (isMeshBasedNode()) {
        RET_ONERROR( mywritef(f, "    ") )
        if (languageFlag & C_SOURCE)
            RET_ONERROR( mywritef(f, "setGlName(&self->%s, %d);\n", getVariableName(),
                                  m_scene->getGlName()) )
        else {
            if (languageFlag & JAVA_SOURCE) {
                MyString className = "";
                if (hasName()) 
                    className += TheApp->getCSceneGraphName();
                else {
                   className += TheApp->getCPrefix();
                   className += getVariableName();
                }
                RET_ONERROR( mywritef(f, "%s.", (const char *)className) )
            }
            RET_ONERROR( mywritef(f, "%s.setGlName(%d);\n", getVariableName(), 
                                  m_scene->getGlName()) )
        }
        CGlNameData data;
        data.glName = m_scene->getGlName();
        data.node = this;
        data.nodeName = "";
        data.nodeName += getVariableName();
        m_scene->m_glNameData.append(data);
        m_scene->increaseGlName();
    }
    if (getType() == VRML_TRANSFORM) {
        int numRouteSources = 0;
        for (int i = 0; i < m_numEventIns; i++)
            for (SocketList::Iterator *j = m_inputs[i].first(); j != NULL; 
                 j = j->next()) 
                if (j->item().getNode()->getProto()->isCRouteSource()) {
                    numRouteSources++;
                }
        RET_ONERROR( mywritestr(f, "    ") )
        if (languageFlag & JAVA_SOURCE)
            RET_ONERROR( mywritestr(f, "        ") )
        if (languageFlag & C_SOURCE)
            RET_ONERROR( mywritestr(f, "self->") )
        MyString className = "";
        if (languageFlag & JAVA_SOURCE) {
            if (hasName()) 
                className += TheApp->getCSceneGraphName();
            else {
                className += TheApp->getCPrefix();
                className += getVariableName();
            }
            RET_ONERROR( mywritef(f, "%s.", (const char *)className) )
        }
        RET_ONERROR( mywritef(f, "%s.route_sources = ", getVariableName()) )
        if (numRouteSources == 0) {
            if (languageFlag & JAVA_SOURCE)
                RET_ONERROR( mywritestr(f, " null;\n") )
            else
                RET_ONERROR( mywritestr(f, " NULL;\n") )
        } else {
            if (languageFlag & C_SOURCE)
                RET_ONERROR( mywritef(f, 
                                 "(%sNode **)malloc(%d * sizeof(%sNode *));\n",
                                 TheApp->getCPrefix(), numRouteSources,
                                 TheApp->getCPrefix()) )
            else if (languageFlag & CC_SOURCE)
                RET_ONERROR( mywritef(f, "new %sNode*[%d];\n",
                                      TheApp->getCPrefix(), numRouteSources) )
            else if (languageFlag & JAVA_SOURCE)
                RET_ONERROR( mywritef(f, "new %sNode[%d];\n",
                                      TheApp->getCPrefix(), numRouteSources) )
        }
        numRouteSources = 0;
        for (int i = 0; i < m_numEventIns; i++)
            for (SocketList::Iterator *j = m_inputs[i].first(); j != NULL; 
                 j = j->next()) 
                if (j->item().getNode()->getProto()->isCRouteSource()) {
                    RET_ONERROR( mywritestr(f, "    ") ) 
                    if (languageFlag & JAVA_SOURCE)
                        RET_ONERROR( mywritef(f, "        %s.", 
                                                 (const char *)className) )
                    if (languageFlag & C_SOURCE)
                        RET_ONERROR( mywritestr(f, "self->") )
                    RET_ONERROR( mywritef(f, "%s.route_sources[%d] = ", 
                                 getVariableName(), numRouteSources) )
                    if (languageFlag & (C_SOURCE | CC_SOURCE))
                        RET_ONERROR( mywritestr(f, "&") ) 
                    if (languageFlag & JAVA_SOURCE)
                        RET_ONERROR( mywritef(f, "%s.", 
                                                 (const char *)className) )
                    if (languageFlag & C_SOURCE)
                        RET_ONERROR( mywritestr(f, "self->") )
                    RET_ONERROR( mywritestr(f, 
                        j->item().getNode()->getVariableName()) )
                    RET_ONERROR( mywritestr(f, ";\n") ) 
                    numRouteSources++;
                }

        RET_ONERROR( mywritef(f, "    ") )
        if (languageFlag & C_SOURCE)
            RET_ONERROR( mywritestr(f, "self->") )
        if (languageFlag & JAVA_SOURCE) {
            MyString className = "";
            if (hasName()) 
                className += TheApp->getCSceneGraphName();
            else {
                className += TheApp->getCPrefix();
                className += getVariableName();
            }
            RET_ONERROR( mywritef(f, "        %s.", (const char *)className) )
        }
        RET_ONERROR( mywritef(f, "%s.num_route_source = %d;\n", 
                                 getVariableName(), numRouteSources) )
        
    }

    // allows several nodetypes to output a warning via inheritance
    writeCWarning();

    bool writeInside = true;
    if (languageFlag & MANY_JAVA_CLASSES)
        if (writeJavaOutsideClass(languageFlag))
            writeInside = false;

    if (languageFlag & C_SOURCE) {
        RET_ONERROR( mywritestr(f, "    ") )
        RET_ONERROR( mywritestr(f, getClassName()) )
        RET_ONERROR( mywritestr(f, "Init(&") )
        RET_ONERROR( writeCVariable(f, languageFlag) )
        RET_ONERROR( mywritestr(f, ");\n") )    
    }


    if (languageFlag & (C_SOURCE | CC_SOURCE)) {
            RET_ONERROR( mywritestr(f, "    ") )      
        if (languageFlag & C_SOURCE)
            RET_ONERROR( mywritestr(f, "self->") )      
        RET_ONERROR( mywritestr(f, getVariableName()) )    
        RET_ONERROR( mywritef(f, ".m_parent = (%sNode *)", TheApp->getCPrefix())
                   )  
        if (getParent() == NULL)
            RET_ONERROR( mywritestr(f, "NULL") )
        else { 
            RET_ONERROR( mywritestr(f, "&") )
            if (languageFlag & C_SOURCE)
                RET_ONERROR( mywritestr(f, "self->") )      
            RET_ONERROR( mywritestr(f, getParent()->getVariableName()) )    
        }
        RET_ONERROR( mywritestr(f, ";\n") )  
    }
    if (languageFlag & (C_SOURCE | CC_SOURCE)) {
        for (int i = 0; i < m_numFields; i++)
            RET_ONERROR( writeCElement(f, EL_FIELD, i, languageFlag, true) )    
        for (int i = 0; i < m_numFields; i++)
            RET_ONERROR( writeCElement(f, EL_FIELD, i, languageFlag, false) )
        for (int i = 0; i < m_numEventIns; i++)
            RET_ONERROR( writeCElement(f, EL_EVENT_IN, i, languageFlag, true) )
        for (int i = 0; i < m_numEventIns; i++)
            RET_ONERROR( writeCElement(f, EL_EVENT_IN, i, languageFlag, false) )
        for (int i = 0; i < m_numEventOuts; i++)
            RET_ONERROR( writeCElement(f, EL_EVENT_OUT, i, languageFlag, true) )
        for (int i = 0; i < m_numEventOuts; i++)
            RET_ONERROR( writeCElement(f, EL_EVENT_OUT, i, languageFlag, false) 
                       )    
    } 

    if (writeInside) {
        if (hasProtoNodes()) {
            if (languageFlag & JAVA_SOURCE) {       
                RET_ONERROR( mywritestr(f, "    ") )
                if (languageFlag & MANY_JAVA_CLASSES)
                    RET_ONERROR( mywritestr(f, "    ") )
            }
            Node *protoRoot = ((NodePROTO *)this)->getProtoRoot();
            RET_ONERROR( protoRoot->writeC(f, languageFlag) )

            RET_ONERROR( mywritestr(f, "    ") )
            RET_ONERROR( writeCVariable(f, languageFlag) )    
            RET_ONERROR( mywritestr(f, ".m_protoRoot = ") )
            if (languageFlag & (C_SOURCE | CC_SOURCE))
                RET_ONERROR( mywritestr(f, "&") )  
            RET_ONERROR( protoRoot->writeCVariable(f, languageFlag) )
            RET_ONERROR( mywritestr(f, ";\n") )  

            NodePROTO *nodeProto = (NodePROTO *)this;
            for (int i = 1; i < nodeProto->getNumProtoNodes(); i++)
                 RET_ONERROR(nodeProto->getProtoNode(i)->writeC(f, 
                                                                languageFlag))
        }  
    }
 
    if (languageFlag & C_SOURCE)
        if (this == m_scene->getRoot())
            RET_ONERROR( mywritestr(f, "    {\n") )    

    if (languageFlag & C_SOURCE) {
        RET_ONERROR( mywritestr(f, "    self->") )
        RET_ONERROR( mywritestr(f, getVariableName()) )
        RET_ONERROR( mywritestr(f, ".") )
        RET_ONERROR( mywritestr(f, "m_") )
        RET_ONERROR( mywritestr(f, "type = ") )
        if (hasProtoNodes())
            RET_ONERROR( mywritef(f, "%d", m_scene->getProtoType(m_proto)) )
        else        
            RET_ONERROR( mywritef(f, "%d", getType()) )
        RET_ONERROR( mywritestr(f, ";\n") )    

        RET_ONERROR( mywritestr(f, "    self->") )
        RET_ONERROR( mywritestr(f, getVariableName()) )
        RET_ONERROR( mywritestr(f, ".") )
        RET_ONERROR( mywritestr(f, "m_") )
        RET_ONERROR( mywritestr(f, "data = NULL") )
        RET_ONERROR( mywritestr(f, ";\n") )    
    } 

    return 0;
}

int
Node::writeCDataAsFunctions(int f, int languageFlag, bool cont)
{
    if (m_convertedNodes.size() > 0) {
        for (int i = 0; i < m_convertedNodes.size(); i++)
            RET_ONERROR( m_convertedNodes[i]->writeCDataAsFunctions(f, 
                                                                   languageFlag)
                       )
        return 0;
    }

    if (getType() == VRML_COMMENT)
        return 0;
    if (m_proto->isMismatchingProto())
        return 0;  

    // do not write multiple declarations for USE'd nodes
    if (getFlag(NODE_FLAG_TOUCHED))
        return 0;
    setFlag(NODE_FLAG_TOUCHED); 

    if (languageFlag & (C_SOURCE | CC_SOURCE))
        return 0;

    m_numberCDataFunctions = 0;

    for (int i = 0; i < m_numFields; i++)
        RET_ONERROR( writeCElementFunction(f, EL_FIELD, i, languageFlag, true,
                                           cont) )
    for (int i = 0; i < m_numFields; i++)
        RET_ONERROR( writeCElementFunction(f, EL_FIELD, i, languageFlag, false,
                                           cont) )
    for (int i = 0; i < m_numEventIns; i++)
        RET_ONERROR( writeCElementFunction(f, EL_EVENT_IN, i, languageFlag, 
                                           true, cont))
    for (int i = 0; i < m_numEventIns; i++)
        RET_ONERROR( writeCElementFunction(f, EL_EVENT_IN, i, languageFlag, 
                                           false, cont))
    for (int i = 0; i < m_numEventOuts; i++)
        RET_ONERROR( writeCElementFunction(f, EL_EVENT_OUT, i, languageFlag, 
                                           true, cont))
    for (int i = 0; i < m_numEventOuts; i++)
        RET_ONERROR( writeCElementFunction(f, EL_EVENT_OUT, i, languageFlag, 
                                           false, cont))

    if (hasProtoNodes()) {
        Node *protoRoot = ((NodePROTO *)this)->getProtoRoot();
        RET_ONERROR( protoRoot->writeCDataAsFunctions(f, languageFlag) )    

        NodePROTO *nodeProto = (NodePROTO *)this;
        for (int i = 1; i < nodeProto->getNumProtoNodes(); i++)
            RET_ONERROR( nodeProto->getProtoNode(i)->writeCDataAsFunctions(
                             f, languageFlag, cont) )
    }
    return 0;
}

int
Node::writeCElementFunction(int f, int elementType, int i, int languageFlag, 
                            bool nodeFlag, bool cont)
{
    if (m_convertedNodes.size() > 0) {
        for (int j = 0; j < m_convertedNodes.size(); j++)
            RET_ONERROR( m_convertedNodes[j]->writeCElementFunction(f, 
                               elementType, i, languageFlag, nodeFlag, cont) )
        return 0;
    }

    if ((elementType == EL_FIELD) && (i == m_proto->metadata_Field()))
        return 0;

    Element *element = m_proto->getElement(elementType, i);
    int type = element->getType();

    if (m_scene->isInvalidElement(element))
        return 0;

    if (nodeFlag)
        if ((type != SFNODE) && (type != MFNODE))
            return 0;

    FieldValue *value = NULL; 
    switch(elementType) {
      case EL_FIELD:
        value =m_fields[i];
        break;
      case EL_EVENT_IN:
        if (m_proto->getEventIn(i)->getExposedField() != NULL)
            return 0;
        value = typeDefaultValue(type);
        break;
      case EL_EVENT_OUT:        
        if (m_proto->getEventOut(i)->getExposedField() != NULL)
            return 0;
        value = typeDefaultValue(type);
        break;
      default:
        assert(0);
    }

    if (value->writeType(languageFlag)) {
        if ((!cont) && languageFlag & MANY_JAVA_CLASSES) {
            RET_ONERROR( mywritef(f, "    class %s%sDataClass%d {\n", 
                                  TheApp->getPrefix(), getVariableName(), 
                                  m_numberCDataFunctions,  
                                  (const char*)getClassName()) )    
            RET_ONERROR( mywritef(f, "        public %s%sDataClass%d() {\n", 
                                  TheApp->getPrefix(), getVariableName(), 
                                  m_numberCDataFunctions) )
        } else
            RET_ONERROR( mywritef(f, "    void %sDataFunction%d() {\n", 
                                  getVariableName(), m_numberCDataFunctions,  
                                  (const char*)getClassName()) )    
        m_numberCDataFunctions++; 
        RET_ONERROR( writeCElement(f, elementType, i, languageFlag, nodeFlag) )
        if (languageFlag & MANY_JAVA_CLASSES)
            RET_ONERROR( mywritestr(f, "        }\n") )    
        RET_ONERROR( mywritestr(f, "    }\n") )    
    } else if (!nodeFlag) {
        // write long array into a series of functions to fight against the
        // java "code too long" (64 KB) problem
        MFieldValue *mvalue = (MFieldValue *)value;
        int javaCodeToLongLimit = 1 << 16;
        int javaSizeOfLine = 8;
        int maxNumberElements = TheApp->GetWrittenElementsPerJavaArray();
        if (maxNumberElements == -1)
            maxNumberElements = javaCodeToLongLimit / mvalue->getStride() / 
                                javaSizeOfLine / 4;
        int numberFunctions = mvalue->getSFSize() / maxNumberElements + 1;
        int offset = 0;
        for (int i = 0; i < numberFunctions; i++) {
            if ((!cont) && languageFlag & MANY_JAVA_CLASSES) {
                RET_ONERROR( mywritef(f, "    class %s%sDataClass%d {\n", 
                                      TheApp->getPrefix(), getVariableName(), 
                                      m_numberCDataFunctions,  
                                      (const char*)getClassName()) )    
                RET_ONERROR( mywritef(f, "        public %s%sDataClass%d() {\n",
                                      TheApp->getPrefix(), getVariableName(), 
                                      m_numberCDataFunctions) )    
            } else
                RET_ONERROR( mywritef(f, "    void %sDataFunction%d() {\n", 
                                      getVariableName(), 
                                      m_numberCDataFunctions) )    
            m_numberCDataFunctions++; 
            const char *elementName = element->getName(true); 
            if (i == 0) {
                RET_ONERROR( mywritestr(f, "        ") )
                if (languageFlag & MANY_JAVA_CLASSES)
                    RET_ONERROR( mywritestr(f, "    ") )
                RET_ONERROR( writeCVariable(f, languageFlag) )    
                RET_ONERROR( mywritef(f, ".%s = ",
                                      (const char *)element->getName(true)) )
                int size = mvalue->getSFSize() * mvalue->getStride();
                if (size > 0)
                    RET_ONERROR( mywritef(f, "new %s[%d];\n",
                                          value->getTypeC(languageFlag), size) )
                else 
                    RET_ONERROR( mywritestr(f, "null;\n") )
            }
            
            RET_ONERROR( mywritestr(f, "        ") )
            if (languageFlag & MANY_JAVA_CLASSES)
                RET_ONERROR( mywritestr(f, "    ") )
            RET_ONERROR( mywritestr(f, getClassName()) )
            RET_ONERROR( mywritestr(f, " v = ") )
            RET_ONERROR( writeCVariable(f, languageFlag) )    
            RET_ONERROR( mywritestr(f, ";\n") )

            RET_ONERROR( mvalue->writeJavaLongArray(f, languageFlag,
                  elementName, offset, maxNumberElements, 
                  TheApp->isWonderlandModuleExport() &&
                  (element->getFlags() & FF_WONDERLAND_ART), m_scene->getURL()) )
            if (languageFlag & MANY_JAVA_CLASSES)  
                RET_ONERROR( mywritestr(f, "        }\n") )
            RET_ONERROR( mywritestr(f, "    }\n") )
            offset += maxNumberElements;    
        }
    }

    if (nodeFlag && (value->getType() == SFNODE)) {
        if (value) {
            Node *node = ((SFNode *)value)->getValue();
            if (node && (node != this)) // avoid simple cyclic scenegraph
                 RET_ONERROR( node->writeCDataAsFunctions(f, languageFlag) )
        }
    } else if (nodeFlag && (value->getType() == MFNODE)) {
        if (value) {
            MFNode *nodes = (MFNode *)value;
            for (int i = 0; i < nodes->getSize(); i++) {
                Node *node = nodes->getValue(i);
                if (node && (node != this)) // avoid simple cyclic scenegraph
                     RET_ONERROR(node->writeCDataAsFunctions(f, languageFlag) )
            }
        }
    }

    return 0;
}

int 
Node::writeCElement(int f, int elementType, int i, int languageFlag, 
                    bool nodeFlag)
{
    if (!m_proto) return(0);

    Element *element = m_proto->getElement(elementType, i);

    if (m_scene->isInvalidElement(element))
        return 0;

    if (element->getFlags() & FF_X3DOM_ONLY)
        return 0;

    int type = element->getType();

    FieldValue *value = NULL; 
    switch(elementType) {
      case EL_FIELD:
        value = m_fields[i];
        break;
      case EL_EVENT_IN:
        if (m_proto->getEventIn(i)->getExposedField() != NULL)
            return 0;
        value = typeDefaultValue(type);
        break;
      case EL_EVENT_OUT:        
        if (m_proto->getEventOut(i)->getExposedField() != NULL)
            return 0;
        value = typeDefaultValue(type);
        break;
      default:
        assert(0);
    }

    const char *name = element->getName(true);

    if (nodeFlag && (value->getType() == SFNODE)) {
        if (value) {
            Node *node = ((SFNode *)value)->getValue();
            if (node && (node != this)) // avoid simple cyclic scenegraph
                 RET_ONERROR( node->writeC(f, languageFlag) )
        }
    } else if (nodeFlag && (value->getType() == MFNODE)) {
        if (value) {
            MFNode *nodes = (MFNode *)value;
            for (int i = 0; i < nodes->getSize(); i++) {
                Node *node = nodes->getValue(i);
                if (node && (node != this)) { // avoid simple cyclic scenegraph
                     RET_ONERROR( node->writeC(f, languageFlag) )
                }
            }
        }
    } else if (!nodeFlag) {
        if (value->isArrayInC()) {
            if (languageFlag & JAVA_SOURCE)
                RET_ONERROR( mywritestr(f, "    ") )
            RET_ONERROR( mywritestr(f, "    {\n") )
        }
        RET_ONERROR( mywritestr(f, "    ") )
        if (languageFlag & JAVA_SOURCE) {       
            RET_ONERROR( mywritestr(f, "    ") )
            if (languageFlag & MANY_JAVA_CLASSES)  
                RET_ONERROR( mywritestr(f, "    ") )
        }
        if (value->isArrayInC()) {
            if (languageFlag & (C_SOURCE | CC_SOURCE))
                RET_ONERROR( mywritestr(f, "static ") )
            RET_ONERROR( mywritestr(f, value->getTypeC(languageFlag)) )
            RET_ONERROR( mywritestr(f, " ") )
        } else {
            RET_ONERROR( writeCVariable(f, languageFlag) )    
            RET_ONERROR( mywritestr(f, ".") )
        }
        if (TheApp->isWonderlandModuleExport() &&
            (element->getFlags() & FF_WONDERLAND_ART)) {
                 RET_ONERROR( value->writeCWonderlandArt(f, name, languageFlag) 
                            )
        } else
             RET_ONERROR( value->writeC(f, name, languageFlag) )
        if (value->isArrayInC()) {
            if (languageFlag & JAVA_SOURCE) {
                RET_ONERROR( mywritestr(f, "    ") )
                if (languageFlag & MANY_JAVA_CLASSES)  
                    RET_ONERROR( mywritestr(f, "    ") )
            }
            RET_ONERROR( mywritestr(f, "    ") )
            RET_ONERROR( writeCVariable(f, languageFlag) )    
            RET_ONERROR( mywritestr(f, ".") )
            RET_ONERROR( mywritestr(f, name) )
            RET_ONERROR( mywritestr(f, " = ") )
            bool isNull = false;
            if (languageFlag & (C_SOURCE | CC_SOURCE))
                if (value->isMFieldValue()) {
                    MFieldValue *mvalue = (MFieldValue *)value;
                    if (mvalue->getSFSize() == 0)
                        isNull = true;
                }
            if (isNull)
                RET_ONERROR( mywritestr(f, "NULL") )
            else {
                RET_ONERROR( mywritestr(f, "m_") )
                RET_ONERROR( mywritestr(f, name) )
            }
            RET_ONERROR( mywritestr(f, ";\n") )
            if (languageFlag & (C_SOURCE | CC_SOURCE)) {       
                RET_ONERROR( mywritestr(f, "    ") )
                RET_ONERROR( writeCVariable(f, languageFlag) )
                RET_ONERROR( mywritestr(f, ".") )
                RET_ONERROR( mywritestr(f, name) )
                RET_ONERROR( mywritestr(f, "_length") )
                RET_ONERROR( mywritestr(f, " = ") )
                if (value->isMFieldValue() && 
                    (((MFieldValue *)value)->getSFSize() == 0)) {
                    RET_ONERROR( mywritestr(f, "0") )
                } else {
                    RET_ONERROR( mywritestr(f, "sizeof(") )
                    RET_ONERROR( mywritestr(f, "m_") )
                    RET_ONERROR( mywritestr(f, name) )
                    RET_ONERROR( mywritestr(f, ") / sizeof(") )
                    RET_ONERROR( mywritestr(f, value->getTypeC(languageFlag)) )
                    RET_ONERROR( mywritestr(f, ")") )
                }
                RET_ONERROR( mywritestr(f, ";\n") )
            }
            if (languageFlag & JAVA_SOURCE) {
                RET_ONERROR( mywritestr(f, "    ") )
                if (languageFlag & MANY_JAVA_CLASSES)  
                    RET_ONERROR( mywritestr(f, "    ") )
            }
            RET_ONERROR( mywritestr(f, "    }\n") )
        }
    }
    return 0;
}

int
NodeData::writeCVariable(int f, int languageFlag)
{
    if (languageFlag & C_SOURCE)
        RET_ONERROR( mywritestr(f, "self->") )
    if (languageFlag & MANY_JAVA_CLASSES) {
        if (needExtraJavaClass() || isConvertedInCurveAnimaton())
            RET_ONERROR( mywritef(f, "%s%s.", TheApp->getCPrefix(),
                                  getVariableName()) )
        else if (hasName() || (m_scene->getRoot() == this))
            RET_ONERROR( mywritef(f, "%s.", TheApp->getCSceneGraphName()) )
    }
    RET_ONERROR( mywritestr(f, getVariableName()) )
    return 0;
}

int
Node::writeCProcessEventCallback(int f, int languageFlag) 
{
    RET_ONERROR( mywritestr(f, TheApp->getCPrefix()) )
    RET_ONERROR( mywritestr(f, m_proto->getCName(true)) )
    if (languageFlag & C_SOURCE)
        RET_ONERROR( mywritestr(f, "ProcessEventCallback") )
    else if (languageFlag & CC_SOURCE) {
        RET_ONERROR( mywritestr(f, "::") )
        RET_ONERROR( mywritestr(f, "processEventCallback") )
    } else if (languageFlag & JAVA_SOURCE) {
        RET_ONERROR( mywritestr(f, ".") )
        RET_ONERROR( mywritestr(f, m_proto->getProcessEventCallbackName()) )
    }
    return 0;
}

int
Node::writeCProcessEvent(int f, int indent, int languageFlag, 
                         const char *eventName)
{
    if (getType() == DUNE_CURVE_ANIMATION) {
        NodeCurveAnimation *curve = (NodeCurveAnimation *) this;
        Node *inter = curve->getPositionInterpolator();
        RET_ONERROR( inter->writeCProcessEvent(f, indent, languageFlag, 
                                               eventName) )
        inter = curve->getOrientationInterpolator();
        RET_ONERROR( inter->writeCProcessEvent(f, indent, languageFlag, 
                                               eventName) )
        return 0; 
    } 

    RET_ONERROR( indentf(f, indent) )
    if (languageFlag & JAVA_SOURCE)
        RET_ONERROR( mywritestr(f, "    ") )
    RET_ONERROR( mywritestr(f, "if (") )
    RET_ONERROR( writeCProcessEventCallback(f, languageFlag) )
    RET_ONERROR( mywritestr(f, " != ") )
    if (languageFlag & JAVA_SOURCE)
        RET_ONERROR( mywritestr(f, "null") )
    else
        RET_ONERROR( mywritestr(f, "NULL") )
    RET_ONERROR( mywritestr(f, ") {\n") )

    if (languageFlag & JAVA_SOURCE) {
        RET_ONERROR( indentf(f, indent + 8) )
        RET_ONERROR( mywritestr(f, "try {\n") )
        RET_ONERROR( mywritestr(f, "    ") )
    }  

    RET_ONERROR( indentf(f, indent + 4) )
    if (languageFlag & JAVA_SOURCE)
        RET_ONERROR( mywritestr(f, "   ") )
    RET_ONERROR( mywritestr(f, "nextEvent = ") )
    RET_ONERROR( writeCProcessEventCallback(f, languageFlag) )
    if (languageFlag & JAVA_SOURCE)
        RET_ONERROR( mywritestr(f, ".processEvent") )
    RET_ONERROR( mywritestr(f, "(") )
    if (languageFlag & (C_SOURCE | CC_SOURCE))
        RET_ONERROR( mywritestr(f, "&self->") )
    if (languageFlag & JAVA_SOURCE)
        RET_ONERROR( writeCVariable(f, languageFlag) )
    else
        RET_ONERROR( mywritestr(f, getVariableName()) )
    RET_ONERROR( mywritestr(f, ", \"") )
    RET_ONERROR( mywritestr(f, eventName) )    
    RET_ONERROR( mywritestr(f, "\"") )
    if (languageFlag & (C_SOURCE | CC_SOURCE))
        RET_ONERROR( mywritestr(f, ", data") )
    RET_ONERROR( mywritestr(f, ");\n") )

    if (languageFlag & JAVA_SOURCE) {
        RET_ONERROR( indentf(f, indent + 8) )
        RET_ONERROR( mywritestr(f, "} catch (Exception e) {\n") )
        RET_ONERROR( indentf(f, indent + 12) )
        RET_ONERROR( mywritestr(f, TheApp->getCPrefix()) )
        RET_ONERROR( mywritestr(f, "ShowError(e);\n") )
        RET_ONERROR( indentf(f, indent + 8) )
        RET_ONERROR( mywritestr(f, "}\n") )
    }

    RET_ONERROR( indentf(f, indent + 4) )
    if (languageFlag & JAVA_SOURCE)
        RET_ONERROR( mywritestr(f, "    ") )
    RET_ONERROR( mywritestr(f, "if (nextEvent) {\n") )

    return 0;
}

int 
NodeData::writeCSendEvent(int f, int indent, int languageFlag, 
                          EventIn *target, EventOut *source, Node *sNode)
{ 
    const char *sourceVariableName = source->getName(true);
    if (source->getExposedField() != NULL)
        sourceVariableName = source->getExposedField()->getName(true);
    
    const char *targetVariableName = target->getName(true);
    if (target->getExposedField() != NULL)
        targetVariableName = target->getExposedField()->getName(true);

    return writeCDowithEvent(f, indent + 4,  languageFlag, targetVariableName,
                             sourceVariableName, sNode, 
                             isArrayInC(target->getType()), target->getType());
}

int 
NodeData::writeCCopyEvent(int f, int indent, int languageFlag, 
                          EventOut *target, EventOut *source, Node *sNode)
{ 
    const char *sourceVariableName = source->getName(true);
    if (source->getExposedField() != NULL)
        sourceVariableName = source->getExposedField()->getName(true);
    
    const char *targetVariableName = target->getName(true);
    if (target->getExposedField() != NULL)
        targetVariableName = target->getExposedField()->getName(true);

    return writeCDowithEvent(f, indent,  languageFlag, targetVariableName,
                             sourceVariableName, sNode,
                             isArrayInC(target->getType()), target->getType());
}

int 
NodeData::writeCCopyEvent(int f, int indent, int languageFlag, 
                          EventIn *target, EventIn *source, Node *sNode)
{ 
    const char *sourceVariableName = source->getName(true);
    if (source->getExposedField() != NULL)
        sourceVariableName = source->getExposedField()->getName(true);
    
    const char *targetVariableName = target->getName(true);
    if (target->getExposedField() != NULL)
        targetVariableName = target->getExposedField()->getName(true);

    return writeCDowithEvent(f, indent,  languageFlag, targetVariableName,
                             sourceVariableName, sNode,
                             isArrayInC(target->getType()), target->getType());
}

int
NodeData::writeCDowithEvent(int f, int indent, int languageFlag, 
                            const char *targetVariableName, 
                            const char *sourceVariableName,
                            Node *sNode, bool isArray, int type)
{ 
    if ((languageFlag & JAVA_SOURCE) && isArray) {
        RET_ONERROR( indentf(f, indent + 12) )
        RET_ONERROR( mywritestr(f, "if ((") )
        RET_ONERROR( writeCVariable(f, languageFlag) )
        RET_ONERROR( mywritestr(f, ".") )
        RET_ONERROR( mywritestr(f, sourceVariableName) )
        RET_ONERROR( mywritestr(f, " != null) && (") )
        RET_ONERROR( sNode->writeCVariable(f, languageFlag) )
        RET_ONERROR( mywritestr(f, " != null)) {\n") )

        RET_ONERROR( indentf(f, indent + 20) )
        RET_ONERROR( mywritestr(f, "if ((") )
        RET_ONERROR( sNode->writeCVariable(f, languageFlag) )
        RET_ONERROR( mywritestr(f, ".") )
        RET_ONERROR( mywritestr(f, targetVariableName) )
        RET_ONERROR( mywritestr(f, " == null) || (") )
        RET_ONERROR( sNode->writeCVariable(f, languageFlag) )
        RET_ONERROR( mywritestr(f, ".") )
        RET_ONERROR( mywritestr(f, targetVariableName) )
        RET_ONERROR( mywritestr(f, ".length != ") )
        RET_ONERROR( writeCVariable(f, languageFlag) )
        RET_ONERROR( mywritestr(f, ".") )
        RET_ONERROR( mywritestr(f, sourceVariableName) )
        RET_ONERROR( mywritestr(f, ".length))\n") )

        RET_ONERROR( indentf(f, indent + 24) )
        RET_ONERROR( sNode->writeCVariable(f, languageFlag) )
        RET_ONERROR( mywritestr(f, ".") )
        RET_ONERROR( mywritestr(f, targetVariableName) )
        RET_ONERROR( mywritestr(f, " = new ") )
        RET_ONERROR( mywritestr(f, getTypeC(type, languageFlag)) )
        RET_ONERROR( mywritestr(f, "[") )
        RET_ONERROR( writeCVariable(f, languageFlag) )
        RET_ONERROR( mywritestr(f, ".") )
        RET_ONERROR( mywritestr(f, sourceVariableName) )
        RET_ONERROR( mywritestr(f, ".length") )
        RET_ONERROR( mywritestr(f, "];\n") )
    }

    RET_ONERROR( indentf(f, indent + 8) )
    if (languageFlag & JAVA_SOURCE)
        RET_ONERROR( mywritestr(f, "    ") )
    if ((languageFlag & JAVA_SOURCE) && isArray) {
        RET_ONERROR( mywritestr(f, "    ") )
    }
    if ((languageFlag & JAVA_SOURCE) && (!isArray)) {
        RET_ONERROR( sNode->writeCVariable(f, languageFlag) )
        RET_ONERROR( mywritestr(f, ".") )
        RET_ONERROR( mywritestr(f, targetVariableName) )
        RET_ONERROR( mywritestr(f, " = ") )
    }
    RET_ONERROR( mywritestr(f, TheApp->getCPrefix()) )
    RET_ONERROR( mywritestr(f, typeEnumToString(type)) )
    RET_ONERROR( mywritestr(f, "SendEvent(") )
    if ((languageFlag & (C_SOURCE | CC_SOURCE)) ||
        ((languageFlag & JAVA_SOURCE) && isArray)) {
        if (languageFlag & (C_SOURCE | CC_SOURCE)) {
            if (!isArrayInC(type))
                RET_ONERROR( mywritestr(f, "&(") )
            RET_ONERROR( mywritestr(f, "self->") )
        }
        if (languageFlag & JAVA_SOURCE)
            RET_ONERROR( sNode->writeCVariable(f, languageFlag) )
        else
            RET_ONERROR( mywritestr(f, sNode->getVariableName()) )
        RET_ONERROR( mywritestr(f, ".") )
        RET_ONERROR( mywritestr(f, targetVariableName) )
        if (languageFlag & (C_SOURCE | CC_SOURCE))
            if (!isArray)
                RET_ONERROR( mywritestr(f, ")") )
        RET_ONERROR( mywritestr(f, ", ") )
    }
    if (languageFlag & (C_SOURCE | CC_SOURCE))
        RET_ONERROR( mywritestr(f, "self->") )
    if (languageFlag & JAVA_SOURCE)
        RET_ONERROR( writeCVariable(f, languageFlag) )
    else
        RET_ONERROR( mywritestr(f, getVariableName()) )
    RET_ONERROR( mywritestr(f, ".") )
    RET_ONERROR( mywritestr(f, sourceVariableName) )
    if (isMFType(type)) {
        RET_ONERROR( mywritestr(f, ", ") )
        if (languageFlag & (C_SOURCE | CC_SOURCE))
            RET_ONERROR( mywritestr(f, "self->") )
        if (languageFlag & JAVA_SOURCE)
            RET_ONERROR( writeCVariable(f, languageFlag) )
        else
            RET_ONERROR( mywritestr(f, getVariableName()) )
        RET_ONERROR( mywritestr(f, ".") )
        RET_ONERROR( mywritestr(f, sourceVariableName) )
        if (languageFlag & JAVA_SOURCE)
            RET_ONERROR( mywritestr(f, ".length") )
        else
            RET_ONERROR( mywritestr(f, "_length") )
    }
    RET_ONERROR( mywritestr(f, ");\n") )

    if ((languageFlag & JAVA_SOURCE) && isArray) {
        RET_ONERROR( indentf(f, indent + 12) )
        RET_ONERROR( mywritestr(f, "}\n") )
    }
    return 0;
}

int 
Node::writeCAndFollowRoutes(int f, int indent, int languageFlag, 
                            bool writeSensorNodes, const char *eventName)
{
    Proto *proto = getProto();
    if (!getProto()->isLoaded())
        RET_ONERROR( writeCProcessEvent(f, indent, languageFlag, eventName) )
    else
        indent = indent - 4;

    if (isInsideProto()) {
        Node *parentNode = getProtoParent();
        Proto *parentProto = parentNode->getProto();

        for (int i = 0; i < m_proto->getNumEventOuts(); i++) {
            int event = getOutsideProto()->lookupIsEventOut(this, i);
            if (event < 0)
                continue;
            EventOut *source = m_proto->getEventOut(i);
            int isEventOut = event; 
            if (isEventOut != -1) {
                EventOut *protoTarget = parentProto->getEventOut(isEventOut);
                if (this->writeCCopyEvent(f, indent, languageFlag, 
                                          protoTarget, source, getProtoParent()
                                         ) != 0)
                   return -1;
            }
        }
    }
    if (getProto()->isLoaded()) {
        for (int i = 0; i < m_proto->getNumEventIns(); i++) {
            bool exposed = false;
            int event = getProto()->lookupIsEventIn(eventName);
            if (event < 0) {
                int exf = getProto()->lookupIsExposedField(eventName);
                if (exf > -1) {
                   ExposedField *exField = getProto()->getExposedField(exf);
                   exposed = true;
                   event = exField->getEventIn();
                }
            }
            if (event < 0)
                continue;
            EventIn *target = m_proto->getEventIn(i);
            MyString eventInName = "";
            if (exposed)
                eventInName += "set_";
            eventInName += eventName;
            if (strcmp(target->getName(true), eventInName) != 0)
                continue;
            int isEventIn = event; 
            if (isEventIn != -1) {
                if (exposed)
                    for (int j = 0; 
                         j < proto->getExposedField(isEventIn)->getNumIs(); 
                         j++) {
                        ExposedField *eF = m_proto->getExposedField(isEventIn);
                        Node *node = eF->getIsNode(j);
                        node = m_scene->searchProtoNodeId(node->getId());
                        EventIn *protoSource = node->getProto()->getEventIn(
                                                   eF->getIsField(j));
                        if (this->writeCCopyEvent(f, indent, languageFlag, 
                                                  protoSource, target, node
                                                 ) != 0)
                            return -1;
                    }
                else
                    for (int j = 0; 
                         j < proto->getEventIn(isEventIn)->getNumIs(); 
                         j++) {
                        EventIn *eI = m_proto->getEventIn(isEventIn);
                        Node *node = eI->getIsNode(j);
                        node = m_scene->searchProtoNodeId(node->getId());
                        EventIn *protoSource = node->getProto()->getEventIn(
                                                   eI->getIsField(j));
                        if (this->writeCCopyEvent(f, indent, languageFlag, 
                                                  protoSource, target, node
                                                 ) != 0)
                            return -1;
                    }
            }
        }
    }

    SocketList::Iterator *j;
    for (int i = 0; i < getProto()->getNumEventOuts(); i++) {
        for (j = m_outputs[i].first(); j != NULL; j = j->next()) {
            RouteSocket s = j->item();

            Node *sNode = s.getNode();

            bool isCurveAnimation = (sNode->getType() == DUNE_CURVE_ANIMATION);

            EventIn *target = sNode->getProto()->getEventIn(s.getField());
            EventOut *source = proto->getEventOut(i);

            RET_ONERROR( indentf(f, indent + 8) )
            if ((languageFlag & JAVA_SOURCE) && !isCurveAnimation) {
                RET_ONERROR( indentf(f, indent + 4) )
                RET_ONERROR( mywritestr(f, "if (") )
                RET_ONERROR( sNode->writeCVariable(f, languageFlag) )
                RET_ONERROR( mywritestr(f, " != null) ") )
            }
            RET_ONERROR( mywritestr(f, "{\n") )

            if (isInsideProto()) {
                sNode = m_scene->searchProtoNodeId(sNode->getId());
                if (sNode == NULL) {
                    swDebugf("internal Error: sNode == NULL\n");
                    return -1;
                }
            }

            if ((getNodeClass() & SENSOR_NODE) && 
                hasOutputsOrIs()) {
                m_scene->addToSensorNodes(this);
            }

            bool replaceEventOut = false;
            if (sNode->getType() == DUNE_CURVE_ANIMATION) {
                NodeCurveAnimation *curve = (NodeCurveAnimation *) sNode;
                if (s.getField() == curve->getFraction_Field()) {
                    replaceEventOut = true;
                    NodePositionInterpolator *inter =
                        (NodePositionInterpolator *)(curve->
                        getPositionInterpolator());
                    source = inter->getProto()->getEventOut(
                                 inter->set_fraction_Field());
                    if (getType() == VRML_TIME_SENSOR) {
                        NodeTimeSensor *timer = (NodeTimeSensor *)this;
                        int eventOut = timer->getProto()->lookupEventOut(
                                     "fraction_changed", true);
                        source = timer->getProto()->getEventOut(eventOut);
                    }     

                    if (writeCSendEvent(f, indent, languageFlag, target, 
                                        source, inter) != 0)
                        return -1;
                    if (writeCEndSendEvent(f, indent, languageFlag) !=0)
                        return -1;
 
                    NodeOrientationInterpolator *inter2 =
                        (NodeOrientationInterpolator *)(curve->
                        getOrientationInterpolator());
                    source = inter2->getProto()->getEventOut(
                                 inter2->set_fraction_Field());
                    if (getType() == VRML_TIME_SENSOR) {
                        NodeTimeSensor *timer = (NodeTimeSensor *)this;
                        int eventOut = timer->getProto()->lookupEventOut(
                                     "fraction_changed", true);
                        source = timer->getProto()->getEventOut(eventOut);
                    }     

                    if (writeCSendEvent(f, indent, languageFlag, target, 
                                        source, inter2) != 0)
                        return -1;
                }
            }
            bool replaceEventIn = false;
            if (getType() == DUNE_CURVE_ANIMATION) {
                NodeCurveAnimation *curve = (NodeCurveAnimation *) this;
                if (i == curve->getPosition_Field()) {
                    replaceEventIn = true;
                    NodePositionInterpolator *inter =
                        (NodePositionInterpolator *)(curve->
                        getPositionInterpolator());
                    source = inter->getProto()->getEventOut(
                                 inter->value_changed_Field());
                    if (inter->writeCSendEvent(f, indent, languageFlag, 
                                               target, source, sNode) != 0)
                        return -1;
                }
                if (i == curve->getOrientation_Field()) {
                    replaceEventIn = true;
                    NodeOrientationInterpolator *inter =
                        (NodeOrientationInterpolator *)(curve->
                        getOrientationInterpolator());
                    source = inter->getProto()->getEventOut(
                                 inter->value_changed_Field());
                    if (inter->writeCSendEvent(f, indent, languageFlag, 
                                               target, source, sNode) != 0)
                        return -1;
                }
            }
            if ((!replaceEventOut) && (!replaceEventIn))
                if (writeCSendEvent(f, indent, languageFlag, target, source,
                                    sNode) != 0)
                    return -1;

            const char *targetVariableName = target->getName(true);
            if (target->getExposedField() != NULL)
                targetVariableName = target->getExposedField()->getName(true);

            RET_ONERROR( sNode->writeCAndFollowRoutes(f, indent + 12,
                             languageFlag, writeSensorNodes, 
                             targetVariableName) )

            RET_ONERROR( indentf(f, indent + 8) )
            if (languageFlag & JAVA_SOURCE)
                RET_ONERROR( mywritestr(f, "    ") )
            RET_ONERROR( mywritestr(f, "}\n") )
        }
    }
    writeCEndSendEvent(f, indent, languageFlag);
    return 0;
}                                                                

int                 
Node::writeCEndSendEvent(int f, int indent, int languageFlag)
{
    if (getProto()->isLoaded())
        return 0;
    RET_ONERROR( indentf(f, indent + 4) )
    if (languageFlag & JAVA_SOURCE)
        RET_ONERROR( mywritestr(f, "    ") )
    RET_ONERROR( mywritestr(f, "}\n") )
    RET_ONERROR( indentf(f, indent) )
    if (languageFlag & JAVA_SOURCE)
        RET_ONERROR( mywritestr(f, "    ") )
    RET_ONERROR( mywritestr(f, "}\n") )
    return 0;
}

int                 
Node::writeCInstallDynamicNodeCallback(int f, int languageFlag, Proto *proto)
{
    if (languageFlag & C_SOURCE) {
        RET_ONERROR( mywritestr(f, "    ") )
        RET_ONERROR( mywritestr(f, TheApp->getCPrefix()) )
        RET_ONERROR( mywritestr(f, proto->getCName(true)) )
        RET_ONERROR( mywritestr(f, "ProcessEventCallback = ") )
        RET_ONERROR( mywritestr(f, TheApp->getCPrefix()) )
        RET_ONERROR( mywritestr(f, proto->getCName(true)) )
        RET_ONERROR( mywritestr(f, "SendEventsCallback;\n") )
    } else if (languageFlag & CC_SOURCE) {
        RET_ONERROR( mywritestr(f, "    ") )
        RET_ONERROR( mywritestr(f, TheApp->getCPrefix()) )
        RET_ONERROR( mywritestr(f, proto->getCName(true)) )
        RET_ONERROR( mywritestr(f, "::") )
        RET_ONERROR( mywritestr(f, "processEventCallback = ") )
        RET_ONERROR( mywritestr(f, TheApp->getCPrefix()) )
        RET_ONERROR( mywritestr(f, proto->getCName(true)) )
        RET_ONERROR( mywritestr(f, "SendEventsCallback;\n") )
    } else if (languageFlag & JAVA_SOURCE) {
        RET_ONERROR( mywritestr(f, "    ") )
        RET_ONERROR( mywritestr(f, "    {\n") )
        RET_ONERROR( mywritestr(f, "    ") )
        RET_ONERROR( mywritestr(f, "    ") )
        RET_ONERROR( mywritestr(f, TheApp->getCPrefix()) )
        RET_ONERROR( mywritestr(f, proto->getCName(true)) )
        RET_ONERROR( mywritestr(f, "SendEventsCallback var = new ") )
        RET_ONERROR( mywritestr(f, TheApp->getCPrefix()) )
        RET_ONERROR( mywritestr(f, proto->getCName(true)) )
        RET_ONERROR( mywritestr(f, "SendEventsCallback();\n") )
        RET_ONERROR( mywritestr(f, "    ") )
        RET_ONERROR( mywritestr(f, "    ") )
        RET_ONERROR( mywritestr(f, TheApp->getCPrefix()) )
        RET_ONERROR( mywritestr(f, proto->getCName(true)) )
        RET_ONERROR( mywritestr(f, ".set") )
        RET_ONERROR( mywritestr(f, TheApp->getCPrefix()) )
        RET_ONERROR( mywritestr(f, proto->getCName(true)) )
        RET_ONERROR( mywritestr(f, "ProcessEventCallback(var);\n") )
        RET_ONERROR( mywritestr(f, "        }\n") )
    }
    return 0;
}

int
NodeData::lookupEventIn(const char *name, bool x3d) const
{
    return m_proto ? m_proto->lookupEventIn(name, x3d) : INVALID_INDEX;
}

int
NodeData::lookupEventOut(const char *name, bool x3d) const
{
    return m_proto ? m_proto->lookupEventOut(name, x3d) : INVALID_INDEX;
}

MyString
NodeData::newEventName(int typeEnum, bool out)
{
    bool x3d = m_scene->isX3d();
    char name[1024];
    MyString eventName = "";
    const char *typestr = typeEnumToString(typeEnum);
    if (typestr[0] == 'M')
        eventName += "m";
    for (unsigned int i = 2; i < strlen(typestr); i++)
        eventName += tolower(typestr[i]);
    int counter = 0;
    bool foundflag;
    do {
        foundflag = false;
        counter++;
        const char *format = "%d_in";
        if (out)
            format = "%d_out";
        mysnprintf(name, 1023, (const char *)format, counter);

        int numberEvents = 0;
        if (out)
            numberEvents = m_proto->getNumEventOuts();
        else
            numberEvents = m_proto->getNumEventIns();
        for (int i = 0; i < numberEvents; i++) {
            const char *oldName;
            if (out)
               oldName = (const char*) m_proto->getEventOut(i)->getName(x3d);
            else 
               oldName = (const char*) m_proto->getEventIn(i)->getName(x3d);
            MyString cmpName = "";
            cmpName += eventName;
            cmpName += name; 
            if (strcmp(oldName, (const char*)cmpName) == 0)
                foundflag = true;
        }
    } while (foundflag == true);
    eventName += name;
    return eventName;
}

void
NodeData::addInput(int eventIn, Node *src, int eventOut)
{
    if (eventIn >= m_numEventIns)
        if (isDynamicFieldsNode()) {
            int typeEnum = src->getProto()->getEventOut(eventOut)->getType();
            MyString event = "";
            event += newEventName(typeEnum, false);
            if (TheApp->mainWnd() != NULL) {
                ScriptEventDialog dlg(TheApp->mainWnd(), IDD_SCRIPT_EVENT,
                                      event, (Node *)this);
                dlg.DoModal();
                if (dlg.GetValue()) {
                    event = "";
                    event += dlg.GetValue();
                }
            }
            ((DynamicFieldsNode *)this)->addEventIn(typeEnum, event); 
            update();
            NodeUpdate *hint= new NodeUpdate((Node *)this, NULL, 0);
            m_scene->UpdateViews(NULL, UPDATE_CHANGE_INTERFACE_NODE, 
                                 (Hint*) hint);
        }
    m_inputs[eventIn].append(RouteSocket(src, eventOut));
}

void
NodeData::addOutput(int eventOut, Node *dst, int eventIn)
{
    if (eventOut >= m_numEventOuts)
        if (isNodeWithAdditionalEvents()) {
            int typeEnum = dst->getProto()->getEventIn(eventIn)->getType();
            MyString event = "";
            event += newEventName(typeEnum, true);
            if (TheApp->mainWnd() != NULL) {
                ScriptEventDialog dlg(TheApp->mainWnd(), IDD_SCRIPT_EVENT,
                                      event, (Node *)this);
                dlg.DoModal();
                if (dlg.GetValue()) {
                    event = "";
                    event += dlg.GetValue();
                }
            }
            ((DynamicFieldsNode *)this)->addEventOut(typeEnum, event); 
            update();
            NodeUpdate *hint= new NodeUpdate((Node *)this, NULL, 0);
            m_scene->UpdateViews(NULL, UPDATE_CHANGE_INTERFACE_NODE, 
                                 (Hint*) hint);
        }
    m_outputs[eventOut].append(RouteSocket(dst, eventIn));
}

void
NodeData::removeInput(int eventIn, Node *src, int eventOut)
{
    SocketList::Iterator *j = m_inputs[eventIn].find(RouteSocket(src, eventOut));
    if (j) 
        m_inputs[eventIn].remove(j);
}

void
NodeData::removeOutput(int eventOut, Node *dst, int eventIn)
{
    SocketList::Iterator *j = m_outputs[eventOut].find(RouteSocket(dst, 
                                                                   eventIn));
    if (j)
        m_outputs[eventOut].remove(j);
}

void
NodeData::update()
{
    // used by Script to update its fields and
    // used by shapes to redraw after a change of a MF-field and
    // used by nodes to redraw parent after a change of a field

    // also update C export name of node
    m_scene->generateVariableName((Node *)this);
}

void
NodeData::reInit()
{
    // used to reinitialise private data of a "copy"-ed node
    // e.g. to reinitialise mesh data of some geometric shapes
}


void
Node::addParent(Node *parent, int field)
{
    for (int i = 0; i < m_parents.size(); i++)
        if ((m_parents[i].m_node == parent) && (m_parents[i].m_field == field)){
            m_geometricParentIndex = i;
            return; // parent already set
        }
    m_parents.append(Parent(parent, field, this));
    m_geometricParentIndex = m_parents.size() - 1;
}

void
Node::removeParent(void)
{
    if (m_geometricParentIndex != -1) {
        for (int i = m_geometricParentIndex + 1; i < m_parents.size(); i++)
            m_parents[i].m_self->m_geometricParentIndex--;
        m_parents.remove(m_geometricParentIndex);
        m_geometricParentIndex = -1;
    }
}


int
NodeData::findChild(Node *child, int field) const
{
    assert(field >= 0 && field < m_numFields);

    FieldValue *value = m_fields[field];

    if (value->getType() == SFNODE) {
        return (((SFNode *) value)->getValue() == child) ? 0 : -1;
    } else if (value->getType() == MFNODE) {
        NodeList *list = ((MFNode *) value)->getValues();
        for (int i = 0; i < list->size(); i++) {
            if (list->get(i) == child) return i;
        }
        return -1;
    } else {
        return -1;
    }
}

bool
Node::hasAncestor(Node *node) const
{
    if (this != m_scene->getRoot()) {
        if (hasParent()) {
            Node *parent = getParent();
            if (parent == node || parent->hasAncestor(node)) 
                return true;
        }  
    }
    return false;
}

bool 
NodeData::checkValid(Element *field)
{
    if (m_scene != NULL) {
        if (m_scene->isInvalidElement(field))
            return false;
    }
    return true;
}

int
NodeData::findValidFieldType(int childType)
{
    int match = -1;
    for (int field = 0; field < m_numFields; field++) {
        if (validChildType(field, childType)) {
            if (match != -1) {
                // ambiguous
                return - 1; 
            }
            match = field;
        }
    }
    return match;
}

int
NodeData::findFirstValidFieldType(int childType)
{
    // start loop with 1 to avoid metadataField
    for (int field = 1; field < m_numFields; field++) {
        if (validChildType(field, childType)) {
            return field;
        }
    }
    return -1;
}

int
NodeData::findValidField(Node *child)
{
    int ret = findValidFieldType(child->getNodeClass());
    if (ret == -1)
        ret = findValidFieldType(child->getType());
    return ret;
}

int
NodeData::findChildType(int childType)
{
    int match = -1;
    for (int field = 0; field < m_numFields; field++) {
        if (checkChildType(field, childType, false)) {
            if (match != -1) {
                // ambiguous
                return -1; 
            }
            match = field;
        }
    }
    return match;
}

bool
NodeData::validChild(int field, Node *child)
{
    return validChildType(field, child->getNodeClass());
}

bool
NodeData::validChildType(int field, int childType)
{
    return checkChildType(field, childType, true);
}

bool
NodeData::checkChildType(int field, int childType, bool checkNewNode)
{
    if (field < 0 || field >= m_numFields) return false;

    Field *def = m_proto->getField(field);

    // SFNode field already in use ?
    if (def->getType() == SFNODE) {
        SFNode *node = (SFNode *)m_fields[field];
        if (checkNewNode)
            if (node->getValue() != NULL)
                return false;
    }

    if ((def->getType() == SFNODE) || (def->getType() == MFNODE)) {
        int nodeType = def->getNodeType();

        if (childType & NOT_SELF_NODE) {
            int bits = ::getMaskedNodeClass(nodeType);
            if (bits == 0) {
               bits = nodeType & (~NOT_SELF_NODE);
               if (bits & childType)
                   return false;
            } else
               if (bits & childType)
                   return false;
        }
        if (nodeType == ANY_NODE) {
            return checkValid(def);
        } else if ((::getMaskedNodeClass(childType) != 0) &&
                   (::getMaskedNodeClass(childType) == 
                    ::getMaskedNodeClass(nodeType))) {
            return checkValid(def);
        } else if (::matchNodeClass(nodeType, childType)) {
            return checkValid(def);
        } else if ((getType() == VRML_CONTOUR_2D) && 
                   (childType == NURBS_CONTROL_CURVE_NODE)) {
            return checkValid(def);
        } else
           return false;
    } else {
        return false;
    }
    return false;
}

void
NodeData::setFlagRec(int flag)
{
    setFlag(flag);

    for (int i = 0; i < m_numFields; i++) {
        if (m_fields[i]->getType() == MFNODE) {
            MFNode *nodes = (MFNode *) m_fields[i];
            for (int j = 0; j < nodes->getSize(); j++)
                if (nodes->getValue(j) != NULL)
                    nodes->getValue(j)->setFlagRec(flag);
        } else if (m_fields[i]->getType() == SFNODE) {
            SFNode *value = (SFNode *) m_fields[i];
            if (value->getValue()) value->getValue()->setFlagRec(flag);
        }
    }
}

void
NodeData::clearFlagRec(int flag)
{
    clearFlag(flag);

    for (int i = 0; i < m_numFields; i++) {
        if (m_fields[i]->getType() == MFNODE) {
            MFNode *nodes = (MFNode *) m_fields[i];
            for (int j = 0; j < nodes->getSize(); j++)
                if (nodes->getValue(j) != NULL)
                    nodes->getValue(j)->clearFlagRec(flag);
        } else if (m_fields[i]->getType() == SFNODE) {
            SFNode *value = (SFNode *) m_fields[i];
            if (value->getValue()) value->getValue()->clearFlagRec(flag);
        }
    }
}

void
NodeData::sendEvent(int eventOut, double timestamp, FieldValue *value)
{
    assert(eventOut >= 0 && eventOut <= m_numEventOuts);

    // handle IS
    EventOut *evOut = m_proto->getEventOut(eventOut);
    if ((m_isEventOuts.size() > 0) && (m_isEventOuts[eventOut] != NULL))
        evOut = m_isEventOuts[eventOut];
    if (evOut && (evOut->getFlags() & FF_IS))
        for (int i = 0; i < evOut->getNumIs(); i++)
            if (evOut->getFlags() & EOF_IS_HIDDEN) {
                Node *isNode = evOut->getIsNode(i);
                isNode->sendEvent(evOut->getIsField(i), timestamp, value);
            }
    SocketList::Iterator *i;
    for (i = m_outputs[eventOut].first(); i != NULL; i = i->next()) {
        RouteSocket s = i->item();
        s.getNode()->receiveEvent(s.getField(), timestamp, value);
    }
}

void
NodeData::receiveEvent(int eventIn, double timestamp, FieldValue *value)
{
    if (TheApp->getDemoMode()) {
        if ((getType() == VRML_VIEWPOINT) && !TheApp->timeOut())
            return;
    }

    // check to see if this eventIn is part of an exposedField or
    // conntected to a normal field

    int field = -1;
    ExposedField *e = m_proto->getEventIn(eventIn)->getExposedField();

    if (e) {
        field = e->getFieldIndex();
        eventIn = e->getEventIn();
    } else
        field = m_proto->getEventIn(eventIn)->getField();

    // handle IS
    EventIn *evIn = m_proto->getEventIn(eventIn);
    if ((m_isEventIns.size() > 0) && (m_isEventIns[eventIn] != NULL))
        evIn = m_isEventIns[eventIn];
    if (evIn && (evIn->getFlags() & FF_IS)) {
        if (getType() != VRML_SCRIPT) {
            for (int i = 0; i < evIn->getNumIs(); i++) {
                Node *isNode = evIn->getIsNode(i);
                isNode->receiveEvent(evIn->getIsField(i), timestamp, value);

            }
        }
    } else {
        if (field != -1) {
            // set the appropriate field
            setField(field, value);
            m_scene->OnFieldChange((Node*)this, field);
            // fire off an event here?
        }
    }        
}

void                
NodeData::addIsElement(Node *node, int field, int elementType,
                       Proto *origProto, int origField, int flags)
{
    switch(elementType) {
      case EL_EVENT_IN:
        if (origField < origProto->getNumEventIns()) {
            if (m_isEventIns.size() == 0)
                for (int i = 0; i < origProto->getNumEventIns(); i++)
                    m_isEventIns[i] = NULL;
            m_isEventIns[origField] = new EventIn(origProto->
                                                  getEventIn(origField));
            m_isEventIns[origField]->addIs(node, field, elementType, 
                                           origProto, origField, flags);
        }
        break;
      case EL_EVENT_OUT:
        if (origField < origProto->getNumEventOuts()) {
            if (m_isEventOuts.size() == 0)
                for (int i = 0; i < origProto->getNumEventOuts(); i++)
                    m_isEventOuts[i] = NULL;
            m_isEventOuts[origField] = new EventOut(origProto->
                                                    getEventOut(origField));
            m_isEventOuts[origField]->addIs(node, field, elementType, 
                                            origProto, origField, flags);
        }
        break;
      case EL_EXPOSED_FIELD:
        if (origField < origProto->getNumExposedFields()) {
            for (int i = 0; i < origProto->getNumExposedFields(); i++)
                m_isExposedFields[i] = NULL;
            m_isExposedFields[origField] = new ExposedField(origProto->
                                                   getExposedField(origField));
            m_isExposedFields[origField]->addIs(node, field, elementType, 
                                                origProto, origField, flags);
        }
        break;
      case EL_FIELD:
        if (origField < origProto->getNumEventIns()) {
            for (int i = 0; i < origProto->getNumFields(); i++)
                m_isFields[i] = NULL;
            m_isFields[origField] = new Field(origProto->getField(origField));
            m_isFields[origField]->addIs(node, field, elementType, 
                                        origProto, origField, flags);
        }
        break;
    }
}

// compare content
bool
NodeData::isEqual(Node* node)
{
    if (node == NULL) {
        if (this == NULL)
            return true;
        else
            return false;
    }
    if (m_identifier == node->m_identifier)
        return true;
    return false;
}

// compare content
bool
NodeData::isEqualCopy(Node* node)
{
    if (node == NULL) {
        if (this == NULL)
            return true;
        else
            return false;
    }
    if (m_identifierCopy >= 0)
        if (m_identifierCopy == node->m_identifierCopy)
            return true;
    if (m_identifierCopy == node->m_identifier)
        return true;
    if (m_identifier == node->m_identifierCopy)
        return true;
    return false;
}

bool
NodeData::hasRoute(SocketList socketlist)
{
   if (socketlist.first() == NULL) 
      return false;
   else
      return true;
}

Path *
Node::getPath() const
{
    int len = 0;
    const Node *node = this;
    const Node *protoRoot = node;

    for (protoRoot = this; protoRoot->hasParentOrProtoParent(); 
         protoRoot = protoRoot->getParentOrProtoParent())
        len += 2;

    MyArray<int> protoList;
    m_scene->getProtoList(&protoList, protoRoot);
    if (protoList.size() > 0)
        len += 4;

    if (len == 0) {
        Path *path = new Path(NULL, 0, m_scene);
        return path;
    }

    int *list = new int[len];

    int i = len - 1;

    for (node = this; node->hasParentOrProtoParent(); 
         node = node->getParentOrProtoParent()) {
        Node *parent = node->getParentOrProtoParent();
        int field = node->getParentFieldOrProtoParentField();
        FieldValue *value = parent->getField(field);
        if (value->getType() == MFNODE) {
            list[i--] = ((MFNode *) value)->getValues()->find((Node *) node);
        } else if (value->getType() == SFNODE)  {
            list[i--] = 0;
        } else {
            assert(0);
        }
        list[i--] = field;
    }

    if (protoList.size() > 0) {
        list[i--] = 0;
        list[i--] = 0;
        list[i--] = protoList[1];
        list[i--] = protoList[0];
    }

    Path *path = new Path(list, len, m_scene);
    delete [] list;
    list = NULL;
    return path;
}

bool
Node::isInScene(Scene* scene) const
{
    if (scene != m_scene) 
        return false;
    if (scene == NULL)
        return false;
    if (this == m_scene->getRoot()) 
        return true;
    if (((NodeData *)this)->isInsideProto()) {
        Node *node = ((NodeData *)this)->getNodePROTO();
        if (node)
            return node->isInScene(scene);
        else
            return false;
    }
    if (!this->hasParent())
        return false;
    if (this->getParent()->isInScene(scene))
        return true;
    return false;
}

bool 
Node::supportAnimation(void)
{
    Proto *proto = getProto();
    for (int i = 0; i < proto->getNumEventIns(); i++) {
        int type = proto->getEventIn(i)->getType();
        if (m_scene->isInvalidElement(proto->getEventIn(i)))
            continue;

        if (typeDefaultValue(type)->supportAnimation(m_scene->isX3d()))
            return true;
    }
    return false;
}


bool 
Node::supportInteraction(void)
{
    Proto *proto = getProto();
    for (int i = 0; i < proto->getNumEventIns(); i++) {
        int type = proto->getEventIn(i)->getType();
        if (typeDefaultValue(type)->supportInteraction())
            return true;
    }
    return false;
}

bool
Node::isInvalidChild(void)
{
    if (this == m_scene->getRoot())
        return true;
    if (isInvalidChildNode() &&
        (getParent()->getProto()->getField(getParentField())->getType() 
         == MFNODE)
       )
        return true;
    return false;
}

Vec3f               
Node::getMinBoundingBox(void)
{
   Vec3f ret(0, 0, 0);
   return ret;
}

Vec3f               
Node::getMaxBoundingBox(void)
{
   Vec3f ret(0, 0, 0);
   return ret;
}

void 
Node::appendComment(Node *node)
{ 
    m_commentsList->append(node); 
}

void Node::appendTo(NodeList* nodelist)
{
    for (int i = 0;i < m_commentsList->size(); i++)
       nodelist->append((*m_commentsList)[i]);
    nodelist->append(this);
    m_commentsList->resize(0);
}

void                
Node::doWithParents(DoWithNodeCallback callback, void *data,
                    bool searchInRest, bool callSelf)
{
    for (int i = 0; i < m_parents.size(); i++) {
        bool call = true;
        if (i == m_geometricParentIndex)
            if (!callSelf)
                call = false;
        if (call) 
            if (!callback(m_parents[i].m_node, data))
                if (!searchInRest)
                    break; 
    }
}

void
Node::doWithSiblings(DoWithNodeCallback callback, void *data,
                     bool searchInRest, bool callSelf)
{
    if (hasParent()) {
        Node *parent = getParent();
        int parentField = getParentField();
        FieldValue *parentValue = parent->getField(parentField);
        if (parentValue->getType() == MFNODE) {
            MFNode *value = (MFNode *)parentValue;
            for (int i = 0; i < value->getSize(); i++) {
                Node *sibling = value->getValue(i);
                if (sibling != NULL) {
                    bool call = true;
                    if (sibling == this)
                        if (!callSelf)
                            call = false;
                    if (call)    
                        if (!callback(sibling, data))
                            if (!searchInRest)
                                break; 
                }
            }      
        }
    }
}

bool Node::doWithBranch(DoWithNodeCallback callback, void *data, 
                        bool searchInRest, bool skipBranch, 
                        bool skipProto, bool callSelf, bool skipInline,
                        bool searchInConvertedNodes)
{
    if (this == NULL)
        return false;
    bool searchOn = true;
    bool handleInline = !skipInline;
    if (searchOn && (!skipProto) && hasProtoNodes()) {
        NodePROTO *nodeProto = (NodePROTO *)this;
        for (int i = 0; i < nodeProto->getNumProtoNodes(); i++)
            if (!(nodeProto->getProtoNode(i)->doWithBranch(callback, data,
                  false, true, false, true, skipInline, 
                  searchInConvertedNodes)))
                return false;
    }
    // search in current node itself
    if (callSelf)
        searchOn = callback(this, data);
    if (searchOn)
        for (int i = 0; i < getNumConvertedNodes(); i++) {
            searchOn = callback(getConvertedNode(i), data);
            if (!searchOn)
                break;
        }
    if (searchOn) {
        // search in children of of current node
        for (int i = 0; i < m_proto->getNumFields(); i++) {
            if (m_scene->isInvalidElement(m_proto->getField(i)))
                continue;

            if (m_proto->getField(i)->getType() == SFNODE) {
                Node *child = ((SFNode *) getField(i))->getValue();
                if (child) {
                    if (child == this) { 
                        swDebugf("%s %s: %s",
                                 "Warning: simple cyclic scenegraph: USE",
                                 (const char *)getName(),
                                 "invalid VRML/X3D file...\n");
                        return false; 
                    }
                    if (child == getParent()) {
                        swDebugf("%s %s: %s",
                                 "Warning: simple cyclic scenegraph: USE",
                                  (const char *)getName(),
                                  "invalid VRML/X3D file...\n");
                        return false; 
                    }
                    if (!child->doWithBranch(callback, data, false, 
                                             skipBranch, skipProto, callSelf, 
                                             skipInline, searchInConvertedNodes)) {
                        if (skipBranch)
                            continue;
                        else
                            return false;
                    }
                } 
            } else {
                NodeList *childList = NULL;
                if (handleInline && ((getType() == VRML_INLINE) ||
                                     (getType() == VRML_INLINE_LOAD_CONTROL))) {
                     NodeInline *node = (NodeInline *)this;
                     childList = node->getLoadedNodes();
                     handleInline = false;
                }    
                if (m_proto->getField(i)->getType() == MFNODE)
                    childList = ((MFNode *) getField(i))->getValues();
                if (childList) {
                    for (int j = 0; j < childList->size(); j++) {
                        Node *child = childList->get(j);
                        if (child == this) {
                            swDebugf("%s %s: %s",
                                     "Warning: simple cyclic scenegraph: USE",
                                     (const char *)getName(),
                                     "invalid VRML/X3D file...\n");
                            return false; 
                        }
                        if (child == getParent()) {
                            swDebugf("%s %s: %s",
                                     "Warning: simple cyclic scenegraph: USE",
                                     (const char *)getName(),
                                     "invalid VRML/X3D file...\n");
                            return false; 
                        }
                        if (child) { 
                            if (!child->doWithBranch(callback, data, 
                                                     false, skipBranch, 
                                                     skipProto, callSelf, 
                                                     skipInline,
                                                     searchInConvertedNodes)) {
                                if (skipBranch)
                                    continue;
                                else
                                    return false;
                            }
                        }
                    }
                }
            }
        if (searchInRest) {
            // search in next children of current parent
            if (this != m_scene->getRoot())
                if (hasParent() && isInScene(m_scene)) {
                    Node *parent = getParent();
                    FieldValue *selfField = parent->getField(getParentField()); 
                    if ((selfField != NULL) && 
                        (selfField->getType() == MFNODE)) {
                        int index = parent->findChild(this, getParentField());
                        MFNode *mfNode = (MFNode *)selfField;
                        NodeList *childList = mfNode->getValues();
                        if (childList)
                            for (int j = index + 1; j < childList->size(); j++){
                                Node *child = childList->get(j);
                                if (child) { 
                                    if (child == getParent()) {
                                        swDebugf("%s %s: %s",
                                                 "Warning: simple cyclic scenegraph: USE",
                                                 (const char *)getName(),
                                                 "invalid VRML/X3D file...\n");
                                                  return false; 
                                    }
                                    if (!child->doWithBranch(callback, data, 
                                           false, skipBranch, skipProto, 
                                           callSelf, skipInline,
                                           searchInConvertedNodes)) {
                                        if (skipBranch)
                                            continue;
                                        else
                                            return false;       
                                    }
                                }
                            }
                    }
                }
            }
        }
    }
    return searchOn;
}

bool
Node::doWithSimilarBranch(DoWithSimilarBranchCallback callback, 
                          Node *similarNode, void *data)
{
    if (!callback(this, similarNode, data))
        return false;
    if (m_numFields != similarNode->getProto()->getNumFields())
        return false;
    for (int i = 0; i < m_numFields; i++) {
        if (m_fields[i]->getType() == SFNODE) {
            if (similarNode->getField(i)->getType() != SFNODE)
                return false;
            Node *child = ((SFNode *) m_fields[i])->getValue();
            Node *node = ((SFNode *) similarNode->getField(i))->getValue(); 
            if (child && node) { 
                if (!child->doWithSimilarBranch(callback, node, data))
                    return false;
            } else if (child || node)
                return false;
        } else if (m_fields[i]->getType() == MFNODE) {
            if (similarNode->getField(i)->getType() != MFNODE)
                return false;
            NodeList *childList = ((MFNode *) m_fields[i])->getValues();
            NodeList *nodeList = ((MFNode *) 
                                  similarNode->getField(i))->getValues(); 
            if (childList && nodeList) {
                if (childList->size() != nodeList->size())
                    return false;
                for (int j = 0; j < childList->size(); j++) {
                    Node *child = childList->get(j);
                    Node *node = nodeList->get(j);
                    if (child && node) {
                        if (!child->doWithSimilarBranch(callback, node, data)) 
                            return false;
                    } else if (child || node)
                        return false;
                }
            } else if ((childList != NULL) || (nodeList != NULL))
                return false;
        }
    }
    return true;   
}

void
Node::copyChildrenTo(Node *copyedNode, bool copyNonNodes)
{
    for (int i = 0; i < m_numFields; i++) {
        if (m_fields[i]->getType() == SFNODE) {
            Node *child = ((SFNode *) m_fields[i])->getValue();
            if (child) {
                Node *copyChild = child->copy();
                copyChild->ref();
                copyedNode->unSetVariableName();
                copyedNode->setField(i, new SFNode(copyChild));
                child->copyChildrenTo(copyChild, copyNonNodes);
                child->copyOutputsTo(copyChild);
            }
        } else if (m_fields[i]->getType() == MFNODE) {
            NodeList *childList = ((MFNode *) m_fields[i])->getValues();
            if (childList) {
                MFNode *field = new MFNode((MFNode *)m_fields[i]);
                if (field) {
                    copyedNode->setField(i, field);
                    MFNode *mfnode = (MFNode *) copyedNode->getField(i);
                    for (int j = 0; j < childList->size(); j++) {
                        Node *copyChild = mfnode->getValue(j);
                        if (copyChild && childList->get(j)) {
                            childList->get(j)->ref();
                            childList->get(j)->unSetVariableName();
                            childList->get(j)->copyChildrenTo(copyChild,
                                                              copyNonNodes);
                            childList->get(j)->copyOutputsTo(copyChild);
                        }
                    }
                }
            } else if (copyNonNodes) {
                FieldValue *value = m_fields[i];
                if (value)
                    setField(i, value->copy());
            }
        }
    }   
}

void
Node::copyOutputsTo(Node *copyedNode)
{
    for (int i = 0; i < m_proto->getNumEventOuts(); i++) {
        for (SocketList::Iterator *j = getOutput(i).first(); 
             j != NULL; j = j->next()) 
            copyedNode->addOutput(i, j->item().getNode(), 
                                     j->item().getField());
    }   
}

void
Node::doWithAllElements(DoWithAllElementsCallback callback, void *data)
{
    int i = 0;
    for (i = 0; i < m_proto->getNumFields(); i++)
        callback(m_proto->getField(i), data);
    for (i = 0; i < m_proto->getNumEventIns(); i++)
        callback(m_proto->getEventIn(i), data);
    for (i = 0; i < m_proto->getNumEventOuts(); i++)
        callback(m_proto->getEventOut(i), data);
    for (i = 0; i < m_proto->getNumExposedFields(); i++)
        callback(m_proto->getExposedField(i), data);
}

void
NodeData::removeChildren(void) 
{
    for (int i = 0; i < m_numFields; i++)
        if (m_fields[i]->getType() == SFNODE)
            setField(i, new SFNode(NULL));
        else if (m_fields[i]->getType() == MFNODE)
            setField(i, new MFNode());
}

void
NodeData::handleIs(void)
{
    // PROTO initialisation ready ?
    if (!getProto()->isLoaded())
        return;

    bool x3d = m_scene->isX3d();

    // handle IS
    for (int i = 0; i < m_proto->getNumExposedFields(); i++) {
        m_isExposedFields[i] = NULL;
        ExposedField *field = m_proto->getExposedField(i);
        if (field->getFlags() & FF_IS)
            for (int j = 0; j < field->getNumIs(); j++)
                if (field->getFlags() & FF_IS) {
                    Node *isNode = field->getIsNode(j);
                    Proto *proto = isNode->getProto();
                    FieldValue *value = field->getValue()->copy();
                    if ((isNode != NULL))
                        isNode->setField(field->getIsField(j), value);
                    m_isExposedFields[i] = new ExposedField(field);
                    isNode->addIsElement((Node *)this, i, 
                                         EL_EXPOSED_FIELD, proto, 
                                         field->getIsField(j));
            }
    }
    for (int i = 0; i < m_proto->getNumFields(); i++) {
        m_isFields[i] = NULL;
        Field *field = m_proto->getField(i);
        if (field->getFlags() & FF_IS)
            for (int j = 0; j < field->getNumIs(); j++)
                if (field->getFlags() & FF_IS) {
                    Node *isNode = field->getIsNode(j);
                    Proto *proto = isNode->getProto();
                    FieldValue *value = field->getDefault(x3d)->copy();
                    if ((isNode != NULL)) {
                        isNode->setField(field->getIsField(j), value);
                    }
                    m_isFields[i] = new Field(field);
                    isNode->addIsElement((Node *)this, i, EL_FIELD, proto,
                                         field->getIsField(j));
                }                                 
    }

    for (int i = 0; i < m_proto->getNumEventOuts(); i++) {
        m_isEventOuts[i] = NULL;
        EventOut *eventOut = m_proto->getEventOut(i);
        if (eventOut->getFlags() & FF_IS)
            for (int j = 0; j < eventOut->getNumIs(); j++)
                if (eventOut->getFlags() & EOF_IS) {
                    Node *isNode = eventOut->getIsNode(j); 
                    Proto *proto = isNode->getProto();
                    int evOut = eventOut->getIsField(j);
                    m_isEventOuts[i] = new EventOut(eventOut);
                    isNode->addIsElement((Node *)this, i, EL_EVENT_OUT, proto, 
                                          evOut, EOF_IS_HIDDEN);
                }
    }
    for (int i = 0; i < m_proto->getNumEventIns(); i++) {
        m_isEventIns[i] = NULL;
        EventIn *eventIn = m_proto->getEventIn(i);
        if (eventIn->getFlags() & FF_IS)
            for (int j = 0; j < eventIn->getNumIs(); j++)
                if (eventIn->getFlags() & EIF_IS) {
                    Node *isNode = eventIn->getIsNode(j); 
                    Proto *proto = isNode->getProto();
                    int evIn = eventIn->getIsField(j);
                    m_isEventIns[i] = new EventIn(eventIn);
                    isNode->addIsElement((Node *)this, i, EL_EVENT_IN, 
                                         proto, evIn, EIF_IS_HIDDEN);
                }
    }
}

Node *
NodeData::getIsNode(int nodeIndex)
{
    NodePROTO *nodeProto = (NodePROTO *)this;
    if (nodeIndex < 0)
        return NULL;
    Node *nodeInPROTO = nodeProto->getIsNode(nodeIndex);
    if (nodeInPROTO)
        return nodeInPROTO;
    return NULL;
}

bool                
NodeData::isDefault(int field) const
{
    bool x3d = m_scene->isX3d();
    return m_fields[field]->equals(
                getProto()->getField(field)->getDefault(x3d));
}

bool                
NodeData::isDefaultAngle(int field) const
{
    bool x3d = m_scene->isX3d();
    double angleUnit = m_scene->getUnitAngle();
    if (angleUnit == 0)
        angleUnit = 1;
    return m_fields[field]->equalsAngle(
               getProto()->getField(field)->getDefault(x3d), angleUnit);
}

bool 
NodeData::hasInputsOrIs(void)
{
    if (hasInputs())
        return true;
    for (int i = 0; i < m_proto->getNumFields(); i++) {
        Field *field = getProto()->getField(i);
        if (field->getFlags() & FF_HIDDEN)
            continue;
        if (isInsideProto() && getOutsideProto()->lookupIsField((Node *)this, i)
            != -1)
            return true;
    }
    return false;
}

bool 
NodeData::hasOutputsOrIs(void)
{
    if (hasOutputs())
        return true;
    for (int i = 0; i < m_proto->getNumEventOuts(); i++)
        if (isInsideProto() && getOutsideProto()->lookupIsEventOut(
                                   (Node *)this, i)
            != -1)
            return true;
    for (int i = 0; i < m_proto->getNumExposedFields(); i++)
        if (isInsideProto() && getOutsideProto()->lookupIsExposedField(
                                   (Node *)this, i)
            != -1)
            return true;
    return false;
}

bool 
NodeData::hasInputs(void)
{
    for (int j = 0; j < m_proto->getNumEventIns(); j++)
        if (getInput(j).size() > 0)
            return true;
    return false;
}

bool 
NodeData::hasOutputs(void)
{
    for (int j = 0; j < m_proto->getNumEventOuts(); j++)
        if (getOutput(j).size() > 0)
            return true;
    return false;
}

bool 
NodeData::hasInput(const char* routename) const
{
    bool x3d = m_scene->isX3d();
    for (int i = 0; i < m_proto->getNumEventIns(); i++)
        if (strcmp(m_proto->getEventIn(i)->getName(x3d), routename) == 0) {
            if (getInput(i).size() > 0)
                return true;
            else
                return false;
        }
    assert(0);
    return false;
}

bool 
NodeData::hasOutput(const char* routename) const
{
    bool x3d = m_scene->isX3d();
    for (int i = 0; i < m_proto->getNumEventOuts(); i++)
        if (strcmp(m_proto->getEventOut(i)->getName(x3d), routename) == 0) {
            if (getOutput(i).size() > 0)
                return true;
            else
                return false;
        }
    assert(0);
    return false;
}

int
NodeData::writeProtoArguments(int f) const
{
    int flags = m_scene->getWriteFlags() | WITHOUT_VALUE;
    for (int i = 0; i < m_proto->getNumFields(); i++) {
        if (m_scene->isInvalidElement(m_proto->getField(i)))
            continue;
        RET_ONERROR( m_proto->getField(i)->write(f, 1, flags) )
        TheApp->incSelectionLinenumber();
    }
    for (int i = 0; i < m_proto->getNumExposedFields(); i++) {
        if (m_scene->isInvalidElement(m_proto->getExposedField(i)))
            continue;
        RET_ONERROR( m_proto->getExposedField(i)->write(f, 1, flags) )
        TheApp->incSelectionLinenumber();
    }
    for (int i = 0; i < m_proto->getNumEventIns(); i++) {
        if (m_scene->isInvalidElement(m_proto->getEventIn(i)))
            continue;
        RET_ONERROR( m_proto->getEventIn(i)->write(f, 1, flags) )
        TheApp->incSelectionLinenumber();
    }
    for (int i = 0; i < m_proto->getNumEventOuts(); i++) {
        if (m_scene->isInvalidElement(m_proto->getEventOut(i)))
            continue;
        RET_ONERROR( m_proto->getEventOut(i)->write(f, 1, flags) )
        TheApp->incSelectionLinenumber();
    }
    return 0;
}

bool
NodeData::hasDefault(int flag)
{
    bool x3d = m_scene->isX3d();
    for (int i = 0; i < m_numFields; i++) {
        Field *field = m_proto->getField(i);
        if (field->getFlags() & flag)
            if (!(m_fields[i]->equals(field->getDefault(x3d))))
                return false;
    }
    return true;
}

static bool searchInputs(Node *node, void *data)
{
    bool *result = (bool *)data;
    if (node->hasInputs()) {
        *result = true;
        return false;
    }
    return true;
}

bool 
Node::hasBranchInputs(void)
{
    bool result = false;
    doWithBranch(searchInputs, &result);
    return result;
}

static bool removeConvertedNode(Node *node, void *data)
{
    if (node != NULL) {
        node->removeRoutes();
        node->getScene()->removeNode(node);
        node->getScene()->undef(node->getName());
        node->getScene()->OnRemoveNode(node, NULL, -1);
    }
    return true;
}     


void              
NodeData::deleteConvertedNodes(void)
{
    for (int i = 0; i < m_convertedNodes.size(); i++)
        m_convertedNodes[i]->doWithBranch(removeConvertedNode, NULL, 
                                         false);
    m_convertedNodes.resize(0);
}

bool              
NodeData::needExtraJavaClass(void)
{
    if (m_convertedNodes.size() > 0)
        for (int i = 0; i < m_convertedNodes.size(); i++)
            if (m_convertedNodes[i]->needExtraJavaClass())
                return true;
    if (hasName() || (m_scene->getRoot() == this))
        return false;
    return true;
}

bool
NodeData::writeJavaOutsideClass(int languageFlag)
{
    return ((languageFlag & MANY_JAVA_CLASSES) &&
            (languageFlag & OUTSIDE_JAVA_CLASS));
}

void
NodeData::generateTreeLabel(void)
{
    m_treeLabel = "";
    if (TheApp->is4Catt())
        if (isMesh()) {
            if (hasTwoSides()) {
               if (isDoubleSided())
                   m_treeLabel += "(2 sides) ";
               else
                   m_treeLabel += "(1 side) ";
            } else
               m_treeLabel += "(only 1 side) ";
        }     
    if (hasName()) {
        m_treeLabel += getName();
        const char *protoName = m_proto->getName(m_scene->isX3d());
        if (stringncmp(getName(), protoName) != 0) { 
            m_treeLabel += " ";
            m_treeLabel += protoName;
        }
    } else
        m_treeLabel += m_proto->getName(m_scene->isX3d());
}

Node *
Node::getParentOrProtoParent(void) const
{ 
    Node *parent = getParent();
    if (parent == NULL)
        if (((NodeData *)this)->isInsideProto() && (m_nodePROTO != NULL))
            return m_nodePROTO->getParent();
    return parent;
}

int
Node::getParentFieldOrProtoParentField(void) const
{ 
    Node *parent = getParent();
    if (parent == NULL)
        if (((NodeData *)this)->isInsideProto() && (m_nodePROTO != NULL))
            return m_nodePROTO->getParentField();
    return getParentField();
}

int
Node::getParentIndex(void) const
{
    FieldValue *value = getParentFieldValue();
    if (value == NULL)
        return -1;
    if (value->getType() == MFNODE) {
        MFNode *mfValue = (MFNode *)value;
        for (int i = 0; i < mfValue->getSize(); i++) {
            Node *node = mfValue->getValue(i);
            if (node->isEqual((Node *)this))
                if (m_geometricParentIndex == node->getGeometricParentIndex())
                    return i;
            }
    }
    return -1;
}

Node *
Node::searchParent(int nodeType) const
{
    for (int i = 0; i < m_parents.size(); i++) {
        Node *node = m_parents[i].m_node;
        if (node->getType() == nodeType)
            return node;
    }
    return getParent();
}

Node *
Node::searchParentField(int parentField) const
{
    for (int i = 0; i < m_parents.size(); i++) {
        if (m_parents[i].m_field == parentField)
            return m_parents[i].m_node;
    }
    return getParent();
}

FieldValue *
Node::getParentFieldValue(void) const
{
    if (!hasParent())
        return NULL;
    Node *parent = getParent();
    int parentField = getParentField();
    return parentField == -1 ? NULL : parent->getField(parentField);
}

int
Node::getPrevSiblingIndex(void)
{
    int parentIndex = getParentIndex();
    if (parentIndex > 0)
        return parentIndex - 1;
    return -1;
}

int
Node::getNextSiblingIndex(void)
{
    FieldValue *value = getParentFieldValue();
    if (value == NULL)
        return -1;
    if (value->getType() == MFNODE) {
        MFNode *mfValue = (MFNode *)value;
        for (int i = 0; i < mfValue->getSize() - 1; i++) {
            Node *node = mfValue->getValue(i);
            if (node->isEqual((Node *)this))
                if (m_geometricParentIndex == node->getGeometricParentIndex())
                    return i + 1;
            }
    }
    return -1;
}

Node *
Node::getPrevSibling(void)
{
    FieldValue *value = getParentFieldValue();
    if (value == NULL)
        return NULL;
    if (value->getType() == MFNODE) {
        MFNode *mfValue = (MFNode *)value;
        int parentIndex = getParentIndex();
        if (parentIndex > 0)
            return mfValue->getValue(parentIndex - 1);
    }
    return NULL;
}

Node *
Node::getNextSibling(void)
{
    FieldValue *value = getParentFieldValue();
    if (value == NULL)
        return NULL;
    if (value->getType() == MFNODE) {
        MFNode *mfValue = (MFNode *)value;
        for (int i = 0; i < mfValue->getSize() - 1; i++) {
            Node *node = mfValue->getValue(i);
            if (node->isEqual((Node *)this))
                if (m_geometricParentIndex == node->getGeometricParentIndex())
                    return mfValue->getValue(i + 1);
            }
    }
    return NULL;
}

void 
NodeData::setDefault(void)
{
    bool x3d = m_scene->isX3d();
    for (int i = 0; i < m_numFields; i++) {
        Field *field = m_proto->getField(i);
        if ((field->getType() != SFNODE) && (field->getType() != MFNODE))
            m_scene->setField((Node *)this, i, field->getDefault(x3d)->copy());
    }
}

void                
Node::setNodePROTO(NodePROTO *node) 
{ 
    m_nodePROTO = node; 
    m_insideProto = ((Node *)node)->getProto();
    setProtoParent(node);
}


bool
Node::isDeepInsideProto(void)
{
    if (m_insideProto)
        return true;
    if (isPROTO())
        return true;
    if (this == NULL)
        return false;
    Node *parent = this;
    if (hasParent())
         while (parent->hasParent()) {
             if (parent->isInsideProto())
                 return true;
             parent = parent->getParent();
        }
    else
        return false;
    return false;
}

char*
Node::getExportMaterialName(const char *defaultName)
{
    Node *node = this;   
    // search for DEF name till root of scenegraph
    while (1) {
        if (node->hasName()) {
            char *name = strdup(node->getName());
            if (TheApp->SkipMaterialNameBeforeFirstUnderscore()) {
                char *lastPartOfName = strchr(name, '_');
                if (lastPartOfName) {
                    if (strlen(lastPartOfName + 1) > 0) {
                        name = lastPartOfName + 1;
                    } else if (defaultName != NULL) {                        
                        return buildExportMaterialName(defaultName);
                    }
                }
            } 
            if (TheApp->SkipMaterialNameAfterLastUnderscore()) {
                char *endPointer = (char *)strrchr(name, '_');
                if (endPointer) {
                    endPointer[0] = 0;
                    if ((strlen(name) == 0) && (defaultName != NULL))
                        return buildExportMaterialName(defaultName);
                } 
            }
            return buildExportMaterialName(name);
        } else {
            if (node->hasParent())
                node = node->getParent();
            else {
               if (defaultName != NULL)
                   return buildExportMaterialName(defaultName);
            }           
        }
    }
    return buildExportMaterialName(defaultName);
}


Node *
NodeData::convert2X3d(void) {
    if (getFlag(NODE_FLAG_CONVERTED))
        return NULL;
    setFlag(NODE_FLAG_CONVERTED);
    for (int i = 0; i < m_numFields; i++)
        if (m_fields[i] != NULL) {
           if (m_fields[i]->getType() == SFNODE) {
               if (((SFNode *) m_fields[i])->getValue())
                   ((SFNode *) m_fields[i])->getValue()->convert2X3d();
           } else if (m_fields[i]->getType() == MFNODE)
               ((MFNode *) m_fields[i])->convert2X3d();
        }
    return NULL;
}

Node *
NodeData::convert2Vrml(void) {
    if (getFlag(NODE_FLAG_CONVERTED))
        return NULL;
    setFlag(NODE_FLAG_CONVERTED);
    for (int i = 0; i < m_numFields; i++)
        if (m_fields[i] != NULL) {
           if (m_fields[i]->getType() == SFNODE) {
               if (((SFNode *) m_fields[i])->getValue())
                   ((SFNode *) m_fields[i])->getValue()->convert2Vrml();
           } else if (m_fields[i]->getType() == MFNODE)
               ((MFNode *) m_fields[i])->convert2Vrml();
        }
    return NULL;
}

bool
NodeData::canWriteAc3d(void)
{ 
    for (int i = 0; i < m_numFields; i++)
        if (m_fields[i] != NULL) {
            if (m_fields[i]->getType() == SFNODE) {
                Node *node = ((SFNode *)m_fields[i])->getValue();
                if (node && (node != this)) // avoid simple cyclic scenegraph
                    return node->canWriteAc3d();
            } else if (m_fields[i]->getType() == MFNODE) {
                MFNode *nodes = (MFNode *) m_fields[i];
                for (int j = 0; j < nodes->getSize(); j++) {
                    Node *node = nodes->getValue(j);
                    if (node && (node != this)) // avoid simple cyclic scenegraph
                        if (node->canWriteAc3d())
                            return true; 
                } 
            }     
        }
    return false; 
}

int
NodeData::writeAc3d(int f, int indent)
{ 
    for (int i = 0; i < m_numFields; i++)
        if (m_fields[i] != NULL) {
           if (m_fields[i]->getType() == SFNODE)
               RET_ONERROR( ((SFNode *) m_fields[i])->writeAc3d(f, indent) )
           else if (m_fields[i]->getType() == MFNODE)
               RET_ONERROR( ((MFNode *) m_fields[i])->writeAc3d(f, indent) )
        }
    return 0; 
}

int
NodeData::writeRib(int f, int indent)
{ 
    for (int i = 0; i < m_numFields; i++)
        if (m_fields[i] != NULL) {
           if (m_fields[i]->getType() == SFNODE)
               RET_ONERROR( ((SFNode *) m_fields[i])->writeRib(f, indent) )
           else if (m_fields[i]->getType() == MFNODE)
               RET_ONERROR( ((MFNode *) m_fields[i])->writeRib(f, indent) )
        }
    return 0; 
}

void 
NodeData::handleAc3dMaterial(ac3dMaterialCallback callback, Scene* scene)
{
    for (int i = 0; i < m_numFields; i++)
        if (m_fields[i] != NULL) {
           if (m_fields[i]->getType() == SFNODE) {
               SFNode *field = (SFNode *) m_fields[i];
               field->handleAc3dMaterial(callback, scene);
           } else if (m_fields[i]->getType() == MFNODE) {
               MFNode *field = (MFNode *) m_fields[i];
               field->handleAc3dMaterial(callback, scene);
           }
        }
}

long                
NodeData::getId(void) 
{ 
    return m_identifier; 
}

bool
Node::canWriteCattGeo(void)
{ 
    for (int i = 0; i < m_numFields; i++)
        if (m_fields[i] != NULL) {
           if (m_fields[i]->getType() == SFNODE) {
               Node *node = ((SFNode *)m_fields[i])->getValue();
               if (node && (node != this)) // avoid simple cyclic scenegraph
                   return node->canWriteCattGeo();
           } else if (m_fields[i]->getType() == MFNODE) {
               MFNode *nodes = (MFNode *) m_fields[i];
               for (int j = 0; j < nodes->getSize(); j++) {
                   Node *node = nodes->getValue(j);
                   if (node && (node != this)) // avoid simple cyclic scenegraph
                       if (node->canWriteCattGeo())
                           return true; 
               }
           }     
        }
    return false; 
}

int
Node::writeCattGeo(int f, int indent)
{ 
    for (int i = 0; i < m_numFields; i++)
        if (m_fields[i] != NULL) {
           if (m_fields[i]->getType() == SFNODE)
               RET_ONERROR( ((SFNode *) m_fields[i])->writeCattGeo(f, indent) )
           else if (m_fields[i]->getType() == MFNODE)
               RET_ONERROR( ((MFNode *) m_fields[i])->writeCattGeo((Node *)this,
                                                                   f, indent) )
        }
    return 0; 
}

bool
NodeData::canWriteLdrawDat(void)
{ 
    for (int i = 0; i < m_numFields; i++)
        if (m_fields[i] != NULL) {
           if (m_fields[i]->getType() == SFNODE) {
               Node *node = ((SFNode *)m_fields[i])->getValue();
               if (node && (node != this)) // avoid simple cyclic scenegraph
                   return node->canWriteLdrawDat();
           } else if (m_fields[i]->getType() == MFNODE) {
               MFNode *nodes = (MFNode *) m_fields[i];
               for (int j = 0; j < nodes->getSize(); j++) {
                   Node *node = nodes->getValue(j);
                   if (node && (node != this)) // avoid simple cyclic scenegraph
                       if (node->canWriteLdrawDat())
                           return true; 
               }
           }     
        }
    return false; 
}

int
NodeData::writeLdrawDat(int f, int indent)
{ 
    for (int i = 0; i < m_numFields; i++)
        if (m_fields[i] != NULL) {
           if (m_fields[i]->getType() == SFNODE)
               RET_ONERROR( ((SFNode *) m_fields[i])->writeLdrawDat(f, indent) )
           else if (m_fields[i]->getType() == MFNODE)
               RET_ONERROR( ((MFNode *) m_fields[i])->writeLdrawDat(f, indent) )
        }
    return 0; 
}

NodeHAnimHumanoid *
Node::getHumanoid()
{
    NodeHAnimHumanoid *human = NULL;
    Node *child = this;
    while (child && (child != m_scene->getRoot()))  {
        if (child->getType() == X3D_HANIM_HUMANOID) {
            human = (NodeHAnimHumanoid *)child;
            break;
         }
         child = child->getParent();
    } 
    return human;
}

bool        
Node::showFields() 
{ 
    if (m_proto->showFields())
        return true;
    return m_proto->showFields(m_scene->isX3d()); 
}

int
Node::getContainerField(void)
{
    return m_containerField;
}

void             
Node::setContainerField(int containerField)
{
    m_containerField = containerField;
}

NodeList
Node::getParents(void)
{
    NodeList ret;
    for (int i = 0; i < getNumParents(); i++)
        ret.append(getParent(i));
    return ret;
}

bool       
NodeData::isPROTO(void) const 
{ 
    return false;
}

