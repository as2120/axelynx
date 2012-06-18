#pragma once
#ifndef AXELYNX_BODY_H
#define AXELYNX_BODY_H

#include "File.h"
#include <string>
#include <axelynx\math\vectormath.h>

namespace axelynx
{
	class Pivot;

	class Body
	{
	public:
	
		virtual int AddPivot(Pivot *pivot)=0;
		virtual Pivot* GetPivot(int index) const =0;
		virtual int CountPivots() const =0;
		virtual int ClearPivots()=0;
		virtual void RemovePivot(int index)=0;
		virtual void RemovePivot(Pivot* pivot)=0;

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

		virtual ~Body()
		{
		}
		//virtual void Bind() const = 0; //for instancing
	};
}
#endif