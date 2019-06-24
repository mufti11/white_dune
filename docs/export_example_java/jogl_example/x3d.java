/* Copyright (c) Stefan Wolf, 2010. */
/* Copyright (c) J. "MUFTI" Scheurich, 2015,2016, 2019. */

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

import java.util.Random;

// draws a X3DV file with shapes (converted to) IndexedFaceSet nodes

public class x3d implements GLEventListener 
{
    public static final float Z_NEAR = 0.05f;
    public static final float Z_FAR = 7000.0f;
   
    public static final float EPSILON = 1E-9f;
   
    public static boolean viewpointRendered = false;
    public static boolean viewPointExists = false;
    public static float viewpoint1Position[] = { 0, 0, 10 };

    public static X3dNode viewpoint1 = null;

    public static boolean lightExists = false;
   
    public static boolean preRender = false;
    public static boolean initRender = false;
   
    public static X3dSceneGraph x3dSceneGraph = new X3dSceneGraph();

    public static double[] projectionMatrix = new double[16];
   
    public static X3dPointLight pointlight = null;

    private static int numLights = 0;

    public static float view_rotx = 0.0f, view_roty = 0.0f;
    public static final float view_rotz = 0.0f;
    public static float view_dist = -10.0f;

    private static int prevMouseX, prevMouseY;
    private static int mouseBotton;

    public static float navigation_matrix[] = new float[16];

    public static GLUT glut = new GLUT();
    public static GLU glu = new GLU();

    public static void main(String[] args) 
    {
        final x3d x3d = new x3d();

        java.awt.Frame frame = new java.awt.Frame("white_dune java viewer");
        frame.setSize(500, 500);
        frame.setLayout(new java.awt.BorderLayout());

        GLCanvas canvas = new GLCanvas();
        // GLCapabilities caps = new GLCapabilities(GLProfile.getDefault());
        // GLCanvas canvas = new GLCanvas(caps);

        canvas.addGLEventListener(x3d);

        frame.add(canvas, java.awt.BorderLayout.CENTER);
        frame.validate();

        frame.setVisible(true);

        Animator animator = new Animator(canvas);
        animator.start();
    }

    public static void error(String errormsg)
    {
        System.err.println(errormsg);
    }


    public static double getTimerTime() 
    {
        java.util.Date date = new java.util.Date();
        return date.getTime() / 1000.0;
    }
    
    @Override
    public void init(GLAutoDrawable drawable) 
    {
        // Use debug pipeline
        // drawable.setGL(new DebugGL(drawable.getGL()));

        x3dSceneGraph.initThings = false;
        x3dSceneGraph = new X3dSceneGraph();
        x3dSceneGraph.initThings = false;
        x3dSceneGraph = new X3dSceneGraph();

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
        x3d2.setWidthHeight(width, height);

        GL2 gl = drawable.getGL().getGL2();

        float h = (float)height / (float)width;

        gl.glMatrixMode(GL2.GL_PROJECTION);

        gl.glLoadIdentity();
        gl.glFrustum(-1.0f, 1.0f, -h, h, 5.0f, 60.0f);
        gl.glMatrixMode(GL2.GL_MODELVIEW);
        gl.glLoadIdentity();
        gl.glTranslatef(0.0f, 0.0f, -10.0f);
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
        x3d.gl.glMatrixMode(GL2.GL_MODELVIEW_MATRIX);
        x3d.gl.glLoadIdentity();
        x3d.gl.glTranslatef(0.0f, 0.0f, x3d.view_dist);
        x3d.gl.glRotatef(view_rotx, 1.0f, 0.0f, 0.0f);
        x3d.gl.glRotatef(view_roty, 0.0f, 1.0f, 0.0f);
        x3d.gl.glRotatef(view_rotz, 0.0f, 0.0f, 1.0f);
        x3d.gl.glGetFloatv(GL2.GL_MODELVIEW_MATRIX, navigation_matrix, 0);
        x3d.gl.glLoadIdentity();

        // draw scenegraph
        x3d2.drawX3d(navigation_matrix);
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
            x3d2.setMouseClick(prevMouseX, prevMouseY); 
        }
    
        @Override
    	public void mouseReleased(MouseEvent e) {
            x3d2.setMouseRelease(e.getX(), e.getY()); 
        }
    
        private boolean init = false;

        @Override
        public void mouseDragged(MouseEvent e) {
            final int x = e.getX();
            final int y = e.getY();
            if (!init) {
                prevMouseX = x;
                prevMouseY = y;    
                init = true;        
            }
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
                if (!x3d2.hasHit()) {
                    view_rotx += thetaX;
                    view_roty += thetaY;
                }
            } else {
                view_dist += ( (float)(prevMouseY-y)/(float)height * 10.0f);
            }

            x3d2.setMouseMove(x, y, prevMouseX, prevMouseY); 

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

        MyIndexedFaceSetCreateNormalsCallback myIndexedFaceSetCreateNormalsCallback = new MyIndexedFaceSetCreateNormalsCallback();
        X3dIndexedFaceSet.setX3dIndexedFaceSetCreateNormalsCallback(myIndexedFaceSetCreateNormalsCallback);

        MyMaterialRenderCallback myMaterialRenderCallback = new MyMaterialRenderCallback();
        X3dMaterial.setX3dMaterialRenderCallback(myMaterialRenderCallback);

        MyPointLightRenderCallback myPointLightRenderCallback = new MyPointLightRenderCallback();
        X3dPointLight.setX3dPointLightRenderCallback(myPointLightRenderCallback);

        MyViewpointRenderCallback myViewpointRenderCallback = new MyViewpointRenderCallback();
        X3dViewpoint.setX3dViewpointRenderCallback(myViewpointRenderCallback);

        MyPixelTextureRenderCallback myPixelTextureRenderCallback = new MyPixelTextureRenderCallback();
        X3dPixelTexture.setX3dPixelTextureRenderCallback(myPixelTextureRenderCallback);

        MyImageTextureRenderCallback myImageTextureRenderCallback = new MyImageTextureRenderCallback();
        X3dImageTexture.setX3dImageTextureRenderCallback(myImageTextureRenderCallback);

        MyShapeRenderCallback myShapeRenderCallback = new MyShapeRenderCallback();
        X3dShape.setX3dShapeRenderCallback(myShapeRenderCallback);

        MySwitchRenderCallback mySwitchRenderCallback = new MySwitchRenderCallback();
        X3dSwitch.setX3dSwitchRenderCallback(mySwitchRenderCallback);

        MyHAnimHumanoidRenderCallback myHAnimHumanoidRenderCallback = new MyHAnimHumanoidRenderCallback();
        X3dHAnimHumanoid.setX3dHAnimHumanoidRenderCallback(myHAnimHumanoidRenderCallback);

        MyHAnimJointRenderCallback myHAnimJointRenderCallback = new MyHAnimJointRenderCallback();
        X3dHAnimJoint.setX3dHAnimJointRenderCallback(myHAnimJointRenderCallback);

        MyParticleSystemRenderCallback myParticleSystemRenderCallback = new MyParticleSystemRenderCallback();
        X3dParticleSystem.setX3dParticleSystemRenderCallback(myParticleSystemRenderCallback);

        MyTimeSensorProcessEventCallback myTimeSensorProcessEventCallback = new MyTimeSensorProcessEventCallback();
        X3dTimeSensor.setX3dTimeSensorProcessEventCallback(myTimeSensorProcessEventCallback);

