

#include "JPEGImageFormat.h"
#include "../gl/axgl.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>




#include "../CFile.h"

#include "../CTexture.h"

#include "../libs/jpeg/jpeglib.h"
JPEGImageFormat * JPEGImageFormat::instance_=0;


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
};

static struct gl_texture_t *
ReadJPEGFromFile (const wchar_t *filename)
{
  struct gl_texture_t *texinfo = NULL;
  FILE *fp = NULL;
  struct jpeg_decompress_struct cinfo;
  struct jpeg_error_mgr jerr;
  JSAMPROW j;
  int i;

  /* Open image file */
  fp = axReadFile(filename);
  if (!fp)
    {
      fprintf (stderr, "error: couldn't open \"%s\"!\n", filename);
      return NULL;
    }


  /* Create and configure decompressor */
  jpeg_create_decompress (&cinfo);
  cinfo.err = jpeg_std_error (&jerr);
  jpeg_stdio_src (&cinfo, fp);

  /*
   * NOTE: this is the simplest "readJpegFile" function. There
   * is no advanced error handling.  It would be a good idea to
   * setup an error manager with a setjmp/longjmp mechanism.
   * In this function, if an error occurs during reading the JPEG
   * file, the libjpeg abords the program.
   * See jpeg_mem.c (or RTFM) for an advanced error handling which
   * prevent this kind of behavior (http://tfc.duke.free.fr)
   */

  /* Read header and prepare for decompression */
  jpeg_read_header (&cinfo, TRUE);
  jpeg_start_decompress (&cinfo);

  /* Initialize image's member variables */
  texinfo = (struct gl_texture_t *)
    malloc (sizeof (struct gl_texture_t));
  texinfo->width = cinfo.image_width;
  texinfo->height = cinfo.image_height;
  texinfo->internalFormat = cinfo.num_components;

  if (cinfo.num_components == 1)
    texinfo->format = GL_LUMINANCE;
  else
    texinfo->format = GL_RGB;

  texinfo->texels = (GLubyte *)malloc (sizeof (GLubyte) * texinfo->width
			       * texinfo->height * texinfo->internalFormat);

  /* Extract each scanline of the image */
  for (i = 0; i < texinfo->height; ++i)
    {
      j = (texinfo->texels +
	((texinfo->height - (i + 1)) * texinfo->width * texinfo->internalFormat));
      jpeg_read_scanlines (&cinfo, &j, 1);
    }

  /* Finish decompression and release memory */
  jpeg_finish_decompress (&cinfo);
  jpeg_destroy_decompress (&cinfo);

  fclose (fp);
  return texinfo;
}

//Wrapper Section
JPEGImageFormat::JPEGImageFormat()
{
    AllFormats[L"jpg"] = this;
    AllFormats[L"jpeg"] = this;
    //ctor
}

JPEGImageFormat::~JPEGImageFormat()
{
    AllFormats[L"jpg"] = 0;
    AllFormats[L"jpeg"] = 0;
    //dtor
}

JPEGImageFormat* JPEGImageFormat::instance()
{
    if(!instance_)
    {
        instance_ = new JPEGImageFormat();
    }

    return instance_;
}

axelynx::PixMap* JPEGImageFormat::LoadPixMap(axelynx::File filename)
{
    gl_texture_t *t = 0;

	t = ReadJPEGFromFile(filename.GetFilename().c_str());

    axelynx::PixMap *pm = axelynx::PixMap::Create(t->width,t->height,t->internalFormat);
    memcpy(pm->Data(),t->texels,t->width*t->height*t->internalFormat);
    return pm;
}

bool JPEGImageFormat::SavePixMap(axelynx::File filename, const axelynx::PixMap *pixmap)
{
    return false;
}

axelynx::Texture* JPEGImageFormat::LoadTexture(axelynx::File filename,axelynx::Texture::Desc &desc)
{
    gl_texture_t *t = 0;

	t = ReadJPEGFromFile (filename.GetFilename().c_str());

	GLuint format = GL_RGB;
	GLuint internalFormat = GL_RGB8;

	switch(t->internalFormat)
	{
	case 1:
		format = GL_RED;
		internalFormat = GL_R8;
	break;
	case 3:
		format = GL_RGB;
		internalFormat = GL_RGB8;
	break;
	case 4:
		format = GL_RGBA;
		internalFormat = GL_RGBA8;
	break;
	}


	axelynx::Texture::Desc desc2 = desc;
	desc2.width = t->width;
	desc2.height = t->height;
	desc2.cpp = t->internalFormat;

	CTexture *tex = new CTexture(desc2);
	tex->Build(t->texels,format,internalFormat);

    return tex;
}

bool JPEGImageFormat::SaveTexture(axelynx::File filename, const axelynx::Texture *texture)
{
    return false;
}