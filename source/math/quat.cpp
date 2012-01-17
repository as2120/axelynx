#include <axelynx/math/quat.h>
#include <cmath>
#include <cstdlib>

AXELYNX_API axelynx::quat::quat()
{
	x = 0;
	y = 0;
	z = 0;
	w = 1;
}

AXELYNX_API axelynx::quat::quat(const vec3& axis, float degrees)
{
  // Вначале конвертируем углы в радианы
  // поскольку углы в радианах
  float angle = float((degrees / 180.0f) * 3.1415f);
  // Вычислим sin( theta / 2) для оптимизации
  float result = (float)sinf( angle / 2.0f );

  // Вычисляем значение w как cos( theta / 2 )
  w = (float)cosf( angle / 2.0f );

  // Вычислим x, y и z кватерниона
  x = float(axis.x * result);
  y = float(axis.y * result);
  z = float(axis.z * result);
}

AXELYNX_API axelynx::quat::quat(const axelynx::vec3& angles)
{
	quat q1(vec3(1,0,0),angles.x);
	quat q2(vec3(0,1,0),angles.y);
	quat q3(vec3(0,0,1),angles.z);

	quat q4 = q1 * q2 * q3;

	x = q4.x;
	y = q4.y;
	z = q4.z;
	w = q4.w;
}


AXELYNX_API axelynx::quat::quat(const axelynx::mat3& rotatematrix)
{
}

axelynx::mat3 axelynx::quat::ToMat3() const
{
 // Первая строка
axelynx::mat3 mat;

  mat[ 0] = 1.0f - 2.0f * ( y * y + z * z );
  mat[ 1] = 2.0f * (x * y + z * w);
  mat[ 2] = 2.0f * (x * z - y * w);

  // Вторая сторока
  mat[ 3] = 2.0f * ( x * y - z * w );
  mat[ 4] = 1.0f - 2.0f * ( x * x + z * z );
  mat[ 5] = 2.0f * (z * y + x * w );

  // Третья строка
  mat[ 6] = 2.0f * ( x * z + y * w );
  mat[ 7] = 2.0f * ( y * z - x * w );
  mat[8] = 1.0f - 2.0f * ( x * x + y * y );

  return mat;
}

axelynx::vec3 axelynx::quat::ztransform(float value) const
{
	axelynx::vec3 result;

	result.x = (2.0f * (x * z - y * w)) * value;
	result.y = (2.0f * (z * y + x * w )) * value;
	result.z = (1.0f - 2.0f * ( x * x + y * y )) * value;

	return result;
}

axelynx::quat axelynx::quat::operator*(float other) const
{
   axelynx::quat r;

  return r.slerp(*this,other);
}

axelynx::quat axelynx::quat::operator*(axelynx::quat other) const
{
  axelynx::quat r;

  r.w = (*this).w*other.w - (*this).x*other.x - (*this).y*other.y - (*this).z*other.z;
  r.x = (*this).w*other.x + (*this).x*other.w + (*this).y*other.z - (*this).z*other.y;
  r.y = (*this).w*other.y + (*this).y*other.w + (*this).z*other.x - (*this).x*other.z;
  r.z = (*this).w*other.z + (*this).z*other.w + (*this).x*other.y - (*this).y*other.x;

  return r;
}

const axelynx::quat& axelynx::quat::operator*=(const axelynx::quat& other)
{
  axelynx::quat r;

  r.w = (*this).w*other.w - (*this).x*other.x - (*this).y*other.y - (*this).z*other.z;
  r.x = (*this).w*other.x + (*this).x*other.w + (*this).y*other.z - (*this).z*other.y;
  r.y = (*this).w*other.y + (*this).y*other.w + (*this).z*other.x - (*this).x*other.z;
  r.z = (*this).w*other.z + (*this).z*other.w + (*this).x*other.y - (*this).y*other.x;

  (*this) = r;
	return (*this);
}


axelynx::quat axelynx::quat::slerp(const axelynx::quat& qb, float t) const{
	// quaternion to return
	axelynx::quat qm;
	const axelynx::quat &qa = *this;
	// Calculate angle between them.
	float cosHalfTheta = qa.w * qb.w + qa.x * qb.x + qa.y * qb.y + qa.z * qb.z;
	// if qa=qb or qa=-qb then theta = 0 and we can return qa
	if (abs(cosHalfTheta) >= 1.0f){
		qm.w = qa.w;qm.x = qa.x;qm.y = qa.y;qm.z = qa.z;
		return qm;
	}
	// Calculate temporary values.
	float halfTheta = acos(cosHalfTheta);
	float sinHalfTheta = sqrt(1.0f - cosHalfTheta*cosHalfTheta);
	// if theta = 180 degrees then result is not fully defined
	// we could rotate around any axis normal to qa or qb
	if (fabs(sinHalfTheta) < 0.001f){ // fabs is floating point absolute
		qm.w = (qa.w * 0.5f + qb.w * 0.5f);
		qm.x = (qa.x * 0.5f + qb.x * 0.5f);
		qm.y = (qa.y * 0.5f + qb.y * 0.5f);
		qm.z = (qa.z * 0.5f + qb.z * 0.5f);
		return qm;
	}
	float ratioA = sin((1 - t) * halfTheta) / sinHalfTheta;
	float ratioB = sin(t * halfTheta) / sinHalfTheta;
	//calculate Quaternion.
	qm.w = (qa.w * ratioA + qb.w * ratioB);
	qm.x = (qa.x * ratioA + qb.x * ratioB);
	qm.y = (qa.y * ratioA + qb.y * ratioB);
	qm.z = (qa.z * ratioA + qb.z * ratioB);
	return qm;
}
