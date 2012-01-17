#ifndef JPEGIMAGEFORMAT_H
#define JPEGIMAGEFORMAT_H


#include "axelynx/ImageFormat.h"

class JPEGImageFormat : public axelynx::ImageFormat
{
    public:
        static JPEGImageFormat* instance();

        virtual axelynx::PixMap* LoadPixMap(axelynx::File file);
        virtual bool SavePixMap(axelynx::File file, const axelynx::PixMap *pixmap);

        virtual axelynx::Texture* LoadTexture(axelynx::File file,axelynx::Texture::Desc &desc);
        virtual bool SaveTexture(axelynx::File file, const axelynx::Texture *texture);
    protected:
    private:
        JPEGImageFormat();
        virtual ~JPEGImageFormat();

        static JPEGImageFormat * instance_;
};

#endif // JPEGIMAGEFORMAT_H
