

#ifdef WIN32
#include <cassert>
#include <iostream>
#include "CWindowWin32.h"
#include "CCanvas.h"
#include <axelynx\Logger.h>
#include <MMSystem.h>
#include "CStatistics.h"
LRESULT CALLBACK GLWindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

CWindowWin32* CWindowWin32::instance_ =0;


CWindowWin32::CWindowWin32()
	:wnd_(0),rc_(0),dc_(0),hInstance_(0),isActive_(true),isRunning_(true),maxarea_(false)
{

}


CWindowWin32::~CWindowWin32()
{
}

bool CWindowWin32::Init(axelynx::Window::SystemHandle *handle,int bpp, int samples)
{
	timeBeginPeriod(1);

	RECT                  rect;
	HGLRC                 hRCTemp;
	DWORD                 style, exStyle;
	int                   x, y, format;
	PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB = NULL;
	PIXELFORMATDESCRIPTOR pfd;

// получим дескриптор контекста окна
	wnd_ = reinterpret_cast<HWND>(handle->tag);

	dc_ = GetDC(wnd_);

	if (!dc_)
	{
		LOG_ERROR(L"GetDC fail");
		return false;
	}

	memset(&pfd, 0, sizeof(pfd));
	pfd.nSize      = sizeof(pfd);
	pfd.nVersion   = 1;
	pfd.dwFlags    = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER | PFD_SWAP_EXCHANGE;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 32;
	pfd.cDepthBits = 24;

	// запросим формат пикселей, ближайший к описанному выше
	std::cout<<wglChoosePixelFormat<<std::endl;
	std::cout<<samples<<std::endl;
	SetProcessAffinityMask(reinterpret_cast<HANDLE>(GetCurrentProcessId()), 0x1);

	if( wglChoosePixelFormat !=0 && samples>0) //инициализация windows
	{
		int pixelFormat[1] = {0};
		int valid=0;
		UINT numFormats = 0;
		float fAttributes[] = {0,0};

		int iAttributes[] = {
			WGL_DRAW_TO_WINDOW_ARB,GL_TRUE, // Истинна, если формат пикселя может быть использован в окне
			WGL_SUPPORT_OPENGL_ARB,GL_TRUE, // Истинна, если поддерживается OpenGL
			WGL_ACCELERATION_ARB,WGL_FULL_ACCELERATION_ARB, // Полная аппаратная поддержка
			WGL_COLOR_BITS_ARB,24,          // Цветность
			WGL_ALPHA_BITS_ARB,8,           // Размерность альфа-канала
			WGL_DEPTH_BITS_ARB,24,          // Глубина буфера глубины
			WGL_STENCIL_BITS_ARB,0,         // Глубина буфера шаблона
			WGL_DOUBLE_BUFFER_ARB,GL_TRUE,  // Истина, если используется двойная буферизация
			WGL_SAMPLE_BUFFERS_ARB,(samples>0)?GL_TRUE:GL_FALSE, // Что мы и хотим
			WGL_SAMPLES_ARB, samples ,            // проверка на 4x тип
			0,0};

	//	//iAttributes[19] = samples;

		while((valid==0 || numFormats ==0) && iAttributes[19]>0)
		{
			std::cout<<"ii:"<<iAttributes[19]<<std::endl;

			valid = wglChoosePixelFormat(dc_,iAttributes,fAttributes,1,pixelFormat,&numFormats);
			iAttributes[19]=iAttributes[19]>>1;
			std::cout<<"vaild:"<<valid<<std::endl;
			std::cout<<"numFormats:"<<numFormats<<std::endl;
			std::cout<<"iAttributes[19]:"<<iAttributes[19]<<std::endl;

		}

		if(iAttributes[19] == 16)
			LOG_HINT(L"multisample: 16");
		if(iAttributes[19] == 8)
			LOG_HINT(L"multisample: 8");
		if(iAttributes[19] == 4)
			LOG_HINT(L"multisample: 4");
		if(iAttributes[19] == 2)
			LOG_HINT(L"multisample: 2");
		if(iAttributes[19] == 1)
			LOG_HINT(L"multisample: 1");

		std::cout<<"wgl pixel format:"<<pixelFormat[0]<<std::endl;
		SetPixelFormat( dc_, pixelFormat[0], &pfd );
	}
	else
	{
		format = ChoosePixelFormat(dc_, &pfd);
		if (!format || !SetPixelFormat(dc_, format, &pfd))
		{
			LOG_ERROR(L"Setting pixel format fail");
			return false;
		}
	}

	GetWindowRect((HWND)handle->tag,&rect);

	width_ = rect.right - rect.left;
	height_ = rect.bottom - rect.top;

	// создадим временный контекст рендеринга
	// он нужен для получения функции wglCreateContextAttribsARB
	hRCTemp = wglCreateContext(dc_);
	if (!hRCTemp || !wglMakeCurrent(dc_, hRCTemp))
	{
		LOG_ERROR(L"Creating temp context fail");
		return false;
	}

	PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormat = reinterpret_cast<PFNWGLCHOOSEPIXELFORMATARBPROC>(
		wglGetProcAddress("wglChoosePixelFormatARB"));


	rc_ = hRCTemp;
	if(true)
	{
	    	// укажем атрибуты для создания расширенного контекста OpenGL
        int attribs[] =
        {
            WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
            WGL_CONTEXT_MINOR_VERSION_ARB, 3,
            WGL_CONTEXT_FLAGS_ARB,         WGL_CONTEXT_CORE_PROFILE_BIT_ARB | WGL_CONTEXT_ES2_PROFILE_BIT_EXT,
            WGL_CONTEXT_PROFILE_MASK_ARB,  WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
            0
        };

		// получим адрес функции установки атрибутов контекста рендеринга
		wglCreateContextAttribsARB = reinterpret_cast<PFNWGLCREATECONTEXTATTRIBSARBPROC>(
			wglGetProcAddress("wglCreateContextAttribsARB"));

		if (!wglCreateContextAttribsARB)
		{
			LOG_WARNING(L"wglCreateContextAttribsARB fail");
		}

		// создадим расширенный контекст с поддержкой OpenGL 3
		rc_ = wglCreateContextAttribsARB(dc_, 0, attribs);


		if (!rc_ || !wglMakeCurrent(dc_, rc_))
		{
			LOG_WARNING(L"Creating render context fail");
			rc_ = hRCTemp; // откатываемся на то - что получлось получить
		}
		else
		{
			// временный контекст OpenGL нам больше не нужен
			wglMakeCurrent(NULL, NULL);
			wglDeleteContext(hRCTemp);
			wglMakeCurrent(dc_, rc_);
		}

		for(int c=0;c<count_loading_rc;++c)
		{
			lrc_[c] = wglCreateContextAttribsARB(dc_, rc_, attribs);
			free_lrc_[c] = true;
		}
	}
	// выведем в лог немного информации о контексте OpenGL
	/*int major, minor;
	glGetIntegerv(GL_MAJOR_VERSION, &major);
	glGetIntegerv(GL_MINOR_VERSION, &minor);
	LOG_DEBUG("OpenGL render context information:\n"
		"  Renderer       : %s\n"
		"  Vendor         : %s\n"
		"  Version        : %s\n"
		"  GLSL version   : %s\n"
		"  OpenGL version : %d.%d\n",
		(const char*)glGetString(GL_RENDERER),
		(const char*)glGetString(GL_VENDOR),
		(const char*)glGetString(GL_VERSION),
		(const char*)glGetString(GL_SHADING_LANGUAGE_VERSION),
		major, minor
	);*/

	isFullscreen_ = false;
	Resize(width_,height_);
}


