#pragma once
#include "axelynx/Shader.h"

class CShadowPass
{
	static bool shadow_pass;
	static axelynx::Shader *shadow_shader_;
public:
	static axelynx::Shader* GetShader()
	{
		return shadow_shader_;
	}

	static void SetShadowShader(axelynx::Shader *shader)
	{
		shadow_shader_ = shader;
	}

	static void StartPass()
	{
		shadow_pass = true;
	}
	static void EndPass()
	{
		shadow_pass = false;
	}

	static bool inShadowPass()
	{
		return shadow_pass;
	}
};