        MyTouchSensorProcessEventCallback myTouchSensorProcessEventCallback = new MyTouchSensorProcessEventCallback();
        X3dTouchSensor.setX3dTouchSensorProcessEventCallback(myTouchSensorProcessEventCallback);

        MyPlaneSensorProcessEventCallback myPlaneSensorProcessEventCallback = new MyPlaneSensorProcessEventCallback();
        X3dPlaneSensor.setX3dPlaneSensorProcessEventCallback(myPlaneSensorProcessEventCallback);

        MyCylinderSensorProcessEventCallback myCylinderSensorProcessEventCallback = new MyCylinderSensorProcessEventCallback();
        X3dCylinderSensor.setX3dCylinderSensorProcessEventCallback(myCylinderSensorProcessEventCallback);

        MySphereSensorProcessEventCallback mySphereSensorProcessEventCallback = new MySphereSensorProcessEventCallback();
        X3dSphereSensor.setX3dSphereSensorProcessEventCallback(mySphereSensorProcessEventCallback);

        MyProximitySensorProcessEventCallback myProximitySensorProcessEventCallback = new MyProximitySensorProcessEventCallback();
        X3dProximitySensor.setX3dProximitySensorProcessEventCallback(myProximitySensorProcessEventCallback);

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

        x3d.gl.glMatrixMode(GL2.GL_PROJECTION);
        x3d.gl.glLoadIdentity();
        x3d.glu.gluPerspective(x3d2.fieldOfViewdegree, 1.0, Z_NEAR, Z_FAR); 
        x3d.gl.glGetDoublev(GL2.GL_PROJECTION_MATRIX, projectionMatrix, 0);
        x3d.gl.glMatrixMode(GL2.GL_MODELVIEW);

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
        if (rootNode != null)
            rootNode.treeRender(null, null);

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
            viewpoint.treeRender(null, null);
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
            x3d.gl.glPopMatrix();
        }
    }
    
    static void textureTransformUnBind()
    {
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

    public static float fieldOfViewdegree = 45;

    public static int PICK_BUFFER_SIZE = 65536;
    public static float PICK_REGION_SIZE = 2.5f;

    public static int mouseX = -1;
    public static int mouseY = -1;
    public static boolean clicked = false;
    public static int mouseXMove = -1;
    public static int mouseYMove = -1;
    public static int mouseXOld = -1;
    public static int mouseYOld = -1;
    public static boolean moved = false;
    public static boolean released = false;

    public static void setMouseClick(int x, int y) 
    {
        mouseX = x;
        mouseY = y;
        clicked = true;
        released = false;  
    }

    public static void setMouseRelease(int x, int y) 
    {
        mouseX = x;
        mouseY = y;
        released = true;  
    }

    public static void setMouseMove(int x, int y, int x_old, int y_old) 
    {
        mouseXMove = x;
        mouseYMove = y;
        mouseXOld = x_old;
        mouseYOld = y_old;
        moved = true;
    }

    public static int width = 600;
    public static int height = 600;

    public static void setWidthHeight(int w, int h)
    {
        width = w;
        height = h;
    }


    static float mod1(float x) 
    {
        if (x == 0)
            return 0;
        if (x - (int)x == 0)
            return 1;
        return x - (int)x;
    }

    static void normalizeAxis(float[] value)
    {
        float rlen = (float) Math.sqrt(value[0] * value[0] + 
                                       value[1] * value[1] +  
                                       value[2] * value[2] );  
        if (rlen > 0.000000001f) { 
            rlen = 1.0f / rlen; 
            value[0] *= rlen;
            value[1] *= rlen;
            value[2] *= rlen;
        }
    }
    
    static void normalizeQuaternion(float[] value)
    {
        float rlen = (float) Math.sqrt(value[0] * value[0] + 
                                       value[1] * value[1] +  
                                       value[2] * value[2] +  
                                       value[3] * value[3] );  
        if (rlen > 0.000000001f) { 
            rlen = 1.0f / rlen; 
            value[0] *= rlen;
            value[1] *= rlen;
            value[2] *= rlen;
            value[3] *= rlen;
        }
    }
    
    static void SFRotation2quaternion(float ret[], float rot[])
    {
        float s = (float) Math.sin(rot[3] * 0.5f);
        ret[0] = rot[0];
        ret[1] = rot[1];
        ret[2] = rot[2];
        normalizeAxis(ret);
        for (int i = 0; i < 3; i++)
            ret[i] = ret[i] * s;
        ret[3] = (float) Math.cos(rot[3] * 0.5);
    
        normalizeQuaternion(ret);
    }
    
    static void quaternion2SFRotation(float ret[], float q[])
    {
        float s = 2 * (float) Math.asin(mod1(q[3]));
        ret[0] = q[0];
        ret[1] = q[1];
        ret[2] = q[2];
        for (int i = 0; i < 3; i++)
            ret[i] = ret[i] * s;
        ret[3] = 2 * (float) Math.acos(mod1(q[3]));
    }
    
    static void quaternionMult(float ret[], float q1[], float q2[])
    {
        ret[0] = q2[3] * q1[0] + q2[0] * q1[3] + q2[1] * q1[2] - q2[2] * q1[1];
        ret[1] = q2[3] * q1[1] + q2[1] * q1[3] + q2[2] * q1[0] - q2[0] * q1[2];
        ret[2] = q2[3] * q1[2] + q2[2] * q1[3] + q2[0] * q1[1] - q2[1] * q1[0];
        ret[3] = q2[3] * q1[3] - q2[0] * q1[0] - q2[1] * q1[1] - q2[2] * q1[2];
    }
    
    static void quaternionMultVec(float ret[], float q[], float vec[])
    {
        float quat[] = new float[4];;
        quat[0] = -q[0];
        quat[1] = -q[1];
        quat[2] = -q[2];
        quat[3] =  q[3];
        float quat2[] = new float[4];;
        quat2[0] = vec[0];
        quat2[1] = vec[1];
        quat2[2] = vec[2];
        quat2[3] = 0;
        float quat3[] = new float[4];;
        quaternionMult(quat3, quat, quat2);
        normalizeQuaternion(quat3);
        quat[0] = q[0];
        quat[1] = q[1];
        quat[2] = q[2];
        quat[3] = q[3];
        float quat4[] = new float[4];
        quaternionMult(quat4, quat3, quat);
        normalizeQuaternion(quat4);
        ret[0] = quat4[0];
        ret[1] = quat4[1];
        ret[2] = quat4[2]; 
    }
    
    static void quaternionFromEuler(float ret[], float yaw, float pitch, float roll)
    {
        double cy = Math.cos(yaw * 0.5);
        double sy = Math.sin(yaw * 0.5);
        double cp = Math.cos(pitch * 0.5);
        double sp = Math.sin(pitch * 0.5);
        double cr = Math.cos(roll * 0.5);
        double sr = Math.sin(roll * 0.5);
    
        ret[0] = (float)(cy * cp * sr - sy * sp * cr);
        ret[1] = (float)(sy * cp * sr + cy * sp * cr);
        ret[2] = (float)(sy * cp * cr - cy * sp * sr);
        ret[3] = (float)(cy * cp * cr + sy * sp * sr);
    
        normalizeQuaternion(ret);
    }
    
    static void SFRotationFromEuler(float ret[], float rotx, float roty, float rotz)
    {
        float quat[] = new float[4];
        quaternionFromEuler(quat, rotz * 2 * (float)Math.PI / 360.0f, 
                                  roty * 2 * (float)Math.PI / 360.0f, 
                                  rotx * 2 * (float)Math.PI / 360.0f);
        quaternion2SFRotation(ret, quat);
    }
    
    static void multMatrix4Vec(float ret[], float mat[], float vec[])
    {
        ret[0] = mat[0] * vec[0] + mat[1] * vec[1] + mat[2] * vec[2];
        ret[1] = mat[4] * vec[0] + mat[5] * vec[1] + mat[6] * vec[2];
        ret[2] = mat[8] * vec[0] + mat[9] * vec[1] + mat[10] * vec[2];
    }
    
    static void drawX3d(float matrix[])
    {
        x3d.gl.glMatrixMode(GL2.GL_PROJECTION);
        x3d.gl.glLoadIdentity();
        x3d.glu.gluPerspective(fieldOfViewdegree, 1.0, x3d.Z_NEAR, x3d.Z_FAR);  /* fieldOfView in degree, aspect radio, Z nearest, Z farest */

        x3d.gl.glMatrixMode(GL2.GL_MODELVIEW);

        x3d.gl.glLoadMatrixf(matrix, 0);    

        x3d.viewpointRendered = false;
        
        x3d.preRender = true;
    
        if (rootNode != null)
            rootNode.treeRender(null, null);

        if(!x3d.lightExists)
        {
            int lightNum = x3d.allocLight();
            x3d.gl.glEnable(lightNum);
            x3d.lightExists = true;
        }

        x3d.gl.glMatrixMode(GL2.GL_MODELVIEW);
        x3d.preRender = false;
        if (rootNode != null)
            rootNode.treeRender(null, null);

        if (clicked || moved) {
            // render to pickbuffer
            IntBuffer pickBuffer = Buffers.newDirectIntBuffer(PICK_BUFFER_SIZE); 
            x3d.gl.glSelectBuffer(PICK_BUFFER_SIZE, pickBuffer);
            x3d.gl.glRenderMode(GL2.GL_SELECT);
            x3d.gl.glInitNames(); 

            x3d.gl.glMatrixMode(GL2.GL_PROJECTION);
            x3d.gl.glLoadIdentity();
            int v[] = new int[4];
            x3d.gl.glGetIntegerv(GL2.GL_VIEWPORT, v, 0);
            if (clicked)
                x3d.glu.gluPickMatrix(mouseX, (height - mouseY), 1, 1, v, 0);
            if (moved)
                x3d.glu.gluPickMatrix(mouseXMove, (height - mouseYMove), 1, 1, v, 0);

            x3d.glu.gluPerspective(fieldOfViewdegree, 1.0, x3d.Z_NEAR, x3d.Z_FAR);  /* fieldOfView in degree, aspect radio, Z nearest, Z farest */
    
            x3d.gl.glMatrixMode(GL2.GL_MODELVIEW);
    
            x3d.gl.glLoadMatrixf(matrix, 0);    
    
            x3d.viewpointRendered = false;
            
            x3d.preRender = true;
        
            if (rootNode != null)
                rootNode.treeRender(null, null);
    
            x3d.gl.glMatrixMode(GL2.GL_MODELVIEW);
            x3d.preRender = false;
            if (rootNode != null)
                rootNode.treeRender(null, null);
       
            int hits = x3d.gl.glRenderMode(GL2.GL_RENDER);
            if (hits < 0) // overflow flag has been set, ignore
                hits = - hits;
            processHits(hits, pickBuffer);
        }
    }

    private static void transform4HandleData(X3dTransform transform)
    {
        x3d.gl.glTranslatef(transform.translation[0], transform.translation[1], transform.translation[2]);
    }

    private static void transform(X3dNode node)
    {
        for (X3dNode parent = node.m_parent; parent != null; 
             parent = parent.m_parent)
            if (parent.getType() == X3dTransformType.type) {
                X3dTransform transform = (X3dTransform)parent;
                boolean hasTransform = false;
                for (int i = 0; i < transform.num_route_source; i++)
                    if (transform.route_sources[i] == node)
                        hasTransform = true;
                if (!hasTransform)
                    transform4HandleData(transform);            
            }
     }

    static class CylinderSensorExtraDataStruct {
        public float x_sum;
    };

    private static boolean isHit = false;

    public static boolean hasHit() {
        return isHit;
    }

    public static void handleSibling(X3dNode sibling) 
    {
        if (sibling.getType() == X3dTouchSensorType.type) {
            X3dTouchSensor touchSensor = (X3dTouchSensor )sibling;
            touchSensor.touchTime = x3d.getTimerTime();
            isHit = true;
        }
        if (moved && sibling.getType() == X3dPlaneSensorType.type) {
            X3dPlaneSensor planeSensor = (X3dPlaneSensor)sibling;
            x3d.gl.glMatrixMode(GL2.GL_MODELVIEW);
            x3d.gl.glPushMatrix();
            x3d.gl.glLoadIdentity();
            transform(planeSensor);
            x3d.gl.glTranslatef(0, 0, x3d.view_dist);
            x3d.gl.glMatrixMode(GL2.GL_PROJECTION);
            x3d.gl.glLoadMatrixd(x3d.projectionMatrix, 0);
            x3d.gl.glMatrixMode(GL2.GL_MODELVIEW);
            int x = mouseXMove;
            int y = mouseYMove;
            float model[] = new float[16];
            float proj[] = new float[16];
            int view[] = new int[4];
            float pan[] = new float[3];
            x3d.gl.glGetFloatv(GL2.GL_MODELVIEW_MATRIX, model, 0);
            x3d.gl.glGetFloatv(GL2.GL_PROJECTION_MATRIX, proj, 0);
            x3d.gl.glGetIntegerv(GL2.GL_VIEWPORT, view, 0);
            x3d.glu.gluProject(x, y, 0.0f, model, 0, proj, 0, view, 0, pan, 0);
            x3d.glu.gluUnProject(x, y, pan[2], model, 0, proj, 0, view, 0,
                                 pan, 0);
            pan[1] = -pan[1];
            x3d.gl.glPopMatrix();
            planeSensor.translation_changed[0] = pan[0];
            planeSensor.translation_changed[1] = pan[1];
            planeSensor.translation_changed[2] = 0;
            isHit = true;
        }
        if (moved && sibling.getType() == X3dCylinderSensorType.type) {
            X3dCylinderSensor cylinderSensor = (X3dCylinderSensor)sibling;
         
            CylinderSensorExtraDataStruct extraVar = null; 
            if (cylinderSensor.extra_data == null) {
                cylinderSensor.extra_data = new
                                             CylinderSensorExtraDataStruct();
                CylinderSensorExtraDataStruct extraVar2 = 
                    (CylinderSensorExtraDataStruct)cylinderSensor.extra_data;
                extraVar2.x_sum = 0;
            }
            extraVar = (CylinderSensorExtraDataStruct)cylinderSensor.extra_data;
    
            float rot1[] = { 0, 1, 0, (mouseXMove - mouseXOld + extraVar.x_sum) / 20.f};
            float quat[] = new float[4];
            SFRotation2quaternion(quat, rot1);
            quaternion2SFRotation(cylinderSensor.rotation_changed, quat);
            extraVar.x_sum += mouseXMove - mouseXOld; 
            isHit = true;
        }
        if (moved && sibling.getType() == X3dSphereSensorType.type) {
            X3dSphereSensor sphereSensor = (X3dSphereSensor)sibling;
    
            float rot1[] = { 0, 1, 0, (mouseXMove - mouseXOld) / 20.f};
            float rot2[] = { 1, 0, 0, (mouseYMove - mouseYOld) / 20.f};
    
            float q1[] = new float[4];
            SFRotation2quaternion(q1, rot1);
            float q2[] = new float[4];
            SFRotation2quaternion(q2, rot2);
            float quat[] = new float[4];
            quaternionMult(quat, q1, q2);
            float rotSphere[] = { sphereSensor.rotation_changed[0],
                                  sphereSensor.rotation_changed[1],
                                  sphereSensor.rotation_changed[2],
                                 -sphereSensor.rotation_changed[3] };
            float qSphere[] = new float[4];
            SFRotation2quaternion(qSphere, rotSphere);
            float rotSphere2[] = { sphereSensor.rotation_changed[0],
                                   sphereSensor.rotation_changed[1],
                                   sphereSensor.rotation_changed[2],
                                   sphereSensor.rotation_changed[3] };
            float qSphere2[] = new float[4];
            SFRotation2quaternion(qSphere2, rotSphere2);
            float quat2[] = new float[4];
            quaternionMult(quat2, quat, qSphere); 
            float quat3[] = new float[4];
            quaternionMult(quat3, qSphere2, quat2); 
            float quat4[] = new float[4];
            quaternionMult(quat4, quat3, qSphere2); 
            quaternion2SFRotation(sphereSensor.rotation_changed, quat4);
            normalizeAxis(sphereSensor.rotation_changed);
            isHit = true;
        }
    }

    public static void processHits(int hits, IntBuffer pickBuffer)
    {
        int depth = Integer.MAX_VALUE;
        int hit = -1;
        int bufferPtr = 0;
        for (int j = 0; j < hits; j++) {
            int numNames = pickBuffer.get(bufferPtr++);
            int minDepth = pickBuffer.get(bufferPtr++);
            int maxDepth = pickBuffer.get(bufferPtr++);
            for (int i = 0; i < numNames; i++) {
                int buffer = pickBuffer.get(bufferPtr++);
                if (maxDepth < depth) {
                    depth = maxDepth; 
                    hit = buffer;
                }
            } 
        }    
        isHit = false;
        if (hit > -1) {
            X3dNode node = x3d.x3dSceneGraph.getNodeFromGlName(hit);
            if (node != null)
                 for (X3dNode parent = node.m_parent; 
                      parent != null; 
                      parent = parent.m_parent) {
                     X3dNode sibling = null;
                     if (parent.getType() == X3dGroupType.type) {
                         X3dGroup group = (X3dGroup)parent;
                         for (int i = 0; i < group.children.length; i++) {
                             if (group.children[i] != null && 
                                 group.children[i] != node) {
                                 if (clicked && group.children[i].getType() == 
                                     X3dTouchSensorType.type) {
                                     sibling = group.children[i];
                                     handleSibling(sibling);
                                 }
                                 if ((moved && group.children[i].getType() == 
                                      X3dPlaneSensorType.type) ||
                                      (moved && group.children[i].getType() == 
                                      X3dCylinderSensorType.type) ||
                                      (moved && group.children[i].getType() == 
                                      X3dSphereSensorType.type)) {
                                     handleSibling(group.children[i]);
                                 }
                            }
                         }
                     }
                     if (parent.getType() == X3dTransformType.type) {
                         X3dTransform transform = (X3dTransform)parent;
                         for (int i = 0; i < transform.children.length; i++) {
                             if (transform.children[i] != null && 
                                 transform.children[i] != node) {
                                 if (clicked &&
                                     transform.children[i].getType() == 
                                     X3dTouchSensorType.type) {
                                     sibling = transform.children[i];
                                     handleSibling(sibling);
                                 }
                                 if ((moved && transform.children[i].getType() == 
                                     X3dPlaneSensorType.type) ||
                                     (moved && transform.children[i].getType() == 
                                     X3dCylinderSensorType.type) ||
                                     (moved && transform.children[i].getType() == 
                                     X3dSphereSensorType.type)) {
                                     handleSibling(transform.children[i]);
                                 }
                             }
                         }
                     }
                 }     
        }
        clicked = false;
        moved = false;
    }
}

