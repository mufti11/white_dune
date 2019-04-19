/*
 * NodeMovieTexture.cpp
 *
 * Copyright (C) 1999 Stephen F. White
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

#include "NodeMovieTexture.h"
#include "Proto.h"
#include "FieldValue.h"
#include "SFBool.h"
#include "SFFloat.h"
#include "SFTime.h"
#include "MFString.h"
#include "Field.h"  // for FF_URL
#include "ExposedField.h"
#include "DuneApp.h"
#include "Scene.h"
#include "swt.h"

ProtoMovieTexture::ProtoMovieTexture(Scene *scene)
  : Proto(scene, "MovieTexture")
{
    description.set( 
        addExposedField(SFSTRING, "description", new SFString("")));
    setFieldFlags(description, FF_X3D_ONLY);
    loop.set(
          addExposedField(SFBOOL, "loop", new SFBool(false)));
    speed.set(
          addExposedField(SFFLOAT, "speed", new SFFloat(1.0f)));
    pauseTime.set(
          addExposedField(SFTIME, "pauseTime", new SFTime(0.0)));
    setFieldFlags(pauseTime, FF_X3D_ONLY); 
    pitch.set(
          addExposedField(SFFLOAT, "pitch", new SFFloat(1.0f), 
                          new SFFloat(0.0f)));
    setFieldFlags(pitch, FF_X3D_ONLY); 
    resumeTime.set(
          addExposedField(SFTIME, "resumeTime", new SFTime(0.0)));
    setFieldFlags(resumeTime, FF_X3D_ONLY);
    startTime.set(
          addExposedField(SFTIME, "startTime", new SFTime(0.0f)));
    setFieldFlags(startTime, EIF_RECOMMENDED);
    stopTime.set(
          addExposedField(SFTIME, "stopTime", new SFTime(0.0f)));
    setFieldFlags(stopTime, EIF_RECOMMENDED);
    url.set(
          addExposedField(MFSTRING, "url", new MFString(), FF_URL, NULL));
    repeatS.set(
          addField(SFBOOL, "repeatS", new SFBool(true)));
    repeatT.set(
          addField(SFBOOL, "repeatT", new SFBool(true)));
    if (TheApp->getCoverMode()) {
        // non standard Covise/COVER extensions
        blendMode.set(
          addField(SFINT32, "blendMode", new SFInt32(0), 
                   new SFInt32(0), new SFInt32(5)));
        setFieldFlags(blendMode, FF_COVER_ONLY);
    }

    textureProperties.set(
        addField(SFNODE, "textureProperties", new SFNode(), 
                 X3D_TEXTURE_PROPERTIES));
    setFieldFlags(textureProperties, FF_X3D_ONLY);

    kambiTextureCommonFields()
    kambiVrmlTextureCommonFields()

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

    addEventOut(SFTIME, "duration_changed");
    addEventOut(SFTIME, "elapsedTime", FF_X3D_ONLY);
    addEventOut(SFBOOL, "isActive");
    addEventOut(SFBOOL, "isPaused", FF_X3D_ONLY);
}

Node *
ProtoMovieTexture::create(Scene *scene)
{ 
    return new NodeMovieTexture(scene, this);
}

NodeMovieTexture::NodeMovieTexture(Scene *scene, Proto *def)
  : Node(scene, def)
{
     m_loaded = false;
     m_textureName = 0;
}

void
NodeMovieTexture::load()
{
    MFString *urls = url();

    if (urls->getSize() == 0)
        return;
    for (int i = 0; i < urls->getSize(); i++) {
        MyString path;
        URL urlI(TheApp->getImportURL(), urls->getValue(i));
        if (urls->getValue(i).length() == 0) continue;
        m_scene->Download(urlI, &path);
        // download only
    }
}

#include "config.h"
#include "freewrl_define.h"

#ifdef HAVE_FFMPEG

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


/* July 2016 note:
    http://www.web3d.org/documents/specifications/19775-1/V3.3/Part01/components/texturing.html#MovieTexture
    - specs say to support MPEG1
    - online says all patents have expired for MPEG1 and its Level II audio
    http://www.web3d.org/x3d/content/examples/ConformanceNist/
    - see Movie texture example with VTS.mpg

    2.3 ffmpeg.org
        LGPL by default (can add in GPL parts, we don't need)
        but may include patented algorithms for post-MPEG1-Audio_Level_II
        but has a way to limit codecs available at runtime: 
            instead of load_all() you would somehow say which ones to load?? see dranger tutorial about tut5
        and if so and you limit codecs to MPEG1 with Level II audio, then
        ffplay.c and tutorial http://dranger.com/ffmpeg/ can help, 
            substituting freewrl's audio lib's API, pthreads, and our openGL plumbing for SDL
            might be able to #define some pthread for SDL thread functions in ffplay.c
            - except don't assume freewrl's pthreads is complete: no cancel, its an emulated lib on some platforms
        https://github.com/FFmpeg/FFmpeg
        https://github.com/FFmpeg/FFmpeg/blob/master/ffplay.c
            line interesting function
            3352 event_loop() - play, pause, rewind user functions
            3000 infinite_buffer real_time -in theory could pull from url instead of file
            2660 forcing codec by name - if blocking patented MPEG2+ / sticking to MPEG1+LevelII audio
            2400 audio_decode_frame - somwhere we need to get the audio PCM decompressed buffer, so we can pass to our audio API
            1808 get_video_frame - somewhere we need to get the current frame, perhaps from do_MovieTextureTick() we would get closest-to-current frame and freeze it for opengl
            1506 video_refresh called to display each frame


    3. Platform supplied
    3.1 windows > directX audio and video
    3.2 android > 
    3.3 linux desktop > 

    B. freewrl texture and sound plumbing
    MAJOR DESIGN OPTIONS:
    I. Process video frame into mipmapped opengl texture:
    a) on loading:, pre-process entire video into mipmapped opengl textures, one per video frame (2009 implementation) 
        Disadvantage: 30fps x 10 seconds is 300 textures - a lot to store, and prepare if unneeded
        x a lot of opengl textures needed - exhaustion/limits?
        Benefit: faster per frame when playing
    b) on-the-fly during play: in a separate thread, process video frames to replace single opengl texture 
        Benefit: vs c: mpeg decompression: successive frames require previous frame decoded, so state is managed
        - vs a,c: thread can do its own throttling 
        - vs a: storage is just the decompression sequence
        Disadvantage: single core will be doing a lot of un-needed mipmapping
        x thread collisions - texture being replaced in one thread and drawn in another?
        x stereo vision > left and right eye might see different texture
    c) on-the-fly in do_tick (once per render frame), interpolate closest frame based on time, replace single opengl texture
        Benefit: vs a,b: no unnecessary frames interpolated, no unnecessary mipmapping in any thread, just the frame needed
        vs. b: same frame appears in left/right stereo, no timing weirdness
        vs. a: storage is just the decompression sequence
    d) combo of b) and c): separate thread prepares small set of raw frames, 
        do_MovieTextureTick() picks one or asks for one and opengl-izes it
    II. Support streaming?
        a) Continuous Streaming video from url or 
        b) just finite local file
        SIMPLIFYING DECISION: b) finite local file
    III. Separate thread for decoding / interpolating frames?
        a) or in rendering thread (any throttling problems?)
        b) new separate thread (but what about mipmapping and opengizing interpolated frame?)
        c) (somehow) use texture thread which is currently parked/unused once images are mipmapped 
            - but currently triggered during image file loading process
            - could set a flag to an earlier state and re-submit?
        SIMPLIFYING DECISION: depends on implementation ie what libs you get and how easy it is

    A few facts / details:
    input media: MPEG1 contains Audio and/or Video
    Nodes: which we want to use to support SoundSource and/or Texture2D
    Texture2D: shows one frame at a time in opengl
    SoundSource: used as / like an AudioClip for Sound node. AudioClip has its own private thread
    It doesn't make sense to load a movietexture 2x if using for both texture and sound. 
    - you would DEF for one use, and USE for the other
    - then when you play, you play both USEs at the same time so audio and video are synced
    Sound is handled per-node.
    Textures have an intermediary texturetableindexstruct
    
    2003 - 2009 freewrl movietexture:
        - on load: decoded and created an opengl texture for each movie frame


    Proposed freewrl plumbing:

    1. for texture rendering, MovieTexture works like ImageTexture on each render_hier frame, 
        with a single opengl texture number
    2. leave it to the library-specifics to decide if
        a) decode-on-load
        b) decode in a separate thread, anticipatory/queue
        c) decode-on-demand
    3. the pause, stop, play, rewind interface is usable as SoundSource like Audioclip, analogous to AudioClip

    top level interface:
    X3DMovieTexture._movie; an opaque pointer that will hold a malloced struct representing 
        the movie container and streams; different implementations will be different
    movie_load() - like loadTextures.c image loaders - takes local path and gets things started
        puts intial load into ._movie of the requesting node ie res->(wheretoplacedata,offset) = (MovieTexture,_movie)
    do_MovieTextureTick() 
        in senseInterp.c, once per frame (so stereo uses same movie frame for left/right)
        could ask for closest frame based on movie time
        node->tti->texdata = getClosestMovieFrame(movietime)
            a) decode-on-load would have the frame ready in a list 
            b) multi-thread anticipatory decode would have a private queue/list of decoded frames,
                and get the closest one, and discard stale frames, and restart decode queue to fill up
                queue again
            c) decode-on-demand would decode on demand
        Texture2D(,,,node->tti->opeglTexture,,,node->tti->texdata) //reset texture data

    loadstatus_MovieTexture(struct X3D_MovieTexture *node) - loadsensor can check if file loaded
    loadstatus_AudioClip(struct X3D_AudioClip *node) - loadsensor can check if file loaded
    locateAudioSource (struct X3D_AudioClip *node) - will work for MovieTexture
    
    search code for MovieTexture, resm_movie to see all hits
*/

