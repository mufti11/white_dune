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



void                
SceneView::debugOnUpdate(SceneView* sender, int type, Hint *hint)
{
    if (sender != NULL)
        printf("from %s to %s: ", sender->getName(), getName());
    else 
        printf("from NULL to %s: ", getName());

    switch(type) {
      case UPDATE_ALL: printf("UPDATE_ALL\n");
        break;
      case UPDATE_SELECTION: printf("UPDATE_SELECTION\n");
        break;     
      case UPDATE_FIELD: printf("UPDATE_FIELD\n");
        break;
      case UPDATE_ADD_NODE: printf("UPDATE_ADD_NODE\n");
        break;
      case UPDATE_REMOVE_NODE: printf("UPDATE_REMOVE_NODE\n");
        break;
      case UPDATE_CHANGE_INTERFACE_NODE: printf("UPDATE_CHANGE_INTERFACE_NODE\n");
        break;
      case UPDATE_ADD_ROUTE: printf("UPDATE_ADD_ROUTE\n");
        break;
      case UPDATE_DELETE_ROUTE: printf("UPDATE_DELETE_ROUTE\n");
        break;
      case UPDATE_MODE: printf("UPDATE_MODE\n");
        break;
      case UPDATE_TIME: printf("UPDATE_TIME\n");
        break;
      case UPDATE_START_FIELD_EDIT: printf("UPDATE_START_FIELD_EDIT\n");
        break;
      case UPDATE_STOP_FIELD_EDIT: printf("UPDATE_STOP_FIELD_EDIT\n");
        break;
      case UPDATE_ENABLE_COLOR_CIRCLE: printf("UPDATE_ENABLE_COLOR_CIRCLE\n");
        break;
      case UPDATE_DISABLE_COLOR_CIRCLE: printf("UPDATE_DISABLE_COLOR_CIRCLE\n");
        break;
      case UPDATE_CLOSE_COLOR_CIRCLE: printf("UPDATE_CLOSE_COLOR_CIRCLE\n");
        break;
      case UPDATE_SELECTED_FIELD: printf("UPDATE_SELECTED_FIELD\n");
        break;
      case UPDATE_SELECTION_NAME: printf("UPDATE_SELECTION_NAME\n");
        break;
      case UPDATE_NODE_NAME: printf("UPDATE_NODE_NAME\n");
        break;
      case UPDATE_ADD_NODE_SCENE_GRAPH_VIEW: printf("UPDATE_ADD_NODE_SCENE_GRAPH_VIEW\n");
        break;
      case UPDATE_REDRAW: printf("UPDATE_REDRAW\n");
        break;
      case UPDATE_PREVIEW: printf("UPDATE_PREVIEW\n");
        break;
      case UPDATE_SOLID_CHANGED: printf("UPDATE_SOLID_CHANGED\n");
        break;
      default:
        swDebugf("internal debugging problem: type %d is missing in %s\n",
                 type, "SceneView::debugOnUpdate");
    }    
}


