/*
 * NodeLdrawDatExport.cpp
 *
 * Copyright (C) 2008 J. "MUFTI" Scheurich
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

/*
    Implementation of the Ldraw standard 3D fileformat
    The "LdrawDatExport" node contains informations from the the header
    of a Ldraw.dat file (fields "partDescription" till "history")
    and a field named "commands" which are useable for own initial 
    Ldraw commands before the ldraw polygon/line/colour commands of the 
    VRML/X3D model.
    For more information about the "LDraw Official Library header format",
    see http://www.ldraw.org/Article398.html    
    For more information about the LDraw commands of the
    "LDraw.org File Format Version 1.0.0" see 
    http://www.ldraw.org/Article218.html
 */

#include <stdio.h>
#include "stdafx.h"

#include <sys/types.h>
#include <sys/stat.h>
#ifndef _WIN32
# include <fcntl.h>
#endif
#include <time.h>

#include "NodeLdrawDatExport.h"
#include "Scene.h"
#include "Proto.h"
#include "Field.h"
#include "RenderState.h"
#include "Util.h"
#include "swt.h"
#include "NodeNavigationInfo.h"
#include "resource.h"

ProtoLdrawDatExport::ProtoLdrawDatExport(Scene *scene)
  : Proto(scene, "LdrawDatExport")
{
    partDescription.set(
         addField(SFSTRING, "partDescription", new SFString("")));
    author.set(
         addField(SFSTRING, "author", new SFString("")));
    fileType.set(
         addField(SFSTRING, "fileType", new SFString("Unofficial_Part")));
    license.set(
         addField(SFSTRING, "license", new SFString("Redistributable under CCAL version 2.0 : see CAreadme.txt")));
    bfc.set(
         addField(SFSTRING, "bfc", new SFString("NOCERTIFY")));
    help.set(
         addField(MFSTRING, "help", new MFString("")));
    category.set(
         addField(SFSTRING, "category", new SFString("")));
    keywords.set(
         addField(MFSTRING, "keywords", new MFString("")));
    cmdline.set(
         addField(SFSTRING, "cmdline", new SFString("")));
    double dtime = swGetCurrentTime();
    time_t t = fabs(dtime);
    struct tm *ltime = localtime(&t);
    char historyTime[256];
    mysnprintf(historyTime, 255, "%d-%d-%d", 
             ltime->tm_year + 1900, ltime->tm_mon + 1, ltime->tm_mday);
    history.set(
         addField(MFSTRING, "history", new MFString(historyTime)));
    commands.set(
         addField(MFSTRING, "commands", new MFString("")));

    addURLs(URL_EXPORT_CONTAINER);
}

Node *
ProtoLdrawDatExport::create(Scene *scene)
{ 
    return new NodeLdrawDatExport(scene, this); 
}

NodeLdrawDatExport::NodeLdrawDatExport(Scene *scene, Proto *def)
  : Node(scene, def)
{
}

int
NodeLdrawDatExport::writeProto(int f)
{
    return ((Node *)this)->writeProto(f, "", "exportContainers"
#ifdef HAVE_EXPORT_CONTAINER_PROTO_URL
                                      , HAVE_EXPORT_CONTAINER_PROTO_URL
#endif
                                     );
}

bool
NodeLdrawDatExport::checkMissingData(int fieldIndex, int limit) 
{
    Field *field = m_proto->getField(fieldIndex);
    if (field->getType() == SFSTRING) {
        SFString *fieldValue = (SFString *)getField(fieldIndex);
        if (strlen(fieldValue->getValue()) == 0) {
            const char *errorString = field->getName(false);
            if (fieldIndex == fileType_Field())
                errorString = "fileType (\"!LDRAW_ORG\" command)"; 
            TheApp->MessageBox(IDS_LDRAW_DAT_EXPORT_NEEDS_DATA, errorString);
            return false;
        }
        if (limit > 0)
            if (strlen(fieldValue->getValue()) > limit) {
                char format[256];
                swLoadString(IDS_LDRAW_DAT_EXPORT_FIELD_FOR_TOO_LONG, format, 
                             255);
                char msg[1024];
                mysnprintf(msg, 1023, format, 
                           (const char *)field->getName(false), limit);
                TheApp->MessageBox(msg);
                return false;
            }
    } else if (field->getType() == MFSTRING) {
        MFString *fieldValue = (MFString *)getField(fieldIndex);
        bool missingData = false;
        if (fieldValue->getSize() == 0)
            missingData = true;
        else {
            for (int i = 0; i < fieldValue->getSize(); i++)
                if (strlen(fieldValue->getValue(i)) == 0)
                    missingData = true;
            if (missingData) {
                TheApp->MessageBox(IDS_LDRAW_DAT_EXPORT_NEEDS_DATA, 
                                   field->getName(false));
                return false;
            }
        }
    }    
    return true;
}

bool
NodeLdrawDatExport::validate(void) 
{ 
    if ( !checkMissingData(partDescription_Field(), 51) ||
         !checkMissingData(author_Field()) ||
         !checkMissingData(fileType_Field()) ||
         !checkMissingData(license_Field()) ||
         !checkMissingData(category_Field()) ||
         !checkMissingData(history_Field()) )
        return false;
    return true; 
}


int
NodeLdrawDatExport::writeLdrawDatHeader(int filedes, const char *fileName)
{ 
    for (int j = 0; j < m_proto->getNumFields(); j++) {
        Field *field = m_proto->getField(j);
        if (field->getType() == SFSTRING) {
            SFString *fieldValue = (SFString *)getField(j);
            if (strlen(fieldValue->getValue()) > 0) {
                const char *commandName = field->getName(false);

                if (j == partDescription_Field())
                     commandName = ""; 
                if (j == author_Field()) {
                     // write filename ("0 Name: " first)
                     RET_ONERROR( mywritestr(filedes, "0 Name: ") )
                     RET_ONERROR( mywritestr(filedes, fileName) )
                     RET_ONERROR( mywritestr(filedes, "\n") )

                     commandName = "Author: ";
                } 
                if (j == license_Field())
                     commandName = ""; 
                if (j == fileType_Field())
                     commandName = "!LICENSE "; 
                if (j == bfc_Field())
                     commandName = "BFC "; 
                if (j == category_Field())
                     commandName = "!CATEGORY "; 
                if (j == cmdline_Field())
                     commandName = "!CMDLINE "; 

                RET_ONERROR( mywritestr(filedes, "0 ") )
                RET_ONERROR( mywritestr(filedes, commandName) )
                RET_ONERROR( mywritestr(filedes, fieldValue->getValue()) )
                RET_ONERROR( mywritestr(filedes, "\n") )
            }
        } else if (field->getType() == MFSTRING) {
            MFString *fieldValue = (MFString *)getField(j);
            for (int i = 0; i < fieldValue->getSize(); i++)
                if (strlen(fieldValue->getValue(i)) > 0) {
                    const char *commandName = field->getName(false);
                    if (j == help_Field())
                         commandName = "0 !HELP "; 
                    if (j == keywords_Field())
                         commandName = "0 !KEYWORDS "; 
                    if (j == history_Field())
                         commandName = "0 !HISTORY "; 
                    if (j == commands_Field())
                         commandName = ""; 
    
                    RET_ONERROR( mywritestr(filedes, commandName) )
                    RET_ONERROR( mywritestr(filedes, fieldValue->getValue(i)) )
                    RET_ONERROR( mywritestr(filedes, "\n") )
                }
        }
    }    
    return 0;
}


