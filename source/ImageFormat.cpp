

#include "axelynx/ImageFormat.h"
#include <string>
#include "axelynx/Logger.h"

std::map<std::wstring,axelynx::ImageFormat*> axelynx::ImageFormat::AllFormats; //static

axelynx::ImageFormat* axelynx::ImageFormat::FindByExt(std::wstring ext)
{
    ImageFormat * ifm = AllFormats[ext];

    if(!ifm)
	{
		std::wstring message= L"Not found image extension:"+ext;
        LOG_ERROR(message.c_str());
	}
    return ifm;
}