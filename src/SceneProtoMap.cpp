/*
 * SceneProtoMap.cpp
 *
 * Copyright (C) 1999 Stephen F. White, 2008 J. "MUFTI" Scheurich
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
#include "SceneProtoMap.h"

#include "NodeAnchor.h"
#include "NodeAppearance.h"
#include "NodeArc2D.h"
#include "NodeArcClose2D.h"
#include "NodeARSensor.h"
#include "NodeAudioClip.h"
#include "NodeBackground.h"
#include "NodeBallJoint.h"
#include "NodeBinaryGeometry.h"
#include "NodeBillboard.h"
#include "NodeBlendMode.h"
#include "NodeBlendedVolumeStyle.h"
#include "NodeBlock.h"
#include "NodeBooleanFilter.h"
#include "NodeBooleanToggle.h"
#include "NodeBooleanTrigger.h"
#include "NodeBooleanSequencer.h"
#include "NodeBoundaryEnhancementVolumeStyle.h"
#include "NodeBoundedPhysicsModel.h"
#include "NodeBox.h"
#include "NodeCADAssembly.h"
#include "NodeCADFace.h"
#include "NodeCADLayer.h"
#include "NodeCADPart.h"
#include "NodeCOVER.h"
#include "NodeCartoonVolumeStyle.h"
#include "NodeCattExportRec.h"
#include "NodeCattExportSrc.h"
#include "NodeCircle2D.h"
#include "NodeClipPlane.h"
#include "NodeCollidableOffset.h"
#include "NodeCollidableShape.h"
#include "NodeCollision.h"
#include "NodeCollisionCollection.h"
#include "NodeCollisionSensor.h"
#include "NodeCollisionSpace.h"
#include "NodeColor.h"
#include "NodeColorDamper.h"
#include "NodeColorChaser.h"
#include "NodeColorInterpolator.h"
#include "NodeColorMaskMode.h"
#include "NodeColorRGBA.h"
#include "NodeColorSetInterpolator.h"
#include "NodeCommonSurfaceShader.h"
#include "NodeComposedCubeMapTexture.h"
#include "NodeComposedShader.h"
#include "NodeComposedTexture3D.h"
#include "NodeComposedVolumeStyle.h"
#include "NodeCone.h"
#include "NodeConeEmitter.h"
#include "NodeContact.h"
#include "NodeContour2D.h"
#include "NodeContourPolyline2D.h"
#include "NodeCoordinate.h"
#include "NodeCoordinateChaser.h"
#include "NodeCoordinateDamper.h"
#include "NodeCoordinateDeformer.h"
#include "NodeCoordinateDouble.h"
#include "NodeCoordinateInterpolator2D.h"
#include "NodeCoordinateInterpolator.h"
#include "NodeCubeTexture.h"
#include "NodeCubicBezier2DOrientationInterpolator.h"
#include "NodeCubicBezierPositionInterpolator.h"
#include "NodeCurveAnimation.h"
#include "NodeCylinder.h"
#include "NodeCylinderSensor.h"
#include "NodeDepthMode.h"
#include "NodeDirectionalLight.h"
#include "NodeDISEntityManager.h"
#include "NodeDISEntityTypeMapping.h"
#include "NodeDish.h"
#include "NodeDisk2D.h"
#include "NodeDoubleAxisHingeJoint.h"
#include "NodeDynamicLOD.h"
#include "NodeEaseInEaseOut.h"
#include "NodeEdgeEnhancementVolumeStyle.h"
#include "NodeEffect.h"
#include "NodeEffectPart.h"
#include "NodeElevationGrid.h"
#include "NodeEnvironment.h"
#include "NodeEspduTransform.h"
#include "NodeExplosionEmitter.h"
#include "NodeExternalGeometry.h"
#include "NodeExtrusion.h"
#include "NodeField.h"
#include "NodeFillProperties.h"
#include "NodeFloatVertexAttribute.h"
#include "NodeFog.h"
#include "NodeFogCoordinate.h"
#include "NodeFontStyle.h"
#include "NodeForcePhysicsModel.h"
#include "NodeGeneratedCubeMapTexture.h"
#include "NodeGeneratedShadowMap.h"
#include "NodeGeoCoordinate.h"
#include "NodeGeoElevationGrid.h"
#include "NodeGeoMetadata.h"
#include "NodeGeoLocation.h"
#include "NodeGeoLOD.h"
#include "NodeGeoPositionInterpolator.h"
#include "NodeGeoProximitySensor.h"
#include "NodeGeoOrigin.h"
#include "NodeGeoTouchSensor.h"
#include "NodeGeoTransform.h"
#include "NodeGeoViewpoint.h" 
#include "NodeGroup.h"
#include "NodeHAnimDisplacer.h"
#include "NodeHAnimHumanoid.h"
#include "NodeHAnimMotion.h"
#include "NodeHAnimJoint.h"
#include "NodeHAnimSegment.h"
#include "NodeHAnimSite.h"
#include "NodeImageCubeMapTexture.h"
#include "NodeImageGeometry.h"
#include "NodeImageTexture.h"
#include "NodeImageTexture3D.h"
#include "NodeImageTextureAtlas.h"
#include "NodeIndexedFaceSet.h"
#include "NodeIndexedLineSet.h"
#include "NodeIndexedQuadSet.h"
#include "NodeIndexedTriangleFanSet.h"
#include "NodeIndexedTriangleSet.h"
#include "NodeIndexedTriangleStripSet.h"
#include "NodeInline.h"
#include "NodeInlineLoadControl.h"
#include "NodeIntegerSequencer.h"
#include "NodeIntegerTrigger.h"
#include "NodeIsoSurfaceVolumeData.h"
#include "NodeJoystickSensor.h"
#include "NodeKambiAppearance.h"
#include "NodeKambiHeadLight.h"
#include "NodeKambiInline.h"
#include "NodeKambiNavigationInfo.h"
#include "NodeKambiOctreeProperties.h"
#include "NodeKambiTriangulation.h"
#include "NodeKeySensor.h"
#include "NodeLabView.h"
#include "NodeLayer.h"
#include "NodeLayerSet.h"
#include "NodeLayout.h"
#include "NodeLayoutGroup.h"
#include "NodeLayoutLayer.h"
#include "NodeLdrawDatExport.h"
#include "NodeLinePickSensor.h"
#include "NodeLineProperties.h"
#include "NodeLineSet.h"
#include "NodeLoadSensor.h"
#include "NodeLocalFog.h"
#include "NodeLOD.h"
#include "NodeMaterial.h"
#include "NodeMatrix3VertexAttribute.h"
#include "NodeMatrix4VertexAttribute.h"
#include "NodeMatrixTransform.h"
#include "NodeMesh.h"
#include "NodeMetadataBoolean.h"
#include "NodeMetadataDouble.h"
#include "NodeMetadataFloat.h"
#include "NodeMetadataInteger.h"
#include "NodeMetadataSet.h"
#include "NodeMetadataString.h"
#include "NodeMotorJoint.h"
#include "NodeMovieTexture.h"
#include "NodeMPRVolumeStyle.h"
#include "NodeMultiGeneratedTextureCoordinate.h"
#include "NodeMultiPart.h"
#include "NodeMultiTexture.h"
#include "NodeMultiTextureCoordinate.h"
#include "NodeMultiTextureTransform.h"
#include "NodeNavigationInfo.h"
#include "NodeNormal.h"
#include "NodeNormalInterpolator.h"
#include "NodeNozzle.h"
#include "NodeNurbsCurve.h"
#include "NodeNurbsCurve2D.h"
#include "NodeNurbsGroup.h"
#include "NodeNurbsOrientationInterpolator.h"
#include "NodeNurbsPositionInterpolator.h"
#include "NodeNurbsSet.h"
#include "NodeNurbsSurface.h"
#include "NodeNurbsSurfaceInterpolator.h"
#include "NodeNurbsSweptSurface.h"
#include "NodeNurbsSwungSurface.h"
#include "NodeNurbsTextureSurface.h"
#include "NodeNurbsTextureCoordinate.h"
#include "NodeNurbsTrimmedSurface.h"
#include "NodeOpacityMapVolumeStyle.h"
#include "NodeOrientationChaser.h"
#include "NodeOrientationDamper.h"
#include "NodeOrientationInterpolator.h"
#include "NodeOrthoViewpoint.h"
#include "NodePackagedShader.h"
#include "NodeParam.h"
#include "NodeParticleSet.h"
#include "NodeParticleSystem.h"
#include "NodePickableGroup.h"
#include "NodePixelTexture.h"
#include "NodePixelTexture3D.h"
#include "NodePlane.h"
#include "NodePlaneSensor.h"
#include "NodePointEmitter.h"
#include "NodePointLight.h"
#include "NodePointProperties.h"
#include "NodePointPickSensor.h"
#include "NodePointSet.h"
#include "NodePolyline2D.h"
#include "NodePolylineEmitter.h"
#include "NodePolypoint2D.h"
#include "NodePopGeometry.h"
#include "NodePopGeometryLevel.h"
#include "NodePositionChaser.h"
#include "NodePositionChaser2D.h"
#include "NodePositionDamper.h"
#include "NodePositionDamper2D.h"
#include "NodePositionInterpolator.h"
#include "NodePositionInterpolator2D.h"
#include "NodePrimitivePickSensor.h"
#include "NodeProjectionVolumeStyle.h"
#include "NodeProgramShader.h"
#include "NodeProjectedTextureCoordinate.h"
#include "NodeProximitySensor.h"
#include "NodePyramid.h"
#include "NodeQuadSet.h"
#include "NodeRadarVolumeStyle.h"
#include "NodeReceiverPdu.h"
#include "NodeRectangle2D.h"
#include "NodeRectangularTorus.h"
#include "NodeRenderedTexture.h"
#include "NodeRefinementTexture.h"
#include "NodeRemoteSelectionGroup.h"
#include "NodeRigidBody.h"
#include "NodeRigidBodyCollection.h"
#include "NodeScalarChaser.h"
#include "NodeScalarDamper.h"
#include "NodeScalarInterpolator.h"
#include "NodeScreenEffect.h"
#include "NodeScreenFontStyle.h"
#include "NodeScreenGroup.h"
#include "NodeSegmentedVolumeData.h"
#include "NodeShadedVolumeStyle.h"
#include "NodeShaderPart.h"
#include "NodeShaderProgram.h"
#include "NodeShaderTexture.h"
#include "NodeShape.h"
#include "NodeSignalPdu.h"
#include "NodeSilhouetteEnhancementVolumeStyle.h"
#include "NodeSingleAxisHingeJoint.h"
#include "NodeSky.h"
#include "NodeSliderJoint.h"
#include "NodeSlopedCylinder.h"
#include "NodeSnout.h"
#include "NodeSolidOfRevolution.h"
#include "NodeSound.h"
#include "NodeSpaceSensor.h"
#include "NodeSphere.h"
#include "NodeSphereSegment.h"
#include "NodeSphereSensor.h"
#include "NodeSplinePositionInterpolator.h"
#include "NodeSplinePositionInterpolator2D.h"
#include "NodeSplineScalarInterpolator.h"
#include "NodeSpotLight.h"
#include "NodeSquadOrientationInterpolator.h"
#include "NodeStaticGroup.h"
#include "NodeSteeringWheel.h"
#include "NodeStippleVolumeStyle.h"
#include "NodeStringSensor.h"
#include "NodeSuperEllipsoid.h"
#include "NodeSuperExtrusion.h"
#include "NodeSuperRevolver.h"
#include "NodeSuperShape.h"
#include "NodeSurfaceEmitter.h"
#include "NodeSurfaceShaderTexture.h"
#include "NodeSwitch.h"
#include "NodeTeapot.h"
#include "NodeTexCoordChaser2D.h"
#include "NodeTexCoordDamper2D.h"
#include "NodeText.h"
#include "NodeText3D.h"
#include "NodeTextureBackground.h"
#include "NodeTextureCoordinate.h"
#include "NodeTextureCoordinate3D.h"
#include "NodeTextureCoordinate4D.h"
#include "NodeTextureCoordinateGenerator.h"
#include "NodeTextureProperties.h"
#include "NodeTextureTransform.h"
#include "NodeTextureTransform3D.h"
#include "NodeTextureTransformMatrix3D.h"
#include "NodeTimeTrigger.h"
#include "NodeTimeSensor.h"
#include "NodeToneMappedVolumeStyle.h"
#include "NodeTorus.h"
#include "NodeTouchSensor.h"
#include "NodeTransformSensor.h"
#include "NodeTransmitterPdu.h"
#include "NodeTriangleFanSet.h"
#include "NodeTriangleSet.h"
#include "NodeTriangleSet2D.h"
#include "NodeTriangleStripSet.h"
#include "NodeTrimmedSurface.h"
#include "NodeTransform.h"
#include "NodeTUIButton.h"
#include "NodeTUIComboBox.h"
#include "NodeTUIFloatSlider.h"
#include "NodeTUIFrame.h"
#include "NodeTUILabel.h"
#include "NodeTUIListBox.h"
#include "NodeTUIMap.h"
#include "NodeTUIProgressBar.h"
#include "NodeTUISlider.h"
#include "NodeTUISplitter.h"
#include "NodeTUITab.h"
#include "NodeTUITabFolder.h"
#include "NodeTUIToggleButton.h"
#include "NodeTwoSidedMaterial.h"
#include "NodeUniform.h"
#include "NodeUniversalJoint.h"
#include "NodeVehicle.h"
#include "NodeVectorInterpolator.h"
#include "NodeViewfrustum.h"
#include "NodeViewpoint.h"
#include "NodeViewpointGroup.h"
#include "NodeViewport.h"
#include "NodeVisibilitySensor.h"
#include "NodeVirtualAcoustics.h"
#include "NodeVirtualSoundSource.h"
#include "NodeVolumeData.h"
#include "NodeVolumeEmitter.h"
#include "NodeVolumePickSensor.h"
#include "NodeVrmlCut.h"
#include "NodeVrmlScene.h"
#include "NodeWave.h"
#include "NodeWindPhysicsModel.h"
#include "NodeWonderlandImportJava.h"
#include "NodeWorldInfo.h"
#include "NodeComment.h"
#include "NodeImport.h"
#include "NodeExport.h"

void
SceneProtoMap::addOrReplace(ProtoMap *protos, const char *string,
                            Proto *proto, bool add)
{
    if (add)
        protos->add(string, proto);
    else if (strcmp(string, (const char *)proto->getName(true)) == 0)
        protos->replace(string, proto);
}


void
SceneProtoMap::createProtoMap(ProtoMap *protos, Scene *scene, bool add)
{
    addOrReplace(protos, "Anchor", new ProtoAnchor(scene), add);
    addOrReplace(protos, "Appearance", new ProtoAppearance(scene), add);
    addOrReplace(protos, "Arc2D", new ProtoArc2D(scene), add);
    addOrReplace(protos, "ArcClose2D", new ProtoArcClose2D(scene), add);
    addOrReplace(protos, "ARSensor", new ProtoARSensor(scene), add);
    addOrReplace(protos, "AudioClip", new ProtoAudioClip(scene), add);
    addOrReplace(protos, "Background", new ProtoBackground(scene), add);
    addOrReplace(protos, "BallJoint", new ProtoBallJoint(scene), add);
    addOrReplace(protos, "BinaryGeometry", new ProtoBinaryGeometry(scene), add);
    addOrReplace(protos, "Billboard", new ProtoBillboard(scene), add);
    addOrReplace(protos, "BlendMode", new ProtoBlendMode(scene), add);
    addOrReplace(protos, "BlendedVolumeStyle", 
                 new ProtoBlendedVolumeStyle(scene), add);
    addOrReplace(protos, "Block", new ProtoBlock(scene), add);
    addOrReplace(protos, "BooleanFilter", new ProtoBooleanFilter(scene), add);
    addOrReplace(protos, "BooleanToggle", new ProtoBooleanToggle(scene), add);
    addOrReplace(protos, "BooleanTrigger", 
                 new ProtoBooleanTrigger(scene), add);
    addOrReplace(protos, "BooleanSequencer", 
                 new ProtoBooleanSequencer(scene), add);
    addOrReplace(protos, "BoundaryEnhancementVolumeStyle", 
                 new ProtoBoundaryEnhancementVolumeStyle(scene), add);
    addOrReplace(protos, "BoundedPhysicsModel", 
                 new ProtoBoundedPhysicsModel(scene), add);
    addOrReplace(protos, "Box", new ProtoBox(scene), add);
    addOrReplace(protos, "CADAssembly", new ProtoCADAssembly(scene), add);
    addOrReplace(protos, "CADFace", new ProtoCADFace(scene), add);
    addOrReplace(protos, "CADLayer", new ProtoCADLayer(scene), add);
    addOrReplace(protos, "CADPart", new ProtoCADPart(scene), add);
    addOrReplace(protos, "CartoonVolumeStyle", 
                 new ProtoCartoonVolumeStyle(scene), add);
    addOrReplace(protos, "CattExportRec", new ProtoCattExportRec(scene), add);
    addOrReplace(protos, "CattExportSrc", new ProtoCattExportSrc(scene), add);
    addOrReplace(protos, "Circle2D", new ProtoCircle2D(scene), add);
    addOrReplace(protos, "ClipPlane", new ProtoClipPlane(scene), add);
    addOrReplace(protos, "CollidableOffset", 
                 new ProtoCollidableOffset(scene), add);
    addOrReplace(protos, "CollidableShape", 
                 new ProtoCollidableShape(scene), add);
    addOrReplace(protos, "Collision", new ProtoCollision(scene), add);
    addOrReplace(protos, "CollisionCollection", 
                 new ProtoCollisionCollection(scene), add);
    addOrReplace(protos, "CollisionSensor", 
                 new ProtoCollisionSensor(scene), add);
    addOrReplace(protos, "CollisionSpace", 
                 new ProtoCollisionSpace(scene), add);
    addOrReplace(protos, "Color", new ProtoColor(scene), add);
    addOrReplace(protos, "ColorChaser", new ProtoColorChaser(scene), add);
    addOrReplace(protos, "ColorDamper", new ProtoColorDamper(scene), add);
    addOrReplace(protos, "ColorInterpolator", 
                 new ProtoColorInterpolator(scene), add);
    addOrReplace(protos, "ColorMaskMode", new ProtoColorMaskMode(scene), add);
    addOrReplace(protos, "ColorRGBA", new ProtoColorRGBA(scene), add);
    addOrReplace(protos, "ColorSetInterpolator", 
                 new ProtoColorSetInterpolator(scene), add);
    addOrReplace(protos, "ComposedCubeMapTexture", 
                 new ProtoComposedCubeMapTexture(scene), add);
    addOrReplace(protos, "CommonSurfaceShader", 
                 new ProtoCommonSurfaceShader(scene), add);
    addOrReplace(protos, "ComposedShader", 
                 new ProtoComposedShader(scene), add);
    addOrReplace(protos, "ComposedTexture3D", 
                 new ProtoComposedTexture3D(scene), add);
    addOrReplace(protos, "ComposedVolumeStyle", 
                 new ProtoComposedVolumeStyle(scene), add);
    addOrReplace(protos, "Cone", new ProtoCone(scene), add);
    addOrReplace(protos, "ConeEmitter", new ProtoConeEmitter(scene), add);
    addOrReplace(protos, "Contact", new ProtoContact(scene), add);
    addOrReplace(protos, "Contour2D", new ProtoContour2D(scene), add);
    addOrReplace(protos, "ContourPolyline2D", 
                 new ProtoContourPolyline2D(scene), add);
    addOrReplace(protos, "Coordinate", new ProtoCoordinate(scene), add);
    addOrReplace(protos, "CoordinateChaser", 
                 new ProtoCoordinateChaser(scene), add);
    addOrReplace(protos, "CoordinateDamper", 
                 new ProtoCoordinateDamper(scene), add);
    addOrReplace(protos, "CoordinateDeformer", 
                 new ProtoCoordinateDeformer(scene), add);
    addOrReplace(protos, "CoordinateDouble", 
                 new ProtoCoordinateDouble(scene), add);
    addOrReplace(protos, "CoordinateInterpolator", 
                 new ProtoCoordinateInterpolator(scene), add);
    addOrReplace(protos, "CoordinateInterpolator2D", 
                 new ProtoCoordinateInterpolator2D(scene), add);
    addOrReplace(protos, "COVER", new ProtoCOVER(scene), add);
    addOrReplace(protos, "CubeTexture", new ProtoCubeTexture(scene), add);
    addOrReplace(protos, "CubicBezier2DOrientationInterpolator", 
                 new ProtoCubicBezier2DOrientationInterpolator(scene), add);
    addOrReplace(protos, "CubicBezierPositionInterpolator", 
                 new ProtoCubicBezierPositionInterpolator(scene), add);
    addOrReplace(protos, "CurveAnimation", 
                 new ProtoCurveAnimation(scene), add);
    addOrReplace(protos, "Cylinder", new ProtoCylinder(scene), add);
    addOrReplace(protos, "CylinderSensor", 
                 new ProtoCylinderSensor(scene), add);
    addOrReplace(protos, "DepthMode", new ProtoDepthMode(scene), add);
    addOrReplace(protos, "DirectionalLight", 
                 new ProtoDirectionalLight(scene), add);
    addOrReplace(protos, "DISEntityManager", 
                 new ProtoDISEntityManager(scene), add);
    addOrReplace(protos, "DISEntityTypeMapping", 
                 new ProtoDISEntityTypeMapping(scene), add);
    addOrReplace(protos, "Dish", new ProtoDish(scene), add);
    addOrReplace(protos, "Disk2D", new ProtoDisk2D(scene), add);
    addOrReplace(protos, "DoubleAxisHingeJoint", 
                 new ProtoDoubleAxisHingeJoint(scene), add);
    addOrReplace(protos, "DynamicLOD", new ProtoDynamicLOD(scene), add);
    addOrReplace(protos, "EaseInEaseOut", new ProtoEaseInEaseOut(scene), add);
    addOrReplace(protos, "EdgeEnhancementVolumeStyle",
                 new ProtoEdgeEnhancementVolumeStyle(scene), add);
    addOrReplace(protos, "Effect", new ProtoEffect(scene), add);
    addOrReplace(protos, "EffectPart", new ProtoEffectPart(scene), add);
    addOrReplace(protos, "Environment", new ProtoEnvironment(scene), add);
    addOrReplace(protos, "ExternalGeometry", 
                 new ProtoExternalGeometry(scene), add);
    addOrReplace(protos, "ElevationGrid", new ProtoElevationGrid(scene), add);
    addOrReplace(protos, "EspduTransform", 
                 new ProtoEspduTransform(scene), add);
    addOrReplace(protos, "ExplosionEmitter", 
                 new ProtoExplosionEmitter(scene), add);
    addOrReplace(protos, "Extrusion", new ProtoExtrusion(scene), add);
    addOrReplace(protos, "Field", new ProtoField(scene), add);
    addOrReplace(protos, "FillProperties", 
                 new ProtoFillProperties(scene), add);
    addOrReplace(protos, "FloatVertexAttribute",
                 new ProtoFloatVertexAttribute(scene), add);
    addOrReplace(protos, "Fog", new ProtoFog(scene), add);
    addOrReplace(protos, "FogCoordinate", new ProtoFogCoordinate(scene), add);
    addOrReplace(protos, "FontStyle", new ProtoFontStyle(scene), add);
    addOrReplace(protos, "ForcePhysicsModel", 
                 new ProtoForcePhysicsModel(scene), add);
    addOrReplace(protos, "GeneratedCubeMapTexture", 
                 new ProtoGeneratedCubeMapTexture(scene), add);
    addOrReplace(protos, "GeneratedShadowMap", 
                 new ProtoGeneratedShadowMap(scene), add);
    addOrReplace(protos, "GeoCoordinate", new ProtoGeoCoordinate(scene), add);
    addOrReplace(protos, "GeoElevationGrid", 
                 new ProtoGeoElevationGrid(scene), add);
    addOrReplace(protos, "GeoLocation", new ProtoGeoLocation(scene), add);
    addOrReplace(protos, "GeoLOD", new ProtoGeoLOD(scene), add);
    addOrReplace(protos, "GeoMetadata", new ProtoGeoMetadata(scene), add);
    addOrReplace(protos, "GeoOrigin", new ProtoGeoOrigin(scene), add);
    addOrReplace(protos, "GeoPositionInterpolator", 
                 new ProtoGeoPositionInterpolator(scene), add);
    addOrReplace(protos, "GeoProximitySensor", 
                 new ProtoGeoProximitySensor(scene), add);
    addOrReplace(protos, "GeoTouchSensor", 
                 new ProtoGeoTouchSensor(scene), add);
    addOrReplace(protos, "GeoTransform", new ProtoGeoTransform(scene), add);
    addOrReplace(protos, "GeoViewpoint", new ProtoGeoViewpoint(scene), add);
    addOrReplace(protos, "Group", new ProtoGroup(scene), add);
    addOrReplace(protos, "HAnimDisplacer", 
                 new ProtoHAnimDisplacer(scene), add);
    addOrReplace(protos, "HAnimHumanoid", new ProtoHAnimHumanoid(scene), add);
    addOrReplace(protos, "HAnimMotion", new ProtoHAnimMotion(scene), add);
    addOrReplace(protos, "HAnimJoint", new ProtoHAnimJoint(scene), add);
    addOrReplace(protos, "HAnimSegment", new ProtoHAnimSegment(scene), add);
    addOrReplace(protos, "HAnimSite", new ProtoHAnimSite(scene), add);
    addOrReplace(protos, "ImageCubeMapTexture", 
                 new ProtoImageCubeMapTexture(scene), add);
    addOrReplace(protos, "ImageGeometry", new ProtoImageGeometry(scene), add);
    addOrReplace(protos, "ImageTexture", new ProtoImageTexture(scene), add);
    addOrReplace(protos, "ImageTexture3D", 
                 new ProtoImageTexture3D(scene), add);
    addOrReplace(protos, "ImageTextureAtlas", 
                 new ProtoImageTextureAtlas(scene), add);
    addOrReplace(protos, "IndexedFaceSet", 
                 new ProtoIndexedFaceSet(scene), add);
    addOrReplace(protos, "IndexedLineSet", 
                 new ProtoIndexedLineSet(scene), add);
    addOrReplace(protos, "IndexedQuadSet", 
                 new ProtoIndexedQuadSet(scene), add);
    addOrReplace(protos, "IndexedTriangleFanSet", 
                 new ProtoIndexedTriangleFanSet(scene), add);
    addOrReplace(protos, "IndexedTriangleSet", 
                 new ProtoIndexedTriangleSet(scene), add);
    addOrReplace(protos, "IndexedTriangleStripSet", 
                 new ProtoIndexedTriangleStripSet(scene), add);
    addOrReplace(protos, "Inline", new ProtoInline(scene), add);
    addOrReplace(protos, "InlineLoadControl",
                 new ProtoInlineLoadControl(scene), add);
    addOrReplace(protos, "IntegerSequencer", 
                 new ProtoIntegerSequencer(scene), add);
    addOrReplace(protos, "IntegerTrigger", 
                 new ProtoIntegerTrigger(scene), add);
    addOrReplace(protos, "IsoSurfaceVolumeData", 
                 new ProtoIsoSurfaceVolumeData(scene), add);
    addOrReplace(protos, "JoystickSensor", 
                 new ProtoJoystickSensor(scene), add);
    addOrReplace(protos, "KambiAppearance", 
                 new ProtoKambiAppearance(scene), add);
    addOrReplace(protos, "KambiHeadLight", 
                 new ProtoKambiHeadLight(scene), add);
    addOrReplace(protos, "KambiInline", new ProtoKambiInline(scene), add);
    addOrReplace(protos, "KambiNavigationInfo", 
                 new ProtoKambiNavigationInfo(scene), add);
    addOrReplace(protos, "KambiOctreeProperties", 
                 new ProtoKambiOctreeProperties(scene), add);
    addOrReplace(protos, "KambiTriangulation", 
                 new ProtoKambiTriangulation(scene), add);
    addOrReplace(protos, "KeySensor", new ProtoKeySensor(scene), add);
    addOrReplace(protos, "LabView", new ProtoLabView(scene), add);
    addOrReplace(protos, "Layer", new ProtoLayer(scene), add);
    addOrReplace(protos, "LayerSet", new ProtoLayerSet(scene), add);
    addOrReplace(protos, "Layout", new ProtoLayout(scene), add);
    addOrReplace(protos, "LayoutGroup", new ProtoLayoutGroup(scene), add);
    addOrReplace(protos, "LayoutLayer", new ProtoLayoutLayer(scene), add);
    addOrReplace(protos, "LdrawDatExport", 
                 new ProtoLdrawDatExport(scene), add);
    addOrReplace(protos, "LinePickSensor", 
                 new ProtoLinePickSensor(scene), add);
    addOrReplace(protos, "LineProperties", 
                 new ProtoLineProperties(scene), add);
    addOrReplace(protos, "LineSet", new ProtoLineSet(scene), add);
    addOrReplace(protos, "LoadSensor", new ProtoLoadSensor(scene), add);
    addOrReplace(protos, "LocalFog", new ProtoLocalFog(scene), add);
    addOrReplace(protos, "LOD", new ProtoLOD(scene), add);
    addOrReplace(protos, "Material", new ProtoMaterial(scene), add);
    addOrReplace(protos, "Matrix3VertexAttribute",
                 new ProtoMatrix3VertexAttribute(scene), add);
    addOrReplace(protos, "Matrix4VertexAttribute",
                 new ProtoMatrix4VertexAttribute(scene), add);
    addOrReplace(protos, "MatrixTransform", 
                 new ProtoMatrixTransform(scene), add);
    addOrReplace(protos, "Mesh", new ProtoMesh(scene), add);
    addOrReplace(protos, "MetadataBoolean", 
                 new ProtoMetadataBoolean(scene), add);
    addOrReplace(protos, "MetadataDouble", 
                 new ProtoMetadataDouble(scene), add);
    addOrReplace(protos, "MetadataFloat", new ProtoMetadataFloat(scene), add);
    addOrReplace(protos, "MetadataInteger", 
                 new ProtoMetadataInteger(scene), add);
    addOrReplace(protos, "MetadataSet", new ProtoMetadataSet(scene), add);
    addOrReplace(protos, "MetadataString", 
                 new ProtoMetadataString(scene), add);
    addOrReplace(protos, "MotorJoint", new ProtoMotorJoint(scene), add);
    addOrReplace(protos, "MovieTexture", new ProtoMovieTexture(scene), add);
    addOrReplace(protos, "MPRVolumeStyle", new ProtoMPRVolumeStyle(scene), add);
    addOrReplace(protos, "MultiGeneratedTextureCoordinate", 
                 new ProtoMultiGeneratedTextureCoordinate(scene), add);
    addOrReplace(protos, "MultiPart", new ProtoMultiPart(scene), add);
    addOrReplace(protos, "MultiTexture", new ProtoMultiTexture(scene), add);
    addOrReplace(protos, "MultiTextureCoordinate", 
                 new ProtoMultiTextureCoordinate(scene), add);
    addOrReplace(protos, "MultiTextureTransform", 
                 new ProtoMultiTextureTransform(scene), add);
    addOrReplace(protos, "NavigationInfo", 
                 new ProtoNavigationInfo(scene), add);
    addOrReplace(protos, "Normal", new ProtoNormal(scene), add);
    addOrReplace(protos, "NormalInterpolator", 
                 new ProtoNormalInterpolator(scene), add);
    addOrReplace(protos, "Nozzle", new ProtoNozzle(scene), add);
    addOrReplace(protos, "NurbsCurve", new ProtoNurbsCurve(scene), add);
    addOrReplace(protos, "NurbsCurve2D", new ProtoNurbsCurve2D(scene), add);
    addOrReplace(protos, "NurbsGroup", new ProtoNurbsGroup(scene), add);
    addOrReplace(protos, "NurbsOrientationInterpolator", 
                 new ProtoNurbsOrientationInterpolator(scene), add);
    addOrReplace(protos, "NurbsPositionInterpolator", 
                 new ProtoNurbsPositionInterpolator(scene), add);
    addOrReplace(protos, "NurbsSet", new ProtoNurbsSet(scene), add);
    addOrReplace(protos, "NurbsSurface", new ProtoNurbsSurface(scene), add);
    addOrReplace(protos, "NurbsSurfaceInterpolator", 
                 new ProtoNurbsSurfaceInterpolator(scene), add);
    addOrReplace(protos, "NurbsSweptSurface", 
                 new ProtoNurbsSweptSurface(scene), add);
    addOrReplace(protos, "NurbsSwungSurface", 
                 new ProtoNurbsSwungSurface(scene), add);
    addOrReplace(protos, "NurbsTextureSurface", 
                 new ProtoNurbsTextureSurface(scene), add);
    addOrReplace(protos, "NurbsTextureCoordinate", 
                 new ProtoNurbsTextureCoordinate(scene), add);
    addOrReplace(protos, "NurbsTrimmedSurface", 
                 new ProtoNurbsTrimmedSurface(scene), add);
    addOrReplace(protos, "OpacityMapVolumeStyle",
                 new ProtoOpacityMapVolumeStyle(scene), add);
    addOrReplace(protos, "OrientationChaser", 
                 new ProtoOrientationChaser(scene), add);
    addOrReplace(protos, "OrientationDamper", 
                 new ProtoOrientationDamper(scene), add);
    addOrReplace(protos, "OrientationInterpolator", 
                 new ProtoOrientationInterpolator(scene), add);
    addOrReplace(protos, "OrthoViewpoint", 
                 new ProtoOrthoViewpoint(scene), add);
    addOrReplace(protos, "PackagedShader", 
                 new ProtoPackagedShader(scene), add);
    addOrReplace(protos, "Param", new ProtoParam(scene), add);
    addOrReplace(protos, "ParticleSet", new ProtoParticleSet(scene), add);
    addOrReplace(protos, "ParticleSystem", 
                 new ProtoParticleSystem(scene), add);
    addOrReplace(protos, "PickableGroup", new ProtoPickableGroup(scene), add);
    addOrReplace(protos, "PixelTexture", new ProtoPixelTexture(scene), add);
    addOrReplace(protos, "PixelTexture3D", 
                 new ProtoPixelTexture3D(scene), add);
    addOrReplace(protos, "Plane", new ProtoPlane(scene), add);
    addOrReplace(protos, "PlaneSensor", new ProtoPlaneSensor(scene), add);
    addOrReplace(protos, "PointEmitter", new ProtoPointEmitter(scene), add);
    addOrReplace(protos, "PointLight", new ProtoPointLight(scene), add);
    addOrReplace(protos, "PointProperties", 
                 new ProtoPointProperties(scene), add);
    addOrReplace(protos, "PointPickSensor", 
                 new ProtoPointPickSensor(scene), add);
    addOrReplace(protos, "PointSet", new ProtoPointSet(scene), add);
    addOrReplace(protos, "Polyline2D", new ProtoPolyline2D(scene), add);
    addOrReplace(protos, "PolylineEmitter", 
                 new ProtoPolylineEmitter(scene), add);
    addOrReplace(protos, "Polypoint2D", new ProtoPolypoint2D(scene), add);
    addOrReplace(protos, "PopGeometry", new ProtoPopGeometry(scene), add);
    addOrReplace(protos, "PopGeometryLevel", 
                 new ProtoPopGeometryLevel(scene), add);
    addOrReplace(protos, "PositionChaser", 
                 new ProtoPositionChaser(scene), add);
    addOrReplace(protos, "PositionChaser2D", 
                 new ProtoPositionChaser2D(scene), add);
    addOrReplace(protos, "PositionDamper", 
                 new ProtoPositionDamper(scene), add);
    addOrReplace(protos, "PositionDamper2D", 
                 new ProtoPositionDamper2D(scene), add);
    addOrReplace(protos, "PositionInterpolator", 
                 new ProtoPositionInterpolator(scene), add);
    addOrReplace(protos, "PositionInterpolator2D", 
                 new ProtoPositionInterpolator2D(scene), add);
    addOrReplace(protos, "PrimitivePickSensor", 
                 new ProtoPrimitivePickSensor(scene), add);
    addOrReplace(protos, "ProgramShader", new ProtoProgramShader(scene), add);
    addOrReplace(protos, "ProjectedTextureCoordinate", 
                 new ProtoProjectedTextureCoordinate(scene), add);
    addOrReplace(protos, "ProjectionVolumeStyle", 
                 new ProtoProjectionVolumeStyle(scene), add);
    addOrReplace(protos, "ProximitySensor", 
                 new ProtoProximitySensor(scene), add);
    addOrReplace(protos, "Pyramid", new ProtoPyramid(scene), add);
    addOrReplace(protos, "QuadSet", new ProtoQuadSet(scene), add);
    addOrReplace(protos, "RadarVolumeStyle", 
                          new ProtoRadarVolumeStyle(scene), add);
    addOrReplace(protos, "ReceiverPdu", new ProtoReceiverPdu(scene), add);
    addOrReplace(protos, "Rectangle2D", new ProtoRectangle2D(scene), add);
    addOrReplace(protos, "RectangularTorus", 
                 new ProtoRectangularTorus(scene), add);
    addOrReplace(protos, "RenderedTexture", 
                 new ProtoRenderedTexture(scene), add);
    addOrReplace(protos, "RefinementTexture", 
                 new ProtoRefinementTexture(scene), add);
    addOrReplace(protos, "RemoteSelectionGroup", 
                 new ProtoRemoteSelectionGroup(scene), add);
    addOrReplace(protos, "RigidBody", new ProtoRigidBody(scene), add);
    addOrReplace(protos, "RigidBodyCollection", 
                 new ProtoRigidBodyCollection(scene), add);
    addOrReplace(protos, "ScalarChaser", new ProtoScalarChaser(scene), add);
    addOrReplace(protos, "ScalarDamper", new ProtoScalarDamper(scene), add);
    addOrReplace(protos, "ScalarInterpolator", 
                 new ProtoScalarInterpolator(scene), add);
    addOrReplace(protos, "ScreenEffect", new ProtoScreenEffect(scene), add);
    addOrReplace(protos, "ScreenFontStyle", 
                 new ProtoScreenFontStyle(scene), add);
    addOrReplace(protos, "ScreenGroup", new ProtoScreenGroup(scene), add);
    addOrReplace(protos, "SegmentedVolumeData", 
                 new ProtoSegmentedVolumeData(scene), add);
    addOrReplace(protos, "ShadedVolumeStyle", 
                 new ProtoShadedVolumeStyle(scene), add);
    addOrReplace(protos, "ShaderPart", new ProtoShaderPart(scene), add);
    addOrReplace(protos, "ShaderProgram", new ProtoShaderProgram(scene), add);
    addOrReplace(protos, "ShaderTexture", new ProtoShaderTexture(scene), add);
    addOrReplace(protos, "Shape", new ProtoShape(scene), add);
    addOrReplace(protos, "SilhouetteEnhancementVolumeStyle", 
                 new ProtoSilhouetteEnhancementVolumeStyle(scene), add);
    addOrReplace(protos, "SignalPdu", new ProtoSignalPdu(scene), add);
    addOrReplace(protos, "SingleAxisHingeJoint", 
                 new ProtoSingleAxisHingeJoint(scene), add);
    addOrReplace(protos, "Sky", new ProtoSky(scene), add);
    addOrReplace(protos, "SliderJoint", new ProtoSliderJoint(scene), add);
    addOrReplace(protos, "SlopedCylinder", new ProtoSlopedCylinder(scene), add);
    addOrReplace(protos, "Snout", new ProtoSnout(scene), add);
    addOrReplace(protos, "SolidOfRevolution", new ProtoSolidOfRevolution(scene),
                 add);
    addOrReplace(protos, "Sound", new ProtoSound(scene), add);
    addOrReplace(protos, "SpaceSensor", new ProtoSpaceSensor(scene), add);
    addOrReplace(protos, "Sphere", new ProtoSphere(scene), add);
    addOrReplace(protos, "SphereSegment", new ProtoSphereSegment(scene), add);
    addOrReplace(protos, "SphereSensor", new ProtoSphereSensor(scene), add);
    addOrReplace(protos, "SplinePositionInterpolator", 
                 new ProtoSplinePositionInterpolator(scene), add);
    addOrReplace(protos, "SplinePositionInterpolator2D", 
                 new ProtoSplinePositionInterpolator2D(scene), add);
    addOrReplace(protos, "SplineScalarInterpolator", 
                 new ProtoSplineScalarInterpolator(scene), add);
    addOrReplace(protos, "SpotLight", new ProtoSpotLight(scene), add);
    addOrReplace(protos, "SquadOrientationInterpolator", 
                 new ProtoSquadOrientationInterpolator(scene), add);
    addOrReplace(protos, "StaticGroup", new ProtoStaticGroup(scene), add);
    addOrReplace(protos, "SteeringWheel", new ProtoSteeringWheel(scene), add);
    addOrReplace(protos, "StippleVolumeStyle", 
                 new ProtoStippleVolumeStyle(scene), add);
    addOrReplace(protos, "StringSensor", new ProtoStringSensor(scene), add);
    addOrReplace(protos, "SuperEllipsoid", 
                 new ProtoSuperEllipsoid(scene), add);
    addOrReplace(protos, "SuperExtrusion", 
                 new ProtoSuperExtrusion(scene), add);
    addOrReplace(protos, "SuperShape", new ProtoSuperShape(scene), add);
    addOrReplace(protos, "SuperRevolver", new ProtoSuperRevolver(scene), add);
    addOrReplace(protos, "SurfaceEmitter", 
                 new ProtoSurfaceEmitter(scene), add);
    addOrReplace(protos, "SurfaceShaderTexture", 
                 new ProtoSurfaceShaderTexture(scene), add);
    addOrReplace(protos, "Switch", new ProtoSwitch(scene), add);
    addOrReplace(protos, "Teapot", new ProtoTeapot(scene), add);
    addOrReplace(protos, "TexCoordChaser2D", 
                 new ProtoTexCoordChaser2D(scene), add);
    addOrReplace(protos, "TexCoordDamper2D", 
                 new ProtoTexCoordDamper2D(scene), add);
    addOrReplace(protos, "Text", new ProtoText(scene), add);
    addOrReplace(protos, "Text3D", new ProtoText3D(scene), add);
    addOrReplace(protos, "TextureBackground", 
                 new ProtoTextureBackground(scene), add);
    addOrReplace(protos, "TextureCoordinate", 
                 new ProtoTextureCoordinate(scene), add);
    addOrReplace(protos, "TextureCoordinate3D", 
                 new ProtoTextureCoordinate3D(scene), add);
    addOrReplace(protos, "TextureCoordinate4D", 
                 new ProtoTextureCoordinate4D(scene), add);
    addOrReplace(protos, "TextureCoordinateGenerator", 
                 new ProtoTextureCoordinateGenerator(scene), add);
    addOrReplace(protos, "TextureProperties", 
                 new ProtoTextureProperties(scene), add);
    addOrReplace(protos, "TextureTransform", 
                 new ProtoTextureTransform(scene), add);
    addOrReplace(protos, "TextureTransform3D", 
                 new ProtoTextureTransform3D(scene), add);
    addOrReplace(protos, "TextureTransformMatrix3D", 
                 new ProtoTextureTransformMatrix3D(scene), add);
    addOrReplace(protos, "TimeSensor", new ProtoTimeSensor(scene), add);
    addOrReplace(protos, "TimeTrigger", new ProtoTimeTrigger(scene), add);
    addOrReplace(protos, "ToneMappedVolumeStyle", 
                 new ProtoToneMappedVolumeStyle(scene), add);
    addOrReplace(protos, "TouchSensor", new ProtoTouchSensor(scene), add);
    addOrReplace(protos, "Torus", new ProtoTorus(scene), add);
    addOrReplace(protos, "Transform", new ProtoTransform(scene), add);
    addOrReplace(protos, "TransformSensor", 
                 new ProtoTransformSensor(scene), add);
    addOrReplace(protos, "TransmitterPdu", 
                 new ProtoTransmitterPdu(scene), add);
    addOrReplace(protos, "TriangleFanSet", 
                 new ProtoTriangleFanSet(scene), add);
    addOrReplace(protos, "TriangleSet", new ProtoTriangleSet(scene), add);
    addOrReplace(protos, "TriangleSet2D", new ProtoTriangleSet2D(scene), add);
    addOrReplace(protos, "TriangleStripSet", 
                 new ProtoTriangleStripSet(scene), add);
    addOrReplace(protos, "TrimmedSurface", new ProtoTrimmedSurface(scene), add);
    addOrReplace(protos, "TUIButton", new ProtoTUIButton(scene), add);
    addOrReplace(protos, "TUIComboBox", new ProtoTUIComboBox(scene), add);
    addOrReplace(protos, "TUIFloatSlider", 
                 new ProtoTUIFloatSlider(scene), add);
    addOrReplace(protos, "TUIFrame", new ProtoTUIFrame(scene), add);
    addOrReplace(protos, "TUILabel", new ProtoTUILabel(scene), add);
    addOrReplace(protos, "TUIListBox", new ProtoTUIListBox(scene), add);
    addOrReplace(protos, "TUIMap", new ProtoTUIMap(scene), add);
    addOrReplace(protos, "TUIProgressBar", 
                 new ProtoTUIProgressBar(scene), add);
    addOrReplace(protos, "TUISlider", new ProtoTUISlider(scene), add);
    addOrReplace(protos, "TUISplitter", new ProtoTUISplitter(scene), add);
    addOrReplace(protos, "TUITab", new ProtoTUITab(scene), add);
    addOrReplace(protos, "TUITabFolder", new ProtoTUITabFolder(scene), add);
    addOrReplace(protos, "TUIToggleButton", 
                 new ProtoTUIToggleButton(scene), add);
    addOrReplace(protos, "TwoSidedMaterial", 
                 new ProtoTwoSidedMaterial(scene), add);
    addOrReplace(protos, "Uniform", new ProtoUniform(scene), add);
    addOrReplace(protos, "UniversalJoint", new ProtoUniversalJoint(scene), add);
    addOrReplace(protos, "Vehicle", new ProtoVehicle(scene), add);
    addOrReplace(protos, "VectorInterpolator", 
                 new ProtoVectorInterpolator(scene), add);
    addOrReplace(protos, "Viewfrustum", new ProtoViewfrustum(scene), add);
    addOrReplace(protos, "Viewpoint", new ProtoViewpoint(scene), add);
    addOrReplace(protos, "ViewpointGroup", 
                 new ProtoViewpointGroup(scene), add);
    addOrReplace(protos, "Viewport", new ProtoViewport(scene), add);
    addOrReplace(protos, "VirtualAcoustics", new ProtoVirtualAcoustics(scene), add);
    addOrReplace(protos, "VirtualSoundSource", 
                 new ProtoVirtualSoundSource(scene), add);
    addOrReplace(protos, "VisibilitySensor", 
                 new ProtoVisibilitySensor(scene), add);
    addOrReplace(protos, "VolumeData", new ProtoVolumeData(scene), add);
    addOrReplace(protos, "VolumeEmitter", new ProtoVolumeEmitter(scene), add);
    addOrReplace(protos, "VolumePickSensor", 
                 new ProtoVolumePickSensor(scene), add);
    addOrReplace(protos, "VrmlCut", new ProtoVrmlCut(scene), add);
    addOrReplace(protos, "VrmlScene", new ProtoVrmlScene(scene), add);
    addOrReplace(protos, "Wave", new ProtoWave(scene), add);
    addOrReplace(protos, "WindPhysicsModel", 
                 new ProtoWindPhysicsModel(scene), add);
    addOrReplace(protos, "WonderlandImportJava", 
                 new ProtoWonderlandImportJava(scene), add);
    addOrReplace(protos, "WorldInfo", new ProtoWorldInfo(scene), add);
    addOrReplace(protos, "#", new ProtoComment(scene), add);
    addOrReplace(protos, "IMPORT", new ProtoImport(scene), add);
    addOrReplace(protos, "EXPORT", new ProtoExport(scene), add);
}

void
SceneProtoMap::updateProtoMap(ProtoMap* protos, Scene *scene)
{
    bool needUpdate = false;
    ProtoMap::Chain::Iterator *j;
    for (int i = 0; i < protos->width(); i++) {
        for (j = protos->chain(i).first(); j != NULL; j = j->next()) {
            if (j->item()->getData() && j->item()->getData()->needUpdate()) {
                needUpdate = true;
                break;
            }
        }
        if (needUpdate)
            break;
    }
    if (needUpdate)
        createProtoMap(protos, scene, false);

    MyString contourPolyline2D = "";
    contourPolyline2D += "ContourPolyline2D";
    (*protos)[contourPolyline2D] = new ProtoContourPolyline2D(scene);    
    const NodeList *nodes = scene->getNodes();
    for (long i = 0; i < nodes->size(); i++) {
         Node *node = nodes->get(i);
         if (node->getType() == X3D_CONTOUR_POLYLINE_2D) 
             node->setProto((*protos)[contourPolyline2D]);
    }
}


