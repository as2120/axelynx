#pragma once

#include "axelynx/Engine.h"
#include "axelynx/Logger.h"
#include "axelynx/Window.h"
#include "axelynx/Scene.h"
#include "CPhysicsContext.h"

#include "CTexture.h"

#include <list>
#undef LoadImage
class CEngine : public axelynx::Engine
{
	static CEngine * instance_;

	axelynx::Window *syswnd;
	axelynx::Canvas *canvas_;

	axelynx::Engine::TSettings settings_;
	
	
	struct ASyncTextureLoadData
	{
		std::wstring format;
		axelynx::File file;
		axelynx::Texture::Desc desc;
		CTexture *tex;
	};

	typedef std::list<ASyncTextureLoadData> AsyncTexturesList;
	static  AsyncTexturesList *async_textures_list;

	static void __cdecl LoadTextureAsync(void *param);
	void InitOpenGL();
public:

	virtual axelynx::Window* AddWindow_(int width, int height, int bpp, axelynx::WindowMode wm, int samples);
	virtual axelynx::Window* ApplyWindow(axelynx::Window::SystemHandle *handle,int bpp, int samples);

	virtual CBulletPhysicsContext* AddPhysicsContext();

	virtual bool Free();
	virtual bool Sync();
	virtual bool Flip();

	virtual bool isActive();
	virtual bool isRunning();

	virtual bool SetText(const wchar_t * text);
	virtual axelynx::Canvas* GetCanvas();

	virtual bool Render(bool clear=false);
	virtual bool Update(float twin = 1.0f);

	virtual axelynx::PixMap* CreatePixMap(int width, int height, int bpp);
	virtual axelynx::Texture* CreateTexture(axelynx::Texture::Desc &desc);
	virtual axelynx::Texture* CreateDepthTexture(int width, int height);

	virtual axelynx::Texture* LoadTextureFromResource(int ID, int type);

	virtual axelynx::PixMap* LoadPixMap(axelynx::File file, const wchar_t *format);
	virtual axelynx::Texture* LoadTexture(axelynx::File filename,axelynx::Texture::Desc &desc, const wchar_t *format);
	
	virtual axelynx::Texture* Load3DTexture(axelynx::File filename, int width, int height, int count_x, int count_y,axelynx::Texture::Desc &desc, const wchar_t *format);
	virtual axelynx::Texture* LoadTextureArray(axelynx::File filename, int width, int height, int count_x, int count_y,axelynx::Texture::Desc &desc, const wchar_t *format);

	virtual axelynx::Texture* LoadCubeMapTexture(axelynx::File filename,axelynx::Texture::Desc &desc, const wchar_t *format);
	
	virtual axelynx::Texture* LoadCubeMapTexture(axelynx::File filename_neg_x
		,axelynx::File filename_pos_x
		,axelynx::File filename_neg_y
		,axelynx::File filename_pos_y
		,axelynx::File filename_neg_z
		,axelynx::File filename_pos_z
		,axelynx::Texture::Desc &desc, const wchar_t *format);

	virtual axelynx::Shader* CreateBlankShader();
	virtual axelynx::Shader* LoadShader(axelynx::File file);
	virtual axelynx::Shader* LoadShaderBinary(axelynx::File file);
	virtual axelynx::Shader* LoadShader(axelynx::File vsfile, axelynx::File fsfile);
	virtual axelynx::Shader* LoadShader(axelynx::File vsfile, axelynx::File fsfile, axelynx::File gsfile);
	virtual axelynx::Shader* CreateShader(const char* vertexsource, const char* fragmentsource=0, const char* geometrysource=0);

	virtual axelynx::ImageSystem* LoadImageSystem(axelynx::File filename, int capaticity_, const wchar_t *format);
	
	virtual axelynx::ImageSystem* CreateImageSystem(const axelynx::PixMap* pixmap, int capaticity_);
	virtual axelynx::Image* CreateImage(const axelynx::PixMap* pixmap, int capaticity_);

	virtual axelynx::Sprite *CreateSprite(float size = 1.0f);
	virtual axelynx::Sprite *LoadSprite(axelynx::File filename,float size, const wchar_t *format);

	virtual axelynx::Engine::Info GetInfo();

	virtual axelynx::Scene* AddScene();

	virtual axelynx::Surface *CreateSurface(int vertices, int tris);
	virtual axelynx::Surface *LoadSurface(axelynx::File filename, const wchar_t *format);

	virtual axelynx::Material* CreateMaterial();

	virtual axelynx::RenderTarget* CreateRenderTarget();
	virtual axelynx::RenderTarget* CreateRenderTarget(int width, int height);

	virtual axelynx::Font* LoadFont(const axelynx::Font::Desc& desc_);
	virtual axelynx::Font* LoadHGEFont(const axelynx::Font::Desc& desc_);
	virtual axelynx::Font* LoadFreeTypeFont(const axelynx::Font::Desc& desc_);

	virtual axelynx::Statistics* GetStatistics();
	virtual axelynx::HardwareInfo* GetHardwareInfo();
	virtual void VSync(bool enabled);

	static axelynx::Engine * Instance();

	virtual axelynx::SpriteSystem * CreateSpriteSystem(int capacity = 16);
	virtual axelynx::SpriteSystem * LoadSpriteSystem(axelynx::File filename, const wchar_t *format);

	virtual axelynx::ParticleSystem * CreateParticleSystem();
	virtual axelynx::ParticleSystem * LoadParticleSystem(axelynx::File filename);

	virtual axelynx::RaySystem * CreateRaySystem(int capacity = 16);

	virtual axelynx::Image* LoadImage(axelynx::File vs_file, int capaticity_=16384, const wchar_t* format=L"");
	virtual axelynx::Theora * LoadTheoraVideo(axelynx::File filename);

	virtual axelynx::PostEffect* CreatePostEffect(int width = 0, int height = 0, int channels = 3, int channelsize = 1);
	virtual axelynx::PostEffect* LoadPostEffect(axelynx::File file);

	virtual axelynx::InstanceGroup* CreateInstanceGroup();

	virtual axelynx::Texture* RestoreTexture(axelynx::File file);
	virtual axelynx::Surface* RestoreSurface(axelynx::File file);

	virtual axelynx::MorfedMesh* LoadMorfedMesh(axelynx::File file, const wchar_t* format);
	virtual axelynx::Engine::TSettings& Settings()
	{
		return settings_;
	}

	virtual axelynx::SceneGraph* GetDefaultSceneGraph();
	CEngine();
	~CEngine();
};