static int s_left=0;
static int s_right=0;
static int s_top=0;
static int s_bottom=0;

#ifndef _MSVCRT_
#define __in
#endif
static BOOL CALLBACK MonitorEnumProc(
  __in  HMONITOR hMonitor,
  __in  HDC hdcMonitor,
  __in  LPRECT lprcMonitor,
  __in  LPARAM dwData
)
{
	if(lprcMonitor->bottom > s_bottom)
	{
		s_bottom = lprcMonitor->bottom;
	}
	if(lprcMonitor->top < s_top)
	{
		s_top = lprcMonitor->top;
	}

	if(lprcMonitor->right > s_right)
	{
		s_right = lprcMonitor->right;
	}
	if(lprcMonitor->left < s_left)
	{
		s_left = lprcMonitor->left;
	}

	return true;
}

static void CalcMaxArea()
{
	EnumDisplayMonitors(0, NULL, MonitorEnumProc, 0);
}

bool CWindowWin32::Init(int width, int height, int bpp, axelynx::WindowMode wm, int samples)
{
	if(width ==0)
	{
		maxarea_ = true;
		CalcMaxArea();

		width = s_right - s_left;
		height = s_bottom - s_top;
	}

	LOG_HINT(L"bool CWindowWin32::Init(int width, int height, int bpp, axelynx::WindowMode wm, int samples)");
	WNDCLASSEXW            wcx;

	RECT                  rect;
	HGLRC                 hRCTemp;
	DWORD                 style, exStyle;
	int                   x, y, format;

	// определим указатель на функцию создания расширенного контекста OpenGL

	hInstance_ = static_cast<HINSTANCE>(GetModuleHandle(NULL));

	// регистрация класса окна
	memset(&wcx, 0, sizeof(wcx));
	wcx.cbSize        = sizeof(wcx);
	wcx.style         = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wcx.lpfnWndProc   = reinterpret_cast<WNDPROC>(GLWindowProc);
	wcx.hInstance     = hInstance_;
	wcx.lpszClassName = L"axelynx";
	wcx.hIcon         = LoadIcon(NULL, IDI_APPLICATION);
	wcx.hCursor       = LoadCursor(NULL, IDC_ARROW);

	if (!RegisterClassExW(&wcx))
	{
		LOG_ERROR(L"RegisterClassEx fail");
		return false;
	}

	// стили окна
	style   = WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;
	exStyle = WS_EX_APPWINDOW;

	// выровняем окно по центру экрана
	x = (GetSystemMetrics(SM_CXSCREEN) - width)  / 2;
	y = (GetSystemMetrics(SM_CYSCREEN) - height) / 2;

	rect.left   = x;
	rect.right  = x + width;
	rect.top    = y;
	rect.bottom = y + height;

	// подгоним размер окна под стили
	AdjustWindowRectEx (&rect, style, FALSE, exStyle);

	// создаем окно
	wnd_ = CreateWindowExW(exStyle, L"axelynx", L"axelynx", style, rect.left, rect.top,
		rect.right - rect.left, rect.bottom - rect.top, NULL, NULL, hInstance_, NULL);

	if (!wnd_)
	{
		LOG_ERROR(L"CreateWindowEx fail");
		return false;
	}

	axelynx::Window::SystemHandle handle;
	handle.tag = reinterpret_cast<int>(wnd_);

// описание формата пикселей


	Init(&handle,bpp,samples);


	Resize(width,height,wm);
	OPENGL_CHECK_FOR_ERRORS();

	return true;
}