class MyGroupRenderCallback extends X3dGroupRenderCallback
{
    public void render(X3dNode data, Object object)
    {
        X3dGroup group = (X3dGroup)data;
        if (group == null)
            return;
        if (group.children != null)
            for (int i = 0; i < group.children.length; i++)
                if (group.children[i] != null) 
                {
                    if(!(x3d.preRender || x3d.initRender))
                        x3d.gl.glDisable(GL2.GL_TEXTURE_2D);
                    group.children[i].treeRender(null, null);
                }
    }
}

class MyIndexedFaceSetRenderCallback extends X3dIndexedFaceSetRenderCallback
{
    public void render(X3dNode data, Object object)
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
            boolean colorRGBA = false;
            X3dNode Xcolor = null;
            if (Xindexedfaceset.color != null) 
                if (Xindexedfaceset.color.getType() == X3dColorRGBAType.type) {
                   Xcolor = (X3dColorRGBA)Xindexedfaceset.color;
                   colorRGBA = true;
                } else
                   Xcolor = (X3dColor)Xindexedfaceset.color;
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
                if (colorRGBA) {
                    colors = ((X3dColorRGBA)Xcolor).color;
                    color_len = ((X3dColorRGBA)Xcolor).color.length;
                } else {
                    colors = ((X3dColor)Xcolor).color;
                    color_len = ((X3dColor)Xcolor).color.length;
                }
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
            if (object != null)
                normal = (float[])((FloatContainer)object).vector;
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
            x3d.gl.glPushName(Xindexedfaceset.glName_number);
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
                if (colorRGBA)
                    x3d.gl.glColor4f(colors[4 * facecounter], colors[4 * facecounter + 1], colors[4 * facecounter + 2], colors[4 * facecounter + 3]);
                else 
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
                    if((colors != null) && colorpervertex)
                        if (colorRGBA)
                            x3d.gl.glColor4f(colors[buffer*4], colors[buffer*4+1], colors[buffer*4+2], colors[buffer*4+3]);
                        else
                            x3d.gl.glColor3f(colors[buffer*3], colors[buffer*3+1], colors[buffer*3+2]);
                    if(Xtexturecoordinate != null)
                        if((Xtexturecoordinate.point != null) && (texturecoordinateindex != null))
                            x3d.gl.glTexCoord2f(Xtexturecoordinate.point[texturebuffer*2], Xtexturecoordinate.point[texturebuffer*2+1]);
                    x3d.gl.glVertex3f(vertex[buffer*3], vertex[buffer*3+1], vertex[buffer*3+2]);
                }
            }
            x3d.gl.glEnd();
            x3d.gl.glPopName();
        }
        x3d.gl.glDisable(GL2.GL_COLOR_MATERIAL); //Maybe needfull
    }
    public void createNormals(X3dNode data, Object extraVar) 
    {
    }
}

