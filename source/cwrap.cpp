#include "axelynx/cwrap.h"

#include "axelynx/axelynx.h"

using namespace axelynx;

static Engine *engine = 0;
static Canvas *canvas = 0;

AXELYNX_API void	axCreateEnging()
{
	if(!engine)
	{
		engine = Engine::Init();
	}
}

AXELYNX_API void	axFreeEnging()
{
	if(engine)
	{
		engine->Free();
		delete engine;
		engine = 0;
		canvas = 0;
	}
}

AXELYNX_API INT		axCreateWindow(INT32 width, INT32 height, INT32 type, INT32 samples)
{
	if(engine)
	{
		Window *wnd = engine->AddWindow(width,height,32,axelynx::WindowMode(type),samples);

		canvas = engine->GetCanvas();

		return reinterpret_cast<INT>(wnd);
	}

	return 0;
}

AXELYNX_API INT		axCreateWindowOn(INT32 hwnd, INT32 bpp, INT32 samples)
{
	if(engine)
	{
		axelynx::Window::SystemHandle handle;
		handle.tag = hwnd;

		Window *wnd = engine->ApplyWindow(&handle,bpp,samples);
		canvas = engine->GetCanvas();
		return reinterpret_cast<INT>(wnd);
	}
}

AXELYNX_API void	axFreeWindow(INT window)
{
	if(engine)
	{
		if(window)
		{
			Window *wnd = reinterpret_cast<Window*>(window);
			wnd->Free();
		}
	}
}

AXELYNX_API void	axFlip(INT window)
{
	if(engine)
	{
		if(window)
		{
			Window *wnd = reinterpret_cast<Window*>(window);
			wnd->Flip();
			wnd->Sync();

				//shell::ShowMessage(L"axFlip");
		}
	}
}

AXELYNX_API void	axClearColor(float red, float green, float blue, float alpha)
{
	if(canvas)
	{
		canvas->ClearColor(red,green,blue,alpha);
			//shell::ShowMessage(L"axClearColor");
	}
}

AXELYNX_API void	axClear()
{
	if(canvas)
	{
		canvas->Clear();
		//shell::ShowMessage(L"axClear");
	}
}
