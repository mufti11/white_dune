/*
 * GeoMacros.h
 *
 * Copyright (C) 1999 Stephen F. White, J. "MUFTI" Scheurich
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

#define addGeoFields() { \
    geoOrigin.set( \
          addExposedField(SFNODE, "geoOrigin", new SFNode(NULL), \
                          VRML_GEO_ORIGIN));\
    addGeoSystem(); \
}

#define addGeoSystem() { \
    static const char *systems[] = { "GD", "WE", NULL}; \
    StringArray *defaultsystems = new StringArray(); \
    defaultsystems->append("GD"); \
    defaultsystems->append("WE"); \
    geoSystem.set( \
          addExposedField(MFSTRING, "geoSystem", new MFString(defaultsystems), \
                          0, systems)); \
}
