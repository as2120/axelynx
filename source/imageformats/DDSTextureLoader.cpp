

#include "DDSImageFormat.h"
#include "../CTexture.h"
#include "../gl/axgl.h"
#include <iostream>

//typedef unsigned int DWORD;
typedef unsigned char UBYTE;

#define MAKEFOURCC(ch0, ch1, ch2, ch3) \
  (GLuint)( \
    (((DWORD)(UBYTE)(ch3) << 24) & 0xFF000000) | \
    (((DWORD)(UBYTE)(ch2) << 16) & 0x00FF0000) | \
    (((DWORD)(UBYTE)(ch1) <<  8) & 0x0000FF00) | \
     ((DWORD)(UBYTE)(ch0)        & 0x000000FF) )

struct DDS_PIXELFORMAT
{
    DWORD dwSize;
    DWORD dwFlags;
    DWORD dwFourCC;
    DWORD dwRGBBitCount;
    DWORD dwRBitMask;
    DWORD dwGBitMask;
    DWORD dwBBitMask;
    DWORD dwABitMask;
};

#define DDS_FOURCC 0x00000004  // DDPF_FOURCC
#define DDS_RGB    0x00000040  // DDPF_RGB
#define DDS_RGBA   0x00000041  // DDPF_RGB | DDPF_ALPHAPIXELS

const DWORD DDS_MAGIC = MAKEFOURCC('D','D','S',' ');

const DDS_PIXELFORMAT DDSPF_DXT1 =
    { sizeof(DDS_PIXELFORMAT), DDS_FOURCC, MAKEFOURCC('D','X','T','1'), 0, 0, 0, 0, 0 };

const DDS_PIXELFORMAT DDSPF_DXT2 =
    { sizeof(DDS_PIXELFORMAT), DDS_FOURCC, MAKEFOURCC('D','X','T','2'), 0, 0, 0, 0, 0 };

const DDS_PIXELFORMAT DDSPF_DXT3 =
    { sizeof(DDS_PIXELFORMAT), DDS_FOURCC, MAKEFOURCC('D','X','T','3'), 0, 0, 0, 0, 0 };

const DDS_PIXELFORMAT DDSPF_DXT4 =
    { sizeof(DDS_PIXELFORMAT), DDS_FOURCC, MAKEFOURCC('D','X','T','4'), 0, 0, 0, 0, 0 };

const DDS_PIXELFORMAT DDSPF_DXT5 =
    { sizeof(DDS_PIXELFORMAT), DDS_FOURCC, MAKEFOURCC('D','X','T','5'), 0, 0, 0, 0, 0 };

const DDS_PIXELFORMAT DDSPF_A8R8G8B8 =
    { sizeof(DDS_PIXELFORMAT), DDS_RGBA, 0, 32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000 };

const DDS_PIXELFORMAT DDSPF_A1R5G5B5 =
    { sizeof(DDS_PIXELFORMAT), DDS_RGBA, 0, 16, 0x00007c00, 0x000003e0, 0x0000001f, 0x00008000 };

const DDS_PIXELFORMAT DDSPF_A4R4G4B4 =
    { sizeof(DDS_PIXELFORMAT), DDS_RGBA, 0, 16, 0x00000f00, 0x000000f0, 0x0000000f, 0x0000f000 };

const DDS_PIXELFORMAT DDSPF_R8G8B8 =
    { sizeof(DDS_PIXELFORMAT), DDS_RGB, 0, 24, 0x00ff0000, 0x0000ff00, 0x000000ff, 0x00000000 };

const DDS_PIXELFORMAT DDSPF_R5G6B5 =
    { sizeof(DDS_PIXELFORMAT), DDS_RGB, 0, 16, 0x0000f800, 0x000007e0, 0x0000001f, 0x00000000 };

#define DDS_HEADER_FLAGS_TEXTURE    0x00001007  // DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH | DDSD_PIXELFORMAT
#define DDS_HEADER_FLAGS_MIPMAP     0x00020000  // DDSD_MIPMAPCOUNT
#define DDS_HEADER_FLAGS_VOLUME     0x00800000  // DDSD_DEPTH
#define DDS_HEADER_FLAGS_PITCH      0x00000008  // DDSD_PITCH
#define DDS_HEADER_FLAGS_LINEARSIZE 0x00080000  // DDSD_LINEARSIZE

