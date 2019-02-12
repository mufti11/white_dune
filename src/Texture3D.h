// The following uses code from FreeWRL
/****************************************************************************
    ... is part of the FreeWRL/FreeX3D Distribution.

    Copyright 2009 CRC Canada. (http://www.crc.gc.ca)

    FreeWRL/FreeX3D is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    FreeWRL/FreeX3D is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with FreeWRL/FreeX3D.  If not, see <http://www.gnu.org/licenses/>.
****************************************************************************/

#ifndef TEXTURE3D_H
#define TEXTURE3D_H 1

class NodeTextureProperties;

#define UNUSED(v) ((void) v)
#define min(A,B) ({ __typeof__(A) __a = (A); __typeof__(B) __b = (B); __a < __b ? __a : __b; })
#define max(A,B) ({ __typeof__(A) __a = (A); __typeof__(B) __b = (B); __a < __b ? __b : __a; })
#define MALLOC(t,sz) ((t)malloc(sz))
#define MALLOCV(_sz) (malloc(_sz))
#define STRDUP strdup
#define FREE free
#define REALLOC realloc
#define ASSERT(_whatever)

#define FPRINTF fprintf
#define ERROR_MSG(...) FPRINTF(stderr, __VA_ARGS__)
#define DEBUG_MSG(...) FPRINTF(stderr, __VA_ARGS__)
#define DEBUG_MEM(...)
#define DEBUG_TEX(...)

#define uint32 unsigned int

#define ConsoleMessage(...) FPRINTF(stderr, __VA_ARGS__)

#define FREE_IF_NZ(_ptr) if (_ptr) { \
                             FREE(_ptr); \
                             _ptr = 0; } \
                         else { \
                             DEBUG_MEM("free, pointer is already null at %s:%d\n", __FILE__, __LINE__); \
                         }

#define FW_GL_DELETETEXTURES(aaa,bbb) glDeleteTextures(aaa,bbb);
#define FW_GL_GENTEXTURES(aaa,bbb) glGenTextures(aaa,bbb)
#define FW_GL_TEXPARAMETERI(aaa,bbb,ccc) glTexParameteri(aaa,bbb,ccc)
#define FW_GL_TEXPARAMETERF(aaa,bbb,ccc) glTexParameterf(aaa,bbb,ccc)
#define FW_GL_TEXPARAMETERFV(aaa,bbb,ccc) glTexParameterfv(aaa,bbb,ccc)
#define FW_GL_TEXIMAGE2D(aaa,bbb,ccc,ddd,eee,fff,ggg,hhh,iii) glTexImage2D(aaa,bbb,ccc,ddd,eee,fff,ggg,hhh,iii)
#define FW_GL_FLUSH glFlush
#define FW_GL_TRANSLATE_F(xxx,yyy,zzz) glTranslatef(xxx,yyy,zzz)
#define FW_GL_SCALE_F(xxx,yyy,zzz) glScalef(xxx,yyy,zzz)
#define FW_GL_ROTATE_RADIANS(aaa,xxx,yyy,zzz) glRotatef(aaa*2*M_PI,xxx,yyy,zzz)
#define FW_GL_CLEAR(zzz) glClear(zzz);
#define FW_GL_READPIXELS(aaa,bbb,ccc,ddd,eee,fff,ggg) glReadPixels(aaa,bbb,ccc,ddd,eee,fff,ggg)
#define FW_GL_GETDOUBLEV(aaa,bbb) fw_glGetDoublev(aaa,bbb);

#define GLUNIFORM1F glUniform1f
#define GLUNIFORM4F glUniform4f
#define GLUNIFORM4FV glUniform4fv

#define USE_SHADER(aaa) glUseProgram(aaa)

#define GET_UNIFORM(aaa,bbb) glGetUniformLocation(aaa,bbb)
#define GET_ATTRIB(aaa,bbb) glGetAttribLocation(aaa,bbb)

#define MAX_LIGHT_STACK 8 //going down the transform stack, up to 7 local lights, with most-local as last

#define MARK_NODE_COMPILED 

#define GLDOUBLE double

#define X3D_TEXTUREPROPERTIES(node) node

#define INT_ID_UNDEFINED -1

#define MTMODE_OFF    16

