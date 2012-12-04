#ifndef __SkyboxEntity__
#define __SkyboxEntity__

#include "Entity.h"
#include "Texture.h"

class SkyboxEntity : public Entity
{
    private:
        Texture *mTexture;
        float mHalfSide;

    public:
        SkyboxEntity(const std::string& texture, float halfSide);
        ~SkyboxEntity();

        virtual bool generate();
        virtual bool render();
};

#endif
