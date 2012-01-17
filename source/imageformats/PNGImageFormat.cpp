

#include "PNGImageFormat.h"


#include "../libs/libpng/png.h"
#include "../gl/axgl.h"
#include <cstdlib>
#include "../CFile.h"

#include "../CTexture.h"
#include <iostream>

PNGImageFormat * PNGImageFormat::instance_=0;


//Loader Section
struct gl_texture_t
{
  GLsizei width;
  GLsizei height;

  GLenum format;
  GLint internalFormat;
  GLuint id;

  GLubyte *texels;
};


static void
GetPNGtextureInfo (int color_type, struct gl_texture_t *texinfo)
{
  switch (color_type)
    {
    case PNG_COLOR_TYPE_GRAY:
      texinfo->format = GL_LUMINANCE;
      texinfo->internalFormat = 1;
      break;

    case PNG_COLOR_TYPE_GRAY_ALPHA:
      texinfo->format = GL_LUMINANCE_ALPHA;
      texinfo->internalFormat = 2;
      break;

    case PNG_COLOR_TYPE_RGB:
      texinfo->format = GL_RGB;
      texinfo->internalFormat = 3;
      break;

    case PNG_COLOR_TYPE_RGB_ALPHA:
      texinfo->format = GL_RGBA;
      texinfo->internalFormat = 4;
      break;

    default:
      /* Badness */
      break;
    }
}

/* File buffer struct */
struct file_buffer_t
{
  char name[256];
  unsigned char *data;
  long length;
  long offset;
};

static void
png_read_from_mem (png_structp png_ptr, png_bytep data, png_size_t length)
{
  struct file_buffer_t *src
    = (struct file_buffer_t *)png_get_io_ptr (png_ptr);

  /* Copy data from image buffer */
  memcpy (data, src->data + src->offset, length);

  /* Advance in the file */
  src->offset += length;
}

static void
png_error_fn (png_structp png_ptr, png_const_charp error_msg)
{
  fprintf (stderr, "png_error: %s (%s)\n", error_msg,
	   (char *)png_get_error_ptr (png_ptr));

  longjmp (png_jmpbuf (png_ptr), 1);
}

static void
png_warning_fn (png_structp png_ptr, png_const_charp warning_msg)
{
  fprintf (stderr, "png_warning: %s (%s)\n", warning_msg,
	   (char *)png_get_error_ptr (png_ptr));
}

