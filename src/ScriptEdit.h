/*
 * ScriptEdit.h
 *
 * Copyright (C) 2003, 2006 J. "MUFTI" Scheurich
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

#ifndef _ECMA_SCRIPT_EDIT_H
#define _ECMA_SCRIPT_EDIT_H

#include "Scene.h"
#include "Array.h"
#include "swt.h"
#include "NodeScript.h"
#include "NodeImageTexture.h"
#include "NodePixelTexture.h"
#include "NodeMovieTexture.h"
#include "NodeAudioClip.h"

typedef void  (*EditorReadyCallback)(void *data);

class ObjectEdit {
public:
                        ObjectEdit(Node* node, SWND wnd, 
                                   EditorReadyCallback editorReadyCallback,
                                   void *data);
    virtual            ~ObjectEdit();

    bool                edit(bool newFile = true, bool is4Kids = false);
    void                OnTimer(void);    
    Node               *getNode(void) { return m_node; }

protected:
    virtual bool        readEditorfile(void) { return true; }    
    bool                checkEditorData(void);
    bool                readQuotedEditorFile(char *fileName, 
                                             Node *node, int field);

    virtual void        generateFilename(bool secondTry) = 0;
    virtual bool        needOpen(void) { return false; }
    virtual bool        writeFile(int f) { return true; }
    virtual void        generateCommand(bool is4Kids) = 0;

    bool                write2file(int f, const void *data, size_t count);
    bool                write2file(int f, const char* string) 
                           { 
                           return ObjectEdit::write2file(f, string, 
                                                         strlen(string));
                           }

    Node               *m_node;
    SWND                m_wnd;
    STIMER              m_timer;
    char                m_editorFile[1024];
    char                m_command[1024+1024+2];
    void               *m_data;
    EditorReadyCallback m_editorReadyCallback;
    Array<int>          m_urlStartData;
    Array<int>          m_urlEndData;
    char               *m_urlData;
    int                 m_urlDataLength;
    bool                m_popUp;
};

class ScriptEdit : public ObjectEdit {
public:
                        ScriptEdit(NodeScript* node, SWND wnd, 
                                   EditorReadyCallback editorReadyCallback,
                                   void *data);
    virtual            ~ScriptEdit();
    virtual bool        readEditorfile(void);

protected:

    virtual void        generateFilename(bool secondTry);
    virtual bool        needOpen(void) { return true; }
    virtual bool        writeFile(int f);
    virtual void        generateCommand(bool is4Kids);

    bool                writeSFStringUrl(int f, const char* string);
    bool                writeSFStringX3domUrl(int f, const char* string);

    char               *ecmaScriptEdit(void);
    bool                ecmaScriptReadEditorfile(char* filename = NULL);

private:

    NodeScript         *m_scriptNode;
};

class ShaderEdit : public ObjectEdit {
public:
                        ShaderEdit(Node* node, SWND wnd, 
                                   EditorReadyCallback 
                                   editorReadyCallback,
                                   void *data);
    virtual            ~ShaderEdit();
    virtual bool        readEditorfile(void);

protected:

    virtual void        generateFilename(bool secondTry);
    virtual bool        needOpen(void) { return true; }
    virtual bool        writeFile(int f);
    virtual void        generateCommand(bool is4Kids);
    bool                shaderReadEditorfile(char* filename = NULL);

private:

    Node               *m_shaderNode;
};

class ImageTextureEdit : public ObjectEdit {
public:
                        ImageTextureEdit(NodeImageTexture* node, SWND wnd, 
                                         EditorReadyCallback 
                                         editorReadyCallback,
                                         void *data);
    virtual            ~ImageTextureEdit();

protected:

    virtual void        generateFilename(bool secondTry);
    virtual void        generateCommand(bool is4Kids);

private:

    NodeImageTexture   *m_imageTextureNode;
};

class PixelTextureEdit : public ObjectEdit {
public:
                        PixelTextureEdit(NodeImageTexture* node, SWND wnd, 
                                         EditorReadyCallback 
                                         editorReadyCallback,
                                         void *data) :
                        ObjectEdit(node, wnd, editorReadyCallback, data) {}

protected:

    virtual void        generateFilename(bool secondTry) {}
    virtual void        generateCommand(bool is4Kids) {}

private:

    NodePixelTexture   *m_pixelTextureNode;
};

class MovieTextureEdit : public ObjectEdit {
public:
                        MovieTextureEdit(NodeMovieTexture* node, SWND wnd, 
                                         EditorReadyCallback 
                                         editorReadyCallback,
                                         void *data) :
                        ObjectEdit(node, wnd, editorReadyCallback, data)
                           { m_movieTextureNode = node; }
     virtual            ~MovieTextureEdit();

protected:

    virtual void        generateFilename(bool secondTry);
    virtual void        generateCommand(bool is4Kids);

private:

    NodeMovieTexture   *m_movieTextureNode;
};

class AudioClipEdit : public ObjectEdit {
public:
                        AudioClipEdit(NodeAudioClip* node, SWND wnd, 
                                      EditorReadyCallback editorReadyCallback,
                                      void *data) :
                        ObjectEdit(node, wnd, editorReadyCallback, data) 
                           { m_audioClipNode = node; }
    virtual            ~AudioClipEdit() { delete  m_audioClipNode; }
protected:

    virtual void        generateFilename(bool secondTry);
    virtual void        generateCommand(bool is4Kids);

private:

    NodeAudioClip   *m_audioClipNode;
};


#endif
