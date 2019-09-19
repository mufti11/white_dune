# This Makefile is free software; the Free Software Foundation
# gives unlimited permission to copy, distribute and modify it.

CC = gcc
CXX = g++
RANLIB = /usr/bin/ranlib 
CXXCPP = g++ -E
YACC = bison -y
LEX = flex --nounistd
AWK = gawk
PREFIX = /usr
EXEC_PREFIX = ${prefix}
GREP_FLAGS = --binary-files=text

RESOURCE_FILES = dune.english.rc singleResources.rc dune.french.rc dune.german.rc dune.italian.rc dune.portuguese.rc

LIBEFENCE_A = 
EFENCE_EXTRA_LIBS = 

LIBDUMA_A = 
DUMA_EXTRA_LIBS = 

LD_OPTIONS = 

EGREP_PROBLEMS = no

# gcc Warning flags (a lot of -Wno- comes from vcglib usage)
# WARNINGS = -Wall -D_FORTIFY_SOURCE=2 -Werror -Wno-error=strict-overflow \
#            -Wno-error=unused-function -Wno-error=ignored-attributes \
#            -Wno-error=deprecated-declarations -Wno-ignored-attributes \
#            -Wno-deprecated-declarations -Wno-misleading-indentation \
#            -Wno-error=int-in-bool-context -Wno-error=sign-compare \
#            -Wno-sign-compare -Wno-error=nonnull-compare

# IRIX cc flags
# WARNINGS = -woff 3303
# LFLAGS = -woff 1167 -woff 1110 -woff 3322

# use -DYYDEBUG=1 for yacc/bison debugging 
#(also change int yydebug = 1; in parser.cpp)

CPPFLAGS= -I. -Iswt/include  -I/usr/local/include -I/usr/local/include/eigenlib -I/usr/include/vcglib -I.//src -I.//vcglib -I.//vcglib/eigenlib -I/root/vcglib -I.//../vcglib -I/root/vcglib/eigenlib -I.//../vcglib/eigenlib -I/usr/include/freetype2 -I/usr/include/libpng16 -I/usr/include/ffmpeg/   -I/usr/X11R6/include/ -I/usr/X11R6/LessTif/Motif1.2/include -I/usr/local/include -I/usr/local/include
COMMON_FLAGS = $(WARNINGS) -I. -Iswt/include  -I/usr/local/include -I/usr/local/include/eigenlib -I/usr/include/vcglib -I.//src -I.//vcglib -I.//vcglib/eigenlib -I/root/vcglib -I.//../vcglib -I/root/vcglib/eigenlib -I.//../vcglib/eigenlib -I/usr/include/freetype2 -I/usr/include/libpng16 -I/usr/include/ffmpeg/   -I/usr/X11R6/include/ -I/usr/X11R6/LessTif/Motif1.2/include -I/usr/local/include -I/usr/local/include 
CFLAGS_BIG = -O2 -g $(COMMON_FLAGS)  -fopenmp -g -O2 -fopenmp
CFLAGS = $(COMMON_FLAGS)  -fopenmp
CXXFLAGS = -O2 -g $(COMMON_FLAGS) -g -O2 -fopenmp
LIBS =   -L/usr/local/lib -L/usr/local/lib -O2 -g -Lswt/lib -lswt -LSDLjoystick  -lusb -lsball -lswresample -lswscale -lavformat -lavutil -lavcodec -ljpeg -lpng -lz -lexpat -lGLU -lGL -lcurl -lXi -lmpfr -lgmp -lfreetype -lXm -lXt -lXmu -lX11 -lm -lgomp   -L/usr/local/lib  -L/usr/X11R6/LessTif/Motif1.2/lib -lCGAL -losdCPU 
NEBULA = 
PARSER_CPP_CFLAG = 

.SUFFIXES: .cpp

.cpp.o:
	$(CXX) $(CXXFLAGS) -c $<

