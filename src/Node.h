/*
 * Node.h
 *
 * Copyright (C) 1999 Stephen F. White, 2005 J. "MUFTI" Scheurich
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

#include <stdio.h>
#include "Array.h"
#include "List.h"
#include "Vec3f.h"
#include "MyString.h"
#include "FieldIndex.h"
#include "Element.h"

#include "NodeFlags.h"
#include "MeshFlags.h"
#include "ac3dMaterialCallback.h"
#include "NodeArray.h"
#include "Stack.h"
#include "Matrix.h"
#include "Quaternion.h"

class FieldValue;
class Node;
class Scene;
class NodePROTO;
class Path;
class NodeList;
class EventOut;
class EventIn;
class Field;
class ExposedField;
class MyMesh;
class MFVec3f;
class MFVec2f;
class NodeHAnimHumanoid;
class SFFloat; 
class SFNode;

class RouteSocket {
public:
                RouteSocket() 
                    { 
                    m_node = NULL; 
                    m_field = -1; 
                    }
                RouteSocket(Node *node, int index) 
                    { 
                    m_node = node; 
                    m_field = index; 
                    }

    int         operator==(const RouteSocket &t)
                    { return t.m_node == m_node && t.m_field == m_field; }
 
    int         operator!=(const RouteSocket &t)
                    { return t.m_node != m_node || t.m_field != m_field; }

    Node       *getNode(void) const { return m_node; }
    int         getField(void) const { return m_field; }
protected:
    Node       *m_node;
    int         m_field;
};

typedef List<RouteSocket> SocketList;

enum parentFlag {
    PARENT_NOT_TOUCHED,
    PARENT_TOUCHED,
    PARENT_RECURSIVE
};

class Parent {
public:
                Parent() 
                    { 
                    m_node = NULL; 
                    m_field = -1;
                    m_self = NULL; 
                    m_parentFlag = PARENT_NOT_TOUCHED;
                    }
  
                Parent(Node *node, int field, Node *self) 
                    { 
                    m_node = node; 
                    m_field = field;
                    m_self = self; 
                    m_parentFlag = PARENT_NOT_TOUCHED;
                    }

    int         operator==(const Parent &t)
                    { return t.m_self == m_self; }

    int         operator!=(const Parent &t)
                    { return t.m_self != m_self; }

    Node       *m_node;
    int         m_field;
    Node       *m_self;
    parentFlag  m_parentFlag;             
};

typedef MyArray<Parent> ParentArray;

enum Constraint {
    CONSTRAIN_NONE = 0,
    CONSTRAIN_X,
    CONSTRAIN_Y,
    CONSTRAIN_Z,
    CONSTRAIN_XY,
    CONSTRAIN_YZ,
    CONSTRAIN_ZX,
    CONSTRAIN_SPHERE
};


// X3D_NODE_TYPE
enum NodeEnum {
    VRML_ANCHOR,
    VRML_APPEARANCE,
    X3D_ARC_2D,
    X3D_ARC_CLOSE_2D,
    COVER_AR_SENSOR,
    VRML_AUDIO_CLIP,
    VRML_BACKGROUND,
    X3D_BALL_JOINT,
    X3DOM_BINARY_GEOMETRY,
    VRML_BILLBOARD,
    X3DOM_BLOCK,
    KAMBI_BLEND_MODE,
    X3D_BLENDED_VOLUME_STYLE,
    X3D_BOOLEAN_FILTER,
    X3D_BOOLEAN_TOGGLE,
    X3D_BOOLEAN_TRIGGER,
    X3D_BOOLEAN_SEQUENCER,
    X3D_BOUNDARY_ENHANCEMENT_VOLUME_STYLE,
    X3D_BOUNDED_PHYSICS_MODEL,
    VRML_BOX,
    X3D_CAD_ASSEMBLY,
    X3D_CAD_FACE,
    X3D_CAD_LAYER,
    X3D_CAD_PART,
    X3D_CARTOON_VOLUME_STYLE,
    DUNE_CATT_EXPORT_REC, // Export container node for CATT 8 file format
    DUNE_CATT_EXPORT_SRC, // Export container node for CATT 8 file format
    X3D_CIRCLE_2D,
    X3D_CLIP_PLANE,
    X3D_COLLIDABLE_OFFSET,
    X3D_COLLIDABLE_SHAPE,
    VRML_COLLISION,
    X3D_COLLISION_COLLECTION,
    X3D_COLLISION_SENSOR,
    X3D_COLLISION_SPACE,
    VRML_COLOR,
    X3D_COLOR_CHASER,
    X3D_COLOR_DAMPER,
    VRML_COLOR_INTERPOLATOR,
    X3DOM_COLOR_MASK_MODE,
    X3D_COLOR_RGBA,
    KAMBI_COLOR_SET_INTERPOLATOR,
    X3DOM_COMMON_SURFACE_SHADER,
    X3D_COMPOSED_CUBE_MAP_TEXTURE,
    X3D_COMPOSED_SHADER,
    X3D_COMPOSED_TEXTURE_3D,
    X3D_COMPOSED_VOLUME_STYLE,
    VRML_CONE,
    X3D_CONE_EMITTER,
    X3D_CONTACT,
    VRML_CONTOUR_2D,
    X3D_CONTOUR_POLYLINE_2D,
    VRML_COORDINATE,
    X3D_COORDINATE_CHASER,
    X3D_COORDINATE_DAMPER,
    VRML_COORDINATE_DEFORMER,
    X3D_COORDINATE_DOUBLE,
    VRML_COORDINATE_INTERPOLATOR,
    X3D_COORDINATE_INTERPOLATOR_2D,
    COVER_COVER,
    COVER_CUBE_TEXTURE,
    KAMBI_CUBIC_BEZIER_2D_ORIENTATION_INTERPOLATOR,
    KAMBI_CUBIC_BEZIER_POSITION_INTERPOLATOR,
    DUNE_CURVE_ANIMATION,
    VRML_CYLINDER,
    VRML_CYLINDER_SENSOR,
    X3DOM_DEPTH_MODE,
    VRML_DIRECTIONAL_LIGHT,
    X3DOM_DISH,
    X3D_DIS_ENTITY_MANAGER,
    X3D_DIS_ENTITY_TYPE_MAPPING,
    X3D_DISK_2D,
    X3DOM_DYNAMIC_LOD,
    X3D_DOUBLE_AXIS_HINGE_JOINT,
    X3D_EASE_IN_EASE_OUT,
    KAMBI_EFFECT,
    KAMBI_EFFECT_PART,
    X3D_EDGE_ENHANCEMENT_VOLUME_STYLE,
    VRML_ELEVATION_GRID,
    X3DOM_ENVIRONMENT,
    X3D_ESPDU_TRANSFORM,
    X3D_EXPLOSION_EMITTER,
    X3DOM_EXTERNAL_GEOMETRY,
    VRML_EXTRUSION,
    X3DOM_FIELD,
    X3D_FILL_PROPERTIES,
    X3D_FLOAT_VERTEX_ATTRIBUTE,
    VRML_FOG,
    X3D_FOG_COORDINATE,
    VRML_FONT_STYLE,
    X3D_FORCE_PHYSICS_MODEL,
    X3D_GENERATED_CUBE_MAP_TEXTURE,
    KAMBI_GENERATED_SHADOW_MAP,
    VRML_GEO_COORDINATE,
    VRML_GEO_ELEVATION_GRID,
    VRML_GEO_LOCATION,
    VRML_GEO_LOD,
    X3D_GEO_METADATA,
    VRML_GEO_ORIGIN,
    VRML_GEO_POSITION_INTERPOLATOR,
    X3D_GEO_PROXIMITY_SENSOR,
    VRML_GEO_TOUCH_SENSOR,
    X3D_GEO_TRANSFORM,
    VRML_GEO_VIEWPOINT,
    VRML_GROUP,
    X3D_HANIM_DISPLACER,
    X3D_HANIM_HUMANOID,
    X3D_HANIM_JOINT,
    X3D_HANIM_MOTION,
    X3D_HANIM_SEGMENT,
    X3D_HANIM_SITE,
    X3D_IMAGE_CUBE_MAP_TEXTURE,
    X3DOM_IMAGE_GEOMETRY,
    VRML_IMAGE_TEXTURE,
    X3D_IMAGE_TEXTURE_3D,
    X3DOM_IMAGE_TEXTURE_ATLAS,
    VRML_INDEXED_FACE_SET,
    VRML_INDEXED_LINE_SET,
    X3D_INDEXED_QUAD_SET,
    X3D_INDEXED_TRIANGLE_FAN_SET,
    X3D_INDEXED_TRIANGLE_SET,
    X3D_INDEXED_TRIANGLE_STRIP_SET,
    VRML_INLINE,
    VRML_INLINE_LOAD_CONTROL,
    X3D_INTEGER_SEQUENCER,
    X3D_INTEGER_TRIGGER,
    X3D_ISO_SURFACE_VOLUME_DATA,
    COVER_JOYSTICK_SENSOR,
    KAMBI_KAMBI_APPEARANCE,
    KAMBI_KAMBI_HEAD_LIGHT,
    KAMBI_KAMBI_INLINE,
    KAMBI_KAMBI_NAVIGATION_INFO,
    KAMBI_KAMBI_OCTREE_PROPERTIES,
    KAMBI_KAMBI_TRIANGULATION,
    X3D_KEY_SENSOR,
    COVER_LAB_VIEW,
    X3D_LAYER,
    X3D_LAYER_SET,
    X3D_LAYOUT,
    X3D_LAYOUT_GROUP,
    X3D_LAYOUT_LAYER,
    DUNE_LDRAW_DAT_EXPORT, // Export container node for ldraw.dat file format
    X3D_LINE_PICK_SENSOR,
    X3D_LINE_PROPERTIES,
    X3D_LINE_SET,
    X3D_LOAD_SENSOR,
    X3D_LOCAL_FOG,
    VRML_LOD,
    VRML_MATERIAL,
    X3D_MATRIX_3_VERTEX_ATTRIBUTE,
    X3D_MATRIX_4_VERTEX_ATTRIBUTE,
    KAMBI_MATRIX_TRANSFORM,
    X3DOM_MESH,
    X3D_METADATA_BOOLEAN,
    X3D_METADATA_DOUBLE,
    X3D_METADATA_FLOAT,
    X3D_METADATA_INTEGER,
    X3D_METADATA_SET,
    X3D_METADATA_STRING,
    X3D_MOTOR_JOINT,
    VRML_MOVIE_TEXTURE,
    X3DOM_MPR_VOLUME_STYLE,
    KAMBI_MULTI_GENERATED_TEXTURE_COORDINATE,
    X3DOM_MULTI_PART,
    X3D_MULTI_TEXTURE,
    X3D_MULTI_TEXTURE_COORDINATE,
    X3D_MULTI_TEXTURE_TRANSFORM,
    VRML_NAVIGATION_INFO,
    VRML_NORMAL,
    VRML_NORMAL_INTERPOLATOR,
    X3DOM_NOZZLE,
    VRML_NURBS_CURVE,
    VRML_NURBS_CURVE_2D,
    VRML_NURBS_GROUP,
    X3D_NURBS_ORIENTATION_INTERPOLATOR,
    VRML_NURBS_POSITION_INTERPOLATOR,
    X3D_NURBS_SET,
    VRML_NURBS_SURFACE,
    X3D_NURBS_SURFACE_INTERPOLATOR,
    X3D_NURBS_SWEPT_SURFACE,
    X3D_NURBS_SWUNG_SURFACE,
    X3D_NURBS_TEXTURE_COORDINATE,
    VRML_NURBS_TEXTURE_SURFACE,
    X3D_NURBS_TRIMMED_SURFACE,
    X3D_OPACITY_MAP_VOLUME_STYLE,
    X3D_ORIENTATION_CHASER,
    X3D_ORIENTATION_DAMPER,
    VRML_ORIENTATION_INTERPOLATOR,
    X3D_ORTHO_VIEWPOINT,
    X3D_PACKAGED_SHADER,
    X3DOM_PARAM,
    X3DOM_PARTICLE_SET,
    X3D_PARTICLE_SYSTEM,
    X3D_PICKABLE_GROUP,
    VRML_PIXEL_TEXTURE,
    X3D_PIXEL_TEXTURE_3D,
    X3DOM_PLANE,
    VRML_PLANE_SENSOR,
    X3D_POINT_EMITTER,
    VRML_POINT_LIGHT,
    X3D_POINT_PICK_SENSOR,
    X3D_POINT_PROPERTIES,
    VRML_POINT_SET,
    VRML_POLYLINE_2D,
    X3D_POLYLINE_EMITTER,
    X3D_POLYPOINT_2D,
    X3DOM_POP_GEOMETRY,
    X3DOM_POP_GEOMETRY_LEVEL,
    X3D_POSITION_CHASER_2D,
    X3D_POSITION_CHASER,
    X3D_POSITION_DAMPER_2D,
    X3D_POSITION_DAMPER,
    VRML_POSITION_INTERPOLATOR,
    X3D_POSITION_INTERPOLATOR_2D,
    X3D_PRIMITIVE_PICK_SENSOR,
    X3D_PROGRAM_SHADER,
    KAMBI_PROJECTED_TEXTURE_COORDINATE,
    X3D_PROJECTION_VOLUME_STYLE,
    VRML_PROXIMITY_SENSOR,
    X3DOM_PYRAMID,
    X3D_QUAD_SET,
    X3DOM_RADAR_VOLUME_STYLE,
    X3D_RECEIVER_PDU,
    X3D_RECTANGLE_2D,
    X3DOM_RECTANGULAR_TORUS,
    X3DOM_REFINEMENT_TEXTURE,
    X3DOM_REMOTE_SELECTION_GROUP,
    KAMBI_RENDERED_TEXTURE,
    X3D_RIGID_BODY,
    X3D_RIGID_BODY_COLLECTION,
    X3D_SCALAR_CHASER,
    X3D_SCALAR_DAMPER,
    VRML_SCALAR_INTERPOLATOR,
    KAMBI_SCREEN_EFFECT,
    X3D_SCREEN_FONT_STYLE,
    X3D_SCREEN_GROUP,
    VRML_SCRIPT,
    X3D_SEGMENTED_VOLUME_DATA,
    X3D_SHADED_VOLUME_STYLE,
    X3D_SHADER_PART,
    X3D_SHADER_PROGRAM,
    KAMBI_SHADER_TEXTURE,
    VRML_SHAPE,
    X3D_SIGNAL_PDU,
    X3D_SINGLE_AXIS_HINGE_JOINT,
    X3D_SILHOUETTE_ENHANCEMENT_VOLUME_STYLE,
    COVER_SKY,
    X3D_SLIDER_JOINT,
    X3DOM_SLOPED_CYLINDER,
    X3DOM_SNOUT,
    X3DOM_SOLID_OF_REVOLUTION,
    VRML_SOUND,
    COVER_SPACE_SENSOR,
    VRML_SPHERE,
    X3DOM_SPHERE_SEGMENT,
    VRML_SPHERE_SENSOR,
    X3D_SPLINE_POSITION_INTERPOLATOR,
    X3D_SPLINE_POSITION_INTERPOLATOR_2D,
    X3D_SPLINE_SCALAR_INTERPOLATOR,
    VRML_SPOT_LIGHT,
    X3D_SQUAD_ORIENTATION_INTERPOLATOR,
    X3D_STATIC_GROUP,
    X3DOM_STIPPLE_VOLUME_STYLE,
    X3D_STRING_SENSOR,
    COVER_STEERING_WHEEL,
    DUNE_SUPER_ELLIPSOID,
    DUNE_SUPER_EXTRUSION,
    DUNE_SUPER_REVOLVER,
    DUNE_SUPER_SHAPE,
    X3D_SURFACE_EMITTER,
    X3DOM_SURFACE_SHADER_TEXTURE,
    VRML_SWITCH,
    KAMBI_TEAPOT,
    X3D_TEX_COORD_CHASER_2D,
    X3D_TEX_COORD_DAMPER_2D,
    VRML_TEXT,
    KAMBI_TEXT_3D,
    X3D_TEXTURE_BACKGROUND,
    VRML_TEXTURE_COORDINATE,
    X3D_TEXTURE_COORDINATE_3D,
    X3D_TEXTURE_COORDINATE_4D,
    X3D_TEXTURE_COORDINATE_GENERATOR,
    X3D_TEXTURE_PROPERTIES,
    VRML_TEXTURE_TRANSFORM,
    X3D_TEXTURE_TRANSFORM_3D,
    X3D_TEXTURE_TRANSFORM_MATRIX_3D,
    VRML_TIME_SENSOR,
    X3D_TIME_TRIGGER,
    X3D_TONE_MAPPED_VOLUME_STYLE,
    X3DOM_TORUS,
    VRML_TOUCH_SENSOR,
    VRML_TRANSFORM,
    X3D_TRANSFORM_SENSOR,
    X3D_TRANSMITTER_PDU,
    X3D_TRIANGLE_FAN_SET,
    X3D_TRIANGLE_SET,
    X3D_TRIANGLE_SET_2D,
    X3D_TRIANGLE_STRIP_SET,
    VRML_TRIMMED_SURFACE,
    COVER_TUI_BUTTON,
    COVER_TUI_COMBO_BOX,
    COVER_TUI_FLOAT_SLIDER,
    COVER_TUI_FRAME,
    COVER_TUI_LABEL,
    COVER_TUI_LIST_BOX,
    COVER_TUI_MAP,
    COVER_TUI_PROGRESS_BAR,
    COVER_TUI_SLIDER,
    COVER_TUI_SPLITTER,
    COVER_TUI_TAB,
    COVER_TUI_TAB_FOLDER,
    COVER_TUI_TOGGLE_BUTTON,
    X3D_TWO_SIDED_MATERIAL,
    X3DOM_UNIFORM,
    X3D_UNIVERSAL_JOINT,
    COVER_VEHICLE,
    KAMBI_VECTOR_INTERPOLATOR,
    X3DOM_VIEWFRUSTUM,
    VRML_VIEWPOINT,
    X3D_VIEWPOINT_GROUP,
    X3D_VIEWPORT,
    COVER_VIRTUAL_ACOUSTICS,
    COVER_VIRTUAL_SOUND_SOURCE,
    VRML_VISIBILITY_SENSOR,
    X3D_VOLUME_DATA,
    X3D_VOLUME_EMITTER,
    X3D_VOLUME_PICK_SENSOR,
    DUNE_VRML_CUT,
    DUNE_VRML_SCENE,
    COVER_WAVE,
    X3D_WIND_PHYSICS_MODEL,
    DUNE_WONDERLAND_IMPORT_JAVA,
    VRML_WORLD_INFO,

    // fake nodes
    VRML_COMMENT,
    X3D_EXPORT,
    X3D_IMPORT,

    // the following are node "classes", used to validate the
    // scene graph hierarchy

    ANY_NODE,
    AUDIO_CLIP_NODE,
    BODY_COLLIDABLE_NODE,
    BODY_COLLISION_SPACE_NODE,
    BOUNDED_OBJECT_NODE,
    COMPOSABLE_VOLUME_RENDER_STYLE_NODE,
    COORDINATE_NODE,
    FONT_STYLE_NODE,
    HANIM_CHILD_NODE,
    INLINE_NODE,
    INTERPOLATOR_NODE,
    MATERIAL_NODE,
    METADATA_NODE,
    MOVIE_TEXTURE_NODE,
    NURBS_CONTROL_CURVE_NODE,
    NURBS_TEXTURE_COORDINATE_NODE,
    LAYER_NODE,
    LIGHT_NODE,
    LINE_SET_NODE,
    LOD_NODE,
    PARAMETRIC_GEOMETRY_NODE,
    PARTICLE_EMITTER_NODE,
    PARTICLE_PHYSICS_MODEL_NODE,
    PRODUCT_STRUCTURE_CHILD_NODE,
    PRIMITIVE_GEOMETRY_NODE,
    RIGID_JOINT_NODE,
    ROOT_NODE, // still a dummy
    SHAPE_NODE,
    SHADER_NODE,
    TEXTURE_3D_NODE,
    VERTEX_ATTRIBUTE_NODE,
    VIEWPOINT_NODE,
    VIEWPOINT_GROUP_NODE,
    VOLUME_DATA_NODE,

    // combined types
    AUDIO_CLIP_OR_MOVIE_TEXTURE_NODE,
    BODY_COLLIDABLE_OR_BODY_COLLISION_SPACE_NODE,
    VOLUME_RENDER_STYLE_NODE,
    GENERATED_TEXTURE_COORDINATE_NODE,
    NURBS_TEXTURE_COORDINATE_OR_TEXTURE_COORDINATE_NODE,
    PRIMITIVE_GEOMETRY_OR_MASS_DENSITY_MODEL_NODE,
    SHAPE_OR_INLINE_NODE,
    SHAPE_OR_LOD_NODE,
    SPOTLIGHT_OR_DIRECTIONALLIGHT_OR_VIEWPOINT_NODE,
    TEXTURE_OR_TEXTURE_3D_NODE,
    VIEWPOINT_OR_VIEWPOINT_GROUP_NODE,
    NURBS_CURVE_2D_OR_CONTOUR_POLYLINE_2D_NODE,

    // the nodeclass of the following types can be bitwise OR'ed to any node
    NOT_SELF_NODE           = 1 << 19,
    SENSOR_NODE             = 1 << 20,
    TEXTURE_COORDINATE_NODE = 1 << 21,
    TEXTURE_TRANSFORM_NODE  = 1 << 22,
    TEXTURE_NODE            = 1 << 23,
    TEXTURE_2D_NODE         = 1 << 24,
    GEOMETRY_NODE           = 1 << 25,
    COLOR_NODE              = 1 << 26,
    CHILD_NODE              = 1 << 27,
    GROUPING_NODE           = 1 << 28,
    URL_NODE                = 1 << 29,
    PROTO_NODE              = 1 << 30 
    // do not use bit 31 (sign bit)
};

#define LAST_NODE VRML_COMMENT

enum {
    PROFILE_CORE,
    PROFILE_INTERCHANGE,
    PROFILE_CAD_INTERCHANGE,
    PROFILE_INTERACTIVE,
    PROFILE_MPEG4_INTERACTIVE,
    PROFILE_IMMERSIVE,
    PROFILE_FULL
};

typedef enum {
    XZ_DIRECTION,
    XY_DIRECTION,
    YZ_DIRECTION
} Direction;

enum xmlWriteFlag {
    XML_PROTO_INSTANCE_FIELD,
    XML_IN_TAG,
    XML_IS,
    XML_NODE
};

class EventOutData {
public:
    int eventOut;
    int numOutput;
    Node *protoNode;
};

class Colored;
class NodeColor;
class NodeColorRGBA;
class NodeCoordinate;

class NodeData {
public:
                      NodeData(Scene *scene, Proto *proto);
                      NodeData(const Node &node);
    virtual          ~NodeData();

    void              initIdentifier(void);

    void              delete_this(void);
    bool              isClassType(int type) { return type >= ANY_NODE; }
    bool              matchNodeClass(int childType) const;

    virtual int       writeProto(int filedes);
    int               writeProto(int f, const char *urn, 
                                 const char *directoryName,
                                 const char *haveUrl = "",
                                 bool nameInUrl = true);
    int               writeX3dProto(int filedes);

    int               writeProtoArguments(int filedes) const;
    virtual bool      avoidProtoOnPureVrml(void) { return false; }
    virtual bool      canWriteAc3d();
    virtual int       writeAc3d(int filedes, int indent);
    virtual int       writeAc3dMaterial(int filedes, int indent, 
                                          const char *name) { return 0; }
    virtual void      setAc3dMaterialIndex(int index) {}
    virtual int       getIncAc3dMaterialIndex(void) { return 0; }
    virtual void      handleAc3dMaterial(ac3dMaterialCallback callback, 
                                         Scene* scene);

    virtual int       writeRib(int filedes, int indent);
    virtual int       writePovray(int filedes, int indent);

    virtual bool      canWriteLdrawDat();
    virtual int       writeLdrawDat(int filedes, int indent);

    Scene              *getScene() const { return m_scene; }
    virtual FieldValue *getField(int index) const;
    virtual void        setField(int index, FieldValue *value, 
                                 int containerField = -1);

    virtual FieldValue *getField(FieldIndex index) const
                            { return getField((int) index); }
    virtual void        setField(FieldIndex index, FieldValue *value)
                            { setField((int) index, value); }

    virtual FieldValue *getUntranslatedField(int index) const;

    const char       *getTreeLabel(void) { return m_treeLabel; }
    const MyString   &getName(void);
    const MyString   &getNameOrNewName(void);
    // setName should only be called from Scene::def
    void              setName(const char *name);
    bool              hasName(void);
    bool              needsDEF() const;

    virtual void      addToConvertedNodes(int writeFlags);

    int               getNumConvertedNodes() { return m_convertedNodes.size(); }
    Node             *getConvertedNode(int i) { return m_convertedNodes[i]; }
    void              deleteConvertedNodes(void);

    bool              needExtraJavaClass(void);
    bool              writeJavaOutsideClass(int languageFlag);

    virtual bool      isConvertedInCurveAnimaton(void) 
                          { return m_isConvertedInCurveAnimaton; }
    void              setConvertedInCurveAnimaton(void)
                          { m_isConvertedInCurveAnimaton = true; }      

    void              getGraphPosition(float *x, float *y) const
                          { *x = m_graphX; *y = m_graphY; }
    void              setGraphPosition(float x, float y)
                          { m_graphX = x; m_graphY = y; }

    void              getGraphSize(int *width, int *height) const
                          { *width = m_graphWidth; *height = m_graphHeight; }
    void              setGraphSize(int width, int height)
                          { m_graphWidth = width; m_graphHeight = height; }

    void              ref() { m_refs++; }
    void              unref() 
                          { 
                          if (--m_refs == 0) 
                              delete_this(); 
                          }
    int               getRefs() { return m_refs; }

    virtual void      onAddRoute(Node *s, int out, Node *d, int in) {}

    virtual int       getType() const;
    virtual int       getNodeClass() const;
    virtual int       getProfile(void) const { return -1; }

    virtual const char* getComponentName(void) const { return ""; }
    virtual int       getComponentLevel(void) const { return -1; }

    int               getMaskedNodeClass(void);

    virtual Node     *copy() const = 0;
    void              copyData(const NodeData &node);

    int               findChild(Node *child, int field) const;

    int               lookupEventIn(const char *name, bool x3d) const;
    int               lookupEventOut(const char *name, bool x3d) const;

    Proto            *getProto() const { return m_proto; }
    void              setProto(Proto *proto) { m_proto = proto; }
    virtual Proto    *getPrimaryProto() const { return getProto(); }
 
    virtual void      addInput(int eventIn, Node *src, int eventOut);
    virtual void      addOutput(int eventOut, Node *dst, int eventIn);
    virtual void      removeInput(int eventIn, Node *src, int eventOut);
    virtual void      removeOutput(int eventOut, Node *dst, int eventIn);
    const SocketList &getInput(int i) const { return m_inputs[i]; }
    const SocketList &getOutput(int i) const { return m_outputs[i]; }

    virtual void      update();
    virtual void      reInit();

    virtual void      updateTime() {}

    virtual bool      validate(void) { return true; }

    virtual void      setInternal(bool flag) {}

    virtual bool      hasNumbers4kids(void) { return false; }

    virtual bool      maySetDefault(void) { return true; }

    int               getNumParents() const { return m_parents.size(); }
    void              copyParentList(const Node &node);

    bool              checkValid(Element *field);
    bool              validChild(int field, Node *child);
    bool              validChildType(int field, int childType);
    int               findValidField(Node *child);
    int               findValidFieldType(int childType);
    int               findFirstValidFieldType(int childType);
    bool              checkChildType(int field, int childType, 
                                     bool checkValid = false);
    int               findChildType(int childType);

    bool              getFlag(int flag) const 
                          { return (m_flags & (1 << flag)) != 0; }
    void              setFlag(int flag) { m_flags |= (1 << flag); }
    void              setFlagRec(int flag);
    void              clearFlag(int flag) { m_flags &= ~(1 << flag); }
    void              clearFlagRec(int flag);

    bool              isCollapsed() const 
                          { return getFlag(NODE_FLAG_COLLAPSED); }

    virtual void      transform() {}
    virtual void      transformBranch() {}
    virtual void      transformForHandle(int handle) {}
    virtual void      preDraw() {}
    virtual void      specialDraw() {}
    virtual void      draw() {}
    virtual void      draw(int pass) {}
    bool              canDraw() { return m_canDraw; }
    virtual void      bind() {}
    virtual void      unbind() {}
    virtual void      bind(GLuint textureId, GLuint textureUnit) {}
    virtual void      load() {}
    virtual void      drawHandles() {}
    virtual void      updateHandles() {}
    virtual void      drawJointHandles(float scale, Node *parent, Node *that)
                          {}
    /// countPolygons() do not count polygons in primitives
    virtual int       countPolygons(void) {return 0;}
    virtual int       countPrimitives(void) {return 0;}
    virtual int       countPolygons1Sided(void) {return 0;}
    virtual int       countPolygons2Sided(void) {return 0;}

    virtual bool      isMesh(void);
    virtual MyMesh   *getMesh(void) { return NULL; }

    virtual bool      hasTwoSides(void) { return false; }
    virtual bool      isDoubleSided(void) { return false; }
    virtual void      toggleDoubleSided(void) {}
    virtual int       getConvexField() { return -1; }
    virtual int       getNormalField() { return -1; }
    virtual int       getTexCoordField() { return -1; }
    virtual int       getSolidField() { return -1; }
    virtual void      flipSide(void) {}

    virtual void      startSetMultiHandles() {}
    virtual void      endSetMultiHandles() {}
    virtual bool      validHandle(int handle) { return true; }
    virtual bool      checkHandle(int handle) { return true; }
    virtual int       getMaxHandle(void) { return 0; }
    virtual Vec3f     getHandle(int handle, int *constraint, int *field)
                          { 
                          *field = -1;
                          return Vec3f(0.0f, 0.0f, 0.0f); 
                          }
    virtual void      setHandle(int /* handle */, const Vec3f & /* v */) {}

    virtual int       getNumVertex(void) { return 0; }
    virtual Vec3f     getVertex(int i) { return Vec3f(); }
    virtual bool      getValidVertex(void) { return false; }

    virtual void      sendEvent(int eventOut, double timestamp, 
                                FieldValue *value);
    virtual void      receiveEvent(int eventIn, double timestamp, 
                                   FieldValue *value);

    int               writeCSendEvent(int f, int indent, int languageFlag, 
                                      EventIn *target, EventOut *source, 
                                      Node *sNode);

    int               writeCCopyEvent(int f, int indent, int languageFlag, 
                                      EventOut *target, EventOut *source, 
                                      Node *sNode);

    int               writeCCopyEvent(int f, int indent, int languageFlag, 
                                      EventIn *target, EventIn *source, 
                                      Node *sNode);

    int               writeCDowithEvent(int f, int indent, int languageFlag, 
                                        const char *target, const char *source,
                                        Node *sNode, bool isArray, int type);

    virtual bool      isInterpolator() { return false; }

    virtual bool      hasProtoNodes(void) { return false; }

    virtual bool      hasControlPoints(void) { return false; }

    //nurbs conversion virtual
    virtual Node     *toNurbs() {return NULL;}
    virtual Node     *toNurbs(int nshell, int narea, int narcs, 
                              int uDegree, int vDegree) {return NULL;}
    virtual Node     *toNurbs(int narcslong,  int narcslat, 
                              int uDegree, int vDegree) {return NULL;}
    virtual Node     *toNurbs(int nAreaPoints, int Degree) {return NULL;}
    virtual Node     *toNurbs(int narcs, int pDegree, float rDegree, 
                              int axis) {return NULL;}
    virtual Node     *toNurbs(int narcs, int pDegree, float uDegree, 
                              Vec3f &P1, Vec3f &P2) {return NULL;}
    virtual Node     *degreeElevate(int newDegree) {return NULL;}
    virtual Node     *degreeElevate(int newUDegree, int newVDegree)
                          {return NULL;}

    virtual int       getDegree(bool u = true) { return -1; }
    virtual void      elongateNurbs(int handle, float *controlPoints) {}

    // mesh conversion virtual
    virtual Node     *toIndexedFaceSet(int meshFlags = MESH_WANT_NORMAL,
                                       bool cleanVertices = true,
                                       bool triangulate = true)
                          { return NULL; }
    virtual bool      canConvertToIndexedFaceSet(void) { return false; }
    virtual bool      shouldConvertToIndexedFaceSet(void) 
                          { return canConvertToIndexedFaceSet(); }

    virtual Node     *toNurbsTrimmedSurface(void) { return NULL; }

    // indexed triangle set conversion virtual
    virtual Node     *toIndexedTriangleSet(int meshFlags = MESH_TARGET_HAS_CCW)
                          { return NULL; }
    virtual bool      canConvertToIndexedTriangleSet(void) { return false; }
    virtual bool      shouldConvertToIndexedTriangleSet(void) 
                         { return canConvertToIndexedTriangleSet(); }

    // triangle set conversion virtual
    virtual Node     *toTriangleSet(int meshFlags = MESH_TARGET_HAS_CCW) 
                          { return NULL; }
    virtual bool      canConvertToTriangleSet(void) { return false; }
    virtual bool      shouldConvertToTriangleSet(void)
                          { return canConvertToTriangleSet(); }

    virtual bool      canSimpleTriangulate(void) { return false; }
    void              simpleTriangulation(void) {}

    // chain conversion virtual
    virtual Node     *toIndexedLineSet(void) { return NULL; }
    virtual bool      canConvertToIndexedLineSet(void) { return false; }

    // extrusion conversion virtual
    virtual Node     *toExtrusion(void) { return NULL; }
    virtual bool      canConvertToExtrusion(void) { return false; }

    // convertion to interpolators
    virtual bool      canConvertToPositionInterpolator(void) 
                          { return false; }
    virtual Node     *toPositionInterpolator(void)
                          { return NULL; }

    virtual bool      canConvertToOrientationInterpolator(void) 
                          { return false; }
    virtual Node     *toOrientationInterpolator(Direction direction)
                          { return NULL; }

    virtual bool      canConvertToPositionAndOrientationInterpolators(void) 
                          { return false; }
    virtual void      toPositionAndOrientationInterpolators(NodeList *nodes)
                          { return; }

    // other conversions
    virtual Node     *toCurveAnimation(void) { return NULL; }

    virtual void      setMeshDirty(void) {}

    // texture glColorMode
    virtual int       textureGlColorMode(void) { return -1; }

    virtual bool      isMeshBasedNode(void) { return false; }
    virtual bool      isMeshMorphingNode(void) { return false; }
    virtual bool      isChainBasedNode(void) { return false; }

    // compare content
    bool              isEqual(Node* node);
    friend bool       isEqual(Node* node);

    bool              isEqualCopy(Node* node);
    MyString          newEventName(int typeEnum, bool out);

    bool              hasRoute(SocketList socketlist);

    virtual bool      isInvalidChildNode(void)
                          {
                          // true if node may not be part of a MFNode field
                          // of a other node (or at root of scenegraph)
                          return false;
                          }

    virtual bool      isProgrammableShaderObject(void) { return false; }

    virtual bool      isNodeWithAdditionalEvents(void) 
                          { return isProgrammableShaderObject(); }

    virtual int       getX3dVersion(void) const = 0;
    virtual bool      isCoverNode(void) { return false; }
    virtual bool      hasCoverFields(void) { return false; }
    bool              hasCoverDefault() { return hasDefault(FF_COVER_ONLY); }
    virtual bool      isKambiNode(void) { return false; }
    virtual bool      hasKambiFields(void) { return false; }
    bool              hasKambiDefault() { return hasDefault(FF_KAMBI_ONLY); }
    bool              hasX3dDefault() { return hasDefault(FF_X3D_ONLY); }
    bool              hasDefault(int flag);

    virtual void      setNormalFromMesh(Node *nnormal) {}
    virtual void      setTexCoordFromMesh(Node *ntexCoord) {}

    virtual bool      isTransparent(void) { return false; }
    virtual float     getTransparency(void) { return 0; }

    virtual bool      isFlat(void) { return false; }

    virtual NodeCoordinate *getCoordinateNode() { return NULL; }

    virtual NodeColor *getColorNode() { return NULL; }
    virtual bool      hasColor(void) { return false; }
    virtual bool      hasColorRGBA(void) { return false; }
    virtual NodeColorRGBA *getColorRGBANode() { return NULL; }

    virtual bool      hasX3domOnOutputChange(void) { return false; }
    virtual bool      hasX3domOnclick(void) { return false; }

    void              handleIs(void);
    Node             *getIsNode(int nodeIndex);
    void              setScene(Scene *scene) { m_scene = scene; }
    bool              isInsideProto(void) 
                          { return m_insideProto != NULL; }
    void              setOutsideProto(Proto *proto) { m_insideProto = proto; }
    Proto            *getOutsideProto(void) { return m_insideProto; }

    NodePROTO        *getNodePROTO(void) { return m_nodePROTO; }

    void              removeChildren(void);
    bool              isDefault(int field) const;
    bool              isDefaultAngle(int field) const;
    bool              hasOutput(const char* routename) const;
    bool              hasOutputs(void);
    bool              hasOutputsOrIs(void);
    bool              hasInput(const char* routename) const;
    bool              hasInputs(void);
    bool              hasInputsOrIs(void);
    bool              hasInputsIs(void);
    int               writeRoutes(int f, int indent) const;
    void              removeRoutes(void);

    void              generateTreeLabel(void);

    void              setDefault(void);
    virtual bool      canMoveTo(int direction) { return false; }

    virtual Node     *convert2X3d(void);
    virtual Node     *convert2Vrml(void);

    virtual void      setupFinished(void) {}

    virtual int       getChildrenField(void) const { return -1; }

    virtual const char *getVariableName(void);
    void              setVariableName(const char *name);
    void              unSetVariableName(void) { m_variableName = ""; }

    const char       *getClassName(void);

    virtual bool      isDynamicFieldsNode(void) { return false; }

    virtual bool      isPROTO(void) const;

    virtual NodeList *getLoadedNodes(void) { return NULL; }

    virtual int       repairField(int field) const;
    virtual int       translateField(int field) const;

    int               writeCVariable(int f, int languageFlag);
    int               getCounter4SceneTreeView(void)
                          { return m_counter4SceneTreeView; }
    void              addCounter4SceneTreeView(void)
                          { m_counter4SceneTreeView++; }
    void              setCounter4SceneTreeViewToZero(void )
                          { m_counter4SceneTreeView = 0; }
    long              getId(void);
    void              setId(long id);

    // for Node*Viewpoint
    virtual Vec3d     getPosition() const { return Vec3d(); }
    virtual Quaternion getOrientation() const { return Quaternion(); } 
    virtual void      setPosition(const Vec3d &pos) {}
    virtual void      setOrientation(const Quaternion &quat)  {}
    virtual SFFloat  *fov() { return NULL; };
    virtual void      getMatrix(float* matrix) {}
    void              apply(bool useStereo = false) {}
    bool              getWritten(void) { return m_written; }
    void              setWritten(bool flag) { m_written = flag; }

    bool              isInAlreadyWrittenEventOuts(int eventOut, 
                                                  int numOutput,
                                                  Node *protoNode)
                          {
                          for (long i = 0; 
                              i < m_alreadyWrittenEventOuts.size(); i++)
                              if ((m_alreadyWrittenEventOuts[i].eventOut
                                   == eventOut) &&                   
                                  (m_alreadyWrittenEventOuts[i].numOutput
                                   == numOutput) &&
                                  (m_alreadyWrittenEventOuts[i].protoNode
                                   == protoNode)
                                 ) 
                                 return true;
                          return false;
                          }

    void              appendToAlreadyWrittenEventOuts(int eventOut, 
                                                      int numOutput,
                                                      Node *protoNode)
                          {
                          EventOutData data;
                          data.eventOut = eventOut;
                          data.numOutput = numOutput;
                          data.protoNode = protoNode;
                          m_alreadyWrittenEventOuts.append(data);
                          }
    void              removeAlreadyWrittenEventOuts(void)
                          { m_alreadyWrittenEventOuts.resize(0); }

    bool              hasRouteForProcessEvent(void);

    const char *      getX3domId(void) 
                          { 
                          if (strlen(m_x3domId) > 0) 
                              return m_x3domId; 
                          return m_name; 
                          }
    void              setX3domId(const char *id) { m_x3domId = strdup(id); }
    void              setX3domOnOutputChange(const char *function) 
                          { m_x3domOnOutputChange = strdup(function); }
    void              setX3domOnClick(const char *function) 
                          { m_x3domOnClick = strdup(function); }

