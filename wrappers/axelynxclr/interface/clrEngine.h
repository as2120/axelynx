#pragma once
#include <axelynx/axelynx.h>

public ref class Engine
	{
		axelynx::Engine *eng;
	public:
		Engine(axelynx::Engine *eng_):eng(eng_){}

		ref class Info
		{
		public:
			int version;
			String^ renderer;
			String^ vendor;
			String^ driver;
			int major,minor;
			String^ glsl;
		};

		ref class Settings
		{
		public:
			ref class ShadersSettings
			{
			public:
				bool UseCash;
				bool ShowLogOnFailCompile;
			};

			ref class RendererSettings
			{
			public:
				bool UseMaterialSorting;
				int AlphaSortingThresold;
				int MaxUnpackedVertices;
			};

			ref class FileSystemSettings
			{
			public:
				enum class FileExistsLevel_Enum
				{
					NO_WARNINGS
					,LOG_WARNING
					,MESSAGE_WARNING
					,FATAL
				};

				FileExistsLevel_Enum FileExistsLevel; //0 - no warnings, 1 - log warning, 2 - message warning, 3 - log warning + message error
			};

			ref class DebugSettings
			{
			public:
				int html_log;
				bool UseRunTimeProfiler;
			};

			ref class SystemSettings
			{
			public:
				bool LowMemoryMode;
				bool LowVideoMemoryMode;
			};

			RendererSettings Renderer;
			FileSystemSettings FileSystem;
			DebugSettings Debug;
			SystemSettings System;
			ShadersSettings Shaders;
		};
		
		Window^ AddWindow(int width, int height, int bpp, WindowMode wm, int samples)
		{
			return gcnew Window(eng->AddWindow(width,height,bpp,static_cast<axelynx::WindowMode>(wm),samples));
		}

		Window^ ApplyWindow(IntPtr handle,int bpp, int samples)
		{
			axelynx::Window::SystemHandle temp;
			temp.tag = static_cast<int>(handle);

			return gcnew Window(eng->ApplyWindow(&temp,bpp,samples));
		}

		Canvas^ GetCanvas()
		{
			return gcnew Canvas(eng->GetCanvas());
		}

		Font^ LoadFreeTypeFont(String^ fontname, int fontsize)
		{
			pin_ptr<const wchar_t> wch = PtrToStringChars(fontname);
			static axelynx::Font::Desc desc(wch);
			int t = sizeof(axelynx::Font::Desc);
			//desc.size = fontsize;

			axelynx::Font *ptr = eng->LoadFreeTypeFont(desc);

			//eng->GetCanvas()->SetFont(eng->LoadFreeTypeFont(desc));
			return gcnew Font(ptr);
		}

		static Engine^ Init()
		{
			return gcnew Engine(axelynx::Engine::Init(axelynx::AXELYNX_VERSION));
		}

		bool Free()
		{
			return eng->Free();
		}


		/*



		virtual PixMap* CreatePixMap(int width, int height, int bpp)=0;
		virtual Texture* CreateTexture(Texture::Desc &desc)=0;
		virtual Texture* CreateDepthTexture(int width, int height) = 0;
		

		PixMap* LoadPixMap(std::wstring filename)
		{
			return LoadPixMap(File(filename), utils::GetExtension(filename).c_str());
		}

		virtual PixMap* LoadPixMap(File file, const wchar_t* format)=0;

		Texture* Load3DTexture(File file, int width, int height, int count_x, int count_y)
		{
			Texture::Desc desc;
			return Load3DTexture(file, width, height, count_x, count_y,desc);
		}
		virtual Texture* Load3DTexture(File file, int width, int height, int count_x, int count_y,Texture::Desc &desc, const wchar_t* format=L"")=0;

		Texture* LoadTextureArray(File file, int width, int height, int count_x, int count_y)
		{
			Texture::Desc desc;
			return LoadTextureArray(file, width, height, count_x, count_y,desc);
		}
		virtual Texture* LoadTextureArray(File file, int width, int height, int count_x, int count_y,Texture::Desc &desc, const wchar_t* format=L"")=0;

		
		Texture* LoadTexture(File file)
		{
			Texture::Desc desc;
			return LoadTexture(file,desc);
		}

		virtual Texture* LoadTexture(File file,Texture::Desc &desc, const wchar_t* format=L"")=0;

		virtual Texture* RestoreTexture(File file)=0;
		virtual Surface* RestoreSurface(File file)=0;

		virtual Texture* LoadCubeMapTexture(File file_pos_x
			,File file_neg_x
			,File file_pos_y
			,File file_neg_y
			,File file_pos_z
			,File file_neg_z
			,Texture::Desc &desc, const wchar_t* format=L"")=0;

		Texture* LoadCubeMapTexture(File file_pos_x
			,File file_neg_x
			,File file_pos_y
			,File file_neg_y
			,File file_pos_z
			,File file_neg_z
			)
		{
			Texture::Desc desc;
			return LoadCubeMapTexture(file_pos_x,file_neg_x,file_pos_y,file_neg_y,file_pos_z,file_neg_z,desc);
		}

		virtual Texture* LoadCubeMapTexture(File file,Texture::Desc &desc, const wchar_t* format=L"")=0;

		Texture* LoadCubeMapTexture(File file)
		{
			Texture::Desc desc;
			return LoadCubeMapTexture(file,desc);
		}

		virtual Shader* CreateBlankShader()=0;
		virtual Shader* LoadShader(File file)=0;
		virtual Shader* LoadShaderBinary(File file)=0;
		virtual Shader* LoadShader(File vs_file, File fs_file)=0;
		virtual Shader* LoadShader(File vs_file, File fs_file, File gs_file)=0;
		virtual Shader* CreateShader(const char* vertexsource, const char* fragmentsource=0, const char* geometrysource=0)=0;

		virtual ImageSystem* LoadImageSystem(File vs_file, int capaticity_=16384, const wchar_t* format=L"")=0;
		virtual Image* LoadImage(File vs_file, int capaticity_=16384, const wchar_t* format=L"")=0;
		virtual ImageSystem* CreateImageSystem(const PixMap* pixmap, int capaticity_=16384)=0;
		virtual Image* CreateImage(const PixMap* pixmap, int capaticity_=16384)=0;

		virtual Scene* AddScene()=0;

		virtual Sprite *CreateSprite(float size = 1.0f)=0;
		virtual Sprite *LoadSprite(File file,float size = 1.0f, const wchar_t* format=L"")=0;

		virtual Surface *CreateSurface(int vertices, int tris)=0;
		virtual Surface *LoadSurface(File file, const wchar_t* format=L"")=0;

		virtual Material* CreateMaterial()=0;

		virtual RenderTarget* CreateRenderTarget()=0;
		virtual RenderTarget* CreateRenderTarget(int width, int height)=0;

		virtual Font* LoadFont(const Font::Desc& desc_)=0;
		virtual Font* LoadHGEFont(const Font::Desc& desc_)=0;


		virtual SpriteSystem * CreateSpriteSystem(int capacity = 16)=0;
		virtual SpriteSystem * LoadSpriteSystem(File file, const wchar_t* format)=0;

		virtual ParticleSystem * CreateParticleSystem()=0;
		virtual ParticleSystem * LoadParticleSystem(File file)=0;

		virtual RaySystem * CreateRaySystem(int capacity = 16) = 0;

		virtual Theora* LoadTheoraVideo(File file) = 0;
		virtual PostEffect* CreatePostEffect(int width = 0, int height = 0, int channels = 3, int channelsize = 1)=0;
		virtual PostEffect* LoadPostEffect(File file) = 0;

		virtual InstanceGroup* CreateInstanceGroup() = 0;
		Window* AddWindow()
		{
			return AddWindow_(0, 0, 32, WM_FULLSCREEN, 0);
		}

		Window* AddWindow(int width, int height, int bpp=32, WindowMode wm = WM_WINDOWED, int samples=0)
		{
			return AddWindow_(width, height, bpp, wm, samples);
		}

		virtual Info GetInfo()=0;
		virtual Statistics* GetStatistics()=0;
		virtual void VSync(bool enabled)=0;

		AXELYNX_API static axelynx::Engine * Instance();

		virtual struct Settings& Settings()=0;
		virtual ~Engine(){};
		*/
	};