double compute_duration(int ibuffer){
    double retval = 1.0;
#ifdef HAVE_OPENAL
    int ibytes;
    int ibits;
    int ichannels;
    int ifreq;
    double framesizebytes, bytespersecond;
    alGetBufferi(ibuffer,AL_FREQUENCY,&ifreq);
    alGetBufferi(ibuffer,AL_BITS,&ibits);
    alGetBufferi(ibuffer,AL_CHANNELS,&ichannels);
    alGetBufferi(ibuffer,AL_SIZE,&ibytes);
    framesizebytes = (double)(ibits * ichannels)/8.0;
    bytespersecond = framesizebytes * (double)ifreq;
    if(bytespersecond > 0.0)
        retval = (double)(ibytes) / bytespersecond;
    else
        retval = 1.0;
#endif
#ifdef HAVE_OLDSOUND
    //not sure how this is supposed to work, havent compiled it, good luck
    float pitch;
    double stime, sttime;
    int loop;
    pitch = node->pitch;
    stime = node->startTime;
    sttime = node->stopTime;
    loop = node->loop;

    retval = SoundSourceInit (ibuffer, node->loop,
        (double) pitch,(double) stime, (double) sttime, filename);
    
#endif
    return retval;
}


//put your choice in your config.h (or windows preprocessor directives):
#define MOVIETEXTURE_FFMPEG 1

//Option A.
//    movie_load - load as BLOB using standard FILE2BLOB in io_files.c retval = resource_load(res);  //FILE2BLOB
//    parse_movie - converts BLOB to sound and video parts, returns parts
//Option B.
//  movie_load - parse movie as loading
//  parse_movie - return movie parts 

#if MOVIETEXTURE_FFMPEG
//#include "MPEG_Utils_ffmpeg.c"
int movie_load_from_file(char *fname, void **opaque);
double movie_get_duration(void *opaque);
unsigned char *movie_get_frame_by_fraction(void *opaque, float fraction, int *width, int *height, int *nchan);
unsigned char * movie_get_audio_PCM_buffer(void *opaque,int *freq, int *channels, int *size, int *bits);
//BufferData * alutBufferDataConstruct (ALvoid *data, size_t length, ALint numChannels,
//                          ALint bitsPerSample, ALfloat sampleFrequency);

#define LOAD_STABLE 10 //from Component_Sound.c
#endif