class MyIndexedFaceSetCreateNormalsCallback extends X3dIndexedFaceSetCreateNormalsCallback
{
    public void createNormals(X3dNode data, Object extraVar)
    {
        if(x3d.initRender)
        {
            X3dIndexedFaceSet Xindexedfaceset = (X3dIndexedFaceSet)data;
            X3dCoordinate Xcoordinate = (X3dCoordinate)Xindexedfaceset.coord;
            X3dNormal Xnormal = (X3dNormal)Xindexedfaceset.normal;
            int normalIndex[] = Xindexedfaceset.normalIndex;
            int normalIndexLen = 0;
            if (normalIndex == null) {
                normalIndex = Xindexedfaceset.coordIndex;
                normalIndexLen = Xindexedfaceset.coordIndex.length;
            } else
                normalIndexLen = Xindexedfaceset.normalIndex.length;
            int maxLen = -1;
            for (int i = 0; i < normalIndexLen; i++)
                if (normalIndex[i] > maxLen)
                    maxLen = normalIndex[i];
            maxLen++;
            ((FloatContainer)extraVar).vector = new float[maxLen * 3];
        }
        else
        {
            X3dIndexedFaceSet Xindexedfaceset = (X3dIndexedFaceSet)data;
            X3dCoordinate Xcoordinate = (X3dCoordinate)Xindexedfaceset.coord;
            if (Xcoordinate == null)
                return;
            X3dNormal Xnormal = (X3dNormal)Xindexedfaceset.normal;
            int normalIndex[] = Xindexedfaceset.normalIndex;
            if (normalIndex == null)
                normalIndex = Xindexedfaceset.coordIndex;
            float vec[] = (float[])((FloatContainer)extraVar).vector;
            float creaseAngle = Xindexedfaceset.creaseAngle;
            int ci[] = Xindexedfaceset.coordIndex;
            int ciLen = Xindexedfaceset.coordIndex.length;
            float points[] = Xcoordinate.point;
            int pointLen = Xcoordinate.point.length;
            for (int i = 0; i < ciLen; i++) 
            {
                int first = -1;
                int second = -1;
                int third = -1;
                if (ci[i] > -1) {
                    if ((i > 0) && (ci[i - 1] > -1))
                        first = i - 1;
                    else 
                    {
                        for (int j = i; j < ciLen; j++)
                            if (ci[j] < 0)
                            {
                                first = j - 1;
                                break;
                            }
                        if (first == -1)
                            first = ciLen - 1;
                    }
                    second = i;
                    if ((i + 1 < ciLen) && (ci[i + 1] > -1))
                        third = i + 1;
                    else {
                        for (int j = i; j > -1; j--)
                            if (ci[j] < 0) 
                            {
                                third = j + 1;
                                break;
                            }
                        if (third == -1)
                            third = 0;
                    }
                    if ((first > -1) && (second > -1) && (third > -1)) 
                    {
                        float v1x = points[ci[first] * 3    ] - points[ci[second] * 3    ];
                        float v1y = points[ci[first] * 3 + 1] - points[ci[second] * 3 + 1];
                        float v1z = points[ci[first] * 3 + 2] - points[ci[second] * 3 + 2];
    
                        float v2x = points[ci[third] * 3    ] - points[ci[second] * 3    ];
                        float v2y = points[ci[third] * 3 + 1] - points[ci[second] * 3 + 1];
                        float v2z = points[ci[third] * 3 + 2] - points[ci[second] * 3 + 2];
        
                        float crossx = v1y * v2z - v1z * v2y;
                        float crossy = v1z * v2x - v1x * v2z;
                        float crossz = v1x * v2y - v1y * v2x;
        
                        float vLen = (float)Math.sqrt(crossx * crossx + crossy * crossy + crossz * crossz);
                        if (vLen > 0) {
                            vec[normalIndex[i] * 3    ] = - crossx / vLen;      
                            vec[normalIndex[i] * 3 + 1] = - crossy / vLen;      
                            vec[normalIndex[i] * 3 + 2] = - crossz / vLen;
                        } else {
                            vec[normalIndex[i] * 3    ] = 0;      
                            vec[normalIndex[i] * 3 + 1] = 0;      
                            vec[normalIndex[i] * 3 + 2] = 0;
                        }
                    }
                }    
            }
        }    
    }
}
    