NODECLASSES = \
        NodeAnchor.o \
	NodeAppearance.o \
	NodeAudioClip.o \
	NodeArc2D.o \
	NodeArcClose2D.o \
	NodeBackground.o \
	NodeBallJoint.o \
	NodeBillboard.o \
        NodeBlendedVolumeStyle.o \
	NodeBooleanFilter.o \
	NodeBooleanSequencer.o \
	NodeBooleanToggle.o \
	NodeBooleanTrigger.o \
        NodeBoundaryEnhancementVolumeStyle.o \
        NodeBoundedPhysicsModel.o \
	NodeBox.o \
        NodeCADAssembly.o \
        NodeCADFace.o \
        NodeCADLayer.o \
        NodeCADPart.o \
        NodeCartoonVolumeStyle.o \
	NodeCircle2D.o \
	NodeClipPlane.o \
        NodeCollidableShape.o \
        NodeCollidableOffset.o \
	NodeCollision.o \
        NodeCollisionCollection.o \
        NodeCollisionSensor.o \
        NodeCollisionSpace.o \
	NodeColor.o \
        NodeColorChaser.o \
        NodeColorDamper.o \
	NodeColorRGBA.o \
	NodeColorInterpolator.o \
        NodeCommonSurfaceShader.o \
        NodeComposedCubeMapTexture.o \
        NodeComposedShader.o \
        NodeComposedTexture3D.o \
        NodeComposedVolumeStyle.o \
	NodeCone.o \
        NodeConeEmitter.o \
	NodeContact.o \
	NodeContour2D.o \
	NodeContourPolyline2D.o \
	NodeCoordinate.o \
        NodeCoordinateChaser.o \
        NodeCoordinateDamper.o \
	NodeCoordinateDeformer.o \
	NodeCoordinateDouble.o \
	NodeCoordinateInterpolator.o \
	NodeCoordinateInterpolator2D.o \
	NodeCylinder.o \
	NodeCylinderSensor.o \
        NodeDISEntityManager.o \
        NodeDISEntityTypeMapping.o \
	NodeDirectionalLight.o \
	NodeDisk2D.o \
        NodeDoubleAxisHingeJoint.o \
        NodeEaseInEaseOut.o \
	NodeElevationGrid.o \
        NodeEspduTransform.o \
        NodeExplosionEmitter.o \
	NodeExtrusion.o \
        NodeFillProperties.o \
        NodeFloatVertexAttribute.o \
	NodeFog.o \
        NodeFogCoordinate.o \
	NodeFontStyle.o \
        NodeForcePhysicsModel.o \
	NodeGeoCoordinate.o \
	NodeGeoElevationGrid.o \
        NodeGeoMetadata.o \
        NodeGeoLocation.o \
        NodeGeoLOD.o \
        NodeGeoPositionInterpolator.o \
        NodeGeoProximitySensor.o \
        NodeGeoOrigin.o \
        NodeGeoTouchSensor.o \
        NodeGeoTransform.o \
        NodeGeoViewpoint.o \
        NodeGeneratedCubeMapTexture.o \
	NodeGroup.o \
        NodeHAnimDisplacer.o \
        NodeHAnimHumanoid.o \
        NodeHAnimJoint.o \
        NodeHAnimSegment.o \
        NodeHAnimSite.o \
        NodeImageCubeMapTexture.o \
	NodeImageTexture.o \
        NodeImageTexture3D.o \
        NodeIndexedFaceSet.o \
	NodeIndexedLineSet.o \
        NodeIndexedQuadSet.o \
	NodeIndexedTriangleFanSet.o \
	NodeIndexedTriangleSet.o \
	NodeIndexedTriangleStripSet.o \
	NodeInline.o \
        NodeInlineLoadControl.o \
	NodeIntegerSequencer.o \
	NodeIntegerTrigger.o \
        NodeKeySensor.o \
        NodeLayer.o \
        NodeLayerSet.o \
        NodeLayout.o \
        NodeLayoutGroup.o \
        NodeLayoutLayer.o \
        NodeLinePickSensor.o \
        NodeLineProperties.o \
        NodeLineSet.o \
        NodeLoadSensor.o \
        NodeLocalFog.o \
	NodeLOD.o \
	NodeMaterial.o \
        NodeMatrix3VertexAttribute.o \
        NodeMatrix4VertexAttribute.o \
        NodeMetadataBoolean.o \
	NodeMetadataDouble.o \
	NodeMetadataFloat.o \
	NodeMetadataInteger.o \
	NodeMetadataSet.o \
	NodeMetadataString.o \
        NodeMotorJoint.o \
	NodeMovieTexture.o \
	NodeMultiTexture.o \
	NodeMultiTextureCoordinate.o \
	NodeMultiTextureTransform.o \
	NodeNavigationInfo.o \
	NodeNormal.o \
	NodeNormalInterpolator.o \
	NodeNurbsCurve.o \
	NodeNurbsCurve2D.o \
	NodeNurbsGroup.o \
	NodeNurbsOrientationInterpolator.o \
	NodeNurbsPositionInterpolator.o \
	NodeNurbsSet.o \
	NodeNurbsSurface.o \
	NodeNurbsSurfaceInterpolator.o \
        NodeNurbsSweptSurface.o \
        NodeNurbsSwungSurface.o \
        NodeNurbsTextureCoordinate.o \
	NodeNurbsTextureSurface.o \
        NodeNurbsTrimmedSurface.o \
        NodeOrientationChaser.o \
        NodeOrientationDamper.o \
	NodeOrientationInterpolator.o \
        NodeOrthoViewpoint.o \
        NodePackagedShader.o \
        NodeParticleSystem.o \
        NodePickableGroup.o \
	NodePixelTexture.o \
        NodePixelTexture3D.o \
	NodePlaneSensor.o \
        NodePointEmitter.o \
	NodePointLight.o \
        NodePointPickSensor.o \
	NodePointSet.o \
	NodePolyline2D.o \
        NodePolylineEmitter.o \
	NodePolypoint2D.o \
        NodePositionChaser.o \
        NodePositionChaser2D.o \
        NodePositionDamper.o \
        NodePositionDamper2D.o \
	NodePositionInterpolator.o \
	NodePositionInterpolator2D.o \
        NodePrimitivePickSensor.o \
        NodeProgramShader.o \
        NodeProjectionVolumeStyle.o \
	NodeProximitySensor.o \
        NodeQuadSet.o \
        NodeReceiverPdu.o \
	NodeRectangle2D.o \
        NodeRigidBody.o \
        NodeRigidBodyCollection.o \
        NodeScalarChaser.o \
        NodeScalarDamper.o \
	NodeScalarInterpolator.o \
        NodeScreenFontStyle.o \
        NodeScreenGroup.o \
	NodeScript.o \
        NodeShaderPart.o \
        NodeShaderProgram.o \
	NodeShape.o \
        NodeSignalPdu.o \
        NodeSingleAxisHingeJoint.o \
        NodeSliderJoint.o \
	NodeSound.o \
	NodeSphere.o \
	NodeSphereSensor.o \
        NodeSplinePositionInterpolator.o \
        NodeSplinePositionInterpolator2D.o \
        NodeSplineScalarInterpolator.o \
	NodeSpotLight.o \
        NodeSquadOrientationInterpolator.o \
	NodeStaticGroup.o \
	NodeStringSensor.o \
        NodeSurfaceEmitter.o \
	NodeSwitch.o \
        NodeTexCoordChaser2D.o \
        NodeTexCoordDamper2D.o \
	NodeText.o \
	NodeTextureBackground.o \
	NodeTextureCoordinate.o \
        NodeTextureCoordinate3D.o \
        NodeTextureCoordinate4D.o \
	NodeTextureCoordinateGenerator.o \
	NodeTextureProperties.o \
	NodeTextureTransform.o \
        NodeTextureTransform3D.o \
        NodeTextureTransformMatrix3D.o \
	NodeTimeSensor.o \
        NodeTimeTrigger.o \
	NodeTouchSensor.o \
	NodeTransform.o \
        NodeTransformSensor.o \
        NodeTransmitterPdu.o \
        NodeTriangleFanSet.o \
        NodeTriangleSet.o \
        NodeTriangleSet2D.o \
        NodeTriangleStripSet.o \
        NodeTrimmedSurface.o \
        NodeTwoSidedMaterial.o \
        NodeUniversalJoint.o \
	NodeViewpoint.o \
        NodeViewpointGroup.o \
        NodeViewport.o \
	NodeVisibilitySensor.o \
        NodeVolumeEmitter.o \
        NodeVolumePickSensor.o \
        NodeWindPhysicsModel.o \
        NodeEdgeEnhancementVolumeStyle.o \
        NodeIsoSurfaceVolumeData.o \
        NodeOpacityMapVolumeStyle.o \
        NodeSegmentedVolumeData.o \
        NodeShadedVolumeStyle.o \
        NodeSilhouetteEnhancementVolumeStyle.o \
        NodeToneMappedVolumeStyle.o \
        NodeVolumeData.o \
	NodeWorldInfo.o \
        NodeComment.o \
        NodeImport.o \
        NodeExport.o