bool movie_load(NodeMovieTexture *node){
    bool retval;
    // see io_files.c for call place
    //Option A: just load blob for later
    // retval = resource_load(res);  //FILE2BLOB
    //Option B:
    // parse during load
    // copied from imagery_load - but TEX_READ flag will be wrong for movie
    //int textureNumber;
    //struct textureTableIndexStruct *entry; // = res->whereToPlaceData;
    //textureNumber = res->textureNumber;
    //if(res->status == ress_downloaded){
    //    entry = getTableIndex(textureNumber);
    //    if(entry)
    //    if (movie_load_from_file(entry, res->actual_file)) {
    //        entry->status = TEX_READ; // tell the texture thread to convert data to OpenGL-format 
    //        res->status = ress_loaded;
    //        retval = TRUE;
    //        return retval;
    //    }
    //}
    //res->status = ress_not_loaded;
    retval = FALSE;

#if MOVIETEXTURE_FFMPEG
    {
        void *opaque;
        int loaded;

        MyString filename = "";
        MyString oldDir = "";
        oldDir += TheApp->getImportURL();
        MFString *urls = node->url();
        if (urls == NULL)
            return false;
        for (int j = 0; j < urls->getSize(); j++) {
            if (urls->getValue(j).length() == 0) 
                continue;
            URL url(oldDir, urls->getValue(j));
            MyString path;
            if (node->getScene()->Download(url, &path)) {
                TheApp->setImportURL(url.GetPath());
                filename += path;
            }
        }
        const char *fname = filename;
        loaded = movie_load_from_file((char *)fname,&opaque);
        retval = loaded > -1 ? TRUE : FALSE;
        if(loaded){
            int freq,channels,size,bits;
            unsigned char * pcmbuf;
/*
            res->status = ress_loaded;
            res->complete = TRUE;
            res->status = ress_parsed; //we'll skip the parse_movie/load_from_blob handler 
*/
//            node = (struct X3D_MovieTexture *) res->whereToPlaceData;
            //AUDIO AND/OR VIDEO CHANNELS?
            node->m_duration_changed = movie_get_duration(opaque);
            node->m_fw_movie = opaque;
            node->m_loadstatus = LOAD_STABLE;
            //VIDEO CHANNEL?
            //double totalframes = node->duration_changed * 30.0; 
            node->m_speed = 1.0; //1 means normal speed 30.0 / totalframes; //in fractions per second = speed in frames/second / totalframes
//            MARK_EVENT (X3D_NODE(node), offsetof(struct X3D_MovieTexture, duration_changed));
            //AUDIO CHANNEL?
            //node->__sourceNumber = parse_movie(node,buffer,len); //__sourceNumber will be openAL buffer number
            pcmbuf = movie_get_audio_PCM_buffer(opaque,&freq,&channels,&size,&bits);
            if(pcmbuf){
                //MPEG1 level1,2 are compressed audio
                //decoders generally deliver so called PCM pulse code modulated buffers
                //and that's what audio drivers on computers normally take
                //and same with the APIs that wrap the hardware drivers ie openAL API
                printf("audio freq %d channels %d size %d bits per channel %d\n",freq,channels,size,bits);
                #ifdef HAVE_OPENAL
                // http://open-activewrl.sourceforge.net/data/OpenAL_PGuide.pdf
                // page 6
                {
                    int format;
                    ALuint albuffer; 
                    static int once = 0;
                    if(!once){
                        #ifdef HAVE_ALUT
                        //alutInit(0, NULL); // Initialize OpenAL 
                        if (!alutInitWithoutContext(NULL, NULL))
                            ConsoleMessage("ALUT init failed\n");
                        #endif //HAVE_ALUT
                        alGetError(); // Clear Error Code
                        //SoundEngineInit();
                        once = 1;
                    }

                    alGenBuffers(1, &albuffer); 
                    //al.h
                    //#define AL_FORMAT_MONO8                          0x1100
                    //#define AL_FORMAT_MONO16                         0x1101
                    //#define AL_FORMAT_STEREO8                        0x1102
                    //#define AL_FORMAT_STEREO16                       0x1103
                    //if(bits == 8)
                    //    format = AL_FORMAT_MONO8;
                    //else
                    //    format = AL_FORMAT_MONO16;
                    //if(channels == 2) 
                    //    if(bits == 8)
                    //        format = AL_FORMAT_STEREO8;
                    //    else
                    //        format = AL_FORMAT_STEREO16;
                    format = 0;
                    switch(bits){
                        case 8:
                            format = AL_FORMAT_MONO8;
                            if(channels == 2) 
                                format = AL_FORMAT_STEREO8;
                            break;
                        case 16:
                            format = AL_FORMAT_MONO16;
                            if (channels == 2)
                                format = AL_FORMAT_STEREO16;
                            break;
                        case 32:
                            #ifdef AL_EXT_float32
                            format = AL_FORMAT_MONO_FLOAT32;
                            if (channels == 2) 
                                format = AL_FORMAT_STEREO_FLOAT32;
                            break;
                            #endif
                        default:
                            break;
                    }
                    if(format > 0){
                        //this is a complex function that tries to figure out if its float, int PCM etc
                        alBufferData(albuffer,format,pcmbuf,size,freq); 
                        //BufferData * bdata = _alutBufferDataConstruct( pcmbuf,size,channels,bits, freq);

                        node->__sourceNumber = albuffer;
                    }
                }
                #endif //HAVE_OPENAL
            }
        } 

//        printf("opqaue = %p, loaded=%d \n",opaque,res->status);
    }
#endif
    return retval;
}
int    parse_audioclip(struct X3D_AudioClip *node,char *bbuffer, int len);
int parse_movie(NodeMovieTexture *node, char *buffer,int len){
    //Option B - parse blob
    //if your movie api will take a blob, you can call it from here to parse
    //convert BLOB (binary large object) into video and audio structures
    //Option A and B - return audio and video parts
    int audio_sourcenumber;
    audio_sourcenumber = -1; //BADAUDIOSOURCE
    //MPEG1 level1,2 are compressed audio
    //decoders generally deliver so called PCM pulse code modulated buffers
    //and that's what audio drivers on computers normally take
    //and same with the APIs that wrap the hardware drivers ie openAL API
#ifdef MOVIETEXTURE_STUB
#elif MOVIETEXTURE_BERKLEYBROWN
#elif MOVIETEXTURE_FFMPEG
#elif MOVIETEXTURE_LIBMPEG2
#endif
    return audio_sourcenumber;
}
double compute_duration(int ibuffer);

