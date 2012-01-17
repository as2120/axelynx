#pragma once

#include "axelynx/ImageFormat.h"

class _7BIImageFormat : public axelynx::ImageFormat
{
    public:
        static _7BIImageFormat* instance();

        virtual axelynx::PixMap* LoadPixMap(axelynx::File file);
        virtual bool SavePixMap(axelynx::File file, const axelynx::PixMap *pixmap);

        virtual axelynx::Texture* LoadTexture(axelynx::File file,axelynx::Texture::Desc &desc);
        virtual bool SaveTexture(axelynx::File file, const axelynx::Texture *texture);
    protected:
    private:
        _7BIImageFormat();
        virtual ~_7BIImageFormat();

        static _7BIImageFormat * instance_;
};
