/* #ident   "@(#)x11:contrib/clients/xloadimage/gif.c 6.16 94/07/29 Labtam" */
/* gif.c:
 *
 * adapted from code by kirk johnson (tuna@athena.mit.edu).  most of this
 * code is unchanged. -- jim frost 12.31.89
 *
 * gifin.c
 * kirk johnson
 * november 1989
 *
 * routines for reading GIF files
 *
 * Copyright 1989 Kirk L. Johnson (see the included file
 * "kljcpyrght.h" for complete copyright information)
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define GIF_C
#include "kljcpyrght.h"
#include "gif.h"

/*
#include "stdafx.h"
*/

/*
 * typedef BYTE for convenience
 */

typedef unsigned char BYTE;

static int gifin_open_file();
static int gifin_open_image();
static int gifin_get_pixel();
static int gifin_close_file();
static int gifin_load_cmap(BYTE cmap [3][256], int ncolors);
static int gifin_skip_extension();
static int gifin_read_data_block();
static int gifin_add_string(int p, int e);

/****
 **
 ** local #defines
 **
 ****/

#define PUSH_PIXEL(p)                                       \
{                                                           \
  if (pstk_idx == PSTK_SIZE)                                \
    return GIFIN_ERR_PSO;                                   \
  else                                                      \
    pstk[pstk_idx++] = (p);                                 \
}

/*
 * push a string (denoted by a code) onto the pixel stack
 * (returns the code of the first pixel in the string in ps_rslt)
 */

int ps_rslt;    /* return result */

#define GIFIN_PUSH_STRING(code)                             \
{                                                           \
  int ps_code;                                              \
  ps_code = code;                                           \
                                                            \
  while (((unsigned int)ps_code) < STAB_SIZE && prefix[ps_code] != NULL_CODE) \
  {                                                         \
    PUSH_PIXEL(extnsn[ps_code]);                            \
    ps_code = prefix[ps_code];                              \
  }                                                         \
                                                            \
  if (((unsigned int)ps_code) >= STAB_SIZE)                                 \
    return GIFIN_ERR_TAO;                                   \
  PUSH_PIXEL(extnsn[ps_code]);                              \
  ps_rslt = extnsn[ps_code];                                \
  if (((unsigned int)ps_rslt) >= STAB_SIZE)                                 \
    return GIFIN_ERR_TAO;                                   \
}

/****
 **
 ** local variables
 **
 ****/

static int interlace_start[4]= { /* start line for interlacing */
  0, 4, 2, 1
};

static int interlace_rate[4]= { /* rate at which we accelerate vertically */
  8, 8, 4, 2
};

static BYTE file_open  = 0;     /* status flags */
static BYTE image_open = 0;

static FILE *ins;               /* input stream */

static int  root_size;          /* root code size */
static int  clr_code;           /* clear code */
static int  eoi_code;           /* end of information code */
static int  code_size;          /* current code size */
static int  code_mask;          /* current code mask */
static int  prev_code;          /* previous code */

/*
 * NOTE: a long is assumed to be at least 32 bits wide
 */
static long work_data;          /* working bit buffer */
static int  work_bits;          /* working bit count */

static BYTE buf[256];           /* byte buffer */
static int  buf_cnt;            /* byte count */
static int  buf_idx;            /* buffer index */

static int table_size;          /* string table size */
static int prefix[STAB_SIZE];   /* string table : prefixes */
static int extnsn[STAB_SIZE];   /* string table : extensions */

static BYTE pstk[PSTK_SIZE];    /* pixel stack */
static int  pstk_idx;           /* pixel stack pointer */


/****
 **
 ** global variables
 **
 ****/

static int  gifin_rast_width;          /* raster width */
static int  gifin_rast_height;         /* raster height */
static BYTE gifin_g_cmap_flag;         /* global colormap flag */
static int  gifin_g_pixel_bits;        /* bits per pixel, global colormap */
static int  gifin_g_ncolors;           /* number of colors, global colormap */
static BYTE gifin_g_cmap[3][256];      /* global colormap */
static BYTE gifin_g_cmap_sorted;       /* global colormap sorted (GIF89a only) */
static int  gifin_bg_color;            /* background color index */
static int  gifin_color_bits;          /* bits of color resolution */
static double  gifin_aspect;           /* pixel aspect ratio (width/height) */
static int  gifin_version;             /* gif file version */

