/*libC++RWD Library for C++ Rendering of White_dune Data (in Development)*/

/* Copyright (c) Stefan Wolf, 2010. */
/* Copyright (c) J. "MUFTI" Scheurich, 2015-2019. */

/*
Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
3. The name of the author may not be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR
TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#define __USE_XOPEN 1 /* for M_PI */
#include <math.h>
#include <sys/time.h>
#include <string.h>
#include <setjmp.h>
#include <jpeglib.h>
#include <png.h>

#ifdef _WIN32
# define random(x) rand(x)
#endif

namespace CPPRWD
{
    void error(const char *errormsg);

    void IndexedFaceSetRender(X3dNode *data, void*);

    void IndexedFaceSetCreateNormals(X3dNode *data, void*);

    void ImageTextureRender(X3dNode *data, void*);

    void PixelTextureRender(X3dNode *data, void*);

    void PointLightRender(X3dNode *data, void*);

    void DirectionalLightRender(X3dNode *data, void*);

    void SpotLightRender(X3dNode *data, void*);

    void MaterialRender(X3dNode *data, void*);

    void TransformTreeRender(X3dNode *data, void *dataptr);

    void GroupTreeRender(X3dNode *data, void *dataptr);

    void ViewpointRender(X3dNode *data, void*);

    void SwitchTreeRender(X3dNode *data, void *dataptr);

    void HAnimHumanoidTreeRender(X3dNode *data, void *dataptr);

    void HAnimJointTreeRender(X3dNode *data, void *dataptr);

    void ParticleSystemTreeRender(X3dNode *data, void *dataptr);

    void NavigationInfoTreeRender(X3dNode *data, void *dataptr);

    bool TimeSensorSendEvents(X3dNode *data, const char *event, 
                              void* extraData);

    bool TouchSensorSendEvents(X3dNode *data, const char *event, 
                               void *extraData);

    bool PlaneSensorSendEvents(X3dNode *data, const char *event,
                               void *extraData);

    bool CylinderSensorSendEvents(X3dNode *data, const char *event, 
                                  void *extraData);
                                          
    bool SphereSensorSendEvents(X3dNode *data, const char *event, 
                                void *extraData);
                                          
    bool ProximitySensorSendEvents(X3dNode *data, const char *event, 
                                   void *extraData);
                                          
    bool CollisionSendEvents(X3dNode *data, const char *event, void *extraData);
                                          
    float interpolate(float t, float key, float oldKey,
                               float value, float oldValue);

    void accountInterpolator(float *target, float fraction,
                             float *keys, float *keyValues, int numKeys,
                             int stride, int rotation);

    bool PositionInterpolatorSendEvents(X3dNode *data, const char *event,
                                        void* extraData);

    bool OrientationInterpolatorSendEvents(X3dNode *data, const char *event,
                                           void* extraData);

    bool ColorInterpolatorSendEvents(X3dNode *data, const char *event,
                                     void* extraData);

    bool ScalarInterpolatorSendEvents(X3dNode *data, const char *event,
                                      void* extraData);

    bool CoordinateInterpolatorSendEvents(X3dNode *data, const char *event,
                                          void* extraData);

    bool NormalInterpolatorSendEvents(X3dNode *data, const char *event,
                                      void* extraData);

    bool PositionInterpolator2DSendEvents(X3dNode *data, const char *event,
                                          void* extraData);

    void draw(bool render);

    void processEvents();

    void processHits(GLint hits, GLuint *pickBuffer);

    void setMouseClick(int x, int y);

    void setMouseRelease(int x, int y);

    void setMousePosition(int x, int y);

    void setMouseMove(int x, int old_x, int y, int old_y);

    bool hasHit(void);

    int getNavigation(void);

    float getNavigationSpeed(void);

    void setWidthHeight(int width, int height);

    void init();

    void finalize();

    int allocLight();

    float navigate(int x, int y, float z);

    float getInitialDist(void);

    bool distInitialised(void);

    void startWalking(void);
 
    void walkCamera(float *walk);

    void orbitCamera(float dtheta, float dphi, float z);
};

#define Z_NEAR 0.05
#define Z_FAR 7000.0

#define EPSILON 1E-9

#define NAV_EXAMINE 1
#define NAV_WALK    2
#define NAV_FLY     3
#define NAV_NONE    4
#define NAV_ANY     5

static int navigation = NAV_EXAMINE;

int CPPRWD::getNavigation(void)
{
    return navigation;
}

static float speed = 1;

float CPPRWD::getNavigationSpeed(void)
{
    return speed;
}

static bool viewpointRendered = false;
static bool viewPointExists = false;

static X3dViewpoint *viewpoint1 = NULL;

static short lightExists = 0;
static int numLights = 0;

static double projectionMatrix[16];
static double fieldOfViewdegree = 45;

static bool preRender = false;
static bool initRender = false;

static X3dSceneGraph scenegraph;

static int mouseX = -1;
static int mouseY = -1;
static bool clicked = false;
static bool released = false;

void CPPRWD::setMouseClick(int x, int y)
{
     mouseX = x;
     mouseY = y;
     clicked = true;
     released = false;
}

void CPPRWD::setMouseRelease(int x, int y)
{
     mouseX = x;
     mouseY = y;
     released = true;
}

void CPPRWD::setMousePosition(int x, int y)
{
     mouseX = x;
     mouseY = y;
}

static int mouseXMove = -1;
static int mouseXOld = -1;
static int mouseYMove = -1;
static int mouseYOld = -1;
static bool moved = false;

void CPPRWD::setMouseMove(int x, int x_old, int y, int y_old)
{
    mouseXMove = x;
    mouseXOld = x_old;
    mouseYMove = y;
    mouseYOld = y_old;
    moved = true;
}

static int width;
static int height;

void CPPRWD::setWidthHeight(int w, int h)
{
    width = w;
    height = h;
}


static bool stop = false;

static void startNavigation(void)
{
    stop = false;
}

static void stopNavigation(void)
{
    stop = true;
}

static float navigationMatrix[16];

float CPPRWD::navigate(int x, int y, float z)
{
    float walk[3];
    static float xOld = x;
    static float yOld = y;
    static float zOld = z;

    switch(getNavigation()) {
      case NAV_ANY:
      case NAV_WALK:
        startWalking();
        walk[0] = -x / 5.0;
        walk[1] = 0;
        if ((!stop) || (y >= 0)) { 
            walk[2] = y / 5.0;
            stop = false;
        } else
            walk[2] = 0;
        walkCamera(walk);
        break;
      case NAV_EXAMINE:
        if ((!stop) || (z - zOld > 0)) {
            orbitCamera(x / 5.0, y / 5.0, z);
            stop = false;
        } else {
            orbitCamera(xOld / 5.0, yOld / 5.0, zOld);
            return zOld;
        }
        break;
      case NAV_FLY:
        if ((!stop) || (z - zOld > 0)) { 
            orbitCamera(x / 5.0, y / 5.0, z - zOld);
            stop = false;
        } else
            orbitCamera(xOld / 5.0, yOld / 5.0, 0);
        break;
    }
    if (!stop) {
        xOld = x;
        yOld = y;
        zOld = z;
    }
    return z;
}

float CPPRWD::getInitialDist(void)
{
    if (viewpoint1)
        return viewpoint1->position[2];
    return -10;
}

bool CPPRWD::distInitialised(void)
{
    return (viewpoint1 != NULL);
}

bool isHit = false;

bool CPPRWD::hasHit(void)
{
     return isHit;
}

static double getTimerTime(void)
{
#ifdef WIN32
    struct _timeb t;
    _ftime(&t);
    return (double) t.time + t.millitm / 1000.0;
#else
    struct timeval t;
    gettimeofday(&t, NULL);
    return (double) t.tv_sec + (double) t.tv_usec / 1000000.0;
#endif
}

static float mod1(float x) 
{
    if (x == 0)
        return 0;
    if (x - floorf(x) == 0)
        return 1;
    return x - floorf(x);
}

static void normalizeAxis(float* value)
{
    float rlen = (float) sqrt(value[0] * value[0] + 
                              value[1] * value[1] +  
                              value[2] * value[2] );  
    if (rlen > 0.000000001f) { 
        rlen = 1.0f / rlen; 
        value[0] *= rlen;
        value[1] *= rlen;
        value[2] *= rlen;
    }
}

