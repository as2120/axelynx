#include <axelynx/math/mat4.h>
#include <cmath>

AXELYNX_API axelynx::mat4 axelynx::mat4::Identity()
{
	axelynx::mat4 m;

	m.m[1] = m.m[2] = m.m[3] = m.m[4] = m.m[6] = m.m[7] = m.m[8] = m.m[9] = m.m[11] = m.m[12] = m.m[13] = m.m[14] = 0;
	m.m[0] = m.m[5] = m.m[10] = m.m[15] = 1;

	return m;
}

AXELYNX_API axelynx::mat4 Make(const axelynx::mat3& orient, const axelynx::vec3& position)
{
	axelynx::mat4 m;

	for(int i=0;i<3;++i)
	{
		for(int j=0;i<3;++i)
		{
			m(i,j) = orient(i,j);
		}
	}

	m.m[12] = position.x;
	m.m[13] = position.y;
	m.m[14] = position.z;

	m.m[15] = 1.0f;

	return m;
}

AXELYNX_API axelynx::mat4 Make(const axelynx::mat3& orient, const axelynx::vec3& position, const axelynx::vec3& scale)
{
	axelynx::mat4 m = axelynx::mat4::Identity();
	return m;
}

AXELYNX_API axelynx::mat4 Make(const axelynx::mat3& orient, const axelynx::vec3& position, float scale)
{
	axelynx::mat4 m = axelynx::mat4::Identity();
	return m;
}

AXELYNX_API axelynx::mat4 Make(const axelynx::quat& orient, const axelynx::vec3& position)
{
	axelynx::mat4 m= axelynx::mat4::Identity();
	return m;
}

AXELYNX_API axelynx::mat4 axelynx::mat4::Make(const axelynx::quat& orient, const axelynx::vec3& position, const axelynx::vec3& scale)
{
	axelynx::mat4 mat= axelynx::mat4::Identity();
	axelynx::mat4::Make(mat,orient,position,scale);	
	return mat;
}

AXELYNX_API void axelynx::mat4::Make(axelynx::mat4& into,const axelynx::quat& orient, const axelynx::vec3& position, const axelynx::vec3& scale)
{

  into[ 0] = (1.0f - 2.0f * ( orient.y * orient.y + orient.z * orient.z )) * scale.x;
  into[ 1] = (2.0f * (orient.x * orient.y + orient.z * orient.w)) * scale.x;
  into[ 2] = (2.0f * (orient.x * orient.z - orient.y * orient.w)) * scale.x;
  into[3] = 0;

  // Вторая сторока
  into[ 4] = (2.0f * ( orient.x * orient.y - orient.z * orient.w )) * scale.y;
  into[ 5] = (1.0f - 2.0f * ( orient.x * orient.x + orient.z * orient.z )) * scale.y;
  into[ 6] = (2.0f * (orient.z * orient.y + orient.x * orient.w )) * scale.y;
  into[7] = 0;

  // Третья строка
  into[ 8] = (2.0f * ( orient.x * orient.z + orient.y * orient.w )) * scale.z;
  into[ 9] = (2.0f * ( orient.y * orient.z - orient.x * orient.w )) * scale.z;
  into[10] = (1.0f - 2.0f * ( orient.x * orient.x + orient.y * orient.y )) * scale.z;
  into[11] = 0;

  into[12] = position.x;
  into[13] = position.y;
  into[14] = position.z;
  into[15] = 1.0;
}

AXELYNX_API axelynx::mat4 Make(const axelynx::quat& orient, const axelynx::vec3& position, float scale)
{
	axelynx::mat4 m= axelynx::mat4::Identity();
	return m;
}

AXELYNX_API axelynx::mat4 axelynx::mat4::Perspective(float fov, float aspect, float znear, float zfar)
{
	float f = 1 / tanf(fov / 2.0f);
	float A = (zfar + znear) / (znear - zfar);
	float B = (2 * zfar * znear) / (znear - zfar);
	
	axelynx::mat4 m= axelynx::mat4::Identity();

	m[0] = f / aspect;
	m[5] = f;
	m[10] = A;
	m[14] = B;
	m[11] = -1;
	m[15] = 0;

	return m;
}

