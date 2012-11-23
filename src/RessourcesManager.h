#ifndef __RessourcesManager__
#define __RessourcesManager__

#include <SFML/Graphics/Image.hpp>
#include <map>
#include <stdexcept>
#include <iostream>
#include "Entity.h"
#include "Texture.h"
#include "Debug.h"

class Texture;

#define R() RessourcesManager::getInstance()

/**
 * @brief Manages the ressources.
 * Each ressource is identified by an unique string (path, name...)
 */
class RessourcesManager
{
    private:
        inline explicit RessourcesManager() {}
        ~RessourcesManager();
        inline explicit RessourcesManager(RessourcesManager const&) {}
        inline RessourcesManager& operator=(RessourcesManager const&) { return *this; }

    protected:
        std::map<std::string, sf::Image*> mImages;
        std::map<std::string, Entity*> mEntities;
        std::map<std::string, Texture*> mTextures;

    public:
        inline static RessourcesManager& getInstance() {
            static RessourcesManager instance;
            return instance;
        }

        bool loadImage(const std::string& path);
        void addImage(sf::Image *, const std::string& id);
        sf::Image* getImage(const std::string& path) const;

        Texture* loadTexture(const std::string &path);
        bool addTexture(Texture* texture, const std::string &path);

        void addEntity(Entity *entity, const std::string &name);
        Entity* getEntity(const std::string& name) const;
};


#endif
