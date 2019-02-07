/*
 * NodeImageTexture3D.cpp
 *
 * Copyright (C) 2009 J. "MUFTI" Scheurich
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

#include "NodeImageTexture3D.h"
#include "Proto.h"
#include "FieldValue.h"
#include "MFString.h"
#include "SFBool.h"
#include "SFBool.h"
#include "SFBool.h"
#include "SFNode.h"
#include "DuneApp.h"
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
    m_textureTableIndex = 0;
    m_nodeNeedsCompiling = true;
    m_textures_prv = NULL;
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
****************************************************************************/

#if 0
code not ready...
//#ifdef HAVE_LIBIMLIB2

#include <Imlib2.h>
#include "Texture3D.h"

ppTextures textures_prv = NULL;

struct multiTexParams {
    int multitex_mode[2];
    int multitex_source[2];
    int multitex_function;
};

typedef struct pRenderTextures{
    struct multiTexParams textureParameterStack[MAX_MULTITEXTURE];
}* ppRenderTextures;

static int _noisy = 0;
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
        int istart, iend;
        istart = me->allocn;
        if(me->allocn)
        {
            me->allocn*=2;
        }
        else
        {
            me->allocn=1;
            me->n = 0;
        }
        iend = me->allocn;
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
            FW_GL_DELETETEXTURES(1, &ti->OpenGLTexture);
            ti->OpenGLTexture = TEXTURE_INVALID;
    /*         FREE_IF_NZ(ti->OpenGLTexture); */
        }
    }
}

/* is this node a texture node? if so, lets keep track of its textures. */
/* worry about threads - do not make anything reallocable */
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

/**
 *   texst: texture status string.
 */
const char *texst(int num)
{
    if (num == TEX_NOTLOADED) return "TEX_NOTLOADED";
    if (num == TEX_LOADING) return "TEX_LOADING";
    if (num == TEX_NEEDSBINDING)return "TEX_NEEDSBINDING";
    if (num == TEX_LOADED)return "TEX_LOADED";
    if (num == TEX_UNSQUASHED)return "UNSQUASHED";
    return "unknown";
}

/**
 * ml_prev: returns item preceding 'item' in 'list'
 * or NULL if either 'item' is null or 'item' cannot be found
 * in forward search from 'list'
 */
s_list_t* ml_prev(s_list_t *list, s_list_t *item)
{
    s_list_t *n;
    if (!item) return NULL;
    while (list) {
        n = ml_next(list);
        if (!n)
            return NULL;
        if (n == item)
            return list;
        list = n;
    }
    return NULL;
}

/**
 * ml_insert: inserts item 'item' before 'point' in 'list'
 */
s_list_t* ml_insert(s_list_t *list, s_list_t *point, s_list_t *item)
{
    if (!list) {
        if (item) ml_next(item) = point;
        return item;
    }
    if (!point || (list == point)) {
        if (item) ml_next(item) = list;
    } else {
        s_list_t *prev;
        prev = ml_prev(list, point);
        if (prev) {
                ml_next(prev) = item;
                ml_next(item) = point;
        } else {
                item = NULL;
        }
    }
    return item;
}

void ml_enqueue(s_list_t **list, s_list_t *item)
{
	*list = ml_insert(*list,*list,item);
}

void threadsafe_enqueue_item_signal(s_list_t *item, s_list_t** queue)
{
//    pthread_mutex_lock(queue_lock);
//    if (*queue == NULL)
//        pthread_cond_signal(queue_nonzero);
    ml_enqueue(queue,item);
//    pthread_mutex_unlock(queue_lock);
}

void texitem_enqueue(s_list_t *item){
    ppLoadTextures p;
//    ttglobal tg = gglobal();
    p = (ppLoadTextures)textures_prv;

    threadsafe_enqueue_item_signal(item, &p->texture_request_list);
}

static int sniffImageChannels_bruteForce(unsigned char *imageblob, int width, int height){
    //iterates over entire 4byte-per-pixel RGBA image blob, or until it knows the answer,
    // and returns number of channels 1=Luminance, 2=Lum-alpha 3=rgb 4=rgba
    //detects by comparing alpha != 1 to detect alpha, and r != g != b to detect color
    int i,ii4,j,jj4, hasAlpha, hasColor, channels;
    hasAlpha = 0;
    hasColor = 0;
    channels = 4;
    for(i=0;i<height;i++){
        ii4 = i*width*4;
        if(!hasColor){
            //for gray-scale images, will need to scan the whole image looking for r != g != b
            //not tested with lossy compression ie jpeg, but jpeg is usually RGB -not gray, and no alpha- 
            // - so jpeg should exit color detection early anyway
            for(j=0;j<width;j++){
                jj4 = ii4 + j*4;
                hasAlpha = hasAlpha || imageblob[jj4+3] != 255;
                hasColor = hasColor || imageblob[jj4] != imageblob[jj4+1] || imageblob[jj4+1] != imageblob[jj4+2];
            }
        }else{
            //color found, can stop looking for color. now just look for alpha
            //- this is likely the most work, because if Alpha all 1s, it won't know until it scans whole image
            for(j=3;j<width*4;j+=4){
                hasAlpha = hasAlpha || imageblob[ii4 + j] != 255;
            }
        }
        if(hasAlpha && hasColor)break; //got the maximum possible answer, can exit early
    }
    channels = hasColor ? 3 : 1;
    channels = hasAlpha ? channels + 1 : channels;
    return channels;
}

static void texture_swap_B_R(textureTableIndexStruct_s* this_tex)
{
    //swap red and blue // BGRA - converts back and forth from BGRA to RGBA 
    //search for GL_RGBA in textures.c
    int x,y,z,i,j,k,ipix,ibyte;
    unsigned char R,B,*data;
    x = this_tex->x;
    y = this_tex->y;
    z = this_tex->z;
    data = this_tex->texdata;
    for(i=0;i<z;i++){
        for(j=0;j<y;j++){
            for(k=0;k<x;k++)
            {
                ipix = (i*y + j)*x + k;
                ibyte = ipix * 4; //assumes tti->texdata is 4 bytes per pixel, in BGRA or RGBA order
                R = data[ibyte];
                B = data[ibyte+2];
                data[ibyte] = B;
                data[ibyte+2] = R;
            }
        }
    }
}

int iclamp(int ival, int istart, int iend) {
     int iret = ival;
     iret = ival > iend? iend : ival;
     iret = iret < istart ? istart : iret;
     return iret;
}

