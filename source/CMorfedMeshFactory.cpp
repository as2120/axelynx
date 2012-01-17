

#include "CMorfedMeshFactory.h"

#include <map>

std::map<std::wstring,axelynx::MorfedMeshFactory*> factories;

AXELYNX_API axelynx::MorfedMeshFactory* axelynx::MorfedMeshFactory::getByExtension(const wchar_t *ext)
{
	return factories[ext];
}

AXELYNX_API void axelynx::MorfedMeshFactory::RegisterFactory(const wchar_t *ext, axelynx::MorfedMeshFactory* factory)
{
	factories[ext] = factory;
}