#define MAT_FIRST 0x100000

#define MULTITEXTUREDefs " \
#define MTMODE_ADD	1\n \
#define MTMODE_ADDSIGNED	2\n \
#define MTMODE_ADDSIGNED2X	3\n \
#define MTMODE_ADDSMOOTH	4\n \
#define MTMODE_BLENDCURRENTALPHA	5\n \
#define MTMODE_BLENDDIFFUSEALPHA	6\n \
#define MTMODE_BLENDFACTORALPHA	7\n \
#define MTMODE_BLENDTEXTUREALPHA	8\n \
#define MTMODE_DOTPRODUCT3	9\n \
#define MTMODE_MODULATE	10\n \
#define MTMODE_MODULATE2X	11\n \
#define MTMODE_MODULATE4X	12\n \
#define MTMODE_MODULATEALPHA_ADDCOLOR	13\n \
#define MTMODE_MODULATEINVALPHA_ADDCOLOR	14\n \
#define MTMODE_MODULATEINVCOLOR_ADDALPHA	15\n \
#define MTMODE_OFF	16\n \
#define MTMODE_REPLACE	17\n \
#define MTMODE_SELECTARG1	18\n \
#define MTMODE_SELECTARG2	19\n \
#define MTMODE_SUBTRACT	20\n \
";

#define FOG_APPEARANCE_SHADER  0X00800
#define TEX3D_SHADER           0X400000
#define TEX3D_LAYER_SHADER     0x800000
#define CLIPPLANE_SHADER       0x1000000
#define PARTICLE_SHADER        0X2000000

#define CREATE_SHADER glCreateShader
#define SHADER_SOURCE glShaderSource
#define COMPILE_SHADER glCompileShader
#define GET_SHADER_INFO glGetShaderiv
#define ATTACH_SHADER glAttachShader
#define LINK_SHADER glLinkProgram

#define COMPILE_STATUS GL_COMPILE_STATUS
#define VERTEX_SHADER GL_VERTEX_SHADER
#define FRAGMENT_SHADER GL_FRAGMENT_SHADER

#define TEXTURE_REPLACE_PRIOR  0x02000
#define TEXALPHA_REPLACE_PRIOR 0x04000

//goes into flags.volume
#define SHADERFLAGS_VOLUME_DATA_BASIC		0x001
#define SHADERFLAGS_VOLUME_DATA_SEGMENT		0x002
#define SHADERFLAGS_VOLUME_DATA_ISO			0x004
#define SHADERFLAGS_VOLUME_DATA_ISO_MODE3	0x008

#define SHADERFLAGS_VOLUME_STYLE_DEFAULT	1
#define SHADERFLAGS_VOLUME_STYLE_OPACITY	2
#define SHADERFLAGS_VOLUME_STYLE_BLENDED	3
#define SHADERFLAGS_VOLUME_STYLE_BOUNDARY	4
#define SHADERFLAGS_VOLUME_STYLE_CARTOON	5
#define SHADERFLAGS_VOLUME_STYLE_COMPOSED	6
#define SHADERFLAGS_VOLUME_STYLE_EDGE		7
#define SHADERFLAGS_VOLUME_STYLE_PROJECTION	8
#define SHADERFLAGS_VOLUME_STYLE_SHADED		9
#define SHADERFLAGS_VOLUME_STYLE_SILHOUETTE	10
#define SHADERFLAGS_VOLUME_STYLE_TONE		11

#define SHADINGSTYLE_PHONG     0x40000

#define NO_APPEARANCE_SHADER 0x0001

#define MATERIAL_APPEARANCE_SHADER 0x0002

#define TWO_MATERIAL_APPEARANCE_SHADER 0x0004

#define ONE_TEX_APPEARANCE_SHADER 0x0008

#define MULTI_TEX_APPEARANCE_SHADER 0x0010

#define COLOUR_MATERIAL_SHADER 0x00020

#define FILL_PROPERTIES_SHADER 0x00040

#define HAVE_LINEPOINTS_APPEARANCE 0x00100

#define HAVE_LINEPOINTS_COLOR 0x0080

#define HAVE_TEXTURECOORDINATEGENERATOR 0x00200

#define HAVE_CUBEMAP_TEXTURE   0x00400