bool  process_res_movie(NodeMovieTexture *node){
    // METHOD_LOAD_ON_DEMAND
    //you'll get in here if you didn't (completely) handle movie_load from file
    //
    char *buffer;
    int len;
    buffer = NULL;
    len = 0;

    MyString filename = "";
    MyString oldDir = "";
    oldDir += TheApp->getImportURL();
    MFString *urls = node->url();
    if (urls == NULL)
        return false;
    for (int j = 0; j < urls->getSize(); j++) {
        if (urls->getValue(j).length() == 0) 
            continue;
        URL url(oldDir, urls->getValue(j));
        MyString path;
        if (node->getScene()->Download(url, &path)) {
            TheApp->setImportURL(url.GetPath());
            filename += path;
        }
    }
    const char *fname = filename;
    buffer = (char *)fname;
    node->m_sourceNumber = parse_movie(node,buffer,len); //__sourceNumber will be openAL buffer number
    if(node->m_sourceNumber > -1) {
        node->m_duration_changed = compute_duration(node->m_sourceNumber);
//        MARK_EVENT (X3D_NODE(node), offsetof(struct X3D_MovieTexture, duration_changed));
        return TRUE;
    } 
    return FALSE;
}


// - still needed ? don't know depends on implementation
//void getMovieTextureOpenGLFrames(int *highest, int *lowest,int myIndex) {
//        textureTableIndexStruct_s *ti;
//
///*        if (myIndex  == 0) {
//        printf ("getMovieTextureOpenGLFrames, myIndex is ZERL\n");
//        *highest=0; *lowest=0;
//    } else {
//*/
//    *highest=0; *lowest=0;
//    
//    #ifdef TEXVERBOSE
//    printf ("in getMovieTextureOpenGLFrames, calling getTableIndex\n");
//    #endif
//
//           ti = getTableIndex(myIndex);
//
///*     if (ti->frames>0) { */
//        if (ti->OpenGLTexture != TEXTURE_INVALID) {
//            *lowest = ti->OpenGLTexture;
//            *highest = 0;
///*             *highest = ti->OpenGLTexture[(ti->frames) -1]; */
//        }
///*     } */
//}

unsigned char *movietexture_get_frame_by_fraction(Node* node, float fraction, int *width, int *height, int *nchan){
    unsigned char* retval = NULL;
    if(node && node->getType() == VRML_MOVIE_TEXTURE){
        NodeMovieTexture *movietexture = (NodeMovieTexture *)node;
#if MOVIETEXTURE_FFMPEG
        retval = movie_get_frame_by_fraction(movietexture->m_fw_movie,fraction,width,height,nchan);
#endif
    }
    return retval;
}

// http://dranger.com/ffmpeg/tutorial01.html
#ifdef HAVE_INTTYPES_H
#include <inttypes.h>
#endif
#include <math.h>
#include <limits.h>
#include <signal.h>
#include <stdint.h>

#define inline   //someone in ffmpeg put a bit of cpp in their headers, this seemed to fix it
extern "C" {
    //#include "libavutil/avstring.h"
    //#include "libavutil/colorspace.h"
    //#include "libavutil/mathematics.h"
    //#include "libavutil/pixdesc.h"
    #include "libavutil/imgutils.h"
    //#include "libavutil/pixfmt.h"
    //#include "libavutil/dict.h"
    //#include "libavutil/parseutils.h"
    //#include "libavutil/samplefmt.h"
    //#include "libavutil/avassert.h"
    //#include "libavutil/time.h"
    #include "libavformat/avformat.h"
    //#include "libavdevice/avdevice.h"
    #include "libswscale/swscale.h"
    //#include "libswresample/swresample.h"
    #include "libavutil/opt.h"
    //#include "libavcodec/avfft.h"
    
    //we don't support libav variant, which has libavresample 
    // ... (ubuntu 1404LTS has libav, can get ffmpeg on 1606)
    #include "libswresample/swresample.h"
}
// compatibility with newer API
#if LIBAVCODEC_VERSION_INT < AV_VERSION_INT(55,28,1)
#define av_frame_alloc avcodec_alloc_frame
#define av_frame_free avcodec_free_frame
#endif