static int  gifin_img_left;            /* image position on raster */
static int  gifin_img_top;             /* image position on raster */
static int  gifin_img_width;           /* image width */
static int  gifin_img_height;          /* image height */
static BYTE gifin_l_cmap_flag;         /* local colormap flag */
static int  gifin_l_pixel_bits;        /* bits per pixel, local colormap */
static int  gifin_l_ncolors;           /* number of colors, local colormap */
static BYTE gifin_l_cmap[3][256];      /* local colormap */
static BYTE gifin_interlace_flag;      /* interlace image format flag */

/*
 * open a GIF file, using s as the input stream
 */

static int gifin_open_file(FILE *s)
{
  int errorno;
  /* make sure there isn't already a file open */
  if (file_open)
    return GIFIN_ERR_FAO;

  /* remember that we've got this file open */
  file_open = 1;
  ins       = s;

  /* check GIF signature */
  if (fread(buf, 1, GIF_SIG_LEN, ins) != GIF_SIG_LEN)
    return GIFIN_ERR_EOF;

  buf[GIF_SIG_LEN] = '\0';
  if (strcmp((char *) buf, GIF_SIG) == 0)
    gifin_version = GIF87a;
  else if(strcmp((char *) buf, GIF_SIG_89) == 0)
    gifin_version = GIF89a;
  else
    return GIFIN_ERR_BAD_SIG;

  /* read screen descriptor */
  if (fread(buf, 1, GIF_SD_SIZE, ins) != GIF_SD_SIZE)
    return GIFIN_ERR_EOF;

  /* decode screen descriptor */
  gifin_rast_width   = (buf[1] << 8) + buf[0];
  gifin_rast_height  = (buf[3] << 8) + buf[2];
  gifin_g_cmap_flag  = (buf[4] & 0x80) ? 1 : 0;
  gifin_color_bits   = (((int)(buf[4] & 0x70)) >> 4) + 1;
  gifin_g_pixel_bits = (buf[4] & 0x07) + 1;
  gifin_bg_color     = buf[5];
  gifin_aspect = 1.0;

  if (gifin_version == GIF87a) {
    if (buf[4] & 0x08 || buf[6] != 0)
      return GIFIN_ERR_BAD_SD;
  } else {
    gifin_g_cmap_sorted = ((buf[4] & 0x08) != 0);
    if (buf[6] != 0)
      gifin_aspect = ((double)buf[6] + 15.0) / 64.0;
  }

  /* load global colormap */
  if (gifin_g_cmap_flag)
  {
    gifin_g_ncolors = (1 << gifin_g_pixel_bits);

    if ((errorno = gifin_load_cmap(gifin_g_cmap, gifin_g_ncolors)) != GIFIN_SUCCESS)
      return errorno;
  }
  else
  {
    gifin_g_ncolors = 0;
  }

  /* done! */
  return GIFIN_SUCCESS;
}


/*
 * open next GIF image in the input stream; returns GIFIN_SUCCESS if
 * successful. if there are no more images, returns GIFIN_DONE. (might
 * also return various GIFIN_ERR codes.)
 */

