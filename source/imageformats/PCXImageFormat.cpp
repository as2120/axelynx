
#include "PCXImageFormat.h"

PCXImageFormat * PCXImageFormat::instance_=0;

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "../gl/axgl.h"
#include "../CFile.h"

#include "../CTexture.h"

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

#pragma pack(push,1)
/* PCX header */
struct pcx_header_t
{
  GLubyte manufacturer;
  GLubyte version;
  GLubyte encoding;
  GLubyte bitsPerPixel;

  GLushort xmin, ymin;
  GLushort xmax, ymax;
  GLushort horzRes, vertRes;

  GLubyte palette[48];
  GLubyte reserved;
  GLubyte numColorPlanes;

  GLushort bytesPerScanLine;
  GLushort paletteType;
  GLushort horzSize, vertSize;

  GLubyte padding[54];
};
#pragma pack(pop)

static void
ReadPCX1bit (FILE *fp, const struct pcx_header_t *hdr,
	     struct gl_texture_t *texinfo)
{
  int y, i, bytes;
  int colorIndex;
  int rle_count = 0, rle_value = 0;
  GLubyte *ptr = texinfo->texels;

  for (y = 0; y < texinfo->height; ++y)
    {
      ptr = &texinfo->texels[(texinfo->height - (y + 1)) * texinfo->width * 3];
      bytes = hdr->bytesPerScanLine;

      /* Decode line number y */
      while (bytes--)
	{
	  if (rle_count == 0)
	    {
	      if ( (rle_value = fgetc (fp)) < 0xc0)
		{
		  rle_count = 1;
		}
	      else
		{
		  rle_count = rle_value - 0xc0;
		  rle_value = fgetc (fp);
		}
	    }

	  rle_count--;

	  for (i = 7; i >= 0; --i, ptr += 3)
	    {
	      colorIndex = ((rle_value & (1 << i)) > 0);

	      ptr[0] = hdr->palette[colorIndex * 3 + 0];
	      ptr[1] = hdr->palette[colorIndex * 3 + 1];
	      ptr[2] = hdr->palette[colorIndex * 3 + 2];
	    }
	}
    }
}

static void
ReadPCX4bits (FILE *fp, const struct pcx_header_t *hdr,
	      struct gl_texture_t *texinfo)
{
  GLubyte *colorIndex, *line;
  GLubyte *pLine, *ptr;
  int rle_count = 0, rle_value = 0;
  int x, y, c;
  int bytes;

  colorIndex = (GLubyte *)malloc (sizeof (GLubyte) * texinfo->width);
  line = (GLubyte *)malloc (sizeof (GLubyte) * hdr->bytesPerScanLine);

  for (y = 0; y < texinfo->height; ++y)
    {
      ptr = &texinfo->texels[(texinfo->height - (y + 1)) * texinfo->width * 3];

      memset (colorIndex, 0, texinfo->width * sizeof (GLubyte));

      for (c = 0; c < 4; ++c)
	{
	  pLine = line;
	  bytes = hdr->bytesPerScanLine;

	  /* Decode line number y */
	  while (bytes--)
	    {
	      if (rle_count == 0)
		{
		  if ( (rle_value = fgetc (fp)) < 0xc0)
		    {
		      rle_count = 1;
		    }
		  else
		    {
		      rle_count = rle_value - 0xc0;
		      rle_value = fgetc (fp);
		    }
		}

	      rle_count--;
	      *(pLine++) = rle_value;
	    }

	  /* Compute line's color indexes */
	  for (x = 0; x < texinfo->width; ++x)
	    {
	      if (line[x / 8] & (128 >> (x % 8)))
		colorIndex[x] += (1 << c);
	    }
	}

      /* Decode scan line.  color index => rgb  */
      for (x = 0; x < texinfo->width; ++x, ptr += 3)
	{
	  ptr[0] = hdr->palette[colorIndex[x] * 3 + 0];
	  ptr[1] = hdr->palette[colorIndex[x] * 3 + 1];
	  ptr[2] = hdr->palette[colorIndex[x] * 3 + 2];
	}
    }

  /* Release memory */
  free (colorIndex);
  free (line);
}

static void
ReadPCX8bits (FILE *fp, const struct pcx_header_t *hdr,
	      struct gl_texture_t *texinfo)
{
  int rle_count = 0, rle_value = 0;
  GLubyte palette[768];
  GLubyte magic;
  GLubyte *ptr;
  fpos_t curpos;
  int y, bytes;

  /* The palette is contained in the last 769 bytes of the file */
  fgetpos (fp, &curpos);
  fseek (fp, -769, SEEK_END);
  magic = fgetc (fp);

  /* First byte must be equal to 0x0c (12) */
  if (magic != 0x0c)
    {
      fprintf (stderr, "error: colormap's first byte must be 0x0c! "
	       "(%#x)\n", magic);

      free (texinfo->texels);
      texinfo->texels = NULL;
      return;
    }

  /* Read palette */
  fread (palette, sizeof (GLubyte), 768, fp);
  fsetpos (fp, &curpos);