//from ffmpeg tutorial01.c
//save to .ppm imge format for debugging, which gimp will read but only if RGB24 / nchan==3
void SaveFrame(AVFrame *pFrame, int width, int height, int nchan, int iFrame) {
  FILE *pFile;
  char szFilename[32];
  int  y;
  
  // Open file
  sprintf(szFilename, "frame%d.ppm", iFrame);
  pFile=fopen(szFilename, "wb");
  if(pFile==NULL)
    return;
  
  // Write header
  fprintf(pFile, "P6\n%d %d\n255\n", width, height);
  
  // Write pixel data
  for(y=0; y<height; y++)
    fwrite(pFrame->data[0]+y*pFrame->linesize[0], 1, width*nchan, pFile);
  
  // Close file
  fclose(pFile);
}
float fwroundf(float val){
    //some math.h don't have round. here's dug9 version.
    int ival;
    float singv, valv;
    singv = val < 0.0f ? -1.0f : 1.0f;
    valv = fabsf(val);
    valv = valv + .5f;
    ival = (int)valv;
    valv = (float)ival;
    valv *= singv;
    return valv;
}
//our opaque pointer is a struct:
struct fw_movietexture {
    //AVFormatContext *pFormatCtx; //don't need to save for decode-on-load
    //AVCodecContext *pVideoCodecCtx; //don't need to save for decode-on-load
    //video and audio section:
    double duration;
    //video section:
    int width,height,nchan,nframes,fps;
    unsigned char **frames;
    //audio section:
    unsigned char *audio_buf;
    int audio_buf_size;
    int channels;
    int freq;
    int bits_per_channel;
};
int movie_load_from_file(char *fname, void **opaque){
    static int once = 0;
    struct fw_movietexture fw_movie;
    AVFormatContext *pFormatCtx;
    int i, videoStream, audioStream;
    AVCodecContext *pCodecCtxOrig;
    AVCodecContext *pCodecCtx;
    AVCodecContext  *aCodecCtxOrig;
    AVCodecContext  *aCodecCtx;
    AVCodec         *aCodec;
    AVFrame            *aFrame;
    AVFrame            *aFrameB;
    //uint8_t *audio_pkt_data = NULL;
    //int audio_pkt_size = 0;
    unsigned int audio_buf_size;
    unsigned int audio_buf_index;
    uint8_t * audio_buf;
    SwrContext *swr; 
    int audio_resample_target_fmt;
    int do_audio_resample;
    struct SwsContext *sws_ctx;
    int frameFinished;
    AVPacket packet;
    AVFrame *pFrame;
    AVCodec *pCodec;
    Stack *fw_framequeue;
    AVFrame *pFrameRGB;
    int nchan;
    uint8_t *buffer;



    *opaque = NULL;
    //initialize ffmpeg libs once per process
    if(once == 0){
        av_register_all(); //register all codecs - will filter in the future for patent non-expiry
        once = 1;
    }
    pFormatCtx = NULL;

    // Open video file
    if(avformat_open_input(&pFormatCtx, fname, NULL, NULL)!=0)
        return -1; // Couldn't open file

    // Retrieve stream information
    if(avformat_find_stream_info(pFormatCtx, NULL)<0)
        return -1; // Couldn't find stream information

    // Dump information about file onto standard error
    av_dump_format(pFormatCtx, 0, fname, 0);
    //fw_movie.pFormatCtx = pFormatCtx;

    pCodecCtxOrig = NULL;
    pCodecCtx = NULL;

    // Find the first video stream
    videoStream=-1;
    audioStream=-1;
    for(i=0; i<pFormatCtx->nb_streams; i++){
        if(pFormatCtx->streams[i]->codec->codec_type==AVMEDIA_TYPE_VIDEO && videoStream < 0) {
            videoStream=i;
        }
        if(pFormatCtx->streams[i]->codec->codec_type==AVMEDIA_TYPE_AUDIO && audioStream < 0) {
            audioStream=i;
        }
    }
    if(videoStream==-1 && audioStream == -1)
        return -1; // Didn't find either video or audio stream



    //audio and video prep
    memset(&fw_movie,0,sizeof(struct fw_movietexture));
    fw_movie.frames = NULL;
    fw_movie.nframes = 0;
    fw_movie.audio_buf = NULL;
    fw_movie.audio_buf_size = 0;

    //audio function-scope variables
    aCodecCtxOrig = NULL;
    aCodecCtx = NULL;
    aCodec = NULL;
    aFrame = NULL;
    aFrameB = NULL;
    //uint8_t *audio_pkt_data = NULL;
    //int audio_pkt_size = 0;
    audio_buf_size = 1000000;
    audio_buf_index = 0;
    audio_buf = NULL;
    swr = NULL; 
    audio_resample_target_fmt = 0;
    do_audio_resample = FALSE;

    //audio prep
    if(audioStream > -1){

        aCodecCtxOrig=pFormatCtx->streams[audioStream]->codec;
        aCodec = avcodec_find_decoder(aCodecCtxOrig->codec_id);
        if(!aCodec) {
            fprintf(stderr, "Unsupported codec!\n");
            return -1;
        }

        // Copy context
        //if(avcodec_copy_context(aCodecCtx, aCodecCtxOrig) != 0) {
        //    fprintf(stderr, "Couldn't copy codec context");
        //    return -1; // Error copying codec context
        //}
        #if LIBAVCODEC_VERSION_MAJOR == 56 //ffmpeg 2.8
        // https://ffmpeg.org/doxygen/2.8/filtering_video_8c-example.html //in open_input_file
        aCodecCtx = aCodecCtxOrig;
        #elif LIBAVCODEC_VERSION_MAJOR == 570  //ffmpeg 3.2
        {
            AVCodecParameters *aparams;
            aCodecCtx = avcodec_alloc_context3(aCodec);
            aparams = avcodec_parameters_alloc();
            avcodec_parameters_from_context(aparams, aCodecCtxOrig);
            avcodec_parameters_to_context(aCodecCtx,aparams);
            avcodec_parameters_free(&aparams);
        }
        #elif LIBAVCODEC_VERSION_MAJOR >= 57 //58 //ffmpeg 4.0 and 3.2 also runs here
        // https://ffmpeg.org/doxygen/4.0/filtering_video_8c-example.html  //in open_input_file
        aCodecCtx = avcodec_alloc_context3(aCodecCtxOrig->codec);
        avcodec_parameters_to_context(aCodecCtx, pFormatCtx->streams[audioStream]->codecpar);
        av_opt_set_int(aCodecCtx, "refcounted_frames", 1, 0);
        #endif

        // Set audio settings from codec info
        fw_movie.channels = aCodecCtx->channels;
        fw_movie.freq = aCodecCtx->sample_rate;
        fw_movie.bits_per_channel = aCodecCtx->bits_per_raw_sample; 

        //printf("audio sample format %d\n",aCodecCtx->sample_fmt);
        // online I found request_sample_fmt is for older versions 1.1 and down, use swresample now
        //aCodecCtx->request_sample_fmt = AV_SAMPLE_FMT_FLTP; //AV_SAMPLE_FMT_S16P; //AV_SAMPLE_FMT_S16;

        printf("bits per coded channel=%d\n",aCodecCtx->bits_per_coded_sample);


        if(avcodec_open2(aCodecCtx, aCodec, NULL) < 0){
            fprintf(stderr, "Could not open codec\n");
            return -1;
        }
    

        audio_buf = (uint8_t *)malloc(audio_buf_size);
        aFrame=av_frame_alloc();
        aFrameB=av_frame_alloc();

        //assuming we resample to what we want:
        audio_resample_target_fmt = aCodecCtx->sample_fmt;
        if(aCodecCtx->sample_fmt != AV_SAMPLE_FMT_S16) {
            fw_movie.channels = 2;
            fw_movie.freq = 44100;
            fw_movie.bits_per_channel = 16; 
            audio_resample_target_fmt = AV_SAMPLE_FMT_S16;
            do_audio_resample = TRUE;

            // win32 openAL has problems with FLTP (float) audio format  
            // and android openSLES says when queuing chunks can only use PCM
            // recent versions of libavcodec convert mp4 audio to FLTP
            // so we will convert to an older S16 or S16P format
            //swresample didn't work for me, hand-coded did
            //// Set up SWR context once you've got codec information
            //swr = swr_alloc();
            //av_opt_set_int(swr, "in_channel_layout",  aCodecCtx->channel_layout, 0);
            //av_opt_set_int(swr, "out_channel_layout", aCodecCtx->channel_layout,  0);
            //av_opt_set_int(swr, "in_sample_rate",     aCodecCtx->sample_rate, 0);
            //av_opt_set_int(swr, "out_sample_rate",    aCodecCtx->sample_rate, 0);
            //av_opt_set_sample_fmt(swr, "in_sample_fmt",  aCodecCtx->sample_fmt, 0);
            //av_opt_set_sample_fmt(swr, "out_sample_fmt", AV_SAMPLE_FMT_S16,  0);
            // https://www.ffmpeg.org/doxygen/3.2/group__lswr.html#details

            swr = swr_alloc_set_opts(NULL,  // we're allocating a new context
                AV_CH_LAYOUT_STEREO,  // out_ch_layout
                AV_SAMPLE_FMT_S16,    // out_sample_fmt
                44100,                // out_sample_rate
                aCodecCtx->channel_layout, // in_ch_layout
                aCodecCtx->sample_fmt,   // in_sample_fmt
                aCodecCtx->sample_rate,   // in_sample_rate
                0,                    // log_offset
                NULL);                // log_ctx
            swr_init(swr);
        }

    }

    //video function-scope variables
    sws_ctx = NULL;
    pFrame = NULL;
    pCodec = NULL;
    fw_framequeue = NULL;
    pFrameRGB = NULL;
    buffer = NULL;
    //video prep
    if(videoStream > -1){
        int numBytes;
        AVPixelFormat av_pix_fmt;

        // Get a pointer to the codec context for the video stream
        pCodecCtxOrig = pFormatCtx->streams[videoStream]->codec;


        // Find the decoder for the video stream
        pCodec=avcodec_find_decoder(pCodecCtxOrig->codec_id);
        if(pCodec==NULL) {
            fprintf(stderr, "Unsupported codec!\n");
            return -1; // Codec not found
        }
        // Copy context
        #if LIBAVCODEC_VERSION_MAJOR == 56 //ffmpeg 2.8
        // https://ffmpeg.org/doxygen/2.8/filtering_video_8c-example.html //in open_input_file
        pCodecCtx = pCodecCtxOrig;
        #elif LIBAVCODEC_VERSION_MAJOR == 570  //ffmpeg 3.2
        {
            AVCodecParameters *vparams;        
            pCodecCtx = avcodec_alloc_context3(pCodec);
            vparams = avcodec_parameters_alloc();
            avcodec_parameters_from_context(vparams, pCodecCtxOrig);
            avcodec_parameters_to_context(pCodecCtx, vparams);
            avcodec_parameters_free(&vparams);
        }
        #elif LIBAVCODEC_VERSION_MAJOR >= 57 //58 //ffmpeg 4.0 and 3.2 also runs here
        // https://ffmpeg.org/doxygen/4.0/filtering_video_8c-example.html  //in open_input_file
        pCodecCtx = avcodec_alloc_context3(pCodecCtxOrig->codec);
        avcodec_parameters_to_context(pCodecCtx, pFormatCtx->streams[videoStream]->codecpar);
        av_opt_set_int(pCodecCtx, "refcounted_frames", 1, 0);
        #endif
        // Open codec

        if(avcodec_open2(pCodecCtx, pCodec, NULL)<0)
            return -1; // Could not open codec
        //fw_movie.pVideoCodecCtx = pCodecCtx;


        // Allocate video frame
        pFrame=av_frame_alloc();

        // Allocate an AVFrame structure
        pFrameRGB=av_frame_alloc();
        if(pFrameRGB==NULL)
            return -1;

        // Determine required buffer size and allocate buffer
        if(0){
            nchan = 3;
            av_pix_fmt = AV_PIX_FMT_RGB24;
        }else{
            nchan = 4;
            av_pix_fmt = AV_PIX_FMT_RGBA;
        }

        fw_movie.nchan = nchan; //RGB24 == 3, RGBA == 4
        fw_movie.width = pCodecCtx->width;
        fw_movie.height = pCodecCtx->height;

        //numBytes=avpicture_get_size(av_pix_fmt, pCodecCtx->width,  //AV_PIX_FMT_RGB24, AV_PIX_FMT_RGBA
        //                            pCodecCtx->height);
        numBytes = av_image_get_buffer_size(av_pix_fmt, pCodecCtx->width, pCodecCtx->height,1); //in ffmpeg code I see 1, 16, 32 for align
        buffer=(uint8_t *)av_malloc(numBytes*sizeof(uint8_t));


        // Assign appropriate parts of buffer to image planes in pFrameRGB
        // Note that pFrameRGB is an AVFrame, but AVFrame is a superset
        // of AVPicture
        //avpicture_fill((AVPicture *)pFrameRGB, buffer,av_pix_fmt, //AV_PIX_FMT_RGBA, //AV_PIX_FMT_RGB24,
        //    pCodecCtx->width, pCodecCtx->height);
        av_image_fill_arrays(pFrameRGB->data,pFrameRGB->linesize,buffer,av_pix_fmt,pCodecCtx->width, pCodecCtx->height,1);

        // initialize SWS context for software scaling
        sws_ctx = sws_getContext(pCodecCtx->width,
            pCodecCtx->height,
            pCodecCtx->pix_fmt,
            pCodecCtx->width,
            pCodecCtx->height,
            av_pix_fmt, //AV_PIX_FMT_RGBA, //AV_PIX_FMT_RGB24,
            SWS_BILINEAR,
            NULL,
            NULL,
            NULL
            );

        //if( METHOD_DECODE_ON_LOAD ) - decodes all frames in resource thread when loading the file
        fw_framequeue = newStack(unsigned char *); //I like stack because stack_push will realloc
    }

    //video and audo decoded in combined loop (could split for decode-on-load)
    i=0;
    while(av_read_frame(pFormatCtx, &packet)>=0) {
        // Is this a packet from the video stream?
        if(packet.stream_index==videoStream) {
            // Decode video frame
            #if LIBAVCODEC_VERSION_MAJOR < 57
            // ffmpeg 2.8
            avcodec_decode_video2(pCodecCtx, pFrame, &frameFinished, &packet);
            #else
            // ffmpeg 3.2 - 4.0
            avcodec_send_packet(pCodecCtx,&packet);
            frameFinished = avcodec_receive_frame(pCodecCtx,pFrame) == 0? TRUE : FALSE;
            #endif
            // Did we get a video frame?
            if(frameFinished) {
                // Convert the image from its native format to RGB
                unsigned char * fw_frame;
                int k;
                sws_scale(sws_ctx, (uint8_t const * const *)pFrame->data,
                    pFrame->linesize, 0, pCodecCtx->height,
                    pFrameRGB->data, pFrameRGB->linesize);
    
                // Save the frame to disk
                ++i;
                //if(++i<=5)
                if(0) if(i<=5){
                    SaveFrame(pFrameRGB, pCodecCtx->width, 
                            pCodecCtx->height, nchan, i);
                }
                //printf("saving frame %d %d %d\n",pCodecCtx->width,pCodecCtx->height, i);
                //printf("linesize = %d \n",pFrameRGB->linesize[0]);

                fw_frame = (unsigned char*) malloc(fw_movie.height * fw_movie.width *  nchan); //assumes width == linesize[0]
                
                for(k=0;k<pCodecCtx->height;k++){
                    int kd,ks,kk;
                    unsigned char *src;
                    kk = pCodecCtx->height - k - 1; //flip y-down to y-up for opengl
                    ks = k*pFrame->linesize[0]*nchan;
                    kd = kk * fw_movie.width * nchan;
                    src = ((unsigned char *)pFrameRGB->data[0]) + ks;
                    memcpy(&fw_frame[kd],src,fw_movie.width * nchan);
                }
                stack_push(unsigned char *,fw_framequeue,fw_frame);
            }
            //av_free_packet(&packet);
        } else if(packet.stream_index==audioStream) {
            // http://open-activewrl.sourceforge.net/data/OpenAL_PGuide.pdf
            // page 5:
            // "Fill the buffers with PCM data using alBufferData."
            // alBufferData(g_Buffers[0],format,data,size,freq); 
            // Goal: PCM data
            // taking code from decode_audio_frame in ffmpeg tutorial03.c
            int buf_size;
            int got_frame = 0;
            int data_size = 0;
            #if LIBAVCODEC_VERSION_MAJOR < 57
            // ffmpeg 2.8
            int len1;
            len1 = avcodec_decode_audio4(aCodecCtx, aFrame, &got_frame, &packet);
            #else //3.2 - 4.0
            avcodec_send_packet(aCodecCtx, &packet);
            got_frame = avcodec_receive_frame(aCodecCtx, aFrame) == 0 ? TRUE : FALSE;
            #endif
            buf_size = audio_buf_size - audio_buf_index;
            if(got_frame) {
                //aFrameOut->format = aCodecCtx->sample_fmt;
                if(aFrame->nb_samples > 0){
                    data_size = av_samples_get_buffer_size(NULL, 
                                            aFrame->channels, //aCodecCtx->channels,
                                            aFrame->nb_samples,
                                            (AVSampleFormat)aFrame->format, //AV_SAMPLE_FMT_S16P, //aCodecCtx->sample_fmt,
                                            1);
                    //printf("aCodecCtx->sample_fmt= %d channels=%d samples=%d",aCodecCtx->sample_fmt,aCodecCtx->channels,aFrame->nb_samples);
                    //if this chunk's reformatted output will be bigger than the room we have left
                    // in our allocated big audio buffer, then realloc the big audio buffer * 2
                    if(data_size * 2 > buf_size){
                        audio_buf = (uint8_t*)realloc(audio_buf,audio_buf_size *2);
                        audio_buf_size *= 2;
                    }
                    if (do_audio_resample) //TRUE && aCodecCtx->sample_fmt == AV_SAMPLE_FMT_FLTP)
                    {
                        if(0){
                            //hand-coded FLTP to S16 
                            // works with apple1984veryshort.mp4 on win32 openAL, but not generally trusted, just as hacker code
                            //http://stackoverflow.com/questions/14989397/how-to-convert-sample-rate-from-av-sample-fmt-fltp-to-av-sample-fmt-s16
                            int i,c;
                            int nb_samples = aFrame->nb_samples;
                            int channels = aFrame->channels;
                            int outputBufferLen = nb_samples * channels * 2;
                            short* outputBuffer = (short*)&audio_buf[audio_buf_index];

                            for (i = 0; i < nb_samples; i++)
                            {
                                 for (c = 0; c < channels; c++)
                                 {
                                     float* extended_data = (float*)aFrame->extended_data[c];
                                     float sample = extended_data[i];
                                     if (sample < -1.0f) sample = -1.0f;
                                     else if (sample > 1.0f) sample = 1.0f;
                                     outputBuffer[i * channels + c] = (short)fwroundf(sample * 32767.0f);
                                 }
                            }
                            audio_buf_index += outputBufferLen;
                        }
                        else if(1){
                            //swresample module > swr_convert - works uwp and win32
                            //should convert non PCM 16 formats to PCM 16bit/channel stereo, 44100Hz
                            uint8_t *output;
                            int in_samples = aFrame->nb_samples;

                            int out_samples = (int)av_rescale_rnd(swr_get_delay(swr, aCodecCtx->sample_rate) + in_samples, 44100, aCodecCtx->sample_rate, AV_ROUND_UP);
                            av_samples_alloc(&output, NULL, 2, out_samples,    AV_SAMPLE_FMT_S16, 0);
                            out_samples = swr_convert(swr,(uint8_t**)&output,out_samples, (const uint8_t**)aFrame->extended_data, aFrame->nb_samples);  
                            memcpy(&audio_buf[audio_buf_index],output, out_samples * 2 * 2);
                            audio_buf_index +=  out_samples * 2 * 2;
                            av_freep(&output);
                        }
                    }else{
                        //works when incoming audio is already in s16 format and decoder doesn't change it
                        //ie mpgsys.mpg
                        //(but for mp4 audio, libav gives FLP/float format, and using this simple
                        // memcpy it comes out junk/noise in openAL H: openal can't handle float, just s16)
                        memcpy(&audio_buf[audio_buf_index], aFrame->data[0], data_size);
                        audio_buf_index += data_size;
                    }
                }
            }

        } else {
            // Free the packet that was allocated by av_read_frame
            //av_free_packet(&packet);
        }
    }

    //video fin
    if(videoStream > -1){
        fw_movie.frames = (unsigned char**)fw_framequeue->data;
        fw_movie.nframes = fw_framequeue->n;
        fw_movie.duration = (double)(fw_movie.nframes) / 30.0; //s = frames / fps 

/*
        if(0){
            //write out frames in .web3dit image format for testing
            int k;
            textureTableIndexStruct_s ttipp, *ttip;
            ttip = &ttipp;
            ttip->x = fw_movie.width;
            ttip->y = fw_movie.height;
            ttip->z = 1;
            ttip->hasAlpha = 1;
            ttip->channels = nchan;

            for(k=0;k<fw_movie.nframes;k++){
                char namebuf[100];
                ttip->texdata = fw_movie.frames[k];
                sprintf(namebuf,"%s%d.web3dit","ffmpeg_frame_",k);
                saveImage_web3dit(ttip, namebuf);
            }
        }
*/
        //IF(METHOD_DECODE_ON_LOAD)
        //   GARBAGE COLLECT FFMPEG STUFF
        // Free the RGB image
        av_free(buffer);
        av_frame_free(&pFrameRGB);
  
        // Free the YUV frame
        av_frame_free(&pFrame);
  
        // Close the codecs
        avcodec_close(pCodecCtx);
        avcodec_close(pCodecCtxOrig);
    }
    //audio fin
    if(audioStream > -1){
        fw_movie.audio_buf = audio_buf;
        fw_movie.audio_buf_size = audio_buf_index;
        fw_movie.duration = (double)(fw_movie.nframes) / 30.0; //s = frames / fps 

        avcodec_close(aCodecCtxOrig);
        avcodec_close(aCodecCtx);
    }

    //audio and video fin
        // Close the video file
        avformat_close_input(&pFormatCtx);
        *opaque = malloc(sizeof(struct fw_movietexture));
        memcpy(*opaque,&fw_movie,sizeof(struct fw_movietexture));
    

    return 1;
}
double movie_get_duration(void *opaque){
    struct fw_movietexture *fw_movie = (struct fw_movietexture *)opaque;
    return fw_movie->duration;
}

