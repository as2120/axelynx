#include "axelynx/Timer.h"
#include "CTimer.h"

#ifdef WIN32
#include <Windows.h>
static LONG g_frencky;
#endif

#ifdef UNIX
#include <sys/time.h>
#endif



AXELYNX_API int axelynx::Timer::Millisecs()
{
#ifdef WIN32
	return GetTickCount();
#endif
#ifdef UNIX
    struct timeval t;
    gettimeofday( &t, 0 );
    return ((t.tv_sec) * 1000) + (t.tv_usec / 1000);
#endif
	return 0;
}

AXELYNX_API float axelynx::Timer::Microsecs()
{
#ifdef WIN32
	return static_cast<float>(GetTickCount());
#endif
	return 0;
}


bool InitTimer()
{


	return true;
}

AXELYNX_API float axelynx::Timer::Delta()
{
	float deltaTime;
    static __int64 gTime,gLastTime;
    __int64 freq;
    QueryPerformanceCounter((LARGE_INTEGER *)&gTime);  // Get current count
    QueryPerformanceFrequency((LARGE_INTEGER *)&freq); // Get processor freq
    deltaTime = (float)((double)(gTime - gLastTime)*1000.0f/(double)freq);
    gLastTime = gTime;
    return deltaTime;
}

AXELYNX_API void axelynx::Timer::Wait(int millisecs)
{
	Sleep(millisecs);
}