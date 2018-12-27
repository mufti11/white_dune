/* 
 * SceneViewDebugOnUpdate.cpp.m4
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

define(CASE, case $1: printf("$1\n");)

void                
SceneView::debugOnUpdate(SceneView* sender, int type, Hint *hint)
{
    if (sender != NULL)
        printf("from %s to %s: ", sender->getName(), getName());
    else 
        printf("from NULL to %s: ", getName());

    switch(type) {
      CASE(UPDATE_ALL)
        break;
      CASE(UPDATE_SELECTION)
        break;     
      CASE(UPDATE_FIELD)
        break;
      CASE(UPDATE_ADD_NODE)
        break;
      CASE(UPDATE_REMOVE_NODE)
        break;
      CASE(UPDATE_CHANGE_INTERFACE_NODE)
        break;
      CASE(UPDATE_ADD_ROUTE)
        break;
      CASE(UPDATE_DELETE_ROUTE)
        break;
      CASE(UPDATE_MODE)
        break;
      CASE(UPDATE_TIME)
        break;
      CASE(UPDATE_START_FIELD_EDIT)
        break;
      CASE(UPDATE_STOP_FIELD_EDIT)
        break;
      CASE(UPDATE_ENABLE_COLOR_CIRCLE)
        break;
      CASE(UPDATE_DISABLE_COLOR_CIRCLE)
        break;
      CASE(UPDATE_CLOSE_COLOR_CIRCLE)
        break;
      CASE(UPDATE_SELECTED_FIELD)
        break;
      CASE(UPDATE_SELECTION_NAME)
        break;
      CASE(UPDATE_NODE_NAME)
        break;
      CASE(UPDATE_ADD_NODE_SCENE_GRAPH_VIEW)
        break;
      CASE(UPDATE_REDRAW)
        break;
      CASE(UPDATE_PREVIEW)
        break;
      CASE(UPDATE_SOLID_CHANGED)
        break;
      default:
        swDebugf("internal debugging problem: type %d is missing in %s\n",
                 type, "SceneView::debugOnUpdate");
    }    
}