static struct gl_texture_t *
ReadPNGFromMemory (const struct file_buffer_t *file)
{
  struct gl_texture_t *texinfo;
  png_structp png_ptr;
  png_infop info_ptr;
  int bit_depth, color_type;
  png_bytep *row_pointers = NULL;
  png_uint_32 w, h;
  int i;

  /* Check for valid magic number */
  if (!png_check_sig (file->data, 8))
    {
      fprintf (stderr, "error: \"%s\" is not a valid PNG image!\n",
	       file->name);
      return NULL;
    }

  /* Create a png read struct */
  png_ptr = png_create_read_struct (PNG_LIBPNG_VER_STRING,
	(png_voidp *)file->name, png_error_fn, png_warning_fn);
  if (!png_ptr)
    return NULL;

  /* Create a png info struct */
  info_ptr = png_create_info_struct (png_ptr);
  if (!info_ptr)
    {
      png_destroy_read_struct (&png_ptr, NULL, NULL);
      return NULL;
    }

  /* Create our OpenGL texture object */
  texinfo = (struct gl_texture_t *)
    malloc (sizeof (struct gl_texture_t));

  /* Initialize the setjmp for returning properly after a libpng
     error occured */
  if (setjmp (png_jmpbuf (png_ptr)))
    {
      png_destroy_read_struct (&png_ptr, &info_ptr, NULL);

      if (row_pointers)
	free (row_pointers);

      if (texinfo)
	{
	  if (texinfo->texels)
	    free (texinfo->texels);

	  free (texinfo);
	}

      return NULL;
    }

  /* Set "png_read" callback function and give source of data */
  png_set_read_fn (png_ptr, (png_voidp *)file, png_read_from_mem);

  /* Read png info */
  png_read_info (png_ptr, info_ptr);

  /* Get some usefull information from header */
  bit_depth = png_get_bit_depth (png_ptr, info_ptr);
  color_type = png_get_color_type (png_ptr, info_ptr);

  /* Convert index color images to RGB images */
  if (color_type == PNG_COLOR_TYPE_PALETTE)
    png_set_palette_to_rgb (png_ptr);

  /* Convert 1-2-4 bits grayscale images to 8 bits
     grayscale. */
  if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8)
    png_set_gray_to_rgb (png_ptr);

  if (png_get_valid (png_ptr, info_ptr, PNG_INFO_tRNS))
    png_set_tRNS_to_alpha (png_ptr);

  if (bit_depth == 16)
    png_set_strip_16 (png_ptr);
  else if (bit_depth < 8)
    png_set_packing (png_ptr);

  /* Update info structure to apply transformations */
  png_read_update_info (png_ptr, info_ptr);

  /* Retrieve updated information */
  png_get_IHDR (png_ptr, info_ptr, &w, &h, &bit_depth,
		&color_type, NULL, NULL, NULL);
  texinfo->width = w;
  texinfo->height = h;

  /* Get image format and components per pixel */
  GetPNGtextureInfo (color_type, texinfo);

  /* We can now allocate memory for storing pixel data */
  texinfo->texels = (GLubyte *)malloc (sizeof (GLubyte) * texinfo->width
	       * texinfo->height * texinfo->internalFormat);

  /* Setup a pointer array.  Each one points at the begening of a row. */
  row_pointers = (png_bytep *)malloc (sizeof (png_bytep) * texinfo->height);

  for (i = 0; i < texinfo->height; ++i)
    {
      row_pointers[i] = (png_bytep)(texinfo->texels +
	((texinfo->height - (i + 1)) * texinfo->width * texinfo->internalFormat));
    }

  /* Read pixel data using row pointers */
  png_read_image (png_ptr, row_pointers);

  /* Finish decompression and release memory */
  png_read_end (png_ptr, NULL);
  png_destroy_read_struct (&png_ptr, &info_ptr, NULL);

  /* Don't need row pointers anymore */
  free (row_pointers);

  return texinfo;
}

