/*
 * NodeImageTexture3D.cpp
 *
 * Copyright (C) 2009-2019 J. "MUFTI" Scheurich
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

#include <stdio.h>
#include "stdafx.h"
#include "GL/gl.h"

#include "NodeImageTexture3D.h"
#include "Proto.h"
#include "FieldValue.h"
#include "MFString.h"
#include "SFBool.h"
#include "SFNode.h"
#include "DuneApp.h"
#include "Scene.h"
#include "MyMesh.h"
#include "Util.h"
#include "NodeMaterial.h"
#include "NodeTwoSidedMaterial.h"
#include "NodeVolumeData.h"
#include "NodeTextureProperties.h"
#include "NodeTextureTransform.h"

ProtoImageTexture3D::ProtoImageTexture3D(Scene *scene)
  : Proto(scene, "ImageTexture3D")
{
    url.set(
        addExposedField(MFSTRING, "url", new MFString()));
    repeatS.set(
        addField(SFBOOL, "repeatS", new SFBool(false)));
    repeatT.set(
        addField(SFBOOL, "repeatT", new SFBool(false)));
    repeatR.set(
        addField(SFBOOL, "repeatR", new SFBool(false)));
    textureProperties.set(
        addField(SFNODE, "textureProperties", new SFNode(), 
                 X3D_TEXTURE_PROPERTIES));

    kambiTextureCommonFields()

    crossOrigin.set(
          addExposedField(SFSTRING, "crossOrigin", new SFString("")));
    setFieldFlags(crossOrigin, FF_X3DOM_ONLY);

    hideChildren.set(
          addExposedField(SFBOOL, "hideChildren", new SFBool(true)));
    setFieldFlags(hideChildren, FF_X3DOM_ONLY);

    origChannelCount.set(
          addExposedField(SFINT32, "origChannelCount", new SFInt32(0),
                          new SFInt32(0)));
    setFieldFlags(origChannelCount, FF_X3DOM_ONLY);

    scale.set(
          addExposedField(SFBOOL, "scale", new SFBool(true)));
    setFieldFlags(scale, FF_X3DOM_ONLY);
}

Node *
ProtoImageTexture3D::create(Scene *scene)
{ 
    return new NodeImageTexture3D(scene, this); 
}

NodeImageTexture3D::NodeImageTexture3D(Scene *scene, Proto *def)
  : Node(scene, def)
{
    m_imageData = NULL;                      
    m_imageSizeX = 0;
    m_imageSizeY = 0;
    m_imageSizeZ = 0;
    m_textureTableIndex = 0;
    m_loaded = false;
    m_loadedTexture = false;
    m_textures_prv = NULL;
    m_tableIndex = NULL;
}

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
****************************************************************************

and from vx https://github.com/neurolabusc/vx/blob/master/src/tex3d.inc


****************************************************************************
This is a BSD 3-Clause License :: https://opensource.org/licenses/BSD-3-Clause

The Software has been developed for research purposes only and is not a clinical tool
Copyright (c) 2014-2018 Chris Rorden. All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions
are met:
1. Redistributions of source code must retain the above copyright
   notice, this list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright
   notice, this list of conditions and the following disclaimer in the
   documentation and/or other materials provided with the distribution.
3. The name of the copyright owner may be used to endorse or promote
   products derived from this software without specific prior written
   permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT OWNER ``AS IS'' AND ANY EXPRESS OR
IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO
EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
****************************************************************************/

#include "Texture3DNode.h"

ppTextures textures_prv = NULL;

struct multiTexParams {
    int multitex_mode[2];
    int multitex_source[2];
    int multitex_function;
};

typedef struct pRenderTextures{
    struct multiTexParams textureParameterStack[MAX_MULTITEXTURE];
}* ppRenderTextures;

textureTableIndexStruct_s *getTableIndex(int indx) {

     ppTextures p = textures_prv;

#ifdef VERBOSE
    {char line[200];
    sprintf (line,"getTableIndex, looking for %d",indx);
    ConsoleMessage (line);}
#endif
     if (indx < 0) {
          ConsoleMessage ("getTableIndex, texture <0 requested");
          return NULL;
     }

     if (p->activeTextureTable ==NULL ) {
          ConsoleMessage ("NULL sizing errror in getTableIndex");
          return NULL;
     }


     if (indx >= vectorSize(p->activeTextureTable)) {
          ConsoleMessage ("sizing errror in getTableIndex");
          return NULL;
     }


#ifdef VERBOSE
    {char line[200];
    printf ("getTableIndex - valid request\n");
    sprintf (line,"getTableIndex, for %d, size %d",indx, vectorSize(p->activeTextureTable));
    ConsoleMessage (line);}
#endif

     return vector_get(textureTableIndexStruct_s *, p->activeTextureTable, indx);
}

/* this node has changed - if there was a texture, destroy it */
void releaseTexture(Node *node) {

    int tableIndex;
    textureTableIndexStruct_s *ti;

    tableIndex  = ((NodeImageTexture3D *)node)->m_textureTableIndex;

    ti = getTableIndex(tableIndex);
    if(ti){
        ti->status = TEX_NOTLOADED;
        if (ti->OpenGLTexture != TEXTURE_INVALID) {
            ti->OpenGLTexture = TEXTURE_INVALID;
    /*         FREE_IF_NZ(ti->OpenGLTexture); */
        }
    }
}

void registerTexture0(int iaction, Node *tmp) {
    //iaction =1 add, =0 remove
    if (1) {
        ppTextures p = (ppTextures)textures_prv;
        if(iaction){
            //ADD
            // for the index, stored in the X3D node.
            int textureNumber;
            // new texture table entry. Zero all data
            textureTableIndexStruct_s * newTexture = MALLOC (textureTableIndexStruct_s *,sizeof (textureTableIndexStruct_s));
            memset(newTexture,0,sizeof(textureTableIndexStruct_s));
            newTexture->z = 1; //just texturing3D is > 1

            if (p->activeTextureTable == NULL) {
                p->activeTextureTable =newVector(textureTableIndexStruct_s *, 16);
            }

            // keep track of which texture this one is.
            textureNumber = vectorSize(p->activeTextureTable);
            //{char line[200]; sprintf (line,"registerTexture textureNumber %d",textureNumber); ConsoleMessage(line);}

            DEBUG_TEX("CREATING TEXTURE NODE: type %d\n", it->_nodeType);
            /* I need to know the texture "url" here... */

            NodeImageTexture3D *pt;
            pt = (NodeImageTexture3D *) tmp;
            pt->m_textureTableIndex = textureNumber;

            /* set the scenegraphNode here */
            newTexture->nodeType = tmp->getType();
            newTexture->scenegraphNode = tmp;
            newTexture->textureNumber = textureNumber;
            // save this to our texture table
            vector_pushBack(textureTableIndexStruct_s *, p->activeTextureTable, newTexture);
        }else{
            //REMOVE
            //we're using int indexes so we can't compact the vector to remove the element
            //we need to flag it some how, and many functions use tti *getTableIndex(int num)
            //and check if the returned value is null before trying to use it.
            //we'll try using NULL as the signal its deleted.
            textureTableIndexStruct_s * tti = NULL;
            int *textureNumber = NULL;

            releaseTexture(tmp); //Mar 23, 2015 added, to zap from gl texture name list (and its texture storage)
                                //otherwise for geoLOD and inline, the OS MEM usage keeps going up after unload/load cycle

            NodeImageTexture3D *pt;
            pt = (NodeImageTexture3D *) tmp;
            textureNumber = &pt->m_textureTableIndex;
            if(textureNumber){
                tti = getTableIndex(*textureNumber);
                if(tti){
                    // (*textureNumber) = -1; //is there a better flag?
                    vector_set(textureTableIndexStruct_s *,p->activeTextureTable,*textureNumber,NULL);
                    //unregister/unbind/deallocate anything else that was registered/bound/allocated above

                    //Problem: when unloading an inline (including geoLOD inlines) with images that haven't yet loaded,
                    // load_inline > unload_broto > unregister_broto_instance > unRegisterX3DAnyNode > unRegisterTexture > registerTexture0 (here)
                    // if we zap the tti, then when the resource thread finishes downloading the image and goes to paste
                    // into wheretoplacedata* which is a tti* that's been zapped, we crash.
                    //Solution:
                    // quick fix: don't zap tti here, leave a memory leak TRIED, WORKED AS BANDAID
                    // proper fix: redesign resource fetch so it has a way to check if tti still exists, IMPLEMENTED Dec5,2014
                    //  for example, have it use the table index number instead of a pointer directly to *tti,
                    //  and here leave a NULL in the table at that index so resource thread can check if its been zapped
                    FREE_IF_NZ(tti->texdata);
                    //->filename is not strduped, just a pointer to actual_file which is freed in resource
                    FREE_IF_NZ(tti);
                }
            }
        }
    } else {
        //ConsoleMessage ("registerTexture, ignoring this node");
    }
}
void registerTexture(Node *tmp) {
    registerTexture0(1,tmp);
}
void unRegisterTexture(Node *tmp) {
    registerTexture0(0,tmp);
}

/* ************************************************************************** */
/* ******************************** Vector ********************************** */
/* ************************************************************************** */

/* Constructor/destructor */

struct Vector* newVector_(int elSize, int initSize,const char *fi, int line) {
     struct Vector* ret;
#ifdef DEBUG_MALLOC
    //inherit __line__ and __file__ from particular spot in code that does newVector
    ret=(struct Vector *)freewrlMalloc(line,fi,sizeof(struct Vector), FALSE);
#else
    ret=MALLOC(struct Vector *, sizeof(struct Vector));
#endif
     ASSERT(ret);
     ret->n=0;
     ret->allocn=initSize;
#ifdef DEBUG_MALLOC
    //inherit __line__ and __file__ from particular spot in code that does newVector
    ret->data=(void *)freewrlMalloc(line+1, fi,elSize*ret->allocn, FALSE);
#else
     ret->data=MALLOC(void *, elSize*ret->allocn);
#endif
     ASSERT(ret->data);
    #ifdef DEBUG_MALLOC2
        ConsoleMessage ("vector, new  %x, data %x, size %d at %s:%d",ret, ret->data, initSize,fi,line);
    #endif
    