static void normalizeQuaternion(float* value)
{
    float rlen = (float) sqrt(value[0] * value[0] + 
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

static void SFRotation2quaternion(float* ret, float *rot)
{
    float s = (float) sin(rot[3] * 0.5f);
    ret[0] = rot[0];
    ret[1] = rot[1];
    ret[2] = rot[2];
    normalizeAxis(ret);
    for (int i = 0; i < 3; i++)
        ret[i] = ret[i] * s;
    ret[3] = (float) cos(rot[3] * 0.5);

    normalizeQuaternion(ret);
}

static void quaternion2SFRotation(float* ret, float *q)
{
    float s = 2 * (float) asin(mod1(q[3]));
    ret[0] = q[0];
    ret[1] = q[1];
    ret[2] = q[2];
    for (int i = 0; i < 3; i++)
        ret[i] = ret[i] * s;
    ret[3] = 2 * (float) acos(mod1(q[3]));
}

void norm_quaternion_from_rotation(float *ret, float *rot)
{
    SFRotation2quaternion(ret, rot);
}

void quaternionMult(float *ret, float *q1, float *q2)
{
    ret[0] = q2[3] * q1[0] + q2[0] * q1[3] + q2[1] * q1[2] - q2[2] * q1[1];
    ret[1] = q2[3] * q1[1] + q2[1] * q1[3] + q2[2] * q1[0] - q2[0] * q1[2];
    ret[2] = q2[3] * q1[2] + q2[2] * q1[3] + q2[0] * q1[1] - q2[1] * q1[0];
    ret[3] = q2[3] * q1[3] - q2[0] * q1[0] - q2[1] * q1[1] - q2[2] * q1[2];
}

static void quaternionMultVec(float *ret, float *q, float *vec)
{
    float quat[4];
    quat[0] = -q[0];
    quat[1] = -q[1];
    quat[2] = -q[2];
    quat[3] =  q[3];
    float quat2[4];
    quat2[0] = vec[0];
    quat2[1] = vec[1];
    quat2[2] = vec[2];
    quat2[3] = 0;
    float quat3[4];
    quaternionMult(quat3, quat, quat2);
    quat[0] = q[0];
    quat[1] = q[1];
    quat[2] = q[2];
    quat[3] = q[3];
    float quat4[4];
    quaternionMult(quat4, quat3, quat);
    ret[0] = quat4[0];
    ret[1] = quat4[1];
    ret[2] = quat4[2]; 
}


static void quaternionFromEuler(float *ret, float yaw, float pitch, float roll)
{
    double cy = cos(yaw * 0.5);
    double sy = sin(yaw * 0.5);
    double cp = cos(pitch * 0.5);
    double sp = sin(pitch * 0.5);
    double cr = cos(roll * 0.5);
    double sr = sin(roll * 0.5);

    ret[0] = cy * cp * sr - sy * sp * cr;
    ret[1] = sy * cp * sr + cy * sp * cr;
    ret[2] = sy * cp * cr - cy * sp * sr;
    ret[3] = cy * cp * cr + sy * sp * sr;

    normalizeQuaternion(ret);
}


static void SFRotationFromEuler(float *ret, float rotx, float roty, float rotz)
{
    float quat[4];
    quaternionFromEuler(quat, rotz * 2 * M_PI / 360.0, 
                              roty * 2 * M_PI / 360.0, 
                              rotx * 2 * M_PI / 360.0);
    quaternion2SFRotation(ret, quat);
}

static void multMatrix4Vec(float *ret, float *mat, float* vec)
{
    ret[0] = mat[0] * vec[0] + mat[1] * vec[1] + mat[2] * vec[2];
    ret[1] = mat[4] * vec[0] + mat[5] * vec[1] + mat[6] * vec[2];
    ret[2] = mat[8] * vec[0] + mat[9] * vec[1] + mat[10] * vec[2];
}

static double oldWalkTime = 0;

void CPPRWD::startWalking(void)
{
    oldWalkTime = getTimerTime();
}

void CPPRWD::walkCamera(float *walk)
{
    float pos[3];
    pos[0] = viewpoint1->position[0];
    pos[1] = viewpoint1->position[1];
    pos[2] = viewpoint1->position[2];
    float sfrot[4];
    sfrot[0] = viewpoint1->orientation[0];
    sfrot[1] = viewpoint1->orientation[1];
    sfrot[2] = viewpoint1->orientation[2];
    sfrot[3] = viewpoint1->orientation[3];
    float rot[4];
    SFRotation2quaternion(rot, sfrot);
    float fspeed = getNavigationSpeed();
    double dt = getTimerTime() - oldWalkTime;

    if ((oldWalkTime > 0) && (dt > 0)) {
        float sfaround[4];
        sfaround[0] = 0;
        sfaround[1] = 1;
        sfaround[2] = 0;
        sfaround[3] = -(walk[0] * 2.0f) * 2.0f * M_PI / 360.0f;
        float around[4];
        SFRotation2quaternion(around, sfaround);
        float newRot[4];
        quaternionMult(newRot, around, rot);
        normalizeQuaternion(newRot);
        normalizeQuaternion(rot);
        float sfnewRot[4];
        quaternion2SFRotation(sfnewRot, newRot);
        viewpoint1->orientation[0] = sfnewRot[0];
        viewpoint1->orientation[1] = sfnewRot[1];
        viewpoint1->orientation[2] = sfnewRot[2];
        viewpoint1->orientation[3] = sfnewRot[3];
        float z = walk[2] * 2.0f * fspeed;
        float vec[3];
        vec[0] = 0;
        vec[1] = 0;
        vec[2] = -z;
        float pos2[3];
        quaternionMultVec(pos2, rot, vec);
        viewpoint1->position[0] = pos[0] + pos2[0];        
        viewpoint1->position[1] = pos[1] + pos2[1];        
        viewpoint1->position[2] = pos[2] + pos2[2];        
    }
}

void CPPRWD::orbitCamera(float dtheta, float dphi, float z)
{    
    float pos[3];
    pos[0] = viewpoint1->position[0];
    pos[1] = viewpoint1->position[1];
    pos[2] = viewpoint1->position[2];
    float sfrot[4];
    sfrot[0] = viewpoint1->orientation[0];
    sfrot[1] = viewpoint1->orientation[1];
    sfrot[2] = viewpoint1->orientation[2];
    sfrot[3] = viewpoint1->orientation[3];
    float rot[4];
    SFRotation2quaternion(rot, sfrot);
    float fspeed = getNavigationSpeed();
    float sfup[4];
    sfup[0] = 0;
    sfup[1] = 1;
    sfup[2] = 0;
    sfup[3] = dtheta * 2.0f * M_PI / 360.0f;
    float up[4];
    SFRotation2quaternion(up, sfup);
    float sfaround[4];
    sfaround[0] = 1;
    sfaround[1] = 0;
    sfaround[2] = 0;
    sfaround[3] = dphi * 2.0f * M_PI / 360.0f;
    float around[4];
    SFRotation2quaternion(around, sfaround);
    float q[4];
    quaternionMult(q, around, rot);
    float newRot[4];
    quaternionMult(newRot, up, q);
    float sfnewRot[4];
    quaternion2SFRotation(sfnewRot, newRot);
    
    float zAxis[3];
    zAxis[0] = 0;
    zAxis[1] = 0;
    zAxis[2] = z;
    float dist[3];
    quaternionMultVec(dist, rot, zAxis);
    float *newPos = dist;
    if (navigation == NAV_EXAMINE) {
        viewpoint1->position[0] = newPos[0];        
        viewpoint1->position[1] = newPos[1];        
        viewpoint1->position[2] = newPos[2];        
    } else {
        viewpoint1->position[0] += newPos[0];        
        viewpoint1->position[1] += newPos[1];        
        viewpoint1->position[2] += newPos[2];        
     }
     viewpoint1->orientation[0] = sfnewRot[0];
     viewpoint1->orientation[1] = sfnewRot[1];
     viewpoint1->orientation[2] = sfnewRot[2];
     viewpoint1->orientation[3] = sfnewRot[3];
}


void CPPRWD::error(const char *errormsg)
{
    #ifdef WIN32
    MessageBox(NULL, errormsg, NULL, MB_OK);
    #else
    fprintf(stderr, "%s\n", errormsg);
    #endif
}

int CPPRWD::allocLight()
{
    GLint maxLights;
    glGetIntegerv(GL_MAX_LIGHTS, &maxLights);
    
    if (numLights >= maxLights) {
        error("too many lights!\n");
        return GL_LIGHT0;
    }
    
    return (GL_LIGHT0 + numLights++);
}

void CPPRWD::IndexedFaceSetRender(X3dNode *data, void* extraData)
{
    if(preRender)
    {
    }
    else if(initRender)
    {
    }
    else
    {
        X3dIndexedFaceSet *Xindexedfaceset = (X3dIndexedFaceSet*)data;
        X3dCoordinate *Xcoordinate = (X3dCoordinate*)Xindexedfaceset->coord;
        X3dNormal *Xnormal = (X3dNormal*)Xindexedfaceset->normal;
        bool colorRGBA = false;
        X3dNode *Xcolor = NULL;
        if (Xindexedfaceset->color) 
            if (Xindexedfaceset->color->getType() == X3dColorRGBAType) {
                Xcolor = (X3dColorRGBA*)Xindexedfaceset->color;
                colorRGBA = true;
            } else
                Xcolor = (X3dColor*)Xindexedfaceset->color;;
        X3dTextureCoordinate *Xtexturecoordinate = (X3dTextureCoordinate*)Xindexedfaceset->texCoord;
        GLint *faces = NULL;
        GLfloat *vertex = NULL;
        GLint *normalindex = NULL;
        GLfloat *normal = NULL;
        GLfloat *colors = NULL;
        GLint *colorindex = NULL;
        GLint *texturecoordinateindex = NULL;
        int faces_len, vertex_len, normalindex_len, normal_len, color_len, colorindex_len, texturecoordinateindex_len;
        bool normalpervertex, colorpervertex;
        if(Xcolor != NULL)
        {
            glEnable(GL_COLOR_MATERIAL); //Maybe needfull
            glColorMaterial(GL_FRONT_AND_BACK, GL_DIFFUSE);
            if (colorRGBA) {
                colors = ((X3dColorRGBA *)Xcolor)->color;
                color_len = ((X3dColorRGBA *)Xcolor)->color_length;
            } else {
                colors = ((X3dColor *)Xcolor)->color;
                color_len = ((X3dColor *)Xcolor)->color_length;
            }
            colorpervertex = Xindexedfaceset->colorPerVertex;
        }
        if(Xindexedfaceset->colorIndex != NULL)
        {
            colorindex = Xindexedfaceset->colorIndex;
            colorindex_len = Xindexedfaceset->colorIndex_length;
        }
        if(Xindexedfaceset->texCoordIndex != NULL)
        {
            texturecoordinateindex = Xindexedfaceset->texCoordIndex;
            texturecoordinateindex_len = Xindexedfaceset->texCoordIndex_length;
        }
        vertex = Xcoordinate->point;
        faces = Xindexedfaceset->coordIndex;
        normalindex = Xindexedfaceset->normalIndex;
        if(Xnormal!=NULL)normal = Xnormal->vector;
        if (extraData != NULL)
            normal = (float *)extraData;
        vertex_len = Xcoordinate->point_length;
        faces_len = Xindexedfaceset->coordIndex_length;
        if(Xnormal != NULL)normal_len = Xnormal->vector_length;
        normalindex_len = Xindexedfaceset->normalIndex_length;
        normalpervertex = Xindexedfaceset->normalPerVertex;
        int buffer, normalbuffer, texturebuffer = 0;
        int facecounter = 0;
        glPushName(Xindexedfaceset->glName_number);
        if (Xindexedfaceset->ccw != 0)
        {
           glFrontFace(GL_CCW);
        }
        else
        {
           glFrontFace(GL_CW);
        }
        if (Xindexedfaceset->solid == 0) 
        {
           glDisable(GL_CULL_FACE);
           glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
        } 
        else 
        {
           glEnable(GL_CULL_FACE);
           glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);
        }

        glBegin(GL_POLYGON);
        if(colors && !colorpervertex)
            if (colorRGBA)
                glColor4f(colors[4 * facecounter], colors[4 * facecounter + 1], colors[4 * facecounter + 2], colors[4 * facecounter + 3]);
            else 
                glColor3f(colors[3 * facecounter], colors[3 * facecounter + 1], colors[3 * facecounter + 2]);

        for(int i = 0; i != faces_len; i++)
        {
            buffer = faces[i];
            if (buffer < 0) {
                glEnd();
                if (i != faces_len - 1)
                    glBegin(GL_POLYGON);
            }

            if(normalindex != NULL)
                normalbuffer = normalindex[i];
            else 
                normalbuffer = buffer;
            if(texturecoordinateindex != NULL)
                texturebuffer = texturecoordinateindex[i];
            else
                texturebuffer = buffer;
            if(buffer == -1 && (normalbuffer == -1 || normalindex != NULL))
            {
                facecounter++;
                glEnd();
                glBegin(GL_POLYGON);
                if(colors && !colorpervertex)
                    if (colorRGBA)
                        glColor4f(colors[4 * facecounter], colors[4 * facecounter + 1], colors[4 * facecounter + 2], colors[4 * facecounter + 3]);
                    else
                        glColor3f(colors[3 * facecounter], colors[3 * facecounter + 1], colors[3 * facecounter + 2]);
            }
            else if(((buffer == -1 && normalbuffer != -1) || (buffer != -1 && normalbuffer == -1)) && normalindex != NULL)
                error("Error in normalIndex");
            else
            {
                if(normalpervertex && normal != NULL)
                    glNormal3f(normal[normalbuffer*3], normal[normalbuffer*3+1], normal[normalbuffer*3+2]);
                if(colors && color_len > 0 && colorpervertex)
                    if (colorRGBA)
                        glColor4f(colors[buffer*4], colors[buffer*4+1], colors[buffer*4+2], colors[buffer*4+3]);
                    else
                        glColor3f(colors[buffer*3], colors[buffer*3+1], colors[buffer*3+2]);
                if(Xtexturecoordinate)
                    if((Xtexturecoordinate->point) && (texturecoordinateindex))
                        glTexCoord2f(Xtexturecoordinate->point[texturebuffer*2], Xtexturecoordinate->point[texturebuffer*2+1]);
                glVertex3f(vertex[buffer*3], vertex[buffer*3+1], vertex[buffer*3+2]);
            }
        }
        glEnd();
        glPopName();
    }
    glDisable(GL_COLOR_MATERIAL);
}

void CPPRWD::IndexedFaceSetCreateNormals(X3dNode *data, void* extraData)
{
    if(initRender)
    {
        X3dIndexedFaceSet *Xindexedfaceset = (X3dIndexedFaceSet*)data;
        X3dCoordinate *Xcoordinate = (X3dCoordinate*)Xindexedfaceset->coord;
        X3dNormal *Xnormal = (X3dNormal*)Xindexedfaceset->normal;
        int *normalIndex = Xindexedfaceset->normalIndex;
        int normalIndexLen = Xindexedfaceset->normalIndex_length;
        if (normalIndex == NULL) {
            normalIndex = Xindexedfaceset->coordIndex;
            normalIndexLen = Xindexedfaceset->coordIndex_length;
        } 
        int maxLen = -1;
        for (int i = 0; i < normalIndexLen; i++)
            if (normalIndex[i] > maxLen)
                maxLen = normalIndex[i];
        maxLen++;
        float **vec = (float **)extraData;
        *vec = (float *)malloc(maxLen * sizeof(float) * 3);
    }
    else
    {
        X3dIndexedFaceSet *Xindexedfaceset = (X3dIndexedFaceSet*)data;
        X3dCoordinate *Xcoordinate = (X3dCoordinate*)Xindexedfaceset->coord;
        X3dNormal *Xnormal = (X3dNormal*)Xindexedfaceset->normal;
        int *normalIndex = Xindexedfaceset->normalIndex;
        if (normalIndex == NULL)
            normalIndex = Xindexedfaceset->coordIndex;
        float *vec = *(float **)extraData;
        float creaseAngle = Xindexedfaceset->creaseAngle;
        int *ci = Xindexedfaceset->coordIndex;
        int ciLen = Xindexedfaceset->coordIndex_length;
        float *points = Xcoordinate->point;
        int pointLen = Xcoordinate->point_length;
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
    
                    float vLen = sqrt(crossx * crossx + crossy * crossy + crossz * crossz);
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

static bool not2PowN(int number)
{
    bool ret = true;
    for (int i = 0; i < 32; i++)
        if (number == (1 << i))
        {
            ret = false;
            break;
        }
    return ret;
}

static void textureTransformBind(X3dNode *data)
{
    int i;
    X3dTextureTransform *texTransform = (X3dTextureTransform *)data;
    if(preRender)
    {
    }
    else if(initRender)
    {
    }
    else {
        const float *fcenter = texTransform->center;
        const float *fscale = texTransform->scale;
        const float *ftranslation = texTransform->translation;

        glPushMatrix();
        glMatrixMode(GL_TEXTURE);
        glLoadIdentity();
        glTranslatef(-fcenter[0], -fcenter[1], 0.0f);
        glScalef(fscale[0], fscale[1], 1.0f);
        glRotatef(texTransform->rotation * 180.0f / M_PI, 0.0f, 0.0f, 1.0f);
        glTranslatef(fcenter[0], fcenter[1], 0.0f);
        glTranslatef(ftranslation[0], ftranslation[1], 0.0f);
        glMatrixMode(GL_MODELVIEW);
    }
}

static void textureTransformUnBind(void)
{
    glPopMatrix();
}

static void renderIntTexture(GLuint *textureName, int height, int width, 
                             int format, GLuint *data, 
                             bool repeatS, bool repeatT)
{
    if (*textureName == 0) {
        glGenTextures(1, textureName);
        glBindTexture(GL_TEXTURE_2D, *textureName);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, 
                     GL_UNSIGNED_BYTE, data);
    } else {
        glBindTexture(GL_TEXTURE_2D, *textureName);
    }
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, 
                    repeatS ? GL_REPEAT : GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, 
                    repeatT ? GL_REPEAT : GL_CLAMP);
    glEnable(GL_TEXTURE_2D);
}

static void renderByteTexture(GLuint *textureName, int height, int width, 
                              int format, GLbyte *data,
                              int repeatS, int repeatT)
{
    if (*textureName == 0) {
        glGenTextures(1, textureName);
        glBindTexture(GL_TEXTURE_2D, *textureName);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glPixelStorei(GL_PACK_ALIGNMENT, 1);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, 
                     GL_UNSIGNED_BYTE, data);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    } else {
        glBindTexture(GL_TEXTURE_2D, *textureName);
    }
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, 
                    repeatS ? GL_REPEAT : GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, 
                    repeatT ? GL_REPEAT : GL_CLAMP);
    glEnable(GL_TEXTURE_2D);
}

static int errorFlag = 0;
static jmp_buf jpegEnv;

struct ImageTextureExtraDataStruct {
    unsigned int height;
    unsigned int width;
    unsigned int colorMode;
    unsigned char *pixels;
    GLuint *textureName;
};

static void CRWDJpgErrorExit(j_common_ptr cinfo)
{
    longjmp(jpegEnv,1);
    errorFlag = 1;
}

