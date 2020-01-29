/*
 * NodeVrmlCut.h
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

#include "Node.h"
#include "ProtoMacros.h"
#include "Proto.h"
#include "SFMFTypes.h"
#include "CommandList.h"
#include "DynamicFieldsNode.h"

#define BEGIN_VRML_CUT_SCRIPT "BEGIN VrmlCut Script"
#define END_VRML_CUT_SCRIPT   "END VrmlCut Script"

class ProtoVrmlCut : public DynamicFieldsProto {
public:
                    ProtoVrmlCut(Scene *scene);
    virtual Node   *create(Scene *scene);

    virtual int     getType() const { return DUNE_VRML_CUT; }

    FieldIndex sceneLengths;
    FieldIndex sceneDelays;
    FieldIndex sceneNumber;
    FieldIndex scenes;
    FieldIndex numberPreviousScenes;
    FieldIndex numberNextScenes;

    virtual bool    isScriptedProto(void) { return true; }

    int getFirstDynamicEventOut() { return m_firstDynamicEventOut; }
protected:
    int m_firstDynamicEventOut;
};

class NodeVrmlCut : public DynamicFieldsNode {
public:
                    NodeVrmlCut(Scene *scene, Proto* proto);
                    NodeVrmlCut(Node *node, Proto* proto) : 
                               DynamicFieldsNode(node, proto) 
                       { initialize(); }

    void            initialize(void);

    virtual int     getProfile(void) const { return PROFILE_IMMERSIVE; }
    virtual int     getX3dVersion(void) const { return -1; }    
    virtual Node   *copy() const { return new NodeVrmlCut(*this); }

    virtual bool    hasNumbers4kids(void) { return true; } 

    virtual int     write(int filedes, int indent, bool avoidUse = false) 
                       { 
                       writeProto(filedes);
                       return DynamicFieldsNode::write(filedes, indent, 
                                                       avoidUse); 
                       }

    virtual int     writeXml(int filedes, int indent, 
                             int containerField = -1, bool avoidUse = false);

    int             writeX3domScript(int filedes, int indent);

    virtual int     writeRib(int filedes, int indent);
    virtual int     writeProto(int filedes);
    virtual int     writeXmlProto(int filedes);
    virtual int     writeJavaScript(int filedes);

    virtual void    updateTime(void);

    virtual void    preDraw();
    virtual void    draw(int pass);

    virtual void    flip(int index) { scenes()->flip(index); }    
    virtual void    swap(int fromTo) { scenes()->swap(fromTo); }

    virtual int     countPolygons(void)
                       { return scenes()->countPolygons(); } 
    virtual int     countPrimitives(void) 
                       { return scenes()->countPrimitives(); } 

    virtual int     countPolygons1Sided(void)
                       { return scenes()->countPolygons1Sided(); } 
    virtual int     countPolygons2Sided(void)
                       { return scenes()->countPolygons2Sided(); } 

    virtual bool    showFields() { return true; }

    virtual void    setField(int index, FieldValue *value, int cf = -1);

    virtual void    update();
    void            updateCycleInterval(Node *vrmlScene, SFTime *interval);
    void            updateSelection(void);

    bool            getEventOutsInitialised(void)
                        { return m_eventOutsInitialised; }
    void            setEventOutsInitialised(void)
                        { m_eventOutsInitialised = true; }

    fieldMacros(MFTime,  sceneLengths,         ProtoVrmlCut)
    fieldMacros(MFTime,  sceneDelays,          ProtoVrmlCut)
    fieldMacros(SFInt32, sceneNumber,          ProtoVrmlCut)
    fieldMacros(MFNode,  scenes,               ProtoVrmlCut)
    fieldMacros(SFInt32, numberPreviousScenes, ProtoVrmlCut)
    fieldMacros(SFInt32, numberNextScenes,     ProtoVrmlCut)

protected:
    virtual        ~NodeVrmlCut();

    char           *newStartTimeName(void);
    void            startNextScene(int currentScene, double time);
    int             accountWhich(void);
    int             getVrmlSceneWhich(void);

    void            accountAllSceneLengths();

protected:
    int            m_firstScene;
    int            m_lastScene;
    int            m_currentScene;
    int            m_selectionWhich;
    double         m_allSceneLengths;
    bool           m_eventOutsInitialised;
};

