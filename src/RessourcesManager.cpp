#include "RessourcesManager.h"
#include "Debug.h"

/**
 * @brief Free all allocated ressources
 */
RessourcesManager::~RessourcesManager()
{
    dinf << "Deleting all entities" << std::endl;
    std::map<std::string, Entity*>::const_iterator entIt;
    for(entIt = mEntities.begin(); entIt != mEntities.end(); entIt++) {
        dinf << "Deleting image " << entIt->first << std::endl;
        delete entIt->second;
    }

    dinf << "Deleting all ressources" << std::endl;
    std::map<std::string, sf::Image*>::const_iterator it;
    for(it = mImages.begin(); it != mImages.end(); it++) {
        dinf << "Deleting image " << it->first << std::endl;
        delete it->second;
    }

    dinf << "Deleting all textures" << std::endl;
    std::map<std::string, Texture*>::const_iterator texIt;
    for(texIt = mTextures.begin(); texIt != mTextures.end(); texIt++) {
        dinf << "Deleting image " << texIt->first << std::endl;
        delete texIt->second;
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
    dinf << "RessourcesManager: Image " << id << " added." << std::endl;
    mImages[id] = img;
}

Texture * RessourcesManager::loadTexture(const std::string &path)
{
    if ( mTextures.find( path ) != mTextures.end() ) {
        dinf << "Texture " << path << " already loaded" << std::endl;
        return mTextures[path];
    }
    Texture *texture = new Texture();
    if(texture->loadTexture(path)) {
        dinf << "Texture " << path << " loaded" << std::endl;
        mTextures[path] = texture;
        return texture;
    }
    return 0;
}

bool RessourcesManager::addTexture(Texture *texture, const std::string &path)
{
    if ( mTextures.find( path ) != mTextures.end() ) {
        dinf << "Texture " << path << " already loaded" << std::endl;
        return false;
    } else {
        if(texture != 0) {
            return false;
        } else {
            mTextures[path] = texture;
            return true;
        }
    }

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
    std::map<std::string, sf::Image*>::const_iterator it =  mImages.find(path);
    if(it != mImages.end()) {
        return it->second;
    } else {
        return 0;
    }
}

void RessourcesManager::addEntity(Entity *entity, const std::string& entityName)
{
    dinf << "RessourcesManager: Entity " << entityName << " added." << std::endl;
    mEntities[entityName] = entity;
}

Entity* RessourcesManager::getEntity(const std::string& name) const
{
    std::map<std::string, Entity*>::const_iterator it =  mEntities.find(name);
    if(it != mEntities.end()) {
        return it->second;
    } else {
        return 0;
    }
}