protected:
    const char       *searchIsName(int i, int type);

    int               writeIs(int f, int indent, 
                              const char *name, const char * isName);
    virtual int       writeFields(int f, int indent);
    int               writeField(int f, int indent, int i, 
                                 bool script = false);
    int               writeEventIn(int f, int indent, int i, 
                                   bool eventName = false);
    int               writeEventOut(int f, int indent, int i, 
                                    bool eventName = false);

    int               writeEventInStr(int f);
    int               writeEventOutStr(int f);
    int               writeFieldStr(int f);
    int               writeExposedFieldStr(int f);

    int               writeXmlIs(int f, int indent, 
                                 const char *name, const char *isName);
    int               writeXmlProtoInstanceField(int f, int indent, 
                                                 const char *name, 
                                                 FieldValue *value); 
    int               writeXmlFields(int f, int indent, int when,
                                     int containerField = -1,
                                     bool avoidUse = false);
    int               writeXmlField(int f, int indent, int i, int when,
                                    bool script = false,
                                    int containerField = -1,
                                    bool avoidUse = false);
    int               writeXmlEventIn(int f, int indent, int i, int when,
                                      bool eventName = false);
    int               writeXmlEventOut(int f, int indent, int i, int when,
                                       bool eventName = false);

    void              addIsElement(Node *node, int field, int elementType,
                                   Proto *origProto, int origField, 
                                   int flags = 0);