bool CWindowWin32::Resize(int width, int height, axelynx::WindowMode wm)
{

	assert(width > 0);
	assert(height > 0);

	width_ = width;
	height_ = height;

	RECT    rect;
	DWORD   style, exStyle;
	DEVMODE devMode;
	LONG    result;
	int     x, y;
	bool fullscreen = (wm == axelynx::WM_FULLSCREEN);

	// если мы возвращаемся из полноэкранного режима
	if (isFullscreen() && !fullscreen)
	{
		ChangeDisplaySettings(NULL, CDS_RESET);
		ShowCursor(TRUE);
	}

	isFullscreen_ = fullscreen;

	// если необходим полноэкранный режим
	if (fullscreen)
	{
		memset(&devMode, 0, sizeof(devMode));
		devMode.dmSize       = sizeof(devMode);
		devMode.dmPelsWidth  = width;
		devMode.dmPelsHeight = height;
		devMode.dmBitsPerPel = GetDeviceCaps(dc_, BITSPIXEL);
		devMode.dmFields     = DM_PELSWIDTH | DM_PELSHEIGHT | DM_BITSPERPEL;

		// попытка установить полноэкранный режим
		result = ChangeDisplaySettings(&devMode, CDS_FULLSCREEN);
		if (result != DISP_CHANGE_SUCCESSFUL)
		{
			LOG_ERROR(L"ChangeDisplaySettings fail");
			isFullscreen_ = false;
		}
	}

	// если был запрошен полноэкранный режим и его удалось установить
	if (isFullscreen_)
	{
		style   = WS_POPUP;
		exStyle = WS_EX_APPWINDOW | WS_EX_TOPMOST;

		x = y = 0;
	} else // если полноэкранный режим не нужен, или его не удалось установить
	{
		if(wm == axelynx::WM_WINDOWED)
		{
			style   = WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;
		}

		if(wm == axelynx::WM_SPLASH)
		{
			style   = WS_POPUP;
		}

		if(wm == axelynx::WM_SIZEBLE)
		{
			style   = WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_SIZEBOX | WS_MAXIMIZEBOX;
		}

		exStyle = WS_EX_APPWINDOW;

		// выровняем окно по центру экрана
		x = (GetSystemMetrics(SM_CXSCREEN) - width)  / 2;
		y = (GetSystemMetrics(SM_CYSCREEN) - height) / 2;
	}

	if(maxarea_)
	{
		rect.left   = s_left;
		rect.right  = s_right;
		rect.top    = s_top;
		rect.bottom = s_bottom;
	}
	else
	{
		rect.left   = x;
		rect.right  = x + width;
		rect.top    = y;
		rect.bottom = y + height;
	}

	// подгоним размер окна под стили
	AdjustWindowRectEx (&rect, style, FALSE, exStyle);

	// установим стили окна
	SetWindowLong(wnd_, GWL_STYLE,   style);
	SetWindowLong(wnd_, GWL_EXSTYLE, exStyle);

	// обновим позицию окна
	SetWindowPos(wnd_, HWND_TOP, rect.left, rect.top,
		rect.right - rect.left, rect.bottom - rect.top,
		SWP_FRAMECHANGED);

	// покажем окно на экране
	ShowWindow(wnd_, SW_SHOW);
	SetForegroundWindow(wnd_);
	SetFocus(wnd_);
	UpdateWindow(wnd_);

	// получим размеры окна
	GetClientRect(wnd_, &rect);
	//g_window.width  = rect.right - rect.left;
	//g_window.height = rect.bottom - rect.top;

	// устанавливаем вьюпорт на все окно
	glViewport(0, 0, width, height);
	glDisable (GL_CULL_FACE);
	glDisable ( GL_DEPTH_TEST );

	// центрируем курсор относительно окна
	SetCursorPos(x + width / 2, y + height / 2);

	return true;
}


