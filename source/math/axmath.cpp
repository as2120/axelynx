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


AXELYNX_API bool axelynx::LineSphereIntersect(const axelynx::vec3& base, const axelynx::vec3& dir, const axelynx::vec4& sphere)
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

AXELYNX_API void axelynx::CalcTriangleBasis( const axelynx::vec3& E, const axelynx::vec3& F, const axelynx::vec3& G, float sE,
		float tE, float sF, float tF, float sG, float tG, axelynx::vec3& tangentX,
		axelynx::vec3& tangentY )
{
	using namespace axelynx;

	vec3 P = F - E;
	vec3 Q = G - E;
	float s1 = sF - sE;
	float t1 = tF - tE;
	float s2 = sG - sE;
	float t2 = tG - tE;
	float pqMatrix[2][3];
	pqMatrix[0][0] = P[0];
	pqMatrix[0][1] = P[1];
	pqMatrix[0][2] = P[2];
	pqMatrix[1][0] = Q[0];
	pqMatrix[1][1] = Q[1];
	pqMatrix[1][2] = Q[2];
	float temp = 1.0f / ( s1 * t2 - s2 * t1);
	float stMatrix[2][2];
	stMatrix[0][0] =  t2 * temp;
	stMatrix[0][1] = -t1 * temp;
	stMatrix[1][0] = -s2 * temp;
	stMatrix[1][1] =  s1 * temp;
	float tbMatrix[2][3];
	// stMatrix * pqMatrix
	tbMatrix[0][0] = stMatrix[0][0] * pqMatrix[0][0] + stMatrix[0][1] * pqMatrix[1][0];
	tbMatrix[0][1] = stMatrix[0][0] * pqMatrix[0][1] + stMatrix[0][1] * pqMatrix[1][1];
	tbMatrix[0][2] = stMatrix[0][0] * pqMatrix[0][2] + stMatrix[0][1] * pqMatrix[1][2];
	tbMatrix[1][0] = stMatrix[1][0] * pqMatrix[0][0] + stMatrix[1][1] * pqMatrix[1][0];
	tbMatrix[1][1] = stMatrix[1][0] * pqMatrix[0][1] + stMatrix[1][1] * pqMatrix[1][1];
	tbMatrix[1][2] = stMatrix[1][0] * pqMatrix[0][2] + stMatrix[1][1] * pqMatrix[1][2];

	tangentX.x =  tbMatrix[0][0];
	tangentX.y =  tbMatrix[0][1];
	tangentX.z =  tbMatrix[0][2];


	tangentY.x = tbMatrix[1][0];
	tangentY.y = tbMatrix[1][1];
	tangentY.z = tbMatrix[1][2];
	tangentX.normalize();
	tangentY.normalize();
}

AXELYNX_API axelynx::vec3 axelynx::ClosestPointOnLine( const axelynx::vec3& a, const axelynx::vec3& b, const axelynx::vec3& p )
{
	using namespace axelynx;

	vec3 c = p - a;
	vec3 V = b - a;
	float d = V.length();
	V.normalize();
	float t = V.dot(c);		// скалярное произведение векторов
 
	// проверка на выход за границы отрезка
	if ( t < 0.0f )
		return a;
	if ( t > d )
		return b;
 
	// Вернем точку между a и b
	V *= t;
	return ( a + V );
}

AXELYNX_API axelynx::vec3 axelynx::Ortogonalize( const axelynx::vec3& v1, const axelynx::vec3& v2 )
{
	using namespace axelynx;

	vec3 v2ProjV1 = ClosestPointOnLine( v1, -v1, v2 );
	vec3 res = v2 - v2ProjV1;
	res.normalize();
	return res;
}