protected:
    Scene            *m_scene;
    ParentArray       m_parents;
    MyString          m_name;
    MyString          m_treeLabel;
    MyString          m_variableName;
    NodeArray         m_convertedNodes;
    int               m_refs;
    int               m_flags;
    Proto            *m_proto;
    FieldValue      **m_fields;
    int               m_numFields;
    int               m_numEventIns;
    int               m_numEventOuts;
    SocketList       *m_inputs;
    SocketList       *m_outputs;
    float             m_graphX, m_graphY;
    int               m_graphWidth, m_graphHeight;
    long              m_identifier;
    long              m_identifierCopy;
    Proto            *m_insideProto;
    NodePROTO        *m_nodePROTO;
    bool              m_canDraw;
    bool              m_isConvertedInCurveAnimaton;
    MyArray<EventIn *>      m_isEventIns;
    MyArray<EventOut *>     m_isEventOuts;
    MyArray<Field *>        m_isFields;
    MyArray<ExposedField *> m_isExposedFields;
    int               m_counter4SceneTreeView;
    bool              m_written;
    MyArray<EventOutData> m_alreadyWrittenEventOuts;
    const char *      m_x3domId;
    const char *      m_x3domOnOutputChange;
    const char *      m_x3domOnClick;
    bool              m_alreadyConverted;
    bool              m_writtenCompleteCProcessEvent;
};
   