void compute_3D_alpha_gradient_store_rgb(unsigned char *dest,int x,int y, int z){
     //assumes we have a scalar image with info only in alpha, but (unused) RGB channels
     //we compute 3D alpha/scalar gradient using one of sobel, roberts ...
     //gradient has magnitude and direction (vs normal, which is of unit length)
     //here we do an axis-aligned roberts ie gradient_x = x1 - x0
     int iz,iy,ix, jz,jy,jx,jzz,jyy,jxx, k;
     char *rgba0, *rgba1;
     int gradient[3], maxgradient[3], mingradient[3];
     unsigned char *urgba;
     uint32 *pixels = (uint32 *)dest;

     for(k=0;k<3;k++) {
          maxgradient[k] = -1;
          mingradient[k] = 1;
     }


     for(iz=0;iz<z;iz++){
          for(iy=0;iy<y;iy++){
               for(ix=0;ix<x;ix++){
                    //initialize gradient
                    for(k=0;k<3;k++) gradient[k] = 0;
                    rgba0 = (char *) &pixels[(iz*y +iy)*x + ix];
                    urgba = (unsigned char *)rgba0;
                    if(1){
                         //sum onto gradient
                         jxx = jyy = jzz = 0;
                         //what if we are on the edge? for roberts, just duplicate next-to-edge by backing up one
                         if(iz == z-1) jzz = -1;
                         if(iy == y-1) jyy = -1;
                         if(ix == x-1) jxx = -1;
                         jx = jxx; jy = jyy; jz = jzz;
                         jx = jxx + 1;
                         rgba1 = (char *) &pixels[((iz+jz)*y +(iy+jy))*x + (ix+jx)];
                         gradient[0] = (int)rgba1[3] - (int)rgba0[3];
                         jx = jxx;
                         jy = jyy+1;
                         rgba1 = (char *) &pixels[((iz+jz)*y +(iy+jy))*x + (ix+jx)];
                         gradient[1] = (int)rgba1[3] - (int)rgba0[3];
                         jy = jyy;
                         jz = jzz+1;
                         rgba1 = (char *) &pixels[((iz+jz)*y +(iy+jy))*x + (ix+jx)];
                         gradient[2] = (int)rgba1[3] - (int)rgba0[3];
                    }else {
                         //extract edge-clamped 3x3x3
                         //       X  Y  Z with [1] in center
                         int cube[3][3][3], i,j,ii,jj,kk;
                         for(i=-1;i<2;i++){
                              ii = iclamp(ix+i,0,x-1);
                              for(j=-1;j<2;j++){
                                   jj= iclamp(iy+j,0,y-1);
                                   for(k=-1;k<2;k++){
                                        kk = iclamp(iz+k,0,z-1);
                                        cube[i+1][j+1][k+1] = ((unsigned char *)&pixels[(kk*y +jj)*x + ii])[3];
                                   }
                              }
                         }
                         if(1){
                              //roberts cross
                              //gradient[0] = (cube[2][2][2] - cube[1][1][1]) + (cube[2][0][0] - cube[1][1][1];
                         }
                         if(1){
                              //sobel gradient
                              gradient[0] = 0;
                              gradient[0] += cube[0][0][1] + 2*cube[0][1][1] + cube[0][2][1];
                              gradient[0] -= cube[2][0][1] + 2*cube[2][1][1] + cube[2][2][1];
                              gradient[0] += cube[0][1][0] + 2*cube[0][1][1] + cube[0][1][2];
                              gradient[0] -= cube[2][1][0] + 2*cube[2][1][1] + cube[2][1][2];

                              gradient[1] = 0;
                              gradient[1] += cube[1][0][0] + 2*cube[1][0][1] + cube[1][0][2];
                              gradient[1] -= cube[1][2][0] + 2*cube[1][2][1] + cube[1][2][2];
                              gradient[1] += cube[0][0][1] + 2*cube[1][0][1] + cube[2][0][1];
                              gradient[1] -= cube[9][2][1] + 2*cube[1][2][1] + cube[2][2][1];

                              gradient[2] = 0;
                              gradient[2] += cube[0][1][0] + 2*cube[1][1][0] + cube[2][1][0];
                              gradient[2] -= cube[0][1][2] + 2*cube[1][1][2] + cube[2][1][2];
                              gradient[2] += cube[1][0][0] + 2*cube[1][1][0] + cube[1][2][0];
                              gradient[2] -= cube[1][9][2] + 2*cube[1][1][2] + cube[1][2][2];
                              for(k=0;k<3;k++)
                                   gradient[k] /= 2;

                         }
                    }

                    //scale gradient to -127 to +127 in each dimension
                    //roberts: a1 - a0 could be in range (255 - 0) to (0 -255) or -255 to 255,
                    // we need -127 to 127 signed char on each dim
                    for(k=0;k<3;k++) gradient[k] /= 2;
                    for(k=0;k<3;k++) {
                         maxgradient[k] = max(maxgradient[k],gradient[k]);
                         mingradient[k] = min(mingradient[k],gradient[k]);
                    }

                    //but when texture2D / sampler2D convert from image pixel to float,
                    //the expect the pixels to be unsigned char.
                    //so we add 127 here, and subtract .5 in the shader, once they are float
                    for(k=0;k<3;k++) gradient[k] += 127;

                    //set gradient in RGB channels
                    for(k=0;k<3;k++) urgba[k] = (unsigned char)gradient[k];
                    //if(rgba0[0] || rgba0[1] || rgba0[2]){
                    //     if(rgba0[0] != rgba0[1] || rgba0[1] != rgba0[2])
                    //          printf("[%d %d %d]",(int)rgba0[0],(int)rgba0[1],(int)rgba0[2]);
                    //}
               }
          }
     }
     if(0){
     printf("mingradient %d %d %d\n",mingradient[0],mingradient[1],mingradient[2]);
     printf("maxgradient %d %d %d\n",maxgradient[0],maxgradient[1],maxgradient[2]);
     }
     if(0){
          //save gradient image for testing
          textureTableIndexStruct_s *tti2, tt;
          tti2 = &tt;
          tti2->x = x;
          tti2->y = y;
          tti2->z = z;
          tti2->texdata = (unsigned char *)dest;
          tti2->channels = 3;
//          saveImage_web3dit(tti2,"gradientRGB.web3dit");
          tti2->channels = 4;
//          saveImage_web3dit(tti2,"gradientRGBA.web3dit");

     }

}

static void myScaleImage3D(int srcX,int srcY,int srcZ, int destX,int destY,int destZ, unsigned char *src, unsigned char *dest) {
     float YscaleFactor;
     float XscaleFactor;
     float ZscaleFactor;
     float fx,fy,fz;
     int iy, ix, iz;
     uint32 *src32 = (uint32 *)src;
     uint32 *dest32 = (uint32 *)dest;

     if ((srcY<=0) || (destY<=0) || (srcX<=0) || (destX<=0) || (srcZ<=0) || (destZ<=0)) return;
     if (src == NULL) return;
     if (dest == NULL) return;

     if ((srcY==destY) && (srcX==destX) && (srcZ==destZ)) {
          /* printf ("simple copy\n"); */
          memcpy (dest,src,srcY*srcX*srcZ*4); /* assuming FreeWRL-standard RGBA or BGRA textures */
     }

     /* do x direction first */
     YscaleFactor = ((float)srcY) / ((float)destY);
     XscaleFactor = ((float)srcX) / ((float)destX);
     ZscaleFactor = ((float)srcZ) / ((float)destZ);

     for (iz=0; iz<destZ; iz++) {
          int page;
          fz = ZscaleFactor * ((float) iz);
          page = (int)fz;
          for (iy=0; iy<destY; iy++) {
               int row;
               fy = YscaleFactor * ((float) iy);
               row = (int)(fy);
               for (ix=0; ix<destX; ix++) {
                    int column;
                    int oldIndex;

                    fx = XscaleFactor * ((float) ix);
                    column = (int)(fx);
                    oldIndex = (page * srcY + row) * srcX + column; /* so many rows, each row has srcX columns */
                    dest32[(iz*destY + iy)*destX+ix] = src32[oldIndex];
               }
          }
     }
}

static void GenMipMap2D( GLubyte *src, GLubyte **dst, int srcWidth, int srcHeight, int *dstWidth, int *dstHeight )
{
   int x,
       y;
   int texelSize = 4;
   size_t total_size;
   GLubyte *dest, *pix;


   *dstWidth = srcWidth / 2;
   if ( *dstWidth <= 0 )
      *dstWidth = 1;

   *dstHeight = srcHeight / 2;
   if ( *dstHeight <= 0 )
      *dstHeight = 1;

   total_size = sizeof(GLubyte) * (size_t)texelSize * (size_t)(*dstWidth) * (size_t)(*dstHeight);
   *dst = (GLubyte*)MALLOC(void *, total_size );
   dest = *dst;
   if ( *dst == NULL )
      return;

   for ( y = 0; y < *dstHeight; y++ )
   {
      for( x = 0; x < *dstWidth; x++ )
      {
         size_t srcIndex[4], x2, y2, swidth, texsize, kd;
         float r = 0.0f,
               g = 0.0f,
               b = 0.0f,
           a = 0.0f;

         int sample;

        // Compute the offsets for 2x2 grid of pixels in previous
         // image to perform box filter
         //srcIndex[0] =
         //   (((y * 2) * srcWidth) + (x * 2)) * texelSize;
         //srcIndex[1] =
         //   (((y * 2) * srcWidth) + (x * 2 + 1)) * texelSize;
         //srcIndex[2] =
         //   ((((y * 2) + 1) * srcWidth) + (x * 2)) * texelSize;
         //srcIndex[3] =
         //   ((((y * 2) + 1) * srcWidth) + (x * 2 + 1)) * texelSize;

         swidth = srcWidth;
         texsize = texelSize;
         y2 = y * 2L;
         x2 = x * 2L;
         srcIndex[0] = (y2*swidth + x2) * texsize;
         srcIndex[1] = (y2*swidth + x2 + (size_t)1L) * texsize;
         srcIndex[2] = ((y2 + 1L)*swidth + x2) * texsize;
         srcIndex[3] = ((y2 + 1L)*swidth + x2 + (size_t)1L) * texsize;

         // Sum all pixels
         for ( sample = 0; sample < 4; sample++ )
         {
            r += src[srcIndex[sample] + (size_t)0L];
            g += src[srcIndex[sample] + (size_t)1L];
            b += src[srcIndex[sample] + (size_t)2L];
            a += src[srcIndex[sample] + (size_t)3L];
         }

         // Average results
         r /= 4.0f;
         g /= 4.0f;
         b /= 4.0f;
         a /= 4.0f;

         // Store resulting pixels
         kd = ((size_t)y * (size_t)(*dstWidth) + (size_t)x ) * (size_t)texelSize;
         pix = dest + kd;
         pix[0] = (GLubyte)( r );
         pix[1] = (GLubyte)( g );
         pix[2] = (GLubyte)( b );
         pix[3] = (GLubyte)( a );

         //(*dst)[ ( y * (*dstWidth) + x ) * texelSize ] = (GLubyte)( r );
         //(*dst)[ ( y * (*dstWidth) + x ) * texelSize + 1] = (GLubyte)( g );
         //(*dst)[ ( y * (*dstWidth) + x ) * texelSize + 2] = (GLubyte)( b );
         //(*dst)[ ( y * (*dstWidth) + x ) * texelSize + 3] = (GLubyte)( a );
      }
   }
}

