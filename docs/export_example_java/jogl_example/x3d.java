/* Copyright (c) Stefan Wolf, 2010. */
/* Copyright (c) J. "MUFTI" Scheurich, 2015,2016. */

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

/**
 * Based on Gears.java <BR>
 * author: Brian Paul (converted to Java by Ron Cemer and Sven Gothel) <P>
 */

import com.jogamp.opengl.GL2;
import com.jogamp.opengl.glu.GLU;
import com.jogamp.opengl.util.gl2.GLUT;
import com.jogamp.opengl.GLAutoDrawable;
import com.jogamp.opengl.GLEventListener;
import com.jogamp.opengl.GLProfile;
import com.jogamp.opengl.awt.GLCanvas;
import com.jogamp.opengl.util.*;
import com.jogamp.opengl.GLException;
import com.jogamp.opengl.util.texture.Texture;
import com.jogamp.opengl.util.texture.TextureIO;

import com.jogamp.common.nio.Buffers;

import com.jogamp.newt.Window;
import com.jogamp.newt.event.KeyAdapter;
import com.jogamp.newt.event.KeyEvent;
import com.jogamp.newt.event.KeyListener;
import com.jogamp.newt.event.MouseAdapter;
import com.jogamp.newt.event.MouseEvent;
import com.jogamp.newt.event.MouseListener;
import com.jogamp.newt.event.awt.AWTKeyAdapter;
import com.jogamp.newt.event.awt.AWTMouseAdapter;

import java.io.IOException;

import java.nio.FloatBuffer;
import java.nio.Buffer;
import java.nio.IntBuffer;
import java.nio.ByteBuffer;

import java.io.File;

// draws a X3DV file with shapes (converted to) IndexedFaceSet nodes

public class x3d implements GLEventListener 
{
    public static final float Z_NEAR = 0.05f;
    public static final float Z_FAR = 7000.0f;
   
    public static final float EPSILON = 1E-9f;
   
    public static boolean viewpointRendered = false;
    public static boolean viewPointExists = false;
    public static float viewpoint1Position[] = { 0, 0, 10 };

    public static boolean lightExists = false;
   
    public static boolean preRender = false;
    public static boolean initRender = false;
   
    public static X3dSceneGraph x3dSceneGraph = new X3dSceneGraph();

    public static float[] projectionMatrix = new float[16];
   
    public static X3dPointLight pointlight = null;

    private static int numLights = 0;

    private static float view_rotx = 0.0f, view_roty = 0.0f;
    private static final float view_rotz = 0.0f;
    private static float view_dist = -10.0f;

    private static int prevMouseX, prevMouseY;
    private static int mouseBotton;

    public static float navigation_matrix[] = new float[16];

    public static GLUT glut = new GLUT();
    public static GLU glu = new GLU();

    public static void main(String[] args) 
    {
        java.awt.Frame frame = new java.awt.Frame("X3D Demo");
        frame.setSize(500, 500);
        frame.setLayout(new java.awt.BorderLayout());

        GLCanvas canvas = new GLCanvas();
        // GLCapabilities caps = new GLCapabilities(GLProfile.getDefault());
        // GLCanvas canvas = new GLCanvas(caps);

        final x3d x3d = new x3d();
        canvas.addGLEventListener(x3d);

        frame.add(canvas, java.awt.BorderLayout.CENTER);
        frame.validate();

        frame.setVisible(true);

        Animator animator = new Animator(canvas);
        animator.start();
    }

    public static void error(String errormsg)
    {
        System.out.println(errormsg);
    }


    private static double getTimerTime() 
    {
        java.util.Date date = new java.util.Date();
        return date.getTime() / 1000.0;
    }
    
    class TimeSensorData 
    {
        public double startTime;
        public double stopTime;
    };
    
    @Override
    public void init(GLAutoDrawable drawable) 
    {
        // Use debug pipeline
        // drawable.setGL(new DebugGL(drawable.getGL()));

        X3dSceneGraph x3dSceneGraph = new X3dSceneGraph();

        x3d.gl = drawable.getGL().getGL2();

        x3d.gl.glEnable(GL2.GL_CULL_FACE);
        x3d.gl.glEnable(GL2.GL_LIGHTING);
        x3d.gl.glEnable(GL2.GL_LIGHT0);
        x3d.gl.glEnable(GL2.GL_DEPTH_TEST);

        x3d.gl.glEnable(GL2.GL_NORMALIZE);

        MouseListener x3dMouse = new x3dMouseAdapter();
        KeyListener x3dKeys = new x3dKeyAdapter();
    
        if (drawable instanceof Window) {
            Window window = (Window) drawable;
            window.addMouseListener(x3dMouse);
            window.addKeyListener(x3dKeys);
        } else if (GLProfile.isAWTAvailable() && drawable instanceof java.awt.Component) {
            java.awt.Component comp = (java.awt.Component) drawable;
            new AWTMouseAdapter(x3dMouse, drawable).addTo(comp);
            new AWTKeyAdapter(x3dKeys, drawable).addTo(comp);
        }
        initX3d();
    }

    @Override
    public void reshape(GLAutoDrawable drawable, int x, int y, int width, int height) 
    {
        GL2 gl = drawable.getGL().getGL2();

        float h = (float)height / (float)width;

        gl.glMatrixMode(GL2.GL_PROJECTION);

        gl.glLoadIdentity();
        gl.glFrustum(-1.0f, 1.0f, -h, h, 5.0f, 60.0f);
        gl.glMatrixMode(GL2.GL_MODELVIEW);
        gl.glLoadIdentity();
        gl.glTranslatef(0.0f, 0.0f, -20.0f);
    }

    @Override
    public void dispose(GLAutoDrawable drawable) 
    {
        System.err.println("dispose");
    }

    @Override
    public void display(GLAutoDrawable drawable) 
    {
        update();
        render(drawable);
    }

    private void update() 
    {
        x3d.x3dSceneGraph.X3dProcessEvents();
    }

    static GL2 gl;

    public static void render(GLAutoDrawable drawable) 
    {

        // Get the GL corresponding to the drawable we are animating
        x3d.gl = drawable.getGL().getGL2();

        x3d.gl.glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

        x3d.gl.glClear(GL2.GL_COLOR_BUFFER_BIT | GL2.GL_DEPTH_BUFFER_BIT);

        // Rotate the entire assembly of x3d window based on how the user
        // dragged the mouse around
        x3d.gl.glPushMatrix();
        x3d.gl.glMatrixMode(GL2.GL_MODELVIEW_MATRIX);
        x3d.gl.glLoadIdentity();
        x3d.gl.glTranslatef(0.0f, 0.0f, view_dist);
        x3d.gl.glRotatef(view_rotx, 1.0f, 0.0f, 0.0f);
        x3d.gl.glRotatef(view_roty, 0.0f, 1.0f, 0.0f);
        x3d.gl.glRotatef(view_rotz, 0.0f, 0.0f, 1.0f);
        x3d.gl.glGetFloatv(GL2.GL_MODELVIEW_MATRIX, navigation_matrix, 0);
        x3d.gl.glLoadIdentity();

        // draw scenegraph
        x3d2.drawX3d(navigation_matrix);

        x3d.gl.glPopMatrix();
    }

    class x3dKeyAdapter extends KeyAdapter 
    {
        @Override
    	public void keyPressed(KeyEvent e) {
            int kc = e.getKeyCode();
            if(KeyEvent.VK_LEFT == kc) {
                view_roty -= 1;
            } else if(KeyEvent.VK_RIGHT == kc) {
                view_roty += 1;
            } else if(KeyEvent.VK_UP == kc) {
                view_rotx -= 1;
            } else if(KeyEvent.VK_DOWN == kc) {
                view_rotx += 1;
            }
        }
    }
    
    class x3dMouseAdapter extends MouseAdapter 
    {
        @Override
    	public void mousePressed(MouseEvent e) {
            prevMouseX = e.getX();
            prevMouseY = e.getY();
            mouseBotton = e.getButton();
          }
    
        @Override
    	public void mouseReleased(MouseEvent e) {
          }
    
        @Override
        public void mouseDragged(MouseEvent e) {
            final int x = e.getX();
            final int y = e.getY();
            int width=0, height=0;
            Object source = e.getSource();
            if(source instanceof Window) {
                Window window = (Window) source;
                width=window.getSurfaceWidth();
                height=window.getSurfaceHeight();
            } else if(source instanceof GLAutoDrawable) {
            	GLAutoDrawable glad = (GLAutoDrawable) source;
                width=glad.getSurfaceWidth();
                height=glad.getSurfaceHeight();
            } else if (GLProfile.isAWTAvailable() && source instanceof java.awt.Component) {
                java.awt.Component comp = (java.awt.Component) source;
                width=comp.getWidth();
                height=comp.getHeight();
            } else {
                throw new RuntimeException("Event source neither Window nor Component: "+source);
            }
            float thetaY = 360.0f * ( (float)(x-prevMouseX)/(float)width);
            float thetaX = 360.0f * ( (float)(y-prevMouseY)/(float)height);
    
            if (mouseBotton == MouseEvent.BUTTON1) {
                view_rotx += thetaX;
                view_roty += thetaY;
            } else {
                view_dist += ( (float)(prevMouseY-y)/(float)height * 10.0f);
            }

            prevMouseX = x;
            prevMouseY = y;    
        }
    }

