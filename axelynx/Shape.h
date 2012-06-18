#pragma once
#ifndef AXELYNX_SHAPE_H
#define AXELYNX_SHAPE_H

#include "File.h"
#include <string>


namespace axelynx
{
	class Pivot;

	class Shape
	{
	public:
	
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
			return L"shape";
		}

		virtual ~Shape()
		{
		}
		//virtual void Bind() const = 0; //for instancing
	};
}
#endif