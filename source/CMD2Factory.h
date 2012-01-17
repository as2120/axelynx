#pragma once

#include "CMorfedMeshFactory.h"

class CMD2Factory : public axelynx::MorfedMeshFactory
{
public:
	virtual axelynx::MorfedMesh * CreateFromFile(axelynx::File file);
};