typedef bool (*DoWithNodeCallback)(Node *node, void *data);
typedef bool (*DoWithSimilarBranchCallback)(Node *node, Node *similarNode, 
                                            void *data);
typedef void (*DoWithAllElementsCallback)(Element *element, void *data);

class Node : public NodeData
{
public:
                      Node(Scene *scene, Proto *proto);
                      Node(const Node &node);
                      Node(const Node &node, Proto *proto);

    virtual          ~Node();

    void              addParent(Node *parent, int index);
    void              removeParent(void);
    bool              hasParent(void) const
                          { 
#ifdef HAVE_NULL_COMPARE
                          if (this == NULL)
                              return false;
#endif
                          if (getNumParents() < 0) 
                              return false;
                          if (m_geometricParentIndex == -1)
                              return false;
                          if (m_geometricParentIndex > getNumParents())
                              return false;
                          return (getParent() != NULL);
                          }
    Node             *getParent(void) const 
                          { 
                          if (m_parents.size() == 0)
                              return NULL;
                          return getParent(m_geometricParentIndex);
                          }
    bool              hasParentOrProtoParent(void) const
                          { return (getParentOrProtoParent() != NULL); }
    Node             *getParentOrProtoParent(void) const;
    int               getParentField(void) const 
                          { return  getParentField(m_geometricParentIndex); }

