#pragma once

#include "MorfedMesh.h"
#include "File.h"
#include "axlib.h"

namespace axelynx
{
	class MorfedMeshFactory
	{
	public:
		virtual MorfedMesh * CreateFromFile(File file) = 0;
		static AXELYNX_API MorfedMeshFactory* getByExtension(const wchar_t *ext);
		static AXELYNX_API void RegisterFactory(const wchar_t *ext, MorfedMeshFactory* factory);
	};
}