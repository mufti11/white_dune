/*libC++RWD Library for C++ Rendering of White_dune Data (in Development)*/
// namespace definitions

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

class X3dNode;

namespace CPPRWD
{
    void error(const char *errormsg);

    void IndexedFaceSetRender(X3dNode *data, void*);

    void IndexedFaceSetCreateNormals(X3dNode *data, void*);

    void IndexedLineSetRender(X3dNode *data, void*);

    void LineSetRender(X3dNode *data, void*);

    void PointSetRender(X3dNode *data, void*);

    void ImageTextureRender(X3dNode *data, void*);

    void PixelTextureRender(X3dNode *data, void*);

    void PointLightRender(X3dNode *data, void*);

    void DirectionalLightRender(X3dNode *data, void*);

    void SpotLightRender(X3dNode *data, void*);

    void MaterialRender(X3dNode *data, void*);

    void ShapeRender(X3dNode *data, void*);

    void CoordinateRender(X3dNode *data, void*);

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

    void setWalkOn(void);

    void walkOn(void);

    void startWalking(void);
 
    void walkCamera(float *walk, bool walkOn);

    void orbitCamera(float dtheta, float dphi, float z);
};

