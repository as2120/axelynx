#ifndef PNGIMAGEFORMAT_H
#define PNGIMAGEFORMAT_H


#include "axelynx/ImageFormat.h"

class PNGImageFormat : public axelynx::ImageFormat
{
    public:
        static PNGImageFormat* instance();

        virtual axelynx::PixMap* LoadPixMap(axelynx::File file);
        virtual bool SavePixMap(axelynx::File file, const axelynx::PixMap *pixmap);

        virtual axelynx::Texture* LoadTexture(axelynx::File file,axelynx::Texture::Desc &desc);
        virtual bool SaveTexture(axelynx::File file, const axelynx::Texture *texture);
    protected:
    private:
        PNGImageFormat();
        virtual ~PNGImageFormat();

        static PNGImageFormat * instance_;
};

#endif // PNGIMAGEFORMAT_H