class MyMaterialRenderCallback extends X3dMaterialRenderCallback
{
    public void render(X3dNode data, Object object)
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
    public void render(X3dNode data, Object object)
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
    public void render(X3dNode data, Object object)
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
    public void render(X3dNode data, Object object)
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
    public void render(X3dNode data, Object object)
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
    public void render(X3dNode data, Object object)
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
    public void render(X3dNode data, Object object)
    {
        X3dTransform transform = (X3dTransform)data;
        if(x3d.preRender || x3d.initRender)
        {
            if (transform.children != null)
                for (int i = 0; i < transform.children.length; i++)
                    if (transform.children[i] != null)
                        transform.children[i].treeRender(null, null);
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
                        transform.children[i].treeRender(null, null);
                    }
            x3d.gl.glPopMatrix();
        }
    }
}    


class MyViewpointRenderCallback extends X3dViewpointRenderCallback
{
    public void render(X3dNode data, Object object)
    {
        X3dViewpoint viewpoint = (X3dViewpoint)data;
        if(x3d.preRender || x3d.initRender)
        {
            x3d.viewPointExists = true;
    
            if(!x3d.viewpointRendered)
            {
                x3d.viewpointRendered = true;
                x3d.viewpoint1 = viewpoint;

                for (int i = 0; i < 3; i++)
                    x3d.viewpoint1Position[i] = viewpoint.position[i];
    
                x3d2.fieldOfViewdegree = ((viewpoint.fieldOfView / (float)Math.PI) * 180.0f);

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

class MyShapeRenderCallback extends X3dShapeRenderCallback
{
    public void render(X3dNode data, Object object)
    {
        X3dShape shapeNode = (X3dShape)data;
        if (shapeNode == null)
            return;
        x3d.gl.glPushMatrix();
        if (shapeNode.appearance != null) {
            X3dAppearance appear = (X3dAppearance)shapeNode.appearance;
            appear.treeRender(appear, object);
        }
        if (shapeNode.geometry != null) {
            X3dIndexedFaceSet faceset = (X3dIndexedFaceSet)shapeNode.geometry;
            faceset.treeRender(faceset, object);
        }
        x3d.gl.glPopMatrix();
    }
}

class MySwitchRenderCallback extends X3dSwitchRenderCallback
{
    public void render(X3dNode data, Object object)
    {
        X3dSwitch switchNode = (X3dSwitch)data;
        if (switchNode == null)
            return;
        x3d.gl.glPushMatrix();
        int choice = switchNode.whichChoice;
        if (choice > -1 && switchNode.children != null)
            if (choice < switchNode.children.length)
                if (switchNode.children[choice] != null) 
                {
                    if(!(x3d.preRender || x3d.initRender))
                        x3d.gl.glDisable(GL2.GL_TEXTURE_2D);
                    switchNode.children[choice].treeRender(null, null);
                }
        x3d.gl.glPopMatrix();
    }
}

class FloatContainer {
    public float[] vector;
}

class HAnimJointExtraDataStruct {
    public float jointMatrix[];
    public float thisMatrix[];
};

class HAnimHumanoidExtraDataStruct {
     public float origVertices[];
     public int origVertices_length;
     public float thisMatrix[];
     FloatContainer floatContainer;
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
         float mayMatrix[] = { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1};
    
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
    public void render(X3dNode data, Object object)
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
            ((HAnimHumanoidExtraDataStruct)humanoid.extra_data).floatContainer = new FloatContainer();
            if (coord == null)
                System.err.println("Warning: Humanoid.skinCoord not set");
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
                        humanoid.skeleton[i].treeRender(data, null);;
                    }       
            if (humanoid.skin != null)
                for (i = 0; i < humanoid.skin.length; i++) 
                {
                    if (humanoid.skin[i] != null) {
                        humanoid.skin[i].treeRender(data, null);
                        X3dIndexedFaceSet geometry = null;
                        if (humanoid.skin[i].getType() == X3dShapeType.type) 
                        {
                            X3dShape shape = (X3dShape )humanoid.skin[i];
                            if ((shape.geometry != null) && shape.geometry.getType() == X3dIndexedFaceSetType.type)
                                geometry = (X3dIndexedFaceSet)shape.geometry;
                        }
                        if (geometry != null)
                            geometry.createNormals(extraVar.floatContainer);
                        humanoid.skin[i].treeRender(data, extraVar.floatContainer);
                    }
                }
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
//            x3d.gl.glMultMatrixf(extraVar.thisMatrix, 0);
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
                        humanoid.skeleton[i].treeRender(data, null);
                    }
            if (humanoid.skin != null) {
                for (i = 0; i < humanoid.skin.length; i++)
                    if (humanoid.skin[i] != null) {
                        x3d.gl.glDisable(GL2.GL_TEXTURE_2D);
                        humanoid.skin[i].treeRender(data, null);
                        X3dIndexedFaceSet geometry = null;
                        if (humanoid.skin[i].getType() == X3dShapeType.type) 
                        {
                            X3dShape shape = (X3dShape )humanoid.skin[i];
                            if ((shape.geometry != null) && shape.geometry.getType() == X3dIndexedFaceSetType.type)
                                geometry = (X3dIndexedFaceSet)shape.geometry;
                        }
                        if (geometry != null)
                            geometry.createNormals(extraVar.floatContainer);
                        humanoid.skin[i].treeRender(data, extraVar.floatContainer);
                    }
            }
            x3d.gl.glPopMatrix();
        }
    }
}