EXTRACLASSES = \
        NodeSuperEllipsoid.o \
        NodeSuperShape.o \
        NodeSuperRevolver.o \
        NodeSuperExtrusion.o \
	NodeVrmlCut.o \
	NodeVrmlScene.o \
        NodeCurveAnimation.o \
	NodeCOVER.o \
	NodeSpaceSensor.o \
	NodeCubeTexture.o \
	NodeWave.o \
        NodeJoystickSensor.o \
        NodeLabView.o \
        NodeSteeringWheel.o \
        NodeVehicle.o \
        NodeARSensor.o \
        NodeSky.o \
        NodeTUIButton.o \
        NodeTUIComboBox.o \
        NodeTUIFloatSlider.o \
        NodeTUIFrame.o \
        NodeTUILabel.o \
        NodeTUIListBox.o \
        NodeTUIMap.o \
        NodeTUIProgressBar.o \
        NodeTUISlider.o \
        NodeTUISplitter.o \
        NodeTUITab.o \
        NodeTUITabFolder.o \
        NodeTUIToggleButton.o \
        NodeVirtualAcoustics.o \
        NodeVirtualSoundSource.o \
        NodeCubicBezier2DOrientationInterpolator.o \
        NodeCubicBezierPositionInterpolator.o \
        NodeBlendMode.o \
        NodeColorSetInterpolator.o \
        NodeEffect.o \
        NodeEffectPart.o \
        NodeGeneratedShadowMap.o \
        NodeMatrixTransform.o \
        NodeMultiGeneratedTextureCoordinate.o \
	NodeKambiAppearance.o \
	NodeKambiHeadLight.o \
        NodeKambiInline.o \
        NodeKambiNavigationInfo.o \
        NodeKambiOctreeProperties.o \
	NodeKambiTriangulation.o \
	NodeProjectedTextureCoordinate.o \
        NodeRenderedTexture.o \
        NodeScreenEffect.o \
        NodeTeapot.o \
	NodeText3D.o \
        NodeShaderTexture.o \
        NodeVectorInterpolator.o \
        NodeBinaryGeometry.o \
        NodeBlock.o \
        NodeColorMaskMode.o \
        NodeDepthMode.o \
        NodeDish.o \
        NodeDynamicLOD.o \
        NodeEnvironment.o \
        NodeExternalGeometry.o \
        NodeField.o \
        NodeImageGeometry.o \
        NodeImageTextureAtlas.o \
        NodeNozzle.o \
        NodeMesh.o \
        NodeMPRVolumeStyle.o \
        NodeMultiPart.o \
        NodeParam.o \
        NodeParticleSet.o \
        NodePlane.o \
        NodePopGeometry.o \
        NodePopGeometryLevel.o \
        NodePyramid.o \
        NodeRadarVolumeStyle.o \
        NodeRectangularTorus.o \
        NodeRefinementTexture.o \
        NodeRemoteSelectionGroup.o \
        NodeSlopedCylinder.o \
        NodeSnout.o \
        NodeSolidOfRevolution.o \
        NodeSphereSegment.o \
        NodeStippleVolumeStyle.o \
        NodeSurfaceShaderTexture.o \
        NodeTorus.o \
        NodeUniform.o \
        NodeViewfrustum.o \
        NodeCattExportRec.o \
        NodeCattExportSrc.o \
        NodeLdrawDatExport.o \
        NodeWonderlandImportJava.o

NODEPARENTCLASSES = \
        BasicTransformNode.o \
        TransformNode.o \
        DynamicFieldsNode.o \
        GeoNode.o \
        GroupNode.o \
        StaticGroupNode.o \
        MeshBasedNode.o \
        MeshMorphingNode.o \
        ChainBasedNode.o \
        EmitterNode.o \
        GeometryNode.o \
        IndexedTriangleSetNode.o \
        X3DNBodyCollidableNode.o \
        X3DNBodyCollisionSpaceNode.o \
        X3DRigidJointNode.o \
        AnchoredJointNode.o \
        Texture3DNode.o \
        FontStyleNode.o \
        ViewpointNode.o \
        DamperNode.o \
        ChaserNode.o

TYPECLASSES = \
	SFBool.o \
        SFBoolItem.o \
	SFColor.o \
        SFColorItem.o \
	SFColorRGBA.o \
        SFColorRGBAItem.o \
	SFDouble.o \
        SFDoubleItem.o \
	SFFloat.o \
        SFFloatItem.o \
	SFImage.o \
        SFImageItem.o \
	SFInt32.o \
        SFInt32Item.o \
	SFMatrix3f.o \
        SFMatrix3fItem.o \
	SFMatrix4f.o \
        SFMatrix4fItem.o \
	SFNode.o \
        SFNodeItem.o \
	SFRotation.o \
        SFRotationItem.o \
	SFString.o \
        SFStringItem.o \
	SFTime.o \
        SFTimeItem.o \
	SFVec2f.o \
        SFVec2fItem.o \
	SFVec2d.o \
        SFVec2dItem.o \
	SFVec3f.o \
        SFVec3fItem.o \
	SFVec3d.o \
        SFVec3dItem.o \
	SFVec4f.o \
        SFVec4fItem.o \
	MFBool.o \
        MFBoolItem.o \
	MFColor.o \
        MFColorItem.o \
	MFColorRGBA.o \
        MFColorRGBAItem.o \
	MFDouble.o \
        MFDoubleItem.o \
	MFInt32.o \
        MFInt32Item.o \
	MFFloat.o \
        MFFloatItem.o \
	MFNode.o \
        MFNodeItem.o \
	MFRotation.o \
        MFRotationItem.o \
	MFString.o \
        MFStringItem.o \
	MFTime.o \
        MFTimeItem.o \
	MFVec2f.o \
        MFVec2fItem.o \
	MFVec2d.o \
        MFVec2dItem.o \
	MFVec3f.o \
        MFVec3fItem.o \
	MFVec3d.o \
        MFVec3dItem.o \
	MFVec4f.o \
        MFVec4fItem.o \
        MFMatrix3f.o \
        MFMatrix3fItem.o \
        MFMatrix4f.o \
        MFMatrix4fItem.o

