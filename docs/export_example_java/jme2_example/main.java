/* Copyright (c) J. "MUFTI" Scheurich, 2010. */

/*
   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions are met:
   1. Redistributions of source code must retain the above copyright notice,
      this list of conditions and the following disclaimer.
   2. Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
   3. The name of the author may not be used to endorse or promote products
      derived from this software without specific prior written permission.

   THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
   IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
   MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
   EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
   SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
   TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
   PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
   LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, ORTORT (INCLUDING
   NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF TH
   IS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

import com.jme.animation.SpatialTransformer;

import com.jme.app.SimpleGame;
import com.jme.scene.Node;
import com.jme.scene.Geometry;
import com.jme.scene.TriMesh;
import com.jme.scene.TriMesh.Mode;
import com.jme.scene.Line;
import com.jme.scene.Point;
import com.jme.scene.shape.Quad;
import com.jme.scene.Text;
import com.jme.scene.TexCoords;
import com.jme.scene.state.RenderState;
import com.jme.scene.state.LightState;
import com.jme.scene.state.CullState;
import com.jme.scene.state.MaterialState;
import com.jme.scene.state.MaterialState.ColorMaterial;
import com.jme.scene.state.TextureState;
import com.jme.scene.state.BlendState;
import com.jme.scene.Spatial;
import com.jme.scene.SharedNode;
import com.jme.scene.BillboardNode;
import com.jme.scene.lod.DiscreteLodNode;
import com.jme.scene.DistanceSwitchModel;

import com.jme.system.DisplaySystem;

import com.jme.renderer.Renderer;
import com.jme.renderer.ColorRGBA;

import com.jme.light.DirectionalLight;

import com.jme.image.Image;
import com.jme.image.Texture;
import com.jme.image.Texture2D;
import com.jme.image.Texture.MinificationFilter;
import com.jme.image.Texture.MagnificationFilter;

import com.jme.util.TextureManager;

import java.net.MalformedURLException;
import java.net.URL;

import java.awt.Color;
import java.awt.Font;
import java.awt.Graphics2D;
import java.awt.geom.Rectangle2D;
import java.awt.geom.AffineTransform;
import java.awt.image.BufferedImage;
import java.awt.RenderingHints;
import java.awt.Toolkit;

import com.jmex.font3d.Text3D;
import com.jmex.font3d.Font3D;

import com.jme.util.geom.NormalGenerator;
import com.jme.util.geom.BufferUtils;

import com.jme.math.Vector3f;
import com.jme.math.Matrix4f;
import com.jme.math.TransformMatrix;
import com.jme.math.Quaternion;

import java.nio.FloatBuffer;
import java.nio.IntBuffer;

// draws a X3DV file with shapes (converted to) IndexedTriangleSet,TriangleSet
// and triangulated IndexedFaceSet nodes

class X3d_Util {
    public static Texture loadTexture(String url) {
        /* in Wonderland
         * URL textureUrl = null;
         * try {
         *     textureUrl = AssetUtils.getAssetURL(url);
         * } catch (MalformedURLException ex) {
         *     return null;
         * }
         * return TextureManager.loadTexture(textureUrl,
         */
         return TextureManager.loadTexture(url,
                     Texture.MinificationFilter.NearestNeighborNoMipMaps,
                     Texture.MagnificationFilter.NearestNeighbor,
                     1.0f, true);
    }

    public static URL getUrl(String[] urls) {
        URL textureUrl = null;
        for (int i = 0; i < urls.length; i++) {
            try {
               /* in Wonderland
                * textureUrl = AssetUtils.getAssetURL(urls[i]);
                */
                String prefix = "";
                if (urls[i].charAt(0) == '/')
                    prefix = "//";
                textureUrl = new URL("file", "", prefix + urls[i]);
            } catch (MalformedURLException ex) {
                if (i == urls.length - 1)
                    return null;
                else
                    continue;
            }
            return textureUrl;
        }
        return textureUrl;
    }

    public static void makeTexture(Geometry geometry, X3dNode node) {
        TextureState textureState = X3d_TextureToJme.makeTexture(node);
        if (textureState != null)
            geometry.setRenderState(textureState);
    }

    public static void applyStates(Geometry geometry, boolean newStates,
                                   X3dNode node, X3dNode color,
                                   boolean solid, boolean ccw) {
        MaterialState materialState = null;
        X3d_Data data = (X3d_Data)node.m_data;
        // MaterialState needs to be reused
        if (newStates) {
            materialState = X3d_MaterialToJme.makeMaterial(node);
            data.materialState = materialState;
        } else {
            materialState = data.materialState;
            X3d_MaterialToJme.setMaterial(materialState, node);

        }
        geometry.setRenderState(X3d_AlphaToJme.makeAlpha());
        materialState.setMaterialFace(MaterialState.MaterialFace.FrontAndBack);
        if (solid)
           if (ccw)
              materialState.setMaterialFace(MaterialState.MaterialFace.Front);
           else
              materialState.setMaterialFace(MaterialState.MaterialFace.Back);

        if (color != null)
            geometry.setLightCombineMode(Spatial.LightCombineMode.Off);

        geometry.clearRenderState(RenderState.StateType.Cull);
        CullState cullState = DisplaySystem.getDisplaySystem().getRenderer().createCullState();
        cullState.setCullFace(CullState.Face.None);
        if (solid)
           if (ccw)
              cullState.setCullFace(CullState.Face.Back);
          else
              cullState.setCullFace(CullState.Face.Front);
        geometry.setRenderState(cullState);

        if (X3d_TextureToJme.isTransparentTexture(node) ||
            X3d_MaterialToJme.isTransparentMaterial(node)) {
            geometry.setRenderState(X3d_AlphaToJme.makeAlpha());
            geometry.setRenderQueueMode(Renderer.QUEUE_TRANSPARENT);
        }
        geometry.setRenderState(materialState);
    }
}

class X3d_Data {
    public Spatial node;
    public DoWithDataCallback callback = null;
    public MaterialState materialState = null;
    public Texture texture = null;
    public Texture oldTexture = null;
    public Texture veryOldTexture = null;
    public int oldIndex = -1;
}

class X3d_GeometryBuffers {
    public FloatBuffer verticesBuffer = null;
    public FloatBuffer normalsBuffer = null;
    public FloatBuffer colorsBuffer = null;
    public TexCoords texCoords = null;

    public void accountBuffers(X3dNode coord, X3dNode normal,
                               X3dNode color, X3dNode texCoord) {

        float vertices[] = null;
        verticesBuffer = null;
        if (coord != null) {
            vertices = ((X3dCoordinate)coord).point;
            verticesBuffer = BufferUtils.createFloatBuffer(vertices.length);
            verticesBuffer.put(vertices);
        }

        normalsBuffer = null;
        if (normal != null) {
            float normals[] = ((X3dNormal)normal).vector;
            if (normals != null) {
                normalsBuffer = BufferUtils.createFloatBuffer(normals.length);
                normalsBuffer.put(normals);
            }
        }

        if (vertices != null)
            colorsBuffer = X3d_ColorToJme.makeColor(color, vertices.length / 3);

        texCoords = null;
        if (texCoord != null) {
            float texCoordinates[] = ((X3dTextureCoordinate)texCoord).point;

            if (texCoordinates != null) {
                 FloatBuffer texCoordsBuffer = BufferUtils.createFloatBuffer(
                                                         texCoordinates.length);
                texCoordsBuffer.put(texCoordinates);
                texCoords = new TexCoords(texCoordsBuffer);
            }
        }
    }

    IntBuffer accountIndicesBuffer(int coordIndex[]) {

        int numIndices = 0;
        for (int i = 0; i < coordIndex.length; i++)
            if (coordIndex[i] > -1)
                numIndices++;
        int indices[] = new int[numIndices];
        int currentIndex = 0;
        for (int i = 0; i < coordIndex.length; i++)
            if (coordIndex[i] > -1)
                indices[currentIndex++] = coordIndex[i];

        IntBuffer indicesBuffer = BufferUtils.createIntBuffer(indices.length);
        indicesBuffer.put(indices);
        return indicesBuffer;
    }
}

class X3d_Interpolator {
    static float interpolate(float t, float key, float oldKey,
                             float value, float oldValue) {
        float delta = (key == oldKey) ? 0 : (t - oldKey) / (key - oldKey);
        return oldValue + delta * (value - oldValue);
    }

    public static void accountInterpolator(float target[], float fraction,
                                           float keys[], float keyValues[],
                                           int stride) {
        int numKeys = keys.length;
        if (numKeys > 0) {
            for (int j = 0; j < stride; j++)
                target[j]=keyValues[j];
            int oldi = 0;
            int i = 0;
            for (i = 0; i < numKeys; i++) {
                if ((fraction >= keys[oldi]) && (fraction < keys[i])) {
                    for (int j = 0; j < stride; j++)
                        target[j] = interpolate(fraction, keys[i], keys[oldi],
                                                keyValues[(i * stride + j)],
                                                keyValues[(oldi * stride + j)]);
                    break;
                }
                oldi = i;
            }
            if (i == numKeys)
                for (int j = 0; j < stride; j++)
                    target[j] = keyValues[(numKeys - 1) * stride + j];
        }
    }
}

class X3d_AppearanceToJme {
    static X3dAppearance getAppearance(X3dNode node) {
        if (((X3dShape)node.m_parent) == null)
            return null;
        return (X3dAppearance)((X3dShape)node.m_parent).appearance;
    }
}

class X3d_MaterialToJme {
    static X3dMaterial getMaterial(X3dNode node) {
        X3dAppearance appearance = X3d_AppearanceToJme.getAppearance(node);
        if (appearance != null) {
            X3dMaterial material = (X3dMaterial)((X3dAppearance)appearance).material;
            return material;
        }
        return null;
    }

