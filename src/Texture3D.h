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

#define GLDOUBLE double

#define X3D_TEXTUREPROPERTIES(node) node

#define INT_ID_UNDEFINED -1

#define MTMODE_OFF	16

/* this is platform-dependent but there is no default so set one here */
#ifndef MAX_MULTITEXTURE
#define MAX_MULTITEXTURE 4
#endif

#define MAX_USER_DEFINED_SHADERS 40 //Sept 2016, changed from 4 to 40. In theory could do 2^32 - we have an int32 now just for user shaders

#define TMAG_AVG_PIXEL	0

#define BOUNDARY_TO_GL(direct) direct##rc = GL_REPEAT;

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
#define TEX_NOTFOUND		6


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


#endif