    return ret;
}

/* Ensures there's at least one space free. */
void vector_ensureSpace_(int elSize, struct Vector* me, const char *fi, int line) {
    ASSERT(me);
    if (me->n > me->allocn)
    {
        ASSERT(FALSE);
    }
    if(me->n == me->allocn) {
//        int istart, iend;
//        istart = me->allocn;
        if(me->allocn)
        {
            me->allocn*=2;
        }
        else
        {
            me->allocn=1;
            me->n = 0;
        }
//        iend = me->allocn;
#ifdef DEBUG_MALLOC
        me->data=freewrlRealloc(line, fi,me->data, elSize*me->allocn);
#else
        me->data=REALLOC(me->data, elSize*me->allocn);
#endif
        //if(iend > istart){
        //    char *cdata = (char*)me->data;
        //    memset(&cdata[istart*elSize],0,(iend-istart)*elSize);
        //}
        #ifdef DEBUG_MALLOC
            if(_noisy) printf ("vector, ensureSpace, me %p, data %p\n",me, me->data);
        #endif
        ASSERT(me->data);
    }
    ASSERT(me->n<me->allocn);
}

struct DdsLoadInfo {
  bool compressed;
  bool swap;
  bool palette;
  unsigned int divSize;
  unsigned int blockBytes;
  GLenum internalFormat;
  GLenum externalFormat;
  GLenum type;
};

#if defined (GL_BGRA)

    struct DdsLoadInfo loadInfoBGRA8 = {
      false, false, false, 1, 4, GL_RGBA8, GL_BGRA, GL_UNSIGNED_BYTE
    };
    struct DdsLoadInfo loadInfoBGR5A1 = {
      false, true, false, 1, 2, GL_RGB5_A1, GL_BGRA, GL_UNSIGNED_SHORT_1_5_5_5_REV
    };
    struct DdsLoadInfo loadInfoIndex8 = {
      false, false, true, 1, 1, GL_RGB8, GL_BGRA, GL_UNSIGNED_BYTE
    };
#endif //BGRA textures supported

#ifndef _WIN32
struct DdsLoadInfo loadInfoDXT1 = {
  true, false, false, 4, 8, GL_COMPRESSED_RGBA_S3TC_DXT1_EXT
};
struct DdsLoadInfo loadInfoDXT3 = {
  true, false, false, 4, 16, GL_COMPRESSED_RGBA_S3TC_DXT3_EXT
};
struct DdsLoadInfo loadInfoDXT5 = {
  true, false, false, 4, 16, GL_COMPRESSED_RGBA_S3TC_DXT5_EXT
};
#endif

struct DdsLoadInfo loadInfoRGB8 = {
  false, false, false, 1, 3, GL_RGB8, GL_RGB, GL_UNSIGNED_BYTE
};

#ifndef _WIN32
struct DdsLoadInfo loadInfoBGR8 = {
  false, false, false, 1, 3, GL_RGB8, GL_BGR, GL_UNSIGNED_BYTE
};
struct DdsLoadInfo loadInfoBGR565 = {
  false, true, false, 1, 2, GL_RGB5, GL_RGB, GL_UNSIGNED_SHORT_5_6_5
};
#endif

/* is this a DDS file? If so, get it, and subdivide it. Ignore MIPMAPS for now */
/* see: http://www.mindcontrol.org/~hplus/graphics/dds-info/MyDDS.cpp */
/* see: http://msdn.microsoft.com/en-us/library/bb943991.aspx/ */
// 2016: https://msdn.microsoft.com/en-us/library/windows/desktop/bb943982(v=vs.85).aspx

/* DDS readstuff */
/* DDS loader written by Jon Watte 2002 */
/* Permission granted to use freely, as long as Jon Watte */
/* is held harmless for all possible damages resulting from */
/* your use or failure to use this code. */
/* No warranty is expressed or implied. Use at your own risk, */
/* or not at all. */

#if !defined( mydds_h )

//  little-endian, of course
#define DDS_MAGIC 0x20534444


//  DDS_header.dwFlags
    #define DDSD_CAPS                   0x00000001 
//    #define DDSD_HEIGHT                 0x00000002 
//    #define DDSD_WIDTH                  0x00000004 
//    #define DDSD_PITCH                  0x00000008 
    #define DDSD_PIXELFORMAT            0x00001000 
//    #define DDSD_MIPMAPCOUNT            0x00020000 
//    #define DDSD_LINEARSIZE             0x00080000 
    #define DDSD_DEPTH                  0x00800000 

//  DDS_header.sPixelFormat.dwFlags
    #define DDPF_ALPHAPIXELS            0x00000001 
    #define DDPF_FOURCC                 0x00000004 
    #define DDPF_INDEXED                0x00000020 
    #define DDPF_RGB                    0x00000040 

//  DDS_header.sCaps.dwCaps1
    #define DDSCAPS_COMPLEX             0x00000008 
//    #define DDSCAPS_TEXTURE             0x00001000 
//    #define DDSCAPS_MIPMAP              0x00400000 

//  DDS_header.sCaps.dwCaps2
    #define DDSCAPS2_CUBEMAP            0x00000200 
    #define DDSCAPS2_CUBEMAP_POSITIVEX  0x00000400 
    #define DDSCAPS2_CUBEMAP_NEGATIVEX  0x00000800 
    #define DDSCAPS2_CUBEMAP_POSITIVEY  0x00001000 
    #define DDSCAPS2_CUBEMAP_NEGATIVEY  0x00002000 
    #define DDSCAPS2_CUBEMAP_POSITIVEZ  0x00004000 
    #define DDSCAPS2_CUBEMAP_NEGATIVEZ  0x00008000 
    #define DDSCAPS2_VOLUME             0x00200000 

/* old way - use 4-char string and cast later, not a good idea 
    #define D3DFMT_DXT1     "1TXD"    //  DXT1 compression texture format 
    #define D3DFMT_DXT2     "2TXD"    //  DXT2 compression texture format 
    #define D3DFMT_DXT3     "3TXD"    //  DXT3 compression texture format 
    #define D3DFMT_DXT4     "4TXD"    //  DXT4 compression texture format 
    #define D3DFMT_DXT5     "5TXD"    //  DXT5 compression texture format 
*/
/* new way - use actual four-byte unsigned integer value */
    #define D3DFMT_DXT1    0x31545844
//    #define D3DFMT_DXT2    0x32545844
    #define D3DFMT_DXT3    0x33545844
//    #define D3DFMT_DXT4    0x34545844
    #define D3DFMT_DXT5    0x35545844


#define PF_IS_DXT1(pf) \
  ((pf.dwFlags & DDPF_FOURCC) && \
   (pf.dwFourCC == (unsigned int) D3DFMT_DXT1))

#define PF_IS_DXT3(pf) \
  ((pf.dwFlags & DDPF_FOURCC) && \
   (pf.dwFourCC == (unsigned int) D3DFMT_DXT3))

#define PF_IS_DXT5(pf) \
  ((pf.dwFlags & DDPF_FOURCC) && \
   (pf.dwFourCC == (unsigned int) D3DFMT_DXT5))

#define PF_IS_BGRA8(pf) \
  ((pf.dwFlags & DDPF_RGB) && \
   (pf.dwFlags & DDPF_ALPHAPIXELS) && \
   (pf.dwRGBBitCount == 32) && \
   (pf.dwRBitMask == 0xff0000) && \
   (pf.dwGBitMask == 0xff00) && \
   (pf.dwBBitMask == 0xff) && \
   (pf.dwAlphaBitMask == 0xff000000U))

#define PF_IS_RGB8(pf) \
  ((pf.dwFlags & DDPF_RGB) && \
  !(pf.dwFlags & DDPF_ALPHAPIXELS) && \
   (pf.dwRGBBitCount == 24) && \
   (pf.dwRBitMask == 0xff) && \
   (pf.dwGBitMask == 0xff00) && \
   (pf.dwBBitMask == 0xff0000))

#define PF_IS_BGR8(pf) \
  ((pf.dwFlags & DDPF_RGB) && \
  !(pf.dwFlags & DDPF_ALPHAPIXELS) && \
   (pf.dwRGBBitCount == 24) && \
   (pf.dwRBitMask == 0xff0000) && \
   (pf.dwGBitMask == 0xff00) && \
   (pf.dwBBitMask == 0xff))

#define PF_IS_BGR5A1(pf) \
  ((pf.dwFlags & DDPF_RGB) && \
   (pf.dwFlags & DDPF_ALPHAPIXELS) && \
   (pf.dwRGBBitCount == 16) && \
   (pf.dwRBitMask == 0x00007c00) && \
   (pf.dwGBitMask == 0x000003e0) && \
   (pf.dwBBitMask == 0x0000001f) && \
   (pf.dwAlphaBitMask == 0x00008000))

#define PF_IS_BGR565(pf) \
  ((pf.dwFlags & DDPF_RGB) && \
  !(pf.dwFlags & DDPF_ALPHAPIXELS) && \
   (pf.dwRGBBitCount == 16) && \
   (pf.dwRBitMask == 0x0000f800) && \
   (pf.dwGBitMask == 0x000007e0) && \
   (pf.dwBBitMask == 0x0000001f))

