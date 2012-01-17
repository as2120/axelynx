#include "CPak.h"
#include "CZipArchive.h"

#include "axelynx/utils/axstring.h"

AXELYNX_API axelynx::Pak* axelynx::Pak::Open(const wchar_t *filename)
{
	std::wstring ext = axelynx::utils::GetExtension(filename);

	if(ext.compare(L"zip") == 0)
	{
		return new CZipArchive(filename);
	}

	return new CPak(filename);
}

AXELYNX_API axelynx::Pak* axelynx::Pak::Load(const wchar_t *filename)
{
	std::wstring ext = axelynx::utils::GetExtension(filename);

	if(ext.compare(L"zip") == 0)
	{
		return new CZipArchive(filename);
	}

	return new CPak(filename);
}

AXELYNX_API axelynx::Pak* axelynx::Pak::Open(const char *filename)
{
	wchar_t buff[260];
	axelynx::utils::MultiByteToWideChar(filename,buff);

	std::wstring ext = axelynx::utils::GetExtension(buff);

	if(ext.compare(L"zip") == 0)
	{
		return new CZipArchive(buff);
	}

	return new CPak(buff);
}

AXELYNX_API axelynx::Pak* axelynx::Pak::Load(const char *filename)
{
	wchar_t buff[260];
	axelynx::utils::MultiByteToWideChar(filename,buff);

	std::wstring ext = axelynx::utils::GetExtension(buff);

	if(ext.compare(L"zip") == 0)
	{
		return new CZipArchive(buff);
	}

	return new CPak(buff);
}