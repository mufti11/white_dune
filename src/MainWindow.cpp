/*
 * MainWindow.cpp
 *
 * Copyright (C) 1999 Stephen F. White, 2018 J. "MUFTI" Scheurich
 * 
 * Modified by Aaron Cram for SAND Dune
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

#include <errno.h>
#include <stdio.h>
#include <ctype.h>
#ifndef _WIN32
# include <sys/types.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <stdlib.h>
#endif

#include "stdafx.h"
#include "MainWindow.h"
#include "swt.h"
#include "resource.h"
#include "maxpath.h"

#include "Scene.h"
#include "Node.h"
#include "Field.h"
#include "Proto.h"
#include "Path.h"
#include "MFFloat.h"
#include "MFNode.h"
#include "MFVec3f.h"
#include "MFString.h"
#include "SFInt32.h"
#include "SFNode.h"
#include "SceneTreeView.h"
#include "Scene3DView.h"
#include "FieldView.h"
#include "SceneGraphView.h"
#include "ChannelView.h"
#include "ToolbarWindow.h"
#include "StatusBar.h"
#include "DuneApp.h"
#include "MoveCommand.h"
#include "RouteCommand.h"
#include "URL.h"

#include "X3DRigidJointNode.h"

#include "NodeAppearance.h"
#include "NodeAudioClip.h"
#include "NodeContact.h"
#include "NodeContour2D.h"
#include "NodeColor.h"
#include "NodeColorRGBA.h"
#include "NodeElevationGrid.h"
#include "NodeGroup.h"
#include "NodeTransform.h"
#include "NodeCollision.h"
#include "NodeCollidableShape.h"
#include "NodeBillboard.h"
#include "NodeLOD.h"
#include "NodeSwitch.h"
#include "NodeImageTexture.h"
#include "NodeCubeTexture.h"
#include "NodeHAnimHumanoid.h"
#include "NodeHAnimJoint.h"
#include "NodeInline.h"
#include "NodeInlineLoadControl.h"
#include "NodeMaterial.h"
#include "NodeMovieTexture.h"
#include "NodeNurbsGroup.h"
#include "NodeNurbsSet.h"
#include "NodeNurbsSurface.h"
#include "NodeNurbsCurve.h"
#include "NodeNurbsCurve2D.h"
#include "NodeNurbsPositionInterpolator.h"
#include "NodeNurbsOrientationInterpolator.h"
#include "NodeNurbsSweptSurface.h"
#include "NodeNurbsSwungSurface.h"
#include "NodeNurbsTrimmedSurface.h"
#include "NodeShape.h"
#include "NodeScript.h"
#include "NodeSound.h"
#include "NodeSuperEllipsoid.h"
#include "NodeSuperExtrusion.h"
#include "NodeSuperShape.h"
#include "NodeSuperRevolver.h"
#include "NodeCurveAnimation.h"
#include "NodeNormal.h"
#include "NodeText.h"
#include "NodeTextureCoordinate.h"
#include "NodeTextureCoordinateGenerator.h"
#include "NodeTimeSensor.h"
#include "NodeAnchor.h"
#include "NodeTriangleSet.h"
#include "NodeIndexedTriangleSet.h"
#include "NodeIndexedFaceSet.h"
#include "NodeIndexedLineSet.h"
#include "NodePointSet.h"
#include "NodeViewpoint.h"
#include "NodeOrthoViewpoint.h"
#include "NodeNavigationInfo.h"
#include "NodeWorldInfo.h"

#include "NodeLineSet.h"

#include "NodeGeoCoordinate.h"
#include "NodeGeoElevationGrid.h"
#include "NodeGeoLocation.h"
#include "NodeGeoLOD.h"
#include "NodeGeoMetadata.h"
#include "NodeGeoOrigin.h"
#include "NodeGeoPositionInterpolator.h"
#include "NodeGeoTouchSensor.h"
#include "NodeGeoViewpoint.h"
#include "NodeRigidBody.h"
#include "NodeRigidBodyCollection.h"
#include "NodeCollisionCollection.h"
#include "NodeVrmlCut.h"
#include "NodeVrmlScene.h"
#include "NodeBox.h"
#include "NodeCone.h"
#include "NodeCylinder.h"
#include "NodeSphere.h"
#include "NodeCollisionSensor.h"
#include "NodeParticleSystem.h"
#include "NodeCADFace.h"

#include "NodePackagedShader.h"
#include "NodeShaderProgram.h"
#include "NodeShaderPart.h"

#include "NodeVirtualSoundSource.h"

#include "NodeKambiAppearance.h"
#include "NodeKambiHeadLight.h"
#include "NodeKambiTriangulation.h"
#include "NodeText3D.h"

#include "AboutDialog.h"
#include "AnimationDialog.h"
#include "CurveAnimationDialog.h"
#include "InteractionDialog.h"
#include "InteractionDialog2.h"
#include "InteractionDialog3.h"
#include "ProtoDialog.h"
#include "ScriptDialog.h"
#include "ElevationGridDialog.h"
#include "NurbsPlaneDialog.h"
#include "NurbsCurveDialog.h"
#include "ModellingSettingsDialog.h"
#include "RenderSettingsDialog.h"
#include "EcmaScriptSettingsDialog.h"
#include "OutputSettingsDialog.h"
#include "PreviewSettingsDialog.h"
#include "UploadSettingsDialog.h"
#include "HelpSettingsDialog.h"
#include "TexteditSettingsDialog.h"
#include "StereoViewSettingsDialog.h"
#include "InputDeviceSelectionDialog.h"
#include "InputDeviceNameSelectionDialog.h"
#include "InputDeviceSettingsDialog.h"
#include "RouteViewSettingsDialog.h"
#include "FieldPipeDialog.h"
#include "ProtoNameDialog.h"
#include "ToIndexedFaceSetDialog.h"
#include "HAnimJointDialog.h"
#include "ViewpointDialog.h"
#include "NurbsCurve2DDialog.h"
#include "InsertToNurbsCurveDialog.h"
#include "InsertToNurbsSurfaceDialog.h"

#ifdef HAVE_AFLOCK
# include "AflockSettingsDialog.h"
#endif

#include "DefDialog.h"
#include "CoverDefDialog.h"
#include "Cylinder2NurbsDialog.h"
#include "Cone2NurbsDialog.h"
#include "Sphere2NurbsDialog.h"
#include "Box2NurbsDialog.h"
#include "NurbsCurve2NurbsSurfDialog.h"
#include "URLDialog.h"
#include "ArrayDialog.h"
#include "OneBoolDialog.h"
#include "OneIntDialog.h"
#include "OneFloatDialog.h"
#include "SliderFloatDialog.h"
#include "OneTextDialog.h"
#include "TwoRadioButtonsDialog.h"
#include "WonderlandModuleExportDialog.h"
#include "CattExportDialog.h"
#include "SplitFacesDialog.h"
#include "Util.h"

#include "FaceData.h"

#include "ColorCircle.h"
#include "VertexModifier.h"
#include "ScriptEdit.h"

#include "RenderState.h"

#ifdef _WIN32
# undef ARRAYSIZE
#endif
#define ARRAYSIZE(A) (sizeof(A) / sizeof(A[0]))
#define BS -1
#define BUFSIZE 1024

#define MFDouble MFFloat

enum {
    SAME_MIN,
    SAME,
    SAME_MAX
};

static int standardButtons[] = {
    0,  ID_DUNE_FILE_NEW,
    41, ID_DUNE_FILE_NEW_X3DV,
    1,  ID_DUNE_FILE_OPEN,
    2,  ID_DUNE_FILE_SAVE,
    3,  ID_DUNE_FILE_PREVIEW,
    BS, 0,
    4,  ID_DUNE_EDIT_CUT,
    5,  ID_DUNE_EDIT_COPY,
    6,  ID_DUNE_EDIT_PASTE,
    7,  ID_DUNE_EDIT_DELETE,
    BS, 0,
    8,  ID_DUNE_VIEW_FULL_SCREEN,
    BS, 0,
    29, ID_ANIMATION,
    37, ID_INTERACTION,
    BS, 0,
    38, ID_X_ONLY,
    39, ID_Y_ONLY,
    40, ID_Z_ONLY,
    BS, 0,
    28, ID_X_SYMETRIC,
    22, ID_COLOR_CIRCLE,
    46, ID_VERTEX_MODIFIER,
    23, ID_OBJECT_EDIT,
    25, ID_URL_EDIT,
    BS, 0,
    33, ID_TARGET,
    BS, 0,
    34, ID_WALK_MOUSE_MODE,
    35, ID_EXAMINE_MOUSE_MODE,
    24, ID_FLY_MOUSE_MODE,
    32, ID_ROLL_MOUSE_MODE,
    44, ID_FOLLOW_MOUSE_MODE,
    30, ID_MOUSE_NAVIGATION_MODE,
    BS, 0,
    31, ID_INPUT_DEVICE_NAVIGATION_MODE,
//    36, ID_SHOW_JOINT_LIKE_HANDLES,
    9,  ID_MOVE_MODE,
    10, ID_ROTATE_MODE,
    11, ID_SCALE_MODE,
    42, ID_UNIFORM_SCALE_MODE,
    12, ID_CENTER_MODE,
    13, ID_6D_MODE,
    14, ID_6DLOCAL_MODE,
    15, ID_ROCKET_MODE,
    16, ID_HOVER_MODE,
    BS, 0,
    26, ID_INPUTDEVICE_GREATER,
    27, ID_INPUTDEVICE_LESSER,
    BS, 0,
    20, ID_NEAR_FAR_MODE,
    21, ID_UP_DOWN_MODE,
    BS, 0,
    17, ID_3D_MODE,
    18, ID_2D_MODE,
    19, ID_1D_MODE
};

static int vcrButtons[] = {
    0,  ID_REWIND,
    5,  ID_LAST_KEY,
    1,  ID_STOP,
    2,  ID_RECORD,
    3,  ID_PLAY,
    6,  ID_NEXT_KEY,
    4,  ID_FAST_FORWARD
};

static NodeButton buttons1[] = {
    { VRML_BOX,                      ID_NEW_BOX,              true, true },
    { VRML_SPHERE,                   ID_NEW_SPHERE,           true, true },
    { VRML_CONE,                     ID_NEW_CONE,             true, true },
    { VRML_CYLINDER,                 ID_NEW_CYLINDER,         true, true },
    { VRML_INDEXED_FACE_SET,         ID_NEW_INDEXED_FACE_SET, true, true },
    { VRML_INDEXED_LINE_SET,         ID_NEW_INDEXED_LINE_SET, true, true },
    { VRML_POINT_SET,                ID_NEW_POINT_SET,        true, true },
    { VRML_ELEVATION_GRID,           ID_NEW_ELEVATION_GRID,   true, true },
    { VRML_EXTRUSION,                ID_NEW_EXTRUSION,        true, true },
    { VRML_TEXT,                     ID_NEW_TEXT,             true, true },
    { BS,                            0,                       true, true },
    { VRML_GROUP,                    ID_NEW_GROUP,            true, true },
    { VRML_TRANSFORM,                ID_NEW_TRANSFORM,        true, true },
    { VRML_BILLBOARD,                ID_NEW_BILLBOARD,        true, true },
    { VRML_COLLISION,                ID_NEW_COLLISION,        true, true },
    { VRML_LOD,                      ID_NEW_LOD,              true, true },
    { VRML_SWITCH,                   ID_NEW_SWITCH,           true, true },
    { BS,                            0,                       true, true },
    { VRML_ANCHOR,                   ID_NEW_ANCHOR,           true, true },
    { VRML_INLINE,                   ID_NEW_INLINE,           true, true }
};

static NodeButton buttons2[] = {
    { VRML_SHAPE,                    ID_NEW_SHAPE,              true,  true },
    { VRML_COORDINATE,               ID_NEW_COORDINATE,         false, true },
    { VRML_NORMAL,                   ID_NEW_NORMAL,             false, true },
    { VRML_COLOR,                    ID_NEW_COLOR,              false, true },
    { VRML_TEXTURE_COORDINATE,       ID_NEW_TEXTURE_COORDINATE, false, true },
    { VRML_FONT_STYLE,               ID_NEW_FONT_STYLE,         false, true },
    { BS,                            0,                         true,  true },
    { VRML_APPEARANCE,               ID_NEW_APPEARANCE,         false, true },
    { VRML_MATERIAL,                 ID_NEW_MATERIAL,           false, true },
    { VRML_IMAGE_TEXTURE,            ID_NEW_IMAGE_TEXTURE,      false, true },
    { VRML_PIXEL_TEXTURE,            ID_NEW_PIXEL_TEXTURE,      false, true },
    { VRML_MOVIE_TEXTURE,            ID_NEW_MOVIE_TEXTURE,      false, true },
    { VRML_TEXTURE_TRANSFORM,        ID_NEW_TEXTURE_TRANSFORM,  false, true },
    { BS,                            0,                         true,  true },
    { VRML_SOUND,                    ID_NEW_SOUND,              true,  true },
    { VRML_AUDIO_CLIP,               ID_NEW_AUDIO_CLIP,         false, true },
    { BS,                            0,                         true,  true },
    { VRML_DIRECTIONAL_LIGHT,        ID_NEW_DIRECTIONAL_LIGHT,  true,  true },
    { VRML_POINT_LIGHT,              ID_NEW_POINT_LIGHT,        true,  true },
    { VRML_SPOT_LIGHT,               ID_NEW_SPOT_LIGHT,         true,  true },
    { BS,                            0,                         true,  true },
    { VRML_FOG,                      ID_NEW_FOG,                true,  true },
    { VRML_BACKGROUND,               ID_NEW_BACKGROUND,         true,  true },
    { VRML_VIEWPOINT,                ID_NEW_VIEWPOINT,          true,  true },
    { VRML_NAVIGATION_INFO,          ID_NEW_NAVIGATION_INFO,    true,  true },
    { VRML_WORLD_INFO,               ID_NEW_WORLD_INFO,         true,  true }
};

static NodeButton buttons3[] = {
    { VRML_PROXIMITY_SENSOR,         ID_NEW_PROXIMITY_SENSOR,     true, true },
    { VRML_CYLINDER_SENSOR,          ID_NEW_CYLINDER_SENSOR,      true, true },
    { VRML_SPHERE_SENSOR,            ID_NEW_SPHERE_SENSOR,        true, true },
    { VRML_PLANE_SENSOR,             ID_NEW_PLANE_SENSOR,         true, true },
    { VRML_TIME_SENSOR,              ID_NEW_TIME_SENSOR,          true, true },
    { VRML_TOUCH_SENSOR,             ID_NEW_TOUCH_SENSOR,         true, true },
    { VRML_VISIBILITY_SENSOR,        ID_NEW_VISIBILITY_SENSOR,    true, true },
    { BS,                            0,                           true, true },
    { VRML_COLOR_INTERPOLATOR,       ID_NEW_COLOR_INTERPOLATOR,   true, true },
    { VRML_COORDINATE_INTERPOLATOR,  ID_NEW_COORDINATE_INTERPOLATOR, true, 
                                                                     true },
    { VRML_POSITION_INTERPOLATOR,    ID_NEW_POSITION_INTERPOLATOR, true, true },
    { VRML_ORIENTATION_INTERPOLATOR, ID_NEW_ORIENTATION_INTERPOLATOR, true, 
                                                                      true },
    { VRML_NORMAL_INTERPOLATOR,      ID_NEW_NORMAL_INTERPOLATOR,  true, true },
    { VRML_SCALAR_INTERPOLATOR,      ID_NEW_SCALAR_INTERPOLATOR,  true, true },
    { BS,                            0,                           true, true },
    { VRML_SCRIPT,                   ID_NEW_SCRIPT,               true, true },
    { BS,                            0,                           true, true },
    { VRML_COMMENT,                  ID_NEW_COMMENT,              true, true }
};

static NodeButton buttonsVRML200x[] = {
    { VRML_NURBS_CURVE,           ID_NEW_NURBS_CURVE,           true,  true },
    { VRML_NURBS_SURFACE,         ID_NEW_NURBS_PLANE_ZX,        true,  true },
    { X3D_NURBS_TEXTURE_COORDINATE, 
                                  ID_NEW_NURBS_TEXTURE_COORDINATE, 
                                                                false, true },
    { BS,                         0,                            true,  true },
    { LAST_NODE + 1,              ID_NEW_NURBS_GROUP,           true,  true },
    { BS,                         0,                            true,  true },
    { VRML_TRIMMED_SURFACE,       ID_NEW_TRIMMED_SURFACE,       false, true },
    { VRML_CONTOUR_2D,            ID_NEW_CONTOUR_2D,            false, true },
    { VRML_NURBS_CURVE_2D,        ID_NEW_NURBS_CURVE_2D,        false, true },
    { X3D_CONTOUR_POLYLINE_2D,    ID_NEW_CONTOUR_POLYLINE_2D,   false, true },
    { BS,                         0,                            true,  true },
    { VRML_COORDINATE_DEFORMER,   ID_NEW_COORDINATE_DEFORMER,   true,  true },
    { BS,                         0,                            true,  true },
    { VRML_NURBS_POSITION_INTERPOLATOR, ID_NEW_NURBS_POSITION_INTERPOLATOR, 
                                                                true,  true },
    { BS,                         0,                            true,  true },
    { VRML_GEO_LOCATION,          ID_NEW_GEO_LOCATION,          true,  true },
    { VRML_GEO_ELEVATION_GRID,    ID_NEW_GEO_ELEVATION_GRID,    true,  true },
    { X3D_GEO_TRANSFORM,          ID_NEW_GEO_TRANSFORM,         true,  true },
    { VRML_GEO_ORIGIN,            ID_NEW_GEO_ORIGIN,            false, true },
    { VRML_GEO_LOD,               ID_NEW_GEO_LOD,               true,  true },
    { VRML_GEO_COORDINATE,        ID_NEW_GEO_COORDINATE,        false, true },
    { VRML_GEO_VIEWPOINT,         ID_NEW_GEO_VIEWPOINT,         true,  true },
    { X3D_GEO_PROXIMITY_SENSOR,   ID_NEW_GEO_PROXIMITY_SENSOR,  true,  true },
    { VRML_GEO_TOUCH_SENSOR,      ID_NEW_GEO_TOUCH_SENSOR,      true,  true },
    { VRML_GEO_POSITION_INTERPOLATOR, ID_NEW_GEO_POSITION_INTERPOLATOR, 
                                                                true,  true },
    { X3D_GEO_METADATA,           ID_NEW_GEO_METADATA,          true,  true },
    { BS,                         0,                            true,  true },
    { VRML_INLINE_LOAD_CONTROL,   ID_NEW_INLINE_LOAD_CONTROL,   true,  true },
    { BS,                         0,                            true,  true },
    { X3D_VOLUME_DATA,            ID_NEW_VOLUME_DATA,           true,  true },
    { X3D_ISO_SURFACE_VOLUME_DATA,                        
                                  ID_NEW_ISO_SURFACE_VOLUME_DATA,   
                                                                true,  true },
    { X3D_SEGMENTED_VOLUME_DATA, ID_NEW_SEGMENTED_VOLUME_DATA,  true,  true },
    { BS,                         0,                            true,  true },
    { X3D_BLENDED_VOLUME_STYLE,   ID_NEW_BLENDED_VOLUME_STYLE,  true,  true },
    { X3D_BOUNDARY_ENHANCEMENT_VOLUME_STYLE, 
                                  ID_NEW_BOUNDARY_ENHANCEMENT_VOLUME_STYLE,
                                                                false,  true },
    { X3D_CARTOON_VOLUME_STYLE,   ID_NEW_CARTOON_VOLUME_STYLE,
                                                                false,  true },
    { X3D_COMPOSED_VOLUME_STYLE,  ID_NEW_COMPOSED_VOLUME_STYLE,
                                                                false,  true },
    { X3D_EDGE_ENHANCEMENT_VOLUME_STYLE,     
                                  ID_NEW_EDGE_ENHANCEMENT_VOLUME_STYLE,
                                                                false,  true },
    { X3D_OPACITY_MAP_VOLUME_STYLE,
                                  ID_NEW_OPACITY_MAP_VOLUME_STYLE,
                                                                false,  true },
    { X3D_PROJECTION_VOLUME_STYLE,
                                  ID_NEW_PROJECTION_VOLUME_STYLE,
                                                                false,  true },
    { X3D_SHADED_VOLUME_STYLE,    ID_NEW_SHADED_VOLUME_STYLE,   false,  true },
    { X3D_SILHOUETTE_ENHANCEMENT_VOLUME_STYLE, 
                                  ID_NEW_SILHOUETTE_ENHANCEMENT_VOLUME_STYLE,
                                                                false,  true },
    { X3D_TONE_MAPPED_VOLUME_STYLE,
                                  ID_NEW_TONE_MAPPED_VOLUME_STYLE,
                                                                false,  true }
};

static NodeButton buttonsX3DComponents1[] = {
    { X3D_NURBS_SWEPT_SURFACE,   ID_NEW_NURBS_SWEPT_SURFACE,   false, true },
    { X3D_NURBS_SWUNG_SURFACE,   ID_NEW_NURBS_SWUNG_SURFACE,   false, true },
    { X3D_NURBS_TRIMMED_SURFACE, ID_NEW_NURBS_TRIMMED_SURFACE, false, true },
    { X3D_LINE_SET,              ID_NEW_LINE_SET,              true,  true },
    { X3D_INDEXED_TRIANGLE_STRIP_SET, 
                                 ID_NEW_INDEXED_TRIANGLE_STRIP_SET,
                                                               true,  true },
    { X3D_INDEXED_TRIANGLE_FAN_SET,  
                                 ID_NEW_INDEXED_TRIANGLE_FAN_SET,  
                                                               true,  true },
    { X3D_INDEXED_TRIANGLE_SET,  ID_NEW_INDEXED_TRIANGLE_SET,  true,  true },
    { X3D_TRIANGLE_STRIP_SET,    ID_NEW_TRIANGLE_STRIP_SET,    true,  true },
    { X3D_TRIANGLE_FAN_SET,      ID_NEW_TRIANGLE_FAN_SET,      true,  true },
    { X3D_TRIANGLE_SET,          ID_NEW_TRIANGLE_SET,          true,  true },
    { BS,                        0,                            true,  true },
    { X3D_INDEXED_QUAD_SET,      ID_NEW_INDEXED_QUAD_SET,      true,  true },
    { X3D_QUAD_SET,              ID_NEW_QUAD_SET,              true,  true },
    { BS,                        0,                            true,  true },
    { X3D_TRIANGLE_SET_2D,       ID_NEW_TRIANGLE_SET_2D,       true,  true },
    { X3D_RECTANGLE_2D,          ID_NEW_RECTANGLE_2D,          true,  true },
    { X3D_DISK_2D,               ID_NEW_DISK_2D,               true,  true },
    { X3D_ARC_CLOSE_2D,          ID_NEW_ARC_CLOSE_2D,          true,  true },
    { X3D_CIRCLE_2D,             ID_NEW_CIRCLE_2D,             true,  true },
    { X3D_ARC_2D,                ID_NEW_ARC_2D,                true,  true },
    { VRML_POLYLINE_2D,          ID_NEW_POLYLINE_2D,           false, true },
    { X3D_POLYPOINT_2D,          ID_NEW_POLYPOINT_2D,          true,  true },
    { BS,                        0,                            true,  true },
    { X3D_STATIC_GROUP,          ID_NEW_STATIC_GROUP,          true,  true },
    { BS,                        0,                            true,  true },
    { X3D_COORDINATE_DOUBLE,     ID_NEW_COORDINATE_DOUBLE,     false, true },
    { X3D_COLOR_RGBA,            ID_NEW_COLOR_RGBA,            false, true },
    { X3D_CLIP_PLANE,            ID_NEW_CLIP_PLANE,            true,  true },
    { X3D_FOG_COORDINATE,        ID_NEW_FOG_COORDINATE,        false, true },
    { X3D_NURBS_TEXTURE_COORDINATE, 
                                 ID_NEW_NURBS_TEXTURE_COORDINATE, 
                                                               false, true },
    { BS,                        0,                            true,  true },
    { X3D_TWO_SIDED_MATERIAL,    ID_NEW_TWO_SIDED_MATERIAL,    false, true },
    { BS,                        0,                            true,  true },
    { X3D_FILL_PROPERTIES,       ID_NEW_FILL_PROPERTIES,       false, true },
    { X3D_LINE_PROPERTIES,       ID_NEW_LINE_PROPERTIES,       false, true },
    { BS,                        0,                            true,  true },
    { X3D_TEXTURE_PROPERTIES,    ID_NEW_TEXTURE_PROPERTIES,    false, true },
    { BS,                        0,                            true,  true },
    { X3D_LOCAL_FOG,             ID_NEW_LOCAL_FOG,             true,  true },
    { X3D_ORTHO_VIEWPOINT,       ID_NEW_ORTHO_VIEWPOINT,       true,  true },
    { X3D_VIEWPOINT_GROUP,       ID_NEW_VIEWPOINT_GROUP,       true,  true }
};

static NodeButton buttonsX3DComponents2[] = {
    { X3D_MULTI_TEXTURE_COORDINATE,         
                                 ID_NEW_MULTI_TEXTURE_COORDINATE,
                                                               false, true },
    { X3D_TEXTURE_COORDINATE_GENERATOR,
                                  ID_NEW_TEXTURE_COORDINATE_GENERATOR,
                                                               false, true },
    { X3D_TEXTURE_COORDINATE_3D, ID_NEW_TEXTURE_COORDINATE_3D, false, true },
    { X3D_TEXTURE_COORDINATE_4D, ID_NEW_TEXTURE_COORDINATE_4D, false, true },
    { BS,                        0,                            true,  true },
    { X3D_MULTI_TEXTURE,         ID_NEW_MULTI_TEXTURE,         false, true },
    { X3D_COMPOSED_TEXTURE_3D,   ID_NEW_COMPOSED_TEXTURE_3D,   false, true },
    { X3D_IMAGE_TEXTURE_3D,      ID_NEW_IMAGE_TEXTURE_3D,      false, true },
    { X3D_PIXEL_TEXTURE_3D,      ID_NEW_PIXEL_TEXTURE_3D,      false, true },
    { BS,                        0,                            true,  true },
    { X3D_MULTI_TEXTURE_TRANSFORM,     
                                 ID_NEW_MULTI_TEXTURE_TRANSFORM,
                                                               false, true },
    { X3D_TEXTURE_TRANSFORM_MATRIX_3D,     
                                 ID_NEW_TEXTURE_TRANSFORM_MATRIX_3D,
                                                               false, true },
    { X3D_TEXTURE_TRANSFORM_3D, ID_NEW_TEXTURE_TRANSFORM_3D,   false, true },
    { BS,                        0,                            true,  true },
    { X3D_POSITION_INTERPOLATOR_2D, 
                                 ID_NEW_POSITION_INTERPOLATOR_2D, 
                                                               true,  true },
    { X3D_COORDINATE_INTERPOLATOR_2D, 
                                 ID_NEW_COORDINATE_INTERPOLATOR_2D, 
                                                               true,  true },
    { X3D_NURBS_ORIENTATION_INTERPOLATOR,
                                 ID_NEW_NURBS_ORIENTATION_INTERPOLATOR,
                                                               true,  true },
    { X3D_NURBS_SURFACE_INTERPOLATOR, 
                                 ID_NEW_NURBS_SURFACE_INTERPOLATOR, 
                                                               true,  true },
    { X3D_SPLINE_POSITION_INTERPOLATOR,
                                 ID_NEW_SPLINE_POSITION_INTERPOLATOR,
                                                               true,  true },
    { X3D_SPLINE_POSITION_INTERPOLATOR_2D,
                                 ID_NEW_SPLINE_POSITION_INTERPOLATOR_2D,
                                                               true,  true },
    { X3D_SPLINE_SCALAR_INTERPOLATOR, 
                                 ID_NEW_SPLINE_SCALAR_INTERPOLATOR,
                                                               true,  true },
    { X3D_SQUAD_ORIENTATION_INTERPOLATOR,
                                 ID_NEW_SQUAD_ORIENTATION_INTERPOLATOR,
                                                               true,  true },
    { X3D_EASE_IN_EASE_OUT,      ID_NEW_EASE_IN_EASE_OUT,      true,  true },
    { BS,                        0,                            true,  true },
    { X3D_BOOLEAN_FILTER,        ID_NEW_BOOLEAN_FILTER,        true,  true },
    { X3D_BOOLEAN_TOGGLE,        ID_NEW_BOOLEAN_TOGGLE,        true,  true },
    { X3D_BOOLEAN_TRIGGER,       ID_NEW_BOOLEAN_TRIGGER,       true,  true },
    { X3D_INTEGER_TRIGGER,       ID_NEW_INTEGER_TRIGGER,       true,  true },
    { X3D_TIME_TRIGGER,          ID_NEW_TIME_TRIGGER,          true,  true },
    { X3D_BOOLEAN_SEQUENCER,     ID_NEW_BOOLEAN_SEQUENCER,     true,  true },
    { X3D_INTEGER_SEQUENCER,     ID_NEW_INTEGER_SEQUENCER,     true,  true },
    { BS,                        0,                            true,  true },
    { X3D_KEY_SENSOR,            ID_NEW_KEY_SENSOR,            true,  true },
    { X3D_STRING_SENSOR,         ID_NEW_STRING_SENSOR,         true,  true },
    { X3D_LOAD_SENSOR,           ID_NEW_LOAD_SENSOR,           true,  true },
    { X3D_TRANSFORM_SENSOR,      ID_NEW_TRANSFORM_SENSOR,      true,  true }
};

static NodeButton buttonsX3DComponents3[] = {
    { X3D_COMPOSED_SHADER,       ID_NEW_COMPOSED_SHADER,       false, true },
    { X3D_SHADER_PART,           ID_NEW_SHADER_PART,           false, true },
    { X3D_PACKAGED_SHADER,       ID_NEW_PACKAGED_SHADER,       false, true },
    { X3D_PROGRAM_SHADER,        ID_NEW_PROGRAM_SHADER,        false, true },
    { X3D_SHADER_PROGRAM,        ID_NEW_SHADER_PROGRAM,        false, true },
    { BS,                        0,                            true,  true },
    { X3D_FLOAT_VERTEX_ATTRIBUTE,
                                 ID_NEW_FLOAT_VERTEX_ATTRIBUTE,
                                                               false, true },
    { X3D_MATRIX_3_VERTEX_ATTRIBUTE,     
                                 ID_NEW_MATRIX_3_VERTEX_ATTRIBUTE,
                                                               false, true },
    { X3D_MATRIX_4_VERTEX_ATTRIBUTE,     
                                 ID_NEW_MATRIX_4_VERTEX_ATTRIBUTE,
                                                               false, true },
    { BS,                        0,                            true,  true },
    { X3D_LAYER_SET,             ID_NEW_LAYER_SET,             false, true },
    { X3D_LAYER,                 ID_NEW_LAYER,                 false, true },
    { X3D_VIEWPORT,              ID_NEW_VIEWPORT,              true,  true },
    { BS,                        0,                            true,  true },
    { X3D_LAYOUT_GROUP,          ID_NEW_LAYOUT_GROUP,          true,  true },
    { X3D_LAYOUT,                ID_NEW_LAYOUT,                false, true },
    { X3D_LAYOUT_LAYER,          ID_NEW_LAYOUT_LAYER,          false, true },
    { X3D_SCREEN_FONT_STYLE,     ID_NEW_SCREEN_FONT_STYLE,     false, true },
    { X3D_SCREEN_GROUP,          ID_NEW_SCREEN_GROUP,          true,  true },
    { BS,                        0,                            true,  true },
    { X3D_CAD_ASSEMBLY,          ID_NEW_CAD_ASSEMBLY,          true,  true },
    { X3D_CAD_FACE,              ID_NEW_CAD_FACE,              false, true },
    { X3D_CAD_LAYER,             ID_NEW_CAD_LAYER,             true,  true },
    { X3D_CAD_PART,              ID_NEW_CAD_PART,              true,  true },
    { BS,                        0,                            true,  true },
    { X3D_PARTICLE_SYSTEM,       ID_NEW_PARTICLE_SYSTEM,       true,  true },
    { BS,                        0,                            true,  true },
    { X3D_VOLUME_EMITTER,        ID_NEW_VOLUME_EMITTER,        false, true },
    { X3D_SURFACE_EMITTER,       ID_NEW_SURFACE_EMITTER,       false, true },
    { X3D_CONE_EMITTER,          ID_NEW_CONE_EMITTER,          false, true },
    { X3D_POLYLINE_EMITTER,      ID_NEW_POLYLINE_EMITTER,      false, true },
    { X3D_POINT_EMITTER,         ID_NEW_POINT_EMITTER,         false, true },
    { X3D_EXPLOSION_EMITTER,     ID_NEW_EXPLOSION_EMITTER,     false, true },
    { BS,                        0,                            true,  true },
    { X3D_BOUNDED_PHYSICS_MODEL, ID_NEW_BOUNDED_PHYSICS_MODEL, false, true },
    { X3D_FORCE_PHYSICS_MODEL,   ID_NEW_FORCE_PHYSICS_MODEL,   false, true },
    { X3D_WIND_PHYSICS_MODEL,    ID_NEW_WIND_PHYSICS_MODEL,    false, true }
};

static NodeButton buttonsX3DComponents4[] = {
    { X3D_RIGID_BODY_COLLECTION,   ID_NEW_RIGID_BODY_COLLECTION, true,  true },
    { X3D_RIGID_BODY,              ID_NEW_RIGID_BODY,            false, true },
    { BS,                          0,                            true,  true },
    { X3D_COLLIDABLE_SHAPE,        ID_NEW_COLLIDABLE_SHAPE,      true,  true },
    { X3D_COLLIDABLE_OFFSET,       ID_NEW_COLLIDABLE_OFFSET,     true,  true },
    { BS,                          0,                            true,  true },
    { X3D_CONTACT,                 ID_NEW_CONTACT,               true,  true },
    { BS,                          0,                            true,  true },
    { X3D_COLLISION_COLLECTION,    ID_NEW_COLLISION_COLLECTION,  false, true },
    { X3D_COLLISION_SPACE,         ID_NEW_COLLISION_SPACE,       false, true },
    { X3D_COLLISION_SENSOR,        ID_NEW_COLLISION_SENSOR,      true,  true },
    { BS,                          0,                            true,  true },
    { X3D_SLIDER_JOINT,            ID_NEW_SLIDER_JOINT,          false, true },
    { X3D_SINGLE_AXIS_HINGE_JOINT, ID_NEW_SINGLE_AXIS_HINGE_JOINT, 
                                                                 false, true },
    { X3D_DOUBLE_AXIS_HINGE_JOINT, ID_NEW_DOUBLE_AXIS_HINGE_JOINT, 
                                                                 false, true },
    { X3D_BALL_JOINT,              ID_NEW_BALL_JOINT,            false, true },
    { X3D_MOTOR_JOINT,             ID_NEW_MOTOR_JOINT,           false, true },
    { X3D_UNIVERSAL_JOINT,         ID_NEW_UNIVERSAL_JOINT,       false, true },
    { BS,                          0,                            true,  true },
    { X3D_PICKABLE_GROUP,          ID_NEW_PICKABLE_GROUP,        true,  true },
    { BS,                          0,                            true,  true },
    { X3D_LINE_PICK_SENSOR,        ID_NEW_LINE_PICK_SENSOR,      true,  true },
    { X3D_POINT_PICK_SENSOR,       ID_NEW_POINT_PICK_SENSOR,     true,  true },
    { X3D_PRIMITIVE_PICK_SENSOR,   ID_NEW_PRIMITIVE_PICK_SENSOR, true,  true },
    { X3D_VOLUME_PICK_SENSOR,      ID_NEW_VOLUME_PICK_SENSOR,    true,  true },
    { BS,                          0,                            true,  true },
    { X3D_HANIM_HUMANOID,          ID_NEW_HANIM_HUMANOID,        true,  true },
    { X3D_HANIM_JOINT,             ID_NEW_HANIM_JOINT,           false,  true },
    { X3D_HANIM_SEGMENT,           ID_NEW_HANIM_SEGMENT,         false,  true },
    { X3D_HANIM_DISPLACER,         ID_NEW_HANIM_DISPLACER,       false,  true },
    { X3D_HANIM_SITE,              ID_NEW_HANIM_SITE,            false,  true },
    { BS,                          0,                            true,  true },
    { X3D_COLOR_DAMPER,            ID_NEW_COLOR_DAMPER,          true,  true },
    { X3D_COORDINATE_DAMPER,       ID_NEW_COORDINATE_DAMPER,     true,  true },
    { X3D_ORIENTATION_DAMPER,      ID_NEW_ORIENTATION_DAMPER,    true,  true },
    { X3D_POSITION_DAMPER,         ID_NEW_POSITION_DAMPER,       true,  true },
    { X3D_POSITION_DAMPER_2D,      ID_NEW_POSITION_DAMPER_2D,    true,  true },
    { X3D_SCALAR_DAMPER,           ID_NEW_SCALAR_DAMPER,         true,  true },
    { X3D_TEX_COORD_DAMPER_2D,     ID_NEW_TEX_COORD_DAMPER_2D,   true,  true },
    { BS,                          0,                            true,  true },
    { X3D_COLOR_CHASER,            ID_NEW_COLOR_CHASER,          true,  true },
    { X3D_COORDINATE_CHASER,       ID_NEW_COORDINATE_CHASER,     true,  true },
    { X3D_ORIENTATION_CHASER,      ID_NEW_ORIENTATION_CHASER,    true,  true },
    { X3D_POSITION_CHASER,         ID_NEW_POSITION_CHASER,       true,  true },
    { X3D_POSITION_CHASER_2D,      ID_NEW_POSITION_CHASER_2D,    true,  true },
    { X3D_SCALAR_CHASER,           ID_NEW_SCALAR_CHASER,         true,  true },
    { X3D_TEX_COORD_CHASER_2D,     ID_NEW_TEX_COORD_CHASER_2D,   true,  true },
    { BS,                          0,                            true,  true },
    { X3D_ESPDU_TRANSFORM,         ID_NEW_ESPDU_TRANSFORM,       true,  true },
    { X3D_RECEIVER_PDU,            ID_NEW_RECEIVER_PDU,          true,  true },
    { X3D_SIGNAL_PDU,              ID_NEW_SIGNAL_PDU,            true,  true },
    { X3D_TRANSMITTER_PDU,         ID_NEW_TRANSMITTER_PDU,       true,  true },
    { X3D_DIS_ENTITY_MANAGER,      ID_NEW_DIS_ENTITY_MANAGER,    true,  true },
    { X3D_DIS_ENTITY_TYPE_MAPPING, 
                                   ID_NEW_DIS_ENTITY_TYPE_MAPPING,
                                                                 true,  true }
};

static NodeButton buttonsCover[] = {
    { COVER_SKY,                   ID_NEW_COVER_SKY,              true,  true },
    { BS,0, true, true },
    { COVER_CUBE_TEXTURE,          ID_NEW_COVER_CUBE_TEXTURE,     false, true },
    { BS,                          0,                             true,  true },
#ifdef HAVE_COVER_WAVE
    { COVER_WAVE,                  ID_NEW_COVER_WAVE,             true,  true },
    { BS,                          0,                             true,  true },
#endif
    { COVER_VIRTUAL_ACOUSTICS,     ID_NEW_COVER_VIRTUAL_ACOUSTICS,
                                                                  true,  true },
    { COVER_VIRTUAL_SOUND_SOURCE,  ID_NEW_COVER_VIRTUAL_SOUND_SOURCE,
                                                                  false, true},
    { BS,                          0,                             true,  true },
    { COVER_COVER,                 ID_NEW_COVER_COVER,            true,  true },
    { COVER_SPACE_SENSOR,          ID_NEW_COVER_SPACE_SENSOR,     true,  true },
    { COVER_AR_SENSOR,             ID_NEW_COVER_AR_SENSOR,        true,  true },
    { COVER_JOYSTICK_SENSOR,       ID_NEW_COVER_JOYSTICK_SENSOR,  true,  true },
    { COVER_STEERING_WHEEL,        ID_NEW_COVER_STEERING_WHEEL,   true,  true },
    { COVER_VEHICLE,               ID_NEW_COVER_VEHICLE,          true,  true },
    { COVER_LAB_VIEW,              ID_NEW_COVER_LAB_VIEW,         true,  true },
    { BS,                          0,                             true,  true },
    { COVER_TUI_BUTTON,            ID_NEW_COVER_TUI_BUTTON,       true,  true },
    { COVER_TUI_COMBO_BOX,         ID_NEW_COVER_TUI_COMBO_BOX,    true,  true },
    { COVER_TUI_FLOAT_SLIDER,      ID_NEW_COVER_TUI_FLOAT_SLIDER, true,  true },
    { COVER_TUI_FRAME,             ID_NEW_COVER_TUI_FRAME,        true,  true },
    { COVER_TUI_LABEL,             ID_NEW_COVER_TUI_LABEL,        true,  true },
    { COVER_TUI_LIST_BOX,          ID_NEW_COVER_TUI_LIST_BOX,     true,  true },
#ifdef HAVE_COVER_TUI_MAP
    { COVER_TUI_MAP,               ID_NEW_COVER_TUI_MAP,          true,  true },
#endif
    { COVER_TUI_PROGRESS_BAR,      ID_NEW_COVER_TUI_PROGRESS_BAR, true,  true },
    { COVER_TUI_SLIDER,            ID_NEW_COVER_TUI_SLIDER,       true,  true },
    { COVER_TUI_SPLITTER,          ID_NEW_COVER_TUI_SPLITTER,     true,  true },
    { COVER_TUI_TAB,               ID_NEW_COVER_TUI_TAB,          true,  true },
    { COVER_TUI_TAB_FOLDER,        ID_NEW_COVER_TUI_TAB_FOLDER,   true,  true },
    { COVER_TUI_TOGGLE_BUTTON,     ID_NEW_COVER_TUI_TOGGLE_BUTTON, true,  true }
};

static NodeButton buttonsKambi[] = {
    { KAMBI_TEAPOT,                ID_NEW_KAMBI_TEAPOT,           true,  true },
    { KAMBI_TEXT_3D,               ID_NEW_KAMBI_TEXT_3D,          true,  true },
    { BS,                          0,                             true,  true },
    { KAMBI_KAMBI_INLINE,          ID_NEW_KAMBI_INLINE,           true,  true },
    { BS,                          0,                             true,  true },
    { KAMBI_MATRIX_TRANSFORM,      ID_NEW_KAMBI_MATRIX_TRANSFORM, true,  true },
    { BS,                          0,                             true,  true },
    { KAMBI_KAMBI_APPEARANCE,      ID_NEW_KAMBI_APPEARANCE,       false, true },
    { KAMBI_BLEND_MODE,            ID_NEW_KAMBI_BLEND_MODE,       false, true },
    { KAMBI_EFFECT,                ID_NEW_KAMBI_EFFECT,           true,  true },
    { KAMBI_EFFECT_PART,           ID_NEW_KAMBI_EFFECT_PART,      false, true },
    { KAMBI_SCREEN_EFFECT,         ID_NEW_KAMBI_SCREEN_EFFECT,    true,  true },
    { KAMBI_KAMBI_OCTREE_PROPERTIES, ID_NEW_KAMBI_OCTREE_PROPERTIES,
                                                                  false, true },
    { KAMBI_GENERATED_SHADOW_MAP,  ID_NEW_KAMBI_GENERATED_SHADOW_MAP, 
                                                                  false, true },
    { KAMBI_MULTI_GENERATED_TEXTURE_COORDINATE,
                              ID_NEW_KAMBI_MULTI_GENERATED_TEXTURE_COORDINATE,
                                                                  false, true },
    { KAMBI_PROJECTED_TEXTURE_COORDINATE,      
                                   ID_NEW_KAMBI_PROJECTED_TEXTURE_COORDINATE, 
                                                                  false, true },
    { KAMBI_RENDERED_TEXTURE,      ID_NEW_KAMBI_RENDERED_TEXTURE, false, true },
    { KAMBI_SHADER_TEXTURE,        ID_NEW_KAMBI_SHADER_TEXTURE,   false, true },
    { BS,                          0,                             true,  true },
    { KAMBI_KAMBI_NAVIGATION_INFO, ID_NEW_KAMBI_NAVIGATION_INFO,  true,  true },
    { KAMBI_KAMBI_HEAD_LIGHT,      ID_NEW_KAMBI_HEAD_LIGHT,       true,  true },
    { BS,                          0,                             true,  true },
    { KAMBI_KAMBI_TRIANGULATION,   ID_NEW_KAMBI_TRIANGULATION,    true,  true },
    { BS,                          0,                             true,  true },
    { KAMBI_COLOR_SET_INTERPOLATOR,   
                                   ID_NEW_KAMBI_COLOR_SET_INTERPOLATOR,    
                                                                  true,  true },
    { KAMBI_VECTOR_INTERPOLATOR,   ID_NEW_KAMBI_VECTOR_INTERPOLATOR,    
                                                                  true,  true },
    { KAMBI_CUBIC_BEZIER_2D_ORIENTATION_INTERPOLATOR,
      ID_NEW_KAMBI_CUBIC_BEZIER_2D_ORIENTATION_INTERPOLATOR,      true,  true },
    { KAMBI_CUBIC_BEZIER_POSITION_INTERPOLATOR,
      ID_NEW_KAMBI_CUBIC_BEZIER_POSITION_INTERPOLATOR,            true,  true }
};

static NodeButton buttonsX3dom[] = {
    { X3DOM_BINARY_GEOMETRY,       ID_NEW_BINARY_GEOMETRY,        true,  true },
    { X3DOM_DISH,                  ID_NEW_DISH,                   true,  true },
    { X3DOM_EXTERNAL_GEOMETRY,     ID_NEW_EXTERNAL_GEOMETRY,      true,  true },
    { X3DOM_IMAGE_GEOMETRY,        ID_NEW_IMAGE_GEOMETRY,         true,  true },
    { X3DOM_NOZZLE,                ID_NEW_NOZZLE,                 true,  true },
    { X3DOM_MESH,                  ID_NEW_MESH,                   true,  true },
    { X3DOM_PARTICLE_SET,          ID_NEW_PARTICLE_SET,           true,  true },
    { X3DOM_PLANE,                 ID_NEW_PLANE,                  true,  true },
    { X3DOM_POP_GEOMETRY,          ID_NEW_POP_GEOMETRY,           true,  true },
    { X3DOM_PYRAMID,               ID_NEW_PYRAMID,                true,  true },
    { X3DOM_RECTANGULAR_TORUS,     ID_NEW_RECTANGULAR_TORUS,      true,  true },
    { X3DOM_SLOPED_CYLINDER,       ID_NEW_SLOPED_CYLINDER,        true,  true },
    { X3DOM_SNOUT,                 ID_NEW_SNOUT,                  true,  true },
    { X3DOM_SOLID_OF_REVOLUTION,   ID_NEW_SOLID_OF_REVOLUTION,    true,  true },
    { X3DOM_SPHERE_SEGMENT,        ID_NEW_SPHERE_SEGMENT,         true,  true },
    { X3DOM_TORUS,                 ID_NEW_X3DOM_TORUS,            true,  true },
    { BS,                          0,                             true,  true },
    { X3DOM_BLOCK,                 ID_NEW_BLOCK,                  true,  true },
    { X3DOM_REMOTE_SELECTION_GROUP,                 
                                   ID_NEW_REMOTE_SELECTION_GROUP, true,  true },
    { BS,                          0,                             true,  true },
    { X3DOM_COLOR_MASK_MODE,       ID_NEW_COLOR_MASK_MODE,        false, true },
    { X3DOM_DEPTH_MODE,            ID_NEW_DEPTH_MODE,             false, true },
    { X3DOM_POP_GEOMETRY_LEVEL,    ID_NEW_POP_GEOMETRY_LEVEL,     false, true },
    { BS,                          0,                             true,  true },
    { X3DOM_DYNAMIC_LOD,           ID_NEW_DYNAMIC_LOD,            true,  true },
    { BS,                          0,                             true,  true },
    { X3DOM_REFINEMENT_TEXTURE,    ID_NEW_REFINEMENT_TEXTURE,     false, true },
    { KAMBI_RENDERED_TEXTURE,      ID_NEW_RENDERED_TEXTURE,       false, true },
    { X3DOM_SURFACE_SHADER_TEXTURE, 
                                   ID_NEW_SURFACE_SHADER_TEXTURE, false, true },
    { BS,                          0,                             true,  true },
    { X3DOM_VIEWFRUSTUM,           ID_NEW_VIEWFRUSTUM,            true,  true },
    { BS,                          0,                             true,  true },
    { X3DOM_MULTI_PART,            ID_NEW_MULTI_PART,             false, true },
    { BS,                          0,                             true,  true },
    { X3DOM_ENVIRONMENT,           ID_NEW_ENVIRONMENT,            true,  true },
    { BS,                          0,                             true,  true },
    { X3DOM_MPR_VOLUME_STYLE,      ID_NEW_MPR_VOLUME_STYLE,       false, true },
    { X3DOM_RADAR_VOLUME_STYLE,    ID_NEW_RADAR_VOLUME_STYLE,     false, true },
    { X3DOM_STIPPLE_VOLUME_STYLE,  ID_NEW_STIPPLE_VOLUME_STYLE,   false, true },
    { BS,                          0,                             true,  true },
    { X3DOM_FIELD,                 ID_NEW_FIELD,                  true,  true },
    { X3DOM_UNIFORM,               ID_NEW_UNIFORM,                true,  true },
    { BS,                          0,                             true,  true },
    { X3DOM_IMAGE_TEXTURE_ATLAS,   ID_NEW_IMAGE_TEXTURE_ATLAS,    false, true },
    { BS,                          0,                             true,  true },
    { X3DOM_COMMON_SURFACE_SHADER, ID_NEW_COMMON_SURFACE_SHADER,  false, true }
};

static NodeButton buttonsScripted[] = {
    { DUNE_SUPER_ELLIPSOID,        ID_NEW_SUPER_ELLIPSOID,        true,  true },
    { DUNE_SUPER_EXTRUSION,        ID_NEW_SUPER_EXTRUSION,        true,  true },
    { DUNE_SUPER_SHAPE,            ID_NEW_SUPER_SHAPE,            true,  true },
    { DUNE_SUPER_REVOLVER,         ID_NEW_SUPER_REVOLVER,         true,  true },
    { BS,                          0,                             true,  true },
    { DUNE_VRML_CUT,               ID_NEW_VRML_CUT,               false, true },
    { DUNE_VRML_SCENE,             ID_NEW_VRML_SCENE,             false, true },
    { BS,                          0,                             true,  true },
    { DUNE_CURVE_ANIMATION,        ID_NEW_CURVE_ANIMATION,        true,  true },
    { BS,                          0,                             true,  true },
    { DUNE_CATT_EXPORT_SRC,        ID_NEW_CATT_EXPORT_SRC,        true,  true },
    { DUNE_CATT_EXPORT_REC,        ID_NEW_CATT_EXPORT_REC,        true,  true },
    { DUNE_LDRAW_DAT_EXPORT,       ID_NEW_LDRAW_DAT_EXPORT,       true,  true },
    { DUNE_WONDERLAND_IMPORT_JAVA, ID_NEW_WONDERLAND_IMPORT_JAVA, true,  true }
};


static int standardButtons4Kids[] = {
    1,  ID_DUNE_FILE_OPEN,
    2,  ID_DUNE_FILE_SAVE,
    BS, 0,
    43, ID_TO_NURBS_4KIDS,
    46, ID_VERTEX_MODIFIER,
    29, ID_ANIMATION_OR_CURVE_ANIMATION,
    BS, 0,
    38, ID_X_ONLY,
    39, ID_Y_ONLY,
    40, ID_Z_ONLY,
    BS, 0,
    28, ID_X_SYMETRIC,
    BS, 0,
    8,  ID_DUNE_VIEW_FULL_SCREEN,
    BS, 0,
    33, ID_TARGET,
    BS, 0,
    34, ID_WALK_MOUSE_MODE,
    35, ID_EXAMINE_MOUSE_MODE,
    24, ID_FLY_MOUSE_MODE,
    32, ID_ROLL_MOUSE_MODE,
    44, ID_FOLLOW_MOUSE_MODE,
    30, ID_MOUSE_NAVIGATION_MODE,
    BS, 0,
//    36, ID_SHOW_JOINT_LIKE_HANDLES,
    9,  ID_MOVE_MODE,
    10, ID_ROTATE_MODE,
    11, ID_SCALE_MODE,
    42, ID_UNIFORM_SCALE_MODE,
    12, ID_CENTER_MODE
};

static int standardButtons4KidsWithInputDevices[] = {
    1,  ID_DUNE_FILE_OPEN,
    2,  ID_DUNE_FILE_SAVE,
    BS, 0,
    43, ID_TO_NURBS_4KIDS,
    29, ID_ANIMATION_OR_CURVE_ANIMATION,
    BS, 0,
    38, ID_X_ONLY,
    39, ID_Y_ONLY,
    40, ID_Z_ONLY,
    BS, 0,
    28, ID_X_SYMETRIC,
    BS, 0,
    8,  ID_DUNE_VIEW_FULL_SCREEN,
    BS, 0,
    33, ID_TARGET,
    BS, 0,
    34, ID_WALK_MOUSE_MODE,
    35, ID_EXAMINE_MOUSE_MODE,
    24, ID_FLY_MOUSE_MODE,
    32, ID_ROLL_MOUSE_MODE,
    44, ID_FOLLOW_MOUSE_MODE,
    30, ID_MOUSE_NAVIGATION_MODE,
    BS, 0,
    31, ID_INPUT_DEVICE_NAVIGATION_MODE,
//    36, ID_SHOW_JOINT_LIKE_HANDLES,
    9,  ID_MOVE_MODE,
    10, ID_ROTATE_MODE,
    11, ID_SCALE_MODE,
    42, ID_UNIFORM_SCALE_MODE,
    12, ID_CENTER_MODE,
    13, ID_6D_MODE,
    14, ID_6DLOCAL_MODE,
    15, ID_ROCKET_MODE,
    16, ID_HOVER_MODE,
    BS, 0,
    26, ID_INPUTDEVICE_GREATER,
    27, ID_INPUTDEVICE_LESSER,
    BS, 0,
    20, ID_NEAR_FAR_MODE,
    21, ID_UP_DOWN_MODE,
    BS, 0,
    17, ID_3D_MODE,
    18, ID_2D_MODE,
    19, ID_1D_MODE
};

IconPos::IconPos(void)
{
    int size = sizeof(standardButtons);
    int *buttons = standardButtons;
    if (TheApp->is4Kids()) {
        if (TheApp->hasInputDevices()) {
            size = sizeof(standardButtons4KidsWithInputDevices);
            buttons = standardButtons4KidsWithInputDevices;
        } else {
            size = sizeof(standardButtons4Kids);
            buttons = standardButtons4Kids;
        }
    }
    m_fileNewIconPos     = GetIconPos(buttons, size, ID_DUNE_FILE_NEW);
    m_fileNewX3dIconPos  = GetIconPos(buttons, size, ID_DUNE_FILE_NEW_X3DV);
    m_fileOpenIconPos    = GetIconPos(buttons, size, ID_DUNE_FILE_OPEN);
    m_fileSaveIconPos    = GetIconPos(buttons, size, ID_DUNE_FILE_SAVE);
    m_filePreviewIconPos = GetIconPos(buttons, size, ID_DUNE_FILE_PREVIEW);
    m_fileWonderlandIconPos = GetIconPos(buttons, size, ID_DUNE_FILE_PREVIEW);

    m_editCutIconPos    = GetIconPos(buttons, size, ID_DUNE_EDIT_CUT);
    m_editCopyIconPos   = GetIconPos(buttons, size, ID_DUNE_EDIT_COPY);
    m_editPasteIconPos  = GetIconPos(buttons, size, ID_DUNE_EDIT_PASTE);
    m_editDeleteIconPos = GetIconPos(buttons, size, ID_DUNE_EDIT_DELETE);

    m_fullScreenIconPos = GetIconPos(buttons, size, ID_DUNE_VIEW_FULL_SCREEN);

    m_colorCircleIconPos = GetIconPos(buttons, size, ID_COLOR_CIRCLE);
    m_vertexModifierIconPos = GetIconPos(buttons, size, ID_VERTEX_MODIFIER);

    m_objectEditIconPos = GetIconPos(buttons, size, ID_OBJECT_EDIT);
    m_urlEditIconPos    = GetIconPos(buttons, size, ID_URL_EDIT);

    m_animationIconPos = GetIconPos(buttons, size, ID_ANIMATION);
    if (m_animationIconPos < 0)
        m_animationIconPos = GetIconPos(buttons, size, 
                                        ID_ANIMATION_OR_CURVE_ANIMATION);
    m_toNurbs4KidsIconPos = GetIconPos(buttons, size, ID_TO_NURBS_4KIDS);

    m_interactionIconPos = GetIconPos(buttons, size, ID_INTERACTION);

    m_x_symetricIconPos = GetIconPos(buttons, size, ID_X_SYMETRIC);

    m_mouseExamineIconPos = GetIconPos(buttons, size, ID_EXAMINE_MOUSE_MODE);
    m_mouseWalkIconPos    = GetIconPos(buttons, size, ID_WALK_MOUSE_MODE);
    m_mouseFlyIconPos     = GetIconPos(buttons, size, ID_FLY_MOUSE_MODE);
    m_mouseRollIconPos    = GetIconPos(buttons, size, ID_ROLL_MOUSE_MODE);
    m_mouseFollowIconPos  = GetIconPos(buttons, size, ID_FOLLOW_MOUSE_MODE);

    m_navigationMouseIconPos = GetIconPos(buttons, size, 
                                         ID_MOUSE_NAVIGATION_MODE);
    m_navigationInputDeviceIconPos = GetIconPos(buttons, size,
                                               ID_INPUT_DEVICE_NAVIGATION_MODE);

    m_moveModeIconPos = GetIconPos(buttons, size, ID_MOVE_MODE);
    m_rotateModeIconPos = GetIconPos(buttons, size, ID_ROTATE_MODE);
    m_scaleModeIconPos = GetIconPos(buttons, size, ID_SCALE_MODE);
    m_uniformScaleModeIconPos = GetIconPos(buttons, size, ID_UNIFORM_SCALE_MODE);
    m_centerModeIconPos = GetIconPos(buttons, size, ID_CENTER_MODE);
    m_sixDModeIconPos = GetIconPos(buttons, size, ID_6D_MODE);
    m_sixDLocalModeIconPos = GetIconPos(buttons, size, ID_6DLOCAL_MODE);
    m_rocketModeIconPos = GetIconPos(buttons, size, ID_ROCKET_MODE);
    m_hoverModeIconPos = GetIconPos(buttons, size, ID_HOVER_MODE);

    m_tDimensionStartIconPos = GetIconPos(buttons, size, ID_3D_MODE);

    m_t2axesStartIconPos = GetIconPos(buttons, size, ID_NEAR_FAR_MODE);

    m_xOnlyIconPos = GetIconPos(buttons, size, ID_X_ONLY);
    m_yOnlyIconPos = GetIconPos(buttons, size, ID_Y_ONLY);
    m_zOnlyIconPos = GetIconPos(buttons, size, ID_Z_ONLY);

    m_inputDeviceGreaterIconPos = GetIconPos(buttons, size,
                                             ID_INPUTDEVICE_GREATER);
    m_inputDeviceLesserIconPos = GetIconPos(buttons, size,
                                            ID_INPUTDEVICE_LESSER);
}

int IconPos::GetIconPos(int* buttons, int length, int icon)
{
    if (TheApp->is4Kids() && (buttons == standardButtons))
        return -1;
    for (int i = 0; i < length; i++)
        if (buttons[2 * i + 1] == icon)
            return i;
    return -1;
}

static int
timerCB(void *data)
{
    return ((MainWindow *) data)->OnTimer();
}

// callback for MainWindow::doWithBranch(callback, data)

static bool searchVrmlCut(Node *node, void *data)
{
    if (node->getType() == DUNE_VRML_CUT) {
        MainWindow *wnd = (MainWindow *)data;
        wnd->setVrmlCutNode(node);
        return false;
    }
    return true;     
}

MainWindow::MainWindow(Scene *scene, SWND wnd)
  : PanedWindow(scene, wnd, false)
{
    m_destroyed = false;
    m_colorCircle_enabled = false;
    m_colorCircle_active = false;
    m_colorCircleHint = NULL;
    m_vertexModifier_enabled = false;
    m_vertexModifier_active = false;
    m_vertexModifierHint = NULL;
    m_vertexModifier_overwrite = false;
    TheApp->accoutMaxNumberAxesInputDevices();
    m_vrmlCutNode = NULL;
    m_parentWindow = wnd;
    m_fieldView = NULL;
    m_timer = NULL;
    m_fieldPipeCommand = "";
    m_fieldPipeFilterNode = "";
    m_fieldPipeFilterField = "";
    m_showNumbers4Kids = false;

    if (TheApp->is4Kids())
        m_menu = swLoadMenuBar(wnd, IDR_DUNE_4KIDS_TYPE + swGetLang());
    else if (TheApp->is4Catt())
        m_menu = swLoadMenuBar(wnd, IDR_DUNE_4CATT_TYPE + swGetLang());
    else {
        m_menu = swLoadMenuBar(wnd, IDR_DUNE_TYPE + swGetLang());
#ifdef HAVE_OLPC
        swDebugf("On the olpc, you may prefer to start \"dune -4kids\"\n");
#endif
    }

    RefreshProtoMenu();
    RefreshRecentFileMenu();

#ifdef HAVE_SAND
    m_nebulaExporter.InitMenu(m_menu);
#endif

    SWND sb = swCreateCanvas("", 0, 0, 800, 22, wnd);

    m_statusBar = new StatusBar(scene, sb);
    SetPane(m_statusBar, PW_BOTTOM);
    if (TheApp->GetBoolPreference("ShowStatusBar", true)) {
        swMenuSetFlags(m_menu, ID_DUNE_VIEW_STATUS_BAR,
                       SW_MENU_CHECKED, SW_MENU_CHECKED);
    } else {
        swHideWindow(sb);
    }

    SWND o = swCreateCanvas("", 0, 0, 800, 400, wnd);
    m_outerPane = new PanedWindow(scene, o, false);
    SetPane(m_outerPane, PW_CENTER);

    SWND i = swCreateCanvas("", 0, 0, 800, 400, o);
    m_innerPane = new PanedWindow(scene, i, true);

    SWND t = swCreateCanvas("", 0, 0, 800, 100, o);
    m_toolbarWindow = new ToolbarWindow(scene, t, this);

    SWND t2 = swCreateCanvas("", 0, 0, 800, 100, o);
    m_toolbarWindow2 = new ToolbarWindow(scene, t2, this);

    SWND c1 = swCreateCanvas("", 0, 0, 200, 400, i);
    m_treeView = new SceneTreeView(scene, c1);
    m_innerPane->SetPane(m_treeView, PW_LEFT);
    if (TheApp->is4Kids() | TheApp->is4Catt()) {
        swMenuSetFlags(m_menu, ID_DUNE_VIEW_SCENE_TREE, SW_MENU_CHECKED, 
                       SW_MENU_CHECKED);
        m_treeView->setEnabled(true);
    } else {
        if (TheApp->GetBoolPreference("ShowSceneTree", true)) {
            swMenuSetFlags(m_menu, ID_DUNE_VIEW_SCENE_TREE, SW_MENU_CHECKED, 
            SW_MENU_CHECKED);
            m_treeView->setEnabled(true);
        } else {
            swHideWindow(m_treeView->GetWindow());
            m_treeView->setEnabled(false);
        }
    }

    SWND c2 = swCreateCanvas("", 200, 0, 400, 400, i);
    m_S3DView = new Scene3DView(scene, c2);
    m_innerPane->SetPane(m_S3DView, PW_CENTER);

    int width = 160 + swGetFontHeight(swGetDefaultFont()) * 18;
    SWND c3 = swCreateCanvas("", 600, 0, width, 400, i);
    m_fieldCanvas = c3;
    m_fieldView = new FieldView(scene, c3);
    m_innerPane->SetPane(m_fieldView, PW_RIGHT);
    if (TheApp->is4Kids()) {
        swHideWindow(m_fieldView->GetWindow());
        m_fieldView->setEnabled(false);
        swMenuSetFlags(m_menu, ID_DUNE_VIEW_FIELD_VIEW_4_KIDS, SW_MENU_CHECKED, 
                       m_showNumbers4Kids ? SW_MENU_CHECKED : 0);
    } else {
        if (TheApp->GetBoolPreference("ShowFieldView", true) ||
            TheApp->is4Catt()) {
            swMenuSetFlags(m_menu, ID_DUNE_VIEW_FIELD_VIEW, SW_MENU_CHECKED, 
                           SW_MENU_CHECKED);
            m_fieldView->setEnabled(true);
        } else {
            swHideWindow(m_fieldView->GetWindow());
            m_fieldView->setEnabled(false);
        }
    }

    SWND c4 = swCreateCanvas("", 0, 0, 800, 160, i);
    m_graphView = new SceneGraphView(scene, c4);
    m_innerPane->SetPane(m_graphView, PW_TOP);
    if (TheApp->is4Kids() | TheApp->is4Catt()) {
        swHideWindow(m_graphView->GetWindow());
        m_graphView->setEnabled(false);
    } else {
        if (TheApp->GetBoolPreference("ShowRouteView", false)) {
            swMenuSetFlags(m_menu, ID_DUNE_VIEW_SCENE_GRAPH, SW_MENU_CHECKED, 
                           SW_MENU_CHECKED);
            m_graphView->setEnabled(true);
        } else {
            swHideWindow(m_graphView->GetWindow());
            m_graphView->setEnabled(false);
        }
    }

    SWND c5 = swCreateCanvas("", 0, 400, 800, 200, i);
    m_channelView = new ChannelView(scene, c5);
    m_innerPane->SetPane(m_channelView, PW_BOTTOM);
    if (TheApp->is4Kids() | TheApp->is4Catt()) {
        swHideWindow(m_channelView->GetWindow());
       m_channelView->setEnabled(false);
    } else {
        if (TheApp->GetBoolPreference("ShowChannelView", true)) {
            swMenuSetFlags(m_menu, ID_DUNE_VIEW_CHANNEL_VIEW, SW_MENU_CHECKED, 
                           SW_MENU_CHECKED);
            m_channelView->setEnabled(true);
        } else {
            swHideWindow(m_channelView->GetWindow());
            m_channelView->setEnabled(false);
        }
    }

    bool olpc = TheApp->isOLPC();
    int idb_standard_toolbar = IDB_STANDARD_TOOLBAR;
    int size = ARRAYSIZE(standardButtons);
    int *buttons = standardButtons;
    if (TheApp->is4Kids()) {
        if (TheApp->hasInputDevices()) {
            size = ARRAYSIZE(standardButtons4KidsWithInputDevices);
            buttons = standardButtons4KidsWithInputDevices;
        } else {
            size = ARRAYSIZE(standardButtons4Kids);
            buttons = standardButtons4Kids;
        }
    }
    m_standardToolbar = LoadToolbar(m_toolbarWindow, idb_standard_toolbar,
                                    size / 2, buttons, ID_DUNE_VIEW_TOOLBAR,
                                    "StandardToolbar");
    if (TheApp->is4Catt())
        m_toolbarWindow->ShowToolbar(m_standardToolbar, false); 
    if (TheApp->is4Kids())
        m_toolbarWindow->ShowToolbar(m_standardToolbar, true); 
    if (!TheApp->hasStandardToolbar())
        m_toolbarWindow->ShowToolbar(m_standardToolbar, false); 
    int idb_node_icons = IDB_NODE_ICONS;
     // avoid update of toolbar button after use of this button,
     // cause this button will close the window and open a new one
     swToolbarSetButtonFlags(m_standardToolbar, m_fileOpenIconPos, 
                             SW_TB_DONT_UPDATE, SW_TB_DONT_UPDATE);
     swToolbarSetButtonFlags(m_standardToolbar, m_fileOpenIconPos, 
                             SW_TB_DISABLED, 0);
     swToolbarSetButtonFlags(m_standardToolbar, m_fileSaveIconPos, 
                             SW_TB_DISABLED, 0);

    m_nodeToolbar1 = LoadToolbar(m_toolbarWindow, idb_node_icons,
                                 ARRAYSIZE(buttons1), buttons1,
                                 ID_DUNE_VIEW_NODE_TOOLBAR_1, "NodeToolbar1");
    if (TheApp->is4Kids() || TheApp->is4Catt())
        m_nodeToolbar1Enabled = false;
    else
        m_nodeToolbar1Enabled = TheApp->GetBoolPreference("NodeToolbar1", !olpc);
    m_toolbarWindow->ShowToolbar(m_nodeToolbar1, m_nodeToolbar1Enabled);


    m_nodeToolbar2 = LoadToolbar(m_toolbarWindow, idb_node_icons,
                                 ARRAYSIZE(buttons2), buttons2,
                                 ID_DUNE_VIEW_NODE_TOOLBAR_2, "NodeToolbar2");
    if (TheApp->is4Kids() || TheApp->is4Catt())
        m_nodeToolbar2Enabled = false;
    else
        m_nodeToolbar2Enabled = TheApp->GetBoolPreference("NodeToolbar2", !olpc);
    m_toolbarWindow->ShowToolbar(m_nodeToolbar2, m_nodeToolbar2Enabled);

    m_nodeToolbar3 = LoadToolbar(m_toolbarWindow, idb_node_icons,
                                ARRAYSIZE(buttons3), buttons3, 
                                ID_DUNE_VIEW_NODE_TOOLBAR_3, "NodeToolbar3");
    if (TheApp->is4Kids() || TheApp->is4Catt())
        m_nodeToolbar3Enabled = false;
    else
        m_nodeToolbar3Enabled = TheApp->GetBoolPreference("NodeToolbar3", !olpc);
    m_toolbarWindow->ShowToolbar(m_nodeToolbar3, m_nodeToolbar3Enabled);

    for (unsigned int j = 0; j < ARRAYSIZE(buttonsVRML200x); j++) {
        // replace NurbsGroup icon with combined NurbsGroup/NurbsSet icon
        if (buttonsVRML200x[j].type == VRML_NURBS_GROUP) {
            buttonsVRML200x[j].type = scene->getNumberBuildinProtos() + 1;
            break;
        }
    }
    m_nodeToolbarVRML200x = LoadToolbar(m_toolbarWindow, idb_node_icons,
                                        ARRAYSIZE(buttonsVRML200x),
                                        buttonsVRML200x, 
                                        ID_DUNE_VIEW_NODE_TOOLBAR_VRML200X,
                                        "NodeToolbarVRML200x");
    if (TheApp->is4Kids() || TheApp->is4Catt())
        m_nodeToolbarVRML200xEnabled = false;
    else
        m_nodeToolbarVRML200xEnabled = TheApp->GetBoolPreference(
                                       "NodeToolbarVRML200x", false);
    m_toolbarWindow->ShowToolbar(m_nodeToolbarVRML200x, 
                                 m_nodeToolbarVRML200xEnabled);

    m_nodeToolbarX3DComponents1 = LoadToolbar(m_toolbarWindow, idb_node_icons,
                                     ARRAYSIZE(buttonsX3DComponents1),
                                     buttonsX3DComponents1, 
                                     ID_DUNE_VIEW_NODE_TOOLBAR_X3D_COMPONENTS_1,
                                     "NodeToolbarX3DComponents1");
    if (TheApp->is4Kids() || TheApp->is4Catt())
        m_nodeToolbarX3DComponents1Enabled = false;
    else
        m_nodeToolbarX3DComponents1Enabled = TheApp->GetBoolPreference(
                                            "NodeToolbarX3DComponents1", false);
     m_toolbarWindow->ShowToolbar(m_nodeToolbarX3DComponents1, 
                                  m_nodeToolbarX3DComponents1Enabled);


    m_nodeToolbarX3DComponents2 = LoadToolbar(m_toolbarWindow, idb_node_icons,
                                     ARRAYSIZE(buttonsX3DComponents2),
                                     buttonsX3DComponents2, 
                                     ID_DUNE_VIEW_NODE_TOOLBAR_X3D_COMPONENTS_2,
                                     "NodeToolbarX3DComponents2");
    if (TheApp->is4Kids() || TheApp->is4Catt())
        m_nodeToolbarX3DComponents2Enabled = false;
    else
        m_nodeToolbarX3DComponents2Enabled = TheApp->GetBoolPreference(
                                            "NodeToolbarX3DComponents2", false);
     m_toolbarWindow->ShowToolbar(m_nodeToolbarX3DComponents2, 
                                 m_nodeToolbarX3DComponents2Enabled);

    m_nodeToolbarX3DComponents3 = LoadToolbar(m_toolbarWindow, idb_node_icons,
                                     ARRAYSIZE(buttonsX3DComponents3),
                                     buttonsX3DComponents3, 
                                     ID_DUNE_VIEW_NODE_TOOLBAR_X3D_COMPONENTS_3,
                                     "NodeToolbarX3DComponents3");
    if (TheApp->is4Kids() || TheApp->is4Catt())
        m_nodeToolbarX3DComponents3Enabled = false;
    else
        m_nodeToolbarX3DComponents3Enabled = TheApp->GetBoolPreference(
                                             "NodeToolbarX3DComponents3", false);
     m_toolbarWindow->ShowToolbar(m_nodeToolbarX3DComponents3, 
                                  m_nodeToolbarX3DComponents3Enabled);

    m_nodeToolbarX3DComponents4 = LoadToolbar(m_toolbarWindow, idb_node_icons,
                                     ARRAYSIZE(buttonsX3DComponents4),
                                     buttonsX3DComponents4, 
                                     ID_DUNE_VIEW_NODE_TOOLBAR_X3D_COMPONENTS_4,
                                     "NodeToolbarX3DComponents4");
    if (TheApp->is4Kids() || TheApp->is4Catt())
        m_nodeToolbarX3DComponents4Enabled = false;
    else
        m_nodeToolbarX3DComponents4Enabled = TheApp->GetBoolPreference(
                                             "NodeToolbarX3DComponents4", false);
     m_toolbarWindow->ShowToolbar(m_nodeToolbarX3DComponents4, 
                                  m_nodeToolbarX3DComponents4Enabled);

    m_nodeToolbarCover = LoadToolbar(m_toolbarWindow, idb_node_icons,
                                    ARRAYSIZE(buttonsCover), 
                                    buttonsCover, 
                                    ID_DUNE_VIEW_NODE_TOOLBAR_COVER,
                                    "NodeToolbarCover");
    if (TheApp->is4Kids() || TheApp->is4Catt() || !TheApp->getCoverMode()) 
        m_nodeToolbarCoverEnabled = false;
    else
        m_nodeToolbarCoverEnabled = TheApp->GetBoolPreference("NodeToolbarCover",
                                                             !olpc);
    m_toolbarWindow->ShowToolbar(m_nodeToolbarCover, m_nodeToolbarCoverEnabled);

    m_nodeToolbarKambi = LoadToolbar(m_toolbarWindow, idb_node_icons,
                                    ARRAYSIZE(buttonsKambi), 
                                    buttonsKambi, 
                                    ID_DUNE_VIEW_NODE_TOOLBAR_KAMBI,
                                    "NodeToolbarKambi");
    if (TheApp->is4Kids() || TheApp->is4Catt() || !TheApp->getKambiMode())
        m_nodeToolbarKambiEnabled = false;
    else
        m_nodeToolbarKambiEnabled = TheApp->GetBoolPreference("NodeToolbarKambi",
                                                             !olpc);
    m_toolbarWindow->ShowToolbar(m_nodeToolbarKambi, m_nodeToolbarKambiEnabled);

    m_nodeToolbarX3dom = LoadToolbar(m_toolbarWindow, idb_node_icons,
                                    ARRAYSIZE(buttonsX3dom), 
                                    buttonsX3dom, 
                                    ID_DUNE_VIEW_NODE_TOOLBAR_X3DOM,
                                    "NodeToolbarX3dom");
    if (TheApp->is4Kids() || TheApp->is4Catt() || !TheApp->getX3domMode())
        m_nodeToolbarX3domEnabled = false;
    else
        m_nodeToolbarX3domEnabled = TheApp->GetBoolPreference("NodeToolbarX3dom",
                                                             !olpc);
    m_toolbarWindow->ShowToolbar(m_nodeToolbarX3dom, m_nodeToolbarX3domEnabled);

    m_nodeToolbarScripted = LoadToolbar(m_toolbarWindow, idb_node_icons,
                                       ARRAYSIZE(buttonsScripted),
                                       buttonsScripted, 
                                       ID_DUNE_VIEW_NODE_TOOLBAR_SCRIPTED,
                                       "NodeToolbarScripted");
    if (TheApp->is4Kids() || TheApp->is4Catt())
        m_nodeToolbarScriptedEnabled = false;
    else
        m_nodeToolbarScriptedEnabled = TheApp->GetBoolPreference(
                                      "NodeToolbarScripted", false);
    m_toolbarWindow->ShowToolbar(m_nodeToolbarScripted, 
                                m_nodeToolbarScriptedEnabled);
    m_outerPane->Layout();

    int idb_vcr_toolbar = IDB_VCR_TOOLBAR;
    m_vcrToolbar = LoadToolbar(m_toolbarWindow2, idb_vcr_toolbar,
                               ARRAYSIZE(vcrButtons) / 2, vcrButtons,
                               ID_DUNE_VIEW_PLAY_TOOLBAR, "PlayToolbar");
    m_outerPane->SetPane(m_toolbarWindow, PW_TOP);
    m_outerPane->SetPane(m_toolbarWindow2, PW_BOTTOM);
    m_outerPane->SetPane(m_innerPane, PW_CENTER);

    InitToolbars();

    m_fullScreen_enabled = TheApp->GetBoolPreference("FullScreen", false) ||
                           TheApp->getFullScreenAtBegin();
    swMenuSetFlags(m_menu, ID_DUNE_VIEW_FULL_SCREEN, SW_MENU_RADIO_ITEM, 0);
    swMenuSetFlags(m_menu, ID_DUNE_VIEW_FULL_SCREEN, SW_MENU_CHECKED, 0);

    swMenuSetFlags(m_menu, ID_DUNE_DESELECT, SW_MENU_CHECKED, 
                   scene->getDeselect() ? SW_MENU_CHECKED : 0);

    scene->setSelection(scene->getRoot());

    setColorCircleIcon();
    setVertexModifierIcon();
    m_objectEdit_enabled = false;
    m_urlEdit_enabled = false;
    setXSymetricNurbsIcon();
    m_navigation_mouse_active = false;
    m_scene->setMouseNavigationMode(m_navigation_mouse_active);
    m_navigation_input_device_active = false;
    m_scene->setInputDeviceNavigationMode(m_navigation_input_device_active);

    swMenuSetFlags(m_menu, ID_EXAMINE_MOUSE_MODE, SW_MENU_RADIO_ITEM, 
                   SW_MENU_RADIO_ITEM);
    swMenuSetFlags(m_menu, ID_FLY_MOUSE_MODE, SW_MENU_RADIO_ITEM, 
                   SW_MENU_RADIO_ITEM);
    swMenuSetFlags(m_menu, ID_WALK_MOUSE_MODE, SW_MENU_RADIO_ITEM, 
                   SW_MENU_RADIO_ITEM);
    swMenuSetFlags(m_menu, ID_ROLL_MOUSE_MODE, SW_MENU_RADIO_ITEM, 
                   SW_MENU_RADIO_ITEM);
    swMenuSetFlags(m_menu, ID_FOLLOW_MOUSE_MODE, SW_MENU_RADIO_ITEM, 
                   SW_MENU_RADIO_ITEM);

    swMenuSetFlags(m_menu, ID_MOVE_MODE, SW_MENU_RADIO_ITEM, SW_MENU_RADIO_ITEM);
    swMenuSetFlags(m_menu, ID_ROTATE_MODE, SW_MENU_RADIO_ITEM, SW_MENU_RADIO_ITEM);
    swMenuSetFlags(m_menu, ID_SCALE_MODE, SW_MENU_RADIO_ITEM, SW_MENU_RADIO_ITEM);
    swMenuSetFlags(m_menu, ID_UNIFORM_SCALE_MODE, SW_MENU_RADIO_ITEM, SW_MENU_RADIO_ITEM);
    swMenuSetFlags(m_menu, ID_CENTER_MODE, SW_MENU_RADIO_ITEM, SW_MENU_RADIO_ITEM);

    if (TheApp->getMaxNumberAxesInputDevices()>4) {
       swMenuSetFlags(m_menu, ID_6DLOCAL_MODE, SW_MENU_RADIO_ITEM, SW_MENU_RADIO_ITEM);
       swMenuSetFlags(m_menu, ID_6D_MODE, SW_MENU_RADIO_ITEM, SW_MENU_RADIO_ITEM);
    } else {
       swMenuSetFlags(m_menu, ID_6DLOCAL_MODE, SW_MENU_RADIO_ITEM, SW_MENU_DISABLED);
       swMenuSetFlags(m_menu, ID_6D_MODE, SW_MENU_RADIO_ITEM, SW_MENU_DISABLED);
    }

    if (TheApp->getMaxNumberAxesInputDevices()>=3)
       swMenuSetFlags(m_menu, ID_ROCKET_MODE, SW_MENU_RADIO_ITEM, SW_MENU_RADIO_ITEM);
    else
       swMenuSetFlags(m_menu, ID_ROCKET_MODE, SW_MENU_RADIO_ITEM, SW_MENU_DISABLED);

    if (TheApp->getMaxNumberAxesInputDevices()>=2) 
       {
       swMenuSetFlags(m_menu, ID_HOVER_MODE, SW_MENU_RADIO_ITEM, SW_MENU_RADIO_ITEM);
       swMenuSetFlags(m_menu, ID_3D_MODE, SW_MENU_RADIO_ITEM, SW_MENU_RADIO_ITEM);
       swMenuSetFlags(m_menu, ID_2D_MODE, SW_MENU_RADIO_ITEM, SW_MENU_RADIO_ITEM);
       swMenuSetFlags(m_menu, ID_1D_MODE, SW_MENU_RADIO_ITEM, SW_MENU_RADIO_ITEM);
       }
    else
       {
       swMenuSetFlags(m_menu, ID_HOVER_MODE, SW_MENU_RADIO_ITEM, SW_MENU_DISABLED);
       swMenuSetFlags(m_menu, ID_3D_MODE, SW_MENU_RADIO_ITEM, SW_MENU_DISABLED);
       swMenuSetFlags(m_menu, ID_2D_MODE, SW_MENU_RADIO_ITEM, SW_MENU_DISABLED);
       swMenuSetFlags(m_menu, ID_1D_MODE, SW_MENU_RADIO_ITEM, SW_MENU_DISABLED);
       }
    swMenuSetFlags(m_menu, ID_RECALIBRATE, SW_MENU_DISABLED, 0);
       
    setTMode(m_scene->getTransformMode()->tmode);
    setTDimension(m_scene->getTransformMode()->tdimension);

    if (TheApp->has2AxesInputDevices() || TheApp->has3AxesInputDevices()) {
       swMenuSetFlags(m_menu, ID_NEAR_FAR_MODE, SW_MENU_RADIO_ITEM, SW_MENU_RADIO_ITEM);
       swMenuSetFlags(m_menu, ID_UP_DOWN_MODE, SW_MENU_RADIO_ITEM, SW_MENU_RADIO_ITEM);
    } else {
       swMenuSetFlags(m_menu, ID_NEAR_FAR_MODE, SW_MENU_RADIO_ITEM, SW_MENU_DISABLED);
       swMenuSetFlags(m_menu, ID_UP_DOWN_MODE, SW_MENU_RADIO_ITEM, SW_MENU_DISABLED);
    }      

    setT2axes(m_scene->getTransformMode()->t2axes);

    swMenuSetFlags(m_menu, ID_DUNE_FILE_EXPORT_VRML97_COVER, SW_MENU_DISABLED, 
                   TheApp->getCoverMode() ? 0 : SW_MENU_DISABLED);

    swToolbarSetButtonFlags(m_standardToolbar, m_fileNewX3dIconPos, 
                            SW_TB_DISABLED, 
                            TheApp->getCoverMode() ? SW_TB_DISABLED : 0);
    swMenuSetFlags(m_menu, ID_DUNE_FILE_EXPORT_X3DV, SW_MENU_DISABLED, 
                   TheApp->getCoverMode() ? SW_MENU_DISABLED : 0);
    swMenuSetFlags(m_menu, ID_DUNE_FILE_EXPORT_X3D_XML, SW_MENU_DISABLED, 
                   TheApp->getCoverMode() ? SW_MENU_DISABLED : 0);
    swMenuSetFlags(m_menu, ID_DUNE_FILE_EXPORT_X3D_4_WONDERLAND, 
                   SW_MENU_DISABLED, 
                   TheApp->getCoverMode() ? SW_MENU_DISABLED : 0);

    m_xOnly = false;
    m_yOnly = false;
    m_zOnly = false;
    m_scene->setXonly(m_xOnly);
    m_scene->setYonly(m_yOnly);
    m_scene->setZonly(m_zOnly);

    m_scriptEdit = NULL;
    m_scriptEditorInUse = false;
    m_textEditorInUse = false;
    m_textEdit = NULL;
    m_shaderEdit = NULL;
    m_imageTextureEdit = NULL;
    m_pixelTextureEdit = NULL;
    m_imageEditorInUse = false;
    m_soundEdit = NULL;
    m_soundEditorInUse = false;
    m_movieEdit = NULL;
    m_movieEditorInUse = false;
    m_selectedField = -1;
    static MyString staticString;
    m_statusText = staticString;
    m_searchText = "";

    scene->UpdateViews(this, UPDATE_ALL, NULL);

    UpdateToolbar(ID_DUNE_FILE_UPLOAD);
    UpdateToolbar(ID_SKIP_MATERIAL_NAME_BEFORE_FIRST_UNDERSCORE);        
    UpdateToolbar(ID_SKIP_MATERIAL_NAME_AFTER_LAST_UNDERSCORE);        

    if (TheApp->GetBoolPreference("MaximizeWindows", false)) {
        Layout(true);
        swMaximizeWindow(m_wnd);
    } else 
        Layout();

    m_scene->getRoot()->doWithBranch(searchVrmlCut, this);

    UpdateTitle();

    UpdateToolbars();

    // the following method of updating cover or kambi nodes are only needed 
    // once, because the commandlineoptions -cover or -kambi are the only 
    // way to enable covermode or kambimode
    UpdateStaticMenuCoverNodes();
    UpdateStaticMenuKambiNodes();
    UpdateStaticMenuX3domNodes();

    UpdateToolbarSelection();

    swShowWindow(m_wnd);

    if (m_fullScreen_enabled)
        setFullScreen();

    swToolbarSetButtonFlags(m_standardToolbar, m_xOnlyIconPos, 
                            SW_TB_DISABLED, 0);
    swToolbarSetButtonFlags(m_standardToolbar, m_yOnlyIconPos, 
                            SW_TB_DISABLED, 0);
    swToolbarSetButtonFlags(m_standardToolbar, m_zOnlyIconPos, 
                            SW_TB_DISABLED, 0);
}

void
MainWindow::destroyMainWindow(void) // but keep scene
{
    if (m_treeView && (m_scene != m_treeView->GetScene())) {
        // something strange happend 8-(
        return;
    }
    m_destroyed = true;
    if (!TheApp->is4Kids())
        if (m_vertexModifier_active)
            TheApp->SetBoolPreference("ShowFieldView", true);
    Stop();
    TheApp->SetBoolPreference("MaximizeWindows", swIsMaximized(m_wnd) != 0);
#ifndef _WIN32 
    swDestroyWindow(m_wnd);
#endif
    delete m_treeView;
    m_treeView = NULL;
    delete m_S3DView;
    m_S3DView = NULL;
    delete m_fieldView;
    m_fieldView = NULL;
    delete m_graphView;
    m_graphView = NULL;
    delete m_channelView;
    m_channelView = NULL;
    delete m_toolbarWindow;
    m_toolbarWindow = NULL;
    delete m_toolbarWindow2;
    m_toolbarWindow2 = NULL;
    delete m_innerPane;
    m_innerPane = NULL;
    delete m_outerPane;
    m_outerPane = NULL;
    delete m_statusBar;
    m_statusBar = NULL;
    delete m_colorCircleHint;
    m_colorCircleHint = NULL;
    delete m_vertexModifierHint;
    m_vertexModifierHint = NULL;
    swDestroyToolbar(m_standardToolbar);
    m_standardToolbar = NULL;
    swDestroyToolbar(m_nodeToolbar1);
    m_nodeToolbar1 = NULL;
    swDestroyToolbar(m_nodeToolbar2);
    m_nodeToolbar2 = NULL;
    swDestroyToolbar(m_nodeToolbar3);
    m_nodeToolbar3 = NULL;
    swDestroyToolbar(m_nodeToolbarVRML200x);
    m_nodeToolbarVRML200x = NULL;
    swDestroyToolbar(m_nodeToolbarX3DComponents1);
    m_nodeToolbarX3DComponents1 = NULL;
    swDestroyToolbar(m_nodeToolbarX3DComponents2);
    m_nodeToolbarX3DComponents2 = NULL;
    swDestroyToolbar(m_nodeToolbarX3DComponents3);
    m_nodeToolbarX3DComponents3 = NULL;
    swDestroyToolbar(m_nodeToolbarX3DComponents4);
    m_nodeToolbarX3DComponents4 = NULL;
    swDestroyToolbar(m_nodeToolbarCover);
    m_nodeToolbarCover = NULL;
    swDestroyToolbar(m_nodeToolbarKambi);
    m_nodeToolbarKambi = NULL;
    swDestroyToolbar(m_nodeToolbarX3dom);
    m_nodeToolbarX3dom = NULL;
    swDestroyToolbar(m_nodeToolbarScripted);
    m_nodeToolbarScripted = NULL;
    swDestroyToolbar(m_vcrToolbar);
    m_vcrToolbar = NULL;
}


MainWindow::~MainWindow()
{
    destroyMainWindow();
    delete m_scene;
    swDestroyWindow(m_parentWindow);
}

void
MainWindow::UpdateTitle()
{
    MyString title = "Dune ";
    const char *path = m_scene->getPath();

    if (TheApp->canQuadBufferStereo())
        title += "(stereo visual)";
    else
        title += "(no stereo visual)";
    if (TheApp->getCoverMode())
        title += " (cover mode)"; 
    if (TheApp->getKambiMode())
        title += " (kambi mode)"; 
    if (TheApp->getX3domMode())
        title += " (X3DOM mode)"; 
    title += " : ";
    if (path[0]) {
        title += path;
    } else {
        title += "Untitled";
    }

    swSetTitle(m_wnd, title);
}

#ifdef HAVE_SAND
void MainWindow::OnSANDExport(void)
{
    char path[1024];
    path[0] = '\0';
    if (TheApp->getFileDialogDir())
        chdir(TheApp->getFileDialogDir());
    if (swSaveFileDialog(m_wnd, "Save As",
        "The Nebula Device TCL Script (.tcl)\0*.tcl;*.TCL\0All Files (*.*)\0*.*\0\0",
        path, 1024,".tcl")) {
        m_nebulaExporter.ExportScene(path, m_scene, m_statusBar);
    }
}
#endif   //  #ifdef HAVE_SAND

// callbacks for MainWindow::doWithBranch(callback, data)

template<class NormalNode> void removeNormalsTemplate(NormalNode node)
{ 
    NormalNode normalNode = (NormalNode) node;
    Node *nnormal = normalNode->normal()->getValue();
    if (nnormal != NULL) {
        MoveCommand *command = new MoveCommand(nnormal, normalNode, 
                                               normalNode->normal_Field(), 
                                               NULL, -1);
        command->execute();
    }
}

static bool removeNormals(Node *node, void *data)
{
    if (node->getType() == VRML_INDEXED_FACE_SET) {
        NodeIndexedFaceSet *faceSet = (NodeIndexedFaceSet *) node;
        removeNormalsTemplate(faceSet);
        if (faceSet->normalIndex()->getSize()>0)
            faceSet->normalIndex(new MFInt32());
    } else if (node->getType() == VRML_ELEVATION_GRID)
        removeNormalsTemplate((NodeElevationGrid *) node);
    else if (node->getType() == X3D_TRIANGLE_SET)
        removeNormalsTemplate((NodeTriangleSet *) node);
    return true;     
}

template<class TexCoordNode> void removeTexCoordTemplate(TexCoordNode node)
{ 
    TexCoordNode texCoordNode = (TexCoordNode) node;
    Node *ntexCoord = texCoordNode->texCoord()->getValue();
    if (ntexCoord != NULL) {
        MoveCommand *command = new MoveCommand(ntexCoord, texCoordNode, 
                                               texCoordNode->texCoord_Field(), 
                                               NULL, -1);
        command->execute();
    }
}

static bool removeTextureCoordinates(Node *node, void *data)
{
    if (node->getType() == VRML_INDEXED_FACE_SET) {
        NodeIndexedFaceSet *faceSet = (NodeIndexedFaceSet *) node;
        removeTexCoordTemplate(faceSet);
        if (faceSet->texCoordIndex()->getSize() > 0)
           faceSet->texCoordIndex(new MFInt32());
    } else if (node->getType() == VRML_ELEVATION_GRID)
        removeTexCoordTemplate((NodeElevationGrid *) node);
    else if (node->getType() == VRML_NURBS_SURFACE)
        removeTexCoordTemplate((NodeNurbsSurface *) node);
    else if (node->getType() == DUNE_SUPER_ELLIPSOID)
        removeTexCoordTemplate((NodeSuperEllipsoid *) node);
    return true;     
}

template<class ColorNode> void removeColorTemplate(ColorNode node)
{ 
    ColorNode colorNode = (ColorNode) node;
    Node *ncolor = colorNode->color()->getValue();
    if (ncolor != NULL) {
        MoveCommand *command = new MoveCommand(ncolor, colorNode, 
                                               colorNode->color_Field(), 
                                               NULL, -1);
        command->execute();
    }
}

static bool removeColors(Node *node, void *data)
{
    if (node->getType() == VRML_INDEXED_FACE_SET) {
        NodeIndexedFaceSet *faceSet = (NodeIndexedFaceSet *) node;
        removeColorTemplate(faceSet);
        if (faceSet->colorIndex()->getSize() > 0)
           faceSet->colorIndex(new MFInt32());
    } else if (node->getType() == VRML_INDEXED_LINE_SET) {
        NodeIndexedLineSet *lineSet = (NodeIndexedLineSet *) node;
        removeColorTemplate(lineSet);
        if (lineSet->colorIndex()->getSize() > 0)
           lineSet->colorIndex(new MFInt32());
    } else if (node->getType() == VRML_POINT_SET) {
        removeColorTemplate((NodePointSet *) node);
    } else if (node->getType() == VRML_ELEVATION_GRID)
        removeColorTemplate((NodeElevationGrid *) node);
    return true;     
}

static bool removeTexture(Node *node, void *data)
{
    if ((node->getType() == VRML_APPEARANCE) ||
        (node->getType() == KAMBI_KAMBI_APPEARANCE)) {
        NodeAppearance *appearanceNode = (NodeAppearance *) node;
        Node *ntexture = appearanceNode->texture()->getValue();
        if (ntexture != NULL) {
            MoveCommand *command = new MoveCommand(ntexture, 
                  appearanceNode, appearanceNode->texture_Field(), NULL, -1);
            command->execute();
        }
    }
    return true;     
}

static bool removeAppearance(Node *node, void *data)
{
    if (node->getType() == VRML_SHAPE) {
        NodeShape *shapeNode = (NodeShape *) node;
        Node *nappearance = shapeNode->appearance()->getValue();
        if (nappearance != NULL) {
            MoveCommand *command = new MoveCommand(nappearance, 
                  shapeNode, shapeNode->appearance_Field(), NULL, -1);
            command->execute();
        }
    }
    return true;     
}

static bool removeMaterial(Node *node, void *data)
{
    if ((node->getType() == VRML_APPEARANCE) ||
        (node->getType() == KAMBI_KAMBI_APPEARANCE)) {
        NodeAppearance *appearanceNode = (NodeAppearance *) node;
        Node *nmaterial = NULL;
        if (appearanceNode)
            nmaterial = appearanceNode->material()->getValue();
        if (nmaterial != NULL) {
            MoveCommand *command = new MoveCommand(nmaterial, 
                  appearanceNode, appearanceNode->material_Field(), NULL, -1);
            command->execute();
        }
    }
    return true;     
}

static bool removeDefName(Node *node, void *data)
{
    if (node->hasName()) 
        if ((!node->hasInputs()) && (!node->hasOutputs())) {
            // ignore nodes with routes
            node->setName("");
            NodeUpdate *hint= new NodeUpdate(node, NULL, 0);
            node->getScene()->UpdateViews(NULL, UPDATE_NODE_NAME, (Hint*) hint);
        }
    return true;     
}

class searchData {
public:
    searchData(const char* c, Scene *s) {
        compare = c;
        scene = s;
    }
    const char* compare;
    Scene *scene;
};

static bool searchNode(Node *node, void *data)
{
    const char *searchString = ((searchData *)data)->compare;
    const char *compareString = NULL;
    if (node->hasName()) 
        compareString = node->getName();
    if (compareString != NULL)
        if (strstr(compareString, searchString) != NULL) {
            ((searchData *)data)->scene->setSelection(node);
            ((searchData *)data)->scene->UpdateViews(NULL, UPDATE_SELECTION);
            return false;
        }
    compareString = node->getProto()->getName(node->getScene()->isX3d());
    if (compareString != NULL)
        if (strstr(compareString, searchString) != NULL) {
            ((searchData *)data)->scene->setSelection(node);
            ((searchData *)data)->scene->UpdateViews(NULL, UPDATE_SELECTION);
            return false;
        }
    return true;     
}

static bool getAllNodes(Node *node, void *data)
{
    NodeArray *nodeArray = (NodeArray *) data;
    nodeArray->append(node);
    return true;     
}

// validate callback for OneText dialog

static bool textValidate(MyString text)
{
    return true;
}

void
MainWindow::OnCommand(void *vid)
{
    int id = swFromPtr(vid);
    // start command
    m_scene->addNextCommand();
    bool exitFlag = false;
    Node *selectionNode = NULL;
    bool vertexModifier_active;
    float vertexModifier_radius = 0.5;
    float vertexModifier_amount = 0.5;
    switch (id) {
      case ID_DUNE_FILE_OPEN:
        OnFileOpen();
#ifndef HAVE_OPEN_IN_NEW_WINDOW
        exitFlag = true;
#endif
        break;
      case ID_DUNE_FILE_HTML_OPEN:
        OnFileOpenHtml();
#ifndef HAVE_OPEN_IN_NEW_WINDOW
        exitFlag = true;
#endif
        break;
      case ID_DUNE_FILE_URL_OPEN:
        OnFileOpenUrl();
        break;
      case ID_DUNE_FILE_IMPORT:
        TheApp->importingDataFile(true);
        OnFileImport();
        m_scene->getRoot()->doWithBranch(searchVrmlCut, m_parentWindow);
        TheApp->importingDataFile(false);
        UpdateToolbars();
        break;
      case ID_DUNE_FILE_NEW:
        if (isEditorInUse(true))
            return;
        m_vrmlCutNode = NULL;
        UpdateToolbars();
        TheApp->OnFileNew();
        exitFlag = true;
        break;
      case ID_DUNE_FILE_NEW_X3DV:
        if (isEditorInUse(true))
            return;
        m_vrmlCutNode = NULL;
        UpdateToolbars();
        TheApp->OnFileNew(ID_DUNE_FILE_NEW_X3DV);
        exitFlag = true;
        break;
      case ID_DUNE_FILE_NEW_X3D_XML:
        if (isEditorInUse(true))
            return;
        m_vrmlCutNode = NULL;
        UpdateToolbars();
        TheApp->OnFileNew(ID_DUNE_FILE_NEW_X3D_XML);
        exitFlag = true;
        break;
      case ID_DUNE_FILE_NEW_WINDOW:
        TheApp->OnFileNewWindow();
        break;
      case ID_DUNE_FILE_PREVIEW:
        m_scene->setUpdateViewsSelection(false);
        TheApp->OnFilePreview(m_scene);
        return;
      case ID_DUNE_FILE_SAVE:
        OnFileSave();
        break;
      case ID_DUNE_FILE_SAVE_AS:
        OnFileSaveAs(m_scene->isX3dv() ? X3DV : 0);
        break;
      case ID_DUNE_FILE_EXPORT_X3D_XML:
        OnFileExportX3DXML();
        break;
      case ID_DUNE_FILE_EXPORT_X3D_4_WONDERLAND:
        OnFileExportX3D4Wonderland();
        break;
      case ID_DUNE_FILE_EXPORT_VRML97:
        OnFileExportVRML97();
        break;
      case ID_DUNE_FILE_EXPORT_X3DV:
        OnFileExportX3DV();
        break;
      case ID_DUNE_FILE_EXPORT_VRML97_COVER:
        OnFileExportCover();
        break;
      case ID_DUNE_FILE_EXPORT_KANIM:
        OnFileExportKanim();
        break;
      case ID_DUNE_FILE_EXPORT_X3DOM:
        OnFileExportX3dom();
        break;
      case ID_DUNE_FILE_EXPORT_XITE:
        OnFileExportXite();
        break;
      case ID_DUNE_FILE_EXPORT_C_SOURCE:
        OnFileExportC();
        break;
      case ID_DUNE_FILE_EXPORT_CC_SOURCE:
        OnFileExportCC();
        break;
      case ID_DUNE_FILE_EXPORT_JAVA_SOURCE:
        OnFileExportJava();
        break;
      case ID_DUNE_FILE_EXPORT_WONDERLAND_5_MODULE:
        OnFileExportWonderlandModule();
        break;
      case ID_DUNE_FILE_EXPORT_AC3D:
        OnFileExportAc3d();
        break;
      case ID_DUNE_FILE_EXPORT_AC3D_4_RAVEN:
        OnFileExportAc3d4Raven();
        break;
      case ID_DUNE_FILE_EXPORT_RIB:
        OnFileExportRib();
        break;
      case ID_DUNE_FILE_EXPORT_GEO_CATT:
        OnFileExportCattGeo();
        break;
      case ID_DUNE_FILE_EXPORT_LDRAW_DAT:
        OnFileExportLdrawDat();
        break;
#ifdef HAVE_LIBCGAL
      case ID_DUNE_FILE_EXPORT_OFF:
        OnFileExportOff();
        break;
      case ID_DUNE_FILE_IMPORT_OFF:
        OnFileImportOff();
        break;
#endif
      case ID_DUNE_FILE_CLOSE:
        if (isEditorInUse(true))
            return;
        m_scene->setUpdateViewsSelection(false);
        TheApp->OnFileClose(this);
        exitFlag = true;
        break;
      case ID_DUNE_FILE_TEXT_EDIT:
        if (isEditorInUse(true))
            return;
        m_scene->setUpdateViewsSelection(false);
        TheApp->OnFileEdit(this,m_scene);
        exitFlag = true;
        break;
      case ID_DUNE_FILE_UPLOAD:
        if (!TheApp->hasUpload())
            return;
        TheApp->OnFileUpload(m_scene);
        break;
      case ID_DUNE_APP_EXIT:
        if (isEditorInUse(true))
            return;
        m_scene->setUpdateViewsSelection(false);
        TheApp->OnFileExit();
        exitFlag = true;
        break;

      case ID_DUNE_EDIT_CUT:
#ifdef HAVE_CUT
        OnEditCut();
#endif
        break;
      case ID_DUNE_EDIT_COPY:
        OnEditCopy();
        break;
      case ID_DUNE_EDIT_COPY_BRANCH_TO_ROOT:
        OnEditCopyBranchToRoot();
        break;
      case ID_DUNE_EDIT_PASTE:
        OnEditPaste();
        break;
      case ID_DUNE_EDIT_PASTE_SYMMETRIC_X:
        OnEditPasteSymetric(0);
        break;
      case ID_DUNE_EDIT_PASTE_SYMMETRIC_Y:
        OnEditPasteSymetric(1);
        break;
      case ID_DUNE_EDIT_PASTE_SYMMETRIC_Z:
        OnEditPasteSymetric(2);
        break;
      case ID_DUNE_EDIT_DELETE:
        OnEditDelete();
        break;
      case ID_DUNE_EDIT_UNDO:
        selectionNode = m_scene->getSelection()->getNode();
        vertexModifier_active = m_vertexModifier_active;
        if (m_scene->getVertexModifier()) {
            vertexModifier_radius = m_scene->getVertexModifier()->getRadius();
            vertexModifier_amount = m_scene->getVertexModifier()->getAmount();
        }
        if (m_scene->canUndo()) m_scene->undo();
        m_scene->UpdateViews(NULL, UPDATE_ALL);
        if (selectionNode->isInScene(m_scene)) {
            m_scene->setSelection(selectionNode);
            m_vertexModifier_active = !vertexModifier_active;
            updateVertexModifier();
        } else
            m_scene->setSelection(m_scene->getRoot());
        m_vertexModifier_overwrite = true;
        if (m_scene->getVertexModifier()) {
            m_scene->getVertexModifier()->setRadius(vertexModifier_radius);
            m_scene->getVertexModifier()->setAmount(vertexModifier_amount);
        }
        m_scene->UpdateViews(NULL, UPDATE_SELECTION);
        m_vertexModifier_overwrite = false;
        break;
      case ID_DUNE_EDIT_REDO:
        selectionNode = m_scene->getSelection()->getNode();
        vertexModifier_active = m_vertexModifier_active;
        if (m_scene->getVertexModifier()) {
            vertexModifier_radius = m_scene->getVertexModifier()->getRadius();
            vertexModifier_amount = m_scene->getVertexModifier()->getAmount();
        }
        if (m_scene->canRedo()) m_scene->redo();
        if (selectionNode->isInScene(m_scene)) {
            m_scene->setSelection(selectionNode);
            m_vertexModifier_active = !vertexModifier_active;
            updateVertexModifier();
        } else
            m_scene->setSelection(m_scene->getRoot());
        m_vertexModifier_overwrite = true;
        if (m_scene->getVertexModifier()) {
            m_scene->getVertexModifier()->setRadius(vertexModifier_radius);
            m_scene->getVertexModifier()->setAmount(vertexModifier_amount);
        }
        m_scene->UpdateViews(NULL, UPDATE_SELECTION);
        m_vertexModifier_overwrite = false;
        break;
      case ID_DUNE_EDIT_DEF:
          {
            Node *selection = m_scene->getSelection()->getNode();
            if (selection->getType() == VRML_COMMENT)
                break;
            if (selection != m_scene->getRoot()) {
                if (TheApp->getCoverMode()) {
                    CoverDefDialog dlg(m_wnd, m_scene);
                    dlg.DoModal();
                } else {
                    DefDialog dlg(m_wnd, m_scene);
                    dlg.DoModal();
                }
                m_scene->UpdateViews(NULL, UPDATE_SELECTION_NAME);
            }
          }
        break;
      case ID_DUNE_EDIT_USE:
          {
            Node *selectionNode = m_scene->getSelection()->getNode();
            int selectionField = m_scene->getSelection()->getField();
            if (m_scene->canUse(selectionNode, selectionField))
                if (!m_scene->use(selectionNode, selectionField))
                    TheApp->MessageBoxId(IDS_RESCURSIVE_USE);
          }
        break;
      case ID_DUNE_EDIT_FIND:
        OnEditFind();
        break;
      case ID_DUNE_EDIT_FIND_AGAIN:
        OnEditFindAgain();
        break;

      case ID_DUNE_SELECTION_VERTICES:
        setSelectionMode(SELECTION_MODE_VERTICES);
        UpdateToolbars();
        break;
      case ID_DUNE_SELECTION_FACES:
        setSelectionMode(SELECTION_MODE_FACES);
        UpdateToolbars();
        break;
      case ID_DUNE_SELECTION_LINES:
        setSelectionMode(SELECTION_MODE_LINES);
        UpdateToolbars();
        break;
      case ID_DUNE_SELECTION_HANIM_JOINT_WEIGHT:
        setSelectionMode(SELECTION_HANIM_JOINT_WEIGHT);
        UpdateToolbars();
        m_scene->UpdateViews(NULL, UPDATE_REDRAW_3D);        
        break;
      case ID_DUNE_SELECTION_RANGE:
        if (m_scene->getLastSelectedHandle() == -1)
            TheApp->MessageBoxId(IDS_SELECT_FIRST_VERTEX);
        else {
             m_scene->setfirstSelectionRangeHandle();
             static char message[256];
             swLoadString(IDS_SELECT_NEXT_VERTEX, message, 255);
             m_statusText = message;
             m_statusBar->SetText(message);
        }
        break;
      case ID_DUNE_SELECTION_NEIGHBOUR:
        selectNeighbours();
        break;
      case ID_DUNE_SELECTION_NEIGHBOUR_U:
        selectNeighbours(true);
        break;
      case ID_DUNE_SELECTION_NEIGHBOUR_V:
        selectNeighbours(false);
        break;
      case ID_DUNE_GET_OLD_SELECTION:
        m_scene->restoreSelectedHandles();
        m_scene->UpdateViews(NULL, UPDATE_REDRAW_3D);        
        break;
      case ID_DUNE_DESELECT:
        m_scene->toggleDeselect();
        swMenuSetFlags(m_menu, ID_DUNE_DESELECT, SW_MENU_CHECKED, 
                       m_scene->getDeselect() ? SW_MENU_CHECKED : 0);
        break;
      case ID_DUNE_SHOW_ONLY_SELECTED_VERTICES:
        m_scene->showOnlySelectedVertices();
        m_scene->UpdateViews(NULL, UPDATE_REDRAW_3D);        
        break;
      case ID_DUNE_HIDE_VERTICES:
        m_scene->hideSelectedVertices();
        m_scene->UpdateViews(NULL, UPDATE_REDRAW_3D);        
        break;
      case ID_DUNE_UNHIDE_VERTICES:
        m_scene->unHideVertices();
        m_scene->UpdateViews(NULL, UPDATE_REDRAW_3D);        
        break;
      case ID_DUNE_SELECTION_STORE4CONVEX_HULL:
        m_scene->addToStore4Convex_hull();
        break;
      case ID_DUNE_DELETE_STORE4CONVEX_HULL:
        m_scene->removeStore4Convex_hull();
        break;

      case ID_ANIMATION:
        createAnimation();
        break;
      case ID_CURVE_ANIMATION:
        createCurveAnimation();
        break;
      case ID_ANIMATION_OR_CURVE_ANIMATION:
        {
        Node *node = m_scene->getSelection()->getNode();
        if (node && (node->getType() == VRML_TRANSFORM)) 
            createCurveAnimation();
        else
            createAnimation();
        }
        break;
      case ID_INTERACTION:
        createInteraction();
        break;

      case ID_TWO_SIDED:
        {
        Node *node = m_scene->getSelection()->getNode();
        node->toggleDoubleSided();
        UpdateToolbars();
        NodeUpdate *hint= new NodeUpdate(node, NULL, 0);
        m_scene->UpdateViews(NULL, UPDATE_SOLID_CHANGED, (Hint*) hint);
        m_scene->UpdateViews(NULL, UPDATE_REDRAW, NULL);
        }
        break;
      case ID_CHANGE_SIDE:
        m_scene->getSelection()->getNode()->flipSide();
        m_scene->UpdateViews(NULL, UPDATE_REDRAW, NULL);
        break;
      case ID_SKIP_MATERIAL_NAME_AFTER_LAST_UNDERSCORE:
      case ID_SKIP_MATERIAL_NAME_BEFORE_FIRST_UNDERSCORE:
        toogleMaterialName(id);
        break;
      case ID_HANDLE_SCALE:
        HandleScale();
        break;
      case ID_HANDLE_SIZE:
        HandleSize();
        break;
      case ID_HANDLE_EPSILON:
        HandleEpsilon();
        break;

      case ID_X_SYMETRIC:
        if (TheApp->GetXSymetricMode())
           TheApp->SetXSymetricMode(false);
        else
           TheApp->SetXSymetricMode(true);
        setXSymetricNurbsIcon();
        break;
      case ID_OBJECT_EDIT:
        EditObject();
        break;
      case ID_URL_EDIT:
        EditUrl();
        break;
      case ID_SEARCH_INTERPOLATOR:
        searchInterpolator();
        break;
      case ID_SEARCH_TIME_SENSOR:
        searchTimeSensor();
        break;
      case ID_CHANGE_ENABLE_ANIMATION:
        changeEnableAnimation();
        break;
      case ID_CHANGE_DISABLE_ANIMATION:
        changeDisableAnimation();
        break;
      case ID_CHANGE_ANIMATION_TIME:
        changeAnimationTime();
        break;
      case ID_DELETE_ANIMATION_DATA:
        deleteAnimationData();
        break;
      case ID_CHANGE_COLOR_PER_VERTEX:
        showColorPerVertexColorCircle();
        break;
      case ID_CHANGE_MATERIAL_DIFFUSE:
        showDiffuseColorCircle();
        break;
      case ID_CHANGE_MATERIAL_EMISSIVE:
        showEmissiveColorCircle();
        break;
      case ID_CHANGE_MATERIAL_SPECULAR:
        showSpecularColorCircle();
        break;
      case ID_CHANGE_MATERIAL_TRANSPARENCY:
        changeTransparency();
        break;
      case ID_CHANGE_MATERIAL_SHININESS:
        changeShininess();
        break;
      case ID_CHANGE_IMAGE_REPEAT:
        changeImageRepeat();
        break;
      case ID_CREATE_AT_ZERO_X:
        toggleCreateAtZero(0);
        break;
      case ID_CREATE_AT_ZERO_Y:
        toggleCreateAtZero(1);
        break;
      case ID_CREATE_AT_ZERO_Z:
        toggleCreateAtZero(2);
        break;
      case ID_NEW_ONE_TEXT:
        createOneText();
        break;
      case ID_CHANGE_ONE_TEXT:
        changeOneText();
        break;

      case ID_MOVE_SIBLING_UP:
        moveSiblingUp();
        break;
      case ID_MOVE_SIBLING_DOWN:
        moveSiblingDown();
        break;
      case ID_MOVE_SIBLING_FIRST:
        moveSiblingFirst();
        break;
      case ID_MOVE_SIBLING_LAST:
        moveSiblingLast();
        break;

      case ID_BRANCH_TO_PARENT:
        moveBranchToParent();
        break;
      case ID_BRANCH_TO_TRANSFORM_SELECTION:
        moveBranchToTransformSelection();
        break;
      case ID_BRANCH_TO_GROUP:
        moveBranchTo("Group", "children");
        break;
      case ID_BRANCH_TO_TRANSFORM:
        moveBranchTo("Transform", "children");
        break;
      case ID_BRANCH_TO_COLLISION:
        moveBranchTo("Collision", "children");
        break;
      case ID_BRANCH_TO_COLLISIONPROXY:
        moveBranchTo("Collision", "proxy");
        break;
      case ID_BRANCH_TO_ANCHOR:
        moveBranchTo("Anchor", "children");
        break;
      case ID_BRANCH_TO_BILLBOARD:
        moveBranchTo("Billboard", "children");
        break;
      case ID_BRANCH_TO_LOD   :
        moveBranchTo("LOD", "level");
        break;
      case ID_BRANCH_TO_SWITCH:
        moveBranchTo("Switch", "choice");
        break;
      case ID_BRANCH_TO_INLINE:
        moveBranchToInline();
        break;
      case ID_BRANCH_TO_COLLISION_SPACE:
        moveBranchTo("CollisionSpace", "collidables");
        break;
     case ID_BRANCH_TO_VRML_SCENE_BEGIN:
        moveBranchToVrmlScene(true);
        break;
     case ID_BRANCH_TO_VRML_SCENE_END:
        moveBranchToVrmlScene(false);
        break;
      case ID_BRANCH_TO_PROTO:
        moveBranchToProto();
        break;

      case ID_BRANCH_CREATE_NORMAL:
        m_scene->branchCreateNormals(m_scene->getSelection()->getNode());
        break;
      case ID_BRANCH_CREATE_TEXTURE_COORDINATE:
        m_scene->branchCreateTextureCoordinates(
              m_scene->getSelection()->getNode());
        break;
      case ID_BRANCH_CREATE_TEXTURE:
        createBranchImageTexture();
        break;
      case ID_BRANCH_CREATE_MATERIAL:
        createBranchMaterial();
        break;
      case ID_BRANCH_CREATE_APPEARANCE:
        createBranchAppearance();
        break;
      case ID_BRANCH_SET_FLIP_SIDE:
        setBranchFlipSide();
        m_scene->UpdateViews(NULL, UPDATE_REDRAW, NULL);
        break;
      case ID_BRANCH_SET_CONVEX:
        setBranchConvex();
        break;
      case ID_BRANCH_SET_SOLID:
        setBranchSolid();
        break;
      case ID_BRANCH_SET_TWO_SIDED:
        setBranchTwoSided();
        break;
      case ID_BRANCH_SET_CREASE_ANGLE:
        setBranchCreaseAngle();
        break;
      case ID_BRANCH_SET_TRANSPARENCY:
        setBranchTransparency();
        break;
      case ID_BRANCH_SET_SHININESS:
        setBranchShininess();
        break;
      case ID_BRANCH_CONVERT_TO_TRIANGLESET:
        branchConvertToTriangleSetUpdate();
        break;
      case ID_BRANCH_CONVERT_TO_INDEXED_TRIANGLESET:
        branchConvertToIndexedTriangleSetUpdate();
        break;
      case ID_BRANCH_CONVERT_TO_INDEXED_FACESET:
        branchConvertToIndexedFaceSetUpdate();
        break;
      case ID_BRANCH_REMOVE_NORMAL:
        doWithBranchUpdate(removeNormals, NULL);
        break;
      case ID_BRANCH_REMOVE_TEXTURE_COORDINATE:
        doWithBranchUpdate(removeTextureCoordinates, NULL);
        break;
      case ID_BRANCH_REMOVE_COLOR:
        doWithBranchUpdate(removeColors, NULL);
        break;
      case ID_BRANCH_REMOVE_TEXTURE:
        doWithBranchUpdate(removeTexture, NULL);
        break;
      case ID_BRANCH_REMOVE_MATERIAL:
        doWithBranchUpdate(removeMaterial, NULL);
        break;
      case ID_BRANCH_REMOVE_APPEARANCE:
        doWithBranch(removeAppearance, NULL);
        break;
      case ID_BRANCH_REMOVE_DEF_NAME:
        doWithBranch(removeDefName, NULL);
        m_scene->UpdateViews(NULL, UPDATE_REDRAW, NULL);
        break;
      case ID_BRANCH_REMOVE_NODE:
        removeBranchNode();
        break;
      case ID_BRANCH_FIELD_PIPE:
        branchFieldPipe();
        break;
      case ID_BRANCH_BUILD_RIGID_BODY_COLLECTION:
        branchBuildRigidBodyCollection();
        break;
      case ID_BRANCH_COUNT_POLYGONS:
        countPolygons();
        break;
      case ID_BRANCH_COUNT_POLYGONS_4_CATT:
        countPolygons4catt();
        break;
      case ID_BRANCH_OPTIMIZE:
        branchOptimize();
        break;
#ifdef HAVE_LIBCGAL
      case ID_BRANCH_CSG_UNION:
        branchCSGUnion();
        break;
#endif
      case ID_BRANCH_SET_BBOX:
        m_scene->branchSetBbox();
        break;
      case ID_VERTEX_MODIFIER:
        showVertexModifier();
        break;
      case ID_CENTER_TO_MID:
        centerToMid();
        break;
      case ID_CENTER_TO_MINX:
        centerToMin(0);
        break;
      case ID_CENTER_TO_MINY:
        centerToMin(1);
        break;
      case ID_CENTER_TO_MINZ:
        centerToMin(2);
        break;
      case ID_CENTER_TO_MAXX:
        centerToMax(0);
        break;
      case ID_CENTER_TO_MAXY:
        centerToMax(1);
        break;
      case ID_CENTER_TO_MAXZ:
        centerToMax(2);
        break;
      case ID_ARRAY:
        insertArray();
        break;
      case ID_FLIP_X:
        flip(0);
        m_scene->UpdateViews(NULL, UPDATE_REDRAW, NULL);
        break;
      case ID_FLIP_Y:
        flip(1);
        m_scene->UpdateViews(NULL, UPDATE_REDRAW, NULL);
        break;
      case ID_FLIP_Z:
        flip(2);
        m_scene->UpdateViews(NULL, UPDATE_REDRAW, NULL);
        break;
      case ID_SWAP_XY:
        swap(SWAP_XY);
        m_scene->UpdateViews(NULL, UPDATE_REDRAW, NULL);
        break;
      case ID_SWAP_XZ:
        swap(SWAP_XZ);
        m_scene->UpdateViews(NULL, UPDATE_REDRAW, NULL);
        break;
      case ID_SWAP_YZ:
        swap(SWAP_YZ);
        m_scene->UpdateViews(NULL, UPDATE_REDRAW, NULL);
        break;
      case ID_SET_DEFAULT:
        setDefault();
        break;
      case ID_MOVE_TO_X:
        moveTo(0);
        break;
      case ID_MOVE_TO_Y:
        moveTo(1);
        break;
      case ID_MOVE_TO_Z:
        moveTo(2);
        break;
      case ID_SCALE_BY_X:
        scaleBy(0);
        break;
      case ID_SCALE_BY_Y:
        scaleBy(1);
        break;
      case ID_SCALE_BY_Z:
        scaleBy(2);
        break;
      case ID_SAME_MAX_X:
        same(0, SAME_MAX);
        break;
      case ID_SAME_MAX_Y:
        same(1, SAME_MAX);
        break;
      case ID_SAME_MAX_Z:
        same(2, SAME_MAX);
        break;
      case ID_SAME_X:
        same(0, SAME);
        break;
      case ID_SAME_Y:
        same(1, SAME);
        break;
      case ID_SAME_Z:
        same(2, SAME);
        break;
      case ID_SAME_MIN_X:
        same(0, SAME_MIN);
        break;
      case ID_SAME_MIN_Y:
        same(1, SAME_MIN);
        break;
      case ID_SAME_MIN_Z:
        same(2, SAME_MIN);
        break;
      case ID_REMOVE_ILLEGAL_NODES:
        removeIllegalNodes();
        break;
      case ID_REBUILD_NURBS_CONTROL_POINTS:
        rebuildControlPoints();
        break;
      case ID_MAKE_NURBS_SURFACE_SYMETRIC_X:
        makeNurbSurfaceSymetric(0);
        break;
      case ID_MAKE_NURBS_SURFACE_SYMETRIC_Y:
        makeNurbSurfaceSymetric(1);
        break;
      case ID_MAKE_NURBS_SURFACE_SYMETRIC_Z:
        makeNurbSurfaceSymetric(2);
        break;
      case ID_INSERT_TO_NURBS_CURVE:
        insertToNurbsCurve();
        break;
      case ID_INSERT_TO_NURBS_SURFACE:
        insertToNurbsSurface();
        break;
      case ID_DEGREE_ELEVATE_UP:
        degreeElevate(1);
        break;
      case ID_DEGREE_ELEVATE_DOWN:            
        degreeElevate(-1);
        break;
      case ID_U_DEGREE_ELEVATE_UP:            
        uDegreeElevate(1);
        break;
      case ID_U_DEGREE_ELEVATE_DOWN:
        uDegreeElevate(-1);
        break;
      case ID_V_DEGREE_ELEVATE_UP:  
        vDegreeElevate(1);
        break;
      case ID_V_DEGREE_ELEVATE_DOWN:
        vDegreeElevate(-1);
        break;
      case ID_LINEAR_UKNOT:
        linearUknot();
        break;
      case ID_LINEAR_VKNOT:
        linearVknot();
        break;
      case ID_SET_WEIGHTS_TO_1:
        setWeightsTo1();
        break;
      case ID_HANIM_JOINT_WEIGHT_SET:
        setHAnimJointWeight();
        break;
      case ID_HANIM_JOINT_WEIGHT_REMOVE:
        removeHAnimJointWeight();
        break;
      case ID_PIPE:
        pipeField();
        break;
      case ID_TIME_SHIFT:
        timeShift();
        break;
      case ID_COLOR_CIRCLE:
        updateColorCircle();
        break;
      case ID_TOGGLE_XRAY_RENDERING:
        toggleXrayRendering();
        break;
      case ID_SET_URL:
        setPathAllURLs();
        break;
      case ID_SHOW_JOINT_LIKE_HANDLES:
        if (m_scene->hasJoints()) {
            if (m_scene->showJoints())
                m_scene->setShowJoints(false);
            else
                m_scene->setShowJoints(true);
        }
        m_scene->UpdateViews(NULL, UPDATE_ALL, NULL);    
        break;
#ifdef HAVE_LIBCGAL
      case ID_CONVEX_HULL:
        convexHull();
        break;
#endif
      case ID_VIEWPOINT:
        setViewpoint();
        break;

      case ID_SET_OPTIMIZE:
        optimizeSet();
        break;
      case ID_TRIANGULATE:
        triangulate();
        break;
      case ID_SUBDIVIDE:
        subdivide();
        break;
      case ID_SPLIT_FACES:
        splitFaces();
        break;
      case ID_BUILD_QUAD:
        buildQuad();
        break;
      case ID_EXTRUDE:
        extrude();
        break;
      case ID_INSET_FACE:
        insetFace();
        break;
      case ID_SPLIT_INTO_PIECES:
        splitIntoPieces();
        break;
#ifdef HAVE_LIBVCG
      case ID_MESH_REDUCE:
        meshReduce();
        break;
#endif
      case ID_MAKE_SYMETRIC_X:
        makeSymetric(0);
        break;
      case ID_MAKE_SYMETRIC_Y:
        makeSymetric(1);
        break;
      case ID_MAKE_SYMETRIC_Z:
        makeSymetric(2);
        break;
      case ID_SNAP_TOGETHER:
        snapTogether();
        break;
#ifdef HAVE_LIBCGAL
      case ID_CSG_SUBTRACT:
        csg(SUBTRACT);
        break;
      case ID_CSG_UNION:
        csg(UNION);
        break;
      case ID_CSG_INTERSECTION:
        csg(INTERSECTION);
        break;
#endif
      case ID_SIMPLE_JOIN:
        simpleJoin();
        break;

      case ID_TO_NURBS:
      case ID_TO_NURBS_4KIDS:
        toNurbs();
        break;
      case ID_TO_NURBS_TRIMMED:
        toNurbsTrimmedSurface();
        break;
      case ID_TO_NURBS_CURVE:
        toNurbsCurve();
        break;
      case ID_TO_SUPER_EXTRUSION:
        toSuperExtrusion();
        break;
      case ID_TO_SUPER_REVOLVER:
        toSuperRevolver();
        break;
      case ID_TO_NURBS_POSITION_INTERPOLATOR:
        toNurbsPositionInterpolator();
        break;
      case ID_TO_NURBS_ORIENTATION_INTERPOLATOR:
        toNurbsOrientationInterpolator();
        break;
      case ID_TO_EXTRUSION:
        toExtrusion();
        break;
      case ID_TO_INDEXED_FACESET:
      case ID_TO_INDEXED_FACESET_4KIDS:
        toIndexedFaceSet();
        break;
      case ID_TO_INDEXED_TRIANGLESET:
        toIndexedTriangleSet();
        break;
      case ID_TO_TRIANGLESET:
        toTriangleSet();
        break;
      case ID_TO_INDEXED_LINESET:
        toIndexedLineSet();
        break;
      case ID_TO_POINTSET:
        toPointSet();
        break;
      case ID_TO_CURVE_ANIMATION:
        toCurveAnimation();
        break;
      case ID_TO_POSITION_AND_ROTATION_INTERPOLATORS:
        toPositionAndOrientationInterpolators();
        break;
      case ID_TO_POSITION_INTERPOLATOR:
        toPositionInterpolator();
        break;
      case ID_TO_ORIENTATION_INTERPOLATOR_XZ:
        toOrientationInterpolator(XZ_DIRECTION);
        break;
      case ID_TO_ORIENTATION_INTERPOLATOR_YZ:
        toOrientationInterpolator(YZ_DIRECTION);
        break;
      case ID_TO_ORIENTATION_INTERPOLATOR_XY:
        toOrientationInterpolator(XY_DIRECTION);
        break;

      case ID_MOVE_MODE:
        setTMode(TM_TRANSLATE);
        break;
      case ID_ROTATE_MODE:
        setTMode(TM_ROTATE);
        break;
      case ID_SCALE_MODE:
        setTMode(TM_SCALE);
        break;
      case ID_UNIFORM_SCALE_MODE:
        setTMode(TM_UNIFORM_SCALE);
        break;
      case ID_CENTER_MODE:
        setTMode(TM_CENTER);
        break;
      case ID_6D_MODE:
        setTMode(TM_6D);
        break;
      case ID_6DLOCAL_MODE:
        setTMode(TM_6DLOCAL);
        break;
      case ID_ROCKET_MODE:
        setTMode(TM_ROCKET);
        break;
      case ID_HOVER_MODE:
        setTMode(TM_HOVER);
        break;
      case ID_X_ONLY:
        m_xOnly = !m_xOnly;
        m_scene->setXonly(m_xOnly);
        UpdateToolbar(ID_X_ONLY);
        m_scene->UpdateViews(NULL, UPDATE_MODE);
        break;
      case ID_Y_ONLY:
        m_yOnly = !m_yOnly;
        m_scene->setYonly(m_yOnly);
        UpdateToolbar(ID_Y_ONLY);
        m_scene->UpdateViews(NULL, UPDATE_MODE);
        break;
      case ID_Z_ONLY:
        m_zOnly = !m_zOnly;
        m_scene->setZonly(m_zOnly);
        UpdateToolbar(ID_Z_ONLY);
        m_scene->UpdateViews(NULL, UPDATE_MODE);
        break;
      case ID_3D_MODE:
        setTDimension(TM_3D);
        break;
      case ID_2D_MODE:
        setTDimension(TM_2D);
        break;
      case ID_1D_MODE:
        setTDimension(TM_1D);
        break;
      case ID_NEAR_FAR_MODE:
        setT2axes(TM_NEAR_FAR);
        break;
      case ID_UP_DOWN_MODE:
        setT2axes(TM_UP_DOWN);
        break;
      case ID_INPUTDEVICE_GREATER:
        TheApp->increaseInputDevice(m_scene->getTransformMode());
        break;
      case ID_INPUTDEVICE_LESSER:
        TheApp->decreaseInputDevice(m_scene->getTransformMode());
        break;
      case ID_INPUT_DEVICE_NAVIGATION_MODE:
        if (m_navigation_input_device_active)
           m_navigation_input_device_active=false;
        else
           m_navigation_input_device_active=true;
        m_scene->setInputDeviceNavigationMode(m_navigation_input_device_active);
        UpdateToolbar(ID_INPUT_DEVICE_NAVIGATION_MODE);
        break;
      case ID_MOUSE_NAVIGATION_MODE:
        if (m_navigation_mouse_active)
           m_navigation_mouse_active=false;
        else
           m_navigation_mouse_active=true;
        m_scene->setMouseNavigationMode(m_navigation_mouse_active);
        UpdateToolbar(ID_NAVIGATION_OPERATIONS);
        break;
      case ID_STAND_UP:
        m_scene->standUpCamera();
        break;
     case ID_TARGET:
        setTarget();
        break;

      case ID_EXAMINE_MOUSE_MODE:
        TheApp->SetMouseMode(MOUSE_EXAMINE);
        UpdateToolbar(ID_NAVIGATION_OPERATIONS);
        break;      
      case ID_INCREASE_TURNPOINT:
        m_scene->changeTurnPointDistance(2.0f);
        break;
      case ID_DECREASE_TURNPOINT:
        m_scene->changeTurnPointDistance(0.5f);
        break;
      case ID_WALK_MOUSE_MODE:
        TheApp->SetMouseMode(MOUSE_WALK);
        UpdateToolbar(ID_NAVIGATION_OPERATIONS);
        break;
      case ID_FLY_MOUSE_MODE:
        TheApp->SetMouseMode(MOUSE_FLY);
        UpdateToolbar(ID_NAVIGATION_OPERATIONS);
        break;
      case ID_ROLL_MOUSE_MODE:
        TheApp->SetMouseMode(MOUSE_ROLL);
        UpdateToolbar(ID_NAVIGATION_OPERATIONS);
        break;
      case ID_FOLLOW_MOUSE_MODE:
        TheApp->SetMouseMode(MOUSE_FOLLOW);
        UpdateToolbar(ID_NAVIGATION_OPERATIONS);
        break;
      case ID_RECALIBRATE:
        m_scene->recalibrate();
        break;

      case ID_ROUTE_ZOOM_IN:
        m_graphView->zoomIn();        
        UpdateToolbars();
        break;
      case ID_ROUTE_ZOOM_OUT:
        m_graphView->zoomOut();        
        UpdateToolbars();
        break;
      case ID_ROUTE_UNZOOM:
        m_graphView->unZoom();        
        UpdateToolbars();
        break;
      case ID_ROUTE_REBUILD:
        ShowView(PW_TOP, m_graphView, ID_DUNE_VIEW_SCENE_GRAPH, "ShowRouteView");
        m_graphView->Initialize();
        UpdateToolbars();
        break;
      case ID_ROUTE_MOVE_UP:
        ShowView(PW_TOP, m_graphView, ID_DUNE_VIEW_SCENE_GRAPH, "ShowRouteView");
        m_graphView->moveToTop(m_scene->getSelection()->getNode());
        UpdateToolbars();
        break;
      case ID_ROUTE_MOVE_UP_ROUTES:
        ShowView(PW_TOP, m_graphView, ID_DUNE_VIEW_SCENE_GRAPH, "ShowRouteView");
        m_graphView->moveRoutesToTop(m_scene->getSelection()->getNode());
        UpdateToolbars();
        break;

      case ID_DUNE_VIEW_FULL_SCREEN:
        toggleFullScreen();
        break;
      case ID_DUNE_VIEW_SCENE_GRAPH:
        toggleView(PW_TOP, m_graphView, ID_DUNE_VIEW_SCENE_GRAPH, 
                   "ShowRouteView");
        UpdateToolbars();
        break;
      case ID_DUNE_VIEW_SCENE_TREE:
        toggleView(PW_LEFT, m_treeView, ID_DUNE_VIEW_SCENE_TREE, 
                   "ShowSceneTree");
        break;
      case ID_DUNE_VIEW_FIELD_VIEW:
        toggleView(PW_RIGHT, m_fieldView, ID_DUNE_VIEW_FIELD_VIEW, 
                   "ShowFieldView");
        break;
      case ID_DUNE_VIEW_FIELD_VIEW_4_KIDS:
        m_showNumbers4Kids = !m_showNumbers4Kids;
        UpdateNumbers4Kids();
        swMenuSetFlags(m_menu, ID_DUNE_VIEW_FIELD_VIEW_4_KIDS, SW_MENU_CHECKED, 
                       m_showNumbers4Kids ? SW_MENU_CHECKED : 0);
        break;
      case ID_DUNE_VIEW_CHANNEL_VIEW:
        toggleView(PW_BOTTOM, m_channelView, ID_DUNE_VIEW_CHANNEL_VIEW,
                   "ShowChannelView");
        break;
      case ID_DUNE_VIEW_TOOLBAR:
        toggleToolbar(m_toolbarWindow, m_standardToolbar, ID_DUNE_VIEW_TOOLBAR,
                      "StandardToolbar");
        break;
      case ID_DUNE_VIEW_NODE_TOOLBAR_1:
        m_nodeToolbar1Enabled = !m_nodeToolbar1Enabled;
        toggleToolbar(m_toolbarWindow, m_nodeToolbar1, 
                      ID_DUNE_VIEW_NODE_TOOLBAR_1, "NodeToolbar1");
        break;
      case ID_DUNE_VIEW_NODE_TOOLBAR_2:
        m_nodeToolbar2Enabled=!m_nodeToolbar2Enabled;
        toggleToolbar(m_toolbarWindow, m_nodeToolbar2, 
                      ID_DUNE_VIEW_NODE_TOOLBAR_2, "NodeToolbar2");
        break;
      case ID_DUNE_VIEW_NODE_TOOLBAR_3:
        m_nodeToolbar3Enabled = !m_nodeToolbar3Enabled;
        toggleToolbar(m_toolbarWindow, m_nodeToolbar3, 
                      ID_DUNE_VIEW_NODE_TOOLBAR_3, "NodeToolbar3");
        break;
      case ID_DUNE_VIEW_NODE_TOOLBAR_VRML200X:
        m_nodeToolbarVRML200xEnabled = !m_nodeToolbarVRML200xEnabled;
        toggleToolbar(m_toolbarWindow, m_nodeToolbarVRML200x, 
                      ID_DUNE_VIEW_NODE_TOOLBAR_VRML200X, 
                      "NodeToolbarVRML200x");
        break;
      case ID_DUNE_VIEW_NODE_TOOLBAR_X3D_COMPONENTS_1:
        m_nodeToolbarX3DComponents1Enabled = !m_nodeToolbarX3DComponents1Enabled;
        toggleToolbar(m_toolbarWindow, m_nodeToolbarX3DComponents1, 
                      ID_DUNE_VIEW_NODE_TOOLBAR_X3D_COMPONENTS_1, 
                      "NodeToolbarX3DComponents1");
        break;
      case ID_DUNE_VIEW_NODE_TOOLBAR_X3D_COMPONENTS_2:
        m_nodeToolbarX3DComponents2Enabled = !m_nodeToolbarX3DComponents2Enabled;
        toggleToolbar(m_toolbarWindow, m_nodeToolbarX3DComponents2, 
                      ID_DUNE_VIEW_NODE_TOOLBAR_X3D_COMPONENTS_2, 
                      "NodeToolbarX3DComponents2");
        break;
       case ID_DUNE_VIEW_NODE_TOOLBAR_X3D_COMPONENTS_3:
        m_nodeToolbarX3DComponents3Enabled = !m_nodeToolbarX3DComponents3Enabled;
        toggleToolbar(m_toolbarWindow, m_nodeToolbarX3DComponents3, 
                      ID_DUNE_VIEW_NODE_TOOLBAR_X3D_COMPONENTS_3, 
                      "NodeToolbarX3DComponents3");
        break;
      case ID_DUNE_VIEW_NODE_TOOLBAR_X3D_COMPONENTS_4:
        m_nodeToolbarX3DComponents4Enabled = !m_nodeToolbarX3DComponents4Enabled;
        toggleToolbar(m_toolbarWindow, m_nodeToolbarX3DComponents4, 
                      ID_DUNE_VIEW_NODE_TOOLBAR_X3D_COMPONENTS_4, 
                      "NodeToolbarX3DComponents4");
        break;
      case ID_DUNE_VIEW_NODE_TOOLBAR_COVER:
        m_nodeToolbarCoverEnabled = !m_nodeToolbarCoverEnabled;
        toggleToolbar(m_toolbarWindow, m_nodeToolbarCover, 
                      ID_DUNE_VIEW_NODE_TOOLBAR_COVER, "NodeToolbarCover");
        break;
      case ID_DUNE_VIEW_NODE_TOOLBAR_KAMBI:
        m_nodeToolbarKambiEnabled = !m_nodeToolbarKambiEnabled;
        toggleToolbar(m_toolbarWindow, m_nodeToolbarKambi, 
                      ID_DUNE_VIEW_NODE_TOOLBAR_KAMBI, "NodeToolbarKambi");
        break;
      case ID_DUNE_VIEW_NODE_TOOLBAR_X3DOM:
        m_nodeToolbarX3domEnabled = !m_nodeToolbarX3domEnabled;
        toggleToolbar(m_toolbarWindow, m_nodeToolbarX3dom, 
                      ID_DUNE_VIEW_NODE_TOOLBAR_X3DOM, "NodeToolbarX3dom");
        break;
      case ID_DUNE_VIEW_NODE_TOOLBAR_SCRIPTED:
        m_nodeToolbarScriptedEnabled = !m_nodeToolbarScriptedEnabled;
        toggleToolbar(m_toolbarWindow, m_nodeToolbarScripted, 
                      ID_DUNE_VIEW_NODE_TOOLBAR_SCRIPTED, 
                      "NodeToolbarScripted");
        break;
      case ID_DUNE_VIEW_PLAY_TOOLBAR:
        toggleToolbar(m_toolbarWindow2, m_vcrToolbar, ID_DUNE_VIEW_PLAY_TOOLBAR,
                      "PlayToolbar");
        break;
      case ID_DUNE_VIEW_STATUS_BAR:
        toggleStatusbar();
        break;

      case ID_NEW_ANCHOR:
        createNode("Anchor");
        break;
      case ID_NEW_APPEARANCE:
        insertNode(VRML_APPEARANCE, "Appearance");
        break;
      case ID_NEW_ARC_2D:
        createGeometryNode("Arc2D", true);
        break;
      case ID_NEW_ARC_CLOSE_2D:
        createGeometryNode("ArcClose2D");
        break;
      case ID_NEW_AUDIO_CLIP:
        insertAudioClip();
        break;
      case ID_NEW_BACKGROUND:
        createNode("Background");
        break;
      case ID_NEW_BALL_JOINT:
        createNode("BallJoint");
        break;
      case ID_NEW_BILLBOARD:
        createNode("Billboard");
        break;
      case ID_NEW_BOOLEAN_FILTER:
        createNode("BooleanFilter");
        break;
      case ID_NEW_BOOLEAN_TOGGLE:
        createNode("BooleanToggle");
        break;
      case ID_NEW_BOOLEAN_TRIGGER:
        createNode("BooleanTrigger");
        break;
      case ID_NEW_BOOLEAN_SEQUENCER:
        createNode("BooleanSequencer");
        break;
      case ID_NEW_BOUNDED_PHYSICS_MODEL:
        insertNode(PARTICLE_PHYSICS_MODEL_NODE, "BoundedPhysicsModel");
        break;
      case ID_NEW_BOX:
        createGeometryNode("Box");
        break;
      case ID_NEW_CAD_ASSEMBLY:
        createNode("CADAssembly");
        break;
      case ID_NEW_CAD_FACE:
        insertNode(PRODUCT_STRUCTURE_CHILD_NODE, "CADFace");
        break;
      case ID_NEW_CAD_LAYER:
        createNode("CADLayer");
        break;
      case ID_NEW_CAD_PART:
        createNode("CADPart");
        break;
      case ID_NEW_CIRCLE_2D:
        createGeometryNode("Circle2D", true);
        break;
      case ID_NEW_CLIP_PLANE:
        createNode("ClipPlane");
        break;
      case ID_NEW_COLLIDABLE_OFFSET:
        createNode("CollidableOffset");
        break;
      case ID_NEW_COLLIDABLE_SHAPE:
        createNode("CollidableShape");
        break;
      case ID_NEW_COLLISION:
        createNode("Collision");
        break;
      case ID_NEW_COLLISION_COLLECTION:
        createNode("CollisionCollection");
        break;
      case ID_NEW_COLLISION_SENSOR:
        createNode("CollisionSensor");
        break;
      case ID_NEW_COLLISION_SPACE:
        createNode("CollisionSpace");
        break;
      case ID_NEW_COLOR:
        insertNode(COLOR_NODE, "Color");
        break;
      case ID_NEW_COLOR_CHASER:
        createNode("ColorChaser");
        break;
      case ID_NEW_COLOR_DAMPER:
        createNode("ColorDamper");
        break;
      case ID_NEW_COLOR_INTERPOLATOR:
        createNode("ColorInterpolator");
        break;
      case ID_NEW_COLOR_RGBA:
        insertNode(COLOR_NODE, "ColorRGBA");
        break;
      case ID_NEW_COMPOSED_CUBE_MAP_TEXTURE:
        insertNode(TEXTURE_NODE, "ComposedCubeMapTexture");
        break;
      case ID_NEW_COMPOSED_SHADER:
        insertNode(SHADER_NODE, "ComposedShader");
        break;
      case ID_NEW_COMPOSED_TEXTURE_3D:
        insertNode(TEXTURE_NODE, "ComposedTexture3D");
        break;
      case ID_NEW_CONE:
        createGeometryNode("Cone");
        break;
      case ID_NEW_CONE_EMITTER:
        insertNode(PARTICLE_EMITTER_NODE, "ConeEmitter");
        break;
      case ID_NEW_CONTACT:
        createNode("Contact");
        break;
      case ID_NEW_CONTOUR_2D:
        insertNode(VRML_CONTOUR_2D, "Contour2D");
        break;
      case ID_NEW_CONTOUR_POLYLINE_2D:
        if (m_scene->isX3d())
            insertNode(NURBS_CONTROL_CURVE_NODE, "ContourPolyline2D");
        else
            insertNode(NURBS_CONTROL_CURVE_NODE, "Polyline2D");
        break;
      case ID_NEW_COORDINATE:
        insertNode(COORDINATE_NODE, "Coordinate");
        break;
      case ID_NEW_COORDINATE_DAMPER:
        createNode("CoordinateDamper");
        break;
      case ID_NEW_COORDINATE_CHASER:
        createNode("CoordinateChaser");
        break;
      case ID_NEW_COORDINATE_DEFORMER:
        createNode("CoordinateDeformer");
        break;
      case ID_NEW_COORDINATE_DOUBLE:
        insertNode(COORDINATE_NODE, "CoordinateDouble");
        break;
      case ID_NEW_COORDINATE_INTERPOLATOR:
        createNode("CoordinateInterpolator");
        break;
      case ID_NEW_COORDINATE_INTERPOLATOR_2D:
        createNode("CoordinateInterpolator2D");
        break;
      case ID_NEW_CYLINDER:
        createGeometryNode("Cylinder");
        break;
      case ID_NEW_CYLINDER_SENSOR:
        createNode("CylinderSensor");
        break;
      case ID_NEW_DIRECTIONAL_LIGHT:
        createNode("DirectionalLight");
        break;
      case ID_NEW_DIS_ENTITY_MANAGER:
        createNode("DISEntityManager");
        break;
      case ID_NEW_DIS_ENTITY_TYPE_MAPPING:
        createNode("DISEntityTypeMapping");
        break;
      case ID_NEW_DISK_2D:
        createGeometryNode("Disk2D");
        break;
      case ID_NEW_DOUBLE_AXIS_HINGE_JOINT:
        createNode("DoubleAxisHingeJoint");
        break;
      case ID_NEW_EASE_IN_EASE_OUT:
        createNode("EaseInEaseOut");
        break;
      case ID_NEW_ELEVATION_GRID:
        createElevationGrid();
        break;
      case ID_NEW_ESPDU_TRANSFORM:
        createNode("EspduTransform");
        break;
      case ID_NEW_EXPLOSION_EMITTER:
        insertNode(PARTICLE_EMITTER_NODE, "ExplosionEmitter");
        break;
      case ID_NEW_EXTRUSION:
        createGeometryNode("Extrusion");
        break;
      case ID_NEW_FILL_PROPERTIES:
        insertNode(X3D_FILL_PROPERTIES, "FillProperties");
        break;
      case ID_NEW_FLOAT_VERTEX_ATTRIBUTE:
        insertNode(VERTEX_ATTRIBUTE_NODE, "FloatVertexAttribute");
        break;
      case ID_NEW_FOG:
        createNode("Fog");
        break;
      case ID_NEW_FOG_COORDINATE:
        insertNode(X3D_FOG_COORDINATE, "FogCoordinate");
        break;
      case ID_NEW_FONT_STYLE:
        insertNode(FONT_STYLE_NODE, "FontStyle");
        break;
      case ID_NEW_FORCE_PHYSICS_MODEL:
        insertNode(PARTICLE_PHYSICS_MODEL_NODE, "ForcePhysicsModel");
        break;
      case ID_NEW_GENERATED_CUBE_MAP_TEXTURE:
        insertNode(TEXTURE_NODE, "GeneratedCubeMapTexture");
        break;
      case ID_NEW_GEO_COORDINATE:
        insertNode(COORDINATE_NODE, "GeoCoordinate");
        break;
      case ID_NEW_GEO_ELEVATION_GRID:
        createGeoElevationGrid();
        break;
      case ID_NEW_GEO_LOCATION:
        createNode("GeoLocation");
        break;
      case ID_NEW_GEO_LOD:
        createNode("GeoLOD");
        break;
      case ID_NEW_GEO_METADATA:
        insertNode(X3D_GEO_METADATA, "GeoMetadata");
        break;
      case ID_NEW_GEO_ORIGIN:
        insertNode(VRML_GEO_ORIGIN, "GeoOrigin");
        break;
      case ID_NEW_GEO_POSITION_INTERPOLATOR:
        createNode("GeoPositionInterpolator");
        break;
      case ID_NEW_GEO_PROXIMITY_SENSOR:
        createNode("GeoProximitySensor");
        break;
      case ID_NEW_GEO_TOUCH_SENSOR:
        createNode("GeoTouchSensor");
        break;
      case ID_NEW_GEO_TRANSFORM:
        createNode("GeoTransform");
        break;
      case ID_NEW_GEO_VIEWPOINT:
        createGeoViewpoint();
        break;
      case ID_NEW_GROUP:
        createNode("Group");
        break;
      case ID_NEW_HANIM_DISPLACER:
        insertNode(X3D_HANIM_DISPLACER, "HAnimDisplacer");
        break;
      case ID_NEW_HANIM_HUMANOID:
        createNode("HAnimHumanoid");
        break;
      case ID_NEW_HANIM_JOINT:
        insertHAnimJoint();
        break;
      case ID_NEW_HANIM_SEGMENT:
        insertHAnimSegment();
        break;
      case ID_NEW_HANIM_SITE:
        insertHAnimSite();
        break;
      case ID_NEW_IMAGE_TEXTURE:
        insertImageTexture();
        break;
      case ID_NEW_IMAGE_CUBE_MAP_TEXTURE:
        insertNode(TEXTURE_NODE, "ImageCubeMapTexture");
        break;
      case ID_NEW_IMAGE_TEXTURE_3D:
        insertNode(TEXTURE_NODE, "ImageTexture3D");
        break;
      case ID_NEW_INDEXED_FACE_SET:
        createGeometryNode("IndexedFaceSet");
        break;
      case ID_NEW_INDEXED_LINE_SET:
        createGeometryNode("IndexedLineSet");
        break;
      case ID_NEW_INDEXED_QUAD_SET:
        createGeometryNode("IndexedQuadSet");
        break;
      case ID_NEW_INDEXED_TRIANGLE_FAN_SET:
        createGeometryNode("IndexedTriangleFanSet");
        break;
      case ID_NEW_INDEXED_TRIANGLE_SET:
        createGeometryNode("IndexedTriangleSet");
        break;
      case ID_NEW_INDEXED_TRIANGLE_STRIP_SET:
        createGeometryNode("IndexedTriangleStripSet");
        break;
      case ID_NEW_INLINE:
        insertInline();
        break;
      case ID_NEW_INLINE_LOAD_CONTROL:
        insertInline(true);
        break;
      case ID_NEW_INTEGER_SEQUENCER:
        createNode("IntegerSequencer");
        break;
      case ID_NEW_INTEGER_TRIGGER:
        createNode("IntegerTrigger");
        break;
      case ID_NEW_ISO_SURFACE_VOLUME_DATA:
        insertNode(CHILD_NODE, "IsoSurfaceVolumeData");
        break;
      case ID_NEW_KEY_SENSOR:
        createNode("KeySensor");
        break;
      case ID_NEW_LAYER:
        insertNode(LAYER_NODE, "Layer");
        break;
      case ID_NEW_LAYER_SET:
        insertNode(ROOT_NODE, "LayerSet");
        break;
      case ID_NEW_LAYOUT:
        insertNode(X3D_LAYOUT, "Layout");
        break;
      case ID_NEW_LAYOUT_GROUP:
        createNode("LayoutGroup");
        break;
      case ID_NEW_LAYOUT_LAYER:
        insertNode(LAYER_NODE, "LayoutLayer");
        break;
      case ID_NEW_LOAD_SENSOR:
        createNode("LoadSensor");
        break;
      case ID_NEW_LINE_PICK_SENSOR:
        createNode("LinePickSensor");
        break;
      case ID_NEW_LINE_PROPERTIES:
        insertNode(X3D_LINE_PROPERTIES, "LineProperties");
        break;
      case ID_NEW_LINE_SET:
        createGeometryNode("LineSet");
        break;
      case ID_NEW_LOCAL_FOG:
        createNode("LocalFog");
        break;
      case ID_NEW_LOD:
        createNode("LOD");
        break;
      case ID_NEW_MATERIAL:
        insertNode(MATERIAL_NODE, "Material");
        break;
      case ID_NEW_MATRIX_3_VERTEX_ATTRIBUTE:
        insertNode(VERTEX_ATTRIBUTE_NODE, "Matrix3VertexAttribute");
        break;
      case ID_NEW_MATRIX_4_VERTEX_ATTRIBUTE:
        insertNode(VERTEX_ATTRIBUTE_NODE, "Matrix4VertexAttribute");
        break;
      case ID_NEW_METADATA_DOUBLE:
        insertNode(METADATA_NODE, "MetadataDouble");
        break;
      case ID_NEW_METADATA_FLOAT:
        insertNode(METADATA_NODE, "MetadataFloat");
        break;
      case ID_NEW_METADATA_INTEGER:
        insertNode(METADATA_NODE, "MetadataInteger");
        break;
      case ID_NEW_METADATA_SET:
        insertNode(METADATA_NODE, "MetadataSet");
        break;
      case ID_NEW_METADATA_STRING:
        insertNode(METADATA_NODE, "MetadataString");
        break;
      case ID_NEW_MOTOR_JOINT:
        createNode("MotorJoint");
        break;
      case ID_NEW_MOVIE_TEXTURE:
        insertMovieTexture();
        break;
      case ID_NEW_MULTI_TEXTURE:
        insertMultiTexture();
        break;
      case ID_NEW_MULTI_TEXTURE_COORDINATE:
        insertMultiTextureCoordinate();
        break;
      case ID_NEW_MULTI_TEXTURE_TRANSFORM:
        insertMultiTextureTransform();
        break;
      case ID_NEW_NAVIGATION_INFO:
        createNode("NavigationInfo");
        break;
      case ID_NEW_NORMAL:
        insertNormal();
        break;
      case ID_NEW_NORMAL_INTERPOLATOR:
        createNode("NormalInterpolator");
        break;
      case ID_NEW_NURBS_CURVE:
        createNurbsCurve();
        break;
      case ID_NEW_NURBS_CURVE_2D:
        createNurbsCurve2D();
        break;
      case ID_NEW_NURBS_GROUP:
        if (m_scene->isX3d())
            createGeometryNode("NurbsSet");
        else
            createNode("NurbsGroup");
        break;
      case ID_NEW_NURBS_ORIENTATION_INTERPOLATOR:
        createNode("NurbsOrientationInterpolator");
        break;
      case ID_NEW_NURBS_POSITION_INTERPOLATOR:
        createNode("NurbsPositionInterpolator");
        break;
      // for ID_NEW_NURBS_SURFACE see ID_NEW_NURBS_PLANE
      case ID_NEW_NURBS_SURFACE_INTERPOLATOR:
        createNode("NurbsSurfaceInterpolator");
        break;
      case ID_NEW_NURBS_SWEPT_SURFACE:
        createGeometryNode("NurbsSweptSurface");
        break;
      case ID_NEW_NURBS_SWUNG_SURFACE:
        createGeometryNode("NurbsSwungSurface");
        break;
      case ID_NEW_NURBS_TEXTURE_COORDINATE:
      case ID_NEW_NURBS_TEXTURE_SURFACE:
        if (m_scene->isX3d())
            insertNode(NURBS_TEXTURE_COORDINATE_NODE, "NurbsTextureCoordinate");
        else
            insertNode(NURBS_TEXTURE_COORDINATE_NODE, "NurbsTextureSurface");
        break;
      case ID_NEW_NURBS_TRIMMED_SURFACE:
        createGeometryNode("NurbsTrimmedSurface");
        break;
      case ID_NEW_ORIENTATION_CHASER:
        createNode("OrientationChaser");
        break;
      case ID_NEW_ORIENTATION_DAMPER:
        createNode("OrientationDamper");
        break;
      case ID_NEW_ORIENTATION_INTERPOLATOR:
        createNode("OrientationInterpolator");
        break;
      case ID_NEW_ORTHO_VIEWPOINT:
        createNode("OrthoViewpoint");
        break;
      case ID_NEW_PACKAGED_SHADER:
        insertNode(SHADER_NODE, "PackagedShader");
        break;
      case ID_NEW_PARTICLE_SYSTEM:
        createNode("ParticleSystem");
        break;
      case ID_NEW_PICKABLE_GROUP:
        createNode("PickableGroup");
        break;
      case ID_NEW_PIXEL_TEXTURE:
        insertNode(TEXTURE_NODE, "PixelTexture");
        break;
      case ID_NEW_PIXEL_TEXTURE_3D:
        insertNode(TEXTURE_3D_NODE, "PixelTexture3D");
        break;
      case ID_NEW_PLANE_SENSOR:
        createNode("PlaneSensor");
        break;
      case ID_NEW_POINT_EMITTER:
        insertNode(PARTICLE_EMITTER_NODE, "PointEmitter");
        break;
      case ID_NEW_POINT_LIGHT:
        createNode("PointLight");
        break;
      case ID_NEW_POINT_PICK_SENSOR:
        createNode("PointPickSensor");
        break;
      case ID_NEW_POINT_SET:
        createGeometryNode("PointSet");
        break;
      case ID_NEW_POLYLINE_2D:
        if (m_scene->isX3d())
            createGeometryNode("Polyline2D", true);
        else
            insertNode(NURBS_CONTROL_CURVE_NODE, "Polyline2D");
        break;
      case ID_NEW_POLYLINE_EMITTER:
        insertNode(PARTICLE_EMITTER_NODE, "PolylineEmitter");
        break;
      case ID_NEW_POLYPOINT_2D:
        createGeometryNode("Polypoint2D", true);
        break;
      case ID_NEW_POSITION_CHASER:
        createNode("PositionChaser");
        break;
      case ID_NEW_POSITION_CHASER_2D:
        createNode("PositionChaser2D");
        break;
      case ID_NEW_POSITION_DAMPER:
        createNode("PositionDamper");
        break;
      case ID_NEW_POSITION_DAMPER_2D:
        createNode("PositionDamper2D");
        break;
      case ID_NEW_POSITION_INTERPOLATOR:
        createNode("PositionInterpolator");
        break;
      case ID_NEW_POSITION_INTERPOLATOR_2D:
        createNode("PositionInterpolator2D");
        break;
      case ID_NEW_PRIMITIVE_PICK_SENSOR:
        createNode("PrimitivePickSensor");
        break;
      case ID_NEW_PROGRAM_SHADER:
        insertNode(SHADER_NODE, "ProgramShader");
        break;
      case ID_NEW_PROXIMITY_SENSOR:
        createNode("ProximitySensor");
        break;
      case ID_NEW_QUAD_SET:
        createGeometryNode("QuadSet");
        break;
      case ID_NEW_RECEIVER_PDU:
        createNode("ReceiverPdu");
        break;
      case ID_NEW_RECTANGLE_2D:
        createGeometryNode("Rectangle2D");
        break;
      case ID_NEW_RIGID_BODY:
        insertRigidBody();
        break;
      case ID_NEW_RIGID_BODY_COLLECTION:
        createNode("RigidBodyCollection");
        break;
      case ID_NEW_SCALAR_CHASER:
        createNode("ScalarChaser");
        break;
      case ID_NEW_SCALAR_DAMPER:
        createNode("ScalarDamper");
        break;
      case ID_NEW_SCALAR_INTERPOLATOR:
        createNode("ScalarInterpolator");
        break;
      case ID_NEW_SCREEN_FONT_STYLE:
        insertNode(FONT_STYLE_NODE, "ScreenFontStyle");
        break;
      case ID_NEW_SCREEN_GROUP:
        createNode("ScreenGroup");
        break;
      case ID_NEW_SCRIPT:
        createScript();
        break;
      case ID_NEW_SEGMENTED_VOLUME_DATA:
        insertNode(CHILD_NODE, "SegmentedVolumeData");
        break;
      case ID_NEW_SHADER_PART:
        insertNode(X3D_SHADER_PART, "ShaderPart");
        break;
      case ID_NEW_SHADER_PROGRAM:
        insertNode(X3D_SHADER_PROGRAM, "ShaderProgram");
        break;
      case ID_NEW_SHAPE:
        createNode("Shape");
        break;
      case ID_NEW_SIGNAL_PDU:
        createNode("SignalPdu");
        break;
      case ID_NEW_SINGLE_AXIS_HINGE_JOINT:
        createNode("SingleAxisHingeJoint");
        break;
      case ID_NEW_SLIDER_JOINT:
        createNode("SliderJoint");
        break;
      case ID_NEW_SOUND:
        createNode("Sound");
        break;
      case ID_NEW_SPHERE:
        createGeometryNode("Sphere");
        break;
      case ID_NEW_SPHERE_SENSOR:
        createNode("SphereSensor");
        break;
      case ID_NEW_SPOT_LIGHT:
        createNode("SpotLight");
        break;
      case ID_NEW_SPLINE_POSITION_INTERPOLATOR:
        createNode("SplinePositionInterpolator");
        break;
      case ID_NEW_SPLINE_POSITION_INTERPOLATOR_2D:
        createNode("SplinePositionInterpolator2D");
        break;
      case ID_NEW_SPLINE_SCALAR_INTERPOLATOR:
        createNode("SplineScalarInterpolator");
        break;
      case ID_NEW_SQUAD_ORIENTATION_INTERPOLATOR:
        createNode("SquadOrientationInterpolator");
        break;
      case ID_NEW_STATIC_GROUP:
        createNode("StaticGroup");
        break;
      case ID_NEW_STRING_SENSOR:
        createNode("StringSensor");
        break;
      case ID_NEW_SURFACE_EMITTER:
        insertNode(PARTICLE_EMITTER_NODE, "SurfaceEmitter");
        break;
      case ID_NEW_SWITCH:
        createNode("Switch");
        break;
      case ID_NEW_TEX_COORD_CHASER_2D:
        createNode("TexCoordChaser2D");
        break;
      case ID_NEW_TEX_COORD_DAMPER_2D:
        createNode("TexCoordDamper2D");
        break;
      case ID_NEW_TEXTURE_BACKGROUND:
        createGeometryNode("TextureBackground");
        break;
      case ID_NEW_TEXT:
        createGeometryNode("Text");
        break;
      case ID_NEW_TEXTURE_COORDINATE:
        insertTextureCoordinate(false);
        break;
      case ID_NEW_TEXTURE_COORDINATE_3D:
        insertNode(TEXTURE_COORDINATE_NODE, "TextureCoordinate3D");
        break;
      case ID_NEW_TEXTURE_COORDINATE_4D:
        insertNode(TEXTURE_COORDINATE_NODE, "TextureCoordinate4D");
        break;
      case ID_NEW_TEXTURE_COORDINATE_GENERATOR:
        insertTextureCoordinate(true);
        break;
      case ID_NEW_TEXTURE_PROPERTIES:
        insertNode(X3D_TEXTURE_PROPERTIES, "TextureProperties");
        break;
      case ID_NEW_TEXTURE_TRANSFORM:
        insertNode(TEXTURE_TRANSFORM_NODE, "TextureTransform");
        break;
      case ID_NEW_TEXTURE_TRANSFORM_3D:
        insertNode(TEXTURE_TRANSFORM_NODE, "TextureTransform3D");
        break;
      case ID_NEW_TEXTURE_TRANSFORM_MATRIX_3D:
        insertNode(TEXTURE_TRANSFORM_NODE, "TextureTransformMatrix3D");
        break;
      case ID_NEW_TIME_SENSOR:
        createNode("TimeSensor");
        break;
      case ID_NEW_TIME_TRIGGER:
        createNode("TimeTrigger");
        break;
      case ID_NEW_TOUCH_SENSOR:
        createNode("TouchSensor");
        break;
      case ID_NEW_TWO_SIDED_MATERIAL:
        insertNode(MATERIAL_NODE, "TwoSidedMaterial");
        break;
      case ID_NEW_TRANSFORM:
        createNode("Transform");
        break;
      case ID_NEW_TRANSFORM_SENSOR:
        createNode("TransformSensor");
        break;
      case ID_NEW_TRANSMITTER_PDU:
        createNode("TransmitterPdu");
        break;
      case ID_NEW_TRIANGLE_FAN_SET:
        createGeometryNode("TriangleFanSet");
        break;
      case ID_NEW_TRIANGLE_SET:
        createGeometryNode("TriangleSet");
        break;
      case ID_NEW_TRIANGLE_SET_2D:
        createGeometryNode("TriangleSet2D");
        break;
      case ID_NEW_TRIANGLE_STRIP_SET:
        createGeometryNode("TriangleStripSet");
        break;
      case ID_NEW_UNIVERSAL_JOINT:
        createNode("UniversalJoint");
        break;
      case ID_NEW_VIEWPOINT:
        createViewpoint();
        break;
      case ID_NEW_VIEWPOINT_GROUP:
        createNode("ViewpointGroup");
        break;
      case ID_NEW_VIEWPORT:
        insertViewport();
        break;
      case ID_NEW_VISIBILITY_SENSOR:
        createNode("VisibilitySensor");
        break;
      case ID_NEW_VOLUME_DATA:
        insertNode(CHILD_NODE, "VolumeData");
        break;
      case ID_NEW_VOLUME_EMITTER:
        insertNode(PARTICLE_EMITTER_NODE, "VolumeEmitter");
        break;
      case ID_NEW_VOLUME_PICK_SENSOR:
        createNode("VolumePickSensor");
        break;
      case ID_NEW_WIND_PHYSICS_MODEL:
        insertNode(PARTICLE_PHYSICS_MODEL_NODE, "WindPhysicsModel");
        break;
      case ID_NEW_WORLD_INFO:
        createNode("WorldInfo");
        break;
      case ID_NEW_BLENDED_VOLUME_STYLE:
        insertNode(VOLUME_RENDER_STYLE_NODE, "BlendedVolumeStyle");
        break;
      case ID_NEW_BOUNDARY_ENHANCEMENT_VOLUME_STYLE:
        insertNode(VOLUME_RENDER_STYLE_NODE, 
                   "BoundaryEnhancementVolumeStyle");
        break;
      case ID_NEW_CARTOON_VOLUME_STYLE:
        insertNode(VOLUME_RENDER_STYLE_NODE, "CartoonVolumeStyle");
        break;
      case ID_NEW_COMPOSED_VOLUME_STYLE:
        insertNode(VOLUME_RENDER_STYLE_NODE, "ComposedVolumeStyle");
        break;
      case ID_NEW_EDGE_ENHANCEMENT_VOLUME_STYLE:
        insertNode(VOLUME_RENDER_STYLE_NODE, "EdgeEnhancementVolumeStyle");
        break;
      case ID_NEW_OPACITY_MAP_VOLUME_STYLE:
        insertNode(VOLUME_RENDER_STYLE_NODE, "OpacityMapVolume_style");
        break;
      case ID_NEW_PROJECTION_VOLUME_STYLE:
        insertNode(VOLUME_RENDER_STYLE_NODE, "ProjectionVolumeStyle");
        break;
      case ID_NEW_SHADED_VOLUME_STYLE:
        insertNode(VOLUME_RENDER_STYLE_NODE, "ShadedVolumeStyle");
        break;
      case ID_NEW_SILHOUETTE_ENHANCEMENT_VOLUME_STYLE:
        insertNode(VOLUME_RENDER_STYLE_NODE, "SilhouetteEnhancementVolumeStyle");
        break;
      case ID_NEW_TONE_MAPPED_VOLUME_STYLE:
        insertNode(VOLUME_RENDER_STYLE_NODE, "ToneMappedVolumeStyle");
        break;
      case ID_NEW_COMMENT:
        createNode("#");
        break;

      case ID_NEW_TRIMMED_SURFACE:
        createGeometryNode("TrimmedSurface");
        break;

      case ID_NEW_COVER_COVER:
        createNode("COVER");
        break;
      case ID_NEW_COVER_SPACE_SENSOR:
        createNode("SpaceSensor");
        break;
      case ID_NEW_COVER_AR_SENSOR:
        createNode("ARSensor");
        break;
      case ID_NEW_COVER_STEERING_WHEEL:
        createNode("SteeringWheel");
        break;
      case ID_NEW_COVER_VEHICLE:
        createNode("Vehicle");
        break;
      case ID_NEW_COVER_JOYSTICK_SENSOR:
        createNode("JoystickSensor");
        break;
      case ID_NEW_COVER_LAB_VIEW:
        createNode("LabView");
        break;
      case ID_NEW_COVER_CUBE_TEXTURE:
        insertCubeTexture();
        break;
      case ID_NEW_COVER_WAVE:
        insertNode(COVER_WAVE, "Wave");
        break;
      case ID_NEW_COVER_VIRTUAL_ACOUSTICS:
        createVirtualAcoustics();
        break;
      case ID_NEW_COVER_VIRTUAL_SOUND_SOURCE:
        insertVirtualSoundSource();
        break;
      case ID_NEW_COVER_SKY:
        createNode("Sky");
        break;
      case ID_NEW_COVER_TUI_BUTTON:
        createNode("TUIButton");
        break;
      case ID_NEW_COVER_TUI_COMBO_BOX:
        createNode("TUIComboBox");
        break;
      case ID_NEW_COVER_TUI_FLOAT_SLIDER:
        createNode("TUIFloatSlider");
        break;
      case ID_NEW_COVER_TUI_FRAME:
        createNode("TUIFrame");
        break;
      case ID_NEW_COVER_TUI_LABEL:
        createNode("TUILabel");
        break;
      case ID_NEW_COVER_TUI_LIST_BOX:
        createNode("TUIListBox");
        break;
      case ID_NEW_COVER_TUI_MAP:
        createNode("TUIMap");
        break;
      case ID_NEW_COVER_TUI_PROGRESS_BAR:
        createNode("TUIProgressBar");
        break;
      case ID_NEW_COVER_TUI_SLIDER:
        createNode("TUISlider");
        break;
      case ID_NEW_COVER_TUI_SPLITTER:
        createNode("TUISplitter");
        break;
      case ID_NEW_COVER_TUI_TAB:
        createNode("TUITab");
        break;
      case ID_NEW_COVER_TUI_TAB_FOLDER:
        createNode("TUITabFolder");
        break;
      case ID_NEW_COVER_TUI_TOGGLE_BUTTON:
        createNode("TUIToggleButton");
        break;

      case ID_NEW_KAMBI_TEXT_3D:
        createGeometryNode("Text3D");
        break;
      case ID_NEW_KAMBI_TEAPOT:
        createGeometryNode("Teapot");
        break;
      case ID_NEW_KAMBI_INLINE:
        createNode("KambiInline");
        break;
      case ID_NEW_KAMBI_MATRIX_TRANSFORM:
        createNode("MatrixTransform");
        break;
      case ID_NEW_KAMBI_APPEARANCE:
        insertNode(VRML_APPEARANCE, "KambiAppearance");
        break;
      case ID_NEW_KAMBI_BLEND_MODE:
        if (m_scene->getSelection()->getNode()->getType() == KAMBI_KAMBI_APPEARANCE)
            insertNode(KAMBI_BLEND_MODE, "BlendMode");
        break;
      case ID_NEW_KAMBI_OCTREE_PROPERTIES:
        insertNode(KAMBI_KAMBI_OCTREE_PROPERTIES, "KambiOctreeProperties");
        break;
     case ID_NEW_KAMBI_GENERATED_SHADOW_MAP:
        insertNode(KAMBI_GENERATED_SHADOW_MAP, "GeneratedShadowMap");
        break;
      case ID_NEW_KAMBI_RENDERED_TEXTURE:
      case ID_NEW_RENDERED_TEXTURE:
        insertNode(TEXTURE_NODE, "RenderedTexture");
        break;
      case ID_NEW_KAMBI_MULTI_GENERATED_TEXTURE_COORDINATE:
        insertMultiGeneratedTextureCoordinate();
        break;
      case ID_NEW_KAMBI_PROJECTED_TEXTURE_COORDINATE:
        insertNode(TEXTURE_COORDINATE_NODE | 
                   GENERATED_TEXTURE_COORDINATE_NODE, 
                   "ProjectedTextureCoordinate");
        break;
      case ID_NEW_KAMBI_HEAD_LIGHT:
        createNode("KambiHeadLight");
        break;
      case ID_NEW_KAMBI_TRIANGULATION:
        createNode("KambiTriangulation");
        break;
      case ID_NEW_KAMBI_NAVIGATION_INFO:
        createNode("KambiNavigationInfo");
        break;
      case ID_NEW_KAMBI_SHADER_TEXTURE:
        insertNode(TEXTURE_NODE, "ShaderTexture");
        break;
      case ID_NEW_KAMBI_EFFECT:
        createNode("Effect");
        break;
      case ID_NEW_KAMBI_EFFECT_PART:
        insertNode(KAMBI_EFFECT_PART, "EffectPart");
        break;
      case ID_NEW_KAMBI_SCREEN_EFFECT:
        createNode("ScreenEffect");
        break;
      case ID_NEW_KAMBI_COLOR_SET_INTERPOLATOR:
        createNode("ColorSetInterpolator");
        break;
      case ID_NEW_KAMBI_VECTOR_INTERPOLATOR:
        createNode("VectorInterpolator");
        break;
      case ID_NEW_KAMBI_CUBIC_BEZIER_2D_ORIENTATION_INTERPOLATOR:
        createNode("CubicBezier2DOrientationInterpolator");
        break;
      case ID_NEW_KAMBI_CUBIC_BEZIER_POSITION_INTERPOLATOR:
        createNode("CubicBezierPositionInterpolator");
        break;
      case ID_NEW_SURFACE_SHADER_TEXTURE:
        insertNode(TEXTURE_NODE, "SurfaceShaderTexture");
        break;

      case ID_NEW_BINARY_GEOMETRY:
        createGeometryNode("BinaryGeometry");
        break;
      case ID_NEW_BLOCK:
        createNode("Block");
        break;
      case ID_NEW_COLOR_MASK_MODE:
        insertNode(X3DOM_COLOR_MASK_MODE, "ColorMaskMode");
        break;
      case ID_NEW_COMMON_SURFACE_SHADER:
        insertNode(TEXTURE_NODE, "CommonSurfaceShader");
        break;
      case ID_NEW_DISH:
        createGeometryNode("Dish");
        break;
      case ID_NEW_DEPTH_MODE:
        insertNode(X3DOM_DEPTH_MODE, "DepthMode");
        break;
      case ID_NEW_DYNAMIC_LOD:
        createNode("DynamicLOD");
        break;
      case ID_NEW_ENVIRONMENT:
        createNode("Environment");
        break;
      case ID_NEW_EXTERNAL_GEOMETRY:
        createGeometryNode("ExternalGeometry");
        break;
      case ID_NEW_FIELD:
        createNode("Field");
        break;
      case ID_NEW_IMAGE_GEOMETRY:
        createGeometryNode("ImageGeometry");
        break;
      case ID_NEW_IMAGE_TEXTURE_ATLAS:
        insertNode(TEXTURE_NODE, "ImageTextureAtlas");
        break;
      case ID_NEW_NOZZLE:
        createGeometryNode("Nozzle");
        break;
      case ID_NEW_MPR_VOLUME_STYLE:
        insertNode(VOLUME_RENDER_STYLE_NODE, "MPRVolumeStyle");
        break;
      case ID_NEW_MESH:
        createGeometryNode("Mesh");
        break;
      case ID_NEW_MULTI_PART:
        insertNode(X3DOM_MULTI_PART, "MultiPart");
        break;
      case ID_NEW_PARTICLE_SET:
        createGeometryNode("ParticleSet");
        break;
      case ID_NEW_PLANE:
        createGeometryNode("Plane");
        break;
      case ID_NEW_POP_GEOMETRY:
        createGeometryNode("PopGeometry");
        break;
      case ID_NEW_POP_GEOMETRY_LEVEL:
        insertNode(X3DOM_POP_GEOMETRY_LEVEL, "PopGeometryLevel");
        break;
      case ID_NEW_PYRAMID:
        createGeometryNode("Pyramid");
        break;
      case ID_NEW_RADAR_VOLUME_STYLE:
        insertNode(VOLUME_RENDER_STYLE_NODE, "RadarVolumeStyle");
        break;
      case ID_NEW_RECTANGULAR_TORUS:
        createGeometryNode("RectangularTorus");
        break;
      case ID_NEW_REFINEMENT_TEXTURE:
        insertNode(TEXTURE_NODE, "RefinementTexture");
        break;
      case ID_NEW_REMOTE_SELECTION_GROUP:
        createNode("RemoteSelectionGroup");
        break;
      case ID_NEW_SLOPED_CYLINDER:
        createGeometryNode("SlopedCylinder");
        break;
      case ID_NEW_SNOUT:
        createGeometryNode("Snout");
        break;
      case ID_NEW_SOLID_OF_REVOLUTION:
        createGeometryNode("SolidOfRevolution");
        break;
      case ID_NEW_SPHERE_SEGMENT:
        createGeometryNode("SphereSegment");
        break;
      case ID_NEW_STIPPLE_VOLUME_STYLE:
        insertNode(VOLUME_RENDER_STYLE_NODE, "StippleVolumeStyle");
        break;
      case ID_NEW_X3DOM_TORUS:
        createGeometryNode("Torus");
        break;
      case ID_NEW_UNIFORM:
        createNode("Uniform");
        break;
      case ID_NEW_VIEWFRUSTUM:
        createNode("Viewfrustum");
        break;

      case ID_NEW_SUPER_ELLIPSOID:
        createGeometryNode("SuperEllipsoid");
        break;
      case ID_NEW_SUPER_EXTRUSION:
        createSuperExtrusion();
        break;
      case ID_NEW_SUPER_SHAPE:
        createGeometryNode("SuperShape");
        break;
      case ID_NEW_SUPER_REVOLVER:
        createSuperRevolver();
        break;

      case ID_NEW_VRML_CUT:
        if (m_vrmlCutNode == NULL) {
            m_vrmlCutNode = createNode("VrmlCut");
            UpdateToolbars();
        }
        break;
      case ID_NEW_VRML_SCENE:
        insertVrmlScene();
        break;

      case ID_NEW_CURVE_ANIMATION:
        createCurveAnimationNode();
        break;


      case ID_NEW_NURBS_PLANE_XY:
        createNurbsPlane(0);
        break;
      case ID_NEW_NURBS_PLANE_YZ:
        createNurbsPlane(1);
        break;
      case ID_NEW_NURBS_PLANE_ZX:
        createNurbsPlane(2);
        break;
      case ID_NEW_NURBS_BOX:
        createGeometryNode("Box");
        toNurbs();
        break;
      case ID_NEW_NURBS_CONE:
        createGeometryNode("Cone");
        toNurbs();
        break;
      case ID_NEW_NURBS_CYLINDER:
        createGeometryNode("Cylinder");
        toNurbs();
        break;
      case ID_NEW_NURBS_SPHERE:
        createGeometryNode("Sphere");
        toNurbs();
        break;

      case ID_NEW_TUBE:
        createTube();
        break;
      case ID_NEW_HORN:
        createTube(true);
        break;
      case ID_NEW_TORUS:
        createTorus();
        break;
      case ID_NEW_STAR_FISH_0:
        createStarFish(0);
        break;
      case ID_NEW_STAR_FISH_1:
        createStarFish(1);
        break;
      case ID_NEW_STAR_FISH_2:
        createStarFish(2);
        break;
      case ID_NEW_STAR_FISH_3:
        createStarFish(3);
        break;
      case ID_NEW_STAR_FISH_4:
        createStarFish(4);
        break;
      case ID_NEW_STAR_FISH_5:
        createStarFish(5);
        break;
      case ID_NEW_STAR_FISH_6:
        createStarFish(6);
        break;
      case ID_NEW_STAR_FISH_7:
        createStarFish(7);
        break;
      case ID_NEW_STAR_FISH_8:
        createStarFish(8);
        break;
      case ID_NEW_FLOWER_0:
        createFlower(0);
        break;
      case ID_NEW_FLOWER_1:
        createFlower(1);
        break;
      case ID_NEW_FLOWER_2:
        createFlower(2);
        break;
      case ID_NEW_FLOWER_3:
        createFlower(3);
        break;
      case ID_NEW_FLOWER_4:
        createFlower(4);
        break;
      case ID_NEW_FLOWER_5:
        createFlower(5);
        break;
      case ID_NEW_FLOWER_6:
        createFlower(6);
        break;
      case ID_NEW_FLOWER_7:
        createFlower(7);
        break;
      case ID_NEW_FLOWER_8:
        createFlower(8);
        break;
      case ID_NEW_SHELL:
        createShell();   
        break;
      case ID_NEW_UFO:
        createUfo();   
       break;
      case ID_NEW_INSECT_REAR_0:
        createInsectRear(0);
        break;
      case ID_NEW_INSECT_REAR_1:
        createInsectRear(1);
        break;
      case ID_NEW_INSECT_REAR_2:
        createInsectRear(2);
        break;
      case ID_NEW_INSECT_REAR_3:
        createInsectRear(3);
        break;
      case ID_NEW_INSECT_REAR_4:
        createInsectRear(4);
        break;
      case ID_NEW_INSECT_REAR_5:
        createInsectRear(5);
        break;
      case ID_NEW_INSECT_REAR_6:
        createInsectRear(6);
        break;
      case ID_NEW_INSECT_REAR_7:
        createInsectRear(7);
        break;
      case ID_NEW_INSECT_REAR_8:
        createInsectRear(8);
        break;
      case ID_NEW_HALF_SPHERE:
        createHalfSphere();
        break;
      case ID_NEW_HEART:
        createHeart();
        break;
      case ID_NEW_SPINDLE:
        createSpindle();
        break;
      case ID_NEW_MUSHROOM_ROUND:
        createMushroom(false);
        break;
      case ID_NEW_MUSHROOM_SULCATE:
        createMushroom(true);
        break;
      case ID_NEW_TEXT_3D:
        createText3D();
        break;

      case ID_NEW_CATT_EXPORT_SRC:
        createNode("CattExportSrc");
        break;
      case ID_NEW_CATT_EXPORT_REC:
        createNode("CattExportRec");
        break;
      case ID_NEW_LDRAW_DAT_EXPORT:
        createNode("LdrawDatExport");
        break;
      case ID_NEW_WONDERLAND_IMPORT_JAVA:
        createNode("WonderlandImportJava");
        break;

      case ID_NEW_NODE_BY_NAME:
        insertNodeByName();
        break;

      case ID_PLAY:
        Play();
        break;
      case ID_STOP:
        Stop();
        break;
      case ID_RECORD:
        Record();
        break;
      case ID_REWIND:
        m_channelView->OnRewind();
        break;
      case ID_FAST_FORWARD:
        m_channelView->OnFastForward();
        break;
      case ID_NEXT_KEY:
        m_channelView->GoToNextKey();
        break;
      case ID_LAST_KEY:
        m_channelView->GoToLastKey();
        break;

      case ID_DUNE_APP_ABOUT:
          {
            AboutDialog dlg(m_wnd);
            dlg.DoModal();
          }
        break;
      case ID_DUNE_APP_HELP:
        OnHelpOverview();
        break;
      case ID_DUNE_APP_CATT_HELP:
        OnHelpOverviewCatt();
        break;
      case ID_HELP_SELECTION:
        OnHelpSelection();
        break;
      case ID_HELP_SELECTION_COVER:
        OnHelpCoverSelection();
        break;

      case ID_OPTIONS_MODELLING:
          {
            ModellingSettingsDialog dlg(m_wnd);
            dlg.DoModal();
          }
        break;
      case ID_OPTIONS_RENDER:
          {
            RenderSettingsDialog dlg(m_wnd);
            dlg.DoModal();
          }
        break;
      case ID_OPTIONS_ROUTE_VIEW:
          {
            RouteViewSettingsDialog dlg(m_wnd);
            dlg.DoModal();
            if (TheApp->GetShowRoutesAtBegin())
                m_graphView->Initialize();
            swInvalidateWindow(m_graphView->GetWindow());
          }
        break;
      case ID_OPTIONS_OUTPUT:
          {
            OutputSettingsDialog dlg(m_wnd);
            dlg.DoModal();
          }
        break;
      case ID_OPTIONS_ECMA_SCRIPT:
          {
            EcmaScriptSettingsDialog dlg(m_wnd);
            dlg.DoModal();
          }
        break;
      case ID_OPTIONS_PREVIEW:
          {
            PreviewSettingsDialog dlg(m_wnd);
            dlg.DoModal();
          }
        break;
      case ID_OPTIONS_UPLOAD:
          {
            UploadSettingsDialog dlg(m_wnd);
            dlg.DoModal();
            UpdateToolbar(ID_DUNE_FILE_UPLOAD);
          }
        break;
      case ID_OPTIONS_HELP:
          {
            HelpSettingsDialog dlg(m_wnd);
            dlg.DoModal();
          }
        break;
      case ID_OPTIONS_OBJECT_EDIT:
          {
            TexteditSettingsDialog dlg(m_wnd);
            dlg.DoModal();
          }
        break;
      case ID_OPTIONS_STEREO_VIEW:
          {
            bool canStereo = TheApp->canQuadBufferStereo();
            StereoViewSettingsDialog dlg(m_wnd);
            dlg.DoModal();
            if (TheApp->wantStereo()) {
                if (canStereo) 
                    TheApp->setUseStereo(true);
                else
                    TheApp->MessageBoxId(IDS_RESTART_4_STEREO);
            } else {
                TheApp->setUseStereo(false);
            }
          }
        break;
      case ID_OPTIONS_INPUT_DEVICE:
          {
            InputDevice *inputDevice = NULL;
#ifdef HAVE_AFLOCK
            AflockDevice *aflockDevice = NULL;
#endif
            InputDeviceSelectionDialog dlgSelect(m_wnd);
            if (dlgSelect.DoModal() == IDOK) {
                inputDevice = dlgSelect.getInputDevice();
#ifdef HAVE_AFLOCK
                aflockDevice = dlgSelect.getAflockDevice();
#endif
                MyString deviceOption = dlgSelect.getDeviceOption();
                if (deviceOption.length() > 0) {
                    InputDeviceNameSelectionDialog dlgName(m_wnd, deviceOption);
                    if (dlgName.DoModal() == IDOK) {
                        inputDevice = dlgName.getInputDevice();
#ifdef HAVE_AFLOCK
                        aflockDevice = dlgName.getAflockDevice();
#endif
                    }
                }
            }
            if (inputDevice != NULL) {
                InputDeviceSettingsDialog dlgSettings(m_wnd, inputDevice);
                dlgSettings.DoModal();
            }
#ifdef HAVE_AFLOCK
            else if (aflockDevice != NULL) {
                AflockSettingsDialog dlg(m_wnd, aflockDevice);
                dlg.DoModal();
            }
#endif
          }          
        break;
      case ID_OPTIONS_START_NORMAL:
        TheApp->SetStartVariant("dune");
        break;
      case ID_OPTIONS_START_WITH_4KIDS:
        TheApp->SetStartVariant("4kids");
        break;
      case ID_OPTIONS_START_WITH_4CATT:
        TheApp->SetStartVariant("4catt");
        break;
      case ID_OPTIONS_START_WITH_COVER:
        TheApp->SetStartVariant("cover");
        break;
      case ID_OPTIONS_START_WITH_KAMBI:
        TheApp->SetStartVariant("kambi");
        break;
      case ID_OPTIONS_START_WITH_X3DOM:
        TheApp->SetStartVariant("x3dom");
        break;
      case ID_OPTIONS_START_LANGUAGE_ENGLISH:
        TheApp->SetStartLanguage("en");
        break;
      case ID_OPTIONS_START_LANGUAGE_GERMAN:
        TheApp->SetStartLanguage("de");
        break;
      case ID_OPTIONS_START_LANGUAGE_ITALIAN:
        TheApp->SetStartLanguage("it");
        break;
      case ID_OPTIONS_START_LANGUAGE_FRENCH:
        TheApp->SetStartLanguage("fr");
        break;
      case ID_OPTIONS_START_LANGUAGE_PORTUGENESE:
        TheApp->SetStartLanguage("pt");
        break;
      case ID_TEACHER_CORRECT_NAME:
        setName(IDS_CORRECT);
        break;
      case ID_TEACHER_WRONG_NAME:
        setName(IDS_WRONG);
        break;
      case ID_TEACHER_CORRECT_SHAPE:
        createText(IDS_CORRECT);
        break;
      case ID_TEACHER_WRONG_SHAPE:
        createText(IDS_WRONG);
        break;

#ifdef HAVE_SAND
      case ID_SAND_EXPORT:
        OnSANDExport();
        break;
      case ID_SAND_WRITEMESHES:
        m_nebulaExporter.OnToggleWriteMeshes(m_menu);
        break;
      case ID_SAND_RELPATHS:
        m_nebulaExporter.OnToggleUseRelativePaths(m_menu);
        break;
      case ID_SAND_ALPHATEST:
        m_nebulaExporter.OnToggleAlphaTest(m_menu);
        break;
#endif
      case ID_TEST_IN_MENU:
        testInMenu();
        break;
      default:
        // recent files
        if ((id > ID_DUNE_FILE_MRU_FIRST) && (id < ID_DUNE_FILE_MRU_LAST)) { 
            if (!TheApp->is4Kids())
                if (m_vertexModifier_active)
                    TheApp->SetBoolPreference("ShowFieldView", true);

#ifndef HAVE_OPEN_IN_NEW_WINDOW
            if (!TheApp->checkSaveOldWindow())
                return;
#endif
            if (TheApp->getFileDialogDir())
                while (chdir(TheApp->getFileDialogDir()) == -2);
            int fileId = id - ID_DUNE_FILE_MRU_FILE1;
            if (OpenFileCheck(TheApp->GetRecentFile(fileId))) {
#ifndef HAVE_OPEN_IN_NEW_WINDOW
                TheApp->deleteOldWindow();
                exitFlag = true;
#endif
            }
        } 
        // proto usage
        if ((id >= ID_PROTO_MRU_1) && 
            (id < ID_PROTO_MRU_1 + (long)m_protoMenu.size())) 
            for (size_t i = 0; i < m_protoMenu.size(); i++)
                if (((int) id == m_protoMenu[i].id) && 
                    (!m_protoMenu[i].disabled)) {
                    Node *currentNode = m_scene->getSelection()->getNode();
                    Proto *outsideProto = currentNode->getOutsideProto();
                    if (outsideProto) {
                        if (strcmp(outsideProto->getName(m_scene->isX3d()),
                                   m_protoMenu[i].protoName) == 0)
                            continue;
                    }    
                    Node *node = createNode((const char*)
                                            m_protoMenu[i].protoName);
                    node->handleIs();
                    break;
                }
    }
    // end command
    if (exitFlag == false)
        m_scene->deleteLastNextCommand();
}

void MainWindow::updateColorCircle(void)
{
    if (m_colorCircle_enabled) {
       m_fieldView->setEnabled(false);
       m_scene->RemoveView(m_fieldView);
       if (m_fieldView != NULL) {
          m_fieldView->DeleteView();
// memory leak 8-( delete m_fieldView;
       }
       if (m_colorCircle_active) {
          m_fieldView = new FieldView(m_scene, m_fieldCanvas);
          m_colorCircle_active = false;
          m_scene->AddView(m_fieldView); 
          m_innerPane->SetPane(m_fieldView, PW_RIGHT);
          m_fieldView->setEnabled(true);
          Layout();
          // avoid recurive call of scene->UpdateViews(NULL, UPDATE_SELECTION);
          ((FieldView *)m_fieldView)->OnUpdate(NULL, UPDATE_SELECTION, NULL);
       } else {
          m_fieldView = new ColorCircle(m_scene, m_fieldCanvas, 
                                        m_colorCircleHint);
          m_colorCircle_active = true;
          m_scene->AddView(m_fieldView); 
          m_innerPane->SetPane(m_fieldView, PW_RIGHT);
          m_fieldView->setEnabled(false);
          Layout();
       }
    } else
       m_colorCircle_active = false;
    setColorCircleIcon();
}

void MainWindow::updateVertexModifier(void)
{
    if (m_vertexModifier_enabled) {
       m_fieldView->setEnabled(false);
       m_scene->RemoveView(m_fieldView);
       if (m_fieldView != NULL) {
          m_fieldView->DeleteView();
// memory leak 8-( delete m_fieldView;
       }
       if (m_vertexModifier_active) {
          m_fieldView = new FieldView(m_scene, m_fieldCanvas);
          m_vertexModifier_active = false;
          m_scene->AddView(m_fieldView); 
          m_innerPane->SetPane(m_fieldView, PW_RIGHT);
          m_fieldView->setEnabled(true);
          m_scene->setVertexModifier(NULL);
          Layout();
          // avoid recurive call of scene->UpdateViews(NULL, UPDATE_SELECTION);
          ((FieldView *)m_fieldView)->OnUpdate(NULL, UPDATE_SELECTION, NULL);
       } else {
          Node *node = m_scene->getSelection()->getNode();
          if ((node->getType() != VRML_COORDINATE) && 
              (node->getType() != VRML_NURBS_SURFACE))
              return;
    
          if (node && node->getType() == VRML_COORDINATE) {
              NodeCoordinate *coord = (NodeCoordinate *)node;
              if (m_vertexModifierHint)
                  delete m_vertexModifierHint;
              m_vertexModifierHint = new FieldUpdate(node, coord->point_Field(),
                                                     -1);
          }
          if (node && node->getType() == VRML_NURBS_SURFACE) {
              NodeNurbsSurface *nurbs = (NodeNurbsSurface *)node;
              if (m_vertexModifierHint)
                  delete m_vertexModifierHint;
              m_vertexModifierHint = new FieldUpdate(node, 
                                             nurbs->controlPoint_Field(), -1);
          }   
          m_fieldView = new VertexModifier(m_scene, m_fieldCanvas, 
                                           m_vertexModifierHint);
          m_vertexModifier_active = true;
          m_scene->AddView(m_fieldView);
          m_scene->UpdateViews(NULL, UPDATE_REDRAW_3D);
          m_innerPane->SetPane(m_fieldView, PW_RIGHT);
          m_fieldView->setEnabled(false);
          m_scene->setVertexModifier((VertexModifier *)m_fieldView);
          Layout();
       }
    } else {
       m_vertexModifier_active = false;
       m_scene->setVertexModifier(NULL);
    }
    setVertexModifierIcon();
}

void
MainWindow::showVertexModifier(void)
{
    Node *node = m_scene->getSelection()->getNode();
    if ((node->getType() != VRML_COORDINATE) && 
        (node->getType() != VRML_NURBS_SURFACE))
        return;
    
    if (node && node->getType() == VRML_COORDINATE) {
        NodeCoordinate *coord = (NodeCoordinate *)node;
        m_vertexModifierHint = new FieldUpdate(node, coord->point_Field(), -1);
        m_vertexModifier_enabled = true;
    }
    if (node && node->getType() == VRML_NURBS_SURFACE) {
        NodeNurbsSurface *nurbs = (NodeNurbsSurface *)node;
        m_vertexModifierHint = new FieldUpdate(node, 
                                               nurbs->controlPoint_Field(), -1);
        m_vertexModifier_enabled = true;
    }
    ShowView(PW_RIGHT, m_fieldView, ID_DUNE_VIEW_FIELD_VIEW, "ShowFieldView");
    updateVertexModifier();
}

void
MainWindow::showColorPerVertexColorCircle(void)
{
    Node *node = m_scene->getSelection()->getNode();
    if ((node->getType() != VRML_COLOR) && (node->getType() != X3D_COLOR_RGBA))
        return;
    Node *color = node;
    if (color && color->getType() == VRML_COLOR) {
        NodeColor *c = (NodeColor *)color;
        m_colorCircleHint = new FieldUpdate(color, c->color_Field(), -1);
        m_colorCircle_enabled = true;
        ShowView(PW_RIGHT, m_fieldView, ID_DUNE_VIEW_FIELD_VIEW, 
                 "ShowFieldView");
        updateColorCircle();
    }
    if (color && color->getType() == X3D_COLOR_RGBA) {
        NodeColorRGBA *c = (NodeColorRGBA *)color;
        m_colorCircleHint = new FieldUpdate(color, c->color_Field(), -1);
        m_colorCircle_enabled = true;
        ShowView(PW_RIGHT, m_fieldView, ID_DUNE_VIEW_FIELD_VIEW, 
                 "ShowFieldView");
        updateColorCircle();
    }
}

void MainWindow::showDiffuseColorCircle(void)
{
    Node *node = m_scene->getSelection()->getNode();
    if (node->getType() != VRML_MATERIAL)
        return;
    NodeMaterial *material = (NodeMaterial *) node;
    if (m_colorCircleHint)
        delete m_colorCircleHint;
    m_colorCircleHint = new FieldUpdate(node, material->diffuseColor_Field(),
                                        -1);
    m_colorCircle_enabled = true;
    ShowView(PW_RIGHT, m_fieldView, ID_DUNE_VIEW_FIELD_VIEW, "ShowFieldView");
    updateColorCircle();
}

void MainWindow::showEmissiveColorCircle(void)
{
    Node *node = m_scene->getSelection()->getNode();
    if (node->getType() != VRML_MATERIAL)
        return;
    NodeMaterial *material = (NodeMaterial *) node;
    if (m_colorCircleHint)
        delete m_colorCircleHint;
    m_colorCircleHint = new FieldUpdate(node, material->emissiveColor_Field(), 
                                        -1);
    m_colorCircle_enabled = true;
    ShowView(PW_RIGHT, m_fieldView, ID_DUNE_VIEW_FIELD_VIEW, "ShowFieldView");
    updateColorCircle();
}

void MainWindow::showSpecularColorCircle(void)
{
    Node *node = m_scene->getSelection()->getNode();
    if (node->getType() != VRML_MATERIAL)
        return;
    NodeMaterial *material = (NodeMaterial *) node;
    if (m_colorCircleHint)
        delete m_colorCircleHint;
    m_colorCircleHint = new FieldUpdate(node, material->specularColor_Field(), 
                                        -1);
    m_colorCircle_enabled = true;
    ShowView(PW_RIGHT, m_fieldView, ID_DUNE_VIEW_FIELD_VIEW, "ShowFieldView");
    updateColorCircle();
}

void                
MainWindow::makeSymetric(int direction)
{
    Node *node = m_scene->getSelection()->getNode();
    if (node && (node->getType() == VRML_COORDINATE))
        node = node->getParent();
    if (node && node->getType() != VRML_INDEXED_FACE_SET)
        return;
    NodeIndexedFaceSet *faceset = (NodeIndexedFaceSet *)node;
    triangulate();
    int idd = IDD_SYMETRIC_X;
    switch (direction) {
      case 1:
        idd = IDD_SYMETRIC_Y;
        break;
      case 2:
        idd = IDD_SYMETRIC_Z;
        break;
    }
    OneBoolDialog dlg(m_wnd, idd, true);
    if (dlg.DoModal() == IDCANCEL)
        return;

    faceset->makeSymetric(direction, dlg.GetValue());
    m_scene->setSelection(m_scene->replaceNode(node, faceset));
    m_scene->UpdateViews(NULL, UPDATE_SELECTION);
}

void                
MainWindow::setViewpoint(void)
{
    ViewpointDialog dlg(m_wnd, IDD_VIEWPOINT, m_scene);
    if (dlg.DoModal() == IDCANCEL)
        return;    
    float *viewData = dlg.GetViewpointData();
    float dist = dlg.GetDist();
    NodeNavigationInfo *navi = m_scene->getNavigationInfoNode();
    if (navi && dlg.GetValidDist()) {
        MFFloat *avatarSize = navi->avatarSize();
        avatarSize->setValue(3, dist);
        Node *camera = m_scene->getCamera();
        Vec3d vec = camera->getPosition();
        vec.normalize(); 
        vec = vec * dist;
        camera->setPosition(vec);
        m_scene->UpdateViews(NULL, UPDATE_SELECTION);
    }
    if (viewData) {
        Vec3d vec(viewData[0], viewData[1], viewData[2]);
        vec.normalize();
        vec = vec * dist;
        SFRotation *rot = new SFRotation(viewData[3], viewData[4], viewData[5],
                                         viewData[6]);
        m_scene->getCamera()->setPosition(vec);
        m_scene->getCamera()->setOrientation(rot->getQuat());
        m_scene->UpdateViews(NULL, UPDATE_SELECTION);
    }
    m_scene->UpdateViews(NULL, UPDATE_REDRAW_3D, NULL);
}

void                
MainWindow::setTarget(void)
{
    Node *node = m_scene->getSelection()->getNode();
    MyStack<Node *> nodeStack;
    while (node) {
       if (node->getType() == VRML_TRANSFORM)
           nodeStack.push(node);
       node = node->getParent();
    }
    Matrix matrix = Matrix::identity();
    while (!nodeStack.empty()) {
        NodeTransform *trans = (NodeTransform *)nodeStack.pop();
        Matrix mat;
        trans->getMatrix(mat);
        matrix = matrix * mat;
    }
    Node *viewpoint = m_scene->getCamera();
    float dist = 0;
    Vec3f vec;
    float mat[4][4];
    matrix.getValue(mat);
    vec.x = mat[3][0];
    vec.y = mat[3][1];
    vec.z = mat[3][2];
    Vec3d viewVec = viewpoint->getPosition();
    if (viewpoint)
        dist = sqrt((viewVec.x - vec.x) * (viewVec.x - vec.x) +
                    (viewVec.y - vec.y) * (viewVec.y - vec.y) +
                    (viewVec.z - vec.z) * (viewVec.z - vec.z));
    NodeNavigationInfo *navi = m_scene->getNavigationInfoNode();
    if (navi) {
        MFFloat *avatarSize = navi->avatarSize();
        avatarSize->setValue(3, dist);
    }
    m_scene->UpdateViews(NULL, UPDATE_REDRAW_3D, NULL);
}

void 
MainWindow::changeTransparency(void)
{
    Node *node = m_scene->getSelection()->getNode();
    if (node->getType() != VRML_MATERIAL)
        return;
    OneFloatDialog dlg(m_wnd, IDD_TRANSPARENCY, 0.5, 0, 1);
    if (dlg.DoModal() == IDCANCEL)
        return;
    NodeMaterial *material = (NodeMaterial *)node;
    m_scene->setField(material, material->transparency_Field(), 
                     new SFFloat(dlg.GetValue()));
}

void MainWindow::searchInterpolator(void)
{
    Node *node = m_scene->getSelection()->getNode();
    if (!node->hasInputs())
        return;
    MyArray<Node *> targets = m_scene->searchInterpolators();
    for (size_t i = 0; i < targets.size(); i++) {
        m_scene->setSelection(targets[i]);
        m_scene->UpdateViews(NULL, UPDATE_SELECTION);
        if (i < (targets.size() - 1)) {
            char str[256], title[256], message[256];
            swLoadString(IDS_DUNE, title, 255);
            swLoadString(IDS_ASK_IF_NEXT_INTERPOLATOR, str, 255);
            mysnprintf(message, 255, str, targets.size());
            if (swMessageBox(m_wnd, message, title, SW_MB_YESNO, SW_MB_WARNING)
                == IDNO) 
                break;
        }
    }     
}

void MainWindow::searchTimeSensor(void)
{
    MyArray<Node *> targets = m_scene->searchTimeSensors();
    for (size_t i = 0; i < targets.size(); i++) {
        m_scene->setSelection(targets[i]);
        m_scene->UpdateViews(NULL, UPDATE_SELECTION);
        if (i < (targets.size() - 1)) {
            char str[256], title[256], message[256];
            swLoadString(IDS_DUNE, title, 256);
            swLoadString(IDS_ASK_IF_NEXT_TIME_SENSOR, str, 256);
            mysnprintf(message, 255, str, targets.size());
            if (swMessageBox(m_wnd, message, title, SW_MB_YESNO, SW_MB_WARNING)
                == IDNO) 
                break;
        }
    }     
}

void MainWindow::changeEnableAnimation(void)
{
    NodeTimeSensor *node = (NodeTimeSensor *)m_scene->getSelection()->getNode();
    if (node->getType() != VRML_TIME_SENSOR)
        return;
    m_scene->setField(node, node->loop_Field(), new SFBool(true));
    UpdateToolbarSelection();
}

void MainWindow::changeDisableAnimation(void)
{
    NodeTimeSensor *node = (NodeTimeSensor *)m_scene->getSelection()->getNode();
    if (node->getType() != VRML_TIME_SENSOR)
        return;
    m_scene->setField(node, node->loop_Field(), new SFBool(false));
    UpdateToolbarSelection();
}

void MainWindow::changeAnimationTime(void)
{
    NodeTimeSensor *node = (NodeTimeSensor *)m_scene->getSelection()->getNode();
    if (node->getType() != VRML_TIME_SENSOR)
        return;
    
    OneFloatDialog dlg(m_wnd, IDD_ANIMATION_TIME, 
                       node->cycleInterval()->getValue(), 0, FLT_MAX);
    if (dlg.DoModal() == IDCANCEL)
        return;
    m_scene->setField(node, node->cycleInterval_Field(), 
                     new SFTime(dlg.GetValue()));
}

void MainWindow::deleteAnimationData(void)
{
    Node *node = (Node *)m_scene->getSelection()->getNode();
    if (node == NULL)
        return;
    for (int i = 0 ; i < node->getProto()->getNumEventIns(); i++) {
        SocketList::Iterator *j;
        for (j = node->getInput(i).first(); j != NULL; j = j->next()) {
            Node *inputNode = j->item().getNode();
            if (inputNode->isInterpolator()) {
                Interpolator *interpolator = (Interpolator *)inputNode;
                int min = interpolator->findKeyInclusive(0);
                int max = interpolator->findKey(1);
                if (max == interpolator->key()->getSize())
                    max--;
                interpolator->deleteKeys(min, max);
            }
        }
    }
}

void MainWindow::changeShininess(void)
{
    Node *node = m_scene->getSelection()->getNode();
    if (node->getType() != VRML_MATERIAL)
        return;
    OneFloatDialog dlg(m_wnd, IDD_SHININESS, 0.2, 0, 1);
    if (dlg.DoModal() == IDCANCEL)
        return;
    NodeMaterial *material = (NodeMaterial *)node;
    m_scene->setField(material, material->shininess_Field(), 
                     new SFFloat(dlg.GetValue()));
}

void MainWindow::changeImageRepeat(void)
{
    Node *node = m_scene->getSelection()->getNode();
    if (node == NULL)
        return;
    if ((node->getType() != VRML_APPEARANCE) && 
        (node->getType() != TEXTURE_TRANSFORM_NODE))
        return;
    OneFloatDialog dlg(m_wnd, IDD_IMAGE_REPEAT, 1, 0);
    if (dlg.DoModal() == IDCANCEL)
        return;
    if (node->getType() == VRML_APPEARANCE) {
        insertNode(TEXTURE_TRANSFORM_NODE, "TextureTransform");
        node = ((NodeAppearance *)node)->textureTransform()->getValue();
    }
    float fscale = dlg.GetValue();
    NodeTextureTransform *texTrans = (NodeTextureTransform *)node;
    m_scene->setField(texTrans, texTrans->scale_Field(), 
                     new SFVec2f(fscale, fscale));
}

int 
MainWindow::considerIdRanges(int id)
{
    int ret = id;
    if ((id >= ID_DUNE_FILE_MRU_FIRST) && (id <= ID_DUNE_FILE_MRU_LAST))
        ret = ID_DUNE_FILE_MRU_FILE1;
    return ret;
}

void
MainWindow::OnHighlight(void *vid)
{
    if (vid == NULL) {
        if (m_statusBar)
            m_statusBar->SetText(m_statusText);
        return;
    }
    int id = swFromPtr(vid);
    static char buf[BUFSIZE];
    buf[0] = '\0';
    swLoadString(considerIdRanges(id), buf, BUFSIZE);
    char *b = strchr(buf, '\n');
    if (b) *b = '\0';
    m_statusBar->SetText(buf);
}

void
MainWindow::UpdateNumbers4Kids(void)
{
    Node *selectedNode = m_scene->getSelection()->getNode();
    bool hasNumbers = selectedNode->hasNumbers4kids() ||
                      selectedNode->getProto()->hasNumbers4kids();
    bool showNumbers = hasNumbers && m_showNumbers4Kids;
    bool visible = swIsVisible(m_fieldView->GetWindow());
    if ((!visible && showNumbers) || (visible && !showNumbers))
        toggleView(PW_RIGHT, m_fieldView, ID_DUNE_VIEW_FIELD_VIEW, 
                   "ShowFieldView");
}

void
MainWindow::OnUpdate(SceneView *sender, int type, Hint *hint)
{
    if (m_destroyed)
        return;

    Node *node = NULL;
    switch (type) {
      CASE_UPDATE(UPDATE_SELECTION)
        node = m_scene->getSelection()->getNode();
        if (m_colorCircle_active == true) {
            updateColorCircle();
            m_colorCircle_enabled = false;
            setColorCircleIcon();
            if (TheApp->is4Kids())
                if (swIsVisible(m_fieldView->GetWindow()))
                    toggleView(PW_RIGHT, m_fieldView, ID_DUNE_VIEW_FIELD_VIEW, 
                               "ShowFieldView");
         } else if ((!m_vertexModifier_overwrite) &&
                    m_vertexModifier_active == true) {
            updateVertexModifier();
            m_vertexModifier_enabled = false;
            setVertexModifierIcon();
            if (TheApp->is4Kids())
                if (swIsVisible(m_fieldView->GetWindow()))
                    toggleView(PW_RIGHT, m_fieldView, ID_DUNE_VIEW_FIELD_VIEW, 
                               "ShowFieldView");
        } else if (TheApp->is4Kids()) 
            UpdateNumbers4Kids();
        else
            m_colorCircle_enabled = false;            
        setVertexModifierIcon();
        UpdateToolbarSelection();
        RefreshProtoMenu();
        m_selectedField = -1;
        clearStatusText();
        if (node == NULL)
            return;
        RefreshHelpSelection();
        break;
      CASE_UPDATE(UPDATE_ALL)
        UpdateToolbars();
        RefreshProtoMenu();
        break;
      CASE_UPDATE(UPDATE_SELECTED_FIELD)
        {
        FieldUpdate* fieldUpdate=(FieldUpdate *)hint;
        m_selectedField = fieldUpdate->field;
        UpdateToolbars();
        }
        break;
      CASE_UPDATE(UPDATE_ENABLE_COLOR_CIRCLE)
        {
        FieldUpdate* fieldUpdate = (FieldUpdate *)hint;
        m_colorCircle_enabled = true;
        if (m_colorCircleHint)
            delete m_colorCircleHint;
        m_colorCircleHint = new FieldUpdate(fieldUpdate->node,fieldUpdate->field,
                                            fieldUpdate->index);
        }
        setColorCircleIcon();
        break;
      CASE_UPDATE(UPDATE_DISABLE_COLOR_CIRCLE)      
        m_colorCircle_enabled = false;
        setColorCircleIcon();
        break;
      CASE_UPDATE(UPDATE_CLOSE_COLOR_CIRCLE)
        updateColorCircle();
        setColorCircleIcon();
        if (TheApp->is4Kids())
            if (swIsVisible(m_fieldView->GetWindow()))
                toggleView(PW_RIGHT, m_fieldView, ID_DUNE_VIEW_FIELD_VIEW, 
                           "ShowFieldView");
        break;
      CASE_UPDATE(UPDATE_CLOSE_VERTEX_MODIFIER)
        updateVertexModifier();
        setVertexModifierIcon();
        if (TheApp->is4Kids())
            if (swIsVisible(m_fieldView->GetWindow()))
                toggleView(PW_RIGHT, m_fieldView, ID_DUNE_VIEW_FIELD_VIEW, 
                           "ShowFieldView");
        break;
      CASE_UPDATE(UPDATE_SOLID_CHANGED)
        if (TheApp->is4Catt())
            UpdateToolbars();
        break;
      CASE_UPDATE(UPDATE_FIELD)
        {
        FieldUpdate* fieldUpdate=(FieldUpdate *)hint;
        int field = fieldUpdate->field;
        Node *node = fieldUpdate->node;    
        if (node->getProto()->getField(field)->getFlags() & FF_UPDATE) {
            UpdateToolbarSelection();
            UpdateMenuVisible();
        }
        }
        break;
      CASE_UPDATE(UPDATE_REMOVE_NODE)
        if (((NodeUpdate *) hint)->node->getType() == DUNE_VRML_CUT)
            m_vrmlCutNode = NULL;
      CASE_UPDATE(UPDATE_TOOLBAR)
        setColorCircleIcon();
        UpdateToolbarSelection();
        UpdateMenuVisible();
        break;
    }
}

void
MainWindow::OnSize(int width, int height)
{
    PanedWindow::OnSize(width, height);
    if (!swIsMaximized(m_wnd)) {
        swGetTotalSize(m_wnd, &width, &height);
        TheApp->SetIntPreference("WindowWidth", width);
        TheApp->SetIntPreference("WindowHeight", height);
    }
}

void
MainWindow::toggleCreateAtZero(int index)
{
        bool flag =  TheApp->getCreateAtZero(index);
        TheApp->setCreateAtZero(index, !flag);
        UpdateToolbars();
}

bool isToNurbsAllowedParent(Node *parent)
{
    if ( (parent == parent->getScene()->getRoot()) ||
         (parent->getType() == VRML_GROUP) ||
         (parent->getType() == VRML_TRANSFORM) ||
         (parent->getType() == VRML_ANCHOR) ||
         (parent->getType() == VRML_SHAPE) ||
         (parent->getType() == VRML_CONTOUR_2D) )
       return true;
    else
       return false;
}

bool 
MainWindow::getValidTexture(void)
{
    Node *node = m_scene->getSelection()->getNode();
    if (node == NULL)
        return false;
    int field = m_scene->getSelection()->getField();
    bool validTexture = false;
    if (field != -1) 
        if (node->validChildType(field, TEXTURE_NODE))
            validTexture = true;
    if (node->findFirstValidFieldType(TEXTURE_NODE) != -1)
        validTexture = true;
    return validTexture;
}


bool 
MainWindow::getValidTexture3D(void)
{
    Node *node = m_scene->getSelection()->getNode();
    int field = m_scene->getSelection()->getField();
    bool validTexture = false;
    if (field != -1) 
        if (node->validChildType(field, TEXTURE_3D_NODE))
            validTexture = true;
    if (node->findFirstValidFieldType(TEXTURE_3D_NODE) != -1)
        validTexture = true;
    return validTexture;
}

void MainWindow::UpdateStaticMenuCoverNodes(void)
{
    int disabled = TheApp->getCoverMode() ? 0 : SW_MENU_DISABLED;
    swMenuSetFlags(m_menu, ID_DUNE_VIEW_NODE_TOOLBAR_COVER, SW_MENU_DISABLED,  
                   disabled);   
    swMenuSetFlags(m_menu, ID_NEW_COVER_SKY, SW_MENU_DISABLED, 
                   disabled);
    swMenuSetFlags(m_menu, ID_NEW_COVER_LAB_VIEW, SW_MENU_DISABLED, 
                   disabled);
    swMenuSetFlags(m_menu, ID_NEW_COVER_COVER, SW_MENU_DISABLED, 
                   disabled);
    swMenuSetFlags(m_menu, ID_NEW_COVER_SPACE_SENSOR, SW_MENU_DISABLED, 
                   disabled);
    swMenuSetFlags(m_menu, ID_NEW_COVER_AR_SENSOR, SW_MENU_DISABLED, 
                   disabled);
    swMenuSetFlags(m_menu, ID_NEW_COVER_JOYSTICK_SENSOR, SW_MENU_DISABLED, 
                   disabled);
    swMenuSetFlags(m_menu, ID_NEW_COVER_STEERING_WHEEL, SW_MENU_DISABLED, 
                   disabled);
    swMenuSetFlags(m_menu, ID_NEW_COVER_VEHICLE, SW_MENU_DISABLED, 
                   disabled);
    swMenuSetFlags(m_menu, ID_NEW_COVER_LAB_VIEW, SW_MENU_DISABLED,
                   disabled);
    swMenuSetFlags(m_menu, ID_NEW_COVER_VIRTUAL_ACOUSTICS, SW_MENU_DISABLED,
                   disabled);
    swMenuSetFlags(m_menu, ID_NEW_COVER_TUI_BUTTON, SW_MENU_DISABLED,
                   disabled);
    swMenuSetFlags(m_menu, ID_NEW_COVER_TUI_COMBO_BOX, SW_MENU_DISABLED,
                   disabled);
    swMenuSetFlags(m_menu, ID_NEW_COVER_TUI_FLOAT_SLIDER, SW_MENU_DISABLED,
                   disabled);
    swMenuSetFlags(m_menu, ID_NEW_COVER_TUI_FRAME, SW_MENU_DISABLED,
                   disabled);
    swMenuSetFlags(m_menu, ID_NEW_COVER_TUI_LABEL, SW_MENU_DISABLED,
                   disabled);
    swMenuSetFlags(m_menu, ID_NEW_COVER_TUI_LIST_BOX, SW_MENU_DISABLED,
                   disabled);
    swMenuSetFlags(m_menu, ID_NEW_COVER_TUI_MAP, SW_MENU_DISABLED,
                   disabled);
    swMenuSetFlags(m_menu, ID_NEW_COVER_TUI_PROGRESS_BAR, SW_MENU_DISABLED,
                   disabled);
    swMenuSetFlags(m_menu, ID_NEW_COVER_TUI_SLIDER, SW_MENU_DISABLED,
                   disabled);
    swMenuSetFlags(m_menu, ID_NEW_COVER_TUI_SPLITTER, SW_MENU_DISABLED,
                   disabled);
    swMenuSetFlags(m_menu, ID_NEW_COVER_TUI_TAB, SW_MENU_DISABLED,
                   disabled);
    swMenuSetFlags(m_menu, ID_NEW_COVER_TUI_TAB_FOLDER, SW_MENU_DISABLED,
                   disabled);
    swMenuSetFlags(m_menu, ID_NEW_COVER_TUI_TOGGLE_BUTTON, SW_MENU_DISABLED,
                   disabled);
}


void MainWindow::UpdateMenuCoverNodes(void)
{
    Node *node = m_scene->getSelection()->getNode();
    if (node == NULL)
        return;
    int field = m_scene->getSelection()->getField();
    bool validTexture = getValidTexture();
    if (!validTexture) 
        validTexture = (node->findValidFieldType(TEXTURE_NODE) != -1);
    else
        if (!node->validChildType(field, TEXTURE_NODE))
            validTexture = false;
    swMenuSetFlags(m_menu, ID_NEW_COVER_CUBE_TEXTURE, SW_MENU_DISABLED, 
                   validTexture && 
                   TheApp->getCoverMode() ? 0 : SW_MENU_DISABLED);
    swToolbarSetButtonFlags(m_nodeToolbarCover, 2, SW_TB_DISABLED,
                            validTexture &&
                            TheApp->getCoverMode() ? 0 : SW_TB_DISABLED);

#ifdef HAVE_COVER_WAVE
    bool validEffect = TheApp->getCoverMode() &&
                       (node->findValidFieldType(COVER_WAVE) != -1);
    swMenuSetFlags(m_menu, ID_NEW_COVER_WAVE, SW_MENU_DISABLED, 
                   validEffect ? 0 : SW_MENU_DISABLED);
    swToolbarSetButtonFlags(m_nodeToolbarCover, 4, SW_TB_DISABLED,
                            validEffect ? 0 : SW_TB_DISABLED);
#endif

    swMenuSetFlags(m_menu, ID_HELP_SELECTION_COVER, SW_MENU_DISABLED, 
                   TheApp->getCoverMode() && 
                   (node->hasCoverFields() || node->isCoverNode()) ? 
                   0 : SW_MENU_DISABLED);

    if (!TheApp->getCoverMode())
        swMenuSetFlags(m_menu, ID_NEW_COVER_VIRTUAL_SOUND_SOURCE, 
                       SW_MENU_DISABLED, SW_MENU_DISABLED);

}

void MainWindow::UpdateStaticMenuKambiNodes(void)
{
    int disabled = TheApp->getKambiMode() ? 0 : SW_MENU_DISABLED;
    swMenuSetFlags(m_menu, ID_NEW_KAMBI_TEAPOT, SW_MENU_DISABLED, disabled);
    swMenuSetFlags(m_menu, ID_NEW_KAMBI_TEXT_3D, SW_MENU_DISABLED, disabled);
    swMenuSetFlags(m_menu, ID_NEW_KAMBI_INLINE, SW_MENU_DISABLED, disabled);
    swMenuSetFlags(m_menu, ID_NEW_KAMBI_MATRIX_TRANSFORM, SW_MENU_DISABLED, disabled);
    swMenuSetFlags(m_menu, ID_DUNE_VIEW_NODE_TOOLBAR_KAMBI, SW_MENU_DISABLED, disabled);   
    swMenuSetFlags(m_menu, ID_NEW_KAMBI_HEAD_LIGHT, SW_MENU_DISABLED, disabled);
    swMenuSetFlags(m_menu, ID_NEW_KAMBI_TRIANGULATION, SW_MENU_DISABLED, disabled);
    swMenuSetFlags(m_menu, ID_NEW_KAMBI_NAVIGATION_INFO, SW_MENU_DISABLED, disabled);
}

void 
MainWindow::UpdateMenuKambiNodes(void)
{
    Node *node = m_scene->getSelection()->getNode();
    bool kambi = TheApp->getKambiMode();
    bool x3dom = TheApp->getX3domMode();
    swMenuSetFlags(m_menu, ID_NEW_KAMBI_APPEARANCE, SW_MENU_DISABLED,
                   kambi && (node->getType() == VRML_SHAPE) ? 0 : SW_MENU_DISABLED);
    swMenuSetFlags(m_menu, ID_NEW_KAMBI_BLEND_MODE,  SW_MENU_DISABLED,
                   kambi && (node->getType() == KAMBI_KAMBI_APPEARANCE) ? 0 : SW_MENU_DISABLED);
    swMenuSetFlags(m_menu, ID_NEW_KAMBI_OCTREE_PROPERTIES,  SW_MENU_DISABLED,
                   kambi && ((node->getType() == KAMBI_KAMBI_NAVIGATION_INFO) ||
                   (node->getType() == VRML_SHAPE)) ? 0 : SW_MENU_DISABLED);
    swMenuSetFlags(m_menu, ID_NEW_KAMBI_GENERATED_SHADOW_MAP, SW_MENU_DISABLED,
                   kambi && ((node->getType() == KAMBI_KAMBI_APPEARANCE) ||
                   (node->getType() == VRML_APPEARANCE)) ? 0 : SW_MENU_DISABLED);
    swMenuSetFlags(m_menu, ID_NEW_KAMBI_RENDERED_TEXTURE, SW_MENU_DISABLED,
                   (kambi || x3dom) && 
                   ((node->getType() == KAMBI_KAMBI_APPEARANCE) ||
                    ((node->getType() == VRML_APPEARANCE) ? 
                      0 : SW_MENU_DISABLED)));
}

void MainWindow::UpdateStaticMenuX3domNodes(void)
{
    int disabled = TheApp->getX3domMode() ? 0 : SW_MENU_DISABLED;
    swMenuSetFlags(m_menu, ID_NEW_COLOR_MASK_MODE, SW_MENU_DISABLED, disabled);
    swMenuSetFlags(m_menu, ID_NEW_DEPTH_MODE, SW_MENU_DISABLED, disabled);
    swMenuSetFlags(m_menu, ID_NEW_COMMON_SURFACE_SHADER, SW_MENU_DISABLED,
                   disabled);
    swMenuSetFlags(m_menu, ID_NEW_MULTI_PART, SW_MENU_DISABLED, disabled);
    swMenuSetFlags(m_menu, ID_NEW_PARTICLE_SET, SW_MENU_DISABLED, disabled);
    swMenuSetFlags(m_menu, ID_NEW_POP_GEOMETRY_LEVEL, SW_MENU_DISABLED, 
                   disabled);
    swMenuSetFlags(m_menu, ID_NEW_REFINEMENT_TEXTURE, SW_MENU_DISABLED,
                   disabled);
}

void 
MainWindow::UpdateMenuX3domNodes(void)
{
    Node *node = m_scene->getSelection()->getNode();
    bool x3dom = TheApp->getX3domMode();
    swMenuSetFlags(m_menu, ID_NEW_COLOR_MASK_MODE, SW_MENU_DISABLED,
                   x3dom && (node->getType() == VRML_APPEARANCE) ? 
                   0 : SW_MENU_DISABLED);
    swMenuSetFlags(m_menu, ID_NEW_DEPTH_MODE, SW_MENU_DISABLED,
                   x3dom && (node->getType() == VRML_APPEARANCE) ? 
                   0 : SW_MENU_DISABLED);
    swMenuSetFlags(m_menu, ID_NEW_COMMON_SURFACE_SHADER, SW_MENU_DISABLED,
                   x3dom && (node->getType() == VRML_APPEARANCE) ? 
                   0 : SW_MENU_DISABLED);
    swMenuSetFlags(m_menu, ID_NEW_MULTI_PART, SW_MENU_DISABLED,
                   x3dom && (node->getType() == VRML_INLINE) ? 
                   0 : SW_MENU_DISABLED);
    swMenuSetFlags(m_menu, ID_NEW_PARTICLE_SET, SW_MENU_DISABLED,
                   x3dom && (node->getType() == VRML_POINT_SET) ? 
                   0 : SW_MENU_DISABLED);
    swMenuSetFlags(m_menu, ID_NEW_POP_GEOMETRY_LEVEL, SW_MENU_DISABLED,
                   x3dom && (node->getType() == X3DOM_POP_GEOMETRY) ? 
                   0 : SW_MENU_DISABLED);
}

void MainWindow::UpdateMenuVisible(void)
{

    swMenuSetFlags(m_menu, ID_ROUTE_ZOOM_IN, SW_MENU_DISABLED,  
                   swIsVisible(m_graphView->GetWindow()) && 
                   m_graphView->canZoomIn() ? 0 : SW_MENU_DISABLED); 
    swMenuSetFlags(m_menu, ID_ROUTE_ZOOM_OUT, SW_MENU_DISABLED,  
                   swIsVisible(m_graphView->GetWindow()) ? 0 : SW_MENU_DISABLED);
    swMenuSetFlags(m_menu, ID_ROUTE_UNZOOM, SW_MENU_DISABLED,  
                   swIsVisible(m_graphView->GetWindow()) ? 0 : SW_MENU_DISABLED);
}

void
MainWindow::UpdateObjectEdit(const Path *selection)
{
    Node *node = selection->getNode();
    int field = m_selectedField;
    bool editObject_flag = false;
    bool editUrl_flag = false;
    if (!isEditorInUse()) {
        if (selection->isProto(m_scene)) 
            editObject_flag = true;
        else {
            switch (node->getType()) {
              case VRML_SCRIPT:
              case X3D_SHADER_PROGRAM:
              case X3D_PACKAGED_SHADER:
                editObject_flag = true;
              case VRML_IMAGE_TEXTURE:
              case VRML_MOVIE_TEXTURE:
              case VRML_AUDIO_CLIP:
                editUrl_flag = true;
                break;
              case X3D_SHADER_PART:
                editUrl_flag = true;
                break;
              case X3D_COMPOSED_SHADER:
                editObject_flag = true;
                break;
              case VRML_TEXT:
                {
                NodeText *text = (NodeText *)node;
                if (field == text->string_Field())
                    editObject_flag = true;
                }
                break;
              case KAMBI_TEXT_3D:
                {
                NodeText3D *text = (NodeText3D *)node;
                if (field == text->string_Field())
                    editObject_flag = true;
                }
                break;
              case VRML_WORLD_INFO:
                {
                NodeWorldInfo *world = (NodeWorldInfo *)node;
                if (field == world->info_Field())
                    editObject_flag = true;
                }
                break;
            }
        }
    }
    swMenuSetFlags(m_menu, ID_OBJECT_EDIT, SW_MENU_DISABLED, 
                   editObject_flag ? 0 : SW_MENU_DISABLED);
    swToolbarSetButtonFlags(m_standardToolbar, m_objectEditIconPos, 
                            SW_TB_DISABLED, 
                            editObject_flag ? 0 : SW_TB_DISABLED);
    swMenuSetFlags(m_menu, ID_URL_EDIT, SW_MENU_DISABLED, 
                   editUrl_flag ? 0 : SW_MENU_DISABLED);
    swToolbarSetButtonFlags(m_standardToolbar, m_urlEditIconPos, SW_TB_DISABLED, 
                            editUrl_flag ? 0 : SW_TB_DISABLED);
}


//
// UpdateToolbars(void)
//
// updates all the "insert"-type buttons (eg., appearance, material, etc)
// by checking to see if the current selection contains a field which
// can hold this type.
//
// Also update menus.
//

void
MainWindow::UpdateToolbars(void)
{
    UpdateToolbarSelection();

    UpdateMenuVisible();

    swMenuSetFlags(m_menu, ID_DUNE_EDIT_FIND_AGAIN, SW_MENU_DISABLED, 
                   m_searchText.length() == 0 ? SW_MENU_DISABLED : 0);

    swMenuSetFlags(m_menu, ID_CREATE_AT_ZERO_X, SW_MENU_CHECKED, 
                   TheApp->getCreateAtZero(0) ? SW_MENU_CHECKED : 0);
    swMenuSetFlags(m_menu, ID_CREATE_AT_ZERO_Y, SW_MENU_CHECKED, 
                   TheApp->getCreateAtZero(1) ? SW_MENU_CHECKED : 0);
    swMenuSetFlags(m_menu, ID_CREATE_AT_ZERO_Z, SW_MENU_CHECKED, 
                   TheApp->getCreateAtZero(2) ? SW_MENU_CHECKED : 0);

    swMenuSetFlags(m_menu, ID_DUNE_SELECTION_VERTICES, SW_MENU_CHECKED, 
                   m_scene->getSelectionMode() == SELECTION_MODE_VERTICES ? 
                   SW_MENU_CHECKED : 0);
    swMenuSetFlags(m_menu, ID_DUNE_SELECTION_FACES, SW_MENU_CHECKED, 
                   m_scene->getSelectionMode() == SELECTION_MODE_FACES ? 
                   SW_MENU_CHECKED : 0);
    swMenuSetFlags(m_menu, ID_DUNE_SELECTION_LINES, SW_MENU_CHECKED, 
                   m_scene->getSelectionMode() == SELECTION_MODE_LINES ? 
                   SW_MENU_CHECKED : 0);
    swMenuSetFlags(m_menu, ID_DUNE_SELECTION_HANIM_JOINT_WEIGHT, 
                   SW_MENU_CHECKED, m_scene->getSelectionMode() == 
                   SELECTION_HANIM_JOINT_WEIGHT ? SW_MENU_CHECKED : 0);

    swMenuSetFlags(m_menu, ID_INPUTDEVICE_GREATER, SW_MENU_DISABLED, 
                   TheApp->hasInputDevices() ? 0 : SW_MENU_DISABLED);
    swToolbarSetButtonFlags(m_standardToolbar, m_inputDeviceGreaterIconPos, 
                            SW_TB_DISABLED, 
                            TheApp->hasInputDevices() ? 0 : SW_TB_DISABLED);
    swMenuSetFlags(m_menu, ID_INPUTDEVICE_LESSER, SW_MENU_DISABLED, 
                   TheApp->hasInputDevices() ? 0 : SW_MENU_DISABLED);
    swToolbarSetButtonFlags(m_standardToolbar, m_inputDeviceLesserIconPos, 
                            SW_TB_DISABLED, 
                            TheApp->hasInputDevices() ? 0 : SW_TB_DISABLED);

//    swMenuSetFlags(m_menu, ID_OPTIONS_STEREO_VIEW, SW_MENU_DISABLED,
//                   TheApp->canStereo() ? 0 : SW_MENU_DISABLED);

    UpdateToolbar(ID_NAVIGATION_OPERATIONS);
    
    UpdateToolbar(ID_X_ONLY);
    UpdateToolbar(ID_Y_ONLY);
    UpdateToolbar(ID_Z_ONLY);

    UpdateToolbar(ID_INPUT_DEVICE_NAVIGATION_MODE);

    UpdateMenuCoverNodes();
}

void 
MainWindow::InitToolbar(STOOLBAR toolbar, NodeButton *buttons, int count)
{
    for (int i = 0; i < count; i++) {
        buttons[i].enabled = true;
        swToolbarSetButtonFlags(toolbar, i, SW_TB_DISABLED, 0);
    }
}

void
MainWindow::InitToolbars(void)
{
    int arraySize = ARRAYSIZE(standardButtons) / 2;
    if (TheApp->is4Kids()) {
        if (TheApp->hasInputDevices())
            arraySize = ARRAYSIZE(standardButtons4KidsWithInputDevices) / 2;
        else
            arraySize = ARRAYSIZE(standardButtons4Kids) / 2;
        for (int i = 0; i < arraySize; i++)
            swToolbarSetButtonFlags(m_standardToolbar, i, SW_TB_DISABLED, 0);
    } else {
        for (int i = 0; i < arraySize; i++)
            swToolbarSetButtonFlags(m_standardToolbar, i, SW_TB_DISABLED, 0);
        InitToolbar(m_nodeToolbar1, buttons1, ARRAYSIZE(buttons1));
        InitToolbar(m_nodeToolbar2, buttons2, ARRAYSIZE(buttons2));
        InitToolbar(m_nodeToolbar3, buttons3, ARRAYSIZE(buttons3));
        InitToolbar(m_nodeToolbarVRML200x, buttonsVRML200x, 
                    ARRAYSIZE(buttonsVRML200x));
        InitToolbar(m_nodeToolbarX3DComponents1, buttonsX3DComponents1, 
                    ARRAYSIZE(buttonsX3DComponents1));
        InitToolbar(m_nodeToolbarX3DComponents2, buttonsX3DComponents2, 
                    ARRAYSIZE(buttonsX3DComponents2));
        InitToolbar(m_nodeToolbarX3DComponents3, buttonsX3DComponents3, 
                    ARRAYSIZE(buttonsX3DComponents3));
        InitToolbar(m_nodeToolbarX3DComponents4, buttonsX3DComponents4, 
                    ARRAYSIZE(buttonsX3DComponents4));
        InitToolbar(m_nodeToolbarScripted, buttonsScripted, 
                    ARRAYSIZE(buttonsScripted));
        InitToolbar(m_nodeToolbarCover, buttonsCover, ARRAYSIZE(buttonsCover));
        InitToolbar(m_nodeToolbarKambi, buttonsKambi, ARRAYSIZE(buttonsKambi));
        InitToolbar(m_nodeToolbarKambi, buttonsX3dom, ARRAYSIZE(buttonsX3dom));
    }
}

void
MainWindow::UpdateToolbar(int id)
{
    switch (id) {
      case ID_X_ONLY:
        swMenuSetFlags(m_menu, ID_X_ONLY, SW_MENU_CHECKED,
                       m_xOnly ? SW_MENU_CHECKED : 0);
        swToolbarSetButtonFlags(m_standardToolbar, m_xOnlyIconPos, 
                                SW_TB_CHECKED, m_xOnly ? SW_TB_CHECKED :  0);
        break;
      case ID_Y_ONLY:
        swMenuSetFlags(m_menu, ID_Y_ONLY, SW_MENU_CHECKED,
                       m_yOnly ? SW_MENU_CHECKED : 0);
        swToolbarSetButtonFlags(m_standardToolbar, m_yOnlyIconPos, 
                                SW_TB_CHECKED, m_yOnly ? SW_TB_CHECKED :  0);
        break;
      case ID_Z_ONLY:
        swMenuSetFlags(m_menu, ID_Z_ONLY, SW_MENU_CHECKED,
                       m_zOnly ? SW_MENU_CHECKED : 0);
        swToolbarSetButtonFlags(m_standardToolbar, m_zOnlyIconPos, 
                                SW_TB_CHECKED, m_zOnly ? SW_TB_CHECKED :  0);
        break;
     case ID_OBJECT_EDIT:
        UpdateObjectEdit(m_scene->getSelection());
        break;
      case ID_DUNE_FILE_UPLOAD:
        swMenuSetFlags(m_menu, ID_DUNE_FILE_UPLOAD, SW_MENU_DISABLED, 
                      TheApp->hasUpload() == 0 ? SW_MENU_DISABLED : 0);
        break;
      case ID_INPUT_DEVICE_NAVIGATION_MODE:
        if (TheApp->getMaxNumberAxesInputDevices()>=2) { 
            swMenuSetFlags(m_menu, ID_INPUT_DEVICE_NAVIGATION_MODE, 
                           SW_MENU_CHECKED, m_navigation_input_device_active ? 
                           SW_MENU_CHECKED : 0);
            swToolbarSetButtonFlags(m_standardToolbar, 
                                    m_navigationInputDeviceIconPos, 
                                    SW_TB_CHECKED,
                                    m_navigation_input_device_active ? 
                                    SW_TB_CHECKED : 0);
        } else {
            swMenuSetFlags(m_menu, ID_INPUT_DEVICE_NAVIGATION_MODE, 
                           SW_MENU_DISABLED, SW_MENU_DISABLED);
            swToolbarSetButtonFlags(m_standardToolbar, 
                                    m_navigationInputDeviceIconPos, 
                                    SW_TB_DISABLED, SW_TB_DISABLED);
        }
        break;
      case ID_NAVIGATION_OPERATIONS:
        {    
          bool isFly = TheApp->GetMouseMode() == MOUSE_FLY;
          swMenuSetFlags(m_menu, ID_FLY_MOUSE_MODE, SW_MENU_CHECKED, 
                         isFly ? SW_MENU_CHECKED : 0);
          swToolbarSetButtonFlags(m_standardToolbar, m_mouseFlyIconPos, 
                                  SW_TB_CHECKED, isFly ? SW_TB_CHECKED :  0);

          bool isExamine = TheApp->GetMouseMode() == MOUSE_EXAMINE;
          swMenuSetFlags(m_menu, ID_EXAMINE_MOUSE_MODE, SW_MENU_CHECKED, 
                         isExamine ? SW_MENU_CHECKED : 0);
          swToolbarSetButtonFlags(m_standardToolbar, m_mouseExamineIconPos, 
                                  SW_TB_CHECKED, isExamine ? SW_TB_CHECKED : 0);

          bool isWalk = TheApp->GetMouseMode() == MOUSE_WALK;
          swMenuSetFlags(m_menu, ID_WALK_MOUSE_MODE, SW_MENU_CHECKED, 
                         isWalk ? SW_MENU_CHECKED : 0);
          swToolbarSetButtonFlags(m_standardToolbar, m_mouseWalkIconPos, 
                                  SW_TB_CHECKED, isWalk ? SW_TB_CHECKED :  0);

          bool isRoll = TheApp->GetMouseMode() == MOUSE_ROLL;
          swMenuSetFlags(m_menu, ID_ROLL_MOUSE_MODE, SW_MENU_CHECKED, 
                         isRoll ? SW_MENU_CHECKED : 0);
          swToolbarSetButtonFlags(m_standardToolbar, m_mouseRollIconPos, 
                                  SW_TB_CHECKED, isRoll ? SW_TB_CHECKED :  0);
          bool isFollow = TheApp->GetMouseMode() == MOUSE_FOLLOW;
          swMenuSetFlags(m_menu, ID_FOLLOW_MOUSE_MODE, SW_MENU_CHECKED, 
                         isFollow ? SW_MENU_CHECKED : 0);
          swToolbarSetButtonFlags(m_standardToolbar, m_mouseFollowIconPos, 
                                  SW_TB_CHECKED, isFollow ? SW_TB_CHECKED :  0);

          swMenuSetFlags(m_menu, ID_MOUSE_NAVIGATION_MODE, SW_MENU_CHECKED,
                         m_navigation_mouse_active ? SW_MENU_CHECKED : 0);
          swToolbarSetButtonFlags(m_standardToolbar, m_navigationMouseIconPos, 
                                  SW_TB_CHECKED, m_navigation_mouse_active ? 
                                  SW_TB_CHECKED :  0);
          }
        break;
      case ID_SKIP_MATERIAL_NAME_BEFORE_FIRST_UNDERSCORE:
        swMenuSetFlags(m_menu, ID_SKIP_MATERIAL_NAME_BEFORE_FIRST_UNDERSCORE, 
                       SW_MENU_CHECKED, 
                       TheApp->SkipMaterialNameBeforeFirstUnderscore() ? 
                       SW_MENU_CHECKED : 0);
        break;
      case ID_SKIP_MATERIAL_NAME_AFTER_LAST_UNDERSCORE:
        swMenuSetFlags(m_menu, ID_SKIP_MATERIAL_NAME_AFTER_LAST_UNDERSCORE, 
                       SW_MENU_CHECKED,
                       TheApp->SkipMaterialNameAfterLastUnderscore() 
                       ? SW_MENU_CHECKED : 0);
        break;
      default:
        swDebugf("unhandled call to MainWindow::UpdateToolbar(%d)\n", id);
    }
}

bool 
MainWindow::isValidNodeType(Node* node, int field)
{
    bool valid = false;
    if (node->findFirstValidFieldType(node->getType()) != -1)
        valid = true;
    if (field != -1)
        if (node->validChildType(field, node->getType()))
            valid = true;
    return valid;
}

void
MainWindow::UpdateToolbar(STOOLBAR toolbar, Node *node, int field,
                          NodeButton *buttons, int count) 
{
    if (node == NULL)
        return;
    for (int i = 0; i < count; i++) { 
        bool valid = true;
        switch (buttons[i].type) {
          case VRML_AUDIO_CLIP:
            valid = (node->getType() == VRML_SOUND);
            break;
          case X3D_BOUNDED_PHYSICS_MODEL:
          case X3D_FORCE_PHYSICS_MODEL:
          case X3D_WIND_PHYSICS_MODEL:
            valid = m_scene->isX3d();
            if (!valid)
                break;
            valid = node->findValidFieldType(PARTICLE_PHYSICS_MODEL_NODE) 
                    != -1;
            break;
          case X3D_CONTOUR_POLYLINE_2D:
            valid = node->findValidFieldType(NURBS_CONTROL_CURVE_NODE) != 
                    -1;
            break;
          case X3D_COLOR_RGBA:
            valid = m_scene->isX3d();
            if (!valid)
                break;
          case VRML_COLOR:
            valid = node->findValidFieldType(COLOR_NODE) != -1;
            break;
          case X3D_COORDINATE_DOUBLE:
            valid = m_scene->isX3d();
            if (!valid)
                break;
          case VRML_COORDINATE:
          case VRML_GEO_COORDINATE:
            valid = node->findValidFieldType(COORDINATE_NODE) != -1;
            break;
          case VRML_MOVIE_TEXTURE:
            valid = getValidTexture();
            if (node->getType() == VRML_SOUND)
                valid = true;
            break;
          case VRML_IMAGE_TEXTURE:
          case VRML_PIXEL_TEXTURE:
          case X3DOM_IMAGE_TEXTURE_ATLAS:                
          case X3DOM_REFINEMENT_TEXTURE:                
            valid = getValidTexture();
            break;
          case VRML_TEXTURE_COORDINATE:
            valid = node->findValidFieldType(TEXTURE_COORDINATE_NODE) != -1;
            if (field != -1)
                if (node->validChildType(field, TEXTURE_COORDINATE_NODE))
                    valid = true;
            break;
          case X3D_TEXTURE_COORDINATE_GENERATOR:
            valid = node->findValidFieldType(
                          GENERATED_TEXTURE_COORDINATE_NODE) != -1;
            if (field != -1)
                if (node->validChildType(field, 
                                         GENERATED_TEXTURE_COORDINATE_NODE))
                    valid = true;
            if (valid)
                break;
            break;
          case X3D_TEXTURE_COORDINATE_3D:
          case X3D_TEXTURE_COORDINATE_4D:
            valid = m_scene->isX3d();
            if (!valid)
                break;
            valid = node->findValidFieldType(TEXTURE_COORDINATE_NODE) != -1;
            if (field != -1)
                if (node->validChildType(field, TEXTURE_COORDINATE_NODE))
                    valid = true;
            break;
          case X3D_TEXTURE_TRANSFORM_3D:
          case X3D_TEXTURE_TRANSFORM_MATRIX_3D:
            valid = m_scene->isX3d();
            if (!valid)
                break;
          case VRML_TEXTURE_TRANSFORM:
            valid = node->findFirstValidFieldType(TEXTURE_TRANSFORM_NODE) 
                    != -1;
            if (field != -1)
                if (node->validChildType(field, TEXTURE_TRANSFORM_NODE))
                    valid = true;
            break;
          case X3D_MULTI_TEXTURE:
            valid = m_scene->isX3d() && getValidTexture() && 
                    (node->getType() != X3D_MULTI_TEXTURE);
            break;
          case X3D_MULTI_TEXTURE_COORDINATE:
            valid = m_scene->isX3d() &&  
                    (node->findFirstValidFieldType(TEXTURE_COORDINATE_NODE) != 
                     -1) && 
                    (node->getType() != X3D_MULTI_TEXTURE_COORDINATE);
            break;
          case X3D_MULTI_TEXTURE_TRANSFORM:
            valid = m_scene->isX3d() &&  
                    (node->findFirstValidFieldType(TEXTURE_TRANSFORM_NODE) != 
                     -1) && 
                    (node->getType() != X3D_MULTI_TEXTURE_TRANSFORM);
            break;
          case X3D_TWO_SIDED_MATERIAL:
            valid = m_scene->isX3d();
            if (!valid)
                break;
          case VRML_MATERIAL:
            valid = node->findValidFieldType(MATERIAL_NODE) != -1;
            break;
          case X3D_NURBS_TEXTURE_COORDINATE:
            valid = valid &&
                    node->findFirstValidFieldType(
                                NURBS_TEXTURE_COORDINATE_NODE) != -1;
            break;
          case VRML_NURBS_TEXTURE_SURFACE:
            valid = node->findFirstValidFieldType(
                              NURBS_TEXTURE_COORDINATE_NODE) != -1;
            break;
          case X3D_NURBS_TRIMMED_SURFACE:
          case X3D_NURBS_SWEPT_SURFACE:
          case X3D_NURBS_SWUNG_SURFACE:
            valid = m_scene->isX3d();
            break;
          case VRML_NURBS_CURVE_2D:
            valid = (node->findFirstValidFieldType(NURBS_CONTROL_CURVE_NODE)
                     != -1);
            valid = valid || 
                    (node->findFirstValidFieldType(VRML_NURBS_CURVE_2D) != 
                     -1);
            break;
          case VRML_CONTOUR_2D:
            valid = (node->getType() == VRML_TRIMMED_SURFACE) ||
                    (node->getType() == X3D_NURBS_TRIMMED_SURFACE);
            break;
          case VRML_POLYLINE_2D:
            valid = m_scene->isX3d();
            if (!valid)
                break;
            break;
          case X3D_RIGID_BODY:
            valid = m_scene->isX3d();
            if (!valid)
                break;
            valid = node->findValidFieldType(X3D_RIGID_BODY) != -1;
            if (!valid) {
                if (node->matchNodeClass(RIGID_JOINT_NODE)) {
                    X3DRigidJointNode *joint = (X3DRigidJointNode *)node;
                    if (joint->body1()->getValue() == NULL)
                        valid = true;
                    else if (joint->body2()->getValue() == NULL)
                        valid = true; 
                }
                if (node->getType() == X3D_CONTACT) {
                    NodeContact *contact = (NodeContact *)node;
                    if (contact->body1()->getValue() == NULL)
                        valid = true;                            
                    else if (contact->body2()->getValue() == NULL)
                        valid = true;
                }
            }
            break;
          case X3D_BALL_JOINT:
          case X3D_DOUBLE_AXIS_HINGE_JOINT:
          case X3D_MOTOR_JOINT:
          case X3D_SINGLE_AXIS_HINGE_JOINT:
          case X3D_SLIDER_JOINT:
          case X3D_UNIVERSAL_JOINT:
            valid = m_scene->isX3d();
            if (!valid)
                break;
            valid = (node->getType() == X3D_RIGID_BODY_COLLECTION);
            break;
          case X3D_COLLISION_COLLECTION:
            valid = m_scene->isX3d();
            if (!valid)
                break;
            valid = false;
            if (node->getType() == X3D_RIGID_BODY_COLLECTION) {
                NodeRigidBodyCollection *coll = (NodeRigidBodyCollection *)node;
                if (coll->collider()->getValue() == NULL)
                    valid = true;                            
            }
            if (node->getType() == X3D_COLLISION_SENSOR) {
                NodeCollisionSensor *sensor = (NodeCollisionSensor *)node;
                if (sensor->collider()->getValue() == NULL)
                    valid = true;                            
            }
            break;
          case X3D_COLLISION_SPACE:
            valid = m_scene->isX3d();
            if (!valid)
                break;
            valid = false;
            if (node->getType() == X3D_COLLISION_COLLECTION)
                valid = true;
            if (node->getType() == X3D_COLLISION_SPACE)
                valid = true;
            break;
          case X3D_HANIM_JOINT:
            valid = false;
            if (node->getType() == X3D_HANIM_HUMANOID)
                valid = true;
            if (node->getType() == X3D_HANIM_JOINT)
                valid = true;
            if (node->getType() == VRML_COORDINATE)
                valid = true;
            break;
          case X3D_HANIM_SEGMENT:
            valid = node->findValidFieldType(buttons[i].type) != -1;
            if (node->getType() == X3D_HANIM_JOINT) {
                NodeHAnimJoint *joint = (NodeHAnimJoint *)node;
                if (field != joint->displacers_Field())
                    valid = true;
            }
            break;
          case X3D_HANIM_SITE:
            valid = node->findValidFieldType(buttons[i].type) != -1;
            if (node->getType() == X3D_HANIM_JOINT) {
                NodeHAnimJoint *joint = (NodeHAnimJoint *)node;
                if (field != joint->displacers_Field())
                    valid = true;
            }
            if (node->getType() == X3D_HANIM_HUMANOID) {
                NodeHAnimHumanoid *human = (NodeHAnimHumanoid *)node;
                if (field == human->sites_Field())
                    valid = true;
                if (field == human->viewpoints_Field())
                    valid = true;
            }
            break;
          case X3D_CAD_FACE:
            valid = m_scene->isX3d();
            if (!valid)
                break;
            valid = node->findValidFieldType(PRODUCT_STRUCTURE_CHILD_NODE)
                    != -1;
            break;
          case X3D_COMPOSED_CUBE_MAP_TEXTURE:
          case X3D_GENERATED_CUBE_MAP_TEXTURE:
          case X3D_IMAGE_CUBE_MAP_TEXTURE:
            valid = m_scene->isX3d();
            if (!valid)
                break;
            valid = m_scene->isX3d() && getValidTexture();
            break;
          case X3D_COMPOSED_TEXTURE_3D:
          case X3D_IMAGE_TEXTURE_3D:
          case X3D_PIXEL_TEXTURE_3D:
            valid = m_scene->isX3d();
            if (!valid)
                break;
            valid = m_scene->isX3d() && getValidTexture3D();
            break;
          case X3D_COMPOSED_SHADER:
          case X3D_PACKAGED_SHADER:
          case X3D_PROGRAM_SHADER:
            valid = m_scene->isX3d();
            if (!valid)
                break;
            valid = node->findValidFieldType(SHADER_NODE) != -1;
            break;
          case X3D_PARTICLE_SYSTEM:
            valid = m_scene->isX3d();
            if (!valid)
                break;
            break;
          case X3D_CONE_EMITTER:
          case X3D_EXPLOSION_EMITTER:
          case X3D_POINT_EMITTER:
          case X3D_POLYLINE_EMITTER:
          case X3D_SURFACE_EMITTER:
          case X3D_VOLUME_EMITTER:
            valid = m_scene->isX3d();
            if (!valid)
                break;
            valid = node->findValidFieldType(PARTICLE_EMITTER_NODE) != -1;
            break;
          case X3D_FLOAT_VERTEX_ATTRIBUTE:
          case X3D_MATRIX_3_VERTEX_ATTRIBUTE:
          case X3D_MATRIX_4_VERTEX_ATTRIBUTE:
            valid = m_scene->isX3d();
            if (!valid)
                break;
            valid = node->findValidFieldType(VERTEX_ATTRIBUTE_NODE) != -1;
            break;
          case X3D_LAYER:
            valid = m_scene->isX3d();
            if (!valid)
                break;
            valid = node->findValidFieldType(LAYER_NODE) != -1;
            break;
          case X3D_LAYER_SET:
            valid = m_scene->isX3d();
            if (!valid)
                break;
            valid = (m_scene->getRoot() == node);
            break;
          case X3D_LAYOUT_LAYER:
            valid = m_scene->isX3d();
            if (!valid)
                break;
            valid = node->findValidFieldType(LAYER_NODE) != -1;
            break;
          case X3D_SCREEN_FONT_STYLE:
            valid = m_scene->isX3d();
            if (!valid)
                break;
          case VRML_FONT_STYLE:
            valid = node->findValidFieldType(FONT_STYLE_NODE) != -1;
            break;
          case X3D_VOLUME_DATA:
          case X3D_ISO_SURFACE_VOLUME_DATA:
          case X3D_SEGMENTED_VOLUME_DATA:
            valid = m_scene->isX3d();
            if (!valid)
                break;
            valid = node->findValidFieldType(CHILD_NODE) != -1;
            break;
          case X3D_BLENDED_VOLUME_STYLE:
          case X3D_BOUNDARY_ENHANCEMENT_VOLUME_STYLE:
          case X3D_CARTOON_VOLUME_STYLE:
          case X3D_COMPOSED_VOLUME_STYLE:
          case X3D_EDGE_ENHANCEMENT_VOLUME_STYLE:
          case X3D_OPACITY_MAP_VOLUME_STYLE:
          case X3D_PROJECTION_VOLUME_STYLE:
          case X3D_SHADED_VOLUME_STYLE:
          case X3D_SILHOUETTE_ENHANCEMENT_VOLUME_STYLE:
          case X3D_TONE_MAPPED_VOLUME_STYLE:
          case X3DOM_MPR_VOLUME_STYLE:
          case X3DOM_RADAR_VOLUME_STYLE:
          case X3DOM_STIPPLE_VOLUME_STYLE:
            valid = m_scene->isX3d();
            if (!valid)
                break;
            valid = node->findValidFieldType(VOLUME_RENDER_STYLE_NODE) != 
                    -1;
            break;


// template for new X3D nodes inserted with insertNode(XXX_NODE, "....
/*
          case X3D_:
            valid = m_scene->isX3d();
            if (!valid)
                break;
            valid = node->findValidFieldType(XXX_NODE) != -1;
            break;
*/
          case DUNE_VRML_CUT:
            valid = (m_vrmlCutNode == NULL);
            if (node->getType() == DUNE_VRML_SCENE)
                valid = true;
            break;
          case KAMBI_KAMBI_APPEARANCE:
            valid = node->getType() == VRML_SHAPE;
            break;
          case KAMBI_KAMBI_OCTREE_PROPERTIES:
            valid = (node->getType() == KAMBI_KAMBI_NAVIGATION_INFO) ||
                    (node->getType() == VRML_SHAPE);
            break;
          case KAMBI_RENDERED_TEXTURE:
            valid = (node->getType() == VRML_APPEARANCE) ||
                    (node->getType() == KAMBI_KAMBI_APPEARANCE);
            break;
          case KAMBI_PROJECTED_TEXTURE_COORDINATE:
            valid = (node->findValidFieldType(GENERATED_TEXTURE_COORDINATE_NODE | 
                                              TEXTURE_COORDINATE_NODE) 
                    != -1);
            break;
          case KAMBI_MULTI_GENERATED_TEXTURE_COORDINATE:
            if (node->getType() == 
                                 KAMBI_MULTI_GENERATED_TEXTURE_COORDINATE) {
                valid = false;
                break;
            }
            valid = (node->findValidFieldType(GENERATED_TEXTURE_COORDINATE_NODE)
                     != -1);
            break;
          case X3DOM_COLOR_MASK_MODE:
          case X3DOM_DEPTH_MODE:
          case X3DOM_COMMON_SURFACE_SHADER:
          case X3DOM_SURFACE_SHADER_TEXTURE:
            valid = node->getType() == VRML_APPEARANCE;
            break;
          case X3DOM_MULTI_PART:
            valid = node->getType() == VRML_INLINE;
            break;
          case X3DOM_POP_GEOMETRY_LEVEL:
            valid = node->getType() == X3DOM_POP_GEOMETRY;
            break;
          default:
            valid = node->findValidFieldType(buttons[i].type) != -1;
        }
        if (buttons[i].alwaysEnabled)
            valid = true;
        if (valid != buttons[i].enabled) {
            swMenuSetFlags(m_menu, buttons[i].id, SW_MENU_DISABLED, 
                           valid ? 0 : SW_MENU_DISABLED);
            swToolbarSetButtonFlags(toolbar, i, SW_TB_DISABLED,
                                    valid ? 0 : SW_TB_DISABLED);
            buttons[i].enabled = valid;                
        }
    }
}

static Node *searchIdxNode;
static Node *searchMaterialNode;
static Matrix searchMeshData;

static bool searchNodes(Node *node, void *data)
{
    if (node && node->isMeshBasedNode()) {
        searchIdxNode = node;
    } 
    if (node->getType() == VRML_MATERIAL) {
        searchMaterialNode = node;
    }
    if (node->getType() == VRML_TRANSFORM) {
        NodeTransform *transform = (NodeTransform *)node;
        Matrix transformMatrix;
        transform->transform();
        transform->getMatrix(transformMatrix);
        searchMeshData = transformMatrix;
    } 
    return true;     
}

void
MainWindow::UpdateToolbarSelection(void)
{
    const Path *selection = m_scene->getSelection();
    Node *node = selection->getNode();
    int field = selection->getField();

    UpdateObjectEdit(selection);

    if (node == NULL)
        return;

    UpdateToolbar(m_nodeToolbar1, node, field, buttons1, ARRAYSIZE(buttons1));
    UpdateToolbar(m_nodeToolbar2, node, field, buttons2, ARRAYSIZE(buttons2));
    UpdateToolbar(m_nodeToolbar3, node, field, buttons3, ARRAYSIZE(buttons3));
    UpdateToolbar(m_nodeToolbarVRML200x, node, field, buttonsVRML200x, 
                  ARRAYSIZE(buttonsVRML200x));
    UpdateToolbar(m_nodeToolbarX3DComponents1, node, field, 
                  buttonsX3DComponents1, ARRAYSIZE(buttonsX3DComponents1));
    UpdateToolbar(m_nodeToolbarX3DComponents2, node, field, 
                  buttonsX3DComponents2, ARRAYSIZE(buttonsX3DComponents2));
    UpdateToolbar(m_nodeToolbarX3DComponents3, node, field, 
                  buttonsX3DComponents3, ARRAYSIZE(buttonsX3DComponents3));
    UpdateToolbar(m_nodeToolbarX3DComponents4, node, field, 
                  buttonsX3DComponents4, ARRAYSIZE(buttonsX3DComponents4));
    UpdateToolbar(m_nodeToolbarScripted, node, field, buttonsScripted, 
                  ARRAYSIZE(buttonsScripted));
    UpdateToolbar(m_nodeToolbarCover, node, field, buttonsCover, 
                  ARRAYSIZE(buttonsCover));
    UpdateToolbar(m_nodeToolbarKambi, node, field, buttonsKambi, 
                  ARRAYSIZE(buttonsKambi));
    UpdateToolbar(m_nodeToolbarX3dom, node, field, buttonsX3dom, 
                  ARRAYSIZE(buttonsX3dom));
    UpdateMenuCoverNodes();
    UpdateMenuKambiNodes();

    swMenuSetFlags(m_menu, ID_DUNE_SELECTION_NEIGHBOUR_U, SW_MENU_DISABLED, 
                   (node->getType() == VRML_NURBS_SURFACE) ?
                   0 : SW_MENU_DISABLED);
    swMenuSetFlags(m_menu, ID_DUNE_SELECTION_NEIGHBOUR_V, SW_MENU_DISABLED, 
                   (node->getType() == VRML_NURBS_SURFACE) ?
                   0 : SW_MENU_DISABLED);
/*
    swMenuSetFlags(m_menu, ID_DUNE_SELECTION_STORE4CONVEX_HULL,
                   SW_MENU_DISABLED, m_scene->getSelectedHandlesSize() > 0 ?
                   0 : SW_MENU_DISABLED);
*/
    swMenuSetFlags(m_menu, ID_DUNE_DELETE_STORE4CONVEX_HULL,
                   SW_MENU_DISABLED, 
                   m_scene->getStore4Convex_hull()->size() > 0 ?
                   0 : SW_MENU_DISABLED);

    swMenuSetFlags(m_menu, ID_INTERACTION, SW_MENU_DISABLED,
                   node->supportInteraction() ? 0 : SW_MENU_DISABLED);
    swToolbarSetButtonFlags(m_standardToolbar, m_interactionIconPos, 
                            SW_TB_DISABLED, 
                            node->supportInteraction() ? 0 : SW_TB_DISABLED);

    swMenuSetFlags(m_menu, ID_ANIMATION, SW_MENU_DISABLED,
                   node->supportAnimation() ? 0 : SW_MENU_DISABLED);
    swToolbarSetButtonFlags(m_standardToolbar, m_animationIconPos, 
                            SW_TB_DISABLED, 
                            node->supportAnimation() ? 0 : SW_TB_DISABLED);

    swMenuSetFlags(m_menu, ID_CURVE_ANIMATION, SW_MENU_DISABLED,
                   node->supportCurveAnimation() ? 0 : SW_MENU_DISABLED);

    bool isRoot = (node == m_scene->getRoot());
    static bool oldIsRoot = !isRoot;
    bool parentIsRoot = isRoot;
    if (node->hasParent())
        parentIsRoot = isRoot ? true : (node->getParent() == m_scene->getRoot());

    swMenuSetFlags(m_menu, ID_HELP_SELECTION_PLACEHOLDER, SW_MENU_DISABLED, 
                   isRoot ? SW_MENU_DISABLED : 0);

    bool loop = false;
    if ((node->getType() == VRML_TIME_SENSOR) && (!node->isPROTO()))
        loop = ((NodeTimeSensor *)node)->loop()->getValue();
    swMenuSetFlags(m_menu, ID_SEARCH_INTERPOLATOR, SW_MENU_DISABLED, 
        node->hasInputs() ? 0 : SW_MENU_DISABLED);
    swMenuSetFlags(m_menu, ID_SEARCH_TIME_SENSOR, SW_MENU_DISABLED, 
        node->hasInputs() ? 0 : SW_MENU_DISABLED);
    swMenuSetFlags(m_menu, ID_CHANGE_ENABLE_ANIMATION, SW_MENU_DISABLED, 
        (node->getType() == VRML_TIME_SENSOR) && !loop ? 
        0 : SW_MENU_DISABLED);
    swMenuSetFlags(m_menu, ID_CHANGE_DISABLE_ANIMATION, SW_MENU_DISABLED, 
        (node->getType() == VRML_TIME_SENSOR) && loop ?
        0 : SW_MENU_DISABLED);
    bool hasAnimation = node->hasInputs();
    if (hasAnimation) {
        hasAnimation = false;
        for (int i = 0 ; i < node->getProto()->getNumEventIns(); i++) {
            SocketList::Iterator *j;
            for (j = node->getInput(i).first(); j != NULL; j = j->next()) {
                Node *inputNode = j->item().getNode();
                if (inputNode->isInterpolator()) {
                    hasAnimation = true;
                    break;
                }
            }
        }
    }
    swMenuSetFlags(m_menu, ID_DELETE_ANIMATION_DATA, SW_MENU_DISABLED, 
        hasAnimation ? 0 : SW_MENU_DISABLED);
    swMenuSetFlags(m_menu, ID_CHANGE_ANIMATION_TIME, SW_MENU_DISABLED, 
        (node->getType() == VRML_TIME_SENSOR) ? 0 : SW_MENU_DISABLED);
    swMenuSetFlags(m_menu, ID_CHANGE_ANIMATION_TIME, SW_MENU_DISABLED, 
        (node->getType() == VRML_TIME_SENSOR) ? 0 : SW_MENU_DISABLED);

    if (TheApp->is4Kids()) {
        swMenuSetFlags(m_menu, ID_CHANGE_MATERIAL_DIFFUSE, SW_MENU_DISABLED, 
            (node->getType() == VRML_MATERIAL) ? 0 : SW_MENU_DISABLED);
        swMenuSetFlags(m_menu, ID_CHANGE_MATERIAL_EMISSIVE, SW_MENU_DISABLED, 
            (node->getType() == VRML_MATERIAL) ? 0 : SW_MENU_DISABLED);
        swMenuSetFlags(m_menu, ID_CHANGE_MATERIAL_SPECULAR, SW_MENU_DISABLED, 
            (node->getType() == VRML_MATERIAL) ? 0 : SW_MENU_DISABLED);
        swMenuSetFlags(m_menu, ID_CHANGE_MATERIAL_TRANSPARENCY, SW_MENU_DISABLED,
            (node->getType() == VRML_MATERIAL) ? 0 : SW_MENU_DISABLED);
        swMenuSetFlags(m_menu, ID_CHANGE_MATERIAL_SHININESS, SW_MENU_DISABLED, 
            (node->getType() == VRML_MATERIAL) ? 0 : SW_MENU_DISABLED);
        swMenuSetFlags(m_menu, ID_NEW_IMAGE_TEXTURE, SW_MENU_DISABLED, 
            (node->getType() == VRML_APPEARANCE) ? 0 : SW_MENU_DISABLED);
        swMenuSetFlags(m_menu, ID_CHANGE_IMAGE_REPEAT, SW_MENU_DISABLED, 
            (node->getType() == VRML_APPEARANCE) || 
            (node->getType() == VRML_TEXTURE_TRANSFORM) ? 0 : SW_MENU_DISABLED);
        swMenuSetFlags(m_menu, ID_CHANGE_ONE_TEXT, SW_MENU_DISABLED, 
            (node->getType() == VRML_TEXT) ? 0 : SW_MENU_DISABLED);
    }

    swMenuSetFlags(m_menu, ID_MOVE_SIBLING_UP, SW_MENU_DISABLED,
                   (node->getPrevSiblingIndex() == -1) ? SW_MENU_DISABLED : 0);
    swMenuSetFlags(m_menu, ID_MOVE_SIBLING_DOWN, SW_MENU_DISABLED,
                   (node->getNextSiblingIndex() == -1) ? SW_MENU_DISABLED : 0);
    swMenuSetFlags(m_menu, ID_MOVE_SIBLING_FIRST, SW_MENU_DISABLED,
                   (node->getPrevSiblingIndex() == -1) ? SW_MENU_DISABLED : 0);
    swMenuSetFlags(m_menu, ID_MOVE_SIBLING_LAST, SW_MENU_DISABLED,
                   (node->getNextSiblingIndex() == -1) ? SW_MENU_DISABLED : 0);

    if (TheApp->is4Catt()) {
        swMenuSetFlags(m_menu, ID_TWO_SIDED, SW_MENU_DISABLED, 
            node->hasTwoSides() ? 0 : SW_MENU_DISABLED);
        if (node->hasTwoSides())
            swMenuSetFlags(m_menu, ID_TWO_SIDED, SW_MENU_CHECKED, 
                 node->isDoubleSided() ? SW_MENU_CHECKED : 0);
        else 
            swMenuSetFlags(m_menu, ID_TWO_SIDED, SW_MENU_CHECKED, 0); 
        swMenuSetFlags(m_menu, ID_CHANGE_SIDE, SW_MENU_DISABLED, 
            node->hasTwoSides() ? 0 : SW_MENU_DISABLED);
    }

    int disablePaste = 1;
    if (TheApp->getClipboardNode()) {
        if (field == -1) {
            field = node->findValidField(TheApp->getClipboardNode());
        }
        if (field != -1) {
            disablePaste = 0;
        }
    }
    if (m_scene->getViewOfLastSelection() && 
        m_scene->getViewOfLastSelection()->canPaste())
        disablePaste = 0;

    swToolbarSetButtonFlags(m_standardToolbar, m_editPasteIconPos, SW_TB_DISABLED,
                            disablePaste ? SW_TB_DISABLED : 0);
    swMenuSetFlags(m_menu, ID_DUNE_EDIT_PASTE, SW_MENU_DISABLED, 
                   disablePaste ? SW_MENU_DISABLED : 0);

    int disablePasteSymetric = 1;
    if (TheApp->getClipboardNode()) {
        int field = -1;
        if (node->hasParent())
            field = node->getParent()->findValidField(
                          TheApp->getClipboardNode());
        if (field != -1)
            disablePasteSymetric = 0;
    }
    if (m_scene->getViewOfLastSelection() && 
        m_scene->getViewOfLastSelection()->canPaste())
        disablePasteSymetric = 0;


    swMenuSetFlags(m_menu, ID_DUNE_EDIT_PASTE_SYMMETRIC_X, SW_MENU_DISABLED, 
                   disablePasteSymetric ? SW_MENU_DISABLED : 0);
    swMenuSetFlags(m_menu, ID_DUNE_EDIT_PASTE_SYMMETRIC_Y, SW_MENU_DISABLED, 
                   disablePasteSymetric ? SW_MENU_DISABLED : 0);
    swMenuSetFlags(m_menu, ID_DUNE_EDIT_PASTE_SYMMETRIC_Z, SW_MENU_DISABLED, 
                   disablePasteSymetric ? SW_MENU_DISABLED : 0);

    if (isRoot != oldIsRoot) {
#ifndef HAVE_CUT
        swMenuSetFlags(m_menu, ID_DUNE_EDIT_CUT, SW_MENU_DISABLED, 
                       isRoot ? SW_MENU_DISABLED : 0);
        swToolbarSetButtonFlags(m_standardToolbar, m_editCutIconPos, SW_TB_DISABLED,
                                isRoot ? SW_TB_DISABLED : 0);
#endif
        swMenuSetFlags(m_menu, ID_DUNE_EDIT_COPY, SW_MENU_DISABLED, 
                       isRoot ? SW_MENU_DISABLED : 0);

         swToolbarSetButtonFlags(m_standardToolbar, m_editCopyIconPos, SW_TB_DISABLED,
                                 isRoot ? SW_TB_DISABLED : 0);
         swMenuSetFlags(m_menu, ID_DUNE_EDIT_COPY_BRANCH_TO_ROOT, SW_MENU_DISABLED, 
                        isRoot ? SW_MENU_DISABLED : 0);
         swMenuSetFlags(m_menu, ID_DUNE_EDIT_DELETE, SW_MENU_DISABLED, 
                        isRoot ? SW_MENU_DISABLED : 0);
         swToolbarSetButtonFlags(m_standardToolbar, m_editDeleteIconPos, 
                                 SW_TB_DISABLED, isRoot ? SW_TB_DISABLED : 0);
         swMenuSetFlags(m_menu, ID_DUNE_EDIT_DEF, SW_MENU_DISABLED, 
                        isRoot ? SW_MENU_DISABLED : 0);
    }

    swMenuSetFlags(m_menu, ID_DUNE_EDIT_USE, SW_MENU_DISABLED, 
                   !m_scene->canUse(node, field) ? SW_MENU_DISABLED : 0);

    swMenuSetFlags(m_menu, ID_SET_DEFAULT, SW_MENU_DISABLED, 
                   node->maySetDefault() ? 0 : SW_MENU_DISABLED);

    swMenuSetFlags(m_menu, ID_MOVE_TO_X, SW_MENU_DISABLED, 
                   node->canMoveTo(0) ? 0 : SW_MENU_DISABLED);
    swMenuSetFlags(m_menu, ID_MOVE_TO_Y, SW_MENU_DISABLED, 
                   node->canMoveTo(1) ? 0 : SW_MENU_DISABLED);
    swMenuSetFlags(m_menu, ID_MOVE_TO_Z, SW_MENU_DISABLED, 
                   node->canMoveTo(2) ? 0 : SW_MENU_DISABLED);

    swMenuSetFlags(m_menu, ID_SCALE_BY_X, SW_MENU_DISABLED, 
                   node->canMoveTo(0) ? 0 : SW_MENU_DISABLED);
    swMenuSetFlags(m_menu, ID_SCALE_BY_Y, SW_MENU_DISABLED, 
                   node->canMoveTo(1) ? 0 : SW_MENU_DISABLED);
    swMenuSetFlags(m_menu, ID_SCALE_BY_Z, SW_MENU_DISABLED, 
                   node->canMoveTo(2) ? 0 : SW_MENU_DISABLED);

    swMenuSetFlags(m_menu, ID_SAME_MIN_X, SW_MENU_DISABLED, 
                   node->canMoveTo(0) ? 0 : SW_MENU_DISABLED);
    swMenuSetFlags(m_menu, ID_SAME_MIN_Y, SW_MENU_DISABLED, 
                   node->canMoveTo(1) ? 0 : SW_MENU_DISABLED);
    swMenuSetFlags(m_menu, ID_SAME_MIN_Z, SW_MENU_DISABLED, 
                   node->canMoveTo(2) ? 0 : SW_MENU_DISABLED);

    swMenuSetFlags(m_menu, ID_SAME_X, SW_MENU_DISABLED, 
                   node->canMoveTo(0) ? 0 : SW_MENU_DISABLED);
    swMenuSetFlags(m_menu, ID_SAME_Y, SW_MENU_DISABLED, 
                   node->canMoveTo(1) ? 0 : SW_MENU_DISABLED);
    swMenuSetFlags(m_menu, ID_SAME_Z, SW_MENU_DISABLED, 
                   node->canMoveTo(2) ? 0 : SW_MENU_DISABLED);

    swMenuSetFlags(m_menu, ID_SAME_MAX_X, SW_MENU_DISABLED, 
                   node->canMoveTo(0) ? 0 : SW_MENU_DISABLED);
    swMenuSetFlags(m_menu, ID_SAME_MAX_Y, SW_MENU_DISABLED, 
                   node->canMoveTo(1) ? 0 : SW_MENU_DISABLED);
    swMenuSetFlags(m_menu, ID_SAME_MAX_Z, SW_MENU_DISABLED, 
                   node->canMoveTo(2) ? 0 : SW_MENU_DISABLED);


    swMenuSetFlags(m_menu, ID_BRANCH_TO_PARENT, SW_MENU_DISABLED, 
                   parentIsRoot || 
                   node->isInvalidChildNode() ? SW_MENU_DISABLED : 0);

    swMenuSetFlags(m_menu, ID_BRANCH_TO_TRANSFORM_SELECTION, SW_MENU_DISABLED, 
                   node->getType() == VRML_TRANSFORM ? 0 : SW_MENU_DISABLED);

    swMenuSetFlags(m_menu, ID_BRANCH_TO_GROUP, SW_MENU_DISABLED, 
                   node->isInvalidChildNode() ? SW_MENU_DISABLED : 0);
    swMenuSetFlags(m_menu, ID_BRANCH_TO_TRANSFORM, SW_MENU_DISABLED, 
                   node->isInvalidChildNode() ? SW_MENU_DISABLED : 0);
    swMenuSetFlags(m_menu, ID_BRANCH_TO_COLLISION, SW_MENU_DISABLED, 
                   node->isInvalidChildNode() ? SW_MENU_DISABLED : 0);
    swMenuSetFlags(m_menu, ID_BRANCH_TO_COLLISIONPROXY, SW_MENU_DISABLED, 
                   node->isInvalidChildNode() ? SW_MENU_DISABLED : 0);
    swMenuSetFlags(m_menu, ID_BRANCH_TO_ANCHOR, SW_MENU_DISABLED, 
                   node->isInvalidChildNode() ? SW_MENU_DISABLED : 0);
    swMenuSetFlags(m_menu, ID_BRANCH_TO_BILLBOARD, SW_MENU_DISABLED, 
                   node->isInvalidChildNode() ? SW_MENU_DISABLED : 0);
    swMenuSetFlags(m_menu, ID_BRANCH_TO_LOD, SW_MENU_DISABLED, 
                   node->isInvalidChildNode() ? SW_MENU_DISABLED : 0);
    swMenuSetFlags(m_menu, ID_BRANCH_TO_SWITCH, SW_MENU_DISABLED, 
                   node->isInvalidChildNode() ? SW_MENU_DISABLED : 0);
    swMenuSetFlags(m_menu, ID_BRANCH_TO_INLINE, SW_MENU_DISABLED, 
                   node->isInvalidChildNode() ? SW_MENU_DISABLED : 0);

    swMenuSetFlags(m_menu, ID_ARRAY, SW_MENU_DISABLED, 
                   (node->hasParent()) && (!node->isInvalidChildNode()) &&
                   (node->getParent()->getType() == VRML_GROUP ||
                    node->getParent()->getType() == VRML_TRANSFORM) ? 
                   0 : SW_MENU_DISABLED);

    bool canCenter = false;
    if (node->hasBoundingBox())
        if ((node->hasParent()) && (!parentIsRoot)) {
            Node *checkNode = node->getParent();
            if (checkNode->getType() == VRML_SHAPE) {
                if (checkNode->hasParent()) {
                    checkNode = checkNode->getParent();
                    if (checkNode->getType() == VRML_TRANSFORM)
                        canCenter = true;
                }
            }
        }

    swMenuSetFlags(m_menu, ID_CENTER_TO_MID, SW_MENU_DISABLED, 
                   canCenter ? 0 : SW_MENU_DISABLED);
    swMenuSetFlags(m_menu, ID_CENTER_TO_MAXX, SW_MENU_DISABLED, 
                   canCenter ? 0 : SW_MENU_DISABLED);
    swMenuSetFlags(m_menu, ID_CENTER_TO_MAXY, SW_MENU_DISABLED, 
                   canCenter ? 0 : SW_MENU_DISABLED);
    swMenuSetFlags(m_menu, ID_CENTER_TO_MAXZ, SW_MENU_DISABLED, 
                   canCenter ? 0 : SW_MENU_DISABLED);
    swMenuSetFlags(m_menu, ID_CENTER_TO_MINX, SW_MENU_DISABLED, 
                   canCenter ? 0 : SW_MENU_DISABLED);
    swMenuSetFlags(m_menu, ID_CENTER_TO_MINY, SW_MENU_DISABLED, 
                   canCenter ? 0 : SW_MENU_DISABLED);
    swMenuSetFlags(m_menu, ID_CENTER_TO_MINZ, SW_MENU_DISABLED, 
                   canCenter ? 0 : SW_MENU_DISABLED);

    swMenuSetFlags(m_menu, ID_HANIM_JOINT_WEIGHT_SET, SW_MENU_DISABLED,
                   (node->getType() == VRML_COORDINATE) ? 
                   0 : SW_MENU_DISABLED);
    swMenuSetFlags(m_menu, ID_HANIM_JOINT_WEIGHT_REMOVE, SW_MENU_DISABLED,
                   (node->getType() == VRML_COORDINATE) ? 
                   0 : SW_MENU_DISABLED);

    swMenuSetFlags(m_menu, ID_SET_OPTIMIZE, SW_MENU_DISABLED,
                   (node->getType() == VRML_COORDINATE) ||
                   (node->getType() == VRML_INDEXED_FACE_SET) ? 
                   0 : SW_MENU_DISABLED);
    swMenuSetFlags(m_menu, ID_TRIANGULATE, SW_MENU_DISABLED,
                   (node->getType() == VRML_COORDINATE) ||
                   (node->getType() == VRML_INDEXED_FACE_SET) ? 
                   0 : SW_MENU_DISABLED);
    swMenuSetFlags(m_menu, ID_SUBDIVIDE, SW_MENU_DISABLED,
                   (node->getType() == VRML_COORDINATE) ||
                   (node->isMeshBasedNode() && (node->getType() != 
                    DUNE_SUPER_EXTRUSION))? 
                   0 : SW_MENU_DISABLED);
    swMenuSetFlags(m_menu, ID_BUILD_QUAD, SW_MENU_DISABLED,
                   (node->getType() == VRML_COORDINATE) &&
                   (m_scene->getSelectionMode() == SELECTION_MODE_FACES) &&
                   (m_scene->getSelectedHandlesSize() == 2) ?
                   0 : SW_MENU_DISABLED);
    swMenuSetFlags(m_menu, ID_SNAP_TOGETHER, SW_MENU_DISABLED,
                   (node->getType() == VRML_COORDINATE) &&
                   (m_scene->getSelectionMode() == SELECTION_MODE_VERTICES) &&
                   (m_scene->getSelectedHandlesSize() > 0) ?
                   0 : SW_MENU_DISABLED);
    bool isGroup2children = false;
    if ((node->getType() == VRML_GROUP) && (node != m_scene->getRoot())) {
        NodeGroup *group = (NodeGroup *)node;
        if (group->children()->getSize() == 2)
            isGroup2children = true;
    }
    swMenuSetFlags(m_menu, ID_CSG_SUBTRACT, SW_MENU_DISABLED,
                   isGroup2children ? 0 : SW_MENU_DISABLED);
    swMenuSetFlags(m_menu, ID_CSG_INTERSECTION, SW_MENU_DISABLED,
                   isGroup2children ? 0 : SW_MENU_DISABLED);
    swMenuSetFlags(m_menu, ID_CSG_UNION, SW_MENU_DISABLED,
                   isGroup2children ? 0 : SW_MENU_DISABLED);
    bool isGroupWithMeshes = false;
    if ((node->getType() == VRML_GROUP) && (node != m_scene->getRoot())) {
        isGroupWithMeshes = true;
        NodeGroup *group = (NodeGroup *)node;
        for (int i = 0; i < group->children()->getSize(); i++) {
            searchIdxNode = NULL;
            group->children()->getValue(i)->doWithBranch(searchNodes, NULL, false);
            Node *node = searchIdxNode;  
            if (node && !(node->isMeshBasedNode()))
                isGroupWithMeshes = false;
        }
    }
    swMenuSetFlags(m_menu, ID_SIMPLE_JOIN, SW_MENU_DISABLED,
                   isGroupWithMeshes ? 0 : SW_MENU_DISABLED);
    swMenuSetFlags(m_menu, ID_SPLIT_FACES, SW_MENU_DISABLED,
                   (node->getType() == VRML_COORDINATE) ?
                   0 : SW_MENU_DISABLED);
    swMenuSetFlags(m_menu, ID_INSET_FACE, SW_MENU_DISABLED,
                   (node->getType() == VRML_COORDINATE) ?
                   0 : SW_MENU_DISABLED);
    swMenuSetFlags(m_menu, ID_EXTRUDE, SW_MENU_DISABLED,
                   (node->getType() == VRML_COORDINATE) ?
                   0 : SW_MENU_DISABLED);
    swMenuSetFlags(m_menu, ID_SPLIT_INTO_PIECES, SW_MENU_DISABLED,
                   (node->getType() == VRML_COORDINATE) ?
                   0 : SW_MENU_DISABLED);
    swMenuSetFlags(m_menu, ID_MESH_REDUCE, SW_MENU_DISABLED,
                   (node->getType() == VRML_COORDINATE) ||
                   (node->getType() == VRML_INDEXED_FACE_SET) ? 
                   0 : SW_MENU_DISABLED);
    bool canDegreeElevate = false;
    switch (node->getType()) {
      case VRML_NURBS_CURVE:
      case VRML_NURBS_CURVE_2D:
      case DUNE_SUPER_EXTRUSION:
      case DUNE_SUPER_REVOLVER:
      case DUNE_CURVE_ANIMATION:
        canDegreeElevate = true;
    }
    swMenuSetFlags(m_menu, ID_REBUILD_NURBS_CONTROL_POINTS, SW_MENU_DISABLED,
                   node->getType() == VRML_NURBS_SURFACE ? 0 : SW_MENU_DISABLED
                  );
    swMenuSetFlags(m_menu, ID_MAKE_NURBS_SURFACE_SYMETRIC_X, SW_MENU_DISABLED,
                   node->getType() == VRML_NURBS_SURFACE ? 0 : SW_MENU_DISABLED
                  );
    swMenuSetFlags(m_menu, ID_MAKE_NURBS_SURFACE_SYMETRIC_Y, SW_MENU_DISABLED,
                   node->getType() == VRML_NURBS_SURFACE ? 0 : SW_MENU_DISABLED
                  );
    swMenuSetFlags(m_menu, ID_MAKE_NURBS_SURFACE_SYMETRIC_Z, SW_MENU_DISABLED,
                   node->getType() == VRML_NURBS_SURFACE ? 0 : SW_MENU_DISABLED
                  );
    swMenuSetFlags(m_menu, ID_INSERT_TO_NURBS_CURVE, SW_MENU_DISABLED,
                   node->getType() == VRML_NURBS_CURVE ? 0 : SW_MENU_DISABLED);
    swMenuSetFlags(m_menu, ID_INSERT_TO_NURBS_SURFACE, SW_MENU_DISABLED,
                   node->getType() == VRML_NURBS_SURFACE ? 0 : SW_MENU_DISABLED
                  );
    swMenuSetFlags(m_menu, ID_DEGREE_ELEVATE_UP, SW_MENU_DISABLED,
                   canDegreeElevate ? 0 : SW_MENU_DISABLED);
    swMenuSetFlags(m_menu, ID_DEGREE_ELEVATE_DOWN, SW_MENU_DISABLED,
                   canDegreeElevate ? 0 : SW_MENU_DISABLED);

    swMenuSetFlags(m_menu, ID_U_DEGREE_ELEVATE_UP, SW_MENU_DISABLED,
                   node->getType() == VRML_NURBS_SURFACE ? 0 : SW_MENU_DISABLED);
    swMenuSetFlags(m_menu, ID_U_DEGREE_ELEVATE_DOWN, SW_MENU_DISABLED,
                   node->getType() == VRML_NURBS_SURFACE ? 0 : SW_MENU_DISABLED);
    swMenuSetFlags(m_menu, ID_V_DEGREE_ELEVATE_UP, SW_MENU_DISABLED,
                   node->getType() == VRML_NURBS_SURFACE ? 0 : SW_MENU_DISABLED);
    swMenuSetFlags(m_menu, ID_V_DEGREE_ELEVATE_DOWN, SW_MENU_DISABLED,
                   node->getType() == VRML_NURBS_SURFACE ? 0 : SW_MENU_DISABLED);
    swMenuSetFlags(m_menu, ID_LINEAR_UKNOT, SW_MENU_DISABLED,
                   node->getType() == VRML_NURBS_SURFACE ? 0 : SW_MENU_DISABLED);
    swMenuSetFlags(m_menu, ID_LINEAR_VKNOT, SW_MENU_DISABLED,
                   node->getType() == VRML_NURBS_SURFACE ? 0 : SW_MENU_DISABLED);
    swMenuSetFlags(m_menu, ID_SET_WEIGHTS_TO_1, SW_MENU_DISABLED,
                   (node->getType() == VRML_NURBS_SURFACE) ||
                   (node->getType() == X3D_NURBS_TRIMMED_SURFACE) ||
                   (node->getType() == VRML_NURBS_CURVE_2D)
                   ? 0 : SW_MENU_DISABLED);

    bool canPipe = false;
    Node *fieldViewNode = NULL;
    int fieldViewField = -1;
    if (m_fieldView->isFieldView()) {
        fieldViewNode = ((FieldView *)m_fieldView)->GetSelectedNode();
        fieldViewField = ((FieldView *)m_fieldView)->GetSelectedField();
    }
    if (fieldViewNode && (fieldViewField > -1))
        canPipe = true;
    if (m_scene->getSelectedHandlesSize() > 1)
        canPipe = true;
    swMenuSetFlags(m_menu, ID_PIPE, SW_MENU_DISABLED, 
                   canPipe ? 0 : SW_MENU_DISABLED);

    swMenuSetFlags(m_menu, ID_MAKE_SYMETRIC_X, SW_MENU_DISABLED, 
                   (node->getType() == VRML_COORDINATE) ||
                   (node->getType() == VRML_INDEXED_FACE_SET) ? 
                   0 : SW_MENU_DISABLED);
    swMenuSetFlags(m_menu, ID_MAKE_SYMETRIC_Y, SW_MENU_DISABLED, 
                   (node->getType() == VRML_COORDINATE) ||
                   (node->getType() == VRML_INDEXED_FACE_SET) ? 
                   0 : SW_MENU_DISABLED);
    swMenuSetFlags(m_menu, ID_MAKE_SYMETRIC_Z, SW_MENU_DISABLED, 
                   (node->getType() == VRML_COORDINATE) ||
                   (node->getType() == VRML_INDEXED_FACE_SET) ? 
                   0 : SW_MENU_DISABLED);
    
    bool toNurbs = false;
    if (node->hasParent()) {
        Node *parent = node->getParent();
        if (isToNurbsAllowedParent(parent)) {
            if (node->getType() == VRML_CYLINDER)
                toNurbs = true;
            if (node->getType() == VRML_CONE)
                toNurbs = true;
            if (node->getType() == VRML_SPHERE)
                toNurbs = true;
            if (node->getType() == VRML_BOX)
                toNurbs = true;
            if (node->getType() == VRML_NURBS_CURVE)
                toNurbs = true;
            if (node->getType() == DUNE_SUPER_ELLIPSOID)
                toNurbs = true;
            if (node->getType() == DUNE_SUPER_EXTRUSION) {
                NodeSuperExtrusion *superEx = (NodeSuperExtrusion *)node;
                if ((superEx->beginCap()->getValue() == FALSE) &&
                    (superEx->endCap()->getValue() == FALSE))
                    toNurbs = true;
            }
            if (node->getType() == DUNE_SUPER_SHAPE)
                toNurbs = true;
            if (node->getType() == DUNE_SUPER_REVOLVER)
                toNurbs = true;
        }
    }
    swMenuSetFlags(m_menu, ID_TO_NURBS, SW_MENU_DISABLED, 
                   toNurbs ? 0 : SW_MENU_DISABLED);    
    if (TheApp->is4Kids()) {
        swMenuSetFlags(m_menu, ID_TO_NURBS_4KIDS, SW_MENU_DISABLED, 
                       toNurbs ? 0 : SW_MENU_DISABLED);    
        swToolbarSetButtonFlags(m_standardToolbar, m_toNurbs4KidsIconPos, 
                                SW_TB_DISABLED, 
                                toNurbs ? 0 : SW_TB_DISABLED);
    }

    swMenuSetFlags(m_menu, ID_TO_NURBS_TRIMMED, SW_MENU_DISABLED, 
                   (node->getType() == VRML_NURBS_SURFACE) ? 
                   0 : SW_MENU_DISABLED);    
    swMenuSetFlags(m_menu, ID_TO_NURBS_CURVE, SW_MENU_DISABLED, 
                   (node->getType() == DUNE_SUPER_EXTRUSION) || 
                   (node->getType() == DUNE_SUPER_REVOLVER) ||
                   (node->getType() == VRML_NURBS_POSITION_INTERPOLATOR) ||
                   (node->getType() == X3D_NURBS_ORIENTATION_INTERPOLATOR) ||
                   (node->getType() == DUNE_CURVE_ANIMATION) ?
                   0 : SW_MENU_DISABLED);    

    swMenuSetFlags(m_menu, ID_TO_SUPER_EXTRUSION, SW_MENU_DISABLED, 
                   node->getType() == VRML_NURBS_CURVE ? 
                   0 : SW_MENU_DISABLED);    

    swMenuSetFlags(m_menu, ID_TO_SUPER_REVOLVER, SW_MENU_DISABLED, 
                   node->getType() == VRML_NURBS_CURVE ? 
                   0 : SW_MENU_DISABLED);    

    swMenuSetFlags(m_menu, ID_TO_EXTRUSION, SW_MENU_DISABLED, 
                   node->canConvertToExtrusion() ? 0 : SW_MENU_DISABLED);

    swMenuSetFlags(m_menu, ID_TO_INDEXED_FACESET, SW_MENU_DISABLED, 
                   node->canConvertToIndexedFaceSet() ? 0 : SW_MENU_DISABLED);

    swMenuSetFlags(m_menu, ID_TO_INDEXED_TRIANGLESET, SW_MENU_DISABLED, 
                   node->canConvertToIndexedTriangleSet() ? 0 : 
                   SW_MENU_DISABLED);

    swMenuSetFlags(m_menu, ID_TO_TRIANGLESET, SW_MENU_DISABLED, 
                   node->canConvertToTriangleSet() ? 0 : SW_MENU_DISABLED);

    swMenuSetFlags(m_menu, ID_TO_INDEXED_LINESET, SW_MENU_DISABLED, 
                   node->canConvertToIndexedLineSet() ||
                   (node->getType() == VRML_INDEXED_FACE_SET) ? 
                   0 : SW_MENU_DISABLED);

    swMenuSetFlags(m_menu, ID_TO_POINTSET, SW_MENU_DISABLED, 
                   (node->getType() == VRML_INDEXED_LINE_SET) ? 
                   0 : SW_MENU_DISABLED);

    swMenuSetFlags(m_menu, ID_TO_CURVE_ANIMATION, SW_MENU_DISABLED, 
                   (node->getType() == VRML_NURBS_CURVE) ? 
                   0 : SW_MENU_DISABLED);

    swMenuSetFlags(m_menu, ID_TO_NURBS_POSITION_INTERPOLATOR, SW_MENU_DISABLED, 
                   (node->getType() == VRML_NURBS_CURVE) ? 
                   0 : SW_MENU_DISABLED);

    swMenuSetFlags(m_menu, ID_TO_NURBS_ORIENTATION_INTERPOLATOR, 
                   SW_MENU_DISABLED, (node->getType() == VRML_NURBS_CURVE) ? 
                   0 : SW_MENU_DISABLED);

    swMenuSetFlags(m_menu, ID_TO_POSITION_AND_ROTATION_INTERPOLATORS, 
                   SW_MENU_DISABLED, 
                   node->canConvertToPositionAndOrientationInterpolators() ? 
                   0 : SW_MENU_DISABLED);

    swMenuSetFlags(m_menu, ID_TO_POSITION_INTERPOLATOR, SW_MENU_DISABLED, 
                   node->canConvertToPositionInterpolator() ? 
                   0 : SW_MENU_DISABLED);

    swMenuSetFlags(m_menu, ID_TO_ORIENTATION_INTERPOLATOR_XZ, SW_MENU_DISABLED, 
                   node->canConvertToOrientationInterpolator() ? 
                   0 : SW_MENU_DISABLED);
    swMenuSetFlags(m_menu, ID_TO_ORIENTATION_INTERPOLATOR_XY, SW_MENU_DISABLED, 
                   node->canConvertToOrientationInterpolator() ? 
                   0 : SW_MENU_DISABLED);
    swMenuSetFlags(m_menu, ID_TO_ORIENTATION_INTERPOLATOR_YZ, SW_MENU_DISABLED, 
                   node->canConvertToOrientationInterpolator() ? 
                   0 : SW_MENU_DISABLED);

    swMenuSetFlags(m_menu, ID_TO_INDEXED_FACESET_4KIDS, SW_MENU_DISABLED, 
                   node->canConvertToIndexedFaceSet() ? 0 : SW_MENU_DISABLED);

    setColorCircleIcon();

    oldIsRoot = isRoot;
}

void
MainWindow::RefreshRecentFileMenu(void)
{
    int i, n = TheApp->GetNumRecentFiles();

    // insert a placeholder (if file 1 is there)
    swInsertMenuItem(m_menu, ID_DUNE_FILE_MRU_FILE1, "",
                     ID_DUNE_FILE_MRU_PLACEHOLDER);
    swMenuSetFlags(m_menu, ID_DUNE_FILE_MRU_PLACEHOLDER, SW_MENU_DISABLED,
                   SW_MENU_DISABLED);
    
    // delete all recent-file menus
    for (i = 0; i < 16; i++) {
        swDeleteMenuItem(m_menu, ID_DUNE_FILE_MRU_FILE1 + i);
    }
    
    // insert recent-file menus
    if (n > 0) {
        for (i = 0; i < n; i++) {
            char buf[1024];
            int id = ID_DUNE_FILE_MRU_FILE1 + i;
            mysnprintf(buf, 1023, "&%d %s", i + 1,
                       (const char *) TheApp->GetRecentFile(i));
            swInsertMenuItem(m_menu, ID_DUNE_FILE_MRU_PLACEHOLDER, buf, id);
        }

        // remove the placeholder
        swDeleteMenuItem(m_menu, ID_DUNE_FILE_MRU_PLACEHOLDER);
    }
}

void
MainWindow::RefreshProtoMenu()
{
    // avoid extra menuitems for reduced menus 
    // under M$Windows this would add unwanted items to the menubar 
    if (TheApp->is4Kids() ||  TheApp->is4Catt())
        return;
 
    bool hasPlaceHolder = false;
    int n = m_scene->getNumProtos();

    // avoid overflow of MenuItem ID's
    if (n > 0xFFF)
       n = 0xFFF;

    // insert recent-file menus
    if (n > 0) {
        int menuSize = m_protoMenu.size();
        for (int i = 0; i < menuSize; i++)
            m_protoMenu[i].disabled = true;
        for (int i = 0; i < m_scene->getNumProtos(); i++) {
            if (m_scene->getProto(i) == NULL)
                continue;
            const char *protoName = m_scene->getProto(i)->getName(true);
            bool found = false;
            for (size_t j = 0; j < m_protoMenu.size(); j++) {
                if (strcmp(protoName, m_protoMenu[j].protoName) == 0) {
                    found = true;
                    m_protoMenu[j].disabled = false;
                    break;
                }
            }
            if (found)
                continue;

            if (!hasPlaceHolder) {
                // insert a placeholder
                swInsertMenuItem(m_menu, ID_PROTO_MRU_1, " ",
                                 ID_PROTO_MRU_PLACEHOLDER);
                swMenuSetFlags(m_menu, ID_PROTO_MRU_PLACEHOLDER, 
                               SW_MENU_DISABLED, SW_MENU_DISABLED);
                hasPlaceHolder = true;
            }
     
            char buf[1024];
            int id = ID_PROTO_MRU_1 + menuSize;
            mysnprintf(buf, 1023, "&%d %s", menuSize + 1, protoName);
            m_protoMenu[menuSize].protoName = "";
            m_protoMenu[menuSize].protoName += protoName;
            m_protoMenu[menuSize].id = id;
            m_protoMenu[menuSize].disabled = false;
            menuSize++;
            swInsertMenuItem(m_menu, ID_PROTO_MRU_PLACEHOLDER, buf, id);
        }        
        for (size_t i = 0; i < m_protoMenu.size(); i++)
            swMenuSetFlags(m_menu, m_protoMenu[i].id, SW_MENU_DISABLED,
                           m_protoMenu[i].disabled ? SW_MENU_DISABLED : 0);
    }

    // remove the placeholder
    if (hasPlaceHolder)
        swDeleteMenuItem(m_menu, ID_PROTO_MRU_PLACEHOLDER);
}

void MainWindow::RefreshHelpSelection()
{
    // avoid extra menuitems for reduced menus 
    // under M$Windows this would add unwanted items to the menubar 
    if (TheApp->is4Kids() ||  TheApp->is4Catt())
        return;

    // insert a placeholder
    Node *node = m_scene->getSelection()->getNode();
    swInsertMenuItem(m_menu, ID_HELP_SELECTION, " ",
                             ID_HELP_SELECTION_PLACEHOLDER);

    swMenuSetFlags(m_menu, ID_HELP_SELECTION_PLACEHOLDER, SW_MENU_DISABLED,
                   SW_MENU_DISABLED);
    
    swDeleteMenuItem(m_menu, ID_HELP_SELECTION);
    swInsertMenuItem(m_menu, ID_HELP_SELECTION_PLACEHOLDER,
                     (const char*) node->getProto()->getName(m_scene->isX3d()), 
                     ID_HELP_SELECTION);

    // remove the placeholder
    swDeleteMenuItem(m_menu, ID_HELP_SELECTION_PLACEHOLDER);
}

Node * 
MainWindow::createNode(Node *node)
{
    const Path *selection = m_scene->getSelection();
    if (selection && selection->isProto(m_scene)) {
        Proto *proto = selection->getProto(m_scene);
        if (node == NULL)
            return NULL;
        proto->addNode(node);
        m_scene->updateNodePROTOs(proto);
        m_scene->UpdateViews(NULL, UPDATE_ALL);
        return node;
    }
    Node *selectionNode = selection->getNode();
    int selectionField = selection->getField();

    Node *targetNode = m_scene->getRoot();
    int targetField = m_scene->getRootField();

    int destField = m_scene->getDestField(node, selectionNode, selectionField);
    if ((node->getType() == VRML_SHAPE) && 
        (selectionNode->getType() == X3D_COLLIDABLE_SHAPE)) {
        NodeCollidableShape *coll = (NodeCollidableShape *)selectionNode;
        if (coll->shape()->getValue() == NULL) {
            targetNode = selectionNode;
            targetField = coll->shape_Field();
        }
    } else if ((destField > -1) && (selectionNode != targetNode) && 
               (node->getType() != VRML_VIEWPOINT)) {
        targetNode = selectionNode;
        targetField = destField;
    }

    if (targetNode->getOutsideProto() &&
        targetNode->getOutsideProto()->lookupIsExposedField(targetNode, 
                                                            targetField) != -1)
        return NULL;
    
    m_scene->execute(new MoveCommand(node, NULL, -1, targetNode, targetField));
    m_scene->setSelection(node);
    m_scene->UpdateViews(NULL, UPDATE_SELECTION);
    return node;
}

Node *
MainWindow::createNode(const char *type)
{
    Node *node = m_scene->createNode(type);
    if (node == NULL)
        return NULL;
    return createNode(node);
}

Node *
MainWindow::createGeometryNode(Node *node, bool emissiveDefaultColor, 
                               bool alignToCamera)
{
    const Path *sel = m_scene->getSelection();
    Node *selection = sel->getNode();
    if (selection->getType() == VRML_SHAPE) {
        NodeShape *shape = (NodeShape *)selection;
        if (shape->geometry()->getValue() == NULL) {
            MoveCommand *command = new MoveCommand(node, NULL, -1, selection, 
                                                   shape->geometry_Field());
            command->execute();                       
            m_scene->setSelection(node);
            m_scene->UpdateViews(NULL, UPDATE_SELECTION);
            return node;
        }
    }
    if (selection->getType() == X3D_PARTICLE_SYSTEM) {
        NodeParticleSystem *shape = (NodeParticleSystem *)selection;
        if (shape->geometry()->getValue() == NULL) {
            MoveCommand *command = new MoveCommand(node, NULL, -1, selection, 
                                                   shape->geometry_Field());
            command->execute();                       
            m_scene->setSelection(node);
            m_scene->UpdateViews(NULL, UPDATE_SELECTION);
            return node;
        }
    }
    if (node->getType() == VRML_NURBS_SURFACE) {
        if (selection->getType() == X3D_NURBS_SET) {
            NodeNurbsSet *nurbsSet = (NodeNurbsSet *)selection;
            MoveCommand *command = new MoveCommand(node, NULL, -1,
                                                   nurbsSet,
                                                   nurbsSet->geometry_Field());
            command->execute();                       
            m_scene->setSelection(node);
            m_scene->UpdateViews(NULL, UPDATE_SELECTION);
            return node;
        }            
    }
    NodeShape *nShape = (NodeShape *) m_scene->createNode("Shape");
    nShape->createNewAppearance(emissiveDefaultColor);
    nShape->geometry(new SFNode(node));
    if (selection->getType() == X3D_COLLIDABLE_SHAPE) {
        NodeCollidableShape *coll = (NodeCollidableShape *)selection;
        if (coll->shape()->getValue() == NULL) {
            MoveCommand *command = new MoveCommand(nShape, NULL, -1,
                                                   coll,
                                                   coll->shape_Field());
            command->execute();                       
            m_scene->setSelection(node);
            m_scene->UpdateViews(NULL, UPDATE_SELECTION);
            return node;            
        }
    }            
    if (selection->getType() == X3D_CAD_FACE) {
        NodeCADFace *face = (NodeCADFace *)selection;
        if (face->shape()->getValue() == NULL) {
            MoveCommand *command = new MoveCommand(nShape, NULL, -1,
                                                   face,
                                                   face->shape_Field());
            command->execute();                       
            m_scene->setSelection(node);
            m_scene->UpdateViews(NULL, UPDATE_SELECTION);
            return node;            
        }
    }            
    NodeTransform *nTransform = (NodeTransform *)
                                m_scene->createNode("Transform");
    Node *camera = m_scene->getCamera();
    nTransform->children(new MFNode(new NodeList(nShape)));
    Node *transformNode = createNode(nTransform);
    if (transformNode == NULL)
        return NULL;
    bool parentIsRoot = false;
    if (transformNode->hasParent())
        if (transformNode->getParent() == m_scene->getRoot())
            parentIsRoot = true;
    if (parentIsRoot) 
        if (camera && alignToCamera) {
            SFRotation *sfRotation = new SFRotation(camera->getOrientation());
            Quaternion quad = sfRotation->getQuat();
            Vec3d vec = camera->getPosition();
            Vec3d ten(0, 0, -10);
            vec += ten * quad;
            nTransform->rotation(new SFRotation(*sfRotation));
            if (TheApp->getCreateAtZero(0))
                vec.x = 0;
            if (TheApp->getCreateAtZero(1))
                vec.y = 0;
            if (TheApp->getCreateAtZero(2))
                vec.z = 0;
            nTransform->translation(new SFVec3f(Vec3f(vec.x, vec.y, vec.z)));
        }
    if (TheApp->is4Kids()) {
        // make Material node visible
        NodeAppearance *nAppearance = (NodeAppearance *)
                                      nShape->appearance()->getValue();
        Node *nMaterial = nAppearance->material()->getValue();
        if (nMaterial) {
            m_scene->setSelection(nAppearance->material()->getValue());
            m_scene->UpdateViews(NULL, UPDATE_SELECTION);
        }
    }
    if (!(node->isDeepInsideProto())) {
        m_scene->setSelection(node);
        m_scene->UpdateViews(NULL, UPDATE_SELECTION);
    }
    return node;
}

Node * 
MainWindow::createGeometryNode(const char *type, bool emissiveDefaultColor, 
                               bool alignToCamera)
{
    Node *node = m_scene->createNode(type);
    createGeometryNode(node, emissiveDefaultColor, alignToCamera);
    return node;
}

void MainWindow::insertNodeByName() 
{
    OneTextDialog dlg(m_wnd, IDD_NODE_BY_NAME, "", NULL);
    if (dlg.DoModal() == IDCANCEL)
        return;
    const char* nodeName = dlg.GetValue();
    if (createNode(nodeName) == NULL)
        TheApp->MessageBox(IDS_UNKNOWN_NODE, nodeName);

}

Node *
MainWindow::getInsertNode(Node *node)
{
    Node *parentNode = m_scene->getRoot();
    if (node->hasParent())
        parentNode = node->getParent();
    while ((parentNode->getType() != VRML_ANCHOR) &&
           (parentNode->getType() != VRML_BILLBOARD) &&
           (parentNode->getType() != VRML_COLLISION) &&
           (parentNode->getType() != VRML_GROUP) &&
           (parentNode->getType() != VRML_TRANSFORM) &&
           (parentNode->getType() != DUNE_VRML_SCENE)) {
        if (parentNode->hasParent()) {
            parentNode = parentNode->getParent();
            if (parentNode == m_scene->getRoot())
                break;
        } else {
            parentNode = m_scene->getRoot();
            break;
        }
    }
    return parentNode;
}
    
void
MainWindow::createAnimation(void)
{
    Node *node = m_scene->getSelection()->getNode();
    if (node == m_scene->getRoot())
        return;
    if (!node->supportAnimation())
        return;
    if (m_scene->isX3d() && (node->getType() == VRML_NURBS_SURFACE)) {
        NodeNurbsSurface *nurbs = (NodeNurbsSurface *)node;
        if (nurbs->controlPointX3D()->getValue())
            node = nurbs->controlPointX3D()->getValue();
        else
            return;
    }
    if (m_scene->isX3d() && (node->getType() == X3D_NURBS_TRIMMED_SURFACE)) {
        NodeNurbsTrimmedSurface *nurbs = (NodeNurbsTrimmedSurface *)node;
        if (nurbs->controlPointX3D()->getValue())
            node = nurbs->controlPointX3D()->getValue();
        else
            return;
    }
    if (m_scene->isX3d() && (node->getType() == VRML_NURBS_CURVE)) {
        NodeNurbsCurve *nurbs = (NodeNurbsCurve *)node;
        if (nurbs->controlPointX3D()->getValue())
            node = nurbs->controlPointX3D()->getValue();
        else
            return;
    }
    AnimationDialog dlg(m_wnd, node);
    if (dlg.DoModal() == IDCANCEL)
        return;
    MyArray<int> eventInFields = dlg.getEventInFields();
    MyArray<int> eventInTypes = dlg.getEventInTypes();
    MyArray<bool> eventInIsAnimated = dlg.getEventInIsAnimated();

    Node *insertNode = getInsertNode(node);

    bool animateSomething = false;
    for (size_t i = 0 ; i < eventInFields.size(); i++) 
        if (eventInIsAnimated[i]) {
            animateSomething = true;
            break;
        }

    if (animateSomething) {
        m_scene->setSelection(insertNode);
        NodeTimeSensor *timeSensor = dlg.getTimeSensor();
        if (timeSensor == NULL) {
            timeSensor = (NodeTimeSensor *) createNode("TimeSensor");
            timeSensor->cycleInterval(new SFTime(dlg.getTimeSensorSeconds()));
            timeSensor->loop(new SFBool(true));
            timeSensor->start(swGetCurrentTime());
        }
        for (size_t i = 0 ; i < eventInFields.size(); i++) {
            if (eventInIsAnimated[i]) {
                m_scene->setSelection(insertNode);
                Node* interpolator = NULL;
                switch (eventInTypes[i]) {
                  case SFCOLOR:
                    interpolator = createNode("ColorInterpolator"); 
                    break;
                  case MFVEC2F:
                    interpolator = createNode("CoordinateInterpolator2D");
                    break;
                  case MFVEC3F:
                    {
                    const char *mfvec3finterpolator;                     
                    if ((node->getType() == VRML_NORMAL) && 
                        (eventInFields[i] == 
                         ((NodeNormal *) node)->vector_Field()))
                        mfvec3finterpolator = "NormalInterpolator";
                    else
                        mfvec3finterpolator = "CoordinateInterpolator";
                    interpolator = createNode(mfvec3finterpolator);
                    break;
                    }
                  case SFVEC2F:
                    interpolator = createNode("PositionInterpolator2D"); 
                    break;
                  case SFVEC3F:
                    interpolator = createNode("PositionInterpolator"); 
                    break;
                  case SFROTATION:
                    interpolator = createNode("OrientationInterpolator");
                    break;
                  case SFFLOAT:
                    interpolator = createNode("ScalarInterpolator"); 
                    break;
                }
                if (interpolator == NULL)
                    continue;
                m_scene->execute(new RouteCommand(timeSensor, 
                      timeSensor->fraction_changed_Field(), 
                      interpolator,
                      interpolator->getProto()->lookupEventIn("set_fraction")));
                m_scene->execute(new RouteCommand(interpolator, 
                      interpolator->getProto()->lookupEventOut("value_changed"),
                      node, eventInFields[i]));
            }
        }
    if (TheApp->is4Kids())
        ShowView(PW_BOTTOM, m_channelView, ID_DUNE_VIEW_CHANNEL_VIEW, 
                 "ShowChannelView");
    }
    m_scene->setSelection(node);
    m_scene->UpdateViews(NULL, UPDATE_SELECTION);
 }

void
MainWindow::createCurveAnimation(void)
{
    Node *selection = m_scene->getSelection()->getNode();

    Node *insertNode = m_scene->getRoot();
    int insertField = m_scene->getRootField();

    if (!selection->supportCurveAnimation())
        return;

    CurveAnimationDialog dlg(m_wnd, 9, 2, 3, m_scene, 5);
    if (dlg.DoModal() == IDOK) {
        int degree = dlg.getDegree();
        int dimension = dlg.getnPoints();
        int direction = dlg.getDirection();

        int order = degree + 1;
        
        NodeCurveAnimation *curve = (NodeCurveAnimation *)
              m_scene->createNode("CurveAnimation");       
        float *controlPoints = new float[dimension * 3];
        float *weights = new float[dimension];
        float *knots = new float[dimension + order]; 

        if (direction == 3) {        
            int numPoints = dimension;
            float *ring = new float[numPoints * 3];
            float radius = 1.0;
            int x = 0;
            int y = 2;
            int z = 1;
                
            for (int i = 0; i < numPoints; i++) {
                ring[i * 3 + x] = radius * sin(i * 2.0f * 
                                               M_PI / (numPoints - 1));
                ring[i * 3 + y] = radius * cos(i * 2.0f *
                                               M_PI / (numPoints - 1));;
                ring[i * 3 + z] = 0;
                if ((i == 1) || (i == numPoints - 2)) {
                    if (numPoints <= 12)
                        weights[i] = sqrt(2.0f) / 2 * numPoints / 9;
                    else
                        weights[i] = 1;
                    if (i == 1)
                        ring[i * 3 + x] -= 2.5f / numPoints;
                    else
                        ring[i * 3 + x] += 2.5f / numPoints;
                    ring[i * 3 + y] += 0.54 / numPoints;
                } else 
                    weights[i] = 1.0f;
            }
            float maxY = 1;
            ring[x] = 0;
            ring[y] = maxY;
            ring[z] = 0;
            ring[(numPoints - 1) * 3 + x] = 0;
            ring[(numPoints - 1) * 3 + y] = maxY;
            ring[(numPoints - 1) * 3 + z] = 0;
            curve->createControlPoints(new MFVec3f(ring, dimension * 3));
        } else if (direction == 4) {        
            int numPoints = dimension;
            float *ring = new float[numPoints * 3];
            float radius = 1.0;
            int x = 0;
            int y = 2;
            int z = 1;
                
            for (int j = 0; j < numPoints; j++) {
                int i = numPoints - 1 - j;
                ring[i * 3 + x] = radius * sin(j * 2.0f * 
                                               M_PI / (numPoints - 1));
                ring[i * 3 + y] = radius * cos(j * 2.0f * 
                                               M_PI / (numPoints - 1));;
                ring[i * 3 + z] = 0;
                if ((i == 1) || (i == numPoints - 2)) {
                    if (numPoints <= 12)
                        weights[i] = sqrt(2.0f) / 2 * numPoints / 9;
                    else
                        weights[i] = 1;
                    if (i == 1)
                        ring[i * 3 + x] += 2.5f / numPoints;
                    else
                        ring[i * 3 + x] -= 2.5f / numPoints;
                    ring[i * 3 + y] += 0.54 / numPoints;
                } else 
                    weights[i] = 1.0f;
            }
            float maxY = 1;
            ring[x] = 0;
            ring[y] = maxY;
            ring[z] = 0;
            ring[(numPoints - 1) * 3 + x] = 0;
            ring[(numPoints - 1) * 3 + y] = maxY;
            ring[(numPoints - 1) * 3 + z] = 0;
            curve->createControlPoints(new MFVec3f(ring, dimension * 3));
        } else {
            for (int i = 0; i < dimension; i++){
                float point = ((float) i - 0.5 * (dimension - 1.0)) / 
                          (dimension - 1.0) * 2.0;
                controlPoints[(i*3)]   = (direction == 0 ? point : 0);
                controlPoints[(i*3)+1] = (direction == 1 ? point : 0);
                controlPoints[(i*3)+2] = (direction == 2 ? point : 0);
                weights[i] = 1.0f;
            }
            curve->createControlPoints(new MFVec3f(controlPoints, dimension * 3));
        }    

        curve->weight(new MFFloat(weights, dimension));

        for (int i = 0; i < order; i++) {
            knots[i] = 0.0f;
            knots[dimension + i] = (float) (dimension - order + 1);
        }

        for (int i = 0; i < dimension - order; i++) {
            knots[order + i] = (float) (i + 1);
        }
    
        curve->knot(new MFFloat(knots, dimension + order));
        curve->order(new SFInt32(order));
        m_scene->execute(new MoveCommand(curve, NULL, -1, 
                                         insertNode, insertField));

        NodeTimeSensor *timeSensor = dlg.getTimeSensor();
        if (timeSensor == NULL) {
            timeSensor = (NodeTimeSensor *) createNode("TimeSensor");
            timeSensor->cycleInterval(new SFTime(dlg.getTimeSensorSeconds()));
            timeSensor->loop(new SFBool(true));
            timeSensor->start(swGetCurrentTime());
        }
        m_scene->execute(new RouteCommand(timeSensor, 
              timeSensor->fraction_changed_Field(), curve,
              curve->getProto()->lookupEventIn("set_fraction")));
        int eventInPosition;
        int eventInOrientation;
        if (selection->getType() == VRML_TRANSFORM) {
            NodeTransform *transform = (NodeTransform *)selection;
            eventInPosition = transform->translation_Field();
            eventInOrientation = transform->rotation_Field();
            if (selection->isInScene(m_scene)) {
                insertNode = selection;
                insertField = transform->children_Field();
            }
        } else if (selection->getType() == VRML_VIEWPOINT) {
            NodeViewpoint *viewpoint = (NodeViewpoint *)selection;
            eventInPosition = viewpoint->position_Field();
            eventInOrientation = viewpoint->orientation_Field();
        } else if (selection->getType() == X3D_HANIM_HUMANOID) {
            NodeHAnimHumanoid *human = (NodeHAnimHumanoid *)selection;
            eventInPosition = human->translation_Field();
            eventInOrientation = human->rotation_Field();
        } else 
            return;
        m_scene->execute(new RouteCommand(curve,
              curve->getProto()->lookupEventOut("position_changed"),
              selection, eventInPosition));
        m_scene->execute(new RouteCommand(curve,
              curve->getProto()->lookupEventOut("orientation_changed"),
              selection, eventInOrientation));
        if (TheApp->is4Kids())
            ShowView(PW_BOTTOM, m_channelView, ID_DUNE_VIEW_CHANNEL_VIEW, 
                 "ShowChannelView");
        m_scene->setSelection(curve);
        m_scene->UpdateViews(NULL, UPDATE_SELECTION);
    }
 }

void
MainWindow::createInteraction(void)
{
    Node *node = m_scene->getSelection()->getNode();
    if (node == m_scene->getRoot())
        return;
    if (!node->supportInteraction())
        return;
    InteractionRouteData routeData;
    InteractionDialog dlg(m_wnd, node, &routeData);
    if (dlg.DoModal() == IDCANCEL)
        return;
    InteractionDialog2 dlg2(m_wnd, m_scene, &routeData);
    if (dlg2.DoModal() == IDCANCEL)
        return;
    if (routeData.node == NULL) 
        routeData.node = insertNode(routeData.nodeProtoName); 
    Node *sensor = routeData.node;
    InteractionDialog3 dlg3(m_wnd, m_scene, &routeData);
    if (dlg3.DoModal() == IDCANCEL)
        return;

    m_scene->addRoute(sensor, routeData.eventOutField,
                     node, routeData.eventInField);
}

void
MainWindow::EditProto(Proto* oldProto)
{
    ProtoDialog dlg(m_wnd, oldProto);
    dlg.DoModal();
}

void
MainWindow::EditScript(Node* oldNode)
{
    ScriptDialog dlg(m_wnd, oldNode);
    dlg.DoModal();
}

void
MainWindow::createScript()
{
    Node *node = new NodeScript(m_scene);
    EditScript(node);
    m_scene->execute(new MoveCommand(node, NULL, -1, m_scene->getRoot(), 
                                     m_scene->getRootField()));
    m_scene->setSelection(node);
    m_scene->UpdateViews(NULL, UPDATE_SELECTION);
}

bool
MainWindow::isEditorInUse(bool errormessage)
{
    bool editorInUse = false;
    int id = -1;

    if (m_scriptEditorInUse) {
        id = IDS_SCRIPT_EDITOR_IN_USE_ERROR;
        editorInUse = true;
    }
    if (m_textEditorInUse) {
        id = IDS_TEXT_EDITOR_IN_USE_ERROR;
        editorInUse = true;
    }
    if (m_imageEditorInUse) { 
        id = IDS_IMAGE_EDITOR_IN_USE_ERROR;
        editorInUse = true;
    }
    if (m_soundEditorInUse) { 
        id = IDS_SOUND_EDITOR_IN_USE_ERROR;
        editorInUse = true;
    }
    if (m_movieEditorInUse) { 
        id = IDS_MOVIE_EDITOR_IN_USE_ERROR;
        editorInUse = true;
    }
    
    if (id != -1)
        if (errormessage)
            TheApp->MessageBoxId(id);

    return editorInUse;
}

void
MainWindowScriptEditorReadyCallback(void *data)
{
    MainWindow *mainWindow = (MainWindow *) data;
    mainWindow->OnScriptEditorReadyCallback();
}

void
MainWindow::OnScriptEditorReadyCallback(void)
{
    NodeScript *node = (NodeScript *)m_scriptEdit->getNode();
    FieldUpdate* hint = new FieldUpdate(node, node->url_Field());
    m_scene->UpdateViews(NULL, UPDATE_FIELD, (Hint *) hint);
    delete hint;
    m_scriptEditorInUse = false;
    if (m_scriptEdit != NULL)
        delete  m_scriptEdit;
    m_scene->setExtraModifiedFlag();
    UpdateToolbar(ID_OBJECT_EDIT);
}

void
MainWindowTextEditorReadyCallback(void *data)
{
    MainWindow *mainWindow = (MainWindow *) data;
    mainWindow->OnTextEditorReadyCallback();
}

void
MainWindow::OnTextEditorReadyCallback(void)
{
    Node *node = m_textEdit->getNode();
    int field = -1;
    switch(node->getType()) {
      case VRML_TEXT:
        {
        NodeText *text = (NodeText *)node;
        field = text->string_Field();
        }
        break;
      case KAMBI_TEXT_3D:
        {
        NodeText3D *text = (NodeText3D *)node;
        field = text->string_Field();
        }
        break;
      case VRML_WORLD_INFO:
        {
        NodeWorldInfo *world = (NodeWorldInfo *)node;
        field = world->info_Field();
        }
        break;
    }
    FieldUpdate* hint = new FieldUpdate(node, field);
    m_scene->UpdateViews(NULL, UPDATE_FIELD, (Hint *) hint);
    delete hint;
    m_textEditorInUse = false;
    if (m_textEdit != NULL)
        delete m_textEdit;
    m_textEdit = NULL;
    UpdateToolbar(ID_OBJECT_EDIT);
}

void
MainWindowImageTextureEditorReadyCallback(void *data)
{
    MainWindow *mainWindow = (MainWindow *) data;
    mainWindow->OnImageTextureEditorReadyCallback();
}

void
MainWindow::OnImageTextureEditorReadyCallback(void)
{
    NodeImageTexture *node = (NodeImageTexture *)m_imageTextureEdit->getNode();
    FieldUpdate* hint = new FieldUpdate(node, node->url_Field());
    m_scene->UpdateViews(NULL, UPDATE_FIELD, (Hint *) hint);
    delete hint;
    m_imageEditorInUse = false;
    if (m_imageTextureEdit != NULL)
        delete  m_imageTextureEdit;
    node->unload();
    UpdateToolbar(ID_OBJECT_EDIT);
    m_scene->UpdateViews(NULL, UPDATE_REDRAW_3D, NULL);
}

void
MainWindowSoundEditorReadyCallback(void *data)
{
    MainWindow *mainWindow = (MainWindow *) data;
    mainWindow->OnSoundEditorReadyCallback();
}

void
MainWindow::OnSoundEditorReadyCallback(void)
{
    NodeAudioClip *node = (NodeAudioClip *)m_soundEdit->getNode();
    FieldUpdate* hint = new FieldUpdate(node, node->url_Field());
    m_scene->UpdateViews(NULL, UPDATE_FIELD, (Hint *) hint);
    delete hint;
    m_soundEditorInUse = false;
    if (m_soundEdit != NULL)
        delete  m_soundEdit;
    UpdateToolbar(ID_OBJECT_EDIT);
}

void
MainWindowMovieEditorReadyCallback(void *data)
{
    MainWindow *mainWindow = (MainWindow *) data;
    mainWindow->OnMovieEditorReadyCallback();
}

void
MainWindow::OnMovieEditorReadyCallback(void)
{
    NodeMovieTexture *node = (NodeMovieTexture *)m_movieEdit->getNode();
    FieldUpdate* hint = new FieldUpdate(node, node->url_Field());
    m_scene->UpdateViews(NULL, UPDATE_FIELD, (Hint *) hint);
    delete hint;
    m_movieEditorInUse = false;
    if (m_movieEdit != NULL)
        delete  m_movieEdit;
    UpdateToolbar(ID_OBJECT_EDIT);
}

void
MainWindow::EditText(Node* oldNode)
{
    m_textEdit  = new TextEdit(oldNode, m_wnd, 
                               MainWindowTextEditorReadyCallback, this);
    m_textEdit->edit();
}

void
MainWindow::EditUrl()
{
    Node *node = m_scene->getSelection()->getNode();
    switch (node->getType()) {
      case VRML_SCRIPT:
        if (isEditorInUse(true))
            return;

        m_scriptEditorInUse = true;
        m_scriptEdit = new ScriptEdit((NodeScript *)node, m_wnd, 
                                     MainWindowScriptEditorReadyCallback, this);
        if (m_scriptEdit->edit()) {
            m_scene->setSelection(node);
            m_scene->UpdateViews(NULL, UPDATE_SELECTION);
        } else 
            m_scriptEditorInUse = false;
        break;
      case X3D_SHADER_PROGRAM:
      case X3D_PACKAGED_SHADER:
      case X3D_SHADER_PART:
        if (isEditorInUse(true))
            return;

        m_textEditorInUse = true;
        m_shaderEdit = new ShaderEdit(node, m_wnd, 
                                      MainWindowTextEditorReadyCallback, this);
        if (m_shaderEdit->edit()) {
            m_scene->setSelection(node);
            m_scene->UpdateViews(NULL, UPDATE_SELECTION);
        } else 
            m_textEditorInUse = false;
        break;
      case VRML_IMAGE_TEXTURE:
        if (isEditorInUse(true))
            return;

        m_imageEditorInUse = true;
        m_imageTextureEdit = new ImageTextureEdit((NodeImageTexture *)node, 
              m_wnd, MainWindowImageTextureEditorReadyCallback, this);
        if (m_imageTextureEdit->edit(true, TheApp->is4Kids())) {
            ((NodeImageTexture *)node)->unload();
            m_scene->setSelection(node);
            m_scene->UpdateViews(NULL, UPDATE_SELECTION);
        } else {
            m_imageEditorInUse = false;
            ((NodeImageTexture *)node)->unload();
            node->update();
            m_scene->UpdateViews(NULL, UPDATE_REDRAW);
        }
        break;
      case VRML_AUDIO_CLIP:
        if (isEditorInUse(true))
            return;

        m_soundEditorInUse = true;
        m_soundEdit = new AudioClipEdit((NodeAudioClip *)node, m_wnd, 
                                        MainWindowSoundEditorReadyCallback, 
                                        this);
        if (m_soundEdit->edit()) {
            m_scene->setSelection(node);
            m_scene->UpdateViews(NULL, UPDATE_SELECTION);
        } else 
            m_soundEditorInUse = false;
        break;
      case VRML_MOVIE_TEXTURE:
        if (isEditorInUse(true))
            return;

        m_movieEditorInUse = true;
        m_movieEdit = new MovieTextureEdit((NodeMovieTexture *)node, m_wnd, 
                                           MainWindowMovieEditorReadyCallback, 
                                           this);
        if (m_movieEdit->edit()) {
            m_scene->setSelection(node);
            m_scene->UpdateViews(NULL, UPDATE_SELECTION);
        } else 
            m_movieEditorInUse = false;
        break;
    }
}

void
MainWindow::EditObject()
{
    const Path *selection = m_scene->getSelection();
    Node *node = selection->getNode();
    NodeUpdate *hint = NULL;
    if (selection->isProto(m_scene)) {
        Proto *proto = selection->getProto(m_scene);
        EditProto(proto);
        m_scene->UpdateViews(NULL, UPDATE_SELECTION);
        m_scene->updateNodePROTOs(proto);
        return;
    }
    switch (node->getType()) {
      case VRML_SCRIPT:
      case X3D_SHADER_PROGRAM:
      case X3D_PACKAGED_SHADER:
      case X3D_COMPOSED_SHADER:
        if (m_scriptEditorInUse) {
            TheApp->MessageBoxId(IDS_SCRIPT_EDITOR_IN_USE_ERROR);
            return;
        }
        EditScript(node);
        hint = new NodeUpdate(node, NULL, 0);
        m_scene->UpdateViews(NULL, UPDATE_CHANGE_INTERFACE_NODE, (Hint*) hint);
        break;
      case VRML_TEXT:
      case KAMBI_TEXT_3D:
      case VRML_WORLD_INFO:
        if (m_textEditorInUse) {
            TheApp->MessageBoxId(IDS_TEXT_EDITOR_IN_USE_ERROR);
            return;
        }
        EditText(node);
        hint = new NodeUpdate(node, NULL, 0);
        m_scene->UpdateViews(NULL, UPDATE_CHANGE_INTERFACE_NODE, (Hint*) hint);
        break;
    }
    m_scene->setSelection(node);
    m_scene->UpdateViews(NULL, UPDATE_SELECTION);
}

void
MainWindow::createViewpoint()
{
    NodeViewpoint *node = (NodeViewpoint *)m_scene->createNode("Viewpoint");
    Node *camera = m_scene->getCamera();

    node->fieldOfView((SFFloat *)(camera->fov()->copy()));
//    node->jump((SFBool *)camera->jump()->copy());
    SFRotation *r = new SFRotation(camera->getOrientation());
    node->orientation(r);
    Vec3d pos = camera->getPosition();
    node->position(new SFVec3f(pos.x, pos.y, pos.z));
//    node->description((SFString *)camera->description()->copy());
    m_scene->execute(new MoveCommand(node, NULL, -1, m_scene->getRoot(), 
                                    m_scene->getRootField()));
    m_scene->setSelection(node);
    m_scene->UpdateViews(NULL, UPDATE_SELECTION);
}

void
MainWindow::createGeoViewpoint()
{
    const char *nodeName = "GeoViewpoint";
    NodeGeoViewpoint *node = (NodeGeoViewpoint *)m_scene->createNode(nodeName);
    Node *camera = m_scene->getCamera();

//    node->fieldOfView((SFFloat *)camera->fieldOfView()->copy());
//    node->jump((SFBool *)camera->jump()->copy());
    SFRotation r(camera->getOrientation());
    node->orientation(r);
    char buf[4096];
    Vec3d pos = camera->getPosition();
    mysnprintf(buf, 4095, "%g %g %g", pos.x, pos.y, pos.z);
    node->position(new SFString(buf));
//    node->description((SFString *)camera->description()->copy());
    m_scene->execute(new MoveCommand(node, NULL, -1, m_scene->getRoot(), 
                                    m_scene->getRootField()));
    m_scene->setSelection(node);
    m_scene->UpdateViews(NULL, UPDATE_SELECTION);
}

void
MainWindow::createElevationGrid()
{
    ElevationGridDialog   dlg(m_wnd, 5, 5);
    if (dlg.DoModal() == IDOK) {
        int width = dlg.GetWidth();
        int depth = dlg.GetDepth();
        NodeElevationGrid *node = (NodeElevationGrid *)
                                  m_scene->createNode("ElevationGrid");

        float *values = new float[width * depth];
        memset(values, 0, width * depth * sizeof(float));
        node->height(new MFFloat(values, width * depth));
        node->xDimension(new SFInt32(width));
        node->zDimension(new SFInt32(depth));
        createGeometryNode(node);
    }
}

void
MainWindow::createGeoElevationGrid()
{
    ElevationGridDialog   dlg(m_wnd, 5, 5);
    if (dlg.DoModal() == IDOK) {
        int width = dlg.GetWidth();
        int depth = dlg.GetDepth();
        NodeGeoElevationGrid *node = (NodeGeoElevationGrid *)
                                     m_scene->createNode("GeoElevationGrid");

        float *values = new float[width * depth];
        memset(values, 0, width * depth * sizeof(float));
        node->height(new MFFloat(values, width * depth));
        node->xDimension(new SFInt32(width));
        node->zDimension(new SFInt32(depth));
        createGeometryNode(node);
    }
}

void
MainWindow::createNurbsPlane(int direction)
{
    int idd = IDD_NEW_NURBS_PLANE_XY;
    if (direction == 1)
        idd = IDD_NEW_NURBS_PLANE_YZ;
    else if (direction == 2)
        idd = IDD_NEW_NURBS_PLANE_ZX;
    NurbsPlaneDialog dlg(m_wnd, idd, 4, 4, 3, 3);
    if (dlg.DoModal() == IDOK) {
        int uDimension = dlg.GetN1();
        int vDimension = dlg.GetN2();
        int uOrder = dlg.GetUDegree() + 1;
        int vOrder = dlg.GetVDegree() + 1;
        NodeNurbsSurface *node = (NodeNurbsSurface *)
                                 m_scene->createNode("NurbsSurface");

        float *controlPoints = new float[uDimension * vDimension * 3];
        float *weights = new float[uDimension * vDimension];
        float *uKnots = new float[uDimension + uOrder];
        float *vKnots = new float[vDimension + vOrder];
        // control Points in a list, first for x, second for y, third for z 
        // and so on
        for (int j = 0; j < vDimension; j++) {
            for (int i = 0; i < uDimension; i++) {
                float n1 = (float) i - 0.5 * (uDimension - 1.0);
                float n2 = (float) j - 0.5 * (vDimension - 1.0);
                float x = 0;
                float y = 0;
                float z = 0;
                switch (direction) {
                  case 0:
                    x = n1;
                    y = n2;
                    break;
                  case 1:
                    y = n1;
                    z = n2;
                    break;
                  case 2:
                    z = n1;
                    x = n2;
                    break;
                }
                controlPoints[(j * uDimension + i) * 3    ] = x; 
                controlPoints[(j * uDimension + i) * 3 + 1] = y;
                controlPoints[(j * uDimension + i) * 3 + 2] = z;
                weights[j*uDimension + i] = 1.0f;
            }
        }
        //make Bezier-like behaviour of spline in first control point
        // --> let splines start out of that point
        for (int i = 0; i < uOrder; i++) {
            uKnots[i] = 0.0f;
        }
        for (int i = 0; i < uDimension - uOrder; i++) {
            uKnots[uOrder + i] = (float) (i + 1);
        }
        for (int i = 0; i < uOrder; i++) {
            uKnots[uDimension + i] = (float) (uDimension - uOrder + 1);
        }
        //Same as above for v-direction
        for (int j = 0; j < vOrder; j++) {
            vKnots[j] = 0.0f;
        }
        for (int j = 0; j < vDimension - vOrder; j++) {
            vKnots[vOrder + j] = (float) (j + 1);
        }
        for (int j = 0; j < vOrder; j++) {
            vKnots[vDimension + j] = (float) (vDimension - vOrder + 1);
        }
        node->uDimension(new SFInt32(uDimension));
        node->vDimension(new SFInt32(vDimension));
        node->uKnot(new MFFloat(uKnots, uDimension + uOrder));
        node->vKnot(new MFFloat(vKnots, vDimension + vOrder));
        node->uOrder(new SFInt32(uOrder));
        node->vOrder(new SFInt32(vOrder));
        node->solid(new SFBool(false));
        node->ccw(new SFBool(false));
        node->createControlPoints(
              new MFVec3f(controlPoints, uDimension * vDimension * 3));
        node->weight(new MFFloat(weights, uDimension * vDimension));
        createGeometryNode(node);
    }
}

void
MainWindow::createNurbsCurve2D()
{
    Node *selection = m_scene->getSelection()->getNode();
    int selectedField = m_scene->getSelection()->getField();
    int type = -1;
    bool isContour = false;
    switch (selection->getType()) {
      case VRML_CONTOUR_2D:
        isContour = true;
      case X3D_NURBS_SWEPT_SURFACE:
      case X3D_NURBS_SWUNG_SURFACE:
        type = NURBS_CONTROL_CURVE_NODE;
        break;
    }
    if (type == -1)
        return;
    if (selectedField == -1)
        selectedField = selection->findFirstValidFieldType(type);
    if (!selection->validChildType(selectedField, type))
       return;

    NurbsCurve2DDialog dlg(m_wnd, IDD_NURBS_CURVE_2D, m_scene, 9);
    if (dlg.DoModal() == IDOK) {
        int dimension = dlg.GetPoints();
        int degree = 3;
        int order = degree + 1;
        
        NodeNurbsCurve2D *node = (NodeNurbsCurve2D *)
                                  m_scene->createNode("NurbsCurve2D");       
        double *controlPoints = new double[dimension * 2];
        float *weights = new float[dimension];
        float *knots = new float[dimension + order]; 
        
        if (isContour) {
            // use a closed circle as default
            for (int i = 0; i < dimension - 1; i++) {
                float angle = 2.0 * (dlg.GetCcw() ? dimension - 1 - i : i) / 
                                  (dimension - 1) * M_PI; 
                controlPoints[(i*2)]   = sin(angle) * dlg.GetSize();
                controlPoints[(i*2)+1] = cos(angle) * dlg.GetSize();
                weights[i] = 1.0f;
            }
            controlPoints[(dimension - 1) * 2] = sin(0) * dlg.GetSize();
            controlPoints[(dimension - 1) * 2 + 1] = cos(0) * dlg.GetSize();
            weights[dimension - 1] = 1.0f;
        } else {
            for (int i=0; i < dimension; i++){
                float point = dlg.GetSize() * i / (dimension - 1.0);
                controlPoints[(i*2)]   = point;
                controlPoints[(i*2)+1] = 0;
                weights[i] = 1.0f;
        }

        }
     
        for (int i = 0; i < order; i++) {
            knots[i] = 0.0f;
            knots[dimension + i] = (float) (dimension - order + 1);
        }
        for (int i = 0; i < dimension - order; i++) {
            knots[order + i] = (float) (i + 1);
        }
    
        node->controlPoint(new MFVec2d(controlPoints, dimension * 2));
        node->weight(new MFFloat(weights, dimension));
        node->knot(new MFFloat(knots, dimension + order));
        node->order(new SFInt32(order));
    
        m_scene->execute(new MoveCommand(node, NULL, -1, 
                                        selection, selectedField));
        m_scene->setSelection(node);
        m_scene->UpdateViews(NULL, UPDATE_SELECTION);
    }
}

void
MainWindow::createCurveAnimationNode()
{
    Node *selection = m_scene->getSelection()->getNode();
    int selectedField = m_scene->getSelection()->getField();
    int type = DUNE_CURVE_ANIMATION;
    if (selectedField == -1)
        selectedField = selection->findFirstValidFieldType(type);
    if (!selection->validChildType(selectedField, type)) {
        selection = m_scene->getRoot(),
        selectedField = m_scene->getRootField();
    }
    CurveAnimationDialog dlg(m_wnd, 5, 2, 2, m_scene, 5);
    if (dlg.DoModal() == IDOK) {
        int degree = dlg.getDegree();
        int dimension = dlg.getnPoints();
        int direction = dlg.getDirection();

        int order = degree + 1;
        
        NodeCurveAnimation *curve = (NodeCurveAnimation *)
                                m_scene->createNode("CurveAnimation");       
        float *controlPoints = new float[dimension * 3];
        float *weights = new float[dimension];
        float *knots = new float[dimension + order]; 
        
        for (int i=0; i<dimension; i++){
            float point = ((float) i - 0.5 * (dimension - 1.0)) / 
                          (dimension - 1.0) * 2.0;
            controlPoints[(i*3)]   = (direction == 0 ? point : 0);
            controlPoints[(i*3)+1] = (direction == 1 ? point : 0);
            controlPoints[(i*3)+2] = (direction == 2 ? point : 0);
            weights[i] = 1.0f;
        }
     
        for (int i = 0; i < order; i++) {
            knots[i] = 0.0f;
            knots[dimension + i] = (float) (dimension - order + 1);
        }
        for (int i = 0; i < dimension - order; i++) {
            knots[order + i] = (float) (i + 1);
        }
    
        curve->createControlPoints(new MFVec3f(controlPoints, dimension * 3));
        curve->weight(new MFFloat(weights, dimension));
        curve->knot(new MFFloat(knots, dimension + order));
        curve->order(new SFInt32(order));
    
        m_scene->execute(new MoveCommand(curve, NULL, -1, 
                                        selection, selectedField));
        m_scene->setSelection(curve);
        m_scene->UpdateViews(NULL, UPDATE_SELECTION);
    }
}

void
MainWindow::createSuperExtrusion(SuperExtrusionInfo *info)
{
    int direction = -1;
    int numPoints = (info == NULL) ? 5 : info->numPoints;
    NurbsCurveDialog dlg(m_wnd, numPoints, 2, 1);
    if (dlg.DoModal() == IDOK) {
        int degree = dlg.getDegree();
        int dimension = dlg.getnPoints();
        numPoints = dimension;
        direction = dlg.getDirection();
        int order = degree + 1;

        info->numPoints = numPoints;
        info->direction = direction;
        
        createGeometryNode("SuperExtrusion");
        Node *node = m_scene->getSelection()->getNode();
        if (node && (node->getType() == DUNE_SUPER_EXTRUSION)) {
            NodeSuperExtrusion *superExtrusion = (NodeSuperExtrusion *) node;
            float *controlPoints = new float[dimension * 3];
            float *weights = new float[dimension];
            float *knots = new float[dimension + order]; 
            
            for (int i = 0; i < dimension; i++){
                float point = (float) i / (float) (dimension - 1.0f);
                controlPoints[(i * 3)]     = (direction == 0 ? point : 0);
                controlPoints[(i * 3) + 1] = (direction == 1 ? point : 0);
                controlPoints[(i * 3) + 2] = (direction == 2 ? point : 0);
                weights[i] = 1.0f;
            }
         
            for (int i = 0; i < order; i++) {
                knots[i] = 0.0f;
                knots[dimension + i] = (float) (dimension - order + 1);
            }
            for (int i = 0; i < dimension - order; i++) {
                knots[order + i] = (float) (i + 1);
            }
        
            superExtrusion->controlPoint(new MFVec3f(controlPoints, 
                                                     dimension * 3));    
            superExtrusion->weight(new MFFloat(weights, dimension));
            superExtrusion->knot(new MFFloat(knots, dimension + order));
            superExtrusion->order(new SFInt32(order));
            if (TheApp->is4Kids()) {
                float *scale = new float[dimension * 2];
                for (int i = 0; i < dimension; i++){
                    scale[i * 2    ] = 1;
                    scale[i * 2 + 1] = 1;
                }
                superExtrusion->scale(new MFVec2f(scale, dimension * 2));    
            }
        }        
        m_scene->setSelection(node);
        m_scene->UpdateViews(NULL, UPDATE_SELECTION);
    }
}

void
MainWindow::createSuperRevolver()
{
    int degree = 2;

    OneIntDialog dlg(m_wnd, IDD_SUPER_REVOLVER_POINTS, 7, 3, INT_MAX);
    if (dlg.DoModal() == IDCANCEL)
        return;

    int dimension = dlg.GetValue();

    int order = degree + 1;
    
    Node *node = createGeometryNode("SuperRevolver");
    if (node && (node->getType() == DUNE_SUPER_REVOLVER)) {
        NodeSuperRevolver *superRevolver = (NodeSuperRevolver *) node;
        float *controlPoints = new float[dimension * 2];
        float *weights = new float[dimension];
        float *knots = new float[dimension + order]; 
        
        for (int i = 0; i < dimension; i++) {
            float y = i / (dimension - 1.0);
            controlPoints[i * 2] = sin(y * M_PI);
            controlPoints[i * 2 + 1] = y;
            weights[i] = 1.0f;
        }
        controlPoints[0] = 0.0f;
        controlPoints[(dimension - 1) * 2] = 0.0f;
     
        for (int i = 0; i < order; i++) {
            knots[i] = 0.0f;
            knots[dimension + i] = (float) (dimension - order + 1);
        }
        for (int i = 0; i < dimension - order; i++) {
            knots[order + i] = (float) (i + 1);
        }
    
        superRevolver->controlPoint(new MFVec2f(controlPoints, dimension * 2));
        superRevolver->weight(new MFFloat(weights, dimension));
        superRevolver->knot(new MFFloat(knots, dimension + order));
        superRevolver->order(new SFInt32(order));
    }
    m_scene->setSelection(node);
    m_scene->UpdateViews(NULL, UPDATE_SELECTION);
}

void
MainWindow::createNurbsCurve()
{
    Node *selectedNode = m_scene->getSelection()->getNode();

    NurbsCurveDialog dlg(m_wnd, 5, 2, 
                         selectedNode->getType() == X3D_NURBS_SWEPT_SURFACE);
    if (dlg.DoModal() == IDOK) {
        int degree = dlg.getDegree();
        int dimension = dlg.getnPoints();
        int direction = dlg.getDirection();

        int order = degree + 1;
        
        Node *selection = m_scene->getSelection()->getNode();
        int selectedField = m_scene->getSelection()->getField();
        NodeNurbsCurve *node = NULL;
        if (selection->getType() == X3D_NURBS_SWEPT_SURFACE) {
            NodeNurbsSweptSurface *sweptSurface = (NodeNurbsSweptSurface *)
                                                  selection;
            node = (NodeNurbsCurve *)m_scene->createNode("NurbsCurve");
            if (selectedField == -1)
                selectedField = sweptSurface->trajectoryCurve_Field();
            if (sweptSurface->trajectoryCurve_Field() == selectedField)
                m_scene->execute(new MoveCommand(node, NULL, -1, 
                                                selection, selectedField));
        } else
            node = (NodeNurbsCurve *)createGeometryNode("NurbsCurve", true);

        float *controlPoints = new float[dimension * 3];
        float *weights = new float[dimension];
        float *knots = new float[dimension + order]; 
        
        for (int i=0; i<dimension; i++){
            float point = ((float) i - 0.5 * (dimension - 1.0)) / 
                          (dimension - 1.0) * 2.0;
            controlPoints[(i*3)]   = (direction == 0 ? point : 0);
            controlPoints[(i*3)+1] = (direction == 1 ? point : 0);
            controlPoints[(i*3)+2] = (direction == 2 ? point : 0);
            weights[i] = 1.0f;
        }
     
        for (int i = 0; i < order; i++) {
            knots[i] = 0.0f;
            knots[dimension + i] = (float) (dimension - order + 1);
        }
        for (int i = 0; i < dimension - order; i++) {
            knots[order + i] = (float) (i + 1);
        }
    
        node->createControlPoints(new MFVec3f(controlPoints, dimension * 3));
        node->weight(new MFFloat(weights, dimension));
        node->knot(new MFFloat(knots, dimension + order));
        node->order(new SFInt32(order));
    
        m_scene->setSelection(node);
        m_scene->UpdateViews(NULL, UPDATE_SELECTION);
    }
}

void
MainWindow::createStarFish(int numberArms)
{
    createGeometryNode("SuperShape");
    Node *node = m_scene->getSelection()->getNode();
    if ((node) && (node->getType() == DUNE_SUPER_SHAPE)) {
        NodeSuperShape *superShape = (NodeSuperShape *) node;
        superShape->um(new SFFloat(numberArms));
        superShape->un1(new SFFloat(0.1));
        superShape->un2(new SFFloat(1.7));
        superShape->un3(new SFFloat(1.7));
        superShape->vm(new SFFloat(1.0));
        superShape->vn1(new SFFloat(0.3));
        superShape->vn2(new SFFloat(0.5));
        superShape->vn3(new SFFloat(0.5));
        if (numberArms > 4)
            superShape->uTessellation(new SFInt32(numberArms*9));
        superShape->vTessellation(new SFInt32(8));
    }
    m_scene->setSelection(node);
    m_scene->UpdateViews(NULL, UPDATE_SELECTION);
}

void
MainWindow::createFlower(int numberLeaves)
{
    createGeometryNode("SuperShape");
    Node *node = m_scene->getSelection()->getNode();
    if ((node) && (node->getType() == DUNE_SUPER_SHAPE)) {
        NodeSuperShape *superShape = (NodeSuperShape *) node;
        superShape->um(new SFFloat(numberLeaves));
        superShape->un2(new SFFloat(4.0));
        superShape->un3(new SFFloat(2.0));
        superShape->vm(new SFFloat(2.0));
        superShape->vn1(new SFFloat(1.2));
        superShape->border(new SFFloat(0.0));
    }
    m_scene->setSelection(node);
    m_scene->UpdateViews(NULL, UPDATE_SELECTION);
}


void
MainWindow::createHeart(void)
{
    NodeSuperRevolver *superRevolver = (NodeSuperRevolver *) 
                                       createGeometryNode("SuperRevolver");
    int dimension = 5;        
    int degree = 2;
    int order = degree + 1;
    float *controlPoints = new float[dimension * 2];
    float *weights = new float[dimension];
    float *knots = new float[dimension + order]; 
    
    controlPoints[0] = 0;
    controlPoints[1] = -0.3;

    controlPoints[2] = 0.6;
    controlPoints[3] = -0.3;

    controlPoints[4] = 1;
    controlPoints[5] = 0;

    controlPoints[6] = 0.6;
    controlPoints[7] = 0.3;

    controlPoints[8] = 0;
    controlPoints[9] = 0.3;

    for (int i = 0; i < dimension; i++)
        weights[i] = 1.0f;
    for (int i = 0; i < order; i++) {
        knots[i] = 0.0f;
        knots[dimension + i] = (float) (dimension - order + 1);
    }
    for (int i = 0; i < dimension - order; i++) {
        knots[order + i] = (float) (i + 1);
    }

    superRevolver->controlPoint(new MFVec2f(controlPoints, dimension * 2));
    superRevolver->weight(new MFFloat(weights, dimension));
    superRevolver->knot(new MFFloat(knots, dimension + order));
    superRevolver->order(new SFInt32(order));

    superRevolver->m(new SFFloat(2.5));
    superRevolver->n1(new SFFloat(0.46));
    superRevolver->n2(new SFFloat(1.8));
    superRevolver->n3(new SFFloat(0.88));

    NodeShape *shape = (NodeShape *)superRevolver->getParent();
    if (shape != NULL) {
         NodeTransform *nTransform = (NodeTransform *)shape->getParent();
         SFRotation *rot = new SFRotation(1, 0, 0, M_PI/2.0);
         if (nTransform != NULL)
             m_scene->setField(nTransform, nTransform->rotation_Field(), rot);
         NodeAppearance *nAppearance = (NodeAppearance *)
                                       shape->appearance()->getValue();
         if (nAppearance != NULL) {
              NodeMaterial *nMaterial = (NodeMaterial *)
                                        nAppearance->material()->getValue();
             SFColor* red = new SFColor(1, 0, 0);
             m_scene->setField(nMaterial, nMaterial->diffuseColor_Field(), red);
             SFColor* white = new SFColor(1, 1, 1);
             m_scene->setField(nMaterial, nMaterial->specularColor_Field(), 
                              white);
         }
    }
    m_scene->setSelection(superRevolver);
    m_scene->UpdateViews(NULL, UPDATE_SELECTION);
}

void
MainWindow::createSpindle(void)
{
    createGeometryNode("SuperRevolver");
    Node *node = m_scene->getSelection()->getNode();
    if ((node) && (node->getType() == DUNE_SUPER_REVOLVER)) {
        NodeSuperRevolver *superRevolver = (NodeSuperRevolver *) node;
        int dimension = 7;        
        int degree = 2;
        int order = degree + 1;
        float *controlPoints = new float[dimension * 2];
        float *weights = new float[dimension];
        float *knots = new float[dimension + order]; 
        
        controlPoints[0] = 0;
        controlPoints[1] = -3;

        controlPoints[2] = 2;
        controlPoints[3] = -2;

        controlPoints[4] = 2;
        controlPoints[5] = -1;

        controlPoints[6] = 2;
        controlPoints[7] = 0;

        controlPoints[8] = 2;
        controlPoints[9] = 1;

        controlPoints[10] = 2;
        controlPoints[11] = 2;

        controlPoints[12] = 0;
        controlPoints[13] = 3;

        for (int i = 0; i < dimension; i++)
            weights[i] = 1.0f;
        for (int i = 0; i < order; i++) {
            knots[i] = 0.0f;
            knots[dimension + i] = (float) (dimension - order + 1);
        }
        for (int i = 0; i < dimension - order; i++) {
            knots[order + i] = (float) (i + 1);
        }
    
        superRevolver->controlPoint(new MFVec2f(controlPoints, dimension * 2));
        superRevolver->weight(new MFFloat(weights, dimension));
        superRevolver->knot(new MFFloat(knots, dimension + order));
        superRevolver->order(new SFInt32(order));
    }
    m_scene->setSelection(node);
    m_scene->UpdateViews(NULL, UPDATE_SELECTION);
}

void
MainWindow::createMushroom(bool sulcate)
{
    createGeometryNode("SuperRevolver");
    Node *node = m_scene->getSelection()->getNode();
    if ((node) && (node->getType() == DUNE_SUPER_REVOLVER)) {
        NodeSuperRevolver *superRevolver = (NodeSuperRevolver *) node;
        int dimension = 7;        
        int degree = 2;
        int order = degree + 1;
        float *controlPoints = new float[dimension * 2];
        float *weights = new float[dimension];
        float *knots = new float[dimension + order]; 
        
        controlPoints[0] = 0;
        controlPoints[1] = 0;

        controlPoints[2] = 0.96;
        controlPoints[3] = -2.9e-2;

        controlPoints[4] = 0.40;
        controlPoints[5] = 1.3;

        controlPoints[6] = 0.69;
        controlPoints[7] = 1.65;

        controlPoints[8] = 1.71;
        controlPoints[9] = 1.21;

        controlPoints[10] = 0.94;
        controlPoints[11] = 2.14;

        controlPoints[12] = 0;
        controlPoints[13] = 2;

        for (int i = 0; i < dimension; i++)
            weights[i] = 1.0f;
        for (int i = 0; i < order; i++) {
            knots[i] = 0.0f;
            knots[dimension + i] = (float) (dimension - order + 1);
        }
        for (int i = 0; i < dimension - order; i++) {
            knots[order + i] = (float) (i + 1);
        }
    
        superRevolver->controlPoint(new MFVec2f(controlPoints, dimension * 2));
        superRevolver->weight(new MFFloat(weights, dimension));
        superRevolver->knot(new MFFloat(knots, dimension + order));
        superRevolver->order(new SFInt32(order));

        if (sulcate) {
            superRevolver->b(new SFFloat(0.7));
            superRevolver->m(new SFFloat(16));
            superRevolver->n1(new SFFloat(11));
            superRevolver->n2(new SFFloat(0.6));
            superRevolver->n3(new SFFloat(4.5));
        }
    }
    m_scene->setSelection(node);
    m_scene->UpdateViews(NULL, UPDATE_SELECTION);
}

void
MainWindow::createHalfSphere(void)
{
    createGeometryNode("SuperShape");
    Node *node = m_scene->getSelection()->getNode();
    if ((node) && (node->getType() == DUNE_SUPER_SHAPE)) {
        NodeSuperShape *superShape = (NodeSuperShape *) node;
        m_scene->setField(superShape, superShape->bottomBorder_Field(),
                         new SFFloat(0.0));
        m_scene->UpdateViews(NULL, UPDATE_SELECTION);
    }
    m_scene->setSelection(node);
    m_scene->UpdateViews(NULL, UPDATE_SELECTION);
}

void
MainWindow::createShell(void)
{
    createGeometryNode("SuperShape");
    Node *node = m_scene->getSelection()->getNode();
    if ((node) && (node->getType() == DUNE_SUPER_SHAPE)) {
        float shellsmooth = 0.5;
        if (TheApp->is4Kids()) {
            OneFloatDialog dlg(m_wnd, IDD_SHELL_SMOOTH, 0.5, 0, 1);
            if (dlg.DoModal() == IDCANCEL)
                return;
            shellsmooth = dlg.GetValue();
        }
        NodeSuperShape *superShape = (NodeSuperShape *) node;
        superShape->ua(new SFFloat(0.74));
        superShape->ub(new SFFloat(1e4));
        superShape->um(new SFFloat(-1.08));
        superShape->un1(new SFFloat(-2.04));
        superShape->un2(new SFFloat(-7.3));
        superShape->un3(new SFFloat(0.22));
        superShape->va(new SFFloat(-3.92));
        superShape->vb(new SFFloat(-7.019999));
        superShape->vm(new SFFloat(30.86));
        superShape->vn1(new SFFloat(10.26));
        superShape->vn2(new SFFloat(-shellsmooth));
        superShape->vn3(new SFFloat(0.28));
        superShape->uTessellation(new SFInt32(16));
        superShape->vTessellation(new SFInt32(16));
    }
    m_scene->setSelection(node);
    m_scene->UpdateViews(NULL, UPDATE_SELECTION);
}

void
MainWindow::createUfo(void)
{
    createGeometryNode("SuperShape");
    Node *node = m_scene->getSelection()->getNode();
    if ((node) && (node->getType() == DUNE_SUPER_SHAPE)) {
        NodeSuperShape *superShape = (NodeSuperShape *) node;
        superShape->vm(new SFFloat(1.26));
        superShape->vn1(new SFFloat(0.36));
        superShape->vn2(new SFFloat(1.2));
        superShape->vn3(new SFFloat(0.5));
        superShape->uTessellation(new SFInt32(16));
        superShape->vTessellation(new SFInt32(24));
    }
    m_scene->setSelection(node);
    m_scene->UpdateViews(NULL, UPDATE_SELECTION);
}

void
MainWindow::createInsectRear(int segments)
{
    createGeometryNode("SuperShape");
    Node *node = m_scene->getSelection()->getNode();
    if ((node) && (node->getType() == DUNE_SUPER_SHAPE)) {
        NodeSuperShape *superShape = (NodeSuperShape *) node;
        superShape->vm(new SFFloat(2 * segments));
        superShape->vn1(new SFFloat(-5));
    }
    m_scene->setSelection(node);
    m_scene->UpdateViews(NULL, UPDATE_SELECTION);
}

void
MainWindow::createTube(bool hornFlag)
{
    SuperExtrusionInfo info;
    info.numPoints = 5;
    createSuperExtrusion(&info);
    Node *node = m_scene->getSelection()->getNode();
    if ((node) && (node->getType() == DUNE_SUPER_EXTRUSION)) {
        NodeSuperExtrusion *superExtrusion = (NodeSuperExtrusion *) node;
        superExtrusion->beginCap(new SFBool(false));
        superExtrusion->endCap(new SFBool(false));
        superExtrusion->solid(new SFBool(false));
        if (hornFlag) {
            float *scales = new float[2 * 2];
            scales[0] = 1;
            scales[1] = 1;
            scales[2] = 0;
            scales[3] = 0;
            superExtrusion->scale(new MFVec2f(scales, 2 * 2));    
        }
    }
    m_scene->setSelection(node);
    m_scene->UpdateViews(NULL, UPDATE_SELECTION);
}

void
MainWindow::createTorus(void)
{
    SuperExtrusionInfo info;
    info.numPoints = 17;
    createSuperExtrusion(&info);
    int direction = info.direction;
    Node *node = m_scene->getSelection()->getNode();
    if ((node) && (node->getType() == DUNE_SUPER_EXTRUSION)) {
        NodeSuperExtrusion *superExtrusion = (NodeSuperExtrusion *) node;
        superExtrusion->beginCap(new SFBool(false));
        superExtrusion->endCap(new SFBool(false));
        int numPoints = superExtrusion->controlPoint()->getSFSize();
        float *ring = new float[numPoints * 3];
        float radius = 1.05;
        int x = 0;
        int y = 1;
        int z = 2;
        switch (direction) {
          case 0:
            z = 0;
            y = 2;
            x = 1;
            break;
          case 1:
            x = 2;
            y = 0;
            z = 1;
            break;
          case 2:
            x = 0;
            y = 1;
            z = 2;
            break;
        }
            
        for (int i = 0; i <= (numPoints - 1); i++) {
            
            ring[i * 3 + x] = radius * sin(i * 2.0f * M_PI / (numPoints - 1));
            ring[i * 3 + y] = radius * cos(i * 2.0f * M_PI / (numPoints - 1));;
            ring[i * 3 + z] = 0;
        }
        float maxY = 1;
        ring[x] = 0;
        ring[y] = maxY;
        ring[z] = 0;
        ring[(numPoints - 1) * 3 + x] = 0;
        ring[(numPoints - 1) * 3 + y] = maxY;
        ring[(numPoints - 1) * 3 + z] = 0;
        superExtrusion->a(new SFFloat(0.5));    
        superExtrusion->controlPoint(new MFVec3f(ring, numPoints * 3));    
    }
    m_scene->setSelection(node);
    m_scene->UpdateViews(NULL, UPDATE_SELECTION);
}

void
MainWindow::createText3D(void)
{
    Node *text = createGeometryNode("Text3D");
    m_scene->setSelection(text);
    changeOneText();
    Node *faceset = text->toIndexedFaceSet();
    m_scene->setSelection(m_scene->replaceNode(text, faceset));
    m_scene->UpdateViews(NULL, UPDATE_SELECTION);
}

static Matrix meshDataMatrix;
static MyArray<Vec3f> points;
static int numNodes;

static bool searchMeshDataOrTransform(Node *node, void *data)
{
    numNodes++;
    if (node->getType() == VRML_TRANSFORM) {
        NodeTransform *transform = (NodeTransform *)node;
        Matrix transformMatrix;
        transform->transform();
        transform->getMatrix(transformMatrix);
        meshDataMatrix = transformMatrix;
    } else if (node && node->isMeshBasedNode()) {
        MeshBasedNode *mesh = (MeshBasedNode *)node;
        if (mesh->getMesh() == NULL)
            return true;
        MFVec3f *verts = mesh->getMesh()->getVertices();
        for (int i = 0; i < verts->getSFSize(); i++) {
            points.append(meshDataMatrix * verts->getVec(i));
        }
    } else if (node && node->isChainBasedNode()) {
        ChainBasedNode *chain = (ChainBasedNode *)node;
        const Vec3f *verts = chain->getChain();
        for (int i = 0; i < chain->getChainLength(); i++) {
            points.append(meshDataMatrix * verts[i]);
        }
    } else if (node->getType() == VRML_INDEXED_LINE_SET) {
        NodeIndexedLineSet *set = (NodeIndexedLineSet *)node;
        if (set->coord() == NULL)
            return true;
        MFVec3f *verts = ((NodeCoordinate *)
                          (set->coord()->getValue()))->point();
        if (verts == NULL)
            return true;
        for (int i = 0; i < verts->getSFSize(); i++) {
            points.append(meshDataMatrix * verts->getVec(i));
        }
    } else if (node->getType() == X3D_LINE_SET) {
        NodeLineSet *set = (NodeLineSet *)node;
        if (set->coord() == NULL)
            return true;
        MFVec3f *verts = ((NodeCoordinate *)
                          (set->coord()->getValue()))->point();
        if (verts == NULL)
            return true;
        for (int i = 0; i < verts->getSFSize(); i++) {
            points.append(meshDataMatrix * verts->getVec(i));
        }
    } else if (node->getType() == VRML_POINT_SET) {
        NodePointSet *set = (NodePointSet *)node;
        if (set->coord() == NULL)
            return true;
        MFVec3f *verts = ((NodeCoordinate *)
                          (set->coord()->getValue()))->point();
        if (verts == NULL)
            return true;
        for (int i = 0; i < verts->getSFSize(); i++) {
            points.append(meshDataMatrix * verts->getVec(i));
        }
    }
    return true;     
}

#ifdef HAVE_LIBCGAL
void MainWindow::convexHull(void)
{
    meshDataMatrix = Matrix::identity();
    points.resize(0);
    numNodes = 0;
    Node *node = m_scene->getSelection()->getNode();
    bool isSelectionRoot = node == m_scene->getRoot();
    bool isCoordinate = false;
    if (node && (node->getType() == VRML_COORDINATE)) {
        node = node->getParent();
        isCoordinate = true;
    }
    MyArray<Vec3f> *collectedPoints = m_scene->getStore4Convex_hull();
    bool removeNode = false;
    if (collectedPoints->size() == 0) {
        removeNode = true;
        node->doWithBranch(searchMeshDataOrTransform, NULL);
    } else
        for (size_t i = 0; i < collectedPoints->size(); i++)
            points.append((*collectedPoints)[i]);
    if (node && points.size() > 3) {
        if ((numNodes == 1) || (isCoordinate && (numNodes == 2))) {
            m_scene->setSelection(node->getParent());
        }
        if (removeNode) {
            if (isSelectionRoot)
                m_scene->deleteAll();
            else
                m_scene->deleteLastSelection();
        }
        NodeIndexedFaceSet *faceSet = Util::convexHull(m_scene, points);
        faceSet->optimize();
        createGeometryNode(faceSet, false, false);
    }
    points.resize(0);
}
#endif

//static MyString onetext;

void MainWindow::createOneText(void)
{
    createGeometryNode("Text");
    changeOneText();
}

void MainWindow::changeOneText(void)
{
    Node *node = m_scene->getSelection()->getNode();
    if ((node) && (node->getType() == VRML_TEXT)) {
        NodeText *text = (NodeText *) node;
        MyString s = text->string()->getValue(0);
        OneTextDialog dlg(m_wnd, IDD_TEXT, s, textValidate);
        if (dlg.DoModal() == IDCANCEL)
            return;
        m_scene->setField(text, text->string_Field(), 
                        new MFString(dlg.GetValue()));
    }
    if ((node) && (node->getType() == KAMBI_TEXT_3D)) {
        NodeText3D *text = (NodeText3D *) node;
        MyString s = text->string()->getValue(0);
        OneTextDialog dlg(m_wnd, IDD_TEXT, s, textValidate);
        if (dlg.DoModal() == IDCANCEL)
            return;
        m_scene->setField(text, text->string_Field(), 
                        new MFString(dlg.GetValue()));
    }
    m_scene->setSelection(node);
    m_scene->UpdateViews(NULL, UPDATE_SELECTION);
}

void
MainWindow::moveSibling(int command)
{
    Node *node = m_scene->getSelection()->getNode();
    int newIndex = -1;
    int oldIndex = -1;
    switch (command) {
      case MOVE_SIBLING_UP:
        newIndex = node->getPrevSiblingIndex();
        oldIndex = newIndex + 1;
        break;
      case MOVE_SIBLING_DOWN:
        newIndex = node->getNextSiblingIndex();
        oldIndex = newIndex - 1;
        break;
      case MOVE_SIBLING_FIRST:
        newIndex = 0;
        oldIndex = node->getSiblingIndex() + 1;
        break;
      case MOVE_SIBLING_LAST:
        {
        MFNode *mfNode = (MFNode *)node->getParentFieldValue();
        newIndex = mfNode->getSize() - 1;
        oldIndex = node->getSiblingIndex();
        }
        break;
    }
    if (newIndex != -1) {
        node->ref();
        Node* parent = node->getParent();
        int parentField = node->getParentField();
        float oldSceneLength = 0;
        float newSceneLength = 0;
        NodeVrmlCut *vrmlCut = NULL;
        if (parent->getType() == DUNE_VRML_CUT) {
            vrmlCut = (NodeVrmlCut *)parent;
            oldSceneLength = vrmlCut->sceneLengths()->getValue(oldIndex);
            newSceneLength = vrmlCut->sceneLengths()->getValue(newIndex);
        }
        m_scene->execute(new MoveCommand(node, NULL, -1, 
                                         parent, parentField, newIndex));
        m_scene->execute(new MoveCommand(node, parent, parentField, 
                                         NULL, -1, oldIndex));
        if (parent->getType() == DUNE_VRML_CUT) {
            vrmlCut->sceneLengths()->setValue(newIndex, oldSceneLength);
            vrmlCut->sceneLengths()->setValue(oldIndex, newSceneLength);
        }
        MFNode *mfNode = (MFNode *)node->getParentFieldValue();
        m_scene->setSelection(mfNode->getValue(newIndex));
        m_scene->UpdateViews(NULL, UPDATE_SELECTION);
    }       
}

void
MainWindow::moveBranchTo(const char* nodeName, const char* fieldName)
{
    Node *current = m_scene->getSelection()->getNode();
    if (current->isInvalidChildNode())
        return;
    if (!current->isInScene(m_scene))
        return;
    Node *node = insertNode(nodeName);
    int field = node->getProto()->lookupField(fieldName);
    if (field == INVALID_INDEX)
        field = node->getProto()->lookupExposedField(fieldName);
    if (field == INVALID_INDEX) {
        TheApp->MessageBoxId(IDS_INTERNAL_ERROR);
        return;
    }
    moveBranchTo(node, field, current);
}

void 
MainWindow::moveBranchTo(Node* node, int field, Node* current)
{
    if (current == NULL)
        return;
    Node *target = current; 
    int targetField = -1;
    if (target == m_scene->getRoot())
        targetField = m_scene->getRootField();
    else {
        targetField = target->getParentField();
        target = target->getParent();
    }

    FieldValue  *value = target->getField(targetField);
    if (value->getType() == MFNODE) {
        MFNode *nodes = (MFNode *)value;
        bool inBranch = false;
        for (int i = 0 ; i < nodes->getSize() ; i++) {
            if (nodes->getValue(i)->isEqual(current))
                inBranch = true;
            if (inBranch)
                if (!nodes->getValue(i)->isEqual(node)) {
                    CommandList *list = new CommandList();
                    list->append(new MoveCommand(nodes->getValue(i), 
                                                 target, targetField,
                                                 node, field));
                    list->execute();
                }
        }
        m_scene->setSelection(target);
        m_scene->UpdateViews(NULL, UPDATE_SELECTION);
    }
}

void 
MainWindow::moveBranchToTransformSelection(void)
{
    Node *selection = m_scene->getSelection()->getNode(); 
    if (selection->getType() != VRML_TRANSFORM)
        return;
    NodeTransform *target = (NodeTransform *)selection;
    NodeTransform *transform = target;
    int childrenField = transform->children_Field();
    //to invert a Transform the following fields see
    // ISO/IEC 19775-1:2008 10.4.4 Transform
    NodeTransform *newTransform = NULL;
    CommandList *list = new CommandList();
    const float *center = transform->center()->getValue();
    if ((center[0] != 0) || (center[1] != 0) || (center[2] != 0)) {
        newTransform = (NodeTransform *)m_scene->createNode("Transform");
        newTransform->translation(new SFVec3f(center[0], 
                                              center[1],
                                              center[2]));
        list->append(new MoveCommand(newTransform, NULL, -1, 
                                     target, childrenField));
        target = newTransform;
    }
    const float *scaleOrientation = transform->scaleOrientation()->getValue();
    if (scaleOrientation[3] != 0) {
        newTransform = (NodeTransform *)m_scene->createNode("Transform");
        newTransform->rotation(new SFRotation(scaleOrientation[0], 
                                              scaleOrientation[1],
                                              scaleOrientation[2],
                                              scaleOrientation[3]));
        list->append(new MoveCommand(newTransform, NULL, -1, 
                                     target, childrenField));
        target = newTransform;
    }
    const float *scale = transform->scale()->getValue();
    if ((scale[0] != 1.0f) || (scale[1] != 1.0f) || (scale[2] != 1.0f)) {
        newTransform = (NodeTransform *)m_scene->createNode("Transform");
        newTransform->scale(new SFVec3f(scale[0] == 0 ? 1 : 1.0f / scale[0],
                                        scale[1] == 0 ? 1 : 1.0f / scale[1],
                                        scale[2] == 0 ? 1 : 1.0f / scale[2]));
        list->append(new MoveCommand(newTransform, NULL, -1, 
                                     target, childrenField));
        target = newTransform;
    }
    if (scaleOrientation[3] != 0) {
        newTransform = (NodeTransform *)m_scene->createNode("Transform");
        newTransform->rotation(new SFRotation(scaleOrientation[0], 
                                              scaleOrientation[1],
                                              scaleOrientation[2],
                                              -scaleOrientation[3]));
        list->append(new MoveCommand(newTransform, NULL, -1, 
                                     target, childrenField));
        target = newTransform;
    }
    const float *rotation = transform->rotation()->getValue();
    if (rotation[3] != 0) {
        newTransform = (NodeTransform *)m_scene->createNode("Transform");
        newTransform->rotation(new SFRotation(rotation[0], 
                                              rotation[1],
                                              rotation[2],
                                              -rotation[3]));
        list->append(new MoveCommand(newTransform, NULL, -1, 
                                     target, childrenField));
        target = newTransform;
    }
    if ((center[0] != 0) || (center[1] != 0) || (center[2] != 0)) {
        newTransform = (NodeTransform *)m_scene->createNode("Transform");
        newTransform->translation(new SFVec3f(-center[0], 
                                              -center[1],
                                              -center[2]));
        list->append(new MoveCommand(newTransform, NULL, -1, 
                                     target, childrenField));
        target = newTransform;
    }
    const float *translation = transform->translation()->getValue();
    if ((translation[0] != 0) || 
        (translation[1] != 0) || 
        (translation[2] != 0)) {
        newTransform = (NodeTransform *)m_scene->createNode("Transform");
        newTransform->translation(new SFVec3f(-translation[0], 
                                              -translation[1],
                                              -translation[2]));
        list->append(new MoveCommand(newTransform, NULL, -1, 
                                     target, childrenField));
        target = newTransform;
    }
    list->execute();
    moveBranchTo(target, childrenField, selection->getNextSibling());
    m_scene->setSelection(selection);
    m_scene->UpdateViews(NULL, UPDATE_SELECTION);
}

void 
MainWindow::moveBranchToInline()
{
    Node *current = m_scene->getSelection()->getNode();
    if (current->isInvalidChildNode())
        return;
    if (!current->isInScene(m_scene))
        return;
    char path[1024];
    path[0] = '\0';

    NodeArray allNodes;        
    doWithBranchUpdate(getAllNodes, &allNodes);

    NodeInline *newNode = (NodeInline *)insertNode("Inline");

    Node *target = current; 
    int targetField = -1;
    if (target == m_scene->getRoot())
        targetField = m_scene->getRootField();
    else {
        targetField = target->getParentField();
        target = target->getParent();
    }

    FieldValue  *value = target->getField(targetField);

    // build list of nodes in branch
    MyArray<Node *> nodesInBranch;
    if (value->getType() == MFNODE) {
        MFNode *nodes = (MFNode *)value;
        bool inBranch = false;
        for (int i = 0 ; i < nodes->getSize() ; i++) {
            if (nodes->getValue(i)->isEqual(current))
                inBranch = true;
            if (inBranch)
                if (!nodes->getValue(i)->isEqual(newNode))
                     nodesInBranch.append(nodes->getValue(i));
        }
    }

    for (size_t i = 0 ; i < allNodes.size() ; i++) {
        Node *node = allNodes[i];

        // search for eventIns point outside of rest of scenegraph branch
        int numEventIns = node->getProto()->getNumEventIns();
        for (int eIn = 0; eIn < numEventIns; eIn++) {
            SocketList::Iterator *j;
            for (j = node->getInput(eIn).first(); j != NULL; j = j->next())
                if ((j != NULL) && (j->item().getNode())) {
                   bool eventIsOutside = true;
                   for (size_t n = 0; n < allNodes.size(); n++) {
                       if (node != allNodes[n])
                           if (j->item().getNode()->isEqual(allNodes[n])) {
                               eventIsOutside = false;
                               break;
                           }
                   }
                   if (eventIsOutside) {
                       TheApp->MessageBox(IDS_TO_INLINE_EVENT_IN_OUTSIDE,
                                          (const char *)node->getName());
                       // cleanup Inline node
                       m_scene->execute(new MoveCommand(newNode,
                                                       target, targetField,
                                                       NULL, -1));
                       return;
                    }
                }
        }    

        // search for eventOuts point outside of rest of scenegraph branch
        int numEventOuts = node->getProto()->getNumEventOuts();
        for (int eOut = 0; eOut < numEventOuts; eOut++) {
            SocketList::Iterator *j;
            for (j = node->getOutput(eOut).first(); j != NULL; j = j->next())
                if ((j != NULL) && (j->item().getNode())) {
                    bool eventIsOutside = true;
                    for (size_t n = 0; n < allNodes.size(); n++) {
                        if (node != allNodes[n])
                            if (j->item().getNode()->isEqual(nodesInBranch[n])) {
                                eventIsOutside = false;
                                break;
                            }
                    }
                    if (eventIsOutside) {
                        TheApp->MessageBox(IDS_TO_INLINE_EVENT_OUT_OUTSIDE,
                                     (const char *)node->getName());
                        // cleanup Inline node
                        m_scene->execute(new MoveCommand(newNode,
                                                        target, targetField,
                                                        NULL, -1));
                        return;
                    }
                }
        }
        // search for USED nodes (not supported yet)
        if (node->getNumParents() > 1) {
            TheApp->MessageBox(IDS_TO_INLINE_USE, 
                               (const char *)node->getName());
            // cleanup Inline node
            m_scene->execute(new MoveCommand(newNode, target, targetField,
                                            NULL, -1));
            return;
        }    
    }

    if (TheApp->getFileDialogDir())
        while (chdir(TheApp->getFileDialogDir()) == -2);
    if (swSaveFileDialog(TheApp->mainWnd(), "Save As Inline",
#ifdef _WIN32
            "Inline VRML File (.wrl)\0*.wrl;*.WRL\0All Files (*.*)\0*.*\0\0",
#else
            "*.wrl",
#endif
            path, 1024,".wrl")) {
        int f = open(path, O_WRONLY | O_CREAT,00666);  
        if (f == -1) {
            TheApp->MessageBoxPerror(path);
            // cleanup Inline node
            m_scene->execute(new MoveCommand(newNode, target, targetField,
                                            NULL, -1));
            return;
        }
        if (mywritestr(f ,"#VRML V2.0 utf8\n\n") != 0) {
            TheApp->MessageBoxPerror(path);
            // cleanup Inline node
            m_scene->execute(new MoveCommand(newNode, target, targetField,
                                            NULL, -1));
            return;
        }

        // write to Inline file
        for (size_t i = 0 ; i < nodesInBranch.size() ; i++) 
            if (nodesInBranch[i]->write(f, TheApp->GetIndent()) != 0) {
                TheApp->MessageBoxPerror(path);                
                // cleanup Inline node
                m_scene->execute(new MoveCommand(newNode, target, targetField,
                                                NULL, -1));
                return;
            }

        bool writeError = false;
        if (m_scene->writeRouteStrings(f, 0, true) != 0)
            writeError = true;
        else if (swTruncateClose(f))
            writeError = true;
        if (writeError) {
            TheApp->MessageBoxPerror(path);
            // cleanup Inline node
            m_scene->execute(new MoveCommand(newNode, target, targetField,
                                            NULL, -1));
            return;
        }

        // remove nodes from scenegraph
        for (size_t i = 0 ; i < nodesInBranch.size() ; i++) {           
            CommandList *list = new CommandList();
            list->append(new MoveCommand(nodesInBranch[i], target, targetField,
                         NULL, -1));
            list->execute();
        }

        URL url;
        url.FromPath(path);
        newNode->url(new MFString(rewriteURL(url,url.GetDir(),
                    m_scene->getURL())));
        m_scene->readInline(newNode);
        m_scene->setSelection(target);
        m_scene->UpdateViews(NULL, UPDATE_SELECTION);
    } else {
        // cleanup Inline node
         m_scene->execute(new MoveCommand(newNode, target, targetField,
                                         NULL, -1));
    }
}

void                
MainWindow::moveBranchToParent(void) 
{
    Node *current = m_scene->getSelection()->getNode();
    if (current->isInvalidChildNode())
        return;
    if (!current->isInScene(m_scene))
        return;
    Node *target = current; 
    int targetField = -1;
    if (target == m_scene->getRoot())
        return;
    target = target->getParent();
    if (target == m_scene->getRoot())
        return;
    if (target->getParent() == m_scene->getRoot())
       targetField = m_scene->getRootField();
    else
        targetField = target->getParentField();
    target = target->getParent();
    moveBranchTo(target, targetField, current);
}

static Node *vrmlCut = NULL;

static bool getVrmlCut(Node *node, void *data)
{
    if (node && node->getType() == DUNE_VRML_CUT)
        vrmlCut = node;
    return true;     
}

static bool collectBranch(Node *node, void *data)
{
    NodeList *list = (NodeList *)data;
    if (node)
        list->append(node);
    return true;     
}

void 
MainWindow::moveBranchToVrmlScene(bool begin)
{
    Node *node = m_scene->getSelection()->getNode();
    Node *beforeSelection = node->getPrevSibling();
    int nodeParentIndex = node->getParentIndex(); 
    vrmlCut = NULL;
    m_scene->getRoot()->doWithBranch(getVrmlCut, NULL);
    Node *targetNode = vrmlCut;
    bool createVrmlCut = false;
    if (vrmlCut == NULL) {
        targetNode = m_scene->createNode("VrmlCut");
        MoveCommand *command = new MoveCommand(targetNode, NULL, -1, 
                                   m_scene->getRoot(), 
                                   m_scene->getRoot()->getChildrenField());
        command->execute();
        createVrmlCut = true;
    }
    m_scene->setSelection(vrmlCut);
    moveSiblingFirst();
    m_scene->UpdateViews(NULL, UPDATE_ALL, NULL);
    vrmlCut = NULL;
    m_scene->getRoot()->doWithBranch(getVrmlCut, NULL);
    NodeGroup *root = (NodeGroup *)m_scene->getRoot();
    if (node->getParent() == root) {
        for (size_t i = 0; i < root->children()->getSize(); i++) {
            Node *compare = root->children()->getValue(i);
            if ((compare == node) && 
                (compare->getParentIndex() == nodeParentIndex)) {
                node = compare;
                break;
            }
        }
    }
    NodeList nodeList;
    node->doWithNextSiblings(collectBranch, &nodeList);
    m_scene->getRoot()->doWithBranch(getVrmlCut, NULL);
    NodeVrmlCut *cut = (NodeVrmlCut *) vrmlCut;
    MFNode *cutScenes = cut->scenes();
    NodeVrmlScene *vscene = (NodeVrmlScene *)m_scene->createNode("VrmlScene");
        MoveCommand *command = NULL;
    if (createVrmlCut || !begin ) 
        command = new MoveCommand(vscene, NULL, -1, cut, cut->scenes_Field());
    else
        command = new MoveCommand(vscene, NULL, -1, cut, cut->scenes_Field(),
                                  0);
    command->execute();
    m_scene->UpdateViews(NULL, UPDATE_ALL, NULL);
    for (size_t i = 0; i < nodeList.size(); i++) {
        if (node->hasParent()) {
            MoveCommand *command = new MoveCommand(
                 nodeList.get((nodeList.size() - 1) - i), 
                 node->getParent(), node->getParentField(),
                 vscene, vscene->children_Field());
            command->execute();
            m_scene->UpdateViews(NULL, UPDATE_ALL, NULL);
        }        
    }
    m_scene->setSelection(beforeSelection);
    m_scene->UpdateViews(NULL, UPDATE_SELECTION, NULL);
}


void 
MainWindow::moveBranchToProto(void)
{
    Node *node = m_scene->getSelection()->getNode();
    MyString protoName = "";
    protoName += "PROTO1";
    int i = 0;
    while (m_scene->hasProtoName(protoName)) {
        i++;
        protoName = ""; 
        char buf[256] = "";
        mysnprintf(buf, 255, "%d", i);
        if (i > 256) {
            protoName += "_";
            break;
        }
        protoName += (char *)"PROTO";
        protoName += (char *)buf;          
    }
    ProtoNameDialog dlg(m_wnd, protoName.getData());
    if (dlg.DoModal() == IDCANCEL)
        return;
    m_scene->addProtoDefinition();
    Proto *proto = new Proto(m_scene, dlg.getName());
    m_scene->addToProtoDefinition(dlg.getName());
    m_scene->addProtoName(dlg.getName());
    m_scene->addProto(dlg.getName(), proto);
    NodeList nodeList;
    node->doWithNextSiblings(collectBranch, &nodeList);
    Node *targetNode = node;
    if (nodeList.size() > 1)
        targetNode = m_scene->createNode("Group");
    for (size_t i = 0; i < nodeList.size();i++) {        
        NodeGroup *group = (NodeGroup *)targetNode;
        if (nodeList.size() > 1) {
            MoveCommand *command = new MoveCommand(nodeList[i]->copy(), 
                                                   NULL, -1,
                                                   group, 
                                                   group->children_Field());
            command->execute();
        }
    }
    NodeList emptyNodeList;
    proto->define(targetNode, &emptyNodeList); 
    proto->setLoaded(true);
    for (int i = nodeList.size() - 1; i > -1; i--) {
        MoveCommand *command = new MoveCommand(nodeList[i], 
                                               node->getParent(), 
                                               node->getParentField(), 
                                               NULL, -1);
        command->execute();
    }        
    m_scene->setSelection(proto);
    m_graphView->Initialize();
    UpdateObjectEdit(m_scene->getSelection());
    EditObject();
    m_scene->setSelection(proto);
    ShowView(PW_TOP, m_graphView, ID_DUNE_VIEW_SCENE_GRAPH, "ShowRouteView");
    UpdateToolbars();
    m_scene->UpdateViews(NULL, UPDATE_SELECTION);
    m_scene->UpdateViews(NULL, UPDATE_ALL, NULL);
}

void
MainWindow::doWithBranchUpdate(DoWithNodeCallback callback, void *data)
{
    Node *node = m_scene->getSelection()->getNode(); 
    doWithBranch(callback, data);
    m_scene->UpdateViews(NULL, UPDATE_ALL, NULL);
    if (node->isInScene(m_scene)) {
        m_scene->setSelection(node);
        m_scene->UpdateViews(NULL, UPDATE_SELECTION);
    }
}

void
MainWindow::doWithBranch(DoWithNodeCallback callback, void *data)
{
    Node *node = m_scene->getSelection()->getNode(); 
    if (node != NULL)
        node->doWithBranch(callback, data);    
}

void
MainWindow::branchConvertToTriangleSetUpdate(void)
{
    Node *node = m_scene->getSelection()->getNode(); 
    m_scene->branchConvertToTriangleSet(node);
    m_scene->UpdateViews(NULL, UPDATE_ALL, NULL);
    if (node->isInScene(m_scene)) {
        m_scene->setSelection(node);
        m_scene->UpdateViews(NULL, UPDATE_SELECTION);
    }
}

void
MainWindow::branchConvertToIndexedFaceSetUpdate(void)
{
    Node *node = m_scene->getSelection()->getNode(); 
    m_scene->branchConvertToIndexedFaceSet(node);
    m_scene->UpdateViews(NULL, UPDATE_ALL, NULL);
    if (node->isInScene(m_scene)) {
        m_scene->setSelection(node);
        m_scene->UpdateViews(NULL, UPDATE_SELECTION);
    }
}

void
MainWindow::branchConvertToIndexedTriangleSetUpdate(void)
{
    Node *node = m_scene->getSelection()->getNode(); 
    m_scene->branchConvertToIndexedTriangleSet(node);
    m_scene->UpdateViews(NULL, UPDATE_ALL, NULL);
    if (node->isInScene(m_scene)) {
        m_scene->setSelection(node);
        m_scene->UpdateViews(NULL, UPDATE_SELECTION);
    }
}

static bool flipSide(Node *node, void *data)
{
    // avoid double flipping of flipable node and its Coordinate node
    if (node->getNodeClass() != COORDINATE_NODE)
        node->flipSide();
    return true;     
}

void
MainWindow::setBranchFlipSide(void)
{
    Node *node = m_scene->getSelection()->getNode(); 
    node->doWithBranch(flipSide, NULL);
    m_scene->UpdateViews(NULL, UPDATE_SELECTION);
}

static bool setSolid(Node *node, void *data)
{
    bool *isSolid = (bool *)data;
    if (node->getSolidField() != -1) {
        Scene *scene = node->getScene();
        scene->setField(node, node->getSolidField(), new SFBool(*isSolid));
    }
    return true;     
}

static bool setConvex(Node *node, void *data)
{
    bool *isConvex = (bool *)data;  
    if (node->getConvexField() != -1) {
        Scene *scene = node->getScene();
        scene->setField(node, node->getConvexField(), new SFBool(*isConvex));
    }
    return true;     
}

static bool trueFalseValidate(MyString text)
{    
    if (strcmp(text,"TRUE") == 0)
        return true;
    if (strcmp(text,"true") == 0)
        return true;
    if (strcmp(text,"FALSE") == 0)
        return true;
    if (strcmp(text,"false") == 0)
        return true;
    TheApp->MessageBoxId(IDS_TRUE_OR_FALSE);
    return false;
}

void
MainWindow::setBranchSolid(void)
{
    Node *node = m_scene->getSelection()->getNode(); 
    if (node != NULL) {
        MyString s = "";
        OneTextDialog dlg(m_wnd, IDD_SOLID, s, trueFalseValidate);
        if (dlg.DoModal() == IDCANCEL)
            return;
        bool bsolid = false;
        if ((strcmp(dlg.GetValue(), "TRUE") == 0) || 
            (strcmp(dlg.GetValue(), "true") == 0))
            bsolid = true;
        node->doWithBranch(setSolid, &bsolid);
        m_scene->UpdateViews(NULL, UPDATE_SELECTION);
    }
}

void
MainWindow::setBranchConvex(void)
{
    Node *node = m_scene->getSelection()->getNode(); 
    if (node != NULL) {
        MyString s = "";
        OneTextDialog dlg(m_wnd, IDD_CONVEX, s, trueFalseValidate);
        if (dlg.DoModal() == IDCANCEL)
            return;
        bool bconvex = false;
        if (strcasecmp(dlg.GetValue(), "TRUE") == 0)
            bconvex = true;
        node->doWithBranch(setConvex, &bconvex);
        m_scene->UpdateViews(NULL, UPDATE_SELECTION);
    }
}

void
MainWindow::setBranchTwoSided(void)
{
    Node *node = m_scene->getSelection()->getNode(); 
    if (node != NULL) {
        TwoRadioButtonsDialog dlg(m_wnd, IDD_TWO_SIDED, true);
        if (dlg.DoModal() == IDCANCEL)
            return;
        bool bsolid = dlg.GetFirstValue();
        node->doWithBranch(setSolid, &bsolid);
        m_scene->UpdateViews(NULL, UPDATE_SELECTION);
    }
}

static bool setCreaseAngle(Node *node, void *data)
{
    float *angle = (float *)data;
    if (node->getType() == VRML_INDEXED_FACE_SET)
        ((NodeIndexedFaceSet *) node)->creaseAngle(new SFFloat(*angle));
    else if (node->getType() == VRML_ELEVATION_GRID)
        ((NodeElevationGrid *) node)->creaseAngle(new SFFloat(*angle));
    else if (node->getType() == DUNE_SUPER_ELLIPSOID)
        ((NodeSuperEllipsoid *) node)->creaseAngle(new SFFloat(*angle));
    else if (node->getType() == DUNE_SUPER_EXTRUSION)
        ((NodeSuperExtrusion *) node)->creaseAngle(new SFFloat(*angle));
    else if (node->getType() == DUNE_SUPER_SHAPE)
        ((NodeSuperShape *) node)->creaseAngle(new SFFloat(*angle));
    else if (node->getType() == DUNE_SUPER_REVOLVER)
        ((NodeSuperRevolver *) node)->creaseAngle(new SFFloat(*angle));
    return true;     
}

void
MainWindow::setBranchCreaseAngle(void)
{
    Node *node = m_scene->getSelection()->getNode(); 
    if (node != NULL) {
        OneFloatDialog dlg(m_wnd, IDD_CREASE_ANGLE, 1.57, 0, M_PI);
        if (dlg.DoModal() == IDCANCEL)
            return;
        float angle = dlg.GetValue();
        node->doWithBranch(setCreaseAngle, &angle);
        m_scene->UpdateViews(NULL, UPDATE_SELECTION);
    }
}

static bool setTransparency(Node *node, void *data)
{
    float *ftransparency = (float *)data;
    if (node->getType() == VRML_MATERIAL)
        ((NodeMaterial *) node)->transparency(new SFFloat(*ftransparency));
    return true;     
}

void
MainWindow::setBranchTransparency(void)
{
    Node *node = m_scene->getSelection()->getNode(); 
    if (node != NULL) {
        OneFloatDialog dlg(m_wnd, IDD_TRANSPARENCY, 0, 0, 1);
        if (dlg.DoModal() == IDCANCEL)
            return;
        float transparency = dlg.GetValue();
        node->doWithBranch(setTransparency, &transparency);
        m_scene->UpdateViews(NULL, UPDATE_SELECTION);
    }
}

static bool setShininess(Node *node, void *data)
{
    float *fshininess = (float *)data;
    if (node->getType() == VRML_MATERIAL)
        ((NodeMaterial *) node)->shininess(new SFFloat(*fshininess));
    return true;     
}

void
MainWindow::setBranchShininess(void)
{
    Node *node = m_scene->getSelection()->getNode(); 
    if (node != NULL) {
        OneFloatDialog dlg(m_wnd, IDD_SHININESS, 0, 0, 1);
        if (dlg.DoModal() == IDCANCEL)
            return;
        float shininess = dlg.GetValue();
        node->doWithBranch(setShininess, &shininess);
        m_scene->UpdateViews(NULL, UPDATE_SELECTION);
    }
}

static bool removeNodeCallback(Node *node, void *data)
{
    char *nodeName = (char *)data;
    bool x3d = node->getScene()->isX3d();
    if (strcmp(nodeName, node->getProto()->getName(x3d)) == 0)
        if (node->hasParent()) {
            Node *parent = node->getParent();
            int parentField = node->getParentField();
            MoveCommand *command = new MoveCommand(node, parent, parentField, 
                                                   NULL, -1);
           command->execute();
        }
    return true;     
}

static bool nodeNameValidate(MyString text)
{
    return true;
}

void
MainWindow::removeBranchNode(void)
{
    MyString s = "";
    OneTextDialog dlg(m_wnd, IDD_TEXT, s, nodeNameValidate);
    if (dlg.DoModal() == IDCANCEL)
        return;
    const char *nodeName = dlg.GetValue();
    doWithBranch(removeNodeCallback, (void *)nodeName);
    m_scene->UpdateViews(NULL, UPDATE_REDRAW, NULL);    
}

bool
MainWindow::matchFieldPipeFilterNode(Node *node) 
{
    const char *filterNode = m_fieldPipeFilterNode;
    if (node->hasName())
        if (strstr(node->getName(), filterNode) != NULL)
            return true;
    if (strstr(node->getProto()->getName(m_scene->isX3d()), filterNode) != NULL)
        return true;
    return false;
}

bool
MainWindow::matchFieldPipeFilterField(Node *node) 
{
    const char *fieldPart = m_fieldPipeFilterField;
    if (strlen(fieldPart) > 0) {
        for (int i = 0; i < node->getProto()->getNumFields(); i++) {
            bool x3d = m_scene->isX3d();
            if (strstr(node->getProto()->getField(i)->getName(x3d), 
                       fieldPart) != NULL)
                return fieldPipe(node, i, m_fieldPipeCommand);
        }
    }
    return true;
}

static bool fieldPipeCallback(Node *node, void *data)
{
    MainWindow *wnd = (MainWindow *)data;
    if (wnd->matchFieldPipeFilterNode(node))
        return wnd->matchFieldPipeFilterField(node);
    return true;     
}

void
MainWindow::branchFieldPipe(void) {
    Node *node = m_scene->getSelection()->getNode(); 
    if (node != NULL) {
        FieldPipeDialog dlg(m_wnd, m_fieldPipeFilterNode, 
                            m_fieldPipeFilterField, m_fieldPipeCommand);
        if (dlg.DoModal() == IDCANCEL)
            return;
        m_fieldPipeCommand = dlg.GetCommand();
        m_fieldPipeFilterNode = dlg.GetNodeFilter();
        m_fieldPipeFilterField = dlg.GetFieldFilter();
        node->doWithBranch(fieldPipeCallback, this);
        m_scene->UpdateViews(NULL, UPDATE_SELECTION);
    }
}

static bool buildRigidBodyCollection(Node *node, void *data)
{
    // collect SollidableShape and CollidableOffset nodes
    NodeList *collidables = (NodeList *)data;
    if ((node->getType() == X3D_COLLIDABLE_OFFSET) || 
        (node->getType() == X3D_COLLIDABLE_SHAPE))
        collidables->append(node);
    return true;     
}

void
MainWindow::branchBuildRigidBodyCollection(void) {
    m_scene->setSelection(m_scene->getRoot());
    Node *node = m_scene->getSelection()->getNode(); 
    if (node != NULL) {
        NodeList collidables;
        node->doWithBranch(buildRigidBodyCollection, &collidables);
        if (collidables.size() > 0) {
            NodeRigidBodyCollection *collection = (NodeRigidBodyCollection *) 
                  m_scene->createNode("RigidBodyCollection");
            for (size_t i = 0; i < collidables.size(); i++) {
                NodeRigidBody *rigidBody = (NodeRigidBody*) 
                                           m_scene->createNode("RigidBody");
                m_scene->execute(new MoveCommand(collidables[i], NULL, -1,
                                                 rigidBody, 
                                                 rigidBody->geometry_Field()));
                m_scene->execute(new MoveCommand(rigidBody, NULL, -1, 
                                                 collection, 
                                                 collection->bodies_Field()));
            }
            NodeCollisionCollection *collision = (NodeCollisionCollection *) 
                  m_scene->createNode("CollisionCollection");
            for (size_t i = 0; i < collidables.size(); i++) {
                m_scene->execute(new MoveCommand(collidables[i], NULL, -1,
                                                collision, 
                                                collision->collidables_Field()));
            }
            m_scene->execute(new MoveCommand(collision, NULL, -1, collection, 
                                            collection->collider_Field()));
            NodeGroup *root = (NodeGroup *)m_scene->getRoot();
            m_scene->execute(new MoveCommand(collection, NULL, -1, 
                                            root, root->children_Field()));
            NodeCollisionSensor *sensor = (NodeCollisionSensor*) 
                                          m_scene->createNode("CollisionSensor");
            m_scene->execute(new MoveCommand(sensor, NULL, -1, 
                                            root, root->children_Field()));
            m_scene->execute(new MoveCommand(collision, NULL, -1, sensor, 
                                            sensor->collider_Field()));
            int eventOut = sensor->getProto()->lookupEventOut("contacts");
            int eventIn = collection->getProto()->lookupEventIn("set_contacts");
            m_scene->addRoute(sensor, eventOut, collection, eventIn);
            for (size_t i = 0; i < collidables.size(); i++)
                collidables[i]->update();
        }
        m_scene->UpdateViews(NULL, UPDATE_ALL);
    }
}

static bool optimize(Node *node, void *data)
{
    if (node->getType() == VRML_INDEXED_FACE_SET) {
        Scene *scene = (Scene *)data;
        NodeIndexedFaceSet *faceSet = (NodeIndexedFaceSet *) node;
        bool ccw = faceSet->ccw()->getValue();
        bool solid = faceSet->solid()->getValue();
        bool convex = faceSet->convex()->getValue();
        float creaseAngle = faceSet->creaseAngle()->getValue();
        MFVec3f *mfVertices = NULL;
        MFInt32 *mfCoordIndex = NULL;
        faceSet->optimizeMesh(&mfVertices, &mfCoordIndex);
        if (mfVertices && mfCoordIndex && node->hasParent()) {
            Node *parent = node->getParent();
            int parentField = node->getParentField();
            NodeIndexedFaceSet *newFaceSet = (NodeIndexedFaceSet *) 
                                             scene->createNode("IndexedFaceSet"
                                                              );
            NodeCoordinate *coord = (NodeCoordinate *)
                                    scene->createNode("Coordinate");
            newFaceSet->coord(new SFNode(coord));
            coord->point(new MFVec3f(*mfVertices));

            newFaceSet->coordIndex(new MFInt32(*mfCoordIndex));
            newFaceSet->ccw(new SFBool(ccw));
            newFaceSet->solid(new SFBool(solid));
            newFaceSet->convex(new SFBool(convex));
            newFaceSet->creaseAngle(new SFFloat(creaseAngle));            

            MoveCommand *command = new MoveCommand(node, 
                                                   parent, parentField,
                                                   NULL, -1);
            command->execute();
            MoveCommand *command2 = new MoveCommand(newFaceSet, 
                                                    NULL, -1,
                                                    parent, parentField);
            command2->execute();
        }
    }
    return true;     
}

void
MainWindow::subdivide(void)
{
    Node *node = m_scene->getSelection()->getNode(); 
    if (node->getType() == VRML_COORDINATE) {
        node = node->getParent();
        m_scene->setSelection(node);
    }
    if (node->isMeshBasedNode()) {
        MeshBasedNode *mesh = (MeshBasedNode *)node;
        bool ccwResult = false;
        switch (node->getType()) {
          case VRML_CONE:
          case VRML_CYLINDER:
            ccwResult = true;
            break;
        }
        NodeIndexedFaceSet *faceset = mesh->subdivide();
        if (faceset == NULL)
            return;
        m_scene->setSelection(m_scene->replaceNode(node, faceset));
        if (ccwResult)
            faceset->ccw(new SFBool(false));
        optimizeSet();
        m_scene->UpdateViews(NULL, UPDATE_SELECTION);
    }
}

void
MainWindow::splitFaces(void) {
    Node *node = m_scene->getSelection()->getNode(); 
    if (node->getType() == VRML_COORDINATE) {
        node = node->getParent();
        m_scene->setSelection(node);
    }
    if (node->getType() == VRML_INDEXED_FACE_SET) {
        NodeIndexedFaceSet *faceset = ((NodeIndexedFaceSet *)node)
                                      ->splitSelectedFaces();
        if (faceset == NULL)
            return;
        faceset->optimize();
        m_scene->setSelection(m_scene->replaceNode(node, faceset));
        m_scene->UpdateViews(NULL, UPDATE_SELECTION);
    }
}

void
MainWindow::insetFace() {
    Node *node = m_scene->getSelection()->getNode(); 
    bool updateCoord = false;
    if (node->getType() == VRML_COORDINATE) {
        node = node->getParent();
        updateCoord = true;
    }
    if ((node->getType() == VRML_INDEXED_FACE_SET) && 
        (m_scene->getSelectionMode() == SELECTION_MODE_FACES)){
        SliderFloatDialog dlg(m_wnd, IDD_INSET, 0.5, 0, FLT_MAX);
        if (dlg.DoModal() == IDCANCEL)
            return;
        NodeIndexedFaceSet *faceset  = (NodeIndexedFaceSet *)node;
        faceset->insetFace(dlg.GetValue());
        m_scene->UpdateViews(NULL, UPDATE_ALL);        
        if (updateCoord)
            m_scene->setSelection(faceset->coord()->getValue()->getPath());
        else
            m_scene->setSelection(node->getPath());
        m_scene->UpdateViews(NULL, UPDATE_SELECTION);
    }
}

void
MainWindow::extrude() {
    Node *node = m_scene->getSelection()->getNode(); 
    if (node->getType() == VRML_COORDINATE) {
        node = node->getParent();
        m_scene->setSelection(node);
    }
    if (node->getType() == VRML_INDEXED_FACE_SET) {
        ((NodeIndexedFaceSet *)node)->extrudeFaces(
            TheApp->getExtrusionAmount());
    }
    m_scene->UpdateViews(NULL, UPDATE_SELECTION);
}
 
void
MainWindow::buildQuad()
{
    Node *node = m_scene->getSelection()->getNode();
    if (node->getType() == VRML_COORDINATE)
        node = node->getParent();   
    if (node->getType() == VRML_INDEXED_FACE_SET) {
        NodeIndexedFaceSet *face = (NodeIndexedFaceSet *)node;
        char str[256], title[256];
        swLoadString(IDS_DUNE, title, 256);
        swLoadString(IDS_SELECT_TWO_TRIANGLE, str, 256);
        if ((m_scene->getSelectionMode() != SELECTION_MODE_FACES) ||
            (m_scene->getSelectedHandlesSize() != 2)) {
            swMessageBox(TheApp->mainWnd(), str, title, SW_MB_OK, SW_MB_ERROR);
            return;
        }
        if (!face->buildQuad())
            swMessageBox(TheApp->mainWnd(), str, title, SW_MB_OK, SW_MB_ERROR);
    }
}
 
void
MainWindow::snapTogether()
{
    Node *node = m_scene->getSelection()->getNode();
    if (node->getType() == VRML_COORDINATE)
        node = node->getParent();   
    if (node->getType() == VRML_INDEXED_FACE_SET) {
        NodeIndexedFaceSet *face = (NodeIndexedFaceSet *)node;
        face->snapTogether();
    }
}

#ifdef HAVE_LIBCGAL

static Node *docsg(int operation, Node *node_1, Node*node_2, Scene *scene,
                   Matrix *searchMeshData1 = NULL,
                   Matrix *searchMeshData2 = NULL)
{
    searchIdxNode = NULL;
    searchMeshData = Matrix::identity();
    node_1->doWithBranch(searchNodes, NULL, false);
    if (searchMeshData1)
        searchMeshData = *searchMeshData1;
    Matrix matrix1 = searchMeshData;
    Node *node1 = searchIdxNode;  

    searchIdxNode = NULL;
    searchMeshData = Matrix::identity();
    node_2->doWithBranch(searchNodes, NULL, false);
    if (searchMeshData2)
        searchMeshData = *searchMeshData2;
    Matrix matrix2 = searchMeshData;
    Node *node2 = searchIdxNode;  

    if (node1 && node1->isMeshBasedNode() &&
        node2 && node2->isMeshBasedNode()) {

        scene->backupFieldsStart();
        NodeIndexedFaceSet *face1 = (NodeIndexedFaceSet *)
                                    node1->toIndexedFaceSet();
        MyMesh *mesh = face1->triangulateMesh();
        MFInt32 *mfcoordIndex = mesh->getCoordIndex();
        if (mfcoordIndex) {
            mfcoordIndex = (MFInt32 *) mfcoordIndex->copy();
            scene->backupFieldsAppend(face1, face1->coordIndex_Field());
            scene->setField(face1, face1->coordIndex_Field(),
                            new MFInt32(*mfcoordIndex));
        }
        NodeCoordinate *coord = (NodeCoordinate *)face1->coord()->getValue();
        MFVec3f *mfcoord = mesh->getVertices();
        if (coord && mfcoord) {
            mfcoord = (MFVec3f *) mfcoord;
            scene->backupFieldsAppend(coord, coord->point_Field());
            scene->setField(coord, coord->point_Field(), new MFVec3f(*mfcoord));
        }
        MFVec3f *mfVertices = NULL;
        MFInt32 *mfCoordIndex = NULL;
        face1->optimizeMesh(&mfVertices, &mfCoordIndex);
        NodeCoordinate *coord1 = (NodeCoordinate *) 
                                 scene->createNode("Coordinate");
        if (coord1 && mfVertices && mfCoordIndex) {
            scene->backupFieldsAppend(coord1, coord1->point_Field());
            coord1->point(new MFVec3f(*mfVertices));
            face1->coord(new SFNode(coord1));
            scene->backupFieldsAppend(face1, 
                                        face1->coordIndex_Field());
            face1->coordIndex(new MFInt32(*mfCoordIndex));
        }

        NodeIndexedFaceSet *face2 = (NodeIndexedFaceSet *)
                                    node2->toIndexedFaceSet();
        mesh = face2->triangulateMesh();
        mfcoordIndex = mesh->getCoordIndex();
        if (mfcoordIndex) {
            mfcoordIndex = (MFInt32 *) mfcoordIndex->copy();
            scene->backupFieldsAppend(face2, face2->coordIndex_Field());
            scene->setField(face2, face2->coordIndex_Field(),
                            new MFInt32(*mfcoordIndex));
        }
        coord = (NodeCoordinate *)face2->coord()->getValue();
        mfcoord = mesh->getVertices();
        if (coord && mfcoord) {
            mfcoord = (MFVec3f *) mfcoord;
            scene->backupFieldsAppend(coord, coord->point_Field());
            scene->setField(coord, coord->point_Field(), 
                            new MFVec3f(*mfcoord));
        }
        mfVertices = NULL;
        mfCoordIndex = NULL;
        face2->optimizeMesh(&mfVertices, &mfCoordIndex);
        NodeCoordinate *coord2 = (NodeCoordinate *) 
                                 scene->createNode("Coordinate");
        if (coord2 && mfVertices && mfCoordIndex) {
            scene->backupFieldsAppend(coord2, coord2->point_Field());
            coord2->point(new MFVec3f(*mfVertices));
            face2->coord(new SFNode(coord2));
            scene->backupFieldsAppend(face2, face2->coordIndex_Field());
            face2->coordIndex(new MFInt32(*mfCoordIndex));
        }

        scene->backupFieldsDone();

        NodeIndexedFaceSet *face = face1->csg(face2, operation, 
                                              matrix1, matrix2);
        if (face == NULL)
            return NULL;          
        face->solid(new SFBool(false));
        return face;
     }
     return NULL;
}

Node *unionGeometry = NULL;
Matrix unionMeshData1;
Matrix unionMeshData2;
static bool csgUnion(Node *node, void *data)
{
    Scene *scene = (Scene *)data;
    if (node->getType() == VRML_TRANSFORM) {
        NodeTransform *transform = (NodeTransform *)node;
        Matrix transformMatrix;

        glPushMatrix();
        glLoadIdentity();
        scene->transform(transform->getPath());
        glGetFloatv(GL_MODELVIEW_MATRIX, transformMatrix);
        glPopMatrix();
        if (unionGeometry == NULL)
            unionMeshData1 = transformMatrix;
        else
            unionMeshData2 = transformMatrix;
    } 
    if (node->isMeshBasedNode()) {
        bool success = true;
        if (unionGeometry == NULL)
            unionGeometry = node;
        else {
            Node *face = docsg(UNION, unionGeometry->copy(), node, scene, 
                               &unionMeshData1, &unionMeshData2);
            unionMeshData1 = Matrix::identity();
            if (face)
                unionGeometry = face;
            else
                success = false;  
        }
        if (success) {
            if (node->hasParent()) {
                Node *parent = node->getParent();
                if (parent->getType() == VRML_SHAPE) {
                    MoveCommand *command = new MoveCommand(parent, 
                                               parent->getParent(),
                                               parent->getParentField(), 
                                               NULL, -1);
                    command->execute();
                } 
            }
        }
    }
    return true;     
}

void                
MainWindow::branchCSGUnion(void)
{
    Node *node = m_scene->getSelection()->getNode();
    unionGeometry = NULL;
    unionMeshData1 = Matrix::identity();
    unionMeshData2 = Matrix::identity();

    node->doWithBranch(csgUnion, m_scene);
    if (unionGeometry != NULL) {
        Node *newNode = createGeometryNode(unionGeometry, false, false);
        m_scene->UpdateViews(NULL, UPDATE_ALL);
        m_scene->setSelection(newNode);
        m_scene->UpdateViews(NULL, UPDATE_SELECTION);
    }
}

void
MainWindow::csg(int operation)
{
     Node *node = m_scene->getSelection()->getNode();
     Node *parent = node->getParent();
     if (parent && node->getType() == VRML_GROUP) {
         int parentField = node->getParentField();
         NodeGroup *group = (NodeGroup *)node;
         if (group->children()->getSize() == 2) {        
             Node *face = docsg(operation,  group->children()->getValue(0),  
                                            group->children()->getValue(1),
                                            m_scene);
             if (face) {
                 m_scene->setSelection(parent);
                 Node *newNode = createGeometryNode(face, false, false);
 
                 MoveCommand *command = new MoveCommand(node, 
                                                        parent, parentField,
                                                        NULL, -1);
                 command->execute();
                 m_scene->setSelection(newNode);
                 m_scene->UpdateViews(NULL, UPDATE_SELECTION);
            }
        }
    }
}
#endif

void
MainWindow::simpleJoin()
{
    Node *node = m_scene->getSelection()->getNode();
    Node *parent = node->getParent();
    if (parent && node->getType() == VRML_GROUP) {
        MyArray<FacesetAndMatrix> facesets;
        int parentField = node->getParentField();
        NodeGroup *group = (NodeGroup *)node;
        for (int i = 0; i < group->children()->getSize(); i++) {        
            searchIdxNode = NULL;
            searchMaterialNode = NULL;
            searchMeshData = Matrix::identity();
            group->children()->getValue(i)->doWithBranch(searchNodes, NULL, 
                                                         false);
            Matrix matrix = searchMeshData;
            Node *node = searchIdxNode;  

            if (node && node->isMeshBasedNode()) {
                NodeIndexedFaceSet *face = NULL;
                if (node->getType() == VRML_INDEXED_FACE_SET)
                    face = (NodeIndexedFaceSet *) node;
                else
                    face = (NodeIndexedFaceSet *) node->toIndexedFaceSet();
                FacesetAndMatrix faceAndMatrix;
                faceAndMatrix.node = face;
                faceAndMatrix.material = (NodeMaterial *)searchMaterialNode;
                faceAndMatrix.matrix = matrix;

                facesets.append(faceAndMatrix);
           }
        }
        NodeIndexedFaceSet *face = (NodeIndexedFaceSet *)
                                   NodeIndexedFaceSet::simpleJoin(facesets);

        if (face == NULL)
            return;          
        face->solid(new SFBool(false));

        m_scene->setSelection(parent);
        Node *newNode = createGeometryNode(face, false, false);

        MoveCommand *command = new MoveCommand(node, parent, parentField,
                                                     NULL, -1);
        command->execute();
        m_scene->setSelection(newNode);
        m_scene->UpdateViews(NULL, UPDATE_SELECTION);
    }
}

void
MainWindow::splitIntoPieces(void) 
{
    Node *node = m_scene->getSelection()->getNode(); 
    if (node->getType() == VRML_COORDINATE) {
        node = node->getParent();
        m_scene->setSelection(node);
    }
    if (node->getType() == VRML_INDEXED_FACE_SET) {
        NodeIndexedFaceSet *face = (NodeIndexedFaceSet *)node;
        int piecesU = -1;
        int piecesV = -1;
        if (face->canSplitFaces()) {
            SplitFacesDialog dlg(m_wnd);
            if (dlg.DoModal() == IDCANCEL)
                return;
            piecesU = dlg.getPiecesU();
            piecesV = dlg.getPiecesV();
        }   
        face->splitIntoPieces(piecesU, piecesV);
    }
    m_scene->UpdateViews(NULL, UPDATE_SELECTION);
}

#ifdef HAVE_LIBVCG
void
MainWindow::meshReduce(void) {
    Node *node = m_scene->getSelection()->getNode(); 
    if (node->getType() == VRML_COORDINATE) {
        node = node->getParent();
    }
    if (node->getType() == VRML_INDEXED_FACE_SET) {
        NodeIndexedFaceSet *face = (NodeIndexedFaceSet *)node;
        OneFloatDialog dlg(m_wnd, IDD_MESH_REDUCE, 50);
        if (dlg.DoModal() == IDCANCEL)
            return;
        NodeIndexedFaceSet* faceSet = face->meshReduce(dlg.GetValue());
        char mesg[256];
        mysnprintf(mesg, 255, "polygons before: %d  now: %d",
                   face->getMesh()->getNumFaces(), 
                   faceSet->getMesh()->getNumFaces());
        if (faceSet != NULL) {            
            m_scene->setSelection(m_scene->replaceNode(face, faceSet));
            m_scene->UpdateViews(NULL, UPDATE_SELECTION);
        }
        setStatusText(mesg);
    }
}
#endif

void
MainWindow::branchOptimize(void) {
    Node *node = m_scene->getSelection()->getNode(); 
    if (node != NULL) {
        node->doWithBranch(optimize, m_scene);
        m_scene->UpdateViews(NULL, UPDATE_SELECTION);
    }
}

void
MainWindow::HandleScale(void)
{
    Node *node = m_scene->getSelection()->getNode(); 
    if (node != NULL) {
        OneFloatDialog dlg(m_wnd, IDD_HANDLE_SCALE, 1, 0);
        if (dlg.DoModal() == IDCANCEL)
            return;
        TheApp->SetPreferenceHandleScale(dlg.GetValue());
        m_scene->UpdateViews(NULL, UPDATE_REDRAW_3D, NULL);
    }
}

void
MainWindow::HandleSize(void)
{
    Node *node = m_scene->getSelection()->getNode(); 
    if (node != NULL) {
        OneFloatDialog dlg(m_wnd, IDD_HANDLE_SIZE, 3, 0);
        if (dlg.DoModal() == IDCANCEL)
            return;
        TheApp->SetPreferenceHandleSize(dlg.GetValue());
        m_scene->UpdateViews(NULL, UPDATE_REDRAW_3D, NULL);
    }
}

void
MainWindow::HandleEpsilon(void)
{
    Node *node = m_scene->getSelection()->getNode(); 
    if (node != NULL) {
        OneFloatDialog dlg(m_wnd, IDD_HANDLE_EPSILON, TheApp->GetHandleEpsilon(),
                           0);
        if (dlg.DoModal() == IDCANCEL)
            return;
        TheApp->SetHandleEpsilon(dlg.GetValue());
        m_scene->UpdateViews(NULL, UPDATE_REDRAW_3D, NULL);
    }
}

static bool createTexture(Node *node, void *data)
{
    if ((node->getType() == VRML_APPEARANCE) ||
        (node->getType() == KAMBI_KAMBI_APPEARANCE)) {
        NodeAppearance *appearanceNode = (NodeAppearance *) node;
        // ignore Nodes with already set Texture field
        Node *texture = (Node *)data;
        if ((appearanceNode->texture()->getValue() == NULL) &&
            texture->matchNodeClass(TEXTURE_NODE)) {
            MoveCommand *command = new MoveCommand(texture, NULL, -1,
                  appearanceNode, appearanceNode->texture_Field());
            command->execute();
        }
    }
    return true;     
}

void
MainWindow::createBranchImageTexture(void)
{
    char buf[MY_MAX_PATH];

    buf[0] = '\0';
    if (TheApp->getFileDialogDir())
        while (chdir(TheApp->getFileDialogDir()) == -2);
    if (swOpenFileDialog(m_wnd, "Select Image (*.jpg, *.png)",
#ifdef HAVE_LIBDEVIL
# ifdef _WIN32
        "All Files (*.*)\0*.*\0\0",
# else
        "*.*",
# endif
#else
# ifdef _WIN32
        "Image Files (*.gif, *.jpg, *.png)\0*.gif;*.jpg;*.png\0All Files (*.*)\0*.*\0\0",
# else
        "*.[gjp][ipn][fgg]",
# endif
#endif
                         buf, MY_MAX_PATH)) {
        NodeImageTexture *node = (NodeImageTexture *)
                                 m_scene->createNode("ImageTexture");
        URL url;
        url.FromPath(buf);
        node->url(new MFString(rewriteURL(url,url.GetDir(),m_scene->getURL())));
        doWithBranch(createTexture, node);
    }
}

static bool createAppearance(Node *node, void *data)
{
    if (node->getType() == VRML_SHAPE) {
        NodeShape *shapeNode = (NodeShape *) node;
        // ignore Nodes with already set Appearance field
        Node *appearance = (Node *)data;
        if ((shapeNode->appearance()->getValue() == NULL) &&
            (appearance->getType() & VRML_APPEARANCE)) {
            MoveCommand *command = new MoveCommand(appearance, NULL, -1,
                  shapeNode, shapeNode->appearance_Field());
            command->execute();
        }
    }
    return true;     
}

void
MainWindow::createBranchAppearance(void)
{
    NodeAppearance *node = (NodeAppearance *) m_scene->createNode("Appearance");
    doWithBranch(createAppearance, node);
}

static bool createMaterial(Node *node, void *data)
{
    if ((node->getType() == VRML_APPEARANCE) ||
        (node->getType() == KAMBI_KAMBI_APPEARANCE)) {
        NodeAppearance *appearanceNode = (NodeAppearance *) node;
        // ignore Nodes with already set Material field
        Node *material = (Node *)data;
        if (appearanceNode && 
            (appearanceNode->material()->getValue() == NULL) &&
            (material->getType() & VRML_MATERIAL)) {
            MoveCommand *command = new MoveCommand(material, NULL, -1,
                  appearanceNode, appearanceNode->material_Field());
            command->execute();
        }
    }
    return true;     
}

void
MainWindow::createBranchMaterial(void)
{
    NodeMaterial *node = (NodeMaterial *) m_scene->createNode("Material");
    doWithBranch(createMaterial, node);
}

Node*
MainWindow::getTransformForCenter() 
{
    Node *node = m_scene->getSelection()->getNode(); 
    if (node != m_scene->getRoot()) {
        node = node->getParent();
        if (node->getType() == VRML_SHAPE)
           if (node != m_scene->getRoot()) {
               Node *shapeParent = node->getParent();
               if (shapeParent->getType() == VRML_TRANSFORM)
                   return shapeParent;

           }
    }
    return NULL;
}

void
MainWindow::centerToMid()
{
   NodeTransform *transform = (NodeTransform *) getTransformForCenter();
   if (transform != NULL) {
       Node *node = m_scene->getSelection()->getNode(); 
       Vec3f min = node->getMinBoundingBox(); 
       Vec3f max = node->getMaxBoundingBox();
       Vec3f center = ((max + min) / 2.0f) * transform->rotation()->getQuat().conj();
       m_scene->setField(transform, transform->center_Field(), 
                        new SFVec3f(center));
   }
}

void
MainWindow::centerToMax(int index)
{
   NodeTransform *transform = (NodeTransform *) getTransformForCenter();
   if (transform != NULL) {
       Node *node = m_scene->getSelection()->getNode(); 
       Vec3f max = node->getMaxBoundingBox(); 
       Vec3f vcenter = transform->center()->getValue();
       vcenter = vcenter * transform->rotation()->getQuat().conj();
       vcenter[index] = max[index];
       vcenter = vcenter * transform->rotation()->getQuat();
       m_scene->setField(transform, transform->center_Field(), 
                        new SFVec3f(vcenter));
   }
}

void 
MainWindow::centerToMin(int index)
{
   NodeTransform *transform = (NodeTransform *) getTransformForCenter();
   if (transform != NULL) {
       Node *node = m_scene->getSelection()->getNode(); 
       Vec3f min = node->getMinBoundingBox(); 
       Vec3f vcenter = transform->center()->getValue();
       vcenter = vcenter * transform->rotation()->getQuat().conj();
       vcenter[index] = min[index];
       vcenter = vcenter * transform->rotation()->getQuat();
       m_scene->setField(transform, transform->center_Field(), 
                        new SFVec3f(vcenter));
   }
}

void MainWindow::flip(int index)
{
    m_scene->getNodes()->clearFlag(NODE_FLAG_FLIPPED);
    Node *node = m_scene->getSelection()->getNode(); 
    if (node != NULL)
        node->flip(index);
    m_scene->UpdateViews(NULL, UPDATE_SELECTION);
}

void MainWindow::swap(int fromTo)
{
    m_scene->getNodes()->clearFlag(NODE_FLAG_SWAPPED);
    Node *node = m_scene->getSelection()->getNode(); 
    if (node != NULL)
        node->swap(fromTo);
    m_scene->UpdateViews(NULL, UPDATE_SELECTION);
}


void MainWindow::moveTo(Node *node, int handle, int direction, Vec3f v, 
                        float value)
{
    switch (direction) {
      case 0:
        v.x = value;
        break;
      case 1:
        v.y = value;
        break;
      case 2:
        v.z = value;
        break;
    }
    int mode = m_scene->getSelectionMode();
    m_scene->setSelectionMode(SELECTION_MODE_VERTICES);
    node->setHandle(handle, v);
    m_scene->setSelectionMode(mode);
}

void MainWindow::moveTo(int direction)
{
    Node *node = m_scene->getSelection()->getNode(); 
    OneFloatDialog dlg(m_wnd, IDD_MOVE_TO, 0);
    if (dlg.DoModal() == IDCANCEL)
        return;
    float value = dlg.GetValue();
    if (m_scene->getSelectedHandlesSize() > 1)
        node->startSetMultiHandles();
    MyArray<int> selectedVerticesHandles;
    MyArray<Vec3f> selectedVertices;
    Node* parent = node;
    if (node->getType() == VRML_COORDINATE)
        parent = node->getParent();
    for (int i = 0; i < m_scene->getSelectedHandlesSize(); i++) {
        int handle = m_scene->getSelectedHandle(i);
        if ((handle < 0) || (handle >= NO_HANDLE))
            continue;
        int constrain = m_scene->getConstrain();
        int field = -1;
        if ((m_scene->getSelectionMode() == SELECTION_MODE_VERTICES) ||
            (parent->getType() != VRML_INDEXED_FACE_SET)) {
            Vec3f v = node->getHandle(handle, &constrain, &field);
            m_scene->backupFieldsStart();
            m_scene->backupFieldsAppend(node, field);
            m_scene->backupFieldsDone();
            moveTo(node, handle, direction, v, value);
        } else if (m_scene->getSelectionMode() == SELECTION_MODE_FACES) {
            MyMesh *mesh = node->getMesh();
            if (mesh == NULL)
                return;
            MFVec3f *point = mesh->getVertices();
            MFInt32 *ci = mesh->getCoordIndex();
            FaceData *face = mesh->getFace(handle);
            int offset = face->getOffset();
            for (int j = offset; j < offset + face->getNumVertices(); j++) {
                int coordIndex = ci->getValue(j);
                if (coordIndex >= point->getSFSize())
                    continue;
                Vec3f vec = point->getValue(coordIndex);
                bool isInSelectedVertices = false;
                for (size_t n = 0; n < selectedVertices.size(); n++)
                    if (selectedVerticesHandles[n] == coordIndex) {
                        isInSelectedVertices = true;
                        break;
                    }
                if (!isInSelectedVertices) {
                   selectedVerticesHandles.append(coordIndex);
                   selectedVertices.append(vec);    
                }
            }
        } else if (m_scene->getSelectionMode() == SELECTION_MODE_LINES) {
            MyMesh *mesh = node->getMesh();
            if (mesh == NULL)
                return;
            MFVec3f *point = mesh->getVertices();
            MFInt32 *ci = mesh->getCoordIndex();
            int numLine = mesh->getLine(handle).begin;
            int coordIndex1 = ci->getValue(numLine);
            Vec3f vec1 = point->getValue(coordIndex1);
            bool isInSelectedVertices = false;
            for (size_t n = 0; n < selectedVertices.size(); n++)
                 if (selectedVerticesHandles[n] == coordIndex1) {
                     isInSelectedVertices = true;
                     break;
                 }
             if (!isInSelectedVertices) {
                 selectedVerticesHandles.append(coordIndex1);
                 selectedVertices.append(vec1);    
             }
            int coordIndex2 = ci->getValue(mesh->getLine(handle).end);
            Vec3f vec2 = point->getValue(coordIndex2);
            for (size_t n = 0; n < selectedVertices.size(); n++)
                 if (selectedVerticesHandles[n] == coordIndex2) {
                     isInSelectedVertices = true;
                     break;
                 }
             if (!isInSelectedVertices) {
                 selectedVerticesHandles.append(coordIndex2);
                 selectedVertices.append(vec2);    
             }
        }
    }
    for (size_t j = 0; j < selectedVerticesHandles.size(); j++)
        moveTo(node, selectedVerticesHandles[j], direction, 
                selectedVertices[j], value);
    if (m_scene->getSelectedHandlesSize() > 1)
        node->endSetMultiHandles();
    swInvalidateWindow(m_wnd);
}

void MainWindow::scaleBy(Node *node, int handle, int direction, Vec3f v, 
                         float value)
{
    switch (direction) {
      case 0:
        v.x = v.x * value;
        break;
      case 1:
        v.y = v.y * value;
        break;
      case 2:
        v.z = v.z * value;
        break;
    }
    int mode = m_scene->getSelectionMode();
    m_scene->setSelectionMode(SELECTION_MODE_VERTICES);
    node->setHandle(handle, v);
    m_scene->setSelectionMode(mode);
}

void MainWindow::scaleBy(int direction)
{
    Node *node = m_scene->getSelection()->getNode(); 
    OneFloatDialog dlg(m_wnd, IDD_SCALE_BY, 1.0f, 0.0f);
    if (dlg.DoModal() == IDCANCEL)
        return;
    float value = dlg.GetValue();
    if (m_scene->getSelectedHandlesSize() > 1)
        node->startSetMultiHandles();
    MyArray<int> selectedVerticesHandles;
    MyArray<Vec3f> selectedVertices;
    Node* parent = node;
    if (node->getType() == VRML_COORDINATE)
        parent = node->getParent();
    for (int i = 0; i < m_scene->getSelectedHandlesSize(); i++) {
        int handle = m_scene->getSelectedHandle(i);
        if ((handle < 0) || (handle >= NO_HANDLE))
            continue;
        int constrain = m_scene->getConstrain();
        int field = -1;
        if ((m_scene->getSelectionMode() == SELECTION_MODE_VERTICES) ||
            (parent->getType() != VRML_INDEXED_FACE_SET)) {
            Vec3f v = node->getHandle(handle, &constrain, &field);
            m_scene->backupFieldsStart();
            m_scene->backupFieldsAppend(node, field);
            m_scene->backupFieldsDone();
            scaleBy(node, handle, direction, v, value);
        } else if (m_scene->getSelectionMode() == SELECTION_MODE_FACES) {
            MyMesh *mesh = node->getMesh();
            if (mesh == NULL)
                return;
            MFVec3f *point = mesh->getVertices();
            MFInt32 *ci = mesh->getCoordIndex();
            FaceData *face = mesh->getFace(handle);
            int offset = face->getOffset();
            for (int j = offset; j < offset + face->getNumVertices(); j++) {
                int coordIndex = ci->getValue(j);
                if (coordIndex >= point->getSFSize())
                    continue;
                Vec3f vec = point->getValue(coordIndex);
                bool isInSelectedVertices = false;
                for (size_t n = 0; n < selectedVertices.size(); n++)
                    if (selectedVerticesHandles[n] == coordIndex) {
                        isInSelectedVertices = true;
                        break;
                    }
                if (!isInSelectedVertices) {
                   selectedVerticesHandles.append(coordIndex);
                   selectedVertices.append(vec);    
                }
            }
        } else if (m_scene->getSelectionMode() == SELECTION_MODE_LINES) {
            MyMesh *mesh = node->getMesh();
            if (mesh == NULL)
                return;
            MFVec3f *point = mesh->getVertices();
            MFInt32 *ci = mesh->getCoordIndex();
            int numLine = mesh->getLine(handle).begin;
            int coordIndex1 = ci->getValue(numLine);
            Vec3f vec1 = point->getValue(coordIndex1);
            bool isInSelectedVertices = false;
            for (size_t n = 0; n < selectedVertices.size(); n++)
                 if (selectedVerticesHandles[n] == coordIndex1) {
                     isInSelectedVertices = true;
                     break;
                 }
             if (!isInSelectedVertices) {
                 selectedVerticesHandles.append(coordIndex1);
                 selectedVertices.append(vec1);    
             }
            int coordIndex2 = ci->getValue(mesh->getLine(handle).end);
            Vec3f vec2 = point->getValue(coordIndex2);
            for (size_t n = 0; n < selectedVertices.size(); n++)
                 if (selectedVerticesHandles[n] == coordIndex2) {
                     isInSelectedVertices = true;
                     break;
                 }
             if (!isInSelectedVertices) {
                 selectedVerticesHandles.append(coordIndex2);
                 selectedVertices.append(vec2);    
             }
        }
    }
    for (size_t j = 0; j < selectedVerticesHandles.size(); j++)
        scaleBy(node, selectedVerticesHandles[j], direction, 
                selectedVertices[j], value);
    if (m_scene->getSelectedHandlesSize() > 1)
        node->endSetMultiHandles();
    swInvalidateWindow(m_wnd);
}

void MainWindow::same(int direction,int whichSame)
{
    float sameValue = 0;
    Node *node = m_scene->getSelection()->getNode(); 
    if (m_scene->getSelectedHandlesSize() > 1)
        node->startSetMultiHandles();
    MyArray<int> selectedVerticesHandles;
    MyArray<Vec3f> selectedVertices;
    Node* parent = node;
    if (node->getType() == VRML_COORDINATE)
        parent = node->getParent();
    for (int i = 0; i < m_scene->getSelectedHandlesSize(); i++) {
        int handle = m_scene->getSelectedHandle(i);
        if ((handle < 0) || (handle >= NO_HANDLE))
            continue;
        int constrain = m_scene->getConstrain();
        int field = -1;
        if ((m_scene->getSelectionMode() == SELECTION_MODE_VERTICES) ||
            (parent->getType() != VRML_INDEXED_FACE_SET)) {
            Vec3f vec = node->getHandle(handle, &constrain, &field);
            m_scene->backupFieldsStart();
            m_scene->backupFieldsAppend(node, field);
            m_scene->backupFieldsDone();
            selectedVerticesHandles.append(handle);
            selectedVertices.append(vec);    
        } else if (m_scene->getSelectionMode() == SELECTION_MODE_FACES) {
            MyMesh *mesh = node->getMesh();
            if (mesh == NULL)
                return;
            MFVec3f *point = mesh->getVertices();
            MFInt32 *ci = mesh->getCoordIndex();
            FaceData *face = mesh->getFace(handle);
            int offset = face->getOffset();
            for (int j = offset; j < offset + face->getNumVertices(); j++) {
                int coordIndex = ci->getValue(j);
                if (coordIndex >= point->getSFSize())
                    continue;
                Vec3f vec = point->getValue(coordIndex);
                bool isInSelectedVertices = false;
                for (size_t n = 0; n < selectedVertices.size(); n++)
                    if (selectedVerticesHandles[n] == coordIndex) {
                        isInSelectedVertices = true;
                        break;
                    }
                if (!isInSelectedVertices) {
                   selectedVerticesHandles.append(coordIndex);
                   selectedVertices.append(vec);    
                }
            }
        } else if (m_scene->getSelectionMode() == SELECTION_MODE_LINES) {
            MyMesh *mesh = node->getMesh();
            if (mesh == NULL)
                return;
            MFVec3f *point = mesh->getVertices();
            MFInt32 *ci = mesh->getCoordIndex();
            int numLine = mesh->getLine(handle).begin;
            int coordIndex1 = ci->getValue(numLine);
            Vec3f vec1 = point->getValue(coordIndex1);
            bool isInSelectedVertices = false;
            for (size_t n = 0; n < selectedVertices.size(); n++)
                 if (selectedVerticesHandles[n] == coordIndex1) {
                     isInSelectedVertices = true;
                     break;
                 }
             if (!isInSelectedVertices) {
                 selectedVerticesHandles.append(coordIndex1);
                 selectedVertices.append(vec1);    
             }
            int coordIndex2 = ci->getValue(mesh->getLine(handle).end);
            Vec3f vec2 = point->getValue(coordIndex2);
            for (size_t n = 0; n < selectedVertices.size(); n++)
                 if (selectedVerticesHandles[n] == coordIndex2) {
                     isInSelectedVertices = true;
                     break;
                 }
             if (!isInSelectedVertices) {
                 selectedVerticesHandles.append(coordIndex2);
                 selectedVertices.append(vec2);    
             }
        }
    }
    if (whichSame == SAME_MAX)
        sameValue = FLT_MIN;
    else if (whichSame == SAME_MIN)
        sameValue = FLT_MAX;
    int constrain = m_scene->getConstrain();
    int field = -1;
    for (size_t j = 0; j < selectedVerticesHandles.size(); j++) {
        Vec3f vec = node->getHandle(selectedVerticesHandles[j], &constrain, 
                                    &field);
        switch (direction) {
          case 0: 
            switch (whichSame) {
              case SAME_MAX:
                if (vec.x > sameValue)
                    sameValue = vec.x;
                break;
              case SAME:
                sameValue += vec.x;
                break;
              case SAME_MIN:
                if (vec.x < sameValue)
                    sameValue = vec.x;
                break;
            }
            break;
          case 1: 
            switch (whichSame) {
              case SAME_MAX:
                if (vec.y > sameValue)
                    sameValue = vec.y;
                break;
              case SAME:
                sameValue += vec.y;
                break;
              case SAME_MIN:
                if (vec.y < sameValue)
                    sameValue = vec.y;
                break;
            }
            break;
          case 2: 
            switch (whichSame) {
              case SAME_MAX:
                if (vec.z > sameValue)
                    sameValue = vec.z;
                break;
              case SAME:
                sameValue += vec.z;
                break;
              case SAME_MIN:
                if (vec.z < sameValue)
                    sameValue = vec.z;
                break;
            }
        }
    }
    if ((whichSame == SAME) && (selectedVerticesHandles.size() != 0))
        sameValue /= selectedVerticesHandles.size(); 
    for (size_t j = 0; j < selectedVerticesHandles.size(); j++) {
        Vec3f vec = node->getHandle(selectedVerticesHandles[j], &constrain, 
                                    &field);
        moveTo(node, selectedVerticesHandles[j], direction, vec, sameValue);
    }
    if (m_scene->getSelectedHandlesSize() > 1)
        node->endSetMultiHandles();
    swInvalidateWindow(m_wnd);
}

void MainWindow::setDefault()
{
    Node *node = m_scene->getSelection()->getNode(); 
    if (node != NULL)
        if (node->maySetDefault())
            node->setDefault();
}

void MainWindow::insertNewNurbsNode(Node *nurbsNode, Node* parent)
{
    bool defaultColor = false;
    if (nurbsNode != NULL) {
        // be carefull with sending delete commands to a commandlist
        // later commands which would use the place of the deleted node
        // would fail
        CommandList *list = new CommandList();
        if (isToNurbsAllowedParent(parent)) 
            m_scene->deleteSelected();
        else 
            return;
        if (parent == m_scene->getRoot())
            list->append(new MoveCommand(nurbsNode, NULL, -1, 
                            m_scene->getRoot(), m_scene->getRootField()));
        else if (parent->getType() == VRML_GROUP)
            list->append(new MoveCommand(nurbsNode, NULL, -1, parent, 
                           ((NodeGroup *)parent)->children_Field()));
        else if (parent->getType() == VRML_TRANSFORM)
            list->append(new MoveCommand(nurbsNode, NULL, -1, parent, 
                           ((NodeTransform *)parent)->children_Field()));
        else if (parent->getType() == VRML_ANCHOR)
            list->append(new MoveCommand(nurbsNode, NULL, -1, parent, 
                           ((NodeAnchor *)parent)->children_Field()));
        else if (parent->getType() == VRML_SHAPE) {
            list->append(new MoveCommand(nurbsNode, NULL, -1, parent, 
                             ((NodeShape *)parent)->geometry_Field()));
            if (defaultColor) {
                NodeAppearance *nAppearance = (NodeAppearance *)
                                              m_scene->createNode("Appearance");
                NodeMaterial   *nMaterial = (NodeMaterial *) 
                                            m_scene->createNode("Material");
                nAppearance->material(new SFNode(nMaterial));
                if (((NodeShape *)parent)->appearance() != NULL) {
                    list->append(new MoveCommand(
                          ((NodeShape *)parent)->appearance()->getValue(),
                          parent, ((NodeShape *)parent)->appearance_Field(), 
                               NULL, -1));
                    // the former is a delete command, so this command
                    // must be executed at once
                    m_scene->execute(list);
                    list = new CommandList();
                }
                list->append(new MoveCommand(nAppearance, NULL, -1, parent,
                      ((NodeShape *)parent)->appearance_Field()));
            }
        } else if (parent->getType() == VRML_CONTOUR_2D)
            list->append(new MoveCommand(nurbsNode, NULL, -1, parent, 
                           ((NodeContour2D *)parent)->children_Field()));
        m_scene->execute(list);
        m_scene->setSelection(nurbsNode);
        m_scene->UpdateViews(NULL, UPDATE_SELECTION);
    }
}

void MainWindow::triangulate(void) {
    Node *node = m_scene->getSelection()->getNode(); 
    if (node->getType() == VRML_COORDINATE) {
        node = node->getParent();
    }
    if (node->getType() == VRML_INDEXED_FACE_SET) {
        NodeIndexedFaceSet *faceset = (NodeIndexedFaceSet *) node;
        MyMesh *mesh = faceset->triangulateMesh();
        MFInt32 *mfcoordIndex = mesh->getCoordIndex();
        if (mfcoordIndex) {
            mfcoordIndex = (MFInt32 *) mfcoordIndex->copy();
            m_scene->setField(faceset, faceset->coordIndex_Field(),
                              new MFInt32(*mfcoordIndex));
        }
        NodeCoordinate *coord = (NodeCoordinate *)faceset->coord()->getValue();
        MFVec3f *mfcoord = mesh->getVertices();
        if (coord && mfcoord) {
            mfcoord = (MFVec3f *) mfcoord->copy();
            m_scene->setField(coord, coord->point_Field(), 
                              new MFVec3f(*mfcoord));
        }
        NodeNormal *normal = (NodeNormal *)faceset->normal()->getValue();
        MFVec3f *normals = mesh->getNormals();
        if (normal && normals) {
            normals = (MFVec3f *) normals->copy();
            m_scene->setField(normal, normal->vector_Field(), 
                              new MFVec3f(*normals));
        }
        m_scene->setField(faceset, faceset->normalIndex_Field(), new MFInt32());

        MFVec2f *texCoords = mesh->getTexCoords();
        Node *texC = faceset->texCoord()->getValue();
        NodeTextureCoordinate *texCoord = (NodeTextureCoordinate *)texC;
        if (texCoord && texCoords && 
            (texC->getType() == VRML_TEXTURE_COORDINATE)) {
            texCoords = (MFVec2f *) texCoords->copy();
            m_scene->setField(texCoord, texCoord->point_Field(), 
                              new MFVec2f(*texCoords));
        }
        m_scene->setField(faceset, faceset->texCoordIndex_Field(), 
                         new MFInt32());
        m_scene->setField(faceset, faceset->convex_Field(), new SFBool(true));

        m_scene->UpdateViews(NULL, UPDATE_SELECTION);
    }
}

void MainWindow::optimizeSet(void) {
    Node *node = m_scene->getSelection()->getNode(); 
    if (node->getType() == VRML_COORDINATE) {
        node = node->getParent();
    }
    if (node->isMeshBasedNode()) {
        MeshBasedNode *set = (MeshBasedNode *) node;
        MyMesh *mesh = set->getMesh();
        MFVec3f *mfVertices = NULL;
        MFInt32 *mfCoordIndex = NULL;
        set->optimizeMesh(&mfVertices, &mfCoordIndex);
        NodeIndexedFaceSet *faceset = (NodeIndexedFaceSet *)
                                      m_scene->createNode("IndexedFaceSet"); 
        NodeCoordinate *coord = (NodeCoordinate *) 
                                m_scene->createNode("Coordinate");
        if (coord && mfVertices && mfCoordIndex) {
            coord->point(new MFVec3f(*mfVertices));
            faceset->coord(new SFNode(coord));
            faceset->coordIndex(new MFInt32(*mfCoordIndex));
            faceset->texCoordIndex(new MFInt32());
            faceset->texCoord(new SFNode());
            faceset->ccw(new SFBool(mesh->ccw()));
            faceset->convex(new SFBool(mesh->convex()));
            faceset->creaseAngle(new SFFloat(mesh->creaseAngle()));
            faceset->solid(new SFBool(mesh->solid()));
            m_scene->setSelection(m_scene->replaceNode(node, faceset));
            m_scene->UpdateViews(NULL, UPDATE_SELECTION);
        }
    }
}

void MainWindow::rebuildControlPoints()
{
    Node *node = m_scene->getSelection()->getNode(); 
    int type = node->getType();
    Node *parent = node->getParent();

    Node* nurbsNode = NULL;

    if (type == VRML_NURBS_SURFACE) {
        Sphere2NurbsDialog dlg(m_wnd, 8, 8, 2, 2);
        if (dlg.DoModal() == IDOK) {
            int narcslong = dlg.getNarcslong();
            int narcslat = dlg.getNarcslat();
            int uDegree = dlg.getuDegree();
            int vDegree = dlg.getvDegree();
            nurbsNode = node->toNurbs(narcslong, narcslat, uDegree, vDegree);
        }
    }

    if (nurbsNode != NULL) {
        if (!isToNurbsAllowedParent(parent)) 
            return;
        m_scene->unHideVertices();
        m_scene->setSelection(m_scene->replaceNode(node, nurbsNode));
        m_scene->UpdateViews(NULL, UPDATE_SELECTION);
    }
}

void                
MainWindow::makeNurbSurfaceSymetric(int direction)
{
    Node *node = m_scene->getSelection()->getNode();
    if (node && (node->getType() == VRML_COORDINATE))
        node = node->getParent();
    if (node && node->getType() != VRML_NURBS_SURFACE)
        return;
    NodeNurbsSurface *nurbs = (NodeNurbsSurface *)node;
    int idd = IDD_NURBS_SURFACE_SYMETRIC_X;
    switch (direction) {
      case 1:
        idd = IDD_NURBS_SURFACE_SYMETRIC_Y;
        break;
      case 2:
        idd = IDD_NURBS_SURFACE_SYMETRIC_Z;
        break;
    }
    OneBoolDialog dlg(m_wnd, idd, true);
    if (dlg.DoModal() == IDCANCEL)
        return;

    nurbs->makeSymetric(direction, dlg.GetValue());
    nurbs->makeSymetric(direction, dlg.GetValue()); // ???
    m_scene->setSelection(m_scene->replaceNode(node, nurbs));
    m_scene->UpdateViews(NULL, UPDATE_SELECTION);
}

void 
MainWindow::insertToNurbsCurve()
{
    Node *node = m_scene->getSelection()->getNode(); 
    int type = node->getType();

    if (type == VRML_NURBS_CURVE) {
        int min = INT_MAX;
        for (int i = 0; i < m_scene->getSelectedHandlesSize(); i++) {
             int handle = m_scene->getSelectedHandle(i);
             if (handle < NO_HANDLE && handle > -1 && handle < min)
                 min = handle;
        }
        if (min == INT_MAX)
            min = 0;
        int max = 0;
        for (int i = 0; i < m_scene->getSelectedHandlesSize(); i++) {
             int handle = m_scene->getSelectedHandle(i);
             if (handle < NO_HANDLE && handle > max)
                 max = handle;
        }
        InsertToNurbsCurveDialog dlg(m_wnd, min, max);        
        if (dlg.DoModal() == IDOK) {
           int from = dlg.getFrom();
           int to = dlg.getTo();
           int points = dlg.getPoints();
           NodeNurbsCurve *nurbs = (NodeNurbsCurve *)node;
           nurbs->extrudePoints(from, to, points);
        }
        m_scene->UpdateViews(NULL, UPDATE_SELECTION);
    }
}

void 
MainWindow::insertToNurbsSurface()
{
    Node *node = m_scene->getSelection()->getNode(); 
    int type = node->getType();

    if (type == VRML_NURBS_SURFACE) {
        NodeNurbsSurface *nurbs = (NodeNurbsSurface *)node;
        int uDim = nurbs->uDimension()->getValue();
        int umin = INT_MAX;
        int umax = 0;
        int vmin = INT_MAX;
        int vmax = 0;
        for (int i = 0; i < m_scene->getSelectedHandlesSize(); i++) {
             int handle = m_scene->getSelectedHandle(i);
             if (handle < NO_HANDLE && handle > -1) {
                 if (handle % uDim < umin)
                     umin = handle % uDim;
                 if (handle % uDim > umax)
                     umax = handle % uDim;
                 if (handle / uDim < vmin)
                     vmin = handle / uDim;
                 if (handle / uDim > vmax)
                     vmax = handle / uDim;
             }
        }
        if (umin == INT_MAX)
            umin = 0;
        if (vmin == INT_MAX)
            vmin = 0;
        InsertToNurbsSurfaceDialog dlg(m_wnd, umin, umax, vmin, vmax);        
        if (dlg.DoModal() == IDOK) {
           int ufrom = dlg.getFromU();
           int uto = dlg.getToU();
           int upoints = dlg.getPointsU();
           int vfrom = dlg.getFromV();
           int vto = dlg.getToV();
           int vpoints = dlg.getPointsV();
           nurbs->extrudePoints(ufrom, uto, upoints, vfrom, vto, vpoints);
        }
        m_scene->UpdateViews(NULL, UPDATE_SELECTION);
    }
}

void MainWindow::degreeElevate(int degree)
{
    Node *node = m_scene->getSelection()->getNode(); 
    int type = node->getType();
    Node *parent = node->getParent();

    Node* newNode = NULL;

    if (type == VRML_NURBS_CURVE) {
        NodeNurbsCurve *oldNode = (NodeNurbsCurve *)node;
        int newDegree = oldNode->order()->getValue() - 1 + degree;
        newNode = node->degreeElevate(newDegree);
        insertNewNurbsNode(newNode, parent);
    } else if (type == VRML_NURBS_CURVE_2D) {
        NodeNurbsCurve2D *oldNode = (NodeNurbsCurve2D *)node;
        int newDegree = oldNode->order()->getValue() - 1 + degree;
        newNode = node->degreeElevate(newDegree);
        insertNewNurbsNode(newNode, parent);
    } else if (type == DUNE_SUPER_EXTRUSION) {
        NodeSuperExtrusion *oldNode = (NodeSuperExtrusion *)node;
        int newDegree = oldNode->order()->getValue() - 1 + degree;
        newNode = node->degreeElevate(newDegree);
        insertNewNurbsNode(newNode, parent);
    } else if (type == DUNE_SUPER_REVOLVER) {
        NodeSuperRevolver *oldNode = (NodeSuperRevolver *)node;
        int newDegree = oldNode->order()->getValue() - 1 + degree;
        newNode = node->degreeElevate(newDegree);
        insertNewNurbsNode(newNode, parent);
    } else if (type == DUNE_CURVE_ANIMATION) {
        NodeCurveAnimation *oldNode = (NodeCurveAnimation *)node;
        int newDegree = oldNode->order()->getValue() - 1 + degree;
        newNode = node->degreeElevate(newDegree);
        insertNewNurbsNode(newNode, parent);
    }
    m_scene->UpdateViews(NULL, UPDATE_SELECTION);
}

void MainWindow::uDegreeElevate(int degree)
{
    Node *node =m_scene->getSelection()->getNode();
    int type = node->getType();
    Node *parent = node->getParent();
                          
    Node* nurbsNode = NULL;
                    
    if (type == VRML_NURBS_SURFACE) {
        NodeNurbsSurface *oldNurbs = (NodeNurbsSurface *)node;
        int newUDegree = oldNurbs->uOrder()->getValue() - 1 + degree;
        int newVDegree = oldNurbs->vOrder()->getValue() - 1;
        nurbsNode = node->degreeElevate(newUDegree, newVDegree);
        //SFRotation rot(0, 1, 0, M_PI / 2.0);
        //((NodeNurbsSurface *)nurbsNode)->rotate(rot);
        insertNewNurbsNode(nurbsNode, parent);
        m_scene->UpdateViews(NULL, UPDATE_SELECTION);
    }
}

void MainWindow::vDegreeElevate(int degree)
{
    Node *node =m_scene->getSelection()->getNode();
    int type = node->getType();
    Node *parent = node->getParent();
                          
    Node* nurbsNode = NULL;
                    
    if (type == VRML_NURBS_SURFACE) {
        NodeNurbsSurface *oldNurbs = (NodeNurbsSurface *)node;
        int newUDegree = oldNurbs->uOrder()->getValue() - 1;
        int newVDegree = oldNurbs->vOrder()->getValue() -1 + degree;
        nurbsNode = node->degreeElevate(newUDegree, newVDegree);
        //SFRotation rot(0, 1, 0, M_PI / 2.0);
        //((NodeNurbsSurface *)nurbsNode)->rotate(rot);
        insertNewNurbsNode(nurbsNode, parent);
        m_scene->UpdateViews(NULL, UPDATE_SELECTION);
    }
}

void                
MainWindow::setWeightsTo1(void)
{
    Node *node =m_scene->getSelection()->getNode();
    int type = node->getType();
    Node *parent = node->getParent();
                          
    if (type == VRML_NURBS_SURFACE) {
        NodeNurbsSurface *oldNurbs = (NodeNurbsSurface *)node;
        Node *newNurbs = oldNurbs->setWeightsTo1();
        if (newNurbs != NULL)
            insertNewNurbsNode(newNurbs, parent);
    } else if (type == X3D_NURBS_TRIMMED_SURFACE) {
        NodeNurbsTrimmedSurface *oldNurbs = (NodeNurbsTrimmedSurface *)node;
        Node *newNurbs = oldNurbs->setWeightsTo1();
        if (newNurbs != NULL)
            insertNewNurbsNode(newNurbs, parent);
    } else if (type == VRML_NURBS_CURVE_2D) {
        NodeNurbsCurve2D *oldNurbs = (NodeNurbsCurve2D *)node;
        Node *newNurbs = oldNurbs->setWeightsTo1();
        if (newNurbs != NULL)
            insertNewNurbsNode(newNurbs, parent);
    }
    m_scene->UpdateViews(NULL, UPDATE_SELECTION);
}


void                
MainWindow::elongateNurbs()
{
    if (m_scene->getSelectedHandlesSize() == 0)
        return;
    if (m_scene->getSelectedHandlesSize() != 1)
        return;
    Node *node = m_scene->getSelection()->getNode();
    if (node->getType() == DUNE_CURVE_ANIMATION) {
        NodeCurveAnimation *animation = (NodeCurveAnimation *)node;
        node = animation->toNurbsCurve();
    }
    if (node->getType() == X3D_NURBS_SWEPT_SURFACE) {
        NodeNurbsSweptSurface *sweptSurface = (NodeNurbsSweptSurface *)node;
        if (sweptSurface->trajectoryCurve()->getValue() != NULL)
            node = sweptSurface->trajectoryCurve()->getValue();
    }
    NurbsCurveDialog dlg(m_wnd, 5, node->getDegree(), 0, true);
    if (dlg.DoModal() == IDOK) {
        int dimension = dlg.getnPoints();
        int direction = dlg.getDirection();

        float *controlPoints = new float[dimension * 3];
        
        for (int i=0; i<dimension; i++){
            float point = ((float) i - 0.5 * (dimension - 1.0)) / 
                          (dimension - 1.0) * 2.0;
            controlPoints[(i*3)]   = (direction == 0 ? point : 0);
            controlPoints[(i*3)+1] = (direction == 1 ? point : 0);
            controlPoints[(i*3)+2] = (direction == 2 ? point : 0);
        }
        node->elongateNurbs(m_scene->getSelectedHandle(0), controlPoints);
        m_scene->UpdateViews(NULL, UPDATE_SELECTION);
    }
}

void 
MainWindow::linearUknot(void)
{
    Node *node =m_scene->getSelection()->getNode();
    if (node->getType() == VRML_NURBS_SURFACE) {
        NodeNurbsSurface *nurbs = (NodeNurbsSurface *)node;
        nurbs->linearUknot();
        m_scene->UpdateViews(NULL, UPDATE_SELECTION);
    }
}

void MainWindow::linearVknot(void)
{
    Node *node =m_scene->getSelection()->getNode();
    if (node->getType() == VRML_NURBS_SURFACE) {
        NodeNurbsSurface *nurbs = (NodeNurbsSurface *)node;
        nurbs->linearVknot();
        m_scene->UpdateViews(NULL, UPDATE_SELECTION);
    }
}

void
MainWindow::swapDiffuseEmissiveShape(NodeShape *shape,
                                     ColorConversion conversion)
{
    SFNode *appear = shape->appearance();
    if (appear != NULL) {
        NodeAppearance *nAppearance = (NodeAppearance *) appear->getValue();
        NodeMaterial *nMaterial = NULL;
        if (nAppearance)
            nMaterial = (NodeMaterial *) nAppearance->material()->getValue(); 
        if (nMaterial != NULL) {
            switch(conversion) {
               case DIFFUSE2EMISSIVE:
                 nMaterial->diffuse2emissive();
                 break;
               case EMISSIVE2DIFFUSE:
                 nMaterial->emissive2diffuse();
                 break;
               default:
                 assert(0);
            }
        }
    }
}

void
MainWindow::toNurbs()
{
    Node *node = m_scene->getSelection()->getNode(); 
    int type = node->getType();
    if (!node->hasParent())
        return;
    Node *parent = node->getParent();

    Node* nurbsNode = NULL;

    if (type == VRML_CYLINDER) {
        Cylinder2NurbsDialog dlg(m_wnd, node, TheApp->getWeights1() ? 8 : 4, 
                                 2, 3, 2, 2);
        if (dlg.DoModal() == IDOK) {
            int narcs = dlg.getNarcs();
            int nshell = dlg.getNshell();
            int narea  = dlg.getNarea();
            int uDegree = dlg.getuDegree();
            int vDegree = dlg.getvDegree();
            nurbsNode = node->toNurbs(nshell, narea, narcs, uDegree, vDegree);
            SFRotation rot(0, 1 , 0, M_PI / 2.0);
            ((NodeNurbsSurface *)nurbsNode)->rotate(rot);
            if (TheApp->getWeights1())
                ((NodeNurbsSurface *)nurbsNode)->setWeightsTo1();
        }
    }
    if (type == VRML_CONE) {
        Cone2NurbsDialog dlg(m_wnd, node, TheApp->getWeights1() ? 8 : 4, 2, 4, 
                             2, 2);
        if (dlg.DoModal() == IDOK) {
            int narcs = dlg.getNarcs();
            int nshell = dlg.getNshell();
            int narea  = dlg.getNarea();
            int uDegree = dlg.getuDegree();
            int vDegree = dlg.getvDegree();
            nurbsNode = node->toNurbs(nshell, narea, narcs, uDegree, vDegree);
            SFRotation rot(0, 1, 0, M_PI / 2.0);
            ((NodeNurbsSurface *)nurbsNode)->rotate(rot);
            if (TheApp->getWeights1())
                ((NodeNurbsSurface *)nurbsNode)->setWeightsTo1();
        }
    }
    if (type == VRML_SPHERE) {
        Sphere2NurbsDialog dlg(m_wnd, TheApp->getWeights1() ? 4 : 2, 
                                      TheApp->getWeights1() ? 8 : 4, 2, 2);
        if (dlg.DoModal() == IDOK) {
            int narcslong = dlg.getNarcslong();
            int narcslat = dlg.getNarcslat();
            int uDegree = dlg.getuDegree();
            int vDegree = dlg.getvDegree();
            nurbsNode = node->toNurbs(narcslong,  narcslat, uDegree, vDegree);
            SFRotation rot(0, 1, 0, M_PI / 2.0);
            ((NodeNurbsSurface *)nurbsNode)->rotate(rot);
            if (TheApp->getWeights1())
                ((NodeNurbsSurface *)nurbsNode)->setWeightsTo1();
        }
    }
    if (type == VRML_BOX) {
        Box2NurbsDialog dlg(m_wnd, 3, 3, 3, 2, 2);
        if (dlg.DoModal() == IDOK) {
            bool have6Planes = dlg.getHave6Planes();
            int nuAreaPoints = dlg.getNuAreaPoints();
            int uDegree = dlg.getuDegree();
            int nvAreaPoints = dlg.getNvAreaPoints();
            int vDegree = dlg.getvDegree();
            if (have6Planes) {
                nurbsNode = node->toNurbs(nuAreaPoints, uDegree, nvAreaPoints, vDegree);
                if (nurbsNode != NULL)
                    if (parent->getType() == VRML_SHAPE) {
                        // a NurbsGroup was created, need to delete Shape node
                        Node *grandParent = parent->getParent();
                        if (isToNurbsAllowedParent(parent)) {
                            m_scene->setSelection(parent);
                            m_scene->UpdateViews(NULL, UPDATE_SELECTION);
                            parent = grandParent;
                        } else
                            return;
                    }                
            } else {
                int nzAreaPoints;
                nzAreaPoints = dlg.getNzAreaPoints();
                nurbsNode = node->toNurbs(nuAreaPoints, uDegree, 
                                          nvAreaPoints, vDegree, nzAreaPoints);
            }
        }
    }
    if (type == VRML_NURBS_CURVE) {
        NurbsCurve2NurbsSurfDialog dlg(m_wnd, node, 4, 2, 360, 
                                       NURBS_ROT_POINT_TO_POINT);
        if (dlg.DoModal() == IDOK) {
            int narcs = dlg.getNarcs();
            int pDegree = dlg.getuDegree();
            float rDegree = dlg.getrDegree();
            Vec3f P1 = dlg.getP1();
            Vec3f P2 = dlg.getP2();
            if (dlg.getFlatten())
                if (!((NodeNurbsCurve *)node)->revolveFlatten(dlg.getMethod()))
                     TheApp->PrintMessageWindowsId(
                           IDS_FLATTEN_NOT_SUPPORTED_ERROR);
            nurbsNode = node->toNurbs(narcs, pDegree, rDegree, P1, P2);
            if (nurbsNode == NULL)
                TheApp->MessageBoxId(IDS_NURBS_CURVE_CONVERT_ERROR);
            if ((dlg.getMethod() == NURBS_ROT_Y_AXIS) && dlg.getFlatten()) {
                float angle = - 0.5 * M_PI - 0.5 * DEG2RAD(rDegree);
                SFRotation rot(0, 1, 0, angle);
                ((NodeNurbsSurface *)nurbsNode)->rotate(rot);
            }
        }
        swapDiffuseEmissiveShape((NodeShape *)parent, EMISSIVE2DIFFUSE);
    }
    if (type == DUNE_SUPER_ELLIPSOID) {
        NodeSuperEllipsoid *super = (NodeSuperEllipsoid *)node;
        int uTess = super->uTessellation()->getValue();
        if (uTess == 0) {
            if (super->n2()->getValue() != 1)
                uTess = 22;
            else
                uTess = 6;
        }
        int vTess = super->vTessellation()->getValue();
        if (vTess == 0) {
            if (super->n1()->getValue() != 1)
                vTess = 20;
            else            
                vTess = 7;
        }
        Sphere2NurbsDialog dlg(m_wnd, uTess, vTess, 2, 2);
        if (dlg.DoModal() == IDOK) {
            int narcslong = dlg.getNarcslong();
            narcslong = 2 + ((narcslong - 2) / 4) * 4;
            int narcslat = dlg.getNarcslat();
            int uDegree = dlg.getuDegree();
            int vDegree = dlg.getvDegree();
            nurbsNode = node->toNurbs(narcslat,  narcslong, vDegree, uDegree);
        }
    }
    if (type == DUNE_SUPER_SHAPE) {
        NodeSuperShape *super = (NodeSuperShape *)node;
        int uTess = super->uTessellation()->getValue();
        if (uTess == 0) {
            float um = fabs(super->um()->getValue());
            uTess = 6;
            if (um * 3 > uTess)
                uTess = um * 3;
        }
        uTess = 2 + ((uTess - 2) / 4) * 4;
        int vTess = super->vTessellation()->getValue();
        if (vTess == 0) {
            vTess = 7;
            float vm = fabs(super->vm()->getValue());
            if (vm * 3 > vTess)
                vTess = vm * 3;
        } else
            vTess++;
        Sphere2NurbsDialog dlg(m_wnd, uTess, vTess, 2, 2);
        if (dlg.DoModal() == IDOK) {
            int narcslong = dlg.getNarcslong();
            narcslong = 2 + ((narcslong - 2) / 4) * 4;
            int narcslat = dlg.getNarcslat();
            int uDegree = dlg.getuDegree();
            int vDegree = dlg.getvDegree();
            super->top(new SFBool(false));
            super->bottom(new SFBool(false));
            nurbsNode = super->toNurbs(narcslong, narcslat, uDegree, vDegree);
        }
    }
    if (type == DUNE_SUPER_EXTRUSION) {
        NodeSuperExtrusion *super = (NodeSuperExtrusion *)node;
        if ((super->beginCap()->getValue() == true) ||
            (super->endCap()->getValue() == true))
            return;
        int uTess = super->superTessellation()->getValue();
        if (uTess == 0) {
            float um = fabs(super->m()->getValue());
            uTess = 6;
            if (um * 3 > uTess)
                uTess = um * 3;
        }
        uTess = 2 + ((uTess - 2) / 4) * 4;
        int vTess = super->spineTessellation()->getValue();
        if (vTess == 0)
            vTess = 8;
        Sphere2NurbsDialog dlg(m_wnd, vTess, uTess, 2, 2);
        if (dlg.DoModal() == IDOK) {
            super->beginCap(new SFBool(false));
            super->endCap(new SFBool(false));
            int narcslong = dlg.getNarcslong();
            int narcslat = dlg.getNarcslat();
            narcslat = 2 + ((narcslat - 2) / 4) * 4;
            int uDegree = dlg.getuDegree();
            int vDegree = dlg.getvDegree();
            nurbsNode = node->toNurbs(narcslat, narcslong, uDegree, vDegree);
        }
    }
    if (type == DUNE_SUPER_REVOLVER) {
        NodeSuperRevolver *super = (NodeSuperRevolver *)node;
        int uTess = super->superTessellation()->getValue();
        if (uTess == 0) {
            float um = fabs(super->m()->getValue());
            uTess = 6;
            if (um * 4 > uTess)
                uTess = um * 4;
        }
        uTess = 2 + ((uTess - 2) / 4) * 4;
        int vTess = super->nurbsTessellation()->getValue();
        if (vTess == 0)
            vTess = 8;
        Sphere2NurbsDialog dlg(m_wnd, uTess, vTess, 2, 2);
        if (dlg.DoModal() == IDOK) {
            super->pieceOfCake(new SFBool(false));
            int narcslong = dlg.getNarcslong();
            narcslong = 2 + ((narcslong - 2) / 4) * 4;
            int narcslat = dlg.getNarcslat();
            int uDegree = dlg.getuDegree();
            int vDegree = dlg.getvDegree();
            nurbsNode = node->toNurbs(narcslong, narcslat, uDegree, vDegree);
        }
    }

    if (nurbsNode != NULL) {
        if (!isToNurbsAllowedParent(parent)) 
            return;
        m_scene->setSelection(m_scene->replaceNode(node, nurbsNode));
        m_scene->UpdateViews(NULL, UPDATE_SELECTION);
    }
}

void
MainWindow::toNurbsTrimmedSurface()
{
    Node *node =m_scene->getSelection()->getNode(); 
    if (!node->hasParent())
        return;
    Node *parent = node->getParent();

    Node* nurbsNode = node->toNurbsTrimmedSurface();

    if (nurbsNode != NULL) {
        if (!isToNurbsAllowedParent(parent)) 
            return;
        m_scene->setSelection(m_scene->replaceNode(node, nurbsNode));
        m_scene->UpdateViews(NULL, UPDATE_SELECTION);
    }
}

void
MainWindow::toNurbsCurve()
{
    Node *node = m_scene->getSelection()->getNode(); 
    int type = node->getType();
    Node *parent = node->getParent();

    Node* nurbsCurveNode = NULL;

    if (type == DUNE_SUPER_EXTRUSION) {
        nurbsCurveNode = ((NodeSuperExtrusion *)node)->toNurbsCurve();
        swapDiffuseEmissiveShape((NodeShape *)parent, DIFFUSE2EMISSIVE);
    }
    if (type == DUNE_SUPER_REVOLVER) {
        nurbsCurveNode = ((NodeSuperRevolver *)node)->toNurbsCurve();
        swapDiffuseEmissiveShape((NodeShape *)parent, DIFFUSE2EMISSIVE);
    }
    if (nurbsCurveNode != NULL) {
        if (!isToNurbsAllowedParent(parent)) 
            return;
        m_scene->setSelection(m_scene->replaceNode(node, nurbsCurveNode));
    }
    if (type == DUNE_CURVE_ANIMATION) {
        NodeCurveAnimation *animation = (NodeCurveAnimation *)node;
        nurbsCurveNode = animation->toNurbsCurve();
        Node *node2 = createGeometryNode(nurbsCurveNode, true);
        parent = node2->getParent();
        m_scene->setSelection(node2);
    } else if (type == VRML_NURBS_POSITION_INTERPOLATOR) {
        NodeNurbsPositionInterpolator *interpolator = 
            (NodeNurbsPositionInterpolator *)node;
        nurbsCurveNode = interpolator->toNurbsCurve();
        Node *node2 = createGeometryNode(nurbsCurveNode, true);
        parent = node2->getParent();
        m_scene->setSelection(node2);
    } else if (type == X3D_NURBS_ORIENTATION_INTERPOLATOR) {
        NodeNurbsOrientationInterpolator *interpolator = 
            (NodeNurbsOrientationInterpolator *)node;
        nurbsCurveNode = interpolator->toNurbsCurve();
        Node *node2 = createGeometryNode(nurbsCurveNode, true);
        parent = node2->getParent();
        m_scene->setSelection(node2);
    }

    m_scene->UpdateViews(NULL, UPDATE_SELECTION);

}

void
MainWindow::toSuperRevolver()
{
    Node *node =m_scene->getSelection()->getNode(); 
    int type = node->getType();
    Node *parent = node->getParent();

    Node* superRevolverNode = NULL;

    if (type == VRML_NURBS_CURVE) {
        superRevolverNode = ((NodeNurbsCurve *)node)->toSuperRevolver();
        swapDiffuseEmissiveShape((NodeShape *)parent, EMISSIVE2DIFFUSE);
    }
    if (superRevolverNode != NULL) {
        // be carefull with sending delete commands to a commandlist
        // later commands which would use the place of the deleted node
        // would fail
        if (!isToNurbsAllowedParent(parent)) 
            return;
        m_scene->setSelection(m_scene->replaceNode(node, superRevolverNode));
        m_scene->UpdateViews(NULL, UPDATE_SELECTION);
    }
}

void
MainWindow::toSuperExtrusion()
{
    Node *node =m_scene->getSelection()->getNode(); 
    int type = node->getType();
    Node *parent = node->getParent();

    Node* superExtrusionNode = NULL;

    if (type == VRML_NURBS_CURVE) {
        superExtrusionNode = ((NodeNurbsCurve *)node)->toSuperExtrusion();
        swapDiffuseEmissiveShape((NodeShape *)parent, EMISSIVE2DIFFUSE);
    }
    if (superExtrusionNode != NULL) {
        // be carefull with sending delete commands to a commandlist
        // later commands which would use the place of the deleted node
        // would fail
        if (!isToNurbsAllowedParent(parent)) 
            return;
        m_scene->setSelection(m_scene->replaceNode(node, superExtrusionNode));
        m_scene->UpdateViews(NULL, UPDATE_SELECTION);
    }
}

void
MainWindow::toNurbsPositionInterpolator()
{
    Node *node = m_scene->getSelection()->getNode(); 
    int type = node->getType();

    Node* nurbsNode = NULL;

    if (type == VRML_NURBS_CURVE) {
        nurbsNode = ((NodeNurbsCurve *)node)->toNurbsPositionInterpolator();
        m_scene->execute(new MoveCommand(nurbsNode, NULL, -1, 
                                         m_scene->getRoot(), 
                                         m_scene->getRootField()));
        m_scene->setSelection(nurbsNode);
    } 
    m_scene->UpdateViews(NULL, UPDATE_SELECTION);
}

void
MainWindow::toNurbsOrientationInterpolator()
{
    Node *node = m_scene->getSelection()->getNode(); 
    int type = node->getType();

    Node* nurbsNode = NULL;

    if (type == VRML_NURBS_CURVE) {
        nurbsNode = ((NodeNurbsCurve *)node)->toNurbsOrientationInterpolator();
        m_scene->execute(new MoveCommand(nurbsNode, NULL, -1, 
                                         m_scene->getRoot(), 
                                         m_scene->getRootField()));
        m_scene->setSelection(nurbsNode);
    } 
    m_scene->UpdateViews(NULL, UPDATE_SELECTION);
}

void
MainWindow::toExtrusion()
{
    Node *node =m_scene->getSelection()->getNode(); 

    if (node->canConvertToExtrusion()) {
        Node *extrusionNode = node->toExtrusion();
        if (extrusionNode != NULL) {
            m_scene->setSelection(m_scene->replaceNode(node, extrusionNode));
            m_scene->UpdateViews(NULL, UPDATE_SELECTION);
        }
    }
}

void
MainWindow::toIndexedFaceSet()
{
    Node *node =m_scene->getSelection()->getNode(); 

    if (node->canConvertToIndexedFaceSet()) {
        bool wantNormals = true;
        bool wantTessellation = false;
        bool wantTriangulation = true;
        if (node->getType() == VRML_NURBS_SURFACE)
            wantNormals = false;
        if (node->getType() == X3D_NURBS_TRIMMED_SURFACE)
            wantNormals = false;
        if (node->getType() == DUNE_SUPER_ELLIPSOID)
            wantNormals = false;
        if (node->getType() == DUNE_SUPER_SHAPE)
            wantNormals = false;
        if (node->getType() == DUNE_SUPER_REVOLVER)
            wantNormals = false;
        if (node->getType() == VRML_EXTRUSION)
            wantNormals = false;
        if (node->getType() == VRML_ELEVATION_GRID)
            wantNormals = false;
        if (node->getType() == VRML_BOX)
            wantNormals = false;
        if (node->getType() == VRML_CONE)
            wantTessellation = true;
        if (node->getType() == VRML_CYLINDER) {
            wantNormals = false;
            wantTessellation = true;
        }
        if (node->getType() == VRML_SPHERE) {
            wantNormals = false;
            wantTessellation = true;
        }
        if (node->getType() == VRML_NURBS_SURFACE) {
            ToIndexedFaceSetDialog dlg(m_wnd);
            if (dlg.DoModal() == IDCANCEL)
                return;
            wantTriangulation = dlg.getTriangulation();
        }
        if (node->getType() == DUNE_SUPER_ELLIPSOID) {
            ToIndexedFaceSetDialog dlg(m_wnd);
            if (dlg.DoModal() == IDCANCEL)
                return;
            wantTriangulation = dlg.getTriangulation();
        }
        if (wantTessellation) {
            OneIntDialog dlg(m_wnd, IDD_TESSELLATION, 
                             TheApp->getTessellation(), 4, 64);
            if (dlg.DoModal() == IDCANCEL)
                return;
            TheApp->setTessellation(dlg.GetValue());
            ((MeshBasedNode *)node)->createMesh();
        }
        int meshFlags = 0;
        if (wantNormals)
            meshFlags = MESH_WANT_NORMAL;
        node->setMeshDirty();
        Node *faceSetNode = node->toIndexedFaceSet(meshFlags, true, 
                                                   wantTriangulation);
        if (faceSetNode != NULL) {            
            m_scene->setSelection(m_scene->replaceNode(node, faceSetNode));
            m_scene->UpdateViews(NULL, UPDATE_SELECTION);
        }
        if (wantTessellation)
            TheApp->setTessellation(DEFAULT_TESSELLATION);
    }
}

void
MainWindow::toIndexedTriangleSet()
{
    Node *node = m_scene->getSelection()->getNode(); 

    if (node->canConvertToIndexedTriangleSet()) {
        Node *triangleSetNode = node->toIndexedTriangleSet();
        if (triangleSetNode != NULL) {            
            m_scene->setSelection(m_scene->replaceNode(node, triangleSetNode));
            m_scene->UpdateViews(NULL, UPDATE_SELECTION);
        }
    }
}

void
MainWindow::toTriangleSet()
{
    Node *node =m_scene->getSelection()->getNode(); 

    if (node->canConvertToTriangleSet()) {
        Node *triangleSetNode = node->toTriangleSet();
        if (triangleSetNode != NULL) {
            m_scene->setSelection(m_scene->replaceNode(node, triangleSetNode));
            m_scene->UpdateViews(NULL, UPDATE_SELECTION);
        }
    }
}

void
MainWindow::toIndexedFaceSetFromMesh()
{
    Node *node = m_scene->getSelection()->getNode();
    if (!node->hasParent())
         return;        
    Node *parent = node->getParent();
    int parentField = node->getParentField();
    if (parentField == -1)
         return;        
    if (node->isMesh()) {
        MeshBasedNode *mesh = (MeshBasedNode *)node;
        mesh->simpleQuadTriangulateMesh();
        Node *newNode = mesh->toIndexedFaceSet();
        MoveCommand *command = new MoveCommand(node, parent, parentField, 
                                               NULL, -1);
        command->execute();            
        command = new MoveCommand(newNode, NULL, -1, parent, parentField);
        command->execute();            
        m_scene->UpdateViews(NULL, UPDATE_ALL, NULL);
        m_scene->setSelection(newNode->getPath());
        m_scene->UpdateViews(NULL, UPDATE_SELECTION);
    }
}


void
MainWindow::toIndexedLineSet()
{
    Node *node =m_scene->getSelection()->getNode(); 
    int type = node->getType();

    if (node->canConvertToIndexedLineSet() || (type == VRML_INDEXED_FACE_SET)) {
        Node *lineSetNode = NULL;
        if (node->canConvertToIndexedLineSet())
            lineSetNode = node->toIndexedLineSet();
        else if (type == VRML_INDEXED_FACE_SET) {
            NodeIndexedFaceSet *faceSet = (NodeIndexedFaceSet *)node;
            Node *textureCoordinate = faceSet->texCoord()->getValue();
            if (textureCoordinate)
                m_scene->execute(new MoveCommand(textureCoordinate, faceSet, 
                                                faceSet->texCoord_Field(),
                                                NULL, -1));
            lineSetNode = faceSet->toIndexedLineSet();
        }
        if (lineSetNode != NULL) {
            m_scene->setSelection(m_scene->replaceNode(node, lineSetNode));
            m_scene->UpdateViews(NULL, UPDATE_SELECTION);
        }
    }
}

void
MainWindow::toPointSet()
{
    Node *node =m_scene->getSelection()->getNode(); 
    int type = node->getType();

    if (type == VRML_INDEXED_LINE_SET) {
        Node *pointSetNode = ((NodeIndexedLineSet *)node)->toPointSet();
        if (pointSetNode != NULL) {
            m_scene->setSelection(m_scene->replaceNode(node, pointSetNode));
            m_scene->UpdateViews(NULL, UPDATE_SELECTION);
        }
    }
}

void
MainWindow::toPositionAndOrientationInterpolators()
{
    Node *node =m_scene->getSelection()->getNode(); 

    if (node->canConvertToPositionAndOrientationInterpolators()) {
        NodeList nodeList;
        node->toPositionAndOrientationInterpolators(&nodeList);
        if (nodeList.size() != 2)
            return;

        long j = -1;
        for (j = 0; j < ((long)nodeList.size()); j++)
            if (nodeList[j] != NULL)
                m_scene->execute(new MoveCommand(nodeList[j], NULL, -1, 
                                                 m_scene->getRoot(), 
                                                 m_scene->getRootField()));
        SocketList::Iterator *i = NULL;
        CommandList *list = NULL;

        Proto *fromProto = node->getProto();

        Node *fromNode = node;
        int field1 = -1;
        int field2 = -1;
        for (j = 0; j < fromProto->getNumEventOuts(); j++)
            for (i = fromNode->getOutput(j).first(); i != NULL; i = i->next()) {
                NodePositionInterpolator *pos = (NodePositionInterpolator *)
                                                nodeList[0];
                field1 = pos->set_fraction_Field();
                if (j != node->getProto()->lookupEventOut("position_changed"))
                    continue;
                if (list == NULL)
                    list = new CommandList();
                list->append(new RouteCommand(pos, pos->value_changed_Field(),
                                              i->item().getNode(),
                                              i->item().getField()));
            }
        for (j = 0; j < fromProto->getNumEventOuts(); j++)
            for (i = fromNode->getOutput(j).first(); i != NULL; i = i->next()) {
                NodeOrientationInterpolator *rot = 
                      (NodeOrientationInterpolator *)nodeList[1];
                field2 = rot->set_fraction_Field();
                if (j != 
                    node->getProto()->lookupEventOut("orientation_changed"))
                    continue;
                if (list == NULL)
                    list = new CommandList();
                list->append(new RouteCommand(rot, rot->value_changed_Field(),
                                              i->item().getNode(),
                                              i->item().getField()));
            }
        for (j = 0; j < fromProto->getNumEventIns(); j++)
            for (i = fromNode->getInput(j).first(); i != NULL; i = i->next()) {
                if (i->item().getNode()->getType() != VRML_TIME_SENSOR)
                    continue;
                NodeTimeSensor *timer = (NodeTimeSensor *)i->item().getNode();
                if (i->item().getField() != timer->fraction_changed_Field() )
                    continue;
                if (list == NULL)
                    list = new CommandList();
                list->append(new RouteCommand(i->item().getNode(),
                                              i->item().getField(),
                                              nodeList[0], field1));
                list->append(new RouteCommand(i->item().getNode(),
                                              i->item().getField(),
                                              nodeList[1], field2));
            }
        if (list) { 
            list->execute();
            m_scene->setSelection(node);  
            m_scene->UpdateViews(NULL, UPDATE_SELECTION);
            OnEditDelete();
        }
    }
}

void
MainWindow::toPositionInterpolator()
{
    Node *node =m_scene->getSelection()->getNode(); 

    if (node->canConvertToPositionInterpolator()) {
        Node *interpolator = node->toPositionInterpolator();
        if (interpolator != NULL) {
            m_scene->execute(new MoveCommand(interpolator, NULL, -1, 
                                            m_scene->getRoot(), 
                                            m_scene->getRootField())); 
            m_scene->setSelection(interpolator);
            m_scene->UpdateViews(NULL, UPDATE_SELECTION);
        }
    }
}

void
MainWindow::toCurveAnimation()
{
    Node *node =m_scene->getSelection()->getNode(); 

    if (node->getType() == VRML_NURBS_CURVE) {
        Node *animation = node->toCurveAnimation();
        if (animation != NULL) {
            m_scene->execute(new MoveCommand(animation, NULL, -1,
                                             m_scene->getRoot(), 
                                             m_scene->getRootField()));
            m_scene->setSelection(animation);
            m_scene->UpdateViews(NULL, UPDATE_SELECTION);
        }
    }
}

void
MainWindow::toOrientationInterpolator(Direction direction)
{
    Node *node =m_scene->getSelection()->getNode(); 

    if (node->canConvertToOrientationInterpolator()) {
        Node *interpolator = node->toOrientationInterpolator(direction);
        if (interpolator != NULL) {
            m_scene->execute(new MoveCommand(interpolator, NULL, -1, 
                                            m_scene->getRoot(), 
                                            m_scene->getRootField())); 
            m_scene->setSelection(interpolator);
            m_scene->UpdateViews(NULL, UPDATE_SELECTION);
        }
    }
}

void
MainWindow::timeShift()
{
/*
    TimeShiftDialog dlg(m_wnd);
    if (dlg.DoModal() == IDOK) {
          printf("%f %d\n",dlg.getFraction(),(int)dlg.getWrapAround());
    }
*/
}

void
MainWindow::setPathAllURLs()
{
    URLDialog dlg(m_wnd);
    if (dlg.DoModal() == IDOK) {
        Node *node = m_scene->getSelection()->getNode();
        m_scene->setPathAllURL(dlg.getPath());
        m_scene->UpdateViews(NULL, UPDATE_ALL, NULL);
        m_scene->setSelection(node);
        m_scene->UpdateViews(NULL, UPDATE_SELECTION);
    }
}

void
MainWindow::insertAudioClip()
{
    Node *current = m_scene->getSelection()->getNode();
    int field = -1;
    if (current->getType() == VRML_SOUND) {
        NodeSound *sound = (NodeSound *)current;
        field = sound->source_Field();
    }
        
    if (field != -1) {

        char buf[MY_MAX_PATH];
        buf[0] = '\0';
        if (TheApp->getFileDialogDir())
            while (chdir(TheApp->getFileDialogDir()) == -2);
        if (swOpenFileDialog(m_wnd, "Select Sound Data (*.wav,*.midi)",
#ifdef _WIN32
                             "Sound Files (*.wav, *.midi)\0*.wav;*.midi\0All Files (*.*)\0*.*\0\0",
#else
                             "*.[wm][ai][vd]*]",
#endif
                             buf, MY_MAX_PATH)) {

            NodeAudioClip *node = (NodeAudioClip *)
                                  m_scene->createNode("AudioClip");
            URL url;
            url.FromPath(buf);
            node->url(new 
                      MFString(rewriteURL(url,url.GetDir(),m_scene->getURL())));
            m_scene->execute(new MoveCommand(node, NULL, -1, current, field));
            m_scene->setSelection(node);
            m_scene->UpdateViews(NULL, UPDATE_SELECTION);
        }
    }
}

void
MainWindow::insertImageTexture()
{
    Node *current = m_scene->getSelection()->getNode();
    int field = m_scene->getSelection()->getField();
    if (field == -1) 
        field = current->findFirstValidFieldType(TEXTURE_NODE);
    else
        if (!current->validChildType(field, TEXTURE_NODE))
            field = -1;

    if (field != -1) {
        char buf[MY_MAX_PATH];

        buf[0] = '\0';
       if (TheApp->getFileDialogDir())
           while (chdir(TheApp->getFileDialogDir()) == -2);
        if (swOpenFileDialog(m_wnd, "Select Image (*.jpg, *.png)",
#ifdef HAVE_LIBDEVIL
#ifdef _WIN32
            "All Files (*.*)\0*.*\0\0",
# else
            "*.*",
# endif
#else
#ifdef _WIN32
            "Image Files (*.gif, *.jpg, *.png)\0*.gif;*.jpg;*.png\0All Files (*.*)\0*.*\0\0",
# else
            "*.[gjp][ipn][fgg]",
# endif
#endif
                             buf, MY_MAX_PATH)) {
            NodeImageTexture *node = (NodeImageTexture *)
                                     m_scene->createNode("ImageTexture");
            URL url;
            url.FromPath(buf);
            node->url(new 
                      MFString(rewriteURL(url,url.GetDir(),m_scene->getURL())));
            m_scene->execute(new MoveCommand(node, NULL, -1, current, field));
            m_scene->setSelection(node);
            m_scene->UpdateViews(NULL, UPDATE_SELECTION);
        }
    }
}

void
MainWindow::insertMultiTexture()
{
    Node *current = m_scene->getSelection()->getNode();
    int field = m_scene->getSelection()->getField();
    if (field == -1) 
        field = current->findFirstValidFieldType(TEXTURE_NODE);
    else
        if (!current->validChildType(field, TEXTURE_NODE))
            field = -1;

    if (field != -1) {
        if (current->getType() != X3D_MULTI_TEXTURE) {
            Node *node = m_scene->createNode("MultiTexture");
            m_scene->execute(new MoveCommand(node, NULL, -1, current, field));
            m_scene->setSelection(node);
            m_scene->UpdateViews(NULL, UPDATE_SELECTION);
        }
    }
}

void
MainWindow::insertMultiTextureCoordinate()
{
    Node *current = m_scene->getSelection()->getNode();
    int field = m_scene->getSelection()->getField();
    if (field == -1)
        field = current->findFirstValidFieldType(TEXTURE_COORDINATE_NODE);
    else 
        if (!current->validChildType(field, TEXTURE_COORDINATE_NODE))
            field = -1;

    if (field != -1) {
        if (current->getType() != X3D_MULTI_TEXTURE_TRANSFORM) {
            Node *ntexCoord = m_scene->createNode("MultiTextureCoordinate");
            if (ntexCoord != NULL) {
                MoveCommand *command = new MoveCommand(ntexCoord, NULL, -1,
                                                       current, field); 
                command->execute();
                current->setTexCoordFromMesh(ntexCoord);
                m_scene->UpdateViews(NULL, UPDATE_SELECTION);
            }
        }
    }
}

void
MainWindow::insertMultiTextureTransform()
{
    Node *current = m_scene->getSelection()->getNode();
    if (current->getType() != X3D_MULTI_TEXTURE_TRANSFORM)
        insertNode(TEXTURE_TRANSFORM_NODE, "MultiTextureTransform");
}

void
MainWindow::insertMultiGeneratedTextureCoordinate()
{
    Node *current = m_scene->getSelection()->getNode();
    if (current->getType() != KAMBI_MULTI_GENERATED_TEXTURE_COORDINATE)
        insertNode(GENERATED_TEXTURE_COORDINATE_NODE, 
                   "MultiGeneratedTextureCoordinate");
}

void MainWindow::insertRigidBody()
{
    Node *current = m_scene->getSelection()->getNode();
    int field = m_scene->getSelection()->getField();
    if (field == -1) 
        field = current->findFirstValidFieldType(X3D_RIGID_BODY);
    else
       if (!current->validChildType(field, X3D_RIGID_BODY))
           field = -1;
    
    if (current->matchNodeClass(RIGID_JOINT_NODE)) {
        X3DRigidJointNode *joint = (X3DRigidJointNode *)current;
        if (joint->body1()->getValue() == NULL)
            field = joint->body1_Field();
        else if (joint->body2()->getValue() == NULL)
            field = joint->body2_Field();
    }
    if (current->getType() == X3D_CONTACT) {
        NodeContact *contact = (NodeContact *)current;
        if (contact->body1()->getValue() == NULL)
            field = contact->body1_Field();
        else if (contact->body2()->getValue() == NULL)
            field = contact->body2_Field();
    }

    if (field != -1) {
        Node *node = m_scene->createNode("RigidBody");
        m_scene->execute(new MoveCommand(node, NULL, -1, current, field));
        m_scene->setSelection(node);
        m_scene->UpdateViews(NULL, UPDATE_SELECTION);
    }
}

void
MainWindow::insertViewport()
{
    Node *node = insertNode(X3D_VIEWPORT, "Viewport");
    if (node == NULL)
        insertNode("Viewport");
}

void
MainWindow::insertVrmlScene()
{
    insertNode(DUNE_VRML_SCENE, "VrmlScene");
    TheApp->PrintMessageWindowsId(IDS_NOW_USE_FILE_IMPORT);
}

void
MainWindow::insertCubeTexture()
{
    Node *current = m_scene->getSelection()->getNode();
    int field = m_scene->getSelection()->getField();
    if (field == -1) 
        field = current->findValidFieldType(TEXTURE_NODE);
    else
        if (!current->validChildType(field, TEXTURE_NODE))
            field = -1;

    if (field != -1) {
        char buf[MY_MAX_PATH];

        NodeCubeTexture *node = (NodeCubeTexture *)
                                m_scene->createNode("CubeTexture");
        for (int i = 0; i < 6; i++) {
            buf[0] = '\0';
            const char *message = "";
            if (i == 0)
                message = "Select XP Image (*.jpg, *.png)";
            else if (i == 1)
                message = "Select XN Image (*.jpg, *.png)";
            else if (i == 2)
                message = "Select YP Image (*.jpg, *.png)";
            else if (i == 3)
                message = "Select YN Image (*.jpg, *.png)";
            else if (i == 4)
                message = "Select ZP Image (*.jpg, *.png)";
            else if (i == 5)
                message = "Select ZN Image (*.jpg, *.png)";
            if (TheApp->getFileDialogDir())
                while (chdir(TheApp->getFileDialogDir()) == -2);
            if (swOpenFileDialog(m_wnd, message,
#ifdef _WIN32
                "Image Files (*.jpg, *.png)\0*.jpg;*.png\0All Files (*.*)\0*.*\0\0",
#else
                "*.[gjp][ipn][fgg]",
#endif
                             buf, MY_MAX_PATH)) {
                URL url;
                url.FromPath(buf);
                MFString *mfString = new MFString(rewriteURL(url,url.GetDir(),
                                                             m_scene->getURL()));
                if (i == 0)
                   node->urlXP(mfString);
                else if (i == 1)
                   node->urlXN(mfString);
                else if (i == 2)
                   node->urlYP(mfString);
                else if (i == 3)
                   node->urlYN(mfString);
                else if (i == 4)
                   node->urlZP(mfString);
                else if (i == 5)
                   node->urlZN(mfString);
            }
        }
        m_scene->execute(new MoveCommand(node, NULL, -1, current, field));
        m_scene->setSelection(node);
        m_scene->UpdateViews(NULL, UPDATE_SELECTION);
    }
}

void
MainWindow::insertArray(void)
{
    ArrayDialog dlg(m_wnd);
    if (dlg.DoModal() == IDOK) {
        Node *node = m_scene->getSelection()->getNode();
        NodeTransform *transform = (NodeTransform *) insertNode("Transform");
        for (int i = 1; i <= dlg.getNumberCopies(); i++) {
            NodeTransform *copyTransform = (NodeTransform *)
                                           m_scene->createNode("Transform"); 
            Vec3f scale(dlg.getScale().getValue());
            Vec3f one(1,1,1);
            scale = (scale - one) * i;
            copyTransform->scale(new SFVec3f(one + scale));
            copyTransform->center(new SFVec3f(dlg.getCenter()));
            Vec3f translation(dlg.getTranslation().getValue());
            copyTransform->translation(new SFVec3f(translation * i));
            SFRotation rotation(dlg.getRotation());
            rotation.setValue(3, rotation.getValue(3) * i);
            copyTransform->rotation(new SFRotation(rotation));
            m_scene->execute(new MoveCommand(copyTransform, NULL, -1, 
                                            transform,
                                            transform->children_Field()));
            m_scene->execute(new MoveCommand(node, NULL, -1, copyTransform,
                                            copyTransform->children_Field()));
        }
        m_scene->UpdateViews(NULL, UPDATE_ALL, NULL);    
        if (node->isInScene(m_scene)) {
            m_scene->setSelection(node);
            m_scene->UpdateViews(NULL, UPDATE_SELECTION);
        }
    }

}

void
MainWindow::insertInline(bool withLoadControl)
{
    char buf[MY_MAX_PATH];

    buf[0] = '\0';
    if (TheApp->getFileDialogDir())
        while (chdir(TheApp->getFileDialogDir()) == -2);    
#ifdef WIN32
    char *fileSelectorText = getFileSelectorText();
#else
    const char *fileSelectorText = "*.[wWxX][rR33][lLdD]*";
#endif
    if (swOpenFileDialog(m_wnd, "Insert Inline", fileSelectorText, buf, 
                         MY_MAX_PATH)) {
         NodeInline *node;
         if (withLoadControl)
             node = (NodeInline *)createNode("InlineLoadControl");
         else
             node = (NodeInline *)createNode("Inline");
         URL url;
         url.FromPath(buf);
         node->url(new MFString(rewriteURL(url,url.GetDir(),m_scene->getURL())));
         m_scene->readInline(node);
    }
    delete [] fileSelectorText;
}

void
MainWindow::insertMovieTexture()
{
    char buf[MY_MAX_PATH];

    buf[0] = '\0';
    if (TheApp->getFileDialogDir())
        while (chdir(TheApp->getFileDialogDir()) == -2);
    if (swOpenFileDialog(m_wnd, "Select movie (MPEG1) File (*.mpg, *.mpeg)",
#ifdef WIN32
         "MPEG1 Files (*.mpg, *.mpeg)\0*.mpg;*.mpeg\0All Files (*.*)\0*.*\0\0",
#else
         "*.mpg",
#endif
         buf, MY_MAX_PATH)) {
        Node *current = m_scene->getSelection()->getNode();
        int field = m_scene->getSelection()->getField();
        if (field == -1) 
            field = current->findValidFieldType(TEXTURE_NODE);
        if (field == -1) 
            field = current->findValidFieldType(AUDIO_CLIP_OR_MOVIE_TEXTURE_NODE);

        if (field != -1) {
            NodeMovieTexture *node = (NodeMovieTexture *)
                                     m_scene->createNode("MovieTexture");
            URL url;
            url.FromPath(buf);
            node->url(new 
                      MFString(rewriteURL(url,url.GetDir(),m_scene->getURL())));
            m_scene->execute(new MoveCommand(node, NULL, -1, current, field));
            m_scene->setSelection(node);
            m_scene->UpdateViews(NULL, UPDATE_SELECTION);
        }
    }
}

void
MainWindow::insertNormal()
{
    Node *node = m_scene->getSelection()->getNode();
    if (node != NULL) {
        m_scene->createNormal(node);
        m_scene->UpdateViews(NULL, UPDATE_SELECTION);
    }
}

void
MainWindow::insertTextureCoordinate(bool generator)
{
    Node *node = m_scene->getSelection()->getNode();
    if (node != NULL) {
        int field = m_scene->getSelection()->getField();
        if (field == -1)
            field = node->findFirstValidFieldType(TEXTURE_COORDINATE_NODE);
        else
           if (!node->validChildType(field, TEXTURE_COORDINATE_NODE))
               field = -1;
        if ((field == -1) && generator)
            field = node->findFirstValidFieldType(
                                           GENERATED_TEXTURE_COORDINATE_NODE);
        if ((field == -1) && (!TheApp->getCoverMode()) && 
            node->getType() == VRML_INDEXED_FACE_SET) {
            field = ((NodeIndexedFaceSet *)node)->texCoord_Field();
        }
        if (field == -1)
            field = node->findFirstValidFieldType(VRML_TEXTURE_COORDINATE);
        if ((field == -1) && generator)
            field = node->findFirstValidFieldType(
                                         X3D_TEXTURE_COORDINATE_GENERATOR);
        if (field != -1) {
            int type = node->getProto()->getField(field)->getType();
            // ignore Nodes with already set field
            if (type == SFNODE) { 
                SFNode *sfNode = (SFNode *) node->getField(field);
                if (sfNode->getValue() != NULL)
                    return;
            }
            Node *ntexCoord = NULL;
            if (generator) 
                ntexCoord = m_scene->createNode("TextureCoordinateGenerator");
            else
                ntexCoord = m_scene->createNode("TextureCoordinate");
            if (ntexCoord != NULL) {
                MoveCommand *command = new MoveCommand(ntexCoord, NULL, -1,
                                                       node, field); 
                command->execute();
                node->setTexCoordFromMesh(ntexCoord);
                m_scene->UpdateViews(NULL, UPDATE_SELECTION);
            }
        }
    }
}

void
MainWindow::insertHAnimJoint()
{
    Node *current = m_scene->getSelection()->getNode();
    if (current->findValidFieldType(X3D_HANIM_JOINT) != -1)
        insertNode(X3D_HANIM_JOINT, "HAnimJoint");
    else if (current->findValidFieldType(HANIM_CHILD_NODE) != -1)
        insertNode(HANIM_CHILD_NODE, "HAnimJoint");
    else {    
        if (current->getType() == X3D_HANIM_HUMANOID) {
            int field = m_scene->getSelection()->getField();
            NodeHAnimHumanoid *human = (NodeHAnimHumanoid *)current;
            Node *node = m_scene->createNode("HAnimJoint");
            if (field != human->joints_Field())
                field = human->skeleton_Field();
            m_scene->execute(new MoveCommand(node, NULL, -1, current, field));
            m_scene->setSelection(node);
            m_scene->UpdateViews(NULL, UPDATE_SELECTION);
        } else if (current->getType() == VRML_COORDINATE) {
            NodeCoordinate *coord = (NodeCoordinate *)current;
            coord->selectSymetricHandles();
            int numHandles = m_scene->getSelectedHandlesSize();
            if (numHandles == 0)
                return;
            MyArray<int> handles;
            for (int i = 0; i < numHandles; i++) {
                handles.append(m_scene->getSelectedHandle(i));
            }
            HAnimJointDialog dlg(m_wnd, IDD_HANIM_JOINT, m_scene, 1); 
            if (dlg.DoModal() == IDCANCEL)
                return;
            float weight = dlg.GetWeight();
            NodeHAnimHumanoid *human = current->getHumanoid();
            if (human == NULL)
                return;
            NodeHAnimJoint *joint = (NodeHAnimJoint *)dlg.GetNode(); 
            if (joint == NULL) {
                joint = (NodeHAnimJoint *)m_scene->createNode("HAnimJoint");
                m_scene->execute(new MoveCommand(joint, NULL, -1, human, 
                                                 human->joints_Field()));
                m_scene->setSelection(joint);
                m_scene->UpdateViews(NULL, UPDATE_SELECTION);
                DefDialog dlg(m_wnd, m_scene);
                dlg.DoModal();
                m_scene->UpdateViews(NULL, UPDATE_SELECTION_NAME);
            }
            MFFloat *weights = (MFFloat *)joint->skinCoordWeight()->copy();
            MFInt32 *indices = (MFInt32 *)joint->skinCoordIndex()->copy();
            for (size_t i = 0; i < handles.size(); i++) {
                int handle = handles[i];
                if (handle >= NO_HANDLE)
                    continue;
                int f = indices->find(handle);
                if (f < 0) {
                    indices->appendSFValue(handle);
                    weights->appendSFValue(weight);
                } else
                    weights->setSFValue(f, weight);
            }
            joint->skinCoordIndex(indices);
            joint->skinCoordWeight(weights);
            if (weight < 1) {
                NodeHAnimJoint *parent = (NodeHAnimJoint *)dlg.GetParent();
                MFFloat *weights = (MFFloat *)parent->skinCoordWeight()->copy();
                MFInt32 *indices = (MFInt32 *)parent->skinCoordIndex()->copy();
                for (size_t i = 0; i < handles.size(); i++) {
                    int handle = m_scene->getSelectedHandle(i);
                    int f = indices->find(handle);
                    if (f < 0) {
                        indices->appendSFValue(handle);
                        weights->appendSFValue(1 - weight);
                    } else
                        weights->setSFValue(f, 1 - weight);
                }
                parent->skinCoordIndex(indices);
                parent->skinCoordWeight(weights);
            }    
            m_scene->UpdateViews(NULL, UPDATE_REDRAW_3D);
        }
    }
}

void
MainWindow::setHAnimJointWeight()
{
    Node *current = m_scene->getSelection()->getNode();
    if (current->getType() == VRML_COORDINATE) {
        int numHandles = m_scene->getSelectedHandlesSize();
        if (numHandles == 0)
            return;
        MyArray<int> handles;
        for (int i = 0; i < numHandles; i++) {
            int handle = m_scene->getSelectedHandle(i);
            if ((handle < 0) || (handle >= NO_HANDLE))
                continue;
            handles.append(handle);
        }
        Node *node = m_scene->getLastSelectedHAnimJointOrHumanoid();
        if (node == NULL) {
            char str[256], title[256];
            swLoadString(IDS_DUNE, title, 256);
            swLoadString(IDS_SELECT_HANIM_JOINT_FIRST, str, 256);
            swMessageBox(TheApp->mainWnd(), str, title, SW_MB_OK, SW_MB_ERROR);
            return;
        }
        HAnimJointDialog dlg(m_wnd, IDD_HANIM_JOINT, m_scene, 1, true, false); 
        if (dlg.DoModal() == IDCANCEL)
            return;
        float weight = dlg.GetWeight();
        NodeHAnimHumanoid *human = current->getHumanoid();
        if (human == NULL)
            return;
        NodeHAnimJoint *joint = (NodeHAnimJoint *)dlg.GetNode(); 
        bool newJoint = false;
        if (joint == NULL) {
            newJoint = true;
            joint = (NodeHAnimJoint *)m_scene->createNode("HAnimJoint");
            m_scene->execute(new MoveCommand(joint, NULL, -1, human, 
                                             human->joints_Field()));
            m_scene->setSelection(joint);
            m_scene->UpdateViews(NULL, UPDATE_SELECTION);
            DefDialog dlg(m_wnd, m_scene);
            dlg.DoModal();
            m_scene->UpdateViews(NULL, UPDATE_SELECTION_NAME);
        }
        NodeHAnimJoint *oldJoint = NULL;
        if (node->getType() == X3D_HANIM_HUMANOID)
            oldJoint = joint;
        else
            oldJoint = (NodeHAnimJoint *)node;
        MFFloat *oldWeights = (MFFloat *)oldJoint->skinCoordWeight()->copy();
        MFInt32 *oldIndices = (MFInt32 *)oldJoint->skinCoordIndex()->copy();
        MFFloat *weights =  new MFFloat();
        MFInt32 *indices = new MFInt32();
        if ((!newJoint) && node->getType() == X3D_HANIM_HUMANOID) {
            for (int i = 0; i < oldIndices->getSize(); i++)
                indices->appendSFValue(oldIndices->getValue(i));
            for (int i = 0; i < oldWeights->getSize(); i++)
                weights->appendSFValue(oldWeights->getValue(i));
            
        }
        if (newJoint) {
            MFFloat *newWeights = (MFFloat *)oldJoint->skinCoordWeight()->copy();
            MFInt32 *newIndices = (MFInt32 *)oldJoint->skinCoordIndex()->copy();
            for (size_t i = 0; i < handles.size(); i++) {
                int handle = handles[i];
                int index = newIndices->find(handle);
                if (index > -1) {
                    newIndices->removeSFValue(index);
                    newWeights->removeSFValue(index);
                }
            }
            oldJoint->skinCoordIndex(newIndices);
            oldJoint->skinCoordWeight(newWeights);
            for (size_t j = 0; j < handles.size(); j++) {
                int handle = handles[j];
                int index = oldIndices->find(handle);
                if (index > -1) {
                    indices->appendSFValue(handle);
                    weights->appendSFValue(weight);
               }
           }
        } else {
            for (size_t i = 0; i < handles.size(); i++) {
                int handle = handles[i];
                int f = indices->find(handle);
                if (f < 0) {
                    indices->appendSFValue(handle);
                    weights->appendSFValue(weight);
               }
            }
        }
        delete oldWeights;
        delete oldIndices;
        joint->skinCoordIndex(indices);
        joint->skinCoordWeight(weights);
        if (weight < 1) {
            NodeHAnimJoint *parent = (NodeHAnimJoint *)dlg.GetParent();
            MFFloat *weights = (MFFloat *)parent->skinCoordWeight()->copy();
            MFInt32 *indices = (MFInt32 *)parent->skinCoordIndex()->copy();
            for (size_t i = 0; i < handles.size(); i++) {
                int handle = m_scene->getSelectedHandle(i);
                if ((handle < 0) || (handle >= NO_HANDLE))
                    continue;
                int f = indices->find(handle);
                if (f < 0) {
                    indices->appendSFValue(handle);
                    weights->appendSFValue(1 - weight);
                } else
                    weights->setSFValue(f, 1 - weight);
            }
            parent->skinCoordIndex(indices);
            parent->skinCoordWeight(weights);
        }    
        m_scene->UpdateViews(NULL, UPDATE_REDRAW_3D);
    }
}

static MyArray<Node *> joints;

static bool searchJoints(Node *node, void *data)
{
    if (node->getType() == X3D_HANIM_JOINT) {
        joints.append(node);
    } 
    return true;     
}

void
MainWindow::removeHAnimJointWeight()
{
    Node *current = m_scene->getSelection()->getNode();
    if (current->getType() == VRML_COORDINATE) {
        int numHandles = m_scene->getSelectedHandlesSize();
        if (numHandles == 0)
            return;
        MyArray<int> handles;
        for (int i = 0; i < numHandles; i++) {
            int handle = m_scene->getSelectedHandle(i);
            if ((handle < 0) || (handle >= NO_HANDLE))
                continue;
            handles.append(handle);
        }
        NodeHAnimHumanoid *human = current->getHumanoid();
        if (human == NULL)
            return;
        for (int n = 0; n < human->skeleton()->getSize(); n++) {
            Node *skel = human->skeleton()->getValue(n);
            if (skel == NULL)
                return;
            joints.resize(0);
            skel->doWithBranch(searchJoints, NULL);
            for (size_t j = 0; j < joints.size(); j++) {
                NodeHAnimJoint *joint = (NodeHAnimJoint *)joints[j]; 
                bool changed = false;
                if (joint != NULL) {
                    MFFloat *weights = (MFFloat *)
                                       joint->skinCoordWeight()->copy();
                    MFInt32 *indices = (MFInt32 *)
                                       joint->skinCoordIndex()->copy();
                    for (size_t i = 0; i < handles.size(); i++) {
                        int handle = handles[i];
                        if (handle >= NO_HANDLE)
                            continue;
                        int f = indices->find(handle);
                        if (f > -1) {
                            indices->removeSFValue(f);
                            weights->removeSFValue(f);
                            changed = true;
                        } 
                   }
                   if (changed) {
                       joint->skinCoordIndex(indices);
                       joint->skinCoordWeight(weights);
                       human->update();
                   }
                }
            }
        }
    }
}

void
MainWindow::insertHAnimSegment()
{
    Node *current = m_scene->getSelection()->getNode();
    if (current->findValidFieldType(X3D_HANIM_SEGMENT) != -1)
        insertNode(X3D_HANIM_SEGMENT, "HAnimSegment");
    else {    
        if (current->getType() == X3D_HANIM_JOINT) {
            int field = m_scene->getSelection()->getField();
            NodeHAnimJoint *joint = (NodeHAnimJoint *)current;
            if (field != joint->displacers_Field()) {
                Node *node = m_scene->createNode("HAnimSegment");
                m_scene->execute(new MoveCommand(node, NULL, -1, current, 
                                                 joint->children_Field()));
                m_scene->setSelection(node);
                m_scene->UpdateViews(NULL, UPDATE_SELECTION);
            }
        }
    }
}

void
MainWindow::insertHAnimSite()
{
    Node *current = m_scene->getSelection()->getNode();
    if (current->findValidFieldType(X3D_HANIM_SITE) != -1)
        insertNode(X3D_HANIM_SITE, "HAnimSite");
    else {    
        if (current->getType() == X3D_HANIM_JOINT) {
            int field = m_scene->getSelection()->getField();
            NodeHAnimJoint *joint = (NodeHAnimJoint *)current;
            if (field != joint->displacers_Field()) {
                Node *node = m_scene->createNode("HAnimSite");
                m_scene->execute(new MoveCommand(node, NULL, -1, current, 
                                                joint->children_Field()));
                m_scene->setSelection(node);
                m_scene->UpdateViews(NULL, UPDATE_SELECTION);
            }
        }
        if (current->getType() == X3D_HANIM_HUMANOID) {
            int field = m_scene->getSelection()->getField();
            NodeHAnimHumanoid *human = (NodeHAnimHumanoid *)current;
            if (field == human->sites_Field()) {
                Node *node = m_scene->createNode("HAnimSite");
                m_scene->execute(new MoveCommand(node, NULL, -1, current, 
                                                 human->sites_Field()));
                m_scene->setSelection(node);
                m_scene->UpdateViews(NULL, UPDATE_SELECTION);
            }
            if (field == human->viewpoints_Field()) {
                Node *node = m_scene->createNode("HAnimSite");
                m_scene->execute(new MoveCommand(node, NULL, -1, current, 
                                                human->viewpoints_Field()));
                m_scene->setSelection(node);
                m_scene->UpdateViews(NULL, UPDATE_SELECTION);
            }
        }
    }
}

void
MainWindow::createVirtualAcoustics()
{
    createNode("VirtualAcoustics");
}

void
MainWindow::insertVirtualSoundSource()
{
    Node *current = m_scene->getSelection()->getNode();
    int field = current->findValidFieldType(COVER_VIRTUAL_SOUND_SOURCE);
    if (field != -1) {

        char buf[MY_MAX_PATH];
        buf[0] = '\0';
        if (TheApp->getFileDialogDir())
            while (chdir(TheApp->getFileDialogDir()) == -2);
        if (swOpenFileDialog(m_wnd, "Select Sound Wave File (*.wav, *.wave)",
#ifdef WIN32
                             "Wave Sound Files (*.wav, *.wave)\0*.wav;*.wave\0All Files (*.*)\0*.*\0\0",
#else
                             "*.wav*",
#endif
                             buf, MY_MAX_PATH)) {
            NodeVirtualSoundSource *node = (NodeVirtualSoundSource *)
                  m_scene->createNode("VirtualSoundSource");
            char *lastSeperator = strrchr(buf, swGetPathSeperator());
            char *fileName = buf;
            if (lastSeperator != NULL)
                fileName = lastSeperator + 1;
            node->source(new SFString(fileName));
            m_scene->execute(new MoveCommand(node, NULL, -1, current, field));
            m_scene->setSelection(node);
            m_scene->UpdateViews(NULL, UPDATE_SELECTION);
        }
    }
}

Node*
MainWindow::insertNode(const char *type, Node* current)
{
    if (current == NULL) {
        current = m_scene->getSelection()->getNode();
    }

    int field = -1;
    if (current == m_scene->getRoot())
        field = m_scene->getRootField();
    else {
        field = current->getParentField();
        current = current->getParent();
    }

    Node *ret = NULL;
    if (field != -1) {
        Node *node = m_scene->createNode(type);
        if (node == NULL)
            return NULL;
        if (node->matchNodeClass(RIGID_JOINT_NODE) ||
            (node->getType() == X3D_CONTACT))
            if (current->findValidField(node) == -1)
                return NULL;
        m_scene->execute(new MoveCommand(node, NULL, -1, current, field));
        m_scene->setSelection(node);
        m_scene->UpdateViews(NULL, UPDATE_SELECTION);
        ret = node;
    }
    return ret;
}

Node*
MainWindow::insertNode(int fieldType, const char *type, Node* current)
{
    if (current == NULL) {
        current = m_scene->getSelection()->getNode();
    }
    int field = m_scene->getSelection()->getField();
    if (fieldType == ROOT_NODE) {
        if (current == m_scene->getRoot())
            field = m_scene->getRootField();
        else
            return NULL;
    } else {
        if (field == -1) 
            field = current->findFirstValidFieldType(fieldType);
        else
            if (!current->validChildType(field, fieldType))
                field = -1;
    }

    Node *ret = NULL;
    if (field != -1) {
        Node *node = m_scene->createNode(type);
        if (node == NULL)
            return NULL;
        m_scene->execute(new MoveCommand(node, NULL, -1, current, field));
        m_scene->setSelection(node);
        m_scene->UpdateViews(NULL, UPDATE_SELECTION);
        ret = node;
    }
    return ret;
}

void MainWindow::toggleXrayRendering()
{
    static bool xrayRendering = false;
    xrayRendering = !xrayRendering;
    swMenuSetFlags(m_menu, ID_TOGGLE_XRAY_RENDERING, SW_MENU_CHECKED, 
                   xrayRendering ? SW_MENU_CHECKED  : 0);
    m_scene->setXrayRendering(xrayRendering);
}

void MainWindow::toggleFullScreen()
{
    m_fullScreen_enabled=!m_fullScreen_enabled;
    TheApp->SetBoolPreference("FullScreen", m_fullScreen_enabled);
    setFullScreen();
}

void MainWindow::setFullScreen()
{
    swMenuSetFlags(m_menu, ID_DUNE_VIEW_FULL_SCREEN, SW_MENU_CHECKED, 
                   m_fullScreen_enabled ? SW_MENU_CHECKED  : 0);
    swToolbarSetButtonFlags(m_standardToolbar, m_fullScreenIconPos, 
                            SW_TB_CHECKED, 
                            m_fullScreen_enabled ? SW_TB_CHECKED : 0);
    if (m_fullScreen_enabled) {
        swHideWindow(m_treeView->GetWindow());
        swHideWindow(m_fieldView->GetWindow());
        swHideWindow(m_graphView->GetWindow());
        swHideWindow(m_channelView->GetWindow());
        swMenuSetFlags(m_menu, ID_DUNE_VIEW_SCENE_TREE, SW_MENU_DISABLED, 
                       SW_MENU_DISABLED);
        swMenuSetFlags(m_menu, ID_DUNE_VIEW_SCENE_GRAPH, SW_MENU_DISABLED, 
                       SW_MENU_DISABLED);
        swMenuSetFlags(m_menu, ID_DUNE_VIEW_FIELD_VIEW, SW_MENU_DISABLED, 
                       SW_MENU_DISABLED);
        swMenuSetFlags(m_menu, ID_DUNE_VIEW_CHANNEL_VIEW, SW_MENU_DISABLED, 
                       SW_MENU_DISABLED);  
        m_toolbarWindow->ShowToolbar(m_nodeToolbar1, false);
        m_toolbarWindow->ShowToolbar(m_nodeToolbar2, false);
        m_toolbarWindow->ShowToolbar(m_nodeToolbar3, false);
        m_toolbarWindow->ShowToolbar(m_nodeToolbarVRML200x, false);
        m_toolbarWindow->ShowToolbar(m_nodeToolbarX3DComponents1, false);
        m_toolbarWindow->ShowToolbar(m_nodeToolbarX3DComponents2, false);
        m_toolbarWindow->ShowToolbar(m_nodeToolbarX3DComponents3, false);
        m_toolbarWindow->ShowToolbar(m_nodeToolbarX3DComponents4, false);
        m_toolbarWindow->ShowToolbar(m_nodeToolbarScripted, false);
        m_toolbarWindow->ShowToolbar(m_nodeToolbarCover, false);
        swMenuSetFlags(m_menu, ID_DUNE_VIEW_NODE_TOOLBAR_1, SW_MENU_DISABLED, 
                       SW_MENU_DISABLED);
        swMenuSetFlags(m_menu, ID_DUNE_VIEW_NODE_TOOLBAR_2, SW_MENU_DISABLED, 
                       SW_MENU_DISABLED);
        swMenuSetFlags(m_menu, ID_DUNE_VIEW_NODE_TOOLBAR_3, SW_MENU_DISABLED, 
                       SW_MENU_DISABLED);
        swMenuSetFlags(m_menu, ID_DUNE_VIEW_NODE_TOOLBAR_VRML200X, 
                       SW_MENU_DISABLED, SW_MENU_DISABLED);
        swMenuSetFlags(m_menu, ID_DUNE_VIEW_NODE_TOOLBAR_X3D_COMPONENTS_1, 
                       SW_MENU_DISABLED, SW_MENU_DISABLED);
        swMenuSetFlags(m_menu, ID_DUNE_VIEW_NODE_TOOLBAR_X3D_COMPONENTS_2, 
                       SW_MENU_DISABLED, SW_MENU_DISABLED);
        swMenuSetFlags(m_menu, ID_DUNE_VIEW_NODE_TOOLBAR_X3D_COMPONENTS_3, 
                       SW_MENU_DISABLED, SW_MENU_DISABLED);
        swMenuSetFlags(m_menu, ID_DUNE_VIEW_NODE_TOOLBAR_X3D_COMPONENTS_4, 
                       SW_MENU_DISABLED, SW_MENU_DISABLED);
        swMenuSetFlags(m_menu, ID_DUNE_VIEW_NODE_TOOLBAR_RIGID_BODY, 
                       SW_MENU_DISABLED, SW_MENU_DISABLED);
        swMenuSetFlags(m_menu, ID_DUNE_VIEW_NODE_TOOLBAR_SCRIPTED, 
                       SW_MENU_DISABLED, SW_MENU_DISABLED);
        swMenuSetFlags(m_menu, ID_DUNE_VIEW_NODE_TOOLBAR_COVER, 
                       SW_MENU_DISABLED, SW_MENU_DISABLED);
    } else {
        if (m_treeView->getEnabled())
            swShowWindow(m_treeView->GetWindow());
        if (m_fieldView->getEnabled())
            swShowWindow(m_fieldView->GetWindow());
        if (m_graphView->getEnabled())
            swShowWindow(m_graphView->GetWindow());
        if (m_channelView->getEnabled())
            swShowWindow(m_channelView->GetWindow());
        swMenuSetFlags(m_menu, ID_DUNE_VIEW_SCENE_TREE, SW_MENU_DISABLED, 0);
        swMenuSetFlags(m_menu, ID_DUNE_VIEW_SCENE_GRAPH, SW_MENU_DISABLED, 0);
        swMenuSetFlags(m_menu, ID_DUNE_VIEW_FIELD_VIEW, SW_MENU_DISABLED, 0);
        swMenuSetFlags(m_menu, ID_DUNE_VIEW_CHANNEL_VIEW, SW_MENU_DISABLED, 0);  
        if (m_nodeToolbar1Enabled)
            m_toolbarWindow->ShowToolbar(m_nodeToolbar1, true);
        if (m_nodeToolbar2Enabled)
            m_toolbarWindow->ShowToolbar(m_nodeToolbar2, true);
        if (m_nodeToolbar3Enabled)
            m_toolbarWindow->ShowToolbar(m_nodeToolbar3, true);
        if (m_nodeToolbarVRML200xEnabled)
            m_toolbarWindow->ShowToolbar(m_nodeToolbarVRML200x, true);
        if (m_nodeToolbarX3DComponents1Enabled)
            m_toolbarWindow->ShowToolbar(m_nodeToolbarX3DComponents1, true);
        if (m_nodeToolbarX3DComponents2Enabled)
            m_toolbarWindow->ShowToolbar(m_nodeToolbarX3DComponents2, true);
        if (m_nodeToolbarX3DComponents3Enabled)
            m_toolbarWindow->ShowToolbar(m_nodeToolbarX3DComponents3, true);
        if (m_nodeToolbarX3DComponents4Enabled)
            m_toolbarWindow->ShowToolbar(m_nodeToolbarX3DComponents4, true);
        if (m_nodeToolbarScriptedEnabled)
            m_toolbarWindow->ShowToolbar(m_nodeToolbarScripted, true);
        if (m_nodeToolbarCoverEnabled)
            m_toolbarWindow->ShowToolbar(m_nodeToolbarCover, true);
        swMenuSetFlags(m_menu, ID_DUNE_VIEW_NODE_TOOLBAR_1, SW_MENU_DISABLED,0); 
        swMenuSetFlags(m_menu, ID_DUNE_VIEW_NODE_TOOLBAR_2, SW_MENU_DISABLED,0); 
        swMenuSetFlags(m_menu, ID_DUNE_VIEW_NODE_TOOLBAR_3, SW_MENU_DISABLED,0); 
        swMenuSetFlags(m_menu, ID_DUNE_VIEW_NODE_TOOLBAR_VRML200X, 
                       SW_MENU_DISABLED, 0); 
        swMenuSetFlags(m_menu, ID_DUNE_VIEW_NODE_TOOLBAR_X3D_COMPONENTS_1, 
                       SW_MENU_DISABLED, 0); 
        swMenuSetFlags(m_menu, ID_DUNE_VIEW_NODE_TOOLBAR_X3D_COMPONENTS_2, 
                       SW_MENU_DISABLED, 0); 
        swMenuSetFlags(m_menu, ID_DUNE_VIEW_NODE_TOOLBAR_X3D_COMPONENTS_3, 
                       SW_MENU_DISABLED, 0); 
        swMenuSetFlags(m_menu, ID_DUNE_VIEW_NODE_TOOLBAR_X3D_COMPONENTS_4, 
                       SW_MENU_DISABLED, 0); 
        swMenuSetFlags(m_menu, ID_DUNE_VIEW_NODE_TOOLBAR_SCRIPTED, 
                       SW_MENU_DISABLED, 0); 
        swMenuSetFlags(m_menu, ID_DUNE_VIEW_NODE_TOOLBAR_COVER, 
                       SW_MENU_DISABLED, 0); 
    }
    m_innerPane->Layout();
    m_outerPane->Layout();
    Layout();
}

void MainWindow::toggleView(int direction, SceneView *view, int id,
                            const char *name)
{
    bool visible = !swIsVisible(view->GetWindow());
    view->setEnabled(visible); 
    swMenuSetFlags(m_menu, id, SW_MENU_CHECKED, visible ? SW_MENU_CHECKED : 0);
    if (visible) {
        swShowWindow(view->GetWindow());
    } else {
        swHideWindow(view->GetWindow());
    }
    if ((id != ID_DUNE_VIEW_FIELD_VIEW) || (!TheApp->is4Kids()))
        TheApp->SetBoolPreference(name, visible);
    m_innerPane->Layout();
}

void MainWindow::ShowView(int direction, SceneView *view, int id,
                          const char *name)
{
    bool visible = !swIsVisible(view->GetWindow());
    view->setEnabled(visible); 
    swMenuSetFlags(m_menu, id, SW_MENU_CHECKED, visible ? SW_MENU_CHECKED : 0);
    if (visible) {
        swShowWindow(view->GetWindow());
    }
    if ((id != ID_DUNE_VIEW_FIELD_VIEW) || (!TheApp->is4Kids()))
        TheApp->SetBoolPreference(name, visible);
    m_innerPane->Layout();
}

void MainWindow::toggleToolbar(ToolbarWindow *tbWin, STOOLBAR toolbar,
                               int id, const char *name)
{
    bool visible = !swIsVisible(swToolbarGetWindow(toolbar));
    if ((id == ID_DUNE_VIEW_NODE_TOOLBAR_COVER) && !TheApp->getCoverMode())
        visible = false;    
    swMenuSetFlags(m_menu, id, SW_MENU_CHECKED, visible ? SW_MENU_CHECKED : 0);
    tbWin->ShowToolbar(toolbar, visible);
    TheApp->SetBoolPreference(name, visible);
    m_outerPane->Layout();
}

void MainWindow::toggleStatusbar()
{
    SWND wnd = m_statusBar->GetWindow();
    bool visible = !swIsVisible(wnd);
    m_statusBar->setEnabled(visible); 
    swMenuSetFlags(m_menu, ID_DUNE_VIEW_STATUS_BAR,
                   SW_MENU_CHECKED, visible ? SW_MENU_CHECKED : 0);
    if (visible) {
        swShowWindow(wnd);
    } else {
        swHideWindow(wnd);
    }
    TheApp->SetBoolPreference("ShowStatusBar", visible);
    Layout();
}

STOOLBAR
MainWindow::LoadToolbar(ToolbarWindow *tbWin, int id, int count,
                        const int *buttons, int menuid, const char *prefName)
{
    STOOLBAR toolbar;
    bool defaultPref = true;
    if (strcmp(prefName, "NodeToolbarVRML200x") == 0)
        defaultPref = false;
    if (stringncmp(prefName, "NodeToolbarX3DComponents") == 0)
        defaultPref = false;
    if (strcmp(prefName, "NodeToolbarRigidBody") == 0)
        defaultPref = false;
    if (strcmp(prefName, "NodeToolbarScripted") == 0)
        defaultPref = false;
    if (strcmp(prefName, "NodeToolbarCover") == 0)
        defaultPref = false;
    bool visible = TheApp->GetBoolPreference(prefName, defaultPref);
    toolbar = tbWin->LoadToolbar(id, count, buttons);
    tbWin->ShowToolbar(toolbar, visible);
    swMenuSetFlags(m_menu, menuid, SW_MENU_CHECKED,
                   visible ? SW_MENU_CHECKED : 0);
    return toolbar;
}

STOOLBAR
MainWindow::LoadToolbar(ToolbarWindow *tbWin, int id, int count,
                        const NodeButton *buttons, int menuid, 
                        const char *prefName)
{
    int *intButtons = new int[2 * count];
    for (int i = 0; i < count; i++) {
        intButtons[i * 2] = buttons[i].type;
        intButtons[i * 2 + 1] = buttons[i].id;  
    }    
    STOOLBAR toolbar = LoadToolbar(tbWin, id, count, (const int*) intButtons, 
                                   menuid, prefName);
    delete [] intButtons;
    return toolbar;
}

void MainWindow::setColorCircleIcon()
{
    Node *current = m_scene->getSelection()->getNode();
    if (current && (current->getType() == VRML_COLOR) && 
        (m_scene->getSelectedHandlesSize() > 0)) {
        m_colorCircle_enabled = true;
        NodeColor *color = (NodeColor *)current;
        if (m_colorCircleHint)
            delete m_colorCircleHint;
        m_colorCircleHint = new FieldUpdate(color, color->color_Field(), -1);
    }
    if (current && (current->getType() == X3D_COLOR_RGBA) && 
        (m_scene->getSelectedHandlesSize() > 0)) {
        m_colorCircle_enabled = true;
        NodeColorRGBA *color = (NodeColorRGBA *)current;
        if (m_colorCircleHint)
            delete m_colorCircleHint;
        m_colorCircleHint = new FieldUpdate(color, color->color_Field(), -1);
    }
    swMenuSetFlags(m_menu, ID_COLOR_CIRCLE, SW_MENU_CHECKED, 
                          m_colorCircle_active ? SW_MENU_CHECKED : 0);
    swMenuSetFlags(m_menu, ID_COLOR_CIRCLE, SW_MENU_DISABLED, 
                          m_colorCircle_enabled ? 0 : SW_MENU_DISABLED);
    if (m_colorCircle_enabled)
       {
       swToolbarSetButtonFlags(m_standardToolbar, m_colorCircleIconPos, 
                               SW_TB_DISABLED, 0);
       swToolbarSetButtonFlags(m_standardToolbar, m_colorCircleIconPos, 
                               SW_TB_CHECKED, 
                               m_colorCircle_active ? SW_TB_CHECKED : 0);
       }
    else
       {
       swToolbarSetButtonFlags(m_standardToolbar, m_colorCircleIconPos, 
                               SW_TB_DISABLED, SW_TB_DISABLED);
       }
}
 
void MainWindow::setVertexModifierIcon()
{
    m_vertexModifier_enabled = false;
    Node *current = m_scene->getSelection()->getNode();
    if (current && (current->getType() == VRML_COORDINATE)) {
        m_vertexModifier_enabled = true;
    }
    if (current && (current->getType() == VRML_NURBS_SURFACE)) {
        m_vertexModifier_enabled = true;
    }
    swMenuSetFlags(m_menu, ID_VERTEX_MODIFIER, SW_MENU_CHECKED, 
                          m_vertexModifier_enabled ? SW_MENU_CHECKED : 0);
    swMenuSetFlags(m_menu, ID_VERTEX_MODIFIER, SW_MENU_DISABLED, 
                          m_vertexModifier_enabled ? 0 : SW_MENU_DISABLED);
    if (m_vertexModifier_enabled)
       {
       swToolbarSetButtonFlags(m_standardToolbar, m_vertexModifierIconPos, 
                               SW_TB_DISABLED, 0);
       swToolbarSetButtonFlags(m_standardToolbar, m_vertexModifierIconPos, 
                               SW_TB_CHECKED, 
                               m_vertexModifier_active ? SW_TB_CHECKED : 0);
       }
    else
       {
       swToolbarSetButtonFlags(m_standardToolbar, m_vertexModifierIconPos, 
                               SW_TB_DISABLED, SW_TB_DISABLED);
       }
}

void MainWindow::setXSymetricNurbsIcon()
{
       swMenuSetFlags(m_menu, ID_X_SYMETRIC, SW_MENU_CHECKED,
                          TheApp->GetXSymetricMode() ? SW_MENU_CHECKED : 0);
       swToolbarSetButtonFlags(m_standardToolbar, m_x_symetricIconPos,
                               SW_TB_CHECKED, 
                               TheApp->GetXSymetricMode() ? SW_TB_CHECKED : 0);
}

void MainWindow::setTMode(TMode mode)
{
    int arraySize = ARRAYSIZE(standardButtons) / 2;
    if (TheApp->is4Kids()) {
        if (TheApp->hasInputDevices())
            arraySize = ARRAYSIZE(standardButtons4KidsWithInputDevices) / 2;
        else
            arraySize = ARRAYSIZE(standardButtons4Kids) / 2;
    }
    for (int i = 0; i < arraySize; i++)
        swToolbarSetButtonFlags(m_standardToolbar, i, SW_TB_DISABLED, 0);

    if (m_scene->getTransformMode()->tmode!=mode)
       TheApp->calibrateInputDevices();
    m_scene->setTransformMode(mode);
    swMenuSetFlags(m_menu, ID_MOVE_MODE, SW_MENU_CHECKED,
                   mode == TM_TRANSLATE ? SW_MENU_CHECKED : 0);
    swMenuSetFlags(m_menu, ID_ROTATE_MODE, SW_MENU_CHECKED,
                   mode == TM_ROTATE ? SW_MENU_CHECKED : 0);
    swMenuSetFlags(m_menu, ID_SCALE_MODE, SW_MENU_CHECKED,
                   mode == TM_SCALE ? SW_MENU_CHECKED : 0);
    swMenuSetFlags(m_menu, ID_UNIFORM_SCALE_MODE, SW_MENU_CHECKED,
                   mode == TM_UNIFORM_SCALE ? SW_MENU_CHECKED : 0);
    swMenuSetFlags(m_menu, ID_CENTER_MODE, SW_MENU_CHECKED,
                   mode == TM_CENTER ? SW_MENU_CHECKED : 0);
    swMenuSetFlags(m_menu, ID_6D_MODE, SW_MENU_CHECKED,
                   mode == TM_6D ? SW_MENU_CHECKED : 0);
    swMenuSetFlags(m_menu, ID_6DLOCAL_MODE, SW_MENU_CHECKED,
                   mode == TM_6DLOCAL ? SW_MENU_CHECKED : 0);
    swMenuSetFlags(m_menu, ID_ROCKET_MODE, SW_MENU_CHECKED,
                   mode == TM_ROCKET ? SW_MENU_CHECKED : 0);
    swMenuSetFlags(m_menu, ID_HOVER_MODE, SW_MENU_CHECKED,
                   mode == TM_HOVER ? SW_MENU_CHECKED : 0);
    swToolbarSetButtonFlags(m_standardToolbar, m_moveModeIconPos, SW_TB_CHECKED,
                            mode == TM_TRANSLATE ? SW_TB_CHECKED : 0);
    swToolbarSetButtonFlags(m_standardToolbar, m_rotateModeIconPos, SW_TB_CHECKED,
                            mode == TM_ROTATE ? SW_TB_CHECKED : 0);
    swToolbarSetButtonFlags(m_standardToolbar, m_scaleModeIconPos, SW_TB_CHECKED,
                            mode == TM_SCALE ? SW_TB_CHECKED : 0);
    swToolbarSetButtonFlags(m_standardToolbar, m_uniformScaleModeIconPos, 
                            SW_TB_CHECKED,
                            mode == TM_UNIFORM_SCALE ? SW_TB_CHECKED : 0);
    swToolbarSetButtonFlags(m_standardToolbar, m_centerModeIconPos, SW_TB_CHECKED,
                            mode == TM_CENTER ? SW_TB_CHECKED : 0);
    bool inputDeviceIcons = TheApp->hasInputDevices() || !TheApp->is4Kids();
    if (TheApp->getMaxNumberAxesInputDevices()>4) {
       swToolbarSetButtonFlags(m_standardToolbar, m_sixDModeIconPos, SW_TB_CHECKED,
                               mode == TM_6D ? SW_TB_CHECKED : 0);
       swToolbarSetButtonFlags(m_standardToolbar, m_sixDLocalModeIconPos, 
                               SW_TB_CHECKED,
                               mode == TM_6DLOCAL ? SW_TB_CHECKED : 0);
    } else if (inputDeviceIcons) {
       swToolbarSetButtonFlags(m_standardToolbar, m_sixDModeIconPos, 
                               SW_TB_DISABLED, SW_TB_DISABLED);
       swToolbarSetButtonFlags(m_standardToolbar, m_sixDLocalModeIconPos, 
                               SW_TB_DISABLED, SW_TB_DISABLED);
    }
    if (TheApp->getMaxNumberAxesInputDevices()>=3) 
       swToolbarSetButtonFlags(m_standardToolbar, m_rocketModeIconPos, 
                               SW_TB_CHECKED,
                               mode == TM_ROCKET ? SW_TB_CHECKED : 0);
    else if (inputDeviceIcons)
       swToolbarSetButtonFlags(m_standardToolbar, m_rocketModeIconPos,
                               SW_TB_DISABLED, SW_TB_DISABLED);
    if (TheApp->getMaxNumberAxesInputDevices()>=2) 
       swToolbarSetButtonFlags(m_standardToolbar, m_hoverModeIconPos, 
                               SW_TB_CHECKED,
                               mode == TM_HOVER ? SW_TB_CHECKED : 0);
    else if (inputDeviceIcons)
       swToolbarSetButtonFlags(m_standardToolbar, m_hoverModeIconPos, 
                               SW_TB_DISABLED, SW_TB_DISABLED);
    
    setT2axes(m_scene->getTransformMode()->t2axes);
    m_scene->UpdateViews(NULL, UPDATE_MODE);
}

void MainWindow::setTDimension(TDimension dim)
{
    m_scene->setTransformMode(dim);
    int i=m_tDimensionStartIconPos;
    if (TheApp->getMaxNumberAxesInputDevices()>=2) 
       {
       swMenuSetFlags(m_menu, ID_3D_MODE, SW_MENU_RADIO_ITEM, SW_MENU_RADIO_ITEM);
       swMenuSetFlags(m_menu, ID_2D_MODE, SW_MENU_RADIO_ITEM, SW_MENU_RADIO_ITEM);
       swMenuSetFlags(m_menu, ID_1D_MODE, SW_MENU_RADIO_ITEM, SW_MENU_RADIO_ITEM);

      swToolbarSetButtonFlags(m_standardToolbar, i++, SW_TB_CHECKED,
                              dim == TM_3D ? SW_TB_CHECKED : 0);
      swToolbarSetButtonFlags(m_standardToolbar, i++, SW_TB_CHECKED,
                              dim == TM_2D ? SW_TB_CHECKED : 0);
      swToolbarSetButtonFlags(m_standardToolbar, i++, SW_TB_CHECKED,
                              dim == TM_1D ? SW_TB_CHECKED : 0);
       }
    else
       {
       swMenuSetFlags(m_menu, ID_3D_MODE, SW_MENU_RADIO_ITEM, SW_MENU_DISABLED);
       swMenuSetFlags(m_menu, ID_2D_MODE, SW_MENU_RADIO_ITEM, SW_MENU_DISABLED);
       swMenuSetFlags(m_menu, ID_1D_MODE, SW_MENU_RADIO_ITEM, SW_MENU_DISABLED);

      swToolbarSetButtonFlags(m_standardToolbar, i++, SW_TB_DISABLED, 
                              SW_TB_DISABLED); 
      swToolbarSetButtonFlags(m_standardToolbar, i++, SW_TB_DISABLED, 
                              SW_TB_DISABLED); 
      swToolbarSetButtonFlags(m_standardToolbar, i++, SW_TB_DISABLED, 
                              SW_TB_DISABLED);
       }

    m_scene->UpdateViews(NULL, UPDATE_MODE);
}

void MainWindow::setT2axes(T2axes axes)
{
    m_scene->setTransformMode(axes);
    bool showIcons=false;
    if ( TheApp->has2AxesInputDevices() ||
         (TheApp->has3AxesInputDevices() && 
          (m_scene->getTransformMode()->tmode==TM_ROCKET)) )
         showIcons=true;
    int i=m_t2axesStartIconPos;
    if (showIcons) {
       swMenuSetFlags(m_menu, ID_NEAR_FAR_MODE, SW_MENU_RADIO_ITEM, 
                      axes == TM_NEAR_FAR ? SW_MENU_RADIO_ITEM : 0);
       swToolbarSetButtonFlags(m_standardToolbar, i, SW_TB_DISABLED, 0);
       swToolbarSetButtonFlags(m_standardToolbar, i++, 
            SW_TB_CHECKED, axes == TM_NEAR_FAR ? SW_TB_CHECKED : 0 );
    } else {
       swMenuSetFlags(m_menu, ID_NEAR_FAR_MODE, SW_MENU_RADIO_ITEM, 
                      SW_MENU_DISABLED);
       swToolbarSetButtonFlags(m_standardToolbar, i++, 
            SW_TB_DISABLED, SW_TB_DISABLED);
    }
    if (showIcons) {
       swMenuSetFlags(m_menu, ID_UP_DOWN_MODE, SW_MENU_RADIO_ITEM, 
                      axes == TM_UP_DOWN ? SW_MENU_RADIO_ITEM : 0);
       swToolbarSetButtonFlags(m_standardToolbar, i, SW_TB_DISABLED, 0);
       swToolbarSetButtonFlags(m_standardToolbar, i++, 
            SW_TB_CHECKED, axes == TM_UP_DOWN ? SW_TB_CHECKED : 0 );
    } else {
       swMenuSetFlags(m_menu, ID_UP_DOWN_MODE, SW_MENU_RADIO_ITEM, 
                      SW_MENU_DISABLED);
       swToolbarSetButtonFlags(m_standardToolbar, i++, 
            SW_TB_DISABLED, SW_TB_DISABLED);
    }
    m_scene->UpdateViews(NULL, UPDATE_MODE);
}


int MainWindow::OnTimer()
{
    if (m_scene->isRunning()) {
        m_scene->disableMakeSimilarName();
        m_scene->updateTime();
        m_scene->enableMakeSimilarName();
        if (m_vrmlCutNode != NULL)
            ((NodeVrmlCut *)m_vrmlCutNode)->updateTime();
        return TRUE;
    } else {
        return FALSE;
    }
}

void MainWindow::Play()
{
    if (m_scene->isRunning()) {
        swKillTimer(m_timer);
        m_scene->stop();
    } else {
        int timeout = 10;
        if (m_scene->hasParticleSystem())
            timeout = 1;
        m_timer = swSetTimer(m_wnd, timeout, timerCB, this);
        m_scene->start();
    }
    swToolbarSetButtonFlags(m_vcrToolbar, 4, SW_TB_CHECKED,
                            m_scene->isRunning() ? SW_TB_CHECKED : 0);
    if (m_scene->isRecording())
        TheApp->setTrackAnimation();
    m_scene->UpdateViews(NULL, UPDATE_REDRAW_3D, NULL);
}

void MainWindow::Stop()
{
    if (m_scene->isRunning()) {
        swKillTimer(m_timer);
        m_scene->stop();
        m_scene->UpdateViews(NULL, UPDATE_REDRAW, NULL);
    }
    m_scene->setRecording(false);
    if (m_vcrToolbar) {
        swToolbarSetButtonFlags(m_vcrToolbar, 4, SW_TB_CHECKED, 0);
        swToolbarSetButtonFlags(m_vcrToolbar, 3, SW_TB_CHECKED, 0);
    }
    TheApp->endTrackAnimation();
}

void MainWindow::Record()
{
    m_scene->setRecording(!m_scene->isRecording());
    swToolbarSetButtonFlags(m_vcrToolbar, 3, SW_TB_CHECKED,
                            m_scene->isRecording() ? SW_TB_CHECKED : 0);
    if (m_scene->isRunning())
        TheApp->setTrackAnimation();
}

char *
MainWindow::getFileSelectorText(void)
{
   /* use "delete" on the returned value after usage */
   StringArray descriptions;
   StringArray wildcards;
   int numFileType = 0;
   descriptions[numFileType] = "VRML (*.wrl)";
#ifdef _WIN32
   wildcards[numFileType++] = "*.wrl;*.WRL;";
#else
   wildcards[numFileType++] = "*.wrl;*.WRL;*.x3dv;*.X3DV";
#endif
#ifdef HAVE_LIBZ
   descriptions[numFileType] = "compressed VRML (*.wrz)";
   wildcards[numFileType++] = "*.wrz;*.WRZ";
#endif
   descriptions[numFileType] = "X3DV (*.x3dv)";
   wildcards[numFileType++] = "*.x3dv;*.X3DV";
#ifdef HAVE_LIBZ
   descriptions[numFileType] = "compressed X3DV (*.x3dvz)";
   wildcards[numFileType++] = "*.x3dvz;*.X3DVZ";
#endif
   descriptions[numFileType] = "X3D (*.x3d)";
   wildcards[numFileType++] = "*.x3d;*.X3D";
   int len = 0;
   for (int i = 0; i < numFileType; i++) {
      len += strlen(descriptions[i]);
      len++;
      len += strlen(wildcards[i]);
      len++;
   }
   len++;
   char *ret = new char[len];
   len = 0;
   for (int i = 0; i < numFileType; i++) {
      strcpy(ret + len, descriptions[i]);
      len += strlen(descriptions[i]);
      len++;
      strcpy(ret + len, wildcards[i]);      
      len += strlen(wildcards[i]);
      len++;
   }
   ret[len] = (char) 0;
   return ret;
}

void
MainWindow::OnFileOpen()
{
#ifndef HAVE_OPEN_IN_NEW_WINDOW
    if (!TheApp->checkSaveOldWindow())
        return;
#endif
    char path[1024];
    path[0] = '\0';
#ifdef _WIN32
    char *fileSelectorText = getFileSelectorText();
#else
    const char *fileSelectorText = "*.[wWxX][rR33][lLdD]*";
#endif
    if (TheApp->getFileDialogDir())
        while(chdir(TheApp->getFileDialogDir()) == -2);
    if (swOpenFileDialog(m_wnd, "Open", fileSelectorText, path, 1024)) {
        if (OpenFileCheck(path)) {
#ifndef HAVE_OPEN_IN_NEW_WINDOW
            TheApp->deleteOldWindow();
#endif
        }
    }
}

void
MainWindow::OnFileOpenHtml()
{
#ifndef HAVE_OPEN_IN_NEW_WINDOW
    if (!TheApp->checkSaveOldWindow())
        return;
#endif
    char path[1024];
    path[0] = '\0';
    const char *fileSelectorText = "*.html";
    if (TheApp->getFileDialogDir())
        while(chdir(TheApp->getFileDialogDir()) == -2);
    if (swOpenFileDialog(m_wnd, "Open X3DOM html", fileSelectorText, path, 
                         1024)) {
        if (OpenFileCheck(path)) {
            m_scene->storeAsHtml();
#ifndef HAVE_OPEN_IN_NEW_WINDOW
            TheApp->deleteOldWindow();
#endif
        }
    }
}

static bool urlValidate(MyString text)
{
    URL myurl(text);
    if ((strcasecmp(myurl.getScheme(), "https") == 0) ||
        (strcasecmp(myurl.getScheme(), "http") == 0) ||
        (strcasecmp(myurl.getScheme(), "ftp") == 0))
        return true;
   TheApp->MessageBoxId(IDS_URL_SHOULD_BEGIN_WITH);
   return false;
}

void
MainWindow::OnFileOpenUrl()
{
    OneTextDialog dlg(m_wnd, IDD_OPEN_URL, "",urlValidate);
    if (dlg.DoModal() == IDCANCEL)
        return;
    if (!TheApp->OpenFile(dlg.GetValue()))
        if (errno != 0)
            TheApp->MessageBoxPerror(dlg.GetValue());
}

//
// OpenFileCheck() -- open a file, or display a messagebox if there's a problem
//

bool
MainWindow::OpenFileCheck(const char *path)
{
    TheApp->SetBoolPreference("MaximizeWindows", swIsMaximized(m_wnd) != 0);
    if (!TheApp->OpenFile(path)) {
        if (errno != 0)
            TheApp->MessageBoxPerror(path);
        return false;
    }
    return true;
}

void
MainWindow::OnFileImport()
{
    bool oldX3d = m_scene->isX3d();
    Node *current = m_scene->getSelection()->getNode();
    int field = m_scene->getSelection()->getField();
    if (field == -1) 
        field = current->findValidFieldType(CHILD_NODE);
    else
       if (!current->validChildType(field, CHILD_NODE))
           field = -1;

    if (field == -1)
        current = NULL;

    char path[1024];

    path[0] = '\0';
    if (TheApp->getFileDialogDir())
        while(chdir(TheApp->getFileDialogDir()) == -2);    
    char *fileSelectorText = getFileSelectorText();
    if (swOpenFileDialog(m_wnd, "Import", fileSelectorText, path, 1024)) {
        ImportFileCheck(path, current, field);
    }        
    delete [] fileSelectorText;
    if (oldX3d && (!m_scene->isX3d()))
        m_scene->setX3d();
    else if ((!oldX3d) && m_scene->isX3d())             
        m_scene->setVrml();
}

//
// ImportFileCheck() -- import a file, or display a messagebox if there's a problem
//

void
MainWindow::ImportFileCheck(const char *path, Node *node, int field)
{
    Node *importNode = node;
    importNode = m_scene->createNode("Group");
    int importField = ((NodeGroup *)importNode)->children_Field();
    m_scene->execute(new MoveCommand(importNode, NULL, -1,
                                     m_scene->getRoot(), 
                                     m_scene->getRootField()));
    if (!TheApp->ImportFile(path, m_scene, importNode, importField))
        TheApp->MessageBoxPerror(path);
}

void 
MainWindow::checkInFile(const char *path)
{ 
    char cmd[2048];
    mysnprintf(cmd, 2047, TheApp->GetRevisionControlCheckinCommand(), path);
    setStatusText(cmd);
    if (system(cmd) != 0)
        TheApp->MessageBox(IDS_REVISION_CONTROL_COMMAND_FAILED, path);
}


bool
MainWindow::SaveFile(const char *filepath, const char *url, int writeFlags,
                     char *x3dPath)
{
    if (!(writeFlags & SKIP_SAVED_TEST)) {
        if ((writeFlags & X3DV) && m_scene->getSavedX3dv())
            return true;
        if ((writeFlags & X3D_XML) && m_scene->getSavedX3dXml())
            return true;
    }
    bool writeError = true;
    char path[1024];
    mystrncpy_secure(path,filepath,1024); 
    int f = open(path, O_WRONLY | O_CREAT, 00664);
   
    if (f != -1) {
        if (!(writeFlags & TEMP_EXPORT) && 
            TheApp->GetRevisionControlCheckinFlag())
            checkInFile(path);

        writeError = false;
        int flags = writeFlags;
        if (strstr(filepath, ".x3dv") != NULL)
            flags = flags | X3DV;
        else if (strstr(filepath, ".x3d") != NULL) {
            flags = flags & ~X3DV; 
            flags = flags | X3D_XML;        
        } else if (strstr(filepath, ".wrl") != NULL) {
            flags = flags & ~X3DV; 
            flags = flags & ~X3D_XML; 
        } else if (strstr(filepath, ".ac") != NULL)
            flags = flags & AC3D;
        else if (strstr(filepath, ".rib") != NULL)
            flags = flags & RIB;
        else if (strstr(filepath, ".dat") != NULL)
            flags = flags & LDRAW_DAT;
        if (writeFlags & KANIM) {
            MyString kanimPattern = "";
            kanimPattern += url;
            OneTextDialog dlg(m_wnd, IDD_KANIM_PATTERN, kanimPattern, 
                              textValidate);
            if (dlg.DoModal() == IDCANCEL)
                return false;
            kanimPattern = dlg.GetValue();
            if (m_scene->writeKanim(f, kanimPattern) != 0)
                writeError = true;
        } else if (flags & AC3D) {
            if (m_scene->writeAc3d(f, writeFlags & AC3D_4_RAVEN) != 0)
                writeError = true;
            else if (swTruncateClose(f))
                writeError = true;
        } else if (flags & RIB) {
            if (m_scene->writeRib(f, path) != 0)
                writeError = true;
        } else if (flags & LDRAW_DAT) {
            if (m_scene->writeLdrawDat(f, path) != 0)
                writeError = true;
            else if (swTruncateClose(f))
                writeError = true;
        } else {
            if (m_scene->write(f, url, flags, x3dPath) != 0)
                writeError = true;
            else if (swTruncateClose(f))
                writeError = true;
        }
        if (!(writeFlags & TEMP_EXPORT) && 
            TheApp->GetRevisionControlCheckinFlag())
            checkInFile(path);
    }
    if (writeError) {
        const char *errors = m_scene->getErrors();
        if (strlen(errors) > 0)
            TheApp->MessageBox(errors);
#ifndef _WIN32
        if (errno != 0)
            TheApp->MessageBoxPerror(path);
#endif
        return false;
    }
    m_scene->setSaved();
    return true;
}

bool MainWindow::OnFileSave()
{
    const char *path = m_scene->getPath();
    int writeFlags = 0;
    const char *testx3dv = strstr(path,".x3dv");
    if ((testx3dv != NULL) && 
        ((path + strlen(path) - strlen(".x3dv")) == testx3dv))
       writeFlags = X3DV;
    const char *testhtml = strstr(path,".html");
    if ((testhtml != NULL) && 
        ((path + strlen(path) - strlen(".html")) == testhtml)) {
       writeFlags = X3D_XML | X3DOM;
       m_scene->storeAsHtml();
    }
    int ret = -1;
    if (path[0] && (path[0] != '-')) {
        char lastChar = path[strlen(path) > 0 ? strlen(path) - 1 : 0];
        if (lastChar == 'z')
            ret = OnFileSaveAs(m_scene->isX3d() ? X3DV | SKIP_SAVED_TEST: 0);
        else    
            ret = SaveFile(path, m_scene->getURL(), 
                           writeFlags | SKIP_SAVED_TEST);
    }
    else if (m_scene->isX3dXml())
        ret = OnFileSaveAs(X3D_XML | SKIP_SAVED_TEST);
    else
        ret = OnFileSaveAs(m_scene->isX3dv() ? X3DV | SKIP_SAVED_TEST: 0);
    if (ret == 0)
        m_scene->setSaved();
    return ret;
}

bool MainWindow::OnFileExportVRML97()
{
    return OnFileSaveAs(PURE_VRML97 | TEMP_EXPORT);
}

bool MainWindow::OnFileExportX3DV()
{
    return OnFileSaveAs(PURE_X3DV | TEMP_EXPORT);
}

bool MainWindow::OnFileExportX3DXML()
{
    return OnFileSaveAs(X3D_XML | TEMP_EXPORT | SKIP_SAVED_TEST);
}

bool MainWindow::OnFileExportX3D4Wonderland()
{
    return OnFileSaveAs(X3D_XML | X3D_4_WONDERLAND | TEMP_EXPORT);
}

bool MainWindow::OnFileExportCover()
{
    return OnFileSaveAs(PURE_VRML97 | COVER | TEMP_EXPORT);
}

bool MainWindow::OnFileExportKanim()
{
    return OnFileSaveAs(KANIM | TEMP_EXPORT);
}

bool MainWindow::OnFileExportX3dom()
{
    return OnFileSaveAs(X3DOM | X3D_XML | TEMP_EXPORT | SKIP_SAVED_TEST);
}

bool MainWindow::OnFileExportXite()
{
    return OnFileSaveAs(XITE | PURE_X3DV | TEMP_EXPORT);
}

bool MainWindow::OnFileExportC()
{
    return OnFileSaveAs(C_SOURCE | TEMP_EXPORT);
}

bool MainWindow::OnFileExportCC()
{
    return OnFileSaveAs(CC_SOURCE | TEMP_EXPORT);
}

bool MainWindow::OnFileExportJava()
{
    return OnFileSaveAs(JAVA_SOURCE | MANY_JAVA_CLASSES | TRIANGULATE | 
                        TEMP_EXPORT);
}

bool MainWindow::OnFileExportAc3d()
{
    int ravenBits = 0;
    if (TheApp->GetAc3dExport4Raven())
        ravenBits = AC3D_4_RAVEN;
    return OnFileSaveAs(AC3D | ravenBits | TEMP_EXPORT);
}

bool MainWindow::OnFileExportAc3d4Raven()
{
    return OnFileSaveAs(AC3D | AC3D_4_RAVEN | TEMP_EXPORT);
}

bool MainWindow::OnFileExportRib()
{
    return OnFileSaveAs(RIB | TEMP_EXPORT);
}

bool MainWindow::OnFileExportLdrawDat()
{
    return OnFileSaveAs(LDRAW_DAT | TEMP_EXPORT);
}

#ifdef HAVE_LIBCGAL
bool MainWindow::OnFileExportOff()
{
    Node *node = m_scene->getSelection()->getNode();
    if (node->getType() == VRML_COORDINATE) 
        node = node->getParent();
    if (node->isMeshBasedNode()) {
        NodeIndexedFaceSet *faceset = (NodeIndexedFaceSet *)node->copy();
        char path[1024] = { 0 };
        if (swSaveFileDialog(m_wnd, "Save As",
#ifdef _WIN32
                             "off (.off)\0*.off;*.OFF\0All Files (*.*)\0*.*\0\0",
#else
                             "*.off",

#endif
                             path, 1024, ".off")) {
            faceset->writeOff(path);
            ((Node *)faceset)->unref();
            return true;
        } else
            ((Node *)faceset)->unref();
    } else {
        char str[256], title[256], message[256];
        swLoadString(IDS_DUNE, title, 255);
        swLoadString(IDS_SELECT_A_INDEXEDFACESET, str, 255);
        mysnprintf(message, 255, str);
        swMessageBox(m_wnd, message, title, SW_MB_OK, SW_MB_WARNING);
    }
    return false;
}

bool MainWindow::OnFileImportOff()
{
    char path[1024] = { 0 };
    if (swOpenFileDialog(m_wnd, "Select OFF (*.off)", "*.off\0\0", path, 
                         1024)) {
        NodeIndexedFaceSet *faceset = (NodeIndexedFaceSet *)
                                      m_scene->createNode("IndexedFaceSet");
        faceset = faceset->readOff(path);
        if (faceset) {
            createGeometryNode(faceset);
            return true;
        }
    }
    return false;
}
#endif

bool MainWindow::OnFileSaveAs(int writeFlags)
{
    char path[1024] = { 0 };
    char x3dPath[1024] = { 0 };

    path[0] = '\0';
    if (writeFlags & (X3DV | PURE_X3DV)) {
        strcpy(path,"Untitled.x3dv");
    }
    else if (writeFlags & PURE_VRML97) 
        strcpy(path,"PureVrmlExport.wrl");
    else if (writeFlags & KANIM) 
        strcpy(path,"KanimExport.kanim");
    else if (writeFlags & X3DOM) 
        strcpy(path,"X3domExport.html");
    else if (writeFlags & C_SOURCE) 
        strcpy(path,"CExport.c");
    else if (writeFlags & CC_SOURCE) 
        strcpy(path,"C++Export.cc");
    else if (writeFlags & JAVA_SOURCE) 
        strcpy(path,"JavaExport.java");
    else if (writeFlags & AC3D) 
        strcpy(path,"Ac3dExport.ac");
    else if (writeFlags & RIB) 
        strcpy(path,"RibExport.rib");
    else if (writeFlags & LDRAW_DAT)
        strcpy(path,"LdrawExport.dat");
    else if (writeFlags & X3D_XML)
        strcpy(path,"Untitled.x3d");
    else
        strcpy(path,"Untitled.wrl");

    struct stat fileStat;
    int numberfile = 2;
    while (stat(path, &fileStat) == 0) {
       if (writeFlags & (X3DV | PURE_X3DV))
           mysnprintf(path, 1023, "Untitled%d.x3dv", numberfile++);
       else if (writeFlags & PURE_VRML97)
           mysnprintf(path, 1023, "PureVrmlExport%d.wrl", numberfile++);
       else if (writeFlags & KANIM)
           mysnprintf(path, 1023, "KanimExport%d.kanim", numberfile++);
       else if (writeFlags & X3DOM)
           mysnprintf(path, 1023, "X3domExport%d.html", numberfile++);
       else if (writeFlags & C_SOURCE)
           mysnprintf(path, 1023, "CExport%d.c", numberfile++);
       else if (writeFlags & CC_SOURCE)
           mysnprintf(path, 1023, "C++Export%d.cc", numberfile++);
       else if (writeFlags & JAVA_SOURCE)
           mysnprintf(path, 1023, "JavaExport%d.java", numberfile++);
       else if (writeFlags & AC3D)
           mysnprintf(path, 1023, "Ac3dExport%d.ac", numberfile++);
       else if (writeFlags & RIB)
           mysnprintf(path, 1023, "RibExport%d.rib", numberfile++);
       else if (writeFlags & LDRAW_DAT)
           mysnprintf(path, 1023, "LdrawExport%d.dat", numberfile++);
       else if (writeFlags & X3D_XML)
           mysnprintf(path, 1023, "Untitled%d.x3d", numberfile++);
       else
           mysnprintf(path, 1023, "Untitled%d.wrl", numberfile++);
    }
    if (TheApp->getFileDialogDir())
        while (chdir(TheApp->getFileDialogDir()) == -2); 
    char *fileSelectorText = getFileSelectorText();
    bool save = false;
    if (writeFlags & XITE) {
        if (stat(path, &fileStat) != 0) {
            mysnprintf(path, 1023, "Untitled%d.html", numberfile);
            mysnprintf(x3dPath, 1023, "Untitled%d.x3dv", numberfile);
        }
        while (stat(path, &fileStat) == 0) {
            mysnprintf(path, 1023, "Untitled%d.html", numberfile);
            mysnprintf(x3dPath, 1023, "Untitled%d.x3dv", numberfile++);
        }
    }
    if (writeFlags & XITE) {   
        if (swSaveFileDialog(m_wnd, "Save As",
#ifdef _WIN32
                             "html (.html)\0*.html;*.HTML\0All Files (*.*)\0*.*\0\0",
#else
                            "*.html", 
#endif
                             path, 1024, ".html"))
           save = true; 
    } else if (writeFlags & (X3DV | PURE_X3DV)) {   
        if (swSaveFileDialog(m_wnd, "Save As",
#ifdef _WIN32
                             "X3DV (.x3dv)\0*.x3dv;*.X3DV\0All Files (*.*)\0*.*\0\0",
#else
                            "*.x3dv", 
#endif
                             path, 1024, ".x3dv"))
           save = true; 
    } else if (writeFlags & KANIM) {   
        if (swSaveFileDialog(m_wnd, "Save As",
#ifdef _WIN32
                             "Kambi VRML engine's animation (.kanim)\0*.kanim;*.KANIM\0All Files (*.*)\0*.*\0\0",
#else
                            "*.kanim", 
#endif
                             path, 1024, ".kanim"))
            save = true; 
    } else if (writeFlags & X3DOM) {   
        if (swSaveFileDialog(m_wnd, "Save As",
#ifdef _WIN32
                            "X3DOM html page (.html)\0*.html;*.HTML\0All Files (*.*)\0*.*\0\0",
#else
                            "*.html", 
#endif
                            path, 1024, ".html"))
            save = true; 
    } else if (writeFlags & C_SOURCE) {   
        if (swSaveFileDialog(m_wnd, "Save As",
#ifdef _WIN32
                            "C source (.c)\0*.c;*.AC\0All Files (*.*)\0*.*\0\0",
#else
                            "*.c", 
#endif
                            path, 1024, ".c"))
            save = true; 
    } else if (writeFlags & CC_SOURCE) {   
        if (swSaveFileDialog(m_wnd, "Save As",
#ifdef _WIN32
                             "C++ source (.cc)\0*.cc;*.AC\0All Files (*.*)\0*.*\0\0",
#else
                            "*.cc", 
#endif
                             path, 1024, ".cc"))
            save = true; 
    } else if (writeFlags & JAVA_SOURCE) {   
        if (swSaveFileDialog(m_wnd, "Save As",
#ifdef _WIN32
                             "java source (.java)\0*.java;*.AC\0All Files (*.*)\0*.*\0\0",
#else
                            "*.java", 
#endif
                             path, 1024, ".java"))
            save = true; 
    } else if (writeFlags & AC3D) {   
        if (swSaveFileDialog(m_wnd, "Save As",
#ifdef _WIN32
                             "ac3d (.ac)\0*.ac;*.AC\0All Files (*.*)\0*.*\0\0",
#else
                            "*.ac", 
#endif
                             path, 1024, ".ac"))
            save = true; 
    } else if (writeFlags & RIB) {   
        if (swSaveFileDialog(m_wnd, "Save As",
#ifdef _WIN32
                             "rib (.rib)\0*.rib;*.RIB\0All Files (*.*)\0*.*\0\0",
#else
                            "*.ib", 
#endif
                             path, 1024, ".rib"))
            save = true; 
    } else if (writeFlags & LDRAW_DAT) {   
        if (m_scene->validateLdrawExport())
            if (swSaveFileDialog(m_wnd, "Save As",
#ifdef _WIN32
                                 "ldraw dat (.dat)\0*.dat;*.DAT\0All Files (*.*)\0*.*\0\0",
#else
                            "*.dat", 
#endif
                                 path, 1024, ".dat"))
                save = true; 
    } else if (writeFlags & X3D_XML) {   
        if (swSaveFileDialog(m_wnd, "Save As",
#ifdef _WIN32
                             "x3d (.x3d)\0*.x3d;*.X3D\0All Files (*.*)\0*.*\0\0",
#else
                            "*.x3d", 
#endif
                             path, 1024, ".x3d"))
            save = true; 
    } else if (writeFlags & X3DV) {   
        if (swSaveFileDialog(m_wnd, "Save As",
#ifdef _WIN32
                             "x3dv (.x3dv)\0*.x3dv;*.X3DV\0All Files (*.*)\0*.*\0\0",
#else
                            "*.x3dv", 
#endif
                             path, 1024, ".x3dv"))
            save = true; 

#ifdef _WIN32
    } else if (swSaveFileDialog(m_wnd, "Save As", fileSelectorText, path, 1024,
                                ".wrl"))
#else
    } else if (swSaveFileDialog(m_wnd, "Save As", "*.wrl", path, 1024,
                                ".wrl"))
#endif
        save = true; 

    if (writeFlags & SKIP_SAVED_TEST) {
        if ((writeFlags & X3DV) && m_scene->getSavedX3dv())
            save = false;
        if ((writeFlags & X3D_XML) && m_scene->getSavedX3dXml())
            save = false;
    }
    delete [] fileSelectorText;
    if (save) {
        URL url;

        url.FromPath(path);
        bool x3d = m_scene->isX3d();
        if (SaveFile(path, url, writeFlags, x3dPath)) {
            if (!(writeFlags & TEMP_EXPORT)) {
                TheApp->AddToRecentFiles(path);
                m_scene->setPath(!(writeFlags & XITE) ? path : x3dPath);
                m_scene->setURL(!(writeFlags & XITE) ? url : x3dPath);
                UpdateTitle();
                RefreshRecentFileMenu();
                if (x3d != m_scene->isX3d())
                    m_scene->UpdateViews(NULL, UPDATE_ALL);
                else
                    m_scene->UpdateViews(NULL, UPDATE_REDRAW, NULL);
                return true;
            }
        } else
            TheApp->MessageBoxPerror(path);
        if (writeFlags & XITE) {
             if (SaveFile(x3dPath, url, writeFlags & (~XITE)))
                 TheApp->AddToRecentFiles(x3dPath);
             else
                TheApp->MessageBoxPerror(x3dPath);
        }
    }
    return false;
}

bool MainWindow::OnFileExportWonderlandModule()
{
    WonderlandModuleExportDialog dlg(m_wnd, m_scene);
    if (dlg.DoModal() == IDCANCEL)
        return true;
    
    return dlg.getSuccess();
}

bool MainWindow::OnFileExportCattGeo()
{
    CattExportDialog dlg(m_wnd);
    if (dlg.DoModal() == IDCANCEL)
        return true;
    
    return m_scene->writeCattGeo();
}

/*
bool MainWindow::OnFileExportCattGeo()
{
    char path[1024];

    path[0] = '\0';
    if (TheApp->getFileDialogDir())
        chdir(TheApp->getFileDialogDir()); 
#ifdef _WIN32
    if (swOpenFileDialog(m_wnd, "Select", "Catt GEO Material file(material.geo)\0material.geo\0\0", 
                         path, 1024)) {
#else
    if (swOpenFileDialog(m_wnd, "Select", "material.geo", path, 1024)) {
#endif
        for (int i = strlen(path); i >=0; i--)
             if (path[i] == swGetPathSelector()) {
                 path[i + 1] = '\0';
                 break;
             }
        return m_scene->writeCattGeo(path);
    }          
    return false;
}
*/

void MainWindow::OnEditCut()
{
    const Path *sel = m_scene->getSelection();
    Node *node = sel->getNode();
    Node *parent = sel->getParent();
    int parentField = sel->getParentField();

    OnEditCopy();
    m_scene->execute(new MoveCommand(node, parent, parentField, NULL, -1));
}

void MainWindow::OnEditCopy()
{
    Node *node = m_scene->getSelection()->getNode();
    if (node) {
        if (TheApp->getClipboardNode() != NULL)
           if (!node->isEqual(TheApp->getClipboardNode()))
              TheApp->getClipboardNode()->unref();
        TheApp->setClipboardNode(node->copy());
        TheApp->getClipboardNode()->ref();
        UpdateToolbarSelection();
        m_scene->copyLastSelection();
        m_scene->UpdateViews(NULL, UPDATE_SELECTION);
    }
}

void MainWindow::OnEditCopyBranchToRoot()
{
    Node *node = m_scene->getSelection()->getNode();
    if (node && (node != m_scene->getRoot())) {
        NodeArray nodeArray;
        MyArray<int> fieldArray;
        MyArray<bool> mfNodeFlagArray;
        Node *compareNode = node;
        int numNodes = 0;
        while (compareNode != m_scene->getRoot()) {
            nodeArray[numNodes] = compareNode;
            int parentField = compareNode->getParentField();
            fieldArray[numNodes] = parentField;
            mfNodeFlagArray[numNodes] = true;
            if (compareNode->hasParent()) {
                compareNode = compareNode->getParent();
                if (compareNode->getField(parentField)->getType() == SFNODE)
                    mfNodeFlagArray[numNodes] = false;
            } else
                break;            
            numNodes++;
        }
        Node *currentNode = nodeArray[0]->copy();
        currentNode->ref();
        currentNode->removeChildren();
        for (int i = 1; i < numNodes; i++) {
            Node *newNode = nodeArray[i]->copy();
            newNode->ref();
            newNode->removeChildren();
            if (mfNodeFlagArray[i - 1])
                newNode->setField(fieldArray[i - 1], new MFNode(currentNode));
            else
                newNode->setField(fieldArray[i - 1], new SFNode(currentNode));
            currentNode = newNode;
        }
        if (TheApp->getClipboardNode() != NULL)
            TheApp->getClipboardNode()->unref();
        TheApp->setClipboardNode(currentNode);
    }
}

void MainWindow::OnEditPaste()
{
    if (m_scene->getViewOfLastSelection() &&
        m_scene->getViewOfLastSelection()->canPaste()) {
        m_scene->pasteLastSelection();
        return;
    }

    Node *destNode = m_scene->getSelection()->getNode();
    int destField = m_scene->getSelection()->getField();
    
    if (TheApp->getClipboardNode() != NULL) {
        if (destField == -1) {
            destField = destNode->findValidField(TheApp->getClipboardNode());
        }
        if (destField != -1) {
            Node *node = TheApp->getClipboardNode()->copy();
            TheApp->getClipboardNode()->copyChildrenTo(node);
            if (!node->isPROTO())
                node->reInit();
            m_scene->execute(new MoveCommand(node, NULL, -1,
                                             destNode, destField));
            m_scene->setSelection(node);
            m_scene->UpdateViews(NULL, UPDATE_SELECTION);
        }
    }
}

void MainWindow::OnEditPasteSymetric(int direction)
{
    if (m_scene->getViewOfLastSelection() &&
        m_scene->getViewOfLastSelection()->canPaste()) {
        m_scene->pasteSymetricLastSelection(direction);
        return;
    }

    Node *destNode = m_scene->getSelection()->getNode();
    if (destNode->hasParent())
        destNode = destNode->getParent();
    else
        return;
    int destField = destNode->findValidField(TheApp->getClipboardNode());

    if (TheApp->getClipboardNode()) {
        if (destField != -1) {
            m_scene->getNodes()->clearFlag(NODE_FLAG_FLIPPED);
            Node *node = TheApp->getClipboardNode()->copy();
            TheApp->getClipboardNode()->copyChildrenTo(node);
            if (!node->isPROTO())
                node->reInit();
            node->flip(direction);
            m_scene->execute(new MoveCommand(node, NULL, -1,
                                             destNode, destField));
            m_scene->setSelection(node);
            m_scene->UpdateViews(NULL, UPDATE_SELECTION);
        }
    }
}


void MainWindow::OnEditDelete()
{
    Node *node = m_scene->getSelection()->getNode();
    bool deleteTwice = false;
    // libGLU crashes if a single NurbsCurve2D is not deleted with its Contour2D
    if (node && node->getType() == VRML_NURBS_CURVE_2D)
        if (node->hasParent()) {
            Node *parent = node->getParent();
            if (parent->getType() == VRML_CONTOUR_2D) {
                NodeContour2D *contour = (NodeContour2D *)parent;
                if (contour->children()->getSFSize() == 1)
                    deleteTwice = true;
           }
        }
    m_scene->deleteLastSelection();
    if (deleteTwice)
        m_scene->deleteLastSelection();
}

void MainWindow::OnEditFindAgain()
{
    Node *node = m_scene->getSelection()->getNode();

    if (node != NULL) {    
        NodeArray allNodes;
        m_scene->getRoot()->doWithBranch(getAllNodes, &allNodes);
        int positionSelection = -1;
        for (size_t i = 0; i < allNodes.size(); i++)
            if (allNodes[i] == node) {
                positionSelection = i;
                break;
            }
        searchData data(m_searchText, m_scene); 
        for (size_t i = positionSelection + 1; i < allNodes.size(); i++)
            if (!searchNode(allNodes[i], &data))
                break;
    }
}

void MainWindow::OnEditFind()
{
    Node *node = m_scene->getSelection()->getNode();
    if (node != NULL) {    
        MyString s = m_searchText;
        OneTextDialog dlg(m_wnd, IDD_TEXT, s, textValidate);
        if (dlg.DoModal() == IDCANCEL)
            return;
        m_searchText = "";
        m_searchText += dlg.GetValue();
        OnEditFindAgain();
    }
    UpdateToolbars();
}

void MainWindow::setSelectionMode(int mode)
{
    m_scene->setSelectionMode(mode);
}

void MainWindow::selectNeighbours(bool uHandles)
{
    Node *node = m_scene->getSelection()->getNode();
    OneBoolDialog dlg(m_wnd, IDD_SELECT_NEIGHBOUR, true);
    if (dlg.DoModal() == IDCANCEL)
        return;

    int numHandles = m_scene->getSelectedHandlesSize();
    if (node != NULL) {    
        if (node->getType() == VRML_NURBS_CURVE) {
            NodeNurbsCurve *nurbs = (NodeNurbsCurve *) node;
            int size = nurbs->controlPoint()->getSFSize();
            for (int i = 0; i < numHandles; i++) {
                int handle = m_scene->getSelectedHandle(i);
                if (dlg.GetValue()) {
                    if (handle < size - 1)
                        m_scene->setSelectedHandle(handle + 1);
                } else {
                    if (handle > 0)
                        m_scene->setSelectedHandle(handle - 1);
                }
            }
        } else if (node->getType() == VRML_NURBS_SURFACE) {
            NodeNurbsSurface *nurbs = (NodeNurbsSurface *) node;
            int usize = nurbs->uDimension()->getValue();
            int vsize = nurbs->vDimension()->getValue();
            for (int i = 0; i < numHandles; i++) {
                int handle = m_scene->getSelectedHandle(i);
                int u = handle % usize;
                int v = handle / usize;
                if (uHandles) { 
                    if (dlg.GetValue()) {
                        if (u < usize - 1)
                            m_scene->setSelectedHandle(u + 1 + v * usize);
                    } else {
                        if (handle > 0)
                            m_scene->setSelectedHandle(u - 1 + v * usize);
                   }
                } else {
                    if (dlg.GetValue()) {
                        if (v < vsize - 1)
                            m_scene->setSelectedHandle(u + (v + 1)* usize);
                    } else {
                        if (v > 0)
                            m_scene->setSelectedHandle(u + (v - 1) * usize);
                   }
                }
            }
        }
        m_scene->UpdateViews(NULL, UPDATE_REDRAW_3D);
    }
}

bool MainWindow::SaveModified()
{
    bool ok = true;
    char str[256], message[256], title[256];

    if (m_scene->isModified() && 
        ((!m_scene->getSavedX3dv()) && (!m_scene->getSavedX3dv()))) {
        const char *path = m_scene->getPath();
        swSetFocus(m_wnd);
        swLoadString(IDS_DUNE, title, 256);
        swLoadString(IDS_SAVE_CHANGES, str, 256);
        if (path[0]) {
            mysnprintf(message, 255, str, path);
        } else {
            mysnprintf(message, 255, str, "Untitled");
        }
        switch (swMessageBox(m_wnd, message, title, SW_MB_YESNOCANCEL,
                             SW_MB_WARNING))
        {
          case IDYES:
            ok = OnFileSave();
            break;
          case IDCANCEL:
            ok = false;
            break;
          case IDNO:
            if (TheApp->is4Kids()) {
                swLoadString(IDS_NOT_SAVED, str, 256);
                switch (swMessageBox(m_wnd, str, title, SW_MB_YESNO,
                                     SW_MB_WARNING))
                {
                  case IDYES:
                    ok = OnFileSave();
                    break;
                  case IDNO:
                    break;
                }
            }
            break;
        }
    }
    return ok;
}

void
MainWindow::setStatusText(const char *str)
{
    m_statusText = str;
    OnHighlight(NULL);
}

void
MainWindow::OnHelpOverview()
{
    swHelpBrowserHTML(TheApp->GetHelpBrowser(), m_wnd);
}

#define CATT_DOC_URL "usage_docs/dune_commandline_de.html#catt8geo"

void
MainWindow::OnHelpOverviewCatt()
{
#ifdef _WIN32
     int pathLen = 4095;
     char path[4096];
#else
    int pathLen = 4096 + strlen(CATT_DOC_URL) + 1;
    char path[pathLen];
    pathLen--;
#endif
    char buf[4096];
    
    char *url = swGetHelpUrl(TheApp->GetHelpBrowser());
    snprintf(path, 4095, "%s", url);
    char *lastSeperator = strrchr(path, swGetPathSeperator());
#ifdef _WIN32
    if (lastSeperator == NULL)
        lastSeperator = strrchr(path, '/');
#endif
    if (lastSeperator == (path + strlen(path) - 1)) {
        // seperator at end
        snprintf(path, 4095, "%s%s", url, CATT_DOC_URL);
    } else {
        char *lastDot = strrchr(url, '.');
        if (lastDot == NULL) {
            // no . (as in "something.html") => url "should" be directory
            snprintf(buf, 4095, "%s%c%s", path, '/', CATT_DOC_URL);
            strcpy(path, buf);
        } else {
            if (lastSeperator == NULL) {
                // no seperator but dot => url "should" be file
                snprintf(path, pathLen, "%s/%s", url, CATT_DOC_URL); 
            } else {
                if (lastDot > lastSeperator) {
                    *lastSeperator = 0;
                }
                snprintf(buf, 4095, "%s%c%s", path, '/', CATT_DOC_URL);
                strcpy(path, buf);
            }
        }    
    }
    
    swHelpBrowserPath(TheApp->GetHelpBrowser(), path, m_wnd);
}

void
MainWindow::OnHelpSelection()
{
    bool x3d = m_scene->isX3d();
    Node *selection = m_scene->getSelection()->getNode();
    const char *component = selection->getComponentName();
    if (strcmp(component, "DIS") == 0)
        component = "dis";
    if (strcmp(component, "CubeMapTexturing") == 0)
        component = "env_texture";
    if (strcmp(component, "EnvironmentalEffects") == 0)
        component = "enveffects";
    if (strcmp(component, "EnvironmentalSensor") == 0)
        component = "envsensor";
    if (strcmp(component, "Followers") == 0)
        component = "followers";
    if (strcmp(component, "Geometry2D") == 0)
        component = "geometry2D";
    if (strcmp(component, "Geometry3D") == 0)
        component = "geometry3D";
    if (strcmp(component, "Geospatial") == 0)
        component = "geodata";
    if (strcmp(component, "Grouping") == 0)
        component = "group";
    if (strcmp(component, "H-Anim") == 0)
        component = "hanim";
    if (strcmp(component, "Interpolation") == 0)
        component = "interp";
    if (strcmp(component, "KeyDeviceSensor") == 0)
        component = "keyboard";
    if (strcmp(component, "Layering") == 0)
        component = "layering";
    if (strcmp(component, "Layout") == 0)
        component = "layout";
    if (strcmp(component, "Lighting") == 0)
        component = "lighting";
    if (strcmp(component, "Navigation") == 0)
        component = "navigation";
    if (strcmp(component, "Networking") == 0)
        component = "networking";
    if (strcmp(component, "NURBS") == 0)
        component = "nurbs";
    if (strcmp(component, "ParticleSystems") == 0)
        component = "particle_systems";
    if (strcmp(component, "Picking") == 0)
        component = "picking";
    if (strcmp(component, "PointingDeviceSensor") == 0)
        component = "pointingsensor";
    if (strcmp(component, "Rendering") == 0)
        component = "rendering";
    if (strcmp(component, "RigidBodyPhysics") == 0)
        component = "rigid_physics";
    if (strcmp(component, "Scripting") == 0)
        component = "scripting";
    if (strcmp(component, "Shaders") == 0)
        component = "shaders";
    if (strcmp(component, "Shape") == 0)
        component = "shape";
    if (strcmp(component, "Sound") == 0)
        component = "sound";
    if (strcmp(component, "Text") == 0)
        component = "text";
    if (strcmp(component, "Texturing3D") == 0)
        component = "texture3D";
    if (strcmp(component, "Texturing") == 0)
        component = "texturing";
    if (strcmp(component, "Time") == 0)
        component = "time";
    if (strcmp(component, "EventUtilities") == 0)
        component = "utils";
    
    if (strlen(component) > 0) {
        MyString path = "";
        path += "Part01/components/";
        path += component;
        path += ".html#";
        path += selection->getProto()->getName(x3d);
        swHelpBrowserX3d(TheApp->GetHelpBrowser(), (const char*) path, m_wnd);
    } else 
        swHelpBrowserVRML(TheApp->GetHelpBrowser(),  
                          (const char*) selection->getProto()->getName(x3d), 
                          0, m_wnd);
}

void
MainWindow::OnHelpCoverSelection()
{
    swHelpBrowserVRML(TheApp->GetHelpBrowser(),  (const char*) 
                      m_scene->getSelection()->getNode()->getProto()->
                      getName(m_scene->isX3d()), 1, m_wnd);
}

void
MainWindow::removeIllegalNodes()
{
    Node *selection = m_scene->getSelection()->getNode();
    const NodeList *nodes = m_scene->getNodes();
    for (size_t i = 0; i < nodes->size(); i++) {
        Node *node = nodes->get(i);
        if (node->isInvalidChild() && (node != m_scene->getRoot())) {
            CommandList *list = new CommandList();
            Node *parent = node->getParent();
            int parentField = node->getParentField();
            list->append(new MoveCommand(node, parent, parentField, NULL, -1));
            m_scene->execute(list);
        }
    }
    m_scene->setSelection(m_scene->getRoot());
    m_scene->UpdateViews(NULL, UPDATE_ALL, NULL);    
    if (selection->isInScene(m_scene)) {
        m_scene->setSelection(selection);
        m_scene->UpdateViews(NULL, UPDATE_SELECTION);
    }
}

void
MainWindow::countPolygons()
{
    int polygons = 0;
    int primitives = 0;
    Node *node = m_scene->getSelection()->getNode();
    bool nodeFound = false;
    Node *parent = NULL;
    if (node->hasParent())
        parent = node->getParent();
    else {
        parent = m_scene->getRoot();
        nodeFound = true;
    }
    for (int i = 0; i < parent->getProto()->getNumFields(); i++) {
        if (parent->getField(i)->getType() == SFNODE) {
            Node *child = ((SFNode *) parent->getField(i))->getValue(); 
            if (child) {
                if (child == node)
                    nodeFound = true;
                if (nodeFound) {
                    polygons += child->countPolygons();
                    primitives += child->countPrimitives();
                }
            }
        } else if (parent->getField(i)->getType() == MFNODE) {
                NodeList *childList = ((MFNode *) parent->getField(i))->
                                      getValues();
                if (childList) {
                    for (size_t j = 0; j < childList->size(); j++) {
                        Node *child = childList->get(j);
                    if (child) {
                        if (child == node)
                            nodeFound = true;
                        if (nodeFound) {
                            polygons += child->countPolygons();
                            primitives += child->countPrimitives();
                        }
                    }
                }
            }
        }
    }
    char numberPolygons[256];
    swLoadString(IDS_NUMBER_POLYGONS, numberPolygons, 255);
    char numberPrimitives[256];
    swLoadString(IDS_NUMBER_PRIMITIVES, numberPrimitives, 255);
    char statusText[256];
    mysnprintf(statusText, 255, "%s: %d  %s: %d",
               numberPolygons, polygons,  
               numberPrimitives, primitives);
    setStatusText(statusText);
}

void
MainWindow::countPolygons4catt()
{
    int singleSided = 0;
    int doubleSided = 0;
    Node *node = m_scene->getSelection()->getNode();
    bool nodeFound = false;
    Node *parent = NULL;
    if (node->hasParent())
        parent = node->getParent();
    else {
        parent = m_scene->getRoot();
        nodeFound = true;
    }
    for (int i = 0; i < parent->getProto()->getNumFields(); i++) {
        if (parent->getField(i)->getType() == SFNODE) {
            Node *child = ((SFNode *) parent->getField(i))->getValue(); 
            if (child) {
                if (child == node)
                    nodeFound = true;
                if (nodeFound) {
                    if (child->isDoubleSided())
                        doubleSided += child->countPolygons2Sided();
                    else
                        singleSided += child->countPolygons1Sided();
                }
            }
        } else if (parent->getField(i)->getType() == MFNODE) {
                NodeList *childList = ((MFNode *) parent->getField(i))->
                                      getValues();
                if (childList) {
                    for (size_t j = 0; j < childList->size(); j++) {
                        Node *child = childList->get(j);
                    if (child) {
                        if (child == node)
                            nodeFound = true;
                        if (nodeFound) {
                            if (child->isDoubleSided())
                                doubleSided += child->countPolygons2Sided();
                            else
                                singleSided += child->countPolygons1Sided();
                        }
                    }
                }
            }
        }
    }

    char numberSingleSidedPolygons[256];
    swLoadString(IDS_NUMBER_SINGLE_SIDED_POLYGONS, 
                 numberSingleSidedPolygons, 255);
    char numberDoubleSidedPolygons[256];
    swLoadString(IDS_NUMBER_DOUBLE_SIDED_POLYGONS, 
                 numberDoubleSidedPolygons, 255);
    char numberPolygons[256];
    swLoadString(IDS_NUMBER_POLYGONS_4_CATT, numberPolygons, 255);

    char statusText[256]; 
    mysnprintf(statusText, 255, "%s: %d  %s: %d   %s: %d",
               numberDoubleSidedPolygons, doubleSided / 2,
               numberSingleSidedPolygons, singleSided,
               numberPolygons, singleSided + doubleSided);

    setStatusText(statusText);
}

void
MainWindow::clearStatusText()
{
    m_statusText = "";
    OnHighlight(NULL);
}

bool
MainWindow::fieldPipe(Node *node, int field, const char *pipeCommand,
                      MyArray<int> *handles)
{
    bool success = true;
    static char path_out[1024];        
    swGetTempPath(path_out, ".dune_pipe_in", ".txt", 1024);
    int f_out = open(path_out, O_WRONLY | O_CREAT,00666);
    if (f_out == -1) {
        TheApp->MessageBoxPerror(path_out);
        return false;
    }
    int writeError = false;
    FieldValue *value = NULL;
    if (handles == NULL)
        value = node->getField(field);
    else {
        value = new MFVec3f();
        if (m_scene->getSelectedHandlesSize() > 1)
            node->startSetMultiHandles();
        for (int i = 0; i < m_scene->getSelectedHandlesSize(); i++) {
            int handle = m_scene->getSelectedHandle(i);
            if ((handle < 0) || (handle >= NO_HANDLE))
                continue;
            int constrain = m_scene->getConstrain();
            int field = -1;
            Vec3f v = node->getHandle(handle, &constrain, &field);
            m_scene->backupFieldsStart();
            m_scene->backupFieldsAppend(node, field);
            m_scene->backupFieldsDone();
            ((MFVec3f *)value)->appendSFValue(v.x, v.y, v.z);
        }
        if (m_scene->getSelectedHandlesSize() > 1)
            node->endSetMultiHandles();
    }
    if (value->write4FieldPipe(f_out, 0) != 0)
        writeError = true;
    else if (swTruncateClose(f_out))
        writeError = true;
    if (writeError) {
        TheApp->MessageBoxPerror(path_out);
        return false;
    }
    static char path_in[1024];        
    swGetTempPath(path_in, ".dune_pipe_out", ".txt", 1024);
#ifndef _WIN32
    static char path_err[1024];        
    swGetTempPath(path_err, ".dune_pipe_error", ".txt", 1024);
#endif
    MyString command = "";
    command += pipeCommand;
    command += " <";
    command += path_out;
    command += " >";
    command += path_in;
#ifndef _WIN32
    command += " 2>";
    command += path_err;
#endif
    if (system((const char *) command) == 0) {
        TheApp->setImportFile(path_in);
        FILE* f_in = fopen(path_in, "rb");
        if ((value->getType() == MFNODE) || (value->getType() == SFNODE)) {
            Node *selectionNode = m_scene->getSelection()->getNode();
            m_scene->backupField(node, field);
            m_scene->parse(f_in, node, field, SCAN_FOR_BOTH);
            node->update();
            m_scene->UpdateViews(NULL, UPDATE_ALL, NULL);
            if (selectionNode->isInScene(m_scene))
                m_scene->setSelection(selectionNode);
            else
                m_scene->setSelection(m_scene->getRoot());
            m_scene->UpdateViews(NULL, UPDATE_SELECTION);
        } else {
            if (f_in == NULL)
                TheApp->MessageBoxPerror(path_out);
            else {
                bool error = false;
       
                int index = 0;
                FieldValue *newValue = value->copy();
                newValue->makeEmpty();
                while (!feof(f_in)) {
                    char line[65536];
                    if (fgets(line, 65536, f_in) == NULL)
                        break;
                    if ((line[strlen(line) - 1] == '\n') || 
                        (line[strlen(line) - 1] == '\r'))
                        line[strlen(line) - 1] = 0;
                    if ((line[strlen(line) - 1] == '\n') || 
                        (line[strlen(line) - 1] == '\r'))
                        line[strlen(line) - 1] = 0;
                    if (newValue->checkInput(line))
                        newValue->readLine(index++, line);
                    else {
                        error = true;
                        char format[256];
                        swLoadString(IDS_ERROR_IN_OUTPUT_FILE, format, 255);
                        char msg[1024];
                        mysnprintf(msg, 1023, format, index + 1, path_in);
                        TheApp->MessageBox(msg);
                        success = false;
                        break;
                    }
                }
                fclose(f_in);
                if (!error) {
                    Node *selectionNode = m_scene->getSelection()->getNode();
                    m_scene->backupField(node, field);
                    if (handles == NULL)
                        m_scene->setField(node, field, newValue);
                    else {
                       for (int i = 0; i < m_scene->getSelectedHandlesSize() &&
                            (i < index); i++) {
                            int handle = m_scene->getSelectedHandle(i);
                            if ((handle < 0) || (handle >= NO_HANDLE))
                                continue;
                            Vec3f v = ((MFVec3f *)newValue)->getVec(i);
                            node->setHandle(handle, v);
                       }
                    }
                    node->update();
                    m_scene->UpdateViews(NULL, UPDATE_ALL, NULL);
                    if (selectionNode->isInScene(m_scene))
                        m_scene->setSelection(selectionNode);
                    else
                        m_scene->setSelection(m_scene->getRoot());
                    m_scene->UpdateViews(NULL, UPDATE_SELECTION);
                }
            }
        }
    } else {
#ifndef _WIN32
        int readErrorMessage = false; 
        int f_err = open(path_err, O_RDONLY, 00666);
        #define MSG_LEN 1024
        char msg[MSG_LEN];
        if (f_err == 0)
            readErrorMessage = true;
        else {
            int chars = read(f_err, msg, MSG_LEN);
            if (chars == -1)
                readErrorMessage = true;
            else
                msg[chars < MSG_LEN ? chars : MSG_LEN - 1] = 0;
            close(f_err);
        }
        if (!readErrorMessage)
            TheApp->MessageBox(msg);
        else
#endif
        {
        success = false;
        TheApp->MessageBoxId(IDS_PIPE_FAILED);
        }
    }
    unlink(path_out);
    unlink(path_in);
#ifndef _WIN32
    unlink(path_err);
#endif
    return success;
}

void
MainWindow::pipeField(void)
{
    Node *node = NULL;
    int field = -1;
    if (m_fieldView->isFieldView()) {
        node = ((FieldView *)m_fieldView)->GetSelectedNode();
        field = ((FieldView *)m_fieldView)->GetSelectedField();
        if ((node == NULL) || (node == m_scene->getRoot()))
            return;
    }
    MyArray<int> handles;
    if (field == -1) {
        for (int i = 0; i < m_scene->getSelectedHandlesSize(); i++) {
            int handle = m_scene->getSelectedHandle(i);
            if ((handle < 0) || (handle >= NO_HANDLE))
                continue;
            int constrain = m_scene->getConstrain();
            node->getHandle(handle, &constrain, &field);
            handles.append(handle);
        }
    }     
    if (node && (field > -1)) {
        static char pipeCommand[1024];
        static bool initialise = false;
        if (!initialise) {
            pipeCommand[0] = 0;
            initialise = true;
        }
        OneTextDialog dlg(m_wnd, IDD_TEXT, pipeCommand, NULL);
        if (dlg.DoModal() == IDCANCEL)
            return;
        mysnprintf(pipeCommand, 1024, "%s", (const char *)dlg.GetValue());
        fieldPipe(node, field, pipeCommand, 
                  handles.size() == 0 ? NULL : &handles);
    }    
}

void
MainWindow::setName(int id)
{
    Node *selection = m_scene->getSelection()->getNode();
    if (selection->getType() == VRML_COMMENT)
        return;
    if (selection != m_scene->getRoot()) {
        char name[256];
        swLoadString(id, name, 255);
        m_scene->def(m_scene->getUniqueNodeName(name), selection);
        m_scene->UpdateViews(NULL, UPDATE_SELECTION_NAME);
    }
}

void
MainWindow::createText(int id)
{
    createGeometryNode("Text");
    Node *node = m_scene->getSelection()->getNode();
    if ((node) && (node->getType() == VRML_TEXT)) {
        char name[256];
        swLoadString(id, name, 255);
        NodeText *text = (NodeText *) node;
        text->string(new MFString(name));
    }
    m_scene->UpdateViews(NULL, UPDATE_SELECTION);
}

void
MainWindow::toogleMaterialName(int id)
{
    switch (id) {
      case ID_SKIP_MATERIAL_NAME_BEFORE_FIRST_UNDERSCORE:
        TheApp->SetSkipMaterialNameBeforeFirstUnderscore(
              !TheApp->SkipMaterialNameBeforeFirstUnderscore());
        break;
      case ID_SKIP_MATERIAL_NAME_AFTER_LAST_UNDERSCORE:
        TheApp->SetSkipMaterialNameAfterLastUnderscore(
              !TheApp->SkipMaterialNameAfterLastUnderscore());
        break;
    }
    UpdateToolbar(id);        
}

void
MainWindow::testInMenu()
{
    // usually this routine is unused.
    // it is only used for softwaredevelopers who want to check new 
    // things but do not want to take care about the menu mechanisms

    // printf("test, only used when HAVE_TEST_IN_MENU defined in dune.rc\n");

    // in your test routine, you possibly want something like

    //    Node *node =m_scene->getSelection()->getNode();
    //    int type = node->getType();
    //    if (type == VRML_NURBS_CURVE) {
    //        doSomeThingWith(node);
    //        m_scene->UpdateViews(NULL, UPDATE_ALL, NULL);
    //    }

    // if your test only works with VRML_NURBS_CURVE type nodes.
}

