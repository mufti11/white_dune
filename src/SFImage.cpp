/*
 * SFImage.cpp
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

#include <stdio.h>
#include <string.h>
#include "stdafx.h"

#include "SFImage.h"
#include "DuneApp.h"

SFImage::SFImage(int width, int height, int components, const int *pixels)
{
    setWidth(width);
    setHeight(height);
    setComponents(components);
    resizeImage();
    if (pixels)
        for (int i = 0; i < width * height; i++)
            setPixel(i + 3, pixels[i]);
}

SFImage::SFImage(void)
{
    setWidth(0);
    setHeight(0);
    setComponents(0);
}

int SFImage::writeData(int f, int i) const
{
    int number;
    switch (i) {
      case 0:
        number = getWidth();
        break;
      case 1:
        number = getHeight();
        break;
      case 2:
        number = getComponents();
        break;
      default:
        number = getPixels()[i - 3];
    }
    return mywritef(f, "0x%08x", number);
}

int SFImage::write(int f, int indent) const
{
    RET_ONERROR( mywritef(f, "%d %d %d\n", getWidth(), getHeight(), 
                          getComponents()) )
    for (int i = 0; i < getNumPixels(); i++) {
        RET_ONERROR( indentf(f, indent + TheApp->GetIndent()) )
        RET_ONERROR( mywritef(f, "0x%08x\n", getPixels()[i]) )
        TheApp->incSelectionLinenumber();
    }
    return 0;
}

int SFImage::writeXml(int f, int indent) const
{
    RET_ONERROR( mywritef(f, "'%d %d %d", getWidth(), getHeight(), 
                          getComponents()) )
    for (int i = 0; i < getNumPixels(); i++)
        RET_ONERROR( mywritef(f, " 0x%08x", getPixels()[i]) )
    RET_ONERROR( mywritef(f, "'") )
    return 0;
}

bool        
SFImage::readLine(int index, char *line)
{
    return false;
}

bool
SFImage::equals(const FieldValue *value) const
{
    if (value->getType() == SFIMAGE) {
        SFImage *v = (SFImage *) value;
        bool samePixels = false;
        if ((v->getPixels() == NULL) && (getPixels() == NULL))
            samePixels = true;
        else if (v->getPixels() == NULL)
            samePixels = false;
        else if (getPixels() == NULL)
            samePixels = false;
        else
            samePixels = !memcmp(v->getPixels(), getPixels(), 
                                 getWidth() * getHeight());

        return v->getWidth() == getWidth()
            && v->getHeight() == getHeight()
            && v->getComponents() == getComponents()
            && samePixels;
    }
    return false;
}

int
SFImage::getWidth(void) const
{
    if (m_value.size() > 0)
        return m_value[0];
    else
        return 0;
}

void 
SFImage::setWidth(int width)
{
    for (int i = 0; i < 1; i++)
        if ((int)m_value.size() <= i) {
            m_value.resize(i + 1); 
            m_value[i] = 0;
        }
    m_value[0] = width;    
    resizeImage();
}

int
SFImage::getHeight() const
{
    if (m_value.size() > 1)
        return m_value[1];
    else
        return 0;
}
 
void
SFImage::setHeight(int height)
{
    for (int i = 0; i < 2; i++)
        if ((int)m_value.size() <= i) {
            m_value.resize(i + 1); 
            m_value[i] = 0;
        }
    m_value[1] = height;
    resizeImage();
}

int
SFImage::getComponents() const
{
    if ((int)m_value.size() > 2)
        return m_value[2];
    else
        return 0;
}
   
void
SFImage::setComponents(int components)
{
    for (int i = 0; i < 3; i++)
        if ((int)m_value.size() <= i) {
            m_value.resize(i + 1); 
            m_value[i] = 0;
        }
    m_value[2] = components;
    resizeImage();
}

const int *
SFImage::getPixels() const
{
    if ((getWidth() != 0) && (getHeight() != 0) && (getComponents() != 0))
        return m_value.getData() + 3;
    else
        return NULL;
}

void
SFImage::setPixel(int index, int pixel)
{
    m_value[index] = pixel;
}
 
int
SFImage::getNumPixels() const
{
    int pixels = getWidth() * getHeight();
    if (pixels > ((int)m_value.size() - 3))
        pixels = m_value.size() - 3;
    return pixels;
}

void
SFImage::setSFValue(int index, FieldValue *value)
{
   MFInt32::setSFValue(index, value);
   if (index == 0)
      setWidth(m_value[index]);
   else if (index == 1)
      setHeight(m_value[index]);
   else if (index == 2)
      setComponents(m_value[index]);
}


MyString
SFImage::getEcmaScriptComment(MyString name, int flags) const
{
    const char *indent = ((FieldValue *)this)->getEcmaScriptIndent(flags);
    MyString ret;
    ret = "";
    if (TheApp->GetEcmaScriptAddAllowedValues()) {
        ret += indent;
        ret += "// allowed values:\n";

        ret += indent;
        ret += "   // 3 integer values + ";
        ret += name;
        ret += "[0]*";
        ret += name;
        ret += "[1]*";
        ret += name;
        ret += "[2] byte (range 0-256) values\n";
    }
    if (TheApp->GetEcmaScriptAddAllowedComponents()) {
        ret += indent;
        ret += "// allowed components:\n";

        ret += indent;
        ret += "   // width  of image: ";
        ret += name;
        ret += "[0]\n";

        ret += indent;
        ret += "   // height of image: ";
        ret += name;
        ret += "[1]\n";

        ret += indent;
        ret += "   // number of components of image (e.g. 3 for rgb, 4 for rgba): ";
        ret += name;
        ret += "[2]\n";

        ret += indent;
        ret += "   // the following numbers represent pixel values \n";
        ret += indent;
        ret += "   // pixel value: integer with (number of components) bytes (0-255))\n";
    }
    if (TheApp->GetEcmaScriptAddAvailableFunctions()) {
        ret += indent;
        ret += "// available functions:\n";
        if (flags != EL_EVENT_IN) {
            ret += indent;
            ret += "   // ";
            ret += name;
            ret += " = new SFImage(numeric_x, numeric_y, numeric_comp, mfint32_iarray);\n";
        }
        if (flags != EL_EVENT_OUT) {
            ret += indent;
            ret += "   // string_str = ";
            ret += name;
            ret += ".toString();\n";
       }
    }
    if (TheApp->GetEcmaScriptAddExampleUsage()) {
        ret += indent;
        ret += "// example usage:\n";
        if (flags != EL_EVENT_IN) {
             ret += indent;
             ret += "   // ";
             ret += name;
             ret += " = new SFImage(2, 1, 3,  ";
             ret += "new MFInt32(0xFF0000,0x0000FF));\n";
        } 
        if (flags != EL_EVENT_OUT) {
             ret += indent;
             ret += "   // int_height = ";
             ret += name;
             ret += "[1];\n";
        }
    }
    return ret;
}

void
SFImage::resizeImage(void)
{
    int oldSize = m_value.size();
    if (getNumPixels() == 0)
        return;
    if ((getNumPixels() + 3) != oldSize) {
        m_value.resize(getNumPixels() + 3);
        // fill empty room with a black & white pattern
        for (size_t i = oldSize; i < m_value.size(); i++)
            if (i & 1)
               m_value[i] = 0;
            else {
                m_value[i] = 0xff;
                for (int j = 1; j <= getComponents(); j++)
                    m_value[i] = (m_value[i] << 8) | 0xff;
            }           
    }
}

FieldValue *
SFImage::getRandom(Scene *scene, int nodeType)
{
    int width = INT_RAND();
    int height = INT_RAND();
    int components = 3;
    int size = width * height * components;
    int *array = new int[size];
    for (int i = 0; i < size; i++)
        array[i] = INT_RAND();
    return new SFImage(width, height, components, array);
}