#define PF_IS_INDEX8(pf) \
  ((pf.dwFlags & DDPF_INDEXED) && \
   (pf.dwRGBBitCount == 8))

#define PF_IS_VOLUME(pf) \
  ((pf.dwFlags & DDSD_DEPTH))
  //&& 
  // (pf.sCaps.dwCaps1 & DDSCAPS_COMPLEX) && 
  // (pf.sCaps.dwCaps1 & DDSCAPS2_VOLUME)) 



union DDS_header {
  struct {
    unsigned int    dwMagic;
    unsigned int    dwSize;
    unsigned int    dwFlags;
    unsigned int    dwHeight;
    unsigned int    dwWidth;
    unsigned int    dwPitchOrLinearSize;
    unsigned int    dwDepth;
    unsigned int    dwMipMapCount;
    unsigned int    dwReserved1[ 11 ];

    //  DDPIXELFORMAT
    struct {
      unsigned int    dwSize;
      unsigned int    dwFlags;
      unsigned int    dwFourCC;
      unsigned int    dwRGBBitCount;
      unsigned int    dwRBitMask;
      unsigned int    dwGBitMask;
      unsigned int    dwBBitMask;
      unsigned int    dwAlphaBitMask;
    } sPixelFormat;

    //  DDCAPS2
    struct {
      unsigned int    dwCaps1;
      unsigned int    dwCaps2;
      unsigned int    dwDDSX;
      unsigned int    dwReserved;
    } sCaps;
    unsigned int    dwReserved2;
  }; //JASdefStruct; // put "name" in here to get rid of compiler warning
char data[ 128 ];
};

#endif  //  mydds_h

// LoadTextures.c likes to call this one
int textureIsDDS(textureTableIndexStruct_s* this_tex, char *filename) {
    FILE *file;
    unsigned char *buffer, *bdata; //, *bdata2;
    char sniffbuf[20];
    unsigned long fileLen;
    union DDS_header hdr;
    unsigned int x = 0;
    unsigned int y = 0;
    unsigned int z = 0;
    //unsigned int rshift[4]; //to go with color bitmask
    int nchan, idoFrontBackSwap;
    //unsigned int mipMapCount = 0;
    unsigned int xSize, ySize,zSize;

    struct DdsLoadInfo * li;
    size_t xx;

    UNUSED(xx); // compiler warning mitigation
    xSize=ySize=zSize=0;
    li = NULL;

    //printf ("textureIsDDS... node %s, file %s\n",
    //    stringNodeType(this_tex->scenegraphNode->_nodeType), filename);

    /* read in file */
    file = fopen(filename,"rb");
    if (!file) 
        return FALSE;

    //sniff header
    xx=fread(sniffbuf, 4, 1, file);
    fclose(file);
    if(strncmp(sniffbuf,"DDS ",4)){
        //not DDS file
        //sniffbuf[5] = '\0';
        //printf("sniff header = %s\n",sniffbuf);
        return FALSE;
    }
    file = fopen(filename,"rb");
    /* have file, read in data */


    /* get file length */
    fseek(file, 0, SEEK_END);
    fileLen=ftell(file);
    fseek(file, 0, SEEK_SET);

    /* llocate memory */
    buffer=MALLOC(unsigned char *, fileLen+1);
    if (!buffer) {
        fclose(file);
        return FALSE;
    }

    /* read file */
    xx=fread(buffer, fileLen, 1, file);
    fclose(file);

    /* check to see if this could be a valid DDS file */
    if (fileLen < sizeof(hdr)) 
        return FALSE;

    /* look at the header, see what kind of a DDS file it might be */
    memcpy( &hdr, buffer, sizeof(hdr));

    /* does this start off with "DDS " an so on ?? */
    if ((hdr.dwMagic == DDS_MAGIC) && (hdr.dwSize == 124) &&
        (hdr.dwFlags & DDSD_PIXELFORMAT) && (hdr.dwFlags & DDSD_CAPS)) {
        //printf ("matched :DDS :\n");

        
        //printf ("dwFlags %x, DDSD_PIXELFORMAT %x, DDSD_CAPS %x\n",hdr.dwFlags, DDSD_PIXELFORMAT, DDSD_CAPS);
            xSize = hdr.dwWidth;
            ySize = hdr.dwHeight;
        //printf ("size %d, %d\n",xSize, ySize);
        

        /*
            assert( !(xSize & (xSize-1)) );
            assert( !(ySize & (ySize-1)) );
        */

        if(0){
            printf ("looking to see what it is...\n");
            printf ("DDPF_FOURCC dwFlags %x mask %x, final %x\n",hdr.sPixelFormat.dwFlags,DDPF_FOURCC,hdr.sPixelFormat.dwFlags & DDPF_FOURCC);

            printf ("if it is a dwFourCC, %x and %x\n", hdr.sPixelFormat.dwFourCC ,D3DFMT_DXT1);

            printf ("dwFlags %x\n",hdr.sPixelFormat.dwFlags);
            printf ("dwRGBBitCount %d\n",hdr.sPixelFormat.dwRGBBitCount); //24 for normal RGB
            printf ("dwRBitMask %x\n",hdr.sPixelFormat.dwRBitMask);
            printf ("dwGBitMask %x\n",hdr.sPixelFormat.dwGBitMask);
            printf ("dwBBitMask %x\n",hdr.sPixelFormat.dwBBitMask);
            printf ("dwAlphaBitMask %x\n",hdr.sPixelFormat.dwAlphaBitMask);
            printf ("dwFlags and DDPF_ALPHAPIXELS... %x\n",DDPF_ALPHAPIXELS & hdr.sPixelFormat.dwFlags);
            printf ("dwflags & DDPF_RGB %x\n,",hdr.sPixelFormat.dwFlags & DDPF_RGB);

            printf ("dwFlags and DEPTH %x\n",hdr.dwFlags & DDSD_DEPTH);
            printf ("dwCaps1 and complex %x\n",   (hdr.sCaps.dwCaps1 & DDSCAPS_COMPLEX));
            printf ("dwCaps1 and VOLUME %x\n", (hdr.sCaps.dwCaps1 & DDSCAPS2_VOLUME));
        }
        //rshift[0] = GetLowestBitPos(hdr.sPixelFormat.dwRBitMask);
        //rshift[1] = GetLowestBitPos(hdr.sPixelFormat.dwGBitMask);
        //rshift[2] = GetLowestBitPos(hdr.sPixelFormat.dwBBitMask);
        //rshift[3] = GetLowestBitPos(hdr.sPixelFormat.dwAlphaBitMask);
        bdata = NULL;
        if(hdr.sPixelFormat.dwFlags & DDPF_FOURCC){
#ifndef _WIN32
            if( PF_IS_DXT1( hdr.sPixelFormat ) ) {
                li = &loadInfoDXT1;
            }
            else if( PF_IS_DXT3( hdr.sPixelFormat ) ) {
                li = &loadInfoDXT3;
            }
            else if( PF_IS_DXT5( hdr.sPixelFormat ) ) {
                li = &loadInfoDXT5;
            }
#endif  
            #if defined (GL_BGRA)
            else if( PF_IS_BGRA8( hdr.sPixelFormat ) ) {
                li = &loadInfoBGRA8;
            }
            else if( PF_IS_BGR5A1( hdr.sPixelFormat ) ) {
                li = &loadInfoBGR5A1;
            }
            else if( PF_IS_INDEX8( hdr.sPixelFormat ) ) {
                li = &loadInfoIndex8;
            } else 
            #endif
            if( PF_IS_RGB8( hdr.sPixelFormat ) ) {
                li = &loadInfoRGB8;
            }
#ifndef _WIN32
            else if( PF_IS_BGR8( hdr.sPixelFormat ) ) {
                li = &loadInfoBGR8;
            }
            else if( PF_IS_BGR565( hdr.sPixelFormat ) ) {
                li = &loadInfoBGR565;
            }
#endif
            //else {
            //    ConsoleMessage("CubeMap li failure\n");
            //    return FALSE;
            //}
        }else{
            //no FOURCC
            bdata = &buffer[sizeof(union DDS_header)];
            //bdata = &hdr.data[0];
        }
        //fixme: do cube maps later
        //fixme: do 3d later
        x = xSize = hdr.dwWidth;
        y = ySize = hdr.dwHeight;
        z = zSize = 1;
        idoFrontBackSwap = 0;
        if( PF_IS_VOLUME(hdr) )
            z = zSize = hdr.dwDepth;
        if( hdr.sCaps.dwCaps2 & DDSCAPS2_CUBEMAP){
            int facecount = 0;
            if(hdr.sCaps.dwCaps2 & DDSCAPS2_CUBEMAP_POSITIVEX) facecount++;
            if(hdr.sCaps.dwCaps2 & DDSCAPS2_CUBEMAP_NEGATIVEX) facecount++;
            if(hdr.sCaps.dwCaps2 & DDSCAPS2_CUBEMAP_POSITIVEY) facecount++;
            if(hdr.sCaps.dwCaps2 & DDSCAPS2_CUBEMAP_NEGATIVEY) facecount++;
            if(hdr.sCaps.dwCaps2 & DDSCAPS2_CUBEMAP_POSITIVEZ) facecount++;
            if(hdr.sCaps.dwCaps2 & DDSCAPS2_CUBEMAP_NEGATIVEZ) facecount++;
            z = zSize = facecount;
            if(z==6)
                idoFrontBackSwap = 1;
        }
        nchan = 3;
        if(DDPF_ALPHAPIXELS & hdr.sPixelFormat.dwFlags) nchan = 4;
        //if(li == NULL)
        //    return FALSE;
        //if(!hdr.dwFlags & DDSD_MIPMAPCOUNT){
        if(bdata){
            //simple, convert to rgba and set tti
            int ipix,jpix,bpp, ir, ig, ib;
            unsigned int i,j,k;
            unsigned char * rgbablob = (unsigned char*)malloc(x*y*z *4);
            bpp = hdr.sPixelFormat.dwRGBBitCount / 8;
            ir = 0; ig = 1; ib = 2; //if incoming is BGR order
            if(hdr.sPixelFormat.dwRBitMask > hdr.sPixelFormat.dwBBitMask){
                //if incoming is RGB order
                ir = 2;
                ib = 0;
                //printf("BGR\n");
            }
            //printf("bitmasks R %d G %d B %d\n",hdr.sPixelFormat.dwRBitMask,hdr.sPixelFormat.dwGBitMask,hdr.sPixelFormat.dwBBitMask);
            //printf("bpp=%d x %d y %d z %d\n",bpp, x,y,z);
            for(i=0;i<z;i++){
                for(j=0;j<y;j++){
                    for(k=0;k<x;k++){
                        unsigned char *pixel,*rgba;
                        int ii;
                        ii = idoFrontBackSwap && i == 4? 5 : i; //swap Front and Back faces for opengl order
                        ii = idoFrontBackSwap && i == 5? 4 : i;
                        ii = i;
                        ipix = (i*y +j)*x +k;     //top down, for input image
                        jpix = (ii*y +(y-1-j))*x + k; //bottom up, for ouput texture
                        pixel = &bdata[ipix * bpp];
                        rgba = &rgbablob[jpix *4];
                        //freewrl target format: RGBA
                        //swizzle if incoming is BGRA
                        rgba[3] = 255;
                        rgba[0] = pixel[ir];
                        rgba[1] = pixel[ig];
                        rgba[2] = pixel[ib];
                        if(nchan == 4)
                            rgba[3] = pixel[3];
                        if(0){
                            static int once = 0;
                            if(!once){
                                printf("pixel R=%x G=%x B=%x A=%x\n",rgba[0],rgba[1],rgba[2],rgba[3]);
                                //once = 1;
                            }
                        }

                    }
                }
            }
            this_tex->channels = nchan;
            this_tex->x = x;
            this_tex->y = y;
            this_tex->z = z;
            this_tex->texdata = rgbablob;
            return TRUE;
        }else{
            return FALSE;
        }

        //mipMapCount = (hdr.dwFlags & DDSD_MIPMAPCOUNT) ? hdr.dwMipMapCount : 1;
        //printf ("mipMapCount %d\n",mipMapCount);

        if( li->compressed ) {
            //printf ("compressed\n");
            /*
            size_t size = max( li->divSize, x )/li->divSize * max( li->divSize, y )/li->divSize * li->blockBytes;
            assert( size == hdr.dwPitchOrLinearSize );
            assert( hdr.dwFlags & DDSD_LINEARSIZE );
            unsigned char * data = (unsigned char *)malloc( size );
            if( !data ) {
                goto failure;
            }
            format = cFormat = li->internalFormat;
            for( unsigned int ix = 0; ix < mipMapCount; ++ix ) {
                fread( data, 1, size, f );
                glCompressedTexImage2D( GL_TEXTURE_2D, ix, li->internalFormat, x, y, 0, size, data );
                gl->updateError();
                x = (x+1)>>1;
                y = (y+1)>>1;
                size = max( li->divSize, x )/li->divSize * max( li->divSize, y )/li->divSize * li->blockBytes;
            }
            free( data );
            */
        } else if( li->palette ) {
            //printf ("palette\n");
            /*
            //  currently, we unpack palette into BGRA
            //  I'm not sure we always get pitch...
            assert( hdr.dwFlags & DDSD_PITCH );
            assert( hdr.sPixelFormat.dwRGBBitCount == 8 );
            size_t size = hdr.dwPitchOrLinearSize * ySize;
            //  And I'm even less sure we don't get padding on the smaller MIP levels...
            assert( size == x * y * li->blockBytes );
            format = li->externalFormat;
            cFormat = li->internalFormat;
            unsigned char * data = (unsigned char *)malloc( size );
            unsigned int palette[ 256 ];
            unsigned int * unpacked = (unsigned int *)malloc( size*sizeof( unsigned int ) );
            fread( palette, 4, 256, f );
            for( unsigned int ix = 0; ix < mipMapCount; ++ix ) {
                fread( data, 1, size, f );
                for( unsigned int zz = 0; zz < size; ++zz ) {
                unpacked[ zz ] = palette[ data[ zz ] ];
                }
                glPixelStorei( GL_UNPACK_ROW_LENGTH, y );
                glTexImage2D( GL_TEXTURE_2D, ix, li->internalFormat, x, y, 0, li->externalFormat, li->type, unpacked );
                gl->updateError();
                x = (x+1)>>1;
                y = (y+1)>>1;
                size = x * y * li->blockBytes;
            }
            free( data );
            free( unpacked );
            */  
        } else {
            //int size;

            if( li->swap ) {
            //printf ("swap\n");

            /*
            glPixelStorei( GL_UNPACK_SWAP_BYTES, GL_TRUE );
            */
            }
            //size = x * y * li->blockBytes;

            //printf ("size is %d\n",size);
            /*
            format = li->externalFormat;
            cFormat = li->internalFormat;
            unsigned char * data = (unsigned char *)malloc( size );
            //fixme: how are MIP maps stored for 24-bit if pitch != ySize*3 ?
            for( unsigned int ix = 0; ix < mipMapCount; ++ix ) {
                fread( data, 1, size, f );
                glPixelStorei( GL_UNPACK_ROW_LENGTH, y );
                glTexImage2D( GL_TEXTURE_2D, ix, li->internalFormat, x, y, 0, li->externalFormat, li->type, data );
                gl->updateError();
                x = (x+1)>>1;
                y = (y+1)>>1;
                size = x * y * li->blockBytes;
            }
            free( data );
            glPixelStorei( GL_UNPACK_SWAP_BYTES, GL_FALSE );
            gl->updateError();
            */
        }
        /*
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, mipMapCount-1 );
        gl->updateError();

        return true;

        failure:
        return false;
        }
        */

    }
    // else {
    //    //printf ("put in the dummy file here, and call it quits\n");
    //}
    FREE_IF_NZ(buffer);
    return FALSE;
}