static int CRWDJpgLoad(struct jpeg_decompress_struct *cinfo, FILE *file, 
                       unsigned int *width, unsigned int *height, 
                       unsigned int* components)
{
    static struct jpeg_error_mgr jpgError;

    errorFlag=0;
    if (setjmp(jpegEnv)!=0)
    {
        errorFlag = 1;
        return 1;
    }
    cinfo->err = jpeg_std_error(&jpgError);
    cinfo->err->error_exit = CRWDJpgErrorExit;
    jpeg_create_decompress(cinfo);

    jpeg_stdio_src(cinfo, file);
    jpeg_read_header(cinfo, 1);
    jpeg_start_decompress(cinfo);

    *width = cinfo->output_width;
    *height = cinfo->output_height;
    *components = cinfo->output_components;

    return errorFlag;
}

static int CRWDJpgRead(struct jpeg_decompress_struct *cinfo, 
                       unsigned char *pixels)
{
    JSAMPARRAY buffer;
    int rowStride;
    unsigned char *ptr;
    int buffer_height = 1;

    rowStride = cinfo->output_width * cinfo->output_components;
    buffer = (JSAMPARRAY)malloc(sizeof(JSAMPROW) * buffer_height);
    buffer[0] = (JSAMPROW)malloc(sizeof(JSAMPLE) * rowStride);
    
    ptr = pixels + rowStride * (cinfo->output_height - 1);
    while (cinfo->output_scanline < cinfo->output_height) 
    {
        jpeg_read_scanlines(cinfo, buffer, 1);
        memcpy(ptr, buffer[0], rowStride);
        ptr -= rowStride;
    }

    jpeg_finish_decompress(cinfo);
    jpeg_destroy_decompress(cinfo);
    free(buffer[0]);
    free(buffer);
    return errorFlag;
}

static double getGammaExp()
{
    static double defaultExponent = 2.2;
    static int set = 0;

    if (!set)
    {
#if defined(NeXT)
        defaultExponent = 1.0;
#elif defined(__sgi)
        defaultExponent = 1.7;
#elif defined(MACOSX)
        defaultExponent = 1.5;
#endif
        set = 1;
    }
    return defaultExponent;
}
#ifndef png_jmpbuf 
#define png_jmpbuf(png_ptr) ((png_ptr)->jmpbuf) 
#endif 

#define PNG_SIGNATURE_SIZE 8

static int CRWDPngLoad(FILE *f, png_structp *pngPtr, png_infop *infoPtr,
                       unsigned int *width, unsigned int *height, 
                       unsigned int *components)
{
    unsigned char signature[PNG_SIGNATURE_SIZE];
    
    int bitDepth;
    int colorType;
    int interlaceType;
    int grayPalette;

    fread(signature, 1, PNG_SIGNATURE_SIZE, f);
    if (!png_check_sig(signature, PNG_SIGNATURE_SIZE))
    {
        errorFlag = 1;
        fclose(f);
        return 1;
    }
    *pngPtr = png_create_read_struct(PNG_LIBPNG_VER_STRING, 0, 0, 0);    
    if (*pngPtr == NULL)
    {
        errorFlag = 1;
        fclose(f);
        return 1;
    }
    *infoPtr = png_create_info_struct(*pngPtr);
    if (*infoPtr == NULL)
    {
        errorFlag = 1;
        png_destroy_read_struct(pngPtr, NULL, NULL);
        fclose(f);
        return 1;
    }
    if (setjmp(png_jmpbuf(*pngPtr)))
    {
        png_destroy_read_struct(pngPtr, infoPtr, NULL);
        errorFlag = 1;
        fclose(f);
        return 1;
    }
#ifndef PNG_NO_STDIO
    png_init_io(*pngPtr, f);
#endif
    png_set_sig_bytes(*pngPtr, PNG_SIGNATURE_SIZE);
    png_read_info(*pngPtr, *infoPtr);
    png_get_IHDR(*pngPtr, *infoPtr, width, height, &bitDepth, &colorType,
                 &interlaceType, NULL, NULL);
    png_set_strip_16(*pngPtr);
    png_set_packing(*pngPtr);
    *components = png_get_channels(*pngPtr, *infoPtr);

    if (colorType == PNG_COLOR_TYPE_PALETTE)
    {
        png_set_expand(*pngPtr);
        *components = 3;
    }

    if ((colorType == PNG_COLOR_TYPE_GRAY) && (bitDepth < 8))
        png_set_expand(*pngPtr);

    if (png_get_valid(*pngPtr, *infoPtr, PNG_INFO_tRNS))
    {
      png_set_expand(*pngPtr);
      *components = *components + 1;
    }
    
    grayPalette = 0;
    if (colorType == PNG_COLOR_TYPE_PALETTE)
    {
        int n;
        int numPalette;
        png_colorp palette;

        if (png_get_PLTE(*pngPtr, *infoPtr, &palette, &numPalette))
        {
          grayPalette = 1;
          for (n = 0; n < numPalette; ++n)
            if ((palette[n].red != palette[n].green) ||
                (palette[n].red != palette[n].blue))
            {
                grayPalette = 0;
                break;
            }
        }
    }
    
    {
        double gamma;
        double exponent = getGammaExp();
        if (png_get_gAMA(*pngPtr, *infoPtr, &gamma))
            png_set_gamma(*pngPtr, exponent, gamma);
        else
            png_set_gamma(*pngPtr, exponent, 0.45455);
    }

    png_read_update_info(*pngPtr, *infoPtr);
    png_get_IHDR(*pngPtr, *infoPtr, width, height, &bitDepth, &colorType,
                 &interlaceType, NULL, NULL);
    return 0;
}

static void CRWDPngRead(png_structp *pngPtr, png_infop *infoPtr,
                        struct ImageTextureExtraDataStruct *data, 
                        unsigned int *components)
{
    int rowStride = *components * data->width;
    png_bytep *rowPtrs;
    int row;
    unsigned char *ptr;
    int i;

    rowPtrs = (png_bytep *) malloc(sizeof(png_bytep) * data->height);

    for (row = 0; row < data->height; row++)
    {
        rowPtrs[row] = (png_bytep)png_malloc(*pngPtr, 
                            png_get_rowbytes(*pngPtr, *infoPtr));
    }

    png_read_image(*pngPtr, rowPtrs);

    ptr = data->pixels + rowStride * (data->height - 1);
    for (i = 0; i < data->height; i++)
    {
        memcpy(ptr, rowPtrs[i], rowStride);
        ptr -= rowStride;
    }
    png_destroy_read_struct(pngPtr, infoPtr, NULL);
    free(rowPtrs);
}

void CPPRWD::ImageTextureRender(X3dNode *data, void *)
{
    int i;
    struct ImageTextureExtraDataStruct *extraVar;

    struct X3dImageTexture *imageTexture = (struct X3dImageTexture*) data;
    extraVar = (struct ImageTextureExtraDataStruct *)imageTexture->extra_data;
    if(initRender || (extraVar == NULL))
    {
        int i;
        const char *filename;
        unsigned int components;
        struct jpeg_decompress_struct cinfo;
        png_structp pngPtr;
        png_infop infoPtr;
        FILE *file;

        imageTexture->extra_data = malloc(sizeof(struct 
                                                 ImageTextureExtraDataStruct));
        extraVar = (struct ImageTextureExtraDataStruct *)
                   imageTexture->extra_data;

        extraVar->textureName = new GLuint[1];
        *(extraVar->textureName) = 0;

        filename = *(imageTexture->url);
#ifdef PICTURE_PATH
        char *name = (char *)filename;
        char *pathName = (char *)
                         malloc(strlen(PICTURE_PATH) + strlen(name) + 2);
        strcpy(pathName, PICTURE_PATH);
        strcat(pathName, "/");
        strcat(pathName, name);
        filename = pathName;
#endif
        if (!filename)
            return;
        else if (strlen(filename) == 0)
            return;
        else if (!(file = fopen(filename, "rb")))
            return;

        /* try to handle file as jpeg file */
        CRWDJpgLoad(&cinfo, file, &(extraVar->width), &(extraVar->height), 
                    &components);
        if (!errorFlag)
        {
            extraVar->pixels = (unsigned char *)malloc(components *
                                   extraVar->width * extraVar->height);
            if (extraVar->pixels == NULL)
                return;        

            CRWDJpgRead(&cinfo, extraVar->pixels);
            extraVar->colorMode = GL_RGB;
        }
        fclose(file);
        if (errorFlag) 
        { 
            /* try to handle file as PNG file) */
            file = fopen(filename, "rb");
            if (!CRWDPngLoad(file, &pngPtr, &infoPtr, 
                             &(extraVar->width), &(extraVar->height),
                             &components))
            {
                extraVar->pixels = (unsigned char *)malloc(components *
                                       extraVar->width * extraVar->height);
                if (extraVar->pixels == NULL)
                     return;
                 CRWDPngRead(&pngPtr, &infoPtr, extraVar, &components);
            }
            fclose(file);
        }

        extraVar->colorMode = GL_RGBA;
        switch (components)
        {
          case 1:
            extraVar->colorMode = GL_LUMINANCE;
            break;
          case 2:
            extraVar->colorMode = GL_LUMINANCE_ALPHA;
            break;
          case 3:
            extraVar->colorMode = GL_RGB;
            break;          
        }
        if (not2PowN(extraVar->width))
            error("warning: texture width is not 2 pow N");            
        if (not2PowN(extraVar->height))
            error("warning: texture height is not 2 pow N");            
        if (extraVar->width != extraVar->height)
            error("warning: texture width and height is not equal\n");
    } 
    else 
    {
        X3dNode *textureTransform = NULL;
        if (imageTexture->m_parent)
            textureTransform = ((struct X3dAppearance *)imageTexture->m_parent)->
                               textureTransform;

        if (textureTransform)
            textureTransformBind(textureTransform);
        glMatrixMode(GL_TEXTURE);
        if (extraVar)
            renderByteTexture(extraVar->textureName,
                              extraVar->width, extraVar->height, 
                              extraVar->colorMode, (GLbyte *)(extraVar->pixels),
                              imageTexture->repeatS, imageTexture->repeatT); 
        glMatrixMode(GL_MODELVIEW);
        if (textureTransform)
            textureTransformUnBind();
    }
}

void CPPRWD::PixelTextureRender(X3dNode *data, void*)
{
    X3dPixelTexture *pixelTexture = (X3dPixelTexture*) data;
    GLuint *textureName = (GLuint *) pixelTexture->extra_data;
    if(preRender)
    {
    }
    else if(initRender)
    {
        int bigEndian = 1;
        bigEndian = (*(unsigned char *)&bigEndian) == 0;

        pixelTexture->extra_data = malloc(sizeof(GLuint));
        textureName = (GLuint *) pixelTexture->extra_data;
        *textureName = 0;

        if (not2PowN(pixelTexture->image[0]))
            error("warning: texture width is not 2 pow N");            
        if (not2PowN(pixelTexture->image[1]))
            error("warning: texture height is not 2 pow N");            
        if (pixelTexture->image[0] != pixelTexture->image[1])
            error("warning: texture width and height is not equal\n");
        // store modified colors back to pixelTexture.image
        for (int i = 3; i < pixelTexture->image_length; i++) {
            int a = 0xff;
            int r,g,b;
            switch (pixelTexture->image[2])
            {
              case 4:
                r = (pixelTexture->image[i]&0xFF000000) >> 24;
                g = (pixelTexture->image[i]&0x00FF0000) >> 16;
                b = (pixelTexture->image[i]&0x0000FF00) >> 8;
                a = (pixelTexture->image[i]&0x000000FF);
                break;
              case 3:
                r = (pixelTexture->image[i]&0x00FF0000) >> 16;
                g = (pixelTexture->image[i]&0x0000FF00) >> 8;
                b = (pixelTexture->image[i]&0x000000FF);
                break;
              case 2:
                r = g = b = (pixelTexture->image[i]&0x0000FF00) >> 8;
                a = (pixelTexture->image[i]&0x000000FF);
                break;
              case 1:
                r = g = b = (pixelTexture->image[i]&0xFF);
                break;
            }
            if (bigEndian)
            {
                pixelTexture->image[i] = a | 
                                         (b << 8) | (g << 16) | (r << 24);
            }
            else
            {
                pixelTexture->image[i] = r | (g << 8) | (b << 16) | 
                                         (a << 24);
            }
        }
    }
    else {
        X3dNode *textureTransform = NULL;
        if (pixelTexture->m_parent)
            textureTransform = ((X3dAppearance *)pixelTexture->m_parent)->
                               textureTransform;
                
        if (textureTransform)
            textureTransformBind(textureTransform);
        renderIntTexture(textureName, 
                         pixelTexture->image[0], pixelTexture->image[1], 
                         GL_RGBA, (GLuint *)&pixelTexture->image[3], 
                         pixelTexture->repeatS, pixelTexture->repeatT); 
        if (textureTransform)
            textureTransformUnBind();
    }
}

