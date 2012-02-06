#include "CEngine.h"
#include "gl/axgl.h"



#include "CCanvas.h"
#include "CPixMap.h"
#include "CShader.h"

#include "imageformats/_7BIImageFormat.h"
#include "imageformats/BMPImageFormat.h"
#include "imageformats/DDSImageFormat.h"
#include "imageformats/JPEGImageFormat.h"
#include "imageformats/PCXImageFormat.h"
#include "imageformats/PNGImageFormat.h"
#include "imageformats/TGAImageFormat.h"

#include "CImage.h"
#include "CImageSystem.h"
#include "CScene.h"
#include "CSurface.h"
#include "CMaterial.h"
#include "CRenderTarget.h"
#include "CStatistics.h"
#include "CHardwareInfo.h"
#include "CTimer.h"
#include "CFont.h"
#include "CHGEFont.h"
#include "CFreeTypeFont.h"
#include "CFile.h"
#include "CSpriteSystem.h"
#include "CRaySystem.h"
#include "CPostEffect.h"
#include "axelynx/Shell.h"
#include "CInstanceGroup.h"
#include "CMorfedMeshFactory.h"
#include "CMD2Factory.h"

#include "CTheora.h"

#include "axelynx/Timer.h"

#include <process.h>

//Scenegraphs
#include "StandartSceneGraph.h"

#include "axelynx/utils/axstring.h"

#ifdef WIN32
#include "CWindowWin32.h"
#endif

#undef LoadImage

#ifdef UNIX
#include "CWindowUnix.h"
#endif

CEngine * CEngine::instance_=0;
CEngine::AsyncTexturesList *CEngine::async_textures_list=0;

//int CEngineCurretnFrame = 0;
//int CEngineCurrentFPS = 0;
//int CEngineCurrentFPSTime = 0;

AXELYNX_API axelynx::Engine * axelynx::Engine::Init(int version)
{
	if(version != axelynx::AXELYNX_VERSION)
	{
		shell::ShowError(L"incomaptible version lib and headers!!!");
		return 0;
	}

	axelynx::Engine *eng = new CEngine();
	Logger::Instance()->Init(L"application");
	#ifdef _MSC_VER_
	_set_SSE2_enable(1);
	#endif
	return eng;
}

axelynx::Window* CEngine::AddWindow_(int width, int height, int bpp, axelynx::WindowMode wm, int samples)
{
    #ifdef WIN32
	if(samples>0)
	{
		syswnd = CWindowWin32::Instance();
		syswnd->Init(width,height,bpp,wm,0);
		AxelynxInitExtensions(); //extension init
		InitShaderVersion(); //sader version prolog init (#version 330)
		syswnd->Free();
		syswnd->Init(width,height,bpp,wm,samples);
		AxelynxInitExtensions(); //extension init
	}
	else
	{
		syswnd = CWindowWin32::Instance();
		syswnd->Init(width,height,bpp,wm,samples);
		AxelynxInitExtensions(); //extension init
		InitShaderVersion(); //sader version prolog init (#version 330)
	}
    #endif

    #ifdef UNIX
    syswnd = CWindowUnix::Instance();
	syswnd->Init(width,height,bpp,wm,samples);
	AxelynxInitExtensions(); //extension init
	InitShaderVersion(); //sader version prolog init (#version 330)
    #endif

	InitOpenGL();

	return syswnd;
}