#define DDS_SURFACE_FLAGS_TEXTURE 0x00001000 // DDSCAPS_TEXTURE
#define DDS_SURFACE_FLAGS_MIPMAP  0x00400008 // DDSCAPS_COMPLEX | DDSCAPS_MIPMAP
#define DDS_SURFACE_FLAGS_CUBEMAP 0x00000008 // DDSCAPS_COMPLEX

#define DDS_CUBEMAP_POSITIVEX 0x00000600 // DDSCAPS2_CUBEMAP | DDSCAPS2_CUBEMAP_POSITIVEX
#define DDS_CUBEMAP_NEGATIVEX 0x00000a00 // DDSCAPS2_CUBEMAP | DDSCAPS2_CUBEMAP_NEGATIVEX
#define DDS_CUBEMAP_POSITIVEY 0x00001200 // DDSCAPS2_CUBEMAP | DDSCAPS2_CUBEMAP_POSITIVEY
#define DDS_CUBEMAP_NEGATIVEY 0x00002200 // DDSCAPS2_CUBEMAP | DDSCAPS2_CUBEMAP_NEGATIVEY
#define DDS_CUBEMAP_POSITIVEZ 0x00004200 // DDSCAPS2_CUBEMAP | DDSCAPS2_CUBEMAP_POSITIVEZ
#define DDS_CUBEMAP_NEGATIVEZ 0x00008200 // DDSCAPS2_CUBEMAP | DDSCAPS2_CUBEMAP_NEGATIVEZ

#define DDS_CUBEMAP_ALLFACES ( DDS_CUBEMAP_POSITIVEX | DDS_CUBEMAP_NEGATIVEX |\
                               DDS_CUBEMAP_POSITIVEY | DDS_CUBEMAP_NEGATIVEY |\
                               DDS_CUBEMAP_POSITIVEZ | DDS_CUBEMAP_NEGATIVEZ )

#define DDS_FLAGS_VOLUME 0x00200000 // DDSCAPS2_VOLUME


struct DDS_HEADER
{
    DWORD dwSize;
    DWORD dwFlags;
    DWORD dwHeight;
    DWORD dwWidth;
    DWORD dwPitchOrLinearSize;
    DWORD dwDepth; // only if DDS_HEADER_FLAGS_VOLUME is set in dwHeaderFlags
    DWORD dwMipMapCount;
    DWORD dwReserved1[11];
    DDS_PIXELFORMAT ddspf;
    DWORD dwCaps;
	DWORD dwCaps2;
	DWORD dwCaps3;
	DWORD dwCaps4;
    DWORD dwReserved2;
};

typedef enum D3D10_RESOURCE_DIMENSION {
  D3D10_RESOURCE_DIMENSION_UNKNOWN     = 0,
  D3D10_RESOURCE_DIMENSION_BUFFER      = 1,
  D3D10_RESOURCE_DIMENSION_TEXTURE1D   = 2,
  D3D10_RESOURCE_DIMENSION_TEXTURE2D   = 3,
  D3D10_RESOURCE_DIMENSION_TEXTURE3D   = 4
} D3D10_RESOURCE_DIMENSION;