/* create the MIPMAPS ourselves, as the OpenGL ES 2.0 can not do it */
static void myTexImage2D (int generateMipMaps, GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, GLubyte *pixels) {
    GLubyte *prevImage = NULL;
    GLubyte *newImage = NULL;
    size_t total_size;

    /* first, base image */
    FW_GL_TEXIMAGE2D(target,level,internalformat,width,height,border,format,type,pixels);
    {
        GLenum err;
        err = glGetError();
        switch(err){
            case GL_NO_ERROR: break;
            default:
                ConsoleMessage("glError %d in glTexImage2D\n",(int)err);
        }
    }
    if (!generateMipMaps) return;
    if ((width <=1) && (height <=1)) return;


    /* go and create a bunch of mipmaps */
    total_size = (size_t)4L * width * height;
    prevImage = MALLOC(GLubyte *, total_size);
    memcpy (prevImage, pixels, total_size);

    /* from the OpenGL-ES 2.0 book, page 189 */
    level = 1;

    while ((width > 1) && (height > 1)) {
        GLint newWidth, newHeight;

        GenMipMap2D(prevImage,&newImage, width, height, &newWidth, &newHeight);

        FW_GL_TEXIMAGE2D(target,level,internalformat,newWidth,newHeight,0,format,GL_UNSIGNED_BYTE,newImage);

        FREE_IF_NZ(prevImage);
        prevImage = newImage;
        level++;
        width = newWidth;
        height = newHeight;
    }

    FREE_IF_NZ(newImage);
}

static void myScaleImage(int srcX,int srcY,int destX,int destY,unsigned char *src, unsigned char *dest) {
    float YscaleFactor;
    float XscaleFactor;
    int wye, yex;
    uint32 *src32 = (uint32 *)src;
    uint32 *dest32 = (uint32 *)dest;

    if ((srcY<=0) || (destY<=0) || (srcX<=0) || (destX<=0)) return;
    if (src == NULL) return;
    if (dest == NULL) return;

    if ((srcY==destY) && (srcX==destX)) {
        /* printf ("simple copy\n"); */
        memcpy (dest,src,srcY*srcX*4); /* assuming FreeWRL-standard RGBA or BGRA textures */
    }

    /* do x direction first */
    YscaleFactor = ((float)srcY) / ((float)destY);
    XscaleFactor = ((float)srcX) / ((float)destX);

    for (wye=0; wye<destY; wye++) {
        for (yex=0; yex<destX; yex++) {
            float fx, fy;
            int row, column;
            size_t oldIndex, newIndex;

            fx = YscaleFactor * ((float) wye);
            fy = XscaleFactor * ((float) yex);
            row = (int)(fx);
            column = (int)(fy);
            oldIndex = (size_t)row * (size_t)srcX + (size_t)column; /* so many rows, each row has srcX columns */
            newIndex = (size_t)wye * (size_t)destX + (size_t)yex; //wye*destX+yex
            dest32[newIndex] = src32[oldIndex];
        }
    }
}