    static void initX3d()
    {    
        MyGroupRenderCallback myGroupRenderCallback = new MyGroupRenderCallback();
        X3dGroup.setX3dGroupRenderCallback(myGroupRenderCallback);

        MyTransformRenderCallback myTransformRenderCallback = new MyTransformRenderCallback();
        X3dTransform.setX3dTransformRenderCallback(myTransformRenderCallback);

        MyIndexedFaceSetRenderCallback myIndexedFaceSetRenderCallback = new MyIndexedFaceSetRenderCallback();
        X3dIndexedFaceSet.setX3dIndexedFaceSetRenderCallback(myIndexedFaceSetRenderCallback);

        MyMaterialRenderCallback myMaterialRenderCallback = new MyMaterialRenderCallback();
        X3dMaterial.setX3dMaterialRenderCallback(myMaterialRenderCallback);

        MyTextRenderCallback myTextRenderCallback = new MyTextRenderCallback();
        X3dText.setX3dTextRenderCallback(myTextRenderCallback);

        MyPointLightRenderCallback myPointLightRenderCallback = new MyPointLightRenderCallback();
        X3dPointLight.setX3dPointLightRenderCallback(myPointLightRenderCallback);

        MyViewpointRenderCallback myViewpointRenderCallback = new MyViewpointRenderCallback();
        X3dViewpoint.setX3dViewpointRenderCallback(myViewpointRenderCallback);

        MyPixelTextureRenderCallback myPixelTextureRenderCallback = new MyPixelTextureRenderCallback();
        X3dPixelTexture.setX3dPixelTextureRenderCallback(myPixelTextureRenderCallback);

        MyImageTextureRenderCallback myImageTextureRenderCallback = new MyImageTextureRenderCallback();
        X3dImageTexture.setX3dImageTextureRenderCallback(myImageTextureRenderCallback);

        MySwitchRenderCallback mySwitchRenderCallback = new MySwitchRenderCallback();
        X3dSwitch.setX3dSwitchRenderCallback(mySwitchRenderCallback);

        MyHAnimHumanoidRenderCallback myHAnimHumanoidRenderCallback = new MyHAnimHumanoidRenderCallback();
        X3dHAnimHumanoid.setX3dHAnimHumanoidRenderCallback(myHAnimHumanoidRenderCallback);

        MyHAnimJointRenderCallback myHAnimJointRenderCallback = new MyHAnimJointRenderCallback();
        X3dHAnimJoint.setX3dHAnimJointRenderCallback(myHAnimJointRenderCallback);


        MyTimeSensorProcessEventCallback myTimeSensorProcessEventCallback = new MyTimeSensorProcessEventCallback();
        X3dTimeSensor.setX3dTimeSensorProcessEventCallback(myTimeSensorProcessEventCallback);

        MyPositionInterpolatorProcessEventCallback myPositionInterpolatorProcessEventCallback = new MyPositionInterpolatorProcessEventCallback();
        X3dPositionInterpolator.setX3dPositionInterpolatorProcessEventCallback(myPositionInterpolatorProcessEventCallback);

        MyOrientationInterpolatorProcessEventCallback myOrientationInterpolatorProcessEventCallback = new MyOrientationInterpolatorProcessEventCallback();
        X3dOrientationInterpolator.setX3dOrientationInterpolatorProcessEventCallback(myOrientationInterpolatorProcessEventCallback);

        MyColorInterpolatorProcessEventCallback myColorInterpolatorProcessEventCallback = new MyColorInterpolatorProcessEventCallback();
        X3dColorInterpolator.setX3dColorInterpolatorProcessEventCallback(myColorInterpolatorProcessEventCallback);

        MyScalarInterpolatorProcessEventCallback myScalarInterpolatorProcessEventCallback = new MyScalarInterpolatorProcessEventCallback();
        X3dScalarInterpolator.setX3dScalarInterpolatorProcessEventCallback(myScalarInterpolatorProcessEventCallback);

        MyCoordinateInterpolatorProcessEventCallback myCoordinateInterpolatorProcessEventCallback = new MyCoordinateInterpolatorProcessEventCallback();
        X3dCoordinateInterpolator.setX3dCoordinateInterpolatorProcessEventCallback(myCoordinateInterpolatorProcessEventCallback);

        MyNormalInterpolatorProcessEventCallback myNormalInterpolatorProcessEventCallback = new MyNormalInterpolatorProcessEventCallback();
        X3dNormalInterpolator.setX3dNormalInterpolatorProcessEventCallback(myNormalInterpolatorProcessEventCallback);

        MyPositionInterpolator2DProcessEventCallback myPositionInterpolator2DProcessEventCallback = new MyPositionInterpolator2DProcessEventCallback();
        X3dPositionInterpolator2D.setX3dPositionInterpolator2DProcessEventCallback(myPositionInterpolator2DProcessEventCallback);

        x3d.gl.glColorMaterial(GL2.GL_FRONT_AND_BACK, GL2.GL_AMBIENT);
        x3d.gl.glColorMaterial(GL2.GL_FRONT_AND_BACK, GL2.GL_DIFFUSE);
        x3d.gl.glColorMaterial(GL2.GL_FRONT_AND_BACK, GL2.GL_EMISSION);
        x3d.gl.glColorMaterial(GL2.GL_FRONT_AND_BACK, GL2.GL_SPECULAR);
        x3d.gl.glColorMaterial(GL2.GL_FRONT_AND_BACK, GL2.GL_SHININESS);
    
        // Enable light
        x3d.gl.glEnable(GL2.GL_LIGHTING);
    
        int zero[] = {0, 0, 0, 0};
        x3d.gl.glLightModeliv(GL2.GL_LIGHT_MODEL_AMBIENT, zero, 0);
    
        // Use depth buffering for hidden surface elimination. 
        x3d.gl.glEnable(GL2.GL_DEPTH_TEST);
        x3d.gl.glEnable(GL2.GL_NORMALIZE);
    
        x3d.gl.glDepthMask(true);
        x3d.gl.glDepthFunc(GL2.GL_LEQUAL);
    
        x3d.gl.glEnable(GL2.GL_ALPHA_TEST);
        x3d.gl.glEnable(GL2.GL_BLEND);
        x3d.gl.glBlendFunc(GL2.GL_SRC_ALPHA, GL2.GL_ONE_MINUS_SRC_ALPHA);
        x3d.gl.glAlphaFunc(GL2.GL_NOTEQUAL, 0);
    
        x3d.gl.glShadeModel(GL2.GL_SMOOTH);
    
        x3d.preRender = false;
        x3d.initRender = true;
    
        X3dNode rootNode = x3d.x3dSceneGraph.root;
        rootNode.treeRender(null);

        if(!x3d.viewPointExists)
        {
            X3dViewpoint viewpoint = new X3dViewpoint();
            viewpoint.fieldOfView = 0.785398f;
            viewpoint.position = new float[3];
            viewpoint.position[0] = 0;
            viewpoint.position[1] = 0;
            viewpoint.position[2] = 10;
            viewpoint.orientation = new float[4];
            viewpoint.orientation[0] = 0;
            viewpoint.orientation[1] = 0;
            viewpoint.orientation[2] = 1;
            viewpoint.orientation[3] = 0;
            viewpoint.treeRender(null);
        }

        x3d.initRender = false;
    }

    static boolean not2PowN(int number)
    {
        boolean ret = true;
        for (int i = 0; i < 32; i++)
            if (number == (1 << i))
            {
                ret = false;
                break;
            }
        return ret;
    }    

    static void textureTransformBind(X3dNode data)
    {
        int i;
        X3dTextureTransform texTransform = (X3dTextureTransform)data;
        if(x3d.preRender)
        {
        }
        else if(x3d.initRender)
        {
        }
        else {
            float fcenter[] = texTransform.center;
            float fscale[] = texTransform.scale;
            float ftranslation[] = texTransform.translation;
    
            x3d.gl.glPushMatrix();
            x3d.gl.glMatrixMode(GL2.GL_TEXTURE);
            x3d.gl.glLoadIdentity();
            x3d.gl.glTranslatef(-fcenter[0], -fcenter[1], 0.0f);
            x3d.gl.glScalef(fscale[0], fscale[1], 1.0f);
            x3d.gl.glRotatef(texTransform.rotation * 180.0f / (float)Math.PI, 0.0f, 0.0f, 1.0f);
            x3d.gl.glTranslatef(fcenter[0], fcenter[1], 0.0f);
            x3d.gl.glTranslatef(ftranslation[0], ftranslation[1], 0.0f);
            x3d.gl.glMatrixMode(GL2.GL_MODELVIEW);
        }
    }
    