#define HAVE_FOG_COORDS        0x01000

#define HAVE_UNLIT_COLOR       0x4000000


/* this is platform-dependent but there is no default so set one here */
#ifndef MAX_MULTITEXTURE
#define MAX_MULTITEXTURE 4
#endif

#define MAX_USER_DEFINED_SHADERS 40 //Sept 2016, changed from 4 to 40. In theory could do 2^32 - we have an int32 now just for user shaders

#define PBUFSIZE 16384 //must hold largets PlugValue

#define TMAG_AVG_PIXEL    0

#define DESIRE(whichOne,zzz) ((whichOne & zzz)==zzz)

#define BOUNDARY_TO_GL(direct) direct##rc = GL_REPEAT;

#define SHADERFLAGS_VOLUME_STYLE_DEFAULT    1
#define SHADERFLAGS_VOLUME_STYLE_OPACITY    2
#define SHADERFLAGS_VOLUME_STYLE_BLENDED    3
#define SHADERFLAGS_VOLUME_STYLE_BOUNDARY    4
#define SHADERFLAGS_VOLUME_STYLE_CARTOON    5
#define SHADERFLAGS_VOLUME_STYLE_COMPOSED    6
#define SHADERFLAGS_VOLUME_STYLE_EDGE        7
#define SHADERFLAGS_VOLUME_STYLE_PROJECTION    8
#define SHADERFLAGS_VOLUME_STYLE_SHADED        9
#define SHADERFLAGS_VOLUME_STYLE_SILHOUETTE    10
#define SHADERFLAGS_VOLUME_STYLE_TONE        11

#define SBUFSIZE 32767 //must hold final size of composited shader part, could do per-gglobal-instance malloced buffer instead and resize to largest composited shader

#define WANT_ANAGLYPH          0x200000

#define CPV_REPLACE_PRIOR      0x08000

struct Multi_Node { int n; struct X3D_Node * *p; };

#define X3D_EFFECT(node) (NodeEffect*)node)

/* a string is stored as a pointer, and a length of that mallocd pointer */
struct Uni_String {
	int len;
	char * strptr;
	int touched;
};

struct Multi_String { int n; struct Uni_String * *p; };

struct X3D_EffectPart {
       int _nodeType; /* unique integer for each type */ 
       int _renderFlags; /*sensitive, etc */ 
       int _hit; 
       int _change; 
       int _ichange; 
       struct Vector* _parentVector; 
       double _dist; /*sorting for blending */ 
       float _extent[6]; /* used for boundingboxes - +-x, +-y, +-z */ 
       struct X3D_PolyRep *_intern; 
       int referenceCount; /* if this reaches zero, nobody wants it anymore */ 
       int _defaultContainer; /* holds the container */
       void* _gc; /* ptr to vector of ptrs to free */
       struct X3D_Node* _executionContext; /* scene or protoInstance */
 	/*** node specific data: *****/
	struct X3D_Node *metadata;
	struct Multi_String url;
	struct Uni_String *type;
	int __loadstatus;
	void * _parentResource;
	void * __loadResource;
	struct X3D_Node *_shaderUserDefinedFields;
};

//type
//  TShaderType = (vertex, geometry, fragment);
//  TShaderSource = array [TShaderType] of a string list;
//
enum TShaderType {
	SHADERPART_VERTEX,
	SHADERPART_GEOMETRY,
	SHADERPART_FRAGMENT
};

typedef enum vertexShaderResources {
    vertexGLSLVersion,

    vertexPrecisionDeclare,
    vertMaxLightsDeclare,

    vertexLightDefines,
        vertexTCGTDefines,

    vertexNormalDeclare,
    vertexPositionDeclare,
    vertexSimpleColourDeclare,
    vertexTextureMatrixDeclare,
    vertexOneMaterialDeclare,
    vertexBackMaterialDeclare,
    vertFrontColourDeclare,
    vertexTexCoordInputDeclare,
    vertexTexCoordOutputDeclare,
    vertexHatchPositionDeclare,
    vertexPointSizeDeclare,

    vertexNormPosOutput,

    vertexLightingEquation,

    vertexMainStart,

    vertexPointSizeAssign,
    vertexPositionCalculation,

    vertexOneMaterialCalculation,
    vertexNormPosCalculation,
        vertexSingleTextureCalculation,
    vertexADSLCalculation,
    vertexSimpleColourCalculation,
    vertexHatchPositionCalculation,

    vertexUserDefinedInput,

    vertexMainEnd,
    vertexEndMarker
} vertexShaderResources_t;

