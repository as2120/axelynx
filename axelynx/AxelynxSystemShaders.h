#pragma once

#include "Shader.h"
#include "axlib.h"

namespace axelynx
{
	class StandartShaders
	{
	public:

		class Draw
		{
		public:
			static AXELYNX_API Shader * Position();
			static AXELYNX_API Shader * PositionColor();
			static AXELYNX_API Shader * PositionUV();
			static AXELYNX_API Shader * PositionColorUV();
		};

		class Render
		{
		public:
			static AXELYNX_API Shader * Texturing();
			static AXELYNX_API Shader * PointSprite();

			static AXELYNX_API Shader * FakeLines();
			static AXELYNX_API Shader * VolumetricLines();

			static AXELYNX_API Shader * QuadSprite();

			static AXELYNX_API Shader * BumpMapping();
			static AXELYNX_API Shader * BumpMappingAdvanced();
			static AXELYNX_API Shader * BumpLightMapping();
			static AXELYNX_API Shader * OnlyNormals();

			static AXELYNX_API Shader * TexturingLighting();

			static AXELYNX_API Shader * TexturingInstanced();
			static AXELYNX_API Shader * ZPassTexturingInstanced();

			static AXELYNX_API Shader * TransformOnly();

			static AXELYNX_API Shader * VolumetricRendering();

			static AXELYNX_API Shader * Paralax();

			static AXELYNX_API Shader * MorfedMeshTexturing();
		};

		static AXELYNX_API Shader * FindByName(const char *shadername);
	};
}