/*
 * ScriptEdit.cpp
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

#include "ScriptEdit.h"
#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

#include "SFMFTypes.h"
#include "Field.h"
#include "EventIn.h"
#include "EventOut.h"
#include "ExposedField.h"
#include "Element.h"
#include "DuneApp.h"
#include "resource.h"

#include "NodePackagedShader.h"
#include "NodeShaderProgram.h"
#include "NodeShaderPart.h"
#include "NodeText.h"
#include "NodeText3D.h"
#include "NodeWorldInfo.h"

enum { blankOrTabMode, someTextMode };

ObjectEdit::ObjectEdit(Node *node, SWND wnd, 
                       EditorReadyCallback editorReadyCallback, void *data)
{
    m_node = node;
    m_timer = NULL;
    m_wnd = wnd;
    m_editorReadyCallback = editorReadyCallback;
    m_data = data;
#ifdef _WIN32
    m_popUp = true;
#else
    m_popUp = false;
#endif
}

ObjectEdit::~ObjectEdit()
{
    swKillTimer(m_timer);
}

static int
editorTimerCallback(void *data)
{
    ObjectEdit *objectEdit = (ObjectEdit *) data;
    objectEdit->OnTimer();
    return 0;
}

void
ObjectEdit::OnTimer(void)
{
    if (m_timer) {
        swKillTimer(m_timer);
        m_timer = NULL;
        if (swCheckRunningProcess()) 
            m_timer = swSetTimer(m_wnd, 500, editorTimerCallback, this);
        else {
            if (readEditorfile())
                m_editorReadyCallback(m_data);
        } 
    }
}

bool ObjectEdit::write2file(int f, const void *data, long count)
{
    if (mywrite(f, data, count) < 0) {
        TheApp->MessageBoxPerror(m_editorFile);
        return false;
    }
    return true;    
}

bool
ObjectEdit::edit(bool newFile, bool is4Kids)
{
    if (newFile) {
        if (needOpen()) {
            int f = -1;
            for (int i = 1; i <= 2; i++) {
                generateFilename(i == 2);
                f = open(m_editorFile, O_WRONLY | O_CREAT,00666);   
                if (f != -1)
                    break;
                if (i == 2) {             
                    char format[256];
                    swLoadString(IDS_ERROR_SAVE_FILE_TO_EDIT, format, 255);
                    char msg[1024];
                    mysnprintf(msg, 1023, format, m_editorFile, strerror(errno));
                    TheApp->MessageBox(msg);
                    return false;
                }
            }
            TheApp->AddToFilesToDelete(m_editorFile);
            if (!writeFile(f))
                return false;
            if (swTruncateClose(f))
                return false;
        } else
            generateFilename(false);
    }

    generateCommand(is4Kids);
    if (m_popUp) {
        if (system(m_command) != 0)
            return false;
        readEditorfile();
        m_editorReadyCallback(m_data);
    } else {
        m_timer = swSetTimer(m_wnd, 500, editorTimerCallback, this);
        if (swCreateCheckableProcess(m_command) != 0) {
            MyString object = "";
            object += m_command;
            object += " ";
            object += m_editorFile;
            TheApp->MessageBoxPerror(object);
            return false;
        }
    }
    return true;
}


ScriptEdit::ScriptEdit(NodeScript *node, SWND wnd, 
                       EditorReadyCallback editorReadyCallback, void *data) :
ObjectEdit(node, wnd, editorReadyCallback, data)
{
    m_scriptNode = node;
    m_urlData = NULL;
}

ScriptEdit::~ScriptEdit()
{
    delete [] m_urlData;
}

void
ScriptEdit::generateFilename(bool secondTry)
{
    TheApp->checkAndRepairTextEditCommand();

    int texteditUseExtensionTxt;

    swTextEditGetSettingsUseExtensionTxt(TheApp->GetTextedit(),
                                         &texteditUseExtensionTxt);

    if (texteditUseExtensionTxt) {
        if (secondTry) 
            swGetTempPath(m_editorFile,".dune_ecmascript",".txt",1024);
        else
            swGetTempFile(m_editorFile,".dune_ecmascript", ".txt", 1024);
    } else {
        if (secondTry) 
            swGetTempPath(m_editorFile,".dune_ecmascript",".js",1024);
        else
            swGetTempFile(m_editorFile,".dune_ecmascript", ".js", 1024);
    }
}

bool
ScriptEdit::writeFile(int f)
{
    bool writeError = false;
    MFString *url = (MFString *)m_scriptNode->url();
    bool hasUrl = true;
    if (url == NULL)
        hasUrl = false;
    if (url->getSize() == 0)
        hasUrl = false;
    const char* string = "";           
    if (hasUrl) {
        for (int i = 0; i < url->getSize(); i++) {
            string = url->getValue(i);
            if (strlen(string) > 0) {
                if (!writeSFStringUrl(f, string))
                    writeError = true;
                if (!writeSFStringUrl(f, "\n"))
                    writeError = true;
            }
        }
    } else {
        if (!writeSFStringUrl(f, ""))
            writeError = true;
        if (TheApp->getX3domMode() && (strlen(string) == 0))
            if (!writeSFStringX3domUrl(f, ""))
                writeError = true;
    }
    if (writeError) {
        TheApp->MessageBoxPerror(m_editorFile);
        return false;
    }
         
    return true;
}    

void
ScriptEdit::generateCommand(bool is4Kids)
{
    TheApp->checkAndRepairTextEditCommand();
    mysnprintf(m_command, sizeof(m_command), "%s %s", 
               swTextEditGetCommand(TheApp->GetTextedit()), m_editorFile);
    
}

bool   
ScriptEdit::readEditorfile(void)
{
    return ecmaScriptReadEditorfile(m_editorFile);     
}

// add javascript scheme if necessary and write to file to be edited

bool
ScriptEdit::writeSFStringUrl(int f, const char* string)  
{
     bool hasJavascript = false;
 
     Proto* proto = m_scriptNode->getProto();
     if (proto == NULL)
         return false;
 
    if (strlen(string) == 0) {
        if (m_scriptNode->getScene()->isX3d()) {
            if (!write2file(f, "\"ecmascript:\n\n"))
                 return false;        
        } else {
            if (!write2file(f, "\"javascript:\n\n"))
                 return false;        
        }
        if (!write2file(f, "// insert program code only into functions\n\n"))
            return false;        
        hasJavascript = true;
    } else if (isSortOfEcmascript(string)) {
        if (isJavascript(string)) {
            if (!write2file(f, "\"javascript:\n\n"))
                return false;        
            string += strlen("\"javascript:");
        } else if (isEcmascript(string)) {
            if (!write2file(f, "\"ecmascript:\n\n"))
                return false;        
            string += strlen("\"ecmascript:");
        }
        hasJavascript = true;
        if (string[0] == '\n')
            string++;
        if (string[0] == '\n')
            string++;
    }
    bool addComment = true;
    MyString javascript = "";
    InterfaceArray *interfaceData = m_scriptNode->getInterfaceData();
    for (long j = 0; j < interfaceData->size(); j++) {
        int ind = interfaceData->get(j)->m_elementIndex;
        MyString text = "";
        MyString cmptext = "";
        MyString extratext = "";  
        switch (interfaceData->get(j)->m_elementEnum) {
          case EL_FIELD:
            {
              MyString name = proto->getField(ind)->getName(false); 
              int type = proto->getField(ind)->getType();
              text += " // field ";
              text += typeEnumToString(type);
              text += " ";
              text += name;
              text += " //";
              cmptext = text;
              extratext += "\n";
              if (addComment) {
                  extratext += typeDefaultValue(type)->
                               getEcmaScriptComment(name, EL_FIELD);
                  extratext += "\n";
              }
            }
            break;
          case EL_EVENT_OUT:
            {
              MyString name = proto->getEventOut(ind)->getName(false);
              int type = proto->getEventOut(ind)->getType();
              text += " // eventOut ";
              text += typeEnumToString(type);
              text += " ";
              text += name;
              text += " //";
              cmptext = text;
              extratext += "\n";
              if (addComment) {
                  extratext += typeDefaultValue(type)->
                               getEcmaScriptComment(name, EL_EVENT_OUT);
                  extratext += "\n";
              }
            }
            break;
          case EL_EVENT_IN:
            {
              MyString name = proto->getEventIn(ind)->getName(false);
              int type = proto->getEventIn(ind)->getType();
              text += " function ";
              text += name;
              cmptext = "";
              cmptext += text;
              text += "(";
              cmptext += " (";
              extratext = "value)\n    {";
              extratext += "\n    // value  "; 
              extratext += typeEnumToString(type);
              extratext += "\n";
              if (addComment) {
                  extratext += typeDefaultValue(type)->
                               getEcmaScriptComment("value", EL_EVENT_IN);
                  extratext += "\n";
              }
              extratext += "    }\n\n";
            }
            break;
          case EL_EXPOSED_FIELD:
            {
              MyString name = proto->getExposedField(ind)->getName(false);
              int type = proto->getExposedField(ind)->getType();
              text += " function ";
              text += name;
              cmptext = "";
              cmptext += text;
              text += "(";
              cmptext += " (";
              extratext = "value)\n    {";
              extratext += "\n    // value  "; 
              extratext += typeEnumToString(type);
              extratext += "\n";
              if (addComment) {
                  extratext += typeDefaultValue(type)->
                               getEcmaScriptComment("value", EL_EVENT_IN);
                  extratext += "\n";
              }
              extratext += "    }\n\n";
            }
            break;
        }
        // if not already in, attach to javascript code
        if (m_scriptNode->strwhitespacestr(string, cmptext) == false) {
            javascript += text;
            javascript += extratext;
        }
    }  
    if (m_scriptNode->getWantInitialize()) {
        m_scriptNode->wantInitialize(false);
        MyString text = " function initialize(";
        MyString extratext = ")\n    {\n    \n    }\n\n";
        // if not already in, attach to javascript code
        if (m_scriptNode->hasInitialize() == false) {
            javascript += text;
            javascript += extratext;
        }
    }
    if (m_scriptNode->getWantEventsProcessed()) {
        m_scriptNode->wantEventsProcessed(false);
        MyString text = " function eventsProcessed(";
        MyString extratext = ")\n    {\n    \n    }\n\n";
        // if not already in, attach to javascript code
        if (m_scriptNode->hasEventsProcessed() == false) {
            javascript += text;
            javascript += extratext;
        }
    }
    if (m_scriptNode->getWantShutdown()) {
        m_scriptNode->wantShutdown(false);
        MyString text = " function shutdown(";
        MyString extratext = ")\n    {\n    \n    }\n\n";
        // if not already in, attach to javascript code
        if (m_scriptNode->hasShutdown() == false) {
            javascript += text;
            javascript += extratext;
        }
    }
    if (!write2file(f, string))
        return false;
    return true;
}

// add javascript scheme if necessary and write to file to be edited

bool
ScriptEdit::writeSFStringX3domUrl(int f, const char* string)  
{
     bool hasJavascript = false;
 
     Proto* proto = m_scriptNode->getProto();
     if (proto == NULL)
         return false;
 
    if (!write2file(f, ",\n\""))
        return false;
    if (strlen(string) == 0) {
        if (!write2file(f, "x3domscript:\n\n"))
            return false;        
        if (!write2file(f, "// insert program code only into functions\n\n"))
            return false;        
        hasJavascript = true;
    }
    bool addComment = true;
    MyString javascript = "";
    InterfaceArray *interfaceData = m_scriptNode->getInterfaceData();
    for (long j = 0; j < interfaceData->size(); j++) {
        int ind = interfaceData->get(j)->m_elementIndex;
        MyString text = "";
        MyString cmptext = "";
        MyString extratext = "";  
        switch (interfaceData->get(j)->m_elementEnum) {
          case EL_EVENT_IN:
            {
              MyString name = proto->getEventIn(ind)->getName(false);
              text += " function ";
              text += name;
              cmptext = "";
              cmptext += text;
              text += "(";
              cmptext += " (";
              extratext = "event)\n    {";
              extratext += "\n    var value = event.value;\n";
              if (addComment) {
                  extratext += "//    document.getElementById('DEF')";
                  extratext += ".setAttribute('FIELD', value);"; 
                  extratext += "\n";
              }
              extratext += "    }\n\n";
            }
            break;
        }
        // if not already in, attach to javascript code
        if (m_scriptNode->strwhitespacestr(string, cmptext) == false) {
            javascript += text;
            javascript += extratext;
        }
    }  
    if (hasJavascript)
        if (!write2file(f, javascript))
            return false;        
    if (!write2file(f, string))
        return false;
    if (!write2file(f, "\""))
        return false;
    return true;
}

bool
ScriptEdit::ecmaScriptReadEditorfile(char *fileName)
{
    return ObjectEdit::readEditorFile(fileName, m_scriptNode, 
                                      m_scriptNode->url_Field());
    return readQuotedEditorFile(fileName, m_scriptNode,
                                m_scriptNode->url_Field());
}


TextEdit::TextEdit(Node *node, SWND wnd, 
                   EditorReadyCallback editorReadyCallback, void *data) :
ObjectEdit(node, wnd, editorReadyCallback, data)
{
    m_node = node;
    m_urlData = NULL;
}

TextEdit::~TextEdit()
{
    delete [] m_urlData;
}

void
TextEdit::generateFilename(bool secondTry)
{
    TheApp->checkAndRepairTextEditCommand();

    int texteditUseExtensionTxt;

    swTextEditGetSettingsUseExtensionTxt(TheApp->GetTextedit(),
                                         &texteditUseExtensionTxt);

    if (texteditUseExtensionTxt) {
        if (secondTry) 
            swGetTempPath(m_editorFile,".dune_ecmascript",".txt",1024);
        else
            swGetTempFile(m_editorFile,".dune_ecmascript", ".txt", 1024);
    } else {
        if (secondTry) 
            swGetTempPath(m_editorFile,".dune_ecmascript",".js",1024);
        else
            swGetTempFile(m_editorFile,".dune_ecmascript", ".js", 1024);
    }
}

bool
TextEdit::writeFile(int f)
{
    bool writeError = false;
    MFString *text = NULL;

    switch (m_node->getType()) {
      case VRML_TEXT:
        {
        NodeText *textNode = (NodeText *)m_node;
        text = textNode->string();
        }
        break;
      case KAMBI_TEXT_3D:
        {
        NodeText3D *textNode = (NodeText3D *)m_node;
        text = textNode->string();
        }
        break;
      case VRML_WORLD_INFO:
        {
        NodeWorldInfo *worldNode = (NodeWorldInfo *)m_node;
        text = worldNode->info();
        }
        break;
    }    

    bool hasText = true;
    if (text == NULL)
        hasText = false;
    if (text->getSize() == 0)
        hasText = false;
    const char* string = "";           
    if (hasText) {
        for (int i = 0; i < text->getSize(); i++) {
            string = text->getValue(i);
            if (!writeSFStringText(f, string))
                writeError = true;
            if (i < text->getSize()-1)
                if (!write2file(f, "\n")) 
                    writeError = true;
        }
    } else {
        if (!writeSFStringText(f, ""))
            writeError = true;
    }
    if (writeError) {
        TheApp->MessageBoxPerror(m_editorFile);
        return false;
    }
         
    return true;
}    

void
TextEdit::generateCommand(bool is4Kids)
{
    TheApp->checkAndRepairTextEditCommand();
    mysnprintf(m_command, sizeof(m_command), "%s %s", 
               swTextEditGetCommand(TheApp->GetTextedit()), m_editorFile);
    
}

bool   
TextEdit::readEditorfile(void)
{
    return textReadEditorfile(m_editorFile);     
}

// write to file to be edited

bool
TextEdit::writeSFStringText(int f, const char* string)  
{
     bool hasJavascript = false;
 
     Proto* proto = m_node->getProto();
     if (proto == NULL)
         return false;
 
    if (!write2file(f, string))
        return false;
    return true;
}

bool
TextEdit::textReadEditorfile(char *fileName)
{
    int field = -1;

    switch (m_node->getType()) {
      case VRML_TEXT:
        {
        NodeText *text = (NodeText *)m_node;
        field = text->string_Field();
        }
        break;
      case KAMBI_TEXT_3D:
        {
        NodeText3D *text = (NodeText3D *)m_node;
        field = text->string_Field();
        }
        break;
      case VRML_WORLD_INFO:
        {
        NodeWorldInfo *worldNode = (NodeWorldInfo *)m_node;
        field = worldNode->info_Field();
        }
        break;
    }    
    if (field != -1)
        return readEditorFile(fileName, m_node, field);
    return false;
}

bool
ObjectEdit::readQuotedEditorFile(char *fileName, Node *node, int field)
{
    char *file = fileName; 
    if (file == NULL)
        file = m_editorFile;
    int f = open(file, O_RDONLY, 00666);   
    if (f == -1) {
        TheApp->MessageBoxPerror(IDS_ERROR_READ_SCRIPT_DATA, file);
        return true;
    } 
    lseek(f, 0, SEEK_SET);
    m_urlDataLength = lseek(f, 0, SEEK_END);
    lseek(f,0,SEEK_SET);
    m_urlData = new char[m_urlDataLength + 1];
    int offset = 0;
    do {
        int bytes = read(f, m_urlData + offset, m_urlDataLength - offset);
#ifdef _WIN32
        // M$Windows do a CR-LF translation, number of read byte is reduced
        // under M$Windows bytes == 0 is EOF
        if (bytes == 0)
            m_urlDataLength = offset;
#endif
        if (bytes < 0) {
            TheApp->MessageBox(IDS_PREVIOUS_EOF, m_editorFile);
            return true;
        } 
        offset += bytes;
    } while (offset < m_urlDataLength);  

    MFString* newUrl = new MFString();
    if (m_urlStartData.size() != m_urlEndData.size()) {
        TheApp->MessageBox(IDS_MISSING_QUOTE_PAIR, m_editorFile);
        edit(false);
        return false;
    } 
    for (long i = 0 ; i < m_urlStartData.size(); i++) {
        m_urlData[m_urlEndData[i]] = '\0';
        MyString urlString = "";
        urlString += (&m_urlData[m_urlStartData[i]+1]);
        urlString += "\n";
        newUrl->setSFValue(i, new SFString(urlString));
    }       
    node->setField(field, newUrl);     
    return true;
}

// check for valid " qouting in data from editor
// valid is 
// 
//     blank_or_tab " some_text " 
// (multiple) continued with
//     blank_or_tab , blank_or_tab " some_text " 
//
// blank_or_tab and some_text may be empty

// \" is not counted as " 
static bool isDoubleQuoute(char* data, int offset)
{ 
    if (offset == 0)
       return data[offset] == '"';
    else
       return (data[offset] == '"') && (data[offset-1] != '\\');
}

static bool isBlankOrTab(char* data, int offset)
{
    return ((data[offset] == ' ') || (data[offset] == '\t'));
}

static bool isLineFeed(char* data, int offset)
{
    return (data[offset] == '\n') || (data[offset] == '\r');
}

static bool is2CharLineFeed(char* data, int offset)
{
  if ( ((data[offset] == '\n') && (data[offset+1] == '\r')) || 
       ((data[offset] == '\r') && (data[offset+1] == '\n')) )
      return true;
  else
      return false;
}

// additionly to check for this tokens, ecmaScriptCheckEditorData
// also checks for beginning and end of the Script url field(s) (type MFString)

bool
ObjectEdit::checkEditorData(void)
{
    int mode = blankOrTabMode;
    int lineCount = 1;
    int charsPerLine = 0;
    bool beforeFirstDoubleQuoute = true;
    m_urlStartData.resize(0);    
    m_urlEndData.resize(0);    
    for (int offset = 0; offset < m_urlDataLength; offset++) {
        charsPerLine++;           
        if (isLineFeed(m_urlData,offset)) {
            if (is2CharLineFeed(m_urlData, offset))
                offset++;
            lineCount++;
            charsPerLine = 0;
        } else if (mode == blankOrTabMode) {
            bool invalidChar = false;
            if (!isBlankOrTab(m_urlData,offset)) {
                if (m_urlData[offset] != ',') { 
                    if (isDoubleQuoute(m_urlData,offset)) {
                        mode = someTextMode;
                        m_urlStartData.append(offset);
                        }
                }
            }
            if (invalidChar) {        
                char format[256];
                swLoadString(IDS_MISSING_CLOSING_QUOTE, format, 255);
                char msg[1024];
                mysnprintf(msg, 1023, format, lineCount, charsPerLine);
                TheApp->MessageBox(msg);
                return false;
            }        
        } else 
            if (isDoubleQuoute(m_urlData,offset)) {
                mode = blankOrTabMode;
                beforeFirstDoubleQuoute = false;
                m_urlEndData.append(offset);
            }
    }
    if (mode == someTextMode) {
        char format[256];
        swLoadString(IDS_MISSING_CLOSING_QUOTE, format, 255);
        char msg[1024];
        mysnprintf(msg, 1023, format, lineCount, charsPerLine, m_editorFile);
        TheApp->MessageBox(msg);
        return false;
    } 
    return true;   
}


bool
ObjectEdit::readEditorFile(char *fileName, Node *node, int field)
{
    char *file = fileName; 
    if (file == NULL)
        file = m_editorFile;
    int f = open(file, O_RDONLY, 00666);   
    if (f == -1) {
        TheApp->MessageBoxPerror(IDS_ERROR_READ_TEXT_DATA, file);
        return true;
    } 
    lseek(f, 0, SEEK_SET);
    m_urlDataLength = lseek(f, 0, SEEK_END);
    lseek(f,0,SEEK_SET);
    m_urlData = new char[m_urlDataLength + 1];
    int offset = 0;
    do {
        int bytes = read(f, m_urlData + offset, m_urlDataLength - offset);
#ifdef _WIN32
        // M$Windows do a CR-LF translation, number of read byte is reduced
        // under M$Windows bytes == 0 is EOF
        if (bytes == 0)
            m_urlDataLength = offset;
#endif
        if (bytes < 0) {
            TheApp->MessageBox(IDS_PREVIOUS_EOF, m_editorFile);
            return true;
        } 
        offset += bytes;
    } while (offset < m_urlDataLength);  

    int mode = blankOrTabMode;
    int lineCount = 1;
    bool beforeFirstDoubleQuoute = true;
    m_urlStartData.resize(0);    
    m_urlEndData.resize(0);    
    m_urlStartData.append(0);
    for (int offset = 0; offset < m_urlDataLength; offset++)
        if (isLineFeed(m_urlData,offset)) {
            m_urlStartData.append(offset + 1);
            m_urlEndData.append(offset);
        }
    m_urlEndData.append(m_urlDataLength);

    MFString* newUrl = new MFString();
    for (long i = 0 ; i < m_urlStartData.size(); i++) {
        m_urlData[m_urlEndData[i]] = '\0';
        MyString urlString = "";
        urlString += (&m_urlData[m_urlStartData[i]]);
        newUrl->setSFValue(i, new SFString(urlString));
    }       
    node->setField(field, newUrl);     
    return true;
}

ShaderEdit::ShaderEdit(Node *node, SWND wnd, 
                       EditorReadyCallback editorReadyCallback, void *data) :
ObjectEdit(node, wnd, editorReadyCallback, data)
{
    m_node = node;
}

ShaderEdit::~ShaderEdit()
{
    delete m_node;
}

void
ShaderEdit::generateFilename(bool secondTry)
{
    m_editorFile[0] = (char) 0;

    if (secondTry) 
        swGetTempPath(m_editorFile,".dune_shaderprogram",".txt",1024);
    else
        swGetTempFile(m_editorFile,".dune_shaderprogram", ".txt", 1024);
}

void
ShaderEdit::generateCommand(bool is4Kids)
{
   TheApp->checkAndRepairTextEditCommand();
   mysnprintf(m_command, sizeof(m_command), "%s %s", 
              swTextEditGetCommand(TheApp->GetTextedit()), m_editorFile);
}

bool
ShaderEdit::writeFile(int f)
{
    bool writeError = false;
    MFString *url = NULL;
    switch(m_node->getType()) {
      case X3D_PACKAGED_SHADER:
        url = ((NodePackagedShader *)m_node)->url();
        break;
      case X3D_SHADER_PROGRAM:
        url = ((NodeShaderProgram *)m_node)->url();
        break;
      case X3D_SHADER_PART:
        url = ((NodeShaderPart *)m_node)->url();
        break;
    }

    bool hasUrl = true;
    if (url == NULL)
        hasUrl = false;
    if (url->getSize() == 0)
        hasUrl = false;
    if (hasUrl) {
        for (int i = 0; i < url->getSize(); i++) {
            if (!write2file(f, "\""))
                writeError = true;
            char* string= (char*) ((const char*) url->getValue(i));
            if (!write2file(f, string))
               writeError = true;
            if (!write2file(f, "\""))
                writeError = true;
            if (i < url->getSize()-1)
               if (!write2file(f, ",\n")) 
                  writeError = true;
        }
    } else
        if (!write2file(f, "\"\""))
    if (writeError) {
        TheApp->MessageBoxPerror(m_editorFile);
        return false;
    }
         
    return true;
}    

bool
ShaderEdit::shaderReadEditorfile(char *fileName)
{    
    int field = -1;
    switch (m_node->getType()) {
      case X3D_PACKAGED_SHADER:
        field = ((NodePackagedShader *)m_node)->url_Field();
        break;
      case X3D_SHADER_PROGRAM:
        field = ((NodeShaderProgram *)m_node)->url_Field();
        break;
      case X3D_SHADER_PART:
        field = ((NodeShaderPart *)m_node)->url_Field();
        break;
    }
    if (field != -1)
        return readQuotedEditorFile(fileName, m_node, field);
    return false;
}

bool   
ShaderEdit::readEditorfile(void)
{
    return shaderReadEditorfile(m_editorFile);     
}

ImageTextureEdit::ImageTextureEdit(NodeImageTexture *node, SWND wnd, 
                       EditorReadyCallback editorReadyCallback, void *data) :
ObjectEdit(node, wnd, editorReadyCallback, data)
{
    m_imageTextureNode = node;
}

ImageTextureEdit::~ImageTextureEdit()
{
}

void
ImageTextureEdit::generateFilename(bool secondTry)
{
    m_editorFile[0] = (char) 0;
    MyString path = "";

    MFString *urls = m_imageTextureNode->url();
    for (int i = 0; i < urls->getSize(); i++) {
        URL url(m_imageTextureNode->getScene()->getURL(), urls->getValue(i));
        if (urls->getValue(i).length() == 0) continue;
        m_imageTextureNode->getScene()->Download(url, &path);
        break;
    }
    if (strlen(path) < sizeof(m_editorFile))
        strcpy(m_editorFile, path);
}

void
ImageTextureEdit::generateCommand(bool is4Kids)
{
    if (is4Kids) {
        TheApp->checkAndRepairImageEdit4KidsCommand();
        mysnprintf(m_command, sizeof(m_command), "%s %s", 
                   swImageEdit4KidsGetSettings(TheApp->GetTextedit()), 
                                               m_editorFile);
    } else {
        TheApp->checkAndRepairImageEditCommand();
        mysnprintf(m_command, sizeof(m_command), "%s %s", 
                   swImageEditGetSettings(TheApp->GetTextedit()), m_editorFile);
    }
    
}

MovieTextureEdit::~MovieTextureEdit()
{
    delete m_movieTextureNode;
}

void
MovieTextureEdit::generateFilename(bool secondTry)
{
    m_editorFile[0] = (char) 0;
    MyString path = "";

    MFString *urls = m_movieTextureNode->url();
    for (int i = 0; i < urls->getSize(); i++) {
        URL url(m_movieTextureNode->getScene()->getURL(), urls->getValue(i));
        if (urls->getValue(i).length() == 0) continue;
        m_movieTextureNode->getScene()->Download(url, &path);
        break;
    }
    if (strlen(path) < sizeof(m_editorFile))
        strcpy(m_editorFile, path);
}

void
MovieTextureEdit::generateCommand(bool is4Kids)
{
    TheApp->checkAndRepairMovieEditCommand();
    mysnprintf(m_command, sizeof(m_command), "%s %s", 
               swMovieEditGetSettings(TheApp->GetTextedit()), m_editorFile);
    
}

void
AudioClipEdit::generateFilename(bool secondTry)
{
    m_editorFile[0] = (char) 0;
    MyString path = "";

    MFString *urls = m_audioClipNode->url();
    for (int i = 0; i < urls->getSize(); i++) {
        URL url(m_audioClipNode->getScene()->getURL(), urls->getValue(i));
        if (urls->getValue(i).length() == 0) continue;
        m_audioClipNode->getScene()->Download(url, &path);
        break;
    }
    if (strlen(path) < sizeof(m_editorFile))
        strcpy(m_editorFile, path);
}

void
AudioClipEdit::generateCommand(bool is4Kids)
{
    TheApp->checkAndRepairSoundEditCommand();
    mysnprintf(m_command, sizeof(m_command), "%s %s", 
               swSoundEditGetSettings(TheApp->GetTextedit()), m_editorFile);
    
}




