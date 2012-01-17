

#include "_7BIImageFormat.h"


#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "axelynx/Logger.h"

#include "../gl/axgl.h"
#include "../CFile.h"

#include "../CTexture.h"

_7BIImageFormat * _7BIImageFormat::instance_=0;


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

int Compress7bi(const unsigned char * input, unsigned char * output, int size)
{
    int oc = 0, zb = 0;
    for(int i =0;i<size;++i)
    {
        //LOG_MESSAGE("input[%d] = %d",i,input[i]);

        if(input[i]>7)
        {
            if(zb)
            {
                output[oc++]= 128 + zb;
            }

            output[oc++]=input[i] /2;
            zb = 0;
        }
        else
        {
            zb++;
            if(zb==127)
            {
                output[oc++]=255;
                zb = 0;
            }
        }
    }

    if(!zb)
    {
        output[oc++]= 128 + zb;
    }

    return oc;
}

int DeCompress7bi(const unsigned char *input, unsigned char* output, int size, int maxsize)
{
    memset(output,0,maxsize);

    int oc=0;
    for(int i=0;i<size;++i)
    {
        if(input[i]<128)
        {
            //if(oc>=(maxsize-1))
            //    return -1;

            output[oc++] = input[i]*2;
        }
        else
        {
            oc+= (input[i]-128);
        }
    }
    return oc;
}

//Wrapper Section
_7BIImageFormat::_7BIImageFormat()
{
    AllFormats[L"7bi"] = this;
    //ctor
}

_7BIImageFormat::~_7BIImageFormat()
{
    AllFormats[L"7bi"] = 0;
    //dtor
}

_7BIImageFormat* _7BIImageFormat::instance()
{
    if(!instance_)
    {
        instance_ = new _7BIImageFormat();
    }

    return instance_;
}

axelynx::PixMap* _7BIImageFormat::LoadPixMap(axelynx::File file)
{
	file.Open();   
    int width=0, height = 0;
	int sz = file.Size() -8;
	file.Read(width);
	file.Read(height);
    axelynx::PixMap *pm = axelynx::PixMap::Create(width,height,1);
    unsigned char *buff = new unsigned char[sz];
	file.Read(buff,sz);
    DeCompress7bi(buff,pm->Data(),sz,width*height);

    delete[] buff;

	file.Close();
    return pm;
}

bool _7BIImageFormat::SavePixMap(axelynx::File file, const axelynx::PixMap *pixmap)
{
    if(pixmap->Bpp()!=1)
    {
        LOG_ERROR(L"only 1-channels PixMap may be saved as 7bi image!");
        return false;
    }
    FILE *fp = NULL;

    /* Write image file */
    int width = pixmap->Width();
    int height = pixmap->Height();
    unsigned char *buff = new unsigned char[width * height];

    int size = Compress7bi(pixmap->Data(),buff,width*height);

	file.Open();
	file.Write(width);
	file.Write(height);
	file.Write(buff,size);
	file.Close();
    delete[] buff;

    return false;
}

axelynx::Texture* _7BIImageFormat::LoadTexture(axelynx::File file,axelynx::Texture::Desc &desc)
{
    gl_texture_t *t = 0;

	file.Open();   
    int width=0, height = 0;
	int sz = file.Size() -8;
	file.Read(width);
	file.Read(height);
    axelynx::PixMap *pm = axelynx::PixMap::Create(width,height,1);
    unsigned char *buff = new unsigned char[sz];
	file.Read(buff,sz);
    DeCompress7bi(buff,pm->Data(),sz,width*height);


	GLuint format = GL_LUMINANCE;
	GLuint internalFormat = GL_LUMINANCE8;

	axelynx::Texture::Desc desc2 = desc;
	desc2.width = width;
	desc2.height = height;
	desc2.cpp = 1;

	CTexture *tex = new CTexture(desc2);
	tex->Build(buff,format,internalFormat);
    delete[] buff;

    return tex;
}

bool _7BIImageFormat::SaveTexture(axelynx::File file, const axelynx::Texture *texture)
{
    return false;
}