void CEngine::InitOpenGL()
{
    CCanvas * cc = CCanvas::Create(syswnd->GetWidth(),syswnd->GetHeight());
    cc->Recalc();

	CFreeTypeFont *def_font = new CFreeTypeFont(axelynx::Font::Desc());
	cc->SetFont(def_font);

	canvas_ = cc;
	OPENGL_CHECK_FOR_ERRORS();
	glClearColor(0,0,0,1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClampColor(GL_CLAMP_READ_COLOR,GL_FALSE);
	OPENGL_CHECK_FOR_ERRORS();

	{
		wchar_t *buff=0;

		LOG_WRITE(L"<table border = 1 <caption>videocard info</caption><tr><td>Vendor</td><td>");
		buff = utils::MultiByteToWideChar(reinterpret_cast<const char*>(glGetString(GL_VENDOR)));
		LOG_WRITE(buff);
		delete[] buff;
		OPENGL_CHECK_FOR_ERRORS();
		LOG_WRITE(L"</td></tr><td>Renderer</td><td>");
		buff = utils::MultiByteToWideChar(reinterpret_cast<const char*>(glGetString(GL_RENDERER)));
		LOG_WRITE(buff);
		delete[] buff;
		OPENGL_CHECK_FOR_ERRORS();
		LOG_WRITE(L"</td></tr><td>Driver version</td><td>");
		buff = utils::MultiByteToWideChar(reinterpret_cast<const char*>(glGetString(GL_VERSION)));
		LOG_WRITE(buff);
		delete[] buff;
		OPENGL_CHECK_FOR_ERRORS();
		LOG_WRITE(L"</td></tr><td>glsl compiler</td><td>");
		buff = utils::MultiByteToWideChar(reinterpret_cast<const char*>(glGetString(GL_SHADING_LANGUAGE_VERSION)));
		LOG_WRITE(buff);
		delete[] buff;
		OPENGL_CHECK_FOR_ERRORS();
		LOG_WRITE(L"</td></tr></table>");
	}

	{
		GLint ints[16];
		wchar_t buff[256];
		
		LOG_WRITE(L"<table border = 1 <caption>opengl renderer info</caption><tr><td>max clipplanes</td><td>");
		glGetIntegerv(GL_MAX_CLIP_PLANES,ints);
		wsprintf(buff,L"%d",ints[0]);
		LOG_WRITE(buff);
		OPENGL_CHECK_FOR_ERRORS();
		LOG_WRITE(L"</td></tr><td>max texture image units</td><td>");
		glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS,ints);
		wsprintf(buff,L"%d",ints[0]);
		LOG_WRITE(buff);
		OPENGL_CHECK_FOR_ERRORS();
		LOG_WRITE(L"</td></tr><td>max texture size</td><td>");
		glGetIntegerv(GL_MAX_TEXTURE_SIZE,ints);
		wsprintf(buff,L"%d",ints[0]);
		LOG_WRITE(buff);
		OPENGL_CHECK_FOR_ERRORS();
		LOG_WRITE(L"</td></tr><td>max 3d texture size</td><td>");
		glGetIntegerv(GL_MAX_3D_TEXTURE_SIZE,ints);
		wsprintf(buff,L"%d",ints[0]);
		LOG_WRITE(buff);
		OPENGL_CHECK_FOR_ERRORS();
		LOG_WRITE(L"</td></tr><td>max cubemap texture size</td><td>");
		glGetIntegerv(GL_MAX_CUBE_MAP_TEXTURE_SIZE,ints);
		wsprintf(buff,L"%d",ints[0]);
		LOG_WRITE(buff);
		OPENGL_CHECK_FOR_ERRORS();
		LOG_WRITE(L"</td></tr><td>MRT buffers</td><td>");
		glGetIntegerv(GL_MAX_DRAW_BUFFERS,ints);
		wsprintf(buff,L"%d",ints[0]);
		LOG_WRITE(buff);
		OPENGL_CHECK_FOR_ERRORS();
		LOG_WRITE(L"</td></tr><td>max indices</td><td>");
		glGetIntegerv(GL_MAX_ELEMENTS_INDICES,ints);
		wsprintf(buff,L"%d",ints[0]);
		LOG_WRITE(buff);
		OPENGL_CHECK_FOR_ERRORS();
		LOG_WRITE(L"</td></tr><td>max verices</td><td>");
		glGetIntegerv(GL_MAX_ELEMENTS_VERTICES,ints);
		wsprintf(buff,L"%d",ints[0]);
		LOG_WRITE(buff);
		OPENGL_CHECK_FOR_ERRORS();
		LOG_WRITE(L"</td></tr><td>max vertext uniforms</td><td>");
		glGetIntegerv(GL_MAX_VERTEX_UNIFORM_COMPONENTS,ints);
		wsprintf(buff,L"%d",ints[0]);
		LOG_WRITE(buff);
		OPENGL_CHECK_FOR_ERRORS();
		LOG_WRITE(L"</td></tr><td>max fragment uniforms</td><td>");
		glGetIntegerv(GL_MAX_FRAGMENT_UNIFORM_COMPONENTS,ints);
		wsprintf(buff,L"%d",ints[0]);
		LOG_WRITE(buff);
		OPENGL_CHECK_FOR_ERRORS();
		LOG_WRITE(L"</td></tr><td>max varyings floats</td><td>");
		glGetIntegerv(GL_MAX_VARYING_FLOATS,ints);
		wsprintf(buff,L"%d",ints[0]);
		LOG_WRITE(buff);
		OPENGL_CHECK_FOR_ERRORS();
		LOG_WRITE(L"</td></tr><td>max vertex attribs</td><td>");
		glGetIntegerv(GL_MAX_VERTEX_ATTRIBS,ints);
		wsprintf(buff,L"%d",ints[0]);
		LOG_WRITE(buff);
		OPENGL_CHECK_FOR_ERRORS();
		LOG_WRITE(L"</td></tr><td>max vertex textures units</td><td>");
		glGetIntegerv(GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS,ints);
		wsprintf(buff,L"%d",ints[0]);
		LOG_WRITE(buff);
		OPENGL_CHECK_FOR_ERRORS();
		LOG_WRITE(L"</td></tr><td>max viewport dims</td><td>");
		glGetIntegerv(GL_MAX_VIEWPORT_DIMS,ints);
		wsprintf(buff,L"%d",ints[0]);
		LOG_WRITE(buff);
		LOG_WRITE(L"</td></tr></table>");
	}

	InitTimer();
	srand(axelynx::Timer::Millisecs());

	OPENGL_CHECK_FOR_ERRORS();
	//glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );// Улучшение в вычислении перспективы
	//OPENGL_CHECK_FOR_ERRORS();
	glHint( GL_POINT_SMOOTH_HINT, GL_NICEST );// Улучшение в вычислении перспективы
	OPENGL_CHECK_FOR_ERRORS();
	glHint( GL_LINE_SMOOTH_HINT, GL_NICEST );// Улучшение в вычислении перспективы
	OPENGL_CHECK_FOR_ERRORS();
	glHint( GL_POLYGON_SMOOTH_HINT, GL_NICEST );// Улучшение в вычислении перспективы
	OPENGL_CHECK_FOR_ERRORS();
	//glHint( GL_FOG_HINT, GL_NICEST );// Улучшение в вычислении перспективы
	//OPENGL_CHECK_FOR_ERRORS();
	//glHint( GL_GENERATE_MIPMAP_HINT, GL_NICEST );// Улучшение в вычислении перспективы
	//OPENGL_CHECK_FOR_ERRORS();
	glHint( GL_TEXTURE_COMPRESSION_HINT, GL_NICEST );// Улучшение в вычислении перспективы

	OPENGL_CHECK_FOR_ERRORS();
	syswnd->VSync(true);
}