bool CWindowWin32::Resize(int width, int height)
{

	assert(width > 0);
	assert(height > 0);

	width_ = width;
	height_ = height;

	// устанавливаем вьюпорт на все окно
	glViewport(0, 0, width, height);
	glDisable (GL_CULL_FACE);
	glDisable ( GL_DEPTH_TEST );

	return true;
}

bool CWindowWin32::Free()
{
	if (isFullscreen())
	{
		ChangeDisplaySettings(NULL, CDS_RESET);
		ShowCursor(TRUE);
	}

	// удаляем контекст рендеринга
	if (rc_)
	{
		wglMakeCurrent(NULL, NULL);
		wglDeleteContext(rc_);
	}

	// освобождаем контекст окна
	if (dc_)
		ReleaseDC(wnd_, dc_);

	// удаляем окно
	if (wnd_)
		DestroyWindow(wnd_);

	// удаляем класс окна
	if (hInstance_)
		UnregisterClassW(L"axelynx", hInstance_);

	return true;
}

bool CWindowWin32::SetText(const wchar_t *unicodetext)
{
	return SetWindowTextW(wnd_,unicodetext)==0;
}

bool CWindowWin32::ReInit(int width, int height, int bpp, axelynx::WindowMode wm, int samples)
{
	Free();
	return Init(width, height, bpp, wm, samples);
}

