#pragma once

#include "axlib.h"
#include "keystates.h"

namespace axelynx
{
	bool AXELYNX_API KeyDown(int keystate);
	bool AXELYNX_API KeyHit(int keydown);
	bool AXELYNX_API KeyUp(int keydown);
}