typedef enum fragmenShaderResources {
    fragmentGLSLVersion,
    fragmentPrecisionDeclare,
    fragMaxLightsDeclare,

    fragmentMultiTexDefines,
    fragmentLightDefines,
    fragmentFillPropDefines,

    fragmentNormalColorDefs,

    fragmentTexCoordDeclare,
    fragmentTex0Declare,
    fragmentMultiTexDeclare,
    fragmentSimpleColourDeclare,
    fragmentOneColourDeclare,
    fragmentBackColourDeclare,
    fragmentNormPosDeclare,
    fragmentHatchPositionDeclare,
    fragmentADSLLightModel,
    fragmentMultiTexModel,
    fragmentFillPropModel,

    fragmentMainStart,

    fragmentUserDefinedInput,

    fragmentSimpleColourAssign,
    fragmentOneColourAssign,
    fragmentADSLAssign,
    fragmentTextureAssign,
    fragmentFillPropAssign,

    fragmentMainEnd,
    fragmentEndMarker
} fragmentShaderResources_t;


typedef struct _s_list_t {

    void *elem;
    struct _s_list_t *next;

} s_list_t;

#define ml_elem(_item) (_item->elem)
#define ml_next(_item) (_item->next)

typedef void f_free_t(void *ptr);
#if defined(DEBUG_MALLOC) && defined(DEBUG_MALLOC_LIST)
extern s_list_t* _ml_new(const void *elem, int line, char *fi);
#define ml_new(elem) _ml_new(elem,__LINE__,__FILE__)
#else
extern s_list_t* ml_new(const void *elem);
#endif

struct tRenderTextures{
    //struct multiTexParams textureParameterStack[MAX_MULTITEXTURE];
    void *textureParameterStack;
    void *prv;
};//RenderTextures;

typedef struct {

    const char *renderer; /* replace GL_REN */
    const char *version;
    const char *vendor;
    const char *extensions;
    float versionf;
    bool have_GL_VERSION_1_1;
    bool have_GL_VERSION_1_2;
    bool have_GL_VERSION_1_3;
    bool have_GL_VERSION_1_4;
    bool have_GL_VERSION_1_5;
    bool have_GL_VERSION_2_0;
    bool have_GL_VERSION_2_1;
    bool have_GL_VERSION_3_0;

    bool av_multitexture; /* Multi textures available ? */
    bool av_npot_texture; /* Non power of 2 textures available ? */
    bool av_texture_rect; /* Rectangle textures available ? */
    bool av_occlusion_q;  /* Occlusion query available ? */
    
    int texture_units;
    int runtime_max_texture_size;
    int system_max_texture_size;
    float anisotropicDegree;
    
    GLboolean quadBuffer;        /* does platform support quadbuffer? */

} s_renderer_capabilities_t;

enum {
    IMAGETYPE_UNKNOWN = 0,
    IMAGETYPE_PNG = 1,
    IMAGETYPE_JPEG,
    IMAGETYPE_GIF,
    IMAGETYPE_DDS,
    IMAGETYPE_WEB3DIT,
    IMAGETYPE_NRRD,
    IMAGETYPE_VOL,
};

#define TEXTURE_INVALID 0

/* appearance does material depending on last texture depth */
#define NOTEXTURE 0
#define TEXTURE_NO_ALPHA 1
#define TEXTURE_ALPHA 2

/* for texIsloaded structure */
#define TEX_NOTLOADED       0
#define TEX_LOADING         1
#define TEX_READ            2
#define TEX_NEEDSBINDING    3
#define TEX_LOADED          4
#define TEX_UNSQUASHED      5
#define TEX_NOTFOUND        6

//with Proto for Brotos
#define POSSIBLE_PROTO_EXPANSION(type,inNode,outNode) \
    outNode = (type)inNode;

#define RENDER_MATERIAL_SUBNODES(which) \
    { Node *tmpN;   \
        POSSIBLE_PROTO_EXPANSION(Node *, which,tmpN) \
        if(tmpN) { \
            tmpN->bind(); \
        } \
    }