int loadImage_web3dit(struct textureTableIndexStruct *tti, char *fname){
/*    TESTED ONLY RGB Geometry 3 and C AS OF SEPT 9, 2016
    reads image in ascii format almost like you would put inline for PixelTexture
    Goal: easy to create image file format just sufficient for web3d types:
        2 2D texture
        3 3D texture
        C cubemap
        volume (float luminance)
        panorama
    with sniffable header web3dit:
    """
web3ditG #H 7 byte magic header, means web3d compatible image in text form, 1byte for Geometry sniffing
1       #F file version
C       #G {C,P,3,2}: image geometry: C: cubemap RHS y-up z/depth/layer/order [+-x,+-y,+-z], top of top +z, bottom of bottom -z P: 360 panorama [L->R, 360/z ], 3: texture3D or Volume [z=depth], 2: texture2D
        #O optional description
x       #T {x,i,f} how to read space-delimited value: x as hex, i as int, f as float
0 255   #R range of channel, most useful for normalizing floats
4       #N channels/components per value ie RGBA as int: 4, RGBA as 4 ints: 1
1       #M values per pixel ie RGBA as int: 1, RGBA as 4 ints: 4
RGBA    #C[N*M] component names and order, choose from: {R,G,B,A,L} ie RGBA, LA, L, RGB
3       #D number of dimensions, 2 for normal 2D image, 3 for 3D image
3 3 3   #P[D] size in pixels in each dimension: x,y,z (use 1 for z if 2D)
D       #Y {U,D} image y-Down or texture y-Up row order
#I image values follow with x in inner loop, Y image direction, z in outer:
0xFF00FF .... 
    """
    format 'invented' by dug9 for testing freewrl, License: MIT
*/
    int i,j,k,m,nx,ny,nz,nv,nc, iret, totalbytes, ipix, jpix, kpix, nchan;
    int version, Rmin, Rmax, Nchannelspervalue, Mvaluesperpixel, Dimensions;
    unsigned int pixint, Pixels[10], iydown;
    float pixfloat;
    char Geometry, ODescription[200], Type, Componentnames[10], YDirection;
    FILE *fp;

    iret = FALSE;

    fp = fopen(fname,"r");
    if (fp != NULL) {
        char *rv; 
        UNUSED(rv);

        char line [1000];
        rv = fgets(line,1000,fp);
        if(strncmp(line,"web3dit",7)){
            //not our type
            fclose(fp);
            return iret;
        }
        //could sniff Geometry here, if caller says what geometry type is OK for them, return if not OK
        rv=fgets(line,1000,fp);
        sscanf(line,"%c",&Geometry);
        rv=fgets(line,1000,fp);
        sscanf(line,"%d",&version);
        rv=fgets(line,1000,fp);
        sscanf(line,"%s",ODescription);
        rv=fgets(line,1000,fp);
        sscanf(line,"%c",&Type);
        rv=fgets(line,1000,fp);
        sscanf(line,"%d %d",&Rmin,&Rmax);

        rv=fgets(line,1000,fp);
        sscanf(line,"%d",&Nchannelspervalue);
        rv=fgets(line,1000,fp);
        sscanf(line,"%d",&Mvaluesperpixel);
        rv=fgets(line,1000,fp);
        sscanf(line,"%s",Componentnames);
        rv=fgets(line,1000,fp);
        sscanf(line,"%d",&Dimensions);
        rv=fgets(line,1000,fp);
        sscanf(line,"%d %d %d",&Pixels[0], &Pixels[1], &Pixels[2]);
        rv=fgets(line,1000,fp);
        sscanf(line,"%c",&YDirection);
        rv=fgets(line,1000,fp); //waste #I Image warning line

        nx = ny = nz = 1;
        nx = Pixels[0];
        ny = Pixels[1];
        if(Dimensions > 2) nz = Pixels[2];
        nv = Mvaluesperpixel;
        nc = Nchannelspervalue;
        nchan = nv * nc;
        iydown = 1;
        if(YDirection == 'U') iydown = 0;
            
        totalbytes = 4 * nx * ny * nz; //output 4 channel RGBA image size
        if(totalbytes <= 256 * 256 * 256 * 4){
            unsigned char *rgbablob;
            rgbablob = (unsigned char*)malloc(totalbytes);
            memset(rgbablob,0,totalbytes);

            //now convert to RGBA 4 bytes per pixel
            for(i=0;i<nz;i++){
                for(j=0;j<ny;j++){
                    for(k=0;k<nx;k++){
                        unsigned char pixel[4],*rgba, n;
                        pixel[0] = pixel[1] = pixel[2] = pixel[3] = 0;
                        for(m=0;m<nv;m++){
                            int rvi;
                            UNUSED(rvi);

                            switch(Type){
                                case 'f':
                                rvi=fscanf(fp,"%f",&pixfloat);
                                break;
                                case 'x':
                                rvi=fscanf(fp,"%x",&pixint);
                                break;
                                case 'i':
                                default:
                                rvi=fscanf(fp,"%d",&pixint);
                                break;
                            }
                            for(n=0;n<nc;n++){
                                switch(Type){
                                    case 'f':
                                        pixel[n] = (unsigned char)(unsigned int)((pixfloat - Rmin) / (Rmax - Rmin) * 255.0);
                                    break;
                                    case 'x':
                                    case 'i':
                                    default:
                                        pixel[n] = (pixint >> n*8) & 0xff;
                                        break;
                                }
                            }

                        }
                        //for RGBA, pixel[0] is A, pixel[3] is B
                        //printf("[%x %x %x %x]\n",(int)pixel[0],(int)pixel[1],(int)pixel[2],(int)pixel[3]);
                        
                        ipix = (i*ny +j)*nx +k;          //if file is like outgoing y-up texture order: first row is bottom of texture
                        jpix = (i*ny +(ny-1-j))*nx + k;  //if file is in y-down image order: first row is top of image
                        kpix = iydown ? jpix : ipix;
                        rgba = &rgbablob[kpix*4];
                        //http://www.color-hex.com/ #aabbcc
                        switch(nchan){
                            case 1: rgba[0] = rgba[1] = rgba[2] = pixel[0]; rgba[3] = 255;break;
                            case 2: rgba[0] = rgba[1] = rgba[2] = pixel[1]; rgba[3] = pixel[0];break;
                            case 3: rgba[0] = pixel[2]; rgba[1] = pixel[1]; rgba[2] = pixel[0]; rgba[3] = 255;  // BGRA
                            break;
                            case 4: rgba[0] = pixel[3]; rgba[1] = pixel[2]; rgba[2] = pixel[1]; rgba[3] = pixel[0]; break; // BGRA
                            default:
                                break;
                        }
                        //memcpy(rgba,&pixint,4);

                    }
                }
            }
            tti->channels = nchan;
            tti->x = nx;
            tti->y = ny;
            tti->z = nz;
            tti->texdata = rgbablob;
            if(0){
                printf("\n");
                for(i=0;i<tti->z;i++){
                    for(j=0;j<tti->y;j++){
                        for(k=0;k<tti->x;k++){
                            int ipix,kpix;
                            // int jpix;
                            unsigned int pixint;
                            ipix = (i*tti->y + j)*tti->x + k;
                            //jpix = (i*tti->y + (tti->y -1 - j))*tti->x + k;
                            kpix = ipix; //print it like we see it
                            memcpy(&pixint,&tti->texdata[kpix*4],4);
                            printf("%x ",pixint);
                        }
                        printf("\n");
                    }

                }
            }
            iret = TRUE;
        }
        fclose(fp);
    }
    return iret;

}

