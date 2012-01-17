#pragma once

namespace axelynx
{
	class Statistics
	{
	public:
		virtual int TrisRendered()=0;
		virtual int VerticesRendered()=0;
		virtual int DIPs()=0;
		virtual int ShaderBinded()=0;
		virtual int TextureBinded()=0;
		virtual int MaterialBinded()=0;
		virtual int GetFPS()=0;

		virtual int UniformsLoad()=0;
		virtual int UniformsData()=0;
	};
}