/* Texture loading table :
   newer Texture handling procedures
   each texture has this kind of structure
*/
struct textureTableIndexStruct {
    Node*  scenegraphNode;
    int    nodeType;
    int    status;
    int    hasAlpha;
    GLuint OpenGLTexture;
    GLuint ifbobuffer; //in case this texture is used as an fbo render target
    GLuint idepthbuffer; //in case this texture is used as an fbo render target
    int    frames;
    char   *filename;
    int    x;
    int    y;
    int    z;
    int    tiles[3]; //when using TILED emulator for texture3D, nx, ny tiles, and resampled z
    unsigned char *texdata;
    GLint  repeatSTR[3]; //repeatR - used for non-builtin-Texture3D ie shader will manually apply this rule
    GLint magFilter; //needed in TEX3D frag shader for Z
    int textureNumber;
    int channels; //number of original image file image channels/components 0=no texture default, 1=Intensity 2=IntensityAlpha 3=RGB 4=RGBA
};
typedef struct textureTableIndexStruct textureTableIndexStruct_s;

typedef struct pTextures{
    struct Vector *activeTextureTable;
    textureTableIndexStruct_s* loadThisTexture;

    /* current index into loadparams that texture thread is working on */
    int currentlyWorkingOn;// = -1;
    int textureInProcess;// = -1;
}* ppTextures;

#define IBOOL int

#define INSTANCEGLOBAL 1

typedef struct pLoadTextures{
    s_list_t* texture_request_list;// = NULL;
    bool loader_waiting;// = false;
    /* list of texture table entries to load */
    s_list_t *texture_list;// = NULL;
    /* are we currently active? */
    int TextureParsing; // = FALSE;
}* ppLoadTextures;


#include <stddef.h>

/* ************************************************************************** */
/* ******************************** Vector ********************************** */
/* ************************************************************************** */

/* This is the vector structure. */
struct Vector
{
 int    n;
 int    allocn;
 void*    data;
};

typedef struct Vector Stack;

/* Constructor/destructor */
struct Vector* newVector_(int elSize, int initSize,const char *,int);
#define newVector(type, initSize) \
 newVector_((int)sizeof(type), initSize,__FILE__,__LINE__)

#if defined(WRAP_MALLOC) || defined(DEBUG_MALLOC)
    void deleteVectorDebug_(char *file, int line, int elSize, struct Vector**);
    #define deleteVector(type, me) deleteVectorDebug_(__FILE__,__LINE__,(int)sizeof(type), &(me))
#else
    void deleteVector_(int elSize, struct Vector**);
    #define deleteVector(type, me) deleteVector_((int)sizeof(type), ((struct Vector**)(&me)))
#endif

/* Ensures there's at least one space free. */
void vector_ensureSpace_(int, struct Vector*, const char *fi, int line);

/* Element retrieval. */
#define vector_get(type, me, ind) \
 ((type*)((struct Vector*)me)->data)[ind]

/* pointer to element retrieval */
#define vector_get_ptr(type, me, ind) \
 &((type*)((struct Vector*)me)->data)[ind]


/* Element set. */
#define vector_set(type,me,element,value) \
        ((type*)((struct Vector *)  (me))->data)[element]=value

void vector_removeElement(int elSize,struct Vector* myp, int element);
#define vector_remove_elem(type,me,element) \
 vector_removeElement((int)sizeof(type),me,element)

/* Size of vector */
#define vectorSize(me) \
 (((struct Vector*)me)->n)

/* Back of a vector */
#define vector_back(type, me) \
 vector_get(type, me, vectorSize(me)-1)

/* Is the vector empty? */
#define vector_empty(me) \
 (!vectorSize(me))

/* Shrink the vector to minimum required space. */
void vector_shrink_(int, struct Vector*);
#define vector_shrink(type, me) \
 vector_shrink_((int)sizeof(type), me)

 /* clear out allocated data, n=0,nalloc=0 so ready for pushBack*/
 void vector_clear(struct Vector* me);