bool CWindowWin32::Sync()
{
	MSG msg;

	while (PeekMessage(&msg, wnd_, 0, 0, PM_REMOVE))
	{
		if (msg.message == WM_QUIT)
		{
			isRunning_ = false;
			break;
		}
		// TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	Sleep(0);

	return true;
}

bool CWindowWin32::Flip(bool withsync)
{
	//LOG_HINT(L"Flip");
	SwapBuffers(dc_);

	CStatistics::ClearFrame();

	if(withsync)
		Sync();

	return true;
}

CWindowWin32 * CWindowWin32::Instance()
{
	if(!instance_)
		instance_ = new CWindowWin32();

	return instance_;
}

int CWindowWin32::VSync(bool enabled)
{
	if(wglSwapInterval)
	{
		wglSwapInterval(enabled?1:0);
		wglSwapInterval(enabled?1:0);
	}
	return 1;
}

HGLRC CWindowWin32::GetRenderingContext()
{
	wglMakeCurrent(dc_,rc_);
	return rc_;
}

HGLRC CWindowWin32::GetResourceContext()
{
	wglMakeCurrent(dc_,lrc_[0]);
	return lrc_[0];
}

int CWindowWin32::GetFreeLoadingRC()
{
	int free = -1;
	int c = 0;
	while(true)
	{
		if(free_lrc_[c])
		{
			wglMakeCurrent(dc_,lrc_[c]);
			free_lrc_[c] = false;
			return c;
		}

		if(c>=count_loading_rc)
			c = 0;

		Sleep(0);
	}
}

bool CWindowWin32::FreeLoadingRC(int rc)
{
	wglMakeCurrent(dc_,0);
	free_lrc_[rc] = true;
	return true;
}

LRESULT CALLBACK GLWindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
		case WM_SETFOCUS:
			CWindowWin32::Instance()->isActive_ = true;
			return FALSE;

		case WM_KILLFOCUS:
			CWindowWin32::Instance()->isActive_ = false;
			return FALSE;

		case WM_ACTIVATE:
			CWindowWin32::Instance()->isActive_ =  (LOWORD(wParam) == WA_INACTIVE);
			return FALSE;

		case WM_CLOSE:
			CWindowWin32::Instance()->isActive_ = CWindowWin32::Instance()->isRunning_ = false;
			PostQuitMessage(0);
			return FALSE;

		case WM_KEYDOWN:
			if(wParam == VK_LBUTTON)
			{
				int a =1;
			}
			break;
		case WM_SIZE:
			CCanvas::ResizeCanvas(LOWORD(lParam), HIWORD(lParam));
			return FALSE;

		case WM_SYSCOMMAND:
			switch (wParam & 0xFFF0)
			{
				case SC_SCREENSAVE:
				case SC_MONITORPOWER:
					if (CWindowWin32::Instance()->isFullscreen())
						return FALSE;
					break;

				case SC_KEYMENU:
					return FALSE;
			}
			break;

		case WM_ERASEBKGND:
			return FALSE;
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}

bool CWindowWin32::ChangeResolution(int width, int height, axelynx::WindowMode wm)
{
	if(width ==0)
	{
		width =GetSystemMetrics(SM_CXSCREEN);
		height =GetSystemMetrics(SM_CYSCREEN);
		wm = axelynx::WM_FULLSCREEN;
	}

	return Resize(width, height, wm);
}

#endif
