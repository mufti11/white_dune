/*
 * SFImage.h
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

#ifndef _SFIMAGE_H
#define _SFIMAGE_H

#ifndef _FIELDVALUE_H
#include "FieldValue.h"
#endif
#ifndef _ARRAY_H
#include "Array.h"
#endif
#include "MFInt32.h"

class SFImage : public MFInt32 {
public:
                        SFImage(int width, int height, int depth, const int *pixels);
                        SFImage(void); // silly default

    virtual int         getType() const { return SFIMAGE; }
    virtual const char *getTypeName() const { return "SFImage"; }

    virtual bool        writeBrackets(void) { return false; }
    virtual int         writeData(int filedes, int i); 

    virtual int         write(int filedes, int indent);
    virtual int         writeXml(int filedes, int indent, int containerField,
                                 bool avoidUse);

    virtual bool        readLine(int index, char *line);

    virtual int         getNumbersPerType(void) const { return 1; }

    virtual bool        equals(const FieldValue *value) const;
    virtual FieldValue *copy() { return new SFImage(*this); }

    const int           getValue(long index)
                            { return m_value[index]; }
    virtual void        setSFValue(int index, FieldValue *value);

    int                 getWidth(void);
    void                setWidth(int width);
    int                 getHeight(void); 
    void                setHeight(int height); 
    int                 getComponents(void); 
    void                setComponents(int components); 
    int                *getPixels(void); 
    void                setPixel(int index, int pixel); 
    int                 getNumPixels(void);

//    void                flip(int index);
//    void                swap(int fromTo);

    MyString            getEcmaScriptComment(MyString name, int flags);

    bool                supportAnimation(bool x3d) { return true; }

    FieldValue         *getRandom(Scene *scene, int nodetype);
protected:
    void                resizeImage();
};

#endif // _SFIMAGE_H
