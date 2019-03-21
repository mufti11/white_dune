/*
 * NodeLayout.cpp
 *
 * Copyright (C) 2009 J. "MUFTI" Scheurich
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
#include "stdafx.h"

#include "NodeLayout.h"
#include "Proto.h"
#include "FieldValue.h"
#include "MFString.h"
#include "MFFloat.h"
#include "MFString.h"
#include "StringArray.h"
#include "DuneApp.h"

ProtoLayout::ProtoLayout(Scene *scene)
  : Proto(scene, "Layout")
{
    StringArray *defaultalign = new StringArray();
    defaultalign->append("CENTER");
    defaultalign->append("CENTER");
    align.set(
        addExposedField(MFSTRING, "align", new MFString(defaultalign)));

    float offsetvalues[] = { 1.0f, 1.0f };
    float *o = new float[2];  
    memcpy(o, offsetvalues, 2 * sizeof(float));
    offset.set(
        addExposedField(MFFLOAT, "offset", new MFFloat(o, 2)));

    StringArray *defaultoffsetUnits = new StringArray();
    defaultoffsetUnits->append("WORLD");
    defaultoffsetUnits->append("WORLD");
    offsetUnits.set(
        addExposedField(MFSTRING, "offsetUnits", 
                        new MFString(defaultoffsetUnits)));

    StringArray *defaultscaleMode = new StringArray();
    defaultscaleMode->append("NONE");
    defaultscaleMode->append("NONE");
    scaleMode.set(
        addExposedField(MFSTRING, "scaleMode", new MFString(defaultscaleMode)));

    float sizevalues[] = { 1.0f, 1.0f };
    float *s = new float[2];  
    memcpy(s, sizevalues, 2 * sizeof(float));
    size.set(
        addExposedField(MFFLOAT, "size", new MFFloat(s, 2)));

    StringArray *defaultsizeUnits = new StringArray();
    defaultsizeUnits->append("WORLD");
    defaultsizeUnits->append("WORLD");
    sizeUnits.set(
        addExposedField(MFSTRING, "sizeUnits", new MFString(defaultsizeUnits)));
}

Node *
ProtoLayout::create(Scene *scene)
{ 
    return new NodeLayout(scene, this); 
}

NodeLayout::NodeLayout(Scene *scene, Proto *def)
  : Node(scene, def)
{
}

void
NodeLayout::modifyViewportData(float *data, 
                               bool sizeChangeAbleX, bool sizeChangeAbleY, 
                               int screenWidth, int screenHeight,
                               float *scaleX, float *scaleY)
{   
    bool sizeChangedX = false;
    bool sizePixelsX = false;
    if (sizeUnits()->getSize() > 0)
       sizePixelsX = strcmp(sizeUnits()->getValue(0), "PIXEL") == 0;
    bool sizeChangedY = false;
    bool sizePixelsY = false;
    if (sizeUnits()->getSize() > 1)
       sizePixelsY = strcmp(sizeUnits()->getValue(1), "PIXEL") == 0;

    float sizeX = 1;
    if (size()->getSize() > 0) {
        sizeX = size()->getValue(0);
        if (sizePixelsX)
            sizeX = sizeX / screenWidth;
        sizeChangedX = true;
    }

    float sizeY = 1;
    if (size()->getSize() > 1) {
        sizeY = size()->getValue(1);
        if (sizePixelsY)
            sizeY = sizeY / screenHeight;
        sizeChangedY = true;
    }

    bool offsetPixelsX = false;
    bool offsetChangedX = false;
    if (offsetUnits()->getSize() > 0)
       offsetPixelsX = strcmp(offsetUnits()->getValue(0), "PIXEL") == 0;
    bool offsetChangedY = false;
    bool offsetPixelsY = false;
    if (offsetUnits()->getSize() > 1)
       offsetPixelsY = strcmp(offsetUnits()->getValue(1), "PIXEL") == 0;

    float offsetX = 0;
    if (offset()->getSize() > 0) {
        offsetX = offset()->getValue(0);
        if (offsetPixelsX)
            offsetX = offsetX / screenWidth;
        offsetChangedX = true;
    }
    float offsetY = 0;
    if (offset()->getSize() > 1) {
        offsetY = offset()->getValue(1);
        if (offsetPixelsY)
            offsetY = offsetY / screenHeight;
        offsetChangedY = true;
    }

    if (align()->getValue(0)) {
        if (strcmp(align()->getValue(0),"LEFT") == 0) {
            offsetX = 0;
            offsetChangedX = true;
        } else if (strcmp(align()->getValue(0), "CENTER") == 0) {
            offsetX = sizeX - offsetX / 2.0f;
            offsetChangedX = true;
        } else if (strcmp(align()->getValue(0), "RIGHT") == 0) {
            offsetX = 1 - sizeX - 0.01;
            offsetChangedX = true;
        }
    }
    if (align()->getValue(1)) {
        if (strcmp(align()->getValue(1),"BOTTOM") == 0) {
            offsetY = 0;
            offsetChangedY = true;
        } else if (strcmp(align()->getValue(1), "CENTER") == 0) {
            offsetY = sizeY - offsetY / 2.0f;
            offsetChangedY = true;
        } else if (strcmp(align()->getValue(1), "TOP") == 0) {
            offsetY = 1 - sizeY - 0.01;
            offsetChangedY = true;
        }
    }

    *scaleX = 1;
    *scaleY = 1;

    if (scaleMode()->getSize() > 0) {
        if (strcmp(scaleMode()->getValue(0), "PIXEL") == 0)
            *scaleX = 2.0f / screenWidth;
        if (strcmp(scaleMode()->getValue(0), "WORLD") == 0)
            *scaleX = 2.0f;
        if (strcmp(scaleMode()->getValue(0), "FRACTION") == 0)
            *scaleX = 2.0f;
    }
    if (scaleMode()->getSize() > 1) {
        if (strcmp(scaleMode()->getValue(1), "PIXEL") == 0)
            *scaleY = 2.0f / screenHeight;
        if (strcmp(scaleMode()->getValue(1), "WORLD") == 0)
            *scaleY = 2.0f;
        if (strcmp(scaleMode()->getValue(1), "FRACTION") == 0)
            *scaleY = 2.0f;
    }

    float differentialScale = 1;
    if (sizeX != 0)
         differentialScale = sizeY/sizeX;
    if (scaleMode()->getSize() > 0)
        if (strcmp(scaleMode()->getValue(0), "STRETCH") == 0)
            *scaleX = *scaleY / differentialScale;
    if (scaleMode()->getSize() > 1)
        if (strcmp(scaleMode()->getValue(1), "STRETCH") == 0)
            *scaleY = *scaleX * differentialScale;

    if (sizeChangedX) {
        if ((sizeX < data[1]) || sizeChangeAbleX)
            if ((sizeX >= 0.0f) && (sizeX <= 1.0f))
                data[1] = sizeX;
    }
    if (sizeChangedY)  {
        if ((sizeY < data[3]) || sizeChangeAbleY)
            if ((sizeY >= 0.0f) && (sizeY <= 1.0f))
                data[3] = sizeY;
    }
    if (offsetChangedX) {
        if ((offsetX >= 0.0f) && (offsetX <= 1.0f))
            data[0] = offsetX;
    }
    if (offsetChangedY) {
        if ((offsetY >= 0.0f) && (offsetY <= 1.0f))
            data[2] = offsetY;
    }
}