axelynx::Canvas* CEngine::GetCanvas()
{
	return canvas_;
}

bool  CEngine::Free()
{
	syswnd->Free();
	syswnd = 0;
	return true;
}

bool  CEngine::Sync()
{
	syswnd->Sync();
	return true;
}

bool  CEngine::Flip()
{
	syswnd->Flip(false);

	return true;
}

 CEngine::CEngine()
{
	//IMAGE FORMATS INIT
	_7BIImageFormat::instance();
	BMPImageFormat::instance();
	PNGImageFormat::instance();
	JPEGImageFormat::instance();
	TGAImageFormat::instance();
	PCXImageFormat::instance();
	DDSImageFormat::instance();

	//Morfed mesh format init
	axelynx::MorfedMeshFactory::RegisterFactory(L"md2",new CMD2Factory());

	//Default engine settings
	//Default system settings
	settings_.System.LowMemoryMode = false;
	settings_.System.LowVideoMemoryMode = false;

	//Default debug settings
	settings_.Debug.html_log = true;
	settings_.Debug.UseRunTimeProfiler = false;

	#ifdef _DEBUG
		settings_.Debug.FailOnOpenGLError = true;
		settings_.Debug.FailOnEngineError = true;
	#else
		settings_.Debug.FailOnOpenGLError = false;
		settings_.Debug.FailOnEngineError = false;
	#endif

	//Default filesystem settings
	settings_.FileSystem.FileExistsLevel = axelynx::Engine::TSettings::FileSystemSettings::FEL_FATAL;

	//Default renderer settings
	settings_.Renderer.AlphaSortingThresold = 100;
	settings_.Renderer.MaxUnpackedVertices = 1024;
	settings_.Renderer.UseMaterialSorting = true;
	settings_.Renderer.MinimumIndexSize = 4;

	//Default shaders settings
	#ifdef _DEBUG
		settings_.Shaders.UseCash = false;
	#else
		settings_.Shaders.UseCash = false;
	#endif
	settings_.Shaders.ShowLogOnFailCompile = true;

	instance_ = this;
}

 CEngine::~CEngine()
{
}

bool CEngine::isActive()
{
	return syswnd->isActive();
}

bool CEngine::isRunning()
{
	return syswnd->isRunning();
}