    Node             *getParent(int index) const 
                          { 
                          return index == -1 ? NULL : m_parents[index].m_node; 
                          }
    int               getParentField(int index) const 
                          { 
                          return index == -1 ? -1 : m_parents[index].m_field;
                          }

    int               getParentFieldOrProtoParentField(void) const;
    Node             *searchParent(int nodeType) const; 
    Node             *searchParentField(int parentfield) const; 
    int               getParentIndex(void) const;
    FieldValue       *getParentFieldValue(void) const;
    void              setParentFieldValue(FieldValue *value);

    NodeList          getParents(void);

    bool              hasBranchInputs(void);

    virtual int       write(int filedes, int indent, bool avoidUse = false);
    virtual int       writeXml(int filedes, int indent, 
                               int containerField = -1, bool avoidUse = false);

    bool              isInScene(Scene* scene) const;
    virtual void      addFieldNodeList(int index, NodeList *value,
                                      int containerField = -1);
    bool              hasAncestor(Node *node) const;
    virtual int       getAnimatedNodeField(int field) { return -1; }
    virtual bool      supportAnimation(void);
    virtual bool      supportCurveAnimation(void) { return false; }
    bool              supportInteraction(void);
    virtual Colored  *getColored() { return NULL; }
    virtual int       getAnimationCommentID(void) { return -1; }
    virtual int       getInteractionCommentID(void) { return -1; }
    virtual bool      isInvalidChild(void);
    virtual bool      hasBoundingBox(void) { return false; }
    virtual void      setBoundingBox(void) {}
    virtual Vec3f     getMinBoundingBox(void);
    virtual Vec3f     getMaxBoundingBox(void);
    Vec3f             getBboxSize(void);
    Vec3f             getBboxCenter(void);
    virtual void      flip(int index) {}
    virtual void      swap(int fromTo) {}
    virtual bool      showFields(); 
    virtual bool      isEXTERNPROTO(void) { return false; }
    // for Hanim
    virtual void      applyJoint(int skinNum, MyMesh *mesh, MFVec3f *vertices,
                                 Node *parent) {}
    virtual bool      getMatrixDirty(void) { return false; }

