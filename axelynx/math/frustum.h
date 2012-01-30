#pragma once
#ifndef AXELYNX_FRUSTUM_H
#define AXELYNX_FRUSTUM_H

#include "../axlib.h"
#include "vec3.h"

namespace axelynx
{
	class AXELYNX_API frustum
	{
		float planes[6][4];

	public:
		bool FAST_CALL CheckPoint(const vec3& position) const;
		bool FAST_CALL CheckSphere(const vec3& position, float radius) const;
		bool FAST_CALL CheckOOB(const vec3 position[8]) const;

		const float& FAST_CALL operator()(int i,int j) const
		{
			return planes[i][j];
		}

		float& FAST_CALL operator()(int i,int j)
		{
			return planes[i][j];
		}
	};
}

#endif