DIALOGCLASSES = \
	Dialog.o \
	ElevationGridDialog.o \
        ScriptDialog.o \
	NurbsCurveDialog.o \
	NurbsPlaneDialog.o \
	ModellingSettingsDialog.o \
        RenderSettingsDialog.o \
        DefDialog.o \
        CoverDefDialog.o \
	Box2NurbsDialog.o \
	Cone2NurbsDialog.o \
	Cylinder2NurbsDialog.o \
	Sphere2NurbsDialog.o \
        NurbsCurve2NurbsSurfDialog.o \
        InteractionDialog.o \
        InteractionDialog2.o \
        InteractionDialog3.o \
        AnimationDialog.o \
        CurveAnimationDialog.o \
        URLDialog.o \
        ArrayDialog.o \
        FieldPipeDialog.o \
        OneBoolDialog.o \
        OneFloatDialog.o \
        OneIntDialog.o \
        OneTextDialog.o \
        TwoRadioButtonsDialog.o \
	EcmaScriptSettingsDialog.o \
	PreviewSettingsDialog.o \
	UploadSettingsDialog.o \
	HelpSettingsDialog.o \
	TexteditSettingsDialog.o \
	StereoViewSettingsDialog.o \
        OutputSettingsDialog.o \
        InputDeviceSelectionDialog.o \
        InputDeviceNameSelectionDialog.o \
        InputDeviceSettingsDialog.o \
        AflockSettingsDialog.o \
        RouteViewSettingsDialog.o \
        WonderlandModuleExportDialog.o \
        ProtoNameDialog.o \
        ScriptEventDialog.o \
        CattExportDialog.o \
        ProtoDialog.o \
        SplitFacesDialog.o \
        ToIndexedFaceSetDialog.o \
        HAnimJointDialog.o \
        ViewpointDialog.o \
        NurbsCurve2DDialog.o \
        InsertToNurbsCurveDialog.o \
        InsertToNurbsSurfaceDialog.o \
        SliderFloatDialog.o

APPCLASSES = \
	DuneApp.o \
        PreferencesApp.o \
        EcmaScriptApp.o \
        StereoViewApp.o \
        InputDeviceApp.o \
        OutputApp.o \
        RouteViewApp.o \
        StartWithApp.o \
        ExportNamesApp.o \
        ModellingApp.o \
        RenderApp.o

DUNECLASSES = \
	AboutDialog.o \
	CommandList.o \
	Element.o \
	EventIn.o \
	EventOut.o \
	ExposedField.o \
	Field.o \
	FieldCommand.o \
	MFieldCommand.o \
        TUIElement.o \
	FaceData.o \
	FieldValue.o \
	Image.o \
	Interpolator.o \
	Matrix.o \
	Metadata.o \
	MoveCommand.o \
	NodeList.o \
	Path.o \
	Proto.o \
	RouteCommand.o \
	MyString.o \
	UnRouteCommand.o \
	MyMesh.o \
	SelectionCommand.o \
	Quaternion.o \
	RenderState.o \
	SceneTreeView.o \
	SceneGraphView.o \
	Scene3DView.o \
	SceneView.o \
	FieldView.o \
	FieldViewItem.o \
	Util.o \
	URL.o \
	PanedWindow.o \
	StatusBar.o \
	ChannelView.o \
	ToolbarWindow.o \
	EulerAngles.o \
        xerrorhandler.o \
        InputDevice.o \
        TransformMode.o \
        ColorCircle.o \
        Aflock.o \
        ScriptEdit.o \
        Types.o \
        Commandline.o \
	NurbsArc.o\
	NurbsCurveDegreeElevate.o \
	NurbsSurfaceDegreeElevate.o \
	NurbsMakeRevolvedSurface.o \
        CheckBoxWindow.o \
        GpsMathLib.o \
        WonderlandModuleExport.o \
        WriteWonderlandCellRenderCode.o \
        MeshSubdivide.o \
        NurbsCurve.o \
        Vec3x.o \
        quadric_simp.o \
        VertexModifier.o \
        illegal2vrml.o $(NEBULA)

BIGDUNECLASSES = \
        Scene.o \
        SceneProtoMap.o \
	Node.o \
        MainWindow.o

FONT23DCLASSES = \
        advancing_front.o \
        sweep.o \
        sweep_context.o \
        cdt.o \
        shapes.o \
        Contour.o \
        PointFtgl.o \
        Vectoriser.o

CLASSES = \
        $(DUNECLASSES) \
        $(APPCLASSES) \
        $(DIALOGCLASSES) \
        $(TYPECLASSES) \
        $(NODEPARENTCLASSES) \
        $(NODECLASSES) \
        $(EXTRACLASSES) \
        $(FONT23DCLASSES) \
	parser.o \
	lexer.o

OBJSC = \
        buf.o \
        mathx.o \
        subd_mesh.o \
        subd.o \
        openglutfont.o \
	jpegLoad.o \
	pngLoad.o gif.o

OBJS = $(BIGDUNECLASSES) $(CLASSES) $(OBJSC) resource.o mysnprintf.o 