    static void textureTransformUnBind()
    {
        x3d.gl.glPopMatrix();
    }
    
    static void renderByteTexture(TextureExtraDataStruct extraData,
                                  boolean repeatS, boolean repeatT)
    {
        if (extraData.textureName == null) 
        {
            extraData.textureName = new int[1];
            x3d.gl.glGenTextures(1, extraData.textureName, 0);
            x3d.gl.glBindTexture(GL2.GL_TEXTURE_2D, extraData.textureName[0]); 
            x3d.gl.glPixelStorei(GL2.GL_UNPACK_ALIGNMENT, 1);
            x3d.gl.glPixelStorei(GL2.GL_PACK_ALIGNMENT, 1);
            x3d.gl.glTexImage2D(GL2.GL_TEXTURE_2D, 0, extraData.colorMode, 
                                extraData.width, extraData.height, 0, 
                                extraData.colorMode, GL2.GL_UNSIGNED_BYTE, 
                                extraData.pixels);
            x3d.gl.glTexParameterf(GL2.GL_TEXTURE_2D, GL2.GL_TEXTURE_MAG_FILTER,
                                   GL2.GL_NEAREST);
            x3d.gl.glTexParameterf(GL2.GL_TEXTURE_2D, GL2.GL_TEXTURE_MIN_FILTER,
                                   GL2.GL_NEAREST);
            x3d.gl.glEnable(GL2.GL_TEXTURE_2D);
        } else {
            x3d.gl.glBindTexture(GL2.GL_TEXTURE_2D, extraData.textureName[0]);
        }
        x3d.gl.glTexParameterf(GL2.GL_TEXTURE_2D, GL2.GL_TEXTURE_WRAP_S, 
                               repeatS ? GL2.GL_REPEAT : GL2.GL_CLAMP);
        x3d.gl.glTexParameterf(GL2.GL_TEXTURE_2D, GL2.GL_TEXTURE_WRAP_T, 
                               repeatT ? GL2.GL_REPEAT : GL2.GL_CLAMP);
        x3d.gl.glEnable(GL2.GL_TEXTURE_2D);
    }

    static int allocLight()
    {
        int maxLights[] = new int[1];
        x3d.gl.glGetIntegerv(x3d.gl.GL_MAX_LIGHTS, maxLights, 0);
        
        if (numLights >= maxLights[0]) {
            error("too many lights!\n");
            return x3d.gl.GL_LIGHT0;
        }
        
        return (x3d.gl.GL_LIGHT0 + numLights++);
    }
}        
 
class LightStruct
{
    public int intValue;
    public float floatValue;
    LightStruct(int intVal, float floatVal)
    {
        intValue = intVal;
        floatValue = floatVal;
    }
}

class x3d2
{   
    static X3dNode rootNode = x3d.x3dSceneGraph.root;
    //static X3dNode rootNode = x3d.x3dSceneGraph.DEFNAME;
    
    static void drawX3d(float matrix[])
    {
        x3d.gl.glMatrixMode(GL2.GL_MODELVIEW);

        x3d.gl.glLoadMatrixf(matrix, 0);    

        x3d.viewpointRendered = false;
        
        x3d.preRender = true;
    
        rootNode.treeRender(null);

        if(!x3d.lightExists)
        {
            int lightNum = x3d.allocLight();
            x3d.gl.glEnable(lightNum);
            x3d.lightExists = true;
        }

        x3d.gl.glMatrixMode(GL2.GL_MODELVIEW);
        x3d.preRender = false;
        rootNode.treeRender(null);
    }
}

class MyGroupRenderCallback extends X3dGroupRenderCallback
{
    public void render(X3dNode data)
    {
        X3dGroup group = (X3dGroup)data;
        if (group == null)
            return;
        x3d.gl.glPushMatrix();
        if (group.children != null)
            for (int i = 0; i < group.children.length; i++)
                if (group.children[i] != null) 
                {
                    if(!(x3d.preRender || x3d.initRender))
                        x3d.gl.glDisable(GL2.GL_TEXTURE_2D);
                    group.children[i].treeRender(null);
                }
        x3d.gl.glPopMatrix();
    }
}

class MyIndexedFaceSetRenderCallback extends X3dIndexedFaceSetRenderCallback
{
    public void render(X3dNode data)
    {
        if(x3d.preRender)
        {
        }
        else if(x3d.initRender)
        {
        }
        else
        {
            X3dIndexedFaceSet Xindexedfaceset = (X3dIndexedFaceSet)data;
            X3dCoordinate Xcoordinate = (X3dCoordinate)Xindexedfaceset.coord;
            if (Xcoordinate == null)
                return;
            X3dNormal Xnormal = (X3dNormal)Xindexedfaceset.normal;
            X3dColor Xcolor = (X3dColor)Xindexedfaceset.color;
            X3dTextureCoordinate Xtexturecoordinate = (X3dTextureCoordinate)Xindexedfaceset.texCoord;
            int faces[] = null;
            float vertex[] = null;
            int normalindex[] = null;
            float normal[] = null;
            float colors[] = null;
            int colorindex[] = null;
            int texturecoordinateindex[] = null;
            int faces_len, vertex_len, normalindex_len, normal_len, color_len, colorindex_len, texturecoordinateindex_len;
            boolean normalpervertex = true; 
            boolean colorpervertex = true;
            if(Xcolor != null)
            {
                x3d.gl.glEnable(GL2.GL_COLOR_MATERIAL); //Maybe needfull
                x3d.gl.glColorMaterial(GL2.GL_FRONT_AND_BACK, GL2.GL_DIFFUSE);
                colors = Xcolor.color;
                color_len = Xcolor.color.length;
                colorpervertex = Xindexedfaceset.colorPerVertex;
            }
            if(Xindexedfaceset.colorIndex != null)
            {
                colorindex = Xindexedfaceset.colorIndex;
                colorindex_len = Xindexedfaceset.colorIndex.length;
            }
            if(Xindexedfaceset.texCoordIndex != null)
            {
                texturecoordinateindex = Xindexedfaceset.texCoordIndex;
                texturecoordinateindex_len = Xindexedfaceset.texCoordIndex.length;
            }
            vertex = Xcoordinate.point;
            faces = Xindexedfaceset.coordIndex;
            normalindex = Xindexedfaceset.normalIndex;
            if(Xnormal!=null)normal = Xnormal.vector;
            vertex_len = Xcoordinate.point.length;
            if (Xindexedfaceset.coordIndex == null)
                return;
            faces_len = Xindexedfaceset.coordIndex.length;
            if(Xnormal != null)normal_len = Xnormal.vector.length;
            if (Xindexedfaceset.normalIndex != null)
                normalindex_len = Xindexedfaceset.normalIndex.length;
            else
                normalindex_len = 0;
            normalpervertex = Xindexedfaceset.normalPerVertex;
            int buffer = 0;
            int normalbuffer = 0;
            int texturebuffer = 0;
            int facecounter = 0;
            if (Xindexedfaceset.ccw)
            {
               x3d.gl.glFrontFace(GL2.GL_CCW);
            }
            else
            {
               x3d.gl.glFrontFace(GL2.GL_CW);
            }
            if (Xindexedfaceset.solid == false) 
            {
               x3d.gl.glDisable(GL2.GL_CULL_FACE);
               x3d.gl.glLightModeli(GL2.GL_LIGHT_MODEL_TWO_SIDE, GL2.GL_TRUE);
            } 
            else 
            {
               x3d.gl.glEnable(GL2.GL_CULL_FACE);
               x3d.gl.glLightModeli(GL2.GL_LIGHT_MODEL_TWO_SIDE, GL2.GL_FALSE);
            }
    
            x3d.gl.glBegin(GL2.GL_POLYGON);
            if((colors != null) && !colorpervertex)
                x3d.gl.glColor3f(colors[3 * facecounter], colors[3 * facecounter + 1], colors[3 * facecounter + 2]);
    
            for(int i = 0; i != faces_len; i++)
            {
                buffer = faces[i];
                if (buffer < 0) 
                {
                    x3d.gl.glEnd();
                    if (i != faces_len - 1)
                        x3d.gl.glBegin(GL2.GL_POLYGON);
                }
                if(normalindex != null)
                    normalbuffer = normalindex[i];
                else 
                    normalbuffer = buffer;
                if(texturecoordinateindex != null)
                    texturebuffer = texturecoordinateindex[i];
                else
                    texturebuffer = buffer;
                if(buffer == -1 && (normalbuffer == -1 || normalindex != null))
                {
                    facecounter++;
                    x3d.gl.glEnd();
                    x3d.gl.glBegin(GL2.GL_POLYGON);
                    if((colors != null) && !colorpervertex)
                        x3d.gl.glColor3f(colors[3 * facecounter], colors[3 * facecounter + 1], colors[3 * facecounter + 2]);
                }
                else if(((buffer == -1 && normalbuffer != -1) || (buffer != -1 && normalbuffer == -1)) && normalindex != null)
                    x3d.error("Error in normalIndex");
                else
                {
                    if(normalpervertex && normal != null)x3d.gl.glNormal3f(normal[normalbuffer*3], normal[normalbuffer*3+1], normal[normalbuffer*3+2]);
                    if((colors != null) && colorpervertex)x3d.gl.glColor3f(colors[buffer*3], colors[buffer*3+1], colors[buffer*3+2]);
                    if(Xtexturecoordinate != null)
                        if((Xtexturecoordinate.point != null) && (texturecoordinateindex != null))
                            x3d.gl.glTexCoord2f(Xtexturecoordinate.point[texturebuffer*2], Xtexturecoordinate.point[texturebuffer*2+1]);
                    x3d.gl.glVertex3f(vertex[buffer*3], vertex[buffer*3+1], vertex[buffer*3+2]);
                }
            }
            x3d.gl.glEnd();
        }
    }
    x3d.gl.glDisable(GL2.GL_COLOR_MATERIAL); //Maybe needfull
}