unsigned char *movie_get_frame_by_fraction(void *opaque, float fraction, int *width, int *height, int *nchan){
    int iframe;
    struct fw_movietexture *fw_movie = (struct fw_movietexture *)opaque;
    if(!fw_movie) return NULL;

    iframe = (int)(fraction * ((float)(fw_movie->nframes -1) + .5f));
    iframe = max(0,iframe);
    iframe = min(fw_movie->nframes -1,iframe);
    *width = fw_movie->width;
    *height = fw_movie->height;
    *nchan = fw_movie->nchan;
    return fw_movie->frames[iframe];
}
unsigned char * movie_get_audio_PCM_buffer(void *opaque,int *freq, int *channels, int *size, int *bits){
    struct fw_movietexture *fw_movie = (struct fw_movietexture *)opaque;
    if(!fw_movie) return NULL;
    if(!fw_movie->audio_buf) return NULL;
    *freq = fw_movie->freq;
    *channels = fw_movie->channels;
    *size = fw_movie->audio_buf_size;
    *bits = fw_movie->bits_per_channel;
    return fw_movie->audio_buf;
}
void movie_free(void *opaque){
    struct fw_movietexture *fw_movie = (struct fw_movietexture *)opaque;
    if(fw_movie) {
        int k;
        for(k=0;k<fw_movie->nframes;k++){
            FREE_IF_NZ(fw_movie->frames[k]);
        }
        free(opaque);
    }
}

