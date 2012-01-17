#pragma once

#include "axlib.h"

namespace axelynx
{
	class Timer
	{
	public:
		static AXELYNX_API int Millisecs();
		static AXELYNX_API float Microsecs();
		static AXELYNX_API float Delta();
		static AXELYNX_API void Wait(int millisecs);
	};

	

}