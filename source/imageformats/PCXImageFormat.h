#pragma once


#include "axelynx/ImageFormat.h"

class PCXImageFormat : public axelynx::ImageFormat
{
    public:
        static PCXImageFormat* instance();

        virtual axelynx::PixMap* LoadPixMap(axelynx::File file);
        virtual bool SavePixMap(axelynx::File file, const axelynx::PixMap *pixmap);

        virtual axelynx::Texture* LoadTexture(axelynx::File file,axelynx::Texture::Desc &desc);
        virtual bool SaveTexture(axelynx::File file, const axelynx::Texture *texture);
    protected:
    private:
        PCXImageFormat();
        virtual ~PCXImageFormat();

        static PCXImageFormat * instance_;
};
