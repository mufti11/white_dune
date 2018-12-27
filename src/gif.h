/* #ident       "@(#)x11:contrib/clients/xloadimage/gif.h 6.9 93/07/23 Labtam" */
/* gif.h:
 *
 * gifin.h
 * kirk johnson
 * november 1989
 * external interface to gifin.c
 *
 * Copyright 1989 Kirk L. Johnson (see the included file
 * "kljcpyrght.h" for complete copyright information)
 */

#ifdef __cplusplus
extern "C" {
#endif

/*
 * gifin return codes
 */
#define GIFIN_SUCCESS       0   /* success */
#define GIFIN_DONE          1   /* no more images */

#define GIFIN_ERR_BAD_SD   -1   /* bad screen descriptor */
#define GIFIN_ERR_BAD_SEP  -2   /* bad image separator */
#define GIFIN_ERR_BAD_SIG  -3   /* bad signature */
#define GIFIN_ERR_EOD      -4   /* unexpected end of raster data */
#define GIFIN_ERR_EOF      -5   /* unexpected end of input stream */
#define GIFIN_ERR_FAO      -6   /* file already open */
#define GIFIN_ERR_IAO      -7   /* image already open */
#define GIFIN_ERR_NFO      -8   /* no file open */
#define GIFIN_ERR_NIO      -9   /* no image open */
#define GIFIN_ERR_PSO      -10  /* pixel stack overflow */
#define GIFIN_ERR_TAO      -11  /* table overflow */
#define GIFIN_ERR_BAD_DES  -12  /* bad image descriptor */

#define GIFIN_ERR_BAD_SD_STR   "Bad screen descriptor"
#define GIFIN_ERR_BAD_SEP_STR  "Bad image separator"
#define GIFIN_ERR_BAD_SIG_STR  "Bad signature"
#define GIFIN_ERR_EOD_STR      "Unexpected end of raster data"
#define GIFIN_ERR_EOF_STR      "Unexpected end of input stream"
#define GIFIN_ERR_FAO_STR      "File already open"
#define GIFIN_ERR_IAO_STR      "Image already open"
#define GIFIN_ERR_NFO_STR      "No file open"
#define GIFIN_ERR_NIO_STR      "No image open"
#define GIFIN_ERR_PSO_STR      "Pixel stack overflow"
#define GIFIN_ERR_TAO_STR      "Table overflow"
#define GIFIN_ERR_BAD_DES_STR  "Bad image descriptor"

typedef struct {
    int err_no;
    const char *name;
    } gif_err_string;

#ifdef GIF_C
gif_err_string gif_err_strings[] = {
        {GIFIN_ERR_BAD_SD, GIFIN_ERR_BAD_SD_STR},
        {GIFIN_ERR_BAD_SEP, GIFIN_ERR_BAD_SEP_STR},
        {GIFIN_ERR_BAD_SIG, GIFIN_ERR_BAD_SIG_STR},
        {GIFIN_ERR_EOD, GIFIN_ERR_EOD_STR},
        {GIFIN_ERR_EOF, GIFIN_ERR_EOF_STR},
        {GIFIN_ERR_FAO, GIFIN_ERR_FAO_STR},
        {GIFIN_ERR_IAO, GIFIN_ERR_IAO_STR},
        {GIFIN_ERR_NFO, GIFIN_ERR_NFO_STR},
        {GIFIN_ERR_NIO, GIFIN_ERR_NIO_STR},
        {GIFIN_ERR_PSO, GIFIN_ERR_PSO_STR},
        {GIFIN_ERR_TAO, GIFIN_ERR_TAO_STR},
        {GIFIN_ERR_BAD_DES, GIFIN_ERR_BAD_DES_STR},
        {0}
    };
#endif

/*
 * colormap indices 
 */

#define GIF_RED  0
#define GIF_GRN  1
#define GIF_BLU  2

/* #defines, typedefs, and such
 */

#define GIF_SIG      "GIF87a"
#define GIF_SIG_89   "GIF89a"

#define GIF87a 0                /* Gif file version type */
#define GIF89a 1                /* Gif file version type */

#define GIF_SIG_LEN  6          /* GIF signature length */
#define GIF_SD_SIZE  7          /* GIF screen descriptor size */
#define GIF_ID_SIZE  9          /* GIF image descriptor size */

#define GIF_SEPARATOR   ','     /* GIF image separator */
#define GIF_EXTENSION   '!'     /* GIF extension block marker */
#define GIF_TERMINATOR  ';'     /* GIF terminator */

#define STAB_SIZE  4096         /* string table size */
#define PSTK_SIZE  4096         /* pixel stack size */

#define NULL_CODE  -1           /* string table null code */

#ifdef GIF_C
const char *gif_version_name[] = {
    GIF_SIG,
    GIF_SIG_89
    };
#endif

extern int gifOpen(const char *filename, int *width, int *height);
extern void gifRead(unsigned char *data);
       

#ifdef __cplusplus
}
#endif

