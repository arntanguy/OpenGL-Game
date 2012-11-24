#ifndef __GrassEntity__
#define __GrassEntity__

#include <GL/glew.h>
#include <GL/glu.h>
#include "Entity.h"
#include "Texture.h"
#include "Debug.h"
#include "Shader.h"

class GrassEntity : public Entity
{
    private:
        Texture *mTexture;
        GLuint mDisplayListIndex;
        Shader mGrassShader;
    public:
        GrassEntity(Texture *texture, const std::string& vertexShader, const std::string &fragmentShader);
        virtual ~GrassEntity();
        bool generate();
        bool render();
};

#endif
