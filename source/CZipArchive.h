#pragma once

#include "axelynx/Pak.h"
#include <cstdio>

#include "libs/unzip/unzip.h"
#include "ZippedFile.h"

class CZipArchive : public axelynx::Pak
{

public:
	CZipArchive(const wchar_t *filename);

	virtual axelynx::File::FileImpl* _getFile(const char *filename);
	virtual axelynx::File::FileImpl* _getFile(const wchar_t *filename);

	virtual ~CZipArchive();

	unzFile file_;
};