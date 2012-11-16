#include "Textures.h"

Textures::Textures()
{
}

Textures::~Textures()
{
}

/**
 * @brief Add a texture to the list
 *
 * @param imgPath
 *        Path of the image to use. This image must have been loaded first through RessourcesManager
 *
 * @return
 *      true on success
 *      flase on failure (image not loaded)
 */
bool Textures::addTexture(const std::string& imgPath)
{
    sf::Image *img = RessourcesManager::getInstance().getImage(imgPath);
    if(img == NULL) return false;

    mTextures[imgPath] = img;
    return true;
}

