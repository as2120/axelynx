#pragma once

#include "axelynx/Pak.h"
#include <cstdio>
#include "PakFile.h"

class CPak : public axelynx::Pak
{
	void *data_ptr_;
	FILE *file_;
public:
	CPak(const wchar_t *filename);

	virtual axelynx::File::FileImpl* _getFile(const char *filename);
	virtual axelynx::File::FileImpl* _getFile(const wchar_t *filename);

	virtual ~CPak();
};