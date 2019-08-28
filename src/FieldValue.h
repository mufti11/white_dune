/*
 * FieldValue.h
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

#ifndef _FIELDVALUE_H
#define _FIELDVALUE_H

#ifndef _ARRAY_H
#include "Array.h"
#endif
#ifndef _DUNE_STRING_H
#include "MyString.h"
#endif
#ifndef _ELEMENT_H
#include "Element.h"
#endif

class Scene;
class Node;

#include "Arrays.h" 

#include "MySwap.h"
#include "Types.h"

#ifdef _WIN32
# define drand48() (rand()/(double)RAND_MAX)
#endif

#define RAND() (drand48())
#define FLOAT_RAND() (drand48() * 10.0f)
#define INT_RAND() ((int)(drand48() * 10.0))

class FieldValue {
public:
                        FieldValue();
    virtual            ~FieldValue() {}
    
    virtual int         getStride() const { return 1; }

    // int i argument is used for MFieldValue, otherwise it is a dummy argument
    virtual int         writeData(int filedes, int i) const = 0;
    // write data without quotes to file
    virtual int         writeRaw(int filedes, int indent) const { return 0; }
    virtual int         writeDequoted(int filedes, const char *string);
    // write data in VRML syntax to file
    virtual int         write(int filedes, int indent) const;
    // write data in X3D/XML syntax to file
    virtual int         writeXml(int filedes, int indent) const;
    virtual int         writeDataXml(int filedes, int i) const 
                            { return writeData(filedes, i); }
    // write data in AC3D b syntax to file
    virtual int         writeAc3d(int filedes, int indent) const;
    // write data in Catt 8 syntax to file
    virtual int         writeCattGeo(int filedes, int indent) const;
    // write data in C++/Java syntax to file
    virtual int         writeC(int filedes, const char* variableName,
                               int languageFlag) const;
    virtual int         writeCDeclaration(int filedes, int languageFlag) const 
                           { return 0; }
    virtual int         writeCSendEventFunction(int filedes, int languageFlag);
    virtual int         writeDataC(int filedes, int i, int languageFlag) const 
                           { return writeData(filedes, i); }
    virtual int         writeCWonderlandArt(int filedes, 
                                            const char* variableName,
                                            int languageFlag) const 
                           { return writeC(filedes, variableName, 
                                           languageFlag); }
    virtual const char *getTypeC(int languageFlag) const = 0;
    virtual const char *getDefaultC(int languageFlag) const;
    virtual bool        isArrayInC(void) const { return false; }

    virtual int         write4FieldPipe(int filedes, int indent) const 
                            { return write(filedes, indent); }

    virtual bool        readLine(int index, char *line) = 0;

    virtual bool        isMFieldValue(void) const { return false; }
    virtual bool        isNode(void) const { return false; }

    virtual bool        isNullNode(void) const { return false; }
    virtual bool        isUseNode(void) const { return false; }

    virtual bool        isNull(void) const { return false; }

    virtual const char *getDefName(void) const { return ""; }

    virtual bool        checkInput(char *line);
    virtual int         getNumbersPerType(void) const = 0;
    virtual bool        needCheckFloat(void) const { return false; }

    virtual int         getType() const = 0;
    virtual const char *getTypeName() const = 0;
    virtual MyString    getString(int index = -1, int stride = -1) const 
                           { return "";};

    virtual bool        equals(const FieldValue *value) const = 0;
    virtual bool        equalsAngle(const FieldValue *value, double angleUnit) 
                            const { return false; }
    virtual FieldValue *addNode(Node *node, int index = -1) const;
    virtual FieldValue *removeNode(Node *node) const;

    virtual void        clamp(const FieldValue *min, const FieldValue *max) {}

    virtual void        flip(int index) {}

    void                ref() 
                           { 
#ifdef HAVE_NULL_COMPARE
                           if (this != NULL) 
#endif
                           m_refs++; 
                           }
    void                unref() 
                           { 
                           if ((this != NULL) && (--m_refs == 0))
                               delete this; 
                           }
    int                 getRefs() { return m_refs; }

    virtual FieldValue *copy() = 0;

    virtual void        fixAngle(double angleUnit) {}

    virtual MyString    getEcmaScriptComment(MyString name, int flags) 
                              const = 0;

    const char         *getEcmaScriptIndent(int  flags);

    virtual bool        supportAnimation(bool x3d) const = 0;
    virtual bool        supportInteraction(void) const { return false; }
    virtual void        makeEmpty(void) {}

    virtual bool        isX3DType() { return false; }

    bool                writeType(int languageFlag) const;
    
    bool                isDefaultValue(void) { return m_isDefaultValue; }
    void                setIsDefaultValue(void) { m_isDefaultValue = true; }
    void                removeIsDefaultValue(void) { m_isDefaultValue = false; }

    virtual int         getContainerField(void) { return -1; }
    virtual void        setContainerField(int f) {}

    //only for testing
    virtual FieldValue *getRandom(Scene *scene, int nodeType = -1) = 0;
private:
    int                 m_refs;
    bool                m_isDefaultValue;
};

class MFieldValue : public FieldValue
{
public:
    virtual int         getSFSize() const = 0;
    virtual FieldValue *getSFValue(int index) const = 0;
    virtual void        setSFValue(int index, FieldValue *value) = 0;
    virtual bool        isMFNode(void) const { return false; }
    virtual bool        isMFieldValue(void) const { return true; }
    virtual bool        writeBrackets(void) const { return(getSFSize() != 1); }
    virtual int         write(int filedes, int indent, 
                              bool writeBrackets) const;
    virtual int         write(int filedes, int indent) const 
                           { return write(filedes, indent, writeBrackets()); }
    virtual int         write4FieldPipe(int filedes, int indent) const 
                            { return write(filedes, indent, false); } 
    virtual int         writeXml(int filedes, int indent) const;
    virtual int         writeC(int filedes, const char* variableName,
                               int languageFlag) const;
    virtual int         writeCSendEventFunction(int filedes, int languageFlag);
    virtual bool        isArrayInC(void) const { return true; }

    int                 writeJavaLongArray(int filedes, int languageFlag,
                                           const char* variableName, 
                                           int offset, int length,
                                           bool wonderlandArt,
                                           const char *sceneUrl) const;
    virtual bool        isNull(void) const { return getSFSize() > 0; }

    virtual void        removeSFValue(int index) = 0;
    virtual void        makeEmpty(void) 
                           { 
                           for (int i = getSFSize() - 1; i >= 0; i--)
                               removeSFValue(i);
                           }
    void                getDiff(IntArray *newIndices, IntArray *oldIndices, 
                                MFieldValue *old);
    virtual bool        supportAnimation(bool x3d) const {return false; }
};

extern FieldValue *rewriteField(FieldValue *value, 
                                const char *oldBase, const char *newBase,
                                int writeFlags);

#endif // _FIELDVALUE_H