class MyHAnimJointRenderCallback extends X3dHAnimJointRenderCallback
{
    public void render(X3dNode data, Object object)
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
                         joint.children[i].treeRender(data, null);
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
             x3d.gl.glPushMatrix();
             x3d.gl.glMultMatrixf(extraVar.thisMatrix, 0);
             if (joint.children != null)
                 for (i = 0; i < joint.children.length; i++)
                     if (joint.children[i] != null)
                     {
                         x3d.gl.glDisable(GL2.GL_TEXTURE_2D);
                         joint.children[i].treeRender(data, null);
                     }
             x3d.gl.glPopMatrix();
         }
     }
}

class MyParticleSystemRenderCallback extends X3dParticleSystemRenderCallback
{
    private static double getTimerTime() 
    {
        java.util.Date date = new java.util.Date();
        return date.getTime() / 1000.0;
    }

    public void render(X3dNode data, Object object)
    {
        X3dParticleSystem system = (X3dParticleSystem)data;
        if (system == null)
            return;
        if(x3d.initRender)
        {
             m_rand = new Random();
             m_enabled = true;
             if (!system.geometryType.equals("GEOMETRY")) 
             {
                 m_enabled = false;
                 System.err.println( 
                         "Warning: only Particlesystemem.geometryType " +
                         "== GEOMETRY are rendered");
             } else {
                 int numParticles = system.maxParticles;
                 m_internPosition = new float[numParticles * 3]; 
                 m_internVector = new float[numParticles * 3];
                 m_lifeTime = new double[numParticles];
                 m_startTime = new double[numParticles]; 
                 for (int i = 0; i < numParticles; i++)
                     startParticle(system, i);                      
             }
             m_particlesDirty = true;
             m_internTime = 0;
             m_mass = 0;
             
             if (m_force != null) {    
                 m_force[0] = 0;
                 m_force[1] = 0;
                 m_force[2] = 0;
             }
        }
        else if (!x3d.preRender)
        {
            if (m_enabled) 
            {
                if (m_force != null)
                    for (int j = 0; j < 3; j ++)
                        m_force[j] += 0;
        
                for (int i = 0; i < system.physics.length; i++)
                    if (system.physics[i] != null) 
                    {
                        X3dNode phys = system.physics[i];
                        if (phys.getType() == X3dForcePhysicsModelType.type) 
                        {
                            X3dForcePhysicsModel model = (X3dForcePhysicsModel)
                                                         phys;
                            for (int j = 0; j < 3; j ++)
                                m_force[j] += model.force[j];
                        } else if (phys.getType() == X3dWindPhysicsModelType.type) {
                            X3dWindPhysicsModel model = (X3dWindPhysicsModel)
                                                         phys;
                            for (int j = 0; j < 3; j++)
                                m_force[j] += model.direction[j] *
                                                        Math.pow(10, 2.0f * 
                                                                Math.log(model.speed) * 
                                                                0.64615f);
                        }
                    }
                int numberParticles = system.maxParticles;
            
                double t = getTimerTime();
                double delta = 0;
                if (m_internTime == 0)
                    delta = 0;
                else
                   delta = t - m_internTime;
                for (int i = 0; i < numberParticles; i++) 
                {
                    if ((t - m_startTime[i]) > m_lifeTime[i])
                        startParticle(system, i);
                    else  
                        for (int j = 0; j < 3; j++)
                            m_internVector[i * 3 + j] += 
                                delta * m_force[j];
                    float mass = m_mass;
                    for (int j = 0; j < 3; j++)
                        m_internPosition[3 * i + j] += 
                            mass * m_internVector[3 * i + j] * delta;
            
                    x3d.gl.glPushMatrix();
                    x3d.gl.glTranslatef(m_internPosition[3 * i],
                                        m_internPosition[3 * i + 1],
                                        m_internPosition[3 * i + 2]);
                    if (system.appearance != null)
                        system.appearance.treeRender(null, null);
                    if (system.geometry != null)
                        system.geometry.treeRender(null, null);
                    x3d.gl.glPopMatrix();
                }
                m_internTime = getTimerTime();
    
            }
       }
    }

    boolean m_enabled;
    float   m_force[];
    float   m_internPosition[]; 
    float   m_internVector[]; 
    double  m_lifeTime[];
    double  m_startTime[]; 
    boolean m_particlesDirty;
    double  m_internTime;
    float   m_mass;
    Random  m_rand;
    private  static boolean once;

    public MyParticleSystemRenderCallback()
    {
        once = false;
        m_force = new float[3];
    }

