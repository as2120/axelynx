

#include "DDSImageFormat.h"

#include <cstdio>
#include <cstdlib>
#include <string>
#include <string.h>
#include "../gl/axgl.h"
#include "../CFile.h"

DDSImageFormat * DDSImageFormat::instance_=0;


//Loader Section

/* OpenGL texture info */
struct gl_texture_t
{
  GLsizei width;
  GLsizei height;

  GLenum format;
  GLint internalFormat;
  GLuint id;

  GLubyte *texels;

  GLint numMipmaps;
};

/* DirectDraw's structures */
struct DDPixelFormat
{
  GLuint size;
  GLuint flags;
  GLuint fourCC;
  GLuint bpp;
  GLuint redMask;
  GLuint greenMask;
  GLuint blueMask;
  GLuint alphaMask;
};

struct DDSCaps
{
  GLuint caps;
  GLuint caps2;
  GLuint caps3;
  GLuint caps4;
};

struct DDColorKey
{
  GLuint lowVal;
  GLuint highVal;
};

struct DDSurfaceDesc
{
  GLuint size;
  GLuint flags;
  GLuint height;
  GLuint width;
  GLuint pitch;
  GLuint depth;
  GLuint mipMapLevels;
  GLuint alphaBitDepth;
  GLuint reserved;
  GLuint surface;

  struct DDColorKey ckDestOverlay;
  struct DDColorKey ckDestBlt;
  struct DDColorKey ckSrcOverlay;
  struct DDColorKey ckSrcBlt;

  struct DDPixelFormat format;
  struct DDSCaps caps;

  GLuint textureStage;
};

#define MAKEFOURCC(ch0, ch1, ch2, ch3) \
  (GLuint)( \
    (((GLuint)(GLubyte)(ch3) << 24) & 0xFF000000) | \
    (((GLuint)(GLubyte)(ch2) << 16) & 0x00FF0000) | \
    (((GLuint)(GLubyte)(ch1) <<  8) & 0x0000FF00) | \
     ((GLuint)(GLubyte)(ch0)        & 0x000000FF) )

#define FOURCC_DXT1 MAKEFOURCC('D', 'X', 'T', '1')
#define FOURCC_DXT3 MAKEFOURCC('D', 'X', 'T', '3')
#define FOURCC_DXT5 MAKEFOURCC('D', 'X', 'T', '5')

static struct gl_texture_t *
ReadDDSFile (const wchar_t *filename)
{
  struct DDSurfaceDesc ddsd;
  struct gl_texture_t *texinfo;
  FILE *fp;
  char magic[4];
  int mipmapFactor;
  long bufferSize, curr, end;

  /* Open the file */
  fp = axReadFile(filename);
  if (!fp)
    {
      fprintf (stderr, "error: couldn't open \"%s\"!\n", filename);
      return NULL;
    }

  /* Read magic number and check if valid .dds file */
  fread (&magic, sizeof (char), 4, fp);

  if (strncmp (magic, "DDS ", 4) != 0)
    {
      fprintf (stderr, "the file \"%s\" doesn't appear to be"
	       "a valid .dds file!\n", filename);
      fclose (fp);
      return NULL;
    }

  /* Get the surface descriptor */
  fread (&ddsd, sizeof (ddsd), 1, fp);

  texinfo = (struct gl_texture_t *)
    calloc (sizeof (struct gl_texture_t), 1);
  texinfo->width = ddsd.width;
  texinfo->height = ddsd.height;
  texinfo->numMipmaps = ddsd.mipMapLevels;

  switch (ddsd.format.fourCC)
    {
    case FOURCC_DXT1:
      /* DXT1's compression ratio is 8:1 */
      texinfo->format = 1;//GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
      texinfo->internalFormat = 3;
      mipmapFactor = 2;
      break;

    case FOURCC_DXT3:
      /* DXT3's compression ratio is 4:1 */
      texinfo->format = 2;//GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
      texinfo->internalFormat = 4;
      mipmapFactor = 4;
      break;

    case FOURCC_DXT5:
      /* DXT5's compression ratio is 4:1 */
      texinfo->format = 3;//GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
      texinfo->internalFormat = 4;
      mipmapFactor = 4;
      break;

    default:
      /* Bad fourCC, unsupported or bad format */
      fprintf (stderr, "the file \"%s\" doesn't appear to be"
	       "compressed using DXT1, DXT3, or DXT5! [%i]\n",
	       filename, ddsd.format.fourCC);
      free (texinfo);
      fclose (fp);
      return NULL;
    }

  /* Calculate pixel data size */
  curr = ftell (fp);
  fseek (fp, 0, SEEK_END);
  end = ftell (fp);
  fseek (fp, curr, SEEK_SET);
  bufferSize = end - curr;

  /* Read pixel data with mipmaps */
  texinfo->texels = (GLubyte *)malloc (bufferSize * sizeof (GLubyte));
  fread (texinfo->texels, sizeof (GLubyte), bufferSize, fp);

  /* Close the file */
  fclose (fp);
  return texinfo;
}


//Wrapper Section
DDSImageFormat::DDSImageFormat()
{
    AllFormats[L"dds"] = this;
    //ctor
}

DDSImageFormat::~DDSImageFormat()
{
    AllFormats[L"dds"] = 0;
    //dtor
}

DDSImageFormat* DDSImageFormat::instance()
{
    if(!instance_)
    {
        instance_ = new DDSImageFormat();
    }

    return instance_;
}

axelynx::PixMap* DDSImageFormat::LoadPixMap(axelynx::File filename)
{
    gl_texture_t *t = 0;

	t = ReadDDSFile (filename.GetFilename().c_str());

    axelynx::PixMap *pm = axelynx::PixMap::Create(t->width,t->height,t->internalFormat);
    memcpy(pm->Data(),t->texels,t->width*t->height*t->internalFormat);
    return pm;
}

bool DDSImageFormat::SavePixMap(axelynx::File filename, const axelynx::PixMap *pixmap)
{
    return false;
}