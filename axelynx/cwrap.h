#pragma once
#include "axlib.h"

#ifdef _MSVCRT_
#include <intsafe.h>
#else
typedef unsigned int INT;
typedef int INT32;
#endif

extern "C"
{
	AXELYNX_API void	axCreateEnging();
	AXELYNX_API void	axFreeEnging();
	AXELYNX_API INT		axCreateWindow(INT32 width, INT32 height, INT32 type, INT32 samples);
	AXELYNX_API INT		axCreateWindowOn(INT32 hwnd, INT32 bpp, INT32 samples);
	AXELYNX_API void	axFreeWindow(INT window);
	AXELYNX_API void	axFlip(INT window);
	AXELYNX_API void	axClearColor(float red, float green, float blue, float alpha);
	AXELYNX_API void	axClear();
}