int loadImage3DVol(struct textureTableIndexStruct *tti, char *fname){
/* UNTESTED, UNUSED AS OF SEPT 6, 2016
    a simple 3D volume texture format 
    - primarily for int gray/luminance, useful for VolumeRendering
    - does have a 4 channel 
    x but only 2 bytes for 4 channels - 4 bits per channel
    x doesn't have an official sniff header
    - more appropriate for communicating between your own 2 programs, 
        where you know the meaning of the numbers
    x not generally appropriate for international standards support like web3d

http://paulbourke.net/dataformats/volumetric/
The data type is indicated as follows
1 - single bit per cell, two categories
2 - two byes per cell, 4 discrete levels or categories
4 - nibble per cell, 16 discrete levels
8 - one byte per cell (unsigned), 256 levels
16 - two bytes representing a signed "short" integer
32 - four bytes representing a signed integer
The endian is one of
0 for big endian (most significant byte first). For example Motorola processors, Sun, SGI, some HP.
1 for little endian (least significant byte first). For example Intel processors, Dec Alphas.
*/
    int i,j,k,nx,ny,nz, bitsperpixel, bpp, iendian, iret, totalbytes, ipix, nchan;
    // unused int jpix;
    float sx,sy,sz,tx,ty,tz;
    FILE *fp;

    iret = FALSE;

    fp = fopen(fname,"r+b");
    if (fp != NULL) {
        char *rv;
        UNUSED(rv);

        char line [1000];
        rv=fgets(line,1000,fp);
        if(strncmp(line,"vol",3)){
            //for now we'll enforce 'vol' as first the chars of file for sniffing, but not enforcable
            fclose(fp);
            return iret;
        }
        rv=fgets(line,1000,fp);
        sscanf(line,"%d %d %d",&nx,&ny,&nz);
        rv=fgets(line,1000,fp);
        sscanf(line,"%f %f %f",&sx,&sy,&sz);
        rv=fgets(line,1000,fp);
        sscanf(line,"%f %f %f",&tx,&ty,&tz);
        rv=fgets(line,1000,fp);
        sscanf(line,"%d %d",&bitsperpixel,&iendian);
        bpp = bitsperpixel / 8;
        nchan = 1;
        switch(bitsperpixel){
            case 1: nchan = 1; break;
            //1 - single bit per cell, two categories
            case 2: nchan = 4; break;
            //2 - two byes per cell, 4 discrete levels or categories
            case 4: nchan = 1; break;
            //4 - nibble per cell, 16 discrete levels
            case 8: nchan = 1; break;
            //8 - one byte per cell (unsigned), 256 levels
            case 16: nchan = 1; break;
            //16 - two bytes representing a signed "short" integer
            case 32: nchan = 1; break;
            //32 - four bytes representing a signed integer
            default:
                break;
        }

        totalbytes = bpp * nx * ny * nz;
        if(totalbytes < 128 * 128 * 128 *4){
            unsigned char* blob, *rgbablob;
            size_t rvt;
            UNUSED(rvt);

            blob = (unsigned char*)malloc(totalbytes + 4);
            rgbablob = (unsigned char*)malloc(nx * ny * nz * 4);
            memset(rgbablob,0,nx*ny*nz*4);

            rvt=fread(blob,totalbytes,1,fp);
            //now convert to RGBA 4 bytes per pixel
            for(i=0;i<nz;i++){
                for(j=0;j<ny;j++){
                    for(k=0;k<nx;k++){
                        unsigned char *pixel,*rgba;
                        ipix = (i*ny +j)*nx +k; //incoming image 
                        //jpix = (i*ny +(ny-1-j))*nx +  k;
                        pixel = &blob[ipix*bpp];
                        rgba = &rgbablob[ipix*4];
                        rgba[3] = 255;
                        switch(bitsperpixel){
                            case 1: break;
                            //1 - single bit per cell, two categories
                            //rgba[0] = rgba[1] = rgba[2] = 
                            case 2:
                            //2 - two byes per cell, 4 discrete levels or categories
                            rgba[0] = pixel[0] >> 4;
                            rgba[1] = pixel[0] & 0xF;
                            rgba[2] = pixel[1] >> 4;
                            rgba[3] = pixel[1] & 0xF;
                            break;
                            case 4:
                            //4 - nibble per cell, 16 discrete levels
                            break;
                            case 8:
                            //8 - one byte per cell (unsigned), 256 levels
                            rgba[0] = rgba[1] = rgba[2] = (unsigned char)pixel[0];
                            break;
                            case 16:
                            //16 - two bytes representing a signed "short" integer
                            rgba[0] = rgba[1] = rgba[2] = (unsigned char) *(unsigned short*)pixel;
                            break;
                            case 32:
                            //32 - four bytes representing a signed integer
                            rgba[0] = pixel[0]; //too much range, we will split high part into rgb
                            rgba[1] = pixel[1];
                            rgba[2] = pixel[2];
                            rgba[3] = pixel[3]; //and we'll even take a byte as alpha, for fun
                            break;
                            default:
                                break;
                        }
                    }
                }
            }
            if(blob) free(blob);
            tti->channels = nchan;
            tti->x = nx;
            tti->y = ny;
            tti->z = nz;
            tti->texdata = rgbablob;
            iret = TRUE;
        }
        fclose(fp);
    }
    return iret;

}

// NRRD MIT ===========================>>>>>>>>>>>>>>
// a mini-nrrd reader, with MIT licence
// Oct 2, 2016: only luminance / scalar-value-per-voxel implemented, only unsigned char type tested
//#include <endian.h> //windows doesn't have
//#define IS_LITTLE_ENDIAN (1 == *(unsigned char *)&(const int)1)   //found on internet
int isMachineLittleEndian(){
    //dug9: this will/should detect at runtime between big and little endian host machines
    // which reverse byte order- but not mixed endian ie pdp endian
    unsigned short int one = 1;
    unsigned char *c;
    int iret; //, itest;
    c = (unsigned char *)&one;
    iret = (c[0] == 1) ? TRUE : FALSE;
    //itest = IS_LITTLE_ENDIAN ? TRUE : FALSE;
    //if(iret != itest) printf("endian confusion\n");
    return iret;
}