void move_texture_to_opengl(textureTableIndexStruct_s* me) {
    int rx,ry,rz,sx,sy,sz;
    int x,y,z;
    GLint iformat;
    GLenum format;

    /* default texture properties; can be changed by a TextureProperties node */
    float anisotropicDegree=1.0f;
    int borderWidth;

        GLint Trc,Src,Rrc;
        GLint minFilter, magFilter;
        GLint compression;
        int generateMipMaps;

    unsigned char *mytexdata;

    // JAS - if multi-threading using a creation program,
    // we can have issues sending textures here, so
    // single thread this.
    #ifdef PATH_PLANNER
    pthread_mutex_t gl_mutex1 = PTHREAD_MUTEX_INITIALIZER;
    #endif //PATH_PLANNER

    /* for getting repeatS and repeatT info. */
    struct X3D_PixelTexture *pt = NULL;
    struct X3D_MovieTexture *mt = NULL;
    struct X3D_ImageTexture *it = NULL;
    struct X3D_PixelTexture3D *pt3d = NULL;

    NodeTextureProperties *tpNode = NULL;
    int haveValidTexturePropertiesNode;
    GLfloat texPri;
//    struct SFColorRGBA borderColour;
    s_renderer_capabilities_t *rdr_caps;

    #ifdef PATH_PLANNER
    pthread_mutex_lock( &gl_mutex1 );
    #endif //PATH_PLANNER

//    ttglobal tg = gglobal();
//    rdr_caps = tg->display.rdr_caps;


    /* initialization */
    Src = FALSE; Trc = FALSE; Rrc = FALSE;
    tpNode = NULL;
    haveValidTexturePropertiesNode = FALSE;
    texPri=0.0f;
//    borderColour.c[0]=0.0f;borderColour.c[1]=0.0f;borderColour.c[2]=0.0f;borderColour.c[3]=0.0f;
    compression = GL_FALSE;
    borderWidth = 0;
    mytexdata = NULL;

    /* did this node get killed on the way here? */
/*
    if (!checkNode(me->scenegraphNode, __FILE__,__LINE__)) {
        ConsoleMessage ("main node disappeared, ignoring texture\n");
        me->status = TEXTURE_INVALID;

    #ifdef PATH_PLANNER
    pthread_mutex_unlock( &gl_mutex1 );
    #endif //PATH_PLANNER

        return;
    }
*/
    /* printf ("move_texture_to_opengl, node of type %s\n",stringNodeType(me->scenegraphNode->_nodeType));  */

    /* is this texture invalid and NOT caught before here? */
    /* this is the same as the defaultBlankTexture; the following code should NOT be executed */
    if (me->texdata == NULL) {
        short buff[] = {0x70, 0x70, 0x70, 0xff} ; /* same format as ImageTextures - GL_BGRA or GL_RGBA here */
        me->x = 1;
        me->y = 1;
        me->z = 1;
        me->hasAlpha = FALSE;
        me->texdata = MALLOC(unsigned char *, 4);
        memcpy (me->texdata, buff, 4);
    }

    /* do we need to convert this to an OpenGL texture stream?*/

//printf ("JAS - moveTex, texture %d, TEXTURE_INVALID %d at %s:%d\n",me->OpenGLTexture,TEXTURE_INVALID,__FILE__,__LINE__);

    /* we need to get parameters. */
    if (me->OpenGLTexture == TEXTURE_INVALID) {
/*         me->OpenGLTexture = MALLOC (GLuint *, sizeof (GLuint) * me->frames); */
/*
        if ((getAppearanceProperties()->cubeFace==0) || (getAppearanceProperties()->cubeFace == GL_TEXTURE_CUBE_MAP_POSITIVE_X_EXT)) {
                FW_GL_GENTEXTURES (1, &me->OpenGLTexture);
        }
*/

#ifdef TEXVERBOSE
        printf ("just glGend texture for block %p is %u, type %s\n",
            me, me->OpenGLTexture,stringNodeType(me->nodeType));
#endif

    }

    /* get the repeatS and repeatT info from the scenegraph node */
    NodeImageTexture3D *it3d = (NodeImageTexture3D *) me->scenegraphNode;
    Src = it3d->repeatS()->getValue(); 
    Trc = it3d->repeatT()->getValue(); 
    Rrc = it3d->repeatR()->getValue();
    tpNode = (NodeTextureProperties *)it3d->textureProperties()->getValue();

    //texure3D faked via texture2D (in non-extended GLES2)
    //.. needs repeats for manual wrap vs clamp, will send in
    //.. passedInGenTex
    me->repeatSTR[0] = Src;
    me->repeatSTR[1] = Trc;
    me->repeatSTR[2] = Rrc;


    /* do we have a TextureProperties node? */
    if (tpNode) {
        if (tpNode->getType() != X3D_TEXTURE_PROPERTIES) {
            ConsoleMessage ("have a %s as a textureProperties node", (const char *)tpNode->getProto()->getName(true));
        } else {
            haveValidTexturePropertiesNode = TRUE;
            generateMipMaps = tpNode->generateMipMaps()->getValue()?GL_TRUE:GL_FALSE;
            texPri = tpNode->texturePriority()->getValue();
            if ((texPri < 0.0) || (texPri>1.0)) {
                texPri = 0.0f;
                ConsoleMessage ("invalid texturePriority of %f",tpNode->texturePriority()->getValue());
            }
//            memcpy(&borderColour,&(tpNode->borderColor),sizeof(struct SFColorRGBA));

            anisotropicDegree = tpNode->anisotropicDegree()->getValue();
            if ((anisotropicDegree < 1.0) || (anisotropicDegree>rdr_caps->anisotropicDegree)) {
                /* we can be quiet here
                   ConsoleMessage ("anisotropicDegree error %f, must be between 1.0 and %f",anisotropicDegree, gglobal()->display.rdr_caps.anisotropicDegree);
                */
                anisotropicDegree = rdr_caps->anisotropicDegree;
            }

            borderWidth = tpNode->borderWidth()->getValue();
            if (borderWidth < 0) borderWidth=0; if (borderWidth>1) borderWidth = 1;

            // http://www.web3d.org/documents/specifications/19775-1/V3.3/Part01/components/texturing.html#t-TextureMagnificationModes

/*
            switch (findFieldInTEXTUREMAGNIFICATIONKEYWORDS(tpNode->magnificationFilter()->getValue())) {
                case TMAG_AVG_PIXEL:
                    magFilter = GL_LINEAR; break; // GL_NEAREST; break;
                case TMAG_DEFAULT: magFilter = GL_LINEAR; break;
                case TMAG_FASTEST: magFilter = GL_LINEAR; break;  
                case TMAG_NEAREST_PIXEL: magFilter = GL_NEAREST; break;
                case TMAG_NICEST: magFilter = GL_NEAREST; break;
                default: magFilter = GL_NEAREST; ConsoleMessage ("unknown magnification filter %s",
                    tpNode->magnificationFilter->strptr);
            }

            // minFilter depends on Mipmapping 
            if (generateMipMaps) switch (findFieldInTEXTUREMINIFICATIONKEYWORDS(tpNode->minificationFilter->strptr)) {
                case TMIN_AVG_PIXEL: minFilter = GL_NEAREST; break;
                case TMIN_AVG_PIXEL_AVG_MIPMAP: minFilter = GL_NEAREST_MIPMAP_NEAREST; break;
                case TMIN_AVG_PIXEL_NEAREST_MIPMAP: minFilter = GL_NEAREST_MIPMAP_NEAREST; break;
                case TMIN_DEFAULT: minFilter = GL_NEAREST_MIPMAP_LINEAR; break;
                case TMIN_FASTEST: minFilter = GL_NEAREST_MIPMAP_LINEAR; break;
                case TMIN_NICEST: minFilter = GL_NEAREST_MIPMAP_NEAREST; break;
                case TMIN_NEAREST_PIXEL: minFilter = GL_NEAREST; break;
                case TMIN_NEAREST_PIXEL_NEAREST_MIPMAP: minFilter = GL_NEAREST_MIPMAP_LINEAR; break;
                default: minFilter = GL_NEAREST_MIPMAP_NEAREST;
                    ConsoleMessage ("unknown minificationFilter of %s",
                        tpNode->minificationFilter->strptr);
            }
            else switch (findFieldInTEXTUREMINIFICATIONKEYWORDS(tpNode->minificationFilter->strptr)) {
                case TMIN_AVG_PIXEL:
                case TMIN_AVG_PIXEL_AVG_MIPMAP:
                case TMIN_AVG_PIXEL_NEAREST_MIPMAP:
                case TMIN_DEFAULT:
                case TMIN_FASTEST:
                case TMIN_NICEST:
                case TMIN_NEAREST_PIXEL: minFilter = GL_NEAREST; break;
                case TMIN_NEAREST_PIXEL_NEAREST_MIPMAP: minFilter = GL_LINEAR; break;
                default: minFilter = GL_NEAREST;
                    ConsoleMessage ("unknown minificationFilter of %s",
                        tpNode->minificationFilter->strptr);
            }

            switch (findFieldInTEXTURECOMPRESSIONKEYWORDS(tpNode->textureCompression->strptr)) {
                case TC_DEFAULT: compression = GL_FASTEST; break;
                case TC_FASTEST: compression = GL_NONE; break;
                case TC_HIGH: compression = GL_FASTEST; break;
                case TC_LOW: compression = GL_NONE; break;
                case TC_MEDIUM: compression = GL_NICEST; break;
                case TC_NICEST: compression = GL_NICEST; break;

                default: compression = GL_NEAREST_MIPMAP_NEAREST;
                    ConsoleMessage ("unknown textureCompression of %s",
                        tpNode->textureCompression->strptr);
            }
*/

            magFilter = GL_NEAREST;
            minFilter = GL_NEAREST_MIPMAP_NEAREST;
            compression = GL_FASTEST;

            BOUNDARY_TO_GL(S);
            BOUNDARY_TO_GL(T);
            BOUNDARY_TO_GL(R);
        }
    }



    if (!haveValidTexturePropertiesNode) {
        /* convert TRUE/FALSE to GL_TRUE/GL_FALSE for wrapS and wrapT */
        Src = Src ? GL_REPEAT : GL_CLAMP_TO_EDGE; //GL_CLAMP;   //du9 changed from CLAMP to CLAMP_TO_EDGE Sept18,2011 to fix panorama seamline visibility
        Trc = Trc ? GL_REPEAT : GL_CLAMP_TO_EDGE; //GL_CLAMP;
        Rrc = Rrc ? GL_REPEAT : GL_CLAMP_TO_EDGE; //GL_CLAMP;
        generateMipMaps = GL_TRUE;

        if(me->x > 0 && me->y > 0)
        {
            // experimental code to deal with oblong texture images
            // The 'cause' of blurry oblong textures with GLES2 is the anisotropic
            // scaling of the image needed to square up the image for mipmapping.
            // For example if your texture image is oblong by a factor of 3, then
            // your avatar will need to be 3 times closer to the object to see the
            // same mipmap level as you would with an unscaled image.
            // (OpenGL redbook talks about MipMap levels and rho and lambda.)
            // here we detect if there's big anisotropic scaling/squaring coming up,
            // and if so turn off mipmapping and squaring
            // scene authors need to make their repeating textures (brick, siding,
            // shingles etc) squarish to get mipmapping and avoid moire/scintilation
            float ratio = 1.0f;
            if(me->x < me->y*me->z) ratio = (float)(me->y*me->z) / (float)me->x;
            else ratio = (float)me->x / (float)(me->y*me->z);
            if(ratio > 2.0f) generateMipMaps = GL_FALSE;
        }

        /* choose smaller images to be NEAREST, larger ones to be LINEAR */
        if ((me->x<=256) || ((me->y*me->z)<=256)) {
            minFilter = GL_NEAREST_MIPMAP_NEAREST;
            if(!generateMipMaps) minFilter = GL_NEAREST;
            magFilter = GL_NEAREST;
        } else {
            minFilter = GL_LINEAR_MIPMAP_NEAREST;
            if(!generateMipMaps) minFilter = GL_LINEAR;
            magFilter = GL_LINEAR;
        }
    }

    //ConsoleMessage ("move_texture_to_opengl cubeFace %x\n",getAppearanceProperties()->cubeFace);

    /* is this a CubeMap? If so, lets try this... */
    //if(0){
    //    printf("before texture to GPU and mipmapping thread=%x time = %lf\n",pthread_self().p,Time1970sec());
    //}


//    if (getAppearanceProperties()->cubeFace != 0) {
#if 0
        //this is a single cubmap face pixeltexture tti (ie from __subTextures in ImageCubemap)
        unsigned char *dest = me->texdata;
        uint32 *sp;

        int cx;


        //#if defined (GL_BGRA)
        //iformat = GL_RGBA; format = GL_BGRA;
        //#else
        iformat = GL_RGBA; format = GL_RGBA;
        //#endif


        /* first image in the ComposedCubeMap, do some setups */
/*
        if (getAppearanceProperties()->cubeFace == GL_TEXTURE_CUBE_MAP_POSITIVE_X_EXT) {
            FW_GL_TEXPARAMETERI(GL_TEXTURE_CUBE_MAP_EXT, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            FW_GL_TEXPARAMETERI(GL_TEXTURE_CUBE_MAP_EXT, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            FW_GL_TEXPARAMETERI(GL_TEXTURE_CUBE_MAP_EXT, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            FW_GL_TEXPARAMETERI(GL_TEXTURE_CUBE_MAP_EXT, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            FW_GL_TEXPARAMETERI(GL_TEXTURE_CUBE_MAP_EXT, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        }
*/
        rx = me->x;
        ry = me->y;
        dest = me->texdata;
        if(1){
            //flip cubemap textures to be y-down following opengl specs table 3-19
            //'renderman' convention
            //stack method: row chunks at a time
            uint32 tp[512];
            int cy, cyy, icsize;
            sp = (uint32 *) me->texdata;
            for (cy=0; cy<ry/2; cy++) {
                cyy = ry - cy -1;
                for(cx=0;cx<rx;cx+=512){
                    icsize = min(512,rx-cx-1)*4;
                    memcpy(tp,&sp[cy*rx + cx],icsize);
                    memcpy(&sp[cy*rx + cx],&sp[cyy*rx + cx],icsize);
                    memcpy(&sp[cyy*rx + cx],tp,icsize);
                }
            }
            //printf("__flipping__\n"); //are we in here on every frame? yes, for generatedcubemaptexture, no for other cubemaps
        }
        generateMipMaps = 0;
        myTexImage2D(generateMipMaps, getAppearanceProperties()->cubeFace, 0, iformat,  rx, ry, 0, format, GL_UNSIGNED_BYTE, dest);

        /* last thing to do at the end of the setup for the 6th face */
        if (getAppearanceProperties()->cubeFace == GL_TEXTURE_CUBE_MAP_NEGATIVE_Z) {
            glEnable(GL_TEXTURE_CUBE_MAP);
            glEnable(GL_TEXTURE_GEN_S);
            glEnable(GL_TEXTURE_GEN_T);
            glEnable(GL_TEXTURE_GEN_R);
        }

    } else 