typedef enum DXGI_FORMAT {
  DXGI_FORMAT_UNKNOWN                      = 0,
  DXGI_FORMAT_R32G32B32A32_TYPELESS        = 1,
  DXGI_FORMAT_R32G32B32A32_FLOAT           = 2,
  DXGI_FORMAT_R32G32B32A32_UINT            = 3,
  DXGI_FORMAT_R32G32B32A32_SINT            = 4,
  DXGI_FORMAT_R32G32B32_TYPELESS           = 5,
  DXGI_FORMAT_R32G32B32_FLOAT              = 6,
  DXGI_FORMAT_R32G32B32_UINT               = 7,
  DXGI_FORMAT_R32G32B32_SINT               = 8,
  DXGI_FORMAT_R16G16B16A16_TYPELESS        = 9,
  DXGI_FORMAT_R16G16B16A16_FLOAT           = 10,
  DXGI_FORMAT_R16G16B16A16_UNORM           = 11,
  DXGI_FORMAT_R16G16B16A16_UINT            = 12,
  DXGI_FORMAT_R16G16B16A16_SNORM           = 13,
  DXGI_FORMAT_R16G16B16A16_SINT            = 14,
  DXGI_FORMAT_R32G32_TYPELESS              = 15,
  DXGI_FORMAT_R32G32_FLOAT                 = 16,
  DXGI_FORMAT_R32G32_UINT                  = 17,
  DXGI_FORMAT_R32G32_SINT                  = 18,
  DXGI_FORMAT_R32G8X24_TYPELESS            = 19,
  DXGI_FORMAT_D32_FLOAT_S8X24_UINT         = 20,
  DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS     = 21,
  DXGI_FORMAT_X32_TYPELESS_G8X24_UINT      = 22,
  DXGI_FORMAT_R10G10B10A2_TYPELESS         = 23,
  DXGI_FORMAT_R10G10B10A2_UNORM            = 24,
  DXGI_FORMAT_R10G10B10A2_UINT             = 25,
  DXGI_FORMAT_R11G11B10_FLOAT              = 26,
  DXGI_FORMAT_R8G8B8A8_TYPELESS            = 27,
  DXGI_FORMAT_R8G8B8A8_UNORM               = 28,
  DXGI_FORMAT_R8G8B8A8_UNORM_SRGB          = 29,
  DXGI_FORMAT_R8G8B8A8_UINT                = 30,
  DXGI_FORMAT_R8G8B8A8_SNORM               = 31,
  DXGI_FORMAT_R8G8B8A8_SINT                = 32,
  DXGI_FORMAT_R16G16_TYPELESS              = 33,
  DXGI_FORMAT_R16G16_FLOAT                 = 34,
  DXGI_FORMAT_R16G16_UNORM                 = 35,
  DXGI_FORMAT_R16G16_UINT                  = 36,
  DXGI_FORMAT_R16G16_SNORM                 = 37,
  DXGI_FORMAT_R16G16_SINT                  = 38,
  DXGI_FORMAT_R32_TYPELESS                 = 39,
  DXGI_FORMAT_D32_FLOAT                    = 40,
  DXGI_FORMAT_R32_FLOAT                    = 41,
  DXGI_FORMAT_R32_UINT                     = 42,
  DXGI_FORMAT_R32_SINT                     = 43,
  DXGI_FORMAT_R24G8_TYPELESS               = 44,
  DXGI_FORMAT_D24_UNORM_S8_UINT            = 45,
  DXGI_FORMAT_R24_UNORM_X8_TYPELESS        = 46,
  DXGI_FORMAT_X24_TYPELESS_G8_UINT         = 47,
  DXGI_FORMAT_R8G8_TYPELESS                = 48,
  DXGI_FORMAT_R8G8_UNORM                   = 49,
  DXGI_FORMAT_R8G8_UINT                    = 50,
  DXGI_FORMAT_R8G8_SNORM                   = 51,
  DXGI_FORMAT_R8G8_SINT                    = 52,
  DXGI_FORMAT_R16_TYPELESS                 = 53,
  DXGI_FORMAT_R16_FLOAT                    = 54,
  DXGI_FORMAT_D16_UNORM                    = 55,
  DXGI_FORMAT_R16_UNORM                    = 56,
  DXGI_FORMAT_R16_UINT                     = 57,
  DXGI_FORMAT_R16_SNORM                    = 58,
  DXGI_FORMAT_R16_SINT                     = 59,
  DXGI_FORMAT_R8_TYPELESS                  = 60,
  DXGI_FORMAT_R8_UNORM                     = 61,
  DXGI_FORMAT_R8_UINT                      = 62,
  DXGI_FORMAT_R8_SNORM                     = 63,
  DXGI_FORMAT_R8_SINT                      = 64,
  DXGI_FORMAT_A8_UNORM                     = 65,
  DXGI_FORMAT_R1_UNORM                     = 66,
  DXGI_FORMAT_R9G9B9E5_SHAREDEXP           = 67,
  DXGI_FORMAT_R8G8_B8G8_UNORM              = 68,
  DXGI_FORMAT_G8R8_G8B8_UNORM              = 69,
  DXGI_FORMAT_BC1_TYPELESS                 = 70,
  DXGI_FORMAT_BC1_UNORM                    = 71,
  DXGI_FORMAT_BC1_UNORM_SRGB               = 72,
  DXGI_FORMAT_BC2_TYPELESS                 = 73,
  DXGI_FORMAT_BC2_UNORM                    = 74,
  DXGI_FORMAT_BC2_UNORM_SRGB               = 75,
  DXGI_FORMAT_BC3_TYPELESS                 = 76,
  DXGI_FORMAT_BC3_UNORM                    = 77,
  DXGI_FORMAT_BC3_UNORM_SRGB               = 78,
  DXGI_FORMAT_BC4_TYPELESS                 = 79,
  DXGI_FORMAT_BC4_UNORM                    = 80,
  DXGI_FORMAT_BC4_SNORM                    = 81,
  DXGI_FORMAT_BC5_TYPELESS                 = 82,
  DXGI_FORMAT_BC5_UNORM                    = 83,
  DXGI_FORMAT_BC5_SNORM                    = 84,
  DXGI_FORMAT_B5G6R5_UNORM                 = 85,
  DXGI_FORMAT_B5G5R5A1_UNORM               = 86,
  DXGI_FORMAT_B8G8R8A8_UNORM               = 87,
  DXGI_FORMAT_B8G8R8X8_UNORM               = 88,
  DXGI_FORMAT_R10G10B10_XR_BIAS_A2_UNORM   = 89,
  DXGI_FORMAT_B8G8R8A8_TYPELESS            = 90,
  DXGI_FORMAT_B8G8R8A8_UNORM_SRGB          = 91,
  DXGI_FORMAT_B8G8R8X8_TYPELESS            = 92,
  DXGI_FORMAT_B8G8R8X8_UNORM_SRGB          = 93,
  DXGI_FORMAT_BC6H_TYPELESS                = 94,
  DXGI_FORMAT_BC6H_UF16                    = 95,
  DXGI_FORMAT_BC6H_SF16                    = 96,
  DXGI_FORMAT_BC7_TYPELESS                 = 97,
  DXGI_FORMAT_BC7_UNORM                    = 98,
  DXGI_FORMAT_BC7_UNORM_SRGB               = 99,
  DXGI_FORMAT_FORCE_UINT                   = 0xffffffffUL
} DXGI_FORMAT, *LPDXGI_FORMAT;

