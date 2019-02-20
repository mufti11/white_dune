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
#include "GL/gl.h"

#include "NodeImageTexture3D.h"
#include "Proto.h"
#include "FieldValue.h"
#include "MFString.h"
#include "SFBool.h"
#include "SFBool.h"
#include "SFBool.h"
#include "SFNode.h"
#include "DuneApp.h"
#include "Scene.h"
#include "NodeMaterial.h"
#include "NodeTwoSidedMaterial.h"
#include "NodeVolumeData.h"
#include "NodeTextureProperties.h"
#include "NodeTextureTransform.h"
#include "NodeOpacityMapVolumeStyle.h"
#include "NodeComposedVolumeStyle.h"
#include "NodeBlendedVolumeStyle.h"
#include "NodeBoundaryEnhancementVolumeStyle.h"
#include "NodeCartoonVolumeStyle.h"
#include "NodeEdgeEnhancementVolumeStyle.h"
#include "NodeProjectionVolumeStyle.h"
#include "NodeShadedVolumeStyle.h"
#include "NodeSilhouetteEnhancementVolumeStyle.h"
#include "NodeToneMappedVolumeStyle.h"
#include "NodeIsoSurfaceVolumeData.h"
#include "NodeSegmentedVolumeData.h"

#include "NodeEffect.h"
#include "NodeEffectPart.h"

#include "NodePixelTexture3D.h"

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
    m_loaded = false;
#ifdef HAVE_LIBIMLIB2
# ifdef HAVE_LIBGLESV2
    m_textures_prv = NULL;
    m_tableIndex = NULL;
# endif
#endif
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

#ifdef HAVE_LIBIMLIB2
# ifdef HAVE_LIBGLESV2

// code not ready

#include "GL/glext.h"

#include "GLES2/gl2.h"
#include <Imlib2.h>
#include "Texture3D.h"

static int isMobile = TRUE;

ppTextures textures_prv = NULL;
ppOpenGL_Utils openGL_Utils_prv = NULL;
ppRenderFuncs renderFuncs_prv = NULL;

Node *renderFuncs_texturenode = NULL;
int renderFuncs_textureStackTop = 0;

struct multiTexParams {
    int multitex_mode[2];
    int multitex_source[2];
    int multitex_function;
};

typedef struct pRenderTextures{
    struct multiTexParams textureParameterStack[MAX_MULTITEXTURE];
}* ppRenderTextures;

static int _noisy = 0;

const static GLchar *pointSizeDeclare="uniform float pointSize;\n";
const static GLchar *pointSizeAss="gl_PointSize = pointSize; \n";

/* A stack is essentially a vector */
typedef struct Vector Stack;

/* Constructor and destructor */
#define newStack(type) \
 newVector(type, 4)
#define deleteStack(type, me) \
 deleteVector(type, me)

/* Push and pop */
#define stack_push(type, me, el) \
 vector_pushBack(type, me, el)
#define stack_pop(type, me) \
 vector_popBack(type, me)

/* Top of stack */
#define stack_top(type, me) \
 vector_get(type, me, vectorSize(me)-1)

/* Is the stack empty? */
#define stack_empty(me) \
 vector_empty(me)

Stack *getEffectStack(){
/*
	ttglobal tg = gglobal();
	ppComponent_ProgrammableShaders p = (ppComponent_ProgrammableShaders)tg->Component_ProgrammableShaders.prv;
	return p->effect_stack;
*/
        return NULL;
}


static const GLchar *ADSLLightModel = "\n\
/* use ADSLightModel here the ADS colour is returned from the function.  */\n\
vec4 ADSLightModel(in vec3 myNormal, in vec4 myPosition, in bool useMatDiffuse) {\n\
  int i;\n\
  vec4 diffuse = vec4(0., 0., 0., 0.);\n\
  vec4 ambient = vec4(0., 0., 0., 0.);\n\
  vec4 specular = vec4(0., 0., 0., 1.);\n\
  vec3 normal = normalize (myNormal);\n\
\n\
  vec3 viewv = -normalize(myPosition.xyz); \n \
  bool backFacing = (dot(normal,viewv) < 0.0); \n \
  vec4 emissive;\n\
  vec4 matdiffuse = vec4(1.0,1.0,1.0,1.0);\n\
  float myAlph = 0.0;\n\
\n\
  fw_MaterialParameters myMat = fw_FrontMaterial;\n\
\n\
/* back Facing materials - flip the normal and grab back materials */ \n \
if (backFacing) { \n \
    normal = -normal; \n \
    myMat = fw_BackMaterial; \n \
} \n \
\n\
  emissive = myMat.emission;\n\
  myAlph = myMat.diffuse.a;\n\
  if(useMatDiffuse)\n\
    matdiffuse = myMat.diffuse;\n\
\n\
  /* apply the lights to this material */\n\
  for (i=0; i<MAX_LIGHTS; i++) {\n\
    if(i<lightcount) { /*weird but ANGLE needs constant loop*/ \n\
      vec4 myLightDiffuse = fw_LightSource[i].diffuse;\n\
      vec4 myLightAmbient = fw_LightSource[i].ambient;\n\
      vec4 myLightSpecular = fw_LightSource[i].specular;\n\
      vec4 myLightPosition = fw_LightSource[i].position; \n\
      int myLightType = lightType[i]; //fw_LightSource[i].lightType;\n\
      vec3 myLightDir = fw_LightSource[i].spotDirection.xyz; \n\
      vec3 eyeVector = normalize(myPosition.xyz);\n\
      vec3  VP;     /* vector of light direction and distance */\n\
      VP = myLightPosition.xyz - myPosition.xyz;\n\
      vec3 L = myLightDir; /*directional light*/ \n\
      if(myLightType < 2) /*point and spot*/ \n\
       L = normalize(VP); \n\
      float nDotL = max(dot(normal, L), 0.0);\n\
      vec3 halfVector = normalize(L - eyeVector);\n\
      /* normal dot light half vector */\n\
      float nDotHV = max(dot(normal,halfVector),0.0);\n\
      \n\
      if (myLightType==1) {\n\
        /* SpotLight */\n\
        float spotDot; \n\
        float spotAttenuation = 0.0; \n\
        float powerFactor = 0.0; /* for light dropoff */ \n\
        float attenuation; /* computed attenuation factor */\n\
        float d;            /* distance to vertex */            \n\
        d = length(VP);\n\
        if (nDotL > 0.0) {\n\
          powerFactor = pow(nDotL,myMat.shininess); \n\
          /* tone down the power factor if myMat.shininess borders 0 */\n\
          if (myMat.shininess < 1.0) {\n\
            powerFactor *= myMat.shininess; \n\
          } \n\
        } \n\
        attenuation = 1.0/(fw_LightSource[i].Attenuations.x + (fw_LightSource[i].Attenuations.y * d) + (fw_LightSource[i].Attenuations.z *d *d));\n\
        spotDot = dot (-L,myLightDir);\n\
        /* check against spotCosCutoff */\n\
        if (spotDot > fw_LightSource[i].spotCutoff) {\n\
          spotAttenuation = pow(spotDot,fw_LightSource[i].spotCutoff);\n\
        }\n\
        attenuation *= spotAttenuation;\n\
        /* diffuse light computation */\n\
        diffuse += nDotL* matdiffuse*myLightDiffuse * attenuation;\n\
        /* ambient light computation */\n\
        ambient += myMat.ambient*myLightAmbient;\n\
        /* specular light computation */\n\
        specular += myLightSpecular * powerFactor * attenuation;\n\
        \n\
      } else if (myLightType == 2) { \n\
        /* DirectionalLight */ \n\
        float powerFactor = 0.0; /* for light dropoff */\n\
        if (nDotL > 0.0) {\n\
          powerFactor = pow(nDotHV, myMat.shininess);\n\
          /* tone down the power factor if myMat.shininess borders 0 */\n\
          if (myMat.shininess < 1.0) {\n\
           powerFactor *= myMat.shininess;\n\
          }\n\
        }\n\
        /* Specular light computation */\n\
        specular += myMat.specular *myLightSpecular*powerFactor;\n\
        /* diffuse light computation */\n\
        diffuse += nDotL*matdiffuse*myLightDiffuse;\n\
        /* ambient light computation */\n\
        ambient += myMat.ambient*myLightAmbient; \n\
      } else {\n\
        /* PointLight */\n\
        float powerFactor=0.0; /* for light dropoff */\n\
        float attenuation = 0.0; /* computed attenuation factor */\n\
        float d = length(VP);  /* distance to vertex */ \n\
        /* are we within range? */\n\
        if (d <= fw_LightSource[i].lightRadius) {\n\
          if (nDotL > 0.0) {\n\
            powerFactor = pow(nDotL, myMat.shininess);\n\
            //attenuation = (myMat.shininess-128.0);\n\
          }\n\
          /* this is actually the SFVec3f attenuation field */\n\
          attenuation = 1.0/(fw_LightSource[i].Attenuations.x + (fw_LightSource[i].Attenuations.y * d) + (fw_LightSource[i].Attenuations.z *d *d));\n\
          /* diffuse light computation */\n\
          diffuse += nDotL* matdiffuse*myLightDiffuse * attenuation;\n\
          /* ambient light computation */\n\
          ambient += myMat.ambient*myLightAmbient;\n\
          /* specular light computation */\n\
          attenuation *= (myMat.shininess/128.0);\n\
          specular += myLightSpecular * powerFactor * attenuation;\n\
        }\n\
      }\n\
    }\n\
  }\n\
  return clamp(vec4(vec3(ambient+diffuse+specular+emissive),myAlph), 0.0, 1.0);\n\
}\n\
";

static const GLchar *vertex_plug_clip_apply =    "\
#ifdef CLIP \n\
#define FW_MAXCLIPPLANES 4 \n\
uniform int fw_nclipplanes; \n\
uniform vec4 fw_clipplanes[FW_MAXCLIPPLANES]; \n\
varying float fw_ClipDistance[FW_MAXCLIPPLANES]; \n\
 \n\
void PLUG_vertex_object_space (in vec4 vertex_object, in vec3 normal_object){ \n\
  for ( int i=0; i<fw_nclipplanes; i++ ) \n\
    fw_ClipDistance[i] = dot( fw_clipplanes[i], vertex_object); \n\
} \n\
#endif //CLIP \n\
";

static const GLchar *frag_plug_clip_apply =    "\
#ifdef CLIP \n\
#define FW_MAXCLIPPLANES 4 \n\
uniform int fw_nclipplanes; \n\
varying float fw_ClipDistance[FW_MAXCLIPPLANES]; \n\
void PLUG_fog_apply (inout vec4 finalFrag, in vec3 normal_eye_fragment ){ \n\
    for(int i=0;i<fw_nclipplanes;i++) { \n\
      //if(normal_eye_fragment.z > fw_ClipDistance[i]) discard;  \n\
      if(fw_ClipDistance[i] < 0.0) discard; \n\
    } \n\
} \n\
#endif //CLIP \n\
";

// http://www.web3d.org/documents/specifications/19775-1/V3.3/Part01/components/lighting.html#t-foginterpolant
// PLUG: fog_apply (fragment_color, normal_eye_fragment)
static const GLchar *plug_fog_apply =    "\
void PLUG_fog_apply (inout vec4 finalFrag, in vec3 normal_eye_fragment ){ \n\
  float ff = 1.0; \n\
  float depth = abs(castle_vertex_eye.z/castle_vertex_eye.w); \n\
  if(fw_fogparams.fogType > 0){ \n\
    ff = 0.0;  \n\
    if(fw_fogparams.fogType == 1){ //FOGTYPE_LINEAR \n\
      if(depth < fw_fogparams.visibilityRange) \n\
        ff = (fw_fogparams.visibilityRange-depth)/fw_fogparams.visibilityRange; \n\
    } else { //FOGTYPE_EXPONENTIAL \n\
        if(depth < fw_fogparams.visibilityRange){ \n\
          ff = exp(-depth/(fw_fogparams.visibilityRange -depth) ); \n\
          ff = clamp(ff, 0.0, 1.0);  \n\
        } \n\
    } \n\
    finalFrag = mix(finalFrag,fw_fogparams.fogColor,1.0 - ff);  \n\
  } \n\
} \n\
";

//MULTITEXTURE
// http://www.web3d.org/documents/specifications/19775-1/V3.3/Part01/components/texturing.html#MultiTexture
  /* PLUG: texture_apply (fragment_color, normal_eye_fragment) */
static const GLchar *plug_fragment_texture_apply =    "\
void PLUG_texture_apply (inout vec4 finalFrag, in vec3 normal_eye_fragment ){ \n\
\n\
  #ifdef MTEX \n\
  vec4 source; \n\
  int isource,iasource, mode; \n\
  //finalFrag = texture2D(fw_Texture_unit0, fw_TexCoord[0].st) * finalFrag; \n\
  if(textureCount>0){ \n\
    if(fw_Texture_mode0[0] != MTMODE_OFF) { \n\
      isource = fw_Texture_source0[0]; //castle-style dual sources \n\
      iasource = fw_Texture_source0[1]; \n\
      if(isource == MT_DEFAULT) source = finalFrag; \n\
      else if(isource == MTSRC_DIFFUSE) source = matdiff_color; \n\
      else if(isource == MTSRC_SPECULAR) source = vec4(castle_ColorES.rgb,1.0); \n\
      else if(isource == MTSRC_FACTOR) source = mt_Color; \n\
      if(iasource != 0){ \n\
        if(iasource == MT_DEFAULT) source.a = finalFrag.a; \n\
        else if(iasource == MTSRC_DIFFUSE) source.a = matdiff_color.a; \n\
        else if(iasource == MTSRC_SPECULAR) source.a = 1.0; \n\
        else if(iasource == MTSRC_FACTOR) source.a = mt_Color.a; \n\
      } \n\
      finalColCalc(source,fw_Texture_mode0[0],fw_Texture_mode0[1],fw_Texture_function0, fw_Texture_unit0,fw_TexCoord[0].st); \n\
      finalFrag = source; \n\
    } \n\
  } \n\
  if(textureCount>1){ \n\
    if(fw_Texture_mode1[0] != MTMODE_OFF) { \n\
      isource = fw_Texture_source1[0]; //castle-style dual sources \n\
      iasource = fw_Texture_source1[1]; \n\
      if(isource == MT_DEFAULT) source = finalFrag; \n\
      else if(isource == MTSRC_DIFFUSE) source = matdiff_color; \n\
      else if(isource == MTSRC_SPECULAR) source = vec4(castle_ColorES.rgb,1.0); \n\
      else if(isource == MTSRC_FACTOR) source = mt_Color; \n\
      if(iasource != 0){ \n\
        if(iasource == MT_DEFAULT) source.a = finalFrag.a; \n\
        else if(iasource == MTSRC_DIFFUSE) source.a = matdiff_color.a; \n\
        else if(iasource == MTSRC_SPECULAR) source.a = 1.0; \n\
        else if(iasource == MTSRC_FACTOR) source.a = mt_Color.a; \n\
      } \n\
      finalColCalc(source,fw_Texture_mode1[0],fw_Texture_mode1[1],fw_Texture_function1, fw_Texture_unit1,fw_TexCoord[1].st); \n\
      finalFrag = source; \n\
    } \n\
  } \n\
  if(textureCount>2){ \n\
    if(fw_Texture_mode2[0] != MTMODE_OFF) { \n\
      isource = fw_Texture_source2[0]; //castle-style dual sources \n\
      iasource = fw_Texture_source2[1]; \n\
      if(isource == MT_DEFAULT) source = finalFrag; \n\
      else if(isource == MTSRC_DIFFUSE) source = matdiff_color; \n\
      else if(isource == MTSRC_SPECULAR) source = vec4(castle_ColorES.rgb,1.0); \n\
      else if(isource == MTSRC_FACTOR) source = mt_Color; \n\
      if(iasource != 0){ \n\
        if(iasource == MT_DEFAULT) source.a = finalFrag.a; \n\
        else if(iasource == MTSRC_DIFFUSE) source.a = matdiff_color.a; \n\
        else if(iasource == MTSRC_SPECULAR) source.a = 1.0; \n\
        else if(iasource == MTSRC_FACTOR) source.a = mt_Color.a; \n\
      } \n\
      finalColCalc(source,fw_Texture_mode2[0],fw_Texture_mode2[1],fw_Texture_function2,fw_Texture_unit2,fw_TexCoord[2].st); \n\
      finalFrag = source; \n\
    } \n\
  } \n\
  if(textureCount>3){ \n\
    if(fw_Texture_mode3[0] != MTMODE_OFF) { \n\
      isource = fw_Texture_source3[0]; //castle-style dual sources \n\
      iasource = fw_Texture_source3[1]; \n\
      if(isource == MT_DEFAULT) source = finalFrag; \n\
      else if(isource == MTSRC_DIFFUSE) source = matdiff_color; \n\
      else if(isource == MTSRC_SPECULAR) source = vec4(castle_ColorES.rgb,1.0); \n\
      else if(isource == MTSRC_FACTOR) source = mt_Color; \n\
      if(iasource != 0){ \n\
        if(iasource == MT_DEFAULT) source.a = finalFrag.a; \n\
        else if(iasource == MTSRC_DIFFUSE) source.a = matdiff_color.a; \n\
        else if(iasource == MTSRC_SPECULAR) source.a = 1.0; \n\
        else if(iasource == MTSRC_FACTOR) source.a = mt_Color.a; \n\
      } \n\
      finalColCalc(source,fw_Texture_mode3[0],fw_Texture_mode3[1],fw_Texture_function3,fw_Texture_unit3,fw_TexCoord[3].st); \n\
      finalFrag = source; \n\
    } \n\
  } \n\
  #else //MTEX \n\
  /* ONE TEXTURE */ \n\
  #ifdef CUB \n\
  finalFrag = textureCube(fw_Texture_unit0, fw_TexCoord[0]) * finalFrag; \n\
  #else //CUB \n\
  finalFrag = texture2D(fw_Texture_unit0, fw_TexCoord[0].st) * finalFrag; \n\
  #endif //CUB \n\
  #endif //MTEX \n\
  \n\
}\n";

static const GLchar *plug_fragment_texture3D_apply_volume =    "\n\
vec4 texture3Demu0( sampler2D sampler, in vec3 texcoord3, in int magfilter){ \n\
  vec4 sample = vec4(0.0); \n\
  #ifdef TEX3D \n\
  //TILED method (vs Y strip method) \n\
  vec3 texcoord = texcoord3; \n\
  //texcoord.z = 1.0 - texcoord.z; //flip z from RHS to LHS\n\
  float depth = max(1.0,float(tex3dTiles[2])); \n\
  if(repeatSTR[0] == 0) texcoord.x = clamp(texcoord.x,0.0001,.9999); \n\
  else texcoord.x = mod(texcoord.x,1.0); \n\
  if(repeatSTR[1] == 0) texcoord.y = clamp(texcoord.y,0.0001,.9999); \n\
  else texcoord.y = mod(texcoord.y,1.0); \n\
  if(repeatSTR[2] == 0) texcoord.z = clamp(texcoord.z,0.0001,.9999); \n\
  else texcoord.z = mod(texcoord.z,1.0); \n\
  vec4 texel; \n\
  int izf = int(floor(texcoord.z*depth)); //floor z \n\
  int izc = int(ceil(texcoord.z*depth));  //ceiling z \n\
  izc = izc == tex3dTiles[2] ? izc - 1 : izc; //clamp int z \n\
  vec4 ftexel, ctexel; \n\
  \n\
  int nx = tex3dTiles[0]; //0-11 \n\
  int ny = tex3dTiles[1]; \n\
  float fnx = 1.0/float(nx); //.1\n\
  float fny = 1.0/float(ny); \n\
  int ix = izc / ny; //60/11=5\n\
  int ixny = ix * ny; //5*11=55\n\
  int iy = izc - ixny; //60-55=5 modulus remainder \n\
  float cix = float(ix); //5 \n\
  float ciy = float(iy); \n\
  float xxc = (cix + texcoord.s)*fnx; //(5 + .5)*.1 = .55\n\
  float yyc = (ciy + texcoord.t)*fny; \n\
  ix = izf / ny; \n\
  ixny = ix * ny; \n\
  iy = izf - ixny; //modulus remainder \n\
  float fix = float(ix); \n\
  float fiy = float(iy); \n\
  float xxf = (fix + texcoord.s)*fnx; \n\
  float yyf = (fiy + texcoord.t)*fny; \n\
  \n\
  vec2 ftexcoord, ctexcoord; //texcoord is 3D, ftexcoord and ctexcoord are 2D coords\n\
  ftexcoord.s = xxf; \n\
  ftexcoord.t = yyf; \n\
  ctexcoord.s = xxc; \n\
  ctexcoord.t = yyc; \n\
  ftexel = texture2D(sampler,ftexcoord.st); \n\
  ctexel = texture2D(sampler,ctexcoord.st); \n\
  float fraction = mod(texcoord.z*depth,1.0); \n\
  if(magfilter == 1) \n\
    texel = mix(ctexel,ftexel,1.0-fraction); //lerp GL_LINEAR \n\
  else \n\
    texel = ftexel; //fraction > .5 ? ctexel : ftexel; //GL_NEAREST \n\
  sample = texel; \n\
  #endif //TEX3D \n\
  return sample; \n\
} \n\
vec4 texture3Demu( sampler2D sampler, in vec3 texcoord3){ \n\
    //use uniform magfilter \n\
    return texture3Demu0( sampler, texcoord3, magFilter); \n\
} \n\
void PLUG_texture3D( inout vec4 sample, in vec3 texcoord3 ){ \n\
    sample = texture3Demu(fw_Texture_unit0,texcoord3); \n\
} \n\
void PLUG_texture_apply (inout vec4 finalFrag, in vec3 normal_eye_fragment ){ \n\
\n\
    vec4 sample; \n\
    sample = texture3Demu(fw_Texture_unit0,fw_TexCoord[0]); \n\
    finalFrag *= sample; \n\
  \n\
}\n";


static const GLchar *plug_fragment_texture3Dlayer_apply =    "\
void PLUG_texture_apply (inout vec4 finalFrag, in vec3 normal_eye_fragment ){ \n\
\n\
  #ifdef TEX3DLAY \n\
  vec3 texcoord = fw_TexCoord[0]; \n\
  texcoord.z = 1.0 - texcoord.z; //flip z from RHS to LHS\n\
  float depth = max(1.0,float(textureCount-1)); \n\
  float delta = 1.0/depth; \n\
  if(repeatSTR[0] == 0) texcoord.x = clamp(texcoord.x,0.0001,.9999); \n\
  else texcoord.x = mod(texcoord.x,1.0); \n\
  if(repeatSTR[1] == 0) texcoord.y = clamp(texcoord.y,0.0001,.9999); \n\
  else texcoord.y = mod(texcoord.y,1.0); \n\
  if(repeatSTR[2] == 0) texcoord.z = clamp(texcoord.z,0.0001,.9999); \n\
  else texcoord.z = mod(texcoord.z,1.0); \n\
  int flay = int(floor(texcoord.z*depth)); \n\
  int clay = int(ceil(texcoord.z*depth)); \n\
  vec4 ftexel, ctexel; \n\
  //flay = 0; \n\
  //clay = 1; \n\
  if(flay == 0) ftexel = texture2D(fw_Texture_unit0,texcoord.st);  \n\
  if(clay == 0) ctexel = texture2D(fw_Texture_unit0,texcoord.st);  \n\
  if(flay == 1) ftexel = texture2D(fw_Texture_unit1,texcoord.st);  \n\
  if(clay == 1) ctexel = texture2D(fw_Texture_unit1,texcoord.st);  \n\
  if(flay == 2) ftexel = texture2D(fw_Texture_unit2,texcoord.st);  \n\
  if(clay == 2) ctexel = texture2D(fw_Texture_unit2,texcoord.st);  \n\
  if(flay == 3) ftexel = texture2D(fw_Texture_unit3,texcoord.st);  \n\
  if(clay == 3) ctexel = texture2D(fw_Texture_unit3,texcoord.st); \n\
  float fraction = mod(texcoord.z*depth,1.0); \n\
  vec4 texel; \n\
  if(magFilter == 1) \n\
    texel = mix(ctexel,ftexel,(1.0-fraction)); //lerp GL_LINEAR \n\
  else \n\
    texel = fraction > .5 ? ctexel : ftexel; //GL_NEAREST \n\
  finalFrag *= texel; \n\
  #endif //TEX3DLAY \n\
  \n\
}\n";

static const GLchar *plug_vertex_lighting_ADSLightModel = "\n\
/* use ADSLightModel here the ADS colour is returned from the function.  */ \n\
void PLUG_add_light_contribution2 (inout vec4 vertexcolor, inout vec3 specularcolor, in vec4 myPosition, in vec3 myNormal, in float shininess ) { \n\
  //working in eye space: eye is at 0,0,0 looking generally in direction 0,0,-1 \n\
  //myPosition, myNormal - of surface vertex, in eyespace \n\
  //vertexcolor - diffuse+ambient -will be replaced or modulated by texture color \n\
  //specularcolor - specular+emissive or non-diffuse (emissive added outside this function) \n\
  //algo: uses Blinn-Phong specular reflection: half-vector pow(N*H,shininess) \n\
  int i; \n\
  vec4 diffuse = vec4(0., 0., 0., 0.); \n\
  vec4 ambient = vec4(0., 0., 0., 0.); \n\
  vec4 specular = vec4(0., 0., 0., 1.); \n\
  vec3 N = normalize (myNormal); \n\
  \n\
  vec3 E = -normalize(myPosition.xyz); \n \
  vec4 matdiffuse = vec4(1.0,1.0,1.0,1.0); \n\
  float myAlph = 0.0;\n\
  \n\
  fw_MaterialParameters myMat = fw_FrontMaterial; \n\
  \n\
  /* back Facing materials - flip the normal and grab back materials */ \n\
  bool backFacing = (dot(N,E) < 0.0); \n\
  if (backFacing) { \n\
    N = -N; \n\
    #ifdef TWO \n\
    myMat = fw_BackMaterial; \n\
    #endif //TWO \n\
  } \n\
  \n\
  myAlph = myMat.diffuse.a; \n\
  //if(useMatDiffuse) \n\
  matdiffuse = myMat.diffuse; \n\
  \n\
  /* apply the lights to this material */ \n\
  /* weird but ANGLE needs constant loop */ \n\
  for (i=0; i<MAX_LIGHTS; i++) {\n\
    if(i < lightcount) { \n\
      vec4 myLightDiffuse = fw_LightSource[i].diffuse; \n\
      vec4 myLightAmbient = fw_LightSource[i].ambient; \n\
      vec4 myLightSpecular = fw_LightSource[i].specular; \n\
      vec4 myLightPosition = fw_LightSource[i].position; \n\
      int myLightType = lightType[i]; \n\
      vec3 myLightDir = fw_LightSource[i].spotDirection.xyz; \n\
      vec3  VP;     /* vector of light direction and distance */ \n\
      VP = myLightPosition.xyz - myPosition.xyz; \n\
      vec3 L = myLightDir; /*directional light*/ \n\
      if(myLightType < 2) /*point and spot*/ \n\
        L = normalize(VP); \n\
      float NdotL = max(dot(N, L), 0.0); //Lambertian diffuse term \n\
      /*specular reflection models, phong or blinn-phong*/ \n\
      //#define PHONG 1 \n\
      #ifdef PHONG \n\
      //Phong \n\
      vec3 R = normalize(-reflect(L,N)); \n\
      float RdotE = max(dot(R,E),0.0); \n\
      float specbase = RdotE; \n\
      float specpow = .3 * myMat.shininess; //assume shini tuned to blinn, adjust for phong \n\
      #else //PHONG \n\
      //Blinn-Phong \n\
      vec3 H = normalize(L + E); //halfvector\n\
      float NdotH = max(dot(N,H),0.0); \n\
      float specbase = NdotH; \n\
      float specpow = myMat.shininess; \n\
      #endif //PHONG \n\
      float powerFactor = 0.0; /* for light dropoff */ \n\
      if (specbase > 0.0) { \n\
        powerFactor = pow(specbase,specpow); \n\
        /* tone down the power factor if myMat.shininess borders 0 */ \n\
        if (myMat.shininess < 1.0) { \n\
          powerFactor *= myMat.shininess; \n\
        } \n\
      } \n\
      \n\
      if (myLightType==1) { \n\
        /* SpotLight */ \n\
        float spotDot, multiplier; \n\
        float spotAttenuation = 0.0; \n\
        float attenuation; /* computed attenuation factor */ \n\
        float D; /* distance to vertex */ \n\
        D = length(VP); \n\
        attenuation = 1.0/(fw_LightSource[i].Attenuations.x + (fw_LightSource[i].Attenuations.y * D) + (fw_LightSource[i].Attenuations.z *D*D)); \n\
        multiplier = 0.0; \n\
        spotDot = dot (-L,myLightDir); \n\
        /* check against spotCosCutoff */ \n\
        if (spotDot > fw_LightSource[i].spotCutoff) { \n\
          //?? what was this: spotAttenuation = pow(spotDot,fw_LightSource[i].spotExponent); \n\
          if(spotDot > fw_LightSource[i].spotBeamWidth) { \n\
            multiplier = 1.0; \n\
          } else { \n\
            multiplier = (spotDot - fw_LightSource[i].spotCutoff)/(fw_LightSource[i].spotBeamWidth - fw_LightSource[i].spotCutoff); \n\
          } \n\
        } \n\
        //attenuation *= spotAttenuation; \n\
        attenuation *= multiplier; \n\
        /* diffuse light computation */ \n\
        diffuse += NdotL* matdiffuse*myLightDiffuse * attenuation; \n\
        /* ambient light computation */ \n\
        ambient += myMat.ambient*myLightAmbient; \n\
        /* specular light computation */ \n\
        specular += myLightSpecular * powerFactor * attenuation; \n\
        \n\
      } else if (myLightType == 2) { \n\
        /* DirectionalLight */ \n\
        /* Specular light computation */ \n\
        specular += myMat.specular *myLightSpecular*powerFactor; \n\
        /* diffuse light computation */ \n\
        diffuse += NdotL*matdiffuse*myLightDiffuse; \n\
        /* ambient light computation */ \n\
        ambient += myMat.ambient*myLightAmbient; \n\
      } else { \n\
        /* PointLight */ \n\
        float attenuation = 0.0; /* computed attenuation factor */ \n\
        float D = length(VP);  /* distance to vertex */ \n\
        /* are we within range? */ \n\
        if (D <= fw_LightSource[i].lightRadius) { \n\
          /* this is actually the SFVec3f attenuation field */ \n\
          attenuation = 1.0/(fw_LightSource[i].Attenuations.x + (fw_LightSource[i].Attenuations.y * D) + (fw_LightSource[i].Attenuations.z *D*D)); \n\
          /* diffuse light computation */ \n\
          diffuse += NdotL* matdiffuse*myLightDiffuse * attenuation; \n\
          /* ambient light computation */ \n\
          ambient += myMat.ambient*myLightAmbient; \n\
          /* specular light computation */ \n\
          attenuation *= (myMat.shininess/128.0); \n\
          specular += myLightSpecular * powerFactor * attenuation; \n\
        } \n\
      } \n\
    } \n\
  } \n\
  vertexcolor = clamp(vec4(vec3(ambient + diffuse ) + vertexcolor.rgb ,myAlph), 0.0, 1.0); \n\
  specularcolor = clamp(specular.rgb + specularcolor, 0.0, 1.0); \n\
} \n\
";

#define STR_MAX_LIGHTS "\n#define MAX_LIGHTS 8\n "
static const GLchar *maxLights = STR_MAX_LIGHTS;

static const GLchar *fragHighPrecision = "precision highp float;\n ";

static const GLchar *fragMediumPrecision = "precision mediump float;\n ";

static const GLchar *discardInFragEnd = "if (finalFrag.a==0.0) {discard; } else {gl_FragColor = finalFrag;}}";

static const GLchar *fragEnd = "gl_FragColor = finalFrag;}";

static const GLchar *fragNoAppAss = "finalFrag = vec4(1.0, 1.0, 1.0, 1.0);\n";

const static GLchar *fragADSLAss = "finalFrag = ADSLightModel(vertexNorm,vertexPos,true) * finalFrag;";

static const GLchar *fragFrontColAss=    " finalFrag = v_front_colour * finalFrag;";

static const GLchar *fragTex0Dec = "uniform sampler2D fw_Texture_unit0; \n";

const static GLchar *fragSingTexAss = "finalFrag = texture2D(fw_Texture_unit0, fw_TexCoord[0].st) * finalFrag;\n";

static const GLchar *fragTex0CubeDec = "uniform samplerCube fw_Texture_unit0; \n";

const static GLchar *fragSingTexCubeAss = "finalFrag = textureCube(fw_Texture_unit0, fw_TexCoord[0]) * finalFrag;\n";

const static GLchar *fragMulTexCalc = "\
if(textureCount>=1) {finalFrag=finalColCalc(finalFrag,fw_Texture_mode0,fw_Texture_unit0,fw_TexCoord[0].st);} \n\
if(textureCount>=2) {finalFrag=finalColCalc(finalFrag,fw_Texture_mode1,fw_Texture_unit1,fw_TexCoord[0].st);} \n\
if(textureCount>=3) {finalFrag=finalColCalc(finalFrag,fw_Texture_mode2,fw_Texture_unit2,fw_TexCoord[0].st);} \n\
/* REMOVE these as shader compile takes long \
if(textureCount>=4) {finalFrag=finalColCalc(finalFrag,fw_Texture_mode3,fw_Texture_unit3,fw_TexCoord[0].st);} \n\
if(textureCount>=5) {finalFrag=finalColCalc(finalFrag,fw_Texture_mode4,fw_Texture_unit4,fw_TexCoord[0].st);} \n\
if(textureCount>=6) {finalFrag=finalColCalc(finalFrag,fw_Texture_mode5,fw_Texture_unit5,fw_TexCoord[0].st);} \n\
if(textureCount>=7) {finalFrag=finalColCalc(finalFrag,fw_Texture_mode6,fw_Texture_unit6,fw_TexCoord[0].st);} \n\
if(textureCount>=8) {finalFrag=finalColCalc(finalFrag,fw_Texture_mode7,fw_Texture_unit7,fw_TexCoord[0].st);} \n\
*/ \n";

static const GLchar *fragFillPropFunc = "\
vec4 fillPropCalc(in vec4 prevColour, vec2 MCposition, int algorithm) {\
vec4 colour; \
vec2 position, useBrick; \
\
position = MCposition / HatchScale; \
\
if (algorithm == 0) {/* bricking  */ \
    if (fract(position.y * 0.5) > 0.5) \
        position.x += 0.5; \
        }\
\
/* algorithm 1, 2 = no futzing required here  */ \
if (algorithm == 3) {/* positive diagonals */ \
    vec2 curpos = position; \
    position.x -= curpos.y; \
} \
\
if (algorithm == 4) {/* negative diagonals */ \
    vec2 curpos = position; \
    position.x += curpos.y; \
} \
\
if (algorithm == 6) {/* diagonal crosshatch */ \
    vec2 curpos = position; \
    if (fract(position.y) > 0.5)  { \
        if (fract(position.x) < 0.5) position.x += curpos.y; \
        else position.x -= curpos.y; \
    } else { \
        if (fract(position.x) > 0.5) position.x += curpos.y; \
        else position.x -= curpos.y; \
    } \
} \
\
position = fract(position); \
\
useBrick = step(position, HatchPct); \
\
    if (filled) {colour = prevColour;} else { colour=vec4(0.,0.,0.,0); }\
if (hatched) { \
    colour = mix(HatchColour, colour, useBrick.x * useBrick.y); \
} \
return colour; } ";

static const GLchar *fragFillPropCalc = "\
finalFrag= fillPropCalc(finalFrag, hatchPosition, algorithm);\n";

static const GLchar *fragMulTexFunc ="\
vec4 finalColCalc(in vec4 prevColour, in int mode, in sampler2D tex, in vec2 texcoord) { \
vec4 texel = texture2D(tex,texcoord); \
vec4 rv = vec4(1.,0.,1.,1.);  \
\
if (mode==MTMODE_OFF) { rv = vec4(prevColour);} \
else if (mode==MTMODE_REPLACE) {rv = vec4(texture2D(tex, texcoord));}\
else if (mode==MTMODE_MODULATE) { \
\
vec3 ct,cf; \
float at,af; \
\
cf = prevColour.rgb; \
af = prevColour.a; \
\
ct = texel.rgb; \
at = texel.a; \
\
\
rv = vec4(ct*cf, at*af); \
\
} \
else if (mode==MTMODE_MODULATE2X) { \
vec3 ct,cf; \
float at,af; \
\
cf = prevColour.rgb; \
af = prevColour.a; \
\
ct = texel.rgb; \
at = texel.a; \
rv = vec4(vec4(ct*cf, at*af)*vec4(2.,2.,2.,2.)); \
}\
else if (mode==MTMODE_MODULATE4X) { \
vec3 ct,cf; \
float at,af; \
\
cf = prevColour.rgb; \
af = prevColour.a; \
\
ct = texel.rgb; \
at = texel.a; \
rv = vec4(vec4(ct*cf, at*af)*vec4(4.,4.,4.,4.)); \
}\
else if (mode== MTMODE_ADDSIGNED) {\
rv = vec4 (prevColour + texel - vec4 (0.5, 0.5, 0.5, -.5)); \
} \
else if (mode== MTMODE_ADDSIGNED2X) {\
rv = vec4 ((prevColour + texel - vec4 (0.5, 0.5, 0.5, -.5))*vec4(2.,2.,2.,2.)); \
} \
else if (mode== MTMODE_ADD) {\
rv= vec4 (prevColour + texel); \
} \
else if (mode== MTMODE_SUBTRACT) {\
rv = vec4 (prevColour - texel); \
} \
else if (mode==MTMODE_ADDSMOOTH) { \
rv = vec4 (prevColour + (prevColour - vec4 (1.,1.,1.,1.)) * texel); \
} \
return rv; \
\
} \n";

static const GLchar *fragMultiTexUniforms = " \
/* defined for single textures... uniform sampler2D fw_Texture_unit0; */\
uniform sampler2D fw_Texture_unit1; \
uniform sampler2D fw_Texture_unit2; \
/* REMOVE these as shader compile takes long \
uniform sampler2D fw_Texture_unit3; \
uniform sampler2D fw_Texture_unit4; \
uniform sampler2D fw_Texture_unit5; \
uniform sampler2D fw_Texture_unit6; \
uniform sampler2D fw_Texture_unit7; \
*/  \
uniform int fw_Texture_mode0; \
uniform int fw_Texture_mode1; \
uniform int fw_Texture_mode2; \
/* REMOVE these as shader compile takes long \
uniform int fw_Texture_mode3; \
uniform int fw_Texture_mode4; \
uniform int fw_Texture_mode5; \
uniform int fw_Texture_mode6; \
uniform int fw_Texture_mode7; \
*/ \n\
\
uniform int textureCount;\n";

const static GLchar *fragMultiTexDef = MULTITEXTUREDefs;

const static GLchar *vertADSLCalc = "v_front_colour = ADSLightModel(vertexNorm,vertexPos,true);";

const static GLchar *vertADSLCalc0 = "v_front_colour = ADSLightModel(vertexNorm,vertexPos,false);";

static const GLchar *vertHatchPosCalc = "hatchPosition = fw_Vertex.xy;\n";

static const GLchar *fillPropDefines = "\
uniform vec4 HatchColour; \n\
uniform bool hatched; uniform bool filled;\n\
uniform vec2 HatchScale; uniform vec2 HatchPct; uniform int algorithm; ";

//=============STRUCT METHOD FOR LIGHTS==================
// use for opengl, and angleproject desktop/d3d9
static const GLchar *lightDefines = "\
struct fw_MaterialParameters {\n\
  vec4 emission;\n\
  vec4 ambient;\n\
  vec4 diffuse;\n\
  vec4 specular;\n\
  float shininess;\n\
};\n\
uniform int lightcount;\n\
//uniform float lightRadius[MAX_LIGHTS];\n\
uniform int lightType[MAX_LIGHTS];//ANGLE like this\n\
struct fw_LightSourceParameters { \n\
  vec4 ambient;  \n\
  vec4 diffuse;   \n\
  vec4 specular; \n\
  vec4 position;   \n\
  vec4 halfVector;  \n\
  vec4 spotDirection; \n\
  float spotBeamWidth; \n\
  float spotCutoff; \n\
  vec3 Attenuations; \n\
  //float constantAttenuation; \n\
  //float linearAttenuation;  \n\
  //float quadraticAttenuation; \n\
  float lightRadius; \n\
  //int lightType; ANGLE doesnt like int in struct array \n\
}; \n\
\n\
uniform fw_LightSourceParameters fw_LightSource[MAX_LIGHTS] /* gl_MaxLights */ ;\n\
";

/*
Good hints for code here: http://www.opengl.org/wiki/Mathematics_of_glTexGen
*/
static const GLchar *sphEnvMapCalc = " \n     \
/* sphereEnvironMapping Calculation */ \
/* vec3 u=normalize(vec3(fw_ModelViewMatrix * fw_Vertex));  (myEyeVertex)  \
vec3 n=normalize(vec3(fw_NormalMatrix*fw_Normal)); \
vec3 r = reflect(u,n);  (myEyeNormal) */ \n\
vec3 u=normalize(vec3(vertexPos)); /* u is normalized position, used below more than once */ \n \
vec3 r= reflect(u,vertexNorm); \n\
if (fw_textureCoordGenType==TCGT_SPHERE) { /* TCGT_SPHERE  GL_SPHERE_MAP OpenGL Equiv */ \n\
    float m=2.0 * sqrt(r.x*r.x + r.y*r.y + (r.z*1.0)*(r.z*1.0)); \n\
    fw_TexCoord[0] = vec3(r.x/m+0.5,r.y/m+0.5,0.0); \n \
}else if (fw_textureCoordGenType==TCGT_CAMERASPACENORMAL) /* GL_REFLECTION_MAP used for sampling cubemaps */ {\n \
    float dotResult = 2.0 * dot(u,r); \n\
    fw_TexCoord[0] = vec3(u-r)*dotResult;\n\
} else { /* default usage - like default CubeMaps */ \n\
    vec3 u=normalize(vec3(fw_ProjectionMatrix * fw_Vertex)); /* myEyeVertex */ \
    fw_TexCoord[0] = reflect(u,vertexNorm);\n \
}\n\
";

#define TEXTURECOORDINATEGENERATORDefs " \
#define TCGT_CAMERASPACENORMAL    0\n \
#define TCGT_CAMERASPACEPOSITION    1\n \
#define TCGT_CAMERASPACEREFLECTION    2\n \
#define TCGT_COORD    3\n \
#define TCGT_COORD_EYE    4\n \
#define TCGT_NOISE    5\n \
#define TCGT_NOISE_EYE    6\n \
#define TCGT_SPHERE    7\n \
#define TCGT_SPHERE_LOCAL    8\n \
#define TCGT_SPHERE_REFLECT    9\n \
#define TCGT_SPHERE_REFLECT_LOCAL    10\n \
";

int fw_strcpy_s(char *dest, int destsize, const char *source){
    int ier = -1;
    if(dest)
    if(source && strlen(source) < (unsigned)destsize){
        strcpy(dest,source);
        ier = 0;
    }
    return ier;
}

int fw_strcat_s(char *dest, int destsize, const char *source){
    int ier = -1;
    if(dest){
        int destlen = strlen(dest);
        if(source)
            if(strlen(source)+destlen < (unsigned)destsize){
                strcat(dest,source);
                ier = 0;
            }
    }
    return ier;
}

static double identity[] = { 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0 };

void loadIdentityMatrix (double *mat) {
        memcpy((void *)mat, (void *)identity, sizeof(double)*16);
}

void fw_glGetDoublev (int ty, GLDOUBLE *mat) {
    GLDOUBLE *dp;
//    ppOpenGL_Utils p = (ppOpenGL_Utils)gglobal()->OpenGL_Utils.prv;
    ppOpenGL_Utils p = (ppOpenGL_Utils)openGL_Utils_prv;

/*
    switch (ty) {
        case GL_PROJECTION_MATRIX: printf ("getDoublev(GL_PROJECTION_MATRIX)\n"); break;
        case GL_MODELVIEW_MATRIX: printf ("getDoublev(GL_MODELVIEW_MATRIX)\n"); break;
        case GL_TEXTURE_MATRIX: printf ("getDoublev(GL_TEXTURE_MATRIX)\n"); break;
    }
*/

    switch (ty) {
        case GL_PROJECTION_MATRIX: dp = p->FW_ProjectionView[p->projectionviewTOS]; break;
        case GL_MODELVIEW_MATRIX: dp = p->FW_ModelView[p->modelviewTOS]; break;
        case GL_TEXTURE_MATRIX: dp = p->FW_TextureView[p->textureviewTOS]; break;
        default: {
            loadIdentityMatrix(mat);
        printf ("invalid mode sent in it is %d, expected one of %d %d %d\n",ty,GL_PROJECTION_MATRIX,GL_MODELVIEW_MATRIX,GL_TEXTURE_MATRIX);
            return;}
    }
    memcpy((void *)mat, (void *) dp, sizeof (GLDOUBLE) * MATRIX_SIZE);
}

static void shaderErrorLog(GLuint myShader, char *which) {
    #if defined  (GL_VERSION_2_0) || defined (GL_ES_VERSION_2_0)
        #define MAX_INFO_LOG_SIZE 512
        GLchar infoLog[MAX_INFO_LOG_SIZE];
        char outline[MAX_INFO_LOG_SIZE*2];
        glGetShaderInfoLog(myShader, MAX_INFO_LOG_SIZE, NULL, infoLog);
        sprintf(outline,"problem with %s shader: %s",which, infoLog);
//        ConsoleMessage (outline);
    #else
        ConsoleMessage ("Problem compiling shader");
    #endif
}
 
char *insertBefore(char *current, char *insert, char* wholeBuffer, int wholeBuffersize){
    //inserts insert at current location
    char *newcurrent, *here;
    int insertlen, wholelen, need, movesize;

    wholelen = strlen(current) +1; //plus 1 to get the \0 at the end in memmove
    insertlen = strlen(insert);
    need = wholelen + insertlen + 1;
    movesize = wholelen;
    newcurrent = current;
    if(need < wholeBuffersize){
        here = current;
        newcurrent = current + insertlen;
        memmove(newcurrent,current,movesize);
        memcpy(here,insert,insertlen);
    }else{
        ConsoleMessage("Not enough buffer for compositing shader buffsz=%d need=%d\n",wholeBuffersize,need);
    }
    //if(1){
    //    char *tmp = strdup(wholeBuffer);
    //    printf("strdup: %s",tmp);
    //    free(tmp);
    //}
    return newcurrent;
}

void removeAt(char *here, int len){
    //removes len bytes from string, moving tail bytes up by len bytes
    int wholelen, movesize;
    char *source;
    wholelen = strlen(here) + 1; //take the \0
    source = here + len;
    movesize = wholelen - len;
    memmove(here,source,movesize);
}

void replaceAll(char *buffer,int bsize, char *oldstr, char *newstr){
    char *found;
    while((found = strstr(buffer,oldstr))){
        removeAt(found,strlen(oldstr));
        insertBefore(found,newstr,buffer,bsize);
    }

}

void extractPlugCall(char *start, char *PlugName,char *PlugParameters){
    //from the addon shader, get the name PLUG_<name>(declaredParameters)
    char *pns, *pne, *pps, *ppe;
    int len;
    pns = strstr(start,"PLUG: ");
    pns += strlen("PLUG: ");
    pne = strchr(pns,' ');
    len = pne - pns;
    strncpy(PlugName,pns,len);
    PlugName[len] = '\0';
    pps = strchr(pne,'(');
    ppe = strchr(pps,')') + 1;
    len = ppe - pps;
    strncpy(PlugParameters,pps,len);
    PlugParameters[len] = '\0';
    //printf("PlugName %s PlugParameters %s\n",PlugName,PlugParameters);
}

int LookForPlugDeclarations( char * CodeForPlugDeclarations, int bsize, char *PlugName, char *ProcedureName, char *ForwardDeclaration) {
    //in the main code, look for matching PLUG: <PlugName>(plugparams) and place a call to ProcedureName(plugparams)
    //Result := false
    //for each S: string in CodeForPlugDeclaration do
    //begin
    int Result;
    //i = 0;
    //while(CodeForPlugDeclarations[i]){
        char *S;
        char MainPlugName[100], MainPlugParams[1000];
        //char PlugDeclarationBuffer[10000];
        int AnyOccurrencesHere = FALSE; //:= false
    Result = FALSE;
        //strcpy_s(PlugDeclarationBuffer,10000,*CodeForPlugDeclarations);
        S = CodeForPlugDeclarations;
        do {
            //while we can find an occurrence
            //  of /* PLUG: PlugName (...) */ inside S do
            //begin
            S = strstr(S,"/* PLUG: ");
            //if(S)
            //    printf("found PLUG:\n");
            //else
            //    printf("PLUG: not found\n");
            if(S){  ////where = strstr(haystack,needle)
                char ProcedureCallBuffer[500], *ProcedureCall;
                extractPlugCall(S,MainPlugName,MainPlugParams);
                if(!strcmp(MainPlugName,PlugName)){
                    //found the place in the main shader to make the call to addon function
                    //insert into S a call to ProcedureName,
                    //with parameters specified inside the /* PLUG: PlugName (...) */,
                    //right before the place where we found /* PLUG: PlugName (...) */
                    ProcedureCall = ProcedureCallBuffer;
                    sprintf(ProcedureCall,"%s%s;\n",ProcedureName,MainPlugParams);
                    S = insertBefore(S,ProcedureCall,CodeForPlugDeclarations,bsize);
                    AnyOccurrencesHere = TRUE; //:= true
                    Result = TRUE; //:= true
                }else{
                    //printf("found a PLUG: %s but doesn't match PLUG_%s\n",MainPlugName,PlugName);
                }
                S += strlen("/* PLUG:") + strlen(MainPlugName) + strlen(MainPlugParams);
            }
        }
        while(S); //AnyOccurrencesHere);
        //end

        //if AnyOccurrencesHere then
        if(AnyOccurrencesHere){
            //insert the PlugForwardDeclaration into S,
            //at the place of /* PLUG-DECLARATIONS */ inside
            //(or at the beginning, if no /* PLUG-DECLARATIONS */)
            S = CodeForPlugDeclarations;
            S = strstr(S,"/* PLUG-DECLARATIONS */");
            if(!S) S = CodeForPlugDeclarations;
            S = insertBefore(S,ForwardDeclaration,CodeForPlugDeclarations,bsize);
            //S = *CodeForPlugDeclarations;
            //*CodeForPlugDeclarations = strdup(PlugDeclarationBuffer);
            //FREE_IF_NZ(S);
        }else{
            printf("didn't find PLUG_%s\n",PlugName);
        }
    //    i++;
    //} //end
    return Result;
} //end

void extractPlugName(char *start, char *PlugName,char *PlugDeclaredParameters){
    //from the addon shader, get the name PLUG_<name>(declaredParameters)
    char *pns, *pne, *pps, *ppe;
    int len;
    pns = strstr(start,"PLUG_");
    pns += strlen("PLUG_");
    //pne = strchr(pns,' ');
    pne = strpbrk(pns," (");
    len = pne - pns;
    strncpy(PlugName,pns,len);
    PlugName[len] = '\0';
    pps = strchr(pne,'(');
    ppe = strchr(pps,')') + 1;
    len = ppe - pps;
    strncpy(PlugDeclaredParameters,pps,len);
    PlugDeclaredParameters[len] = '\0';
    //printf("PlugName %s PlugDeclaredParameters %s\n",PlugName,PlugDeclaredParameters);
}

void Plug( int EffectPartType, const char *PlugValue, char **CompleteCode, int *unique_int)
{
    //Algo: 
    // outer loop: search for PLUG_<name> in (addon) effect
    //   inner loop: search for matching PLUG: <name> in main shader
    //     if found, paste addon into main:
    //        a) forward declaration at top, 
    //        b) method call at PLUG: point 
    //        c) method definition at bottom
    //var
    //  PlugName, ProcedureName, PlugForwardDeclaration: string;
    char PlugName[100], PlugDeclaredParameters[1000], PlugForwardDeclaration[1000], ProcedureName[100], PLUG_PlugName[100];
    char Code[SBUFSIZE], Plug[PBUFSIZE];
    int HasGeometryMain = FALSE, AnyOccurrences;
    char *found;
    int err;

    UNUSED(err);

    //var
    // Code: string list;
    //begin
    
    if(!CompleteCode[EffectPartType]) return;
    err = fw_strcpy_s(Code,SBUFSIZE, CompleteCode[EffectPartType]);
    err = fw_strcpy_s(Plug,PBUFSIZE, PlugValue);

    //HasGeometryMain := HasGeometryMain or
    //  ( EffectPartType = geometry and
    //    PlugValue contains 'main()' );
    HasGeometryMain = HasGeometryMain || 
        ((EffectPartType == SHADERPART_GEOMETRY) && strstr("main(",Plug));

    //while we can find PLUG_xxx inside PlugValue do
    //begin
    found = Plug;
    do {
        found = strstr(found,"void PLUG_"); //where = strstr(haystack,needle)
        //if(!found)
        //    printf("I'd like to complain: void PLUG_ isn't found in the addon\n");
        if(found){
            //PlugName := the plug name we found, the "xxx" inside PLUG_xxx
            //PlugDeclaredParameters := parameters declared at PLUG_xxx function
            extractPlugName(found,PlugName,PlugDeclaredParameters);
            found += strlen("void PLUG_") + strlen(PlugName) + strlen(PlugDeclaredParameters);
            //{ Rename found PLUG_xxx to something unique. }
            //ProcedureName := generate new unique procedure name,
            //for example take 'plugged_' + some unique integer
            snprintf(ProcedureName,999,"%s_%d",PlugName,(*unique_int));
            (*unique_int)++;

            //replace inside PlugValue all occurrences of 'PLUG_' + PlugName
            //with ProcedureName
            snprintf(PLUG_PlugName,99,"%s%s","PLUG_",PlugName);
            replaceAll(Plug,PBUFSIZE,PLUG_PlugName,ProcedureName);

            //PlugForwardDeclaration := 'void ' + ProcedureName +
            //PlugDeclaredParameters + ';' + newline
            snprintf(PlugForwardDeclaration,999,"void %s%s;\n",ProcedureName,PlugDeclaredParameters);

            //AnyOccurrences := LookForPlugDeclaration(Code)
            AnyOccurrences = LookForPlugDeclarations(Code,SBUFSIZE, PlugName,ProcedureName,PlugForwardDeclaration);

            /* If the plug declaration is not found in Code, then try to find it
                in the final shader. This happens if Code is special for given
                light/texture effect, but PLUG_xxx is not special to the
                light/texture effect (it is applicable to the whole shape as well).
                For example, using PLUG_vertex_object_space inside
                the X3DTextureNode.effects. 
            */
            //if not AnyOccurrences and
            //    Code <> Source[EffectPartType] then
            //    AnyOccurrences := LookForPlugDeclaration(Source[EffectPartType])
            //if(!AnyOccurrences && Code != Source[EffectPartType]){
            //    AnyOccurrences = LookForPlugDeclarations(Source[EffectPartType]);
            //}
            //if not AnyOccurrences then
            //    Warning('Plug name ' + PlugName + ' not declared')
            //}
            if(!AnyOccurrences){
                ConsoleMessage("Plug name %s not declared\n",PlugName);
            }
        }
    }while(found);
    //end

    /*{ regardless if any (and how many) plug points were found,
    always insert PlugValue into Code. This way EffectPart with a library
    of utility functions (no PLUG_xxx inside) also works. }*/
    //Code.Add(PlugValue)
    //printf("strlen Code = %d strlen PlugValue=%d\n",strlen(Code),strlen(PlugValue));
    err = fw_strcat_s(Code,SBUFSIZE,Plug);
    FREE_IF_NZ(CompleteCode[EffectPartType]);
    CompleteCode[EffectPartType] = strdup(Code);
} //end

void EnableEffect(struct Node *node, char **CompletedCode, int *unique_int){
    int i, ipart;
    const char *str;

    ipart=SHADERPART_VERTEX;
    NodeEffect *effect = (NodeEffect *)node;
    for(i=0;i<effect->parts()->getSize();i++){
        NodeEffectPart *part = (NodeEffectPart*)effect->parts()->getValue(i);
        if(part->getType() == KAMBI_EFFECT_PART){
            if(!strcmp(part->type()->getValue(),"FRAGMENT"))
                ipart = SHADERPART_FRAGMENT;
            else if(!strcmp(part->type()->getValue(),"VERTEX"))
                ipart = SHADERPART_VERTEX;
            str = part->url()->getValue(0).getData();
            if(!strncmp(str,"data:text/plain,",strlen("data:text/plain,")))
                str += strlen("data:text/plain,");
            Plug(ipart,str, CompletedCode, unique_int);
        }
    }
}

const static GLchar *fragTCGTDefs = TEXTURECOORDINATEGENERATORDefs;

/* VERTEX inputs */

static const GLchar *vertPosDec = "\
    attribute      vec4 fw_Vertex; \n \
    uniform         mat4 fw_ModelViewMatrix; \n \
    uniform         mat4 fw_ProjectionMatrix; \n ";

static const GLchar *vertNormDec = " \
    uniform        mat3 fw_NormalMatrix;\n \
    attribute      vec3 fw_Normal; \n";

static const GLchar *vertSimColDec = "\
    attribute  vec4 fw_Color;\n ";

static const GLchar *vertTexMatrixDec = "\
    uniform mat4 fw_TextureMatrix0;\n";

static const GLchar *vertTexCoordGenDec ="\
    uniform int fw_textureCoordGenType;\n";

static const GLchar *vertTexCoordDec = "\
    attribute vec2 fw_MultiTexCoord0;\n";

static const GLchar *vertOneMatDec = "\
    uniform fw_MaterialParameters\n\
    fw_FrontMaterial; \n";
static const GLchar *vertBackMatDec = "\
    uniform fw_MaterialParameters fw_BackMaterial; \n";
static const GLchar *fragSimColAss = "finalFrag = v_front_colour * finalFrag;\n ";


/* VERTEX outputs */

static const GLchar *vecNormPos = " \
    vec3 vertexNorm; \
    vec4 vertexPos; \n";

static const GLchar *varyingNormPos = " \
    varying vec3 vertexNorm; \
    varying vec4 vertexPos; \n";

static const GLchar *varyingTexCoord = "\
    varying vec3 fw_TexCoord[4];\n";

static const GLchar *varyingFrontColour = "\
    varying vec4    v_front_colour; \n";

static const GLchar *varyingHatchPosition = "\
    varying vec2 hatchPosition; \n";

/* VERTEX Calculations */

static const GLchar *vertMainStart = "void main(void) { \n";

static const GLchar *vertEnd = "}";

static const GLchar *vertPos = "gl_Position = fw_ProjectionMatrix * fw_ModelViewMatrix * fw_Vertex;\n ";

static const GLchar *vertNormPosCalc = "\
    vertexNorm = normalize(fw_NormalMatrix * fw_Normal);\n \
    vertexPos = fw_ModelViewMatrix * fw_Vertex;\n ";

static const GLchar *vertSimColUse = "v_front_colour = fw_Color; \n";

static const GLchar *vertEmissionOnlyColourAss = "v_front_colour = fw_FrontMaterial.emission;\n";
static const GLchar *vertSingTexCalc = "fw_TexCoord[0] = vec3(vec4(fw_TextureMatrix0 *vec4(fw_MultiTexCoord0,0,0))).stp;\n";

static const GLchar *vertSingTexCubeCalc = "\
    vec3 u=normalize(vec3(fw_ProjectionMatrix * fw_Vertex)); /* myEyeVertex */ \
    /* vec3 n=normalize(vec3(fw_NormalMatrix*fw_Normal)); \
    fw_TexCoord[0] = reflect(u,n); myEyeNormal */ \n \
    /* v_texC = reflect(normalize(vec3(vertexPos)),vertexNorm);\n */ \
    fw_TexCoord[0] = reflect(u,vertexNorm);\n";

//dug9 Jan 5, 2014 static const GLchar *fragMainStart = "void main() { vec4 finalFrag = vec4(0.,0.,0.,0.);\n";
static const GLchar *fragMainStart = "void main() { vec4 finalFrag = vec4(1.,1.,1.,1.);\n";
static const GLchar *anaglyphGrayFragEnd =    "float gray = dot(finalFrag.rgb, vec3(0.299, 0.587, 0.114)); \n \
        gl_FragColor = vec4(gray, gray, gray, finalFrag.a);}";

// START MIT, VOLUME RENDERING >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

/* Generic GLSL vertex shader, used on OpenGL ES. */
static const GLchar *volumeVertexGLES2 = " \n\
uniform mat4 fw_ModelViewMatrix; \n\
uniform mat4 fw_ProjectionMatrix; \n\
attribute vec4 fw_Vertex; \n\
 \n\
/* PLUG-DECLARATIONS */ \n\
 \n\
varying vec4 castle_vertex_eye; \n\
varying vec4 castle_Color; \n\
 \n\
void main(void) \n\
{ \n\
  vec4 vertex_object = fw_Vertex; \n\
  vec3 normal_object = vec3(0.0); \n\
  /* PLUG: vertex_object_space (vertex_object, normal_object) */ \n\
  castle_vertex_eye = fw_ModelViewMatrix * vertex_object; \n\
   \n\
   castle_Color = vec4(1.0,.5,.5,1.0); \n\
  \n\
  gl_Position = fw_ProjectionMatrix * castle_vertex_eye; \n\
   \n\
} \n\
";

/* Generic GLSL fragment shader, used on OpenGL ES. */
static const GLchar *volumeFragmentGLES2 = " \n\
/* DEFINES */ \n\
#ifdef MOBILE \n\
//precision highp float; \n\
precision mediump float; \n\
#endif //MOBILE \n\
 \n\
 vec4 HeatMapColor(float value, float minValue, float maxValue) \n\
{ \n\
    //used for debugging. If min=0,max=1 then magenta is 0, blue,green,yellow, red is 1 \n\
    vec4 ret; \n\
    int HEATMAP_COLORS_COUNT; \n\
    vec4 colors[6]; \n\
    HEATMAP_COLORS_COUNT = 6; \n\
    colors[0] = vec4(0.32, 0.00, 0.32, 1.0); \n\
    colors[1] = vec4( 0.00, 0.00, 1.00, 1.00); \n\
    colors[2] = vec4(0.00, 1.00, 0.00, 1.00); \n\
    colors[3] = vec4(1.00, 1.00, 0.00, 1.00); \n\
    colors[4] = vec4(1.00, 0.60, 0.00, 1.00); \n\
    colors[5] = vec4(1.00, 0.00, 0.00, 1.00); \n\
    float ratio=(float(HEATMAP_COLORS_COUNT)-1.0)*clamp((value-minValue)/(maxValue-minValue),0.0,1.0); \n\
    int indexMin=int(floor(ratio)); \n\
    int indexMax= indexMin+1 < HEATMAP_COLORS_COUNT-1 ? indexMin+1 : HEATMAP_COLORS_COUNT-1; \n\
    ret = mix(colors[indexMin], colors[indexMax], ratio-float(indexMin)); \n\
    if(value < minValue) ret = vec4(0.0,0.0,0.0,1.0); \n\
    if(value > maxValue) ret = vec4(1.0,1.0,1.0,1.0); \n\
    return ret; \n\
} \n\
vec4 debug_color; \n\
float hash( float n ) \n\
{ \n\
    return fract(sin(n)*43758.5453); \n\
} \n\
float noise( vec3 xyz ) \n\
{ \n\
    // The noise function returns a value in the range -1.0f -> 1.0f \n\
    vec3 p = floor(xyz); \n\
    vec3 f = fract(xyz); \n\
    \n\
    f = f*f*(3.0-2.0*f); \n\
    float n = p.x + p.y*57.0 + 113.0*p.z; \n\
    \n\
    return mix(mix(mix( hash(n+0.0), hash(n+1.0),f.x), \n\
                   mix( hash(n+57.0), hash(n+58.0),f.x),f.y), \n\
               mix(mix( hash(n+113.0), hash(n+114.0),f.x), \n\
                   mix( hash(n+170.0), hash(n+171.0),f.x),f.y),f.z); \n\
} \n\
vec3 noise3( in vec3 xyz, in float range ){ \n\
    vec3 rxyz = vec3(xyz); \n\
    rxyz.x += noise(xyz)*range; \n\
    rxyz.y += noise(xyz)*range; \n\
    rxyz.z += noise(xyz)*range; \n\
    return rxyz; \n\
} \n\
 \n\
varying vec4 castle_vertex_eye; \n\
varying vec4 castle_Color; \n\
uniform mat4 fw_ModelViewProjInverse; \n\
//uniform float fw_FocalLength; \n\
uniform vec4 fw_viewport; \n\
uniform vec3 fw_dimensions; \n\
//uniform vec3 fw_RayOrigin; \n\
uniform sampler2D fw_Texture_unit0; \n\
uniform sampler2D fw_Texture_unit1; \n\
uniform sampler2D fw_Texture_unit2; \n\
uniform sampler2D fw_Texture_unit3; \n\
#ifdef TEX3D \n\
uniform int tex3dTiles[3]; \n\
uniform int repeatSTR[3]; \n\
uniform int magFilter; \n\
#endif //TEX3D \n\
#ifdef SEGMENT \n\
uniform int fw_nIDs; \n\
uniform int fw_enableIDs[10]; \n\
uniform int fw_surfaceStyles[2]; \n\
uniform int fw_nStyles; \n\
vec4 texture3Demu( sampler2D sampler, in vec3 texcoord3); \n\
vec4 texture3Demu0( sampler2D sampler, in vec3 texcoord3, int magfilter); \n\
bool inEnabledSegment(in vec3 texcoords, inout int jstyle){ \n\
    bool inside = true; \n\
    jstyle = 1; //DEFAULT \n\
    vec4 segel = texture3Demu0(fw_Texture_unit1,texcoords,0); \n\
    //convert from GL_FLOAT 0-1 to int 0-255 \n\
    //Q. is there a way to do int images in GLES2? \n\
    int ID = int(floor(segel.a * 255.0 + .1)); \n\
    //debug_color = HeatMapColor(float(ID),0.0,5.0); \n\
    //debug_color.a = .2; \n\
    if(ID < fw_nIDs){ \n\
        //specs: The indices of this array corresponds to the segment identifier. \n\
        inside = fw_enableIDs[ID] == 0 ? false : true; \n\
    } \n\
    if(inside){ \n\
        int kstyle = fw_nStyles-1; \n\
        kstyle = ID < fw_nStyles ? ID : kstyle; \n\
        jstyle = fw_surfaceStyles[kstyle]; \n\
        jstyle = jstyle == 1 ? 0 : jstyle; \n\
    } \n\
    return inside; \n\
} \n\
#endif //SEGMENT \n\
#ifdef ISO \n\
uniform float fw_stepSize; \n\
uniform float fw_tolerance; \n\
uniform float fw_surfaceVals[]; \n\
uniform int fw_nVals; \n\
uniform int fw_surfaceStyles[]; \n\
uniform int fw_nStyles; \n\
#endif //ISO \n\
 \n\
struct Ray { \n\
  vec3 Origin; \n\
  vec3 Dir; \n\
}; \n\
struct AABB { \n\
  vec3 Min; \n\
  vec3 Max; \n\
}; \n\
bool IntersectBox(Ray r, AABB aabb, out float t0, out float t1) \n\
{ \n\
    vec3 invR = 1.0 / r.Dir; \n\
    vec3 tbot = invR * (aabb.Min-r.Origin); \n\
    vec3 ttop = invR * (aabb.Max-r.Origin); \n\
    vec3 tmin = min(ttop, tbot); \n\
    vec3 tmax = max(ttop, tbot); \n\
    vec2 t = max(tmin.xx, tmin.yz); \n\
    t0 = max(t.x, t.y); \n\
    t = min(tmax.xx, tmax.yz); \n\
    t1 = min(t.x, t.y); \n\
    return t0 <= t1; \n\
} \n\
/* PLUG-DECLARATIONS */ \n\
 \n\
vec3 fw_TexCoord[1]; \n\
#ifdef CLIP \n\
#define FW_MAXCLIPPLANES 4 \n\
uniform int fw_nclipplanes; \n\
uniform vec4 fw_clipplanes[FW_MAXCLIPPLANES]; \n\
bool clip (in vec3 vertex_object){ \n\
  bool iclip = false; \n\
  for ( int i=0; i<fw_nclipplanes; i++ ) { \n\
    if( dot( fw_clipplanes[i], vec4(vertex_object,1.0)) < 0.0) \n\
        iclip = true; \n\
  } \n\
  return iclip; \n\
} \n\
#endif //CLIP \n\
vec3 vertex_eye; \n\
vec3 normal_eye; \n\
vec4 raysum; \n\
void main(void) \n\
{ \n\
    debug_color = vec4(0.0); \n\
    float maxDist = length(fw_dimensions); //1.414214; //sqrt(2.0); \n\
    int numSamples = 128; \n\
    float fnumSamples = float(numSamples); \n\
    float stepSize = maxDist/fnumSamples; \n\
    float densityFactor = 5.0/fnumSamples; //.88; // 1.0=normal H3D, .5 see deeper  \n\
     \n\
    vec4 fragment_color; \n\
    //vec4 raysum; \n\
    vec3 rayDirection; \n\
    //convert window to frustum \n\
    rayDirection.xy = 2.0 * (gl_FragCoord.xy - fw_viewport.xy) / fw_viewport.zw - vec2(1.0); \n\
    rayDirection.z = 0.0; \n\
    vec3 rayOrigin; // = fw_RayOrigin; \n\
    //the equivalent of gluUnproject \n\
    //by unprojecting 2 points on ray here, this should also work with ortho viewpoint \n\
    vec4 ray4 = vec4(rayDirection,1.0); \n\
    vec4 org4 = ray4; \n\
    //if I back up the ray origin by -1.0 the front plane clipping works properly \n\
    ray4.z = 0.0; //1.0; \n\
    org4.z = -1.0; //0.0; \n\
    ray4 = fw_ModelViewProjInverse * ray4; \n\
    org4 = fw_ModelViewProjInverse * org4; \n\
    ray4 /= ray4.w; \n\
    org4 /= org4.w; \n\
    rayDirection = normalize(ray4.xyz - org4.xyz); \n\
    rayOrigin = org4.xyz; \n\
    \n\
    Ray eye = Ray( rayOrigin, rayDirection); \n\
    vec3 half_dimensions = fw_dimensions * .5; \n\
    vec3 minus_half_dimensions = half_dimensions * -1.0; \n\
    AABB aabb = AABB(minus_half_dimensions,half_dimensions); \n\
    \n\
    float tnear, tfar; \n\
    IntersectBox(eye, aabb, tnear, tfar); \n\
    if (tnear < 0.0) tnear = 0.0; \n\
    vec3 rayStart = eye.Origin + eye.Dir * tnear; \n\
    vec3 rayStop = eye.Origin + eye.Dir * tfar; \n\
    // Perform the ray marching: \n\
    vec3 pos = rayStart; \n\
    vec3 step = normalize(rayStop-rayStart) * stepSize; \n\
    float totaltravel = distance(rayStop, rayStart); \n\
    float travel = totaltravel; \n\
    float T = 1.0; \n\
    vec3 Lo = vec3(0.0); \n\
    normal_eye = rayDirection; \n\
    vec3 pos2 = pos; \n\
    // Transform from object space to texture coordinate space: \n\
    pos2 = (pos2+half_dimensions)/fw_dimensions; \n\
    pos2 = clamp(pos2,0.001,.999); \n\
    raysum = vec4(0.0); \n\
    float depth = 0.0; \n\
    float lastdensity; \n\
    float lastdensity_iso; \n\
    \n\
    for (int i=0; i < numSamples; ++i) { \n\
        //raysum = HeatMapColor(travel,0.0,2.0); \n\
        //break; \n\
       // ...lighting and absorption stuff here... \n\
        pos2 = pos; \n\
        vertex_eye = pos2; \n\
        // Transform from object space to texture coordinate space: \n\
        pos2 = (pos2+half_dimensions)/fw_dimensions; \n\
        //pos2.z = 1.0 - pos2.z; //RHS to LHS \n\
        pos2 = clamp(pos2,0.001,.999); \n\
        vec3 texcoord3 = pos2; \n\
        bool iclip = false; \n\
        #ifdef CLIP \n\
        iclip = clip(vertex_eye); //clip(totaltravel - travel); \n\
        #endif //CLIP \n\
        if(!iclip) { \n\
            fragment_color = vec4(1.0,0.0,1.0,1.0); //do I need a default? seems not \n\
            /* PLUG: texture3D ( fragment_color, texcoord3) */ \n\
            #ifdef SEGMENT \n\
            int jstyle = 1; \n\
            if(inEnabledSegment(texcoord3,jstyle)){ \n\
            #endif //SEGMENT \n\
            //assuming we had a scalar input image and put L into .a, \n\
            // and computed gradient and put in .rgb : \n\
            float density = fragment_color.a; //recover the scalar value \n\
            vec3 gradient = fragment_color.rgb - vec3(.5,.5,.5); //we added 127 to (-127 to 127) in CPU gradient computation\n\
            //vec4 voxel = vec4(density,density,density,density); //this is where the black visual voxels come from\n\
            vec4 voxel = vec4(density,density,density,density); //this is where the black visual voxels come from\n\
            \n\
            #ifdef ISO \n\
            if(i==0){ \n\
                lastdensity = density; \n\
                lastdensity_iso = 0.0; \n\
            } \n\
            int MODE = fw_nVals == 1 ? 1 : 3; \n\
            MODE = fw_stepSize != 0.0 && MODE == 1 ? 2 : 1; \n\
            #ifdef ISO_MODE3 \n\
            if(MODE == 3){ \n\
                for(int i=0;i<fw_nVals;i++){ \n\
                    float iso = fw_surfaceVals[i]; \n\
                    if( sign( density - iso) != sign( lastdensity - iso) && length(gradient) > fw_tolerance ){ \n\
                        voxel.a = 1.0; \n\
                        int jstyle = min(i,fw_nStyles-1); \n\
                        jstyle = fw_surfaceStyles[jstyle]; \n\
                        if(jstyle == 1){ \n\
                            /* PLUG: voxel_apply_DEFAULT (voxel, gradient) */ \n\
                        } else if(jstyle == 2) { \n\
                            /* PLUG: voxel_apply_OPACITY (voxel, gradient) */ \n\
                        } else if(jstyle == 3) { \n\
                            /* PLUG: voxel_apply_BLENDED (voxel, gradient) */ \n\
                        } else if(jstyle == 4) { \n\
                            /* PLUG: voxel_apply_BOUNDARY (voxel, gradient) */ \n\
                        } else if(jstyle == 5) { \n\
                            /* PLUG: voxel_apply_CARTOON (voxel, gradient) */ \n\
                        } else if(jstyle == 6) { \n\
                            /* PLUG: voxel_apply_DEFAULT (voxel, gradient) */ \n\
                        } else if(jstyle == 7) { \n\
                            /* PLUG: voxel_apply_EDGE (voxel, gradient) */ \n\
                        } else if(jstyle == 8) { \n\
                            /* PLUG: voxel_apply_PROJECTION (voxel, gradient) */ \n\
                        } else if(jstyle == 9) { \n\
                            /* PLUG: voxel_apply_SHADED (voxel, gradient) */ \n\
                        } else if(jstyle == 10) { \n\
                            /* PLUG: voxel_apply_SILHOUETTE (voxel, gradient) */ \n\
                        } else if(jstyle == 11) { \n\
                            /* PLUG: voxel_apply_TONE (voxel, gradient) */ \n\
                        } \n\
                    } else { \n\
                        voxel = vec4(0.0); //similar to discard \n\
                    } \n\
                } \n\
                lastdensity = density; \n\
            } \n\
            #else //ISO_MODE3 \n\
            if(MODE == 1){ \n\
                float iso = fw_surfaceVals[0]; \n\
                if( sign( density - iso) != sign( lastdensity - iso) && length(gradient) > fw_tolerance ){ \n\
                    //debug_color = HeatMapColor(iso,0.0,.3); \n\
                    voxel.a = 1.0; \n\
                    /* PLUG: voxel_apply (voxel, gradient) */ \n\
                } else { \n\
                    voxel = vec4(0.0); //similar to discard \n\
                } \n\
                lastdensity = density; \n\
            } else if(MODE == 2){ \n\
                float iso = fw_surfaceVals[0]; \n\
                float density_iso = density / fw_stepSize; \n\
                if( sign( density_iso - iso) != sign( lastdensity_iso - iso) && length(gradient) > fw_tolerance ){ \n\
                    voxel.a = 1.0; \n\
                    /* PLUG: voxel_apply (voxel, gradient) */ \n\
                } else { \n\
                    voxel = vec4(0.0); //similar to discard \n\
                } \n\
                lastdensity = density; \n\
                lastdensity_iso = density_iso; \n\
            }  \n\
            #endif //ISO_MODE3 \n\
            #else //ISO \n\
            #ifdef SEGMENT \n\
            //debug_color = HeatMapColor(float(jstyle),1.0,12.0); \n\
            //debug_color.a = .2; \n\
            if(jstyle == 1){ \n\
                /* PLUG: voxel_apply_DEFAULT (voxel, gradient) */ \n\
            } else if(jstyle == 2) { \n\
                /* PLUG: voxel_apply_OPACITY (voxel, gradient) */ \n\
            } else if(jstyle == 3) { \n\
                /* PLUG: voxel_apply_BLENDED (voxel, gradient) */ \n\
            } else if(jstyle == 4) { \n\
                /* PLUG: voxel_apply_BOUNDARY (voxel, gradient) */ \n\
            } else if(jstyle == 5) { \n\
                /* PLUG: voxel_apply_CARTOON (voxel, gradient) */ \n\
            } else if(jstyle == 6) { \n\
                /* PLUG: voxel_apply_DEFAULT (voxel, gradient) */ \n\
            } else if(jstyle == 7) { \n\
                /* PLUG: voxel_apply_EDGE (voxel, gradient) */ \n\
            } else if(jstyle == 8) { \n\
                /* PLUG: voxel_apply_PROJECTION (voxel, gradient) */ \n\
            } else if(jstyle == 9) { \n\
                /* PLUG: voxel_apply_SHADED (voxel, gradient) */ \n\
            } else if(jstyle == 10) { \n\
                /* PLUG: voxel_apply_SILHOUETTE (voxel, gradient) */ \n\
            } else if(jstyle == 11) { \n\
                /* PLUG: voxel_apply_TONE (voxel, gradient) */ \n\
            } \n\
            #else //SEGMENT \n\
            //non-iso rendering styles \n\
            //void PLUG_voxel_apply (inout vec4 voxel, inout vec3 gradient) \n\
            /* PLUG: voxel_apply (voxel, gradient) */ \n\
            #endif //SEGMENT \n\
            #endif //ISO \n\
            density = voxel.a; \n\
            //debug_color = HeatMapColor(densityFactor,0.134,.135); \n\
            T = (1.0 - raysum.a); \n\
            raysum.a += density * T; \n\
            raysum.rgb += voxel.rgb  * T * density; \n\
            if(raysum.a > .99) { \n\
                break; \n\
            } \n\
            #ifdef SEGMENT \n\
            } //if inEnabledSegment \n\
            #endif //SEGMENT \n\
        } //iclip \n\
        travel -= stepSize; \n\
        depth += stepSize; \n\
        if(travel <= 0.0) break; \n\
        pos += step; \n\
        \n\
    }  \n\
    //void PLUG_ray_apply (inout vec4 raysum) \n\
    /* PLUG: ray_apply (raysum) */ \n\
    if(true) gl_FragColor = raysum; \n\
    else gl_FragColor = debug_color; \n\
} \n\
";

const char *getVolumeVertex(void){
    return volumeVertexGLES2; //genericVertexDesktop
}
const char *getVolumeFragment(){
    return volumeFragmentGLES2; //genericFragmentDesktop;
}

/* Generic GLSL vertex shader.
   Used by ../castlerendererinternalshader.pas to construct the final shader.

   This is converted to template.vs.inc, and is then compiled
   in program's binary.
   When you change this file, rerun `make' and then recompile Pascal sources.
*/

static const GLchar *genericVertexGLES2 = "\
/* DEFINES */ \n\
/* Generic GLSL vertex shader, used on OpenGL ES. */ \n\
 \n\
uniform mat4 fw_ModelViewMatrix; \n\
uniform mat4 fw_ProjectionMatrix; \n\
uniform mat3 fw_NormalMatrix; \n\
#ifdef CUB \n\
uniform mat4 fw_ModelViewInverseMatrix; \n\
#endif //CUB \n\
attribute vec4 fw_Vertex; \n\
attribute vec3 fw_Normal; \n\
 \n\
#ifdef TEX \n\
uniform mat4 fw_TextureMatrix0; \n\
attribute vec4 fw_MultiTexCoord0; \n\
//varying vec3 v_texC; \n\
varying vec3 fw_TexCoord[4]; \n\
#ifdef TEX3D \n\
uniform int tex3dUseVertex; \n\
#endif //TEX3D \n\
#ifdef MTEX \n\
uniform mat4 fw_TextureMatrix1; \n\
uniform mat4 fw_TextureMatrix2; \n\
uniform mat4 fw_TextureMatrix3; \n\
attribute vec4 fw_MultiTexCoord1; \n\
attribute vec4 fw_MultiTexCoord2; \n\
attribute vec4 fw_MultiTexCoord3; \n\
#endif //MTEX \n\
#ifdef TGEN \n\
 #define TCGT_CAMERASPACENORMAL    0  \n\
 #define TCGT_CAMERASPACEPOSITION    1 \n\
 #define TCGT_CAMERASPACEREFLECTION    2 \n\
 #define TCGT_COORD    3 \n\
 #define TCGT_COORD_EYE    4 \n\
 #define TCGT_NOISE    5 \n\
 #define TCGT_NOISE_EYE    6 \n\
 #define TCGT_SPHERE    7 \n\
 #define TCGT_SPHERE_LOCAL    8 \n\
 #define TCGT_SPHERE_REFLECT    9 \n\
 #define TCGT_SPHERE_REFLECT_LOCAL    10 \n\
 uniform int fw_textureCoordGenType; \n\
#endif //TGEN \n\
#endif //TEX \n\
#ifdef FILL \n\
varying vec2 hatchPosition; \n\
#endif //FILL \n\
\n\
/* PLUG-DECLARATIONS */ \n\
 \n\
varying vec4 castle_vertex_eye; \n\
varying vec3 castle_normal_eye; \n\
varying vec4 castle_Color; //DA diffuse ambient term \n\
 \n\
//uniform float castle_MaterialDiffuseAlpha; \n\
//uniform float castle_MaterialShininess; \n\
/* Color summed with all the lights. \n\
   Like gl_Front/BackLightModelProduct.sceneColor: \n\
   material emissive color + material ambient color * global (light model) ambient. \n\
*/ \n\
\n\
#ifdef LITE \n\
#define MAX_LIGHTS 8 \n\
uniform int lightcount; \n\
//uniform float lightRadius[MAX_LIGHTS]; \n\
uniform int lightType[MAX_LIGHTS];//ANGLE like this \n\
struct fw_LightSourceParameters { \n\
  vec4 ambient;  \n\
  vec4 diffuse;   \n\
  vec4 specular; \n\
  vec4 position;   \n\
  vec4 halfVector;  \n\
  vec4 spotDirection; \n\
  float spotBeamWidth; \n\
  float spotCutoff; \n\
  vec3 Attenuations; \n\
  float lightRadius; \n\
}; \n\
\n\
uniform fw_LightSourceParameters fw_LightSource[MAX_LIGHTS] /* gl_MaxLights */ ;\n\
#endif //LITE \n\
\n\
//uniform vec3 castle_SceneColor; \n\
//uniform vec4 castle_UnlitColor; \n\
#ifdef UNLIT \n\
uniform vec4 fw_UnlitColor; \n\
#endif //UNLIT \n\
#ifdef LIT \n\
struct fw_MaterialParameters { \n\
  vec4 emission; \n\
  vec4 ambient; \n\
  vec4 diffuse; \n\
  vec4 specular; \n\
  float shininess; \n\
}; \n\
uniform fw_MaterialParameters fw_FrontMaterial; \n\
varying vec3 castle_ColorES; //emissive shininess term \n\
vec3 castle_Emissive; \n\
#ifdef TWO \n\
uniform fw_MaterialParameters fw_BackMaterial; \n\
#endif //TWO \n\
#endif //LIT \n\
#ifdef FOG \n\
struct fogParams \n\
{  \n\
  vec4 fogColor; \n\
  float visibilityRange; \n\
  float fogScale; //applied on cpu side to visrange \n\
  int fogType; // 0 None, 1= FOGTYPE_LINEAR, 2 = FOGTYPE_EXPONENTIAL \n\
  // ifdefed int haveFogCoords; \n\
}; \n\
uniform fogParams fw_fogparams; \n\
#ifdef FOGCOORD \n\
attribute float fw_FogCoords; \n\
#endif \n\
#endif //FOG \n\
float castle_MaterialDiffuseAlpha; \n\
float castle_MaterialShininess; \n\
vec3 castle_SceneColor; \n\
vec4 castle_UnlitColor; \n\
vec4 castle_Specular; \n\
 \n\
#ifdef CPV \n\
attribute vec4 fw_Color; //castle_ColorPerVertex; \n\
varying vec4 cpv_Color; \n\
#endif //CPV \n\
#ifdef PARTICLE \n\
uniform vec3 particlePosition; \n\
uniform int fw_ParticleGeomType; \n\
#endif //PARTICLE \n\
 \n\
 vec3 dehomogenize(in mat4 matrix, in vec4 vector){ \n\
    vec4 tempv = vector; \n\
    if(tempv.w == 0.0) tempv.w = 1.0; \n\
    vec4 temp = matrix * tempv; \n\
    float winv = 1.0/temp.w; \n\
    return temp.xyz * winv; \n\
 } \n\
void main(void) \n\
{ \n\
  #ifdef LIT \n\
  castle_MaterialDiffuseAlpha = fw_FrontMaterial.diffuse.a; \n\
  #ifdef TEX \n\
  #ifdef TAREP \n\
  //to modulate or not to modulate, this is the question \n\
  //in here, we turn off modulation and use image alpha \n\
  castle_MaterialDiffuseAlpha = 1.0; \n\
  #endif //TAREP \n\
  #endif //TEX \n\
  castle_MaterialShininess =    fw_FrontMaterial.shininess; \n\
  castle_SceneColor = fw_FrontMaterial.ambient.rgb; \n\
  castle_Specular =    fw_FrontMaterial.specular; \n\
  castle_Emissive = fw_FrontMaterial.emission.rgb; \n\
  #ifdef LINE \n\
   castle_SceneColor = vec3(0.0,0.0,0.0); //line gets color from castle_Emissive \n\
  #endif //LINE\n\
  #else //LIT \n\
  //default unlits in case we dont set them \n\
  castle_UnlitColor = vec4(1.0,1.0,1.0,1.0); \n\
  castle_MaterialDiffuseAlpha = 1.0; \n\
  #endif //LIT \n\
  \n\
  #ifdef FILL \n\
  hatchPosition = fw_Vertex.xy; \n\
  #endif //FILL \n\
  \n\
  vec4 vertex_object = fw_Vertex; \n\
  #ifdef PARTICLE \n\
  if(fw_ParticleGeomType != 4){ \n\
    vertex_object.xyz += particlePosition; \n\
  } \n\
  #endif //PARTICLE \n\
  vec3 normal_object = fw_Normal; \n\
  /* PLUG: vertex_object_space_change (vertex_object, normal_object) */ \n\
  /* PLUG: vertex_object_space (vertex_object, normal_object) */ \n\
   \n\
  #ifdef CASTLE_BUGGY_GLSL_READ_VARYING \n\
  /* use local variables, instead of reading + writing to varying variables, \n\
     when VARYING_NOT_READABLE */ \n\
  vec4 temp_castle_vertex_eye; \n\
  vec3 temp_castle_normal_eye; \n\
  vec4 temp_castle_Color; \n\
  #define castle_vertex_eye temp_castle_vertex_eye \n\
  #define castle_normal_eye temp_castle_normal_eye \n\
  #define castle_Color      temp_castle_Color \n\
  #endif //CASTLE_BUGGY_GLSL_READ_VARYING \n\
  \n\
  castle_vertex_eye = fw_ModelViewMatrix * vertex_object; \n\
  #ifdef PARTICLE \n\
  //sprite: align to viewer \n\
  if(fw_ParticleGeomType == 4){ \n\
    vec4 ppos = vec4(particlePosition,1.0); \n\
    vec4 particle_eye = fw_ModelViewMatrix * ppos; \n\
    ppos.x += 1.0; \n\
    vec4 particle_eye1 = fw_ModelViewMatrix * ppos; \n\
    float pscal = length(particle_eye1.xyz - particle_eye.xyz); \n\
    castle_vertex_eye = particle_eye + pscal*vertex_object; \n\
  } \n\
  #endif //PARTICLE \n\
  castle_normal_eye = normalize(fw_NormalMatrix * normal_object); \n\
  \n\
  /* PLUG: vertex_eye_space (castle_vertex_eye, castle_normal_eye) */ \n\
   \n\
  #ifdef LIT \n\
  castle_ColorES = castle_Emissive; \n\
  castle_Color = vec4(castle_SceneColor, 1.0); \n\
  /* PLUG: add_light_contribution2 (castle_Color, castle_ColorES, castle_vertex_eye, castle_normal_eye, castle_MaterialShininess) */ \n\
  /* PLUG: add_light_contribution (castle_Color, castle_vertex_eye, castle_normal_eye, castle_MaterialShininess) */ \n\
  castle_Color.a = castle_MaterialDiffuseAlpha; \n\
  /* Clamp sum of lights colors to be <= 1. See template.fs for comments. */ \n\
  castle_Color.rgb = min(castle_Color.rgb, 1.0); \n\
  #else //LIT \n\
  castle_Color.rgb = castle_UnlitColor.rgb; \n\
  #endif //LIT \n\
  \n\
  #ifdef CPV //color per vertex \n\
  cpv_Color = fw_Color; \n\
  #endif //CPV \n\
  \n\
  #ifdef TEX \n\
  vec4 texcoord = fw_MultiTexCoord0; \n\
  #ifdef TEX3D \n\
  //to re-use vertex coords as texturecoords3D, we need them in 0-1 range: CPU calc of fw_TextureMatrix0 \n\
  if(tex3dUseVertex == 1) \n\
    texcoord = vec4(fw_Vertex.xyz,1.0); \n\
  #endif //TEX3D \n\
  #ifdef TGEN  \n\
  { \n\
    vec3 vertexNorm; \n\
    vec4 vertexPos; \n\
    vec3 texcoord3 = texcoord.xyz; \n\
    vertexNorm = normalize(fw_NormalMatrix * fw_Normal); \n\
    vertexPos = fw_ModelViewMatrix * fw_Vertex; \n\
    /* sphereEnvironMapping Calculation */  \n\
    vec3 u=normalize(vec3(vertexPos)); /* u is normalized position, used below more than once */ \n\
    vec3 r= reflect(u,vertexNorm); \n\
    if (fw_textureCoordGenType==TCGT_SPHERE) { /* TCGT_SPHERE  GL_SPHERE_MAP OpenGL Equiv */ \n\
      float m=2.0 * sqrt(r.x*r.x + r.y*r.y + (r.z*1.0)*(r.z*1.0)); \n\
      texcoord3 = vec3(r.x/m+0.5,r.y/m+0.5,0.0); \n\
    }else if (fw_textureCoordGenType==TCGT_CAMERASPACENORMAL) { \n\
      /* GL_REFLECTION_MAP used for sampling cubemaps */ \n\
      float dotResult = 2.0 * dot(u,r); \n\
      texcoord3 = vec3(u-r)*dotResult; \n\
    }else if (fw_textureCoordGenType==TCGT_COORD) { \n\
      /* 3D textures can use coords in 0-1 range */ \n\
      texcoord3 = fw_Vertex.xyz; //xyz; \n\
    } else { /* default usage - like default CubeMaps */ \n\
      vec3 u=normalize(vec3(fw_ProjectionMatrix * fw_Vertex)); /* myEyeVertex */  \n\
      texcoord3 =    reflect(u,vertexNorm); \n\
    } \n\
    texcoord.xyz = texcoord3; \n\
  } \n\
  #endif //TGEN \n\
  fw_TexCoord[0] = dehomogenize(fw_TextureMatrix0, texcoord); \n\
  #ifdef MTEX \n\
  fw_TexCoord[1] = dehomogenize(fw_TextureMatrix1,fw_MultiTexCoord1); \n\
  fw_TexCoord[2] = dehomogenize(fw_TextureMatrix2,fw_MultiTexCoord2); \n\
  fw_TexCoord[3] = dehomogenize(fw_TextureMatrix3,fw_MultiTexCoord3); \n\
  #endif //MTEX \n\
  #endif //TEX \n\
  \n\
  gl_Position = fw_ProjectionMatrix * castle_vertex_eye; \n\
  \n\
  #ifdef CUB \n\
  //cubemap \n\
  vec4 camera = fw_ModelViewInverseMatrix * vec4(0.0,0.0,0.0,1.0); \n\
  //vec3 u = normalize( vec4(castle_vertex_eye - camera).xyz ); \n\
  vec3 u = normalize( vec4(vertex_object + camera).xyz ); \n\
  vec3 v = normalize(fw_Normal); \n\
  fw_TexCoord[0] = normalize(reflect(u,v)); //computed in object space \n\
  fw_TexCoord[0].st = -fw_TexCoord[0].st; //helps with renderman cubemap convention \n\
  #endif //CUB \n\
  #ifdef CASTLE_BUGGY_GLSL_READ_VARYING \n\
  #undef castle_vertex_eye \n\
  #undef castle_normal_eye \n\
  #undef castle_Color \n\
  castle_vertex_eye = temp_castle_vertex_eye; \n\
  castle_normal_eye = temp_castle_normal_eye; \n\
  castle_Color      = temp_castle_Color; \n\
  #endif //CASTLE_BUGGY_GLSL_READ_VARYING \n\
  \n\
  #ifdef FOG \n\
  #ifdef FOGCOORD \n\
  castle_vertex_eye.z = fw_FogCoords; \n\
  #endif //FOGCOORD \n\
  #endif //FOG \n\
  #ifdef UNLIT \n\
  castle_Color = fw_UnlitColor; \n\
  #endif //UNLIT \n\
} \n";


/* Generic GLSL fragment shader.
   Used by ../castlerendererinternalshader.pas to construct the final shader.

   This is converted to template.fs.inc, and is then compiled
   in program's binary.
   When you change this file, rerun `make' and then recompile Pascal sources.
*/
/* 
    started with: http://svn.code.sf.net/p/castle-engine/code/trunk/castle_game_engine/src/x3d/opengl/glsl/template_mobile.fs
  defines:
  GL_ES_VERSION_2_0 - non-desktop
  HAS_GEOMETRY_SHADER - version 3+ gles
*/

const char *getGenericVertex(void){
        return genericVertexGLES2; //genericVertexDesktop
}

static const GLchar *genericFragmentGLES2 = "\
/* DEFINES */ \n\
#ifdef MOBILE \n\
//precision highp float; \n\
precision mediump float; \n\
#endif //MOBILE \n\
/* Generic GLSL fragment shader, used on OpenGL ES. */ \n\
 \n\
varying vec4 castle_Color; \n\
 \n\
#ifdef LITE \n\
#define MAX_LIGHTS 8 \n\
uniform int lightcount; \n\
//uniform float lightRadius[MAX_LIGHTS]; \n\
uniform int lightType[MAX_LIGHTS];//ANGLE like this \n\
struct fw_LightSourceParameters { \n\
  vec4 ambient;  \n\
  vec4 diffuse;   \n\
  vec4 specular; \n\
  vec4 position;   \n\
  vec4 halfVector;  \n\
  vec4 spotDirection; \n\
  float spotBeamWidth; \n\
  float spotCutoff; \n\
  vec3 Attenuations; \n\
  float lightRadius; \n\
}; \n\
\n\
uniform fw_LightSourceParameters fw_LightSource[MAX_LIGHTS] /* gl_MaxLights */ ;\n\
#endif //LITE \n\
\n\
#ifdef CPV \n\
varying vec4 cpv_Color; \n\
#endif //CPV \n\
\n\
#ifdef TEX \n\
#ifdef CUB \n\
uniform samplerCube fw_Texture_unit0; \n\
#else //CUB \n\
uniform sampler2D fw_Texture_unit0; \n\
#endif //CUB \n\
varying vec3 fw_TexCoord[4]; \n\
#ifdef TEX3D \n\
uniform int tex3dTiles[3]; \n\
uniform int repeatSTR[3]; \n\
uniform int magFilter; \n\
#endif //TEX3D \n\
#ifdef TEX3DLAY \n\
uniform sampler2D fw_Texture_unit1; \n\
uniform sampler2D fw_Texture_unit2; \n\
uniform sampler2D fw_Texture_unit3; \n\
uniform int textureCount; \n\
#endif //TEX3DLAY \n\
#ifdef MTEX \n\
uniform sampler2D fw_Texture_unit1; \n\
uniform sampler2D fw_Texture_unit2; \n\
uniform sampler2D fw_Texture_unit3; \n\
uniform ivec2 fw_Texture_mode0;  \n\
uniform ivec2 fw_Texture_mode1;  \n\
uniform ivec2 fw_Texture_mode2;  \n\
uniform ivec2 fw_Texture_mode3;  \n\
uniform ivec2 fw_Texture_source0;  \n\
uniform ivec2 fw_Texture_source1;  \n\
uniform ivec2 fw_Texture_source2;  \n\
uniform ivec2 fw_Texture_source3;  \n\
uniform int fw_Texture_function0;  \n\
uniform int fw_Texture_function1;  \n\
uniform int fw_Texture_function2;  \n\
uniform int fw_Texture_function3;  \n\
uniform int textureCount; \n\
uniform vec4 mt_Color; \n\
#define MTMODE_ADD    1\n \
#define MTMODE_ADDSIGNED    2\n \
#define MTMODE_ADDSIGNED2X    3\n \
#define MTMODE_ADDSMOOTH    4\n \
#define MTMODE_BLENDCURRENTALPHA    5\n \
#define MTMODE_BLENDDIFFUSEALPHA    6\n \
#define MTMODE_BLENDFACTORALPHA    7\n \
#define MTMODE_BLENDTEXTUREALPHA    8\n \
#define MTMODE_DOTPRODUCT3    9\n \
#define MTMODE_MODULATE    10\n \
#define MTMODE_MODULATE2X    11\n \
#define MTMODE_MODULATE4X    12\n \
#define MTMODE_MODULATEALPHA_ADDCOLOR    13\n \
#define MTMODE_MODULATEINVALPHA_ADDCOLOR    14\n \
#define MTMODE_MODULATEINVCOLOR_ADDALPHA    15\n \
#define MTMODE_OFF    16\n \
#define MTMODE_REPLACE    17\n \
#define MTMODE_SELECTARG1    18\n \
#define MTMODE_SELECTARG2    19\n \
#define MTMODE_SUBTRACT    20\n \
#define MTSRC_DIFFUSE    1 \n\
#define MTSRC_FACTOR    2 \n\
#define MTSRC_SPECULAR    3 \n\
#define MTFN_ALPHAREPLICATE    0 \n\
#define MTFN_COMPLEMENT    1 \n\
#define MT_DEFAULT -1 \n\
\n\
void finalColCalc(inout vec4 prevColour, in int mode, in int modea, in int func, in sampler2D tex, in vec2 texcoord) { \n\
  vec4 texel = texture2D(tex,texcoord); \n\
  vec4 rv = vec4(1.,0.,1.,1.);   \n\
  if (mode==MTMODE_OFF) {  \n\
    rv = vec4(prevColour); \n\
  } else if (mode==MTMODE_REPLACE) { \n\
    rv = vec4(texture2D(tex, texcoord)); \n\
  }else if (mode==MTMODE_MODULATE) {  \n\
    vec3 ct,cf;  \n\
    float at,af;  \n\
    cf = prevColour.rgb;  \n\
    af = prevColour.a;  \n\
    ct = texel.rgb;  \n\
    at = texel.a;  \n\
    rv = vec4(ct*cf, at*af);  \n\
  } else if (mode==MTMODE_MODULATE2X) {  \n\
    vec3 ct,cf;  \n\
    float at,af;  \n\
    cf = prevColour.rgb;  \n\
    af = prevColour.a;  \n\
    ct = texel.rgb;  \n\
    at = texel.a;  \n\
    rv = vec4(vec4(ct*cf, at*af)*vec4(2.,2.,2.,2.));  \n\
  }else if (mode==MTMODE_MODULATE4X) {  \n\
    vec3 ct,cf;  \n\
    float at,af;  \n\
    cf = prevColour.rgb; \n\
    af = prevColour.a;  \n\
    ct = texel.rgb;  \n\
    at = texel.a;  \n\
    rv = vec4(vec4(ct*cf, at*af)*vec4(4.,4.,4.,4.));  \n\
  }else if (mode== MTMODE_ADDSIGNED) { \n\
    rv = vec4 (prevColour + texel - vec4 (0.5, 0.5, 0.5, -.5));  \n\
  } else if (mode== MTMODE_ADDSIGNED2X) { \n\
    rv = vec4 ((prevColour + texel - vec4 (0.5, 0.5, 0.5, -.5))*vec4(2.,2.,2.,2.));  \n\
  } else if (mode== MTMODE_ADD) { \n\
    rv= vec4 (prevColour + texel);  \n\
  } else if (mode== MTMODE_SUBTRACT) { \n\
    rv = vec4 (texel - prevColour); //jas had prev - tex \n\
  } else if (mode==MTMODE_ADDSMOOTH) {  \n\
    rv = vec4 (prevColour + (prevColour - vec4 (1.,1.,1.,1.)) * texel);  \n\
  } else if (mode==MTMODE_BLENDDIFFUSEALPHA) {  \n\
    rv = vec4 (mix(prevColour,texel,castle_Color.a)); \n\
  } else if (mode==MTMODE_BLENDTEXTUREALPHA) {  \n\
    rv = vec4 (mix(prevColour,texel,texel.a)); \n\
  } else if (mode==MTMODE_BLENDFACTORALPHA) {  \n\
    rv = vec4 (mix(prevColour,texel,mt_Color.a)); \n\
  } else if (mode==MTMODE_BLENDCURRENTALPHA) {  \n\
    rv = vec4 (mix(prevColour,texel,prevColour.a)); \n\
  } else if (mode==MTMODE_SELECTARG1) {  \n\
    rv = texel;  \n\
  } else if (mode==MTMODE_SELECTARG2) {  \n\
    rv = prevColour;  \n\
  } \n\
  if(modea != 0){ \n\
    if (modea==MTMODE_OFF) {  \n\
      rv.a = prevColour.a; \n\
    } else if (modea==MTMODE_REPLACE) { \n\
      rv.a = 1.0; \n\
    }else if (modea==MTMODE_MODULATE) {  \n\
      float at,af;  \n\
      af = prevColour.a;  \n\
      at = texel.a;  \n\
      rv.a = at*af;  \n\
    } else if (modea==MTMODE_MODULATE2X) {  \n\
      float at,af;  \n\
      af = prevColour.a;  \n\
      at = texel.a;  \n\
      rv.a = at*af*2.0;  \n\
    }else if (modea==MTMODE_MODULATE4X) {  \n\
      float at,af;  \n\
      af = prevColour.a;  \n\
      at = texel.a;  \n\
      rv.a = at*af*4.0;  \n\
    }else if (modea== MTMODE_ADDSIGNED) { \n\
      rv.a = (prevColour.a + texel.a + .5);  \n\
    } else if (modea== MTMODE_ADDSIGNED2X) { \n\
      rv.a = ((prevColour.a + texel.a + .5))*2.0;  \n\
    } else if (modea== MTMODE_ADD) { \n\
      rv.a = prevColour.a + texel.a;  \n\
    } else if (modea== MTMODE_SUBTRACT) { \n\
      rv.a = texel.a - prevColour.a;  //jas had prev - texel \n\
    } else if (modea==MTMODE_ADDSMOOTH) {  \n\
      rv.a = (prevColour.a + (prevColour.a - 1.)) * texel.a;  \n\
    } else if (modea==MTMODE_BLENDDIFFUSEALPHA) {  \n\
      rv.a = mix(prevColour.a,texel.a,castle_Color.a); \n\
    } else if (modea==MTMODE_BLENDTEXTUREALPHA) {  \n\
      rv.a = mix(prevColour.a,texel.a,texel.a); \n\
    } else if (modea==MTMODE_BLENDFACTORALPHA) {  \n\
      rv.a = mix(prevColour.a,texel.a,mt_Color.a); \n\
    } else if (modea==MTMODE_BLENDCURRENTALPHA) {  \n\
      rv.a = mix(prevColour.a,texel.a,prevColour.a); \n\
    } else if (modea==MTMODE_SELECTARG1) {  \n\
      rv.a = texel.a;  \n\
    } else if (modea==MTMODE_SELECTARG2) {  \n\
      rv.a = prevColour.a;  \n\
    } \n\
  } \n\
  if(func == MTFN_COMPLEMENT){ \n\
    //rv = vec4(1.0,1.0,1.0,1.0) - rv; \n\
    rv = vec4( vec3(1.0,1.0,1.0) - rv.rgb, rv.a); \n\
  }else if(func == MTFN_ALPHAREPLICATE){ \n\
    rv = vec4(rv.a,rv.a,rv.a,rv.a); \n\
  } \n\
  prevColour = rv;  \n\
} \n\
#endif //MTEX \n\
#endif //TEX \n\
#ifdef FILL \n\
uniform vec4 HatchColour; \n\
uniform bool hatched; uniform bool filled;\n\
uniform vec2 HatchScale; \n\
uniform vec2 HatchPct; \n\
uniform int algorithm; \n\
varying vec2 hatchPosition; \n\
void fillPropCalc(inout vec4 prevColour, vec2 MCposition, int algorithm) { \n\
  vec4 colour; \n\
  vec2 position, useBrick; \n\
  \n\
  position = MCposition / HatchScale; \n\
  \n\
  if (algorithm == 0) {/* bricking  */ \n\
    if (fract(position.y * 0.5) > 0.5) \n\
      position.x += 0.5; \n\
  } \n\
  \n\
  /* algorithm 1, 2 = no futzing required here  */ \n\
  if (algorithm == 3) { /* positive diagonals */ \n\
    vec2 curpos = position; \n\
    position.x -= curpos.y; \n\
  } \n\
  \n\
  if (algorithm == 4) {  /* negative diagonals */ \n\
    vec2 curpos = position; \n\
    position.x += curpos.y; \n\
  } \n\
  \n\
  if (algorithm == 6) {  /* diagonal crosshatch */ \n\
    vec2 curpos = position; \n\
    if (fract(position.y) > 0.5)  { \n\
      if (fract(position.x) < 0.5) position.x += curpos.y; \n\
      else position.x -= curpos.y; \n\
    } else { \n\
      if (fract(position.x) > 0.5) position.x += curpos.y; \n\
      else position.x -= curpos.y; \n\
    } \n\
  } \n\
  \n\
  position = fract(position); \n\
  \n\
  useBrick = step(position, HatchPct); \n\
  \n\
  if (filled) {colour = prevColour;} else { colour=vec4(0.,0.,0.,0); }\n\
  if (hatched) { \n\
      colour = mix(HatchColour, colour, useBrick.x * useBrick.y); \n\
  } \n\
  prevColour = colour; \n\
} \n\
#endif //FILL \n\
#ifdef FOG \n\
struct fogParams \n\
{  \n\
  vec4 fogColor; \n\
  float visibilityRange; \n\
  float fogScale; \n\
  int fogType; // 0 None, 1= FOGTYPE_LINEAR, 2 = FOGTYPE_EXPONENTIAL \n\
  // ifdefed int haveFogCoords; \n\
}; \n\
uniform fogParams fw_fogparams; \n\
#endif //FOG \n\
 \n\
/* PLUG-DECLARATIONS */ \n\
 \n\
#ifdef HAS_GEOMETRY_SHADER \n\
#define castle_vertex_eye castle_vertex_eye_geoshader \n\
#define castle_normal_eye castle_normal_eye_geoshader \n\
#endif \n\
 \n\
varying vec4 castle_vertex_eye; \n\
varying vec3 castle_normal_eye; \n\
#ifdef LIT \n\
#ifdef LITE \n\
//per-fragment lighting ie phong \n\
struct fw_MaterialParameters { \n\
  vec4 emission; \n\
  vec4 ambient; \n\
  vec4 diffuse; \n\
  vec4 specular; \n\
  float shininess; \n\
}; \n\
uniform fw_MaterialParameters fw_FrontMaterial; \n\
#ifdef TWO \n\
uniform fw_MaterialParameters fw_BackMaterial; \n\
#endif //TWO \n\
vec3 castle_ColorES; \n\
#else //LITE \n\
//per-vertex lighting - interpolated Emissive-specular \n\
varying vec3 castle_ColorES; //emissive shininess term \n\
#endif //LITE \n\
#endif //LIT\n\
 \n\
/* Wrapper for calling PLUG texture_coord_shift */ \n\
vec2 texture_coord_shifted(in vec2 tex_coord) \n\
{ \n\
  /* PLUG: texture_coord_shift (tex_coord) */ \n\
  return tex_coord; \n\
} \n\
 \n\
vec4 matdiff_color; \n\
void main(void) \n\
{ \n\
  vec4 fragment_color = vec4(1.0,1.0,1.0,1.0); \n\
  matdiff_color = castle_Color; \n\
  float castle_MaterialDiffuseAlpha = castle_Color.a; \n\
  \n\
  #ifdef LITE \n\
  //per-fragment lighting aka PHONG \n\
  //start over with the color, since we have material and lighting in here \n\
  castle_MaterialDiffuseAlpha = fw_FrontMaterial.diffuse.a; \n\
  matdiff_color = vec4(0,0,0,1.0); \n\
  castle_ColorES = fw_FrontMaterial.emission.rgb; \n\
  /* PLUG: add_light_contribution2 (matdiff_color, castle_ColorES, castle_vertex_eye, castle_normal_eye, fw_FrontMaterial.shininess) */ \n\
  #endif //LITE \n\
  \n\
  #ifdef LIT \n\
  #ifdef MATFIR \n\
  fragment_color.rgb = matdiff_color.rgb; \n\
  #endif //MATFIR \n\
  #endif //LIT \n\
  #ifdef UNLIT \n\
  fragment_color = castle_Color; \n\
  #endif //UNLIT \n\
  \n\
  #ifdef CPV \n\
  #ifdef CPVREP \n\
  fragment_color = cpv_Color; //CPV replaces mat.diffuse prior \n\
  fragment_color.a *= castle_MaterialDiffuseAlpha; \n\
  #else \n\
  fragment_color *= cpv_Color; //CPV modulates prior \n\
  #endif //CPVREP \n\
  #endif //CPV \n\
  \n\
  #ifdef TEX \n\
  #ifdef TEXREP \n\
  fragment_color = vec4(1.0,1.0,1.0,1.0); //texture replaces prior \n\
  #endif //TEXREP \n\
  #endif //TEX \n\
  \n\
  /* Fragment shader on mobile doesn't get a normal vector now, for speed. */ \n\
  //#define normal_eye_fragment castle_normal_eye //vec3(0.0) \n\
  #define normal_eye_fragment vec3(0.0) \n\
  \n\
  #ifdef FILL \n\
  fillPropCalc(matdiff_color, hatchPosition, algorithm); \n\
  #endif //FILL \n\
  \n\
  #ifdef LIT \n\
  #ifndef MATFIR \n\
  //modulate texture with mat.diffuse \n\
  fragment_color.rgb *= matdiff_color.rgb; \n\
  fragment_color.a *= castle_MaterialDiffuseAlpha; \n\
  #endif //MATFIR \n\
  fragment_color.rgb = clamp(fragment_color.rgb + castle_ColorES, 0.0, 1.0); \n\
  #endif //LIT \n\
  \n\
  /* PLUG: texture_apply (fragment_color, normal_eye_fragment) */ \n\
  /* PLUG: steep_parallax_shadow_apply (fragment_color) */ \n\
  /* PLUG: fog_apply (fragment_color, normal_eye_fragment) */ \n\
  \n\
  #undef normal_eye_fragment \n\
  \n\
  gl_FragColor = fragment_color; \n\
  \n\
  /* PLUG: fragment_end (gl_FragColor) */ \n\
} \n";

// http://www.web3d.org/documents/specifications/19775-1/V3.3/Part01/components/volume.html#OpacityMapVolumeStyle
// opacity with intensity == intensity lookup/transferFunction

static const GLchar *plug_voxel_DEFAULT =    "\
void voxel_apply_DEFAULT (inout vec4 voxel, inout vec3 gradient) { \n\
    float alpha = voxel.a; \n\
    //voxel.a = voxel.r; \n\
    //voxel.rgb = vec3(alpha); \n\
} \n\
void PLUG_voxel_apply_DEFAULT (inout vec4 voxel, inout vec3 gradient) { \n\
    voxel_apply_DEFAULT(voxel,gradient); \n\
} \n\
void PLUG_voxel_apply (inout vec4 voxel, inout vec3 gradient) { \n\
    voxel_apply_DEFAULT(voxel,gradient); \n\
} \n\
";

// http://www.web3d.org/documents/specifications/19775-1/V3.3/Part01/components/volume.html#OpacityMapVolumeStyle
static const GLchar *plug_voxel_OPACITY =    "\
uniform int fw_opacTexture; \n\
//uniform sampler2D fw_Texture_unit3; \n\
void voxel_apply_OPACITY (inout vec4 voxel, inout vec3 gradient) { \n\
    if(fw_opacTexture == 1){ \n\
        vec4 lookup_color; \n\
        float lum = voxel.r; \n\
        vec2 texcoord = vec2(lum,0.0); \n\
        //this is too simple for the lookups in the specs \n\
        //http://www.web3d.org/documents/specifications/19775-1/V3.3/Part01/components/volume.html#t-transferFunctionTextureCoordinateMapping \n\
        lookup_color = texture2D(fw_Texture_unit3,texcoord); \n\
        voxel.rgb = lookup_color.rgb; \n\
        voxel.a = lum; \n\
    }else{ \n\
        //like default \n\
        float alpha = voxel.a; \n\
        voxel.a = voxel.r; \n\
        voxel.rgb = vec3(alpha); \n\
    } \n\
} \n\
void PLUG_voxel_apply_OPACITY (inout vec4 voxel, inout vec3 gradient) { \n\
    voxel_apply_OPACITY(voxel, gradient); \n\
} \n\
void PLUG_voxel_apply (inout vec4 voxel, inout vec3 gradient) { \n\
    voxel_apply_OPACITY(voxel, gradient); \n\
} \n\
";

// http://www.web3d.org/documents/specifications/19775-1/V3.3/Part01/components/volume.html#BlendedVolumeStyle
static const GLchar *plug_voxel_BLENDED =    "\
void PLUG_voxel_apply (inout vec4 voxel, inout vec3 gradient) { \n\
} \n\
";

// http://www.web3d.org/documents/specifications/19775-1/V3.3/Part01/components/volume.html#BoundaryEnhancementVolumeStyle
static const GLchar *plug_voxel_BOUNDARY =    "\n\
uniform float fw_boundaryOpacity; \n\
uniform float fw_retainedOpacity; \n\
uniform float fw_opacityFactor; \n\
void voxel_apply_BOUNDARY (inout vec4 voxel, inout vec3 gradient) { \n\
    float magnitude = length(gradient); \n\
    float factor = (fw_retainedOpacity + fw_boundaryOpacity*pow(magnitude,fw_opacityFactor) ); \n\
    voxel.a = voxel.a * factor; \n\
    //debug_color = HeatMapColor(factor,0.0,1.0); \n\
} \n\
void PLUG_voxel_apply_BOUNDARY (inout vec4 voxel, inout vec3 gradient) { \n\
    voxel_apply_BOUNDARY(voxel, gradient); \n\
} \n\
void PLUG_voxel_apply (inout vec4 voxel, inout vec3 gradient) { \n\
    voxel_apply_BOUNDARY(voxel, gradient); \n\
} \n\
";

// http://www.web3d.org/documents/specifications/19775-1/V3.3/Part01/components/volume.html#CartoonVolumeStyle
static const GLchar *plug_voxel_CARTOON =    "\n\
uniform int fw_colorSteps; \n\
uniform vec4 fw_orthoColor; \n\
uniform vec4 fw_paraColor; \n\
void voxel_apply_CARTOON (inout vec4 voxel, inout vec3 gradient) { \n\
    float len = length(gradient); \n\
    if(len > 0.01) { \n\
        vec3 ng = normalize(gradient); \n\
        float ndotv = dot(normal_eye,ng); \n\
        if(ndotv > 0.01 && voxel.a > 0.0) { \n\
            ndotv = floor(ndotv/float(fw_colorSteps))*float(fw_colorSteps); \n\
            vec4 color = mix(fw_orthoColor,fw_paraColor,ndotv); \n\
            //voxel.rgb = color.rgb*voxel.a; \n\
            voxel.rgb = color.rgb; \n\
        } else { \n\
            voxel = vec4(0.0); //similar to discard \n\
        } \n\
    } else { \n\
        voxel = vec4(0.0); //similar to discard \n\
    } \n\
} \n\
void PLUG_voxel_apply_CARTOON (inout vec4 voxel, inout vec3 gradient) { \n\
    voxel_apply_CARTOON(voxel, gradient); \n\
} \n\
void PLUG_voxel_apply (inout vec4 voxel, inout vec3 gradient) { \n\
    voxel_apply_CARTOON(voxel, gradient); \n\
} \n\
";

// http://www.web3d.org/documents/specifications/19775-1/V3.3/Part01/components/volume.html#ComposedVolumeStyle
static const GLchar *plug_voxel_COMPOSED =    "\
void PLUG_voxel_apply (inout vec4 voxel, inout vec3 gradient) { \n\
} \n\
";

// http://www.web3d.org/documents/specifications/19775-1/V3.3/Part01/components/volume.html#EdgeEnhancementVolumeStyle
static const GLchar *plug_voxel_EDGE =    "\
uniform float fw_cosGradientThreshold; \n\
uniform vec4 fw_edgeColor; \n\
void voxel_apply_EDGE (inout vec4 voxel, inout vec3 gradient) { \n\
    float len = length(gradient); \n\
    if(len > 0.01) { \n\
        vec3 ng = normalize(gradient); \n\
        float ndotv = abs(dot(normal_eye,ng));  \n\
        if( ndotv < fw_cosGradientThreshold ) { \n\
            voxel = mix(voxel,fw_edgeColor,1.0 -ndotv); \n\
        } \n\
    } \n\
} \n\
void PLUG_voxel_apply_EDGE (inout vec4 voxel, inout vec3 gradient) { \n\
    voxel_apply_EDGE(voxel, gradient); \n\
} \n\
void PLUG_voxel_apply (inout vec4 voxel, inout vec3 gradient) { \n\
    voxel_apply_EDGE(voxel, gradient); \n\
} \n\
";

// http://www.web3d.org/documents/specifications/19775-1/V3.3/Part01/components/volume.html#ProjectionVolumeStyle
static const GLchar *plug_voxel_PROJECTION =    "\n\
uniform float fw_intensityThreshold; \n\
uniform int fw_projType; \n\
float MAXPROJ = 0.0; \n\
float MINPROJ = 1.0; \n\
float AVEPROJ = 0.0; \n\
float LMIP = 0.0; \n\
vec4 RGBAPROJ; \n\
int PROJCOUNT = 0; \n\
void voxel_apply_PROJECTION (inout vec4 voxel, inout vec3 gradient) { \n\
    PROJCOUNT++; \n\
    float cval = length(voxel.rgb); \n\
    if(fw_projType == 1){ \n\
        //MIN \n\
        if(cval < MINPROJ){ \n\
            MINPROJ = cval; \n\
            RGBAPROJ = voxel; \n\
        } \n\
    }else if(fw_projType == 2){ \n\
        //MAX \n\
        if(fw_intensityThreshold > 0.0){ \n\
            //LMIP \n\
            if(LMIP == 0.0) { \n\
                LMIP = cval; \n\
                RGBAPROJ = voxel; \n\
            } \n\
        } else { \n\
            //MIP \n\
            if(cval > MAXPROJ){ \n\
                MAXPROJ = cval; \n\
                RGBAPROJ = voxel; \n\
            } \n\
        } \n\
    }else if(fw_projType==3){ \n\
        //AVERAGE \n\
        AVEPROJ += cval; \n\
        RGBAPROJ += voxel; \n\
    } \n\
} \n\
void PLUG_voxel_apply_PROJECTION (inout vec4 voxel, inout vec3 gradient) { \n\
    voxel_apply_PROJECTION(voxel, gradient); \n\
} \n\
void PLUG_voxel_apply (inout vec4 voxel, inout vec3 gradient) { \n\
    voxel_apply_PROJECTION(voxel, gradient); \n\
} \n\
void PLUG_ray_apply (inout vec4 raysum) { \n\
    float value = 0.0; \n\
    vec4 color = vec4(1.0); \n\
    if(fw_projType == 1){ \n\
        //MIN \n\
        value = MINPROJ; \n\
        color = RGBAPROJ; \n\
    }else if(fw_projType == 2){ \n\
        //MAX \n\
        if(fw_intensityThreshold > 0.0){ \n\
            //LMIP \n\
            value = LMIP; \n\
            color = RGBAPROJ; \n\
        } else { \n\
            //MIP \n\
            value = MAXPROJ; \n\
            color = RGBAPROJ; \n\
        } \n\
    }else if(fw_projType==3){ \n\
        //AVERAGE \n\
        value = AVEPROJ / float(PROJCOUNT); \n\
        color = RGBAPROJ / float(PROJCOUNT); \n\
    } \n\
    //raysum.rgb = color.rgb * color.a; \n\
    //raysum.a = color.a; \n\
    \n\
    raysum.rgb = vec3(value,value,value); \n\
//    raysum.a = 1.0 - value; \n\
    //raysum.a = value;\n\
    //raysum.a = color.a; \n\
    //raysum = color; \n\
} \n\
";

// http://www.web3d.org/documents/specifications/19775-1/V3.3/Part01/components/volume.html#ShadedVolumeStyle
static const GLchar *plug_voxel_SHADED =    "\
#ifdef LITE \n\
#define MAX_LIGHTS 8 \n\
uniform int lightcount; \n\
//uniform float lightRadius[MAX_LIGHTS]; \n\
uniform int lightType[MAX_LIGHTS];//ANGLE like this \n\
struct fw_LightSourceParameters { \n\
  vec4 ambient;  \n\
  vec4 diffuse;   \n\
  vec4 specular; \n\
  vec4 position;   \n\
  vec4 halfVector;  \n\
  vec4 spotDirection; \n\
  float spotBeamWidth; \n\
  float spotCutoff; \n\
  vec3 Attenuations; \n\
  float lightRadius; \n\
}; \n\
\n\
uniform fw_LightSourceParameters fw_LightSource[MAX_LIGHTS] /* gl_MaxLights */ ;\n\
#endif //LITE \n\
 \n\
#ifdef FOG \n\
struct fogParams \n\
{  \n\
  vec4 fogColor; \n\
  float visibilityRange; \n\
  float fogScale; \n\
  int fogType; // 0 None, 1= FOGTYPE_LINEAR, 2 = FOGTYPE_EXPONENTIAL \n\
  // ifdefed int haveFogCoords; \n\
}; \n\
uniform fogParams fw_fogparams; \n\
#endif //FOG \n\
#ifdef LIT \n\
#ifdef LITE \n\
//per-fragment lighting ie phong \n\
struct fw_MaterialParameters { \n\
  vec4 emission; \n\
  vec4 ambient; \n\
  vec4 diffuse; \n\
  vec4 specular; \n\
  float shininess; \n\
}; \n\
uniform fw_MaterialParameters fw_FrontMaterial; \n\
#ifdef TWO \n\
uniform fw_MaterialParameters fw_BackMaterial; \n\
#endif //TWO \n\
vec3 castle_ColorES; \n\
#else //LITE \n\
//per-vertex lighting - interpolated Emissive-specular \n\
varying vec3 castle_ColorES; //emissive shininess term \n\
#endif //LITE \n\
#endif //LIT\n\
uniform int fw_phase; \n\
uniform int fw_lighting; \n\
uniform int fw_shadows; \n\
void voxel_apply_SHADED (inout vec4 voxel, inout vec3 gradient) { \n\
    float len = length(gradient); \n\
    vec3 ng = vec3(0.0); \n\
    if(len > 0.0) \n\
      ng = normalize(gradient); \n\
    vec4 color = vec4(1.0); \n\
    #ifdef LIT \n\
    vec3 castle_ColorES = fw_FrontMaterial.specular.rgb; \n\
    color.rgb = fw_FrontMaterial.diffuse.rgb; \n\
    #else //LIT \n\
    color.rgb = vec3(0,0,0.0,0.0); \n\
    vec3 castle_ColorES = vec3(0.0,0.0,0.0); \n\
    #endif //LIT    \n\
    // void add_light_contribution2(inout vec4 vertexcolor, inout vec3 specularcolor, in vec4 myPosition, in vec3 myNormal, in float shininess ); \n\
    vec4 vertex_eye4 = vec4(vertex_eye,1.0); \n\
    /* PLUG: add_light_contribution2 (color, castle_ColorES, vertex_eye4, ng, fw_FrontMaterial.shininess) */ \n\
    // voxel.rgb = color.rgb; \n\
    color.rgb = mix(color.rgb,castle_ColorES,dot(ng,normal_eye)); \n\
    voxel.rgb = color.rgb; \n\
    //voxel.rgb = voxel.rgb * color.rgb; \n\
} \n\
void PLUG_voxel_apply_SHADED (inout vec4 voxel, inout vec3 gradient) { \n\
    voxel_apply_SHADED(voxel, gradient); \n\
} \n\
void PLUG_voxel_apply (inout vec4 voxel, inout vec3 gradient) { \n\
    voxel_apply_SHADED(voxel, gradient); \n\
} \n\
";

// http://www.web3d.org/documents/specifications/19775-1/V3.3/Part01/components/volume.html#SilhouetteEnhancementVolumeStyle
static const GLchar *plug_voxel_SILHOUETTE =    "\n\
uniform float fw_BoundaryOpacity; \n\
uniform float fw_RetainedOpacity; \n\
uniform float fw_Sharpness; \n\
void voxel_apply_SILHOUETTE (inout vec4 voxel, inout vec3 gradient) { \n\
    float len = length(gradient); \n\
    if(len > 0.01) { \n\
        vec3 ng = normalize(gradient); \n\
        float ndotv = abs(dot(ng,normal_eye)); \n\
        float factor = (fw_RetainedOpacity + fw_BoundaryOpacity*pow(1.0 - ndotv,fw_Sharpness)); \n\
        //float factor = (fw_RetainedOpacity + pow(fw_BoundaryOpacity*(1.0 - ndotv),fw_Sharpness)); \n\
        //debug_color = HeatMapColor(factor,0.0,1.0); \n\
        voxel.a = voxel.a * factor; \n\
    } \n\
} \n\
void PLUG_voxel_apply_SILHOUETTE (inout vec4 voxel, inout vec3 gradient) { \n\
    voxel_apply_SILHOUETTE(voxel, gradient); \n\
} \n\
void PLUG_voxel_apply (inout vec4 voxel, inout vec3 gradient) { \n\
    voxel_apply_SILHOUETTE(voxel, gradient); \n\
} \n\
";

// http://www.web3d.org/documents/specifications/19775-1/V3.3/Part01/components/volume.html#ToneMappedVolumeStyle
static const GLchar *plug_voxel_TONE =    "\
uniform vec4 fw_coolColor; \n\
uniform vec4 fw_warmColor; \n\
void voxel_apply_TONE (inout vec4 voxel, inout vec3 gradient) { \n\
    float len = length(gradient); \n\
    if(len > 0.0) { \n\
        vec3 color; \n\
        vec3 ng = normalize(gradient); \n\
        //vec3 L = normalize(vec3(-.707,-.707,.707)); \n\
        float cc = (1.0 + dot(normal_eye,ng))*.5; \n\
        //float cc = (1.0 + dot(L,ng))*.5; \n\
        //debug_color = HeatMapColor(cc,0.0,1.0); \n\
        color = mix(fw_coolColor.rgb,fw_warmColor.rgb,cc); \n\
        voxel = vec4(color,voxel.a); \n\
    } else { \n\
        voxel.a = 0.0; \n\
        //debug_color = vec4(0.0); \n\
    } \n\
} \n\
void PLUG_voxel_apply_TONE (inout vec4 voxel, inout vec3 gradient) { \n\
    voxel_apply_TONE(voxel, gradient); \n\
} \n\
void PLUG_voxel_apply (inout vec4 voxel, inout vec3 gradient) { \n\
    voxel_apply_TONE(voxel, gradient); \n\
} \n\
";

// http://www.web3d.org/documents/specifications/19775-1/V3.3/Part01/components/volume.html#BlendedVolumeStyle
// blended (BlendedVolumeStyle) works (was interpreted and implemented by dug9, Oct 2016)
//  by rendering 2 volumedatas to 2 fbo textures, then running
// this shader to blend the 2 fbo textures and spit out fragments just
// where the box is. Use with the regular volume Vertex shader so
// only frags over the box show, and so we get box depth for depth blending
// with the main scene
static const GLchar *volumeBlendedFragmentGLES2 = " \n\
/* DEFINES */ \n\
#ifdef MOBILE \n\
//precision highp float; \n\
precision mediump float; \n\
#endif //MOBILE \n\
 vec4 HeatMapColor(float value, float minValue, float maxValue) \n\
{ \n\
    //used for debugging. If min=0,max=1 then magenta is 0, blue,green,yellow, red is 1 \n\
    vec4 ret; \n\
    int HEATMAP_COLORS_COUNT; \n\
    vec4 colors[6]; \n\
    HEATMAP_COLORS_COUNT = 6; \n\
    colors[0] = vec4(0.32, 0.00, 0.32, 1.0); \n\
    colors[1] = vec4( 0.00, 0.00, 1.00, 1.00); \n\
    colors[2] = vec4(0.00, 1.00, 0.00, 1.00); \n\
    colors[3] = vec4(1.00, 1.00, 0.00, 1.00); \n\
    colors[4] = vec4(1.00, 0.60, 0.00, 1.00); \n\
    colors[5] = vec4(1.00, 0.00, 0.00, 1.00); \n\
    float ratio=(float(HEATMAP_COLORS_COUNT)-1.0)*clamp((value-minValue)/(maxValue-minValue),0.0,1.0); \n\
    int indexMin=int(floor(ratio)); \n\
    int indexMax= indexMin+1 < HEATMAP_COLORS_COUNT-1 ? indexMin+1 : HEATMAP_COLORS_COUNT-1; \n\
    ret = mix(colors[indexMin], colors[indexMax], ratio-float(indexMin)); \n\
    if(value < minValue) ret = vec4(0.0,0.0,0.0,1.0); \n\
    if(value > maxValue) ret = vec4(1.0,1.0,1.0,1.0); \n\
    return ret; \n\
} \n\
vec4 debug_color; \n\
 \n\
uniform vec4 fw_viewport; \n\
uniform sampler2D fw_Texture_unit0; \n\
uniform sampler2D fw_Texture_unit1; \n\
uniform sampler2D fw_Texture_unit2; \n\
uniform sampler2D fw_Texture_unit3; \n\
uniform float fw_iwtc1; \n\
uniform float fw_iwtc2; \n\
uniform int fw_iwtf1; \n\
uniform int fw_iwtf2; \n\
uniform int fw_haveTransfers; \n\
vec3 weightcolor( in vec3 color, in int func, in float wt, in float ov,  in float oblend, in sampler2D table){ \n\
    vec3 ret; \n\
    if(func == 1){ \n\
        ret = color * wt; \n\
    }else if(func == 2){ \n\
        ret = color * ov; \n\
    }else if(func == 3){ \n\
        ret = color * oblend; \n\
    }else if(func == 4){ \n\
        ret = color * (1.0 - oblend); \n\
    }else if(func == 5){ \n\
        vec2 texcoord = color.rg;\n\
        ret = color * texture2D(table,texcoord).r; \n\
    } \n\
    return ret; \n\
} \n\
float weightalpha( in float alpha, in int func, in float wt, in float ov, in float oblend, in sampler2D table){ \n\
    float ret; \n\
    if(func == 1){ \n\
        ret = alpha * wt; \n\
    }else if(func == 2){ \n\
        ret = alpha * ov; \n\
    }else if(func == 3){ \n\
        ret = alpha * oblend; \n\
    }else if(func == 4){ \n\
        ret = alpha * (1.0 - oblend); \n\
    }else if(func == 5){ \n\
        vec2 texcoord = vec2(alpha,0);\n\
        ret = alpha * texture2D(table,texcoord).r; \n\
    } \n\
    return ret; \n\
} \n\
void main(void) \n\
{ \n\
    vec2 fc = (gl_FragCoord.xy - fw_viewport.xy) / fw_viewport.zw; \n\
    vec4 frag0 = texture2D(fw_Texture_unit0,fc); \n\
    vec4 frag1 = texture2D(fw_Texture_unit1,fc); \n\
    vec3 cv = frag0.rgb; \n\
    float ov = frag0.a; \n\
    vec3 cblend = frag1.rgb; \n\
    float oblend = frag1.a; \n\
    vec3 cvw, cbw; \n\
    float ovw, obw; \n\
    cvw = weightcolor(cv,fw_iwtf1,fw_iwtc1,ov,oblend,fw_Texture_unit2); \n\
    ovw = weightalpha(ov,fw_iwtf1,fw_iwtc1,ov,oblend,fw_Texture_unit2); \n\
    cbw = weightcolor(cblend,fw_iwtf2,fw_iwtc2,ov,oblend,fw_Texture_unit3); \n\
    obw = weightalpha(oblend,fw_iwtf2,fw_iwtc2,ov,oblend,fw_Texture_unit3); \n\
    vec3 cg = clamp( cvw + cbw, 0.0, 1.0); \n\
    float og = clamp(ovw + obw, 0.0, 1.0); \n\
    \n\
    gl_FragColor = vec4(cg,og); \n\
} \n\
";

static int getSpecificShaderSourceOriginal (const GLchar *vertexSource[vertexEndMarker], 
    const GLchar *fragmentSource[fragmentEndMarker], shaderflagsstruct whichOne) { //unsigned int whichOne) {

    bool doThis;
    bool didADSLmaterial;
#ifdef USING_SHADER_LIGHT_ARRAY_METHOD
    //for angleproject winRT d3d11 - can't do struct[] array for lights
    const GLchar *lightDefines0 = lightDefinesArrayMethod;
    const GLchar *ADSLLightModel0 = ADSLLightModelArrayMethod;
#else
    const GLchar *lightDefines0 = lightDefines;
    const GLchar *ADSLLightModel0 = ADSLLightModel;
#endif
    
    /* GL_ES - do we have medium precision, or just low precision?? */
    /* Phong shading - use the highest we have */
    /* GL_ES_VERSION_2_0 has these definitions */

#if defined (GL_ES_VERSION_2_0)
    bool haveHighPrecisionFragmentShaders = false;

#ifdef VARY_VERTEX_PRECISION
    bool haveHighPrecisionVertexShaders = false;
#endif

    GLint range[2]; GLint precision;

    // see where we are doing the lighting. Use highest precision there, if we can.
    if (DESIRE(whichOne.base,SHADINGSTYLE_PHONG)) {
        glGetShaderPrecisionFormat(GL_FRAGMENT_SHADER,GL_HIGH_FLOAT, range, &precision);
        if (precision!=0) {
            haveHighPrecisionFragmentShaders=true;
        } else {
            haveHighPrecisionFragmentShaders=false;
            glGetShaderPrecisionFormat(GL_FRAGMENT_SHADER,GL_MEDIUM_FLOAT, range, &precision);
            if (precision == 0) {
                ConsoleMessage("low precision Fragment shaders only available - view may not work so well");
            }
        }
#ifdef VARY_VERTEX_PRECISION
    // if we do lighting on the Vertex shader side, do we have to worry about precision?
    } else {
        glGetShaderPrecisionFormat(GL_VERTEX_SHADER,GL_HIGH_FLOAT, range, &precision);
        if (precision!=0) {
            haveHighPrecisionVertexShaders=true;
        } else {
            haveHighPrecisionVertexShaders=false;
            glGetShaderPrecisionFormat(GL_VERTEX_SHADER,GL_MEDIUM_FLOAT, range, &precision);
            if (precision == 0) {
                ConsoleMessage("low precision Vertex shaders only available - view may not work so well");
            }
        }
#endif //VARY_VERTEX_PRECISION

    }
#else
    // ConsoleMessage ("seem to not have GL_MEDIUM_FLOAT or GL_HIGH_FLOAT");
#endif // GL_ES_VERSION_2_0 for GL_HIGH_FLOAT or GL_MEDIUM_FLOAT

    #if defined (VERBOSE) && defined (GL_ES_VERSION_2_0)
    { /* debugging - only */
    GLboolean b;

    glGetBooleanv(GL_SHADER_COMPILER,&b);
    if (b) ConsoleMessage("have shader compiler"); else ConsoleMessage("NO SHADER COMPILER");


    glGetShaderPrecisionFormat(GL_VERTEX_SHADER,GL_LOW_FLOAT, range, &precision);
    ConsoleMessage ("GL_VERTEX_SHADER, GL_LOW_FLOAT range [%d,%d],precision %d",range[0],range[1],precision);
    glGetShaderPrecisionFormat(GL_VERTEX_SHADER,GL_MEDIUM_FLOAT, range, &precision);
    ConsoleMessage ("GL_VERTEX_SHADER, GL_MEDIUM_FLOAT range [%d,%d],precision %d",range[0],range[1],precision);
    glGetShaderPrecisionFormat(GL_VERTEX_SHADER,GL_HIGH_FLOAT, range, &precision);
    ConsoleMessage ("GL_VERTEX_SHADER, GL_HIGH_FLOAT range [%d,%d],precision %d",range[0],range[1],precision);

    glGetShaderPrecisionFormat(GL_VERTEX_SHADER,GL_LOW_INT, range, &precision);
    ConsoleMessage ("GL_VERTEX_SHADER, GL_LOW_INT range [%d,%d],precision %d",range[0],range[1],precision);
    glGetShaderPrecisionFormat(GL_VERTEX_SHADER,GL_MEDIUM_INT, range, &precision);
    ConsoleMessage ("GL_VERTEX_SHADER, GL_MEDIUM_INT range [%d,%d],precision %d",range[0],range[1],precision);
    glGetShaderPrecisionFormat(GL_VERTEX_SHADER,GL_HIGH_INT, range, &precision);
    ConsoleMessage ("GL_VERTEX_SHADER, GL_HIGH_INT range [%d,%d],precision %d",range[0],range[1],precision);

    glGetShaderPrecisionFormat(GL_FRAGMENT_SHADER,GL_LOW_FLOAT, range, &precision);
    ConsoleMessage ("GL_FRAGMENT_SHADER, GL_LOW_FLOAT range [%d,%d],precision %d",range[0],range[1],precision);
    glGetShaderPrecisionFormat(GL_FRAGMENT_SHADER,GL_MEDIUM_FLOAT, range, &precision);
    ConsoleMessage ("GL_FRAGMENT_SHADER, GL_MEDIUM_FLOAT range [%d,%d],precision %d",range[0],range[1],precision);
    glGetShaderPrecisionFormat(GL_FRAGMENT_SHADER,GL_HIGH_FLOAT, range, &precision);
    ConsoleMessage ("GL_FRAGMENT_SHADER, GL_HIGH_FLOAT range [%d,%d],precision %d",range[0],range[1],precision);

    glGetShaderPrecisionFormat(GL_FRAGMENT_SHADER,GL_LOW_INT, range, &precision);
    ConsoleMessage ("GL_FRAGMENT_SHADER, GL_LOW_INT range [%d,%d],precision %d",range[0],range[1],precision);
    glGetShaderPrecisionFormat(GL_FRAGMENT_SHADER,GL_MEDIUM_INT, range, &precision);
    ConsoleMessage ("GL_FRAGMENT_SHADER, GL_MEDIUM_INT range [%d,%d],precision %d",range[0],range[1],precision);
    glGetShaderPrecisionFormat(GL_FRAGMENT_SHADER,GL_HIGH_INT, range, &precision);
    ConsoleMessage ("GL_FRAGMENT_SHADER, GL_HIGH_INT range [%d,%d],precision %d",range[0],range[1],precision);
    }
    #endif //VERBOSE for GL_ES_VERSION_2_0

    #ifdef VERBOSE
    if DESIRE(whichOne.base,NO_APPEARANCE_SHADER) ConsoleMessage ("want NO_APPEARANCE_SHADER");
    if DESIRE(whichOne.base,MATERIAL_APPEARANCE_SHADER) ConsoleMessage ("want MATERIAL_APPEARANCE_SHADER");
    if DESIRE(whichOne.base,TWO_MATERIAL_APPEARANCE_SHADER) ConsoleMessage ("want TWO_MATERIAL_APPEARANCE_SHADER");
    if DESIRE(whichOne.base,ONE_TEX_APPEARANCE_SHADER)ConsoleMessage("want ONE_TEX_APPEARANCE_SHADER");
    if DESIRE(whichOne.base,MULTI_TEX_APPEARANCE_SHADER)ConsoleMessage("want MULTI_TEX_APPEARANCE_SHADER");
    if DESIRE(whichOne.base,COLOUR_MATERIAL_SHADER)ConsoleMessage("want COLOUR_MATERIAL_SHADER");
    if DESIRE(whichOne.base,FILL_PROPERTIES_SHADER)ConsoleMessage("want FILL_PROPERTIES_SHADER");
    if DESIRE(whichOne.base,HAVE_LINEPOINTS_COLOR)ConsoleMessage ("want LINE_POINTS_COLOR");
    if DESIRE(whichOne.base,HAVE_LINEPOINTS_APPEARANCE)ConsoleMessage ("want LINE_POINTS_APPEARANCE");
    if DESIRE(whichOne.base,HAVE_TEXTURECOORDINATEGENERATOR) ConsoleMessage ("want HAVE_TEXTURECOORDINATEGENERATOR");
    if DESIRE(whichOne.base,HAVE_CUBEMAP_TEXTURE) ConsoleMessage ("want HAVE_CUBEMAP_TEXTURE");
    #endif //VERBOSE
#undef VERBOSE


    /* Cross shader Fragment bits - GL_ES_VERSION_2_0 has this */
#if defined(GL_ES_VERSION_2_0)
    fragmentSource[fragmentGLSLVersion] = "#version 100\n";
    vertexSource[vertexGLSLVersion] = "#version 100\n";
    if (haveHighPrecisionFragmentShaders)  {
        fragmentSource[fragmentPrecisionDeclare] = fragHighPrecision;
        //ConsoleMessage("have high precision fragment shaders");
    } else {
        fragmentSource[fragmentPrecisionDeclare] = fragMediumPrecision;
        //ConsoleMessage("have medium precision fragment shaders");
    }

#ifdef VARY_VERTEX_PRECISION
    // if we do lighting on the Vertex shader side, do we have to worry about precision?
    if (haveHighPrecisionVertexShaders)  {
        vertexSource[vertexPrecisionDeclare] = fragHighPrecision;
        ConsoleMessage("have high precision vertex shaders");
    } else {
        vertexSource[vertexPrecisionDeclare] = fragMediumPrecision;
        ConsoleMessage("have medium precision vertex shaders");
    }
#endif //VARY_VERTEX_PRECISION

#else
    //changed from 120 to 110 Apr2014: main shaders still seem to work the same, openGL 2.0 now compiles them, and 2.1 (by specs) compiles 110
    fragmentSource[fragmentGLSLVersion] = "#version 110\n";//"#version 120\n";
    vertexSource[vertexGLSLVersion] = "#version 110\n"; //"#version 120\n";
#endif

    fragmentSource[fragMaxLightsDeclare] = maxLights;
    vertexSource[vertMaxLightsDeclare] = maxLights;
    vertexSource[vertexPositionDeclare] = vertPosDec;



    /* User defined shaders - only give the defines, let the user do the rest */

    if (!whichOne.usershaders) { // & USER_DEFINED_SHADER_MASK) == 0) {
        /* initialize */

        /* Generic things first */

        /* Cross shader Vertex bits */

        vertexSource[vertexMainStart] = vertMainStart;
        vertexSource[vertexPositionCalculation] = vertPos;
        vertexSource[vertexMainEnd] = vertEnd;


        fragmentSource[fragmentMainStart] = fragMainStart;
/*
        if(Viewer()->anaglyph || Viewer()->anaglyphB)
            fragmentSource[fragmentMainEnd] = anaglyphGrayFragEnd;
        else 
*/ 
                {
            if (DESIRE(whichOne.base,SHADINGSTYLE_PHONG)) fragmentSource[fragmentMainEnd] = discardInFragEnd;
            else fragmentSource[fragmentMainEnd] = fragEnd;
            //fragmentSource[fragmentMainEnd] = discardInFragEnd;
        }

        //ConsoleMessage ("whichOne %x mask %x",whichOne,~whichOne);


        /* specific strings for specific shader capabilities */

        if DESIRE(whichOne.base,COLOUR_MATERIAL_SHADER) {
            vertexSource[vertexSimpleColourDeclare] = vertSimColDec;
            vertexSource[vertFrontColourDeclare] = varyingFrontColour;
            vertexSource[vertexSimpleColourCalculation] = vertSimColUse;
            vertexSource[vertexPointSizeDeclare] = pointSizeDeclare;
            vertexSource[vertexPointSizeAssign] = pointSizeAss;
            fragmentSource[fragmentSimpleColourDeclare] = varyingFrontColour;
            fragmentSource[fragmentSimpleColourAssign] = fragSimColAss;
        }

        if DESIRE(whichOne.base,NO_APPEARANCE_SHADER) {
            fragmentSource[fragmentSimpleColourAssign] = fragNoAppAss;
            vertexSource[vertexPointSizeDeclare] = pointSizeDeclare;
            vertexSource[vertexPointSizeAssign] = pointSizeAss;

        }


        /* One or TWO material no texture shaders - one material, choose between
            Phong shading (slower) or Gouraud shading (faster). */

        if (DESIRE(whichOne.base,SHADINGSTYLE_PHONG)) {
            doThis = (DESIRE(whichOne.base,MATERIAL_APPEARANCE_SHADER)) ||
                (DESIRE(whichOne.base,TWO_MATERIAL_APPEARANCE_SHADER));
        } else {
            doThis = DESIRE(whichOne.base,TWO_MATERIAL_APPEARANCE_SHADER);
        }

        if (doThis) {
            vertexSource[vertexNormPosOutput] = varyingNormPos;
            vertexSource[vertexNormalDeclare] = vertNormDec;
            vertexSource[vertexNormPosCalculation] = vertNormPosCalc;

            fragmentSource[fragmentLightDefines] = lightDefines0;
            fragmentSource[fragmentOneColourDeclare] = vertOneMatDec;
            fragmentSource[fragmentBackColourDeclare] = vertBackMatDec;
            fragmentSource[fragmentNormPosDeclare] = varyingNormPos;
            fragmentSource[fragmentADSLLightModel] = ADSLLightModel0;
            fragmentSource[fragmentADSLAssign] = fragADSLAss;

        }


        /* TWO_MATERIAL_APPEARANCE_SHADER - this does not crop up
                that often, so just use the PHONG shader. */
        didADSLmaterial = false;
        if((DESIRE(whichOne.base,MATERIAL_APPEARANCE_SHADER)) && (!DESIRE(whichOne.base,SHADINGSTYLE_PHONG))) {
            vertexSource[vertexNormalDeclare] = vertNormDec;
            vertexSource[vertexLightDefines] = lightDefines0;
            vertexSource[vertexOneMaterialDeclare] = vertOneMatDec;
            vertexSource[vertFrontColourDeclare] = varyingFrontColour;
            vertexSource[vertexNormPosCalculation] = vertNormPosCalc;
            vertexSource[vertexNormPosOutput] = vecNormPos;
            vertexSource[vertexLightingEquation] = ADSLLightModel0;
            vertexSource[vertexBackMaterialDeclare] = vertBackMatDec;
            vertexSource[vertexADSLCalculation] = vertADSLCalc;
            didADSLmaterial = true;
            fragmentSource[fragmentOneColourDeclare] = varyingFrontColour;
            fragmentSource[fragmentOneColourAssign] = fragFrontColAss;
        }


        if DESIRE(whichOne.base,HAVE_LINEPOINTS_APPEARANCE) {
            vertexSource[vertexLightDefines] = lightDefines0;
            vertexSource[vertFrontColourDeclare] = varyingFrontColour;
            vertexSource[vertexOneMaterialDeclare] = vertOneMatDec;

            #if defined (GL_ES_VERSION_2_0)
        vertexSource[vertexPointSizeDeclare] = pointSizeDeclare;
        vertexSource[vertexPointSizeAssign] = pointSizeAss;
        #endif

            vertexSource[vertexOneMaterialCalculation] = vertEmissionOnlyColourAss;
            fragmentSource[fragmentSimpleColourDeclare] = varyingFrontColour;
            fragmentSource[fragmentSimpleColourAssign] = fragSimColAss;
        }


        /* texturing - MULTI_TEX builds on ONE_TEX */
        if (DESIRE(whichOne.base,ONE_TEX_APPEARANCE_SHADER) ||
            DESIRE(whichOne.base,HAVE_TEXTURECOORDINATEGENERATOR) ||
            DESIRE(whichOne.base,HAVE_CUBEMAP_TEXTURE) ||
            DESIRE(whichOne.base,MULTI_TEX_APPEARANCE_SHADER)) {
            vertexSource[vertexTexCoordInputDeclare] = vertTexCoordDec;
            vertexSource[vertexTexCoordOutputDeclare] = varyingTexCoord;
            vertexSource[vertexTextureMatrixDeclare] = vertTexMatrixDec;
            vertexSource[vertexSingleTextureCalculation] = vertSingTexCalc;
            if(didADSLmaterial)
                vertexSource[vertexADSLCalculation] = vertADSLCalc0; //over-ride material diffuseColor with texture

            fragmentSource[fragmentTexCoordDeclare] = varyingTexCoord;
            fragmentSource[fragmentTex0Declare] = fragTex0Dec;
            fragmentSource[fragmentTextureAssign] = fragSingTexAss;
        }

        /* Cubemaps - do not multi-texture these yet */
        if (DESIRE(whichOne.base,HAVE_CUBEMAP_TEXTURE)) {
            vertexSource[vertexSingleTextureCalculation] = vertSingTexCubeCalc;

            fragmentSource[fragmentTex0Declare] = fragTex0CubeDec;
            fragmentSource[fragmentTextureAssign] = fragSingTexCubeAss;
        }

        /* MULTI_TEX builds on ONE_TEX */
        if DESIRE(whichOne.base,MULTI_TEX_APPEARANCE_SHADER) {
            /* we have to do the material params, in case we need to
                modulate/play with this. */

            vertexSource[vertexOneMaterialDeclare] = vertOneMatDec;
            vertexSource[vertexLightDefines] = lightDefines0;
            vertexSource[vertexNormPosCalculation] = vertNormPosCalc;
            vertexSource[vertexNormPosOutput] = vecNormPos;
            vertexSource[vertexLightingEquation] = ADSLLightModel0;
            vertexSource[vertexBackMaterialDeclare] = vertBackMatDec;

            fragmentSource[fragmentMultiTexDefines]= fragMultiTexUniforms;
            fragmentSource[fragmentMultiTexDeclare] = fragMultiTexDef;
            fragmentSource[fragmentTex0Declare] = fragTex0Dec;
            fragmentSource[fragmentMultiTexModel] = fragMulTexFunc;
            fragmentSource[fragmentTextureAssign] = fragMulTexCalc;
        }

        /* TextureCoordinateGenerator - do calcs in Vertex, fragment like one texture */
        if DESIRE(whichOne.base,HAVE_TEXTURECOORDINATEGENERATOR) {
            /* the vertex single texture calculation is different from normal single texture */
            /* pass in the type of generator, and do the calculations */
            vertexSource[vertexTextureMatrixDeclare] = vertTexCoordGenDec;
            vertexSource[vertexSingleTextureCalculation] = sphEnvMapCalc;

            vertexSource[vertexTCGTDefines] = fragTCGTDefs;

        }

            if DESIRE(whichOne.base,FILL_PROPERTIES_SHADER) {
                /* just add on top of the other shaders the fill properties "stuff" */

                vertexSource[vertexHatchPositionDeclare] = varyingHatchPosition;
                vertexSource[vertexHatchPositionCalculation] = vertHatchPosCalc;

                fragmentSource[fragmentFillPropDefines] = fillPropDefines;
                fragmentSource[fragmentHatchPositionDeclare] = varyingHatchPosition;
                fragmentSource[fragmentFillPropModel] = fragFillPropFunc;
                fragmentSource[fragmentFillPropAssign] = fragFillPropCalc;
            }

    } else {  // user defined shaders

        if (whichOne.usershaders) { // >= USER_DEFINED_SHADER_START) {
            int me = 0;
            ppOpenGL_Utils p;
            //ttglobal tg = gglobal();
            p = (ppOpenGL_Utils)openGL_Utils_prv;

            //me = (whichOne / USER_DEFINED_SHADER_START) -1;
            me = whichOne.usershaders;
            //ConsoleMessage ("HAVE USER DEFINED SHADER %x",whichOne);

            // add the following:
            // this has both Vertex manipulations, and lighting, etc.
    //        #define HEADLIGHT_LIGHT (MAX_LIGHTS-1)\n
            vertexSource[vertexMainStart] = "  \n \
            #define HEADLIGHT_LIGHT 0\n \
            #define ftransform() (fw_ProjectionMatrix*fw_ModelViewMatrix*fw_Vertex)\n \
            #define gl_ModelViewProjectionMatrix (fw_ProjectionMatrix*fw_ModelViewMatrix)\n \
            #define gl_NormalMatrix fw_NormalMatrix\n \
            #define gl_ProjectionMatrix fw_ProjectionMatrix \n\
            #define gl_ModelViewMatrix fw_ModelViewMatrix \n\
            #define fw_TextureMatrix fw_TextureMatrix0 \n\
            #define gl_TextureMatrix fw_TextureMatrix0 \n\
            #define gl_Vertex fw_Vertex \n \
            #define gl_Normal fw_Normal\n \
            #define gl_Texture_unit0 fw_Texture_unit0\n \
            #define gl_MultiTexCoord0 fw_MultiTexCoord0\n \
            #define gl_Texture_unit1 fw_Texture_unit1\n \
            #define gl_MultiTexCoord1 fw_MultiTexCoord1\n \
            #define gl_Texture_unit2 fw_Texture_unit2\n \
            #define gl_MultiTexCoord2 fw_MultiTexCoord2\n \
            #define gl_LightSource fw_LightSource\n ";

        // copy over the same defines, but for the fragment shader.
        // Some GLSL compilers will complain about the "fttransform()"
        // definition if defined in a Fragment shader, so we judiciously
        // copy over things that are fragment-only.

        //    #define HEADLIGHT_LIGHT (MAX_LIGHTS-1)\n
            fragmentSource[fragmentMainStart] = " \
            #define HEADLIGHT_LIGHT 0\n \
            #define gl_NormalMatrix fw_NormalMatrix\n \
            #define gl_Normal fw_Normal\n \
            #define gl_LightSource fw_LightSource\n ";




            vertexSource[vertexLightDefines] = lightDefines0;
            vertexSource[vertexSimpleColourDeclare] = vertSimColDec;
            vertexSource[vertFrontColourDeclare] = varyingFrontColour;



            vertexSource[vertexNormalDeclare] = vertNormDec;
            fragmentSource[fragmentLightDefines] = lightDefines0;
            //ConsoleMessage ("sources here for %d are %p and %p", me, p->userDefinedVertexShader[me], p->userDefinedFragmentShader[me]);

            if ((p->userDefinedVertexShader[me] == NULL) || (p->userDefinedFragmentShader[me]==NULL)) {
                ConsoleMessage ("no Shader Source found for user defined shaders...");
                return false;

            }
            fragmentSource[fragmentUserDefinedInput] = p->userDefinedFragmentShader[me];
            vertexSource[vertexUserDefinedInput] = p->userDefinedVertexShader[me];

        }
    }

//#define VERBOSE 1
    #ifdef VERBOSE
    /* print out the vertex source here */
        {
            vertexShaderResources_t x1;
            fragmentShaderResources_t x2;
            int i;

            ConsoleMessage ("Vertex source:\n");
            for (x1=vertexGLSLVersion; x1<vertexEndMarker; x1++) {
                if (strlen(vertexSource[x1])>0)
                    ConsoleMessage("%s",vertexSource[x1]);
            }
            ConsoleMessage("Fragment Source:\n");
            i=0;
            for (x2=fragmentGLSLVersion; x2<fragmentEndMarker; x2++) {
                if (strlen(fragmentSource[x2])>0)
                    ConsoleMessage("%s",fragmentSource[x2]);
            }
        }
    #endif //VERBOSE
//#undef VERBOSE
    return TRUE;
}
#undef VERBOSE

const char *getGenericFragment(){
        return genericFragmentGLES2; //genericFragmentDesktop;
}

void AddVersion( int EffectPartType, int versionNumber, char **CompleteCode){
    //puts #version <number> at top of shader, first line
    char Code[SBUFSIZE], line[1000];
    char *found;
    int err;

    UNUSED(err);

    if (!CompleteCode[EffectPartType]) return;
    err = fw_strcpy_s(Code, SBUFSIZE, CompleteCode[EffectPartType]);

    found = Code;
    if (found) {
        sprintf(line, "#version %d \n", versionNumber);
        insertBefore(found, line, Code, SBUFSIZE);
        FREE_IF_NZ(CompleteCode[EffectPartType]);
        CompleteCode[EffectPartType] = strdup(Code);
    }
}

void AddDefine0( int EffectPartType, const char *defineName, int defineValue, char **CompleteCode)
{
    //same as AddDEfine but you can say a number other than 1
    char Code[SBUFSIZE], line[1000];
    char *found;
    int err;

    UNUSED(err);

    if(!CompleteCode[EffectPartType]) return;
    err = fw_strcpy_s(Code,SBUFSIZE, CompleteCode[EffectPartType]);

    found = strstr(Code,"/* DEFINE"); 
    if(found){
        sprintf(line,"#define %s %d \n",defineName,defineValue);
        insertBefore(found,line,Code,SBUFSIZE);
        FREE_IF_NZ(CompleteCode[EffectPartType]);
        CompleteCode[EffectPartType] = strdup(Code);
    }
} 

void AddDefine( int EffectPartType, const char *defineName, char **CompleteCode){
    //adds #define <defineName> 1 to shader part, just above "/* DEFINES */" line in ShaderPart
    // char *CompleteCode[3] has char *vs *gs *fs parts, and will be realloced inside
    AddDefine0(EffectPartType,defineName,1,CompleteCode);
}

Stack *getEffectStack();
void EnableEffects( char **CompletedCode, int *unique_int){
    int i;
    Stack *effect_stack;
    effect_stack = getEffectStack();
    for(i=0;i<vectorSize(effect_stack);i++){
        Node *node = vector_get(Node*,effect_stack,i);
        if(node->getType() == KAMBI_EFFECT){
            EnableEffect(node,CompletedCode,unique_int);
        }
    }
}

static const GLchar *plug_fragment_end_anaglyph =    "\
void PLUG_fragment_end (inout vec4 finalFrag){ \n\
    float gray = dot(finalFrag.rgb, vec3(0.299, 0.587, 0.114)); \n\
    finalFrag = vec4(gray,gray,gray, finalFrag.a); \n\
}\n";

int getSpecificShaderSourceCastlePlugs (const GLchar **vertexSource, const GLchar **fragmentSource, shaderflagsstruct whichOne) 
{
    //for building the Builtin (similar to fixed-function pipeline, except from shader parts)
    //in OpenGL_Utils.c L.2553 set usingCastlePlugs = 1 to get in here.
    //whichone - a bitmask of shader requirements, one bit for each requirement, so shader permutation can be built

    int retval, unique_int;
    char *CompleteCode[3];
    char *vs, *fs;
    retval = FALSE;
    if(whichOne.usershaders ) //& USER_DEFINED_SHADER_MASK) 
        return retval; //not supported yet as of Aug 9, 2016
    retval = TRUE;

    //generic
    vs = strdup(getGenericVertex());
    fs = strdup(getGenericFragment());
        
    CompleteCode[SHADERPART_VERTEX] = vs;
    CompleteCode[SHADERPART_GEOMETRY] = NULL;
    CompleteCode[SHADERPART_FRAGMENT] = fs;

    // what we really have here: UberShader with CastlePlugs
    // UberShader: one giant shader peppered with #ifdefs, and you add #defines at the top for permutations
    // CastlePlugs: allows users to add effects on to uberShader with PLUGs
    // - and internally, we can do a few permutations with PLUGs too

    if(isMobile){
        AddVersion(SHADERPART_VERTEX, 100, CompleteCode); //lower precision floats
        AddVersion(SHADERPART_FRAGMENT, 100, CompleteCode); //lower precision floats
        AddDefine(SHADERPART_FRAGMENT,"MOBILE",CompleteCode); //lower precision floats
    }else{
        //desktop, emulating GLES2
        AddVersion(SHADERPART_VERTEX, 110, CompleteCode); //lower precision floats
        AddVersion(SHADERPART_FRAGMENT, 110, CompleteCode); //lower precision floats
    }

    unique_int = 0; //helps generate method name PLUG_xxx_<unique_int> to avoid clash when multiple PLUGs supplied for same PLUG point
    //Add in:
    //Lit
    //Fog
    //analglyph
    if(DESIRE(whichOne.base,WANT_ANAGLYPH))
        Plug(SHADERPART_FRAGMENT,plug_fragment_end_anaglyph,CompleteCode,&unique_int);  //works, converts frag to gray
    //color per vertex
    if DESIRE(whichOne.base,COLOUR_MATERIAL_SHADER) {
        AddDefine(SHADERPART_VERTEX,"CPV",CompleteCode);
        AddDefine(SHADERPART_FRAGMENT,"CPV",CompleteCode);
        if(DESIRE(whichOne.base,CPV_REPLACE_PRIOR)){
            AddDefine(SHADERPART_VERTEX,"CPVREP",CompleteCode);
            AddDefine(SHADERPART_FRAGMENT,"CPVREP",CompleteCode);
        }
    }
    //material appearance
    //2 material appearance
    //phong vs gourard
    if(DESIRE(whichOne.base,MATERIAL_APPEARANCE_SHADER) || DESIRE(whichOne.base,TWO_MATERIAL_APPEARANCE_SHADER)){
        //if(isLit)
        if(DESIRE(whichOne.base,MAT_FIRST)){
            //strict table 17-3 with no other modulation means Texture > CPV > mat.diffuse > (111)
            AddDefine(SHADERPART_VERTEX,"MATFIR",CompleteCode);
            AddDefine(SHADERPART_FRAGMENT,"MATFIR",CompleteCode);
        }
        if(DESIRE(whichOne.base,SHADINGSTYLE_PHONG) && !DESIRE(whichOne.base,HAVE_LINEPOINTS_COLOR)){
            //when we say phong in freewrl, we really mean per-fragment lighting
            AddDefine(SHADERPART_FRAGMENT,"LIT",CompleteCode);
            AddDefine(SHADERPART_FRAGMENT,"LITE",CompleteCode);  //add some lights
            Plug(SHADERPART_FRAGMENT,plug_vertex_lighting_ADSLightModel,CompleteCode,&unique_int); //use lights

            if(DESIRE(whichOne.base,TWO_MATERIAL_APPEARANCE_SHADER))
                AddDefine(SHADERPART_FRAGMENT,"TWO",CompleteCode);
            //but even if we mean per-fragment, for another dot product per fragment we can upgrade
            //from blinn-phong to phong and get the real phong reflection model 
            //(although dug9 can't tell the difference):
            AddDefine(SHADERPART_FRAGMENT,"PHONG",CompleteCode);
        }else{
            AddDefine(SHADERPART_VERTEX,"LIT",CompleteCode);
            AddDefine(SHADERPART_FRAGMENT,"LIT",CompleteCode);
            //lines and points 
            if( DESIRE(whichOne.base,HAVE_LINEPOINTS_COLOR) ) {
                AddDefine(SHADERPART_VERTEX,"LINE",CompleteCode);
            }else{
                AddDefine(SHADERPART_VERTEX,"LITE",CompleteCode);  //add some lights
                Plug(SHADERPART_VERTEX,plug_vertex_lighting_ADSLightModel,CompleteCode,&unique_int); //use lights
                if(DESIRE(whichOne.base,TWO_MATERIAL_APPEARANCE_SHADER))
                    AddDefine(SHADERPART_VERTEX,"TWO",CompleteCode);
            }
        }
    }
    //textureCoordinategen
    //cubemap texure
    //one tex appearance
    //multi tex appearance
    //cubemap tex
    /*    http://www.web3d.org/documents/specifications/19775-1/V3.3/Part01/components/lighting.html#Lightingon
        "The Material's transparency field modulates the alpha in the texture. Hence, 
        a transparency of 0 will result in an alpha equal to that of the texture. 
        A transparency of 1 will result in an alpha of 0 regardless of the value in the texture."
        That doesn't seem to me to be what browsers Octaga, InstantReality, or Cortona are doing, 
        and its not what table 17-3 and the Lighting equation say is happening. 
        In the table, alpha is never 'modulated' ie there's never an alpha= AT * (1-TM) term.
        - freewrl version 3 and vivaty do modulate.
        I've put a define to set if you don't want modulation ie table 17-3.
        If you do want to modulate ie the above quote "to modulate", comment out the define
        I put a mantis issue to web3d.org for clarification Aug 16, 2016
    */

    if(DESIRE(whichOne.base,HAVE_UNLIT_COLOR)){
        AddDefine(SHADERPART_VERTEX,"UNLIT",CompleteCode);
        AddDefine(SHADERPART_FRAGMENT,"UNLIT",CompleteCode);
    }
    if (DESIRE(whichOne.base,ONE_TEX_APPEARANCE_SHADER) ||
        DESIRE(whichOne.base,HAVE_TEXTURECOORDINATEGENERATOR) ||
        DESIRE(whichOne.base,HAVE_CUBEMAP_TEXTURE) ||
        DESIRE(whichOne.base,MULTI_TEX_APPEARANCE_SHADER)) {
        AddDefine(SHADERPART_VERTEX,"TEX",CompleteCode);
        AddDefine(SHADERPART_FRAGMENT,"TEX",CompleteCode);
        if(DESIRE(whichOne.base,HAVE_TEXTURECOORDINATEGENERATOR) )
            AddDefine(SHADERPART_VERTEX,"TGEN",CompleteCode);
        if(DESIRE(whichOne.base,TEX3D_SHADER)){
            //in theory, if the texcoordgen "COORD" and TextureTransform3D are set in scenefile 
            // and working properly in freewrl, then don't need TEX3D for that node in VERTEX shader
            // which is using tex3dbbox (shape->_extent reworked) to get vertex coords in 0-1 range
            // x Sept 4, 2016 either TextureTransform3D or CoordinateGenerator "COORD" isn't working right for Texture3D
            // so we're using the bbox method
            //vertex str texture coords computed same for both volume and layered tex3d
            AddDefine(SHADERPART_VERTEX,"TEX3D",CompleteCode); 
            AddDefine(SHADERPART_FRAGMENT,"TEX3D",CompleteCode); 
            //fragment part different:
            if(DESIRE(whichOne.base,TEX3D_LAYER_SHADER)){
                //up to 6 textures, with lerp between floor,ceil textures
                AddDefine(SHADERPART_FRAGMENT,"TEX3DLAY",CompleteCode);
                Plug(SHADERPART_FRAGMENT,plug_fragment_texture3Dlayer_apply,CompleteCode,&unique_int);
            }else{
                //TEX3D_VOLUME_SHADER
                //AddDefine(SHADERPART_FRAGMENT,"TEX3D",CompleteCode);
                Plug(SHADERPART_FRAGMENT,plug_fragment_texture3D_apply_volume,CompleteCode,&unique_int);
            }
        }else{
            if(DESIRE(whichOne.base,HAVE_CUBEMAP_TEXTURE)){
                AddDefine(SHADERPART_VERTEX,"CUB",CompleteCode);
                AddDefine(SHADERPART_FRAGMENT,"CUB",CompleteCode);
            } else if(DESIRE(whichOne.base,MULTI_TEX_APPEARANCE_SHADER)){
                AddDefine(SHADERPART_VERTEX,"MTEX",CompleteCode);
                AddDefine(SHADERPART_FRAGMENT,"MTEX",CompleteCode);
            }
            if(DESIRE(whichOne.base,TEXTURE_REPLACE_PRIOR) )
                AddDefine(SHADERPART_FRAGMENT,"TEXREP",CompleteCode);
            if(DESIRE(whichOne.base,TEXALPHA_REPLACE_PRIOR))
                AddDefine(SHADERPART_VERTEX,"TAREP",CompleteCode);

            Plug(SHADERPART_FRAGMENT,plug_fragment_texture_apply,CompleteCode,&unique_int);

            //if(texture has alpha ie channels == 2 or 4) then vertex diffuse = 111 and fragment diffuse*=texture
            //H: we currently assume image alpha, and maybe fill the alpha channel with (1-material.transparency)?
            //AddDefine(SHADERPART_VERTEX,"TAT",CompleteCode);
            //AddDefine(SHADERPART_FRAGMENT,"TAT",CompleteCode);
        }
    }

    //fill properties / hatching
    if(DESIRE(whichOne.base,FILL_PROPERTIES_SHADER)) {
        AddDefine(SHADERPART_VERTEX,"FILL",CompleteCode);        
        AddDefine(SHADERPART_FRAGMENT,"FILL",CompleteCode);        
    }
    //FOG
    if(DESIRE(whichOne.base,FOG_APPEARANCE_SHADER)){
        AddDefine(SHADERPART_VERTEX,"FOG",CompleteCode);        
        AddDefine(SHADERPART_FRAGMENT,"FOG",CompleteCode);    
        if(DESIRE(whichOne.base,HAVE_FOG_COORDS))
            AddDefine(SHADERPART_VERTEX,"FOGCOORD",CompleteCode);
        Plug(SHADERPART_FRAGMENT,plug_fog_apply,CompleteCode,&unique_int);    
    }
    //CLIPPLANE
    //FOG
    if(DESIRE(whichOne.base,CLIPPLANE_SHADER)){
        AddDefine(SHADERPART_VERTEX,"CLIP",CompleteCode);    
        Plug(SHADERPART_VERTEX,vertex_plug_clip_apply,CompleteCode,&unique_int);    
        AddDefine(SHADERPART_FRAGMENT,"CLIP",CompleteCode);    
        Plug(SHADERPART_FRAGMENT,frag_plug_clip_apply,CompleteCode,&unique_int);    
    }
    if(DESIRE(whichOne.base,PARTICLE_SHADER)){
        AddDefine(SHADERPART_VERTEX,"PARTICLE",CompleteCode);
    }
    //EFFECTS - castle game engine effect nodes X3D_Effect with plugs applied here
    EnableEffects(CompleteCode,&unique_int);

    // stripUnusedDefines(CompleteCode);
    // http://freecode.com/projects/unifdef/  example: unifdef -UTEX -UGMTEX shader.vs > out.vs will strip the TEX and MTEX sections out
    // or hack something like https://github.com/evanplaice/pypreprocessor 
    // dug9 hacked py3 for shader pre-processing: http://dug9.users.sourceforge.net/web3d/tests/largetexture/preprocessor_dug9_3T.py

    *fragmentSource = CompleteCode[SHADERPART_FRAGMENT]; //original_fragment; //fs;
    *vertexSource = CompleteCode[SHADERPART_VERTEX]; //original_vertex; //vs;
    if(0){
        //write out ubershader text to files (for preprocessing and analysis)
        static int n = 0;
        char filenamestr[100];
        n++;
        sprintf(filenamestr,"shader_vertex_%d.txt",n);
        FILE* fp = fopen(filenamestr,"w+");
        fwrite(*vertexSource,strlen(*vertexSource)+1,1,fp);
        fclose(fp);
        sprintf(filenamestr,"shader_frag_%d.txt",n);
        fp = fopen(filenamestr,"w+");
        fwrite(*fragmentSource,strlen(*fragmentSource)+1,1,fp);
        fclose(fp);
    }
    return retval;
}

int getSpecificShaderSourceVolume (const GLchar **vertexSource, const GLchar **fragmentSource, shaderflagsstruct whichOne) 
{
    //for building the Builtin (similar to fixed-function pipeline, except from shader parts)
    //in OpenGL_Utils.c L.2553 set usingCastlePlugs = 1 to get in here.
    //whichone - a bitmask of shader requirements, one bit for each requirement, so shader permutation can be built

    int retval, unique_int;
    unsigned int volflags;
    unsigned char volflag[7];
    int kflags,i,k;
    char *CompleteCode[3];
    char *vs, *fs;
    retval = FALSE;
    if(whichOne.usershaders ) //& USER_DEFINED_SHADER_MASK) 
        return retval; //not supported yet as of Aug 9, 2016
    retval = TRUE;

    //generic
    vs = strdup(getVolumeVertex());
    fs = strdup(getVolumeFragment());
        
    CompleteCode[SHADERPART_VERTEX] = vs;
    CompleteCode[SHADERPART_GEOMETRY] = NULL;
    if(whichOne.volume == SHADERFLAGS_VOLUME_STYLE_BLENDED << 4){
        CompleteCode[SHADERPART_FRAGMENT] = STRDUP(volumeBlendedFragmentGLES2);

    }else{
        CompleteCode[SHADERPART_FRAGMENT] = fs;
    }

    // what we really have here: UberShader with CastlePlugs
    // UberShader: one giant shader peppered with #ifdefs, and you add #defines at the top for permutations
    // CastlePlugs: allows users to add effects on to uberShader with PLUGs
    // - and internally, we can do a few permutations with PLUGs too

    if(isMobile){
        AddVersion(SHADERPART_VERTEX, 100, CompleteCode); //lower precision floats
        AddVersion(SHADERPART_FRAGMENT, 100, CompleteCode); //lower precision floats
        AddDefine(SHADERPART_FRAGMENT,"MOBILE",CompleteCode); //lower precision floats
    }else{
        //desktop, emulating GLES2
        AddVersion(SHADERPART_VERTEX, 110, CompleteCode); //lower precision floats
        AddVersion(SHADERPART_FRAGMENT, 110, CompleteCode); //lower precision floats
    }

    if(whichOne.volume == SHADERFLAGS_VOLUME_STYLE_BLENDED << 4){
        *fragmentSource = CompleteCode[SHADERPART_FRAGMENT]; //original_fragment; //fs;
        *vertexSource = CompleteCode[SHADERPART_VERTEX]; //original_vertex; //vs;
        return retval;
    }

    unique_int = 0; //helps generate method name PLUG_xxx_<unique_int> to avoid clash when multiple PLUGs supplied for same PLUG 

    //if(DESIRE(whichOne.volume,TEX3D_SHADER)){
    AddDefine(SHADERPART_FRAGMENT,"TEX3D",CompleteCode); 
    Plug(SHADERPART_FRAGMENT,plug_fragment_texture3D_apply_volume,CompleteCode,&unique_int); //uses TILED
    //}

    if(DESIRE(whichOne.volume,SHADERFLAGS_VOLUME_DATA_BASIC)){
        AddDefine(SHADERPART_FRAGMENT,"BASIC",CompleteCode); 
    }
    if(DESIRE(whichOne.volume,SHADERFLAGS_VOLUME_DATA_ISO)){
        AddDefine(SHADERPART_FRAGMENT,"ISO",CompleteCode); 
        if(DESIRE(whichOne.volume,SHADERFLAGS_VOLUME_DATA_ISO_MODE3)){
            AddDefine(SHADERPART_FRAGMENT,"ISO_MODE3",CompleteCode); 
        }
    }
    if(DESIRE(whichOne.volume,SHADERFLAGS_VOLUME_DATA_SEGMENT)){
        AddDefine(SHADERPART_FRAGMENT,"SEGMENT",CompleteCode); 
    }
    if(DESIRE(whichOne.base,CLIPPLANE_SHADER)){
        AddDefine(SHADERPART_FRAGMENT,"CLIP",CompleteCode);    
        // we use a special function in frag for clip for volume
    }

    //unsigned int 32 bits - 4 for basic, leaves 28/4 = max 7 styles
    //work from left to right (the order declared), skip any 0/null/empties
    volflags = whichOne.volume;
    // this was just here, but is defined above. volflag[7];
    kflags = 0;
    for(i=0;i<7;i++){
        int iflag = (volflags >> (7-i)*4) & 0xF;
        if(iflag){
            volflag[kflags] = iflag;
            kflags++;
        }
    }
    //now volflag[] is in the order declared with no 0s/nulls 
    for(k=0;k<kflags;k++){
        switch(volflag[k]){
        case SHADERFLAGS_VOLUME_STYLE_DEFAULT:
            AddDefine(SHADERPART_FRAGMENT,"DEFAULT",CompleteCode); 
            Plug(SHADERPART_FRAGMENT,plug_voxel_DEFAULT,CompleteCode,&unique_int);
            break;
        case SHADERFLAGS_VOLUME_STYLE_OPACITY:
            AddDefine(SHADERPART_FRAGMENT,"OPACITY",CompleteCode); 
            Plug(SHADERPART_FRAGMENT,plug_voxel_OPACITY,CompleteCode,&unique_int);
            break;
        case SHADERFLAGS_VOLUME_STYLE_BLENDED:
            AddDefine(SHADERPART_FRAGMENT,"BLENDED",CompleteCode); 
            Plug(SHADERPART_FRAGMENT,plug_voxel_BLENDED,CompleteCode,&unique_int);
            break;
        case SHADERFLAGS_VOLUME_STYLE_BOUNDARY:
            AddDefine(SHADERPART_FRAGMENT,"BOUNDARY",CompleteCode); 
            Plug(SHADERPART_FRAGMENT,plug_voxel_BOUNDARY,CompleteCode,&unique_int);
            break;
        case SHADERFLAGS_VOLUME_STYLE_CARTOON:
            AddDefine(SHADERPART_FRAGMENT,"CARTOON",CompleteCode); 
            Plug(SHADERPART_FRAGMENT,plug_voxel_CARTOON,CompleteCode,&unique_int);
            break;
        case SHADERFLAGS_VOLUME_STYLE_COMPOSED:
            AddDefine(SHADERPART_FRAGMENT,"COMPOSED",CompleteCode); 
            Plug(SHADERPART_FRAGMENT,plug_voxel_COMPOSED,CompleteCode,&unique_int);
            break;
        case SHADERFLAGS_VOLUME_STYLE_EDGE:
            AddDefine(SHADERPART_FRAGMENT,"EDGE",CompleteCode); 
            Plug(SHADERPART_FRAGMENT,plug_voxel_EDGE,CompleteCode,&unique_int);
            break;
        case SHADERFLAGS_VOLUME_STYLE_PROJECTION:
            AddDefine(SHADERPART_FRAGMENT,"PROJECTION",CompleteCode); 
            Plug(SHADERPART_FRAGMENT,plug_voxel_PROJECTION,CompleteCode,&unique_int);
            break;
        case SHADERFLAGS_VOLUME_STYLE_SHADED:
            AddDefine(SHADERPART_FRAGMENT,"SHADED",CompleteCode); 
            Plug(SHADERPART_FRAGMENT,plug_voxel_SHADED,CompleteCode,&unique_int);
            //if you want a plug within a plug, then if you include the higher level
            // plug first, then the lower level plug should find its tartget in the CompleteCode
            AddDefine(SHADERPART_FRAGMENT,"LIT",CompleteCode); 
            AddDefine(SHADERPART_FRAGMENT,"LITE",CompleteCode); 
            Plug(SHADERPART_FRAGMENT,plug_vertex_lighting_ADSLightModel,CompleteCode,&unique_int);
            break;
        case SHADERFLAGS_VOLUME_STYLE_SILHOUETTE:
            AddDefine(SHADERPART_FRAGMENT,"SILHOUETTE",CompleteCode); 
            Plug(SHADERPART_FRAGMENT,plug_voxel_SILHOUETTE,CompleteCode,&unique_int);
            break;
        case SHADERFLAGS_VOLUME_STYLE_TONE:
            AddDefine(SHADERPART_FRAGMENT,"TONE",CompleteCode); 
            Plug(SHADERPART_FRAGMENT,plug_voxel_TONE,CompleteCode,&unique_int);
            break;
        default:
            //if 0, just skip
            break;
        }
    }


    //shader doesn't compile?
    //in visual studio, this is a good place to get the composed shader source, then paste into
    // an editor that has line numbers, to get to the ERROR line
    *fragmentSource = CompleteCode[SHADERPART_FRAGMENT]; //original_fragment; //fs;
    *vertexSource = CompleteCode[SHADERPART_VERTEX]; //original_vertex; //vs;
    return retval;

}
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< END MIT, VOLUME RENDERING

static int getSpecificShaderSource (const GLchar *vertexSource[vertexEndMarker], const GLchar *fragmentSource[fragmentEndMarker], 
    shaderflagsstruct whichOne) {
    int iret, userDefined, usingCastlePlugs = 1;
    userDefined = whichOne.usershaders ? TRUE : FALSE;

    if(usingCastlePlugs && !userDefined) { // && !DESIRE(whichOne,SHADINGSTYLE_PHONG)) {
        //new Aug 2016 castle plugs
        if(whichOne.volume)
            iret = getSpecificShaderSourceVolume(vertexSource, fragmentSource, whichOne);
        else
            iret = getSpecificShaderSourceCastlePlugs(vertexSource, fragmentSource, whichOne);
    }else{
        iret = getSpecificShaderSourceOriginal(vertexSource, fragmentSource, whichOne);
    }
    return iret;
}

static void getShaderCommonInterfaces (s_shader_capabilities_t *me) {
    GLuint myProg = me->myShaderProgram;
    int i;


    #ifdef SHADERVERBOSE
    {
    GLsizei count;
    GLuint shaders[10];
    GLint  xxx[10];
    int i;
    GLchar sl[3000];


    printf ("getShaderCommonInterfaces, I am program %d\n",myProg);

    if (glIsProgram(myProg)) 
        printf ("getShaderCommonInterfaces, %d is a program\n",myProg); 
    else 
        printf ("hmmm - it is not a program!\n");
    glGetAttachedShaders(myProg,10,&count,shaders);
    printf ("got %d attached shaders, they are: \n",count);
    for (i=0; i<count; i++) {
        GLsizei len;

        printf ("%d\n",shaders[i]);
        glGetShaderSource(shaders[i],3000,&len,sl);
        printf ("len %d\n",len);
        printf ("sl: %s\n",sl);
    }
    glGetProgramiv(myProg,GL_INFO_LOG_LENGTH, xxx); printf ("GL_INFO_LOG_LENGTH_STATUS %d\n",xxx[0]);
    glGetProgramiv(myProg,GL_LINK_STATUS, xxx); printf ("GL_LINK_STATUS %d\n",xxx[0]);
    glGetProgramiv(myProg,GL_VALIDATE_STATUS, xxx); printf ("GL_VALIDATE_STATUS %d\n",xxx[0]);
    glGetProgramiv(myProg,GL_ACTIVE_ATTRIBUTES, xxx); printf ("GL_ACTIVE_ATTRIBUTES %d\n",xxx[0]);
    glGetProgramiv(myProg,GL_ACTIVE_UNIFORMS, xxx); printf ("GL_ACTIVE_UNIFORMS %d\n",xxx[0]);

    glGetProgramiv(myProg,GL_INFO_LOG_LENGTH, xxx);
    if (xxx[0] != 0) {
        #define MAX_INFO_LOG_SIZE 512
        GLchar infoLog[MAX_INFO_LOG_SIZE];
        glGetProgramInfoLog(myProg, MAX_INFO_LOG_SIZE, NULL, infoLog);
        printf ("log: %s\n",infoLog);
    }
    }
    #endif /* DEBUG */


    me->myMaterialEmission = GET_UNIFORM(myProg,"fw_FrontMaterial.emission");
    me->myMaterialDiffuse = GET_UNIFORM(myProg,"fw_FrontMaterial.diffuse");
    me->myMaterialShininess = GET_UNIFORM(myProg,"fw_FrontMaterial.shininess");
    me->myMaterialAmbient = GET_UNIFORM(myProg,"fw_FrontMaterial.ambient");
    me->myMaterialSpecular = GET_UNIFORM(myProg,"fw_FrontMaterial.specular");

    me->myMaterialBackEmission = GET_UNIFORM(myProg,"fw_BackMaterial.emission");
    me->myMaterialBackDiffuse = GET_UNIFORM(myProg,"fw_BackMaterial.diffuse");
    me->myMaterialBackShininess = GET_UNIFORM(myProg,"fw_BackMaterial.shininess");
    me->myMaterialBackAmbient = GET_UNIFORM(myProg,"fw_BackMaterial.ambient");
    me->myMaterialBackSpecular = GET_UNIFORM(myProg,"fw_BackMaterial.specular");

    //me->lightState = GET_UNIFORM(myProg,"lightState");
    //me->lightType = GET_UNIFORM(myProg,"lightType");
    //me->lightRadius = GET_UNIFORM(myProg,"lightRadius");
    //me->lightcount = GET_UNIFORM(myProg,"lightcount");

    /* get lights in a more normal OpenGL GLSL format */

    /*
        struct gl_LightSourceParameters
        {
        vec4 ambient;              // Aclarri
        vec4 diffuse;              // Dcli
        vec4 specular;             // Scli
        vec4 position;             // Ppli
        vec4 halfVector;           // Derived: Hi
        vec4 spotDirection;        // Sdli
        float spotExponent;        // Srli
        float spotCutoff;          // Crli
        float spotCosCutoff;       // Derived: cos(Crli)
        vec3 Attenuations (const,lin,quad)
        //float constantAttenuation; // K0
        //float linearAttenuation;   // K1
        //float quadraticAttenuation;// K2
        float lightRadius;
        int lightType;
        };


        uniform gl_LightSourceParameters gl_LightSource[gl_MaxLights];
    */
    {
        //using lighsource arrays - see shader
        char uniformName[100];
        me->haveLightInShader = false;
#ifdef USING_SHADER_LIGHT_ARRAY_METHOD
        //char* sndx;
        for (i = 0; i<MAX_LIGHTS; i++) {
            char* sndx;
            /* go through and modify the array for each variable */
            strcpy(uniformName, "lightambient[0]");
            sndx = strstr(uniformName, "["); 
            sndx[1] = '0' + i;
            me->lightAmbient[i] = GET_UNIFORM(myProg, uniformName);

            //ConsoleMessage ("light Uniform test for %d is %s, %d",i,uniformName,me->lightAmbient[i]);

            strcpy(uniformName, "lightdiffuse[0]");
            sndx = strstr(uniformName, "[");
            sndx[1] = '0' + i;
            me->lightDiffuse[i] = GET_UNIFORM(myProg, uniformName);
            //ConsoleMessage ("light Uniform test for %d is %s, %d",i,uniformName,me->lightDiffuse[i]);


            strcpy(uniformName, "lightspecular[0]");
            sndx = strstr(uniformName, "[");
            sndx[1] = '0' + i;
            me->lightSpecular[i] = GET_UNIFORM(myProg, uniformName);
            //ConsoleMessage ("light Uniform test for %d is %s, %d",i,uniformName,me->lightSpecular[i]);


            strcpy(uniformName, "lightposition[0]");
            sndx = strstr(uniformName, "[");
            sndx[1] = '0' + i;
            me->lightPosition[i] = GET_UNIFORM(myProg, uniformName);
            //ConsoleMessage ("light Uniform test for %d is %s, %d",i,uniformName,me->lightPosition[i]);


            // flag used to determine if we have to send light position info to this shader
            if (me->lightPosition[i] != -1) me->haveLightInShader = true;

            strcpy(uniformName, "lightspotDirection[0]");
            sndx = strstr(uniformName, "[");
            sndx[1] = '0' + i;
            me->lightSpotDir[i] = GET_UNIFORM(myProg, uniformName);
            //ConsoleMessage ("light Uniform test for %d is %s, %d",i,uniformName,me->lightSpotDir[i]);


            strcpy(uniformName, "lightspotExponent[0]");
            sndx = strstr(uniformName, "[");
            sndx[1] = '0' + i;
            me->lightSpotBeamWidth[i] = GET_UNIFORM(myProg, uniformName);
            //ConsoleMessage ("light Uniform test for %d is %s, %d",i,uniformName,me->lightSpotBeamWidth[i]);


            strcpy(uniformName, "lightspotCutoff[0]");
            sndx = strstr(uniformName, "[");
            sndx[1] = '0' + i;
            me->lightSpotCutoffAngle[i] = GET_UNIFORM(myProg, uniformName);
            //ConsoleMessage ("light Uniform test for %d is %s, %d",i,uniformName,me->lightSpotCutoffAngle[i]);


            strcpy(uniformName, "lightAttenuations[0]");
            sndx = strstr(uniformName, "[");
            sndx[1] = '0' + i;
            me->lightAtten[i] = GET_UNIFORM(myProg, uniformName);

            strcpy(uniformName, "lightRadius[0]");
            sndx = strstr(uniformName, "[");
            sndx[1] = '0' + i;
            me->lightRadius[i] = GET_UNIFORM(myProg, uniformName);
            //ConsoleMessage ("light Uniform test for %d is %s, %d",i,uniformName,me->lightQuadAtten[i]);

        }

#else //USING_SHADER_LIGHT_ARRAY_METHOD
/*
        strcpy(uniformName,"fw_LightSource[0].");
        for (i=0; i<MAX_LIGHTS; i++) {
            /* go through and modify the array for each variable */
            uniformName[15] = '0' + i;

            strcpy(&uniformName[18],"ambient");
/*
            //ConsoleMessage ("have uniform name request :%s:",uniformName);
            me->lightAmbient[i] = GET_UNIFORM(myProg,uniformName);

            //ConsoleMessage ("light Uniform test for %d is %s, %d",i,uniformName,me->lightAmbient[i]);

            strcpy(&uniformName[18],"diffuse");
            me->lightDiffuse[i] = GET_UNIFORM(myProg,uniformName);
            //ConsoleMessage ("light Uniform test for %d is %s, %d",i,uniformName,me->lightDiffuse[i]);


            strcpy(&uniformName[18],"specular");
            me->lightSpecular[i] = GET_UNIFORM(myProg,uniformName);
            //ConsoleMessage ("light Uniform test for %d is %s, %d",i,uniformName,me->lightSpecular[i]);


            strcpy(&uniformName[18],"position");
            me->lightPosition[i] = GET_UNIFORM(myProg,uniformName);
            //ConsoleMessage ("light Uniform test for %d is %s, %d",i,uniformName,me->lightPosition[i]);


            // flag used to determine if we have to send light position info to this shader
            if (me->lightPosition[i] != -1) me->haveLightInShader = true;

            strcpy(&uniformName[18],"spotDirection");
            me->lightSpotDir[i] = GET_UNIFORM(myProg,uniformName);
            //ConsoleMessage ("light Uniform test for %d is %s, %d",i,uniformName,me->lightSpotDir[i]);


            //strcpy(&uniformName[18],"spotExponent");
            strcpy(&uniformName[18],"spotBeamWidth");
            me->lightSpotBeamWidth[i] = GET_UNIFORM(myProg,uniformName);
            //ConsoleMessage ("light Uniform test for %d is %s, %d",i,uniformName,me->lightSpotBeamWidth[i]);


            strcpy(&uniformName[18],"spotCutoff");
            me->lightSpotCutoffAngle[i] = GET_UNIFORM(myProg,uniformName);
            //ConsoleMessage ("light Uniform test for %d is %s, %d",i,uniformName,me->lightSpotCutoffAngle[i]);


            strcpy(&uniformName[18],"Attenuations");
            me->lightAtten[i] = GET_UNIFORM(myProg,uniformName);

        //strcpy(&uniformName[18],"constantAttenuation");
            //me->lightConstAtten[i] = GET_UNIFORM(myProg,uniformName);
            ////ConsoleMessage ("light Uniform test for %d is %s, %d",i,uniformName,me->lightConstAtten[i]);
            //

            //strcpy(&uniformName[18],"linearAttenuation");
            //me->lightLinAtten[i] = GET_UNIFORM(myProg,uniformName);
            ////ConsoleMessage ("light Uniform test for %d is %s, %d",i,uniformName,me->lightLinAtten[i]);
            //

            //strcpy(&uniformName[18],"quadraticAttenuation");
            //me->lightQuadAtten[i] = GET_UNIFORM(myProg,uniformName);
            ////ConsoleMessage ("light Uniform test for %d is %s, %d",i,uniformName,me->lightQuadAtten[i]);

            strcpy(&uniformName[18],"lightRadius");
            me->lightRadius[i] = GET_UNIFORM(myProg,uniformName);
            //ConsoleMessage ("light Uniform test for %d is %s, %d",i,uniformName,me->lightQuadAtten[i]);

            //strcpy(&uniformName[18],"lightType");
            //me->lightType[i] = GET_UNIFORM(myProg,uniformName);
            //ConsoleMessage ("light Uniform test for %d is %s, %d",i,uniformName,me->lightQuadAtten[i]);
        }
*/
#endif // USING_SHADER_LIGHT_ARRAY_METHOD
//        strcpy(uniformName,"lightType[0]");
//        for (i = 0; i < MAX_LIGHTS; i++) {
//            /* go through and modify the array for each variable */
//            uniformName[10] = '0' + i;
//            me->lightType[i] = GET_UNIFORM(myProg, uniformName);
//        }
    }

    //if (me->haveLightInShader) ConsoleMessage ("this shader HAS lightfields");

    me->ModelViewMatrix = GET_UNIFORM(myProg,"fw_ModelViewMatrix");
    me->ProjectionMatrix = GET_UNIFORM(myProg,"fw_ProjectionMatrix");
    me->NormalMatrix = GET_UNIFORM(myProg,"fw_NormalMatrix");
    me->ModelViewInverseMatrix = GET_UNIFORM(myProg,"fw_ModelViewInverseMatrix");
    //for (i=0; i<MAX_MULTITEXTURE; i++) {
    me->TextureMatrix[0] = GET_UNIFORM(myProg,"fw_TextureMatrix0");
    me->TextureMatrix[1] = GET_UNIFORM(myProg,"fw_TextureMatrix1");
    me->TextureMatrix[2] = GET_UNIFORM(myProg,"fw_TextureMatrix2");
    me->TextureMatrix[3] = GET_UNIFORM(myProg,"fw_TextureMatrix3");

    me->Vertices = GET_ATTRIB(myProg,"fw_Vertex");

    me->Normals = GET_ATTRIB(myProg,"fw_Normal");
    me->Colours = GET_ATTRIB(myProg,"fw_Color");
    me->FogCoords = GET_ATTRIB(myProg,"fw_FogCoords");


    //for (i=0; i<MAX_MULTITEXTURE; i++) {
    me->TexCoords[0] = GET_ATTRIB(myProg,"fw_MultiTexCoord0");
    me->TexCoords[1] = GET_ATTRIB(myProg,"fw_MultiTexCoord1");
    me->TexCoords[2] = GET_ATTRIB(myProg,"fw_MultiTexCoord2");
    me->TexCoords[3] = GET_ATTRIB(myProg,"fw_MultiTexCoord3");


    for (i=0; i<MAX_MULTITEXTURE; i++) {
        char line[200];
        sprintf (line,"fw_Texture_unit%d",i);
        me->TextureUnit[i]= GET_UNIFORM(myProg,line);
        sprintf (line,"fw_Texture_mode%d",i);
        me->TextureMode[i] = GET_UNIFORM(myProg,line);
        sprintf (line,"fw_Texture_source%d",i);
        me->TextureSource[i] = GET_UNIFORM(myProg,line);
        sprintf (line,"fw_Texture_function%d",i);
        me->TextureFunction[i] = GET_UNIFORM(myProg,line);
        //printf ("   i %d tu %d mode %d\n",i,me->TextureUnit[i],me->TextureMode[i]);

    }

    me->textureCount = GET_UNIFORM(myProg,"textureCount");
    me->multitextureColor = GET_UNIFORM(myProg,"mt_Color");
    //printf ("GETUNIFORM for textureCount is %d\n",me->textureCount);

    //texture3D
    me->tex3dTiles = GET_UNIFORM(myProg,"tex3dTiles");
    me->tex3dUseVertex = GET_UNIFORM(myProg,"tex3dUseVertex");
    me->magFilter = GET_UNIFORM(myProg,"magFilter");
    me->repeatSTR = GET_UNIFORM(myProg,"repeatSTR");


    /* for FillProperties */
    me->myPointSize = GET_UNIFORM(myProg, "pointSize");
    me->hatchColour = GET_UNIFORM(myProg,"HatchColour");
    me->hatchPercent = GET_UNIFORM(myProg,"HatchPct");
    me->hatchScale = GET_UNIFORM(myProg,"HatchScale");
    me->filledBool = GET_UNIFORM(myProg,"filled");
    me->hatchedBool = GET_UNIFORM(myProg,"hatched");
    me->algorithm = GET_UNIFORM(myProg,"algorithm");

    me->fogColor = GET_UNIFORM(myProg,"fw_fogparams.fogColor");
    me->fogvisibilityRange = GET_UNIFORM(myProg,"fw_fogparams.visibilityRange");
    me->fogScale = GET_UNIFORM(myProg,"fw_fogparams.fogScale");
    me->fogType = GET_UNIFORM(myProg,"fw_fogparams.fogType");

    /* clipplane */
    me->clipplanes = GET_UNIFORM(myProg,"fw_clipplanes");
    me->nclipplanes = GET_UNIFORM(myProg,"fw_nclipplanes");

    /* TextureCoordinateGenerator */
    me->texCoordGenType = GET_UNIFORM(myProg,"fw_textureCoordGenType");


    #ifdef VERBOSE
    printf ("shader uniforms: vertex %d normal %d modelview %d projection %d\n",
        me->Vertices, me->Normals, me->ModelViewMatrix, me->ProjectionMatrix);
    printf ("hatchColour %d, hatchPercent %d",me->hatchColour, me->hatchPercent);
    #endif
}

static void makeAndCompileShader(struct shaderTableEntry *me) {

    GLint success;
    GLuint myVertexShader = 0;
    GLuint myFragmentShader= 0;

    GLuint myProg = 0;
    s_shader_capabilities_t *myShader = me->myCapabilities;
    const GLchar *vertexSource[vertexEndMarker];
    const GLchar  *fragmentSource[fragmentEndMarker];
//    vertexShaderResources_t x1;
//    fragmentShaderResources_t x2;
    int x1;
    int x2;


#ifdef VERBOSE
    ConsoleMessage ("makeAndCompileShader called");
#endif //VERBOSE
#undef VERBOSE

    /* initialize shader sources to blank strings, later we'll fill it in */
    for (x1=vertexGLSLVersion; x1<vertexEndMarker; x1++)
        vertexSource[x1] = "";
    for (x2=fragmentGLSLVersion; x2<fragmentEndMarker; x2++)
        fragmentSource[x2] = "";


    /* pointerize this */
    myProg = glCreateProgram(); /* CREATE_PROGRAM */
    (*myShader).myShaderProgram = myProg;

    /* assume the worst... */
    (*myShader).compiledOK = FALSE;

    /* we put the sources in 2 formats, allows for differing GL/GLES prefixes */
    if (!getSpecificShaderSource(vertexSource, fragmentSource, me->whichOne)) {
        return;
    }


    myVertexShader = CREATE_SHADER (VERTEX_SHADER);
    SHADER_SOURCE(myVertexShader, vertexEndMarker, ((const GLchar **)vertexSource), NULL);
    COMPILE_SHADER(myVertexShader);
    GET_SHADER_INFO(myVertexShader, COMPILE_STATUS, &success);
    if (!success) {
        shaderErrorLog(myVertexShader,(char *)"VERTEX");
    } else {

        ATTACH_SHADER(myProg, myVertexShader);
    }

    /* get Fragment shader */
    myFragmentShader = CREATE_SHADER (FRAGMENT_SHADER);
    SHADER_SOURCE(myFragmentShader, fragmentEndMarker, (const GLchar **) fragmentSource, NULL);
    COMPILE_SHADER(myFragmentShader);
    GET_SHADER_INFO(myFragmentShader, COMPILE_STATUS, &success);
    if (!success) {
        shaderErrorLog(myFragmentShader,(char *)"FRAGMENT");
    } else {
        ATTACH_SHADER(myProg, myFragmentShader);
    }

    LINK_SHADER(myProg);

    glGetProgramiv(myProg,GL_LINK_STATUS, &success);
    (*myShader).compiledOK = (success == GL_TRUE);

    getShaderCommonInterfaces(myShader);
}

/* find a shader that matches the capabilities requested. If no match, recreate it */
s_shader_capabilities_t *getMyShaders(shaderflagsstruct rq_cap0) { //unsigned int rq_cap0) {

    /* GL_ES_VERSION_2_0 has GL_SHADER_COMPILER */
    #ifdef GL_SHADER_COMPILER
    GLboolean b;
    static bool haveDoneThis = false;
    #endif
    //unsigned int rq_cap;
    shaderflagsstruct rq_cap;
    int i;

    ppOpenGL_Utils p = openGL_Utils_prv;
    struct Vector *myShaderTable = p->myShaderTable;
    struct shaderTableEntry *new1 = NULL;

    rq_cap = rq_cap0;
    //rq_cap = NO_APPEARANCE_SHADER; //for thunking to simplest when debugging

    for (i=0; i<vectorSize(myShaderTable); i++) {
        struct shaderTableEntry *me = vector_get(struct shaderTableEntry *,myShaderTable, i);
        if(rq_cap0.volume){
            if(me->whichOne.volume == rq_cap0.volume && me->whichOne.effects == rq_cap0.effects){
                return me->myCapabilities;
            }
        }else{
            if (me->whichOne.base == rq_cap0.base && me->whichOne.effects == rq_cap0.effects && me->whichOne.usershaders == rq_cap0.usershaders) {
                //printf("getMyShaders chosen shader caps base %d effects %d user %d\n",me->whichOne.base,me->whichOne.effects,me->whichOne.usershaders);
                return me->myCapabilities;
            }
        }
    }


    // if here, we did not find the shader already compiled for us.

    //ConsoleMessage ("getMyShader, looking for %x",rq_cap);

    //ConsoleMessage ("getMyShader, not found, have to create");
    //for (i=0; i<vectorSize(myShaderTable); i++) {
        //struct shaderTableEntry *me = vector_get(struct shaderTableEntry *,myShaderTable, i);
        //ConsoleMessage ("getMyShader, i %d, rq_cap %x, me->whichOne %x myCap %p\n",i,rq_cap,me->whichOne,me->myCapabilities);
        //}





    /* GL_ES_VERSION_2_0 has GL_SHADER_COMPILER */
#ifdef GL_SHADER_COMPILER
        glGetBooleanv(GL_SHADER_COMPILER,&b);
        if (!haveDoneThis) {
            haveDoneThis = true;
            if (!b) {
            //I found desktop openGL version 2.1.2  comes in here, but does still render OK
            //ConsoleMessage("NO SHADER COMPILER - have to sometime figure out binary shader distros");
            ConsoleMessage("no shader compiler\n");
            return NULL;
            }
        }
#endif

    // ConsoleMessage ("getMyShader, here now");

#ifdef VERBOSE
#if defined (GL_SHADER_COMPILER) && defined (GL_HIGH_FLOAT)
    /* GL_ES_VERSION_2_0 variables for shaders */
    { /* debugging */
    GLint range[2]; GLint precision;
    GLboolean b;

    ConsoleMessage("starting getMyShader");

    glGetBooleanv(GL_SHADER_COMPILER,&b);
    if (b) ConsoleMessage("have shader compiler"); else ConsoleMessage("NO SHADER COMPILER");


    glGetShaderPrecisionFormat(GL_VERTEX_SHADER,GL_LOW_FLOAT, range, &precision);
    ConsoleMessage ("GL_VERTEX_SHADER, GL_LOW_FLOAT range [%d,%d],precision %d",range[0],range[1],precision);
    glGetShaderPrecisionFormat(GL_VERTEX_SHADER,GL_MEDIUM_FLOAT, range, &precision);
    ConsoleMessage ("GL_VERTEX_SHADER, GL_MEDIUM_FLOAT range [%d,%d],precision %d",range[0],range[1],precision);
    glGetShaderPrecisionFormat(GL_VERTEX_SHADER,GL_HIGH_FLOAT, range, &precision);
    ConsoleMessage ("GL_VERTEX_SHADER, GL_HIGH_FLOAT range [%d,%d],precision %d",range[0],range[1],precision);

    glGetShaderPrecisionFormat(GL_VERTEX_SHADER,GL_LOW_INT, range, &precision);
    ConsoleMessage ("GL_VERTEX_SHADER, GL_LOW_INT range [%d,%d],precision %d",range[0],range[1],precision);
    glGetShaderPrecisionFormat(GL_VERTEX_SHADER,GL_MEDIUM_INT, range, &precision);
    ConsoleMessage ("GL_VERTEX_SHADER, GL_MEDIUM_INT range [%d,%d],precision %d",range[0],range[1],precision);
    glGetShaderPrecisionFormat(GL_VERTEX_SHADER,GL_HIGH_INT, range, &precision);
    ConsoleMessage ("GL_VERTEX_SHADER, GL_HIGH_INT range [%d,%d],precision %d",range[0],range[1],precision);

    glGetShaderPrecisionFormat(GL_FRAGMENT_SHADER,GL_LOW_FLOAT, range, &precision);
    ConsoleMessage ("GL_FRAGMENT_SHADER, GL_LOW_FLOAT range [%d,%d],precision %d",range[0],range[1],precision);
    glGetShaderPrecisionFormat(GL_FRAGMENT_SHADER,GL_MEDIUM_FLOAT, range, &precision);
    ConsoleMessage ("GL_FRAGMENT_SHADER, GL_MEDIUM_FLOAT range [%d,%d],precision %d",range[0],range[1],precision);
    glGetShaderPrecisionFormat(GL_FRAGMENT_SHADER,GL_HIGH_FLOAT, range, &precision);
    ConsoleMessage ("GL_FRAGMENT_SHADER, GL_HIGH_FLOAT range [%d,%d],precision %d",range[0],range[1],precision);

    glGetShaderPrecisionFormat(GL_FRAGMENT_SHADER,GL_LOW_INT, range, &precision);
    ConsoleMessage ("GL_FRAGMENT_SHADER, GL_LOW_INT range [%d,%d],precision %d",range[0],range[1],precision);
    glGetShaderPrecisionFormat(GL_FRAGMENT_SHADER,GL_MEDIUM_INT, range, &precision);
    ConsoleMessage ("GL_FRAGMENT_SHADER, GL_MEDIUM_INT range [%d,%d],precision %d",range[0],range[1],precision);
    glGetShaderPrecisionFormat(GL_FRAGMENT_SHADER,GL_HIGH_INT, range, &precision);
    ConsoleMessage ("GL_FRAGMENT_SHADER, GL_HIGH_INT range [%d,%d],precision %d",range[0],range[1],precision);
    }
#endif // #ifdef GL_ES_VERSION_2_0 specific debugging
#endif //VERBOSE

    new1 = MALLOC(struct shaderTableEntry *, sizeof (struct shaderTableEntry));

    new1 ->whichOne = rq_cap;
    new1->myCapabilities = MALLOC(s_shader_capabilities_t*, sizeof (s_shader_capabilities_t));

    //ConsoleMessage ("going to compile new shader for %x",rq_cap);
    makeAndCompileShader(new1);

    vector_pushBack(struct shaderTableEntry*, myShaderTable, new1);

    //ConsoleMessage ("going to return new %p",new);
    //ConsoleMessage ("... myCapabilities is %p",new->myCapabilities);
    return new1->myCapabilities;
}

s_shader_capabilities_t *getMyShader(unsigned int rq_cap0) {
    shaderflagsstruct rq_cap0s;
    memset(&rq_cap0s,0,sizeof(shaderflagsstruct));
    rq_cap0s.base = rq_cap0;
    return getMyShaders(rq_cap0s);
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
printf("textureNumber %d\n", textureNumber);
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
//      ml_enqueue(queue,item);
//    pthread_mutex_unlock(queue_lock);
}

void texitem_enqueue(s_list_t *item){
    ppLoadTextures p;
//    ttglobal tg = gglobal();
//    p = (ppLoadTextures)textures_prv;

//    threadsafe_enqueue_item_signal(item, &p->texture_request_list);
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
//                            FREE_IF_NZ(me->texdata);
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
bool texture_load_from_file(textureTableIndexStruct_s* this_tex, const char *filename)
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
            this_tex->filename = (char *)filename;
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
    if (texture->m_tableIndex == NULL)
        texture->m_tableIndex = myTableIndex;
    else
        myTableIndex = texture->m_tableIndex;
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
    t->prv = textures_prv;
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

ppComponent_Shape component_Shape_prv = NULL;

//getters
struct matpropstruct *getAppearanceProperties(){
    ppComponent_Shape p = component_Shape_prv;

    return &p->appearanceProperties;
}


/*


X3D Volume Rendering Component

*/

/*
Volumee Rendering aka voxels
http://www.web3d.org/documents/specifications/19775-1/V3.3/Part01/components/volume.html
- VolumeData is like shape node with its own equivalents to appearance, geometry
- Style nodes - like appearance, except sometimes composable


Before starting to implement this there are a few other nodes and components that might be needed:
- clipPlane - besides transparent voxels, you may want to slice a volumetric image with a clipplane to look inside IMPLEMENTED SEPT 2016
- we have TextureProperties to get the RST
- Texturing3D component > for 3D image file format reading: 
    http://paulbourke.net/dataformats/volumetric/
    - Simplest 3d texture file, if you are writing your own
    http://www.web3d.org/x3d/content/examples/Basic/VolumeRendering/
    - these examples use nrrd format, not much harder IMPLEMENTED Oct 2, 2016
Links:
    http://http.developer.nvidia.com/GPUGems/gpugems_ch39.html
    - GpuGems online, ideas about volume rendering
    http://teem.sourceforge.net/ 
    - same place as nrrd file format lib
    - unu.exe commandline program is handy:
        print nrrd file header:
            unu head brain.nrrd
        resize an image ie from 512x512x512 to 128x128x128:
            unu resample -s 128 128 128 -i brain.nrrd -o brain128.nrrd
                
    http://castle-engine.sourceforge.net/compositing_shaders.php
    - in the "Compositing Shaders in X3D" .pdf, page 9 mentions volume nodes
    - the VolumeRendering Component has 10 style nodes, and Kambu is suggesting his Plug/hook method
    http://cs.iupui.edu/~tuceryan/research/Microscopy/vis98.pdf
    - paper: "Image-Based Transfer Function Design for Data Exploration in Volume Visualization"
        pain: hard to stay organized with general functions in volume data
        solution: goal-directed composable steps ie sharpen surface, colorize via 'transfer functions'
        1. Apply transfer functions
            A. Gray = F(Gray) or F(F(F(F(Gray)))) ie can chain grayscale functions for each voxel processed
                a) F() image functions - anything from 2D image processing generalized to 3D
                b) F() spatial functions - edge sharpening ie sobel or smoothing also from image processing
            B. Gray to RGBA - lookup table
        2. do your raycasting on RGBA
    http://demos.vicomtech.org/
    - uses webgl and x3dom
    https://www.slicer.org/
    - Uses teem
    http://teem.sourceforge.net/mite/opts.html
    - teem > Mite - has some transfer tables
    http://graphicsrunner.blogspot.ca/2009/01/volume-rendering-101.html
    - shows 'volume raycasting' method, shader (directx) and results
    http://prideout.net/blog/?tag=volume-rendering
    - shows volume raycasting shader example
    https://www.opengl.org/discussion_boards/showthread.php/174814-Save-vertices-to-texture-problem
    - xyz texture preparation, related to volume raycasting shader technique
    http://http.developer.nvidia.com/GPUGems3/gpugems3_ch30.html
    - see 30.3.1 Volume Rendering for raymarching nuances


COMPONENT VOLUMERENDERING ISSUES Oct 29, 2016
http://dug9.users.sourceforge.net/web3d/tests/volume/
- these sample scenes use max size 128 .nrrd textures

By Node:

VoiumeData
    - works
SegmentedVolumeData
    - no confirmation its working
    - SegmentedVentricles.x3d - cycles every 3 seconds or so
IsoSurfaceVolumeData
    - no confirm
    - IsoSurfaceSkull.x3d

BlendedVolumeStyle
    - blackscreens
    - BlendedBodyInternals.x3d - blackscreens
    - BlendedComposedVolumes.x3d - blackscreens
BoundaryEnhancementVolumeStyle
    - no confirm
    - BoundaryEnhancementInternals.x3d - blackscreens
    - BlendedComposedVolumes.x3d - blackscreens
CartoonVolumeStyle
    - works
    - CartoonBackpack.x3d 
    - BlendedComposedVolumes.x3d
ComposedVolumeStyle
    - no confirm
    - ComposedBackpack.x3d
    - BlendedComposedVolumes.x3d
EdgeEnhancementVolumeStyle
    - works
    - EdgeBrain.x3d
    - ComposedBackpack.x3d
    - BlendedComposedVolumes.x3d
OpacityMapVolumeStyle
    - no TransferFunction verification
    - basics (implicit) works, plus explicit:
    - BlendedComposedVolumes.x3d
    - SegmentedVentricles.x3d
ProjectionVolumeStyle
    - works
    - ProjectionMaxVentricles.x3d 
ShadedVolumeStyle
    - no confirm
    - ShadedBrain.x3d - no evidence of shading
SillouetteEnhancementVolumeStyle
    - works but hard to see a difference
    - SilhouetteSkull.x3d
    - ComposedBackpack.x3d
    - BlendedComposedVolumes.x3d
ToneMappedVolumeStyle
    - works
    - BlendedComposedVolumes.x3d
    - ToneInternal.x3d - blackscreens


By Technique:

gradients:
    IsoSurfaceVolumeData

surfaceNormals:
    CartoonVolumeStyle
    EdgeEnhacementVolumeStyle
    ShadedVolumeStyle
    SilhouetteEnhancementVolumeStyle
    ToneMappedVolumeStyle

segmentIdentifiers:
    SegmentedVolumeData

Texture2D as transfer function:
    BlendedVolumeStyle > weightTransferFunction1, weightTransferFunction2
    OpacityMapVolumeStyle > transferFunction

MF list:
MFNode -renderStyle > ComposedVolumeStyle, IsoSurfaceVolumeData, SegmentedVolumeStyle
MFFloat -surfaceValues > IsoSurfaceVolumeData


*/

typedef struct pComponent_VolumeRendering{
    GLuint front_texture;
    GLuint back_texture;
    GLint ifbobuffer;
    GLint idepthbuffer;
    int width, height;
    GLfloat *quad;
}* ppComponent_VolumeRendering;
void *Component_VolumeRendering_constructor(){
    void *v = MALLOCV(sizeof(struct pComponent_VolumeRendering));
    memset(v,0,sizeof(struct pComponent_VolumeRendering));
    return v;
}
void Component_VolumeRendering_init(struct tComponent_VolumeRendering *t){
    //public
    //private
    t->prv = Component_VolumeRendering_constructor();
    {
        ppComponent_VolumeRendering p = (ppComponent_VolumeRendering)t->prv;
        p->back_texture = 0;
        p->front_texture = 0;
        p->ifbobuffer = 0;
        p->idepthbuffer = 0;
        p->width = 0;
        p->height = 0;
        p->quad = NULL;
    }
}
void Component_VolumeRendering_clear(struct tComponent_VolumeRendering *t){
    //public
    //private
    {
        ppComponent_VolumeRendering p = (ppComponent_VolumeRendering)t->prv;
        //p->front_texture;
        //p->back_texture;
        FREE_IF_NZ(p->quad);
    }
}
//ppComponent_VolumeRendering p = (ppComponent_VolumeRendering)gglobal()->Component_VolumeRendering.prv;




//6 faces x 2 triangles per face x 3 vertices per triangle x 3 scalars (xyz) per vertex = 6 x 2 x 3 x 3 = 108
GLfloat box [108] = {1.0f, 1.0f, 1.0f, -1.0f, 1.0f, 1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, -1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, -1.0f, -1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, -1.0f, -1.0f, -1.0f, 1.0f, -1.0f, 1.0f, -1.0f, -1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, -1.0f, 1.0f, 1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, -1.0f, -1.0f, 1.0f, 1.0f, -1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f, 1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f, };

//      6  7 //back far z
//      4  5
// 2  3   //front near z
// 0  1 
float boxvert [24] = {
    -.5f,-.5f, .5f, .5f,-.5f, .5f, -.5f,.5f, .5f, .5f,.5f, .5f, //near z LL LR UL UR
    -.5f,-.5f,-.5f, .5f,-.5f,-.5f, -.5f,.5f,-.5f, .5f,.5f,-.5f, //far z
};
//ccw tris
short boxtriindccw [48] = {
    0, 1, 3, -1,  //near z
    3, 2, 0, -1,
    1, 5, 7, -1, //right
    7, 3, 1, -1,
    5, 4, 6, -1, //back z
    6, 7, 5, -1, 
    4, 0, 2, -1, //left
    2, 6, 4, -1,
    2, 3, 7, -1, //top y
    7, 6, 2, -1,
    4, 5, 1, -1, //bottom y
    1, 0, 4, -1,
};
short boxtriindcw [48] = {
0, 3, 1, -1,  //near z
3, 0, 2, -1,
1, 7, 5, -1, //right
7, 1, 3, -1,
5, 6, 4, -1, //back z
6, 5, 7, -1, 
4, 2, 0, -1, //left
2, 4, 6, -1,
2, 7, 3, -1, //top y
7, 2, 6, -1,
4, 1, 5, -1, //bottom y
1, 4, 0, -1,
};

void compile_VolumeData(NodeVolumeData *node){
    int i,j,itri, ind, jvert;
    float *boxtris;

    ConsoleMessage("compile_volumedata\n");
    if(node->m_boxtris == NULL){
        node->m_boxtris = (float *)MALLOC(void *,108 * sizeof(float));
    }
    boxtris = (float*)node->m_boxtris;
    if(0)
    for(i=0;i<36;i++){
        for(j=0;j<3;j++)
            boxtris[i*3 + j] = .5f * node->dimensions()->getValue()[j] * box[i*3 + j];  //raw triangles are -1 to 1, dimensions are absolute

    }
    if(1)
    for(itri=0;itri<12;itri++){
        for(jvert=0;jvert<3;jvert++) {
            float *vert;
            ind = boxtriindccw[itri*4 + jvert];
            vert = &boxvert[ind*3];
            for(j=0;j<3;j++){
                boxtris[(itri*3 +jvert)*3 + j] = node->dimensions()->getValue()[j]*vert[j];
            }
        }
    }
    //for(i=0;i<36;i++)
    //    printf("%f %f %f\n",boxtris[i*3 +0],boxtris[i*3 +1],boxtris[i*3 +2]);
    MARK_NODE_COMPILED
}

#ifdef OLDCODE
#ifdef GL_DEPTH_COMPONENT32
#define FW_GL_DEPTH_COMPONENT GL_DEPTH_COMPONENT32
#else
#define FW_GL_DEPTH_COMPONENT GL_DEPTH_COMPONENT16
#endif
#endif //OLDCODE


void gluMultMatricesd(const GLDOUBLE a[16], const GLDOUBLE b[16],    GLDOUBLE r[16]);
int gluInvertMatrixd(const GLDOUBLE m[16], GLDOUBLE invOut[16]);
ivec4 get_current_viewport();

int getTextureTableIndexFromFromTextureNode(Node *node){
    int thisTexture = -1;
    int thisTextureType = node->getType();

    if (thisTextureType==X3D_PIXEL_TEXTURE_3D){
        NodePixelTexture3D* pt = (NodePixelTexture3D*) node;
        thisTexture = pt->m_textureTableIndex;
    } else if (thisTextureType==X3D_IMAGE_TEXTURE_3D){
        NodeImageTexture3D* pt = (NodeImageTexture3D*) node;
        thisTexture = pt->m_textureTableIndex;
    } else {
        ConsoleMessage ("Invalid type for texture\n");
    }
    return thisTexture;
}

void gluMultMatricesd(const GLDOUBLE a[16], const GLDOUBLE b[16],
					GLDOUBLE r[16])
{
	int i, j;

	for (i = 0; i < 4; i++) {
		for (j = 0; j < 4; j++) {
			r[i*4+j] =
				a[i*4+0]*b[0*4+j] +
				a[i*4+1]*b[1*4+j] +
				a[i*4+2]*b[2*4+j] +
				a[i*4+3]*b[3*4+j];
		}
	}
}

/*
** Invert 4x4 matrix.
** Contributed by David Moore (See Mesa bug #6748)
*/
int gluInvertMatrixd(const GLDOUBLE m[16], GLDOUBLE invOut[16])
{
	GLDOUBLE inv[16], det;
	int i;

	inv[0] =   m[5]*m[10]*m[15] - m[5]*m[11]*m[14] - m[9]*m[6]*m[15]
				+ m[9]*m[7]*m[14] + m[13]*m[6]*m[11] - m[13]*m[7]*m[10];
	inv[4] =  -m[4]*m[10]*m[15] + m[4]*m[11]*m[14] + m[8]*m[6]*m[15]
				- m[8]*m[7]*m[14] - m[12]*m[6]*m[11] + m[12]*m[7]*m[10];
	inv[8] =   m[4]*m[9]*m[15] - m[4]*m[11]*m[13] - m[8]*m[5]*m[15]
				+ m[8]*m[7]*m[13] + m[12]*m[5]*m[11] - m[12]*m[7]*m[9];
	inv[12] = -m[4]*m[9]*m[14] + m[4]*m[10]*m[13] + m[8]*m[5]*m[14]
				- m[8]*m[6]*m[13] - m[12]*m[5]*m[10] + m[12]*m[6]*m[9];
	inv[1] =  -m[1]*m[10]*m[15] + m[1]*m[11]*m[14] + m[9]*m[2]*m[15]
				- m[9]*m[3]*m[14] - m[13]*m[2]*m[11] + m[13]*m[3]*m[10];
	inv[5] =   m[0]*m[10]*m[15] - m[0]*m[11]*m[14] - m[8]*m[2]*m[15]
				+ m[8]*m[3]*m[14] + m[12]*m[2]*m[11] - m[12]*m[3]*m[10];
	inv[9] =  -m[0]*m[9]*m[15] + m[0]*m[11]*m[13] + m[8]*m[1]*m[15]
				- m[8]*m[3]*m[13] - m[12]*m[1]*m[11] + m[12]*m[3]*m[9];
	inv[13] =  m[0]*m[9]*m[14] - m[0]*m[10]*m[13] - m[8]*m[1]*m[14]
				+ m[8]*m[2]*m[13] + m[12]*m[1]*m[10] - m[12]*m[2]*m[9];
	inv[2] =   m[1]*m[6]*m[15] - m[1]*m[7]*m[14] - m[5]*m[2]*m[15]
				+ m[5]*m[3]*m[14] + m[13]*m[2]*m[7] - m[13]*m[3]*m[6];
	inv[6] =  -m[0]*m[6]*m[15] + m[0]*m[7]*m[14] + m[4]*m[2]*m[15]
				- m[4]*m[3]*m[14] - m[12]*m[2]*m[7] + m[12]*m[3]*m[6];
	inv[10] =  m[0]*m[5]*m[15] - m[0]*m[7]*m[13] - m[4]*m[1]*m[15]
				+ m[4]*m[3]*m[13] + m[12]*m[1]*m[7] - m[12]*m[3]*m[5];
	inv[14] = -m[0]*m[5]*m[14] + m[0]*m[6]*m[13] + m[4]*m[1]*m[14]
				- m[4]*m[2]*m[13] - m[12]*m[1]*m[6] + m[12]*m[2]*m[5];
	inv[3] =  -m[1]*m[6]*m[11] + m[1]*m[7]*m[10] + m[5]*m[2]*m[11]
				- m[5]*m[3]*m[10] - m[9]*m[2]*m[7] + m[9]*m[3]*m[6];
	inv[7] =   m[0]*m[6]*m[11] - m[0]*m[7]*m[10] - m[4]*m[2]*m[11]
				+ m[4]*m[3]*m[10] + m[8]*m[2]*m[7] - m[8]*m[3]*m[6];
	inv[11] = -m[0]*m[5]*m[11] + m[0]*m[7]*m[9] + m[4]*m[1]*m[11]
				- m[4]*m[3]*m[9] - m[8]*m[1]*m[7] + m[8]*m[3]*m[5];
	inv[15] =  m[0]*m[5]*m[10] - m[0]*m[6]*m[9] - m[4]*m[1]*m[10]
				+ m[4]*m[2]*m[9] + m[8]*m[1]*m[6] - m[8]*m[2]*m[5];

	det = m[0]*inv[0] + m[1]*inv[4] + m[2]*inv[8] + m[3]*inv[12];
	if (det == 0)
		return GL_FALSE;

	det = 1.0 / det;

	for (i = 0; i < 16; i++)
		invOut[i] = inv[i] * det;

	return GL_TRUE;
}

textureTableIndexStruct_s *getTableTableFromTextureNode(Node *textureNode){
    textureTableIndexStruct_s *ret = NULL;
    int index = getTextureTableIndexFromFromTextureNode(textureNode);
    if(index > -1)
        ret = getTableIndex(index);
    return ret;
}


unsigned int prep_volumestyle(Node *vstyle, unsigned int volflags){
    NodeOpacityMapVolumeStyle *style0 = (NodeOpacityMapVolumeStyle*)vstyle;
    if(style0->enabled()->getValue()){
        switch(vstyle->getType()){
            case X3D_OPACITY_MAP_VOLUME_STYLE:
                volflags = volflags << 4;
                volflags |= SHADERFLAGS_VOLUME_STYLE_OPACITY;
                break;
            case X3D_BLENDED_VOLUME_STYLE:
                //volflags = volflags << 4;
                //volflags |= SHADERFLAGS_VOLUME_STYLE_BLENDED;
                //do nothing, so parent renders as default, or gets a style via composed
                break;
            case X3D_BOUNDARY_ENHANCEMENT_VOLUME_STYLE:
                volflags = volflags << 4;
                volflags |= SHADERFLAGS_VOLUME_STYLE_BOUNDARY;
                break;
            case X3D_CARTOON_VOLUME_STYLE:
                volflags = volflags << 4;
                volflags |= SHADERFLAGS_VOLUME_STYLE_CARTOON;
                break;
            case X3D_COMPOSED_VOLUME_STYLE:
                {
                    int i;
                    NodeComposedVolumeStyle *style = (NodeComposedVolumeStyle*)vstyle;
                    //volflags = volflags << 4;
                    //volflags |= SHADERFLAGS_VOLUME_STYLE_COMPOSED;
                    // I 'unroll' composed here, into a bit-shifted list with 4 bits per entry
                    for(i=0;i<style->renderStyle()->getSize();i++){
                        volflags = prep_volumestyle(style->renderStyle()->getValue(i), volflags);
                    }
                }
                break;
            case X3D_EDGE_ENHANCEMENT_VOLUME_STYLE:
                volflags = volflags << 4;
                volflags |= SHADERFLAGS_VOLUME_STYLE_EDGE;
                break;
            case X3D_PROJECTION_VOLUME_STYLE:
                volflags = volflags << 4;
                volflags |= SHADERFLAGS_VOLUME_STYLE_PROJECTION;
                break;
            case X3D_SHADED_VOLUME_STYLE:
                volflags = volflags << 4;
                volflags |= SHADERFLAGS_VOLUME_STYLE_SHADED;
                break;
            case X3D_SILHOUETTE_ENHANCEMENT_VOLUME_STYLE:
                volflags = volflags << 4;
                volflags |= SHADERFLAGS_VOLUME_STYLE_SILHOUETTE;
                break;
            case X3D_TONE_MAPPED_VOLUME_STYLE:
                volflags = volflags << 4;
                volflags |= SHADERFLAGS_VOLUME_STYLE_TONE;
                break;
            default:
                break;
        }
    }
    return volflags;
}

void profile_start(const char *name){}
void profile_end(const char *name){}

void render_volume_data(Node *renderStyle, Node *voxels, NodeVolumeData *node);
NodeMaterial *get_material_oneSided();
void render_volumestyle(Node *vstyle, GLint myProg){
    NodeOpacityMapVolumeStyle *style0 = (NodeOpacityMapVolumeStyle*)vstyle;
    if(style0->enabled()->getValue()){
        switch(vstyle->getType()){
            case X3D_OPACITY_MAP_VOLUME_STYLE:
                {
                    // http://www.web3d.org/documents/specifications/19775-1/V3.3/Part01/components/volume.html#OpacityMapVolumeStyle
                    // Q. how do the transfer function on GPU? its defined like its on the CPU ie 
                    //   with integers. On GPU the .a will be 0.0 to 1.0. I guess that can be used as 1D texture coordinate.
                    int havetexture;
                    GLint iopactex;
                    NodeOpacityMapVolumeStyle *style = (NodeOpacityMapVolumeStyle*)vstyle;
                    havetexture = 0;
                    if(style->transferFunction()->getValue()){
                        //load texture
                        struct NodeImageTexture3D *tmpN;
                        textureTableIndexStruct_s *tti;
//                        ttglobal tg = gglobal();

                        POSSIBLE_PROTO_EXPANSION(NodeImageTexture3D *, style->transferFunction()->getValue(),tmpN);
                        renderFuncs_texturenode = tmpN;

                        //problem: I don't want it sending image dimensions to my volume shader,
                        // which could confuse the voxel sampler
                        //render_node(tmpN); //render_node(node->texture); 
                        loadTextureNode(tmpN,NULL);
                        tti = getTableTableFromTextureNode(tmpN);
                        if(tti && tti->status >= TEX_LOADED){
                            glActiveTexture(GL_TEXTURE0+3); 
                            glBindTexture(GL_TEXTURE_2D,tti->OpenGLTexture); 
                            havetexture = 1;
                        }
                    }
                    iopactex = GET_UNIFORM(myProg,"fw_opacTexture");
                    glUniform1i(iopactex,havetexture);

                }
                break;
            case X3D_BLENDED_VOLUME_STYLE:
                {
                    // http://www.web3d.org/documents/specifications/19775-1/V3.3/Part01/components/volume.html#BlendedVolumeStyle
                    NodeBlendedVolumeStyle *style = (NodeBlendedVolumeStyle*)vstyle;
                    //FBO blending
                    //a)  render the parent volumeData to fbo:
                    //    - in prep Blended push fbo
                    //    - render main to fbo
                    //    - in fin Blended: read pixels or save renderbuffer texture 0
                    //b)  in fin Blended: render blended (voxels,stye) as VolumeData to fbo
                    //    - read pixels or same renderbuffer texture 1
                    //c)   pop fbo
                    //d)  set 2 pixelsets as textures
                    //     and render via a special little shader that blends 2 textures and sends to GL_BACK. 
                    #define BLENDED 1
                    #ifdef BLENDED
                    int *fbohandles = style->m_fbohandles;
                    GLint iviewport[4];
                    float vp[4] = {0.0f,1.0f,0.0f,1.0f}; //arbitrary

                    glGetIntegerv(GL_VIEWPORT, iviewport); //xmin,ymin,w,h
                    if(1){
                        if(fbohandles[0] == 0){
                            GLuint depthrenderbuffer;
                            // https://www.opengl.org/wiki/Framebuffer_Object
                            glGenFramebuffers(1, (GLuint *) &fbohandles[0]);
//                            pushnset_framebuffer(fbohandles[0]); //binds framebuffer. we push here, in case higher up we are already rendering the whole scene to an fbo

                            // The depth buffer - optional
                            glGenRenderbuffers(1, &depthrenderbuffer);
                            glBindRenderbuffer(GL_RENDERBUFFER, depthrenderbuffer);
                            glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, iviewport[2],iviewport[3]);
                            glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthrenderbuffer);


                            glGenTextures(1,(GLuint *) &fbohandles[1]);
                            glBindTexture(GL_TEXTURE_2D, fbohandles[1]);
                            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, iviewport[2], iviewport[3], 0, GL_RGBA , GL_UNSIGNED_BYTE, 0);
                            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
                            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

                            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fbohandles[1], 0);

                            
                            glGenTextures(1,(GLuint *)&fbohandles[2]);
                            glBindTexture(GL_TEXTURE_2D, fbohandles[2]);
                            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, iviewport[2], iviewport[3], 0, GL_RGBA , GL_UNSIGNED_BYTE, 0);
                            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
                            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

                            //glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0+1, GL_TEXTURE_2D, fbohandles[2], 0);
                            //--dont assign the second texture till after the parent VolumeData has drawn itself
                            //glDrawBuffers(1,&fbohandles[1]);
                            if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
                                printf("ouch framebuffer not complete\n");
                            //popnset_framebuffer(); //pop after drawing
                        }
                    }
//                    pushnset_viewport(vp); //something to push so we can pop-and-set below, so any mainloop GL_BACK viewport is restored
                    glViewport(0,0,iviewport[2],iviewport[3]); //viewport we want 
                    glClearColor(0.0f,0.0f,0.0f,0.0f); //red, for diagnostics during debugging
                    FW_GL_CLEAR(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

                    #endif //BLENDED
                }
                break;
            case X3D_BOUNDARY_ENHANCEMENT_VOLUME_STYLE:
                {
                    // http://www.web3d.org/documents/specifications/19775-1/V3.3/Part01/components/volume.html#BoundaryEnhancementVolumeStyle
                    NodeBoundaryEnhancementVolumeStyle *style = (NodeBoundaryEnhancementVolumeStyle*)vstyle;
                    //SFFloat     [in,out] boundaryOpacity  0.9     [0,1]
                    //SFFloat     [in,out] opacityFactor    2       [0,?)
                    //SFFloat     [in,out] retainedOpacity  0.2     [0,1]
                    GLint ibebound, iberetain, ibefactor;
                    ibebound = GET_UNIFORM(myProg,"fw_boundaryOpacity");
                    glUniform1f(ibebound,style->boundaryOpacity()->getValue());
                    iberetain = GET_UNIFORM(myProg,"fw_retainedOpacity");
                    glUniform1f(iberetain,style->retainedOpacity()->getValue());
                    ibefactor = GET_UNIFORM(myProg,"fw_opacityFactor");
                    glUniform1f(ibefactor,style->opacityFactor()->getValue());
                }
                break;
            case X3D_CARTOON_VOLUME_STYLE:
                {
                    // http://www.web3d.org/documents/specifications/19775-1/V3.3/Part01/components/volume.html#CartoonVolumeStyle
                    NodeCartoonVolumeStyle *style = (NodeCartoonVolumeStyle*)vstyle;
                    //SFInt32     [in,out] colorSteps       4       [1,64]
                    //SFColorRGBA [in,out] orthogonalColor  1 1 1 1 [0,1]
                    //SFColorRGBA [in,out] parallelColor    0 0 0 1 [0,1]
                    //SFNode      [in,out] surfaceNormals   NULL    [X3DTexture3DNode]
                    GLint itoonsteps, itoonortho, itoonparallel;
                    itoonsteps = GET_UNIFORM(myProg,"fw_colorSteps");
                    glUniform1i(itoonsteps,style->colorSteps()->getValue());
                    itoonortho = GET_UNIFORM(myProg,"fw_orthoColor");
                    glUniform4fv(itoonortho,1,style->orthogonalColor()->getValue());
                    itoonparallel = GET_UNIFORM(myProg,"fw_paraColor");
                    glUniform4fv(itoonparallel,1,style->parallelColor()->getValue());

                }
                break;
            case X3D_COMPOSED_VOLUME_STYLE:
                {
                    int i;
                    // http://www.web3d.org/documents/specifications/19775-1/V3.3/Part01/components/volume.html#ComposedVolumeStyle
                    NodeComposedVolumeStyle *style = (NodeComposedVolumeStyle*)vstyle;
                    for(i=0;i<style->renderStyle()->getSize();i++){
                        render_volumestyle(style->renderStyle()->getValue(i), myProg);
                    }
                }
                break;
            case X3D_EDGE_ENHANCEMENT_VOLUME_STYLE:
                {
                    // http://www.web3d.org/documents/specifications/19775-1/V3.3/Part01/components/volume.html#EdgeEnhancementVolumeStyle
                    //SFColorRGBA [in,out] edgeColor         0 0 0 1 [0,1]
                    //SFBool      [in,out] enabled           TRUE
                    //SFFloat     [in,out] gradientThreshold 0.4     [0,PI]
                    //SFNode      [in,out] metadata          NULL    [X3DMetadataObject]
                    //SFNode      [in,out] surfaceNormals    NULL    [X3DTexture3DNode]
                    NodeEdgeEnhancementVolumeStyle *style = (NodeEdgeEnhancementVolumeStyle*)vstyle;
                    GLint iedgeColor, igradientThreshold;
                    const float *rgba;
                    rgba = style->edgeColor()->getValue();
                    iedgeColor = GET_UNIFORM(myProg,"fw_edgeColor");
                    glUniform4fv(iedgeColor,1,rgba);
                    igradientThreshold = GET_UNIFORM(myProg,"fw_cosGradientThreshold");
                    glUniform1f(igradientThreshold,cosf(style->gradientThreshold()->getValue()));
                    //printf("edge uniforms color %d gradthresh %d\n",iedgeColor,igradientThreshold);
                }
                break;
            case X3D_PROJECTION_VOLUME_STYLE:
                {
                    // http://www.web3d.org/documents/specifications/19775-1/V3.3/Part01/components/volume.html#ProjectionVolumeStyle
                    NodeProjectionVolumeStyle *style = (NodeProjectionVolumeStyle*)vstyle;
                    //SFFloat  [in,out] intensityThreshold 0     [0,1]
                    //SFString [in,put] type               "MAX" ["MAX", "MIN", "AVERAGE"]
                    GLint iintensity, itype;
                    int ktype = 1; // initialize this to something that makes sense
                    const char *ctype;
                    iintensity = GET_UNIFORM(myProg,"fw_intensityThreshold");
                    glUniform1f(iintensity,style->intensityThreshold()->getValue());
                    itype = GET_UNIFORM(myProg,"fw_projType");
                    if(style->m_type == 0){
                        ctype = style->type()->getValue();
                        if(!strcmp(ctype,"MIN")) 
                            ktype = 1;
                        else if(!strcmp(ctype,"MAX")) 
                            ktype = 2;
                        else if(!strcmp(ctype,"AVERAGE")) 
                            ktype = 3;
                        style->m_type = ktype;
                    }
                    glUniform1i(itype,style->m_type);
                }
                break;
            case X3D_SHADED_VOLUME_STYLE:
                {
                    // http://www.web3d.org/documents/specifications/19775-1/V3.3/Part01/components/volume.html#ShadedVolumeStyle
                    GLint iphase, ilite, ishadow;
                    NodeShadedVolumeStyle *style = (NodeShadedVolumeStyle*)vstyle;
                    //SFBool   [in,out] lighting       FALSE
                    //SFNode   [in,out] material       NULL                [X3DMaterialNode]
                    //SFBool   [in,out] shadows        FALSE
                    //SFNode   [in,out] surfaceNormals NULL                [X3DTexture3DNode]
                    //SFString []       phaseFunction  "Henyey-Greenstein" ["Henyey-Greenstein","NONE",...]
                    //MATERIAL
                    if(style->material()->getValue()){
                        struct fw_MaterialParameters defaultMaterials = {
                                    {0.0f, 0.0f, 0.0f, 1.0f}, /* Emission */
                                    {0.0f, 0.0f, 0.0f, 1.0f}, /* Ambient */
                                    {0.8f, 0.8f, 0.8f, 1.0f}, /* Diffuse */
                                    {0.0f, 0.0f, 0.0f, 1.0f}, /* Specular */
                                    10.0f};                   /* Shininess */

                        NodeMaterial *matone;
                        NodeTwoSidedMaterial *mattwo;
                        struct fw_MaterialParameters *fw_FrontMaterial;
                        struct fw_MaterialParameters *fw_BackMaterial;
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
                        struct matpropstruct *myap = getAppearanceProperties();

                        memcpy (&myap->fw_FrontMaterial, &defaultMaterials, sizeof (struct fw_MaterialParameters));
                        memcpy (&myap->fw_BackMaterial, &defaultMaterials, sizeof (struct fw_MaterialParameters));

                        RENDER_MATERIAL_SUBNODES(style->material()->getValue());
                        //struct matpropstruct matprop;
                        //s_shader_capabilities_t mysp;
                        //sendFogToShader(mysp); 
#if 0
                        matone = get_material_oneSided();
                        mattwo = get_material_twoSided();
                        //sendMaterialsToShader(mysp);
                        if (matone != NULL) {
                            memcpy (&myap->fw_FrontMaterial, matone->m_verifiedColor, sizeof (struct fw_MaterialParameters));
                            memcpy (&myap->fw_BackMaterial, matone->m_verifiedColor, sizeof (struct fw_MaterialParameters));
                            /* copy the emissive colour over for lines and points */
                            memcpy(&myap->emissionColour,matone->m_verifiedColor, 3*sizeof(float));

                        } else if (mattwo != NULL) {
                            memcpy (&myap->fw_FrontMaterial, mattwo->m_verifiedFrontColor, sizeof (struct fw_MaterialParameters));
                            memcpy (&myap->fw_BackMaterial, mattwo->m_verifiedBackColor, sizeof (struct fw_MaterialParameters));
                            /* copy the emissive colour over for lines and points */
                            memcpy(&myap->emissionColour,mattwo->m_verifiedFrontColor, 3*sizeof(float));
                        } else {
                            /* no materials selected.... */
                        }
#endif


                        if (!myap) return;
                        fw_FrontMaterial = &myap->fw_FrontMaterial;
                        fw_BackMaterial = &myap->fw_BackMaterial;


//                        PRINT_GL_ERROR_IF_ANY("BEGIN sendMaterialsToShader");

                        /* eventually do this with code blocks in glsl */


                        myMaterialEmission = GET_UNIFORM(myProg,"fw_FrontMaterial.emission");
                        myMaterialDiffuse = GET_UNIFORM(myProg,"fw_FrontMaterial.diffuse");
                        myMaterialShininess = GET_UNIFORM(myProg,"fw_FrontMaterial.shininess");
                        myMaterialAmbient = GET_UNIFORM(myProg,"fw_FrontMaterial.ambient");
                        myMaterialSpecular = GET_UNIFORM(myProg,"fw_FrontMaterial.specular");

                        myMaterialBackEmission = GET_UNIFORM(myProg,"fw_BackMaterial.emission");
                        myMaterialBackDiffuse = GET_UNIFORM(myProg,"fw_BackMaterial.diffuse");
                        myMaterialBackShininess = GET_UNIFORM(myProg,"fw_BackMaterial.shininess");
                        myMaterialBackAmbient = GET_UNIFORM(myProg,"fw_BackMaterial.ambient");
                        myMaterialBackSpecular = GET_UNIFORM(myProg,"fw_BackMaterial.specular");


                        profile_start("sendvec");
                        GLUNIFORM4FV(myMaterialAmbient,1,fw_FrontMaterial->ambient);
                        GLUNIFORM4FV(myMaterialDiffuse,1,fw_FrontMaterial->diffuse);
                        GLUNIFORM4FV(myMaterialSpecular,1,fw_FrontMaterial->specular);
                        GLUNIFORM4FV(myMaterialEmission,1,fw_FrontMaterial->emission);
                        GLUNIFORM1F(myMaterialShininess,fw_FrontMaterial->shininess);

                        GLUNIFORM4FV(myMaterialBackAmbient,1,fw_BackMaterial->ambient);
                        GLUNIFORM4FV(myMaterialBackDiffuse,1,fw_BackMaterial->diffuse);
                        GLUNIFORM4FV(myMaterialBackSpecular,1,fw_BackMaterial->specular);
                        GLUNIFORM4FV(myMaterialBackEmission,1,fw_BackMaterial->emission);
                        GLUNIFORM1F(myMaterialBackShininess,fw_BackMaterial->shininess);
                        profile_end("sendvec");


                    }
                    if(style->lighting()->getValue()){
                        //LIGHT
                        //FOG
                        //-these are from the scenegraph above the voldata node, and -like clipplane- can/should be
                        //set generically
                    }

                    //phasefunc
                    if(style->phaseFunction()->getValue() == 0){
                        if(!strcmp(style->phaseFunction()->getValue(),"NONE"))
                            style->m_phaseFunction = 1;
                        else if(!strcmp(style->phaseFunction()->getValue(),"Henyey-Greenstein"))
                            style->m_phaseFunction = 2;
                    }
                    iphase = GET_UNIFORM(myProg,"fw_phase");
                    glUniform1i(iphase,style->m_phaseFunction);
                    ilite = GET_UNIFORM(myProg,"fw_lighting");
                    glUniform1i(ilite,style->lighting()->getValue());
                    ishadow = GET_UNIFORM(myProg,"fw_shadows");
                    glUniform1i(ishadow,style->shadows()->getValue());
                }
                break;
            case X3D_SILHOUETTE_ENHANCEMENT_VOLUME_STYLE:
                {
                    // http://www.web3d.org/documents/specifications/19775-1/V3.3/Part01/components/volume.html#SilhouetteEnhancementVolumeStyle
                    NodeSilhouetteEnhancementVolumeStyle *style = (NodeSilhouetteEnhancementVolumeStyle*)vstyle;
                    //SFFloat [in,out] silhouetteBoundaryOpacity 0    [0,1]
                    //SFFloat [in,out] silhouetteRetainedOpacity 1    [0,1]
                    //SFFloat [in,out] silhouetteSharpness       0.5  [0,8)
                    GLint isilbound, isilretain, isilsharp;
                    isilbound = GET_UNIFORM(myProg,"fw_BoundaryOpacity");
                    glUniform1f(isilbound,style->silhouetteBoundaryOpacity()->getValue());
                    isilretain = GET_UNIFORM(myProg,"fw_RetainedOpacity");
                    glUniform1f(isilretain,style->silhouetteRetainedOpacity()->getValue());
                    isilsharp = GET_UNIFORM(myProg,"fw_Sharpness");
                    glUniform1f(isilsharp,style->silhouetteSharpness()->getValue());
                }
                break;
            case X3D_TONE_MAPPED_VOLUME_STYLE:
                {
                    // http://www.web3d.org/documents/specifications/19775-1/V3.3/Part01/components/volume.html#ToneMappedVolumeStyle
                    //SFColorRGBA [in,out] coolColor      0 0 1 0 [0,1]
                    //SFColorRGBA [in,out] warmColor      1 1 0 0 [0,1]
                    //SFNode      [in,out] surfaceNormals NULL    [X3DTexture3DNode]
                    NodeToneMappedVolumeStyle *style = (NodeToneMappedVolumeStyle*)vstyle;
                    //send warm, cool to shader
                    GLint icool, iwarm;
                    icool = GET_UNIFORM(myProg,"fw_coolColor");
                    glUniform4fv(icool,1,style->coolColor()->getValue());
                    iwarm = GET_UNIFORM(myProg,"fw_warmColor");
                    glUniform4fv(iwarm,1,style->warmColor()->getValue());
                }
                break;
            default:
                break;
        }
    }
}
static struct {
const char *ctype;
int itype;
} blendfuncs [] = {
{"CONSTANT",1},
{"ALPHA1",2},
{"ALPHA2",3},
{"TABLE",4},
{"ONE_MINUS_ALPHA1",5},
{"ONE_MINUS_ALPHA2",6},
{NULL,0},
};

int lookup_blendfunc(const char *funcname){
    int iret, i;
    i = 0;
    iret = 0;
    do{
        if(!strcmp(blendfuncs[i].ctype,funcname)){
            iret = blendfuncs[i].itype;
            break;
        }
        i++;
    }while(blendfuncs[i].ctype);
    return iret;
}

void sendBindBufferToGPU (GLenum target, GLuint buffer, const char *file, int line) {

    
/*
    if (target == GL_ARRAY_BUFFER_BINDING) printf ("glBindBuffer, GL_ARRAY_BUFFER_BINDING %d at %s:%d\n",buffer,file,line);
    else if (target == GL_ARRAY_BUFFER) printf ("glBindBuffer, GL_ARRAY_BUFFER %d at %s:%d\n",buffer,file,line);
    else if (target == GL_ELEMENT_ARRAY_BUFFER) printf ("glBindBuffer, GL_ELEMENT_ARRAY_BUFFER %d at %s:%d\n",buffer,file,line);
    else printf ("glBindBuffer, %d %d at %s:%d\n",target,buffer,file,line);
    
*/

    glBindBuffer(target,buffer);
}


void render_GENERIC_volume_data(s_shader_capabilities_t *caps, Node **renderStyle, int nstyle, Node *voxels, NodeVolumeData *node );
s_shader_capabilities_t * getVolumeProgram(Node **renderStyle, int nstyle, int VOLUME_DATA_FLAG);
void saveImage_web3dit(struct textureTableIndexStruct *tti, char *fname);

/* finished rendering thisshape. */
void finishedWithGlobalShader(void) {
    //printf ("finishedWithGlobalShader\n");


    /* get rid of the shader */
    getAppearanceProperties()->currentShaderProperties = NULL;
FW_GL_BINDBUFFER(GL_ARRAY_BUFFER, 0);

FW_GL_BINDBUFFER(GL_ELEMENT_ARRAY_BUFFER, 0);

}

/* choose and turn on a shader for this geometry */

void enableGlobalShader(s_shader_capabilities_t *myShader) {
    ppRenderFuncs p = (ppRenderFuncs)renderFuncs_prv;

    //ConsoleMessage ("enableGlobalShader, have myShader %d",myShader->myShaderProgram);
    if (myShader == NULL) {
        finishedWithGlobalShader(); 
        return;
    };
    
    
    getAppearanceProperties()->currentShaderProperties = myShader;
    if (myShader->myShaderProgram != p->currentShader) {
        USE_SHADER(myShader->myShaderProgram);
        p->currentShader = myShader->myShaderProgram;
    }
}

void fin_volumestyle(Node *vstyle, NodeVolumeData *dataParent){
    NodeOpacityMapVolumeStyle *style0 = (NodeOpacityMapVolumeStyle*)vstyle;
    if(style0 && style0->enabled()->getValue()){
        switch(vstyle->getType()){
            case X3D_OPACITY_MAP_VOLUME_STYLE:
                {
                    //do I need to do this?
                    renderFuncs_textureStackTop = 0;
                    renderFuncs_texturenode = NULL;
                }
            break;
            case X3D_BLENDED_VOLUME_STYLE:
                {
                    // http://www.web3d.org/documents/specifications/19775-1/V3.3/Part01/components/volume.html#BlendedVolumeStyle
                    NodeBlendedVolumeStyle *style = (NodeBlendedVolumeStyle*)vstyle;
                    //FBO blending
                    //a)  render the parent volumeData to fbo:
                    //    - in prep Blended push fbo
                    //    - render main to fbo
                    //    - in fin Blended: read pixels or save renderbuffer texture 0
                    //b)  in fin Blended: render blended (voxels,stye) as VolumeData to fbo
                    //    - read pixels or same renderbuffer texture 1
                    //c)   pop fbo
                    //d)  set 2 pixelsets as textures
                    //     and render via a special little shader that blends 2 textures and sends to GL_BACK. 
                #ifdef BLENDED
                    GLuint pixelType = GL_RGBA;
                    int *fbohandles = style->m_fbohandles;
                    if(fbohandles[0] > 0){
                        //readpixels from parent volumedata render
                        static int iframe = 0;
                        s_shader_capabilities_t *caps;
                        int nsubstyle;
                        //GLuint myProg;
                        int method_draw_cube, method_draw_quad;

                        iframe++;
                        //FW_GL_READPIXELS (0,0,isize,isize,pixelType,GL_UNSIGNED_BYTE, ttip->texdata);
                        if(0) if(iframe==500){
                            //write out whats in the framebuffer, and use as texture in test scene, to see fbo rendered OK
                            GLint iviewport[4];
                            char namebuf[100];
                            textureTableIndexStruct_s ttipp, *ttip;
                            ttip = &ttipp;
                            glGetIntegerv(GL_VIEWPORT, iviewport); //xmin,ymin,w,h

                            ttip->texdata = (unsigned char *)MALLOC (GLvoid *, 4*iviewport[2]*iviewport[3]);

                            /* grab the data */
                            //FW_GL_PIXELSTOREI (GL_UNPACK_ALIGNMENT, 1);
                            //FW_GL_PIXELSTOREI (GL_PACK_ALIGNMENT, 1);
                            
                            // https://www.khronos.org/opengles/sdk/docs/man/xhtml/glReadPixels.xml
                            FW_GL_READPIXELS (iviewport[0],iviewport[1],iviewport[2],iviewport[3],pixelType,GL_UNSIGNED_BYTE, ttip->texdata);
                            ttip->x = iviewport[2];
                            ttip->y = iviewport[3];
                            ttip->z = 1;
                            ttip->hasAlpha = 1;
                            ttip->channels = 4;
                            //write out tti as web3dit image files for diagnostic viewing, can use for BackGround node
                            //void saveImage_web3dit(struct textureTableIndexStruct *tti, char *fname)
                            sprintf(namebuf,"%s%d.web3dit","blended_fbo_",0);
                            saveImage_web3dit(ttip, namebuf);
                            FREE_IF_NZ(ttip->texdata);
                        }

                        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fbohandles[2], 0);
                        //glDrawBuffers(1,&fbohandles[2]);

                        glClearColor(0.0f,0.0f,0.0f,0.0f); //red, for diagnostics during debugging
                        FW_GL_CLEAR(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

                        //render blended as volumedata to fbo
                        //render_volume_data(style->renderStyle,style->voxels,dataParent);
                        nsubstyle = style->renderStyle()->getValue() ? 1 : 0;
                        Node *tmpNode = style->renderStyle()->getValue();
                        caps = getVolumeProgram(&tmpNode,nsubstyle, SHADERFLAGS_VOLUME_DATA_BASIC);
                        //render generic volume 
                        render_GENERIC_volume_data(caps,&tmpNode,nsubstyle,style->voxels()->getValue(),(NodeVolumeData*)dataParent );
                        //render_GENERIC_volume_data(caps,style->renderStyle,1,style->voxels,(NodeVolumeData*)dataParent );

                        //glDrawBuffers(0,NULL);

                        //read blended from fbo
                        //FW_GL_READPIXELS (0,0,isize,isize,pixelType,GL_UNSIGNED_BYTE, ttip->texdata);
                        if(0) if(iframe==500){
                            //write out whats in the framebuffer, and use as texture in test scene, to see fbo rendered OK
                            GLint iviewport[4];
                            char namebuf[100];
                            textureTableIndexStruct_s ttipp, *ttip;
                            ttip = &ttipp;
                            glGetIntegerv(GL_VIEWPORT, iviewport); //xmin,ymin,w,h

                            ttip->texdata = (unsigned char*)MALLOC (GLvoid *, 4*iviewport[2]*iviewport[3]);

                            /* grab the data */
                            //FW_GL_PIXELSTOREI (GL_UNPACK_ALIGNMENT, 1);
                            //FW_GL_PIXELSTOREI (GL_PACK_ALIGNMENT, 1);
                            // https://www.khronos.org/opengles/sdk/docs/man/xhtml/glReadPixels.xml
                            FW_GL_READPIXELS (iviewport[0],iviewport[1],iviewport[2],iviewport[3],pixelType,GL_UNSIGNED_BYTE, ttip->texdata);
                            ttip->x = iviewport[2];
                            ttip->y = iviewport[3];
                            ttip->z = 1;
                            ttip->hasAlpha = 1;
                            ttip->channels = 4;
                            //write out tti as web3dit image files for diagnostic viewing, can use for BackGround node
                            //void saveImage_web3dit(struct textureTableIndexStruct *tti, char *fname)
                            sprintf(namebuf,"%s%d.web3dit","blended_fbo_",1);
                            saveImage_web3dit(ttip, namebuf);
                            FREE_IF_NZ(ttip->texdata);
                            printf("wrote blended_fbo_.web3dit \n");
                        }
//                        popnset_framebuffer();
//                        popnset_viewport();
                        //we're now back to rendering to the screen
                        //we should have 2 textures

                        //render 2 textures as blended multitexture, or in special shader for blending, 
                        //2 textures are fbohandles[0] (parent voldata), fbohandles[1] (blend voldata)
                        //over window-filling quad
                        //Options: 
                        //1) draw our cube again, to get the depth (and skip unneeded pixels)
                        //   but use gl_fragCoords as texture interpolator
                        //2) draw quad in ortho mode (but where depth buffer?)
                        method_draw_cube = method_draw_quad = 0;
                        method_draw_cube = 1;
                        if(method_draw_cube){
                            GLint myProg;
                            GLint iwtc1, iwtc2, iwtf1, iwtf2;
                            GLint TextureUnit;
                            int havetextures;
                            GLint iopactex, vp;
                            GLint iviewport[4];
                            float viewport[4];
                            shaderflagsstruct shader_requirements; //shaderflags, 
                            s_shader_capabilities_t *caps;
                            GLint Vertices, mvm, proj;
                            double modelviewMatrix[16], projMatrix[16]; //, mvp[16]; // mvpinverse[16]; //mvmInverse[16], 


                            memset(&shader_requirements,0,sizeof(shaderflagsstruct));
                            shader_requirements.volume = SHADERFLAGS_VOLUME_STYLE_BLENDED << 4; //send the following through the volume ubershader
                            // by default we'll mash it in: shader_requirements.volume |= TEX3D_SHADER;
                            caps = getMyShaders(shader_requirements);
                            enableGlobalShader(caps);
                            myProg = caps->myShaderProgram;

                            //send the usual matrices - same vertex shader as volumedata
                            //but simpler frag shader that uses gl_fragCoords, and does blend

                            //set shader flags
                            //build or get shader program
                            //set attributes 
                            //SFFloat  [in,out] weightConstant1         0.5        [0,1]
                            //SFFloat  [in,out] weightConstant2         0.5        [0,1]
                            //BRUTZMAN: ALPHA0,1 here should be ALPHA1,2 to match table 14.1
                            //SFString [in,out] weightFunction1         "CONSTANT" ["CONSTANT", "ALPHA0", "ALPHA1", "TABLE",
                            //                                                    "ONE_MINUS_ALPHA0", "ONE_MINUS_ALPHA1" ] 
                            //SFString [in,out] weightFunction2         "CONSTANT" ["CONSTANT", "ALPHA0", "ALPHA1", "TABLE",
                            //                                                    "ONE_MINUS_ALPHA0", "ONE_MINUS_ALPHA1" ] 
                            //SFNode   [in,out] weightTransferFunction1 NULL       [X3DTexture2DNode]
                            //SFNode   [in,out] weightTransferFunction2 NULL       [X3DTexture2DNode]

                            iwtc1 = GET_UNIFORM(myProg,"fw_iwtc1");
                            iwtc2 = GET_UNIFORM(myProg,"fw_iwtc2");
                            iwtf1 = GET_UNIFORM(myProg,"fw_iwtf1");
                            iwtf2 = GET_UNIFORM(myProg,"fw_iwtf2");
                            glUniform1f(iwtc1,style->m_weightConstant1);
                            glUniform1f(iwtc2,style->m_weightConstant2);
/*
                            if(style->m_weightFunction1 == 0)
                                style->m_weightFunction1 = lookup_blendfunc(style->weightFunction1->strptr);
                            if(style->m_weightFunction2 == 0)
                                style->_weightFunction2 = lookup_blendfunc(style->weightFunction2->strptr);
                            glUniform1i(iwtf1,style->m_weightFunction1);
*/                            glUniform1i(iwtf2,style->m_weightFunction2);
/*
                            //set the 2 textures from the fbo rendering
                            glActiveTexture ( GL_TEXTURE0 );
                            glBindTexture(GL_TEXTURE_2D,style->m_fbohandles1);
                            FW_GL_TEXPARAMETERI( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);  //don't interpolate integer segment IDs

                            glActiveTexture ( GL_TEXTURE0+1 );
                            glBindTexture(GL_TEXTURE_2D,style->_fbohandles.p[2]);
                            FW_GL_TEXPARAMETERI( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);  //don't interpolate integer segment IDs
*/
                            TextureUnit= GET_UNIFORM(myProg,"fw_Texture_unit0");
                            glUniform1i(TextureUnit,0);
                            TextureUnit= GET_UNIFORM(myProg,"fw_Texture_unit1");
                            glUniform1i(TextureUnit,1);

/*
                            //set the 2 transfer function textures
                            havetextures = 0;
                            if(style->weightTransferFunction1){
                                //load texture
                                Node *tmpN;
                                textureTableIndexStruct_s *tti;
                                ttglobal tg = gglobal();

                                POSSIBLE_PROTO_EXPANSION(struct X3D_Node *, style->weightTransferFunction1,tmpN);
                                tg->RenderFuncs.texturenode = (void*)tmpN;

                                //problem: I don't want it sending image dimensions to my volume shader,
                                // which could confuse the voxel sampler
                                //render_node(tmpN); //render_node(node->texture); 
                                loadTextureNode(tmpN,NULL);
                                tti = getTableTableFromTextureNode(tmpN);
                                if(tti && tti->status >= TEX_LOADED){
                                    glActiveTexture(GL_TEXTURE0+2); 
                                    glBindTexture(GL_TEXTURE_2D,tti->OpenGLTexture); 
                                    havetextures |= 1;
                                }
                            }
                            if(style->weightTransferFunction2){
                                //load texture
                                struct X3D_Node *tmpN;
                                textureTableIndexStruct_s *tti;
                                ttglobal tg = gglobal();

                                POSSIBLE_PROTO_EXPANSION(struct X3D_Node *, style->weightTransferFunction2,tmpN);
                                tg->RenderFuncs.texturenode = (void*)tmpN;

                                //problem: I don't want it sending image dimensions to my volume shader,
                                // which could confuse the voxel sampler
                                //render_node(tmpN); //render_node(node->texture); 
                                loadTextureNode(tmpN,NULL);
                                tti = getTableTableFromTextureNode(tmpN);
                                if(tti && tti->status >= TEX_LOADED){
                                    glActiveTexture(GL_TEXTURE0+3); 
                                    glBindTexture(GL_TEXTURE_2D,tti->OpenGLTexture); 
                                    havetextures |= 2;
                                }
                            }
*/
                            iopactex = GET_UNIFORM(myProg,"fw_haveTransfers");
                            glUniform1i(iopactex,havetextures);


                            glGetIntegerv(GL_VIEWPORT, iviewport); //xmin,ymin,w,h

                            vp = GET_UNIFORM(myProg,"fw_viewport");
                            viewport[0] = (float)iviewport[0]; //xmin
                            viewport[1] = (float)iviewport[1]; //ymin
                            viewport[2] = (float)iviewport[2]; //width
                            viewport[3] = (float)iviewport[3]; //height
                            GLUNIFORM4F(vp,viewport[0],viewport[1],viewport[2],viewport[3]);

                            //draw the box

                            Vertices = GET_ATTRIB(myProg,"fw_Vertex");
                            mvm = GET_UNIFORM(myProg,"fw_ModelViewMatrix"); //fw_ModelViewMatrix
                            proj = GET_UNIFORM(myProg,"fw_ProjectionMatrix"); //fw_ProjectionMatrix
//                            sendExplicitMatriciesToShader(mvm,proj,-1,NULL,-1);
                            FW_GL_GETDOUBLEV(GL_MODELVIEW_MATRIX, modelviewMatrix);
                            FW_GL_GETDOUBLEV(GL_PROJECTION_MATRIX, projMatrix);

                            glEnableVertexAttribArray(Vertices);

                            glVertexAttribPointer(Vertices, 3, GL_FLOAT, GL_FALSE, 0, dataParent->m_boxtris);


                            glEnable(GL_CULL_FACE);
                            glFrontFace(GL_CW); 
                            glDrawArrays(GL_TRIANGLES,0,36);
                            glDisable(GL_CULL_FACE);

                        }else if(method_draw_quad){
                            ////we need a shader that doesn't bother with matrices - just draws quad like ortho
                            //GLint Vertices = GET_ATTRIB(myProg,"fw_Vertex");
                            //glEnableVertexAttribArray(Vertices);
                            //glVertexAttribPointer(Vertices, 3, GL_FLOAT, GL_FALSE, 0, box);
                            //glDrawArrays(GL_TRIANGLES,0,6); //6 vertices for quad
                        }

                    }
                #endif //BLENDED
                }
            case X3D_COMPOSED_VOLUME_STYLE:
                {
                    int i;
                    // http://www.web3d.org/documents/specifications/19775-1/V3.3/Part01/components/volume.html#ComposedVolumeStyle
                    NodeComposedVolumeStyle *style = (NodeComposedVolumeStyle*)vstyle;
                    for(i=0;i<style->renderStyle()->getSize();i++){
                        fin_volumestyle(style->renderStyle()->getValue(i),dataParent);
                    }
                }
                break;

        default:
            break;
        }
    }
}
int volstyle_needs_normal(Node *vstyle){
    //IDEA: compute image gradient and store in RGB, if a style requests it
    // then surfaceNormal = normalize(gradient)
    //SFNode [in,out] surfaceNormals NULL [X3DTexture3DNode
    // Cartoon
    // Edge
    // Shaded
    // SilhouetteEnhancement
    // ToneMappedVolumeStyle
    //
    //SFNode [in,out] gradients NULL [X3DTexture3DNode]
    // IsoSurfaceVolumeData
    //
    //SFNode [in,out] segmentIdentifiers NULL [X3DTexture3DNode]
    // SegmentedVolumeData
    int need_normal;
    NodeOpacityMapVolumeStyle *style0 = (NodeOpacityMapVolumeStyle*)vstyle;
    need_normal = FALSE;
    if(style0->enabled()->getValue()){
        switch(vstyle->getType()){
            case X3D_COMPOSED_VOLUME_STYLE:
                {
                    int i;
                    NodeComposedVolumeStyle *style = (NodeComposedVolumeStyle*)vstyle;
                    for(i=0;i<style->renderStyle()->getSize();i++){
                        need_normal = need_normal || volstyle_needs_normal(style->renderStyle()->getValue(i));
                    }
                }
                break;
            case X3D_CARTOON_VOLUME_STYLE:
            case X3D_EDGE_ENHANCEMENT_VOLUME_STYLE:
            case X3D_SHADED_VOLUME_STYLE:
            case X3D_SILHOUETTE_ENHANCEMENT_VOLUME_STYLE:
            case X3D_TONE_MAPPED_VOLUME_STYLE:
                {
                    //in perl structs, for these nodes its all the 3rd field after enabled, metadata, surfacenormals
                    NodeToneMappedVolumeStyle *style = (NodeToneMappedVolumeStyle*)vstyle;
                    need_normal = need_normal || (style->surfaceNormals()->getValue() == NULL);
                }
                break;
            default:
                break;
        }
    }
    return need_normal;
}

void compile_IsoSurfaceVolumeData(NodeIsoSurfaceVolumeData *node){
    // http://www.web3d.org/documents/specifications/19775-1/V3.3/Part01/components/volume.html#IsoSurfaceVolumeData
    // VolumeData + 4 fields:
    //SFFloat [in,out] contourStepSize  0        (-INF,INF)
    //SFNode  [in,out] gradients        NULL     [X3DTexture3DNode]
    //SFFloat [in,out] surfaceTolerance 0        [0,INF)
    //MFFloat [in,out] surfaceValues    []       (-INF,INF)
    printf("compile_isosurfacevolumedata not implemented\n");
    compile_VolumeData((NodeVolumeData *)node);
}

//unsigned int getShaderFlags(){
shaderflagsstruct getShaderFlags(){
    //return top-of-stack global shaderflags
    //unsigned int retval;
    shaderflagsstruct retval;
//    ttglobal tg = gglobal();
    ppRenderFuncs p = (ppRenderFuncs)renderFuncs_prv;
    //retval = stack_top(unsigned int,p->shaderflags_stack);
    retval = stack_top(shaderflagsstruct,p->shaderflags_stack);
    return retval;
}

void compile_SegmentedVolumeData(NodeSegmentedVolumeData *node){
    // http://www.web3d.org/documents/specifications/19775-1/V3.3/Part01/components/volume.html#SegmentedVolumeData
    // VolumeData + 2 fields:
    //MFBool  [in,out] segmentEnabled     []
    //SFNode  [in,out] segmentIdentifiers NULL     [X3DTexture3DNode]
    printf("compile_segmentedvolumedata \n");
    compile_VolumeData((NodeVolumeData *)node);
}

s_shader_capabilities_t * getVolumeProgram(Node **renderStyle, int nstyle, int VOLUME_DATA_FLAG){
    static int once = 0;
    unsigned int volflags;
    int i;
    s_shader_capabilities_t *caps;

    if(!once)
        ConsoleMessage("getVolumeProgram\n");
    volflags = 0;
    if(nstyle){
        for(i=0;i<nstyle;i++){
            NodeOpacityMapVolumeStyle *style0 = (NodeOpacityMapVolumeStyle*)renderStyle[i];
            if(style0->enabled()->getValue()){
                volflags = prep_volumestyle(renderStyle[i], volflags); //get shader flags
            }
        }
    }else{
        volflags = SHADERFLAGS_VOLUME_STYLE_DEFAULT;
    }

    if(!once){
        printf("volflags= ");
        for(i=0;i<8;i++)
            printf("%d ",((volflags >> (8-i-1)*4) & 0xF)); //show 4 int
        printf("\n");
    }

    //render 
    //Step 1: set the 3D texture
    //if(node->voxels)
    //    render_node(node->voxels);
    //Step 2: get rays to cast: start point and direction vector for each ray to cast

    //method: use cpu math to compute a few uniforms so frag shader can do box intersections
    //http://prideout.net/blog/?p=64
    //- one step raycasting using gl_fragCoord
    //- we modified this general method to use gluUnproject math instead of focallength

    //Step 3: accumulate along rays and render opacity fragment in one step
    //GPU VERSION
    {
        shaderflagsstruct shader_requirements; //shaderflags, 
        // OLDCODE GLint myProg;

        memset(&shader_requirements,0,sizeof(shaderflagsstruct));
        //shaderflags = getShaderFlags();
        shader_requirements.volume = VOLUME_DATA_FLAG; //SHADERFLAGS_VOLUME_DATA_BASIC; //send the following through the volume ubershader
        shader_requirements.volume |= (volflags << 4); //SHADERFLAGS_VOLUME_STYLE_OPACITY;
        //CLIPPLANES ?
        shader_requirements.base |= getShaderFlags().base & CLIPPLANE_SHADER; 
        // by default we'll mash it in: shader_requirements.volume |= TEX3D_SHADER;
        caps = getMyShaders(shader_requirements);
        enableGlobalShader(caps);
        // OLDCODE myProg =  caps->myShaderProgram;
    }
    //Step 1: set the 3D texture
    once = 1;
    //return myProg;
    return caps; 
}



void render_node(struct Node *node) {
    struct X3D_Virt *virt;

    //int srg = 0;
    //int sch = 0;
    int justGeom = 0;
    int pushed_ray;
    int pushed_sensor;
    //struct currayhit *srh = NULL;
    ppRenderFuncs p;
//    ttglobal tg = gglobal();
    p = (ppRenderFuncs)renderFuncs_prv;

    X3D_NODE_CHECK(node);
//#define RENDERVERBOSE 1
#ifdef RENDERVERBOSE
    p->renderLevel ++;
#endif

    if(!node) {
#ifdef RENDERVERBOSE
        DEBUG_RENDER("%d no node, quick return\n", renderLevel);
        p->renderLevel--;
#endif
        return;
    }
//    virt = virtTable[node->getType()];

#ifdef RENDERVERBOSE 
    //printf("%d =========================================NODE RENDERED===================================================\n",renderLevel);
    {
        int i;
        for(i=0;i<p->renderLevel;i++) printf(" ");
    }
    printf("%d node %u (%s) , v %u renderFlags %x ",p->renderLevel, node,stringNodeType(node->_nodeType),virt,node->_renderFlags);

/*
    if ((node->_renderFlags & VF_Viewpoint) == VF_Viewpoint) printf (" VF_Viewpoint");
    if ((node->_renderFlags & VF_Geom )== VF_Geom) printf (" VF_Geom");
    if ((node->_renderFlags & VF_localLight )== VF_localLight) printf (" VF_localLight");
    if ((node->_renderFlags & VF_Sensitive) == VF_Sensitive) printf (" VF_Sensitive");
    if ((node->_renderFlags & VF_Blend) == VF_Blend) printf (" VF_Blend");
    if ((node->_renderFlags & VF_Proximity) == VF_Proximity) printf (" VF_Proximity");
    if ((node->_renderFlags & VF_Collision) == VF_Collision) printf (" VF_Collision");
    if ((node->_renderFlags & VF_globalLight) == VF_globalLight) printf (" VF_globalLight");
    if ((node->_renderFlags & VF_hasVisibleChildren) == VF_hasVisibleChildren) printf (" VF_hasVisibleChildren");
    if ((node->_renderFlags & VF_shouldSortChildren) == VF_shouldSortChildren) printf (" VF_shouldSortChildren");
    if ((node->_renderFlags & VF_Other) == VF_Other) printf (" VF_Other");
*/
    /*
    if ((node->_renderFlags & VF_inPickableGroup == VF_inPickableGroup) printf (" VF_inPickableGroup");
    if ((node->_renderFlags & VF_PickingSensor == VF_PickingSensor) printf (" VF_PickingSensor");
    */
    printf ("\n");

    //printf("PREP: %d REND: %d CH: %d FIN: %d RAY: %d HYP: %d\n",virt->prep, virt->rend, virt->children, virt->fin,
    //       virt->rendray, hypersensitive);
    //printf("%d state: vp %d geom %d light %d sens %d blend %d prox %d col %d ", renderLevel, 
 //            render_vp,render_geom,render_light,render_sensitive,render_blend,render_proximity,render_collision); 
    //printf("change %d ichange %d \n",node->_change, node->_ichange);
#endif



    // leaf-node filtering (we still do the transform-children stack)
    // if we are doing Viewpoints, and we don't have a Viewpoint, don't bother doing anything here *
    //if (renderstate()->render_vp == VF_Viewpoint) { 
/*
    if (p->renderstate.render_vp == VF_Viewpoint) { 
        //if(tg->Bindable.activeLayer == 0)  //no Layerset nodes
        if ((node->_renderFlags & VF_Viewpoint) != VF_Viewpoint) { 
            #ifdef RENDERVERBOSE
            printf ("doing Viewpoint, but this  node is not for us - just returning\n"); 
            p->renderLevel--;
            #endif
            return; 
        } 
        if(p->renderstate.render_vp == VF_Viewpoint && render_foundLayerViewpoint()){ 
            //on vp pass, just find first DEF/USE of bound viewpoint
            return;
        }
    }
*/
#if 0
    /* are we working through global PointLights, DirectionalLights or SpotLights, but none exist from here on down? */
    if (p->renderstate.render_light ) { 
        if((node->_renderFlags & VF_globalLight) != VF_globalLight) { 
    #ifdef RENDERVERBOSE
            printf ("doing globalLight, but this  node is not for us - just returning\n"); 
            p->renderLevel--;
    #endif
            return; 
        }
    }
    justGeom = p->renderstate.render_geom && !p->renderstate.render_sensitive && !p->renderstate.render_blend;
    pushed_ray = FALSE;
    pushed_sensor = FALSE;

    if(virt->prep) {
        //transform types will pushmatrix and multiply in their translation.rotation,scale here (and popmatrix in virt->fin)
        DEBUG_RENDER("rs 2\n");
        profile_start("prep");
        if(justGeom)
            profile_start("prepgeom");
        virt->prep(node);  
        profile_end("prep");
        if(justGeom)
            profile_end("prepgeom");
        //if(p->renderstate.render_sensitive && !tg->RenderFuncs.hypersensitive) {
        //    push_ray(); //upd_ray(); 
        //    pushed_ray = TRUE;
        //}
        PRINT_GL_ERROR_IF_ANY("prep"); PRINT_NODE(node,virt);
        if(p->renderstate.render_boxes) extent6f_draw(node->_extent);

    }
    if(p->renderstate.render_sensitive && !tg->RenderFuncs.hypersensitive) {
        push_ray(); //upd_ray(); 
        pushed_ray = TRUE;
    }
    if(p->renderstate.render_proximity && virt->proximity) {
        DEBUG_RENDER("rs 2a\n");
        profile_start("proximity");
        virt->proximity(node);
        profile_end("proximity");
        PRINT_GL_ERROR_IF_ANY("render_proximity"); PRINT_NODE(node,virt);
    }
    if(p->renderstate.render_geom && ((node->_renderFlags & VF_USE) == VF_USE) && !p->renderstate.render_picking){
        //picking sensor, transform sensor and generally any USE_NODE-USE_NODE scenario
        //ideally we would come in here once per scenegraph USE per frame, even when stereo or quad views
        //because we want to work in world coordinates (not view coordinates) so by the time
        //we strip off the view matrix we would have duplicate entries with stereo
        if(getWindex() == 0){
            //just on first window of stereo or quad
            //we don't do this in VF_Proximity because that pass doesn't go all the way to all geom nodes
            //we could give it its own pass, but doing just the first window is a simple hack.
            double modelviewMatrix[16];
            //IF VIEW == 0
            //GL_GET_MODELVIEWMATRIX
            FW_GL_GETDOUBLEV(GL_MODELVIEW_MATRIX, modelviewMatrix);
            //strip viewmatrix - will happen when we invert one of the USEUSE pair, and multiply
            usehit_add2(node,modelviewMatrix,getpickablegroupdata());
        }
    }
    if(p->renderstate.render_picking && node->_nodeType == NODE_Shape ){
        //this is for when called from Component_Picking.c on a partial scenegraph,
        //to get geometry nodes in the usehitB list
        //I put vrit->rendray as a way to detect if its geometry, is there a better way?
        double modelviewMatrix[16];
        struct X3D_Shape *shapenode = (struct X3D_Shape*)node;
        if(shapenode->geometry){
            //IF VIEW == 0
            //GL_GET_MODELVIEWMATRIX
            FW_GL_GETDOUBLEV(GL_MODELVIEW_MATRIX, modelviewMatrix);
            //strip viewmatrix - will happen when we invert one of the USEUSE pair, and multiply
            usehitB_add2(shapenode->geometry,modelviewMatrix,getpickablegroupdata());
        }
    }
    
    if(p->renderstate.render_collision && virt->collision) {
        DEBUG_RENDER("rs 2b\n");
        profile_start("collision");
        virt->collision(node);
        profile_end("collision");
        PRINT_GL_ERROR_IF_ANY("render_collision"); PRINT_NODE(node,virt);
    }

    if(p->renderstate.render_geom && !p->renderstate.render_sensitive && !p->renderstate.render_picking && virt->rend) {
            DEBUG_RENDER("rs 3\n");
            PRINT_GL_ERROR_IF_ANY("BEFORE render_geom"); PRINT_NODE(node,virt);
            profile_start("rend");
            virt->rend(node);
            profile_end("rend");
            PRINT_GL_ERROR_IF_ANY("render_geom"); PRINT_NODE(node,virt);
    }
    if(p->renderstate.render_other && virt->other )
    {
        virt->other(node);
    } //other

    if(p->renderstate.render_sensitive && ((node->_renderFlags & VF_Sensitive)|| Viewer()->LookatMode ==2)) {
        DEBUG_RENDER("rs 5\n");
        profile_start("sensitive");
        push_sensor(node);
        pushed_sensor = TRUE;
        profile_end("sensitive");
    }
    if(p->renderstate.render_geom && p->renderstate.render_sensitive && !tg->RenderFuncs.hypersensitive && virt->rendray) {
        DEBUG_RENDER("rs 6\n");
        profile_start("rendray");
        if(pickrayHitsMBB(node))
            virt->rendray(node);
        profile_end("rendray");
        PRINT_GL_ERROR_IF_ANY("rs 6"); PRINT_NODE(node,virt);
    }

    /* May 16 2016: now we don't come into render_hier on hypersensitive
    if((p->renderstate.render_sensitive) && (tg->RenderFuncs.hypersensitive == node)) {
        DEBUG_RENDER("rs 7\n");
        p->hyper_r1 = p->t_r123.p1; //tg->RenderFuncs.t_r1;
        p->hyper_r2 = p->t_r123.p2; //tg->RenderFuncs.t_r2;
        tg->RenderFuncs.hyperhit = 1;
    }
    */

    /* start recursive section */
    if(virt->children) { 
        DEBUG_RENDER("rs 8 - has valid child node pointer\n");
        //if(! (p->renderstate.render_vp == VF_Viewpoint && render_foundLayerViewpoint())){ //on vp pass, just find first DEF/USE of bound viewpoint
            //printf("children ");
            virt->children(node);
        //}
        //}else{
        //    printf("skipping ");
        //}
        PRINT_GL_ERROR_IF_ANY("children"); PRINT_NODE(node,virt);
    }
    /* end recursive section */

    if(p->renderstate.render_other && virt->other)
    {
    }

    if(pushed_sensor)
        pop_sensor();

    if(virt->fin) {
        DEBUG_RENDER("rs A\n");
        profile_start("fin");
        if(justGeom)
            profile_start("fingeom");

        virt->fin(node);
        profile_end("fin");
        if(justGeom)
            profile_end("fingeom");
        //if(p->renderstate.render_sensitive && virt == &virt_Transform) {
        //    upd_ray();
        //}
        PRINT_GL_ERROR_IF_ANY("fin"); PRINT_NODE(node,virt);
    }
    if(pushed_ray)
        pop_ray();

#ifdef RENDERVERBOSE 
    {
        int i;
        for(i=0;i<p->renderLevel;i++)printf(" ");
    }
    printf("%d (end render_node)\n",p->renderLevel);
    p->renderLevel--;
#endif
#endif
}
//#undef RENDERVERBOSE


void render_SEGMENTED_volume_data(s_shader_capabilities_t *caps, Node *segmentIDs, int itexture, NodeSegmentedVolumeData *node) {
    int myProg;
    GLint inids, ienable;

    /*int*/
    const bool *enabledIDsb = node->segmentEnabled()->getValues();
    int nIDs = node->segmentEnabled()->getSize();
    int *enabledIDs = (int *)malloc(nIDs * sizeof(int));
    for (int i = 0; i < nIDs; i++)
        enabledIDs[i] = enabledIDsb[i];
    myProg = caps->myShaderProgram;
    if(segmentIDs){
        Node *tmpN;
        textureTableIndexStruct_s *tti;
//        ttglobal tg = gglobal();

        POSSIBLE_PROTO_EXPANSION(Node *, segmentIDs,tmpN);
        renderFuncs_texturenode = (Node*)tmpN;

        render_node(tmpN); //render_node(node->voxels); 

        tti = getTableTableFromTextureNode(tmpN);
        if(tti && tti->status >= TEX_LOADED){
            GLint TextureUnit;
            if(0){
                //in theory these will be set by the main voxel texture but don't match
                //here we want NEAREST not LINEAR
                GLint tex3dUseVertex, ttiles, repeatSTR, magFilter;
                ttiles = GET_UNIFORM(myProg,"tex3dTiles");
                GLUNIFORM1IV(ttiles,3,tti->tiles);

                //me->tex3dUseVertex = GET_UNIFORM(myProg,"tex3dUseVertex");
                tex3dUseVertex = GET_UNIFORM(myProg,"tex3dUseVertex");
                glUniform1i(tex3dUseVertex,0); 
                repeatSTR = GET_UNIFORM(myProg,"repeatSTR");
                glUniform1iv(repeatSTR,3,tti->repeatSTR);
                magFilter = GET_UNIFORM(myProg,"magFilter");
                glUniform1i(magFilter,0); //tti->magFilter); //NEAREST
            }
            TextureUnit= GET_UNIFORM(myProg,"fw_Texture_unit1");
            glUniform1i(TextureUnit,itexture);
            glActiveTexture(GL_TEXTURE0+itexture); 
            glBindTexture(GL_TEXTURE_2D,tti->OpenGLTexture); 
            FW_GL_TEXPARAMETERI( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);  //don't interpolate integer segment IDs
        }
    }
    inids = GET_UNIFORM(myProg,"fw_nIDs");
    glUniform1i(inids,nIDs); 
    ienable = GET_UNIFORM(myProg,"fw_enableIDs");
    glUniform1iv(ienable,nIDs,enabledIDs); 

    //similar to ISO, there are multiple rendering styles
    if(node->renderStyle()->getSize()){
        int i, *styleflags,instyles;
        GLint istyles;

        styleflags = MALLOC(int*,sizeof(int)*node->renderStyle()->getSize());
        for(i=0;i<node->renderStyle()->getSize();i++){
            styleflags[i] = prep_volumestyle(node->renderStyle()->getValue(i),0);
        }
        //printf("%d %d\n",styleflags[0],styleflags[1]);
        istyles = GET_UNIFORM(myProg,"fw_surfaceStyles");
        glUniform1iv(istyles,node->renderStyle()->getSize(),styleflags);
        instyles = GET_UNIFORM(myProg,"fw_nStyles");
        glUniform1i(instyles,node->renderStyle()->getSize());
    }

    free(enabledIDs);

}

ttrenderstate renderstate()
{
    ppRenderFuncs p = renderFuncs_prv;
    return &p->renderstate;
}

GLDOUBLE det3x3(GLDOUBLE* data)
{
    return -data[1]*data[10]*data[4] +data[0]*data[10]*data[5] -data[2]*data[5]*data[8] +data[1]*data[6]*data[8] +data[2]*data[4]*data[9] -data[0]*data[6]*data[9];
}

GLDOUBLE* matinverse98(GLDOUBLE* res, GLDOUBLE* mm)
{
    /*FLOPs 98 double: det3 9, 1/det 1, adj3x3 9x4=36, inv*T 13x4=52 */
    //July 2016 THIS IS WRONG DON'T USE 
    //see the glu equivalent elsewhere
    //you can check with A*A-1 = I and this function doesn't give I
    double Deta;
    GLDOUBLE mcpy[16];
    GLDOUBLE *m;

    m = mm;
    if(res == m) {
    memcpy(mcpy,m,sizeof(GLDOUBLE)*16);
    m = mcpy;
    }

    Deta = det3x3(m);
    if(APPROX(Deta,0.0))
        printf("deta 0\n");
    Deta = 1.0 / Deta;

    res[0] = (-m[9]*m[6] +m[5]*m[10])*Deta;
    res[4] = (+m[8]*m[6] -m[4]*m[10])*Deta;
    res[8] = (-m[8]*m[5] +m[4]*m[9])*Deta;
    res[12] = ( m[12]*m[9]*m[6] -m[8]*m[13]*m[6] -m[12]*m[5]*m[10] +m[4]*m[13]*m[10] +m[8]*m[5]*m[14] -m[4]*m[9]*m[14])*Deta;

    res[1] = (+m[9]*m[2] -m[1]*m[10])*Deta;
    res[5] = (-m[8]*m[2] +m[0]*m[10])*Deta;
    res[9] = (+m[8]*m[1] -m[0]*m[9])*Deta;
    res[13] = (-m[12]*m[9]*m[2] +m[8]*m[13]*m[2] +m[12]*m[1]*m[10] -m[0]*m[13]*m[10] -m[8]*m[1]*m[14] +m[0]*m[9]*m[14])*Deta;

    res[2] = (-m[5]*m[2] +m[1]*m[6])*Deta;
    res[6] = (+m[4]*m[2] -m[0]*m[6])*Deta;
    res[10] = (-m[4]*m[1] +m[0]*m[5])*Deta;
    res[14] = ( m[12]*m[5]*m[2] -m[4]*m[13]*m[2] -m[12]*m[1]*m[6] +m[0]*m[13]*m[6] +m[4]*m[1]*m[14] -m[0]*m[5]*m[14])*Deta;

    res[3] = (+m[5]*m[2]*m[11] -m[1]*m[6]*m[11])*Deta;
    res[7] = (-m[4]*m[2]*m[11] +m[0]*m[6]*m[11])*Deta;
    res[11] = (+m[4]*m[1]*m[11] -m[0]*m[5]*m[11])*Deta;
    res[15] = (-m[8]*m[5]*m[2] +m[4]*m[9]*m[2] +m[8]*m[1]*m[6] -m[0]*m[9]*m[6] -m[4]*m[1]*m[10] +m[0]*m[5]*m[10])*Deta;

    return res;
}

GLDOUBLE* matinverseFULL(GLDOUBLE* res, GLDOUBLE* mm){
        matinverse98(res,mm);
        return res;
}

float *matdouble2float4(float *rmat4, double *dmat4){
    int i;
    /* convert GLDOUBLE to float */
    for (i=0; i<16; i++) {
        rmat4[i] = (float)dmat4[i];
    }
    return rmat4;
}

GLDOUBLE* matmultiplyFULL(GLDOUBLE* r, GLDOUBLE* mm , GLDOUBLE* nn)
{
    /* full 4x4, will do perspectives 
    FLOPs 64 double: 4x4x4 
    r = mm x nn
    */
    GLDOUBLE tm[16],tn[16];
    GLDOUBLE *m, *n;
    int i,j,k;
    /* prevent self-multiplication problems.*/
    m = mm;
    n = nn;
    if(r == m) {
    memcpy(tm,m,sizeof(GLDOUBLE)*16);
    m = tm;
    }
    if(r == n) {
    memcpy(tn,n,sizeof(GLDOUBLE)*16);
    n = tn;
    }
    if(0){
        if(1) for(i=0;i<3;i++){
            if(mm[i +12] != 0.0){
                double p = mm[i +12];
                printf("Ft[%d]%f ",i,p);
            }
            if(nn[i + 12] != 0.0){
                double p = nn[i +12];
                printf("FT[%d]%f ",i,p);
            }
        }
        if(1) for(i=0;i<3;i++){
            if(mm[i*4 + 3] != 0.0){
                double p = mm[i*4 + 3];
                printf("Fp[%d]%f ",i,p);
            }
            if(nn[i*4 + 3] != 0.0){
                double p = nn[i*4 + 3];
                printf("FP[%d]%f ",i,p);
            }
        }
    }
    /* assume 4x4 homgenous transform */
    for(i=0;i<4;i++)
        for(j=0;j<4;j++)
        {
            r[i*4+j] = 0.0;
            for(k=0;k<4;k++)
                r[i*4+j] += m[i*4+k]*n[k*4+j];
        }
    return r;
}

/* Generic GLSL fragment shader, used on OpenGL ES. */
/* DEFINES */ 
#ifdef MOBILE 
//precision highp float; 
precision mediump float; 
#endif //MOBILE 

class vec2 {
public:
    float x;
    float y;
    vec2(void) {
        x = 0;
        y = 0;
    }
};
 
class vec3 {
public:
    float x;
    float y;
    float z;
    vec3(void) {
        x = 0;
        y = 0;
        z = 0;
    }
    vec3(float fx, float fy, float fz) {
        x = fx;
        y = fy;
        z = fz;
    }
    vec3 operator*(vec3 &arg) {
        vec3 ret;
        ret.x = x * arg.x;
        ret.y = y * arg.y;
        ret.z = z * arg.z;
        return ret;
    }
    vec3 operator+(const float &arg) {
        vec3 ret;
        ret.x = x + arg;
        ret.y = y + arg;
        ret.z = z + arg;
        return ret;
    }
    vec3 operator-(const float &arg) {
        vec3 ret;
        ret.x = x - arg;
        ret.y = y - arg;
        ret.z = z - arg;
        return ret;
    }
    vec3 operator*(const float &arg) {
        vec3 ret;
        ret.x = x * arg;
        ret.y = y * arg;
        ret.z = z * arg;
        return ret;
    }
    bool operator<(const vec3 &arg) {
        float len = sqrt(x * x + y * y + z * z);
        float lenarg = sqrt(arg.x * arg.x + arg.y * arg.y + arg.z * arg.z);
        if (len < lenarg)
            return true;
        return false;
    }
    vec3 operator/(const float &arg) {
        vec3 ret;
        ret.x = x / arg;
        ret.y = y / arg;
        ret.z = z / arg;
        return ret;
    }
    vec3 operator-()
    { 
        return vec3(-x, -y, -z); 
    }
    vec3 operator-(const vec3 &arg) {
        vec3 ret;
        ret.x = x - arg.x;
        ret.y = y - arg.y;
        ret.z = z - arg.z;
        return ret;
    }
    vec3 operator+(const vec3 &arg) {
        vec3 ret;
        ret.x = x + arg.x;
        ret.y = y + arg.y;
        ret.z = z + arg.z;
        return ret;
    }
    vec3 operator*(const vec3 &arg) {
        vec3 ret;
        ret.x = x * arg.x;
        ret.y = y * arg.y;
        ret.z = z * arg.z;
        return ret;
    }
    vec3 operator/(const vec3 &arg) {
        vec3 ret;
        ret.x = x / arg.x;
        ret.y = y / arg.y;
        ret.z = z / arg.z;
        return ret;
    }
    vec3 divrev(const float &arg) {
        vec3 ret;
        ret.x = arg / x;
        ret.y = arg / y;
        ret.z = arg / z;
        return ret;
    }
};

vec3 floor(vec3 &arg) {
    vec3 ret;
    ret.x = floor(arg.x);
    ret.y = floor(arg.y);
    ret.z = floor(arg.z);
    return ret;
}

float fract(float arg)
{
   return arg - floor(arg);
}

vec3 fract(vec3 arg)
{
   vec3 ret;
   ret.x = arg.x - floor(arg.x);
   ret.y = arg.y - floor(arg.y);
   ret.z = arg.z - floor(arg.z);
   return ret;
}

float length(vec3 arg)
{
    return sqrt(arg.x * arg.x + arg.y * arg.y + arg.z * arg.z);
}

float distance(vec3 p0, vec3 p1)
{
   return length(p0 - p1);
}

vec3 normalize(vec3 arg) 
{
    vec3 ret = arg;
    float fx = arg.x,
          fy = arg.y,
          fz = arg.z;
    float len = fx*fx + fy*fy + fz*fz;
    if (len > 0) {
        len = 1.0 / sqrt(len);
        ret.x = arg.x * len;
        ret.y = arg.y * len;
        ret.z = arg.z * len;
    }
    return ret;
}

class vec4 {
public:
    float x;
    float y;
    float z;
    float a;
    vec4(void) {
        x = 0;
        y = 0;
        z = 0;
        a = 0;
    }    
    vec4(float fx, float fy, float fz, float fa) {
        x = fx;
        y = fy;
        z = fz;
        a = fa;
    }
    vec4 operator/(const float &arg) {
        vec4 ret;
        ret.x = x / arg;
        ret.y = y / arg;
        ret.z = z / arg;
        ret.a = a / arg;
        return ret;
    }
    vec4 normalize(vec4 arg) 
    {
        vec4 ret = arg;
        float fx = arg.x,
              fy = arg.y,
              fz = arg.z,
              fa = arg.a;
        float len = fx*fx + fy*fy + fz*fz + fa*fa;
        if (len > 0) {
            len = 1.0 / sqrt(len);
            ret.x = arg.x * len;
            ret.y = arg.y * len;
            ret.z = arg.z * len;
            ret.a = arg.a * len;
        }
        return ret;
    }
};

float mix(float arg1, float arg2, float arg3)
{
     return arg1 * (1 - arg3) + arg2 * arg3;
}

vec4 mix(vec4 arg1, vec4 arg2, float arg3) 
{
    vec4 ret;
    ret.x = arg1.x * (1 - arg3) + arg2.x * arg3;
    ret.y = arg1.y * (1 - arg3) + arg2.y * arg3;
    ret.z = arg1.z * (1 - arg3) + arg2.z * arg3;
    return ret;
}

float clamp(float arg, float minval, float maxval)
{
    return min(max(arg, minval), maxval);
}

class mat4 {
public:
    float data[16];
    mat4()
    {
        data[0] = 1;
        data[1] = 0;
        data[2] = 0;
        data[3] = 0;
    
        data[4] = 0;
        data[5] = 1;
        data[6] = 0;
        data[7] = 0;
    
        data[8] = 0;
        data[9] = 0;
        data[10] = 1;
        data[11] = 0;
    
        data[12] = 0;
        data[13] = 0;
        data[14] = 0;
        data[15] = 1;
    }
    vec4 operator*(const vec4 &arg) 
    {
        vec4 ret;
        ret.x = data[0]  * arg.x + data[1]  * arg.y + data[2]  * arg.z + data[3]  * arg.a;
        ret.y = data[4]  * arg.x + data[5]  * arg.y + data[6]  * arg.z + data[7]  * arg.a;
        ret.z = data[8]  * arg.x + data[9]  * arg.y + data[10] * arg.z + data[11] * arg.a;
        ret.a = data[12] * arg.x + data[13] * arg.y + data[14] * arg.z + data[15] * arg.a  ;
        return ret;
    }
};

vec4 HeatMapColor(float value, float minValue, float maxValue) 
{ 
    //used for debugging. If min=0,max=1 then magenta is 0, blue,green,yellow, red is 1 
    vec4 ret; 
    int HEATMAP_COLORS_COUNT; 
    vec4 colors[6]; 
    HEATMAP_COLORS_COUNT = 6; 
    colors[0] = vec4(0.32, 0.00, 0.32, 1.0); 
    colors[1] = vec4( 0.00, 0.00, 1.00, 1.00); 
    colors[2] = vec4(0.00, 1.00, 0.00, 1.00); 
    colors[3] = vec4(1.00, 1.00, 0.00, 1.00); 
    colors[4] = vec4(1.00, 0.60, 0.00, 1.00); 
    colors[5] = vec4(1.00, 0.00, 0.00, 1.00); 
    float ratio=(float(HEATMAP_COLORS_COUNT)-1.0)*clamp((value-minValue)/(maxValue-minValue),0.0,1.0); 
    int indexMin=int(floor(ratio)); 
    int indexMax= indexMin+1 < HEATMAP_COLORS_COUNT-1 ? indexMin+1 : HEATMAP_COLORS_COUNT-1; 
    ret = mix(colors[indexMin], colors[indexMax], ratio-float(indexMin)); 
    if(value < minValue) ret = vec4(0.0,0.0,0.0,1.0); 
    if(value > maxValue) ret = vec4(1.0,1.0,1.0,1.0); 
    return ret; 
} 
vec4 debug_color; 
float hash(float n) 
{ 
    return fract(sin(n)*43758.5453); 
} 
float noise( vec3 xyz ) 
{ 
    // The noise function returns a value in the range -1.0f -> 1.0f 
    vec3 p = floor(xyz); 
    vec3 f = fract(xyz); 
    vec3 a = -f*2.0f;
    vec3 b = a+3.0f;     
    f = f*f*b;
    float n = p.x + p.y*57.0 + 113.0*p.z; 
    
    return mix(mix(mix( hash(n+0.0), hash(n+1.0),f.x), 
                   mix( hash(n+57.0), hash(n+58.0),f.x),f.y), 
               mix(mix( hash(n+113.0), hash(n+114.0),f.x), 
                   mix( hash(n+170.0), hash(n+171.0),f.x),f.y),f.z); 
} 
vec3 noise3(vec3 xyz, float range ){ 
    vec3 rxyz = vec3(xyz); 
    rxyz.x += noise(xyz)*range; 
    rxyz.y += noise(xyz)*range; 
    rxyz.z += noise(xyz)*range; 
    return rxyz; 
} 

#define uniform 
class sampler2D
{
};

vec4 gl_FragCoord;
/*varying*/ vec4 castle_vertex_eye; 
/*varying*/ vec4 castle_Color; 
uniform mat4 fw_ModelViewProjInverse; 
//uniform float fw_FocalLength; 
uniform vec4 fw_viewport; 
uniform vec3 fw_dimensions; 
//uniform vec3 fw_RayOrigin; 
uniform sampler2D fw_Texture_unit0; 
uniform sampler2D fw_Texture_unit1; 
uniform sampler2D fw_Texture_unit2; 
uniform sampler2D fw_Texture_unit3; 
#ifdef TEX3D 
uniform int tex3dTiles[3]; 
uniform int repeatSTR[3]; 
uniform int magFilter; 
#endif //TEX3D 
#ifdef SEGMENT 
uniform int fw_nIDs; 
uniform int fw_enableIDs[10]; 
uniform int fw_surfaceStyles[2]; 
uniform int fw_nStyles; 
vec4 texture3Demu( sampler2D sampler, in vec3 texcoord3); 
vec4 texture3Demu0( sampler2D sampler, in vec3 texcoord3, int magfilter); 
bool inEnabledSegment(in vec3 texcoords, inout int jstyle){ 
    bool inside = true; 
    jstyle = 1; //DEFAULT 
    vec4 segel = texture3Demu0(fw_Texture_unit1,texcoords,0); 
    //convert from GL_FLOAT 0-1 to int 0-255 
    //Q. is there a way to do int images in GLES2? 
    int ID = int(floor(segel.a * 255.0 + .1)); 
    //debug_color = HeatMapColor(float(ID),0.0,5.0); 
    //debug_color.a = .2; 
    if(ID < fw_nIDs){ 
        //specs: The indices of this array corresponds to the segment identifier. 
        inside = fw_enableIDs[ID] == 0 ? false : true; 
    } 
    if(inside){ 
        int kstyle = fw_nStyles-1; 
        kstyle = ID < fw_nStyles ? ID : kstyle; 
        jstyle = fw_surfaceStyles[kstyle]; 
        jstyle = jstyle == 1 ? 0 : jstyle; 
    } 
    return inside; 
} 
#endif //SEGMENT 
#ifdef ISO 
uniform float fw_stepSize; 
uniform float fw_tolerance; 
uniform float fw_surfaceVals[]; 
uniform int fw_nVals; 
uniform int fw_surfaceStyles[]; 
uniform int fw_nStyles; 
#endif //ISO 
 
class Ray { 
public:
  vec3 Origin; 
  vec3 Dir; 
  Ray(vec3 o, float *d) 
  {
     Origin = o;
     Dir.x = d[0];
     Dir.y = d[1];
     Dir.z = d[2];
  }
}; 

class AABB { 
public:
  vec3 Min; 
  vec3 Max; 
  AABB(vec3 min, vec3 max)
  {
  Min = min;
  Max = max;
  }
}; 

bool IntersectBox(Ray r, AABB aabb, /*out*/ float t0, /*out*/ float t1) 
{ 
    vec3 invR = r.Dir.divrev(1.0f);
    vec3 tbot = invR * (aabb.Min-r.Origin); 
    vec3 ttop = invR * (aabb.Max-r.Origin); 
    vec3 tmin = min(ttop, tbot); 
    vec3 tmax = max(ttop, tbot); 
    vec2 t;
    t.x = max(tmin.x, tmin.y); 
    t.y = max(tmin.x, tmin.z); 
    t0 = max(t.x, t.y); 
    t.x = min(tmax.x, tmax.y); 
    t.y = min(tmax.x, tmax.z); 
    t1 = min(t.x, t.y); 
    return t0 <= t1; 
} 
/* PLUG-DECLARATIONS */ 

vec4 gl_FragColor; 
vec3 fw_TexCoord[1]; 
#ifdef CLIP 
#define FW_MAXCLIPPLANES 4 
uniform int fw_nclipplanes; 
uniform vec4 fw_clipplanes[FW_MAXCLIPPLANES]; 
bool clip (in vec3 vertex_object){ 
  bool iclip = false; 
  for ( int i=0; i<fw_nclipplanes; i++ ) { 
    if( dot( fw_clipplanes[i], vec4(vertex_object,1.0)) < 0.0) 
        iclip = true; 
  } 
  return iclip; 
} 
#endif //CLIP 
vec3 vertex_eye; 
vec3 normal_eye; 
vec4 raysum; 
void mainShader(void) 
{ 
    debug_color = vec4(0.0, 0.0, 0.0, 0.0); 
    float maxDist = length(fw_dimensions); //1.414214; //sqrt(2.0); 
    int numSamples = 128; 
    float fnumSamples = float(numSamples); 
    float stepSize = maxDist/fnumSamples; 
    float densityFactor = 5.0/fnumSamples; //.88; // 1.0=normal H3D, .5 see deeper  
     
    vec4 fragment_color; 
    //vec4 raysum; 
    float rayDirection[3]; 
    //convert window to frustum 
    rayDirection[0] = 2.0 * (gl_FragCoord.x - fw_viewport.x) / fw_viewport.z - 1.0f; 
    rayDirection[1] = 2.0 * (gl_FragCoord.y - fw_viewport.y) / fw_viewport.a - 1.0f; 
    rayDirection[2] = 0.0; 
    vec3 rayOrigin; // = fw_RayOrigin; 
    //the equivalent of gluUnproject 
    //by unprojecting 2 points on ray here, this should also work with ortho viewpoint 
    vec4 ray4 = vec4(rayDirection[0], rayDirection[1], rayDirection[2], 1.0f);
    vec4 org4 = ray4; 
    //if I back up the ray origin by -1.0 the front plane clipping works properly 
    ray4.z = 0.0; //1.0; 
    org4.z = -1.0; //0.0; 
    ray4 = fw_ModelViewProjInverse * ray4; 
    org4 = fw_ModelViewProjInverse * org4; 
    ray4 = ray4 / ray4.a; 
    org4 = org4 / org4.a; 
    vec3 ray(ray4.x, ray4.y, ray4.z);
    vec3 org(org4.x, org4.y, org4.z);
    vec3 rayDir = normalize(ray - org); 
    rayDirection[0] = rayDir.x;
    rayDirection[1] = rayDir.y;
    rayDirection[2] = rayDir.z;
    rayOrigin.x = org4.x; 
    rayOrigin.y = org4.y; 
    rayOrigin.z = org4.z; 
    
    Ray eye = Ray( rayOrigin, rayDirection); 
    vec3 half_dimensions = fw_dimensions * .5; 
    vec3 minus_half_dimensions = half_dimensions * -1.0; 
    AABB aabb = AABB(minus_half_dimensions,half_dimensions); 
    
    float tnear, tfar; 
    IntersectBox(eye, aabb, tnear, tfar); 
    if (tnear < 0.0) tnear = 0.0; 
    vec3 rayStart = eye.Origin + eye.Dir * tnear; 
    vec3 rayStop = eye.Origin + eye.Dir * tfar; 
    // Perform the ray marching: 
    vec3 pos = rayStart; 
    vec3 step = normalize(rayStop-rayStart) * stepSize; 
    float totaltravel = distance(rayStop, rayStart); 
    float travel = totaltravel; 
    float T = 1.0; 
    vec3 Lo = vec3(0.0f, 0.0f, 0.0f); 
    normal_eye.x = rayDirection[0]; 
    normal_eye.y = rayDirection[1]; 
    normal_eye.z = rayDirection[2]; 
    vec3 pos2 = pos; 
    // Transform from object space to texture coordinate space: 
    pos2 = (pos2+half_dimensions)/fw_dimensions; 
    pos2.x = clamp(pos2.x,0.001,.999); 
    pos2.y = clamp(pos2.y,0.001,.999); 
    pos2.z = clamp(pos2.z,0.001,.999); 
    raysum = vec4(0.0f, 0.0f, 0.0f, 0.0f); 
    float depth = 0.0; 
    float lastdensity; 
    float lastdensity_iso; 
    
    for (int i=0; i < numSamples; ++i) { 
        //raysum = HeatMapColor(travel,0.0,2.0); 
        //break; 
       // ...lighting and absorption stuff here... 
        pos2 = pos; 
        vertex_eye = pos2; 
        // Transform from object space to texture coordinate space: 
        pos2 = (pos2+half_dimensions)/fw_dimensions; 
        //pos2.z = 1.0 - pos2.z; //RHS to LHS 
        pos2.x = clamp(pos2.x,0.001,.999); 
        pos2.y = clamp(pos2.y,0.001,.999); 
        pos2.z = clamp(pos2.z,0.001,.999); 
        vec3 texcoord3 = pos2; 
        bool iclip = false; 
        #ifdef CLIP 
        iclip = clip(vertex_eye); //clip(totaltravel - travel); 
        #endif //CLIP 
        if(!iclip) { 
            fragment_color.x = 1.0; //do I need a default? seems not 
            fragment_color.y = 0.0; //do I need a default? seems not 
            fragment_color.z = 1.0; //do I need a default? seems not 
            fragment_color.a = 1.0; //do I need a default? seems not 
            /* PLUG: texture3D ( fragment_color, texcoord3) */ 
            #ifdef SEGMENT 
            int jstyle = 1; 
            if(inEnabledSegment(texcoord3,jstyle)){ 
            #endif //SEGMENT 
            //assuming we had a scalar input image and put L into .a, 
            // and computed gradient and put in .rgb : 
            float density = fragment_color.a; //recover the scalar value 
            vec3 gradient; //= fragment_color.rgb - vec3(.5,.5,.5); //we added 127 to (-127 to 127) in CPU gradient computation
            gradient.x = fragment_color.x - 0.5; //we added 127 to (-127 to 127) in CPU gradient computation
            gradient.y = fragment_color.y - 0.5; //we added 127 to (-127 to 127) in CPU gradient computation
            gradient.z = fragment_color.z - 0.5; //we added 127 to (-127 to 127) in CPU gradient computation
            //vec4 voxel = vec4(density,density,density,density); //this is where the black visual voxels come from
            vec4 voxel = vec4(density,density,density,density); //this is where the black visual voxels come from
            
            #ifdef ISO 
            if(i==0){ 
                lastdensity = density; 
                lastdensity_iso = 0.0; 
            } 
            int MODE = fw_nVals == 1 ? 1 : 3; 
            MODE = fw_stepSize != 0.0 && MODE == 1 ? 2 : 1; 
            #ifdef ISO_MODE3 
            if(MODE == 3){ 
                for(int i=0;i<fw_nVals;i++){ 
                    float iso = fw_surfaceVals[i]; 
                    if( sign( density - iso) != sign( lastdensity - iso) && length(gradient) > fw_tolerance ){ 
                        voxel.a = 1.0; 
                        int jstyle = min(i,fw_nStyles-1); 
                        jstyle = fw_surfaceStyles[jstyle]; 
                        if(jstyle == 1){ 
                            /* PLUG: voxel_apply_DEFAULT (voxel, gradient) */ 
                        } else if(jstyle == 2) { 
                            /* PLUG: voxel_apply_OPACITY (voxel, gradient) */ 
                        } else if(jstyle == 3) { 
                            /* PLUG: voxel_apply_BLENDED (voxel, gradient) */ 
                        } else if(jstyle == 4) { 
                            /* PLUG: voxel_apply_BOUNDARY (voxel, gradient) */ 
                        } else if(jstyle == 5) { 
                            /* PLUG: voxel_apply_CARTOON (voxel, gradient) */ 
                        } else if(jstyle == 6) { 
                            /* PLUG: voxel_apply_DEFAULT (voxel, gradient) */ 
                        } else if(jstyle == 7) { 
                            /* PLUG: voxel_apply_EDGE (voxel, gradient) */ 
                        } else if(jstyle == 8) { 
                            /* PLUG: voxel_apply_PROJECTION (voxel, gradient) */ 
                        } else if(jstyle == 9) { 
                            /* PLUG: voxel_apply_SHADED (voxel, gradient) */ 
                        } else if(jstyle == 10) { 
                            /* PLUG: voxel_apply_SILHOUETTE (voxel, gradient) */ 
                        } else if(jstyle == 11) { 
                            /* PLUG: voxel_apply_TONE (voxel, gradient) */ 
                        } 
                    } else { 
                        voxel = vec4(0.0f, 0.0f, 0.0f, 0.0f); //similar to discard 
                    } 
                } 
                lastdensity = density; 
            } 
            #else //ISO_MODE3 
            if(MODE == 1){ 
                float iso = fw_surfaceVals[0]; 
                if( sign( density - iso) != sign( lastdensity - iso) && length(gradient) > fw_tolerance ){ 
                    //debug_color = HeatMapColor(iso,0.0,.3); 
                    voxel.a = 1.0; 
                    /* PLUG: voxel_apply (voxel, gradient) */ 
                } else { 
                    voxel = vec4(0.0); //similar to discard 
                } 
                lastdensity = density; 
            } else if(MODE == 2){ 
                float iso = fw_surfaceVals[0]; 
                float density_iso = density / fw_stepSize; 
                if( sign( density_iso - iso) != sign( lastdensity_iso - iso) && length(gradient) > fw_tolerance ){ 
                    voxel.a = 1.0; 
                    /* PLUG: voxel_apply (voxel, gradient) */ 
                } else { 
                    voxel = vec4(0.0); //similar to discard 
                } 
                lastdensity = density; 
                lastdensity_iso = density_iso; 
            }  
            #endif //ISO_MODE3 
            #else //ISO 
            #ifdef SEGMENT 
            //debug_color = HeatMapColor(float(jstyle),1.0,12.0); 
            //debug_color.a = .2; 
            if(jstyle == 1){ 
                /* PLUG: voxel_apply_DEFAULT (voxel, gradient) */ 
            } else if(jstyle == 2) { 
                /* PLUG: voxel_apply_OPACITY (voxel, gradient) */ 
            } else if(jstyle == 3) { 
                /* PLUG: voxel_apply_BLENDED (voxel, gradient) */ 
            } else if(jstyle == 4) { 
                /* PLUG: voxel_apply_BOUNDARY (voxel, gradient) */ 
            } else if(jstyle == 5) { 
                /* PLUG: voxel_apply_CARTOON (voxel, gradient) */ 
            } else if(jstyle == 6) { 
                /* PLUG: voxel_apply_DEFAULT (voxel, gradient) */ 
            } else if(jstyle == 7) { 
                /* PLUG: voxel_apply_EDGE (voxel, gradient) */ 
            } else if(jstyle == 8) { 
                /* PLUG: voxel_apply_PROJECTION (voxel, gradient) */ 
            } else if(jstyle == 9) { 
                /* PLUG: voxel_apply_SHADED (voxel, gradient) */ 
            } else if(jstyle == 10) { 
                /* PLUG: voxel_apply_SILHOUETTE (voxel, gradient) */ 
            } else if(jstyle == 11) { 
                /* PLUG: voxel_apply_TONE (voxel, gradient) */ 
            } 
            #else //SEGMENT 
            //non-iso rendering styles 
            //void PLUG_voxel_apply (inout vec4 voxel, inout vec3 gradient) 
            /* PLUG: voxel_apply (voxel, gradient) */ 
            #endif //SEGMENT 
            #endif //ISO 
            density = voxel.a; 
            //debug_color = HeatMapColor(densityFactor,0.134,.135); 
            T = (1.0 - raysum.a); 
            raysum.a += density * T; 
            raysum.x += voxel.x  * T * density; 
            raysum.y += voxel.y  * T * density; 
            raysum.z += voxel.z  * T * density; 
            if(raysum.a > .99) { 
                break; 
            } 
            #ifdef SEGMENT 
            } //if inEnabledSegment 
            #endif //SEGMENT 
        } //iclip 
        travel -= stepSize; 
        depth += stepSize; 
        if(travel <= 0.0) break; 
        pos = pos + step; 
        
    }  
    //void PLUG_ray_apply (inout vec4 raysum) 
    /* PLUG: ray_apply (raysum) */ 
    if(true) gl_FragColor = raysum; 
    else gl_FragColor = debug_color; 
} 

float *getTransformedClipPlanes();
void sendFogToShader(s_shader_capabilities_t *me);
void render_GENERIC_volume_data(s_shader_capabilities_t *caps, Node **renderStyle, int nstyle, Node *voxels, NodeVolumeData *node ) {
    static int once = 0;
    int myProg;
    //unsigned int volflags;
    GLint Vertices, mvm, proj, mvpi;
    double modelviewMatrix[16],projMatrix[16], mvp[16], mvpinverse[16]; // mvmInverse[16], 
    float spmat[16];
    int nsend;
    GLint iclipplanes, inclipplanes;
    float *clipplanes;
    GLint iviewport[4];
    float viewport[4];
    GLint vp, dim;
    const float *dimensions;

//    ttglobal tg = gglobal();

//    myProg = caps->myShaderProgram;

    if(voxels){
        Node *tmpN;
        textureTableIndexStruct_s *tti;
        POSSIBLE_PROTO_EXPANSION(Node *, voxels,tmpN);
        renderFuncs_texturenode = (Node*)tmpN;

        //gradient > Oct 2016 we compute in textures.c if channels==1 and z>1 and put in rgb
        // - saves mallocing another RGBA 
        // - for scalar images RGB is unused or just 111 anyway
        // - takes 1 second on desktop CPU for 17 Mpixel image
        //if(node->renderStyle){
        //    if(volstyle_needs_normal(node->renderStyle)){
        //        switch(tmpN->_nodeType){
        //            case NODE_PixelTexture3D:
        //                ((struct X3D_PixelTexture3D*)tmpN)->_needs_gradient = TRUE; break;
        //            case NODE_ImageTexture3D:
        //                ((struct X3D_ImageTexture3D*)tmpN)->_needs_gradient = TRUE; break;
        //        }
        //    }
        //}
        //render_node(voxels) should keep pulling the texture through all stages of loading and opengl
        render_node(tmpN); //render_node(node->voxels); 

        tti = getTableTableFromTextureNode(tmpN);
        if(tti && tti->status >= TEX_LOADED){
            GLint ttiles, tex3dUseVertex,repeatSTR,magFilter;
            ttiles = GET_UNIFORM(myProg,"tex3dTiles");
            GLUNIFORM1IV(ttiles,3,tti->tiles);

            //me->tex3dUseVertex = GET_UNIFORM(myProg,"tex3dUseVertex");
            tex3dUseVertex = GET_UNIFORM(myProg,"tex3dUseVertex");
            glUniform1i(tex3dUseVertex,0); 
            repeatSTR = GET_UNIFORM(myProg,"repeatSTR");
            glUniform1iv(repeatSTR,3,tti->repeatSTR);
            magFilter = GET_UNIFORM(myProg,"magFilter");
            glUniform1i(magFilter,1); //need LINEAR //tti->magFilter);

            glActiveTexture(GL_TEXTURE0); 
            glBindTexture(GL_TEXTURE_2D,tti->OpenGLTexture); 
            FW_GL_TEXPARAMETERI( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        }
    }

    if(nstyle){
        int i;
        for(i=0;i<nstyle;i++){
            if (renderStyle[i]) {
                NodeOpacityMapVolumeStyle *style0 = (NodeOpacityMapVolumeStyle*)renderStyle[i];
                if(style0->enabled()->getValue()){
                    render_volumestyle(renderStyle[i],myProg); //send uniforms
                    // if style uses a texture, it should be the next texture ie GL_TEXTURE0+1,2..
                }
            }
        }
    }
fprintf(stderr, "before glerror 0\n");
TheApp->printRenderErrors();
#if 0
    //3.1 set uniforms: dimensions, focal length, fov (field of view), window size, modelview matrix
    //    set attributes vertices of triangles of bounding box
    // set box with vol.dimensions with triangles
    Vertices = GET_ATTRIB(myProg,"fw_Vertex");
    mvm = GET_UNIFORM(myProg,"fw_ModelViewMatrix"); //fw_ModelViewMatrix
    proj = GET_UNIFORM(myProg,"fw_ProjectionMatrix"); //fw_ProjectionMatrix
#endif
fprintf(stderr, "before glerror 1\n");
TheApp->printRenderErrors();
    if(!once)
        ConsoleMessage("vertices %d mvm %d proj %d\n",Vertices,mvm,proj);
fprintf(stderr, "before glerror 1.7\n");
TheApp->printRenderErrors();
//    sendExplicitMatriciesToShader(mvm,proj,-1,NULL,-1);
#if 0
    FW_GL_GETDOUBLEV(GL_MODELVIEW_MATRIX, modelviewMatrix);
    FW_GL_GETDOUBLEV(GL_PROJECTION_MATRIX, projMatrix);
    if(1){
        //see gluUnproject in Opengl_Utils.c
        gluMultMatricesd(modelviewMatrix, projMatrix, mvp);
        if (!gluInvertMatrixd(mvp, mvpinverse)) return;
    }else{
        matmultiplyFULL(mvp,modelviewMatrix,projMatrix);
        //matmultiplyFULL(mvp,projMatrix,modelviewMatrix);
        //if (!gluInvertMatrixd(mvp, mvpinverse)) return;
        matinverseFULL(mvpinverse,mvp); //seems different than glu's. H0: just wrong H1: transopose H2: full inverse vs factorized
    }
    matdouble2float4(spmat,mvpinverse);

    mvpi = GET_UNIFORM(myProg,"fw_ModelViewProjInverse");
    GLUNIFORMMATRIX4FV(mvpi,1,GL_FALSE,spmat);

//SEND CLIPPLANES?
    //sendClipplanesToShader(mysp);
    clipplanes = getTransformedClipPlanes();
    
    nsend = getClipPlaneCount();
    iclipplanes = GET_UNIFORM(myProg,"fw_clipplanes");
    inclipplanes = GET_UNIFORM(myProg,"fw_nclipplanes");

    GLUNIFORM4FV(iclipplanes,nsend,clipplanes);
    GLUNIFORM1I(inclipplanes,nsend);

//SEND LIGHTS IF WE HAVE A SHADER STYLE 
    //int haveShaderStyle = FALSE;
    //if(nstyle){
    //    for(int i=0;i<nstyle;i++){
    //        haveShaderStyle = haveShaderStyle || (renderStyle[i]->_nodeType == NODE_ShadedVolumeStyle); 
    //    }
    //}
    //if(haveShaderStyle){
        //send lights
/*
        if (caps->haveLightInShader) {
            sendLightInfo(caps);
            sendFogToShader(caps);
        }
*/
    //}

#endif
fprintf(stderr, "before glerror 2\n");
TheApp->printRenderErrors();

#if 0
    //get the current viewport
    glGetIntegerv(GL_VIEWPORT, iviewport); //xmin,ymin,w,h
fprintf(stderr, "after glerror 0,2\n");
TheApp->printRenderErrors();
    vp = GET_UNIFORM(myProg,"fw_viewport");
fprintf(stderr, "after glerror 0,3\n");
TheApp->printRenderErrors();
    viewport[0] = (float)iviewport[0]; //xmin
    viewport[1] = (float)iviewport[1]; //ymin
    viewport[2] = (float)iviewport[2]; //width
    viewport[3] = (float)iviewport[3]; //height
    GLUNIFORM4F(vp,viewport[0],viewport[1],viewport[2],viewport[3]);
#endif
fprintf(stderr, "after glerror 0,5\n");
TheApp->printRenderErrors();
//    dim = GET_UNIFORM(myProg,"fw_dimensions");
//    GLUNIFORM3F(dim,dimensions[0],dimensions[1],dimensions[2]);
//    GLUNIFORM3F(dimensions[0],dimensions[1],dimensions[2]);

//    if(!once) ConsoleMessage("dim %d vp %d \n",dim,vp );

fprintf(stderr, "after glerror 1.1\n");
TheApp->printRenderErrors();
    //3.2 draw with shader
//    glEnableVertexAttribArray(Vertices);
//    glVertexAttribPointer(Vertices, 3, GL_FLOAT, GL_FALSE, 0, node->m_boxtris);
TheApp->printRenderErrors();
fprintf(stderr, "after glerror 1.1\n");
     // https://www.opengl.org/wiki/Face_Culling
    glEnable(GL_CULL_FACE);
    //we want to draw only either back/far or front/near triangles, not both
    //so that we comput a ray only once.
    //and because we want to use clipplane (or frustum near side) to slice into
    //volumes, we want to make sure we are still getting ray fragments when slicing
    //so instead of drawing the front faces (which would slice away fragments/rays)
    //we want to draw only the far/back triangles so even when slicing, we'll get
    //fragment shader calls, and can compute rays.
    //assuming our triangles are defined CCW (normal)
    //setting front-face to GL_CW should ensure only the far/back triangles are rendered
    glFrontFace(GL_CW); 
TheApp->printRenderErrors();
fprintf(stderr, "after glerror 1.5\n");
     
//    glDrawArrays(GL_TRIANGLES,0,36);
    dimensions = node->dimensions()->getValue();
printf("dim 2 %f %f %f\n", dimensions[0],dimensions[1],dimensions[2]);
     glBegin(GL_TRIANGLES);
     for (int i = 0; i < 48; i += 4) {
         for (int j = 0; j < 3; j++) {
             glVertex3f(boxvert[boxtriindccw[i + j] * 3    ] * dimensions[0], 
                        boxvert[boxtriindccw[i + j] * 3 + 1] * dimensions[1], 
                        boxvert[boxtriindccw[i + j] * 3 + 2] * dimensions[2]);
         }
     }
     glEnd();
TheApp->printRenderErrors();
fprintf(stderr, "after glerror 2\n");
    glDisable(GL_CULL_FACE);
    if(voxels){
        renderFuncs_textureStackTop = 0;
        renderFuncs_texturenode = NULL;
    }
    if(nstyle){
        int i;
        for(i=0;i<nstyle;i++)
            fin_volumestyle(renderStyle[i],node);
    }
    once = 1;

TheApp->printRenderErrors();
fprintf(stderr, "after glerror\n");
} 

void child_SegmentedVolumeData(NodeSegmentedVolumeData *node){
    // http://www.web3d.org/documents/specifications/19775-1/V3.3/Part01/components/volume.html#SegmentedVolumeData
    // VolumeData + 2 fields:
    //MFBool  [in,out] segmentEnabled     []
    //SFNode  [in,out] segmentIdentifiers NULL     [X3DTexture3DNode]
    s_shader_capabilities_t *caps;
    static int once = 0;
    COMPILE_IF_REQUIRED

//    if (renderstate()->render_blend == (node->m_renderFlags & VF_Blend)) {
    if (1) {
        int itexture = 1; //voxels=0,segmentIDs=1

        if(!once)
            printf("child segmentedvolumedata \n");
        //int nstyles = 0;
        //if(node->renderStyle) nstyles = 1;

        Node *tmpNode = NULL; 
        if (node->renderStyle()->getSize() > 0)
            tmpNode = node->renderStyle()->getValue(0);
        caps = getVolumeProgram(&tmpNode,node->renderStyle()->getSize(),SHADERFLAGS_VOLUME_DATA_SEGMENT);
        //get and set segment-specific uniforms
        itexture = 1; //voxels=0,segmentIDs=1
        render_SEGMENTED_volume_data(caps,node->segmentIdentifiers()->getValue(),itexture,node);
        //render generic volume 
        render_GENERIC_volume_data(caps,&tmpNode,node->renderStyle()->getSize(),node->voxels()->getValue(),(NodeVolumeData*)node );
        once = 1;
    } //if VF_Blend

}
void render_ISO_volume_data(s_shader_capabilities_t *caps,NodeIsoSurfaceVolumeData *node){
    // http://www.web3d.org/documents/specifications/19775-1/V3.3/Part01/components/volume.html#IsoSurfaceVolumeData
    // VolumeData + 4 fields, minus 1 field
    //SFFloat [in,out] contourStepSize  0        (-INF,INF)
    //SFNode  [in,out] gradients        NULL     [X3DTexture3DNode]
    //SFFloat [in,out] surfaceTolerance 0        [0,INF)
    //MFFloat [in,out] surfaceValues    []       (-INF,INF)
    //MFNode  [in,out] renderStyle      []       [X3DVolumeRenderStyleNode]
    //minus SFNode renderStyle
    int myProg;
    GLint istep, itol,ivals,invals;
    myProg= caps->myShaderProgram;
    istep = GET_UNIFORM(myProg,"fw_stepSize");
    glUniform1f(istep,node->contourStepSize()->getValue()); 
    itol = GET_UNIFORM(myProg,"fw_tolerance");
    glUniform1f(itol,node->surfaceTolerance()->getValue()); 

    ivals = GET_UNIFORM(myProg,"fw_surfaceVals");
    glUniform1fv(ivals,node->surfaceValues()->getSize(),node->surfaceValues()->getValues()); 
    invals = GET_UNIFORM(myProg,"fw_nVals");
    glUniform1i(invals,node->surfaceValues()->getSize());
    if(node->renderStyle()->getSize()){
        int i;
        // OLDCODE GLint istyles;
        GLint instyles;
        int *styleflags = MALLOC(int*,sizeof(int)*node->renderStyle()->getSize());
        for(i=0;i<node->renderStyle()->getSize();i++){
            styleflags[i] = prep_volumestyle(node->renderStyle()->getValue(i),0);
        }
        // OLDCODE istyles = GET_UNIFORM(myProg,"fw_surfaceStyles");
        glUniform1iv(ivals,node->renderStyle()->getSize(),styleflags);
        instyles = GET_UNIFORM(myProg,"fw_nStyles");
        glUniform1i(instyles,node->renderStyle()->getSize());
    }

    //renderstyle handling?
    //Options:
    // a) include all renderstyles in the shader
    // b) go through the list and |= (multiple times should show up as once?)
    // then make a special shader to (equivalent to switch-case) on each voxel/gradient after iso value has been computed
}

void child_IsoSurfaceVolumeData(NodeIsoSurfaceVolumeData *node){
    // http://www.web3d.org/documents/specifications/19775-1/V3.3/Part01/components/volume.html#IsoSurfaceVolumeData
    // VolumeData + 4 fields:
    //SFFloat [in,out] contourStepSize  0        (-INF,INF)
    //SFNode  [in,out] gradients        NULL     [X3DTexture3DNode]
    //SFFloat [in,out] surfaceTolerance 0        [0,INF)
    //MFFloat [in,out] surfaceValues    []       (-INF,INF)
    static int once = 0;
    COMPILE_IF_REQUIRED
//    if (renderstate()->render_blend == (node->m_renderFlags & VF_Blend)) {
    if (1) {
        unsigned int voldataflags;
        s_shader_capabilities_t *caps;
        int MODE;

        if(!once)
            printf("child segmentedvolumedata \n");
        voldataflags = SHADERFLAGS_VOLUME_DATA_ISO;

        MODE = node->surfaceValues()->getSize() == 1 ? 1 : 3;
        MODE = node->contourStepSize()->getValue() != 0.0f && MODE == 1 ? 2 : 1;
        if(MODE == 3)
            voldataflags |= SHADERFLAGS_VOLUME_DATA_ISO_MODE3;
        Node *tmpNode = NULL;
        if (node->renderStyle()->getSize() > 0)
            tmpNode = node->renderStyle()->getValue(0);
        caps = getVolumeProgram(&tmpNode,node->renderStyle()->getSize(), voldataflags);
        //get and set ISO-specific uniforms
        render_ISO_volume_data(caps,node);
        //render generic volume 
        render_GENERIC_volume_data(caps,&tmpNode,node->renderStyle()->getSize(),node->voxels()->getValue(),(NodeVolumeData*)node );
        once = 1;
    } //if VF_Blend
}

void child_VolumeData(NodeVolumeData *node){
    // http://www.web3d.org/documents/specifications/19775-1/V3.3/Part01/components/volume.html#VolumeData
    // VolumeData 
    s_shader_capabilities_t *caps;
    static int once = 0;
    COMPILE_IF_REQUIRED

//    if (renderstate()->render_blend == (node->_renderFlags & VF_Blend)) {
    if (1) {
        int nstyles = 0;
        if(!once)
            printf("child volumedata \n");
        if(node->renderStyle()->getValue()) nstyles = 1;
        Node *tmpNode = node->renderStyle()->getValue();
        caps = getVolumeProgram(&tmpNode,nstyles, SHADERFLAGS_VOLUME_DATA_BASIC);
        //render generic volume 
        render_GENERIC_volume_data(caps,&tmpNode,nstyles,node->voxels()->getValue(),(NodeVolumeData*)node );
        once = 1;
    } //if VF_Blend

}

void
NodeImageTexture3D::load()
{
    if (!m_loaded) {
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
        loadTextureNode(this, NULL);    
        m_loaded = true;
    }
}

void
NodeImageTexture3D::preDraw()
{
    load();
}

void
NodeImageTexture3D::draw(int pass)
{
    textures_prv = m_textures_prv;
    Node *node = getParent();
    if (node->getType() == X3D_VOLUME_DATA) {
        NodeVolumeData *data = (NodeVolumeData *)node;
        Node *tmpNode = data->renderStyle()->getValue();
        render_GENERIC_volume_data(NULL, &tmpNode, 1, 
                                   data->voxels()->getValue(), data);
   }
}
# endif
#endif