bool CEngine::SetText(const wchar_t * text)
{

	return syswnd->SetText(text);
}

bool CEngine::Render(bool clear)
{
	if(clear)
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	return true;
}

bool CEngine::Update(float twin)
{

	return true;
}

axelynx::PixMap* CEngine::CreatePixMap(int width, int height, int bpp)
{
	return axelynx::PixMap::Create(width, height, bpp);
}

axelynx::Texture* CEngine::CreateTexture(axelynx::Texture::Desc &desc)
{
	CTexture * tex = new CTexture(desc);

	GLenum format;
	GLenum internalformat;
	GetGLTextures(format,internalformat,desc.cpp,desc.bpc);
	tex->Build(0,format,internalformat);
	return tex;
}

axelynx::PixMap* CEngine::LoadPixMap(axelynx::File file, const wchar_t *format)
{
	axelynx::ImageFormat *ifrm = axelynx::ImageFormat::FindByExt(axelynx::utils::GetExtension(format));

	return ifrm->LoadPixMap(file);
}




void CEngine::LoadTextureAsync(void *parameter)
{
	int rc = CWindowWin32::Instance()->GetFreeLoadingRC();

	while(!async_textures_list->empty())
	{
		ASyncTextureLoadData data = *(async_textures_list->begin());

		axelynx::File file = data.file;
		axelynx::Texture::Desc desc = data.desc;

		axelynx::ImageFormat *ifrm = axelynx::ImageFormat::FindByExt(data.format);

		axelynx::Texture *tex = ifrm->LoadTexture(file,desc);
		CTexture *ctex = static_cast<CTexture*>(tex);
		std::cout<<ctex->GetHandle()<<std::endl;
		data.tex->SetInnerTex(ctex);

		async_textures_list->erase(async_textures_list->begin(),++async_textures_list->begin());

		Sleep(1);
	}

	delete async_textures_list;
	async_textures_list =0;

	CWindowWin32::Instance()->FreeLoadingRC(rc);
}

axelynx::Texture* CEngine::LoadTexture(axelynx::File file,Texture::Desc &desc, const wchar_t *format)
{
	std::wstring ext = format;
	if(ext == std::wstring(L""))
		ext = axelynx::utils::GetExtension(file.GetFilename());

	axelynx::Texture *tex = 0;
	if(desc.async)
	{
		CTexture *ctex = new CTexture(CTexture::GetFake()->GetHandle(),desc);
		ASyncTextureLoadData params;
		params.file = file;
		params.desc = desc;
		params.tex = ctex;
		params.format = ext;
		if(!async_textures_list)
		{
			async_textures_list = new AsyncTexturesList();
			_beginthread(LoadTextureAsync,0,0);
		}

		async_textures_list->push_back(params);
		tex = ctex;
	}
	else
	{
		axelynx::ImageFormat *ifrm = axelynx::ImageFormat::FindByExt(ext);
		tex = ifrm->LoadTexture(file,desc);

		OPENGL_CHECK_FOR_ERRORS();
	}
	return tex;
}

axelynx::Shader* CEngine::CreateBlankShader()
{
	return new CShader();
}

axelynx::Shader* CEngine::LoadShader(axelynx::File file)
{
	std::wstring filename = file.GetFilename();
	if(filename.empty())
		shell::ShowError(L"only filenames can be used in Engine::LoadShader command!");

	CShader * shader = new CShader();

	std::wstring filename_bin = std::wstring(filename)+L".bin";
	if(glProgramBinary && settings_.Shaders.UseCash && axFileType(filename_bin.c_str()))
	{
		shader->LoadBinary(filename_bin.c_str());
	}

	if(!shader->Valid()) //no loading binary
	{
		std::wstring filename_vs = std::wstring(filename)+L".vs";
		std::wstring filename_fs = std::wstring(filename)+L".fs";
		std::wstring filename_gs = std::wstring(filename)+L".gs";
		std::wstring filename_tcs = std::wstring(filename)+L".tcs";
		std::wstring filename_tes = std::wstring(filename)+L".tes";

		if(axFileType(filename_vs.c_str()))
			shader->LoadVertexSource(filename_vs.c_str());

		if(axFileType(filename_fs.c_str()))
			shader->LoadFragmentSource(filename_fs.c_str());

		if(axFileType(filename_gs.c_str()))
			shader->LoadGeometrySource(filename_gs.c_str());

		if(axFileType(filename_tcs.c_str()))
			shader->LoadTesselationControlSource(filename_tcs.c_str());

		if(axFileType(filename_tes.c_str()))
			shader->LoadTesselationEvaluationSource(filename_tes.c_str());

		shader->Compile();

		if(settings_.Shaders.UseCash && shader->Valid()) //save binary cash
			shader->SaveBinary(filename_bin.c_str());
	}
	return shader;
}