static int gifin_open_image(void)
{
  int i;
  int separator;
  int errorno;

  /* make sure there's a file open */
  if (!file_open)
    return GIFIN_ERR_NFO;

  /* make sure there isn't already an image open */
  if (image_open)
    return GIFIN_ERR_IAO;

  /* remember that we've got this image open */
  image_open = 1;

  /* skip over any extension blocks */
  do
  {
    separator = getc(ins);
    if (separator == GIF_EXTENSION)
    {
      if ((errorno = gifin_skip_extension()) != GIFIN_SUCCESS)
        return errorno;
    }
  }
  while (separator == GIF_EXTENSION);

  /* check for end of file marker */
  if (separator == GIF_TERMINATOR)
    return GIFIN_DONE;

  /* make sure we've got an image separator */
  if (separator != GIF_SEPARATOR)
    return GIFIN_ERR_BAD_SEP;

  /* read image descriptor */
  if (fread(buf, 1, GIF_ID_SIZE, ins) != GIF_ID_SIZE)
    return GIFIN_ERR_EOF;

  /* decode image descriptor */
  gifin_img_left       = (buf[1] << 8) + buf[0];
  gifin_img_top        = (buf[3] << 8) + buf[2];
  gifin_img_width      = (buf[5] << 8) + buf[4];
  gifin_img_height     = (buf[7] << 8) + buf[6];
  gifin_l_cmap_flag    = (buf[8] & 0x80) ? 1 : 0;
  gifin_interlace_flag = (buf[8] & 0x40) ? 1 : 0;
  gifin_l_pixel_bits   = (buf[8] & 0x07) + 1;

  /* load local colormap */
  if (gifin_l_cmap_flag)
  {
    gifin_l_ncolors = (1 << gifin_l_pixel_bits);

    if ((errorno = gifin_load_cmap(gifin_l_cmap, gifin_l_ncolors)) != GIFIN_SUCCESS)
      return errorno;
  }
  else
  {
    gifin_l_ncolors = 0;
  }

  /* initialize raster data stream decoder */
  root_size = getc(ins);
  clr_code  = 1 << root_size;
  eoi_code  = clr_code + 1;
  code_size = root_size + 1;
  code_mask = (1 << code_size) - 1;
  work_bits = 0;
  work_data = 0;
  buf_cnt   = 0;
  buf_idx   = 0;

  /* initialize string table */
  for (i=0; i<STAB_SIZE; i++)
  {
    prefix[i] = NULL_CODE;
    extnsn[i] = i;
  }

  /* initialize pixel stack */
  pstk_idx = 0;

  /* done! */
  return GIFIN_SUCCESS;
}

/*
 * try to read next pixel from the raster, return result in *pel
 */

static int gifin_get_pixel(int* pel)
{
  int  code;
  int  first;
  int  place;
  int  errorno;

  /* decode until there are some pixels on the pixel stack */
  while (pstk_idx == 0)
  {
    /* load bytes until we have enough bits for another code */
    while (work_bits < code_size)
    {
      if (buf_idx == buf_cnt)
      {
        /* read a new data block */
        if ((errorno = gifin_read_data_block()) != GIFIN_SUCCESS)
          return errorno;

        if (buf_cnt == 0)
          return GIFIN_ERR_EOD;
      }

      work_data |= ((long) buf[buf_idx++]) << work_bits;
      work_bits += 8;
    }

    /* get the next code */
    code        = work_data & code_mask;
    work_data >>= code_size;
    work_bits  -= code_size;

    /* interpret the code */
    if (code == clr_code)
    {
      /* reset decoder stream */
      code_size  = root_size + 1;
      code_mask  = (1 << code_size) - 1;
      prev_code  = NULL_CODE;
      table_size = eoi_code + 1;
    }
    else if (code == eoi_code)
    {
      /* Ooops! no more pixels */
      return GIFIN_ERR_EOF;
    }
    else if (prev_code == NULL_CODE)
    {
      GIFIN_PUSH_STRING(code);
      prev_code = code;
    }
    else
    {
      if (code < table_size)
      {
        GIFIN_PUSH_STRING(code);
        first = ps_rslt;
      }
      else
      {
        place = pstk_idx;
        PUSH_PIXEL(NULL_CODE);
        GIFIN_PUSH_STRING(prev_code);
        first = ps_rslt;
        pstk[place] = first;
      }

      if((errorno = gifin_add_string(prev_code, first)) != GIFIN_SUCCESS)
        return errorno;
      prev_code = code;
    }
  }

  /* pop a pixel off the pixel stack */
  *pel = (int) pstk[--pstk_idx];

  /* done! */
  return GIFIN_SUCCESS;
}

/*
 * close an open GIF file
 */

static int gifin_close_file()
{
  /* make sure there's a file open */
  if (!file_open)
    return GIFIN_ERR_NFO;

  /* mark file (and image) as closed */
  file_open  = 0;
  image_open = 0;

  /* done! */
  return GIFIN_SUCCESS;
}

/*
 * load a colormap from the input stream
 */

static int gifin_load_cmap(BYTE cmap [3][256], int ncolors)
{
  int i;

  for (i=0; i<ncolors; i++)
  {
    if (fread(buf, 1, 3, ins) != 3)
      return GIFIN_ERR_EOF;
    
    cmap[GIF_RED][i] = buf[GIF_RED];
    cmap[GIF_GRN][i] = buf[GIF_GRN];
    cmap[GIF_BLU][i] = buf[GIF_BLU];
  }

  /* done! */
  return GIFIN_SUCCESS;
}
 
