/* VRML97/X3D file "" converted to C with white_dune*/

#ifndef NULL
# define NULL (void *)0
#endif

struct X3dSceneGraph;

typedef void X3dNode;

struct X3dNodeStruct;

typedef void (*X3dCallback)(X3dNode *node, void *data);

void X3dTreeRenderCallback(struct X3dNodeStruct *node, void *data);
void X3dTreeDoWithDataCallback(struct X3dNodeStruct *node, void *data);
typedef int (*X3dProcessEventCallback)(X3dNode *node, const char *event, void *data);



struct X3dColorRGBA {
    X3dNode* m_parent;
    X3dNode* m_protoRoot;
    int m_type;
    void *m_data;
    float* color;
    int color_length;
    void* extra_data;
};

void X3dColorRGBAInit(struct X3dColorRGBA* self) {
    self->m_protoRoot = NULL;
    self->color = NULL;
    self->color_length = 0;
    self->extra_data = NULL;
}

X3dCallback X3dColorRGBARenderCallback = NULL;
X3dCallback X3dColorRGBATreeRenderCallback = NULL;
X3dCallback X3dColorRGBADoWithDataCallback = NULL;
X3dCallback X3dColorRGBATreeDoWithDataCallback = NULL;

X3dProcessEventCallback X3dColorRGBAProcessEventCallback = NULL;

int X3dColorRGBAType = 40;

void X3dColorRGBARender(X3dNode* self, void *dataptr) {
    if (X3dColorRGBARenderCallback)
        X3dColorRGBARenderCallback(self, dataptr);
}
void X3dColorRGBATreeRender(X3dNode* self, void *dataptr) {
    int i;
    if (((struct X3dColorRGBA*)self)->m_protoRoot != NULL)
        X3dTreeRenderCallback(((struct X3dColorRGBA*)self)->m_protoRoot, dataptr);
    else {
        X3dColorRGBARender(self, dataptr);
    }
}
void X3dColorRGBADoWithData(X3dNode* self, void *dataptr) {
    if (X3dColorRGBADoWithDataCallback)
        X3dColorRGBADoWithDataCallback(self, dataptr);
}
void X3dColorRGBATreeDoWithData(X3dNode* self, void *dataptr) {
    int i;
    if (((struct X3dColorRGBA*)self)->m_protoRoot != NULL)
        X3dTreeDoWithDataCallback(((struct X3dColorRGBA*)self)->m_protoRoot, dataptr);
    else {
        X3dColorRGBADoWithData(self, dataptr);
    }
}


struct X3dVolumeEmitter {
    X3dNode* m_parent;
    X3dNode* m_protoRoot;
    int m_type;
    void *m_data;
    float speed;
    float variation;
    float mass;
    float surfaceArea;
    X3dNode* coord;
    float* direction;
    int direction_length;
    int* coordIndex;
    int coordIndex_length;
    short internal;
    void* extra_data;
};

void X3dVolumeEmitterInit(struct X3dVolumeEmitter* self) {
    self->m_protoRoot = NULL;
    self->speed = 0.0f;
    self->variation = 0.0f;
    self->mass = 0.0f;
    self->surfaceArea = 0.0f;
    self->coord = NULL;
    self->direction = NULL;
    self->direction_length = 0;
    self->coordIndex = NULL;
    self->coordIndex_length = 0;
    self->internal = 0;
    self->extra_data = NULL;
}

X3dCallback X3dVolumeEmitterRenderCallback = NULL;
X3dCallback X3dVolumeEmitterTreeRenderCallback = NULL;
X3dCallback X3dVolumeEmitterDoWithDataCallback = NULL;
X3dCallback X3dVolumeEmitterTreeDoWithDataCallback = NULL;

X3dProcessEventCallback X3dVolumeEmitterProcessEventCallback = NULL;

int X3dVolumeEmitterType = 323;

void X3dVolumeEmitterRender(X3dNode* self, void *dataptr) {
    if (X3dVolumeEmitterRenderCallback)
        X3dVolumeEmitterRenderCallback(self, dataptr);
}
void X3dVolumeEmitterTreeRender(X3dNode* self, void *dataptr) {
    int i;
    if (((struct X3dVolumeEmitter*)self)->m_protoRoot != NULL)
        X3dTreeRenderCallback(((struct X3dVolumeEmitter*)self)->m_protoRoot, dataptr);
    else {
        if (((struct X3dVolumeEmitter*)self)->coord)
            X3dTreeRenderCallback(((struct X3dVolumeEmitter*)self)->coord, dataptr);
        X3dVolumeEmitterRender(self, dataptr);
    }
}
void X3dVolumeEmitterDoWithData(X3dNode* self, void *dataptr) {
    if (X3dVolumeEmitterDoWithDataCallback)
        X3dVolumeEmitterDoWithDataCallback(self, dataptr);
}
void X3dVolumeEmitterTreeDoWithData(X3dNode* self, void *dataptr) {
    int i;
    if (((struct X3dVolumeEmitter*)self)->m_protoRoot != NULL)
        X3dTreeDoWithDataCallback(((struct X3dVolumeEmitter*)self)->m_protoRoot, dataptr);
    else {
        if (((struct X3dVolumeEmitter*)self)->coord)
            X3dTreeDoWithDataCallback(((struct X3dVolumeEmitter*)self)->coord, dataptr);
        X3dVolumeEmitterDoWithData(self, dataptr);
    }
}


struct X3dHAnimJoint {
    X3dNode* m_parent;
    X3dNode* m_protoRoot;
    int m_type;
    void *m_data;
    X3dNode** addChildren;
    int addChildren_length;
    X3dNode** removeChildren;
    int removeChildren_length;
    float* rotation;
    int rotation_length;
    float* translation;
    int translation_length;
    float* bboxCenter;
    int bboxCenter_length;
    float* bboxSize;
    int bboxSize_length;
    float* center;
    int center_length;
    float* scale;
    int scale_length;
    float* scaleOrientation;
    int scaleOrientation_length;
    X3dNode** children;
    int children_length;
    X3dNode** displacers;
    int displacers_length;
    float* limitOrientation;
    int limitOrientation_length;
    float* llimit;
    int llimit_length;
    const char* name;
    int* skinCoordIndex;
    int skinCoordIndex_length;
    float* skinCoordWeight;
    int skinCoordWeight_length;
    float* stiffness;
    int stiffness_length;
    float* ulimit;
    int ulimit_length;
    void* extra_data;
};

void X3dHAnimJointInit(struct X3dHAnimJoint* self) {
    self->m_protoRoot = NULL;
    self->rotation = NULL;
    self->rotation_length = 0;
    self->translation = NULL;
    self->translation_length = 0;
    self->bboxCenter = NULL;
    self->bboxCenter_length = 0;
    self->bboxSize = NULL;
    self->bboxSize_length = 0;
    self->center = NULL;
    self->center_length = 0;
    self->scale = NULL;
    self->scale_length = 0;
    self->scaleOrientation = NULL;
    self->scaleOrientation_length = 0;
    self->children = NULL;
    self->children_length = 0;
    self->displacers = NULL;
    self->displacers_length = 0;
    self->limitOrientation = NULL;
    self->limitOrientation_length = 0;
    self->llimit = NULL;
    self->llimit_length = 0;
    self->name = NULL;
    self->skinCoordIndex = NULL;
    self->skinCoordIndex_length = 0;
    self->skinCoordWeight = NULL;
    self->skinCoordWeight_length = 0;
    self->stiffness = NULL;
    self->stiffness_length = 0;
    self->ulimit = NULL;
    self->ulimit_length = 0;
    self->extra_data = NULL;
}

X3dCallback X3dHAnimJointRenderCallback = NULL;
X3dCallback X3dHAnimJointTreeRenderCallback = NULL;
X3dCallback X3dHAnimJointDoWithDataCallback = NULL;
X3dCallback X3dHAnimJointTreeDoWithDataCallback = NULL;

X3dProcessEventCallback X3dHAnimJointProcessEventCallback = NULL;

int X3dHAnimJointType = 107;

void X3dHAnimJointRender(X3dNode* self, void *dataptr) {
    if (X3dHAnimJointRenderCallback)
        X3dHAnimJointRenderCallback(self, dataptr);
}
void X3dHAnimJointTreeRender(X3dNode* self, void *dataptr) {
    int i;
    if (((struct X3dHAnimJoint*)self)->m_protoRoot != NULL)
        X3dTreeRenderCallback(((struct X3dHAnimJoint*)self)->m_protoRoot, dataptr);
    else {
        if (((struct X3dHAnimJoint*)self)->children)
            for (i = 0; i < ((struct X3dHAnimJoint*)self)->children_length; i++)
                if (((struct X3dHAnimJoint*)self)->children[i])
                    X3dTreeRenderCallback(((struct X3dHAnimJoint*)self)->children[i], dataptr);
        if (((struct X3dHAnimJoint*)self)->displacers)
            for (i = 0; i < ((struct X3dHAnimJoint*)self)->displacers_length; i++)
                if (((struct X3dHAnimJoint*)self)->displacers[i])
                    X3dTreeRenderCallback(((struct X3dHAnimJoint*)self)->displacers[i], dataptr);
        X3dHAnimJointRender(self, dataptr);
    }
}
void X3dHAnimJointDoWithData(X3dNode* self, void *dataptr) {
    if (X3dHAnimJointDoWithDataCallback)
        X3dHAnimJointDoWithDataCallback(self, dataptr);
}
void X3dHAnimJointTreeDoWithData(X3dNode* self, void *dataptr) {
    int i;
    if (((struct X3dHAnimJoint*)self)->m_protoRoot != NULL)
        X3dTreeDoWithDataCallback(((struct X3dHAnimJoint*)self)->m_protoRoot, dataptr);
    else {
        if (((struct X3dHAnimJoint*)self)->children)
            for (i = 0; i < ((struct X3dHAnimJoint*)self)->children_length; i++)
                if (((struct X3dHAnimJoint*)self)->children[i])
                    X3dTreeDoWithDataCallback(((struct X3dHAnimJoint*)self)->children[i], dataptr);
        if (((struct X3dHAnimJoint*)self)->displacers)
            for (i = 0; i < ((struct X3dHAnimJoint*)self)->displacers_length; i++)
                if (((struct X3dHAnimJoint*)self)->displacers[i])
                    X3dTreeDoWithDataCallback(((struct X3dHAnimJoint*)self)->displacers[i], dataptr);
        X3dHAnimJointDoWithData(self, dataptr);
    }
}


struct X3dWindPhysicsModel {
    X3dNode* m_parent;
    X3dNode* m_protoRoot;
    int m_type;
    void *m_data;
    float* direction;
    int direction_length;
    short enabled;
    float gustiness;
    float speed;
    float turbulence;
    void* extra_data;
};

void X3dWindPhysicsModelInit(struct X3dWindPhysicsModel* self) {
    self->m_protoRoot = NULL;
    self->direction = NULL;
    self->direction_length = 0;
    self->enabled = 0;
    self->gustiness = 0.0f;
    self->speed = 0.0f;
    self->turbulence = 0.0f;
    self->extra_data = NULL;
}

X3dCallback X3dWindPhysicsModelRenderCallback = NULL;
X3dCallback X3dWindPhysicsModelTreeRenderCallback = NULL;
X3dCallback X3dWindPhysicsModelDoWithDataCallback = NULL;
X3dCallback X3dWindPhysicsModelTreeDoWithDataCallback = NULL;

X3dProcessEventCallback X3dWindPhysicsModelProcessEventCallback = NULL;

int X3dWindPhysicsModelType = 328;

void X3dWindPhysicsModelRender(X3dNode* self, void *dataptr) {
    if (X3dWindPhysicsModelRenderCallback)
        X3dWindPhysicsModelRenderCallback(self, dataptr);
}
void X3dWindPhysicsModelTreeRender(X3dNode* self, void *dataptr) {
    int i;
    if (((struct X3dWindPhysicsModel*)self)->m_protoRoot != NULL)
        X3dTreeRenderCallback(((struct X3dWindPhysicsModel*)self)->m_protoRoot, dataptr);
    else {
        X3dWindPhysicsModelRender(self, dataptr);
    }
}
void X3dWindPhysicsModelDoWithData(X3dNode* self, void *dataptr) {
    if (X3dWindPhysicsModelDoWithDataCallback)
        X3dWindPhysicsModelDoWithDataCallback(self, dataptr);
}
void X3dWindPhysicsModelTreeDoWithData(X3dNode* self, void *dataptr) {
    int i;
    if (((struct X3dWindPhysicsModel*)self)->m_protoRoot != NULL)
        X3dTreeDoWithDataCallback(((struct X3dWindPhysicsModel*)self)->m_protoRoot, dataptr);
    else {
        X3dWindPhysicsModelDoWithData(self, dataptr);
    }
}


struct X3dTransform {
    X3dNode* m_parent;
    X3dNode* m_protoRoot;
    int m_type;
    void *m_data;
    X3dNode** addChildren;
    int addChildren_length;
    X3dNode** removeChildren;
    int removeChildren_length;
    float* rotation;
    int rotation_length;
    float* translation;
    int translation_length;
    float* bboxCenter;
    int bboxCenter_length;
    float* bboxSize;
    int bboxSize_length;
    float* center;
    int center_length;
    float* scale;
    int scale_length;
    float* scaleOrientation;
    int scaleOrientation_length;
    X3dNode** children;
    int children_length;
    void* extra_data;
    int num_route_source;
    X3dNode **route_sources;
};

void X3dTransformInit(struct X3dTransform* self) {
    self->m_protoRoot = NULL;
    self->rotation = NULL;
    self->rotation_length = 0;
    self->translation = NULL;
    self->translation_length = 0;
    self->bboxCenter = NULL;
    self->bboxCenter_length = 0;
    self->bboxSize = NULL;
    self->bboxSize_length = 0;
    self->center = NULL;
    self->center_length = 0;
    self->scale = NULL;
    self->scale_length = 0;
    self->scaleOrientation = NULL;
    self->scaleOrientation_length = 0;
    self->children = NULL;
    self->children_length = 0;
    self->extra_data = NULL;
}

X3dCallback X3dTransformRenderCallback = NULL;
X3dCallback X3dTransformTreeRenderCallback = NULL;
X3dCallback X3dTransformDoWithDataCallback = NULL;
X3dCallback X3dTransformTreeDoWithDataCallback = NULL;

X3dProcessEventCallback X3dTransformProcessEventCallback = NULL;

int X3dTransformType = 289;

void X3dTransformRender(X3dNode* self, void *dataptr) {
    if (X3dTransformRenderCallback)
        X3dTransformRenderCallback(self, dataptr);
}
void X3dTransformTreeRender(X3dNode* self, void *dataptr) {
    int i;
    if (((struct X3dTransform*)self)->m_protoRoot != NULL)
        X3dTreeRenderCallback(((struct X3dTransform*)self)->m_protoRoot, dataptr);
    else {
        if (((struct X3dTransform*)self)->children)
            for (i = 0; i < ((struct X3dTransform*)self)->children_length; i++)
                if (((struct X3dTransform*)self)->children[i])
                    X3dTreeRenderCallback(((struct X3dTransform*)self)->children[i], dataptr);
        X3dTransformRender(self, dataptr);
    }
}
void X3dTransformDoWithData(X3dNode* self, void *dataptr) {
    if (X3dTransformDoWithDataCallback)
        X3dTransformDoWithDataCallback(self, dataptr);
}
void X3dTransformTreeDoWithData(X3dNode* self, void *dataptr) {
    int i;
    if (((struct X3dTransform*)self)->m_protoRoot != NULL)
        X3dTreeDoWithDataCallback(((struct X3dTransform*)self)->m_protoRoot, dataptr);
    else {
        if (((struct X3dTransform*)self)->children)
            for (i = 0; i < ((struct X3dTransform*)self)->children_length; i++)
                if (((struct X3dTransform*)self)->children[i])
                    X3dTreeDoWithDataCallback(((struct X3dTransform*)self)->children[i], dataptr);
        X3dTransformDoWithData(self, dataptr);
    }
}


struct X3dCylinderSensor {
    X3dNode* m_parent;
    X3dNode* m_protoRoot;
    int m_type;
    void *m_data;
    short isActive;
    short isOver;
    float* rotation_changed;
    int rotation_changed_length;
    float* trackPoint_changed;
    int trackPoint_changed_length;
    short autoOffset;
    float* axisRotation;
    int axisRotation_length;
    const char* description;
    float diskAngle;
    short enabled;
    float maxAngle;
    float minAngle;
    float offset;
    void* extra_data;
};

void X3dCylinderSensorInit(struct X3dCylinderSensor* self) {
    self->m_protoRoot = NULL;
    self->autoOffset = 0;
    self->axisRotation = NULL;
    self->axisRotation_length = 0;
    self->description = NULL;
    self->diskAngle = 0.0f;
    self->enabled = 0;
    self->maxAngle = 0.0f;
    self->minAngle = 0.0f;
    self->offset = 0.0f;
    self->extra_data = NULL;
}

X3dCallback X3dCylinderSensorRenderCallback = NULL;
X3dCallback X3dCylinderSensorTreeRenderCallback = NULL;
X3dCallback X3dCylinderSensorDoWithDataCallback = NULL;
X3dCallback X3dCylinderSensorTreeDoWithDataCallback = NULL;

X3dProcessEventCallback X3dCylinderSensorProcessEventCallback = NULL;

int X3dCylinderSensorType = 65;

void X3dCylinderSensorRender(X3dNode* self, void *dataptr) {
    if (X3dCylinderSensorRenderCallback)
        X3dCylinderSensorRenderCallback(self, dataptr);
}
void X3dCylinderSensorTreeRender(X3dNode* self, void *dataptr) {
    int i;
    if (((struct X3dCylinderSensor*)self)->m_protoRoot != NULL)
        X3dTreeRenderCallback(((struct X3dCylinderSensor*)self)->m_protoRoot, dataptr);
    else {
        X3dCylinderSensorRender(self, dataptr);
    }
}
void X3dCylinderSensorDoWithData(X3dNode* self, void *dataptr) {
    if (X3dCylinderSensorDoWithDataCallback)
        X3dCylinderSensorDoWithDataCallback(self, dataptr);
}
void X3dCylinderSensorTreeDoWithData(X3dNode* self, void *dataptr) {
    int i;
    if (((struct X3dCylinderSensor*)self)->m_protoRoot != NULL)
        X3dTreeDoWithDataCallback(((struct X3dCylinderSensor*)self)->m_protoRoot, dataptr);
    else {
        X3dCylinderSensorDoWithData(self, dataptr);
    }
}


struct X3dCommonSurfaceShader {
    X3dNode* m_parent;
    X3dNode* m_protoRoot;
    int m_type;
    void *m_data;
    X3dNode** addChildren;
    int addChildren_length;
    X3dNode** removeChildren;
    int removeChildren_length;
    X3dNode** children;
    int children_length;
    float* bboxCenter;
    int bboxCenter_length;
    float* bboxSize;
    int bboxSize_length;
    void* extra_data;
};

void X3dCommonSurfaceShaderInit(struct X3dCommonSurfaceShader* self) {
    self->m_protoRoot = NULL;
    self->children = NULL;
    self->children_length = 0;
    self->bboxCenter = NULL;
    self->bboxCenter_length = 0;
    self->bboxSize = NULL;
    self->bboxSize_length = 0;
    self->extra_data = NULL;
}

X3dCallback X3dCommonSurfaceShaderRenderCallback = NULL;
X3dCallback X3dCommonSurfaceShaderTreeRenderCallback = NULL;
X3dCallback X3dCommonSurfaceShaderDoWithDataCallback = NULL;
X3dCallback X3dCommonSurfaceShaderTreeDoWithDataCallback = NULL;

X3dProcessEventCallback X3dCommonSurfaceShaderProcessEventCallback = NULL;

int X3dCommonSurfaceShaderType = 42;

void X3dCommonSurfaceShaderRender(X3dNode* self, void *dataptr) {
    if (X3dCommonSurfaceShaderRenderCallback)
        X3dCommonSurfaceShaderRenderCallback(self, dataptr);
}
void X3dCommonSurfaceShaderTreeRender(X3dNode* self, void *dataptr) {
    int i;
    if (((struct X3dCommonSurfaceShader*)self)->m_protoRoot != NULL)
        X3dTreeRenderCallback(((struct X3dCommonSurfaceShader*)self)->m_protoRoot, dataptr);
    else {
        if (((struct X3dCommonSurfaceShader*)self)->children)
            for (i = 0; i < ((struct X3dCommonSurfaceShader*)self)->children_length; i++)
                if (((struct X3dCommonSurfaceShader*)self)->children[i])
                    X3dTreeRenderCallback(((struct X3dCommonSurfaceShader*)self)->children[i], dataptr);
        X3dCommonSurfaceShaderRender(self, dataptr);
    }
}
void X3dCommonSurfaceShaderDoWithData(X3dNode* self, void *dataptr) {
    if (X3dCommonSurfaceShaderDoWithDataCallback)
        X3dCommonSurfaceShaderDoWithDataCallback(self, dataptr);
}
void X3dCommonSurfaceShaderTreeDoWithData(X3dNode* self, void *dataptr) {
    int i;
    if (((struct X3dCommonSurfaceShader*)self)->m_protoRoot != NULL)
        X3dTreeDoWithDataCallback(((struct X3dCommonSurfaceShader*)self)->m_protoRoot, dataptr);
    else {
        if (((struct X3dCommonSurfaceShader*)self)->children)
            for (i = 0; i < ((struct X3dCommonSurfaceShader*)self)->children_length; i++)
                if (((struct X3dCommonSurfaceShader*)self)->children[i])
                    X3dTreeDoWithDataCallback(((struct X3dCommonSurfaceShader*)self)->children[i], dataptr);
        X3dCommonSurfaceShaderDoWithData(self, dataptr);
    }
}


struct X3dPlaneSensor {
    X3dNode* m_parent;
    X3dNode* m_protoRoot;
    int m_type;
    void *m_data;
    short isOver;
    short isActive;
    float* trackPoint_changed;
    int trackPoint_changed_length;
    float* translation_changed;
    int translation_changed_length;
    short autoOffset;
    float* axisRotation;
    int axisRotation_length;
    const char* description;
    short enabled;
    float* maxPosition;
    int maxPosition_length;
    float* minPosition;
    int minPosition_length;
    float* offset;
    int offset_length;
    void* extra_data;
};

void X3dPlaneSensorInit(struct X3dPlaneSensor* self) {
    self->m_protoRoot = NULL;
    self->autoOffset = 0;
    self->axisRotation = NULL;
    self->axisRotation_length = 0;
    self->description = NULL;
    self->enabled = 0;
    self->maxPosition = NULL;
    self->maxPosition_length = 0;
    self->minPosition = NULL;
    self->minPosition_length = 0;
    self->offset = NULL;
    self->offset_length = 0;
    self->extra_data = NULL;
}

X3dCallback X3dPlaneSensorRenderCallback = NULL;
X3dCallback X3dPlaneSensorTreeRenderCallback = NULL;
X3dCallback X3dPlaneSensorDoWithDataCallback = NULL;
X3dCallback X3dPlaneSensorTreeDoWithDataCallback = NULL;

X3dProcessEventCallback X3dPlaneSensorProcessEventCallback = NULL;

int X3dPlaneSensorType = 196;

void X3dPlaneSensorRender(X3dNode* self, void *dataptr) {
    if (X3dPlaneSensorRenderCallback)
        X3dPlaneSensorRenderCallback(self, dataptr);
}
void X3dPlaneSensorTreeRender(X3dNode* self, void *dataptr) {
    int i;
    if (((struct X3dPlaneSensor*)self)->m_protoRoot != NULL)
        X3dTreeRenderCallback(((struct X3dPlaneSensor*)self)->m_protoRoot, dataptr);
    else {
        X3dPlaneSensorRender(self, dataptr);
    }
}
void X3dPlaneSensorDoWithData(X3dNode* self, void *dataptr) {
    if (X3dPlaneSensorDoWithDataCallback)
        X3dPlaneSensorDoWithDataCallback(self, dataptr);
}
void X3dPlaneSensorTreeDoWithData(X3dNode* self, void *dataptr) {
    int i;
    if (((struct X3dPlaneSensor*)self)->m_protoRoot != NULL)
        X3dTreeDoWithDataCallback(((struct X3dPlaneSensor*)self)->m_protoRoot, dataptr);
    else {
        X3dPlaneSensorDoWithData(self, dataptr);
    }
}


struct X3dPositionInterpolator2D {
    X3dNode* m_parent;
    X3dNode* m_protoRoot;
    int m_type;
    void *m_data;
    float set_fraction;
    float* value_changed;
    int value_changed_length;
    float* key;
    int key_length;
    float* keyValue;
    int keyValue_length;
    void* extra_data;
};

void X3dPositionInterpolator2DInit(struct X3dPositionInterpolator2D* self) {
    self->m_protoRoot = NULL;
    self->key = NULL;
    self->key_length = 0;
    self->keyValue = NULL;
    self->keyValue_length = 0;
    self->extra_data = NULL;
}

X3dCallback X3dPositionInterpolator2DRenderCallback = NULL;
X3dCallback X3dPositionInterpolator2DTreeRenderCallback = NULL;
X3dCallback X3dPositionInterpolator2DDoWithDataCallback = NULL;
X3dCallback X3dPositionInterpolator2DTreeDoWithDataCallback = NULL;

X3dProcessEventCallback X3dPositionInterpolator2DProcessEventCallback = NULL;

int X3dPositionInterpolator2DType = 211;

void X3dPositionInterpolator2DRender(X3dNode* self, void *dataptr) {
    if (X3dPositionInterpolator2DRenderCallback)
        X3dPositionInterpolator2DRenderCallback(self, dataptr);
}
void X3dPositionInterpolator2DTreeRender(X3dNode* self, void *dataptr) {
    int i;
    if (((struct X3dPositionInterpolator2D*)self)->m_protoRoot != NULL)
        X3dTreeRenderCallback(((struct X3dPositionInterpolator2D*)self)->m_protoRoot, dataptr);
    else {
        X3dPositionInterpolator2DRender(self, dataptr);
    }
}
void X3dPositionInterpolator2DDoWithData(X3dNode* self, void *dataptr) {
    if (X3dPositionInterpolator2DDoWithDataCallback)
        X3dPositionInterpolator2DDoWithDataCallback(self, dataptr);
}
void X3dPositionInterpolator2DTreeDoWithData(X3dNode* self, void *dataptr) {
    int i;
    if (((struct X3dPositionInterpolator2D*)self)->m_protoRoot != NULL)
        X3dTreeDoWithDataCallback(((struct X3dPositionInterpolator2D*)self)->m_protoRoot, dataptr);
    else {
        X3dPositionInterpolator2DDoWithData(self, dataptr);
    }
}


struct X3dHAnimSite {
    X3dNode* m_parent;
    X3dNode* m_protoRoot;
    int m_type;
    void *m_data;
    X3dNode** addChildren;
    int addChildren_length;
    X3dNode** removeChildren;
    int removeChildren_length;
    float* rotation;
    int rotation_length;
    float* translation;
    int translation_length;
    float* bboxCenter;
    int bboxCenter_length;
    float* bboxSize;
    int bboxSize_length;
    float* center;
    int center_length;
    float* scale;
    int scale_length;
    float* scaleOrientation;
    int scaleOrientation_length;
    X3dNode** children;
    int children_length;
    const char* name;
    void* extra_data;
};

void X3dHAnimSiteInit(struct X3dHAnimSite* self) {
    self->m_protoRoot = NULL;
    self->rotation = NULL;
    self->rotation_length = 0;
    self->translation = NULL;
    self->translation_length = 0;
    self->bboxCenter = NULL;
    self->bboxCenter_length = 0;
    self->bboxSize = NULL;
    self->bboxSize_length = 0;
    self->center = NULL;
    self->center_length = 0;
    self->scale = NULL;
    self->scale_length = 0;
    self->scaleOrientation = NULL;
    self->scaleOrientation_length = 0;
    self->children = NULL;
    self->children_length = 0;
    self->name = NULL;
    self->extra_data = NULL;
}

X3dCallback X3dHAnimSiteRenderCallback = NULL;
X3dCallback X3dHAnimSiteTreeRenderCallback = NULL;
X3dCallback X3dHAnimSiteDoWithDataCallback = NULL;
X3dCallback X3dHAnimSiteTreeDoWithDataCallback = NULL;

X3dProcessEventCallback X3dHAnimSiteProcessEventCallback = NULL;

int X3dHAnimSiteType = 109;

void X3dHAnimSiteRender(X3dNode* self, void *dataptr) {
    if (X3dHAnimSiteRenderCallback)
        X3dHAnimSiteRenderCallback(self, dataptr);
}
void X3dHAnimSiteTreeRender(X3dNode* self, void *dataptr) {
    int i;
    if (((struct X3dHAnimSite*)self)->m_protoRoot != NULL)
        X3dTreeRenderCallback(((struct X3dHAnimSite*)self)->m_protoRoot, dataptr);
    else {
        if (((struct X3dHAnimSite*)self)->children)
            for (i = 0; i < ((struct X3dHAnimSite*)self)->children_length; i++)
                if (((struct X3dHAnimSite*)self)->children[i])
                    X3dTreeRenderCallback(((struct X3dHAnimSite*)self)->children[i], dataptr);
        X3dHAnimSiteRender(self, dataptr);
    }
}
void X3dHAnimSiteDoWithData(X3dNode* self, void *dataptr) {
    if (X3dHAnimSiteDoWithDataCallback)
        X3dHAnimSiteDoWithDataCallback(self, dataptr);
}
void X3dHAnimSiteTreeDoWithData(X3dNode* self, void *dataptr) {
    int i;
    if (((struct X3dHAnimSite*)self)->m_protoRoot != NULL)
        X3dTreeDoWithDataCallback(((struct X3dHAnimSite*)self)->m_protoRoot, dataptr);
    else {
        if (((struct X3dHAnimSite*)self)->children)
            for (i = 0; i < ((struct X3dHAnimSite*)self)->children_length; i++)
                if (((struct X3dHAnimSite*)self)->children[i])
                    X3dTreeDoWithDataCallback(((struct X3dHAnimSite*)self)->children[i], dataptr);
        X3dHAnimSiteDoWithData(self, dataptr);
    }
}


struct X3dImageTexture {
    X3dNode* m_parent;
    X3dNode* m_protoRoot;
    int m_type;
    void *m_data;
    const char** url;
    int url_length;
    short repeatS;
    short repeatT;
    X3dNode* textureProperties;
    X3dNode** effects;
    int effects_length;
    const char* alphaChannel;
    void* extra_data;
};

void X3dImageTextureInit(struct X3dImageTexture* self) {
    self->m_protoRoot = NULL;
    self->url = NULL;
    self->url_length = 0;
    self->repeatS = 0;
    self->repeatT = 0;
    self->textureProperties = NULL;
    self->effects = NULL;
    self->effects_length = 0;
    self->alphaChannel = NULL;
    self->extra_data = NULL;
}

X3dCallback X3dImageTextureRenderCallback = NULL;
X3dCallback X3dImageTextureTreeRenderCallback = NULL;
X3dCallback X3dImageTextureDoWithDataCallback = NULL;
X3dCallback X3dImageTextureTreeDoWithDataCallback = NULL;

X3dProcessEventCallback X3dImageTextureProcessEventCallback = NULL;

int X3dImageTextureType = 112;

void X3dImageTextureRender(X3dNode* self, void *dataptr) {
    if (X3dImageTextureRenderCallback)
        X3dImageTextureRenderCallback(self, dataptr);
}
void X3dImageTextureTreeRender(X3dNode* self, void *dataptr) {
    int i;
    if (((struct X3dImageTexture*)self)->m_protoRoot != NULL)
        X3dTreeRenderCallback(((struct X3dImageTexture*)self)->m_protoRoot, dataptr);
    else {
        if (((struct X3dImageTexture*)self)->textureProperties)
            X3dTreeRenderCallback(((struct X3dImageTexture*)self)->textureProperties, dataptr);
        if (((struct X3dImageTexture*)self)->effects)
            for (i = 0; i < ((struct X3dImageTexture*)self)->effects_length; i++)
                if (((struct X3dImageTexture*)self)->effects[i])
                    X3dTreeRenderCallback(((struct X3dImageTexture*)self)->effects[i], dataptr);
        X3dImageTextureRender(self, dataptr);
    }
}
void X3dImageTextureDoWithData(X3dNode* self, void *dataptr) {
    if (X3dImageTextureDoWithDataCallback)
        X3dImageTextureDoWithDataCallback(self, dataptr);
}
void X3dImageTextureTreeDoWithData(X3dNode* self, void *dataptr) {
    int i;
    if (((struct X3dImageTexture*)self)->m_protoRoot != NULL)
        X3dTreeDoWithDataCallback(((struct X3dImageTexture*)self)->m_protoRoot, dataptr);
    else {
        if (((struct X3dImageTexture*)self)->textureProperties)
            X3dTreeDoWithDataCallback(((struct X3dImageTexture*)self)->textureProperties, dataptr);
        if (((struct X3dImageTexture*)self)->effects)
            for (i = 0; i < ((struct X3dImageTexture*)self)->effects_length; i++)
                if (((struct X3dImageTexture*)self)->effects[i])
                    X3dTreeDoWithDataCallback(((struct X3dImageTexture*)self)->effects[i], dataptr);
        X3dImageTextureDoWithData(self, dataptr);
    }
}


struct X3dScreenFontStyle {
    X3dNode* m_parent;
    X3dNode* m_protoRoot;
    int m_type;
    void *m_data;
    const char** family;
    int family_length;
    short horizontal;
    const char** justify;
    int justify_length;
    const char* language;
    short leftToRight;
    float spacing;
    const char* style;
    short topToBottom;
    float pointSize;
    void* extra_data;
};

void X3dScreenFontStyleInit(struct X3dScreenFontStyle* self) {
    self->m_protoRoot = NULL;
    self->family = NULL;
    self->family_length = 0;
    self->horizontal = 0;
    self->justify = NULL;
    self->justify_length = 0;
    self->language = NULL;
    self->leftToRight = 0;
    self->spacing = 0.0f;
    self->style = NULL;
    self->topToBottom = 0;
    self->pointSize = 0.0f;
    self->extra_data = NULL;
}

X3dCallback X3dScreenFontStyleRenderCallback = NULL;
X3dCallback X3dScreenFontStyleTreeRenderCallback = NULL;
X3dCallback X3dScreenFontStyleDoWithDataCallback = NULL;
X3dCallback X3dScreenFontStyleTreeDoWithDataCallback = NULL;

X3dProcessEventCallback X3dScreenFontStyleProcessEventCallback = NULL;

int X3dScreenFontStyleType = 232;

void X3dScreenFontStyleRender(X3dNode* self, void *dataptr) {
    if (X3dScreenFontStyleRenderCallback)
        X3dScreenFontStyleRenderCallback(self, dataptr);
}
void X3dScreenFontStyleTreeRender(X3dNode* self, void *dataptr) {
    int i;
    if (((struct X3dScreenFontStyle*)self)->m_protoRoot != NULL)
        X3dTreeRenderCallback(((struct X3dScreenFontStyle*)self)->m_protoRoot, dataptr);
    else {
        X3dScreenFontStyleRender(self, dataptr);
    }
}
void X3dScreenFontStyleDoWithData(X3dNode* self, void *dataptr) {
    if (X3dScreenFontStyleDoWithDataCallback)
        X3dScreenFontStyleDoWithDataCallback(self, dataptr);
}
void X3dScreenFontStyleTreeDoWithData(X3dNode* self, void *dataptr) {
    int i;
    if (((struct X3dScreenFontStyle*)self)->m_protoRoot != NULL)
        X3dTreeDoWithDataCallback(((struct X3dScreenFontStyle*)self)->m_protoRoot, dataptr);
    else {
        X3dScreenFontStyleDoWithData(self, dataptr);
    }
}


struct X3dNavigationInfo {
    X3dNode* m_parent;
    X3dNode* m_protoRoot;
    int m_type;
    void *m_data;
    short set_bind;
    double bindTime;
    short isBound;
    short transitionComplete;
    float* avatarSize;
    int avatarSize_length;
    short headlight;
    float speed;
    double transitionTime;
    const char** transitionType;
    int transitionType_length;
    const char** type;
    int type_length;
    float visibilityLimit;
    const char* blendingSort;
    void* extra_data;
};

void X3dNavigationInfoInit(struct X3dNavigationInfo* self) {
    self->m_protoRoot = NULL;
    self->avatarSize = NULL;
    self->avatarSize_length = 0;
    self->headlight = 0;
    self->speed = 0.0f;
    self->transitionTime = 0;
    self->transitionType = NULL;
    self->transitionType_length = 0;
    self->type = NULL;
    self->type_length = 0;
    self->visibilityLimit = 0.0f;
    self->blendingSort = NULL;
    self->extra_data = NULL;
}

X3dCallback X3dNavigationInfoRenderCallback = NULL;
X3dCallback X3dNavigationInfoTreeRenderCallback = NULL;
X3dCallback X3dNavigationInfoDoWithDataCallback = NULL;
X3dCallback X3dNavigationInfoTreeDoWithDataCallback = NULL;

X3dProcessEventCallback X3dNavigationInfoProcessEventCallback = NULL;

int X3dNavigationInfoType = 166;

void X3dNavigationInfoRender(X3dNode* self, void *dataptr) {
    if (X3dNavigationInfoRenderCallback)
        X3dNavigationInfoRenderCallback(self, dataptr);
}
void X3dNavigationInfoTreeRender(X3dNode* self, void *dataptr) {
    int i;
    if (((struct X3dNavigationInfo*)self)->m_protoRoot != NULL)
        X3dTreeRenderCallback(((struct X3dNavigationInfo*)self)->m_protoRoot, dataptr);
    else {
        X3dNavigationInfoRender(self, dataptr);
    }
}
void X3dNavigationInfoDoWithData(X3dNode* self, void *dataptr) {
    if (X3dNavigationInfoDoWithDataCallback)
        X3dNavigationInfoDoWithDataCallback(self, dataptr);
}
void X3dNavigationInfoTreeDoWithData(X3dNode* self, void *dataptr) {
    int i;
    if (((struct X3dNavigationInfo*)self)->m_protoRoot != NULL)
        X3dTreeDoWithDataCallback(((struct X3dNavigationInfo*)self)->m_protoRoot, dataptr);
    else {
        X3dNavigationInfoDoWithData(self, dataptr);
    }
}


struct X3dRemoteSelectionGroup {
    X3dNode* m_parent;
    X3dNode* m_protoRoot;
    int m_type;
    void *m_data;
    X3dNode** addChildren;
    int addChildren_length;
    X3dNode** removeChildren;
    int removeChildren_length;
    X3dNode** children;
    int children_length;
    float* bboxCenter;
    int bboxCenter_length;
    float* bboxSize;
    int bboxSize_length;
    void* extra_data;
};

void X3dRemoteSelectionGroupInit(struct X3dRemoteSelectionGroup* self) {
    self->m_protoRoot = NULL;
    self->children = NULL;
    self->children_length = 0;
    self->bboxCenter = NULL;
    self->bboxCenter_length = 0;
    self->bboxSize = NULL;
    self->bboxSize_length = 0;
    self->extra_data = NULL;
}

X3dCallback X3dRemoteSelectionGroupRenderCallback = NULL;
X3dCallback X3dRemoteSelectionGroupTreeRenderCallback = NULL;
X3dCallback X3dRemoteSelectionGroupDoWithDataCallback = NULL;
X3dCallback X3dRemoteSelectionGroupTreeDoWithDataCallback = NULL;

X3dProcessEventCallback X3dRemoteSelectionGroupProcessEventCallback = NULL;

int X3dRemoteSelectionGroupType = 224;

void X3dRemoteSelectionGroupRender(X3dNode* self, void *dataptr) {
    if (X3dRemoteSelectionGroupRenderCallback)
        X3dRemoteSelectionGroupRenderCallback(self, dataptr);
}
void X3dRemoteSelectionGroupTreeRender(X3dNode* self, void *dataptr) {
    int i;
    if (((struct X3dRemoteSelectionGroup*)self)->m_protoRoot != NULL)
        X3dTreeRenderCallback(((struct X3dRemoteSelectionGroup*)self)->m_protoRoot, dataptr);
    else {
        if (((struct X3dRemoteSelectionGroup*)self)->children)
            for (i = 0; i < ((struct X3dRemoteSelectionGroup*)self)->children_length; i++)
                if (((struct X3dRemoteSelectionGroup*)self)->children[i])
                    X3dTreeRenderCallback(((struct X3dRemoteSelectionGroup*)self)->children[i], dataptr);
        X3dRemoteSelectionGroupRender(self, dataptr);
    }
}
void X3dRemoteSelectionGroupDoWithData(X3dNode* self, void *dataptr) {
    if (X3dRemoteSelectionGroupDoWithDataCallback)
        X3dRemoteSelectionGroupDoWithDataCallback(self, dataptr);
}
void X3dRemoteSelectionGroupTreeDoWithData(X3dNode* self, void *dataptr) {
    int i;
    if (((struct X3dRemoteSelectionGroup*)self)->m_protoRoot != NULL)
        X3dTreeDoWithDataCallback(((struct X3dRemoteSelectionGroup*)self)->m_protoRoot, dataptr);
    else {
        if (((struct X3dRemoteSelectionGroup*)self)->children)
            for (i = 0; i < ((struct X3dRemoteSelectionGroup*)self)->children_length; i++)
                if (((struct X3dRemoteSelectionGroup*)self)->children[i])
                    X3dTreeDoWithDataCallback(((struct X3dRemoteSelectionGroup*)self)->children[i], dataptr);
        X3dRemoteSelectionGroupDoWithData(self, dataptr);
    }
}


struct X3dCoordinateInterpolator {
    X3dNode* m_parent;
    X3dNode* m_protoRoot;
    int m_type;
    void *m_data;
    float set_fraction;
    float* value_changed;
    int value_changed_length;
    float* key;
    int key_length;
    float* keyValue;
    int keyValue_length;
    void* extra_data;
};

void X3dCoordinateInterpolatorInit(struct X3dCoordinateInterpolator* self) {
    self->m_protoRoot = NULL;
    self->key = NULL;
    self->key_length = 0;
    self->keyValue = NULL;
    self->keyValue_length = 0;
    self->extra_data = NULL;
}

X3dCallback X3dCoordinateInterpolatorRenderCallback = NULL;
X3dCallback X3dCoordinateInterpolatorTreeRenderCallback = NULL;
X3dCallback X3dCoordinateInterpolatorDoWithDataCallback = NULL;
X3dCallback X3dCoordinateInterpolatorTreeDoWithDataCallback = NULL;

X3dProcessEventCallback X3dCoordinateInterpolatorProcessEventCallback = NULL;

int X3dCoordinateInterpolatorType = 57;

void X3dCoordinateInterpolatorRender(X3dNode* self, void *dataptr) {
    if (X3dCoordinateInterpolatorRenderCallback)
        X3dCoordinateInterpolatorRenderCallback(self, dataptr);
}
void X3dCoordinateInterpolatorTreeRender(X3dNode* self, void *dataptr) {
    int i;
    if (((struct X3dCoordinateInterpolator*)self)->m_protoRoot != NULL)
        X3dTreeRenderCallback(((struct X3dCoordinateInterpolator*)self)->m_protoRoot, dataptr);
    else {
        X3dCoordinateInterpolatorRender(self, dataptr);
    }
}
void X3dCoordinateInterpolatorDoWithData(X3dNode* self, void *dataptr) {
    if (X3dCoordinateInterpolatorDoWithDataCallback)
        X3dCoordinateInterpolatorDoWithDataCallback(self, dataptr);
}
void X3dCoordinateInterpolatorTreeDoWithData(X3dNode* self, void *dataptr) {
    int i;
    if (((struct X3dCoordinateInterpolator*)self)->m_protoRoot != NULL)
        X3dTreeDoWithDataCallback(((struct X3dCoordinateInterpolator*)self)->m_protoRoot, dataptr);
    else {
        X3dCoordinateInterpolatorDoWithData(self, dataptr);
    }
}


struct X3dDirectionalLight {
    X3dNode* m_parent;
    X3dNode* m_protoRoot;
    int m_type;
    void *m_data;
    float ambientIntensity;
    float* color;
    int color_length;
    float* direction;
    int direction_length;
    short global;
    float intensity;
    short on;
    float* projectionRectangle;
    int projectionRectangle_length;
    float* projectionLocation;
    int projectionLocation_length;
    short shadows;
    short kambiShadows;
    short kambiShadowsMain;
    float projectionNear;
    float projectionFar;
    float* up;
    int up_length;
    X3dNode* defaultShadowMap;
    X3dNode** effects;
    int effects_length;
    short shadowVolumes;
    short shadowVolumesMain;
    void* extra_data;
};

void X3dDirectionalLightInit(struct X3dDirectionalLight* self) {
    self->m_protoRoot = NULL;
    self->ambientIntensity = 0.0f;
    self->color = NULL;
    self->color_length = 0;
    self->direction = NULL;
    self->direction_length = 0;
    self->global = 0;
    self->intensity = 0.0f;
    self->on = 0;
    self->projectionRectangle = NULL;
    self->projectionRectangle_length = 0;
    self->projectionLocation = NULL;
    self->projectionLocation_length = 0;
    self->shadows = 0;
    self->kambiShadows = 0;
    self->kambiShadowsMain = 0;
    self->projectionNear = 0.0f;
    self->projectionFar = 0.0f;
    self->up = NULL;
    self->up_length = 0;
    self->defaultShadowMap = NULL;
    self->effects = NULL;
    self->effects_length = 0;
    self->shadowVolumes = 0;
    self->shadowVolumesMain = 0;
    self->extra_data = NULL;
}

X3dCallback X3dDirectionalLightRenderCallback = NULL;
X3dCallback X3dDirectionalLightTreeRenderCallback = NULL;
X3dCallback X3dDirectionalLightDoWithDataCallback = NULL;
X3dCallback X3dDirectionalLightTreeDoWithDataCallback = NULL;

X3dProcessEventCallback X3dDirectionalLightProcessEventCallback = NULL;

int X3dDirectionalLightType = 67;

void X3dDirectionalLightRender(X3dNode* self, void *dataptr) {
    if (X3dDirectionalLightRenderCallback)
        X3dDirectionalLightRenderCallback(self, dataptr);
}
void X3dDirectionalLightTreeRender(X3dNode* self, void *dataptr) {
    int i;
    if (((struct X3dDirectionalLight*)self)->m_protoRoot != NULL)
        X3dTreeRenderCallback(((struct X3dDirectionalLight*)self)->m_protoRoot, dataptr);
    else {
        if (((struct X3dDirectionalLight*)self)->defaultShadowMap)
            X3dTreeRenderCallback(((struct X3dDirectionalLight*)self)->defaultShadowMap, dataptr);
        if (((struct X3dDirectionalLight*)self)->effects)
            for (i = 0; i < ((struct X3dDirectionalLight*)self)->effects_length; i++)
                if (((struct X3dDirectionalLight*)self)->effects[i])
                    X3dTreeRenderCallback(((struct X3dDirectionalLight*)self)->effects[i], dataptr);
        X3dDirectionalLightRender(self, dataptr);
    }
}
void X3dDirectionalLightDoWithData(X3dNode* self, void *dataptr) {
    if (X3dDirectionalLightDoWithDataCallback)
        X3dDirectionalLightDoWithDataCallback(self, dataptr);
}
void X3dDirectionalLightTreeDoWithData(X3dNode* self, void *dataptr) {
    int i;
    if (((struct X3dDirectionalLight*)self)->m_protoRoot != NULL)
        X3dTreeDoWithDataCallback(((struct X3dDirectionalLight*)self)->m_protoRoot, dataptr);
    else {
        if (((struct X3dDirectionalLight*)self)->defaultShadowMap)
            X3dTreeDoWithDataCallback(((struct X3dDirectionalLight*)self)->defaultShadowMap, dataptr);
        if (((struct X3dDirectionalLight*)self)->effects)
            for (i = 0; i < ((struct X3dDirectionalLight*)self)->effects_length; i++)
                if (((struct X3dDirectionalLight*)self)->effects[i])
                    X3dTreeDoWithDataCallback(((struct X3dDirectionalLight*)self)->effects[i], dataptr);
        X3dDirectionalLightDoWithData(self, dataptr);
    }
}


struct X3dCollision {
    X3dNode* m_parent;
    X3dNode* m_protoRoot;
    int m_type;
    void *m_data;
    X3dNode** addChildren;
    int addChildren_length;
    X3dNode** removeChildren;
    int removeChildren_length;
    double collideTime;
    short isActive;
    X3dNode** children;
    int children_length;
    short enabled;
    float* bboxCenter;
    int bboxCenter_length;
    float* bboxSize;
    int bboxSize_length;
    X3dNode* proxy;
    void* extra_data;
};

void X3dCollisionInit(struct X3dCollision* self) {
    self->m_protoRoot = NULL;
    self->children = NULL;
    self->children_length = 0;
    self->enabled = 0;
    self->bboxCenter = NULL;
    self->bboxCenter_length = 0;
    self->bboxSize = NULL;
    self->bboxSize_length = 0;
    self->proxy = NULL;
    self->extra_data = NULL;
}

X3dCallback X3dCollisionRenderCallback = NULL;
X3dCallback X3dCollisionTreeRenderCallback = NULL;
X3dCallback X3dCollisionDoWithDataCallback = NULL;
X3dCallback X3dCollisionTreeDoWithDataCallback = NULL;

X3dProcessEventCallback X3dCollisionProcessEventCallback = NULL;

int X3dCollisionType = 31;

void X3dCollisionRender(X3dNode* self, void *dataptr) {
    if (X3dCollisionRenderCallback)
        X3dCollisionRenderCallback(self, dataptr);
}
void X3dCollisionTreeRender(X3dNode* self, void *dataptr) {
    int i;
    if (((struct X3dCollision*)self)->m_protoRoot != NULL)
        X3dTreeRenderCallback(((struct X3dCollision*)self)->m_protoRoot, dataptr);
    else {
        if (((struct X3dCollision*)self)->children)
            for (i = 0; i < ((struct X3dCollision*)self)->children_length; i++)
                if (((struct X3dCollision*)self)->children[i])
                    X3dTreeRenderCallback(((struct X3dCollision*)self)->children[i], dataptr);
        if (((struct X3dCollision*)self)->proxy)
            X3dTreeRenderCallback(((struct X3dCollision*)self)->proxy, dataptr);
        X3dCollisionRender(self, dataptr);
    }
}
void X3dCollisionDoWithData(X3dNode* self, void *dataptr) {
    if (X3dCollisionDoWithDataCallback)
        X3dCollisionDoWithDataCallback(self, dataptr);
}
void X3dCollisionTreeDoWithData(X3dNode* self, void *dataptr) {
    int i;
    if (((struct X3dCollision*)self)->m_protoRoot != NULL)
        X3dTreeDoWithDataCallback(((struct X3dCollision*)self)->m_protoRoot, dataptr);
    else {
        if (((struct X3dCollision*)self)->children)
            for (i = 0; i < ((struct X3dCollision*)self)->children_length; i++)
                if (((struct X3dCollision*)self)->children[i])
                    X3dTreeDoWithDataCallback(((struct X3dCollision*)self)->children[i], dataptr);
        if (((struct X3dCollision*)self)->proxy)
            X3dTreeDoWithDataCallback(((struct X3dCollision*)self)->proxy, dataptr);
        X3dCollisionDoWithData(self, dataptr);
    }
}


struct X3dNurbsGroup {
    X3dNode* m_parent;
    X3dNode* m_protoRoot;
    int m_type;
    void *m_data;
    X3dNode** addChildren;
    int addChildren_length;
    X3dNode** removeChildren;
    int removeChildren_length;
    X3dNode** children;
    int children_length;
    float* bboxCenter;
    int bboxCenter_length;
    float* bboxSize;
    int bboxSize_length;
    float tessellationScale;
    void* extra_data;
};

void X3dNurbsGroupInit(struct X3dNurbsGroup* self) {
    self->m_protoRoot = NULL;
    self->children = NULL;
    self->children_length = 0;
    self->bboxCenter = NULL;
    self->bboxCenter_length = 0;
    self->bboxSize = NULL;
    self->bboxSize_length = 0;
    self->tessellationScale = 0.0f;
    self->extra_data = NULL;
}

X3dCallback X3dNurbsGroupRenderCallback = NULL;
X3dCallback X3dNurbsGroupTreeRenderCallback = NULL;
X3dCallback X3dNurbsGroupDoWithDataCallback = NULL;
X3dCallback X3dNurbsGroupTreeDoWithDataCallback = NULL;

X3dProcessEventCallback X3dNurbsGroupProcessEventCallback = NULL;

int X3dNurbsGroupType = 172;

void X3dNurbsGroupRender(X3dNode* self, void *dataptr) {
    if (X3dNurbsGroupRenderCallback)
        X3dNurbsGroupRenderCallback(self, dataptr);
}
void X3dNurbsGroupTreeRender(X3dNode* self, void *dataptr) {
    int i;
    if (((struct X3dNurbsGroup*)self)->m_protoRoot != NULL)
        X3dTreeRenderCallback(((struct X3dNurbsGroup*)self)->m_protoRoot, dataptr);
    else {
        if (((struct X3dNurbsGroup*)self)->children)
            for (i = 0; i < ((struct X3dNurbsGroup*)self)->children_length; i++)
                if (((struct X3dNurbsGroup*)self)->children[i])
                    X3dTreeRenderCallback(((struct X3dNurbsGroup*)self)->children[i], dataptr);
        X3dNurbsGroupRender(self, dataptr);
    }
}
void X3dNurbsGroupDoWithData(X3dNode* self, void *dataptr) {
    if (X3dNurbsGroupDoWithDataCallback)
        X3dNurbsGroupDoWithDataCallback(self, dataptr);
}
void X3dNurbsGroupTreeDoWithData(X3dNode* self, void *dataptr) {
    int i;
    if (((struct X3dNurbsGroup*)self)->m_protoRoot != NULL)
        X3dTreeDoWithDataCallback(((struct X3dNurbsGroup*)self)->m_protoRoot, dataptr);
    else {
        if (((struct X3dNurbsGroup*)self)->children)
            for (i = 0; i < ((struct X3dNurbsGroup*)self)->children_length; i++)
                if (((struct X3dNurbsGroup*)self)->children[i])
                    X3dTreeDoWithDataCallback(((struct X3dNurbsGroup*)self)->children[i], dataptr);
        X3dNurbsGroupDoWithData(self, dataptr);
    }
}


struct X3dPointEmitter {
    X3dNode* m_parent;
    X3dNode* m_protoRoot;
    int m_type;
    void *m_data;
    float speed;
    float variation;
    float mass;
    float surfaceArea;
    float* direction;
    int direction_length;
    float* position;
    int position_length;
    void* extra_data;
};

void X3dPointEmitterInit(struct X3dPointEmitter* self) {
    self->m_protoRoot = NULL;
    self->speed = 0.0f;
    self->variation = 0.0f;
    self->mass = 0.0f;
    self->surfaceArea = 0.0f;
    self->direction = NULL;
    self->direction_length = 0;
    self->position = NULL;
    self->position_length = 0;
    self->extra_data = NULL;
}

X3dCallback X3dPointEmitterRenderCallback = NULL;
X3dCallback X3dPointEmitterTreeRenderCallback = NULL;
X3dCallback X3dPointEmitterDoWithDataCallback = NULL;
X3dCallback X3dPointEmitterTreeDoWithDataCallback = NULL;

X3dProcessEventCallback X3dPointEmitterProcessEventCallback = NULL;

int X3dPointEmitterType = 197;

void X3dPointEmitterRender(X3dNode* self, void *dataptr) {
    if (X3dPointEmitterRenderCallback)
        X3dPointEmitterRenderCallback(self, dataptr);
}
void X3dPointEmitterTreeRender(X3dNode* self, void *dataptr) {
    int i;
    if (((struct X3dPointEmitter*)self)->m_protoRoot != NULL)
        X3dTreeRenderCallback(((struct X3dPointEmitter*)self)->m_protoRoot, dataptr);
    else {
        X3dPointEmitterRender(self, dataptr);
    }
}
void X3dPointEmitterDoWithData(X3dNode* self, void *dataptr) {
    if (X3dPointEmitterDoWithDataCallback)
        X3dPointEmitterDoWithDataCallback(self, dataptr);
}
void X3dPointEmitterTreeDoWithData(X3dNode* self, void *dataptr) {
    int i;
    if (((struct X3dPointEmitter*)self)->m_protoRoot != NULL)
        X3dTreeDoWithDataCallback(((struct X3dPointEmitter*)self)->m_protoRoot, dataptr);
    else {
        X3dPointEmitterDoWithData(self, dataptr);
    }
}


struct X3dProximitySensor {
    X3dNode* m_parent;
    X3dNode* m_protoRoot;
    int m_type;
    void *m_data;
    double enterTime;
    double exitTime;
    float* centerOfRotation_changed;
    int centerOfRotation_changed_length;
    short isActive;
    float* position_changed;
    int position_changed_length;
    float* orientation_changed;
    int orientation_changed_length;
    float* center;
    int center_length;
    short enabled;
    float* size;
    int size_length;
    void* extra_data;
};

void X3dProximitySensorInit(struct X3dProximitySensor* self) {
    self->m_protoRoot = NULL;
    self->center = NULL;
    self->center_length = 0;
    self->enabled = 0;
    self->size = NULL;
    self->size_length = 0;
    self->extra_data = NULL;
}

X3dCallback X3dProximitySensorRenderCallback = NULL;
X3dCallback X3dProximitySensorTreeRenderCallback = NULL;
X3dCallback X3dProximitySensorDoWithDataCallback = NULL;
X3dCallback X3dProximitySensorTreeDoWithDataCallback = NULL;

X3dProcessEventCallback X3dProximitySensorProcessEventCallback = NULL;

int X3dProximitySensorType = 216;

void X3dProximitySensorRender(X3dNode* self, void *dataptr) {
    if (X3dProximitySensorRenderCallback)
        X3dProximitySensorRenderCallback(self, dataptr);
}
void X3dProximitySensorTreeRender(X3dNode* self, void *dataptr) {
    int i;
    if (((struct X3dProximitySensor*)self)->m_protoRoot != NULL)
        X3dTreeRenderCallback(((struct X3dProximitySensor*)self)->m_protoRoot, dataptr);
    else {
        X3dProximitySensorRender(self, dataptr);
    }
}
void X3dProximitySensorDoWithData(X3dNode* self, void *dataptr) {
    if (X3dProximitySensorDoWithDataCallback)
        X3dProximitySensorDoWithDataCallback(self, dataptr);
}
void X3dProximitySensorTreeDoWithData(X3dNode* self, void *dataptr) {
    int i;
    if (((struct X3dProximitySensor*)self)->m_protoRoot != NULL)
        X3dTreeDoWithDataCallback(((struct X3dProximitySensor*)self)->m_protoRoot, dataptr);
    else {
        X3dProximitySensorDoWithData(self, dataptr);
    }
}


struct X3dHAnimHumanoid {
    X3dNode* m_parent;
    X3dNode* m_protoRoot;
    int m_type;
    void *m_data;
    float* rotation;
    int rotation_length;
    float* translation;
    int translation_length;
    float* bboxCenter;
    int bboxCenter_length;
    float* bboxSize;
    int bboxSize_length;
    float* center;
    int center_length;
    float* scale;
    int scale_length;
    float* scaleOrientation;
    int scaleOrientation_length;
    const char** info;
    int info_length;
    X3dNode** joints;
    int joints_length;
    const char* name;
    X3dNode** segments;
    int segments_length;
    X3dNode** sites;
    int sites_length;
    X3dNode** skeleton;
    int skeleton_length;
    X3dNode** skin;
    int skin_length;
    X3dNode* skinCoord;
    X3dNode* skinNormal;
    const char* version;
    X3dNode** viewpoints;
    int viewpoints_length;
    void* extra_data;
};

void X3dHAnimHumanoidInit(struct X3dHAnimHumanoid* self) {
    self->m_protoRoot = NULL;
    self->rotation = NULL;
    self->rotation_length = 0;
    self->translation = NULL;
    self->translation_length = 0;
    self->bboxCenter = NULL;
    self->bboxCenter_length = 0;
    self->bboxSize = NULL;
    self->bboxSize_length = 0;
    self->center = NULL;
    self->center_length = 0;
    self->scale = NULL;
    self->scale_length = 0;
    self->scaleOrientation = NULL;
    self->scaleOrientation_length = 0;
    self->info = NULL;
    self->info_length = 0;
    self->joints = NULL;
    self->joints_length = 0;
    self->name = NULL;
    self->segments = NULL;
    self->segments_length = 0;
    self->sites = NULL;
    self->sites_length = 0;
    self->skeleton = NULL;
    self->skeleton_length = 0;
    self->skin = NULL;
    self->skin_length = 0;
    self->skinCoord = NULL;
    self->skinNormal = NULL;
    self->version = NULL;
    self->viewpoints = NULL;
    self->viewpoints_length = 0;
    self->extra_data = NULL;
}

X3dCallback X3dHAnimHumanoidRenderCallback = NULL;
X3dCallback X3dHAnimHumanoidTreeRenderCallback = NULL;
X3dCallback X3dHAnimHumanoidDoWithDataCallback = NULL;
X3dCallback X3dHAnimHumanoidTreeDoWithDataCallback = NULL;

X3dProcessEventCallback X3dHAnimHumanoidProcessEventCallback = NULL;

int X3dHAnimHumanoidType = 106;

void X3dHAnimHumanoidRender(X3dNode* self, void *dataptr) {
    if (X3dHAnimHumanoidRenderCallback)
        X3dHAnimHumanoidRenderCallback(self, dataptr);
}
void X3dHAnimHumanoidTreeRender(X3dNode* self, void *dataptr) {
    int i;
    if (((struct X3dHAnimHumanoid*)self)->m_protoRoot != NULL)
        X3dTreeRenderCallback(((struct X3dHAnimHumanoid*)self)->m_protoRoot, dataptr);
    else {
        if (((struct X3dHAnimHumanoid*)self)->joints)
            for (i = 0; i < ((struct X3dHAnimHumanoid*)self)->joints_length; i++)
                if (((struct X3dHAnimHumanoid*)self)->joints[i])
                    X3dTreeRenderCallback(((struct X3dHAnimHumanoid*)self)->joints[i], dataptr);
        if (((struct X3dHAnimHumanoid*)self)->segments)
            for (i = 0; i < ((struct X3dHAnimHumanoid*)self)->segments_length; i++)
                if (((struct X3dHAnimHumanoid*)self)->segments[i])
                    X3dTreeRenderCallback(((struct X3dHAnimHumanoid*)self)->segments[i], dataptr);
        if (((struct X3dHAnimHumanoid*)self)->sites)
            for (i = 0; i < ((struct X3dHAnimHumanoid*)self)->sites_length; i++)
                if (((struct X3dHAnimHumanoid*)self)->sites[i])
                    X3dTreeRenderCallback(((struct X3dHAnimHumanoid*)self)->sites[i], dataptr);
        if (((struct X3dHAnimHumanoid*)self)->skeleton)
            for (i = 0; i < ((struct X3dHAnimHumanoid*)self)->skeleton_length; i++)
                if (((struct X3dHAnimHumanoid*)self)->skeleton[i])
                    X3dTreeRenderCallback(((struct X3dHAnimHumanoid*)self)->skeleton[i], dataptr);
        if (((struct X3dHAnimHumanoid*)self)->skin)
            for (i = 0; i < ((struct X3dHAnimHumanoid*)self)->skin_length; i++)
                if (((struct X3dHAnimHumanoid*)self)->skin[i])
                    X3dTreeRenderCallback(((struct X3dHAnimHumanoid*)self)->skin[i], dataptr);
        if (((struct X3dHAnimHumanoid*)self)->skinCoord)
            X3dTreeRenderCallback(((struct X3dHAnimHumanoid*)self)->skinCoord, dataptr);
        if (((struct X3dHAnimHumanoid*)self)->skinNormal)
            X3dTreeRenderCallback(((struct X3dHAnimHumanoid*)self)->skinNormal, dataptr);
        if (((struct X3dHAnimHumanoid*)self)->viewpoints)
            for (i = 0; i < ((struct X3dHAnimHumanoid*)self)->viewpoints_length; i++)
                if (((struct X3dHAnimHumanoid*)self)->viewpoints[i])
                    X3dTreeRenderCallback(((struct X3dHAnimHumanoid*)self)->viewpoints[i], dataptr);
        X3dHAnimHumanoidRender(self, dataptr);
    }
}
void X3dHAnimHumanoidDoWithData(X3dNode* self, void *dataptr) {
    if (X3dHAnimHumanoidDoWithDataCallback)
        X3dHAnimHumanoidDoWithDataCallback(self, dataptr);
}
void X3dHAnimHumanoidTreeDoWithData(X3dNode* self, void *dataptr) {
    int i;
    if (((struct X3dHAnimHumanoid*)self)->m_protoRoot != NULL)
        X3dTreeDoWithDataCallback(((struct X3dHAnimHumanoid*)self)->m_protoRoot, dataptr);
    else {
        if (((struct X3dHAnimHumanoid*)self)->joints)
            for (i = 0; i < ((struct X3dHAnimHumanoid*)self)->joints_length; i++)
                if (((struct X3dHAnimHumanoid*)self)->joints[i])
                    X3dTreeDoWithDataCallback(((struct X3dHAnimHumanoid*)self)->joints[i], dataptr);
        if (((struct X3dHAnimHumanoid*)self)->segments)
            for (i = 0; i < ((struct X3dHAnimHumanoid*)self)->segments_length; i++)
                if (((struct X3dHAnimHumanoid*)self)->segments[i])
                    X3dTreeDoWithDataCallback(((struct X3dHAnimHumanoid*)self)->segments[i], dataptr);
        if (((struct X3dHAnimHumanoid*)self)->sites)
            for (i = 0; i < ((struct X3dHAnimHumanoid*)self)->sites_length; i++)
                if (((struct X3dHAnimHumanoid*)self)->sites[i])
                    X3dTreeDoWithDataCallback(((struct X3dHAnimHumanoid*)self)->sites[i], dataptr);
        if (((struct X3dHAnimHumanoid*)self)->skeleton)
            for (i = 0; i < ((struct X3dHAnimHumanoid*)self)->skeleton_length; i++)
                if (((struct X3dHAnimHumanoid*)self)->skeleton[i])
                    X3dTreeDoWithDataCallback(((struct X3dHAnimHumanoid*)self)->skeleton[i], dataptr);
        if (((struct X3dHAnimHumanoid*)self)->skin)
            for (i = 0; i < ((struct X3dHAnimHumanoid*)self)->skin_length; i++)
                if (((struct X3dHAnimHumanoid*)self)->skin[i])
                    X3dTreeDoWithDataCallback(((struct X3dHAnimHumanoid*)self)->skin[i], dataptr);
        if (((struct X3dHAnimHumanoid*)self)->skinCoord)
            X3dTreeDoWithDataCallback(((struct X3dHAnimHumanoid*)self)->skinCoord, dataptr);
        if (((struct X3dHAnimHumanoid*)self)->skinNormal)
            X3dTreeDoWithDataCallback(((struct X3dHAnimHumanoid*)self)->skinNormal, dataptr);
        if (((struct X3dHAnimHumanoid*)self)->viewpoints)
            for (i = 0; i < ((struct X3dHAnimHumanoid*)self)->viewpoints_length; i++)
                if (((struct X3dHAnimHumanoid*)self)->viewpoints[i])
                    X3dTreeDoWithDataCallback(((struct X3dHAnimHumanoid*)self)->viewpoints[i], dataptr);
        X3dHAnimHumanoidDoWithData(self, dataptr);
    }
}


struct X3dIndexedFaceSet {
    X3dNode* m_parent;
    X3dNode* m_protoRoot;
    int m_type;
    void *m_data;
    int* set_colorIndex;
    int set_colorIndex_length;
    int* set_coordIndex;
    int set_coordIndex_length;
    int* set_normalIndex;
    int set_normalIndex_length;
    int* set_texCoordIndex;
    int set_texCoordIndex_length;
    X3dNode* color;
    X3dNode* coord;
    X3dNode* normal;
    X3dNode* texCoord;
    short ccw;
    int* colorIndex;
    int colorIndex_length;
    short colorPerVertex;
    short convex;
    int* coordIndex;
    int coordIndex_length;
    float creaseAngle;
    int* normalIndex;
    int normalIndex_length;
    short normalPerVertex;
    short solid;
    int* texCoordIndex;
    int texCoordIndex_length;
    float* radianceTransfer;
    int radianceTransfer_length;
    X3dNode** attrib;
    int attrib_length;
    X3dNode* fogCoord;
    void* extra_data;
    int glName_number;
};

    void X3dSetGlName(struct X3dIndexedFaceSet* self, int number)
{
    self->glName_number = number;
}

void X3dIndexedFaceSetInit(struct X3dIndexedFaceSet* self) {
    self->m_protoRoot = NULL;
    self->color = NULL;
    self->coord = NULL;
    self->normal = NULL;
    self->texCoord = NULL;
    self->ccw = 0;
    self->colorIndex = NULL;
    self->colorIndex_length = 0;
    self->colorPerVertex = 0;
    self->convex = 0;
    self->coordIndex = NULL;
    self->coordIndex_length = 0;
    self->creaseAngle = 0.0f;
    self->normalIndex = NULL;
    self->normalIndex_length = 0;
    self->normalPerVertex = 0;
    self->solid = 0;
    self->texCoordIndex = NULL;
    self->texCoordIndex_length = 0;
    self->radianceTransfer = NULL;
    self->radianceTransfer_length = 0;
    self->attrib = NULL;
    self->attrib_length = 0;
    self->fogCoord = NULL;
    self->extra_data = NULL;
}

X3dCallback X3dIndexedFaceSetRenderCallback = NULL;
X3dCallback X3dIndexedFaceSetTreeRenderCallback = NULL;
X3dCallback X3dIndexedFaceSetCreateNormalsCallback = NULL;
X3dCallback X3dIndexedFaceSetDoWithDataCallback = NULL;
X3dCallback X3dIndexedFaceSetTreeDoWithDataCallback = NULL;

X3dProcessEventCallback X3dIndexedFaceSetProcessEventCallback = NULL;

int X3dIndexedFaceSetType = 115;

void X3dIndexedFaceSetRender(X3dNode* self, void *dataptr) {
    if (X3dIndexedFaceSetRenderCallback)
        X3dIndexedFaceSetRenderCallback(self, dataptr);
}
void X3dIndexedFaceSetTreeRender(X3dNode* self, void *dataptr) {
    int i;
    if (((struct X3dIndexedFaceSet*)self)->m_protoRoot != NULL)
        X3dTreeRenderCallback(((struct X3dIndexedFaceSet*)self)->m_protoRoot, dataptr);
    else {
        if (((struct X3dIndexedFaceSet*)self)->color)
            X3dTreeRenderCallback(((struct X3dIndexedFaceSet*)self)->color, dataptr);
        if (((struct X3dIndexedFaceSet*)self)->coord)
            X3dTreeRenderCallback(((struct X3dIndexedFaceSet*)self)->coord, dataptr);
        if (((struct X3dIndexedFaceSet*)self)->normal)
            X3dTreeRenderCallback(((struct X3dIndexedFaceSet*)self)->normal, dataptr);
        if (((struct X3dIndexedFaceSet*)self)->texCoord)
            X3dTreeRenderCallback(((struct X3dIndexedFaceSet*)self)->texCoord, dataptr);
        if (((struct X3dIndexedFaceSet*)self)->attrib)
            for (i = 0; i < ((struct X3dIndexedFaceSet*)self)->attrib_length; i++)
                if (((struct X3dIndexedFaceSet*)self)->attrib[i])
                    X3dTreeRenderCallback(((struct X3dIndexedFaceSet*)self)->attrib[i], dataptr);
        if (((struct X3dIndexedFaceSet*)self)->fogCoord)
            X3dTreeRenderCallback(((struct X3dIndexedFaceSet*)self)->fogCoord, dataptr);
        X3dIndexedFaceSetRender(self, dataptr);
    }
}
void X3dIndexedFaceSetCreateNormals(X3dNode* self, void *dataptr) {
    if (X3dIndexedFaceSetCreateNormalsCallback)
        X3dIndexedFaceSetCreateNormalsCallback(self, dataptr);
}
void X3dIndexedFaceSetDoWithData(X3dNode* self, void *dataptr) {
    if (X3dIndexedFaceSetDoWithDataCallback)
        X3dIndexedFaceSetDoWithDataCallback(self, dataptr);
}
void X3dIndexedFaceSetTreeDoWithData(X3dNode* self, void *dataptr) {
    int i;
    if (((struct X3dIndexedFaceSet*)self)->m_protoRoot != NULL)
        X3dTreeDoWithDataCallback(((struct X3dIndexedFaceSet*)self)->m_protoRoot, dataptr);
    else {
        if (((struct X3dIndexedFaceSet*)self)->color)
            X3dTreeDoWithDataCallback(((struct X3dIndexedFaceSet*)self)->color, dataptr);
        if (((struct X3dIndexedFaceSet*)self)->coord)
            X3dTreeDoWithDataCallback(((struct X3dIndexedFaceSet*)self)->coord, dataptr);
        if (((struct X3dIndexedFaceSet*)self)->normal)
            X3dTreeDoWithDataCallback(((struct X3dIndexedFaceSet*)self)->normal, dataptr);
        if (((struct X3dIndexedFaceSet*)self)->texCoord)
            X3dTreeDoWithDataCallback(((struct X3dIndexedFaceSet*)self)->texCoord, dataptr);
        if (((struct X3dIndexedFaceSet*)self)->attrib)
            for (i = 0; i < ((struct X3dIndexedFaceSet*)self)->attrib_length; i++)
                if (((struct X3dIndexedFaceSet*)self)->attrib[i])
                    X3dTreeDoWithDataCallback(((struct X3dIndexedFaceSet*)self)->attrib[i], dataptr);
        if (((struct X3dIndexedFaceSet*)self)->fogCoord)
            X3dTreeDoWithDataCallback(((struct X3dIndexedFaceSet*)self)->fogCoord, dataptr);
        X3dIndexedFaceSetDoWithData(self, dataptr);
    }
}


struct X3dTriangleSet {
    X3dNode* m_parent;
    X3dNode* m_protoRoot;
    int m_type;
    void *m_data;
    X3dNode* color;
    X3dNode* coord;
    X3dNode* normal;
    X3dNode* texCoord;
    short ccw;
    short colorPerVertex;
    short normalPerVertex;
    short solid;
    float* radianceTransfer;
    int radianceTransfer_length;
    X3dNode** attrib;
    int attrib_length;
    X3dNode* fogCoord;
    void* extra_data;
};

void X3dTriangleSetInit(struct X3dTriangleSet* self) {
    self->m_protoRoot = NULL;
    self->color = NULL;
    self->coord = NULL;
    self->normal = NULL;
    self->texCoord = NULL;
    self->ccw = 0;
    self->colorPerVertex = 0;
    self->normalPerVertex = 0;
    self->solid = 0;
    self->radianceTransfer = NULL;
    self->radianceTransfer_length = 0;
    self->attrib = NULL;
    self->attrib_length = 0;
    self->fogCoord = NULL;
    self->extra_data = NULL;
}

X3dCallback X3dTriangleSetRenderCallback = NULL;
X3dCallback X3dTriangleSetTreeRenderCallback = NULL;
X3dCallback X3dTriangleSetDoWithDataCallback = NULL;
X3dCallback X3dTriangleSetTreeDoWithDataCallback = NULL;

X3dProcessEventCallback X3dTriangleSetProcessEventCallback = NULL;

int X3dTriangleSetType = 293;

void X3dTriangleSetRender(X3dNode* self, void *dataptr) {
    if (X3dTriangleSetRenderCallback)
        X3dTriangleSetRenderCallback(self, dataptr);
}
void X3dTriangleSetTreeRender(X3dNode* self, void *dataptr) {
    int i;
    if (((struct X3dTriangleSet*)self)->m_protoRoot != NULL)
        X3dTreeRenderCallback(((struct X3dTriangleSet*)self)->m_protoRoot, dataptr);
    else {
        if (((struct X3dTriangleSet*)self)->color)
            X3dTreeRenderCallback(((struct X3dTriangleSet*)self)->color, dataptr);
        if (((struct X3dTriangleSet*)self)->coord)
            X3dTreeRenderCallback(((struct X3dTriangleSet*)self)->coord, dataptr);
        if (((struct X3dTriangleSet*)self)->normal)
            X3dTreeRenderCallback(((struct X3dTriangleSet*)self)->normal, dataptr);
        if (((struct X3dTriangleSet*)self)->texCoord)
            X3dTreeRenderCallback(((struct X3dTriangleSet*)self)->texCoord, dataptr);
        if (((struct X3dTriangleSet*)self)->attrib)
            for (i = 0; i < ((struct X3dTriangleSet*)self)->attrib_length; i++)
                if (((struct X3dTriangleSet*)self)->attrib[i])
                    X3dTreeRenderCallback(((struct X3dTriangleSet*)self)->attrib[i], dataptr);
        if (((struct X3dTriangleSet*)self)->fogCoord)
            X3dTreeRenderCallback(((struct X3dTriangleSet*)self)->fogCoord, dataptr);
        X3dTriangleSetRender(self, dataptr);
    }
}
void X3dTriangleSetDoWithData(X3dNode* self, void *dataptr) {
    if (X3dTriangleSetDoWithDataCallback)
        X3dTriangleSetDoWithDataCallback(self, dataptr);
}
void X3dTriangleSetTreeDoWithData(X3dNode* self, void *dataptr) {
    int i;
    if (((struct X3dTriangleSet*)self)->m_protoRoot != NULL)
        X3dTreeDoWithDataCallback(((struct X3dTriangleSet*)self)->m_protoRoot, dataptr);
    else {
        if (((struct X3dTriangleSet*)self)->color)
            X3dTreeDoWithDataCallback(((struct X3dTriangleSet*)self)->color, dataptr);
        if (((struct X3dTriangleSet*)self)->coord)
            X3dTreeDoWithDataCallback(((struct X3dTriangleSet*)self)->coord, dataptr);
        if (((struct X3dTriangleSet*)self)->normal)
            X3dTreeDoWithDataCallback(((struct X3dTriangleSet*)self)->normal, dataptr);
        if (((struct X3dTriangleSet*)self)->texCoord)
            X3dTreeDoWithDataCallback(((struct X3dTriangleSet*)self)->texCoord, dataptr);
        if (((struct X3dTriangleSet*)self)->attrib)
            for (i = 0; i < ((struct X3dTriangleSet*)self)->attrib_length; i++)
                if (((struct X3dTriangleSet*)self)->attrib[i])
                    X3dTreeDoWithDataCallback(((struct X3dTriangleSet*)self)->attrib[i], dataptr);
        if (((struct X3dTriangleSet*)self)->fogCoord)
            X3dTreeDoWithDataCallback(((struct X3dTriangleSet*)self)->fogCoord, dataptr);
        X3dTriangleSetDoWithData(self, dataptr);
    }
}


struct X3dFontStyle {
    X3dNode* m_parent;
    X3dNode* m_protoRoot;
    int m_type;
    void *m_data;
    const char** family;
    int family_length;
    short horizontal;
    const char** justify;
    int justify_length;
    const char* language;
    short leftToRight;
    float spacing;
    const char* style;
    short topToBottom;
    float size;
    void* extra_data;
};

void X3dFontStyleInit(struct X3dFontStyle* self) {
    self->m_protoRoot = NULL;
    self->family = NULL;
    self->family_length = 0;
    self->horizontal = 0;
    self->justify = NULL;
    self->justify_length = 0;
    self->language = NULL;
    self->leftToRight = 0;
    self->spacing = 0.0f;
    self->style = NULL;
    self->topToBottom = 0;
    self->size = 0.0f;
    self->extra_data = NULL;
}

X3dCallback X3dFontStyleRenderCallback = NULL;
X3dCallback X3dFontStyleTreeRenderCallback = NULL;
X3dCallback X3dFontStyleDoWithDataCallback = NULL;
X3dCallback X3dFontStyleTreeDoWithDataCallback = NULL;

X3dProcessEventCallback X3dFontStyleProcessEventCallback = NULL;

int X3dFontStyleType = 89;

void X3dFontStyleRender(X3dNode* self, void *dataptr) {
    if (X3dFontStyleRenderCallback)
        X3dFontStyleRenderCallback(self, dataptr);
}
void X3dFontStyleTreeRender(X3dNode* self, void *dataptr) {
    int i;
    if (((struct X3dFontStyle*)self)->m_protoRoot != NULL)
        X3dTreeRenderCallback(((struct X3dFontStyle*)self)->m_protoRoot, dataptr);
    else {
        X3dFontStyleRender(self, dataptr);
    }
}
void X3dFontStyleDoWithData(X3dNode* self, void *dataptr) {
    if (X3dFontStyleDoWithDataCallback)
        X3dFontStyleDoWithDataCallback(self, dataptr);
}
void X3dFontStyleTreeDoWithData(X3dNode* self, void *dataptr) {
    int i;
    if (((struct X3dFontStyle*)self)->m_protoRoot != NULL)
        X3dTreeDoWithDataCallback(((struct X3dFontStyle*)self)->m_protoRoot, dataptr);
    else {
        X3dFontStyleDoWithData(self, dataptr);
    }
}


struct X3dMaterial {
    X3dNode* m_parent;
    X3dNode* m_protoRoot;
    int m_type;
    void *m_data;
    float ambientIntensity;
    float* diffuseColor;
    int diffuseColor_length;
    float* emissiveColor;
    int emissiveColor_length;
    float shininess;
    float* specularColor;
    int specularColor_length;
    float transparency;
    float mirror;
    float* reflSpecular;
    int reflSpecular_length;
    float* reflDiffuse;
    int reflDiffuse_length;
    float* transSpecular;
    int transSpecular_length;
    float* transDiffuse;
    int transDiffuse_length;
    float reflSpecularExp;
    float transSpecularExp;
    void* extra_data;
};

void X3dMaterialInit(struct X3dMaterial* self) {
    self->m_protoRoot = NULL;
    self->ambientIntensity = 0.0f;
    self->diffuseColor = NULL;
    self->diffuseColor_length = 0;
    self->emissiveColor = NULL;
    self->emissiveColor_length = 0;
    self->shininess = 0.0f;
    self->specularColor = NULL;
    self->specularColor_length = 0;
    self->transparency = 0.0f;
    self->mirror = 0.0f;
    self->reflSpecular = NULL;
    self->reflSpecular_length = 0;
    self->reflDiffuse = NULL;
    self->reflDiffuse_length = 0;
    self->transSpecular = NULL;
    self->transSpecular_length = 0;
    self->transDiffuse = NULL;
    self->transDiffuse_length = 0;
    self->reflSpecularExp = 0.0f;
    self->transSpecularExp = 0.0f;
    self->extra_data = NULL;
}

X3dCallback X3dMaterialRenderCallback = NULL;
X3dCallback X3dMaterialTreeRenderCallback = NULL;
X3dCallback X3dMaterialDoWithDataCallback = NULL;
X3dCallback X3dMaterialTreeDoWithDataCallback = NULL;

X3dProcessEventCallback X3dMaterialProcessEventCallback = NULL;

int X3dMaterialType = 147;

void X3dMaterialRender(X3dNode* self, void *dataptr) {
    if (X3dMaterialRenderCallback)
        X3dMaterialRenderCallback(self, dataptr);
}
void X3dMaterialTreeRender(X3dNode* self, void *dataptr) {
    int i;
    if (((struct X3dMaterial*)self)->m_protoRoot != NULL)
        X3dTreeRenderCallback(((struct X3dMaterial*)self)->m_protoRoot, dataptr);
    else {
        X3dMaterialRender(self, dataptr);
    }
}
void X3dMaterialDoWithData(X3dNode* self, void *dataptr) {
    if (X3dMaterialDoWithDataCallback)
        X3dMaterialDoWithDataCallback(self, dataptr);
}
void X3dMaterialTreeDoWithData(X3dNode* self, void *dataptr) {
    int i;
    if (((struct X3dMaterial*)self)->m_protoRoot != NULL)
        X3dTreeDoWithDataCallback(((struct X3dMaterial*)self)->m_protoRoot, dataptr);
    else {
        X3dMaterialDoWithData(self, dataptr);
    }
}


struct X3dConeEmitter {
    X3dNode* m_parent;
    X3dNode* m_protoRoot;
    int m_type;
    void *m_data;
    float speed;
    float variation;
    float mass;
    float surfaceArea;
    float angle;
    float* direction;
    int direction_length;
    float* position;
    int position_length;
    void* extra_data;
};

void X3dConeEmitterInit(struct X3dConeEmitter* self) {
    self->m_protoRoot = NULL;
    self->speed = 0.0f;
    self->variation = 0.0f;
    self->mass = 0.0f;
    self->surfaceArea = 0.0f;
    self->angle = 0.0f;
    self->direction = NULL;
    self->direction_length = 0;
    self->position = NULL;
    self->position_length = 0;
    self->extra_data = NULL;
}

X3dCallback X3dConeEmitterRenderCallback = NULL;
X3dCallback X3dConeEmitterTreeRenderCallback = NULL;
X3dCallback X3dConeEmitterDoWithDataCallback = NULL;
X3dCallback X3dConeEmitterTreeDoWithDataCallback = NULL;

X3dProcessEventCallback X3dConeEmitterProcessEventCallback = NULL;

int X3dConeEmitterType = 48;

void X3dConeEmitterRender(X3dNode* self, void *dataptr) {
    if (X3dConeEmitterRenderCallback)
        X3dConeEmitterRenderCallback(self, dataptr);
}
void X3dConeEmitterTreeRender(X3dNode* self, void *dataptr) {
    int i;
    if (((struct X3dConeEmitter*)self)->m_protoRoot != NULL)
        X3dTreeRenderCallback(((struct X3dConeEmitter*)self)->m_protoRoot, dataptr);
    else {
        X3dConeEmitterRender(self, dataptr);
    }
}
void X3dConeEmitterDoWithData(X3dNode* self, void *dataptr) {
    if (X3dConeEmitterDoWithDataCallback)
        X3dConeEmitterDoWithDataCallback(self, dataptr);
}
void X3dConeEmitterTreeDoWithData(X3dNode* self, void *dataptr) {
    int i;
    if (((struct X3dConeEmitter*)self)->m_protoRoot != NULL)
        X3dTreeDoWithDataCallback(((struct X3dConeEmitter*)self)->m_protoRoot, dataptr);
    else {
        X3dConeEmitterDoWithData(self, dataptr);
    }
}


struct X3dOrientationInterpolator {
    X3dNode* m_parent;
    X3dNode* m_protoRoot;
    int m_type;
    void *m_data;
    float set_fraction;
    float* value_changed;
    int value_changed_length;
    float* key;
    int key_length;
    float* keyValue;
    int keyValue_length;
    void* extra_data;
};

void X3dOrientationInterpolatorInit(struct X3dOrientationInterpolator* self) {
    self->m_protoRoot = NULL;
    self->key = NULL;
    self->key_length = 0;
    self->keyValue = NULL;
    self->keyValue_length = 0;
    self->extra_data = NULL;
}

X3dCallback X3dOrientationInterpolatorRenderCallback = NULL;
X3dCallback X3dOrientationInterpolatorTreeRenderCallback = NULL;
X3dCallback X3dOrientationInterpolatorDoWithDataCallback = NULL;
X3dCallback X3dOrientationInterpolatorTreeDoWithDataCallback = NULL;

X3dProcessEventCallback X3dOrientationInterpolatorProcessEventCallback = NULL;

int X3dOrientationInterpolatorType = 186;

void X3dOrientationInterpolatorRender(X3dNode* self, void *dataptr) {
    if (X3dOrientationInterpolatorRenderCallback)
        X3dOrientationInterpolatorRenderCallback(self, dataptr);
}
void X3dOrientationInterpolatorTreeRender(X3dNode* self, void *dataptr) {
    int i;
    if (((struct X3dOrientationInterpolator*)self)->m_protoRoot != NULL)
        X3dTreeRenderCallback(((struct X3dOrientationInterpolator*)self)->m_protoRoot, dataptr);
    else {
        X3dOrientationInterpolatorRender(self, dataptr);
    }
}
void X3dOrientationInterpolatorDoWithData(X3dNode* self, void *dataptr) {
    if (X3dOrientationInterpolatorDoWithDataCallback)
        X3dOrientationInterpolatorDoWithDataCallback(self, dataptr);
}
void X3dOrientationInterpolatorTreeDoWithData(X3dNode* self, void *dataptr) {
    int i;
    if (((struct X3dOrientationInterpolator*)self)->m_protoRoot != NULL)
        X3dTreeDoWithDataCallback(((struct X3dOrientationInterpolator*)self)->m_protoRoot, dataptr);
    else {
        X3dOrientationInterpolatorDoWithData(self, dataptr);
    }
}


struct X3dTimeSensor {
    X3dNode* m_parent;
    X3dNode* m_protoRoot;
    int m_type;
    void *m_data;
    double cycleTime;
    double elapsedTime;
    float fraction_changed;
    short isActive;
    short isPaused;
    double time;
    double cycleInterval;
    short enabled;
    short loop;
    double pauseTime;
    double resumeTime;
    double startTime;
    double stopTime;
    void* extra_data;
};

void X3dTimeSensorInit(struct X3dTimeSensor* self) {
    self->m_protoRoot = NULL;
    self->cycleInterval = 0;
    self->enabled = 0;
    self->loop = 0;
    self->pauseTime = 0;
    self->resumeTime = 0;
    self->startTime = 0;
    self->stopTime = 0;
    self->extra_data = NULL;
}

X3dCallback X3dTimeSensorRenderCallback = NULL;
X3dCallback X3dTimeSensorTreeRenderCallback = NULL;
X3dCallback X3dTimeSensorDoWithDataCallback = NULL;
X3dCallback X3dTimeSensorTreeDoWithDataCallback = NULL;

X3dProcessEventCallback X3dTimeSensorProcessEventCallback = NULL;

int X3dTimeSensorType = 284;

void X3dTimeSensorRender(X3dNode* self, void *dataptr) {
    if (X3dTimeSensorRenderCallback)
        X3dTimeSensorRenderCallback(self, dataptr);
}
void X3dTimeSensorTreeRender(X3dNode* self, void *dataptr) {
    int i;
    if (((struct X3dTimeSensor*)self)->m_protoRoot != NULL)
        X3dTreeRenderCallback(((struct X3dTimeSensor*)self)->m_protoRoot, dataptr);
    else {
        X3dTimeSensorRender(self, dataptr);
    }
}
void X3dTimeSensorDoWithData(X3dNode* self, void *dataptr) {
    if (X3dTimeSensorDoWithDataCallback)
        X3dTimeSensorDoWithDataCallback(self, dataptr);
}
void X3dTimeSensorTreeDoWithData(X3dNode* self, void *dataptr) {
    int i;
    if (((struct X3dTimeSensor*)self)->m_protoRoot != NULL)
        X3dTreeDoWithDataCallback(((struct X3dTimeSensor*)self)->m_protoRoot, dataptr);
    else {
        X3dTimeSensorDoWithData(self, dataptr);
    }
}


struct X3dIndexedLineSet {
    X3dNode* m_parent;
    X3dNode* m_protoRoot;
    int m_type;
    void *m_data;
    int* set_colorIndex;
    int set_colorIndex_length;
    int* set_coordIndex;
    int set_coordIndex_length;
    X3dNode** attrib;
    int attrib_length;
    X3dNode* color;
    X3dNode* coord;
    int* colorIndex;
    int colorIndex_length;
    short colorPerVertex;
    int* coordIndex;
    int coordIndex_length;
    X3dNode* fogCoord;
    void* extra_data;
    int glName_number;
};

    void X3dSetGlNameIndexedLineSet(struct X3dIndexedFaceSet* self, int number)
{
    self->glName_number = number;
}

void X3dIndexedLineSetInit(struct X3dIndexedLineSet* self) {
    self->m_protoRoot = NULL;
    self->attrib = NULL;
    self->attrib_length = 0;
    self->color = NULL;
    self->coord = NULL;
    self->colorIndex = NULL;
    self->colorIndex_length = 0;
    self->colorPerVertex = 0;
    self->coordIndex = NULL;
    self->coordIndex_length = 0;
    self->fogCoord = NULL;
    self->extra_data = NULL;
}

X3dCallback X3dIndexedLineSetRenderCallback = NULL;
X3dCallback X3dIndexedLineSetTreeRenderCallback = NULL;
X3dCallback X3dIndexedLineSetDoWithDataCallback = NULL;
X3dCallback X3dIndexedLineSetTreeDoWithDataCallback = NULL;

X3dProcessEventCallback X3dIndexedLineSetProcessEventCallback = NULL;

int X3dIndexedLineSetType = 116;

void X3dIndexedLineSetRender(X3dNode* self, void *dataptr) {
    if (X3dIndexedLineSetRenderCallback)
        X3dIndexedLineSetRenderCallback(self, dataptr);
}
void X3dIndexedLineSetTreeRender(X3dNode* self, void *dataptr) {
    int i;
    if (((struct X3dIndexedLineSet*)self)->m_protoRoot != NULL)
        X3dTreeRenderCallback(((struct X3dIndexedLineSet*)self)->m_protoRoot, dataptr);
    else {
        if (((struct X3dIndexedLineSet*)self)->attrib)
            for (i = 0; i < ((struct X3dIndexedLineSet*)self)->attrib_length; i++)
                if (((struct X3dIndexedLineSet*)self)->attrib[i])
                    X3dTreeRenderCallback(((struct X3dIndexedLineSet*)self)->attrib[i], dataptr);
        if (((struct X3dIndexedLineSet*)self)->color)
            X3dTreeRenderCallback(((struct X3dIndexedLineSet*)self)->color, dataptr);
        if (((struct X3dIndexedLineSet*)self)->coord)
            X3dTreeRenderCallback(((struct X3dIndexedLineSet*)self)->coord, dataptr);
        if (((struct X3dIndexedLineSet*)self)->fogCoord)
            X3dTreeRenderCallback(((struct X3dIndexedLineSet*)self)->fogCoord, dataptr);
        X3dIndexedLineSetRender(self, dataptr);
    }
}
void X3dIndexedLineSetDoWithData(X3dNode* self, void *dataptr) {
    if (X3dIndexedLineSetDoWithDataCallback)
        X3dIndexedLineSetDoWithDataCallback(self, dataptr);
}
void X3dIndexedLineSetTreeDoWithData(X3dNode* self, void *dataptr) {
    int i;
    if (((struct X3dIndexedLineSet*)self)->m_protoRoot != NULL)
        X3dTreeDoWithDataCallback(((struct X3dIndexedLineSet*)self)->m_protoRoot, dataptr);
    else {
        if (((struct X3dIndexedLineSet*)self)->attrib)
            for (i = 0; i < ((struct X3dIndexedLineSet*)self)->attrib_length; i++)
                if (((struct X3dIndexedLineSet*)self)->attrib[i])
                    X3dTreeDoWithDataCallback(((struct X3dIndexedLineSet*)self)->attrib[i], dataptr);
        if (((struct X3dIndexedLineSet*)self)->color)
            X3dTreeDoWithDataCallback(((struct X3dIndexedLineSet*)self)->color, dataptr);
        if (((struct X3dIndexedLineSet*)self)->coord)
            X3dTreeDoWithDataCallback(((struct X3dIndexedLineSet*)self)->coord, dataptr);
        if (((struct X3dIndexedLineSet*)self)->fogCoord)
            X3dTreeDoWithDataCallback(((struct X3dIndexedLineSet*)self)->fogCoord, dataptr);
        X3dIndexedLineSetDoWithData(self, dataptr);
    }
}


struct X3dMultiPart {
    X3dNode* m_parent;
    X3dNode* m_protoRoot;
    int m_type;
    void *m_data;
    X3dNode** addChildren;
    int addChildren_length;
    X3dNode** removeChildren;
    int removeChildren_length;
    X3dNode** children;
    int children_length;
    float* bboxCenter;
    int bboxCenter_length;
    float* bboxSize;
    int bboxSize_length;
    void* extra_data;
};

void X3dMultiPartInit(struct X3dMultiPart* self) {
    self->m_protoRoot = NULL;
    self->children = NULL;
    self->children_length = 0;
    self->bboxCenter = NULL;
    self->bboxCenter_length = 0;
    self->bboxSize = NULL;
    self->bboxSize_length = 0;
    self->extra_data = NULL;
}

X3dCallback X3dMultiPartRenderCallback = NULL;
X3dCallback X3dMultiPartTreeRenderCallback = NULL;
X3dCallback X3dMultiPartDoWithDataCallback = NULL;
X3dCallback X3dMultiPartTreeDoWithDataCallback = NULL;

X3dProcessEventCallback X3dMultiPartProcessEventCallback = NULL;

int X3dMultiPartType = 162;

void X3dMultiPartRender(X3dNode* self, void *dataptr) {
    if (X3dMultiPartRenderCallback)
        X3dMultiPartRenderCallback(self, dataptr);
}
void X3dMultiPartTreeRender(X3dNode* self, void *dataptr) {
    int i;
    if (((struct X3dMultiPart*)self)->m_protoRoot != NULL)
        X3dTreeRenderCallback(((struct X3dMultiPart*)self)->m_protoRoot, dataptr);
    else {
        if (((struct X3dMultiPart*)self)->children)
            for (i = 0; i < ((struct X3dMultiPart*)self)->children_length; i++)
                if (((struct X3dMultiPart*)self)->children[i])
                    X3dTreeRenderCallback(((struct X3dMultiPart*)self)->children[i], dataptr);
        X3dMultiPartRender(self, dataptr);
    }
}
void X3dMultiPartDoWithData(X3dNode* self, void *dataptr) {
    if (X3dMultiPartDoWithDataCallback)
        X3dMultiPartDoWithDataCallback(self, dataptr);
}
void X3dMultiPartTreeDoWithData(X3dNode* self, void *dataptr) {
    int i;
    if (((struct X3dMultiPart*)self)->m_protoRoot != NULL)
        X3dTreeDoWithDataCallback(((struct X3dMultiPart*)self)->m_protoRoot, dataptr);
    else {
        if (((struct X3dMultiPart*)self)->children)
            for (i = 0; i < ((struct X3dMultiPart*)self)->children_length; i++)
                if (((struct X3dMultiPart*)self)->children[i])
                    X3dTreeDoWithDataCallback(((struct X3dMultiPart*)self)->children[i], dataptr);
        X3dMultiPartDoWithData(self, dataptr);
    }
}


struct X3dViewport {
    X3dNode* m_parent;
    X3dNode* m_protoRoot;
    int m_type;
    void *m_data;
    X3dNode** addChildren;
    int addChildren_length;
    X3dNode** removeChildren;
    int removeChildren_length;
    X3dNode** children;
    int children_length;
    float* bboxCenter;
    int bboxCenter_length;
    float* bboxSize;
    int bboxSize_length;
    float* clipBoundary;
    int clipBoundary_length;
    void* extra_data;
};

void X3dViewportInit(struct X3dViewport* self) {
    self->m_protoRoot = NULL;
    self->children = NULL;
    self->children_length = 0;
    self->bboxCenter = NULL;
    self->bboxCenter_length = 0;
    self->bboxSize = NULL;
    self->bboxSize_length = 0;
    self->clipBoundary = NULL;
    self->clipBoundary_length = 0;
    self->extra_data = NULL;
}

X3dCallback X3dViewportRenderCallback = NULL;
X3dCallback X3dViewportTreeRenderCallback = NULL;
X3dCallback X3dViewportDoWithDataCallback = NULL;
X3dCallback X3dViewportTreeDoWithDataCallback = NULL;

X3dProcessEventCallback X3dViewportProcessEventCallback = NULL;

int X3dViewportType = 318;

void X3dViewportRender(X3dNode* self, void *dataptr) {
    if (X3dViewportRenderCallback)
        X3dViewportRenderCallback(self, dataptr);
}
void X3dViewportTreeRender(X3dNode* self, void *dataptr) {
    int i;
    if (((struct X3dViewport*)self)->m_protoRoot != NULL)
        X3dTreeRenderCallback(((struct X3dViewport*)self)->m_protoRoot, dataptr);
    else {
        if (((struct X3dViewport*)self)->children)
            for (i = 0; i < ((struct X3dViewport*)self)->children_length; i++)
                if (((struct X3dViewport*)self)->children[i])
                    X3dTreeRenderCallback(((struct X3dViewport*)self)->children[i], dataptr);
        X3dViewportRender(self, dataptr);
    }
}
void X3dViewportDoWithData(X3dNode* self, void *dataptr) {
    if (X3dViewportDoWithDataCallback)
        X3dViewportDoWithDataCallback(self, dataptr);
}
void X3dViewportTreeDoWithData(X3dNode* self, void *dataptr) {
    int i;
    if (((struct X3dViewport*)self)->m_protoRoot != NULL)
        X3dTreeDoWithDataCallback(((struct X3dViewport*)self)->m_protoRoot, dataptr);
    else {
        if (((struct X3dViewport*)self)->children)
            for (i = 0; i < ((struct X3dViewport*)self)->children_length; i++)
                if (((struct X3dViewport*)self)->children[i])
                    X3dTreeDoWithDataCallback(((struct X3dViewport*)self)->children[i], dataptr);
        X3dViewportDoWithData(self, dataptr);
    }
}


struct X3dAnchor {
    X3dNode* m_parent;
    X3dNode* m_protoRoot;
    int m_type;
    void *m_data;
    X3dNode** addChildren;
    int addChildren_length;
    X3dNode** removeChildren;
    int removeChildren_length;
    X3dNode** children;
    int children_length;
    float* bboxCenter;
    int bboxCenter_length;
    float* bboxSize;
    int bboxSize_length;
    const char* description;
    const char** parameter;
    int parameter_length;
    const char** url;
    int url_length;
    void* extra_data;
};

void X3dAnchorInit(struct X3dAnchor* self) {
    self->m_protoRoot = NULL;
    self->children = NULL;
    self->children_length = 0;
    self->bboxCenter = NULL;
    self->bboxCenter_length = 0;
    self->bboxSize = NULL;
    self->bboxSize_length = 0;
    self->description = NULL;
    self->parameter = NULL;
    self->parameter_length = 0;
    self->url = NULL;
    self->url_length = 0;
    self->extra_data = NULL;
}

X3dCallback X3dAnchorRenderCallback = NULL;
X3dCallback X3dAnchorTreeRenderCallback = NULL;
X3dCallback X3dAnchorDoWithDataCallback = NULL;
X3dCallback X3dAnchorTreeDoWithDataCallback = NULL;

X3dProcessEventCallback X3dAnchorProcessEventCallback = NULL;

int X3dAnchorType = 0;

void X3dAnchorRender(X3dNode* self, void *dataptr) {
    if (X3dAnchorRenderCallback)
        X3dAnchorRenderCallback(self, dataptr);
}
void X3dAnchorTreeRender(X3dNode* self, void *dataptr) {
    int i;
    if (((struct X3dAnchor*)self)->m_protoRoot != NULL)
        X3dTreeRenderCallback(((struct X3dAnchor*)self)->m_protoRoot, dataptr);
    else {
        if (((struct X3dAnchor*)self)->children)
            for (i = 0; i < ((struct X3dAnchor*)self)->children_length; i++)
                if (((struct X3dAnchor*)self)->children[i])
                    X3dTreeRenderCallback(((struct X3dAnchor*)self)->children[i], dataptr);
        X3dAnchorRender(self, dataptr);
    }
}
void X3dAnchorDoWithData(X3dNode* self, void *dataptr) {
    if (X3dAnchorDoWithDataCallback)
        X3dAnchorDoWithDataCallback(self, dataptr);
}
void X3dAnchorTreeDoWithData(X3dNode* self, void *dataptr) {
    int i;
    if (((struct X3dAnchor*)self)->m_protoRoot != NULL)
        X3dTreeDoWithDataCallback(((struct X3dAnchor*)self)->m_protoRoot, dataptr);
    else {
        if (((struct X3dAnchor*)self)->children)
            for (i = 0; i < ((struct X3dAnchor*)self)->children_length; i++)
                if (((struct X3dAnchor*)self)->children[i])
                    X3dTreeDoWithDataCallback(((struct X3dAnchor*)self)->children[i], dataptr);
        X3dAnchorDoWithData(self, dataptr);
    }
}


struct X3dColor {
    X3dNode* m_parent;
    X3dNode* m_protoRoot;
    int m_type;
    void *m_data;
    float* color;
    int color_length;
    void* extra_data;
};

void X3dColorInit(struct X3dColor* self) {
    self->m_protoRoot = NULL;
    self->color = NULL;
    self->color_length = 0;
    self->extra_data = NULL;
}

X3dCallback X3dColorRenderCallback = NULL;
X3dCallback X3dColorTreeRenderCallback = NULL;
X3dCallback X3dColorDoWithDataCallback = NULL;
X3dCallback X3dColorTreeDoWithDataCallback = NULL;

X3dProcessEventCallback X3dColorProcessEventCallback = NULL;

int X3dColorType = 35;

void X3dColorRender(X3dNode* self, void *dataptr) {
    if (X3dColorRenderCallback)
        X3dColorRenderCallback(self, dataptr);
}
void X3dColorTreeRender(X3dNode* self, void *dataptr) {
    int i;
    if (((struct X3dColor*)self)->m_protoRoot != NULL)
        X3dTreeRenderCallback(((struct X3dColor*)self)->m_protoRoot, dataptr);
    else {
        X3dColorRender(self, dataptr);
    }
}
void X3dColorDoWithData(X3dNode* self, void *dataptr) {
    if (X3dColorDoWithDataCallback)
        X3dColorDoWithDataCallback(self, dataptr);
}
void X3dColorTreeDoWithData(X3dNode* self, void *dataptr) {
    int i;
    if (((struct X3dColor*)self)->m_protoRoot != NULL)
        X3dTreeDoWithDataCallback(((struct X3dColor*)self)->m_protoRoot, dataptr);
    else {
        X3dColorDoWithData(self, dataptr);
    }
}


struct X3dShape {
    X3dNode* m_parent;
    X3dNode* m_protoRoot;
    int m_type;
    void *m_data;
    X3dNode* appearance;
    X3dNode* geometry;
    float* bboxCenter;
    int bboxCenter_length;
    float* bboxSize;
    int bboxSize_length;
    X3dNode* octreeTriangles;
    const char* shading;
    void* extra_data;
};

void X3dShapeInit(struct X3dShape* self) {
    self->m_protoRoot = NULL;
    self->appearance = NULL;
    self->geometry = NULL;
    self->bboxCenter = NULL;
    self->bboxCenter_length = 0;
    self->bboxSize = NULL;
    self->bboxSize_length = 0;
    self->octreeTriangles = NULL;
    self->shading = NULL;
    self->extra_data = NULL;
}

X3dCallback X3dShapeRenderCallback = NULL;
X3dCallback X3dShapeTreeRenderCallback = NULL;
X3dCallback X3dShapeDoWithDataCallback = NULL;
X3dCallback X3dShapeTreeDoWithDataCallback = NULL;

X3dProcessEventCallback X3dShapeProcessEventCallback = NULL;

int X3dShapeType = 240;

void X3dShapeRender(X3dNode* self, void *dataptr) {
    if (X3dShapeRenderCallback)
        X3dShapeRenderCallback(self, dataptr);
}
void X3dShapeTreeRender(X3dNode* self, void *dataptr) {
    int i;
    if (((struct X3dShape*)self)->m_protoRoot != NULL)
        X3dTreeRenderCallback(((struct X3dShape*)self)->m_protoRoot, dataptr);
    else {
        if (((struct X3dShape*)self)->appearance)
            X3dTreeRenderCallback(((struct X3dShape*)self)->appearance, dataptr);
        if (((struct X3dShape*)self)->geometry)
            X3dTreeRenderCallback(((struct X3dShape*)self)->geometry, dataptr);
        if (((struct X3dShape*)self)->octreeTriangles)
            X3dTreeRenderCallback(((struct X3dShape*)self)->octreeTriangles, dataptr);
        X3dShapeRender(self, dataptr);
    }
}
void X3dShapeDoWithData(X3dNode* self, void *dataptr) {
    if (X3dShapeDoWithDataCallback)
        X3dShapeDoWithDataCallback(self, dataptr);
}
void X3dShapeTreeDoWithData(X3dNode* self, void *dataptr) {
    int i;
    if (((struct X3dShape*)self)->m_protoRoot != NULL)
        X3dTreeDoWithDataCallback(((struct X3dShape*)self)->m_protoRoot, dataptr);
    else {
        if (((struct X3dShape*)self)->appearance)
            X3dTreeDoWithDataCallback(((struct X3dShape*)self)->appearance, dataptr);
        if (((struct X3dShape*)self)->geometry)
            X3dTreeDoWithDataCallback(((struct X3dShape*)self)->geometry, dataptr);
        if (((struct X3dShape*)self)->octreeTriangles)
            X3dTreeDoWithDataCallback(((struct X3dShape*)self)->octreeTriangles, dataptr);
        X3dShapeDoWithData(self, dataptr);
    }
}


struct X3dNormalInterpolator {
    X3dNode* m_parent;
    X3dNode* m_protoRoot;
    int m_type;
    void *m_data;
    float set_fraction;
    float* value_changed;
    int value_changed_length;
    float* key;
    int key_length;
    float* keyValue;
    int keyValue_length;
    void* extra_data;
};

void X3dNormalInterpolatorInit(struct X3dNormalInterpolator* self) {
    self->m_protoRoot = NULL;
    self->key = NULL;
    self->key_length = 0;
    self->keyValue = NULL;
    self->keyValue_length = 0;
    self->extra_data = NULL;
}

X3dCallback X3dNormalInterpolatorRenderCallback = NULL;
X3dCallback X3dNormalInterpolatorTreeRenderCallback = NULL;
X3dCallback X3dNormalInterpolatorDoWithDataCallback = NULL;
X3dCallback X3dNormalInterpolatorTreeDoWithDataCallback = NULL;

X3dProcessEventCallback X3dNormalInterpolatorProcessEventCallback = NULL;

int X3dNormalInterpolatorType = 168;

void X3dNormalInterpolatorRender(X3dNode* self, void *dataptr) {
    if (X3dNormalInterpolatorRenderCallback)
        X3dNormalInterpolatorRenderCallback(self, dataptr);
}
void X3dNormalInterpolatorTreeRender(X3dNode* self, void *dataptr) {
    int i;
    if (((struct X3dNormalInterpolator*)self)->m_protoRoot != NULL)
        X3dTreeRenderCallback(((struct X3dNormalInterpolator*)self)->m_protoRoot, dataptr);
    else {
        X3dNormalInterpolatorRender(self, dataptr);
    }
}
void X3dNormalInterpolatorDoWithData(X3dNode* self, void *dataptr) {
    if (X3dNormalInterpolatorDoWithDataCallback)
        X3dNormalInterpolatorDoWithDataCallback(self, dataptr);
}
void X3dNormalInterpolatorTreeDoWithData(X3dNode* self, void *dataptr) {
    int i;
    if (((struct X3dNormalInterpolator*)self)->m_protoRoot != NULL)
        X3dTreeDoWithDataCallback(((struct X3dNormalInterpolator*)self)->m_protoRoot, dataptr);
    else {
        X3dNormalInterpolatorDoWithData(self, dataptr);
    }
}


struct X3dScalarInterpolator {
    X3dNode* m_parent;
    X3dNode* m_protoRoot;
    int m_type;
    void *m_data;
    float set_fraction;
    float value_changed;
    float* key;
    int key_length;
    float* keyValue;
    int keyValue_length;
    void* extra_data;
};

void X3dScalarInterpolatorInit(struct X3dScalarInterpolator* self) {
    self->m_protoRoot = NULL;
    self->key = NULL;
    self->key_length = 0;
    self->keyValue = NULL;
    self->keyValue_length = 0;
    self->extra_data = NULL;
}

X3dCallback X3dScalarInterpolatorRenderCallback = NULL;
X3dCallback X3dScalarInterpolatorTreeRenderCallback = NULL;
X3dCallback X3dScalarInterpolatorDoWithDataCallback = NULL;
X3dCallback X3dScalarInterpolatorTreeDoWithDataCallback = NULL;

X3dProcessEventCallback X3dScalarInterpolatorProcessEventCallback = NULL;

int X3dScalarInterpolatorType = 230;

void X3dScalarInterpolatorRender(X3dNode* self, void *dataptr) {
    if (X3dScalarInterpolatorRenderCallback)
        X3dScalarInterpolatorRenderCallback(self, dataptr);
}
void X3dScalarInterpolatorTreeRender(X3dNode* self, void *dataptr) {
    int i;
    if (((struct X3dScalarInterpolator*)self)->m_protoRoot != NULL)
        X3dTreeRenderCallback(((struct X3dScalarInterpolator*)self)->m_protoRoot, dataptr);
    else {
        X3dScalarInterpolatorRender(self, dataptr);
    }
}
void X3dScalarInterpolatorDoWithData(X3dNode* self, void *dataptr) {
    if (X3dScalarInterpolatorDoWithDataCallback)
        X3dScalarInterpolatorDoWithDataCallback(self, dataptr);
}
void X3dScalarInterpolatorTreeDoWithData(X3dNode* self, void *dataptr) {
    int i;
    if (((struct X3dScalarInterpolator*)self)->m_protoRoot != NULL)
        X3dTreeDoWithDataCallback(((struct X3dScalarInterpolator*)self)->m_protoRoot, dataptr);
    else {
        X3dScalarInterpolatorDoWithData(self, dataptr);
    }
}


struct X3dHAnimSegment {
    X3dNode* m_parent;
    X3dNode* m_protoRoot;
    int m_type;
    void *m_data;
    X3dNode** addChildren;
    int addChildren_length;
    X3dNode** removeChildren;
    int removeChildren_length;
    X3dNode** children;
    int children_length;
    float* bboxCenter;
    int bboxCenter_length;
    float* bboxSize;
    int bboxSize_length;
    float* centerOfMass;
    int centerOfMass_length;
    X3dNode* coord;
    X3dNode** displacers;
    int displacers_length;
    float mass;
    float* momentsOfInertia;
    int momentsOfInertia_length;
    const char* name;
    void* extra_data;
};

void X3dHAnimSegmentInit(struct X3dHAnimSegment* self) {
    self->m_protoRoot = NULL;
    self->children = NULL;
    self->children_length = 0;
    self->bboxCenter = NULL;
    self->bboxCenter_length = 0;
    self->bboxSize = NULL;
    self->bboxSize_length = 0;
    self->centerOfMass = NULL;
    self->centerOfMass_length = 0;
    self->coord = NULL;
    self->displacers = NULL;
    self->displacers_length = 0;
    self->mass = 0.0f;
    self->momentsOfInertia = NULL;
    self->momentsOfInertia_length = 0;
    self->name = NULL;
    self->extra_data = NULL;
}

X3dCallback X3dHAnimSegmentRenderCallback = NULL;
X3dCallback X3dHAnimSegmentTreeRenderCallback = NULL;
X3dCallback X3dHAnimSegmentDoWithDataCallback = NULL;
X3dCallback X3dHAnimSegmentTreeDoWithDataCallback = NULL;

X3dProcessEventCallback X3dHAnimSegmentProcessEventCallback = NULL;

int X3dHAnimSegmentType = 108;

void X3dHAnimSegmentRender(X3dNode* self, void *dataptr) {
    if (X3dHAnimSegmentRenderCallback)
        X3dHAnimSegmentRenderCallback(self, dataptr);
}
void X3dHAnimSegmentTreeRender(X3dNode* self, void *dataptr) {
    int i;
    if (((struct X3dHAnimSegment*)self)->m_protoRoot != NULL)
        X3dTreeRenderCallback(((struct X3dHAnimSegment*)self)->m_protoRoot, dataptr);
    else {
        if (((struct X3dHAnimSegment*)self)->children)
            for (i = 0; i < ((struct X3dHAnimSegment*)self)->children_length; i++)
                if (((struct X3dHAnimSegment*)self)->children[i])
                    X3dTreeRenderCallback(((struct X3dHAnimSegment*)self)->children[i], dataptr);
        if (((struct X3dHAnimSegment*)self)->coord)
            X3dTreeRenderCallback(((struct X3dHAnimSegment*)self)->coord, dataptr);
        if (((struct X3dHAnimSegment*)self)->displacers)
            for (i = 0; i < ((struct X3dHAnimSegment*)self)->displacers_length; i++)
                if (((struct X3dHAnimSegment*)self)->displacers[i])
                    X3dTreeRenderCallback(((struct X3dHAnimSegment*)self)->displacers[i], dataptr);
        X3dHAnimSegmentRender(self, dataptr);
    }
}
void X3dHAnimSegmentDoWithData(X3dNode* self, void *dataptr) {
    if (X3dHAnimSegmentDoWithDataCallback)
        X3dHAnimSegmentDoWithDataCallback(self, dataptr);
}
void X3dHAnimSegmentTreeDoWithData(X3dNode* self, void *dataptr) {
    int i;
    if (((struct X3dHAnimSegment*)self)->m_protoRoot != NULL)
        X3dTreeDoWithDataCallback(((struct X3dHAnimSegment*)self)->m_protoRoot, dataptr);
    else {
        if (((struct X3dHAnimSegment*)self)->children)
            for (i = 0; i < ((struct X3dHAnimSegment*)self)->children_length; i++)
                if (((struct X3dHAnimSegment*)self)->children[i])
                    X3dTreeDoWithDataCallback(((struct X3dHAnimSegment*)self)->children[i], dataptr);
        if (((struct X3dHAnimSegment*)self)->coord)
            X3dTreeDoWithDataCallback(((struct X3dHAnimSegment*)self)->coord, dataptr);
        if (((struct X3dHAnimSegment*)self)->displacers)
            for (i = 0; i < ((struct X3dHAnimSegment*)self)->displacers_length; i++)
                if (((struct X3dHAnimSegment*)self)->displacers[i])
                    X3dTreeDoWithDataCallback(((struct X3dHAnimSegment*)self)->displacers[i], dataptr);
        X3dHAnimSegmentDoWithData(self, dataptr);
    }
}


struct X3dTextureCoordinate {
    X3dNode* m_parent;
    X3dNode* m_protoRoot;
    int m_type;
    void *m_data;
    float* point;
    int point_length;
    void* extra_data;
};

void X3dTextureCoordinateInit(struct X3dTextureCoordinate* self) {
    self->m_protoRoot = NULL;
    self->point = NULL;
    self->point_length = 0;
    self->extra_data = NULL;
}

X3dCallback X3dTextureCoordinateRenderCallback = NULL;
X3dCallback X3dTextureCoordinateTreeRenderCallback = NULL;
X3dCallback X3dTextureCoordinateDoWithDataCallback = NULL;
X3dCallback X3dTextureCoordinateTreeDoWithDataCallback = NULL;

X3dProcessEventCallback X3dTextureCoordinateProcessEventCallback = NULL;

int X3dTextureCoordinateType = 276;

void X3dTextureCoordinateRender(X3dNode* self, void *dataptr) {
    if (X3dTextureCoordinateRenderCallback)
        X3dTextureCoordinateRenderCallback(self, dataptr);
}
void X3dTextureCoordinateTreeRender(X3dNode* self, void *dataptr) {
    int i;
    if (((struct X3dTextureCoordinate*)self)->m_protoRoot != NULL)
        X3dTreeRenderCallback(((struct X3dTextureCoordinate*)self)->m_protoRoot, dataptr);
    else {
        X3dTextureCoordinateRender(self, dataptr);
    }
}
void X3dTextureCoordinateDoWithData(X3dNode* self, void *dataptr) {
    if (X3dTextureCoordinateDoWithDataCallback)
        X3dTextureCoordinateDoWithDataCallback(self, dataptr);
}
void X3dTextureCoordinateTreeDoWithData(X3dNode* self, void *dataptr) {
    int i;
    if (((struct X3dTextureCoordinate*)self)->m_protoRoot != NULL)
        X3dTreeDoWithDataCallback(((struct X3dTextureCoordinate*)self)->m_protoRoot, dataptr);
    else {
        X3dTextureCoordinateDoWithData(self, dataptr);
    }
}


struct X3dNormal {
    X3dNode* m_parent;
    X3dNode* m_protoRoot;
    int m_type;
    void *m_data;
    float* vector;
    int vector_length;
    void* extra_data;
};

void X3dNormalInit(struct X3dNormal* self) {
    self->m_protoRoot = NULL;
    self->vector = NULL;
    self->vector_length = 0;
    self->extra_data = NULL;
}

X3dCallback X3dNormalRenderCallback = NULL;
X3dCallback X3dNormalTreeRenderCallback = NULL;
X3dCallback X3dNormalDoWithDataCallback = NULL;
X3dCallback X3dNormalTreeDoWithDataCallback = NULL;

X3dProcessEventCallback X3dNormalProcessEventCallback = NULL;

int X3dNormalType = 167;

void X3dNormalRender(X3dNode* self, void *dataptr) {
    if (X3dNormalRenderCallback)
        X3dNormalRenderCallback(self, dataptr);
}
void X3dNormalTreeRender(X3dNode* self, void *dataptr) {
    int i;
    if (((struct X3dNormal*)self)->m_protoRoot != NULL)
        X3dTreeRenderCallback(((struct X3dNormal*)self)->m_protoRoot, dataptr);
    else {
        X3dNormalRender(self, dataptr);
    }
}
void X3dNormalDoWithData(X3dNode* self, void *dataptr) {
    if (X3dNormalDoWithDataCallback)
        X3dNormalDoWithDataCallback(self, dataptr);
}
void X3dNormalTreeDoWithData(X3dNode* self, void *dataptr) {
    int i;
    if (((struct X3dNormal*)self)->m_protoRoot != NULL)
        X3dTreeDoWithDataCallback(((struct X3dNormal*)self)->m_protoRoot, dataptr);
    else {
        X3dNormalDoWithData(self, dataptr);
    }
}


struct X3dSpotLight {
    X3dNode* m_parent;
    X3dNode* m_protoRoot;
    int m_type;
    void *m_data;
    float ambientIntensity;
    float* attenuation;
    int attenuation_length;
    float beamWidth;
    float* color;
    int color_length;
    float cutOffAngle;
    float* direction;
    int direction_length;
    short global;
    float intensity;
    float* location;
    int location_length;
    short on;
    float radius;
    float projectionAngle;
    short shadows;
    short kambiShadows;
    short kambiShadowsMain;
    float projectionNear;
    float projectionFar;
    float* up;
    int up_length;
    X3dNode* defaultShadowMap;
    X3dNode** effects;
    int effects_length;
    short shadowVolumes;
    short shadowVolumesMain;
    void* extra_data;
};

void X3dSpotLightInit(struct X3dSpotLight* self) {
    self->m_protoRoot = NULL;
    self->ambientIntensity = 0.0f;
    self->attenuation = NULL;
    self->attenuation_length = 0;
    self->beamWidth = 0.0f;
    self->color = NULL;
    self->color_length = 0;
    self->cutOffAngle = 0.0f;
    self->direction = NULL;
    self->direction_length = 0;
    self->global = 0;
    self->intensity = 0.0f;
    self->location = NULL;
    self->location_length = 0;
    self->on = 0;
    self->radius = 0.0f;
    self->projectionAngle = 0.0f;
    self->shadows = 0;
    self->kambiShadows = 0;
    self->kambiShadowsMain = 0;
    self->projectionNear = 0.0f;
    self->projectionFar = 0.0f;
    self->up = NULL;
    self->up_length = 0;
    self->defaultShadowMap = NULL;
    self->effects = NULL;
    self->effects_length = 0;
    self->shadowVolumes = 0;
    self->shadowVolumesMain = 0;
    self->extra_data = NULL;
}

X3dCallback X3dSpotLightRenderCallback = NULL;
X3dCallback X3dSpotLightTreeRenderCallback = NULL;
X3dCallback X3dSpotLightDoWithDataCallback = NULL;
X3dCallback X3dSpotLightTreeDoWithDataCallback = NULL;

X3dProcessEventCallback X3dSpotLightProcessEventCallback = NULL;

int X3dSpotLightType = 257;

void X3dSpotLightRender(X3dNode* self, void *dataptr) {
    if (X3dSpotLightRenderCallback)
        X3dSpotLightRenderCallback(self, dataptr);
}
void X3dSpotLightTreeRender(X3dNode* self, void *dataptr) {
    int i;
    if (((struct X3dSpotLight*)self)->m_protoRoot != NULL)
        X3dTreeRenderCallback(((struct X3dSpotLight*)self)->m_protoRoot, dataptr);
    else {
        if (((struct X3dSpotLight*)self)->defaultShadowMap)
            X3dTreeRenderCallback(((struct X3dSpotLight*)self)->defaultShadowMap, dataptr);
        if (((struct X3dSpotLight*)self)->effects)
            for (i = 0; i < ((struct X3dSpotLight*)self)->effects_length; i++)
                if (((struct X3dSpotLight*)self)->effects[i])
                    X3dTreeRenderCallback(((struct X3dSpotLight*)self)->effects[i], dataptr);
        X3dSpotLightRender(self, dataptr);
    }
}
void X3dSpotLightDoWithData(X3dNode* self, void *dataptr) {
    if (X3dSpotLightDoWithDataCallback)
        X3dSpotLightDoWithDataCallback(self, dataptr);
}
void X3dSpotLightTreeDoWithData(X3dNode* self, void *dataptr) {
    int i;
    if (((struct X3dSpotLight*)self)->m_protoRoot != NULL)
        X3dTreeDoWithDataCallback(((struct X3dSpotLight*)self)->m_protoRoot, dataptr);
    else {
        if (((struct X3dSpotLight*)self)->defaultShadowMap)
            X3dTreeDoWithDataCallback(((struct X3dSpotLight*)self)->defaultShadowMap, dataptr);
        if (((struct X3dSpotLight*)self)->effects)
            for (i = 0; i < ((struct X3dSpotLight*)self)->effects_length; i++)
                if (((struct X3dSpotLight*)self)->effects[i])
                    X3dTreeDoWithDataCallback(((struct X3dSpotLight*)self)->effects[i], dataptr);
        X3dSpotLightDoWithData(self, dataptr);
    }
}


struct X3dColorInterpolator {
    X3dNode* m_parent;
    X3dNode* m_protoRoot;
    int m_type;
    void *m_data;
    float set_fraction;
    float* value_changed;
    int value_changed_length;
    float* key;
    int key_length;
    float* keyValue;
    int keyValue_length;
    void* extra_data;
};

void X3dColorInterpolatorInit(struct X3dColorInterpolator* self) {
    self->m_protoRoot = NULL;
    self->key = NULL;
    self->key_length = 0;
    self->keyValue = NULL;
    self->keyValue_length = 0;
    self->extra_data = NULL;
}

X3dCallback X3dColorInterpolatorRenderCallback = NULL;
X3dCallback X3dColorInterpolatorTreeRenderCallback = NULL;
X3dCallback X3dColorInterpolatorDoWithDataCallback = NULL;
X3dCallback X3dColorInterpolatorTreeDoWithDataCallback = NULL;

X3dProcessEventCallback X3dColorInterpolatorProcessEventCallback = NULL;

int X3dColorInterpolatorType = 38;

void X3dColorInterpolatorRender(X3dNode* self, void *dataptr) {
    if (X3dColorInterpolatorRenderCallback)
        X3dColorInterpolatorRenderCallback(self, dataptr);
}
void X3dColorInterpolatorTreeRender(X3dNode* self, void *dataptr) {
    int i;
    if (((struct X3dColorInterpolator*)self)->m_protoRoot != NULL)
        X3dTreeRenderCallback(((struct X3dColorInterpolator*)self)->m_protoRoot, dataptr);
    else {
        X3dColorInterpolatorRender(self, dataptr);
    }
}
void X3dColorInterpolatorDoWithData(X3dNode* self, void *dataptr) {
    if (X3dColorInterpolatorDoWithDataCallback)
        X3dColorInterpolatorDoWithDataCallback(self, dataptr);
}
void X3dColorInterpolatorTreeDoWithData(X3dNode* self, void *dataptr) {
    int i;
    if (((struct X3dColorInterpolator*)self)->m_protoRoot != NULL)
        X3dTreeDoWithDataCallback(((struct X3dColorInterpolator*)self)->m_protoRoot, dataptr);
    else {
        X3dColorInterpolatorDoWithData(self, dataptr);
    }
}


struct X3dExplosionEmitter {
    X3dNode* m_parent;
    X3dNode* m_protoRoot;
    int m_type;
    void *m_data;
    float speed;
    float variation;
    float mass;
    float surfaceArea;
    float* position;
    int position_length;
    void* extra_data;
};

void X3dExplosionEmitterInit(struct X3dExplosionEmitter* self) {
    self->m_protoRoot = NULL;
    self->speed = 0.0f;
    self->variation = 0.0f;
    self->mass = 0.0f;
    self->surfaceArea = 0.0f;
    self->position = NULL;
    self->position_length = 0;
    self->extra_data = NULL;
}

X3dCallback X3dExplosionEmitterRenderCallback = NULL;
X3dCallback X3dExplosionEmitterTreeRenderCallback = NULL;
X3dCallback X3dExplosionEmitterDoWithDataCallback = NULL;
X3dCallback X3dExplosionEmitterTreeDoWithDataCallback = NULL;

X3dProcessEventCallback X3dExplosionEmitterProcessEventCallback = NULL;

int X3dExplosionEmitterType = 81;

void X3dExplosionEmitterRender(X3dNode* self, void *dataptr) {
    if (X3dExplosionEmitterRenderCallback)
        X3dExplosionEmitterRenderCallback(self, dataptr);
}
void X3dExplosionEmitterTreeRender(X3dNode* self, void *dataptr) {
    int i;
    if (((struct X3dExplosionEmitter*)self)->m_protoRoot != NULL)
        X3dTreeRenderCallback(((struct X3dExplosionEmitter*)self)->m_protoRoot, dataptr);
    else {
        X3dExplosionEmitterRender(self, dataptr);
    }
}
void X3dExplosionEmitterDoWithData(X3dNode* self, void *dataptr) {
    if (X3dExplosionEmitterDoWithDataCallback)
        X3dExplosionEmitterDoWithDataCallback(self, dataptr);
}
void X3dExplosionEmitterTreeDoWithData(X3dNode* self, void *dataptr) {
    int i;
    if (((struct X3dExplosionEmitter*)self)->m_protoRoot != NULL)
        X3dTreeDoWithDataCallback(((struct X3dExplosionEmitter*)self)->m_protoRoot, dataptr);
    else {
        X3dExplosionEmitterDoWithData(self, dataptr);
    }
}


struct X3dLineSet {
    X3dNode* m_parent;
    X3dNode* m_protoRoot;
    int m_type;
    void *m_data;
    X3dNode** attrib;
    int attrib_length;
    X3dNode* color;
    X3dNode* coord;
    X3dNode* fogCoord;
    int* vertexCount;
    int vertexCount_length;
    void* extra_data;
    int glName_number;
};

    void X3dSetGlNameLineSet(struct X3dIndexedFaceSet* self, int number)
{
    self->glName_number = number;
}

void X3dLineSetInit(struct X3dLineSet* self) {
    self->m_protoRoot = NULL;
    self->attrib = NULL;
    self->attrib_length = 0;
    self->color = NULL;
    self->coord = NULL;
    self->fogCoord = NULL;
    self->vertexCount = NULL;
    self->vertexCount_length = 0;
    self->extra_data = NULL;
}

X3dCallback X3dLineSetRenderCallback = NULL;
X3dCallback X3dLineSetTreeRenderCallback = NULL;
X3dCallback X3dLineSetDoWithDataCallback = NULL;
X3dCallback X3dLineSetTreeDoWithDataCallback = NULL;

X3dProcessEventCallback X3dLineSetProcessEventCallback = NULL;

int X3dLineSetType = 143;

void X3dLineSetRender(X3dNode* self, void *dataptr) {
    if (X3dLineSetRenderCallback)
        X3dLineSetRenderCallback(self, dataptr);
}
void X3dLineSetTreeRender(X3dNode* self, void *dataptr) {
    int i;
    if (((struct X3dLineSet*)self)->m_protoRoot != NULL)
        X3dTreeRenderCallback(((struct X3dLineSet*)self)->m_protoRoot, dataptr);
    else {
        if (((struct X3dLineSet*)self)->attrib)
            for (i = 0; i < ((struct X3dLineSet*)self)->attrib_length; i++)
                if (((struct X3dLineSet*)self)->attrib[i])
                    X3dTreeRenderCallback(((struct X3dLineSet*)self)->attrib[i], dataptr);
        if (((struct X3dLineSet*)self)->color)
            X3dTreeRenderCallback(((struct X3dLineSet*)self)->color, dataptr);
        if (((struct X3dLineSet*)self)->coord)
            X3dTreeRenderCallback(((struct X3dLineSet*)self)->coord, dataptr);
        if (((struct X3dLineSet*)self)->fogCoord)
            X3dTreeRenderCallback(((struct X3dLineSet*)self)->fogCoord, dataptr);
        X3dLineSetRender(self, dataptr);
    }
}
void X3dLineSetDoWithData(X3dNode* self, void *dataptr) {
    if (X3dLineSetDoWithDataCallback)
        X3dLineSetDoWithDataCallback(self, dataptr);
}
void X3dLineSetTreeDoWithData(X3dNode* self, void *dataptr) {
    int i;
    if (((struct X3dLineSet*)self)->m_protoRoot != NULL)
        X3dTreeDoWithDataCallback(((struct X3dLineSet*)self)->m_protoRoot, dataptr);
    else {
        if (((struct X3dLineSet*)self)->attrib)
            for (i = 0; i < ((struct X3dLineSet*)self)->attrib_length; i++)
                if (((struct X3dLineSet*)self)->attrib[i])
                    X3dTreeDoWithDataCallback(((struct X3dLineSet*)self)->attrib[i], dataptr);
        if (((struct X3dLineSet*)self)->color)
            X3dTreeDoWithDataCallback(((struct X3dLineSet*)self)->color, dataptr);
        if (((struct X3dLineSet*)self)->coord)
            X3dTreeDoWithDataCallback(((struct X3dLineSet*)self)->coord, dataptr);
        if (((struct X3dLineSet*)self)->fogCoord)
            X3dTreeDoWithDataCallback(((struct X3dLineSet*)self)->fogCoord, dataptr);
        X3dLineSetDoWithData(self, dataptr);
    }
}


struct X3dPixelTexture {
    X3dNode* m_parent;
    X3dNode* m_protoRoot;
    int m_type;
    void *m_data;
    int* image;
    int image_length;
    short repeatS;
    short repeatT;
    X3dNode* textureProperties;
    X3dNode** effects;
    int effects_length;
    const char* alphaChannel;
    void* extra_data;
};

void X3dPixelTextureInit(struct X3dPixelTexture* self) {
    self->m_protoRoot = NULL;
    self->image = NULL;
    self->image_length = 0;
    self->repeatS = 0;
    self->repeatT = 0;
    self->textureProperties = NULL;
    self->effects = NULL;
    self->effects_length = 0;
    self->alphaChannel = NULL;
    self->extra_data = NULL;
}

X3dCallback X3dPixelTextureRenderCallback = NULL;
X3dCallback X3dPixelTextureTreeRenderCallback = NULL;
X3dCallback X3dPixelTextureDoWithDataCallback = NULL;
X3dCallback X3dPixelTextureTreeDoWithDataCallback = NULL;

X3dProcessEventCallback X3dPixelTextureProcessEventCallback = NULL;

int X3dPixelTextureType = 193;

void X3dPixelTextureRender(X3dNode* self, void *dataptr) {
    if (X3dPixelTextureRenderCallback)
        X3dPixelTextureRenderCallback(self, dataptr);
}
void X3dPixelTextureTreeRender(X3dNode* self, void *dataptr) {
    int i;
    if (((struct X3dPixelTexture*)self)->m_protoRoot != NULL)
        X3dTreeRenderCallback(((struct X3dPixelTexture*)self)->m_protoRoot, dataptr);
    else {
        if (((struct X3dPixelTexture*)self)->textureProperties)
            X3dTreeRenderCallback(((struct X3dPixelTexture*)self)->textureProperties, dataptr);
        if (((struct X3dPixelTexture*)self)->effects)
            for (i = 0; i < ((struct X3dPixelTexture*)self)->effects_length; i++)
                if (((struct X3dPixelTexture*)self)->effects[i])
                    X3dTreeRenderCallback(((struct X3dPixelTexture*)self)->effects[i], dataptr);
        X3dPixelTextureRender(self, dataptr);
    }
}
void X3dPixelTextureDoWithData(X3dNode* self, void *dataptr) {
    if (X3dPixelTextureDoWithDataCallback)
        X3dPixelTextureDoWithDataCallback(self, dataptr);
}
void X3dPixelTextureTreeDoWithData(X3dNode* self, void *dataptr) {
    int i;
    if (((struct X3dPixelTexture*)self)->m_protoRoot != NULL)
        X3dTreeDoWithDataCallback(((struct X3dPixelTexture*)self)->m_protoRoot, dataptr);
    else {
        if (((struct X3dPixelTexture*)self)->textureProperties)
            X3dTreeDoWithDataCallback(((struct X3dPixelTexture*)self)->textureProperties, dataptr);
        if (((struct X3dPixelTexture*)self)->effects)
            for (i = 0; i < ((struct X3dPixelTexture*)self)->effects_length; i++)
                if (((struct X3dPixelTexture*)self)->effects[i])
                    X3dTreeDoWithDataCallback(((struct X3dPixelTexture*)self)->effects[i], dataptr);
        X3dPixelTextureDoWithData(self, dataptr);
    }
}


struct X3dSwitch {
    X3dNode* m_parent;
    X3dNode* m_protoRoot;
    int m_type;
    void *m_data;
    X3dNode** addChildren;
    int addChildren_length;
    X3dNode** removeChildren;
    int removeChildren_length;
    float* bboxCenter;
    int bboxCenter_length;
    float* bboxSize;
    int bboxSize_length;
    X3dNode** children;
    int children_length;
    int whichChoice;
    void* extra_data;
};

void X3dSwitchInit(struct X3dSwitch* self) {
    self->m_protoRoot = NULL;
    self->bboxCenter = NULL;
    self->bboxCenter_length = 0;
    self->bboxSize = NULL;
    self->bboxSize_length = 0;
    self->children = NULL;
    self->children_length = 0;
    self->whichChoice = 0;
    self->extra_data = NULL;
}

X3dCallback X3dSwitchRenderCallback = NULL;
X3dCallback X3dSwitchTreeRenderCallback = NULL;
X3dCallback X3dSwitchDoWithDataCallback = NULL;
X3dCallback X3dSwitchTreeDoWithDataCallback = NULL;

X3dProcessEventCallback X3dSwitchProcessEventCallback = NULL;

int X3dSwitchType = 269;

void X3dSwitchRender(X3dNode* self, void *dataptr) {
    if (X3dSwitchRenderCallback)
        X3dSwitchRenderCallback(self, dataptr);
}
void X3dSwitchTreeRender(X3dNode* self, void *dataptr) {
    int i;
    if (((struct X3dSwitch*)self)->m_protoRoot != NULL)
        X3dTreeRenderCallback(((struct X3dSwitch*)self)->m_protoRoot, dataptr);
    else {
        if (((struct X3dSwitch*)self)->children)
            for (i = 0; i < ((struct X3dSwitch*)self)->children_length; i++)
                if (((struct X3dSwitch*)self)->children[i])
                    X3dTreeRenderCallback(((struct X3dSwitch*)self)->children[i], dataptr);
        X3dSwitchRender(self, dataptr);
    }
}
void X3dSwitchDoWithData(X3dNode* self, void *dataptr) {
    if (X3dSwitchDoWithDataCallback)
        X3dSwitchDoWithDataCallback(self, dataptr);
}
void X3dSwitchTreeDoWithData(X3dNode* self, void *dataptr) {
    int i;
    if (((struct X3dSwitch*)self)->m_protoRoot != NULL)
        X3dTreeDoWithDataCallback(((struct X3dSwitch*)self)->m_protoRoot, dataptr);
    else {
        if (((struct X3dSwitch*)self)->children)
            for (i = 0; i < ((struct X3dSwitch*)self)->children_length; i++)
                if (((struct X3dSwitch*)self)->children[i])
                    X3dTreeDoWithDataCallback(((struct X3dSwitch*)self)->children[i], dataptr);
        X3dSwitchDoWithData(self, dataptr);
    }
}


struct X3dBlock {
    X3dNode* m_parent;
    X3dNode* m_protoRoot;
    int m_type;
    void *m_data;
    X3dNode** addChildren;
    int addChildren_length;
    X3dNode** removeChildren;
    int removeChildren_length;
    X3dNode** children;
    int children_length;
    float* bboxCenter;
    int bboxCenter_length;
    float* bboxSize;
    int bboxSize_length;
    void* extra_data;
};

void X3dBlockInit(struct X3dBlock* self) {
    self->m_protoRoot = NULL;
    self->children = NULL;
    self->children_length = 0;
    self->bboxCenter = NULL;
    self->bboxCenter_length = 0;
    self->bboxSize = NULL;
    self->bboxSize_length = 0;
    self->extra_data = NULL;
}

X3dCallback X3dBlockRenderCallback = NULL;
X3dCallback X3dBlockTreeRenderCallback = NULL;
X3dCallback X3dBlockDoWithDataCallback = NULL;
X3dCallback X3dBlockTreeDoWithDataCallback = NULL;

X3dProcessEventCallback X3dBlockProcessEventCallback = NULL;

int X3dBlockType = 10;

void X3dBlockRender(X3dNode* self, void *dataptr) {
    if (X3dBlockRenderCallback)
        X3dBlockRenderCallback(self, dataptr);
}
void X3dBlockTreeRender(X3dNode* self, void *dataptr) {
    int i;
    if (((struct X3dBlock*)self)->m_protoRoot != NULL)
        X3dTreeRenderCallback(((struct X3dBlock*)self)->m_protoRoot, dataptr);
    else {
        if (((struct X3dBlock*)self)->children)
            for (i = 0; i < ((struct X3dBlock*)self)->children_length; i++)
                if (((struct X3dBlock*)self)->children[i])
                    X3dTreeRenderCallback(((struct X3dBlock*)self)->children[i], dataptr);
        X3dBlockRender(self, dataptr);
    }
}
void X3dBlockDoWithData(X3dNode* self, void *dataptr) {
    if (X3dBlockDoWithDataCallback)
        X3dBlockDoWithDataCallback(self, dataptr);
}
void X3dBlockTreeDoWithData(X3dNode* self, void *dataptr) {
    int i;
    if (((struct X3dBlock*)self)->m_protoRoot != NULL)
        X3dTreeDoWithDataCallback(((struct X3dBlock*)self)->m_protoRoot, dataptr);
    else {
        if (((struct X3dBlock*)self)->children)
            for (i = 0; i < ((struct X3dBlock*)self)->children_length; i++)
                if (((struct X3dBlock*)self)->children[i])
                    X3dTreeDoWithDataCallback(((struct X3dBlock*)self)->children[i], dataptr);
        X3dBlockDoWithData(self, dataptr);
    }
}


struct X3dViewpoint {
    X3dNode* m_parent;
    X3dNode* m_protoRoot;
    int m_type;
    void *m_data;
    short set_bind;
    double bindTime;
    short isBound;
    float* centerOfRotation;
    int centerOfRotation_length;
    const char* description;
    float fieldOfView;
    short jump;
    float* orientation;
    int orientation_length;
    short retainUserOffsets;
    float* position;
    int position_length;
    void* extra_data;
};

void X3dViewpointInit(struct X3dViewpoint* self) {
    self->m_protoRoot = NULL;
    self->centerOfRotation = NULL;
    self->centerOfRotation_length = 0;
    self->description = NULL;
    self->fieldOfView = 0.0f;
    self->jump = 0;
    self->orientation = NULL;
    self->orientation_length = 0;
    self->retainUserOffsets = 0;
    self->position = NULL;
    self->position_length = 0;
    self->extra_data = NULL;
}

X3dCallback X3dViewpointRenderCallback = NULL;
X3dCallback X3dViewpointTreeRenderCallback = NULL;
X3dCallback X3dViewpointDoWithDataCallback = NULL;
X3dCallback X3dViewpointTreeDoWithDataCallback = NULL;

X3dProcessEventCallback X3dViewpointProcessEventCallback = NULL;

int X3dViewpointType = 316;

void X3dViewpointRender(X3dNode* self, void *dataptr) {
    if (X3dViewpointRenderCallback)
        X3dViewpointRenderCallback(self, dataptr);
}
void X3dViewpointTreeRender(X3dNode* self, void *dataptr) {
    int i;
    if (((struct X3dViewpoint*)self)->m_protoRoot != NULL)
        X3dTreeRenderCallback(((struct X3dViewpoint*)self)->m_protoRoot, dataptr);
    else {
        X3dViewpointRender(self, dataptr);
    }
}
void X3dViewpointDoWithData(X3dNode* self, void *dataptr) {
    if (X3dViewpointDoWithDataCallback)
        X3dViewpointDoWithDataCallback(self, dataptr);
}
void X3dViewpointTreeDoWithData(X3dNode* self, void *dataptr) {
    int i;
    if (((struct X3dViewpoint*)self)->m_protoRoot != NULL)
        X3dTreeDoWithDataCallback(((struct X3dViewpoint*)self)->m_protoRoot, dataptr);
    else {
        X3dViewpointDoWithData(self, dataptr);
    }
}


struct X3dPointSet {
    X3dNode* m_parent;
    X3dNode* m_protoRoot;
    int m_type;
    void *m_data;
    X3dNode** attrib;
    int attrib_length;
    X3dNode* color;
    X3dNode* coord;
    X3dNode* fogCoord;
    void* extra_data;
    int glName_number;
};

    void X3dSetGlNamePointSet(struct X3dIndexedFaceSet* self, int number)
{
    self->glName_number = number;
}

void X3dPointSetInit(struct X3dPointSet* self) {
    self->m_protoRoot = NULL;
    self->attrib = NULL;
    self->attrib_length = 0;
    self->color = NULL;
    self->coord = NULL;
    self->fogCoord = NULL;
    self->extra_data = NULL;
}

X3dCallback X3dPointSetRenderCallback = NULL;
X3dCallback X3dPointSetTreeRenderCallback = NULL;
X3dCallback X3dPointSetDoWithDataCallback = NULL;
X3dCallback X3dPointSetTreeDoWithDataCallback = NULL;

X3dProcessEventCallback X3dPointSetProcessEventCallback = NULL;

int X3dPointSetType = 200;

void X3dPointSetRender(X3dNode* self, void *dataptr) {
    if (X3dPointSetRenderCallback)
        X3dPointSetRenderCallback(self, dataptr);
}
void X3dPointSetTreeRender(X3dNode* self, void *dataptr) {
    int i;
    if (((struct X3dPointSet*)self)->m_protoRoot != NULL)
        X3dTreeRenderCallback(((struct X3dPointSet*)self)->m_protoRoot, dataptr);
    else {
        if (((struct X3dPointSet*)self)->attrib)
            for (i = 0; i < ((struct X3dPointSet*)self)->attrib_length; i++)
                if (((struct X3dPointSet*)self)->attrib[i])
                    X3dTreeRenderCallback(((struct X3dPointSet*)self)->attrib[i], dataptr);
        if (((struct X3dPointSet*)self)->color)
            X3dTreeRenderCallback(((struct X3dPointSet*)self)->color, dataptr);
        if (((struct X3dPointSet*)self)->coord)
            X3dTreeRenderCallback(((struct X3dPointSet*)self)->coord, dataptr);
        if (((struct X3dPointSet*)self)->fogCoord)
            X3dTreeRenderCallback(((struct X3dPointSet*)self)->fogCoord, dataptr);
        X3dPointSetRender(self, dataptr);
    }
}
void X3dPointSetDoWithData(X3dNode* self, void *dataptr) {
    if (X3dPointSetDoWithDataCallback)
        X3dPointSetDoWithDataCallback(self, dataptr);
}
void X3dPointSetTreeDoWithData(X3dNode* self, void *dataptr) {
    int i;
    if (((struct X3dPointSet*)self)->m_protoRoot != NULL)
        X3dTreeDoWithDataCallback(((struct X3dPointSet*)self)->m_protoRoot, dataptr);
    else {
        if (((struct X3dPointSet*)self)->attrib)
            for (i = 0; i < ((struct X3dPointSet*)self)->attrib_length; i++)
                if (((struct X3dPointSet*)self)->attrib[i])
                    X3dTreeDoWithDataCallback(((struct X3dPointSet*)self)->attrib[i], dataptr);
        if (((struct X3dPointSet*)self)->color)
            X3dTreeDoWithDataCallback(((struct X3dPointSet*)self)->color, dataptr);
        if (((struct X3dPointSet*)self)->coord)
            X3dTreeDoWithDataCallback(((struct X3dPointSet*)self)->coord, dataptr);
        if (((struct X3dPointSet*)self)->fogCoord)
            X3dTreeDoWithDataCallback(((struct X3dPointSet*)self)->fogCoord, dataptr);
        X3dPointSetDoWithData(self, dataptr);
    }
}


struct X3dBoundedPhysicsModel {
    X3dNode* m_parent;
    X3dNode* m_protoRoot;
    int m_type;
    void *m_data;
    short enabled;
    X3dNode* geometry;
    void* extra_data;
};

void X3dBoundedPhysicsModelInit(struct X3dBoundedPhysicsModel* self) {
    self->m_protoRoot = NULL;
    self->enabled = 0;
    self->geometry = NULL;
    self->extra_data = NULL;
}

X3dCallback X3dBoundedPhysicsModelRenderCallback = NULL;
X3dCallback X3dBoundedPhysicsModelTreeRenderCallback = NULL;
X3dCallback X3dBoundedPhysicsModelDoWithDataCallback = NULL;
X3dCallback X3dBoundedPhysicsModelTreeDoWithDataCallback = NULL;

X3dProcessEventCallback X3dBoundedPhysicsModelProcessEventCallback = NULL;

int X3dBoundedPhysicsModelType = 18;

void X3dBoundedPhysicsModelRender(X3dNode* self, void *dataptr) {
    if (X3dBoundedPhysicsModelRenderCallback)
        X3dBoundedPhysicsModelRenderCallback(self, dataptr);
}
void X3dBoundedPhysicsModelTreeRender(X3dNode* self, void *dataptr) {
    int i;
    if (((struct X3dBoundedPhysicsModel*)self)->m_protoRoot != NULL)
        X3dTreeRenderCallback(((struct X3dBoundedPhysicsModel*)self)->m_protoRoot, dataptr);
    else {
        if (((struct X3dBoundedPhysicsModel*)self)->geometry)
            X3dTreeRenderCallback(((struct X3dBoundedPhysicsModel*)self)->geometry, dataptr);
        X3dBoundedPhysicsModelRender(self, dataptr);
    }
}
void X3dBoundedPhysicsModelDoWithData(X3dNode* self, void *dataptr) {
    if (X3dBoundedPhysicsModelDoWithDataCallback)
        X3dBoundedPhysicsModelDoWithDataCallback(self, dataptr);
}
void X3dBoundedPhysicsModelTreeDoWithData(X3dNode* self, void *dataptr) {
    int i;
    if (((struct X3dBoundedPhysicsModel*)self)->m_protoRoot != NULL)
        X3dTreeDoWithDataCallback(((struct X3dBoundedPhysicsModel*)self)->m_protoRoot, dataptr);
    else {
        if (((struct X3dBoundedPhysicsModel*)self)->geometry)
            X3dTreeDoWithDataCallback(((struct X3dBoundedPhysicsModel*)self)->geometry, dataptr);
        X3dBoundedPhysicsModelDoWithData(self, dataptr);
    }
}


struct X3dGroup {
    X3dNode* m_parent;
    X3dNode* m_protoRoot;
    int m_type;
    void *m_data;
    X3dNode** addChildren;
    int addChildren_length;
    X3dNode** removeChildren;
    int removeChildren_length;
    X3dNode** children;
    int children_length;
    float* bboxCenter;
    int bboxCenter_length;
    float* bboxSize;
    int bboxSize_length;
    void* extra_data;
};

void X3dGroupInit(struct X3dGroup* self) {
    self->m_protoRoot = NULL;
    self->children = NULL;
    self->children_length = 0;
    self->bboxCenter = NULL;
    self->bboxCenter_length = 0;
    self->bboxSize = NULL;
    self->bboxSize_length = 0;
    self->extra_data = NULL;
}

X3dCallback X3dGroupRenderCallback = NULL;
X3dCallback X3dGroupTreeRenderCallback = NULL;
X3dCallback X3dGroupDoWithDataCallback = NULL;
X3dCallback X3dGroupTreeDoWithDataCallback = NULL;

X3dProcessEventCallback X3dGroupProcessEventCallback = NULL;

int X3dGroupType = 104;

void X3dGroupRender(X3dNode* self, void *dataptr) {
    if (X3dGroupRenderCallback)
        X3dGroupRenderCallback(self, dataptr);
}
void X3dGroupTreeRender(X3dNode* self, void *dataptr) {
    int i;
    if (((struct X3dGroup*)self)->m_protoRoot != NULL)
        X3dTreeRenderCallback(((struct X3dGroup*)self)->m_protoRoot, dataptr);
    else {
        if (((struct X3dGroup*)self)->children)
            for (i = 0; i < ((struct X3dGroup*)self)->children_length; i++)
                if (((struct X3dGroup*)self)->children[i])
                    X3dTreeRenderCallback(((struct X3dGroup*)self)->children[i], dataptr);
        X3dGroupRender(self, dataptr);
    }
}
void X3dGroupDoWithData(X3dNode* self, void *dataptr) {
    if (X3dGroupDoWithDataCallback)
        X3dGroupDoWithDataCallback(self, dataptr);
}
void X3dGroupTreeDoWithData(X3dNode* self, void *dataptr) {
    int i;
    if (((struct X3dGroup*)self)->m_protoRoot != NULL)
        X3dTreeDoWithDataCallback(((struct X3dGroup*)self)->m_protoRoot, dataptr);
    else {
        if (((struct X3dGroup*)self)->children)
            for (i = 0; i < ((struct X3dGroup*)self)->children_length; i++)
                if (((struct X3dGroup*)self)->children[i])
                    X3dTreeDoWithDataCallback(((struct X3dGroup*)self)->children[i], dataptr);
        X3dGroupDoWithData(self, dataptr);
    }
}


struct X3dPolylineEmitter {
    X3dNode* m_parent;
    X3dNode* m_protoRoot;
    int m_type;
    void *m_data;
    float speed;
    float variation;
    float mass;
    float surfaceArea;
    X3dNode* coord;
    float* direction;
    int direction_length;
    int* coordIndex;
    int coordIndex_length;
    void* extra_data;
};

void X3dPolylineEmitterInit(struct X3dPolylineEmitter* self) {
    self->m_protoRoot = NULL;
    self->speed = 0.0f;
    self->variation = 0.0f;
    self->mass = 0.0f;
    self->surfaceArea = 0.0f;
    self->coord = NULL;
    self->direction = NULL;
    self->direction_length = 0;
    self->coordIndex = NULL;
    self->coordIndex_length = 0;
    self->extra_data = NULL;
}

X3dCallback X3dPolylineEmitterRenderCallback = NULL;
X3dCallback X3dPolylineEmitterTreeRenderCallback = NULL;
X3dCallback X3dPolylineEmitterDoWithDataCallback = NULL;
X3dCallback X3dPolylineEmitterTreeDoWithDataCallback = NULL;

X3dProcessEventCallback X3dPolylineEmitterProcessEventCallback = NULL;

int X3dPolylineEmitterType = 202;

void X3dPolylineEmitterRender(X3dNode* self, void *dataptr) {
    if (X3dPolylineEmitterRenderCallback)
        X3dPolylineEmitterRenderCallback(self, dataptr);
}
void X3dPolylineEmitterTreeRender(X3dNode* self, void *dataptr) {
    int i;
    if (((struct X3dPolylineEmitter*)self)->m_protoRoot != NULL)
        X3dTreeRenderCallback(((struct X3dPolylineEmitter*)self)->m_protoRoot, dataptr);
    else {
        if (((struct X3dPolylineEmitter*)self)->coord)
            X3dTreeRenderCallback(((struct X3dPolylineEmitter*)self)->coord, dataptr);
        X3dPolylineEmitterRender(self, dataptr);
    }
}
void X3dPolylineEmitterDoWithData(X3dNode* self, void *dataptr) {
    if (X3dPolylineEmitterDoWithDataCallback)
        X3dPolylineEmitterDoWithDataCallback(self, dataptr);
}
void X3dPolylineEmitterTreeDoWithData(X3dNode* self, void *dataptr) {
    int i;
    if (((struct X3dPolylineEmitter*)self)->m_protoRoot != NULL)
        X3dTreeDoWithDataCallback(((struct X3dPolylineEmitter*)self)->m_protoRoot, dataptr);
    else {
        if (((struct X3dPolylineEmitter*)self)->coord)
            X3dTreeDoWithDataCallback(((struct X3dPolylineEmitter*)self)->coord, dataptr);
        X3dPolylineEmitterDoWithData(self, dataptr);
    }
}


struct X3dPositionInterpolator {
    X3dNode* m_parent;
    X3dNode* m_protoRoot;
    int m_type;
    void *m_data;
    float set_fraction;
    float* value_changed;
    int value_changed_length;
    float* key;
    int key_length;
    float* keyValue;
    int keyValue_length;
    void* extra_data;
};

void X3dPositionInterpolatorInit(struct X3dPositionInterpolator* self) {
    self->m_protoRoot = NULL;
    self->key = NULL;
    self->key_length = 0;
    self->keyValue = NULL;
    self->keyValue_length = 0;
    self->extra_data = NULL;
}

X3dCallback X3dPositionInterpolatorRenderCallback = NULL;
X3dCallback X3dPositionInterpolatorTreeRenderCallback = NULL;
X3dCallback X3dPositionInterpolatorDoWithDataCallback = NULL;
X3dCallback X3dPositionInterpolatorTreeDoWithDataCallback = NULL;

X3dProcessEventCallback X3dPositionInterpolatorProcessEventCallback = NULL;

int X3dPositionInterpolatorType = 210;

void X3dPositionInterpolatorRender(X3dNode* self, void *dataptr) {
    if (X3dPositionInterpolatorRenderCallback)
        X3dPositionInterpolatorRenderCallback(self, dataptr);
}
void X3dPositionInterpolatorTreeRender(X3dNode* self, void *dataptr) {
    int i;
    if (((struct X3dPositionInterpolator*)self)->m_protoRoot != NULL)
        X3dTreeRenderCallback(((struct X3dPositionInterpolator*)self)->m_protoRoot, dataptr);
    else {
        X3dPositionInterpolatorRender(self, dataptr);
    }
}
void X3dPositionInterpolatorDoWithData(X3dNode* self, void *dataptr) {
    if (X3dPositionInterpolatorDoWithDataCallback)
        X3dPositionInterpolatorDoWithDataCallback(self, dataptr);
}
void X3dPositionInterpolatorTreeDoWithData(X3dNode* self, void *dataptr) {
    int i;
    if (((struct X3dPositionInterpolator*)self)->m_protoRoot != NULL)
        X3dTreeDoWithDataCallback(((struct X3dPositionInterpolator*)self)->m_protoRoot, dataptr);
    else {
        X3dPositionInterpolatorDoWithData(self, dataptr);
    }
}


struct X3dCoordinate {
    X3dNode* m_parent;
    X3dNode* m_protoRoot;
    int m_type;
    void *m_data;
    float* point;
    int point_length;
    void* extra_data;
};

void X3dCoordinateInit(struct X3dCoordinate* self) {
    self->m_protoRoot = NULL;
    self->point = NULL;
    self->point_length = 0;
    self->extra_data = NULL;
}

X3dCallback X3dCoordinateRenderCallback = NULL;
X3dCallback X3dCoordinateTreeRenderCallback = NULL;
X3dCallback X3dCoordinateDoWithDataCallback = NULL;
X3dCallback X3dCoordinateTreeDoWithDataCallback = NULL;

X3dProcessEventCallback X3dCoordinateProcessEventCallback = NULL;

int X3dCoordinateType = 52;

void X3dCoordinateRender(X3dNode* self, void *dataptr) {
    if (X3dCoordinateRenderCallback)
        X3dCoordinateRenderCallback(self, dataptr);
}
void X3dCoordinateTreeRender(X3dNode* self, void *dataptr) {
    int i;
    if (((struct X3dCoordinate*)self)->m_protoRoot != NULL)
        X3dTreeRenderCallback(((struct X3dCoordinate*)self)->m_protoRoot, dataptr);
    else {
        X3dCoordinateRender(self, dataptr);
    }
}
void X3dCoordinateDoWithData(X3dNode* self, void *dataptr) {
    if (X3dCoordinateDoWithDataCallback)
        X3dCoordinateDoWithDataCallback(self, dataptr);
}
void X3dCoordinateTreeDoWithData(X3dNode* self, void *dataptr) {
    int i;
    if (((struct X3dCoordinate*)self)->m_protoRoot != NULL)
        X3dTreeDoWithDataCallback(((struct X3dCoordinate*)self)->m_protoRoot, dataptr);
    else {
        X3dCoordinateDoWithData(self, dataptr);
    }
}


struct X3dForcePhysicsModel {
    X3dNode* m_parent;
    X3dNode* m_protoRoot;
    int m_type;
    void *m_data;
    short enabled;
    float* force;
    int force_length;
    void* extra_data;
};

void X3dForcePhysicsModelInit(struct X3dForcePhysicsModel* self) {
    self->m_protoRoot = NULL;
    self->enabled = 0;
    self->force = NULL;
    self->force_length = 0;
    self->extra_data = NULL;
}

X3dCallback X3dForcePhysicsModelRenderCallback = NULL;
X3dCallback X3dForcePhysicsModelTreeRenderCallback = NULL;
X3dCallback X3dForcePhysicsModelDoWithDataCallback = NULL;
X3dCallback X3dForcePhysicsModelTreeDoWithDataCallback = NULL;

X3dProcessEventCallback X3dForcePhysicsModelProcessEventCallback = NULL;

int X3dForcePhysicsModelType = 90;

void X3dForcePhysicsModelRender(X3dNode* self, void *dataptr) {
    if (X3dForcePhysicsModelRenderCallback)
        X3dForcePhysicsModelRenderCallback(self, dataptr);
}
void X3dForcePhysicsModelTreeRender(X3dNode* self, void *dataptr) {
    int i;
    if (((struct X3dForcePhysicsModel*)self)->m_protoRoot != NULL)
        X3dTreeRenderCallback(((struct X3dForcePhysicsModel*)self)->m_protoRoot, dataptr);
    else {
        X3dForcePhysicsModelRender(self, dataptr);
    }
}
void X3dForcePhysicsModelDoWithData(X3dNode* self, void *dataptr) {
    if (X3dForcePhysicsModelDoWithDataCallback)
        X3dForcePhysicsModelDoWithDataCallback(self, dataptr);
}
void X3dForcePhysicsModelTreeDoWithData(X3dNode* self, void *dataptr) {
    int i;
    if (((struct X3dForcePhysicsModel*)self)->m_protoRoot != NULL)
        X3dTreeDoWithDataCallback(((struct X3dForcePhysicsModel*)self)->m_protoRoot, dataptr);
    else {
        X3dForcePhysicsModelDoWithData(self, dataptr);
    }
}


struct X3dSphereSensor {
    X3dNode* m_parent;
    X3dNode* m_protoRoot;
    int m_type;
    void *m_data;
    short isActive;
    short isOver;
    float* rotation_changed;
    int rotation_changed_length;
    float* trackPoint_changed;
    int trackPoint_changed_length;
    short autoOffset;
    const char* description;
    short enabled;
    float* offset;
    int offset_length;
    void* extra_data;
};

void X3dSphereSensorInit(struct X3dSphereSensor* self) {
    self->m_protoRoot = NULL;
    self->autoOffset = 0;
    self->description = NULL;
    self->enabled = 0;
    self->offset = NULL;
    self->offset_length = 0;
    self->extra_data = NULL;
}

X3dCallback X3dSphereSensorRenderCallback = NULL;
X3dCallback X3dSphereSensorTreeRenderCallback = NULL;
X3dCallback X3dSphereSensorDoWithDataCallback = NULL;
X3dCallback X3dSphereSensorTreeDoWithDataCallback = NULL;

X3dProcessEventCallback X3dSphereSensorProcessEventCallback = NULL;

int X3dSphereSensorType = 253;

void X3dSphereSensorRender(X3dNode* self, void *dataptr) {
    if (X3dSphereSensorRenderCallback)
        X3dSphereSensorRenderCallback(self, dataptr);
}
void X3dSphereSensorTreeRender(X3dNode* self, void *dataptr) {
    int i;
    if (((struct X3dSphereSensor*)self)->m_protoRoot != NULL)
        X3dTreeRenderCallback(((struct X3dSphereSensor*)self)->m_protoRoot, dataptr);
    else {
        X3dSphereSensorRender(self, dataptr);
    }
}
void X3dSphereSensorDoWithData(X3dNode* self, void *dataptr) {
    if (X3dSphereSensorDoWithDataCallback)
        X3dSphereSensorDoWithDataCallback(self, dataptr);
}
void X3dSphereSensorTreeDoWithData(X3dNode* self, void *dataptr) {
    int i;
    if (((struct X3dSphereSensor*)self)->m_protoRoot != NULL)
        X3dTreeDoWithDataCallback(((struct X3dSphereSensor*)self)->m_protoRoot, dataptr);
    else {
        X3dSphereSensorDoWithData(self, dataptr);
    }
}


struct X3dSurfaceEmitter {
    X3dNode* m_parent;
    X3dNode* m_protoRoot;
    int m_type;
    void *m_data;
    float speed;
    float variation;
    float mass;
    float surfaceArea;
    int* coordIndex;
    int coordIndex_length;
    X3dNode* surface;
    void* extra_data;
};

void X3dSurfaceEmitterInit(struct X3dSurfaceEmitter* self) {
    self->m_protoRoot = NULL;
    self->speed = 0.0f;
    self->variation = 0.0f;
    self->mass = 0.0f;
    self->surfaceArea = 0.0f;
    self->coordIndex = NULL;
    self->coordIndex_length = 0;
    self->surface = NULL;
    self->extra_data = NULL;
}

X3dCallback X3dSurfaceEmitterRenderCallback = NULL;
X3dCallback X3dSurfaceEmitterTreeRenderCallback = NULL;
X3dCallback X3dSurfaceEmitterDoWithDataCallback = NULL;
X3dCallback X3dSurfaceEmitterTreeDoWithDataCallback = NULL;

X3dProcessEventCallback X3dSurfaceEmitterProcessEventCallback = NULL;

int X3dSurfaceEmitterType = 267;

void X3dSurfaceEmitterRender(X3dNode* self, void *dataptr) {
    if (X3dSurfaceEmitterRenderCallback)
        X3dSurfaceEmitterRenderCallback(self, dataptr);
}
void X3dSurfaceEmitterTreeRender(X3dNode* self, void *dataptr) {
    int i;
    if (((struct X3dSurfaceEmitter*)self)->m_protoRoot != NULL)
        X3dTreeRenderCallback(((struct X3dSurfaceEmitter*)self)->m_protoRoot, dataptr);
    else {
        if (((struct X3dSurfaceEmitter*)self)->surface)
            X3dTreeRenderCallback(((struct X3dSurfaceEmitter*)self)->surface, dataptr);
        X3dSurfaceEmitterRender(self, dataptr);
    }
}
void X3dSurfaceEmitterDoWithData(X3dNode* self, void *dataptr) {
    if (X3dSurfaceEmitterDoWithDataCallback)
        X3dSurfaceEmitterDoWithDataCallback(self, dataptr);
}
void X3dSurfaceEmitterTreeDoWithData(X3dNode* self, void *dataptr) {
    int i;
    if (((struct X3dSurfaceEmitter*)self)->m_protoRoot != NULL)
        X3dTreeDoWithDataCallback(((struct X3dSurfaceEmitter*)self)->m_protoRoot, dataptr);
    else {
        if (((struct X3dSurfaceEmitter*)self)->surface)
            X3dTreeDoWithDataCallback(((struct X3dSurfaceEmitter*)self)->surface, dataptr);
        X3dSurfaceEmitterDoWithData(self, dataptr);
    }
}


struct X3dTouchSensor {
    X3dNode* m_parent;
    X3dNode* m_protoRoot;
    int m_type;
    void *m_data;
    float* hitNormal_changed;
    int hitNormal_changed_length;
    float* hitPoint_changed;
    int hitPoint_changed_length;
    float* hitTexCoord_changed;
    int hitTexCoord_changed_length;
    short isActive;
    short isOver;
    double touchTime;
    const char* description;
    short enabled;
    void* extra_data;
};

void X3dTouchSensorInit(struct X3dTouchSensor* self) {
    self->m_protoRoot = NULL;
    self->description = NULL;
    self->enabled = 0;
    self->extra_data = NULL;
}

X3dCallback X3dTouchSensorRenderCallback = NULL;
X3dCallback X3dTouchSensorTreeRenderCallback = NULL;
X3dCallback X3dTouchSensorDoWithDataCallback = NULL;
X3dCallback X3dTouchSensorTreeDoWithDataCallback = NULL;

X3dProcessEventCallback X3dTouchSensorProcessEventCallback = NULL;

int X3dTouchSensorType = 288;

void X3dTouchSensorRender(X3dNode* self, void *dataptr) {
    if (X3dTouchSensorRenderCallback)
        X3dTouchSensorRenderCallback(self, dataptr);
}
void X3dTouchSensorTreeRender(X3dNode* self, void *dataptr) {
    int i;
    if (((struct X3dTouchSensor*)self)->m_protoRoot != NULL)
        X3dTreeRenderCallback(((struct X3dTouchSensor*)self)->m_protoRoot, dataptr);
    else {
        X3dTouchSensorRender(self, dataptr);
    }
}
void X3dTouchSensorDoWithData(X3dNode* self, void *dataptr) {
    if (X3dTouchSensorDoWithDataCallback)
        X3dTouchSensorDoWithDataCallback(self, dataptr);
}
void X3dTouchSensorTreeDoWithData(X3dNode* self, void *dataptr) {
    int i;
    if (((struct X3dTouchSensor*)self)->m_protoRoot != NULL)
        X3dTreeDoWithDataCallback(((struct X3dTouchSensor*)self)->m_protoRoot, dataptr);
    else {
        X3dTouchSensorDoWithData(self, dataptr);
    }
}


struct X3dPointLight {
    X3dNode* m_parent;
    X3dNode* m_protoRoot;
    int m_type;
    void *m_data;
    float ambientIntensity;
    float* attenuation;
    int attenuation_length;
    float* color;
    int color_length;
    short global;
    float intensity;
    float* location;
    int location_length;
    short on;
    float radius;
    short shadows;
    short kambiShadows;
    short kambiShadowsMain;
    float projectionNear;
    float projectionFar;
    float* up;
    int up_length;
    X3dNode* defaultShadowMap;
    X3dNode** effects;
    int effects_length;
    short shadowVolumes;
    short shadowVolumesMain;
    void* extra_data;
};

void X3dPointLightInit(struct X3dPointLight* self) {
    self->m_protoRoot = NULL;
    self->ambientIntensity = 0.0f;
    self->attenuation = NULL;
    self->attenuation_length = 0;
    self->color = NULL;
    self->color_length = 0;
    self->global = 0;
    self->intensity = 0.0f;
    self->location = NULL;
    self->location_length = 0;
    self->on = 0;
    self->radius = 0.0f;
    self->shadows = 0;
    self->kambiShadows = 0;
    self->kambiShadowsMain = 0;
    self->projectionNear = 0.0f;
    self->projectionFar = 0.0f;
    self->up = NULL;
    self->up_length = 0;
    self->defaultShadowMap = NULL;
    self->effects = NULL;
    self->effects_length = 0;
    self->shadowVolumes = 0;
    self->shadowVolumesMain = 0;
    self->extra_data = NULL;
}

X3dCallback X3dPointLightRenderCallback = NULL;
X3dCallback X3dPointLightTreeRenderCallback = NULL;
X3dCallback X3dPointLightDoWithDataCallback = NULL;
X3dCallback X3dPointLightTreeDoWithDataCallback = NULL;

X3dProcessEventCallback X3dPointLightProcessEventCallback = NULL;

int X3dPointLightType = 198;

void X3dPointLightRender(X3dNode* self, void *dataptr) {
    if (X3dPointLightRenderCallback)
        X3dPointLightRenderCallback(self, dataptr);
}
void X3dPointLightTreeRender(X3dNode* self, void *dataptr) {
    int i;
    if (((struct X3dPointLight*)self)->m_protoRoot != NULL)
        X3dTreeRenderCallback(((struct X3dPointLight*)self)->m_protoRoot, dataptr);
    else {
        if (((struct X3dPointLight*)self)->defaultShadowMap)
            X3dTreeRenderCallback(((struct X3dPointLight*)self)->defaultShadowMap, dataptr);
        if (((struct X3dPointLight*)self)->effects)
            for (i = 0; i < ((struct X3dPointLight*)self)->effects_length; i++)
                if (((struct X3dPointLight*)self)->effects[i])
                    X3dTreeRenderCallback(((struct X3dPointLight*)self)->effects[i], dataptr);
        X3dPointLightRender(self, dataptr);
    }
}
void X3dPointLightDoWithData(X3dNode* self, void *dataptr) {
    if (X3dPointLightDoWithDataCallback)
        X3dPointLightDoWithDataCallback(self, dataptr);
}
void X3dPointLightTreeDoWithData(X3dNode* self, void *dataptr) {
    int i;
    if (((struct X3dPointLight*)self)->m_protoRoot != NULL)
        X3dTreeDoWithDataCallback(((struct X3dPointLight*)self)->m_protoRoot, dataptr);
    else {
        if (((struct X3dPointLight*)self)->defaultShadowMap)
            X3dTreeDoWithDataCallback(((struct X3dPointLight*)self)->defaultShadowMap, dataptr);
        if (((struct X3dPointLight*)self)->effects)
            for (i = 0; i < ((struct X3dPointLight*)self)->effects_length; i++)
                if (((struct X3dPointLight*)self)->effects[i])
                    X3dTreeDoWithDataCallback(((struct X3dPointLight*)self)->effects[i], dataptr);
        X3dPointLightDoWithData(self, dataptr);
    }
}


struct X3dAppearance {
    X3dNode* m_parent;
    X3dNode* m_protoRoot;
    int m_type;
    void *m_data;
    X3dNode* material;
    X3dNode* texture;
    X3dNode* textureTransform;
    X3dNode* fillProperties;
    X3dNode* lineProperties;
    X3dNode** shaders;
    int shaders_length;
    X3dNode** receiveShadows;
    int receiveShadows_length;
    short shadowCaster;
    X3dNode* normalMap;
    X3dNode* heightMap;
    float heightMapScale;
    X3dNode** effects;
    int effects_length;
    void* extra_data;
};

void X3dAppearanceInit(struct X3dAppearance* self) {
    self->m_protoRoot = NULL;
    self->material = NULL;
    self->texture = NULL;
    self->textureTransform = NULL;
    self->fillProperties = NULL;
    self->lineProperties = NULL;
    self->shaders = NULL;
    self->shaders_length = 0;
    self->receiveShadows = NULL;
    self->receiveShadows_length = 0;
    self->shadowCaster = 0;
    self->normalMap = NULL;
    self->heightMap = NULL;
    self->heightMapScale = 0.0f;
    self->effects = NULL;
    self->effects_length = 0;
    self->extra_data = NULL;
}

X3dCallback X3dAppearanceRenderCallback = NULL;
X3dCallback X3dAppearanceTreeRenderCallback = NULL;
X3dCallback X3dAppearanceDoWithDataCallback = NULL;
X3dCallback X3dAppearanceTreeDoWithDataCallback = NULL;

X3dProcessEventCallback X3dAppearanceProcessEventCallback = NULL;

int X3dAppearanceType = 1;

void X3dAppearanceRender(X3dNode* self, void *dataptr) {
    if (X3dAppearanceRenderCallback)
        X3dAppearanceRenderCallback(self, dataptr);
}
void X3dAppearanceTreeRender(X3dNode* self, void *dataptr) {
    int i;
    if (((struct X3dAppearance*)self)->m_protoRoot != NULL)
        X3dTreeRenderCallback(((struct X3dAppearance*)self)->m_protoRoot, dataptr);
    else {
        if (((struct X3dAppearance*)self)->material)
            X3dTreeRenderCallback(((struct X3dAppearance*)self)->material, dataptr);
        if (((struct X3dAppearance*)self)->texture)
            X3dTreeRenderCallback(((struct X3dAppearance*)self)->texture, dataptr);
        if (((struct X3dAppearance*)self)->textureTransform)
            X3dTreeRenderCallback(((struct X3dAppearance*)self)->textureTransform, dataptr);
        if (((struct X3dAppearance*)self)->fillProperties)
            X3dTreeRenderCallback(((struct X3dAppearance*)self)->fillProperties, dataptr);
        if (((struct X3dAppearance*)self)->lineProperties)
            X3dTreeRenderCallback(((struct X3dAppearance*)self)->lineProperties, dataptr);
        if (((struct X3dAppearance*)self)->shaders)
            for (i = 0; i < ((struct X3dAppearance*)self)->shaders_length; i++)
                if (((struct X3dAppearance*)self)->shaders[i])
                    X3dTreeRenderCallback(((struct X3dAppearance*)self)->shaders[i], dataptr);
        if (((struct X3dAppearance*)self)->receiveShadows)
            for (i = 0; i < ((struct X3dAppearance*)self)->receiveShadows_length; i++)
                if (((struct X3dAppearance*)self)->receiveShadows[i])
                    X3dTreeRenderCallback(((struct X3dAppearance*)self)->receiveShadows[i], dataptr);
        if (((struct X3dAppearance*)self)->normalMap)
            X3dTreeRenderCallback(((struct X3dAppearance*)self)->normalMap, dataptr);
        if (((struct X3dAppearance*)self)->heightMap)
            X3dTreeRenderCallback(((struct X3dAppearance*)self)->heightMap, dataptr);
        if (((struct X3dAppearance*)self)->effects)
            for (i = 0; i < ((struct X3dAppearance*)self)->effects_length; i++)
                if (((struct X3dAppearance*)self)->effects[i])
                    X3dTreeRenderCallback(((struct X3dAppearance*)self)->effects[i], dataptr);
        X3dAppearanceRender(self, dataptr);
    }
}
void X3dAppearanceDoWithData(X3dNode* self, void *dataptr) {
    if (X3dAppearanceDoWithDataCallback)
        X3dAppearanceDoWithDataCallback(self, dataptr);
}
void X3dAppearanceTreeDoWithData(X3dNode* self, void *dataptr) {
    int i;
    if (((struct X3dAppearance*)self)->m_protoRoot != NULL)
        X3dTreeDoWithDataCallback(((struct X3dAppearance*)self)->m_protoRoot, dataptr);
    else {
        if (((struct X3dAppearance*)self)->material)
            X3dTreeDoWithDataCallback(((struct X3dAppearance*)self)->material, dataptr);
        if (((struct X3dAppearance*)self)->texture)
            X3dTreeDoWithDataCallback(((struct X3dAppearance*)self)->texture, dataptr);
        if (((struct X3dAppearance*)self)->textureTransform)
            X3dTreeDoWithDataCallback(((struct X3dAppearance*)self)->textureTransform, dataptr);
        if (((struct X3dAppearance*)self)->fillProperties)
            X3dTreeDoWithDataCallback(((struct X3dAppearance*)self)->fillProperties, dataptr);
        if (((struct X3dAppearance*)self)->lineProperties)
            X3dTreeDoWithDataCallback(((struct X3dAppearance*)self)->lineProperties, dataptr);
        if (((struct X3dAppearance*)self)->shaders)
            for (i = 0; i < ((struct X3dAppearance*)self)->shaders_length; i++)
                if (((struct X3dAppearance*)self)->shaders[i])
                    X3dTreeDoWithDataCallback(((struct X3dAppearance*)self)->shaders[i], dataptr);
        if (((struct X3dAppearance*)self)->receiveShadows)
            for (i = 0; i < ((struct X3dAppearance*)self)->receiveShadows_length; i++)
                if (((struct X3dAppearance*)self)->receiveShadows[i])
                    X3dTreeDoWithDataCallback(((struct X3dAppearance*)self)->receiveShadows[i], dataptr);
        if (((struct X3dAppearance*)self)->normalMap)
            X3dTreeDoWithDataCallback(((struct X3dAppearance*)self)->normalMap, dataptr);
        if (((struct X3dAppearance*)self)->heightMap)
            X3dTreeDoWithDataCallback(((struct X3dAppearance*)self)->heightMap, dataptr);
        if (((struct X3dAppearance*)self)->effects)
            for (i = 0; i < ((struct X3dAppearance*)self)->effects_length; i++)
                if (((struct X3dAppearance*)self)->effects[i])
                    X3dTreeDoWithDataCallback(((struct X3dAppearance*)self)->effects[i], dataptr);
        X3dAppearanceDoWithData(self, dataptr);
    }
}


struct X3dParticleSystem {
    X3dNode* m_parent;
    X3dNode* m_protoRoot;
    int m_type;
    void *m_data;
    X3dNode* appearance;
    short createParticles;
    X3dNode* geometry;
    short enabled;
    float lifetimeVariation;
    int maxParticles;
    float particleLifetime;
    float* particleSize;
    int particleSize_length;
    float* bboxCenter;
    int bboxCenter_length;
    float* bboxSize;
    int bboxSize_length;
    X3dNode* colorRamp;
    float* colorKey;
    int colorKey_length;
    X3dNode* emitter;
    const char* geometryType;
    X3dNode** physics;
    int physics_length;
    X3dNode* texCoordRamp;
    float* texCoordKey;
    int texCoordKey_length;
    void* extra_data;
};

void X3dParticleSystemInit(struct X3dParticleSystem* self) {
    self->m_protoRoot = NULL;
    self->appearance = NULL;
    self->createParticles = 0;
    self->geometry = NULL;
    self->enabled = 0;
    self->lifetimeVariation = 0.0f;
    self->maxParticles = 0;
    self->particleLifetime = 0.0f;
    self->particleSize = NULL;
    self->particleSize_length = 0;
    self->bboxCenter = NULL;
    self->bboxCenter_length = 0;
    self->bboxSize = NULL;
    self->bboxSize_length = 0;
    self->colorRamp = NULL;
    self->colorKey = NULL;
    self->colorKey_length = 0;
    self->emitter = NULL;
    self->geometryType = NULL;
    self->physics = NULL;
    self->physics_length = 0;
    self->texCoordRamp = NULL;
    self->texCoordKey = NULL;
    self->texCoordKey_length = 0;
    self->extra_data = NULL;
}

X3dCallback X3dParticleSystemRenderCallback = NULL;
X3dCallback X3dParticleSystemTreeRenderCallback = NULL;
X3dCallback X3dParticleSystemDoWithDataCallback = NULL;
X3dCallback X3dParticleSystemTreeDoWithDataCallback = NULL;

X3dProcessEventCallback X3dParticleSystemProcessEventCallback = NULL;

int X3dParticleSystemType = 191;

void X3dParticleSystemRender(X3dNode* self, void *dataptr) {
    if (X3dParticleSystemRenderCallback)
        X3dParticleSystemRenderCallback(self, dataptr);
}
void X3dParticleSystemTreeRender(X3dNode* self, void *dataptr) {
    int i;
    if (((struct X3dParticleSystem*)self)->m_protoRoot != NULL)
        X3dTreeRenderCallback(((struct X3dParticleSystem*)self)->m_protoRoot, dataptr);
    else {
        if (((struct X3dParticleSystem*)self)->appearance)
            X3dTreeRenderCallback(((struct X3dParticleSystem*)self)->appearance, dataptr);
        if (((struct X3dParticleSystem*)self)->geometry)
            X3dTreeRenderCallback(((struct X3dParticleSystem*)self)->geometry, dataptr);
        if (((struct X3dParticleSystem*)self)->colorRamp)
            X3dTreeRenderCallback(((struct X3dParticleSystem*)self)->colorRamp, dataptr);
        if (((struct X3dParticleSystem*)self)->emitter)
            X3dTreeRenderCallback(((struct X3dParticleSystem*)self)->emitter, dataptr);
        if (((struct X3dParticleSystem*)self)->physics)
            for (i = 0; i < ((struct X3dParticleSystem*)self)->physics_length; i++)
                if (((struct X3dParticleSystem*)self)->physics[i])
                    X3dTreeRenderCallback(((struct X3dParticleSystem*)self)->physics[i], dataptr);
        if (((struct X3dParticleSystem*)self)->texCoordRamp)
            X3dTreeRenderCallback(((struct X3dParticleSystem*)self)->texCoordRamp, dataptr);
        X3dParticleSystemRender(self, dataptr);
    }
}
void X3dParticleSystemDoWithData(X3dNode* self, void *dataptr) {
    if (X3dParticleSystemDoWithDataCallback)
        X3dParticleSystemDoWithDataCallback(self, dataptr);
}
void X3dParticleSystemTreeDoWithData(X3dNode* self, void *dataptr) {
    int i;
    if (((struct X3dParticleSystem*)self)->m_protoRoot != NULL)
        X3dTreeDoWithDataCallback(((struct X3dParticleSystem*)self)->m_protoRoot, dataptr);
    else {
        if (((struct X3dParticleSystem*)self)->appearance)
            X3dTreeDoWithDataCallback(((struct X3dParticleSystem*)self)->appearance, dataptr);
        if (((struct X3dParticleSystem*)self)->geometry)
            X3dTreeDoWithDataCallback(((struct X3dParticleSystem*)self)->geometry, dataptr);
        if (((struct X3dParticleSystem*)self)->colorRamp)
            X3dTreeDoWithDataCallback(((struct X3dParticleSystem*)self)->colorRamp, dataptr);
        if (((struct X3dParticleSystem*)self)->emitter)
            X3dTreeDoWithDataCallback(((struct X3dParticleSystem*)self)->emitter, dataptr);
        if (((struct X3dParticleSystem*)self)->physics)
            for (i = 0; i < ((struct X3dParticleSystem*)self)->physics_length; i++)
                if (((struct X3dParticleSystem*)self)->physics[i])
                    X3dTreeDoWithDataCallback(((struct X3dParticleSystem*)self)->physics[i], dataptr);
        if (((struct X3dParticleSystem*)self)->texCoordRamp)
            X3dTreeDoWithDataCallback(((struct X3dParticleSystem*)self)->texCoordRamp, dataptr);
        X3dParticleSystemDoWithData(self, dataptr);
    }
}


struct X3dTextureTransform {
    X3dNode* m_parent;
    X3dNode* m_protoRoot;
    int m_type;
    void *m_data;
    float* center;
    int center_length;
    float rotation;
    float* scale;
    int scale_length;
    float* translation;
    int translation_length;
    void* extra_data;
};

void X3dTextureTransformInit(struct X3dTextureTransform* self) {
    self->m_protoRoot = NULL;
    self->center = NULL;
    self->center_length = 0;
    self->rotation = 0.0f;
    self->scale = NULL;
    self->scale_length = 0;
    self->translation = NULL;
    self->translation_length = 0;
    self->extra_data = NULL;
}

X3dCallback X3dTextureTransformRenderCallback = NULL;
X3dCallback X3dTextureTransformTreeRenderCallback = NULL;
X3dCallback X3dTextureTransformDoWithDataCallback = NULL;
X3dCallback X3dTextureTransformTreeDoWithDataCallback = NULL;

X3dProcessEventCallback X3dTextureTransformProcessEventCallback = NULL;

int X3dTextureTransformType = 281;

void X3dTextureTransformRender(X3dNode* self, void *dataptr) {
    if (X3dTextureTransformRenderCallback)
        X3dTextureTransformRenderCallback(self, dataptr);
}
void X3dTextureTransformTreeRender(X3dNode* self, void *dataptr) {
    int i;
    if (((struct X3dTextureTransform*)self)->m_protoRoot != NULL)
        X3dTreeRenderCallback(((struct X3dTextureTransform*)self)->m_protoRoot, dataptr);
    else {
        X3dTextureTransformRender(self, dataptr);
    }
}
void X3dTextureTransformDoWithData(X3dNode* self, void *dataptr) {
    if (X3dTextureTransformDoWithDataCallback)
        X3dTextureTransformDoWithDataCallback(self, dataptr);
}
void X3dTextureTransformTreeDoWithData(X3dNode* self, void *dataptr) {
    int i;
    if (((struct X3dTextureTransform*)self)->m_protoRoot != NULL)
        X3dTreeDoWithDataCallback(((struct X3dTextureTransform*)self)->m_protoRoot, dataptr);
    else {
        X3dTextureTransformDoWithData(self, dataptr);
    }
}



struct X3dSceneGraph {
    struct X3dGroup root;
    struct X3dTransform Transform1;
    struct X3dShape Shape_9;
    struct X3dAppearance Appearance_10;
    struct X3dMaterial Material_11;
    struct X3dIndexedFaceSet Box_7;
    struct X3dCoordinate Coordinate1;
    struct X3dNormal Normal1;
    struct X3dTextureCoordinate TextureCoordinate1;
    struct X3dCoordinate Coordinate_15;
    struct X3dNormal Normal_16;
    struct X3dTextureCoordinate TextureCoordinate_17;
    struct X3dPlaneSensor PlaneSensor1;
    struct X3dTransform Transform2;
    struct X3dShape Shape_12;
    struct X3dAppearance Appearance_13;
    struct X3dMaterial Material_14;
    struct X3dIndexedFaceSet Sphere_8;
    struct X3dCoordinate Coordinate2;
    struct X3dNormal Normal2;
    struct X3dTextureCoordinate TextureCoordinate2;
    struct X3dCoordinate Coordinate_18;
    struct X3dNormal Normal_19;
    struct X3dTextureCoordinate TextureCoordinate_20;
    struct X3dTimeSensor TimeSensor1;
    struct X3dPositionInterpolator PositionInterpolator1;
};

struct X3dNodeStruct {
    X3dNode *m_parent;
    X3dNode *m_protoRoot;
    int m_type;
    void *m_data;
};

void X3dSceneGraphInit(struct X3dSceneGraph *self) {
    X3dGroupInit(&self->root);
    self->root.m_parent = (X3dNode *)NULL;
    self->Transform1.route_sources = (X3dNode **)malloc(1 * sizeof(X3dNode *));
    self->Transform1.route_sources[0] = &self->PlaneSensor1;
    self->Transform1.num_route_source = 1;
    X3dTransformInit(&self->Transform1);
    self->Transform1.m_parent = (X3dNode *)&self->root;
    X3dShapeInit(&self->Shape_9);
    self->Shape_9.m_parent = (X3dNode *)&self->Transform1;
    X3dAppearanceInit(&self->Appearance_10);
    self->Appearance_10.m_parent = (X3dNode *)&self->Shape_9;
    X3dMaterialInit(&self->Material_11);
    self->Material_11.m_parent = (X3dNode *)&self->Appearance_10;
    self->Material_11.ambientIntensity = 0.200000;
    {
    static float m_diffuseColor[3] = { 0.800000, 0.800000, 0.800000 };
    self->Material_11.diffuseColor = m_diffuseColor;
    self->Material_11.diffuseColor_length = sizeof(m_diffuseColor) / sizeof(float);
    }
    {
    static float m_emissiveColor[3] = { 0, 0, 0 };
    self->Material_11.emissiveColor = m_emissiveColor;
    self->Material_11.emissiveColor_length = sizeof(m_emissiveColor) / sizeof(float);
    }
    self->Material_11.shininess = 0.200000;
    {
    static float m_specularColor[3] = { 0, 0, 0 };
    self->Material_11.specularColor = m_specularColor;
    self->Material_11.specularColor_length = sizeof(m_specularColor) / sizeof(float);
    }
    self->Material_11.transparency = 0;
    self->Material_11.m_type = 147;
    self->Material_11.m_data = NULL;
    self->Appearance_10.material = &self->Material_11;
    self->Appearance_10.texture = NULL;
    self->Appearance_10.textureTransform = NULL;
    self->Appearance_10.m_type = 1;
    self->Appearance_10.m_data = NULL;
    X3dSetGlName(&self->Box_7, 0);
    X3dIndexedFaceSetInit(&self->Box_7);
    self->Box_7.m_parent = (X3dNode *)&self->Shape_9;
    X3dCoordinateInit(&self->Coordinate1);
    self->Coordinate1.m_parent = (X3dNode *)&self->Box_7;
    {
    static float m_point[] = { -1, 1, 1, -1, -1, 1, 1, 1, 1, 1, -1, 1, 1, 1, -1, 1, -1, -1, -1, 1, -1, -1, -1, -1 };
    self->Coordinate1.point = m_point;
    self->Coordinate1.point_length = sizeof(m_point) / sizeof(float);
    }
    self->Coordinate1.m_type = 52;
    self->Coordinate1.m_data = NULL;
    X3dNormalInit(&self->Normal1);
    self->Normal1.m_parent = (X3dNode *)&self->Box_7;
    {
    static float m_vector[] = { 0, 0, 1, 0, 1, 0, -1, 0, 0, 0, 0, 1, 0, -1, 0, -1, 0, 0, 0, 0, 1, 0, -1, 0, 1, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, -1, 0, 1, 0, 1, 0, 0, 0, 0, -1, 0, -1, 0, 1, 0, 0, 0, 0, -1, 0, -1, 0, -1, 0, 0, 0, 0, -1, 0, 1, 0, -1, 0, 0 };
    self->Normal1.vector = m_vector;
    self->Normal1.vector_length = sizeof(m_vector) / sizeof(float);
    }
    self->Normal1.m_type = 167;
    self->Normal1.m_data = NULL;
    X3dTextureCoordinateInit(&self->TextureCoordinate1);
    self->TextureCoordinate1.m_parent = (X3dNode *)&self->Box_7;
    {
    static float m_point[] = { 0, 1, 0, 0, 1, 1, 1, 0 };
    self->TextureCoordinate1.point = m_point;
    self->TextureCoordinate1.point_length = sizeof(m_point) / sizeof(float);
    }
    self->TextureCoordinate1.m_type = 276;
    self->TextureCoordinate1.m_data = NULL;
    self->Box_7.color = NULL;
    self->Box_7.coord = &self->Coordinate1;
    self->Box_7.normal = &self->Normal1;
    self->Box_7.texCoord = &self->TextureCoordinate1;
    self->Box_7.ccw = -1;
    {
    static int m_colorIndex[] = { 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1 };
    self->Box_7.colorIndex = m_colorIndex;
    self->Box_7.colorIndex_length = sizeof(m_colorIndex) / sizeof(int);
    }
    self->Box_7.colorPerVertex = 0;
    self->Box_7.convex = 0;
    {
    static int m_coordIndex[] = { 1, 3, 2, -1, 1, 2, 0, -1, 5, 7, 6, -1, 5, 6, 4, -1, 0, 2, 4, -1, 0, 4, 6, -1, 7, 5, 3, -1, 7, 3, 1, -1, 7, 1, 0, -1, 7, 0, 6, -1, 2, 3, 5, -1, 2, 5, 4, -1 };
    self->Box_7.coordIndex = m_coordIndex;
    self->Box_7.coordIndex_length = sizeof(m_coordIndex) / sizeof(int);
    }
    self->Box_7.creaseAngle = 0;
    {
    static int m_normalIndex[] = { 3, 6, 9, -1, 3, 9, 0, -1, 15, 18, 21, -1, 15, 21, 12, -1, 1, 10, 13, -1, 1, 13, 22, -1, 19, 16, 7, -1, 19, 7, 4, -1, 20, 5, 2, -1, 20, 2, 23, -1, 11, 8, 17, -1, 11, 17, 14, -1 };
    self->Box_7.normalIndex = m_normalIndex;
    self->Box_7.normalIndex_length = sizeof(m_normalIndex) / sizeof(int);
    }
    self->Box_7.normalPerVertex = -1;
    self->Box_7.solid = -1;
    {
    static int m_texCoordIndex[] = { 1, 3, 2, -1, 1, 2, 0, -1, 1, 3, 2, -1, 1, 2, 0, -1, 1, 3, 2, -1, 1, 2, 0, -1, 1, 3, 2, -1, 1, 2, 0, -1, 1, 3, 2, -1, 1, 2, 0, -1, 1, 3, 2, -1, 1, 2, 0, -1 };
    self->Box_7.texCoordIndex = m_texCoordIndex;
    self->Box_7.texCoordIndex_length = sizeof(m_texCoordIndex) / sizeof(int);
    }
    {
    static int m_set_colorIndex[1];
    self->Box_7.set_colorIndex = NULL;
    self->Box_7.set_colorIndex_length = 0;
    }
    {
    static int m_set_coordIndex[1];
    self->Box_7.set_coordIndex = NULL;
    self->Box_7.set_coordIndex_length = 0;
    }
    {
    static int m_set_normalIndex[1];
    self->Box_7.set_normalIndex = NULL;
    self->Box_7.set_normalIndex_length = 0;
    }
    {
    static int m_set_texCoordIndex[1];
    self->Box_7.set_texCoordIndex = NULL;
    self->Box_7.set_texCoordIndex_length = 0;
    }
    self->Box_7.m_type = 115;
    self->Box_7.m_data = NULL;
    self->Shape_9.appearance = &self->Appearance_10;
    self->Shape_9.geometry = &self->Box_7;
    self->Shape_9.m_type = 240;
    self->Shape_9.m_data = NULL;
    X3dPlaneSensorInit(&self->PlaneSensor1);
    self->PlaneSensor1.m_parent = (X3dNode *)&self->Transform1;
    self->PlaneSensor1.autoOffset = -1;
    self->PlaneSensor1.enabled = -1;
    {
    static float m_maxPosition[2] = { -1, -1 };
    self->PlaneSensor1.maxPosition = m_maxPosition;
    self->PlaneSensor1.maxPosition_length = sizeof(m_maxPosition) / sizeof(float);
    }
    {
    static float m_minPosition[2] = { 0, 0 };
    self->PlaneSensor1.minPosition = m_minPosition;
    self->PlaneSensor1.minPosition_length = sizeof(m_minPosition) / sizeof(float);
    }
    {
    static float m_offset[3] = { 0, 0, 0 };
    self->PlaneSensor1.offset = m_offset;
    self->PlaneSensor1.offset_length = sizeof(m_offset) / sizeof(float);
    }
    self->PlaneSensor1.isActive = 0;
    {
    static float m_trackPoint_changed[3] = { 0, 0, 0 };
    self->PlaneSensor1.trackPoint_changed = m_trackPoint_changed;
    self->PlaneSensor1.trackPoint_changed_length = sizeof(m_trackPoint_changed) / sizeof(float);
    }
    {
    static float m_translation_changed[3] = { 0, 0, 0 };
    self->PlaneSensor1.translation_changed = m_translation_changed;
    self->PlaneSensor1.translation_changed_length = sizeof(m_translation_changed) / sizeof(float);
    }
    self->PlaneSensor1.m_type = 196;
    self->PlaneSensor1.m_data = NULL;
    {
    static float m_rotation[4] = { 0, 0, 1, 0 };
    self->Transform1.rotation = m_rotation;
    self->Transform1.rotation_length = sizeof(m_rotation) / sizeof(float);
    }
    {
    static float m_translation[3] = { 1.825035, 0.188337, 0.292423 };
    self->Transform1.translation = m_translation;
    self->Transform1.translation_length = sizeof(m_translation) / sizeof(float);
    }
    {
    static float m_bboxCenter[3] = { 0, 0, 0 };
    self->Transform1.bboxCenter = m_bboxCenter;
    self->Transform1.bboxCenter_length = sizeof(m_bboxCenter) / sizeof(float);
    }
    {
    static float m_bboxSize[3] = { -1, -1, -1 };
    self->Transform1.bboxSize = m_bboxSize;
    self->Transform1.bboxSize_length = sizeof(m_bboxSize) / sizeof(float);
    }
    {
    static float m_center[3] = { 0, 0, 0 };
    self->Transform1.center = m_center;
    self->Transform1.center_length = sizeof(m_center) / sizeof(float);
    }
    {
    static float m_scale[3] = { 1, 1, 1 };
    self->Transform1.scale = m_scale;
    self->Transform1.scale_length = sizeof(m_scale) / sizeof(float);
    }
    {
    static float m_scaleOrientation[4] = { 0, 0, 1, 0 };
    self->Transform1.scaleOrientation = m_scaleOrientation;
    self->Transform1.scaleOrientation_length = sizeof(m_scaleOrientation) / sizeof(float);
    }
    {
    static X3dNode* m_children[2];
    m_children[0] =&self->Shape_9;
    m_children[1] =&self->PlaneSensor1;
    self->Transform1.children = m_children;
    self->Transform1.children_length = sizeof(m_children) / sizeof(X3dNode*);
    }
    {
    static X3dNode* m_addChildren = NULL;
    self->Transform1.addChildren = NULL;
    self->Transform1.addChildren_length = 0;
    }
    {
    static X3dNode* m_removeChildren = NULL;
    self->Transform1.removeChildren = NULL;
    self->Transform1.removeChildren_length = 0;
    }
    self->Transform1.m_type = 289;
    self->Transform1.m_data = NULL;
    self->Transform2.route_sources =  NULL;
    self->Transform2.num_route_source = 0;
    X3dTransformInit(&self->Transform2);
    self->Transform2.m_parent = (X3dNode *)&self->root;
    X3dShapeInit(&self->Shape_12);
    self->Shape_12.m_parent = (X3dNode *)&self->Transform2;
    X3dAppearanceInit(&self->Appearance_13);
    self->Appearance_13.m_parent = (X3dNode *)&self->Shape_12;
    X3dMaterialInit(&self->Material_14);
    self->Material_14.m_parent = (X3dNode *)&self->Appearance_13;
    self->Material_14.ambientIntensity = 0.200000;
    {
    static float m_diffuseColor[3] = { 0.800000, 0.800000, 0.800000 };
    self->Material_14.diffuseColor = m_diffuseColor;
    self->Material_14.diffuseColor_length = sizeof(m_diffuseColor) / sizeof(float);
    }
    {
    static float m_emissiveColor[3] = { 0, 0, 0 };
    self->Material_14.emissiveColor = m_emissiveColor;
    self->Material_14.emissiveColor_length = sizeof(m_emissiveColor) / sizeof(float);
    }
    self->Material_14.shininess = 0.200000;
    {
    static float m_specularColor[3] = { 0, 0, 0 };
    self->Material_14.specularColor = m_specularColor;
    self->Material_14.specularColor_length = sizeof(m_specularColor) / sizeof(float);
    }
    self->Material_14.transparency = 0;
    self->Material_14.m_type = 147;
    self->Material_14.m_data = NULL;
    self->Appearance_13.material = &self->Material_14;
    self->Appearance_13.texture = NULL;
    self->Appearance_13.textureTransform = NULL;
    self->Appearance_13.m_type = 1;
    self->Appearance_13.m_data = NULL;
    X3dSetGlName(&self->Sphere_8, 1);
    X3dIndexedFaceSetInit(&self->Sphere_8);
    self->Sphere_8.m_parent = (X3dNode *)&self->Shape_12;
    X3dCoordinateInit(&self->Coordinate2);
    self->Coordinate2.m_parent = (X3dNode *)&self->Sphere_8;
    {
    static float m_point[] = { 0, 1, 0, 0, 0.993527, 0.104528, 0.021062, 0.993527, 0.102389, 0.041263, 0.993527, 0.096057, 0.059774, 0.993527, 0.085793, 0.075837, 0.993527, 0.072017, 0.088796, 0.993527, 0.055292, 0.098120, 0.993527, 0.036303, 0.103426, 0.993527, 0.015829, 0.104499, 0.993527, -0.005294, 0.101293, 0.993527, -0.026200, 0.093940, 0.993527, -0.046034, 0.082741, 0.993527, -0.063983, 0.068155, 0.993527, -0.079312, 0.050779, 0.993527, -0.091394, 0.031323, 0.993527, -0.099735, 0.010586, 0.993527, -0.103992, -0.010586, 0.993527, -0.103992, -0.031323, 0.993527, -0.099735, -0.050779, 0.993527, -0.091394, -0.068155, 0.993527, -0.079312, -0.082741, 0.993527, -0.063982, -0.093940, 0.993527, -0.046034, -0.101293, 0.993527, -0.026200, -0.104499, 0.993527, -0.005294, -0.103426, 0.993527, 0.015828, -0.098120, 0.993527, 0.036303, -0.088796, 0.993527, 0.055292, -0.075837, 0.993527, 0.072017, -0.059774, 0.993527, 0.085793, -0.041263, 0.993527, 0.096057, -0.021062, 0.993527, 0.102389, 0.021055, 0.977170, 0.206845, 0.062303, 0.977170, 0.198377, 0.101001, 0.977170, 0.181787, 0.135563, 0.977170, 0.157755, 0.164576, 0.977170, 0.127264, 0.186851, 0.977170, 0.091563, 0.201476, 0.977170, 0.052114, 0.207852, 0.977170, 0.010531, 0.205720, 0.977170, -0.031484, 0.195165, 0.977170, -0.072209, 0.176619, 0.977170, -0.109978, 0.150844, 0.977170, -0.143244, 0.118892, 0.977170, -0.170646, 0.082073, 0.977170, -0.191062, 0.041894, 0.977170, -0.203656, -0, 0.977170, -0.207912, -0.041894, 0.977170, -0.203656, -0.082073, 0.977170, -0.191062, -0.118892, 0.977170, -0.170646, -0.150844, 0.977170, -0.143244, -0.176620, 0.977170, -0.109978, -0.195165, 0.977170, -0.072209, -0.205720, 0.977170, -0.031484, -0.207852, 0.977170, 0.010531, -0.201476, 0.977170, 0.052114, -0.186851, 0.977170, 0.091563, -0.164576, 0.977170, 0.127264, -0.135563, 0.977170, 0.157755, -0.101001, 0.977170, 0.181787, -0.062303, 0.977170, 0.198377, -0.021055, 0.977170, 0.206845, 0, 0.950105, 0.309017, 0.062267, 0.950105, 0.302691, 0.121985, 0.950105, 0.283974, 0.176708, 0.950105, 0.253630, 0.224197, 0.950105, 0.212902, 0.262508, 0.950105, 0.163459, 0.290071, 0.950105, 0.107323, 0.305759, 0.950105, 0.046794, 0.308929, 0.950105, -0.015651, 0.299451, 0.950105, -0.077456, 0.277714, 0.950105, -0.136089, 0.244607, 0.950105, -0.189151, 0.201486, 0.950105, -0.234469, 0.150117, 0.950105, -0.270188, 0.092601, 0.950105, -0.294845, 0.031294, 0.950105, -0.307432, -0.031294, 0.950105, -0.307432, -0.092601, 0.950105, -0.294845, -0.150117, 0.950105, -0.270188, -0.201486, 0.950105, -0.234469, -0.244608, 0.950105, -0.189151, -0.277714, 0.950105, -0.136089, -0.299451, 0.950105, -0.077456, -0.308929, 0.950105, -0.015651, -0.305759, 0.950105, 0.046794, -0.290071, 0.950105, 0.107323, -0.262508, 0.950105, 0.163459, -0.224197, 0.950105, 0.212902, -0.176708, 0.950105, 0.253630, -0.121985, 0.950105, 0.283974, -0.062267, 0.950105, 0.302691, 0.041190, 0.912632, 0.404650, 0.121884, 0.912632, 0.388083, 0.197587, 0.912632, 0.355629, 0.265202, 0.912632, 0.308615, 0.321959, 0.912632, 0.248966, 0.365535, 0.912632, 0.179124, 0.394146, 0.912632, 0.101950, 0.406621, 0.912632, 0.020601, 0.402448, 0.912632, -0.061591, 0.381800, 0.912632, -0.141262, 0.345520, 0.912632, -0.215149, 0.295095, 0.912632, -0.280228, 0.232588, 0.912632, -0.333835, 0.160559, 0.912632, -0.373774, 0.081957, 0.912632, -0.398411, -0, 0.912632, -0.406737, -0.081957, 0.912632, -0.398411, -0.160559, 0.912632, -0.373774, -0.232588, 0.912632, -0.333835, -0.295095, 0.912632, -0.280228, -0.345520, 0.912632, -0.215149, -0.381800, 0.912632, -0.141262, -0.402448, 0.912632, -0.061591, -0.406621, 0.912632, 0.020601, -0.394146, 0.912632, 0.101950, -0.365535, 0.912632, 0.179124, -0.321959, 0.912632, 0.248966, -0.265202, 0.912632, 0.308615, -0.197587, 0.912632, 0.355629, -0.121884, 0.912632, 0.388083, -0.041190, 0.912632, 0.404650, 0, 0.865159, 0.500000, 0.100750, 0.865159, 0.489765, 0.197375, 0.865159, 0.459479, 0.285920, 0.865159, 0.410382, 0.362759, 0.865159, 0.344483, 0.424746, 0.865159, 0.264482, 0.469345, 0.865159, 0.173653, 0.494728, 0.865159, 0.075714, 0.499858, 0.865159, -0.025325, 0.484523, 0.865159, -0.125326, 0.449351, 0.865159, -0.220197, 0.395783, 0.865159, -0.306053, 0.326012, 0.865159, -0.379379, 0.242894, 0.865159, -0.437173, 0.149831, 0.865159, -0.477070, 0.050635, 0.865159, -0.497435, -0.050635, 0.865159, -0.497435, -0.149831, 0.865159, -0.477070, -0.242894, 0.865159, -0.437173, -0.326012, 0.865159, -0.379379, -0.395783, 0.865159, -0.306053, -0.449351, 0.865159, -0.220197, -0.484523, 0.865159, -0.125326, -0.499858, 0.865159, -0.025324, -0.494728, 0.865159, 0.075714, -0.469345, 0.865159, 0.173653, -0.424747, 0.865159, 0.264482, -0.362759, 0.865159, 0.344483, -0.285920, 0.865159, 0.410382, -0.197375, 0.865159, 0.459479, -0.100750, 0.865159, 0.489765, 0.059525, 0.808208, 0.584770, 0.176137, 0.808208, 0.560829, 0.285539, 0.808208, 0.513928, 0.383250, 0.808208, 0.445987, 0.465271, 0.808208, 0.359787, 0.528244, 0.808208, 0.258857, 0.569591, 0.808208, 0.147330, 0.587618, 0.808208, 0.029771, 0.581588, 0.808208, -0.089007, 0.551748, 0.808208, -0.204141, 0.499319, 0.808208, -0.310917, 0.426449, 0.808208, -0.404965, 0.336119, 0.808208, -0.482433, 0.232028, 0.808208, -0.540150, 0.118439, 0.808208, -0.575753, -0, 0.808208, -0.587785, -0.118439, 0.808208, -0.575753, -0.232028, 0.808208, -0.540150, -0.336119, 0.808208, -0.482433, -0.426449, 0.808208, -0.404965, -0.499319, 0.808208, -0.310917, -0.551748, 0.808208, -0.204141, -0.581588, 0.808208, -0.089007, -0.587618, 0.808208, 0.029771, -0.569591, 0.808208, 0.147330, -0.528244, 0.808208, 0.258857, -0.465271, 0.808208, 0.359787, -0.383250, 0.808208, 0.445987, -0.285539, 0.808208, 0.513928, -0.176137, 0.808208, 0.560829, -0.059525, 0.808208, 0.584770, 0, 0.742402, 0.669131, 0.134830, 0.742402, 0.655433, 0.264139, 0.742402, 0.614903, 0.382635, 0.742402, 0.549198, 0.485466, 0.742402, 0.461009, 0.568422, 0.742402, 0.353946, 0.628106, 0.742402, 0.232393, 0.662076, 0.742402, 0.101325, 0.668940, 0.742402, -0.033891, 0.648418, 0.742402, -0.167719, 0.601349, 0.742402, -0.294681, 0.529661, 0.742402, -0.409579, 0.436289, 0.742402, -0.507708, 0.325055, 0.742402, -0.585052, 0.200513, 0.742402, -0.638444, 0.067762, 0.742402, -0.665698, -0.067763, 0.742402, -0.665698, -0.200513, 0.742402, -0.638444, -0.325055, 0.742402, -0.585052, -0.436289, 0.742402, -0.507708, -0.529661, 0.742402, -0.409579, -0.601349, 0.742402, -0.294681, -0.648418, 0.742402, -0.167719, -0.668940, 0.742402, -0.033891, -0.662076, 0.742402, 0.101325, -0.628106, 0.742402, 0.232392, -0.568422, 0.742402, 0.353946, -0.485466, 0.742402, 0.461009, -0.382635, 0.742402, 0.549198, -0.264139, 0.742402, 0.614903, -0.134830, 0.742402, 0.655433, 0.075258, 0.668461, 0.739332, 0.222693, 0.668461, 0.709064, 0.361010, 0.668461, 0.649766, 0.484548, 0.668461, 0.563867, 0.588249, 0.668461, 0.454883, 0.667866, 0.668461, 0.327277, 0.720141, 0.668461, 0.186271, 0.742933, 0.668461, 0.037640, 0.735310, 0.668461, -0.112533, 0.697583, 0.668461, -0.258098, 0.631296, 0.668461, -0.393097, 0.539165, 0.668461, -0.512002, 0.424960, 0.668461, -0.609946, 0.293357, 0.668461, -0.682919, 0.149744, 0.668461, -0.727933, -0, 0.668461, -0.743145, -0.149743, 0.668461, -0.727933, -0.293357, 0.668461, -0.682919, -0.424960, 0.668461, -0.609946, -0.539165, 0.668461, -0.512002, -0.631296, 0.668461, -0.393097, -0.697583, 0.668461, -0.258098, -0.735310, 0.668461, -0.112533, -0.742933, 0.668461, 0.037640, -0.720141, 0.668461, 0.186271, -0.667866, 0.668461, 0.327277, -0.588249, 0.668461, 0.454883, -0.484548, 0.668461, 0.563867, -0.361010, 0.668461, 0.649766, -0.222693, 0.668461, 0.709064, -0.075258, 0.668461, 0.739332, 0, 0.587197, 0.809017, 0.163017, 0.587197, 0.792456, 0.319360, 0.587197, 0.743452, 0.462628, 0.587197, 0.664012, 0.586956, 0.587197, 0.557386, 0.687254, 0.587197, 0.427941, 0.759416, 0.587197, 0.280976, 0.800487, 0.587197, 0.122508, 0.808787, 0.587197, -0.040976, 0.783974, 0.587197, -0.202782, 0.727066, 0.587197, -0.356286, 0.640391, 0.587197, -0.495204, 0.527498, 0.587197, -0.613848, 0.393010, 0.587197, -0.707361, 0.242432, 0.587197, -0.771915, 0.081929, 0.587197, -0.804866, -0.081929, 0.587197, -0.804866, -0.242432, 0.587197, -0.771915, -0.393010, 0.587197, -0.707361, -0.527498, 0.587197, -0.613848, -0.640391, 0.587197, -0.495204, -0.727066, 0.587197, -0.356286, -0.783974, 0.587197, -0.202782, -0.808787, 0.587197, -0.040976, -0.800487, 0.587197, 0.122507, -0.759416, 0.587197, 0.280976, -0.687254, 0.587197, 0.427941, -0.586956, 0.587197, 0.557386, -0.462628, 0.587197, 0.664012, -0.319360, 0.587197, 0.743452, -0.163017, 0.587197, 0.792456, 0.087702, 0.499500, 0.861582, 0.259515, 0.499500, 0.826309, 0.420704, 0.499500, 0.757206, 0.564669, 0.499500, 0.657104, 0.685517, 0.499500, 0.530099, 0.778299, 0.499500, 0.381393, 0.839218, 0.499500, 0.217071, 0.865779, 0.499500, 0.043863, 0.856895, 0.499500, -0.131140, 0.812929, 0.499500, -0.300775, 0.735682, 0.499500, -0.458096, 0.628317, 0.499500, -0.596663, 0.495228, 0.499500, -0.710802, 0.341864, 0.499500, -0.795841, 0.174504, 0.499500, -0.848298, -0, 0.499500, -0.866025, -0.174504, 0.499500, -0.848298, -0.341864, 0.499500, -0.795841, -0.495228, 0.499500, -0.710802, -0.628317, 0.499500, -0.596663, -0.735683, 0.499500, -0.458096, -0.812929, 0.499500, -0.300775, -0.856895, 0.499500, -0.131140, -0.865779, 0.499500, 0.043863, -0.839218, 0.499500, 0.217071, -0.778299, 0.499500, 0.381392, -0.685517, 0.499500, 0.530099, -0.564669, 0.499500, 0.657104, -0.420704, 0.499500, 0.757206, -0.259515, 0.499500, 0.826309, -0.087702, 0.499500, 0.861582, 0, 0.406330, 0.913545, 0.184079, 0.406330, 0.894845, 0.360622, 0.406330, 0.839510, 0.522401, 0.406330, 0.749805, 0.662793, 0.406330, 0.629403, 0.776050, 0.406330, 0.483233, 0.857536, 0.406330, 0.317279, 0.903914, 0.406330, 0.138336, 0.913285, 0.406330, -0.046270, 0.885267, 0.406330, -0.228982, 0.821005, 0.406330, -0.402320, 0.723132, 0.406330, -0.559187, 0.595654, 0.406330, -0.693160, 0.443789, 0.406330, -0.798755, 0.273755, 0.406330, -0.871650, 0.092514, 0.406330, -0.908858, -0.092514, 0.406330, -0.908858, -0.273755, 0.406330, -0.871650, -0.443789, 0.406330, -0.798755, -0.595653, 0.406330, -0.693160, -0.723132, 0.406330, -0.559187, -0.821006, 0.406330, -0.402320, -0.885267, 0.406330, -0.228982, -0.913285, 0.406330, -0.046270, -0.903914, 0.406330, 0.138336, -0.857536, 0.406330, 0.317279, -0.776050, 0.406330, 0.483233, -0.662793, 0.406330, 0.629403, -0.522401, 0.406330, 0.749805, -0.360622, 0.406330, 0.839510, -0.184079, 0.406330, 0.894845, 0.096313, 0.308708, 0.946177, 0.284996, 0.308708, 0.907440, 0.462011, 0.308708, 0.831553, 0.620112, 0.308708, 0.721622, 0.752825, 0.308708, 0.582147, 0.854717, 0.308708, 0.418840, 0.921617, 0.308708, 0.238385, 0.950786, 0.308708, 0.048170, 0.941029, 0.308708, -0.144016, 0.892747, 0.308708, -0.330307, 0.807916, 0.308708, -0.503075, 0.690008, 0.308708, -0.655247, 0.543852, 0.308708, -0.780592, 0.375430, 0.308708, -0.873981, 0.191638, 0.308708, -0.931588, -0, 0.308708, -0.951057, -0.191638, 0.308708, -0.931588, -0.375430, 0.308708, -0.873981, -0.543852, 0.308708, -0.780592, -0.690008, 0.308708, -0.655246, -0.807916, 0.308708, -0.503075, -0.892747, 0.308708, -0.330307, -0.941029, 0.308708, -0.144016, -0.950786, 0.308708, 0.048170, -0.921617, 0.308708, 0.238385, -0.854717, 0.308708, 0.418840, -0.752825, 0.308708, 0.582147, -0.620112, 0.308708, 0.721622, -0.462011, 0.308708, 0.831553, -0.284996, 0.308708, 0.907440, -0.096313, 0.308708, 0.946177, 0, 0.207704, 0.978148, 0.197097, 0.207704, 0.958125, 0.386124, 0.207704, 0.898876, 0.559343, 0.207704, 0.802828, 0.709663, 0.207704, 0.673911, 0.830929, 0.207704, 0.517405, 0.918177, 0.207704, 0.339716, 0.967835, 0.207704, 0.148119, 0.977869, 0.207704, -0.049542, 0.947869, 0.207704, -0.245175, 0.879064, 0.207704, -0.430771, 0.774269, 0.207704, -0.598730, 0.637776, 0.207704, -0.742177, 0.475172, 0.207704, -0.855240, 0.293114, 0.207704, -0.933289, 0.099056, 0.207704, -0.973129, -0.099057, 0.207704, -0.973129, -0.293114, 0.207704, -0.933289, -0.475172, 0.207704, -0.855240, -0.637776, 0.207704, -0.742177, -0.774269, 0.207704, -0.598730, -0.879064, 0.207704, -0.430770, -0.947869, 0.207704, -0.245175, -0.977869, 0.207704, -0.049542, -0.967835, 0.207704, 0.148119, -0.918177, 0.207704, 0.339716, -0.830930, 0.207704, 0.517405, -0.709663, 0.207704, 0.673911, -0.559343, 0.207704, 0.802828, -0.386124, 0.207704, 0.898876, -0.197096, 0.207704, 0.958125, 0.100715, 0.104424, 0.989419, 0.298021, 0.104424, 0.948912, 0.483126, 0.104424, 0.869557, 0.648452, 0.104424, 0.754602, 0.787230, 0.104424, 0.608753, 0.893779, 0.104424, 0.437982, 0.963737, 0.104424, 0.249279, 0.994239, 0.104424, 0.050372, 0.984037, 0.104424, -0.150598, 0.933548, 0.104424, -0.345403, 0.844839, 0.104424, -0.526066, 0.721543, 0.104424, -0.685193, 0.568707, 0.104424, -0.816267, 0.392588, 0.104424, -0.913924, 0.200396, 0.104424, -0.974164, -0, 0.104424, -0.994522, -0.200396, 0.104424, -0.974164, -0.392588, 0.104424, -0.913924, -0.568707, 0.104424, -0.816267, -0.721543, 0.104424, -0.685193, -0.844839, 0.104424, -0.526066, -0.933548, 0.104424, -0.345403, -0.984037, 0.104424, -0.150598, -0.994239, 0.104424, 0.050371, -0.963737, 0.104424, 0.249279, -0.893779, 0.104424, 0.437982, -0.787230, 0.104424, 0.608753, -0.648452, 0.104424, 0.754602, -0.483126, 0.104424, 0.869557, -0.298021, 0.104424, 0.948912, -0.100715, 0.104424, 0.989419, 0, -0, 1, 0.201500, -0, 0.979530, 0.394750, -0, 0.918958, 0.571840, -0, 0.820763, 0.725518, -0, 0.688967, 0.849493, -0, 0.528964, 0.938690, -0, 0.347305, 0.989457, -0, 0.151428, 0.999715, -0, -0.050649, 0.969045, -0, -0.250653, 0.898702, -0, -0.440394, 0.791566, -0, -0.612106, 0.652024, -0, -0.758758, 0.485787, -0, -0.874347, 0.299662, -0, -0.954139, 0.101269, -0, -0.994869, -0.101270, -0, -0.994869, -0.299662, -0, -0.954139, -0.485787, -0, -0.874347, -0.652024, -0, -0.758758, -0.791567, -0, -0.612106, -0.898702, -0, -0.440394, -0.969045, -0, -0.250652, -0.999715, -0, -0.050649, -0.989457, -0, 0.151428, -0.938690, -0, 0.347305, -0.849493, -0, 0.528964, -0.725518, -0, 0.688967, -0.571840, -0, 0.820763, -0.394750, -0, 0.918958, -0.201500, -0, 0.979530, 0.100715, -0.104424, 0.989419, 0.298021, -0.104424, 0.948912, 0.483126, -0.104424, 0.869557, 0.648452, -0.104424, 0.754602, 0.787230, -0.104424, 0.608753, 0.893779, -0.104424, 0.437982, 0.963737, -0.104424, 0.249279, 0.994239, -0.104424, 0.050372, 0.984037, -0.104424, -0.150598, 0.933548, -0.104424, -0.345403, 0.844839, -0.104424, -0.526066, 0.721543, -0.104424, -0.685193, 0.568707, -0.104424, -0.816267, 0.392588, -0.104424, -0.913924, 0.200396, -0.104424, -0.974164, -0, -0.104424, -0.994522, -0.200396, -0.104424, -0.974164, -0.392588, -0.104424, -0.913924, -0.568707, -0.104424, -0.816267, -0.721543, -0.104424, -0.685193, -0.844839, -0.104424, -0.526066, -0.933548, -0.104424, -0.345403, -0.984037, -0.104424, -0.150598, -0.994239, -0.104424, 0.050371, -0.963737, -0.104424, 0.249279, -0.893779, -0.104424, 0.437982, -0.787230, -0.104424, 0.608753, -0.648452, -0.104424, 0.754602, -0.483126, -0.104424, 0.869557, -0.298021, -0.104424, 0.948912, -0.100715, -0.104424, 0.989419, 0, -0.207704, 0.978148, 0.197097, -0.207704, 0.958125, 0.386124, -0.207704, 0.898876, 0.559343, -0.207704, 0.802828, 0.709663, -0.207704, 0.673911, 0.830929, -0.207704, 0.517405, 0.918177, -0.207704, 0.339716, 0.967835, -0.207704, 0.148119, 0.977869, -0.207704, -0.049542, 0.947869, -0.207704, -0.245175, 0.879063, -0.207704, -0.430771, 0.774269, -0.207704, -0.598730, 0.637776, -0.207704, -0.742177, 0.475172, -0.207704, -0.855240, 0.293114, -0.207704, -0.933289, 0.099056, -0.207704, -0.973129, -0.099057, -0.207704, -0.973129, -0.293114, -0.207704, -0.933289, -0.475172, -0.207704, -0.855240, -0.637776, -0.207704, -0.742177, -0.774269, -0.207704, -0.598730, -0.879064, -0.207704, -0.430770, -0.947869, -0.207704, -0.245175, -0.977869, -0.207704, -0.049542, -0.967835, -0.207704, 0.148119, -0.918177, -0.207704, 0.339716, -0.830930, -0.207704, 0.517405, -0.709663, -0.207704, 0.673911, -0.559343, -0.207704, 0.802828, -0.386124, -0.207704, 0.898876, -0.197096, -0.207704, 0.958125, 0.096313, -0.308708, 0.946177, 0.284996, -0.308708, 0.907440, 0.462011, -0.308708, 0.831553, 0.620112, -0.308708, 0.721622, 0.752824, -0.308708, 0.582147, 0.854717, -0.308708, 0.418840, 0.921617, -0.308708, 0.238385, 0.950786, -0.308708, 0.048170, 0.941029, -0.308708, -0.144016, 0.892747, -0.308708, -0.330307, 0.807916, -0.308708, -0.503075, 0.690008, -0.308708, -0.655246, 0.543852, -0.308708, -0.780592, 0.375430, -0.308708, -0.873981, 0.191638, -0.308708, -0.931588, -0, -0.308708, -0.951056, -0.191638, -0.308708, -0.931588, -0.375430, -0.308708, -0.873981, -0.543852, -0.308708, -0.780592, -0.690008, -0.308708, -0.655246, -0.807916, -0.308708, -0.503075, -0.892747, -0.308708, -0.330307, -0.941029, -0.308708, -0.144016, -0.950786, -0.308708, 0.048170, -0.921617, -0.308708, 0.238385, -0.854717, -0.308708, 0.418840, -0.752825, -0.308708, 0.582147, -0.620112, -0.308708, 0.721622, -0.462011, -0.308708, 0.831553, -0.284996, -0.308708, 0.907440, -0.096313, -0.308708, 0.946177, 0, -0.406330, 0.913545, 0.184079, -0.406330, 0.894845, 0.360622, -0.406330, 0.839510, 0.522401, -0.406330, 0.749805, 0.662793, -0.406330, 0.629403, 0.776050, -0.406330, 0.483233, 0.857536, -0.406330, 0.317279, 0.903914, -0.406330, 0.138336, 0.913285, -0.406330, -0.046270, 0.885267, -0.406330, -0.228982, 0.821005, -0.406330, -0.402320, 0.723132, -0.406330, -0.559187, 0.595653, -0.406330, -0.693160, 0.443789, -0.406330, -0.798755, 0.273755, -0.406330, -0.871650, 0.092514, -0.406330, -0.908858, -0.092514, -0.406330, -0.908858, -0.273755, -0.406330, -0.871650, -0.443789, -0.406330, -0.798755, -0.595653, -0.406330, -0.693160, -0.723132, -0.406330, -0.559187, -0.821006, -0.406330, -0.402320, -0.885267, -0.406330, -0.228982, -0.913285, -0.406330, -0.046270, -0.903914, -0.406330, 0.138336, -0.857536, -0.406330, 0.317279, -0.776050, -0.406330, 0.483233, -0.662793, -0.406330, 0.629403, -0.522401, -0.406330, 0.749805, -0.360622, -0.406330, 0.839510, -0.184079, -0.406330, 0.894845, 0.087702, -0.499500, 0.861582, 0.259515, -0.499500, 0.826309, 0.420704, -0.499500, 0.757206, 0.564669, -0.499500, 0.657104, 0.685517, -0.499500, 0.530099, 0.778299, -0.499500, 0.381392, 0.839218, -0.499500, 0.217071, 0.865779, -0.499500, 0.043863, 0.856895, -0.499500, -0.131140, 0.812929, -0.499500, -0.300775, 0.735682, -0.499500, -0.458096, 0.628317, -0.499500, -0.596663, 0.495228, -0.499500, -0.710802, 0.341864, -0.499500, -0.795841, 0.174504, -0.499500, -0.848298, -0, -0.499500, -0.866025, -0.174504, -0.499500, -0.848298, -0.341864, -0.499500, -0.795841, -0.495228, -0.499500, -0.710802, -0.628317, -0.499500, -0.596663, -0.735682, -0.499500, -0.458096, -0.812929, -0.499500, -0.300775, -0.856895, -0.499500, -0.131140, -0.865779, -0.499500, 0.043863, -0.839218, -0.499500, 0.217071, -0.778299, -0.499500, 0.381392, -0.685517, -0.499500, 0.530099, -0.564669, -0.499500, 0.657104, -0.420704, -0.499500, 0.757206, -0.259515, -0.499500, 0.826309, -0.087702, -0.499500, 0.861582, 0, -0.587197, 0.809017, 0.163017, -0.587197, 0.792456, 0.319360, -0.587197, 0.743452, 0.462628, -0.587197, 0.664012, 0.586956, -0.587197, 0.557386, 0.687254, -0.587197, 0.427941, 0.759416, -0.587197, 0.280976, 0.800487, -0.587197, 0.122508, 0.808787, -0.587197, -0.040976, 0.783974, -0.587197, -0.202782, 0.727066, -0.587197, -0.356286, 0.640391, -0.587197, -0.495204, 0.527498, -0.587197, -0.613848, 0.393010, -0.587197, -0.707361, 0.242432, -0.587197, -0.771915, 0.081929, -0.587197, -0.804866, -0.081929, -0.587197, -0.804866, -0.242432, -0.587197, -0.771915, -0.393010, -0.587197, -0.707361, -0.527498, -0.587197, -0.613848, -0.640391, -0.587197, -0.495204, -0.727066, -0.587197, -0.356286, -0.783974, -0.587197, -0.202782, -0.808787, -0.587197, -0.040976, -0.800487, -0.587197, 0.122507, -0.759416, -0.587197, 0.280976, -0.687254, -0.587197, 0.427941, -0.586956, -0.587197, 0.557386, -0.462628, -0.587197, 0.664012, -0.319360, -0.587197, 0.743452, -0.163017, -0.587197, 0.792456, 0.075258, -0.668462, 0.739332, 0.222693, -0.668462, 0.709064, 0.361010, -0.668462, 0.649766, 0.484548, -0.668462, 0.563867, 0.588249, -0.668462, 0.454883, 0.667866, -0.668462, 0.327277, 0.720141, -0.668462, 0.186271, 0.742933, -0.668462, 0.037640, 0.735310, -0.668462, -0.112533, 0.697583, -0.668462, -0.258098, 0.631296, -0.668462, -0.393097, 0.539165, -0.668462, -0.512002, 0.424960, -0.668462, -0.609946, 0.293357, -0.668462, -0.682919, 0.149744, -0.668462, -0.727933, -0, -0.668462, -0.743145, -0.149743, -0.668462, -0.727933, -0.293357, -0.668462, -0.682919, -0.424960, -0.668462, -0.609946, -0.539165, -0.668462, -0.512002, -0.631296, -0.668462, -0.393097, -0.697583, -0.668462, -0.258098, -0.735310, -0.668462, -0.112533, -0.742933, -0.668462, 0.037640, -0.720141, -0.668462, 0.186271, -0.667866, -0.668462, 0.327277, -0.588249, -0.668462, 0.454883, -0.484548, -0.668462, 0.563867, -0.361010, -0.668462, 0.649766, -0.222693, -0.668462, 0.709064, -0.075258, -0.668462, 0.739332, 0, -0.742402, 0.669131, 0.134830, -0.742402, 0.655433, 0.264139, -0.742402, 0.614903, 0.382635, -0.742402, 0.549198, 0.485466, -0.742402, 0.461009, 0.568422, -0.742402, 0.353946, 0.628106, -0.742402, 0.232393, 0.662076, -0.742402, 0.101325, 0.668940, -0.742402, -0.033891, 0.648418, -0.742402, -0.167719, 0.601349, -0.742402, -0.294681, 0.529661, -0.742402, -0.409579, 0.436289, -0.742402, -0.507708, 0.325055, -0.742402, -0.585052, 0.200513, -0.742402, -0.638444, 0.067762, -0.742402, -0.665698, -0.067763, -0.742402, -0.665698, -0.200513, -0.742402, -0.638444, -0.325055, -0.742402, -0.585052, -0.436289, -0.742402, -0.507708, -0.529661, -0.742402, -0.409579, -0.601349, -0.742402, -0.294681, -0.648418, -0.742402, -0.167719, -0.668940, -0.742402, -0.033891, -0.662076, -0.742402, 0.101325, -0.628106, -0.742402, 0.232392, -0.568422, -0.742402, 0.353946, -0.485466, -0.742402, 0.461009, -0.382635, -0.742402, 0.549198, -0.264139, -0.742402, 0.614903, -0.134830, -0.742402, 0.655433, 0.059525, -0.808208, 0.584769, 0.176137, -0.808208, 0.560829, 0.285539, -0.808208, 0.513928, 0.383250, -0.808208, 0.445987, 0.465271, -0.808208, 0.359787, 0.528244, -0.808208, 0.258857, 0.569590, -0.808208, 0.147330, 0.587618, -0.808208, 0.029771, 0.581588, -0.808208, -0.089007, 0.551748, -0.808208, -0.204141, 0.499319, -0.808208, -0.310917, 0.426448, -0.808208, -0.404965, 0.336119, -0.808208, -0.482433, 0.232028, -0.808208, -0.540150, 0.118439, -0.808208, -0.575753, -0, -0.808208, -0.587785, -0.118439, -0.808208, -0.575753, -0.232028, -0.808208, -0.540150, -0.336119, -0.808208, -0.482433, -0.426449, -0.808208, -0.404965, -0.499319, -0.808208, -0.310917, -0.551748, -0.808208, -0.204141, -0.581588, -0.808208, -0.089007, -0.587618, -0.808208, 0.029771, -0.569591, -0.808208, 0.147330, -0.528244, -0.808208, 0.258857, -0.465271, -0.808208, 0.359787, -0.383250, -0.808208, 0.445987, -0.285539, -0.808208, 0.513928, -0.176137, -0.808208, 0.560829, -0.059525, -0.808208, 0.584769, 0, -0.865159, 0.500000, 0.100750, -0.865159, 0.489765, 0.197375, -0.865159, 0.459479, 0.285920, -0.865159, 0.410382, 0.362759, -0.865159, 0.344483, 0.424747, -0.865159, 0.264482, 0.469345, -0.865159, 0.173653, 0.494728, -0.865159, 0.075714, 0.499858, -0.865159, -0.025325, 0.484523, -0.865159, -0.125326, 0.449351, -0.865159, -0.220197, 0.395783, -0.865159, -0.306053, 0.326012, -0.865159, -0.379379, 0.242894, -0.865159, -0.437173, 0.149831, -0.865159, -0.477070, 0.050635, -0.865159, -0.497435, -0.050635, -0.865159, -0.497435, -0.149831, -0.865159, -0.477070, -0.242894, -0.865159, -0.437173, -0.326012, -0.865159, -0.379379, -0.395783, -0.865159, -0.306053, -0.449351, -0.865159, -0.220197, -0.484523, -0.865159, -0.125326, -0.499858, -0.865159, -0.025324, -0.494728, -0.865159, 0.075714, -0.469345, -0.865159, 0.173653, -0.424747, -0.865159, 0.264482, -0.362759, -0.865159, 0.344483, -0.285920, -0.865159, 0.410382, -0.197375, -0.865159, 0.459479, -0.100750, -0.865159, 0.489765, 0.041190, -0.912632, 0.404650, 0.121884, -0.912632, 0.388083, 0.197587, -0.912632, 0.355629, 0.265202, -0.912632, 0.308615, 0.321959, -0.912632, 0.248966, 0.365535, -0.912632, 0.179124, 0.394146, -0.912632, 0.101950, 0.406621, -0.912632, 0.020601, 0.402448, -0.912632, -0.061591, 0.381800, -0.912632, -0.141262, 0.345520, -0.912632, -0.215149, 0.295095, -0.912632, -0.280228, 0.232588, -0.912632, -0.333834, 0.160559, -0.912632, -0.373774, 0.081957, -0.912632, -0.398411, -0, -0.912632, -0.406737, -0.081957, -0.912632, -0.398411, -0.160559, -0.912632, -0.373774, -0.232588, -0.912632, -0.333834, -0.295095, -0.912632, -0.280228, -0.345520, -0.912632, -0.215149, -0.381800, -0.912632, -0.141262, -0.402448, -0.912632, -0.061591, -0.406621, -0.912632, 0.020601, -0.394146, -0.912632, 0.101949, -0.365535, -0.912632, 0.179124, -0.321959, -0.912632, 0.248966, -0.265202, -0.912632, 0.308615, -0.197587, -0.912632, 0.355629, -0.121884, -0.912632, 0.388083, -0.041190, -0.912632, 0.404650, 0, -0.950105, 0.309017, 0.062267, -0.950105, 0.302691, 0.121985, -0.950105, 0.283974, 0.176708, -0.950105, 0.253630, 0.224197, -0.950105, 0.212903, 0.262508, -0.950105, 0.163459, 0.290071, -0.950105, 0.107323, 0.305759, -0.950105, 0.046794, 0.308929, -0.950105, -0.015651, 0.299451, -0.950105, -0.077456, 0.277714, -0.950105, -0.136089, 0.244608, -0.950105, -0.189151, 0.201486, -0.950105, -0.234469, 0.150117, -0.950105, -0.270188, 0.092601, -0.950105, -0.294845, 0.031294, -0.950105, -0.307432, -0.031294, -0.950105, -0.307432, -0.092601, -0.950105, -0.294845, -0.150117, -0.950105, -0.270188, -0.201486, -0.950105, -0.234469, -0.244608, -0.950105, -0.189151, -0.277714, -0.950105, -0.136089, -0.299452, -0.950105, -0.077456, -0.308929, -0.950105, -0.015651, -0.305759, -0.950105, 0.046794, -0.290071, -0.950105, 0.107323, -0.262508, -0.950105, 0.163459, -0.224197, -0.950105, 0.212903, -0.176708, -0.950105, 0.253630, -0.121985, -0.950105, 0.283974, -0.062267, -0.950105, 0.302691, 0.021055, -0.977170, 0.206845, 0.062303, -0.977170, 0.198377, 0.101001, -0.977170, 0.181787, 0.135563, -0.977170, 0.157755, 0.164576, -0.977170, 0.127264, 0.186851, -0.977170, 0.091563, 0.201476, -0.977170, 0.052114, 0.207852, -0.977170, 0.010531, 0.205720, -0.977170, -0.031484, 0.195165, -0.977170, -0.072209, 0.176619, -0.977170, -0.109978, 0.150844, -0.977170, -0.143244, 0.118892, -0.977170, -0.170646, 0.082073, -0.977170, -0.191062, 0.041894, -0.977170, -0.203656, -0, -0.977170, -0.207912, -0.041894, -0.977170, -0.203656, -0.082073, -0.977170, -0.191062, -0.118892, -0.977170, -0.170646, -0.150844, -0.977170, -0.143244, -0.176619, -0.977170, -0.109978, -0.195165, -0.977170, -0.072209, -0.205720, -0.977170, -0.031484, -0.207852, -0.977170, 0.010531, -0.201476, -0.977170, 0.052114, -0.186851, -0.977170, 0.091563, -0.164576, -0.977170, 0.127264, -0.135563, -0.977170, 0.157755, -0.101001, -0.977170, 0.181787, -0.062303, -0.977170, 0.198377, -0.021055, -0.977170, 0.206845, 0, -0.993527, 0.104528, 0.021062, -0.993527, 0.102389, 0.041263, -0.993527, 0.096057, 0.059774, -0.993527, 0.085793, 0.075837, -0.993527, 0.072017, 0.088796, -0.993527, 0.055292, 0.098120, -0.993527, 0.036303, 0.103426, -0.993527, 0.015829, 0.104499, -0.993527, -0.005294, 0.101293, -0.993527, -0.026200, 0.093940, -0.993527, -0.046034, 0.082741, -0.993527, -0.063983, 0.068155, -0.993527, -0.079312, 0.050779, -0.993527, -0.091394, 0.031323, -0.993527, -0.099735, 0.010586, -0.993527, -0.103992, -0.010586, -0.993527, -0.103992, -0.031323, -0.993527, -0.099735, -0.050779, -0.993527, -0.091394, -0.068155, -0.993527, -0.079312, -0.082741, -0.993527, -0.063983, -0.093940, -0.993527, -0.046034, -0.101293, -0.993527, -0.026200, -0.104499, -0.993527, -0.005294, -0.103426, -0.993527, 0.015828, -0.098120, -0.993527, 0.036303, -0.088796, -0.993527, 0.055292, -0.075837, -0.993527, 0.072017, -0.059774, -0.993527, 0.085793, -0.041263, -0.993527, 0.096057, -0.021062, -0.993527, 0.102389, 0, -1, 0 };
    self->Coordinate2.point = m_point;
    self->Coordinate2.point_length = sizeof(m_point) / sizeof(float);
    }
    self->Coordinate2.m_type = 52;
    self->Coordinate2.m_data = NULL;
    X3dNormalInit(&self->Normal2);
    self->Normal2.m_parent = (X3dNode *)&self->Sphere_8;
    {
    static float m_vector[] = { 0, 1, 0, 0, 0.994511, 0.104632, 0.021083, 0.994511, 0.102490, 0.041303, 0.994509, 0.096152, 0.059832, 0.994507, 0.085878, 0.075912, 0.994505, 0.072088, 0.088883, 0.994503, 0.055346, 0.098216, 0.994501, 0.036339, 0.103528, 0.994500, 0.015844, 0.104601, 0.994500, -0.005299, 0.101392, 0.994501, -0.026226, 0.094032, 0.994502, -0.046079, 0.082823, 0.994504, -0.064045, 0.068222, 0.994506, -0.079390, 0.050829, 0.994508, -0.091484, 0.031354, 0.994510, -0.099833, 0.010596, 0.994511, -0.104095, -0.010596, 0.994511, -0.104095, -0.031354, 0.994510, -0.099833, -0.050829, 0.994508, -0.091484, -0.068222, 0.994506, -0.079390, -0.082823, 0.994504, -0.064045, -0.094032, 0.994502, -0.046079, -0.101392, 0.994501, -0.026226, -0.104601, 0.994500, -0.005299, -0.103528, 0.994500, 0.015844, -0.098216, 0.994501, 0.036339, -0.088883, 0.994503, 0.055346, -0.075912, 0.994505, 0.072088, -0.059832, 0.994507, 0.085878, -0.041303, 0.994509, 0.096152, -0.021083, 0.994511, 0.102490, 0.021075, 0.978105, 0.207043, 0.062363, 0.978101, 0.198566, 0.101097, 0.978095, 0.181959, 0.135691, 0.978087, 0.157903, 0.164729, 0.978079, 0.127382, 0.187023, 0.978071, 0.091648, 0.201661, 0.978066, 0.052161, 0.208043, 0.978063, 0.010540, 0.205908, 0.978064, -0.031512, 0.195344, 0.978068, -0.072275, 0.176783, 0.978075, -0.110080, 0.150985, 0.978083, -0.143378, 0.119004, 0.978091, -0.170807, 0.082151, 0.978099, -0.191244, 0.041934, 0.978104, -0.203850, -0, 0.978105, -0.208111, -0.041934, 0.978104, -0.203850, -0.082151, 0.978099, -0.191244, -0.119004, 0.978091, -0.170807, -0.150985, 0.978083, -0.143378, -0.176783, 0.978075, -0.110080, -0.195344, 0.978068, -0.072275, -0.205908, 0.978064, -0.031512, -0.208043, 0.978063, 0.010540, -0.201661, 0.978066, 0.052161, -0.187023, 0.978071, 0.091648, -0.164729, 0.978079, 0.127382, -0.135691, 0.978087, 0.157903, -0.101097, 0.978095, 0.181959, -0.062363, 0.978101, 0.198566, -0.021075, 0.978105, 0.207043, 0, 0.950966, 0.309297, 0.062323, 0.950962, 0.302964, 0.122093, 0.950951, 0.284226, 0.176863, 0.950936, 0.253852, 0.224389, 0.950918, 0.213084, 0.262727, 0.950900, 0.163596, 0.290309, 0.950886, 0.107411, 0.306007, 0.950877, 0.046832, 0.309179, 0.950875, -0.015664, 0.299696, 0.950880, -0.077519, 0.277944, 0.950892, -0.136202, 0.244814, 0.950909, -0.189311, 0.201661, 0.950927, -0.234672, 0.150249, 0.950944, -0.270426, 0.092684, 0.950957, -0.295110, 0.031322, 0.950965, -0.307710, -0.031322, 0.950965, -0.307710, -0.092684, 0.950957, -0.295110, -0.150249, 0.950944, -0.270426, -0.201661, 0.950927, -0.234672, -0.244814, 0.950909, -0.189311, -0.277944, 0.950892, -0.136202, -0.299696, 0.950880, -0.077519, -0.309179, 0.950875, -0.015664, -0.306007, 0.950877, 0.046832, -0.290309, 0.950886, 0.107411, -0.262727, 0.950900, 0.163596, -0.224389, 0.950918, 0.213084, -0.176863, 0.950936, 0.253852, -0.122093, 0.950951, 0.284226, -0.062323, 0.950962, 0.302964, 0.041224, 0.913393, 0.404987, 0.121984, 0.913381, 0.388402, 0.197745, 0.913358, 0.355912, 0.265405, 0.913330, 0.308851, 0.322195, 0.913299, 0.249148, 0.365792, 0.913272, 0.179250, 0.394414, 0.913252, 0.102019, 0.406893, 0.913243, 0.020615, 0.402719, 0.913246, -0.061633, 0.382063, 0.913261, -0.141359, 0.345767, 0.913285, -0.215303, 0.295315, 0.913315, -0.280438, 0.232770, 0.913345, -0.334095, 0.160689, 0.913371, -0.374076, 0.082025, 0.913388, -0.398741, -0, 0.913394, -0.407076, -0.082025, 0.913388, -0.398741, -0.160689, 0.913371, -0.374076, -0.232770, 0.913345, -0.334095, -0.295315, 0.913315, -0.280438, -0.345767, 0.913285, -0.215303, -0.382063, 0.913261, -0.141359, -0.402719, 0.913246, -0.061633, -0.406893, 0.913243, 0.020615, -0.394414, 0.913252, 0.102019, -0.365792, 0.913272, 0.179250, -0.322195, 0.913299, 0.249148, -0.265405, 0.913330, 0.308851, -0.197745, 0.913358, 0.355912, -0.121984, 0.913381, 0.388402, -0.041224, 0.913393, 0.404987, 0, 0.865809, 0.500375, 0.100825, 0.865800, 0.490128, 0.197516, 0.865775, 0.459806, 0.286111, 0.865738, 0.410656, 0.362983, 0.865695, 0.344697, 0.424989, 0.865653, 0.264633, 0.469594, 0.865618, 0.173745, 0.494979, 0.865597, 0.075752, 0.500108, 0.865592, -0.025337, 0.484772, 0.865605, -0.125391, 0.449598, 0.865634, -0.220318, 0.396018, 0.865673, -0.306235, 0.326222, 0.865717, -0.379623, 0.243062, 0.865758, -0.437476, 0.149940, 0.865789, -0.477417, 0.050673, 0.865806, -0.497807, -0.050673, 0.865806, -0.497807, -0.149940, 0.865789, -0.477417, -0.243062, 0.865758, -0.437476, -0.326222, 0.865717, -0.379623, -0.396018, 0.865673, -0.306235, -0.449598, 0.865634, -0.220318, -0.484772, 0.865605, -0.125391, -0.500108, 0.865592, -0.025337, -0.494979, 0.865597, 0.075752, -0.469594, 0.865618, 0.173745, -0.424989, 0.865653, 0.264633, -0.362983, 0.865695, 0.344697, -0.286111, 0.865738, 0.410656, -0.197516, 0.865775, 0.459806, -0.100824, 0.865800, 0.490128, 0.059563, 0.808734, 0.585150, 0.176247, 0.808712, 0.561179, 0.285702, 0.808671, 0.514223, 0.383445, 0.808618, 0.446213, 0.465475, 0.808562, 0.359945, 0.528443, 0.808512, 0.258954, 0.569779, 0.808475, 0.147379, 0.587800, 0.808458, 0.029780, 0.581772, 0.808464, -0.089035, 0.551941, 0.808491, -0.204212, 0.499522, 0.808536, -0.311043, 0.426650, 0.808590, -0.405156, 0.336301, 0.808646, -0.482694, 0.232168, 0.808694, -0.540474, 0.118515, 0.808726, -0.576122, -0, 0.808737, -0.588170, -0.118514, 0.808726, -0.576122, -0.232168, 0.808694, -0.540474, -0.336301, 0.808646, -0.482694, -0.426650, 0.808590, -0.405156, -0.499522, 0.808536, -0.311043, -0.551941, 0.808491, -0.204212, -0.581772, 0.808464, -0.089035, -0.587800, 0.808458, 0.029780, -0.569779, 0.808475, 0.147378, -0.528443, 0.808512, 0.258954, -0.465475, 0.808562, 0.359945, -0.383445, 0.808618, 0.446213, -0.285702, 0.808671, 0.514223, -0.176247, 0.808712, 0.561179, -0.059563, 0.808734, 0.585150, 0, 0.742812, 0.669500, 0.134902, 0.742798, 0.655784, 0.264267, 0.742760, 0.615200, 0.382791, 0.742703, 0.549421, 0.485620, 0.742637, 0.461155, 0.568552, 0.742572, 0.354027, 0.628205, 0.742519, 0.232429, 0.662152, 0.742486, 0.101337, 0.669010, 0.742480, -0.033895, 0.648504, 0.742500, -0.167741, 0.601464, 0.742543, -0.294738, 0.529805, 0.742604, -0.409690, 0.436447, 0.742670, -0.507892, 0.325200, 0.742733, -0.585313, 0.200616, 0.742782, -0.638771, 0.067800, 0.742808, -0.666062, -0.067800, 0.742808, -0.666062, -0.200616, 0.742782, -0.638771, -0.325200, 0.742733, -0.585313, -0.436447, 0.742670, -0.507892, -0.529806, 0.742604, -0.409690, -0.601464, 0.742543, -0.294737, -0.648504, 0.742500, -0.167741, -0.669010, 0.742480, -0.033894, -0.662152, 0.742486, 0.101336, -0.628206, 0.742519, 0.232429, -0.568552, 0.742572, 0.354027, -0.485620, 0.742637, 0.461155, -0.382791, 0.742703, 0.549421, -0.264267, 0.742760, 0.615200, -0.134902, 0.742798, 0.655784, 0.075291, 0.668757, 0.739659, 0.222781, 0.668728, 0.709346, 0.361125, 0.668674, 0.649972, 0.484651, 0.668604, 0.563987, 0.588309, 0.668530, 0.454930, 0.667867, 0.668463, 0.327277, 0.720090, 0.668414, 0.186258, 0.742856, 0.668392, 0.037636, 0.735242, 0.668400, -0.112522, 0.697556, 0.668436, -0.258088, 0.631328, 0.668495, -0.393116, 0.539249, 0.668567, -0.512083, 0.425073, 0.668640, -0.610109, 0.293463, 0.668703, -0.683166, 0.149807, 0.668746, -0.728242, -0, 0.668761, -0.743478, -0.149807, 0.668746, -0.728242, -0.293463, 0.668703, -0.683166, -0.425073, 0.668640, -0.610109, -0.539250, 0.668567, -0.512083, -0.631328, 0.668495, -0.393116, -0.697556, 0.668436, -0.258088, -0.735242, 0.668400, -0.112522, -0.742856, 0.668392, 0.037636, -0.720090, 0.668414, 0.186258, -0.667867, 0.668463, 0.327277, -0.588309, 0.668530, 0.454930, -0.484651, 0.668604, 0.563987, -0.361125, 0.668674, 0.649973, -0.222781, 0.668728, 0.709346, -0.075291, 0.668757, 0.739659, 0, 0.587400, 0.809296, 0.163069, 0.587385, 0.792709, 0.319437, 0.587340, 0.743634, 0.462689, 0.587275, 0.664099, 0.586957, 0.587198, 0.557387, 0.687167, 0.587123, 0.427887, 0.759241, 0.587062, 0.280911, 0.800252, 0.587025, 0.122472, 0.808538, 0.587017, -0.040963, 0.783764, 0.587040, -0.202728, 0.726933, 0.587090, -0.356221, 0.640350, 0.587160, -0.495172, 0.527534, 0.587237, -0.613890, 0.393085, 0.587310, -0.707496, 0.242502, 0.587366, -0.772136, 0.081956, 0.587396, -0.805139, -0.081957, 0.587396, -0.805139, -0.242502, 0.587366, -0.772136, -0.393085, 0.587310, -0.707496, -0.527534, 0.587237, -0.613890, -0.640350, 0.587160, -0.495172, -0.726933, 0.587090, -0.356221, -0.783764, 0.587040, -0.202728, -0.808538, 0.587017, -0.040963, -0.800252, 0.587025, 0.122471, -0.759241, 0.587062, 0.280911, -0.687167, 0.587123, 0.427887, -0.586957, 0.587198, 0.557387, -0.462689, 0.587275, 0.664099, -0.319437, 0.587340, 0.743634, -0.163069, 0.587385, 0.792709, 0.087723, 0.499621, 0.861791, 0.259563, 0.499591, 0.826460, 0.420735, 0.499537, 0.757262, 0.564631, 0.499466, 0.657059, 0.685366, 0.499390, 0.529983, 0.778023, 0.499323, 0.381257, 0.838838, 0.499274, 0.216973, 0.865347, 0.499251, 0.043842, 0.856481, 0.499259, -0.131077, 0.812596, 0.499295, -0.300652, 0.735469, 0.499355, -0.457963, 0.628226, 0.499428, -0.596577, 0.495230, 0.499502, -0.710806, 0.341909, 0.499566, -0.795947, 0.174542, 0.499610, -0.848484, -0, 0.499625, -0.866242, -0.174542, 0.499610, -0.848484, -0.341909, 0.499566, -0.795947, -0.495230, 0.499502, -0.710806, -0.628226, 0.499428, -0.596577, -0.735469, 0.499355, -0.457963, -0.812596, 0.499295, -0.300652, -0.856481, 0.499259, -0.131077, -0.865347, 0.499251, 0.043841, -0.838838, 0.499274, 0.216973, -0.778023, 0.499323, 0.381257, -0.685366, 0.499390, 0.529983, -0.564631, 0.499466, 0.657059, -0.420735, 0.499537, 0.757262, -0.259563, 0.499591, 0.826460, -0.087723, 0.499621, 0.861791, 0, 0.406397, 0.913697, 0.184103, 0.406383, 0.894963, 0.360635, 0.406344, 0.839540, 0.522345, 0.406286, 0.749724, 0.662612, 0.406219, 0.629231, 0.775712, 0.406153, 0.483022, 0.857048, 0.406099, 0.317099, 0.903326, 0.406066, 0.138246, 0.912676, 0.406059, -0.046240, 0.884721, 0.406079, -0.228841, 0.820589, 0.406124, -0.402116, 0.722874, 0.406185, -0.558987, 0.595541, 0.406253, -0.693029, 0.443775, 0.406317, -0.798730, 0.273780, 0.406367, -0.871728, 0.092529, 0.406394, -0.909001, -0.092529, 0.406394, -0.909001, -0.273780, 0.406367, -0.871729, -0.443775, 0.406317, -0.798730, -0.595541, 0.406253, -0.693029, -0.722874, 0.406185, -0.558987, -0.820589, 0.406124, -0.402116, -0.884721, 0.406079, -0.228841, -0.912676, 0.406059, -0.046239, -0.903326, 0.406066, 0.138246, -0.857049, 0.406099, 0.317099, -0.775712, 0.406153, 0.483022, -0.662612, 0.406219, 0.629231, -0.522345, 0.406286, 0.749724, -0.360635, 0.406344, 0.839540, -0.184103, 0.406383, 0.894963, 0.096321, 0.308735, 0.946258, 0.285000, 0.308712, 0.907453, 0.461957, 0.308672, 0.831455, 0.619933, 0.308619, 0.721414, 0.752471, 0.308563, 0.581874, 0.854175, 0.308512, 0.418574, 0.920924, 0.308476, 0.238206, 0.950019, 0.308459, 0.048131, 0.940288, 0.308465, -0.143903, 0.892123, 0.308492, -0.330076, 0.807467, 0.308536, -0.502795, 0.689746, 0.308591, -0.654998, 0.543743, 0.308646, -0.780436, 0.375413, 0.308694, -0.873941, 0.191649, 0.308726, -0.931643, -0, 0.308737, -0.951147, -0.191649, 0.308726, -0.931643, -0.375413, 0.308694, -0.873941, -0.543743, 0.308646, -0.780436, -0.689746, 0.308591, -0.654998, -0.807467, 0.308536, -0.502795, -0.892123, 0.308492, -0.330076, -0.940288, 0.308465, -0.143903, -0.950019, 0.308459, 0.048131, -0.920924, 0.308476, 0.238205, -0.854175, 0.308512, 0.418574, -0.752471, 0.308563, 0.581874, -0.619933, 0.308619, 0.721414, -0.461957, 0.308672, 0.831455, -0.285000, 0.308712, 0.907453, -0.096321, 0.308735, 0.946259, 0, 0.207713, 0.978190, 0.197097, 0.207705, 0.958129, 0.386083, 0.207682, 0.898781, 0.559193, 0.207648, 0.802612, 0.709337, 0.207608, 0.673602, 0.830393, 0.207570, 0.517071, 0.917445, 0.207538, 0.339445, 0.966973, 0.207519, 0.147987, 0.976979, 0.207515, -0.049497, 0.947061, 0.207527, -0.244966, 0.878424, 0.207553, -0.430457, 0.773839, 0.207588, -0.598398, 0.637544, 0.207628, -0.741908, 0.475085, 0.207666, -0.855084, 0.293102, 0.207695, -0.933249, 0.099060, 0.207711, -0.973162, -0.099060, 0.207711, -0.973162, -0.293102, 0.207695, -0.933249, -0.475085, 0.207666, -0.855084, -0.637544, 0.207628, -0.741908, -0.773839, 0.207588, -0.598398, -0.878424, 0.207553, -0.430457, -0.947061, 0.207527, -0.244966, -0.976979, 0.207515, -0.049497, -0.966973, 0.207519, 0.147987, -0.917445, 0.207538, 0.339445, -0.830393, 0.207570, 0.517071, -0.709337, 0.207608, 0.673602, -0.559193, 0.207648, 0.802612, -0.386083, 0.207682, 0.898781, -0.197097, 0.207705, 0.958129, 0.100715, 0.104424, 0.989420, 0.297998, 0.104416, 0.948839, 0.483019, 0.104401, 0.869364, 0.648187, 0.104381, 0.754293, 0.786752, 0.104360, 0.608383, 0.893077, 0.104342, 0.437637, 0.962855, 0.104328, 0.249051, 0.993270, 0.104322, 0.050323, 0.983097, 0.104324, -0.150454, 0.932747, 0.104334, -0.345106, 0.844247, 0.104351, -0.525698, 0.721176, 0.104371, -0.684844, 0.568530, 0.104391, -0.816013, 0.392532, 0.104409, -0.913793, 0.200390, 0.104421, -0.974136, -0, 0.104425, -0.994533, -0.200390, 0.104421, -0.974136, -0.392532, 0.104409, -0.913793, -0.568530, 0.104391, -0.816013, -0.721176, 0.104371, -0.684844, -0.844247, 0.104351, -0.525697, -0.932747, 0.104334, -0.345106, -0.983097, 0.104324, -0.150454, -0.993270, 0.104322, 0.050322, -0.962855, 0.104328, 0.249051, -0.893077, 0.104342, 0.437637, -0.786752, 0.104360, 0.608383, -0.648187, 0.104381, 0.754293, -0.483019, 0.104401, 0.869364, -0.297998, 0.104416, 0.948839, -0.100715, 0.104424, 0.989420, 0, -0, 1, 0.201492, -0, 0.979490, 0.394689, -0, 0.918815, 0.571653, -0, 0.820496, 0.725137, -0, 0.688605, 0.848881, -0, 0.528583, 0.937865, -0, 0.347000, 0.988491, -0, 0.151280, 0.998719, -0, -0.050599, 0.968138, -0, -0.250418, 0.897978, -0, -0.440039, 0.791072, -0, -0.611723, 0.651747, -0, -0.758436, 0.485673, -0, -0.874141, 0.299636, -0, -0.954054, 0.101268, -0, -0.994859, -0.101269, -0, -0.994859, -0.299636, -0, -0.954054, -0.485673, -0, -0.874141, -0.651747, -0, -0.758436, -0.791072, -0, -0.611723, -0.897978, -0, -0.440039, -0.968138, -0, -0.250418, -0.998719, -0, -0.050598, -0.988491, -0, 0.151280, -0.937865, -0, 0.347000, -0.848881, -0, 0.528583, -0.725137, -0, 0.688605, -0.571653, -0, 0.820496, -0.394689, -0, 0.918815, -0.201492, -0, 0.979490, 0.100715, -0.104424, 0.989420, 0.297998, -0.104416, 0.948839, 0.483019, -0.104401, 0.869364, 0.648187, -0.104381, 0.754293, 0.786752, -0.104361, 0.608383, 0.893077, -0.104342, 0.437637, 0.962855, -0.104328, 0.249051, 0.993270, -0.104322, 0.050323, 0.983097, -0.104324, -0.150454, 0.932747, -0.104334, -0.345106, 0.844247, -0.104351, -0.525698, 0.721176, -0.104371, -0.684844, 0.568530, -0.104391, -0.816013, 0.392532, -0.104409, -0.913793, 0.200390, -0.104421, -0.974136, -0, -0.104425, -0.994533, -0.200390, -0.104421, -0.974136, -0.392532, -0.104409, -0.913793, -0.568530, -0.104391, -0.816013, -0.721176, -0.104371, -0.684844, -0.844247, -0.104351, -0.525697, -0.932747, -0.104334, -0.345106, -0.983097, -0.104324, -0.150454, -0.993270, -0.104322, 0.050322, -0.962855, -0.104328, 0.249051, -0.893077, -0.104342, 0.437637, -0.786752, -0.104361, 0.608383, -0.648187, -0.104381, 0.754293, -0.483019, -0.104401, 0.869364, -0.297998, -0.104416, 0.948839, -0.100715, -0.104424, 0.989420, 0, -0.207713, 0.978190, 0.197097, -0.207705, 0.958129, 0.386083, -0.207682, 0.898781, 0.559193, -0.207648, 0.802612, 0.709337, -0.207608, 0.673602, 0.830393, -0.207570, 0.517071, 0.917445, -0.207538, 0.339445, 0.966972, -0.207519, 0.147987, 0.976979, -0.207515, -0.049497, 0.947061, -0.207527, -0.244966, 0.878424, -0.207553, -0.430457, 0.773839, -0.207589, -0.598398, 0.637544, -0.207628, -0.741908, 0.475085, -0.207666, -0.855084, 0.293102, -0.207695, -0.933249, 0.099060, -0.207711, -0.973162, -0.099060, -0.207711, -0.973162, -0.293102, -0.207695, -0.933249, -0.475085, -0.207666, -0.855084, -0.637544, -0.207628, -0.741908, -0.773839, -0.207589, -0.598398, -0.878424, -0.207553, -0.430457, -0.947061, -0.207527, -0.244966, -0.976979, -0.207515, -0.049497, -0.966972, -0.207519, 0.147987, -0.917445, -0.207538, 0.339445, -0.830393, -0.207570, 0.517071, -0.709337, -0.207608, 0.673602, -0.559193, -0.207648, 0.802612, -0.386083, -0.207682, 0.898781, -0.197097, -0.207705, 0.958129, 0.096321, -0.308735, 0.946258, 0.285000, -0.308712, 0.907453, 0.461957, -0.308672, 0.831455, 0.619933, -0.308619, 0.721414, 0.752471, -0.308563, 0.581874, 0.854175, -0.308512, 0.418574, 0.920924, -0.308476, 0.238206, 0.950019, -0.308459, 0.048131, 0.940288, -0.308465, -0.143903, 0.892123, -0.308492, -0.330076, 0.807467, -0.308536, -0.502795, 0.689746, -0.308591, -0.654998, 0.543743, -0.308646, -0.780436, 0.375413, -0.308694, -0.873941, 0.191649, -0.308726, -0.931643, -0, -0.308737, -0.951147, -0.191649, -0.308726, -0.931643, -0.375413, -0.308694, -0.873941, -0.543743, -0.308646, -0.780436, -0.689746, -0.308591, -0.654998, -0.807467, -0.308536, -0.502795, -0.892123, -0.308492, -0.330076, -0.940288, -0.308465, -0.143903, -0.950019, -0.308459, 0.048131, -0.920924, -0.308476, 0.238205, -0.854175, -0.308512, 0.418574, -0.752471, -0.308563, 0.581874, -0.619933, -0.308619, 0.721414, -0.461957, -0.308672, 0.831455, -0.285000, -0.308712, 0.907453, -0.096321, -0.308735, 0.946258, 0, -0.406397, 0.913697, 0.184103, -0.406383, 0.894963, 0.360635, -0.406344, 0.839540, 0.522345, -0.406286, 0.749724, 0.662612, -0.406219, 0.629231, 0.775712, -0.406153, 0.483022, 0.857048, -0.406099, 0.317099, 0.903326, -0.406066, 0.138246, 0.912676, -0.406059, -0.046240, 0.884721, -0.406079, -0.228841, 0.820589, -0.406124, -0.402116, 0.722874, -0.406185, -0.558987, 0.595541, -0.406253, -0.693029, 0.443775, -0.406317, -0.798730, 0.273780, -0.406367, -0.871728, 0.092529, -0.406394, -0.909001, -0.092529, -0.406394, -0.909001, -0.273780, -0.406367, -0.871728, -0.443775, -0.406317, -0.798730, -0.595541, -0.406253, -0.693029, -0.722874, -0.406185, -0.558987, -0.820589, -0.406124, -0.402116, -0.884721, -0.406079, -0.228841, -0.912676, -0.406059, -0.046239, -0.903326, -0.406066, 0.138246, -0.857048, -0.406099, 0.317099, -0.775712, -0.406153, 0.483022, -0.662612, -0.406219, 0.629231, -0.522345, -0.406286, 0.749724, -0.360635, -0.406344, 0.839540, -0.184103, -0.406383, 0.894963, 0.087723, -0.499621, 0.861791, 0.259563, -0.499591, 0.826460, 0.420735, -0.499537, 0.757262, 0.564631, -0.499466, 0.657059, 0.685366, -0.499391, 0.529983, 0.778023, -0.499323, 0.381257, 0.838838, -0.499274, 0.216973, 0.865347, -0.499251, 0.043842, 0.856481, -0.499259, -0.131077, 0.812596, -0.499295, -0.300652, 0.735469, -0.499355, -0.457963, 0.628226, -0.499428, -0.596577, 0.495230, -0.499503, -0.710805, 0.341909, -0.499567, -0.795947, 0.174542, -0.499610, -0.848484, -0, -0.499625, -0.866242, -0.174542, -0.499610, -0.848484, -0.341909, -0.499567, -0.795947, -0.495230, -0.499503, -0.710806, -0.628226, -0.499428, -0.596577, -0.735469, -0.499355, -0.457963, -0.812596, -0.499295, -0.300652, -0.856481, -0.499259, -0.131077, -0.865347, -0.499251, 0.043841, -0.838838, -0.499274, 0.216973, -0.778023, -0.499323, 0.381257, -0.685366, -0.499391, 0.529983, -0.564631, -0.499466, 0.657059, -0.420735, -0.499537, 0.757262, -0.259563, -0.499591, 0.826460, -0.087723, -0.499621, 0.861791, 0, -0.587400, 0.809297, 0.163069, -0.587385, 0.792709, 0.319437, -0.587340, 0.743634, 0.462689, -0.587275, 0.664099, 0.586957, -0.587198, 0.557387, 0.687168, -0.587123, 0.427887, 0.759241, -0.587062, 0.280911, 0.800252, -0.587025, 0.122472, 0.808538, -0.587017, -0.040963, 0.783764, -0.587040, -0.202728, 0.726933, -0.587090, -0.356221, 0.640350, -0.587160, -0.495172, 0.527534, -0.587237, -0.613890, 0.393085, -0.587310, -0.707496, 0.242502, -0.587366, -0.772136, 0.081956, -0.587396, -0.805139, -0.081957, -0.587396, -0.805139, -0.242502, -0.587366, -0.772136, -0.393085, -0.587310, -0.707497, -0.527534, -0.587237, -0.613890, -0.640350, -0.587160, -0.495172, -0.726933, -0.587090, -0.356221, -0.783764, -0.587040, -0.202728, -0.808538, -0.587017, -0.040963, -0.800252, -0.587025, 0.122471, -0.759241, -0.587062, 0.280911, -0.687168, -0.587123, 0.427887, -0.586957, -0.587198, 0.557387, -0.462689, -0.587275, 0.664099, -0.319437, -0.587340, 0.743634, -0.163069, -0.587385, 0.792709, 0.075291, -0.668757, 0.739659, 0.222781, -0.668728, 0.709346, 0.361125, -0.668674, 0.649972, 0.484651, -0.668604, 0.563987, 0.588309, -0.668530, 0.454930, 0.667867, -0.668463, 0.327277, 0.720090, -0.668415, 0.186258, 0.742856, -0.668392, 0.037636, 0.735242, -0.668400, -0.112522, 0.697556, -0.668436, -0.258088, 0.631328, -0.668495, -0.393116, 0.539249, -0.668567, -0.512083, 0.425073, -0.668640, -0.610109, 0.293463, -0.668703, -0.683166, 0.149807, -0.668746, -0.728242, -0, -0.668761, -0.743478, -0.149807, -0.668746, -0.728242, -0.293463, -0.668703, -0.683166, -0.425073, -0.668640, -0.610109, -0.539249, -0.668567, -0.512083, -0.631328, -0.668495, -0.393116, -0.697556, -0.668436, -0.258088, -0.735242, -0.668400, -0.112522, -0.742856, -0.668392, 0.037636, -0.720090, -0.668415, 0.186258, -0.667867, -0.668463, 0.327277, -0.588309, -0.668530, 0.454930, -0.484651, -0.668604, 0.563987, -0.361125, -0.668674, 0.649972, -0.222781, -0.668728, 0.709346, -0.075291, -0.668757, 0.739659, 0, -0.742812, 0.669500, 0.134902, -0.742798, 0.655784, 0.264267, -0.742760, 0.615200, 0.382791, -0.742703, 0.549421, 0.485620, -0.742637, 0.461155, 0.568552, -0.742572, 0.354027, 0.628205, -0.742519, 0.232429, 0.662152, -0.742486, 0.101337, 0.669010, -0.742480, -0.033895, 0.648504, -0.742500, -0.167741, 0.601464, -0.742543, -0.294738, 0.529805, -0.742604, -0.409690, 0.436447, -0.742670, -0.507892, 0.325200, -0.742733, -0.585313, 0.200616, -0.742782, -0.638771, 0.067800, -0.742808, -0.666062, -0.067800, -0.742808, -0.666062, -0.200616, -0.742782, -0.638771, -0.325200, -0.742733, -0.585313, -0.436447, -0.742670, -0.507892, -0.529806, -0.742604, -0.409690, -0.601464, -0.742543, -0.294737, -0.648504, -0.742500, -0.167741, -0.669010, -0.742480, -0.033894, -0.662152, -0.742486, 0.101336, -0.628206, -0.742519, 0.232429, -0.568552, -0.742572, 0.354027, -0.485620, -0.742637, 0.461155, -0.382791, -0.742703, 0.549421, -0.264267, -0.742760, 0.615200, -0.134902, -0.742798, 0.655784, 0.059563, -0.808734, 0.585150, 0.176247, -0.808712, 0.561179, 0.285702, -0.808671, 0.514223, 0.383445, -0.808618, 0.446213, 0.465475, -0.808562, 0.359944, 0.528442, -0.808512, 0.258954, 0.569779, -0.808475, 0.147379, 0.587800, -0.808458, 0.029780, 0.581772, -0.808464, -0.089035, 0.551941, -0.808491, -0.204212, 0.499522, -0.808536, -0.311043, 0.426650, -0.808590, -0.405156, 0.336301, -0.808646, -0.482694, 0.232168, -0.808694, -0.540474, 0.118514, -0.808726, -0.576122, -0, -0.808737, -0.588170, -0.118514, -0.808726, -0.576122, -0.232168, -0.808694, -0.540474, -0.336301, -0.808646, -0.482694, -0.426650, -0.808590, -0.405156, -0.499522, -0.808536, -0.311043, -0.551941, -0.808491, -0.204212, -0.581772, -0.808464, -0.089035, -0.587800, -0.808458, 0.029780, -0.569779, -0.808475, 0.147378, -0.528442, -0.808512, 0.258954, -0.465475, -0.808562, 0.359944, -0.383445, -0.808618, 0.446213, -0.285702, -0.808671, 0.514223, -0.176247, -0.808712, 0.561179, -0.059563, -0.808734, 0.585150, 0, -0.865809, 0.500375, 0.100825, -0.865800, 0.490128, 0.197516, -0.865775, 0.459806, 0.286111, -0.865738, 0.410656, 0.362983, -0.865695, 0.344697, 0.424989, -0.865652, 0.264633, 0.469594, -0.865618, 0.173745, 0.494979, -0.865597, 0.075752, 0.500108, -0.865592, -0.025337, 0.484772, -0.865605, -0.125391, 0.449598, -0.865634, -0.220318, 0.396018, -0.865673, -0.306235, 0.326222, -0.865717, -0.379623, 0.243062, -0.865758, -0.437476, 0.149940, -0.865789, -0.477417, 0.050673, -0.865806, -0.497807, -0.050673, -0.865806, -0.497807, -0.149940, -0.865789, -0.477417, -0.243062, -0.865758, -0.437476, -0.326222, -0.865717, -0.379623, -0.396018, -0.865673, -0.306235, -0.449598, -0.865634, -0.220318, -0.484773, -0.865605, -0.125391, -0.500108, -0.865592, -0.025337, -0.494979, -0.865597, 0.075752, -0.469594, -0.865618, 0.173745, -0.424989, -0.865653, 0.264633, -0.362983, -0.865695, 0.344697, -0.286111, -0.865738, 0.410656, -0.197516, -0.865775, 0.459806, -0.100824, -0.865800, 0.490128, 0.041224, -0.913393, 0.404987, 0.121984, -0.913381, 0.388402, 0.197745, -0.913359, 0.355912, 0.265405, -0.913330, 0.308851, 0.322195, -0.913300, 0.249148, 0.365792, -0.913272, 0.179250, 0.394414, -0.913252, 0.102019, 0.406893, -0.913243, 0.020615, 0.402719, -0.913246, -0.061633, 0.382063, -0.913261, -0.141359, 0.345767, -0.913285, -0.215303, 0.295315, -0.913315, -0.280438, 0.232770, -0.913345, -0.334095, 0.160689, -0.913371, -0.374076, 0.082025, -0.913388, -0.398741, -0, -0.913394, -0.407076, -0.082025, -0.913388, -0.398741, -0.160689, -0.913371, -0.374076, -0.232770, -0.913345, -0.334095, -0.295315, -0.913315, -0.280438, -0.345767, -0.913285, -0.215303, -0.382063, -0.913261, -0.141359, -0.402719, -0.913246, -0.061633, -0.406893, -0.913243, 0.020615, -0.394414, -0.913252, 0.102019, -0.365792, -0.913272, 0.179250, -0.322195, -0.913299, 0.249148, -0.265405, -0.913330, 0.308851, -0.197745, -0.913359, 0.355912, -0.121984, -0.913381, 0.388402, -0.041224, -0.913393, 0.404987, 0, -0.950966, 0.309297, 0.062323, -0.950962, 0.302964, 0.122093, -0.950951, 0.284226, 0.176863, -0.950936, 0.253852, 0.224389, -0.950918, 0.213085, 0.262727, -0.950900, 0.163596, 0.290309, -0.950886, 0.107411, 0.306007, -0.950877, 0.046832, 0.309179, -0.950875, -0.015664, 0.299696, -0.950880, -0.077519, 0.277944, -0.950892, -0.136202, 0.244814, -0.950909, -0.189311, 0.201661, -0.950927, -0.234672, 0.150249, -0.950944, -0.270427, 0.092684, -0.950957, -0.295110, 0.031322, -0.950965, -0.307710, -0.031322, -0.950965, -0.307710, -0.092684, -0.950957, -0.295110, -0.150249, -0.950944, -0.270427, -0.201661, -0.950927, -0.234672, -0.244814, -0.950909, -0.189311, -0.277944, -0.950892, -0.136202, -0.299696, -0.950880, -0.077519, -0.309179, -0.950875, -0.015664, -0.306007, -0.950877, 0.046832, -0.290309, -0.950886, 0.107411, -0.262727, -0.950900, 0.163596, -0.224389, -0.950918, 0.213085, -0.176863, -0.950936, 0.253852, -0.122093, -0.950951, 0.284226, -0.062323, -0.950962, 0.302964, 0.021075, -0.978105, 0.207043, 0.062363, -0.978101, 0.198566, 0.101097, -0.978095, 0.181959, 0.135691, -0.978087, 0.157903, 0.164729, -0.978079, 0.127382, 0.187023, -0.978071, 0.091648, 0.201660, -0.978066, 0.052161, 0.208042, -0.978063, 0.010540, 0.205908, -0.978064, -0.031512, 0.195344, -0.978068, -0.072275, 0.176783, -0.978075, -0.110080, 0.150985, -0.978083, -0.143378, 0.119004, -0.978091, -0.170807, 0.082151, -0.978099, -0.191244, 0.041934, -0.978104, -0.203850, -0, -0.978105, -0.208111, -0.041934, -0.978104, -0.203850, -0.082151, -0.978099, -0.191244, -0.119004, -0.978091, -0.170807, -0.150985, -0.978083, -0.143378, -0.176783, -0.978075, -0.110080, -0.195344, -0.978068, -0.072275, -0.205908, -0.978064, -0.031512, -0.208042, -0.978063, 0.010540, -0.201661, -0.978066, 0.052161, -0.187023, -0.978071, 0.091648, -0.164729, -0.978079, 0.127382, -0.135691, -0.978087, 0.157903, -0.101096, -0.978095, 0.181959, -0.062363, -0.978101, 0.198566, -0.021075, -0.978105, 0.207043, 0, -0.994511, 0.104632, 0.021083, -0.994511, 0.102490, 0.041303, -0.994509, 0.096152, 0.059832, -0.994507, 0.085878, 0.075912, -0.994505, 0.072088, 0.088883, -0.994503, 0.055346, 0.098216, -0.994501, 0.036339, 0.103528, -0.994500, 0.015844, 0.104601, -0.994500, -0.005299, 0.101392, -0.994501, -0.026226, 0.094032, -0.994502, -0.046079, 0.082823, -0.994504, -0.064045, 0.068222, -0.994506, -0.079390, 0.050829, -0.994508, -0.091484, 0.031354, -0.994510, -0.099833, 0.010596, -0.994511, -0.104095, -0.010596, -0.994511, -0.104095, -0.031354, -0.994510, -0.099833, -0.050829, -0.994508, -0.091484, -0.068222, -0.994506, -0.079390, -0.082823, -0.994504, -0.064045, -0.094032, -0.994502, -0.046079, -0.101392, -0.994501, -0.026226, -0.104601, -0.994500, -0.005299, -0.103528, -0.994500, 0.015844, -0.098216, -0.994501, 0.036339, -0.088883, -0.994503, 0.055346, -0.075912, -0.994505, 0.072088, -0.059832, -0.994507, 0.085878, -0.041303, -0.994509, 0.096152, -0.021083, -0.994511, 0.102490, 0, -1, 0 };
    self->Normal2.vector = m_vector;
    self->Normal2.vector_length = sizeof(m_vector) / sizeof(float);
    }
    self->Normal2.m_type = 167;
    self->Normal2.m_data = NULL;
    X3dTextureCoordinateInit(&self->TextureCoordinate2);
    self->TextureCoordinate2.m_parent = (X3dNode *)&self->Sphere_8;
    {
    static float m_point[] = { 1, 0.500000, 0.996764, 0.500000, 0.996764, 0.510534, 0.996764, 0.520637, 0.996764, 0.529895, 0.996764, 0.537929, 0.996764, 0.544411, 0.996764, 0.549074, 0.996764, 0.551728, 0.996764, 0.552264, 0.996764, 0.550661, 0.996764, 0.546983, 0.996764, 0.541382, 0.996764, 0.534087, 0.996764, 0.525397, 0.996764, 0.515666, 0.996764, 0.505294, 0.996764, 0.494706, 0.996764, 0.484334, 0.996764, 0.474603, 0.996764, 0.465913, 0.996764, 0.458618, 0.996764, 0.453017, 0.996764, 0.449339, 0.996764, 0.447736, 0.996764, 0.448272, 0.996764, 0.450926, 0.996764, 0.455589, 0.996764, 0.462071, 0.996764, 0.470105, 0.996764, 0.479363, 0.996764, 0.489466, 0.988585, 0.510531, 0.988585, 0.531161, 0.988585, 0.550515, 0.988585, 0.567801, 0.988585, 0.582311, 0.988585, 0.593452, 0.988585, 0.600767, 0.988585, 0.603956, 0.988585, 0.602889, 0.988585, 0.597610, 0.988585, 0.588335, 0.988585, 0.575443, 0.988585, 0.559463, 0.988585, 0.541048, 0.988585, 0.520953, 0.988585, 0.500000, 0.988585, 0.479047, 0.988585, 0.458952, 0.988585, 0.440537, 0.988585, 0.424557, 0.988585, 0.411665, 0.988585, 0.402390, 0.988585, 0.397111, 0.988585, 0.396044, 0.988585, 0.399233, 0.988585, 0.406548, 0.988585, 0.417689, 0.988585, 0.432199, 0.988585, 0.449485, 0.988585, 0.468839, 0.988585, 0.489469, 0.975053, 0.500000, 0.975053, 0.531142, 0.975053, 0.561010, 0.975053, 0.588379, 0.975053, 0.612131, 0.975053, 0.631291, 0.975053, 0.645077, 0.975053, 0.652923, 0.975053, 0.654509, 0.975053, 0.649768, 0.975053, 0.638897, 0.975053, 0.622339, 0.975053, 0.600772, 0.975053, 0.575080, 0.975053, 0.546314, 0.975053, 0.515651, 0.975053, 0.484349, 0.975053, 0.453686, 0.975053, 0.424920, 0.975053, 0.399228, 0.975053, 0.377661, 0.975053, 0.361103, 0.975053, 0.350232, 0.975053, 0.345491, 0.975053, 0.347077, 0.975053, 0.354923, 0.975053, 0.368709, 0.975053, 0.387869, 0.975053, 0.411621, 0.975053, 0.438990, 0.975053, 0.468858, 0.956316, 0.520601, 0.956316, 0.560959, 0.956316, 0.598822, 0.956316, 0.632639, 0.956316, 0.661025, 0.956316, 0.682820, 0.956316, 0.697129, 0.956316, 0.703368, 0.956316, 0.701281, 0.956316, 0.690954, 0.956316, 0.672809, 0.956316, 0.647589, 0.956316, 0.616327, 0.956316, 0.580303, 0.956316, 0.540990, 0.956316, 0.500000, 0.956316, 0.459010, 0.956316, 0.419697, 0.956316, 0.383673, 0.956316, 0.352411, 0.956316, 0.327191, 0.956316, 0.309046, 0.956316, 0.298719, 0.956316, 0.296632, 0.956316, 0.302871, 0.956316, 0.317180, 0.956316, 0.338975, 0.956316, 0.367361, 0.956316, 0.401178, 0.956316, 0.439041, 0.956316, 0.479399, 0.932580, 0.500000, 0.932580, 0.550389, 0.932580, 0.598716, 0.932580, 0.643001, 0.932580, 0.681431, 0.932580, 0.712434, 0.932580, 0.734739, 0.932580, 0.747435, 0.932580, 0.750000, 0.932580, 0.742330, 0.932580, 0.724740, 0.932580, 0.697948, 0.932580, 0.663052, 0.932580, 0.621481, 0.932580, 0.574937, 0.932580, 0.525325, 0.932580, 0.474675, 0.932580, 0.425063, 0.932580, 0.378519, 0.932580, 0.336948, 0.932580, 0.302052, 0.932580, 0.275260, 0.932580, 0.257670, 0.932580, 0.250000, 0.932580, 0.252565, 0.932580, 0.265261, 0.932580, 0.287566, 0.932580, 0.318569, 0.932580, 0.356999, 0.932580, 0.401284, 0.932580, 0.449611, 0.904104, 0.529771, 0.904104, 0.588094, 0.904104, 0.642810, 0.904104, 0.691680, 0.904104, 0.732702, 0.904104, 0.764197, 0.904104, 0.784876, 0.904104, 0.793893, 0.904104, 0.790877, 0.904104, 0.775953, 0.904104, 0.749731, 0.904104, 0.713285, 0.904104, 0.668107, 0.904104, 0.616047, 0.904104, 0.559236, 0.904104, 0.500000, 0.904104, 0.440764, 0.904104, 0.383953, 0.904104, 0.331893, 0.904104, 0.286715, 0.904104, 0.250269, 0.904104, 0.224047, 0.904104, 0.209123, 0.904104, 0.206107, 0.904104, 0.215124, 0.904104, 0.235803, 0.904104, 0.267298, 0.904104, 0.308320, 0.904104, 0.357190, 0.904104, 0.411906, 0.904104, 0.470229, 0.871201, 0.500000, 0.871201, 0.567434, 0.871201, 0.632107, 0.871201, 0.691372, 0.871201, 0.742802, 0.871201, 0.784292, 0.871201, 0.814143, 0.871201, 0.831132, 0.871201, 0.834565, 0.871201, 0.824301, 0.871201, 0.800760, 0.871201, 0.764906, 0.871201, 0.718207, 0.871201, 0.662574, 0.871201, 0.600285, 0.871201, 0.533891, 0.871201, 0.466109, 0.871201, 0.399715, 0.871201, 0.337426, 0.871201, 0.281793, 0.871201, 0.235094, 0.871201, 0.199240, 0.871201, 0.175699, 0.871201, 0.165435, 0.871201, 0.168868, 0.871201, 0.185857, 0.871201, 0.215708, 0.871201, 0.257198, 0.871201, 0.308628, 0.871201, 0.367893, 0.871201, 0.432566, 0.834231, 0.537640, 0.834231, 0.611378, 0.834231, 0.680557, 0.834231, 0.742343, 0.834231, 0.794208, 0.834231, 0.834028, 0.834231, 0.860173, 0.834231, 0.871572, 0.834231, 0.867760, 0.834231, 0.848891, 0.834231, 0.815738, 0.834231, 0.769659, 0.834231, 0.712540, 0.834231, 0.646720, 0.834231, 0.574893, 0.834231, 0.500000, 0.834231, 0.425107, 0.834231, 0.353280, 0.834231, 0.287460, 0.834231, 0.230341, 0.834231, 0.184262, 0.834231, 0.151109, 0.834231, 0.132240, 0.834231, 0.128428, 0.834231, 0.139827, 0.834231, 0.165972, 0.834231, 0.205792, 0.834231, 0.257657, 0.834231, 0.319443, 0.834231, 0.388622, 0.834231, 0.462360, 0.793599, 0.500000, 0.793599, 0.581532, 0.793599, 0.659725, 0.793599, 0.731380, 0.793599, 0.793562, 0.793599, 0.843725, 0.793599, 0.879816, 0.793599, 0.900358, 0.793599, 0.904509, 0.793599, 0.892099, 0.793599, 0.863636, 0.793599, 0.820287, 0.793599, 0.763824, 0.793599, 0.696561, 0.793599, 0.621251, 0.793599, 0.540976, 0.793599, 0.459024, 0.793599, 0.378749, 0.793599, 0.303439, 0.793599, 0.236176, 0.793599, 0.179713, 0.793599, 0.136364, 0.793599, 0.107901, 0.793599, 0.095491, 0.793599, 0.099642, 0.793599, 0.120184, 0.793599, 0.156275, 0.793599, 0.206438, 0.793599, 0.268620, 0.793599, 0.340275, 0.793599, 0.418468, 0.749750, 0.543863, 0.749750, 0.629795, 0.749750, 0.710412, 0.749750, 0.782415, 0.749750, 0.842856, 0.749750, 0.889260, 0.749750, 0.919728, 0.749750, 0.933013, 0.749750, 0.928569, 0.749750, 0.906580, 0.749750, 0.867946, 0.749750, 0.814248, 0.749750, 0.747684, 0.749750, 0.670981, 0.749750, 0.587277, 0.749750, 0.500000, 0.749750, 0.412723, 0.749750, 0.329019, 0.749750, 0.252316, 0.749750, 0.185752, 0.749750, 0.132054, 0.749750, 0.093420, 0.749750, 0.071431, 0.749750, 0.066987, 0.749750, 0.080272, 0.749750, 0.110740, 0.749750, 0.157144, 0.749750, 0.217585, 0.749750, 0.289588, 0.749750, 0.370205, 0.749750, 0.456137, 0.703165, 0.500000, 0.703165, 0.592066, 0.703165, 0.680362, 0.703165, 0.761275, 0.703165, 0.831491, 0.703165, 0.888136, 0.703165, 0.928890, 0.703165, 0.952086, 0.703165, 0.956773, 0.703165, 0.942760, 0.703165, 0.910620, 0.703165, 0.861669, 0.703165, 0.797912, 0.703165, 0.721958, 0.703165, 0.636917, 0.703165, 0.546270, 0.703165, 0.453730, 0.703165, 0.363083, 0.703165, 0.278042, 0.703165, 0.202088, 0.703165, 0.138331, 0.703165, 0.089380, 0.703165, 0.057240, 0.703165, 0.043227, 0.703165, 0.047914, 0.703165, 0.071110, 0.703165, 0.111864, 0.703165, 0.168509, 0.703165, 0.238725, 0.703165, 0.319638, 0.703165, 0.407934, 0.654354, 0.548170, 0.654354, 0.642539, 0.654354, 0.731071, 0.654354, 0.810144, 0.654354, 0.876520, 0.654354, 0.927480, 0.654354, 0.960940, 0.654354, 0.975528, 0.654354, 0.970649, 0.654354, 0.946501, 0.654354, 0.904073, 0.654354, 0.845102, 0.654354, 0.772003, 0.654354, 0.687768, 0.654354, 0.595846, 0.654354, 0.500000, 0.654354, 0.404154, 0.654354, 0.312232, 0.654354, 0.227997, 0.654354, 0.154898, 0.654354, 0.095927, 0.654354, 0.053499, 0.654354, 0.029351, 0.654354, 0.024472, 0.654354, 0.039060, 0.654354, 0.072520, 0.654354, 0.123480, 0.654354, 0.189856, 0.654354, 0.268929, 0.654354, 0.357461, 0.654354, 0.451830, 0.603852, 0.500000, 0.603852, 0.598576, 0.603852, 0.693117, 0.603852, 0.779751, 0.603852, 0.854933, 0.603852, 0.915583, 0.603852, 0.959219, 0.603852, 0.984055, 0.603852, 0.989074, 0.603852, 0.974070, 0.603852, 0.939657, 0.603852, 0.887245, 0.603852, 0.818979, 0.603852, 0.737654, 0.603852, 0.646599, 0.603852, 0.549542, 0.603852, 0.450458, 0.603852, 0.353401, 0.603852, 0.262347, 0.603852, 0.181021, 0.603852, 0.112755, 0.603852, 0.060343, 0.603852, 0.025930, 0.603852, 0.010926, 0.603852, 0.015945, 0.603852, 0.040781, 0.603852, 0.084417, 0.603852, 0.145067, 0.603852, 0.220249, 0.603852, 0.306883, 0.603852, 0.401424, 0.552212, 0.550372, 0.552212, 0.649053, 0.552212, 0.741632, 0.552212, 0.824318, 0.552212, 0.893727, 0.552212, 0.947017, 0.552212, 0.982006, 0.552212, 0.997261, 0.552212, 0.992158, 0.552212, 0.966907, 0.552212, 0.922540, 0.552212, 0.860874, 0.552212, 0.784434, 0.552212, 0.696350, 0.552212, 0.600227, 0.552212, 0.500000, 0.552212, 0.399774, 0.552212, 0.303650, 0.552212, 0.215566, 0.552212, 0.139126, 0.552212, 0.077460, 0.552212, 0.033093, 0.552212, 0.007842, 0.552212, 0.002739, 0.552212, 0.017994, 0.552212, 0.052983, 0.552212, 0.106273, 0.552212, 0.175682, 0.552212, 0.258368, 0.552212, 0.350947, 0.552212, 0.449628, 0.500000, 0.500000, 0.500000, 0.600779, 0.500000, 0.697431, 0.500000, 0.786001, 0.500000, 0.862862, 0.500000, 0.924867, 0.500000, 0.969479, 0.500000, 0.994869, 0.500000, 1, 0.500000, 0.984661, 0.500000, 0.949479, 0.500000, 0.895896, 0.500000, 0.826105, 0.500000, 0.742963, 0.500000, 0.649874, 0.500000, 0.550649, 0.500000, 0.449351, 0.500000, 0.350126, 0.500000, 0.257037, 0.500000, 0.173895, 0.500000, 0.104104, 0.500000, 0.050521, 0.500000, 0.015339, 0.500000, 0, 0.500000, 0.005131, 0.500000, 0.030521, 0.500000, 0.075133, 0.500000, 0.137138, 0.500000, 0.213999, 0.500000, 0.302569, 0.500000, 0.399221, 0.447788, 0.550372, 0.447788, 0.649053, 0.447788, 0.741632, 0.447788, 0.824318, 0.447788, 0.893727, 0.447788, 0.947017, 0.447788, 0.982006, 0.447788, 0.997261, 0.447788, 0.992158, 0.447788, 0.966907, 0.447788, 0.922540, 0.447788, 0.860874, 0.447788, 0.784434, 0.447788, 0.696350, 0.447788, 0.600227, 0.447788, 0.500000, 0.447788, 0.399774, 0.447788, 0.303650, 0.447788, 0.215566, 0.447788, 0.139126, 0.447788, 0.077460, 0.447788, 0.033093, 0.447788, 0.007842, 0.447788, 0.002739, 0.447788, 0.017994, 0.447788, 0.052983, 0.447788, 0.106273, 0.447788, 0.175682, 0.447788, 0.258368, 0.447788, 0.350947, 0.447788, 0.449628, 0.396148, 0.500000, 0.396148, 0.598576, 0.396148, 0.693117, 0.396148, 0.779751, 0.396148, 0.854933, 0.396148, 0.915583, 0.396148, 0.959219, 0.396148, 0.984055, 0.396148, 0.989074, 0.396148, 0.974070, 0.396148, 0.939657, 0.396148, 0.887245, 0.396148, 0.818979, 0.396148, 0.737653, 0.396148, 0.646599, 0.396148, 0.549542, 0.396148, 0.450458, 0.396148, 0.353401, 0.396148, 0.262347, 0.396148, 0.181021, 0.396148, 0.112755, 0.396148, 0.060343, 0.396148, 0.025930, 0.396148, 0.010926, 0.396148, 0.015945, 0.396148, 0.040781, 0.396148, 0.084417, 0.396148, 0.145067, 0.396148, 0.220249, 0.396148, 0.306883, 0.396148, 0.401424, 0.345646, 0.548170, 0.345646, 0.642539, 0.345646, 0.731071, 0.345646, 0.810144, 0.345646, 0.876519, 0.345646, 0.927480, 0.345646, 0.960940, 0.345646, 0.975528, 0.345646, 0.970649, 0.345646, 0.946501, 0.345646, 0.904073, 0.345646, 0.845102, 0.345646, 0.772003, 0.345646, 0.687768, 0.345646, 0.595846, 0.345646, 0.500000, 0.345646, 0.404154, 0.345646, 0.312232, 0.345646, 0.227997, 0.345646, 0.154898, 0.345646, 0.095927, 0.345646, 0.053499, 0.345646, 0.029351, 0.345646, 0.024472, 0.345646, 0.039060, 0.345646, 0.072520, 0.345646, 0.123481, 0.345646, 0.189856, 0.345646, 0.268929, 0.345646, 0.357461, 0.345646, 0.451830, 0.296835, 0.500000, 0.296835, 0.592066, 0.296835, 0.680362, 0.296835, 0.761275, 0.296835, 0.831491, 0.296835, 0.888136, 0.296835, 0.928890, 0.296835, 0.952086, 0.296835, 0.956773, 0.296835, 0.942760, 0.296835, 0.910620, 0.296835, 0.861669, 0.296835, 0.797912, 0.296835, 0.721958, 0.296835, 0.636917, 0.296835, 0.546270, 0.296835, 0.453730, 0.296835, 0.363083, 0.296835, 0.278042, 0.296835, 0.202088, 0.296835, 0.138331, 0.296835, 0.089380, 0.296835, 0.057240, 0.296835, 0.043227, 0.296835, 0.047914, 0.296835, 0.071110, 0.296835, 0.111864, 0.296835, 0.168509, 0.296835, 0.238725, 0.296835, 0.319638, 0.296835, 0.407934, 0.250250, 0.543863, 0.250250, 0.629795, 0.250250, 0.710412, 0.250250, 0.782415, 0.250250, 0.842856, 0.250250, 0.889260, 0.250250, 0.919728, 0.250250, 0.933013, 0.250250, 0.928569, 0.250250, 0.906580, 0.250250, 0.867946, 0.250250, 0.814248, 0.250250, 0.747684, 0.250250, 0.670981, 0.250250, 0.587277, 0.250250, 0.500000, 0.250250, 0.412723, 0.250250, 0.329019, 0.250250, 0.252316, 0.250250, 0.185752, 0.250250, 0.132054, 0.250250, 0.093420, 0.250250, 0.071431, 0.250250, 0.066987, 0.250250, 0.080272, 0.250250, 0.110740, 0.250250, 0.157144, 0.250250, 0.217585, 0.250250, 0.289588, 0.250250, 0.370205, 0.250250, 0.456137, 0.206401, 0.500000, 0.206401, 0.581532, 0.206401, 0.659725, 0.206401, 0.731380, 0.206401, 0.793562, 0.206401, 0.843725, 0.206401, 0.879816, 0.206401, 0.900358, 0.206401, 0.904509, 0.206401, 0.892099, 0.206401, 0.863636, 0.206401, 0.820287, 0.206401, 0.763824, 0.206401, 0.696561, 0.206401, 0.621251, 0.206401, 0.540976, 0.206401, 0.459024, 0.206401, 0.378749, 0.206401, 0.303439, 0.206401, 0.236176, 0.206401, 0.179713, 0.206401, 0.136364, 0.206401, 0.107901, 0.206401, 0.095491, 0.206401, 0.099642, 0.206401, 0.120184, 0.206401, 0.156275, 0.206401, 0.206438, 0.206401, 0.268620, 0.206401, 0.340275, 0.206401, 0.418468, 0.165769, 0.537640, 0.165769, 0.611378, 0.165769, 0.680557, 0.165769, 0.742343, 0.165769, 0.794208, 0.165769, 0.834028, 0.165769, 0.860173, 0.165769, 0.871572, 0.165769, 0.867760, 0.165769, 0.848891, 0.165769, 0.815738, 0.165769, 0.769659, 0.165769, 0.712540, 0.165769, 0.646720, 0.165769, 0.574893, 0.165769, 0.500000, 0.165769, 0.425107, 0.165769, 0.353280, 0.165769, 0.287460, 0.165769, 0.230341, 0.165769, 0.184262, 0.165769, 0.151109, 0.165769, 0.132240, 0.165769, 0.128428, 0.165769, 0.139827, 0.165769, 0.165972, 0.165769, 0.205792, 0.165769, 0.257657, 0.165769, 0.319443, 0.165769, 0.388622, 0.165769, 0.462360, 0.128799, 0.500000, 0.128799, 0.567434, 0.128799, 0.632107, 0.128799, 0.691372, 0.128799, 0.742802, 0.128799, 0.784292, 0.128799, 0.814143, 0.128799, 0.831132, 0.128799, 0.834565, 0.128799, 0.824301, 0.128799, 0.800760, 0.128799, 0.764906, 0.128799, 0.718207, 0.128799, 0.662574, 0.128799, 0.600285, 0.128799, 0.533891, 0.128799, 0.466109, 0.128799, 0.399715, 0.128799, 0.337426, 0.128799, 0.281793, 0.128799, 0.235094, 0.128799, 0.199240, 0.128799, 0.175699, 0.128799, 0.165435, 0.128799, 0.168868, 0.128799, 0.185857, 0.128799, 0.215708, 0.128799, 0.257198, 0.128799, 0.308628, 0.128799, 0.367893, 0.128799, 0.432566, 0.095896, 0.529771, 0.095896, 0.588094, 0.095896, 0.642810, 0.095896, 0.691680, 0.095896, 0.732702, 0.095896, 0.764197, 0.095896, 0.784876, 0.095896, 0.793893, 0.095896, 0.790877, 0.095896, 0.775953, 0.095896, 0.749731, 0.095896, 0.713285, 0.095896, 0.668107, 0.095896, 0.616047, 0.095896, 0.559236, 0.095896, 0.500000, 0.095896, 0.440764, 0.095896, 0.383953, 0.095896, 0.331893, 0.095896, 0.286715, 0.095896, 0.250269, 0.095896, 0.224047, 0.095896, 0.209123, 0.095896, 0.206107, 0.095896, 0.215124, 0.095896, 0.235803, 0.095896, 0.267298, 0.095896, 0.308320, 0.095896, 0.357190, 0.095896, 0.411906, 0.095896, 0.470229, 0.067420, 0.500000, 0.067420, 0.550389, 0.067420, 0.598716, 0.067420, 0.643001, 0.067420, 0.681431, 0.067420, 0.712434, 0.067420, 0.734739, 0.067420, 0.747435, 0.067420, 0.750000, 0.067420, 0.742330, 0.067420, 0.724740, 0.067420, 0.697948, 0.067420, 0.663052, 0.067420, 0.621481, 0.067420, 0.574937, 0.067420, 0.525325, 0.067420, 0.474675, 0.067420, 0.425063, 0.067420, 0.378519, 0.067420, 0.336948, 0.067420, 0.302052, 0.067420, 0.275260, 0.067420, 0.257670, 0.067420, 0.250000, 0.067420, 0.252565, 0.067420, 0.265261, 0.067420, 0.287566, 0.067420, 0.318569, 0.067420, 0.356999, 0.067420, 0.401284, 0.067420, 0.449611, 0.043684, 0.520601, 0.043684, 0.560959, 0.043684, 0.598822, 0.043684, 0.632639, 0.043684, 0.661025, 0.043684, 0.682820, 0.043684, 0.697129, 0.043684, 0.703368, 0.043684, 0.701281, 0.043684, 0.690954, 0.043684, 0.672809, 0.043684, 0.647589, 0.043684, 0.616327, 0.043684, 0.580303, 0.043684, 0.540990, 0.043684, 0.500000, 0.043684, 0.459010, 0.043684, 0.419697, 0.043684, 0.383673, 0.043684, 0.352411, 0.043684, 0.327191, 0.043684, 0.309046, 0.043684, 0.298719, 0.043684, 0.296632, 0.043684, 0.302871, 0.043684, 0.317180, 0.043684, 0.338975, 0.043684, 0.367361, 0.043684, 0.401178, 0.043684, 0.439041, 0.043684, 0.479399, 0.024947, 0.500000, 0.024947, 0.531142, 0.024947, 0.561010, 0.024947, 0.588379, 0.024947, 0.612131, 0.024947, 0.631291, 0.024947, 0.645077, 0.024947, 0.652923, 0.024947, 0.654509, 0.024947, 0.649768, 0.024947, 0.638897, 0.024947, 0.622339, 0.024947, 0.600772, 0.024947, 0.575080, 0.024947, 0.546314, 0.024947, 0.515651, 0.024947, 0.484349, 0.024947, 0.453686, 0.024947, 0.424920, 0.024947, 0.399228, 0.024947, 0.377661, 0.024947, 0.361103, 0.024947, 0.350232, 0.024947, 0.345491, 0.024947, 0.347077, 0.024947, 0.354923, 0.024947, 0.368709, 0.024947, 0.387869, 0.024947, 0.411621, 0.024947, 0.438990, 0.024947, 0.468858, 0.011415, 0.510531, 0.011415, 0.531161, 0.011415, 0.550515, 0.011415, 0.567801, 0.011415, 0.582311, 0.011415, 0.593452, 0.011415, 0.600767, 0.011415, 0.603956, 0.011415, 0.602889, 0.011415, 0.597610, 0.011415, 0.588335, 0.011415, 0.575443, 0.011415, 0.559463, 0.011415, 0.541048, 0.011415, 0.520953, 0.011415, 0.500000, 0.011415, 0.479047, 0.011415, 0.458952, 0.011415, 0.440537, 0.011415, 0.424557, 0.011415, 0.411665, 0.011415, 0.402390, 0.011415, 0.397111, 0.011415, 0.396044, 0.011415, 0.399233, 0.011415, 0.406548, 0.011415, 0.417689, 0.011415, 0.432199, 0.011415, 0.449485, 0.011415, 0.468839, 0.011415, 0.489469, 0.003236, 0.500000, 0.003236, 0.510534, 0.003236, 0.520637, 0.003236, 0.529895, 0.003236, 0.537929, 0.003236, 0.544411, 0.003236, 0.549074, 0.003236, 0.551728, 0.003236, 0.552264, 0.003236, 0.550661, 0.003236, 0.546983, 0.003236, 0.541382, 0.003236, 0.534087, 0.003236, 0.525397, 0.003236, 0.515666, 0.003236, 0.505294, 0.003236, 0.494706, 0.003236, 0.484334, 0.003236, 0.474603, 0.003236, 0.465913, 0.003236, 0.458618, 0.003236, 0.453017, 0.003236, 0.449339, 0.003236, 0.447736, 0.003236, 0.448272, 0.003236, 0.450926, 0.003236, 0.455589, 0.003236, 0.462071, 0.003236, 0.470105, 0.003236, 0.479363, 0.003236, 0.489466, 0, 0.500000 };
    self->TextureCoordinate2.point = m_point;
    self->TextureCoordinate2.point_length = sizeof(m_point) / sizeof(float);
    }
    self->TextureCoordinate2.m_type = 276;
    self->TextureCoordinate2.m_data = NULL;
    self->Sphere_8.color = NULL;
    self->Sphere_8.coord = &self->Coordinate2;
    self->Sphere_8.normal = &self->Normal2;
    self->Sphere_8.texCoord = &self->TextureCoordinate2;
    self->Sphere_8.ccw = -1;
    {
    static int m_colorIndex[] = { 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1 };
    self->Sphere_8.colorIndex = m_colorIndex;
    self->Sphere_8.colorIndex_length = sizeof(m_colorIndex) / sizeof(int);
    }
    self->Sphere_8.colorPerVertex = 0;
    self->Sphere_8.convex = 0;
    {

    self->Sphere_8.coordIndex = m_coordIndex;
    self->Sphere_8.coordIndex_length = sizeof(m_coordIndex) / sizeof(int);
    }
    self->Sphere_8.creaseAngle = 1.570696;
    {

    self->Sphere_8.normalIndex = m_normalIndex;
    self->Sphere_8.normalIndex_length = sizeof(m_normalIndex) / sizeof(int);
    }
    self->Sphere_8.normalPerVertex = -1;
    self->Sphere_8.solid = -1;
    {

    self->Sphere_8.texCoordIndex = m_texCoordIndex;
    self->Sphere_8.texCoordIndex_length = sizeof(m_texCoordIndex) / sizeof(int);
    }
    {
    static int m_set_colorIndex[1];
    self->Sphere_8.set_colorIndex = NULL;
    self->Sphere_8.set_colorIndex_length = 0;
    }
    {
    static int m_set_coordIndex[1];
    self->Sphere_8.set_coordIndex = NULL;
    self->Sphere_8.set_coordIndex_length = 0;
    }
    {
    static int m_set_normalIndex[1];
    self->Sphere_8.set_normalIndex = NULL;
    self->Sphere_8.set_normalIndex_length = 0;
    }
    {
    static int m_set_texCoordIndex[1];
    self->Sphere_8.set_texCoordIndex = NULL;
    self->Sphere_8.set_texCoordIndex_length = 0;
    }
    self->Sphere_8.m_type = 115;
    self->Sphere_8.m_data = NULL;
    self->Shape_12.appearance = &self->Appearance_13;
    self->Shape_12.geometry = &self->Sphere_8;
    self->Shape_12.m_type = 240;
    self->Shape_12.m_data = NULL;
    {
    static float m_rotation[4] = { 0, 0, 1, 0 };
    self->Transform2.rotation = m_rotation;
    self->Transform2.rotation_length = sizeof(m_rotation) / sizeof(float);
    }
    {
    static float m_translation[3] = { -0.930091, -0.693633, -0.159536 };
    self->Transform2.translation = m_translation;
    self->Transform2.translation_length = sizeof(m_translation) / sizeof(float);
    }
    {
    static float m_bboxCenter[3] = { 0, 0, 0 };
    self->Transform2.bboxCenter = m_bboxCenter;
    self->Transform2.bboxCenter_length = sizeof(m_bboxCenter) / sizeof(float);
    }
    {
    static float m_bboxSize[3] = { -1, -1, -1 };
    self->Transform2.bboxSize = m_bboxSize;
    self->Transform2.bboxSize_length = sizeof(m_bboxSize) / sizeof(float);
    }
    {
    static float m_center[3] = { 0, 0, 0 };
    self->Transform2.center = m_center;
    self->Transform2.center_length = sizeof(m_center) / sizeof(float);
    }
    {
    static float m_scale[3] = { 1, 1, 1 };
    self->Transform2.scale = m_scale;
    self->Transform2.scale_length = sizeof(m_scale) / sizeof(float);
    }
    {
    static float m_scaleOrientation[4] = { 0, 0, 1, 0 };
    self->Transform2.scaleOrientation = m_scaleOrientation;
    self->Transform2.scaleOrientation_length = sizeof(m_scaleOrientation) / sizeof(float);
    }
    {
    static X3dNode* m_children[1];
    m_children[0] =&self->Shape_12;
    self->Transform2.children = m_children;
    self->Transform2.children_length = sizeof(m_children) / sizeof(X3dNode*);
    }
    {
    static X3dNode* m_addChildren = NULL;
    self->Transform2.addChildren = NULL;
    self->Transform2.addChildren_length = 0;
    }
    {
    static X3dNode* m_removeChildren = NULL;
    self->Transform2.removeChildren = NULL;
    self->Transform2.removeChildren_length = 0;
    }
    self->Transform2.m_type = 289;
    self->Transform2.m_data = NULL;
    X3dTimeSensorInit(&self->TimeSensor1);
    self->TimeSensor1.m_parent = (X3dNode *)&self->root;
    self->TimeSensor1.cycleInterval = 5;
    self->TimeSensor1.enabled = -1;
    self->TimeSensor1.loop = -1;
    self->TimeSensor1.startTime = 0;
    self->TimeSensor1.stopTime = 0;
    self->TimeSensor1.cycleTime = 0;
    self->TimeSensor1.fraction_changed = 0;
    self->TimeSensor1.isActive = 0;
    self->TimeSensor1.time = 0;
    self->TimeSensor1.m_type = 284;
    self->TimeSensor1.m_data = NULL;
    X3dPositionInterpolatorInit(&self->PositionInterpolator1);
    self->PositionInterpolator1.m_parent = (X3dNode *)&self->root;
    {
    static float m_key[] = { 0, 1 };
    self->PositionInterpolator1.key = m_key;
    self->PositionInterpolator1.key_length = sizeof(m_key) / sizeof(float);
    }
    {
    static float m_keyValue[] = { 0, 0, 0, -1.877895, -1.400477, -0.322109 };
    self->PositionInterpolator1.keyValue = m_keyValue;
    self->PositionInterpolator1.keyValue_length = sizeof(m_keyValue) / sizeof(float);
    }
    self->PositionInterpolator1.set_fraction = 0;
    {
    static float m_value_changed[3] = { 0, 0, 0 };
    self->PositionInterpolator1.value_changed = m_value_changed;
    self->PositionInterpolator1.value_changed_length = sizeof(m_value_changed) / sizeof(float);
    }
    self->PositionInterpolator1.m_type = 210;
    self->PositionInterpolator1.m_data = NULL;
    {
    static X3dNode* m_children[4];
    m_children[0] =&self->Transform1;
    m_children[1] =&self->Transform2;
    m_children[2] =&self->TimeSensor1;
    m_children[3] =&self->PositionInterpolator1;
    self->root.children = m_children;
    self->root.children_length = sizeof(m_children) / sizeof(X3dNode*);
    }
    {
    static float m_bboxCenter[3] = { 0, 0, 0 };
    self->root.bboxCenter = m_bboxCenter;
    self->root.bboxCenter_length = sizeof(m_bboxCenter) / sizeof(float);
    }
    {
    static float m_bboxSize[3] = { -1, -1, -1 };
    self->root.bboxSize = m_bboxSize;
    self->root.bboxSize_length = sizeof(m_bboxSize) / sizeof(float);
    }
    {
    static X3dNode* m_addChildren = NULL;
    self->root.addChildren = NULL;
    self->root.addChildren_length = 0;
    }
    {
    static X3dNode* m_removeChildren = NULL;
    self->root.removeChildren = NULL;
    self->root.removeChildren_length = 0;
    }
    {
    self->root.m_type = 104;
    self->root.m_data = NULL;
    }

}

void X3dTreeRenderCallback(struct X3dNodeStruct *node, void *data) {
    switch(node->m_type) {
      case 289:
        if (X3dTransformTreeRenderCallback)
            X3dTransformTreeRenderCallback(node, data);
        else
            X3dTransformTreeRender(node, data);
        break;
      case 196:
        if (X3dPlaneSensorTreeRenderCallback)
            X3dPlaneSensorTreeRenderCallback(node, data);
        else
            X3dPlaneSensorTreeRender(node, data);
        break;
      case 115:
        if (X3dIndexedFaceSetTreeRenderCallback)
            X3dIndexedFaceSetTreeRenderCallback(node, data);
        else
            X3dIndexedFaceSetTreeRender(node, data);
        break;
      case 147:
        if (X3dMaterialTreeRenderCallback)
            X3dMaterialTreeRenderCallback(node, data);
        else
            X3dMaterialTreeRender(node, data);
        break;
      case 284:
        if (X3dTimeSensorTreeRenderCallback)
            X3dTimeSensorTreeRenderCallback(node, data);
        else
            X3dTimeSensorTreeRender(node, data);
        break;
      case 240:
        if (X3dShapeTreeRenderCallback)
            X3dShapeTreeRenderCallback(node, data);
        else
            X3dShapeTreeRender(node, data);
        break;
      case 104:
        if (X3dGroupTreeRenderCallback)
            X3dGroupTreeRenderCallback(node, data);
        else
            X3dGroupTreeRender(node, data);
        break;
      case 210:
        if (X3dPositionInterpolatorTreeRenderCallback)
            X3dPositionInterpolatorTreeRenderCallback(node, data);
        else
            X3dPositionInterpolatorTreeRender(node, data);
        break;
      case 1:
        if (X3dAppearanceTreeRenderCallback)
            X3dAppearanceTreeRenderCallback(node, data);
        else
            X3dAppearanceTreeRender(node, data);
        break;
    }
}

void X3dTreeDoWithDataCallback(struct X3dNodeStruct *node, void *data) {
    switch(node->m_type) {
      case 289:
        if (X3dTransformTreeDoWithDataCallback)
            X3dTransformTreeDoWithDataCallback(node, data);
        else
            X3dTransformTreeDoWithData(node, data);
        break;
      case 196:
        if (X3dPlaneSensorTreeDoWithDataCallback)
            X3dPlaneSensorTreeDoWithDataCallback(node, data);
        else
            X3dPlaneSensorTreeDoWithData(node, data);
        break;
      case 115:
        if (X3dIndexedFaceSetTreeDoWithDataCallback)
            X3dIndexedFaceSetTreeDoWithDataCallback(node, data);
        else
            X3dIndexedFaceSetTreeDoWithData(node, data);
        break;
      case 147:
        if (X3dMaterialTreeDoWithDataCallback)
            X3dMaterialTreeDoWithDataCallback(node, data);
        else
            X3dMaterialTreeDoWithData(node, data);
        break;
      case 284:
        if (X3dTimeSensorTreeDoWithDataCallback)
            X3dTimeSensorTreeDoWithDataCallback(node, data);
        else
            X3dTimeSensorTreeDoWithData(node, data);
        break;
      case 240:
        if (X3dShapeTreeDoWithDataCallback)
            X3dShapeTreeDoWithDataCallback(node, data);
        else
            X3dShapeTreeDoWithData(node, data);
        break;
      case 104:
        if (X3dGroupTreeDoWithDataCallback)
            X3dGroupTreeDoWithDataCallback(node, data);
        else
            X3dGroupTreeDoWithData(node, data);
        break;
      case 210:
        if (X3dPositionInterpolatorTreeDoWithDataCallback)
            X3dPositionInterpolatorTreeDoWithDataCallback(node, data);
        else
            X3dPositionInterpolatorTreeDoWithData(node, data);
        break;
      case 1:
        if (X3dAppearanceTreeDoWithDataCallback)
            X3dAppearanceTreeDoWithDataCallback(node, data);
        else
            X3dAppearanceTreeDoWithData(node, data);
        break;
    }
}

X3dNode *X3dGetNodeFromGlName(struct X3dSceneGraph *self, int glName) {
    switch (glName) {
       case 0:
         return &self->Box_7;
       case 1:
         return &self->Sphere_8;
    }
    return NULL;
}
void X3dMFBoolSendEvent(short* target, short* source, int size) {
    int i;
    if ((target == NULL) || (source == NULL)) return;
    for (i = 0; i < size; i++)
        target[i] = source[i];
}
void X3dMFColorSendEvent(float* target, float* source, int size) {
    int i;
    if ((target == NULL) || (source == NULL)) return;
    for (i = 0; i < size; i++)
        target[i] = source[i];
}
void X3dMFColorRGBASendEvent(float* target, float* source, int size) {
    int i;
    if ((target == NULL) || (source == NULL)) return;
    for (i = 0; i < size; i++)
        target[i] = source[i];
}
void X3dMFDoubleSendEvent(double* target, double* source, int size) {
    int i;
    if ((target == NULL) || (source == NULL)) return;
    for (i = 0; i < size; i++)
        target[i] = source[i];
}
void X3dMFFloatSendEvent(float* target, float* source, int size) {
    int i;
    if ((target == NULL) || (source == NULL)) return;
    for (i = 0; i < size; i++)
        target[i] = source[i];
}
void X3dMFInt32SendEvent(int* target, int* source, int size) {
    int i;
    if ((target == NULL) || (source == NULL)) return;
    for (i = 0; i < size; i++)
        target[i] = source[i];
}
void X3dMFMatrix3fSendEvent(float* target, float* source, int size) {
    int i;
    if ((target == NULL) || (source == NULL)) return;
    for (i = 0; i < size; i++)
        target[i] = source[i];
}
void X3dMFMatrix4fSendEvent(float* target, float* source, int size) {
    int i;
    if ((target == NULL) || (source == NULL)) return;
    for (i = 0; i < size; i++)
        target[i] = source[i];
}
void X3dMFNodeSendEvent(X3dNode** target, X3dNode** source, int size) {
    int i;
    if ((target == NULL) || (source == NULL)) return;
    for (i = 0; i < size; i++)
        target[i] = source[i];
}
void X3dMFRotationSendEvent(float* target, float* source, int size) {
    int i;
    if ((target == NULL) || (source == NULL)) return;
    for (i = 0; i < size; i++)
        target[i] = source[i];
}
void X3dMFStringSendEvent(const char** target, const char** source, int size) {
    int i;
    if ((target == NULL) || (source == NULL)) return;
    for (i = 0; i < size; i++)
        target[i] = source[i];
}
void X3dMFTimeSendEvent(double* target, double* source, int size) {
    int i;
    if ((target == NULL) || (source == NULL)) return;
    for (i = 0; i < size; i++)
        target[i] = source[i];
}
void X3dMFVec2dSendEvent(double* target, double* source, int size) {
    int i;
    if ((target == NULL) || (source == NULL)) return;
    for (i = 0; i < size; i++)
        target[i] = source[i];
}
void X3dMFVec2fSendEvent(float* target, float* source, int size) {
    int i;
    if ((target == NULL) || (source == NULL)) return;
    for (i = 0; i < size; i++)
        target[i] = source[i];
}
void X3dMFVec3dSendEvent(double* target, double* source, int size) {
    int i;
    if ((target == NULL) || (source == NULL)) return;
    for (i = 0; i < size; i++)
        target[i] = source[i];
}
void X3dMFVec3fSendEvent(float* target, float* source, int size) {
    int i;
    if ((target == NULL) || (source == NULL)) return;
    for (i = 0; i < size; i++)
        target[i] = source[i];
}
void X3dMFVec4fSendEvent(float* target, float* source, int size) {
    int i;
    if ((target == NULL) || (source == NULL)) return;
    for (i = 0; i < size; i++)
        target[i] = source[i];
}
void X3dSFBoolSendEvent(short* target, short source) {
    *target = source;
}
void X3dSFColorSendEvent(float* target, float* source) {
    int i;
    if ((target == NULL) || (source == NULL)) return;
    for (i = 0; i < 3; i++)
        target[i] = source[i];
}
void X3dSFColorRGBASendEvent(float* target, float* source) {
    int i;
    if ((target == NULL) || (source == NULL)) return;
    for (i = 0; i < 4; i++)
        target[i] = source[i];
}
void X3dSFDoubleSendEvent(double* target, double source) {
    *target = source;
}
void X3dSFFloatSendEvent(float* target, float source) {
    *target = source;
}
void X3dSFImageSendEvent(int* target, int* source, int size) {
    int i;
    if ((target == NULL) || (source == NULL)) return;
    for (i = 0; i < size; i++)
        target[i] = source[i];
}
void X3dSFInt32SendEvent(int* target, int source) {
    *target = source;
}
void X3dSFMatrix3fSendEvent(float* target, float* source) {
    int i;
    if ((target == NULL) || (source == NULL)) return;
    for (i = 0; i < 9; i++)
        target[i] = source[i];
}
void X3dSFMatrix4fSendEvent(float* target, float* source) {
    int i;
    if ((target == NULL) || (source == NULL)) return;
    for (i = 0; i < 9; i++)
        target[i] = source[i];
}
void X3dSFNodeSendEvent(X3dNode** target, X3dNode* source) {
    *target = source;
}
void X3dSFRotationSendEvent(float* target, float* source) {
    int i;
    if ((target == NULL) || (source == NULL)) return;
    for (i = 0; i < 4; i++)
        target[i] = source[i];
}
void X3dSFStringSendEvent(const char** target, const char* source) {
    *target = source;
}
void X3dSFTimeSendEvent(double* target, double source) {
    *target = source;
}
void X3dSFVec2dSendEvent(double* target, double* source) {
    int i;
    if ((target == NULL) || (source == NULL)) return;
    for (i = 0; i < 2; i++)
        target[i] = source[i];
}
void X3dSFVec2fSendEvent(float* target, float* source) {
    int i;
    if ((target == NULL) || (source == NULL)) return;
    for (i = 0; i < 2; i++)
        target[i] = source[i];
}
void X3dSFVec3dSendEvent(double* target, double* source) {
    int i;
    if ((target == NULL) || (source == NULL)) return;
    for (i = 0; i < 3; i++)
        target[i] = source[i];
}
void X3dSFVec3fSendEvent(float* target, float* source) {
    int i;
    if ((target == NULL) || (source == NULL)) return;
    for (i = 0; i < 3; i++)
        target[i] = source[i];
}
void X3dProcessEvents(struct X3dSceneGraph *self, void *data) {
    int nextEvent = 0;
    if (X3dPlaneSensorProcessEventCallback != NULL) {
        nextEvent = X3dPlaneSensorProcessEventCallback(&self->PlaneSensor1, "", data);
        if (nextEvent) {
            {
            X3dSFVec3fSendEvent(self->Transform1.translation, self->PlaneSensor1.trackPoint_changed);
            if (X3dTransformProcessEventCallback != NULL) {
                nextEvent = X3dTransformProcessEventCallback(&self->Transform1, "translation", data);
                if (nextEvent) {
                }
            }
            }
        }
    }
    if (X3dTimeSensorProcessEventCallback != NULL) {
        nextEvent = X3dTimeSensorProcessEventCallback(&self->TimeSensor1, "", data);
        if (nextEvent) {
            {
            X3dSFFloatSendEvent(&(self->PositionInterpolator1.set_fraction), self->TimeSensor1.fraction_changed);
            if (X3dPositionInterpolatorProcessEventCallback != NULL) {
                nextEvent = X3dPositionInterpolatorProcessEventCallback(&self->PositionInterpolator1, "set_fraction", data);
                if (nextEvent) {
                    {
                    X3dSFVec3fSendEvent(self->Transform2.translation, self->PositionInterpolator1.value_changed);
                    if (X3dTransformProcessEventCallback != NULL) {
                        nextEvent = X3dTransformProcessEventCallback(&self->Transform2, "translation", data);
                        if (nextEvent) {
                        }
                    }
                }
                }
            }
            }
        }
    }
    reInitSensor(&(self->PlaneSensor1));
    reInitSensor(&(self->TimeSensor1));
}
/*nexteventcounter 2 */
/*nexteventcounter 1 */
/*nexteventcounter 3 */
/*nexteventcounter 2 */
/*nexteventcounter 1 */