enum {
    NRRDFIELD_type = 1,
    NRRDFIELD_dimension,
    NRRDFIELD_sizes,
    NRRDFIELD_spacing,
    NRRDFIELD_encoding,
    NRRDFIELD_endian,
};

struct {
    const char *fieldname;
    int len;
const int fieldtype;
} nrrdfields [] = {
    {"type:",5,NRRDFIELD_type},
    {"dimension:",10,NRRDFIELD_dimension},
    {"sizes:",6,NRRDFIELD_sizes},
    {"spacings:",9,NRRDFIELD_spacing},
    {"encoding:",9,NRRDFIELD_encoding},
    {"endian:",7,NRRDFIELD_endian},
    {NULL,0,0},
};

enum {
    CDATATYPE_char = 1,
    CDATATYPE_uchar,
    CDATATYPE_short,
    CDATATYPE_ushort,
    CDATATYPE_int,
    CDATATYPE_uint,
    CDATATYPE_longlong,
    CDATATYPE_ulonglong,
    CDATATYPE_float,
    CDATATYPE_double,
};

struct {
    const char * stypes[7];
    const int itype;
    const int bsize;
    const char * fmt;
} nrrddatatypes [] = {
    {{"signed char", "int8_t", "int8",  NULL,NULL,NULL,NULL},    CDATATYPE_char, 1, "%hh"},
    {{"uchar", "unsigned char", "uint8", "uint8_t",    NULL,NULL,NULL}, CDATATYPE_uchar, 1, "%hhu" },
    {{"short", "short int", "signed short", "signed short int", "int16", "int16_t", NULL}, CDATATYPE_short, 2, "%hd" },
    {{"ushort", "unsigned short", "unsigned short int", "uint16", "uint16_t", NULL, NULL}, CDATATYPE_ushort, 2, "%hu" },
    {{"int", "signed int", "int32", "int32_t", NULL, NULL, NULL},    CDATATYPE_int, 4, "%d"},
    {{"uint", "unsigned int", "uint32", "uint32_t", NULL, NULL, NULL}, CDATATYPE_uint, 4, "%u" },
    {{"longlong", "long long", "long long int", "signed long long", "signed long long int", "int64", "int64_t"}, CDATATYPE_longlong, 8, "%lld"},
    {{"ulonglong", "unsigned long long", "unsigned long long int", "uint64", "uint64_t", NULL, NULL}, CDATATYPE_ulonglong, 8, "%llu" },
    {{"float", NULL,NULL,NULL,NULL, NULL,NULL},CDATATYPE_float,4, "%f" },
    {{"double", NULL,NULL,NULL,NULL, NULL,NULL},CDATATYPE_double,8, "%lf"},
    {{NULL,NULL,NULL,NULL, NULL,NULL,NULL},0, 0, ""},
};

enum {
    NRRDENDIAN_LITTLE = 1,
    NRRDENDIAN_BIG,
};

enum {
    NRRDENCODING_RAW = 1,
    NRRDENCODING_ASCII,
};

static int sniffImageFileHeader(char *filename) {
// return value:
// 0 unknown
// 1 png
// 2 jpeg
// 3 gif
//filenames coming in can be temp file names - scrambled
//there are 3 ways to tell in the backend what type of image file:
//a) .xxx original filename suffix
//b) MIME type 
//c) file signature https://en.wikipedia.org/wiki/List_of_file_signatures
// right now we aren't passing in the .xxx or mime or signature bytes
// except through the file conents we can get the signature
    char header[20];
    int iret;
    size_t rvt;
    UNUSED(rvt);

    FILE* fp = fopen(filename,"rb");
    if (fp == NULL)
        return -1;
    rvt=fread(header,20,1,fp);
    fclose(fp);

    iret = IMAGETYPE_UNKNOWN;
    if(!strncmp(&header[1],"PNG",3))
        iret = IMAGETYPE_PNG;

    if(!strncmp(header,"ÿØÿ",3)) //JPEG
        iret = IMAGETYPE_JPEG;

    if(!strncmp(header,"GIF",3))
        iret = IMAGETYPE_GIF;

    if(!strncmp(header,"DDS ",4)) // MS .dds cubemap and 3d textures
        iret = IMAGETYPE_DDS;

    if(!strncmp(header,"web3dit",7)) //.web3dit dug9/freewrl invention
        iret = IMAGETYPE_WEB3DIT;

    if(!strncmp(header,"NRRD",4))  //.nrrd 3D volume texture
        iret = IMAGETYPE_NRRD;

    if(!strncmp(header,"vol",3)) //.vol 3D volume
        iret = IMAGETYPE_VOL;

    return iret;
}