#endif
    {
#if 0
        if (me->nodeType == NODE_ImageCubeMapTexture) {
            if(me->z == 1){
                /* if we have an single 2D image, ImageCubeMap, we have most likely got a png map;
                   ________
                  |     T    | - Top
                  |L F R B| - Left, Front, Right, Back
                  |__D____| - Down(bottom)
                    let the  render_ImageCubeMapTexture code unpack the maps from this one png */
                /* this is ok - what is happening is that we have one image, that needs to be
                    split up into each face */
                /* this should print if we are actually working ok
                if (me->status != TEX_LOADED) {
                    printf ("have ImageCubeMapTexture, but status != TEX_LOADED\n");
                }
                */

                /* call the routine in Component_CubeMapTexturing.c to split this baby apart */
                unpackImageCubeMap(me);
                me->status = TEX_LOADED; /* finito */
            }else if(me->z == 6){
                //likely a .DDS (MS invention) or web3dit (dug9 invention)
                //order of images: +x,-x,+y,-y,+z,-z (or R,L,F,B,T,D)
                unpackImageCubeMap6(me);
                me->status = TEX_LOADED; /* finito */
            }
            //now the __subTextures individual face textures will show as single faces above
        } else if(me->nodeType == NODE_GeneratedCubeMapTexture){
            //already unpacked into 6 separate PixelTexture tti->texdata during cubemap generation
            me->status = TEX_LOADED; /* finito */
        } else 
#endif
        {
            int npot;
            /* a pointer to the tex data. We increment the pointer for movie texures */
            mytexdata = me->texdata;
            if (mytexdata == NULL) {
                printf ("mytexdata is null, texture failed, put something here\n");
            }

            glBindTexture (GL_TEXTURE_2D, me->OpenGLTexture);

            /* save this to determine whether we need to do material node
              within appearance or not */

            /*
            repeatS,repeatT,repeatR
            https://open.gl/textures
            how the texture should be sampled when a coordinate outside the range of 0to 1 is given
            - repeat (*1)
            - mirrored repeat
            - clamp to edge (*2)
            - clamp to border
            subset of opengl supported by web3d:
            http://www.web3d.org/documents/specifications/19775-1/V3.3/Part01/components/texturing.html#Texturecoordinates
            (*1) If repeatS is TRUE (the default), the texture map is repeated outside
                the [0.0, 1.0] texture coordinate range in the S direction so that it fills the shape.
            (*2) If repeatS is FALSE, the texture coordinates are clamped in the S direction to lie
                within the [0.0, 1.0] range.
            */

            FW_GL_TEXPARAMETERI( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, Src);
            FW_GL_TEXPARAMETERI( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, Trc);
#if !defined(GL_ES_VERSION_2_0)
            FW_GL_TEXPARAMETERI( GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, Rrc);
            FW_GL_TEXPARAMETERF(GL_TEXTURE_2D,GL_TEXTURE_PRIORITY, texPri);
//            FW_GL_TEXPARAMETERFV(GL_TEXTURE_2D,GL_TEXTURE_BORDER_COLOR,(GLfloat *)&borderColour);
#endif /*  GL_ES_VERSION_2_0 */

#ifdef GL_TEXTURE_MAX_ANISOTROPY_EXT
            FW_GL_TEXPARAMETERF(GL_TEXTURE_2D,GL_TEXTURE_MAX_ANISOTROPY_EXT,anisotropicDegree);
#endif

            if (compression != GL_NONE) {
                FW_GL_TEXPARAMETERI(GL_TEXTURE_2D, GL_TEXTURE_INTERNAL_FORMAT, GL_COMPRESSED_RGBA);
                glHint(GL_TEXTURE_COMPRESSION_HINT, compression);
            }
            npot = rdr_caps->av_npot_texture;
            x = me->x;
            y = me->y; // * me->z; //takes care of texture3D using strip image
            z = me->z;

            FW_GL_TEXPARAMETERI( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
            FW_GL_TEXPARAMETERI( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);
            me->magFilter = magFilter ==  GL_LINEAR ? 1 : 0;  //needed in frag shader for TEX3D simulation of texture3D with texture2D

            /* BGRA is seemingly faster on desktop machines... */
            //#if defined (GL_BGRA)
            //iformat = GL_RGBA; format = GL_BGRA;
            //#else
            iformat = GL_RGBA; format = GL_RGBA;
            //#endif

            /* do the image. */
            if(x && y) {
                unsigned char *dest = mytexdata;

                /* do we have to do power of two textures? */
                if (npot) { //rdr_caps->av_npot_texture) {
                    rx = x; ry = y; rz = z;
                } else {
                    /* find a power of two that fits */
                    static int round_down = 1;
                    static int trunc_down = 0;
                    rx = 1;
                    sx = x;
                    while(sx) {sx /= 2; rx *= 2;}
                    if(rx/2 == x) {rx /= 2;}
                    //if its just a few pixels over a power of 2, round down
                    if(round_down)
                        if(x < rx && (float)(x - rx/2)/(float)(rx - rx/2) < .25) rx = rx/2;
                    //or we might only scale down, never up, if we don't want to malloc again
                    if(trunc_down)
                        if(x < rx) rx = rx/2;
                    ry = 1;
                    sy = y;
                    while(sy) {sy /= 2; ry *= 2;}
                    if(ry/2 == y) {ry /= 2;}
                    if(round_down)
                        if(y < ry && (float)(y - ry/2)/(float)(ry - ry/2) < .25) ry = ry/2;
                    if(trunc_down)
                        if(y < ry) ry = ry/2;

                    rz = 1;
                    sz = z;
                    while(sz) {sz /= 2; rz *= 2;}
                    if(rz/2 == z) {rz /= 2;}
                    if(round_down)
                        if(z < rz && (float)(z - rz/2)/(float)(rz - rz/2) < .25) rz = rz/2;
                    if(trunc_down)
                        if(z < rz) rz = rz/2;
                }

/*
                if (gglobal()->internalc.global_print_opengl_errors) {
                    DEBUG_MSG("initial texture scale to %d %d\n",rx,ry);
                }
*/
                //ConsoleMessage ("loadTextureNode, runtime texture size %d",gglobal()->display.rdr_caps.runtime_max_texture_size);
                if(z > 1){
                    //its a texture3D / volume image
                    int emulating3D_TILED;
                    emulating3D_TILED = TRUE;
                    generateMipMaps = FALSE;
                    if(emulating3D_TILED){
                        //tiled uses more of the max_texture_size x max_texture_size
                        //    texture3D emulator via TILED texture2D
                        //  reason for emulating: 2016 GLES2 via ANGLEPROJECT(gles emulator over DirectX on windows)
                        //     doesn't have Texture3D or Texture3DOES or Texture3DEXT.
                        //  reason for TILES: an oblong Y-STRIP approach exceded max texture size in Y (but had lots left in X)
                        //     desktop computer max_size (of 2D image in one dimension) 16384
                        //     android phone max_size 4096
                        //     and so would be resampled (blurry) in y and good in x
                        //     using tiles means room for more full z slices ie 256x256x256 == 4096x4096 == 16M,
                        //            512x512x512 == 134M == 16384x16384/2, and therefore less blurry images
                        //  tiles start in upper left with z=0, increase in y,
                        //  then when hit ny tiles in a y strip, move right one tile, and restart at top
                        //  uniform tex3dTiles[3] = {nx,ny,z}
                        //  example ny = 4, nx = 3, z = 11
                        //  1  5  9
                        //  2  6  10
                        //  3  7  11
                        //  4  8
                        //
                        //
                        int rc,sc,c,cube_root, max_size;
                        unsigned char *texdataTiles = NULL;
                        uint32 *p2, *p1;
                        int nx, ny, ix, iy, nxx, nyy;
                        int iz,j,k;

                        max_size = rdr_caps->runtime_max_texture_size;
                        //if(32bit) I find process doesn't have enough RAM left for opengl to malloc 512x512x512x4byte.
                        //could try single channel, single byte textures, but for now we'll keep it under 17M pixels
                        if(x * y * z > 256 * 256 * 256)
                            max_size = min(max_size,4096);
                        //max_size = 2048; //can re-set here for experiments
                        cube_root = (int)pow( max_size * max_size  + 3, 1.0/3.0);
                        c = cube_root;
                        //need lower-power-of-two so we squeeze into space available, and leave a little
                        rc = 1;
                        sc = c;
                        while(sc) {sc /= 2; rc *= 2;}
                        if(rc > c) {rc /= 2;}
                        //ConsoleMessage("pow2 cube root %d\n",rc);
                        cube_root = rc;
                        if(rx != x || ry != y || rz != z || rx > cube_root || ry > cube_root || rz > cube_root) {
                            /* do we have texture limits???
                            dug9: windows intel i5: desktop opengl and uwp/angleproject 16384
                            16384 x 16394 = 268M. cube-root 268M = 645.xx lets round down to pow2: 512
                            android LG nexus 4096
                            4096 x 4096 = 16.7M; cube-root 16.7M = 256.
                            */
                            if (rx > cube_root) rx = cube_root;
                            if (ry > cube_root) ry = cube_root;
                            if (rz > cube_root) rz = cube_root;
                        }

/*
                        if (gglobal()->internalc.global_print_opengl_errors) {
                            DEBUG_MSG("texture size after maxTextureSize taken into account: %d %d, from %d %d\n",rx,ry,x,y);
                        }
                        //ConsoleMessage("texture size after maxTextureSize taken into account: %d %d %d, from %d %d %d\n",rx,ry,rz,x,y,z);
*/
                        //rescale sub-images if/as needed
                        dest = mytexdata;
                        //if(rx != x || ry != y || rz != z){
                        //if(rx > x || ry > y || rz > z){
                        if(x > rx || y > ry || z > rz){
                            int mx,my,mz;
                            mx = min(x,rx);
                            my = min(y,ry);
                            mz = min(z,rz);
                            dest = MALLOC(unsigned char *, 4 * rx * ry * rz);
                            myScaleImage3D(x,y,z,mx,my,mz,mytexdata,dest);
                            x = mx;
                            y = my;
                            z = mz;
                            FREE_IF_NZ(me->texdata);
                        }

                        //COMPUTE GRADIENT - we'll do unconditionally if channels == 1 for 3D image
                        //and hope that the one info channel is alpha because we overwrite rgb
                        if(me->channels == 1){
                            //alpha only scalar image, RGB are free to hold gradient
                            compute_3D_alpha_gradient_store_rgb(dest,x,y,z);
                        }

                        ny = (int) sqrt(z+1);
                        nx = z / ny;
                        nx = z - nx*ny > 0 ? nx+1 : nx;

                        me->tiles[0] = nx; //let the shader tiled emulator for texture3D know via uniform about the tile layout
                        me->tiles[1] = ny;
                        me->tiles[2] = z;
                        //ConsoleMessage("Tiles ny %d nx %d zplanes %d\n",nx,ny,z);
                        nxx = nx*rx;
                        nyy = ny*ry;

                        //place in tile formation - a series of fullish y strips
                        texdataTiles =  MALLOC(unsigned char *,nxx * nyy * 4);
                        p2 = (uint32 *)texdataTiles;
                        p1 = (uint32 *)dest;
                        for(iz=0;iz<z;iz++){
                            iy = iz % ny;
                            ix = iz / ny;
                            for(j=0;j<y;j++){
                                for(k=0;k<x;k++){
                                    int ifrom, ito;
                                    uint32 pixel;
                                    ifrom = (iz*y + j)*x + k;
                                    ito = (iy*y + j)*nxx + (ix*x) + k;
                                    pixel = p1[ifrom];
                                    p2[ito] = pixel;
                                }
                            }
                        }
                        if(0){
                            //write out tiled image for inspection
                            textureTableIndexStruct_s tti2, *tti3;
                            tti3 = &tti2;
                            tti3->x = nxx;
                            tti3->y = nyy;
                            tti3->z = 1;
                            tti3->channels = 4;
                            tti3->texdata = texdataTiles;
//                            saveImage_web3dit(tti3, "test_tiled_texture.web3dit");
                        }

                        myTexImage2D(generateMipMaps, GL_TEXTURE_2D, 0, iformat,  nxx, nyy, 0, format, GL_UNSIGNED_BYTE, texdataTiles);
                        ConsoleMessage("final texture2D size %d %d\n",nxx,nyy);
                        FREE_IF_NZ(texdataTiles);
                        if(dest != me->texdata) FREE_IF_NZ(dest);
                    }else{
                        //use Texture3D which android and winRT/uwp don't have
                    }
                }else{
                    //ordinary 2D image textures
                    if(rx != x || ry != y || rx > rdr_caps->runtime_max_texture_size || ry > rdr_caps->runtime_max_texture_size) {
                        /* do we have texture limits???
                        dug9: windows intel i5: desktop opengl and uwp/angleproject 16384
                        16384 x 16394 = 268M. cube-root 268M = 645.xx lets round down to pow2: 512
                        android LG nexus 4096
                        4096 x 4096 = 16.7M; cube-root 16.7M = 256.
                        */
                        rx = min(rx,rdr_caps->runtime_max_texture_size);
                        ry = min(ry,rdr_caps->runtime_max_texture_size);
                    }
/*
                    if (gglobal()->internalc.global_print_opengl_errors) {
                        DEBUG_MSG("texture size after maxTextureSize taken into account: %d %d, from %d %d\n",rx,ry,x,y);
                    }
                    //ConsoleMessage("texture size after maxTextureSize taken into account: %d %d, from %d %d\n",rx,ry,x,y);
*/
                    /* it is a power of 2, lets make sure it is square */
                    /* ES 2.0 needs this for cross-platform; do not need to do this for desktops, but
                       lets just keep things consistent
                       But if not mipmapping, then (experience with win32 GLES2 emulator and QNX device)
                       then it's not necessary to square the image, although current code will get here with
                       generateMipMap always true.
                       */
                    if (rx != ry) {
                        if(generateMipMaps){
                            if (rx>ry)ry=rx;
                            else rx=ry;
                        }
                    }
                    /* if scaling is ok... */
                    if ((x==rx) && (y==ry)) {
                        dest = mytexdata;
                    } else {

                        /* try this texture on for size, keep scaling down until we can do it */
                        /* all textures are 4 bytes/pixel */
                        size_t total_size = (size_t)4L * rx * ry;
                        dest = MALLOC(unsigned char *, total_size); //4 * rx * ry);

                        myScaleImage(x,y,rx,ry,mytexdata,dest);
                    }
                    if(rx > 8192 || ry > 8192) ConsoleMessage("texture size rx %d ry %d\n",rx,ry);
                    myTexImage2D(generateMipMaps, GL_TEXTURE_2D, 0, iformat,  rx, ry, 0, format, GL_UNSIGNED_BYTE, dest);
                }
                if(mytexdata != dest) {
                    FREE_IF_NZ(dest);
                }
            }

            /* we can get rid of the original texture data here */
            FREE_IF_NZ (me->texdata);
        }
    }


    /* ensure this data is written to the driver for the rendering context */
    FW_GL_FLUSH();

    /* and, now, the Texture is loaded */
    me->status = TEX_LOADED;

    #ifdef PATH_PLANNER
    pthread_mutex_unlock( &gl_mutex1 );
    #endif //PATH_PLANNER

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

struct DdsLoadInfo loadInfoDXT1 = {
  true, false, false, 4, 8, GL_COMPRESSED_RGBA_S3TC_DXT1_EXT
};
struct DdsLoadInfo loadInfoDXT3 = {
  true, false, false, 4, 16, GL_COMPRESSED_RGBA_S3TC_DXT3_EXT
};
struct DdsLoadInfo loadInfoDXT5 = {
  true, false, false, 4, 16, GL_COMPRESSED_RGBA_S3TC_DXT5_EXT
};

struct DdsLoadInfo loadInfoRGB8 = {
  false, false, false, 1, 3, GL_RGB8, GL_RGB, GL_UNSIGNED_BYTE
};
struct DdsLoadInfo loadInfoBGR8 = {
  false, false, false, 1, 3, GL_RGB8, GL_BGR, GL_UNSIGNED_BYTE
};
struct DdsLoadInfo loadInfoBGR565 = {
  false, true, false, 1, 2, GL_RGB5, GL_RGB, GL_UNSIGNED_SHORT_5_6_5
};

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
    }               sPixelFormat;

    //  DDCAPS2
    struct {
      unsigned int    dwCaps1;
      unsigned int    dwCaps2;
      unsigned int    dwDDSX;
      unsigned int    dwReserved;
    }               sCaps;
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
            if( PF_IS_DXT1( hdr.sPixelFormat ) ) {
                li = &loadInfoDXT1;
            }
            else if( PF_IS_DXT3( hdr.sPixelFormat ) ) {
                li = &loadInfoDXT3;
            }
            else if( PF_IS_DXT5( hdr.sPixelFormat ) ) {
                li = &loadInfoDXT5;
            }
  
            #if defined (GL_BGRA)
            else if( PF_IS_BGRA8( hdr.sPixelFormat ) ) {
                li = &loadInfoBGRA8;
            }
            else if( PF_IS_BGR5A1( hdr.sPixelFormat ) ) {
                li = &loadInfoBGR5A1;
            }
            else if( PF_IS_INDEX8( hdr.sPixelFormat ) ) {
                li = &loadInfoIndex8;
            }
            #endif

            else if( PF_IS_RGB8( hdr.sPixelFormat ) ) {
                li = &loadInfoRGB8;
            }
            else if( PF_IS_BGR8( hdr.sPixelFormat ) ) {
                li = &loadInfoBGR8;
            }
            else if( PF_IS_BGR565( hdr.sPixelFormat ) ) {
                li = &loadInfoBGR565;
            }
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

