

#include "CTexture.h"
#include <cassert>
#include "axelynx/Logger.h"
#include "CStatistics.h"
#include <iostream>
#include <vector>

CTexture * CTexture::current_[] =	{
									0,0,0,0,0,0,0,0,
									0,0,0,0,0,0,0,0,
									0,0,0,0,0,0,0,0,
									0,0,0,0,0,0,0,0
									};
CTexture * CTexture::fake_ = 0;

int CTexture::GetWidth() const
{
	return width_;
}

int CTexture::GetHeight() const
{
	return height_;
}

int CTexture::GetDepth() const
{
	return depth_;
}

axelynx::Texture::TextureType CTexture::GetType() const
{
	return axelynx_type_;
}
		
bool CTexture::Bind(int layer)
{
	restored_ = current_[current_layer_];
	restored_layer_ = current_layer_;

	assert(layer >= 0 && layer<32);
	current_layer_ = layer;

	if(current_[layer]!=this)
	{
		glActiveTexture(GL_TEXTURE0 + current_layer_);
		//std::cout<<inner_tex_->handle_<<std::endl;

		glBindTexture(gl_type_,handle_);

		current_[layer] = this;

		CStatistics::textures++;
	}
	return true;


}

bool CTexture::UnBind(bool restore)
{
	if(restore)
		if(restored_)
			restored_->Bind(restored_layer_);

	if(current_layer_>=0)
	{
		if(current_[current_layer_])
		{
			glActiveTexture(GL_TEXTURE0 + current_layer_);
			glBindTexture(gl_type_,0);
			glActiveTexture(GL_TEXTURE0);
			current_[current_layer_] = 0;
			current_layer_ = -1;
		}
	}
	return true;
}

void CTexture::init(axelynx::Texture::Desc &desc)
{
	current_layer_ = 0;
	axelynx_type_ = desc.TT;

	width_ = desc.width;
	height_ = desc.height;
	depth_ = desc.depth;

	cpp_ = desc.cpp;
	bpc_ = desc.bpc;

	anisotropic_ = desc.anisotropic;
	glGenTextures(1,&handle_);

	compressed_ = desc.compressed;
	use_mipmaps_ = desc.use_mipmaps;
	switch( desc.TT)
	{
	case axelynx::Texture::TT_1D:
		gl_type_ = GL_TEXTURE_1D;
		break;
	case axelynx::Texture::TT_3D:
		gl_type_ = GL_TEXTURE_3D;
		break;
	case axelynx::Texture::TT_2D:;
	case axelynx::Texture::TT_DEPTH:
		gl_type_ = GL_TEXTURE_2D;
		break;
	case axelynx::Texture::TT_CUBE:
		gl_type_ = GL_TEXTURE_CUBE_MAP;
		break;
	case axelynx::Texture::TT_ARRAY:
		gl_type_ = GL_TEXTURE_2D_ARRAY;
		break;
	default:
		LOG_WARNING(L"Not suppott!");
	}

	restored_=0;
	restored_layer_=0;

	inner_tex_ = 0;
}

CTexture::CTexture(axelynx::Texture::Desc &desc)
{
current_layer_ = 0;
	axelynx_type_ = desc.TT;

	width_ = desc.width;
	height_ = desc.height;
	depth_ = desc.depth;

	cpp_ = desc.cpp;
	bpc_ = desc.bpc;

	switch( desc.TT)
	{
	case axelynx::Texture::TT_1D:
		gl_type_ = GL_TEXTURE_1D;
		break;
	case axelynx::Texture::TT_3D:
		gl_type_ = GL_TEXTURE_3D;
		break;

	case axelynx::Texture::TT_DEPTH:
	case axelynx::Texture::TT_2D:
		gl_type_ = GL_TEXTURE_2D;
		break;
	case axelynx::Texture::TT_CUBE:
		gl_type_ = GL_TEXTURE_CUBE_MAP;
		break;
	case axelynx::Texture::TT_ARRAY:
		gl_type_ = GL_TEXTURE_2D_ARRAY;
		break;
	default:
		LOG_WARNING(L"Not suppott!");
	}

	anisotropic_ = desc.anisotropic;
	glGenTextures(1,&handle_);
	std::cout<<handle_<<std::endl;
	compressed_ = desc.compressed;
	use_mipmaps_ = desc.use_mipmaps;
	

	restored_=0;
	restored_layer_=0;

	inner_tex_ = 0;
}


