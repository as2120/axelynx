#pragma once

#include "axlib.h"
#include "math/vec2.h"

namespace axelynx
{
	int AXELYNX_API MouseX();
	int AXELYNX_API MouseY();

	vec2 AXELYNX_API MousePos();
	bool AXELYNX_API SetMousePos(const vec2& position);
	bool AXELYNX_API SetMousePos(int x, int y);
}