  /* Read pixel data */
  for (y = 0; y < texinfo->height; ++y)
    {
      ptr = &texinfo->texels[(texinfo->height - (y + 1)) * texinfo->width * 3];
      bytes = hdr->bytesPerScanLine;

      /* Decode line number y */
      while (bytes--)
	{
	  if (rle_count == 0)
	    {
	      if( (rle_value = fgetc (fp)) < 0xc0)
		{
		  rle_count = 1;
		}
	      else
		{
		  rle_count = rle_value - 0xc0;
		  rle_value = fgetc (fp);
		}
	    }

	  rle_count--;

	  ptr[0] = palette[rle_value * 3 + 0];
	  ptr[1] = palette[rle_value * 3 + 1];
	  ptr[2] = palette[rle_value * 3 + 2];
	  ptr += 3;
	}
    }
}

static void
ReadPCX24bits (FILE *fp, const struct pcx_header_t *hdr,
	       struct gl_texture_t *texinfo)
{
  GLubyte *ptr = texinfo->texels;
  int rle_count = 0, rle_value = 0;
  int y, c;
  int bytes;

  for (y = 0; y < texinfo->height; ++y)
    {
      /* For each color plane */
      for (c = 0; c < 3; ++c)
	{
	  ptr = &texinfo->texels[(texinfo->height - (y + 1)) * texinfo->width * 3];
	  bytes = hdr->bytesPerScanLine;

	  /* Decode line number y */
	  while (bytes--)
	    {
	      if (rle_count == 0)
		{
		  if( (rle_value = fgetc (fp)) < 0xc0)
		    {
		      rle_count = 1;
		    }
		  else
		    {
		      rle_count = rle_value - 0xc0;
		      rle_value = fgetc (fp);
		    }
		}

	      rle_count--;
	      ptr[c] = (GLubyte)rle_value;
	      ptr += 3;
	    }
	}
    }
}

static struct gl_texture_t *
ReadPCXFile (const wchar_t *filename)
{
  struct gl_texture_t *texinfo;
  struct pcx_header_t header;
  FILE *fp = NULL;
  int bitcount;

  /* Open image file */
  fp =  axReadFile(filename);
  if (!fp)
    {
      fprintf (stderr, "error: couldn't open \"%s\"!\n", filename);
      return NULL;
    }

  /* Read header file */
  fread (&header, sizeof (struct pcx_header_t), 1, fp);
  if (header.manufacturer != 0x0a)
    {
      fprintf (stderr, "error: bad version number! (%i)\n",
	       header.manufacturer);
      return NULL;
    }

  /* Initialize texture parameters */
  texinfo = (struct gl_texture_t *)
    malloc (sizeof (struct gl_texture_t));
  texinfo->width = header.xmax - header.xmin + 1;
  texinfo->height = header.ymax - header.ymin + 1;
  texinfo->format = GL_RGB;
  texinfo->internalFormat = 3;
  texinfo->texels = (GLubyte *)
    malloc (sizeof (GLubyte) * texinfo->width
	    * texinfo->height * texinfo->internalFormat);

  bitcount = header.bitsPerPixel * header.numColorPlanes;

  /* Read image data */
  switch (bitcount)
    {
    case 1:
      /* 1 bit color index */
      ReadPCX1bit (fp, &header, texinfo);
      break;

    case 4:
      /* 4 bits color index */
      ReadPCX4bits (fp, &header, texinfo);
      break;

    case 8:
      /* 8 bits color index */
      ReadPCX8bits (fp, &header, texinfo);
      break;

    case 24:
      /* 24 bits */
      ReadPCX24bits (fp, &header, texinfo);
      break;

    default:
      /* Unsupported */
      fprintf (stderr, "error: unknown %i bitcount pcx files\n", bitcount);
      free (texinfo->texels);
      free (texinfo);
      texinfo = NULL;
      break;
    }

  fclose (fp);
  return texinfo;
}

//Wrapper Section
PCXImageFormat::PCXImageFormat()
{
    AllFormats[L"pcx"] = this;
    //ctor
}

PCXImageFormat::~PCXImageFormat()
{
    AllFormats[L"pcx"] = 0;
    //dtor
}

PCXImageFormat* PCXImageFormat::instance()
{
    if(!instance_)
    {
        instance_ = new PCXImageFormat();
    }

    return instance_;
}

axelynx::PixMap* PCXImageFormat::LoadPixMap(axelynx::File filename)
{
    gl_texture_t *t = 0;

	t = ReadPCXFile (filename.GetFilename().c_str());

    axelynx::PixMap *pm = axelynx::PixMap::Create(t->width,t->height,t->internalFormat);
    memcpy(pm->Data(),t->texels,t->width*t->height*t->internalFormat);
    return pm;
}

bool PCXImageFormat::SavePixMap(axelynx::File filename, const axelynx::PixMap *pixmap)
{
    return false;
}

axelynx::Texture* PCXImageFormat::LoadTexture(axelynx::File filename,axelynx::Texture::Desc &desc)
{
    gl_texture_t *t = 0;

	t = ReadPCXFile (filename.GetFilename().c_str());


	GLuint format = (t->internalFormat == 3 ? GL_RGB : GL_RGBA);
	GLuint internalFormat = (t->internalFormat == 3 ? GL_RGB8 : GL_RGBA8);

	axelynx::Texture::Desc desc2 = desc;
	desc2.width = t->width;
	desc2.height = t->height;
	desc2.cpp = t->internalFormat;

	CTexture *tex = new CTexture(desc2);
	tex->Build(t->texels,format,internalFormat);

    return tex;
}

bool PCXImageFormat::SaveTexture(axelynx::File filename, const axelynx::Texture *texture)
{
    return false;
}