class MyMaterialRenderCallback extends X3dMaterialRenderCallback
{
    public void render(X3dNode data)
    {
        if(x3d.preRender)
        {
        }
        else if(x3d.initRender)
        {
        }
        else
        {
            X3dMaterial material = (X3dMaterial)data;
            float diffuse_color[] = {material.diffuseColor[0], material.diffuseColor[1], material.diffuseColor[2], 1.0f};
            float ambient = material.ambientIntensity;
            float ambient_color[] = {ambient * material.diffuseColor[0], ambient * material.diffuseColor[1], ambient * material.diffuseColor[2], 1};
            float emissive_color[] = {material.emissiveColor[0], material.emissiveColor[1], material.emissiveColor[2], 1.0f};
            float specuar_color[] = {material.specularColor[0], material.specularColor[1], material.specularColor[2], 1.0f};
            float shininess = material.shininess*128.0f;
            x3d.gl.glMaterialfv(GL2.GL_FRONT_AND_BACK, GL2.GL_DIFFUSE, diffuse_color, 0);
            x3d.gl.glMaterialfv(GL2.GL_FRONT_AND_BACK, GL2.GL_AMBIENT, ambient_color, 0);
            x3d.gl.glMaterialfv(GL2.GL_FRONT_AND_BACK, GL2.GL_EMISSION, emissive_color, 0);
            x3d.gl.glMaterialfv(GL2.GL_FRONT_AND_BACK, GL2.GL_SPECULAR, specuar_color, 0);
            x3d.gl.glMaterialf(GL2.GL_FRONT_AND_BACK, GL2.GL_SHININESS, shininess);
        }
    }
}    

class MyTextRenderCallback extends X3dTextRenderCallback
{
    public void render(X3dNode data)
    {
        if(x3d.preRender)
        {
        }
        else if(x3d.initRender)
        {
        }
        else
        {
            X3dText text = (X3dText)data;
            x3d.gl.glPushAttrib(GL2.GL_ENABLE_BIT);
        
            x3d.gl.glDisable(GL2.GL_LIGHTING);
            x3d.gl.glDisable(GL2.GL_BLEND);
            
            x3d.gl.glEnable(GL2.GL_LINE_SMOOTH);
        
            float fsize = 1;
            X3dFontStyle fontStyle = (X3dFontStyle)text.fontStyle;
            if (fontStyle != null)
               fsize = fontStyle.size;
        
            for (int j = 0; j < text.string.length;j++) 
            {
                String str = text.string[j];
                x3d.gl.glPushMatrix();
                x3d.gl.glTranslatef(0, -j * fsize, 0);
                final float GLUT_STROKE_ROMAN_SIZE = 119.05f;
                float scale = 1/GLUT_STROKE_ROMAN_SIZE;
                x3d.gl.glScalef(scale * fsize, scale * fsize, 1.0f);
                for (int i = 0; i < str.length(); i++)
                    x3d.glut.glutStrokeCharacter(GLUT.STROKE_ROMAN, str.charAt(i));
                x3d.gl.glPopMatrix();
            }
            x3d.gl.glPopAttrib();
        }
    }
}

class TextureExtraDataStruct {
    public int height;
    public int width;
    public int colorMode;
    public int format;
    public Buffer pixels;
    public int textureName[];
};

class MyImageTextureRenderCallback extends X3dImageTextureRenderCallback
{
    public void render(X3dNode data)
    {
        X3dImageTexture imageTexture = (X3dImageTexture)data;

        Texture extraVar = null;
    
        if(x3d.preRender)
        {
        }
        else if(x3d.initRender)
        {
            String filename;
    
            filename = imageTexture.url[0];
    
            if (filename.length() == 0)
                return;
            
            Texture texture = null;
            File file = null;
            try 
            {            
                file = new File(filename);
            } 
            catch (NullPointerException e)
            {
                System.err.println("Caught NullPointerException: " + e.getMessage());  
            }
            try 
            {
                texture = TextureIO.newTexture(file, false);    
            }
            catch (IOException e)
            {
                System.err.println("Caught IOException: " + e.getMessage());  
            }
            catch (GLException e)
            {
                System.err.println("Caught GLException: " + e.getMessage());  
            }

            imageTexture.extra_data = texture;
        } 
        else if (imageTexture.extra_data != null)
        {
            extraVar = (Texture)imageTexture.extra_data;

            X3dNode textureTransform;
            textureTransform = ((X3dAppearance)imageTexture.m_parent).
                               textureTransform;
    
            if (textureTransform != null)
                x3d.textureTransformBind(textureTransform);
            x3d.gl.glMatrixMode(GL2.GL_TEXTURE);
            extraVar.bind(x3d.gl);
            x3d.gl.glTexParameterf(GL2.GL_TEXTURE_2D, GL2.GL_TEXTURE_WRAP_S, 
                                   imageTexture.repeatS ? 
                                   GL2.GL_REPEAT : GL2.GL_CLAMP);
            x3d.gl.glTexParameterf(GL2.GL_TEXTURE_2D, GL2.GL_TEXTURE_WRAP_T, 
                                   imageTexture.repeatT ? 
                                   GL2.GL_REPEAT : GL2.GL_CLAMP);
            x3d.gl.glEnable(GL2.GL_TEXTURE_2D);
            if (textureTransform != null)
                x3d.textureTransformUnBind();
            x3d.gl.glMatrixMode(GL2.GL_MODELVIEW);
        }
    }
}
 
class MyPixelTextureRenderCallback extends X3dPixelTextureRenderCallback
{      
    public void render(X3dNode data)
    {
        X3dPixelTexture pixelTexture = (X3dPixelTexture) data;

        TextureExtraDataStruct extraVar = null;
        extraVar = (TextureExtraDataStruct)pixelTexture.extra_data;

        if(x3d.preRender)
        {
        }
        else if(x3d.initRender)
        {
            int bigEndian = 1;

            extraVar = new TextureExtraDataStruct();    
            pixelTexture.extra_data = extraVar;

            extraVar.textureName = null;
            extraVar.width = pixelTexture.image[0];
            extraVar.height = pixelTexture.image[1];
            extraVar.colorMode = GL2.GL_RGBA;
            if (x3d.not2PowN(extraVar.width))
                x3d.error("warning: texture width is not 2 pow N");
            if (x3d.not2PowN(extraVar.height))
                x3d.error("warning: texture height is not 2 pow N");
            if (extraVar.width != extraVar.height)
                x3d.error("warning: texture width and height is not equal\n");
            // store modified colors back to pixelTexture.image
            byte pixels[] = new byte[extraVar.width * extraVar.height * 4];
            for (int i = 0; i < pixelTexture.image.length - 3; i++) {
                byte a = (byte)0xff;
                byte r = 0,g = 0,b = 0;
                switch (pixelTexture.image[2])
                {
                  case 4:
                    r = (byte)((pixelTexture.image[i + 3]&0xFF000000) >> 24);
                    g = (byte)((pixelTexture.image[i + 3]&0x00FF0000) >> 16);
                    b = (byte)((pixelTexture.image[i + 3]&0x0000FF00) >> 8);
                    a = (byte)((pixelTexture.image[i + 3]&0x000000FF));
                    break;
                  case 3:
                    r = (byte)((pixelTexture.image[i + 3]&0x00FF0000) >> 16);
                    g = (byte)((pixelTexture.image[i + 3]&0x0000FF00) >> 8);
                    b = (byte)((pixelTexture.image[i + 3]&0x000000FF));
                    break;
                  case 2:
                    r = g = b = (byte)((pixelTexture.image[i + 3]&0x0000FF00) >> 8);
                    a = (byte)((pixelTexture.image[i + 3]&0x000000FF));
                    break;
                  case 1:
                    r = g = b = (byte)((pixelTexture.image[i + 3]&0xFF));
                    break;
                }
                if (bigEndian != 0)
                {
                    pixels[i * 4 + 0] = r;
                    pixels[i * 4 + 1] = g;
                    pixels[i * 4 + 2] = b;
                    pixels[i * 4 + 3] = a;
                }
                else
                {
                    pixels[i * 4 + 0] = a;
                    pixels[i * 4 + 1] = b;
                    pixels[i * 4 + 2] = g;
                    pixels[i * 4 + 3] = r;
                }
            }
            extraVar.pixels = ByteBuffer.wrap(pixels);
        }
        else {
            X3dNode textureTransform;
            textureTransform = ((X3dAppearance)pixelTexture.m_parent).
                               textureTransform;
                    
            if (textureTransform != null)
                x3d.textureTransformBind(textureTransform);
            x3d.renderByteTexture(extraVar,
                                  pixelTexture.repeatS, pixelTexture.repeatT); 
            if (textureTransform != null)
                x3d.textureTransformUnBind();
        }
    }
}


