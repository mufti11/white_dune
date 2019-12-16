/*
 * SFNode.h
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

#ifndef _SFNODE_H
#define _SFNODE_H

#include "config.h"

#ifndef _FIELDVALUE_H
#include "FieldValue.h"
#endif

#include "Node.h"
#include "ac3dMaterialCallback.h"

class Element;

class SFNode : public FieldValue {
public:
                        SFNode(Node *value);
                        SFNode(Node *value, int containerField);
                        SFNode(const SFNode &other);
                        SFNode(void) { m_value = NULL; } // silly default
    virtual            ~SFNode();

    virtual int         getType() const 
                           { 
                           if (m_value && (Node *)m_value->hasNeedRef())
                               m_value->ref();
                           return SFNODE; 
                           }
    virtual const char *getTypeName() const
                           {
                           if (m_value && m_value->hasNeedRef())
                               m_value->ref();
                           return "SFNode"; 
                           }

    virtual int         writeData(int filedes, int i); 
    virtual int         write(int filedes, int indent);
    virtual int         writeXml(int filedes, int indent, int containerField,
                                 bool aviodUse);

    virtual int         writeC(int filedes, const char* variableName,
                               int languageFlag);
    virtual const char *getTypeC(int languageFlag) const;
    virtual int         writeCDeclaration(int filedes, 
                                          int languageFlag);  

    virtual bool        readLine(int index, char *line);

    virtual int         getNumbersPerType(void) const { return 0; }

    virtual bool        equals(const FieldValue *value) const;
    virtual FieldValue *copy();

    virtual bool        isNull() const { return m_value == NULL; }

    Node               *getValue() const { 
                                         if (m_value && m_value->hasNeedRef())
                                             m_value->ref();
#ifdef HAVE_NULL_COMPARE
                                         // strange fix needed for problem
                                         // incomplete PROTO implementation
                                         if (this==NULL) return NULL; 
#endif
                                         return m_value; 
                                         }

    virtual Node       *convert2X3d(void);
    virtual Node       *convert2Vrml(void);

    virtual FieldValue *addNode(Node *node, int index = -1);
    virtual FieldValue *removeNode(Node *node, int index = 1);

    MyString            getEcmaScriptComment(MyString name, int flags);

    virtual bool        isNode(void) const { return true; } 

    virtual bool        isNullNode(void) const;
    virtual bool        isUseNode(void) const;

    virtual const char *getDefName(void);

    virtual int         writeAc3d(int filedes, int indent);
    virtual void        handleAc3dMaterial(ac3dMaterialCallback callback, 
                                           Scene* scene);

    virtual int         writeRib(int filedes, int indent);

    virtual int         writeCattGeo(int filedes, int indent);

    virtual int         writeLdrawDat(int filedes, int indent);

    virtual bool        supportAnimation(bool x3d) { return false; }

    FieldValue         *getRandom(Scene *scene, int nodeType);

    virtual int         getContainerField(void) const
                            { return m_containerField; }
    virtual void        setContainerField(int f) { m_containerField = f; }

    virtual void        ref()
                           {
                           if (m_value && m_value->hasNeedRef()) {
                               m_value->ref();
                               m_value->unsetNeedRef();
                               return;
                           }
                           FieldValue::ref();
                           } 
    virtual void        unref()
                           { 
                           if (m_value && m_value->hasNeedRef()) {
                               m_value->ref();
                               m_value->unsetNeedRef();
                           }
                           FieldValue::unref();                           
                           }
    virtual int         getRefs() 
                           {
                           if (m_value && m_value->hasNeedRef()) {
                               m_value->ref();
                               m_value->unsetNeedRef();
                           }
                           return FieldValue::getRefs();
                           }
                            

private:
    Node               *m_value;
    int                 m_containerField;
};

#endif // _SFNODE_H
