/*
 * EventOut.h
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

# include "Element.h"
# include "Node.h"   

class ExposedField;

class EventOut : public Element {
public:
                    EventOut(int type, const MyString &name, int flags = 0, 
                             ExposedField *exposedField = NULL,
                             const MyString &x3dName = "");
                    EventOut(EventOut *ptr); 
    virtual        ~EventOut();
    virtual int     getElementType() const { return EL_EVENT_OUT; }
    virtual const char *getElementName(bool x3d) const;
    virtual int     write(int filedes, int indent, int flags = 0) const;

    ExposedField   *getExposedField() const { return m_exposedField; }

private:
    ExposedField   *m_exposedField;
};