    void              appendTo(NodeList* nodelist);
    void              appendComment(Node *node);

    void              doWithParents(DoWithNodeCallback callback, void *data,
                                    bool searchInRest = true, 
                                   bool callSelf = true);
    void              doWithSiblings(DoWithNodeCallback callback, void *data,
                                     bool searchInRest = true, 
                                     bool callSelf = true);
    void              doWithNextSiblings(DoWithNodeCallback callback, 
                                         void *data, bool searchInRest = true, 
                                         bool callSelf = true);

    bool              doWithBranch(DoWithNodeCallback callback, void *data,
                                   bool searchInRest = true, 
                                   bool skipBranch = false,
                                   bool skipProto = false,
                                   bool callSelf = true,
                                   bool skipInline = true,
                                   bool searchInConvertedNodes = false);

    bool              doWithSimilarBranch(DoWithSimilarBranchCallback 
                                          callback, Node *similarNode, 
                                          void *data);

    void              doWithAllElements(DoWithAllElementsCallback callback, 
                                        void *data);

    parentFlag        getParentFlag(int index) const 
                          { return m_parents.get(index).m_parentFlag; }

    void              setParentFlag(int index, parentFlag flag) 
                          { m_parents[index].m_parentFlag = flag; }

    bool              isFirstUSE(void)
                          { 
                          if (isPROTO())
                              return m_geometricParentIndex == 1; 
                          else
                              return m_geometricParentIndex == 0; 
                          }
    bool              isUnused(void) { return m_geometricParentIndex == -1; }
    bool              getIsUse(void) { return m_isUse; }
    void              setIsUse(bool flag) { m_isUse = flag; }