    static void setMaterial(MaterialState materialState, X3dNode node) {
        X3dMaterial material = getMaterial(node);
        if (material == null)
            return;

        materialState.setAmbient(new ColorRGBA(material.diffuseColor[0],
                                               material.diffuseColor[1],
                                               material.diffuseColor[2],
                                               1 - material.transparency));

        materialState.setDiffuse(new ColorRGBA(material.diffuseColor[0],
                                               material.diffuseColor[1],
                                               material.diffuseColor[2],
                                               1 - material.transparency));

        materialState.setEmissive(new ColorRGBA(material.emissiveColor[0],
                                                material.emissiveColor[1],
                                                material.emissiveColor[2],
                                                1 - material.transparency));

        materialState.setSpecular(new ColorRGBA(material.specularColor[0],
                                                material.specularColor[1],
                                                material.specularColor[2],
                                                1 - material.transparency));

        materialState.setShininess(material.shininess * 128.0f);

        materialState.setEnabled(true);
   }

   static boolean isTransparentMaterial(X3dNode node) {
       X3dMaterial material = getMaterial(node);
       if (material != null)
           return material.transparency > 0.0f;
       return true;
   }

   static MaterialState makeMaterial(X3dNode node) {
       MaterialState materialState = DisplaySystem.getDisplaySystem().getRenderer().createMaterialState();
       setMaterial(materialState, node);
       return materialState;
   }
}

class X3d_TextureToJme {
    static TextureState makeTexture(X3dNode node) {
        X3dAppearance appearance = X3d_AppearanceToJme.getAppearance(node);
        if (appearance != null) {
            if ((appearance.texture != null) &&
                (appearance.texture.getClass() == X3dImageTexture.class)) {
                X3dImageTexture textureNode = (X3dImageTexture)appearance.texture;
                if (textureNode.url.length > 0) {
                    for (int i = 0; i < textureNode.url.length; i++) {
                        Texture texture = X3d_Util.loadTexture(textureNode.url[i]);
                        if (texture == null)
                            continue;
                        texture.setWrap(Texture.WrapAxis.S, textureNode.repeatS ? Texture.WrapMode.Repeat : Texture.WrapMode.Clamp);
                        texture.setWrap(Texture.WrapAxis.T, textureNode.repeatT ? Texture.WrapMode.Repeat : Texture.WrapMode.Clamp);
                        texture.setMatrix(X3d_TextureToJme.getTextureTransformMatrix(node));
                        X3d_Data data = (X3d_Data)node.m_data;
                        data.texture = texture;
                        TextureState textureState = DisplaySystem.getDisplaySystem().getRenderer().createTextureState();
                        textureState.setTexture(texture, 0);

                        return textureState;
                    }
                }
            }
        }
        return null;
    }

    static Matrix4f getTextureTransformMatrix(X3dNode node) {
        Matrix4f transformMatrix = new Matrix4f();
        X3dAppearance appearance = X3d_AppearanceToJme.getAppearance(node);
        if (appearance != null) {
            if ((appearance.texture != null) &&
                (appearance.texture.getClass() == X3dImageTexture.class)) {
                float scale[] = new float[2];
                scale[0] = 1.0f;
                scale[1] = 1.0f;
                float center[] = new float[2];
                center[0] = 1.0f;
                center[1] = 1.0f;
                float rotation = 0.0f;
                float translation[] = new float[2];
                translation[0] = 1.0f;
                translation[1] = 1.0f;
                if ((appearance.textureTransform != null) &&
                    (appearance.textureTransform.getClass() == X3dTextureTransform.class)) {
                    X3dTextureTransform textureTransformNode = (X3dTextureTransform)appearance.textureTransform;
                    scale[0] = textureTransformNode.scale[0];
                    scale[1] = textureTransformNode.scale[1];
                    center[0] = textureTransformNode.center[0];
                    center[1] = textureTransformNode.center[1];
                    rotation = textureTransformNode.rotation;
                    translation[0] = textureTransformNode.translation[0];
                    translation[1] = textureTransformNode.translation[1];
                }

                transformMatrix.loadIdentity();
                Matrix4f multMatrix = new Matrix4f();

                multMatrix.loadIdentity();
                multMatrix.setTranslation(-center[0], -center[1], 0.0f);
                transformMatrix = transformMatrix.mult(multMatrix);

                multMatrix.loadIdentity();
                multMatrix.scale(new Vector3f(scale[0], scale[1], 0));
                transformMatrix = transformMatrix.mult(multMatrix);

                multMatrix.loadIdentity();
                multMatrix.fromAngleAxis(rotation,
                                         new Vector3f(0.0f, 0.0f, 1.0f));
                transformMatrix = transformMatrix.mult(multMatrix);

                multMatrix.loadIdentity();
                multMatrix.setTranslation(center[0], center[1], 0.0f);
                transformMatrix = transformMatrix.mult(multMatrix);

                multMatrix.loadIdentity();
                multMatrix.setTranslation(translation[0], translation[1], 0.0f);
                transformMatrix = transformMatrix.mult(multMatrix);
            }
        }
        return transformMatrix;
    }


    static boolean isTransparentTexture(X3dNode node) {
        X3dAppearance appearance = X3d_AppearanceToJme.getAppearance(node);
        if (appearance != null) {
            if ((appearance.texture != null) &&
                (appearance.texture.getClass() == X3dImageTexture.class)) {
                X3dImageTexture textureNode = (X3dImageTexture)appearance.texture;
                URL textureUrl = X3d_Util.getUrl(textureNode.url);
                if (textureUrl != null) {
                    java.awt.Image img = java.awt.Toolkit.getDefaultToolkit().getImage(textureUrl);
                    return TextureManager.hasAlpha(img);
               }
            }
        }
        return false;
    }
}

class X3d_AlphaToJme {
    static BlendState makeAlpha() {
        BlendState blendState = DisplaySystem.getDisplaySystem().getRenderer().createBlendState();
        blendState.setTestEnabled(true);
        blendState.setBlendEnabled(true);
        blendState.setSourceFunction(BlendState.SourceFunction.SourceAlpha);
        blendState.setDestinationFunction(BlendState.DestinationFunction.OneMinusSourceAlpha);
        blendState.setTestFunction(BlendState.TestFunction.GreaterThan);

        return blendState;
    }
}

class X3d_ColorToJme {
    static FloatBuffer makeColor(X3dNode node, int needed_length) {

        int numDefinedColors = 0;
        float r = 0.8f;
        float b = 0.8f;
        float g = 0.8f;
        float a = 1;
        float color[] = new float[needed_length * 4];
        if (node != null) {
            if (node.getClass() == X3dColor.class) {
                X3dColor colors = (X3dColor) node;
                int colorLength = colors.color.length / 3 > needed_length ?
                                  needed_length : colors.color.length / 3;
                if (colors.color.length == 3) {
                    r = colors.color[0];
                    b = colors.color[1];
                    g = colors.color[2];
                } else {
                   for (int i = 0; i < colorLength; i++) {
                       for (int j = 0; j < 3; j++)
                           color[i * 4 + j] = colors.color[i * 3 + j];
                       color[i * 4 + 3] = 1;
                       numDefinedColors++;
                   }
                }
            } else if (node.getClass() == X3dColorRGBA.class) {
                X3dColorRGBA colors = (X3dColorRGBA) node;
                for (int i = 0; i < colors.color.length; i++) {
                    color[i] = colors.color[i];
                    numDefinedColors++;
                }
            }
        }
        for (int i = 0; i < needed_length; i++)
            color[i] = 0.8f;
        FloatBuffer colorsBuffer = BufferUtils.createFloatBuffer(color.length);
        colorsBuffer.put(color);
        return colorsBuffer;
    }
}


class MyTriangleSetDoWithDataCallback extends X3dTriangleSetDoWithDataCallback {

    X3d_GeometryBuffers buffers = new X3d_GeometryBuffers();

    IntBuffer accountIndicesBuffer(X3dNode coord) {

        float vertices[] = ((X3dCoordinate)coord).point;
        int indices[] = new int[vertices.length / 3];
        for (int i = 0; i < indices.length; i++)
            indices[i] = i;
        IntBuffer indicesBuffer = BufferUtils.createIntBuffer(indices.length);
        indicesBuffer.put(indices);
        return indicesBuffer;
    }

    public void update(X3dNode node) {
        X3dTriangleSet triangles = (X3dTriangleSet) node;

        buffers.accountBuffers(triangles.coord, triangles.normal,
                               triangles.color, triangles.texCoord);

        X3d_Data data = (X3d_Data)node.m_data;
        TriMesh tri = (TriMesh)data.node;
        tri.reconstruct(buffers.verticesBuffer,
                        buffers.normalsBuffer,
                        buffers.colorsBuffer,
                        buffers.texCoords,
                        accountIndicesBuffer(triangles.coord));

        X3d_Util.applyStates(tri, false, triangles,
                             triangles.color, triangles.solid, triangles.ccw);
    }

    public void doWithData(X3dNode node) {
        X3dTriangleSet triangles = (X3dTriangleSet) node;

        buffers.accountBuffers(triangles.coord, triangles.normal,
                               triangles.color, triangles.texCoord);

        TriMesh tri = new TriMesh("TriangleSet",
                                  buffers.verticesBuffer,
                                  buffers.normalsBuffer,
                                  buffers.colorsBuffer,
                                  buffers.texCoords,
                                  accountIndicesBuffer(triangles.coord));

        X3d_Data data = new X3d_Data();
        node.m_data = data;
        data.node = tri;
        data.callback = this;

        X3d_Util.applyStates(tri, true, triangles,
                             triangles.color, triangles.solid, triangles.ccw);

        X3d_Util.makeTexture(tri, triangles);
    }
}

class MyIndexedTriangleSetDoWithDataCallback extends X3dIndexedTriangleSetDoWithDataCallback {

    X3d_GeometryBuffers buffers = new X3d_GeometryBuffers();

