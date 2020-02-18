/*
 * DynamicFields.cpp
 *
 * Copyright (C) 2006 J. "MUFTI" Scheurichx
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

#include "Proto.h"
#include "DynamicFieldsNode.h"
#include "DuneApp.h"
#include "Scene.h"
#include "Field.h"
#include "ExposedField.h"
#include "EventIn.h"
#include "EventOut.h"
#include "RouteCommand.h"
#include "UnRouteCommand.h"
#include "NodeScript.h"

DynamicFieldsProto::DynamicFieldsProto(Scene *scene, const char* name)
  : Proto(scene, name)
{
}

void DynamicFieldsNode::initialise(void)
{
    m_routeList = NULL;
    m_proto->setDynamicProto();
}

int DynamicFieldsNode::write(int f, int indent, bool avoidUse)
{
    int flags = m_scene->getWriteFlags();
    TheApp->checkSelectionLinenumberCounting(m_scene, this);
    if (getFlag(NODE_FLAG_DEFED)) {
        RET_ONERROR( mywritestr(f ,"USE ") )
        RET_ONERROR( mywritestr(f ,(const char *) m_name) )
        RET_ONERROR( mywritestr(f ,"\n") )
        TheApp->incSelectionLinenumber();
    } else {
        if (needsDEF()) {
            if (!m_name[0]) m_scene->generateUniqueNodeName(this);
                RET_ONERROR( mywritestr(f,"DEF ") )
                RET_ONERROR( mywritestr(f, (const char *) m_name) )
                RET_ONERROR( mywritestr(f," ") )
            }
        setFlag(NODE_FLAG_DEFED);
        RET_ONERROR( mywritestr(f, getProto()->getName(::isX3d(flags))) )
        RET_ONERROR( mywritestr(f, " ") )
        if (!TheApp->GetkrFormating()) {
            RET_ONERROR( mywritestr(f, "\n") )
            TheApp->incSelectionLinenumber();
            RET_ONERROR( indentf(f, indent + TheApp->GetIndent()) )
        }
        RET_ONERROR( mywritestr(f, "{\n") )
        TheApp->incSelectionLinenumber();
        RET_ONERROR( writeFields(f, indent + TheApp->GetIndent()) )
        RET_ONERROR( writeEvents(f, indent + TheApp->GetIndent()) )
        if (!TheApp->GetkrFormating())
            RET_ONERROR( indentf(f, indent + TheApp->GetIndent()) )
        else
            RET_ONERROR( indentf(f, indent) )
        RET_ONERROR( mywritestr(f, "}\n") )
        TheApp->incSelectionLinenumber();
        if (indent==0) {
            RET_ONERROR( mywritestr(f ,"\n") )
            TheApp->incSelectionLinenumber();
        }
        setFlag(NODE_FLAG_TOUCHED);
        RET_ONERROR( writeRoutes(f, indent) )
    }
    return(0);
}

int DynamicFieldsNode::writeXml(int f, int indent, int containerField, 
                                bool avoidUse)
{
    if (getType() == VRML_SCRIPT) { 
        NodeScript *node = (NodeScript *) this;
        int i = 0;
        for (i = 0; i < m_proto->getNumFields(); i++) {
            if (i == node->directOutput_Field())
                continue;
            if (i == node->mustEvaluate_Field())
                continue;
            if (i == node->url_Field())
                m_proto->getField(i)->addFlags(FF_SCRIPT_URL);
            else
                m_proto->getField(i)->addFlags(FF_IN_SCRIPT);
        }
        for (i = 0; i < m_proto->getNumEventIns(); i++)
            m_proto->getEventIn(i)->addFlags(FF_IN_SCRIPT);
        for (i = 0; i < m_proto->getNumEventOuts(); i++)
            m_proto->getEventOut(i)->addFlags(FF_IN_SCRIPT);
        for (i = 0; i < m_proto->getNumExposedFields(); i++)
                m_proto->getExposedField(i)->addFlags(FF_IN_SCRIPT);
    }
    if (isPROTO())
        return ((NodePROTO *)this)->writeXml(f, indent, avoidUse);
    return Node::writeXml(f, indent, avoidUse);
}

int DynamicFieldsNode::writeField(int f, int indent, int fieldIndex)
{
    const char *oldBase = m_scene->getURL();
    const char *newBase = m_scene->getNewURL();
    bool tempSave = m_scene->isTempSave();

    bool x3d = m_scene->isX3d();

    Field *field = m_proto->getField(fieldIndex);
    ExposedField *exposedField = field->getExposedField();
    FieldValue *value = m_fields[fieldIndex];
    if ((field->getFlags() & FF_STATIC) || 
        (fieldIndex == m_proto->metadata_Field())) {
        if (value && !value->equals(field->getDefault(x3d))) {
            RET_ONERROR( indentf(f, indent) )
            RET_ONERROR( mywritestr(f, (const char *) field->getName(x3d)) )
            RET_ONERROR( mywritestr(f, " ") )
            if ((field->getFlags() & FF_URL)) {
                value = rewriteField(value, oldBase, newBase, 
                                     m_scene->getWriteFlags());
//                RET_ONERROR( indentf(f, indent) );
                RET_ONERROR( mywritestr(f, "\"") );
                RET_ONERROR( value->writeRaw(f, 0) )
                RET_ONERROR( mywritestr(f, "\n") );
                RET_ONERROR( indentf(f, indent) );
                RET_ONERROR( mywritestr(f, "\"\n") );
                if (!tempSave) {
                    setField(fieldIndex, value);
                } else {
                    delete value;
                }
            } else if (field->getFlags() & (FF_STATIC | FF_HIDDEN)) {
                RET_ONERROR( value->write(f, indent) )
            } else {
                RET_ONERROR( write(f, indent) )
            }
        }
    } else {
        Element *element = field;
        if (exposedField != NULL)
            element = exposedField;
        RET_ONERROR( indentf(f, indent) )
        RET_ONERROR( mywritestr(f, element->getElementName(x3d)) )
        RET_ONERROR( mywritestr(f, " ") )
        RET_ONERROR( mywritestr(f, typeEnumToString(element->getType())) ) 
        RET_ONERROR( mywritestr(f, " ") )
        RET_ONERROR( Node::writeField(f, indent, fieldIndex, true) )
    }
    return(0);
}

void incSelectionLinenumberOnMultilineMFString(MFString *mfstring)
{
    // count end of line characters in a multiline MFString like url in Script
    if (mfstring != NULL) {
        for (int i=0; i < mfstring->getSize() ; i++) {
            char* string = (char*) ((const char*) mfstring->getValue(i));
            while ((string=strchr(string, '\n')) !=NULL) {
                TheApp->incSelectionLinenumber();
                string++;
            }
        }
    }
}

int DynamicFieldsNode::writeFields(int f, int indent)
{
    if (!m_proto) return(0);
    for (int i = 0; i < m_numFields; i++) {
        RET_ONERROR( writeField(f, indent, i) )
        if (i == getMultilineMFStringField())
            incSelectionLinenumberOnMultilineMFString((MFString *) getField(i));
    }
    return(0);
}

int 
DynamicFieldsNode::writeEvents(int f, int indent)
{
    if (!m_proto) return(0);

    bool x3d = m_scene->isX3d();

    for (int i = 0; i < m_numEventIns; i++) {
        EventIn *eventIn = m_proto->getEventIn(i);
        if (eventIn->getFlags() & (FF_STATIC | FF_HIDDEN))
            continue; 
        ExposedField *exposedField = eventIn->getExposedField();
        if (exposedField && (exposedField->getFlags() & (FF_STATIC | FF_HIDDEN))
           )
            continue; 
        if (exposedField && !isPROTO())
            continue; 
        RET_ONERROR( indentf(f, indent) )
        RET_ONERROR( mywritestr(f, eventIn->getElementName(x3d)) )
        RET_ONERROR( mywritestr(f, " ") )
        RET_ONERROR( mywritestr(f, typeEnumToString(eventIn->getType())) ) 
        RET_ONERROR( mywritestr(f, " ") )
        RET_ONERROR( writeEventIn(f, 0, i, true) )
    }

    for (int i = 0; i < m_numEventOuts; i++) {
        EventOut *eventOut = m_proto->getEventOut(i);
        if (eventOut->getFlags() & (FF_STATIC | FF_HIDDEN))
            continue; 
        ExposedField *exposedField = eventOut->getExposedField();
        if (exposedField && (exposedField->getFlags() & (FF_STATIC | FF_HIDDEN))
           )
            continue; 
        if (exposedField && !isPROTO())
            continue; 
        RET_ONERROR( indentf(f, indent) )
        RET_ONERROR( mywritestr(f, eventOut->getElementName(x3d)) )
        RET_ONERROR( mywritestr(f, " ") )
        RET_ONERROR( mywritestr(f, typeEnumToString(eventOut->getType())) ) 
        RET_ONERROR( mywritestr(f, " ") )
        RET_ONERROR( writeEventOut(f, 0, i, true) )
    }

    return(0);
}

//  When a eventIn is deleted, the routes must be updated.
//  Routes are handled via "sockets", a list which contain (for eventIns)
//  the node of source of a route and a integer index to the event/field 
//  of the source route

void DynamicFieldsNode::updateEventIn(int newIndex, int oldIndex)
{
     CommandList* deleteList = new CommandList();
     for (SocketList::Iterator *i = m_inputs[oldIndex].first(); i != NULL; 
          i = i->next()) {
         RouteSocket outS = i->item();
         for (SocketList::Iterator *j = 
              outS.getNode()->getOutput(outS.getField()).first(); j != NULL; 
              j = j->next()) {
             RouteSocket inS = j->item();
             if ((inS.getNode() == this) && (inS.getField() == oldIndex)) {
                 // delete old route
                 deleteList->append(new UnRouteCommand(outS.getNode(), 
                                                       outS.getField(), 
                                                       this, oldIndex));
                 
                 // add new route to m_routeList
                 if (!m_routeList) m_routeList = new CommandList();
                 m_routeList->append(new RouteCommand(outS.getNode(), 
                                                      outS.getField(), 
                                                      this, newIndex));
             }
         }
     }
     m_scene->execute(deleteList);
}

//  When a eventOut is deleted, the routes must be updated.
//  Routes are handled via "sockets", a list which contain (for eventOut)
//  the node of target of a route and a integer index to the event/field 
//  of the target route

void DynamicFieldsNode::updateEventOut(int newIndex, int oldIndex)
{
     CommandList* deleteList = new CommandList();
     for (SocketList::Iterator *i = m_outputs[oldIndex].first(); i != NULL; 
          i = i->next()) {
         RouteSocket inS = i->item();
         for (SocketList::Iterator *j = inS.getNode()->getInput(inS.getField())
              .first(); j != NULL; j = j->next()) {
             RouteSocket outS = j->item();
             if ((outS.getNode() == this) && (outS.getField() == oldIndex)) {
                 // delete old route
                 deleteList->append(new UnRouteCommand(this, oldIndex,
                                                       inS.getNode(), 
                                                       inS.getField()));
                                                       
                 // add new route to m_routeList
                 if (!m_routeList) m_routeList = new CommandList();
                 m_routeList->append(new RouteCommand(this, newIndex,
                                                     inS.getNode(), 
                                                     inS.getField()));
             }
         }
     }
     m_scene->execute(deleteList);
}

// since scripts can have dynamically-added fields, we need to update
// the node's fields after the script is defined.
// Same problem for the scriptinterfacebuilder (ScriptDialog) (add or delete)
//
// It is important, that only one item (field, eventIn or eventOut) can
// be added or deleted during this call of update.

void
DynamicFieldsNode::updateDynamicFields()
{
    bool x3d = m_scene->isX3d();

    int newNumFields = m_proto->getNumFields();
    // add 
    if (m_numFields < newNumFields) {
        FieldValue **newFields = new FieldValue *[newNumFields];
        for (int i = 0; i < m_numFields; i++) {
            newFields[i] = m_fields[i];
        }
        for (int i = m_numFields; i < newNumFields; i++) {
            newFields[i] = m_proto->getField(i)->getDefault(x3d);
            if (newFields[i])
                newFields[i]->ref();
        }

        // avoid delete of m_fields[something]
        for (int i = 0 ; i < m_numFields ; i++)
            m_fields[i] = NULL;
        delete [] m_fields;
        m_fields = newFields;
        m_numFields = newNumFields;
    } else {
        // delete ?
        for (int i = 0; i < m_numFields; i++)
            if (((m_proto->getField(i)->getFlags() & FF_DELETED) != 0) ||
                 (m_proto->getField(i)->getExposedField() &&
                  ((m_proto->getField(i)->getExposedField()->getFlags() & 
                  FF_DELETED) != 0))) {
                // delete !
                newNumFields = m_numFields-1;
                FieldValue **newFields = new FieldValue *[newNumFields];
                int newIndex=0;
                for (int j = 0; j < m_numFields; j++)
                    if (j != i)
                        newFields[newIndex++] = m_fields[j];
                if (m_proto->getField(i)->getExposedField()) {
                    for (int j = 0; j < m_proto->getNumExposedFields(); j++)
                        if (m_proto->getExposedField(j) == 
                            m_proto->getField(i)->getExposedField())
                            m_proto->m_exposedFields.remove(j);
                }
                m_proto->m_fields.remove(i);

                // avoid delete of m_fields[something]
                for (int i = 0 ; i < m_numFields ; i++)
                    m_fields[i] = NULL;
                delete [] m_fields;
                m_fields = newFields;
                m_numFields = newNumFields;
                break;
            }
    }

    int newNumEventIns = m_proto->getNumEventIns();
    // add 
    if (m_numEventIns < newNumEventIns) {
        SocketList *newInputs = new SocketList[newNumEventIns];
        for (int i = 0; i < m_numEventIns; i++) {
            newInputs[i] = m_inputs[i];
        }

        // avoid delete of m_inputs[something]
        for (int i = 0 ; i < m_numEventIns ; i++)
            m_inputs[i] = SocketList();
        delete [] m_inputs;
        m_inputs = newInputs;
        m_numEventIns = newNumEventIns;
    } else {
        // delete ?
        for (int i = 0; i < m_numEventIns; i++)
            if ((m_proto->getEventIn(i)->getFlags() & FF_DELETED) != 0) {
                // delete !
                newNumEventIns = m_numEventIns-1;
                SocketList *newInputs = new SocketList[newNumEventIns];
                int newIndex=0;
                for (int j = 0; j < m_numEventIns; j++) {
                    if (newIndex != j)
                        updateEventIn(newIndex, j);
                    if (j != i)
                        newInputs[newIndex++] = m_inputs[j];
                }

                m_proto->m_eventIns.remove(i);

                // avoid delete of m_inputs[something!=i]
                for (int k = 0 ; k < m_numEventIns ; k++)
                    if (k != i)
                        m_inputs[k] = SocketList();
                delete [] m_inputs;
                m_inputs = newInputs;
                m_numEventIns = newNumEventIns;
                // execute addRouteCommands from updateEventIn()
                if (m_routeList) m_scene->execute(m_routeList);
                m_routeList = NULL;
                break;
            }
    }

    int newNumEventOuts = m_proto->getNumEventOuts();
    // add 
    if (m_numEventOuts < newNumEventOuts) {
        SocketList *newOutputs = new SocketList[newNumEventOuts];
        for (int i = 0; i < m_numEventOuts; i++) {
            newOutputs[i] = m_outputs[i];
        }

        // avoid delete of m_outputs[something]
        for (int i = 0 ; i < m_numEventOuts ; i++)
            m_outputs[i] = SocketList();
        delete [] m_outputs;
        m_outputs = newOutputs;
        m_numEventOuts = newNumEventOuts;
    } else {
        // delete ?
        for (int i = 0; i < m_numEventOuts; i++)
            if (m_proto->getEventOut(i) && 
                ((m_proto->getEventOut(i)->getFlags() & FF_DELETED) != 0)) {
                // delete !
                newNumEventOuts = m_numEventOuts-1;
                SocketList *newOutputs = new SocketList[newNumEventOuts];
                int newIndex=0;
                for (int j = 0; j < m_numEventOuts; j++) {
                    if (newIndex != j)
                        updateEventOut(newIndex, j);
                    if (j != i) 
                        newOutputs[newIndex++] = m_outputs[j];
                }
                m_proto->m_eventOuts.remove(i);

                // avoid delete of m_outputs[something!=i]
                for (int k = 0 ; k < m_numEventOuts ; k++)
                    if (k != i)
                        m_outputs[k] = SocketList();
                delete [] m_outputs;
                m_outputs = newOutputs;
                m_numEventOuts = newNumEventOuts;
                // execute addRouteCommands from updateEventIn()
                if (m_routeList) m_scene->execute(m_routeList);
                m_routeList = NULL;
                break;
            }
    }
}

InterfaceArray *
DynamicFieldsNode::getInterfaceData(void)
{
    return m_proto->getInterfaceData(m_scene->isX3d()); 
} 

void            
DynamicFieldsNode::addField(int fieldType, const MyString &name)
{
    getProto()->addField(fieldType, name, typeDefaultValue(fieldType));
}

void            
DynamicFieldsNode::addExposedField(int fieldType, const MyString &name)
{
    getProto()->addExposedField(fieldType, name, typeDefaultValue(fieldType));
}

void            
DynamicFieldsNode::addEventIn(int fieldType, const MyString &name)
{
    getProto()->addEventIn(fieldType, name);
}

void            
DynamicFieldsNode::addEventOut(int fieldType, const MyString &name)
{
    getProto()->addEventOut(fieldType, name);
}

void            
DynamicFieldsNode::deleteEventOut(int index)
{
    getProto()->deleteEventOut(index);
}

