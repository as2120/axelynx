#pragma once

#include "axelynx/Statistics.h"

class CStatistics : public axelynx::Statistics
{
	static CStatistics *instance_;
public:
	virtual int TrisRendered()
	{
		int t = tris_rendered;
		//tris_rendered = 0;
		return t;
	}
	virtual int VerticesRendered()
	{
		int t = vertices_rendered;
		//vertices_rendered = 0;
		return t;
	}
	virtual int DIPs()
	{
		int t = dips;
		//dips = 0;
		return t;
	}
	virtual int ShaderBinded()
	{
		int t = shaders;
		//shaders = 0;
		return t;
	}
	virtual int TextureBinded()
	{
		int t = textures;
		//textures = 0;
		return t;
	}
	virtual int MaterialBinded()
	{
		int t = materials;
		//materials = 0;
		return t;
	}

	virtual int UniformsLoad()
	{
		int t = uniforms;
		//uniforms = 0;
		return t;
	}

	virtual int UniformsData()
	{
		int t = uniformsdata;
		//uniformsdata = 0;
		return t;
	}
	virtual int GetFPS() //int
	{
		int t = fps;
		return t;
	}


	static int tris_rendered;
	static int vertices_rendered;
	static int dips;
	static int shaders;
	static int textures;
	static int materials;
	static int uniforms;
	static int uniformsdata;

	static int fpsframe;
	static int fps;
	static int fpstime;

	static void ClearFrame();
	static CStatistics * Instance()
	{
		if(!instance_)
			instance_ = new CStatistics();
		return instance_;
	}
};