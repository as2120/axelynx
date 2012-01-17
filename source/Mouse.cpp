#include <axelynx\Mouse.h>

#ifdef WIN32
#include <windows.h>
#include "CWindowWin32.h"

int AXELYNX_API axelynx::MouseX()
{
	POINT mp;
	GetCursorPos(&mp);

	ScreenToClient(CWindowWin32::GetHandle(),&mp);
	return mp.x;
}

int AXELYNX_API axelynx::MouseY()
{
	POINT mp;
	GetCursorPos(&mp);

	ScreenToClient(CWindowWin32::GetHandle(),&mp);
	return mp.y;
}

axelynx::vec2 AXELYNX_API axelynx::MousePos()
{
	POINT mp;
	GetCursorPos(&mp);

	ScreenToClient(CWindowWin32::GetHandle(),&mp);
	return axelynx::vec2(static_cast<float>(mp.x),static_cast<float>(mp.y));
}

bool AXELYNX_API axelynx::SetMousePos(const axelynx::vec2& position)
{
	SetCursorPos(static_cast<int>(position.x)
				,static_cast<int>(position.y));

	return true;
}

bool AXELYNX_API axelynx::SetMousePos(int x, int y)
{
	return true;
}

#endif

#ifdef UNIX
int AXELYNX_API axelynx::MouseX()
{
	return 0;
}

int AXELYNX_API axelynx::MouseY()
{
	return 0;
}

axelynx::vec2 AXELYNX_API axelynx::MousePos()
{
	return axelynx::vec2();
}

bool AXELYNX_API axelynx::SetMousePos(const axelynx::vec2& position)
{
	return true;
}

bool AXELYNX_API axelynx::SetMousePos(int x, int y)
{
	return true;
}
#endif