    void startParticle(X3dParticleSystem system, int i)
    {
        float speed = 1;
        m_internPosition[3 * i] = 0;
        m_internPosition[3 * i + 1] = 0; 
        m_internPosition[3 * i + 2] = 0;
        float alpha = (m_rand.nextFloat()) * 2.0f * (float)Math.PI;
        float maxAngle = 2.0f * (float)Math.PI;
        if (system.emitter != null)
            if (system.emitter.getType() == X3dConeEmitterType.type)
            {
                X3dConeEmitter emit = (X3dConeEmitter) system.emitter;
                maxAngle = emit.angle;
            }
        float angle = (m_rand.nextFloat()) * maxAngle; 

        if (m_force == null)
            return;
        m_internVector[3 * i] = speed * m_force[0] * 
                                (float)Math.sin(angle) * (float)Math.cos(alpha);
        m_internVector[3 * i + 1] = speed * m_force[1] * 
                                    (float)Math.cos(angle);
        m_internVector[3 * i + 2]= speed * m_force[2] * 
                                   (float)Math.sin(angle) * (float)Math.sin(alpha);
        boolean setInternVector = false;
    
        if (system.emitter == null)
            return;
        if (system.emitter.getType() == X3dConeEmitterType.type) {
            X3dConeEmitter emit = (X3dConeEmitter)system.emitter;
            m_mass = emit.mass;
            m_internPosition[3 * i] = emit.position[0];
            m_internPosition[3 * i + 1] = emit.position[1]; 
            m_internPosition[3 * i + 2] = emit.position[2];
            speed = (m_rand.nextFloat()) * 
                    (emit.variation / 2.0f + 1) * emit.speed;
        } else if (system.emitter.getType() == X3dPointEmitterType.type) {
            X3dPointEmitter emit = (X3dPointEmitter)system.emitter;
            m_mass = emit.mass;
            m_internPosition[i * 3] = emit.position[0];
            m_internPosition[i * 3 + 1] = emit.position[1]; 
            m_internPosition[i * 3 + 2] = emit.position[2];
            speed = (m_rand.nextFloat()) * 
                    (emit.variation / 2.0f + 1) * emit.speed;
            if ((emit.direction[0] != 0) &&
                (emit.direction[1] != 0) &&
                (emit.direction[0] != 0)) {
                setInternVector = true;
                m_internVector[i * 3    ] = speed * emit.direction[0];
                m_internVector[i * 3 + 1] = speed * emit.direction[1];
                m_internVector[i * 3 + 2] = speed * emit.direction[2];
            }
        } else if (system.emitter.getType() == X3dExplosionEmitterType.type) {
            X3dExplosionEmitter emit = (X3dExplosionEmitter)system.emitter;
            m_mass = emit.mass;
            m_internPosition[3 * i    ] = emit.position[0];
            m_internPosition[3 * i + 1] = emit.position[1]; 
            m_internPosition[3 * i + 2] = emit.position[2];
            speed = (m_rand.nextFloat()) * 
                    (emit.variation / 2.0f + 1) * emit.speed;
        } else if (system.emitter.getType() == X3dPolylineEmitterType.type) {
            X3dPolylineEmitter emit = (X3dPolylineEmitter)system.emitter;
            m_mass = emit.mass;
            X3dCoordinate ncoord = (X3dCoordinate)emit.coord;
            speed = (m_rand.nextFloat()) * 
                    (emit.variation / 2.0f + 1) * emit.speed;
            int numLines = 0;
            float coords[] = null;
            int lineIndicesCoord1[] = new int[emit.coordIndex.length];
            int lineIndicesCoord2[] = new int[emit.coordIndex.length];
            if (ncoord != null) {
                coords = ncoord.point;
                boolean startLine = true;
                boolean validLine = false;
                for (int j = 0; j < emit.coordIndex.length; j++) {
                    if (emit.coordIndex[j] < 0) {
                        startLine = true;
                        validLine = false;
                    } else if (!validLine)
                        validLine = true;  
                    else if (emit.coordIndex[j] != emit.coordIndex[j - 1]) {
                         numLines++;
                         lineIndicesCoord1[numLines - 1] = emit.coordIndex[j];
                         lineIndicesCoord2[numLines - 1] = emit.coordIndex[j - 
                                                                            1];
                    }
                }
            }
            if ((coords != null) && (numLines > 0)) {
                int numLine = (int)((m_rand.nextFloat()) * numLines);
                if (numLine == numLines)
                    numLine--;
                int coord1 = lineIndicesCoord1[numLine];              
                int coord2 = lineIndicesCoord2[numLine];
                if ((coord1 * 3 < ncoord.point.length) && 
                    (coord2 * 3 < ncoord.point.length)) {
                    float point1x = coords[coord1 * 3];              
                    float point1y = coords[coord1 * 3 + 1];              
                    float point1z = coords[coord1 * 3 + 2];              
                    float point2x = coords[coord2 * 3];              
                    float point2y = coords[coord2 * 3 + 1];              
                    float point2z = coords[coord2 * 3 + 2];              
                    float vecx = point1x - point2x;
                    float vecy = point1y - point2y;
                    float vecz = point1z - point2z;
                    float ran = (m_rand.nextFloat());
                    float startPointx = point2x + vecx * ran;
                    float startPointy = point2y + vecy * ran;
                    float startPointz = point2z + vecz * ran;
                    m_internPosition[i * 3    ] = startPointx;
                    m_internPosition[i * 3 + 1] = startPointy;
                    m_internPosition[i * 3 + 2] = startPointz;
                } else {
                    m_internPosition[i * 3    ] = 0;
                    m_internPosition[i * 3 + 1] = 0;
                    m_internPosition[i * 3 + 2] = 0;
                }
            } else {
                m_internPosition[i * 3    ] = 0;
                m_internPosition[i * 3 + 1] = 0;
                m_internPosition[i * 3 + 2] = 0;
            }
            if ((emit.direction[0] != 0) &&
                (emit.direction[1] != 0) &&
                (emit.direction[0] != 0))
            {
                setInternVector = true;
                m_internVector[3 * i    ] = speed * emit.direction[0];
                m_internVector[3 * i + 1] = speed * emit.direction[1];
                m_internVector[3 * i + 1] = speed * emit.direction[2];
            }
        } else if (system.emitter.getType() == X3dVolumeEmitterType.type) {
            X3dVolumeEmitter emit = (X3dVolumeEmitter)system.emitter;
            m_mass = emit.mass;
            X3dCoordinate ncoord = (X3dCoordinate)emit.coord;
            speed = (m_rand.nextFloat()) * 
                    (emit.variation / 2.0f + 1) * emit.speed;
            int numPoints = 0;
            float coords[] = null;
            float coordArray[] = new float[3 * emit.coordIndex.length];
            if (ncoord != null) {
                coords = ncoord.point;
                for (int j = 0; j < emit.coordIndex.length; j++) {
                    int index = emit.coordIndex[j];
                    if (index > -1) {
                        numPoints++;
                        coordArray[(numPoints - 1) * 3    ] = 
                            coords[index * 3    ];
                        coordArray[(numPoints - 1) * 3 + 1] = 
                            coords[index * 3 + 1];
                        coordArray[(numPoints - 1) * 3 + 2] = 
                            coords[index * 3 + 2];
                    }
                }
            }
            if ((coords != null) && (numPoints > 0)) {
                int numPoint = (int)((m_rand.nextFloat()) * numPoints);
                m_internPosition[i * 3    ] = 
                    coordArray[numPoint * 3    ];
                m_internPosition[i * 3 + 1] = 
                    coordArray[numPoint * 3 + 1];
                m_internPosition[i * 3 + 2] =
                    coordArray[numPoint * 3 + 2]; 
            }
        } else if (system.emitter.getType() == X3dSurfaceEmitterType.type) {
            X3dSurfaceEmitter emit = (X3dSurfaceEmitter)system.emitter;
            m_mass = emit.mass;
            speed = (m_rand.nextFloat()) * 
                    (emit.variation / 2.0f + 1) * emit.speed;
            if (!once) {
                System.err.println("Sorry, C/C++/java export of SurfaceEmitter is not supported");
                once = true;
            }
            m_internPosition[i * 3    ] = 0;
            m_internPosition[i * 3 + 1] = 0;
            m_internPosition[i * 3 + 2] = 0;
        }

        if (!setInternVector) {
            m_internVector[3 * i] = speed * m_force[0] * 
                (float)Math.cos(angle) * (float)Math.cos(alpha);
            m_internVector[3 * i + 1] = speed * m_force[1] * 
                (float)Math.sin(angle);
            m_internVector[3 * i + 2] = speed * m_force[2] *
                (float)Math.cos(angle) * (float)Math.sin(alpha);
        }
        m_lifeTime[i] = (m_rand.nextFloat()) * 
                        (system.lifetimeVariation / 2.0f + 1) * 
                        system.particleLifetime;
        m_startTime[i] = getTimerTime();
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
               System.err.println("invalid PositionInterpolator: number keys do not match number values");
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
               System.err.println("invalid OrientationInterpolator: number keys do not match number values");
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
               System.err.println("invalid ColorInterpolator: number keys do not match number values");
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
               System.err.println("invalid ScalarInterpolator: number keys do not match number values");
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
        if (timeSensor == null)
            return false;
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
    
        if (currentTime > dataPtr.stopTime) {
            dataPtr.startTime = timeSensor.startTime;
            dataPtr.stopTime = timeSensor.startTime + timeSensor.cycleInterval;
        } 

        if (!timeSensor.loop) { 
            if (currentTime > dataPtr.stopTime) {
                timeSensor.fraction_changed = 1.0f;
                if (timeSensor.isActive) {
                    dataPtr.stopTime = currentTime;
                    timeSensor.isActive = false;
                }
                return false;
            } else
            if (currentTime < timeSensor.startTime) {
                return false;
            }
        }
    
        timeSensor.isActive = true;
                    
        callTime = currentTime - dataPtr.startTime;
        timeSensor.fraction_changed = (float)((callTime %  
                                               timeSensor.cycleInterval) / 
                                              timeSensor.cycleInterval);
        return true;
    }
}

