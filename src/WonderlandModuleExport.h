/*
 * WonderlandModuleExport.h
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

#pragma once

#define WONDERLAND_EXPORT_DIRECTORY "exportX3d"

class Scene;

class WonderlandModuleExport {
public:
            WonderlandModuleExport() { m_artPath = NULL; }
           ~WonderlandModuleExport();
    void    setArtPath(const char *path);
    int     write(const char* directory, const char* name, Scene *scene, 
                  bool manyClasses);
    int     writeArtPath(int filedes, const char *string) const;
protected:
    char  *m_artPath;
    char  *m_lowerName;
};

extern int       
writeCellRendererCode(int f, const char *upperName, const char *lowerName,
                      Scene *scene);