/*
s_list_t *texitem_dequeue(){
    ppLoadTextures p;
//    ttglobal tg = gglobal();
//    p = (ppLoadTextures)gglobal()->LoadTextures.prv;
        p = (ppLoadTextures)textures_prv;

    return threadsafe_dequeue_item_wait(&p->texture_request_list, &tg->threads.mutex_texture_list, &tg->threads.texture_list_condition, &tg->threads.TextureThreadWaiting);
}
*/

//s_list_t* _ml_new(const void *elem, int line, char *fi)
s_list_t* ml_new(void const *elem)
{
    s_list_t *item;
    item = (s_list_t*)malloc(sizeof(s_list_t));
    ml_elem(item) = (void *) elem;
    return item;
}


//we want the void* addresses of the following, so the int value doesn't matter
static const int tex_command_exit = 0;

void texitem_queue_exit(){
    texitem_enqueue(ml_new(&tex_command_exit));
}

void send_texture_to_loader(textureTableIndexStruct_s *entry)
{
    texitem_enqueue(ml_new(entry));
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
    {{NULL,NULL,NULL,NULL, NULL,NULL,NULL},0},
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
        int idim, ilen, isize[4], iendian, iencoding, ifound,slen,klen, bsize;
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
                    for(i=0;i<slen;i++){
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
        if(1){
            printf("iendian %d idatatype %d iencoding %d idim %d isizes %d %d %d %d bsize %d\n",
                    iendian,idatatype,iencoding,idim,isize[0],isize[1],isize[2],isize[3], bsize);
            printf("machine endian isLittle=%d\n",isMachineLittleEndian());
            printf("hows that?\n");

        }
        //clean up dimensions
        if(isize[0] == 1){
            //remove degenerate dimension, found in some images
            for(i=0;i<idim-1;i++){
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
            size_t nelem_read, element_size = 0L;
            element_size = bsize;
            nelem_read = fread(data,element_size, (size_t)nvoxel,fp);
            printf("num elems read = %llu elemsize %ld bytes requeted = %llu %llu\n",(unsigned long long)nelem_read,(long)bsize,bsize*nvoxel,totalbytes);
            //endian conversion
            dataLittleEndian = iendian == NRRDENDIAN_LITTLE ? TRUE : FALSE;
            if(isMachineLittleEndian() != dataLittleEndian && bsize > 1){
                //data endian doesnt match machine endian - swap unconditionally
                printf("swapping endian\n");
                for(i=0;i<nvoxel;i++){
                    unsigned char * voxel = &data[i*bsize];
                    for(j=0;j<bsize/2;j++){
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
            for(i=0;i<isize[2];i++){
                //read a slice
                for(j=0;j<isize[1];j++){
                    //read a row
                    for(k=0;k<isize[0];k++){
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
        if(1) printf("nrrd image voxel range hi %lf lo %lf 255range scale factor %lf\n",dhi,dlo,d255range);
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
            rgba[0] = 255;
            rgba[1] = 255;
            rgba[2] = 255;
            rgba[3] = A;
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
bool texture_load_from_file(textureTableIndexStruct_s* this_tex, char *filename)
{
/* WINDOWS */
#if defined (_MSC_VER) 
    char *fname;
    int ret, imtype;

    fname = STRDUP(filename);
    imtype = sniffImageFileHeader(fname);

    ret = FALSE;
    switch(imtype){
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

    FREE(fname);
    return (ret != 0);

#endif


/* LINUX */

#if !defined (_MSC_VER) && !defined(_ANDROID) && !defined(ANDROIDNDK)
    Imlib_Image image;
    Imlib_Load_Error error_return;
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
        case IMAGETYPE_PNG:
        case IMAGETYPE_JPEG:
        case IMAGETYPE_GIF:
        case IMAGETYPE_UNKNOWN:
        default:
            //JAS     ret = FALSE;
            //image = imlib_load_image_immediately(filename);
            //image = imlib_load_image(filename);
            image = imlib_load_image_with_error_return(filename,&error_return);
            ret = (error_return == 0);

            if (!image) {
                const char *es = NULL;
                switch(error_return){
                    case IMLIB_LOAD_ERROR_NONE: es = "IMLIB_LOAD_ERROR_NONE";break;
                    case IMLIB_LOAD_ERROR_FILE_DOES_NOT_EXIST: es = "IMLIB_LOAD_ERROR_FILE_DOES_NOT_EXIST";break;
                    case IMLIB_LOAD_ERROR_FILE_IS_DIRECTORY: es = "IMLIB_LOAD_ERROR_FILE_IS_DIRECTORY";break;
                    case IMLIB_LOAD_ERROR_PERMISSION_DENIED_TO_READ: es = "IMLIB_LOAD_ERROR_PERMISSION_DENIED_TO_READ";break;
                    case IMLIB_LOAD_ERROR_NO_LOADER_FOR_FILE_FORMAT: es = "IMLIB_LOAD_ERROR_NO_LOADER_FOR_FILE_FORMAT";break;
                    case IMLIB_LOAD_ERROR_PATH_TOO_LONG: es = "IMLIB_LOAD_ERROR_PATH_TOO_LONG";break;
                    case IMLIB_LOAD_ERROR_PATH_COMPONENT_NON_EXISTANT: es = "IMLIB_LOAD_ERROR_PATH_COMPONENT_NON_EXISTANT";break;
                    case IMLIB_LOAD_ERROR_PATH_COMPONENT_NOT_DIRECTORY: es = "IMLIB_LOAD_ERROR_PATH_COMPONENT_NOT_DIRECTORY";break;
                    case IMLIB_LOAD_ERROR_PATH_POINTS_OUTSIDE_ADDRESS_SPACE: es = "IMLIB_LOAD_ERROR_PATH_POINTS_OUTSIDE_ADDRESS_SPACE";break;
                    case IMLIB_LOAD_ERROR_TOO_MANY_SYMBOLIC_LINKS: es = "IMLIB_LOAD_ERROR_TOO_MANY_SYMBOLIC_LINKS";break;
                    case IMLIB_LOAD_ERROR_OUT_OF_MEMORY: es = "IMLIB_LOAD_ERROR_OUT_OF_MEMORY";break;
                    case IMLIB_LOAD_ERROR_OUT_OF_FILE_DESCRIPTORS: es = "IMLIB_LOAD_ERROR_OUT_OF_FILE_DESCRIPTORS";break;
                    case IMLIB_LOAD_ERROR_PERMISSION_DENIED_TO_WRITE: es = "IMLIB_LOAD_ERROR_PERMISSION_DENIED_TO_WRITE";break;
                    case IMLIB_LOAD_ERROR_OUT_OF_DISK_SPACE: es = "IMLIB_LOAD_ERROR_OUT_OF_DISK_SPACE";break;
                    case IMLIB_LOAD_ERROR_UNKNOWN:
                    default:
                    es = "IMLIB_LOAD_ERROR_UNKNOWN";break;
                }
                ERROR_MSG("imlib load error = %d %s\n",error_return,es);
                ERROR_MSG("load_texture_from_file: failed to load image: %s\n", filename);
                return FALSE;
            }
            DEBUG_TEX("load_texture_from_file: Imlib2 succeeded to load image: %s\n", filename);

            imlib_context_set_image(image);
            imlib_image_flip_vertical(); /* FIXME: do we really need this ? */

            /* store actual filename, status, ... */
            this_tex->filename = filename;
            this_tex->hasAlpha = (imlib_image_has_alpha() == 1);
            this_tex->channels = this_tex->hasAlpha ? 4 : 3;
            this_tex->frames = 1;
            this_tex->x = imlib_image_get_width();
            this_tex->y = imlib_image_get_height();

            this_tex->texdata = (unsigned char *) imlib_image_get_data_for_reading_only(); 
            {
                int nchan;
                //int nchan, imtype;
                if(imtype == IMAGETYPE_JPEG)
                    nchan = 3; //jpeg always rgb, no alpha
                else
                    nchan = sniffImageChannels_bruteForce(this_tex->texdata, this_tex->x, this_tex->y); 
                //nchan = sniffImageChannels(fname);
                if(nchan > -1) this_tex->channels = nchan;
            }
            //(Sept 5, 2016 change) assuming imlib gives BGRA:
            texture_swap_B_R(this_tex); 
            //this_tex->data should now be RGBA. (if not comment above line)
            break;
    }

    FREE(fname);
    return (ret);


#endif


    return FALSE;
}

/**********************************************************************************
 bind the image,

    itype     tells us whether it is a PixelTexture, ImageTexture or MovieTexture.

    parenturl  is a pointer to the url of the parent (for relative loads) OR
        a pointer to the image data (PixelTextures only)

    url    the list of urls from the VRML file, or NULL (for PixelTextures)

    texture_num    the OpenGL texture identifier

    repeatS, repeatT VRML fields

    param - vrml fields, but translated into GL_TEXTURE_ENV_MODE, GL_MODULATE, etc.
************************************************************************************/

void new_bind_image(Node *node, struct multiTexParams *param) {
    int thisTexture;
    int thisTextureType;
    struct X3D_ImageTexture *it;
    struct X3D_PixelTexture *pt;
    struct X3D_MovieTexture *mt;
    struct X3D_ImageCubeMapTexture *ict;
    struct X3D_GeneratedCubeMapTexture *gct;

    textureTableIndexStruct_s *myTableIndex;
    //float dcol[] = {0.8f, 0.8f, 0.8f, 1.0f};
    ppTextures p;
    MFString *mfurl = NULL;
//    ttglobal tg = gglobal();
//    p = (ppTextures)tg->Textures.prv;
    p = textures_prv;
    //#define DEBUG_TEX ConsoleMessage

//    GET_THIS_TEXTURE;
//#define GET_THIS_TEXTURE
    NodeImageTexture3D *texture = (NodeImageTexture3D *)node;
    thisTexture = texture->m_textureTableIndex;
    mfurl = texture->url();

    myTableIndex = getTableIndex(thisTexture);
    if (myTableIndex->status != TEX_LOADED) {
        DEBUG_TEX("new_bind_image, I am %p, textureStackTop %d, thisTexture is %d myTableIndex %p status %s\n",
        node,tg->RenderFuncs.textureStackTop,thisTexture,myTableIndex, texst(myTableIndex->status));
        //printf ("new_bind_image (%d), I am %p, textureStackTop %d, thisTexture is %d myTableIndex %p status %s\n",
        //__LINE__,node,tg->RenderFuncs.textureStackTop,thisTexture,myTableIndex, texst(myTableIndex->status));
    }

    /* default here; this is just a blank texture */
//    tg->RenderFuncs.boundTextureStack[tg->RenderFuncs.textureStackTop] = tg->Textures.defaultBlankTexture;
    switch (myTableIndex->status) {
        case TEX_NOTLOADED:
            DEBUG_TEX("feeding texture %p to texture thread...\n", myTableIndex);
            if(mfurl && mfurl->getSize() == 0) {
                //for <ImageTexture /> with url not declared, we should get the default blank image
                myTableIndex->status = TEX_NEEDSBINDING;
            } else {
                myTableIndex->status = TEX_LOADING;
                send_texture_to_loader(myTableIndex);
            }
            break;

        case TEX_LOADING:
        case TEX_READ:
            DEBUG_TEX("I've to wait for %p...\n", myTableIndex);
            break;

        case TEX_NEEDSBINDING:
            DEBUG_TEX("texture loaded into memory... now lets load it into OpenGL...\n");
            move_texture_to_opengl(myTableIndex);

            // do always #ifdef PATH_PLANNER
            // JAS - skipping a rendering loop
            // if we are ok, go direct to rendering this texture.
            if (myTableIndex->status != TEX_LOADED) {
                printf ("issue going from TEX_NEEDSBINDING to TEX_LOADED, is %s\n",
                    texst(myTableIndex->status));
                break;
            }
            // do always #else
            // do always break;
            // do always #endif  //PATH_PLANNER

        case TEX_LOADED:
            //DEBUG_TEX("now binding to pre-bound tex %u\n", myTableIndex->OpenGLTexture);

            /* set the texture depth - required for Material diffuseColor selection */
//            if (myTableIndex->hasAlpha) tg->RenderFuncs.last_texture_type =  TEXTURE_ALPHA;
//            else tg->RenderFuncs.last_texture_type = TEXTURE_NO_ALPHA;

//printf ("last_texture_type = TEXTURE_NO_ALPHA now\n"); last_texture_type=TEXTURE_NO_ALPHA;

            if (myTableIndex->OpenGLTexture == TEXTURE_INVALID) {

                DEBUG_TEX("no openGLtexture here status %s\n", texst(myTableIndex->status));
                return;
            }

//            tg->RenderFuncs.boundTextureStack[tg->RenderFuncs.textureStackTop] = myTableIndex->OpenGLTexture;
            //printf ("new_bind, boundTextureStack[%d] set to %d\n",tg->RenderFuncs.textureStackTop,myTableIndex->OpenGLTexture);


            /* save the texture params for when we go through the MultiTexture stack. Non
               MultiTextures should have this textureStackTop as 0 */

/*
            if (param != NULL) {
                struct multiTexParams *textureParameterStack = (struct multiTexParams *) tg->RenderTextures.textureParameterStack;
                memcpy(&(textureParameterStack[tg->RenderFuncs.textureStackTop]), param,sizeof (struct multiTexParams));
                //memcpy(&(tg->RenderTextures.textureParameterStack[tg->RenderFuncs.textureStackTop]), param,sizeof (struct multiTexParams));
            }
            p->textureInProcess = -1; // we have finished the whole process 
            break;
*/
        case TEX_UNSQUASHED:
        default: {
            printf ("unknown texture status %d\n",myTableIndex->status);
        }
    }
    //#define DEBUG_TEX
}

/* load in a texture, if possible */
void loadTextureNode (NodeImageTexture3D *node, void *vparam)
{
    //printf ("loadTextureNode, node %p, params %p",node,param);
    if (node->m_nodeNeedsCompiling) {

    DEBUG_TEX ("FORCE NODE RELOAD: %p %s\n", node, stringNodeType(node->_nodeType));

    /* force a node reload - make it a new texture. Don't change
       the parameters for the original number, because if this
       texture is shared, then ALL references will change! so,
       we just accept that the current texture parameters have to
       be left behind. */
    //MARK_NODE_COMPILED;
    node->m_nodeNeedsCompiling = false;
    /* this will cause bind_image to create a new "slot" for this texture */
    /* cast to GLuint because __texture defined in VRMLNodes.pm as SFInt */

    releaseTexture(node);
    }

    new_bind_image(node, (struct multiTexParams *)vparam);
    return;
}

/*


Texturing during Runtime 
texture enabling - works for single texture, for multitexture. 

*/

void *RenderTextures_constructor(){
    void *v = MALLOCV(sizeof(struct pRenderTextures));
    memset(v,0,sizeof(struct pRenderTextures));
    return v;
}
void RenderTextures_init(struct tRenderTextures *t){
    textures_prv = (ppTextures)RenderTextures_constructor();
    {
        ppRenderTextures p = (ppRenderTextures)textures_prv;
        /* variables for keeping track of status */
        t->textureParameterStack = (void *)p->textureParameterStack;
    }
}


/* function params */
//static void passedInGenTex(struct textureVertexInfo *genTex);

/* which texture unit are we going to use? is this texture not OFF?? Should we set the
   background coloUr??? Larry the Cucumber, help! */

static int setActiveTexture (int c, GLfloat thisTransparency,  GLint *texUnit, GLint *texMode) 
{
    ppRenderTextures p;
    p = (ppRenderTextures)textures_prv;

    /* which texture unit are we working on? */
    
    /* tie each fw_TextureX uniform into the correct texture unit */
    
    /* here we assign the texture unit to a specific number. NOTE: in the current code, this will ALWAYS
     * be [0] = 0, [1] = 1; etc. */
    texUnit[c] = c;

#ifdef TEXVERBOSE
    if (getAppearanceProperties()->currentShaderProperties != NULL) {
        printf ("setActiveTexture %d, boundTextureStack is %d, sending to uniform %d\n",c,
            tg->RenderFuncs.boundTextureStack[c],
            getAppearanceProperties()->currentShaderProperties->TextureUnit[c]);
    } else {
        printf ("setActiveTexture %d, boundTextureStack is %d, sending to uniform [NULL--No Shader]\n",c,
            tg->RenderFuncs.boundTextureStack[c]);
    }
#endif
    
    /* is this a MultiTexture, or just a "normal" single texture?  When we
     * bind_image, we store a pointer for the texture parameters. It is
     * NULL, possibly different for MultiTextures */

    if (p->textureParameterStack[c].multitex_mode[0] == INT_ID_UNDEFINED) {
        
        #ifdef TEXVERBOSE
        printf ("setActiveTexture - simple texture NOT a MultiTexture \n"); 
        #endif

        /* should we set the coloUr to 1,1,1,1 so that the material does not show
         * through a RGB texture?? */
        /* only do for the first texture if MultiTexturing */
        if (c == 0) {
            #ifdef TEXVERBOSE
            printf ("setActiveTexture - firsttexture  \n"); 
            #endif
            texMode[c]= GL_MODULATE;
        } else {
            texMode[c]=GL_ADD;
        }

    } else {
    /* printf ("muititex source for %d is %d\n",c,tg->RenderTextures.textureParameterStack[c].multitex_source); */
        if (p->textureParameterStack[c].multitex_source[0] != MTMODE_OFF) {
        } else {
            //glDisable(GL_TEXTURE_2D); /* DISABLE_TEXTURES */
            //return FALSE;
            // we do OFF right in the shader
        }
    }

    return TRUE;
}



typedef struct pOpenGL_Utils{
    // list of all X3D nodes in this system.
    // scene graph is tree-structured. this is a linear list.
    struct Vector *linearNodeTable;
    // how many holes might we have in this table, due to killing nodes, etc?
    int potentialHoleCount;

    float cc_red, cc_green, cc_blue, cc_alpha;
    pthread_mutex_t  memtablelock;// = PTHREAD_MUTEX_INITIALIZER;
/*
    MATRIX4 FW_ModelView[MAX_LARGE_MATRIX_STACK];
    MATRIX4 FW_ProjectionView[MAX_SMALL_MATRIX_STACK];
    MATRIX4 FW_TextureView[MAX_SMALL_MATRIX_STACK];
*/
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

/* lets disable texture transforms here */
void textureTransform_end(void) {
    int j;
//    ttglobal tg = gglobal();
    
#ifdef TEXVERBOSE
    printf ("start of textureDraw_end\n");
#endif

    /* DISABLE_TEXTURES */
    /* setting this ENSURES that items, like the HUD, that are not within the normal
       rendering path do not try and use textures... */
    glMatrixMode(GL_TEXTURE);
/*
    for(j=0;j<tg->RenderFuncs.textureStackTop;j++)
        FW_GL_POP_MATRIX(); //pushed in passedInGenTex

    tg->RenderFuncs.textureStackTop = 0;
    tg->RenderFuncs.texturenode = NULL;
*/
    glMatrixMode(GL_MODELVIEW);
}

void
NodeImageTexture3D::preDraw()
{
    RenderTextures_init((tRenderTextures*)&textures_prv);
    registerTexture(this);
    int textureNumber = m_textureTableIndex;
    texture_load_from_file(getTableIndex(textureNumber), 
                           (char *)url()->getValue(0).getData());
    m_textures_prv = textures_prv;
}

void
NodeImageTexture3D::draw(int pass)
{
    textures_prv = m_textures_prv;
    loadTextureNode(this, NULL);    
}

#else

void
NodeImageTexture3D::preDraw()
{
}

void
NodeImageTexture3D::draw(int pass)
{
}

#endif