CTexture::CTexture(GLuint handele, axelynx::Texture::Desc &desc)
{
	current_layer_ = 0;
	axelynx_type_ = desc.TT;

	width_ = desc.width;
	height_ = desc.height;
	depth_ = desc.depth;

	cpp_ = desc.cpp;
	bpc_ = desc.bpc;

	anisotropic_ = desc.anisotropic;
	handle_ =handele;

	compressed_ = desc.compressed;
	use_mipmaps_ = desc.use_mipmaps;
	switch( desc.TT)
	{
	case axelynx::Texture::TT_1D:
		gl_type_ = GL_TEXTURE_1D;
		break;
	case axelynx::Texture::TT_3D:
		gl_type_ = GL_TEXTURE_3D;
		break;
	case axelynx::Texture::TT_2D:;
	case axelynx::Texture::TT_DEPTH:
		gl_type_ = GL_TEXTURE_2D;
		break;
	case axelynx::Texture::TT_CUBE:
		gl_type_ = GL_TEXTURE_CUBE_MAP;
		break;
	case axelynx::Texture::TT_ARRAY:
		gl_type_ = GL_TEXTURE_2D_ARRAY;
		break;
	default:
		LOG_WARNING(L"Not suppott!");
	}

	restored_=0;
	restored_layer_=0;

	inner_tex_ = 0;

}