/* Push back operation. */
#define vector_pushBack(type, me, el) \
 { \
  vector_ensureSpace_((int)sizeof(type), me,__FILE__,__LINE__); \
  ASSERT(((struct Vector*)me)->n<((struct Vector*)me)->allocn); \
  vector_get(type, me, ((struct Vector*)me)->n)=el; \
  ++((struct Vector*)me)->n; \
 }

/* Pop back operation */
void vector_popBack_(struct Vector*, size_t count);
#define vector_popBack(type, me) \
 { \
  ASSERT(!vector_empty(me)); \
  --((struct Vector*)me)->n; \
 }
#define vector_popBackN(type, me, popn) \
 { \
  ASSERT(popn<=vectorSize(me)); \
  ((struct Vector*)me)->n-=popn; \
 }

/* Release and get vector data. */
void* vector_releaseData_(int, struct Vector*);
#define vector_releaseData(type, me) \
 vector_releaseData_((int)sizeof(type), me)

#define COMPILE_IF_REQUIRED

/* OpenGL renderer capabilities */


typedef struct s_shader_capabilities{
    GLint compiledOK;
    GLuint myShaderProgram;

    GLint myMaterialAmbient;
    GLint myMaterialDiffuse;
    GLint myMaterialSpecular;
    GLint myMaterialShininess;
    GLint myMaterialEmission;

    GLint myMaterialBackAmbient;
    GLint myMaterialBackDiffuse;
    GLint myMaterialBackSpecular;
    GLint myMaterialBackShininess;
    GLint myMaterialBackEmission;

    GLint myPointSize;
    
    // do we need to send down light information?
    bool  haveLightInShader; 

/*
    GLint lightcount;
    //GLint lightType;
    GLint lightType[MAX_LIGHTS];
    GLint lightAmbient[MAX_LIGHTS];
    GLint lightDiffuse[MAX_LIGHTS];
    GLint lightSpecular[MAX_LIGHTS];
    GLint lightPosition[MAX_LIGHTS];
    GLint lightSpotDir[MAX_LIGHTS];
    GLint lightAtten[MAX_LIGHTS];
    //GLint lightConstAtten[MAX_LIGHTS];
    //GLint lightLinAtten[MAX_LIGHTS];
    //GLint lightQuadAtten[MAX_LIGHTS];
    GLint lightSpotCutoffAngle[MAX_LIGHTS];
    GLint lightSpotBeamWidth[MAX_LIGHTS];
    //GLint lightRadius;
    GLint lightRadius[MAX_LIGHTS];
*/

    GLint ModelViewMatrix;
    GLint ProjectionMatrix;
    GLint NormalMatrix;
    GLint ModelViewInverseMatrix;
    GLint TextureMatrix[MAX_MULTITEXTURE];
    GLint Vertices;
    GLint Normals;
    GLint Colours;
    GLint TexCoords[MAX_MULTITEXTURE];
    GLint FogCoords; //Aug 2016

    GLint TextureUnit[MAX_MULTITEXTURE];
    GLint TextureMode[MAX_MULTITEXTURE];
    GLint TextureSource[MAX_MULTITEXTURE];
    GLint TextureFunction[MAX_MULTITEXTURE];
    GLint textureCount;
    GLint multitextureColor;

    /* texture3D */
    GLint tex3dTiles; //int[2] nx, ny number of tiles in x, y 
    GLint tex3dUseVertex; //bool flag when no 3D texture coords supplied, vertex shader should use vertex
    GLint repeatSTR;
    GLint magFilter;

    /* fill properties */
    GLint hatchColour;
    GLint hatchPercent;
    GLint hatchScale;
    GLint filledBool;
    GLint hatchedBool;
    GLint algorithm;
    
    /* TextureCoordinateGenerator type */
    GLint texCoordGenType;

    GLint fogColor;  //Aug 2016
    GLint fogvisibilityRange;
    GLint fogScale;
    GLint fogType;
    GLint fogHaveCoords;

    GLint clipplanes; //Sept 2016
    GLint nclipplanes;

/* attributes - reduce redundant state chage calls on GPU */
/*
    need to ensure that all calls to glEnableVertexAttribArray
    are tagged for redundancy - eg, in statusbarHud.c, etc.
    before trying to reduce the glEnableVertexAttribArray and
    glDisableVertexAttribArray calls.

    bool vertexAttribEnabled;
    bool texCoordAttribEnabled;
    bool colourAttribEnabled;
    bool normalAttribEnabled;
*/
    
} s_shader_capabilities_t;