class MyPointLightRenderCallback extends X3dPointLightRenderCallback
{       
    public void render(X3dNode data)
    {
        LightStruct lightStruct = (LightStruct)((X3dPointLight)data).extra_data;
        if(x3d.preRender)
        {
            int lightNum = lightStruct.intValue;
            x3d.gl.glEnable(lightNum);
            X3dPointLight light = (X3dPointLight)data;
            float light_color[] = {light.color[0]*light.intensity, light.color[1]*light.intensity, light.color[2]*light.intensity, 1};
            float light_ambient_color[] = {light.color[0]*light.ambientIntensity, light.color[1]*light.ambientIntensity, light.color[2]*light.ambientIntensity, 1};
            float light_position[] = {light.location[0], light.location[1], light.location[2], 
                                     lightStruct.floatValue};
            float light_attenuation[] = {light.attenuation[0], light.attenuation[1], light.attenuation[2]};
            x3d.gl.glLightfv(lightNum, GL2.GL_DIFFUSE, light_color, 0);
            x3d.gl.glLightfv(lightNum, GL2.GL_AMBIENT, light_ambient_color, 0);
            x3d.gl.glLightfv(lightNum, GL2.GL_POSITION, light_position, 0);
            x3d.gl.glLightf(lightNum, GL2.GL_CONSTANT_ATTENUATION, light_attenuation[0]);
            x3d.gl.glLightf(lightNum, GL2.GL_LINEAR_ATTENUATION, light_attenuation[1]);
            x3d.gl.glLightf(lightNum, GL2.GL_QUADRATIC_ATTENUATION, light_attenuation[2]);
            x3d.gl.glLightf(lightNum, GL2.GL_SPOT_CUTOFF, 180.0f);
            x3d.gl.glLightf(lightNum, GL2.GL_SPOT_EXPONENT, 0.0f);
        }
        else if(x3d.initRender)
        {
            x3d.lightExists = true;
            ((X3dPointLight)data).extra_data = new LightStruct(x3d.allocLight(), lightStruct == null ? 1.0f : 0.0f);
        }
        else
        {
        }
    }
}

class MyDirectionalLightRenderCallback extends X3dDirectionalLightRenderCallback
{       
    public void render(X3dNode data)
    {
        LightStruct lightStruct = (LightStruct)
                                  ((X3dDirectionalLight)data).extra_data;
        if(x3d.preRender)
        {
            int lightNum = lightStruct.intValue;
            x3d.gl.glEnable(lightNum);
            X3dDirectionalLight light = (X3dDirectionalLight)data;

            float color[] = new float[4];
            float ambient_color[] = new float[4];
            float position[] = new float[4]; 

            for (int i = 0; i < 3; i++) 
                color[i] = light.color[i] * light.intensity;
            color[3] = 1;
    
            for (int i = 0; i < 3; i++) 
                ambient_color[i] = light.color[i] * light.ambientIntensity;
            ambient_color[3] = 1;
    
            for (int i = 0; i < 3; i++)
                position[i] = -light.direction[i];
            position[3] = lightStruct.floatValue;
    
            x3d.gl.glLightfv(lightNum, GL2.GL_DIFFUSE, color, 0);
            x3d.gl.glLightfv(lightNum, GL2.GL_AMBIENT, ambient_color, 0);
            x3d.gl.glLightfv(lightNum, GL2.GL_POSITION, position, 0);
            x3d.gl.glLightf(lightNum, GL2.GL_CONSTANT_ATTENUATION, 1);
            x3d.gl.glLightf(lightNum, GL2.GL_LINEAR_ATTENUATION, 0);
            x3d.gl.glLightf(lightNum, GL2.GL_QUADRATIC_ATTENUATION, 0);
            x3d.gl.glLightf(lightNum, GL2.GL_SPOT_CUTOFF, 180.0f);
            x3d.gl.glLightf(lightNum, GL2.GL_SPOT_EXPONENT, 0.0f);
        }
        else if(x3d.initRender)
        {
            x3d.lightExists = true;
            ((X3dDirectionalLight)data).extra_data = new LightStruct(x3d.allocLight(), lightStruct == null ? 1.0f : 0.0f);
        }
        else
        {
        }
    }
}

class MySpotLightRenderCallback extends X3dSpotLightRenderCallback
{       
    public void render(X3dNode data)
    {
        LightStruct lightStruct = (LightStruct)((X3dSpotLight)data).extra_data;
        if(x3d.preRender)
        {
            int lightNum = lightStruct.intValue;
            x3d.gl.glEnable(lightNum);
            X3dSpotLight light = (X3dSpotLight)data;
            float color[] = new float[4];
            float ambient_color[] = new float[4];
            float position[] = new float[4]; 
            float attenuation[] = {light.attenuation[0], light.attenuation[1], light.attenuation[2]};

            for (int i = 0; i < 3; i++) 
                color[i] = light.color[i] * light.intensity;
            color[3] = 1;
    
            for (int i = 0; i < 3; i++) 
                ambient_color[i] = light.color[i] * light.ambientIntensity;
            ambient_color[3] = 1;
    
            for (int i = 0; i < 3; i++)
                position[i] = -light.direction[i];
            position[3] = lightStruct.floatValue;
    
            x3d.gl.glLightfv(lightNum, GL2.GL_AMBIENT, ambient_color, 0);
            x3d.gl.glLightfv(lightNum, GL2.GL_DIFFUSE, color, 0);
            x3d.gl.glLightfv(lightNum, GL2.GL_POSITION, position, 0);
            x3d.gl.glLightfv(lightNum, GL2.GL_SPECULAR, color, 0);
            x3d.gl.glLightfv(lightNum, GL2.GL_SPOT_DIRECTION, light.direction, 0);
            x3d.gl.glLightf(lightNum, GL2.GL_SPOT_CUTOFF, light.cutOffAngle * 180.0f / (float)Math.PI);
            x3d.gl.glLightf(lightNum, GL2.GL_SPOT_EXPONENT, light.beamWidth < light.cutOffAngle ? 1.0f : 0.0f);
            x3d.gl.glLightf(lightNum, GL2.GL_CONSTANT_ATTENUATION, light.attenuation[0]);
            x3d.gl.glLightf(lightNum, GL2.GL_LINEAR_ATTENUATION, light.attenuation[1]);
            x3d.gl.glLightf(lightNum, GL2.GL_QUADRATIC_ATTENUATION, light.attenuation[2]);
        }
        else if(x3d.initRender)
        {
            x3d.lightExists = true;
            ((X3dSpotLight)data).extra_data = new LightStruct(x3d.allocLight(), lightStruct == null ? 1.0f : 0.0f);
        }
        else
        {
        }
    }
}

    
class MyTransformRenderCallback extends X3dTransformRenderCallback
{
    public void render(X3dNode data)
    {
        X3dTransform transform = (X3dTransform)data;
        if(x3d.preRender || x3d.initRender)
        {
            if (transform.children != null)
                for (int i = 0; i < transform.children.length; i++)
                    if (transform.children[i] != null)
                        transform.children[i].treeRender(null);
        }
        else
        {
            x3d.gl.glPushMatrix();
            x3d.gl.glTranslatef(transform.translation[0], transform.translation[1], transform.translation[2]);
            x3d.gl.glTranslatef(transform.center[0], transform.center[1], transform.center[2]);
            x3d.gl.glRotatef( ((transform.rotation[3] / (float)Math.PI) * 180.0f), transform.rotation[0], transform.rotation[1], transform.rotation[2]);
            x3d.gl.glRotatef( ((transform.scaleOrientation[3] / (float)Math.PI) * 180.0f), transform.scaleOrientation[0], transform.scaleOrientation[1], transform.scaleOrientation[2]);
            x3d.gl.glScalef(transform.scale[0], transform.scale[1], transform.scale[2]);
            x3d.gl.glRotatef( ((transform.scaleOrientation[3] / (float)Math.PI) * 180.0f) * -1.0f, transform.scaleOrientation[0], transform.scaleOrientation[1], transform.scaleOrientation[2]);
            x3d.gl.glTranslatef(transform.center[0] * -1.0f, transform.center[1] * -1.0f, transform.center[2] * -1.0f);
            if (transform.children != null)
                for (int i = 0; i < transform.children.length; i++)
                    if (transform.children[i] != null) 
                    {
                        x3d.gl.glDisable(GL2.GL_TEXTURE_2D);
                        transform.children[i].treeRender(null);
                    }
            x3d.gl.glPopMatrix();
        }
    }
}    


