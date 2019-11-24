/*
 * StartWithApp.h
 *
 * Copyright (C) 2007 J. "MUFTI" Scheurich
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

#ifndef _START_WITH_APP_H
#define _START_WITH_APP_H

class StartWithApp {
public:
                        StartWithApp();

    char               *GetStartVariant()
                           { return m_startVariant;}
    void                SetStartVariant(const char *variant)
                           {
                           if (m_startVariant != NULL)
                               free(m_startVariant);
                           m_startVariant = strdup(variant);
                           }

    char               *GetStartLanguage()
                           { return m_startLanguage;}
    void                SetStartLanguage(const char *language)
                           {
                           if (m_startLanguage != NULL)
                               free(m_startLanguage);
                           m_startLanguage = strdup(language);
                           }

    double              getDefaultSceneLength()
                           { return m_defaultSceneLength; }
    void                setDefaultSceneLength(double time) 
                           { m_defaultSceneLength = time; }

    void                StartWithSetDefaults();

    void                StartWithLoadPreferences();
    void                StartWithSavePreferences();

private:
    char               *m_startVariant;
    char               *m_startLanguage;
    double              m_defaultSceneLength;
};

#endif

