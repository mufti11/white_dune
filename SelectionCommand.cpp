/*
 * SelectionCommand.cpp
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
#include "stdafx.h"

#include "SelectionCommand.h"
#include "Scene.h"
#include "Path.h"

SelectionCommand::SelectionCommand(Scene *scene, Path *newPath)
{
    m_oldPath = m_scene->getSelection();
    m_newPath = newPath;
}

SelectionCommand::~SelectionCommand()
{
    delete m_oldPath;
    delete m_newPath;
}

void
SelectionCommand::execute()
{
    m_scene->setSelection(m_newPath);
    m_scene->UpdateViews(NULL, UPDATE_SELECTION);
}

void
SelectionCommand::undo()
{
    m_scene->setSelection(m_oldPath);
    m_scene->UpdateViews(NULL, UPDATE_SELECTION);
}
