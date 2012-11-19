#ifndef __GrassEntity__
#define __GrassEntity__

#include "Entity.h"
#include "Texture.h"
#include <GL/glu.h>

class GrassEntity {
    private:
        Texture mTexture;
        GLuint mDisplayListIndex;
    public:
        GrassEntity(const Texture &texture);
        virtual ~GrassEntity();
        bool generate();
        bool render();
};

#endif