class MyViewpointRenderCallback extends X3dViewpointRenderCallback
{
    public void render(X3dNode data)
    {
        X3dViewpoint viewpoint = (X3dViewpoint)data;
        if(x3d.preRender || x3d.initRender)
        {
            x3d.viewPointExists = true;
    
            if(!x3d.viewpointRendered)
            {
                x3d.viewpointRendered = true;
                for (int i = 0; i < 3; i++)
                    x3d.viewpoint1Position[i] = viewpoint.position[i];
    
                x3d.gl.glMatrixMode(GL2.GL_PROJECTION);
                float fieldOfViewdegree = ((viewpoint.fieldOfView / (float)Math.PI) * 180.0f);
    
                if(x3d.initRender) {
                    x3d.glu.gluPerspective(fieldOfViewdegree, 1.0, x3d.Z_NEAR, x3d.Z_FAR);  /* fieldOfView in degree, aspect radio, Z nearest, Z farest */
                    x3d.gl.glGetFloatv(GL2.GL_PROJECTION_MATRIX, x3d.projectionMatrix, 0);
                }
                x3d.gl.glMatrixMode(GL2.GL_MODELVIEW);
                x3d.gl.glLoadMatrixf(x3d.projectionMatrix, 0);
    
                x3d.gl.glRotatef( ( -(viewpoint.orientation[3] / (2*(float)Math.PI) ) * 360), viewpoint.orientation[0], viewpoint.orientation[1], viewpoint.orientation[2]);
                x3d.gl.glTranslatef(-viewpoint.position[0], -viewpoint.position[1], -viewpoint.position[2]);
                x3d.gl.glClear(GL2.GL_COLOR_BUFFER_BIT | GL2.GL_DEPTH_BUFFER_BIT);
            }
        }
        else
        {
        }
    }
}    

class MySwitchRenderCallback extends X3dSwitchRenderCallback
{
    public void render(X3dNode data)
    {
        X3dSwitch switchNode = (X3dSwitch)data;
        if (switchNode == null)
            return;
        x3d.gl.glPushMatrix();
        int choice = switchNode.whichChoice;
        if (switchNode.children != null)
            if (choice < switchNode.children.length)
                if (switchNode.children[choice] != null) 
                {
                    if(!(x3d.preRender || x3d.initRender))
                        x3d.gl.glDisable(GL2.GL_TEXTURE_2D);
                    switchNode.children[choice].treeRender(null);
                }
        x3d.gl.glPopMatrix();
    }
}


class HAnimJointExtraDataStruct {
    public float jointMatrix[];
    public float thisMatrix[];
};

class HAnimHumanoidExtraDataStruct {
     public float origVertices[];
     public int origVertices_length;
     public float thisMatrix[];
};

class HAnimJointExtra {
    public static void multMatrixVec(float ret[], float mat[], 
                                     float x, float y, float z)
    {
        float s = mat[4*0+3]*x + mat[4*1+3]*y + mat[4*2+3]*z + mat[4*3+3];
        if (s == 0)
            s = 1;
    
        ret[0] = (mat[4*0+0]*x + mat[4*1+0]*y + mat[4*2+0]*z + mat[4*3+0])/s;
        ret[1] = (mat[4*0+1]*x + mat[4*1+1]*y + mat[4*2+1]*z + mat[4*3+1])/s;
        ret[2] = (mat[4*0+2]*x + mat[4*1+2]*y + mat[4*2+2]*z + mat[4*3+2])/s;
    }

    public static void applyJoint(X3dNode data, X3dNode x3dcoord, 
                                  float origVertices[], X3dNode parent)
    {
         X3dHAnimJoint joint = (X3dHAnimJoint)data;
         X3dCoordinate coord = (X3dCoordinate)x3dcoord;
         int i;
         HAnimJointExtraDataStruct extraVar;
         float mayMatrix[];
    
         extraVar = (HAnimJointExtraDataStruct)(joint.extra_data);
         if (extraVar == null)
             return;
         if (parent.getType() == X3dHAnimJointType.type) 
         {
             X3dHAnimJoint mayJoint = (X3dHAnimJoint)parent;
             HAnimJointExtraDataStruct jointData =
                 (HAnimJointExtraDataStruct)(mayJoint.extra_data);
             mayMatrix = jointData.jointMatrix;
         } else {
             X3dHAnimHumanoid mayHuman = (X3dHAnimHumanoid)parent;
             HAnimHumanoidExtraDataStruct humanData =
                 (HAnimHumanoidExtraDataStruct)(mayHuman.extra_data);
             mayMatrix = humanData.thisMatrix;
         }
         x3d.gl.glMatrixMode(GL2.GL_MODELVIEW);
         x3d.gl.glPushMatrix();
         x3d.gl.glLoadIdentity();
         x3d.gl.glMultMatrixf(mayMatrix, 0);
         x3d.gl.glMultMatrixf(extraVar.thisMatrix, 0);
         x3d.gl.glGetFloatv(GL2.GL_MODELVIEW_MATRIX, extraVar.jointMatrix, 0);
         x3d.gl.glPopMatrix();
    
         if ((coord != null) && (coord.point != null))
         {
             int mskinCoordIndex[] = joint.skinCoordIndex;
             float mskinCoordWeight[] = joint.skinCoordWeight;
             for (i = 0; i < joint.skinCoordIndex.length; i++)
                 if (mskinCoordIndex[i] > -1) {
                     int index = mskinCoordIndex[i];
                     float point[] = origVertices;
                     float wpoint[] = new float[3];
                     float newpoint[] = new float[3];
                     int sCwI = joint.skinCoordWeight.length - 1;
                     float weight = mskinCoordWeight[i < sCwI ? i : sCwI];
                     newpoint[0] = coord.point[index * 3];
                     newpoint[1] = coord.point[index * 3 + 1];
                     newpoint[2] = coord.point[index * 3 + 2];
                     multMatrixVec(wpoint, extraVar.jointMatrix, 
                                   point[index * 3 + 0], 
                                   point[index * 3 + 1],
                                   point[index * 3 + 2]);
                     wpoint[0] = wpoint[0] * weight; 
                     wpoint[1] = wpoint[1] * weight; 
                     wpoint[2] = wpoint[2] * weight; 
                     newpoint[0] += wpoint[0];
                     newpoint[1] += wpoint[1];
                     newpoint[2] += wpoint[2];
                     coord.point[index * 3 + 0] = newpoint[0];
                     coord.point[index * 3 + 1] = newpoint[1];
                     coord.point[index * 3 + 2] = newpoint[2];
                 }
        }
        if (joint.children != null)
            for (i = 0; i < joint.children.length; i++)
                if (joint.children[i] != null)
                {
                    if (joint.children[i].getType() == X3dHAnimJointType.type)
                        if ((coord != null) && (coord.point != null))
                            applyJoint(joint.children[i], x3dcoord, 
                                       origVertices, data);
                }
     }
}

