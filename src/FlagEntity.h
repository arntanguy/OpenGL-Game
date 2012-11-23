#ifndef __FlagEntity__
#define __FlagEntity__


#include "Entity.h"
#include "Texture.h"
#include <GL/glu.h>
#include "Debug.h"

class FlagEntity : public Entity
{
    private:
        Texture *mTexture;
        GLuint mDisplayListIndex;
        float mNbSquares = 20;
        float mWidth=80;
        float mHeight=10;

    public:
        FlagEntity(Texture *texture, float width, float height, float nbSquares);
        FlagEntity(float width, float height, float nbSquares);
        ~FlagEntity();
        virtual bool generate();
        virtual bool render();

        float getNbSquares() const;
        float getWidth() const;
        float getHeight() const;

};

#endif
