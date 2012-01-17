#ifndef DDSIMAGEFORMAT_H
#define DDSIMAGEFORMAT_H


#include "axelynx/ImageFormat.h"

class DDSImageFormat : public axelynx::ImageFormat
{
    public:
        static DDSImageFormat* instance();

        virtual axelynx::PixMap* LoadPixMap(axelynx::File file);
        virtual bool SavePixMap(axelynx::File file, const axelynx::PixMap *pixmap);

        virtual axelynx::Texture* LoadTexture(axelynx::File file,axelynx::Texture::Desc &desc);
        virtual bool SaveTexture(axelynx::File file, const axelynx::Texture *texture);
    protected:
    private:
        DDSImageFormat();
        virtual ~DDSImageFormat();

        static DDSImageFormat * instance_;
};

#endif // DDSIMAGEFORMAT_H