static struct gl_texture_t *
ReadPNGFromFile (const wchar_t *filename)
{
  struct gl_texture_t *texinfo;
  png_byte magic[8];
  png_structp png_ptr;
  png_infop info_ptr;
  int bit_depth, color_type;
  FILE *fp = NULL;
  png_bytep *row_pointers = NULL;
  png_uint_32 w, h;
  int i;

  /* Open image file */
  fp = axReadFile(filename);
  if (!fp)
    {
      fprintf (stderr, "error: couldn't open \"%s\"!\n", filename);
      return NULL;
    }

  /* Read magic number */
  fread (magic, 1, sizeof (magic), fp);

  /* Check for valid magic number */
  if (!png_check_sig (magic, sizeof (magic)))
    {
      fprintf (stderr, "error: \"%s\" is not a valid PNG image!\n",
	       filename);
      fclose (fp);
      return NULL;
    }

  /* Create a png read struct */
  png_ptr = png_create_read_struct
    (PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
  if (!png_ptr)
    {
      fclose (fp);
      return NULL;
    }

  /* Create a png info struct */
  info_ptr = png_create_info_struct (png_ptr);
  if (!info_ptr)
    {
      fclose (fp);
      png_destroy_read_struct (&png_ptr, NULL, NULL);
      return NULL;
    }

  /* Create our OpenGL texture object */
  texinfo = (struct gl_texture_t *)
    malloc (sizeof (struct gl_texture_t));

  /* Initialize the setjmp for returning properly after a libpng
     error occured */

     /*
  if (setjmp (png_jmpbuf (png_ptr)))
    {
      fclose (fp);
      png_destroy_read_struct (&png_ptr, &info_ptr, NULL);

      if (row_pointers)
	free (row_pointers);

      if (texinfo)
	{
	  if (texinfo->texels)
	    free (texinfo->texels);

	  free (texinfo);
	}

      return NULL;
    }
*/

  /* Setup libpng for using standard C fread() function
     with our FILE pointer */
  png_init_io (png_ptr, fp);

  /* Tell libpng that we have already read the magic number */
  png_set_sig_bytes (png_ptr, sizeof (magic));

  /* Read png info */
  png_read_info (png_ptr, info_ptr);

  /* Get some usefull information from header */
  bit_depth = png_get_bit_depth (png_ptr, info_ptr);
  color_type = png_get_color_type (png_ptr, info_ptr);

  /* Convert index color images to RGB images */
  if (color_type == PNG_COLOR_TYPE_PALETTE)
    png_set_palette_to_rgb (png_ptr);

  /* Convert 1-2-4 bits grayscale images to 8 bits
     grayscale. */
  if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8)
    png_set_expand_gray_1_2_4_to_8 (png_ptr);

  if (png_get_valid (png_ptr, info_ptr, PNG_INFO_tRNS))
    png_set_tRNS_to_alpha (png_ptr);

  if (bit_depth == 16)
    png_set_strip_16 (png_ptr);
  else if (bit_depth < 8)
    png_set_packing (png_ptr);

  /* Update info structure to apply transformations */
  png_read_update_info (png_ptr, info_ptr);

  /* Retrieve updated information */
  png_get_IHDR (png_ptr, info_ptr, &w, &h, &bit_depth,
		&color_type, NULL, NULL, NULL);
  texinfo->width = w;
  texinfo->height = h;

  /* Get image format and components per pixel */
  GetPNGtextureInfo (color_type, texinfo);

  /* We can now allocate memory for storing pixel data */
  texinfo->texels = (GLubyte *)malloc (sizeof (GLubyte) * texinfo->width
	       * texinfo->height * texinfo->internalFormat);

  /* Setup a pointer array.  Each one points at the begening of a row. */
  row_pointers = (png_bytep *)malloc (sizeof (png_bytep) * texinfo->height);

  for (i = 0; i < texinfo->height; ++i)
    {
#if 1
      row_pointers[i] = (png_bytep)(texinfo->texels +
	((texinfo->height - (i + 1)) * texinfo->width * texinfo->internalFormat));
#else
      row_pointers[i] = (png_bytep)(texinfo->texels +
	 (texinfo->width * i * texinfo->internalFormat));
#endif
    }

  /* Read pixel data using row pointers */
  png_read_image (png_ptr, row_pointers);

  /* Finish decompression and release memory */
  png_read_end (png_ptr, NULL);
  png_destroy_read_struct (&png_ptr, &info_ptr, NULL);

  /* We don't need row pointers anymore */
  free (row_pointers);

  fclose (fp);
  return texinfo;
}
//Wrapper Section
PNGImageFormat::PNGImageFormat()
{
    AllFormats[L"png"] = this;
    //ctor
}

PNGImageFormat::~PNGImageFormat()
{
    AllFormats[L"png"] = 0;
    //dtor
}

PNGImageFormat* PNGImageFormat::instance()
{
    if(!instance_)
    {
        instance_ = new PNGImageFormat();
    }

    return instance_;
}

axelynx::PixMap* PNGImageFormat::LoadPixMap(axelynx::File filename)
{
    gl_texture_t *t=0;

	t = ReadPNGFromFile(filename.GetFilename().c_str());

    axelynx::PixMap *pm = axelynx::PixMap::Create(t->width,t->height,t->internalFormat);

    memcpy(pm->Data(),t->texels,t->width*t->height*t->internalFormat);

    return pm;
}

bool PNGImageFormat::SavePixMap(axelynx::File filename, const axelynx::PixMap *pixmap)
{
    return false;
}

axelynx::Texture* PNGImageFormat::LoadTexture(axelynx::File filename,axelynx::Texture::Desc &desc)
{
    gl_texture_t *t = 0;

	if(filename.GetFilename() != L"")
	{
		t = ReadPNGFromFile (filename.GetFilename().c_str());
	}
	else
	{
		file_buffer_t buff;
		buff.data = static_cast<unsigned char*>(filename.GetPointer());
		buff.length = filename.Size();
		buff.offset = 0;
		strcpy(buff.name,"memory file");

		t = ReadPNGFromMemory(&buff);
	}
	std::cout<<"if:"<<t->internalFormat<<std::endl;

	GLuint format = 0;
	GLuint internalFormat = 0;
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
	};


	axelynx::Texture::Desc desc2 = desc;
	desc2.width = t->width;
	desc2.height = t->height;
	desc2.cpp = t->internalFormat;

	CTexture *tex = new CTexture(desc2);

	tex->Build(t->texels,format,internalFormat);

    return tex;
}

bool PNGImageFormat::SaveTexture(axelynx::File filename, const axelynx::Texture *texture)
{
    return false;
}