ARCHIVES = MainWindow.o SceneProtoMap.o Scene.o parser.o lexer.o \
           libdune_extra.a libdune_nodes.a libdune_nodeparents.a \
           libdune_dialogs.a libdune_app.a libdune_dune.a \
           Node.o libdune_type.a libdune_c.a resource.o mysnprintf.o

SRCSC = $(OBJSC:.o=.c) mysnprintf.c
SRCSDUNECLASSES = $(DUNECLASSES:.o=.cpp)
SRCSAPPCLASSES = $(APPCLASSES:.o=.cpp)
SRCSDIALOGCLASSES = $(DIALOGCLASSES:.o=.cpp)
SRCSBIGDUNECLASSES = $(BIGDUNECLASSES:.o=.cpp)
SRCSTYPECLASSES = $(TYPECLASSES:.o=.cpp)
SRCSNODEPARENTCLASSES = $(NODEPARENTCLASSES:.o=.cpp)
SRCSNODECLASSES = $(NODECLASSES:.o=.cpp)
SRCSEXTRACLASSES = $(EXTRACLASSES:.o=.cpp)

TESTOJBS = selftest.o allNodesTest.o allNodesTestX3d.o \
           allNodesTestCover.o allNodesTestKambi.o \
           allNodesTestX3dom.o \
           printFieldType.o printNodeType.o printFieldNumber.o \
           testzeroclass.o testprotos.o 

YFLAGS = -d

target: mysnprintf.o
	if test "X_" = "X_SDLjoystick/libSDLjoystick.a" ;then cd SDLjoystick;$(MAKE); fi
	$(MAKE) SFMFTypes.h
	(cd swt ; $(MAKE))
#	$(MAKE) ../bin/png2PixelTexture
	$(MAKE) ../bin/dune

install: target
	install -m 755 -p ../bin/dune /usr/bin
	install -m 644 -p ../man/dune.1 /usr/share/man/man1
	install -m 644 -p ../desktop/kde/dune.desktop /usr/share/applications/dune.desktop
	install -m 644 -p ../desktop/kde/dune.png /usr/share/pixmaps/dune.png
	install -m 644 -p ../desktop/kde/dune4kids.desktop /usr/share/applications/dune4kids.desktop
	install -m 644 -p ../desktop/kde/dune4kids.png /usr/share/pixmaps/dune4kids.png

mysnprintf.o: mysnprintf.c
	$(CC) -g $(CFLAGS) -std=c99 -c -o $@ mysnprintf.c

debug_mysnprintf.o: mysnprintf.c
	$(CC) -g $(CFLAGS) -DDEBUG=1 -std=c99 -c -o mysnprintf.o mysnprintf.c

debug_Path.o: Path.cpp
	$(CXX) $(CFLAGS) -DDEBUG=1 -c -o Path.o Path.cpp

Node.o: Node.cpp
	$(CXX) $(CFLAGS_BIG) -c -o $@ Node.cpp

Scene.o: Scene.cpp
	$(CXX) $(CFLAGS_BIG) -c -o $@ Scene.cpp

SceneProtoMap.o: SceneProtoMap.cpp
	$(CXX) $(CFLAGS_BIG) -c -o $@ SceneProtoMap.cpp

nebula/ExportNebula.o: nebula/ExportNebula.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ nebula/ExportNebula.cpp

../bin/dune: main.o $(OBJS) swt/lib/libswt.a  
	$(CXX) main.o $(OBJS) $(LIBS) -o $@
	sh -c "if test `uname` = IRIX64 ; then tag 0x00ff1a00 $@ ; fi"
	sh -c "if test `uname` = IRIX   ; then tag 0x00ff1a00 $@ ; fi"
	sh -c "if test `uname` = Darwin ; then (if test -d ./desktop/macosx; then cd ../desktop/macosx/ && tar -xvf white_dune_tiger.app.tar && cd white_dune.app/Contents/MacOS && ln -sf ../../../../../bin/dune . ; fi) ; fi"

../bin/efencedune:  main.o $(OBJS) 
	(cd swt ; $(MAKE) efence)
	rm -f /tmp/efencedune.o
	echo no way to extract strdup efence may fail in strdup
	ld $(LD_OPTIONS) -r -o /tmp/efencedune.o  main.o $(OBJS) swt/lib/libswt.a  $(LIBEFENCE_A)
	perl ../batch/bin_gsub.pl /tmp/efencedune.o malloc madune
	perl ../batch/bin_gsub.pl /tmp/efencedune.o calloc cadune
	perl ../batch/bin_gsub.pl /tmp/efencedune.o valloc vadune
	perl ../batch/bin_gsub.pl /tmp/efencedune.o realloc readune
	perl ../batch/bin_gsub.pl /tmp/efencedune.o free dune
	perl ../batch/bin_gsub.pl /tmp/efencedune.o memalign memadune
	$(CXX) $(CXXFLAGS) /tmp/efencedune.o $(LIBS) $(EFENCE_EXTRA_LIBS) -o $@
	rm -f /tmp/efencedune.o

../bin/dumadune:  main.o $(OBJS) 
	(cd swt ; $(MAKE) efence)
	rm -f /tmp/dumadune.o
	echo no way to extract strdup efence may fail in strdup
	ld $(LD_OPTIONS) -r -o /tmp/dumadune.o  main.o $(OBJS) swt/lib/libswt.a  $(LIBDUMA_A)
	perl ../batch/bin_gsub.pl /tmp/dumadune.o malloc madune
	perl ../batch/bin_gsub.pl /tmp/dumadune.o calloc cadune
	perl ../batch/bin_gsub.pl /tmp/dumadune.o valloc vadune
	perl ../batch/bin_gsub.pl /tmp/dumadune.o realloc readune
	perl ../batch/bin_gsub.pl /tmp/dumadune.o free dune
	perl ../batch/bin_gsub.pl /tmp/dumadune.o memalign memadune
	$(CXX) $(CXXFLAGS) /tmp/dumadune.o $(LIBS) $(DUMA_EXTRA_LIBS) -o $@
	rm -f /tmp/dumadune.o

libdune_nodeparents.a: $(NODEPARENTCLASSES)
	ar -r libdune_nodeparents.a $(NODEPARENTCLASSES)
	$(RANLIB) libdune_nodeparents.a