void CPPRWD::PointLightRender(X3dNode *data, void* extraData)
{
    int lightNum = -1;
    X3dPointLight *lightNode = (X3dPointLight*)data;
    if(preRender && lightNode->on)
    {
        lightNum = *(int *)(lightNode->extra_data);
        glEnable(lightNum);
        X3dPointLight *light = (X3dPointLight *)data;
        GLfloat light_color[4] = {light->color[0]*light->intensity, light->color[1]*light->intensity, light->color[2]*light->intensity, 1};
        GLfloat light_ambient_color[4] = {light->color[0]*light->ambientIntensity, light->color[1]*light->ambientIntensity, light->color[2]*light->ambientIntensity, 1};
        GLfloat light_position[4] = {light->location[0], light->location[1], light->location[2], 
                                     extraData != NULL ? *((float *)extraData) : 0.0f};
        GLfloat light_attenuation[3] = {light->attenuation[0], light->attenuation[1], light->attenuation[2]};
        glLightfv(lightNum, GL_DIFFUSE, light_color);
        glLightfv(lightNum, GL_AMBIENT, light_ambient_color);
        glLightfv(lightNum, GL_POSITION, light_position);
        glLightf(lightNum, GL_CONSTANT_ATTENUATION, light_attenuation[0]);
        glLightf(lightNum, GL_LINEAR_ATTENUATION, light_attenuation[1]);
        glLightf(lightNum, GL_QUADRATIC_ATTENUATION, light_attenuation[2]);
        glLightf(lightNum, GL_SPOT_CUTOFF, 180.0f);
        glLightf(lightNum, GL_SPOT_EXPONENT, 0.0f);
    }
    else if(initRender)
    {
        lightExists = true;
        lightNode->extra_data = malloc(sizeof(int));
        *(int *)(lightNode->extra_data) = allocLight();
    }
    else
    {
    }
}

void CPPRWD::DirectionalLightRender(X3dNode *data, void* extraData)
{
    X3dDirectionalLight *lightNode = (X3dDirectionalLight*)data;
    int lightNum = -1;
    if(preRender && lightNode->on)
    {
        int i;
        GLfloat color[4];
        GLfloat ambient_color[4];
        GLfloat position[4];

        for (i = 0; i < 3; i++) 
            color[i] = lightNode->color[i] * lightNode->intensity;
        color[3] = 1;

        for (i = 0; i < 3; i++) 
            ambient_color[i] = lightNode->color[i] * lightNode->ambientIntensity;
        ambient_color[3] = 1;

        for (i = 0; i < 3; i++)
            position[i] = -lightNode->direction[i];
        position[3] = 0.0f;

        lightNum = *(int *)(lightNode->extra_data);
        glLightfv(lightNum, GL_AMBIENT, ambient_color);
        glLightfv(lightNum, GL_DIFFUSE, color);
        glLightfv(lightNum, GL_POSITION, position);
        glLightfv(lightNum, GL_SPECULAR, color);
        glLightf(lightNum, GL_SPOT_CUTOFF, 180.0f);
        glLightf(lightNum, GL_SPOT_EXPONENT, 0.0f);
        glLightf(lightNum, GL_CONSTANT_ATTENUATION, 1);
        glLightf(lightNum, GL_LINEAR_ATTENUATION, 0);
        glLightf(lightNum, GL_QUADRATIC_ATTENUATION, 0);
        glEnable(lightNum);
    }
    else if(initRender)
    {
        lightExists = -1;
        lightNode->extra_data = malloc(sizeof(int));
        *(int *)(lightNode->extra_data) = allocLight();
    }
}

void CPPRWD::SpotLightRender(X3dNode *data, void* extraData)
{
    X3dSpotLight *lightNode = (X3dSpotLight*)data;
    int lightNum = -1;
    if(preRender && lightNode->on)
    {
        int i;
        GLfloat color[4];
        GLfloat ambient_color[4];
        GLfloat position[4];
        GLfloat ambientIntensity[4];

        for (i = 0; i < 3; i++) 
            color[i] = lightNode->color[i] * lightNode->intensity;
        color[3] = 1;

        for (i = 0; i < 3; i++) 
            ambient_color[i] = lightNode->color[i] * lightNode->ambientIntensity;
        ambient_color[3] = 1;

        for (i = 0; i < 3; i++)
            position[i] = lightNode->direction[i];
        position[3] = 1.0f;

        lightNum = *(int *)(lightNode->extra_data);
        glLightfv(lightNum, GL_AMBIENT, ambient_color);
        glLightfv(lightNum, GL_DIFFUSE, color);
        glLightfv(lightNum, GL_POSITION, position);
        glLightfv(lightNum, GL_SPECULAR, color);
        glLightfv(lightNum, GL_SPOT_DIRECTION, lightNode->direction);
        glLightf(lightNum, GL_SPOT_CUTOFF, lightNode->cutOffAngle * 180.0f / M_PI);
        glLightf(lightNum, GL_SPOT_EXPONENT, lightNode->beamWidth < lightNode->cutOffAngle ? 1.0f : 0.0f);
        glLightf(lightNum, GL_CONSTANT_ATTENUATION, lightNode->attenuation[0]);
        glLightf(lightNum, GL_LINEAR_ATTENUATION, lightNode->attenuation[1]);
        glLightf(lightNum, GL_QUADRATIC_ATTENUATION, lightNode->attenuation[2]);
        glEnable(lightNum);
    }
    else if(initRender)
    {
        lightExists = -1;
        lightNode->extra_data = malloc(sizeof(int));
        *(int *)(lightNode->extra_data) = allocLight();
    }
}


void CPPRWD::MaterialRender(X3dNode *data, void*)
{
    if(preRender)
    {
    }
    else if(initRender)
    {
    }
    else
    {
        X3dMaterial *material = (X3dMaterial*)data;
        GLfloat diffuse_color[4] = {material->diffuseColor[0], material->diffuseColor[1], material->diffuseColor[2], 1.0f};
        GLfloat ambient = material->ambientIntensity;
        GLfloat ambient_color[4] = {ambient * material->diffuseColor[0], ambient * material->diffuseColor[1], ambient * material->diffuseColor[2], 1};
        GLfloat emissive_color[4] = {material->emissiveColor[0], material->emissiveColor[1], material->emissiveColor[2], 1.0f};
        GLfloat specuar_color[4] = {material->specularColor[0], material->specularColor[1], material->specularColor[2], 1.0f};
        GLfloat shininess = material->shininess*128.0f;
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse_color);
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient_color);
        glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, emissive_color);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specuar_color);
        glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess);
    }
}

void CPPRWD::GroupTreeRender(X3dNode *data, void *dataptr)
{
    X3dGroup *group = (X3dGroup*)data;
    if (group->children)
        for (int i = 0; i < group->children_length; i++)
            if (group->children[i]) 
            {
                if(!(preRender || initRender))
                    glDisable(GL_TEXTURE_2D);
                group->children[i]->treeRender(dataptr);
            }
}

static void transformData(X3dTransform *transform)
{
        glTranslatef(transform->translation[0], transform->translation[1], transform->translation[2]);
        glTranslatef(transform->center[0], transform->center[1], transform->center[2]);
        glRotatef( ((transform->rotation[3] / M_PI) * 180.0f), transform->rotation[0], transform->rotation[1], transform->rotation[2]);
        glRotatef( ((transform->scaleOrientation[3] / M_PI) * 180.0f), transform->scaleOrientation[0], transform->scaleOrientation[1], transform->scaleOrientation[2]);
        glScalef(transform->scale[0], transform->scale[1], transform->scale[2]);
        glRotatef( ((transform->scaleOrientation[3] / M_PI) * 180.0f) * -1.0f, transform->scaleOrientation[0], transform->scaleOrientation[1], transform->scaleOrientation[2]);
        glTranslatef(transform->center[0] * -1.0f, transform->center[1] * -1.0f, transform->center[2] * -1.0f);
}

void CPPRWD::TransformTreeRender(X3dNode *data, void *dataptr)
{
    X3dTransform *transform = (X3dTransform*)data;
    if(preRender || initRender)
    {
        if (transform->children)
            for (int i = 0; i < transform->children_length; i++)
                if (transform->children[i])
                    transform->children[i]->treeRender(dataptr);
    }
    else
    {
        glPushMatrix();
        transformData(transform);
        if (transform->children)
            for (int i = 0; i < transform->children_length; i++)
                if (transform->children[i]) 
                {
                    glDisable(GL_TEXTURE_2D);
                    transform->children[i]->treeRender(dataptr);
                }
        glPopMatrix();
    }
}

void CPPRWD::ViewpointRender(X3dNode *data, void*)
{
    X3dViewpoint *viewpoint = (X3dViewpoint*)data;
    if(preRender)
    {
        viewPointExists = true;
        
        if(!viewpointRendered)
        {
            viewpointRendered = true;
            viewpoint1 = viewpoint;

            fieldOfViewdegree = ((viewpoint->fieldOfView / M_PI) * 180.0f);
        }
    }
}

void CPPRWD::SwitchTreeRender(X3dNode *data, void *dataptr)
{
    struct X3dSwitch *nodeSwitch = (struct X3dSwitch *)data;
    glPushMatrix();
    if ((nodeSwitch->children) && (nodeSwitch->whichChoice > -1))
       if (nodeSwitch->whichChoice < nodeSwitch->children_length)
           if (nodeSwitch->children[nodeSwitch->whichChoice])
           {
               if(!(preRender || initRender))
                   glDisable(GL_TEXTURE_2D);
               nodeSwitch->children[nodeSwitch->whichChoice]->
                     treeRender(dataptr);
           }
    glPopMatrix();
}

struct HAnimJointExtraDataStruct {
    float jointMatrix[16];
    float thisMatrix[16];
};

void multMatrixVec(float *ret, float mat[16], float x, float y, float z)
{
    float s = mat[4*0+3]*x + mat[4*1+3]*y + mat[4*2+3]*z + mat[4*3+3];
    if (s == 0)
        s = 1;

    ret[0] = (mat[4*0+0]*x + mat[4*1+0]*y + mat[4*2+0]*z + mat[4*3+0])/s;
    ret[1] = (mat[4*0+1]*x + mat[4*1+1]*y + mat[4*2+1]*z + mat[4*3+1])/s;
    ret[2] = (mat[4*0+2]*x + mat[4*1+2]*y + mat[4*2+2]*z + mat[4*3+2])/s;
}

struct HAnimHumanoidExtraDataStruct {
     float *origVertices;
     int origVertices_length;
     float thisMatrix[16];
     float *normalData;
};

void applyJoint(X3dNode *data, X3dNode *x3dcoord, float *origVertices, 
                X3dNode *parent)
{
     struct X3dHAnimJoint *joint = (struct X3dHAnimJoint*)data;
     struct X3dCoordinate *coord = (X3dCoordinate *)x3dcoord;
     int i;
     struct HAnimJointExtraDataStruct *extraVar;
     struct X3dHAnimJoint *mayJoint = (struct X3dHAnimJoint*)parent;
     struct X3dHAnimHumanoid *mayHuman = (struct X3dHAnimHumanoid*)parent;
     float *mayMatrix;

     extraVar = (struct HAnimJointExtraDataStruct *)(joint->extra_data);
     if (extraVar == NULL)
         return;
     if (mayJoint->getType() == X3dHAnimJointType) 
     {
         struct HAnimJointExtraDataStruct *jointData =
             (struct HAnimJointExtraDataStruct *)(mayJoint->extra_data);
         mayMatrix = jointData->jointMatrix;
     } else {
         struct HAnimHumanoidExtraDataStruct *humanData =
             (struct HAnimHumanoidExtraDataStruct *)(mayHuman->extra_data);
         mayMatrix = humanData->thisMatrix;
     }
     glMatrixMode(GL_MODELVIEW);
     glPushMatrix();
     glLoadIdentity();
     glMultMatrixf(mayMatrix);
     glMultMatrixf(extraVar->thisMatrix);
     glGetFloatv(GL_MODELVIEW_MATRIX, extraVar->jointMatrix);
     glPopMatrix();

     if (coord && coord->point)
     {
         int *mskinCoordIndex = joint->skinCoordIndex;
         float *mskinCoordWeight = joint->skinCoordWeight;
         for (i = 0; i < joint->skinCoordIndex_length; i++)
             if (mskinCoordIndex[i] > -1) {
                 int index = mskinCoordIndex[i];
                 float *point = origVertices + index * 3;
                 float wpoint[3];
                 float newpoint[3];
                 int sCwI = joint->skinCoordWeight_length - 1;
                 float weight = mskinCoordWeight[i < sCwI ? i : sCwI];
                 newpoint[0] = coord->point[index * 3];
                 newpoint[1] = coord->point[index * 3 + 1];
                 newpoint[2] = coord->point[index * 3 + 2];
                 multMatrixVec(wpoint, extraVar->jointMatrix, 
                               point[0], point[1], point[2]);
                 wpoint[0] = wpoint[0] * weight; 
                 wpoint[1] = wpoint[1] * weight; 
                 wpoint[2] = wpoint[2] * weight; 
                 newpoint[0] += wpoint[0];
                 newpoint[1] += wpoint[1];
                 newpoint[2] += wpoint[2];
                 coord->point[index * 3 + 0] = newpoint[0];
                 coord->point[index * 3 + 1] = newpoint[1];
                 coord->point[index * 3 + 2] = newpoint[2];
             }
    }
    if (joint->children)
        for (i = 0; i < joint->children_length; i++)
            if (joint->children[i])
            {
                if (joint->children[i]->getType() == X3dHAnimJointType)
                    if (coord && coord->point)
                        applyJoint(joint->children[i], x3dcoord, 
                                   origVertices, data);
            }
}