    public void update(X3dNode node) {
        X3dIndexedTriangleSet triangles = (X3dIndexedTriangleSet) node;

        buffers.accountBuffers(triangles.coord, triangles.normal,
                               triangles.color, triangles.texCoord);

        X3d_Data data = (X3d_Data)node.m_data;
        TriMesh tri = (TriMesh)data.node;
        tri.reconstruct(buffers.verticesBuffer,
                        buffers.normalsBuffer,
                        buffers.colorsBuffer,
                        buffers.texCoords,
                        buffers.accountIndicesBuffer(triangles.index));

        X3d_Util.applyStates(tri, false, triangles,
                             triangles.color, triangles.solid, triangles.ccw);
    }

    public void doWithData(X3dNode node) {
        X3dIndexedTriangleSet triangles = (X3dIndexedTriangleSet) node;

        buffers.accountBuffers(triangles.coord, triangles.normal,
                               triangles.color, triangles.texCoord);

        TriMesh tri = new TriMesh("IndexedTriangleSet",
                                  buffers.verticesBuffer,
                                  buffers.normalsBuffer,
                                  buffers.colorsBuffer,
                                  buffers.texCoords,
                                  buffers.accountIndicesBuffer(triangles.index));

        X3d_Data data = new X3d_Data();
        node.m_data = data;
        data.node = tri;
        data.callback = this;

        X3d_Util.applyStates(tri, true, triangles,
                             triangles.color, triangles.solid, triangles.ccw);

        X3d_Util.makeTexture(tri, triangles);
    }
}

class MyCoordinateProcessEventCallback extends X3dCoordinateProcessEventCallback
{
    public boolean processEvent(X3dNode node, String event) {
        if (node != null)
            ((X3d_Data)node.m_parent.m_data).callback.update(node.m_parent);
        return true;
    }
}

class MyColorProcessEventCallback extends X3dColorProcessEventCallback
{
    public boolean processEvent(X3dNode node, String event) {
        if (node != null)
            ((X3d_Data)node.m_parent.m_data).callback.update(node.m_parent);
        return true;
    }
}

class MyNormalProcessEventCallback extends X3dNormalProcessEventCallback
{
    public boolean processEvent(X3dNode node, String event) {
        if (node != null)
            ((X3d_Data)node.m_parent.m_data).callback.update(node.m_parent);
        return true;
    }
}

class MyTextureCoordinateProcessEventCallback extends X3dTextureCoordinateProcessEventCallback
{
    public boolean processEvent(X3dNode node, String event) {
        if (node != null)
            ((X3d_Data)node.m_parent.m_data).callback.update(node.m_parent);
        return true;
    }
}

class MyIndexedFaceSetDoWithDataCallback extends X3dIndexedFaceSetDoWithDataCallback {

    X3d_GeometryBuffers buffers = new X3d_GeometryBuffers();

    public void accountBuffers(X3dNode coord, int coordIndices[],
                               X3dNode normal, int normalIndices[],
                               X3dNode color, int colorIndices[],
                               X3dNode texCoord, int texCoordIndices[]) {

        float vertices[] = null;
        buffers.verticesBuffer = null;
        if (coord != null) {
            vertices = ((X3dCoordinate)coord).point;
            buffers.verticesBuffer = BufferUtils.createFloatBuffer(vertices.length);
            buffers.verticesBuffer.put(vertices);
        }

        buffers.normalsBuffer = null;
        if (normal != null) {
            float normals[] = ((X3dNormal)normal).vector;
            float newNormals[] = new float[vertices.length];
            for (int i = 0; i < vertices.length; i++)
                 newNormals[i] = 0;
            if (normals != null) {
                int normalCount = 0;
                for (int i = 0; i < coordIndices.length; i++) {
                    int newIndex = coordIndices[i];
                    int newNormalIndex = newIndex;
                    if (normalIndices != null)
                        newNormalIndex = normalIndices[i];
                    if ((newIndex > -1) && (newIndex < vertices.length)) {
                        newNormals[newIndex * 3]     += normals[newNormalIndex * 3];
                        newNormals[newIndex * 3 + 1] += normals[newNormalIndex * 3 + 1];
                        newNormals[newIndex * 3 + 2] += normals[newNormalIndex * 3 + 2];
                    }
                }
                for (int i = 0; i < newNormals.length; i += 3) {
                    double len = java.lang.Math.sqrt(newNormals[i] *
                                                     newNormals[i] +
                                                     newNormals[i + 1] *
                                                     newNormals[i + 1] +
                                                     newNormals[i + 2] *
                                                     newNormals[i + 2]);
                    if (len != 0) {
                        newNormals[i] /= len;
                        newNormals[i + 1] /= len;
                        newNormals[i + 2] /= len;
                    }
                }

                buffers.normalsBuffer = BufferUtils.createFloatBuffer(newNormals.length);
                buffers.normalsBuffer.put(newNormals);
            }
        }

        if (color != null) {
            float colors[] = ((X3dColor)color).color;
            float newColors[] = new float[vertices.length / 3 * 4];
            for (int i = 0; i < vertices.length / 3 * 4; i++)
                 newColors[i] = 0;
            if (colors != null) {
                int colorCount = 0;
                for (int i = 0; i < coordIndices.length; i++) {
                    int newIndex = coordIndices[i];
                    int newColorIndex = newIndex;
                    if (colorIndices != null)
                        newColorIndex = colorIndices[i];
                    if ((newIndex > -1) && (newIndex < vertices.length)) {
                        newColors[newIndex * 4]     += colors[newColorIndex * 3];
                        newColors[newIndex * 4 + 1] += colors[newColorIndex * 3 + 1];
                        newColors[newIndex * 4 + 2] += colors[newColorIndex * 3 + 2];
                        newColors[newIndex * 4 + 3] += 1;
                    }
                }
                buffers.colorsBuffer = BufferUtils.createFloatBuffer(newColors.length);
                buffers.colorsBuffer.put(newColors);
            }
        } else if (vertices != null)
            buffers.colorsBuffer = X3d_ColorToJme.makeColor(color, vertices.length / 3);

        if (color != null) {
            float colors[] = ((X3dColor)color).color;
            float newColors[] = new float[vertices.length / 3 * 4];
            for (int i = 0; i < vertices.length / 3 * 4; i++)
                 newColors[i] = 0;
            if (colors != null) {
                int colorCount = 0;
                for (int i = 0; i < coordIndices.length; i++) {
                    int newIndex = coordIndices[i];
                    int newColorIndex = newIndex;
                    if (colorIndices != null)
                        newColorIndex = colorIndices[i];
                    if ((newIndex > -1) && (newIndex < vertices.length)) {
                        newColors[newIndex * 4]     += colors[newColorIndex * 3];
                        newColors[newIndex * 4 + 1] += colors[newColorIndex * 3 + 1];
                        newColors[newIndex * 4 + 2] += colors[newColorIndex * 3 + 2];
                        newColors[newIndex * 4 + 3] += 1;
                    }
                }
                buffers.colorsBuffer = BufferUtils.createFloatBuffer(newColors.length);
                buffers.colorsBuffer.put(newColors);
            }
        } else if (vertices != null)
            buffers.colorsBuffer = X3d_ColorToJme.makeColor(color, vertices.length / 3);

        buffers.texCoords = null;
        if (texCoord != null) {
            float texCoords[] = ((X3dTextureCoordinate)texCoord).point;
            if (texCoords != null) {
                float newTexCoords[] = new float[vertices.length / 3 * 2];
                for (int i = 0; i < vertices.length / 3 * 2; i++)
                     newTexCoords[i] = 0;
                if (texCoords != null) {
                    int texCoordCount = 0;
                    for (int i = 0; i < coordIndices.length; i++) {
                        int newIndex = coordIndices[i];
                        int newTexCoordIndex = newIndex;
                        if (texCoordIndices != null)
                            newTexCoordIndex = texCoordIndices[i];
                        if ((newIndex > -1) && (newIndex < vertices.length)) {
                            newTexCoords[newIndex * 2]     += texCoords[newTexCoordIndex * 2];
                            newTexCoords[newIndex * 2 + 1] += texCoords[newTexCoordIndex * 2 + 1];
                        }
                    }
                    FloatBuffer texCoordsBuffer = BufferUtils.createFloatBuffer(
                                                             newTexCoords.length);
                    texCoordsBuffer.put(newTexCoords);
                    buffers.texCoords = new TexCoords(texCoordsBuffer);
                }
            }
        }
    }

    public void update(X3dNode node) {
        X3dIndexedFaceSet triangles = (X3dIndexedFaceSet) node;

        accountBuffers(triangles.coord, triangles.coordIndex,
                       triangles.normal, triangles.normalIndex,
                       triangles.color, triangles.colorIndex,
                       triangles.texCoord, triangles.texCoordIndex);

        X3d_Data data = (X3d_Data)node.m_data;
        TriMesh tri = (TriMesh)data.node;
        tri.reconstruct(buffers.verticesBuffer,
                        buffers.normalsBuffer,
                        buffers.colorsBuffer,
                        buffers.texCoords,
                        buffers.accountIndicesBuffer(triangles.coordIndex));

        X3d_Util.applyStates(tri, false, triangles,
                             triangles.color, triangles.solid, triangles.ccw);
    }

    public void doWithData(X3dNode node) {
        X3dIndexedFaceSet triangles = (X3dIndexedFaceSet) node;

        accountBuffers(triangles.coord, triangles.coordIndex,
                       triangles.normal, triangles.normalIndex,
                       triangles.color, triangles.colorIndex,
                       triangles.texCoord, triangles.texCoordIndex);

        TriMesh tri = new TriMesh("triangulated IndexedFaceSet",
                                  buffers.verticesBuffer,
                                  buffers.normalsBuffer,
                                  buffers.colorsBuffer,
                                  buffers.texCoords,
                                  buffers.accountIndicesBuffer(triangles.coordIndex));

        X3d_Data data = new X3d_Data();
        node.m_data = data;
        data.node = tri;
        data.callback = this;

        X3d_Util.applyStates(tri, true, triangles,
                             triangles.color, triangles.solid, triangles.ccw);

        X3d_Util.makeTexture(tri, triangles);
    }
}

