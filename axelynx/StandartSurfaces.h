#pragma once

#pragma once

#include "axlib.h"

#include "Surface.h"

namespace axelynx
{
	class StandartSurfaces
	{
	public:

		static AXELYNX_API Surface * Cube(float size = 1);
		static AXELYNX_API Surface * Quad(float size = 1);
		static AXELYNX_API Surface * Sphere(float size = 1, int segments = 16);
		static AXELYNX_API Surface * GeoSphere(float size = 1, int iterations = 16);
		static AXELYNX_API Surface * Plane(float sizex = 1024, float sizez = 1024, float tile_u = 64, float tile_v = 64);
		static AXELYNX_API Surface * Grid(float sizex,float sizez, int cellsx, int cellsz,bool recalc_tangets = true);
	};
}