axelynx::Shader* CEngine::LoadShader(axelynx::File vsfile, axelynx::File fsfile)
{
	CShader * shader = new CShader();


	shader->LoadVertexSource(vsfile);
	shader->LoadFragmentSource(fsfile);
	shader->Compile();

	return shader;
}

axelynx::Shader* CEngine::LoadShader(axelynx::File vsfile, axelynx::File fsfile, axelynx::File gsfile)
{
	CShader * shader = new CShader();


	shader->LoadVertexSource(vsfile);
	shader->LoadFragmentSource(fsfile);
	shader->LoadGeometrySource(gsfile);
	shader->Compile();

	return shader;
}

axelynx::Shader* CEngine::CreateShader(const char* vertexsource, const char* fragmentsource, const char* geometrysource)
{
	axelynx::Shader *sh = new CShader();

	sh->VertexSource(vertexsource);
	sh->FragmentSource(fragmentsource);
	sh->GeometrySource(geometrysource);
	return sh;
}

axelynx::Texture* CEngine::CreateDepthTexture(int width, int height)
{
	axelynx::Texture::Desc desc;
	desc.Size(width,height);
	desc.TexType(axelynx::Texture::TT_DEPTH);

	CTexture *tex = new CTexture(desc);
	tex->Build(0,GL_DEPTH_COMPONENT,GL_DEPTH_COMPONENT);

	return tex;
}


axelynx::Engine::Info CEngine::GetInfo()
{
	axelynx::Engine::Info info;

	int major, minor;
	glGetIntegerv(GL_MAJOR_VERSION, &major);
	glGetIntegerv(GL_MINOR_VERSION, &minor);

	info.version = axelynx::AXELYNX_VERSION;
	info.renderer = std::string((const char*)glGetString(GL_RENDERER));
	info.vendor = std::string((const char*)glGetString(GL_VENDOR));
	info.driver = std::string((const char*)glGetString(GL_VERSION));
	info.major = major;
	info.minor = minor;
	info.glsl = std::string((const char*)glGetString(GL_SHADING_LANGUAGE_VERSION));

	return info;
}


axelynx::ImageSystem* CEngine::LoadImageSystem(axelynx::File file, int capaticity_, const wchar_t *format)
{
	std::wstring ext = format;
	if(ext == std::wstring(L""))
		ext = axelynx::utils::GetExtension(file.GetFilename());

	axelynx::PixMap *pm = LoadPixMap(file,ext.c_str());
	axelynx::ImageSystem *cis = CreateImageSystem(pm,capaticity_);
	delete pm;

	return cis;
}

axelynx::Image* CEngine::LoadImage(axelynx::File filename, int capaticity, const wchar_t *format)
{
	return 0;
}

axelynx::ImageSystem* CEngine::CreateImageSystem(const PixMap* pixmap, int capaticity_)
{
	axelynx::Texture *tex = CTexture::FromPixMap(pixmap);
	CImageSystem *cis = new CImageSystem(tex,capaticity_);
	return cis;
}

axelynx::Image* CEngine::CreateImage(const PixMap* pixmap, int capaticity_)
{
	return 0;
}

axelynx::Scene* CEngine::AddScene()
{
	return new CScene(new StandartSceneGraph());
}

axelynx::Sprite* CEngine::CreateSprite(float size)
{
	return 0;
}

axelynx::Sprite* CEngine::LoadSprite(axelynx::File file,float size, const wchar_t * format)
{
	return 0;
}

axelynx::Surface* CEngine::CreateSurface(int vertices, int tris)
{
	return new CSurface(vertices,tris * 3);
}

axelynx::Surface* CEngine::LoadSurface(axelynx::File file, const wchar_t * format)
{
	std::wstring ext = format;
	if(ext == std::wstring(L""))
		ext = axelynx::utils::GetExtension(file.GetFilename());

	axelynx::Surface *surf = 0;
	if(ext == std::wstring(L"sbs"))
		surf = CSurface::LoadSBS(file);
	if(ext == std::wstring(L"axs"))
		surf = CSurface::LoadAXS(file);

	if(!surf) //делаем заглушку
		surf = new CSurface(1,1);

	OPENGL_CHECK_FOR_ERRORS();
	return surf;
}

