#ifndef __Textures__
#define __Textures__

#include <map>
#include <string>
#include <SFML/Graphics/Image.hpp>
#include <SFML/OpenGL.hpp>
#include "RessourcesManager.h"

class Textures
{
    private:
        std::map<std::string, sf::Image *> mTextures;

    public:
        Textures();
        virtual ~Textures();

        bool addTexture(const std::string& imgPath);
};


#endif
