#ifndef __Texture__
#define __Texture__


#include <iostream>
#include <string>
#include <SFML/Graphics/Image.hpp>
#include <SFML/OpenGL.hpp>
#include "RessourcesManager.h"


/**
 * @brief Manages texture loading and passing textures to opengl
 *
 * @description This class uses SFML to load images.
 * Then it creates the corresponding opengl textures.
 * It can the be bound by calling bind, which will effectively bind the texture to the
 * GL_TEXTURE_2D unit
 */
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
        bool loadTexture(sf::Image *img, const std::string& name);
        void bind();
        GLuint getTextureId() const;
        sf::Image *getImage() const;
};


#endif