void CPPRWD::HAnimHumanoidTreeRender(X3dNode *data, void *dataptr)
{
    struct X3dHAnimHumanoid *humanoid = (struct X3dHAnimHumanoid*)data;
    int i;
    struct HAnimHumanoidExtraDataStruct *extraVar;
    struct X3dCoordinate *coord = (X3dCoordinate *)humanoid->skinCoord;

    if(initRender)
    {
        humanoid->extra_data = malloc(sizeof(struct 
                                             HAnimHumanoidExtraDataStruct));
    }
    extraVar = (struct HAnimHumanoidExtraDataStruct *)humanoid->extra_data;
    if (extraVar == NULL)
        return;
    if(initRender) {
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glLoadIdentity();
        glGetFloatv(GL_MODELVIEW_MATRIX, extraVar->thisMatrix);
        glPopMatrix();

        if (coord && coord->point) 
        {
            extraVar->origVertices = (float *)malloc(coord->point_length * 
                                                     sizeof(float));
            for (i = 0; i < coord->point_length; i++)
                extraVar->origVertices[i] = coord->point[i];
            extraVar->origVertices_length = coord->point_length;
        }
    }
    if(initRender)
    {
        if (humanoid->skeleton)
            for (i = 0; i < humanoid->skeleton_length; i++)
                if (humanoid->skeleton[i]) 
                {
                    humanoid->skeleton[i]->treeRender(dataptr);;
                }       
        if (humanoid->skin)
        {
            for (i = 0; i < humanoid->skin_length; i++)
                if (humanoid->skin[i])
                    humanoid->skin[i]->treeRender(dataptr);
            for (i = 0; i < humanoid->skin_length; i++)
                if (humanoid->skin[i]) {
                    glDisable(GL_TEXTURE_2D);
                    X3dIndexedFaceSet *geometry = NULL;
                    if (humanoid->skin[i]->getType() == X3dShapeType) {
                        X3dShape *shape = (X3dShape *)humanoid->skin[i];
                        if (shape->geometry && shape->geometry->getType() == X3dIndexedFaceSetType)
                            geometry = (X3dIndexedFaceSet *)shape->geometry;
                    }
                    if (geometry)
                        geometry->createNormals(&extraVar->normalData);
                    humanoid->skin[i]->treeRender(extraVar->normalData);
                }
        }
    }
    else
    {
        struct X3dCoordinate *coord = (X3dCoordinate *)humanoid->skinCoord;

        if (preRender)
        {
            glPushMatrix();
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();
            glTranslatef(humanoid->translation[0], humanoid->translation[1], humanoid->translation[2]);
            glTranslatef(humanoid->center[0], humanoid->center[1], humanoid->center[2]);
            glRotatef( ( (humanoid->rotation[3] / (2*M_PI) ) * 360), humanoid->rotation[0], humanoid->rotation[1], humanoid->rotation[2]);
            glRotatef( ( (humanoid->scaleOrientation[3] / (2*M_PI) ) * 360), humanoid->scaleOrientation[0], humanoid->scaleOrientation[1], humanoid->scaleOrientation[2]);
            glScalef(humanoid->scale[0], humanoid->scale[1], humanoid->scale[2]);
            glRotatef( ( (humanoid->scaleOrientation[3] / (2*M_PI) ) * 360) * -1, humanoid->scaleOrientation[0], humanoid->scaleOrientation[1], humanoid->scaleOrientation[2]);
            glTranslatef(humanoid->center[0] * -1, humanoid->center[1] * -1, humanoid->center[2] * -1);
            glGetFloatv(GL_MODELVIEW_MATRIX, extraVar->thisMatrix);
            glPopMatrix();
            if (coord)
                for (i = 0; i < coord->point_length; i++)
                    coord->point[i] = 0;
        }
        glPushMatrix();
        glMultMatrixf(extraVar->thisMatrix);
        if (humanoid->skeleton)
            for (i = 0; i < humanoid->skeleton_length; i++)
                if (humanoid->skeleton[i])
                {
                    if (preRender) 
                    {
                        if (humanoid->skeleton[i]->getType() == 
                            X3dHAnimJointType)
                            if (coord && coord->point)
                                applyJoint(humanoid->skeleton[i], coord, 
                                           extraVar->origVertices, data);
                    }
                    glDisable(GL_TEXTURE_2D);
                    humanoid->skeleton[i]->treeRender(dataptr);
                }
        glPopMatrix();
        if (humanoid->skin)
            for (i = 0; i < humanoid->skin_length; i++)
                if (humanoid->skin[i]) {
                    glDisable(GL_TEXTURE_2D);
                    X3dIndexedFaceSet *geometry = NULL;
                    if (humanoid->skin[i]->getType() == X3dShapeType) {
                        X3dShape *shape = (X3dShape *)humanoid->skin[i];
                        if (shape->geometry && shape->geometry->getType() == X3dIndexedFaceSetType)
                            geometry = (X3dIndexedFaceSet *)shape->geometry;
                    }
                    if (geometry)
                        geometry->createNormals(&extraVar->normalData);
                    humanoid->skin[i]->treeRender(extraVar->normalData);
                }
    }
}

void CPPRWD::HAnimJointTreeRender(X3dNode *data, void *dataptr)
{
    struct X3dHAnimJoint *joint = (struct X3dHAnimJoint*)data;
    struct HAnimJointExtraDataStruct *extraVar;
    int i;

    if(initRender)
    {
        joint->extra_data = malloc(sizeof(struct 
                                          HAnimJointExtraDataStruct));
    }
    extraVar = (struct HAnimJointExtraDataStruct *)joint->extra_data;
    if (extraVar == NULL)
        return;
    if(initRender)
    {
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glLoadIdentity();
        glGetFloatv(GL_MODELVIEW_MATRIX, extraVar->jointMatrix);
        glGetFloatv(GL_MODELVIEW_MATRIX, extraVar->thisMatrix);
        glPopMatrix();
        if (joint->children)
            for (i = 0; i < joint->children_length; i++)
                if (joint->children[i])
                {
                    glDisable(GL_TEXTURE_2D);
                    joint->children[i]->treeRender(dataptr);
                }
    }
    else
    {
        if (preRender)
        {
            glMatrixMode(GL_MODELVIEW);
            glPushMatrix();
            glLoadIdentity();
            glTranslatef(joint->translation[0], joint->translation[1], joint->translation[2]);
            glTranslatef(joint->center[0], joint->center[1], joint->center[2]);
            glRotatef( ( (joint->rotation[3] / (2*M_PI) ) * 360), joint->rotation[0], joint->rotation[1], joint->rotation[2]);
            glRotatef( ( (joint->scaleOrientation[3] / (2*M_PI) ) * 360), joint->scaleOrientation[0], joint->scaleOrientation[1], joint->scaleOrientation[2]);
            glScalef(joint->scale[0], joint->scale[1], joint->scale[2]);
            glRotatef( ( (joint->scaleOrientation[3] / (2*M_PI) ) * 360) * -1, joint->scaleOrientation[0], joint->scaleOrientation[1], joint->scaleOrientation[2]);
            glTranslatef(joint->center[0] * -1, joint->center[1] * -1, joint->center[2] * -1);
            glGetFloatv(GL_MODELVIEW_MATRIX, extraVar->thisMatrix);
            glPopMatrix();
        }
        glMultMatrixf(extraVar->thisMatrix);
        if (joint->children)
            for (i = 0; i < joint->children_length; i++)
                if (joint->children[i])
                {
                    glDisable(GL_TEXTURE_2D);
                    joint->children[i]->treeRender(dataptr);
                }
    }
}

struct ParticleSystemExtraDataStruct {
    int m_enabled;
    float m_force[3];
    float *m_internPosition; 
    float *m_internVector; 
    double *m_lifeTime;
    double *m_startTime; 
    bool    m_particlesDirty;
    double  m_internTime;
    float   m_mass;
};

void startParticle(struct X3dParticleSystem *system, int i, 
                   struct ParticleSystemExtraDataStruct *extraVar)
{
    float speed = 1;
    extraVar->m_internPosition[3 * i] = 0;
    extraVar->m_internPosition[3 * i + 1] = 0; 
    extraVar->m_internPosition[3 * i + 2] = 0;
    float alpha = (random() / (float)RAND_MAX) * 2.0 * M_PI;
    float maxAngle = 2.0 * M_PI;
    if (system->emitter)
        if (system->emitter->getType() == X3dConeEmitterType)
        {
            X3dConeEmitter *emit = (X3dConeEmitter *) system->emitter;
            maxAngle = emit->angle;
        }
    float angle = (random() / (float)RAND_MAX) * maxAngle; 

    extraVar->m_internVector[3 * i] = speed * extraVar->m_force[0] * 
                                      sin(angle) * cos(alpha);
    extraVar->m_internVector[3 * i + 1] = speed * extraVar->m_force[1] * 
                                          cos(angle);
    extraVar->m_internVector[3 * i + 2]= speed * extraVar->m_force[2] * 
                                         sin(angle) * sin(alpha);
    bool setInternVector = false;

    if (system->emitter)
        if (system->emitter->getType() == X3dConeEmitterType) 
        {
            X3dConeEmitter *emit = (X3dConeEmitter *)system->emitter;
            extraVar->m_mass = emit->mass;
            extraVar->m_internPosition[3 * i] = emit->position[0];
            extraVar->m_internPosition[3 * i + 1] = emit->position[1]; 
            extraVar->m_internPosition[3 * i + 2] = emit->position[2];
            speed = (random() / (float)RAND_MAX) * 
                    (emit->variation / 2.0f + 1) * emit->speed;
        } 
        else if (system->emitter->getType() == X3dPointEmitterType) 
        {
            X3dPointEmitter *emit = (X3dPointEmitter *)system->emitter;
            extraVar->m_mass = emit->mass;
            extraVar->m_internPosition[i * 3] = emit->position[0];
            extraVar->m_internPosition[i * 3 + 1] = emit->position[1]; 
            extraVar->m_internPosition[i * 3 + 2] = emit->position[2];
            speed = (random() / (float)RAND_MAX) * 
                    (emit->variation / 2.0f + 1) * emit->speed;
            if ((emit->direction[0] != 0) &&
                (emit->direction[1] != 0) &&
                (emit->direction[0] != 0)) {
                setInternVector = true;
                extraVar->m_internVector[i * 3] = speed * 
                                                  emit->direction[0];
                extraVar->m_internVector[i * 3 + 1] = speed * 
                                                      emit->direction[1];
                extraVar->m_internVector[i * 3 + 2] = speed * 
                                                      emit->direction[2];
            }
        }
        else if (system->emitter->getType() == X3dExplosionEmitterType) 
        {
            X3dExplosionEmitter *emit = (X3dExplosionEmitter *)system->emitter;
            extraVar->m_mass = emit->mass;
            extraVar->m_internPosition[3 * i] = emit->position[0];
            extraVar->m_internPosition[3 * i + 1] = emit->position[1]; 
            extraVar->m_internPosition[3 * i + 2] = emit->position[2];
            speed = (random() / (float)RAND_MAX) * 
                    (emit->variation / 2.0f + 1) * emit->speed;
        }
        else if (system->emitter->getType() == X3dPolylineEmitterType) 
        {
            X3dPolylineEmitter *emit = (X3dPolylineEmitter *)system->emitter;
            extraVar->m_mass = emit->mass;
            X3dCoordinate *ncoord = (X3dCoordinate *)emit->coord;
            speed = (random() / (float)RAND_MAX) * 
                    (emit->variation / 2.0f + 1) * emit->speed;
            int numLines = 0;
            float *coords = NULL;
            int *lineIndicesCoord1 = (int *)malloc(sizeof(int));
            int *lineIndicesCoord2 = (int *)malloc(sizeof(int));
            if (ncoord) 
            {
                coords = ncoord->point;
                bool startLine = true;
                bool validLine = false;
                for (int i = 0; i < emit->coordIndex_length; i++) {
                    if (emit->coordIndex[i] < 0) 
                    {
                        startLine = true;
                        validLine = false;
                    } else if (!validLine)
                        validLine = true;  
                    else if (emit->coordIndex[i] != emit->coordIndex[i - 1]) 
                    {
                         numLines++;
                         lineIndicesCoord1 = (int *)realloc(lineIndicesCoord1,
                                                            numLines *
                                                            sizeof(int));
                         lineIndicesCoord2 = (int *)realloc(lineIndicesCoord2,
                                                            numLines *
                                                            sizeof(int));
                         lineIndicesCoord1[numLines - 1] = emit->coordIndex[i];
                         lineIndicesCoord2[numLines - 1] = emit->coordIndex[i - 
                                                                            1];
                    }
                }
            }
            if (coords && (numLines > 0))
            {
                int numLine = (random() / (float)RAND_MAX) * numLines;
                if (numLine == numLines)
                    numLine--;
                int coord1 = lineIndicesCoord1[numLine];              
                int coord2 = lineIndicesCoord2[numLine];
                if ((coord1 * 3 < ncoord->point_length) && 
                    (coord2 * 3 < ncoord->point_length))
                {
                    float point1x = coords[coord1 * 3];              
                    float point1y = coords[coord1 * 3 + 1];              
                    float point1z = coords[coord1 * 3 + 2];              
                    float point2x = coords[coord2 * 3];              
                    float point2y = coords[coord2 * 3 + 1];              
                    float point2z = coords[coord2 * 3 + 2];              
                    float vecx = point1x - point2x;
                    float vecy = point1y - point2y;
                    float vecz = point1z - point2z;
                    float ran = (random() / (float)RAND_MAX);
                    float startPointx = point2x + vecx * ran;
                    float startPointy = point2y + vecy * ran;
                    float startPointz = point2z + vecz * ran;
                    extraVar->m_internPosition[i * 3    ] = startPointx;
                    extraVar->m_internPosition[i * 3 + 1] = startPointy;
                    extraVar->m_internPosition[i * 3 + 2] = startPointz;
                } 
                else 
                {
                    extraVar->m_internPosition[i * 3    ] = 0;
                    extraVar->m_internPosition[i * 3 + 1] = 0;
                    extraVar->m_internPosition[i * 3 + 2] = 0;
                }
            } 
            else 
            {
                extraVar->m_internPosition[i * 3    ] = 0;
                extraVar->m_internPosition[i * 3 + 1] = 0;
                extraVar->m_internPosition[i * 3 + 2] = 0;
            }
            free(lineIndicesCoord1);
            free(lineIndicesCoord2);
            if ((emit->direction[0] != 0) &&
                (emit->direction[1] != 0) &&
                (emit->direction[0] != 0))
            {
                setInternVector = true;
                extraVar->m_internVector[3 * i] = speed * 
                                                  emit->direction[0];
                extraVar->m_internVector[3 * i + 1] = speed * 
                                                      emit->direction[1];
                extraVar->m_internVector[3 * i + 1] = speed * 
                                                      emit->direction[2];
            }
        }
        else if (system->emitter->getType() == X3dVolumeEmitterType) 
        {
            X3dVolumeEmitter *emit = (X3dVolumeEmitter *)system->emitter;
            extraVar->m_mass = emit->mass;
            X3dCoordinate *ncoord = (X3dCoordinate *)emit->coord;
            speed = (random() / (float)RAND_MAX) * 
                    (emit->variation / 2.0f + 1) * emit->speed;
            int numPoints = 0;
            float *coords = NULL;
            float *coordArray = (float *)malloc(sizeof(float));
            if (ncoord) {
                coords = ncoord->point;
                for (int i = 0; i < emit->coordIndex_length; i++)
                {
                    int index = emit->coordIndex[i];
                    if (index > -1)
                    {
                        numPoints++;
                        coordArray = (float *)realloc(coordArray, 
                                                      numPoints * 3 *
                                                      sizeof(float));
                        coordArray[(numPoints - 1) * 3    ] = 
                            coords[index * 3    ];
                        coordArray[(numPoints - 1) * 3 + 1] = 
                            coords[index * 3 + 1];
                        coordArray[(numPoints - 1) * 3 + 2] = 
                            coords[index * 3 + 2];
                    }
                }
            }
            if (coords && (numPoints > 0)) {
                int numPoint = (random() / (float)RAND_MAX) * numPoints;
                extraVar->m_internPosition[i * 3    ] = 
                    coordArray[numPoint * 3    ];
                extraVar->m_internPosition[i * 3 + 1] = 
                    coordArray[numPoint * 3 + 1];
                extraVar->m_internPosition[i * 3 + 2] =
                    coordArray[numPoint * 3 + 2]; 
            }
            free(coordArray);
        }
        else if (system->emitter->getType() == X3dSurfaceEmitterType) 
        {
            X3dSurfaceEmitter *emit = (X3dSurfaceEmitter *)system->emitter;
            extraVar->m_mass = emit->mass;
            speed = (random() / (float)RAND_MAX) * 
                    (emit->variation / 2.0f + 1) * emit->speed;
            static bool once = false;
            if (!once) 
            {
                fprintf(stderr, "Sorry, C/C++/java export of SurfaceEmitter is not supported\n");
                once = true;
            }
            extraVar->m_internPosition[i * 3    ] = 0;
            extraVar->m_internPosition[i * 3 + 1] = 0;
            extraVar->m_internPosition[i * 3 + 2] = 0;
        }

