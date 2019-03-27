/*
 * UnRouteCommand.cpp
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

#include "stdafx.h"

#include "UnRouteCommand.h"
#include "Node.h"
#include "Scene.h"

UnRouteCommand::UnRouteCommand(Node *src, int eventOut, Node *dest, int eventIn)
{
    m_src = src;
    m_eventOut = eventOut;
    m_dest = dest;
    m_eventIn = eventIn;
}

void
UnRouteCommand::execute(SceneView* sender)
{
    Scene   *scene = m_src->getScene();

    scene->deleteRoute(m_src, m_eventOut, m_dest, m_eventIn);
}

void
UnRouteCommand::undo()
{
    Scene   *scene = m_src->getScene();

    scene->addRoute(m_src, m_eventOut, m_dest, m_eventIn);
}
