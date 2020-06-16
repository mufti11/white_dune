/*
 * SFVec3d.h
 *
 * Copyright (C) 1999 Stephen F. White, 2006 J. "MUFTI" Scheurich
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

#ifndef _SFVEC3D_H
#define _SFVEC3D_H

#ifndef _SFVEC3X_H
# include "SfVec3x.h"
#endif
#ifndef _VEC3D_H
# include "Vec3d.h"
#endif
#ifndef _SFSTRING_H
# include "SFString.h"
#endif

class SFVec3d : public SfVec3x<double> {
public:
                        SFVec3d(const Vec3d &v)
                          {
                          m_value[0] = v.x; 
                          m_value[1] = v.y; 
                          m_value[2] = v.z;
                          }
                        SFVec3d(double x, double y, double z)
                           {
                           m_value[0] = x; 
                           m_value[1] = y; 
                           m_value[2] = z;
                           }
                        SFVec3d(const double* value)
                           {
                           m_value[0] = value[0]; 
                           m_value[1] = value[1]; 
                           m_value[2] = value[2];
                           }
                        SFVec3d(const float* value)
                           {
                           m_value[0] = value[0]; 
                           m_value[1] = value[1]; 
                           m_value[2] = value[2];
                           }
                        SFVec3d(const char* value)
                           {
                           setValue(value);
                           }
                        SFVec3d(void)  // silly default
                           {
                           m_value[0] = m_value[1] = m_value[2] = 0.0; 
                           }
                        SFVec3d(SFString *value);

    virtual int         getType() const { return SFVEC3D; }
    virtual const char *getTypeName() const { return "SFVec3d"; }
    virtual int         getStride() const { return 3; }
    virtual MyString    getString(int index, int stride) const;
    virtual FieldValue *copy() { return new SFVec3d(*this); }
    virtual bool        equals(const FieldValue *value) const;
    virtual void        clamp(const FieldValue *min, const FieldValue *max);
    virtual bool        supportAnimation(bool x3d) const { return false; }
    virtual bool        supportInteraction(void) const { return false; }
    MyString            getEcmaScriptComment(MyString name, int flags) const;

    virtual int         writeData(int filedes, int i) const; 

    virtual int         writeC(int filedes, const char* variableName,
                               int languageFlag) const;
    virtual const char *getTypeC(int languageFlag) const { return "double"; }
    virtual bool        isArrayInC(void) const { return true; }

    virtual bool        readLine(int index, char *line);
    virtual void        setValue(int index, double value)
                           {
                           assert(index >= 0 && index < 3);
                           m_value[index] = value;
                           }
    void                setValue(const char *value)
                           {
                           sscanf(value, "%lf %lf %lf",
                                  &m_value[0], &m_value[1], &m_value[2]);
                           }
    virtual bool        isX3DType() { return true; }

    FieldValue         *getRandom(Scene *scene, int nodeType) 
                           { return new SFVec3d(FLOAT_RAND(), FLOAT_RAND(), 
                                                FLOAT_RAND()); }
};

#endif // _SFVEC3D_H