struct tComponent_VolumeRendering{
    void *prv;
} Component_VolumeRendering;

typedef struct ivec4 {int X; int Y; int W; int H;} ivec4;

struct fw_MaterialParameters {
    float emission[4];
    float ambient[4];
    float diffuse[4];
    float specular[4];
    float shininess; 
};

struct matpropstruct {
    /* material properties for current shape */
    struct fw_MaterialParameters fw_FrontMaterial;
    struct fw_MaterialParameters fw_BackMaterial;

    /* which shader is active; 0 = no shader active */
    s_shader_capabilities_t *currentShaderProperties;

    float    transparency;
    GLfloat    emissionColour[3];
    GLint    cubeFace;    /* for cubemapping, if 0, not cube mapping */
    int     cullFace;    /* is this single-sided or two-sided? Simply used to reduce calls to
                        GL_ENABLE(GL_CULL_FACE), etc */

    /* for FillProperties, and LineProperties, line type (NOT pointsize) */
    int algorithm;
    bool hatchedBool;
    bool filledBool;
    GLfloat hatchPercent[2];
    GLfloat hatchScale[2];
    GLfloat hatchColour[4];

    // points now specified in shader, not via an opengl call 
    GLfloat pointSize;   

    //TextureCoordinateGenerator value - a "TCGT_XXX" type
};

class NodeMaterial;
class NodeTwoSidedMaterial;

typedef struct pComponent_Shape{

    struct matpropstruct appearanceProperties;

    /* pointer for a TextureTransform type of node */
    Node *  this_textureTransform;  /* do we have some kind of textureTransform? */

    /* for doing shader material properties */
    NodeTwoSidedMaterial *material_twoSided;
    NodeMaterial *material_oneSided;

    /* Any user defined shaders here? */
    Node * userShaderNode;
    int modulation;
    
}* ppComponent_Shape;

typedef struct {
    int base;
    int effects;
    int usershaders; 
    int volume;
} shaderflagsstruct;

#define MATRIX_SIZE 16                /* 4 x 4 matrix */
#define MAX_LARGE_MATRIX_STACK 256    /* depth of stacks */
#define MAX_SMALL_MATRIX_STACK 9      /* depth of stacks */

typedef GLDOUBLE MATRIX4[MATRIX_SIZE];

typedef struct pOpenGL_Utils{
    // list of all X3D nodes in this system.
    // scene graph is tree-structured. this is a linear list.
    struct Vector *linearNodeTable;
    // how many holes might we have in this table, due to killing nodes, etc?
    int potentialHoleCount;

    float cc_red, cc_green, cc_blue, cc_alpha;
    pthread_mutex_t  memtablelock;// = PTHREAD_MUTEX_INITIALIZER;
    MATRIX4 FW_ModelView[MAX_LARGE_MATRIX_STACK];
    MATRIX4 FW_ProjectionView[MAX_SMALL_MATRIX_STACK];
    MATRIX4 FW_TextureView[MAX_SMALL_MATRIX_STACK];

    int modelviewTOS;// = 0;
    int projectionviewTOS;// = 0;
    int textureviewTOS;// = 0;
    //int pickrayviewTOS;// = 0;

    int whichMode;// = GL_MODELVIEW;
    GLDOUBLE *currentMatrix;// = FW_ModelView[0];
#ifdef GLEW_MX
    GLEWContext glewC;
#endif

    struct Vector *myShaderTable; /* list of all active shaders requested by input */
    int userDefinedShaderCount;    /* if the user actually has a Shader node */
    char *userDefinedFragmentShader[MAX_USER_DEFINED_SHADERS];
    char *userDefinedVertexShader[MAX_USER_DEFINED_SHADERS];

    int shadingStyle; //0=flat, 1=gouraud, 2=phong 3=wireframe
    int maxStackUsed;
}* ppOpenGL_Utils;

void *OpenGL_Utils_constructor(){
    void *v = MALLOCV(sizeof(struct pOpenGL_Utils));
    memset(v,0,sizeof(struct pOpenGL_Utils));
    return v;
}