axelynx::Material* CEngine::CreateMaterial()
{
	return new CMaterial();
}

axelynx::Statistics* CEngine::GetStatistics()
{
	return CStatistics::Instance();
}

axelynx::HardwareInfo* CEngine::GetHardwareInfo()
{
	return CHardwareInfo::instance();
}

void CEngine::VSync(bool enabled)
{
	syswnd->VSync(enabled);
}

axelynx::RenderTarget* CEngine::CreateRenderTarget()
{
	return CreateRenderTarget(CCanvas::Instance()->GetWidth(),CCanvas::Instance()->GetHeight());
}

axelynx::RenderTarget* CEngine::CreateRenderTarget(int width, int height)
{
	return new CRenderTarget(width, height);
}

axelynx::Font* CEngine::LoadFont(const axelynx::Font::Desc& desc_)
{
	std::wstring ext = axelynx::utils::GetExtension(desc_.file.GetFilename());

	axelynx::Font *font = 0;
	if(ext == std::wstring(L"fnt")) //HGE font
		font = LoadHGEFont(desc_);
	if(ext == std::wstring(L"ttf")) //True Type font
		font = LoadFreeTypeFont(desc_);
	OPENGL_CHECK_FOR_ERRORS();
	return font;
}

axelynx::Font* CEngine::LoadHGEFont(const axelynx::Font::Desc& desc_)
{
	return new CHGEFont(desc_);
}

axelynx::Font* CEngine::LoadFreeTypeFont(const axelynx::Font::Desc& desc_)
{
	return new CFreeTypeFont(desc_);
}

AXELYNX_API axelynx::Engine * axelynx::Engine::Instance()
{
	return CEngine::Instance();
}

axelynx::Engine * CEngine::Instance()
{
	return instance_;
}

axelynx::SpriteSystem * CEngine::CreateSpriteSystem(int capacity)
{
	return new CSpriteSystem(capacity);
}

axelynx::SpriteSystem * CEngine::LoadSpriteSystem(axelynx::File file, const wchar_t * format)
{
	return 0;
}

axelynx::ParticleSystem * CEngine::CreateParticleSystem()
{
	return 0;
}

axelynx::ParticleSystem * CEngine::LoadParticleSystem(axelynx::File file)
{
	return 0;
}


axelynx::RaySystem * CEngine::CreateRaySystem(int capacity)
{
	return new CRaySystem(capacity);
}


axelynx::Shader* CEngine::LoadShaderBinary(axelynx::File file)
{
	CShader *s = new CShader();
	s->LoadBinary(file);
	return s;
}

axelynx::Texture* CEngine::LoadCubeMapTexture(axelynx::File sfile,axelynx::Texture::Desc &desc, const wchar_t * format)
{
	std::wstring ext = format;
	if(ext == L"")
		ext = axelynx::utils::GetExtension(sfile.GetFilename());

	std::wcout<<L"ext:"<<ext<<std::endl;

	std::wstring filename = sfile.GetFilename();
	std::wstring file = filename.substr(0,filename.length() - ext.length() - 1);
	std::wcout<<L"file:"<<file<<std::endl;

	return LoadCubeMapTexture((file+L"_posx."+ext).c_str(),(file+L"_negx."+ext).c_str()
					,(file+L"_posy."+ext).c_str(),(file+L"_negy."+ext).c_str()
					,(file+L"_posz."+ext).c_str(),(file+L"_negz."+ext).c_str()
					,desc,ext.c_str());
}