    if (!setInternVector)
    {
        extraVar->m_internVector[3 * i] = speed * extraVar->m_force[1] * 
                                          cos(angle) * cos(alpha);
        extraVar->m_internVector[3 * i + 1] = speed * extraVar->m_force[2] * 
                                              sin(angle);
        extraVar->m_internVector[3 * i + 2] = speed * extraVar->m_force[3] * 
                                              cos(angle) * sin(alpha);
    }
    extraVar->m_lifeTime[i] = (random() / (float)RAND_MAX) * 
                              (system->lifetimeVariation / 2.0f + 1) * 
                              system->particleLifetime;
    extraVar->m_startTime[i] = getTimerTime();
}

void CPPRWD::ParticleSystemTreeRender(X3dNode *data, void *dataptr)
{
    struct X3dParticleSystem *system = (struct X3dParticleSystem*)data;
    struct ParticleSystemExtraDataStruct *extraVar;
    int i;

    if(initRender)
    {
        system->extra_data = malloc(sizeof(struct 
                                           ParticleSystemExtraDataStruct));
        if (system->appearance)
            system->appearance->treeRender(NULL);
        if (system->geometry)
            system->geometry->treeRender(NULL);
    }
    extraVar = (struct ParticleSystemExtraDataStruct *)system->extra_data;
    if (extraVar == NULL)
        return;
    if(initRender)
    {
         extraVar->m_enabled = 1;
         if (strcmp(system->geometryType, "GEOMETRY") != 0) 
         {
             extraVar->m_enabled = 0;
             fprintf(stderr, "%s%s\n", 
                     "Warning: only ParticleSystem.geometryType ",
                     "== GEOMETRY are rendered\n");
         } else {
             int numParticles = system->maxParticles;
             extraVar->m_internPosition = (float *)malloc(numParticles * 
                                               3 * sizeof(float)); 
             extraVar->m_internVector = (float *)malloc(numParticles * 
                                                        3 * sizeof(float));
             extraVar->m_lifeTime = (double *)malloc(numParticles * 
                                                     sizeof(double));
             extraVar->m_startTime = (double *)malloc(numParticles * 
                                                      sizeof(double));; 
             for (int i = 0; i < numParticles; i++)
                 startParticle(system, i, extraVar);                      
         }
         extraVar->m_particlesDirty = true;
         extraVar->m_internTime = 0;
         extraVar->m_mass = 0;

         extraVar->m_force[0] = 0;
         extraVar->m_force[1] = 0;
         extraVar->m_force[2] = 0;
    }
    else if (!preRender)
    {
        if (extraVar->m_enabled) 
        {
            for (int j = 0; j < 3; j ++)
                extraVar->m_force[j] += 0;
    
            for (int i = 0; i < system->physics_length; i++)
                if (system->physics[i]) 
                {
                    X3dNode *phys = system->physics[i];
                    if (phys->getType() == X3dForcePhysicsModelType) 
                    {
                        X3dForcePhysicsModel *model = (X3dForcePhysicsModel *)
                                                      phys;
                        const float *forceForce = model->force;
                        for (int j = 0; j < 3; j ++)
                            extraVar->m_force[j] += forceForce[j];
                    } else if (phys->getType() == X3dWindPhysicsModelType) {
                        X3dWindPhysicsModel *model = (X3dWindPhysicsModel *)
                                                     phys;
                        for (int j = 0; j < 3; j++)
                            extraVar->m_force[j] += model->direction[j] *
                                                    pow(10, 2.0f * 
                                                            log(model->speed) * 
                                                            0.64615f);
                    }
                }
            int numberParticles = system->maxParticles;
        
            double t = getTimerTime();
            double delta = 0;
            if (extraVar->m_internTime == 0)
                delta = 0;
            else
               delta = t - extraVar->m_internTime;
            for (int i = 0; i < numberParticles; i++) 
            {
                if ((t - extraVar->m_startTime[i]) > extraVar->m_lifeTime[i])
                    startParticle(system, i, extraVar);
                else  
                    for (int j = 0; j < 3; j++)
                        extraVar->m_internVector[i * 3 + j] += 
                            delta * extraVar->m_force[j];
                float mass = extraVar->m_mass;
                if (mass == 0.0f)
                    mass = 1.0f;
                for (int j = 0; j < 3; j++)
                    extraVar->m_internPosition[3 * i + j] += 
                        mass * extraVar->m_internVector[3 * i + j] * delta;
        
                glPushMatrix();
                glTranslatef(extraVar->m_internPosition[3 * i],
                             extraVar->m_internPosition[3 * i + 1],
                             extraVar->m_internPosition[3 * i + 2]);
                if (system->appearance)
                    system->appearance->treeRender(NULL);
                if (system->geometry)
                    system->geometry->treeRender(NULL);
                glPopMatrix();
            }
            extraVar->m_internTime = getTimerTime();

        }
    }
}

void CPPRWD::NavigationInfoTreeRender(X3dNode *data, void *dataptr)
{
    struct X3dNavigationInfo *navi = (struct X3dNavigationInfo*)data;

    if(initRender)
    {
       if (navi->type_length > 0)  {
           if (strcmp(navi->type[0], "EXAMINE") == 0)
               navigation = NAV_EXAMINE;
           else if (strcmp(navi->type[0], "WALK") == 0)
               navigation = NAV_WALK;
           else if (strcmp(navi->type[0], "FLY") == 0)
               navigation = NAV_FLY;
           else if (strcmp(navi->type[0], "NONE") == 0)
               navigation = NAV_NONE;
           else if (strcmp(navi->type[0], "ANY") == 0)
               navigation = NAV_ANY;
       }           
    }
}

class CPPRWDTimeSensorData {
public:
    double startTime;
    double stopTime;
};

bool CPPRWD::TimeSensorSendEvents(X3dNode *data, const char *event, 
                                  void* extraData)
{
    X3dTimeSensor *timeSensor = (struct X3dTimeSensor *)data;
    double currentTime = 0;
    CPPRWDTimeSensorData *dataPtr;
    double callTime;
    if (timeSensor->m_data == NULL) {
        timeSensor->m_data = (CPPRWDTimeSensorData *)
                             new CPPRWDTimeSensorData();
        timeSensor->isActive = 0;     
        dataPtr = (CPPRWDTimeSensorData *)timeSensor->m_data;
        dataPtr->startTime = timeSensor->startTime;
        dataPtr->stopTime = timeSensor->stopTime;       
    }

    dataPtr = (CPPRWDTimeSensorData *)timeSensor->m_data;

    if (!timeSensor->enabled)
        return false;

    currentTime = getTimerTime();

    if (timeSensor->isActive) {
        timeSensor->time = currentTime;
    } else {
        if (timeSensor->startTime > dataPtr->stopTime)
            dataPtr->startTime = timeSensor->startTime;
    }
    if (!timeSensor->loop) { 
        if (dataPtr->stopTime > dataPtr->startTime) {
            if (timeSensor->isActive) {
                dataPtr->stopTime = currentTime;
                timeSensor->isActive = 1;
                return true;  
            }
            return false;
        }
        if (currentTime > (dataPtr->startTime + 
                           timeSensor->cycleInterval)) {
            timeSensor->fraction_changed = 1.0f;
            if (timeSensor->isActive) {
                dataPtr->stopTime = currentTime;
                timeSensor->isActive = 0;
                return true;
            }
            return false;
        }
        if (currentTime < timeSensor->startTime)
            return false;
    }

    timeSensor->isActive = 1;
                
    callTime = currentTime - dataPtr->startTime;
    timeSensor->fraction_changed = (float)(fmod(callTime, 
                                                timeSensor->cycleInterval) / 
                                           timeSensor->cycleInterval);
    return true;
}


bool CPPRWD::TouchSensorSendEvents(X3dNode *data, const char *event,
                                   void *extraData)
{
    X3dTouchSensor *touchSensor = (struct X3dTouchSensor *)data;
    touchSensor->isActive = !released;
    if (!touchSensor->isActive && !touchSensor->isOver)
        return false;           
    return touchSensor->enabled;
}

bool CPPRWD::PlaneSensorSendEvents(X3dNode *data, const char *event,
                                   void *extraData)
{
    X3dPlaneSensor *planeSensor = (struct X3dPlaneSensor *)data;
    if (planeSensor->maxPosition[0] > planeSensor->minPosition[0]) {
        if (planeSensor->translation_changed[0] > planeSensor->maxPosition[0])
            planeSensor->translation_changed[0] = planeSensor->maxPosition[0];
        if (planeSensor->translation_changed[0] < planeSensor->minPosition[0])
            planeSensor->translation_changed[0] = planeSensor->minPosition[0];
    }
    if (planeSensor->maxPosition[1] > planeSensor->minPosition[1]) {
        if (planeSensor->translation_changed[1] > planeSensor->maxPosition[1])
            planeSensor->translation_changed[1] = planeSensor->maxPosition[1];
        if (planeSensor->translation_changed[1] < planeSensor->minPosition[1])
            planeSensor->translation_changed[1] = planeSensor->minPosition[1];
    }
    return planeSensor->enabled;
}

bool CPPRWD::CylinderSensorSendEvents(X3dNode *data, const char *event,
                                      void *extraData)
{
    X3dCylinderSensor *cylinderSensor = (struct X3dCylinderSensor *)data;
    if (cylinderSensor->maxAngle > cylinderSensor->minAngle) {
        if (cylinderSensor->rotation_changed[3] > cylinderSensor->maxAngle)
            cylinderSensor->rotation_changed[3] = cylinderSensor->maxAngle;
        if (cylinderSensor->rotation_changed[3] < cylinderSensor->minAngle)
            cylinderSensor->rotation_changed[3] = cylinderSensor->minAngle;
    }
    return cylinderSensor->enabled;
}

bool CPPRWD::SphereSensorSendEvents(X3dNode *data, const char *event,
                                    void *extraData)
{
    X3dSphereSensor *sphereSensor = (struct X3dSphereSensor *)data;
    return sphereSensor->enabled;
}

bool CPPRWD::ProximitySensorSendEvents(X3dNode *data, const char *event,
                                       void *extraData)
{
    X3dProximitySensor *proximitySensor = (struct X3dProximitySensor *)data;
    bool enabled = proximitySensor->enabled;
    if ((proximitySensor->size[0] == 0) || 
        (proximitySensor->size[1] == 0) || 
        (proximitySensor->size[2] == 0))
        enabled = false;
    else {
        X3dViewpoint *viewpoint = (X3dViewpoint *)viewpoint1;
        float xmax =  proximitySensor->size[0] / 2 + proximitySensor->center[0];
        float xmin = -proximitySensor->size[0] / 2 + proximitySensor->center[0];
        float ymax =  proximitySensor->size[1] / 2 + proximitySensor->center[1];
        float ymin = -proximitySensor->size[1] / 2 + proximitySensor->center[1];
        float zmax =  proximitySensor->size[2] / 2 + proximitySensor->center[2];
        float zmin = -proximitySensor->size[2] / 2 + proximitySensor->center[2];
        if ((viewpoint->position) &&
            (viewpoint->position[0] > xmin) &&
            (viewpoint->position[1] < ymax) &&
            (viewpoint->position[1] > ymin) &&
            (viewpoint->position[2] < zmax) &&
            (viewpoint->position[2] > zmin)) {
            proximitySensor->position_changed[0] = viewpoint->position[0];
            proximitySensor->position_changed[1] = viewpoint->position[1];
            proximitySensor->position_changed[2] = viewpoint->position[2];
            proximitySensor->orientation_changed[0] = viewpoint->orientation[0];
            proximitySensor->orientation_changed[1] = viewpoint->orientation[1];
            proximitySensor->orientation_changed[2] = viewpoint->orientation[2];
            proximitySensor->orientation_changed[3] = viewpoint->orientation[3];
        } else
            enabled = false;
    }
    return enabled;
}

