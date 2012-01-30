#pragma once
#ifndef AXELYNX_GEOMETRY_H
#define AXELYNX_GEOMETRY_H

#include "File.h"
#include <string>

namespace axelynx
{
	class Geometry
	{
	public:
		virtual bool Draw() const =0;

		virtual int CountTriangles() const =0;

		virtual int CountVertices() const =0;

		virtual float GetRadius() const =0;

		virtual ~Geometry(){};

		virtual int GetBinarySize() const
		{
			return 0;
		}
		virtual int WriteBinary(File file) const
		{
			return 0;
		}
		
		virtual int RestoreFromBinary(File file)
		{
			return 0;
		}

		virtual std::wstring GetClass() const
		{
			return L"unknow";
		}
		//virtual void Bind() const = 0; //for instancing
	};
}
#endif