AXELYNX_API axelynx::mat4 axelynx::mat4::Ortho(float left, float right, float top, float bottom, float znear, float zfar)
{
	float tx = - (right + left) / (right - left);
	float ty = - (top + bottom) / (top - bottom);
	float tz = - (zfar + znear) / (zfar - znear);
	
	axelynx::mat4 m= axelynx::mat4::Identity();

	m[0] = 2.0f / (right - left);
	m[12] = tx;
	m[5] = 2 / (top - bottom);
	m[13] = ty;
	m[10] = -2 / (zfar - znear);
	m[14] = tz;

	return m;
}

AXELYNX_API axelynx::mat4 axelynx::mat4::operator*(const mat4& B) const
{
	//axelynx::mat4 m;

	//m[ 0] = (*this)[ 0] * B[ 0] + (*this)[ 1] * B[ 4] + (*this)[ 2] * B[ 8] + (*this)[ 3] * B[12];
	//m[ 1] = (*this)[ 0] * B[ 1] + (*this)[ 1] * B[ 5] + (*this)[ 2] * B[ 9] + (*this)[ 3] * B[13];
	//m[ 2] = (*this)[ 0] * B[ 2] + (*this)[ 1] * B[ 6] + (*this)[ 2] * B[10] + (*this)[ 3] * B[14];
	//m[ 3] = (*this)[ 0] * B[ 3] + (*this)[ 1] * B[ 7] + (*this)[ 2] * B[11] + (*this)[ 3] * B[15];
	//m[ 4] = (*this)[ 4] * B[ 0] + (*this)[ 5] * B[ 4] + (*this)[ 6] * B[ 8] + (*this)[ 7] * B[12];
	//m[ 5] = (*this)[ 4] * B[ 1] + (*this)[ 5] * B[ 5] + (*this)[ 6] * B[ 9] + (*this)[ 7] * B[13];
	//m[ 6] = (*this)[ 4] * B[ 2] + (*this)[ 5] * B[ 6] + (*this)[ 6] * B[10] + (*this)[ 7] * B[14];
	//m[ 7] = (*this)[ 4] * B[ 3] + (*this)[ 5] * B[ 7] + (*this)[ 6] * B[11] + (*this)[ 7] * B[15];
	//m[ 8] = (*this)[ 8] * B[ 0] + (*this)[ 9] * B[ 4] + (*this)[10] * B[ 8] + (*this)[11] * B[12];
	//m[ 9] = (*this)[ 8] * B[ 1] + (*this)[ 9] * B[ 5] + (*this)[10] * B[ 9] + (*this)[11] * B[13];
	//m[10] = (*this)[ 8] * B[ 2] + (*this)[ 9] * B[ 6] + (*this)[10] * B[10] + (*this)[11] * B[14];
	//m[11] = (*this)[ 8] * B[ 3] + (*this)[ 9] * B[ 7] + (*this)[10] * B[11] + (*this)[11] * B[15];
	//m[12] = (*this)[12] * B[ 0] + (*this)[13] * B[ 4] + (*this)[14] * B[ 8] + (*this)[15] * B[12];
	//m[13] = (*this)[12] * B[ 1] + (*this)[13] * B[ 5] + (*this)[14] * B[ 9] + (*this)[15] * B[13];
	//m[14] = (*this)[12] * B[ 2] + (*this)[13] * B[ 6] + (*this)[14] * B[10] + (*this)[15] * B[14];
	//m[15] = (*this)[12] * B[ 3] + (*this)[13] * B[ 7] + (*this)[14] * B[11] + (*this)[15] * B[15];

	return axelynx::mat4(m0 * B.m0 + m1 * B.m4 + m2 * B.m8 + m3 * B.m12
						,m0 * B.m1 + m1 * B.m5 + m2 * B.m9 + m3 * B.m13
						,m0 * B.m2 + m1 * B.m6 + m2 * B.m10+ m3 * B.m14
						,m0 * B.m3 + m1 * B.m7 + m2 * B.m11+ m3 * B.m15
						,m4 * B.m0 + m5 * B.m4 + m6 * B.m8 + m7 * B.m12
						,m4 * B.m1 + m5 * B.m5 + m6 * B.m9 + m7 * B.m13
						,m4 * B.m2 + m5 * B.m6 + m6 * B.m10+ m7 * B.m14
						,m4 * B.m3 + m5 * B.m7 + m6 * B.m11+ m7 * B.m15
						,m8 * B.m0 + m9 * B.m4 + m10* B.m8 + m11* B.m12
						,m8 * B.m1 + m9 * B.m5 + m10* B.m9 + m11* B.m13
						,m8 * B.m2 + m9 * B.m6 + m10* B.m10+ m11* B.m14
						,m8 * B.m3 + m9 * B.m7 + m10* B.m11+ m11* B.m15
						,m12* B.m0 + m13* B.m4 + m14* B.m8 + m15* B.m12
						,m12* B.m1 + m13* B.m5 + m14* B.m9 + m15* B.m13
						,m12* B.m2 + m13* B.m6 + m14* B.m10+ m15* B.m14
						,m12* B.m3 + m13* B.m7 + m14* B.m11+ m15* B.m15);
}

