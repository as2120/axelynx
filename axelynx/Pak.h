#pragma once

#include "axlib.h"
#include "File.h"

namespace axelynx
{
	class Pak
	{	
	public:
		static AXELYNX_API Pak* Open(const wchar_t *filename);
		static AXELYNX_API Pak* Load(const wchar_t *filename);

		static AXELYNX_API Pak* Open(const char *filename);
		static AXELYNX_API Pak* Load(const char *filename);
		
		virtual File::FileImpl* _getFile(const char *filename)=0;
		virtual File::FileImpl* _getFile(const wchar_t *filename)=0;

		File get(const char *filename)
		{
			return File(this,filename);
		}

		File get(const wchar_t *filename)
		{
			return File(this,filename);
		}
		virtual ~Pak(){};
	};
}