class MyIndexedLineSetDoWithDataCallback extends X3dIndexedLineSetDoWithDataCallback {

    X3d_GeometryBuffers buffers = new X3d_GeometryBuffers();

    FloatBuffer accountVerticesBuffer(X3dNode node) {
        X3dIndexedLineSet lines = (X3dIndexedLineSet) node;

        int indices[] = lines.coordIndex;
        float vertices[] = ((X3dCoordinate)lines.coord).point;
        float newVertices[] = new float[indices.length * 2 * 3];
        int newVerticesLen = 0;
        for (int i = 1; i < indices.length; i++) {
            if ((indices[i] > -1) && (indices[i - 1] > -1))  {
                newVertices[newVerticesLen++] = vertices[3 * indices[i - 1]];
                newVertices[newVerticesLen++] = vertices[3 * indices[i - 1] + 1];
                newVertices[newVerticesLen++] = vertices[3 * indices[i - 1] + 2];
                newVertices[newVerticesLen++] = vertices[3 * indices[i]];
                newVertices[newVerticesLen++] = vertices[3 * indices[i] + 1];
                newVertices[newVerticesLen++] = vertices[3 * indices[i] + 2];
            }
        }
        float lineVertices[] = new float[newVerticesLen];
        for (int i = 0; i < newVerticesLen; i++)
            lineVertices[i] = newVertices[i];
        FloatBuffer verticesBuffer = BufferUtils.createFloatBuffer(lineVertices.length);
        verticesBuffer.put(lineVertices);
        return verticesBuffer;
    }

    public void update(X3dNode node) {
        X3dIndexedLineSet lines = (X3dIndexedLineSet) node;

        if (lines.coord == null)
            return;

        FloatBuffer colorsBuffer = X3d_ColorToJme.makeColor(lines.color,
                                         lines.coordIndex.length * 2 * 3);

        X3d_Data data = (X3d_Data)node.m_data;
        Line line = (Line)data.node;
        line.reconstruct(accountVerticesBuffer(node), (FloatBuffer)null,
                         colorsBuffer, (TexCoords)null);

        X3d_Util.applyStates(line, false, lines,
                             lines.color, false, true);
    }

    public void doWithData(X3dNode node) {

        X3dIndexedLineSet lines = (X3dIndexedLineSet) node;

        if (lines.coord == null)
            return;

        FloatBuffer colorsBuffer = X3d_ColorToJme.makeColor(lines.color,
                                         lines.coordIndex.length * 2 * 3);

        FloatBuffer normalsBuffer = null;
        TexCoords texCoords = new TexCoords(null);
        Line line = new Line("IndexedLineSet", accountVerticesBuffer(node),
                                               normalsBuffer,
                                               colorsBuffer,
                                               texCoords);

        X3d_Data data = new X3d_Data();
        node.m_data = data;
        data.node = line;
        data.callback = this;

        X3d_Util.applyStates(line, true, lines,
                             lines.color, false, true);
   }
}

class MyPointSetDoWithDataCallback extends X3dPointSetDoWithDataCallback {

    public void update(X3dNode node) {
        X3dPointSet points = (X3dPointSet) node;

        if (points.coord == null)
            return;

        float vertices[] = ((X3dCoordinate)points.coord).point;
        FloatBuffer verticesBuffer = BufferUtils.createFloatBuffer(vertices.length);
        verticesBuffer.put(vertices);

        FloatBuffer colorsBuffer = X3d_ColorToJme.makeColor(points.color, vertices.length);

        FloatBuffer normalsBuffer = null;
        TexCoords texCoords = new TexCoords(null);

        X3d_Data data = (X3d_Data)node.m_data;
        Point point = (Point)data.node;
        point.reconstruct(verticesBuffer, (FloatBuffer)null, colorsBuffer,
                          (TexCoords)null);

        X3d_Util.applyStates(point, false, points,
                             points.color, false, true);
    }

    public void doWithData(X3dNode node) {

        X3dPointSet points = (X3dPointSet) node;

        if (points.coord == null)
            return;

        float vertices[] = ((X3dCoordinate)points.coord).point;
        FloatBuffer verticesBuffer = BufferUtils.createFloatBuffer(vertices.length);
        verticesBuffer.put(vertices);

        FloatBuffer colorsBuffer = X3d_ColorToJme.makeColor(points.color, vertices.length);

        FloatBuffer normalsBuffer = null;
        TexCoords texCoords = new TexCoords(null);
        Point point = new Point("PointSet", verticesBuffer,
                                            normalsBuffer,
                                            colorsBuffer,
                                            texCoords);

        X3d_Data data = new X3d_Data();
        node.m_data = data;
        data.node = point;
        data.callback = this;

        X3d_Util.applyStates(point, false, points,
                             points.color, false, true);
    }
}

class MyTextProcessEventCallback extends X3dTextProcessEventCallback
{
    public boolean processEvent(X3dNode node, String event) {
        if (node != null)
            ((X3d_Data)node.m_data).callback.update(node);
        return true;
    }
}

class MyTextDoWithDataCallback extends X3dTextDoWithDataCallback {

    final int textureUpdate = 1;
    final int textureSwitch = 2;
    final int triangulation = 3;

    int renderMethod = textureUpdate;

    final int textRes = 256;
    final int maxQuadRes = 2048; // maximal 2^n texturesize on (most ?) older cards
    final float scaleFactor = 1.2f;
    final float offset = 0.2f;


    ColorRGBA makeTextColor(X3dNode node) {
        ColorRGBA color = new ColorRGBA(0.8f, 0.8f, 0.8f, 0.0f);
        X3dAppearance appearance = (X3dAppearance)((X3dShape)node.m_parent).appearance;
        if (appearance != null) {
            X3dMaterial material = (X3dMaterial)((X3dAppearance)appearance).material;

            if (material != null) // try to emulate diffuse/emissive mix
                color = new ColorRGBA((material.diffuseColor[0] +
                                      1 -
                                      (1 - material.emissiveColor[0]) / 2) / 2.0f,
                                      (material.diffuseColor[1] +
                                      1 -
                                      (1 - material.emissiveColor[1]) / 2) / 2.0f,
                                      (material.diffuseColor[2] +
                                      1 -
                                      (1 - material.emissiveColor[2]) / 2) / 2.0f,
                                      1 - material.transparency);
        }
        return color;
    }

    int getFontSize(X3dNode node) {
        X3dText text = (X3dText) node;

        int size = 1;

        X3dFontStyle fontStyle = (X3dFontStyle)text.fontStyle;
        if (fontStyle != null)
            size = fontStyle.size < 1 ? 1 : (int)fontStyle.size;
        return size;
    }

    Font makeTextFont(X3dNode node) {
        X3dText text = (X3dText) node;

        Font font = null;

        X3dFontStyle fontStyle = (X3dFontStyle)text.fontStyle;
        int size = textRes;
        String family = "Serif";
        int style = Font.PLAIN;

        if (fontStyle != null) {
            if (fontStyle.style != null) {
                if (fontStyle.style.equals("PLAIN"))
                    style = Font.PLAIN;
                else if (fontStyle.style.equals("BOLD"))
                    style = Font.BOLD;
                else if (fontStyle.style.equals("ITALIC"))
                    style = Font.ITALIC;
                else if (fontStyle.style.equals("BOLDITALIC"))
                    style = Font.ITALIC | Font.BOLD;
                else
                    style = Font.PLAIN;
            }
            if (fontStyle.family != null) {
                for (int i = 0; i < fontStyle.family.length; i++) {
                    if (fontStyle.family[i].equals("SERIF")) {
                        family = "Serif";
                    } else if (fontStyle.family[i].equals("SANS")) {
                        family = "SansSerif";
                    } else if (fontStyle.family[i].equals("TYPEWRITER")) {
                        family = "Monospaced";
                    } else {
                        font = new Font(fontStyle.family[i], style, size);
                        if (font.getFamily().equals(fontStyle.family[i]))
                            break;
                        else
                            continue;
                    }
                    font = new Font(family, style, size);
                    break;
                }
            } else
               font = new Font(family, style, size);
        } else
            font = new Font(family, style, size);
        return font;
    }