AXELYNX_API axelynx::mat4 axelynx::mat4::operator*=(const mat4& B)
{
	axelynx::mat4 m;

	m[ 0] = (*this)[ 0] * B[ 0] + (*this)[ 1] * B[ 4] + (*this)[ 2] * B[ 8] + (*this)[ 3] * B[12];
	m[ 1] = (*this)[ 0] * B[ 1] + (*this)[ 1] * B[ 5] + (*this)[ 2] * B[ 9] + (*this)[ 3] * B[13];
	m[ 2] = (*this)[ 0] * B[ 2] + (*this)[ 1] * B[ 6] + (*this)[ 2] * B[10] + (*this)[ 3] * B[14];
	m[ 3] = (*this)[ 0] * B[ 3] + (*this)[ 1] * B[ 7] + (*this)[ 2] * B[11] + (*this)[ 3] * B[15];
	m[ 4] = (*this)[ 4] * B[ 0] + (*this)[ 5] * B[ 4] + (*this)[ 6] * B[ 8] + (*this)[ 7] * B[12];
	m[ 5] = (*this)[ 4] * B[ 1] + (*this)[ 5] * B[ 5] + (*this)[ 6] * B[ 9] + (*this)[ 7] * B[13];
	m[ 6] = (*this)[ 4] * B[ 2] + (*this)[ 5] * B[ 6] + (*this)[ 6] * B[10] + (*this)[ 7] * B[14];
	m[ 7] = (*this)[ 4] * B[ 3] + (*this)[ 5] * B[ 7] + (*this)[ 6] * B[11] + (*this)[ 7] * B[15];
	m[ 8] = (*this)[ 8] * B[ 0] + (*this)[ 9] * B[ 4] + (*this)[10] * B[ 8] + (*this)[11] * B[12];
	m[ 9] = (*this)[ 8] * B[ 1] + (*this)[ 9] * B[ 5] + (*this)[10] * B[ 9] + (*this)[11] * B[13];
	m[10] = (*this)[ 8] * B[ 2] + (*this)[ 9] * B[ 6] + (*this)[10] * B[10] + (*this)[11] * B[14];
	m[11] = (*this)[ 8] * B[ 3] + (*this)[ 9] * B[ 7] + (*this)[10] * B[11] + (*this)[11] * B[15];
	m[12] = (*this)[12] * B[ 0] + (*this)[13] * B[ 4] + (*this)[14] * B[ 8] + (*this)[15] * B[12];
	m[13] = (*this)[12] * B[ 1] + (*this)[13] * B[ 5] + (*this)[14] * B[ 9] + (*this)[15] * B[13];
	m[14] = (*this)[12] * B[ 2] + (*this)[13] * B[ 6] + (*this)[14] * B[10] + (*this)[15] * B[14];
	m[15] = (*this)[12] * B[ 3] + (*this)[13] * B[ 7] + (*this)[14] * B[11] + (*this)[15] * B[15];

	(*this) = m;
	return m;
}