void    
NodeMovieTexture::bind()
{
    if (!m_loaded) {
        movie_load(this);
        m_loaded = true;
        m_startTime = swGetCurrentTime();

        if (m_textureName != 0) glDeleteTextures(1, &m_textureName);
        m_textureName = 0;
    }

    if (m_textureName == 0) {
        glGenTextures(1, &m_textureName);
    }

    struct fw_movietexture *fw_movie = (struct fw_movietexture *)m_fw_movie;

    float fraction = fmod(swGetCurrentTime() - m_startTime, fw_movie->duration);

    if (!loop()->getValue())
        fraction = 0;
    
    int nchan = fw_movie->nchan; //RGB24 == 3, RGBA == 4
    int width = fw_movie->width;
    int height = fw_movie->height;

    m_isTransparent = (nchan == 4);                                          
    m_glColorMode = (nchan == 4) ? GL_RGBA : GL_RGB;     

    unsigned char *frame = movie_get_frame_by_fraction(fw_movie, fraction, 
                                                       &width, &height, &nchan);
    glEnable(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, m_textureName);    

    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(GL_TEXTURE_2D, 0, m_glColorMode, width, height, 0,
                 m_glColorMode, GL_UNSIGNED_BYTE, frame);
    GLenum error=glGetError();
    if (error!=0) {
        /* what's wrong here with "invalid value" ? */
        fprintf(stderr,"GL Error: %s\n",gluErrorString(error));
    }    

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, 
                    repeatS()->getValue() ? GL_REPEAT : GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, 
                    repeatT()->getValue() ? GL_REPEAT : GL_CLAMP);

}  

void    
NodeMovieTexture::unbind()
{
    glDisable(GL_TEXTURE_2D);
}

#endif