class MyTouchSensorProcessEventCallback extends X3dTouchSensorProcessEventCallback 
{
    public boolean processEvent(X3dNode node, String event) {
        X3dTouchSensor touchSensor = (X3dTouchSensor)node;
        touchSensor.isActive = !x3d2.released;
        if (touchSensor == null)
            return false;
        if (!touchSensor.enabled)
            return false;
        return true;
    }
}

class MyPlaneSensorProcessEventCallback extends X3dPlaneSensorProcessEventCallback 
{
    public boolean processEvent(X3dNode node, String event) {
        X3dPlaneSensor planeSensor = (X3dPlaneSensor)node;
        if (planeSensor.maxPosition[0] > planeSensor.minPosition[0]) {
            if (planeSensor.translation_changed[0] > planeSensor.maxPosition[0])
                planeSensor.translation_changed[0] = planeSensor.maxPosition[0];
            if (planeSensor.translation_changed[0] < planeSensor.minPosition[0])
                planeSensor.translation_changed[0] = planeSensor.minPosition[0];
        }
        if (planeSensor.maxPosition[1] > planeSensor.minPosition[1]) {
            if (planeSensor.translation_changed[1] > planeSensor.maxPosition[1])
                planeSensor.translation_changed[1] = planeSensor.maxPosition[1];
            if (planeSensor.translation_changed[1] < planeSensor.minPosition[1])
                planeSensor.translation_changed[1] = planeSensor.minPosition[1];
        }
        return planeSensor.enabled;
    }
}

class MyCylinderSensorProcessEventCallback extends X3dCylinderSensorProcessEventCallback 
{
    public boolean processEvent(X3dNode node, String event) {
        X3dCylinderSensor cylinderSensor = (X3dCylinderSensor)node;
        if (cylinderSensor.maxAngle > cylinderSensor.minAngle) {
            if (cylinderSensor.rotation_changed[3] > cylinderSensor.maxAngle)
                cylinderSensor.rotation_changed[3] = cylinderSensor.maxAngle;
            if (cylinderSensor.rotation_changed[3] < cylinderSensor.minAngle)
                cylinderSensor.rotation_changed[3] = cylinderSensor.minAngle;
        }
        return cylinderSensor.enabled;
    }
}

class MySphereSensorProcessEventCallback extends X3dSphereSensorProcessEventCallback 
{
    public boolean processEvent(X3dNode node, String event) {
        X3dSphereSensor sphereSensor = (X3dSphereSensor)node;
        return sphereSensor.enabled;
    }
}

class MyProximitySensorProcessEventCallback extends X3dProximitySensorProcessEventCallback 
{
    public boolean processEvent(X3dNode node, String event) {
        X3dProximitySensor proximitySensor = (X3dProximitySensor)node;
        boolean enabled = proximitySensor.enabled;
        if ((proximitySensor.size[0] == 0) || 
            (proximitySensor.size[1] == 0) || 
            (proximitySensor.size[2] == 0))
            enabled = false;
        else {
            X3dViewpoint viewpoint = (X3dViewpoint)x3d.viewpoint1;
            float xmax =  proximitySensor.size[0] / 2 + proximitySensor.center[0];
            float xmin = -proximitySensor.size[0] / 2 + proximitySensor.center[0];
            float ymax =  proximitySensor.size[1] / 2 + proximitySensor.center[1];
            float ymin = -proximitySensor.size[1] / 2 + proximitySensor.center[1];
            float zmax =  proximitySensor.size[2] / 2 + proximitySensor.center[2];
            float zmin = -proximitySensor.size[2] / 2 + proximitySensor.center[2];
            if ((viewpoint.position != null) &&
                (viewpoint.position[0] > xmin) &&
                (viewpoint.position[1] < ymax) &&
                (viewpoint.position[1] > ymin) &&
                (viewpoint.position[2] < zmax) &&
                (viewpoint.position[2] > zmin)) {
               float rot[] = new float[4];
               x3d2.SFRotationFromEuler(rot, x3d.view_rotx, x3d.view_roty, x3d.view_rotz);
               proximitySensor.orientation_changed[0] = rot[0];
               proximitySensor.orientation_changed[1] = rot[1];
               proximitySensor.orientation_changed[2] = rot[2];
               proximitySensor.orientation_changed[3] = -rot[3];
    
               proximitySensor.position_changed[0] = 0;
               proximitySensor.position_changed[1] = 0;
               proximitySensor.position_changed[2] = -x3d.view_dist;
    
               float vec[]= new float[3];
               x3d2.multMatrix4Vec(vec, x3d.navigation_matrix, 
                                   proximitySensor.position_changed);
               proximitySensor.position_changed[0] = vec[0];
               proximitySensor.position_changed[1] = vec[1];
               proximitySensor.position_changed[2] = vec[2];
    /*
               proximitySensor.position_changed[0] = viewpoint.position[0];
               proximitySensor.position_changed[1] = viewpoint.position[1];
               proximitySensor.position_changed[2] = viewpoint.position[2];
               proximitySensor.orientation_changed[0] = viewpoint.orientation[0];
               proximitySensor.orientation_changed[1] = viewpoint.orientation[1];
               proximitySensor.orientation_changed[2] = viewpoint.orientation[2];
               proximitySensor.orientation_changed[3] = viewpoint.orientation[3];
    */
           } else
               enabled = false;
        }
        return enabled;
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