    Spatial makeText(X3dNode node, boolean update) {
        X3dText text = (X3dText) node;

        boolean expensiveUpdate = false;
        if (renderMethod == textureSwitch)
            expensiveUpdate = true;

        if ((text.string == null) || (text.string.length == 0))
            return null;

        if (((X3dShape)node.m_parent) == null)
            return null;

        Quad txt = null;
        if (update)
            txt = (Quad)((X3d_Data)node.m_data).node;

        int len = 0;
        for (int i = 0; i < text.string.length; i++)
            if (text.string[i].length() > len)
                len = text.string[i].length();
        if (len == 0)
            return null;

        int size = getFontSize(node);

        int quadRes = 128;
        if ((len > 128) || (text.string.length > 128))
            quadRes = maxQuadRes;
        else if ((len > 64) || (text.string.length > 64))
            quadRes = (2048 >= maxQuadRes) ? maxQuadRes : 2048;
        else if ((len > 32) || (text.string.length > 32))
            quadRes = (1024 >= maxQuadRes) ? maxQuadRes : 1024;
        else if ((len > 16) || (text.string.length > 16))
            quadRes = 512;
        else if ((len > 8) || (text.string.length > 8))
            quadRes = 256;

        BufferedImage img = new BufferedImage(quadRes, quadRes,
                                              BufferedImage.TYPE_INT_ARGB);
        Graphics2D g2d = (Graphics2D) img.getGraphics();
        g2d.setFont(makeTextFont(node));
        g2d.setRenderingHint(RenderingHints.KEY_ANTIALIASING,
                             RenderingHints.VALUE_ANTIALIAS_ON);
        g2d.setRenderingHint(RenderingHints.KEY_TEXT_ANTIALIASING,
                             RenderingHints.VALUE_TEXT_ANTIALIAS_ON);
        AffineTransform transform = new AffineTransform();
        transform.scale(((float)quadRes) / (textRes * len),
                        ((float)quadRes) / (textRes * text.string.length));
        float colors[] = makeTextColor(node).getColorArray();
        Color color = new Color(colors[0], colors[1], colors[2]);
        g2d.setColor(color);
        g2d.transform(transform);
        for (int i = 0; i < text.string.length; i++) {
            g2d.drawString(text.string[i], 0, textRes * (i + 1.0f - offset));
        }

        if (txt == null)
            txt = new Quad("Text", len * size * scaleFactor, text.string.length * size * scaleFactor);
        else
            txt.resize(len * size * scaleFactor, text.string.length * size * scaleFactor);
        txt.setLocalTranslation(len * size / 2.0f, (2 - text.string.length) * size / 2.0f - offset, 0);

        if (update || expensiveUpdate) {
            TextureManager.releaseTexture(((X3d_Data)node.m_data).veryOldTexture);
            TextureManager.deleteTextureFromCard(((X3d_Data)node.m_data).veryOldTexture);
/*
            if (--counter == 0) {
                TextureManager.doTextureCleanup();
                counter = 10;
            }
*/
        }
        Texture tex = TextureManager.loadTexture(img,
                                     MinificationFilter.BilinearNoMipMaps,
                                     MagnificationFilter.Bilinear, true);
        ((X3d_Data)node.m_data).oldTexture = tex;
        ((X3d_Data)node.m_data).veryOldTexture = ((X3d_Data)node.m_data).oldTexture;
        TextureState ts = DisplaySystem.getDisplaySystem().getRenderer().createTextureState();
        ts.setEnabled(true);
        ts.setTexture(tex);
        txt.setRenderState(ts);
        txt.setRenderState(X3d_AlphaToJme.makeAlpha());
        txt.setRenderQueueMode(Renderer.QUEUE_TRANSPARENT);
        if (update || expensiveUpdate)
            txt.updateRenderState();
        return txt;
    }

    public void update(X3dNode node) {
        if (renderMethod == triangulation) {
            X3dText text = (X3dText) node;
            Text3D text3d = (Text3D)((X3d_Data)node.m_data).node;
            text3d.setText(text.string[0]);
        } else if (renderMethod == textureSwitch) {
            X3dShape shape = (X3dShape)node.m_parent;
            Node jmeShape = (Node)((X3d_Data)shape.m_data).node;
            boolean detached = false;
            if (shape.geometry != null)
                if (shape.geometry.m_data != null) {
                    Spatial txt = makeText(node, false);
                    detached = true;
                    if (!jmeShape.getChildren().isEmpty())
                        jmeShape.detachChildAt(0);
                    if (txt != null)
                        jmeShape.attachChild(txt);
                }
            if (!detached)
                if (!jmeShape.getChildren().isEmpty())
                    jmeShape.detachChildAt(0);
        } else
            makeText(node, true);
    }

    public void doWithData(X3dNode node) {
        X3dText text = (X3dText) node;
        node.m_data = new X3d_Data();

        if (renderMethod == triangulation) {
            Font3D font = new Font3D(makeTextFont(node), 0.0, false, true, true);
            Text3D text3d = new Text3D(font, text.string[0], 1);
            text3d.setFontColor(new ColorRGBA(1,0,0,0));
            ((X3d_Data)node.m_data).node = text3d;
        } else {
            ((X3d_Data)node.m_data).node = makeText(node, false);
        }

        ((X3d_Data)node.m_data).callback = this;
    }
}

class MyMaterialProcessEventCallback extends X3dMaterialProcessEventCallback {
    public boolean processEvent(X3dNode node) {
        X3dNode appearance = node.m_parent;
        if (appearance != null) {
            X3dShape shape = (X3dShape)(appearance.m_parent);
            if (shape != null) {
                X3dNode geometry = shape.geometry;
                if (geometry != null)
                    if ((geometry.m_data != null) &&
                        ((X3d_Data)geometry.m_data).callback != null)
                        ((X3d_Data)geometry.m_data).callback.update(geometry);
            }
        }
        return true;
    }
}

class MyTextureTransformProcessEventCallback extends X3dTextureTransformProcessEventCallback {
    public boolean processEvent(X3dNode node, String event) {
        X3dNode appearance = node.m_parent;
        if (appearance != null) {
            X3dShape shape = (X3dShape)(appearance.m_parent);
            if (shape != null) {
                X3dNode geometry = shape.geometry;
                if (geometry != null)
                    if ((geometry.m_data != null) &&
                        ((X3d_Data)geometry.m_data).callback != null) {
                        ((X3d_Data)geometry.m_data).callback.update(geometry);
                        Texture texture = ((X3d_Data)geometry.m_data).texture;
                        TextureState textureState = DisplaySystem.getDisplaySystem().getRenderer().createTextureState();
                        texture.setMatrix(X3d_TextureToJme.getTextureTransformMatrix(geometry));
                        textureState.setTexture(texture, 0);
                    }
            }
        }
        return true;
    }
}

class MyShapeProcessEventCallback extends X3dShapeProcessEventCallback {
    public boolean processEvent(X3dNode node, String event) {
        MyShapeDoWithDataCallback callback = (MyShapeDoWithDataCallback)(((X3d_Data)node.m_data).callback);
        callback.update(node);
        return true;
    }
}

class MyShapeDoWithDataCallback extends X3dShapeDoWithDataCallback {
    public void update(X3dNode node) {
        X3dNode updateNode = ((X3dShape)node.m_parent.m_parent).geometry;
        ((X3d_Data)updateNode.m_data).callback.update(updateNode);
    }

    public void doWithData(X3dNode node) {
        X3dShape shape = (X3dShape) node;
        Node jmeShape = new Node("Shape");
        if (shape.geometry != null)
            if (shape.geometry.m_data != null)
                if (((X3d_Data)shape.geometry.m_data).node != null)
                    jmeShape.attachChild(((Spatial)((X3d_Data)shape.geometry.m_data).node));

        node.m_data = new X3d_Data();
        ((X3d_Data)node.m_data).node = jmeShape;
        ((X3d_Data)node.m_data).callback = this;
    }
}

class MyTransformDoWithDataCallback extends X3dTransformDoWithDataCallback {

    public void update(X3dNode node) {
        X3dTransform transform = (X3dTransform) node;

        Node jmeTransform = ((Node)((X3d_Data)node.m_data).node);

        // a usual 4x4 Matrix holding all information can not be applied
        // directly to a Spatial, so some information (scaleOrientation gets lost)
        TransformMatrix matrix = new TransformMatrix();
        matrix.loadIdentity();
        TransformMatrix multMatrix = new TransformMatrix();
        Vector3f tempStore = new Vector3f();
        Quaternion quat = new Quaternion();

        multMatrix.loadIdentity();
        multMatrix.setTranslation(transform.translation[0],
                                  transform.translation[1],
                                  transform.translation[2]);
        matrix.multLocal(multMatrix, tempStore);

        multMatrix.loadIdentity();
        multMatrix.setTranslation(transform.center[0],
                                  transform.center[1],
                                  transform.center[2]);
        matrix.multLocal(multMatrix, tempStore);

        multMatrix.loadIdentity();
        quat.fromAngleAxis(transform.rotation[3],
                           new Vector3f(transform.rotation[0],
                                        transform.rotation[1],
                                        transform.rotation[2]));
        multMatrix.setRotationQuaternion(quat);
        matrix.multLocal(multMatrix, tempStore);

        // scaleOrientation (shearing ?) not supported by TransformMatrix ?
        multMatrix.loadIdentity();
        quat.loadIdentity();
        quat.fromAngleAxis(transform.scaleOrientation[3],
                           new Vector3f(transform.scaleOrientation[0],
                                        transform.scaleOrientation[1],
                                        transform.scaleOrientation[2]));
        multMatrix.setRotationQuaternion(quat);
        matrix.multLocal(multMatrix, tempStore);

        multMatrix.loadIdentity();
        multMatrix.setScale(new Vector3f(transform.scale[0],
                                         transform.scale[1],
                                         transform.scale[2]));
        matrix.multLocal(multMatrix, tempStore);

        // scaleOrientation (shearing ?) not supported by TransformMatrix ?
        multMatrix.loadIdentity();
        quat.loadIdentity();
        quat.fromAngleAxis(-transform.scaleOrientation[3],
                           new Vector3f(transform.scaleOrientation[0],
                                        transform.scaleOrientation[1],
                                        transform.scaleOrientation[2]));
        multMatrix.setRotationQuaternion(quat);
        matrix.multLocal(multMatrix, tempStore);

        // "TransformMatrix" is not a 4x4 Matrix, -center needs scale
        multMatrix.loadIdentity();
        multMatrix.setTranslation(-transform.center[0] * transform.scale[0],
                                  -transform.center[1] * transform.scale[1],
                                  -transform.center[2] * transform.scale[2]);
        matrix.multLocal(multMatrix, tempStore);

        matrix.applyToSpatial(jmeTransform);
    }

    public void doWithData(X3dNode node) {
        X3dTransform transform = (X3dTransform) node;
        Node jmeTransform = new Node("Transform");
        node.m_data = new X3d_Data();
        ((X3d_Data)node.m_data).node = jmeTransform;
        ((X3d_Data)node.m_data).callback = this;
        if (transform.children != null) {
            for (int i = 0; i < transform.children.length; i++) {
                if (transform.children[i] == null)
                    continue;
                if (transform.children[i].m_data == null)
                    continue;
                if (((X3d_Data)transform.children[i].m_data).node == null)
                    continue;
                jmeTransform.attachChild(((Node)((X3d_Data)transform.children[i].m_data).node));
            }
        }
        update(node);
    }
}