struct shaderTableEntry {
    //unsigned int whichOne;
    shaderflagsstruct whichOne;
    s_shader_capabilities_t *myCapabilities;

};

typedef struct trenderstate{
int render_sensitive,render_picking,render_vp,render_light,render_proximity,render_other,
verbose,render_blend,render_geom,render_collision,render_cube,render_background, render_boxes;
}* ttrenderstate;

/* structure for rayhits */
struct currayhit {
	struct Node *hitNode; /* What node hit at that distance? */
	GLDOUBLE modelMatrix[16]; /* What the matrices were at that node */
	GLDOUBLE projMatrix[16];
	GLDOUBLE justModel[16]; //view taken off, so transfroms from geometry-local to World
};

struct point_XYZ {GLDOUBLE x,y,z;};
struct orient_XYZA {GLDOUBLE x,y,z,a;};

struct point_XYZ3 {
	struct point_XYZ p1;
	struct point_XYZ p2;
	struct point_XYZ p3;
};

struct point_XYZ hyper_r1,hyper_r2;

typedef float shaderVec4[4];

struct profile_entry {
	char *name;
	double start;
	double accum;
	int hits;
};

typedef struct pRenderFuncs{
	int profile_entry_count;
	struct profile_entry profile_entries[100];
	int profiling_on;
	float light_linAtten[MAX_LIGHT_STACK];
	float light_constAtten[MAX_LIGHT_STACK];
	float light_quadAtten[MAX_LIGHT_STACK];
	float light_spotCutoffAngle[MAX_LIGHT_STACK];
	float light_spotBeamWidth[MAX_LIGHT_STACK];
	shaderVec4 light_amb[MAX_LIGHT_STACK];
	shaderVec4 light_dif[MAX_LIGHT_STACK];
	shaderVec4 light_pos[MAX_LIGHT_STACK];
	shaderVec4 light_spec[MAX_LIGHT_STACK];
	shaderVec4 light_spotDir[MAX_LIGHT_STACK];
    float light_radius[MAX_LIGHT_STACK];
	GLint lightType[MAX_LIGHT_STACK]; //0=point 1=spot 2=directional
	/* Rearrange to take advantage of headlight when off */
	int nextFreeLight;// = 0;
	int refreshLightUniforms;
	unsigned int currentLoop;
	unsigned int lastLoop;
	unsigned int sendCount;
	//int firstLight;//=0;
	/* lights status. Light HEADLIGHT_LIGHT is the headlight */
	GLint lightOnOff[MAX_LIGHT_STACK];
	GLint lightChanged[MAX_LIGHT_STACK]; //optimization
	GLint lastShader;
	//int cur_hits;//=0;
	void *empty_group;//=0;
	//struct point_XYZ ht1, ht2; not used
	struct point_XYZ hyper_r1,hyper_r2; /* Transformed ray for the hypersensitive node */
	struct currayhit rayph;
	struct X3D_Node *rootNode;//=NULL;	/* scene graph root node */
	struct Vector *libraries; //vector of extern proto library scenes in X3D_Proto format that are parsed shallow (not instanced scenes) - the library protos will be in X3D_Proto->protoDeclares vector
	struct X3D_Anchor *AnchorsAnchor;// = NULL;
	struct currayhit rayHit; //,rayHitHyper;
	struct trenderstate renderstate;
	int renderLevel;

	// which Shader is currently in use?
	GLint currentShader;
	Stack *render_geom_stack;
	Stack *sensor_stack;
	Stack *ray_stack;
	Stack *shaderflags_stack;
	Stack *fog_stack;
	Stack *localLight_stack;

	//struct point_XYZ t_r1,t_r2,t_r3; /* transformed ray */
	struct point_XYZ3 t_r123;
	struct point_XYZ hp;
	Stack *usehits_stack;
	Stack *usehitsB_stack;
	Stack *pickablegroupdata_stack;
	Stack *draw_call_params_stack;
}* ppRenderFuncs;
void *RenderFuncs_constructor(){
	void *v = MALLOCV(sizeof(struct pRenderFuncs));
	memset(v,0,sizeof(struct pRenderFuncs));
	return v;
}

#define FW_GL_BINDBUFFER(target,buffer) {sendBindBufferToGPU(target,buffer,__FILE__,__LINE__); }

#endif