bool CPPRWD::CollisionSendEvents(X3dNode *data, const char *event,
                                       void *extraData)
{
    X3dCollision *collision = (struct X3dCollision *)data;
    bool enabled = collision->enabled;
    return enabled;
}

float CPPRWD::interpolate(float t, float key, float oldKey, 
                                  float value, float oldValue)
{
    float delta = (key == oldKey) ? 0 : (t - oldKey) / (key - oldKey);
    return oldValue + delta * (value - oldValue);
}

void CPPRWD::accountInterpolator(float *target, float fraction,
                                 float *keys, float *keyValues, int numKeys,
                                 int stride, int rotation)
{
    int i;
    int j;
    if (!rotation) {
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
        float k1 = 0.0;
        float k2 = 1.0;
        int pos1 = 0;
        int pos2 = 0;
        int pos;
        float s;
        float q[4];
        float q1[4];
        float q2[4];
        int flip;
        float alpha;
        float beta;
        float theta;
        float sin_t;
        float cos_t;
        float len;

        if (numKeys == 0) {
            target[0] = 0;
            target[1] = 0;
            target[2] = 1;
            target[3] = 0;
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

        s = sin(keyValues[pos1 * 4 + 3] * 0.5f);
        for (i = 0; i < 3; i++)
             q1[i] = keyValues[pos1 * 4 + i] * s;
        q1[3] = cos(keyValues[pos1 * 4 + 3] * 0.5f);

        s = sin(keyValues[pos2 * 4 + 3] * 0.5f);
        for (i = 0; i < 3; i++)
             q2[i] = keyValues[pos2 * 4 + i] * s;
        q2[3] = cos(keyValues[pos2 * 4 + 3] * 0.5f);
        
        cos_t = q1[0]*q2[0] + q1[1]*q2[1] + q1[2]*q2[2] + q1[3]*q2[3];

        len = (float) sqrt(cos_t);
        if (len > 0)
            len = 1 / len;
        else
            len = 1;

        if (cos_t < 0.0) {
            cos_t = -cos_t;
            flip = 1;
        } else
            flip = 0;
        if (1.0 - cos_t < EPSILON)
            beta = 1.0f - alpha;
        else {
            theta = (float) acos(cos_t);
            sin_t = (float) sin(theta);
            beta = (float) sin(theta - alpha * theta) / sin_t;
            alpha = (float) sin(alpha * theta) / sin_t;
        }
        if (flip)
            alpha = -alpha;
        for (i = 0; i < 4; i++)
            q[i] = (q1[i] * beta + q2[i] * alpha);
        for (i = 0; i < 3; i++)
            target[i] = q[i] * len;
        target[3] = 2.0f * (float) acos(q[3]);
    }
}

bool CPPRWD::PositionInterpolatorSendEvents(X3dNode *data, const char *event,
                                            void* extraData)
{
    static int warned = 0;
    X3dPositionInterpolator *interpolator = (X3dPositionInterpolator *)data;
    if (interpolator->key_length * 3 != interpolator->keyValue_length) { 
        if (!warned)
            error("invalid PositionInterpolator: number keys do not match number values\n");
        warned = 1;
        return false;
    }
    accountInterpolator(interpolator->value_changed, interpolator->set_fraction,
                        interpolator->key, interpolator->keyValue, 
                        interpolator->key_length, 3, 0); 
    return true;
}

bool CPPRWD::OrientationInterpolatorSendEvents(X3dNode *data, const char *event,
                                               void* extraData)
{
    static int warned = 0;
    X3dOrientationInterpolator *interpolator = (X3dOrientationInterpolator *)
                                               data;
    if (interpolator->key_length * 4 != interpolator->keyValue_length) { 
        if (!warned)
            error("invalid OrientationInterpolator: number keys do not match number values\n");
        warned = 1;
        return false;
    }
    accountInterpolator(interpolator->value_changed, interpolator->set_fraction,
                        interpolator->key, interpolator->keyValue, 
                        interpolator->key_length, 4, 1); 
    return true;
}

bool CPPRWD::ColorInterpolatorSendEvents(X3dNode *data, const char *event,
                                         void* extraData)
{
    static int warned = 0;
    X3dColorInterpolator *interpolator = (X3dColorInterpolator *)data;
    if (interpolator->key_length * 3 != interpolator->keyValue_length) { 
        if (!warned)
            error("invalid ColorInterpolator: number keys do not match number values\n");
        warned = 1;
        return false;
    }
    accountInterpolator(interpolator->value_changed, interpolator->set_fraction,
                        interpolator->key, interpolator->keyValue, 
                        interpolator->key_length, 3, 0); 
    return true;
}

bool CPPRWD::ScalarInterpolatorSendEvents(X3dNode *data, const char *event,
                                          void* extraData)
{
    static int warned = 0;
    X3dScalarInterpolator *interpolator = (X3dScalarInterpolator *)data;
    if (interpolator->key_length * 1 != interpolator->keyValue_length) { 
        if (!warned)
            error("invalid ScalarInterpolator: number keys do not match number values\n");
        warned = 1;
        return false;
    }
    accountInterpolator(&interpolator->value_changed, interpolator->set_fraction,
                        interpolator->key, interpolator->keyValue, 
                        interpolator->key_length, 1, 0); 
    return true;
}

bool CPPRWD::CoordinateInterpolatorSendEvents(X3dNode *data, const char *event,
                                              void* extraData)
{
    X3dCoordinateInterpolator *interpolator = (X3dCoordinateInterpolator *)data;
    int targetSize = interpolator->keyValue_length / interpolator->key_length;
    if (interpolator->value_changed == NULL) {
        if (interpolator->key_length * targetSize != 
            interpolator->keyValue_length){
            error("invalid CoodinateInterpolator: number keys do not match number values\n");
            return false;
        }
        interpolator->value_changed_length = targetSize;
        interpolator->value_changed = new float[targetSize == 0 ? 1 :
                                                targetSize * sizeof(float)];
    }
        
    accountInterpolator(interpolator->value_changed, interpolator->set_fraction,
                        interpolator->key, interpolator->keyValue, 
                        interpolator->key_length, 
                        interpolator->value_changed_length, 0);
    return true;
}

bool CPPRWD::NormalInterpolatorSendEvents(X3dNode *data, const char *event,
                                          void* extraData)
{
    int i,j;
    X3dNormalInterpolator *interpolator = (X3dNormalInterpolator *)data;
    int targetSize = interpolator->keyValue_length / interpolator->key_length;
    if (interpolator->value_changed == NULL) {
        if (interpolator->key_length * targetSize != 
            interpolator->keyValue_length){
            error("invalid CoodinateInterpolator: number keys do not match number values\n");
            return false;
        }
        interpolator->value_changed_length = targetSize;
        interpolator->value_changed = new float[targetSize == 0 ? 1 :
                                                targetSize * sizeof(float)];
    }
        
    accountInterpolator(interpolator->value_changed, interpolator->set_fraction,
                        interpolator->key, interpolator->keyValue, 
                        interpolator->key_length, 
                        interpolator->value_changed_length, 0);

    /* normalize results */
    for (i = 0; i < targetSize; i += 3) {
        float *vec = interpolator->keyValue + 3 * i;
        float len = sqrt(vec[0] * vec[0] + vec[1] * vec[1] + vec[2] * vec[2]);
        if (len != 0)
            for (j = 0; j < 3 ; j++)
                vec[j] /= len;
    }   
    return true;
}

bool CPPRWD::PositionInterpolator2DSendEvents(X3dNode *data, const char *event,
                                              void* extraData)
{
    static int warned = 0;
    X3dPositionInterpolator2D *interpolator = (X3dPositionInterpolator2D *)data;
    if (interpolator->key_length * 2 != interpolator->keyValue_length) { 
        if (!warned)
            error("invalid PositionInterpolator: number keys do not match number values\n");
        warned = 1;
        return false;
    }
    accountInterpolator(interpolator->value_changed, interpolator->set_fraction,
                        interpolator->key, interpolator->keyValue, 
                        interpolator->key_length, 2, 0); 
    return true;
}

#define PICK_BUFFER_SIZE 65536
#define PICK_REGION_SIZE 2.5
        
X3dNode *rootNode = &scenegraph.root;
//X3dNode *rootNode = &scenegraph.DEFNAME;

void CPPRWD::processEvents() {
    X3dProcessEvents(&scenegraph, NULL);
}

void CPPRWD::draw(bool render)
{
    if (render) {
        glClearColor(0, 0, 0, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(fieldOfViewdegree, 1.0, Z_NEAR, Z_FAR); 

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        if (viewpoint1) {
            glRotatef(( -(viewpoint1->orientation[3] / (2*M_PI) ) * 360), viewpoint1->orientation[0], viewpoint1->orientation[1], viewpoint1->orientation[2]);
            glTranslatef(-viewpoint1->position[0], -viewpoint1->position[1], -viewpoint1->position[2]);
        }
        glGetFloatv(GL_MODELVIEW_MATRIX, navigationMatrix);

        viewpointRendered = false;

        preRender = true;
        rootNode->treeRender(NULL);

        if(!lightExists)
        {
            static int light = -1;
            if (light == -1)
                light = allocLight();
            glEnable(light);
        }

        glMatrixMode(GL_MODELVIEW);
        preRender = false;
        glRenderMode(GL_RENDER);
        rootNode->treeRender(NULL);

    } else {
        // render to pickbuffer
        GLuint pickBuffer[PICK_BUFFER_SIZE];
        glSelectBuffer(PICK_BUFFER_SIZE, pickBuffer);
        glRenderMode(GL_SELECT);

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        GLint v[4];
        glGetIntegerv(GL_VIEWPORT, v);
        if (moved)
            gluPickMatrix((GLdouble)mouseXMove, (GLdouble)(height - mouseYMove), 1, 1, v);
        else
            gluPickMatrix((GLdouble)mouseX, (GLdouble)(height - mouseY), 1, 1, v);

        glMatrixMode(GL_PROJECTION);
        gluPerspective(fieldOfViewdegree, 1.0, Z_NEAR, Z_FAR); 

        glMatrixMode(GL_MODELVIEW);
        preRender = true;
        rootNode->treeRender(NULL);

        glMatrixMode(GL_MODELVIEW);
        glLoadMatrixf(navigationMatrix);

        glInitNames();
        preRender = false;
        rootNode->treeRender(NULL);

        GLint hits = glRenderMode(GL_RENDER);
        if (hits < 0) // overflow flag has been set, ignore
            hits = - hits;
        processHits(hits, pickBuffer);
    }
}

static X3dNode *getCollision(X3dNode *node)
{
    for (X3dNode *parent = node->m_parent; parent != NULL; 
         parent = parent->m_parent)
        if (parent->getType() == X3dCollisionType) 
            return parent;
    return NULL;
}

static void transform(X3dNode *node)
{
    for (X3dNode *parent = node->m_parent; parent != NULL; 
         parent = parent->m_parent)
        if (parent->getType() == X3dTransformType) {
            X3dTransform *transform = (X3dTransform *)parent;
            bool hasTransform = false;
            for (int i = 0; i < transform->num_route_source; i++)
                if (transform->route_sources[i] == node)
                    hasTransform = true;
            if (!hasTransform)
                transformData(transform);            
        }
}

static void handleCollision(X3dNode *node, float depth)
{
    X3dNode *maybeCollision = getCollision(node);
    if (maybeCollision != NULL) {
        X3dCollision *collision = (X3dCollision *)maybeCollision;
        if (collision->enabled) {
            int x = mouseXMove;
            int y = mouseYMove;
            GLdouble model[16];
            GLdouble proj[16];
            GLint view[4];
            GLdouble pan_x, pan_y, pan_z;
            glGetDoublev(GL_MODELVIEW_MATRIX, model);
            glGetDoublev(GL_PROJECTION_MATRIX, proj);
            glGetIntegerv(GL_VIEWPORT, view);
            gluUnProject((GLdouble)x, (GLdouble)y, (GLdouble) depth / UINT_MAX, 
                         model, proj, view, &pan_x, &pan_y, &pan_z);
            float viewz = 0;
            if (viewpoint1)
                viewz = viewpoint1->position[2];
            if (fabs(pan_z - viewz) < 2) {
                stopNavigation();
                collision->isActive = true;
                collision->collideTime = getTimerTime();            
            }
        }
    }
}

struct CylinderSensorExtraDataStruct {
    float x_sum;
};

static void handleSibling(X3dNode *sibling, float depth) 
{
    if (sibling->getType() == X3dTouchSensorType) {
        X3dTouchSensor *touchSensor = (X3dTouchSensor *)sibling;
        if (clicked)
            touchSensor->touchTime = getTimerTime();
        touchSensor->isOver = true;
        int x = mouseXMove;
        int y = mouseYMove;
        GLdouble model[16];
        GLdouble proj[16];
        GLint view[4];
        GLdouble pan_x, pan_y, pan_z;
        glGetDoublev(GL_MODELVIEW_MATRIX, model);
        glGetDoublev(GL_PROJECTION_MATRIX, proj);
        glGetIntegerv(GL_VIEWPORT, view);
        gluUnProject((GLdouble)x, (GLdouble)y, (GLdouble) depth / UINT_MAX, 
                     model, proj, view, &pan_x, &pan_y, &pan_z);
        touchSensor->hitPoint_changed[0] = (float)pan_x;
        touchSensor->hitPoint_changed[1] = (float)pan_y;
        touchSensor->hitPoint_changed[2] = (float)pan_z;
        isHit = true;
    }
    if (moved && sibling->getType() == X3dPlaneSensorType) {
        X3dPlaneSensor *planeSensor = (X3dPlaneSensor *)sibling;
        int x = mouseXMove;
        int y = mouseYMove;
        GLdouble model[16];
        GLdouble proj[16];
        GLint view[4];
        GLdouble pan_x, pan_y, pan_z;
        glGetDoublev(GL_MODELVIEW_MATRIX, model);
        glGetDoublev(GL_PROJECTION_MATRIX, proj);
        glGetIntegerv(GL_VIEWPORT, view);
        gluUnProject((GLdouble)x, (GLdouble)y, (GLdouble) depth / UINT_MAX, 
                     model, proj, view, &pan_x, &pan_y, &pan_z);
        planeSensor->translation_changed[0] = (float)pan_x;
        planeSensor->translation_changed[1] = (float)pan_y;
        planeSensor->translation_changed[2] = 0;
        planeSensor->trackPoint_changed[0] = (float)pan_x;
        planeSensor->trackPoint_changed[1] = (float)pan_y;
        planeSensor->trackPoint_changed[2] = (float)pan_z;
        isHit = true;
    }
    if (sibling->getType() == X3dPlaneSensorType) {
        X3dPlaneSensor *planeSensor = (X3dPlaneSensor *)sibling;
        planeSensor->isOver = true;
    }
    if (moved && sibling->getType() == X3dCylinderSensorType) {
        X3dCylinderSensor *cylinderSensor = (X3dCylinderSensor *)sibling;
     
        struct CylinderSensorExtraDataStruct *extraVar = NULL; 
        if (cylinderSensor->extra_data == NULL) {
            cylinderSensor->extra_data = malloc(sizeof(struct
                                                CylinderSensorExtraDataStruct));
            struct CylinderSensorExtraDataStruct *extraVar = 
                (CylinderSensorExtraDataStruct *)cylinderSensor->extra_data;
            extraVar->x_sum = 0;
        }
        extraVar = (struct CylinderSensorExtraDataStruct *) 
                   cylinderSensor->extra_data;

        float rot1[4] = { 0, 1, 0, (mouseXMove - mouseXOld + extraVar->x_sum) / 20.f};
        float quat[4];
        SFRotation2quaternion(quat, rot1);
        quaternion2SFRotation(cylinderSensor->rotation_changed, quat);
        extraVar->x_sum += mouseXMove - mouseXOld; 
        int x = mouseXMove;
        int y = mouseYMove;
        GLdouble model[16];
        GLdouble proj[16];
        GLint view[4];
        GLdouble pan_x, pan_y, pan_z;
        glGetDoublev(GL_MODELVIEW_MATRIX, model);
        glGetDoublev(GL_PROJECTION_MATRIX, proj);
        glGetIntegerv(GL_VIEWPORT, view);
        gluUnProject((GLdouble)x, (GLdouble)y, (GLdouble) depth / UINT_MAX, 
                     model, proj, view, &pan_x, &pan_y, &pan_z);
        cylinderSensor->trackPoint_changed[0] = (float)pan_x;
        cylinderSensor->trackPoint_changed[1] = (float)pan_y;
        cylinderSensor->trackPoint_changed[2] = (float)pan_z;
        isHit = true;
    }
    if (sibling->getType() == X3dCylinderSensorType) {
        X3dCylinderSensor *cylinderSensor = (X3dCylinderSensor *)sibling;
        cylinderSensor->isOver = true;
    }
    if (moved && sibling->getType() == X3dSphereSensorType) {
        X3dSphereSensor *sphereSensor = (X3dSphereSensor *)sibling;

        float rot1[4] = { 0, 1, 0, (mouseXMove - mouseXOld) / 20.f};
        float rot2[4] = { 1, 0, 0, (mouseYMove - mouseYOld) / 20.f};

        float q1[4];
        SFRotation2quaternion(q1, rot1);
        float q2[4];
        SFRotation2quaternion(q2, rot2);
        float quat[4];
        quaternionMult(quat, q1, q2);
        float rotSphere[4] = { sphereSensor->rotation_changed[0],
                               sphereSensor->rotation_changed[1],
                               sphereSensor->rotation_changed[2],
                               -sphereSensor->rotation_changed[3] };
        float qSphere[4];
        SFRotation2quaternion(qSphere, rotSphere);
        float rotSphere2[4] = { sphereSensor->rotation_changed[0],
                                sphereSensor->rotation_changed[1],
                                sphereSensor->rotation_changed[2],
                                sphereSensor->rotation_changed[3] };
        float qSphere2[4];
        SFRotation2quaternion(qSphere2, rotSphere2);
        float quat2[4];
        quaternionMult(quat2, quat, qSphere); 
        float quat3[4];
        quaternionMult(quat3, qSphere2, quat2); 
        float quat4[4];
        quaternionMult(quat4, quat3, qSphere2); 
        quaternion2SFRotation(sphereSensor->rotation_changed, quat4);
        normalizeAxis(sphereSensor->rotation_changed);
        int x = mouseXMove;
        int y = mouseYMove;
        GLdouble model[16];
        GLdouble proj[16];
        GLint view[4];
        GLdouble pan_x, pan_y, pan_z;
        glGetDoublev(GL_MODELVIEW_MATRIX, model);
        glGetDoublev(GL_PROJECTION_MATRIX, proj);
        glGetIntegerv(GL_VIEWPORT, view);
        gluUnProject((GLdouble)x, (GLdouble)y, (GLdouble) depth / UINT_MAX, 
                     model, proj, view, &pan_x, &pan_y, &pan_z);
        sphereSensor->trackPoint_changed[0] = (float)pan_x;
        sphereSensor->trackPoint_changed[1] = (float)pan_y;
        sphereSensor->trackPoint_changed[2] = (float)pan_z;
        isHit = true;
    }
    if (sibling->getType() == X3dSphereSensorType) {
        X3dSphereSensor *sphereSensor = (X3dSphereSensor *)sibling;
        sphereSensor->isOver = true;
    }
}

void CPPRWD::processHits(GLint hits, GLuint *pickBuffer)
{
    unsigned depth = UINT_MAX;
    int hit = -1;
    for (int i = 0; i < hits; i++) {
        unsigned numNames = *pickBuffer++;
        unsigned minDepth = *pickBuffer++;
        unsigned maxDepth = *pickBuffer++;
        for (int i = 0; i < numNames; i++) {
           int buffer = *pickBuffer++;
           if (maxDepth < depth) {
               depth = maxDepth; 
               hit = buffer;
           }
        } 
    }    
    isHit = false;
    if (hit > -1) {
        X3dNode *node = scenegraph.getNodeFromGlName(hit);
        if (node) {
             handleCollision(node, depth);
             for (X3dNode *parent = node->m_parent; parent != NULL; 
                 parent = parent->m_parent) {
                 if (parent->getType() == X3dGroupType) {
                     X3dGroup *group = (X3dGroup *)parent;
                     for (int i = 0; i < group->children_length; i++)
                         if (group->children[i] && 
                             group->children[i] != node) {
                             if (group->children[i]->getType() == 
                                 X3dTouchSensorType) {
                                 handleSibling(group->children[i], depth);
                             }
                             if ((moved && group->children[i]->getType() == 
                                  X3dPlaneSensorType) ||
                                 (moved && group->children[i]->getType() == 
                                  X3dCylinderSensorType) ||
                                 (moved && group->children[i]->getType() == 
                                  X3dSphereSensorType)) {
                                 handleSibling(group->children[i], depth);
                             }
                         }
                 }
                 if (parent->getType() == X3dTransformType) {
                     X3dTransform *transform = (X3dTransform *)parent;
                     for (int i = 0; i < transform->children_length; i++)
                         if (transform->children[i] && 
                             transform->children[i] != node) {
                             if (transform->children[i]->getType() == 
                                     X3dTouchSensorType) {
                                 handleSibling(transform->children[i], depth);
                             }
                             if ((moved && transform->children[i]->getType() == 
                                  X3dPlaneSensorType) ||
                                 (moved && transform->children[i]->getType() == 
                                  X3dCylinderSensorType) ||
                                 (moved && transform->children[i]->getType() == 
                                  X3dSphereSensorType)) {
                                 handleSibling(transform->children[i], depth);
                             }
                         }
                 }
             }
        }     
    }
    clicked = false;
    moved = false;
}

void CPPRWD::init()
{
    X3dTransform::treeRenderCallback = TransformTreeRender;
    X3dGroup::treeRenderCallback = GroupTreeRender;
    X3dIndexedFaceSet::renderCallback = IndexedFaceSetRender;
    X3dIndexedFaceSet::createNormalsCallback = IndexedFaceSetCreateNormals;
    X3dPointLight::renderCallback = PointLightRender;
    X3dDirectionalLight::renderCallback = DirectionalLightRender;
    X3dSpotLight::renderCallback = SpotLightRender;
    X3dMaterial::renderCallback = MaterialRender;
    X3dViewpoint::renderCallback = ViewpointRender;
    X3dPixelTexture::renderCallback = PixelTextureRender;
    X3dImageTexture::renderCallback = ImageTextureRender;
    X3dSwitch::treeRenderCallback = SwitchTreeRender;
    X3dParticleSystem::treeRenderCallback = ParticleSystemTreeRender;
    X3dHAnimHumanoid::treeRenderCallback = HAnimHumanoidTreeRender;
    X3dHAnimJoint::treeRenderCallback = HAnimJointTreeRender;
    X3dNavigationInfo::treeRenderCallback = NavigationInfoTreeRender;
    X3dTimeSensor::processEventCallback = TimeSensorSendEvents;
    X3dTouchSensor::processEventCallback = TouchSensorSendEvents;
    X3dPlaneSensor::processEventCallback = PlaneSensorSendEvents;
    X3dCylinderSensor::processEventCallback = CylinderSensorSendEvents;
    X3dSphereSensor::processEventCallback = SphereSensorSendEvents;
    X3dProximitySensor::processEventCallback = ProximitySensorSendEvents;
    X3dCollision::processEventCallback = CollisionSendEvents;
    X3dPositionInterpolator::processEventCallback = PositionInterpolatorSendEvents;
    X3dOrientationInterpolator::processEventCallback = OrientationInterpolatorSendEvents;
    X3dColorInterpolator::processEventCallback = ColorInterpolatorSendEvents;
    X3dScalarInterpolator::processEventCallback = ScalarInterpolatorSendEvents;
    X3dCoordinateInterpolator::processEventCallback = CoordinateInterpolatorSendEvents;

    glViewport(0 ,0, width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(fieldOfViewdegree, 1.0, Z_NEAR, Z_FAR); 
    glGetDoublev(GL_PROJECTION_MATRIX, projectionMatrix);
    glMatrixMode(GL_MODELVIEW);

    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT);
    glColorMaterial(GL_FRONT_AND_BACK, GL_DIFFUSE);
    glColorMaterial(GL_FRONT_AND_BACK, GL_EMISSION);
    glColorMaterial(GL_FRONT_AND_BACK, GL_SPECULAR);
    glColorMaterial(GL_FRONT_AND_BACK, GL_SHININESS);

    /*Enable light*/
    glEnable(GL_LIGHTING);

    GLint one[4] = {1, 1, 1, 0};
    glLightModeliv(GL_LIGHT_MODEL_AMBIENT, one);

    /* Use depth buffering for hidden surface elimination. */
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE);

    glDepthMask(GL_TRUE);
    glDepthFunc(GL_LEQUAL);

    glEnable(GL_ALPHA_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glAlphaFunc(GL_NOTEQUAL, 0);

    glShadeModel(GL_SMOOTH);

    preRender = false;
    initRender = true;

    rootNode->treeRender(NULL);

    if(!viewPointExists)
    {
        X3dViewpoint viewpoint;
        viewpoint.fieldOfView = 0.785398;
        viewpoint.position = (float *)malloc(3 * sizeof(float));
        viewpoint.position[0] = 0;
        viewpoint.position[1] = 0;
        viewpoint.position[2] = 10;
        viewpoint.orientation = (float *)malloc(4 * sizeof(float));
        viewpoint.orientation[0] = 0;
        viewpoint.orientation[1] = 0;
        viewpoint.orientation[2] = 1;
        viewpoint.orientation[3] = 0;
        ViewpointRender(&viewpoint, NULL);
        free(viewpoint.position);
        viewpoint.position = NULL;
        free(viewpoint.orientation);
        viewpoint.orientation = NULL;
    }
    initRender = false;

}

void CPPRWD::finalize()
{
}

void reInitSensor(void *node) 
{
    if (((X3dNode *)node)->getType() == X3dTouchSensorType) {
        X3dTouchSensor *touchSensor = (X3dTouchSensor *)node;
        touchSensor->isOver = false;
    }
    if (((X3dNode *)node)->getType() == X3dPlaneSensorType) {
        X3dPlaneSensor *planeSensor = (X3dPlaneSensor *)node;
        planeSensor->isOver = false;
    }
    if (((X3dNode *)node)->getType() == X3dCylinderSensorType) {
        X3dCylinderSensor *cylinderSensor = (X3dCylinderSensor *)node;
        cylinderSensor->isOver = false;
    }
    if (((X3dNode *)node)->getType() == X3dSphereSensorType) {
        X3dSphereSensor *sphereSensor = (X3dSphereSensor *)node;
        sphereSensor->isOver = false;
    }
    if (((X3dNode *)node)->getType() == X3dCollisionType) {
        X3dCollision *collision = (X3dCollision *)node;
        collision->isActive = false;
    }
}