class MyTransformProcessEventCallback extends X3dTransformProcessEventCallback {
    public boolean processEvent(X3dNode node, String event) {
        MyTransformDoWithDataCallback callback = (MyTransformDoWithDataCallback)(((X3d_Data)node.m_data).callback);
        callback.update(node);
        return true;
    }
}

class MyGroupDoWithDataCallback extends X3dGroupDoWithDataCallback {

    public void doWithData(X3dNode node) {
        X3dGroup group = (X3dGroup)node;
        Node jmeGroup = new Node("Group");
        if (node.m_parent != null) {
            // not root node
            node.m_data = new X3d_Data();
            ((X3d_Data)node.m_data).node = jmeGroup;
        }
        if (group.children != null) {
            for (int i = 0; i < group.children.length; i++) {
                if (group.children[i] == null)
                    continue;
                if (group.children[i].m_data == null)
                    continue;
                if (((X3d_Data)group.children[i].m_data).node == null)
                    continue;
                ((Node)((X3d_Data)node.m_data).node).attachChild(((Node)((X3d_Data)group.children[i].m_data).node));
            }
        }
    }
}

class MyAnchorDoWithDataCallback extends X3dAnchorDoWithDataCallback {

    public void doWithData(X3dNode node) {
        X3dAnchor anchor = (X3dAnchor)node;
        Node jmeAnchor = new Node("Anchor");
        if (anchor.children != null) {
            for (int i = 0; i < anchor.children.length; i++) {
                if (anchor.children[i] == null)
                    continue;
                if (anchor.children[i].m_data == null)
                    continue;
                if (((X3d_Data)anchor.children[i].m_data).node == null)
                    continue;
                jmeAnchor.attachChild(((Node)((X3d_Data)anchor.children[i].m_data).node));
            }
       }
       node.m_data = new X3d_Data();
       ((X3d_Data)node.m_data).node = jmeAnchor;
    }
}

class MySwitchProcessEventCallback extends X3dSwitchProcessEventCallback
{
    public boolean processEvent(X3dNode node, String event) {
        if (node != null)
            ((X3d_Data)node.m_data).callback.update(node);
        return true;
    }
}

class MySwitchDoWithDataCallback extends X3dSwitchDoWithDataCallback {

    public void update(X3dNode node) {
        X3dSwitch swith = (X3dSwitch)node;
        if (swith.whichChoice == ((X3d_Data)node.m_data).oldIndex)
            return;
        Node jmeSwitch = (Node)((X3d_Data)node.m_data).node;
        if (!jmeSwitch.getChildren().isEmpty())
            jmeSwitch.detachChildAt(0);
        if ((swith.whichChoice < 0) ||
            (swith.whichChoice >= swith.children.length))
            return;
        if ((swith.children[swith.whichChoice] != null) &&
            (swith.children[swith.whichChoice].m_data != null) &&
            (((X3d_Data)swith.children[swith.whichChoice].m_data).node != null))
            jmeSwitch.attachChild(((Node)((X3d_Data)swith.children[swith.whichChoice].m_data).node));
        ((X3d_Data)node.m_data).oldIndex = swith.whichChoice;
    }

    public void doWithData(X3dNode node) {
        X3dSwitch swith = (X3dSwitch)node;
        if ((swith.whichChoice < 0) ||
            (swith.whichChoice >= swith.children.length))
            return;
        Node jmeSwitch = new Node("Switch");
        if ((swith.children[swith.whichChoice] != null) &&
            (swith.children[swith.whichChoice].m_data != null) &&
            (((X3d_Data)swith.children[swith.whichChoice].m_data).node != null))
            jmeSwitch.attachChild(((Node)((X3d_Data)swith.children[swith.whichChoice].m_data).node));
        node.m_data = new X3d_Data();
        ((X3d_Data)node.m_data).node = jmeSwitch;
        ((X3d_Data)node.m_data).callback = this;
        ((X3d_Data)node.m_data).oldIndex = swith.whichChoice;
    }
}

class MyCollisionDoWithDataCallback extends X3dCollisionDoWithDataCallback {

    public void doWithData(X3dNode node) {
        X3dCollision collision = (X3dCollision)node;
        Node jmeCollision = new Node("Collision");
        if (collision.children != null) {
            for (int i = 0; i < collision.children.length; i++) {
                if (collision.children[i] == null)
                    continue;
                if (collision.children[i].m_data == null)
                    continue;
                if (((X3d_Data)collision.children[i].m_data).node == null)
                    continue;
                jmeCollision.attachChild(((Node)((X3d_Data)collision.children[i].m_data).node));
            }
        }
        node.m_data = new X3d_Data();
        ((X3d_Data)node.m_data).node = jmeCollision;
    }
}

class MyBillboardDoWithDataCallback extends X3dBillboardDoWithDataCallback {

    public void doWithData(X3dNode node) {
        X3dBillboard billboard = (X3dBillboard)node;
        boolean valid = false;
        int axis = -1;
        if (billboard.axisOfRotation[0] == 0)
            if (billboard.axisOfRotation[1] == 0)
                if (billboard.axisOfRotation[2] == 0) {
                    valid = true;
                    axis = BillboardNode.CAMERA_ALIGNED;
                } else {
                    valid = true;
                    axis = BillboardNode.AXIAL_Z;
                } else if (billboard.axisOfRotation[2] == 0) {
                   valid = true;
                   axis = BillboardNode.AXIAL_Y;
                }

        BillboardNode jmeBillboard = new BillboardNode(valid ? "Billboard" :
                                                       "unsupported Billboard");
        if (valid)
            jmeBillboard.setAlignment(axis);
        node.m_data = new X3d_Data();
        ((X3d_Data)node.m_data).node = jmeBillboard;
        if (billboard.children != null) {
            for (int i = 0; i < billboard.children.length; i++) {
                if (billboard.children[i] == null)
                    continue;
                if (billboard.children[i].m_data == null)
                    continue;
                if (((X3d_Data)billboard.children[i].m_data).node == null)
                    continue;
                jmeBillboard.attachChild(((Node)((X3d_Data)billboard.children[i].m_data).node));
            }
        }
    }
}

class MyLODDoWithDataCallback extends X3dLODDoWithDataCallback {

    public void doWithData(X3dNode node) {
        X3dLOD lod = (X3dLOD)node;
        DistanceSwitchModel switchModel = new DistanceSwitchModel();
        DiscreteLodNode jmeLOD = new DiscreteLodNode("LOD", switchModel);
        node.m_data = new X3d_Data();
        ((X3d_Data)node.m_data).node = jmeLOD;
        if (lod.children != null) {
            for (int i = 0; i < lod.children.length; i++) {
                if (lod.children[i] == null)
                    continue;
                if (lod.children[i].m_data == null)
                    continue;
                if (((X3d_Data)lod.children[i].m_data).node == null)
                    continue;
                jmeLOD.attachChild(((Node)((X3d_Data)lod.children[i].m_data).node));
                if (i < lod.children.length) {
                    float minRange = 0;
                    if (i > 0)
                        minRange = lod.range[i - 1];
                    float maxRange = java.lang.Float.MAX_VALUE;
                    if (i < lod.range.length)
                        maxRange = lod.range[i];
                    switchModel.setModelDistance(i, minRange, maxRange);
                }
            }
        }
    }
}

class MyCADPartDoWithDataCallback extends X3dCADPartDoWithDataCallback {

    public void doWithData(X3dNode node) {
        X3dCADPart cadPart = (X3dCADPart)node;
        Node jmeCADPart = new Node("CADPart");

        TransformMatrix matrix = new TransformMatrix();
        TransformMatrix multMatrix = new TransformMatrix();
        Vector3f tempStore = new Vector3f();
        Quaternion quat = new Quaternion();

        multMatrix.setTranslation(cadPart.translation[0],
                                  cadPart.translation[1],
                                  cadPart.translation[2]);
        matrix.multLocal(multMatrix, tempStore);

        multMatrix.loadIdentity();
        multMatrix.setTranslation(cadPart.center[0],
                                  cadPart.center[1],
                                  cadPart.center[2]);
        matrix.multLocal(multMatrix, tempStore);

        multMatrix.loadIdentity();
        quat.fromAngleAxis(cadPart.rotation[3],
                           new Vector3f(cadPart.rotation[0],
                                        cadPart.rotation[1],
                                        cadPart.rotation[2]));
        multMatrix.setRotationQuaternion(quat);
        matrix.multLocal(multMatrix, tempStore);

        // scaleOrientation (shearing ?) not supported by TransformMatrix ?
        multMatrix.loadIdentity();
        quat.loadIdentity();
        quat.fromAngleAxis(cadPart.scaleOrientation[3],
                           new Vector3f(cadPart.scaleOrientation[0],
                                        cadPart.scaleOrientation[1],
                                        cadPart.scaleOrientation[2]));
        multMatrix.setRotationQuaternion(quat);
        matrix.multLocal(multMatrix, tempStore);

        multMatrix.loadIdentity();
        multMatrix.setScale(new Vector3f(cadPart.scale[0],
                                         cadPart.scale[1],
                                         cadPart.scale[2]));
        matrix.multLocal(multMatrix, tempStore);

        // scaleOrientation (shearing ?) not supported by TransformMatrix ?
        multMatrix.loadIdentity();
        quat.loadIdentity();
        quat.fromAngleAxis(-cadPart.scaleOrientation[3],
                           new Vector3f(cadPart.scaleOrientation[0],
                                        cadPart.scaleOrientation[1],
                                        cadPart.scaleOrientation[2]));
        multMatrix.setRotationQuaternion(quat);
        matrix.multLocal(multMatrix, tempStore);

        // "TransformMatrix" is not a 4x4 Matrix, -center needs scale
        multMatrix.loadIdentity();
        multMatrix.setTranslation(-cadPart.center[0] * cadPart.scale[0],
                                  -cadPart.center[1] * cadPart.scale[1],
                                  -cadPart.center[2] * cadPart.scale[2]);
        matrix.multLocal(multMatrix, tempStore);

        matrix.applyToSpatial(jmeCADPart);
        if (cadPart.children != null) {
            for (int i = 0; i < cadPart.children.length; i++) {
                if (cadPart.children[i] == null)
                    continue;
                if (cadPart.children[i].m_data == null)
                    continue;
                if (((X3d_Data)cadPart.children[i].m_data).node == null)
                    continue;
                jmeCADPart.attachChild(((Node)((X3d_Data)cadPart.children[i].m_data).node));
            }
        }
        node.m_data = new X3d_Data();
        ((X3d_Data)node.m_data).node = jmeCADPart;
    }
}