axelynx::Texture* CEngine::LoadCubeMapTexture(axelynx::File file_pos_x
	,axelynx::File file_neg_x
	,axelynx::File file_pos_y
	,axelynx::File file_neg_y
	,axelynx::File file_pos_z
	,axelynx::File file_neg_z
	,axelynx::Texture::Desc &desc, const wchar_t * fformat)
{
	std::wstring ext = fformat;

	PixMap *posx = LoadPixMap(file_pos_x,ext.c_str());
	PixMap *negx = LoadPixMap(file_neg_x,ext.c_str());
	PixMap *posy = LoadPixMap(file_pos_y,ext.c_str());
	PixMap *negy = LoadPixMap(file_neg_y,ext.c_str());
	PixMap *posz = LoadPixMap(file_pos_z,ext.c_str());
	PixMap *negz = LoadPixMap(file_neg_z,ext.c_str());

	desc.TT = axelynx::Texture::TT_CUBE;
	desc.width = desc.height = posx->Width();
	desc.bpc = 1;
	desc.cpp = posx->Bpp();
	desc.internalbytes = posx->Bpp();
	desc.use_mipmaps = false;

	CTexture * tex = new CTexture(desc);

	GLenum format = 0;
	GLenum internalformat = 0;
	switch(posx->Bpp())
	{
	case 1:
		format = GL_R;
		internalformat = GL_R8;
		break;
	case 2:
		format = GL_RG;
		internalformat = GL_RG8;
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

	tex->Build(posx->Data(),format,internalformat,negx->Data(),posy->Data(),negy->Data(),posz->Data(),negz->Data());


	delete posx;
	delete negx;
	delete posy;
	delete negy;
	delete posz;
	delete negz;
	return tex;
}

axelynx::Texture* CEngine::Load3DTexture(axelynx::File file, int width, int height, int count_x, int count_y,axelynx::Texture::Desc &desc_,const wchar_t * fformat)
{
	axelynx::Texture::Desc desc(desc_);
	desc.TT = axelynx::Texture::TT_3D;
	axelynx::PixMap *pm = LoadPixMap(file,fformat);

	desc.depth = count_x * count_y;
	desc.width = pm->Width() / count_x;
	desc.height = pm->Height() / count_y;

	unsigned char *data = new unsigned char[desc.depth * desc.height * desc.width * 3];
	//frames
	int pix = 0;

	for(int i = 0;i<count_x;++i)
	{
		for(int j = 0;j<count_y;++j)
		{
			//pixels
			for(int x = 0;x<desc.width;++x)
			{
				for(int y = 0;y<desc.height;++y)
				{
					int x0 = i * desc.width + x;
					int y0 = j * desc.height + y;

					data[pix + 0] = pm->GetPixel(x0,y0,0);
					data[pix + 1] = pm->GetPixel(x0,y0,1);
					data[pix + 2] = pm->GetPixel(x0,y0,2);
					pix+=3;
				}
			}
		}
	}

	CTexture *tex = new CTexture(desc);
	tex->Build(data,GL_RGB,GL_RGB8);
	delete[] data;
	delete pm;
	return tex;
}

axelynx::Texture* CEngine::LoadTextureArray(axelynx::File file, int width, int height, int count_x, int count_y,axelynx::Texture::Desc &desc_,const wchar_t *fformat)
{
	std::wstring ext = fformat;
	if(ext == std::wstring(L""))
		ext = axelynx::utils::GetExtension(file.GetFilename());

	axelynx::Texture::Desc desc(desc_);
	desc.TT = axelynx::Texture::TT_ARRAY;
	axelynx::PixMap *pm = LoadPixMap(file,ext.c_str());

	desc.depth = count_x * count_y;
	desc.width = pm->Width() / count_x;
	desc.height = pm->Height() / count_y;

	unsigned char *data = new unsigned char[desc.depth * desc.height * desc.width * 3];
	//frames
	int pix = 0;

	for(int i = 0;i<count_x;++i)
	{
		for(int j = 0;j<count_y;++j)
		{
			//pixels
			for(int x = 0;x<desc.width;++x)
			{
				for(int y = 0;y<desc.height;++y)
				{
					int x0 = i * desc.width + x;
					int y0 = j * desc.height + y;

					data[pix + 0] = pm->GetPixel(x0,y0,0);
					data[pix + 1] = pm->GetPixel(x0,y0,1);
					data[pix + 2] = pm->GetPixel(x0,y0,2);
					pix+=3;
				}
			}
		}
	}

	CTexture *tex = new CTexture(desc);
	tex->Build(data,GL_RGB,GL_RGB8);

	delete[] data;
	delete pm;
	return tex;
}

axelynx::Theora * CEngine::LoadTheoraVideo(axelynx::File file)
{
	std::cout<<"trace new theora"<<std::endl;
	return new CTheora(file);
}

axelynx::PostEffect* CEngine::CreatePostEffect(int width, int height, int channels, int channelsize)
{
	if(width==0)
	{
		width = CCanvas::Instance()->GetWidth();
		height = CCanvas::Instance()->GetHeight();
	}
	return new CPostEffect(width, height, channels, channelsize);
}

axelynx::PostEffect* CEngine::LoadPostEffect(axelynx::File file)
{
	axelynx::PostEffect *efx= 0;

	efx = CreatePostEffect();

	axelynx::Shader *shader = axelynx::Shader::Create();

	const char *vs =	"in vec2 position;\n"
						"uniform mat4 projection;\n"
						"uniform mat3x2 transform;\n"
						"in vec4 color;\n"
						"in vec2 texcoord;\n"
						"out vec4 fragmentcolor;\n"
						"out vec2 fragmentuv;\n"
						"vec2 VTransform(vec2 pos, mat3x2 m)\n"
						"{\n"
						"return vec2(pos.x * m[0][0] + pos.y * m[1][0] + m[2][0], pos.x * m[0][1] + pos.y *m[1][1] + m[2][1]);\n"
						"}\n"
						"void main(void) {\n"
						"vec2 tpos = VTransform(position, transform);\n"
						"gl_Position   = projection * vec4(tpos.x, tpos.y, 0.0,1.0);\n"
						"fragmentcolor = color;\n"
						"fragmentuv = texcoord;\n"
						"}\n";

	shader->VertexSource(vs);
	shader->LoadFragmentSource(file);

	shader->Compile();
	efx->BindShader(shader);
	return efx;

	return efx;
}

axelynx::Window* CEngine::ApplyWindow(axelynx::Window::SystemHandle *handle,int bpp, int samples)
{
 #ifdef WIN32
	if(samples>0)
	{
		syswnd = CWindowWin32::Instance();
		syswnd->Init(1,1,bpp,axelynx::WM_SPLASH,0);
		AxelynxInitExtensions(); //extension init
		InitShaderVersion(); //sader version prolog init (#version 330)
		syswnd->Free();
		syswnd->Init(handle,bpp,samples);
	}
	else
	{
		syswnd = CWindowWin32::Instance();
		syswnd->Init(handle,bpp,samples);
		AxelynxInitExtensions(); //extension init
		InitShaderVersion(); //sader version prolog init (#version 330)
	}
    #endif

    #ifdef UNIX
    syswnd = CWindowUnix::Instance();
	syswnd->Init(width,height,bpp,wm,samples);
	AxelynxInitExtensions(); //extension init
	InitShaderVersion(); //sader version prolog init (#version 330)
    #endif

	InitOpenGL();

	return syswnd;
}

axelynx::InstanceGroup* CEngine::CreateInstanceGroup()
{
	return new CInstanceGroup();
}

axelynx::Texture* CEngine::RestoreTexture(axelynx::File file)
{
	return new CTexture(file);
}

axelynx::Surface* CEngine::RestoreSurface(axelynx::File file)
{
	CSurface *surface = new CSurface();
	file.Open();
	surface->RestoreFromBinary(file);
	file.Close();
	return surface;
}

axelynx::MorfedMesh* CEngine::LoadMorfedMesh(axelynx::File file, const wchar_t* format)
{
	return axelynx::MorfedMeshFactory::getByExtension(format)->CreateFromFile(file);
}


axelynx::Texture* CEngine::LoadTextureFromResource(int ID, int type)
{


	if(type == 0)
	{
		CTexture * tex = 0;

		HBITMAP hBmp = (HBITMAP)LoadImageW(GetModuleHandle(NULL),MAKEINTRESOURCE(ID), IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION);
		BITMAP bmp;
		GetObject(hBmp,sizeof(bmp), &bmp);

		axelynx::Texture::Desc desc;

		desc.width = 	bmp.bmWidth;
		desc.height = bmp.bmHeight;
		desc.cpp = 3;

		tex = new CTexture(desc);

		GLenum format;
		GLenum internalformat;
		GetGLTextures(format,internalformat,desc.cpp,desc.bpc);
		tex->Build(bmp.bmBits,GL_BGR,internalformat);

		DeleteObject(hBmp);

		return tex;
	}

	if(type == 1)
	{
		axelynx::Texture * tex = 0;

		HRSRC hResource = ::FindResourceW(GetModuleHandle(NULL), MAKEINTRESOURCE(ID), L"PNG");
		if (!hResource)
			return false;

		DWORD imageSize = ::SizeofResource(GetModuleHandle(NULL), hResource);
		if (!imageSize)
			return false;

		void* pResourceData = ::LockResource(::LoadResource(GetModuleHandle(NULL),
												  hResource));
		if (!pResourceData)
			return false;

		File f(pResourceData,imageSize);
		axelynx::ImageFormat *ifrm = axelynx::ImageFormat::FindByExt(L"png");
		axelynx::Texture::Desc desc;
		tex = ifrm->LoadTexture(f,desc);

		return tex;
	}

	return 0;
}

axelynx::SceneGraph* CEngine::GetDefaultSceneGraph()
{
	return new StandartSceneGraph();
}