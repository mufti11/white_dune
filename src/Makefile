# This Makefile is free software; the Free Software Foundation
# gives unlimited permission to copy, distribute and modify it.

CC = gcc
CXX = g++
RANLIB = /usr/bin/ranlib 
CXXCPP = g++ -E
YACC = bison -y
LEX = flex --nounistd
AWK = gawk
PREFIX = /usr/local
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

CPPFLAGS= -I. -Iswt/include  -I/usr/local/include -I/usr/local/include/eigenlib -I/usr/include/vcglib -I.//src -I.//vcglib -I.//vcglib/eigenlib -I/home/mufti/vcglib -I.//../vcglib -I/home/mufti/vcglib/eigenlib -I.//../vcglib/eigenlib -I/usr/include/freetype2 -I/usr/include/libpng16 -I/usr/include/ffmpeg/   -I/usr/X11R6/include/ -I/usr/X11R6/LessTif/Motif1.2/include -I/usr/local/include -I/usr/local/include
COMMON_FLAGS = $(WARNINGS) -I. -Iswt/include  -I/usr/local/include -I/usr/local/include/eigenlib -I/usr/include/vcglib -I.//src -I.//vcglib -I.//vcglib/eigenlib -I/home/mufti/vcglib -I.//../vcglib -I/home/mufti/vcglib/eigenlib -I.//../vcglib/eigenlib -I/usr/include/freetype2 -I/usr/include/libpng16 -I/usr/include/ffmpeg/   -I/usr/X11R6/include/ -I/usr/X11R6/LessTif/Motif1.2/include -I/usr/local/include -I/usr/local/include 
CFLAGS_BIG = -g $(COMMON_FLAGS)  -fopenmp  -fopenmp
CFLAGS = $(COMMON_FLAGS)  -fopenmp
CXXFLAGS = -g $(COMMON_FLAGS)  -fopenmp
LIBS =   -L/usr/local/lib -L/usr/local/lib -g -Lswt/lib -lswt -LSDLjoystick  -lusb -lswresample -lswscale -lavformat -lavutil -lavcodec -ljpeg -lpng -lz -lexpat -lGLU -lGL -lcurl -lXi -lmpfr -lgmp -lfreetype -lXm -lXext -lXt -lXmu -lX11 -lm -ldl -lgomp   -L/usr/local/lib  -L/usr/X11R6/LessTif/Motif1.2/lib -lCGAL -lboost_thread -lboost_system -losdCPU 
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
	NodeVrmlCut.o \
        NodeGeneratedShadowMap.o \
        NodeMatrixTransform.o \
        NodeMultiGeneratedTextureCoordinate.o \
        NodeRenderedTexture.o \
        NodeSuperShape.o \
	NodeVrmlScene.o \
	NodeKambiAppearance.o \
	NodeKambiHeadLight.o \
        NodeKambiInline.o \
        NodeKambiNavigationInfo.o \
        NodeKambiOctreeProperties.o \
	NodeKambiTriangulation.o \
	NodeProjectedTextureCoordinate.o \
        NodeSuperRevolver.o \
        NodeScreenEffect.o \
        NodeTeapot.o \
	NodeText3D.o \
        NodeShaderTexture.o \
        NodeVectorInterpolator.o \
        NodeCurveAnimation.o \
        NodeSuperExtrusion.o \
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
        SliderFloatDialog.o \
        SceneDialog.o

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
        NurbsCurve.o \
        Vec3x.o \
        VertexModifier.o \
        illegal2vrml.o $(NEBULA)

BIGDUNECLASSES = \
	Util.o \
        quadric_simp.o \
	NodeMovieTexture.o \
        MeshSubdivide.o \
        Scene.o \
        SceneProtoMap.o \
	Node.o \
        MainWindow.o \
	NodeIndexedFaceSet.o

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
        resource.o \
        mysnprintf.o \
        buf.o \
        mathx.o \
        subd_mesh.o \
        subd.o \
        openglutfont.o \
	jpegLoad.o \
	pngLoad.o gif.o

OBJS = $(BIGDUNECLASSES) $(CLASSES) $(OBJSC) 

OBJS_LOW_MEM = $(CLASSES) $(OBJSC)

ARCHIVES = MainWindow.o SceneProtoMap.o Scene.o parser.o lexer.o \
           libdune_extra.a libdune_nodes.a libdune_nodeparents.a \
           libdune_dialogs.a libdune_app.a libdune_dune.a \
           Node.o libdune_type.a libdune_c.a resource.o mysnprintf.o

SRCSC = $(OBJSC:.o=.c) resource.c mysnprintf.c
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
	install -m 755 -p ../tools/run_dune_and_aqsis.sh /usr/bin
	install -m 644 -p ../include/white_dune/libC++RWD.h /usr/include/white_dune/libC++RWD.h
	install -m 644 -p ../include/white_dune/libC++RWD_namespace.h /usr/include/white_dune/libC++RWD_namespace.h
	install -m 644 -p ../include/white_dune/libCRWD.h /usr/include/white_dune/libCRWD.h
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

objc: $(SRCSC)
	$(CC) $(CFLAGS) -c $(SRCSC)

# special target to compile in low memory/no swap situtations 
duneLowMem: main.o objc $(CLASSES) 
	if test "X_" = "X_SDLjoystick/libSDLjoystick.a" ;then cd SDLjoystick;$(MAKE); fi
	$(MAKE) SFMFTypes.h
	(cd swt ; $(MAKE))
	make -j2 $(BIGDUNECLASSES)
	$(CXX) main.o $(OBJS) $(LIBS) -o ../bin/dune
	sh -c "if test `uname` = IRIX64 ; then tag 0x00ff1a00 ../bin/dune ; fi"
	sh -c "if test `uname` = IRIX   ; then tag 0x00ff1a00 ../bin/dune ; fi"
	sh -c "if test `uname` = Darwin ; then (if test -d ./desktop/macosx; then cd ../desktop/macosx/ && tar -xvf white_dune_tiger.app.tar && cd white_dune.app/Contents/MacOS && ln -sf ../../../../../bin/dune . ; fi) ; fi"

../bin/dune: main.o $(OBJS)  swt/lib/libswt.a
	(cd swt ; $(MAKE))
	$(CXX) main.o $(OBJS) $(LIBS) -o $@
	sh -c "if test `uname` = IRIX64 ; then tag 0x00ff1a00 $@ ; fi"
	sh -c "if test `uname` = IRIX   ; then tag 0x00ff1a00 $@ ; fi"
	sh -c "if test `uname` = Darwin ; then (if test -d ./desktop/macosx; then cd ../desktop/macosx/ && tar -xvf white_dune_tiger.app.tar && cd white_dune.app/Contents/MacOS && ln -sf ../../../../../bin/dune . ; fi) ; fi"

../bin/efencedune:  main.o $(OBJS) 
	(cd swt ; $(MAKE))
	(cd swt ; $(MAKE) efence)
	rm -f /tmp/efencedune.o
	echo no way to extract strdup efence may fail in strdup
	ld $(LD_OPTIONS) -r -o /tmp/efencedune.o  main.o $(OBJS)  $(LIBEFENCE_A)
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
	rm -rf .git y.tab.c $(OBJS) main.o $(TESTOJBS) *.bak \
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

AboutDialog.o: AboutDialog.cpp stdafx.h config.h mysnprintf.h \
 AboutDialog.h swt/include/swttypedef.h Dialog.h resource.h \
 swt/include/swt.h swt/include/keycodes.h swt/include/swlanguages.h \
 swt/include/swDebugf.h swt/include/swttypedef.h MyString.h StringArray.h \
 Array.h
CommandList.o: CommandList.cpp stdafx.h config.h mysnprintf.h \
 CommandList.h Command.h List.h
Element.o: Element.cpp stdafx.h config.h mysnprintf.h Element.h \
 MyString.h StringArray.h Array.h x3dFlags.h swt/include/WriteFlags.h \
 Types.h DuneApp.h List.h Node.h Vec3f.h Vec3x.h FieldIndex.h NodeFlags.h \
 MeshFlags.h ac3dMaterialCallback.h NodeArray.h Stack.h Matrix.h Vec3d.h \
 Quaternion.h URL.h PreferencesApp.h swt/include/swttypedef.h \
 EcmaScriptApp.h StereoViewApp.h swt/include/stereoview.h \
 InputDeviceApp.h TransformMode.h OutputApp.h RouteViewApp.h \
 StartWithApp.h ExportNamesApp.h ModellingApp.h RenderApp.h Proto.h Map.h \
 ProtoMacros.h NodeList.h InterfaceArray.h DynamicFieldsNode.h \
 SFMFTypes.h MFBool.h FieldValue.h Arrays.h MySwap.h MFColor.h MFFloat.h \
 SFFloat.h MFColorRGBA.h MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h \
 MFMatrix4f.h MFNode.h MFRotation.h MFString.h MFTime.h SFTime.h \
 MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h \
 SFBool.h SFColor.h SFColorRGBA.h SFImage.h SFInt32.h SFMatrix3f.h \
 SFMatrix4f.h SFNode.h SFRotation.h SFString.h SFVec2d.h SFVec2f.h \
 Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h CommandList.h Command.h
EventIn.o: EventIn.cpp stdafx.h config.h mysnprintf.h EventIn.h Element.h \
 MyString.h StringArray.h Array.h x3dFlags.h swt/include/WriteFlags.h \
 Node.h List.h Vec3f.h Vec3x.h FieldIndex.h NodeFlags.h MeshFlags.h \
 ac3dMaterialCallback.h NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h \
 Scene.h Map.h CommandList.h Command.h NodeList.h URL.h Proto.h \
 ProtoMacros.h InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h MFBool.h \
 FieldValue.h Arrays.h MySwap.h Types.h MFColor.h MFFloat.h SFFloat.h \
 MFColorRGBA.h MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h \
 MFNode.h MFRotation.h MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h \
 MFVec2f.h MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h \
 SFColorRGBA.h SFImage.h SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h \
 SFRotation.h SFString.h SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h \
 SFVec3f.h SFVec4f.h DuneApp.h PreferencesApp.h swt/include/swttypedef.h \
 EcmaScriptApp.h StereoViewApp.h swt/include/stereoview.h \
 InputDeviceApp.h TransformMode.h OutputApp.h RouteViewApp.h \
 StartWithApp.h ExportNamesApp.h ModellingApp.h RenderApp.h RenderState.h \
 ProtoMap.h
EventOut.o: EventOut.cpp stdafx.h config.h mysnprintf.h EventOut.h \
 Element.h MyString.h StringArray.h Array.h x3dFlags.h \
 swt/include/WriteFlags.h Node.h List.h Vec3f.h Vec3x.h FieldIndex.h \
 NodeFlags.h MeshFlags.h ac3dMaterialCallback.h NodeArray.h Stack.h \
 Matrix.h Vec3d.h Quaternion.h Scene.h Map.h CommandList.h Command.h \
 NodeList.h URL.h Proto.h ProtoMacros.h InterfaceArray.h \
 DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h Arrays.h MySwap.h \
 Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h MFDouble.h \
 SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h MFRotation.h \
 MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h \
 MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h SFImage.h \
 SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h SFString.h \
 SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h \
 DuneApp.h PreferencesApp.h swt/include/swttypedef.h EcmaScriptApp.h \
 StereoViewApp.h swt/include/stereoview.h InputDeviceApp.h \
 TransformMode.h OutputApp.h RouteViewApp.h StartWithApp.h \
 ExportNamesApp.h ModellingApp.h RenderApp.h RenderState.h ProtoMap.h
ExposedField.o: ExposedField.cpp stdafx.h config.h mysnprintf.h \
 ExposedField.h Element.h MyString.h StringArray.h Array.h x3dFlags.h \
 swt/include/WriteFlags.h Node.h List.h Vec3f.h Vec3x.h FieldIndex.h \
 NodeFlags.h MeshFlags.h ac3dMaterialCallback.h NodeArray.h Stack.h \
 Matrix.h Vec3d.h Quaternion.h Scene.h Map.h CommandList.h Command.h \
 NodeList.h URL.h Proto.h ProtoMacros.h InterfaceArray.h \
 DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h Arrays.h MySwap.h \
 Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h MFDouble.h \
 SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h MFRotation.h \
 MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h \
 MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h SFImage.h \
 SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h SFString.h \
 SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h \
 DuneApp.h PreferencesApp.h swt/include/swttypedef.h EcmaScriptApp.h \
 StereoViewApp.h swt/include/stereoview.h InputDeviceApp.h \
 TransformMode.h OutputApp.h RouteViewApp.h StartWithApp.h \
 ExportNamesApp.h ModellingApp.h RenderApp.h RenderState.h ProtoMap.h \
 Field.h
Field.o: Field.cpp stdafx.h config.h mysnprintf.h Field.h Element.h \
 MyString.h StringArray.h Array.h x3dFlags.h swt/include/WriteFlags.h \
 Node.h List.h Vec3f.h Vec3x.h FieldIndex.h NodeFlags.h MeshFlags.h \
 ac3dMaterialCallback.h NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h \
 ExposedField.h Scene.h Map.h CommandList.h Command.h NodeList.h URL.h \
 Proto.h ProtoMacros.h InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h \
 MFBool.h FieldValue.h Arrays.h MySwap.h Types.h MFColor.h MFFloat.h \
 SFFloat.h MFColorRGBA.h MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h \
 MFMatrix4f.h MFNode.h MFRotation.h MFString.h MFTime.h SFTime.h \
 MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h \
 SFBool.h SFColor.h SFColorRGBA.h SFImage.h SFInt32.h SFMatrix3f.h \
 SFMatrix4f.h SFNode.h SFRotation.h SFString.h SFVec2d.h SFVec2f.h \
 Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h DuneApp.h \
 PreferencesApp.h swt/include/swttypedef.h EcmaScriptApp.h \
 StereoViewApp.h swt/include/stereoview.h InputDeviceApp.h \
 TransformMode.h OutputApp.h RouteViewApp.h StartWithApp.h \
 ExportNamesApp.h ModellingApp.h RenderApp.h RenderState.h ProtoMap.h
FieldCommand.o: FieldCommand.cpp stdafx.h config.h mysnprintf.h \
 FieldCommand.h Command.h Scene.h Array.h Map.h List.h Stack.h \
 CommandList.h NodeList.h NodeArray.h Node.h Vec3f.h Vec3x.h MyString.h \
 StringArray.h FieldIndex.h Element.h x3dFlags.h swt/include/WriteFlags.h \
 NodeFlags.h MeshFlags.h ac3dMaterialCallback.h Matrix.h Vec3d.h \
 Quaternion.h URL.h Proto.h ProtoMacros.h InterfaceArray.h \
 DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h Arrays.h MySwap.h \
 Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h MFDouble.h \
 SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h MFRotation.h \
 MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h \
 MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h SFImage.h \
 SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h SFString.h \
 SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h \
 DuneApp.h PreferencesApp.h swt/include/swttypedef.h EcmaScriptApp.h \
 StereoViewApp.h swt/include/stereoview.h InputDeviceApp.h \
 TransformMode.h OutputApp.h RouteViewApp.h StartWithApp.h \
 ExportNamesApp.h ModellingApp.h RenderApp.h RenderState.h ProtoMap.h \
 NodeNurbsSurface.h MeshMorphingNode.h Interpolator.h MeshBasedNode.h \
 GeometryNode.h NodeAppearance.h NodeMaterial.h swt/include/swt.h \
 swt/include/keycodes.h swt/include/swlanguages.h swt/include/swDebugf.h \
 swt/include/swttypedef.h NodeImageTexture.h KambiTextureCommonFields.h \
 NodeTextureTransform.h GeometryNodeMacros.h
MFieldCommand.o: MFieldCommand.cpp stdafx.h config.h mysnprintf.h \
 MFieldCommand.h Command.h Scene.h Array.h Map.h List.h Stack.h \
 CommandList.h NodeList.h NodeArray.h Node.h Vec3f.h Vec3x.h MyString.h \
 StringArray.h FieldIndex.h Element.h x3dFlags.h swt/include/WriteFlags.h \
 NodeFlags.h MeshFlags.h ac3dMaterialCallback.h Matrix.h Vec3d.h \
 Quaternion.h URL.h Proto.h ProtoMacros.h InterfaceArray.h \
 DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h Arrays.h MySwap.h \
 Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h MFDouble.h \
 SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h MFRotation.h \
 MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h \
 MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h SFImage.h \
 SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h SFString.h \
 SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h \
 DuneApp.h PreferencesApp.h swt/include/swttypedef.h EcmaScriptApp.h \
 StereoViewApp.h swt/include/stereoview.h InputDeviceApp.h \
 TransformMode.h OutputApp.h RouteViewApp.h StartWithApp.h \
 ExportNamesApp.h ModellingApp.h RenderApp.h RenderState.h ProtoMap.h
TUIElement.o: TUIElement.cpp stdafx.h config.h mysnprintf.h TUIElement.h \
 CoverNode.h Node.h Array.h List.h Vec3f.h Vec3x.h MyString.h \
 StringArray.h FieldIndex.h Element.h x3dFlags.h swt/include/WriteFlags.h \
 NodeFlags.h MeshFlags.h ac3dMaterialCallback.h NodeArray.h Stack.h \
 Matrix.h Vec3d.h Quaternion.h ProtoMacros.h Proto.h Map.h NodeList.h \
 InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h \
 Arrays.h MySwap.h Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h \
 MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h \
 MFRotation.h MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h \
 MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h \
 SFImage.h SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h \
 SFString.h SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h \
 SFVec4f.h CommandList.h Command.h DuneApp.h URL.h PreferencesApp.h \
 swt/include/swttypedef.h EcmaScriptApp.h StereoViewApp.h \
 swt/include/stereoview.h InputDeviceApp.h TransformMode.h OutputApp.h \
 RouteViewApp.h StartWithApp.h ExportNamesApp.h ModellingApp.h \
 RenderApp.h Field.h
FaceData.o: FaceData.cpp stdafx.h config.h mysnprintf.h FaceData.h \
 Vec3f.h Vec3x.h
FieldValue.o: FieldValue.cpp stdafx.h config.h mysnprintf.h FieldValue.h \
 Array.h MyString.h StringArray.h Element.h x3dFlags.h \
 swt/include/WriteFlags.h Arrays.h MySwap.h Types.h SFMFTypes.h MFBool.h \
 MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h MFDouble.h SFDouble.h \
 MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h Node.h List.h Vec3f.h \
 Vec3x.h FieldIndex.h NodeFlags.h MeshFlags.h ac3dMaterialCallback.h \
 NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h NodeList.h \
 MFRotation.h MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h \
 MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h \
 SFImage.h SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h \
 SFString.h SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h \
 SFVec4f.h Scene.h Map.h CommandList.h Command.h URL.h Proto.h \
 ProtoMacros.h InterfaceArray.h DynamicFieldsNode.h DuneApp.h \
 PreferencesApp.h swt/include/swttypedef.h EcmaScriptApp.h \
 StereoViewApp.h swt/include/stereoview.h InputDeviceApp.h \
 TransformMode.h OutputApp.h RouteViewApp.h StartWithApp.h \
 ExportNamesApp.h ModellingApp.h RenderApp.h RenderState.h ProtoMap.h
Image.o: Image.cpp stdafx.h config.h mysnprintf.h Image.h gif.h \
 jpegLoad.h pngLoad.h
Interpolator.o: Interpolator.cpp stdafx.h config.h mysnprintf.h \
 Interpolator.h Node.h Array.h List.h Vec3f.h Vec3x.h MyString.h \
 StringArray.h FieldIndex.h Element.h x3dFlags.h swt/include/WriteFlags.h \
 NodeFlags.h MeshFlags.h ac3dMaterialCallback.h NodeArray.h Stack.h \
 Matrix.h Vec3d.h Quaternion.h ProtoMacros.h Proto.h Map.h NodeList.h \
 InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h \
 Arrays.h MySwap.h Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h \
 MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h \
 MFRotation.h MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h \
 MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h \
 SFImage.h SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h \
 SFString.h SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h \
 SFVec4f.h CommandList.h Command.h Scene.h URL.h DuneApp.h \
 PreferencesApp.h swt/include/swttypedef.h EcmaScriptApp.h \
 StereoViewApp.h swt/include/stereoview.h InputDeviceApp.h \
 TransformMode.h OutputApp.h RouteViewApp.h StartWithApp.h \
 ExportNamesApp.h ModellingApp.h RenderApp.h RenderState.h ProtoMap.h \
 MFieldCommand.h FieldCommand.h InputDeviceTime.h Field.h \
 swt/include/swt.h swt/include/keycodes.h swt/include/swlanguages.h \
 swt/include/swDebugf.h swt/include/swttypedef.h NodeCurveAnimation.h \
 NodeNurbsCurve.h NurbsCurve.h GeometryNode.h NodeAppearance.h \
 NodeMaterial.h NodeImageTexture.h KambiTextureCommonFields.h \
 NodeTextureTransform.h GeometryNodeMacros.h ChainBasedNode.h \
 NodePositionInterpolator.h NodeOrientationInterpolator.h \
 NodeTimeSensor.h
Matrix.o: Matrix.cpp stdafx.h config.h mysnprintf.h swt/include/swt.h \
 swt/include/keycodes.h swt/include/swlanguages.h swt/include/swDebugf.h \
 swt/include/swttypedef.h Matrix.h Vec3f.h Vec3x.h Vec3d.h EulerAngles.h \
 QuatTypes.h Quaternion.h
Metadata.o: Metadata.cpp stdafx.h config.h mysnprintf.h Metadata.h Node.h \
 Array.h List.h Vec3f.h Vec3x.h MyString.h StringArray.h FieldIndex.h \
 Element.h x3dFlags.h swt/include/WriteFlags.h NodeFlags.h MeshFlags.h \
 ac3dMaterialCallback.h NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h \
 Proto.h Map.h ProtoMacros.h NodeList.h InterfaceArray.h \
 DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h Arrays.h MySwap.h \
 Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h MFDouble.h \
 SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h MFRotation.h \
 MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h \
 MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h SFImage.h \
 SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h SFString.h \
 SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h \
 CommandList.h Command.h
MoveCommand.o: MoveCommand.cpp stdafx.h config.h mysnprintf.h \
 MoveCommand.h Command.h Scene.h Array.h Map.h List.h Stack.h \
 CommandList.h NodeList.h NodeArray.h Node.h Vec3f.h Vec3x.h MyString.h \
 StringArray.h FieldIndex.h Element.h x3dFlags.h swt/include/WriteFlags.h \
 NodeFlags.h MeshFlags.h ac3dMaterialCallback.h Matrix.h Vec3d.h \
 Quaternion.h URL.h Proto.h ProtoMacros.h InterfaceArray.h \
 DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h Arrays.h MySwap.h \
 Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h MFDouble.h \
 SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h MFRotation.h \
 MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h \
 MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h SFImage.h \
 SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h SFString.h \
 SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h \
 DuneApp.h PreferencesApp.h swt/include/swttypedef.h EcmaScriptApp.h \
 StereoViewApp.h swt/include/stereoview.h InputDeviceApp.h \
 TransformMode.h OutputApp.h RouteViewApp.h StartWithApp.h \
 ExportNamesApp.h ModellingApp.h RenderApp.h RenderState.h ProtoMap.h \
 Path.h
NodeList.o: NodeList.cpp stdafx.h config.h mysnprintf.h swt/include/swt.h \
 swt/include/keycodes.h swt/include/swlanguages.h swt/include/swDebugf.h \
 swt/include/swttypedef.h NodeList.h Array.h NodeArray.h Node.h List.h \
 Vec3f.h Vec3x.h MyString.h StringArray.h FieldIndex.h Element.h \
 x3dFlags.h swt/include/WriteFlags.h NodeFlags.h MeshFlags.h \
 ac3dMaterialCallback.h Stack.h Matrix.h Vec3d.h Quaternion.h Scene.h \
 Map.h CommandList.h Command.h URL.h Proto.h ProtoMacros.h \
 InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h \
 Arrays.h MySwap.h Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h \
 MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h \
 MFRotation.h MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h \
 MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h \
 SFImage.h SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h \
 SFString.h SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h \
 SFVec4f.h DuneApp.h PreferencesApp.h swt/include/swttypedef.h \
 EcmaScriptApp.h StereoViewApp.h swt/include/stereoview.h \
 InputDeviceApp.h TransformMode.h OutputApp.h RouteViewApp.h \
 StartWithApp.h ExportNamesApp.h ModellingApp.h RenderApp.h RenderState.h \
 ProtoMap.h
Path.o: Path.cpp stdafx.h config.h mysnprintf.h Path.h FieldValue.h \
 Array.h MyString.h StringArray.h Element.h x3dFlags.h \
 swt/include/WriteFlags.h Arrays.h MySwap.h Types.h NodeList.h \
 NodeArray.h Node.h List.h Vec3f.h Vec3x.h FieldIndex.h NodeFlags.h \
 MeshFlags.h ac3dMaterialCallback.h Stack.h Matrix.h Vec3d.h Quaternion.h \
 Scene.h Map.h CommandList.h Command.h URL.h Proto.h ProtoMacros.h \
 InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h MFBool.h MFColor.h \
 MFFloat.h SFFloat.h MFColorRGBA.h MFDouble.h SFDouble.h MFInt32.h \
 MFMatrix3f.h MFMatrix4f.h MFNode.h MFRotation.h MFString.h MFTime.h \
 SFTime.h MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h MFVec3f.h MFVec4f.h \
 Vec4f.h SFBool.h SFColor.h SFColorRGBA.h SFImage.h SFInt32.h \
 SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h SFString.h SFVec2d.h \
 SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h DuneApp.h \
 PreferencesApp.h swt/include/swttypedef.h EcmaScriptApp.h \
 StereoViewApp.h swt/include/stereoview.h InputDeviceApp.h \
 TransformMode.h OutputApp.h RouteViewApp.h StartWithApp.h \
 ExportNamesApp.h ModellingApp.h RenderApp.h RenderState.h ProtoMap.h \
 Field.h swt/include/swDebugf.h swt/include/swt.h swt/include/keycodes.h \
 swt/include/swlanguages.h swt/include/swDebugf.h \
 swt/include/swttypedef.h
Proto.o: Proto.cpp stdafx.h config.h mysnprintf.h resource.h Proto.h \
 Array.h Map.h List.h MyString.h StringArray.h Node.h Vec3f.h Vec3x.h \
 FieldIndex.h Element.h x3dFlags.h swt/include/WriteFlags.h NodeFlags.h \
 MeshFlags.h ac3dMaterialCallback.h NodeArray.h Stack.h Matrix.h Vec3d.h \
 Quaternion.h ProtoMacros.h NodeList.h InterfaceArray.h \
 DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h Arrays.h MySwap.h \
 Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h MFDouble.h \
 SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h MFRotation.h \
 MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h \
 MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h SFImage.h \
 SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h SFString.h \
 SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h \
 CommandList.h Command.h Scene.h URL.h DuneApp.h PreferencesApp.h \
 swt/include/swttypedef.h EcmaScriptApp.h StereoViewApp.h \
 swt/include/stereoview.h InputDeviceApp.h TransformMode.h OutputApp.h \
 RouteViewApp.h StartWithApp.h ExportNamesApp.h ModellingApp.h \
 RenderApp.h RenderState.h ProtoMap.h EventIn.h EventOut.h Field.h \
 ExposedField.h NodeTransform.h swt/include/swt.h swt/include/keycodes.h \
 swt/include/swlanguages.h swt/include/swDebugf.h \
 swt/include/swttypedef.h TransformNode.h BasicTransformNode.h
RouteCommand.o: RouteCommand.cpp stdafx.h config.h mysnprintf.h \
 RouteCommand.h Command.h Node.h Array.h List.h Vec3f.h Vec3x.h \
 MyString.h StringArray.h FieldIndex.h Element.h x3dFlags.h \
 swt/include/WriteFlags.h NodeFlags.h MeshFlags.h ac3dMaterialCallback.h \
 NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h Scene.h Map.h \
 CommandList.h NodeList.h URL.h Proto.h ProtoMacros.h InterfaceArray.h \
 DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h Arrays.h MySwap.h \
 Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h MFDouble.h \
 SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h MFRotation.h \
 MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h \
 MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h SFImage.h \
 SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h SFString.h \
 SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h \
 DuneApp.h PreferencesApp.h swt/include/swttypedef.h EcmaScriptApp.h \
 StereoViewApp.h swt/include/stereoview.h InputDeviceApp.h \
 TransformMode.h OutputApp.h RouteViewApp.h StartWithApp.h \
 ExportNamesApp.h ModellingApp.h RenderApp.h RenderState.h ProtoMap.h
MyString.o: MyString.cpp stdafx.h config.h mysnprintf.h MyString.h \
 StringArray.h Array.h
UnRouteCommand.o: UnRouteCommand.cpp stdafx.h config.h mysnprintf.h \
 UnRouteCommand.h Command.h Node.h Array.h List.h Vec3f.h Vec3x.h \
 MyString.h StringArray.h FieldIndex.h Element.h x3dFlags.h \
 swt/include/WriteFlags.h NodeFlags.h MeshFlags.h ac3dMaterialCallback.h \
 NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h Scene.h Map.h \
 CommandList.h NodeList.h URL.h Proto.h ProtoMacros.h InterfaceArray.h \
 DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h Arrays.h MySwap.h \
 Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h MFDouble.h \
 SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h MFRotation.h \
 MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h \
 MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h SFImage.h \
 SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h SFString.h \
 SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h \
 DuneApp.h PreferencesApp.h swt/include/swttypedef.h EcmaScriptApp.h \
 StereoViewApp.h swt/include/stereoview.h InputDeviceApp.h \
 TransformMode.h OutputApp.h RouteViewApp.h StartWithApp.h \
 ExportNamesApp.h ModellingApp.h RenderApp.h RenderState.h ProtoMap.h
MyMesh.o: MyMesh.cpp MyMesh.h Array.h stdafx.h config.h mysnprintf.h \
 List.h Vec4f.h Vec3f.h Vec3x.h Vec2f.h Vec3d.h swt/include/swt.h \
 swt/include/keycodes.h swt/include/swlanguages.h swt/include/swDebugf.h \
 swt/include/swttypedef.h NodeIndexedFaceSet.h MeshBasedNode.h \
 GeometryNode.h Node.h MyString.h StringArray.h FieldIndex.h Element.h \
 x3dFlags.h swt/include/WriteFlags.h NodeFlags.h MeshFlags.h \
 ac3dMaterialCallback.h NodeArray.h Stack.h Matrix.h Quaternion.h \
 NodeAppearance.h ProtoMacros.h Proto.h Map.h NodeList.h InterfaceArray.h \
 DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h Arrays.h MySwap.h \
 Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h MFDouble.h \
 SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h MFRotation.h \
 MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h \
 MFVec3f.h MFVec4f.h SFBool.h SFColor.h SFColorRGBA.h SFImage.h SFInt32.h \
 SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h SFString.h SFVec2d.h \
 SFVec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h CommandList.h \
 Command.h DuneApp.h URL.h PreferencesApp.h swt/include/swttypedef.h \
 EcmaScriptApp.h StereoViewApp.h swt/include/stereoview.h \
 InputDeviceApp.h TransformMode.h OutputApp.h RouteViewApp.h \
 StartWithApp.h ExportNamesApp.h ModellingApp.h RenderApp.h \
 NodeMaterial.h NodeImageTexture.h KambiTextureCommonFields.h \
 NodeTextureTransform.h GeometryNodeMacros.h NodeCoordinate.h Colored.h \
 NodeColor.h NodeColorRGBA.h ComposedGeometryMacros.h FaceData.h \
 NodeNormal.h NodeTextureCoordinate.h NodeTextureCoordinateGenerator.h \
 NodeMultiTextureCoordinate.h Scene.h RenderState.h ProtoMap.h Util.h
SelectionCommand.o: SelectionCommand.cpp stdafx.h config.h mysnprintf.h \
 SelectionCommand.h Command.h Scene.h Array.h Map.h List.h Stack.h \
 CommandList.h NodeList.h NodeArray.h Node.h Vec3f.h Vec3x.h MyString.h \
 StringArray.h FieldIndex.h Element.h x3dFlags.h swt/include/WriteFlags.h \
 NodeFlags.h MeshFlags.h ac3dMaterialCallback.h Matrix.h Vec3d.h \
 Quaternion.h URL.h Proto.h ProtoMacros.h InterfaceArray.h \
 DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h Arrays.h MySwap.h \
 Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h MFDouble.h \
 SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h MFRotation.h \
 MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h \
 MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h SFImage.h \
 SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h SFString.h \
 SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h \
 DuneApp.h PreferencesApp.h swt/include/swttypedef.h EcmaScriptApp.h \
 StereoViewApp.h swt/include/stereoview.h InputDeviceApp.h \
 TransformMode.h OutputApp.h RouteViewApp.h StartWithApp.h \
 ExportNamesApp.h ModellingApp.h RenderApp.h RenderState.h ProtoMap.h \
 Path.h
Quaternion.o: Quaternion.cpp stdafx.h config.h mysnprintf.h Quaternion.h \
 Vec3f.h Vec3x.h Matrix.h Vec3d.h
RenderState.o: RenderState.cpp stdafx.h config.h mysnprintf.h DuneApp.h \
 List.h Array.h MyString.h StringArray.h Node.h Vec3f.h Vec3x.h \
 FieldIndex.h Element.h x3dFlags.h swt/include/WriteFlags.h NodeFlags.h \
 MeshFlags.h ac3dMaterialCallback.h NodeArray.h Stack.h Matrix.h Vec3d.h \
 Quaternion.h URL.h PreferencesApp.h swt/include/swttypedef.h \
 EcmaScriptApp.h StereoViewApp.h swt/include/stereoview.h \
 InputDeviceApp.h TransformMode.h OutputApp.h RouteViewApp.h \
 StartWithApp.h ExportNamesApp.h ModellingApp.h RenderApp.h RenderState.h \
 Scene.h Map.h CommandList.h Command.h NodeList.h Proto.h ProtoMacros.h \
 InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h \
 Arrays.h MySwap.h Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h \
 MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h \
 MFRotation.h MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h \
 MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h \
 SFImage.h SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h \
 SFString.h SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h \
 SFVec4f.h ProtoMap.h Util.h
SceneTreeView.o: SceneTreeView.cpp stdafx.h config.h mysnprintf.h \
 SceneTreeView.h SceneView.h DuneApp.h List.h Array.h MyString.h \
 StringArray.h Node.h Vec3f.h Vec3x.h FieldIndex.h Element.h x3dFlags.h \
 swt/include/WriteFlags.h NodeFlags.h MeshFlags.h ac3dMaterialCallback.h \
 NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h URL.h PreferencesApp.h \
 swt/include/swttypedef.h EcmaScriptApp.h StereoViewApp.h \
 swt/include/stereoview.h InputDeviceApp.h TransformMode.h OutputApp.h \
 RouteViewApp.h StartWithApp.h ExportNamesApp.h ModellingApp.h \
 RenderApp.h Scene.h Map.h CommandList.h Command.h NodeList.h Proto.h \
 ProtoMacros.h InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h MFBool.h \
 FieldValue.h Arrays.h MySwap.h Types.h MFColor.h MFFloat.h SFFloat.h \
 MFColorRGBA.h MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h \
 MFNode.h MFRotation.h MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h \
 MFVec2f.h MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h \
 SFColorRGBA.h SFImage.h SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h \
 SFRotation.h SFString.h SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h \
 SFVec3f.h SFVec4f.h RenderState.h ProtoMap.h IconSize.h \
 swt/include/swt.h swt/include/keycodes.h swt/include/swlanguages.h \
 swt/include/swDebugf.h swt/include/swttypedef.h Field.h Path.h \
 resource.h
SceneGraphView.o: SceneGraphView.cpp stdafx.h config.h mysnprintf.h \
 SceneGraphView.h swt/include/swt.h swt/include/keycodes.h \
 swt/include/swlanguages.h swt/include/swDebugf.h \
 swt/include/swttypedef.h SceneView.h DuneApp.h List.h Array.h MyString.h \
 StringArray.h Node.h Vec3f.h Vec3x.h FieldIndex.h Element.h x3dFlags.h \
 swt/include/WriteFlags.h NodeFlags.h MeshFlags.h ac3dMaterialCallback.h \
 NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h URL.h PreferencesApp.h \
 swt/include/swttypedef.h EcmaScriptApp.h StereoViewApp.h \
 swt/include/stereoview.h InputDeviceApp.h TransformMode.h OutputApp.h \
 RouteViewApp.h StartWithApp.h ExportNamesApp.h ModellingApp.h \
 RenderApp.h Scene.h Map.h CommandList.h Command.h NodeList.h Proto.h \
 ProtoMacros.h InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h MFBool.h \
 FieldValue.h Arrays.h MySwap.h Types.h MFColor.h MFFloat.h SFFloat.h \
 MFColorRGBA.h MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h \
 MFNode.h MFRotation.h MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h \
 MFVec2f.h MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h \
 SFColorRGBA.h SFImage.h SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h \
 SFRotation.h SFString.h SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h \
 SFVec3f.h SFVec4f.h RenderState.h ProtoMap.h Point.h Path.h Field.h \
 ExposedField.h EventIn.h EventOut.h MoveCommand.h RouteCommand.h \
 UnRouteCommand.h resource.h Util.h IconSize.h
Scene3DView.o: Scene3DView.cpp stdafx.h config.h mysnprintf.h \
 swt/include/swt.h swt/include/keycodes.h swt/include/swlanguages.h \
 swt/include/swDebugf.h swt/include/swttypedef.h Scene3DView.h Vec3f.h \
 Vec3x.h Quaternion.h Matrix.h Vec3d.h SceneView.h DuneApp.h List.h \
 Array.h MyString.h StringArray.h Node.h FieldIndex.h Element.h \
 x3dFlags.h swt/include/WriteFlags.h NodeFlags.h MeshFlags.h \
 ac3dMaterialCallback.h NodeArray.h Stack.h URL.h PreferencesApp.h \
 swt/include/swttypedef.h EcmaScriptApp.h StereoViewApp.h \
 swt/include/stereoview.h InputDeviceApp.h TransformMode.h OutputApp.h \
 RouteViewApp.h StartWithApp.h ExportNamesApp.h ModellingApp.h \
 RenderApp.h Scene.h Map.h CommandList.h Command.h NodeList.h Proto.h \
 ProtoMacros.h InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h MFBool.h \
 FieldValue.h Arrays.h MySwap.h Types.h MFColor.h MFFloat.h SFFloat.h \
 MFColorRGBA.h MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h \
 MFNode.h MFRotation.h MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h \
 MFVec2f.h MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h \
 SFColorRGBA.h SFImage.h SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h \
 SFRotation.h SFString.h SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h \
 SFVec3f.h SFVec4f.h RenderState.h ProtoMap.h InputDevice.h EulerAngles.h \
 QuatTypes.h swt/include/swtxinput.h Aflock.h Path.h InputDeviceTime.h \
 Util.h VertexModifier.h MFieldCommand.h MyMesh.h NodeIndexedFaceSet.h \
 MeshBasedNode.h GeometryNode.h NodeAppearance.h NodeMaterial.h \
 NodeImageTexture.h KambiTextureCommonFields.h NodeTextureTransform.h \
 GeometryNodeMacros.h NodeCoordinate.h Colored.h NodeColor.h \
 NodeColorRGBA.h ComposedGeometryMacros.h FaceData.h NodeNormal.h \
 NodeTextureCoordinate.h NodeTextureCoordinateGenerator.h \
 NodeMultiTextureCoordinate.h NodeViewpoint.h ViewpointNode.h CoverNode.h \
 KambiViewpointCommonFields.h NodeTransform.h TransformNode.h \
 BasicTransformNode.h NodeNurbsSurface.h MeshMorphingNode.h \
 Interpolator.h NodeNurbsCurve.h NurbsCurve.h ChainBasedNode.h \
 NodeNurbsGroup.h NodeGroup.h StaticGroupNode.h GroupNode.h \
 NodeViewport.h NodeLayout.h NodeLayoutLayer.h NodeLayoutGroup.h
SceneView.o: SceneView.cpp stdafx.h config.h mysnprintf.h \
 swt/include/swt.h swt/include/keycodes.h swt/include/swlanguages.h \
 swt/include/swDebugf.h swt/include/swttypedef.h SceneView.h DuneApp.h \
 List.h Array.h MyString.h StringArray.h Node.h Vec3f.h Vec3x.h \
 FieldIndex.h Element.h x3dFlags.h swt/include/WriteFlags.h NodeFlags.h \
 MeshFlags.h ac3dMaterialCallback.h NodeArray.h Stack.h Matrix.h Vec3d.h \
 Quaternion.h URL.h PreferencesApp.h swt/include/swttypedef.h \
 EcmaScriptApp.h StereoViewApp.h swt/include/stereoview.h \
 InputDeviceApp.h TransformMode.h OutputApp.h RouteViewApp.h \
 StartWithApp.h ExportNamesApp.h ModellingApp.h RenderApp.h Scene.h Map.h \
 CommandList.h Command.h NodeList.h Proto.h ProtoMacros.h \
 InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h \
 Arrays.h MySwap.h Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h \
 MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h \
 MFRotation.h MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h \
 MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h \
 SFImage.h SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h \
 SFString.h SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h \
 SFVec4f.h RenderState.h ProtoMap.h resource.h
FieldView.o: FieldView.cpp stdafx.h config.h mysnprintf.h FieldView.h \
 Array.h SceneView.h DuneApp.h List.h MyString.h StringArray.h Node.h \
 Vec3f.h Vec3x.h FieldIndex.h Element.h x3dFlags.h \
 swt/include/WriteFlags.h NodeFlags.h MeshFlags.h ac3dMaterialCallback.h \
 NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h URL.h PreferencesApp.h \
 swt/include/swttypedef.h EcmaScriptApp.h StereoViewApp.h \
 swt/include/stereoview.h InputDeviceApp.h TransformMode.h OutputApp.h \
 RouteViewApp.h StartWithApp.h ExportNamesApp.h ModellingApp.h \
 RenderApp.h Scene.h Map.h CommandList.h Command.h NodeList.h Proto.h \
 ProtoMacros.h InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h MFBool.h \
 FieldValue.h Arrays.h MySwap.h Types.h MFColor.h MFFloat.h SFFloat.h \
 MFColorRGBA.h MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h \
 MFNode.h MFRotation.h MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h \
 MFVec2f.h MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h \
 SFColorRGBA.h SFImage.h SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h \
 SFRotation.h SFString.h SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h \
 SFVec3f.h SFVec4f.h RenderState.h ProtoMap.h Rect.h Point.h \
 swt/include/swt.h swt/include/keycodes.h swt/include/swlanguages.h \
 swt/include/swDebugf.h swt/include/swttypedef.h resource.h \
 FieldViewItem.h Field.h FieldCommand.h MFieldCommand.h Path.h
FieldViewItem.o: FieldViewItem.cpp stdafx.h config.h mysnprintf.h \
 FieldViewItem.h Array.h Rect.h Point.h MyString.h StringArray.h Field.h \
 Element.h x3dFlags.h swt/include/WriteFlags.h Node.h List.h Vec3f.h \
 Vec3x.h FieldIndex.h NodeFlags.h MeshFlags.h ac3dMaterialCallback.h \
 NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h Types.h \
 swt/include/swttypedef.h swt/include/swt.h swt/include/keycodes.h \
 swt/include/swlanguages.h swt/include/swDebugf.h \
 swt/include/swttypedef.h EulerAngles.h QuatTypes.h FieldView.h \
 SceneView.h DuneApp.h URL.h PreferencesApp.h EcmaScriptApp.h \
 StereoViewApp.h swt/include/stereoview.h InputDeviceApp.h \
 TransformMode.h OutputApp.h RouteViewApp.h StartWithApp.h \
 ExportNamesApp.h ModellingApp.h RenderApp.h Scene.h Map.h CommandList.h \
 Command.h NodeList.h Proto.h ProtoMacros.h InterfaceArray.h \
 DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h Arrays.h MySwap.h \
 MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h MFDouble.h SFDouble.h \
 MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h MFRotation.h MFString.h \
 MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h MFVec3f.h \
 MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h SFImage.h SFInt32.h \
 SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h SFString.h SFVec2d.h \
 SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h RenderState.h \
 ProtoMap.h FieldCommand.h Path.h SFMFTypesItem.h MFBoolItem.h \
 MFColorItem.h MFFloatItem.h MFColorRGBAItem.h MFDoubleItem.h \
 MFInt32Item.h MFMatrix3fItem.h MFMatrix4fItem.h MFNodeItem.h \
 MFRotationItem.h MFStringItem.h MFTimeItem.h MFVec2dItem.h MFVec2fItem.h \
 MFVec3dItem.h MFVec3fItem.h MFVec4fItem.h SFBoolItem.h SFColorItem.h \
 SFColorRGBAItem.h SFDoubleItem.h SFFloatItem.h SFImageItem.h \
 SFInt32Item.h SFMatrix3fItem.h SFMatrix4fItem.h SFNodeItem.h \
 SFRotationItem.h SFStringItem.h SFTimeItem.h SFVec2dItem.h SFVec2fItem.h \
 SFVec3dItem.h SFVec3fItem.h SFVec4fItem.h
URL.o: URL.cpp stdafx.h config.h mysnprintf.h URL.h MyString.h \
 StringArray.h Array.h maxpath.h DuneApp.h List.h Node.h Vec3f.h Vec3x.h \
 FieldIndex.h Element.h x3dFlags.h swt/include/WriteFlags.h NodeFlags.h \
 MeshFlags.h ac3dMaterialCallback.h NodeArray.h Stack.h Matrix.h Vec3d.h \
 Quaternion.h PreferencesApp.h swt/include/swttypedef.h EcmaScriptApp.h \
 StereoViewApp.h swt/include/stereoview.h InputDeviceApp.h \
 TransformMode.h OutputApp.h RouteViewApp.h StartWithApp.h \
 ExportNamesApp.h ModellingApp.h RenderApp.h swt/include/swt.h \
 swt/include/keycodes.h swt/include/swlanguages.h swt/include/swDebugf.h \
 swt/include/swttypedef.h
PanedWindow.o: PanedWindow.cpp stdafx.h config.h mysnprintf.h \
 swt/include/swt.h swt/include/keycodes.h swt/include/swlanguages.h \
 swt/include/swDebugf.h swt/include/swttypedef.h PanedWindow.h \
 SceneView.h DuneApp.h List.h Array.h MyString.h StringArray.h Node.h \
 Vec3f.h Vec3x.h FieldIndex.h Element.h x3dFlags.h \
 swt/include/WriteFlags.h NodeFlags.h MeshFlags.h ac3dMaterialCallback.h \
 NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h URL.h PreferencesApp.h \
 swt/include/swttypedef.h EcmaScriptApp.h StereoViewApp.h \
 swt/include/stereoview.h InputDeviceApp.h TransformMode.h OutputApp.h \
 RouteViewApp.h StartWithApp.h ExportNamesApp.h ModellingApp.h \
 RenderApp.h Scene.h Map.h CommandList.h Command.h NodeList.h Proto.h \
 ProtoMacros.h InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h MFBool.h \
 FieldValue.h Arrays.h MySwap.h Types.h MFColor.h MFFloat.h SFFloat.h \
 MFColorRGBA.h MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h \
 MFNode.h MFRotation.h MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h \
 MFVec2f.h MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h \
 SFColorRGBA.h SFImage.h SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h \
 SFRotation.h SFString.h SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h \
 SFVec3f.h SFVec4f.h RenderState.h ProtoMap.h
StatusBar.o: StatusBar.cpp stdafx.h config.h mysnprintf.h StatusBar.h \
 SceneView.h DuneApp.h List.h Array.h MyString.h StringArray.h Node.h \
 Vec3f.h Vec3x.h FieldIndex.h Element.h x3dFlags.h \
 swt/include/WriteFlags.h NodeFlags.h MeshFlags.h ac3dMaterialCallback.h \
 NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h URL.h PreferencesApp.h \
 swt/include/swttypedef.h EcmaScriptApp.h StereoViewApp.h \
 swt/include/stereoview.h InputDeviceApp.h TransformMode.h OutputApp.h \
 RouteViewApp.h StartWithApp.h ExportNamesApp.h ModellingApp.h \
 RenderApp.h Scene.h Map.h CommandList.h Command.h NodeList.h Proto.h \
 ProtoMacros.h InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h MFBool.h \
 FieldValue.h Arrays.h MySwap.h Types.h MFColor.h MFFloat.h SFFloat.h \
 MFColorRGBA.h MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h \
 MFNode.h MFRotation.h MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h \
 MFVec2f.h MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h \
 SFColorRGBA.h SFImage.h SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h \
 SFRotation.h SFString.h SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h \
 SFVec3f.h SFVec4f.h RenderState.h ProtoMap.h swt/include/swt.h \
 swt/include/keycodes.h swt/include/swlanguages.h swt/include/swDebugf.h \
 swt/include/swttypedef.h
ChannelView.o: ChannelView.cpp stdafx.h config.h mysnprintf.h \
 ChannelView.h Rect.h Point.h SceneView.h DuneApp.h List.h Array.h \
 MyString.h StringArray.h Node.h Vec3f.h Vec3x.h FieldIndex.h Element.h \
 x3dFlags.h swt/include/WriteFlags.h NodeFlags.h MeshFlags.h \
 ac3dMaterialCallback.h NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h \
 URL.h PreferencesApp.h swt/include/swttypedef.h EcmaScriptApp.h \
 StereoViewApp.h swt/include/stereoview.h InputDeviceApp.h \
 TransformMode.h OutputApp.h RouteViewApp.h StartWithApp.h \
 ExportNamesApp.h ModellingApp.h RenderApp.h Scene.h Map.h CommandList.h \
 Command.h NodeList.h Proto.h ProtoMacros.h InterfaceArray.h \
 DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h Arrays.h MySwap.h \
 Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h MFDouble.h \
 SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h MFRotation.h \
 MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h \
 MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h SFImage.h \
 SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h SFString.h \
 SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h \
 RenderState.h ProtoMap.h swt/include/swt.h swt/include/keycodes.h \
 swt/include/swlanguages.h swt/include/swDebugf.h \
 swt/include/swttypedef.h Interpolator.h Path.h
ToolbarWindow.o: ToolbarWindow.cpp stdafx.h config.h mysnprintf.h \
 ToolbarWindow.h SceneView.h DuneApp.h List.h Array.h MyString.h \
 StringArray.h Node.h Vec3f.h Vec3x.h FieldIndex.h Element.h x3dFlags.h \
 swt/include/WriteFlags.h NodeFlags.h MeshFlags.h ac3dMaterialCallback.h \
 NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h URL.h PreferencesApp.h \
 swt/include/swttypedef.h EcmaScriptApp.h StereoViewApp.h \
 swt/include/stereoview.h InputDeviceApp.h TransformMode.h OutputApp.h \
 RouteViewApp.h StartWithApp.h ExportNamesApp.h ModellingApp.h \
 RenderApp.h Scene.h Map.h CommandList.h Command.h NodeList.h Proto.h \
 ProtoMacros.h InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h MFBool.h \
 FieldValue.h Arrays.h MySwap.h Types.h MFColor.h MFFloat.h SFFloat.h \
 MFColorRGBA.h MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h \
 MFNode.h MFRotation.h MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h \
 MFVec2f.h MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h \
 SFColorRGBA.h SFImage.h SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h \
 SFRotation.h SFString.h SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h \
 SFVec3f.h SFVec4f.h RenderState.h ProtoMap.h MainWindow.h PanedWindow.h \
 ColorConversion.h NodeNurbsCurve.h NurbsCurve.h GeometryNode.h \
 NodeAppearance.h NodeMaterial.h swt/include/swt.h swt/include/keycodes.h \
 swt/include/swlanguages.h swt/include/swDebugf.h \
 swt/include/swttypedef.h NodeImageTexture.h KambiTextureCommonFields.h \
 NodeTextureTransform.h GeometryNodeMacros.h ChainBasedNode.h IconSize.h
EulerAngles.o: EulerAngles.cpp stdafx.h config.h mysnprintf.h \
 EulerAngles.h QuatTypes.h Quaternion.h Vec3f.h Vec3x.h Matrix.h Vec3d.h
xerrorhandler.o: xerrorhandler.cpp xerrorhandler.h stdafx.h config.h \
 mysnprintf.h MainWindow.h PanedWindow.h SceneView.h DuneApp.h List.h \
 Array.h MyString.h StringArray.h Node.h Vec3f.h Vec3x.h FieldIndex.h \
 Element.h x3dFlags.h swt/include/WriteFlags.h NodeFlags.h MeshFlags.h \
 ac3dMaterialCallback.h NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h \
 URL.h PreferencesApp.h swt/include/swttypedef.h EcmaScriptApp.h \
 StereoViewApp.h swt/include/stereoview.h InputDeviceApp.h \
 TransformMode.h OutputApp.h RouteViewApp.h StartWithApp.h \
 ExportNamesApp.h ModellingApp.h RenderApp.h Scene.h Map.h CommandList.h \
 Command.h NodeList.h Proto.h ProtoMacros.h InterfaceArray.h \
 DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h Arrays.h MySwap.h \
 Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h MFDouble.h \
 SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h MFRotation.h \
 MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h \
 MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h SFImage.h \
 SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h SFString.h \
 SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h \
 RenderState.h ProtoMap.h ColorConversion.h NodeNurbsCurve.h NurbsCurve.h \
 GeometryNode.h NodeAppearance.h NodeMaterial.h swt/include/swt.h \
 swt/include/keycodes.h swt/include/swlanguages.h swt/include/swDebugf.h \
 swt/include/swttypedef.h NodeImageTexture.h KambiTextureCommonFields.h \
 NodeTextureTransform.h GeometryNodeMacros.h ChainBasedNode.h maxpath.h
InputDevice.o: InputDevice.cpp InputDevice.h stdafx.h config.h \
 mysnprintf.h MyString.h StringArray.h Array.h swt/include/swt.h \
 swt/include/keycodes.h swt/include/swlanguages.h swt/include/swDebugf.h \
 swt/include/swttypedef.h TransformMode.h EulerAngles.h QuatTypes.h \
 Quaternion.h Vec3f.h Vec3x.h Matrix.h Vec3d.h Map.h List.h \
 swt/include/swtxinput.h Aflock.h SFRotation.h FieldValue.h Element.h \
 x3dFlags.h swt/include/WriteFlags.h Arrays.h MySwap.h Types.h
TransformMode.o: TransformMode.cpp TransformMode.h
ColorCircle.o: ColorCircle.cpp ColorCircle.h swt/include/swt.h \
 swt/include/keycodes.h swt/include/swlanguages.h swt/include/swDebugf.h \
 swt/include/swttypedef.h stdafx.h config.h mysnprintf.h Scene.h Array.h \
 Map.h List.h Stack.h CommandList.h Command.h NodeList.h NodeArray.h \
 Node.h Vec3f.h Vec3x.h MyString.h StringArray.h FieldIndex.h Element.h \
 x3dFlags.h swt/include/WriteFlags.h NodeFlags.h MeshFlags.h \
 ac3dMaterialCallback.h Matrix.h Vec3d.h Quaternion.h URL.h Proto.h \
 ProtoMacros.h InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h MFBool.h \
 FieldValue.h Arrays.h MySwap.h Types.h MFColor.h MFFloat.h SFFloat.h \
 MFColorRGBA.h MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h \
 MFNode.h MFRotation.h MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h \
 MFVec2f.h MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h \
 SFColorRGBA.h SFImage.h SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h \
 SFRotation.h SFString.h SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h \
 SFVec3f.h SFVec4f.h DuneApp.h PreferencesApp.h swt/include/swttypedef.h \
 EcmaScriptApp.h StereoViewApp.h swt/include/stereoview.h \
 InputDeviceApp.h TransformMode.h OutputApp.h RouteViewApp.h \
 StartWithApp.h ExportNamesApp.h ModellingApp.h RenderApp.h RenderState.h \
 ProtoMap.h SceneView.h MFieldCommand.h NodeColor.h NodeColorRGBA.h \
 NodeIndexedFaceSet.h MeshBasedNode.h GeometryNode.h NodeAppearance.h \
 NodeMaterial.h NodeImageTexture.h KambiTextureCommonFields.h \
 NodeTextureTransform.h GeometryNodeMacros.h NodeCoordinate.h Colored.h \
 ComposedGeometryMacros.h NodeElevationGrid.h NodeGeoElevationGrid.h \
 GeoNode.h NodeGeoOrigin.h MyMesh.h FaceData.h NodeNormal.h \
 NodeTextureCoordinate.h NodeTextureCoordinateGenerator.h \
 NodeMultiTextureCoordinate.h Util.h
Aflock.o: Aflock.cpp config.h stdafx.h config.h mysnprintf.h DuneApp.h \
 List.h Array.h MyString.h StringArray.h Node.h Vec3f.h Vec3x.h \
 FieldIndex.h Element.h x3dFlags.h swt/include/WriteFlags.h NodeFlags.h \
 MeshFlags.h ac3dMaterialCallback.h NodeArray.h Stack.h Matrix.h Vec3d.h \
 Quaternion.h URL.h PreferencesApp.h swt/include/swttypedef.h \
 EcmaScriptApp.h StereoViewApp.h swt/include/stereoview.h \
 InputDeviceApp.h TransformMode.h OutputApp.h RouteViewApp.h \
 StartWithApp.h ExportNamesApp.h ModellingApp.h RenderApp.h Aflock.h
ScriptEdit.o: ScriptEdit.cpp ScriptEdit.h Scene.h Array.h stdafx.h \
 config.h mysnprintf.h Map.h List.h Stack.h CommandList.h Command.h \
 NodeList.h NodeArray.h Node.h Vec3f.h Vec3x.h MyString.h StringArray.h \
 FieldIndex.h Element.h x3dFlags.h swt/include/WriteFlags.h NodeFlags.h \
 MeshFlags.h ac3dMaterialCallback.h Matrix.h Vec3d.h Quaternion.h URL.h \
 Proto.h ProtoMacros.h InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h \
 MFBool.h FieldValue.h Arrays.h MySwap.h Types.h MFColor.h MFFloat.h \
 SFFloat.h MFColorRGBA.h MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h \
 MFMatrix4f.h MFNode.h MFRotation.h MFString.h MFTime.h SFTime.h \
 MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h \
 SFBool.h SFColor.h SFColorRGBA.h SFImage.h SFInt32.h SFMatrix3f.h \
 SFMatrix4f.h SFNode.h SFRotation.h SFString.h SFVec2d.h SFVec2f.h \
 Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h DuneApp.h \
 PreferencesApp.h swt/include/swttypedef.h EcmaScriptApp.h \
 StereoViewApp.h swt/include/stereoview.h InputDeviceApp.h \
 TransformMode.h OutputApp.h RouteViewApp.h StartWithApp.h \
 ExportNamesApp.h ModellingApp.h RenderApp.h RenderState.h ProtoMap.h \
 swt/include/swt.h swt/include/keycodes.h swt/include/swlanguages.h \
 swt/include/swDebugf.h swt/include/swttypedef.h NodeScript.h \
 NodeImageTexture.h KambiTextureCommonFields.h NodePixelTexture.h \
 NodeMovieTexture.h NodeAudioClip.h Field.h EventIn.h EventOut.h \
 ExposedField.h resource.h NodePackagedShader.h NodeShaderProgram.h \
 NodeShaderPart.h NodeText.h GeometryNode.h NodeAppearance.h \
 NodeMaterial.h NodeTextureTransform.h GeometryNodeMacros.h \
 MeshBasedNode.h NodeText3D.h NodeWorldInfo.h
Types.o: Types.cpp Types.h MyString.h mysnprintf.h config.h StringArray.h \
 Array.h stdafx.h parser.h SFMFTypes.h MFBool.h FieldValue.h Element.h \
 x3dFlags.h swt/include/WriteFlags.h Arrays.h MySwap.h MFColor.h \
 MFFloat.h SFFloat.h MFColorRGBA.h MFDouble.h SFDouble.h MFInt32.h \
 MFMatrix3f.h MFMatrix4f.h MFNode.h Node.h List.h Vec3f.h Vec3x.h \
 FieldIndex.h NodeFlags.h MeshFlags.h ac3dMaterialCallback.h NodeArray.h \
 Stack.h Matrix.h Vec3d.h Quaternion.h NodeList.h MFRotation.h MFString.h \
 MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h MFVec3f.h \
 MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h SFImage.h SFInt32.h \
 SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h SFString.h SFVec2d.h \
 SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h
Commandline.o: Commandline.cpp config.h DuneApp.h List.h Array.h stdafx.h \
 mysnprintf.h MyString.h StringArray.h Node.h Vec3f.h Vec3x.h \
 FieldIndex.h Element.h x3dFlags.h swt/include/WriteFlags.h NodeFlags.h \
 MeshFlags.h ac3dMaterialCallback.h NodeArray.h Stack.h Matrix.h Vec3d.h \
 Quaternion.h URL.h PreferencesApp.h swt/include/swttypedef.h \
 EcmaScriptApp.h StereoViewApp.h swt/include/stereoview.h \
 InputDeviceApp.h TransformMode.h OutputApp.h RouteViewApp.h \
 StartWithApp.h ExportNamesApp.h ModellingApp.h RenderApp.h InputDevice.h \
 swt/include/swt.h swt/include/keycodes.h swt/include/swlanguages.h \
 swt/include/swDebugf.h swt/include/swttypedef.h EulerAngles.h \
 QuatTypes.h Map.h swt/include/swtxinput.h Aflock.h xerrorhandler.h \
 MainWindow.h PanedWindow.h SceneView.h Scene.h CommandList.h Command.h \
 NodeList.h Proto.h ProtoMacros.h InterfaceArray.h DynamicFieldsNode.h \
 SFMFTypes.h MFBool.h FieldValue.h Arrays.h MySwap.h Types.h MFColor.h \
 MFFloat.h SFFloat.h MFColorRGBA.h MFDouble.h SFDouble.h MFInt32.h \
 MFMatrix3f.h MFMatrix4f.h MFNode.h MFRotation.h MFString.h MFTime.h \
 SFTime.h MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h MFVec3f.h MFVec4f.h \
 Vec4f.h SFBool.h SFColor.h SFColorRGBA.h SFImage.h SFInt32.h \
 SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h SFString.h SFVec2d.h \
 SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h RenderState.h \
 ProtoMap.h ColorConversion.h NodeNurbsCurve.h NurbsCurve.h \
 GeometryNode.h NodeAppearance.h NodeMaterial.h NodeImageTexture.h \
 KambiTextureCommonFields.h NodeTextureTransform.h GeometryNodeMacros.h \
 ChainBasedNode.h IconSize.h CommandlineLanguages.h
NurbsArc.o: NurbsArc.cpp stdafx.h config.h mysnprintf.h NurbsArc.h \
 Vec2f.h Vec3f.h Vec3x.h Array.h NurbsCurveDegreeElevate.h
NurbsCurveDegreeElevate.o: NurbsCurveDegreeElevate.cpp \
 NurbsCurveDegreeElevate.h Vec3f.h Vec3x.h stdafx.h config.h mysnprintf.h \
 Array.h Array2D.h Vec4f.h
NurbsSurfaceDegreeElevate.o: NurbsSurfaceDegreeElevate.cpp \
 NurbsSurfaceDegreeElevate.h Vec3f.h Vec3x.h stdafx.h config.h \
 mysnprintf.h Array.h NurbsCurveDegreeElevate.h
NurbsMakeRevolvedSurface.o: NurbsMakeRevolvedSurface.cpp stdafx.h \
 config.h mysnprintf.h NurbsArc.h Vec2f.h Vec3f.h Vec3x.h Array.h \
 NurbsMakeRevolvedSurface.h
CheckBoxWindow.o: CheckBoxWindow.cpp CheckBoxWindow.h swt/include/swt.h \
 swt/include/keycodes.h swt/include/swlanguages.h swt/include/swDebugf.h \
 swt/include/swttypedef.h swt/include/swttypedef.h MyString.h \
 mysnprintf.h config.h StringArray.h Array.h stdafx.h Dialog.h
GpsMathLib.o: GpsMathLib.cpp gpsdatum.h gpsmath.h gpsport.h
WonderlandModuleExport.o: WonderlandModuleExport.cpp config.h stdafx.h \
 mysnprintf.h swt/include/swt.h swt/include/keycodes.h \
 swt/include/swlanguages.h swt/include/swDebugf.h \
 swt/include/swttypedef.h MyString.h StringArray.h Array.h \
 WonderlandModuleExport.h Scene.h Map.h List.h Stack.h CommandList.h \
 Command.h NodeList.h NodeArray.h Node.h Vec3f.h Vec3x.h FieldIndex.h \
 Element.h x3dFlags.h swt/include/WriteFlags.h NodeFlags.h MeshFlags.h \
 ac3dMaterialCallback.h Matrix.h Vec3d.h Quaternion.h URL.h Proto.h \
 ProtoMacros.h InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h MFBool.h \
 FieldValue.h Arrays.h MySwap.h Types.h MFColor.h MFFloat.h SFFloat.h \
 MFColorRGBA.h MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h \
 MFNode.h MFRotation.h MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h \
 MFVec2f.h MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h \
 SFColorRGBA.h SFImage.h SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h \
 SFRotation.h SFString.h SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h \
 SFVec3f.h SFVec4f.h DuneApp.h PreferencesApp.h swt/include/swttypedef.h \
 EcmaScriptApp.h StereoViewApp.h swt/include/stereoview.h \
 InputDeviceApp.h TransformMode.h OutputApp.h RouteViewApp.h \
 StartWithApp.h ExportNamesApp.h ModellingApp.h RenderApp.h RenderState.h \
 ProtoMap.h NodeWonderlandImportJava.h /usr/include/libpng16/png.h \
 /usr/include/libpng16/pnglibconf.h /usr/include/libpng16/pngconf.h
WriteWonderlandCellRenderCode.o: WriteWonderlandCellRenderCode.cpp \
 DuneApp.h List.h Array.h stdafx.h config.h mysnprintf.h MyString.h \
 StringArray.h Node.h Vec3f.h Vec3x.h FieldIndex.h Element.h x3dFlags.h \
 swt/include/WriteFlags.h NodeFlags.h MeshFlags.h ac3dMaterialCallback.h \
 NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h URL.h PreferencesApp.h \
 swt/include/swttypedef.h EcmaScriptApp.h StereoViewApp.h \
 swt/include/stereoview.h InputDeviceApp.h TransformMode.h OutputApp.h \
 RouteViewApp.h StartWithApp.h ExportNamesApp.h ModellingApp.h \
 RenderApp.h swt/include/swt.h swt/include/keycodes.h \
 swt/include/swlanguages.h swt/include/swDebugf.h \
 swt/include/swttypedef.h
NurbsCurve.o: NurbsCurve.cpp stdafx.h config.h mysnprintf.h NurbsCurve.h \
 GeometryNode.h Node.h Array.h List.h Vec3f.h Vec3x.h MyString.h \
 StringArray.h FieldIndex.h Element.h x3dFlags.h swt/include/WriteFlags.h \
 NodeFlags.h MeshFlags.h ac3dMaterialCallback.h NodeArray.h Stack.h \
 Matrix.h Vec3d.h Quaternion.h NodeAppearance.h ProtoMacros.h Proto.h \
 Map.h NodeList.h InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h \
 MFBool.h FieldValue.h Arrays.h MySwap.h Types.h MFColor.h MFFloat.h \
 SFFloat.h MFColorRGBA.h MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h \
 MFMatrix4f.h MFNode.h MFRotation.h MFString.h MFTime.h SFTime.h \
 MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h \
 SFBool.h SFColor.h SFColorRGBA.h SFImage.h SFInt32.h SFMatrix3f.h \
 SFMatrix4f.h SFNode.h SFRotation.h SFString.h SFVec2d.h SFVec2f.h \
 Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h CommandList.h Command.h \
 DuneApp.h URL.h PreferencesApp.h swt/include/swttypedef.h \
 EcmaScriptApp.h StereoViewApp.h swt/include/stereoview.h \
 InputDeviceApp.h TransformMode.h OutputApp.h RouteViewApp.h \
 StartWithApp.h ExportNamesApp.h ModellingApp.h RenderApp.h \
 NodeMaterial.h swt/include/swt.h swt/include/keycodes.h \
 swt/include/swlanguages.h swt/include/swDebugf.h \
 swt/include/swttypedef.h NodeImageTexture.h KambiTextureCommonFields.h \
 NodeTextureTransform.h GeometryNodeMacros.h ChainBasedNode.h
Vec3x.o: Vec3x.cpp Vec3d.h Vec3x.h stdafx.h config.h mysnprintf.h Vec3f.h
VertexModifier.o: VertexModifier.cpp VertexModifier.h swt/include/swt.h \
 swt/include/keycodes.h swt/include/swlanguages.h swt/include/swDebugf.h \
 swt/include/swttypedef.h stdafx.h config.h mysnprintf.h Scene.h Array.h \
 Map.h List.h Stack.h CommandList.h Command.h NodeList.h NodeArray.h \
 Node.h Vec3f.h Vec3x.h MyString.h StringArray.h FieldIndex.h Element.h \
 x3dFlags.h swt/include/WriteFlags.h NodeFlags.h MeshFlags.h \
 ac3dMaterialCallback.h Matrix.h Vec3d.h Quaternion.h URL.h Proto.h \
 ProtoMacros.h InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h MFBool.h \
 FieldValue.h Arrays.h MySwap.h Types.h MFColor.h MFFloat.h SFFloat.h \
 MFColorRGBA.h MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h \
 MFNode.h MFRotation.h MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h \
 MFVec2f.h MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h \
 SFColorRGBA.h SFImage.h SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h \
 SFRotation.h SFString.h SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h \
 SFVec3f.h SFVec4f.h DuneApp.h PreferencesApp.h swt/include/swttypedef.h \
 EcmaScriptApp.h StereoViewApp.h swt/include/stereoview.h \
 InputDeviceApp.h TransformMode.h OutputApp.h RouteViewApp.h \
 StartWithApp.h ExportNamesApp.h ModellingApp.h RenderApp.h RenderState.h \
 ProtoMap.h SceneView.h MFieldCommand.h resource.h NodeCoordinate.h \
 NodeNurbsSurface.h MeshMorphingNode.h Interpolator.h MeshBasedNode.h \
 GeometryNode.h NodeAppearance.h NodeMaterial.h NodeImageTexture.h \
 KambiTextureCommonFields.h NodeTextureTransform.h GeometryNodeMacros.h
illegal2vrml.o: illegal2vrml.cpp stdafx.h config.h mysnprintf.h DuneApp.h \
 List.h Array.h MyString.h StringArray.h Node.h Vec3f.h Vec3x.h \
 FieldIndex.h Element.h x3dFlags.h swt/include/WriteFlags.h NodeFlags.h \
 MeshFlags.h ac3dMaterialCallback.h NodeArray.h Stack.h Matrix.h Vec3d.h \
 Quaternion.h URL.h PreferencesApp.h swt/include/swttypedef.h \
 EcmaScriptApp.h StereoViewApp.h swt/include/stereoview.h \
 InputDeviceApp.h TransformMode.h OutputApp.h RouteViewApp.h \
 StartWithApp.h ExportNamesApp.h ModellingApp.h RenderApp.h Scene.h Map.h \
 CommandList.h Command.h NodeList.h Proto.h ProtoMacros.h \
 InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h \
 Arrays.h MySwap.h Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h \
 MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h \
 MFRotation.h MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h \
 MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h \
 SFImage.h SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h \
 SFString.h SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h \
 SFVec4f.h RenderState.h ProtoMap.h xerrorhandler.h
Dialog.o: Dialog.cpp stdafx.h config.h mysnprintf.h Dialog.h \
 swt/include/swttypedef.h swt/include/swt.h swt/include/keycodes.h \
 swt/include/swlanguages.h swt/include/swDebugf.h \
 swt/include/swttypedef.h DuneApp.h List.h Array.h MyString.h \
 StringArray.h Node.h Vec3f.h Vec3x.h FieldIndex.h Element.h x3dFlags.h \
 swt/include/WriteFlags.h NodeFlags.h MeshFlags.h ac3dMaterialCallback.h \
 NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h URL.h PreferencesApp.h \
 EcmaScriptApp.h StereoViewApp.h swt/include/stereoview.h \
 InputDeviceApp.h TransformMode.h OutputApp.h RouteViewApp.h \
 StartWithApp.h ExportNamesApp.h ModellingApp.h RenderApp.h resource.h
ElevationGridDialog.o: ElevationGridDialog.cpp stdafx.h config.h \
 mysnprintf.h ElevationGridDialog.h Dialog.h swt/include/swttypedef.h \
 swt/include/swt.h swt/include/keycodes.h swt/include/swlanguages.h \
 swt/include/swDebugf.h swt/include/swttypedef.h resource.h
ScriptDialog.o: ScriptDialog.cpp stdafx.h config.h mysnprintf.h \
 ScriptDialog.h Dialog.h swt/include/swttypedef.h NodeScript.h Node.h \
 Array.h List.h Vec3f.h Vec3x.h MyString.h StringArray.h FieldIndex.h \
 Element.h x3dFlags.h swt/include/WriteFlags.h NodeFlags.h MeshFlags.h \
 ac3dMaterialCallback.h NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h \
 ProtoMacros.h Proto.h Map.h NodeList.h InterfaceArray.h \
 DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h Arrays.h MySwap.h \
 Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h MFDouble.h \
 SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h MFRotation.h \
 MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h \
 MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h SFImage.h \
 SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h SFString.h \
 SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h \
 CommandList.h Command.h Scene.h URL.h DuneApp.h PreferencesApp.h \
 EcmaScriptApp.h StereoViewApp.h swt/include/stereoview.h \
 InputDeviceApp.h TransformMode.h OutputApp.h RouteViewApp.h \
 StartWithApp.h ExportNamesApp.h ModellingApp.h RenderApp.h RenderState.h \
 ProtoMap.h swt/include/swt.h swt/include/keycodes.h \
 swt/include/swlanguages.h swt/include/swDebugf.h \
 swt/include/swttypedef.h ScriptEdit.h NodeImageTexture.h \
 KambiTextureCommonFields.h NodePixelTexture.h NodeMovieTexture.h \
 NodeAudioClip.h resource.h Field.h EventIn.h EventOut.h ExposedField.h
NurbsCurveDialog.o: NurbsCurveDialog.cpp stdafx.h config.h mysnprintf.h \
 NurbsCurveDialog.h Dialog.h swt/include/swttypedef.h swt/include/swt.h \
 swt/include/keycodes.h swt/include/swlanguages.h swt/include/swDebugf.h \
 swt/include/swttypedef.h resource.h
NurbsPlaneDialog.o: NurbsPlaneDialog.cpp stdafx.h config.h mysnprintf.h \
 NurbsPlaneDialog.h Dialog.h swt/include/swttypedef.h swt/include/swt.h \
 swt/include/keycodes.h swt/include/swlanguages.h swt/include/swDebugf.h \
 swt/include/swttypedef.h resource.h
ModellingSettingsDialog.o: ModellingSettingsDialog.cpp stdafx.h config.h \
 mysnprintf.h ModellingSettingsDialog.h Dialog.h swt/include/swttypedef.h \
 DuneApp.h List.h Array.h MyString.h StringArray.h Node.h Vec3f.h Vec3x.h \
 FieldIndex.h Element.h x3dFlags.h swt/include/WriteFlags.h NodeFlags.h \
 MeshFlags.h ac3dMaterialCallback.h NodeArray.h Stack.h Matrix.h Vec3d.h \
 Quaternion.h URL.h PreferencesApp.h EcmaScriptApp.h StereoViewApp.h \
 swt/include/stereoview.h InputDeviceApp.h TransformMode.h OutputApp.h \
 RouteViewApp.h StartWithApp.h ExportNamesApp.h ModellingApp.h \
 RenderApp.h resource.h swt/include/swt.h swt/include/keycodes.h \
 swt/include/swlanguages.h swt/include/swDebugf.h \
 swt/include/swttypedef.h EulerAngles.h QuatTypes.h
RenderSettingsDialog.o: RenderSettingsDialog.cpp stdafx.h config.h \
 mysnprintf.h RenderSettingsDialog.h Dialog.h swt/include/swttypedef.h \
 DuneApp.h List.h Array.h MyString.h StringArray.h Node.h Vec3f.h Vec3x.h \
 FieldIndex.h Element.h x3dFlags.h swt/include/WriteFlags.h NodeFlags.h \
 MeshFlags.h ac3dMaterialCallback.h NodeArray.h Stack.h Matrix.h Vec3d.h \
 Quaternion.h URL.h PreferencesApp.h EcmaScriptApp.h StereoViewApp.h \
 swt/include/stereoview.h InputDeviceApp.h TransformMode.h OutputApp.h \
 RouteViewApp.h StartWithApp.h ExportNamesApp.h ModellingApp.h \
 RenderApp.h resource.h swt/include/swt.h swt/include/keycodes.h \
 swt/include/swlanguages.h swt/include/swDebugf.h \
 swt/include/swttypedef.h
DefDialog.o: DefDialog.cpp stdafx.h config.h mysnprintf.h resource.h \
 MyString.h StringArray.h Array.h Path.h FieldValue.h Element.h \
 x3dFlags.h swt/include/WriteFlags.h Arrays.h MySwap.h Types.h NodeList.h \
 NodeArray.h Node.h List.h Vec3f.h Vec3x.h FieldIndex.h NodeFlags.h \
 MeshFlags.h ac3dMaterialCallback.h Stack.h Matrix.h Vec3d.h Quaternion.h \
 DefDialog.h Dialog.h swt/include/swttypedef.h Scene.h Map.h \
 CommandList.h Command.h URL.h Proto.h ProtoMacros.h InterfaceArray.h \
 DynamicFieldsNode.h SFMFTypes.h MFBool.h MFColor.h MFFloat.h SFFloat.h \
 MFColorRGBA.h MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h \
 MFNode.h MFRotation.h MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h \
 MFVec2f.h MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h \
 SFColorRGBA.h SFImage.h SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h \
 SFRotation.h SFString.h SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h \
 SFVec3f.h SFVec4f.h DuneApp.h PreferencesApp.h EcmaScriptApp.h \
 StereoViewApp.h swt/include/stereoview.h InputDeviceApp.h \
 TransformMode.h OutputApp.h RouteViewApp.h StartWithApp.h \
 ExportNamesApp.h ModellingApp.h RenderApp.h RenderState.h ProtoMap.h \
 swt/include/swt.h swt/include/keycodes.h swt/include/swlanguages.h \
 swt/include/swDebugf.h swt/include/swttypedef.h
CoverDefDialog.o: CoverDefDialog.cpp stdafx.h config.h mysnprintf.h \
 resource.h MyString.h StringArray.h Array.h Path.h FieldValue.h \
 Element.h x3dFlags.h swt/include/WriteFlags.h Arrays.h MySwap.h Types.h \
 NodeList.h NodeArray.h Node.h List.h Vec3f.h Vec3x.h FieldIndex.h \
 NodeFlags.h MeshFlags.h ac3dMaterialCallback.h Stack.h Matrix.h Vec3d.h \
 Quaternion.h CoverDefDialog.h Dialog.h swt/include/swttypedef.h Scene.h \
 Map.h CommandList.h Command.h URL.h Proto.h ProtoMacros.h \
 InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h MFBool.h MFColor.h \
 MFFloat.h SFFloat.h MFColorRGBA.h MFDouble.h SFDouble.h MFInt32.h \
 MFMatrix3f.h MFMatrix4f.h MFNode.h MFRotation.h MFString.h MFTime.h \
 SFTime.h MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h MFVec3f.h MFVec4f.h \
 Vec4f.h SFBool.h SFColor.h SFColorRGBA.h SFImage.h SFInt32.h \
 SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h SFString.h SFVec2d.h \
 SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h DuneApp.h \
 PreferencesApp.h EcmaScriptApp.h StereoViewApp.h \
 swt/include/stereoview.h InputDeviceApp.h TransformMode.h OutputApp.h \
 RouteViewApp.h StartWithApp.h ExportNamesApp.h ModellingApp.h \
 RenderApp.h RenderState.h ProtoMap.h swt/include/swt.h \
 swt/include/keycodes.h swt/include/swlanguages.h swt/include/swDebugf.h \
 swt/include/swttypedef.h
Box2NurbsDialog.o: Box2NurbsDialog.cpp stdafx.h config.h mysnprintf.h \
 Box2NurbsDialog.h Dialog.h swt/include/swttypedef.h Node.h Array.h \
 List.h Vec3f.h Vec3x.h MyString.h StringArray.h FieldIndex.h Element.h \
 x3dFlags.h swt/include/WriteFlags.h NodeFlags.h MeshFlags.h \
 ac3dMaterialCallback.h NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h \
 swt/include/swt.h swt/include/keycodes.h swt/include/swlanguages.h \
 swt/include/swDebugf.h swt/include/swttypedef.h resource.h DuneApp.h \
 URL.h PreferencesApp.h EcmaScriptApp.h StereoViewApp.h \
 swt/include/stereoview.h InputDeviceApp.h TransformMode.h OutputApp.h \
 RouteViewApp.h StartWithApp.h ExportNamesApp.h ModellingApp.h \
 RenderApp.h NodeBox.h MeshBasedNode.h GeometryNode.h NodeAppearance.h \
 ProtoMacros.h Proto.h Map.h NodeList.h InterfaceArray.h \
 DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h Arrays.h MySwap.h \
 Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h MFDouble.h \
 SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h MFRotation.h \
 MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h \
 MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h SFImage.h \
 SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h SFString.h \
 SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h \
 CommandList.h Command.h NodeMaterial.h NodeImageTexture.h \
 KambiTextureCommonFields.h NodeTextureTransform.h GeometryNodeMacros.h
Cone2NurbsDialog.o: Cone2NurbsDialog.cpp stdafx.h config.h mysnprintf.h \
 Cone2NurbsDialog.h Dialog.h swt/include/swttypedef.h Node.h Array.h \
 List.h Vec3f.h Vec3x.h MyString.h StringArray.h FieldIndex.h Element.h \
 x3dFlags.h swt/include/WriteFlags.h NodeFlags.h MeshFlags.h \
 ac3dMaterialCallback.h NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h \
 swt/include/swt.h swt/include/keycodes.h swt/include/swlanguages.h \
 swt/include/swDebugf.h swt/include/swttypedef.h resource.h DuneApp.h \
 URL.h PreferencesApp.h EcmaScriptApp.h StereoViewApp.h \
 swt/include/stereoview.h InputDeviceApp.h TransformMode.h OutputApp.h \
 RouteViewApp.h StartWithApp.h ExportNamesApp.h ModellingApp.h \
 RenderApp.h NodeCone.h MeshBasedNode.h GeometryNode.h NodeAppearance.h \
 ProtoMacros.h Proto.h Map.h NodeList.h InterfaceArray.h \
 DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h Arrays.h MySwap.h \
 Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h MFDouble.h \
 SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h MFRotation.h \
 MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h \
 MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h SFImage.h \
 SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h SFString.h \
 SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h \
 CommandList.h Command.h NodeMaterial.h NodeImageTexture.h \
 KambiTextureCommonFields.h NodeTextureTransform.h GeometryNodeMacros.h
Cylinder2NurbsDialog.o: Cylinder2NurbsDialog.cpp stdafx.h config.h \
 mysnprintf.h Cylinder2NurbsDialog.h Dialog.h swt/include/swttypedef.h \
 Node.h Array.h List.h Vec3f.h Vec3x.h MyString.h StringArray.h \
 FieldIndex.h Element.h x3dFlags.h swt/include/WriteFlags.h NodeFlags.h \
 MeshFlags.h ac3dMaterialCallback.h NodeArray.h Stack.h Matrix.h Vec3d.h \
 Quaternion.h swt/include/swt.h swt/include/keycodes.h \
 swt/include/swlanguages.h swt/include/swDebugf.h \
 swt/include/swttypedef.h resource.h DuneApp.h URL.h PreferencesApp.h \
 EcmaScriptApp.h StereoViewApp.h swt/include/stereoview.h \
 InputDeviceApp.h TransformMode.h OutputApp.h RouteViewApp.h \
 StartWithApp.h ExportNamesApp.h ModellingApp.h RenderApp.h \
 NodeCylinder.h MeshBasedNode.h GeometryNode.h NodeAppearance.h \
 ProtoMacros.h Proto.h Map.h NodeList.h InterfaceArray.h \
 DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h Arrays.h MySwap.h \
 Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h MFDouble.h \
 SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h MFRotation.h \
 MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h \
 MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h SFImage.h \
 SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h SFString.h \
 SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h \
 CommandList.h Command.h NodeMaterial.h NodeImageTexture.h \
 KambiTextureCommonFields.h NodeTextureTransform.h GeometryNodeMacros.h
Sphere2NurbsDialog.o: Sphere2NurbsDialog.cpp stdafx.h config.h \
 mysnprintf.h Sphere2NurbsDialog.h Dialog.h swt/include/swttypedef.h \
 swt/include/swt.h swt/include/keycodes.h swt/include/swlanguages.h \
 swt/include/swDebugf.h swt/include/swttypedef.h resource.h DuneApp.h \
 List.h Array.h MyString.h StringArray.h Node.h Vec3f.h Vec3x.h \
 FieldIndex.h Element.h x3dFlags.h swt/include/WriteFlags.h NodeFlags.h \
 MeshFlags.h ac3dMaterialCallback.h NodeArray.h Stack.h Matrix.h Vec3d.h \
 Quaternion.h URL.h PreferencesApp.h EcmaScriptApp.h StereoViewApp.h \
 swt/include/stereoview.h InputDeviceApp.h TransformMode.h OutputApp.h \
 RouteViewApp.h StartWithApp.h ExportNamesApp.h ModellingApp.h \
 RenderApp.h
NurbsCurve2NurbsSurfDialog.o: NurbsCurve2NurbsSurfDialog.cpp Node.h \
 Array.h stdafx.h config.h mysnprintf.h List.h Vec3f.h Vec3x.h MyString.h \
 StringArray.h FieldIndex.h Element.h x3dFlags.h swt/include/WriteFlags.h \
 NodeFlags.h MeshFlags.h ac3dMaterialCallback.h NodeArray.h Stack.h \
 Matrix.h Vec3d.h Quaternion.h SFBool.h FieldValue.h Arrays.h MySwap.h \
 Types.h NurbsCurve2NurbsSurfDialog.h Dialog.h swt/include/swttypedef.h \
 Scene.h Map.h CommandList.h Command.h NodeList.h URL.h Proto.h \
 ProtoMacros.h InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h MFBool.h \
 MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h MFDouble.h SFDouble.h \
 MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h MFRotation.h MFString.h \
 MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h MFVec3f.h \
 MFVec4f.h Vec4f.h SFColor.h SFColorRGBA.h SFImage.h SFInt32.h \
 SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h SFString.h SFVec2d.h \
 SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h DuneApp.h \
 PreferencesApp.h EcmaScriptApp.h StereoViewApp.h \
 swt/include/stereoview.h InputDeviceApp.h TransformMode.h OutputApp.h \
 RouteViewApp.h StartWithApp.h ExportNamesApp.h ModellingApp.h \
 RenderApp.h RenderState.h ProtoMap.h resource.h NodeNurbsCurve.h \
 NurbsCurve.h GeometryNode.h NodeAppearance.h NodeMaterial.h \
 swt/include/swt.h swt/include/keycodes.h swt/include/swlanguages.h \
 swt/include/swDebugf.h swt/include/swttypedef.h NodeImageTexture.h \
 KambiTextureCommonFields.h NodeTextureTransform.h GeometryNodeMacros.h \
 ChainBasedNode.h
InteractionDialog.o: InteractionDialog.cpp InteractionDialog.h Scene.h \
 Array.h stdafx.h config.h mysnprintf.h Map.h List.h Stack.h \
 CommandList.h Command.h NodeList.h NodeArray.h Node.h Vec3f.h Vec3x.h \
 MyString.h StringArray.h FieldIndex.h Element.h x3dFlags.h \
 swt/include/WriteFlags.h NodeFlags.h MeshFlags.h ac3dMaterialCallback.h \
 Matrix.h Vec3d.h Quaternion.h URL.h Proto.h ProtoMacros.h \
 InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h \
 Arrays.h MySwap.h Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h \
 MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h \
 MFRotation.h MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h \
 MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h \
 SFImage.h SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h \
 SFString.h SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h \
 SFVec4f.h DuneApp.h PreferencesApp.h swt/include/swttypedef.h \
 EcmaScriptApp.h StereoViewApp.h swt/include/stereoview.h \
 InputDeviceApp.h TransformMode.h OutputApp.h RouteViewApp.h \
 StartWithApp.h ExportNamesApp.h ModellingApp.h RenderApp.h RenderState.h \
 ProtoMap.h CheckBoxWindow.h swt/include/swt.h swt/include/keycodes.h \
 swt/include/swlanguages.h swt/include/swDebugf.h \
 swt/include/swttypedef.h Dialog.h resource.h Field.h EventIn.h \
 EventOut.h ExposedField.h NodeTransform.h TransformNode.h \
 BasicTransformNode.h
InteractionDialog2.o: InteractionDialog2.cpp InteractionDialog2.h Scene.h \
 Array.h stdafx.h config.h mysnprintf.h Map.h List.h Stack.h \
 CommandList.h Command.h NodeList.h NodeArray.h Node.h Vec3f.h Vec3x.h \
 MyString.h StringArray.h FieldIndex.h Element.h x3dFlags.h \
 swt/include/WriteFlags.h NodeFlags.h MeshFlags.h ac3dMaterialCallback.h \
 Matrix.h Vec3d.h Quaternion.h URL.h Proto.h ProtoMacros.h \
 InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h \
 Arrays.h MySwap.h Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h \
 MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h \
 MFRotation.h MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h \
 MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h \
 SFImage.h SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h \
 SFString.h SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h \
 SFVec4f.h DuneApp.h PreferencesApp.h swt/include/swttypedef.h \
 EcmaScriptApp.h StereoViewApp.h swt/include/stereoview.h \
 InputDeviceApp.h TransformMode.h OutputApp.h RouteViewApp.h \
 StartWithApp.h ExportNamesApp.h ModellingApp.h RenderApp.h RenderState.h \
 ProtoMap.h CheckBoxWindow.h swt/include/swt.h swt/include/keycodes.h \
 swt/include/swlanguages.h swt/include/swDebugf.h \
 swt/include/swttypedef.h Dialog.h InteractionDialog.h resource.h Field.h \
 EventIn.h EventOut.h ExposedField.h NodeTransform.h TransformNode.h \
 BasicTransformNode.h
InteractionDialog3.o: InteractionDialog3.cpp InteractionDialog3.h Scene.h \
 Array.h stdafx.h config.h mysnprintf.h Map.h List.h Stack.h \
 CommandList.h Command.h NodeList.h NodeArray.h Node.h Vec3f.h Vec3x.h \
 MyString.h StringArray.h FieldIndex.h Element.h x3dFlags.h \
 swt/include/WriteFlags.h NodeFlags.h MeshFlags.h ac3dMaterialCallback.h \
 Matrix.h Vec3d.h Quaternion.h URL.h Proto.h ProtoMacros.h \
 InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h \
 Arrays.h MySwap.h Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h \
 MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h \
 MFRotation.h MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h \
 MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h \
 SFImage.h SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h \
 SFString.h SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h \
 SFVec4f.h DuneApp.h PreferencesApp.h swt/include/swttypedef.h \
 EcmaScriptApp.h StereoViewApp.h swt/include/stereoview.h \
 InputDeviceApp.h TransformMode.h OutputApp.h RouteViewApp.h \
 StartWithApp.h ExportNamesApp.h ModellingApp.h RenderApp.h RenderState.h \
 ProtoMap.h CheckBoxWindow.h swt/include/swt.h swt/include/keycodes.h \
 swt/include/swlanguages.h swt/include/swDebugf.h \
 swt/include/swttypedef.h Dialog.h InteractionDialog.h resource.h Field.h \
 EventIn.h EventOut.h ExposedField.h NodeTransform.h TransformNode.h \
 BasicTransformNode.h
AnimationDialog.o: AnimationDialog.cpp AnimationDialog.h Scene.h Array.h \
 stdafx.h config.h mysnprintf.h Map.h List.h Stack.h CommandList.h \
 Command.h NodeList.h NodeArray.h Node.h Vec3f.h Vec3x.h MyString.h \
 StringArray.h FieldIndex.h Element.h x3dFlags.h swt/include/WriteFlags.h \
 NodeFlags.h MeshFlags.h ac3dMaterialCallback.h Matrix.h Vec3d.h \
 Quaternion.h URL.h Proto.h ProtoMacros.h InterfaceArray.h \
 DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h Arrays.h MySwap.h \
 Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h MFDouble.h \
 SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h MFRotation.h \
 MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h \
 MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h SFImage.h \
 SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h SFString.h \
 SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h \
 DuneApp.h PreferencesApp.h swt/include/swttypedef.h EcmaScriptApp.h \
 StereoViewApp.h swt/include/stereoview.h InputDeviceApp.h \
 TransformMode.h OutputApp.h RouteViewApp.h StartWithApp.h \
 ExportNamesApp.h ModellingApp.h RenderApp.h RenderState.h ProtoMap.h \
 CheckBoxWindow.h swt/include/swt.h swt/include/keycodes.h \
 swt/include/swlanguages.h swt/include/swDebugf.h \
 swt/include/swttypedef.h Dialog.h resource.h Field.h EventIn.h \
 EventOut.h ExposedField.h NodeTransform.h TransformNode.h \
 BasicTransformNode.h NodeTimeSensor.h
CurveAnimationDialog.o: CurveAnimationDialog.cpp stdafx.h config.h \
 mysnprintf.h CurveAnimationDialog.h Dialog.h swt/include/swttypedef.h \
 swt/include/swt.h swt/include/keycodes.h swt/include/swlanguages.h \
 swt/include/swDebugf.h swt/include/swttypedef.h resource.h MyString.h \
 StringArray.h Array.h NodeList.h NodeArray.h Node.h List.h Vec3f.h \
 Vec3x.h FieldIndex.h Element.h x3dFlags.h swt/include/WriteFlags.h \
 NodeFlags.h MeshFlags.h ac3dMaterialCallback.h Stack.h Matrix.h Vec3d.h \
 Quaternion.h Scene.h Map.h CommandList.h Command.h URL.h Proto.h \
 ProtoMacros.h InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h MFBool.h \
 FieldValue.h Arrays.h MySwap.h Types.h MFColor.h MFFloat.h SFFloat.h \
 MFColorRGBA.h MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h \
 MFNode.h MFRotation.h MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h \
 MFVec2f.h MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h \
 SFColorRGBA.h SFImage.h SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h \
 SFRotation.h SFString.h SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h \
 SFVec3f.h SFVec4f.h DuneApp.h PreferencesApp.h EcmaScriptApp.h \
 StereoViewApp.h swt/include/stereoview.h InputDeviceApp.h \
 TransformMode.h OutputApp.h RouteViewApp.h StartWithApp.h \
 ExportNamesApp.h ModellingApp.h RenderApp.h RenderState.h ProtoMap.h
URLDialog.o: URLDialog.cpp stdafx.h config.h mysnprintf.h resource.h \
 MyString.h StringArray.h Array.h Path.h FieldValue.h Element.h \
 x3dFlags.h swt/include/WriteFlags.h Arrays.h MySwap.h Types.h NodeList.h \
 NodeArray.h Node.h List.h Vec3f.h Vec3x.h FieldIndex.h NodeFlags.h \
 MeshFlags.h ac3dMaterialCallback.h Stack.h Matrix.h Vec3d.h Quaternion.h \
 URLDialog.h Dialog.h swt/include/swttypedef.h Scene.h Map.h \
 CommandList.h Command.h URL.h Proto.h ProtoMacros.h InterfaceArray.h \
 DynamicFieldsNode.h SFMFTypes.h MFBool.h MFColor.h MFFloat.h SFFloat.h \
 MFColorRGBA.h MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h \
 MFNode.h MFRotation.h MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h \
 MFVec2f.h MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h \
 SFColorRGBA.h SFImage.h SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h \
 SFRotation.h SFString.h SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h \
 SFVec3f.h SFVec4f.h DuneApp.h PreferencesApp.h EcmaScriptApp.h \
 StereoViewApp.h swt/include/stereoview.h InputDeviceApp.h \
 TransformMode.h OutputApp.h RouteViewApp.h StartWithApp.h \
 ExportNamesApp.h ModellingApp.h RenderApp.h RenderState.h ProtoMap.h \
 swt/include/swt.h swt/include/keycodes.h swt/include/swlanguages.h \
 swt/include/swDebugf.h swt/include/swttypedef.h
ArrayDialog.o: ArrayDialog.cpp stdafx.h config.h mysnprintf.h \
 ArrayDialog.h Dialog.h swt/include/swttypedef.h SFVec3f.h SfVec3x.h \
 FieldValue.h Array.h MyString.h StringArray.h Element.h x3dFlags.h \
 swt/include/WriteFlags.h Arrays.h MySwap.h Types.h Vec3f.h Vec3x.h \
 Quaternion.h Matrix.h Vec3d.h SFRotation.h swt/include/swt.h \
 swt/include/keycodes.h swt/include/swlanguages.h swt/include/swDebugf.h \
 swt/include/swttypedef.h resource.h DuneApp.h List.h Node.h FieldIndex.h \
 NodeFlags.h MeshFlags.h ac3dMaterialCallback.h NodeArray.h Stack.h URL.h \
 PreferencesApp.h EcmaScriptApp.h StereoViewApp.h \
 swt/include/stereoview.h InputDeviceApp.h TransformMode.h OutputApp.h \
 RouteViewApp.h StartWithApp.h ExportNamesApp.h ModellingApp.h \
 RenderApp.h NodeCone.h MeshBasedNode.h GeometryNode.h NodeAppearance.h \
 ProtoMacros.h Proto.h Map.h NodeList.h InterfaceArray.h \
 DynamicFieldsNode.h SFMFTypes.h MFBool.h MFColor.h MFFloat.h SFFloat.h \
 MFColorRGBA.h MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h \
 MFNode.h MFRotation.h MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h \
 MFVec2f.h MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h \
 SFColorRGBA.h SFImage.h SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h \
 SFString.h SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SFVec4f.h CommandList.h \
 Command.h NodeMaterial.h NodeImageTexture.h KambiTextureCommonFields.h \
 NodeTextureTransform.h GeometryNodeMacros.h
FieldPipeDialog.o: FieldPipeDialog.cpp stdafx.h config.h mysnprintf.h \
 FieldPipeDialog.h Dialog.h swt/include/swttypedef.h MyString.h \
 StringArray.h Array.h swt/include/swt.h swt/include/keycodes.h \
 swt/include/swlanguages.h swt/include/swDebugf.h \
 swt/include/swttypedef.h resource.h
OneBoolDialog.o: OneBoolDialog.cpp stdafx.h config.h mysnprintf.h \
 OneBoolDialog.h Dialog.h swt/include/swttypedef.h swt/include/swt.h \
 swt/include/keycodes.h swt/include/swlanguages.h swt/include/swDebugf.h \
 swt/include/swttypedef.h resource.h
OneFloatDialog.o: OneFloatDialog.cpp stdafx.h config.h mysnprintf.h \
 OneFloatDialog.h Dialog.h swt/include/swttypedef.h swt/include/swt.h \
 swt/include/keycodes.h swt/include/swlanguages.h swt/include/swDebugf.h \
 swt/include/swttypedef.h resource.h
OneIntDialog.o: OneIntDialog.cpp stdafx.h config.h mysnprintf.h \
 OneIntDialog.h Dialog.h swt/include/swttypedef.h swt/include/swt.h \
 swt/include/keycodes.h swt/include/swlanguages.h swt/include/swDebugf.h \
 swt/include/swttypedef.h resource.h
OneTextDialog.o: OneTextDialog.cpp stdafx.h config.h mysnprintf.h \
 OneTextDialog.h Dialog.h swt/include/swttypedef.h MyString.h \
 StringArray.h Array.h swt/include/swt.h swt/include/keycodes.h \
 swt/include/swlanguages.h swt/include/swDebugf.h \
 swt/include/swttypedef.h resource.h
TwoRadioButtonsDialog.o: TwoRadioButtonsDialog.cpp stdafx.h config.h \
 mysnprintf.h TwoRadioButtonsDialog.h Dialog.h swt/include/swttypedef.h \
 swt/include/swt.h swt/include/keycodes.h swt/include/swlanguages.h \
 swt/include/swDebugf.h swt/include/swttypedef.h resource.h
EcmaScriptSettingsDialog.o: EcmaScriptSettingsDialog.cpp stdafx.h \
 config.h mysnprintf.h EcmaScriptSettingsDialog.h Dialog.h \
 swt/include/swttypedef.h MyString.h StringArray.h Array.h DuneApp.h \
 List.h Node.h Vec3f.h Vec3x.h FieldIndex.h Element.h x3dFlags.h \
 swt/include/WriteFlags.h NodeFlags.h MeshFlags.h ac3dMaterialCallback.h \
 NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h URL.h PreferencesApp.h \
 EcmaScriptApp.h StereoViewApp.h swt/include/stereoview.h \
 InputDeviceApp.h TransformMode.h OutputApp.h RouteViewApp.h \
 StartWithApp.h ExportNamesApp.h ModellingApp.h RenderApp.h resource.h \
 swt/include/swt.h swt/include/keycodes.h swt/include/swlanguages.h \
 swt/include/swDebugf.h swt/include/swttypedef.h
PreviewSettingsDialog.o: PreviewSettingsDialog.cpp stdafx.h config.h \
 mysnprintf.h PreviewSettingsDialog.h Dialog.h swt/include/swttypedef.h \
 MyString.h StringArray.h Array.h DuneApp.h List.h Node.h Vec3f.h Vec3x.h \
 FieldIndex.h Element.h x3dFlags.h swt/include/WriteFlags.h NodeFlags.h \
 MeshFlags.h ac3dMaterialCallback.h NodeArray.h Stack.h Matrix.h Vec3d.h \
 Quaternion.h URL.h PreferencesApp.h EcmaScriptApp.h StereoViewApp.h \
 swt/include/stereoview.h InputDeviceApp.h TransformMode.h OutputApp.h \
 RouteViewApp.h StartWithApp.h ExportNamesApp.h ModellingApp.h \
 RenderApp.h resource.h swt/include/swt.h swt/include/keycodes.h \
 swt/include/swlanguages.h swt/include/swDebugf.h \
 swt/include/swttypedef.h
UploadSettingsDialog.o: UploadSettingsDialog.cpp stdafx.h config.h \
 mysnprintf.h UploadSettingsDialog.h Dialog.h swt/include/swttypedef.h \
 MyString.h StringArray.h Array.h DuneApp.h List.h Node.h Vec3f.h Vec3x.h \
 FieldIndex.h Element.h x3dFlags.h swt/include/WriteFlags.h NodeFlags.h \
 MeshFlags.h ac3dMaterialCallback.h NodeArray.h Stack.h Matrix.h Vec3d.h \
 Quaternion.h URL.h PreferencesApp.h EcmaScriptApp.h StereoViewApp.h \
 swt/include/stereoview.h InputDeviceApp.h TransformMode.h OutputApp.h \
 RouteViewApp.h StartWithApp.h ExportNamesApp.h ModellingApp.h \
 RenderApp.h resource.h swt/include/swt.h swt/include/keycodes.h \
 swt/include/swlanguages.h swt/include/swDebugf.h \
 swt/include/swttypedef.h
HelpSettingsDialog.o: HelpSettingsDialog.cpp stdafx.h config.h \
 mysnprintf.h HelpSettingsDialog.h Dialog.h swt/include/swttypedef.h \
 MyString.h StringArray.h Array.h DuneApp.h List.h Node.h Vec3f.h Vec3x.h \
 FieldIndex.h Element.h x3dFlags.h swt/include/WriteFlags.h NodeFlags.h \
 MeshFlags.h ac3dMaterialCallback.h NodeArray.h Stack.h Matrix.h Vec3d.h \
 Quaternion.h URL.h PreferencesApp.h EcmaScriptApp.h StereoViewApp.h \
 swt/include/stereoview.h InputDeviceApp.h TransformMode.h OutputApp.h \
 RouteViewApp.h StartWithApp.h ExportNamesApp.h ModellingApp.h \
 RenderApp.h resource.h swt/include/swt.h swt/include/keycodes.h \
 swt/include/swlanguages.h swt/include/swDebugf.h \
 swt/include/swttypedef.h
TexteditSettingsDialog.o: TexteditSettingsDialog.cpp stdafx.h config.h \
 mysnprintf.h TexteditSettingsDialog.h Dialog.h swt/include/swttypedef.h \
 MyString.h StringArray.h Array.h DuneApp.h List.h Node.h Vec3f.h Vec3x.h \
 FieldIndex.h Element.h x3dFlags.h swt/include/WriteFlags.h NodeFlags.h \
 MeshFlags.h ac3dMaterialCallback.h NodeArray.h Stack.h Matrix.h Vec3d.h \
 Quaternion.h URL.h PreferencesApp.h EcmaScriptApp.h StereoViewApp.h \
 swt/include/stereoview.h InputDeviceApp.h TransformMode.h OutputApp.h \
 RouteViewApp.h StartWithApp.h ExportNamesApp.h ModellingApp.h \
 RenderApp.h resource.h swt/include/swt.h swt/include/keycodes.h \
 swt/include/swlanguages.h swt/include/swDebugf.h \
 swt/include/swttypedef.h
StereoViewSettingsDialog.o: StereoViewSettingsDialog.cpp stdafx.h \
 config.h mysnprintf.h StereoViewSettingsDialog.h Dialog.h \
 swt/include/swttypedef.h DuneApp.h List.h Array.h MyString.h \
 StringArray.h Node.h Vec3f.h Vec3x.h FieldIndex.h Element.h x3dFlags.h \
 swt/include/WriteFlags.h NodeFlags.h MeshFlags.h ac3dMaterialCallback.h \
 NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h URL.h PreferencesApp.h \
 EcmaScriptApp.h StereoViewApp.h swt/include/stereoview.h \
 InputDeviceApp.h TransformMode.h OutputApp.h RouteViewApp.h \
 StartWithApp.h ExportNamesApp.h ModellingApp.h RenderApp.h resource.h \
 swt/include/swt.h swt/include/keycodes.h swt/include/swlanguages.h \
 swt/include/swDebugf.h swt/include/swttypedef.h
OutputSettingsDialog.o: OutputSettingsDialog.cpp stdafx.h config.h \
 mysnprintf.h OutputSettingsDialog.h Dialog.h swt/include/swttypedef.h \
 DuneApp.h List.h Array.h MyString.h StringArray.h Node.h Vec3f.h Vec3x.h \
 FieldIndex.h Element.h x3dFlags.h swt/include/WriteFlags.h NodeFlags.h \
 MeshFlags.h ac3dMaterialCallback.h NodeArray.h Stack.h Matrix.h Vec3d.h \
 Quaternion.h URL.h PreferencesApp.h EcmaScriptApp.h StereoViewApp.h \
 swt/include/stereoview.h InputDeviceApp.h TransformMode.h OutputApp.h \
 RouteViewApp.h StartWithApp.h ExportNamesApp.h ModellingApp.h \
 RenderApp.h resource.h swt/include/swt.h swt/include/keycodes.h \
 swt/include/swlanguages.h swt/include/swDebugf.h \
 swt/include/swttypedef.h
InputDeviceSelectionDialog.o: InputDeviceSelectionDialog.cpp DuneApp.h \
 List.h Array.h stdafx.h config.h mysnprintf.h MyString.h StringArray.h \
 Node.h Vec3f.h Vec3x.h FieldIndex.h Element.h x3dFlags.h \
 swt/include/WriteFlags.h NodeFlags.h MeshFlags.h ac3dMaterialCallback.h \
 NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h URL.h PreferencesApp.h \
 swt/include/swttypedef.h EcmaScriptApp.h StereoViewApp.h \
 swt/include/stereoview.h InputDeviceApp.h TransformMode.h OutputApp.h \
 RouteViewApp.h StartWithApp.h ExportNamesApp.h ModellingApp.h \
 RenderApp.h InputDeviceSelectionDialog.h Dialog.h InputDevice.h \
 swt/include/swt.h swt/include/keycodes.h swt/include/swlanguages.h \
 swt/include/swDebugf.h swt/include/swttypedef.h EulerAngles.h \
 QuatTypes.h Map.h swt/include/swtxinput.h Aflock.h \
 AflockSettingsDialog.h resource.h
InputDeviceNameSelectionDialog.o: InputDeviceNameSelectionDialog.cpp \
 DuneApp.h List.h Array.h stdafx.h config.h mysnprintf.h MyString.h \
 StringArray.h Node.h Vec3f.h Vec3x.h FieldIndex.h Element.h x3dFlags.h \
 swt/include/WriteFlags.h NodeFlags.h MeshFlags.h ac3dMaterialCallback.h \
 NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h URL.h PreferencesApp.h \
 swt/include/swttypedef.h EcmaScriptApp.h StereoViewApp.h \
 swt/include/stereoview.h InputDeviceApp.h TransformMode.h OutputApp.h \
 RouteViewApp.h StartWithApp.h ExportNamesApp.h ModellingApp.h \
 RenderApp.h InputDeviceNameSelectionDialog.h Dialog.h InputDevice.h \
 swt/include/swt.h swt/include/keycodes.h swt/include/swlanguages.h \
 swt/include/swDebugf.h swt/include/swttypedef.h EulerAngles.h \
 QuatTypes.h Map.h swt/include/swtxinput.h Aflock.h resource.h
InputDeviceSettingsDialog.o: InputDeviceSettingsDialog.cpp DuneApp.h \
 List.h Array.h stdafx.h config.h mysnprintf.h MyString.h StringArray.h \
 Node.h Vec3f.h Vec3x.h FieldIndex.h Element.h x3dFlags.h \
 swt/include/WriteFlags.h NodeFlags.h MeshFlags.h ac3dMaterialCallback.h \
 NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h URL.h PreferencesApp.h \
 swt/include/swttypedef.h EcmaScriptApp.h StereoViewApp.h \
 swt/include/stereoview.h InputDeviceApp.h TransformMode.h OutputApp.h \
 RouteViewApp.h StartWithApp.h ExportNamesApp.h ModellingApp.h \
 RenderApp.h InputDeviceSettingsDialog.h Dialog.h AflockSettingsDialog.h \
 InputDevice.h swt/include/swt.h swt/include/keycodes.h \
 swt/include/swlanguages.h swt/include/swDebugf.h \
 swt/include/swttypedef.h EulerAngles.h QuatTypes.h Map.h \
 swt/include/swtxinput.h Aflock.h resource.h
AflockSettingsDialog.o: AflockSettingsDialog.cpp DuneApp.h List.h Array.h \
 stdafx.h config.h mysnprintf.h MyString.h StringArray.h Node.h Vec3f.h \
 Vec3x.h FieldIndex.h Element.h x3dFlags.h swt/include/WriteFlags.h \
 NodeFlags.h MeshFlags.h ac3dMaterialCallback.h NodeArray.h Stack.h \
 Matrix.h Vec3d.h Quaternion.h URL.h PreferencesApp.h \
 swt/include/swttypedef.h EcmaScriptApp.h StereoViewApp.h \
 swt/include/stereoview.h InputDeviceApp.h TransformMode.h OutputApp.h \
 RouteViewApp.h StartWithApp.h ExportNamesApp.h ModellingApp.h \
 RenderApp.h swt/include/swt.h swt/include/keycodes.h \
 swt/include/swlanguages.h swt/include/swDebugf.h \
 swt/include/swttypedef.h resource.h InputDevice.h EulerAngles.h \
 QuatTypes.h Map.h swt/include/swtxinput.h Aflock.h \
 AflockSettingsDialog.h Dialog.h
RouteViewSettingsDialog.o: RouteViewSettingsDialog.cpp stdafx.h config.h \
 mysnprintf.h RouteViewSettingsDialog.h Dialog.h swt/include/swttypedef.h \
 DuneApp.h List.h Array.h MyString.h StringArray.h Node.h Vec3f.h Vec3x.h \
 FieldIndex.h Element.h x3dFlags.h swt/include/WriteFlags.h NodeFlags.h \
 MeshFlags.h ac3dMaterialCallback.h NodeArray.h Stack.h Matrix.h Vec3d.h \
 Quaternion.h URL.h PreferencesApp.h EcmaScriptApp.h StereoViewApp.h \
 swt/include/stereoview.h InputDeviceApp.h TransformMode.h OutputApp.h \
 RouteViewApp.h StartWithApp.h ExportNamesApp.h ModellingApp.h \
 RenderApp.h resource.h swt/include/swt.h swt/include/keycodes.h \
 swt/include/swlanguages.h swt/include/swDebugf.h \
 swt/include/swttypedef.h
WonderlandModuleExportDialog.o: WonderlandModuleExportDialog.cpp stdafx.h \
 config.h mysnprintf.h resource.h swt/include/swt.h \
 swt/include/keycodes.h swt/include/swlanguages.h swt/include/swDebugf.h \
 swt/include/swttypedef.h WonderlandModuleExportDialog.h Dialog.h \
 swt/include/swttypedef.h DuneApp.h List.h Array.h MyString.h \
 StringArray.h Node.h Vec3f.h Vec3x.h FieldIndex.h Element.h x3dFlags.h \
 swt/include/WriteFlags.h NodeFlags.h MeshFlags.h ac3dMaterialCallback.h \
 NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h URL.h PreferencesApp.h \
 EcmaScriptApp.h StereoViewApp.h swt/include/stereoview.h \
 InputDeviceApp.h TransformMode.h OutputApp.h RouteViewApp.h \
 StartWithApp.h ExportNamesApp.h ModellingApp.h RenderApp.h Scene.h Map.h \
 CommandList.h Command.h NodeList.h Proto.h ProtoMacros.h \
 InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h \
 Arrays.h MySwap.h Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h \
 MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h \
 MFRotation.h MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h \
 MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h \
 SFImage.h SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h \
 SFString.h SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h \
 SFVec4f.h RenderState.h ProtoMap.h
ProtoNameDialog.o: ProtoNameDialog.cpp stdafx.h config.h mysnprintf.h \
 ProtoNameDialog.h Dialog.h swt/include/swttypedef.h Vec3f.h Vec3x.h \
 MyString.h StringArray.h Array.h CheckBoxWindow.h swt/include/swt.h \
 swt/include/keycodes.h swt/include/swlanguages.h swt/include/swDebugf.h \
 swt/include/swttypedef.h resource.h DuneApp.h List.h Node.h FieldIndex.h \
 Element.h x3dFlags.h swt/include/WriteFlags.h NodeFlags.h MeshFlags.h \
 ac3dMaterialCallback.h NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h \
 URL.h PreferencesApp.h EcmaScriptApp.h StereoViewApp.h \
 swt/include/stereoview.h InputDeviceApp.h TransformMode.h OutputApp.h \
 RouteViewApp.h StartWithApp.h ExportNamesApp.h ModellingApp.h \
 RenderApp.h NodeBox.h MeshBasedNode.h GeometryNode.h NodeAppearance.h \
 ProtoMacros.h Proto.h Map.h NodeList.h InterfaceArray.h \
 DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h Arrays.h MySwap.h \
 Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h MFDouble.h \
 SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h MFRotation.h \
 MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h \
 MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h SFImage.h \
 SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h SFString.h \
 SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h \
 CommandList.h Command.h NodeMaterial.h NodeImageTexture.h \
 KambiTextureCommonFields.h NodeTextureTransform.h GeometryNodeMacros.h
ScriptEventDialog.o: ScriptEventDialog.cpp stdafx.h config.h mysnprintf.h \
 ScriptEventDialog.h Dialog.h swt/include/swttypedef.h MyString.h \
 StringArray.h Array.h Node.h List.h Vec3f.h Vec3x.h FieldIndex.h \
 Element.h x3dFlags.h swt/include/WriteFlags.h NodeFlags.h MeshFlags.h \
 ac3dMaterialCallback.h NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h \
 resource.h swt/include/swt.h swt/include/keycodes.h \
 swt/include/swlanguages.h swt/include/swDebugf.h \
 swt/include/swttypedef.h Proto.h Map.h ProtoMacros.h NodeList.h \
 InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h \
 Arrays.h MySwap.h Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h \
 MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h \
 MFRotation.h MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h \
 MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h \
 SFImage.h SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h \
 SFString.h SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h \
 SFVec4f.h CommandList.h Command.h DuneApp.h URL.h PreferencesApp.h \
 EcmaScriptApp.h StereoViewApp.h swt/include/stereoview.h \
 InputDeviceApp.h TransformMode.h OutputApp.h RouteViewApp.h \
 StartWithApp.h ExportNamesApp.h ModellingApp.h RenderApp.h
CattExportDialog.o: CattExportDialog.cpp stdafx.h config.h mysnprintf.h \
 CattExportDialog.h Dialog.h swt/include/swttypedef.h DuneApp.h List.h \
 Array.h MyString.h StringArray.h Node.h Vec3f.h Vec3x.h FieldIndex.h \
 Element.h x3dFlags.h swt/include/WriteFlags.h NodeFlags.h MeshFlags.h \
 ac3dMaterialCallback.h NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h \
 URL.h PreferencesApp.h EcmaScriptApp.h StereoViewApp.h \
 swt/include/stereoview.h InputDeviceApp.h TransformMode.h OutputApp.h \
 RouteViewApp.h StartWithApp.h ExportNamesApp.h ModellingApp.h \
 RenderApp.h resource.h swt/include/swt.h swt/include/keycodes.h \
 swt/include/swlanguages.h swt/include/swDebugf.h \
 swt/include/swttypedef.h
ProtoDialog.o: ProtoDialog.cpp stdafx.h config.h mysnprintf.h \
 ProtoDialog.h Dialog.h swt/include/swttypedef.h NodeScript.h Node.h \
 Array.h List.h Vec3f.h Vec3x.h MyString.h StringArray.h FieldIndex.h \
 Element.h x3dFlags.h swt/include/WriteFlags.h NodeFlags.h MeshFlags.h \
 ac3dMaterialCallback.h NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h \
 ProtoMacros.h Proto.h Map.h NodeList.h InterfaceArray.h \
 DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h Arrays.h MySwap.h \
 Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h MFDouble.h \
 SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h MFRotation.h \
 MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h \
 MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h SFImage.h \
 SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h SFString.h \
 SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h \
 CommandList.h Command.h Scene.h URL.h DuneApp.h PreferencesApp.h \
 EcmaScriptApp.h StereoViewApp.h swt/include/stereoview.h \
 InputDeviceApp.h TransformMode.h OutputApp.h RouteViewApp.h \
 StartWithApp.h ExportNamesApp.h ModellingApp.h RenderApp.h RenderState.h \
 ProtoMap.h swt/include/swt.h swt/include/keycodes.h \
 swt/include/swlanguages.h swt/include/swDebugf.h \
 swt/include/swttypedef.h ScriptEdit.h NodeImageTexture.h \
 KambiTextureCommonFields.h NodePixelTexture.h NodeMovieTexture.h \
 NodeAudioClip.h resource.h Field.h EventIn.h EventOut.h ExposedField.h
SplitFacesDialog.o: SplitFacesDialog.cpp stdafx.h config.h mysnprintf.h \
 SplitFacesDialog.h Dialog.h swt/include/swttypedef.h swt/include/swt.h \
 swt/include/keycodes.h swt/include/swlanguages.h swt/include/swDebugf.h \
 swt/include/swttypedef.h resource.h DuneApp.h List.h Array.h MyString.h \
 StringArray.h Node.h Vec3f.h Vec3x.h FieldIndex.h Element.h x3dFlags.h \
 swt/include/WriteFlags.h NodeFlags.h MeshFlags.h ac3dMaterialCallback.h \
 NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h URL.h PreferencesApp.h \
 EcmaScriptApp.h StereoViewApp.h swt/include/stereoview.h \
 InputDeviceApp.h TransformMode.h OutputApp.h RouteViewApp.h \
 StartWithApp.h ExportNamesApp.h ModellingApp.h RenderApp.h
ToIndexedFaceSetDialog.o: ToIndexedFaceSetDialog.cpp stdafx.h config.h \
 mysnprintf.h ToIndexedFaceSetDialog.h Dialog.h swt/include/swttypedef.h \
 swt/include/swt.h swt/include/keycodes.h swt/include/swlanguages.h \
 swt/include/swDebugf.h swt/include/swttypedef.h resource.h
HAnimJointDialog.o: HAnimJointDialog.cpp stdafx.h config.h mysnprintf.h \
 HAnimJointDialog.h Dialog.h swt/include/swttypedef.h Array.h \
 swt/include/swt.h swt/include/keycodes.h swt/include/swlanguages.h \
 swt/include/swDebugf.h swt/include/swttypedef.h resource.h DuneApp.h \
 List.h MyString.h StringArray.h Node.h Vec3f.h Vec3x.h FieldIndex.h \
 Element.h x3dFlags.h swt/include/WriteFlags.h NodeFlags.h MeshFlags.h \
 ac3dMaterialCallback.h NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h \
 URL.h PreferencesApp.h EcmaScriptApp.h StereoViewApp.h \
 swt/include/stereoview.h InputDeviceApp.h TransformMode.h OutputApp.h \
 RouteViewApp.h StartWithApp.h ExportNamesApp.h ModellingApp.h \
 RenderApp.h NodeList.h Scene.h Map.h CommandList.h Command.h Proto.h \
 ProtoMacros.h InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h MFBool.h \
 FieldValue.h Arrays.h MySwap.h Types.h MFColor.h MFFloat.h SFFloat.h \
 MFColorRGBA.h MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h \
 MFNode.h MFRotation.h MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h \
 MFVec2f.h MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h \
 SFColorRGBA.h SFImage.h SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h \
 SFRotation.h SFString.h SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h \
 SFVec3f.h SFVec4f.h RenderState.h ProtoMap.h Path.h NodeHAnimHumanoid.h \
 MyMesh.h NodeIndexedFaceSet.h MeshBasedNode.h GeometryNode.h \
 NodeAppearance.h NodeMaterial.h NodeImageTexture.h \
 KambiTextureCommonFields.h NodeTextureTransform.h GeometryNodeMacros.h \
 NodeCoordinate.h Colored.h NodeColor.h NodeColorRGBA.h \
 ComposedGeometryMacros.h FaceData.h NodeNormal.h NodeTextureCoordinate.h \
 NodeTextureCoordinateGenerator.h NodeMultiTextureCoordinate.h Util.h \
 TransformNode.h BasicTransformNode.h
ViewpointDialog.o: ViewpointDialog.cpp stdafx.h config.h mysnprintf.h \
 ViewpointDialog.h Dialog.h swt/include/swttypedef.h swt/include/swt.h \
 swt/include/keycodes.h swt/include/swlanguages.h swt/include/swDebugf.h \
 swt/include/swttypedef.h resource.h Scene.h Array.h Map.h List.h Stack.h \
 CommandList.h Command.h NodeList.h NodeArray.h Node.h Vec3f.h Vec3x.h \
 MyString.h StringArray.h FieldIndex.h Element.h x3dFlags.h \
 swt/include/WriteFlags.h NodeFlags.h MeshFlags.h ac3dMaterialCallback.h \
 Matrix.h Vec3d.h Quaternion.h URL.h Proto.h ProtoMacros.h \
 InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h \
 Arrays.h MySwap.h Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h \
 MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h \
 MFRotation.h MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h \
 MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h \
 SFImage.h SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h \
 SFString.h SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h \
 SFVec4f.h DuneApp.h PreferencesApp.h EcmaScriptApp.h StereoViewApp.h \
 swt/include/stereoview.h InputDeviceApp.h TransformMode.h OutputApp.h \
 RouteViewApp.h StartWithApp.h ExportNamesApp.h ModellingApp.h \
 RenderApp.h RenderState.h ProtoMap.h NodeViewpoint.h ViewpointNode.h \
 CoverNode.h KambiViewpointCommonFields.h
NurbsCurve2DDialog.o: NurbsCurve2DDialog.cpp stdafx.h config.h \
 mysnprintf.h NurbsCurve2DDialog.h Dialog.h swt/include/swttypedef.h \
 swt/include/swt.h swt/include/keycodes.h swt/include/swlanguages.h \
 swt/include/swDebugf.h swt/include/swttypedef.h resource.h Scene.h \
 Array.h Map.h List.h Stack.h CommandList.h Command.h NodeList.h \
 NodeArray.h Node.h Vec3f.h Vec3x.h MyString.h StringArray.h FieldIndex.h \
 Element.h x3dFlags.h swt/include/WriteFlags.h NodeFlags.h MeshFlags.h \
 ac3dMaterialCallback.h Matrix.h Vec3d.h Quaternion.h URL.h Proto.h \
 ProtoMacros.h InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h MFBool.h \
 FieldValue.h Arrays.h MySwap.h Types.h MFColor.h MFFloat.h SFFloat.h \
 MFColorRGBA.h MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h \
 MFNode.h MFRotation.h MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h \
 MFVec2f.h MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h \
 SFColorRGBA.h SFImage.h SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h \
 SFRotation.h SFString.h SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h \
 SFVec3f.h SFVec4f.h DuneApp.h PreferencesApp.h EcmaScriptApp.h \
 StereoViewApp.h swt/include/stereoview.h InputDeviceApp.h \
 TransformMode.h OutputApp.h RouteViewApp.h StartWithApp.h \
 ExportNamesApp.h ModellingApp.h RenderApp.h RenderState.h ProtoMap.h \
 NodeViewpoint.h ViewpointNode.h CoverNode.h KambiViewpointCommonFields.h
InsertToNurbsCurveDialog.o: InsertToNurbsCurveDialog.cpp stdafx.h \
 config.h mysnprintf.h InsertToNurbsCurveDialog.h Dialog.h \
 swt/include/swttypedef.h swt/include/swt.h swt/include/keycodes.h \
 swt/include/swlanguages.h swt/include/swDebugf.h \
 swt/include/swttypedef.h resource.h DuneApp.h List.h Array.h MyString.h \
 StringArray.h Node.h Vec3f.h Vec3x.h FieldIndex.h Element.h x3dFlags.h \
 swt/include/WriteFlags.h NodeFlags.h MeshFlags.h ac3dMaterialCallback.h \
 NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h URL.h PreferencesApp.h \
 EcmaScriptApp.h StereoViewApp.h swt/include/stereoview.h \
 InputDeviceApp.h TransformMode.h OutputApp.h RouteViewApp.h \
 StartWithApp.h ExportNamesApp.h ModellingApp.h RenderApp.h
InsertToNurbsSurfaceDialog.o: InsertToNurbsSurfaceDialog.cpp stdafx.h \
 config.h mysnprintf.h InsertToNurbsSurfaceDialog.h Dialog.h \
 swt/include/swttypedef.h swt/include/swt.h swt/include/keycodes.h \
 swt/include/swlanguages.h swt/include/swDebugf.h \
 swt/include/swttypedef.h resource.h DuneApp.h List.h Array.h MyString.h \
 StringArray.h Node.h Vec3f.h Vec3x.h FieldIndex.h Element.h x3dFlags.h \
 swt/include/WriteFlags.h NodeFlags.h MeshFlags.h ac3dMaterialCallback.h \
 NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h URL.h PreferencesApp.h \
 EcmaScriptApp.h StereoViewApp.h swt/include/stereoview.h \
 InputDeviceApp.h TransformMode.h OutputApp.h RouteViewApp.h \
 StartWithApp.h ExportNamesApp.h ModellingApp.h RenderApp.h
SliderFloatDialog.o: SliderFloatDialog.cpp stdafx.h config.h mysnprintf.h \
 SliderFloatDialog.h Dialog.h swt/include/swttypedef.h swt/include/swt.h \
 swt/include/keycodes.h swt/include/swlanguages.h swt/include/swDebugf.h \
 swt/include/swttypedef.h resource.h
SceneDialog.o: SceneDialog.cpp SceneDialog.h Dialog.h \
 swt/include/swttypedef.h Scene.h Array.h stdafx.h config.h mysnprintf.h \
 Map.h List.h Stack.h CommandList.h Command.h NodeList.h NodeArray.h \
 Node.h Vec3f.h Vec3x.h MyString.h StringArray.h FieldIndex.h Element.h \
 x3dFlags.h swt/include/WriteFlags.h NodeFlags.h MeshFlags.h \
 ac3dMaterialCallback.h Matrix.h Vec3d.h Quaternion.h URL.h Proto.h \
 ProtoMacros.h InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h MFBool.h \
 FieldValue.h Arrays.h MySwap.h Types.h MFColor.h MFFloat.h SFFloat.h \
 MFColorRGBA.h MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h \
 MFNode.h MFRotation.h MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h \
 MFVec2f.h MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h \
 SFColorRGBA.h SFImage.h SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h \
 SFRotation.h SFString.h SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h \
 SFVec3f.h SFVec4f.h DuneApp.h PreferencesApp.h EcmaScriptApp.h \
 StereoViewApp.h swt/include/stereoview.h InputDeviceApp.h \
 TransformMode.h OutputApp.h RouteViewApp.h StartWithApp.h \
 ExportNamesApp.h ModellingApp.h RenderApp.h RenderState.h ProtoMap.h \
 NodeVrmlCut.h resource.h swt/include/swt.h swt/include/keycodes.h \
 swt/include/swlanguages.h swt/include/swDebugf.h \
 swt/include/swttypedef.h NodeVrmlScene.h
DuneApp.o: DuneApp.cpp stdafx.h config.h mysnprintf.h swt/include/swt.h \
 swt/include/keycodes.h swt/include/swlanguages.h swt/include/swDebugf.h \
 swt/include/swttypedef.h parser.h DuneApp.h List.h Array.h MyString.h \
 StringArray.h Node.h Vec3f.h Vec3x.h FieldIndex.h Element.h x3dFlags.h \
 swt/include/WriteFlags.h NodeFlags.h MeshFlags.h ac3dMaterialCallback.h \
 NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h URL.h PreferencesApp.h \
 swt/include/swttypedef.h EcmaScriptApp.h StereoViewApp.h \
 swt/include/stereoview.h InputDeviceApp.h TransformMode.h OutputApp.h \
 RouteViewApp.h StartWithApp.h ExportNamesApp.h ModellingApp.h \
 RenderApp.h Scene.h Map.h CommandList.h Command.h NodeList.h Proto.h \
 ProtoMacros.h InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h MFBool.h \
 FieldValue.h Arrays.h MySwap.h Types.h MFColor.h MFFloat.h SFFloat.h \
 MFColorRGBA.h MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h \
 MFNode.h MFRotation.h MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h \
 MFVec2f.h MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h \
 SFColorRGBA.h SFImage.h SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h \
 SFRotation.h SFString.h SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h \
 SFVec3f.h SFVec4f.h RenderState.h ProtoMap.h Path.h MainWindow.h \
 PanedWindow.h SceneView.h ColorConversion.h NodeNurbsCurve.h \
 NurbsCurve.h GeometryNode.h NodeAppearance.h NodeMaterial.h \
 NodeImageTexture.h KambiTextureCommonFields.h NodeTextureTransform.h \
 GeometryNodeMacros.h ChainBasedNode.h EulerAngles.h QuatTypes.h \
 resource.h TexteditSettingsDialog.h Dialog.h xerrorhandler.h
PreferencesApp.o: PreferencesApp.cpp stdafx.h config.h mysnprintf.h \
 swt/include/swt.h swt/include/keycodes.h swt/include/swlanguages.h \
 swt/include/swDebugf.h swt/include/swttypedef.h resource.h \
 PreferencesApp.h swt/include/swttypedef.h MyString.h StringArray.h \
 Array.h MainWindow.h PanedWindow.h SceneView.h DuneApp.h List.h Node.h \
 Vec3f.h Vec3x.h FieldIndex.h Element.h x3dFlags.h \
 swt/include/WriteFlags.h NodeFlags.h MeshFlags.h ac3dMaterialCallback.h \
 NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h URL.h EcmaScriptApp.h \
 StereoViewApp.h swt/include/stereoview.h InputDeviceApp.h \
 TransformMode.h OutputApp.h RouteViewApp.h StartWithApp.h \
 ExportNamesApp.h ModellingApp.h RenderApp.h Scene.h Map.h CommandList.h \
 Command.h NodeList.h Proto.h ProtoMacros.h InterfaceArray.h \
 DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h Arrays.h MySwap.h \
 Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h MFDouble.h \
 SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h MFRotation.h \
 MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h \
 MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h SFImage.h \
 SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h SFString.h \
 SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h \
 RenderState.h ProtoMap.h ColorConversion.h NodeNurbsCurve.h NurbsCurve.h \
 GeometryNode.h NodeAppearance.h NodeMaterial.h NodeImageTexture.h \
 KambiTextureCommonFields.h NodeTextureTransform.h GeometryNodeMacros.h \
 ChainBasedNode.h EulerAngles.h QuatTypes.h
EcmaScriptApp.o: EcmaScriptApp.cpp swt/include/swt.h \
 swt/include/keycodes.h swt/include/swlanguages.h swt/include/swDebugf.h \
 swt/include/swttypedef.h PreferencesApp.h swt/include/swttypedef.h \
 MyString.h mysnprintf.h config.h StringArray.h Array.h stdafx.h \
 EcmaScriptApp.h DuneApp.h List.h Node.h Vec3f.h Vec3x.h FieldIndex.h \
 Element.h x3dFlags.h swt/include/WriteFlags.h NodeFlags.h MeshFlags.h \
 ac3dMaterialCallback.h NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h \
 URL.h StereoViewApp.h swt/include/stereoview.h InputDeviceApp.h \
 TransformMode.h OutputApp.h RouteViewApp.h StartWithApp.h \
 ExportNamesApp.h ModellingApp.h RenderApp.h
StereoViewApp.o: StereoViewApp.cpp stdafx.h config.h mysnprintf.h \
 DuneApp.h List.h Array.h MyString.h StringArray.h Node.h Vec3f.h Vec3x.h \
 FieldIndex.h Element.h x3dFlags.h swt/include/WriteFlags.h NodeFlags.h \
 MeshFlags.h ac3dMaterialCallback.h NodeArray.h Stack.h Matrix.h Vec3d.h \
 Quaternion.h URL.h PreferencesApp.h swt/include/swttypedef.h \
 EcmaScriptApp.h StereoViewApp.h swt/include/stereoview.h \
 InputDeviceApp.h TransformMode.h OutputApp.h RouteViewApp.h \
 StartWithApp.h ExportNamesApp.h ModellingApp.h RenderApp.h
InputDeviceApp.o: InputDeviceApp.cpp stdafx.h config.h mysnprintf.h \
 DuneApp.h List.h Array.h MyString.h StringArray.h Node.h Vec3f.h Vec3x.h \
 FieldIndex.h Element.h x3dFlags.h swt/include/WriteFlags.h NodeFlags.h \
 MeshFlags.h ac3dMaterialCallback.h NodeArray.h Stack.h Matrix.h Vec3d.h \
 Quaternion.h URL.h PreferencesApp.h swt/include/swttypedef.h \
 EcmaScriptApp.h StereoViewApp.h swt/include/stereoview.h \
 InputDeviceApp.h TransformMode.h OutputApp.h RouteViewApp.h \
 StartWithApp.h ExportNamesApp.h ModellingApp.h RenderApp.h InputDevice.h \
 swt/include/swt.h swt/include/keycodes.h swt/include/swlanguages.h \
 swt/include/swDebugf.h swt/include/swttypedef.h EulerAngles.h \
 QuatTypes.h Map.h swt/include/swtxinput.h Aflock.h
OutputApp.o: OutputApp.cpp stdafx.h config.h mysnprintf.h \
 swt/include/swt.h swt/include/keycodes.h swt/include/swlanguages.h \
 swt/include/swDebugf.h swt/include/swttypedef.h Scene.h Array.h Map.h \
 List.h Stack.h CommandList.h Command.h NodeList.h NodeArray.h Node.h \
 Vec3f.h Vec3x.h MyString.h StringArray.h FieldIndex.h Element.h \
 x3dFlags.h swt/include/WriteFlags.h NodeFlags.h MeshFlags.h \
 ac3dMaterialCallback.h Matrix.h Vec3d.h Quaternion.h URL.h Proto.h \
 ProtoMacros.h InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h MFBool.h \
 FieldValue.h Arrays.h MySwap.h Types.h MFColor.h MFFloat.h SFFloat.h \
 MFColorRGBA.h MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h \
 MFNode.h MFRotation.h MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h \
 MFVec2f.h MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h \
 SFColorRGBA.h SFImage.h SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h \
 SFRotation.h SFString.h SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h \
 SFVec3f.h SFVec4f.h DuneApp.h PreferencesApp.h swt/include/swttypedef.h \
 EcmaScriptApp.h StereoViewApp.h swt/include/stereoview.h \
 InputDeviceApp.h TransformMode.h OutputApp.h RouteViewApp.h \
 StartWithApp.h ExportNamesApp.h ModellingApp.h RenderApp.h RenderState.h \
 ProtoMap.h WonderlandModuleExport.h
RouteViewApp.o: RouteViewApp.cpp stdafx.h config.h mysnprintf.h DuneApp.h \
 List.h Array.h MyString.h StringArray.h Node.h Vec3f.h Vec3x.h \
 FieldIndex.h Element.h x3dFlags.h swt/include/WriteFlags.h NodeFlags.h \
 MeshFlags.h ac3dMaterialCallback.h NodeArray.h Stack.h Matrix.h Vec3d.h \
 Quaternion.h URL.h PreferencesApp.h swt/include/swttypedef.h \
 EcmaScriptApp.h StereoViewApp.h swt/include/stereoview.h \
 InputDeviceApp.h TransformMode.h OutputApp.h RouteViewApp.h \
 StartWithApp.h ExportNamesApp.h ModellingApp.h RenderApp.h
StartWithApp.o: StartWithApp.cpp stdafx.h config.h mysnprintf.h DuneApp.h \
 List.h Array.h MyString.h StringArray.h Node.h Vec3f.h Vec3x.h \
 FieldIndex.h Element.h x3dFlags.h swt/include/WriteFlags.h NodeFlags.h \
 MeshFlags.h ac3dMaterialCallback.h NodeArray.h Stack.h Matrix.h Vec3d.h \
 Quaternion.h URL.h PreferencesApp.h swt/include/swttypedef.h \
 EcmaScriptApp.h StereoViewApp.h swt/include/stereoview.h \
 InputDeviceApp.h TransformMode.h OutputApp.h RouteViewApp.h \
 StartWithApp.h ExportNamesApp.h ModellingApp.h RenderApp.h
ExportNamesApp.o: ExportNamesApp.cpp DuneApp.h List.h Array.h stdafx.h \
 config.h mysnprintf.h MyString.h StringArray.h Node.h Vec3f.h Vec3x.h \
 FieldIndex.h Element.h x3dFlags.h swt/include/WriteFlags.h NodeFlags.h \
 MeshFlags.h ac3dMaterialCallback.h NodeArray.h Stack.h Matrix.h Vec3d.h \
 Quaternion.h URL.h PreferencesApp.h swt/include/swttypedef.h \
 EcmaScriptApp.h StereoViewApp.h swt/include/stereoview.h \
 InputDeviceApp.h TransformMode.h OutputApp.h RouteViewApp.h \
 StartWithApp.h ExportNamesApp.h ModellingApp.h RenderApp.h
ModellingApp.o: ModellingApp.cpp stdafx.h config.h mysnprintf.h \
 swt/include/swt.h swt/include/keycodes.h swt/include/swlanguages.h \
 swt/include/swDebugf.h swt/include/swttypedef.h EulerAngles.h \
 QuatTypes.h Quaternion.h Vec3f.h Vec3x.h Matrix.h Vec3d.h ModellingApp.h \
 swt/include/swttypedef.h MyString.h StringArray.h Array.h \
 PreferencesApp.h DuneApp.h List.h Node.h FieldIndex.h Element.h \
 x3dFlags.h swt/include/WriteFlags.h NodeFlags.h MeshFlags.h \
 ac3dMaterialCallback.h NodeArray.h Stack.h URL.h EcmaScriptApp.h \
 StereoViewApp.h swt/include/stereoview.h InputDeviceApp.h \
 TransformMode.h OutputApp.h RouteViewApp.h StartWithApp.h \
 ExportNamesApp.h RenderApp.h
RenderApp.o: RenderApp.cpp stdafx.h config.h mysnprintf.h \
 swt/include/swt.h swt/include/keycodes.h swt/include/swlanguages.h \
 swt/include/swDebugf.h swt/include/swttypedef.h RenderApp.h \
 swt/include/swttypedef.h MyString.h StringArray.h Array.h DuneApp.h \
 List.h Node.h Vec3f.h Vec3x.h FieldIndex.h Element.h x3dFlags.h \
 swt/include/WriteFlags.h NodeFlags.h MeshFlags.h ac3dMaterialCallback.h \
 NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h URL.h PreferencesApp.h \
 EcmaScriptApp.h StereoViewApp.h swt/include/stereoview.h \
 InputDeviceApp.h TransformMode.h OutputApp.h RouteViewApp.h \
 StartWithApp.h ExportNamesApp.h ModellingApp.h
Util.o: Util.cpp stdafx.h config.h mysnprintf.h Util.h Vec3f.h Vec3x.h \
 Array.h Matrix.h Vec3d.h DuneApp.h List.h MyString.h StringArray.h \
 Node.h FieldIndex.h Element.h x3dFlags.h swt/include/WriteFlags.h \
 NodeFlags.h MeshFlags.h ac3dMaterialCallback.h NodeArray.h Stack.h \
 Quaternion.h URL.h PreferencesApp.h swt/include/swttypedef.h \
 EcmaScriptApp.h StereoViewApp.h swt/include/stereoview.h \
 InputDeviceApp.h TransformMode.h OutputApp.h RouteViewApp.h \
 StartWithApp.h ExportNamesApp.h ModellingApp.h RenderApp.h MyMesh.h \
 Vec4f.h Vec2f.h swt/include/swt.h swt/include/keycodes.h \
 swt/include/swlanguages.h swt/include/swDebugf.h \
 swt/include/swttypedef.h NodeIndexedFaceSet.h MeshBasedNode.h \
 GeometryNode.h NodeAppearance.h ProtoMacros.h Proto.h Map.h NodeList.h \
 InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h \
 Arrays.h MySwap.h Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h \
 MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h \
 MFRotation.h MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h \
 MFVec3d.h MFVec3f.h MFVec4f.h SFBool.h SFColor.h SFColorRGBA.h SFImage.h \
 SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h SFString.h \
 SFVec2d.h SFVec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h \
 CommandList.h Command.h NodeMaterial.h NodeImageTexture.h \
 KambiTextureCommonFields.h NodeTextureTransform.h GeometryNodeMacros.h \
 NodeCoordinate.h Colored.h NodeColor.h NodeColorRGBA.h \
 ComposedGeometryMacros.h FaceData.h NodeNormal.h NodeTextureCoordinate.h \
 NodeTextureCoordinateGenerator.h NodeMultiTextureCoordinate.h Scene.h \
 RenderState.h ProtoMap.h LdrawDefines.h NodeExtrusion.h NodeTransform.h \
 TransformNode.h BasicTransformNode.h
quadric_simp.o: quadric_simp.cpp config.h quadric_simp.h \
 ../vcglib/vcg/complex/complex.h ../vcglib/wrap/callback.h \
 ../vcglib/vcg/complex/exception.h \
 ../vcglib/vcg/container/simple_temporary_data.h \
 ../vcglib/vcg/complex/used_types.h ../vcglib/vcg/space/point3.h \
 ../vcglib/vcg/space/deprecated_point3.h ../vcglib/vcg/math/base.h \
 ../vcglib/vcg/space/box3.h ../vcglib/vcg/math/matrix44.h \
 ../vcglib/vcg/space/point4.h ../vcglib/vcg/space/deprecated_point4.h \
 ../vcglib/eigenlib/Eigen/Core \
 ../vcglib/eigenlib/Eigen/src/Core/util/DisableStupidWarnings.h \
 ../vcglib/eigenlib/Eigen/src/Core/util/Macros.h \
 ../vcglib/eigenlib/Eigen/src/Core/util/MKL_support.h \
 ../vcglib/eigenlib/Eigen/src/Core/util/Constants.h \
 ../vcglib/eigenlib/Eigen/src/Core/util/ForwardDeclarations.h \
 ../vcglib/eigenlib/Eigen/src/Core/util/Meta.h \
 ../vcglib/eigenlib/Eigen/src/Core/util/StaticAssert.h \
 ../vcglib/eigenlib/Eigen/src/Core/util/XprHelper.h \
 ../vcglib/eigenlib/Eigen/src/Core/util/Memory.h \
 ../vcglib/eigenlib/Eigen/src/Core/NumTraits.h \
 ../vcglib/eigenlib/Eigen/src/Core/MathFunctions.h \
 ../vcglib/eigenlib/Eigen/src/Core/GenericPacketMath.h \
 ../vcglib/eigenlib/Eigen/src/Core/arch/SSE/PacketMath.h \
 ../vcglib/eigenlib/Eigen/src/Core/arch/SSE/MathFunctions.h \
 ../vcglib/eigenlib/Eigen/src/Core/arch/SSE/Complex.h \
 ../vcglib/eigenlib/Eigen/src/Core/arch/Default/Settings.h \
 ../vcglib/eigenlib/Eigen/src/Core/Functors.h \
 ../vcglib/eigenlib/Eigen/src/Core/DenseCoeffsBase.h \
 ../vcglib/eigenlib/Eigen/src/Core/DenseBase.h \
 ../vcglib/eigenlib/Eigen/src/Core/../plugins/BlockMethods.h \
 ../vcglib/eigenlib/Eigen/src/Core/MatrixBase.h \
 ../vcglib/eigenlib/Eigen/src/Core/../plugins/CommonCwiseUnaryOps.h \
 ../vcglib/eigenlib/Eigen/src/Core/../plugins/CommonCwiseBinaryOps.h \
 ../vcglib/eigenlib/Eigen/src/Core/../plugins/MatrixCwiseUnaryOps.h \
 ../vcglib/eigenlib/Eigen/src/Core/../plugins/MatrixCwiseBinaryOps.h \
 ../vcglib/eigenlib/Eigen/src/Core/EigenBase.h \
 ../vcglib/eigenlib/Eigen/src/Core/Assign.h \
 ../vcglib/eigenlib/Eigen/src/Core/util/BlasUtil.h \
 ../vcglib/eigenlib/Eigen/src/Core/DenseStorage.h \
 ../vcglib/eigenlib/Eigen/src/Core/NestByValue.h \
 ../vcglib/eigenlib/Eigen/src/Core/ForceAlignedAccess.h \
 ../vcglib/eigenlib/Eigen/src/Core/ReturnByValue.h \
 ../vcglib/eigenlib/Eigen/src/Core/NoAlias.h \
 ../vcglib/eigenlib/Eigen/src/Core/PlainObjectBase.h \
 ../vcglib/eigenlib/Eigen/src/Core/Matrix.h \
 ../vcglib/eigenlib/Eigen/src/Core/Array.h \
 ../vcglib/eigenlib/Eigen/src/Core/CwiseBinaryOp.h \
 ../vcglib/eigenlib/Eigen/src/Core/CwiseUnaryOp.h \
 ../vcglib/eigenlib/Eigen/src/Core/CwiseNullaryOp.h \
 ../vcglib/eigenlib/Eigen/src/Core/CwiseUnaryView.h \
 ../vcglib/eigenlib/Eigen/src/Core/SelfCwiseBinaryOp.h \
 ../vcglib/eigenlib/Eigen/src/Core/Dot.h \
 ../vcglib/eigenlib/Eigen/src/Core/StableNorm.h \
 ../vcglib/eigenlib/Eigen/src/Core/MapBase.h \
 ../vcglib/eigenlib/Eigen/src/Core/Stride.h \
 ../vcglib/eigenlib/Eigen/src/Core/Map.h \
 ../vcglib/eigenlib/Eigen/src/Core/Block.h \
 ../vcglib/eigenlib/Eigen/src/Core/VectorBlock.h \
 ../vcglib/eigenlib/Eigen/src/Core/Ref.h \
 ../vcglib/eigenlib/Eigen/src/Core/Transpose.h \
 ../vcglib/eigenlib/Eigen/src/Core/DiagonalMatrix.h \
 ../vcglib/eigenlib/Eigen/src/Core/Diagonal.h \
 ../vcglib/eigenlib/Eigen/src/Core/DiagonalProduct.h \
 ../vcglib/eigenlib/Eigen/src/Core/PermutationMatrix.h \
 ../vcglib/eigenlib/Eigen/src/Core/Transpositions.h \
 ../vcglib/eigenlib/Eigen/src/Core/Redux.h \
 ../vcglib/eigenlib/Eigen/src/Core/Visitor.h \
 ../vcglib/eigenlib/Eigen/src/Core/Fuzzy.h \
 ../vcglib/eigenlib/Eigen/src/Core/IO.h \
 ../vcglib/eigenlib/Eigen/src/Core/Swap.h \
 ../vcglib/eigenlib/Eigen/src/Core/CommaInitializer.h \
 ../vcglib/eigenlib/Eigen/src/Core/Flagged.h \
 ../vcglib/eigenlib/Eigen/src/Core/ProductBase.h \
 ../vcglib/eigenlib/Eigen/src/Core/GeneralProduct.h \
 ../vcglib/eigenlib/Eigen/src/Core/TriangularMatrix.h \
 ../vcglib/eigenlib/Eigen/src/Core/SelfAdjointView.h \
 ../vcglib/eigenlib/Eigen/src/Core/products/GeneralBlockPanelKernel.h \
 ../vcglib/eigenlib/Eigen/src/Core/products/Parallelizer.h \
 ../vcglib/eigenlib/Eigen/src/Core/products/CoeffBasedProduct.h \
 ../vcglib/eigenlib/Eigen/src/Core/products/GeneralMatrixVector.h \
 ../vcglib/eigenlib/Eigen/src/Core/products/GeneralMatrixMatrix.h \
 ../vcglib/eigenlib/Eigen/src/Core/SolveTriangular.h \
 ../vcglib/eigenlib/Eigen/src/Core/products/GeneralMatrixMatrixTriangular.h \
 ../vcglib/eigenlib/Eigen/src/Core/products/SelfadjointMatrixVector.h \
 ../vcglib/eigenlib/Eigen/src/Core/products/SelfadjointMatrixMatrix.h \
 ../vcglib/eigenlib/Eigen/src/Core/products/SelfadjointProduct.h \
 ../vcglib/eigenlib/Eigen/src/Core/products/SelfadjointRank2Update.h \
 ../vcglib/eigenlib/Eigen/src/Core/products/TriangularMatrixVector.h \
 ../vcglib/eigenlib/Eigen/src/Core/products/TriangularMatrixMatrix.h \
 ../vcglib/eigenlib/Eigen/src/Core/products/TriangularSolverMatrix.h \
 ../vcglib/eigenlib/Eigen/src/Core/products/TriangularSolverVector.h \
 ../vcglib/eigenlib/Eigen/src/Core/BandMatrix.h \
 ../vcglib/eigenlib/Eigen/src/Core/CoreIterators.h \
 ../vcglib/eigenlib/Eigen/src/Core/BooleanRedux.h \
 ../vcglib/eigenlib/Eigen/src/Core/Select.h \
 ../vcglib/eigenlib/Eigen/src/Core/VectorwiseOp.h \
 ../vcglib/eigenlib/Eigen/src/Core/Random.h \
 ../vcglib/eigenlib/Eigen/src/Core/Replicate.h \
 ../vcglib/eigenlib/Eigen/src/Core/Reverse.h \
 ../vcglib/eigenlib/Eigen/src/Core/ArrayBase.h \
 ../vcglib/eigenlib/Eigen/src/Core/../plugins/ArrayCwiseUnaryOps.h \
 ../vcglib/eigenlib/Eigen/src/Core/../plugins/ArrayCwiseBinaryOps.h \
 ../vcglib/eigenlib/Eigen/src/Core/ArrayWrapper.h \
 ../vcglib/eigenlib/Eigen/src/Core/GlobalFunctions.h \
 ../vcglib/eigenlib/Eigen/src/Core/util/ReenableStupidWarnings.h \
 ../vcglib/eigenlib/Eigen/LU ../vcglib/eigenlib/Eigen/Core \
 ../vcglib/eigenlib/Eigen/src/misc/Solve.h \
 ../vcglib/eigenlib/Eigen/src/misc/Kernel.h \
 ../vcglib/eigenlib/Eigen/src/misc/Image.h \
 ../vcglib/eigenlib/Eigen/src/LU/FullPivLU.h \
 ../vcglib/eigenlib/Eigen/src/LU/PartialPivLU.h \
 ../vcglib/eigenlib/Eigen/src/LU/Determinant.h \
 ../vcglib/eigenlib/Eigen/src/LU/Inverse.h \
 ../vcglib/eigenlib/Eigen/src/LU/arch/Inverse_SSE.h \
 ../vcglib/vcg/space/line3.h ../vcglib/vcg/space/plane3.h \
 ../vcglib/vcg/space/color4.h ../vcglib/vcg/math/shot.h \
 ../vcglib/vcg/space/point2.h ../vcglib/vcg/space/deprecated_point2.h \
 ../vcglib/vcg/math/similarity.h ../vcglib/vcg/math/quaternion.h \
 ../vcglib/vcg/math/matrix33.h ../vcglib/vcg/math/camera.h \
 ../vcglib/vcg/space/texcoord2.h ../vcglib/vcg/space/triangle3.h \
 ../vcglib/vcg/space/segment3.h ../vcglib/vcg/space/triangle2.h \
 ../vcglib/vcg/space/segment2.h ../vcglib/vcg/space/line2.h \
 ../vcglib/vcg/space/box2.h ../vcglib/vcg/space/polygon3.h \
 ../vcglib/vcg/space/fitting3.h ../vcglib/eigenlib/Eigen/Eigenvalues \
 ../vcglib/eigenlib/Eigen/Cholesky \
 ../vcglib/eigenlib/Eigen/src/Cholesky/LLT.h \
 ../vcglib/eigenlib/Eigen/src/Cholesky/LDLT.h \
 ../vcglib/eigenlib/Eigen/Jacobi \
 ../vcglib/eigenlib/Eigen/src/Jacobi/Jacobi.h \
 ../vcglib/eigenlib/Eigen/Householder \
 ../vcglib/eigenlib/Eigen/src/Householder/Householder.h \
 ../vcglib/eigenlib/Eigen/src/Householder/HouseholderSequence.h \
 ../vcglib/eigenlib/Eigen/src/Householder/BlockHouseholder.h \
 ../vcglib/eigenlib/Eigen/LU ../vcglib/eigenlib/Eigen/Geometry \
 ../vcglib/eigenlib/Eigen/SVD ../vcglib/eigenlib/Eigen/QR \
 ../vcglib/eigenlib/Eigen/src/QR/HouseholderQR.h \
 ../vcglib/eigenlib/Eigen/src/QR/FullPivHouseholderQR.h \
 ../vcglib/eigenlib/Eigen/src/QR/ColPivHouseholderQR.h \
 ../vcglib/eigenlib/Eigen/src/SVD/JacobiSVD.h \
 ../vcglib/eigenlib/Eigen/src/SVD/UpperBidiagonalization.h \
 ../vcglib/eigenlib/Eigen/src/Geometry/OrthoMethods.h \
 ../vcglib/eigenlib/Eigen/src/Geometry/EulerAngles.h \
 ../vcglib/eigenlib/Eigen/src/Geometry/Homogeneous.h \
 ../vcglib/eigenlib/Eigen/src/Geometry/RotationBase.h \
 ../vcglib/eigenlib/Eigen/src/Geometry/Rotation2D.h \
 ../vcglib/eigenlib/Eigen/src/Geometry/Quaternion.h \
 ../vcglib/eigenlib/Eigen/src/Geometry/AngleAxis.h \
 ../vcglib/eigenlib/Eigen/src/Geometry/Transform.h \
 ../vcglib/eigenlib/Eigen/src/Geometry/Translation.h \
 ../vcglib/eigenlib/Eigen/src/Geometry/Scaling.h \
 ../vcglib/eigenlib/Eigen/src/Geometry/Hyperplane.h \
 ../vcglib/eigenlib/Eigen/src/Geometry/ParametrizedLine.h \
 ../vcglib/eigenlib/Eigen/src/Geometry/AlignedBox.h \
 ../vcglib/eigenlib/Eigen/src/Geometry/Umeyama.h \
 ../vcglib/eigenlib/Eigen/src/Geometry/arch/Geometry_SSE.h \
 ../vcglib/eigenlib/Eigen/src/Eigenvalues/Tridiagonalization.h \
 ../vcglib/eigenlib/Eigen/src/Eigenvalues/RealSchur.h \
 ../vcglib/eigenlib/Eigen/src/Eigenvalues/./HessenbergDecomposition.h \
 ../vcglib/eigenlib/Eigen/src/Eigenvalues/EigenSolver.h \
 ../vcglib/eigenlib/Eigen/src/Eigenvalues/./RealSchur.h \
 ../vcglib/eigenlib/Eigen/src/Eigenvalues/SelfAdjointEigenSolver.h \
 ../vcglib/eigenlib/Eigen/src/Eigenvalues/./Tridiagonalization.h \
 ../vcglib/eigenlib/Eigen/src/Eigenvalues/GeneralizedSelfAdjointEigenSolver.h \
 ../vcglib/eigenlib/Eigen/src/Eigenvalues/HessenbergDecomposition.h \
 ../vcglib/eigenlib/Eigen/src/Eigenvalues/ComplexSchur.h \
 ../vcglib/eigenlib/Eigen/src/Eigenvalues/ComplexEigenSolver.h \
 ../vcglib/eigenlib/Eigen/src/Eigenvalues/./ComplexSchur.h \
 ../vcglib/eigenlib/Eigen/src/Eigenvalues/RealQZ.h \
 ../vcglib/eigenlib/Eigen/src/Eigenvalues/GeneralizedEigenSolver.h \
 ../vcglib/eigenlib/Eigen/src/Eigenvalues/./RealQZ.h \
 ../vcglib/eigenlib/Eigen/src/Eigenvalues/MatrixBaseEigenvalues.h \
 ../vcglib/vcg/space/point_matching.h ../vcglib/eigenlib/Eigen/Dense \
 ../vcglib/eigenlib/Eigen/Eigenvalues \
 ../vcglib/vcg/container/derivation_chain.h \
 ../vcglib/vcg/complex/all_types.h \
 ../vcglib/vcg/simplex/vertex/component.h \
 ../vcglib/vcg/simplex/vertex/component_ocf.h \
 ../vcglib/vcg/simplex/vertex/base.h \
 ../vcglib/vcg/simplex/face/component.h \
 ../vcglib/vcg/simplex/face/component_ocf.h \
 ../vcglib/vcg/simplex/face/component_polygon.h \
 ../vcglib/vcg/simplex/face/base.h ../vcglib/vcg/simplex/edge/component.h \
 ../vcglib/vcg/simplex/edge/base.h \
 ../vcglib/vcg/connectors/hedge_component.h \
 ../vcglib/vcg/connectors/hedge.h ../vcglib/vcg/complex/base.h \
 ../vcglib/vcg/complex/allocate.h \
 ../vcglib/vcg/complex/algorithms/update/flag.h \
 ../vcglib/vcg/simplex/face/pos.h \
 ../vcglib/vcg/complex/algorithms/update/selection.h \
 ../vcglib/vcg/complex/algorithms/mesh_assert.h \
 ../vcglib/vcg/complex/append.h \
 ../vcglib/vcg/complex/algorithms/update/position.h \
 ../vcglib/vcg/complex/algorithms/update/normal.h \
 ../vcglib/vcg/complex/algorithms/polygon_support.h \
 ../vcglib/vcg/simplex/face/jumping_pos.h \
 ../vcglib/vcg/space/planar_polygon_tessellation.h \
 ../vcglib/vcg/math/random_generator.h \
 ../vcglib/vcg/complex/algorithms/update/bounding.h \
 ../vcglib/vcg/complex/algorithms/clean.h \
 ../vcglib/vcg/simplex/face/topology.h \
 ../vcglib/vcg/simplex/edge/topology.h ../vcglib/vcg/simplex/edge/pos.h \
 ../vcglib/vcg/complex/algorithms/closest.h \
 ../vcglib/vcg/space/index/grid_util.h ../vcglib/vcg/space/index/base.h \
 ../vcglib/vcg/space/ray3.h ../vcglib/vcg/space/index/space_iterators.h \
 ../vcglib/vcg/space/intersection3.h ../vcglib/vcg/space/sphere3.h \
 ../vcglib/vcg/space/intersection/triangle_triangle3.h \
 ../vcglib/vcg/space/index/grid_closest.h \
 ../vcglib/vcg/simplex/face/distance.h ../vcglib/vcg/space/distance3.h \
 ../vcglib/vcg/simplex/edge/distance.h \
 ../vcglib/vcg/simplex/vertex/distance.h \
 ../vcglib/vcg/space/index/grid_static_ptr.h \
 ../vcglib/vcg/space/index/spatial_hashing.h \
 ../vcglib/vcg/complex/algorithms/update/normal.h \
 ../vcglib/vcg/complex/algorithms/update/topology.h \
 ../vcglib/vcg/complex/algorithms/local_optimization.h \
 ../vcglib/vcg/complex/algorithms/local_optimization/tri_edge_collapse_quadric.h \
 ../vcglib/vcg/math/quadric.h \
 ../vcglib/vcg/complex/algorithms/local_optimization/tri_edge_collapse.h \
 ../vcglib/vcg/complex/algorithms/edge_collapse.h \
 ../vcglib/vcg/complex/algorithms/stat.h ../vcglib/vcg/math/histogram.h \
 ../vcglib/vcg/complex/algorithms/inertia.h ml_mesh_type.h \
 swt/include/swt.h swt/include/keycodes.h swt/include/swlanguages.h \
 swt/include/swDebugf.h swt/include/swttypedef.h
NodeMovieTexture.o: NodeMovieTexture.cpp stdafx.h config.h mysnprintf.h \
 NodeMovieTexture.h Node.h Array.h List.h Vec3f.h Vec3x.h MyString.h \
 StringArray.h FieldIndex.h Element.h x3dFlags.h swt/include/WriteFlags.h \
 NodeFlags.h MeshFlags.h ac3dMaterialCallback.h NodeArray.h Stack.h \
 Matrix.h Vec3d.h Quaternion.h ProtoMacros.h Proto.h Map.h NodeList.h \
 InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h \
 Arrays.h MySwap.h Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h \
 MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h \
 MFRotation.h MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h \
 MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h \
 SFImage.h SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h \
 SFString.h SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h \
 SFVec4f.h CommandList.h Command.h KambiTextureCommonFields.h Field.h \
 ExposedField.h DuneApp.h URL.h PreferencesApp.h swt/include/swttypedef.h \
 EcmaScriptApp.h StereoViewApp.h swt/include/stereoview.h \
 InputDeviceApp.h TransformMode.h OutputApp.h RouteViewApp.h \
 StartWithApp.h ExportNamesApp.h ModellingApp.h RenderApp.h Scene.h \
 RenderState.h ProtoMap.h swt/include/swt.h swt/include/keycodes.h \
 swt/include/swlanguages.h swt/include/swDebugf.h \
 swt/include/swttypedef.h freewrl_define.h
MeshSubdivide.o: MeshSubdivide.cpp stdafx.h config.h mysnprintf.h \
 NodeIndexedFaceSet.h MeshBasedNode.h GeometryNode.h Node.h Array.h \
 List.h Vec3f.h Vec3x.h MyString.h StringArray.h FieldIndex.h Element.h \
 x3dFlags.h swt/include/WriteFlags.h NodeFlags.h MeshFlags.h \
 ac3dMaterialCallback.h NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h \
 NodeAppearance.h ProtoMacros.h Proto.h Map.h NodeList.h InterfaceArray.h \
 DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h Arrays.h MySwap.h \
 Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h MFDouble.h \
 SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h MFRotation.h \
 MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h \
 MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h SFImage.h \
 SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h SFString.h \
 SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h \
 CommandList.h Command.h DuneApp.h URL.h PreferencesApp.h \
 swt/include/swttypedef.h EcmaScriptApp.h StereoViewApp.h \
 swt/include/stereoview.h InputDeviceApp.h TransformMode.h OutputApp.h \
 RouteViewApp.h StartWithApp.h ExportNamesApp.h ModellingApp.h \
 RenderApp.h NodeMaterial.h swt/include/swt.h swt/include/keycodes.h \
 swt/include/swlanguages.h swt/include/swDebugf.h \
 swt/include/swttypedef.h NodeImageTexture.h KambiTextureCommonFields.h \
 NodeTextureTransform.h GeometryNodeMacros.h NodeCoordinate.h Colored.h \
 NodeColor.h NodeColorRGBA.h ComposedGeometryMacros.h Scene.h \
 RenderState.h ProtoMap.h FaceData.h MyMesh.h NodeNormal.h \
 NodeTextureCoordinate.h NodeTextureCoordinateGenerator.h \
 NodeMultiTextureCoordinate.h Util.h
Scene.o: Scene.cpp stdafx.h config.h mysnprintf.h Util.h Vec3f.h Vec3x.h \
 Array.h resource.h swt/include/swt.h swt/include/keycodes.h \
 swt/include/swlanguages.h swt/include/swDebugf.h \
 swt/include/swttypedef.h Scene.h Map.h List.h Stack.h CommandList.h \
 Command.h NodeList.h NodeArray.h Node.h MyString.h StringArray.h \
 FieldIndex.h Element.h x3dFlags.h swt/include/WriteFlags.h NodeFlags.h \
 MeshFlags.h ac3dMaterialCallback.h Matrix.h Vec3d.h Quaternion.h URL.h \
 Proto.h ProtoMacros.h InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h \
 MFBool.h FieldValue.h Arrays.h MySwap.h Types.h MFColor.h MFFloat.h \
 SFFloat.h MFColorRGBA.h MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h \
 MFMatrix4f.h MFNode.h MFRotation.h MFString.h MFTime.h SFTime.h \
 MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h \
 SFBool.h SFColor.h SFColorRGBA.h SFImage.h SFInt32.h SFMatrix3f.h \
 SFMatrix4f.h SFNode.h SFRotation.h SFString.h SFVec2d.h SFVec2f.h \
 Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h DuneApp.h \
 PreferencesApp.h swt/include/swttypedef.h EcmaScriptApp.h \
 StereoViewApp.h swt/include/stereoview.h InputDeviceApp.h \
 TransformMode.h OutputApp.h RouteViewApp.h StartWithApp.h \
 ExportNamesApp.h ModellingApp.h RenderApp.h RenderState.h ProtoMap.h \
 SceneProtoMap.h SceneView.h WonderlandModuleExport.h FieldCommand.h \
 MFieldCommand.h RouteCommand.h UnRouteCommand.h MoveCommand.h parser.h \
 Path.h Field.h EventIn.h EventOut.h ExposedField.h FontInfo.h \
 Interpolator.h NextCommand.h FaceData.h NodeTimeSensor.h NodeVrmlCut.h \
 NodeVrmlScene.h NodeNavigationInfo.h NodeViewpoint.h ViewpointNode.h \
 CoverNode.h KambiViewpointCommonFields.h NodeOrthoViewpoint.h \
 NodeGeoViewpoint.h GeoNode.h NodeBackground.h NodeCurveAnimation.h \
 NodeNurbsCurve.h NurbsCurve.h GeometryNode.h NodeAppearance.h \
 NodeMaterial.h NodeImageTexture.h KambiTextureCommonFields.h \
 NodeTextureTransform.h GeometryNodeMacros.h ChainBasedNode.h \
 NodePositionInterpolator.h NodeOrientationInterpolator.h NodeFog.h \
 FogMacros.h NodeInline.h NodeGroup.h StaticGroupNode.h GroupNode.h \
 NodeShape.h NodeIndexedFaceSet.h MeshBasedNode.h NodeCoordinate.h \
 Colored.h NodeColor.h NodeColorRGBA.h ComposedGeometryMacros.h \
 NodeIndexedLineSet.h NodeIndexedTriangleSet.h MyMesh.h NodeNormal.h \
 NodeTextureCoordinate.h NodeTextureCoordinateGenerator.h \
 NodeMultiTextureCoordinate.h IndexedTriangleSetNode.h NodeTriangleSet.h \
 NodeNurbsSurface.h MeshMorphingNode.h NodeCattExportRec.h \
 NodeLdrawDatExport.h NodeViewport.h
SceneProtoMap.o: SceneProtoMap.cpp stdafx.h config.h mysnprintf.h \
 SceneProtoMap.h ProtoMap.h Map.h List.h MyString.h StringArray.h Array.h \
 NodeAnchor.h Node.h Vec3f.h Vec3x.h FieldIndex.h Element.h x3dFlags.h \
 swt/include/WriteFlags.h NodeFlags.h MeshFlags.h ac3dMaterialCallback.h \
 NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h ProtoMacros.h Proto.h \
 NodeList.h InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h MFBool.h \
 FieldValue.h Arrays.h MySwap.h Types.h MFColor.h MFFloat.h SFFloat.h \
 MFColorRGBA.h MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h \
 MFNode.h MFRotation.h MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h \
 MFVec2f.h MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h \
 SFColorRGBA.h SFImage.h SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h \
 SFRotation.h SFString.h SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h \
 SFVec3f.h SFVec4f.h CommandList.h Command.h NodeGroup.h \
 StaticGroupNode.h GroupNode.h NodeAppearance.h DuneApp.h URL.h \
 PreferencesApp.h swt/include/swttypedef.h EcmaScriptApp.h \
 StereoViewApp.h swt/include/stereoview.h InputDeviceApp.h \
 TransformMode.h OutputApp.h RouteViewApp.h StartWithApp.h \
 ExportNamesApp.h ModellingApp.h RenderApp.h NodeArc2D.h ChainBasedNode.h \
 GeometryNode.h NodeMaterial.h swt/include/swt.h swt/include/keycodes.h \
 swt/include/swlanguages.h swt/include/swDebugf.h \
 swt/include/swttypedef.h NodeImageTexture.h KambiTextureCommonFields.h \
 NodeTextureTransform.h GeometryNodeMacros.h NodeArcClose2D.h \
 MeshBasedNode.h NodeARSensor.h CoverNode.h NodeAudioClip.h \
 NodeBackground.h NodeBallJoint.h AnchoredJointNode.h X3DRigidJointNode.h \
 RigidBodyPhysicsNode.h NodeBinaryGeometry.h NodeBillboard.h \
 NodeBlendMode.h KambiCommonFunctions.h NodeBlendedVolumeStyle.h \
 NodeBlock.h NodeBooleanFilter.h NodeBooleanToggle.h NodeBooleanTrigger.h \
 NodeBooleanSequencer.h Interpolator.h \
 NodeBoundaryEnhancementVolumeStyle.h NodeBoundedPhysicsModel.h NodeBox.h \
 NodeCADAssembly.h NodeCADFace.h NodeCADLayer.h NodeCADPart.h \
 NodeTransform.h TransformNode.h BasicTransformNode.h NodeCOVER.h \
 NodeCartoonVolumeStyle.h NodeCattExportRec.h NodeCattExportSrc.h \
 NodeCircle2D.h NodeClipPlane.h NodeCollidableOffset.h \
 X3DNBodyCollidableNode.h NodeCollidableShape.h NodeCollision.h \
 NodeCollisionCollection.h NodeCollisionSensor.h NodeCollisionSpace.h \
 X3DNBodyCollisionSpaceNode.h NodeColor.h NodeColorDamper.h DamperNode.h \
 NodeColorChaser.h ChaserNode.h NodeColorInterpolator.h \
 NodeColorMaskMode.h NodeColorRGBA.h NodeColorSetInterpolator.h \
 NodeCommonSurfaceShader.h NodeComposedCubeMapTexture.h \
 NodeComposedShader.h NodeComposedTexture3D.h NodeComposedVolumeStyle.h \
 NodeCone.h NodeConeEmitter.h EmitterNode.h NodeContact.h NodeContour2D.h \
 NodeContourPolyline2D.h NodeCoordinate.h NodeCoordinateChaser.h \
 NodeCoordinateDamper.h NodeCoordinateDeformer.h NodeCoordinateDouble.h \
 NodeCoordinateInterpolator2D.h NodeCoordinateInterpolator.h \
 NodeCubeTexture.h NodeCubicBezier2DOrientationInterpolator.h \
 NodeCubicBezierPositionInterpolator.h NodeCurveAnimation.h \
 NodeNurbsCurve.h NurbsCurve.h NodePositionInterpolator.h \
 NodeOrientationInterpolator.h NodeCylinder.h NodeCylinderSensor.h \
 NodeDepthMode.h NodeDirectionalLight.h KambiLightCommonFields.h \
 X3domLightCommonFields.h NodeDISEntityManager.h \
 NodeDISEntityTypeMapping.h NodeDish.h NodeDisk2D.h \
 NodeDoubleAxisHingeJoint.h NodeDynamicLOD.h NodeLOD.h \
 NodeEaseInEaseOut.h NodeEdgeEnhancementVolumeStyle.h NodeEffect.h \
 NodeEffectPart.h NodeElevationGrid.h NodeEnvironment.h \
 NodeEspduTransform.h DISCommonFields.h NodeExplosionEmitter.h \
 NodeExternalGeometry.h NodeExtrusion.h NodeField.h NodeFillProperties.h \
 NodeFloatVertexAttribute.h NodeFog.h FogMacros.h NodeFogCoordinate.h \
 NodeFontStyle.h FontStyleNode.h NodeForcePhysicsModel.h \
 NodeGeneratedCubeMapTexture.h NodeGeneratedShadowMap.h \
 NodeGeoCoordinate.h GeoNode.h NodeGeoElevationGrid.h NodeGeoOrigin.h \
 NodeGeoMetadata.h NodeGeoLocation.h NodeGeoLOD.h \
 NodeGeoPositionInterpolator.h NodeGeoProximitySensor.h \
 NodeGeoTouchSensor.h NodeGeoTransform.h NodeGeoViewpoint.h \
 ViewpointNode.h KambiViewpointCommonFields.h NodeHAnimDisplacer.h \
 NodeHAnimHumanoid.h MyMesh.h NodeIndexedFaceSet.h Colored.h \
 ComposedGeometryMacros.h FaceData.h NodeNormal.h NodeTextureCoordinate.h \
 NodeTextureCoordinateGenerator.h NodeMultiTextureCoordinate.h Scene.h \
 RenderState.h Util.h NodeHAnimJoint.h NodeHAnimSegment.h NodeHAnimSite.h \
 NodeImageCubeMapTexture.h NodeImageGeometry.h NodeImageTexture3D.h \
 freewrl_define.h Texture3DNode.h NodeImageTextureAtlas.h \
 NodeIndexedLineSet.h NodeIndexedQuadSet.h NodeIndexedTriangleFanSet.h \
 IndexedTriangleSetNode.h NodeIndexedTriangleSet.h \
 NodeIndexedTriangleStripSet.h NodeInline.h NodeInlineLoadControl.h \
 NodeIntegerSequencer.h NodeIntegerTrigger.h NodeIsoSurfaceVolumeData.h \
 NodeJoystickSensor.h NodeKambiAppearance.h NodeKambiHeadLight.h \
 NodeKambiInline.h NodeKambiNavigationInfo.h NodeNavigationInfo.h \
 NodeKambiOctreeProperties.h NodeKambiTriangulation.h NodeKeySensor.h \
 NodeLabView.h NodeLayer.h NodeLayerSet.h NodeLayout.h NodeLayoutGroup.h \
 NodeLayoutLayer.h NodeLdrawDatExport.h NodeLinePickSensor.h \
 NodeLineProperties.h NodeLineSet.h NodeLoadSensor.h NodeLocalFog.h \
 NodeMatrix3VertexAttribute.h NodeMatrix4VertexAttribute.h \
 NodeMatrixTransform.h NodeMesh.h NodeMetadataBoolean.h \
 NodeMetadataDouble.h Metadata.h NodeMetadataFloat.h \
 NodeMetadataInteger.h NodeMetadataSet.h NodeMetadataString.h \
 NodeMotorJoint.h NodeMovieTexture.h NodeMPRVolumeStyle.h \
 NodeMultiGeneratedTextureCoordinate.h NodeMultiPart.h NodeMultiTexture.h \
 NodeMultiTextureTransform.h NodeNormalInterpolator.h NodeNozzle.h \
 NodeNurbsCurve2D.h NodeNurbsGroup.h NodeNurbsOrientationInterpolator.h \
 NodeNurbsPositionInterpolator.h NodeNurbsSet.h NodeNurbsSurface.h \
 MeshMorphingNode.h NodeNurbsSurfaceInterpolator.h \
 NodeNurbsSweptSurface.h NodeNurbsSwungSurface.h \
 NodeNurbsTextureSurface.h NodeNurbsTextureCoordinate.h \
 NodeNurbsTrimmedSurface.h NodeOpacityMapVolumeStyle.h \
 NodeOrientationChaser.h NodeOrientationDamper.h NodeOrthoViewpoint.h \
 NodePackagedShader.h NodeParam.h NodeParticleSet.h NodeParticleSystem.h \
 NodePickableGroup.h NodePixelTexture.h NodePixelTexture3D.h NodePlane.h \
 NodePlaneSensor.h NodePointEmitter.h NodePointLight.h \
 NodePointPickSensor.h NodePointSet.h NodePolyline2D.h \
 NodePolylineEmitter.h NodePolypoint2D.h NodePopGeometry.h \
 NodePopGeometryLevel.h NodePositionChaser.h NodePositionChaser2D.h \
 NodePositionDamper.h NodePositionDamper2D.h NodePositionInterpolator2D.h \
 NodePrimitivePickSensor.h NodeProjectionVolumeStyle.h \
 NodeProgramShader.h NodeProjectedTextureCoordinate.h \
 NodeProximitySensor.h NodePyramid.h NodeQuadSet.h NodeRadarVolumeStyle.h \
 NodeReceiverPdu.h NodeRectangle2D.h NodeRectangularTorus.h \
 NodeRenderedTexture.h NodeRefinementTexture.h NodeRemoteSelectionGroup.h \
 NodeRigidBody.h NodeRigidBodyCollection.h NodeScalarChaser.h \
 NodeScalarDamper.h NodeScalarInterpolator.h NodeScreenEffect.h \
 NodeScreenFontStyle.h NodeScreenGroup.h NodeStaticGroup.h \
 NodeSegmentedVolumeData.h NodeShadedVolumeStyle.h NodeShaderPart.h \
 NodeShaderProgram.h NodeShaderTexture.h NodeShape.h NodeSignalPdu.h \
 NodeSilhouetteEnhancementVolumeStyle.h NodeSingleAxisHingeJoint.h \
 NodeSky.h NodeSliderJoint.h NodeSlopedCylinder.h NodeSnout.h \
 NodeSolidOfRevolution.h NodeSound.h NodeSpaceSensor.h NodeSphere.h \
 NodeSphereSegment.h NodeSphereSensor.h NodeSplinePositionInterpolator.h \
 NodeSplinePositionInterpolator2D.h NodeSplineScalarInterpolator.h \
 NodeSpotLight.h NodeSquadOrientationInterpolator.h NodeSteeringWheel.h \
 NodeStippleVolumeStyle.h NodeStringSensor.h NodeSuperEllipsoid.h \
 NodeSuperExtrusion.h NodeSuperRevolver.h NodeSuperShape.h \
 NodeSurfaceEmitter.h NodeSurfaceShaderTexture.h NodeSwitch.h \
 NodeTeapot.h NodeTexCoordChaser2D.h NodeTexCoordDamper2D.h NodeText.h \
 NodeText3D.h NodeTextureBackground.h NodeTextureCoordinate3D.h \
 NodeTextureCoordinate4D.h NodeTextureProperties.h \
 NodeTextureTransform3D.h NodeTextureTransformMatrix3D.h \
 NodeTimeTrigger.h NodeTimeSensor.h NodeToneMappedVolumeStyle.h \
 NodeTorus.h NodeTouchSensor.h NodeTransformSensor.h NodeTransmitterPdu.h \
 NodeTriangleFanSet.h NodeTriangleSet.h NodeTriangleSet2D.h \
 NodeTriangleStripSet.h NodeTrimmedSurface.h NodeTUIButton.h TUIElement.h \
 NodeTUIComboBox.h NodeTUIFloatSlider.h NodeTUIFrame.h NodeTUILabel.h \
 NodeTUIListBox.h NodeTUIMap.h NodeTUIProgressBar.h NodeTUISlider.h \
 NodeTUISplitter.h NodeTUITab.h NodeTUITabFolder.h NodeTUIToggleButton.h \
 NodeTwoSidedMaterial.h NodeUniform.h NodeUniversalJoint.h NodeVehicle.h \
 NodeVectorInterpolator.h NodeViewfrustum.h NodeViewpoint.h \
 NodeViewpointGroup.h NodeViewport.h NodeVisibilitySensor.h \
 NodeVirtualAcoustics.h NodeVirtualSoundSource.h NodeVolumeData.h \
 NodeVolumeEmitter.h NodeVolumePickSensor.h NodeVrmlCut.h NodeVrmlScene.h \
 NodeWave.h NodeWindPhysicsModel.h NodeWonderlandImportJava.h \
 NodeWorldInfo.h NodeComment.h NodeImport.h NodeExport.h
Node.o: Node.cpp stdafx.h config.h mysnprintf.h MeshBasedNode.h \
 GeometryNode.h Node.h Array.h List.h Vec3f.h Vec3x.h MyString.h \
 StringArray.h FieldIndex.h Element.h x3dFlags.h swt/include/WriteFlags.h \
 NodeFlags.h MeshFlags.h ac3dMaterialCallback.h NodeArray.h Stack.h \
 Matrix.h Vec3d.h Quaternion.h NodeAppearance.h ProtoMacros.h Proto.h \
 Map.h NodeList.h InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h \
 MFBool.h FieldValue.h Arrays.h MySwap.h Types.h MFColor.h MFFloat.h \
 SFFloat.h MFColorRGBA.h MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h \
 MFMatrix4f.h MFNode.h MFRotation.h MFString.h MFTime.h SFTime.h \
 MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h \
 SFBool.h SFColor.h SFColorRGBA.h SFImage.h SFInt32.h SFMatrix3f.h \
 SFMatrix4f.h SFNode.h SFRotation.h SFString.h SFVec2d.h SFVec2f.h \
 Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h CommandList.h Command.h \
 DuneApp.h URL.h PreferencesApp.h swt/include/swttypedef.h \
 EcmaScriptApp.h StereoViewApp.h swt/include/stereoview.h \
 InputDeviceApp.h TransformMode.h OutputApp.h RouteViewApp.h \
 StartWithApp.h ExportNamesApp.h ModellingApp.h RenderApp.h \
 NodeMaterial.h swt/include/swt.h swt/include/keycodes.h \
 swt/include/swlanguages.h swt/include/swDebugf.h \
 swt/include/swttypedef.h NodeImageTexture.h KambiTextureCommonFields.h \
 NodeTextureTransform.h GeometryNodeMacros.h Scene.h RenderState.h \
 ProtoMap.h Field.h FieldCommand.h Path.h EventIn.h EventOut.h \
 ExposedField.h swt/include/swDebugf.h MoveCommand.h NodeScript.h \
 NodeIndexedFaceSet.h NodeCoordinate.h Colored.h NodeColor.h \
 NodeColorRGBA.h ComposedGeometryMacros.h NodeCurveAnimation.h \
 NodeNurbsCurve.h NurbsCurve.h ChainBasedNode.h \
 NodePositionInterpolator.h Interpolator.h NodeOrientationInterpolator.h \
 NodeTimeSensor.h NodeInline.h NodeComment.h ScriptEventDialog.h Dialog.h \
 resource.h
MainWindow.o: MainWindow.cpp stdafx.h config.h mysnprintf.h MainWindow.h \
 PanedWindow.h SceneView.h DuneApp.h List.h Array.h MyString.h \
 StringArray.h Node.h Vec3f.h Vec3x.h FieldIndex.h Element.h x3dFlags.h \
 swt/include/WriteFlags.h NodeFlags.h MeshFlags.h ac3dMaterialCallback.h \
 NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h URL.h PreferencesApp.h \
 swt/include/swttypedef.h EcmaScriptApp.h StereoViewApp.h \
 swt/include/stereoview.h InputDeviceApp.h TransformMode.h OutputApp.h \
 RouteViewApp.h StartWithApp.h ExportNamesApp.h ModellingApp.h \
 RenderApp.h Scene.h Map.h CommandList.h Command.h NodeList.h Proto.h \
 ProtoMacros.h InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h MFBool.h \
 FieldValue.h Arrays.h MySwap.h Types.h MFColor.h MFFloat.h SFFloat.h \
 MFColorRGBA.h MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h \
 MFNode.h MFRotation.h MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h \
 MFVec2f.h MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h \
 SFColorRGBA.h SFImage.h SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h \
 SFRotation.h SFString.h SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h \
 SFVec3f.h SFVec4f.h RenderState.h ProtoMap.h ColorConversion.h \
 NodeNurbsCurve.h NurbsCurve.h GeometryNode.h NodeAppearance.h \
 NodeMaterial.h swt/include/swt.h swt/include/keycodes.h \
 swt/include/swlanguages.h swt/include/swDebugf.h \
 swt/include/swttypedef.h NodeImageTexture.h KambiTextureCommonFields.h \
 NodeTextureTransform.h GeometryNodeMacros.h ChainBasedNode.h resource.h \
 maxpath.h Field.h Path.h SceneTreeView.h Scene3DView.h InputDevice.h \
 EulerAngles.h QuatTypes.h swt/include/swtxinput.h Aflock.h FieldView.h \
 Rect.h Point.h SceneGraphView.h ChannelView.h ToolbarWindow.h \
 StatusBar.h MoveCommand.h RouteCommand.h X3DRigidJointNode.h \
 RigidBodyPhysicsNode.h NodeAudioClip.h NodeContact.h NodeContour2D.h \
 NodeColor.h NodeColorRGBA.h NodeElevationGrid.h MeshBasedNode.h \
 NodeGroup.h StaticGroupNode.h GroupNode.h NodeTransform.h \
 TransformNode.h BasicTransformNode.h NodeCollision.h \
 NodeCollidableShape.h X3DNBodyCollidableNode.h NodeBillboard.h NodeLOD.h \
 NodeSwitch.h NodeCubeTexture.h CoverNode.h NodeHAnimHumanoid.h MyMesh.h \
 NodeIndexedFaceSet.h NodeCoordinate.h Colored.h ComposedGeometryMacros.h \
 FaceData.h NodeNormal.h NodeTextureCoordinate.h \
 NodeTextureCoordinateGenerator.h NodeMultiTextureCoordinate.h Util.h \
 NodeHAnimJoint.h NodeInline.h NodeInlineLoadControl.h NodeMovieTexture.h \
 NodeNurbsGroup.h NodeNurbsSet.h NodeNurbsSurface.h MeshMorphingNode.h \
 Interpolator.h NodeNurbsCurve2D.h NodeNurbsPositionInterpolator.h \
 NodeNurbsOrientationInterpolator.h NodeNurbsSweptSurface.h \
 NodeExtrusion.h NodeNurbsSwungSurface.h NodeNurbsTrimmedSurface.h \
 NodeShape.h NodeScript.h NodeSound.h NodeSuperEllipsoid.h \
 NodeSuperExtrusion.h NodeSuperShape.h NodeSuperRevolver.h \
 NodeCurveAnimation.h NodePositionInterpolator.h \
 NodeOrientationInterpolator.h NodeText.h NodeTimeSensor.h NodeAnchor.h \
 NodeTriangleSet.h NodeIndexedTriangleSet.h IndexedTriangleSetNode.h \
 NodeIndexedLineSet.h NodePointSet.h NodeViewpoint.h ViewpointNode.h \
 KambiViewpointCommonFields.h NodeOrthoViewpoint.h NodeNavigationInfo.h \
 NodeWorldInfo.h NodeLineSet.h NodeGeoCoordinate.h GeoNode.h \
 NodeGeoElevationGrid.h NodeGeoOrigin.h NodeGeoLocation.h NodeGeoLOD.h \
 NodeGeoMetadata.h NodeGeoPositionInterpolator.h NodeGeoTouchSensor.h \
 NodeGeoViewpoint.h NodeRigidBody.h NodeRigidBodyCollection.h \
 NodeCollisionCollection.h NodeVrmlCut.h NodeVrmlScene.h NodeBox.h \
 NodeCone.h NodeCylinder.h NodeSphere.h NodeCollisionSensor.h \
 NodeParticleSystem.h NodeCADFace.h NodePackagedShader.h \
 NodeShaderProgram.h NodeShaderPart.h NodeVirtualSoundSource.h \
 NodeKambiAppearance.h KambiCommonFunctions.h NodeKambiHeadLight.h \
 NodeKambiTriangulation.h NodeText3D.h AboutDialog.h Dialog.h \
 AnimationDialog.h CheckBoxWindow.h CurveAnimationDialog.h \
 InteractionDialog.h InteractionDialog2.h InteractionDialog3.h \
 ProtoDialog.h ScriptDialog.h ElevationGridDialog.h NurbsPlaneDialog.h \
 NurbsCurveDialog.h ModellingSettingsDialog.h RenderSettingsDialog.h \
 EcmaScriptSettingsDialog.h OutputSettingsDialog.h \
 PreviewSettingsDialog.h UploadSettingsDialog.h HelpSettingsDialog.h \
 TexteditSettingsDialog.h StereoViewSettingsDialog.h \
 InputDeviceSelectionDialog.h InputDeviceNameSelectionDialog.h \
 InputDeviceSettingsDialog.h RouteViewSettingsDialog.h FieldPipeDialog.h \
 ProtoNameDialog.h ToIndexedFaceSetDialog.h HAnimJointDialog.h \
 ViewpointDialog.h NurbsCurve2DDialog.h InsertToNurbsCurveDialog.h \
 InsertToNurbsSurfaceDialog.h SceneDialog.h AflockSettingsDialog.h \
 DefDialog.h CoverDefDialog.h Cylinder2NurbsDialog.h Cone2NurbsDialog.h \
 Sphere2NurbsDialog.h Box2NurbsDialog.h NurbsCurve2NurbsSurfDialog.h \
 URLDialog.h ArrayDialog.h OneBoolDialog.h OneIntDialog.h \
 OneFloatDialog.h SliderFloatDialog.h OneTextDialog.h \
 TwoRadioButtonsDialog.h WonderlandModuleExportDialog.h \
 CattExportDialog.h SplitFacesDialog.h ColorCircle.h MFieldCommand.h \
 VertexModifier.h ScriptEdit.h NodePixelTexture.h
NodeIndexedFaceSet.o: NodeIndexedFaceSet.cpp stdafx.h config.h \
 mysnprintf.h NodeIndexedFaceSet.h MeshBasedNode.h GeometryNode.h Node.h \
 Array.h List.h Vec3f.h Vec3x.h MyString.h StringArray.h FieldIndex.h \
 Element.h x3dFlags.h swt/include/WriteFlags.h NodeFlags.h MeshFlags.h \
 ac3dMaterialCallback.h NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h \
 NodeAppearance.h ProtoMacros.h Proto.h Map.h NodeList.h InterfaceArray.h \
 DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h Arrays.h MySwap.h \
 Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h MFDouble.h \
 SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h MFRotation.h \
 MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h \
 MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h SFImage.h \
 SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h SFString.h \
 SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h \
 CommandList.h Command.h DuneApp.h URL.h PreferencesApp.h \
 swt/include/swttypedef.h EcmaScriptApp.h StereoViewApp.h \
 swt/include/stereoview.h InputDeviceApp.h TransformMode.h OutputApp.h \
 RouteViewApp.h StartWithApp.h ExportNamesApp.h ModellingApp.h \
 RenderApp.h NodeMaterial.h swt/include/swt.h swt/include/keycodes.h \
 swt/include/swlanguages.h swt/include/swDebugf.h \
 swt/include/swttypedef.h NodeImageTexture.h KambiTextureCommonFields.h \
 NodeTextureTransform.h GeometryNodeMacros.h NodeCoordinate.h Colored.h \
 NodeColor.h NodeColorRGBA.h ComposedGeometryMacros.h Scene.h \
 RenderState.h ProtoMap.h MyMesh.h FaceData.h NodeNormal.h \
 NodeTextureCoordinate.h NodeTextureCoordinateGenerator.h \
 NodeMultiTextureCoordinate.h Util.h NodeGeoCoordinate.h GeoNode.h \
 NodeIndexedLineSet.h NodeShape.h NodeFogCoordinate.h MoveCommand.h \
 Field.h resource.h Path.h ../vcglib/vcg/complex/complex.h \
 ../vcglib/wrap/callback.h ../vcglib/vcg/complex/exception.h \
 ../vcglib/vcg/container/simple_temporary_data.h \
 ../vcglib/vcg/complex/used_types.h ../vcglib/vcg/space/point3.h \
 ../vcglib/vcg/space/deprecated_point3.h ../vcglib/vcg/math/base.h \
 ../vcglib/vcg/space/box3.h ../vcglib/vcg/math/matrix44.h \
 ../vcglib/vcg/space/point4.h ../vcglib/vcg/space/deprecated_point4.h \
 ../vcglib/eigenlib/Eigen/Core ../vcglib/eigenlib/Eigen/LU \
 ../vcglib/vcg/space/line3.h ../vcglib/vcg/space/plane3.h \
 ../vcglib/vcg/space/color4.h ../vcglib/vcg/math/shot.h \
 ../vcglib/vcg/space/point2.h ../vcglib/vcg/space/deprecated_point2.h \
 ../vcglib/vcg/math/similarity.h ../vcglib/vcg/math/quaternion.h \
 ../vcglib/vcg/math/matrix33.h ../vcglib/vcg/math/camera.h \
 ../vcglib/vcg/space/texcoord2.h ../vcglib/vcg/space/triangle3.h \
 ../vcglib/vcg/space/segment3.h ../vcglib/vcg/space/triangle2.h \
 ../vcglib/vcg/space/segment2.h ../vcglib/vcg/space/line2.h \
 ../vcglib/vcg/space/box2.h ../vcglib/vcg/space/polygon3.h \
 ../vcglib/vcg/space/fitting3.h ../vcglib/eigenlib/Eigen/Eigenvalues \
 ../vcglib/vcg/space/point_matching.h ../vcglib/eigenlib/Eigen/Dense \
 ../vcglib/vcg/container/derivation_chain.h \
 ../vcglib/vcg/complex/all_types.h \
 ../vcglib/vcg/simplex/vertex/component.h \
 ../vcglib/vcg/simplex/vertex/component_ocf.h \
 ../vcglib/vcg/simplex/vertex/base.h \
 ../vcglib/vcg/simplex/face/component.h \
 ../vcglib/vcg/simplex/face/component_ocf.h \
 ../vcglib/vcg/simplex/face/component_polygon.h \
 ../vcglib/vcg/simplex/face/base.h ../vcglib/vcg/simplex/edge/component.h \
 ../vcglib/vcg/simplex/edge/base.h \
 ../vcglib/vcg/connectors/hedge_component.h \
 ../vcglib/vcg/connectors/hedge.h ../vcglib/vcg/complex/base.h \
 ../vcglib/vcg/complex/allocate.h \
 ../vcglib/vcg/complex/algorithms/update/flag.h \
 ../vcglib/vcg/simplex/face/pos.h \
 ../vcglib/vcg/complex/algorithms/update/selection.h \
 ../vcglib/vcg/complex/algorithms/mesh_assert.h \
 ../vcglib/vcg/complex/append.h \
 ../vcglib/vcg/complex/algorithms/edge_collapse.h \
 ../vcglib/vcg/simplex/face/topology.h quadric_simp.h \
 ../vcglib/vcg/complex/algorithms/update/position.h \
 ../vcglib/vcg/complex/algorithms/update/normal.h \
 ../vcglib/vcg/complex/algorithms/polygon_support.h \
 ../vcglib/vcg/simplex/face/jumping_pos.h \
 ../vcglib/vcg/space/planar_polygon_tessellation.h \
 ../vcglib/vcg/math/random_generator.h \
 ../vcglib/vcg/complex/algorithms/update/bounding.h \
 ../vcglib/vcg/complex/algorithms/clean.h \
 ../vcglib/vcg/simplex/edge/topology.h ../vcglib/vcg/simplex/edge/pos.h \
 ../vcglib/vcg/complex/algorithms/closest.h \
 ../vcglib/vcg/space/index/grid_util.h ../vcglib/vcg/space/index/base.h \
 ../vcglib/vcg/space/ray3.h ../vcglib/vcg/space/index/space_iterators.h \
 ../vcglib/vcg/space/intersection3.h ../vcglib/vcg/space/sphere3.h \
 ../vcglib/vcg/space/intersection/triangle_triangle3.h \
 ../vcglib/vcg/space/index/grid_closest.h \
 ../vcglib/vcg/simplex/face/distance.h ../vcglib/vcg/space/distance3.h \
 ../vcglib/vcg/simplex/edge/distance.h \
 ../vcglib/vcg/simplex/vertex/distance.h \
 ../vcglib/vcg/space/index/grid_static_ptr.h \
 ../vcglib/vcg/space/index/spatial_hashing.h \
 ../vcglib/vcg/complex/algorithms/update/normal.h \
 ../vcglib/vcg/complex/algorithms/update/topology.h \
 ../vcglib/vcg/complex/algorithms/local_optimization.h \
 ../vcglib/vcg/complex/algorithms/local_optimization/tri_edge_collapse_quadric.h \
 ../vcglib/vcg/math/quadric.h \
 ../vcglib/vcg/complex/algorithms/local_optimization/tri_edge_collapse.h \
 ../vcglib/vcg/complex/algorithms/stat.h ../vcglib/vcg/math/histogram.h \
 ../vcglib/vcg/complex/algorithms/inertia.h ml_mesh_type.h
SFBool.o: SFBool.cpp stdafx.h config.h mysnprintf.h DuneApp.h List.h \
 Array.h MyString.h StringArray.h Node.h Vec3f.h Vec3x.h FieldIndex.h \
 Element.h x3dFlags.h swt/include/WriteFlags.h NodeFlags.h MeshFlags.h \
 ac3dMaterialCallback.h NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h \
 URL.h PreferencesApp.h swt/include/swttypedef.h EcmaScriptApp.h \
 StereoViewApp.h swt/include/stereoview.h InputDeviceApp.h \
 TransformMode.h OutputApp.h RouteViewApp.h StartWithApp.h \
 ExportNamesApp.h ModellingApp.h RenderApp.h SFBool.h FieldValue.h \
 Arrays.h MySwap.h Types.h
SFBoolItem.o: SFBoolItem.cpp SFBoolItem.h FieldViewItem.h Array.h \
 stdafx.h config.h mysnprintf.h Rect.h Point.h MyString.h StringArray.h \
 Field.h Element.h x3dFlags.h swt/include/WriteFlags.h Node.h List.h \
 Vec3f.h Vec3x.h FieldIndex.h NodeFlags.h MeshFlags.h \
 ac3dMaterialCallback.h NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h \
 Types.h swt/include/swttypedef.h swt/include/swt.h \
 swt/include/keycodes.h swt/include/swlanguages.h swt/include/swDebugf.h \
 swt/include/swttypedef.h FieldView.h SceneView.h DuneApp.h URL.h \
 PreferencesApp.h EcmaScriptApp.h StereoViewApp.h \
 swt/include/stereoview.h InputDeviceApp.h TransformMode.h OutputApp.h \
 RouteViewApp.h StartWithApp.h ExportNamesApp.h ModellingApp.h \
 RenderApp.h Scene.h Map.h CommandList.h Command.h NodeList.h Proto.h \
 ProtoMacros.h InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h MFBool.h \
 FieldValue.h Arrays.h MySwap.h MFColor.h MFFloat.h SFFloat.h \
 MFColorRGBA.h MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h \
 MFNode.h MFRotation.h MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h \
 MFVec2f.h MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h \
 SFColorRGBA.h SFImage.h SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h \
 SFRotation.h SFString.h SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h \
 SFVec3f.h SFVec4f.h RenderState.h ProtoMap.h Path.h FieldCommand.h
SFColor.o: SFColor.cpp stdafx.h config.h mysnprintf.h DuneApp.h List.h \
 Array.h MyString.h StringArray.h Node.h Vec3f.h Vec3x.h FieldIndex.h \
 Element.h x3dFlags.h swt/include/WriteFlags.h NodeFlags.h MeshFlags.h \
 ac3dMaterialCallback.h NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h \
 URL.h PreferencesApp.h swt/include/swttypedef.h EcmaScriptApp.h \
 StereoViewApp.h swt/include/stereoview.h InputDeviceApp.h \
 TransformMode.h OutputApp.h RouteViewApp.h StartWithApp.h \
 ExportNamesApp.h ModellingApp.h RenderApp.h SFColor.h FieldValue.h \
 Arrays.h MySwap.h Types.h SFFloat.h
SFColorItem.o: SFColorItem.cpp SFColorItem.h FieldViewItem.h Array.h \
 stdafx.h config.h mysnprintf.h Rect.h Point.h MyString.h StringArray.h \
 Field.h Element.h x3dFlags.h swt/include/WriteFlags.h Node.h List.h \
 Vec3f.h Vec3x.h FieldIndex.h NodeFlags.h MeshFlags.h \
 ac3dMaterialCallback.h NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h \
 Types.h swt/include/swttypedef.h SFColor.h FieldValue.h Arrays.h \
 MySwap.h FieldView.h SceneView.h DuneApp.h URL.h PreferencesApp.h \
 EcmaScriptApp.h StereoViewApp.h swt/include/stereoview.h \
 InputDeviceApp.h TransformMode.h OutputApp.h RouteViewApp.h \
 StartWithApp.h ExportNamesApp.h ModellingApp.h RenderApp.h Scene.h Map.h \
 CommandList.h Command.h NodeList.h Proto.h ProtoMacros.h \
 InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h MFBool.h MFColor.h \
 MFFloat.h SFFloat.h MFColorRGBA.h MFDouble.h SFDouble.h MFInt32.h \
 MFMatrix3f.h MFMatrix4f.h MFNode.h MFRotation.h MFString.h MFTime.h \
 SFTime.h MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h MFVec3f.h MFVec4f.h \
 Vec4f.h SFBool.h SFColorRGBA.h SFImage.h SFInt32.h SFMatrix3f.h \
 SFMatrix4f.h SFNode.h SFRotation.h SFString.h SFVec2d.h SFVec2f.h \
 Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h RenderState.h ProtoMap.h \
 swt/include/swt.h swt/include/keycodes.h swt/include/swlanguages.h \
 swt/include/swDebugf.h swt/include/swttypedef.h
SFColorRGBA.o: SFColorRGBA.cpp stdafx.h config.h mysnprintf.h DuneApp.h \
 List.h Array.h MyString.h StringArray.h Node.h Vec3f.h Vec3x.h \
 FieldIndex.h Element.h x3dFlags.h swt/include/WriteFlags.h NodeFlags.h \
 MeshFlags.h ac3dMaterialCallback.h NodeArray.h Stack.h Matrix.h Vec3d.h \
 Quaternion.h URL.h PreferencesApp.h swt/include/swttypedef.h \
 EcmaScriptApp.h StereoViewApp.h swt/include/stereoview.h \
 InputDeviceApp.h TransformMode.h OutputApp.h RouteViewApp.h \
 StartWithApp.h ExportNamesApp.h ModellingApp.h RenderApp.h Vec4f.h \
 SFColorRGBA.h FieldValue.h Arrays.h MySwap.h Types.h SFFloat.h
SFColorRGBAItem.o: SFColorRGBAItem.cpp SFColorRGBAItem.h FieldViewItem.h \
 Array.h stdafx.h config.h mysnprintf.h Rect.h Point.h MyString.h \
 StringArray.h Field.h Element.h x3dFlags.h swt/include/WriteFlags.h \
 Node.h List.h Vec3f.h Vec3x.h FieldIndex.h NodeFlags.h MeshFlags.h \
 ac3dMaterialCallback.h NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h \
 Types.h swt/include/swttypedef.h SFColorRGBA.h FieldValue.h Arrays.h \
 MySwap.h FieldView.h SceneView.h DuneApp.h URL.h PreferencesApp.h \
 EcmaScriptApp.h StereoViewApp.h swt/include/stereoview.h \
 InputDeviceApp.h TransformMode.h OutputApp.h RouteViewApp.h \
 StartWithApp.h ExportNamesApp.h ModellingApp.h RenderApp.h Scene.h Map.h \
 CommandList.h Command.h NodeList.h Proto.h ProtoMacros.h \
 InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h MFBool.h MFColor.h \
 MFFloat.h SFFloat.h MFColorRGBA.h MFDouble.h SFDouble.h MFInt32.h \
 MFMatrix3f.h MFMatrix4f.h MFNode.h MFRotation.h MFString.h MFTime.h \
 SFTime.h MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h MFVec3f.h MFVec4f.h \
 Vec4f.h SFBool.h SFColor.h SFImage.h SFInt32.h SFMatrix3f.h SFMatrix4f.h \
 SFNode.h SFRotation.h SFString.h SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h \
 SfVec3x.h SFVec3f.h SFVec4f.h RenderState.h ProtoMap.h swt/include/swt.h \
 swt/include/keycodes.h swt/include/swlanguages.h swt/include/swDebugf.h \
 swt/include/swttypedef.h
SFDouble.o: SFDouble.cpp stdafx.h config.h mysnprintf.h DuneApp.h List.h \
 Array.h MyString.h StringArray.h Node.h Vec3f.h Vec3x.h FieldIndex.h \
 Element.h x3dFlags.h swt/include/WriteFlags.h NodeFlags.h MeshFlags.h \
 ac3dMaterialCallback.h NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h \
 URL.h PreferencesApp.h swt/include/swttypedef.h EcmaScriptApp.h \
 StereoViewApp.h swt/include/stereoview.h InputDeviceApp.h \
 TransformMode.h OutputApp.h RouteViewApp.h StartWithApp.h \
 ExportNamesApp.h ModellingApp.h RenderApp.h SFDouble.h FieldValue.h \
 Arrays.h MySwap.h Types.h SFString.h
SFDoubleItem.o: SFDoubleItem.cpp SFDoubleItem.h FieldViewItem.h Array.h \
 stdafx.h config.h mysnprintf.h Rect.h Point.h MyString.h StringArray.h \
 Field.h Element.h x3dFlags.h swt/include/WriteFlags.h Node.h List.h \
 Vec3f.h Vec3x.h FieldIndex.h NodeFlags.h MeshFlags.h \
 ac3dMaterialCallback.h NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h \
 Types.h swt/include/swttypedef.h SFDouble.h FieldValue.h Arrays.h \
 MySwap.h FieldView.h SceneView.h DuneApp.h URL.h PreferencesApp.h \
 EcmaScriptApp.h StereoViewApp.h swt/include/stereoview.h \
 InputDeviceApp.h TransformMode.h OutputApp.h RouteViewApp.h \
 StartWithApp.h ExportNamesApp.h ModellingApp.h RenderApp.h Scene.h Map.h \
 CommandList.h Command.h NodeList.h Proto.h ProtoMacros.h \
 InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h MFBool.h MFColor.h \
 MFFloat.h SFFloat.h MFColorRGBA.h MFDouble.h MFInt32.h MFMatrix3f.h \
 MFMatrix4f.h MFNode.h MFRotation.h MFString.h MFTime.h SFTime.h \
 MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h \
 SFBool.h SFColor.h SFColorRGBA.h SFImage.h SFInt32.h SFMatrix3f.h \
 SFMatrix4f.h SFNode.h SFRotation.h SFString.h SFVec2d.h SFVec2f.h \
 Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h RenderState.h ProtoMap.h \
 swt/include/swt.h swt/include/keycodes.h swt/include/swlanguages.h \
 swt/include/swDebugf.h swt/include/swttypedef.h
SFFloat.o: SFFloat.cpp stdafx.h config.h mysnprintf.h DuneApp.h List.h \
 Array.h MyString.h StringArray.h Node.h Vec3f.h Vec3x.h FieldIndex.h \
 Element.h x3dFlags.h swt/include/WriteFlags.h NodeFlags.h MeshFlags.h \
 ac3dMaterialCallback.h NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h \
 URL.h PreferencesApp.h swt/include/swttypedef.h EcmaScriptApp.h \
 StereoViewApp.h swt/include/stereoview.h InputDeviceApp.h \
 TransformMode.h OutputApp.h RouteViewApp.h StartWithApp.h \
 ExportNamesApp.h ModellingApp.h RenderApp.h SFFloat.h FieldValue.h \
 Arrays.h MySwap.h Types.h
SFFloatItem.o: SFFloatItem.cpp SFFloatItem.h FieldViewItem.h Array.h \
 stdafx.h config.h mysnprintf.h Rect.h Point.h MyString.h StringArray.h \
 Field.h Element.h x3dFlags.h swt/include/WriteFlags.h Node.h List.h \
 Vec3f.h Vec3x.h FieldIndex.h NodeFlags.h MeshFlags.h \
 ac3dMaterialCallback.h NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h \
 Types.h swt/include/swttypedef.h SFFloat.h FieldValue.h Arrays.h \
 MySwap.h FieldView.h SceneView.h DuneApp.h URL.h PreferencesApp.h \
 EcmaScriptApp.h StereoViewApp.h swt/include/stereoview.h \
 InputDeviceApp.h TransformMode.h OutputApp.h RouteViewApp.h \
 StartWithApp.h ExportNamesApp.h ModellingApp.h RenderApp.h Scene.h Map.h \
 CommandList.h Command.h NodeList.h Proto.h ProtoMacros.h \
 InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h MFBool.h MFColor.h \
 MFFloat.h MFColorRGBA.h MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h \
 MFMatrix4f.h MFNode.h MFRotation.h MFString.h MFTime.h SFTime.h \
 MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h \
 SFBool.h SFColor.h SFColorRGBA.h SFImage.h SFInt32.h SFMatrix3f.h \
 SFMatrix4f.h SFNode.h SFRotation.h SFString.h SFVec2d.h SFVec2f.h \
 Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h RenderState.h ProtoMap.h \
 swt/include/swt.h swt/include/keycodes.h swt/include/swlanguages.h \
 swt/include/swDebugf.h swt/include/swttypedef.h
SFImage.o: SFImage.cpp stdafx.h config.h mysnprintf.h SFImage.h \
 FieldValue.h Array.h MyString.h StringArray.h Element.h x3dFlags.h \
 swt/include/WriteFlags.h Arrays.h MySwap.h Types.h MFInt32.h DuneApp.h \
 List.h Node.h Vec3f.h Vec3x.h FieldIndex.h NodeFlags.h MeshFlags.h \
 ac3dMaterialCallback.h NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h \
 URL.h PreferencesApp.h swt/include/swttypedef.h EcmaScriptApp.h \
 StereoViewApp.h swt/include/stereoview.h InputDeviceApp.h \
 TransformMode.h OutputApp.h RouteViewApp.h StartWithApp.h \
 ExportNamesApp.h ModellingApp.h RenderApp.h
SFImageItem.o: SFImageItem.cpp SFImageItem.h MFInt32Item.h \
 FieldViewItem.h Array.h stdafx.h config.h mysnprintf.h Rect.h Point.h \
 MyString.h StringArray.h Field.h Element.h x3dFlags.h \
 swt/include/WriteFlags.h Node.h List.h Vec3f.h Vec3x.h FieldIndex.h \
 NodeFlags.h MeshFlags.h ac3dMaterialCallback.h NodeArray.h Stack.h \
 Matrix.h Vec3d.h Quaternion.h Types.h swt/include/swttypedef.h SFImage.h \
 FieldValue.h Arrays.h MySwap.h MFInt32.h SFInt32Item.h SFInt32.h \
 FieldView.h SceneView.h DuneApp.h URL.h PreferencesApp.h EcmaScriptApp.h \
 StereoViewApp.h swt/include/stereoview.h InputDeviceApp.h \
 TransformMode.h OutputApp.h RouteViewApp.h StartWithApp.h \
 ExportNamesApp.h ModellingApp.h RenderApp.h Scene.h Map.h CommandList.h \
 Command.h NodeList.h Proto.h ProtoMacros.h InterfaceArray.h \
 DynamicFieldsNode.h SFMFTypes.h MFBool.h MFColor.h MFFloat.h SFFloat.h \
 MFColorRGBA.h MFDouble.h SFDouble.h MFMatrix3f.h MFMatrix4f.h MFNode.h \
 MFRotation.h MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h \
 MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h \
 SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h SFString.h SFVec2d.h \
 SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h RenderState.h \
 ProtoMap.h swt/include/swt.h swt/include/keycodes.h \
 swt/include/swlanguages.h swt/include/swDebugf.h \
 swt/include/swttypedef.h
SFInt32.o: SFInt32.cpp stdafx.h config.h mysnprintf.h DuneApp.h List.h \
 Array.h MyString.h StringArray.h Node.h Vec3f.h Vec3x.h FieldIndex.h \
 Element.h x3dFlags.h swt/include/WriteFlags.h NodeFlags.h MeshFlags.h \
 ac3dMaterialCallback.h NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h \
 URL.h PreferencesApp.h swt/include/swttypedef.h EcmaScriptApp.h \
 StereoViewApp.h swt/include/stereoview.h InputDeviceApp.h \
 TransformMode.h OutputApp.h RouteViewApp.h StartWithApp.h \
 ExportNamesApp.h ModellingApp.h RenderApp.h SFInt32.h FieldValue.h \
 Arrays.h MySwap.h Types.h
SFInt32Item.o: SFInt32Item.cpp SFInt32Item.h FieldViewItem.h Array.h \
 stdafx.h config.h mysnprintf.h Rect.h Point.h MyString.h StringArray.h \
 Field.h Element.h x3dFlags.h swt/include/WriteFlags.h Node.h List.h \
 Vec3f.h Vec3x.h FieldIndex.h NodeFlags.h MeshFlags.h \
 ac3dMaterialCallback.h NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h \
 Types.h swt/include/swttypedef.h SFInt32.h FieldValue.h Arrays.h \
 MySwap.h FieldView.h SceneView.h DuneApp.h URL.h PreferencesApp.h \
 EcmaScriptApp.h StereoViewApp.h swt/include/stereoview.h \
 InputDeviceApp.h TransformMode.h OutputApp.h RouteViewApp.h \
 StartWithApp.h ExportNamesApp.h ModellingApp.h RenderApp.h Scene.h Map.h \
 CommandList.h Command.h NodeList.h Proto.h ProtoMacros.h \
 InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h MFBool.h MFColor.h \
 MFFloat.h SFFloat.h MFColorRGBA.h MFDouble.h SFDouble.h MFInt32.h \
 MFMatrix3f.h MFMatrix4f.h MFNode.h MFRotation.h MFString.h MFTime.h \
 SFTime.h MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h MFVec3f.h MFVec4f.h \
 Vec4f.h SFBool.h SFColor.h SFColorRGBA.h SFImage.h SFMatrix3f.h \
 SFMatrix4f.h SFNode.h SFRotation.h SFString.h SFVec2d.h SFVec2f.h \
 Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h RenderState.h ProtoMap.h \
 swt/include/swt.h swt/include/keycodes.h swt/include/swlanguages.h \
 swt/include/swDebugf.h swt/include/swttypedef.h
SFMatrix3f.o: SFMatrix3f.cpp stdafx.h config.h mysnprintf.h DuneApp.h \
 List.h Array.h MyString.h StringArray.h Node.h Vec3f.h Vec3x.h \
 FieldIndex.h Element.h x3dFlags.h swt/include/WriteFlags.h NodeFlags.h \
 MeshFlags.h ac3dMaterialCallback.h NodeArray.h Stack.h Matrix.h Vec3d.h \
 Quaternion.h URL.h PreferencesApp.h swt/include/swttypedef.h \
 EcmaScriptApp.h StereoViewApp.h swt/include/stereoview.h \
 InputDeviceApp.h TransformMode.h OutputApp.h RouteViewApp.h \
 StartWithApp.h ExportNamesApp.h ModellingApp.h RenderApp.h \
 swt/include/swt.h swt/include/keycodes.h swt/include/swlanguages.h \
 swt/include/swDebugf.h swt/include/swttypedef.h SFMatrix3f.h \
 FieldValue.h Arrays.h MySwap.h Types.h SFFloat.h
SFMatrix3fItem.o: SFMatrix3fItem.cpp SFMatrix3fItem.h FieldViewItem.h \
 Array.h stdafx.h config.h mysnprintf.h Rect.h Point.h MyString.h \
 StringArray.h Field.h Element.h x3dFlags.h swt/include/WriteFlags.h \
 Node.h List.h Vec3f.h Vec3x.h FieldIndex.h NodeFlags.h MeshFlags.h \
 ac3dMaterialCallback.h NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h \
 Types.h swt/include/swttypedef.h SFMatrix3f.h FieldValue.h Arrays.h \
 MySwap.h FieldView.h SceneView.h DuneApp.h URL.h PreferencesApp.h \
 EcmaScriptApp.h StereoViewApp.h swt/include/stereoview.h \
 InputDeviceApp.h TransformMode.h OutputApp.h RouteViewApp.h \
 StartWithApp.h ExportNamesApp.h ModellingApp.h RenderApp.h Scene.h Map.h \
 CommandList.h Command.h NodeList.h Proto.h ProtoMacros.h \
 InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h MFBool.h MFColor.h \
 MFFloat.h SFFloat.h MFColorRGBA.h MFDouble.h SFDouble.h MFInt32.h \
 MFMatrix3f.h MFMatrix4f.h MFNode.h MFRotation.h MFString.h MFTime.h \
 SFTime.h MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h MFVec3f.h MFVec4f.h \
 Vec4f.h SFBool.h SFColor.h SFColorRGBA.h SFImage.h SFInt32.h \
 SFMatrix4f.h SFNode.h SFRotation.h SFString.h SFVec2d.h SFVec2f.h \
 Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h RenderState.h ProtoMap.h \
 swt/include/swt.h swt/include/keycodes.h swt/include/swlanguages.h \
 swt/include/swDebugf.h swt/include/swttypedef.h
SFMatrix4f.o: SFMatrix4f.cpp stdafx.h config.h mysnprintf.h DuneApp.h \
 List.h Array.h MyString.h StringArray.h Node.h Vec3f.h Vec3x.h \
 FieldIndex.h Element.h x3dFlags.h swt/include/WriteFlags.h NodeFlags.h \
 MeshFlags.h ac3dMaterialCallback.h NodeArray.h Stack.h Matrix.h Vec3d.h \
 Quaternion.h URL.h PreferencesApp.h swt/include/swttypedef.h \
 EcmaScriptApp.h StereoViewApp.h swt/include/stereoview.h \
 InputDeviceApp.h TransformMode.h OutputApp.h RouteViewApp.h \
 StartWithApp.h ExportNamesApp.h ModellingApp.h RenderApp.h \
 swt/include/swt.h swt/include/keycodes.h swt/include/swlanguages.h \
 swt/include/swDebugf.h swt/include/swttypedef.h SFMatrix4f.h \
 FieldValue.h Arrays.h MySwap.h Types.h SFFloat.h
SFMatrix4fItem.o: SFMatrix4fItem.cpp SFMatrix4fItem.h FieldViewItem.h \
 Array.h stdafx.h config.h mysnprintf.h Rect.h Point.h MyString.h \
 StringArray.h Field.h Element.h x3dFlags.h swt/include/WriteFlags.h \
 Node.h List.h Vec3f.h Vec3x.h FieldIndex.h NodeFlags.h MeshFlags.h \
 ac3dMaterialCallback.h NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h \
 Types.h swt/include/swttypedef.h SFMatrix4f.h FieldValue.h Arrays.h \
 MySwap.h FieldView.h SceneView.h DuneApp.h URL.h PreferencesApp.h \
 EcmaScriptApp.h StereoViewApp.h swt/include/stereoview.h \
 InputDeviceApp.h TransformMode.h OutputApp.h RouteViewApp.h \
 StartWithApp.h ExportNamesApp.h ModellingApp.h RenderApp.h Scene.h Map.h \
 CommandList.h Command.h NodeList.h Proto.h ProtoMacros.h \
 InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h MFBool.h MFColor.h \
 MFFloat.h SFFloat.h MFColorRGBA.h MFDouble.h SFDouble.h MFInt32.h \
 MFMatrix3f.h MFMatrix4f.h MFNode.h MFRotation.h MFString.h MFTime.h \
 SFTime.h MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h MFVec3f.h MFVec4f.h \
 Vec4f.h SFBool.h SFColor.h SFColorRGBA.h SFImage.h SFInt32.h \
 SFMatrix3f.h SFNode.h SFRotation.h SFString.h SFVec2d.h SFVec2f.h \
 Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h RenderState.h ProtoMap.h \
 swt/include/swt.h swt/include/keycodes.h swt/include/swlanguages.h \
 swt/include/swDebugf.h swt/include/swttypedef.h
SFNode.o: SFNode.cpp stdafx.h config.h mysnprintf.h Proto.h Array.h Map.h \
 List.h MyString.h StringArray.h Node.h Vec3f.h Vec3x.h FieldIndex.h \
 Element.h x3dFlags.h swt/include/WriteFlags.h NodeFlags.h MeshFlags.h \
 ac3dMaterialCallback.h NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h \
 ProtoMacros.h NodeList.h InterfaceArray.h DynamicFieldsNode.h \
 SFMFTypes.h MFBool.h FieldValue.h Arrays.h MySwap.h Types.h MFColor.h \
 MFFloat.h SFFloat.h MFColorRGBA.h MFDouble.h SFDouble.h MFInt32.h \
 MFMatrix3f.h MFMatrix4f.h MFNode.h MFRotation.h MFString.h MFTime.h \
 SFTime.h MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h MFVec3f.h MFVec4f.h \
 Vec4f.h SFBool.h SFColor.h SFColorRGBA.h SFImage.h SFInt32.h \
 SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h SFString.h SFVec2d.h \
 SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h CommandList.h \
 Command.h DuneApp.h URL.h PreferencesApp.h swt/include/swttypedef.h \
 EcmaScriptApp.h StereoViewApp.h swt/include/stereoview.h \
 InputDeviceApp.h TransformMode.h OutputApp.h RouteViewApp.h \
 StartWithApp.h ExportNamesApp.h ModellingApp.h RenderApp.h \
 MeshBasedNode.h GeometryNode.h NodeAppearance.h NodeMaterial.h \
 swt/include/swt.h swt/include/keycodes.h swt/include/swlanguages.h \
 swt/include/swDebugf.h swt/include/swttypedef.h NodeImageTexture.h \
 KambiTextureCommonFields.h NodeTextureTransform.h GeometryNodeMacros.h \
 Scene.h RenderState.h ProtoMap.h
SFNodeItem.o: SFNodeItem.cpp SFNodeItem.h FieldViewItem.h Array.h \
 stdafx.h config.h mysnprintf.h Rect.h Point.h MyString.h StringArray.h \
 Field.h Element.h x3dFlags.h swt/include/WriteFlags.h Node.h List.h \
 Vec3f.h Vec3x.h FieldIndex.h NodeFlags.h MeshFlags.h \
 ac3dMaterialCallback.h NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h \
 Types.h swt/include/swttypedef.h SFNode.h FieldValue.h Arrays.h MySwap.h \
 FieldView.h SceneView.h DuneApp.h URL.h PreferencesApp.h EcmaScriptApp.h \
 StereoViewApp.h swt/include/stereoview.h InputDeviceApp.h \
 TransformMode.h OutputApp.h RouteViewApp.h StartWithApp.h \
 ExportNamesApp.h ModellingApp.h RenderApp.h Scene.h Map.h CommandList.h \
 Command.h NodeList.h Proto.h ProtoMacros.h InterfaceArray.h \
 DynamicFieldsNode.h SFMFTypes.h MFBool.h MFColor.h MFFloat.h SFFloat.h \
 MFColorRGBA.h MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h \
 MFNode.h MFRotation.h MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h \
 MFVec2f.h MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h \
 SFColorRGBA.h SFImage.h SFInt32.h SFMatrix3f.h SFMatrix4f.h SFRotation.h \
 SFString.h SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h \
 SFVec4f.h RenderState.h ProtoMap.h swt/include/swt.h \
 swt/include/keycodes.h swt/include/swlanguages.h swt/include/swDebugf.h \
 swt/include/swttypedef.h
SFRotation.o: SFRotation.cpp stdafx.h config.h mysnprintf.h \
 swt/include/swt.h swt/include/keycodes.h swt/include/swlanguages.h \
 swt/include/swDebugf.h swt/include/swttypedef.h DuneApp.h List.h Array.h \
 MyString.h StringArray.h Node.h Vec3f.h Vec3x.h FieldIndex.h Element.h \
 x3dFlags.h swt/include/WriteFlags.h NodeFlags.h MeshFlags.h \
 ac3dMaterialCallback.h NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h \
 URL.h PreferencesApp.h swt/include/swttypedef.h EcmaScriptApp.h \
 StereoViewApp.h swt/include/stereoview.h InputDeviceApp.h \
 TransformMode.h OutputApp.h RouteViewApp.h StartWithApp.h \
 ExportNamesApp.h ModellingApp.h RenderApp.h Vec4f.h SFRotation.h \
 FieldValue.h Arrays.h MySwap.h Types.h EulerAngles.h QuatTypes.h
SFRotationItem.o: SFRotationItem.cpp SFRotationItem.h FieldViewItem.h \
 Array.h stdafx.h config.h mysnprintf.h Rect.h Point.h MyString.h \
 StringArray.h Field.h Element.h x3dFlags.h swt/include/WriteFlags.h \
 Node.h List.h Vec3f.h Vec3x.h FieldIndex.h NodeFlags.h MeshFlags.h \
 ac3dMaterialCallback.h NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h \
 Types.h swt/include/swttypedef.h SFRotation.h FieldValue.h Arrays.h \
 MySwap.h FieldView.h SceneView.h DuneApp.h URL.h PreferencesApp.h \
 EcmaScriptApp.h StereoViewApp.h swt/include/stereoview.h \
 InputDeviceApp.h TransformMode.h OutputApp.h RouteViewApp.h \
 StartWithApp.h ExportNamesApp.h ModellingApp.h RenderApp.h Scene.h Map.h \
 CommandList.h Command.h NodeList.h Proto.h ProtoMacros.h \
 InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h MFBool.h MFColor.h \
 MFFloat.h SFFloat.h MFColorRGBA.h MFDouble.h SFDouble.h MFInt32.h \
 MFMatrix3f.h MFMatrix4f.h MFNode.h MFRotation.h MFString.h MFTime.h \
 SFTime.h MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h MFVec3f.h MFVec4f.h \
 Vec4f.h SFBool.h SFColor.h SFColorRGBA.h SFImage.h SFInt32.h \
 SFMatrix3f.h SFMatrix4f.h SFNode.h SFString.h SFVec2d.h SFVec2f.h \
 Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h RenderState.h ProtoMap.h
SFString.o: SFString.cpp stdafx.h config.h mysnprintf.h SFString.h \
 FieldValue.h Array.h MyString.h StringArray.h Element.h x3dFlags.h \
 swt/include/WriteFlags.h Arrays.h MySwap.h Types.h DuneApp.h List.h \
 Node.h Vec3f.h Vec3x.h FieldIndex.h NodeFlags.h MeshFlags.h \
 ac3dMaterialCallback.h NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h \
 URL.h PreferencesApp.h swt/include/swttypedef.h EcmaScriptApp.h \
 StereoViewApp.h swt/include/stereoview.h InputDeviceApp.h \
 TransformMode.h OutputApp.h RouteViewApp.h StartWithApp.h \
 ExportNamesApp.h ModellingApp.h RenderApp.h
SFStringItem.o: SFStringItem.cpp SFStringItem.h FieldViewItem.h Array.h \
 stdafx.h config.h mysnprintf.h Rect.h Point.h MyString.h StringArray.h \
 Field.h Element.h x3dFlags.h swt/include/WriteFlags.h Node.h List.h \
 Vec3f.h Vec3x.h FieldIndex.h NodeFlags.h MeshFlags.h \
 ac3dMaterialCallback.h NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h \
 Types.h swt/include/swttypedef.h SFString.h FieldValue.h Arrays.h \
 MySwap.h FieldView.h SceneView.h DuneApp.h URL.h PreferencesApp.h \
 EcmaScriptApp.h StereoViewApp.h swt/include/stereoview.h \
 InputDeviceApp.h TransformMode.h OutputApp.h RouteViewApp.h \
 StartWithApp.h ExportNamesApp.h ModellingApp.h RenderApp.h Scene.h Map.h \
 CommandList.h Command.h NodeList.h Proto.h ProtoMacros.h \
 InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h MFBool.h MFColor.h \
 MFFloat.h SFFloat.h MFColorRGBA.h MFDouble.h SFDouble.h MFInt32.h \
 MFMatrix3f.h MFMatrix4f.h MFNode.h MFRotation.h MFString.h MFTime.h \
 SFTime.h MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h MFVec3f.h MFVec4f.h \
 Vec4f.h SFBool.h SFColor.h SFColorRGBA.h SFImage.h SFInt32.h \
 SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h SFVec2d.h SFVec2f.h \
 Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h RenderState.h ProtoMap.h \
 swt/include/swt.h swt/include/keycodes.h swt/include/swlanguages.h \
 swt/include/swDebugf.h swt/include/swttypedef.h
SFTime.o: SFTime.cpp stdafx.h config.h mysnprintf.h DuneApp.h List.h \
 Array.h MyString.h StringArray.h Node.h Vec3f.h Vec3x.h FieldIndex.h \
 Element.h x3dFlags.h swt/include/WriteFlags.h NodeFlags.h MeshFlags.h \
 ac3dMaterialCallback.h NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h \
 URL.h PreferencesApp.h swt/include/swttypedef.h EcmaScriptApp.h \
 StereoViewApp.h swt/include/stereoview.h InputDeviceApp.h \
 TransformMode.h OutputApp.h RouteViewApp.h StartWithApp.h \
 ExportNamesApp.h ModellingApp.h RenderApp.h SFTime.h FieldValue.h \
 Arrays.h MySwap.h Types.h
SFTimeItem.o: SFTimeItem.cpp SFTimeItem.h FieldViewItem.h Array.h \
 stdafx.h config.h mysnprintf.h Rect.h Point.h MyString.h StringArray.h \
 Field.h Element.h x3dFlags.h swt/include/WriteFlags.h Node.h List.h \
 Vec3f.h Vec3x.h FieldIndex.h NodeFlags.h MeshFlags.h \
 ac3dMaterialCallback.h NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h \
 Types.h swt/include/swttypedef.h SFTime.h FieldValue.h Arrays.h MySwap.h \
 FieldView.h SceneView.h DuneApp.h URL.h PreferencesApp.h EcmaScriptApp.h \
 StereoViewApp.h swt/include/stereoview.h InputDeviceApp.h \
 TransformMode.h OutputApp.h RouteViewApp.h StartWithApp.h \
 ExportNamesApp.h ModellingApp.h RenderApp.h Scene.h Map.h CommandList.h \
 Command.h NodeList.h Proto.h ProtoMacros.h InterfaceArray.h \
 DynamicFieldsNode.h SFMFTypes.h MFBool.h MFColor.h MFFloat.h SFFloat.h \
 MFColorRGBA.h MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h \
 MFNode.h MFRotation.h MFString.h MFTime.h MFVec2d.h Vec2d.h MFVec2f.h \
 MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h \
 SFImage.h SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h \
 SFString.h SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h \
 SFVec4f.h RenderState.h ProtoMap.h swt/include/swt.h \
 swt/include/keycodes.h swt/include/swlanguages.h swt/include/swDebugf.h \
 swt/include/swttypedef.h
SFVec2f.o: SFVec2f.cpp stdafx.h config.h mysnprintf.h DuneApp.h List.h \
 Array.h MyString.h StringArray.h Node.h Vec3f.h Vec3x.h FieldIndex.h \
 Element.h x3dFlags.h swt/include/WriteFlags.h NodeFlags.h MeshFlags.h \
 ac3dMaterialCallback.h NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h \
 URL.h PreferencesApp.h swt/include/swttypedef.h EcmaScriptApp.h \
 StereoViewApp.h swt/include/stereoview.h InputDeviceApp.h \
 TransformMode.h OutputApp.h RouteViewApp.h StartWithApp.h \
 ExportNamesApp.h ModellingApp.h RenderApp.h SFVec2f.h FieldValue.h \
 Arrays.h MySwap.h Types.h Vec2f.h
SFVec2fItem.o: SFVec2fItem.cpp SFVec2fItem.h FieldViewItem.h Array.h \
 stdafx.h config.h mysnprintf.h Rect.h Point.h MyString.h StringArray.h \
 Field.h Element.h x3dFlags.h swt/include/WriteFlags.h Node.h List.h \
 Vec3f.h Vec3x.h FieldIndex.h NodeFlags.h MeshFlags.h \
 ac3dMaterialCallback.h NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h \
 Types.h swt/include/swttypedef.h SFVec2f.h FieldValue.h Arrays.h \
 MySwap.h Vec2f.h FieldView.h SceneView.h DuneApp.h URL.h \
 PreferencesApp.h EcmaScriptApp.h StereoViewApp.h \
 swt/include/stereoview.h InputDeviceApp.h TransformMode.h OutputApp.h \
 RouteViewApp.h StartWithApp.h ExportNamesApp.h ModellingApp.h \
 RenderApp.h Scene.h Map.h CommandList.h Command.h NodeList.h Proto.h \
 ProtoMacros.h InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h MFBool.h \
 MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h MFDouble.h SFDouble.h \
 MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h MFRotation.h MFString.h \
 MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h MFVec3f.h \
 MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h SFImage.h SFInt32.h \
 SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h SFString.h SFVec2d.h \
 SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h RenderState.h ProtoMap.h \
 swt/include/swt.h swt/include/keycodes.h swt/include/swlanguages.h \
 swt/include/swDebugf.h swt/include/swttypedef.h
SFVec2d.o: SFVec2d.cpp stdafx.h config.h mysnprintf.h DuneApp.h List.h \
 Array.h MyString.h StringArray.h Node.h Vec3f.h Vec3x.h FieldIndex.h \
 Element.h x3dFlags.h swt/include/WriteFlags.h NodeFlags.h MeshFlags.h \
 ac3dMaterialCallback.h NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h \
 URL.h PreferencesApp.h swt/include/swttypedef.h EcmaScriptApp.h \
 StereoViewApp.h swt/include/stereoview.h InputDeviceApp.h \
 TransformMode.h OutputApp.h RouteViewApp.h StartWithApp.h \
 ExportNamesApp.h ModellingApp.h RenderApp.h SFVec2d.h FieldValue.h \
 Arrays.h MySwap.h Types.h
SFVec2dItem.o: SFVec2dItem.cpp SFVec2dItem.h FieldViewItem.h Array.h \
 stdafx.h config.h mysnprintf.h Rect.h Point.h MyString.h StringArray.h \
 Field.h Element.h x3dFlags.h swt/include/WriteFlags.h Node.h List.h \
 Vec3f.h Vec3x.h FieldIndex.h NodeFlags.h MeshFlags.h \
 ac3dMaterialCallback.h NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h \
 Types.h swt/include/swttypedef.h SFVec2d.h FieldValue.h Arrays.h \
 MySwap.h FieldView.h SceneView.h DuneApp.h URL.h PreferencesApp.h \
 EcmaScriptApp.h StereoViewApp.h swt/include/stereoview.h \
 InputDeviceApp.h TransformMode.h OutputApp.h RouteViewApp.h \
 StartWithApp.h ExportNamesApp.h ModellingApp.h RenderApp.h Scene.h Map.h \
 CommandList.h Command.h NodeList.h Proto.h ProtoMacros.h \
 InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h MFBool.h MFColor.h \
 MFFloat.h SFFloat.h MFColorRGBA.h MFDouble.h SFDouble.h MFInt32.h \
 MFMatrix3f.h MFMatrix4f.h MFNode.h MFRotation.h MFString.h MFTime.h \
 SFTime.h MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h MFVec3f.h MFVec4f.h \
 Vec4f.h SFBool.h SFColor.h SFColorRGBA.h SFImage.h SFInt32.h \
 SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h SFString.h SFVec2f.h \
 Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h RenderState.h ProtoMap.h \
 swt/include/swt.h swt/include/keycodes.h swt/include/swlanguages.h \
 swt/include/swDebugf.h swt/include/swttypedef.h
SFVec3f.o: SFVec3f.cpp stdafx.h config.h mysnprintf.h DuneApp.h List.h \
 Array.h MyString.h StringArray.h Node.h Vec3f.h Vec3x.h FieldIndex.h \
 Element.h x3dFlags.h swt/include/WriteFlags.h NodeFlags.h MeshFlags.h \
 ac3dMaterialCallback.h NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h \
 URL.h PreferencesApp.h swt/include/swttypedef.h EcmaScriptApp.h \
 StereoViewApp.h swt/include/stereoview.h InputDeviceApp.h \
 TransformMode.h OutputApp.h RouteViewApp.h StartWithApp.h \
 ExportNamesApp.h ModellingApp.h RenderApp.h swt/include/swt.h \
 swt/include/keycodes.h swt/include/swlanguages.h swt/include/swDebugf.h \
 swt/include/swttypedef.h SFVec3f.h SfVec3x.h FieldValue.h Arrays.h \
 MySwap.h Types.h SFFloat.h
SFVec3fItem.o: SFVec3fItem.cpp SFVec3fItem.h FieldViewItem.h Array.h \
 stdafx.h config.h mysnprintf.h Rect.h Point.h MyString.h StringArray.h \
 Field.h Element.h x3dFlags.h swt/include/WriteFlags.h Node.h List.h \
 Vec3f.h Vec3x.h FieldIndex.h NodeFlags.h MeshFlags.h \
 ac3dMaterialCallback.h NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h \
 Types.h swt/include/swttypedef.h SFVec3f.h SfVec3x.h FieldValue.h \
 Arrays.h MySwap.h FieldView.h SceneView.h DuneApp.h URL.h \
 PreferencesApp.h EcmaScriptApp.h StereoViewApp.h \
 swt/include/stereoview.h InputDeviceApp.h TransformMode.h OutputApp.h \
 RouteViewApp.h StartWithApp.h ExportNamesApp.h ModellingApp.h \
 RenderApp.h Scene.h Map.h CommandList.h Command.h NodeList.h Proto.h \
 ProtoMacros.h InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h MFBool.h \
 MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h MFDouble.h SFDouble.h \
 MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h MFRotation.h MFString.h \
 MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h MFVec3f.h \
 MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h SFImage.h SFInt32.h \
 SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h SFString.h SFVec2d.h \
 SFVec2f.h Vec2f.h SFVec3d.h SFVec4f.h RenderState.h ProtoMap.h \
 swt/include/swt.h swt/include/keycodes.h swt/include/swlanguages.h \
 swt/include/swDebugf.h swt/include/swttypedef.h
SFVec3d.o: SFVec3d.cpp stdafx.h config.h mysnprintf.h DuneApp.h List.h \
 Array.h MyString.h StringArray.h Node.h Vec3f.h Vec3x.h FieldIndex.h \
 Element.h x3dFlags.h swt/include/WriteFlags.h NodeFlags.h MeshFlags.h \
 ac3dMaterialCallback.h NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h \
 URL.h PreferencesApp.h swt/include/swttypedef.h EcmaScriptApp.h \
 StereoViewApp.h swt/include/stereoview.h InputDeviceApp.h \
 TransformMode.h OutputApp.h RouteViewApp.h StartWithApp.h \
 ExportNamesApp.h ModellingApp.h RenderApp.h SFVec3d.h SfVec3x.h \
 FieldValue.h Arrays.h MySwap.h Types.h SFString.h SFDouble.h
SFVec3dItem.o: SFVec3dItem.cpp SFVec3dItem.h FieldViewItem.h Array.h \
 stdafx.h config.h mysnprintf.h Rect.h Point.h MyString.h StringArray.h \
 Field.h Element.h x3dFlags.h swt/include/WriteFlags.h Node.h List.h \
 Vec3f.h Vec3x.h FieldIndex.h NodeFlags.h MeshFlags.h \
 ac3dMaterialCallback.h NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h \
 Types.h swt/include/swttypedef.h SFVec3d.h SfVec3x.h FieldValue.h \
 Arrays.h MySwap.h SFString.h FieldView.h SceneView.h DuneApp.h URL.h \
 PreferencesApp.h EcmaScriptApp.h StereoViewApp.h \
 swt/include/stereoview.h InputDeviceApp.h TransformMode.h OutputApp.h \
 RouteViewApp.h StartWithApp.h ExportNamesApp.h ModellingApp.h \
 RenderApp.h Scene.h Map.h CommandList.h Command.h NodeList.h Proto.h \
 ProtoMacros.h InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h MFBool.h \
 MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h MFDouble.h SFDouble.h \
 MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h MFRotation.h MFString.h \
 MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h MFVec3f.h \
 MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h SFImage.h SFInt32.h \
 SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h SFVec2d.h SFVec2f.h \
 Vec2f.h SFVec3f.h SFVec4f.h RenderState.h ProtoMap.h swt/include/swt.h \
 swt/include/keycodes.h swt/include/swlanguages.h swt/include/swDebugf.h \
 swt/include/swttypedef.h
SFVec4f.o: SFVec4f.cpp stdafx.h config.h mysnprintf.h DuneApp.h List.h \
 Array.h MyString.h StringArray.h Node.h Vec3f.h Vec3x.h FieldIndex.h \
 Element.h x3dFlags.h swt/include/WriteFlags.h NodeFlags.h MeshFlags.h \
 ac3dMaterialCallback.h NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h \
 URL.h PreferencesApp.h swt/include/swttypedef.h EcmaScriptApp.h \
 StereoViewApp.h swt/include/stereoview.h InputDeviceApp.h \
 TransformMode.h OutputApp.h RouteViewApp.h StartWithApp.h \
 ExportNamesApp.h ModellingApp.h RenderApp.h swt/include/swt.h \
 swt/include/keycodes.h swt/include/swlanguages.h swt/include/swDebugf.h \
 swt/include/swttypedef.h SFVec4f.h Vec4f.h FieldValue.h Arrays.h \
 MySwap.h Types.h SFFloat.h
SFVec4fItem.o: SFVec4fItem.cpp SFVec4fItem.h FieldViewItem.h Array.h \
 stdafx.h config.h mysnprintf.h Rect.h Point.h MyString.h StringArray.h \
 Field.h Element.h x3dFlags.h swt/include/WriteFlags.h Node.h List.h \
 Vec3f.h Vec3x.h FieldIndex.h NodeFlags.h MeshFlags.h \
 ac3dMaterialCallback.h NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h \
 Types.h swt/include/swttypedef.h SFVec4f.h Vec4f.h FieldValue.h Arrays.h \
 MySwap.h FieldView.h SceneView.h DuneApp.h URL.h PreferencesApp.h \
 EcmaScriptApp.h StereoViewApp.h swt/include/stereoview.h \
 InputDeviceApp.h TransformMode.h OutputApp.h RouteViewApp.h \
 StartWithApp.h ExportNamesApp.h ModellingApp.h RenderApp.h Scene.h Map.h \
 CommandList.h Command.h NodeList.h Proto.h ProtoMacros.h \
 InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h MFBool.h MFColor.h \
 MFFloat.h SFFloat.h MFColorRGBA.h MFDouble.h SFDouble.h MFInt32.h \
 MFMatrix3f.h MFMatrix4f.h MFNode.h MFRotation.h MFString.h MFTime.h \
 SFTime.h MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h MFVec3f.h MFVec4f.h \
 SFBool.h SFColor.h SFColorRGBA.h SFImage.h SFInt32.h SFMatrix3f.h \
 SFMatrix4f.h SFNode.h SFRotation.h SFString.h SFVec2d.h SFVec2f.h \
 Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h RenderState.h ProtoMap.h \
 swt/include/swt.h swt/include/keycodes.h swt/include/swlanguages.h \
 swt/include/swDebugf.h swt/include/swttypedef.h
MFBool.o: MFBool.cpp stdafx.h config.h mysnprintf.h MFBool.h Array.h \
 FieldValue.h MyString.h StringArray.h Element.h x3dFlags.h \
 swt/include/WriteFlags.h Arrays.h MySwap.h Types.h SFBool.h DuneApp.h \
 List.h Node.h Vec3f.h Vec3x.h FieldIndex.h NodeFlags.h MeshFlags.h \
 ac3dMaterialCallback.h NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h \
 URL.h PreferencesApp.h swt/include/swttypedef.h EcmaScriptApp.h \
 StereoViewApp.h swt/include/stereoview.h InputDeviceApp.h \
 TransformMode.h OutputApp.h RouteViewApp.h StartWithApp.h \
 ExportNamesApp.h ModellingApp.h RenderApp.h
MFBoolItem.o: MFBoolItem.cpp MFBoolItem.h FieldViewItem.h Array.h \
 stdafx.h config.h mysnprintf.h Rect.h Point.h MyString.h StringArray.h \
 Field.h Element.h x3dFlags.h swt/include/WriteFlags.h Node.h List.h \
 Vec3f.h Vec3x.h FieldIndex.h NodeFlags.h MeshFlags.h \
 ac3dMaterialCallback.h NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h \
 Types.h swt/include/swttypedef.h MFBool.h FieldValue.h Arrays.h MySwap.h \
 SFBoolItem.h swt/include/swt.h swt/include/keycodes.h \
 swt/include/swlanguages.h swt/include/swDebugf.h \
 swt/include/swttypedef.h SFBool.h FieldView.h SceneView.h DuneApp.h \
 URL.h PreferencesApp.h EcmaScriptApp.h StereoViewApp.h \
 swt/include/stereoview.h InputDeviceApp.h TransformMode.h OutputApp.h \
 RouteViewApp.h StartWithApp.h ExportNamesApp.h ModellingApp.h \
 RenderApp.h Scene.h Map.h CommandList.h Command.h NodeList.h Proto.h \
 ProtoMacros.h InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h MFColor.h \
 MFFloat.h SFFloat.h MFColorRGBA.h MFDouble.h SFDouble.h MFInt32.h \
 MFMatrix3f.h MFMatrix4f.h MFNode.h MFRotation.h MFString.h MFTime.h \
 SFTime.h MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h MFVec3f.h MFVec4f.h \
 Vec4f.h SFColor.h SFColorRGBA.h SFImage.h SFInt32.h SFMatrix3f.h \
 SFMatrix4f.h SFNode.h SFRotation.h SFString.h SFVec2d.h SFVec2f.h \
 Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h RenderState.h ProtoMap.h
MFColor.o: MFColor.cpp stdafx.h config.h mysnprintf.h MFColor.h MFFloat.h \
 Array.h FieldValue.h MyString.h StringArray.h Element.h x3dFlags.h \
 swt/include/WriteFlags.h Arrays.h MySwap.h Types.h SFFloat.h SFColor.h \
 DuneApp.h List.h Node.h Vec3f.h Vec3x.h FieldIndex.h NodeFlags.h \
 MeshFlags.h ac3dMaterialCallback.h NodeArray.h Stack.h Matrix.h Vec3d.h \
 Quaternion.h URL.h PreferencesApp.h swt/include/swttypedef.h \
 EcmaScriptApp.h StereoViewApp.h swt/include/stereoview.h \
 InputDeviceApp.h TransformMode.h OutputApp.h RouteViewApp.h \
 StartWithApp.h ExportNamesApp.h ModellingApp.h RenderApp.h
MFColorItem.o: MFColorItem.cpp MFColorItem.h FieldViewItem.h Array.h \
 stdafx.h config.h mysnprintf.h Rect.h Point.h MyString.h StringArray.h \
 Field.h Element.h x3dFlags.h swt/include/WriteFlags.h Node.h List.h \
 Vec3f.h Vec3x.h FieldIndex.h NodeFlags.h MeshFlags.h \
 ac3dMaterialCallback.h NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h \
 Types.h swt/include/swttypedef.h MFFloatItem.h MFColor.h MFFloat.h \
 FieldValue.h Arrays.h MySwap.h SFFloat.h SFColorItem.h SFColor.h \
 FieldView.h SceneView.h DuneApp.h URL.h PreferencesApp.h EcmaScriptApp.h \
 StereoViewApp.h swt/include/stereoview.h InputDeviceApp.h \
 TransformMode.h OutputApp.h RouteViewApp.h StartWithApp.h \
 ExportNamesApp.h ModellingApp.h RenderApp.h Scene.h Map.h CommandList.h \
 Command.h NodeList.h Proto.h ProtoMacros.h InterfaceArray.h \
 DynamicFieldsNode.h SFMFTypes.h MFBool.h MFColorRGBA.h MFDouble.h \
 SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h MFRotation.h \
 MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h \
 MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColorRGBA.h SFImage.h SFInt32.h \
 SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h SFString.h SFVec2d.h \
 SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h RenderState.h \
 ProtoMap.h swt/include/swt.h swt/include/keycodes.h \
 swt/include/swlanguages.h swt/include/swDebugf.h \
 swt/include/swttypedef.h
MFColorRGBA.o: MFColorRGBA.cpp stdafx.h config.h mysnprintf.h \
 MFColorRGBA.h MFFloat.h Array.h FieldValue.h MyString.h StringArray.h \
 Element.h x3dFlags.h swt/include/WriteFlags.h Arrays.h MySwap.h Types.h \
 SFFloat.h SFColorRGBA.h DuneApp.h List.h Node.h Vec3f.h Vec3x.h \
 FieldIndex.h NodeFlags.h MeshFlags.h ac3dMaterialCallback.h NodeArray.h \
 Stack.h Matrix.h Vec3d.h Quaternion.h URL.h PreferencesApp.h \
 swt/include/swttypedef.h EcmaScriptApp.h StereoViewApp.h \
 swt/include/stereoview.h InputDeviceApp.h TransformMode.h OutputApp.h \
 RouteViewApp.h StartWithApp.h ExportNamesApp.h ModellingApp.h \
 RenderApp.h
MFColorRGBAItem.o: MFColorRGBAItem.cpp MFColorRGBAItem.h FieldViewItem.h \
 Array.h stdafx.h config.h mysnprintf.h Rect.h Point.h MyString.h \
 StringArray.h Field.h Element.h x3dFlags.h swt/include/WriteFlags.h \
 Node.h List.h Vec3f.h Vec3x.h FieldIndex.h NodeFlags.h MeshFlags.h \
 ac3dMaterialCallback.h NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h \
 Types.h swt/include/swttypedef.h MFFloatItem.h MFColorRGBA.h MFFloat.h \
 FieldValue.h Arrays.h MySwap.h SFFloat.h SFColorRGBAItem.h SFColorRGBA.h \
 FieldView.h SceneView.h DuneApp.h URL.h PreferencesApp.h EcmaScriptApp.h \
 StereoViewApp.h swt/include/stereoview.h InputDeviceApp.h \
 TransformMode.h OutputApp.h RouteViewApp.h StartWithApp.h \
 ExportNamesApp.h ModellingApp.h RenderApp.h Scene.h Map.h CommandList.h \
 Command.h NodeList.h Proto.h ProtoMacros.h InterfaceArray.h \
 DynamicFieldsNode.h SFMFTypes.h MFBool.h MFColor.h MFDouble.h SFDouble.h \
 MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h MFRotation.h MFString.h \
 MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h MFVec3f.h \
 MFVec4f.h Vec4f.h SFBool.h SFColor.h SFImage.h SFInt32.h SFMatrix3f.h \
 SFMatrix4f.h SFNode.h SFRotation.h SFString.h SFVec2d.h SFVec2f.h \
 Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h RenderState.h ProtoMap.h \
 swt/include/swt.h swt/include/keycodes.h swt/include/swlanguages.h \
 swt/include/swDebugf.h swt/include/swttypedef.h
MFDouble.o: MFDouble.cpp stdafx.h config.h mysnprintf.h MFDouble.h \
 Array.h FieldValue.h MyString.h StringArray.h Element.h x3dFlags.h \
 swt/include/WriteFlags.h Arrays.h MySwap.h Types.h SFDouble.h DuneApp.h \
 List.h Node.h Vec3f.h Vec3x.h FieldIndex.h NodeFlags.h MeshFlags.h \
 ac3dMaterialCallback.h NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h \
 URL.h PreferencesApp.h swt/include/swttypedef.h EcmaScriptApp.h \
 StereoViewApp.h swt/include/stereoview.h InputDeviceApp.h \
 TransformMode.h OutputApp.h RouteViewApp.h StartWithApp.h \
 ExportNamesApp.h ModellingApp.h RenderApp.h
MFDoubleItem.o: MFDoubleItem.cpp MFDoubleItem.h FieldViewItem.h Array.h \
 stdafx.h config.h mysnprintf.h Rect.h Point.h MyString.h StringArray.h \
 Field.h Element.h x3dFlags.h swt/include/WriteFlags.h Node.h List.h \
 Vec3f.h Vec3x.h FieldIndex.h NodeFlags.h MeshFlags.h \
 ac3dMaterialCallback.h NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h \
 Types.h swt/include/swttypedef.h MFDouble.h FieldValue.h Arrays.h \
 MySwap.h SFDouble.h SFDoubleItem.h FieldView.h SceneView.h DuneApp.h \
 URL.h PreferencesApp.h EcmaScriptApp.h StereoViewApp.h \
 swt/include/stereoview.h InputDeviceApp.h TransformMode.h OutputApp.h \
 RouteViewApp.h StartWithApp.h ExportNamesApp.h ModellingApp.h \
 RenderApp.h Scene.h Map.h CommandList.h Command.h NodeList.h Proto.h \
 ProtoMacros.h InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h MFBool.h \
 MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h MFInt32.h MFMatrix3f.h \
 MFMatrix4f.h MFNode.h MFRotation.h MFString.h MFTime.h SFTime.h \
 MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h \
 SFBool.h SFColor.h SFColorRGBA.h SFImage.h SFInt32.h SFMatrix3f.h \
 SFMatrix4f.h SFNode.h SFRotation.h SFString.h SFVec2d.h SFVec2f.h \
 Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h RenderState.h ProtoMap.h \
 swt/include/swt.h swt/include/keycodes.h swt/include/swlanguages.h \
 swt/include/swDebugf.h swt/include/swttypedef.h
MFInt32.o: MFInt32.cpp stdafx.h config.h mysnprintf.h MFInt32.h Array.h \
 FieldValue.h MyString.h StringArray.h Element.h x3dFlags.h \
 swt/include/WriteFlags.h Arrays.h MySwap.h Types.h SFInt32.h DuneApp.h \
 List.h Node.h Vec3f.h Vec3x.h FieldIndex.h NodeFlags.h MeshFlags.h \
 ac3dMaterialCallback.h NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h \
 URL.h PreferencesApp.h swt/include/swttypedef.h EcmaScriptApp.h \
 StereoViewApp.h swt/include/stereoview.h InputDeviceApp.h \
 TransformMode.h OutputApp.h RouteViewApp.h StartWithApp.h \
 ExportNamesApp.h ModellingApp.h RenderApp.h
MFInt32Item.o: MFInt32Item.cpp MFInt32Item.h FieldViewItem.h Array.h \
 stdafx.h config.h mysnprintf.h Rect.h Point.h MyString.h StringArray.h \
 Field.h Element.h x3dFlags.h swt/include/WriteFlags.h Node.h List.h \
 Vec3f.h Vec3x.h FieldIndex.h NodeFlags.h MeshFlags.h \
 ac3dMaterialCallback.h NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h \
 Types.h swt/include/swttypedef.h MFInt32.h FieldValue.h Arrays.h \
 MySwap.h SFInt32Item.h SFInt32.h FieldView.h SceneView.h DuneApp.h URL.h \
 PreferencesApp.h EcmaScriptApp.h StereoViewApp.h \
 swt/include/stereoview.h InputDeviceApp.h TransformMode.h OutputApp.h \
 RouteViewApp.h StartWithApp.h ExportNamesApp.h ModellingApp.h \
 RenderApp.h Scene.h Map.h CommandList.h Command.h NodeList.h Proto.h \
 ProtoMacros.h InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h MFBool.h \
 MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h MFDouble.h SFDouble.h \
 MFMatrix3f.h MFMatrix4f.h MFNode.h MFRotation.h MFString.h MFTime.h \
 SFTime.h MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h MFVec3f.h MFVec4f.h \
 Vec4f.h SFBool.h SFColor.h SFColorRGBA.h SFImage.h SFMatrix3f.h \
 SFMatrix4f.h SFNode.h SFRotation.h SFString.h SFVec2d.h SFVec2f.h \
 Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h RenderState.h ProtoMap.h \
 swt/include/swt.h swt/include/keycodes.h swt/include/swlanguages.h \
 swt/include/swDebugf.h swt/include/swttypedef.h
MFFloat.o: MFFloat.cpp stdafx.h config.h mysnprintf.h MFFloat.h Array.h \
 FieldValue.h MyString.h StringArray.h Element.h x3dFlags.h \
 swt/include/WriteFlags.h Arrays.h MySwap.h Types.h SFFloat.h DuneApp.h \
 List.h Node.h Vec3f.h Vec3x.h FieldIndex.h NodeFlags.h MeshFlags.h \
 ac3dMaterialCallback.h NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h \
 URL.h PreferencesApp.h swt/include/swttypedef.h EcmaScriptApp.h \
 StereoViewApp.h swt/include/stereoview.h InputDeviceApp.h \
 TransformMode.h OutputApp.h RouteViewApp.h StartWithApp.h \
 ExportNamesApp.h ModellingApp.h RenderApp.h
MFFloatItem.o: MFFloatItem.cpp MFFloatItem.h FieldViewItem.h Array.h \
 stdafx.h config.h mysnprintf.h Rect.h Point.h MyString.h StringArray.h \
 Field.h Element.h x3dFlags.h swt/include/WriteFlags.h Node.h List.h \
 Vec3f.h Vec3x.h FieldIndex.h NodeFlags.h MeshFlags.h \
 ac3dMaterialCallback.h NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h \
 Types.h swt/include/swttypedef.h MFFloat.h FieldValue.h Arrays.h \
 MySwap.h SFFloat.h SFFloatItem.h FieldView.h SceneView.h DuneApp.h URL.h \
 PreferencesApp.h EcmaScriptApp.h StereoViewApp.h \
 swt/include/stereoview.h InputDeviceApp.h TransformMode.h OutputApp.h \
 RouteViewApp.h StartWithApp.h ExportNamesApp.h ModellingApp.h \
 RenderApp.h Scene.h Map.h CommandList.h Command.h NodeList.h Proto.h \
 ProtoMacros.h InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h MFBool.h \
 MFColor.h MFColorRGBA.h MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h \
 MFMatrix4f.h MFNode.h MFRotation.h MFString.h MFTime.h SFTime.h \
 MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h \
 SFBool.h SFColor.h SFColorRGBA.h SFImage.h SFInt32.h SFMatrix3f.h \
 SFMatrix4f.h SFNode.h SFRotation.h SFString.h SFVec2d.h SFVec2f.h \
 Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h RenderState.h ProtoMap.h \
 swt/include/swt.h swt/include/keycodes.h swt/include/swlanguages.h \
 swt/include/swDebugf.h swt/include/swttypedef.h
MFNode.o: MFNode.cpp stdafx.h config.h mysnprintf.h MFNode.h FieldValue.h \
 Array.h MyString.h StringArray.h Element.h x3dFlags.h \
 swt/include/WriteFlags.h Arrays.h MySwap.h Types.h Node.h List.h Vec3f.h \
 Vec3x.h FieldIndex.h NodeFlags.h MeshFlags.h ac3dMaterialCallback.h \
 NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h NodeList.h SFNode.h \
 Proto.h Map.h ProtoMacros.h InterfaceArray.h DynamicFieldsNode.h \
 SFMFTypes.h MFBool.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h \
 MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFRotation.h \
 MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h \
 MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h SFImage.h \
 SFInt32.h SFMatrix3f.h SFMatrix4f.h SFRotation.h SFString.h SFVec2d.h \
 SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h CommandList.h \
 Command.h DuneApp.h URL.h PreferencesApp.h swt/include/swttypedef.h \
 EcmaScriptApp.h StereoViewApp.h swt/include/stereoview.h \
 InputDeviceApp.h TransformMode.h OutputApp.h RouteViewApp.h \
 StartWithApp.h ExportNamesApp.h ModellingApp.h RenderApp.h \
 NodeCurveAnimation.h NodeNurbsCurve.h NurbsCurve.h GeometryNode.h \
 NodeAppearance.h NodeMaterial.h swt/include/swt.h swt/include/keycodes.h \
 swt/include/swlanguages.h swt/include/swDebugf.h \
 swt/include/swttypedef.h NodeImageTexture.h KambiTextureCommonFields.h \
 NodeTextureTransform.h GeometryNodeMacros.h ChainBasedNode.h \
 NodePositionInterpolator.h Interpolator.h NodeOrientationInterpolator.h \
 Scene.h RenderState.h ProtoMap.h
MFNodeItem.o: MFNodeItem.cpp MFNodeItem.h FieldViewItem.h Array.h \
 stdafx.h config.h mysnprintf.h Rect.h Point.h MyString.h StringArray.h \
 Field.h Element.h x3dFlags.h swt/include/WriteFlags.h Node.h List.h \
 Vec3f.h Vec3x.h FieldIndex.h NodeFlags.h MeshFlags.h \
 ac3dMaterialCallback.h NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h \
 Types.h swt/include/swttypedef.h SFNodeItem.h
MFRotation.o: MFRotation.cpp stdafx.h config.h mysnprintf.h MFRotation.h \
 MFFloat.h Array.h FieldValue.h MyString.h StringArray.h Element.h \
 x3dFlags.h swt/include/WriteFlags.h Arrays.h MySwap.h Types.h SFFloat.h \
 SFRotation.h Vec3f.h Vec3x.h Matrix.h Vec3d.h Quaternion.h DuneApp.h \
 List.h Node.h FieldIndex.h NodeFlags.h MeshFlags.h \
 ac3dMaterialCallback.h NodeArray.h Stack.h URL.h PreferencesApp.h \
 swt/include/swttypedef.h EcmaScriptApp.h StereoViewApp.h \
 swt/include/stereoview.h InputDeviceApp.h TransformMode.h OutputApp.h \
 RouteViewApp.h StartWithApp.h ExportNamesApp.h ModellingApp.h \
 RenderApp.h
MFRotationItem.o: MFRotationItem.cpp MFRotationItem.h FieldViewItem.h \
 Array.h stdafx.h config.h mysnprintf.h Rect.h Point.h MyString.h \
 StringArray.h Field.h Element.h x3dFlags.h swt/include/WriteFlags.h \
 Node.h List.h Vec3f.h Vec3x.h FieldIndex.h NodeFlags.h MeshFlags.h \
 ac3dMaterialCallback.h NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h \
 Types.h swt/include/swttypedef.h MFFloatItem.h MFRotation.h MFFloat.h \
 FieldValue.h Arrays.h MySwap.h SFFloat.h SFRotationItem.h SFRotation.h \
 FieldView.h SceneView.h DuneApp.h URL.h PreferencesApp.h EcmaScriptApp.h \
 StereoViewApp.h swt/include/stereoview.h InputDeviceApp.h \
 TransformMode.h OutputApp.h RouteViewApp.h StartWithApp.h \
 ExportNamesApp.h ModellingApp.h RenderApp.h Scene.h Map.h CommandList.h \
 Command.h NodeList.h Proto.h ProtoMacros.h InterfaceArray.h \
 DynamicFieldsNode.h SFMFTypes.h MFBool.h MFColor.h MFColorRGBA.h \
 MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h \
 MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h \
 MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h SFImage.h \
 SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFString.h SFVec2d.h \
 SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h RenderState.h \
 ProtoMap.h swt/include/swt.h swt/include/keycodes.h \
 swt/include/swlanguages.h swt/include/swDebugf.h \
 swt/include/swttypedef.h
MFString.o: MFString.cpp stdafx.h config.h mysnprintf.h MFString.h \
 FieldValue.h Array.h MyString.h StringArray.h Element.h x3dFlags.h \
 swt/include/WriteFlags.h Arrays.h MySwap.h Types.h SFString.h DuneApp.h \
 List.h Node.h Vec3f.h Vec3x.h FieldIndex.h NodeFlags.h MeshFlags.h \
 ac3dMaterialCallback.h NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h \
 URL.h PreferencesApp.h swt/include/swttypedef.h EcmaScriptApp.h \
 StereoViewApp.h swt/include/stereoview.h InputDeviceApp.h \
 TransformMode.h OutputApp.h RouteViewApp.h StartWithApp.h \
 ExportNamesApp.h ModellingApp.h RenderApp.h swt/include/swt.h \
 swt/include/keycodes.h swt/include/swlanguages.h swt/include/swDebugf.h \
 swt/include/swttypedef.h
MFStringItem.o: MFStringItem.cpp MFStringItem.h FieldViewItem.h Array.h \
 stdafx.h config.h mysnprintf.h Rect.h Point.h MyString.h StringArray.h \
 Field.h Element.h x3dFlags.h swt/include/WriteFlags.h Node.h List.h \
 Vec3f.h Vec3x.h FieldIndex.h NodeFlags.h MeshFlags.h \
 ac3dMaterialCallback.h NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h \
 Types.h swt/include/swttypedef.h MFString.h FieldValue.h Arrays.h \
 MySwap.h SFStringItem.h SFString.h FieldView.h SceneView.h DuneApp.h \
 URL.h PreferencesApp.h EcmaScriptApp.h StereoViewApp.h \
 swt/include/stereoview.h InputDeviceApp.h TransformMode.h OutputApp.h \
 RouteViewApp.h StartWithApp.h ExportNamesApp.h ModellingApp.h \
 RenderApp.h Scene.h Map.h CommandList.h Command.h NodeList.h Proto.h \
 ProtoMacros.h InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h MFBool.h \
 MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h MFDouble.h SFDouble.h \
 MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h MFRotation.h MFTime.h \
 SFTime.h MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h MFVec3f.h MFVec4f.h \
 Vec4f.h SFBool.h SFColor.h SFColorRGBA.h SFImage.h SFInt32.h \
 SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h SFVec2d.h SFVec2f.h \
 Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h RenderState.h ProtoMap.h \
 swt/include/swt.h swt/include/keycodes.h swt/include/swlanguages.h \
 swt/include/swDebugf.h swt/include/swttypedef.h
MFTime.o: MFTime.cpp stdafx.h config.h mysnprintf.h MFTime.h FieldValue.h \
 Array.h MyString.h StringArray.h Element.h x3dFlags.h \
 swt/include/WriteFlags.h Arrays.h MySwap.h Types.h SFTime.h DuneApp.h \
 List.h Node.h Vec3f.h Vec3x.h FieldIndex.h NodeFlags.h MeshFlags.h \
 ac3dMaterialCallback.h NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h \
 URL.h PreferencesApp.h swt/include/swttypedef.h EcmaScriptApp.h \
 StereoViewApp.h swt/include/stereoview.h InputDeviceApp.h \
 TransformMode.h OutputApp.h RouteViewApp.h StartWithApp.h \
 ExportNamesApp.h ModellingApp.h RenderApp.h
MFTimeItem.o: MFTimeItem.cpp MFTimeItem.h FieldViewItem.h Array.h \
 stdafx.h config.h mysnprintf.h Rect.h Point.h MyString.h StringArray.h \
 Field.h Element.h x3dFlags.h swt/include/WriteFlags.h Node.h List.h \
 Vec3f.h Vec3x.h FieldIndex.h NodeFlags.h MeshFlags.h \
 ac3dMaterialCallback.h NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h \
 Types.h swt/include/swttypedef.h MFTime.h FieldValue.h Arrays.h MySwap.h \
 SFTime.h SFTimeItem.h FieldView.h SceneView.h DuneApp.h URL.h \
 PreferencesApp.h EcmaScriptApp.h StereoViewApp.h \
 swt/include/stereoview.h InputDeviceApp.h TransformMode.h OutputApp.h \
 RouteViewApp.h StartWithApp.h ExportNamesApp.h ModellingApp.h \
 RenderApp.h Scene.h Map.h CommandList.h Command.h NodeList.h Proto.h \
 ProtoMacros.h InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h MFBool.h \
 MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h MFDouble.h SFDouble.h \
 MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h MFRotation.h MFString.h \
 MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h \
 SFBool.h SFColor.h SFColorRGBA.h SFImage.h SFInt32.h SFMatrix3f.h \
 SFMatrix4f.h SFNode.h SFRotation.h SFString.h SFVec2d.h SFVec2f.h \
 Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h RenderState.h ProtoMap.h \
 swt/include/swt.h swt/include/keycodes.h swt/include/swlanguages.h \
 swt/include/swDebugf.h swt/include/swttypedef.h
MFVec2f.o: MFVec2f.cpp stdafx.h config.h mysnprintf.h MFVec2f.h MFFloat.h \
 Array.h FieldValue.h MyString.h StringArray.h Element.h x3dFlags.h \
 swt/include/WriteFlags.h Arrays.h MySwap.h Types.h SFFloat.h SFVec2f.h \
 Vec2f.h DuneApp.h List.h Node.h Vec3f.h Vec3x.h FieldIndex.h NodeFlags.h \
 MeshFlags.h ac3dMaterialCallback.h NodeArray.h Stack.h Matrix.h Vec3d.h \
 Quaternion.h URL.h PreferencesApp.h swt/include/swttypedef.h \
 EcmaScriptApp.h StereoViewApp.h swt/include/stereoview.h \
 InputDeviceApp.h TransformMode.h OutputApp.h RouteViewApp.h \
 StartWithApp.h ExportNamesApp.h ModellingApp.h RenderApp.h
MFVec2fItem.o: MFVec2fItem.cpp MFVec2fItem.h MFFloatItem.h \
 FieldViewItem.h Array.h stdafx.h config.h mysnprintf.h Rect.h Point.h \
 MyString.h StringArray.h Field.h Element.h x3dFlags.h \
 swt/include/WriteFlags.h Node.h List.h Vec3f.h Vec3x.h FieldIndex.h \
 NodeFlags.h MeshFlags.h ac3dMaterialCallback.h NodeArray.h Stack.h \
 Matrix.h Vec3d.h Quaternion.h Types.h swt/include/swttypedef.h MFVec2f.h \
 MFFloat.h FieldValue.h Arrays.h MySwap.h SFFloat.h SFVec2fItem.h \
 SFVec2f.h Vec2f.h FieldView.h SceneView.h DuneApp.h URL.h \
 PreferencesApp.h EcmaScriptApp.h StereoViewApp.h \
 swt/include/stereoview.h InputDeviceApp.h TransformMode.h OutputApp.h \
 RouteViewApp.h StartWithApp.h ExportNamesApp.h ModellingApp.h \
 RenderApp.h Scene.h Map.h CommandList.h Command.h NodeList.h Proto.h \
 ProtoMacros.h InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h MFBool.h \
 MFColor.h MFColorRGBA.h MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h \
 MFMatrix4f.h MFNode.h MFRotation.h MFString.h MFTime.h SFTime.h \
 MFVec2d.h Vec2d.h MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h SFBool.h \
 SFColor.h SFColorRGBA.h SFImage.h SFInt32.h SFMatrix3f.h SFMatrix4f.h \
 SFNode.h SFRotation.h SFString.h SFVec2d.h SFVec3d.h SfVec3x.h SFVec3f.h \
 SFVec4f.h RenderState.h ProtoMap.h swt/include/swt.h \
 swt/include/keycodes.h swt/include/swlanguages.h swt/include/swDebugf.h \
 swt/include/swttypedef.h
MFVec2d.o: MFVec2d.cpp stdafx.h config.h mysnprintf.h MFVec2d.h \
 MFDouble.h Array.h FieldValue.h MyString.h StringArray.h Element.h \
 x3dFlags.h swt/include/WriteFlags.h Arrays.h MySwap.h Types.h SFDouble.h \
 Vec2d.h SFVec2d.h MFString.h DuneApp.h List.h Node.h Vec3f.h Vec3x.h \
 FieldIndex.h NodeFlags.h MeshFlags.h ac3dMaterialCallback.h NodeArray.h \
 Stack.h Matrix.h Vec3d.h Quaternion.h URL.h PreferencesApp.h \
 swt/include/swttypedef.h EcmaScriptApp.h StereoViewApp.h \
 swt/include/stereoview.h InputDeviceApp.h TransformMode.h OutputApp.h \
 RouteViewApp.h StartWithApp.h ExportNamesApp.h ModellingApp.h \
 RenderApp.h
MFVec2dItem.o: MFVec2dItem.cpp MFVec2dItem.h MFFloatItem.h \
 FieldViewItem.h Array.h stdafx.h config.h mysnprintf.h Rect.h Point.h \
 MyString.h StringArray.h Field.h Element.h x3dFlags.h \
 swt/include/WriteFlags.h Node.h List.h Vec3f.h Vec3x.h FieldIndex.h \
 NodeFlags.h MeshFlags.h ac3dMaterialCallback.h NodeArray.h Stack.h \
 Matrix.h Vec3d.h Quaternion.h Types.h swt/include/swttypedef.h MFVec2d.h \
 MFDouble.h FieldValue.h Arrays.h MySwap.h SFDouble.h Vec2d.h \
 SFVec2dItem.h SFVec2d.h FieldView.h SceneView.h DuneApp.h URL.h \
 PreferencesApp.h EcmaScriptApp.h StereoViewApp.h \
 swt/include/stereoview.h InputDeviceApp.h TransformMode.h OutputApp.h \
 RouteViewApp.h StartWithApp.h ExportNamesApp.h ModellingApp.h \
 RenderApp.h Scene.h Map.h CommandList.h Command.h NodeList.h Proto.h \
 ProtoMacros.h InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h MFBool.h \
 MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h MFInt32.h MFMatrix3f.h \
 MFMatrix4f.h MFNode.h MFRotation.h MFString.h MFTime.h SFTime.h \
 MFVec2f.h MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h \
 SFColorRGBA.h SFImage.h SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h \
 SFRotation.h SFString.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h \
 SFVec4f.h RenderState.h ProtoMap.h swt/include/swt.h \
 swt/include/keycodes.h swt/include/swlanguages.h swt/include/swDebugf.h \
 swt/include/swttypedef.h
MFVec3f.o: MFVec3f.cpp stdafx.h config.h mysnprintf.h MFVec3f.h MFFloat.h \
 Array.h FieldValue.h MyString.h StringArray.h Element.h x3dFlags.h \
 swt/include/WriteFlags.h Arrays.h MySwap.h Types.h SFFloat.h Vec3f.h \
 Vec3x.h SFVec3f.h SfVec3x.h Quaternion.h Matrix.h Vec3d.h MFVec2f.h \
 MFVec3d.h MFDouble.h SFDouble.h DuneApp.h List.h Node.h FieldIndex.h \
 NodeFlags.h MeshFlags.h ac3dMaterialCallback.h NodeArray.h Stack.h URL.h \
 PreferencesApp.h swt/include/swttypedef.h EcmaScriptApp.h \
 StereoViewApp.h swt/include/stereoview.h InputDeviceApp.h \
 TransformMode.h OutputApp.h RouteViewApp.h StartWithApp.h \
 ExportNamesApp.h ModellingApp.h RenderApp.h
MFVec3fItem.o: MFVec3fItem.cpp MFVec3fItem.h FieldViewItem.h Array.h \
 stdafx.h config.h mysnprintf.h Rect.h Point.h MyString.h StringArray.h \
 Field.h Element.h x3dFlags.h swt/include/WriteFlags.h Node.h List.h \
 Vec3f.h Vec3x.h FieldIndex.h NodeFlags.h MeshFlags.h \
 ac3dMaterialCallback.h NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h \
 Types.h swt/include/swttypedef.h MFFloatItem.h MFVec3f.h MFFloat.h \
 FieldValue.h Arrays.h MySwap.h SFFloat.h SFVec3fItem.h SFVec3f.h \
 SfVec3x.h FieldView.h SceneView.h DuneApp.h URL.h PreferencesApp.h \
 EcmaScriptApp.h StereoViewApp.h swt/include/stereoview.h \
 InputDeviceApp.h TransformMode.h OutputApp.h RouteViewApp.h \
 StartWithApp.h ExportNamesApp.h ModellingApp.h RenderApp.h Scene.h Map.h \
 CommandList.h Command.h NodeList.h Proto.h ProtoMacros.h \
 InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h MFBool.h MFColor.h \
 MFColorRGBA.h MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h \
 MFNode.h MFRotation.h MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h \
 MFVec2f.h MFVec3d.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h \
 SFImage.h SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h \
 SFString.h SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SFVec4f.h RenderState.h \
 ProtoMap.h swt/include/swt.h swt/include/keycodes.h \
 swt/include/swlanguages.h swt/include/swDebugf.h \
 swt/include/swttypedef.h
MFVec3d.o: MFVec3d.cpp stdafx.h config.h mysnprintf.h MFVec3d.h \
 MFDouble.h Array.h FieldValue.h MyString.h StringArray.h Element.h \
 x3dFlags.h swt/include/WriteFlags.h Arrays.h MySwap.h Types.h SFDouble.h \
 Vec3d.h Vec3x.h SFVec3d.h SfVec3x.h Vec3f.h Quaternion.h Matrix.h \
 SFString.h MFString.h MFVec3f.h MFFloat.h SFFloat.h DuneApp.h List.h \
 Node.h FieldIndex.h NodeFlags.h MeshFlags.h ac3dMaterialCallback.h \
 NodeArray.h Stack.h URL.h PreferencesApp.h swt/include/swttypedef.h \
 EcmaScriptApp.h StereoViewApp.h swt/include/stereoview.h \
 InputDeviceApp.h TransformMode.h OutputApp.h RouteViewApp.h \
 StartWithApp.h ExportNamesApp.h ModellingApp.h RenderApp.h
MFVec3dItem.o: MFVec3dItem.cpp MFVec3dItem.h FieldViewItem.h Array.h \
 stdafx.h config.h mysnprintf.h Rect.h Point.h MyString.h StringArray.h \
 Field.h Element.h x3dFlags.h swt/include/WriteFlags.h Node.h List.h \
 Vec3f.h Vec3x.h FieldIndex.h NodeFlags.h MeshFlags.h \
 ac3dMaterialCallback.h NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h \
 Types.h swt/include/swttypedef.h MFDoubleItem.h MFVec3d.h MFDouble.h \
 FieldValue.h Arrays.h MySwap.h SFDouble.h SFVec3dItem.h SFVec3d.h \
 SfVec3x.h SFString.h FieldView.h SceneView.h DuneApp.h URL.h \
 PreferencesApp.h EcmaScriptApp.h StereoViewApp.h \
 swt/include/stereoview.h InputDeviceApp.h TransformMode.h OutputApp.h \
 RouteViewApp.h StartWithApp.h ExportNamesApp.h ModellingApp.h \
 RenderApp.h Scene.h Map.h CommandList.h Command.h NodeList.h Proto.h \
 ProtoMacros.h InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h MFBool.h \
 MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h MFInt32.h MFMatrix3f.h \
 MFMatrix4f.h MFNode.h MFRotation.h MFString.h MFTime.h SFTime.h \
 MFVec2d.h Vec2d.h MFVec2f.h MFVec3f.h MFVec4f.h Vec4f.h SFBool.h \
 SFColor.h SFColorRGBA.h SFImage.h SFInt32.h SFMatrix3f.h SFMatrix4f.h \
 SFNode.h SFRotation.h SFVec2d.h SFVec2f.h Vec2f.h SFVec3f.h SFVec4f.h \
 RenderState.h ProtoMap.h swt/include/swt.h swt/include/keycodes.h \
 swt/include/swlanguages.h swt/include/swDebugf.h \
 swt/include/swttypedef.h
MFVec4f.o: MFVec4f.cpp stdafx.h config.h mysnprintf.h MFVec4f.h MFFloat.h \
 Array.h FieldValue.h MyString.h StringArray.h Element.h x3dFlags.h \
 swt/include/WriteFlags.h Arrays.h MySwap.h Types.h SFFloat.h Vec4f.h \
 SFVec4f.h MFVec2f.h DuneApp.h List.h Node.h Vec3f.h Vec3x.h FieldIndex.h \
 NodeFlags.h MeshFlags.h ac3dMaterialCallback.h NodeArray.h Stack.h \
 Matrix.h Vec3d.h Quaternion.h URL.h PreferencesApp.h \
 swt/include/swttypedef.h EcmaScriptApp.h StereoViewApp.h \
 swt/include/stereoview.h InputDeviceApp.h TransformMode.h OutputApp.h \
 RouteViewApp.h StartWithApp.h ExportNamesApp.h ModellingApp.h \
 RenderApp.h
MFVec4fItem.o: MFVec4fItem.cpp MFVec4fItem.h FieldViewItem.h Array.h \
 stdafx.h config.h mysnprintf.h Rect.h Point.h MyString.h StringArray.h \
 Field.h Element.h x3dFlags.h swt/include/WriteFlags.h Node.h List.h \
 Vec3f.h Vec3x.h FieldIndex.h NodeFlags.h MeshFlags.h \
 ac3dMaterialCallback.h NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h \
 Types.h swt/include/swttypedef.h MFFloatItem.h MFVec4f.h MFFloat.h \
 FieldValue.h Arrays.h MySwap.h SFFloat.h Vec4f.h SFVec4fItem.h SFVec4f.h \
 FieldView.h SceneView.h DuneApp.h URL.h PreferencesApp.h EcmaScriptApp.h \
 StereoViewApp.h swt/include/stereoview.h InputDeviceApp.h \
 TransformMode.h OutputApp.h RouteViewApp.h StartWithApp.h \
 ExportNamesApp.h ModellingApp.h RenderApp.h Scene.h Map.h CommandList.h \
 Command.h NodeList.h Proto.h ProtoMacros.h InterfaceArray.h \
 DynamicFieldsNode.h SFMFTypes.h MFBool.h MFColor.h MFColorRGBA.h \
 MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h \
 MFRotation.h MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h \
 MFVec3d.h MFVec3f.h SFBool.h SFColor.h SFColorRGBA.h SFImage.h SFInt32.h \
 SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h SFString.h SFVec2d.h \
 SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h RenderState.h ProtoMap.h \
 swt/include/swt.h swt/include/keycodes.h swt/include/swlanguages.h \
 swt/include/swDebugf.h swt/include/swttypedef.h
MFMatrix3f.o: MFMatrix3f.cpp stdafx.h config.h mysnprintf.h MFMatrix3f.h \
 MFFloat.h Array.h FieldValue.h MyString.h StringArray.h Element.h \
 x3dFlags.h swt/include/WriteFlags.h Arrays.h MySwap.h Types.h SFFloat.h \
 SFMatrix3f.h MFVec2f.h DuneApp.h List.h Node.h Vec3f.h Vec3x.h \
 FieldIndex.h NodeFlags.h MeshFlags.h ac3dMaterialCallback.h NodeArray.h \
 Stack.h Matrix.h Vec3d.h Quaternion.h URL.h PreferencesApp.h \
 swt/include/swttypedef.h EcmaScriptApp.h StereoViewApp.h \
 swt/include/stereoview.h InputDeviceApp.h TransformMode.h OutputApp.h \
 RouteViewApp.h StartWithApp.h ExportNamesApp.h ModellingApp.h \
 RenderApp.h
MFMatrix3fItem.o: MFMatrix3fItem.cpp MFMatrix3fItem.h FieldViewItem.h \
 Array.h stdafx.h config.h mysnprintf.h Rect.h Point.h MyString.h \
 StringArray.h Field.h Element.h x3dFlags.h swt/include/WriteFlags.h \
 Node.h List.h Vec3f.h Vec3x.h FieldIndex.h NodeFlags.h MeshFlags.h \
 ac3dMaterialCallback.h NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h \
 Types.h swt/include/swttypedef.h MFFloatItem.h MFMatrix3f.h MFFloat.h \
 FieldValue.h Arrays.h MySwap.h SFFloat.h SFMatrix3fItem.h SFMatrix3f.h \
 FieldView.h SceneView.h DuneApp.h URL.h PreferencesApp.h EcmaScriptApp.h \
 StereoViewApp.h swt/include/stereoview.h InputDeviceApp.h \
 TransformMode.h OutputApp.h RouteViewApp.h StartWithApp.h \
 ExportNamesApp.h ModellingApp.h RenderApp.h Scene.h Map.h CommandList.h \
 Command.h NodeList.h Proto.h ProtoMacros.h InterfaceArray.h \
 DynamicFieldsNode.h SFMFTypes.h MFBool.h MFColor.h MFColorRGBA.h \
 MFDouble.h SFDouble.h MFInt32.h MFMatrix4f.h MFNode.h MFRotation.h \
 MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h \
 MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h SFImage.h \
 SFInt32.h SFMatrix4f.h SFNode.h SFRotation.h SFString.h SFVec2d.h \
 SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h RenderState.h \
 ProtoMap.h swt/include/swt.h swt/include/keycodes.h \
 swt/include/swlanguages.h swt/include/swDebugf.h \
 swt/include/swttypedef.h
MFMatrix4f.o: MFMatrix4f.cpp stdafx.h config.h mysnprintf.h MFMatrix4f.h \
 MFFloat.h Array.h FieldValue.h MyString.h StringArray.h Element.h \
 x3dFlags.h swt/include/WriteFlags.h Arrays.h MySwap.h Types.h SFFloat.h \
 SFMatrix4f.h MFVec2f.h DuneApp.h List.h Node.h Vec3f.h Vec3x.h \
 FieldIndex.h NodeFlags.h MeshFlags.h ac3dMaterialCallback.h NodeArray.h \
 Stack.h Matrix.h Vec3d.h Quaternion.h URL.h PreferencesApp.h \
 swt/include/swttypedef.h EcmaScriptApp.h StereoViewApp.h \
 swt/include/stereoview.h InputDeviceApp.h TransformMode.h OutputApp.h \
 RouteViewApp.h StartWithApp.h ExportNamesApp.h ModellingApp.h \
 RenderApp.h
MFMatrix4fItem.o: MFMatrix4fItem.cpp MFMatrix4fItem.h FieldViewItem.h \
 Array.h stdafx.h config.h mysnprintf.h Rect.h Point.h MyString.h \
 StringArray.h Field.h Element.h x3dFlags.h swt/include/WriteFlags.h \
 Node.h List.h Vec3f.h Vec3x.h FieldIndex.h NodeFlags.h MeshFlags.h \
 ac3dMaterialCallback.h NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h \
 Types.h swt/include/swttypedef.h MFFloatItem.h MFMatrix4f.h MFFloat.h \
 FieldValue.h Arrays.h MySwap.h SFFloat.h SFMatrix4fItem.h SFMatrix4f.h \
 FieldView.h SceneView.h DuneApp.h URL.h PreferencesApp.h EcmaScriptApp.h \
 StereoViewApp.h swt/include/stereoview.h InputDeviceApp.h \
 TransformMode.h OutputApp.h RouteViewApp.h StartWithApp.h \
 ExportNamesApp.h ModellingApp.h RenderApp.h Scene.h Map.h CommandList.h \
 Command.h NodeList.h Proto.h ProtoMacros.h InterfaceArray.h \
 DynamicFieldsNode.h SFMFTypes.h MFBool.h MFColor.h MFColorRGBA.h \
 MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h MFNode.h MFRotation.h \
 MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h \
 MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h SFImage.h \
 SFInt32.h SFMatrix3f.h SFNode.h SFRotation.h SFString.h SFVec2d.h \
 SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h RenderState.h \
 ProtoMap.h swt/include/swt.h swt/include/keycodes.h \
 swt/include/swlanguages.h swt/include/swDebugf.h \
 swt/include/swttypedef.h
NodeSuperEllipsoid.o: NodeSuperEllipsoid.cpp stdafx.h config.h \
 mysnprintf.h DuneApp.h List.h Array.h MyString.h StringArray.h Node.h \
 Vec3f.h Vec3x.h FieldIndex.h Element.h x3dFlags.h \
 swt/include/WriteFlags.h NodeFlags.h MeshFlags.h ac3dMaterialCallback.h \
 NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h URL.h PreferencesApp.h \
 swt/include/swttypedef.h EcmaScriptApp.h StereoViewApp.h \
 swt/include/stereoview.h InputDeviceApp.h TransformMode.h OutputApp.h \
 RouteViewApp.h StartWithApp.h ExportNamesApp.h ModellingApp.h \
 RenderApp.h NodeSuperEllipsoid.h MeshMorphingNode.h Interpolator.h \
 ProtoMacros.h Proto.h Map.h NodeList.h InterfaceArray.h \
 DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h Arrays.h MySwap.h \
 Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h MFDouble.h \
 SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h MFRotation.h \
 MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h \
 MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h SFImage.h \
 SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h SFString.h \
 SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h \
 CommandList.h Command.h MeshBasedNode.h GeometryNode.h NodeAppearance.h \
 NodeMaterial.h swt/include/swt.h swt/include/keycodes.h \
 swt/include/swlanguages.h swt/include/swDebugf.h \
 swt/include/swttypedef.h NodeImageTexture.h KambiTextureCommonFields.h \
 NodeTextureTransform.h GeometryNodeMacros.h Scene.h RenderState.h \
 ProtoMap.h MyMesh.h NodeIndexedFaceSet.h NodeCoordinate.h Colored.h \
 NodeColor.h NodeColorRGBA.h ComposedGeometryMacros.h FaceData.h \
 NodeNormal.h NodeTextureCoordinate.h NodeTextureCoordinateGenerator.h \
 NodeMultiTextureCoordinate.h Util.h NodeNurbsSurface.h
NodeVrmlCut.o: NodeVrmlCut.cpp stdafx.h config.h mysnprintf.h \
 NodeVrmlCut.h Node.h Array.h List.h Vec3f.h Vec3x.h MyString.h \
 StringArray.h FieldIndex.h Element.h x3dFlags.h swt/include/WriteFlags.h \
 NodeFlags.h MeshFlags.h ac3dMaterialCallback.h NodeArray.h Stack.h \
 Matrix.h Vec3d.h Quaternion.h ProtoMacros.h Proto.h Map.h NodeList.h \
 InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h \
 Arrays.h MySwap.h Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h \
 MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h \
 MFRotation.h MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h \
 MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h \
 SFImage.h SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h \
 SFString.h SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h \
 SFVec4f.h CommandList.h Command.h swt/include/swt.h \
 swt/include/keycodes.h swt/include/swlanguages.h swt/include/swDebugf.h \
 swt/include/swttypedef.h Scene.h URL.h DuneApp.h PreferencesApp.h \
 swt/include/swttypedef.h EcmaScriptApp.h StereoViewApp.h \
 swt/include/stereoview.h InputDeviceApp.h TransformMode.h OutputApp.h \
 RouteViewApp.h StartWithApp.h ExportNamesApp.h ModellingApp.h \
 RenderApp.h RenderState.h ProtoMap.h Path.h ExposedField.h Field.h \
 EventIn.h EventOut.h RouteCommand.h UnRouteCommand.h NodeTimeSensor.h \
 NodeViewpoint.h ViewpointNode.h CoverNode.h KambiViewpointCommonFields.h \
 NodeBackground.h NodeCurveAnimation.h NodeNurbsCurve.h NurbsCurve.h \
 GeometryNode.h NodeAppearance.h NodeMaterial.h NodeImageTexture.h \
 KambiTextureCommonFields.h NodeTextureTransform.h GeometryNodeMacros.h \
 ChainBasedNode.h NodePositionInterpolator.h Interpolator.h \
 NodeOrientationInterpolator.h NodeVrmlScene.h
NodeGeneratedShadowMap.o: NodeGeneratedShadowMap.cpp stdafx.h config.h \
 mysnprintf.h NodeGeneratedShadowMap.h Node.h Array.h List.h Vec3f.h \
 Vec3x.h MyString.h StringArray.h FieldIndex.h Element.h x3dFlags.h \
 swt/include/WriteFlags.h NodeFlags.h MeshFlags.h ac3dMaterialCallback.h \
 NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h ProtoMacros.h Proto.h \
 Map.h NodeList.h InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h \
 MFBool.h FieldValue.h Arrays.h MySwap.h Types.h MFColor.h MFFloat.h \
 SFFloat.h MFColorRGBA.h MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h \
 MFMatrix4f.h MFNode.h MFRotation.h MFString.h MFTime.h SFTime.h \
 MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h \
 SFBool.h SFColor.h SFColorRGBA.h SFImage.h SFInt32.h SFMatrix3f.h \
 SFMatrix4f.h SFNode.h SFRotation.h SFString.h SFVec2d.h SFVec2f.h \
 Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h CommandList.h Command.h \
 KambiTextureCommonFields.h KambiCommonFunctions.h DuneApp.h URL.h \
 PreferencesApp.h swt/include/swttypedef.h EcmaScriptApp.h \
 StereoViewApp.h swt/include/stereoview.h InputDeviceApp.h \
 TransformMode.h OutputApp.h RouteViewApp.h StartWithApp.h \
 ExportNamesApp.h ModellingApp.h RenderApp.h
NodeMatrixTransform.o: NodeMatrixTransform.cpp stdafx.h config.h \
 mysnprintf.h NodeMatrixTransform.h Node.h Array.h List.h Vec3f.h Vec3x.h \
 MyString.h StringArray.h FieldIndex.h Element.h x3dFlags.h \
 swt/include/WriteFlags.h NodeFlags.h MeshFlags.h ac3dMaterialCallback.h \
 NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h ProtoMacros.h Proto.h \
 Map.h NodeList.h InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h \
 MFBool.h FieldValue.h Arrays.h MySwap.h Types.h MFColor.h MFFloat.h \
 SFFloat.h MFColorRGBA.h MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h \
 MFMatrix4f.h MFNode.h MFRotation.h MFString.h MFTime.h SFTime.h \
 MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h \
 SFBool.h SFColor.h SFColorRGBA.h SFImage.h SFInt32.h SFMatrix3f.h \
 SFMatrix4f.h SFNode.h SFRotation.h SFString.h SFVec2d.h SFVec2f.h \
 Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h CommandList.h Command.h \
 KambiCommonFunctions.h DuneApp.h URL.h PreferencesApp.h \
 swt/include/swttypedef.h EcmaScriptApp.h StereoViewApp.h \
 swt/include/stereoview.h InputDeviceApp.h TransformMode.h OutputApp.h \
 RouteViewApp.h StartWithApp.h ExportNamesApp.h ModellingApp.h \
 RenderApp.h
NodeMultiGeneratedTextureCoordinate.o: \
 NodeMultiGeneratedTextureCoordinate.cpp stdafx.h config.h mysnprintf.h \
 NodeMultiGeneratedTextureCoordinate.h Node.h Array.h List.h Vec3f.h \
 Vec3x.h MyString.h StringArray.h FieldIndex.h Element.h x3dFlags.h \
 swt/include/WriteFlags.h NodeFlags.h MeshFlags.h ac3dMaterialCallback.h \
 NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h ProtoMacros.h Proto.h \
 Map.h NodeList.h InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h \
 MFBool.h FieldValue.h Arrays.h MySwap.h Types.h MFColor.h MFFloat.h \
 SFFloat.h MFColorRGBA.h MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h \
 MFMatrix4f.h MFNode.h MFRotation.h MFString.h MFTime.h SFTime.h \
 MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h \
 SFBool.h SFColor.h SFColorRGBA.h SFImage.h SFInt32.h SFMatrix3f.h \
 SFMatrix4f.h SFNode.h SFRotation.h SFString.h SFVec2d.h SFVec2f.h \
 Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h CommandList.h Command.h
NodeRenderedTexture.o: NodeRenderedTexture.cpp stdafx.h config.h \
 mysnprintf.h NodeRenderedTexture.h Node.h Array.h List.h Vec3f.h Vec3x.h \
 MyString.h StringArray.h FieldIndex.h Element.h x3dFlags.h \
 swt/include/WriteFlags.h NodeFlags.h MeshFlags.h ac3dMaterialCallback.h \
 NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h ProtoMacros.h Proto.h \
 Map.h NodeList.h InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h \
 MFBool.h FieldValue.h Arrays.h MySwap.h Types.h MFColor.h MFFloat.h \
 SFFloat.h MFColorRGBA.h MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h \
 MFMatrix4f.h MFNode.h MFRotation.h MFString.h MFTime.h SFTime.h \
 MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h \
 SFBool.h SFColor.h SFColorRGBA.h SFImage.h SFInt32.h SFMatrix3f.h \
 SFMatrix4f.h SFNode.h SFRotation.h SFString.h SFVec2d.h SFVec2f.h \
 Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h CommandList.h Command.h \
 KambiCommonFunctions.h KambiTextureCommonFields.h DuneApp.h URL.h \
 PreferencesApp.h swt/include/swttypedef.h EcmaScriptApp.h \
 StereoViewApp.h swt/include/stereoview.h InputDeviceApp.h \
 TransformMode.h OutputApp.h RouteViewApp.h StartWithApp.h \
 ExportNamesApp.h ModellingApp.h RenderApp.h
NodeSuperShape.o: NodeSuperShape.cpp stdafx.h config.h mysnprintf.h \
 NodeSuperShape.h MeshMorphingNode.h Array.h Interpolator.h Node.h List.h \
 Vec3f.h Vec3x.h MyString.h StringArray.h FieldIndex.h Element.h \
 x3dFlags.h swt/include/WriteFlags.h NodeFlags.h MeshFlags.h \
 ac3dMaterialCallback.h NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h \
 ProtoMacros.h Proto.h Map.h NodeList.h InterfaceArray.h \
 DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h Arrays.h MySwap.h \
 Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h MFDouble.h \
 SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h MFRotation.h \
 MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h \
 MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h SFImage.h \
 SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h SFString.h \
 SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h \
 CommandList.h Command.h MeshBasedNode.h GeometryNode.h NodeAppearance.h \
 DuneApp.h URL.h PreferencesApp.h swt/include/swttypedef.h \
 EcmaScriptApp.h StereoViewApp.h swt/include/stereoview.h \
 InputDeviceApp.h TransformMode.h OutputApp.h RouteViewApp.h \
 StartWithApp.h ExportNamesApp.h ModellingApp.h RenderApp.h \
 NodeMaterial.h swt/include/swt.h swt/include/keycodes.h \
 swt/include/swlanguages.h swt/include/swDebugf.h \
 swt/include/swttypedef.h NodeImageTexture.h KambiTextureCommonFields.h \
 NodeTextureTransform.h GeometryNodeMacros.h Scene.h RenderState.h \
 ProtoMap.h MyMesh.h NodeIndexedFaceSet.h NodeCoordinate.h Colored.h \
 NodeColor.h NodeColorRGBA.h ComposedGeometryMacros.h FaceData.h \
 NodeNormal.h NodeTextureCoordinate.h NodeTextureCoordinateGenerator.h \
 NodeMultiTextureCoordinate.h Util.h NodeNurbsSurface.h
NodeVrmlScene.o: NodeVrmlScene.cpp stdafx.h config.h mysnprintf.h \
 NodeVrmlScene.h Node.h Array.h List.h Vec3f.h Vec3x.h MyString.h \
 StringArray.h FieldIndex.h Element.h x3dFlags.h swt/include/WriteFlags.h \
 NodeFlags.h MeshFlags.h ac3dMaterialCallback.h NodeArray.h Stack.h \
 Matrix.h Vec3d.h Quaternion.h ProtoMacros.h Proto.h Map.h NodeList.h \
 InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h \
 Arrays.h MySwap.h Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h \
 MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h \
 MFRotation.h MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h \
 MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h \
 SFImage.h SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h \
 SFString.h SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h \
 SFVec4f.h CommandList.h Command.h Scene.h URL.h DuneApp.h \
 PreferencesApp.h swt/include/swttypedef.h EcmaScriptApp.h \
 StereoViewApp.h swt/include/stereoview.h InputDeviceApp.h \
 TransformMode.h OutputApp.h RouteViewApp.h StartWithApp.h \
 ExportNamesApp.h ModellingApp.h RenderApp.h RenderState.h ProtoMap.h \
 RouteCommand.h NodeTimeSensor.h NodeViewpoint.h ViewpointNode.h \
 swt/include/swt.h swt/include/keycodes.h swt/include/swlanguages.h \
 swt/include/swDebugf.h swt/include/swttypedef.h CoverNode.h \
 KambiViewpointCommonFields.h NodeVrmlCut.h
NodeKambiAppearance.o: NodeKambiAppearance.cpp stdafx.h config.h \
 mysnprintf.h NodeKambiAppearance.h Node.h Array.h List.h Vec3f.h Vec3x.h \
 MyString.h StringArray.h FieldIndex.h Element.h x3dFlags.h \
 swt/include/WriteFlags.h NodeFlags.h MeshFlags.h ac3dMaterialCallback.h \
 NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h ProtoMacros.h Proto.h \
 Map.h NodeList.h InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h \
 MFBool.h FieldValue.h Arrays.h MySwap.h Types.h MFColor.h MFFloat.h \
 SFFloat.h MFColorRGBA.h MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h \
 MFMatrix4f.h MFNode.h MFRotation.h MFString.h MFTime.h SFTime.h \
 MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h \
 SFBool.h SFColor.h SFColorRGBA.h SFImage.h SFInt32.h SFMatrix3f.h \
 SFMatrix4f.h SFNode.h SFRotation.h SFString.h SFVec2d.h SFVec2f.h \
 Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h CommandList.h Command.h \
 DuneApp.h URL.h PreferencesApp.h swt/include/swttypedef.h \
 EcmaScriptApp.h StereoViewApp.h swt/include/stereoview.h \
 InputDeviceApp.h TransformMode.h OutputApp.h RouteViewApp.h \
 StartWithApp.h ExportNamesApp.h ModellingApp.h RenderApp.h \
 KambiCommonFunctions.h NodeAppearance.h NodeMaterial.h swt/include/swt.h \
 swt/include/keycodes.h swt/include/swlanguages.h swt/include/swDebugf.h \
 swt/include/swttypedef.h Util.h
NodeKambiHeadLight.o: NodeKambiHeadLight.cpp stdafx.h config.h \
 mysnprintf.h NodeKambiHeadLight.h Node.h Array.h List.h Vec3f.h Vec3x.h \
 MyString.h StringArray.h FieldIndex.h Element.h x3dFlags.h \
 swt/include/WriteFlags.h NodeFlags.h MeshFlags.h ac3dMaterialCallback.h \
 NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h ProtoMacros.h Proto.h \
 Map.h NodeList.h InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h \
 MFBool.h FieldValue.h Arrays.h MySwap.h Types.h MFColor.h MFFloat.h \
 SFFloat.h MFColorRGBA.h MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h \
 MFMatrix4f.h MFNode.h MFRotation.h MFString.h MFTime.h SFTime.h \
 MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h \
 SFBool.h SFColor.h SFColorRGBA.h SFImage.h SFInt32.h SFMatrix3f.h \
 SFMatrix4f.h SFNode.h SFRotation.h SFString.h SFVec2d.h SFVec2f.h \
 Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h CommandList.h Command.h \
 KambiCommonFunctions.h Scene.h URL.h DuneApp.h PreferencesApp.h \
 swt/include/swttypedef.h EcmaScriptApp.h StereoViewApp.h \
 swt/include/stereoview.h InputDeviceApp.h TransformMode.h OutputApp.h \
 RouteViewApp.h StartWithApp.h ExportNamesApp.h ModellingApp.h \
 RenderApp.h RenderState.h ProtoMap.h
NodeKambiInline.o: NodeKambiInline.cpp stdafx.h config.h mysnprintf.h \
 NodeKambiInline.h Node.h Array.h List.h Vec3f.h Vec3x.h MyString.h \
 StringArray.h FieldIndex.h Element.h x3dFlags.h swt/include/WriteFlags.h \
 NodeFlags.h MeshFlags.h ac3dMaterialCallback.h NodeArray.h Stack.h \
 Matrix.h Vec3d.h Quaternion.h ProtoMacros.h Proto.h Map.h NodeList.h \
 InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h \
 Arrays.h MySwap.h Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h \
 MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h \
 MFRotation.h MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h \
 MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h \
 SFImage.h SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h \
 SFString.h SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h \
 SFVec4f.h CommandList.h Command.h KambiCommonFunctions.h DuneApp.h URL.h \
 PreferencesApp.h swt/include/swttypedef.h EcmaScriptApp.h \
 StereoViewApp.h swt/include/stereoview.h InputDeviceApp.h \
 TransformMode.h OutputApp.h RouteViewApp.h StartWithApp.h \
 ExportNamesApp.h ModellingApp.h RenderApp.h
NodeKambiNavigationInfo.o: NodeKambiNavigationInfo.cpp stdafx.h config.h \
 mysnprintf.h NodeKambiNavigationInfo.h Node.h Array.h List.h Vec3f.h \
 Vec3x.h MyString.h StringArray.h FieldIndex.h Element.h x3dFlags.h \
 swt/include/WriteFlags.h NodeFlags.h MeshFlags.h ac3dMaterialCallback.h \
 NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h ProtoMacros.h Proto.h \
 Map.h NodeList.h InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h \
 MFBool.h FieldValue.h Arrays.h MySwap.h Types.h MFColor.h MFFloat.h \
 SFFloat.h MFColorRGBA.h MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h \
 MFMatrix4f.h MFNode.h MFRotation.h MFString.h MFTime.h SFTime.h \
 MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h \
 SFBool.h SFColor.h SFColorRGBA.h SFImage.h SFInt32.h SFMatrix3f.h \
 SFMatrix4f.h SFNode.h SFRotation.h SFString.h SFVec2d.h SFVec2f.h \
 Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h CommandList.h Command.h \
 KambiCommonFunctions.h NodeNavigationInfo.h DuneApp.h URL.h \
 PreferencesApp.h swt/include/swttypedef.h EcmaScriptApp.h \
 StereoViewApp.h swt/include/stereoview.h InputDeviceApp.h \
 TransformMode.h OutputApp.h RouteViewApp.h StartWithApp.h \
 ExportNamesApp.h ModellingApp.h RenderApp.h
NodeKambiOctreeProperties.o: NodeKambiOctreeProperties.cpp stdafx.h \
 config.h mysnprintf.h NodeKambiOctreeProperties.h Node.h Array.h List.h \
 Vec3f.h Vec3x.h MyString.h StringArray.h FieldIndex.h Element.h \
 x3dFlags.h swt/include/WriteFlags.h NodeFlags.h MeshFlags.h \
 ac3dMaterialCallback.h NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h \
 ProtoMacros.h Proto.h Map.h NodeList.h InterfaceArray.h \
 DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h Arrays.h MySwap.h \
 Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h MFDouble.h \
 SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h MFRotation.h \
 MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h \
 MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h SFImage.h \
 SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h SFString.h \
 SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h \
 CommandList.h Command.h KambiCommonFunctions.h DuneApp.h URL.h \
 PreferencesApp.h swt/include/swttypedef.h EcmaScriptApp.h \
 StereoViewApp.h swt/include/stereoview.h InputDeviceApp.h \
 TransformMode.h OutputApp.h RouteViewApp.h StartWithApp.h \
 ExportNamesApp.h ModellingApp.h RenderApp.h
NodeKambiTriangulation.o: NodeKambiTriangulation.cpp stdafx.h config.h \
 mysnprintf.h NodeKambiTriangulation.h Node.h Array.h List.h Vec3f.h \
 Vec3x.h MyString.h StringArray.h FieldIndex.h Element.h x3dFlags.h \
 swt/include/WriteFlags.h NodeFlags.h MeshFlags.h ac3dMaterialCallback.h \
 NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h ProtoMacros.h Proto.h \
 Map.h NodeList.h InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h \
 MFBool.h FieldValue.h Arrays.h MySwap.h Types.h MFColor.h MFFloat.h \
 SFFloat.h MFColorRGBA.h MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h \
 MFMatrix4f.h MFNode.h MFRotation.h MFString.h MFTime.h SFTime.h \
 MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h \
 SFBool.h SFColor.h SFColorRGBA.h SFImage.h SFInt32.h SFMatrix3f.h \
 SFMatrix4f.h SFNode.h SFRotation.h SFString.h SFVec2d.h SFVec2f.h \
 Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h CommandList.h Command.h \
 KambiCommonFunctions.h DuneApp.h URL.h PreferencesApp.h \
 swt/include/swttypedef.h EcmaScriptApp.h StereoViewApp.h \
 swt/include/stereoview.h InputDeviceApp.h TransformMode.h OutputApp.h \
 RouteViewApp.h StartWithApp.h ExportNamesApp.h ModellingApp.h \
 RenderApp.h
NodeProjectedTextureCoordinate.o: NodeProjectedTextureCoordinate.cpp \
 stdafx.h config.h mysnprintf.h NodeProjectedTextureCoordinate.h Node.h \
 Array.h List.h Vec3f.h Vec3x.h MyString.h StringArray.h FieldIndex.h \
 Element.h x3dFlags.h swt/include/WriteFlags.h NodeFlags.h MeshFlags.h \
 ac3dMaterialCallback.h NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h \
 ProtoMacros.h Proto.h Map.h NodeList.h InterfaceArray.h \
 DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h Arrays.h MySwap.h \
 Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h MFDouble.h \
 SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h MFRotation.h \
 MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h \
 MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h SFImage.h \
 SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h SFString.h \
 SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h \
 CommandList.h Command.h KambiCommonFunctions.h Scene.h URL.h DuneApp.h \
 PreferencesApp.h swt/include/swttypedef.h EcmaScriptApp.h \
 StereoViewApp.h swt/include/stereoview.h InputDeviceApp.h \
 TransformMode.h OutputApp.h RouteViewApp.h StartWithApp.h \
 ExportNamesApp.h ModellingApp.h RenderApp.h RenderState.h ProtoMap.h
NodeSuperRevolver.o: NodeSuperRevolver.cpp stdafx.h config.h mysnprintf.h \
 MyMesh.h Array.h List.h Vec4f.h Vec3f.h Vec3x.h Vec2f.h Vec3d.h \
 swt/include/swt.h swt/include/keycodes.h swt/include/swlanguages.h \
 swt/include/swDebugf.h swt/include/swttypedef.h NodeIndexedFaceSet.h \
 MeshBasedNode.h GeometryNode.h Node.h MyString.h StringArray.h \
 FieldIndex.h Element.h x3dFlags.h swt/include/WriteFlags.h NodeFlags.h \
 MeshFlags.h ac3dMaterialCallback.h NodeArray.h Stack.h Matrix.h \
 Quaternion.h NodeAppearance.h ProtoMacros.h Proto.h Map.h NodeList.h \
 InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h \
 Arrays.h MySwap.h Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h \
 MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h \
 MFRotation.h MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h \
 MFVec3d.h MFVec3f.h MFVec4f.h SFBool.h SFColor.h SFColorRGBA.h SFImage.h \
 SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h SFString.h \
 SFVec2d.h SFVec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h \
 CommandList.h Command.h DuneApp.h URL.h PreferencesApp.h \
 swt/include/swttypedef.h EcmaScriptApp.h StereoViewApp.h \
 swt/include/stereoview.h InputDeviceApp.h TransformMode.h OutputApp.h \
 RouteViewApp.h StartWithApp.h ExportNamesApp.h ModellingApp.h \
 RenderApp.h NodeMaterial.h NodeImageTexture.h KambiTextureCommonFields.h \
 NodeTextureTransform.h GeometryNodeMacros.h NodeCoordinate.h Colored.h \
 NodeColor.h NodeColorRGBA.h ComposedGeometryMacros.h FaceData.h \
 NodeNormal.h NodeTextureCoordinate.h NodeTextureCoordinateGenerator.h \
 NodeMultiTextureCoordinate.h Scene.h RenderState.h ProtoMap.h Util.h \
 NodeSuperRevolver.h NodeNurbsCurve.h NurbsCurve.h ChainBasedNode.h \
 MeshMorphingNode.h Interpolator.h NodeNurbsSurface.h \
 NurbsCurveDegreeElevate.h resource.h
NodeScreenEffect.o: NodeScreenEffect.cpp stdafx.h config.h mysnprintf.h \
 NodeScreenEffect.h Node.h Array.h List.h Vec3f.h Vec3x.h MyString.h \
 StringArray.h FieldIndex.h Element.h x3dFlags.h swt/include/WriteFlags.h \
 NodeFlags.h MeshFlags.h ac3dMaterialCallback.h NodeArray.h Stack.h \
 Matrix.h Vec3d.h Quaternion.h ProtoMacros.h Proto.h Map.h NodeList.h \
 InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h \
 Arrays.h MySwap.h Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h \
 MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h \
 MFRotation.h MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h \
 MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h \
 SFImage.h SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h \
 SFString.h SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h \
 SFVec4f.h CommandList.h Command.h DuneApp.h URL.h PreferencesApp.h \
 swt/include/swttypedef.h EcmaScriptApp.h StereoViewApp.h \
 swt/include/stereoview.h InputDeviceApp.h TransformMode.h OutputApp.h \
 RouteViewApp.h StartWithApp.h ExportNamesApp.h ModellingApp.h \
 RenderApp.h
NodeTeapot.o: NodeTeapot.cpp stdafx.h config.h mysnprintf.h NodeTeapot.h \
 Node.h Array.h List.h Vec3f.h Vec3x.h MyString.h StringArray.h \
 FieldIndex.h Element.h x3dFlags.h swt/include/WriteFlags.h NodeFlags.h \
 MeshFlags.h ac3dMaterialCallback.h NodeArray.h Stack.h Matrix.h Vec3d.h \
 Quaternion.h ProtoMacros.h Proto.h Map.h NodeList.h InterfaceArray.h \
 DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h Arrays.h MySwap.h \
 Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h MFDouble.h \
 SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h MFRotation.h \
 MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h \
 MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h SFImage.h \
 SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h SFString.h \
 SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h \
 CommandList.h Command.h KambiCommonFunctions.h DuneApp.h URL.h \
 PreferencesApp.h swt/include/swttypedef.h EcmaScriptApp.h \
 StereoViewApp.h swt/include/stereoview.h InputDeviceApp.h \
 TransformMode.h OutputApp.h RouteViewApp.h StartWithApp.h \
 ExportNamesApp.h ModellingApp.h RenderApp.h
NodeText3D.o: NodeText3D.cpp stdafx.h config.h mysnprintf.h \
 swt/include/swt.h swt/include/keycodes.h swt/include/swlanguages.h \
 swt/include/swDebugf.h swt/include/swttypedef.h NodeText3D.h Node.h \
 Array.h List.h Vec3f.h Vec3x.h MyString.h StringArray.h FieldIndex.h \
 Element.h x3dFlags.h swt/include/WriteFlags.h NodeFlags.h MeshFlags.h \
 ac3dMaterialCallback.h NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h \
 ProtoMacros.h Proto.h Map.h NodeList.h InterfaceArray.h \
 DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h Arrays.h MySwap.h \
 Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h MFDouble.h \
 SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h MFRotation.h \
 MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h \
 MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h SFImage.h \
 SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h SFString.h \
 SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h \
 CommandList.h Command.h NodeText.h GeometryNode.h NodeAppearance.h \
 DuneApp.h URL.h PreferencesApp.h swt/include/swttypedef.h \
 EcmaScriptApp.h StereoViewApp.h swt/include/stereoview.h \
 InputDeviceApp.h TransformMode.h OutputApp.h RouteViewApp.h \
 StartWithApp.h ExportNamesApp.h ModellingApp.h RenderApp.h \
 NodeMaterial.h NodeImageTexture.h KambiTextureCommonFields.h \
 NodeTextureTransform.h GeometryNodeMacros.h MeshBasedNode.h NodeShape.h \
 NodeAnchor.h NodeGroup.h StaticGroupNode.h GroupNode.h Scene.h \
 RenderState.h ProtoMap.h
NodeShaderTexture.o: NodeShaderTexture.cpp stdafx.h config.h mysnprintf.h \
 NodeShaderTexture.h Node.h Array.h List.h Vec3f.h Vec3x.h MyString.h \
 StringArray.h FieldIndex.h Element.h x3dFlags.h swt/include/WriteFlags.h \
 NodeFlags.h MeshFlags.h ac3dMaterialCallback.h NodeArray.h Stack.h \
 Matrix.h Vec3d.h Quaternion.h ProtoMacros.h Proto.h Map.h NodeList.h \
 InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h \
 Arrays.h MySwap.h Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h \
 MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h \
 MFRotation.h MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h \
 MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h \
 SFImage.h SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h \
 SFString.h SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h \
 SFVec4f.h CommandList.h Command.h DuneApp.h URL.h PreferencesApp.h \
 swt/include/swttypedef.h EcmaScriptApp.h StereoViewApp.h \
 swt/include/stereoview.h InputDeviceApp.h TransformMode.h OutputApp.h \
 RouteViewApp.h StartWithApp.h ExportNamesApp.h ModellingApp.h \
 RenderApp.h
NodeVectorInterpolator.o: NodeVectorInterpolator.cpp stdafx.h config.h \
 mysnprintf.h NodeVectorInterpolator.h Node.h Array.h List.h Vec3f.h \
 Vec3x.h MyString.h StringArray.h FieldIndex.h Element.h x3dFlags.h \
 swt/include/WriteFlags.h NodeFlags.h MeshFlags.h ac3dMaterialCallback.h \
 NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h ProtoMacros.h Proto.h \
 Map.h NodeList.h InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h \
 MFBool.h FieldValue.h Arrays.h MySwap.h Types.h MFColor.h MFFloat.h \
 SFFloat.h MFColorRGBA.h MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h \
 MFMatrix4f.h MFNode.h MFRotation.h MFString.h MFTime.h SFTime.h \
 MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h \
 SFBool.h SFColor.h SFColorRGBA.h SFImage.h SFInt32.h SFMatrix3f.h \
 SFMatrix4f.h SFNode.h SFRotation.h SFString.h SFVec2d.h SFVec2f.h \
 Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h CommandList.h Command.h \
 DuneApp.h URL.h PreferencesApp.h swt/include/swttypedef.h \
 EcmaScriptApp.h StereoViewApp.h swt/include/stereoview.h \
 InputDeviceApp.h TransformMode.h OutputApp.h RouteViewApp.h \
 StartWithApp.h ExportNamesApp.h ModellingApp.h RenderApp.h
NodeCurveAnimation.o: NodeCurveAnimation.cpp stdafx.h config.h \
 mysnprintf.h NodeCurveAnimation.h ProtoMacros.h Proto.h Array.h Map.h \
 List.h MyString.h StringArray.h Node.h Vec3f.h Vec3x.h FieldIndex.h \
 Element.h x3dFlags.h swt/include/WriteFlags.h NodeFlags.h MeshFlags.h \
 ac3dMaterialCallback.h NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h \
 NodeList.h InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h MFBool.h \
 FieldValue.h Arrays.h MySwap.h Types.h MFColor.h MFFloat.h SFFloat.h \
 MFColorRGBA.h MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h \
 MFNode.h MFRotation.h MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h \
 MFVec2f.h MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h \
 SFColorRGBA.h SFImage.h SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h \
 SFRotation.h SFString.h SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h \
 SFVec3f.h SFVec4f.h CommandList.h Command.h NodeNurbsCurve.h \
 NurbsCurve.h GeometryNode.h NodeAppearance.h DuneApp.h URL.h \
 PreferencesApp.h swt/include/swttypedef.h EcmaScriptApp.h \
 StereoViewApp.h swt/include/stereoview.h InputDeviceApp.h \
 TransformMode.h OutputApp.h RouteViewApp.h StartWithApp.h \
 ExportNamesApp.h ModellingApp.h RenderApp.h NodeMaterial.h \
 swt/include/swt.h swt/include/keycodes.h swt/include/swlanguages.h \
 swt/include/swDebugf.h swt/include/swttypedef.h NodeImageTexture.h \
 KambiTextureCommonFields.h NodeTextureTransform.h GeometryNodeMacros.h \
 ChainBasedNode.h NodePositionInterpolator.h Interpolator.h \
 NodeOrientationInterpolator.h Scene.h RenderState.h ProtoMap.h \
 NodeScalarInterpolator.h Util.h EventIn.h EventOut.h \
 NurbsCurveDegreeElevate.h
NodeSuperExtrusion.o: NodeSuperExtrusion.cpp stdafx.h config.h \
 mysnprintf.h NodeSuperExtrusion.h ProtoMacros.h Proto.h Array.h Map.h \
 List.h MyString.h StringArray.h Node.h Vec3f.h Vec3x.h FieldIndex.h \
 Element.h x3dFlags.h swt/include/WriteFlags.h NodeFlags.h MeshFlags.h \
 ac3dMaterialCallback.h NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h \
 NodeList.h InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h MFBool.h \
 FieldValue.h Arrays.h MySwap.h Types.h MFColor.h MFFloat.h SFFloat.h \
 MFColorRGBA.h MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h \
 MFNode.h MFRotation.h MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h \
 MFVec2f.h MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h \
 SFColorRGBA.h SFImage.h SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h \
 SFRotation.h SFString.h SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h \
 SFVec3f.h SFVec4f.h CommandList.h Command.h NodeExtrusion.h \
 MeshBasedNode.h GeometryNode.h NodeAppearance.h DuneApp.h URL.h \
 PreferencesApp.h swt/include/swttypedef.h EcmaScriptApp.h \
 StereoViewApp.h swt/include/stereoview.h InputDeviceApp.h \
 TransformMode.h OutputApp.h RouteViewApp.h StartWithApp.h \
 ExportNamesApp.h ModellingApp.h RenderApp.h NodeMaterial.h \
 swt/include/swt.h swt/include/keycodes.h swt/include/swlanguages.h \
 swt/include/swDebugf.h swt/include/swttypedef.h NodeImageTexture.h \
 KambiTextureCommonFields.h NodeTextureTransform.h GeometryNodeMacros.h \
 NodeNurbsCurve.h NurbsCurve.h ChainBasedNode.h MeshMorphingNode.h \
 Interpolator.h Scene.h RenderState.h ProtoMap.h NodeNurbsSurface.h \
 MyMesh.h NodeIndexedFaceSet.h NodeCoordinate.h Colored.h NodeColor.h \
 NodeColorRGBA.h ComposedGeometryMacros.h FaceData.h NodeNormal.h \
 NodeTextureCoordinate.h NodeTextureCoordinateGenerator.h \
 NodeMultiTextureCoordinate.h Util.h resource.h
NodeCOVER.o: NodeCOVER.cpp stdafx.h config.h mysnprintf.h NodeCOVER.h \
 CoverNode.h Node.h Array.h List.h Vec3f.h Vec3x.h MyString.h \
 StringArray.h FieldIndex.h Element.h x3dFlags.h swt/include/WriteFlags.h \
 NodeFlags.h MeshFlags.h ac3dMaterialCallback.h NodeArray.h Stack.h \
 Matrix.h Vec3d.h Quaternion.h ProtoMacros.h Proto.h Map.h NodeList.h \
 InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h \
 Arrays.h MySwap.h Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h \
 MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h \
 MFRotation.h MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h \
 MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h \
 SFImage.h SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h \
 SFString.h SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h \
 SFVec4f.h CommandList.h Command.h DuneApp.h URL.h PreferencesApp.h \
 swt/include/swttypedef.h EcmaScriptApp.h StereoViewApp.h \
 swt/include/stereoview.h InputDeviceApp.h TransformMode.h OutputApp.h \
 RouteViewApp.h StartWithApp.h ExportNamesApp.h ModellingApp.h \
 RenderApp.h Field.h
NodeSpaceSensor.o: NodeSpaceSensor.cpp stdafx.h config.h mysnprintf.h \
 NodeSpaceSensor.h CoverNode.h Node.h Array.h List.h Vec3f.h Vec3x.h \
 MyString.h StringArray.h FieldIndex.h Element.h x3dFlags.h \
 swt/include/WriteFlags.h NodeFlags.h MeshFlags.h ac3dMaterialCallback.h \
 NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h ProtoMacros.h Proto.h \
 Map.h NodeList.h InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h \
 MFBool.h FieldValue.h Arrays.h MySwap.h Types.h MFColor.h MFFloat.h \
 SFFloat.h MFColorRGBA.h MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h \
 MFMatrix4f.h MFNode.h MFRotation.h MFString.h MFTime.h SFTime.h \
 MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h \
 SFBool.h SFColor.h SFColorRGBA.h SFImage.h SFInt32.h SFMatrix3f.h \
 SFMatrix4f.h SFNode.h SFRotation.h SFString.h SFVec2d.h SFVec2f.h \
 Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h CommandList.h Command.h \
 DuneApp.h URL.h PreferencesApp.h swt/include/swttypedef.h \
 EcmaScriptApp.h StereoViewApp.h swt/include/stereoview.h \
 InputDeviceApp.h TransformMode.h OutputApp.h RouteViewApp.h \
 StartWithApp.h ExportNamesApp.h ModellingApp.h RenderApp.h Field.h
NodeCubeTexture.o: NodeCubeTexture.cpp stdafx.h config.h mysnprintf.h \
 NodeCubeTexture.h CoverNode.h Node.h Array.h List.h Vec3f.h Vec3x.h \
 MyString.h StringArray.h FieldIndex.h Element.h x3dFlags.h \
 swt/include/WriteFlags.h NodeFlags.h MeshFlags.h ac3dMaterialCallback.h \
 NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h ProtoMacros.h Proto.h \
 Map.h NodeList.h InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h \
 MFBool.h FieldValue.h Arrays.h MySwap.h Types.h MFColor.h MFFloat.h \
 SFFloat.h MFColorRGBA.h MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h \
 MFMatrix4f.h MFNode.h MFRotation.h MFString.h MFTime.h SFTime.h \
 MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h \
 SFBool.h SFColor.h SFColorRGBA.h SFImage.h SFInt32.h SFMatrix3f.h \
 SFMatrix4f.h SFNode.h SFRotation.h SFString.h SFVec2d.h SFVec2f.h \
 Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h CommandList.h Command.h \
 DuneApp.h URL.h PreferencesApp.h swt/include/swttypedef.h \
 EcmaScriptApp.h StereoViewApp.h swt/include/stereoview.h \
 InputDeviceApp.h TransformMode.h OutputApp.h RouteViewApp.h \
 StartWithApp.h ExportNamesApp.h ModellingApp.h RenderApp.h Field.h
NodeWave.o: NodeWave.cpp stdafx.h config.h mysnprintf.h NodeWave.h \
 CoverNode.h Node.h Array.h List.h Vec3f.h Vec3x.h MyString.h \
 StringArray.h FieldIndex.h Element.h x3dFlags.h swt/include/WriteFlags.h \
 NodeFlags.h MeshFlags.h ac3dMaterialCallback.h NodeArray.h Stack.h \
 Matrix.h Vec3d.h Quaternion.h ProtoMacros.h Proto.h Map.h NodeList.h \
 InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h \
 Arrays.h MySwap.h Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h \
 MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h \
 MFRotation.h MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h \
 MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h \
 SFImage.h SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h \
 SFString.h SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h \
 SFVec4f.h CommandList.h Command.h DuneApp.h URL.h PreferencesApp.h \
 swt/include/swttypedef.h EcmaScriptApp.h StereoViewApp.h \
 swt/include/stereoview.h InputDeviceApp.h TransformMode.h OutputApp.h \
 RouteViewApp.h StartWithApp.h ExportNamesApp.h ModellingApp.h \
 RenderApp.h Field.h
NodeJoystickSensor.o: NodeJoystickSensor.cpp stdafx.h config.h \
 mysnprintf.h NodeJoystickSensor.h Node.h Array.h List.h Vec3f.h Vec3x.h \
 MyString.h StringArray.h FieldIndex.h Element.h x3dFlags.h \
 swt/include/WriteFlags.h NodeFlags.h MeshFlags.h ac3dMaterialCallback.h \
 NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h ProtoMacros.h Proto.h \
 Map.h NodeList.h InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h \
 MFBool.h FieldValue.h Arrays.h MySwap.h Types.h MFColor.h MFFloat.h \
 SFFloat.h MFColorRGBA.h MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h \
 MFMatrix4f.h MFNode.h MFRotation.h MFString.h MFTime.h SFTime.h \
 MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h \
 SFBool.h SFColor.h SFColorRGBA.h SFImage.h SFInt32.h SFMatrix3f.h \
 SFMatrix4f.h SFNode.h SFRotation.h SFString.h SFVec2d.h SFVec2f.h \
 Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h CommandList.h Command.h \
 CoverNode.h Field.h
NodeLabView.o: NodeLabView.cpp stdafx.h config.h mysnprintf.h \
 swt/include/swt.h swt/include/keycodes.h swt/include/swlanguages.h \
 swt/include/swDebugf.h swt/include/swttypedef.h DuneApp.h List.h Array.h \
 MyString.h StringArray.h Node.h Vec3f.h Vec3x.h FieldIndex.h Element.h \
 x3dFlags.h swt/include/WriteFlags.h NodeFlags.h MeshFlags.h \
 ac3dMaterialCallback.h NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h \
 URL.h PreferencesApp.h swt/include/swttypedef.h EcmaScriptApp.h \
 StereoViewApp.h swt/include/stereoview.h InputDeviceApp.h \
 TransformMode.h OutputApp.h RouteViewApp.h StartWithApp.h \
 ExportNamesApp.h ModellingApp.h RenderApp.h resource.h NodeLabView.h \
 CoverNode.h ProtoMacros.h Proto.h Map.h NodeList.h InterfaceArray.h \
 DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h Arrays.h MySwap.h \
 Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h MFDouble.h \
 SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h MFRotation.h \
 MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h \
 MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h SFImage.h \
 SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h SFString.h \
 SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h \
 CommandList.h Command.h Texture.h
NodeSteeringWheel.o: NodeSteeringWheel.cpp stdafx.h config.h mysnprintf.h \
 NodeSteeringWheel.h CoverNode.h Node.h Array.h List.h Vec3f.h Vec3x.h \
 MyString.h StringArray.h FieldIndex.h Element.h x3dFlags.h \
 swt/include/WriteFlags.h NodeFlags.h MeshFlags.h ac3dMaterialCallback.h \
 NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h ProtoMacros.h Proto.h \
 Map.h NodeList.h InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h \
 MFBool.h FieldValue.h Arrays.h MySwap.h Types.h MFColor.h MFFloat.h \
 SFFloat.h MFColorRGBA.h MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h \
 MFMatrix4f.h MFNode.h MFRotation.h MFString.h MFTime.h SFTime.h \
 MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h \
 SFBool.h SFColor.h SFColorRGBA.h SFImage.h SFInt32.h SFMatrix3f.h \
 SFMatrix4f.h SFNode.h SFRotation.h SFString.h SFVec2d.h SFVec2f.h \
 Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h CommandList.h Command.h
NodeVehicle.o: NodeVehicle.cpp stdafx.h config.h mysnprintf.h \
 NodeVehicle.h CoverNode.h Node.h Array.h List.h Vec3f.h Vec3x.h \
 MyString.h StringArray.h FieldIndex.h Element.h x3dFlags.h \
 swt/include/WriteFlags.h NodeFlags.h MeshFlags.h ac3dMaterialCallback.h \
 NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h ProtoMacros.h Proto.h \
 Map.h NodeList.h InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h \
 MFBool.h FieldValue.h Arrays.h MySwap.h Types.h MFColor.h MFFloat.h \
 SFFloat.h MFColorRGBA.h MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h \
 MFMatrix4f.h MFNode.h MFRotation.h MFString.h MFTime.h SFTime.h \
 MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h \
 SFBool.h SFColor.h SFColorRGBA.h SFImage.h SFInt32.h SFMatrix3f.h \
 SFMatrix4f.h SFNode.h SFRotation.h SFString.h SFVec2d.h SFVec2f.h \
 Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h CommandList.h Command.h
NodeARSensor.o: NodeARSensor.cpp stdafx.h config.h mysnprintf.h DuneApp.h \
 List.h Array.h MyString.h StringArray.h Node.h Vec3f.h Vec3x.h \
 FieldIndex.h Element.h x3dFlags.h swt/include/WriteFlags.h NodeFlags.h \
 MeshFlags.h ac3dMaterialCallback.h NodeArray.h Stack.h Matrix.h Vec3d.h \
 Quaternion.h URL.h PreferencesApp.h swt/include/swttypedef.h \
 EcmaScriptApp.h StereoViewApp.h swt/include/stereoview.h \
 InputDeviceApp.h TransformMode.h OutputApp.h RouteViewApp.h \
 StartWithApp.h ExportNamesApp.h ModellingApp.h RenderApp.h \
 NodeARSensor.h CoverNode.h ProtoMacros.h Proto.h Map.h NodeList.h \
 InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h \
 Arrays.h MySwap.h Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h \
 MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h \
 MFRotation.h MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h \
 MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h \
 SFImage.h SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h \
 SFString.h SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h \
 SFVec4f.h CommandList.h Command.h Scene.h RenderState.h ProtoMap.h \
 Field.h ExposedField.h
NodeSky.o: NodeSky.cpp stdafx.h config.h mysnprintf.h NodeSky.h \
 CoverNode.h Node.h Array.h List.h Vec3f.h Vec3x.h MyString.h \
 StringArray.h FieldIndex.h Element.h x3dFlags.h swt/include/WriteFlags.h \
 NodeFlags.h MeshFlags.h ac3dMaterialCallback.h NodeArray.h Stack.h \
 Matrix.h Vec3d.h Quaternion.h ProtoMacros.h Proto.h Map.h NodeList.h \
 InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h \
 Arrays.h MySwap.h Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h \
 MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h \
 MFRotation.h MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h \
 MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h \
 SFImage.h SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h \
 SFString.h SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h \
 SFVec4f.h CommandList.h Command.h
NodeTUIButton.o: NodeTUIButton.cpp stdafx.h config.h mysnprintf.h \
 NodeTUIButton.h Node.h Array.h List.h Vec3f.h Vec3x.h MyString.h \
 StringArray.h FieldIndex.h Element.h x3dFlags.h swt/include/WriteFlags.h \
 NodeFlags.h MeshFlags.h ac3dMaterialCallback.h NodeArray.h Stack.h \
 Matrix.h Vec3d.h Quaternion.h ProtoMacros.h Proto.h Map.h NodeList.h \
 InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h \
 Arrays.h MySwap.h Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h \
 MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h \
 MFRotation.h MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h \
 MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h \
 SFImage.h SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h \
 SFString.h SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h \
 SFVec4f.h CommandList.h Command.h TUIElement.h CoverNode.h DuneApp.h \
 URL.h PreferencesApp.h swt/include/swttypedef.h EcmaScriptApp.h \
 StereoViewApp.h swt/include/stereoview.h InputDeviceApp.h \
 TransformMode.h OutputApp.h RouteViewApp.h StartWithApp.h \
 ExportNamesApp.h ModellingApp.h RenderApp.h Field.h
NodeTUIComboBox.o: NodeTUIComboBox.cpp stdafx.h config.h mysnprintf.h \
 NodeTUIComboBox.h Node.h Array.h List.h Vec3f.h Vec3x.h MyString.h \
 StringArray.h FieldIndex.h Element.h x3dFlags.h swt/include/WriteFlags.h \
 NodeFlags.h MeshFlags.h ac3dMaterialCallback.h NodeArray.h Stack.h \
 Matrix.h Vec3d.h Quaternion.h ProtoMacros.h Proto.h Map.h NodeList.h \
 InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h \
 Arrays.h MySwap.h Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h \
 MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h \
 MFRotation.h MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h \
 MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h \
 SFImage.h SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h \
 SFString.h SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h \
 SFVec4f.h CommandList.h Command.h TUIElement.h CoverNode.h DuneApp.h \
 URL.h PreferencesApp.h swt/include/swttypedef.h EcmaScriptApp.h \
 StereoViewApp.h swt/include/stereoview.h InputDeviceApp.h \
 TransformMode.h OutputApp.h RouteViewApp.h StartWithApp.h \
 ExportNamesApp.h ModellingApp.h RenderApp.h Field.h
NodeTUIFloatSlider.o: NodeTUIFloatSlider.cpp stdafx.h config.h \
 mysnprintf.h NodeTUIFloatSlider.h Node.h Array.h List.h Vec3f.h Vec3x.h \
 MyString.h StringArray.h FieldIndex.h Element.h x3dFlags.h \
 swt/include/WriteFlags.h NodeFlags.h MeshFlags.h ac3dMaterialCallback.h \
 NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h ProtoMacros.h Proto.h \
 Map.h NodeList.h InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h \
 MFBool.h FieldValue.h Arrays.h MySwap.h Types.h MFColor.h MFFloat.h \
 SFFloat.h MFColorRGBA.h MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h \
 MFMatrix4f.h MFNode.h MFRotation.h MFString.h MFTime.h SFTime.h \
 MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h \
 SFBool.h SFColor.h SFColorRGBA.h SFImage.h SFInt32.h SFMatrix3f.h \
 SFMatrix4f.h SFNode.h SFRotation.h SFString.h SFVec2d.h SFVec2f.h \
 Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h CommandList.h Command.h \
 TUIElement.h CoverNode.h DuneApp.h URL.h PreferencesApp.h \
 swt/include/swttypedef.h EcmaScriptApp.h StereoViewApp.h \
 swt/include/stereoview.h InputDeviceApp.h TransformMode.h OutputApp.h \
 RouteViewApp.h StartWithApp.h ExportNamesApp.h ModellingApp.h \
 RenderApp.h Field.h
NodeTUIFrame.o: NodeTUIFrame.cpp stdafx.h config.h mysnprintf.h \
 NodeTUIFrame.h Node.h Array.h List.h Vec3f.h Vec3x.h MyString.h \
 StringArray.h FieldIndex.h Element.h x3dFlags.h swt/include/WriteFlags.h \
 NodeFlags.h MeshFlags.h ac3dMaterialCallback.h NodeArray.h Stack.h \
 Matrix.h Vec3d.h Quaternion.h ProtoMacros.h Proto.h Map.h NodeList.h \
 InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h \
 Arrays.h MySwap.h Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h \
 MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h \
 MFRotation.h MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h \
 MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h \
 SFImage.h SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h \
 SFString.h SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h \
 SFVec4f.h CommandList.h Command.h TUIElement.h CoverNode.h DuneApp.h \
 URL.h PreferencesApp.h swt/include/swttypedef.h EcmaScriptApp.h \
 StereoViewApp.h swt/include/stereoview.h InputDeviceApp.h \
 TransformMode.h OutputApp.h RouteViewApp.h StartWithApp.h \
 ExportNamesApp.h ModellingApp.h RenderApp.h Field.h
NodeTUILabel.o: NodeTUILabel.cpp stdafx.h config.h mysnprintf.h \
 NodeTUILabel.h Node.h Array.h List.h Vec3f.h Vec3x.h MyString.h \
 StringArray.h FieldIndex.h Element.h x3dFlags.h swt/include/WriteFlags.h \
 NodeFlags.h MeshFlags.h ac3dMaterialCallback.h NodeArray.h Stack.h \
 Matrix.h Vec3d.h Quaternion.h ProtoMacros.h Proto.h Map.h NodeList.h \
 InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h \
 Arrays.h MySwap.h Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h \
 MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h \
 MFRotation.h MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h \
 MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h \
 SFImage.h SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h \
 SFString.h SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h \
 SFVec4f.h CommandList.h Command.h TUIElement.h CoverNode.h DuneApp.h \
 URL.h PreferencesApp.h swt/include/swttypedef.h EcmaScriptApp.h \
 StereoViewApp.h swt/include/stereoview.h InputDeviceApp.h \
 TransformMode.h OutputApp.h RouteViewApp.h StartWithApp.h \
 ExportNamesApp.h ModellingApp.h RenderApp.h Field.h
NodeTUIListBox.o: NodeTUIListBox.cpp stdafx.h config.h mysnprintf.h \
 NodeTUIListBox.h Node.h Array.h List.h Vec3f.h Vec3x.h MyString.h \
 StringArray.h FieldIndex.h Element.h x3dFlags.h swt/include/WriteFlags.h \
 NodeFlags.h MeshFlags.h ac3dMaterialCallback.h NodeArray.h Stack.h \
 Matrix.h Vec3d.h Quaternion.h ProtoMacros.h Proto.h Map.h NodeList.h \
 InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h \
 Arrays.h MySwap.h Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h \
 MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h \
 MFRotation.h MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h \
 MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h \
 SFImage.h SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h \
 SFString.h SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h \
 SFVec4f.h CommandList.h Command.h TUIElement.h CoverNode.h DuneApp.h \
 URL.h PreferencesApp.h swt/include/swttypedef.h EcmaScriptApp.h \
 StereoViewApp.h swt/include/stereoview.h InputDeviceApp.h \
 TransformMode.h OutputApp.h RouteViewApp.h StartWithApp.h \
 ExportNamesApp.h ModellingApp.h RenderApp.h Field.h
NodeTUIMap.o: NodeTUIMap.cpp stdafx.h config.h mysnprintf.h NodeTUIMap.h \
 Node.h Array.h List.h Vec3f.h Vec3x.h MyString.h StringArray.h \
 FieldIndex.h Element.h x3dFlags.h swt/include/WriteFlags.h NodeFlags.h \
 MeshFlags.h ac3dMaterialCallback.h NodeArray.h Stack.h Matrix.h Vec3d.h \
 Quaternion.h ProtoMacros.h Proto.h Map.h NodeList.h InterfaceArray.h \
 DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h Arrays.h MySwap.h \
 Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h MFDouble.h \
 SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h MFRotation.h \
 MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h \
 MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h SFImage.h \
 SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h SFString.h \
 SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h \
 CommandList.h Command.h TUIElement.h CoverNode.h DuneApp.h URL.h \
 PreferencesApp.h swt/include/swttypedef.h EcmaScriptApp.h \
 StereoViewApp.h swt/include/stereoview.h InputDeviceApp.h \
 TransformMode.h OutputApp.h RouteViewApp.h StartWithApp.h \
 ExportNamesApp.h ModellingApp.h RenderApp.h Field.h
NodeTUIProgressBar.o: NodeTUIProgressBar.cpp stdafx.h config.h \
 mysnprintf.h NodeTUIProgressBar.h Node.h Array.h List.h Vec3f.h Vec3x.h \
 MyString.h StringArray.h FieldIndex.h Element.h x3dFlags.h \
 swt/include/WriteFlags.h NodeFlags.h MeshFlags.h ac3dMaterialCallback.h \
 NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h ProtoMacros.h Proto.h \
 Map.h NodeList.h InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h \
 MFBool.h FieldValue.h Arrays.h MySwap.h Types.h MFColor.h MFFloat.h \
 SFFloat.h MFColorRGBA.h MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h \
 MFMatrix4f.h MFNode.h MFRotation.h MFString.h MFTime.h SFTime.h \
 MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h \
 SFBool.h SFColor.h SFColorRGBA.h SFImage.h SFInt32.h SFMatrix3f.h \
 SFMatrix4f.h SFNode.h SFRotation.h SFString.h SFVec2d.h SFVec2f.h \
 Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h CommandList.h Command.h \
 TUIElement.h CoverNode.h DuneApp.h URL.h PreferencesApp.h \
 swt/include/swttypedef.h EcmaScriptApp.h StereoViewApp.h \
 swt/include/stereoview.h InputDeviceApp.h TransformMode.h OutputApp.h \
 RouteViewApp.h StartWithApp.h ExportNamesApp.h ModellingApp.h \
 RenderApp.h Field.h
NodeTUISlider.o: NodeTUISlider.cpp stdafx.h config.h mysnprintf.h \
 NodeTUISlider.h Node.h Array.h List.h Vec3f.h Vec3x.h MyString.h \
 StringArray.h FieldIndex.h Element.h x3dFlags.h swt/include/WriteFlags.h \
 NodeFlags.h MeshFlags.h ac3dMaterialCallback.h NodeArray.h Stack.h \
 Matrix.h Vec3d.h Quaternion.h ProtoMacros.h Proto.h Map.h NodeList.h \
 InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h \
 Arrays.h MySwap.h Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h \
 MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h \
 MFRotation.h MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h \
 MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h \
 SFImage.h SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h \
 SFString.h SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h \
 SFVec4f.h CommandList.h Command.h TUIElement.h CoverNode.h DuneApp.h \
 URL.h PreferencesApp.h swt/include/swttypedef.h EcmaScriptApp.h \
 StereoViewApp.h swt/include/stereoview.h InputDeviceApp.h \
 TransformMode.h OutputApp.h RouteViewApp.h StartWithApp.h \
 ExportNamesApp.h ModellingApp.h RenderApp.h Field.h
NodeTUISplitter.o: NodeTUISplitter.cpp stdafx.h config.h mysnprintf.h \
 NodeTUISplitter.h Node.h Array.h List.h Vec3f.h Vec3x.h MyString.h \
 StringArray.h FieldIndex.h Element.h x3dFlags.h swt/include/WriteFlags.h \
 NodeFlags.h MeshFlags.h ac3dMaterialCallback.h NodeArray.h Stack.h \
 Matrix.h Vec3d.h Quaternion.h ProtoMacros.h Proto.h Map.h NodeList.h \
 InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h \
 Arrays.h MySwap.h Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h \
 MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h \
 MFRotation.h MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h \
 MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h \
 SFImage.h SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h \
 SFString.h SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h \
 SFVec4f.h CommandList.h Command.h TUIElement.h CoverNode.h DuneApp.h \
 URL.h PreferencesApp.h swt/include/swttypedef.h EcmaScriptApp.h \
 StereoViewApp.h swt/include/stereoview.h InputDeviceApp.h \
 TransformMode.h OutputApp.h RouteViewApp.h StartWithApp.h \
 ExportNamesApp.h ModellingApp.h RenderApp.h Field.h
NodeTUITab.o: NodeTUITab.cpp stdafx.h config.h mysnprintf.h NodeTUITab.h \
 Node.h Array.h List.h Vec3f.h Vec3x.h MyString.h StringArray.h \
 FieldIndex.h Element.h x3dFlags.h swt/include/WriteFlags.h NodeFlags.h \
 MeshFlags.h ac3dMaterialCallback.h NodeArray.h Stack.h Matrix.h Vec3d.h \
 Quaternion.h ProtoMacros.h Proto.h Map.h NodeList.h InterfaceArray.h \
 DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h Arrays.h MySwap.h \
 Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h MFDouble.h \
 SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h MFRotation.h \
 MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h \
 MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h SFImage.h \
 SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h SFString.h \
 SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h \
 CommandList.h Command.h TUIElement.h CoverNode.h DuneApp.h URL.h \
 PreferencesApp.h swt/include/swttypedef.h EcmaScriptApp.h \
 StereoViewApp.h swt/include/stereoview.h InputDeviceApp.h \
 TransformMode.h OutputApp.h RouteViewApp.h StartWithApp.h \
 ExportNamesApp.h ModellingApp.h RenderApp.h Field.h
NodeTUITabFolder.o: NodeTUITabFolder.cpp stdafx.h config.h mysnprintf.h \
 NodeTUITabFolder.h Node.h Array.h List.h Vec3f.h Vec3x.h MyString.h \
 StringArray.h FieldIndex.h Element.h x3dFlags.h swt/include/WriteFlags.h \
 NodeFlags.h MeshFlags.h ac3dMaterialCallback.h NodeArray.h Stack.h \
 Matrix.h Vec3d.h Quaternion.h ProtoMacros.h Proto.h Map.h NodeList.h \
 InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h \
 Arrays.h MySwap.h Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h \
 MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h \
 MFRotation.h MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h \
 MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h \
 SFImage.h SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h \
 SFString.h SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h \
 SFVec4f.h CommandList.h Command.h TUIElement.h CoverNode.h DuneApp.h \
 URL.h PreferencesApp.h swt/include/swttypedef.h EcmaScriptApp.h \
 StereoViewApp.h swt/include/stereoview.h InputDeviceApp.h \
 TransformMode.h OutputApp.h RouteViewApp.h StartWithApp.h \
 ExportNamesApp.h ModellingApp.h RenderApp.h Field.h
NodeTUIToggleButton.o: NodeTUIToggleButton.cpp stdafx.h config.h \
 mysnprintf.h NodeTUIToggleButton.h Node.h Array.h List.h Vec3f.h Vec3x.h \
 MyString.h StringArray.h FieldIndex.h Element.h x3dFlags.h \
 swt/include/WriteFlags.h NodeFlags.h MeshFlags.h ac3dMaterialCallback.h \
 NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h ProtoMacros.h Proto.h \
 Map.h NodeList.h InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h \
 MFBool.h FieldValue.h Arrays.h MySwap.h Types.h MFColor.h MFFloat.h \
 SFFloat.h MFColorRGBA.h MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h \
 MFMatrix4f.h MFNode.h MFRotation.h MFString.h MFTime.h SFTime.h \
 MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h \
 SFBool.h SFColor.h SFColorRGBA.h SFImage.h SFInt32.h SFMatrix3f.h \
 SFMatrix4f.h SFNode.h SFRotation.h SFString.h SFVec2d.h SFVec2f.h \
 Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h CommandList.h Command.h \
 TUIElement.h CoverNode.h DuneApp.h URL.h PreferencesApp.h \
 swt/include/swttypedef.h EcmaScriptApp.h StereoViewApp.h \
 swt/include/stereoview.h InputDeviceApp.h TransformMode.h OutputApp.h \
 RouteViewApp.h StartWithApp.h ExportNamesApp.h ModellingApp.h \
 RenderApp.h Field.h
NodeVirtualAcoustics.o: NodeVirtualAcoustics.cpp stdafx.h config.h \
 mysnprintf.h NodeVirtualAcoustics.h CoverNode.h Node.h Array.h List.h \
 Vec3f.h Vec3x.h MyString.h StringArray.h FieldIndex.h Element.h \
 x3dFlags.h swt/include/WriteFlags.h NodeFlags.h MeshFlags.h \
 ac3dMaterialCallback.h NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h \
 ProtoMacros.h Proto.h Map.h NodeList.h InterfaceArray.h \
 DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h Arrays.h MySwap.h \
 Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h MFDouble.h \
 SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h MFRotation.h \
 MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h \
 MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h SFImage.h \
 SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h SFString.h \
 SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h \
 CommandList.h Command.h
NodeVirtualSoundSource.o: NodeVirtualSoundSource.cpp stdafx.h config.h \
 mysnprintf.h NodeVirtualSoundSource.h CoverNode.h Node.h Array.h List.h \
 Vec3f.h Vec3x.h MyString.h StringArray.h FieldIndex.h Element.h \
 x3dFlags.h swt/include/WriteFlags.h NodeFlags.h MeshFlags.h \
 ac3dMaterialCallback.h NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h \
 ProtoMacros.h Proto.h Map.h NodeList.h InterfaceArray.h \
 DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h Arrays.h MySwap.h \
 Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h MFDouble.h \
 SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h MFRotation.h \
 MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h \
 MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h SFImage.h \
 SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h SFString.h \
 SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h \
 CommandList.h Command.h RenderState.h swt/include/swttypedef.h Util.h \
 Scene.h URL.h DuneApp.h PreferencesApp.h EcmaScriptApp.h StereoViewApp.h \
 swt/include/stereoview.h InputDeviceApp.h TransformMode.h OutputApp.h \
 RouteViewApp.h StartWithApp.h ExportNamesApp.h ModellingApp.h \
 RenderApp.h ProtoMap.h
NodeCubicBezier2DOrientationInterpolator.o: \
 NodeCubicBezier2DOrientationInterpolator.cpp stdafx.h config.h \
 mysnprintf.h NodeCubicBezier2DOrientationInterpolator.h Node.h Array.h \
 List.h Vec3f.h Vec3x.h MyString.h StringArray.h FieldIndex.h Element.h \
 x3dFlags.h swt/include/WriteFlags.h NodeFlags.h MeshFlags.h \
 ac3dMaterialCallback.h NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h \
 ProtoMacros.h Proto.h Map.h NodeList.h InterfaceArray.h \
 DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h Arrays.h MySwap.h \
 Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h MFDouble.h \
 SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h MFRotation.h \
 MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h \
 MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h SFImage.h \
 SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h SFString.h \
 SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h \
 CommandList.h Command.h DuneApp.h URL.h PreferencesApp.h \
 swt/include/swttypedef.h EcmaScriptApp.h StereoViewApp.h \
 swt/include/stereoview.h InputDeviceApp.h TransformMode.h OutputApp.h \
 RouteViewApp.h StartWithApp.h ExportNamesApp.h ModellingApp.h \
 RenderApp.h
NodeCubicBezierPositionInterpolator.o: \
 NodeCubicBezierPositionInterpolator.cpp stdafx.h config.h mysnprintf.h \
 NodeCubicBezierPositionInterpolator.h Node.h Array.h List.h Vec3f.h \
 Vec3x.h MyString.h StringArray.h FieldIndex.h Element.h x3dFlags.h \
 swt/include/WriteFlags.h NodeFlags.h MeshFlags.h ac3dMaterialCallback.h \
 NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h ProtoMacros.h Proto.h \
 Map.h NodeList.h InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h \
 MFBool.h FieldValue.h Arrays.h MySwap.h Types.h MFColor.h MFFloat.h \
 SFFloat.h MFColorRGBA.h MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h \
 MFMatrix4f.h MFNode.h MFRotation.h MFString.h MFTime.h SFTime.h \
 MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h \
 SFBool.h SFColor.h SFColorRGBA.h SFImage.h SFInt32.h SFMatrix3f.h \
 SFMatrix4f.h SFNode.h SFRotation.h SFString.h SFVec2d.h SFVec2f.h \
 Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h CommandList.h Command.h \
 DuneApp.h URL.h PreferencesApp.h swt/include/swttypedef.h \
 EcmaScriptApp.h StereoViewApp.h swt/include/stereoview.h \
 InputDeviceApp.h TransformMode.h OutputApp.h RouteViewApp.h \
 StartWithApp.h ExportNamesApp.h ModellingApp.h RenderApp.h
NodeBlendMode.o: NodeBlendMode.cpp stdafx.h config.h mysnprintf.h \
 NodeBlendMode.h Node.h Array.h List.h Vec3f.h Vec3x.h MyString.h \
 StringArray.h FieldIndex.h Element.h x3dFlags.h swt/include/WriteFlags.h \
 NodeFlags.h MeshFlags.h ac3dMaterialCallback.h NodeArray.h Stack.h \
 Matrix.h Vec3d.h Quaternion.h ProtoMacros.h Proto.h Map.h NodeList.h \
 InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h \
 Arrays.h MySwap.h Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h \
 MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h \
 MFRotation.h MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h \
 MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h \
 SFImage.h SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h \
 SFString.h SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h \
 SFVec4f.h CommandList.h Command.h KambiCommonFunctions.h DuneApp.h URL.h \
 PreferencesApp.h swt/include/swttypedef.h EcmaScriptApp.h \
 StereoViewApp.h swt/include/stereoview.h InputDeviceApp.h \
 TransformMode.h OutputApp.h RouteViewApp.h StartWithApp.h \
 ExportNamesApp.h ModellingApp.h RenderApp.h
NodeColorSetInterpolator.o: NodeColorSetInterpolator.cpp stdafx.h \
 config.h mysnprintf.h NodeColorSetInterpolator.h Node.h Array.h List.h \
 Vec3f.h Vec3x.h MyString.h StringArray.h FieldIndex.h Element.h \
 x3dFlags.h swt/include/WriteFlags.h NodeFlags.h MeshFlags.h \
 ac3dMaterialCallback.h NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h \
 ProtoMacros.h Proto.h Map.h NodeList.h InterfaceArray.h \
 DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h Arrays.h MySwap.h \
 Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h MFDouble.h \
 SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h MFRotation.h \
 MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h \
 MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h SFImage.h \
 SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h SFString.h \
 SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h \
 CommandList.h Command.h DuneApp.h URL.h PreferencesApp.h \
 swt/include/swttypedef.h EcmaScriptApp.h StereoViewApp.h \
 swt/include/stereoview.h InputDeviceApp.h TransformMode.h OutputApp.h \
 RouteViewApp.h StartWithApp.h ExportNamesApp.h ModellingApp.h \
 RenderApp.h
NodeEffect.o: NodeEffect.cpp stdafx.h config.h mysnprintf.h NodeEffect.h \
 Node.h Array.h List.h Vec3f.h Vec3x.h MyString.h StringArray.h \
 FieldIndex.h Element.h x3dFlags.h swt/include/WriteFlags.h NodeFlags.h \
 MeshFlags.h ac3dMaterialCallback.h NodeArray.h Stack.h Matrix.h Vec3d.h \
 Quaternion.h ProtoMacros.h Proto.h Map.h NodeList.h InterfaceArray.h \
 DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h Arrays.h MySwap.h \
 Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h MFDouble.h \
 SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h MFRotation.h \
 MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h \
 MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h SFImage.h \
 SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h SFString.h \
 SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h \
 CommandList.h Command.h DuneApp.h URL.h PreferencesApp.h \
 swt/include/swttypedef.h EcmaScriptApp.h StereoViewApp.h \
 swt/include/stereoview.h InputDeviceApp.h TransformMode.h OutputApp.h \
 RouteViewApp.h StartWithApp.h ExportNamesApp.h ModellingApp.h \
 RenderApp.h
NodeEffectPart.o: NodeEffectPart.cpp stdafx.h config.h mysnprintf.h \
 NodeEffectPart.h Node.h Array.h List.h Vec3f.h Vec3x.h MyString.h \
 StringArray.h FieldIndex.h Element.h x3dFlags.h swt/include/WriteFlags.h \
 NodeFlags.h MeshFlags.h ac3dMaterialCallback.h NodeArray.h Stack.h \
 Matrix.h Vec3d.h Quaternion.h ProtoMacros.h Proto.h Map.h NodeList.h \
 InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h \
 Arrays.h MySwap.h Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h \
 MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h \
 MFRotation.h MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h \
 MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h \
 SFImage.h SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h \
 SFString.h SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h \
 SFVec4f.h CommandList.h Command.h DuneApp.h URL.h PreferencesApp.h \
 swt/include/swttypedef.h EcmaScriptApp.h StereoViewApp.h \
 swt/include/stereoview.h InputDeviceApp.h TransformMode.h OutputApp.h \
 RouteViewApp.h StartWithApp.h ExportNamesApp.h ModellingApp.h \
 RenderApp.h
NodeBinaryGeometry.o: NodeBinaryGeometry.cpp NodeBinaryGeometry.h Node.h \
 Array.h stdafx.h config.h mysnprintf.h List.h Vec3f.h Vec3x.h MyString.h \
 StringArray.h FieldIndex.h Element.h x3dFlags.h swt/include/WriteFlags.h \
 NodeFlags.h MeshFlags.h ac3dMaterialCallback.h NodeArray.h Stack.h \
 Matrix.h Vec3d.h Quaternion.h ProtoMacros.h Proto.h Map.h NodeList.h \
 InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h \
 Arrays.h MySwap.h Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h \
 MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h \
 MFRotation.h MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h \
 MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h \
 SFImage.h SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h \
 SFString.h SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h \
 SFVec4f.h CommandList.h Command.h DuneApp.h URL.h PreferencesApp.h \
 swt/include/swttypedef.h EcmaScriptApp.h StereoViewApp.h \
 swt/include/stereoview.h InputDeviceApp.h TransformMode.h OutputApp.h \
 RouteViewApp.h StartWithApp.h ExportNamesApp.h ModellingApp.h \
 RenderApp.h GeometryNode.h NodeAppearance.h NodeMaterial.h \
 swt/include/swt.h swt/include/keycodes.h swt/include/swlanguages.h \
 swt/include/swDebugf.h swt/include/swttypedef.h NodeImageTexture.h \
 KambiTextureCommonFields.h NodeTextureTransform.h GeometryNodeMacros.h \
 Scene.h RenderState.h ProtoMap.h
NodeBlock.o: NodeBlock.cpp stdafx.h config.h mysnprintf.h NodeBlock.h \
 Node.h Array.h List.h Vec3f.h Vec3x.h MyString.h StringArray.h \
 FieldIndex.h Element.h x3dFlags.h swt/include/WriteFlags.h NodeFlags.h \
 MeshFlags.h ac3dMaterialCallback.h NodeArray.h Stack.h Matrix.h Vec3d.h \
 Quaternion.h ProtoMacros.h Proto.h Map.h NodeList.h InterfaceArray.h \
 DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h Arrays.h MySwap.h \
 Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h MFDouble.h \
 SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h MFRotation.h \
 MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h \
 MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h SFImage.h \
 SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h SFString.h \
 SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h \
 CommandList.h Command.h NodeGroup.h StaticGroupNode.h GroupNode.h \
 DuneApp.h URL.h PreferencesApp.h swt/include/swttypedef.h \
 EcmaScriptApp.h StereoViewApp.h swt/include/stereoview.h \
 InputDeviceApp.h TransformMode.h OutputApp.h RouteViewApp.h \
 StartWithApp.h ExportNamesApp.h ModellingApp.h RenderApp.h
NodeColorMaskMode.o: NodeColorMaskMode.cpp NodeColorMaskMode.h Node.h \
 Array.h stdafx.h config.h mysnprintf.h List.h Vec3f.h Vec3x.h MyString.h \
 StringArray.h FieldIndex.h Element.h x3dFlags.h swt/include/WriteFlags.h \
 NodeFlags.h MeshFlags.h ac3dMaterialCallback.h NodeArray.h Stack.h \
 Matrix.h Vec3d.h Quaternion.h ProtoMacros.h Proto.h Map.h NodeList.h \
 InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h \
 Arrays.h MySwap.h Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h \
 MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h \
 MFRotation.h MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h \
 MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h \
 SFImage.h SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h \
 SFString.h SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h \
 SFVec4f.h CommandList.h Command.h DuneApp.h URL.h PreferencesApp.h \
 swt/include/swttypedef.h EcmaScriptApp.h StereoViewApp.h \
 swt/include/stereoview.h InputDeviceApp.h TransformMode.h OutputApp.h \
 RouteViewApp.h StartWithApp.h ExportNamesApp.h ModellingApp.h \
 RenderApp.h
NodeDepthMode.o: NodeDepthMode.cpp NodeDepthMode.h Node.h Array.h \
 stdafx.h config.h mysnprintf.h List.h Vec3f.h Vec3x.h MyString.h \
 StringArray.h FieldIndex.h Element.h x3dFlags.h swt/include/WriteFlags.h \
 NodeFlags.h MeshFlags.h ac3dMaterialCallback.h NodeArray.h Stack.h \
 Matrix.h Vec3d.h Quaternion.h ProtoMacros.h Proto.h Map.h NodeList.h \
 InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h \
 Arrays.h MySwap.h Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h \
 MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h \
 MFRotation.h MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h \
 MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h \
 SFImage.h SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h \
 SFString.h SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h \
 SFVec4f.h CommandList.h Command.h DuneApp.h URL.h PreferencesApp.h \
 swt/include/swttypedef.h EcmaScriptApp.h StereoViewApp.h \
 swt/include/stereoview.h InputDeviceApp.h TransformMode.h OutputApp.h \
 RouteViewApp.h StartWithApp.h ExportNamesApp.h ModellingApp.h \
 RenderApp.h
NodeDish.o: NodeDish.cpp stdafx.h config.h mysnprintf.h NodeDish.h \
 ProtoMacros.h Proto.h Array.h Map.h List.h MyString.h StringArray.h \
 Node.h Vec3f.h Vec3x.h FieldIndex.h Element.h x3dFlags.h \
 swt/include/WriteFlags.h NodeFlags.h MeshFlags.h ac3dMaterialCallback.h \
 NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h NodeList.h \
 InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h \
 Arrays.h MySwap.h Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h \
 MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h \
 MFRotation.h MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h \
 MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h \
 SFImage.h SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h \
 SFString.h SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h \
 SFVec4f.h CommandList.h Command.h GeometryNode.h NodeAppearance.h \
 DuneApp.h URL.h PreferencesApp.h swt/include/swttypedef.h \
 EcmaScriptApp.h StereoViewApp.h swt/include/stereoview.h \
 InputDeviceApp.h TransformMode.h OutputApp.h RouteViewApp.h \
 StartWithApp.h ExportNamesApp.h ModellingApp.h RenderApp.h \
 NodeMaterial.h swt/include/swt.h swt/include/keycodes.h \
 swt/include/swlanguages.h swt/include/swDebugf.h \
 swt/include/swttypedef.h NodeImageTexture.h KambiTextureCommonFields.h \
 NodeTextureTransform.h GeometryNodeMacros.h Scene.h RenderState.h \
 ProtoMap.h
NodeDynamicLOD.o: NodeDynamicLOD.cpp stdafx.h config.h mysnprintf.h \
 swt/include/swt.h swt/include/keycodes.h swt/include/swlanguages.h \
 swt/include/swDebugf.h swt/include/swttypedef.h NodeDynamicLOD.h Node.h \
 Array.h List.h Vec3f.h Vec3x.h MyString.h StringArray.h FieldIndex.h \
 Element.h x3dFlags.h swt/include/WriteFlags.h NodeFlags.h MeshFlags.h \
 ac3dMaterialCallback.h NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h \
 ProtoMacros.h Proto.h Map.h NodeList.h InterfaceArray.h \
 DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h Arrays.h MySwap.h \
 Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h MFDouble.h \
 SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h MFRotation.h \
 MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h \
 MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h SFImage.h \
 SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h SFString.h \
 SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h \
 CommandList.h Command.h NodeLOD.h NodeShape.h Scene.h URL.h DuneApp.h \
 PreferencesApp.h swt/include/swttypedef.h EcmaScriptApp.h \
 StereoViewApp.h swt/include/stereoview.h InputDeviceApp.h \
 TransformMode.h OutputApp.h RouteViewApp.h StartWithApp.h \
 ExportNamesApp.h ModellingApp.h RenderApp.h RenderState.h ProtoMap.h
NodeEnvironment.o: NodeEnvironment.cpp stdafx.h config.h mysnprintf.h \
 NodeEnvironment.h Node.h Array.h List.h Vec3f.h Vec3x.h MyString.h \
 StringArray.h FieldIndex.h Element.h x3dFlags.h swt/include/WriteFlags.h \
 NodeFlags.h MeshFlags.h ac3dMaterialCallback.h NodeArray.h Stack.h \
 Matrix.h Vec3d.h Quaternion.h ProtoMacros.h Proto.h Map.h NodeList.h \
 InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h \
 Arrays.h MySwap.h Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h \
 MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h \
 MFRotation.h MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h \
 MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h \
 SFImage.h SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h \
 SFString.h SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h \
 SFVec4f.h CommandList.h Command.h DuneApp.h URL.h PreferencesApp.h \
 swt/include/swttypedef.h EcmaScriptApp.h StereoViewApp.h \
 swt/include/stereoview.h InputDeviceApp.h TransformMode.h OutputApp.h \
 RouteViewApp.h StartWithApp.h ExportNamesApp.h ModellingApp.h \
 RenderApp.h
NodeExternalGeometry.o: NodeExternalGeometry.cpp stdafx.h config.h \
 mysnprintf.h NodeExternalGeometry.h ProtoMacros.h Proto.h Array.h Map.h \
 List.h MyString.h StringArray.h Node.h Vec3f.h Vec3x.h FieldIndex.h \
 Element.h x3dFlags.h swt/include/WriteFlags.h NodeFlags.h MeshFlags.h \
 ac3dMaterialCallback.h NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h \
 NodeList.h InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h MFBool.h \
 FieldValue.h Arrays.h MySwap.h Types.h MFColor.h MFFloat.h SFFloat.h \
 MFColorRGBA.h MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h \
 MFNode.h MFRotation.h MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h \
 MFVec2f.h MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h \
 SFColorRGBA.h SFImage.h SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h \
 SFRotation.h SFString.h SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h \
 SFVec3f.h SFVec4f.h CommandList.h Command.h GeometryNode.h \
 NodeAppearance.h DuneApp.h URL.h PreferencesApp.h \
 swt/include/swttypedef.h EcmaScriptApp.h StereoViewApp.h \
 swt/include/stereoview.h InputDeviceApp.h TransformMode.h OutputApp.h \
 RouteViewApp.h StartWithApp.h ExportNamesApp.h ModellingApp.h \
 RenderApp.h NodeMaterial.h swt/include/swt.h swt/include/keycodes.h \
 swt/include/swlanguages.h swt/include/swDebugf.h \
 swt/include/swttypedef.h NodeImageTexture.h KambiTextureCommonFields.h \
 NodeTextureTransform.h GeometryNodeMacros.h Scene.h RenderState.h \
 ProtoMap.h
NodeField.o: NodeField.cpp stdafx.h config.h mysnprintf.h NodeField.h \
 ProtoMacros.h Proto.h Array.h Map.h List.h MyString.h StringArray.h \
 Node.h Vec3f.h Vec3x.h FieldIndex.h Element.h x3dFlags.h \
 swt/include/WriteFlags.h NodeFlags.h MeshFlags.h ac3dMaterialCallback.h \
 NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h NodeList.h \
 InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h \
 Arrays.h MySwap.h Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h \
 MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h \
 MFRotation.h MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h \
 MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h \
 SFImage.h SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h \
 SFString.h SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h \
 SFVec4f.h CommandList.h Command.h GeometryNode.h NodeAppearance.h \
 DuneApp.h URL.h PreferencesApp.h swt/include/swttypedef.h \
 EcmaScriptApp.h StereoViewApp.h swt/include/stereoview.h \
 InputDeviceApp.h TransformMode.h OutputApp.h RouteViewApp.h \
 StartWithApp.h ExportNamesApp.h ModellingApp.h RenderApp.h \
 NodeMaterial.h swt/include/swt.h swt/include/keycodes.h \
 swt/include/swlanguages.h swt/include/swDebugf.h \
 swt/include/swttypedef.h NodeImageTexture.h KambiTextureCommonFields.h \
 NodeTextureTransform.h GeometryNodeMacros.h Scene.h RenderState.h \
 ProtoMap.h
NodeImageGeometry.o: NodeImageGeometry.cpp NodeImageGeometry.h Node.h \
 Array.h stdafx.h config.h mysnprintf.h List.h Vec3f.h Vec3x.h MyString.h \
 StringArray.h FieldIndex.h Element.h x3dFlags.h swt/include/WriteFlags.h \
 NodeFlags.h MeshFlags.h ac3dMaterialCallback.h NodeArray.h Stack.h \
 Matrix.h Vec3d.h Quaternion.h ProtoMacros.h Proto.h Map.h NodeList.h \
 InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h \
 Arrays.h MySwap.h Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h \
 MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h \
 MFRotation.h MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h \
 MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h \
 SFImage.h SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h \
 SFString.h SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h \
 SFVec4f.h CommandList.h Command.h DuneApp.h URL.h PreferencesApp.h \
 swt/include/swttypedef.h EcmaScriptApp.h StereoViewApp.h \
 swt/include/stereoview.h InputDeviceApp.h TransformMode.h OutputApp.h \
 RouteViewApp.h StartWithApp.h ExportNamesApp.h ModellingApp.h \
 RenderApp.h GeometryNode.h NodeAppearance.h NodeMaterial.h \
 swt/include/swt.h swt/include/keycodes.h swt/include/swlanguages.h \
 swt/include/swDebugf.h swt/include/swttypedef.h NodeImageTexture.h \
 KambiTextureCommonFields.h NodeTextureTransform.h GeometryNodeMacros.h \
 Scene.h RenderState.h ProtoMap.h
NodeImageTextureAtlas.o: NodeImageTextureAtlas.cpp stdafx.h config.h \
 mysnprintf.h swt/include/swt.h swt/include/keycodes.h \
 swt/include/swlanguages.h swt/include/swDebugf.h \
 swt/include/swttypedef.h DuneApp.h List.h Array.h MyString.h \
 StringArray.h Node.h Vec3f.h Vec3x.h FieldIndex.h Element.h x3dFlags.h \
 swt/include/WriteFlags.h NodeFlags.h MeshFlags.h ac3dMaterialCallback.h \
 NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h URL.h PreferencesApp.h \
 swt/include/swttypedef.h EcmaScriptApp.h StereoViewApp.h \
 swt/include/stereoview.h InputDeviceApp.h TransformMode.h OutputApp.h \
 RouteViewApp.h StartWithApp.h ExportNamesApp.h ModellingApp.h \
 RenderApp.h resource.h NodeImageTextureAtlas.h ProtoMacros.h Proto.h \
 Map.h NodeList.h InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h \
 MFBool.h FieldValue.h Arrays.h MySwap.h Types.h MFColor.h MFFloat.h \
 SFFloat.h MFColorRGBA.h MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h \
 MFMatrix4f.h MFNode.h MFRotation.h MFString.h MFTime.h SFTime.h \
 MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h \
 SFBool.h SFColor.h SFColorRGBA.h SFImage.h SFInt32.h SFMatrix3f.h \
 SFMatrix4f.h SFNode.h SFRotation.h SFString.h SFVec2d.h SFVec2f.h \
 Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h CommandList.h Command.h \
 Scene.h RenderState.h ProtoMap.h
NodeNozzle.o: NodeNozzle.cpp stdafx.h config.h mysnprintf.h NodeNozzle.h \
 ProtoMacros.h Proto.h Array.h Map.h List.h MyString.h StringArray.h \
 Node.h Vec3f.h Vec3x.h FieldIndex.h Element.h x3dFlags.h \
 swt/include/WriteFlags.h NodeFlags.h MeshFlags.h ac3dMaterialCallback.h \
 NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h NodeList.h \
 InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h \
 Arrays.h MySwap.h Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h \
 MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h \
 MFRotation.h MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h \
 MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h \
 SFImage.h SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h \
 SFString.h SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h \
 SFVec4f.h CommandList.h Command.h GeometryNode.h NodeAppearance.h \
 DuneApp.h URL.h PreferencesApp.h swt/include/swttypedef.h \
 EcmaScriptApp.h StereoViewApp.h swt/include/stereoview.h \
 InputDeviceApp.h TransformMode.h OutputApp.h RouteViewApp.h \
 StartWithApp.h ExportNamesApp.h ModellingApp.h RenderApp.h \
 NodeMaterial.h swt/include/swt.h swt/include/keycodes.h \
 swt/include/swlanguages.h swt/include/swDebugf.h \
 swt/include/swttypedef.h NodeImageTexture.h KambiTextureCommonFields.h \
 NodeTextureTransform.h GeometryNodeMacros.h Scene.h RenderState.h \
 ProtoMap.h
NodeMesh.o: NodeMesh.cpp NodeMesh.h Node.h Array.h stdafx.h config.h \
 mysnprintf.h List.h Vec3f.h Vec3x.h MyString.h StringArray.h \
 FieldIndex.h Element.h x3dFlags.h swt/include/WriteFlags.h NodeFlags.h \
 MeshFlags.h ac3dMaterialCallback.h NodeArray.h Stack.h Matrix.h Vec3d.h \
 Quaternion.h ProtoMacros.h Proto.h Map.h NodeList.h InterfaceArray.h \
 DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h Arrays.h MySwap.h \
 Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h MFDouble.h \
 SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h MFRotation.h \
 MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h \
 MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h SFImage.h \
 SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h SFString.h \
 SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h \
 CommandList.h Command.h DuneApp.h URL.h PreferencesApp.h \
 swt/include/swttypedef.h EcmaScriptApp.h StereoViewApp.h \
 swt/include/stereoview.h InputDeviceApp.h TransformMode.h OutputApp.h \
 RouteViewApp.h StartWithApp.h ExportNamesApp.h ModellingApp.h \
 RenderApp.h GeometryNode.h NodeAppearance.h NodeMaterial.h \
 swt/include/swt.h swt/include/keycodes.h swt/include/swlanguages.h \
 swt/include/swDebugf.h swt/include/swttypedef.h NodeImageTexture.h \
 KambiTextureCommonFields.h NodeTextureTransform.h GeometryNodeMacros.h \
 Scene.h RenderState.h ProtoMap.h
NodeMPRVolumeStyle.o: NodeMPRVolumeStyle.cpp stdafx.h config.h \
 mysnprintf.h NodeMPRVolumeStyle.h Node.h Array.h List.h Vec3f.h Vec3x.h \
 MyString.h StringArray.h FieldIndex.h Element.h x3dFlags.h \
 swt/include/WriteFlags.h NodeFlags.h MeshFlags.h ac3dMaterialCallback.h \
 NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h ProtoMacros.h Proto.h \
 Map.h NodeList.h InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h \
 MFBool.h FieldValue.h Arrays.h MySwap.h Types.h MFColor.h MFFloat.h \
 SFFloat.h MFColorRGBA.h MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h \
 MFMatrix4f.h MFNode.h MFRotation.h MFString.h MFTime.h SFTime.h \
 MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h \
 SFBool.h SFColor.h SFColorRGBA.h SFImage.h SFInt32.h SFMatrix3f.h \
 SFMatrix4f.h SFNode.h SFRotation.h SFString.h SFVec2d.h SFVec2f.h \
 Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h CommandList.h Command.h \
 DuneApp.h URL.h PreferencesApp.h swt/include/swttypedef.h \
 EcmaScriptApp.h StereoViewApp.h swt/include/stereoview.h \
 InputDeviceApp.h TransformMode.h OutputApp.h RouteViewApp.h \
 StartWithApp.h ExportNamesApp.h ModellingApp.h RenderApp.h
NodeMultiPart.o: NodeMultiPart.cpp stdafx.h config.h mysnprintf.h \
 NodeMultiPart.h Node.h Array.h List.h Vec3f.h Vec3x.h MyString.h \
 StringArray.h FieldIndex.h Element.h x3dFlags.h swt/include/WriteFlags.h \
 NodeFlags.h MeshFlags.h ac3dMaterialCallback.h NodeArray.h Stack.h \
 Matrix.h Vec3d.h Quaternion.h ProtoMacros.h Proto.h Map.h NodeList.h \
 InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h \
 Arrays.h MySwap.h Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h \
 MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h \
 MFRotation.h MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h \
 MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h \
 SFImage.h SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h \
 SFString.h SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h \
 SFVec4f.h CommandList.h Command.h NodeGroup.h StaticGroupNode.h \
 GroupNode.h DuneApp.h URL.h PreferencesApp.h swt/include/swttypedef.h \
 EcmaScriptApp.h StereoViewApp.h swt/include/stereoview.h \
 InputDeviceApp.h TransformMode.h OutputApp.h RouteViewApp.h \
 StartWithApp.h ExportNamesApp.h ModellingApp.h RenderApp.h Scene.h \
 RenderState.h ProtoMap.h
NodeParam.o: NodeParam.cpp stdafx.h config.h mysnprintf.h NodeParam.h \
 ProtoMacros.h Proto.h Array.h Map.h List.h MyString.h StringArray.h \
 Node.h Vec3f.h Vec3x.h FieldIndex.h Element.h x3dFlags.h \
 swt/include/WriteFlags.h NodeFlags.h MeshFlags.h ac3dMaterialCallback.h \
 NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h NodeList.h \
 InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h \
 Arrays.h MySwap.h Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h \
 MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h \
 MFRotation.h MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h \
 MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h \
 SFImage.h SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h \
 SFString.h SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h \
 SFVec4f.h CommandList.h Command.h GeometryNode.h NodeAppearance.h \
 DuneApp.h URL.h PreferencesApp.h swt/include/swttypedef.h \
 EcmaScriptApp.h StereoViewApp.h swt/include/stereoview.h \
 InputDeviceApp.h TransformMode.h OutputApp.h RouteViewApp.h \
 StartWithApp.h ExportNamesApp.h ModellingApp.h RenderApp.h \
 NodeMaterial.h swt/include/swt.h swt/include/keycodes.h \
 swt/include/swlanguages.h swt/include/swDebugf.h \
 swt/include/swttypedef.h NodeImageTexture.h KambiTextureCommonFields.h \
 NodeTextureTransform.h GeometryNodeMacros.h Scene.h RenderState.h \
 ProtoMap.h
NodeParticleSet.o: NodeParticleSet.cpp NodeParticleSet.h Node.h Array.h \
 stdafx.h config.h mysnprintf.h List.h Vec3f.h Vec3x.h MyString.h \
 StringArray.h FieldIndex.h Element.h x3dFlags.h swt/include/WriteFlags.h \
 NodeFlags.h MeshFlags.h ac3dMaterialCallback.h NodeArray.h Stack.h \
 Matrix.h Vec3d.h Quaternion.h ProtoMacros.h Proto.h Map.h NodeList.h \
 InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h \
 Arrays.h MySwap.h Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h \
 MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h \
 MFRotation.h MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h \
 MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h \
 SFImage.h SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h \
 SFString.h SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h \
 SFVec4f.h CommandList.h Command.h DuneApp.h URL.h PreferencesApp.h \
 swt/include/swttypedef.h EcmaScriptApp.h StereoViewApp.h \
 swt/include/stereoview.h InputDeviceApp.h TransformMode.h OutputApp.h \
 RouteViewApp.h StartWithApp.h ExportNamesApp.h ModellingApp.h \
 RenderApp.h GeometryNode.h NodeAppearance.h NodeMaterial.h \
 swt/include/swt.h swt/include/keycodes.h swt/include/swlanguages.h \
 swt/include/swDebugf.h swt/include/swttypedef.h NodeImageTexture.h \
 KambiTextureCommonFields.h NodeTextureTransform.h GeometryNodeMacros.h \
 Scene.h RenderState.h ProtoMap.h
NodePlane.o: NodePlane.cpp NodePlane.h Node.h Array.h stdafx.h config.h \
 mysnprintf.h List.h Vec3f.h Vec3x.h MyString.h StringArray.h \
 FieldIndex.h Element.h x3dFlags.h swt/include/WriteFlags.h NodeFlags.h \
 MeshFlags.h ac3dMaterialCallback.h NodeArray.h Stack.h Matrix.h Vec3d.h \
 Quaternion.h ProtoMacros.h Proto.h Map.h NodeList.h InterfaceArray.h \
 DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h Arrays.h MySwap.h \
 Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h MFDouble.h \
 SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h MFRotation.h \
 MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h \
 MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h SFImage.h \
 SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h SFString.h \
 SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h \
 CommandList.h Command.h DuneApp.h URL.h PreferencesApp.h \
 swt/include/swttypedef.h EcmaScriptApp.h StereoViewApp.h \
 swt/include/stereoview.h InputDeviceApp.h TransformMode.h OutputApp.h \
 RouteViewApp.h StartWithApp.h ExportNamesApp.h ModellingApp.h \
 RenderApp.h GeometryNode.h NodeAppearance.h NodeMaterial.h \
 swt/include/swt.h swt/include/keycodes.h swt/include/swlanguages.h \
 swt/include/swDebugf.h swt/include/swttypedef.h NodeImageTexture.h \
 KambiTextureCommonFields.h NodeTextureTransform.h GeometryNodeMacros.h \
 Scene.h RenderState.h ProtoMap.h
NodePopGeometry.o: NodePopGeometry.cpp NodePopGeometry.h Node.h Array.h \
 stdafx.h config.h mysnprintf.h List.h Vec3f.h Vec3x.h MyString.h \
 StringArray.h FieldIndex.h Element.h x3dFlags.h swt/include/WriteFlags.h \
 NodeFlags.h MeshFlags.h ac3dMaterialCallback.h NodeArray.h Stack.h \
 Matrix.h Vec3d.h Quaternion.h ProtoMacros.h Proto.h Map.h NodeList.h \
 InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h \
 Arrays.h MySwap.h Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h \
 MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h \
 MFRotation.h MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h \
 MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h \
 SFImage.h SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h \
 SFString.h SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h \
 SFVec4f.h CommandList.h Command.h DuneApp.h URL.h PreferencesApp.h \
 swt/include/swttypedef.h EcmaScriptApp.h StereoViewApp.h \
 swt/include/stereoview.h InputDeviceApp.h TransformMode.h OutputApp.h \
 RouteViewApp.h StartWithApp.h ExportNamesApp.h ModellingApp.h \
 RenderApp.h GeometryNode.h NodeAppearance.h NodeMaterial.h \
 swt/include/swt.h swt/include/keycodes.h swt/include/swlanguages.h \
 swt/include/swDebugf.h swt/include/swttypedef.h NodeImageTexture.h \
 KambiTextureCommonFields.h NodeTextureTransform.h GeometryNodeMacros.h \
 Scene.h RenderState.h ProtoMap.h
NodePopGeometryLevel.o: NodePopGeometryLevel.cpp stdafx.h config.h \
 mysnprintf.h NodePopGeometryLevel.h ProtoMacros.h Proto.h Array.h Map.h \
 List.h MyString.h StringArray.h Node.h Vec3f.h Vec3x.h FieldIndex.h \
 Element.h x3dFlags.h swt/include/WriteFlags.h NodeFlags.h MeshFlags.h \
 ac3dMaterialCallback.h NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h \
 NodeList.h InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h MFBool.h \
 FieldValue.h Arrays.h MySwap.h Types.h MFColor.h MFFloat.h SFFloat.h \
 MFColorRGBA.h MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h \
 MFNode.h MFRotation.h MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h \
 MFVec2f.h MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h \
 SFColorRGBA.h SFImage.h SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h \
 SFRotation.h SFString.h SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h \
 SFVec3f.h SFVec4f.h CommandList.h Command.h Scene.h URL.h DuneApp.h \
 PreferencesApp.h swt/include/swttypedef.h EcmaScriptApp.h \
 StereoViewApp.h swt/include/stereoview.h InputDeviceApp.h \
 TransformMode.h OutputApp.h RouteViewApp.h StartWithApp.h \
 ExportNamesApp.h ModellingApp.h RenderApp.h RenderState.h ProtoMap.h
NodePyramid.o: NodePyramid.cpp stdafx.h config.h mysnprintf.h \
 NodePyramid.h ProtoMacros.h Proto.h Array.h Map.h List.h MyString.h \
 StringArray.h Node.h Vec3f.h Vec3x.h FieldIndex.h Element.h x3dFlags.h \
 swt/include/WriteFlags.h NodeFlags.h MeshFlags.h ac3dMaterialCallback.h \
 NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h NodeList.h \
 InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h \
 Arrays.h MySwap.h Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h \
 MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h \
 MFRotation.h MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h \
 MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h \
 SFImage.h SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h \
 SFString.h SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h \
 SFVec4f.h CommandList.h Command.h GeometryNode.h NodeAppearance.h \
 DuneApp.h URL.h PreferencesApp.h swt/include/swttypedef.h \
 EcmaScriptApp.h StereoViewApp.h swt/include/stereoview.h \
 InputDeviceApp.h TransformMode.h OutputApp.h RouteViewApp.h \
 StartWithApp.h ExportNamesApp.h ModellingApp.h RenderApp.h \
 NodeMaterial.h swt/include/swt.h swt/include/keycodes.h \
 swt/include/swlanguages.h swt/include/swDebugf.h \
 swt/include/swttypedef.h NodeImageTexture.h KambiTextureCommonFields.h \
 NodeTextureTransform.h GeometryNodeMacros.h
NodeRadarVolumeStyle.o: NodeRadarVolumeStyle.cpp stdafx.h config.h \
 mysnprintf.h NodeRadarVolumeStyle.h Node.h Array.h List.h Vec3f.h \
 Vec3x.h MyString.h StringArray.h FieldIndex.h Element.h x3dFlags.h \
 swt/include/WriteFlags.h NodeFlags.h MeshFlags.h ac3dMaterialCallback.h \
 NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h ProtoMacros.h Proto.h \
 Map.h NodeList.h InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h \
 MFBool.h FieldValue.h Arrays.h MySwap.h Types.h MFColor.h MFFloat.h \
 SFFloat.h MFColorRGBA.h MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h \
 MFMatrix4f.h MFNode.h MFRotation.h MFString.h MFTime.h SFTime.h \
 MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h \
 SFBool.h SFColor.h SFColorRGBA.h SFImage.h SFInt32.h SFMatrix3f.h \
 SFMatrix4f.h SFNode.h SFRotation.h SFString.h SFVec2d.h SFVec2f.h \
 Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h CommandList.h Command.h \
 DuneApp.h URL.h PreferencesApp.h swt/include/swttypedef.h \
 EcmaScriptApp.h StereoViewApp.h swt/include/stereoview.h \
 InputDeviceApp.h TransformMode.h OutputApp.h RouteViewApp.h \
 StartWithApp.h ExportNamesApp.h ModellingApp.h RenderApp.h
NodeRectangularTorus.o: NodeRectangularTorus.cpp stdafx.h config.h \
 mysnprintf.h NodeRectangularTorus.h ProtoMacros.h Proto.h Array.h Map.h \
 List.h MyString.h StringArray.h Node.h Vec3f.h Vec3x.h FieldIndex.h \
 Element.h x3dFlags.h swt/include/WriteFlags.h NodeFlags.h MeshFlags.h \
 ac3dMaterialCallback.h NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h \
 NodeList.h InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h MFBool.h \
 FieldValue.h Arrays.h MySwap.h Types.h MFColor.h MFFloat.h SFFloat.h \
 MFColorRGBA.h MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h \
 MFNode.h MFRotation.h MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h \
 MFVec2f.h MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h \
 SFColorRGBA.h SFImage.h SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h \
 SFRotation.h SFString.h SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h \
 SFVec3f.h SFVec4f.h CommandList.h Command.h GeometryNode.h \
 NodeAppearance.h DuneApp.h URL.h PreferencesApp.h \
 swt/include/swttypedef.h EcmaScriptApp.h StereoViewApp.h \
 swt/include/stereoview.h InputDeviceApp.h TransformMode.h OutputApp.h \
 RouteViewApp.h StartWithApp.h ExportNamesApp.h ModellingApp.h \
 RenderApp.h NodeMaterial.h swt/include/swt.h swt/include/keycodes.h \
 swt/include/swlanguages.h swt/include/swDebugf.h \
 swt/include/swttypedef.h NodeImageTexture.h KambiTextureCommonFields.h \
 NodeTextureTransform.h GeometryNodeMacros.h
NodeRefinementTexture.o: NodeRefinementTexture.cpp stdafx.h config.h \
 mysnprintf.h swt/include/swt.h swt/include/keycodes.h \
 swt/include/swlanguages.h swt/include/swDebugf.h \
 swt/include/swttypedef.h DuneApp.h List.h Array.h MyString.h \
 StringArray.h Node.h Vec3f.h Vec3x.h FieldIndex.h Element.h x3dFlags.h \
 swt/include/WriteFlags.h NodeFlags.h MeshFlags.h ac3dMaterialCallback.h \
 NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h URL.h PreferencesApp.h \
 swt/include/swttypedef.h EcmaScriptApp.h StereoViewApp.h \
 swt/include/stereoview.h InputDeviceApp.h TransformMode.h OutputApp.h \
 RouteViewApp.h StartWithApp.h ExportNamesApp.h ModellingApp.h \
 RenderApp.h resource.h NodeRefinementTexture.h ProtoMacros.h Proto.h \
 Map.h NodeList.h InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h \
 MFBool.h FieldValue.h Arrays.h MySwap.h Types.h MFColor.h MFFloat.h \
 SFFloat.h MFColorRGBA.h MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h \
 MFMatrix4f.h MFNode.h MFRotation.h MFString.h MFTime.h SFTime.h \
 MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h \
 SFBool.h SFColor.h SFColorRGBA.h SFImage.h SFInt32.h SFMatrix3f.h \
 SFMatrix4f.h SFNode.h SFRotation.h SFString.h SFVec2d.h SFVec2f.h \
 Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h CommandList.h Command.h \
 Scene.h RenderState.h ProtoMap.h
NodeRemoteSelectionGroup.o: NodeRemoteSelectionGroup.cpp stdafx.h \
 config.h mysnprintf.h NodeRemoteSelectionGroup.h Node.h Array.h List.h \
 Vec3f.h Vec3x.h MyString.h StringArray.h FieldIndex.h Element.h \
 x3dFlags.h swt/include/WriteFlags.h NodeFlags.h MeshFlags.h \
 ac3dMaterialCallback.h NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h \
 ProtoMacros.h Proto.h Map.h NodeList.h InterfaceArray.h \
 DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h Arrays.h MySwap.h \
 Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h MFDouble.h \
 SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h MFRotation.h \
 MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h \
 MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h SFImage.h \
 SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h SFString.h \
 SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h \
 CommandList.h Command.h NodeGroup.h StaticGroupNode.h GroupNode.h \
 DuneApp.h URL.h PreferencesApp.h swt/include/swttypedef.h \
 EcmaScriptApp.h StereoViewApp.h swt/include/stereoview.h \
 InputDeviceApp.h TransformMode.h OutputApp.h RouteViewApp.h \
 StartWithApp.h ExportNamesApp.h ModellingApp.h RenderApp.h
NodeSlopedCylinder.o: NodeSlopedCylinder.cpp stdafx.h config.h \
 mysnprintf.h NodeSlopedCylinder.h MeshBasedNode.h GeometryNode.h Node.h \
 Array.h List.h Vec3f.h Vec3x.h MyString.h StringArray.h FieldIndex.h \
 Element.h x3dFlags.h swt/include/WriteFlags.h NodeFlags.h MeshFlags.h \
 ac3dMaterialCallback.h NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h \
 NodeAppearance.h ProtoMacros.h Proto.h Map.h NodeList.h InterfaceArray.h \
 DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h Arrays.h MySwap.h \
 Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h MFDouble.h \
 SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h MFRotation.h \
 MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h \
 MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h SFImage.h \
 SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h SFString.h \
 SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h \
 CommandList.h Command.h DuneApp.h URL.h PreferencesApp.h \
 swt/include/swttypedef.h EcmaScriptApp.h StereoViewApp.h \
 swt/include/stereoview.h InputDeviceApp.h TransformMode.h OutputApp.h \
 RouteViewApp.h StartWithApp.h ExportNamesApp.h ModellingApp.h \
 RenderApp.h NodeMaterial.h swt/include/swt.h swt/include/keycodes.h \
 swt/include/swlanguages.h swt/include/swDebugf.h \
 swt/include/swttypedef.h NodeImageTexture.h KambiTextureCommonFields.h \
 NodeTextureTransform.h GeometryNodeMacros.h
NodeSnout.o: NodeSnout.cpp stdafx.h config.h mysnprintf.h NodeSnout.h \
 ProtoMacros.h Proto.h Array.h Map.h List.h MyString.h StringArray.h \
 Node.h Vec3f.h Vec3x.h FieldIndex.h Element.h x3dFlags.h \
 swt/include/WriteFlags.h NodeFlags.h MeshFlags.h ac3dMaterialCallback.h \
 NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h NodeList.h \
 InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h \
 Arrays.h MySwap.h Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h \
 MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h \
 MFRotation.h MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h \
 MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h \
 SFImage.h SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h \
 SFString.h SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h \
 SFVec4f.h CommandList.h Command.h GeometryNode.h NodeAppearance.h \
 DuneApp.h URL.h PreferencesApp.h swt/include/swttypedef.h \
 EcmaScriptApp.h StereoViewApp.h swt/include/stereoview.h \
 InputDeviceApp.h TransformMode.h OutputApp.h RouteViewApp.h \
 StartWithApp.h ExportNamesApp.h ModellingApp.h RenderApp.h \
 NodeMaterial.h swt/include/swt.h swt/include/keycodes.h \
 swt/include/swlanguages.h swt/include/swDebugf.h \
 swt/include/swttypedef.h NodeImageTexture.h KambiTextureCommonFields.h \
 NodeTextureTransform.h GeometryNodeMacros.h
NodeSolidOfRevolution.o: NodeSolidOfRevolution.cpp \
 NodeSolidOfRevolution.h Node.h Array.h stdafx.h config.h mysnprintf.h \
 List.h Vec3f.h Vec3x.h MyString.h StringArray.h FieldIndex.h Element.h \
 x3dFlags.h swt/include/WriteFlags.h NodeFlags.h MeshFlags.h \
 ac3dMaterialCallback.h NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h \
 ProtoMacros.h Proto.h Map.h NodeList.h InterfaceArray.h \
 DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h Arrays.h MySwap.h \
 Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h MFDouble.h \
 SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h MFRotation.h \
 MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h \
 MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h SFImage.h \
 SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h SFString.h \
 SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h \
 CommandList.h Command.h GeometryNode.h NodeAppearance.h DuneApp.h URL.h \
 PreferencesApp.h swt/include/swttypedef.h EcmaScriptApp.h \
 StereoViewApp.h swt/include/stereoview.h InputDeviceApp.h \
 TransformMode.h OutputApp.h RouteViewApp.h StartWithApp.h \
 ExportNamesApp.h ModellingApp.h RenderApp.h NodeMaterial.h \
 swt/include/swt.h swt/include/keycodes.h swt/include/swlanguages.h \
 swt/include/swDebugf.h swt/include/swttypedef.h NodeImageTexture.h \
 KambiTextureCommonFields.h NodeTextureTransform.h GeometryNodeMacros.h
NodeSphereSegment.o: NodeSphereSegment.cpp stdafx.h config.h mysnprintf.h \
 NodeSphereSegment.h ProtoMacros.h Proto.h Array.h Map.h List.h \
 MyString.h StringArray.h Node.h Vec3f.h Vec3x.h FieldIndex.h Element.h \
 x3dFlags.h swt/include/WriteFlags.h NodeFlags.h MeshFlags.h \
 ac3dMaterialCallback.h NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h \
 NodeList.h InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h MFBool.h \
 FieldValue.h Arrays.h MySwap.h Types.h MFColor.h MFFloat.h SFFloat.h \
 MFColorRGBA.h MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h \
 MFNode.h MFRotation.h MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h \
 MFVec2f.h MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h \
 SFColorRGBA.h SFImage.h SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h \
 SFRotation.h SFString.h SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h \
 SFVec3f.h SFVec4f.h CommandList.h Command.h GeometryNode.h \
 NodeAppearance.h DuneApp.h URL.h PreferencesApp.h \
 swt/include/swttypedef.h EcmaScriptApp.h StereoViewApp.h \
 swt/include/stereoview.h InputDeviceApp.h TransformMode.h OutputApp.h \
 RouteViewApp.h StartWithApp.h ExportNamesApp.h ModellingApp.h \
 RenderApp.h NodeMaterial.h swt/include/swt.h swt/include/keycodes.h \
 swt/include/swlanguages.h swt/include/swDebugf.h \
 swt/include/swttypedef.h NodeImageTexture.h KambiTextureCommonFields.h \
 NodeTextureTransform.h GeometryNodeMacros.h
NodeStippleVolumeStyle.o: NodeStippleVolumeStyle.cpp stdafx.h config.h \
 mysnprintf.h NodeStippleVolumeStyle.h Node.h Array.h List.h Vec3f.h \
 Vec3x.h MyString.h StringArray.h FieldIndex.h Element.h x3dFlags.h \
 swt/include/WriteFlags.h NodeFlags.h MeshFlags.h ac3dMaterialCallback.h \
 NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h ProtoMacros.h Proto.h \
 Map.h NodeList.h InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h \
 MFBool.h FieldValue.h Arrays.h MySwap.h Types.h MFColor.h MFFloat.h \
 SFFloat.h MFColorRGBA.h MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h \
 MFMatrix4f.h MFNode.h MFRotation.h MFString.h MFTime.h SFTime.h \
 MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h \
 SFBool.h SFColor.h SFColorRGBA.h SFImage.h SFInt32.h SFMatrix3f.h \
 SFMatrix4f.h SFNode.h SFRotation.h SFString.h SFVec2d.h SFVec2f.h \
 Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h CommandList.h Command.h \
 DuneApp.h URL.h PreferencesApp.h swt/include/swttypedef.h \
 EcmaScriptApp.h StereoViewApp.h swt/include/stereoview.h \
 InputDeviceApp.h TransformMode.h OutputApp.h RouteViewApp.h \
 StartWithApp.h ExportNamesApp.h ModellingApp.h RenderApp.h
NodeSurfaceShaderTexture.o: NodeSurfaceShaderTexture.cpp stdafx.h \
 config.h mysnprintf.h Scene.h Array.h Map.h List.h Stack.h CommandList.h \
 Command.h NodeList.h NodeArray.h Node.h Vec3f.h Vec3x.h MyString.h \
 StringArray.h FieldIndex.h Element.h x3dFlags.h swt/include/WriteFlags.h \
 NodeFlags.h MeshFlags.h ac3dMaterialCallback.h Matrix.h Vec3d.h \
 Quaternion.h URL.h Proto.h ProtoMacros.h InterfaceArray.h \
 DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h Arrays.h MySwap.h \
 Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h MFDouble.h \
 SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h MFRotation.h \
 MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h \
 MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h SFImage.h \
 SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h SFString.h \
 SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h \
 DuneApp.h PreferencesApp.h swt/include/swttypedef.h EcmaScriptApp.h \
 StereoViewApp.h swt/include/stereoview.h InputDeviceApp.h \
 TransformMode.h OutputApp.h RouteViewApp.h StartWithApp.h \
 ExportNamesApp.h ModellingApp.h RenderApp.h RenderState.h ProtoMap.h \
 resource.h NodeSurfaceShaderTexture.h
NodeTorus.o: NodeTorus.cpp stdafx.h config.h mysnprintf.h NodeTorus.h \
 ProtoMacros.h Proto.h Array.h Map.h List.h MyString.h StringArray.h \
 Node.h Vec3f.h Vec3x.h FieldIndex.h Element.h x3dFlags.h \
 swt/include/WriteFlags.h NodeFlags.h MeshFlags.h ac3dMaterialCallback.h \
 NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h NodeList.h \
 InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h \
 Arrays.h MySwap.h Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h \
 MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h \
 MFRotation.h MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h \
 MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h \
 SFImage.h SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h \
 SFString.h SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h \
 SFVec4f.h CommandList.h Command.h GeometryNode.h NodeAppearance.h \
 DuneApp.h URL.h PreferencesApp.h swt/include/swttypedef.h \
 EcmaScriptApp.h StereoViewApp.h swt/include/stereoview.h \
 InputDeviceApp.h TransformMode.h OutputApp.h RouteViewApp.h \
 StartWithApp.h ExportNamesApp.h ModellingApp.h RenderApp.h \
 NodeMaterial.h swt/include/swt.h swt/include/keycodes.h \
 swt/include/swlanguages.h swt/include/swDebugf.h \
 swt/include/swttypedef.h NodeImageTexture.h KambiTextureCommonFields.h \
 NodeTextureTransform.h GeometryNodeMacros.h
NodeUniform.o: NodeUniform.cpp stdafx.h config.h mysnprintf.h \
 NodeUniform.h ProtoMacros.h Proto.h Array.h Map.h List.h MyString.h \
 StringArray.h Node.h Vec3f.h Vec3x.h FieldIndex.h Element.h x3dFlags.h \
 swt/include/WriteFlags.h NodeFlags.h MeshFlags.h ac3dMaterialCallback.h \
 NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h NodeList.h \
 InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h \
 Arrays.h MySwap.h Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h \
 MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h \
 MFRotation.h MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h \
 MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h \
 SFImage.h SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h \
 SFString.h SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h \
 SFVec4f.h CommandList.h Command.h Scene.h URL.h DuneApp.h \
 PreferencesApp.h swt/include/swttypedef.h EcmaScriptApp.h \
 StereoViewApp.h swt/include/stereoview.h InputDeviceApp.h \
 TransformMode.h OutputApp.h RouteViewApp.h StartWithApp.h \
 ExportNamesApp.h ModellingApp.h RenderApp.h RenderState.h ProtoMap.h
NodeViewfrustum.o: NodeViewfrustum.cpp stdafx.h config.h mysnprintf.h \
 NodeViewfrustum.h Node.h Array.h List.h Vec3f.h Vec3x.h MyString.h \
 StringArray.h FieldIndex.h Element.h x3dFlags.h swt/include/WriteFlags.h \
 NodeFlags.h MeshFlags.h ac3dMaterialCallback.h NodeArray.h Stack.h \
 Matrix.h Vec3d.h Quaternion.h ProtoMacros.h Proto.h Map.h NodeList.h \
 InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h \
 Arrays.h MySwap.h Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h \
 MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h \
 MFRotation.h MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h \
 MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h \
 SFImage.h SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h \
 SFString.h SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h \
 SFVec4f.h CommandList.h Command.h KambiViewpointCommonFields.h
NodeCattExportRec.o: NodeCattExportRec.cpp stdafx.h config.h mysnprintf.h \
 NodeCattExportRec.h Node.h Array.h List.h Vec3f.h Vec3x.h MyString.h \
 StringArray.h FieldIndex.h Element.h x3dFlags.h swt/include/WriteFlags.h \
 NodeFlags.h MeshFlags.h ac3dMaterialCallback.h NodeArray.h Stack.h \
 Matrix.h Vec3d.h Quaternion.h ProtoMacros.h Proto.h Map.h NodeList.h \
 InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h \
 Arrays.h MySwap.h Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h \
 MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h \
 MFRotation.h MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h \
 MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h \
 SFImage.h SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h \
 SFString.h SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h \
 SFVec4f.h CommandList.h Command.h Scene.h URL.h DuneApp.h \
 PreferencesApp.h swt/include/swttypedef.h EcmaScriptApp.h \
 StereoViewApp.h swt/include/stereoview.h InputDeviceApp.h \
 TransformMode.h OutputApp.h RouteViewApp.h StartWithApp.h \
 ExportNamesApp.h ModellingApp.h RenderApp.h RenderState.h ProtoMap.h \
 Util.h swt/include/swt.h swt/include/keycodes.h \
 swt/include/swlanguages.h swt/include/swDebugf.h \
 swt/include/swttypedef.h NodeNavigationInfo.h
NodeCattExportSrc.o: NodeCattExportSrc.cpp stdafx.h config.h mysnprintf.h \
 swt/include/swt.h swt/include/keycodes.h swt/include/swlanguages.h \
 swt/include/swDebugf.h swt/include/swttypedef.h NodeCattExportSrc.h \
 Node.h Array.h List.h Vec3f.h Vec3x.h MyString.h StringArray.h \
 FieldIndex.h Element.h x3dFlags.h swt/include/WriteFlags.h NodeFlags.h \
 MeshFlags.h ac3dMaterialCallback.h NodeArray.h Stack.h Matrix.h Vec3d.h \
 Quaternion.h ProtoMacros.h Proto.h Map.h NodeList.h InterfaceArray.h \
 DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h Arrays.h MySwap.h \
 Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h MFDouble.h \
 SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h MFRotation.h \
 MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h \
 MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h SFImage.h \
 SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h SFString.h \
 SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h \
 CommandList.h Command.h Scene.h URL.h DuneApp.h PreferencesApp.h \
 swt/include/swttypedef.h EcmaScriptApp.h StereoViewApp.h \
 swt/include/stereoview.h InputDeviceApp.h TransformMode.h OutputApp.h \
 RouteViewApp.h StartWithApp.h ExportNamesApp.h ModellingApp.h \
 RenderApp.h RenderState.h ProtoMap.h Util.h NodeNavigationInfo.h
NodeLdrawDatExport.o: NodeLdrawDatExport.cpp stdafx.h config.h \
 mysnprintf.h NodeLdrawDatExport.h Node.h Array.h List.h Vec3f.h Vec3x.h \
 MyString.h StringArray.h FieldIndex.h Element.h x3dFlags.h \
 swt/include/WriteFlags.h NodeFlags.h MeshFlags.h ac3dMaterialCallback.h \
 NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h ProtoMacros.h Proto.h \
 Map.h NodeList.h InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h \
 MFBool.h FieldValue.h Arrays.h MySwap.h Types.h MFColor.h MFFloat.h \
 SFFloat.h MFColorRGBA.h MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h \
 MFMatrix4f.h MFNode.h MFRotation.h MFString.h MFTime.h SFTime.h \
 MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h \
 SFBool.h SFColor.h SFColorRGBA.h SFImage.h SFInt32.h SFMatrix3f.h \
 SFMatrix4f.h SFNode.h SFRotation.h SFString.h SFVec2d.h SFVec2f.h \
 Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h CommandList.h Command.h \
 Scene.h URL.h DuneApp.h PreferencesApp.h swt/include/swttypedef.h \
 EcmaScriptApp.h StereoViewApp.h swt/include/stereoview.h \
 InputDeviceApp.h TransformMode.h OutputApp.h RouteViewApp.h \
 StartWithApp.h ExportNamesApp.h ModellingApp.h RenderApp.h RenderState.h \
 ProtoMap.h Field.h Util.h swt/include/swt.h swt/include/keycodes.h \
 swt/include/swlanguages.h swt/include/swDebugf.h \
 swt/include/swttypedef.h NodeNavigationInfo.h resource.h
NodeWonderlandImportJava.o: NodeWonderlandImportJava.cpp \
 NodeWonderlandImportJava.h Node.h Array.h stdafx.h config.h mysnprintf.h \
 List.h Vec3f.h Vec3x.h MyString.h StringArray.h FieldIndex.h Element.h \
 x3dFlags.h swt/include/WriteFlags.h NodeFlags.h MeshFlags.h \
 ac3dMaterialCallback.h NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h \
 ProtoMacros.h Proto.h Map.h NodeList.h InterfaceArray.h \
 DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h Arrays.h MySwap.h \
 Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h MFDouble.h \
 SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h MFRotation.h \
 MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h \
 MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h SFImage.h \
 SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h SFString.h \
 SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h \
 CommandList.h Command.h
resource.o: resource.c swt/include/res.h swt/include/swt.h \
 swt/include/keycodes.h swt/include/swlanguages.h swt/include/swDebugf.h \
 swt/include/swttypedef.h
mysnprintf.o: mysnprintf.c config.h
buf.o: buf.c buf.h
mathx.o: mathx.c mathx.h
subd_mesh.o: subd_mesh.c buf.h mathx.h subd_mesh.h
subd.o: subd.c buf.h mathx.h subd_mesh.h
openglutfont.o: openglutfont.c openglutfont.h
jpegLoad.o: jpegLoad.c config.h jpegLoad.h
pngLoad.o: pngLoad.c config.h pngLoad.h /usr/include/libpng16/png.h \
 /usr/include/libpng16/pnglibconf.h /usr/include/libpng16/pngconf.h
gif.o: gif.c kljcpyrght.h gif.h
resource.o: resource.c swt/include/res.h swt/include/swt.h \
 swt/include/keycodes.h swt/include/swlanguages.h swt/include/swDebugf.h \
 swt/include/swttypedef.h
mysnprintf.o: mysnprintf.c config.h
resource.o: resource.c swt/include/res.h swt/include/swt.h \
 swt/include/keycodes.h swt/include/swlanguages.h swt/include/swDebugf.h \
 swt/include/swttypedef.h
parser.o: parser.cpp config.h stdafx.h mysnprintf.h parser.h Scene.h \
 Array.h Map.h List.h Stack.h CommandList.h Command.h NodeList.h \
 NodeArray.h Node.h Vec3f.h Vec3x.h MyString.h StringArray.h FieldIndex.h \
 Element.h x3dFlags.h swt/include/WriteFlags.h NodeFlags.h MeshFlags.h \
 ac3dMaterialCallback.h Matrix.h Vec3d.h Quaternion.h URL.h Proto.h \
 ProtoMacros.h InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h MFBool.h \
 FieldValue.h Arrays.h MySwap.h Types.h MFColor.h MFFloat.h SFFloat.h \
 MFColorRGBA.h MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h \
 MFNode.h MFRotation.h MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h \
 MFVec2f.h MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h \
 SFColorRGBA.h SFImage.h SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h \
 SFRotation.h SFString.h SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h \
 SFVec3f.h SFVec4f.h DuneApp.h PreferencesApp.h swt/include/swttypedef.h \
 EcmaScriptApp.h StereoViewApp.h swt/include/stereoview.h \
 InputDeviceApp.h TransformMode.h OutputApp.h RouteViewApp.h \
 StartWithApp.h ExportNamesApp.h ModellingApp.h RenderApp.h RenderState.h \
 ProtoMap.h EventIn.h EventOut.h ExposedField.h Field.h resource.h \
 NodeScript.h NodeVrmlCut.h NodeNurbsSurface.h MeshMorphingNode.h \
 Interpolator.h MeshBasedNode.h GeometryNode.h NodeAppearance.h \
 NodeMaterial.h swt/include/swt.h swt/include/keycodes.h \
 swt/include/swlanguages.h swt/include/swDebugf.h \
 swt/include/swttypedef.h NodeImageTexture.h KambiTextureCommonFields.h \
 NodeTextureTransform.h GeometryNodeMacros.h NodeNurbsTrimmedSurface.h \
 NodeNurbsCurve.h NurbsCurve.h ChainBasedNode.h \
 NodeNurbsPositionInterpolator.h NodeGeoCoordinate.h GeoNode.h \
 NodeGeoElevationGrid.h NodeGeoOrigin.h NodeGeoLocation.h NodeGeoLOD.h \
 NodeGeoPositionInterpolator.h NodeGeoViewpoint.h ViewpointNode.h \
 CoverNode.h KambiViewpointCommonFields.h NodeCurveAnimation.h \
 NodePositionInterpolator.h NodeOrientationInterpolator.h NodeComment.h \
 NodeExport.h NodeImport.h
lexer.o: lexer.cpp stdafx.h config.h mysnprintf.h swt/include/swDebugf.h \
 Element.h MyString.h StringArray.h Array.h x3dFlags.h \
 swt/include/WriteFlags.h EventIn.h Node.h List.h Vec3f.h Vec3x.h \
 FieldIndex.h NodeFlags.h MeshFlags.h ac3dMaterialCallback.h NodeArray.h \
 Stack.h Matrix.h Vec3d.h Quaternion.h EventOut.h ExposedField.h Field.h \
 FieldValue.h Arrays.h MySwap.h Types.h Scene.h Map.h CommandList.h \
 Command.h NodeList.h URL.h Proto.h ProtoMacros.h InterfaceArray.h \
 DynamicFieldsNode.h SFMFTypes.h MFBool.h MFColor.h MFFloat.h SFFloat.h \
 MFColorRGBA.h MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h \
 MFNode.h MFRotation.h MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h \
 MFVec2f.h MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h \
 SFColorRGBA.h SFImage.h SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h \
 SFRotation.h SFString.h SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h \
 SFVec3f.h SFVec4f.h DuneApp.h PreferencesApp.h swt/include/swttypedef.h \
 EcmaScriptApp.h StereoViewApp.h swt/include/stereoview.h \
 InputDeviceApp.h TransformMode.h OutputApp.h RouteViewApp.h \
 StartWithApp.h ExportNamesApp.h ModellingApp.h RenderApp.h RenderState.h \
 ProtoMap.h parser.h y.tab.h
main.o: main.cpp stdafx.h config.h mysnprintf.h swt/include/swt.h \
 swt/include/keycodes.h swt/include/swlanguages.h swt/include/swDebugf.h \
 swt/include/swttypedef.h DuneApp.h List.h Array.h MyString.h \
 StringArray.h Node.h Vec3f.h Vec3x.h FieldIndex.h Element.h x3dFlags.h \
 swt/include/WriteFlags.h NodeFlags.h MeshFlags.h ac3dMaterialCallback.h \
 NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h URL.h PreferencesApp.h \
 swt/include/swttypedef.h EcmaScriptApp.h StereoViewApp.h \
 swt/include/stereoview.h InputDeviceApp.h TransformMode.h OutputApp.h \
 RouteViewApp.h StartWithApp.h ExportNamesApp.h ModellingApp.h \
 RenderApp.h MainWindow.h PanedWindow.h SceneView.h Scene.h Map.h \
 CommandList.h Command.h NodeList.h Proto.h ProtoMacros.h \
 InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h \
 Arrays.h MySwap.h Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h \
 MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h \
 MFRotation.h MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h \
 MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h \
 SFImage.h SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h \
 SFString.h SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h \
 SFVec4f.h RenderState.h ProtoMap.h ColorConversion.h NodeNurbsCurve.h \
 NurbsCurve.h GeometryNode.h NodeAppearance.h NodeMaterial.h \
 NodeImageTexture.h KambiTextureCommonFields.h NodeTextureTransform.h \
 GeometryNodeMacros.h ChainBasedNode.h xerrorhandler.h
NodeAnchor.o: NodeAnchor.cpp stdafx.h config.h mysnprintf.h NodeAnchor.h \
 Node.h Array.h List.h Vec3f.h Vec3x.h MyString.h StringArray.h \
 FieldIndex.h Element.h x3dFlags.h swt/include/WriteFlags.h NodeFlags.h \
 MeshFlags.h ac3dMaterialCallback.h NodeArray.h Stack.h Matrix.h Vec3d.h \
 Quaternion.h ProtoMacros.h Proto.h Map.h NodeList.h InterfaceArray.h \
 DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h Arrays.h MySwap.h \
 Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h MFDouble.h \
 SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h MFRotation.h \
 MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h \
 MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h SFImage.h \
 SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h SFString.h \
 SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h \
 CommandList.h Command.h NodeGroup.h StaticGroupNode.h GroupNode.h \
 Field.h
NodeAppearance.o: NodeAppearance.cpp stdafx.h config.h mysnprintf.h \
 NodeAppearance.h Node.h Array.h List.h Vec3f.h Vec3x.h MyString.h \
 StringArray.h FieldIndex.h Element.h x3dFlags.h swt/include/WriteFlags.h \
 NodeFlags.h MeshFlags.h ac3dMaterialCallback.h NodeArray.h Stack.h \
 Matrix.h Vec3d.h Quaternion.h ProtoMacros.h Proto.h Map.h NodeList.h \
 InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h \
 Arrays.h MySwap.h Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h \
 MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h \
 MFRotation.h MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h \
 MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h \
 SFImage.h SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h \
 SFString.h SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h \
 SFVec4f.h CommandList.h Command.h DuneApp.h URL.h PreferencesApp.h \
 swt/include/swttypedef.h EcmaScriptApp.h StereoViewApp.h \
 swt/include/stereoview.h InputDeviceApp.h TransformMode.h OutputApp.h \
 RouteViewApp.h StartWithApp.h ExportNamesApp.h ModellingApp.h \
 RenderApp.h NodeMaterial.h swt/include/swt.h swt/include/keycodes.h \
 swt/include/swlanguages.h swt/include/swDebugf.h \
 swt/include/swttypedef.h NodeTwoSidedMaterial.h Util.h
NodeAudioClip.o: NodeAudioClip.cpp stdafx.h config.h mysnprintf.h \
 NodeAudioClip.h Node.h Array.h List.h Vec3f.h Vec3x.h MyString.h \
 StringArray.h FieldIndex.h Element.h x3dFlags.h swt/include/WriteFlags.h \
 NodeFlags.h MeshFlags.h ac3dMaterialCallback.h NodeArray.h Stack.h \
 Matrix.h Vec3d.h Quaternion.h ProtoMacros.h Proto.h Map.h NodeList.h \
 InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h \
 Arrays.h MySwap.h Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h \
 MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h \
 MFRotation.h MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h \
 MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h \
 SFImage.h SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h \
 SFString.h SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h \
 SFVec4f.h CommandList.h Command.h Field.h ExposedField.h Scene.h URL.h \
 DuneApp.h PreferencesApp.h swt/include/swttypedef.h EcmaScriptApp.h \
 StereoViewApp.h swt/include/stereoview.h InputDeviceApp.h \
 TransformMode.h OutputApp.h RouteViewApp.h StartWithApp.h \
 ExportNamesApp.h ModellingApp.h RenderApp.h RenderState.h ProtoMap.h
NodeArc2D.o: NodeArc2D.cpp stdafx.h config.h mysnprintf.h NodeArc2D.h \
 ChainBasedNode.h GeometryNode.h Node.h Array.h List.h Vec3f.h Vec3x.h \
 MyString.h StringArray.h FieldIndex.h Element.h x3dFlags.h \
 swt/include/WriteFlags.h NodeFlags.h MeshFlags.h ac3dMaterialCallback.h \
 NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h NodeAppearance.h \
 ProtoMacros.h Proto.h Map.h NodeList.h InterfaceArray.h \
 DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h Arrays.h MySwap.h \
 Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h MFDouble.h \
 SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h MFRotation.h \
 MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h \
 MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h SFImage.h \
 SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h SFString.h \
 SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h \
 CommandList.h Command.h DuneApp.h URL.h PreferencesApp.h \
 swt/include/swttypedef.h EcmaScriptApp.h StereoViewApp.h \
 swt/include/stereoview.h InputDeviceApp.h TransformMode.h OutputApp.h \
 RouteViewApp.h StartWithApp.h ExportNamesApp.h ModellingApp.h \
 RenderApp.h NodeMaterial.h swt/include/swt.h swt/include/keycodes.h \
 swt/include/swlanguages.h swt/include/swDebugf.h \
 swt/include/swttypedef.h NodeImageTexture.h KambiTextureCommonFields.h \
 NodeTextureTransform.h GeometryNodeMacros.h Scene.h RenderState.h \
 ProtoMap.h Util.h
NodeArcClose2D.o: NodeArcClose2D.cpp stdafx.h config.h mysnprintf.h \
 NodeArcClose2D.h MeshBasedNode.h GeometryNode.h Node.h Array.h List.h \
 Vec3f.h Vec3x.h MyString.h StringArray.h FieldIndex.h Element.h \
 x3dFlags.h swt/include/WriteFlags.h NodeFlags.h MeshFlags.h \
 ac3dMaterialCallback.h NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h \
 NodeAppearance.h ProtoMacros.h Proto.h Map.h NodeList.h InterfaceArray.h \
 DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h Arrays.h MySwap.h \
 Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h MFDouble.h \
 SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h MFRotation.h \
 MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h \
 MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h SFImage.h \
 SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h SFString.h \
 SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h \
 CommandList.h Command.h DuneApp.h URL.h PreferencesApp.h \
 swt/include/swttypedef.h EcmaScriptApp.h StereoViewApp.h \
 swt/include/stereoview.h InputDeviceApp.h TransformMode.h OutputApp.h \
 RouteViewApp.h StartWithApp.h ExportNamesApp.h ModellingApp.h \
 RenderApp.h NodeMaterial.h swt/include/swt.h swt/include/keycodes.h \
 swt/include/swlanguages.h swt/include/swDebugf.h \
 swt/include/swttypedef.h NodeImageTexture.h KambiTextureCommonFields.h \
 NodeTextureTransform.h GeometryNodeMacros.h MyMesh.h \
 NodeIndexedFaceSet.h NodeCoordinate.h Colored.h NodeColor.h \
 NodeColorRGBA.h ComposedGeometryMacros.h FaceData.h NodeNormal.h \
 NodeTextureCoordinate.h NodeTextureCoordinateGenerator.h \
 NodeMultiTextureCoordinate.h Scene.h RenderState.h ProtoMap.h Util.h
NodeBackground.o: NodeBackground.cpp stdafx.h config.h mysnprintf.h \
 NodeBackground.h Node.h Array.h List.h Vec3f.h Vec3x.h MyString.h \
 StringArray.h FieldIndex.h Element.h x3dFlags.h swt/include/WriteFlags.h \
 NodeFlags.h MeshFlags.h ac3dMaterialCallback.h NodeArray.h Stack.h \
 Matrix.h Vec3d.h Quaternion.h ProtoMacros.h Proto.h Map.h NodeList.h \
 InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h \
 Arrays.h MySwap.h Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h \
 MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h \
 MFRotation.h MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h \
 MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h \
 SFImage.h SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h \
 SFString.h SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h \
 SFVec4f.h CommandList.h Command.h NodeImageTexture.h \
 KambiTextureCommonFields.h NodeViewpoint.h ViewpointNode.h \
 swt/include/swt.h swt/include/keycodes.h swt/include/swlanguages.h \
 swt/include/swDebugf.h swt/include/swttypedef.h CoverNode.h \
 KambiViewpointCommonFields.h DuneApp.h URL.h PreferencesApp.h \
 swt/include/swttypedef.h EcmaScriptApp.h StereoViewApp.h \
 swt/include/stereoview.h InputDeviceApp.h TransformMode.h OutputApp.h \
 RouteViewApp.h StartWithApp.h ExportNamesApp.h ModellingApp.h \
 RenderApp.h Scene.h RenderState.h ProtoMap.h Util.h Field.h
NodeBallJoint.o: NodeBallJoint.cpp stdafx.h config.h mysnprintf.h \
 NodeBallJoint.h AnchoredJointNode.h SFMFTypes.h MFBool.h Array.h \
 FieldValue.h MyString.h StringArray.h Element.h x3dFlags.h \
 swt/include/WriteFlags.h Arrays.h MySwap.h Types.h MFColor.h MFFloat.h \
 SFFloat.h MFColorRGBA.h MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h \
 MFMatrix4f.h MFNode.h Node.h List.h Vec3f.h Vec3x.h FieldIndex.h \
 NodeFlags.h MeshFlags.h ac3dMaterialCallback.h NodeArray.h Stack.h \
 Matrix.h Vec3d.h Quaternion.h NodeList.h MFRotation.h MFString.h \
 MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h MFVec3f.h \
 MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h SFImage.h SFInt32.h \
 SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h SFString.h SFVec2d.h \
 SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h \
 X3DRigidJointNode.h RigidBodyPhysicsNode.h ProtoMacros.h Proto.h Map.h \
 InterfaceArray.h DynamicFieldsNode.h CommandList.h Command.h Field.h \
 DuneApp.h URL.h PreferencesApp.h swt/include/swttypedef.h \
 EcmaScriptApp.h StereoViewApp.h swt/include/stereoview.h \
 InputDeviceApp.h TransformMode.h OutputApp.h RouteViewApp.h \
 StartWithApp.h ExportNamesApp.h ModellingApp.h RenderApp.h Scene.h \
 RenderState.h ProtoMap.h NodeNavigationInfo.h
NodeBillboard.o: NodeBillboard.cpp stdafx.h config.h mysnprintf.h \
 NodeBillboard.h Node.h Array.h List.h Vec3f.h Vec3x.h MyString.h \
 StringArray.h FieldIndex.h Element.h x3dFlags.h swt/include/WriteFlags.h \
 NodeFlags.h MeshFlags.h ac3dMaterialCallback.h NodeArray.h Stack.h \
 Matrix.h Vec3d.h Quaternion.h ProtoMacros.h Proto.h Map.h NodeList.h \
 InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h \
 Arrays.h MySwap.h Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h \
 MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h \
 MFRotation.h MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h \
 MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h \
 SFImage.h SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h \
 SFString.h SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h \
 SFVec4f.h CommandList.h Command.h Scene.h URL.h DuneApp.h \
 PreferencesApp.h swt/include/swttypedef.h EcmaScriptApp.h \
 StereoViewApp.h swt/include/stereoview.h InputDeviceApp.h \
 TransformMode.h OutputApp.h RouteViewApp.h StartWithApp.h \
 ExportNamesApp.h ModellingApp.h RenderApp.h RenderState.h ProtoMap.h \
 NodeViewpoint.h ViewpointNode.h swt/include/swt.h swt/include/keycodes.h \
 swt/include/swlanguages.h swt/include/swDebugf.h \
 swt/include/swttypedef.h CoverNode.h KambiViewpointCommonFields.h Util.h \
 resource.h
NodeBlendedVolumeStyle.o: NodeBlendedVolumeStyle.cpp stdafx.h config.h \
 mysnprintf.h NodeBlendedVolumeStyle.h Node.h Array.h List.h Vec3f.h \
 Vec3x.h MyString.h StringArray.h FieldIndex.h Element.h x3dFlags.h \
 swt/include/WriteFlags.h NodeFlags.h MeshFlags.h ac3dMaterialCallback.h \
 NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h ProtoMacros.h Proto.h \
 Map.h NodeList.h InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h \
 MFBool.h FieldValue.h Arrays.h MySwap.h Types.h MFColor.h MFFloat.h \
 SFFloat.h MFColorRGBA.h MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h \
 MFMatrix4f.h MFNode.h MFRotation.h MFString.h MFTime.h SFTime.h \
 MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h \
 SFBool.h SFColor.h SFColorRGBA.h SFImage.h SFInt32.h SFMatrix3f.h \
 SFMatrix4f.h SFNode.h SFRotation.h SFString.h SFVec2d.h SFVec2f.h \
 Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h CommandList.h Command.h \
 DuneApp.h URL.h PreferencesApp.h swt/include/swttypedef.h \
 EcmaScriptApp.h StereoViewApp.h swt/include/stereoview.h \
 InputDeviceApp.h TransformMode.h OutputApp.h RouteViewApp.h \
 StartWithApp.h ExportNamesApp.h ModellingApp.h RenderApp.h
NodeBooleanFilter.o: NodeBooleanFilter.cpp stdafx.h config.h mysnprintf.h \
 swt/include/swt.h swt/include/keycodes.h swt/include/swlanguages.h \
 swt/include/swDebugf.h swt/include/swttypedef.h DuneApp.h List.h Array.h \
 MyString.h StringArray.h Node.h Vec3f.h Vec3x.h FieldIndex.h Element.h \
 x3dFlags.h swt/include/WriteFlags.h NodeFlags.h MeshFlags.h \
 ac3dMaterialCallback.h NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h \
 URL.h PreferencesApp.h swt/include/swttypedef.h EcmaScriptApp.h \
 StereoViewApp.h swt/include/stereoview.h InputDeviceApp.h \
 TransformMode.h OutputApp.h RouteViewApp.h StartWithApp.h \
 ExportNamesApp.h ModellingApp.h RenderApp.h resource.h \
 NodeBooleanFilter.h ProtoMacros.h Proto.h Map.h NodeList.h \
 InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h \
 Arrays.h MySwap.h Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h \
 MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h \
 MFRotation.h MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h \
 MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h \
 SFImage.h SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h \
 SFString.h SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h \
 SFVec4f.h CommandList.h Command.h Texture.h
NodeBooleanSequencer.o: NodeBooleanSequencer.cpp stdafx.h config.h \
 mysnprintf.h swt/include/swt.h swt/include/keycodes.h \
 swt/include/swlanguages.h swt/include/swDebugf.h \
 swt/include/swttypedef.h DuneApp.h List.h Array.h MyString.h \
 StringArray.h Node.h Vec3f.h Vec3x.h FieldIndex.h Element.h x3dFlags.h \
 swt/include/WriteFlags.h NodeFlags.h MeshFlags.h ac3dMaterialCallback.h \
 NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h URL.h PreferencesApp.h \
 swt/include/swttypedef.h EcmaScriptApp.h StereoViewApp.h \
 swt/include/stereoview.h InputDeviceApp.h TransformMode.h OutputApp.h \
 RouteViewApp.h StartWithApp.h ExportNamesApp.h ModellingApp.h \
 RenderApp.h resource.h NodeBooleanSequencer.h Interpolator.h \
 ProtoMacros.h Proto.h Map.h NodeList.h InterfaceArray.h \
 DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h Arrays.h MySwap.h \
 Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h MFDouble.h \
 SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h MFRotation.h \
 MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h \
 MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h SFImage.h \
 SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h SFString.h \
 SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h \
 CommandList.h Command.h Texture.h Scene.h RenderState.h ProtoMap.h \
 FieldCommand.h
NodeBooleanToggle.o: NodeBooleanToggle.cpp stdafx.h config.h mysnprintf.h \
 swt/include/swt.h swt/include/keycodes.h swt/include/swlanguages.h \
 swt/include/swDebugf.h swt/include/swttypedef.h DuneApp.h List.h Array.h \
 MyString.h StringArray.h Node.h Vec3f.h Vec3x.h FieldIndex.h Element.h \
 x3dFlags.h swt/include/WriteFlags.h NodeFlags.h MeshFlags.h \
 ac3dMaterialCallback.h NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h \
 URL.h PreferencesApp.h swt/include/swttypedef.h EcmaScriptApp.h \
 StereoViewApp.h swt/include/stereoview.h InputDeviceApp.h \
 TransformMode.h OutputApp.h RouteViewApp.h StartWithApp.h \
 ExportNamesApp.h ModellingApp.h RenderApp.h resource.h \
 NodeBooleanToggle.h ProtoMacros.h Proto.h Map.h NodeList.h \
 InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h \
 Arrays.h MySwap.h Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h \
 MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h \
 MFRotation.h MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h \
 MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h \
 SFImage.h SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h \
 SFString.h SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h \
 SFVec4f.h CommandList.h Command.h Texture.h
NodeBooleanTrigger.o: NodeBooleanTrigger.cpp stdafx.h config.h \
 mysnprintf.h swt/include/swt.h swt/include/keycodes.h \
 swt/include/swlanguages.h swt/include/swDebugf.h \
 swt/include/swttypedef.h DuneApp.h List.h Array.h MyString.h \
 StringArray.h Node.h Vec3f.h Vec3x.h FieldIndex.h Element.h x3dFlags.h \
 swt/include/WriteFlags.h NodeFlags.h MeshFlags.h ac3dMaterialCallback.h \
 NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h URL.h PreferencesApp.h \
 swt/include/swttypedef.h EcmaScriptApp.h StereoViewApp.h \
 swt/include/stereoview.h InputDeviceApp.h TransformMode.h OutputApp.h \
 RouteViewApp.h StartWithApp.h ExportNamesApp.h ModellingApp.h \
 RenderApp.h resource.h NodeBooleanTrigger.h ProtoMacros.h Proto.h Map.h \
 NodeList.h InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h MFBool.h \
 FieldValue.h Arrays.h MySwap.h Types.h MFColor.h MFFloat.h SFFloat.h \
 MFColorRGBA.h MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h \
 MFNode.h MFRotation.h MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h \
 MFVec2f.h MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h \
 SFColorRGBA.h SFImage.h SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h \
 SFRotation.h SFString.h SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h \
 SFVec3f.h SFVec4f.h CommandList.h Command.h Texture.h
NodeBoundaryEnhancementVolumeStyle.o: \
 NodeBoundaryEnhancementVolumeStyle.cpp stdafx.h config.h mysnprintf.h \
 NodeBoundaryEnhancementVolumeStyle.h Node.h Array.h List.h Vec3f.h \
 Vec3x.h MyString.h StringArray.h FieldIndex.h Element.h x3dFlags.h \
 swt/include/WriteFlags.h NodeFlags.h MeshFlags.h ac3dMaterialCallback.h \
 NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h ProtoMacros.h Proto.h \
 Map.h NodeList.h InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h \
 MFBool.h FieldValue.h Arrays.h MySwap.h Types.h MFColor.h MFFloat.h \
 SFFloat.h MFColorRGBA.h MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h \
 MFMatrix4f.h MFNode.h MFRotation.h MFString.h MFTime.h SFTime.h \
 MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h \
 SFBool.h SFColor.h SFColorRGBA.h SFImage.h SFInt32.h SFMatrix3f.h \
 SFMatrix4f.h SFNode.h SFRotation.h SFString.h SFVec2d.h SFVec2f.h \
 Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h CommandList.h Command.h \
 DuneApp.h URL.h PreferencesApp.h swt/include/swttypedef.h \
 EcmaScriptApp.h StereoViewApp.h swt/include/stereoview.h \
 InputDeviceApp.h TransformMode.h OutputApp.h RouteViewApp.h \
 StartWithApp.h ExportNamesApp.h ModellingApp.h RenderApp.h
NodeBoundedPhysicsModel.o: NodeBoundedPhysicsModel.cpp stdafx.h config.h \
 mysnprintf.h NodeBoundedPhysicsModel.h Node.h Array.h List.h Vec3f.h \
 Vec3x.h MyString.h StringArray.h FieldIndex.h Element.h x3dFlags.h \
 swt/include/WriteFlags.h NodeFlags.h MeshFlags.h ac3dMaterialCallback.h \
 NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h ProtoMacros.h Proto.h \
 Map.h NodeList.h InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h \
 MFBool.h FieldValue.h Arrays.h MySwap.h Types.h MFColor.h MFFloat.h \
 SFFloat.h MFColorRGBA.h MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h \
 MFMatrix4f.h MFNode.h MFRotation.h MFString.h MFTime.h SFTime.h \
 MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h \
 SFBool.h SFColor.h SFColorRGBA.h SFImage.h SFInt32.h SFMatrix3f.h \
 SFMatrix4f.h SFNode.h SFRotation.h SFString.h SFVec2d.h SFVec2f.h \
 Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h CommandList.h Command.h \
 DuneApp.h URL.h PreferencesApp.h swt/include/swttypedef.h \
 EcmaScriptApp.h StereoViewApp.h swt/include/stereoview.h \
 InputDeviceApp.h TransformMode.h OutputApp.h RouteViewApp.h \
 StartWithApp.h ExportNamesApp.h ModellingApp.h RenderApp.h
NodeBox.o: NodeBox.cpp stdafx.h config.h mysnprintf.h NodeBox.h \
 MeshBasedNode.h GeometryNode.h Node.h Array.h List.h Vec3f.h Vec3x.h \
 MyString.h StringArray.h FieldIndex.h Element.h x3dFlags.h \
 swt/include/WriteFlags.h NodeFlags.h MeshFlags.h ac3dMaterialCallback.h \
 NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h NodeAppearance.h \
 ProtoMacros.h Proto.h Map.h NodeList.h InterfaceArray.h \
 DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h Arrays.h MySwap.h \
 Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h MFDouble.h \
 SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h MFRotation.h \
 MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h \
 MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h SFImage.h \
 SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h SFString.h \
 SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h \
 CommandList.h Command.h DuneApp.h URL.h PreferencesApp.h \
 swt/include/swttypedef.h EcmaScriptApp.h StereoViewApp.h \
 swt/include/stereoview.h InputDeviceApp.h TransformMode.h OutputApp.h \
 RouteViewApp.h StartWithApp.h ExportNamesApp.h ModellingApp.h \
 RenderApp.h NodeMaterial.h swt/include/swt.h swt/include/keycodes.h \
 swt/include/swlanguages.h swt/include/swDebugf.h \
 swt/include/swttypedef.h NodeImageTexture.h KambiTextureCommonFields.h \
 NodeTextureTransform.h GeometryNodeMacros.h Scene.h RenderState.h \
 ProtoMap.h FieldCommand.h Util.h NodeShape.h NodeIndexedFaceSet.h \
 NodeCoordinate.h Colored.h NodeColor.h NodeColorRGBA.h \
 ComposedGeometryMacros.h NodeNormal.h NodeNurbsGroup.h NodeGroup.h \
 StaticGroupNode.h GroupNode.h NodeNurbsSurface.h MeshMorphingNode.h \
 Interpolator.h MyMesh.h FaceData.h NodeTextureCoordinate.h \
 NodeTextureCoordinateGenerator.h NodeMultiTextureCoordinate.h
NodeCADAssembly.o: NodeCADAssembly.cpp stdafx.h config.h mysnprintf.h \
 NodeCADAssembly.h Node.h Array.h List.h Vec3f.h Vec3x.h MyString.h \
 StringArray.h FieldIndex.h Element.h x3dFlags.h swt/include/WriteFlags.h \
 NodeFlags.h MeshFlags.h ac3dMaterialCallback.h NodeArray.h Stack.h \
 Matrix.h Vec3d.h Quaternion.h ProtoMacros.h Proto.h Map.h NodeList.h \
 InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h \
 Arrays.h MySwap.h Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h \
 MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h \
 MFRotation.h MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h \
 MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h \
 SFImage.h SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h \
 SFString.h SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h \
 SFVec4f.h CommandList.h Command.h StaticGroupNode.h GroupNode.h \
 DuneApp.h URL.h PreferencesApp.h swt/include/swttypedef.h \
 EcmaScriptApp.h StereoViewApp.h swt/include/stereoview.h \
 InputDeviceApp.h TransformMode.h OutputApp.h RouteViewApp.h \
 StartWithApp.h ExportNamesApp.h ModellingApp.h RenderApp.h
NodeCADFace.o: NodeCADFace.cpp stdafx.h config.h mysnprintf.h \
 NodeCADFace.h Node.h Array.h List.h Vec3f.h Vec3x.h MyString.h \
 StringArray.h FieldIndex.h Element.h x3dFlags.h swt/include/WriteFlags.h \
 NodeFlags.h MeshFlags.h ac3dMaterialCallback.h NodeArray.h Stack.h \
 Matrix.h Vec3d.h Quaternion.h ProtoMacros.h Proto.h Map.h NodeList.h \
 InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h \
 Arrays.h MySwap.h Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h \
 MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h \
 MFRotation.h MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h \
 MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h \
 SFImage.h SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h \
 SFString.h SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h \
 SFVec4f.h CommandList.h Command.h DuneApp.h URL.h PreferencesApp.h \
 swt/include/swttypedef.h EcmaScriptApp.h StereoViewApp.h \
 swt/include/stereoview.h InputDeviceApp.h TransformMode.h OutputApp.h \
 RouteViewApp.h StartWithApp.h ExportNamesApp.h ModellingApp.h \
 RenderApp.h
NodeCADLayer.o: NodeCADLayer.cpp stdafx.h config.h mysnprintf.h \
 NodeCADLayer.h Node.h Array.h List.h Vec3f.h Vec3x.h MyString.h \
 StringArray.h FieldIndex.h Element.h x3dFlags.h swt/include/WriteFlags.h \
 NodeFlags.h MeshFlags.h ac3dMaterialCallback.h NodeArray.h Stack.h \
 Matrix.h Vec3d.h Quaternion.h ProtoMacros.h Proto.h Map.h NodeList.h \
 InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h \
 Arrays.h MySwap.h Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h \
 MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h \
 MFRotation.h MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h \
 MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h \
 SFImage.h SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h \
 SFString.h SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h \
 SFVec4f.h CommandList.h Command.h StaticGroupNode.h GroupNode.h \
 DuneApp.h URL.h PreferencesApp.h swt/include/swttypedef.h \
 EcmaScriptApp.h StereoViewApp.h swt/include/stereoview.h \
 InputDeviceApp.h TransformMode.h OutputApp.h RouteViewApp.h \
 StartWithApp.h ExportNamesApp.h ModellingApp.h RenderApp.h
NodeCADPart.o: NodeCADPart.cpp stdafx.h config.h mysnprintf.h \
 NodeCADPart.h Node.h Array.h List.h Vec3f.h Vec3x.h MyString.h \
 StringArray.h FieldIndex.h Element.h x3dFlags.h swt/include/WriteFlags.h \
 NodeFlags.h MeshFlags.h ac3dMaterialCallback.h NodeArray.h Stack.h \
 Matrix.h Vec3d.h Quaternion.h ProtoMacros.h Proto.h Map.h NodeList.h \
 InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h \
 Arrays.h MySwap.h Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h \
 MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h \
 MFRotation.h MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h \
 MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h \
 SFImage.h SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h \
 SFString.h SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h \
 SFVec4f.h CommandList.h Command.h NodeTransform.h swt/include/swt.h \
 swt/include/keycodes.h swt/include/swlanguages.h swt/include/swDebugf.h \
 swt/include/swttypedef.h TransformNode.h BasicTransformNode.h DuneApp.h \
 URL.h PreferencesApp.h swt/include/swttypedef.h EcmaScriptApp.h \
 StereoViewApp.h swt/include/stereoview.h InputDeviceApp.h \
 TransformMode.h OutputApp.h RouteViewApp.h StartWithApp.h \
 ExportNamesApp.h ModellingApp.h RenderApp.h
NodeCartoonVolumeStyle.o: NodeCartoonVolumeStyle.cpp stdafx.h config.h \
 mysnprintf.h NodeCartoonVolumeStyle.h Node.h Array.h List.h Vec3f.h \
 Vec3x.h MyString.h StringArray.h FieldIndex.h Element.h x3dFlags.h \
 swt/include/WriteFlags.h NodeFlags.h MeshFlags.h ac3dMaterialCallback.h \
 NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h ProtoMacros.h Proto.h \
 Map.h NodeList.h InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h \
 MFBool.h FieldValue.h Arrays.h MySwap.h Types.h MFColor.h MFFloat.h \
 SFFloat.h MFColorRGBA.h MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h \
 MFMatrix4f.h MFNode.h MFRotation.h MFString.h MFTime.h SFTime.h \
 MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h \
 SFBool.h SFColor.h SFColorRGBA.h SFImage.h SFInt32.h SFMatrix3f.h \
 SFMatrix4f.h SFNode.h SFRotation.h SFString.h SFVec2d.h SFVec2f.h \
 Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h CommandList.h Command.h \
 DuneApp.h URL.h PreferencesApp.h swt/include/swttypedef.h \
 EcmaScriptApp.h StereoViewApp.h swt/include/stereoview.h \
 InputDeviceApp.h TransformMode.h OutputApp.h RouteViewApp.h \
 StartWithApp.h ExportNamesApp.h ModellingApp.h RenderApp.h
NodeCircle2D.o: NodeCircle2D.cpp stdafx.h config.h mysnprintf.h \
 NodeCircle2D.h ChainBasedNode.h GeometryNode.h Node.h Array.h List.h \
 Vec3f.h Vec3x.h MyString.h StringArray.h FieldIndex.h Element.h \
 x3dFlags.h swt/include/WriteFlags.h NodeFlags.h MeshFlags.h \
 ac3dMaterialCallback.h NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h \
 NodeAppearance.h ProtoMacros.h Proto.h Map.h NodeList.h InterfaceArray.h \
 DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h Arrays.h MySwap.h \
 Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h MFDouble.h \
 SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h MFRotation.h \
 MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h \
 MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h SFImage.h \
 SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h SFString.h \
 SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h \
 CommandList.h Command.h DuneApp.h URL.h PreferencesApp.h \
 swt/include/swttypedef.h EcmaScriptApp.h StereoViewApp.h \
 swt/include/stereoview.h InputDeviceApp.h TransformMode.h OutputApp.h \
 RouteViewApp.h StartWithApp.h ExportNamesApp.h ModellingApp.h \
 RenderApp.h NodeMaterial.h swt/include/swt.h swt/include/keycodes.h \
 swt/include/swlanguages.h swt/include/swDebugf.h \
 swt/include/swttypedef.h NodeImageTexture.h KambiTextureCommonFields.h \
 NodeTextureTransform.h GeometryNodeMacros.h Scene.h RenderState.h \
 ProtoMap.h Util.h
NodeClipPlane.o: NodeClipPlane.cpp stdafx.h config.h mysnprintf.h \
 NodeClipPlane.h Node.h Array.h List.h Vec3f.h Vec3x.h MyString.h \
 StringArray.h FieldIndex.h Element.h x3dFlags.h swt/include/WriteFlags.h \
 NodeFlags.h MeshFlags.h ac3dMaterialCallback.h NodeArray.h Stack.h \
 Matrix.h Vec3d.h Quaternion.h ProtoMacros.h Proto.h Map.h NodeList.h \
 InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h \
 Arrays.h MySwap.h Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h \
 MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h \
 MFRotation.h MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h \
 MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h \
 SFImage.h SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h \
 SFString.h SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h \
 SFVec4f.h CommandList.h Command.h RenderState.h swt/include/swttypedef.h \
 DuneApp.h URL.h PreferencesApp.h EcmaScriptApp.h StereoViewApp.h \
 swt/include/stereoview.h InputDeviceApp.h TransformMode.h OutputApp.h \
 RouteViewApp.h StartWithApp.h ExportNamesApp.h ModellingApp.h \
 RenderApp.h Scene.h ProtoMap.h
NodeCollidableShape.o: NodeCollidableShape.cpp stdafx.h config.h \
 mysnprintf.h NodeCollidableShape.h X3DNBodyCollidableNode.h \
 RigidBodyPhysicsNode.h Node.h Array.h List.h Vec3f.h Vec3x.h MyString.h \
 StringArray.h FieldIndex.h Element.h x3dFlags.h swt/include/WriteFlags.h \
 NodeFlags.h MeshFlags.h ac3dMaterialCallback.h NodeArray.h Stack.h \
 Matrix.h Vec3d.h Quaternion.h ProtoMacros.h Proto.h Map.h NodeList.h \
 InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h \
 Arrays.h MySwap.h Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h \
 MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h \
 MFRotation.h MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h \
 MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h \
 SFImage.h SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h \
 SFString.h SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h \
 SFVec4f.h CommandList.h Command.h Field.h DuneApp.h URL.h \
 PreferencesApp.h swt/include/swttypedef.h EcmaScriptApp.h \
 StereoViewApp.h swt/include/stereoview.h InputDeviceApp.h \
 TransformMode.h OutputApp.h RouteViewApp.h StartWithApp.h \
 ExportNamesApp.h ModellingApp.h RenderApp.h Scene.h RenderState.h \
 ProtoMap.h
NodeCollidableOffset.o: NodeCollidableOffset.cpp stdafx.h config.h \
 mysnprintf.h NodeCollidableOffset.h X3DNBodyCollidableNode.h \
 RigidBodyPhysicsNode.h Node.h Array.h List.h Vec3f.h Vec3x.h MyString.h \
 StringArray.h FieldIndex.h Element.h x3dFlags.h swt/include/WriteFlags.h \
 NodeFlags.h MeshFlags.h ac3dMaterialCallback.h NodeArray.h Stack.h \
 Matrix.h Vec3d.h Quaternion.h ProtoMacros.h Proto.h Map.h NodeList.h \
 InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h \
 Arrays.h MySwap.h Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h \
 MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h \
 MFRotation.h MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h \
 MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h \
 SFImage.h SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h \
 SFString.h SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h \
 SFVec4f.h CommandList.h Command.h Field.h DuneApp.h URL.h \
 PreferencesApp.h swt/include/swttypedef.h EcmaScriptApp.h \
 StereoViewApp.h swt/include/stereoview.h InputDeviceApp.h \
 TransformMode.h OutputApp.h RouteViewApp.h StartWithApp.h \
 ExportNamesApp.h ModellingApp.h RenderApp.h Scene.h RenderState.h \
 ProtoMap.h
NodeCollision.o: NodeCollision.cpp stdafx.h config.h mysnprintf.h \
 NodeCollision.h Node.h Array.h List.h Vec3f.h Vec3x.h MyString.h \
 StringArray.h FieldIndex.h Element.h x3dFlags.h swt/include/WriteFlags.h \
 NodeFlags.h MeshFlags.h ac3dMaterialCallback.h NodeArray.h Stack.h \
 Matrix.h Vec3d.h Quaternion.h ProtoMacros.h Proto.h Map.h NodeList.h \
 InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h \
 Arrays.h MySwap.h Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h \
 MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h \
 MFRotation.h MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h \
 MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h \
 SFImage.h SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h \
 SFString.h SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h \
 SFVec4f.h CommandList.h Command.h Field.h
NodeCollisionCollection.o: NodeCollisionCollection.cpp stdafx.h config.h \
 mysnprintf.h NodeCollisionCollection.h RigidBodyPhysicsNode.h Node.h \
 Array.h List.h Vec3f.h Vec3x.h MyString.h StringArray.h FieldIndex.h \
 Element.h x3dFlags.h swt/include/WriteFlags.h NodeFlags.h MeshFlags.h \
 ac3dMaterialCallback.h NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h \
 ProtoMacros.h Proto.h Map.h NodeList.h InterfaceArray.h \
 DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h Arrays.h MySwap.h \
 Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h MFDouble.h \
 SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h MFRotation.h \
 MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h \
 MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h SFImage.h \
 SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h SFString.h \
 SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h \
 CommandList.h Command.h DuneApp.h URL.h PreferencesApp.h \
 swt/include/swttypedef.h EcmaScriptApp.h StereoViewApp.h \
 swt/include/stereoview.h InputDeviceApp.h TransformMode.h OutputApp.h \
 RouteViewApp.h StartWithApp.h ExportNamesApp.h ModellingApp.h \
 RenderApp.h
NodeCollisionSensor.o: NodeCollisionSensor.cpp stdafx.h config.h \
 mysnprintf.h NodeCollisionSensor.h RigidBodyPhysicsNode.h Node.h Array.h \
 List.h Vec3f.h Vec3x.h MyString.h StringArray.h FieldIndex.h Element.h \
 x3dFlags.h swt/include/WriteFlags.h NodeFlags.h MeshFlags.h \
 ac3dMaterialCallback.h NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h \
 ProtoMacros.h Proto.h Map.h NodeList.h InterfaceArray.h \
 DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h Arrays.h MySwap.h \
 Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h MFDouble.h \
 SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h MFRotation.h \
 MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h \
 MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h SFImage.h \
 SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h SFString.h \
 SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h \
 CommandList.h Command.h DuneApp.h URL.h PreferencesApp.h \
 swt/include/swttypedef.h EcmaScriptApp.h StereoViewApp.h \
 swt/include/stereoview.h InputDeviceApp.h TransformMode.h OutputApp.h \
 RouteViewApp.h StartWithApp.h ExportNamesApp.h ModellingApp.h \
 RenderApp.h
NodeCollisionSpace.o: NodeCollisionSpace.cpp stdafx.h config.h \
 mysnprintf.h NodeCollisionSpace.h X3DNBodyCollisionSpaceNode.h \
 RigidBodyPhysicsNode.h Node.h Array.h List.h Vec3f.h Vec3x.h MyString.h \
 StringArray.h FieldIndex.h Element.h x3dFlags.h swt/include/WriteFlags.h \
 NodeFlags.h MeshFlags.h ac3dMaterialCallback.h NodeArray.h Stack.h \
 Matrix.h Vec3d.h Quaternion.h ProtoMacros.h Proto.h Map.h NodeList.h \
 InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h \
 Arrays.h MySwap.h Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h \
 MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h \
 MFRotation.h MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h \
 MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h \
 SFImage.h SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h \
 SFString.h SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h \
 SFVec4f.h CommandList.h Command.h Field.h DuneApp.h URL.h \
 PreferencesApp.h swt/include/swttypedef.h EcmaScriptApp.h \
 StereoViewApp.h swt/include/stereoview.h InputDeviceApp.h \
 TransformMode.h OutputApp.h RouteViewApp.h StartWithApp.h \
 ExportNamesApp.h ModellingApp.h RenderApp.h Scene.h RenderState.h \
 ProtoMap.h
NodeColor.o: NodeColor.cpp stdafx.h config.h mysnprintf.h NodeColor.h \
 Node.h Array.h List.h Vec3f.h Vec3x.h MyString.h StringArray.h \
 FieldIndex.h Element.h x3dFlags.h swt/include/WriteFlags.h NodeFlags.h \
 MeshFlags.h ac3dMaterialCallback.h NodeArray.h Stack.h Matrix.h Vec3d.h \
 Quaternion.h ProtoMacros.h Proto.h Map.h NodeList.h InterfaceArray.h \
 DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h Arrays.h MySwap.h \
 Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h MFDouble.h \
 SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h MFRotation.h \
 MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h \
 MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h SFImage.h \
 SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h SFString.h \
 SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h \
 CommandList.h Command.h NodeShape.h NodeAppearance.h DuneApp.h URL.h \
 PreferencesApp.h swt/include/swttypedef.h EcmaScriptApp.h \
 StereoViewApp.h swt/include/stereoview.h InputDeviceApp.h \
 TransformMode.h OutputApp.h RouteViewApp.h StartWithApp.h \
 ExportNamesApp.h ModellingApp.h RenderApp.h NodeMaterial.h \
 swt/include/swt.h swt/include/keycodes.h swt/include/swlanguages.h \
 swt/include/swDebugf.h swt/include/swttypedef.h NodeIndexedFaceSet.h \
 MeshBasedNode.h GeometryNode.h NodeImageTexture.h \
 KambiTextureCommonFields.h NodeTextureTransform.h GeometryNodeMacros.h \
 NodeCoordinate.h Colored.h NodeColorRGBA.h ComposedGeometryMacros.h \
 NodeElevationGrid.h NodeGeoElevationGrid.h GeoNode.h NodeGeoOrigin.h \
 Util.h
NodeColorChaser.o: NodeColorChaser.cpp stdafx.h config.h mysnprintf.h \
 NodeColorChaser.h Node.h Array.h List.h Vec3f.h Vec3x.h MyString.h \
 StringArray.h FieldIndex.h Element.h x3dFlags.h swt/include/WriteFlags.h \
 NodeFlags.h MeshFlags.h ac3dMaterialCallback.h NodeArray.h Stack.h \
 Matrix.h Vec3d.h Quaternion.h ProtoMacros.h Proto.h Map.h NodeList.h \
 InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h \
 Arrays.h MySwap.h Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h \
 MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h \
 MFRotation.h MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h \
 MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h \
 SFImage.h SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h \
 SFString.h SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h \
 SFVec4f.h CommandList.h Command.h ChaserNode.h DuneApp.h URL.h \
 PreferencesApp.h swt/include/swttypedef.h EcmaScriptApp.h \
 StereoViewApp.h swt/include/stereoview.h InputDeviceApp.h \
 TransformMode.h OutputApp.h RouteViewApp.h StartWithApp.h \
 ExportNamesApp.h ModellingApp.h RenderApp.h
NodeColorDamper.o: NodeColorDamper.cpp stdafx.h config.h mysnprintf.h \
 NodeColorDamper.h Node.h Array.h List.h Vec3f.h Vec3x.h MyString.h \
 StringArray.h FieldIndex.h Element.h x3dFlags.h swt/include/WriteFlags.h \
 NodeFlags.h MeshFlags.h ac3dMaterialCallback.h NodeArray.h Stack.h \
 Matrix.h Vec3d.h Quaternion.h ProtoMacros.h Proto.h Map.h NodeList.h \
 InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h \
 Arrays.h MySwap.h Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h \
 MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h \
 MFRotation.h MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h \
 MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h \
 SFImage.h SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h \
 SFString.h SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h \
 SFVec4f.h CommandList.h Command.h DamperNode.h DuneApp.h URL.h \
 PreferencesApp.h swt/include/swttypedef.h EcmaScriptApp.h \
 StereoViewApp.h swt/include/stereoview.h InputDeviceApp.h \
 TransformMode.h OutputApp.h RouteViewApp.h StartWithApp.h \
 ExportNamesApp.h ModellingApp.h RenderApp.h
NodeColorRGBA.o: NodeColorRGBA.cpp stdafx.h config.h mysnprintf.h \
 NodeColorRGBA.h Node.h Array.h List.h Vec3f.h Vec3x.h MyString.h \
 StringArray.h FieldIndex.h Element.h x3dFlags.h swt/include/WriteFlags.h \
 NodeFlags.h MeshFlags.h ac3dMaterialCallback.h NodeArray.h Stack.h \
 Matrix.h Vec3d.h Quaternion.h ProtoMacros.h Proto.h Map.h NodeList.h \
 InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h \
 Arrays.h MySwap.h Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h \
 MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h \
 MFRotation.h MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h \
 MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h \
 SFImage.h SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h \
 SFString.h SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h \
 SFVec4f.h CommandList.h Command.h NodeShape.h NodeAppearance.h DuneApp.h \
 URL.h PreferencesApp.h swt/include/swttypedef.h EcmaScriptApp.h \
 StereoViewApp.h swt/include/stereoview.h InputDeviceApp.h \
 TransformMode.h OutputApp.h RouteViewApp.h StartWithApp.h \
 ExportNamesApp.h ModellingApp.h RenderApp.h NodeMaterial.h \
 swt/include/swt.h swt/include/keycodes.h swt/include/swlanguages.h \
 swt/include/swDebugf.h swt/include/swttypedef.h NodeCoordinate.h \
 NodeElevationGrid.h MeshBasedNode.h GeometryNode.h NodeImageTexture.h \
 KambiTextureCommonFields.h NodeTextureTransform.h GeometryNodeMacros.h \
 NodeGeoElevationGrid.h GeoNode.h NodeGeoOrigin.h Util.h
NodeColorInterpolator.o: NodeColorInterpolator.cpp stdafx.h config.h \
 mysnprintf.h NodeColorInterpolator.h Interpolator.h Node.h Array.h \
 List.h Vec3f.h Vec3x.h MyString.h StringArray.h FieldIndex.h Element.h \
 x3dFlags.h swt/include/WriteFlags.h NodeFlags.h MeshFlags.h \
 ac3dMaterialCallback.h NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h \
 ProtoMacros.h Proto.h Map.h NodeList.h InterfaceArray.h \
 DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h Arrays.h MySwap.h \
 Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h MFDouble.h \
 SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h MFRotation.h \
 MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h \
 MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h SFImage.h \
 SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h SFString.h \
 SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h \
 CommandList.h Command.h Scene.h URL.h DuneApp.h PreferencesApp.h \
 swt/include/swttypedef.h EcmaScriptApp.h StereoViewApp.h \
 swt/include/stereoview.h InputDeviceApp.h TransformMode.h OutputApp.h \
 RouteViewApp.h StartWithApp.h ExportNamesApp.h ModellingApp.h \
 RenderApp.h RenderState.h ProtoMap.h MFieldCommand.h
NodeCommonSurfaceShader.o: NodeCommonSurfaceShader.cpp stdafx.h config.h \
 mysnprintf.h NodeCommonSurfaceShader.h Node.h Array.h List.h Vec3f.h \
 Vec3x.h MyString.h StringArray.h FieldIndex.h Element.h x3dFlags.h \
 swt/include/WriteFlags.h NodeFlags.h MeshFlags.h ac3dMaterialCallback.h \
 NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h ProtoMacros.h Proto.h \
 Map.h NodeList.h InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h \
 MFBool.h FieldValue.h Arrays.h MySwap.h Types.h MFColor.h MFFloat.h \
 SFFloat.h MFColorRGBA.h MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h \
 MFMatrix4f.h MFNode.h MFRotation.h MFString.h MFTime.h SFTime.h \
 MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h \
 SFBool.h SFColor.h SFColorRGBA.h SFImage.h SFInt32.h SFMatrix3f.h \
 SFMatrix4f.h SFNode.h SFRotation.h SFString.h SFVec2d.h SFVec2f.h \
 Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h CommandList.h Command.h \
 NodeGroup.h StaticGroupNode.h GroupNode.h DuneApp.h URL.h \
 PreferencesApp.h swt/include/swttypedef.h EcmaScriptApp.h \
 StereoViewApp.h swt/include/stereoview.h InputDeviceApp.h \
 TransformMode.h OutputApp.h RouteViewApp.h StartWithApp.h \
 ExportNamesApp.h ModellingApp.h RenderApp.h
NodeComposedCubeMapTexture.o: NodeComposedCubeMapTexture.cpp stdafx.h \
 config.h mysnprintf.h NodeComposedCubeMapTexture.h Node.h Array.h List.h \
 Vec3f.h Vec3x.h MyString.h StringArray.h FieldIndex.h Element.h \
 x3dFlags.h swt/include/WriteFlags.h NodeFlags.h MeshFlags.h \
 ac3dMaterialCallback.h NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h \
 ProtoMacros.h Proto.h Map.h NodeList.h InterfaceArray.h \
 DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h Arrays.h MySwap.h \
 Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h MFDouble.h \
 SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h MFRotation.h \
 MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h \
 MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h SFImage.h \
 SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h SFString.h \
 SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h \
 CommandList.h Command.h DuneApp.h URL.h PreferencesApp.h \
 swt/include/swttypedef.h EcmaScriptApp.h StereoViewApp.h \
 swt/include/stereoview.h InputDeviceApp.h TransformMode.h OutputApp.h \
 RouteViewApp.h StartWithApp.h ExportNamesApp.h ModellingApp.h \
 RenderApp.h
NodeComposedShader.o: NodeComposedShader.cpp stdafx.h config.h \
 mysnprintf.h NodeComposedShader.h Node.h Array.h List.h Vec3f.h Vec3x.h \
 MyString.h StringArray.h FieldIndex.h Element.h x3dFlags.h \
 swt/include/WriteFlags.h NodeFlags.h MeshFlags.h ac3dMaterialCallback.h \
 NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h ProtoMacros.h Proto.h \
 Map.h NodeList.h InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h \
 MFBool.h FieldValue.h Arrays.h MySwap.h Types.h MFColor.h MFFloat.h \
 SFFloat.h MFColorRGBA.h MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h \
 MFMatrix4f.h MFNode.h MFRotation.h MFString.h MFTime.h SFTime.h \
 MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h \
 SFBool.h SFColor.h SFColorRGBA.h SFImage.h SFInt32.h SFMatrix3f.h \
 SFMatrix4f.h SFNode.h SFRotation.h SFString.h SFVec2d.h SFVec2f.h \
 Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h CommandList.h Command.h \
 DuneApp.h URL.h PreferencesApp.h swt/include/swttypedef.h \
 EcmaScriptApp.h StereoViewApp.h swt/include/stereoview.h \
 InputDeviceApp.h TransformMode.h OutputApp.h RouteViewApp.h \
 StartWithApp.h ExportNamesApp.h ModellingApp.h RenderApp.h
NodeComposedTexture3D.o: NodeComposedTexture3D.cpp stdafx.h config.h \
 mysnprintf.h NodeComposedTexture3D.h Node.h Array.h List.h Vec3f.h \
 Vec3x.h MyString.h StringArray.h FieldIndex.h Element.h x3dFlags.h \
 swt/include/WriteFlags.h NodeFlags.h MeshFlags.h ac3dMaterialCallback.h \
 NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h ProtoMacros.h Proto.h \
 Map.h NodeList.h InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h \
 MFBool.h FieldValue.h Arrays.h MySwap.h Types.h MFColor.h MFFloat.h \
 SFFloat.h MFColorRGBA.h MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h \
 MFMatrix4f.h MFNode.h MFRotation.h MFString.h MFTime.h SFTime.h \
 MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h \
 SFBool.h SFColor.h SFColorRGBA.h SFImage.h SFInt32.h SFMatrix3f.h \
 SFMatrix4f.h SFNode.h SFRotation.h SFString.h SFVec2d.h SFVec2f.h \
 Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h CommandList.h Command.h \
 KambiTextureCommonFields.h DuneApp.h URL.h PreferencesApp.h \
 swt/include/swttypedef.h EcmaScriptApp.h StereoViewApp.h \
 swt/include/stereoview.h InputDeviceApp.h TransformMode.h OutputApp.h \
 RouteViewApp.h StartWithApp.h ExportNamesApp.h ModellingApp.h \
 RenderApp.h
NodeComposedVolumeStyle.o: NodeComposedVolumeStyle.cpp stdafx.h config.h \
 mysnprintf.h NodeComposedVolumeStyle.h Node.h Array.h List.h Vec3f.h \
 Vec3x.h MyString.h StringArray.h FieldIndex.h Element.h x3dFlags.h \
 swt/include/WriteFlags.h NodeFlags.h MeshFlags.h ac3dMaterialCallback.h \
 NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h ProtoMacros.h Proto.h \
 Map.h NodeList.h InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h \
 MFBool.h FieldValue.h Arrays.h MySwap.h Types.h MFColor.h MFFloat.h \
 SFFloat.h MFColorRGBA.h MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h \
 MFMatrix4f.h MFNode.h MFRotation.h MFString.h MFTime.h SFTime.h \
 MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h \
 SFBool.h SFColor.h SFColorRGBA.h SFImage.h SFInt32.h SFMatrix3f.h \
 SFMatrix4f.h SFNode.h SFRotation.h SFString.h SFVec2d.h SFVec2f.h \
 Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h CommandList.h Command.h \
 DuneApp.h URL.h PreferencesApp.h swt/include/swttypedef.h \
 EcmaScriptApp.h StereoViewApp.h swt/include/stereoview.h \
 InputDeviceApp.h TransformMode.h OutputApp.h RouteViewApp.h \
 StartWithApp.h ExportNamesApp.h ModellingApp.h RenderApp.h
NodeCone.o: NodeCone.cpp stdafx.h config.h mysnprintf.h NodeCone.h \
 MeshBasedNode.h GeometryNode.h Node.h Array.h List.h Vec3f.h Vec3x.h \
 MyString.h StringArray.h FieldIndex.h Element.h x3dFlags.h \
 swt/include/WriteFlags.h NodeFlags.h MeshFlags.h ac3dMaterialCallback.h \
 NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h NodeAppearance.h \
 ProtoMacros.h Proto.h Map.h NodeList.h InterfaceArray.h \
 DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h Arrays.h MySwap.h \
 Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h MFDouble.h \
 SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h MFRotation.h \
 MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h \
 MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h SFImage.h \
 SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h SFString.h \
 SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h \
 CommandList.h Command.h DuneApp.h URL.h PreferencesApp.h \
 swt/include/swttypedef.h EcmaScriptApp.h StereoViewApp.h \
 swt/include/stereoview.h InputDeviceApp.h TransformMode.h OutputApp.h \
 RouteViewApp.h StartWithApp.h ExportNamesApp.h ModellingApp.h \
 RenderApp.h NodeMaterial.h swt/include/swt.h swt/include/keycodes.h \
 swt/include/swlanguages.h swt/include/swDebugf.h \
 swt/include/swttypedef.h NodeImageTexture.h KambiTextureCommonFields.h \
 NodeTextureTransform.h GeometryNodeMacros.h Scene.h RenderState.h \
 ProtoMap.h MyMesh.h NodeIndexedFaceSet.h NodeCoordinate.h Colored.h \
 NodeColor.h NodeColorRGBA.h ComposedGeometryMacros.h FaceData.h \
 NodeNormal.h NodeTextureCoordinate.h NodeTextureCoordinateGenerator.h \
 NodeMultiTextureCoordinate.h Util.h NurbsMakeRevolvedSurface.h \
 NodeNurbsSurface.h MeshMorphingNode.h Interpolator.h
NodeConeEmitter.o: NodeConeEmitter.cpp stdafx.h config.h mysnprintf.h \
 NodeConeEmitter.h Node.h Array.h List.h Vec3f.h Vec3x.h MyString.h \
 StringArray.h FieldIndex.h Element.h x3dFlags.h swt/include/WriteFlags.h \
 NodeFlags.h MeshFlags.h ac3dMaterialCallback.h NodeArray.h Stack.h \
 Matrix.h Vec3d.h Quaternion.h ProtoMacros.h Proto.h Map.h NodeList.h \
 InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h \
 Arrays.h MySwap.h Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h \
 MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h \
 MFRotation.h MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h \
 MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h \
 SFImage.h SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h \
 SFString.h SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h \
 SFVec4f.h CommandList.h Command.h EmitterNode.h NodeAppearance.h \
 DuneApp.h URL.h PreferencesApp.h swt/include/swttypedef.h \
 EcmaScriptApp.h StereoViewApp.h swt/include/stereoview.h \
 InputDeviceApp.h TransformMode.h OutputApp.h RouteViewApp.h \
 StartWithApp.h ExportNamesApp.h ModellingApp.h RenderApp.h \
 NodeMaterial.h swt/include/swt.h swt/include/keycodes.h \
 swt/include/swlanguages.h swt/include/swDebugf.h \
 swt/include/swttypedef.h NodeImageTexture.h KambiTextureCommonFields.h \
 NodeTextureTransform.h Scene.h RenderState.h ProtoMap.h
NodeContact.o: NodeContact.cpp stdafx.h config.h mysnprintf.h \
 NodeContact.h RigidBodyPhysicsNode.h Node.h Array.h List.h Vec3f.h \
 Vec3x.h MyString.h StringArray.h FieldIndex.h Element.h x3dFlags.h \
 swt/include/WriteFlags.h NodeFlags.h MeshFlags.h ac3dMaterialCallback.h \
 NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h ProtoMacros.h Proto.h \
 Map.h NodeList.h InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h \
 MFBool.h FieldValue.h Arrays.h MySwap.h Types.h MFColor.h MFFloat.h \
 SFFloat.h MFColorRGBA.h MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h \
 MFMatrix4f.h MFNode.h MFRotation.h MFString.h MFTime.h SFTime.h \
 MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h \
 SFBool.h SFColor.h SFColorRGBA.h SFImage.h SFInt32.h SFMatrix3f.h \
 SFMatrix4f.h SFNode.h SFRotation.h SFString.h SFVec2d.h SFVec2f.h \
 Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h CommandList.h Command.h \
 DuneApp.h URL.h PreferencesApp.h swt/include/swttypedef.h \
 EcmaScriptApp.h StereoViewApp.h swt/include/stereoview.h \
 InputDeviceApp.h TransformMode.h OutputApp.h RouteViewApp.h \
 StartWithApp.h ExportNamesApp.h ModellingApp.h RenderApp.h
NodeContour2D.o: NodeContour2D.cpp stdafx.h config.h mysnprintf.h \
 NodeContour2D.h Node.h Array.h List.h Vec3f.h Vec3x.h MyString.h \
 StringArray.h FieldIndex.h Element.h x3dFlags.h swt/include/WriteFlags.h \
 NodeFlags.h MeshFlags.h ac3dMaterialCallback.h NodeArray.h Stack.h \
 Matrix.h Vec3d.h Quaternion.h ProtoMacros.h Proto.h Map.h NodeList.h \
 InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h \
 Arrays.h MySwap.h Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h \
 MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h \
 MFRotation.h MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h \
 MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h \
 SFImage.h SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h \
 SFString.h SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h \
 SFVec4f.h CommandList.h Command.h Scene.h URL.h DuneApp.h \
 PreferencesApp.h swt/include/swttypedef.h EcmaScriptApp.h \
 StereoViewApp.h swt/include/stereoview.h InputDeviceApp.h \
 TransformMode.h OutputApp.h RouteViewApp.h StartWithApp.h \
 ExportNamesApp.h ModellingApp.h RenderApp.h RenderState.h ProtoMap.h \
 NodePolyline2D.h ChainBasedNode.h GeometryNode.h NodeAppearance.h \
 NodeMaterial.h swt/include/swt.h swt/include/keycodes.h \
 swt/include/swlanguages.h swt/include/swDebugf.h \
 swt/include/swttypedef.h NodeImageTexture.h KambiTextureCommonFields.h \
 NodeTextureTransform.h GeometryNodeMacros.h NodeContourPolyline2D.h \
 MoveCommand.h
NodeContourPolyline2D.o: NodeContourPolyline2D.cpp stdafx.h config.h \
 mysnprintf.h NodeContourPolyline2D.h ChainBasedNode.h GeometryNode.h \
 Node.h Array.h List.h Vec3f.h Vec3x.h MyString.h StringArray.h \
 FieldIndex.h Element.h x3dFlags.h swt/include/WriteFlags.h NodeFlags.h \
 MeshFlags.h ac3dMaterialCallback.h NodeArray.h Stack.h Matrix.h Vec3d.h \
 Quaternion.h NodeAppearance.h ProtoMacros.h Proto.h Map.h NodeList.h \
 InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h \
 Arrays.h MySwap.h Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h \
 MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h \
 MFRotation.h MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h \
 MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h \
 SFImage.h SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h \
 SFString.h SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h \
 SFVec4f.h CommandList.h Command.h DuneApp.h URL.h PreferencesApp.h \
 swt/include/swttypedef.h EcmaScriptApp.h StereoViewApp.h \
 swt/include/stereoview.h InputDeviceApp.h TransformMode.h OutputApp.h \
 RouteViewApp.h StartWithApp.h ExportNamesApp.h ModellingApp.h \
 RenderApp.h NodeMaterial.h swt/include/swt.h swt/include/keycodes.h \
 swt/include/swlanguages.h swt/include/swDebugf.h \
 swt/include/swttypedef.h NodeImageTexture.h KambiTextureCommonFields.h \
 NodeTextureTransform.h GeometryNodeMacros.h ExposedField.h Field.h \
 Scene.h RenderState.h ProtoMap.h Util.h resource.h
NodeCoordinate.o: NodeCoordinate.cpp stdafx.h config.h mysnprintf.h \
 NodeCoordinate.h Node.h Array.h List.h Vec3f.h Vec3x.h MyString.h \
 StringArray.h FieldIndex.h Element.h x3dFlags.h swt/include/WriteFlags.h \
 NodeFlags.h MeshFlags.h ac3dMaterialCallback.h NodeArray.h Stack.h \
 Matrix.h Vec3d.h Quaternion.h ProtoMacros.h Proto.h Map.h NodeList.h \
 InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h \
 Arrays.h MySwap.h Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h \
 MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h \
 MFRotation.h MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h \
 MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h \
 SFImage.h SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h \
 SFString.h SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h \
 SFVec4f.h CommandList.h Command.h ExposedField.h Field.h RenderState.h \
 swt/include/swttypedef.h DuneApp.h URL.h PreferencesApp.h \
 EcmaScriptApp.h StereoViewApp.h swt/include/stereoview.h \
 InputDeviceApp.h TransformMode.h OutputApp.h RouteViewApp.h \
 StartWithApp.h ExportNamesApp.h ModellingApp.h RenderApp.h Util.h \
 Scene.h ProtoMap.h resource.h FaceData.h NodeIndexedFaceSet.h \
 MeshBasedNode.h GeometryNode.h NodeAppearance.h NodeMaterial.h \
 swt/include/swt.h swt/include/keycodes.h swt/include/swlanguages.h \
 swt/include/swDebugf.h swt/include/swttypedef.h NodeImageTexture.h \
 KambiTextureCommonFields.h NodeTextureTransform.h GeometryNodeMacros.h \
 Colored.h NodeColor.h NodeColorRGBA.h ComposedGeometryMacros.h \
 NodeIndexedLineSet.h NodePointSet.h NodeLineSet.h NodeTriangleSet.h \
 MyMesh.h NodeNormal.h NodeTextureCoordinate.h \
 NodeTextureCoordinateGenerator.h NodeMultiTextureCoordinate.h \
 NodeTriangleFanSet.h NodeTriangleStripSet.h NodeIndexedTriangleSet.h \
 IndexedTriangleSetNode.h NodeIndexedTriangleFanSet.h \
 NodeIndexedTriangleStripSet.h NodeQuadSet.h NodeIndexedQuadSet.h \
 NodeNurbsGroup.h NodeGroup.h StaticGroupNode.h GroupNode.h \
 NodeNurbsSurface.h MeshMorphingNode.h Interpolator.h NodeNurbsCurve.h \
 NurbsCurve.h ChainBasedNode.h NodeNurbsTrimmedSurface.h \
 NodeHAnimHumanoid.h TransformNode.h BasicTransformNode.h \
 NodeHAnimJoint.h NodeTransform.h
NodeCoordinateChaser.o: NodeCoordinateChaser.cpp stdafx.h config.h \
 mysnprintf.h NodeCoordinateChaser.h Node.h Array.h List.h Vec3f.h \
 Vec3x.h MyString.h StringArray.h FieldIndex.h Element.h x3dFlags.h \
 swt/include/WriteFlags.h NodeFlags.h MeshFlags.h ac3dMaterialCallback.h \
 NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h ProtoMacros.h Proto.h \
 Map.h NodeList.h InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h \
 MFBool.h FieldValue.h Arrays.h MySwap.h Types.h MFColor.h MFFloat.h \
 SFFloat.h MFColorRGBA.h MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h \
 MFMatrix4f.h MFNode.h MFRotation.h MFString.h MFTime.h SFTime.h \
 MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h \
 SFBool.h SFColor.h SFColorRGBA.h SFImage.h SFInt32.h SFMatrix3f.h \
 SFMatrix4f.h SFNode.h SFRotation.h SFString.h SFVec2d.h SFVec2f.h \
 Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h CommandList.h Command.h \
 ChaserNode.h DuneApp.h URL.h PreferencesApp.h swt/include/swttypedef.h \
 EcmaScriptApp.h StereoViewApp.h swt/include/stereoview.h \
 InputDeviceApp.h TransformMode.h OutputApp.h RouteViewApp.h \
 StartWithApp.h ExportNamesApp.h ModellingApp.h RenderApp.h \
 swt/include/swt.h swt/include/keycodes.h swt/include/swlanguages.h \
 swt/include/swDebugf.h swt/include/swttypedef.h
NodeCoordinateDamper.o: NodeCoordinateDamper.cpp stdafx.h config.h \
 mysnprintf.h NodeCoordinateDamper.h Node.h Array.h List.h Vec3f.h \
 Vec3x.h MyString.h StringArray.h FieldIndex.h Element.h x3dFlags.h \
 swt/include/WriteFlags.h NodeFlags.h MeshFlags.h ac3dMaterialCallback.h \
 NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h ProtoMacros.h Proto.h \
 Map.h NodeList.h InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h \
 MFBool.h FieldValue.h Arrays.h MySwap.h Types.h MFColor.h MFFloat.h \
 SFFloat.h MFColorRGBA.h MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h \
 MFMatrix4f.h MFNode.h MFRotation.h MFString.h MFTime.h SFTime.h \
 MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h \
 SFBool.h SFColor.h SFColorRGBA.h SFImage.h SFInt32.h SFMatrix3f.h \
 SFMatrix4f.h SFNode.h SFRotation.h SFString.h SFVec2d.h SFVec2f.h \
 Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h CommandList.h Command.h \
 DamperNode.h DuneApp.h URL.h PreferencesApp.h swt/include/swttypedef.h \
 EcmaScriptApp.h StereoViewApp.h swt/include/stereoview.h \
 InputDeviceApp.h TransformMode.h OutputApp.h RouteViewApp.h \
 StartWithApp.h ExportNamesApp.h ModellingApp.h RenderApp.h
NodeCoordinateDeformer.o: NodeCoordinateDeformer.cpp stdafx.h config.h \
 mysnprintf.h NodeCoordinateDeformer.h Node.h Array.h List.h Vec3f.h \
 Vec3x.h MyString.h StringArray.h FieldIndex.h Element.h x3dFlags.h \
 swt/include/WriteFlags.h NodeFlags.h MeshFlags.h ac3dMaterialCallback.h \
 NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h ProtoMacros.h Proto.h \
 Map.h NodeList.h InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h \
 MFBool.h FieldValue.h Arrays.h MySwap.h Types.h MFColor.h MFFloat.h \
 SFFloat.h MFColorRGBA.h MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h \
 MFMatrix4f.h MFNode.h MFRotation.h MFString.h MFTime.h SFTime.h \
 MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h \
 SFBool.h SFColor.h SFColorRGBA.h SFImage.h SFInt32.h SFMatrix3f.h \
 SFMatrix4f.h SFNode.h SFRotation.h SFString.h SFVec2d.h SFVec2f.h \
 Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h CommandList.h Command.h \
 Scene.h URL.h DuneApp.h PreferencesApp.h swt/include/swttypedef.h \
 EcmaScriptApp.h StereoViewApp.h swt/include/stereoview.h \
 InputDeviceApp.h TransformMode.h OutputApp.h RouteViewApp.h \
 StartWithApp.h ExportNamesApp.h ModellingApp.h RenderApp.h RenderState.h \
 ProtoMap.h
NodeCoordinateDouble.o: NodeCoordinateDouble.cpp stdafx.h config.h \
 mysnprintf.h NodeCoordinateDouble.h Node.h Array.h List.h Vec3f.h \
 Vec3x.h MyString.h StringArray.h FieldIndex.h Element.h x3dFlags.h \
 swt/include/WriteFlags.h NodeFlags.h MeshFlags.h ac3dMaterialCallback.h \
 NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h ProtoMacros.h Proto.h \
 Map.h NodeList.h InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h \
 MFBool.h FieldValue.h Arrays.h MySwap.h Types.h MFColor.h MFFloat.h \
 SFFloat.h MFColorRGBA.h MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h \
 MFMatrix4f.h MFNode.h MFRotation.h MFString.h MFTime.h SFTime.h \
 MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h \
 SFBool.h SFColor.h SFColorRGBA.h SFImage.h SFInt32.h SFMatrix3f.h \
 SFMatrix4f.h SFNode.h SFRotation.h SFString.h SFVec2d.h SFVec2f.h \
 Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h CommandList.h Command.h \
 ExposedField.h Field.h RenderState.h swt/include/swttypedef.h DuneApp.h \
 URL.h PreferencesApp.h EcmaScriptApp.h StereoViewApp.h \
 swt/include/stereoview.h InputDeviceApp.h TransformMode.h OutputApp.h \
 RouteViewApp.h StartWithApp.h ExportNamesApp.h ModellingApp.h \
 RenderApp.h Util.h Scene.h ProtoMap.h NodeIndexedFaceSet.h \
 MeshBasedNode.h GeometryNode.h NodeAppearance.h NodeMaterial.h \
 swt/include/swt.h swt/include/keycodes.h swt/include/swlanguages.h \
 swt/include/swDebugf.h swt/include/swttypedef.h NodeImageTexture.h \
 KambiTextureCommonFields.h NodeTextureTransform.h GeometryNodeMacros.h \
 NodeCoordinate.h Colored.h NodeColor.h NodeColorRGBA.h \
 ComposedGeometryMacros.h NodeIndexedLineSet.h NodePointSet.h \
 NodeLineSet.h NodeTriangleSet.h MyMesh.h FaceData.h NodeNormal.h \
 NodeTextureCoordinate.h NodeTextureCoordinateGenerator.h \
 NodeMultiTextureCoordinate.h NodeNurbsGroup.h NodeGroup.h \
 StaticGroupNode.h GroupNode.h
NodeCoordinateInterpolator.o: NodeCoordinateInterpolator.cpp stdafx.h \
 config.h mysnprintf.h NodeCoordinateInterpolator.h Interpolator.h Node.h \
 Array.h List.h Vec3f.h Vec3x.h MyString.h StringArray.h FieldIndex.h \
 Element.h x3dFlags.h swt/include/WriteFlags.h NodeFlags.h MeshFlags.h \
 ac3dMaterialCallback.h NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h \
 ProtoMacros.h Proto.h Map.h NodeList.h InterfaceArray.h \
 DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h Arrays.h MySwap.h \
 Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h MFDouble.h \
 SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h MFRotation.h \
 MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h \
 MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h SFImage.h \
 SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h SFString.h \
 SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h \
 CommandList.h Command.h Scene.h URL.h DuneApp.h PreferencesApp.h \
 swt/include/swttypedef.h EcmaScriptApp.h StereoViewApp.h \
 swt/include/stereoview.h InputDeviceApp.h TransformMode.h OutputApp.h \
 RouteViewApp.h StartWithApp.h ExportNamesApp.h ModellingApp.h \
 RenderApp.h RenderState.h ProtoMap.h
NodeCoordinateInterpolator2D.o: NodeCoordinateInterpolator2D.cpp stdafx.h \
 config.h mysnprintf.h NodeCoordinateInterpolator2D.h Interpolator.h \
 Node.h Array.h List.h Vec3f.h Vec3x.h MyString.h StringArray.h \
 FieldIndex.h Element.h x3dFlags.h swt/include/WriteFlags.h NodeFlags.h \
 MeshFlags.h ac3dMaterialCallback.h NodeArray.h Stack.h Matrix.h Vec3d.h \
 Quaternion.h ProtoMacros.h Proto.h Map.h NodeList.h InterfaceArray.h \
 DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h Arrays.h MySwap.h \
 Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h MFDouble.h \
 SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h MFRotation.h \
 MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h \
 MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h SFImage.h \
 SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h SFString.h \
 SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h \
 CommandList.h Command.h Scene.h URL.h DuneApp.h PreferencesApp.h \
 swt/include/swttypedef.h EcmaScriptApp.h StereoViewApp.h \
 swt/include/stereoview.h InputDeviceApp.h TransformMode.h OutputApp.h \
 RouteViewApp.h StartWithApp.h ExportNamesApp.h ModellingApp.h \
 RenderApp.h RenderState.h ProtoMap.h
NodeCylinder.o: NodeCylinder.cpp stdafx.h config.h mysnprintf.h \
 NodeCylinder.h MeshBasedNode.h GeometryNode.h Node.h Array.h List.h \
 Vec3f.h Vec3x.h MyString.h StringArray.h FieldIndex.h Element.h \
 x3dFlags.h swt/include/WriteFlags.h NodeFlags.h MeshFlags.h \
 ac3dMaterialCallback.h NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h \
 NodeAppearance.h ProtoMacros.h Proto.h Map.h NodeList.h InterfaceArray.h \
 DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h Arrays.h MySwap.h \
 Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h MFDouble.h \
 SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h MFRotation.h \
 MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h \
 MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h SFImage.h \
 SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h SFString.h \
 SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h \
 CommandList.h Command.h DuneApp.h URL.h PreferencesApp.h \
 swt/include/swttypedef.h EcmaScriptApp.h StereoViewApp.h \
 swt/include/stereoview.h InputDeviceApp.h TransformMode.h OutputApp.h \
 RouteViewApp.h StartWithApp.h ExportNamesApp.h ModellingApp.h \
 RenderApp.h NodeMaterial.h swt/include/swt.h swt/include/keycodes.h \
 swt/include/swlanguages.h swt/include/swDebugf.h \
 swt/include/swttypedef.h NodeImageTexture.h KambiTextureCommonFields.h \
 NodeTextureTransform.h GeometryNodeMacros.h Scene.h RenderState.h \
 ProtoMap.h MyMesh.h NodeIndexedFaceSet.h NodeCoordinate.h Colored.h \
 NodeColor.h NodeColorRGBA.h ComposedGeometryMacros.h FaceData.h \
 NodeNormal.h NodeTextureCoordinate.h NodeTextureCoordinateGenerator.h \
 NodeMultiTextureCoordinate.h Util.h NurbsArc.h \
 NurbsMakeRevolvedSurface.h NodeNurbsSurface.h MeshMorphingNode.h \
 Interpolator.h
NodeCylinderSensor.o: NodeCylinderSensor.cpp stdafx.h config.h \
 mysnprintf.h NodeCylinderSensor.h Node.h Array.h List.h Vec3f.h Vec3x.h \
 MyString.h StringArray.h FieldIndex.h Element.h x3dFlags.h \
 swt/include/WriteFlags.h NodeFlags.h MeshFlags.h ac3dMaterialCallback.h \
 NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h ProtoMacros.h Proto.h \
 Map.h NodeList.h InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h \
 MFBool.h FieldValue.h Arrays.h MySwap.h Types.h MFColor.h MFFloat.h \
 SFFloat.h MFColorRGBA.h MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h \
 MFMatrix4f.h MFNode.h MFRotation.h MFString.h MFTime.h SFTime.h \
 MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h \
 SFBool.h SFColor.h SFColorRGBA.h SFImage.h SFInt32.h SFMatrix3f.h \
 SFMatrix4f.h SFNode.h SFRotation.h SFString.h SFVec2d.h SFVec2f.h \
 Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h CommandList.h Command.h \
 Field.h Scene.h URL.h DuneApp.h PreferencesApp.h \
 swt/include/swttypedef.h EcmaScriptApp.h StereoViewApp.h \
 swt/include/stereoview.h InputDeviceApp.h TransformMode.h OutputApp.h \
 RouteViewApp.h StartWithApp.h ExportNamesApp.h ModellingApp.h \
 RenderApp.h RenderState.h ProtoMap.h
NodeDISEntityManager.o: NodeDISEntityManager.cpp stdafx.h config.h \
 mysnprintf.h NodeDISEntityManager.h Node.h Array.h List.h Vec3f.h \
 Vec3x.h MyString.h StringArray.h FieldIndex.h Element.h x3dFlags.h \
 swt/include/WriteFlags.h NodeFlags.h MeshFlags.h ac3dMaterialCallback.h \
 NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h ProtoMacros.h Proto.h \
 Map.h NodeList.h InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h \
 MFBool.h FieldValue.h Arrays.h MySwap.h Types.h MFColor.h MFFloat.h \
 SFFloat.h MFColorRGBA.h MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h \
 MFMatrix4f.h MFNode.h MFRotation.h MFString.h MFTime.h SFTime.h \
 MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h \
 SFBool.h SFColor.h SFColorRGBA.h SFImage.h SFInt32.h SFMatrix3f.h \
 SFMatrix4f.h SFNode.h SFRotation.h SFString.h SFVec2d.h SFVec2f.h \
 Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h CommandList.h Command.h \
 DuneApp.h URL.h PreferencesApp.h swt/include/swttypedef.h \
 EcmaScriptApp.h StereoViewApp.h swt/include/stereoview.h \
 InputDeviceApp.h TransformMode.h OutputApp.h RouteViewApp.h \
 StartWithApp.h ExportNamesApp.h ModellingApp.h RenderApp.h
NodeDISEntityTypeMapping.o: NodeDISEntityTypeMapping.cpp stdafx.h \
 config.h mysnprintf.h NodeDISEntityTypeMapping.h Node.h Array.h List.h \
 Vec3f.h Vec3x.h MyString.h StringArray.h FieldIndex.h Element.h \
 x3dFlags.h swt/include/WriteFlags.h NodeFlags.h MeshFlags.h \
 ac3dMaterialCallback.h NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h \
 ProtoMacros.h Proto.h Map.h NodeList.h InterfaceArray.h \
 DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h Arrays.h MySwap.h \
 Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h MFDouble.h \
 SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h MFRotation.h \
 MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h \
 MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h SFImage.h \
 SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h SFString.h \
 SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h \
 CommandList.h Command.h Scene.h URL.h DuneApp.h PreferencesApp.h \
 swt/include/swttypedef.h EcmaScriptApp.h StereoViewApp.h \
 swt/include/stereoview.h InputDeviceApp.h TransformMode.h OutputApp.h \
 RouteViewApp.h StartWithApp.h ExportNamesApp.h ModellingApp.h \
 RenderApp.h RenderState.h ProtoMap.h
NodeDirectionalLight.o: NodeDirectionalLight.cpp stdafx.h config.h \
 mysnprintf.h NodeDirectionalLight.h Node.h Array.h List.h Vec3f.h \
 Vec3x.h MyString.h StringArray.h FieldIndex.h Element.h x3dFlags.h \
 swt/include/WriteFlags.h NodeFlags.h MeshFlags.h ac3dMaterialCallback.h \
 NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h ProtoMacros.h Proto.h \
 Map.h NodeList.h InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h \
 MFBool.h FieldValue.h Arrays.h MySwap.h Types.h MFColor.h MFFloat.h \
 SFFloat.h MFColorRGBA.h MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h \
 MFMatrix4f.h MFNode.h MFRotation.h MFString.h MFTime.h SFTime.h \
 MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h \
 SFBool.h SFColor.h SFColorRGBA.h SFImage.h SFInt32.h SFMatrix3f.h \
 SFMatrix4f.h SFNode.h SFRotation.h SFString.h SFVec2d.h SFVec2f.h \
 Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h CommandList.h Command.h \
 swt/include/swt.h swt/include/keycodes.h swt/include/swlanguages.h \
 swt/include/swDebugf.h swt/include/swttypedef.h KambiLightCommonFields.h \
 X3domLightCommonFields.h Scene.h URL.h DuneApp.h PreferencesApp.h \
 swt/include/swttypedef.h EcmaScriptApp.h StereoViewApp.h \
 swt/include/stereoview.h InputDeviceApp.h TransformMode.h OutputApp.h \
 RouteViewApp.h StartWithApp.h ExportNamesApp.h ModellingApp.h \
 RenderApp.h RenderState.h ProtoMap.h ExposedField.h resource.h
NodeDisk2D.o: NodeDisk2D.cpp stdafx.h config.h mysnprintf.h NodeDisk2D.h \
 MeshBasedNode.h GeometryNode.h Node.h Array.h List.h Vec3f.h Vec3x.h \
 MyString.h StringArray.h FieldIndex.h Element.h x3dFlags.h \
 swt/include/WriteFlags.h NodeFlags.h MeshFlags.h ac3dMaterialCallback.h \
 NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h NodeAppearance.h \
 ProtoMacros.h Proto.h Map.h NodeList.h InterfaceArray.h \
 DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h Arrays.h MySwap.h \
 Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h MFDouble.h \
 SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h MFRotation.h \
 MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h \
 MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h SFImage.h \
 SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h SFString.h \
 SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h \
 CommandList.h Command.h DuneApp.h URL.h PreferencesApp.h \
 swt/include/swttypedef.h EcmaScriptApp.h StereoViewApp.h \
 swt/include/stereoview.h InputDeviceApp.h TransformMode.h OutputApp.h \
 RouteViewApp.h StartWithApp.h ExportNamesApp.h ModellingApp.h \
 RenderApp.h NodeMaterial.h swt/include/swt.h swt/include/keycodes.h \
 swt/include/swlanguages.h swt/include/swDebugf.h \
 swt/include/swttypedef.h NodeImageTexture.h KambiTextureCommonFields.h \
 NodeTextureTransform.h GeometryNodeMacros.h MyMesh.h \
 NodeIndexedFaceSet.h NodeCoordinate.h Colored.h NodeColor.h \
 NodeColorRGBA.h ComposedGeometryMacros.h FaceData.h NodeNormal.h \
 NodeTextureCoordinate.h NodeTextureCoordinateGenerator.h \
 NodeMultiTextureCoordinate.h Scene.h RenderState.h ProtoMap.h Util.h
NodeDoubleAxisHingeJoint.o: NodeDoubleAxisHingeJoint.cpp stdafx.h \
 config.h mysnprintf.h NodeDoubleAxisHingeJoint.h AnchoredJointNode.h \
 SFMFTypes.h MFBool.h Array.h FieldValue.h MyString.h StringArray.h \
 Element.h x3dFlags.h swt/include/WriteFlags.h Arrays.h MySwap.h Types.h \
 MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h MFDouble.h SFDouble.h \
 MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h Node.h List.h Vec3f.h \
 Vec3x.h FieldIndex.h NodeFlags.h MeshFlags.h ac3dMaterialCallback.h \
 NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h NodeList.h \
 MFRotation.h MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h \
 MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h \
 SFImage.h SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h \
 SFString.h SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h \
 SFVec4f.h X3DRigidJointNode.h RigidBodyPhysicsNode.h ProtoMacros.h \
 Proto.h Map.h InterfaceArray.h DynamicFieldsNode.h CommandList.h \
 Command.h Field.h DuneApp.h URL.h PreferencesApp.h \
 swt/include/swttypedef.h EcmaScriptApp.h StereoViewApp.h \
 swt/include/stereoview.h InputDeviceApp.h TransformMode.h OutputApp.h \
 RouteViewApp.h StartWithApp.h ExportNamesApp.h ModellingApp.h \
 RenderApp.h Scene.h RenderState.h ProtoMap.h
NodeEaseInEaseOut.o: NodeEaseInEaseOut.cpp stdafx.h config.h mysnprintf.h \
 NodeEaseInEaseOut.h Node.h Array.h List.h Vec3f.h Vec3x.h MyString.h \
 StringArray.h FieldIndex.h Element.h x3dFlags.h swt/include/WriteFlags.h \
 NodeFlags.h MeshFlags.h ac3dMaterialCallback.h NodeArray.h Stack.h \
 Matrix.h Vec3d.h Quaternion.h ProtoMacros.h Proto.h Map.h NodeList.h \
 InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h \
 Arrays.h MySwap.h Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h \
 MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h \
 MFRotation.h MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h \
 MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h \
 SFImage.h SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h \
 SFString.h SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h \
 SFVec4f.h CommandList.h Command.h DuneApp.h URL.h PreferencesApp.h \
 swt/include/swttypedef.h EcmaScriptApp.h StereoViewApp.h \
 swt/include/stereoview.h InputDeviceApp.h TransformMode.h OutputApp.h \
 RouteViewApp.h StartWithApp.h ExportNamesApp.h ModellingApp.h \
 RenderApp.h
NodeElevationGrid.o: NodeElevationGrid.cpp stdafx.h config.h mysnprintf.h \
 NodeElevationGrid.h MeshBasedNode.h GeometryNode.h Node.h Array.h List.h \
 Vec3f.h Vec3x.h MyString.h StringArray.h FieldIndex.h Element.h \
 x3dFlags.h swt/include/WriteFlags.h NodeFlags.h MeshFlags.h \
 ac3dMaterialCallback.h NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h \
 NodeAppearance.h ProtoMacros.h Proto.h Map.h NodeList.h InterfaceArray.h \
 DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h Arrays.h MySwap.h \
 Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h MFDouble.h \
 SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h MFRotation.h \
 MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h \
 MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h SFImage.h \
 SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h SFString.h \
 SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h \
 CommandList.h Command.h DuneApp.h URL.h PreferencesApp.h \
 swt/include/swttypedef.h EcmaScriptApp.h StereoViewApp.h \
 swt/include/stereoview.h InputDeviceApp.h TransformMode.h OutputApp.h \
 RouteViewApp.h StartWithApp.h ExportNamesApp.h ModellingApp.h \
 RenderApp.h NodeMaterial.h swt/include/swt.h swt/include/keycodes.h \
 swt/include/swlanguages.h swt/include/swDebugf.h \
 swt/include/swttypedef.h NodeImageTexture.h KambiTextureCommonFields.h \
 NodeTextureTransform.h GeometryNodeMacros.h Scene.h RenderState.h \
 ProtoMap.h MyMesh.h NodeIndexedFaceSet.h NodeCoordinate.h Colored.h \
 NodeColor.h NodeColorRGBA.h ComposedGeometryMacros.h FaceData.h \
 NodeNormal.h NodeTextureCoordinate.h NodeTextureCoordinateGenerator.h \
 NodeMultiTextureCoordinate.h Util.h Field.h resource.h
NodeEspduTransform.o: NodeEspduTransform.cpp stdafx.h config.h \
 mysnprintf.h NodeEspduTransform.h Node.h Array.h List.h Vec3f.h Vec3x.h \
 MyString.h StringArray.h FieldIndex.h Element.h x3dFlags.h \
 swt/include/WriteFlags.h NodeFlags.h MeshFlags.h ac3dMaterialCallback.h \
 NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h ProtoMacros.h Proto.h \
 Map.h NodeList.h InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h \
 MFBool.h FieldValue.h Arrays.h MySwap.h Types.h MFColor.h MFFloat.h \
 SFFloat.h MFColorRGBA.h MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h \
 MFMatrix4f.h MFNode.h MFRotation.h MFString.h MFTime.h SFTime.h \
 MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h \
 SFBool.h SFColor.h SFColorRGBA.h SFImage.h SFInt32.h SFMatrix3f.h \
 SFMatrix4f.h SFNode.h SFRotation.h SFString.h SFVec2d.h SFVec2f.h \
 Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h CommandList.h Command.h \
 DISCommonFields.h NodeTransform.h swt/include/swt.h \
 swt/include/keycodes.h swt/include/swlanguages.h swt/include/swDebugf.h \
 swt/include/swttypedef.h TransformNode.h BasicTransformNode.h
NodeExplosionEmitter.o: NodeExplosionEmitter.cpp stdafx.h config.h \
 mysnprintf.h NodeExplosionEmitter.h Node.h Array.h List.h Vec3f.h \
 Vec3x.h MyString.h StringArray.h FieldIndex.h Element.h x3dFlags.h \
 swt/include/WriteFlags.h NodeFlags.h MeshFlags.h ac3dMaterialCallback.h \
 NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h ProtoMacros.h Proto.h \
 Map.h NodeList.h InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h \
 MFBool.h FieldValue.h Arrays.h MySwap.h Types.h MFColor.h MFFloat.h \
 SFFloat.h MFColorRGBA.h MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h \
 MFMatrix4f.h MFNode.h MFRotation.h MFString.h MFTime.h SFTime.h \
 MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h \
 SFBool.h SFColor.h SFColorRGBA.h SFImage.h SFInt32.h SFMatrix3f.h \
 SFMatrix4f.h SFNode.h SFRotation.h SFString.h SFVec2d.h SFVec2f.h \
 Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h CommandList.h Command.h \
 EmitterNode.h NodeAppearance.h DuneApp.h URL.h PreferencesApp.h \
 swt/include/swttypedef.h EcmaScriptApp.h StereoViewApp.h \
 swt/include/stereoview.h InputDeviceApp.h TransformMode.h OutputApp.h \
 RouteViewApp.h StartWithApp.h ExportNamesApp.h ModellingApp.h \
 RenderApp.h NodeMaterial.h swt/include/swt.h swt/include/keycodes.h \
 swt/include/swlanguages.h swt/include/swDebugf.h \
 swt/include/swttypedef.h NodeImageTexture.h KambiTextureCommonFields.h \
 NodeTextureTransform.h
NodeExtrusion.o: NodeExtrusion.cpp stdafx.h config.h mysnprintf.h \
 NodeExtrusion.h Node.h Array.h List.h Vec3f.h Vec3x.h MyString.h \
 StringArray.h FieldIndex.h Element.h x3dFlags.h swt/include/WriteFlags.h \
 NodeFlags.h MeshFlags.h ac3dMaterialCallback.h NodeArray.h Stack.h \
 Matrix.h Vec3d.h Quaternion.h ProtoMacros.h Proto.h Map.h NodeList.h \
 InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h \
 Arrays.h MySwap.h Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h \
 MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h \
 MFRotation.h MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h \
 MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h \
 SFImage.h SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h \
 SFString.h SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h \
 SFVec4f.h CommandList.h Command.h MeshBasedNode.h GeometryNode.h \
 NodeAppearance.h DuneApp.h URL.h PreferencesApp.h \
 swt/include/swttypedef.h EcmaScriptApp.h StereoViewApp.h \
 swt/include/stereoview.h InputDeviceApp.h TransformMode.h OutputApp.h \
 RouteViewApp.h StartWithApp.h ExportNamesApp.h ModellingApp.h \
 RenderApp.h NodeMaterial.h swt/include/swt.h swt/include/keycodes.h \
 swt/include/swlanguages.h swt/include/swDebugf.h \
 swt/include/swttypedef.h NodeImageTexture.h KambiTextureCommonFields.h \
 NodeTextureTransform.h GeometryNodeMacros.h MyMesh.h \
 NodeIndexedFaceSet.h NodeCoordinate.h Colored.h NodeColor.h \
 NodeColorRGBA.h ComposedGeometryMacros.h FaceData.h NodeNormal.h \
 NodeTextureCoordinate.h NodeTextureCoordinateGenerator.h \
 NodeMultiTextureCoordinate.h Scene.h RenderState.h ProtoMap.h Util.h \
 Field.h resource.h
NodeFillProperties.o: NodeFillProperties.cpp stdafx.h config.h \
 mysnprintf.h NodeFillProperties.h Node.h Array.h List.h Vec3f.h Vec3x.h \
 MyString.h StringArray.h FieldIndex.h Element.h x3dFlags.h \
 swt/include/WriteFlags.h NodeFlags.h MeshFlags.h ac3dMaterialCallback.h \
 NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h ProtoMacros.h Proto.h \
 Map.h NodeList.h InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h \
 MFBool.h FieldValue.h Arrays.h MySwap.h Types.h MFColor.h MFFloat.h \
 SFFloat.h MFColorRGBA.h MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h \
 MFMatrix4f.h MFNode.h MFRotation.h MFString.h MFTime.h SFTime.h \
 MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h \
 SFBool.h SFColor.h SFColorRGBA.h SFImage.h SFInt32.h SFMatrix3f.h \
 SFMatrix4f.h SFNode.h SFRotation.h SFString.h SFVec2d.h SFVec2f.h \
 Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h CommandList.h Command.h \
 DuneApp.h URL.h PreferencesApp.h swt/include/swttypedef.h \
 EcmaScriptApp.h StereoViewApp.h swt/include/stereoview.h \
 InputDeviceApp.h TransformMode.h OutputApp.h RouteViewApp.h \
 StartWithApp.h ExportNamesApp.h ModellingApp.h RenderApp.h
NodeFloatVertexAttribute.o: NodeFloatVertexAttribute.cpp stdafx.h \
 config.h mysnprintf.h NodeFloatVertexAttribute.h Node.h Array.h List.h \
 Vec3f.h Vec3x.h MyString.h StringArray.h FieldIndex.h Element.h \
 x3dFlags.h swt/include/WriteFlags.h NodeFlags.h MeshFlags.h \
 ac3dMaterialCallback.h NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h \
 ProtoMacros.h Proto.h Map.h NodeList.h InterfaceArray.h \
 DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h Arrays.h MySwap.h \
 Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h MFDouble.h \
 SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h MFRotation.h \
 MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h \
 MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h SFImage.h \
 SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h SFString.h \
 SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h \
 CommandList.h Command.h DuneApp.h URL.h PreferencesApp.h \
 swt/include/swttypedef.h EcmaScriptApp.h StereoViewApp.h \
 swt/include/stereoview.h InputDeviceApp.h TransformMode.h OutputApp.h \
 RouteViewApp.h StartWithApp.h ExportNamesApp.h ModellingApp.h \
 RenderApp.h
NodeFog.o: NodeFog.cpp stdafx.h config.h mysnprintf.h NodeFog.h Node.h \
 Array.h List.h Vec3f.h Vec3x.h MyString.h StringArray.h FieldIndex.h \
 Element.h x3dFlags.h swt/include/WriteFlags.h NodeFlags.h MeshFlags.h \
 ac3dMaterialCallback.h NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h \
 ProtoMacros.h Proto.h Map.h NodeList.h InterfaceArray.h \
 DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h Arrays.h MySwap.h \
 Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h MFDouble.h \
 SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h MFRotation.h \
 MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h \
 MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h SFImage.h \
 SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h SFString.h \
 SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h \
 CommandList.h Command.h FogMacros.h swt/include/swt.h \
 swt/include/keycodes.h swt/include/swlanguages.h swt/include/swDebugf.h \
 swt/include/swttypedef.h Field.h Scene.h URL.h DuneApp.h \
 PreferencesApp.h swt/include/swttypedef.h EcmaScriptApp.h \
 StereoViewApp.h swt/include/stereoview.h InputDeviceApp.h \
 TransformMode.h OutputApp.h RouteViewApp.h StartWithApp.h \
 ExportNamesApp.h ModellingApp.h RenderApp.h RenderState.h ProtoMap.h \
 resource.h
NodeFogCoordinate.o: NodeFogCoordinate.cpp stdafx.h config.h mysnprintf.h \
 NodeFogCoordinate.h Node.h Array.h List.h Vec3f.h Vec3x.h MyString.h \
 StringArray.h FieldIndex.h Element.h x3dFlags.h swt/include/WriteFlags.h \
 NodeFlags.h MeshFlags.h ac3dMaterialCallback.h NodeArray.h Stack.h \
 Matrix.h Vec3d.h Quaternion.h ProtoMacros.h Proto.h Map.h NodeList.h \
 InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h \
 Arrays.h MySwap.h Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h \
 MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h \
 MFRotation.h MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h \
 MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h \
 SFImage.h SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h \
 SFString.h SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h \
 SFVec4f.h CommandList.h Command.h DuneApp.h URL.h PreferencesApp.h \
 swt/include/swttypedef.h EcmaScriptApp.h StereoViewApp.h \
 swt/include/stereoview.h InputDeviceApp.h TransformMode.h OutputApp.h \
 RouteViewApp.h StartWithApp.h ExportNamesApp.h ModellingApp.h \
 RenderApp.h
NodeFontStyle.o: NodeFontStyle.cpp stdafx.h config.h mysnprintf.h \
 NodeFontStyle.h Node.h Array.h List.h Vec3f.h Vec3x.h MyString.h \
 StringArray.h FieldIndex.h Element.h x3dFlags.h swt/include/WriteFlags.h \
 NodeFlags.h MeshFlags.h ac3dMaterialCallback.h NodeArray.h Stack.h \
 Matrix.h Vec3d.h Quaternion.h ProtoMacros.h Proto.h Map.h NodeList.h \
 InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h \
 Arrays.h MySwap.h Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h \
 MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h \
 MFRotation.h MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h \
 MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h \
 SFImage.h SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h \
 SFString.h SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h \
 SFVec4f.h CommandList.h Command.h FontStyleNode.h Field.h
NodeForcePhysicsModel.o: NodeForcePhysicsModel.cpp stdafx.h config.h \
 mysnprintf.h NodeForcePhysicsModel.h Node.h Array.h List.h Vec3f.h \
 Vec3x.h MyString.h StringArray.h FieldIndex.h Element.h x3dFlags.h \
 swt/include/WriteFlags.h NodeFlags.h MeshFlags.h ac3dMaterialCallback.h \
 NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h ProtoMacros.h Proto.h \
 Map.h NodeList.h InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h \
 MFBool.h FieldValue.h Arrays.h MySwap.h Types.h MFColor.h MFFloat.h \
 SFFloat.h MFColorRGBA.h MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h \
 MFMatrix4f.h MFNode.h MFRotation.h MFString.h MFTime.h SFTime.h \
 MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h \
 SFBool.h SFColor.h SFColorRGBA.h SFImage.h SFInt32.h SFMatrix3f.h \
 SFMatrix4f.h SFNode.h SFRotation.h SFString.h SFVec2d.h SFVec2f.h \
 Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h CommandList.h Command.h \
 DuneApp.h URL.h PreferencesApp.h swt/include/swttypedef.h \
 EcmaScriptApp.h StereoViewApp.h swt/include/stereoview.h \
 InputDeviceApp.h TransformMode.h OutputApp.h RouteViewApp.h \
 StartWithApp.h ExportNamesApp.h ModellingApp.h RenderApp.h
NodeGeoCoordinate.o: NodeGeoCoordinate.cpp stdafx.h config.h mysnprintf.h \
 NodeGeoCoordinate.h GeoNode.h Node.h Array.h List.h Vec3f.h Vec3x.h \
 MyString.h StringArray.h FieldIndex.h Element.h x3dFlags.h \
 swt/include/WriteFlags.h NodeFlags.h MeshFlags.h ac3dMaterialCallback.h \
 NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h ProtoMacros.h Proto.h \
 Map.h NodeList.h InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h \
 MFBool.h FieldValue.h Arrays.h MySwap.h Types.h MFColor.h MFFloat.h \
 SFFloat.h MFColorRGBA.h MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h \
 MFMatrix4f.h MFNode.h MFRotation.h MFString.h MFTime.h SFTime.h \
 MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h \
 SFBool.h SFColor.h SFColorRGBA.h SFImage.h SFInt32.h SFMatrix3f.h \
 SFMatrix4f.h SFNode.h SFRotation.h SFString.h SFVec2d.h SFVec2f.h \
 Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h CommandList.h Command.h \
 NodeCoordinate.h ExposedField.h Field.h RenderState.h \
 swt/include/swttypedef.h DuneApp.h URL.h PreferencesApp.h \
 EcmaScriptApp.h StereoViewApp.h swt/include/stereoview.h \
 InputDeviceApp.h TransformMode.h OutputApp.h RouteViewApp.h \
 StartWithApp.h ExportNamesApp.h ModellingApp.h RenderApp.h Util.h \
 Scene.h ProtoMap.h resource.h FaceData.h NodeIndexedFaceSet.h \
 MeshBasedNode.h GeometryNode.h NodeAppearance.h NodeMaterial.h \
 swt/include/swt.h swt/include/keycodes.h swt/include/swlanguages.h \
 swt/include/swDebugf.h swt/include/swttypedef.h NodeImageTexture.h \
 KambiTextureCommonFields.h NodeTextureTransform.h GeometryNodeMacros.h \
 Colored.h NodeColor.h NodeColorRGBA.h ComposedGeometryMacros.h \
 NodeIndexedLineSet.h NodePointSet.h NodeLineSet.h NodeTriangleSet.h \
 MyMesh.h NodeNormal.h NodeTextureCoordinate.h \
 NodeTextureCoordinateGenerator.h NodeMultiTextureCoordinate.h \
 NodeTriangleFanSet.h NodeTriangleStripSet.h NodeIndexedTriangleSet.h \
 IndexedTriangleSetNode.h NodeIndexedTriangleFanSet.h \
 NodeIndexedTriangleStripSet.h NodeQuadSet.h NodeIndexedQuadSet.h \
 NodeNurbsGroup.h NodeGroup.h StaticGroupNode.h GroupNode.h \
 NodeNurbsSurface.h MeshMorphingNode.h Interpolator.h NodeNurbsCurve.h \
 NurbsCurve.h ChainBasedNode.h NodeNurbsTrimmedSurface.h \
 NodeHAnimHumanoid.h TransformNode.h BasicTransformNode.h \
 NodeHAnimJoint.h NodeTransform.h NodeGeoOrigin.h
NodeGeoElevationGrid.o: NodeGeoElevationGrid.cpp stdafx.h config.h \
 mysnprintf.h NodeGeoElevationGrid.h GeoNode.h Node.h Array.h List.h \
 Vec3f.h Vec3x.h MyString.h StringArray.h FieldIndex.h Element.h \
 x3dFlags.h swt/include/WriteFlags.h NodeFlags.h MeshFlags.h \
 ac3dMaterialCallback.h NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h \
 ProtoMacros.h Proto.h Map.h NodeList.h InterfaceArray.h \
 DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h Arrays.h MySwap.h \
 Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h MFDouble.h \
 SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h MFRotation.h \
 MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h \
 MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h SFImage.h \
 SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h SFString.h \
 SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h \
 CommandList.h Command.h NodeAppearance.h DuneApp.h URL.h \
 PreferencesApp.h swt/include/swttypedef.h EcmaScriptApp.h \
 StereoViewApp.h swt/include/stereoview.h InputDeviceApp.h \
 TransformMode.h OutputApp.h RouteViewApp.h StartWithApp.h \
 ExportNamesApp.h ModellingApp.h RenderApp.h NodeMaterial.h \
 swt/include/swt.h swt/include/keycodes.h swt/include/swlanguages.h \
 swt/include/swDebugf.h swt/include/swttypedef.h NodeImageTexture.h \
 KambiTextureCommonFields.h NodeTextureTransform.h NodeGeoOrigin.h \
 GeometryNode.h GeometryNodeMacros.h MeshBasedNode.h ExposedField.h \
 Field.h RenderState.h Util.h Scene.h ProtoMap.h NodeIndexedFaceSet.h \
 NodeCoordinate.h Colored.h NodeColor.h NodeColorRGBA.h \
 ComposedGeometryMacros.h NodeNormal.h NodeShape.h MyMesh.h FaceData.h \
 NodeTextureCoordinate.h NodeTextureCoordinateGenerator.h \
 NodeMultiTextureCoordinate.h
NodeGeoMetadata.o: NodeGeoMetadata.cpp stdafx.h config.h mysnprintf.h \
 NodeGeoMetadata.h Node.h Array.h List.h Vec3f.h Vec3x.h MyString.h \
 StringArray.h FieldIndex.h Element.h x3dFlags.h swt/include/WriteFlags.h \
 NodeFlags.h MeshFlags.h ac3dMaterialCallback.h NodeArray.h Stack.h \
 Matrix.h Vec3d.h Quaternion.h ProtoMacros.h Proto.h Map.h NodeList.h \
 InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h \
 Arrays.h MySwap.h Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h \
 MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h \
 MFRotation.h MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h \
 MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h \
 SFImage.h SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h \
 SFString.h SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h \
 SFVec4f.h CommandList.h Command.h ExposedField.h Field.h RenderState.h \
 swt/include/swttypedef.h DuneApp.h URL.h PreferencesApp.h \
 EcmaScriptApp.h StereoViewApp.h swt/include/stereoview.h \
 InputDeviceApp.h TransformMode.h OutputApp.h RouteViewApp.h \
 StartWithApp.h ExportNamesApp.h ModellingApp.h RenderApp.h Util.h \
 Scene.h ProtoMap.h NodeIndexedFaceSet.h MeshBasedNode.h GeometryNode.h \
 NodeAppearance.h NodeMaterial.h swt/include/swt.h swt/include/keycodes.h \
 swt/include/swlanguages.h swt/include/swDebugf.h \
 swt/include/swttypedef.h NodeImageTexture.h KambiTextureCommonFields.h \
 NodeTextureTransform.h GeometryNodeMacros.h NodeCoordinate.h Colored.h \
 NodeColor.h NodeColorRGBA.h ComposedGeometryMacros.h \
 NodeIndexedLineSet.h NodePointSet.h
NodeGeoLocation.o: NodeGeoLocation.cpp stdafx.h config.h mysnprintf.h \
 NodeGeoLocation.h GeoNode.h Node.h Array.h List.h Vec3f.h Vec3x.h \
 MyString.h StringArray.h FieldIndex.h Element.h x3dFlags.h \
 swt/include/WriteFlags.h NodeFlags.h MeshFlags.h ac3dMaterialCallback.h \
 NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h ProtoMacros.h Proto.h \
 Map.h NodeList.h InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h \
 MFBool.h FieldValue.h Arrays.h MySwap.h Types.h MFColor.h MFFloat.h \
 SFFloat.h MFColorRGBA.h MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h \
 MFMatrix4f.h MFNode.h MFRotation.h MFString.h MFTime.h SFTime.h \
 MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h \
 SFBool.h SFColor.h SFColorRGBA.h SFImage.h SFInt32.h SFMatrix3f.h \
 SFMatrix4f.h SFNode.h SFRotation.h SFString.h SFVec2d.h SFVec2f.h \
 Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h CommandList.h Command.h \
 ExposedField.h Field.h RenderState.h swt/include/swttypedef.h DuneApp.h \
 URL.h PreferencesApp.h EcmaScriptApp.h StereoViewApp.h \
 swt/include/stereoview.h InputDeviceApp.h TransformMode.h OutputApp.h \
 RouteViewApp.h StartWithApp.h ExportNamesApp.h ModellingApp.h \
 RenderApp.h Util.h Scene.h ProtoMap.h NodeIndexedFaceSet.h \
 MeshBasedNode.h GeometryNode.h NodeAppearance.h NodeMaterial.h \
 swt/include/swt.h swt/include/keycodes.h swt/include/swlanguages.h \
 swt/include/swDebugf.h swt/include/swttypedef.h NodeImageTexture.h \
 KambiTextureCommonFields.h NodeTextureTransform.h GeometryNodeMacros.h \
 NodeCoordinate.h Colored.h NodeColor.h NodeColorRGBA.h \
 ComposedGeometryMacros.h NodeIndexedLineSet.h NodePointSet.h \
 NodeGeoOrigin.h
NodeGeoLOD.o: NodeGeoLOD.cpp stdafx.h config.h mysnprintf.h NodeGeoLOD.h \
 GeoNode.h Node.h Array.h List.h Vec3f.h Vec3x.h MyString.h StringArray.h \
 FieldIndex.h Element.h x3dFlags.h swt/include/WriteFlags.h NodeFlags.h \
 MeshFlags.h ac3dMaterialCallback.h NodeArray.h Stack.h Matrix.h Vec3d.h \
 Quaternion.h ProtoMacros.h Proto.h Map.h NodeList.h InterfaceArray.h \
 DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h Arrays.h MySwap.h \
 Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h MFDouble.h \
 SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h MFRotation.h \
 MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h \
 MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h SFImage.h \
 SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h SFString.h \
 SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h \
 CommandList.h Command.h NodeGeoOrigin.h ExposedField.h Field.h \
 RenderState.h swt/include/swttypedef.h DuneApp.h URL.h PreferencesApp.h \
 EcmaScriptApp.h StereoViewApp.h swt/include/stereoview.h \
 InputDeviceApp.h TransformMode.h OutputApp.h RouteViewApp.h \
 StartWithApp.h ExportNamesApp.h ModellingApp.h RenderApp.h resource.h \
 Util.h Scene.h ProtoMap.h NodeIndexedFaceSet.h MeshBasedNode.h \
 GeometryNode.h NodeAppearance.h NodeMaterial.h swt/include/swt.h \
 swt/include/keycodes.h swt/include/swlanguages.h swt/include/swDebugf.h \
 swt/include/swttypedef.h NodeImageTexture.h KambiTextureCommonFields.h \
 NodeTextureTransform.h GeometryNodeMacros.h NodeCoordinate.h Colored.h \
 NodeColor.h NodeColorRGBA.h ComposedGeometryMacros.h \
 NodeIndexedLineSet.h NodePointSet.h
NodeGeoPositionInterpolator.o: NodeGeoPositionInterpolator.cpp stdafx.h \
 config.h mysnprintf.h swt/include/swt.h swt/include/keycodes.h \
 swt/include/swlanguages.h swt/include/swDebugf.h \
 swt/include/swttypedef.h NodeGeoPositionInterpolator.h GeoNode.h Node.h \
 Array.h List.h Vec3f.h Vec3x.h MyString.h StringArray.h FieldIndex.h \
 Element.h x3dFlags.h swt/include/WriteFlags.h NodeFlags.h MeshFlags.h \
 ac3dMaterialCallback.h NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h \
 ProtoMacros.h Proto.h Map.h NodeList.h InterfaceArray.h \
 DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h Arrays.h MySwap.h \
 Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h MFDouble.h \
 SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h MFRotation.h \
 MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h \
 MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h SFImage.h \
 SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h SFString.h \
 SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h \
 CommandList.h Command.h Interpolator.h GeoMacros.h ExposedField.h \
 Field.h RenderState.h swt/include/swttypedef.h DuneApp.h URL.h \
 PreferencesApp.h EcmaScriptApp.h StereoViewApp.h \
 swt/include/stereoview.h InputDeviceApp.h TransformMode.h OutputApp.h \
 RouteViewApp.h StartWithApp.h ExportNamesApp.h ModellingApp.h \
 RenderApp.h Util.h Scene.h ProtoMap.h FieldCommand.h
NodeGeoProximitySensor.o: NodeGeoProximitySensor.cpp stdafx.h config.h \
 mysnprintf.h NodeGeoProximitySensor.h Node.h Array.h List.h Vec3f.h \
 Vec3x.h MyString.h StringArray.h FieldIndex.h Element.h x3dFlags.h \
 swt/include/WriteFlags.h NodeFlags.h MeshFlags.h ac3dMaterialCallback.h \
 NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h ProtoMacros.h Proto.h \
 Map.h NodeList.h InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h \
 MFBool.h FieldValue.h Arrays.h MySwap.h Types.h MFColor.h MFFloat.h \
 SFFloat.h MFColorRGBA.h MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h \
 MFMatrix4f.h MFNode.h MFRotation.h MFString.h MFTime.h SFTime.h \
 MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h \
 SFBool.h SFColor.h SFColorRGBA.h SFImage.h SFInt32.h SFMatrix3f.h \
 SFMatrix4f.h SFNode.h SFRotation.h SFString.h SFVec2d.h SFVec2f.h \
 Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h CommandList.h Command.h \
 GeoNode.h DuneApp.h URL.h PreferencesApp.h swt/include/swttypedef.h \
 EcmaScriptApp.h StereoViewApp.h swt/include/stereoview.h \
 InputDeviceApp.h TransformMode.h OutputApp.h RouteViewApp.h \
 StartWithApp.h ExportNamesApp.h ModellingApp.h RenderApp.h
NodeGeoOrigin.o: NodeGeoOrigin.cpp stdafx.h config.h mysnprintf.h \
 GpsMathLib.h NodeGeoOrigin.h Node.h Array.h List.h Vec3f.h Vec3x.h \
 MyString.h StringArray.h FieldIndex.h Element.h x3dFlags.h \
 swt/include/WriteFlags.h NodeFlags.h MeshFlags.h ac3dMaterialCallback.h \
 NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h ProtoMacros.h Proto.h \
 Map.h NodeList.h InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h \
 MFBool.h FieldValue.h Arrays.h MySwap.h Types.h MFColor.h MFFloat.h \
 SFFloat.h MFColorRGBA.h MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h \
 MFMatrix4f.h MFNode.h MFRotation.h MFString.h MFTime.h SFTime.h \
 MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h \
 SFBool.h SFColor.h SFColorRGBA.h SFImage.h SFInt32.h SFMatrix3f.h \
 SFMatrix4f.h SFNode.h SFRotation.h SFString.h SFVec2d.h SFVec2f.h \
 Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h CommandList.h Command.h \
 GeoMacros.h ExposedField.h Field.h RenderState.h \
 swt/include/swttypedef.h DuneApp.h URL.h PreferencesApp.h \
 EcmaScriptApp.h StereoViewApp.h swt/include/stereoview.h \
 InputDeviceApp.h TransformMode.h OutputApp.h RouteViewApp.h \
 StartWithApp.h ExportNamesApp.h ModellingApp.h RenderApp.h Util.h \
 Scene.h ProtoMap.h NodeIndexedFaceSet.h MeshBasedNode.h GeometryNode.h \
 NodeAppearance.h NodeMaterial.h swt/include/swt.h swt/include/keycodes.h \
 swt/include/swlanguages.h swt/include/swDebugf.h \
 swt/include/swttypedef.h NodeImageTexture.h KambiTextureCommonFields.h \
 NodeTextureTransform.h GeometryNodeMacros.h NodeCoordinate.h Colored.h \
 NodeColor.h NodeColorRGBA.h ComposedGeometryMacros.h \
 NodeIndexedLineSet.h NodePointSet.h
NodeGeoTouchSensor.o: NodeGeoTouchSensor.cpp stdafx.h config.h \
 mysnprintf.h NodeGeoTouchSensor.h GeoNode.h Node.h Array.h List.h \
 Vec3f.h Vec3x.h MyString.h StringArray.h FieldIndex.h Element.h \
 x3dFlags.h swt/include/WriteFlags.h NodeFlags.h MeshFlags.h \
 ac3dMaterialCallback.h NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h \
 ProtoMacros.h Proto.h Map.h NodeList.h InterfaceArray.h \
 DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h Arrays.h MySwap.h \
 Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h MFDouble.h \
 SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h MFRotation.h \
 MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h \
 MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h SFImage.h \
 SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h SFString.h \
 SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h \
 CommandList.h Command.h ExposedField.h Field.h RenderState.h \
 swt/include/swttypedef.h DuneApp.h URL.h PreferencesApp.h \
 EcmaScriptApp.h StereoViewApp.h swt/include/stereoview.h \
 InputDeviceApp.h TransformMode.h OutputApp.h RouteViewApp.h \
 StartWithApp.h ExportNamesApp.h ModellingApp.h RenderApp.h Util.h \
 Scene.h ProtoMap.h NodeIndexedFaceSet.h MeshBasedNode.h GeometryNode.h \
 NodeAppearance.h NodeMaterial.h swt/include/swt.h swt/include/keycodes.h \
 swt/include/swlanguages.h swt/include/swDebugf.h \
 swt/include/swttypedef.h NodeImageTexture.h KambiTextureCommonFields.h \
 NodeTextureTransform.h GeometryNodeMacros.h NodeCoordinate.h Colored.h \
 NodeColor.h NodeColorRGBA.h ComposedGeometryMacros.h \
 NodeIndexedLineSet.h NodePointSet.h
NodeGeoTransform.o: NodeGeoTransform.cpp stdafx.h config.h mysnprintf.h \
 NodeGeoTransform.h Node.h Array.h List.h Vec3f.h Vec3x.h MyString.h \
 StringArray.h FieldIndex.h Element.h x3dFlags.h swt/include/WriteFlags.h \
 NodeFlags.h MeshFlags.h ac3dMaterialCallback.h NodeArray.h Stack.h \
 Matrix.h Vec3d.h Quaternion.h ProtoMacros.h Proto.h Map.h NodeList.h \
 InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h \
 Arrays.h MySwap.h Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h \
 MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h \
 MFRotation.h MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h \
 MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h \
 SFImage.h SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h \
 SFString.h SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h \
 SFVec4f.h CommandList.h Command.h GeoNode.h Scene.h URL.h DuneApp.h \
 PreferencesApp.h swt/include/swttypedef.h EcmaScriptApp.h \
 StereoViewApp.h swt/include/stereoview.h InputDeviceApp.h \
 TransformMode.h OutputApp.h RouteViewApp.h StartWithApp.h \
 ExportNamesApp.h ModellingApp.h RenderApp.h RenderState.h ProtoMap.h \
 NodeGeoOrigin.h
NodeGeoViewpoint.o: NodeGeoViewpoint.cpp stdafx.h config.h mysnprintf.h \
 NodeGeoViewpoint.h GeoNode.h Node.h Array.h List.h Vec3f.h Vec3x.h \
 MyString.h StringArray.h FieldIndex.h Element.h x3dFlags.h \
 swt/include/WriteFlags.h NodeFlags.h MeshFlags.h ac3dMaterialCallback.h \
 NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h ProtoMacros.h Proto.h \
 Map.h NodeList.h InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h \
 MFBool.h FieldValue.h Arrays.h MySwap.h Types.h MFColor.h MFFloat.h \
 SFFloat.h MFColorRGBA.h MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h \
 MFMatrix4f.h MFNode.h MFRotation.h MFString.h MFTime.h SFTime.h \
 MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h \
 SFBool.h SFColor.h SFColorRGBA.h SFImage.h SFInt32.h SFMatrix3f.h \
 SFMatrix4f.h SFNode.h SFRotation.h SFString.h SFVec2d.h SFVec2f.h \
 Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h CommandList.h Command.h \
 ViewpointNode.h swt/include/swt.h swt/include/keycodes.h \
 swt/include/swlanguages.h swt/include/swDebugf.h \
 swt/include/swttypedef.h CoverNode.h KambiViewpointCommonFields.h \
 DuneApp.h URL.h PreferencesApp.h swt/include/swttypedef.h \
 EcmaScriptApp.h StereoViewApp.h swt/include/stereoview.h \
 InputDeviceApp.h TransformMode.h OutputApp.h RouteViewApp.h \
 StartWithApp.h ExportNamesApp.h ModellingApp.h RenderApp.h \
 NodeGeoOrigin.h Scene.h RenderState.h ProtoMap.h GeoMacros.h
NodeGeneratedCubeMapTexture.o: NodeGeneratedCubeMapTexture.cpp stdafx.h \
 config.h mysnprintf.h NodeGeneratedCubeMapTexture.h Node.h Array.h \
 List.h Vec3f.h Vec3x.h MyString.h StringArray.h FieldIndex.h Element.h \
 x3dFlags.h swt/include/WriteFlags.h NodeFlags.h MeshFlags.h \
 ac3dMaterialCallback.h NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h \
 ProtoMacros.h Proto.h Map.h NodeList.h InterfaceArray.h \
 DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h Arrays.h MySwap.h \
 Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h MFDouble.h \
 SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h MFRotation.h \
 MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h \
 MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h SFImage.h \
 SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h SFString.h \
 SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h \
 CommandList.h Command.h DuneApp.h URL.h PreferencesApp.h \
 swt/include/swttypedef.h EcmaScriptApp.h StereoViewApp.h \
 swt/include/stereoview.h InputDeviceApp.h TransformMode.h OutputApp.h \
 RouteViewApp.h StartWithApp.h ExportNamesApp.h ModellingApp.h \
 RenderApp.h
NodeGroup.o: NodeGroup.cpp stdafx.h config.h mysnprintf.h \
 swt/include/swt.h swt/include/keycodes.h swt/include/swlanguages.h \
 swt/include/swDebugf.h swt/include/swttypedef.h NodeGroup.h Node.h \
 Array.h List.h Vec3f.h Vec3x.h MyString.h StringArray.h FieldIndex.h \
 Element.h x3dFlags.h swt/include/WriteFlags.h NodeFlags.h MeshFlags.h \
 ac3dMaterialCallback.h NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h \
 ProtoMacros.h Proto.h Map.h NodeList.h InterfaceArray.h \
 DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h Arrays.h MySwap.h \
 Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h MFDouble.h \
 SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h MFRotation.h \
 MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h \
 MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h SFImage.h \
 SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h SFString.h \
 SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h \
 CommandList.h Command.h StaticGroupNode.h GroupNode.h NodeNurbsGroup.h \
 NodeNurbsSet.h NodeShape.h NodeAnchor.h Scene.h URL.h DuneApp.h \
 PreferencesApp.h swt/include/swttypedef.h EcmaScriptApp.h \
 StereoViewApp.h swt/include/stereoview.h InputDeviceApp.h \
 TransformMode.h OutputApp.h RouteViewApp.h StartWithApp.h \
 ExportNamesApp.h ModellingApp.h RenderApp.h RenderState.h ProtoMap.h
NodeHAnimDisplacer.o: NodeHAnimDisplacer.cpp stdafx.h config.h \
 mysnprintf.h swt/include/swt.h swt/include/keycodes.h \
 swt/include/swlanguages.h swt/include/swDebugf.h \
 swt/include/swttypedef.h DuneApp.h List.h Array.h MyString.h \
 StringArray.h Node.h Vec3f.h Vec3x.h FieldIndex.h Element.h x3dFlags.h \
 swt/include/WriteFlags.h NodeFlags.h MeshFlags.h ac3dMaterialCallback.h \
 NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h URL.h PreferencesApp.h \
 swt/include/swttypedef.h EcmaScriptApp.h StereoViewApp.h \
 swt/include/stereoview.h InputDeviceApp.h TransformMode.h OutputApp.h \
 RouteViewApp.h StartWithApp.h ExportNamesApp.h ModellingApp.h \
 RenderApp.h resource.h NodeHAnimDisplacer.h ProtoMacros.h Proto.h Map.h \
 NodeList.h InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h MFBool.h \
 FieldValue.h Arrays.h MySwap.h Types.h MFColor.h MFFloat.h SFFloat.h \
 MFColorRGBA.h MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h \
 MFNode.h MFRotation.h MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h \
 MFVec2f.h MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h \
 SFColorRGBA.h SFImage.h SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h \
 SFRotation.h SFString.h SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h \
 SFVec3f.h SFVec4f.h CommandList.h Command.h Texture.h
NodeHAnimHumanoid.o: NodeHAnimHumanoid.cpp stdafx.h config.h mysnprintf.h \
 swt/include/swt.h swt/include/keycodes.h swt/include/swlanguages.h \
 swt/include/swDebugf.h swt/include/swttypedef.h NodeHAnimHumanoid.h \
 Node.h Array.h List.h Vec3f.h Vec3x.h MyString.h StringArray.h \
 FieldIndex.h Element.h x3dFlags.h swt/include/WriteFlags.h NodeFlags.h \
 MeshFlags.h ac3dMaterialCallback.h NodeArray.h Stack.h Matrix.h Vec3d.h \
 Quaternion.h ProtoMacros.h Proto.h Map.h NodeList.h InterfaceArray.h \
 DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h Arrays.h MySwap.h \
 Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h MFDouble.h \
 SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h MFRotation.h \
 MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h \
 MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h SFImage.h \
 SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h SFString.h \
 SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h \
 CommandList.h Command.h MyMesh.h NodeIndexedFaceSet.h MeshBasedNode.h \
 GeometryNode.h NodeAppearance.h DuneApp.h URL.h PreferencesApp.h \
 swt/include/swttypedef.h EcmaScriptApp.h StereoViewApp.h \
 swt/include/stereoview.h InputDeviceApp.h TransformMode.h OutputApp.h \
 RouteViewApp.h StartWithApp.h ExportNamesApp.h ModellingApp.h \
 RenderApp.h NodeMaterial.h NodeImageTexture.h KambiTextureCommonFields.h \
 NodeTextureTransform.h GeometryNodeMacros.h NodeCoordinate.h Colored.h \
 NodeColor.h NodeColorRGBA.h ComposedGeometryMacros.h FaceData.h \
 NodeNormal.h NodeTextureCoordinate.h NodeTextureCoordinateGenerator.h \
 NodeMultiTextureCoordinate.h Scene.h RenderState.h ProtoMap.h Util.h \
 TransformNode.h BasicTransformNode.h FieldCommand.h Field.h Path.h \
 NodeViewpoint.h ViewpointNode.h CoverNode.h KambiViewpointCommonFields.h \
 NodeNavigationInfo.h ExposedField.h NodeShape.h NodeHAnimJoint.h \
 NodeTransform.h
NodeHAnimJoint.o: NodeHAnimJoint.cpp stdafx.h config.h mysnprintf.h \
 NodeHAnimJoint.h Node.h Array.h List.h Vec3f.h Vec3x.h MyString.h \
 StringArray.h FieldIndex.h Element.h x3dFlags.h swt/include/WriteFlags.h \
 NodeFlags.h MeshFlags.h ac3dMaterialCallback.h NodeArray.h Stack.h \
 Matrix.h Vec3d.h Quaternion.h ProtoMacros.h Proto.h Map.h NodeList.h \
 InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h \
 Arrays.h MySwap.h Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h \
 MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h \
 MFRotation.h MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h \
 MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h \
 SFImage.h SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h \
 SFString.h SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h \
 SFVec4f.h CommandList.h Command.h NodeTransform.h swt/include/swt.h \
 swt/include/keycodes.h swt/include/swlanguages.h swt/include/swDebugf.h \
 swt/include/swttypedef.h TransformNode.h BasicTransformNode.h MyMesh.h \
 NodeIndexedFaceSet.h MeshBasedNode.h GeometryNode.h NodeAppearance.h \
 DuneApp.h URL.h PreferencesApp.h swt/include/swttypedef.h \
 EcmaScriptApp.h StereoViewApp.h swt/include/stereoview.h \
 InputDeviceApp.h TransformMode.h OutputApp.h RouteViewApp.h \
 StartWithApp.h ExportNamesApp.h ModellingApp.h RenderApp.h \
 NodeMaterial.h NodeImageTexture.h KambiTextureCommonFields.h \
 NodeTextureTransform.h GeometryNodeMacros.h NodeCoordinate.h Colored.h \
 NodeColor.h NodeColorRGBA.h ComposedGeometryMacros.h FaceData.h \
 NodeNormal.h NodeTextureCoordinate.h NodeTextureCoordinateGenerator.h \
 NodeMultiTextureCoordinate.h Scene.h RenderState.h ProtoMap.h Util.h \
 NodeHAnimHumanoid.h Path.h
NodeHAnimSegment.o: NodeHAnimSegment.cpp stdafx.h config.h mysnprintf.h \
 NodeHAnimSegment.h Node.h Array.h List.h Vec3f.h Vec3x.h MyString.h \
 StringArray.h FieldIndex.h Element.h x3dFlags.h swt/include/WriteFlags.h \
 NodeFlags.h MeshFlags.h ac3dMaterialCallback.h NodeArray.h Stack.h \
 Matrix.h Vec3d.h Quaternion.h ProtoMacros.h Proto.h Map.h NodeList.h \
 InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h \
 Arrays.h MySwap.h Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h \
 MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h \
 MFRotation.h MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h \
 MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h \
 SFImage.h SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h \
 SFString.h SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h \
 SFVec4f.h CommandList.h Command.h NodeGroup.h StaticGroupNode.h \
 GroupNode.h Field.h DuneApp.h URL.h PreferencesApp.h \
 swt/include/swttypedef.h EcmaScriptApp.h StereoViewApp.h \
 swt/include/stereoview.h InputDeviceApp.h TransformMode.h OutputApp.h \
 RouteViewApp.h StartWithApp.h ExportNamesApp.h ModellingApp.h \
 RenderApp.h NodeNurbsSurface.h MeshMorphingNode.h Interpolator.h \
 MeshBasedNode.h GeometryNode.h NodeAppearance.h NodeMaterial.h \
 swt/include/swt.h swt/include/keycodes.h swt/include/swlanguages.h \
 swt/include/swDebugf.h swt/include/swttypedef.h NodeImageTexture.h \
 KambiTextureCommonFields.h NodeTextureTransform.h GeometryNodeMacros.h \
 NodeShape.h Scene.h RenderState.h ProtoMap.h NodeNurbsSet.h
NodeHAnimSite.o: NodeHAnimSite.cpp stdafx.h config.h mysnprintf.h \
 NodeHAnimSite.h Node.h Array.h List.h Vec3f.h Vec3x.h MyString.h \
 StringArray.h FieldIndex.h Element.h x3dFlags.h swt/include/WriteFlags.h \
 NodeFlags.h MeshFlags.h ac3dMaterialCallback.h NodeArray.h Stack.h \
 Matrix.h Vec3d.h Quaternion.h ProtoMacros.h Proto.h Map.h NodeList.h \
 InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h \
 Arrays.h MySwap.h Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h \
 MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h \
 MFRotation.h MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h \
 MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h \
 SFImage.h SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h \
 SFString.h SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h \
 SFVec4f.h CommandList.h Command.h NodeTransform.h swt/include/swt.h \
 swt/include/keycodes.h swt/include/swlanguages.h swt/include/swDebugf.h \
 swt/include/swttypedef.h TransformNode.h BasicTransformNode.h
NodeImageCubeMapTexture.o: NodeImageCubeMapTexture.cpp stdafx.h config.h \
 mysnprintf.h NodeImageCubeMapTexture.h Node.h Array.h List.h Vec3f.h \
 Vec3x.h MyString.h StringArray.h FieldIndex.h Element.h x3dFlags.h \
 swt/include/WriteFlags.h NodeFlags.h MeshFlags.h ac3dMaterialCallback.h \
 NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h ProtoMacros.h Proto.h \
 Map.h NodeList.h InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h \
 MFBool.h FieldValue.h Arrays.h MySwap.h Types.h MFColor.h MFFloat.h \
 SFFloat.h MFColorRGBA.h MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h \
 MFMatrix4f.h MFNode.h MFRotation.h MFString.h MFTime.h SFTime.h \
 MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h \
 SFBool.h SFColor.h SFColorRGBA.h SFImage.h SFInt32.h SFMatrix3f.h \
 SFMatrix4f.h SFNode.h SFRotation.h SFString.h SFVec2d.h SFVec2f.h \
 Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h CommandList.h Command.h \
 DuneApp.h URL.h PreferencesApp.h swt/include/swttypedef.h \
 EcmaScriptApp.h StereoViewApp.h swt/include/stereoview.h \
 InputDeviceApp.h TransformMode.h OutputApp.h RouteViewApp.h \
 StartWithApp.h ExportNamesApp.h ModellingApp.h RenderApp.h
NodeImageTexture.o: NodeImageTexture.cpp stdafx.h config.h mysnprintf.h \
 swt/include/swt.h swt/include/keycodes.h swt/include/swlanguages.h \
 swt/include/swDebugf.h swt/include/swttypedef.h DuneApp.h List.h Array.h \
 MyString.h StringArray.h Node.h Vec3f.h Vec3x.h FieldIndex.h Element.h \
 x3dFlags.h swt/include/WriteFlags.h NodeFlags.h MeshFlags.h \
 ac3dMaterialCallback.h NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h \
 URL.h PreferencesApp.h swt/include/swttypedef.h EcmaScriptApp.h \
 StereoViewApp.h swt/include/stereoview.h InputDeviceApp.h \
 TransformMode.h OutputApp.h RouteViewApp.h StartWithApp.h \
 ExportNamesApp.h ModellingApp.h RenderApp.h resource.h \
 NodeImageTexture.h ProtoMacros.h Proto.h Map.h NodeList.h \
 InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h \
 Arrays.h MySwap.h Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h \
 MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h \
 MFRotation.h MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h \
 MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h \
 SFImage.h SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h \
 SFString.h SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h \
 SFVec4f.h CommandList.h Command.h KambiTextureCommonFields.h Scene.h \
 RenderState.h ProtoMap.h Field.h Image.h Util.h Texture.h MyMesh.h \
 NodeIndexedFaceSet.h MeshBasedNode.h GeometryNode.h NodeAppearance.h \
 NodeMaterial.h NodeTextureTransform.h GeometryNodeMacros.h \
 NodeCoordinate.h Colored.h NodeColor.h NodeColorRGBA.h \
 ComposedGeometryMacros.h FaceData.h NodeNormal.h NodeTextureCoordinate.h \
 NodeTextureCoordinateGenerator.h NodeMultiTextureCoordinate.h \
 NodeShape.h NodePixelTexture.h
NodeImageTexture3D.o: NodeImageTexture3D.cpp stdafx.h config.h \
 mysnprintf.h NodeImageTexture3D.h MeshBasedNode.h GeometryNode.h Node.h \
 Array.h List.h Vec3f.h Vec3x.h MyString.h StringArray.h FieldIndex.h \
 Element.h x3dFlags.h swt/include/WriteFlags.h NodeFlags.h MeshFlags.h \
 ac3dMaterialCallback.h NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h \
 NodeAppearance.h ProtoMacros.h Proto.h Map.h NodeList.h InterfaceArray.h \
 DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h Arrays.h MySwap.h \
 Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h MFDouble.h \
 SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h MFRotation.h \
 MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h \
 MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h SFImage.h \
 SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h SFString.h \
 SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h \
 CommandList.h Command.h DuneApp.h URL.h PreferencesApp.h \
 swt/include/swttypedef.h EcmaScriptApp.h StereoViewApp.h \
 swt/include/stereoview.h InputDeviceApp.h TransformMode.h OutputApp.h \
 RouteViewApp.h StartWithApp.h ExportNamesApp.h ModellingApp.h \
 RenderApp.h NodeMaterial.h swt/include/swt.h swt/include/keycodes.h \
 swt/include/swlanguages.h swt/include/swDebugf.h \
 swt/include/swttypedef.h NodeImageTexture.h KambiTextureCommonFields.h \
 NodeTextureTransform.h GeometryNodeMacros.h freewrl_define.h \
 Texture3DNode.h Scene.h RenderState.h ProtoMap.h MyMesh.h \
 NodeIndexedFaceSet.h NodeCoordinate.h Colored.h NodeColor.h \
 NodeColorRGBA.h ComposedGeometryMacros.h FaceData.h NodeNormal.h \
 NodeTextureCoordinate.h NodeTextureCoordinateGenerator.h \
 NodeMultiTextureCoordinate.h Util.h NodeTwoSidedMaterial.h \
 NodeVolumeData.h NodeTextureProperties.h
NodeIndexedLineSet.o: NodeIndexedLineSet.cpp stdafx.h config.h \
 mysnprintf.h NodeIndexedLineSet.h GeometryNode.h Node.h Array.h List.h \
 Vec3f.h Vec3x.h MyString.h StringArray.h FieldIndex.h Element.h \
 x3dFlags.h swt/include/WriteFlags.h NodeFlags.h MeshFlags.h \
 ac3dMaterialCallback.h NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h \
 NodeAppearance.h ProtoMacros.h Proto.h Map.h NodeList.h InterfaceArray.h \
 DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h Arrays.h MySwap.h \
 Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h MFDouble.h \
 SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h MFRotation.h \
 MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h \
 MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h SFImage.h \
 SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h SFString.h \
 SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h \
 CommandList.h Command.h DuneApp.h URL.h PreferencesApp.h \
 swt/include/swttypedef.h EcmaScriptApp.h StereoViewApp.h \
 swt/include/stereoview.h InputDeviceApp.h TransformMode.h OutputApp.h \
 RouteViewApp.h StartWithApp.h ExportNamesApp.h ModellingApp.h \
 RenderApp.h NodeMaterial.h swt/include/swt.h swt/include/keycodes.h \
 swt/include/swlanguages.h swt/include/swDebugf.h \
 swt/include/swttypedef.h NodeImageTexture.h KambiTextureCommonFields.h \
 NodeTextureTransform.h GeometryNodeMacros.h Colored.h NodeColor.h \
 NodeColorRGBA.h Scene.h RenderState.h ProtoMap.h NodeShape.h \
 NodeCoordinate.h NodeGeoCoordinate.h GeoNode.h NodePointSet.h \
 NodeFogCoordinate.h Util.h LdrawDefines.h
NodeIndexedQuadSet.o: NodeIndexedQuadSet.cpp stdafx.h config.h \
 mysnprintf.h NodeIndexedQuadSet.h MeshBasedNode.h GeometryNode.h Node.h \
 Array.h List.h Vec3f.h Vec3x.h MyString.h StringArray.h FieldIndex.h \
 Element.h x3dFlags.h swt/include/WriteFlags.h NodeFlags.h MeshFlags.h \
 ac3dMaterialCallback.h NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h \
 NodeAppearance.h ProtoMacros.h Proto.h Map.h NodeList.h InterfaceArray.h \
 DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h Arrays.h MySwap.h \
 Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h MFDouble.h \
 SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h MFRotation.h \
 MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h \
 MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h SFImage.h \
 SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h SFString.h \
 SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h \
 CommandList.h Command.h DuneApp.h URL.h PreferencesApp.h \
 swt/include/swttypedef.h EcmaScriptApp.h StereoViewApp.h \
 swt/include/stereoview.h InputDeviceApp.h TransformMode.h OutputApp.h \
 RouteViewApp.h StartWithApp.h ExportNamesApp.h ModellingApp.h \
 RenderApp.h NodeMaterial.h swt/include/swt.h swt/include/keycodes.h \
 swt/include/swlanguages.h swt/include/swDebugf.h \
 swt/include/swttypedef.h NodeImageTexture.h KambiTextureCommonFields.h \
 NodeTextureTransform.h GeometryNodeMacros.h Colored.h NodeColor.h \
 NodeColorRGBA.h ComposedGeometryMacros.h Scene.h RenderState.h \
 ProtoMap.h MyMesh.h NodeIndexedFaceSet.h NodeCoordinate.h FaceData.h \
 NodeNormal.h NodeTextureCoordinate.h NodeTextureCoordinateGenerator.h \
 NodeMultiTextureCoordinate.h Util.h NodeGeoCoordinate.h GeoNode.h \
 NodeIndexedLineSet.h
NodeIndexedTriangleFanSet.o: NodeIndexedTriangleFanSet.cpp stdafx.h \
 config.h mysnprintf.h NodeIndexedTriangleFanSet.h MeshBasedNode.h \
 GeometryNode.h Node.h Array.h List.h Vec3f.h Vec3x.h MyString.h \
 StringArray.h FieldIndex.h Element.h x3dFlags.h swt/include/WriteFlags.h \
 NodeFlags.h MeshFlags.h ac3dMaterialCallback.h NodeArray.h Stack.h \
 Matrix.h Vec3d.h Quaternion.h NodeAppearance.h ProtoMacros.h Proto.h \
 Map.h NodeList.h InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h \
 MFBool.h FieldValue.h Arrays.h MySwap.h Types.h MFColor.h MFFloat.h \
 SFFloat.h MFColorRGBA.h MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h \
 MFMatrix4f.h MFNode.h MFRotation.h MFString.h MFTime.h SFTime.h \
 MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h \
 SFBool.h SFColor.h SFColorRGBA.h SFImage.h SFInt32.h SFMatrix3f.h \
 SFMatrix4f.h SFNode.h SFRotation.h SFString.h SFVec2d.h SFVec2f.h \
 Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h CommandList.h Command.h \
 DuneApp.h URL.h PreferencesApp.h swt/include/swttypedef.h \
 EcmaScriptApp.h StereoViewApp.h swt/include/stereoview.h \
 InputDeviceApp.h TransformMode.h OutputApp.h RouteViewApp.h \
 StartWithApp.h ExportNamesApp.h ModellingApp.h RenderApp.h \
 NodeMaterial.h swt/include/swt.h swt/include/keycodes.h \
 swt/include/swlanguages.h swt/include/swDebugf.h \
 swt/include/swttypedef.h NodeImageTexture.h KambiTextureCommonFields.h \
 NodeTextureTransform.h GeometryNodeMacros.h MyMesh.h \
 NodeIndexedFaceSet.h NodeCoordinate.h Colored.h NodeColor.h \
 NodeColorRGBA.h ComposedGeometryMacros.h FaceData.h NodeNormal.h \
 NodeTextureCoordinate.h NodeTextureCoordinateGenerator.h \
 NodeMultiTextureCoordinate.h Scene.h RenderState.h ProtoMap.h Util.h \
 IndexedTriangleSetNode.h NodeGeoCoordinate.h GeoNode.h \
 NodeIndexedLineSet.h NodeFogCoordinate.h
NodeIndexedTriangleSet.o: NodeIndexedTriangleSet.cpp stdafx.h config.h \
 mysnprintf.h NodeIndexedTriangleSet.h MeshBasedNode.h GeometryNode.h \
 Node.h Array.h List.h Vec3f.h Vec3x.h MyString.h StringArray.h \
 FieldIndex.h Element.h x3dFlags.h swt/include/WriteFlags.h NodeFlags.h \
 MeshFlags.h ac3dMaterialCallback.h NodeArray.h Stack.h Matrix.h Vec3d.h \
 Quaternion.h NodeAppearance.h ProtoMacros.h Proto.h Map.h NodeList.h \
 InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h \
 Arrays.h MySwap.h Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h \
 MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h \
 MFRotation.h MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h \
 MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h \
 SFImage.h SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h \
 SFString.h SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h \
 SFVec4f.h CommandList.h Command.h DuneApp.h URL.h PreferencesApp.h \
 swt/include/swttypedef.h EcmaScriptApp.h StereoViewApp.h \
 swt/include/stereoview.h InputDeviceApp.h TransformMode.h OutputApp.h \
 RouteViewApp.h StartWithApp.h ExportNamesApp.h ModellingApp.h \
 RenderApp.h NodeMaterial.h swt/include/swt.h swt/include/keycodes.h \
 swt/include/swlanguages.h swt/include/swDebugf.h \
 swt/include/swttypedef.h NodeImageTexture.h KambiTextureCommonFields.h \
 NodeTextureTransform.h GeometryNodeMacros.h MyMesh.h \
 NodeIndexedFaceSet.h NodeCoordinate.h Colored.h NodeColor.h \
 NodeColorRGBA.h ComposedGeometryMacros.h FaceData.h NodeNormal.h \
 NodeTextureCoordinate.h NodeTextureCoordinateGenerator.h \
 NodeMultiTextureCoordinate.h Scene.h RenderState.h ProtoMap.h Util.h \
 IndexedTriangleSetNode.h NodeGeoCoordinate.h GeoNode.h \
 NodeIndexedLineSet.h NodeFogCoordinate.h
NodeIndexedTriangleStripSet.o: NodeIndexedTriangleStripSet.cpp stdafx.h \
 config.h mysnprintf.h NodeIndexedTriangleStripSet.h MeshBasedNode.h \
 GeometryNode.h Node.h Array.h List.h Vec3f.h Vec3x.h MyString.h \
 StringArray.h FieldIndex.h Element.h x3dFlags.h swt/include/WriteFlags.h \
 NodeFlags.h MeshFlags.h ac3dMaterialCallback.h NodeArray.h Stack.h \
 Matrix.h Vec3d.h Quaternion.h NodeAppearance.h ProtoMacros.h Proto.h \
 Map.h NodeList.h InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h \
 MFBool.h FieldValue.h Arrays.h MySwap.h Types.h MFColor.h MFFloat.h \
 SFFloat.h MFColorRGBA.h MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h \
 MFMatrix4f.h MFNode.h MFRotation.h MFString.h MFTime.h SFTime.h \
 MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h \
 SFBool.h SFColor.h SFColorRGBA.h SFImage.h SFInt32.h SFMatrix3f.h \
 SFMatrix4f.h SFNode.h SFRotation.h SFString.h SFVec2d.h SFVec2f.h \
 Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h CommandList.h Command.h \
 DuneApp.h URL.h PreferencesApp.h swt/include/swttypedef.h \
 EcmaScriptApp.h StereoViewApp.h swt/include/stereoview.h \
 InputDeviceApp.h TransformMode.h OutputApp.h RouteViewApp.h \
 StartWithApp.h ExportNamesApp.h ModellingApp.h RenderApp.h \
 NodeMaterial.h swt/include/swt.h swt/include/keycodes.h \
 swt/include/swlanguages.h swt/include/swDebugf.h \
 swt/include/swttypedef.h NodeImageTexture.h KambiTextureCommonFields.h \
 NodeTextureTransform.h GeometryNodeMacros.h MyMesh.h \
 NodeIndexedFaceSet.h NodeCoordinate.h Colored.h NodeColor.h \
 NodeColorRGBA.h ComposedGeometryMacros.h FaceData.h NodeNormal.h \
 NodeTextureCoordinate.h NodeTextureCoordinateGenerator.h \
 NodeMultiTextureCoordinate.h Scene.h RenderState.h ProtoMap.h Util.h \
 IndexedTriangleSetNode.h NodeGeoCoordinate.h GeoNode.h \
 NodeIndexedLineSet.h NodeFogCoordinate.h
NodeInline.o: NodeInline.cpp stdafx.h config.h mysnprintf.h NodeInline.h \
 Node.h Array.h List.h Vec3f.h Vec3x.h MyString.h StringArray.h \
 FieldIndex.h Element.h x3dFlags.h swt/include/WriteFlags.h NodeFlags.h \
 MeshFlags.h ac3dMaterialCallback.h NodeArray.h Stack.h Matrix.h Vec3d.h \
 Quaternion.h ProtoMacros.h Proto.h Map.h NodeList.h InterfaceArray.h \
 DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h Arrays.h MySwap.h \
 Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h MFDouble.h \
 SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h MFRotation.h \
 MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h \
 MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h SFImage.h \
 SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h SFString.h \
 SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h \
 CommandList.h Command.h Field.h Scene.h URL.h DuneApp.h PreferencesApp.h \
 swt/include/swttypedef.h EcmaScriptApp.h StereoViewApp.h \
 swt/include/stereoview.h InputDeviceApp.h TransformMode.h OutputApp.h \
 RouteViewApp.h StartWithApp.h ExportNamesApp.h ModellingApp.h \
 RenderApp.h RenderState.h ProtoMap.h NodeInlineLoadControl.h NodeGroup.h \
 StaticGroupNode.h GroupNode.h MoveCommand.h
NodeInlineLoadControl.o: NodeInlineLoadControl.cpp stdafx.h config.h \
 mysnprintf.h NodeInlineLoadControl.h Node.h Array.h List.h Vec3f.h \
 Vec3x.h MyString.h StringArray.h FieldIndex.h Element.h x3dFlags.h \
 swt/include/WriteFlags.h NodeFlags.h MeshFlags.h ac3dMaterialCallback.h \
 NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h ProtoMacros.h Proto.h \
 Map.h NodeList.h InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h \
 MFBool.h FieldValue.h Arrays.h MySwap.h Types.h MFColor.h MFFloat.h \
 SFFloat.h MFColorRGBA.h MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h \
 MFMatrix4f.h MFNode.h MFRotation.h MFString.h MFTime.h SFTime.h \
 MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h \
 SFBool.h SFColor.h SFColorRGBA.h SFImage.h SFInt32.h SFMatrix3f.h \
 SFMatrix4f.h SFNode.h SFRotation.h SFString.h SFVec2d.h SFVec2f.h \
 Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h CommandList.h Command.h \
 NodeInline.h Field.h DuneApp.h URL.h PreferencesApp.h \
 swt/include/swttypedef.h EcmaScriptApp.h StereoViewApp.h \
 swt/include/stereoview.h InputDeviceApp.h TransformMode.h OutputApp.h \
 RouteViewApp.h StartWithApp.h ExportNamesApp.h ModellingApp.h \
 RenderApp.h Scene.h RenderState.h ProtoMap.h
NodeIntegerSequencer.o: NodeIntegerSequencer.cpp stdafx.h config.h \
 mysnprintf.h swt/include/swt.h swt/include/keycodes.h \
 swt/include/swlanguages.h swt/include/swDebugf.h \
 swt/include/swttypedef.h DuneApp.h List.h Array.h MyString.h \
 StringArray.h Node.h Vec3f.h Vec3x.h FieldIndex.h Element.h x3dFlags.h \
 swt/include/WriteFlags.h NodeFlags.h MeshFlags.h ac3dMaterialCallback.h \
 NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h URL.h PreferencesApp.h \
 swt/include/swttypedef.h EcmaScriptApp.h StereoViewApp.h \
 swt/include/stereoview.h InputDeviceApp.h TransformMode.h OutputApp.h \
 RouteViewApp.h StartWithApp.h ExportNamesApp.h ModellingApp.h \
 RenderApp.h resource.h NodeIntegerSequencer.h Interpolator.h \
 ProtoMacros.h Proto.h Map.h NodeList.h InterfaceArray.h \
 DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h Arrays.h MySwap.h \
 Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h MFDouble.h \
 SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h MFRotation.h \
 MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h \
 MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h SFImage.h \
 SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h SFString.h \
 SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h \
 CommandList.h Command.h Texture.h Scene.h RenderState.h ProtoMap.h \
 FieldCommand.h
NodeIntegerTrigger.o: NodeIntegerTrigger.cpp stdafx.h config.h \
 mysnprintf.h swt/include/swt.h swt/include/keycodes.h \
 swt/include/swlanguages.h swt/include/swDebugf.h \
 swt/include/swttypedef.h DuneApp.h List.h Array.h MyString.h \
 StringArray.h Node.h Vec3f.h Vec3x.h FieldIndex.h Element.h x3dFlags.h \
 swt/include/WriteFlags.h NodeFlags.h MeshFlags.h ac3dMaterialCallback.h \
 NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h URL.h PreferencesApp.h \
 swt/include/swttypedef.h EcmaScriptApp.h StereoViewApp.h \
 swt/include/stereoview.h InputDeviceApp.h TransformMode.h OutputApp.h \
 RouteViewApp.h StartWithApp.h ExportNamesApp.h ModellingApp.h \
 RenderApp.h resource.h NodeIntegerTrigger.h ProtoMacros.h Proto.h Map.h \
 NodeList.h InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h MFBool.h \
 FieldValue.h Arrays.h MySwap.h Types.h MFColor.h MFFloat.h SFFloat.h \
 MFColorRGBA.h MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h \
 MFNode.h MFRotation.h MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h \
 MFVec2f.h MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h \
 SFColorRGBA.h SFImage.h SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h \
 SFRotation.h SFString.h SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h \
 SFVec3f.h SFVec4f.h CommandList.h Command.h Texture.h
NodeKeySensor.o: NodeKeySensor.cpp stdafx.h config.h mysnprintf.h \
 NodeKeySensor.h Node.h Array.h List.h Vec3f.h Vec3x.h MyString.h \
 StringArray.h FieldIndex.h Element.h x3dFlags.h swt/include/WriteFlags.h \
 NodeFlags.h MeshFlags.h ac3dMaterialCallback.h NodeArray.h Stack.h \
 Matrix.h Vec3d.h Quaternion.h ProtoMacros.h Proto.h Map.h NodeList.h \
 InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h \
 Arrays.h MySwap.h Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h \
 MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h \
 MFRotation.h MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h \
 MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h \
 SFImage.h SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h \
 SFString.h SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h \
 SFVec4f.h CommandList.h Command.h Field.h
NodeLayer.o: NodeLayer.cpp stdafx.h config.h mysnprintf.h NodeLayer.h \
 Node.h Array.h List.h Vec3f.h Vec3x.h MyString.h StringArray.h \
 FieldIndex.h Element.h x3dFlags.h swt/include/WriteFlags.h NodeFlags.h \
 MeshFlags.h ac3dMaterialCallback.h NodeArray.h Stack.h Matrix.h Vec3d.h \
 Quaternion.h ProtoMacros.h Proto.h Map.h NodeList.h InterfaceArray.h \
 DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h Arrays.h MySwap.h \
 Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h MFDouble.h \
 SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h MFRotation.h \
 MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h \
 MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h SFImage.h \
 SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h SFString.h \
 SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h \
 CommandList.h Command.h GroupNode.h DuneApp.h URL.h PreferencesApp.h \
 swt/include/swttypedef.h EcmaScriptApp.h StereoViewApp.h \
 swt/include/stereoview.h InputDeviceApp.h TransformMode.h OutputApp.h \
 RouteViewApp.h StartWithApp.h ExportNamesApp.h ModellingApp.h \
 RenderApp.h
NodeLayerSet.o: NodeLayerSet.cpp stdafx.h config.h mysnprintf.h \
 NodeLayerSet.h Node.h Array.h List.h Vec3f.h Vec3x.h MyString.h \
 StringArray.h FieldIndex.h Element.h x3dFlags.h swt/include/WriteFlags.h \
 NodeFlags.h MeshFlags.h ac3dMaterialCallback.h NodeArray.h Stack.h \
 Matrix.h Vec3d.h Quaternion.h ProtoMacros.h Proto.h Map.h NodeList.h \
 InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h \
 Arrays.h MySwap.h Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h \
 MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h \
 MFRotation.h MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h \
 MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h \
 SFImage.h SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h \
 SFString.h SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h \
 SFVec4f.h CommandList.h Command.h DuneApp.h URL.h PreferencesApp.h \
 swt/include/swttypedef.h EcmaScriptApp.h StereoViewApp.h \
 swt/include/stereoview.h InputDeviceApp.h TransformMode.h OutputApp.h \
 RouteViewApp.h StartWithApp.h ExportNamesApp.h ModellingApp.h \
 RenderApp.h
NodeLayout.o: NodeLayout.cpp stdafx.h config.h mysnprintf.h NodeLayout.h \
 Node.h Array.h List.h Vec3f.h Vec3x.h MyString.h StringArray.h \
 FieldIndex.h Element.h x3dFlags.h swt/include/WriteFlags.h NodeFlags.h \
 MeshFlags.h ac3dMaterialCallback.h NodeArray.h Stack.h Matrix.h Vec3d.h \
 Quaternion.h ProtoMacros.h Proto.h Map.h NodeList.h InterfaceArray.h \
 DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h Arrays.h MySwap.h \
 Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h MFDouble.h \
 SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h MFRotation.h \
 MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h \
 MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h SFImage.h \
 SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h SFString.h \
 SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h \
 CommandList.h Command.h DuneApp.h URL.h PreferencesApp.h \
 swt/include/swttypedef.h EcmaScriptApp.h StereoViewApp.h \
 swt/include/stereoview.h InputDeviceApp.h TransformMode.h OutputApp.h \
 RouteViewApp.h StartWithApp.h ExportNamesApp.h ModellingApp.h \
 RenderApp.h
NodeLayoutGroup.o: NodeLayoutGroup.cpp stdafx.h config.h mysnprintf.h \
 NodeLayoutGroup.h Node.h Array.h List.h Vec3f.h Vec3x.h MyString.h \
 StringArray.h FieldIndex.h Element.h x3dFlags.h swt/include/WriteFlags.h \
 NodeFlags.h MeshFlags.h ac3dMaterialCallback.h NodeArray.h Stack.h \
 Matrix.h Vec3d.h Quaternion.h ProtoMacros.h Proto.h Map.h NodeList.h \
 InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h \
 Arrays.h MySwap.h Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h \
 MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h \
 MFRotation.h MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h \
 MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h \
 SFImage.h SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h \
 SFString.h SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h \
 SFVec4f.h CommandList.h Command.h Scene.h URL.h DuneApp.h \
 PreferencesApp.h swt/include/swttypedef.h EcmaScriptApp.h \
 StereoViewApp.h swt/include/stereoview.h InputDeviceApp.h \
 TransformMode.h OutputApp.h RouteViewApp.h StartWithApp.h \
 ExportNamesApp.h ModellingApp.h RenderApp.h RenderState.h ProtoMap.h
NodeLayoutLayer.o: NodeLayoutLayer.cpp stdafx.h config.h mysnprintf.h \
 NodeLayoutLayer.h Node.h Array.h List.h Vec3f.h Vec3x.h MyString.h \
 StringArray.h FieldIndex.h Element.h x3dFlags.h swt/include/WriteFlags.h \
 NodeFlags.h MeshFlags.h ac3dMaterialCallback.h NodeArray.h Stack.h \
 Matrix.h Vec3d.h Quaternion.h ProtoMacros.h Proto.h Map.h NodeList.h \
 InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h \
 Arrays.h MySwap.h Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h \
 MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h \
 MFRotation.h MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h \
 MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h \
 SFImage.h SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h \
 SFString.h SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h \
 SFVec4f.h CommandList.h Command.h Scene.h URL.h DuneApp.h \
 PreferencesApp.h swt/include/swttypedef.h EcmaScriptApp.h \
 StereoViewApp.h swt/include/stereoview.h InputDeviceApp.h \
 TransformMode.h OutputApp.h RouteViewApp.h StartWithApp.h \
 ExportNamesApp.h ModellingApp.h RenderApp.h RenderState.h ProtoMap.h
NodeLinePickSensor.o: NodeLinePickSensor.cpp stdafx.h config.h \
 mysnprintf.h NodeLinePickSensor.h Node.h Array.h List.h Vec3f.h Vec3x.h \
 MyString.h StringArray.h FieldIndex.h Element.h x3dFlags.h \
 swt/include/WriteFlags.h NodeFlags.h MeshFlags.h ac3dMaterialCallback.h \
 NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h ProtoMacros.h Proto.h \
 Map.h NodeList.h InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h \
 MFBool.h FieldValue.h Arrays.h MySwap.h Types.h MFColor.h MFFloat.h \
 SFFloat.h MFColorRGBA.h MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h \
 MFMatrix4f.h MFNode.h MFRotation.h MFString.h MFTime.h SFTime.h \
 MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h \
 SFBool.h SFColor.h SFColorRGBA.h SFImage.h SFInt32.h SFMatrix3f.h \
 SFMatrix4f.h SFNode.h SFRotation.h SFString.h SFVec2d.h SFVec2f.h \
 Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h CommandList.h Command.h \
 DuneApp.h URL.h PreferencesApp.h swt/include/swttypedef.h \
 EcmaScriptApp.h StereoViewApp.h swt/include/stereoview.h \
 InputDeviceApp.h TransformMode.h OutputApp.h RouteViewApp.h \
 StartWithApp.h ExportNamesApp.h ModellingApp.h RenderApp.h
NodeLineProperties.o: NodeLineProperties.cpp stdafx.h config.h \
 mysnprintf.h NodeLineProperties.h Node.h Array.h List.h Vec3f.h Vec3x.h \
 MyString.h StringArray.h FieldIndex.h Element.h x3dFlags.h \
 swt/include/WriteFlags.h NodeFlags.h MeshFlags.h ac3dMaterialCallback.h \
 NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h ProtoMacros.h Proto.h \
 Map.h NodeList.h InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h \
 MFBool.h FieldValue.h Arrays.h MySwap.h Types.h MFColor.h MFFloat.h \
 SFFloat.h MFColorRGBA.h MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h \
 MFMatrix4f.h MFNode.h MFRotation.h MFString.h MFTime.h SFTime.h \
 MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h \
 SFBool.h SFColor.h SFColorRGBA.h SFImage.h SFInt32.h SFMatrix3f.h \
 SFMatrix4f.h SFNode.h SFRotation.h SFString.h SFVec2d.h SFVec2f.h \
 Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h CommandList.h Command.h \
 DuneApp.h URL.h PreferencesApp.h swt/include/swttypedef.h \
 EcmaScriptApp.h StereoViewApp.h swt/include/stereoview.h \
 InputDeviceApp.h TransformMode.h OutputApp.h RouteViewApp.h \
 StartWithApp.h ExportNamesApp.h ModellingApp.h RenderApp.h
NodeLineSet.o: NodeLineSet.cpp stdafx.h config.h mysnprintf.h \
 NodeLineSet.h GeometryNode.h Node.h Array.h List.h Vec3f.h Vec3x.h \
 MyString.h StringArray.h FieldIndex.h Element.h x3dFlags.h \
 swt/include/WriteFlags.h NodeFlags.h MeshFlags.h ac3dMaterialCallback.h \
 NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h NodeAppearance.h \
 ProtoMacros.h Proto.h Map.h NodeList.h InterfaceArray.h \
 DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h Arrays.h MySwap.h \
 Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h MFDouble.h \
 SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h MFRotation.h \
 MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h \
 MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h SFImage.h \
 SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h SFString.h \
 SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h \
 CommandList.h Command.h DuneApp.h URL.h PreferencesApp.h \
 swt/include/swttypedef.h EcmaScriptApp.h StereoViewApp.h \
 swt/include/stereoview.h InputDeviceApp.h TransformMode.h OutputApp.h \
 RouteViewApp.h StartWithApp.h ExportNamesApp.h ModellingApp.h \
 RenderApp.h NodeMaterial.h swt/include/swt.h swt/include/keycodes.h \
 swt/include/swlanguages.h swt/include/swDebugf.h \
 swt/include/swttypedef.h NodeImageTexture.h KambiTextureCommonFields.h \
 NodeTextureTransform.h GeometryNodeMacros.h Colored.h NodeColor.h \
 NodeColorRGBA.h Scene.h RenderState.h ProtoMap.h NodeShape.h \
 NodeCoordinate.h NodeGeoCoordinate.h GeoNode.h NodeFogCoordinate.h \
 Util.h LdrawDefines.h
NodeLoadSensor.o: NodeLoadSensor.cpp stdafx.h config.h mysnprintf.h \
 NodeLoadSensor.h Node.h Array.h List.h Vec3f.h Vec3x.h MyString.h \
 StringArray.h FieldIndex.h Element.h x3dFlags.h swt/include/WriteFlags.h \
 NodeFlags.h MeshFlags.h ac3dMaterialCallback.h NodeArray.h Stack.h \
 Matrix.h Vec3d.h Quaternion.h ProtoMacros.h Proto.h Map.h NodeList.h \
 InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h \
 Arrays.h MySwap.h Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h \
 MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h \
 MFRotation.h MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h \
 MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h \
 SFImage.h SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h \
 SFString.h SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h \
 SFVec4f.h CommandList.h Command.h DuneApp.h URL.h PreferencesApp.h \
 swt/include/swttypedef.h EcmaScriptApp.h StereoViewApp.h \
 swt/include/stereoview.h InputDeviceApp.h TransformMode.h OutputApp.h \
 RouteViewApp.h StartWithApp.h ExportNamesApp.h ModellingApp.h \
 RenderApp.h Field.h
NodeLocalFog.o: NodeLocalFog.cpp stdafx.h config.h mysnprintf.h \
 NodeLocalFog.h Node.h Array.h List.h Vec3f.h Vec3x.h MyString.h \
 StringArray.h FieldIndex.h Element.h x3dFlags.h swt/include/WriteFlags.h \
 NodeFlags.h MeshFlags.h ac3dMaterialCallback.h NodeArray.h Stack.h \
 Matrix.h Vec3d.h Quaternion.h ProtoMacros.h Proto.h Map.h NodeList.h \
 InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h \
 Arrays.h MySwap.h Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h \
 MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h \
 MFRotation.h MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h \
 MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h \
 SFImage.h SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h \
 SFString.h SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h \
 SFVec4f.h CommandList.h Command.h FogMacros.h DuneApp.h URL.h \
 PreferencesApp.h swt/include/swttypedef.h EcmaScriptApp.h \
 StereoViewApp.h swt/include/stereoview.h InputDeviceApp.h \
 TransformMode.h OutputApp.h RouteViewApp.h StartWithApp.h \
 ExportNamesApp.h ModellingApp.h RenderApp.h
NodeLOD.o: NodeLOD.cpp stdafx.h config.h mysnprintf.h NodeLOD.h Node.h \
 Array.h List.h Vec3f.h Vec3x.h MyString.h StringArray.h FieldIndex.h \
 Element.h x3dFlags.h swt/include/WriteFlags.h NodeFlags.h MeshFlags.h \
 ac3dMaterialCallback.h NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h \
 ProtoMacros.h Proto.h Map.h NodeList.h InterfaceArray.h \
 DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h Arrays.h MySwap.h \
 Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h MFDouble.h \
 SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h MFRotation.h \
 MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h \
 MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h SFImage.h \
 SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h SFString.h \
 SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h \
 CommandList.h Command.h Scene.h URL.h DuneApp.h PreferencesApp.h \
 swt/include/swttypedef.h EcmaScriptApp.h StereoViewApp.h \
 swt/include/stereoview.h InputDeviceApp.h TransformMode.h OutputApp.h \
 RouteViewApp.h StartWithApp.h ExportNamesApp.h ModellingApp.h \
 RenderApp.h RenderState.h ProtoMap.h
NodeMaterial.o: NodeMaterial.cpp stdafx.h config.h mysnprintf.h \
 NodeMaterial.h Node.h Array.h List.h Vec3f.h Vec3x.h MyString.h \
 StringArray.h FieldIndex.h Element.h x3dFlags.h swt/include/WriteFlags.h \
 NodeFlags.h MeshFlags.h ac3dMaterialCallback.h NodeArray.h Stack.h \
 Matrix.h Vec3d.h Quaternion.h ProtoMacros.h Proto.h Map.h NodeList.h \
 InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h \
 Arrays.h MySwap.h Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h \
 MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h \
 MFRotation.h MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h \
 MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h \
 SFImage.h SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h \
 SFString.h SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h \
 SFVec4f.h CommandList.h Command.h swt/include/swt.h \
 swt/include/keycodes.h swt/include/swlanguages.h swt/include/swDebugf.h \
 swt/include/swttypedef.h ExposedField.h Field.h Util.h Scene.h URL.h \
 DuneApp.h PreferencesApp.h swt/include/swttypedef.h EcmaScriptApp.h \
 StereoViewApp.h swt/include/stereoview.h InputDeviceApp.h \
 TransformMode.h OutputApp.h RouteViewApp.h StartWithApp.h \
 ExportNamesApp.h ModellingApp.h RenderApp.h RenderState.h ProtoMap.h \
 LdrawDefines.h resource.h
NodeMatrix3VertexAttribute.o: NodeMatrix3VertexAttribute.cpp stdafx.h \
 config.h mysnprintf.h NodeMatrix3VertexAttribute.h Node.h Array.h List.h \
 Vec3f.h Vec3x.h MyString.h StringArray.h FieldIndex.h Element.h \
 x3dFlags.h swt/include/WriteFlags.h NodeFlags.h MeshFlags.h \
 ac3dMaterialCallback.h NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h \
 ProtoMacros.h Proto.h Map.h NodeList.h InterfaceArray.h \
 DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h Arrays.h MySwap.h \
 Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h MFDouble.h \
 SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h MFRotation.h \
 MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h \
 MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h SFImage.h \
 SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h SFString.h \
 SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h \
 CommandList.h Command.h DuneApp.h URL.h PreferencesApp.h \
 swt/include/swttypedef.h EcmaScriptApp.h StereoViewApp.h \
 swt/include/stereoview.h InputDeviceApp.h TransformMode.h OutputApp.h \
 RouteViewApp.h StartWithApp.h ExportNamesApp.h ModellingApp.h \
 RenderApp.h
NodeMatrix4VertexAttribute.o: NodeMatrix4VertexAttribute.cpp stdafx.h \
 config.h mysnprintf.h NodeMatrix4VertexAttribute.h Node.h Array.h List.h \
 Vec3f.h Vec3x.h MyString.h StringArray.h FieldIndex.h Element.h \
 x3dFlags.h swt/include/WriteFlags.h NodeFlags.h MeshFlags.h \
 ac3dMaterialCallback.h NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h \
 ProtoMacros.h Proto.h Map.h NodeList.h InterfaceArray.h \
 DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h Arrays.h MySwap.h \
 Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h MFDouble.h \
 SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h MFRotation.h \
 MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h \
 MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h SFImage.h \
 SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h SFString.h \
 SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h \
 CommandList.h Command.h DuneApp.h URL.h PreferencesApp.h \
 swt/include/swttypedef.h EcmaScriptApp.h StereoViewApp.h \
 swt/include/stereoview.h InputDeviceApp.h TransformMode.h OutputApp.h \
 RouteViewApp.h StartWithApp.h ExportNamesApp.h ModellingApp.h \
 RenderApp.h
NodeMetadataBoolean.o: NodeMetadataBoolean.cpp stdafx.h config.h \
 mysnprintf.h NodeMetadataBoolean.h Node.h Array.h List.h Vec3f.h Vec3x.h \
 MyString.h StringArray.h FieldIndex.h Element.h x3dFlags.h \
 swt/include/WriteFlags.h NodeFlags.h MeshFlags.h ac3dMaterialCallback.h \
 NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h ProtoMacros.h Proto.h \
 Map.h NodeList.h InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h \
 MFBool.h FieldValue.h Arrays.h MySwap.h Types.h MFColor.h MFFloat.h \
 SFFloat.h MFColorRGBA.h MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h \
 MFMatrix4f.h MFNode.h MFRotation.h MFString.h MFTime.h SFTime.h \
 MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h \
 SFBool.h SFColor.h SFColorRGBA.h SFImage.h SFInt32.h SFMatrix3f.h \
 SFMatrix4f.h SFNode.h SFRotation.h SFString.h SFVec2d.h SFVec2f.h \
 Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h CommandList.h Command.h \
 DuneApp.h URL.h PreferencesApp.h swt/include/swttypedef.h \
 EcmaScriptApp.h StereoViewApp.h swt/include/stereoview.h \
 InputDeviceApp.h TransformMode.h OutputApp.h RouteViewApp.h \
 StartWithApp.h ExportNamesApp.h ModellingApp.h RenderApp.h
NodeMetadataDouble.o: NodeMetadataDouble.cpp NodeMetadataDouble.h \
 Metadata.h stdafx.h config.h mysnprintf.h Node.h Array.h List.h Vec3f.h \
 Vec3x.h MyString.h StringArray.h FieldIndex.h Element.h x3dFlags.h \
 swt/include/WriteFlags.h NodeFlags.h MeshFlags.h ac3dMaterialCallback.h \
 NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h Proto.h Map.h \
 ProtoMacros.h NodeList.h InterfaceArray.h DynamicFieldsNode.h \
 SFMFTypes.h MFBool.h FieldValue.h Arrays.h MySwap.h Types.h MFColor.h \
 MFFloat.h SFFloat.h MFColorRGBA.h MFDouble.h SFDouble.h MFInt32.h \
 MFMatrix3f.h MFMatrix4f.h MFNode.h MFRotation.h MFString.h MFTime.h \
 SFTime.h MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h MFVec3f.h MFVec4f.h \
 Vec4f.h SFBool.h SFColor.h SFColorRGBA.h SFImage.h SFInt32.h \
 SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h SFString.h SFVec2d.h \
 SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h CommandList.h \
 Command.h
NodeMetadataFloat.o: NodeMetadataFloat.cpp NodeMetadataFloat.h Metadata.h \
 stdafx.h config.h mysnprintf.h Node.h Array.h List.h Vec3f.h Vec3x.h \
 MyString.h StringArray.h FieldIndex.h Element.h x3dFlags.h \
 swt/include/WriteFlags.h NodeFlags.h MeshFlags.h ac3dMaterialCallback.h \
 NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h Proto.h Map.h \
 ProtoMacros.h NodeList.h InterfaceArray.h DynamicFieldsNode.h \
 SFMFTypes.h MFBool.h FieldValue.h Arrays.h MySwap.h Types.h MFColor.h \
 MFFloat.h SFFloat.h MFColorRGBA.h MFDouble.h SFDouble.h MFInt32.h \
 MFMatrix3f.h MFMatrix4f.h MFNode.h MFRotation.h MFString.h MFTime.h \
 SFTime.h MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h MFVec3f.h MFVec4f.h \
 Vec4f.h SFBool.h SFColor.h SFColorRGBA.h SFImage.h SFInt32.h \
 SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h SFString.h SFVec2d.h \
 SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h CommandList.h \
 Command.h
NodeMetadataInteger.o: NodeMetadataInteger.cpp stdafx.h config.h \
 mysnprintf.h NodeMetadataInteger.h Metadata.h Node.h Array.h List.h \
 Vec3f.h Vec3x.h MyString.h StringArray.h FieldIndex.h Element.h \
 x3dFlags.h swt/include/WriteFlags.h NodeFlags.h MeshFlags.h \
 ac3dMaterialCallback.h NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h \
 Proto.h Map.h ProtoMacros.h NodeList.h InterfaceArray.h \
 DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h Arrays.h MySwap.h \
 Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h MFDouble.h \
 SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h MFRotation.h \
 MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h \
 MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h SFImage.h \
 SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h SFString.h \
 SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h \
 CommandList.h Command.h
NodeMetadataSet.o: NodeMetadataSet.cpp stdafx.h config.h mysnprintf.h \
 NodeMetadataSet.h Metadata.h Node.h Array.h List.h Vec3f.h Vec3x.h \
 MyString.h StringArray.h FieldIndex.h Element.h x3dFlags.h \
 swt/include/WriteFlags.h NodeFlags.h MeshFlags.h ac3dMaterialCallback.h \
 NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h Proto.h Map.h \
 ProtoMacros.h NodeList.h InterfaceArray.h DynamicFieldsNode.h \
 SFMFTypes.h MFBool.h FieldValue.h Arrays.h MySwap.h Types.h MFColor.h \
 MFFloat.h SFFloat.h MFColorRGBA.h MFDouble.h SFDouble.h MFInt32.h \
 MFMatrix3f.h MFMatrix4f.h MFNode.h MFRotation.h MFString.h MFTime.h \
 SFTime.h MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h MFVec3f.h MFVec4f.h \
 Vec4f.h SFBool.h SFColor.h SFColorRGBA.h SFImage.h SFInt32.h \
 SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h SFString.h SFVec2d.h \
 SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h CommandList.h \
 Command.h
NodeMetadataString.o: NodeMetadataString.cpp stdafx.h config.h \
 mysnprintf.h NodeMetadataString.h Metadata.h Node.h Array.h List.h \
 Vec3f.h Vec3x.h MyString.h StringArray.h FieldIndex.h Element.h \
 x3dFlags.h swt/include/WriteFlags.h NodeFlags.h MeshFlags.h \
 ac3dMaterialCallback.h NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h \
 Proto.h Map.h ProtoMacros.h NodeList.h InterfaceArray.h \
 DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h Arrays.h MySwap.h \
 Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h MFDouble.h \
 SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h MFRotation.h \
 MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h \
 MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h SFImage.h \
 SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h SFString.h \
 SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h \
 CommandList.h Command.h
NodeMotorJoint.o: NodeMotorJoint.cpp stdafx.h config.h mysnprintf.h \
 NodeMotorJoint.h X3DRigidJointNode.h RigidBodyPhysicsNode.h Node.h \
 Array.h List.h Vec3f.h Vec3x.h MyString.h StringArray.h FieldIndex.h \
 Element.h x3dFlags.h swt/include/WriteFlags.h NodeFlags.h MeshFlags.h \
 ac3dMaterialCallback.h NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h \
 ProtoMacros.h Proto.h Map.h NodeList.h InterfaceArray.h \
 DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h Arrays.h MySwap.h \
 Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h MFDouble.h \
 SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h MFRotation.h \
 MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h \
 MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h SFImage.h \
 SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h SFString.h \
 SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h \
 CommandList.h Command.h Field.h DuneApp.h URL.h PreferencesApp.h \
 swt/include/swttypedef.h EcmaScriptApp.h StereoViewApp.h \
 swt/include/stereoview.h InputDeviceApp.h TransformMode.h OutputApp.h \
 RouteViewApp.h StartWithApp.h ExportNamesApp.h ModellingApp.h \
 RenderApp.h Scene.h RenderState.h ProtoMap.h
NodeMultiTexture.o: NodeMultiTexture.cpp stdafx.h config.h mysnprintf.h \
 NodeMultiTexture.h Node.h Array.h List.h Vec3f.h Vec3x.h MyString.h \
 StringArray.h FieldIndex.h Element.h x3dFlags.h swt/include/WriteFlags.h \
 NodeFlags.h MeshFlags.h ac3dMaterialCallback.h NodeArray.h Stack.h \
 Matrix.h Vec3d.h Quaternion.h ProtoMacros.h Proto.h Map.h NodeList.h \
 InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h \
 Arrays.h MySwap.h Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h \
 MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h \
 MFRotation.h MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h \
 MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h \
 SFImage.h SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h \
 SFString.h SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h \
 SFVec4f.h CommandList.h Command.h KambiTextureCommonFields.h Util.h \
 swt/include/swt.h swt/include/keycodes.h swt/include/swlanguages.h \
 swt/include/swDebugf.h swt/include/swttypedef.h MyMesh.h \
 NodeIndexedFaceSet.h MeshBasedNode.h GeometryNode.h NodeAppearance.h \
 DuneApp.h URL.h PreferencesApp.h swt/include/swttypedef.h \
 EcmaScriptApp.h StereoViewApp.h swt/include/stereoview.h \
 InputDeviceApp.h TransformMode.h OutputApp.h RouteViewApp.h \
 StartWithApp.h ExportNamesApp.h ModellingApp.h RenderApp.h \
 NodeMaterial.h NodeImageTexture.h NodeTextureTransform.h \
 GeometryNodeMacros.h NodeCoordinate.h Colored.h NodeColor.h \
 NodeColorRGBA.h ComposedGeometryMacros.h FaceData.h NodeNormal.h \
 NodeTextureCoordinate.h NodeTextureCoordinateGenerator.h \
 NodeMultiTextureCoordinate.h Scene.h RenderState.h ProtoMap.h \
 NodeShape.h
NodeMultiTextureCoordinate.o: NodeMultiTextureCoordinate.cpp stdafx.h \
 config.h mysnprintf.h NodeMultiTextureCoordinate.h Node.h Array.h List.h \
 Vec3f.h Vec3x.h MyString.h StringArray.h FieldIndex.h Element.h \
 x3dFlags.h swt/include/WriteFlags.h NodeFlags.h MeshFlags.h \
 ac3dMaterialCallback.h NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h \
 ProtoMacros.h Proto.h Map.h NodeList.h InterfaceArray.h \
 DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h Arrays.h MySwap.h \
 Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h MFDouble.h \
 SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h MFRotation.h \
 MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h \
 MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h SFImage.h \
 SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h SFString.h \
 SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h \
 CommandList.h Command.h MeshBasedNode.h GeometryNode.h NodeAppearance.h \
 DuneApp.h URL.h PreferencesApp.h swt/include/swttypedef.h \
 EcmaScriptApp.h StereoViewApp.h swt/include/stereoview.h \
 InputDeviceApp.h TransformMode.h OutputApp.h RouteViewApp.h \
 StartWithApp.h ExportNamesApp.h ModellingApp.h RenderApp.h \
 NodeMaterial.h swt/include/swt.h swt/include/keycodes.h \
 swt/include/swlanguages.h swt/include/swDebugf.h \
 swt/include/swttypedef.h NodeImageTexture.h KambiTextureCommonFields.h \
 NodeTextureTransform.h GeometryNodeMacros.h Util.h
NodeMultiTextureTransform.o: NodeMultiTextureTransform.cpp stdafx.h \
 config.h mysnprintf.h NodeMultiTextureTransform.h Node.h Array.h List.h \
 Vec3f.h Vec3x.h MyString.h StringArray.h FieldIndex.h Element.h \
 x3dFlags.h swt/include/WriteFlags.h NodeFlags.h MeshFlags.h \
 ac3dMaterialCallback.h NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h \
 ProtoMacros.h Proto.h Map.h NodeList.h InterfaceArray.h \
 DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h Arrays.h MySwap.h \
 Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h MFDouble.h \
 SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h MFRotation.h \
 MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h \
 MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h SFImage.h \
 SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h SFString.h \
 SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h \
 CommandList.h Command.h
NodeNavigationInfo.o: NodeNavigationInfo.cpp stdafx.h config.h \
 mysnprintf.h NodeNavigationInfo.h Node.h Array.h List.h Vec3f.h Vec3x.h \
 MyString.h StringArray.h FieldIndex.h Element.h x3dFlags.h \
 swt/include/WriteFlags.h NodeFlags.h MeshFlags.h ac3dMaterialCallback.h \
 NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h ProtoMacros.h Proto.h \
 Map.h NodeList.h InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h \
 MFBool.h FieldValue.h Arrays.h MySwap.h Types.h MFColor.h MFFloat.h \
 SFFloat.h MFColorRGBA.h MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h \
 MFMatrix4f.h MFNode.h MFRotation.h MFString.h MFTime.h SFTime.h \
 MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h \
 SFBool.h SFColor.h SFColorRGBA.h SFImage.h SFInt32.h SFMatrix3f.h \
 SFMatrix4f.h SFNode.h SFRotation.h SFString.h SFVec2d.h SFVec2f.h \
 Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h CommandList.h Command.h \
 Scene.h URL.h DuneApp.h PreferencesApp.h swt/include/swttypedef.h \
 EcmaScriptApp.h StereoViewApp.h swt/include/stereoview.h \
 InputDeviceApp.h TransformMode.h OutputApp.h RouteViewApp.h \
 StartWithApp.h ExportNamesApp.h ModellingApp.h RenderApp.h RenderState.h \
 ProtoMap.h Field.h
NodeNormal.o: NodeNormal.cpp stdafx.h config.h mysnprintf.h NodeNormal.h \
 Node.h Array.h List.h Vec3f.h Vec3x.h MyString.h StringArray.h \
 FieldIndex.h Element.h x3dFlags.h swt/include/WriteFlags.h NodeFlags.h \
 MeshFlags.h ac3dMaterialCallback.h NodeArray.h Stack.h Matrix.h Vec3d.h \
 Quaternion.h ProtoMacros.h Proto.h Map.h NodeList.h InterfaceArray.h \
 DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h Arrays.h MySwap.h \
 Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h MFDouble.h \
 SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h MFRotation.h \
 MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h \
 MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h SFImage.h \
 SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h SFString.h \
 SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h \
 CommandList.h Command.h NodeIndexedFaceSet.h MeshBasedNode.h \
 GeometryNode.h NodeAppearance.h DuneApp.h URL.h PreferencesApp.h \
 swt/include/swttypedef.h EcmaScriptApp.h StereoViewApp.h \
 swt/include/stereoview.h InputDeviceApp.h TransformMode.h OutputApp.h \
 RouteViewApp.h StartWithApp.h ExportNamesApp.h ModellingApp.h \
 RenderApp.h NodeMaterial.h swt/include/swt.h swt/include/keycodes.h \
 swt/include/swlanguages.h swt/include/swDebugf.h \
 swt/include/swttypedef.h NodeImageTexture.h KambiTextureCommonFields.h \
 NodeTextureTransform.h GeometryNodeMacros.h NodeCoordinate.h Colored.h \
 NodeColor.h NodeColorRGBA.h ComposedGeometryMacros.h
NodeNormalInterpolator.o: NodeNormalInterpolator.cpp stdafx.h config.h \
 mysnprintf.h NodeNormalInterpolator.h Interpolator.h Node.h Array.h \
 List.h Vec3f.h Vec3x.h MyString.h StringArray.h FieldIndex.h Element.h \
 x3dFlags.h swt/include/WriteFlags.h NodeFlags.h MeshFlags.h \
 ac3dMaterialCallback.h NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h \
 ProtoMacros.h Proto.h Map.h NodeList.h InterfaceArray.h \
 DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h Arrays.h MySwap.h \
 Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h MFDouble.h \
 SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h MFRotation.h \
 MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h \
 MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h SFImage.h \
 SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h SFString.h \
 SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h \
 CommandList.h Command.h
NodeNurbsCurve.o: NodeNurbsCurve.cpp stdafx.h config.h mysnprintf.h \
 NodeNurbsCurve.h NurbsCurve.h GeometryNode.h Node.h Array.h List.h \
 Vec3f.h Vec3x.h MyString.h StringArray.h FieldIndex.h Element.h \
 x3dFlags.h swt/include/WriteFlags.h NodeFlags.h MeshFlags.h \
 ac3dMaterialCallback.h NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h \
 NodeAppearance.h ProtoMacros.h Proto.h Map.h NodeList.h InterfaceArray.h \
 DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h Arrays.h MySwap.h \
 Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h MFDouble.h \
 SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h MFRotation.h \
 MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h \
 MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h SFImage.h \
 SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h SFString.h \
 SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h \
 CommandList.h Command.h DuneApp.h URL.h PreferencesApp.h \
 swt/include/swttypedef.h EcmaScriptApp.h StereoViewApp.h \
 swt/include/stereoview.h InputDeviceApp.h TransformMode.h OutputApp.h \
 RouteViewApp.h StartWithApp.h ExportNamesApp.h ModellingApp.h \
 RenderApp.h NodeMaterial.h swt/include/swt.h swt/include/keycodes.h \
 swt/include/swlanguages.h swt/include/swDebugf.h \
 swt/include/swttypedef.h NodeImageTexture.h KambiTextureCommonFields.h \
 NodeTextureTransform.h GeometryNodeMacros.h ChainBasedNode.h Scene.h \
 RenderState.h ProtoMap.h MoveCommand.h NodeCoordinate.h NodeNormal.h \
 NodeTextureCoordinate.h NodeNurbsSurface.h MeshMorphingNode.h \
 Interpolator.h MeshBasedNode.h NurbsMakeRevolvedSurface.h \
 NurbsCurveDegreeElevate.h Util.h NodeIndexedLineSet.h Colored.h \
 NodeColor.h NodeColorRGBA.h NodePositionInterpolator.h \
 NodeOrientationInterpolator.h NodeCurveAnimation.h NodeSuperExtrusion.h \
 NodeExtrusion.h NodeSuperRevolver.h NodeNurbsPositionInterpolator.h \
 NodeNurbsOrientationInterpolator.h NodeCoordinateDouble.h \
 NodeCoordinateInterpolator.h EventIn.h EventOut.h resource.h
NodeNurbsCurve2D.o: NodeNurbsCurve2D.cpp resource.h stdafx.h config.h \
 mysnprintf.h NodeNurbsCurve2D.h Node.h Array.h List.h Vec3f.h Vec3x.h \
 MyString.h StringArray.h FieldIndex.h Element.h x3dFlags.h \
 swt/include/WriteFlags.h NodeFlags.h MeshFlags.h ac3dMaterialCallback.h \
 NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h ProtoMacros.h Proto.h \
 Map.h NodeList.h InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h \
 MFBool.h FieldValue.h Arrays.h MySwap.h Types.h MFColor.h MFFloat.h \
 SFFloat.h MFColorRGBA.h MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h \
 MFMatrix4f.h MFNode.h MFRotation.h MFString.h MFTime.h SFTime.h \
 MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h \
 SFBool.h SFColor.h SFColorRGBA.h SFImage.h SFInt32.h SFMatrix3f.h \
 SFMatrix4f.h SFNode.h SFRotation.h SFString.h SFVec2d.h SFVec2f.h \
 Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h CommandList.h Command.h \
 Scene.h URL.h DuneApp.h PreferencesApp.h swt/include/swttypedef.h \
 EcmaScriptApp.h StereoViewApp.h swt/include/stereoview.h \
 InputDeviceApp.h TransformMode.h OutputApp.h RouteViewApp.h \
 StartWithApp.h ExportNamesApp.h ModellingApp.h RenderApp.h RenderState.h \
 ProtoMap.h NodeNurbsCurve.h NurbsCurve.h GeometryNode.h NodeAppearance.h \
 NodeMaterial.h swt/include/swt.h swt/include/keycodes.h \
 swt/include/swlanguages.h swt/include/swDebugf.h \
 swt/include/swttypedef.h NodeImageTexture.h KambiTextureCommonFields.h \
 NodeTextureTransform.h GeometryNodeMacros.h ChainBasedNode.h Util.h
NodeNurbsGroup.o: NodeNurbsGroup.cpp stdafx.h config.h mysnprintf.h \
 NodeNurbsGroup.h Node.h Array.h List.h Vec3f.h Vec3x.h MyString.h \
 StringArray.h FieldIndex.h Element.h x3dFlags.h swt/include/WriteFlags.h \
 NodeFlags.h MeshFlags.h ac3dMaterialCallback.h NodeArray.h Stack.h \
 Matrix.h Vec3d.h Quaternion.h ProtoMacros.h Proto.h Map.h NodeList.h \
 InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h \
 Arrays.h MySwap.h Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h \
 MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h \
 MFRotation.h MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h \
 MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h \
 SFImage.h SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h \
 SFString.h SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h \
 SFVec4f.h CommandList.h Command.h NodeGroup.h StaticGroupNode.h \
 GroupNode.h Field.h MoveCommand.h DuneApp.h URL.h PreferencesApp.h \
 swt/include/swttypedef.h EcmaScriptApp.h StereoViewApp.h \
 swt/include/stereoview.h InputDeviceApp.h TransformMode.h OutputApp.h \
 RouteViewApp.h StartWithApp.h ExportNamesApp.h ModellingApp.h \
 RenderApp.h NodeNurbsSurface.h MeshMorphingNode.h Interpolator.h \
 MeshBasedNode.h GeometryNode.h NodeAppearance.h NodeMaterial.h \
 swt/include/swt.h swt/include/keycodes.h swt/include/swlanguages.h \
 swt/include/swDebugf.h swt/include/swttypedef.h NodeImageTexture.h \
 KambiTextureCommonFields.h NodeTextureTransform.h GeometryNodeMacros.h \
 NodeShape.h Scene.h RenderState.h ProtoMap.h NodeNurbsSet.h
NodeNurbsOrientationInterpolator.o: NodeNurbsOrientationInterpolator.cpp \
 stdafx.h config.h mysnprintf.h NodeNurbsOrientationInterpolator.h \
 ProtoMacros.h Proto.h Array.h Map.h List.h MyString.h StringArray.h \
 Node.h Vec3f.h Vec3x.h FieldIndex.h Element.h x3dFlags.h \
 swt/include/WriteFlags.h NodeFlags.h MeshFlags.h ac3dMaterialCallback.h \
 NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h NodeList.h \
 InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h \
 Arrays.h MySwap.h Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h \
 MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h \
 MFRotation.h MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h \
 MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h \
 SFImage.h SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h \
 SFString.h SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h \
 SFVec4f.h CommandList.h Command.h NodeNurbsCurve.h NurbsCurve.h \
 GeometryNode.h NodeAppearance.h DuneApp.h URL.h PreferencesApp.h \
 swt/include/swttypedef.h EcmaScriptApp.h StereoViewApp.h \
 swt/include/stereoview.h InputDeviceApp.h TransformMode.h OutputApp.h \
 RouteViewApp.h StartWithApp.h ExportNamesApp.h ModellingApp.h \
 RenderApp.h NodeMaterial.h swt/include/swt.h swt/include/keycodes.h \
 swt/include/swlanguages.h swt/include/swDebugf.h \
 swt/include/swttypedef.h NodeImageTexture.h KambiTextureCommonFields.h \
 NodeTextureTransform.h GeometryNodeMacros.h ChainBasedNode.h Scene.h \
 RenderState.h ProtoMap.h NodeCoordinate.h Util.h
NodeNurbsPositionInterpolator.o: NodeNurbsPositionInterpolator.cpp \
 stdafx.h config.h mysnprintf.h NodeNurbsPositionInterpolator.h \
 ProtoMacros.h Proto.h Array.h Map.h List.h MyString.h StringArray.h \
 Node.h Vec3f.h Vec3x.h FieldIndex.h Element.h x3dFlags.h \
 swt/include/WriteFlags.h NodeFlags.h MeshFlags.h ac3dMaterialCallback.h \
 NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h NodeList.h \
 InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h \
 Arrays.h MySwap.h Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h \
 MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h \
 MFRotation.h MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h \
 MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h \
 SFImage.h SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h \
 SFString.h SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h \
 SFVec4f.h CommandList.h Command.h NurbsCurve.h GeometryNode.h \
 NodeAppearance.h DuneApp.h URL.h PreferencesApp.h \
 swt/include/swttypedef.h EcmaScriptApp.h StereoViewApp.h \
 swt/include/stereoview.h InputDeviceApp.h TransformMode.h OutputApp.h \
 RouteViewApp.h StartWithApp.h ExportNamesApp.h ModellingApp.h \
 RenderApp.h NodeMaterial.h swt/include/swt.h swt/include/keycodes.h \
 swt/include/swlanguages.h swt/include/swDebugf.h \
 swt/include/swttypedef.h NodeImageTexture.h KambiTextureCommonFields.h \
 NodeTextureTransform.h GeometryNodeMacros.h ChainBasedNode.h \
 NodeNurbsCurve.h NodeCoordinate.h Scene.h RenderState.h ProtoMap.h \
 Field.h MoveCommand.h Util.h
NodeNurbsSet.o: NodeNurbsSet.cpp stdafx.h config.h mysnprintf.h \
 NodeNurbsSet.h Node.h Array.h List.h Vec3f.h Vec3x.h MyString.h \
 StringArray.h FieldIndex.h Element.h x3dFlags.h swt/include/WriteFlags.h \
 NodeFlags.h MeshFlags.h ac3dMaterialCallback.h NodeArray.h Stack.h \
 Matrix.h Vec3d.h Quaternion.h ProtoMacros.h Proto.h Map.h NodeList.h \
 InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h \
 Arrays.h MySwap.h Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h \
 MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h \
 MFRotation.h MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h \
 MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h \
 SFImage.h SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h \
 SFString.h SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h \
 SFVec4f.h CommandList.h Command.h Field.h MoveCommand.h DuneApp.h URL.h \
 PreferencesApp.h swt/include/swttypedef.h EcmaScriptApp.h \
 StereoViewApp.h swt/include/stereoview.h InputDeviceApp.h \
 TransformMode.h OutputApp.h RouteViewApp.h StartWithApp.h \
 ExportNamesApp.h ModellingApp.h RenderApp.h NodeNurbsSurface.h \
 MeshMorphingNode.h Interpolator.h MeshBasedNode.h GeometryNode.h \
 NodeAppearance.h NodeMaterial.h swt/include/swt.h swt/include/keycodes.h \
 swt/include/swlanguages.h swt/include/swDebugf.h \
 swt/include/swttypedef.h NodeImageTexture.h KambiTextureCommonFields.h \
 NodeTextureTransform.h GeometryNodeMacros.h NodeShape.h Scene.h \
 RenderState.h ProtoMap.h NodeNurbsGroup.h NodeGroup.h StaticGroupNode.h \
 GroupNode.h
NodeNurbsSurface.o: NodeNurbsSurface.cpp stdafx.h config.h mysnprintf.h \
 NodeNurbsSurface.h MeshMorphingNode.h Array.h Interpolator.h Node.h \
 List.h Vec3f.h Vec3x.h MyString.h StringArray.h FieldIndex.h Element.h \
 x3dFlags.h swt/include/WriteFlags.h NodeFlags.h MeshFlags.h \
 ac3dMaterialCallback.h NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h \
 ProtoMacros.h Proto.h Map.h NodeList.h InterfaceArray.h \
 DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h Arrays.h MySwap.h \
 Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h MFDouble.h \
 SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h MFRotation.h \
 MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h \
 MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h SFImage.h \
 SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h SFString.h \
 SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h \
 CommandList.h Command.h MeshBasedNode.h GeometryNode.h NodeAppearance.h \
 DuneApp.h URL.h PreferencesApp.h swt/include/swttypedef.h \
 EcmaScriptApp.h StereoViewApp.h swt/include/stereoview.h \
 InputDeviceApp.h TransformMode.h OutputApp.h RouteViewApp.h \
 StartWithApp.h ExportNamesApp.h ModellingApp.h RenderApp.h \
 NodeMaterial.h swt/include/swt.h swt/include/keycodes.h \
 swt/include/swlanguages.h swt/include/swDebugf.h \
 swt/include/swttypedef.h NodeImageTexture.h KambiTextureCommonFields.h \
 NodeTextureTransform.h GeometryNodeMacros.h Scene.h RenderState.h \
 ProtoMap.h MyMesh.h NodeIndexedFaceSet.h NodeCoordinate.h Colored.h \
 NodeColor.h NodeColorRGBA.h ComposedGeometryMacros.h FaceData.h \
 NodeNormal.h NodeTextureCoordinate.h NodeTextureCoordinateGenerator.h \
 NodeMultiTextureCoordinate.h Util.h NodeNurbsGroup.h NodeGroup.h \
 StaticGroupNode.h GroupNode.h NodeNurbsSet.h NurbsSurfaceDegreeElevate.h \
 Field.h ExposedField.h MoveCommand.h NodeNurbsTrimmedSurface.h \
 NodeCoordinateDouble.h NodeNurbsTextureCoordinate.h resource.h
NodeNurbsSurfaceInterpolator.o: NodeNurbsSurfaceInterpolator.cpp stdafx.h \
 config.h mysnprintf.h NodeNurbsSurfaceInterpolator.h MeshMorphingNode.h \
 Array.h Interpolator.h Node.h List.h Vec3f.h Vec3x.h MyString.h \
 StringArray.h FieldIndex.h Element.h x3dFlags.h swt/include/WriteFlags.h \
 NodeFlags.h MeshFlags.h ac3dMaterialCallback.h NodeArray.h Stack.h \
 Matrix.h Vec3d.h Quaternion.h ProtoMacros.h Proto.h Map.h NodeList.h \
 InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h \
 Arrays.h MySwap.h Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h \
 MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h \
 MFRotation.h MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h \
 MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h \
 SFImage.h SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h \
 SFString.h SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h \
 SFVec4f.h CommandList.h Command.h MeshBasedNode.h GeometryNode.h \
 NodeAppearance.h DuneApp.h URL.h PreferencesApp.h \
 swt/include/swttypedef.h EcmaScriptApp.h StereoViewApp.h \
 swt/include/stereoview.h InputDeviceApp.h TransformMode.h OutputApp.h \
 RouteViewApp.h StartWithApp.h ExportNamesApp.h ModellingApp.h \
 RenderApp.h NodeMaterial.h swt/include/swt.h swt/include/keycodes.h \
 swt/include/swlanguages.h swt/include/swDebugf.h \
 swt/include/swttypedef.h NodeImageTexture.h KambiTextureCommonFields.h \
 NodeTextureTransform.h GeometryNodeMacros.h Scene.h RenderState.h \
 ProtoMap.h MyMesh.h NodeIndexedFaceSet.h NodeCoordinate.h Colored.h \
 NodeColor.h NodeColorRGBA.h ComposedGeometryMacros.h FaceData.h \
 NodeNormal.h NodeTextureCoordinate.h NodeTextureCoordinateGenerator.h \
 NodeMultiTextureCoordinate.h Util.h NodeNurbsGroup.h NodeGroup.h \
 StaticGroupNode.h GroupNode.h NurbsSurfaceDegreeElevate.h Field.h \
 ExposedField.h MoveCommand.h
NodeNurbsSweptSurface.o: NodeNurbsSweptSurface.cpp stdafx.h config.h \
 mysnprintf.h NodeNurbsSweptSurface.h MeshMorphingNode.h Array.h \
 Interpolator.h Node.h List.h Vec3f.h Vec3x.h MyString.h StringArray.h \
 FieldIndex.h Element.h x3dFlags.h swt/include/WriteFlags.h NodeFlags.h \
 MeshFlags.h ac3dMaterialCallback.h NodeArray.h Stack.h Matrix.h Vec3d.h \
 Quaternion.h ProtoMacros.h Proto.h Map.h NodeList.h InterfaceArray.h \
 DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h Arrays.h MySwap.h \
 Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h MFDouble.h \
 SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h MFRotation.h \
 MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h \
 MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h SFImage.h \
 SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h SFString.h \
 SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h \
 CommandList.h Command.h MeshBasedNode.h GeometryNode.h NodeAppearance.h \
 DuneApp.h URL.h PreferencesApp.h swt/include/swttypedef.h \
 EcmaScriptApp.h StereoViewApp.h swt/include/stereoview.h \
 InputDeviceApp.h TransformMode.h OutputApp.h RouteViewApp.h \
 StartWithApp.h ExportNamesApp.h ModellingApp.h RenderApp.h \
 NodeMaterial.h swt/include/swt.h swt/include/keycodes.h \
 swt/include/swlanguages.h swt/include/swDebugf.h \
 swt/include/swttypedef.h NodeImageTexture.h KambiTextureCommonFields.h \
 NodeTextureTransform.h GeometryNodeMacros.h NodeExtrusion.h \
 NodeNurbsCurve.h NurbsCurve.h ChainBasedNode.h Scene.h RenderState.h \
 ProtoMap.h MyMesh.h NodeIndexedFaceSet.h NodeCoordinate.h Colored.h \
 NodeColor.h NodeColorRGBA.h ComposedGeometryMacros.h FaceData.h \
 NodeNormal.h NodeTextureCoordinate.h NodeTextureCoordinateGenerator.h \
 NodeMultiTextureCoordinate.h Util.h NodeNurbsCurve2D.h \
 NurbsSurfaceDegreeElevate.h Field.h ExposedField.h MoveCommand.h
NodeNurbsSwungSurface.o: NodeNurbsSwungSurface.cpp stdafx.h config.h \
 mysnprintf.h NodeNurbsSwungSurface.h MeshMorphingNode.h Array.h \
 Interpolator.h Node.h List.h Vec3f.h Vec3x.h MyString.h StringArray.h \
 FieldIndex.h Element.h x3dFlags.h swt/include/WriteFlags.h NodeFlags.h \
 MeshFlags.h ac3dMaterialCallback.h NodeArray.h Stack.h Matrix.h Vec3d.h \
 Quaternion.h ProtoMacros.h Proto.h Map.h NodeList.h InterfaceArray.h \
 DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h Arrays.h MySwap.h \
 Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h MFDouble.h \
 SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h MFRotation.h \
 MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h \
 MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h SFImage.h \
 SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h SFString.h \
 SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h \
 CommandList.h Command.h MeshBasedNode.h GeometryNode.h NodeAppearance.h \
 DuneApp.h URL.h PreferencesApp.h swt/include/swttypedef.h \
 EcmaScriptApp.h StereoViewApp.h swt/include/stereoview.h \
 InputDeviceApp.h TransformMode.h OutputApp.h RouteViewApp.h \
 StartWithApp.h ExportNamesApp.h ModellingApp.h RenderApp.h \
 NodeMaterial.h swt/include/swt.h swt/include/keycodes.h \
 swt/include/swlanguages.h swt/include/swDebugf.h \
 swt/include/swttypedef.h NodeImageTexture.h KambiTextureCommonFields.h \
 NodeTextureTransform.h GeometryNodeMacros.h Scene.h RenderState.h \
 ProtoMap.h MyMesh.h NodeIndexedFaceSet.h NodeCoordinate.h Colored.h \
 NodeColor.h NodeColorRGBA.h ComposedGeometryMacros.h FaceData.h \
 NodeNormal.h NodeTextureCoordinate.h NodeTextureCoordinateGenerator.h \
 NodeMultiTextureCoordinate.h Util.h NodeContourPolyline2D.h \
 ChainBasedNode.h NodeNurbsCurve2D.h NodeNurbsCurve.h NurbsCurve.h \
 NodeExtrusion.h NodeNurbsGroup.h NodeGroup.h StaticGroupNode.h \
 GroupNode.h NurbsSurfaceDegreeElevate.h Field.h ExposedField.h \
 MoveCommand.h
NodeNurbsTextureCoordinate.o: NodeNurbsTextureCoordinate.cpp stdafx.h \
 config.h mysnprintf.h NodeNurbsTextureCoordinate.h Node.h Array.h List.h \
 Vec3f.h Vec3x.h MyString.h StringArray.h FieldIndex.h Element.h \
 x3dFlags.h swt/include/WriteFlags.h NodeFlags.h MeshFlags.h \
 ac3dMaterialCallback.h NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h \
 ProtoMacros.h Proto.h Map.h NodeList.h InterfaceArray.h \
 DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h Arrays.h MySwap.h \
 Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h MFDouble.h \
 SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h MFRotation.h \
 MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h \
 MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h SFImage.h \
 SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h SFString.h \
 SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h \
 CommandList.h Command.h Scene.h URL.h DuneApp.h PreferencesApp.h \
 swt/include/swttypedef.h EcmaScriptApp.h StereoViewApp.h \
 swt/include/stereoview.h InputDeviceApp.h TransformMode.h OutputApp.h \
 RouteViewApp.h StartWithApp.h ExportNamesApp.h ModellingApp.h \
 RenderApp.h RenderState.h ProtoMap.h NodeCoordinate.h NodeNormal.h \
 NodeTextureCoordinate.h Util.h Field.h ExposedField.h NodeNurbsSurface.h \
 MeshMorphingNode.h Interpolator.h MeshBasedNode.h GeometryNode.h \
 NodeAppearance.h NodeMaterial.h swt/include/swt.h swt/include/keycodes.h \
 swt/include/swlanguages.h swt/include/swDebugf.h \
 swt/include/swttypedef.h NodeImageTexture.h KambiTextureCommonFields.h \
 NodeTextureTransform.h GeometryNodeMacros.h NodeNurbsTrimmedSurface.h \
 MyMesh.h NodeIndexedFaceSet.h Colored.h NodeColor.h NodeColorRGBA.h \
 ComposedGeometryMacros.h FaceData.h NodeTextureCoordinateGenerator.h \
 NodeMultiTextureCoordinate.h
NodeNurbsTextureSurface.o: NodeNurbsTextureSurface.cpp stdafx.h config.h \
 mysnprintf.h NodeNurbsTextureSurface.h Node.h Array.h List.h Vec3f.h \
 Vec3x.h MyString.h StringArray.h FieldIndex.h Element.h x3dFlags.h \
 swt/include/WriteFlags.h NodeFlags.h MeshFlags.h ac3dMaterialCallback.h \
 NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h ProtoMacros.h Proto.h \
 Map.h NodeList.h InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h \
 MFBool.h FieldValue.h Arrays.h MySwap.h Types.h MFColor.h MFFloat.h \
 SFFloat.h MFColorRGBA.h MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h \
 MFMatrix4f.h MFNode.h MFRotation.h MFString.h MFTime.h SFTime.h \
 MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h \
 SFBool.h SFColor.h SFColorRGBA.h SFImage.h SFInt32.h SFMatrix3f.h \
 SFMatrix4f.h SFNode.h SFRotation.h SFString.h SFVec2d.h SFVec2f.h \
 Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h CommandList.h Command.h \
 NodeNurbsTextureCoordinate.h Scene.h URL.h DuneApp.h PreferencesApp.h \
 swt/include/swttypedef.h EcmaScriptApp.h StereoViewApp.h \
 swt/include/stereoview.h InputDeviceApp.h TransformMode.h OutputApp.h \
 RouteViewApp.h StartWithApp.h ExportNamesApp.h ModellingApp.h \
 RenderApp.h RenderState.h ProtoMap.h Field.h ExposedField.h
NodeNurbsTrimmedSurface.o: NodeNurbsTrimmedSurface.cpp stdafx.h config.h \
 mysnprintf.h NodeNurbsTrimmedSurface.h NodeNurbsSurface.h \
 MeshMorphingNode.h Array.h Interpolator.h Node.h List.h Vec3f.h Vec3x.h \
 MyString.h StringArray.h FieldIndex.h Element.h x3dFlags.h \
 swt/include/WriteFlags.h NodeFlags.h MeshFlags.h ac3dMaterialCallback.h \
 NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h ProtoMacros.h Proto.h \
 Map.h NodeList.h InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h \
 MFBool.h FieldValue.h Arrays.h MySwap.h Types.h MFColor.h MFFloat.h \
 SFFloat.h MFColorRGBA.h MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h \
 MFMatrix4f.h MFNode.h MFRotation.h MFString.h MFTime.h SFTime.h \
 MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h \
 SFBool.h SFColor.h SFColorRGBA.h SFImage.h SFInt32.h SFMatrix3f.h \
 SFMatrix4f.h SFNode.h SFRotation.h SFString.h SFVec2d.h SFVec2f.h \
 Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h CommandList.h Command.h \
 MeshBasedNode.h GeometryNode.h NodeAppearance.h DuneApp.h URL.h \
 PreferencesApp.h swt/include/swttypedef.h EcmaScriptApp.h \
 StereoViewApp.h swt/include/stereoview.h InputDeviceApp.h \
 TransformMode.h OutputApp.h RouteViewApp.h StartWithApp.h \
 ExportNamesApp.h ModellingApp.h RenderApp.h NodeMaterial.h \
 swt/include/swt.h swt/include/keycodes.h swt/include/swlanguages.h \
 swt/include/swDebugf.h swt/include/swttypedef.h NodeImageTexture.h \
 KambiTextureCommonFields.h NodeTextureTransform.h GeometryNodeMacros.h \
 Scene.h RenderState.h ProtoMap.h NodeTrimmedSurface.h MoveCommand.h \
 Util.h NodeContour2D.h NodeNurbsCurve2D.h NodeContourPolyline2D.h \
 ChainBasedNode.h NodePolyline2D.h MyMesh.h NodeIndexedFaceSet.h \
 NodeCoordinate.h Colored.h NodeColor.h NodeColorRGBA.h \
 ComposedGeometryMacros.h FaceData.h NodeNormal.h NodeTextureCoordinate.h \
 NodeTextureCoordinateGenerator.h NodeMultiTextureCoordinate.h
NodeOrientationChaser.o: NodeOrientationChaser.cpp stdafx.h config.h \
 mysnprintf.h NodeOrientationChaser.h Node.h Array.h List.h Vec3f.h \
 Vec3x.h MyString.h StringArray.h FieldIndex.h Element.h x3dFlags.h \
 swt/include/WriteFlags.h NodeFlags.h MeshFlags.h ac3dMaterialCallback.h \
 NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h ProtoMacros.h Proto.h \
 Map.h NodeList.h InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h \
 MFBool.h FieldValue.h Arrays.h MySwap.h Types.h MFColor.h MFFloat.h \
 SFFloat.h MFColorRGBA.h MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h \
 MFMatrix4f.h MFNode.h MFRotation.h MFString.h MFTime.h SFTime.h \
 MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h \
 SFBool.h SFColor.h SFColorRGBA.h SFImage.h SFInt32.h SFMatrix3f.h \
 SFMatrix4f.h SFNode.h SFRotation.h SFString.h SFVec2d.h SFVec2f.h \
 Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h CommandList.h Command.h \
 ChaserNode.h DuneApp.h URL.h PreferencesApp.h swt/include/swttypedef.h \
 EcmaScriptApp.h StereoViewApp.h swt/include/stereoview.h \
 InputDeviceApp.h TransformMode.h OutputApp.h RouteViewApp.h \
 StartWithApp.h ExportNamesApp.h ModellingApp.h RenderApp.h
NodeOrientationDamper.o: NodeOrientationDamper.cpp stdafx.h config.h \
 mysnprintf.h NodeOrientationDamper.h Node.h Array.h List.h Vec3f.h \
 Vec3x.h MyString.h StringArray.h FieldIndex.h Element.h x3dFlags.h \
 swt/include/WriteFlags.h NodeFlags.h MeshFlags.h ac3dMaterialCallback.h \
 NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h ProtoMacros.h Proto.h \
 Map.h NodeList.h InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h \
 MFBool.h FieldValue.h Arrays.h MySwap.h Types.h MFColor.h MFFloat.h \
 SFFloat.h MFColorRGBA.h MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h \
 MFMatrix4f.h MFNode.h MFRotation.h MFString.h MFTime.h SFTime.h \
 MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h \
 SFBool.h SFColor.h SFColorRGBA.h SFImage.h SFInt32.h SFMatrix3f.h \
 SFMatrix4f.h SFNode.h SFRotation.h SFString.h SFVec2d.h SFVec2f.h \
 Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h CommandList.h Command.h \
 DamperNode.h DuneApp.h URL.h PreferencesApp.h swt/include/swttypedef.h \
 EcmaScriptApp.h StereoViewApp.h swt/include/stereoview.h \
 InputDeviceApp.h TransformMode.h OutputApp.h RouteViewApp.h \
 StartWithApp.h ExportNamesApp.h ModellingApp.h RenderApp.h
NodeOrientationInterpolator.o: NodeOrientationInterpolator.cpp stdafx.h \
 config.h mysnprintf.h NodeOrientationInterpolator.h Interpolator.h \
 Node.h Array.h List.h Vec3f.h Vec3x.h MyString.h StringArray.h \
 FieldIndex.h Element.h x3dFlags.h swt/include/WriteFlags.h NodeFlags.h \
 MeshFlags.h ac3dMaterialCallback.h NodeArray.h Stack.h Matrix.h Vec3d.h \
 Quaternion.h ProtoMacros.h Proto.h Map.h NodeList.h InterfaceArray.h \
 DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h Arrays.h MySwap.h \
 Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h MFDouble.h \
 SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h MFRotation.h \
 MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h \
 MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h SFImage.h \
 SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h SFString.h \
 SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h \
 CommandList.h Command.h Scene.h URL.h DuneApp.h PreferencesApp.h \
 swt/include/swttypedef.h EcmaScriptApp.h StereoViewApp.h \
 swt/include/stereoview.h InputDeviceApp.h TransformMode.h OutputApp.h \
 RouteViewApp.h StartWithApp.h ExportNamesApp.h ModellingApp.h \
 RenderApp.h RenderState.h ProtoMap.h
NodeOrthoViewpoint.o: NodeOrthoViewpoint.cpp stdafx.h config.h \
 mysnprintf.h NodeOrthoViewpoint.h Node.h Array.h List.h Vec3f.h Vec3x.h \
 MyString.h StringArray.h FieldIndex.h Element.h x3dFlags.h \
 swt/include/WriteFlags.h NodeFlags.h MeshFlags.h ac3dMaterialCallback.h \
 NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h ProtoMacros.h Proto.h \
 Map.h NodeList.h InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h \
 MFBool.h FieldValue.h Arrays.h MySwap.h Types.h MFColor.h MFFloat.h \
 SFFloat.h MFColorRGBA.h MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h \
 MFMatrix4f.h MFNode.h MFRotation.h MFString.h MFTime.h SFTime.h \
 MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h \
 SFBool.h SFColor.h SFColorRGBA.h SFImage.h SFInt32.h SFMatrix3f.h \
 SFMatrix4f.h SFNode.h SFRotation.h SFString.h SFVec2d.h SFVec2f.h \
 Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h CommandList.h Command.h \
 DuneApp.h URL.h PreferencesApp.h swt/include/swttypedef.h \
 EcmaScriptApp.h StereoViewApp.h swt/include/stereoview.h \
 InputDeviceApp.h TransformMode.h OutputApp.h RouteViewApp.h \
 StartWithApp.h ExportNamesApp.h ModellingApp.h RenderApp.h \
 KambiViewpointCommonFields.h Scene.h RenderState.h ProtoMap.h
NodePackagedShader.o: NodePackagedShader.cpp stdafx.h config.h \
 mysnprintf.h NodePackagedShader.h Node.h Array.h List.h Vec3f.h Vec3x.h \
 MyString.h StringArray.h FieldIndex.h Element.h x3dFlags.h \
 swt/include/WriteFlags.h NodeFlags.h MeshFlags.h ac3dMaterialCallback.h \
 NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h ProtoMacros.h Proto.h \
 Map.h NodeList.h InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h \
 MFBool.h FieldValue.h Arrays.h MySwap.h Types.h MFColor.h MFFloat.h \
 SFFloat.h MFColorRGBA.h MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h \
 MFMatrix4f.h MFNode.h MFRotation.h MFString.h MFTime.h SFTime.h \
 MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h \
 SFBool.h SFColor.h SFColorRGBA.h SFImage.h SFInt32.h SFMatrix3f.h \
 SFMatrix4f.h SFNode.h SFRotation.h SFString.h SFVec2d.h SFVec2f.h \
 Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h CommandList.h Command.h \
 DuneApp.h URL.h PreferencesApp.h swt/include/swttypedef.h \
 EcmaScriptApp.h StereoViewApp.h swt/include/stereoview.h \
 InputDeviceApp.h TransformMode.h OutputApp.h RouteViewApp.h \
 StartWithApp.h ExportNamesApp.h ModellingApp.h RenderApp.h
NodeParticleSystem.o: NodeParticleSystem.cpp stdafx.h config.h \
 mysnprintf.h swt/include/swt.h swt/include/keycodes.h \
 swt/include/swlanguages.h swt/include/swDebugf.h \
 swt/include/swttypedef.h NodeParticleSystem.h Node.h Array.h List.h \
 Vec3f.h Vec3x.h MyString.h StringArray.h FieldIndex.h Element.h \
 x3dFlags.h swt/include/WriteFlags.h NodeFlags.h MeshFlags.h \
 ac3dMaterialCallback.h NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h \
 ProtoMacros.h Proto.h Map.h NodeList.h InterfaceArray.h \
 DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h Arrays.h MySwap.h \
 Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h MFDouble.h \
 SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h MFRotation.h \
 MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h \
 MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h SFImage.h \
 SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h SFString.h \
 SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h \
 CommandList.h Command.h DuneApp.h URL.h PreferencesApp.h \
 swt/include/swttypedef.h EcmaScriptApp.h StereoViewApp.h \
 swt/include/stereoview.h InputDeviceApp.h TransformMode.h OutputApp.h \
 RouteViewApp.h StartWithApp.h ExportNamesApp.h ModellingApp.h \
 RenderApp.h Scene.h RenderState.h ProtoMap.h MyMesh.h \
 NodeIndexedFaceSet.h MeshBasedNode.h GeometryNode.h NodeAppearance.h \
 NodeMaterial.h NodeImageTexture.h KambiTextureCommonFields.h \
 NodeTextureTransform.h GeometryNodeMacros.h NodeCoordinate.h Colored.h \
 NodeColor.h NodeColorRGBA.h ComposedGeometryMacros.h FaceData.h \
 NodeNormal.h NodeTextureCoordinate.h NodeTextureCoordinateGenerator.h \
 NodeMultiTextureCoordinate.h Util.h NodeForcePhysicsModel.h \
 NodeWindPhysicsModel.h NodeConeEmitter.h EmitterNode.h \
 NodeExplosionEmitter.h NodePointEmitter.h NodePolylineEmitter.h \
 NodeVolumeEmitter.h NodeSurfaceEmitter.h NodeIndexedLineSet.h \
 NodePointSet.h
NodePickableGroup.o: NodePickableGroup.cpp stdafx.h config.h mysnprintf.h \
 NodePickableGroup.h Node.h Array.h List.h Vec3f.h Vec3x.h MyString.h \
 StringArray.h FieldIndex.h Element.h x3dFlags.h swt/include/WriteFlags.h \
 NodeFlags.h MeshFlags.h ac3dMaterialCallback.h NodeArray.h Stack.h \
 Matrix.h Vec3d.h Quaternion.h ProtoMacros.h Proto.h Map.h NodeList.h \
 InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h \
 Arrays.h MySwap.h Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h \
 MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h \
 MFRotation.h MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h \
 MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h \
 SFImage.h SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h \
 SFString.h SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h \
 SFVec4f.h CommandList.h Command.h StaticGroupNode.h GroupNode.h \
 DuneApp.h URL.h PreferencesApp.h swt/include/swttypedef.h \
 EcmaScriptApp.h StereoViewApp.h swt/include/stereoview.h \
 InputDeviceApp.h TransformMode.h OutputApp.h RouteViewApp.h \
 StartWithApp.h ExportNamesApp.h ModellingApp.h RenderApp.h
NodePixelTexture.o: NodePixelTexture.cpp stdafx.h config.h mysnprintf.h \
 swt/include/swt.h swt/include/keycodes.h swt/include/swlanguages.h \
 swt/include/swDebugf.h swt/include/swttypedef.h DuneApp.h List.h Array.h \
 MyString.h StringArray.h Node.h Vec3f.h Vec3x.h FieldIndex.h Element.h \
 x3dFlags.h swt/include/WriteFlags.h NodeFlags.h MeshFlags.h \
 ac3dMaterialCallback.h NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h \
 URL.h PreferencesApp.h swt/include/swttypedef.h EcmaScriptApp.h \
 StereoViewApp.h swt/include/stereoview.h InputDeviceApp.h \
 TransformMode.h OutputApp.h RouteViewApp.h StartWithApp.h \
 ExportNamesApp.h ModellingApp.h RenderApp.h resource.h \
 NodePixelTexture.h ProtoMacros.h Proto.h Map.h NodeList.h \
 InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h \
 Arrays.h MySwap.h Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h \
 MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h \
 MFRotation.h MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h \
 MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h \
 SFImage.h SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h \
 SFString.h SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h \
 SFVec4f.h CommandList.h Command.h KambiTextureCommonFields.h Texture.h \
 MyMesh.h NodeIndexedFaceSet.h MeshBasedNode.h GeometryNode.h \
 NodeAppearance.h NodeMaterial.h NodeImageTexture.h \
 NodeTextureTransform.h GeometryNodeMacros.h NodeCoordinate.h Colored.h \
 NodeColor.h NodeColorRGBA.h ComposedGeometryMacros.h FaceData.h \
 NodeNormal.h NodeTextureCoordinate.h NodeTextureCoordinateGenerator.h \
 NodeMultiTextureCoordinate.h Scene.h RenderState.h ProtoMap.h Util.h \
 NodeShape.h
NodePixelTexture3D.o: NodePixelTexture3D.cpp stdafx.h config.h \
 mysnprintf.h NodePixelTexture3D.h Node.h Array.h List.h Vec3f.h Vec3x.h \
 MyString.h StringArray.h FieldIndex.h Element.h x3dFlags.h \
 swt/include/WriteFlags.h NodeFlags.h MeshFlags.h ac3dMaterialCallback.h \
 NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h ProtoMacros.h Proto.h \
 Map.h NodeList.h InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h \
 MFBool.h FieldValue.h Arrays.h MySwap.h Types.h MFColor.h MFFloat.h \
 SFFloat.h MFColorRGBA.h MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h \
 MFMatrix4f.h MFNode.h MFRotation.h MFString.h MFTime.h SFTime.h \
 MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h \
 SFBool.h SFColor.h SFColorRGBA.h SFImage.h SFInt32.h SFMatrix3f.h \
 SFMatrix4f.h SFNode.h SFRotation.h SFString.h SFVec2d.h SFVec2f.h \
 Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h CommandList.h Command.h \
 Texture3DNode.h KambiTextureCommonFields.h Scene.h URL.h DuneApp.h \
 PreferencesApp.h swt/include/swttypedef.h EcmaScriptApp.h \
 StereoViewApp.h swt/include/stereoview.h InputDeviceApp.h \
 TransformMode.h OutputApp.h RouteViewApp.h StartWithApp.h \
 ExportNamesApp.h ModellingApp.h RenderApp.h RenderState.h ProtoMap.h
NodePlaneSensor.o: NodePlaneSensor.cpp stdafx.h config.h mysnprintf.h \
 NodePlaneSensor.h Node.h Array.h List.h Vec3f.h Vec3x.h MyString.h \
 StringArray.h FieldIndex.h Element.h x3dFlags.h swt/include/WriteFlags.h \
 NodeFlags.h MeshFlags.h ac3dMaterialCallback.h NodeArray.h Stack.h \
 Matrix.h Vec3d.h Quaternion.h ProtoMacros.h Proto.h Map.h NodeList.h \
 InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h \
 Arrays.h MySwap.h Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h \
 MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h \
 MFRotation.h MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h \
 MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h \
 SFImage.h SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h \
 SFString.h SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h \
 SFVec4f.h CommandList.h Command.h Field.h
NodePointEmitter.o: NodePointEmitter.cpp stdafx.h config.h mysnprintf.h \
 NodePointEmitter.h Node.h Array.h List.h Vec3f.h Vec3x.h MyString.h \
 StringArray.h FieldIndex.h Element.h x3dFlags.h swt/include/WriteFlags.h \
 NodeFlags.h MeshFlags.h ac3dMaterialCallback.h NodeArray.h Stack.h \
 Matrix.h Vec3d.h Quaternion.h ProtoMacros.h Proto.h Map.h NodeList.h \
 InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h \
 Arrays.h MySwap.h Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h \
 MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h \
 MFRotation.h MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h \
 MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h \
 SFImage.h SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h \
 SFString.h SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h \
 SFVec4f.h CommandList.h Command.h EmitterNode.h NodeAppearance.h \
 DuneApp.h URL.h PreferencesApp.h swt/include/swttypedef.h \
 EcmaScriptApp.h StereoViewApp.h swt/include/stereoview.h \
 InputDeviceApp.h TransformMode.h OutputApp.h RouteViewApp.h \
 StartWithApp.h ExportNamesApp.h ModellingApp.h RenderApp.h \
 NodeMaterial.h swt/include/swt.h swt/include/keycodes.h \
 swt/include/swlanguages.h swt/include/swDebugf.h \
 swt/include/swttypedef.h NodeImageTexture.h KambiTextureCommonFields.h \
 NodeTextureTransform.h
NodePointLight.o: NodePointLight.cpp stdafx.h config.h mysnprintf.h \
 NodePointLight.h Node.h Array.h List.h Vec3f.h Vec3x.h MyString.h \
 StringArray.h FieldIndex.h Element.h x3dFlags.h swt/include/WriteFlags.h \
 NodeFlags.h MeshFlags.h ac3dMaterialCallback.h NodeArray.h Stack.h \
 Matrix.h Vec3d.h Quaternion.h ProtoMacros.h Proto.h Map.h NodeList.h \
 InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h \
 Arrays.h MySwap.h Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h \
 MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h \
 MFRotation.h MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h \
 MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h \
 SFImage.h SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h \
 SFString.h SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h \
 SFVec4f.h CommandList.h Command.h swt/include/swt.h \
 swt/include/keycodes.h swt/include/swlanguages.h swt/include/swDebugf.h \
 swt/include/swttypedef.h KambiLightCommonFields.h \
 X3domLightCommonFields.h Scene.h URL.h DuneApp.h PreferencesApp.h \
 swt/include/swttypedef.h EcmaScriptApp.h StereoViewApp.h \
 swt/include/stereoview.h InputDeviceApp.h TransformMode.h OutputApp.h \
 RouteViewApp.h StartWithApp.h ExportNamesApp.h ModellingApp.h \
 RenderApp.h RenderState.h ProtoMap.h Util.h resource.h
NodePointPickSensor.o: NodePointPickSensor.cpp stdafx.h config.h \
 mysnprintf.h NodePointPickSensor.h Node.h Array.h List.h Vec3f.h Vec3x.h \
 MyString.h StringArray.h FieldIndex.h Element.h x3dFlags.h \
 swt/include/WriteFlags.h NodeFlags.h MeshFlags.h ac3dMaterialCallback.h \
 NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h ProtoMacros.h Proto.h \
 Map.h NodeList.h InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h \
 MFBool.h FieldValue.h Arrays.h MySwap.h Types.h MFColor.h MFFloat.h \
 SFFloat.h MFColorRGBA.h MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h \
 MFMatrix4f.h MFNode.h MFRotation.h MFString.h MFTime.h SFTime.h \
 MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h \
 SFBool.h SFColor.h SFColorRGBA.h SFImage.h SFInt32.h SFMatrix3f.h \
 SFMatrix4f.h SFNode.h SFRotation.h SFString.h SFVec2d.h SFVec2f.h \
 Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h CommandList.h Command.h \
 DuneApp.h URL.h PreferencesApp.h swt/include/swttypedef.h \
 EcmaScriptApp.h StereoViewApp.h swt/include/stereoview.h \
 InputDeviceApp.h TransformMode.h OutputApp.h RouteViewApp.h \
 StartWithApp.h ExportNamesApp.h ModellingApp.h RenderApp.h
NodePointSet.o: NodePointSet.cpp stdafx.h config.h mysnprintf.h \
 NodePointSet.h GeometryNode.h Node.h Array.h List.h Vec3f.h Vec3x.h \
 MyString.h StringArray.h FieldIndex.h Element.h x3dFlags.h \
 swt/include/WriteFlags.h NodeFlags.h MeshFlags.h ac3dMaterialCallback.h \
 NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h NodeAppearance.h \
 ProtoMacros.h Proto.h Map.h NodeList.h InterfaceArray.h \
 DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h Arrays.h MySwap.h \
 Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h MFDouble.h \
 SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h MFRotation.h \
 MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h \
 MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h SFImage.h \
 SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h SFString.h \
 SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h \
 CommandList.h Command.h DuneApp.h URL.h PreferencesApp.h \
 swt/include/swttypedef.h EcmaScriptApp.h StereoViewApp.h \
 swt/include/stereoview.h InputDeviceApp.h TransformMode.h OutputApp.h \
 RouteViewApp.h StartWithApp.h ExportNamesApp.h ModellingApp.h \
 RenderApp.h NodeMaterial.h swt/include/swt.h swt/include/keycodes.h \
 swt/include/swlanguages.h swt/include/swDebugf.h \
 swt/include/swttypedef.h NodeImageTexture.h KambiTextureCommonFields.h \
 NodeTextureTransform.h GeometryNodeMacros.h Colored.h NodeColor.h \
 NodeColorRGBA.h NodeCoordinate.h NodeGeoCoordinate.h GeoNode.h \
 NodeFogCoordinate.h Util.h
NodePolyline2D.o: NodePolyline2D.cpp stdafx.h config.h mysnprintf.h \
 NodePolyline2D.h ChainBasedNode.h GeometryNode.h Node.h Array.h List.h \
 Vec3f.h Vec3x.h MyString.h StringArray.h FieldIndex.h Element.h \
 x3dFlags.h swt/include/WriteFlags.h NodeFlags.h MeshFlags.h \
 ac3dMaterialCallback.h NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h \
 NodeAppearance.h ProtoMacros.h Proto.h Map.h NodeList.h InterfaceArray.h \
 DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h Arrays.h MySwap.h \
 Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h MFDouble.h \
 SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h MFRotation.h \
 MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h \
 MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h SFImage.h \
 SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h SFString.h \
 SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h \
 CommandList.h Command.h DuneApp.h URL.h PreferencesApp.h \
 swt/include/swttypedef.h EcmaScriptApp.h StereoViewApp.h \
 swt/include/stereoview.h InputDeviceApp.h TransformMode.h OutputApp.h \
 RouteViewApp.h StartWithApp.h ExportNamesApp.h ModellingApp.h \
 RenderApp.h NodeMaterial.h swt/include/swt.h swt/include/keycodes.h \
 swt/include/swlanguages.h swt/include/swDebugf.h \
 swt/include/swttypedef.h NodeImageTexture.h KambiTextureCommonFields.h \
 NodeTextureTransform.h GeometryNodeMacros.h Scene.h RenderState.h \
 ProtoMap.h ExposedField.h Field.h
NodePolylineEmitter.o: NodePolylineEmitter.cpp stdafx.h config.h \
 mysnprintf.h NodePolylineEmitter.h Node.h Array.h List.h Vec3f.h Vec3x.h \
 MyString.h StringArray.h FieldIndex.h Element.h x3dFlags.h \
 swt/include/WriteFlags.h NodeFlags.h MeshFlags.h ac3dMaterialCallback.h \
 NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h ProtoMacros.h Proto.h \
 Map.h NodeList.h InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h \
 MFBool.h FieldValue.h Arrays.h MySwap.h Types.h MFColor.h MFFloat.h \
 SFFloat.h MFColorRGBA.h MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h \
 MFMatrix4f.h MFNode.h MFRotation.h MFString.h MFTime.h SFTime.h \
 MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h \
 SFBool.h SFColor.h SFColorRGBA.h SFImage.h SFInt32.h SFMatrix3f.h \
 SFMatrix4f.h SFNode.h SFRotation.h SFString.h SFVec2d.h SFVec2f.h \
 Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h CommandList.h Command.h \
 EmitterNode.h NodeAppearance.h DuneApp.h URL.h PreferencesApp.h \
 swt/include/swttypedef.h EcmaScriptApp.h StereoViewApp.h \
 swt/include/stereoview.h InputDeviceApp.h TransformMode.h OutputApp.h \
 RouteViewApp.h StartWithApp.h ExportNamesApp.h ModellingApp.h \
 RenderApp.h NodeMaterial.h swt/include/swt.h swt/include/keycodes.h \
 swt/include/swlanguages.h swt/include/swDebugf.h \
 swt/include/swttypedef.h NodeImageTexture.h KambiTextureCommonFields.h \
 NodeTextureTransform.h
NodePolypoint2D.o: NodePolypoint2D.cpp Scene.h Array.h stdafx.h config.h \
 mysnprintf.h Map.h List.h Stack.h CommandList.h Command.h NodeList.h \
 NodeArray.h Node.h Vec3f.h Vec3x.h MyString.h StringArray.h FieldIndex.h \
 Element.h x3dFlags.h swt/include/WriteFlags.h NodeFlags.h MeshFlags.h \
 ac3dMaterialCallback.h Matrix.h Vec3d.h Quaternion.h URL.h Proto.h \
 ProtoMacros.h InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h MFBool.h \
 FieldValue.h Arrays.h MySwap.h Types.h MFColor.h MFFloat.h SFFloat.h \
 MFColorRGBA.h MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h \
 MFNode.h MFRotation.h MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h \
 MFVec2f.h MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h \
 SFColorRGBA.h SFImage.h SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h \
 SFRotation.h SFString.h SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h \
 SFVec3f.h SFVec4f.h DuneApp.h PreferencesApp.h swt/include/swttypedef.h \
 EcmaScriptApp.h StereoViewApp.h swt/include/stereoview.h \
 InputDeviceApp.h TransformMode.h OutputApp.h RouteViewApp.h \
 StartWithApp.h ExportNamesApp.h ModellingApp.h RenderApp.h RenderState.h \
 ProtoMap.h NodePolypoint2D.h GeometryNode.h NodeAppearance.h \
 NodeMaterial.h swt/include/swt.h swt/include/keycodes.h \
 swt/include/swlanguages.h swt/include/swDebugf.h \
 swt/include/swttypedef.h NodeImageTexture.h KambiTextureCommonFields.h \
 NodeTextureTransform.h GeometryNodeMacros.h ExposedField.h \
 NodePointSet.h Colored.h NodeColor.h NodeColorRGBA.h NodeCoordinate.h \
 Util.h
NodePositionChaser.o: NodePositionChaser.cpp stdafx.h config.h \
 mysnprintf.h NodePositionChaser.h Node.h Array.h List.h Vec3f.h Vec3x.h \
 MyString.h StringArray.h FieldIndex.h Element.h x3dFlags.h \
 swt/include/WriteFlags.h NodeFlags.h MeshFlags.h ac3dMaterialCallback.h \
 NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h ProtoMacros.h Proto.h \
 Map.h NodeList.h InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h \
 MFBool.h FieldValue.h Arrays.h MySwap.h Types.h MFColor.h MFFloat.h \
 SFFloat.h MFColorRGBA.h MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h \
 MFMatrix4f.h MFNode.h MFRotation.h MFString.h MFTime.h SFTime.h \
 MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h \
 SFBool.h SFColor.h SFColorRGBA.h SFImage.h SFInt32.h SFMatrix3f.h \
 SFMatrix4f.h SFNode.h SFRotation.h SFString.h SFVec2d.h SFVec2f.h \
 Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h CommandList.h Command.h \
 ChaserNode.h DuneApp.h URL.h PreferencesApp.h swt/include/swttypedef.h \
 EcmaScriptApp.h StereoViewApp.h swt/include/stereoview.h \
 InputDeviceApp.h TransformMode.h OutputApp.h RouteViewApp.h \
 StartWithApp.h ExportNamesApp.h ModellingApp.h RenderApp.h
NodePositionChaser2D.o: NodePositionChaser2D.cpp stdafx.h config.h \
 mysnprintf.h NodePositionChaser2D.h Node.h Array.h List.h Vec3f.h \
 Vec3x.h MyString.h StringArray.h FieldIndex.h Element.h x3dFlags.h \
 swt/include/WriteFlags.h NodeFlags.h MeshFlags.h ac3dMaterialCallback.h \
 NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h ProtoMacros.h Proto.h \
 Map.h NodeList.h InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h \
 MFBool.h FieldValue.h Arrays.h MySwap.h Types.h MFColor.h MFFloat.h \
 SFFloat.h MFColorRGBA.h MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h \
 MFMatrix4f.h MFNode.h MFRotation.h MFString.h MFTime.h SFTime.h \
 MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h \
 SFBool.h SFColor.h SFColorRGBA.h SFImage.h SFInt32.h SFMatrix3f.h \
 SFMatrix4f.h SFNode.h SFRotation.h SFString.h SFVec2d.h SFVec2f.h \
 Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h CommandList.h Command.h \
 ChaserNode.h DuneApp.h URL.h PreferencesApp.h swt/include/swttypedef.h \
 EcmaScriptApp.h StereoViewApp.h swt/include/stereoview.h \
 InputDeviceApp.h TransformMode.h OutputApp.h RouteViewApp.h \
 StartWithApp.h ExportNamesApp.h ModellingApp.h RenderApp.h
NodePositionDamper.o: NodePositionDamper.cpp stdafx.h config.h \
 mysnprintf.h NodePositionDamper.h Node.h Array.h List.h Vec3f.h Vec3x.h \
 MyString.h StringArray.h FieldIndex.h Element.h x3dFlags.h \
 swt/include/WriteFlags.h NodeFlags.h MeshFlags.h ac3dMaterialCallback.h \
 NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h ProtoMacros.h Proto.h \
 Map.h NodeList.h InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h \
 MFBool.h FieldValue.h Arrays.h MySwap.h Types.h MFColor.h MFFloat.h \
 SFFloat.h MFColorRGBA.h MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h \
 MFMatrix4f.h MFNode.h MFRotation.h MFString.h MFTime.h SFTime.h \
 MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h \
 SFBool.h SFColor.h SFColorRGBA.h SFImage.h SFInt32.h SFMatrix3f.h \
 SFMatrix4f.h SFNode.h SFRotation.h SFString.h SFVec2d.h SFVec2f.h \
 Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h CommandList.h Command.h \
 DamperNode.h DuneApp.h URL.h PreferencesApp.h swt/include/swttypedef.h \
 EcmaScriptApp.h StereoViewApp.h swt/include/stereoview.h \
 InputDeviceApp.h TransformMode.h OutputApp.h RouteViewApp.h \
 StartWithApp.h ExportNamesApp.h ModellingApp.h RenderApp.h
NodePositionDamper2D.o: NodePositionDamper2D.cpp stdafx.h config.h \
 mysnprintf.h NodePositionDamper2D.h Node.h Array.h List.h Vec3f.h \
 Vec3x.h MyString.h StringArray.h FieldIndex.h Element.h x3dFlags.h \
 swt/include/WriteFlags.h NodeFlags.h MeshFlags.h ac3dMaterialCallback.h \
 NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h ProtoMacros.h Proto.h \
 Map.h NodeList.h InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h \
 MFBool.h FieldValue.h Arrays.h MySwap.h Types.h MFColor.h MFFloat.h \
 SFFloat.h MFColorRGBA.h MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h \
 MFMatrix4f.h MFNode.h MFRotation.h MFString.h MFTime.h SFTime.h \
 MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h \
 SFBool.h SFColor.h SFColorRGBA.h SFImage.h SFInt32.h SFMatrix3f.h \
 SFMatrix4f.h SFNode.h SFRotation.h SFString.h SFVec2d.h SFVec2f.h \
 Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h CommandList.h Command.h \
 DamperNode.h DuneApp.h URL.h PreferencesApp.h swt/include/swttypedef.h \
 EcmaScriptApp.h StereoViewApp.h swt/include/stereoview.h \
 InputDeviceApp.h TransformMode.h OutputApp.h RouteViewApp.h \
 StartWithApp.h ExportNamesApp.h ModellingApp.h RenderApp.h
NodePositionInterpolator.o: NodePositionInterpolator.cpp stdafx.h \
 config.h mysnprintf.h NodePositionInterpolator.h Interpolator.h Node.h \
 Array.h List.h Vec3f.h Vec3x.h MyString.h StringArray.h FieldIndex.h \
 Element.h x3dFlags.h swt/include/WriteFlags.h NodeFlags.h MeshFlags.h \
 ac3dMaterialCallback.h NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h \
 ProtoMacros.h Proto.h Map.h NodeList.h InterfaceArray.h \
 DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h Arrays.h MySwap.h \
 Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h MFDouble.h \
 SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h MFRotation.h \
 MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h \
 MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h SFImage.h \
 SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h SFString.h \
 SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h \
 CommandList.h Command.h Scene.h URL.h DuneApp.h PreferencesApp.h \
 swt/include/swttypedef.h EcmaScriptApp.h StereoViewApp.h \
 swt/include/stereoview.h InputDeviceApp.h TransformMode.h OutputApp.h \
 RouteViewApp.h StartWithApp.h ExportNamesApp.h ModellingApp.h \
 RenderApp.h RenderState.h ProtoMap.h MFieldCommand.h FieldCommand.h
NodePositionInterpolator2D.o: NodePositionInterpolator2D.cpp stdafx.h \
 config.h mysnprintf.h NodePositionInterpolator2D.h Interpolator.h Node.h \
 Array.h List.h Vec3f.h Vec3x.h MyString.h StringArray.h FieldIndex.h \
 Element.h x3dFlags.h swt/include/WriteFlags.h NodeFlags.h MeshFlags.h \
 ac3dMaterialCallback.h NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h \
 ProtoMacros.h Proto.h Map.h NodeList.h InterfaceArray.h \
 DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h Arrays.h MySwap.h \
 Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h MFDouble.h \
 SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h MFRotation.h \
 MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h \
 MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h SFImage.h \
 SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h SFString.h \
 SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h \
 CommandList.h Command.h Scene.h URL.h DuneApp.h PreferencesApp.h \
 swt/include/swttypedef.h EcmaScriptApp.h StereoViewApp.h \
 swt/include/stereoview.h InputDeviceApp.h TransformMode.h OutputApp.h \
 RouteViewApp.h StartWithApp.h ExportNamesApp.h ModellingApp.h \
 RenderApp.h RenderState.h ProtoMap.h MFieldCommand.h FieldCommand.h
NodePrimitivePickSensor.o: NodePrimitivePickSensor.cpp stdafx.h config.h \
 mysnprintf.h NodePrimitivePickSensor.h Node.h Array.h List.h Vec3f.h \
 Vec3x.h MyString.h StringArray.h FieldIndex.h Element.h x3dFlags.h \
 swt/include/WriteFlags.h NodeFlags.h MeshFlags.h ac3dMaterialCallback.h \
 NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h ProtoMacros.h Proto.h \
 Map.h NodeList.h InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h \
 MFBool.h FieldValue.h Arrays.h MySwap.h Types.h MFColor.h MFFloat.h \
 SFFloat.h MFColorRGBA.h MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h \
 MFMatrix4f.h MFNode.h MFRotation.h MFString.h MFTime.h SFTime.h \
 MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h \
 SFBool.h SFColor.h SFColorRGBA.h SFImage.h SFInt32.h SFMatrix3f.h \
 SFMatrix4f.h SFNode.h SFRotation.h SFString.h SFVec2d.h SFVec2f.h \
 Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h CommandList.h Command.h \
 DuneApp.h URL.h PreferencesApp.h swt/include/swttypedef.h \
 EcmaScriptApp.h StereoViewApp.h swt/include/stereoview.h \
 InputDeviceApp.h TransformMode.h OutputApp.h RouteViewApp.h \
 StartWithApp.h ExportNamesApp.h ModellingApp.h RenderApp.h
NodeProgramShader.o: NodeProgramShader.cpp stdafx.h config.h mysnprintf.h \
 NodeProgramShader.h Node.h Array.h List.h Vec3f.h Vec3x.h MyString.h \
 StringArray.h FieldIndex.h Element.h x3dFlags.h swt/include/WriteFlags.h \
 NodeFlags.h MeshFlags.h ac3dMaterialCallback.h NodeArray.h Stack.h \
 Matrix.h Vec3d.h Quaternion.h ProtoMacros.h Proto.h Map.h NodeList.h \
 InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h \
 Arrays.h MySwap.h Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h \
 MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h \
 MFRotation.h MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h \
 MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h \
 SFImage.h SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h \
 SFString.h SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h \
 SFVec4f.h CommandList.h Command.h DuneApp.h URL.h PreferencesApp.h \
 swt/include/swttypedef.h EcmaScriptApp.h StereoViewApp.h \
 swt/include/stereoview.h InputDeviceApp.h TransformMode.h OutputApp.h \
 RouteViewApp.h StartWithApp.h ExportNamesApp.h ModellingApp.h \
 RenderApp.h
NodeProjectionVolumeStyle.o: NodeProjectionVolumeStyle.cpp stdafx.h \
 config.h mysnprintf.h NodeProjectionVolumeStyle.h Node.h Array.h List.h \
 Vec3f.h Vec3x.h MyString.h StringArray.h FieldIndex.h Element.h \
 x3dFlags.h swt/include/WriteFlags.h NodeFlags.h MeshFlags.h \
 ac3dMaterialCallback.h NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h \
 ProtoMacros.h Proto.h Map.h NodeList.h InterfaceArray.h \
 DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h Arrays.h MySwap.h \
 Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h MFDouble.h \
 SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h MFRotation.h \
 MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h \
 MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h SFImage.h \
 SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h SFString.h \
 SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h \
 CommandList.h Command.h DuneApp.h URL.h PreferencesApp.h \
 swt/include/swttypedef.h EcmaScriptApp.h StereoViewApp.h \
 swt/include/stereoview.h InputDeviceApp.h TransformMode.h OutputApp.h \
 RouteViewApp.h StartWithApp.h ExportNamesApp.h ModellingApp.h \
 RenderApp.h
NodeProximitySensor.o: NodeProximitySensor.cpp stdafx.h config.h \
 mysnprintf.h NodeProximitySensor.h Node.h Array.h List.h Vec3f.h Vec3x.h \
 MyString.h StringArray.h FieldIndex.h Element.h x3dFlags.h \
 swt/include/WriteFlags.h NodeFlags.h MeshFlags.h ac3dMaterialCallback.h \
 NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h ProtoMacros.h Proto.h \
 Map.h NodeList.h InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h \
 MFBool.h FieldValue.h Arrays.h MySwap.h Types.h MFColor.h MFFloat.h \
 SFFloat.h MFColorRGBA.h MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h \
 MFMatrix4f.h MFNode.h MFRotation.h MFString.h MFTime.h SFTime.h \
 MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h \
 SFBool.h SFColor.h SFColorRGBA.h SFImage.h SFInt32.h SFMatrix3f.h \
 SFMatrix4f.h SFNode.h SFRotation.h SFString.h SFVec2d.h SFVec2f.h \
 Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h CommandList.h Command.h \
 Field.h RenderState.h swt/include/swttypedef.h Util.h Scene.h URL.h \
 DuneApp.h PreferencesApp.h EcmaScriptApp.h StereoViewApp.h \
 swt/include/stereoview.h InputDeviceApp.h TransformMode.h OutputApp.h \
 RouteViewApp.h StartWithApp.h ExportNamesApp.h ModellingApp.h \
 RenderApp.h ProtoMap.h Path.h NodeNavigationInfo.h
NodeQuadSet.o: NodeQuadSet.cpp stdafx.h config.h mysnprintf.h \
 NodeQuadSet.h MeshBasedNode.h GeometryNode.h Node.h Array.h List.h \
 Vec3f.h Vec3x.h MyString.h StringArray.h FieldIndex.h Element.h \
 x3dFlags.h swt/include/WriteFlags.h NodeFlags.h MeshFlags.h \
 ac3dMaterialCallback.h NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h \
 NodeAppearance.h ProtoMacros.h Proto.h Map.h NodeList.h InterfaceArray.h \
 DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h Arrays.h MySwap.h \
 Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h MFDouble.h \
 SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h MFRotation.h \
 MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h \
 MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h SFImage.h \
 SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h SFString.h \
 SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h \
 CommandList.h Command.h DuneApp.h URL.h PreferencesApp.h \
 swt/include/swttypedef.h EcmaScriptApp.h StereoViewApp.h \
 swt/include/stereoview.h InputDeviceApp.h TransformMode.h OutputApp.h \
 RouteViewApp.h StartWithApp.h ExportNamesApp.h ModellingApp.h \
 RenderApp.h NodeMaterial.h swt/include/swt.h swt/include/keycodes.h \
 swt/include/swlanguages.h swt/include/swDebugf.h \
 swt/include/swttypedef.h NodeImageTexture.h KambiTextureCommonFields.h \
 NodeTextureTransform.h GeometryNodeMacros.h Colored.h NodeColor.h \
 NodeColorRGBA.h ComposedGeometryMacros.h Scene.h RenderState.h \
 ProtoMap.h MyMesh.h NodeIndexedFaceSet.h NodeCoordinate.h FaceData.h \
 NodeNormal.h NodeTextureCoordinate.h NodeTextureCoordinateGenerator.h \
 NodeMultiTextureCoordinate.h Util.h NodeGeoCoordinate.h GeoNode.h \
 NodeIndexedLineSet.h
NodeReceiverPdu.o: NodeReceiverPdu.cpp stdafx.h config.h mysnprintf.h \
 NodeReceiverPdu.h Node.h Array.h List.h Vec3f.h Vec3x.h MyString.h \
 StringArray.h FieldIndex.h Element.h x3dFlags.h swt/include/WriteFlags.h \
 NodeFlags.h MeshFlags.h ac3dMaterialCallback.h NodeArray.h Stack.h \
 Matrix.h Vec3d.h Quaternion.h ProtoMacros.h Proto.h Map.h NodeList.h \
 InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h \
 Arrays.h MySwap.h Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h \
 MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h \
 MFRotation.h MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h \
 MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h \
 SFImage.h SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h \
 SFString.h SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h \
 SFVec4f.h CommandList.h Command.h DISCommonFields.h DuneApp.h URL.h \
 PreferencesApp.h swt/include/swttypedef.h EcmaScriptApp.h \
 StereoViewApp.h swt/include/stereoview.h InputDeviceApp.h \
 TransformMode.h OutputApp.h RouteViewApp.h StartWithApp.h \
 ExportNamesApp.h ModellingApp.h RenderApp.h
NodeRectangle2D.o: NodeRectangle2D.cpp stdafx.h config.h mysnprintf.h \
 NodeRectangle2D.h MeshBasedNode.h GeometryNode.h Node.h Array.h List.h \
 Vec3f.h Vec3x.h MyString.h StringArray.h FieldIndex.h Element.h \
 x3dFlags.h swt/include/WriteFlags.h NodeFlags.h MeshFlags.h \
 ac3dMaterialCallback.h NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h \
 NodeAppearance.h ProtoMacros.h Proto.h Map.h NodeList.h InterfaceArray.h \
 DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h Arrays.h MySwap.h \
 Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h MFDouble.h \
 SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h MFRotation.h \
 MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h \
 MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h SFImage.h \
 SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h SFString.h \
 SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h \
 CommandList.h Command.h DuneApp.h URL.h PreferencesApp.h \
 swt/include/swttypedef.h EcmaScriptApp.h StereoViewApp.h \
 swt/include/stereoview.h InputDeviceApp.h TransformMode.h OutputApp.h \
 RouteViewApp.h StartWithApp.h ExportNamesApp.h ModellingApp.h \
 RenderApp.h NodeMaterial.h swt/include/swt.h swt/include/keycodes.h \
 swt/include/swlanguages.h swt/include/swDebugf.h \
 swt/include/swttypedef.h NodeImageTexture.h KambiTextureCommonFields.h \
 NodeTextureTransform.h GeometryNodeMacros.h MyMesh.h \
 NodeIndexedFaceSet.h NodeCoordinate.h Colored.h NodeColor.h \
 NodeColorRGBA.h ComposedGeometryMacros.h FaceData.h NodeNormal.h \
 NodeTextureCoordinate.h NodeTextureCoordinateGenerator.h \
 NodeMultiTextureCoordinate.h Scene.h RenderState.h ProtoMap.h Util.h
NodeRigidBody.o: NodeRigidBody.cpp stdafx.h config.h mysnprintf.h \
 NodeRigidBody.h RigidBodyPhysicsNode.h Node.h Array.h List.h Vec3f.h \
 Vec3x.h MyString.h StringArray.h FieldIndex.h Element.h x3dFlags.h \
 swt/include/WriteFlags.h NodeFlags.h MeshFlags.h ac3dMaterialCallback.h \
 NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h ProtoMacros.h Proto.h \
 Map.h NodeList.h InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h \
 MFBool.h FieldValue.h Arrays.h MySwap.h Types.h MFColor.h MFFloat.h \
 SFFloat.h MFColorRGBA.h MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h \
 MFMatrix4f.h MFNode.h MFRotation.h MFString.h MFTime.h SFTime.h \
 MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h \
 SFBool.h SFColor.h SFColorRGBA.h SFImage.h SFInt32.h SFMatrix3f.h \
 SFMatrix4f.h SFNode.h SFRotation.h SFString.h SFVec2d.h SFVec2f.h \
 Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h CommandList.h Command.h \
 DuneApp.h URL.h PreferencesApp.h swt/include/swttypedef.h \
 EcmaScriptApp.h StereoViewApp.h swt/include/stereoview.h \
 InputDeviceApp.h TransformMode.h OutputApp.h RouteViewApp.h \
 StartWithApp.h ExportNamesApp.h ModellingApp.h RenderApp.h RenderState.h \
 Scene.h ProtoMap.h NodeNavigationInfo.h Util.h
NodeRigidBodyCollection.o: NodeRigidBodyCollection.cpp stdafx.h config.h \
 mysnprintf.h NodeRigidBodyCollection.h RigidBodyPhysicsNode.h Node.h \
 Array.h List.h Vec3f.h Vec3x.h MyString.h StringArray.h FieldIndex.h \
 Element.h x3dFlags.h swt/include/WriteFlags.h NodeFlags.h MeshFlags.h \
 ac3dMaterialCallback.h NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h \
 ProtoMacros.h Proto.h Map.h NodeList.h InterfaceArray.h \
 DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h Arrays.h MySwap.h \
 Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h MFDouble.h \
 SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h MFRotation.h \
 MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h \
 MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h SFImage.h \
 SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h SFString.h \
 SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h \
 CommandList.h Command.h Scene.h URL.h DuneApp.h PreferencesApp.h \
 swt/include/swttypedef.h EcmaScriptApp.h StereoViewApp.h \
 swt/include/stereoview.h InputDeviceApp.h TransformMode.h OutputApp.h \
 RouteViewApp.h StartWithApp.h ExportNamesApp.h ModellingApp.h \
 RenderApp.h RenderState.h ProtoMap.h
NodeScalarChaser.o: NodeScalarChaser.cpp stdafx.h config.h mysnprintf.h \
 NodeScalarChaser.h Node.h Array.h List.h Vec3f.h Vec3x.h MyString.h \
 StringArray.h FieldIndex.h Element.h x3dFlags.h swt/include/WriteFlags.h \
 NodeFlags.h MeshFlags.h ac3dMaterialCallback.h NodeArray.h Stack.h \
 Matrix.h Vec3d.h Quaternion.h ProtoMacros.h Proto.h Map.h NodeList.h \
 InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h \
 Arrays.h MySwap.h Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h \
 MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h \
 MFRotation.h MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h \
 MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h \
 SFImage.h SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h \
 SFString.h SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h \
 SFVec4f.h CommandList.h Command.h ChaserNode.h DuneApp.h URL.h \
 PreferencesApp.h swt/include/swttypedef.h EcmaScriptApp.h \
 StereoViewApp.h swt/include/stereoview.h InputDeviceApp.h \
 TransformMode.h OutputApp.h RouteViewApp.h StartWithApp.h \
 ExportNamesApp.h ModellingApp.h RenderApp.h
NodeScalarDamper.o: NodeScalarDamper.cpp stdafx.h config.h mysnprintf.h \
 NodeScalarDamper.h Node.h Array.h List.h Vec3f.h Vec3x.h MyString.h \
 StringArray.h FieldIndex.h Element.h x3dFlags.h swt/include/WriteFlags.h \
 NodeFlags.h MeshFlags.h ac3dMaterialCallback.h NodeArray.h Stack.h \
 Matrix.h Vec3d.h Quaternion.h ProtoMacros.h Proto.h Map.h NodeList.h \
 InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h \
 Arrays.h MySwap.h Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h \
 MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h \
 MFRotation.h MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h \
 MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h \
 SFImage.h SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h \
 SFString.h SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h \
 SFVec4f.h CommandList.h Command.h DamperNode.h DuneApp.h URL.h \
 PreferencesApp.h swt/include/swttypedef.h EcmaScriptApp.h \
 StereoViewApp.h swt/include/stereoview.h InputDeviceApp.h \
 TransformMode.h OutputApp.h RouteViewApp.h StartWithApp.h \
 ExportNamesApp.h ModellingApp.h RenderApp.h
NodeScalarInterpolator.o: NodeScalarInterpolator.cpp stdafx.h config.h \
 mysnprintf.h NodeScalarInterpolator.h Interpolator.h Node.h Array.h \
 List.h Vec3f.h Vec3x.h MyString.h StringArray.h FieldIndex.h Element.h \
 x3dFlags.h swt/include/WriteFlags.h NodeFlags.h MeshFlags.h \
 ac3dMaterialCallback.h NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h \
 ProtoMacros.h Proto.h Map.h NodeList.h InterfaceArray.h \
 DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h Arrays.h MySwap.h \
 Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h MFDouble.h \
 SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h MFRotation.h \
 MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h \
 MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h SFImage.h \
 SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h SFString.h \
 SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h \
 CommandList.h Command.h
NodeScreenFontStyle.o: NodeScreenFontStyle.cpp stdafx.h config.h \
 mysnprintf.h NodeScreenFontStyle.h Node.h Array.h List.h Vec3f.h Vec3x.h \
 MyString.h StringArray.h FieldIndex.h Element.h x3dFlags.h \
 swt/include/WriteFlags.h NodeFlags.h MeshFlags.h ac3dMaterialCallback.h \
 NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h ProtoMacros.h Proto.h \
 Map.h NodeList.h InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h \
 MFBool.h FieldValue.h Arrays.h MySwap.h Types.h MFColor.h MFFloat.h \
 SFFloat.h MFColorRGBA.h MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h \
 MFMatrix4f.h MFNode.h MFRotation.h MFString.h MFTime.h SFTime.h \
 MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h \
 SFBool.h SFColor.h SFColorRGBA.h SFImage.h SFInt32.h SFMatrix3f.h \
 SFMatrix4f.h SFNode.h SFRotation.h SFString.h SFVec2d.h SFVec2f.h \
 Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h CommandList.h Command.h \
 FontStyleNode.h Field.h Scene.h URL.h DuneApp.h PreferencesApp.h \
 swt/include/swttypedef.h EcmaScriptApp.h StereoViewApp.h \
 swt/include/stereoview.h InputDeviceApp.h TransformMode.h OutputApp.h \
 RouteViewApp.h StartWithApp.h ExportNamesApp.h ModellingApp.h \
 RenderApp.h RenderState.h ProtoMap.h
NodeScreenGroup.o: NodeScreenGroup.cpp stdafx.h config.h mysnprintf.h \
 NodeScreenGroup.h Node.h Array.h List.h Vec3f.h Vec3x.h MyString.h \
 StringArray.h FieldIndex.h Element.h x3dFlags.h swt/include/WriteFlags.h \
 NodeFlags.h MeshFlags.h ac3dMaterialCallback.h NodeArray.h Stack.h \
 Matrix.h Vec3d.h Quaternion.h ProtoMacros.h Proto.h Map.h NodeList.h \
 InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h \
 Arrays.h MySwap.h Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h \
 MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h \
 MFRotation.h MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h \
 MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h \
 SFImage.h SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h \
 SFString.h SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h \
 SFVec4f.h CommandList.h Command.h NodeStaticGroup.h GroupNode.h Scene.h \
 URL.h DuneApp.h PreferencesApp.h swt/include/swttypedef.h \
 EcmaScriptApp.h StereoViewApp.h swt/include/stereoview.h \
 InputDeviceApp.h TransformMode.h OutputApp.h RouteViewApp.h \
 StartWithApp.h ExportNamesApp.h ModellingApp.h RenderApp.h RenderState.h \
 ProtoMap.h
NodeScript.o: NodeScript.cpp stdafx.h config.h mysnprintf.h NodeScript.h \
 Node.h Array.h List.h Vec3f.h Vec3x.h MyString.h StringArray.h \
 FieldIndex.h Element.h x3dFlags.h swt/include/WriteFlags.h NodeFlags.h \
 MeshFlags.h ac3dMaterialCallback.h NodeArray.h Stack.h Matrix.h Vec3d.h \
 Quaternion.h ProtoMacros.h Proto.h Map.h NodeList.h InterfaceArray.h \
 DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h Arrays.h MySwap.h \
 Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h MFDouble.h \
 SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h MFRotation.h \
 MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h \
 MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h SFImage.h \
 SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h SFString.h \
 SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h \
 CommandList.h Command.h Scene.h URL.h DuneApp.h PreferencesApp.h \
 swt/include/swttypedef.h EcmaScriptApp.h StereoViewApp.h \
 swt/include/stereoview.h InputDeviceApp.h TransformMode.h OutputApp.h \
 RouteViewApp.h StartWithApp.h ExportNamesApp.h ModellingApp.h \
 RenderApp.h RenderState.h ProtoMap.h ExposedField.h Field.h EventIn.h \
 EventOut.h RouteCommand.h UnRouteCommand.h
NodeShaderPart.o: NodeShaderPart.cpp stdafx.h config.h mysnprintf.h \
 NodeShaderPart.h Node.h Array.h List.h Vec3f.h Vec3x.h MyString.h \
 StringArray.h FieldIndex.h Element.h x3dFlags.h swt/include/WriteFlags.h \
 NodeFlags.h MeshFlags.h ac3dMaterialCallback.h NodeArray.h Stack.h \
 Matrix.h Vec3d.h Quaternion.h ProtoMacros.h Proto.h Map.h NodeList.h \
 InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h \
 Arrays.h MySwap.h Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h \
 MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h \
 MFRotation.h MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h \
 MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h \
 SFImage.h SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h \
 SFString.h SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h \
 SFVec4f.h CommandList.h Command.h DuneApp.h URL.h PreferencesApp.h \
 swt/include/swttypedef.h EcmaScriptApp.h StereoViewApp.h \
 swt/include/stereoview.h InputDeviceApp.h TransformMode.h OutputApp.h \
 RouteViewApp.h StartWithApp.h ExportNamesApp.h ModellingApp.h \
 RenderApp.h
NodeShaderProgram.o: NodeShaderProgram.cpp stdafx.h config.h mysnprintf.h \
 NodeShaderProgram.h Node.h Array.h List.h Vec3f.h Vec3x.h MyString.h \
 StringArray.h FieldIndex.h Element.h x3dFlags.h swt/include/WriteFlags.h \
 NodeFlags.h MeshFlags.h ac3dMaterialCallback.h NodeArray.h Stack.h \
 Matrix.h Vec3d.h Quaternion.h ProtoMacros.h Proto.h Map.h NodeList.h \
 InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h \
 Arrays.h MySwap.h Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h \
 MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h \
 MFRotation.h MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h \
 MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h \
 SFImage.h SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h \
 SFString.h SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h \
 SFVec4f.h CommandList.h Command.h DuneApp.h URL.h PreferencesApp.h \
 swt/include/swttypedef.h EcmaScriptApp.h StereoViewApp.h \
 swt/include/stereoview.h InputDeviceApp.h TransformMode.h OutputApp.h \
 RouteViewApp.h StartWithApp.h ExportNamesApp.h ModellingApp.h \
 RenderApp.h
NodeShape.o: NodeShape.cpp stdafx.h config.h mysnprintf.h NodeShape.h \
 Node.h Array.h List.h Vec3f.h Vec3x.h MyString.h StringArray.h \
 FieldIndex.h Element.h x3dFlags.h swt/include/WriteFlags.h NodeFlags.h \
 MeshFlags.h ac3dMaterialCallback.h NodeArray.h Stack.h Matrix.h Vec3d.h \
 Quaternion.h ProtoMacros.h Proto.h Map.h NodeList.h InterfaceArray.h \
 DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h Arrays.h MySwap.h \
 Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h MFDouble.h \
 SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h MFRotation.h \
 MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h \
 MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h SFImage.h \
 SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h SFString.h \
 SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h \
 CommandList.h Command.h NodeAppearance.h DuneApp.h URL.h \
 PreferencesApp.h swt/include/swttypedef.h EcmaScriptApp.h \
 StereoViewApp.h swt/include/stereoview.h InputDeviceApp.h \
 TransformMode.h OutputApp.h RouteViewApp.h StartWithApp.h \
 ExportNamesApp.h ModellingApp.h RenderApp.h NodeMaterial.h \
 swt/include/swt.h swt/include/keycodes.h swt/include/swlanguages.h \
 swt/include/swDebugf.h swt/include/swttypedef.h NodeNurbsGroup.h \
 NodeGroup.h StaticGroupNode.h GroupNode.h NodeNurbsSet.h Util.h Scene.h \
 RenderState.h ProtoMap.h
NodeSignalPdu.o: NodeSignalPdu.cpp stdafx.h config.h mysnprintf.h \
 NodeSignalPdu.h Node.h Array.h List.h Vec3f.h Vec3x.h MyString.h \
 StringArray.h FieldIndex.h Element.h x3dFlags.h swt/include/WriteFlags.h \
 NodeFlags.h MeshFlags.h ac3dMaterialCallback.h NodeArray.h Stack.h \
 Matrix.h Vec3d.h Quaternion.h ProtoMacros.h Proto.h Map.h NodeList.h \
 InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h \
 Arrays.h MySwap.h Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h \
 MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h \
 MFRotation.h MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h \
 MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h \
 SFImage.h SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h \
 SFString.h SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h \
 SFVec4f.h CommandList.h Command.h DISCommonFields.h DuneApp.h URL.h \
 PreferencesApp.h swt/include/swttypedef.h EcmaScriptApp.h \
 StereoViewApp.h swt/include/stereoview.h InputDeviceApp.h \
 TransformMode.h OutputApp.h RouteViewApp.h StartWithApp.h \
 ExportNamesApp.h ModellingApp.h RenderApp.h
NodeSingleAxisHingeJoint.o: NodeSingleAxisHingeJoint.cpp stdafx.h \
 config.h mysnprintf.h NodeSingleAxisHingeJoint.h AnchoredJointNode.h \
 SFMFTypes.h MFBool.h Array.h FieldValue.h MyString.h StringArray.h \
 Element.h x3dFlags.h swt/include/WriteFlags.h Arrays.h MySwap.h Types.h \
 MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h MFDouble.h SFDouble.h \
 MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h Node.h List.h Vec3f.h \
 Vec3x.h FieldIndex.h NodeFlags.h MeshFlags.h ac3dMaterialCallback.h \
 NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h NodeList.h \
 MFRotation.h MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h \
 MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h \
 SFImage.h SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h \
 SFString.h SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h \
 SFVec4f.h X3DRigidJointNode.h RigidBodyPhysicsNode.h ProtoMacros.h \
 Proto.h Map.h InterfaceArray.h DynamicFieldsNode.h CommandList.h \
 Command.h Field.h DuneApp.h URL.h PreferencesApp.h \
 swt/include/swttypedef.h EcmaScriptApp.h StereoViewApp.h \
 swt/include/stereoview.h InputDeviceApp.h TransformMode.h OutputApp.h \
 RouteViewApp.h StartWithApp.h ExportNamesApp.h ModellingApp.h \
 RenderApp.h Scene.h RenderState.h ProtoMap.h Util.h
NodeSliderJoint.o: NodeSliderJoint.cpp stdafx.h config.h mysnprintf.h \
 NodeSliderJoint.h X3DRigidJointNode.h RigidBodyPhysicsNode.h Node.h \
 Array.h List.h Vec3f.h Vec3x.h MyString.h StringArray.h FieldIndex.h \
 Element.h x3dFlags.h swt/include/WriteFlags.h NodeFlags.h MeshFlags.h \
 ac3dMaterialCallback.h NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h \
 ProtoMacros.h Proto.h Map.h NodeList.h InterfaceArray.h \
 DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h Arrays.h MySwap.h \
 Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h MFDouble.h \
 SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h MFRotation.h \
 MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h \
 MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h SFImage.h \
 SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h SFString.h \
 SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h \
 CommandList.h Command.h Field.h DuneApp.h URL.h PreferencesApp.h \
 swt/include/swttypedef.h EcmaScriptApp.h StereoViewApp.h \
 swt/include/stereoview.h InputDeviceApp.h TransformMode.h OutputApp.h \
 RouteViewApp.h StartWithApp.h ExportNamesApp.h ModellingApp.h \
 RenderApp.h Scene.h RenderState.h ProtoMap.h
NodeSound.o: NodeSound.cpp stdafx.h config.h mysnprintf.h NodeSound.h \
 Node.h Array.h List.h Vec3f.h Vec3x.h MyString.h StringArray.h \
 FieldIndex.h Element.h x3dFlags.h swt/include/WriteFlags.h NodeFlags.h \
 MeshFlags.h ac3dMaterialCallback.h NodeArray.h Stack.h Matrix.h Vec3d.h \
 Quaternion.h ProtoMacros.h Proto.h Map.h NodeList.h InterfaceArray.h \
 DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h Arrays.h MySwap.h \
 Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h MFDouble.h \
 SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h MFRotation.h \
 MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h \
 MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h SFImage.h \
 SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h SFString.h \
 SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h \
 CommandList.h Command.h swt/include/swt.h swt/include/keycodes.h \
 swt/include/swlanguages.h swt/include/swDebugf.h \
 swt/include/swttypedef.h DuneApp.h URL.h PreferencesApp.h \
 swt/include/swttypedef.h EcmaScriptApp.h StereoViewApp.h \
 swt/include/stereoview.h InputDeviceApp.h TransformMode.h OutputApp.h \
 RouteViewApp.h StartWithApp.h ExportNamesApp.h ModellingApp.h \
 RenderApp.h resource.h
NodeSphere.o: NodeSphere.cpp stdafx.h config.h mysnprintf.h NodeSphere.h \
 MeshBasedNode.h GeometryNode.h Node.h Array.h List.h Vec3f.h Vec3x.h \
 MyString.h StringArray.h FieldIndex.h Element.h x3dFlags.h \
 swt/include/WriteFlags.h NodeFlags.h MeshFlags.h ac3dMaterialCallback.h \
 NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h NodeAppearance.h \
 ProtoMacros.h Proto.h Map.h NodeList.h InterfaceArray.h \
 DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h Arrays.h MySwap.h \
 Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h MFDouble.h \
 SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h MFRotation.h \
 MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h \
 MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h SFImage.h \
 SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h SFString.h \
 SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h \
 CommandList.h Command.h DuneApp.h URL.h PreferencesApp.h \
 swt/include/swttypedef.h EcmaScriptApp.h StereoViewApp.h \
 swt/include/stereoview.h InputDeviceApp.h TransformMode.h OutputApp.h \
 RouteViewApp.h StartWithApp.h ExportNamesApp.h ModellingApp.h \
 RenderApp.h NodeMaterial.h swt/include/swt.h swt/include/keycodes.h \
 swt/include/swlanguages.h swt/include/swDebugf.h \
 swt/include/swttypedef.h NodeImageTexture.h KambiTextureCommonFields.h \
 NodeTextureTransform.h GeometryNodeMacros.h Scene.h RenderState.h \
 ProtoMap.h MyMesh.h NodeIndexedFaceSet.h NodeCoordinate.h Colored.h \
 NodeColor.h NodeColorRGBA.h ComposedGeometryMacros.h FaceData.h \
 NodeNormal.h NodeTextureCoordinate.h NodeTextureCoordinateGenerator.h \
 NodeMultiTextureCoordinate.h Util.h NurbsArc.h \
 NurbsMakeRevolvedSurface.h NodeNurbsSurface.h MeshMorphingNode.h \
 Interpolator.h
NodeSphereSensor.o: NodeSphereSensor.cpp stdafx.h config.h mysnprintf.h \
 NodeSphereSensor.h Node.h Array.h List.h Vec3f.h Vec3x.h MyString.h \
 StringArray.h FieldIndex.h Element.h x3dFlags.h swt/include/WriteFlags.h \
 NodeFlags.h MeshFlags.h ac3dMaterialCallback.h NodeArray.h Stack.h \
 Matrix.h Vec3d.h Quaternion.h ProtoMacros.h Proto.h Map.h NodeList.h \
 InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h \
 Arrays.h MySwap.h Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h \
 MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h \
 MFRotation.h MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h \
 MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h \
 SFImage.h SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h \
 SFString.h SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h \
 SFVec4f.h CommandList.h Command.h Field.h
NodeSplinePositionInterpolator.o: NodeSplinePositionInterpolator.cpp \
 stdafx.h config.h mysnprintf.h NodeSplinePositionInterpolator.h Node.h \
 Array.h List.h Vec3f.h Vec3x.h MyString.h StringArray.h FieldIndex.h \
 Element.h x3dFlags.h swt/include/WriteFlags.h NodeFlags.h MeshFlags.h \
 ac3dMaterialCallback.h NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h \
 ProtoMacros.h Proto.h Map.h NodeList.h InterfaceArray.h \
 DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h Arrays.h MySwap.h \
 Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h MFDouble.h \
 SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h MFRotation.h \
 MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h \
 MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h SFImage.h \
 SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h SFString.h \
 SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h \
 CommandList.h Command.h DuneApp.h URL.h PreferencesApp.h \
 swt/include/swttypedef.h EcmaScriptApp.h StereoViewApp.h \
 swt/include/stereoview.h InputDeviceApp.h TransformMode.h OutputApp.h \
 RouteViewApp.h StartWithApp.h ExportNamesApp.h ModellingApp.h \
 RenderApp.h
NodeSplinePositionInterpolator2D.o: NodeSplinePositionInterpolator2D.cpp \
 stdafx.h config.h mysnprintf.h NodeSplinePositionInterpolator2D.h Node.h \
 Array.h List.h Vec3f.h Vec3x.h MyString.h StringArray.h FieldIndex.h \
 Element.h x3dFlags.h swt/include/WriteFlags.h NodeFlags.h MeshFlags.h \
 ac3dMaterialCallback.h NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h \
 ProtoMacros.h Proto.h Map.h NodeList.h InterfaceArray.h \
 DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h Arrays.h MySwap.h \
 Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h MFDouble.h \
 SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h MFRotation.h \
 MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h \
 MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h SFImage.h \
 SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h SFString.h \
 SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h \
 CommandList.h Command.h DuneApp.h URL.h PreferencesApp.h \
 swt/include/swttypedef.h EcmaScriptApp.h StereoViewApp.h \
 swt/include/stereoview.h InputDeviceApp.h TransformMode.h OutputApp.h \
 RouteViewApp.h StartWithApp.h ExportNamesApp.h ModellingApp.h \
 RenderApp.h
NodeSplineScalarInterpolator.o: NodeSplineScalarInterpolator.cpp stdafx.h \
 config.h mysnprintf.h NodeSplineScalarInterpolator.h Node.h Array.h \
 List.h Vec3f.h Vec3x.h MyString.h StringArray.h FieldIndex.h Element.h \
 x3dFlags.h swt/include/WriteFlags.h NodeFlags.h MeshFlags.h \
 ac3dMaterialCallback.h NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h \
 ProtoMacros.h Proto.h Map.h NodeList.h InterfaceArray.h \
 DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h Arrays.h MySwap.h \
 Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h MFDouble.h \
 SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h MFRotation.h \
 MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h \
 MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h SFImage.h \
 SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h SFString.h \
 SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h \
 CommandList.h Command.h DuneApp.h URL.h PreferencesApp.h \
 swt/include/swttypedef.h EcmaScriptApp.h StereoViewApp.h \
 swt/include/stereoview.h InputDeviceApp.h TransformMode.h OutputApp.h \
 RouteViewApp.h StartWithApp.h ExportNamesApp.h ModellingApp.h \
 RenderApp.h
NodeSpotLight.o: NodeSpotLight.cpp stdafx.h config.h mysnprintf.h \
 NodeSpotLight.h Node.h Array.h List.h Vec3f.h Vec3x.h MyString.h \
 StringArray.h FieldIndex.h Element.h x3dFlags.h swt/include/WriteFlags.h \
 NodeFlags.h MeshFlags.h ac3dMaterialCallback.h NodeArray.h Stack.h \
 Matrix.h Vec3d.h Quaternion.h ProtoMacros.h Proto.h Map.h NodeList.h \
 InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h \
 Arrays.h MySwap.h Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h \
 MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h \
 MFRotation.h MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h \
 MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h \
 SFImage.h SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h \
 SFString.h SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h \
 SFVec4f.h CommandList.h Command.h KambiLightCommonFields.h \
 X3domLightCommonFields.h Scene.h URL.h DuneApp.h PreferencesApp.h \
 swt/include/swttypedef.h EcmaScriptApp.h StereoViewApp.h \
 swt/include/stereoview.h InputDeviceApp.h TransformMode.h OutputApp.h \
 RouteViewApp.h StartWithApp.h ExportNamesApp.h ModellingApp.h \
 RenderApp.h RenderState.h ProtoMap.h
NodeSquadOrientationInterpolator.o: NodeSquadOrientationInterpolator.cpp \
 stdafx.h config.h mysnprintf.h NodeSquadOrientationInterpolator.h Node.h \
 Array.h List.h Vec3f.h Vec3x.h MyString.h StringArray.h FieldIndex.h \
 Element.h x3dFlags.h swt/include/WriteFlags.h NodeFlags.h MeshFlags.h \
 ac3dMaterialCallback.h NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h \
 ProtoMacros.h Proto.h Map.h NodeList.h InterfaceArray.h \
 DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h Arrays.h MySwap.h \
 Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h MFDouble.h \
 SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h MFRotation.h \
 MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h \
 MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h SFImage.h \
 SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h SFString.h \
 SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h \
 CommandList.h Command.h DuneApp.h URL.h PreferencesApp.h \
 swt/include/swttypedef.h EcmaScriptApp.h StereoViewApp.h \
 swt/include/stereoview.h InputDeviceApp.h TransformMode.h OutputApp.h \
 RouteViewApp.h StartWithApp.h ExportNamesApp.h ModellingApp.h \
 RenderApp.h
NodeStaticGroup.o: NodeStaticGroup.cpp stdafx.h config.h mysnprintf.h \
 swt/include/swt.h swt/include/keycodes.h swt/include/swlanguages.h \
 swt/include/swDebugf.h swt/include/swttypedef.h NodeStaticGroup.h Node.h \
 Array.h List.h Vec3f.h Vec3x.h MyString.h StringArray.h FieldIndex.h \
 Element.h x3dFlags.h swt/include/WriteFlags.h NodeFlags.h MeshFlags.h \
 ac3dMaterialCallback.h NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h \
 ProtoMacros.h Proto.h Map.h NodeList.h InterfaceArray.h \
 DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h Arrays.h MySwap.h \
 Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h MFDouble.h \
 SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h MFRotation.h \
 MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h \
 MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h SFImage.h \
 SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h SFString.h \
 SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h \
 CommandList.h Command.h GroupNode.h NodeShape.h Scene.h URL.h DuneApp.h \
 PreferencesApp.h swt/include/swttypedef.h EcmaScriptApp.h \
 StereoViewApp.h swt/include/stereoview.h InputDeviceApp.h \
 TransformMode.h OutputApp.h RouteViewApp.h StartWithApp.h \
 ExportNamesApp.h ModellingApp.h RenderApp.h RenderState.h ProtoMap.h
NodeStringSensor.o: NodeStringSensor.cpp stdafx.h config.h mysnprintf.h \
 NodeStringSensor.h Node.h Array.h List.h Vec3f.h Vec3x.h MyString.h \
 StringArray.h FieldIndex.h Element.h x3dFlags.h swt/include/WriteFlags.h \
 NodeFlags.h MeshFlags.h ac3dMaterialCallback.h NodeArray.h Stack.h \
 Matrix.h Vec3d.h Quaternion.h ProtoMacros.h Proto.h Map.h NodeList.h \
 InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h \
 Arrays.h MySwap.h Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h \
 MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h \
 MFRotation.h MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h \
 MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h \
 SFImage.h SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h \
 SFString.h SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h \
 SFVec4f.h CommandList.h Command.h Field.h
NodeSurfaceEmitter.o: NodeSurfaceEmitter.cpp stdafx.h config.h \
 mysnprintf.h NodeSurfaceEmitter.h Node.h Array.h List.h Vec3f.h Vec3x.h \
 MyString.h StringArray.h FieldIndex.h Element.h x3dFlags.h \
 swt/include/WriteFlags.h NodeFlags.h MeshFlags.h ac3dMaterialCallback.h \
 NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h ProtoMacros.h Proto.h \
 Map.h NodeList.h InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h \
 MFBool.h FieldValue.h Arrays.h MySwap.h Types.h MFColor.h MFFloat.h \
 SFFloat.h MFColorRGBA.h MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h \
 MFMatrix4f.h MFNode.h MFRotation.h MFString.h MFTime.h SFTime.h \
 MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h \
 SFBool.h SFColor.h SFColorRGBA.h SFImage.h SFInt32.h SFMatrix3f.h \
 SFMatrix4f.h SFNode.h SFRotation.h SFString.h SFVec2d.h SFVec2f.h \
 Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h CommandList.h Command.h \
 EmitterNode.h NodeAppearance.h DuneApp.h URL.h PreferencesApp.h \
 swt/include/swttypedef.h EcmaScriptApp.h StereoViewApp.h \
 swt/include/stereoview.h InputDeviceApp.h TransformMode.h OutputApp.h \
 RouteViewApp.h StartWithApp.h ExportNamesApp.h ModellingApp.h \
 RenderApp.h NodeMaterial.h swt/include/swt.h swt/include/keycodes.h \
 swt/include/swlanguages.h swt/include/swDebugf.h \
 swt/include/swttypedef.h NodeImageTexture.h KambiTextureCommonFields.h \
 NodeTextureTransform.h
NodeSwitch.o: NodeSwitch.cpp stdafx.h config.h mysnprintf.h NodeSwitch.h \
 Node.h Array.h List.h Vec3f.h Vec3x.h MyString.h StringArray.h \
 FieldIndex.h Element.h x3dFlags.h swt/include/WriteFlags.h NodeFlags.h \
 MeshFlags.h ac3dMaterialCallback.h NodeArray.h Stack.h Matrix.h Vec3d.h \
 Quaternion.h ProtoMacros.h Proto.h Map.h NodeList.h InterfaceArray.h \
 DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h Arrays.h MySwap.h \
 Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h MFDouble.h \
 SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h MFRotation.h \
 MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h \
 MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h SFImage.h \
 SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h SFString.h \
 SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h \
 CommandList.h Command.h Scene.h URL.h DuneApp.h PreferencesApp.h \
 swt/include/swttypedef.h EcmaScriptApp.h StereoViewApp.h \
 swt/include/stereoview.h InputDeviceApp.h TransformMode.h OutputApp.h \
 RouteViewApp.h StartWithApp.h ExportNamesApp.h ModellingApp.h \
 RenderApp.h RenderState.h ProtoMap.h
NodeTexCoordChaser2D.o: NodeTexCoordChaser2D.cpp stdafx.h config.h \
 mysnprintf.h NodeTexCoordChaser2D.h Node.h Array.h List.h Vec3f.h \
 Vec3x.h MyString.h StringArray.h FieldIndex.h Element.h x3dFlags.h \
 swt/include/WriteFlags.h NodeFlags.h MeshFlags.h ac3dMaterialCallback.h \
 NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h ProtoMacros.h Proto.h \
 Map.h NodeList.h InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h \
 MFBool.h FieldValue.h Arrays.h MySwap.h Types.h MFColor.h MFFloat.h \
 SFFloat.h MFColorRGBA.h MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h \
 MFMatrix4f.h MFNode.h MFRotation.h MFString.h MFTime.h SFTime.h \
 MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h \
 SFBool.h SFColor.h SFColorRGBA.h SFImage.h SFInt32.h SFMatrix3f.h \
 SFMatrix4f.h SFNode.h SFRotation.h SFString.h SFVec2d.h SFVec2f.h \
 Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h CommandList.h Command.h \
 ChaserNode.h DuneApp.h URL.h PreferencesApp.h swt/include/swttypedef.h \
 EcmaScriptApp.h StereoViewApp.h swt/include/stereoview.h \
 InputDeviceApp.h TransformMode.h OutputApp.h RouteViewApp.h \
 StartWithApp.h ExportNamesApp.h ModellingApp.h RenderApp.h
NodeTexCoordDamper2D.o: NodeTexCoordDamper2D.cpp stdafx.h config.h \
 mysnprintf.h NodeTexCoordDamper2D.h Node.h Array.h List.h Vec3f.h \
 Vec3x.h MyString.h StringArray.h FieldIndex.h Element.h x3dFlags.h \
 swt/include/WriteFlags.h NodeFlags.h MeshFlags.h ac3dMaterialCallback.h \
 NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h ProtoMacros.h Proto.h \
 Map.h NodeList.h InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h \
 MFBool.h FieldValue.h Arrays.h MySwap.h Types.h MFColor.h MFFloat.h \
 SFFloat.h MFColorRGBA.h MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h \
 MFMatrix4f.h MFNode.h MFRotation.h MFString.h MFTime.h SFTime.h \
 MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h \
 SFBool.h SFColor.h SFColorRGBA.h SFImage.h SFInt32.h SFMatrix3f.h \
 SFMatrix4f.h SFNode.h SFRotation.h SFString.h SFVec2d.h SFVec2f.h \
 Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h CommandList.h Command.h \
 DamperNode.h DuneApp.h URL.h PreferencesApp.h swt/include/swttypedef.h \
 EcmaScriptApp.h StereoViewApp.h swt/include/stereoview.h \
 InputDeviceApp.h TransformMode.h OutputApp.h RouteViewApp.h \
 StartWithApp.h ExportNamesApp.h ModellingApp.h RenderApp.h
NodeText.o: NodeText.cpp stdafx.h config.h mysnprintf.h NodeText.h \
 GeometryNode.h Node.h Array.h List.h Vec3f.h Vec3x.h MyString.h \
 StringArray.h FieldIndex.h Element.h x3dFlags.h swt/include/WriteFlags.h \
 NodeFlags.h MeshFlags.h ac3dMaterialCallback.h NodeArray.h Stack.h \
 Matrix.h Vec3d.h Quaternion.h NodeAppearance.h ProtoMacros.h Proto.h \
 Map.h NodeList.h InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h \
 MFBool.h FieldValue.h Arrays.h MySwap.h Types.h MFColor.h MFFloat.h \
 SFFloat.h MFColorRGBA.h MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h \
 MFMatrix4f.h MFNode.h MFRotation.h MFString.h MFTime.h SFTime.h \
 MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h \
 SFBool.h SFColor.h SFColorRGBA.h SFImage.h SFInt32.h SFMatrix3f.h \
 SFMatrix4f.h SFNode.h SFRotation.h SFString.h SFVec2d.h SFVec2f.h \
 Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h CommandList.h Command.h \
 DuneApp.h URL.h PreferencesApp.h swt/include/swttypedef.h \
 EcmaScriptApp.h StereoViewApp.h swt/include/stereoview.h \
 InputDeviceApp.h TransformMode.h OutputApp.h RouteViewApp.h \
 StartWithApp.h ExportNamesApp.h ModellingApp.h RenderApp.h \
 NodeMaterial.h swt/include/swt.h swt/include/keycodes.h \
 swt/include/swlanguages.h swt/include/swDebugf.h \
 swt/include/swttypedef.h NodeImageTexture.h KambiTextureCommonFields.h \
 NodeTextureTransform.h GeometryNodeMacros.h MeshBasedNode.h Scene.h \
 RenderState.h ProtoMap.h FontInfo.h Util.h MyMesh.h NodeIndexedFaceSet.h \
 NodeCoordinate.h Colored.h NodeColor.h NodeColorRGBA.h \
 ComposedGeometryMacros.h FaceData.h NodeNormal.h NodeTextureCoordinate.h \
 NodeTextureCoordinateGenerator.h NodeMultiTextureCoordinate.h \
 NodeFontStyle.h FontStyleNode.h /usr/include/freetype2/ft2build.h \
 /usr/include/freetype2/freetype/config/ftheader.h \
 /usr/include/freetype2/freetype/freetype.h \
 /usr/include/freetype2/freetype/config/ftconfig.h \
 /usr/include/freetype2/freetype/config/ftoption.h \
 /usr/include/freetype2/freetype/config/ftstdlib.h \
 /usr/include/freetype2/freetype/fttypes.h \
 /usr/include/freetype2/freetype/ftsystem.h \
 /usr/include/freetype2/freetype/ftimage.h \
 /usr/include/freetype2/freetype/fterrors.h \
 /usr/include/freetype2/freetype/ftmoderr.h \
 /usr/include/freetype2/freetype/fterrdef.h \
 /usr/include/freetype2/freetype/ftglyph.h \
 /usr/include/freetype2/freetype/ftoutln.h \
 /usr/include/freetype2/freetype/fttrigon.h PointFtgl.h Vectoriser.h \
 Contour.h poly2tri.h shapes.h cdt.h advancing_front.h sweep_context.h \
 sweep.h
NodeTextureBackground.o: NodeTextureBackground.cpp stdafx.h config.h \
 mysnprintf.h NodeTextureBackground.h Node.h Array.h List.h Vec3f.h \
 Vec3x.h MyString.h StringArray.h FieldIndex.h Element.h x3dFlags.h \
 swt/include/WriteFlags.h NodeFlags.h MeshFlags.h ac3dMaterialCallback.h \
 NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h ProtoMacros.h Proto.h \
 Map.h NodeList.h InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h \
 MFBool.h FieldValue.h Arrays.h MySwap.h Types.h MFColor.h MFFloat.h \
 SFFloat.h MFColorRGBA.h MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h \
 MFMatrix4f.h MFNode.h MFRotation.h MFString.h MFTime.h SFTime.h \
 MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h \
 SFBool.h SFColor.h SFColorRGBA.h SFImage.h SFInt32.h SFMatrix3f.h \
 SFMatrix4f.h SFNode.h SFRotation.h SFString.h SFVec2d.h SFVec2f.h \
 Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h CommandList.h Command.h \
 NodeImageTexture.h KambiTextureCommonFields.h NodeViewpoint.h \
 ViewpointNode.h swt/include/swt.h swt/include/keycodes.h \
 swt/include/swlanguages.h swt/include/swDebugf.h \
 swt/include/swttypedef.h CoverNode.h KambiViewpointCommonFields.h \
 DuneApp.h URL.h PreferencesApp.h swt/include/swttypedef.h \
 EcmaScriptApp.h StereoViewApp.h swt/include/stereoview.h \
 InputDeviceApp.h TransformMode.h OutputApp.h RouteViewApp.h \
 StartWithApp.h ExportNamesApp.h ModellingApp.h RenderApp.h Scene.h \
 RenderState.h ProtoMap.h Util.h Field.h
NodeTextureCoordinate.o: NodeTextureCoordinate.cpp stdafx.h config.h \
 mysnprintf.h NodeTextureCoordinate.h Node.h Array.h List.h Vec3f.h \
 Vec3x.h MyString.h StringArray.h FieldIndex.h Element.h x3dFlags.h \
 swt/include/WriteFlags.h NodeFlags.h MeshFlags.h ac3dMaterialCallback.h \
 NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h ProtoMacros.h Proto.h \
 Map.h NodeList.h InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h \
 MFBool.h FieldValue.h Arrays.h MySwap.h Types.h MFColor.h MFFloat.h \
 SFFloat.h MFColorRGBA.h MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h \
 MFMatrix4f.h MFNode.h MFRotation.h MFString.h MFTime.h SFTime.h \
 MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h \
 SFBool.h SFColor.h SFColorRGBA.h SFImage.h SFInt32.h SFMatrix3f.h \
 SFMatrix4f.h SFNode.h SFRotation.h SFString.h SFVec2d.h SFVec2f.h \
 Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h CommandList.h Command.h \
 Scene.h URL.h DuneApp.h PreferencesApp.h swt/include/swttypedef.h \
 EcmaScriptApp.h StereoViewApp.h swt/include/stereoview.h \
 InputDeviceApp.h TransformMode.h OutputApp.h RouteViewApp.h \
 StartWithApp.h ExportNamesApp.h ModellingApp.h RenderApp.h RenderState.h \
 ProtoMap.h NodeIndexedFaceSet.h MeshBasedNode.h GeometryNode.h \
 NodeAppearance.h NodeMaterial.h swt/include/swt.h swt/include/keycodes.h \
 swt/include/swlanguages.h swt/include/swDebugf.h \
 swt/include/swttypedef.h NodeImageTexture.h KambiTextureCommonFields.h \
 NodeTextureTransform.h GeometryNodeMacros.h NodeCoordinate.h Colored.h \
 NodeColor.h NodeColorRGBA.h ComposedGeometryMacros.h MyMesh.h FaceData.h \
 NodeNormal.h NodeTextureCoordinateGenerator.h \
 NodeMultiTextureCoordinate.h Util.h
NodeTextureCoordinate3D.o: NodeTextureCoordinate3D.cpp stdafx.h config.h \
 mysnprintf.h NodeTextureCoordinate3D.h Node.h Array.h List.h Vec3f.h \
 Vec3x.h MyString.h StringArray.h FieldIndex.h Element.h x3dFlags.h \
 swt/include/WriteFlags.h NodeFlags.h MeshFlags.h ac3dMaterialCallback.h \
 NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h ProtoMacros.h Proto.h \
 Map.h NodeList.h InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h \
 MFBool.h FieldValue.h Arrays.h MySwap.h Types.h MFColor.h MFFloat.h \
 SFFloat.h MFColorRGBA.h MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h \
 MFMatrix4f.h MFNode.h MFRotation.h MFString.h MFTime.h SFTime.h \
 MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h \
 SFBool.h SFColor.h SFColorRGBA.h SFImage.h SFInt32.h SFMatrix3f.h \
 SFMatrix4f.h SFNode.h SFRotation.h SFString.h SFVec2d.h SFVec2f.h \
 Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h CommandList.h Command.h \
 DuneApp.h URL.h PreferencesApp.h swt/include/swttypedef.h \
 EcmaScriptApp.h StereoViewApp.h swt/include/stereoview.h \
 InputDeviceApp.h TransformMode.h OutputApp.h RouteViewApp.h \
 StartWithApp.h ExportNamesApp.h ModellingApp.h RenderApp.h
NodeTextureCoordinate4D.o: NodeTextureCoordinate4D.cpp stdafx.h config.h \
 mysnprintf.h NodeTextureCoordinate4D.h Node.h Array.h List.h Vec3f.h \
 Vec3x.h MyString.h StringArray.h FieldIndex.h Element.h x3dFlags.h \
 swt/include/WriteFlags.h NodeFlags.h MeshFlags.h ac3dMaterialCallback.h \
 NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h ProtoMacros.h Proto.h \
 Map.h NodeList.h InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h \
 MFBool.h FieldValue.h Arrays.h MySwap.h Types.h MFColor.h MFFloat.h \
 SFFloat.h MFColorRGBA.h MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h \
 MFMatrix4f.h MFNode.h MFRotation.h MFString.h MFTime.h SFTime.h \
 MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h \
 SFBool.h SFColor.h SFColorRGBA.h SFImage.h SFInt32.h SFMatrix3f.h \
 SFMatrix4f.h SFNode.h SFRotation.h SFString.h SFVec2d.h SFVec2f.h \
 Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h CommandList.h Command.h \
 DuneApp.h URL.h PreferencesApp.h swt/include/swttypedef.h \
 EcmaScriptApp.h StereoViewApp.h swt/include/stereoview.h \
 InputDeviceApp.h TransformMode.h OutputApp.h RouteViewApp.h \
 StartWithApp.h ExportNamesApp.h ModellingApp.h RenderApp.h
NodeTextureCoordinateGenerator.o: NodeTextureCoordinateGenerator.cpp \
 stdafx.h config.h mysnprintf.h swt/include/swt.h swt/include/keycodes.h \
 swt/include/swlanguages.h swt/include/swDebugf.h \
 swt/include/swttypedef.h DuneApp.h List.h Array.h MyString.h \
 StringArray.h Node.h Vec3f.h Vec3x.h FieldIndex.h Element.h x3dFlags.h \
 swt/include/WriteFlags.h NodeFlags.h MeshFlags.h ac3dMaterialCallback.h \
 NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h URL.h PreferencesApp.h \
 swt/include/swttypedef.h EcmaScriptApp.h StereoViewApp.h \
 swt/include/stereoview.h InputDeviceApp.h TransformMode.h OutputApp.h \
 RouteViewApp.h StartWithApp.h ExportNamesApp.h ModellingApp.h \
 RenderApp.h NodeTextureCoordinateGenerator.h ProtoMacros.h Proto.h Map.h \
 NodeList.h InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h MFBool.h \
 FieldValue.h Arrays.h MySwap.h Types.h MFColor.h MFFloat.h SFFloat.h \
 MFColorRGBA.h MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h \
 MFNode.h MFRotation.h MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h \
 MFVec2f.h MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h \
 SFColorRGBA.h SFImage.h SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h \
 SFRotation.h SFString.h SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h \
 SFVec3f.h SFVec4f.h CommandList.h Command.h Texture.h
NodeTextureProperties.o: NodeTextureProperties.cpp stdafx.h config.h \
 mysnprintf.h NodeTextureProperties.h Node.h Array.h List.h Vec3f.h \
 Vec3x.h MyString.h StringArray.h FieldIndex.h Element.h x3dFlags.h \
 swt/include/WriteFlags.h NodeFlags.h MeshFlags.h ac3dMaterialCallback.h \
 NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h ProtoMacros.h Proto.h \
 Map.h NodeList.h InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h \
 MFBool.h FieldValue.h Arrays.h MySwap.h Types.h MFColor.h MFFloat.h \
 SFFloat.h MFColorRGBA.h MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h \
 MFMatrix4f.h MFNode.h MFRotation.h MFString.h MFTime.h SFTime.h \
 MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h \
 SFBool.h SFColor.h SFColorRGBA.h SFImage.h SFInt32.h SFMatrix3f.h \
 SFMatrix4f.h SFNode.h SFRotation.h SFString.h SFVec2d.h SFVec2f.h \
 Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h CommandList.h Command.h \
 DuneApp.h URL.h PreferencesApp.h swt/include/swttypedef.h \
 EcmaScriptApp.h StereoViewApp.h swt/include/stereoview.h \
 InputDeviceApp.h TransformMode.h OutputApp.h RouteViewApp.h \
 StartWithApp.h ExportNamesApp.h ModellingApp.h RenderApp.h
NodeTextureTransform.o: NodeTextureTransform.cpp stdafx.h config.h \
 mysnprintf.h NodeTextureTransform.h Node.h Array.h List.h Vec3f.h \
 Vec3x.h MyString.h StringArray.h FieldIndex.h Element.h x3dFlags.h \
 swt/include/WriteFlags.h NodeFlags.h MeshFlags.h ac3dMaterialCallback.h \
 NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h ProtoMacros.h Proto.h \
 Map.h NodeList.h InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h \
 MFBool.h FieldValue.h Arrays.h MySwap.h Types.h MFColor.h MFFloat.h \
 SFFloat.h MFColorRGBA.h MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h \
 MFMatrix4f.h MFNode.h MFRotation.h MFString.h MFTime.h SFTime.h \
 MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h \
 SFBool.h SFColor.h SFColorRGBA.h SFImage.h SFInt32.h SFMatrix3f.h \
 SFMatrix4f.h SFNode.h SFRotation.h SFString.h SFVec2d.h SFVec2f.h \
 Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h CommandList.h Command.h \
 Scene.h URL.h DuneApp.h PreferencesApp.h swt/include/swttypedef.h \
 EcmaScriptApp.h StereoViewApp.h swt/include/stereoview.h \
 InputDeviceApp.h TransformMode.h OutputApp.h RouteViewApp.h \
 StartWithApp.h ExportNamesApp.h ModellingApp.h RenderApp.h RenderState.h \
 ProtoMap.h
NodeTextureTransform3D.o: NodeTextureTransform3D.cpp stdafx.h config.h \
 mysnprintf.h NodeTextureTransform3D.h Node.h Array.h List.h Vec3f.h \
 Vec3x.h MyString.h StringArray.h FieldIndex.h Element.h x3dFlags.h \
 swt/include/WriteFlags.h NodeFlags.h MeshFlags.h ac3dMaterialCallback.h \
 NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h ProtoMacros.h Proto.h \
 Map.h NodeList.h InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h \
 MFBool.h FieldValue.h Arrays.h MySwap.h Types.h MFColor.h MFFloat.h \
 SFFloat.h MFColorRGBA.h MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h \
 MFMatrix4f.h MFNode.h MFRotation.h MFString.h MFTime.h SFTime.h \
 MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h \
 SFBool.h SFColor.h SFColorRGBA.h SFImage.h SFInt32.h SFMatrix3f.h \
 SFMatrix4f.h SFNode.h SFRotation.h SFString.h SFVec2d.h SFVec2f.h \
 Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h CommandList.h Command.h \
 DuneApp.h URL.h PreferencesApp.h swt/include/swttypedef.h \
 EcmaScriptApp.h StereoViewApp.h swt/include/stereoview.h \
 InputDeviceApp.h TransformMode.h OutputApp.h RouteViewApp.h \
 StartWithApp.h ExportNamesApp.h ModellingApp.h RenderApp.h
NodeTextureTransformMatrix3D.o: NodeTextureTransformMatrix3D.cpp stdafx.h \
 config.h mysnprintf.h NodeTextureTransformMatrix3D.h Node.h Array.h \
 List.h Vec3f.h Vec3x.h MyString.h StringArray.h FieldIndex.h Element.h \
 x3dFlags.h swt/include/WriteFlags.h NodeFlags.h MeshFlags.h \
 ac3dMaterialCallback.h NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h \
 ProtoMacros.h Proto.h Map.h NodeList.h InterfaceArray.h \
 DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h Arrays.h MySwap.h \
 Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h MFDouble.h \
 SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h MFRotation.h \
 MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h \
 MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h SFImage.h \
 SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h SFString.h \
 SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h \
 CommandList.h Command.h DuneApp.h URL.h PreferencesApp.h \
 swt/include/swttypedef.h EcmaScriptApp.h StereoViewApp.h \
 swt/include/stereoview.h InputDeviceApp.h TransformMode.h OutputApp.h \
 RouteViewApp.h StartWithApp.h ExportNamesApp.h ModellingApp.h \
 RenderApp.h
NodeTimeSensor.o: NodeTimeSensor.cpp stdafx.h config.h mysnprintf.h \
 NodeTimeSensor.h Node.h Array.h List.h Vec3f.h Vec3x.h MyString.h \
 StringArray.h FieldIndex.h Element.h x3dFlags.h swt/include/WriteFlags.h \
 NodeFlags.h MeshFlags.h ac3dMaterialCallback.h NodeArray.h Stack.h \
 Matrix.h Vec3d.h Quaternion.h ProtoMacros.h Proto.h Map.h NodeList.h \
 InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h \
 Arrays.h MySwap.h Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h \
 MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h \
 MFRotation.h MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h \
 MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h \
 SFImage.h SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h \
 SFString.h SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h \
 SFVec4f.h CommandList.h Command.h Scene.h URL.h DuneApp.h \
 PreferencesApp.h swt/include/swttypedef.h EcmaScriptApp.h \
 StereoViewApp.h swt/include/stereoview.h InputDeviceApp.h \
 TransformMode.h OutputApp.h RouteViewApp.h StartWithApp.h \
 ExportNamesApp.h ModellingApp.h RenderApp.h RenderState.h ProtoMap.h \
 Field.h ExposedField.h
NodeTimeTrigger.o: NodeTimeTrigger.cpp stdafx.h config.h mysnprintf.h \
 swt/include/swt.h swt/include/keycodes.h swt/include/swlanguages.h \
 swt/include/swDebugf.h swt/include/swttypedef.h DuneApp.h List.h Array.h \
 MyString.h StringArray.h Node.h Vec3f.h Vec3x.h FieldIndex.h Element.h \
 x3dFlags.h swt/include/WriteFlags.h NodeFlags.h MeshFlags.h \
 ac3dMaterialCallback.h NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h \
 URL.h PreferencesApp.h swt/include/swttypedef.h EcmaScriptApp.h \
 StereoViewApp.h swt/include/stereoview.h InputDeviceApp.h \
 TransformMode.h OutputApp.h RouteViewApp.h StartWithApp.h \
 ExportNamesApp.h ModellingApp.h RenderApp.h NodeTimeTrigger.h \
 ProtoMacros.h Proto.h Map.h NodeList.h InterfaceArray.h \
 DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h Arrays.h MySwap.h \
 Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h MFDouble.h \
 SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h MFRotation.h \
 MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h \
 MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h SFImage.h \
 SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h SFString.h \
 SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h \
 CommandList.h Command.h Texture.h
NodeTouchSensor.o: NodeTouchSensor.cpp stdafx.h config.h mysnprintf.h \
 NodeTouchSensor.h Node.h Array.h List.h Vec3f.h Vec3x.h MyString.h \
 StringArray.h FieldIndex.h Element.h x3dFlags.h swt/include/WriteFlags.h \
 NodeFlags.h MeshFlags.h ac3dMaterialCallback.h NodeArray.h Stack.h \
 Matrix.h Vec3d.h Quaternion.h ProtoMacros.h Proto.h Map.h NodeList.h \
 InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h \
 Arrays.h MySwap.h Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h \
 MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h \
 MFRotation.h MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h \
 MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h \
 SFImage.h SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h \
 SFString.h SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h \
 SFVec4f.h CommandList.h Command.h Field.h
NodeTransform.o: NodeTransform.cpp stdafx.h config.h mysnprintf.h \
 swt/include/swt.h swt/include/keycodes.h swt/include/swlanguages.h \
 swt/include/swDebugf.h swt/include/swttypedef.h NodeTransform.h Node.h \
 Array.h List.h Vec3f.h Vec3x.h MyString.h StringArray.h FieldIndex.h \
 Element.h x3dFlags.h swt/include/WriteFlags.h NodeFlags.h MeshFlags.h \
 ac3dMaterialCallback.h NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h \
 ProtoMacros.h Proto.h Map.h NodeList.h InterfaceArray.h \
 DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h Arrays.h MySwap.h \
 Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h MFDouble.h \
 SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h MFRotation.h \
 MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h \
 MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h SFImage.h \
 SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h SFString.h \
 SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h \
 CommandList.h Command.h TransformNode.h BasicTransformNode.h Scene.h \
 URL.h DuneApp.h PreferencesApp.h swt/include/swttypedef.h \
 EcmaScriptApp.h StereoViewApp.h swt/include/stereoview.h \
 InputDeviceApp.h TransformMode.h OutputApp.h RouteViewApp.h \
 StartWithApp.h ExportNamesApp.h ModellingApp.h RenderApp.h RenderState.h \
 ProtoMap.h FieldCommand.h Util.h NodeViewpoint.h ViewpointNode.h \
 CoverNode.h KambiViewpointCommonFields.h NodeNavigationInfo.h Field.h \
 ExposedField.h TransformFields.h resource.h
NodeTransformSensor.o: NodeTransformSensor.cpp stdafx.h config.h \
 mysnprintf.h NodeTransformSensor.h Node.h Array.h List.h Vec3f.h Vec3x.h \
 MyString.h StringArray.h FieldIndex.h Element.h x3dFlags.h \
 swt/include/WriteFlags.h NodeFlags.h MeshFlags.h ac3dMaterialCallback.h \
 NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h ProtoMacros.h Proto.h \
 Map.h NodeList.h InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h \
 MFBool.h FieldValue.h Arrays.h MySwap.h Types.h MFColor.h MFFloat.h \
 SFFloat.h MFColorRGBA.h MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h \
 MFMatrix4f.h MFNode.h MFRotation.h MFString.h MFTime.h SFTime.h \
 MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h \
 SFBool.h SFColor.h SFColorRGBA.h SFImage.h SFInt32.h SFMatrix3f.h \
 SFMatrix4f.h SFNode.h SFRotation.h SFString.h SFVec2d.h SFVec2f.h \
 Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h CommandList.h Command.h \
 DuneApp.h URL.h PreferencesApp.h swt/include/swttypedef.h \
 EcmaScriptApp.h StereoViewApp.h swt/include/stereoview.h \
 InputDeviceApp.h TransformMode.h OutputApp.h RouteViewApp.h \
 StartWithApp.h ExportNamesApp.h ModellingApp.h RenderApp.h
NodeTransmitterPdu.o: NodeTransmitterPdu.cpp stdafx.h config.h \
 mysnprintf.h NodeTransmitterPdu.h Node.h Array.h List.h Vec3f.h Vec3x.h \
 MyString.h StringArray.h FieldIndex.h Element.h x3dFlags.h \
 swt/include/WriteFlags.h NodeFlags.h MeshFlags.h ac3dMaterialCallback.h \
 NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h ProtoMacros.h Proto.h \
 Map.h NodeList.h InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h \
 MFBool.h FieldValue.h Arrays.h MySwap.h Types.h MFColor.h MFFloat.h \
 SFFloat.h MFColorRGBA.h MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h \
 MFMatrix4f.h MFNode.h MFRotation.h MFString.h MFTime.h SFTime.h \
 MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h \
 SFBool.h SFColor.h SFColorRGBA.h SFImage.h SFInt32.h SFMatrix3f.h \
 SFMatrix4f.h SFNode.h SFRotation.h SFString.h SFVec2d.h SFVec2f.h \
 Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h CommandList.h Command.h \
 DISCommonFields.h DuneApp.h URL.h PreferencesApp.h \
 swt/include/swttypedef.h EcmaScriptApp.h StereoViewApp.h \
 swt/include/stereoview.h InputDeviceApp.h TransformMode.h OutputApp.h \
 RouteViewApp.h StartWithApp.h ExportNamesApp.h ModellingApp.h \
 RenderApp.h
NodeTriangleFanSet.o: NodeTriangleFanSet.cpp stdafx.h config.h \
 mysnprintf.h NodeTriangleFanSet.h MeshBasedNode.h GeometryNode.h Node.h \
 Array.h List.h Vec3f.h Vec3x.h MyString.h StringArray.h FieldIndex.h \
 Element.h x3dFlags.h swt/include/WriteFlags.h NodeFlags.h MeshFlags.h \
 ac3dMaterialCallback.h NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h \
 NodeAppearance.h ProtoMacros.h Proto.h Map.h NodeList.h InterfaceArray.h \
 DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h Arrays.h MySwap.h \
 Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h MFDouble.h \
 SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h MFRotation.h \
 MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h \
 MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h SFImage.h \
 SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h SFString.h \
 SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h \
 CommandList.h Command.h DuneApp.h URL.h PreferencesApp.h \
 swt/include/swttypedef.h EcmaScriptApp.h StereoViewApp.h \
 swt/include/stereoview.h InputDeviceApp.h TransformMode.h OutputApp.h \
 RouteViewApp.h StartWithApp.h ExportNamesApp.h ModellingApp.h \
 RenderApp.h NodeMaterial.h swt/include/swt.h swt/include/keycodes.h \
 swt/include/swlanguages.h swt/include/swDebugf.h \
 swt/include/swttypedef.h NodeImageTexture.h KambiTextureCommonFields.h \
 NodeTextureTransform.h GeometryNodeMacros.h MyMesh.h \
 NodeIndexedFaceSet.h NodeCoordinate.h Colored.h NodeColor.h \
 NodeColorRGBA.h ComposedGeometryMacros.h FaceData.h NodeNormal.h \
 NodeTextureCoordinate.h NodeTextureCoordinateGenerator.h \
 NodeMultiTextureCoordinate.h Scene.h RenderState.h ProtoMap.h Util.h \
 NodeGeoCoordinate.h GeoNode.h NodeFogCoordinate.h NodeIndexedLineSet.h
NodeTriangleSet.o: NodeTriangleSet.cpp stdafx.h config.h mysnprintf.h \
 NodeTriangleSet.h MeshBasedNode.h GeometryNode.h Node.h Array.h List.h \
 Vec3f.h Vec3x.h MyString.h StringArray.h FieldIndex.h Element.h \
 x3dFlags.h swt/include/WriteFlags.h NodeFlags.h MeshFlags.h \
 ac3dMaterialCallback.h NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h \
 NodeAppearance.h ProtoMacros.h Proto.h Map.h NodeList.h InterfaceArray.h \
 DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h Arrays.h MySwap.h \
 Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h MFDouble.h \
 SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h MFRotation.h \
 MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h \
 MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h SFImage.h \
 SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h SFString.h \
 SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h \
 CommandList.h Command.h DuneApp.h URL.h PreferencesApp.h \
 swt/include/swttypedef.h EcmaScriptApp.h StereoViewApp.h \
 swt/include/stereoview.h InputDeviceApp.h TransformMode.h OutputApp.h \
 RouteViewApp.h StartWithApp.h ExportNamesApp.h ModellingApp.h \
 RenderApp.h NodeMaterial.h swt/include/swt.h swt/include/keycodes.h \
 swt/include/swlanguages.h swt/include/swDebugf.h \
 swt/include/swttypedef.h NodeImageTexture.h KambiTextureCommonFields.h \
 NodeTextureTransform.h GeometryNodeMacros.h MyMesh.h \
 NodeIndexedFaceSet.h NodeCoordinate.h Colored.h NodeColor.h \
 NodeColorRGBA.h ComposedGeometryMacros.h FaceData.h NodeNormal.h \
 NodeTextureCoordinate.h NodeTextureCoordinateGenerator.h \
 NodeMultiTextureCoordinate.h Scene.h RenderState.h ProtoMap.h Util.h \
 NodeGeoCoordinate.h GeoNode.h NodeIndexedLineSet.h NodeFogCoordinate.h
NodeTriangleSet2D.o: NodeTriangleSet2D.cpp stdafx.h config.h mysnprintf.h \
 NodeTriangleSet2D.h MeshBasedNode.h GeometryNode.h Node.h Array.h List.h \
 Vec3f.h Vec3x.h MyString.h StringArray.h FieldIndex.h Element.h \
 x3dFlags.h swt/include/WriteFlags.h NodeFlags.h MeshFlags.h \
 ac3dMaterialCallback.h NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h \
 NodeAppearance.h ProtoMacros.h Proto.h Map.h NodeList.h InterfaceArray.h \
 DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h Arrays.h MySwap.h \
 Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h MFDouble.h \
 SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h MFRotation.h \
 MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h \
 MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h SFImage.h \
 SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h SFString.h \
 SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h \
 CommandList.h Command.h DuneApp.h URL.h PreferencesApp.h \
 swt/include/swttypedef.h EcmaScriptApp.h StereoViewApp.h \
 swt/include/stereoview.h InputDeviceApp.h TransformMode.h OutputApp.h \
 RouteViewApp.h StartWithApp.h ExportNamesApp.h ModellingApp.h \
 RenderApp.h NodeMaterial.h swt/include/swt.h swt/include/keycodes.h \
 swt/include/swlanguages.h swt/include/swDebugf.h \
 swt/include/swttypedef.h NodeImageTexture.h KambiTextureCommonFields.h \
 NodeTextureTransform.h GeometryNodeMacros.h MyMesh.h \
 NodeIndexedFaceSet.h NodeCoordinate.h Colored.h NodeColor.h \
 NodeColorRGBA.h ComposedGeometryMacros.h FaceData.h NodeNormal.h \
 NodeTextureCoordinate.h NodeTextureCoordinateGenerator.h \
 NodeMultiTextureCoordinate.h Scene.h RenderState.h ProtoMap.h Util.h \
 NodeIndexedLineSet.h
NodeTriangleStripSet.o: NodeTriangleStripSet.cpp stdafx.h config.h \
 mysnprintf.h NodeTriangleStripSet.h MeshBasedNode.h GeometryNode.h \
 Node.h Array.h List.h Vec3f.h Vec3x.h MyString.h StringArray.h \
 FieldIndex.h Element.h x3dFlags.h swt/include/WriteFlags.h NodeFlags.h \
 MeshFlags.h ac3dMaterialCallback.h NodeArray.h Stack.h Matrix.h Vec3d.h \
 Quaternion.h NodeAppearance.h ProtoMacros.h Proto.h Map.h NodeList.h \
 InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h \
 Arrays.h MySwap.h Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h \
 MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h \
 MFRotation.h MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h \
 MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h \
 SFImage.h SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h \
 SFString.h SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h \
 SFVec4f.h CommandList.h Command.h DuneApp.h URL.h PreferencesApp.h \
 swt/include/swttypedef.h EcmaScriptApp.h StereoViewApp.h \
 swt/include/stereoview.h InputDeviceApp.h TransformMode.h OutputApp.h \
 RouteViewApp.h StartWithApp.h ExportNamesApp.h ModellingApp.h \
 RenderApp.h NodeMaterial.h swt/include/swt.h swt/include/keycodes.h \
 swt/include/swlanguages.h swt/include/swDebugf.h \
 swt/include/swttypedef.h NodeImageTexture.h KambiTextureCommonFields.h \
 NodeTextureTransform.h GeometryNodeMacros.h MyMesh.h \
 NodeIndexedFaceSet.h NodeCoordinate.h Colored.h NodeColor.h \
 NodeColorRGBA.h ComposedGeometryMacros.h FaceData.h NodeNormal.h \
 NodeTextureCoordinate.h NodeTextureCoordinateGenerator.h \
 NodeMultiTextureCoordinate.h Scene.h RenderState.h ProtoMap.h Util.h \
 NodeGeoCoordinate.h GeoNode.h NodeIndexedLineSet.h NodeFogCoordinate.h
NodeTrimmedSurface.o: NodeTrimmedSurface.cpp stdafx.h config.h \
 mysnprintf.h NodeTrimmedSurface.h GeometryNode.h Node.h Array.h List.h \
 Vec3f.h Vec3x.h MyString.h StringArray.h FieldIndex.h Element.h \
 x3dFlags.h swt/include/WriteFlags.h NodeFlags.h MeshFlags.h \
 ac3dMaterialCallback.h NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h \
 NodeAppearance.h ProtoMacros.h Proto.h Map.h NodeList.h InterfaceArray.h \
 DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h Arrays.h MySwap.h \
 Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h MFDouble.h \
 SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h MFRotation.h \
 MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h \
 MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h SFImage.h \
 SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h SFString.h \
 SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h \
 CommandList.h Command.h DuneApp.h URL.h PreferencesApp.h \
 swt/include/swttypedef.h EcmaScriptApp.h StereoViewApp.h \
 swt/include/stereoview.h InputDeviceApp.h TransformMode.h OutputApp.h \
 RouteViewApp.h StartWithApp.h ExportNamesApp.h ModellingApp.h \
 RenderApp.h NodeMaterial.h swt/include/swt.h swt/include/keycodes.h \
 swt/include/swlanguages.h swt/include/swDebugf.h \
 swt/include/swttypedef.h NodeImageTexture.h KambiTextureCommonFields.h \
 NodeTextureTransform.h GeometryNodeMacros.h Scene.h RenderState.h \
 ProtoMap.h NodeNurbsTrimmedSurface.h NodeNurbsSurface.h \
 MeshMorphingNode.h Interpolator.h MeshBasedNode.h NodeIndexedFaceSet.h \
 NodeCoordinate.h Colored.h NodeColor.h NodeColorRGBA.h \
 ComposedGeometryMacros.h MoveCommand.h
NodeTwoSidedMaterial.o: NodeTwoSidedMaterial.cpp stdafx.h config.h \
 mysnprintf.h NodeTwoSidedMaterial.h Node.h Array.h List.h Vec3f.h \
 Vec3x.h MyString.h StringArray.h FieldIndex.h Element.h x3dFlags.h \
 swt/include/WriteFlags.h NodeFlags.h MeshFlags.h ac3dMaterialCallback.h \
 NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h ProtoMacros.h Proto.h \
 Map.h NodeList.h InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h \
 MFBool.h FieldValue.h Arrays.h MySwap.h Types.h MFColor.h MFFloat.h \
 SFFloat.h MFColorRGBA.h MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h \
 MFMatrix4f.h MFNode.h MFRotation.h MFString.h MFTime.h SFTime.h \
 MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h \
 SFBool.h SFColor.h SFColorRGBA.h SFImage.h SFInt32.h SFMatrix3f.h \
 SFMatrix4f.h SFNode.h SFRotation.h SFString.h SFVec2d.h SFVec2f.h \
 Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h CommandList.h Command.h \
 ExposedField.h Field.h Util.h Scene.h URL.h DuneApp.h PreferencesApp.h \
 swt/include/swttypedef.h EcmaScriptApp.h StereoViewApp.h \
 swt/include/stereoview.h InputDeviceApp.h TransformMode.h OutputApp.h \
 RouteViewApp.h StartWithApp.h ExportNamesApp.h ModellingApp.h \
 RenderApp.h RenderState.h ProtoMap.h LdrawDefines.h resource.h \
 swt/include/swt.h swt/include/keycodes.h swt/include/swlanguages.h \
 swt/include/swDebugf.h swt/include/swttypedef.h
NodeUniversalJoint.o: NodeUniversalJoint.cpp stdafx.h config.h \
 mysnprintf.h NodeUniversalJoint.h AnchoredJointNode.h SFMFTypes.h \
 MFBool.h Array.h FieldValue.h MyString.h StringArray.h Element.h \
 x3dFlags.h swt/include/WriteFlags.h Arrays.h MySwap.h Types.h MFColor.h \
 MFFloat.h SFFloat.h MFColorRGBA.h MFDouble.h SFDouble.h MFInt32.h \
 MFMatrix3f.h MFMatrix4f.h MFNode.h Node.h List.h Vec3f.h Vec3x.h \
 FieldIndex.h NodeFlags.h MeshFlags.h ac3dMaterialCallback.h NodeArray.h \
 Stack.h Matrix.h Vec3d.h Quaternion.h NodeList.h MFRotation.h MFString.h \
 MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h MFVec3f.h \
 MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h SFImage.h SFInt32.h \
 SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h SFString.h SFVec2d.h \
 SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h \
 X3DRigidJointNode.h RigidBodyPhysicsNode.h ProtoMacros.h Proto.h Map.h \
 InterfaceArray.h DynamicFieldsNode.h CommandList.h Command.h Field.h \
 DuneApp.h URL.h PreferencesApp.h swt/include/swttypedef.h \
 EcmaScriptApp.h StereoViewApp.h swt/include/stereoview.h \
 InputDeviceApp.h TransformMode.h OutputApp.h RouteViewApp.h \
 StartWithApp.h ExportNamesApp.h ModellingApp.h RenderApp.h Scene.h \
 RenderState.h ProtoMap.h
NodeViewpoint.o: NodeViewpoint.cpp stdafx.h config.h mysnprintf.h \
 DuneApp.h List.h Array.h MyString.h StringArray.h Node.h Vec3f.h Vec3x.h \
 FieldIndex.h Element.h x3dFlags.h swt/include/WriteFlags.h NodeFlags.h \
 MeshFlags.h ac3dMaterialCallback.h NodeArray.h Stack.h Matrix.h Vec3d.h \
 Quaternion.h URL.h PreferencesApp.h swt/include/swttypedef.h \
 EcmaScriptApp.h StereoViewApp.h swt/include/stereoview.h \
 InputDeviceApp.h TransformMode.h OutputApp.h RouteViewApp.h \
 StartWithApp.h ExportNamesApp.h ModellingApp.h RenderApp.h \
 NodeViewpoint.h ProtoMacros.h Proto.h Map.h NodeList.h InterfaceArray.h \
 DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h Arrays.h MySwap.h \
 Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h MFDouble.h \
 SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h MFRotation.h \
 MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h \
 MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h SFImage.h \
 SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h SFString.h \
 SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h \
 CommandList.h Command.h ViewpointNode.h swt/include/swt.h \
 swt/include/keycodes.h swt/include/swlanguages.h swt/include/swDebugf.h \
 swt/include/swttypedef.h CoverNode.h KambiViewpointCommonFields.h \
 Scene.h RenderState.h ProtoMap.h Field.h ExposedField.h resource.h \
 Util.h
NodeViewpointGroup.o: NodeViewpointGroup.cpp stdafx.h config.h \
 mysnprintf.h NodeViewpointGroup.h Node.h Array.h List.h Vec3f.h Vec3x.h \
 MyString.h StringArray.h FieldIndex.h Element.h x3dFlags.h \
 swt/include/WriteFlags.h NodeFlags.h MeshFlags.h ac3dMaterialCallback.h \
 NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h ProtoMacros.h Proto.h \
 Map.h NodeList.h InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h \
 MFBool.h FieldValue.h Arrays.h MySwap.h Types.h MFColor.h MFFloat.h \
 SFFloat.h MFColorRGBA.h MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h \
 MFMatrix4f.h MFNode.h MFRotation.h MFString.h MFTime.h SFTime.h \
 MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h \
 SFBool.h SFColor.h SFColorRGBA.h SFImage.h SFInt32.h SFMatrix3f.h \
 SFMatrix4f.h SFNode.h SFRotation.h SFString.h SFVec2d.h SFVec2f.h \
 Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h CommandList.h Command.h \
 DuneApp.h URL.h PreferencesApp.h swt/include/swttypedef.h \
 EcmaScriptApp.h StereoViewApp.h swt/include/stereoview.h \
 InputDeviceApp.h TransformMode.h OutputApp.h RouteViewApp.h \
 StartWithApp.h ExportNamesApp.h ModellingApp.h RenderApp.h
NodeViewport.o: NodeViewport.cpp stdafx.h config.h mysnprintf.h \
 NodeViewport.h Node.h Array.h List.h Vec3f.h Vec3x.h MyString.h \
 StringArray.h FieldIndex.h Element.h x3dFlags.h swt/include/WriteFlags.h \
 NodeFlags.h MeshFlags.h ac3dMaterialCallback.h NodeArray.h Stack.h \
 Matrix.h Vec3d.h Quaternion.h ProtoMacros.h Proto.h Map.h NodeList.h \
 InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h \
 Arrays.h MySwap.h Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h \
 MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h \
 MFRotation.h MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h \
 MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h \
 SFImage.h SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h \
 SFString.h SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h \
 SFVec4f.h CommandList.h Command.h NodeGroup.h StaticGroupNode.h \
 GroupNode.h Scene.h URL.h DuneApp.h PreferencesApp.h \
 swt/include/swttypedef.h EcmaScriptApp.h StereoViewApp.h \
 swt/include/stereoview.h InputDeviceApp.h TransformMode.h OutputApp.h \
 RouteViewApp.h StartWithApp.h ExportNamesApp.h ModellingApp.h \
 RenderApp.h RenderState.h ProtoMap.h
NodeVisibilitySensor.o: NodeVisibilitySensor.cpp stdafx.h config.h \
 mysnprintf.h NodeVisibilitySensor.h Node.h Array.h List.h Vec3f.h \
 Vec3x.h MyString.h StringArray.h FieldIndex.h Element.h x3dFlags.h \
 swt/include/WriteFlags.h NodeFlags.h MeshFlags.h ac3dMaterialCallback.h \
 NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h ProtoMacros.h Proto.h \
 Map.h NodeList.h InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h \
 MFBool.h FieldValue.h Arrays.h MySwap.h Types.h MFColor.h MFFloat.h \
 SFFloat.h MFColorRGBA.h MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h \
 MFMatrix4f.h MFNode.h MFRotation.h MFString.h MFTime.h SFTime.h \
 MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h \
 SFBool.h SFColor.h SFColorRGBA.h SFImage.h SFInt32.h SFMatrix3f.h \
 SFMatrix4f.h SFNode.h SFRotation.h SFString.h SFVec2d.h SFVec2f.h \
 Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h CommandList.h Command.h \
 Field.h
NodeVolumeEmitter.o: NodeVolumeEmitter.cpp stdafx.h config.h mysnprintf.h \
 NodeVolumeEmitter.h Node.h Array.h List.h Vec3f.h Vec3x.h MyString.h \
 StringArray.h FieldIndex.h Element.h x3dFlags.h swt/include/WriteFlags.h \
 NodeFlags.h MeshFlags.h ac3dMaterialCallback.h NodeArray.h Stack.h \
 Matrix.h Vec3d.h Quaternion.h ProtoMacros.h Proto.h Map.h NodeList.h \
 InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h \
 Arrays.h MySwap.h Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h \
 MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h \
 MFRotation.h MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h \
 MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h \
 SFImage.h SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h \
 SFString.h SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h \
 SFVec4f.h CommandList.h Command.h EmitterNode.h NodeAppearance.h \
 DuneApp.h URL.h PreferencesApp.h swt/include/swttypedef.h \
 EcmaScriptApp.h StereoViewApp.h swt/include/stereoview.h \
 InputDeviceApp.h TransformMode.h OutputApp.h RouteViewApp.h \
 StartWithApp.h ExportNamesApp.h ModellingApp.h RenderApp.h \
 NodeMaterial.h swt/include/swt.h swt/include/keycodes.h \
 swt/include/swlanguages.h swt/include/swDebugf.h \
 swt/include/swttypedef.h NodeImageTexture.h KambiTextureCommonFields.h \
 NodeTextureTransform.h
NodeVolumePickSensor.o: NodeVolumePickSensor.cpp stdafx.h config.h \
 mysnprintf.h NodeVolumePickSensor.h Node.h Array.h List.h Vec3f.h \
 Vec3x.h MyString.h StringArray.h FieldIndex.h Element.h x3dFlags.h \
 swt/include/WriteFlags.h NodeFlags.h MeshFlags.h ac3dMaterialCallback.h \
 NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h ProtoMacros.h Proto.h \
 Map.h NodeList.h InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h \
 MFBool.h FieldValue.h Arrays.h MySwap.h Types.h MFColor.h MFFloat.h \
 SFFloat.h MFColorRGBA.h MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h \
 MFMatrix4f.h MFNode.h MFRotation.h MFString.h MFTime.h SFTime.h \
 MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h \
 SFBool.h SFColor.h SFColorRGBA.h SFImage.h SFInt32.h SFMatrix3f.h \
 SFMatrix4f.h SFNode.h SFRotation.h SFString.h SFVec2d.h SFVec2f.h \
 Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h CommandList.h Command.h \
 DuneApp.h URL.h PreferencesApp.h swt/include/swttypedef.h \
 EcmaScriptApp.h StereoViewApp.h swt/include/stereoview.h \
 InputDeviceApp.h TransformMode.h OutputApp.h RouteViewApp.h \
 StartWithApp.h ExportNamesApp.h ModellingApp.h RenderApp.h
NodeWindPhysicsModel.o: NodeWindPhysicsModel.cpp stdafx.h config.h \
 mysnprintf.h NodeWindPhysicsModel.h Node.h Array.h List.h Vec3f.h \
 Vec3x.h MyString.h StringArray.h FieldIndex.h Element.h x3dFlags.h \
 swt/include/WriteFlags.h NodeFlags.h MeshFlags.h ac3dMaterialCallback.h \
 NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h ProtoMacros.h Proto.h \
 Map.h NodeList.h InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h \
 MFBool.h FieldValue.h Arrays.h MySwap.h Types.h MFColor.h MFFloat.h \
 SFFloat.h MFColorRGBA.h MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h \
 MFMatrix4f.h MFNode.h MFRotation.h MFString.h MFTime.h SFTime.h \
 MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h \
 SFBool.h SFColor.h SFColorRGBA.h SFImage.h SFInt32.h SFMatrix3f.h \
 SFMatrix4f.h SFNode.h SFRotation.h SFString.h SFVec2d.h SFVec2f.h \
 Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h CommandList.h Command.h \
 DuneApp.h URL.h PreferencesApp.h swt/include/swttypedef.h \
 EcmaScriptApp.h StereoViewApp.h swt/include/stereoview.h \
 InputDeviceApp.h TransformMode.h OutputApp.h RouteViewApp.h \
 StartWithApp.h ExportNamesApp.h ModellingApp.h RenderApp.h
NodeEdgeEnhancementVolumeStyle.o: NodeEdgeEnhancementVolumeStyle.cpp \
 stdafx.h config.h mysnprintf.h NodeEdgeEnhancementVolumeStyle.h Node.h \
 Array.h List.h Vec3f.h Vec3x.h MyString.h StringArray.h FieldIndex.h \
 Element.h x3dFlags.h swt/include/WriteFlags.h NodeFlags.h MeshFlags.h \
 ac3dMaterialCallback.h NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h \
 ProtoMacros.h Proto.h Map.h NodeList.h InterfaceArray.h \
 DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h Arrays.h MySwap.h \
 Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h MFDouble.h \
 SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h MFRotation.h \
 MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h \
 MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h SFImage.h \
 SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h SFString.h \
 SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h \
 CommandList.h Command.h DuneApp.h URL.h PreferencesApp.h \
 swt/include/swttypedef.h EcmaScriptApp.h StereoViewApp.h \
 swt/include/stereoview.h InputDeviceApp.h TransformMode.h OutputApp.h \
 RouteViewApp.h StartWithApp.h ExportNamesApp.h ModellingApp.h \
 RenderApp.h
NodeIsoSurfaceVolumeData.o: NodeIsoSurfaceVolumeData.cpp stdafx.h \
 config.h mysnprintf.h NodeIsoSurfaceVolumeData.h Node.h Array.h List.h \
 Vec3f.h Vec3x.h MyString.h StringArray.h FieldIndex.h Element.h \
 x3dFlags.h swt/include/WriteFlags.h NodeFlags.h MeshFlags.h \
 ac3dMaterialCallback.h NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h \
 ProtoMacros.h Proto.h Map.h NodeList.h InterfaceArray.h \
 DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h Arrays.h MySwap.h \
 Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h MFDouble.h \
 SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h MFRotation.h \
 MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h \
 MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h SFImage.h \
 SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h SFString.h \
 SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h \
 CommandList.h Command.h DuneApp.h URL.h PreferencesApp.h \
 swt/include/swttypedef.h EcmaScriptApp.h StereoViewApp.h \
 swt/include/stereoview.h InputDeviceApp.h TransformMode.h OutputApp.h \
 RouteViewApp.h StartWithApp.h ExportNamesApp.h ModellingApp.h \
 RenderApp.h
NodeOpacityMapVolumeStyle.o: NodeOpacityMapVolumeStyle.cpp stdafx.h \
 config.h mysnprintf.h NodeOpacityMapVolumeStyle.h Node.h Array.h List.h \
 Vec3f.h Vec3x.h MyString.h StringArray.h FieldIndex.h Element.h \
 x3dFlags.h swt/include/WriteFlags.h NodeFlags.h MeshFlags.h \
 ac3dMaterialCallback.h NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h \
 ProtoMacros.h Proto.h Map.h NodeList.h InterfaceArray.h \
 DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h Arrays.h MySwap.h \
 Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h MFDouble.h \
 SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h MFRotation.h \
 MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h \
 MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h SFImage.h \
 SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h SFString.h \
 SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h \
 CommandList.h Command.h DuneApp.h URL.h PreferencesApp.h \
 swt/include/swttypedef.h EcmaScriptApp.h StereoViewApp.h \
 swt/include/stereoview.h InputDeviceApp.h TransformMode.h OutputApp.h \
 RouteViewApp.h StartWithApp.h ExportNamesApp.h ModellingApp.h \
 RenderApp.h
NodeSegmentedVolumeData.o: NodeSegmentedVolumeData.cpp stdafx.h config.h \
 mysnprintf.h NodeSegmentedVolumeData.h Node.h Array.h List.h Vec3f.h \
 Vec3x.h MyString.h StringArray.h FieldIndex.h Element.h x3dFlags.h \
 swt/include/WriteFlags.h NodeFlags.h MeshFlags.h ac3dMaterialCallback.h \
 NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h ProtoMacros.h Proto.h \
 Map.h NodeList.h InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h \
 MFBool.h FieldValue.h Arrays.h MySwap.h Types.h MFColor.h MFFloat.h \
 SFFloat.h MFColorRGBA.h MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h \
 MFMatrix4f.h MFNode.h MFRotation.h MFString.h MFTime.h SFTime.h \
 MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h \
 SFBool.h SFColor.h SFColorRGBA.h SFImage.h SFInt32.h SFMatrix3f.h \
 SFMatrix4f.h SFNode.h SFRotation.h SFString.h SFVec2d.h SFVec2f.h \
 Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h CommandList.h Command.h \
 DuneApp.h URL.h PreferencesApp.h swt/include/swttypedef.h \
 EcmaScriptApp.h StereoViewApp.h swt/include/stereoview.h \
 InputDeviceApp.h TransformMode.h OutputApp.h RouteViewApp.h \
 StartWithApp.h ExportNamesApp.h ModellingApp.h RenderApp.h
NodeShadedVolumeStyle.o: NodeShadedVolumeStyle.cpp stdafx.h config.h \
 mysnprintf.h NodeShadedVolumeStyle.h Node.h Array.h List.h Vec3f.h \
 Vec3x.h MyString.h StringArray.h FieldIndex.h Element.h x3dFlags.h \
 swt/include/WriteFlags.h NodeFlags.h MeshFlags.h ac3dMaterialCallback.h \
 NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h ProtoMacros.h Proto.h \
 Map.h NodeList.h InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h \
 MFBool.h FieldValue.h Arrays.h MySwap.h Types.h MFColor.h MFFloat.h \
 SFFloat.h MFColorRGBA.h MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h \
 MFMatrix4f.h MFNode.h MFRotation.h MFString.h MFTime.h SFTime.h \
 MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h \
 SFBool.h SFColor.h SFColorRGBA.h SFImage.h SFInt32.h SFMatrix3f.h \
 SFMatrix4f.h SFNode.h SFRotation.h SFString.h SFVec2d.h SFVec2f.h \
 Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h CommandList.h Command.h \
 DuneApp.h URL.h PreferencesApp.h swt/include/swttypedef.h \
 EcmaScriptApp.h StereoViewApp.h swt/include/stereoview.h \
 InputDeviceApp.h TransformMode.h OutputApp.h RouteViewApp.h \
 StartWithApp.h ExportNamesApp.h ModellingApp.h RenderApp.h
NodeSilhouetteEnhancementVolumeStyle.o: \
 NodeSilhouetteEnhancementVolumeStyle.cpp stdafx.h config.h mysnprintf.h \
 NodeSilhouetteEnhancementVolumeStyle.h Node.h Array.h List.h Vec3f.h \
 Vec3x.h MyString.h StringArray.h FieldIndex.h Element.h x3dFlags.h \
 swt/include/WriteFlags.h NodeFlags.h MeshFlags.h ac3dMaterialCallback.h \
 NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h ProtoMacros.h Proto.h \
 Map.h NodeList.h InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h \
 MFBool.h FieldValue.h Arrays.h MySwap.h Types.h MFColor.h MFFloat.h \
 SFFloat.h MFColorRGBA.h MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h \
 MFMatrix4f.h MFNode.h MFRotation.h MFString.h MFTime.h SFTime.h \
 MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h \
 SFBool.h SFColor.h SFColorRGBA.h SFImage.h SFInt32.h SFMatrix3f.h \
 SFMatrix4f.h SFNode.h SFRotation.h SFString.h SFVec2d.h SFVec2f.h \
 Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h CommandList.h Command.h \
 DuneApp.h URL.h PreferencesApp.h swt/include/swttypedef.h \
 EcmaScriptApp.h StereoViewApp.h swt/include/stereoview.h \
 InputDeviceApp.h TransformMode.h OutputApp.h RouteViewApp.h \
 StartWithApp.h ExportNamesApp.h ModellingApp.h RenderApp.h
NodeToneMappedVolumeStyle.o: NodeToneMappedVolumeStyle.cpp stdafx.h \
 config.h mysnprintf.h NodeToneMappedVolumeStyle.h Node.h Array.h List.h \
 Vec3f.h Vec3x.h MyString.h StringArray.h FieldIndex.h Element.h \
 x3dFlags.h swt/include/WriteFlags.h NodeFlags.h MeshFlags.h \
 ac3dMaterialCallback.h NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h \
 ProtoMacros.h Proto.h Map.h NodeList.h InterfaceArray.h \
 DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h Arrays.h MySwap.h \
 Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h MFDouble.h \
 SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h MFRotation.h \
 MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h \
 MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h SFImage.h \
 SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h SFString.h \
 SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h \
 CommandList.h Command.h DuneApp.h URL.h PreferencesApp.h \
 swt/include/swttypedef.h EcmaScriptApp.h StereoViewApp.h \
 swt/include/stereoview.h InputDeviceApp.h TransformMode.h OutputApp.h \
 RouteViewApp.h StartWithApp.h ExportNamesApp.h ModellingApp.h \
 RenderApp.h
NodeVolumeData.o: NodeVolumeData.cpp stdafx.h config.h mysnprintf.h \
 NodeVolumeData.h Node.h Array.h List.h Vec3f.h Vec3x.h MyString.h \
 StringArray.h FieldIndex.h Element.h x3dFlags.h swt/include/WriteFlags.h \
 NodeFlags.h MeshFlags.h ac3dMaterialCallback.h NodeArray.h Stack.h \
 Matrix.h Vec3d.h Quaternion.h ProtoMacros.h Proto.h Map.h NodeList.h \
 InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h \
 Arrays.h MySwap.h Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h \
 MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h \
 MFRotation.h MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h \
 MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h \
 SFImage.h SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h \
 SFString.h SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h \
 SFVec4f.h CommandList.h Command.h DuneApp.h URL.h PreferencesApp.h \
 swt/include/swttypedef.h EcmaScriptApp.h StereoViewApp.h \
 swt/include/stereoview.h InputDeviceApp.h TransformMode.h OutputApp.h \
 RouteViewApp.h StartWithApp.h ExportNamesApp.h ModellingApp.h \
 RenderApp.h Scene.h RenderState.h ProtoMap.h NodeImageTexture3D.h \
 MeshBasedNode.h GeometryNode.h NodeAppearance.h NodeMaterial.h \
 swt/include/swt.h swt/include/keycodes.h swt/include/swlanguages.h \
 swt/include/swDebugf.h swt/include/swttypedef.h NodeImageTexture.h \
 KambiTextureCommonFields.h NodeTextureTransform.h GeometryNodeMacros.h \
 freewrl_define.h Texture3DNode.h NodePixelTexture3D.h
NodeWorldInfo.o: NodeWorldInfo.cpp stdafx.h config.h mysnprintf.h \
 NodeWorldInfo.h Node.h Array.h List.h Vec3f.h Vec3x.h MyString.h \
 StringArray.h FieldIndex.h Element.h x3dFlags.h swt/include/WriteFlags.h \
 NodeFlags.h MeshFlags.h ac3dMaterialCallback.h NodeArray.h Stack.h \
 Matrix.h Vec3d.h Quaternion.h ProtoMacros.h Proto.h Map.h NodeList.h \
 InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h \
 Arrays.h MySwap.h Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h \
 MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h \
 MFRotation.h MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h \
 MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h \
 SFImage.h SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h \
 SFString.h SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h \
 SFVec4f.h CommandList.h Command.h DuneApp.h URL.h PreferencesApp.h \
 swt/include/swttypedef.h EcmaScriptApp.h StereoViewApp.h \
 swt/include/stereoview.h InputDeviceApp.h TransformMode.h OutputApp.h \
 RouteViewApp.h StartWithApp.h ExportNamesApp.h ModellingApp.h \
 RenderApp.h
NodeComment.o: NodeComment.cpp stdafx.h config.h mysnprintf.h \
 NodeComment.h Node.h Array.h List.h Vec3f.h Vec3x.h MyString.h \
 StringArray.h FieldIndex.h Element.h x3dFlags.h swt/include/WriteFlags.h \
 NodeFlags.h MeshFlags.h ac3dMaterialCallback.h NodeArray.h Stack.h \
 Matrix.h Vec3d.h Quaternion.h ProtoMacros.h Proto.h Map.h NodeList.h \
 InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h \
 Arrays.h MySwap.h Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h \
 MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h \
 MFRotation.h MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h \
 MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h \
 SFImage.h SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h \
 SFString.h SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h \
 SFVec4f.h CommandList.h Command.h Scene.h URL.h DuneApp.h \
 PreferencesApp.h swt/include/swttypedef.h EcmaScriptApp.h \
 StereoViewApp.h swt/include/stereoview.h InputDeviceApp.h \
 TransformMode.h OutputApp.h RouteViewApp.h StartWithApp.h \
 ExportNamesApp.h ModellingApp.h RenderApp.h RenderState.h ProtoMap.h
NodeImport.o: NodeImport.cpp stdafx.h config.h mysnprintf.h NodeImport.h \
 Node.h Array.h List.h Vec3f.h Vec3x.h MyString.h StringArray.h \
 FieldIndex.h Element.h x3dFlags.h swt/include/WriteFlags.h NodeFlags.h \
 MeshFlags.h ac3dMaterialCallback.h NodeArray.h Stack.h Matrix.h Vec3d.h \
 Quaternion.h ProtoMacros.h Proto.h Map.h NodeList.h InterfaceArray.h \
 DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h Arrays.h MySwap.h \
 Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h MFDouble.h \
 SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h MFRotation.h \
 MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h \
 MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h SFImage.h \
 SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h SFString.h \
 SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h \
 CommandList.h Command.h Scene.h URL.h DuneApp.h PreferencesApp.h \
 swt/include/swttypedef.h EcmaScriptApp.h StereoViewApp.h \
 swt/include/stereoview.h InputDeviceApp.h TransformMode.h OutputApp.h \
 RouteViewApp.h StartWithApp.h ExportNamesApp.h ModellingApp.h \
 RenderApp.h RenderState.h ProtoMap.h
NodeExport.o: NodeExport.cpp stdafx.h config.h mysnprintf.h NodeExport.h \
 Node.h Array.h List.h Vec3f.h Vec3x.h MyString.h StringArray.h \
 FieldIndex.h Element.h x3dFlags.h swt/include/WriteFlags.h NodeFlags.h \
 MeshFlags.h ac3dMaterialCallback.h NodeArray.h Stack.h Matrix.h Vec3d.h \
 Quaternion.h ProtoMacros.h Proto.h Map.h NodeList.h InterfaceArray.h \
 DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h Arrays.h MySwap.h \
 Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h MFDouble.h \
 SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h MFRotation.h \
 MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h \
 MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h SFImage.h \
 SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h SFString.h \
 SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h \
 CommandList.h Command.h Scene.h URL.h DuneApp.h PreferencesApp.h \
 swt/include/swttypedef.h EcmaScriptApp.h StereoViewApp.h \
 swt/include/stereoview.h InputDeviceApp.h TransformMode.h OutputApp.h \
 RouteViewApp.h StartWithApp.h ExportNamesApp.h ModellingApp.h \
 RenderApp.h RenderState.h ProtoMap.h
BasicTransformNode.o: BasicTransformNode.cpp stdafx.h config.h \
 mysnprintf.h swt/include/swt.h swt/include/keycodes.h \
 swt/include/swlanguages.h swt/include/swDebugf.h \
 swt/include/swttypedef.h BasicTransformNode.h Node.h Array.h List.h \
 Vec3f.h Vec3x.h MyString.h StringArray.h FieldIndex.h Element.h \
 x3dFlags.h swt/include/WriteFlags.h NodeFlags.h MeshFlags.h \
 ac3dMaterialCallback.h NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h \
 ProtoMacros.h Proto.h Map.h NodeList.h InterfaceArray.h \
 DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h Arrays.h MySwap.h \
 Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h MFDouble.h \
 SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h MFRotation.h \
 MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h \
 MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h SFImage.h \
 SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h SFString.h \
 SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h \
 CommandList.h Command.h Scene.h URL.h DuneApp.h PreferencesApp.h \
 swt/include/swttypedef.h EcmaScriptApp.h StereoViewApp.h \
 swt/include/stereoview.h InputDeviceApp.h TransformMode.h OutputApp.h \
 RouteViewApp.h StartWithApp.h ExportNamesApp.h ModellingApp.h \
 RenderApp.h RenderState.h ProtoMap.h FieldCommand.h Util.h \
 NodeViewpoint.h ViewpointNode.h CoverNode.h KambiViewpointCommonFields.h \
 NodeNavigationInfo.h Field.h ExposedField.h
TransformNode.o: TransformNode.cpp stdafx.h config.h mysnprintf.h \
 swt/include/swt.h swt/include/keycodes.h swt/include/swlanguages.h \
 swt/include/swDebugf.h swt/include/swttypedef.h TransformNode.h \
 BasicTransformNode.h Node.h Array.h List.h Vec3f.h Vec3x.h MyString.h \
 StringArray.h FieldIndex.h Element.h x3dFlags.h swt/include/WriteFlags.h \
 NodeFlags.h MeshFlags.h ac3dMaterialCallback.h NodeArray.h Stack.h \
 Matrix.h Vec3d.h Quaternion.h ProtoMacros.h Proto.h Map.h NodeList.h \
 InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h \
 Arrays.h MySwap.h Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h \
 MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h \
 MFRotation.h MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h \
 MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h \
 SFImage.h SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h \
 SFString.h SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h \
 SFVec4f.h CommandList.h Command.h Scene.h URL.h DuneApp.h \
 PreferencesApp.h swt/include/swttypedef.h EcmaScriptApp.h \
 StereoViewApp.h swt/include/stereoview.h InputDeviceApp.h \
 TransformMode.h OutputApp.h RouteViewApp.h StartWithApp.h \
 ExportNamesApp.h ModellingApp.h RenderApp.h RenderState.h ProtoMap.h \
 FieldCommand.h Util.h NodeViewpoint.h ViewpointNode.h CoverNode.h \
 KambiViewpointCommonFields.h NodeNavigationInfo.h Field.h ExposedField.h
DynamicFieldsNode.o: DynamicFieldsNode.cpp Proto.h Array.h stdafx.h \
 config.h mysnprintf.h Map.h List.h MyString.h StringArray.h Node.h \
 Vec3f.h Vec3x.h FieldIndex.h Element.h x3dFlags.h \
 swt/include/WriteFlags.h NodeFlags.h MeshFlags.h ac3dMaterialCallback.h \
 NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h ProtoMacros.h \
 NodeList.h InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h MFBool.h \
 FieldValue.h Arrays.h MySwap.h Types.h MFColor.h MFFloat.h SFFloat.h \
 MFColorRGBA.h MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h \
 MFNode.h MFRotation.h MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h \
 MFVec2f.h MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h \
 SFColorRGBA.h SFImage.h SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h \
 SFRotation.h SFString.h SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h \
 SFVec3f.h SFVec4f.h CommandList.h Command.h DuneApp.h URL.h \
 PreferencesApp.h swt/include/swttypedef.h EcmaScriptApp.h \
 StereoViewApp.h swt/include/stereoview.h InputDeviceApp.h \
 TransformMode.h OutputApp.h RouteViewApp.h StartWithApp.h \
 ExportNamesApp.h ModellingApp.h RenderApp.h Scene.h RenderState.h \
 ProtoMap.h Field.h ExposedField.h EventIn.h EventOut.h RouteCommand.h \
 UnRouteCommand.h NodeScript.h
GeoNode.o: GeoNode.cpp stdafx.h config.h mysnprintf.h GeoNode.h Node.h \
 Array.h List.h Vec3f.h Vec3x.h MyString.h StringArray.h FieldIndex.h \
 Element.h x3dFlags.h swt/include/WriteFlags.h NodeFlags.h MeshFlags.h \
 ac3dMaterialCallback.h NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h \
 ProtoMacros.h Proto.h Map.h NodeList.h InterfaceArray.h \
 DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h Arrays.h MySwap.h \
 Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h MFDouble.h \
 SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h MFRotation.h \
 MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h \
 MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h SFImage.h \
 SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h SFString.h \
 SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h \
 CommandList.h Command.h GeoMacros.h
GroupNode.o: GroupNode.cpp stdafx.h config.h mysnprintf.h \
 swt/include/swt.h swt/include/keycodes.h swt/include/swlanguages.h \
 swt/include/swDebugf.h swt/include/swttypedef.h GroupNode.h Node.h \
 Array.h List.h Vec3f.h Vec3x.h MyString.h StringArray.h FieldIndex.h \
 Element.h x3dFlags.h swt/include/WriteFlags.h NodeFlags.h MeshFlags.h \
 ac3dMaterialCallback.h NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h \
 ProtoMacros.h Proto.h Map.h NodeList.h InterfaceArray.h \
 DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h Arrays.h MySwap.h \
 Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h MFDouble.h \
 SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h MFRotation.h \
 MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h \
 MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h SFImage.h \
 SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h SFString.h \
 SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h \
 CommandList.h Command.h NodeShape.h Scene.h URL.h DuneApp.h \
 PreferencesApp.h swt/include/swttypedef.h EcmaScriptApp.h \
 StereoViewApp.h swt/include/stereoview.h InputDeviceApp.h \
 TransformMode.h OutputApp.h RouteViewApp.h StartWithApp.h \
 ExportNamesApp.h ModellingApp.h RenderApp.h RenderState.h ProtoMap.h
StaticGroupNode.o: StaticGroupNode.cpp stdafx.h config.h mysnprintf.h \
 swt/include/swt.h swt/include/keycodes.h swt/include/swlanguages.h \
 swt/include/swDebugf.h swt/include/swttypedef.h StaticGroupNode.h Node.h \
 Array.h List.h Vec3f.h Vec3x.h MyString.h StringArray.h FieldIndex.h \
 Element.h x3dFlags.h swt/include/WriteFlags.h NodeFlags.h MeshFlags.h \
 ac3dMaterialCallback.h NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h \
 ProtoMacros.h Proto.h Map.h NodeList.h InterfaceArray.h \
 DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h Arrays.h MySwap.h \
 Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h MFDouble.h \
 SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h MFRotation.h \
 MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h \
 MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h SFImage.h \
 SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h SFString.h \
 SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h \
 CommandList.h Command.h GroupNode.h NodeShape.h Scene.h URL.h DuneApp.h \
 PreferencesApp.h swt/include/swttypedef.h EcmaScriptApp.h \
 StereoViewApp.h swt/include/stereoview.h InputDeviceApp.h \
 TransformMode.h OutputApp.h RouteViewApp.h StartWithApp.h \
 ExportNamesApp.h ModellingApp.h RenderApp.h RenderState.h ProtoMap.h
MeshBasedNode.o: MeshBasedNode.cpp swt/include/swt.h \
 swt/include/keycodes.h swt/include/swlanguages.h swt/include/swDebugf.h \
 swt/include/swttypedef.h resource.h stdafx.h config.h mysnprintf.h \
 MeshBasedNode.h GeometryNode.h Node.h Array.h List.h Vec3f.h Vec3x.h \
 MyString.h StringArray.h FieldIndex.h Element.h x3dFlags.h \
 swt/include/WriteFlags.h NodeFlags.h MeshFlags.h ac3dMaterialCallback.h \
 NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h NodeAppearance.h \
 ProtoMacros.h Proto.h Map.h NodeList.h InterfaceArray.h \
 DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h Arrays.h MySwap.h \
 Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h MFDouble.h \
 SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h MFRotation.h \
 MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h \
 MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h SFImage.h \
 SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h SFString.h \
 SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h \
 CommandList.h Command.h DuneApp.h URL.h PreferencesApp.h \
 swt/include/swttypedef.h EcmaScriptApp.h StereoViewApp.h \
 swt/include/stereoview.h InputDeviceApp.h TransformMode.h OutputApp.h \
 RouteViewApp.h StartWithApp.h ExportNamesApp.h ModellingApp.h \
 RenderApp.h NodeMaterial.h NodeImageTexture.h KambiTextureCommonFields.h \
 NodeTextureTransform.h GeometryNodeMacros.h Scene.h RenderState.h \
 ProtoMap.h MyMesh.h NodeIndexedFaceSet.h NodeCoordinate.h Colored.h \
 NodeColor.h NodeColorRGBA.h ComposedGeometryMacros.h FaceData.h \
 NodeNormal.h NodeTextureCoordinate.h NodeTextureCoordinateGenerator.h \
 NodeMultiTextureCoordinate.h Util.h MeshSubdivide.h NodeShape.h \
 NodeIndexedTriangleSet.h IndexedTriangleSetNode.h NodeTriangleSet.h \
 NodeElevationGrid.h NodeGeoElevationGrid.h GeoNode.h NodeGeoOrigin.h \
 NodeCone.h NodeMultiTexture.h NodeViewpoint.h ViewpointNode.h \
 CoverNode.h KambiViewpointCommonFields.h LdrawDefines.h
MeshMorphingNode.o: MeshMorphingNode.cpp MeshMorphingNode.h Array.h \
 stdafx.h config.h mysnprintf.h Interpolator.h Node.h List.h Vec3f.h \
 Vec3x.h MyString.h StringArray.h FieldIndex.h Element.h x3dFlags.h \
 swt/include/WriteFlags.h NodeFlags.h MeshFlags.h ac3dMaterialCallback.h \
 NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h ProtoMacros.h Proto.h \
 Map.h NodeList.h InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h \
 MFBool.h FieldValue.h Arrays.h MySwap.h Types.h MFColor.h MFFloat.h \
 SFFloat.h MFColorRGBA.h MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h \
 MFMatrix4f.h MFNode.h MFRotation.h MFString.h MFTime.h SFTime.h \
 MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h \
 SFBool.h SFColor.h SFColorRGBA.h SFImage.h SFInt32.h SFMatrix3f.h \
 SFMatrix4f.h SFNode.h SFRotation.h SFString.h SFVec2d.h SFVec2f.h \
 Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h CommandList.h Command.h \
 MeshBasedNode.h GeometryNode.h NodeAppearance.h DuneApp.h URL.h \
 PreferencesApp.h swt/include/swttypedef.h EcmaScriptApp.h \
 StereoViewApp.h swt/include/stereoview.h InputDeviceApp.h \
 TransformMode.h OutputApp.h RouteViewApp.h StartWithApp.h \
 ExportNamesApp.h ModellingApp.h RenderApp.h NodeMaterial.h \
 swt/include/swt.h swt/include/keycodes.h swt/include/swlanguages.h \
 swt/include/swDebugf.h swt/include/swttypedef.h NodeImageTexture.h \
 KambiTextureCommonFields.h NodeTextureTransform.h GeometryNodeMacros.h \
 MyMesh.h NodeIndexedFaceSet.h NodeCoordinate.h Colored.h NodeColor.h \
 NodeColorRGBA.h ComposedGeometryMacros.h FaceData.h NodeNormal.h \
 NodeTextureCoordinate.h NodeTextureCoordinateGenerator.h \
 NodeMultiTextureCoordinate.h Scene.h RenderState.h ProtoMap.h Util.h \
 Field.h NodeCoordinateInterpolator.h NodeGroup.h StaticGroupNode.h \
 GroupNode.h NodeNormalInterpolator.h EventOut.h
ChainBasedNode.o: ChainBasedNode.cpp stdafx.h config.h mysnprintf.h \
 Scene.h Array.h Map.h List.h Stack.h CommandList.h Command.h NodeList.h \
 NodeArray.h Node.h Vec3f.h Vec3x.h MyString.h StringArray.h FieldIndex.h \
 Element.h x3dFlags.h swt/include/WriteFlags.h NodeFlags.h MeshFlags.h \
 ac3dMaterialCallback.h Matrix.h Vec3d.h Quaternion.h URL.h Proto.h \
 ProtoMacros.h InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h MFBool.h \
 FieldValue.h Arrays.h MySwap.h Types.h MFColor.h MFFloat.h SFFloat.h \
 MFColorRGBA.h MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h \
 MFNode.h MFRotation.h MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h \
 MFVec2f.h MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h \
 SFColorRGBA.h SFImage.h SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h \
 SFRotation.h SFString.h SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h \
 SFVec3f.h SFVec4f.h DuneApp.h PreferencesApp.h swt/include/swttypedef.h \
 EcmaScriptApp.h StereoViewApp.h swt/include/stereoview.h \
 InputDeviceApp.h TransformMode.h OutputApp.h RouteViewApp.h \
 StartWithApp.h ExportNamesApp.h ModellingApp.h RenderApp.h RenderState.h \
 ProtoMap.h Util.h MoveCommand.h ChainBasedNode.h GeometryNode.h \
 NodeAppearance.h NodeMaterial.h swt/include/swt.h swt/include/keycodes.h \
 swt/include/swlanguages.h swt/include/swDebugf.h \
 swt/include/swttypedef.h NodeImageTexture.h KambiTextureCommonFields.h \
 NodeTextureTransform.h GeometryNodeMacros.h NodeCoordinate.h NodeShape.h \
 NodeIndexedLineSet.h Colored.h NodeColor.h NodeColorRGBA.h \
 NodePositionInterpolator.h Interpolator.h NodeOrientationInterpolator.h \
 NodeCurveAnimation.h NodeNurbsCurve.h NurbsCurve.h LdrawDefines.h
EmitterNode.o: EmitterNode.cpp stdafx.h config.h mysnprintf.h \
 EmitterNode.h Node.h Array.h List.h Vec3f.h Vec3x.h MyString.h \
 StringArray.h FieldIndex.h Element.h x3dFlags.h swt/include/WriteFlags.h \
 NodeFlags.h MeshFlags.h ac3dMaterialCallback.h NodeArray.h Stack.h \
 Matrix.h Vec3d.h Quaternion.h NodeAppearance.h ProtoMacros.h Proto.h \
 Map.h NodeList.h InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h \
 MFBool.h FieldValue.h Arrays.h MySwap.h Types.h MFColor.h MFFloat.h \
 SFFloat.h MFColorRGBA.h MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h \
 MFMatrix4f.h MFNode.h MFRotation.h MFString.h MFTime.h SFTime.h \
 MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h \
 SFBool.h SFColor.h SFColorRGBA.h SFImage.h SFInt32.h SFMatrix3f.h \
 SFMatrix4f.h SFNode.h SFRotation.h SFString.h SFVec2d.h SFVec2f.h \
 Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h CommandList.h Command.h \
 DuneApp.h URL.h PreferencesApp.h swt/include/swttypedef.h \
 EcmaScriptApp.h StereoViewApp.h swt/include/stereoview.h \
 InputDeviceApp.h TransformMode.h OutputApp.h RouteViewApp.h \
 StartWithApp.h ExportNamesApp.h ModellingApp.h RenderApp.h \
 NodeMaterial.h swt/include/swt.h swt/include/keycodes.h \
 swt/include/swlanguages.h swt/include/swDebugf.h \
 swt/include/swttypedef.h NodeImageTexture.h KambiTextureCommonFields.h \
 NodeTextureTransform.h
GeometryNode.o: GeometryNode.cpp GeometryNode.h Node.h Array.h stdafx.h \
 config.h mysnprintf.h List.h Vec3f.h Vec3x.h MyString.h StringArray.h \
 FieldIndex.h Element.h x3dFlags.h swt/include/WriteFlags.h NodeFlags.h \
 MeshFlags.h ac3dMaterialCallback.h NodeArray.h Stack.h Matrix.h Vec3d.h \
 Quaternion.h NodeAppearance.h ProtoMacros.h Proto.h Map.h NodeList.h \
 InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h \
 Arrays.h MySwap.h Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h \
 MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h \
 MFRotation.h MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h \
 MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h \
 SFImage.h SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h \
 SFString.h SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h \
 SFVec4f.h CommandList.h Command.h DuneApp.h URL.h PreferencesApp.h \
 swt/include/swttypedef.h EcmaScriptApp.h StereoViewApp.h \
 swt/include/stereoview.h InputDeviceApp.h TransformMode.h OutputApp.h \
 RouteViewApp.h StartWithApp.h ExportNamesApp.h ModellingApp.h \
 RenderApp.h NodeMaterial.h swt/include/swt.h swt/include/keycodes.h \
 swt/include/swlanguages.h swt/include/swDebugf.h \
 swt/include/swttypedef.h NodeImageTexture.h KambiTextureCommonFields.h \
 NodeTextureTransform.h GeometryNodeMacros.h NodeShape.h
IndexedTriangleSetNode.o: IndexedTriangleSetNode.cpp stdafx.h config.h \
 mysnprintf.h NodeIndexedTriangleFanSet.h MeshBasedNode.h GeometryNode.h \
 Node.h Array.h List.h Vec3f.h Vec3x.h MyString.h StringArray.h \
 FieldIndex.h Element.h x3dFlags.h swt/include/WriteFlags.h NodeFlags.h \
 MeshFlags.h ac3dMaterialCallback.h NodeArray.h Stack.h Matrix.h Vec3d.h \
 Quaternion.h NodeAppearance.h ProtoMacros.h Proto.h Map.h NodeList.h \
 InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h \
 Arrays.h MySwap.h Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h \
 MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h \
 MFRotation.h MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h \
 MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h \
 SFImage.h SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h \
 SFString.h SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h \
 SFVec4f.h CommandList.h Command.h DuneApp.h URL.h PreferencesApp.h \
 swt/include/swttypedef.h EcmaScriptApp.h StereoViewApp.h \
 swt/include/stereoview.h InputDeviceApp.h TransformMode.h OutputApp.h \
 RouteViewApp.h StartWithApp.h ExportNamesApp.h ModellingApp.h \
 RenderApp.h NodeMaterial.h swt/include/swt.h swt/include/keycodes.h \
 swt/include/swlanguages.h swt/include/swDebugf.h \
 swt/include/swttypedef.h NodeImageTexture.h KambiTextureCommonFields.h \
 NodeTextureTransform.h GeometryNodeMacros.h MyMesh.h \
 NodeIndexedFaceSet.h NodeCoordinate.h Colored.h NodeColor.h \
 NodeColorRGBA.h ComposedGeometryMacros.h FaceData.h NodeNormal.h \
 NodeTextureCoordinate.h NodeTextureCoordinateGenerator.h \
 NodeMultiTextureCoordinate.h Scene.h RenderState.h ProtoMap.h Util.h \
 IndexedTriangleSetNode.h NodeGeoCoordinate.h GeoNode.h \
 NodeIndexedLineSet.h
X3DNBodyCollidableNode.o: X3DNBodyCollidableNode.cpp stdafx.h config.h \
 mysnprintf.h X3DNBodyCollidableNode.h RigidBodyPhysicsNode.h Node.h \
 Array.h List.h Vec3f.h Vec3x.h MyString.h StringArray.h FieldIndex.h \
 Element.h x3dFlags.h swt/include/WriteFlags.h NodeFlags.h MeshFlags.h \
 ac3dMaterialCallback.h NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h \
 ProtoMacros.h Proto.h Map.h NodeList.h InterfaceArray.h \
 DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h Arrays.h MySwap.h \
 Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h MFDouble.h \
 SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h MFRotation.h \
 MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h \
 MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h SFImage.h \
 SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h SFString.h \
 SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h \
 CommandList.h Command.h Util.h ExposedField.h Field.h DuneApp.h URL.h \
 PreferencesApp.h swt/include/swttypedef.h EcmaScriptApp.h \
 StereoViewApp.h swt/include/stereoview.h InputDeviceApp.h \
 TransformMode.h OutputApp.h RouteViewApp.h StartWithApp.h \
 ExportNamesApp.h ModellingApp.h RenderApp.h Scene.h RenderState.h \
 ProtoMap.h NodeNavigationInfo.h NodeRigidBody.h
X3DNBodyCollisionSpaceNode.o: X3DNBodyCollisionSpaceNode.cpp stdafx.h \
 config.h mysnprintf.h X3DNBodyCollisionSpaceNode.h \
 RigidBodyPhysicsNode.h Node.h Array.h List.h Vec3f.h Vec3x.h MyString.h \
 StringArray.h FieldIndex.h Element.h x3dFlags.h swt/include/WriteFlags.h \
 NodeFlags.h MeshFlags.h ac3dMaterialCallback.h NodeArray.h Stack.h \
 Matrix.h Vec3d.h Quaternion.h ProtoMacros.h Proto.h Map.h NodeList.h \
 InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h \
 Arrays.h MySwap.h Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h \
 MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h \
 MFRotation.h MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h \
 MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h \
 SFImage.h SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h \
 SFString.h SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h \
 SFVec4f.h CommandList.h Command.h ExposedField.h Field.h DuneApp.h URL.h \
 PreferencesApp.h swt/include/swttypedef.h EcmaScriptApp.h \
 StereoViewApp.h swt/include/stereoview.h InputDeviceApp.h \
 TransformMode.h OutputApp.h RouteViewApp.h StartWithApp.h \
 ExportNamesApp.h ModellingApp.h RenderApp.h Scene.h RenderState.h \
 ProtoMap.h
X3DRigidJointNode.o: X3DRigidJointNode.cpp stdafx.h config.h mysnprintf.h \
 X3DRigidJointNode.h RigidBodyPhysicsNode.h Node.h Array.h List.h Vec3f.h \
 Vec3x.h MyString.h StringArray.h FieldIndex.h Element.h x3dFlags.h \
 swt/include/WriteFlags.h NodeFlags.h MeshFlags.h ac3dMaterialCallback.h \
 NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h ProtoMacros.h Proto.h \
 Map.h NodeList.h InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h \
 MFBool.h FieldValue.h Arrays.h MySwap.h Types.h MFColor.h MFFloat.h \
 SFFloat.h MFColorRGBA.h MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h \
 MFMatrix4f.h MFNode.h MFRotation.h MFString.h MFTime.h SFTime.h \
 MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h \
 SFBool.h SFColor.h SFColorRGBA.h SFImage.h SFInt32.h SFMatrix3f.h \
 SFMatrix4f.h SFNode.h SFRotation.h SFString.h SFVec2d.h SFVec2f.h \
 Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h CommandList.h Command.h \
 ExposedField.h Field.h DuneApp.h URL.h PreferencesApp.h \
 swt/include/swttypedef.h EcmaScriptApp.h StereoViewApp.h \
 swt/include/stereoview.h InputDeviceApp.h TransformMode.h OutputApp.h \
 RouteViewApp.h StartWithApp.h ExportNamesApp.h ModellingApp.h \
 RenderApp.h Scene.h RenderState.h ProtoMap.h NodeRigidBody.h
AnchoredJointNode.o: AnchoredJointNode.cpp AnchoredJointNode.h \
 SFMFTypes.h MFBool.h Array.h stdafx.h config.h mysnprintf.h FieldValue.h \
 MyString.h StringArray.h Element.h x3dFlags.h swt/include/WriteFlags.h \
 Arrays.h MySwap.h Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h \
 MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h \
 Node.h List.h Vec3f.h Vec3x.h FieldIndex.h NodeFlags.h MeshFlags.h \
 ac3dMaterialCallback.h NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h \
 NodeList.h MFRotation.h MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h \
 MFVec2f.h MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h \
 SFColorRGBA.h SFImage.h SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h \
 SFRotation.h SFString.h SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h \
 SFVec3f.h SFVec4f.h X3DRigidJointNode.h RigidBodyPhysicsNode.h \
 ProtoMacros.h Proto.h Map.h InterfaceArray.h DynamicFieldsNode.h \
 CommandList.h Command.h Scene.h URL.h DuneApp.h PreferencesApp.h \
 swt/include/swttypedef.h EcmaScriptApp.h StereoViewApp.h \
 swt/include/stereoview.h InputDeviceApp.h TransformMode.h OutputApp.h \
 RouteViewApp.h StartWithApp.h ExportNamesApp.h ModellingApp.h \
 RenderApp.h RenderState.h ProtoMap.h NodeNavigationInfo.h
Texture3DNode.o: Texture3DNode.cpp stdafx.h config.h mysnprintf.h \
 NodeImageTexture3D.h MeshBasedNode.h GeometryNode.h Node.h Array.h \
 List.h Vec3f.h Vec3x.h MyString.h StringArray.h FieldIndex.h Element.h \
 x3dFlags.h swt/include/WriteFlags.h NodeFlags.h MeshFlags.h \
 ac3dMaterialCallback.h NodeArray.h Stack.h Matrix.h Vec3d.h Quaternion.h \
 NodeAppearance.h ProtoMacros.h Proto.h Map.h NodeList.h InterfaceArray.h \
 DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h Arrays.h MySwap.h \
 Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h MFDouble.h \
 SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h MFRotation.h \
 MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h \
 MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h SFImage.h \
 SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h SFString.h \
 SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h \
 CommandList.h Command.h DuneApp.h URL.h PreferencesApp.h \
 swt/include/swttypedef.h EcmaScriptApp.h StereoViewApp.h \
 swt/include/stereoview.h InputDeviceApp.h TransformMode.h OutputApp.h \
 RouteViewApp.h StartWithApp.h ExportNamesApp.h ModellingApp.h \
 RenderApp.h NodeMaterial.h swt/include/swt.h swt/include/keycodes.h \
 swt/include/swlanguages.h swt/include/swDebugf.h \
 swt/include/swttypedef.h NodeImageTexture.h KambiTextureCommonFields.h \
 NodeTextureTransform.h GeometryNodeMacros.h freewrl_define.h \
 Texture3DNode.h NodeViewpoint.h ViewpointNode.h CoverNode.h \
 KambiViewpointCommonFields.h NodeTransform.h TransformNode.h \
 BasicTransformNode.h Scene.h RenderState.h ProtoMap.h MyMesh.h \
 NodeIndexedFaceSet.h NodeCoordinate.h Colored.h NodeColor.h \
 NodeColorRGBA.h ComposedGeometryMacros.h FaceData.h NodeNormal.h \
 NodeTextureCoordinate.h NodeTextureCoordinateGenerator.h \
 NodeMultiTextureCoordinate.h Util.h NodeVolumeData.h
FontStyleNode.o: FontStyleNode.cpp stdafx.h config.h mysnprintf.h \
 FontStyleNode.h Node.h Array.h List.h Vec3f.h Vec3x.h MyString.h \
 StringArray.h FieldIndex.h Element.h x3dFlags.h swt/include/WriteFlags.h \
 NodeFlags.h MeshFlags.h ac3dMaterialCallback.h NodeArray.h Stack.h \
 Matrix.h Vec3d.h Quaternion.h ProtoMacros.h Proto.h Map.h NodeList.h \
 InterfaceArray.h DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h \
 Arrays.h MySwap.h Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h \
 MFDouble.h SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h \
 MFRotation.h MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h \
 MFVec3d.h MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h \
 SFImage.h SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h \
 SFString.h SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h \
 SFVec4f.h CommandList.h Command.h Field.h
ViewpointNode.o: ViewpointNode.cpp stdafx.h config.h mysnprintf.h \
 DuneApp.h List.h Array.h MyString.h StringArray.h Node.h Vec3f.h Vec3x.h \
 FieldIndex.h Element.h x3dFlags.h swt/include/WriteFlags.h NodeFlags.h \
 MeshFlags.h ac3dMaterialCallback.h NodeArray.h Stack.h Matrix.h Vec3d.h \
 Quaternion.h URL.h PreferencesApp.h swt/include/swttypedef.h \
 EcmaScriptApp.h StereoViewApp.h swt/include/stereoview.h \
 InputDeviceApp.h TransformMode.h OutputApp.h RouteViewApp.h \
 StartWithApp.h ExportNamesApp.h ModellingApp.h RenderApp.h \
 ViewpointNode.h ProtoMacros.h Proto.h Map.h NodeList.h InterfaceArray.h \
 DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h Arrays.h MySwap.h \
 Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h MFDouble.h \
 SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h MFRotation.h \
 MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h \
 MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h SFImage.h \
 SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h SFString.h \
 SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h \
 CommandList.h Command.h swt/include/swt.h swt/include/keycodes.h \
 swt/include/swlanguages.h swt/include/swDebugf.h \
 swt/include/swttypedef.h CoverNode.h KambiViewpointCommonFields.h \
 Scene.h RenderState.h ProtoMap.h Field.h ExposedField.h resource.h \
 Util.h
DamperNode.o: DamperNode.cpp stdafx.h config.h mysnprintf.h DamperNode.h \
 Node.h Array.h List.h Vec3f.h Vec3x.h MyString.h StringArray.h \
 FieldIndex.h Element.h x3dFlags.h swt/include/WriteFlags.h NodeFlags.h \
 MeshFlags.h ac3dMaterialCallback.h NodeArray.h Stack.h Matrix.h Vec3d.h \
 Quaternion.h ProtoMacros.h Proto.h Map.h NodeList.h InterfaceArray.h \
 DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h Arrays.h MySwap.h \
 Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h MFDouble.h \
 SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h MFRotation.h \
 MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h \
 MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h SFImage.h \
 SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h SFString.h \
 SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h \
 CommandList.h Command.h DuneApp.h URL.h PreferencesApp.h \
 swt/include/swttypedef.h EcmaScriptApp.h StereoViewApp.h \
 swt/include/stereoview.h InputDeviceApp.h TransformMode.h OutputApp.h \
 RouteViewApp.h StartWithApp.h ExportNamesApp.h ModellingApp.h \
 RenderApp.h swt/include/swt.h swt/include/keycodes.h \
 swt/include/swlanguages.h swt/include/swDebugf.h \
 swt/include/swttypedef.h
ChaserNode.o: ChaserNode.cpp stdafx.h config.h mysnprintf.h ChaserNode.h \
 Node.h Array.h List.h Vec3f.h Vec3x.h MyString.h StringArray.h \
 FieldIndex.h Element.h x3dFlags.h swt/include/WriteFlags.h NodeFlags.h \
 MeshFlags.h ac3dMaterialCallback.h NodeArray.h Stack.h Matrix.h Vec3d.h \
 Quaternion.h ProtoMacros.h Proto.h Map.h NodeList.h InterfaceArray.h \
 DynamicFieldsNode.h SFMFTypes.h MFBool.h FieldValue.h Arrays.h MySwap.h \
 Types.h MFColor.h MFFloat.h SFFloat.h MFColorRGBA.h MFDouble.h \
 SFDouble.h MFInt32.h MFMatrix3f.h MFMatrix4f.h MFNode.h MFRotation.h \
 MFString.h MFTime.h SFTime.h MFVec2d.h Vec2d.h MFVec2f.h MFVec3d.h \
 MFVec3f.h MFVec4f.h Vec4f.h SFBool.h SFColor.h SFColorRGBA.h SFImage.h \
 SFInt32.h SFMatrix3f.h SFMatrix4f.h SFNode.h SFRotation.h SFString.h \
 SFVec2d.h SFVec2f.h Vec2f.h SFVec3d.h SfVec3x.h SFVec3f.h SFVec4f.h \
 CommandList.h Command.h DuneApp.h URL.h PreferencesApp.h \
 swt/include/swttypedef.h EcmaScriptApp.h StereoViewApp.h \
 swt/include/stereoview.h InputDeviceApp.h TransformMode.h OutputApp.h \
 RouteViewApp.h StartWithApp.h ExportNamesApp.h ModellingApp.h \
 RenderApp.h swt/include/swt.h swt/include/keycodes.h \
 swt/include/swlanguages.h swt/include/swDebugf.h \
 swt/include/swttypedef.h