class MyHAnimHumanoidRenderCallback extends X3dHAnimHumanoidRenderCallback
{
    public void render(X3dNode data)
    {
        X3dHAnimHumanoid humanoid = (X3dHAnimHumanoid)data;
        int i;
        HAnimHumanoidExtraDataStruct extraVar;
        X3dCoordinate coord = (X3dCoordinate)humanoid.skinCoord;
        if(x3d.initRender)
        {
            humanoid.extra_data = new HAnimHumanoidExtraDataStruct();
            ((HAnimHumanoidExtraDataStruct)humanoid.extra_data).thisMatrix = 
                new float[16];
        }
        extraVar = (HAnimHumanoidExtraDataStruct)humanoid.extra_data;
        if (extraVar == null)
            return;
        if(x3d.initRender) {
            x3d.gl.glMatrixMode(GL2.GL_MODELVIEW);
            x3d.gl.glPushMatrix();
            x3d.gl.glLoadIdentity();
            x3d.gl.glGetFloatv(GL2.GL_MODELVIEW_MATRIX, extraVar.thisMatrix, 0);
            x3d.gl.glPopMatrix();
    
            if ((coord != null) && (coord.point != null)) 
            {
                extraVar.origVertices = new float[coord.point.length];
                for (i = 0; i < coord.point.length; i++)
                    extraVar.origVertices[i] = coord.point[i];
            }
        }
        if(x3d.initRender)
        {
            if (humanoid.skeleton != null)
                for (i = 0; i < humanoid.skeleton.length; i++)
                    if (humanoid.skeleton[i] != null) 
                    {
                        humanoid.skeleton[i].treeRender(data);;
                    }       
            if (humanoid.skin != null)
                for (i = 0; i < humanoid.skin.length; i++)
                    if (humanoid.skin[i] != null)
                        humanoid.skin[i].treeRender(data);
        }
        else
        {   
            if (x3d.preRender)
            {
                x3d.gl.glPushMatrix();
                x3d.gl.glMatrixMode(GL2.GL_MODELVIEW);
                x3d.gl.glLoadIdentity();
                x3d.gl.glTranslatef(humanoid.translation[0], humanoid.translation[1], humanoid.translation[2]);
                x3d.gl.glTranslatef(humanoid.center[0], humanoid.center[1], humanoid.center[2]);
                x3d.gl.glRotatef( ( (humanoid.rotation[3] / (2*(float)Math.PI) ) * 360), humanoid.rotation[0], humanoid.rotation[1], humanoid.rotation[2]);
                x3d.gl.glRotatef( ( (humanoid.scaleOrientation[3] / (2*(float)Math.PI) ) * 360), humanoid.scaleOrientation[0], humanoid.scaleOrientation[1], humanoid.scaleOrientation[2]);
                x3d.gl.glScalef(humanoid.scale[0], humanoid.scale[1], humanoid.scale[2]);
                x3d.gl.glRotatef( ( (humanoid.scaleOrientation[3] / (2*(float)Math.PI) ) * 360) * -1, humanoid.scaleOrientation[0], humanoid.scaleOrientation[1], humanoid.scaleOrientation[2]);
                x3d.gl.glTranslatef(humanoid.center[0] * -1, humanoid.center[1] * -1, humanoid.center[2] * -1);
                x3d.gl.glGetFloatv(GL2.GL_MODELVIEW_MATRIX, extraVar.thisMatrix, 0);
                x3d.gl.glPopMatrix();
                if ((coord != null) && (coord.point != null))
                    for (i = 0; i < coord.point.length; i++)
                        coord.point[i] = 0;
            }
            x3d.gl.glPushMatrix();
            x3d.gl.glMultMatrixf(extraVar.thisMatrix, 0);
            if (humanoid.skin != null)
                for (i = 0; i < humanoid.skin.length; i++)
                    if (humanoid.skin[i] != null) {
                        x3d.gl.glDisable(GL2.GL_TEXTURE_2D);
                        humanoid.skin[i].treeRender(data);
                    }
            if (humanoid.skeleton != null)
                for (i = 0; i < humanoid.skeleton.length; i++)
                    if (humanoid.skeleton[i] != null)
                    {
                        if (x3d.preRender) 
                        {
                            if (humanoid.skeleton[i].getType() == 
                                X3dHAnimJointType.type)
                                if ((coord != null) && (coord.point != null))
                                    HAnimJointExtra.applyJoint(
                                        humanoid.skeleton[i], coord, 
                                        extraVar.origVertices, data);
                        }
                        x3d.gl.glDisable(GL2.GL_TEXTURE_2D);
                        humanoid.skeleton[i].treeRender(data);
                    }
            x3d.gl.glPopMatrix();
        }
    }
}



class MyHAnimJointRenderCallback extends X3dHAnimJointRenderCallback
{
    public void render(X3dNode data)
    {
        X3dHAnimJoint joint = (X3dHAnimJoint)data;
        HAnimJointExtraDataStruct extraVar;
        int i;

         if(x3d.initRender)
         {
             joint.extra_data = new HAnimJointExtraDataStruct();
             ((HAnimJointExtraDataStruct)(joint.extra_data)).jointMatrix = new float[16];
             ((HAnimJointExtraDataStruct)(joint.extra_data)).thisMatrix = new float[16];
         }
         extraVar = (HAnimJointExtraDataStruct)joint.extra_data;
         if (extraVar == null)
             return;
         if(x3d.initRender)
         {
             x3d.gl.glMatrixMode(GL2.GL_MODELVIEW);
             x3d.gl.glPushMatrix();
             x3d.gl.glLoadIdentity();
             x3d.gl.glGetFloatv(GL2.GL_MODELVIEW_MATRIX, extraVar.jointMatrix, 0);
             x3d.gl.glGetFloatv(GL2.GL_MODELVIEW_MATRIX, extraVar.thisMatrix, 0);
             x3d.gl.glPopMatrix();
             if (joint.children != null)
                 for (i = 0; i < joint.children.length; i++)
                     if (joint.children[i] != null)
                     {
                         x3d.gl.glDisable(GL2.GL_TEXTURE_2D);
                         joint.children[i].treeRender(data);
                     }
         }
         else
         {
             if (x3d.preRender)
             {
                 x3d.gl.glMatrixMode(GL2.GL_MODELVIEW);
                 x3d.gl.glPushMatrix();
                 x3d.gl.glLoadIdentity();
                 x3d.gl.glTranslatef(joint.translation[0], joint.translation[1], joint.translation[2]);
                 x3d.gl.glTranslatef(joint.center[0], joint.center[1], joint.center[2]);
                 x3d.gl.glRotatef( ( (joint.rotation[3] / (2*(float)Math.PI) ) * 360), joint.rotation[0], joint.rotation[1], joint.rotation[2]);
                 x3d.gl.glRotatef( ( (joint.scaleOrientation[3] / (2*(float)Math.PI) ) * 360), joint.scaleOrientation[0], joint.scaleOrientation[1], joint.scaleOrientation[2]);
                 x3d.gl.glScalef(joint.scale[0], joint.scale[1], joint.scale[2]);
                 x3d.gl.glRotatef( ( (joint.scaleOrientation[3] / (2*(float)Math.PI) ) * 360) * -1, joint.scaleOrientation[0], joint.scaleOrientation[1], joint.scaleOrientation[2]);
                 x3d.gl.glTranslatef(joint.center[0] * -1, joint.center[1] * -1, joint.center[2] * -1);
                 x3d.gl.glGetFloatv(GL2.GL_MODELVIEW_MATRIX, extraVar.thisMatrix, 0);
                 x3d.gl.glPopMatrix();
             }
             x3d.gl.glMultMatrixf(extraVar.thisMatrix, 0);
             if (joint.children != null)
                 for (i = 0; i < joint.children.length; i++)
                     if (joint.children[i] != null)
                     {
                         x3d.gl.glDisable(GL2.GL_TEXTURE_2D);
                         joint.children[i].treeRender(data);
                     }
         }
     }
}


class MyPositionInterpolatorProcessEventCallback extends X3dPositionInterpolatorProcessEventCallback 
{
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
                                             interpolator.keyValue, 
                                             interpolator.key.length, 3, 0);
        return true;
    }
}

class MyOrientationInterpolatorProcessEventCallback extends X3dOrientationInterpolatorProcessEventCallback 
{
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
                                             interpolator.keyValue, 
                                             interpolator.key.length, 4, 1);
        return true;
    }
}

class MyColorInterpolatorProcessEventCallback extends X3dColorInterpolatorProcessEventCallback 
{
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
                                             interpolator.keyValue, 
                                             interpolator.key.length, 3, 0);
        return true;
    }
}

class MyScalarInterpolatorProcessEventCallback extends X3dScalarInterpolatorProcessEventCallback 
{
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
                                             interpolator.keyValue, 
                                             interpolator.key.length, 1, 0);
        interpolator.value_changed = value[0];
        return true;
    }
}


class MyCoordinateInterpolatorProcessEventCallback extends X3dCoordinateInterpolatorProcessEventCallback 
{
    int targetSize;

    public boolean processEvent(X3dNode node, String event) {
        X3dCoordinateInterpolator interpolator = (X3dCoordinateInterpolator)node;
        if (interpolator.value_changed == null) {
            int targetSize = (interpolator.keyValue.length / 3) / 
                             interpolator.key.length;
            if (interpolator.key.length * targetSize * 3 != 
                interpolator.keyValue.length) {
                x3d.error("invalid CoodinateInterpolator: number keys do not match number values\n");
                return false;
            }
            interpolator.value_changed = new float[targetSize == 0 ? 3 :
                                                   targetSize * 3];
        }
            
        X3d_Interpolator.accountInterpolator(interpolator.value_changed, 
                                             interpolator.set_fraction,
                                             interpolator.key, 
                                             interpolator.keyValue, 
                                             interpolator.key.length, 
                                             interpolator.value_changed.length,
                                             0);
        return true;
    }
}

