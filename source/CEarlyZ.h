#pragma once

class CEarlyZ
{
	static bool in_pass;
	static bool on_pass;
public:
	static void StartEarlyZPass();
	static void EndEarlyZPass();

	static void StartRenderPass();

	static void EndRenderPass();

	static bool OnEarlyZPass()
	{
		return on_pass;
	}
	static bool InEarlyZPass()
	{
		return in_pass;
	}
};