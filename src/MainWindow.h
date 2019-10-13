/*
 * MainWindow.h
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

#ifndef _MAIN_WINDOW_H
#define _MAIN_WINDOW_H

#ifndef _PANED_WINDOW_H
#include "PanedWindow.h"
#endif

#ifndef _SCENE_H
#include "Scene.h"
#endif

#ifdef HAVE_SAND
#include "nebula/ExportNebula.h"
#endif

class SceneView;
class ChannelView;
class ToolbarWindow;
class StatusBar;
class ScriptEdit;
class ObjectEdit;
class ImageTextureEdit;
class PixelTextureEdit;
class MovieTextureEdit;
class AudioClipEdit;
class NodeShape;

#include "swttypedef.h"
#include "ColorConversion.h"
#include "NodeNurbsCurve.h"

enum SiblingCommands {
     MOVE_SIBLING_UP,
     MOVE_SIBLING_DOWN,
     MOVE_SIBLING_FIRST,
     MOVE_SIBLING_LAST
};

class IconPos {
public:
    IconPos(void);
protected:
    int                 GetIconPos(int* buttons,int length,int icon);
    int                 m_fileNewIconPos;
    int                 m_fileNewX3dIconPos;
    int                 m_fileOpenIconPos;
    int                 m_fileSaveIconPos;
    int                 m_filePreviewIconPos;
    int                 m_fileWonderlandIconPos;
    int                 m_editCutIconPos;
    int                 m_editCopyIconPos;
    int                 m_editPasteIconPos;
    int                 m_editDeleteIconPos;
    int                 m_fullScreenIconPos;
    int                 m_colorCircleIconPos;
    int                 m_vertexModifierIconPos;
    int                 m_objectEditIconPos;
    int                 m_urlEditIconPos;
    int                 m_animationIconPos;
    int                 m_toNurbs4KidsIconPos;
    int                 m_interactionIconPos;
    int                 m_x_symetricIconPos;
    int                 m_navigationMouseIconPos;
    int                 m_mouseExamineIconPos;
    int                 m_mouseWalkIconPos;
    int                 m_mouseFlyIconPos;
    int                 m_mouseRollIconPos;
    int                 m_mouseFollowIconPos;
    int                 m_moveModeIconPos;
    int                 m_rotateModeIconPos;
    int                 m_scaleModeIconPos;
    int                 m_uniformScaleModeIconPos;
    int                 m_centerModeIconPos;
    int                 m_sixDModeIconPos;
    int                 m_sixDLocalModeIconPos;
    int                 m_rocketModeIconPos;
    int                 m_hoverModeIconPos;
    int                 m_tDimensionStartIconPos;
    int                 m_t2axesStartIconPos;
    int                 m_inputDeviceGreaterIconPos;
    int                 m_inputDeviceLesserIconPos;
    int                 m_navigationInputDeviceIconPos;
    int                 m_xOnlyIconPos;
    int                 m_yOnlyIconPos;
    int                 m_zOnlyIconPos;
    int                 m_showJointLikeHandlesIconPos;
};

class ProtoMenuItem {
public:
    MyString protoName;
    int id;
    bool disabled;
};

class NodeButton {
public:
    int type;
    int id;
    bool alwaysEnabled;
    bool enabled; 
};

class SuperExtrusionInfo {
public:
    int direction;
    int numPoints;
};

class MainWindow : public PanedWindow, IconPos {
public:
                        MainWindow(Scene *scene, SWND parent);

    virtual            ~MainWindow();

    void                destroyMainWindow(void);

    virtual void        OnCommand(void *id);
    virtual void        OnHighlight(void *id);
    virtual void        OnUpdate(SceneView *sender, int type, Hint *hint);
    virtual void        OnSize(int x, int y);

    const char         *getName(void) const { return "MainWindow"; }

    void                OnFileOpen();
    void                OnFileOpenUrl();
    void                OnFileOpenHtml();
    void                OnFileImport();
    bool                OnFileSave();
    bool                OnFileSaveAs(int writeFlags = 0);
    bool                OnFileExportVRML97();
    bool                OnFileExportX3DV();
    bool                OnFileExportX3DXML();
    bool                OnFileExportX3D4Wonderland();
    bool                OnFileExportCover();
    bool                OnFileExportKanim();
    bool                OnFileExportX3dom();
    bool                OnFileExportXite();
    bool                OnFileExportC();
    bool                OnFileExportCC();
    bool                OnFileExportJava();
    bool                OnFileExportWonderlandModule();
    bool                OnFileExportAc3d();
    bool                OnFileExportAc3d4Raven();
    bool                OnFileExportRib();
    bool                OnFileExportCattGeo();
    bool                OnFileExportLdrawDat();
#ifdef HAVE_LIBCGAL
    bool                OnFileExportOff();
    bool                OnFileImportOff();
#endif
    int                 OnTimer();
    bool                SaveModified();
    void                setStatusText(const char *str);
    bool                SaveFile(const char *path, const char *url, 
                                 int writeFlags = 0, char *x3dPath = NULL);
    void                checkInFile(const char *path);

    SWND                getParentWindow() { return m_parentWindow; }
    SWND                getScene3DWindow() { return m_S3DView->GetWindow(); }

    bool                getValidTexture(void);
    bool                getValidTexture3D(void);
    bool                isValidNodeType(Node* node, int field);
    void                InitToolbars(void);
    void                InitToolbar(STOOLBAR toolbar, NodeButton *buttons, 
                                    int count);
    void                UpdateToolbars(void);
    void                UpdateToolbar(int id);
    void                UpdateToolbarSelection(void);
    void                UpdateToolbar(STOOLBAR toolbar, Node *node, int field,
                                      NodeButton *buttons, int count);
    void                UpdateMenuVisible(void);
    void                UpdateMenuCoverNodes(void);
    void                UpdateMenuKambiNodes(void);
    void                UpdateMenuX3domNodes(void);
    void                UpdateStaticMenuCoverNodes(void);
    void                UpdateStaticMenuKambiNodes(void);
    void                UpdateStaticMenuX3domNodes(void);
    void                UpdateObjectEdit(const Path *selection);
    void                OnScriptEditorReadyCallback(void);
    void                OnTextEditorReadyCallback(void);
    void                OnImageTextureEditorReadyCallback(void);
    void                OnSoundEditorReadyCallback(void);
    void                OnMovieEditorReadyCallback(void);
    bool                matchFieldPipeFilterNode(Node *node);
    bool                matchFieldPipeFilterField(Node *node);
    void                setVrmlCutNode(Node *node) { m_vrmlCutNode = node; }
    Node               *createNode(const char *type);
    void                Play();
    void                Stop();

protected:
    Node               *createNode(Node *node);
    Node               *createGeometryNode(const char *type,
                                           bool emissiveDefaultColor = false,
                                           bool alignToCamera = true);
    Node               *createGeometryNode(Node *node,
                                           bool emissiveDefaultColor = false,
                                           bool alignToCamera = true);
    void                createScript();
    void                createElevationGrid();
    void                createGeoElevationGrid();
    void                createViewpoint();
    void                createGeoViewpoint();
    void                createNurbsPlane(int direction);
    void                createNurbsCurve();
    void                createNurbsCurve2D();
    void                createSuperExtrusion(SuperExtrusionInfo *info = NULL);
    void                createSuperRevolver();
    void                createCurveAnimationNode();

    Node               *insertNode(const char *type, Node *current = NULL);
    Node               *insertNode(int fieldType, const char *type, 
                                   Node *current = NULL);
    void                insertAudioClip();
    void                insertImageTexture();
    void                insertMultiTexture();
    void                insertMultiTextureCoordinate();
    void                insertMultiTextureTransform();
    void                insertMultiGeneratedTextureCoordinate();
    void                insertTextureCoordinate(bool generator);
    void                insertNormal();
    void                insertInline(bool withLoadControl = false);
    void                insertMovieTexture();
    void                insertNewNurbsNode(Node *nurbsNode, Node* parent);
    void                insertRigidBody();
    void                insertViewport();
    void                insertVrmlScene();
    void                insertCubeTexture();
    void                insertHAnimJoint();
    void                setHAnimJointWeight();
    void                removeHAnimJointWeight();
    void                insertHAnimSegment();
    void                insertHAnimSite();
    void                createVirtualAcoustics();
    void                insertVirtualSoundSource();

    void                insertNodeByName();

    Node               *getInsertNode(Node *node);

    void                toggleCreateAtZero(int index);
    void                toggleFullScreen();
    void                toggleView(int direction, SceneView *view, int id,
                                   const char *name);
    void                ShowView(int direction, SceneView *view, int id,
                                 const char *name);
    void                UpdateNumbers4Kids(void);
    STOOLBAR            LoadToolbar(ToolbarWindow *tbWin, int id, int count,
                                    const int *buttons, int menuid,
                                    const char *prefName);
    STOOLBAR            LoadToolbar(ToolbarWindow *tbWin, int id, int count,
                                    const NodeButton *buttons, int menuid,
                                    const char *prefName);
    void                toggleToolbar(ToolbarWindow *tbWin, STOOLBAR toolbar,
                                      int id, const char *name);
    void                toggleStatusbar();

    void                setTMode(TMode mode);
    void                setTDimension(TDimension dimension);
    void                setT2axes(T2axes axes);

    void                setColorCircleIcon();
    void                setVertexModifierIcon();

    int                 getMouseMode() {return(m_mouseMode);}
    bool                getMouseNavigationMode() 
                           {return(m_navigation_mouse_active);}
    bool                getInputDeviceNavigationMode() 
                           {return(m_navigation_input_device_active);}
    void                OnEditCut();
    void                OnEditCopy();
    void                OnEditCopyBranchToRoot();
    void                OnEditPaste();
    void                OnEditPasteSymetric(int direction);
    void                OnEditDelete();
    void                OnEditFind();
    void                OnEditFindAgain();
    void                selectNeighbours(bool u = true);
    void                setSelectionMode(int mode);
    void                Record();
    char               *getFileSelectorText(void);
    bool                OpenFileCheck(const char *path);
    void                ImportFileCheck(const char *path, Node *node, 
                                        int field);
    void                RefreshRecentFileMenu();
    void                RefreshProtoMenu();
    void                RefreshHelpSelection();
    void                UpdateTitle();
    void                updateColorCircle(void);
    void                updateVertexModifier(void);
    void                OnHelpOverview();
    void                OnHelpOverviewCatt();
    void                OnHelpSelection();
    void                OnHelpCoverSelection();

    void                EditProto(Proto* oldproto);
    void                EditScript(Node* oldnode);
    void                EditText(Node* oldnode);
    void                EditObject();
    void                EditUrl();
    void                createAnimation(void);
    void                createCurveAnimation(void);
    void                createInteraction(void);
    void                setXSymetricNurbsIcon();

    void                moveSibling(int command);
    void                moveSiblingUp()    { moveSibling(MOVE_SIBLING_UP); }
    void                moveSiblingDown()  { moveSibling(MOVE_SIBLING_DOWN); }
    void                moveSiblingFirst() { moveSibling(MOVE_SIBLING_FIRST); }
    void                moveSiblingLast()  { moveSibling(MOVE_SIBLING_LAST); }

    void                moveBranchToParent(void);
    void                moveBranchToTransformSelection(void);
    void                moveBranchTo(const char* nodeName, 
                                     const char* fieldName);
    void                moveBranchTo(Node* node, int field, Node* current);
    void                moveBranchToInline();
    void                moveBranchToProto(void);
    void                doWithBranch(DoWithNodeCallback callback, void *data);
    void                doWithBranchUpdate(DoWithNodeCallback callback, 
                                           void *data);
    void                branchConvertToTriangleSetUpdate(void);
    void                branchConvertToIndexedFaceSetUpdate(void);
    void                branchConvertToIndexedTriangleSetUpdate(void);

    void                setBranchFlipSide(void);
    void                setBranchConvex(void);
    void                setBranchSolid(void);
    void                setBranchTwoSided(void);
    void                setBranchCreaseAngle(void);
    void                setBranchTransparency(void);
    void                setBranchShininess(void);
    void                createBranchImageTexture(void);
    void                createBranchAppearance(void);
    void                createBranchMaterial(void);
    void                removeBranchNode(void);
    void                branchFieldPipe(void);
    void                branchBuildRigidBodyCollection();
    void                branchOptimize();
    void                branchCSGUnion(void);
    void                removeIllegalNodes();
    Node               *getTransformForCenter();
    void                centerToMid();
    void                centerToMax(int index);
    void                centerToMin(int index);
    void                insertArray(void);
    void                flip(int index);
    void                swap(int fromTo);
    void                moveTo(Node *node, int handle, int direction, Vec3f v,
                               float value);
    void                moveTo(int direction);
    void                scaleBy(Node *node, int handle, int direction, Vec3f v,
                                float value);
    void                scaleBy(int direction);
    void                same(int direction, int whichSame);
    void                setDefault();
    void                rebuildControlPoints();
    void                makeNurbSurfaceSymetric(int direction);
    void                insertToNurbsCurve();
    void                insertToNurbsSurface();
    void                degreeElevate(int degree);
    void                uDegreeElevate(int degree);
    void                vDegreeElevate(int degree);
    void                setWeightsTo1(void);
    void                elongateNurbs();
#ifdef HAVE_LIBCGAL
    void                convexHull();
#endif
    void                setViewpoint(void);
    void                setTarget(void);
    void                insetFace();
    void                extrude();
    void                triangulate(void);
    void                optimizeSet(void);
    void                subdivide(void);
    void                splitFaces(void);
    void                buildQuad();
    void                splitIntoPieces();
    void                makeSymetric(int direction);
#ifdef HAVE_LIBVCG
    void                meshReduce(void);
#endif
    void                snapTogether();
    void                csg(int operation);
    void                simpleJoin();
    void                toNurbs();
    void                toNurbsTrimmedSurface();
    void                toNurbsCurve();
    void                toSuperRevolver();
    void                toSuperExtrusion();
    void                toNurbsPositionInterpolator();
    void                toNurbsOrientationInterpolator();
    void                toExtrusion();
    void                toIndexedFaceSet();
    void                toIndexedTriangleSet();
    void                toTriangleSet();
    void                toIndexedFaceSetFromMesh();
    void                toIndexedLineSet();
    void                toPointSet();
    void                toCurveAnimation();
    void                toPositionInterpolator();
    void                toOrientationInterpolator(Direction direction);
    void                toPositionAndOrientationInterpolators();
    void                timeShift();
    void                toggleXrayRendering();
    void                countPolygons();
    void                countPolygons4catt();
    void                setPathAllURLs();
    void                clearStatusText();
    void                testInMenu();
    void                swapDiffuseEmissiveShape(NodeShape *shape,
                                                 ColorConversion conversion);
    void                createStarFish(int numberArms);
    void                createFlower(int numberLeaves);
    void                createTube(bool hornFlag = false);
    void                createTorus(void);
    void                createHalfSphere(void);
    void                createShell(void);
    void                createUfo(void);
    void                createInsectRear(int segments);
    void                createHeart(void);
    void                createSpindle(void);
    void                createMushroom(bool sulcate);
    void                createText3D(void);
    void                showColorPerVertexColorCircle(void);
    void                showDiffuseColorCircle(void);
    void                showEmissiveColorCircle(void);
    void                showSpecularColorCircle(void);
    void                showVertexModifier(void);
    void                searchInterpolator(void);
    void                searchTimeSensor(void);
    void                changeEnableAnimation(void);
    void                changeDisableAnimation(void);
    void                changeAnimationTime(void);
    void                deleteAnimationData(void);
    void                changeTransparency(void);
    void                changeShininess(void);
    void                changeImageRepeat(void);
    void                createOneText(void);
    void                changeOneText(void);
    void                linearUknot(void);
    void                linearVknot(void);
    bool                isEditorInUse(bool errormessage = false);
    bool                fieldPipe(Node *node, int field, const char *command,
                                  MyArray<int> *handles = NULL);
    void                pipeField(void);
    int                 considerIdRanges(int id);
    void                HandleScale(void);
    void                HandleSize(void);
    void                HandleEpsilon(void);
    void                setName(int id);
    void                createText(int id);
    void                toogleMaterialName(int id);
#ifdef HAVE_SAND
    void OnSANDExport(void);
#endif

protected:
    bool                m_destroyed;
    SWND                m_parentWindow;
    SMENU               m_menu;
    StatusBar          *m_statusBar;
    ToolbarWindow      *m_toolbarWindow;
    ToolbarWindow      *m_toolbarWindow2;
    PanedWindow        *m_innerPane;
    PanedWindow        *m_outerPane;
    SceneView          *m_treeView;
    SceneView          *m_S3DView;
    SceneView          *m_fieldView;
    bool                m_showNumbers4Kids;
    SceneView          *m_graphView;
    ChannelView        *m_channelView;
    STOOLBAR            m_standardToolbar;
    STOOLBAR            m_nodeToolbar1;
    bool                m_nodeToolbar1Enabled;
    STOOLBAR            m_nodeToolbar2;
    bool                m_nodeToolbar2Enabled;
    STOOLBAR            m_nodeToolbar3;
    bool                m_nodeToolbar3Enabled;
    STOOLBAR            m_nodeToolbarVRML200x;
    bool                m_nodeToolbarVRML200xEnabled;
    STOOLBAR            m_nodeToolbarX3DComponents1;
    bool                m_nodeToolbarX3DComponents1Enabled;
    STOOLBAR            m_nodeToolbarX3DComponents2;
    bool                m_nodeToolbarX3DComponents2Enabled;
    STOOLBAR            m_nodeToolbarX3DComponents3;
    bool                m_nodeToolbarX3DComponents3Enabled;
    STOOLBAR            m_nodeToolbarX3DComponents4;
    bool                m_nodeToolbarX3DComponents4Enabled;
    STOOLBAR            m_nodeToolbarCover;
    bool                m_nodeToolbarCoverEnabled;
    STOOLBAR            m_nodeToolbarKambi;
    bool                m_nodeToolbarKambiEnabled;
    STOOLBAR            m_nodeToolbarX3dom;
    bool                m_nodeToolbarX3domEnabled;
    STOOLBAR            m_nodeToolbarScripted;
    bool                m_nodeToolbarScriptedEnabled;
    STOOLBAR            m_vcrToolbar;
    STIMER              m_timer;
    bool                m_fullScreen_enabled;
    SceneView          *m_colorCircle;    
    FieldUpdate        *m_colorCircleHint;
    bool                m_colorCircle_active;
    bool                m_colorCircle_enabled;
    SceneView          *m_vertexModifier;    
    FieldUpdate        *m_vertexModifierHint;
    bool                m_vertexModifier_active;
    bool                m_vertexModifier_enabled;
    bool                m_vertexModifier_overwrite;
    bool                m_objectEdit_enabled;
    bool                m_urlEdit_enabled;
    bool                m_navigation_mouse_active;
    bool                m_navigation_input_device_active;
    int                 m_mouseMode;
    bool                m_xOnly;
    bool                m_yOnly;
    bool                m_zOnly;
    SWND                m_fieldCanvas;
    MyArray<ProtoMenuItem> m_protoMenu;
    ScriptEdit         *m_scriptEdit;
    bool                m_scriptEditorInUse;
    ObjectEdit         *m_textEdit;
    bool                m_textEditorInUse;
    ObjectEdit         *m_shaderEdit;
    ImageTextureEdit   *m_imageTextureEdit;
    PixelTextureEdit   *m_pixelTextureEdit;
    bool                m_imageEditorInUse;
    MovieTextureEdit   *m_movieEdit;
    bool                m_movieEditorInUse;
    AudioClipEdit      *m_soundEdit;
    bool                m_soundEditorInUse;
    int                 m_selectedField;
    const char *        m_statusText;
    MyString            m_searchText;
    Node               *m_vrmlCutNode;
    MyString            m_fieldPipeFilterNode;
    MyString            m_fieldPipeFilterField;
    MyString            m_fieldPipeCommand;
#ifdef HAVE_SAND
    ExportNebula        m_nebulaExporter;
#endif
};

#endif /* _MAIN_WINDOW_H */
