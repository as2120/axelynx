#pragma once

namespace axelynx
{
	enum WindowMode
	{WM_WINDOWED, WM_FULLSCREEN, WM_SPLASH, WM_SIZEBLE};

	class Window
	{
		public:
		class SystemHandle
		{
		public:
			int tag;

			template<typename T>
			SystemHandle(T wnd)
				:tag(reinterpret_cast<int>(wnd))
			{
			}

			SystemHandle()
				:tag(0)
			{
			}
		};

		virtual bool Init(int width, int height, int bpp, axelynx::WindowMode wm, int samples) =0;
		virtual bool Init(SystemHandle *handle,int bpp, int samples) = 0;

		virtual bool ChangeResolution(int width = 0, int height = 0, axelynx::WindowMode wm = WM_WINDOWED) = 0;
		virtual bool Free()=0;

		virtual bool SetText(const wchar_t *unicodetext)=0;

		virtual bool ReInit(int width, int height, int bpp, axelynx::WindowMode wm, int samples)=0;

		virtual bool Sync()=0;
		virtual bool Flip(bool sync = true)=0;

		virtual bool Resize(int width, int height, axelynx::WindowMode wm)=0;
		virtual bool isActive() = 0;
		virtual bool isRunning() = 0;

		virtual int GetHeight() =0;
		virtual int GetWidth() =0;

		virtual int VSync(bool enabled)=0;
	};
}