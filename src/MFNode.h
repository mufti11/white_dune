/*
 * MFNode.h
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

#ifndef _MFNODE_H
#define _MFNODE_H

#ifndef _FIELDVALUE_H
#include "FieldValue.h"
#endif
#ifndef _NODE_H
#include "Node.h"
#endif
#ifndef _NODELIST_H
#include "NodeList.h"
#endif

class MFNode : public MFieldValue {
public:
                        MFNode();
                        MFNode(NodeList *value);
                        MFNode(const MFNode &other);
                        MFNode(const MFNode *other);
                        MFNode(const Node *other);
    virtual            ~MFNode();

    virtual int         getType() const { return MFNODE; }
    virtual const char *getTypeName() const { return "MFNode"; }

    virtual bool        writeBrackets(void) const;
    virtual int         writeData(int filedes, int i) const; 
    virtual int         write(int filedes, int indent, 
                              bool writeBrackets) const;
    virtual int         write(int filedes, int indent) const 
                           { return write(filedes, indent, writeBrackets()); }
    virtual int         write4FieldPipe(int filedes, int indent) const 
                            { return write(filedes, indent, false); } 
    virtual int         writeXml(int filedes, int indent, int containerField,
                                 bool avoidUse) const;

    virtual int         writeC(int filedes, const char* variableName,
                               int languageFlag) const;
    virtual const char *getTypeC(int languageFlag) const;
    virtual int         writeCDeclaration(int filedes, int languageFlag) const;

    virtual bool        readLine(int index, char *line);

    virtual int         getNumbersPerType(void) const { return 0; }

    virtual bool        equals(const FieldValue *value) const;
    int                 getSize() const
                           {
#ifdef HAVE_NULL_COMPARE
                           if (this == NULL)
                               return 0; 
#endif
                           if (m_value == NULL) 
                               return 0;
                           return m_value->size(); 
                           }
    NodeList           *getValues() const 
                           { 
#ifdef HAVE_NULL_COMPARE
                           if (this == NULL)
                               return NULL; 
#endif
                           return m_value; 
                           }
    Node               *getValue(size_t index) const 
                           { 
                           if (m_value->size() == 0)
                               return NULL;
                           assert(index < m_value->size()); 
                           return m_value->get(index); 
                           }

    virtual int         getSFSize() const 
                           { 
                           if (m_value == NULL) 
                               return 0;
                           return m_value->size(); 
                           }
    virtual FieldValue *getSFValue(int index) const;
    virtual void        setSFValue(int index, FieldValue *value);
    virtual void        setSFValue(int index, const Node* value);

    virtual void        removeSFValue(int index) 
                           { removeNode((*m_value)[index]); }

    virtual FieldValue *addNode(Node *node, int index = -1) const;
    virtual FieldValue *removeNode(Node *node) const;
    virtual FieldValue *copy() { return new MFNode(*this); }

    MyString            getEcmaScriptComment(MyString name, int flags) const;

    virtual void        preDraw();
    virtual void        draw(int pass, int mfNodeField);

    virtual void        flip(int index);
    virtual void        swap(int fromTo);

    virtual int         countPolygons(void);
    virtual int         countPrimitives(void);

    virtual int         countPolygons1Sided(void);
    virtual int         countPolygons2Sided(void);

    virtual void        update();

    virtual Node       *convert2X3d(void);
    virtual Node       *convert2Vrml(void);

    virtual bool        isNode(void) const { return true; } 
    virtual bool        isMFNode(void) const { return true; } 

    virtual int         writeAc3d(int filedes, int indent) const;
    virtual void        handleAc3dMaterial(ac3dMaterialCallback callback, 
                                           Scene* scene);

    virtual int         writeRib(int filedes, int indent) const;

    virtual int         writeCattGeo(Node *node, int filedes, int indent) const;

    virtual int         writeLdrawDat(int filedes, int indent) const;

    FieldValue         *getRandom(Scene *scene, int nodeType);

    virtual int         getContainerField(void) { return m_containerField; }
    virtual void        setContainerField(int f) { m_containerField = f; }

    Vec3f               getBboxSize(void);
    Vec3f               getBboxCenter(void);

private:
    NodeList           *m_value;
    int                 m_containerField;
    
};

#endif // _MFNODE_H