/*
 * skip an extension block in the input stream
 */

static int gifin_skip_extension()
{
  int errorno;

  /* get the extension function byte */
  getc(ins);

  /* skip any remaining raster data */
  do
  {
    if ((errorno = gifin_read_data_block()) != GIFIN_SUCCESS)
      return errorno;
  }
  while (buf_cnt > 0);

  /* done! */
  return GIFIN_SUCCESS;
}

/*
 * read a new data block from the input stream
 */

static int gifin_read_data_block()
{
  /* read the data block header */
  buf_cnt = getc(ins);

  /* read the data block body */
  if (fread(buf, 1, buf_cnt, ins) != (size_t)buf_cnt)
    return GIFIN_ERR_EOF;

  buf_idx = 0;

  /* done! */
  return GIFIN_SUCCESS;
}


/*
 * add a new string to the string table
 */

static int gifin_add_string(int p, int e)
{
  prefix[table_size] = p;
  extnsn[table_size] = e;

  if ((table_size == code_mask) && (code_size < 12))
  {
    code_size += 1;
    code_mask  = (1 << code_size) - 1;
  }

  table_size += 1;
  if (table_size > STAB_SIZE)
    return GIFIN_ERR_TAO;
  return GIFIN_SUCCESS;
}

/* these are the routines added for interfacing to the Image class
 */

static FILE *zf;

int
gifOpen(const char *filename, int *width, int *height)
{
    if (!(zf = fopen (filename, "rb"))) {
        return 0;
    } else if ((gifin_open_file(zf) != GIFIN_SUCCESS) || /* read GIF header */
      (gifin_open_image() != GIFIN_SUCCESS)) {  /* read image header */
        gifin_close_file();
        fclose(zf);
        return 0;
    } else {
        *width = gifin_img_width;
        *height = gifin_img_height;
        return 1;
    }
}

void
gifRead(unsigned char *data)
{
    int x, y, pixel, pass;
    BYTE *red, *green, *blue;

    if (gifin_l_cmap_flag)
    {
        red = gifin_l_cmap[GIF_RED];
        green = gifin_l_cmap[GIF_GRN];
        blue = gifin_l_cmap[GIF_BLU];
    }
    else
    {
        red = gifin_g_cmap[GIF_RED];
        green = gifin_g_cmap[GIF_GRN];
        blue = gifin_g_cmap[GIF_BLU];
    }

    /* interlaced image -- futz with the vertical trace.  i wish i knew what
     * kind of drugs the GIF people were on when they decided that they
     * needed to support interlacing.
     */

    if (gifin_interlace_flag)
    {
        /* interlacing takes four passes to read, each starting at a different
         * vertical point.
         */

        for (pass = 0;pass < 4; pass++)
        {
            y = interlace_start[pass];

            while (y < gifin_img_height)
            {
                for (x= 0; x < gifin_img_width; x++)
                {
                    if (gifin_get_pixel(&pixel) != GIFIN_SUCCESS)
                    {
                        y = gifin_img_height;
                        x = gifin_img_width;
                    }
                    else
                    {
                        int invy = gifin_img_height - y - 1;
                        data[(invy * gifin_img_width + x) * 3] = red[pixel];
                        data[(invy * gifin_img_width + x) * 3 + 1] = green[pixel];
                        data[(invy * gifin_img_width + x) * 3 + 2] = blue[pixel];
                    } 
                }
                y += interlace_rate[pass];
            }
        }
    }

    /* not an interlaced image, just read in sequentially
     */

    else
    {
        for (y= 0; y < gifin_img_height; y++)
        {
            for (x= 0; x < gifin_img_width; x++)
            {
                if (gifin_get_pixel(&pixel) != GIFIN_SUCCESS)
                {
                    y = gifin_img_height;
                    x = gifin_img_width;
                }
                else
                {
                    int invy = gifin_img_height - y - 1;
                    data[(invy * gifin_img_width + x) * 3] = red[pixel];
                    data[(invy * gifin_img_width + x) * 3 + 1] = green[pixel];
                    data[(invy * gifin_img_width + x) * 3 + 2] = blue[pixel];
                }
            }
        }
    }
    gifin_close_file ();
    fclose (zf);
}

