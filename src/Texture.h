#ifndef __Texture__
#define __Texture__


#include <iostream>
#include <string>
#include <SFML/Graphics/Image.hpp>
#include <SFML/OpenGL.hpp>
#include "RessourcesManager.h"


class Texture
{
    private:
        GLuint mTextureId;
        sf::Image *mImage;
    public:
        Texture();
        Texture(const std::string& path);
        virtual ~Texture();

        bool loadTexture(const std::string& path);
        void bind();
        GLuint getTextureId() const;
};


#endif
