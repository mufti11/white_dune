/*
 * NodeMovieTexture.h
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

#ifndef _NODE_MOVIETEXTURE_H
#define _NODE_MOVIETEXTURE_H

#ifndef _NODE_H
#include "ExternTheApp.h"
#include "Node.h"
#endif
#ifndef _PROTO_MACROS_H
#include "ProtoMacros.h"
#endif
#ifndef _PROTO_H
#include "Proto.h"
#endif

#include "KambiTextureCommonFields.h"

#include "SFMFTypes.h"

class ProtoMovieTexture : public Proto {
public:
                    ProtoMovieTexture(Scene *scene);
    virtual Node   *create(Scene *scene);

    virtual int     getType() const { return VRML_MOVIE_TEXTURE; }
    virtual int     getNodeClass() const 
                       { return URL_NODE | 
                                TEXTURE_NODE | 
                                TEXTURE_2D_NODE | 
                                MOVIE_TEXTURE_NODE; 
                       }

    FieldIndex description;
    FieldIndex loop;
    FieldIndex pitch;
    FieldIndex pauseTime;
    FieldIndex resumeTime;
    FieldIndex speed;
    FieldIndex startTime;
    FieldIndex stopTime;
    FieldIndex url;
    FieldIndex repeatS;
    FieldIndex repeatT;
    FieldIndex blendMode;
    FieldIndex textureProperties;
    kambiTextureCommonFieldIndex()
    kambiVrmlTextureCommonFieldIndex()
    FieldIndex crossOrigin;
    FieldIndex hideChildren;
    FieldIndex origChannelCount;
    FieldIndex scale;
};

class NodeMovieTexture : public Node {
public:
                    NodeMovieTexture(Scene *scene, Proto *proto);

    virtual int     getProfile(void) const { return PROFILE_IMMERSIVE; }
    virtual int     getX3dVersion(void) const { return 0; }
    virtual Node   *copy() { return new NodeMovieTexture(m_scene, m_proto); }

    virtual bool    isInvalidChildNode(void) { return true; }

    virtual bool    hasCoverFields(void) { return true; }   

    virtual void    load();  

#ifdef HAVE_FFMPEG
    virtual void    bind();
    virtual void    unbind();

    virtual bool    isTransparent(void) { return m_isTransparent; }
    virtual int     textureGlColorMode(void) { return m_glColorMode; }
#endif

    fieldMacros(SFString, description,       ProtoMovieTexture)
    fieldMacros(SFBool,   loop,              ProtoMovieTexture)
    fieldMacros(SFFloat,  pitch,             ProtoMovieTexture)
    fieldMacros(SFTime,   pauseTime,         ProtoMovieTexture)
    fieldMacros(SFTime,   resumeTime,        ProtoMovieTexture)
    fieldMacros(SFFloat,  speed,             ProtoMovieTexture)
    fieldMacros(SFTime,   startTime,         ProtoMovieTexture)
    fieldMacros(SFTime,   stopTime,          ProtoMovieTexture)
    fieldMacros(MFString, url,               ProtoMovieTexture)
    fieldMacros(SFBool,   repeatS,           ProtoMovieTexture)
    fieldMacros(SFBool,   repeatT,           ProtoMovieTexture)
    fieldMacros(SFInt32,  blendMode,         ProtoMovieTexture)
    fieldMacros(SFNode,   textureProperties, ProtoMovieTexture);
    kambiTextureCommonFieldMacros(ProtoMovieTexture)
    kambiVrmlTextureCommonFieldMacros(ProtoMovieTexture)
    fieldMacros(SFString, crossOrigin,       ProtoMovieTexture)
    fieldMacros(SFBool,   hideChildren,      ProtoMovieTexture)
    fieldMacros(SFInt32,  origChannelCount,  ProtoMovieTexture)
    fieldMacros(SFBool,   scale,             ProtoMovieTexture)

protected:
    bool m_loaded;
    double m_startTime;
    unsigned int m_textureName;
    bool m_isTransparent;
    int m_glColorMode;

public:
     int m_duration_changed;
     void *m_fw_movie;
     int m_loadstatus;
     float m_speed;
     int m_sourceNumber;
};

#endif // _NODE_MOVIETEXTURE_H