class MyNormalInterpolatorProcessEventCallback extends X3dNormalInterpolatorProcessEventCallback 
{
    public boolean processEvent(X3dNode node, String event) {
        int i,j;
        X3dNormalInterpolator interpolator = (X3dNormalInterpolator)node;
        int targetSize = (interpolator.keyValue.length / 3) / 
                         interpolator.key.length;
        if (interpolator.value_changed == null) {
            if (interpolator.key.length * targetSize * 3 != 
                interpolator.keyValue.length){
                x3d.error("invalid CoodinateInterpolator: number keys do not match number values\n");
                return false;
            }
            interpolator.value_changed = new float[targetSize == 0 ? 3 :
                                                   targetSize * 3];
        }
            
        X3d_Interpolator.accountInterpolator(interpolator.value_changed, 
                                             interpolator.set_fraction,
                                             interpolator.key, 
                                             interpolator.keyValue, 
                                             interpolator.key.length, 
                                             interpolator.value_changed.length,
                                             0);
    
        /* normalize results */
        for (i = 0; i < targetSize; i += 3) {
            float len = (float)Math.sqrt(interpolator.keyValue[3 * i + 0] * 
                                         interpolator.keyValue[3 * i + 0] + 
                                         interpolator.keyValue[3 * i + 1] * 
                                         interpolator.keyValue[3 * i + 1] + 
                                         interpolator.keyValue[3 * i + 2] * 
                                         interpolator.keyValue[3 * i + 2]);
            if (len != 0)
                for (j = 0; j < 3 ; j++)
                    interpolator.keyValue[3 * i + j] /= len;
        }   
        return true;
    }
}

class MyPositionInterpolator2DProcessEventCallback extends X3dPositionInterpolator2DProcessEventCallback 
{
    private boolean warned;

    MyPositionInterpolator2DProcessEventCallback() 
    {
        warned = false;
    }

    static int warnedPositionInterpolator2DSendEvents = 0;
    
    public boolean processEvent(X3dNode node, String event)
    {
        X3dPositionInterpolator2D interpolator = (X3dPositionInterpolator2D)
                                                 node;
        if (interpolator.key.length * 2 != interpolator.keyValue.length) { 
            if (!warned)
                x3d.error("invalid PositionInterpolator: number keys do not match number values\n");
            warned = true;
            return false;
        }
        X3d_Interpolator.accountInterpolator(interpolator.value_changed, 
                                             interpolator.set_fraction,
                                             interpolator.key, 
                                             interpolator.keyValue, 
                                             interpolator.key.length, 2, 0); 
        return true;
    }
}   

class MyTimeSensorProcessEventCallback extends X3dTimeSensorProcessEventCallback 
{
    private static double getTimerTime() 
    {
        java.util.Date date = new java.util.Date();
        return date.getTime() / 1000.0;
    }
    
    class TimeSensorData 
    {
        public double startTime;
        public double stopTime;
    };

    public boolean processEvent(X3dNode node, String event) {

        X3dTimeSensor timeSensor = (X3dTimeSensor)node;
        double currentTime = 0;
        TimeSensorData dataPtr;
        double callTime;
        if (timeSensor.m_data == null) {
            timeSensor.m_data = new TimeSensorData();
            timeSensor.isActive = false;     
            dataPtr = (TimeSensorData)timeSensor.m_data;
            dataPtr.startTime = timeSensor.startTime;
            dataPtr.stopTime = timeSensor.stopTime;       
        }
    
        dataPtr = (TimeSensorData)timeSensor.m_data;
    
        if (!timeSensor.enabled)
            return false;
    
        currentTime = getTimerTime();
    
        if (timeSensor.isActive) {
            timeSensor.time = currentTime;
        } else {
            if (timeSensor.startTime > dataPtr.stopTime)
                dataPtr.startTime = timeSensor.startTime;
        }
        if (!timeSensor.loop) { 
            if (dataPtr.stopTime > dataPtr.startTime) {
                if (timeSensor.isActive) {
                    dataPtr.stopTime = currentTime;
                    timeSensor.isActive = true;
                    return true;  
                }
                return false;
            }
            if (currentTime > (dataPtr.startTime + 
                               timeSensor.cycleInterval)) {
                timeSensor.fraction_changed = 1.0f;
                if (timeSensor.isActive) {
                    dataPtr.stopTime = currentTime;
                    timeSensor.isActive = false;
                    return true;
                }
                return false;
            }
            if (currentTime < timeSensor.startTime)
                return false;
        }

        timeSensor.isActive = true;
                
        callTime = currentTime - dataPtr.startTime;
        timeSensor.fraction_changed = (float)(callTime % 
                                              timeSensor.cycleInterval) /
                                      (float)timeSensor.cycleInterval;
        return true;
    }
}

class X3d_Interpolator 
{
    static float interpolate(float t, float key, float oldKey, 
                             float value, float oldValue)
    {
        float delta = (key == oldKey) ? 0 : (t - oldKey) / (key - oldKey);
        return oldValue + delta * (value - oldValue);
    }
    
    static void accountInterpolator(float target[], float fraction,
                                    float keys[], float keyValues[], 
                                    int numKeys, int stride, int rotation)
    {
        int i;
        int j;
        if (rotation == 0) {
            if (numKeys > 0) {
                for (j = 0; j < stride; j++)
                    target[j]=keyValues[j];
                int oldi = 0;
                for (i = 0; i < numKeys; i++) {
                    if ((fraction >= keys[oldi]) && (fraction < keys[i])) {
                        for (j = 0; j < stride; j++)
                            target[j] = interpolate(fraction, keys[i], keys[oldi], 
                                                    keyValues[i * stride + j], 
                                                    keyValues[oldi * stride + j]); 
                        break;
                    }
                    oldi = i;
                }
                if (i == numKeys)
                    for (j = 0; j < stride; j++)
                        target[j] = keyValues[(numKeys - 1) * stride + j];
            }
        } else {
            float k1 = 0.0f;
            float k2 = 1.0f;
            int pos1 = 0;
            int pos2 = 0;
            int pos;
            float s;
            float q[] = new float[4];
            float q1[] = new float[4];
            float q2[] = new float[4];
            int flip;
            float alpha;
            float beta;
            float theta;
            float sin_t;
            float cos_t;
            float len;
    
            if (numKeys == 0) {
                target[0] = 0.0f;
                target[1] = 0.0f;
                target[2] = 1.0f;
                target[3] = 0.0f;
                return;
            }
    
            for (pos = 0; pos < numKeys; pos++)
                if (keys[pos] > fraction) 
                    break;
    
            if (pos == 0) {
                k1 = 0.0f;
                pos1 = 0;
            } else {
                k1 = keys[pos - 1];
                pos1 = pos - 1;
            }
    
            if (pos == numKeys) {
                k2 = 1.0f;
                pos2 = numKeys - 1;
            } else {
               k2 = keys[pos];
               pos2 = pos;
            }
    
            if ((k1 == k2) || (pos1==pos2))
                alpha = 0.0f;
            else
                alpha = (fraction - k1) / (k2 - k1);
    
            s = (float)Math.sin(keyValues[pos1 * 4 + 3] * 0.5f);
            for (i = 0; i < 3; i++)
                 q1[i] = keyValues[pos1 * 4 + i] * s;
            q1[3] = (float)Math.cos(keyValues[pos1 * 4 + 3] * 0.5f);
    
            s = (float)Math.sin(keyValues[pos2 * 4 + 3] * 0.5f);
            for (i = 0; i < 3; i++)
                 q2[i] = keyValues[pos2 * 4 + i] * s;
            q2[3] = (float)Math.cos(keyValues[pos2 * 4 + 3] * 0.5f);
            
            cos_t = q1[0]*q2[0] + q1[1]*q2[1] + q1[2]*q2[2] + q1[3]*q2[3];
    
            len = (float) Math.sqrt(cos_t);
            if (len > 0)
                len = 1 / len;
            else
                len = 1;
    
            if (cos_t < 0.0) {
                cos_t = -cos_t;
                flip = 1;
            } else
                flip = 0;
            if (1.0 - cos_t < x3d.EPSILON)
                beta = 1.0f - alpha;
            else {
                theta = (float) Math.acos(cos_t);
                sin_t = (float) Math.sin(theta);
                beta = (float) Math.sin(theta - alpha * theta) / sin_t;
                alpha = (float) Math.sin(alpha * theta) / sin_t;
            }
            if (flip != 0)
                alpha = -alpha;
            for (i = 0; i < 4; i++)
                q[i] = (q1[i] * beta + q2[i] * alpha);
            for (i = 0; i < 3; i++)
                target[i] = q[i] * len;
            if (stride == 4)
                target[3] = 2.0f * (float) Math.acos(q[3]);
        }
    }
}
        

    