int loadImage_nrrd(struct textureTableIndexStruct *tti, char *fname){
/*
    license on this function: MIT or equivalent
    volume / 3D images, for VolumeRendering and Texturing3D components
    http://teem.sourceforge.net/nrrd/format.html
    subset implemented here: assumes luminance-only (scalar per voxel)
    (see the kinds[] field for more general interpretation of the rows)
    we will skip non-basic / optional fields and key/values since we aren't also writing back out in a full cycle
    The headers are ascii, and you can type the file to the console to see the header
C:>type brain.nrrd
NRRD0001
content: "MRI Brain for 3DVisualizer"
type: unsigned char
dimension: 3
sizes: 512 512 230
spacings: 1 1 0.4
encoding: raw

C:>type supine.nrrd
NRRD0001
content: "Torso Supine"
type: unsigned short
dimension: 3
sizes: 512 512 426
spacings: 1 1 1
endian: little
encoding: raw

a few sample images have degenerate first dimension
NRRD0001
type: unsigned char
dimension: 4
sizes: 1 256 256 124
spacings:  NaN 0.01 0.01 0.01
encoding: raw


*/
    int iret;
    FILE *fp;
    iret = FALSE;

    fp = fopen(fname,"r+b"); //need +b for binary mode, to read over nulls
    if (fp != NULL) {
        unsigned long long i,j,k;
        int ifieldtype, idatatype;
        // int kdatatype;
        int idim, isize[4], iendian, iencoding, ifound,slen,klen, bsize;
        long long ilen;
        char line [2048];
        char cendian[256], cencoding[256];
        char *remainder;
        const char *fmt;
        unsigned long long nvoxel;
        unsigned long long totalbytes;
        unsigned char *data;
        unsigned char *voxel;
        double dhi, dlo; 
        double d255range;
        int counts[256]; //histogram
        char *rv;
        UNUSED(rv);

        dhi=0.0; dlo=0.0;

        rv=fgets(line,2047,fp);
        if(strncmp(line,"NRRD",4)){
            //not our type
            fclose(fp);
            return iret;
        }

        fmt = "";
        iendian = 0;// NRRDENDIAN_LITTLE;
        idim = 0; //3;
        idatatype = 0; // CDATATYPE_int;
        isize[0] = isize[1] = isize[2] = isize[3] = 0;
        iencoding = 0; //NRRDENCODING_RAW;
        bsize = 1; //binary size of voxel, in bytes, for mallocing
        // kdatatype = 0; //index into nrrddatatypes array
        //read header field, one per loop:
        for(;;){
            rv=fgets(line,2047,fp);
            i = 0;
            ifieldtype = 0; //unknown
            ilen = 0; //length of field string
            if(strlen(line) < 3){
                // '...the data (following the blank line after the header) ..'
                break; //nrrd signals end of header with blank line ie \n' or \r\n
            }

            //see if we have a matching field from the sub-list that we care about 
            for(;;){
                if(!nrrdfields[i].fieldname)break;
                if(!strncmp(line,nrrdfields[i].fieldname,nrrdfields[i].len)){
                    ifieldtype = nrrdfields[i].fieldtype;
                    ilen = nrrdfields[i].len;
                    break;
                }
                i++;
            }
            remainder = &line[ilen];
            switch(ifieldtype){
                case NRRDFIELD_type:
                    //find first non-blank byte where it starts ie 'unsigned short int'
                    for(i=0;i<10;i++){
                        if(remainder[0] == ' ') remainder = &remainder[1];
                        else break;
                    }
                    slen = strlen(remainder);
                    //find last non-blank, non CRLF 
                    klen = slen;
                    for(long long i=0;i<slen;i++){
                        char c = remainder[slen-1 -i];
                        if(c == '\n' || c == '\r' || c == ' ') klen--;
                        else break;
                    }
                    //compare known types to the full remainder string ie "unsigned short int"
                    k = 0;
                    ifound = FALSE;
                    for(;;){
                        if(nrrddatatypes[k].itype == 0) break;
                        for(j=0;j<7;j++){
                            if(nrrddatatypes[k].stypes[j]){  //some are null
                                if(!strncmp(remainder,nrrddatatypes[k].stypes[j],klen)){
                                    ifound = TRUE;
                                    idatatype = nrrddatatypes[k].itype;
                                    //kdatatype = (int)k;
                                    bsize = nrrddatatypes[k].bsize;
                                    fmt = nrrddatatypes[k].fmt;
                                    break; //break out of 0,7 loop
                                }
                            }
                        }
                        if(ifound) break;
                        k++;
                    }
                    break;
                case NRRDFIELD_dimension:
                    sscanf(remainder,"%d",&idim);
                    idim = min(4,idim); //we can't use more yet ie a time-varying 3D image or separate R,G,B or X,Y,Z per voxel - just scalar per voxel
                    break;
                case NRRDFIELD_sizes:
                    switch(idim){
                        case 1:
                            sscanf(remainder,"%d",&isize[0]);break;
                        case 2:
                            sscanf(remainder,"%d%d",&isize[0],&isize[1]);break;
                        case 3:
                            sscanf(remainder,"%d%d%d",&isize[0],&isize[1],&isize[2]);break;
                        case 4:
                            sscanf(remainder,"%d%d%d%d",&isize[0],&isize[1],&isize[2],&isize[3]);break;
                        default:
                            break;
                    }
                    break;
                case NRRDFIELD_encoding:
                    sscanf(remainder,"%s",cencoding);
                    if(!strcmp(cencoding,"raw"))
                        iencoding = NRRDENCODING_RAW;
                    else if(!strcmp(cencoding,"ascii"))
                        iencoding = NRRDENCODING_ASCII;
                    break;

                    break;
                case NRRDFIELD_endian:
                    sscanf(remainder,"%s",cendian);
                    if(!strcmp(cendian,"little"))
                        iendian = NRRDENDIAN_LITTLE;
                    else if(!strcmp(cendian,"big"))
                        iendian = NRRDENDIAN_BIG;
                    break;
                //we may need kinds[] which say how to interpret the scalars, otherwise limited to scalar-per-voxel
                //range field? would be helpful when compressing voxel significant bits into displayable unsigned char range
                default:
                    //skip fields and key/value stuff we dont need or care about for our display app
                    break;    
            }
        }
        if(0){
            printf("iendian %d idatatype %d iencoding %d idim %d isizes %d %d %d %d bsize %d\n",
                    iendian,idatatype,iencoding,idim,isize[0],isize[1],isize[2],isize[3], bsize);
            printf("machine endian isLittle=%d\n",isMachineLittleEndian());
            printf("hows that?\n");

        }
        //clean up dimensions
        if(isize[0] == 1){
            //remove degenerate dimension, found in some images
            for(long long i=0;i<idim-1;i++){
                isize[i] = isize[i+1];
                //spacing[i] = spacing[i+1];
            }
            idim--;
        }
        if(idim <3) isize[2] = 1;
        if(idim <2) isize[1] = 1;
        if(idim >3) {
            idim = 3; //as of oct 3, 2016 we just do scalar / iso-value 3D images, not color, not time-series, not xyz
        }
        
        //malloc data buffer
        nvoxel = isize[0] * isize[1] * isize[2];
        totalbytes = nvoxel * bsize;
        data = MALLOC(unsigned char *,(size_t)totalbytes);
        memset(data,4,(size_t)totalbytes);
        voxel = MALLOC(unsigned char *, bsize);
        //read data
        if(iencoding == NRRDENCODING_RAW){
            int dataLittleEndian;
            size_t element_size = 0L;
            element_size = bsize;
            fread(data,element_size, (size_t)nvoxel,fp);
//            printf("num elems read = %llu elemsize %ld bytes requeted = %llu %llu\n",(unsigned long long)nelem_read,(long)bsize,bsize*nvoxel,totalbytes);
            //endian conversion
            dataLittleEndian = iendian == NRRDENDIAN_LITTLE ? TRUE : FALSE;
            if(isMachineLittleEndian() != dataLittleEndian && bsize > 1){
                //data endian doesnt match machine endian - swap unconditionally
//                printf("swapping endian\n");
                for(i=0;i<nvoxel;i++){
                    unsigned char * voxel = &data[i*bsize];
                    for(int j=0;j<bsize/2;j++){
                        char c;
                        k = bsize -1 - j;
                        c = voxel[j];
                        voxel[j] = voxel[k];
                        voxel[k] = c;
                    }
                }
            }
        }else if(iencoding == NRRDENCODING_ASCII){
            int kvox = 0;
            //read all slices
            for(long long i=0;i<isize[2];i++){
                //read a slice
                for(int j=0;j<isize[1];j++){
                    //read a row
                    for(int k=0;k<isize[0];k++){
                        int rvi;
                        UNUSED(rvi);

                        //read a voxel - unfamiliar theory/method, dont trust
                        rvi=fscanf(fp,fmt,voxel);
                        //put voxel in data
                        memcpy(&data[kvox*bsize],voxel,bsize);
                    }
                }
            }
        }
        //we have binary data in voxel datatype described in file
        //currently (Oct 2, 2016) this function assumes scalar-per-voxel aka luminance or alpha

        //find range of data so we can compress range into unsigned char range 0-255 from much bigger ints and floats
        //initialize range - use maxint, minint or just init to first pixel which we do here
        voxel = &data[0];
        switch(idatatype){
            case CDATATYPE_char: 
                dlo = -127.0;
                dhi = 127.0; //or is it 128?
            break;
            case CDATATYPE_uchar: 
                dlo = 0.0;
                dhi = 255.0;
            break;
            case CDATATYPE_short: 
                dlo = dhi = (double) *(short*)(voxel);
            break;
            case CDATATYPE_ushort: 
                dlo = dhi = (double) *(unsigned short*)(voxel);
                printf("initial range for ushort hi %lf lo %lf\n",dhi,dlo);
            break;
            case CDATATYPE_int: 
                dlo = dhi = (double) *(long*)(voxel);
            break;
            case CDATATYPE_uint: 
                dlo = dhi = (double) *(unsigned long*)(voxel);
            break;
            case CDATATYPE_longlong: 
                dlo = dhi = (double) *(long long *)(voxel);
            break;
            case CDATATYPE_ulonglong: 
                dlo = dhi = (double) *(unsigned long long *)(voxel);
            break;
            case CDATATYPE_float: 
                dlo = dhi = (double) *(float*)(voxel);
            break;
            case CDATATYPE_double: 
                dlo = dhi = *(double*)(voxel);
            break;
            default:
                break;
        }
        //find lower and upper of range by looking at every value
        for(i=0;i<nvoxel;i++){
            unsigned char *voxel;
            //unsigned char A;
            // unused unsigned char *rgba = &tti->texdata[i*4];
            //LUM-ALPHA with RGB=1, A= voxel scalar
            voxel = &data[i*bsize];
            switch(idatatype){
                case CDATATYPE_char: 
                    dlo = min(dlo,(double)*(char*)(voxel));
                    dhi = max(dhi,(double)*(char*)(voxel));
                break;
                case CDATATYPE_uchar: 
                    dlo = min(dlo,(double)*(unsigned char*)(voxel));
                    dhi = max(dhi,(double)*(unsigned char*)(voxel));
                break;
                case CDATATYPE_short: 
                    dlo = min(dlo,(double)*(short*)(voxel));
                    dhi = max(dhi,(double)*(short*)(voxel));
                break;
                case CDATATYPE_ushort: 
                    dlo = min(dlo,(double)*(unsigned short*)(voxel));
                    dhi = max(dhi,(double)*(unsigned short*)(voxel));
                break;
                case CDATATYPE_int: 
                    dlo = min(dlo,(double)*(long*)(voxel));
                    dhi = max(dhi,(double)*(long*)(voxel));
                break;
                case CDATATYPE_uint: 
                    dlo = min(dlo,(double)*(unsigned long*)(voxel));
                    dhi = max(dhi,(double)*(unsigned long*)(voxel));
                break;
                case CDATATYPE_longlong: 
                    dlo = min(dlo,(double)*(unsigned long long*)(voxel));
                    dhi = max(dhi,(double)*(unsigned long long*)(voxel));
                break;
                case CDATATYPE_ulonglong: 
                    dlo = min(dlo,(double)*(unsigned long*)(voxel));
                    dhi = max(dhi,(double)*(unsigned long*)(voxel));
                break;
                case CDATATYPE_float: 
                    dlo = min(dlo,(double)*(float*)(voxel));
                    dhi = max(dhi,(double)*(float*)(voxel));
                break;
                case CDATATYPE_double: 
                    dlo = min(dlo,(double)*(double*)(voxel));
                    dhi = max(dhi,(double)*(double*)(voxel));
                break;
                default:
                    break;
            }
        }
        d255range = 255.0/(dhi - dlo); 
        if(0) printf("nrrd image voxel range hi %lf lo %lf 255range scale factor %lf\n",dhi,dlo,d255range);
        //now convert to display usable data type which currently is RGBA
        tti->texdata = MALLOC(unsigned char *,(size_t)nvoxel * 4); //4 for RGBA
        tti->channels = 1; //1=lum 2=lum-alpha 3=rgb 4=rgba //doing 2-channel allows modulation of material color
            //Oct 16, 2016: in textures.c we now compute gradient automatically and put in RGB, if channels == 1 and z > 1
        tti->hasAlpha = TRUE;
        tti->x = isize[0];
        tti->y = isize[1];
        tti->z = isize[2];
        memset(counts,0,256*sizeof(int));
        for(i=0;i<nvoxel;i++){
            unsigned char *voxel;
            unsigned char A;
            unsigned char *rgba = &tti->texdata[i*4];
            //LUM-ALPHA with RGB=1, A= voxel scalar

            A = '\0';
            voxel = &data[i*bsize];
            if(1){
                //no range-scale method - might be needed for experiments
                switch(idatatype){
                    case CDATATYPE_char: 
                        A = (char)(voxel[0]) + 127; //convert from signed char to unsigned
                    break;
                    case CDATATYPE_uchar: 
                        A = voxel[0];
                    break;
                    case CDATATYPE_short: 
                        A = (unsigned char) ((*(short *)voxel) / 255) + 127; //scale into uchar range, assumes short range is fully used
                    break;
                    case CDATATYPE_ushort: 
                        {
                        //static unsigned short lastushort = 1;
                        unsigned short thisushort;
                        memcpy(&thisushort,voxel,bsize);
                        //thisushort = *(unsigned short*)voxel;
                        //A = (unsigned char) ((*(unsigned short *)voxel) / 255); //scale into uchar range, "
                        //A = (*(unsigned short *)voxel) >> 8;
                        //A = ((*(unsigned short *)voxel) << 8) >> 8;
                        //A = (unsigned char) abs(*(unsigned short *)voxel)/256;
                        thisushort /= 256;
                        //if(thisushort != lastushort)
                        //    printf("%d ", (int)thisushort);
                        counts[thisushort]++;
                        //lastushort = thisushort;
                        A = (unsigned char) thisushort;
                        }
                    break;
                    case CDATATYPE_int: 
                        A = (unsigned char)((*((long *)voxel))/65536/255 + 127);
                    break;
                    case CDATATYPE_uint: 
                        A = (unsigned char) ((*((unsigned long *)voxel))/65536/255);
                    break;
                    case CDATATYPE_longlong: 
                        A = (unsigned char) ((*((long long *)voxel))/65536/65536/255 + 127);
                    break;
                    case CDATATYPE_ulonglong: 
                        A = (unsigned char) ((*((unsigned long long *)voxel))/65536/65536/255);
                    break;
                    //case CDATATYPE_float: 
                    //    A = (unsigned char) ((int)((*((float *)voxel))/range + range/2.0f) + 127) ;
                    //break;
                    //case CDATATYPE_double: 
                    //    A = (unsigned char) ((int)((*((double *)voxel))/range + range/2.0f) + 127) ;
                    //break;
                    default:
                        break;
                }
            } else {
                //range scaling method
                double dtemp; //, dtemp2;
                //unsigned int lutemp;
                //unsigned short utemp;
                //unsigned char uctemp;

                switch(idatatype){
                    case CDATATYPE_char: 
                        A = (unsigned char)((int)(voxel[0])) + 127; //convert from signed char to unsigned
                    break;
                    case CDATATYPE_uchar: 
                        A = voxel[0];
                    break;
                    case CDATATYPE_short: 
                        dtemp = (double)(*(short *)voxel);
                        A = (unsigned char)(unsigned short)(unsigned int)((dtemp - dlo)*d255range);
                    break;
                    case CDATATYPE_ushort: 
                        dtemp = (double)(*(unsigned short *)voxel);
                        //dtemp2 = (dtemp - dlo)*d255range;
                        //lutemp = (unsigned int)dtemp2;
                        //utemp = (unsigned short)lutemp;
                        //uctemp = (unsigned char)utemp;
                        //A = uctemp;
                        //tip: get it into 0-255 range while still double, then cast to uchar
                        A = (unsigned char)(unsigned short)(unsigned int)((dtemp - dlo)*d255range);
                        //A = (unsigned char)(unsigned short)(unsigned int)dtemp2;
                        //printf("[%lf %lu %u %d]  ",dtemp2,lutemp,utemp,(int)uctemp);
                    break;
                    case CDATATYPE_int: 
                        dtemp = (double)(*(long *)voxel);
                        A = (unsigned char)(unsigned short)(unsigned int)((dtemp - dlo)*d255range);
                    break;
                    case CDATATYPE_uint: 
                        dtemp = (double)(*(unsigned long *)voxel);
                        A = (unsigned char)(unsigned short)(unsigned int)((dtemp - dlo)*d255range);
                    break;
                    case CDATATYPE_longlong: 
                        dtemp = (double)(*(long long *)voxel);
                        A = (unsigned char)(unsigned short)(unsigned int)((dtemp - dlo)*d255range);
                    break;
                    case CDATATYPE_ulonglong: 
                        dtemp = (double)(*(unsigned long long *)voxel);
                        A = (unsigned char)(unsigned short)(unsigned int)((dtemp - dlo)*d255range);
                    break;
                    case CDATATYPE_float: 
                        dtemp = (double)(*(float *)voxel);
                        A = (unsigned char)(unsigned short)(unsigned int)((dtemp - dlo)*d255range);
                    break;
                    case CDATATYPE_double: 
                        dtemp = (double)(*(double *)voxel);
                        A = (unsigned char)(unsigned short)(unsigned int)((dtemp - dlo)*d255range);
                    break;
                    default:
                        break;
                }
                counts[(int)A]++; //histogram accumulation
            }
            //this displays nice in texturing3D as 'white bones x-ray'
            int white = 255;
            rgba[0] = white;
            rgba[1] = white;
            rgba[2] = white;
            rgba[3] = A / 2;
       }
        //print histogram to console
        if(0) for(i=0;i<256;i++)
            if(counts[i] != 0) 
                printf("counts[%ld]=%ld\n",(long)i,(long)counts[i]);

        FREE_IF_NZ(data); //free the raw data we malloced, now that we have rgba, unless we plan to do more processing on scalar values later.
    }
    return TRUE;

}
//<<< NRRD MIT ================================