void CTexture::Build(const void* data,GLuint format,GLuint internalFormat,const void *data1,const void *data2,const void *data3,const void *data4,const void *data5)
{
	OPENGL_CHECK_FOR_ERRORS();
	glBindTexture(gl_type_,handle_);

	OPENGL_CHECK_FOR_ERRORS();
	if(format == GL_DEPTH_COMPONENT)
	{
		glTexParameteri(gl_type_, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(gl_type_, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); 
	}
	else
	{
		glTexParameteri(gl_type_, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(gl_type_, GL_TEXTURE_WRAP_T, GL_REPEAT); 
	}
	OPENGL_CHECK_FOR_ERRORS();
	if(use_mipmaps_)
	{
		glTexParameteri(gl_type_, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(gl_type_, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	else
	{
		if(format == GL_DEPTH_COMPONENT)
		{
			glTexParameteri(gl_type_, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(gl_type_, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		}
		else
		{
			glTexParameteri(gl_type_, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(gl_type_, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		}
	}

	if(anisotropic_ > 0.124f)
		glTexParameterf(gl_type_,GL_TEXTURE_MAX_ANISOTROPY_EXT,anisotropic_);

	GLenum type;OPENGL_CHECK_FOR_ERRORS();
	switch(bpc_)
	{
	case 1:
		type = GL_UNSIGNED_BYTE;
		break;
	case 2:
		type = GL_HALF_FLOAT;
		break;
	case 4:
		type = GL_FLOAT;
		break;
	};

	if(compressed_ == 1)
	{
		if(cpp_==2)
			internalFormat = GL_COMPRESSED_RED;
		if(cpp_==2)
			internalFormat = GL_COMPRESSED_RG_RGTC2;
		if(cpp_==3)
			internalFormat = GL_COMPRESSED_RGB_S3TC_DXT1_EXT;
		if(cpp_==4)
			internalFormat = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
	}

	if(compressed_ ==2)
	{
		if(cpp_==3)
			internalFormat = GL_COMPRESSED_RGB_BPTC_SIGNED_FLOAT_ARB;
	}

	if(gl_type_ == GL_TEXTURE_2D)
		glTexImage2D(gl_type_,0,internalFormat,width_,height_,0,format,type,data);

	if(gl_type_ == GL_TEXTURE_3D || gl_type_ == GL_TEXTURE_2D_ARRAY)
		glTexImage3D(gl_type_,0,internalFormat,width_,height_,depth_,0,format,type,data);

	if(gl_type_ == GL_TEXTURE_1D)
		glTexImage1D(gl_type_,0,internalFormat,width_,0,format,type,data);
	OPENGL_CHECK_FOR_ERRORS();
	if(gl_type_ == GL_TEXTURE_CUBE_MAP)
	{
		std::cout<<width_<<std::endl;
		std::cout<<height_<<std::endl;
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X,0,internalFormat,width_,height_,0,format,type,data);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X,0,internalFormat,width_,height_,0,format,type,data1);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y,0,internalFormat,width_,height_,0,format,type,data2);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,0,internalFormat,width_,height_,0,format,type,data3);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z,0,internalFormat,width_,height_,0,format,type,data4);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z,0,internalFormat,width_,height_,0,format,type,data5);
	}
	OPENGL_CHECK_FOR_ERRORS();
	if(use_mipmaps_)
		glGenerateMipmap(gl_type_);

	OPENGL_CHECK_FOR_ERRORS();
}



AXELYNX_API axelynx::Texture * axelynx::Texture::Current()
{
	return CTexture::Current();
}

CTexture * CTexture::FromPixMap(const axelynx::PixMap *pm, axelynx::Texture::Desc desc)
{

	desc.width = pm->Width();
	desc.height = pm->Height();
	desc.cpp = pm->Bpp();
	desc.bpc = 1;

	GLenum format, internalformat;

	switch(pm->Bpp())
	{
	case 1:
		format = GL_LUMINANCE;
		internalformat = GL_LUMINANCE8;
		break;
	case 2:
		format = GL_LUMINANCE_ALPHA;
		internalformat = GL_LUMINANCE8_ALPHA8;
		break;
	case 3:
		format = GL_RGB;
		internalformat = GL_RGB8;
		break;
	case 4:
		format = GL_RGBA;
		internalformat = GL_RGBA8;
		break;
	};

	CTexture *tex = new CTexture(desc);
	tex->Build(pm->Data(),format,internalformat);
	return tex;
}

bool CTexture::SetInnerTex(CTexture *inner)
{
	inner_tex_ = inner;
	handle_ = inner->GetHandle();
	return true;
}

CTexture* CTexture::GetInnerTex()
{
	return inner_tex_;
}

CTexture *CTexture::GetFake()
{
	if(!fake_)
	{
		axelynx::Texture::Desc desc;

		desc.use_mipmaps = false;
		desc.width = 1;
		desc.height = 1;
		desc.cpp = 4;
		desc.bpc = 1;

		unsigned int white = 0xFFFFFFFF;
		desc.data = &white;

		fake_ = new CTexture(desc);
		fake_ ->Build(&white,GL_RGBA,GL_RGBA8);
	}

	return fake_;
}

int CountTextureLevels()
{
	for(int i=0;i<32;++i)
	{
		int width, height, depth = 0;

		glGetTexLevelParameteriv(GL_TEXTURE_2D,i,GL_TEXTURE_WIDTH,&width);
		glGetTexLevelParameteriv(GL_TEXTURE_2D,i,GL_TEXTURE_HEIGHT,&height);
		glGetTexLevelParameteriv(GL_TEXTURE_2D,i,GL_TEXTURE_DEPTH,&depth);

		if(width == 1 && height == 1 && depth == 1)
		{
			return i;
		}
	}

	return 0;
}

bool CTexture::Save(axelynx::File file) const
{
	file.Create();

	file.Write(axelynx_type_);
	file.Write(gl_type_);
	file.Write(width_);
	file.Write(height_);
	file.Write(depth_);
	file.Write(cpp_);
	file.Write(bpc_);
	file.Write(compressed_);
	file.Write(use_mipmaps_);
	file.Write(anisotropic_);

	glBindTexture(gl_type_,handle_);

	int count_mips = CountTextureLevels();
	std::cout<<"count levels:"<<count_mips<<std::endl;

	file.Write(count_mips);

	std::vector<char> data;

	for(int i=count_mips;i>=0;--i)
	{
		std::cout<<"i:"<<i<<std::endl;
		int internal_format=0;
		glGetTexLevelParameteriv(GL_TEXTURE_2D,i,GL_TEXTURE_INTERNAL_FORMAT,&internal_format);
		file.Write(internal_format);

		int width=0, height=0, depth = 0;
		glGetTexLevelParameteriv(GL_TEXTURE_2D,i,GL_TEXTURE_WIDTH,&width);
		glGetTexLevelParameteriv(GL_TEXTURE_2D,i,GL_TEXTURE_HEIGHT,&height);
		glGetTexLevelParameteriv(GL_TEXTURE_2D,i,GL_TEXTURE_DEPTH,&depth);

		file.Write(width);
		file.Write(height);
		file.Write(depth);

		if(compressed_)
		{
			int image_size=0;
			glGetTexLevelParameteriv(GL_TEXTURE_2D,i,GL_TEXTURE_COMPRESSED_IMAGE_SIZE,&image_size);
			file.Write(image_size);

			data.resize(image_size);
			
			glGetCompressedTexImage(GL_TEXTURE_2D,i,data.data());
		}
		else
		{
			std::cout<<"width:"<<width<<std::endl;
			std::cout<<"height:"<<height<<std::endl;
			std::cout<<"depth:"<<depth<<std::endl;
			std::cout<<"cpp_:"<<cpp_<<std::endl;
			std::cout<<"bpc_:"<<bpc_<<std::endl;
			data.resize(width * height * depth * 4 * bpc_);

			glGetTexImage(GL_TEXTURE_2D,i,GL_RGBA,GL_UNSIGNED_BYTE,data.data());
		}

		file.Write(data.data(),data.size());
	}


	glBindTexture(gl_type_,0);
	file.Close();

	return true;
}

CTexture::CTexture(axelynx::File file)
{
	file.Open();

	current_layer_ = 0;
	file.Read(axelynx_type_);

	file.Read(gl_type_);
	file.Read(width_);
	file.Read(height_);
	file.Read(depth_);
	file.Read(cpp_);
	file.Read(bpc_);
	file.Read(compressed_);
	file.Read(use_mipmaps_);
	file.Read(anisotropic_);

	int count_mips=0;
	file.Read(count_mips);

	glGenTextures(1,&handle_);

	glBindTexture(gl_type_,handle_);
	glTexParameteri(gl_type_, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexParameteri(gl_type_, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(gl_type_, GL_TEXTURE_WRAP_T, GL_REPEAT); 

	if(use_mipmaps_)
	{
		glTexParameteri(gl_type_, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	}
	else
	{
		glTexParameteri(gl_type_, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	}

	if(anisotropic_ > 0.124f)
		glTexParameterf(gl_type_,GL_TEXTURE_MAX_ANISOTROPY_EXT,anisotropic_);

	GLenum type;
	switch(bpc_)
	{
	case 1:
		type = GL_UNSIGNED_BYTE;
		break;
	case 2:
		type = GL_HALF_FLOAT;
		break;
	case 4:
		type = GL_FLOAT;
		break;
	};

	std::vector<char> data;

	for(int i=count_mips;i>=0;--i)
	{
		std::cout<<"i:"<<i<<std::endl;
		int internal_format=0;

		file.Read(internal_format);

		int width=0, height=0, depth = 0;

		file.Read(width);
		file.Read(height);
		file.Read(depth);

		if(compressed_)
		{
			int image_size=0;

			file.Read(image_size);

			data.resize(image_size);
			file.Read(data.data(),image_size);

			glCompressedTexImage2D(gl_type_,i,internal_format,width,height,0,image_size,data.data());
		}
		else
		{
			int image_size = width * height * depth * 4 * bpc_;
			data.resize(image_size);
			file.Read(data.data(),image_size);

			glTexImage2D(gl_type_,i,internal_format,width,height,0,GL_RGBA,GL_UNSIGNED_BYTE,data.data());
		}

		
	}

	restored_=0;
	restored_layer_=0;

	inner_tex_ = 0;

	file.Close();
}


bool CTexture::Grab(int x, int y)
{
	current_[0]->UnBind(true);
	current_[0]->UnBind(true);
	return true;
}

void CTexture::RegenerateMipmaps()
{
	glBindTexture(gl_type_,handle_);
	glGenerateMipmap(gl_type_);
}