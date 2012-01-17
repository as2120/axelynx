#pragma once
#include "Window.h"
#include <cstring>

#ifdef WIN32
#include <Windows.h>
namespace axelynx
{
	class Win32Handle : public Window::SystemHandle
	{
		HWND hwnd;
	public:
		Win32Handle(HWND wnd)
			:hwnd(wnd){}

		virtual int HandleSize()
		{
			return sizeof(HWND);
		}
		virtual bool Handle(void *ptr)
		{
			memcpy(ptr,&hwnd,sizeof(hwnd));
		}
	}
};

#endif