libdune_nodes.a: $(NODECLASSES)
	ar -r libdune_nodes.a $(NODECLASSES)
	$(RANLIB) libdune_nodes.a

libdune_extra.a: $(EXTRACLASSES)
	ar -r libdune_extra.a $(EXTRACLASSES)
	$(RANLIB) libdune_extra.a

libdune_type.a: $(TYPECLASSES)
	ar -r libdune_type.a $(TYPECLASSES)
	$(RANLIB) libdune_type.a

libdune_app.a: $(APPCLASSES)
	ar -r libdune_app.a $(APPCLASSES)
	$(RANLIB) libdune_app.a

libdune_dialogs.a: $(DIALOGCLASSES)
	ar -r libdune_dialogs.a $(DIALOGCLASSES)
	$(RANLIB) libdune_dialogs.a

libdune_dune.a: $(DUNECLASSES) 
	ar -r libdune_dune.a $(DUNECLASSES)
	$(RANLIB) libdune_dune.a

libdune_c.a: $(OBJSC)
	ar -r libdune_c.a $(OBJSC)
	$(RANLIB) libdune_c.a

catt_test/material.geo:
	echo '; file to store absorption and scattering coefficients and RGB-Color' > $@
	echo 'ABS unknown = <15 13 10 9 9 7 > L <30 30 30 30 30 30> { 255 255 0 }' >> $@

selftest: mysnprintf.o 
	if test "X_" = "X_SDLjoystick/libSDLjoystick.a" ;then cd SDLjoystick;$(MAKE); fi
	(cd swt ; $(MAKE))
	$(MAKE) ../test/dune_selftest

selftest.o: ../test/selftest.cpp
	$(CXX) $(CXXFLAGS) -c ../test/selftest.cpp -o $@

../test/dune_selftest: selftest.o  $(OBJS) swt/lib/libswt.a 
	$(CXX) $(CXXFLAGS) selftest.o $(OBJS) $(LIBS)  -o $@

allNodesTest: mysnprintf.o
	if test "X_" = "X_SDLjoystick/libSDLjoystick.a" ;then cd SDLjoystick;$(MAKE); fi
	$(MAKE) SFMFTypes.h
	(cd swt ; $(MAKE))
	$(MAKE) ../test/dune_allNodesTest

allNodesTest.o: ../test/allNodesTest.cpp
	$(CXX) $(CXXFLAGS) -c ../test/allNodesTest.cpp -o $@

../test/dune_allNodesTest: allNodesTest.o  $(OBJS) swt/lib/libswt.a 
	$(CXX) $(CXXFLAGS) allNodesTest.o $(OBJS) $(LIBS)  -o $@

allNodesTestX3d: mysnprintf.o
	if test "X_" = "X_SDLjoystick/libSDLjoystick.a" ;then cd SDLjoystick;$(MAKE); fi
	$(MAKE) SFMFTypes.h
	(cd swt ; $(MAKE))
	$(MAKE) ../test/dune_allNodesTestX3d

allNodesTestX3d.o: ../test/allNodesTestX3d.cpp
	$(CXX) $(CXXFLAGS) -c ../test/allNodesTestX3d.cpp -o $@

../test/dune_allNodesTestX3d: allNodesTestX3d.o  $(OBJS) swt/lib/libswt.a 
	$(CXX) $(CXXFLAGS) allNodesTestX3d.o $(OBJS) $(LIBS)  -o $@

allNodesTestCover: mysnprintf.o
	if test "X_" = "X_SDLjoystick/libSDLjoystick.a" ;then cd SDLjoystick;$(MAKE); fi
	$(MAKE) SFMFTypes.h
	(cd swt ; $(MAKE))
	$(MAKE) ../test/dune_allNodesTestCover

allNodesTestCover.o: ../test/allNodesTestCover.cpp
	$(CXX) $(CXXFLAGS) -c ../test/allNodesTestCover.cpp -o $@

../test/dune_allNodesTestCover: allNodesTestCover.o  $(OBJS) swt/lib/libswt.a 
	$(CXX) $(CXXFLAGS) allNodesTestCover.o $(OBJS) $(LIBS)  -o $@

allNodesTestKambi: mysnprintf.o
	if test "X_" = "X_SDLjoystick/libSDLjoystick.a" ;then cd SDLjoystick;$(MAKE); fi
	$(MAKE) SFMFTypes.h
	(cd swt ; $(MAKE))
	$(MAKE) ../test/dune_allNodesTestKambi

allNodesTestKambi.o: ../test/allNodesTestKambi.cpp
	$(CXX) $(CXXFLAGS) -c ../test/allNodesTestKambi.cpp -o $@

../test/dune_allNodesTestKambi: allNodesTestKambi.o  $(OBJS) swt/lib/libswt.a 
	$(CXX) $(CXXFLAGS) allNodesTestKambi.o $(OBJS) $(LIBS)  -o $@

allNodesTestX3dom: mysnprintf.o
	if test "X_" = "X_SDLjoystick/libSDLjoystick.a" ;then cd SDLjoystick;$(MAKE); fi
	$(MAKE) SFMFTypes.h
	(cd swt ; $(MAKE))
	$(MAKE) ../test/dune_allNodesTestX3dom

allNodesTestX3dom.o: ../test/allNodesTestX3dom.cpp
	$(CXX) $(CXXFLAGS) -c ../test/allNodesTestX3dom.cpp -o $@

../test/dune_allNodesTestX3dom: allNodesTestX3dom.o  $(OBJS) swt/lib/libswt.a 
	$(CXX) $(CXXFLAGS) allNodesTestX3dom.o $(OBJS) $(LIBS)  -o $@

printFieldType.o: ../test/printFieldType.cpp
	$(CXX) $(CXXFLAGS) -c ../test/printFieldType.cpp -o $@

../test/printFieldType: printFieldType.o $(OBJS) swt/lib/libswt.a 
	$(CXX) $(CXXFLAGS) printFieldType.o $(OBJS) $(LIBS) -o $@

