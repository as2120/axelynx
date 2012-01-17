
#include <axelynx/math/frustum.h>

AXELYNX_API bool axelynx::frustum::CheckPoint(const vec3& position) const
{


		if( planes[0][0] * position.x + planes[0][1] * position.y + planes[0][2] * position.z + planes[0][3] <= 0 )
         return false;

		if( planes[1][0] * position.x + planes[1][1] * position.y + planes[1][2] * position.z + planes[1][3] <= 0 )
         return false;

		if( planes[2][0] * position.x + planes[2][1] * position.y + planes[2][2] * position.z + planes[2][3] <= 0 )
         return false;

		if( planes[3][0] * position.x + planes[3][1] * position.y + planes[3][2] * position.z + planes[3][3] <= 0 )
         return false;

		if( planes[4][0] * position.x + planes[4][1] * position.y + planes[4][2] * position.z + planes[4][3] <= 0 )
         return false;
						
		if( planes[5][0] * position.x + planes[5][1] * position.y + planes[5][2] * position.z + planes[5][3] <= 0 )
         return false;

   return true;
}

AXELYNX_API bool axelynx::frustum::CheckSphere(const vec3& position, float radius) const
{
 
		if( planes[0][0] * position.x + planes[0][1] * position.y + planes[0][2] * position.z + planes[0][3] <= -radius )
         return false;

		if( planes[1][0] * position.x + planes[1][1] * position.y + planes[1][2] * position.z + planes[1][3] <= -radius )
         return false;

		if( planes[2][0] * position.x + planes[2][1] * position.y + planes[2][2] * position.z + planes[2][3] <= -radius )
         return false;

		if( planes[3][0] * position.x + planes[3][1] * position.y + planes[3][2] * position.z + planes[3][3] <= -radius )
         return false;

		if( planes[4][0] * position.x + planes[4][1] * position.y + planes[4][2] * position.z + planes[4][3] <= -radius )
         return false;
						
		if( planes[5][0] * position.x + planes[5][1] * position.y + planes[5][2] * position.z + planes[5][3] <= -radius )
         return false;

   return true;
}

AXELYNX_API bool axelynx::frustum::CheckOOB(const vec3 position[8]) const
{
	return true;
}