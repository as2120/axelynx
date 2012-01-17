#pragma once

#include "PixMap.h"
#include "Texture.h"
#include <string>
#include <map>
#include "axlib.h"
#include "File.h"

namespace axelynx
{
    class ImageFormat
    {
        public:

        virtual PixMap* LoadPixMap(axelynx::File file)=0;
		virtual bool SavePixMap(axelynx::File file, const axelynx::PixMap *pixmap)=0;
        virtual Texture* LoadTexture(axelynx::File file,Texture::Desc &desc)=0;
		virtual bool SaveTexture(axelynx::File file, const axelynx::Texture *pixmap)=0;

        static AXELYNX_API ImageFormat* FindByExt(std::wstring ext);

        protected:
        static std::map<std::wstring,ImageFormat*> AllFormats;

		virtual ~ImageFormat(){};
    };
}