/**
 *   texture_load_from_file: a local filename has been found / downloaded,
 *                           load it now.
 */
int textureIsDDS(textureTableIndexStruct_s* this_tex, char *filename); 
bool 
NodeImageTexture3D::texture_load_from_file(textureTableIndexStruct_s* this_tex,
                                           const char *filename)
{
/* WINDOWS */
#if defined (_MSC_VER) 
    char *fname;
    int ret, imtype;

    fname = STRDUP(filename);
    imtype = sniffImageFileHeader(fname);

    ret = FALSE;
    switch(imtype){
/*
        case IMAGETYPE_PNG:
        case IMAGETYPE_JPEG:
        case IMAGETYPE_GIF:
            ret = loadImage(this_tex, fname); 
            #ifndef GL_ES_VERSION_2_0
            texture_swap_B_R(this_tex); //just for windows desktop gdiplusimage loading
            #endif
            {
                int nchan;
                if(imtype == IMAGETYPE_JPEG){
                    nchan = 3; //jpeg always rgb, no alpha
                }else{
                    nchan = sniffImageChannels_bruteForce(this_tex->texdata, this_tex->x, this_tex->y); 
                }
                if(nchan > -1) this_tex->channels = nchan;
            }
            break;
*/
        case IMAGETYPE_DDS:
            ret = textureIsDDS(this_tex, fname); break;
        case IMAGETYPE_WEB3DIT:
            ret = loadImage_web3dit(this_tex,fname); break;
        case IMAGETYPE_NRRD:
            ret = loadImage_nrrd(this_tex,fname); break;
        case IMAGETYPE_VOL:
            ret = loadImage3DVol(this_tex, fname); break;
        case IMAGETYPE_UNKNOWN:
        default:
            ret = FALSE;
    }

#endif


/* LINUX */

#if !defined (_MSC_VER) && !defined(_ANDROID) && !defined(ANDROIDNDK)
    char *fname;
    int ret, imtype;

    fname = STRDUP(filename);
    imtype = sniffImageFileHeader(fname);
    ret = FALSE;

    switch(imtype){
        case IMAGETYPE_DDS:
            ret = textureIsDDS(this_tex, fname); break;
        case IMAGETYPE_WEB3DIT:
            ret = loadImage_web3dit(this_tex,fname); break;
        case IMAGETYPE_NRRD:
            ret = loadImage_nrrd(this_tex,fname); break;
        case IMAGETYPE_VOL:
            ret = loadImage3DVol(this_tex, fname); break;
        default:
            break;
    }




#endif

    m_imageSizeX = this_tex->x;
    m_imageSizeY = this_tex->y;
    m_imageSizeZ = this_tex->z;

    m_imageData = new int[m_imageSizeX * m_imageSizeY * m_imageSizeZ];
        
    int bytes = m_imageSizeX * m_imageSizeY * m_imageSizeZ;
    for (int k = 0; k < bytes; k++) {
         m_imageData[k] = this_tex->texdata[k * 4 + 0] + 
                          this_tex->texdata[k * 4 + 1] * 256 +
                          this_tex->texdata[k * 4 + 2] * 256 * 256 + 
                          this_tex->texdata[k * 4 + 3] * 256 * 256 * 256;
    }                             
    FREE(fname);
    return (ret);

    return FALSE;
}

/*


Texturing during Runtime 
texture enabling

*/

void *RenderTextures_constructor(){
    void *v = MALLOCV(sizeof(struct pRenderTextures));
    memset(v,0,sizeof(struct pRenderTextures));
    return v;
}

void RenderTextures_init(struct tRenderTextures *t){
    textures_prv = (ppTextures)RenderTextures_constructor();
    t->prv = textures_prv;
}

void
NodeImageTexture3D::load()
{
    if (!m_loaded) {
        if (m_imageData)
            delete [] m_imageData;
        MFString *urls = url();
        MyString baseURL = "";
        baseURL += TheApp->getImportURL();
        bool validPath = false;
        MyString path;
        for (int i = 0; i < urls->getSize(); i++) {
            URL urlI(baseURL, urls->getValue(i));
            if (urls->getValue(i).length() == 0) continue;
            if (m_scene->Download(urlI, &path)) {
                validPath = true;
                break;
            }
        }
        if (!validPath)
            return;
        RenderTextures_init((tRenderTextures*)&textures_prv);
        registerTexture(this);
        m_textures_prv = textures_prv;
        int textureNumber = m_textureTableIndex;
        texture_load_from_file(getTableIndex(textureNumber), 
                               (const char *)path);
        m_loaded = true;
    }
}

void
NodeImageTexture3D::draw(int pass)
{
    if (pass == RENDER_PASS_NON_TRANSPARENT)
        return;
  
    drawTexture3D((Node *)this, m_scene);
}

void    
NodeImageTexture3D::setField(int index, FieldValue *value,    
                             int containerField)
{
    m_loaded = false;
    Node::setField(index, value, containerField);
}