printNodeType.o: ../test/printNodeType.cpp
	$(CXX) $(CXXFLAGS) -c ../test/printNodeType.cpp -o $@

../test/printNodeType: printNodeType.o $(OBJS) swt/lib/libswt.a 
	$(CXX) $(CXXFLAGS) printNodeType.o $(OBJS) $(LIBS) -o $@

printFieldNumber.o: ../test/printFieldNumber.cpp
	$(CXX) $(CXXFLAGS) -c ../test/printFieldNumber.cpp -o $@

../test/printFieldNumber: printFieldNumber.o $(OBJS) swt/lib/libswt.a 
	$(CXX) $(CXXFLAGS) printFieldNumber.o $(OBJS) $(LIBS) -o $@

testzeroclass.o: ../test/testzeroclass.cpp
	$(CXX) $(CXXFLAGS) -c ../test/testzeroclass.cpp -o $@

testzeroclass: mysnprintf.o
	if test "X_" = "X_SDLjoystick/libSDLjoystick.a" ;then cd SDLjoystick;$(MAKE); fi
	(cd swt ; $(MAKE))
	$(MAKE) ../test/testzeroclass

../test/testzeroclass: testzeroclass.o  $(OBJS) swt/lib/libswt.a 
	$(CXX) $(CXXFLAGS) testzeroclass.o $(OBJS) $(LIBS)  -o $@

testprotos: mysnprintf.o
	if test "X_" = "X_SDLjoystick/libSDLjoystick.a" ;then cd SDLjoystick;$(MAKE); fi
	(cd swt ; $(MAKE))
	$(MAKE) ../test/dune_testprotos

testprotos.o: ../test/testprotos.cpp
	$(CXX) $(CXXFLAGS) -c ../test/testprotos.cpp -o $@

../test/dune_testprotos: testprotos.o  $(OBJS) swt/lib/libswt.a 
	$(CXX) testprotos.o $(OBJS) $(LIBS)  -o $@

SFMFTypes.h: ../batch/mkSFMFTypes.sh
	sh ../batch/mkSFMFTypes.sh
 
#SFMFTypes.h SFMFTypesItem.h Types.h Types.cpp: ../batch/mkSFMFTypes.sh
#	sh ../batch/mkSFMFTypes.sh


lexer.o: lexer.cpp
	$(CXX) $(CXXFLAGS) $(LFLAGS) -c lexer.cpp

lexer.cpp: lexer.l
	echo '#ifndef WIN32' > lexer.cpp
	echo '#include "unistd.h"' >> lexer.cpp
	echo '#endif' >> lexer.cpp
	if test "X_$(EGREP_PROBLEMS)" = "X_no"; then $(LEX) -t lexer.l | egrep -v '#include *.unistd\.h.' >> lexer.cpp ; else echo // remove include of unistd.h manually when compiling on Windows >> lexer.cpp ; $(LEX) -t lexer.l >> lexer.cpp ; fi

parser.o: parser.cpp
	$(CXX) $(CXXFLAGS) $(PARSER_CPP_CFLAG) -c parser.cpp

parser.cpp: parser.y
	$(YACC) $(YFLAGS) parser.y
	cp y.tab.c parser.cpp

ICONS= res/standard_toolbar.bmp res/node_icons.bmp res/vcr.bmp \
       res/emptySocketSquare.bmp res/halftone.bmp res/fullSocketSquare.bmp \
       res/recommendedSocketSquare.bmp \
       res/recommendedWonderlandSocketSquare.bmp \
       res/wonderlandSocketSquare.bmp

resource.h: resource.h.m4 swt/include/languages.m4 swt/include/resource.m4 
	m4 swt/include/languages.m4 resource.h.m4 > $@

CommandlineLanguages.h: CommandlineLanguages.h.m4 \
                        swt/include/languages.m4 swt/include/resource.m4 
	m4 swt/include/languages.m4 CommandlineLanguages.h.m4 > $@

SceneViewDebugOnUpdate.cpp: SceneViewDebugOnUpdate.cpp.m4
	m4 SceneViewDebugOnUpdate.cpp.m4 > $@

dune.rc: $(RESOURCE_FILES)
	cat singleResources.rc $(RESOURCE_FILES) > $@

resource.c: dune.rc swt/rc/rc $(ICONS) resource.h
	LANG=C sed 's.\\\\./.' < dune.rc > dune1.rc.c
	$(CXXCPP) dune1.rc.c > dune2.rc.c 
	grep $(GREP_FLAGS) -v '//' dune2.rc.c > dune3.rc.c
	if swt/rc/rc < dune3.rc.c > resource.c ; then echo > /dev/null; else rm  resource.c ; exit 1 ; fi
	rm dune1.rc.c dune2.rc.c dune3.rc.c

swt/rc/rc: swt/rc/rc.c
	(cd swt/rc && make)

configure: configure.in
	autoconf
     
clean:
	cd swt ; $(MAKE) clean
	cd SDLjoystick ; $(MAKE) clean
	rm -rf y.tab.c $(OBJS) main.o $(TESTOJBS) *.bak \
        gmon.out config.cache config.log core iifiles ii_files \
	mysnprintftest joysticktest spaceballtest aflocktest \
	Makefile.depend_* cxx_repository allNodesTest*.o 