class MyCADAssemblyDoWithDataCallback extends X3dCADAssemblyDoWithDataCallback {

    public void doWithData(X3dNode node) {
        X3dCADAssembly cadAssembly = (X3dCADAssembly)node;
        Node jmeCADAssembly = new Node("CADAssembly");
        if (cadAssembly.children != null) {
            for (int i = 0; i < cadAssembly.children.length; i++) {
                if (cadAssembly.children[i] == null)
                    continue;
                if (cadAssembly.children[i].m_data == null)
                    continue;
                if (((X3d_Data)cadAssembly.children[i].m_data).node == null)
                    continue;
                jmeCADAssembly.attachChild((Node)((X3d_Data)cadAssembly.children[i].m_data).node);
            }
        }
        node.m_data = new X3d_Data();
        ((X3d_Data)node.m_data).node = jmeCADAssembly;
    }
}

class MyCADLayerDoWithDataCallback extends X3dCADLayerDoWithDataCallback {

    public void doWithData(X3dNode node) {
        X3dCADLayer cadLayer = (X3dCADLayer)node;
        Node jmeCADLayer = new Node("CADLayer");
        node.m_data = new X3d_Data();
        ((X3d_Data)node.m_data).node = jmeCADLayer;
        if (cadLayer.children == null)
            return;
        for (int i = 0; i < cadLayer.children.length; i++) {
            if (cadLayer.children[i] == null)
                continue;
            if (cadLayer.children[i].m_data == null)
                continue;
            if (((X3d_Data)cadLayer.children[i].m_data).node == null)
                continue;
            if ((cadLayer.visible == null) || (i >= cadLayer.visible.length))
                ((Node)((X3d_Data)node.m_data).node).attachChild(((Node)((X3d_Data)cadLayer.children[i].m_data).node));
            else if (cadLayer.visible[i])
                ((Node)((X3d_Data)node.m_data).node).attachChild(((Node)((X3d_Data)cadLayer.children[i].m_data).node));
        }
    }
}

class MyCADFaceDoWithDataCallback extends X3dCADFaceDoWithDataCallback {

    public void doWithData(X3dNode node) {
        X3dCADFace cadFace = (X3dCADFace)node;
        Node jmeCADFace = new Node("CADFace");
        node.m_data = new X3d_Data();
        ((X3d_Data)node.m_data).node = jmeCADFace;
        ((Node)((X3d_Data)node.m_data).node).attachChild(((Node)((X3d_Data)cadFace.shape.m_data).node));
    }
}

class MyPositionInterpolatorProcessEventCallback extends X3dPositionInterpolatorProcessEventCallback {

    private boolean warned;

    MyPositionInterpolatorProcessEventCallback() {
        warned = false;
    }

    public boolean processEvent(X3dNode node, String event) {
        X3dPositionInterpolator interpolator = (X3dPositionInterpolator)node;
        if (interpolator.key.length * 3 != interpolator.keyValue.length) {
            if (!warned)
               System.out.println("invalid PositionInterpolator: number keys do not match number values");
            warned = true;
            return false;
        }
        X3d_Interpolator.accountInterpolator(interpolator.value_changed,
                                             interpolator.set_fraction,
                                             interpolator.key,
                                             interpolator.keyValue, 3);
        return true;
    }
}

class MyOrientationInterpolatorProcessEventCallback extends X3dOrientationInterpolatorProcessEventCallback {

    private boolean warned;

    MyOrientationInterpolatorProcessEventCallback() {
        warned = false;
    }

    public boolean processEvent(X3dNode node, String event) {
        X3dOrientationInterpolator interpolator = (X3dOrientationInterpolator)node;
        if (interpolator.key.length * 4 != interpolator.keyValue.length) {
            if (!warned)
               System.out.println("invalid OrientationInterpolator: number keys do not match number values");
            warned = true;
            return false;
        }
        X3d_Interpolator.accountInterpolator(interpolator.value_changed,
                                             interpolator.set_fraction,
                                             interpolator.key,
                                             interpolator.keyValue, 4);
        return true;
    }
}

class MyColorInterpolatorProcessEventCallback extends X3dColorInterpolatorProcessEventCallback {

    private boolean warned;

    MyColorInterpolatorProcessEventCallback() {
        warned = false;
    }

    public boolean processEvent(X3dNode node, String event) {
        X3dColorInterpolator interpolator = (X3dColorInterpolator)node;
        if (interpolator.key.length * 3 != interpolator.keyValue.length) {
            if (!warned)
               System.out.println("invalid ColorInterpolator: number keys do not match number values");
            warned = true;
            return false;
        }
        X3d_Interpolator.accountInterpolator(interpolator.value_changed,
                                             interpolator.set_fraction,
                                             interpolator.key,
                                             interpolator.keyValue, 3);
        return true;
    }
}

class MyScalarInterpolatorProcessEventCallback extends X3dScalarInterpolatorProcessEventCallback {

    private boolean warned;

    MyScalarInterpolatorProcessEventCallback() {
        warned = false;
    }

    public boolean processEvent(X3dNode node, String event) {
        X3dScalarInterpolator interpolator = (X3dScalarInterpolator)node;
        if (interpolator.key.length != interpolator.keyValue.length) {
            if (!warned)
               System.out.println("invalid ScalarInterpolator: number keys do not match number values");
            warned = true;
            return false;
        }
        float value[] = new float[1];
        X3d_Interpolator.accountInterpolator(value,
                                             interpolator.set_fraction,
                                             interpolator.key,
                                             interpolator.keyValue, 1);
        interpolator.value_changed = value[0];
        return true;
    }
}


class MyCoordinateInterpolatorProcessEventCallback extends X3dCoordinateInterpolatorProcessEventCallback {

    int targetSize;

    public boolean processEvent(X3dNode node, String event) {
        X3dCoordinateInterpolator interpolator = (X3dCoordinateInterpolator)node;
        if (interpolator.value_changed == null) {
            targetSize = interpolator.keyValue.length / interpolator.key.length;
            interpolator.value_changed = new float[targetSize == 0 ? 1 :
                                                   targetSize];
        }
        X3d_Interpolator.accountInterpolator(interpolator.value_changed,
                                             interpolator.set_fraction,
                                             interpolator.key,
                                             interpolator.keyValue,
                                             interpolator.value_changed.length);
        return true;
    }
}

class MyNormalInterpolatorProcessEventCallback extends X3dNormalInterpolatorProcessEventCallback {

    public boolean processEvent(X3dNode node, String event) {
        int targetSize = 0;

        X3dNormalInterpolator interpolator = (X3dNormalInterpolator)node;
        if (interpolator.value_changed == null) {
            targetSize = interpolator.keyValue.length / interpolator.key.length;
            interpolator.value_changed = new float[targetSize == 0 ? 1 :
                                                   targetSize];
        }
        X3d_Interpolator.accountInterpolator(interpolator.value_changed,
                                             interpolator.set_fraction,
                                             interpolator.key,
                                             interpolator.keyValue,
                                             interpolator.value_changed.length);
        /* normalize results */
        for (int i = 0; i < targetSize; i += 3) {
            double vec0 = interpolator.keyValue[3 * i];
            double vec1 = interpolator.keyValue[3 * i + 1];
            double vec2 = interpolator.keyValue[3 * i + 2];
            double len = Math.sqrt(vec0 * vec0 + vec1 * vec1 + vec2 * vec2);
            if (len != 0)
                for (int j = 0; j < 3; j++)
                    interpolator.keyValue[3 * i + j] /= len;
        }   
        return true;
    }
}

class MyTimeSensorProcessEventCallback extends X3dTimeSensorProcessEventCallback {

    com.jme.util.lwjgl.LWJGLTimer timer;

    MyTimeSensorProcessEventCallback() {
        timer = new com.jme.util.lwjgl.LWJGLTimer();
    }

    public boolean processEvent(X3dNode node, String event) {
        X3dTimeSensor timeSensor = (X3dTimeSensor)node;

        timer.update();
        timeSensor.fraction_changed = (float)((timer.getTimeInSeconds() %
                                               timeSensor.cycleInterval) /
                                              timeSensor.cycleInterval);
        return true;
    }
}


public class main extends SimpleGame{

    X3dSceneGraph sceneGraph;

    int loops = 0;

    protected void simpleUpdate() {
        if (loops++ < 10)
            System.out.println("press escape to end");
        sceneGraph.X3dProcessEvents();
    }

