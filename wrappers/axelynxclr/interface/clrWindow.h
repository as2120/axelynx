#pragma once
#include <axelynx/axelynx.h>

public enum class WindowMode
{
	WINDOWED,
	FULLSCREEN,
	SPLASH,
	SIZEBLE
};

public ref class Window
{
	axelynx::Window *obj;
public:
	Window(axelynx::Window *obj_):obj(obj_){}

	bool Init(int width, int height, int bpp, WindowMode wm, int samples)
				{return obj->Init(width,height,bpp,static_cast<axelynx::WindowMode>(wm),samples);}
	bool Init(IntPtr handle,int bpp, int samples)
				{
					axelynx::Window::SystemHandle temp;
					temp.tag = static_cast<int>(handle);
					return obj->Init(&temp,bpp,samples);
				}
	bool ChangeResolution(int width, int height, WindowMode wm)
				{return obj->ChangeResolution(width,height,static_cast<axelynx::WindowMode>(wm));}
	bool Free()
				{return obj->Free();}
	bool SetText(String^ str)
				{
					pin_ptr<const wchar_t> wch = PtrToStringChars(str);
					return obj->SetText(wch);
				}
	bool ReInit(int width, int height, int bpp, WindowMode wm, int samples)
				{return obj->ReInit(width,height,bpp,static_cast<axelynx::WindowMode>(wm),samples);}
	bool Sync()
				{return obj->Sync();}
	bool Flip(bool sync)
				{return obj->Flip(sync);}
	bool Resize(int width, int height, WindowMode wm)
				{return obj->Resize(width,height,static_cast<axelynx::WindowMode>(wm));}
	bool isActive()
				{return obj->isActive();}
	bool isRunning()
				{return obj->isRunning();}
	int GetWidth()
				{return obj->GetWidth();}
	int GetHeigh()
				{return obj->GetHeight();}
	int VSync(bool enabled)
				{return obj->VSync(enabled);}
};