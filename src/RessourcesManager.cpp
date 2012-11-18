#include "RessourcesManager.h"

/**
 * @brief Free all allocated ressources
 */
RessourcesManager::~RessourcesManager()
{
    std::cout << "Deleting all ressources" << std::endl;
    std::map<std::string, sf::Image*>::const_iterator it;
    for(it = mImages.begin(); it != mImages.end(); it++) {
        std::cout << "Deleting image " << it->first << std::endl;
        delete it->second;
    }
}

/**
 * @brief Loads an image if it isn't already loaded
 *
 * @param path
 *        Path of the image
 *
 * @return
 *        true if a new image was loaded
 *        false if the given image was already loaded
 *
 * @throws
 *       std::exception if an error occured during loading
 */
bool RessourcesManager::loadImage(const std::string& path)
{
    if ( mImages.find( path ) != mImages.end() ) {
        std::cout << "Image " << path << " already loaded" << std::endl;
        return false;
    }
    else {
        sf::Image * img = new sf::Image();
        if(!img->loadFromFile(path))
            throw new std::runtime_error("Error: Image "+path+" loading failed!");
        else
            mImages[path] = img;
    }
}
void RessourcesManager::addImage(sf::Image *img, const std::string& id)
{
    mImages[id] = img;
}

/**
 * @brief Returns an image
 *
 * @param path
 *        The path used during image loading
 *
 * @return
 *        A pointer to the image, or NULL if the image isn't loaded
 */
sf::Image* RessourcesManager::getImage(const std::string& path) const {
    std::cout << mImages.size();
    std::map<std::string, sf::Image*>::const_iterator it =  mImages.find(path);
    if(it != mImages.end()) {
        return it->second;
    } else {
        return NULL;
    }
}
