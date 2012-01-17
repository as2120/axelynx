#include <axelynx/math/axmath.h>
#include <cstdlib>
#include <cmath>

AXELYNX_API int FAST_CALL axelynx::rand(int max)
{
	return ::rand() % max;
}

AXELYNX_API int FAST_CALL axelynx::rand(int min, int max)
{
	return ::rand() % (max + 1 - min) + min;
}

#ifdef WIN32
AXELYNX_API float FAST_CALL axelynx::rnd(float max)
{
	double d = ::rand() * RAND_MAX + ::rand();

	return (float)((d * max) / double(static_cast<long>(RAND_MAX)*static_cast<long>(RAND_MAX)));
}

AXELYNX_API float FAST_CALL axelynx::rnd(float min, float max)
{
	return axelynx::rnd(max-min) + min;
}
#endif

#ifdef UNIX
AXELYNX_API float axelynx::rnd(float max)
{
	return (float)((::rand() * max) / double(RAND_MAX));
}

AXELYNX_API float axelynx::rnd(float min, float max)
{
	return axelynx::rnd(max-min) + min;
}
#endif


AXELYNX_API  void axelynx::sincos(float a,float & s,float & c )
{
#if defined(_WIN) && (_MSC_VER > 1200)
	_asm {
		fld		a
		fsincos
		mov		ecx, c
		mov		edx, s
		fstp	dword ptr [ecx]
		fstp	dword ptr [edx]
	}
#else
	s = sinf( a );
	c = cosf( a );
#endif
}


AXELYNX_API bool FAST_CALL  axelynx::LineSphereIntersect(const axelynx::vec3& base, const axelynx::vec3& dir, const axelynx::vec4& sphere)
{
	using namespace axelynx;

	float dist_len  = dir.length();
    vec3 dist       = dir*(1.0f/dist_len);
    const vec3& center = axelynx::vec3(sphere.x,sphere.y,sphere.z);
	const float &radius = sphere.w;

    float proj      = (center-base).dot(dist);

    if (proj>dist_len+radius || proj<-radius)
        return false;
        
    vec3 proj_vec   = base+(dist*proj);
    float rad_sqr   = (proj_vec-center).length();

    if (rad_sqr>radius)
        return false;
    else
        return true;
}