ntclean: 
	rm -f dune.aps dune.opt dune.ncb dune.plg dune.dep \
	      dune.sln dune.suo dune.vcproj.[0-i]* dune.vcproj.[k-z]* \
              swt/win32/Release/*.obj swt/win32/Debug/*.obj \
              Release/*.obj Debug/*.obj *.obj *.dep *.pdb *.lib *.aps *.exp
	rm -rf Debug/dune.tlog x64/Debug/dune.tlog swt/win32/Debug/dune.tlog .vs

ntrealclean: ntclean
	rm -f swt/win32/Release/* swt/win32/Debug/* \
              Release/* Debug/* x64/Debug/* 

catt_test_clean:
	mv catt_test/material.geo .
	rm -f catt_test/*
	mv material.geo catt_test
	
realclean: clean ntrealclean catt_test_clean
	rm -f *.a swt/lib/*.a swt/lib/*.so* swt/rc/rc */*.obj bison.simple
	rm -rf v2x3d/* x3dv2/*
	rm -f writeWonderlandModuletest joysticktest spaceballtest aflocktest
	rm -f FilesMissingInWindowsProjects.o
	cp Makefile Makefile.bak
	awk '{ print } /^# DO NOT DELETE THIS LINE/ { print ""; exit }' < Makefile.bak > Makefile
	(cd swt ; $(MAKE) realclean)
	(cd SDLjoystick ; $(MAKE) realclean)

mysnprintftest: mysnprintf.c
	$(CC) -g -DTEST_SNPRINTF -DHAVE_SNPRINTF=1 -o $@ mysnprintf.c -lm

INPUTTEST_OBJS=EulerAngles.o Quaternion.o Aflock.o

joysticktest: InputDevice.cpp mysnprintf.o $(INPUTTEST_OBJS)
	$(CXX) -DDEBUG=1 $(CXXFLAGS) -g -DTEST_JOYSTICK -DLINUXJOYSTICK=1 \
             -o $@ InputDevice.cpp mysnprintf.o $(INPUTTEST_OBJS) $(LIBS)

spaceballtest: InputDevice.cpp mysnprintf.o $(INPUTTEST_OBJS)
	$(CXX) $(CXXFLAGS) -g -DTEST_SPACEBALL -DHAVE_LIBSBALL=1 \
             -o $@ InputDevice.cpp mysnprintf.o $(INPUTTEST_OBJS) $(LIBS) -lsball

aflocktest: InputDevice.cpp $(INPUTTEST_OBJS) mysnprintf.o
	$(CXX) $(CXXFLAGS) -g -DTEST_AFLOCK -DHAVE_AFLOCK=1 \
             -o $@ InputDevice.cpp $(INPUTTEST_OBJS) mysnprintf.o $(LIBS)

writeWonderlandModuletest: writeWonderlandModule.cpp MyString.o mysnprintf.o swt/lib/libswt.a
	$(CXX) $(CXXFLAGS) -o $@ -g -DTEST=1  writeWonderlandModule.cpp \
             MyString.o mysnprintf.o swt/lib/libswt.a -lGL -lXm -lXi -lXt -lX11 -lXmu

# consistency test
FilesMissingInWindowsProjects.o: FilesMissingInWindowsProjects.cpp
	$(CXX) $(CXXFLAGS) -c FilesMissingInWindowsProjects.cpp
	rm -f FilesMissingInWindowsProjects.o

clobber:
	cd swt ; $(MAKE) clobber
	rm -f dune

depend:	Makefile.depend_dune Makefile.depend_bigdune Makefile.depend_type \
        Makefile.depend_extra Makefile.depend_c Makefile.depend_lexyacc \
        Makefile.depend_main Makefile.depend_nodes
	cp Makefile Makefile.bak
	$(AWK) '{ print } /^# DO NOT DELETE THIS LINE/ { print ""; exit }' < Makefile.bak > Makefile
	cat Makefile.depend_dune Makefile.depend_bigdune Makefile.depend_type \
            Makefile.depend_extra Makefile.depend_c Makefile.depend_lexyacc \
            Makefile.depend_main Makefile.depend_nodes >> Makefile
	rm -f Makefile.depend_dune Makefile.depend_bigdune Makefile.depend_type \
              Makefile.depend_extra Makefile.depend_c Makefile.depend_lexyacc \
              Makefile.depend_main Makefile.depend_nodes

Makefile.depend_dune: SceneViewDebugOnUpdate.cpp
	sh ../batch/mkdep.sh "$(CXX)" $(CPPFLAGS) $(CXXFLAGS) $(SRCSDUNECLASSES) \
           $(SRCSDIALOGCLASSES) $(SRCSAPPCLASSES) > $@

Makefile.depend_bigdune:  
	sh ../batch/mkdep.sh "$(CXX)" $(CPPFLAGS) $(CXXFLAGS) $(SRCSBIGDUNECLASSES) > $@

Makefile.depend_type:  
	sh ../batch/mkdep.sh "$(CXX)" $(CPPFLAGS) $(CXXFLAGS) $(SRCSTYPECLASSES) > $@

Makefile.depend_nodes:  
	sh ../batch/mkdep.sh "$(CXX)" $(CPPFLAGS) $(CXXFLAGS) $(SRCSNODECLASSES) \
           $(SRCSNODEPARENTCLASSES) > $@

Makefile.depend_extra:
	sh ../batch/mkdep.sh "$(CXX)" $(CPPFLAGS) $(CXXFLAGS) $(SRCSEXTRACLASSES) > $@

Makefile.depend_c: resource.c
	sh ../batch/mkdep.sh "$(CXX)" $(CPPFLAGS) $(CXXFLAGS) $(SRCSC) resource.c > $@

Makefile.depend_lexyacc: parser.cpp lexer.cpp 
	sh ../batch/mkdep.sh "$(CXX)" $(CPPFLAGS) $(CXXFLAGS) parser.cpp lexer.cpp >> $@

Makefile.depend_main:
	sh ../batch/mkdep.sh "$(CXX)" $(CPPFLAGS) $(CXXFLAGS) main.cpp > $@

debug: ../bin/dune
	$(CXX) $(CXXFLAGS) -DHAVE_WANT_CORE=1 -c xerrorhandler.cpp
	$(CC) $(CFLAGS) -DDEBUG=1 -std=c99 -c -o mysnprintf.o mysnprintf.c
#	$(CXX) $(CXXFLAGS) -DDEBUG=1 -c Path.cpp
#	$(CXX) $(CXXFLAGS) -DDEBUG=1 -c SceneTreeView.cpp
	make ../bin/dune

../bin/png2PixelTexture: png2PixelTexture.cpp pngLoad.c
	$(CXX) $(CXXFLAGS) -g -o $@ png2PixelTexture.cpp pngLoad.c $(LDFLAGS) -L/usr/local/lib -lpng

tar.bz2:
	sh ../batch/mktarbz2.sh


# DO NOT DELETE THIS LINE -- make depend depends on it.

