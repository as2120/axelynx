#pragma once

#include "Window.h"
#include <X11/X.h>    /* X11 constant (e.g. TrueColor) */
#include "gl/axgl.h"

class CWindowUnix : public axWindow
{
    Display   *dpy_;
    Window     win_;
    GLXContext cx_;
	bool isFullscreen_;
    bool doubleBuffer_;
	static CWindowUnix *instance_;

	CWindowUnix();
	~CWindowUnix();

	int width_, height_;

public:
	virtual bool Init(int width, int height, int bpp, axelynx::WindowMode wm, int samples);
	virtual bool Free();
	virtual bool Resize(int width, int height, axelynx::WindowMode wm);
	virtual bool SetText(const wchar_t *unicodetext);

	virtual bool ReInit(int width, int height, int bpp, axelynx::WindowMode wm, int samples);

	virtual bool Sync();
	virtual bool Flip();

	static bool isFullscreen() {return instance_?instance_->isFullscreen_:0;}

	static CWindowUnix * Instance();

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
};
