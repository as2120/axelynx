#pragma once

#include "axelynx/Window.h"
#include <windows.h>
#include "gl/axgl.h"

class CWindowWin32 : public axelynx::Window
{
	static const int count_loading_rc = 1;

	HWND wnd_;
	HGLRC rc_;
	HGLRC lrc_[count_loading_rc];
	bool free_lrc_[count_loading_rc];
	bool maxarea_;

	HDC dc_;
	HINSTANCE hInstance_;

	bool isFullscreen_;

	static CWindowWin32 *instance_;

	CWindowWin32();
	~CWindowWin32();

	int width_, height_;
	bool Resize(int width_,int height_);
public:
	virtual bool Init(int width, int height, int bpp, axelynx::WindowMode wm, int samples);
	virtual bool Init(axelynx::Window::SystemHandle *handle,int bpp, int samples);

	virtual bool ChangeResolution(int width = 0, int height = 0, axelynx::WindowMode wm = axelynx::WM_WINDOWED);

	virtual bool Free();
	virtual bool Resize(int width, int height, axelynx::WindowMode wm);
	virtual bool SetText(const wchar_t *unicodetext);

	virtual bool ReInit(int width, int height, int bpp, axelynx::WindowMode wm, int samples);

	virtual bool Sync();
	virtual bool Flip(bool sync = true);

	static HWND GetHandle()	{return instance_?instance_->wnd_:0;};
	static bool isFullscreen() {return instance_?instance_->isFullscreen_:0;}

	static CWindowWin32 * Instance();

	virtual bool isActive()
	{
		return isActive_;
	};

	bool isActive_;

	virtual bool isRunning()
	{
		return isRunning_;
	};

	bool isRunning_;

	virtual int GetWidth()
	{
		return width_;
	}

	virtual int GetHeight()
	{
		return height_;
	}

	virtual int VSync(bool enabled);

	HGLRC GetRenderingContext();
	HGLRC GetResourceContext();

	int GetFreeLoadingRC();
	bool FreeLoadingRC(int rc);
};