    void              copyChildrenTo(Node *copyedNode, 
                                     bool copyNonNodes = false);
    void              copyOutputsTo(Node *copyedNode);
    void              copyInputsTo(Node *copyedNode);

    int               getSiblingIndex(void);
    int               getPrevSiblingIndex(void);
    int               getNextSiblingIndex(void);

    Node             *getPrevSibling(void);
    Node             *getNextSibling(void);

    virtual bool      isJoint(void) { return false; }
    virtual bool      isHumanoid(void) { return false; }

    virtual int       writeC(int filedes, int languageFlag);
    int               writeCDataAsFunctions(int filedes, int languageFlag,
                                            bool cont = false);
    int               writeCDataAsClasses(int filedes, int languageFlag);
    int               writeCDataFunction(int filedes, int languageFlag,
                                         bool forward, bool cont = false);
    int               writeCDataFunctionFields(int filedes, int languageFlag,
                                               bool forward, bool cont);
    int               writeCElement(int f, int elementType, int i, 
                                    int languageFlag, bool nodeFlag);
    int               writeCElementFunction(int f, int elementType, 
                                            int i, int languageFlag, 
                                            bool nodeFlag, bool cont = false);
    int               writeCElementClass(int f, int elementType, int i, 
                                         int languageFlag, bool nodeFlag);
    int               writeCAndFollowRoutes(int f, int indent, 
                                            int languageFlag, 
                                            bool writeSensorNodes,
                                            const char *eventName);
    int               writeCProcessEvent(int f, int indent, int languageFlag, 
                                         const char *eventName);
    int               writeCProcessEventCallback(int f, int languageFlag);
    virtual int       writeCDynamicNodeCallback(int f, int languageFlag) 
                          { return 0; }
    int               writeCInstallDynamicNodeCallback(int f, int languageFlag,
                                                       Proto *proto);
    int               writeCGetParent(int filedes, int languageFlag);

    virtual void      writeCWarning(void) {}

    virtual bool      canWriteCattGeo();
    virtual int       writeCattGeo(int filedes, int indent);
    int               getGeometricParentIndex(void) const
                          { return m_geometricParentIndex; }
    Path             *getPath() const;
    bool              isDeepInsideProto(void);

    virtual char     *buildExportMaterialName(const char *name)
                          { return mystrdup(name); }
    char*             getExportMaterialName(const char *defaultName);

    int               getContainerField(void); 
    void              setContainerField(int containerField);

    Node             *getProtoParent(void) { return m_protoParent; }
    void              setProtoParent(Node *n) { m_protoParent = n; }
    void              setNodePROTO(NodePROTO *node);
    NodeHAnimHumanoid *getHumanoid();
    virtual Node     *getIndexedFaceSet(void);
    bool              getColorPerVertex(void) { return false; }
    virtual void      setAlreadyConverted(void);
    virtual bool      getAlreadyConverted(void) { return m_alreadyConverted; }
    bool              isCWriteable();

protected:
    int               m_geometricParentIndex;
    NodeList         *m_commentsList;
    int               m_numberCDataFunctions;
    int               m_containerField;
    Node             *m_protoParent;
    bool              m_isUse;
};