struct DDS_HEADER_DXT10
{
  DXGI_FORMAT              dxgiFormat;
  D3D10_RESOURCE_DIMENSION resourceDimension;
  UINT                     miscFlag;
  UINT                     arraySize;
  UINT                     reserved;
} ;



const DWORD DDSCAPS2_CUBEMAP				=0x200;		//Required for a cube map.
const DWORD DDSCAPS2_CUBEMAP_POSITIVEX		=0x400;		//Required when these surfaces are stored in a cube map.
const DWORD DDSCAPS2_CUBEMAP_NEGATIVEX		=0x800;		//Required when these surfaces are stored in a cube map.
const DWORD DDSCAPS2_CUBEMAP_POSITIVEY		=0x1000;	//Required when these surfaces are stored in a cube map.
const DWORD DDSCAPS2_CUBEMAP_NEGATIVEY		=0x2000;	//Required when these surfaces are stored in a cube map.
const DWORD DDSCAPS2_CUBEMAP_POSITIVEZ		=0x4000;	//Required when these surfaces are stored in a cube map.
const DWORD DDSCAPS2_CUBEMAP_NEGATIVEZ		=0x8000;	//Required when these surfaces are stored in a cube map.
const DWORD DDSCAPS2_VOLUME					=0x200000;	//Required for a volume texture.

