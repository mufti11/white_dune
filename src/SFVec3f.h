/*
 * SFVec3f.h
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

#ifndef _SFVEC3F_H
#define _SFVEC3F_H

#ifndef _SFVEC3X_H
# include "SfVec3x.h"
#endif
#ifndef _VEC3F_H
# include "Vec3f.h"
#endif

class SFVec3f : public SfVec3x<float> {
public:
                        SFVec3f(const Vec3f &v)
                          {
                          m_value[0] = v.x; 
                          m_value[1] = v.y; 
                          m_value[2] = v.z;
                          }
                        SFVec3f(float x, float y, float z)
                           {
                           m_value[0] = x; 
                           m_value[1] = y; 
                           m_value[2] = z;
                           }
                        SFVec3f(const float* value)
                           {
                           m_value[0] = value[0]; 
                           m_value[1] = value[1]; 
                           m_value[2] = value[2];
                           }
                        SFVec3f(void)  // silly default
                           {
                           m_value[0] = m_value[1] = m_value[2] = 0.0; 
                           }

    virtual int         getType() const { return SFVEC3F; }
    virtual const char *getTypeName() { return "SFVec3f"; }
    virtual int         getStride() { return 3; }
    virtual MyString    getString(int index, int stride);
    virtual FieldValue *copy() { return new SFVec3f(*this); }
    virtual bool        equals(FieldValue *value);
    virtual void        clamp(const FieldValue *min, const FieldValue *max);
    virtual bool        supportAnimation(bool x3d) { return true; }
    virtual bool        supportInteraction(void) { return true; }
    MyString            getEcmaScriptComment(MyString name, int flags);

    virtual int         writeData(int filedes, int i); 

    virtual int         writeC(int filedes, const char* variableName,
                               int languageFlag);
    virtual const char *getTypeC(int languageFlag) { return "float"; }
    virtual bool        isArrayInC(void) { return true; }
    void                setSFValue(FieldValue *value)
                            {
                            m_value[0] = ((SFVec3f *)value)->getValue()[0];
                            m_value[1] = ((SFVec3f *)value)->getValue()[1];
                            m_value[2] = ((SFVec3f *)value)->getValue()[2];
                            }

    virtual int         writeAc3d(int filedes, int indent);

    virtual bool        readLine(int index, char *line);

    Vec3f               getVec(void);

    FieldValue         *getRandom(Scene *scene, int nodeType) 
                           { return new SFVec3f(FLOAT_RAND(), FLOAT_RAND(), 
                                                FLOAT_RAND()); }
};

#endif // _SFVEC3F_H
