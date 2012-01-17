#include "axelynx/Keyboard.h"


#ifdef WIN32
#include <windows.h>



bool axelynx::KeyDown(int keystate)
{

	return GetAsyncKeyState(keystate)!=0;
}

bool axelynx::KeyHit(int keystate)
{
	static bool keys[255];

	if(!keys[keystate])
	{
		if(GetAsyncKeyState(keystate)!=0)
		{
			keys[keystate] = true;
			return true;
		}
	}
	else
	{
		if(GetAsyncKeyState(keystate)==0)
		{
			keys[keystate] = false;
		}
	}

	return false;
}

bool axelynx::KeyUp(int keystate)
{
	static bool keys[255];

	if(!keys[keystate])
	{
		if(GetAsyncKeyState(keystate)!=0)
		{
			keys[keystate] = true;
			return false;
		}
	}
	else
	{
		if(GetAsyncKeyState(keystate)==0)
		{
			keys[keystate] = false;
			return true;
		}
	}

	return false;
}

#endif

#ifdef UNIX

bool axelynx::KeyDown(int keystate)
{

	return 0;
}

bool axelynx::KeyHit(int keystate)
{

	return 0;
}
#endif