    public static void main(String args[]) {
       main app = new main();
//       app.setConfigShowMode(ConfigShowMode.AlwaysShow);
       app.start();
    }
    protected static MyTriangleSetDoWithDataCallback myTriangleSetDoWithDataCallback = new MyTriangleSetDoWithDataCallback();
    protected static MyIndexedTriangleSetDoWithDataCallback myIndexedTriangleSetDoWithDataCallback = new MyIndexedTriangleSetDoWithDataCallback();
    protected static MyIndexedFaceSetDoWithDataCallback myIndexedFaceSetDoWithDataCallback = new MyIndexedFaceSetDoWithDataCallback();
    protected static MyTextDoWithDataCallback myTextDoWithDataCallback = new MyTextDoWithDataCallback();
    protected static MyIndexedLineSetDoWithDataCallback myIndexedLineSetDoWithDataCallback = new MyIndexedLineSetDoWithDataCallback();
    protected static MyPointSetDoWithDataCallback myPointSetDoWithDataCallback = new MyPointSetDoWithDataCallback();
    protected static MyShapeDoWithDataCallback myShapeDoWithDataCallback = new MyShapeDoWithDataCallback();
    protected static MyTransformDoWithDataCallback myTransformDoWithDataCallback = new MyTransformDoWithDataCallback();
    protected static MyGroupDoWithDataCallback myGroupDoWithDataCallback = new MyGroupDoWithDataCallback();
    protected static MyAnchorDoWithDataCallback myAnchorDoWithDataCallback = new MyAnchorDoWithDataCallback();
    protected static MySwitchDoWithDataCallback mySwitchDoWithDataCallback = new MySwitchDoWithDataCallback();
    protected static MyCollisionDoWithDataCallback myCollisionDoWithDataCallback = new MyCollisionDoWithDataCallback();
    protected static MyBillboardDoWithDataCallback myBillboardDoWithDataCallback = new MyBillboardDoWithDataCallback();
    protected static MyLODDoWithDataCallback myLODDoWithDataCallback = new MyLODDoWithDataCallback();
    protected static MyCADPartDoWithDataCallback myCADPartDoWithDataCallback = new MyCADPartDoWithDataCallback();
    protected static MyCADAssemblyDoWithDataCallback myCADAssemblyDoWithDataCallback = new MyCADAssemblyDoWithDataCallback();
    protected static MyCADLayerDoWithDataCallback myCADLayerDoWithDataCallback = new MyCADLayerDoWithDataCallback();
    protected static MyCADFaceDoWithDataCallback myCADFaceDoWithDataCallback = new MyCADFaceDoWithDataCallback();

    protected static MyTimeSensorProcessEventCallback myTimeSensorProcessEventCallback = new MyTimeSensorProcessEventCallback();
    protected static MyPositionInterpolatorProcessEventCallback myPositionInterpolatorProcessEventCallback = new MyPositionInterpolatorProcessEventCallback();
    protected static MyOrientationInterpolatorProcessEventCallback myOrientationInterpolatorProcessEventCallback = new MyOrientationInterpolatorProcessEventCallback();
    protected static MyTransformProcessEventCallback myTransformProcessEventCallback = new MyTransformProcessEventCallback();
    protected static MyColorInterpolatorProcessEventCallback myColorInterpolatorProcessEventCallback = new MyColorInterpolatorProcessEventCallback();
    protected static MyScalarInterpolatorProcessEventCallback myScalarInterpolatorProcessEventCallback = new MyScalarInterpolatorProcessEventCallback();
    protected static MyCoordinateInterpolatorProcessEventCallback myCoordinateInterpolatorProcessEventCallback = new MyCoordinateInterpolatorProcessEventCallback();
    protected static MyNormalInterpolatorProcessEventCallback myNormalInterpolatorProcessEventCallback = new MyNormalInterpolatorProcessEventCallback();
    protected static MyMaterialProcessEventCallback myMaterialProcessEventCallback = new MyMaterialProcessEventCallback();
    protected static MyTextureTransformProcessEventCallback myTextureTransformProcessEventCallback = new MyTextureTransformProcessEventCallback();
    protected static MyCoordinateProcessEventCallback myCoordinateProcessEventCallback = new MyCoordinateProcessEventCallback();
    protected static MyColorProcessEventCallback myColorProcessEventCallback = new MyColorProcessEventCallback();
    protected static MyNormalProcessEventCallback myNormalProcessEventCallback = new MyNormalProcessEventCallback();
    protected static MyTextureCoordinateProcessEventCallback myTextureCoordinateProcessEventCallback = new MyTextureCoordinateProcessEventCallback();
    protected static MyTextProcessEventCallback myTextProcessEventCallback = new MyTextProcessEventCallback();
    protected static MySwitchProcessEventCallback mySwitchProcessEventCallback = new MySwitchProcessEventCallback();

    protected static void setCallbacks() {
        X3dTriangleSet.setX3dTriangleSetDoWithDataCallback(myTriangleSetDoWithDataCallback);
        X3dIndexedTriangleSet.setX3dIndexedTriangleSetDoWithDataCallback(myIndexedTriangleSetDoWithDataCallback);
        X3dIndexedFaceSet.setX3dIndexedFaceSetDoWithDataCallback(myIndexedFaceSetDoWithDataCallback);
        X3dText.setX3dTextDoWithDataCallback(myTextDoWithDataCallback);
        X3dIndexedLineSet.setX3dIndexedLineSetDoWithDataCallback(myIndexedLineSetDoWithDataCallback);
        X3dPointSet.setX3dPointSetDoWithDataCallback(myPointSetDoWithDataCallback);
        X3dShape.setX3dShapeDoWithDataCallback(myShapeDoWithDataCallback);
        X3dTransform.setX3dTransformDoWithDataCallback(myTransformDoWithDataCallback);
        X3dGroup.setX3dGroupDoWithDataCallback(myGroupDoWithDataCallback);
        X3dAnchor.setX3dAnchorDoWithDataCallback(myAnchorDoWithDataCallback);
        X3dSwitch.setX3dSwitchDoWithDataCallback(mySwitchDoWithDataCallback);
        X3dCollision.setX3dCollisionDoWithDataCallback(myCollisionDoWithDataCallback);
        X3dBillboard.setX3dBillboardDoWithDataCallback(myBillboardDoWithDataCallback);
        X3dLOD.setX3dLODDoWithDataCallback(myLODDoWithDataCallback);
        X3dCADPart.setX3dCADPartDoWithDataCallback(myCADPartDoWithDataCallback);
        X3dCADAssembly.setX3dCADAssemblyDoWithDataCallback(myCADAssemblyDoWithDataCallback);
        X3dCADLayer.setX3dCADLayerDoWithDataCallback(myCADLayerDoWithDataCallback);
        X3dCADFace.setX3dCADFaceDoWithDataCallback(myCADFaceDoWithDataCallback);

        X3dTimeSensor.setX3dTimeSensorProcessEventCallback(myTimeSensorProcessEventCallback);
        X3dPositionInterpolator.setX3dPositionInterpolatorProcessEventCallback(myPositionInterpolatorProcessEventCallback);
        X3dOrientationInterpolator.setX3dOrientationInterpolatorProcessEventCallback(myOrientationInterpolatorProcessEventCallback);
        X3dTransform.setX3dTransformProcessEventCallback(myTransformProcessEventCallback);
        X3dColorInterpolator.setX3dColorInterpolatorProcessEventCallback(myColorInterpolatorProcessEventCallback);
        X3dScalarInterpolator.setX3dScalarInterpolatorProcessEventCallback(myScalarInterpolatorProcessEventCallback);
        X3dCoordinateInterpolator.setX3dCoordinateInterpolatorProcessEventCallback(myCoordinateInterpolatorProcessEventCallback);
        X3dNormalInterpolator.setX3dNormalInterpolatorProcessEventCallback(myNormalInterpolatorProcessEventCallback);
        X3dMaterial.setX3dMaterialProcessEventCallback(myMaterialProcessEventCallback);
        X3dTextureTransform.setX3dTextureTransformProcessEventCallback(myTextureTransformProcessEventCallback);
        X3dCoordinate.setX3dCoordinateProcessEventCallback(myCoordinateProcessEventCallback);
        X3dColor.setX3dColorProcessEventCallback(myColorProcessEventCallback);
        X3dNormal.setX3dNormalProcessEventCallback(myNormalProcessEventCallback);
        X3dTextureCoordinate.setX3dTextureCoordinateProcessEventCallback(myTextureCoordinateProcessEventCallback);
        X3dText.setX3dTextProcessEventCallback(myTextProcessEventCallback);
        X3dSwitch.setX3dSwitchProcessEventCallback(mySwitchProcessEventCallback);
    }

    protected void simpleInitGame(){
        sceneGraph = new X3dSceneGraph();
        setCallbacks();
// if code for -manyclasses
        X3dSceneGraph.root.m_data = new X3d_Data();
        ((X3d_Data)X3dSceneGraph.root.m_data).node = rootNode;
        X3dSceneGraph.doWithData();
// else
//        sceneGraph.root.m_data = new X3d_Data();
//        ((X3d_Data)sceneGraph.root.m_data).node = rootNode;
//        sceneGraph.doWithData();
// fi

        LightState lightState = DisplaySystem.getDisplaySystem().getRenderer().createLightState();

        lightState.detachAll();

        DirectionalLight light = new DirectionalLight();
        light.setDirection(new Vector3f(0.0f, 0.0f, -1.0f));
        light.setDiffuse(new ColorRGBA(1.0f, 1.0f, 1.0f, 1.0f));
        light.setAmbient(new ColorRGBA(0.1f, 0.1f, 0.1f, 1.0f));
        light.setConstant(100);
        light.setEnabled(true);
        lightState.attach(light);

        DirectionalLight light2 = new DirectionalLight();
        light2.setDirection(new Vector3f(0.0f, 0.0f, 1.0f));
        light2.setDiffuse(new ColorRGBA(1.0f, 1.0f, 1.0f, 1.0f));
        light2.setAmbient(new ColorRGBA(0.1f, 0.1f, 0.1f, 1.0f));
        light2.setConstant(100);
        light2.setEnabled(true);
        lightState.attach(light2);

        rootNode.setRenderState(lightState);
    }
};