axelynx::Texture* DDSImageFormat::LoadTexture(axelynx::File file,axelynx::Texture::Desc &desc21)
{
	file.Open();

	DWORD dds_magic;
	file.Read(dds_magic);

	if(dds_magic != DDS_MAGIC)
	{
		LOG_WARNING(L"dds magic number not correct!");
	}

	DDS_HEADER dds_header;
	file.Read(dds_header);
	DDS_HEADER_DXT10 dds_dx10_header;
	memset(&dds_dx10_header,0,sizeof(dds_dx10_header));

	bool use_dx10_header = false;

	const DWORD DX10 = MAKEFOURCC('D','X','1','0');

	if((dds_header.ddspf.dwFlags == DDS_FOURCC) && (dds_header.ddspf.dwFourCC == DX10))
	{
		file.Read(dds_dx10_header);
		use_dx10_header = true;
	}

	axelynx::Texture::Desc desc = desc21;

	if(dds_header.dwCaps2 & DDSCAPS2_CUBEMAP)
		desc.TT = axelynx::Texture::TT_CUBE;

	if(dds_header.dwCaps2 & DDSCAPS2_VOLUME)
		desc.TT = axelynx::Texture::TT_3D;

	desc.use_mipmaps = (dds_header.dwMipMapCount>0);

	desc.width = dds_header.dwWidth;
	desc.height = dds_header.dwHeight;
	desc.depth = dds_header.dwDepth;

	std::cout<<"use_mipmaps:"<<desc.use_mipmaps<<std::endl;
	std::cout<<"width:"<<desc.width<<std::endl;
	std::cout<<"height:"<<desc.height<<std::endl;
	std::cout<<"depth:"<<desc.depth<<std::endl;

	CTexture *tex = new CTexture(desc);
	tex->Bind();

	GLuint gl_type = tex->GetGLTexType();

	if(desc.anisotropic > 0.124f)
		glTexParameterf(gl_type,GL_TEXTURE_MAX_ANISOTROPY_EXT,desc.anisotropic);

	glTexParameteri(gl_type, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	if(desc.use_mipmaps)
	{
		glTexParameteri(gl_type, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); //GL_LINEAR_MIPMAP_LINEAR
	}
	else
	{
		glTexParameteri(gl_type, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	}

	bool compressed = false;
	GLuint iformat = 0;
	GLuint format = 0;
	GLuint type = GL_UNSIGNED_BYTE;
	int bpp = 4;

	//int va = MAKEFOURCC(0,'B','C','1');
	//int va2 = MAKEFOURCC(0,'B','C','1');

		switch(dds_header.ddspf.dwFourCC)
		{
			case MAKEFOURCC('D','X','T','1'): case MAKEFOURCC('B','C','1','U'): case 72: case 808540228:
				iformat = GL_COMPRESSED_RGB_S3TC_DXT1_EXT;	compressed = true; format = GL_RGB;
			break;
			case MAKEFOURCC('D','X','T','3'): case MAKEFOURCC('B','C','2','U'): case 75:
				iformat = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;	compressed = true;format = GL_RGBA;
			break;
			case MAKEFOURCC('D','X','T','5'): case MAKEFOURCC('B','C','3','U'): case 78:
				iformat = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT; compressed = true;format = GL_RGBA;
			break;
			case MAKEFOURCC('A','T','I','1'): case MAKEFOURCC('B','C','4','U'): case 80:
				iformat = GL_COMPRESSED_RED_RGTC1;			compressed = true;format = GL_RED;
			break;
			case MAKEFOURCC('A','T','I','2'): case MAKEFOURCC('B','C','5','U'): case 83:
				iformat = GL_COMPRESSED_RG_RGTC2;			compressed = true;format = GL_RG;
			break;
			case MAKEFOURCC('B','C','4','S'): case 81:
				iformat = GL_COMPRESSED_SIGNED_RED_RGTC1;			compressed = true;format = GL_RED;
			break;
			case MAKEFOURCC('B','C','5','S'): case 82:
				iformat = GL_COMPRESSED_SIGNED_RG_RGTC2;			compressed = true;format = GL_RG;
			break;
			case 95:
				iformat = GL_COMPRESSED_RGB_BPTC_UNSIGNED_FLOAT_ARB;	compressed = true; format = GL_RGB;
			break;
			case 96:
				iformat = GL_COMPRESSED_RGB_BPTC_SIGNED_FLOAT_ARB;	compressed = true; format = GL_RGB;
			break;
			case 98:
				iformat = GL_COMPRESSED_RGBA_BPTC_UNORM_ARB;	compressed = true; format = GL_RGBA;
			break;
			case 99:
				iformat = GL_COMPRESSED_SRGB_ALPHA_BPTC_UNORM_ARB;	compressed = true; format = GL_SRGB_ALPHA;
			break;

			case 34:
				iformat = GL_RG16;format = GL_RG;type = GL_UNSIGNED_SHORT; bpp = 4;
			break;
			case 36:
				iformat = GL_RGBA16;format = GL_RGBA;type = GL_UNSIGNED_SHORT; bpp = 8;
			  break;
			case 111:
				iformat = GL_R16F;format = GL_RED;type = GL_HALF_FLOAT; bpp = 2;
			  break;
			case 112:
			  iformat = GL_RG16F;format = GL_RG;type = GL_HALF_FLOAT; bpp = 4;
			  break;
			case 113:
			  iformat = GL_RGBA16F;format = GL_RGBA;type = GL_HALF_FLOAT; bpp = 8;
			  break;
			case 114:
			  iformat = GL_R32F;format = GL_RED;type = GL_FLOAT; bpp = 4;
			  break;
			case 115:
			  iformat = GL_RG32F;format = GL_RG;type = GL_FLOAT; bpp = 8;
			  break;
			case 116:
			  iformat = GL_RGBA32F;format = GL_RGBA;type = GL_FLOAT; bpp = 16;
			  break;
			default:
			  switch (dds_header.ddspf.dwRGBBitCount){
				case 8:
				  if (dds_header.ddspf.dwRBitMask == 0xE0){
					iformat = GL_R3_G3_B2; format = GL_RED; bpp = 1;
				  } else {
					iformat = GL_R8; format = GL_RED; bpp = 1;
				  }
				  break;
				case 16:
				  if (dds_header.ddspf.dwABitMask){
					iformat = GL_RG8; format = GL_RG; bpp = 2;
				  } else {
					iformat = GL_R16I; format = GL_RED; bpp = 2;
				  }
				  break;
				case 24:
				  iformat = GL_RGB8; format = GL_RGB; bpp = 3;
				  break;
				case 32:
				  if (dds_header.ddspf.dwRBitMask == 0x3FF00000){
					  iformat = GL_RGB10_A2 ; format = GL_RGBA; bpp = 4;
				  } else {
					iformat = GL_RGBA8; format = GL_RGBA; bpp = 4;
				  }
				  break;
        default:
			LOG_WARNING(L"incorrect FourCC!");
      }
  }

	std::cout<<"iformat:"<<iformat<<std::endl;
	std::cout<<"format:"<<format<<std::endl;
	std::cout<<"bpp:"<<bpp<<std::endl;
	std::cout<<"gl_type:"<<gl_type<<std::endl;
	std::cout<<"desc.TT:"<<desc.TT<<std::endl;
	std::cout<<"compressed:"<<compressed<<std::endl;

	unsigned char *data = new unsigned char[1024 * 1024 * 16];

	//2D Textures
	if(desc.TT == axelynx::Texture::TT_2D)
	{
		if(compressed)
		{
			for(unsigned int i = 0; i<std::max<int>(dds_header.dwMipMapCount,1);++i)
			{
				int cur_width = std::max<int>(2,dds_header.dwWidth)>>i;
				int cur_height = std::max<int>(2,dds_header.dwHeight)>>i;

				int imagesize = std::max<int>(cur_width,4) * std::max<int>(cur_height,4);

				if(iformat == GL_COMPRESSED_RGB_S3TC_DXT1_EXT)
					imagesize /= 2;

				file.Read(data,imagesize);
				glCompressedTexImage2D(gl_type,i,iformat,cur_width,cur_height,0,imagesize,data);
			}
		}
		else
		{
			for(unsigned int i = 0; i<std::max<int>(dds_header.dwMipMapCount,1);++i)
			{
				int cur_width = std::max<int>(2,dds_header.dwWidth)>>i;
				int cur_height = std::max<int>(2,dds_header.dwHeight)>>i;

				int imagesize = cur_width * cur_height * bpp;
				file.Read(data,imagesize);

				glTexImage2D(gl_type,i,iformat,cur_width,cur_height,0,format,type,data);

			}
		}
	}


	//3D Textures
	if(desc.TT == axelynx::Texture::TT_3D)
	{
		if(compressed)
		{
			for(unsigned int i = 0; i<std::max<int>(dds_header.dwMipMapCount,1);++i)
			{
				int cur_width = std::max<int>(2,dds_header.dwWidth)>>i;
				int cur_height = std::max<int>(2,dds_header.dwHeight)>>i;
				int cur_depth = std::max<int>(2,dds_header.dwDepth)>>i;

				int imagesize = std::max<int>(cur_width * cur_height * cur_depth,16);

				if(iformat == GL_COMPRESSED_RGB_S3TC_DXT1_EXT)
					imagesize /= 2;

				file.Read(data,imagesize);
				glCompressedTexImage3D(gl_type,i,iformat,cur_width,cur_height,cur_depth,0,imagesize,data);
			}
		}
		else
		{
			for(unsigned int i = 0; i<std::max<int>(dds_header.dwMipMapCount,1);++i)
			{
				int cur_width = std::max<int>(2,dds_header.dwWidth)>>i;
				int cur_height = std::max<int>(2,dds_header.dwHeight)>>i;
				int cur_depth = std::max<int>(2,dds_header.dwDepth)>>i;

				int imagesize = cur_width * cur_height * cur_depth * bpp;
				file.Read(data,imagesize);

				glTexImage3D(gl_type,i,iformat,cur_width,cur_height,cur_depth,0,format,type,data);
			}
		}
	}

	//glCompressedTexImage2D(gl_type,0,
	//max(1, width x 4) x max(1, height x 4) x 8(DXT1) or 16(DXT2-5)

	/*glTexParameteri(gl_type_, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexParameteri(gl_type_, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(gl_type_, GL_TEXTURE_WRAP_T, GL_REPEAT);



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

	if(gl_type_ == )
		glTexImage1D(gl_type_,0,internalFormat,width_,0,format,type,data);


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
	}*/


	delete[] data;

	tex->UnBind(true);

	file.Close();

	//tex->Build(t->texels,format,internalFormat);

    return tex;
}

bool DDSImageFormat::SaveTexture(axelynx::File file, const axelynx::Texture *texture)
{
	file.Create();

	DWORD dds_magic = DDS_MAGIC;
	file.Write(dds_magic);


	DDS_HEADER dds_header;
	ZeroMemory(&dds_header,sizeof(dds_header));

	dds_header.dwSize = sizeof(dds_header);
	dds_header.ddspf.dwSize = sizeof(dds_header.ddspf.dwSize);
	dds_header.ddspf.dwFlags = DDS_FOURCC;
	dds_header.dwMipMapCount = 0;
	dds_header.ddspf.dwFourCC = MAKEFOURCC('D','X','T','1');
	dds_header.dwCaps = 4096;

	axelynx::Texture *tex = const_cast<axelynx::Texture *>(texture);

	tex->Bind();
	int image_size,image_width,image_height,image_depth=0;

	glGetTexLevelParameteriv(GL_TEXTURE_2D,0,GL_TEXTURE_WIDTH,&image_width);
	glGetTexLevelParameteriv(GL_TEXTURE_2D,0,GL_TEXTURE_WIDTH,&image_height);
	glGetTexLevelParameteriv(GL_TEXTURE_2D,0,GL_TEXTURE_DEPTH,&image_depth);

	
	dds_header.dwWidth = image_width;
	dds_header.dwHeight = image_height;
	dds_header.dwDepth = image_depth;

	file.Write(dds_header);

	glGetTexLevelParameteriv(GL_TEXTURE_2D,0,GL_TEXTURE_COMPRESSED_IMAGE_SIZE,&image_size);


	char * data = new char[image_size];
	
	
	glGetCompressedTexImage(GL_TEXTURE_2D,0,data);

	file.Write(data,image_size);

	delete[] data;

	tex->UnBind();

	dds_header.dwWidth;
	dds_header.dwHeight;
	dds_header.dwDepth;

	

	file.Close();
    return true;
}
