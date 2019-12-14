/*
 * MFVec2f.h
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

#pragma once

#include "MFFloat.h"

class Vec2f;

class MFVec2f : public MFFloat {
public:
                        MFVec2f();
                        MFVec2f(MFVec2f *values) :
                             MFFloat(((MFVec2f *)values)->getValues(), 
                                     ((MFVec2f *)values)->getSize()) {}
                        MFVec2f(int len) : MFFloat(len * 2) {}
                        MFVec2f(float *values, int len) :
                             MFFloat(values, len) {}
                        MFVec2f(double *values, int len) :
                             MFFloat(values, len) {}
                        MFVec2f(float x, float y) : MFFloat(2) 
                             {
                             m_value[0] = x;
                             m_value[1] = y;
                             }

    virtual int         getType() const { return MFVEC2F; }
    virtual const char *getTypeName() { return "MFVec2f"; }
    virtual int         getStride() { return 2; }
    virtual FieldValue *copy();

    virtual bool        readLine(int index, char *line);

    virtual bool        equals(FieldValue *value);

    float              *getValue(int index)
                           { return m_value.getData() + index * 2; } 

    void                setVec(int index, Vec2f v);
    Vec2f               getVec(int index);

    virtual FieldValue *getSFValue(int index);
    virtual void        setSFValue(int index, FieldValue *value);
    virtual void        setSFValue(int index, float *values);
    virtual void        setSFValue(int index, float x, float y);

    virtual void        insertSFValue(int index, FieldValue *value);
    virtual void        insertSFValue(int index, float *values);
    virtual void        insertSFValue(int index, float x, float y);

    virtual void        appendSFValue(float x, float y) 
                            { insertSFValue(getSFSize(), x, y); }
                        
    virtual void        removeSFValue(int index) 
                            { 
                            m_value.remove(2 * index); 
                            m_value.remove(2 * index); 
                            }

    void                flip(int index);
    void                swap(int fromTo);

    bool                supportAnimation(bool x3d) { return x3d; }

    MyString            getEcmaScriptComment(MyString name, int flags);

    randomFunction(MFVec2f)                  
};
